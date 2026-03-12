namespace Corsairs.GroupServer.Handlers.Gate

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Заглушка обработчика гильдейских команд от Gate.
type GuildCommandHandler(logger: ILogger<GuildCommandHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_CM_GULDBASE + Commands.CMD_TP_BASE // placeholder

        member _.Handle(_channel, _packet) =
            logger.LogDebug("GuildCommand (заглушка)")
            ValueTask.CompletedTask
