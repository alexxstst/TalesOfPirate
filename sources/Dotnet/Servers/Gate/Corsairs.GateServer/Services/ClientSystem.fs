namespace Corsairs.GateServer.Services

open System
open System.Collections.Concurrent
open System.Net
open System.Threading
open Corsairs.GateServer.Domain
open Corsairs.GateServer.Config
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Crypto
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options

/// Active patterns для классификации клиентских команд.
module private ClientCommandPatterns =

    /// Команда в диапазоне Client → GameServer (CM, 0..499).
    let (|CmRange|_|) (cmd: uint16) : unit option =
        if cmd / 500us = Commands.CMD_CM_BASE / 500us then Some() else None

    /// Команда в диапазоне Client → GroupServer (CP, 6000..6499).
    let (|CpRange|_|) (cmd: uint16) : unit option =
        if cmd / 500us = Commands.CMD_CP_BASE / 500us then Some() else None

/// Система управления клиентскими подключениями (аналог ToClient в C++).
/// Владеет DirectSystemCommand для TCP-листенера и обработки событий.
type ClientSystem
    (config: IOptions<ClientConfig>, gateConfig: IOptions<GateConfig>, loggerFactory: ILoggerFactory) as this
    =

    let cfg = config.Value
    let gateCfg = gateConfig.Value
    let logger = loggerFactory.CreateLogger<ClientSystem>()

    let system =
        new DirectSystemCommand<PlayerChannelIO>(
            { Endpoints = [| IPEndPoint(IPAddress.Parse(cfg.Address), cfg.Port) |] },
            (fun (socket, handler) -> PlayerChannelIO(socket, handler)),
            loggerFactory
        )

    let _version = uint16 cfg.ClientVersion
    let _wpeEnabled = cfg.WpeProtection
    let _rsaAes = cfg.RsaAes <> 0
    let _rsa = if _rsaAes then new RsaKeyExchange() else null
    let _rsaPublicKey = if not (isNull _rsa) then _rsa.ExportPublicKey() else null
    let _players = ConcurrentDictionary<ChannelId, PlayerChannelIO>()
    let mutable _groupSystem: IGroupServerSystem = null
    let mutable _gameSystem: IGameServerSystem = null
    let mutable _syncTimer: Timer = null

    // ── Chaos-карта ──
    let _chaosActive = cfg.ChaosActive
    let _chaosMap = cfg.ChaosMap

    do
        system.OnConnected.Add(fun ch -> this.OnConnect(ch))

        system.OnCommand.Add(fun (ch, packet) ->
            try
                let decrypted = ch.DecryptIncoming(packet)

                try
                    logger.LogDebug(
                        "RECV Client {Packet} Ch={Ch}, Server counter: {Counter}",
                        decrypted,
                        ch.Id,
                        ch.PacketCounter
                    )

                    this.OnProcessData(ch, decrypted)
                finally
                    if not (obj.ReferenceEquals(decrypted, packet)) then
                        decrypted.Dispose()
            finally
                packet.Dispose())

        system.OnDisconnected.Add(fun ch -> this.OnDisconnect(ch))

    /// Установить ссылки на соседние системы (вызывается GateHostedService перед запуском).
    member _.SetSystems(group: IGroupServerSystem, game: IGameServerSystem) =
        _groupSystem <- group
        _gameSystem <- game

    /// Система GroupServer.
    member _.GroupSystem = _groupSystem

    /// Система GameServer.
    member _.GameSystem = _gameSystem

    // ══════════════════════════════════════════════════════════
    //  Подключение / отключение
    // ══════════════════════════════════════════════════════════

    /// Обработать новое клиентское подключение.
    member private this.OnConnect(channel: PlayerChannelIO) =
        _players[channel.Id] <- channel

        if _rsaAes then
            this.SendHandshake(channel)
        else
            // Шифрование выключено — отправляем пустой handshake (keySize=0),
            // чтобы клиент перешёл в CONNECTED без RSA/AES
            let mutable w = WPacket(4)
            w.WriteCmd(Commands.CMD_MC_SEND_SERVER_PUBLIC_KEY)
            w.WriteSequence(ReadOnlySpan<byte>.Empty)
            channel.SendPacket(w)

        logger.LogInformation(
            "Клиент подключён: Ch#{ChannelId} с {RemoteEndPoint}",
            channel.Id,
            channel.RemoteEndPoint
        )

    /// Обработать отключение клиента. Вызывает CM_LOGOUT.
    member private this.OnDisconnect(channel: PlayerChannelIO) =
        this.CM_LOGOUT(channel)
        _players.TryRemove(channel.Id) |> ignore

        logger.LogInformation(
            "Клиент отключён: Ch#{ChannelId} [{State}]",
            channel.Id,
            channel.State
        )

    // ══════════════════════════════════════════════════════════
    //  Утилиты
    // ══════════════════════════════════════════════════════════

    /// Отправить ошибку клиенту: WPacket с Cmd + errCode + текстовое описание.
    member private _.SendError(player: PlayerChannelIO, cmd: uint16, err: int16) =
        let desc =
            match err with
            | Commands.ERR_MC_NETEXCP      -> "Network error"
            | Commands.ERR_MC_NOTSELCHA    -> "Not in character selection"
            | Commands.ERR_MC_NOTPLAY      -> "Not in game"
            | Commands.ERR_MC_NOTARRIVE    -> "Map not available"
            | Commands.ERR_MC_TOOMANYPLY   -> "Too many players on server"
            | Commands.ERR_MC_NOTLOGIN     -> "Not logged in"
            | Commands.ERR_MC_VER_ERROR    -> "Client version mismatch"
            | Commands.ERR_MC_ENTER_ERROR  -> "Map enter error"
            | Commands.ERR_MC_ENTER_POS    -> "Invalid map position"
            | Commands.ERR_MC_BANUSER      -> "Account banned"
            | Commands.ERR_MC_PBANUSER     -> "Account banned (protection)"
            | Commands.ERR_PT_LOGFAIL      -> "Gate registration failed"
            | Commands.ERR_PT_SAMEGATENAME -> "Gate name already registered"
            | Commands.ERR_PT_INVALIDDAT   -> "Invalid data format"
            | Commands.ERR_PT_INERR        -> "Internal server error"
            | Commands.ERR_PT_NETEXCP      -> "Group-Account connection error"
            | Commands.ERR_PT_DBEXCP       -> "Database error"
            | Commands.ERR_PT_INVALIDCHA   -> "Character not owned by account"
            | Commands.ERR_PT_TOMAXCHA     -> "Character limit reached"
            | Commands.ERR_PT_SAMECHANAME  -> "Character name already taken"
            | Commands.ERR_PT_INVALIDBIRTH -> "Invalid class/profession"
            | Commands.ERR_PT_TOOBIGCHANM  -> "Character name too long"
            | Commands.ERR_PT_KICKUSER     -> "Kicked by server"
            | Commands.ERR_PT_ISGLDLEADER  -> "Cannot delete guild leader"
            | Commands.ERR_PT_ERRCHANAME   -> "Invalid character name"
            | Commands.ERR_PT_SERVERBUSY   -> "Server is busy"
            | Commands.ERR_PT_TOOBIGPW2    -> "Second password too long"
            | Commands.ERR_PT_INVALID_PW2  -> "Invalid second password"
            | Commands.ERR_PT_BANUSER      -> "Account banned"
            | Commands.ERR_PT_PBANUSER     -> "Account banned (protection)"
            | Commands.ERR_PT_GMISLOG      -> "GM already logged in"
            | Commands.ERR_PT_MULTICHA     -> "Multi-character not allowed"
            | Commands.ERR_PT_BONUSCHARS   -> "Bonus character slots"
            | Commands.ERR_PT_BADBOY       -> "Negative reputation (PK)"
            | _ -> $"Unknown error ({err})"

        let w = WPacket(16 + desc.Length)
        w.WriteCmd(cmd)
        w.WriteInt64(int64 err)
        w.WriteString(desc)
        player.SendPacket(w)

    /// Переслать пакет на GameServer с trailer (gateAddr, gmAddr).
    member private _.ForwardToGame
        (player: PlayerChannelIO, playing: PlayerPlaying, packet: IRPacket)
        =
        if not (isNull playing.GameServer) then
            let w = WPacket(packet)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)

            match playing.GameServerPlayerId with
            | Some(GameServerPlayerId_ gmAddr) -> w.WriteInt64(int64 gmAddr)
            | None -> w.WriteInt64(0L)

            playing.GameServer.SendPacket(w)

    /// Переслать пакет на GroupServer с trailer (gateAddr, gpAddr).
    member private this.ForwardToGroup
        (player: PlayerChannelIO, playing: PlayerPlaying, packet: IRPacket)
        =
        let w = WPacket(packet)
        let (ChannelId_ rawId) = player.Id
        w.WriteInt64(int64 rawId)
        w.WriteInt64(int64 playing.GroupServerPlayerId)
        this.SendToGroup(w)

    /// Отправить WPacket на GroupServer (через канал напрямую).
    member private _.SendToGroup(w: WPacket) =
        let ch = _groupSystem.Channel

        if not (isNull ch) then
            ch.SendPacket(w)
        else
            w.Dispose()
            logger.LogWarning("GroupServer не подключён, пакет потерян")

    /// SyncCall к GroupServer с обработкой отсутствия подключения.
    member private _.SyncCallGroup(packet: WPacket, callback: IRPacket option -> unit) =
        let ch = _groupSystem.Channel

        if not (isNull ch) then
            _groupSystem.SyncCall(ch, packet, callback)
        else
            packet.Dispose()
            callback None

    // ══════════════════════════════════════════════════════════
    //  RSA-AES Handshake (порт из ToClient::OnConnected + CMD_CM_SEND_PRIVATE_KEY)
    // ══════════════════════════════════════════════════════════

    /// Отправить RSA public key клиенту (CMD_MC_SEND_SERVER_PUBLIC_KEY).
    /// C++ аналог: ToClient::OnConnected при g_rsaaes.
    member private _.SendHandshake(player: PlayerChannelIO) =
        let mutable w = WPacket(4 + _rsaPublicKey.Length)
        w.WriteCmd(Commands.CMD_MC_SEND_SERVER_PUBLIC_KEY)
        w.WriteSequence(ReadOnlySpan<byte>(_rsaPublicKey))
        player.SendPacket(w)
        logger.LogDebug("Handshake: RSA public key отправлен Ch#{Id}", player.Id)

    /// CMD_CM_SEND_PRIVATE_KEY: клиент прислал зашифрованный AES-ключ.
    /// Дешифруем RSA → создаём AesTransport → активируем шифрование на канале.
    member private _.CM_SEND_PRIVATE_KEY(player: PlayerChannelIO, packet: IRPacket) =
        if isNull _rsa then
            logger.LogWarning("CM_SEND_PRIVATE_KEY: RSA не инициализирован")
            player.Close()
        else
            let encryptedAesKey = packet.ReadSequence().ToArray()

            try
                let aesKey = _rsa.DecryptSessionKey(encryptedAesKey)
                let transport = new AesTransport(aesKey)
                player.SetCrypto(transport)
                logger.LogInformation("Handshake: AES-256-GCM активирован для Ch#{Id}", player.Id)
            with ex ->
                logger.LogError(
                    ex,
                    "CM_SEND_PRIVATE_KEY: ошибка дешифровки для Ch#{Id}, KeySize: {KeySize},
\nHexKey: {HexKey},\nDataSize: {DataSize},\nCommand: {Command}",
                    player.Id,
                    encryptedAesKey.Length,
                    encryptedAesKey.ToHexString(),
                    packet.PayloadLength,
                    packet.GetPacketMemory().ToHexString()
                )

                player.Close()

    // ══════════════════════════════════════════════════════════
    //  WPE-защита (порт из ToClient::OnProcessData)
    // ══════════════════════════════════════════════════════════

    /// WPE pre-processing: удаление 4-байтного счётчика из пакета + опциональная проверка.
    /// C++ SDK WriteCmd() ВСЕГДА пишет 4-байт counter для клиентских команд (1-500, 6000-6500).
    /// Счётчик нужно удалять всегда. При включённом WPE — дополнительно проверяем значение.
    member private _.OnPreProcessingCommand
        (ch: PlayerChannelIO, packet: IRPacket)
        : IRPacket voption =
        if packet.GetPacketSize() < 8 then
            logger.LogWarning(
                "WPE: пакет слишком мал от Ch#{Channel} (DataSize={Size})",
                ch.Id,
                packet.PayloadLength
            )

            ValueNone
        else
            let counter = packet.Sess

            if _wpeEnabled && counter <> ch.PacketCounter then
                logger.LogWarning(
                    "WPE: счётчик не совпадает Ch#{Channel}: ожидался {Expected}, получен {Actual}, CMD={Cmd}",
                    ch.Id,
                    ch.PacketCounter,
                    counter,
                    packet.GetCmd()
                )

                ch.Close()
                ValueNone
            else
                if _wpeEnabled then
                    ch.IncrementPacketCounter()

                ValueSome(packet)
    // ══════════════════════════════════════════════════════════
    //  Диспатч команд (OnProcessData)
    // ══════════════════════════════════════════════════════════

    member private this.OnProcessData(ch: PlayerChannelIO, packet: IRPacket) =
        match this.OnPreProcessingCommand(ch, packet) with
        | ValueNone -> logger.LogDebug("Отброшен пакет для Ch#{ChannelId}", ch.Id)
        | ValueSome processed ->

            let cmd = processed.GetCmd()

            match cmd with
            // ── RSA-AES Handshake ──
            | Commands.CMD_CM_SEND_PRIVATE_KEY -> this.CM_SEND_PRIVATE_KEY(ch, processed)

            // ── Команды авторизации / персонажей (SyncCall к GroupServer) ──
            | Commands.CMD_CM_LOGIN -> this.CM_LOGIN(ch, processed)
            | Commands.CMD_CM_BGNPLAY -> this.CM_BGNPLAY(ch, processed)
            | Commands.CMD_CM_ENDPLAY -> this.CM_ENDPLAY(ch, processed)
            | Commands.CMD_CM_NEWCHA -> this.CM_NEWCHA(ch, processed)
            | Commands.CMD_CM_DELCHA -> this.CM_DELCHA(ch, processed)
            | Commands.CMD_CM_CREATE_PASSWORD2 -> this.CM_CREATE_PASSWORD2(ch, processed)
            | Commands.CMD_CM_UPDATE_PASSWORD2 -> this.CM_UPDATE_PASSWORD2(ch, processed)
            | Commands.CMD_CM_REGISTER -> this.CM_REGISTER(ch, processed)
            | Commands.CMD_CP_CHANGEPASS -> this.CP_CHANGEPASS(ch, processed)

            // ── Прямые обработчики ──
            | Commands.CMD_CP_PING -> this.CP_PING(ch)
            | Commands.CMD_CM_SAY -> this.CM_SAY(ch, processed)
            | Commands.CMD_CM_OFFLINE_MODE -> this.CM_OFFLINE_MODE(ch)

            // ── Диапазонная маршрутизация ──
            | ClientCommandPatterns.CmRange -> this.ForwardCmRange(ch, processed)
            | ClientCommandPatterns.CpRange -> this.ForwardCpRange(ch, cmd, processed)
            | _ -> logger.LogWarning("OnProcessData: неизвестная клиентская команда {Cmd}", cmd)

    // ══════════════════════════════════════════════════════════
    //  SyncCall-обработчики (порт из TransmitCall::Process)
    // ══════════════════════════════════════════════════════════

    /// CMD_CM_LOGIN: Авторизация клиента через GroupServer.
    member private this.CM_LOGIN(player: PlayerChannelIO, packet: IRPacket) =
        // Проверка: уже авторизован?
        if player.IsAuthorized then
            let w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_MAPCRASH)
            w.WriteString("Login Error - You are already logged in.")
            player.SendPacket(w)
        else

            let msg = CommandMessages.Deserialize.cmLoginRequest packet

            logger.LogInformation(
                "CM_LOGIN. Account: {Account}, MAC: {Mac}, PWD: {password} Ch#{ChannelId}",
                msg.AcctName,
                msg.Mac,
                msg.PasswordHash,
                player.Id
            )

            if uint16 msg.ClientVersion <> _version then
                this.SendError(player, Commands.CMD_MC_LOGIN, Commands.ERR_MC_VER_ERROR)
                player.Close()
            else

                let bCheat = uint16 msg.CheatMarker <> 911us
                let (ChannelId_ rawId) = player.Id

                let tpMsg : CommandMessages.TpUserLoginRequest =
                    { AcctName = msg.AcctName; AcctPassword = msg.PasswordHash; AcctMac = msg.Mac
                      ClientIp = player.ClientIp; GateAddr = int64 rawId
                      CheatMarker = if bCheat then 0L else 911L }
                let w = CommandMessages.Serialize.tpUserLoginRequest tpMsg

                this.SyncCallGroup(
                    w,
                    fun response ->
                        match response with
                        | None ->
                            this.SendError(player, Commands.CMD_MC_LOGIN, Commands.ERR_MC_NETEXCP)
                            player.Close()

                        | Some rpkt ->
                            match CommandMessages.Deserialize.tpUserLoginResponse rpkt with
                            | CommandMessages.TpUserLoginError errCode ->
                                this.SendError(player, Commands.CMD_MC_LOGIN, errCode)
                                player.Close()

                            | CommandMessages.TpUserLoginSuccess data ->
                                // Установить состояние Authorized
                                let auth =
                                    { ActId = uint32 data.AcctId
                                      LoginId = uint32 data.AcctLoginId
                                      Password = ""
                                      GroupServerPlayerId = data.GpAddr
                                      Channel = player }

                                player.State <- Authorized_ auth

                                // Отправить CMD_MC_LOGIN клиенту
                                let resp = CommandMessages.Serialize.mcLoginResponse (CommandMessages.McLoginSuccess
                                    { MaxChaNum = data.MaxChaNum; Characters = data.Characters; HasPassword2 = data.HasPassword2 })

                                logger.LogInformation(
                                    "CM_LOGIN: игрок {ActId} авторизован, Ch#{ChannelId}",
                                    data.AcctId,
                                    player.Id
                                )

                                player.SendPacket(resp)
                )

    /// CMD_CM_LOGOUT: Выход клиента (также вызывается при OnDisconnect).
    member private this.CM_LOGOUT(player: PlayerChannelIO) =
        match player.State with
        | PConnected_ -> ()
        | Authorized_ auth ->
            // Отправить CMD_TP_DISC на GroupServer
            let disc = WPacket(64)
            disc.WriteCmd(Commands.CMD_TP_DISC)
            disc.WriteInt64(int64 auth.ActId)
            disc.WriteInt64(int64 player.ClientIp)
            disc.WriteString("normal logout")
            this.SendToGroup(disc)

            // SyncCall CMD_TP_USER_LOGOUT
            let logout = WPacket(32)
            logout.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            let (ChannelId_ rawId) = player.Id
            logout.WriteInt64(int64 rawId)
            logout.WriteInt64(0L) // gp_addr = 0 для Authorized
            this.SyncCallGroup(logout, fun _ -> ())

        | Playing_ playing ->
            // Отправить CMD_TP_DISC на GroupServer
            let disc = WPacket(64)
            disc.WriteCmd(Commands.CMD_TP_DISC)
            disc.WriteInt64(int64 playing.Auth.ActId)
            disc.WriteInt64(int64 player.ClientIp)
            disc.WriteString("normal logout")
            this.SendToGroup(disc)

            // Если в игре — отправить CMD_TM_GOOUTMAP на GameServer
            if not (isNull playing.GameServer) then
                let goout = WPacket(32)
                goout.WriteCmd(Commands.CMD_TM_GOOUTMAP)
                goout.WriteInt64(0L)
                let (ChannelId_ rawId) = player.Id
                goout.WriteInt64(int64 rawId)

                match playing.GameServerPlayerId with
                | Some(GameServerPlayerId_ gmAddr) -> goout.WriteInt64(int64 gmAddr)
                | None -> goout.WriteInt64(0L)

                playing.GameServer.PlayerCount <- playing.GameServer.PlayerCount - 1
                playing.GameServer.SendPacket(goout)
                playing.GameServer <- null
                playing.GameServerPlayerId <- None

            // SyncCall CMD_TP_USER_LOGOUT
            let logout = WPacket(32)
            logout.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            let (ChannelId_ rawId2) = player.Id
            logout.WriteInt64(int64 rawId2)
            logout.WriteInt64(int64 playing.GroupServerPlayerId)
            this.SyncCallGroup(logout, fun _ -> ())

    /// CMD_CM_BGNPLAY: Начать игру (выбор персонажа → вход на карту).
    member private this.CM_BGNPLAY(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _auth ->
            // Десериализация клиентского запроса и формирование запроса к GroupServer
            let chaIndex = packet.ReadInt64()
            let (ChannelId_ rawId) = player.Id
            let w = CommandMessages.Serialize.tpBgnPlayRequest
                        { ChaIndex = chaIndex; GateAddr = int64 rawId; GpAddr = player.GpAddr }

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_BGNPLAY, Commands.ERR_MC_NETEXCP)

                    | Some rpkt ->
                        let resp = CommandMessages.Deserialize.tpBgnPlayResponse rpkt

                        if resp.ErrCode <> 0s then
                            this.SendError(player, Commands.CMD_MC_BGNPLAY, resp.ErrCode)

                            if resp.ErrCode = Commands.ERR_PT_KICKUSER then
                                player.Close()
                        else
                            let data = resp.Data.Value
                            // Успех: данные персонажа
                            let password2 = data.Password2
                            let dbid = uint32 data.ChaId
                            let worldid = uint32 data.WorldId
                            let mapName = data.MapName
                            let garnerWinner = int16 data.Swiner

                            // Найти GameServer для карты
                            match _gameSystem.FindByMap(MapName_ mapName) with
                            | None ->
                                this.SendError(
                                    player,
                                    Commands.CMD_MC_BGNPLAY,
                                    Commands.ERR_MC_NOTARRIVE
                                )

                            | Some game ->
                                if game.PlayerCount > 15000 then
                                    this.SendError(
                                        player,
                                        Commands.CMD_MC_BGNPLAY,
                                        Commands.ERR_MC_TOOMANYPLY
                                    )
                                else
                                    // Переход в Playing
                                    let playing =
                                        { Auth =
                                            { _auth with Password = password2; Channel = player }
                                          Channel = player
                                          DatabaseId = dbid
                                          WorldId = worldid
                                          GameServer = null
                                          GameServerPlayerId = None
                                          GroupServerPlayerId = player.GpAddr
                                          MapName = mapName
                                          GarnerWinner = garnerWinner
                                          ExitStatus = NoExit }

                                    player.State <- Playing_ playing

                                    // Отправить CMD_TM_ENTERMAP на GameServer
                                    let (ChannelId_ gateAddr) = player.Id
                                    let enter = CommandMessages.Serialize.tmEnterMapMessage
                                                    { ActId = playing.Auth.ActId
                                                      Password = password2
                                                      DatabaseId = dbid
                                                      WorldId = worldid
                                                      MapName = mapName
                                                      MapCopyNo = -1
                                                      X = 0u; Y = 0u
                                                      IsSwitch = false
                                                      GateAddr = int64 gateAddr
                                                      GarnerWinner = garnerWinner }
                                    game.SendPacket(enter)

                                    logger.LogInformation(
                                        "CM_BGNPLAY: игрок {DbId} входит на карту [{Map}], Ch#{ChannelId}",
                                        dbid,
                                        mapName,
                                        player.Id
                                    )
            )
        | _ -> this.SendError(player, Commands.CMD_MC_BGNPLAY, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_ENDPLAY: Выйти из игры обратно к выбору персонажа.
    member private this.CM_ENDPLAY(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Playing_ playing when playing.GameServerPlayerId.IsSome ->
            // Отправить CMD_TM_GOOUTMAP на GameServer
            if not (isNull playing.GameServer) then
                let goout = WPacket(packet)
                goout.WriteCmd(Commands.CMD_TM_GOOUTMAP)
                goout.WriteInt64(0L)
                let (ChannelId_ rawId) = player.Id
                goout.WriteInt64(int64 rawId)

                match playing.GameServerPlayerId with
                | Some(GameServerPlayerId_ gmAddr) -> goout.WriteInt64(int64 gmAddr)
                | None -> goout.WriteInt64(0L)

                playing.GameServer.PlayerCount <- playing.GameServer.PlayerCount - 1
                playing.GameServer.SendPacket(goout)
                playing.GameServer <- null
                playing.GameServerPlayerId <- None

            // Переход в CharacterSelection (Authorized)
            player.State <- Authorized_ playing.Auth

            // SyncCall CMD_TP_ENDPLAY к GroupServer
            let endplay = WPacket(packet)
            endplay.WriteCmd(Commands.CMD_TP_ENDPLAY)
            let (ChannelId_ rawId2) = player.Id
            endplay.WriteInt64(int64 rawId2)
            endplay.WriteInt64(int64 playing.GroupServerPlayerId)

            this.SyncCallGroup(
                endplay,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_ENDPLAY, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let groupResp = CommandMessages.Deserialize.tpEndPlayResponse rpkt
                        let w = CommandMessages.Serialize.mcEndPlayResponse
                                    { ErrCode = groupResp.ErrCode
                                      Data = groupResp.Data |> ValueOption.map (fun d ->
                                        { CommandMessages.McEndPlayResponseData.MaxChaNum = d.MaxChaNum
                                          Characters = d.Characters }) }
                        player.SendPacket(w)

                        if groupResp.ErrCode = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ ->
            this.SendError(player, Commands.CMD_MC_ENDPLAY, Commands.ERR_MC_NOTPLAY)
            player.Close()

    /// CMD_CM_NEWCHA: Создание нового персонажа.
    member private this.CM_NEWCHA(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _ ->
            let w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TP_NEWCHA)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)
            w.WriteInt64(int64 player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_NEWCHA, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let groupResp = CommandMessages.Deserialize.tpNewChaResponse rpkt
                        let w = CommandMessages.Serialize.mcNewChaResponse { ErrCode = groupResp.ErrCode }
                        player.SendPacket(w)

                        if groupResp.ErrCode = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ -> this.SendError(player, Commands.CMD_MC_NEWCHA, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_DELCHA: Удаление персонажа.
    member private this.CM_DELCHA(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _ ->
            let w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TP_DELCHA)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)
            w.WriteInt64(int64 player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_DELCHA, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let groupResp = CommandMessages.Deserialize.tpDelChaResponse rpkt
                        let w = CommandMessages.Serialize.mcDelChaResponse { ErrCode = groupResp.ErrCode }
                        player.SendPacket(w)

                        if groupResp.ErrCode = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ -> this.SendError(player, Commands.CMD_MC_DELCHA, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_CREATE_PASSWORD2: Создание второго пароля.
    member private this.CM_CREATE_PASSWORD2(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _ ->
            let w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TP_CREATE_PASSWORD2)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)
            w.WriteInt64(int64 player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(
                            player,
                            Commands.CMD_MC_CREATE_PASSWORD2,
                            Commands.ERR_MC_NETEXCP
                        )

                        player.Close()

                    | Some rpkt ->
                        let errCode = int16 (rpkt.ReadInt64())
                        let w = CommandMessages.Serialize.mcCreatePassword2Response { ErrCode = errCode }
                        player.SendPacket(w)

                        if errCode = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ -> this.SendError(player, Commands.CMD_MC_CREATE_PASSWORD2, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_UPDATE_PASSWORD2: Обновление второго пароля.
    member private this.CM_UPDATE_PASSWORD2(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _ ->
            let w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TP_UPDATE_PASSWORD2)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)
            w.WriteInt64(int64 player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(
                            player,
                            Commands.CMD_MC_UPDATE_PASSWORD2,
                            Commands.ERR_MC_NETEXCP
                        )

                        player.Close()

                    | Some rpkt ->
                        let errCode = int16 (rpkt.ReadInt64())
                        let w = CommandMessages.Serialize.mcUpdatePassword2Response { ErrCode = errCode }
                        player.SendPacket(w)

                        if errCode = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ -> this.SendError(player, Commands.CMD_MC_UPDATE_PASSWORD2, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_REGISTER: Регистрация аккаунта.
    member private this.CM_REGISTER(player: PlayerChannelIO, packet: IRPacket) =
        let w = WPacket(packet)
        w.WriteCmd(Commands.CMD_TP_REGISTER)
        let (ChannelId_ rawId) = player.Id
        w.WriteInt64(int64 rawId)
        w.WriteInt64(int64 player.GpAddr)

        this.SyncCallGroup(
            w,
            fun response ->
                match response with
                | Some rpkt ->
                    let resp = WPacket(rpkt)
                    resp.WriteCmd(Commands.CMD_PC_REGISTER)
                    player.SendPacket(resp)
                | None -> ()

                // Всегда дисконнект после регистрации (как в C++)
                player.Close()
        )

    /// CMD_CP_CHANGEPASS: Смена пароля.
    member private this.CP_CHANGEPASS(player: PlayerChannelIO, packet: IRPacket) =
        let w = WPacket(packet)
        w.WriteCmd(Commands.CMD_TP_CHANGEPASS)
        let (ChannelId_ rawId) = player.Id
        w.WriteInt64(int64 rawId)
        w.WriteInt64(int64 player.GpAddr)

        this.SyncCallGroup(
            w,
            fun response ->
                match response with
                | Some rpkt -> player.ForwardPacket(rpkt)
                | None -> ()
        )

    // ══════════════════════════════════════════════════════════
    //  Прямые обработчики
    // ══════════════════════════════════════════════════════════

    /// CMD_CP_PING: Пинг — переслать на GroupServer с задержкой.
    member private this.CP_PING(player: PlayerChannelIO) =
        match player.State with
        | Playing_ playing when playing.GameServerPlayerId.IsSome ->
            let w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_PING)
            w.WriteInt64(0L) // elapsed (в C++ GetTickCount - m_pingtime)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)
            w.WriteInt64(int64 playing.GroupServerPlayerId)
            this.SendToGroup(w)
        | _ -> ()

    /// CMD_CM_SAY: Чат — проверка estop, фильтр #21, пересылка на GameServer.
    member private this.CM_SAY(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Playing_ playing ->
            let str = packet.ReadString()

            if isNull str then
                ()
            // Фильтр эксплойта #21
            elif str.Contains("#21") then
                ()
            // Проверка estop (мут)
            elif player.Estop then
                let w = WPacket(64)
                w.WriteCmd(Commands.CMD_MC_SYSINFO)
                w.WriteString("You are muted and cannot chat.")
                player.SendPacket(w)

                // Отправить проверку estop на GroupServer
                let check = WPacket(16)
                check.WriteCmd(Commands.CMD_TP_ESTOPUSER_CHECK)
                check.WriteInt64(int64 playing.Auth.ActId)
                this.SendToGroup(check)
            else
                // Переслать на GameServer
                this.ForwardToGame(player, playing, packet)
        | _ -> ()

    /// CMD_CM_OFFLINE_MODE: Офлайн-режим (стойла).
    member private this.CM_OFFLINE_MODE(player: PlayerChannelIO) =
        match player.State with
        | Playing_ playing when not (isNull playing.GameServer) && playing.GameServerPlayerId.IsSome ->
            // SyncCall к GameServer не реализован — отправляем fire-and-forget
            let w = WPacket(32)
            w.WriteCmd(Commands.CMD_TM_OFFLINE_MODE)
            let (ChannelId_ rawId) = player.Id
            w.WriteInt64(int64 rawId)

            match playing.GameServerPlayerId with
            | Some(GameServerPlayerId_ gmAddr) -> w.WriteInt64(int64 gmAddr)
            | None -> w.WriteInt64(0L)

            playing.GameServer.SendPacket(w)
        // NOTE: В C++ есть SyncCall к GameServer с обработкой ответа.
        // Здесь fire-and-forget; GameServer обработает и дисконнектит через MT_PALYEREXIT.
        | _ -> ()

    // ══════════════════════════════════════════════════════════
    //  Диапазонная маршрутизация
    // ══════════════════════════════════════════════════════════

    /// CM range (0..499): пересылка на GameServer.
    member private this.ForwardCmRange(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Playing_ playing -> this.ForwardToGame(player, playing, packet)
        | _ -> ()

    /// CP range (6000..6499): пересылка на GroupServer.
    member private this.ForwardCpRange(player: PlayerChannelIO, cmd: uint16, packet: IRPacket) =
        match player.State with
        | Playing_ playing ->
            // Проверка Chaos-карты для чат-команд
            if _chaosActive then
                match cmd with
                | Commands.CMD_CP_SAY2TRADE
                | Commands.CMD_CP_SAY2ALL
                | Commands.CMD_CP_SAY2YOU
                | Commands.CMD_CP_SAY2GUD
                | Commands.CMD_CP_SESS_SAY ->
                    if playing.MapName = _chaosMap then
                        let w = WPacket(64)
                        w.WriteCmd(Commands.CMD_MC_SYSINFO)
                        w.WriteString("Unable to chat in this map!")
                        player.SendPacket(w)
                        () // return — не пересылать
                    else
                        this.ForwardToGroup(player, playing, packet)
                | _ -> this.ForwardToGroup(player, playing, packet)
            else
                this.ForwardToGroup(player, playing, packet)
        | _ -> ()

    // ══════════════════════════════════════════════════════════
    //  Синхронизация списка игроков (CMD_TP_SYNC_PLYLST)
    // ══════════════════════════════════════════════════════════

    /// Периодическая синхронизация списка игроков с GroupServer.
    /// Аналог ConnectGroupServer::Process в C++ (ToGroupServer.cpp).
    /// Отправляет список всех подключённых игроков, получает их gpAddr.
    member private this.SyncPlayerList() =
        if isNull _groupSystem || not _groupSystem.IsConnected then
            ()
        else

            // Только игроки без установленного GroupServerPlayerId
            let players =
                _players.Values
                |> Seq.filter (fun p -> p.IsAuthorized && p.GpAddr = 0L)
                |> Seq.toArray

            if players.Length = 0 then
                ()
            else

                let playerCount = players.Length

                let w = WPacket(64 + playerCount * 12)
                w.WriteCmd(Commands.CMD_TP_SYNC_PLYLST)
                w.WriteInt64(int64 playerCount)
                w.WriteString(gateCfg.Name)

                for player in players do
                    let (ChannelId_ rawId) = player.Id
                    w.WriteInt64(int64 rawId)
                    w.WriteInt64(int64 player.LoginId)
                    w.WriteInt64(int64 player.ActId)

                this.SyncCallGroup(
                    w,
                    fun response ->
                        match response with
                        | None ->
                            logger.LogWarning(
                                "SyncPlayerList: таймаут или нет подключения к GroupServer"
                            )

                        | Some rpkt ->
                            let err = int16 (rpkt.ReadInt64())

                            if err = Commands.ERR_PT_LOGFAIL then
                                logger.LogError(
                                    "SyncPlayerList: GroupServer отклонил синхронизацию (ERR_PT_LOGFAIL)"
                                )
                            else
                                let num = int (rpkt.ReadInt64())

                                if num <> playerCount then
                                    logger.LogWarning(
                                        "SyncPlayerList: количество не совпадает: отправлено {Sent}, получено {Received}",
                                        playerCount,
                                        num
                                    )
                                else
                                    for i in 0 .. num - 1 do
                                        if int16 (rpkt.ReadInt64()) = 1s then
                                            let gpAddr = rpkt.ReadInt64()

                                            match players[i].State with
                                            | Authorized_ auth ->
                                                players[i].State <-
                                                    Authorized_
                                                        { auth with GroupServerPlayerId = gpAddr }
                                            | Playing_ playing ->
                                                playing.GroupServerPlayerId <- gpAddr
                                            | _ -> ()
                )

    // ══════════════════════════════════════════════════════════
    //  Запуск системы
    // ══════════════════════════════════════════════════════════

    /// Запустить TCP-листенер (event-driven, не блокирует).
    member _.Start(ct: CancellationToken) =
        system.Start(ct)

        // Таймер синхронизации списка игроков (каждую секунду)
        _syncTimer <- new Timer(TimerCallback(fun _ -> this.SyncPlayerList()), null, 1000, 1000)

        logger.LogInformation("ClientSystem: слушает на {Address}:{Port}", cfg.Address, cfg.Port)

    // ══════════════════════════════════════════════════════════
    //  Информация
    // ══════════════════════════════════════════════════════════

    /// Количество подключённых клиентов.
    member _.ConnectionCount = _players.Count

    /// Ожидаемая версия клиента.
    member _.ExpectedVersion = _version

    interface IClientSystem with
        member _.ConnectionCount = _players.Count
        member _.ExpectedVersion = _version

        member _.GetPlayerById(PlayerId_ id) =
            match _players.TryGetValue(ChannelId_ id) with
            | true, player -> Some player
            | _ -> None

        member _.GetPlayingPlayerById(PlayerId_ id) =
            match _players.TryGetValue(ChannelId_ id) with
            | true, player ->
                match player.State with
                | Playing_ playing -> Some playing
                | _ -> None
            | _ -> None

        member _.TryGetByChannelId(channelId) =
            match _players.TryGetValue(channelId) with
            | true, player -> Some player
            | _ -> None

        member _.GetAllPlayers() = _players.Values |> Seq.toArray

        member _.Disconnect(player) = player.Close()

        member _.ResetAllGpAddrs() =
            let mutable count = 0

            for player in _players.Values do
                match player.State with
                | Authorized_ auth when auth.GroupServerPlayerId <> 0L ->
                    player.State <- Authorized_ { auth with GroupServerPlayerId = 0L }
                    count <- count + 1
                | Playing_ playing when playing.GroupServerPlayerId <> 0L ->
                    playing.GroupServerPlayerId <- 0L
                    count <- count + 1
                | _ -> ()

            if count > 0 then
                logger.LogInformation("ResetAllGpAddrs: сброшено {Count} игроков", count)

    interface IDisposable with
        member _.Dispose() =
            if not (isNull _syncTimer) then
                _syncTimer.Dispose()

            (system :> IDisposable).Dispose()

            if not (isNull _rsa) then
                (_rsa :> IDisposable).Dispose()
