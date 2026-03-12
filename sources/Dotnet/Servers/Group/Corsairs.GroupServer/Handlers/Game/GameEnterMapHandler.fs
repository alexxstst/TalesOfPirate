namespace Corsairs.GroupServer.Handlers.Game

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing

/// Заглушка обработчика событий от GameServer.
type GameEnterMapHandler(logger: ILogger<GameEnterMapHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_MP_BASE + 1us // CMD_MP_ENTERMAP placeholder

        member _.Handle(_channel, _packet) =
            logger.LogDebug("GameEnterMap (заглушка)")
            ValueTask.CompletedTask
