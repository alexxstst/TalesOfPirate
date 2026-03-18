namespace Corsairs.Platform.Network.Network

open System
open System.Net
open System.Threading
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging

/// Алиас, чтобы не конфликтовать с System.Net.Sockets.TcpListener.
type private NetListener = Corsairs.Platform.Network.Network.TcpListener

/// Конфигурация DirectSystemCommand.
[<NoComparison; NoEquality>]
type DirectSystemConfig =
    { /// Endpoint'ы для прослушивания.
      Endpoints: IPEndPoint[] }

/// Серверная система с прямой обработкой событий (без внутренней очереди).
/// Агрегирует IoHandlerImpl + NetworkPlatform + TcpListener.
/// Подписчики получают события напрямую из потока обработки IOCP.
/// OnCommand передаёт IRPacket (DirectRPacket, zero-copy) — валиден только на время обработки события.
type DirectSystemCommand<'T when 'T :> ChannelIO>
    (
        config: DirectSystemConfig,
        channelFactory: System.Net.Sockets.Socket * IoHandler -> 'T,
        loggerFactory: ILoggerFactory
    ) =

    let logger = loggerFactory.CreateLogger("DirectSystemCommand")
    let rangedPool = new RangedPool()
    let ioHandler = new IoHandlerImpl(loggerFactory, rangedPool)
    let platform = NetworkPlatform<'T>(loggerFactory.CreateLogger("NetworkPlatform"), ioHandler)
    let listener = new NetListener(loggerFactory.CreateLogger<NetListener>())

    let _onCommand = Event<'T * IRPacket>()

    do
        listener.OnAccept.Add(fun socket ->
            let ch = channelFactory (socket, ioHandler :> IoHandler)
            platform.AttachSocket(ch))

        platform.OnReceiveCommand.Add(fun (ch, packet) ->
            _onCommand.Trigger(ch, packet))

    /// Событие: получена команда. IRPacket валиден только на время обработки события.
    [<CLIEvent>]
    member _.OnCommand = _onCommand.Publish

    /// Событие: новое подключение.
    [<CLIEvent>]
    member _.OnConnected = platform.OnConnected

    /// Событие: отключение.
    [<CLIEvent>]
    member _.OnDisconnected = platform.OnDisconnected

    /// Событие: получен пинг.
    [<CLIEvent>]
    member _.OnPing = platform.OnPing

    /// Подключиться к удалённому endpoint (исходящее соединение).
    member _.ConnectAsync(endpoint: IPEndPoint, ct: CancellationToken) =
        platform.ConnectAsync(endpoint, channelFactory, ct)

    /// IoHandler (для продвинутых сценариев).
    member _.IoHandler: IoHandler = ioHandler :> IoHandler

    /// Платформа (для продвинутых сценариев).
    member _.Platform = platform

    /// Запустить TCP-листенер на сконфигурированных endpoint'ах.
    member _.Start(ct: CancellationToken) =
        listener.Start(config.Endpoints, ct)
        logger.LogInformation("DirectSystemCommand: запущен на {Endpoints}", config.Endpoints)

    /// Количество активных соединений.
    member _.ActiveCount = platform.ActiveCount

    /// Статистика операций.
    member _.UsedOperations = platform.UsedOperations
    member _.FreeOperations = platform.FreeOperations
    member _.TotalOperations = platform.TotalOperations

    /// Все активные каналы.
    member _.GetAllChannels() = platform.GetAllChannels()

    /// Отправить пинг.
    member _.SendPing(channel: 'T) = platform.SendPing(channel)

    interface IDisposable with
        member _.Dispose() =
            (platform :> IDisposable).Dispose()
            (listener :> IDisposable).Dispose()
