namespace rec Corsairs.Platform.Network.Network

open System
open System.Net
open System.Net.Sockets
open System.Runtime.CompilerServices
open System.Threading
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Crypto
open Corsairs.Platform.Network.Protocol

/// Обёртка TCP-сокета: уникальный ID, отправка команд, статистика.
/// Наследники добавляют бизнес-логику (данные сессии и т.д.).
[<AllowNullLiteral>]
type ChannelIO(socket: Socket, handler: IoHandler) =
    static let mutable _generator = 0u

    let _id = ChannelId_ (Interlocked.Increment(&_generator))
    let mutable _resetOperation = false
    let mutable _disposed = false
    let _stats = ChannelStats()
    let _remoteEndPoint = lazy (socket.RemoteEndPoint :?> IPEndPoint)

    /// Уникальный ID канала.
    member _.Id =  _id

    /// Сокет.
    member _.Socket = socket

    /// Удалённый адрес.
    member _.RemoteEndPoint = _remoteEndPoint.Value

    /// Статистика I/O.
    member _.Stats = _stats

    /// Флаг остановки операций (после ResetOperation новые send не принимаются).
    member _.IsResetOperation = Volatile.Read(&_resetOperation)

    /// Пометить канал как остановленный.
    member _.ResetOperation() = Volatile.Write(&_resetOperation, true)

    /// Шифрование исходящего пакета перед отправкой. По умолчанию — passthrough.
    /// Наследники перегружают для AES-шифрования (PlayerChannelIO).
    /// При шифровании: создаёт новый WPacket, оригинал Dispose'ится внутри.
    abstract EncryptOutgoing: WPacket -> WPacket
    default _.EncryptOutgoing(packet) = packet

    /// Расшифровка входящего пакета после приёма. По умолчанию — passthrough.
    /// Наследники перегружают для AES-дешифровки (PlayerChannelIO).
    /// При дешифровке: возвращает новый IRPacket, вызывающий должен Dispose'ить.
    abstract DecryptIncoming: IRPacket -> IRPacket
    default _.DecryptIncoming(packet) = packet

    /// Отправить WPacket. Перед отправкой вызывается EncryptOutgoing.
    /// Владение буфером передаётся — после вызова пакет нельзя использовать.
    member this.SendPacket(packet: WPacket) =
        if not this.IsResetOperation then
            Console.WriteLine($"Decrypted packet({packet}): {packet.ToArray().ToHexString()}")
            let encrypted = this.EncryptOutgoing(packet)
            Console.WriteLine($"Encrypted packet({encrypted}): {encrypted.ToArray().ToHexString()}")
            handler.DoSend(_id, encrypted)
        else
            packet.Dispose()

    /// Переслать пакет (создаёт WPacket-копию из IRPacket).
    member this.ForwardPacket(packet: IRPacket) =
        if not this.IsResetOperation then
            this.SendPacket(WPacket(packet))

    member this.SendPing() =
        if not this.IsResetOperation then
            handler.SendPing(_id)

    /// Закрыть сокет. Потокобезопасно.
    [<MethodImpl(MethodImplOptions.Synchronized)>]
    member _.Close() =
        if not _disposed then
            Volatile.Write(&_resetOperation, true)

            try
                socket.Close()
            with _ ->
                ()

            _disposed <- true

    /// Закрыт ли канал.
    member _.IsDisposed = Volatile.Read(&_disposed)

    [<MethodImpl(MethodImplOptions.Synchronized)>]
    override _.ToString() =
        if _disposed then $"Channel#{_id} [disposed]" else $"Channel#{_id} {_remoteEndPoint.Value}"

/// Интерфейс обработчика I/O-операций.
/// Реализация — IoHandlerImpl.
[<AllowNullLiteral>]
type IoHandler =
    abstract DoSend: channelId: ChannelId * packet: WPacket -> unit
    abstract SendPing: channelId: ChannelId -> unit
    inherit IDisposable
