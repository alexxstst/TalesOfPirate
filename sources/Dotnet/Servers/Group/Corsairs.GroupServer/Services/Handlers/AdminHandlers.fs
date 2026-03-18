module Corsairs.GroupServer.Services.Handlers.AdminHandlers

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

// ══════════════════════════════════════════════════════════
//  AP_KICKUSER — Кик от AccountServer
// ══════════════════════════════════════════════════════════

let handleApKickuser (ctx: HandlerContext) (packet: IRPacket) =
    let actId = uint32 (packet.ReadInt64())

    match ctx.Registry.TryGetByActId(actId) with
    | None ->
        ctx.Logger.LogDebug("AP_KICKUSER: игрок actId={ActId} не найден", actId)
    | Some player ->
        ctx.Logger.LogInformation("AP_KICKUSER: кик actId={ActId} ({Name})", actId, player.CurrentChaName)

        // Кик через GateServer
        ctx.KickUser player.GpAddr player.GateAddr

        // Уведомляем AccountServer (PA_USER_BILLEND)
        let mutable w = WPacket(64)
        w.WriteCmd(Commands.CMD_PA_USER_BILLEND)
        w.WriteString(player.AccountName)
        ctx.AccountSystem.Send(w)

        // Очищаем из реестра
        ctx.Registry.Unregister(player.GpAddr)

// ══════════════════════════════════════════════════════════
//  AP_EXPSCALE — Множитель опыта от AccountServer
// ══════════════════════════════════════════════════════════

let handleApExpscale (ctx: HandlerContext) (packet: IRPacket) =
    let chaId = int (packet.ReadInt64())
    let time = int (packet.ReadInt64())

    ctx.Logger.LogInformation("AP_EXPSCALE: chaId={ChaId} time={Time}", chaId, time)

    // Рассылаем CMD_PM_EXPSCALE всем GameServer через все Gate
    let mutable w = WPacket(32)
    w.WriteCmd(Commands.CMD_PM_EXPSCALE)
    w.WriteInt64(int64 chaId)
    w.WriteInt64(int64 time)
    for gate in ctx.Gates do
        if not (isNull gate) && gate.IsRegistered then
            gate.SendPacket(w.Clone())

// ══════════════════════════════════════════════════════════
//  MP_GMBANACCOUNT — GM: бан аккаунта
// ══════════════════════════════════════════════════════════

let handleMpGmbanaccount (ctx: HandlerContext) (packet: IRPacket) =
    let accountName = packet.ReadString()

    ctx.Logger.LogInformation("MP_GMBANACCOUNT: бан аккаунта {Name}", accountName)

    // Проброс в AccountServer
    let mutable w = WPacket(128)
    w.WriteCmd(Commands.CMD_PA_GMBANACCOUNT)
    w.WriteString(accountName)
    ctx.AccountSystem.Send(w)

// ══════════════════════════════════════════════════════════
//  MP_GMUNBANACCOUNT — GM: разбан аккаунта
// ══════════════════════════════════════════════════════════

let handleMpGmunbanaccount (ctx: HandlerContext) (packet: IRPacket) =
    let accountName = packet.ReadString()

    ctx.Logger.LogInformation("MP_GMUNBANACCOUNT: разбан аккаунта {Name}", accountName)

    // Проброс в AccountServer
    let mutable w = WPacket(128)
    w.WriteCmd(Commands.CMD_PA_GMUNBANACCOUNT)
    w.WriteString(accountName)
    ctx.AccountSystem.Send(w)

// ══════════════════════════════════════════════════════════
//  MP_MUTE_PLAYER — GM: замутить игрока
// ══════════════════════════════════════════════════════════

let handleMpMutePlayer (ctx: HandlerContext) (packet: IRPacket) =
    let chaName = packet.ReadString()
    let seconds = int (packet.ReadInt64())

    match ctx.Registry.TryGetByChaName(chaName) with
    | None ->
        ctx.Logger.LogDebug("MP_MUTE_PLAYER: игрок {Name} не найден", chaName)
    | Some player ->
        ctx.Logger.LogInformation("MP_MUTE_PLAYER: {Name} замучен на {Sec}с", chaName, seconds)

        if seconds > 0 then
            // Отправляем PT_ESTOPUSER на все Gate
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PT_ESTOPUSER)
            w.WriteInt64(int64 player.GpAddr)
            w.WriteInt64(int64 player.GateAddr)
            w.WriteInt64(int64 seconds)
            w.WriteInt64(1L)
            let gate = player.Gate
            if not (isNull gate) && gate.IsRegistered then
                gate.SendPacket(w)
        else
            // Снять мут
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PT_DEL_ESTOPUSER)
            w.WriteInt64(int64 player.GpAddr)
            w.WriteInt64(int64 player.GateAddr)
            w.WriteInt64(1L)
            let gate = player.Gate
            if not (isNull gate) && gate.IsRegistered then
                gate.SendPacket(w)

// ══════════════════════════════════════════════════════════
//  MP_CANRECEIVEREQUESTS — Установить флаг приёма заявок
// ══════════════════════════════════════════════════════════

let handleMpCanreceiverequests (ctx: HandlerContext) (packet: IRPacket) =
    let chaId = int (packet.ReadInt64())
    let flag = int (packet.ReadInt64())

    match ctx.Registry.TryGetByChaId(chaId) with
    | None ->
        ctx.Logger.LogDebug("MP_CANRECEIVEREQUESTS: игрок chaId={ChaId} не найден", chaId)
    | Some player ->
        match player.State with
        | Authorized_ auth ->
            auth.CanReceiveRequests <- (flag <> 0)
        | Playing_ playing ->
            playing.Auth.CanReceiveRequests <- (flag <> 0)
        | _ -> ()
        ctx.Logger.LogDebug("MP_CANRECEIVEREQUESTS: chaId={ChaId} flag={Flag}", chaId, flag)

// ══════════════════════════════════════════════════════════
//  MP_GUILDNOTICE — Уведомление гильдии
// ══════════════════════════════════════════════════════════

let handleMpGuildnotice (ctx: HandlerContext) (packet: IRPacket) =
    let guildId = int (packet.ReadInt64())
    let content = packet.ReadString()

    ctx.Logger.LogDebug("MP_GUILDNOTICE: гильдия {Id}", guildId)

    // Находим онлайн-участников гильдии
    let guildPlayers =
        ctx.Registry.GetAllPlayers()
        |> Seq.filter (fun p -> p.IsPlaying && p.CurrentGuildId = guildId)
        |> Seq.toArray

    if guildPlayers.Length > 0 then
        let mutable w = WPacket(512)
        w.WriteCmd(Commands.CMD_PC_GUILDNOTICE)
        w.WriteString(content)
        ctx.SendToClients guildPlayers w

// ══════════════════════════════════════════════════════════
//  CP_REPORT_WG — Отчёт о WallGuard (античит)
// ══════════════════════════════════════════════════════════

let handleCpReportWg (ctx: HandlerContext) (player: PlayerRecord) (_packet: IRPacket) =
    match getPlayingState player with
    | Some playing ->
        playing.IsCheat <- true
        ctx.Logger.LogInformation("CP_REPORT_WG: {Name} помечен как читер", player.CurrentChaName)
    | None -> ()
