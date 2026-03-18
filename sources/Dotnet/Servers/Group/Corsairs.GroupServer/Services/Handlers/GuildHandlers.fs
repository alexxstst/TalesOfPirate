module Corsairs.GroupServer.Services.Handlers.GuildHandlers

open System
open System.Linq
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Database

// ══════════════════════════════════════════════════════════
//  Константы
// ══════════════════════════════════════════════════════════

[<Literal>]
let private MSG_GUILD_START   = 1uy
[<Literal>]
let private MSG_GUILD_ADD     = 2uy
[<Literal>]
let private MSG_GUILD_DEL     = 3uy
[<Literal>]
let private MSG_GUILD_ONLINE  = 4uy
[<Literal>]
let private MSG_GUILD_OFFLINE = 5uy
[<Literal>]
let private MSG_GUILD_STOP    = 6uy

// Права гильдии (битовые флаги)
[<Literal>]
let private PERM_DEFAULT = 1u
[<Literal>]
let private PERM_MAX = 0xFFFFFFFFu

// ══════════════════════════════════════════════════════════
//  Вспомогательные функции
// ══════════════════════════════════════════════════════════

let private getCurrentSlot (player: PlayerRecord) =
    match player.State with
    | Playing_ p ->
        let auth = p.Auth
        if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
            Some auth.Characters[p.CurrentChaIndex]
        else None
    | _ -> None

/// Получить онлайн-игроков гильдии.
let private getGuildPlayers (ctx: HandlerContext) (guildId: int) =
    ctx.Registry.GetAllPlayers()
    |> Seq.filter (fun p -> p.IsPlaying && p.CurrentGuildId = guildId)
    |> Seq.toArray

/// Отправить PC_GUILD DEL всем участникам гильдии.
let private sendGuildDel (ctx: HandlerContext) (guildId: int) (removedChaId: int) =
    let mutable w = WPacket(32)
    w.WriteCmd(Commands.CMD_PC_GUILD)
    w.WriteInt64(int64 MSG_GUILD_DEL)
    w.WriteInt64(int64 removedChaId)
    let players = getGuildPlayers ctx guildId
    ctx.SendToClients players w

/// Отправить PC_GUILD STOP конкретному игроку.
let private sendGuildStop (ctx: HandlerContext) (player: PlayerRecord) =
    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_PC_GUILD)
    w.WriteInt64(int64 MSG_GUILD_STOP)
    ctx.SendToSingleClient player w

// ══════════════════════════════════════════════════════════
//  MP_GUILD_CREATE — Создание гильдии
// ══════════════════════════════════════════════════════════

let handleMpGuildCreate (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let guildId = int (packet.ReadInt64())
    let guildName = packet.ReadString()
    let _job = packet.ReadString()
    let _degree = int16 (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot ->
        let guild =
            { Id = guildId
              Name = guildName
              Motto = ""
              LeaderId = chaSlot.ChaId
              Status = GuildStatus.None
              RemainMinute = 0
              Tick = 0L
              Members = Map.ofList [ (chaSlot.ChaId,
                  { ChaId = chaSlot.ChaId
                    ChaName = chaSlot.ChaName
                    Permission = PERM_MAX }) ]
              ChallMoney = 0
              ChallPrizeMoney = 0 }
        ctx.Guilds.TryAdd(guildId, guild) |> ignore

        // Обновляем слот персонажа
        chaSlot.GuildId <- guildId
        chaSlot.GuildPermission <- PERM_MAX

        // Ответ клиенту
        let mutable w = WPacket(256)
        w.WriteCmd(Commands.CMD_PC_GUILD)
        w.WriteInt64(int64 MSG_GUILD_START)
        w.WriteInt64(int64 guildId)
        w.WriteString(guildName)
        w.WriteInt64(int64 chaSlot.ChaId)
        // Список участников (лидер)
        w.WriteInt64(1L) // online
        w.WriteInt64(int64 chaSlot.ChaId)
        w.WriteString(chaSlot.ChaName)
        w.WriteString(chaSlot.Motto)
        w.WriteString("") // job
        w.WriteInt64(0L) // degree
        w.WriteInt64(int64 chaSlot.Icon)
        w.WriteInt64(int64 PERM_MAX)
        w.WriteInt64(0L) // packetNum
        w.WriteInt64(1L) // count
        ctx.SendToSingleClient player w

        ctx.Logger.LogInformation("MP_GUILD_CREATE: гильдия {Name} ({Id}) создана лидером {Leader}",
            guildName, guildId, chaSlot.ChaName)

// ══════════════════════════════════════════════════════════
//  MP_GUILD_APPROVE — Одобрение заявки в гильдию
// ══════════════════════════════════════════════════════════

let handleMpGuildApprove (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | false, _ -> ()
        | true, guild ->
            // Находим цель (может быть оффлайн — тогда обновим только БД)
            let targetPlayer = ctx.Registry.TryGetByChaId(targetChaId)
            let targetName =
                match targetPlayer with
                | Some tp ->
                    match getCurrentSlot tp with
                    | Some ts ->
                        ts.GuildId <- guild.Id
                        ts.GuildPermission <- PERM_DEFAULT
                        ts.ChaName
                    | None -> ""
                | None -> ""

            // Добавляем в in-memory
            let memberInfo =
                { ChaId = targetChaId
                  ChaName = if targetName <> "" then targetName else $"Player#{targetChaId}"
                  Permission = PERM_DEFAULT }
            guild.Members <- guild.Members |> Map.add targetChaId memberInfo

            // Уведомляем участников гильдии
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_GUILD)
            w.WriteInt64(int64 MSG_GUILD_ADD)
            w.WriteInt64(if targetPlayer.IsSome then 1L else 0L)
            w.WriteInt64(int64 targetChaId)
            w.WriteString(memberInfo.ChaName)
            w.WriteString("") // motto
            w.WriteString("") // job
            w.WriteInt64(0L) // degree
            w.WriteInt64(0L) // icon
            w.WriteInt64(int64 PERM_DEFAULT)

            let guildPlayers = getGuildPlayers ctx guild.Id
            ctx.SendToClients guildPlayers w

            ctx.Logger.LogInformation("MP_GUILD_APPROVE: {Target} одобрен в гильдию {Guild}",
                targetChaId, guild.Name)

// ══════════════════════════════════════════════════════════
//  MP_GUILD_KICK — Исключение из гильдии
// ══════════════════════════════════════════════════════════

let handleMpGuildKick (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | false, _ -> ()
        | true, guild ->
            // Удаляем из in-memory
            guild.Members <- guild.Members |> Map.remove targetChaId

            // Если онлайн — обновляем слот
            match ctx.Registry.TryGetByChaId(targetChaId) with
            | Some target ->
                match getCurrentSlot target with
                | Some ts ->
                    ts.GuildId <- 0
                    ts.GuildPermission <- 0u
                | None -> ()
                sendGuildStop ctx target
            | None -> ()

            // Уведомляем оставшихся
            sendGuildDel ctx guild.Id targetChaId

            ctx.Logger.LogInformation("MP_GUILD_KICK: {Target} исключён из {Guild}", targetChaId, guild.Name)

// ══════════════════════════════════════════════════════════
//  MP_GUILD_LEAVE — Выход из гильдии
// ══════════════════════════════════════════════════════════

let handleMpGuildLeave (ctx: HandlerContext) (player: PlayerRecord) (_packet: IRPacket) =
    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        let guildId = chaSlot.GuildId
        match ctx.Guilds.TryGetValue(guildId) with
        | false, _ -> ()
        | true, guild ->
            guild.Members <- guild.Members |> Map.remove chaSlot.ChaId
            chaSlot.GuildId <- 0
            chaSlot.GuildPermission <- 0u

            sendGuildStop ctx player
            sendGuildDel ctx guildId chaSlot.ChaId

            ctx.Logger.LogInformation("MP_GUILD_LEAVE: {Name} покинул {Guild}", chaSlot.ChaName, guild.Name)

// ══════════════════════════════════════════════════════════
//  MP_GUILD_DISBAND — Расформирование гильдии
// ══════════════════════════════════════════════════════════

let handleMpGuildDisband (ctx: HandlerContext) (player: PlayerRecord) (_packet: IRPacket) =
    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | false, _ -> ()
        | true, guild when guild.LeaderId <> chaSlot.ChaId ->
            ctx.Logger.LogDebug("MP_GUILD_DISBAND: {Name} не лидер гильдии", chaSlot.ChaName)
        | true, guild ->
            let guildId = guild.Id

            // Обнуляем гильдию у онлайн-участников
            let guildPlayers = getGuildPlayers ctx guildId
            for p in guildPlayers do
                match getCurrentSlot p with
                | Some slot ->
                    slot.GuildId <- 0
                    slot.GuildPermission <- 0u
                | None -> ()
                sendGuildStop ctx p

            // Удаляем гильдию из памяти
            ctx.Guilds.TryRemove(guildId) |> ignore

            // PM_GUILD_DISBAND → все Gate (для GameServer)
            let mutable wPm = WPacket(32)
            wPm.WriteCmd(Commands.CMD_PM_GUILD_DISBAND)
            wPm.WriteInt64(int64 guildId)
            for gate in ctx.Gates do
                if not (isNull gate) && gate.IsRegistered then
                    gate.SendPacket(wPm.Clone())

            ctx.Logger.LogInformation("MP_GUILD_DISBAND: гильдия {Name} ({Id}) расформирована",
                guild.Name, guildId)

// ══════════════════════════════════════════════════════════
//  MP_GUILD_MOTTO — Изменение девиза
// ══════════════════════════════════════════════════════════

let handleMpGuildMotto (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let newMotto = packet.ReadString()

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | true, guild ->
            guild.Motto <- newMotto
            ctx.Logger.LogDebug("MP_GUILD_MOTTO: гильдия {Name} — новый девиз", guild.Name)
        | _ -> ()

// ══════════════════════════════════════════════════════════
//  MP_GUILD_PERM — Изменение прав участника
// ══════════════════════════════════════════════════════════

let handleMpGuildPerm (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetId = int (packet.ReadInt64())
    let permission = uint32 (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 -> ()
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | false, _ -> ()
        | true, guild ->
            // Обновляем in-memory
            match guild.Members.TryFind(targetId) with
            | Some mem ->
                guild.Members <- guild.Members |> Map.add targetId { mem with Permission = permission }
            | None -> ()

            // Обновляем у онлайн-игрока
            match ctx.Registry.TryGetByChaId(targetId) with
            | Some target ->
                match getCurrentSlot target with
                | Some slot -> slot.GuildPermission <- permission
                | None -> ()
            | None -> ()

            // Уведомляем участников
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_GUILD_PERM)
            w.WriteInt64(int64 targetId)
            w.WriteInt64(int64 permission)
            let guildPlayers = getGuildPlayers ctx guild.Id
            ctx.SendToClients guildPlayers w

// ══════════════════════════════════════════════════════════
//  PC_GULD_INIT — Инициализация гильдии при входе
// ══════════════════════════════════════════════════════════

let sendGuildInit (ctx: HandlerContext) (player: PlayerRecord) =
    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot when chaSlot.GuildId = 0 ->
        // Аналог C++: отправляем пустой пакет гильдии для игроков без гильдии
        let mutable w = WPacket(32)
        w.WriteCmd(Commands.CMD_PC_GUILD)
        w.WriteInt64(int64 MSG_GUILD_START)
        w.WriteInt64(0L) // guildId = 0
        w.WriteInt64(0L) // 0
        ctx.SendToSingleClient player w
    | Some chaSlot ->
        match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
        | false, _ ->
            ctx.Logger.LogWarning("sendGuildInit: гильдия {GuildId} не найдена для {Name}", chaSlot.GuildId, chaSlot.ChaName)
        | true, guild ->
            // Получаем данные участников из БД (аналог C++ InitGuildMember с SQL-запросом)
            let memberDetails =
                try
                    use scope = ctx.ScopeFactory.CreateScope()
                    let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                    let members = guild.Members |> Map.values |> Seq.toArray
                    members |> Array.map (fun m ->
                        let isOnline =
                            match ctx.Registry.TryGetByChaId(m.ChaId) with
                            | Some p when p.IsPlaying -> true
                            | _ -> false
                        let cha = db.Characters.Find(m.ChaId)
                        let motto, job, degree, icon =
                            if isNull (box cha) then "", "", 0, 0s
                            else
                                (defaultArg (Option.ofObj cha.Motto) ""),
                                (defaultArg (Option.ofObj cha.Job) ""),
                                int cha.Level,
                                cha.IconId
                        (m, isOnline, motto, job, degree, icon))
                with ex ->
                    ctx.Logger.LogError(ex, "sendGuildInit: ошибка БД для гильдии {GuildId}", guild.Id)
                    guild.Members |> Map.values |> Seq.toArray
                    |> Array.map (fun m ->
                        let isOnline =
                            match ctx.Registry.TryGetByChaId(m.ChaId) with
                            | Some p when p.IsPlaying -> true
                            | _ -> false
                        (m, isOnline, "", "", 0, 0s))

            let mutable w = WPacket(2048)
            w.WriteCmd(Commands.CMD_PC_GUILD)
            w.WriteInt64(int64 MSG_GUILD_START)
            w.WriteInt64(int64 guild.Id)
            w.WriteString(guild.Name)
            w.WriteInt64(int64 guild.LeaderId)

            for m, isOnline, motto, job, degree, icon in memberDetails do
                w.WriteInt64(if isOnline then 1L else 0L)
                w.WriteInt64(int64 m.ChaId)
                w.WriteString(m.ChaName)
                w.WriteString(motto)
                w.WriteString(job)
                w.WriteInt64(int64 degree)
                w.WriteInt64(int64 icon)
                w.WriteInt64(int64 m.Permission)

            w.WriteInt64(0L) // packetNum
            w.WriteInt64(int64 memberDetails.Length)
            ctx.SendToSingleClient player w

            // Уведомить онлайн-участников о входе (CMD_PC_GUILD/MSG_GUILD_ONLINE)
            let mutable wOnline = WPacket(32)
            wOnline.WriteCmd(Commands.CMD_PC_GUILD)
            wOnline.WriteInt64(int64 MSG_GUILD_ONLINE)
            wOnline.WriteInt64(int64 chaSlot.ChaId)
            let otherMembers =
                getGuildPlayers ctx guild.Id
                |> Array.filter (fun p -> p.GpAddr <> player.GpAddr)
            if otherMembers.Length > 0 then
                ctx.SendToClients otherMembers wOnline
