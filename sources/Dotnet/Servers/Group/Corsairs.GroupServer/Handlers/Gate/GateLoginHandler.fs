namespace Corsairs.GroupServer.Handlers.Gate

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Protocol.Routing
open Corsairs.GroupServer.Services
open Corsairs.GroupServer.Types

/// Обработчик CMD_TP_USER_LOGIN от GateServer.
/// Пробрасывает на AccountServer для проверки.
type GateLoginHandler(
    hostService: GroupHostedService,
    registry: PlayerRegistry,
    logger: ILogger<GateLoginHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_TP_USER_LOGIN

        member _.Handle(channel, packet) =
            ValueTask(task {
                let sess = packet.Sess
                let username = packet.ReadString()
                let password = packet.ReadString()
                let clientIp = packet.ReadString()

                logger.LogDebug("Login от Gate: {Username} (IP: {Ip})", username, clientIp)

                let accCh = hostService.AccountChannel
                if isNull accCh then
                    logger.LogError("AccountServer не подключён!")
                else
                    // Пробрасываем на AccountServer
                    let mutable w = WPacket(256)
                    w.WriteCmd(Commands.CMD_PA_USER_LOGIN)
                    w.WriteSess(sess) // Сохраняем sess для обратной маршрутизации
                    w.WriteString(username)
                    w.WriteString(password)

                    accCh.SendPacket(w)
            })
