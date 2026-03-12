namespace Corsairs.GroupServer.Handlers.Gate

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Заглушка обработчика команд друзей.
type FriendCommandHandler(logger: ILogger<FriendCommandHandler>) =
    interface ICommandHandler with
        member _.CommandId = 6010us // CMD_CP_FRIEND placeholder

        member _.Handle(_channel, _packet) =
            logger.LogDebug("FriendCommand (заглушка)")
            ValueTask.CompletedTask
