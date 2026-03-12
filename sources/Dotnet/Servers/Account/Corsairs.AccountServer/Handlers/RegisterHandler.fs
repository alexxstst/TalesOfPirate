namespace Corsairs.AccountServer.Handlers

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Protocol.Routing
open Corsairs.AccountServer.Services

/// Обработчик CMD_PA_REGISTER от GroupServer.
type RegisterHandler(auth: AuthService, logger: ILogger<RegisterHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_PA_REGISTER

        member _.Handle(channel, packet) =
            ValueTask(task {
                let sess = packet.Sess
                let username = packet.ReadString()
                let password = packet.ReadString()

                let! result = auth.Register(username, password)

                let mutable w = WPacket(64)
                w.WriteCmd(Commands.CMD_AP_REGISTER)
                w.WriteSess(sess)

                match result with
                | Ok accountId ->
                    w.WriteUInt8(1uy)
                    w.WriteInt32(accountId)
                    logger.LogInformation("Register OK: {Username} (ID:{Id})", username, accountId)
                | Error msg ->
                    w.WriteUInt8(0uy)
                    w.WriteString(msg)
                    logger.LogInformation("Register Failed: {Username}: {Msg}", username, msg)

                channel.SendPacket(w)
            })
