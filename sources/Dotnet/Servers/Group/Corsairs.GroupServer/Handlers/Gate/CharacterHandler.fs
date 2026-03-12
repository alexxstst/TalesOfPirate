namespace Corsairs.GroupServer.Handlers.Gate

open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Protocol.Routing
open Corsairs.GroupServer.Services

/// Обработчик CMD_TP_NEWCHA (создание персонажа) от GateServer.
type NewCharacterHandler(
    charService: CharacterService,
    logger: ILogger<NewCharacterHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_TP_NEWCHA

        member _.Handle(channel, packet) =
            ValueTask(task {
                let sess = packet.Sess
                let accountId = packet.ReadInt32()
                let name = packet.ReadString()
                let job = packet.ReadString()
                let lookData = packet.ReadString()

                let! result = charService.CreateCharacter(accountId, name, job, lookData)

                let mutable w = WPacket(128)
                // Ответ обратно на Gate (используем CMD в диапазоне PT)
                w.WriteCmd(2507us) // CMD_PT_NEWCHA_RESULT (custom)
                w.WriteSess(sess)

                match result with
                | Ok cha ->
                    w.WriteUInt8(1uy)
                    w.WriteInt32(cha.Id)
                    w.WriteString(cha.Name)
                | Error msg ->
                    w.WriteUInt8(0uy)
                    w.WriteString(msg)

                channel.SendPacket(w)
            })

/// Обработчик CMD_TP_DELCHA (удаление персонажа).
type DeleteCharacterHandler(
    charService: CharacterService,
    logger: ILogger<DeleteCharacterHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_TP_DELCHA

        member _.Handle(channel, packet) =
            ValueTask(task {
                let sess = packet.Sess
                let accountId = packet.ReadInt32()
                let chaId = packet.ReadInt32()

                let! success = charService.DeleteCharacter(chaId, accountId)
                logger.LogDebug("DeleteCha: {ChaId} acc:{AccId} -> {Success}", chaId, accountId, success)
            })
