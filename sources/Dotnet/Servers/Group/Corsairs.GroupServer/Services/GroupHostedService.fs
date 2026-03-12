namespace Corsairs.GroupServer.Services

open System
open System.Net
open System.Threading
open System.Threading.Channels
open System.Threading.Tasks
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options
open Corsairs.GroupServer.Config
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Основной BackgroundService GroupServer.
type GroupHostedService(
    config: IOptions<GroupServerConfig>,
    accountConfig: IOptions<AccountConnectionConfig>,
    logger: ILogger<GroupHostedService>,
    loggerFactory: ILoggerFactory,
    registry: PlayerRegistry,
    router: CommandRouter) =
    inherit BackgroundService()

    let cfg = config.Value
    let accCfg = accountConfig.Value

    let gateEp = IPEndPoint(IPAddress.Parse(cfg.ListenAddress), cfg.GateListenPort)
    let gameEp = IPEndPoint(IPAddress.Parse(cfg.ListenAddress), cfg.GameListenPort)
    let accountEp = IPEndPoint(IPAddress.Parse(accCfg.Address), accCfg.Port)

    let system =
        new ChannelSystemCommand<ChannelIO>(
            { Endpoints = [| gateEp; gameEp |]
              ChannelCapacity = 10000 },
            (fun (socket, handler) -> ChannelIO(socket, handler)),
            loggerFactory)

    let mutable _accountChannel: ChannelIO = null

    let rec connectToAccountServer (ct: CancellationToken) =
        task {
            while not ct.IsCancellationRequested do
                try
                    let! ch = system.ConnectAsync(accountEp, ct)
                    _accountChannel <- ch
                    logger.LogInformation("Подключён к AccountServer: {Endpoint}", accountEp)
                    return ()
                with
                | :? OperationCanceledException -> return ()
                | ex ->
                    logger.LogWarning("Ошибка подключения к AccountServer: {Error}", ex.Message)
                    try do! Task.Delay(5000, ct) with :? OperationCanceledException -> ()
        }

    override _.ExecuteAsync(ct: CancellationToken) =
        task {
            system.Start(ct)
            logger.LogInformation("GroupServer слушает Gate на {GateEp}, Game на {GameEp}", gateEp, gameEp)

            // Подключение к AccountServer (в фоне)
            Task.Run(Func<Task>(fun () -> connectToAccountServer ct :> Task), ct) |> ignore

            // Главный цикл обработки событий
            try
                while not ct.IsCancellationRequested do
                    let! event = system.ReadEventAsync()

                    match event with
                    | CommandReceived(ch, packet) ->
                        try
                            do! router.Dispatch(ch, packet)
                        finally
                            packet.Dispose()
                    | Connected ch ->
                        logger.LogInformation("Подключён: {Endpoint}", ch.RemoteEndPoint)
                    | Disconnected ch ->
                        if obj.ReferenceEquals(ch, _accountChannel) then
                            _accountChannel <- null
                            logger.LogWarning("Отключён от AccountServer")
                            Task.Run(Func<Task>(fun () -> connectToAccountServer ct :> Task), ct) |> ignore
                        else
                            logger.LogInformation("Отключён: Ch#{Id}", ch.Id)
                    | PingReceived _ -> ()
            with
            | :? OperationCanceledException -> ()
            | :? ChannelClosedException -> ()
            | ex -> logger.LogError(ex, "Ошибка в главном цикле GroupServer")

            logger.LogInformation("GroupServer остановлен")
        } :> Task

    /// Канал к AccountServer.
    member _.AccountChannel = _accountChannel

    override _.Dispose() =
        (system :> IDisposable).Dispose()
        base.Dispose()
