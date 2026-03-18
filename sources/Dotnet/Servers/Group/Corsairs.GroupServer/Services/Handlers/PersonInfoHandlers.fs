module Corsairs.GroupServer.Services.Handlers.PersonInfoHandlers

open System
open System.Linq
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages
open Corsairs.Platform.Database

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

// ══════════════════════════════════════════════════════════
//  CP_CHANGE_PERSONINFO — Изменение персональных данных
// ══════════════════════════════════════════════════════════

let handleCpChangePersoninfo (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let motto = packet.ReadString()
    let icon = int16 (packet.ReadInt64())
    let refuseSess = byte (packet.ReadInt64())

    match getPlayingState player, getCurrentSlot player with
    | Some playing, Some chaSlot ->
        // Валидация
        if motto.Length > 50 then
            ctx.Logger.LogDebug("CP_CHANGE_PERSONINFO: девиз слишком длинный для {Name}", chaSlot.ChaName)
        elif int icon > cfg.MaxIconValue then
            ctx.Logger.LogDebug("CP_CHANGE_PERSONINFO: иконка превышает максимум для {Name}", chaSlot.ChaName)
        else
            // Обновляем in-memory
            chaSlot.Motto <- motto
            chaSlot.Icon <- icon
            playing.RefuseSess <- (refuseSess <> 0uy)

            // Ответ клиенту
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_CHANGE_PERSONINFO)
            w.WriteString(motto)
            w.WriteInt64(int64 icon)
            w.WriteInt64(int64 refuseSess)
            ctx.SendToSingleClient player w

            ctx.Logger.LogDebug("CP_CHANGE_PERSONINFO: {Name} обновил профиль", chaSlot.ChaName)
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_PING — Пинг к GroupServer
// ══════════════════════════════════════════════════════════

let handleCpPing (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let pingValue = int (packet.ReadInt64())

    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_PC_PING)
    w.WriteInt64(int64 pingValue)
    ctx.SendToSingleClient player w

// ══════════════════════════════════════════════════════════
//  Константы типов обновления (NetCommand.h)
// ══════════════════════════════════════════════════════════

// MSG_FRND_REFRESH_*
[<Literal>] let private MSG_FRND_REFRESH_START   = 1uy
[<Literal>] let private MSG_FRND_REFRESH_ADD     = 2uy
[<Literal>] let private MSG_FRND_REFRESH_ONLINE  = 4uy

// MSG_MASTER/PRENTICE_REFRESH_*
[<Literal>] let private MSG_MASTER_REFRESH_START      = 1uy
[<Literal>] let private MSG_MASTER_REFRESH_ONLINE     = 4uy
[<Literal>] let private MSG_PRENTICE_REFRESH_START    = 6uy
[<Literal>] let private MSG_PRENTICE_REFRESH_ONLINE   = 9uy

/// Получить данные персонажа (имя, девиз, иконка, онлайн-статус).
let private getCharInfo (ctx: HandlerContext) (db: GameDbContext) (chaId: int) =
    match ctx.Registry.TryGetByChaId(chaId) with
    | Some p when p.IsPlaying ->
        match p.State with
        | Playing_ pp ->
            let auth = pp.Auth
            if pp.CurrentChaIndex >= 0 && pp.CurrentChaIndex < auth.Characters.Length then
                let s = auth.Characters[pp.CurrentChaIndex]
                (s.ChaName, s.Motto, s.Icon, true)
            else ("", "", 0s, false)
        | _ -> ("", "", 0s, false)
    | _ ->
        let cha = db.Characters.Find(chaId)
        if isNull (box cha) then ("", "", 0s, false)
        else ((defaultArg (Option.ofObj cha.Name) ""),
              (defaultArg (Option.ofObj cha.Motto) ""),
              cha.IconId, false)

// ══════════════════════════════════════════════════════════
//  sendFriendsInit — CMD_PC_FRND_REFRESH (START) при входе
// ══════════════════════════════════════════════════════════

let private sendFriendsInit (ctx: HandlerContext) (player: PlayerRecord) (chaSlot: CharacterSlot) =
    try
        use scope = ctx.ScopeFactory.CreateScope()
        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

        let friendships = db.GetFriendships(chaSlot.ChaId)

        // Формат: CMD_PC_FRND_REFRESH, type=START, self, groups[]
        let mutable w = WPacket(4096)
        w.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
        w.WriteInt64(int64 MSG_FRND_REFRESH_START)

        // Self info
        w.WriteInt64(int64 chaSlot.ChaId)
        w.WriteString(chaSlot.ChaName)
        w.WriteString(chaSlot.Motto)
        w.WriteInt64(int64 chaSlot.Icon)

        // Группируем друзей по FriendGroup
        let groups =
            friendships
            |> Array.groupBy (fun f -> f.FriendGroup)
            |> Array.sortBy fst

        w.WriteInt64(int64 groups.Length)

        let mutable onlineFriends = ResizeArray<PlayerRecord>()
        for groupIdx, groupFriends in groups do
            w.WriteString(string groupIdx) // Имя группы
            w.WriteInt64(int64 groupFriends.Length)
            for f in groupFriends do
                let friendChaId = f.CharacterId2
                let name, motto, icon, isOnline = getCharInfo ctx db friendChaId
                w.WriteInt64(int64 friendChaId)
                w.WriteString(name)
                w.WriteString(motto)
                w.WriteInt64(int64 icon)
                w.WriteInt64(if isOnline then 1L else 0L)
                // Собираем онлайн-друзей для оповещения
                if isOnline then
                    match ctx.Registry.TryGetByChaId(friendChaId) with
                    | Some p -> onlineFriends.Add(p)
                    | None -> ()

        ctx.SendToSingleClient player w

        // Уведомляем онлайн-друзей что мы вошли (MSG_FRND_REFRESH_ONLINE)
        if onlineFriends.Count > 0 then
            let mutable wOnline = WPacket(32)
            wOnline.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
            wOnline.WriteInt64(int64 MSG_FRND_REFRESH_ONLINE)
            wOnline.WriteInt64(int64 chaSlot.ChaId)
            ctx.SendToClients (onlineFriends.ToArray()) wOnline
    with ex ->
        ctx.Logger.LogError(ex, "sendFriendsInit: ошибка БД для {Name}", chaSlot.ChaName)

// ══════════════════════════════════════════════════════════
//  sendMasterInit — CMD_PC_MASTER_REFRESH (START) при входе
// ══════════════════════════════════════════════════════════

let private sendMasterInit (ctx: HandlerContext) (player: PlayerRecord) (chaSlot: CharacterSlot) =
    // Ученики текущего игрока (он — мастер)
    let prenticeIds =
        ctx.MasterRelations.Keys
        |> Seq.filter (fun struct (_, mid) -> mid = chaSlot.ChaId)
        |> Seq.map (fun struct (pid, _) -> pid)
        |> Seq.toArray

    // Мастера текущего игрока (он — ученик)
    let masterIds =
        ctx.MasterRelations.Keys
        |> Seq.filter (fun struct (pid, _) -> pid = chaSlot.ChaId)
        |> Seq.map (fun struct (_, mid) -> mid)
        |> Seq.toArray

    try
        use scope = ctx.ScopeFactory.CreateScope()
        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

        // Пакет 1: Список учеников (MSG_PRENTICE_REFRESH_START)
        let mutable w1 = WPacket(1024)
        w1.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
        w1.WriteInt64(int64 MSG_PRENTICE_REFRESH_START)

        // Self info
        w1.WriteInt64(int64 chaSlot.ChaId)
        w1.WriteString(chaSlot.ChaName)
        w1.WriteString(chaSlot.Motto)
        w1.WriteInt64(int64 chaSlot.Icon)

        // Одна группа "Prentice" со всеми учениками
        if prenticeIds.Length > 0 then
            w1.WriteInt64(1L) // 1 группа
            w1.WriteString("Prentice")
            w1.WriteInt64(int64 prenticeIds.Length)
            let mutable onlinePrentices = ResizeArray<PlayerRecord>()
            for pId in prenticeIds do
                let name, motto, icon, isOnline = getCharInfo ctx db pId
                w1.WriteInt64(int64 pId)
                w1.WriteString(name)
                w1.WriteString(motto)
                w1.WriteInt64(int64 icon)
                w1.WriteInt64(if isOnline then 1L else 0L)
                if isOnline then
                    match ctx.Registry.TryGetByChaId(pId) with
                    | Some p -> onlinePrentices.Add(p)
                    | None -> ()
            ctx.SendToSingleClient player w1

            // Уведомляем онлайн-учеников: мастер вошёл (MSG_MASTER_REFRESH_ONLINE)
            if onlinePrentices.Count > 0 then
                let mutable wN = WPacket(16)
                wN.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                wN.WriteInt64(int64 MSG_MASTER_REFRESH_ONLINE)
                wN.WriteInt64(int64 chaSlot.ChaId)
                ctx.SendToClients (onlinePrentices.ToArray()) wN
        else
            w1.WriteInt64(0L)
            ctx.SendToSingleClient player w1

        // Пакет 2: Список мастеров (MSG_MASTER_REFRESH_START)
        let mutable w2 = WPacket(1024)
        w2.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
        w2.WriteInt64(int64 MSG_MASTER_REFRESH_START)

        // Self info
        w2.WriteInt64(int64 chaSlot.ChaId)
        w2.WriteString(chaSlot.ChaName)
        w2.WriteString(chaSlot.Motto)
        w2.WriteInt64(int64 chaSlot.Icon)

        if masterIds.Length > 0 then
            w2.WriteInt64(1L) // 1 группа
            w2.WriteString("Master")
            w2.WriteInt64(int64 masterIds.Length)
            let mutable onlineMasters = ResizeArray<PlayerRecord>()
            for mId in masterIds do
                let name, motto, icon, isOnline = getCharInfo ctx db mId
                w2.WriteInt64(int64 mId)
                w2.WriteString(name)
                w2.WriteString(motto)
                w2.WriteInt64(int64 icon)
                w2.WriteInt64(if isOnline then 1L else 0L)
                if isOnline then
                    match ctx.Registry.TryGetByChaId(mId) with
                    | Some p -> onlineMasters.Add(p)
                    | None -> ()
            ctx.SendToSingleClient player w2

            // Уведомляем онлайн-мастеров: ученик вошёл (MSG_PRENTICE_REFRESH_ONLINE)
            if onlineMasters.Count > 0 then
                let mutable wN = WPacket(16)
                wN.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                wN.WriteInt64(int64 MSG_PRENTICE_REFRESH_ONLINE)
                wN.WriteInt64(int64 chaSlot.ChaId)
                ctx.SendToClients (onlineMasters.ToArray()) wN
        else
            w2.WriteInt64(0L)
            ctx.SendToSingleClient player w2
    with ex ->
        ctx.Logger.LogError(ex, "sendMasterInit: ошибка БД для {Name}", chaSlot.ChaName)

// ══════════════════════════════════════════════════════════
//  CheckEstop — Проверка статуса блокировки
// ══════════════════════════════════════════════════════════

/// Аналог C++ Player::CheckEstop: если персонаж заблокирован (estop),
/// отправляем CMD_PT_ESTOPUSER + системное сообщение.
let private checkEstop (ctx: HandlerContext) (player: PlayerRecord) (chaSlot: CharacterSlot) =
    if chaSlot.Estop then
        let mutable estopPkt = WPacket(16)
        estopPkt.WriteCmd(Commands.CMD_PT_ESTOPUSER)
        ctx.SendToSingleClient player estopPkt

        let mutable sysPkt = WPacket(128)
        sysPkt.WriteCmd(Commands.CMD_MC_SYSINFO)
        sysPkt.WriteString("Your account has been suspended.")
        ctx.SendToSingleClient player sysPkt

        ctx.Logger.LogInformation("CheckEstop: {Name} заблокирован (estop)", chaSlot.ChaName)

// ══════════════════════════════════════════════════════════
//  sendGmInit — CMD_PC_GM_INFO при входе
// ══════════════════════════════════════════════════════════

/// Аналог C++ PC_GM_INIT: отправляем список GM-ов игроку,
/// и если игрок сам GM — оповещаем всех остальных.
let private sendGmInit (ctx: HandlerContext) (player: PlayerRecord) (chaSlot: CharacterSlot) =
    // Собираем онлайн-GM из реестра
    let gmPlayers =
        ctx.Registry.GetAllPlayers()
        |> Seq.filter (fun p -> p.IsPlaying && p.GmLevel > 0y)
        |> Seq.choose (fun p ->
            match getCurrentSlot p with
            | Some s -> Some (p, s)
            | None -> None)
        |> Seq.toArray

    // Отправить список GM текущему игроку (MSG_FRND_REFRESH_START)
    let mutable w = WPacket(256)
    w.WriteCmd(Commands.CMD_PC_GM_INFO)
    w.WriteInt64(int64 MSG_FRND_REFRESH_START)
    w.WriteInt64(int64 gmPlayers.Length)

    for gmPlayer, gmSlot in gmPlayers do
        w.WriteInt64(int64 gmSlot.ChaId)
        w.WriteString(gmSlot.ChaName)
        w.WriteString(gmSlot.Motto)
        w.WriteInt64(int64 gmSlot.Icon)
        w.WriteInt64(if gmPlayer.IsPlaying then 1L else 0L)

    ctx.SendToSingleClient player w

    // Если текущий игрок — GM, оповестить всех остальных (MSG_FRND_REFRESH_ADD)
    if player.GmLevel > 0y then
        let mutable wAll = WPacket(256)
        wAll.WriteCmd(Commands.CMD_PC_GM_INFO)
        wAll.WriteInt64(int64 MSG_FRND_REFRESH_ADD)
        wAll.WriteString("GM")
        wAll.WriteInt64(int64 chaSlot.ChaId)
        wAll.WriteString(chaSlot.ChaName)
        wAll.WriteString(chaSlot.Motto)
        wAll.WriteInt64(int64 chaSlot.Icon)

        let otherPlayers =
            ctx.Registry.GetAllPlayers()
            |> Seq.filter (fun p -> p.IsPlaying && p.GpAddr <> player.GpAddr)
            |> Seq.toArray

        if otherPlayers.Length > 0 then
            ctx.SendToClients otherPlayers wAll

// ══════════════════════════════════════════════════════════
//  sendTeamUpdate — CMD_PM_TEAM (UPDATE) при смене карты
// ══════════════════════════════════════════════════════════

/// Аналог C++ MP_SWITCH: при переключении карты обновляем
/// список команды для GameServer через GateServer.
let private sendTeamUpdate (ctx: HandlerContext) (player: PlayerRecord) =
    match getPlayingState player with
    | None -> ()
    | Some playing when playing.TeamId > 0 ->
        match ctx.Teams.TryGetValue(playing.TeamId) with
        | true, team ->
            let allMembers = team.Members |> Map.values |> Seq.toArray
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_PM_TEAM)
            w.WriteInt64(int64 Commands.TeamMsg.UPDATE)
            w.WriteInt64(int64 allMembers.Length)

            for m in allMembers do
                match ctx.Registry.TryGetByGpAddr(m.GpAddr) with
                | Some p ->
                    let gateName = if isNull p.Gate then "" else p.Gate.Name
                    w.WriteString(gateName)
                    w.WriteInt64(int64 p.GateAddr)
                    w.WriteInt64(int64 m.ChaId)
                | None ->
                    w.WriteString("")
                    w.WriteInt64(0L)
                    w.WriteInt64(int64 m.ChaId)

            // Отправляем на все подключённые Gate (как в C++)
            for gate in ctx.Gates do
                if not (isNull gate) && gate.IsRegistered then
                    gate.SendPacket(w.Clone())

            ctx.Logger.LogDebug("MP_SWITCH: обновление команды #{TeamId} для {Name}", team.Id, player.CurrentChaName)
        | _ -> ()
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  MP_ENTERMAP — Уведомление о входе на карту
// ══════════════════════════════════════════════════════════

let handleMpEntermap (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let msg = Deserialize.mpEnterMapMessage packet
    let isSwitch = byte msg.IsSwitch

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot ->
        ctx.Logger.LogDebug("MP_ENTERMAP: {Name} вошёл на карту (isSwitch={Switch})", chaSlot.ChaName, isSwitch)

        if isSwitch = 0uy then
            // Полная инициализация при первом входе на карту

            // 1. CMD_PA_USER_BILLBGN → AccountServer (начало биллинга)
            match getPlayingState player with
            | Some playing ->
                let billPkt = Serialize.paUserBillBgnMessage { AcctName = playing.Auth.AccountName; Passport = playing.Auth.Passport }
                ctx.AccountSystem.Send(billPkt)
            | None -> ()

            // 2. CheckEstop — проверка блокировки
            checkEstop ctx player chaSlot

            // 3-6. Инициализация социальных систем (порядок как в C++: Friends → GM → Guild → Master)
            sendFriendsInit ctx player chaSlot
            sendGmInit ctx player chaSlot
            Corsairs.GroupServer.Services.Handlers.GuildHandlers.sendGuildInit ctx player
            sendMasterInit ctx player chaSlot
        else
            // Переключение карты — обновление команды
            sendTeamUpdate ctx player
            ctx.Logger.LogDebug("MP_ENTERMAP: {Name} переключил карту", chaSlot.ChaName)
