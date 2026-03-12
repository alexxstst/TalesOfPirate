namespace Corsairs.Platform.Network.Network

open System
open System.Net
open System.Threading
open System.Threading.Channels
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging

/// Объединённое сетевое событие для серверной системы.
[<Struct; NoComparison; NoEquality>]
type SystemEvent<'T> =
    /// Получена команда от удалённой стороны.
    | CommandReceived of channel: 'T * packet: IRPacket
    /// Новое подключение.
    | Connected of connChannel: 'T
    /// Отключение.
    | Disconnected of discChannel: 'T
    /// Получен пинг.
    | PingReceived of pingChannel: 'T

/// Конфигурация ChannelSystemCommand.
[<NoComparison; NoEquality>]
type ChannelSystemConfig =
    { /// Endpoint'ы для прослушивания.
      Endpoints: IPEndPoint[]
      /// Ёмкость внутреннего канала событий (по умолчанию 10000).
      ChannelCapacity: int }

/// Серверная система с внутренней очередью событий (BoundedChannel).
/// Все события (команды, подключения, отключения) приходят через единый канал.
/// Оборачивает DirectSystemCommand, добавляя буферизацию.
/// Команды копируются в BufferedRPacket — данные валидны до вызова Dispose.
type ChannelSystemCommand<'T when 'T :> ChannelIO>
    (
        config: ChannelSystemConfig,
        channelFactory: System.Net.Sockets.Socket * IoHandler -> 'T,
        loggerFactory: ILoggerFactory
    ) =

    let logger = loggerFactory.CreateLogger("ChannelSystemCommand")

    let direct =
        DirectSystemCommand<'T>(
            { Endpoints = config.Endpoints },
            channelFactory,
            loggerFactory
        )

    let _channel =
        Channel.CreateBounded<SystemEvent<'T>>(
            BoundedChannelOptions(config.ChannelCapacity, SingleReader = true)
        )

    do
        // Подписываемся на OnCommand — копируем DirectRPacket в BufferedRPacket для буферизации.
        direct.OnCommand.Add(fun (ch, packet) ->
            let buffered = BufferedRPacket.Create(packet.GetPacketMemory()) :> IRPacket
            if not (_channel.Writer.TryWrite(CommandReceived(ch, buffered))) then
                logger.LogWarning("Очередь событий переполнена, команда отброшена для {Channel}", ch)
                buffered.Dispose())

        direct.OnConnected.Add(fun ch ->
            _channel.Writer.TryWrite(Connected ch) |> ignore)

        direct.OnDisconnected.Add(fun ch ->
            _channel.Writer.TryWrite(Disconnected ch) |> ignore)

        direct.OnPing.Add(fun ch ->
            _channel.Writer.TryWrite(PingReceived ch) |> ignore)

    /// Прочитать следующее событие из очереди (async).
    member _.ReadEventAsync() = _channel.Reader.ReadAsync()

    /// Подключиться к удалённому endpoint (исходящее соединение).
    member _.ConnectAsync(endpoint: IPEndPoint, ct: CancellationToken) =
        direct.ConnectAsync(endpoint, ct)

    /// IoHandler (для продвинутых сценариев).
    member _.IoHandler = direct.IoHandler

    /// Платформа (для продвинутых сценариев).
    member _.Platform = direct.Platform

    /// Запустить TCP-листенер.
    member _.Start(ct: CancellationToken) = direct.Start(ct)

    /// Количество активных соединений.
    member _.ActiveCount = direct.ActiveCount

    /// Статистика операций.
    member _.UsedOperations = direct.UsedOperations
    member _.FreeOperations = direct.FreeOperations
    member _.TotalOperations = direct.TotalOperations

    /// Все активные каналы.
    member _.GetAllChannels() = direct.GetAllChannels()

    /// Отправить пинг.
    member _.SendPing(channel: 'T) = direct.SendPing(channel)

    interface IDisposable with
        member _.Dispose() =
            _channel.Writer.Complete()
            (direct :> IDisposable).Dispose()
