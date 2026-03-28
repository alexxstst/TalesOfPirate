module Corsairs.GroupServer.Services.Handlers.ChatHandlers

open System
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol

// ══════════════════════════════════════════════════════════
//  Вспомогательные функции
// ══════════════════════════════════════════════════════════

let private getPlayingState (player: PlayerRecord) =
    match player.State with
    | Playing_ p -> Some p
    | _ -> None

let private getCurrentSlot (player: PlayerRecord) =
    match player.State with
    | Playing_ p ->
        let auth = p.Auth
        if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
            Some auth.Characters[p.CurrentChaIndex]
        else None
    | _ -> None

/// Получить всех участников команды.
let private getTeamPlayers (ctx: HandlerContext) (playing: PlayerPlaying) =
    if playing.TeamId > 0 then
        match ctx.Teams.TryGetValue(playing.TeamId) with
        | true, team ->
            team.Members
            |> Map.values
            |> Seq.choose (fun m -> ctx.Registry.TryGetByGpAddr(m.GpAddr))
            |> Seq.filter (fun p -> p.IsPlaying)
            |> Seq.toArray
        | _ -> Array.empty
    else Array.empty

/// Получить всех участников гильдии (онлайн).
let private getGuildPlayers (ctx: HandlerContext) (guildId: int) =
    if guildId > 0 then
        ctx.Registry.GetAllPlayers()
        |> Seq.filter (fun p -> p.IsPlaying && p.CurrentGuildId = guildId)
        |> Seq.toArray
    else Array.empty

// ══════════════════════════════════════════════════════════
//  CP_SAY2ALL — Глобальный чат
// ══════════════════════════════════════════════════════════

let handleCpSay2All (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match getPlayingState player, getCurrentSlot player with
    | Some playing, Some chaSlot ->
        let now = Environment.TickCount64

        // Rate limiting
        if playing.WorldChatTick > 0L && now - playing.WorldChatTick < int64 cfg.WorldChatIntervalMs then
            ctx.Logger.LogDebug("CP_SAY2ALL: rate limit для {Name}", chaSlot.ChaName)
        else
            playing.WorldChatTick <- now

            // GM отправляет напрямую, обычные игроки — через GameServer (для проверки денег)
            if player.GmLevel > 0y then
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PC_SAY2ALL)
                w.WriteString(chaSlot.ChaName)
                w.WriteString(content)
                w.WriteInt64(int64 chaSlot.ChatColour)
                ctx.SendToAllClients w
            else
                // Пересылаем GameServer для проверки золота
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PM_SAY2ALL)
                w.WriteInt64(int64 chaSlot.ChaId)
                w.WriteString(content)
                w.WriteInt64(int64 playing.ChatMoney)
                ctx.SendToSingleClient player w
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_SAY2TRADE — Торговый чат
// ══════════════════════════════════════════════════════════

let handleCpSay2Trade (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match getPlayingState player, getCurrentSlot player with
    | Some playing, Some chaSlot ->
        let now = Environment.TickCount64
        if playing.TradeChatTick > 0L && now - playing.TradeChatTick < int64 cfg.TradeChatIntervalMs then
            ctx.Logger.LogDebug("CP_SAY2TRADE: rate limit для {Name}", chaSlot.ChaName)
        else
            playing.TradeChatTick <- now

            if player.GmLevel > 0y then
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PC_SAY2TRADE)
                w.WriteString(chaSlot.ChaName)
                w.WriteString(content)
                w.WriteInt64(int64 chaSlot.ChatColour)
                ctx.SendToAllClients w
            else
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PM_SAY2TRADE)
                w.WriteInt64(int64 chaSlot.ChaId)
                w.WriteString(content)
                w.WriteInt64(int64 playing.TradeChatMoney)
                ctx.SendToSingleClient player w
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_SAY2YOU — Личное сообщение (whisper)
// ══════════════════════════════════════════════════════════

let handleCpSay2You (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetName = packet.ReadString()
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot ->
        if chaSlot.ChaName = targetName then ()
        else

        match ctx.Registry.TryGetByChaName(targetName) with
        | None ->
            ctx.Logger.LogDebug("CP_SAY2YOU: {Name} не найден", targetName)
        | Some target when not target.IsPlaying ->
            ctx.Logger.LogDebug("CP_SAY2YOU: {Name} не в игре", targetName)
        | Some target ->
            match getPlayingState target with
            | Some tp when tp.RefuseToMe ->
                ctx.Logger.LogDebug("CP_SAY2YOU: {Name} блокирует сообщения", targetName)
            | _ ->
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PC_SAY2YOU)
                w.WriteString(chaSlot.ChaName)
                w.WriteString(targetName)
                w.WriteString(content)
                w.WriteInt64(int64 chaSlot.ChatColour)

                // Отправляем обоим
                ctx.SendToSingleClient player (w.Clone())
                ctx.SendToSingleClient target (w.Clone())

// ══════════════════════════════════════════════════════════
//  CP_SAY2TEM — Чат команды
// ══════════════════════════════════════════════════════════

let handleCpSay2Tem (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match getPlayingState player, getCurrentSlot player with
    | Some playing, Some chaSlot ->
        let teamPlayers = getTeamPlayers ctx playing
        if teamPlayers.Length > 0 then
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_PC_SAY2TEM)
            w.WriteInt64(int64 chaSlot.ChaId)
            w.WriteString(content)
            w.WriteInt64(int64 chaSlot.ChatColour)
            ctx.SendToClients teamPlayers w
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_SAY2GUD — Чат гильдии
// ══════════════════════════════════════════════════════════

let handleCpSay2Gud (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        let guildPlayers = getGuildPlayers ctx chaSlot.GuildId
        if guildPlayers.Length > 0 then
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_PC_SAY2GUD)
            w.WriteString(chaSlot.ChaName)
            w.WriteString(content)
            w.WriteInt64(int64 chaSlot.ChatColour)
            ctx.SendToClients guildPlayers w

// ══════════════════════════════════════════════════════════
//  CP_GM1SAY — GM-команда (broadcast)
// ══════════════════════════════════════════════════════════

let handleCpGm1Say (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let content = packet.ReadString()
    if player.GmLevel <= 0y then ()
    else
        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_GM1SAY)
        w.WriteString(player.CurrentChaName)
        w.WriteString(content)
        ctx.SendToAllClients w
        ctx.Logger.LogInformation("CP_GM1SAY: {Name}: {Msg}", player.CurrentChaName, content)

// ══════════════════════════════════════════════════════════
//  CP_GM1SAY1 — GM системное сообщение
// ══════════════════════════════════════════════════════════

let handleCpGm1Say1 (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let content = packet.ReadString()
    // Клиент не отправляет setNum/color — в C++ они читаются только для серверного вызова (ply==NULL).
    // Для клиентского вызова: setNum=1, color=chatColour игрока.
    if player.GmLevel <= 0y then ()
    else
        let chatColour =
            match player.State with
            | Playing_ p ->
                let auth = p.Auth
                if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
                    int auth.Characters[p.CurrentChaIndex].ChatColour
                else 0
            | _ -> 0

        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_GM1SAY1)
        w.WriteString(content)
        w.WriteInt64(1L) // setNum (всегда 1 для клиентского вызова)
        w.WriteInt64(int64 chatColour)
        ctx.SendToAllClients w

// ══════════════════════════════════════════════════════════
//  CP_SESS_CREATE — Создание чат-сессии
// ══════════════════════════════════════════════════════════

let handleCpSessCreate (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let chaNum = int (packet.ReadInt64())

    match getPlayingState player, getCurrentSlot player with
    | Some playing, Some chaSlot ->
        if playing.ChatSessionCount >= 10 then
            ctx.Logger.LogDebug("CP_SESS_CREATE: лимит сессий для {Name}", chaSlot.ChaName)
        else
            let sessId = ctx.AllocateSessionId()
            let mutable participants = [ (player.GpAddr, player.GpAddr) ]
            let mutable failedName = ""

            for _ in 1 .. chaNum do
                let targetName = packet.ReadString()
                if String.IsNullOrEmpty(failedName) then
                    match ctx.Registry.TryGetByChaName(targetName) with
                    | Some target when target.IsPlaying && target.GpAddr <> player.GpAddr ->
                        match getPlayingState target with
                        | Some tp when not tp.RefuseSess ->
                            participants <- (target.GpAddr, target.GpAddr) :: participants
                        | _ ->
                            failedName <- targetName
                    | _ ->
                        failedName <- targetName

            if not (String.IsNullOrEmpty(failedName)) then
                // Ошибка — одна из целей недоступна
                let mutable w = WPacket(64)
                w.WriteCmd(Commands.CMD_PC_SESS_CREATE)
                w.WriteInt64(0L)
                w.WriteString(failedName)
                ctx.SendToSingleClient player w
            else
                // Создаём сессию
                let sessData =
                    { Id = sessId
                      Players = participants |> List.map (fun (k, v) -> (k, v)) |> Map.ofList }
                ctx.ChatSessions.TryAdd(sessId, sessData) |> ignore

                // Обновляем сессии у участников
                for (gpAddr, _) in participants do
                    match ctx.Registry.TryGetByGpAddr(gpAddr) with
                    | Some p ->
                        match getPlayingState p with
                        | Some pp ->
                            pp.ChatSessions <- Array.append pp.ChatSessions [| sessId |]
                            pp.ChatSessionCount <- pp.ChatSessionCount + 1
                        | None -> ()
                    | None -> ()

                // Формируем ответ (count-first: sessId, count, участники)
                let mutable w = WPacket(512)
                w.WriteCmd(Commands.CMD_PC_SESS_CREATE)
                w.WriteInt64(int64 sessId)
                w.WriteInt64(int64 participants.Length) // count-first
                w.WriteInt64(int64 chaSlot.ChaId)
                w.WriteString(chaSlot.ChaName)
                w.WriteString(chaSlot.Motto)
                w.WriteInt64(int64 chaSlot.Icon)
                for (gpAddr, _) in participants do
                    if gpAddr <> player.GpAddr then
                        match ctx.Registry.TryGetByGpAddr(gpAddr), ctx.Registry.TryGetByGpAddr(gpAddr) with
                        | Some p, _ ->
                            match getCurrentSlot p with
                            | Some slot ->
                                w.WriteInt64(int64 slot.ChaId)
                                w.WriteString(slot.ChaName)
                                w.WriteString(slot.Motto)
                                w.WriteInt64(int64 slot.Icon)
                            | None -> ()
                        | _ -> ()

                // Отправляем всем участникам
                for (gpAddr, _) in participants do
                    match ctx.Registry.TryGetByGpAddr(gpAddr) with
                    | Some p -> ctx.SendToSingleClient p (w.Clone())
                    | None -> ()

                ctx.Logger.LogDebug("CP_SESS_CREATE: сессия {Id} создана {Name}", sessId, chaSlot.ChaName)
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_SESS_SAY — Сообщение в чат-сессии
// ══════════════════════════════════════════════════════════

let handleCpSessSay (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let sessId = uint32 (packet.ReadInt64())
    let content = packet.ReadString()
    if String.IsNullOrEmpty(content) || content.Length > 200 then ()
    else

    match ctx.ChatSessions.TryGetValue(sessId) with
    | false, _ -> ()
    | true, sess ->
        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_SESS_SAY)
        w.WriteInt64(int64 sessId)
        w.WriteInt64(int64 player.CurrentChaId)
        w.WriteString(content)

        let recipients =
            sess.Players
            |> Map.keys
            |> Seq.choose ctx.Registry.TryGetByGpAddr
            |> Seq.filter (fun p -> p.IsPlaying)
            |> Seq.toArray
        ctx.SendToClients recipients w

// ══════════════════════════════════════════════════════════
//  CP_SESS_ADD — Добавить в чат-сессию
// ══════════════════════════════════════════════════════════

let handleCpSessAdd (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let sessId = uint32 (packet.ReadInt64())
    let targetName = packet.ReadString()

    match ctx.ChatSessions.TryGetValue(sessId) with
    | false, _ -> ()
    | true, sess ->
        match ctx.Registry.TryGetByChaName(targetName) with
        | None -> ()
        | Some target when not target.IsPlaying -> ()
        | Some target ->
            match getPlayingState target, getCurrentSlot target with
            | Some tp, Some targetSlot when not tp.RefuseSess ->
                sess.Players <- sess.Players |> Map.add target.GpAddr target.GpAddr
                tp.ChatSessions <- Array.append tp.ChatSessions [| sessId |]
                tp.ChatSessionCount <- tp.ChatSessionCount + 1

                // Уведомить существующих участников
                let mutable wAdd = WPacket(128)
                wAdd.WriteCmd(Commands.CMD_PC_SESS_ADD)
                wAdd.WriteInt64(int64 sessId)
                wAdd.WriteInt64(int64 targetSlot.ChaId)
                wAdd.WriteString(targetSlot.ChaName)
                wAdd.WriteString(targetSlot.Motto)
                wAdd.WriteInt64(int64 targetSlot.Icon)

                let existing =
                    sess.Players
                    |> Map.keys
                    |> Seq.choose ctx.Registry.TryGetByGpAddr
                    |> Seq.filter (fun p -> p.IsPlaying && p.GpAddr <> target.GpAddr)
                    |> Seq.toArray
                ctx.SendToClients existing wAdd
            | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_SESS_LEAVE — Покинуть чат-сессию
// ══════════════════════════════════════════════════════════

let handleCpSessLeave (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let sessId = uint32 (packet.ReadInt64())

    match ctx.ChatSessions.TryGetValue(sessId) with
    | false, _ -> ()
    | true, sess ->
        sess.Players <- sess.Players |> Map.remove player.GpAddr

        // Обновляем список сессий игрока
        match getPlayingState player with
        | Some playing ->
            playing.ChatSessions <- playing.ChatSessions |> Array.filter (fun s -> s <> sessId)
            playing.ChatSessionCount <- playing.ChatSessions.Length
        | None -> ()

        let mutable w = WPacket(32)
        w.WriteCmd(Commands.CMD_PC_SESS_LEAVE)
        w.WriteInt64(int64 sessId)
        w.WriteInt64(int64 player.CurrentChaId)

        // Отправить оставшимся
        let remaining =
            sess.Players
            |> Map.keys
            |> Seq.choose ctx.Registry.TryGetByGpAddr
            |> Seq.filter (fun p -> p.IsPlaying)
            |> Seq.toArray
        if remaining.Length > 0 then
            ctx.SendToClients remaining (w.Clone())
        ctx.SendToSingleClient player w

        // Удалить пустую сессию
        if sess.Players.IsEmpty then
            ctx.ChatSessions.TryRemove(sessId) |> ignore

// ══════════════════════════════════════════════════════════
//  CP_REFUSETOME — Блокировка ЛС
// ══════════════════════════════════════════════════════════

let handleCpRefuseToMe (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let flag = byte (packet.ReadInt64())
    match getPlayingState player with
    | Some playing -> playing.RefuseToMe <- (flag <> 0uy)
    | None -> ()

// ══════════════════════════════════════════════════════════
//  MP_SAY2ALL — Глобальный чат от GameServer (ответ)
// ══════════════════════════════════════════════════════════

let handleMpSay2All (ctx: HandlerContext) (packet: IRPacket) =
    let success = byte (packet.ReadInt64())
    if success <> 0uy then
        let chaName = packet.ReadString()
        let content = packet.ReadString()

        // Найти игрока для цвета
        let chatColour =
            match ctx.Registry.TryGetByChaName(chaName) with
            | Some p ->
                match getCurrentSlot p with
                | Some slot -> slot.ChatColour
                | None -> 0u
            | None -> 0u

        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_SAY2ALL)
        w.WriteString(chaName)
        w.WriteString(content)
        w.WriteInt64(int64 chatColour)
        ctx.SendToAllClients w

// ══════════════════════════════════════════════════════════
//  MP_SAY2TRADE — Торговый чат от GameServer (ответ)
// ══════════════════════════════════════════════════════════

let handleMpSay2Trade (ctx: HandlerContext) (packet: IRPacket) =
    let success = byte (packet.ReadInt64())
    if success <> 0uy then
        let chaName = packet.ReadString()
        let content = packet.ReadString()

        let chatColour =
            match ctx.Registry.TryGetByChaName(chaName) with
            | Some p ->
                match getCurrentSlot p with
                | Some slot -> slot.ChatColour
                | None -> 0u
            | None -> 0u

        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_SAY2TRADE)
        w.WriteString(chaName)
        w.WriteString(content)
        w.WriteInt64(int64 chatColour)
        ctx.SendToAllClients w

// ══════════════════════════════════════════════════════════
//  MP_GM1SAY — GM-команда от GameServer
// ══════════════════════════════════════════════════════════

let handleMpGm1Say (ctx: HandlerContext) (packet: IRPacket) =
    let chaName = packet.ReadString()
    let content = packet.ReadString()

    let mutable w = WPacket(512)
    w.WriteCmd(Commands.CMD_PC_GM1SAY)
    w.WriteString(chaName)
    w.WriteString(content)
    ctx.SendToAllClients w

// ══════════════════════════════════════════════════════════
//  MP_GM1SAY1 — GM системное от GameServer
// ══════════════════════════════════════════════════════════

let handleMpGm1Say1 (ctx: HandlerContext) (packet: IRPacket) =
    let content = packet.ReadString()
    let setNum = int (packet.ReadInt64())
    let color = int (packet.ReadInt64())

    let mutable w = WPacket(512)
    w.WriteCmd(Commands.CMD_PC_GM1SAY1)
    w.WriteString(content)
    w.WriteInt64(int64 setNum)
    w.WriteInt64(int64 color)
    ctx.SendToAllClients w
