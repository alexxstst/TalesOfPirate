namespace Corsairs.GroupServer.Handlers.Account

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing
open Corsairs.GroupServer.Services
open Corsairs.GroupServer.Types

/// Обработчик CMD_AP_USER_LOGIN от AccountServer.
/// Перенаправляет результат обратно на GateServer.
type AccountLoginResponseHandler(
    registry: PlayerRegistry,
    logger: ILogger<AccountLoginResponseHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_AP_USER_LOGIN

        member _.Handle(channel, packet) =
            let gateChannelId = packet.Sess
            let status = packet.ReadUInt8()

            match status with
            | 1uy ->
                let accountId = packet.ReadInt32()
                let gmLevel = packet.ReadUInt8()

                // Регистрируем в реестре
                let record = PlayerRecord(
                    AccountId = accountId,
                    GateChannelId = gateChannelId)
                registry.Register(record)

                logger.LogInformation("Login OK от Account: acc:{AccId} → GateCh#{GateId}", accountId, gateChannelId)
                // TODO: Переслать результат обратно на Gate
            | _ ->
                logger.LogDebug("Login rejected (status={Status}) → GateCh#{GateId}", status, gateChannelId)
                // TODO: Переслать отказ на Gate

            ValueTask.CompletedTask
