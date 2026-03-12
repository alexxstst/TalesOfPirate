namespace Corsairs.AccountServer.Services

open System
open System.Net
open System.Threading
open System.Threading.Channels
open System.Threading.Tasks
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options
open Corsairs.AccountServer.Config
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Основной BackgroundService AccountServer.
/// Слушает подключения от GroupServer и обрабатывает команды.
type AccountHostedService(
    config: IOptions<AccountServerConfig>,
    logger: ILogger<AccountHostedService>,
    loggerFactory: ILoggerFactory,
    router: CommandRouter) =
    inherit BackgroundService()

    let cfg = config.Value

    let system =
        new ChannelSystemCommand<ChannelIO>(
            { Endpoints = [| IPEndPoint(IPAddress.Parse(cfg.ListenAddress), cfg.ListenPort) |]
              ChannelCapacity = 10000 },
            (fun (socket, handler) -> ChannelIO(socket, handler)),
            loggerFactory)

    override _.ExecuteAsync(ct: CancellationToken) =
        task {
            system.Start(ct)
            logger.LogInformation("AccountServer слушает на {Address}:{Port}", cfg.ListenAddress, cfg.ListenPort)

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
                        logger.LogInformation("GroupServer подключён: {Endpoint}", ch.RemoteEndPoint)
                    | Disconnected ch ->
                        logger.LogInformation("GroupServer отключён: Ch#{Id}", ch.Id)
                    | PingReceived _ -> ()
            with
            | :? OperationCanceledException -> ()
            | :? ChannelClosedException -> ()
            | ex -> logger.LogError(ex, "Ошибка в главном цикле AccountServer")

            logger.LogInformation("AccountServer остановлен")
        } :> Task

    override _.Dispose() =
        (system :> IDisposable).Dispose()
        base.Dispose()
