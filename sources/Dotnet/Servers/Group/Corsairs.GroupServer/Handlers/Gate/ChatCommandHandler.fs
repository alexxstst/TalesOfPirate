namespace Corsairs.GroupServer.Handlers.Gate

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Заглушка обработчика чат-команд.
type ChatCommandHandler(logger: ILogger<ChatCommandHandler>) =
    interface ICommandHandler with
        member _.CommandId = 6001us // CMD_CP_SAY placeholder

        member _.Handle(_channel, _packet) =
            logger.LogDebug("ChatCommand (заглушка)")
            ValueTask.CompletedTask
