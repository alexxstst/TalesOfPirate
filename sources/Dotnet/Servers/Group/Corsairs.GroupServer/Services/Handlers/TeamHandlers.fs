module Corsairs.GroupServer.Services.Handlers.TeamHandlers

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

/// Отправить PC_TEAM_REFRESH всем участникам команды.
let private sendTeamRefresh (ctx: HandlerContext) (team: TeamData) (msgType: byte) (extraMembers: TeamMemberInfo array) =
    let allMembers = team.Members |> Map.values |> Seq.toArray
    let membersToSend = Array.append allMembers extraMembers

    let mutable w = WPacket(512)
    w.WriteCmd(Commands.CMD_PC_TEAM_REFRESH)
    w.WriteInt64(int64 msgType)
    w.WriteInt64(int64 membersToSend.Length)
    for m in membersToSend do
        w.WriteInt64(int64 m.ChaId)
        w.WriteString(m.ChaName)
        w.WriteString(m.Motto)
        w.WriteInt64(int64 m.Icon)

    // Отправляем каждому участнику
    for m in allMembers do
        match ctx.Registry.TryGetByGpAddr(m.GpAddr) with
        | Some p -> ctx.SendToSingleClient p (w.Clone())
        | None -> ()
    // Дополнительным участникам (покинувший/кикнутый)
    for m in extraMembers do
        match ctx.Registry.TryGetByGpAddr(m.GpAddr) with
        | Some p -> ctx.SendToSingleClient p (w.Clone())
        | None -> ()

/// Отправить PM_TEAM уведомление GameServer-ам через Gate.
let private sendPmTeam (ctx: HandlerContext) (team: TeamData) (msgType: byte) =
    let allMembers = team.Members |> Map.values |> Seq.toArray
    let mutable w = WPacket(512)
    w.WriteCmd(Commands.CMD_PM_TEAM)
    w.WriteInt64(int64 msgType)
    w.WriteInt64(int64 allMembers.Length)
    for m in allMembers do
        // GateName + GateAddr + ChaId
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

    // Отправляем всем подключённым Gate
    for gate in ctx.Gates do
        if not (isNull gate) && gate.IsRegistered then
            gate.SendPacket(w.Clone())

/// Создать TeamMemberInfo из PlayerRecord.
let private makeTeamMember (player: PlayerRecord) =
    match getCurrentSlot player with
    | Some slot ->
        Some { ChaId = slot.ChaId
               ChaName = slot.ChaName
               Motto = slot.Motto
               Icon = slot.Icon
               GpAddr = player.GpAddr }
    | None -> None

/// Убрать игрока из команды, обновить TeamId.
let private removeFromTeam (ctx: HandlerContext) (playing: PlayerPlaying) (team: TeamData) =
    let chaId = playing.Auth.Characters[playing.CurrentChaIndex].ChaId
    team.Members <- team.Members |> Map.remove chaId
    playing.TeamId <- 0

    // Если осталось меньше 2 — расформировать команду
    if team.Members.Count < 2 then
        // Уведомить оставшегося
        for kv in team.Members do
            match ctx.Registry.TryGetByGpAddr(kv.Value.GpAddr) with
            | Some lastPlayer ->
                match getPlayingState lastPlayer with
                | Some lp -> lp.TeamId <- 0
                | None -> ()
            | None -> ()
        ctx.Teams.TryRemove(team.Id) |> ignore

// ══════════════════════════════════════════════════════════
//  CP_TEAM_INVITE — Приглашение в команду
// ══════════════════════════════════════════════════════════

let handleCpTeamInvite (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let targetName = packet.ReadString()

    match getPlayingState player with
    | None -> ()
    | Some playing ->
        let chaSlot = playing.Auth.Characters[playing.CurrentChaIndex]

        // Проверка: если в команде, должен быть лидером
        if playing.TeamId > 0 then
            match ctx.Teams.TryGetValue(playing.TeamId) with
            | true, team ->
                if team.LeaderId <> chaSlot.ChaId then
                    ctx.Logger.LogDebug("CP_TEAM_INVITE: {Name} не лидер команды", chaSlot.ChaName)
                elif team.Members.Count >= cfg.MaxTeamMembers then
                    let mutable w = WPacket(16)
                    w.WriteCmd(Commands.CMD_PC_TEAM_CANCEL)
                    w.WriteInt64(4L)
                    ctx.SendToSingleClient player w
                else
                    // Ищем цель
                    match ctx.Registry.TryGetByChaName(targetName) with
                    | None -> ctx.Logger.LogDebug("CP_TEAM_INVITE: цель {Name} не найдена", targetName)
                    | Some target when not target.IsPlaying ->
                        ctx.Logger.LogDebug("CP_TEAM_INVITE: цель {Name} не в игре", targetName)
                    | Some target ->
                        match getPlayingState target with
                        | None -> ()
                        | Some tp when tp.TeamId > 0 ->
                            ctx.Logger.LogDebug("CP_TEAM_INVITE: {Name} уже в команде", targetName)
                        | Some _ when not target.CanReceiveRequests ->
                            ctx.Logger.LogDebug("CP_TEAM_INVITE: {Name} не принимает запросы", targetName)
                        | Some _ ->
                            let inv =
                                { Type = TeamInvite
                                  FromGpAddr = player.GpAddr
                                  FromChaId = chaSlot.ChaId
                                  FromChaName = chaSlot.ChaName
                                  ToGpAddr = target.GpAddr
                                  ToChaId = target.CurrentChaId
                                  ExpiresAt = DateTimeOffset.UtcNow.AddMilliseconds(float cfg.TeamInviteTimeoutMs)
                                  ExtraData = team.Id }
                            ctx.Invitations.Add(inv)

                            let mutable w = WPacket(128)
                            w.WriteCmd(Commands.CMD_PC_TEAM_INVITE)
                            w.WriteString(chaSlot.ChaName)
                            w.WriteInt64(int64 chaSlot.ChaId)
                            w.WriteInt64(int64 chaSlot.Icon)
                            ctx.SendToSingleClient target w
                            ctx.Logger.LogDebug("CP_TEAM_INVITE: {From} → {To}", chaSlot.ChaName, targetName)
            | _ -> ()
        else
            // Не в команде — приглашение создаёт новую при accept
            match ctx.Registry.TryGetByChaName(targetName) with
            | None -> ()
            | Some target when not target.IsPlaying -> ()
            | Some target ->
                match getPlayingState target with
                | None -> ()
                | Some tp when tp.TeamId > 0 -> ()
                | Some _ when not target.CanReceiveRequests -> ()
                | Some _ ->
                    let inv =
                        { Type = TeamInvite
                          FromGpAddr = player.GpAddr
                          FromChaId = chaSlot.ChaId
                          FromChaName = chaSlot.ChaName
                          ToGpAddr = target.GpAddr
                          ToChaId = target.CurrentChaId
                          ExpiresAt = DateTimeOffset.UtcNow.AddMilliseconds(float cfg.TeamInviteTimeoutMs)
                          ExtraData = 0 }
                    ctx.Invitations.Add(inv)

                    let mutable w = WPacket(128)
                    w.WriteCmd(Commands.CMD_PC_TEAM_INVITE)
                    w.WriteString(chaSlot.ChaName)
                    w.WriteInt64(int64 chaSlot.ChaId)
                    w.WriteInt64(int64 chaSlot.Icon)
                    ctx.SendToSingleClient target w

// ══════════════════════════════════════════════════════════
//  CP_TEAM_ACCEPT — Принятие приглашения
// ══════════════════════════════════════════════════════════

let handleCpTeamAccept (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let inviterChaId = int (packet.ReadInt64())

    match getPlayingState player with
    | None -> ()
    | Some playing when playing.TeamId > 0 ->
        ctx.Logger.LogDebug("CP_TEAM_ACCEPT: {Name} уже в команде", player.CurrentChaName)
    | Some playing ->
        // Ищем приглашение от этого chaId
        match ctx.Registry.TryGetByChaId(inviterChaId) with
        | None ->
            ctx.Logger.LogDebug("CP_TEAM_ACCEPT: инвайтер {Id} не найден", inviterChaId)
        | Some inviter ->
            match getPlayingState inviter with
            | None -> ()
            | Some invPlaying ->
                let invSlot = invPlaying.Auth.Characters[invPlaying.CurrentChaIndex]
                let myMember = makeTeamMember player

                match myMember with
                | None -> ()
                | Some me ->
                    // Определяем команду
                    let teamId =
                        if invPlaying.TeamId > 0 then
                            invPlaying.TeamId
                        else
                            // Создаём новую команду
                            let tid = ctx.AllocateTeamId()
                            let invMember = makeTeamMember inviter
                            match invMember with
                            | Some im ->
                                let team =
                                    { Id = tid
                                      LeaderId = invSlot.ChaId
                                      Members = Map.ofList [ (invSlot.ChaId, im) ] }
                                ctx.Teams.TryAdd(tid, team) |> ignore
                                invPlaying.TeamId <- tid
                                tid
                            | None -> 0

                    if teamId > 0 then
                        match ctx.Teams.TryGetValue(teamId) with
                        | true, team when team.Members.Count < ctx.Config.MaxTeamMembers ->
                            team.Members <- team.Members |> Map.add me.ChaId me
                            playing.TeamId <- teamId

                            // Удалить приглашение
                            ctx.Invitations.TryAcceptFrom(player.GpAddr, inviter.GpAddr, TeamInvite) |> ignore

                            sendTeamRefresh ctx team 1uy Array.empty
                            sendPmTeam ctx team 1uy

                            ctx.Logger.LogInformation("CP_TEAM_ACCEPT: {Name} вступил в команду {Id}",
                                player.CurrentChaName, teamId)
                        | true, _ ->
                            let mutable w = WPacket(16)
                            w.WriteCmd(Commands.CMD_PC_TEAM_CANCEL)
                            w.WriteInt64(4L)
                            ctx.SendToSingleClient player w
                        | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_TEAM_REFUSE — Отклонение приглашения
// ══════════════════════════════════════════════════════════

let handleCpTeamRefuse (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let inviterChaId = int (packet.ReadInt64())

    ctx.Invitations.TryAcceptFrom(player.GpAddr, 0u, TeamInvite) |> ignore

    match ctx.Registry.TryGetByChaId(inviterChaId) with
    | Some inviter when inviter.IsPlaying ->
        ctx.Logger.LogDebug("CP_TEAM_REFUSE: {Name} отклонил приглашение от {Id}",
            player.CurrentChaName, inviterChaId)
    | _ -> ()

// ══════════════════════════════════════════════════════════
//  CP_TEAM_LEAVE — Выход из команды
// ══════════════════════════════════════════════════════════

let handleCpTeamLeave (ctx: HandlerContext) (player: PlayerRecord) (_packet: IRPacket) =
    match getPlayingState player with
    | None -> ()
    | Some playing when playing.TeamId = 0 -> ()
    | Some playing ->
        match ctx.Teams.TryGetValue(playing.TeamId) with
        | false, _ -> playing.TeamId <- 0
        | true, team ->
            let chaSlot = playing.Auth.Characters[playing.CurrentChaIndex]
            let leavingMember =
                match team.Members.TryFind(chaSlot.ChaId) with
                | Some m -> [| m |]
                | None -> Array.empty

            removeFromTeam ctx playing team

            if team.Members.Count >= 2 then
                // Новый лидер если вышел лидер
                if team.LeaderId = chaSlot.ChaId then
                    let firstMember = team.Members |> Map.values |> Seq.head
                    team.LeaderId <- firstMember.ChaId

                sendTeamRefresh ctx team 2uy leavingMember
                sendPmTeam ctx team 2uy
            else
                // Команда расформирована (< 2 участников — обработано в removeFromTeam)
                // Отправить LEAVE последнему + покинувшему
                for m in leavingMember do
                    match ctx.Registry.TryGetByGpAddr(m.GpAddr) with
                    | Some p ->
                        let mutable w = WPacket(64)
                        w.WriteCmd(Commands.CMD_PC_TEAM_REFRESH)
                        w.WriteInt64(2L)
                        w.WriteInt64(0L)
                        ctx.SendToSingleClient p w
                    | None -> ()

            ctx.Logger.LogInformation("CP_TEAM_LEAVE: {Name} покинул команду", chaSlot.ChaName)

// ══════════════════════════════════════════════════════════
//  CP_TEAM_KICK — Исключение из команды
// ══════════════════════════════════════════════════════════

let handleCpTeamKick (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let targetChaId = int (packet.ReadInt64())

    match getPlayingState player with
    | None -> ()
    | Some playing when playing.TeamId = 0 -> ()
    | Some playing ->
        match ctx.Teams.TryGetValue(playing.TeamId) with
        | false, _ -> ()
        | true, team ->
            let chaSlot = playing.Auth.Characters[playing.CurrentChaIndex]
            if team.LeaderId <> chaSlot.ChaId then
                ctx.Logger.LogDebug("CP_TEAM_KICK: {Name} не лидер", chaSlot.ChaName)
            elif targetChaId = chaSlot.ChaId then
                ctx.Logger.LogDebug("CP_TEAM_KICK: {Name} пытается кикнуть себя", chaSlot.ChaName)
            else
                match team.Members.TryFind(targetChaId) with
                | None ->
                    ctx.Logger.LogDebug("CP_TEAM_KICK: {Id} не в команде", targetChaId)
                | Some kickedMember ->
                    // Обновляем TeamId кикнутого
                    match ctx.Registry.TryGetByGpAddr(kickedMember.GpAddr) with
                    | Some kickedPlayer ->
                        match getPlayingState kickedPlayer with
                        | Some kp -> kp.TeamId <- 0
                        | None -> ()
                    | None -> ()

                    team.Members <- team.Members |> Map.remove targetChaId

                    if team.Members.Count >= 2 then
                        sendTeamRefresh ctx team 6uy [| kickedMember |]
                        sendPmTeam ctx team 2uy
                    else
                        // Расформирование
                        for kv in team.Members do
                            match ctx.Registry.TryGetByGpAddr(kv.Value.GpAddr) with
                            | Some p ->
                                match getPlayingState p with
                                | Some lp -> lp.TeamId <- 0
                                | None -> ()
                            | None -> ()
                        ctx.Teams.TryRemove(team.Id) |> ignore

                        // Уведомить всех о расформировании
                        let mutable w = WPacket(64)
                        w.WriteCmd(Commands.CMD_PC_TEAM_REFRESH)
                        w.WriteInt64(6L)
                        w.WriteInt64(0L)

                        for kv in team.Members do
                            match ctx.Registry.TryGetByGpAddr(kv.Value.GpAddr) with
                            | Some p -> ctx.SendToSingleClient p (w.Clone())
                            | None -> ()

                        match ctx.Registry.TryGetByGpAddr(kickedMember.GpAddr) with
                        | Some p -> ctx.SendToSingleClient p (w.Clone())
                        | None -> ()

                    ctx.Logger.LogInformation("CP_TEAM_KICK: {Name} кикнул {Target}",
                        chaSlot.ChaName, kickedMember.ChaName)

// ══════════════════════════════════════════════════════════
//  MP_TEAM_CREATE — Создание команды из GameServer
// ══════════════════════════════════════════════════════════

let handleMpTeamCreate (ctx: HandlerContext) (packet: IRPacket) =
    let name1 = packet.ReadString()
    let name2 = packet.ReadString()

    match ctx.Registry.TryGetByChaName(name1), ctx.Registry.TryGetByChaName(name2) with
    | Some p1, Some p2 when p1.IsPlaying && p2.IsPlaying ->
        match getPlayingState p1, getPlayingState p2 with
        | Some pl1, Some pl2 when pl1.TeamId = 0 && pl2.TeamId = 0 ->
            match makeTeamMember p1, makeTeamMember p2 with
            | Some m1, Some m2 ->
                let tid = ctx.AllocateTeamId()
                let team =
                    { Id = tid
                      LeaderId = m1.ChaId
                      Members = Map.ofList [ (m1.ChaId, m1); (m2.ChaId, m2) ] }
                ctx.Teams.TryAdd(tid, team) |> ignore
                pl1.TeamId <- tid
                pl2.TeamId <- tid

                sendTeamRefresh ctx team 1uy Array.empty
                sendPmTeam ctx team 1uy

                ctx.Logger.LogInformation("MP_TEAM_CREATE: команда {Id} — {N1} + {N2}", tid, name1, name2)
            | _ -> ()
        | _ ->
            ctx.Logger.LogDebug("MP_TEAM_CREATE: {N1} или {N2} уже в команде", name1, name2)
    | _ ->
        ctx.Logger.LogDebug("MP_TEAM_CREATE: {N1} или {N2} не найден", name1, name2)
