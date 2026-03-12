namespace Corsairs.Platform.Network.Network

open System
open System.IO.Pipelines
open System.Net
open System.Net.Sockets
open System.Threading
open System.Threading.Tasks
open Corsairs.Platform.Network.Crypto
open Corsairs.Platform.Network.Protocol

/// Состояние соединения.
type ConnectionState =
    | Connected     = 0
    | Disconnecting = 1
    | Disconnected  = 2

/// Обёртка одного TCP-соединения с System.IO.Pipelines.
/// Обеспечивает zero-copy чтение/запись пакетов и поддержку криптографии.
type PipelineConnection(socket: Socket, connectionId: int) =

    let _pipe = Pipe(PipeOptions(
        pauseWriterThreshold = 65536L,
        resumeWriterThreshold = 32768L,
        useSynchronizationContext = false
    ))

    let _sendSemaphore = new SemaphoreSlim(1, 1)
    let mutable _state = ConnectionState.Connected
    let mutable _crypto: ICryptoMiddleware = NoCryptoMiddleware() :> ICryptoMiddleware
    let mutable _userData: obj = null
    let _disconnectedEvent = new TaskCompletionSource<unit>(TaskCreationOptions.RunContinuationsAsynchronously)
    let _remoteEndPoint = socket.RemoteEndPoint

    /// Уникальный ID соединения.
    member _.Id = connectionId

    /// Удалённый адрес.
    member _.RemoteEndPoint = _remoteEndPoint

    /// Текущее состояние.
    member _.State = _state

    /// Пользовательские данные (например, сессия игрока).
    member _.UserData
        with get() = _userData
        and set v = _userData <- v

    /// Установить криптографический middleware.
    member _.SetCrypto(crypto: ICryptoMiddleware) =
        _crypto <- crypto

    /// Задача, завершающаяся при отключении.
    member _.WhenDisconnected = _disconnectedEvent.Task

    /// Цикл чтения: Socket → Pipe → обработка пакетов.
    /// onPacket вызывается для каждого полного пакета (данные без length prefix).
    member this.StartReceiving(onPacket: PipelineConnection -> ReadOnlyMemory<byte> -> ValueTask, ct: CancellationToken) =
        task {
            let writer = _pipe.Writer
            let reader = _pipe.Reader

            // Задача заполнения Pipe из Socket
            let fillTask = task {
                try
                    while _state = ConnectionState.Connected && not ct.IsCancellationRequested do
                        let memory = writer.GetMemory(4096)
                        let! bytesRead = socket.ReceiveAsync(memory, SocketFlags.None, ct)
                        if bytesRead = 0 then
                            // Соединение закрыто удалённой стороной
                            return ()
                        writer.Advance(bytesRead)
                        let! flushResult = writer.FlushAsync(ct)
                        if flushResult.IsCompleted then
                            return ()
                with
                | :? OperationCanceledException -> ()
                | :? SocketException -> ()
                | :? ObjectDisposedException -> ()

                writer.Complete() |> ignore
            }

            // Задача чтения пакетов из Pipe
            let readTask = task {
                try
                    let mutable continueReading = true
                    while continueReading do
                        let! readResult = reader.ReadAsync(ct)
                        let mutable buffer = readResult.Buffer

                        let mutable keepParsing = true
                        while keepParsing do
                            match PacketCodec.tryReadPacket &buffer with
                            | Packet packetData ->
                                // Дешифровать CMD+payload (SESS не шифруется)
                                let data =
                                    if _crypto.IsActive then
                                        let sessBytes = packetData.Slice(0, 4)
                                        let encrypted = packetData.Slice(4)
                                        let decLen = encrypted.Length - _crypto.Overhead
                                        if decLen < 0 then packetData
                                        else
                                            let result = Array.zeroCreate(4 + decLen)
                                            sessBytes.Span.CopyTo(result.AsSpan())
                                            let written = _crypto.Decrypt(encrypted.Span, Memory<byte>(result, 4, decLen))
                                            if written < 0 then packetData
                                            else ReadOnlyMemory(result)
                                    else
                                        packetData
                                do! onPacket this data
                            | NeedMoreData ->
                                keepParsing <- false

                        reader.AdvanceTo(buffer.Start, buffer.End)

                        if readResult.IsCompleted then
                            continueReading <- false
                with
                | :? OperationCanceledException -> ()
                | :? ObjectDisposedException -> ()

                reader.Complete() |> ignore
            }

            // Запустить обе задачи
            let! _ = Task.WhenAll(fillTask, readTask)

            // Пометить как отключён
            _state <- ConnectionState.Disconnected
            this.Close()
            _disconnectedEvent.TrySetResult(()) |> ignore
        }

    /// Отправить готовый кадр (полный: [length][cmd][payload]).
    /// Используйте WPacket.GetPacketMemory() для получения полного кадра.
    member _.SendFrameAsync(frame: ReadOnlyMemory<byte>, ct: CancellationToken) =
        task {
            if _state <> ConnectionState.Connected then
                return false
            else

                do! _sendSemaphore.WaitAsync(ct)
                let mutable result = false
                try
                    let bytesToSend =
                        if _crypto.IsActive then
                            // Шифруем CMD+payload (после size+SESS=6 байт), SESS не шифруется
                            let dataLen = frame.Length - 6
                            let encLen = dataLen + _crypto.Overhead
                            let newFrame = Array.zeroCreate<byte>(6 + encLen)
                            frame.Span.Slice(0, 6).CopyTo(newFrame.AsSpan())
                            let written = _crypto.Encrypt(frame.Span.Slice(6), Memory<byte>(newFrame, 6, encLen))
                            if written < 0 then frame
                            else
                                System.Buffers.Binary.BinaryPrimitives.WriteUInt16BigEndian(
                                    newFrame.AsSpan(), uint16 newFrame.Length)
                                ReadOnlyMemory(newFrame)
                        else
                            frame

                    let mutable sent = 0
                    let mutable ok = true
                    while ok && sent < bytesToSend.Length do
                        let! n = socket.SendAsync(
                            bytesToSend.Slice(sent),
                            SocketFlags.None, ct)
                        if n = 0 then
                            ok <- false
                        else
                            sent <- sent + n
                    result <- ok
                with
                | :? SocketException -> result <- false
                | :? ObjectDisposedException -> result <- false
                _sendSemaphore.Release() |> ignore
                return result
        }

    /// Отправить пакет из WPacket (WrittenMemory = полный кадр).
    member this.SendPacketAsync(writer: WPacket, ct: CancellationToken) =
        this.SendFrameAsync(writer.GetPacketMemory(), ct)

    /// Закрыть соединение.
    member _.Close() =
        if _state <> ConnectionState.Disconnected then
            _state <- ConnectionState.Disconnecting
        try socket.Shutdown(SocketShutdown.Both) with _ -> ()
        try socket.Close() with _ -> ()
        _state <- ConnectionState.Disconnected
        _disconnectedEvent.TrySetResult(()) |> ignore

    interface IDisposable with
        member this.Dispose() =
            this.Close()
            _sendSemaphore.Dispose()
            socket.Dispose()
