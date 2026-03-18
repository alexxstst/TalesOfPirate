module Corsairs.GroupServer.Services.Handlers.FriendHandlers

open System
open System.Linq
open Microsoft.EntityFrameworkCore
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

/// Проверить количество друзей персонажа в БД.
let private getFriendCount (db: GameDbContext) (chaId: int) =
    db.GetFriendCount(chaId)

/// Проверить существование дружбы.
let private areFriends (db: GameDbContext) (chaId1: int) (chaId2: int) =
    db.AreFriends(chaId1, chaId2)

// ══════════════════════════════════════════════════════════
//  CP_FRND_INVITE — Приглашение в друзья
// ══════════════════════════════════════════════════════════

let handleCpFrndInvite (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let targetName = packet.ReadString()

    match getCurrentSlot player with
    | None -> ()
    | Some chaSlot ->
        match ctx.Registry.TryGetByChaName(targetName) with
        | None ->
            ctx.Logger.LogDebug("CP_FRND_INVITE: {Name} не найден", targetName)
        | Some target when not target.IsPlaying ->
            ctx.Logger.LogDebug("CP_FRND_INVITE: {Name} не в игре", targetName)
        | Some target when target.GpAddr = player.GpAddr ->
            ctx.Logger.LogDebug("CP_FRND_INVITE: нельзя добавить себя в друзья")
        | Some target when not target.CanReceiveRequests ->
            ctx.Logger.LogDebug("CP_FRND_INVITE: {Name} не принимает запросы", targetName)
        | Some target ->
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

                let myCount = getFriendCount db chaSlot.ChaId
                if myCount >= cfg.MaxFriends then
                    ctx.Logger.LogDebug("CP_FRND_INVITE: лимит друзей у {Name}", chaSlot.ChaName)
                elif areFriends db chaSlot.ChaId target.CurrentChaId then
                    ctx.Logger.LogDebug("CP_FRND_INVITE: {From} и {To} уже друзья", chaSlot.ChaName, targetName)
                else
                    let targetCount = getFriendCount db target.CurrentChaId
                    if targetCount >= cfg.MaxFriends then
                        ctx.Logger.LogDebug("CP_FRND_INVITE: лимит друзей у {Name}", targetName)
                    elif ctx.Invitations.HasPendingFrom(player.GpAddr, FriendInvite) then
                        ctx.Logger.LogDebug("CP_FRND_INVITE: уже есть ожидающее приглашение от {Name}", chaSlot.ChaName)
                    else
                        let inv =
                            { Type = FriendInvite
                              FromGpAddr = player.GpAddr
                              FromChaId = chaSlot.ChaId
                              FromChaName = chaSlot.ChaName
                              ToGpAddr = target.GpAddr
                              ToChaId = target.CurrentChaId
                              ExpiresAt = DateTimeOffset.UtcNow.AddMilliseconds(float cfg.FriendInviteTimeoutMs)
                              ExtraData = 0 }
                        ctx.Invitations.Add(inv)

                        let mutable w = WPacket(128)
                        w.WriteCmd(Commands.CMD_PC_FRND_INVITE)
                        w.WriteString(chaSlot.ChaName)
                        w.WriteInt64(int64 chaSlot.ChaId)
                        w.WriteInt64(int64 chaSlot.Icon)
                        ctx.SendToSingleClient target w

                        ctx.Logger.LogDebug("CP_FRND_INVITE: {From} → {To}", chaSlot.ChaName, targetName)
            with ex ->
                ctx.Logger.LogError(ex, "CP_FRND_INVITE: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_FRND_ACCEPT — Принятие в друзья
// ══════════════════════════════════════════════════════════

let handleCpFrndAccept (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let inviterChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        match ctx.Registry.TryGetByChaId(inviterChaId) with
        | None ->
            ctx.Logger.LogDebug("CP_FRND_ACCEPT: инвайтер {Id} не найден", inviterChaId)
        | Some inviter when not inviter.IsPlaying -> ()
        | Some inviter ->
            match getCurrentSlot inviter with
            | None -> ()
            | Some invSlot ->
                // Удаляем приглашение
                ctx.Invitations.TryAcceptFrom(player.GpAddr, inviter.GpAddr, FriendInvite) |> ignore

                try
                    use scope = ctx.ScopeFactory.CreateScope()
                    let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

                    if areFriends db mySlot.ChaId invSlot.ChaId then
                        ctx.Logger.LogDebug("CP_FRND_ACCEPT: уже друзья")
                    elif getFriendCount db mySlot.ChaId >= ctx.Config.MaxFriends then
                        ctx.Logger.LogDebug("CP_FRND_ACCEPT: лимит друзей у {Name}", mySlot.ChaName)
                    elif getFriendCount db invSlot.ChaId >= ctx.Config.MaxFriends then
                        ctx.Logger.LogDebug("CP_FRND_ACCEPT: лимит друзей у {Name}", invSlot.ChaName)
                    else
                        // Добавляем дружбу в обе стороны
                        let f1 = Friendship()
                        f1.CharacterId1 <- mySlot.ChaId
                        f1.CharacterId2 <- invSlot.ChaId
                        f1.RelationType <- 0uy
                        f1.FriendGroup <- 0uy
                        f1.CreatedAt <- DateTimeOffset.UtcNow

                        let f2 = Friendship()
                        f2.CharacterId1 <- invSlot.ChaId
                        f2.CharacterId2 <- mySlot.ChaId
                        f2.RelationType <- 0uy
                        f2.FriendGroup <- 0uy
                        f2.CreatedAt <- DateTimeOffset.UtcNow

                        db.Friendships.Add(f1) |> ignore
                        db.Friendships.Add(f2) |> ignore
                        db.SaveChanges() |> ignore

                        // Уведомляем принявшего
                        let mutable w1 = WPacket(128)
                        w1.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
                        w1.WriteInt64(2L)
                        w1.WriteString("Friends")
                        w1.WriteInt64(int64 invSlot.ChaId)
                        w1.WriteString(invSlot.ChaName)
                        w1.WriteString(invSlot.Motto)
                        w1.WriteInt64(int64 invSlot.Icon)
                        ctx.SendToSingleClient player w1

                        // Уведомляем инвайтера
                        let mutable w2 = WPacket(128)
                        w2.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
                        w2.WriteInt64(2L)
                        w2.WriteString("Friends")
                        w2.WriteInt64(int64 mySlot.ChaId)
                        w2.WriteString(mySlot.ChaName)
                        w2.WriteString(mySlot.Motto)
                        w2.WriteInt64(int64 mySlot.Icon)
                        ctx.SendToSingleClient inviter w2

                        ctx.Logger.LogInformation("CP_FRND_ACCEPT: {Name1} и {Name2} теперь друзья",
                            mySlot.ChaName, invSlot.ChaName)
                with ex ->
                    ctx.Logger.LogError(ex, "CP_FRND_ACCEPT: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_FRND_REFUSE — Отклонение дружбы
// ══════════════════════════════════════════════════════════

let handleCpFrndRefuse (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let inviterChaId = int (packet.ReadInt64())
    ctx.Invitations.TryAcceptFrom(player.GpAddr, 0u, FriendInvite) |> ignore
    ctx.Logger.LogDebug("CP_FRND_REFUSE: {Name} отклонил от {Id}", player.CurrentChaName, inviterChaId)

// ══════════════════════════════════════════════════════════
//  CP_FRND_DELETE — Удаление из друзей
// ══════════════════════════════════════════════════════════

let handleCpFrndDelete (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            if not (areFriends db mySlot.ChaId targetChaId) then
                ctx.Logger.LogDebug("CP_FRND_DELETE: {Name} не друг с {Id}", mySlot.ChaName, targetChaId)
            else
                // Удаляем в обе стороны
                let toRemove = db.GetFriendshipsBothWays(mySlot.ChaId, targetChaId)
                db.Friendships.RemoveRange(toRemove)
                db.SaveChanges() |> ignore

                // Уведомляем удалившего
                let mutable w1 = WPacket(32)
                w1.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
                w1.WriteInt64(3L)
                w1.WriteInt64(int64 targetChaId)
                ctx.SendToSingleClient player w1

                // Уведомляем удалённого (если онлайн)
                match ctx.Registry.TryGetByChaId(targetChaId) with
                | Some target when target.IsPlaying ->
                    let mutable w2 = WPacket(32)
                    w2.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
                    w2.WriteInt64(3L)
                    w2.WriteInt64(int64 mySlot.ChaId)
                    ctx.SendToSingleClient target w2
                | _ -> ()

                ctx.Logger.LogInformation("CP_FRND_DELETE: {Name} удалил друга {Id}", mySlot.ChaName, targetChaId)
        with ex ->
            ctx.Logger.LogError(ex, "CP_FRND_DELETE: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_FRND_CHANGE_GROUP — Изменение группы друга
// ══════════════════════════════════════════════════════════

let handleCpFrndChangeGroup (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())
    let groupName = packet.ReadString()

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            let friendship = db.FindFriendship(mySlot.ChaId, targetChaId)

            if isNull (box friendship) then
                ctx.Logger.LogDebug("CP_FRND_CHANGE_GROUP: дружба не найдена")
            else
                // Конвертируем имя группы в индекс (0-9)
                let groupByte =
                    match Byte.TryParse(groupName) with
                    | true, v when v < byte ctx.Config.MaxFriendGroups -> v
                    | _ -> 0uy
                friendship.FriendGroup <- groupByte
                db.SaveChanges() |> ignore

                let mutable w = WPacket(64)
                w.WriteCmd(Commands.CMD_PC_FRND_CHANGE_GROUP)
                w.WriteInt64(int64 targetChaId)
                w.WriteString(groupName)
                ctx.SendToSingleClient player w
        with ex ->
            ctx.Logger.LogError(ex, "CP_FRND_CHANGE_GROUP: ошибка БД")

// ══════════════════════════════════════════════════════════
//  CP_FRND_REFRESH_INFO — Обновление данных друга
// ══════════════════════════════════════════════════════════

let handleCpFrndRefreshInfo (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())

    match getCurrentSlot player with
    | None -> ()
    | Some mySlot ->
        try
            use scope = ctx.ScopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            if not (areFriends db mySlot.ChaId targetChaId) then
                ctx.Logger.LogDebug("CP_FRND_REFRESH_INFO: {P} и {T} не друзья", mySlot.ChaId, targetChaId)
            else
                let character = db.Characters.Find(targetChaId)
                if not (isNull (box character)) then
                    let guildName =
                        if character.GuildId.HasValue then
                            match ctx.Guilds.TryGetValue(character.GuildId.Value) with
                            | true, guild -> guild.Name
                            | _ -> ""
                        else ""

                    let mutable w = WPacket(256)
                    w.WriteCmd(Commands.CMD_PC_FRND_REFRESH_INFO)
                    w.WriteInt64(int64 targetChaId)
                    w.WriteString(if isNull character.Motto then "" else character.Motto)
                    w.WriteInt64(int64 character.IconId)
                    w.WriteInt64(int64 character.Level)
                    w.WriteString(if isNull character.Job then "" else character.Job)
                    w.WriteString(guildName)
                    ctx.SendToSingleClient player w
        with ex ->
            ctx.Logger.LogError(ex, "CP_FRND_REFRESH_INFO: ошибка БД")
