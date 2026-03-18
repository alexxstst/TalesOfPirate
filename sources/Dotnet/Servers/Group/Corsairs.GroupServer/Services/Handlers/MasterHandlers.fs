module Corsairs.GroupServer.Services.Handlers.MasterHandlers

open System
open System.Linq
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

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

/// Количество наставников у ученика (in-memory).
let private getMasterCount (relations: System.Collections.Concurrent.ConcurrentDictionary<struct (int * int), bool>) (prenticeId: int) =
    relations.Keys |> Seq.filter (fun struct (pid, _) -> pid = prenticeId) |> Seq.length

/// Количество учеников у наставника (in-memory).
let private getPrenticeCount (relations: System.Collections.Concurrent.ConcurrentDictionary<struct (int * int), bool>) (masterId: int) =
    relations.Keys |> Seq.filter (fun struct (_, mid) -> mid = masterId) |> Seq.length

/// Существует ли связь (in-memory).
let private hasRelation (relations: System.Collections.Concurrent.ConcurrentDictionary<struct (int * int), bool>) (prenticeId: int) (masterId: int) =
    relations.ContainsKey(struct (prenticeId, masterId))

// ══════════════════════════════════════════════════════════
//  MP_MASTER_CREATE — Создание связи мастер-ученик
// ══════════════════════════════════════════════════════════

let handleMpMasterCreate (ctx: HandlerContext) (packet: IRPacket) =
    let cfg = ctx.Config
    let prenticeName = packet.ReadString()
    let prenticeId = int (packet.ReadInt64())
    let masterName = packet.ReadString()
    let masterId = int (packet.ReadInt64())

    if hasRelation ctx.MasterRelations prenticeId masterId then
        ctx.Logger.LogDebug("MP_MASTER_CREATE: связь {P}-{M} уже существует", prenticeId, masterId)
    elif getMasterCount ctx.MasterRelations prenticeId >= cfg.MaxMasters then
        ctx.Logger.LogDebug("MP_MASTER_CREATE: у {Name} лимит наставников", prenticeName)
    elif getPrenticeCount ctx.MasterRelations masterId >= cfg.MaxPrentices then
        ctx.Logger.LogDebug("MP_MASTER_CREATE: у {Name} лимит учеников", masterName)
    else
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            let mentorship = Mentorship()
            mentorship.MasterCharacterId <- masterId
            mentorship.PrenticeCharacterId <- prenticeId
            mentorship.IsFinished <- false
            mentorship.CreatedAt <- DateTimeOffset.UtcNow

            db.Mentorships.Add(mentorship) |> ignore
            db.SaveChanges() |> ignore

            // In-memory
            ctx.MasterRelations.TryAdd(struct (prenticeId, masterId), true) |> ignore

            // Уведомляем ученика (если онлайн)
            match ctx.Registry.TryGetByChaId(prenticeId) with
            | Some prentice when prentice.IsPlaying ->
                match getCurrentSlot prentice with
                | Some _ ->
                    // Получаем данные мастера для уведомления
                    match ctx.Registry.TryGetByChaId(masterId) with
                    | Some masterPlayer ->
                        match getCurrentSlot masterPlayer with
                        | Some masterSlot ->
                            let mutable w = WPacket(128)
                            w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                            w.WriteInt64(2L)
                            w.WriteString("Master")
                            w.WriteInt64(int64 masterSlot.ChaId)
                            w.WriteString(masterSlot.ChaName)
                            w.WriteString(masterSlot.Motto)
                            w.WriteInt64(int64 masterSlot.Icon)
                            ctx.SendToSingleClient prentice w
                        | None -> ()
                    | None -> ()
                | None -> ()
            | _ -> ()

            // Уведомляем мастера (если онлайн)
            match ctx.Registry.TryGetByChaId(masterId) with
            | Some masterPlayer when masterPlayer.IsPlaying ->
                match ctx.Registry.TryGetByChaId(prenticeId) with
                | Some prenticePlayer ->
                    match getCurrentSlot prenticePlayer with
                    | Some prenticeSlot ->
                        let mutable w = WPacket(128)
                        w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                        w.WriteInt64(7L)
                        w.WriteString("Prentice")
                        w.WriteInt64(int64 prenticeSlot.ChaId)
                        w.WriteString(prenticeSlot.ChaName)
                        w.WriteString(prenticeSlot.Motto)
                        w.WriteInt64(int64 prenticeSlot.Icon)
                        ctx.SendToSingleClient masterPlayer w
                    | None -> ()
                | None -> ()
            | _ -> ()

            ctx.Logger.LogInformation("MP_MASTER_CREATE: {Prentice} → {Master}", prenticeName, masterName)
        with ex ->
            ctx.Logger.LogError(ex, "MP_MASTER_CREATE: ошибка БД")

// ══════════════════════════════════════════════════════════
//  MP_MASTER_DEL — Удаление связи мастер-ученик
// ══════════════════════════════════════════════════════════

let handleMpMasterDel (ctx: HandlerContext) (packet: IRPacket) =
    let _prenticeName = packet.ReadString()
    let prenticeId = int (packet.ReadInt64())
    let _masterName = packet.ReadString()
    let masterId = int (packet.ReadInt64())

    if not (hasRelation ctx.MasterRelations prenticeId masterId) then
        ctx.Logger.LogDebug("MP_MASTER_DEL: связь {P}-{M} не найдена", prenticeId, masterId)
    else
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            let toRemove = db.FindActiveMentorship(prenticeId, masterId)
            db.Mentorships.RemoveRange(toRemove)
            db.SaveChanges() |> ignore

            // In-memory
            ctx.MasterRelations.TryRemove(struct (prenticeId, masterId)) |> ignore

            // Уведомляем ученика
            match ctx.Registry.TryGetByChaId(prenticeId) with
            | Some prentice when prentice.IsPlaying ->
                let mutable w = WPacket(32)
                w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                w.WriteInt64(3L)
                w.WriteInt64(int64 masterId)
                ctx.SendToSingleClient prentice w
            | _ -> ()

            // Уведомляем мастера
            match ctx.Registry.TryGetByChaId(masterId) with
            | Some masterPlayer when masterPlayer.IsPlaying ->
                let mutable w = WPacket(32)
                w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
                w.WriteInt64(8L)
                w.WriteInt64(int64 prenticeId)
                ctx.SendToSingleClient masterPlayer w
            | _ -> ()

            ctx.Logger.LogInformation("MP_MASTER_DEL: ученик {P} / мастер {M}", prenticeId, masterId)
        with ex ->
            ctx.Logger.LogError(ex, "MP_MASTER_DEL: ошибка БД")

// ══════════════════════════════════════════════════════════
//  MP_MASTER_FINISH — Завершение обучения (выпуск)
// ══════════════════════════════════════════════════════════

let handleMpMasterFinish (ctx: HandlerContext) (packet: IRPacket) =
    let prenticeId = int (packet.ReadInt64())

    // Находим все связи ученика
    let masterIds =
        ctx.MasterRelations.Keys
        |> Seq.filter (fun struct (pid, _) -> pid = prenticeId)
        |> Seq.map (fun struct (_, mid) -> mid)
        |> Seq.toArray

    if masterIds.Length = 0 then
        ctx.Logger.LogDebug("MP_MASTER_FINISH: у {Id} нет мастеров", prenticeId)
    else
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            let toFinish = db.GetActiveMentorshipsByPrentice(prenticeId)
            for m in toFinish do
                m.IsFinished <- true
            db.SaveChanges() |> ignore

            // Удаляем из in-memory
            for masterId in masterIds do
                ctx.MasterRelations.TryRemove(struct (prenticeId, masterId)) |> ignore

            ctx.Logger.LogInformation("MP_MASTER_FINISH: ученик {Id} выпущен", prenticeId)
        with ex ->
            ctx.Logger.LogError(ex, "MP_MASTER_FINISH: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_MASTER_REFRESH_INFO — Обновление данных мастера
// ══════════════════════════════════════════════════════════

let handleCpMasterRefreshInfo (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let masterChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        if not (hasRelation ctx.MasterRelations mySlot.ChaId masterChaId) then
            ctx.Logger.LogDebug("CP_MASTER_REFRESH_INFO: нет связи {P}-{M}", mySlot.ChaId, masterChaId)
        else
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                let character = db.Characters.Find(masterChaId)

                if not (isNull (box character)) then
                    let guildName =
                        if character.GuildId.HasValue then
                            match ctx.Guilds.TryGetValue(character.GuildId.Value) with
                            | true, guild -> guild.Name
                            | _ -> ""
                        else ""

                    let mutable w = WPacket(256)
                    w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH_INFO)
                    w.WriteInt64(int64 masterChaId)
                    w.WriteString(if isNull character.Motto then "" else character.Motto)
                    w.WriteInt64(int64 character.IconId)
                    w.WriteInt64(int64 character.Level)
                    w.WriteString(if isNull character.Job then "" else character.Job)
                    w.WriteString(guildName)
                    ctx.SendToSingleClient player w
            with ex ->
                ctx.Logger.LogError(ex, "CP_MASTER_REFRESH_INFO: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_PRENTICE_REFRESH_INFO — Обновление данных ученика
// ══════════════════════════════════════════════════════════

let handleCpPrenticeRefreshInfo (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let prenticeChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        if not (hasRelation ctx.MasterRelations prenticeChaId mySlot.ChaId) then
            ctx.Logger.LogDebug("CP_PRENTICE_REFRESH_INFO: нет связи {P}-{M}", prenticeChaId, mySlot.ChaId)
        else
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                let character = db.Characters.Find(prenticeChaId)

                if not (isNull (box character)) then
                    let guildName =
                        if character.GuildId.HasValue then
                            match ctx.Guilds.TryGetValue(character.GuildId.Value) with
                            | true, guild -> guild.Name
                            | _ -> ""
                        else ""

                    let mutable w = WPacket(256)
                    w.WriteCmd(Commands.CMD_PC_PRENTICE_REFRESH_INFO)
                    w.WriteInt64(int64 prenticeChaId)
                    w.WriteString(if isNull character.Motto then "" else character.Motto)
                    w.WriteInt64(int64 character.IconId)
                    w.WriteInt64(int64 character.Level)
                    w.WriteString(if isNull character.Job then "" else character.Job)
                    w.WriteString(guildName)
                    ctx.SendToSingleClient player w
            with ex ->
                ctx.Logger.LogError(ex, "CP_PRENTICE_REFRESH_INFO: ошибка БД")
