namespace Corsairs.Platform.Network.Network

open System
open System.Net
open System.Net.Sockets
open System.Threading
open System.Threading.Tasks
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging

/// Высокоуровневая платформа для сетевого I/O.
/// Прямые события без внутренней очереди — подписчики получают данные синхронно
/// из потока обработки IOCP (zero-copy, без промежуточного BoundedChannel).
/// Generic по типу канала (для наследования ChannelIO с бизнес-данными).
type NetworkPlatform<'T when 'T :> ChannelIO>(_logger: ILogger, handler: IoHandlerImpl) =
    let _onReceiveCommand = Event<'T * IRPacket>()
    let _onConnected = Event<'T>()
    let _onDisconnected = Event<'T>()
    let _onPing = Event<'T>()

    do
        handler.OnCloseSocket.Add(fun ch ->
            let typed = ch :?> 'T
            typed.Close()
            _onDisconnected.Trigger(typed))

        handler.OnReceiveCommand.Add(fun (ch, mem) ->
            let packet = DirectRPacket(mem)
            _onReceiveCommand.Trigger(ch :?> 'T, packet))

        handler.OnReceivePing.Add(fun ch -> _onPing.Trigger(ch :?> 'T))

    /// Событие: получена команда.
    [<CLIEvent>]
    member _.OnReceiveCommand = _onReceiveCommand.Publish

    /// Событие: сокет подключён (после AttachSocket).
    [<CLIEvent>]
    member _.OnConnected = _onConnected.Publish

    /// Событие: сокет отключён.
    [<CLIEvent>]
    member _.OnDisconnected = _onDisconnected.Publish

    /// Событие: получен пинг.
    [<CLIEvent>]
    member _.OnPing = _onPing.Publish

    /// Подключить сокет к обработчику (начать приём данных).
    member _.AttachSocket(channel: 'T) =
        handler.BeginReceive(channel)
        _onConnected.Trigger(channel)

    /// Отправить пинг.
    member _.SendPing(channel: 'T) = (handler :> IoHandler).SendPing(channel.Id)

    /// Количество активных соединений.
    member _.ActiveCount = handler.ActiveCount

    /// Статистика операций.
    member _.UsedOperations = handler.UsedOperations
    member _.FreeOperations = handler.FreeOperations
    member _.TotalOperations = handler.TotalOperations

    /// Подключиться к удалённому endpoint, создать канал и начать приём данных.
    /// Канал создаётся до подключения — при ошибке соединения генерируется OnDisconnected.
    member this.ConnectAsync(endpoint: IPEndPoint, channelFactory: Socket * IoHandler -> 'T, ct: CancellationToken) : Task<'T> =
        task {
            let socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
            socket.NoDelay <- true
            let channel = channelFactory (socket, handler :> IoHandler)
            try
                do! socket.ConnectAsync(endpoint, ct)
                this.AttachSocket(channel)
            with ex ->
                _logger.LogWarning("Ошибка подключения к {Endpoint}: {Error}", endpoint, ex.Message)
                channel.Close()
                _onDisconnected.Trigger(channel)
            return channel
        }

    /// Все активные каналы.
    member _.GetAllChannels() = handler.GetAllChannels() |> Array.map (fun ch -> ch :?> 'T)

    interface IDisposable with
        member _.Dispose() = (handler :> IDisposable).Dispose()
