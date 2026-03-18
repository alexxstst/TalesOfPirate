namespace Corsairs.GroupServer.Services

open System
open System.Collections.Concurrent
open System.Net
open System.Threading
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options
open Microsoft.Extensions.DependencyInjection
open Corsairs.GroupServer.Config
open Corsairs.GroupServer.Domain
open System.Linq
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Database

module Auth = Corsairs.GroupServer.Services.Handlers.AuthHandlers
module Cha = Corsairs.GroupServer.Services.Handlers.CharacterHandlers
module Team = Corsairs.GroupServer.Services.Handlers.TeamHandlers
module Frnd = Corsairs.GroupServer.Services.Handlers.FriendHandlers
module Master = Corsairs.GroupServer.Services.Handlers.MasterHandlers
module Chat = Corsairs.GroupServer.Services.Handlers.ChatHandlers
module Guild = Corsairs.GroupServer.Services.Handlers.GuildHandlers
module PInfo = Corsairs.GroupServer.Services.Handlers.PersonInfoHandlers
module Admin = Corsairs.GroupServer.Services.Handlers.AdminHandlers
module Ranking = Corsairs.GroupServer.Services.Handlers.RankingHandlers

/// Система подключений GateServer (TCP-листенер).
/// Аналог GameServerSystem из F# GateServer.
type GateServerSystem
    (
        config: IOptions<GroupServerConfig>,
        loggerFactory: ILoggerFactory,
        scopeFactory: IServiceScopeFactory,
        registry: PlayerRegistry,
        invitations: InvitationManager
    ) as this =

    let cfg = config.Value
    let logger = loggerFactory.CreateLogger<GateServerSystem>()

    // TCP-листенер для GateServer подключений
    let gateEp = IPEndPoint(IPAddress.Parse(cfg.ListenAddress), cfg.GateListenPort)

    let system =
        DirectSystemCommand<GateServerIO>(
            { Endpoints = [| gateEp |] },
            (fun (socket, handler) -> GateServerIO(socket, handler)),
            loggerFactory
        )

    // Слоты для гейтов (аналог std::array<GateServer, GATE_MAX>)
    let _gates: GateServerIO array = Array.zeroCreate cfg.MaxGates
    let mutable _accountSystem: IAccountServerSystem = null
    let mutable _handlerCtx: HandlerContext option = None

    // ── SESS-корреляция ──
    static let SESS_FLAG = 0x80000000u

    // ── In-memory state ──
    let _guilds = ConcurrentDictionary<int, GuildData>()
    let _teams = ConcurrentDictionary<int, TeamData>()
    let _chatSessions = ConcurrentDictionary<uint32, ChatSessionData>()
    let _masterRelations = ConcurrentDictionary<struct (int * int), bool>()
    let _gmBbs = ConcurrentDictionary<int, GmBbsEntry>()
    let _ranking = Array.init MaxOrderNum (fun _ -> OrderInfo.empty ())
    let mutable _nextTeamId = 0
    let mutable _nextSessionId = 0u

    do
        system.OnConnected.Add(fun ch ->
            ch.State <- GateConnected_
            logger.LogInformation("GateServer подключён: {Ep}", ch.RemoteEndPoint))

        system.OnCommand.Add(fun (ch, packet) ->
            logger.LogInformation("IN << {Channel} {Packet}", ch, packet)
            let sess = packet.Sess
            if sess > 0u && sess < SESS_FLAG then
                this.HandleServeCall(ch, sess, packet)
            elif sess = 0u || sess = SESS_FLAG then
                this.OnProcessData(ch, packet)
            else
                // Ответ от AccountServer relay (не используется напрямую)
                logger.LogDebug("GateServerSystem: SESS ответ {Sess}", sess))

        system.OnDisconnected.Add(fun ch ->
            this.OnGateDisconnected(ch))

    /// Получить HandlerContext.
    member this.GetCtx() =
        match _handlerCtx with
        | Some ctx -> ctx
        | None ->
            let ctx =
                { Registry = registry
                  Invitations = invitations
                  Config = cfg
                  AccountSystem = _accountSystem
                  ScopeFactory = scopeFactory
                  Logger = logger
                  Gates = _gates
                  Guilds = _guilds
                  Teams = _teams
                  ChatSessions = _chatSessions
                  MasterRelations = _masterRelations
                  Ranking = _ranking
                  SendRpcResponse = fun ch sess wpk -> this.SendRpcResponse(ch, sess, wpk)
                  SendToSingleClient = fun player wpk -> this.SendToSingleClient(player, wpk)
                  SendToClients = fun players wpk -> this.SendToClients(players, wpk)
                  SendToAllClients = fun wpk -> this.SendToAllClients(wpk)
                  KickUser = fun gpA gtA -> this.KickUser(gpA, gtA)
                  AllocateGpAddr = registry.AllocateGpAddr
                  AllocateTeamId = this.AllocateTeamId
                  AllocateSessionId = this.AllocateSessionId }
            _handlerCtx <- Some ctx
            ctx

    /// Извлечь PlayerRecord из пакета (gpAddr в trailer).
    /// Trailer-поля НЕ отрезаются — handlers используют Deserialize для чтения всех полей.
    member private _.TryGetPlayerFromPacket(packet: IRPacket) =
        let gpAddr = uint32 (packet.ReverseReadInt64())
        let _gtAddr = uint32 (packet.ReverseReadInt64())
        registry.TryGetByGpAddr(gpAddr)

    // ── HandleServeCall: RPC-диспатч ──
    member private this.HandleServeCall(ch: GateServerIO, sess: uint32, packet: IRPacket) =
        let cmd = packet.GetCmd()
        let ctx = this.GetCtx()
        logger.LogDebug("HandleServeCall: cmd={Cmd} sess={Sess}", cmd, sess)

        match cmd with
        // Команды без сессии игрока
        | Commands.CMD_TP_LOGIN ->
            Auth.handleTpLogin ctx ch sess packet
        | Commands.CMD_TP_USER_LOGIN ->
            Auth.handleTpUserLogin ctx ch sess packet
        | Commands.CMD_TP_REQPLYLST ->
            Auth.handleTpReqPlylst ctx ch sess packet
        | Commands.CMD_TP_SYNC_PLYLST ->
            Auth.handleTpSyncPlylst ctx ch sess packet
        | Commands.CMD_TP_REGISTER ->
            Auth.handleTpRegister ctx ch sess packet
        | Commands.CMD_OS_LOGIN ->
            Auth.handleOsLogin ctx ch sess packet
        // Команды с сессией игрока (player pointer в trailer)
        | Commands.CMD_TP_USER_LOGOUT
        | Commands.CMD_TP_BGNPLAY
        | Commands.CMD_TP_ENDPLAY
        | Commands.CMD_TP_NEWCHA
        | Commands.CMD_TP_DELCHA
        | Commands.CMD_TP_CREATE_PASSWORD2
        | Commands.CMD_TP_UPDATE_PASSWORD2
        | Commands.CMD_TP_CHANGEPASS ->
            match this.TryGetPlayerFromPacket(packet) with
            | None ->
                logger.LogWarning("HandleServeCall: игрок не найден для cmd={Cmd}", cmd)
                let mutable w = WPacket(16)
                w.WriteCmd(cmd)
                w.WriteInt64(int64 Commands.ERR_PT_INERR)
                this.SendRpcResponse(ch, sess, w)
            | Some player ->
                match cmd with
                | Commands.CMD_TP_USER_LOGOUT ->
                    Auth.handleTpUserLogout ctx ch sess player
                | Commands.CMD_TP_BGNPLAY ->
                    Cha.handleTpBgnplay ctx ch sess player packet
                | Commands.CMD_TP_ENDPLAY ->
                    Cha.handleTpEndplay ctx ch sess player packet
                | Commands.CMD_TP_NEWCHA ->
                    Cha.handleTpNewcha ctx ch sess player packet
                | Commands.CMD_TP_DELCHA ->
                    Cha.handleTpDelcha ctx ch sess player packet
                | Commands.CMD_TP_CREATE_PASSWORD2 ->
                    Auth.handleTpCreatePassword2 ctx ch sess player packet
                | Commands.CMD_TP_UPDATE_PASSWORD2 ->
                    Auth.handleTpUpdatePassword2 ctx ch sess player packet
                | Commands.CMD_TP_CHANGEPASS ->
                    Auth.handleTpChangepass ctx ch sess player packet
                | _ -> ()
        | Commands.CMD_TP_DISC ->
            Auth.handleTpDisc ctx ch packet
        | _ ->
            logger.LogWarning("HandleServeCall: неизвестная команда {Cmd}", cmd)

    // ── OnProcessData: async-диспатч ──
    member private this.OnProcessData(ch: GateServerIO, packet: IRPacket) =
        let cmd = packet.GetCmd()
        let ctx = this.GetCtx()
        logger.LogDebug("OnProcessData: cmd={Cmd}", cmd)

        match cmd with
        // ── MP_* от GameServer (без trailer) ──
        | Commands.CMD_MP_TEAM_CREATE ->
            Team.handleMpTeamCreate ctx packet
        | Commands.CMD_MP_MASTER_CREATE ->
            Master.handleMpMasterCreate ctx packet
        | Commands.CMD_MP_MASTER_DEL ->
            Master.handleMpMasterDel ctx packet
        | Commands.CMD_MP_MASTER_FINISH ->
            Master.handleMpMasterFinish ctx packet
        // MP_* от GameServer с player в trailer
        | Commands.CMD_MP_ENTERMAP
        | Commands.CMD_MP_GUILD_CREATE
        | Commands.CMD_MP_GUILD_APPROVE
        | Commands.CMD_MP_GUILD_KICK
        | Commands.CMD_MP_GUILD_LEAVE
        | Commands.CMD_MP_GUILD_DISBAND
        | Commands.CMD_MP_GUILD_MOTTO
        | Commands.CMD_MP_GUILD_PERM
        | Commands.CMD_MP_GARNER2_UPDATE
        | Commands.CMD_MP_GARNER2_CGETORDER ->
            match this.TryGetPlayerFromPacket(packet) with
            | None ->
                logger.LogWarning("OnProcessData: игрок не найден для MP cmd={Cmd}", cmd)
            | Some player ->
                match cmd with
                | Commands.CMD_MP_ENTERMAP ->
                    PInfo.handleMpEntermap ctx player packet
                | Commands.CMD_MP_GUILD_CREATE ->
                    Guild.handleMpGuildCreate ctx player packet
                | Commands.CMD_MP_GUILD_APPROVE ->
                    Guild.handleMpGuildApprove ctx player packet
                | Commands.CMD_MP_GUILD_KICK ->
                    Guild.handleMpGuildKick ctx player packet
                | Commands.CMD_MP_GUILD_LEAVE ->
                    Guild.handleMpGuildLeave ctx player packet
                | Commands.CMD_MP_GUILD_DISBAND ->
                    Guild.handleMpGuildDisband ctx player packet
                | Commands.CMD_MP_GUILD_MOTTO ->
                    Guild.handleMpGuildMotto ctx player packet
                | Commands.CMD_MP_GUILD_PERM ->
                    Guild.handleMpGuildPerm ctx player packet
                | Commands.CMD_MP_GARNER2_UPDATE ->
                    Ranking.handleMpGarner2Update ctx player packet
                | Commands.CMD_MP_GARNER2_CGETORDER ->
                    Ranking.handleMpGarner2GetOrder ctx player packet
                | _ -> ()

        // MP_* broadcast (от GameServer, без trailer)
        | Commands.CMD_MP_SAY2ALL ->
            Chat.handleMpSay2All ctx packet
        | Commands.CMD_MP_SAY2TRADE ->
            Chat.handleMpSay2Trade ctx packet
        | Commands.CMD_MP_GM1SAY ->
            Chat.handleMpGm1Say ctx packet
        | Commands.CMD_MP_GM1SAY1 ->
            Chat.handleMpGm1Say1 ctx packet
        // MP_* admin (от GameServer, без trailer)
        | Commands.CMD_MP_GMBANACCOUNT ->
            Admin.handleMpGmbanaccount ctx packet
        | Commands.CMD_MP_GMUNBANACCOUNT ->
            Admin.handleMpGmunbanaccount ctx packet
        | Commands.CMD_MP_MUTE_PLAYER ->
            Admin.handleMpMutePlayer ctx packet
        | Commands.CMD_MP_CANRECEIVEREQUESTS ->
            Admin.handleMpCanreceiverequests ctx packet
        | Commands.CMD_MP_GUILDNOTICE ->
            Admin.handleMpGuildnotice ctx packet

        // ── CP_* от клиента (через trailer) ──
        | Commands.CMD_CP_TEAM_INVITE
        | Commands.CMD_CP_TEAM_ACCEPT
        | Commands.CMD_CP_TEAM_REFUSE
        | Commands.CMD_CP_TEAM_LEAVE
        | Commands.CMD_CP_TEAM_KICK
        | Commands.CMD_CP_FRND_INVITE
        | Commands.CMD_CP_FRND_ACCEPT
        | Commands.CMD_CP_FRND_REFUSE
        | Commands.CMD_CP_FRND_DELETE
        | Commands.CMD_CP_FRND_CHANGE_GROUP
        | Commands.CMD_CP_FRND_REFRESH_INFO
        | Commands.CMD_CP_MASTER_REFRESH_INFO
        | Commands.CMD_CP_PRENTICE_REFRESH_INFO
        | Commands.CMD_CP_SAY2ALL
        | Commands.CMD_CP_SAY2TRADE
        | Commands.CMD_CP_SAY2YOU
        | Commands.CMD_CP_SAY2TEM
        | Commands.CMD_CP_SAY2GUD
        | Commands.CMD_CP_GM1SAY
        | Commands.CMD_CP_GM1SAY1
        | Commands.CMD_CP_SESS_CREATE
        | Commands.CMD_CP_SESS_SAY
        | Commands.CMD_CP_SESS_ADD
        | Commands.CMD_CP_SESS_LEAVE
        | Commands.CMD_CP_REFUSETOME
        | Commands.CMD_CP_CHANGE_PERSONINFO
        | Commands.CMD_CP_PING
        | Commands.CMD_CP_REPORT_WG ->
            match this.TryGetPlayerFromPacket(packet) with
            | None ->
                logger.LogWarning("OnProcessData: игрок не найден для cmd={Cmd}", cmd)
            | Some player ->
                match cmd with
                // Team
                | Commands.CMD_CP_TEAM_INVITE ->
                    Team.handleCpTeamInvite ctx player packet
                | Commands.CMD_CP_TEAM_ACCEPT ->
                    Team.handleCpTeamAccept ctx player packet
                | Commands.CMD_CP_TEAM_REFUSE ->
                    Team.handleCpTeamRefuse ctx player packet
                | Commands.CMD_CP_TEAM_LEAVE ->
                    Team.handleCpTeamLeave ctx player packet
                | Commands.CMD_CP_TEAM_KICK ->
                    Team.handleCpTeamKick ctx player packet
                // Friends
                | Commands.CMD_CP_FRND_INVITE ->
                    Frnd.handleCpFrndInvite ctx player packet
                | Commands.CMD_CP_FRND_ACCEPT ->
                    Frnd.handleCpFrndAccept ctx player packet
                | Commands.CMD_CP_FRND_REFUSE ->
                    Frnd.handleCpFrndRefuse ctx player packet
                | Commands.CMD_CP_FRND_DELETE ->
                    Frnd.handleCpFrndDelete ctx player packet
                | Commands.CMD_CP_FRND_CHANGE_GROUP ->
                    Frnd.handleCpFrndChangeGroup ctx player packet
                | Commands.CMD_CP_FRND_REFRESH_INFO ->
                    Frnd.handleCpFrndRefreshInfo ctx player packet
                // Master
                | Commands.CMD_CP_MASTER_REFRESH_INFO ->
                    Master.handleCpMasterRefreshInfo ctx player packet
                | Commands.CMD_CP_PRENTICE_REFRESH_INFO ->
                    Master.handleCpPrenticeRefreshInfo ctx player packet
                // Chat
                | Commands.CMD_CP_SAY2ALL ->
                    Chat.handleCpSay2All ctx player packet
                | Commands.CMD_CP_SAY2TRADE ->
                    Chat.handleCpSay2Trade ctx player packet
                | Commands.CMD_CP_SAY2YOU ->
                    Chat.handleCpSay2You ctx player packet
                | Commands.CMD_CP_SAY2TEM ->
                    Chat.handleCpSay2Tem ctx player packet
                | Commands.CMD_CP_SAY2GUD ->
                    Chat.handleCpSay2Gud ctx player packet
                | Commands.CMD_CP_GM1SAY ->
                    Chat.handleCpGm1Say ctx player packet
                | Commands.CMD_CP_GM1SAY1 ->
                    Chat.handleCpGm1Say1 ctx player packet
                // Sessions
                | Commands.CMD_CP_SESS_CREATE ->
                    Chat.handleCpSessCreate ctx player packet
                | Commands.CMD_CP_SESS_SAY ->
                    Chat.handleCpSessSay ctx player packet
                | Commands.CMD_CP_SESS_ADD ->
                    Chat.handleCpSessAdd ctx player packet
                | Commands.CMD_CP_SESS_LEAVE ->
                    Chat.handleCpSessLeave ctx player packet
                // PersonInfo
                | Commands.CMD_CP_REFUSETOME ->
                    Chat.handleCpRefuseToMe ctx player packet
                | Commands.CMD_CP_CHANGE_PERSONINFO ->
                    PInfo.handleCpChangePersoninfo ctx player packet
                | Commands.CMD_CP_PING ->
                    PInfo.handleCpPing ctx player packet
                // Admin
                | Commands.CMD_CP_REPORT_WG ->
                    Admin.handleCpReportWg ctx player packet
                | _ -> ()
        | _ ->
            logger.LogDebug("OnProcessData: неизвестная команда {Cmd}", cmd)

    // ── Отключение GateServer ──
    member private this.OnGateDisconnected(ch: GateServerIO) =
        logger.LogInformation("GateServer отключён: {Name}", ch.Name)
        let idx = ch.GateIndex
        if idx >= 0 && idx < _gates.Length then
            _gates[idx] <- null
        // TODO: EndPlay всех игроков этого гейта

    // ── Публичные свойства для обработчиков ──
    member _.Registry = registry
    member _.Invitations = invitations
    member _.Config = cfg
    member _.AccountSystem = _accountSystem
    member _.ScopeFactory = scopeFactory
    member _.Guilds = _guilds
    member _.Teams = _teams
    member _.ChatSessions = _chatSessions
    member _.MasterRelations = _masterRelations
    member _.GmBbs = _gmBbs
    member _.Gates = _gates
    member _.Logger = logger

    member _.AllocateTeamId() = Interlocked.Increment(&_nextTeamId)
    member _.AllocateSessionId() = Interlocked.Increment(&_nextSessionId) |> uint32

    /// Отправить RPC-ответ.
    member _.SendRpcResponse(ch: GateServerIO, sess: uint32, wpk: WPacket) =
        wpk.WriteSess(sess ||| SESS_FLAG)
        ch.SendPacket(wpk)

    /// Отправить пакет одному клиенту через GateServer (с trailer).
    member _.SendToSingleClient(player: PlayerRecord, wpk: WPacket) =
        let gate = player.Gate
        if not (isNull gate) && gate.IsRegistered then
            wpk.WriteInt64(int64 player.GpAddr)
            wpk.WriteInt64(int64 player.GateAddr)
            wpk.WriteInt64(1L)
            gate.SendPacket(wpk)

    /// Отправить пакет нескольким клиентам (группировка по гейтам, trailer).
    member _.SendToClients(players: PlayerRecord array, wpk: WPacket) =
        let groups =
            players
            |> Array.filter (fun p -> p.IsPlaying && not (isNull p.Gate) && p.Gate.IsRegistered)
            |> Array.groupBy (fun p -> p.Gate)
        for gate, gatePlayers in groups do
            let mutable clone = wpk.Clone()
            for p in gatePlayers do
                clone.WriteInt64(int64 p.GpAddr)
                clone.WriteInt64(int64 p.GateAddr)
            clone.WriteInt64(int64 gatePlayers.Length)
            gate.SendPacket(clone)

    /// Broadcast пакет всем онлайн-клиентам.
    member this.SendToAllClients(wpk: WPacket) =
        let players = registry.GetAllPlayers() |> Seq.filter (fun p -> p.IsPlaying) |> Seq.toArray
        if players.Length > 0 then
            this.SendToClients(players, wpk)

    /// Кик игрока.
    member _.KickUser(gpAddr: uint32, gtAddr: uint32) =
        let mutable w = WPacket(32)
        w.WriteCmd(Commands.CMD_PT_KICKUSER)
        w.WriteInt64(int64 gpAddr)
        w.WriteInt64(int64 gtAddr)
        w.WriteInt64(1L)
        match registry.TryGetByGpAddr(gpAddr) with
        | Some player ->
            let gate = player.Gate
            if not (isNull gate) && gate.IsRegistered then
                gate.SendPacket(w)
        | None -> ()

    /// Загрузить рейтинг из БД при старте.
    member this.LoadRanking() =
        Ranking.loadParam (this.GetCtx()) _ranking

    /// Загрузить гильдии из БД при старте (аналог C++ TBLGuilds::InitAllGuilds).
    member _.LoadGuildsFromDb() =
        try
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            // Загружаем все гильдии
            let guilds = db.Guilds.ToArray()
            for g in guilds do
                // Загружаем участников гильдии (все персонажи с данным GuildId)
                // guild_id = 0 в legacy означает «нет гильдии», конвертер не используется
                let guildId = g.Id
                let memberChars = db.GetGuildMembers(guildId)

                let members =
                    memberChars
                    |> Array.fold (fun (acc: Map<int, GuildMemberInfo>) c ->
                        acc.Add(c.Id,
                            { ChaId = c.Id
                              ChaName = c.Name
                              Permission = uint32 c.GuildPermissions }))
                        Map.empty

                let guildData =
                    { Id = g.Id
                      Name = g.Name
                      Motto = defaultArg (Option.ofObj g.Motto) ""
                      LeaderId = g.LeaderCharacterId
                      Status = GuildStatus.None
                      RemainMinute =
                        if g.DisbandScheduledAt.HasValue then
                            int (g.DisbandScheduledAt.Value - DateTimeOffset.UtcNow).TotalMinutes |> max 0
                        else 0
                      Tick = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds()
                      Members = members
                      ChallMoney = int g.ChallengeMoney
                      ChallPrizeMoney = 0 }

                _guilds[g.Id] <- guildData

            logger.LogInformation("LoadGuildsFromDb: загружено {Count} гильдий", _guilds.Count)
        with ex ->
            logger.LogError(ex, "LoadGuildsFromDb: ошибка загрузки гильдий")

    /// Загрузить связи мастер-ученик из БД при старте (аналог C++ InitMasterRelation).
    member _.LoadMasterRelationsFromDb() =
        try
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

            let mentorships = db.GetActiveMentorships()

            for m in mentorships do
                _masterRelations[struct (m.PrenticeCharacterId, m.MasterCharacterId)] <- true

            logger.LogInformation("LoadMasterRelationsFromDb: загружено {Count} связей", _masterRelations.Count)
        with ex ->
            logger.LogError(ex, "LoadMasterRelationsFromDb: ошибка загрузки мастер-связей")

    // ── Связывание систем ──
    member _.SetSystems(account: IAccountServerSystem) =
        _accountSystem <- account

    // ── Lifecycle ──
    member _.Start(ct: CancellationToken) =
        system.Start(ct)
        logger.LogInformation("GateServerSystem: ожидает подключения на {Ep}", gateEp)

    interface IGateServerSystem with
        member this.SendToSingleClient(player, packet) = this.SendToSingleClient(player, packet)
        member this.SendToClients(players, packet) = this.SendToClients(players, packet)
        member this.SendToAllClients(packet) = this.SendToAllClients(packet)
        member this.KickUser(gpAddr, gtAddr) = this.KickUser(gpAddr, gtAddr)
        member _.FindGateByName(name) =
            _gates |> Array.tryFind (fun g -> not (isNull g) && g.IsRegistered && g.Name = name)
