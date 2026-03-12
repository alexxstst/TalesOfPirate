namespace Corsairs.AccountServer.Handlers

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Protocol.Routing
open Corsairs.AccountServer.Services

/// Обработчик CMD_PA_USER_LOGIN от GroupServer.
/// Проверяет учётные данные и отвечает CMD_AP_USER_LOGIN.
type UserLoginHandler(auth: AuthService, logger: ILogger<UserLoginHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_PA_USER_LOGIN

        member _.Handle(channel, packet) =
            ValueTask(task {
                let sess = packet.Sess
                let username = packet.ReadString()
                let password = packet.ReadString()

                logger.LogDebug("Login запрос: {Username}", username)

                let! result = auth.ValidateLogin(username, password)

                let mutable w = WPacket(128)
                w.WriteCmd(Commands.CMD_AP_USER_LOGIN)
                w.WriteSess(sess)

                match result with
                | AuthSuccess(accountId, gmLevel) ->
                    w.WriteUInt8(1uy) // success
                    w.WriteInt32(accountId)
                    w.WriteUInt8(gmLevel)
                    logger.LogInformation("Login OK: {Username} (ID:{Id})", username, accountId)
                | AuthBanned reason ->
                    w.WriteUInt8(2uy) // banned
                    w.WriteString(reason)
                    logger.LogInformation("Login Banned: {Username}", username)
                | AuthInvalidCredentials ->
                    w.WriteUInt8(3uy) // bad password
                    logger.LogInformation("Login BadPass: {Username}", username)
                | AuthAccountNotFound ->
                    w.WriteUInt8(4uy) // not found
                    logger.LogInformation("Login NotFound: {Username}", username)
                | AuthAlreadyOnline ->
                    w.WriteUInt8(5uy) // already online
                    logger.LogInformation("Login AlreadyOnline: {Username}", username)
                | AuthError msg ->
                    w.WriteUInt8(0uy) // error
                    w.WriteString(msg)
                    logger.LogError("Login Error: {Username}: {Msg}", username, msg)

                channel.SendPacket(w)
            })
