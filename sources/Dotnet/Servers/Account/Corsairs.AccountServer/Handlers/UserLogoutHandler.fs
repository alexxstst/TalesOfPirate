namespace Corsairs.AccountServer.Handlers

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing
open Corsairs.AccountServer.Services

/// Обработчик CMD_PA_USER_LOGOUT от GroupServer.
type UserLogoutHandler(auth: AuthService, logger: ILogger<UserLogoutHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_PA_USER_LOGOUT

        member _.Handle(channel, packet) =
            ValueTask(task {
                let accountId = packet.ReadInt32()
                let! _ = auth.Logout(accountId)
                logger.LogDebug("Logout: ID:{Id}", accountId)
            })
