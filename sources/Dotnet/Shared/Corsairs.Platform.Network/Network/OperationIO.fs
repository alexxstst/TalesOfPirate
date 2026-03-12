namespace Corsairs.Platform.Network.Network

open System
open System.Collections.Concurrent
open System.Net.Sockets
open System.Threading
open Corsairs.Platform.Network
open Microsoft.Extensions.Logging

// ═══════════════════════════════════════════════════════════════
//  SendBuffer, SendContext, ReceiveContext, OperationState
// ═══════════════════════════════════════════════════════════════

/// Буфер отправки.
[<Struct>]
[<NoComparison>]
type SendBuffer =
    /// Буфер из RangedPool — нужно вернуть при освобождении.
    | PoolSendBuffer of sendBuf: Memory<byte>
    /// Raw byte[] (ping и т.д.) — не нужно возвращать в пул.
    | RawSendBuffer

/// Контекст операции отправки.
[<Struct>]
[<NoComparison>]
[<NoEquality>]
type SendContext =
    { SessionId: uint64
      ChannelId: ChannelId
      Continuation: (SendStatus -> unit) voption
      Buffer: SendBuffer }

/// Контекст операции приёма.
[<Struct>]
[<NoComparison>]
type ReceiveContext =
    { SessionId: uint64
      ChannelId: ChannelId
      DataLastIndex: int
      Buffer: Memory<byte> }

/// Состояние OperationIO.
[<Struct>]
[<NoComparison>]
[<NoEquality>]
type OperationState =
    /// Свободна, лежит в пуле.
    | Free
    /// Выполняется отправка.
    | Sending of send: SendContext
    /// Выполняется приём.
    | Receiving of recv: ReceiveContext

// ═══════════════════════════════════════════════════════════════
//  OperationIO — обёртка SocketAsyncEventArgs с буфером из пула
// ═══════════════════════════════════════════════════════════════

/// Расширение SocketAsyncEventArgs: хранит контекст операции
/// (канал, накопленную длину данных, callback) и буфер из RangedPool.
type OperationIO(pool: RangedPool, logger: ILogger) =
    inherit SocketAsyncEventArgs()

    static let mutable _idGenerator = 0UL

    let _objectId = Interlocked.Increment(&_idGenerator)
    let mutable _state: OperationState = Free

    let nextSessionId () =
        match _state with
        | Sending ctx -> ctx.SessionId + 1UL
        | Receiving ctx -> ctx.SessionId + 1UL
        | Free -> 1UL

    /// Уникальный ID операции.
    member _.ObjectId = _objectId

    /// Текущее состояние.
    member _.State = _state

    /// Установить continuation (только для Sending).
    member _.SetContinuation(cont: SendStatus -> unit) =
        match _state with
        | Sending ctx -> _state <- Sending { ctx with Continuation = ValueSome cont }
        | _ -> invalidOp "SetContinuation вызван не на Sending операции!"

    /// Обновить накопленную длину после успешного ReceiveAsync.
    member _.UpdateReceivedSize() =
        if base.LastOperation = SocketAsyncOperation.Receive && base.BytesTransferred > 0 then
            match _state with
            | Receiving ctx ->
                let newIdx = ctx.DataLastIndex + base.BytesTransferred

                if newIdx > ctx.Buffer.Length then
                    raise (
                        OverflowException(
                            $"Переполнение буфера! Размер:{ctx.Buffer.Length}, Индекс:{newIdx}"
                        )
                    )

                _state <- Receiving { ctx with DataLastIndex = newIdx }
            | _ -> invalidOp "UpdateReceivedSize вызван не на Receiving операции!"

    /// Сдвинуть индекс данных (при неполном пакете — остаток в начале буфера).
    member this.UpdateDataIndex(idx: int) =
        match _state with
        | Receiving ctx ->
            _state <- Receiving { ctx with DataLastIndex = idx }
            this.SetBuffer(ctx.Buffer.Slice(idx))
        | _ -> invalidOp "UpdateDataIndex вызван не на Receiving операции!"

    /// Назначить параметры для receive с буфером из пула.
    member this.AssignForReceive(channelId: ChannelId, buf: Memory<byte>) =
        _state <-
            Receiving
                { SessionId = nextSessionId ()
                  ChannelId = channelId
                  DataLastIndex = 0
                  Buffer = buf }

        this.SetBuffer(buf)

    /// Назначить параметры для send с буфером из пула.
    member this.AssignForSend
        (channelId: ChannelId, poolItem: Memory<byte>, length: int, needRelease: bool)
        =
        _state <-
            Sending
                { SessionId = nextSessionId ()
                  ChannelId = channelId
                  Continuation = ValueNone
                  Buffer = if needRelease then PoolSendBuffer poolItem else RawSendBuffer }

        this.SetBuffer(if length <> poolItem.Length then poolItem.Slice(0, length) else poolItem)

    /// Назначить параметры для send с byte[] (без пула).
    member this.AssignForRawSend(channelId: ChannelId, buffer: byte[]) =
        _state <-
            Sending
                { SessionId = nextSessionId ()
                  ChannelId = channelId
                  Continuation = ValueNone
                  Buffer = RawSendBuffer }

        this.SetBuffer(buffer, 0, buffer.Length)

    /// Назначить параметры для send с Memory<byte> (без владения буфером).
    member this.AssignForMemorySend(channelId: ChannelId, buffer: Memory<byte>) =
        _state <-
            Sending
                { SessionId = nextSessionId ()
                  ChannelId = channelId
                  Continuation = ValueNone
                  Buffer = RawSendBuffer }

        this.SetBuffer(buffer)

    /// Освободить ресурсы для возврата в пул.
    member _.ReleaseResources() =
        match _state with
        | Receiving ctx -> pool.Free(ctx.Buffer)
        | Sending ctx ->
            match ctx.Buffer with
            | PoolSendBuffer pi -> pool.Free(pi)
            | RawSendBuffer -> ()
        | Free ->
            logger.LogError(
                $"ReleaseResources вызван на уже свободной операции OperationIO#{_objectId}"
            )

        _state <- Free

    /// Вызвать OnCompleted вручную (когда SendAsync/ReceiveAsync завершился синхронно).
    member this.SetComplete() = this.OnCompleted(this)

    override this.OnCompleted(e: SocketAsyncEventArgs) =
        match _state with
        | Free -> invalidOp "OperationIO уже свободна!"
        | Receiving _ ->
            this.UpdateReceivedSize()
            base.OnCompleted(e)
        | Sending _ -> base.OnCompleted(e)

    override _.ToString() =
        match _state with
        | Free -> $"OperationIO#{_objectId} [free]"
        | Sending ctx ->
            let (ChannelId_ chId) = ctx.ChannelId
            $"OperationIO#{_objectId} send ch:{chId} sid:{ctx.SessionId}"
        | Receiving ctx ->
            let (ChannelId_ chId) = ctx.ChannelId
            $"OperationIO#{_objectId} recv ch:{chId} Recv:{ctx.DataLastIndex} Transfer:{base.BytesTransferred}"

// ═══════════════════════════════════════════════════════════════
//  OperationPool — пул OperationIO
// ═══════════════════════════════════════════════════════════════

/// Пул OperationIO для переиспользования без аллокаций.
type OperationPool(pool: RangedPool, logger: ILogger<OperationPool>) =
    let _queue = ConcurrentQueue<OperationIO>()
    let mutable _totalCreated = 0

    let _completed = Event<OperationIO>()

    let onCompleted (_: obj) (e: SocketAsyncEventArgs) = _completed.Trigger(e :?> OperationIO)

    let getOrCreate () =
        match _queue.TryDequeue() with
        | true, op -> op
        | _ ->
            Interlocked.Increment(&_totalCreated) |> ignore
            let op = new OperationIO(pool, logger)
            op.Completed.AddHandler(System.EventHandler<SocketAsyncEventArgs>(onCompleted))
            op

    /// Событие завершения операции (для IoHandler).
    [<CLIEvent>]
    member _.Completed = _completed.Publish

    /// Выделить операцию с буфером из пула (для receive, 30KB по умолчанию).
    member _.AllocateForReceive(channelId: ChannelId) =
        let op = getOrCreate ()
        let buf = pool.Allocate(30000)
        op.AssignForReceive(channelId, buf)
        op

    /// Выделить операцию с заданным буфером из пула (для send).
    member _.AllocateForSend
        (channelId: ChannelId, poolItem: Memory<byte>, length: int, needRelease: bool)
        =
        let op = getOrCreate ()
        op.AssignForSend(channelId, poolItem, length, needRelease)
        op

    /// Выделить операцию с byte[] (для ping).
    member _.AllocateForRawSend(channelId: ChannelId, buffer: byte[]) =
        let op = getOrCreate ()
        op.AssignForRawSend(channelId, buffer)
        op

    /// Выделить операцию с Memory<byte> (без владения буфером).
    member _.AllocateForMemorySend(channelId: ChannelId, buffer: Memory<byte>, length: int) =
        let op = getOrCreate ()
        op.AssignForMemorySend(channelId, buffer.Slice(0, length))
        op

    /// Вернуть операцию в пул.
    member _.Free(op: OperationIO) =
        op.ReleaseResources()
        _queue.Enqueue(op)

    member _.UsedCount = _totalCreated - _queue.Count
    member _.FreeCount = _queue.Count
    member _.TotalCount = _totalCreated
