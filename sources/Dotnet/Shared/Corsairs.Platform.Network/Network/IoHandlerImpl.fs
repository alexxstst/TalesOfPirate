namespace Corsairs.Platform.Network.Network

open System
open System.Collections.Concurrent
open System.Collections.Generic
open System.Net.Sockets
open System.Threading
open Corsairs.Platform.Network
open System.Threading.Channels
open System.Threading.Tasks
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging

/// Span-based парсер пакетов для SAEA-буферов.
/// Протокол: [2b size BE][4b SESS BE][2b CMD BE][payload].
module private PacketParser =

    /// Минимальный размер пакета (size + SESS + CMD = 8).
    [<Literal>]
    let MinPacketSize = 8

    /// Максимальный размер пакета (64 КБ).
    [<Literal>]
    let MaxPacketSize = 65535

    /// Размер пинга (2 байта: [0x00, 0x02]).
    [<Literal>]
    let PingSize = 2

    /// Распарсить одну команду из contiguous Span<byte> буфера.
    let tryParse (span: Span<byte>) : ParseResult =
        if span.Length < 2 then
            Incomplete
        else
            let totalSize = int (BinaryIO.readUInt16 span)

            if totalSize = PingSize then
                Ping
            elif totalSize < MinPacketSize then
                ParseError $"Размер пакета {totalSize} < {MinPacketSize}"
            elif totalSize > MaxPacketSize then
                ParseError $"Размер пакета {totalSize} > {MaxPacketSize}"
            elif span.Length < totalSize then
                Incomplete
            else
                ParsedCommand totalSize

/// Центральный обработчик IOCP-операций.
/// Все completions фаннелятся через BoundedChannel в единственный reader Task.
type IoHandlerImpl(loggerFactory: ILoggerFactory, rangedPool: RangedPool) as this =
    let logger = loggerFactory.CreateLogger<IoHandlerImpl>()

    let _operationPool =
        OperationPool(rangedPool, loggerFactory.CreateLogger<OperationPool>())

    let _channel =
        Channel.CreateBounded<struct (ChannelIO * OperationIO)>(
            BoundedChannelOptions(10000, SingleReader = true, AllowSynchronousContinuations = false)
        )

    let _socketSet = HashSet<ChannelIO>()
    let _socketSetLock = obj ()
    let mutable _socketCounter = 0
    let mutable _disposed = false

    let _pingBytes = [| 0uy; 2uy |]

    let _onCloseSocket = Event<ChannelIO>()
    let _onReceiveCommand = Event<ChannelIO * Memory<byte>>()
    let _onReceivePing = Event<ChannelIO>()

    let _channelMap = ConcurrentDictionary<ChannelId, ChannelIO>()

    let tryGetChannel (channelId: ChannelId) =
        match _channelMap.TryGetValue(channelId) with
        | true, ch -> ValueSome ch
        | _ -> ValueNone

    do
        _operationPool.Completed.Add(fun op ->
            match op.State with
            | Sending ctx -> tryGetChannel ctx.ChannelId
            | Receiving ctx -> tryGetChannel ctx.ChannelId
            | Free -> ValueNone
            |> function
                | ValueSome ch ->
                    if not (_channel.Writer.TryWrite(struct (ch, op))) then
                        _channel.Writer.WriteAsync(struct (ch, op)) |> ignore
                | ValueNone -> _operationPool.Free(op))

        Task.Run(
            Func<Task>(fun () ->
                task {
                    while not _disposed do
                        let! struct (ch, op) = _channel.Reader.ReadAsync()
                        this.OnCompleteOperation(ch, op)
                })
        )
        |> ignore

    // ── Внутренние методы ───────────────────────────────────────

    member private _.CloseSocket(ch: ChannelIO, op: OperationIO) =
        let removed = lock _socketSetLock (fun () -> _socketSet.Remove(ch))

        if removed then
            Interlocked.Decrement(&_socketCounter) |> ignore
            _channelMap.TryRemove(ch.Id) |> ignore
            _onCloseSocket.Trigger(ch)

        _operationPool.Free(op)

    member private _.DoReceiveNext(ch: ChannelIO, op: OperationIO) =
        match op.State with
        | Receiving ctx when ctx.Buffer.Length - ctx.DataLastIndex > 0 ->
            try
                if not (ch.Socket.ReceiveAsync(op)) then
                    op.SetComplete()
            with _ ->
                this.CloseSocket(ch, op)
        | _ ->
            logger.LogError($"Буфер receive переполнен: {ch}, {op}")
            this.CloseSocket(ch, op)

    member private _.ProcessReceiveBuffer(ch: ChannelIO, op: OperationIO) : bool =
        match op.State with
        | Receiving ctx ->
            let buf = ctx.Buffer
            let totalReceived = ctx.DataLastIndex
            let mutable startIdx = 0
            let mutable continueLoop = true
            let mutable success = true

            while continueLoop do
                let remaining = totalReceived - startIdx

                if remaining >= 2 then
                    let slice = buf.Span.Slice(startIdx, remaining)

                    match PacketParser.tryParse slice with
                    | ParseError msg ->
                        logger.LogError($"Ошибка парсинга {ch}: {msg}")
                        success <- false
                        continueLoop <- false
                    | Ping ->
                        _onReceivePing.Trigger(ch)
                        startIdx <- startIdx + PacketParser.PingSize
                    | ParsedCommand consumedBytes ->
                        ch.Stats.AddRecvCmd()
                        _onReceiveCommand.Trigger(ch, buf.Slice(startIdx, consumedBytes))
                        startIdx <- startIdx + consumedBytes
                    | Incomplete ->
                        // Компактим оставшиеся байты в начало буфера
                        if startIdx > 0 then
                            buf.Slice(startIdx, remaining).CopyTo(buf)
                        op.UpdateDataIndex(remaining)
                        continueLoop <- false
                else
                    // Компактим остаток (0 или 1 байт) в начало буфера
                    if remaining > 0 && startIdx > 0 then
                        buf.Slice(startIdx, remaining).CopyTo(buf)
                    op.UpdateDataIndex(remaining)
                    continueLoop <- false

            success
        | _ -> true

    /// Вызвать continuation отправки (если задан) и освободить ресурсы.
    member private _.InvokeSendContinuation(ctx: SendContext, status: SendStatus) =
        match ctx.Continuation with
        | ValueSome cont ->
            try
                cont status
            with ex ->
                logger.LogError(ex.ToString())
        | ValueNone -> ()

    member _.OnCompleteOperation(ch: ChannelIO, op: OperationIO) =
        match op.State with
        | Sending ctx ->
            try
                if op.SocketError <> SocketError.Success || op.BytesTransferred = 0 then
                    this.InvokeSendContinuation(ctx, SendClosing)
                    this.CloseSocket(ch, op)
                elif ch.IsResetOperation then
                    this.InvokeSendContinuation(ctx, SendClosing)
                    _operationPool.Free(op)
                else
                    ch.Stats.AddSendBytes(uint16 op.BytesTransferred)
                    this.InvokeSendContinuation(ctx, SendOk)
                    _operationPool.Free(op)
            with ex ->
                if not (ex :? ObjectDisposedException) then
                    logger.LogError(ex.ToString())

                this.InvokeSendContinuation(ctx, SendClosing)
                this.CloseSocket(ch, op)
        | Receiving ctx ->
            try
                if op.SocketError <> SocketError.Success || op.BytesTransferred = 0 then
                    this.CloseSocket(ch, op)
                elif ch.IsResetOperation then
                    _operationPool.Free(op)
                elif ctx.DataLastIndex < 2 then
                    ch.Stats.AddRecvBytes(uint64 op.BytesTransferred)
                    this.DoReceiveNext(ch, op)
                else
                    ch.Stats.AddRecvBytes(uint64 op.BytesTransferred)

                    if not (this.ProcessReceiveBuffer(ch, op)) then
                        this.CloseSocket(ch, op)
                    else
                        this.DoReceiveNext(ch, op)
            with ex ->
                if not (ex :? ObjectDisposedException) then
                    logger.LogError(ex.ToString())

                this.CloseSocket(ch, op)
        | Free -> _operationPool.Free(op)

    // ── Публичные события ───────────────────────────────────────
    [<CLIEvent>]
    member _.OnCloseSocket = _onCloseSocket.Publish

    [<CLIEvent>]
    member _.OnReceiveCommand = _onReceiveCommand.Publish

    [<CLIEvent>]
    member _.OnReceivePing = _onReceivePing.Publish

    member _.ActiveCount = Volatile.Read(&_socketCounter)
    member _.UsedOperations = _operationPool.UsedCount
    member _.FreeOperations = _operationPool.FreeCount
    member _.TotalOperations = _operationPool.TotalCount

    member _.GetAllChannels() = lock _socketSetLock (fun () -> _socketSet |> Seq.toArray)

    member _.BeginReceive(channel: ChannelIO) =
        lock _socketSetLock (fun () ->
            if not (_socketSet.Add(channel)) then
                invalidOp $"Канал {channel} уже зарегистрирован!")

        _channelMap[channel.Id] <- channel
        Interlocked.Increment(&_socketCounter) |> ignore
        let op = _operationPool.AllocateForReceive(channel.Id)

        try
            if not (channel.Socket.ReceiveAsync(op)) then
                op.SetComplete()
        with _ ->
            this.CloseSocket(channel, op)

    interface IoHandler with

        member _.DoSend(channelId, packet: WPacket) =
            match _channelMap.TryGetValue(channelId) with
            | true, ch when not ch.IsResetOperation ->
                logger.LogDebug("OUT >> {Channel} {Packet}", ch, packet)
                let op =
                    _operationPool.AllocateForMemorySend(channelId, packet.Data, packet.GetPacketSize())

                op.SetContinuation(fun _ -> packet.Dispose())

                try
                    if not (ch.Socket.SendAsync(op)) then
                        op.SetComplete()
                with _ ->
                    packet.Dispose()
                    _operationPool.Free(op)
            | _ -> packet.Dispose()

        member _.SendPing(channelId) =
            match _channelMap.TryGetValue(channelId) with
            | true, ch when not ch.IsResetOperation ->
                logger.LogDebug("OUT >> {Channel} Ping", ch)
                let op = _operationPool.AllocateForRawSend(channelId, _pingBytes)

                try
                    if not (ch.Socket.SendAsync(op)) then
                        op.SetComplete()
                with _ ->
                    _operationPool.Free(op)
            | _ -> ()

        member _.Dispose() =
            lock _socketSetLock (fun () ->
                for ch in _socketSet do
                    try
                        ch.Close()
                    with _ ->
                        ()

                _socketSet.Clear())

            _channelMap.Clear()
            _disposed <- true
