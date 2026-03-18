namespace Corsairs.GroupServer.Services

open System.Threading
open System.Threading.Tasks
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging

/// Основной BackgroundService GroupServer.
/// Оркестрирует запуск подсистем: GateServerSystem, AccountServerSystem.
type GroupHostedService
    (
        logger: ILogger<GroupHostedService>,
        gateSystem: GateServerSystem,
        accountSystem: AccountServerSystem
    ) =
    inherit BackgroundService()

    override _.ExecuteAsync ct =
        task {
            logger.LogInformation("GroupServer запускается...")

            // Связываем системы между собой
            gateSystem.SetSystems(accountSystem :> IAccountServerSystem)
            accountSystem.SetSystems(gateSystem)

            // Загрузка данных из БД при старте
            gateSystem.LoadGuildsFromDb()
            gateSystem.LoadMasterRelationsFromDb()
            gateSystem.LoadRanking()

            // Запуск TCP-листенера для GateServer (event-driven, не блокирует)
            gateSystem.Start(ct)

            // Запуск AccountServer (blocking event loop с реконнектом)
            do! accountSystem.RunAsync(ct)

            logger.LogInformation("GroupServer остановлен")
        }

    /// Система GateServer-подключений.
    member _.GateSystem = gateSystem

    /// Система AccountServer.
    member _.AccountSystem = accountSystem
