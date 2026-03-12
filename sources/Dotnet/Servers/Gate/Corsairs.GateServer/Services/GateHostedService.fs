namespace Corsairs.GateServer.Services

open System.Threading
open System.Threading.Tasks
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging

/// Основной BackgroundService GateServer.
/// Оркестрирует запуск подсистем: Client, GroupServer, GameServer.
type GateHostedService
    (
        logger: ILogger<GateHostedService>,
        clientSystem: ClientSystem,
        groupSystem: GroupServerSystem,
        gameSystem: GameServerSystem
    ) =
    inherit BackgroundService()

    override _.ExecuteAsync ct =
        task {
            logger.LogInformation("GateServer запускается...")

            // Связываем системы между собой через интерфейсы
            clientSystem.SetSystems(groupSystem :> IGroupServerSystem, gameSystem :> IGameServerSystem)
            groupSystem.SetSystems(clientSystem :> IClientSystem, gameSystem :> IGameServerSystem)
            gameSystem.SetSystems(clientSystem :> IClientSystem, groupSystem :> IGroupServerSystem)

            // Запуск event-driven систем (не блокируют)
            gameSystem.Start(ct)
            clientSystem.Start(ct)

            // Запуск GroupServer (blocking event loop с реконнектом)
            do! groupSystem.RunAsync ct

            logger.LogInformation("GateServer остановлен")
        }

    /// Система клиентских подключений.
    member _.ClientSystem = clientSystem

    /// Система подключения к GroupServer.
    member _.GroupServerSystem = groupSystem

    /// Система подключений от GameServer.
    member _.GameServerSystem = gameSystem
