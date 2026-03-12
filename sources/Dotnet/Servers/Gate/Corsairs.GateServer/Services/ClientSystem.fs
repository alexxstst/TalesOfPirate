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
type ClientSystem(config: IOptions<ClientConfig>, loggerFactory: ILoggerFactory) as this =

    let cfg = config.Value
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

    // ── Chaos-карта ──
    let _chaosActive = cfg.ChaosActive
    let _chaosMap = cfg.ChaosMap

    do
        system.OnConnected.Add(fun ch -> this.OnConnect(ch))

        system.OnCommand.Add(fun (ch, packet) ->
            try
                let decrypted = ch.DecryptIncoming(packet)

                try
                    logger.LogDebug("Client ← {Packet} Ch={Ch}", decrypted, ch.Id)
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

    /// Отправить ошибку клиенту: WPacket с Cmd + Int16(errCode).
    member private _.SendError(player: PlayerChannelIO, cmd: uint16, err: int16) =
        let w = WPacket(16)
        w.WriteCmd(cmd)
        w.WriteInt16(err)
        player.SendPacket(w)

    /// Переслать пакет на GameServer с trailer (gateAddr, gmAddr).
    member private _.ForwardToGame
        (player: PlayerChannelIO, playing: PlayerPlaying, packet: IRPacket)
        =
        if not (isNull playing.GameServer) then
            let w = WPacket(packet)
            let (ChannelId_ rawId) = player.Id
            w.WriteUInt32(rawId)

            match playing.GameServerPlayerId with
            | Some(GameServerPlayerId_ gmAddr) -> w.WriteUInt32(gmAddr)
            | None -> w.WriteUInt32(0u)

            playing.GameServer.SendPacket(w)

    /// Переслать пакет на GroupServer с trailer (gateAddr, gpAddr).
    member private this.ForwardToGroup
        (player: PlayerChannelIO, playing: PlayerPlaying, packet: IRPacket)
        =
        let w = WPacket(packet)
        let (ChannelId_ rawId) = player.Id
        w.WriteUInt32(rawId)
        w.WriteUInt32(playing.GroupServerPlayerId)
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
        if packet.PayloadLength < 4 then
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

            let account = packet.ReadString()
            let password = packet.ReadString()
            let mac = packet.ReadString()

            logger.LogInformation(
                "CM_LOGIN. Account: {Account}, MAC: {Mac}, PWD: {password} Ch#{ChannelId}",
                account,
                mac,
                password,
                player.Id
            )

            // Читаем version и cheat-маркер из tail пакета
            let version = packet.ReverseReadUInt16()

            if version <> _version then
                this.SendError(player, Commands.CMD_MC_LOGIN, Commands.ERR_MC_VER_ERROR)
                player.Close()
            else

                let cheatMarker = packet.ReverseReadUInt16()
                let bCheat = cheatMarker <> 911us

                if not bCheat then
                    packet.DiscardLast(2) |> ignore // DiscardLast cheat marker

                packet.DiscardLast(2) |> ignore // DiscardLast version

                // Формируем пакет CMD_TP_USER_LOGIN для GroupServer
                let w = WPacket(packet)
                w.WriteCmd(Commands.CMD_TP_USER_LOGIN)
                w.WriteUInt32(player.ClientIp)
                let (ChannelId_ rawId) = player.Id
                w.WriteUInt32(rawId)
                w.WriteUInt16(if bCheat then 0us else 911us)

                this.SyncCallGroup(
                    w,
                    fun response ->
                        match response with
                        | None ->
                            this.SendError(player, Commands.CMD_MC_LOGIN, Commands.ERR_MC_NETEXCP)
                            player.Close()

                        | Some rpkt ->
                            let errno = rpkt.ReadInt16()

                            if errno <> 0s then
                                // Ошибка — переслать ответ GroupServer клиенту
                                let errW = WPacket(rpkt)
                                errW.WriteCmd(Commands.CMD_MC_LOGIN)
                                errW.WriteInt16(errno)
                                player.SendPacket(errW)
                                player.Close()
                            else
                                // Успех: считать данные из tail ответа
                                let gpAddr = rpkt.ReverseReadUInt32()
                                let loginId = rpkt.ReverseReadUInt32()
                                let actId = rpkt.ReverseReadUInt32()
                                let byPassword = rpkt.ReverseReadUInt8()
                                rpkt.DiscardLast(4 + 4 + 4 + 1) |> ignore

                                // Установить состояние Authorized
                                let auth =
                                    { ActId = actId
                                      LoginId = loginId
                                      Password = ""
                                      GroupServerPlayerId = gpAddr
                                      Channel = player }

                                player.State <- Authorized_ auth

                                // Отправить CMD_MC_LOGIN клиенту (тело ответа + byPassword)
                                let resp = WPacket(rpkt)
                                resp.WriteCmd(Commands.CMD_MC_LOGIN)
                                resp.WriteUInt8(byPassword)

                                logger.LogInformation(
                                    "CM_LOGIN: игрок {ActId} авторизован, Ch#{ChannelId}, Cmd: {Cmd}",
                                    actId,
                                    player.Id,
                                    resp
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
            disc.WriteUInt32(auth.ActId)
            disc.WriteUInt32(player.ClientIp)
            disc.WriteString("normal logout")
            this.SendToGroup(disc)

            // SyncCall CMD_TP_USER_LOGOUT
            let logout = WPacket(32)
            logout.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            let (ChannelId_ rawId) = player.Id
            logout.WriteUInt32(rawId)
            logout.WriteUInt32(0u) // gp_addr = 0 для Authorized
            this.SyncCallGroup(logout, fun _ -> ())

        | Playing_ playing ->
            // Отправить CMD_TP_DISC на GroupServer
            let disc = WPacket(64)
            disc.WriteCmd(Commands.CMD_TP_DISC)
            disc.WriteUInt32(playing.Auth.ActId)
            disc.WriteUInt32(player.ClientIp)
            disc.WriteString("normal logout")
            this.SendToGroup(disc)

            // Если в игре — отправить CMD_TM_GOOUTMAP на GameServer
            if not (isNull playing.GameServer) then
                let goout = WPacket(32)
                goout.WriteCmd(Commands.CMD_TM_GOOUTMAP)
                goout.WriteUInt8(0uy)
                let (ChannelId_ rawId) = player.Id
                goout.WriteUInt32(rawId)

                match playing.GameServerPlayerId with
                | Some(GameServerPlayerId_ gmAddr) -> goout.WriteUInt32(gmAddr)
                | None -> goout.WriteUInt32(0u)

                playing.GameServer.PlayerCount <- playing.GameServer.PlayerCount - 1
                playing.GameServer.SendPacket(goout)
                playing.GameServer <- null
                playing.GameServerPlayerId <- None

            // SyncCall CMD_TP_USER_LOGOUT
            let logout = WPacket(32)
            logout.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            let (ChannelId_ rawId2) = player.Id
            logout.WriteUInt32(rawId2)
            logout.WriteUInt32(playing.GroupServerPlayerId)
            this.SyncCallGroup(logout, fun _ -> ())

    /// CMD_CM_BGNPLAY: Начать игру (выбор персонажа → вход на карту).
    member private this.CM_BGNPLAY(player: PlayerChannelIO, packet: IRPacket) =
        match player.State with
        | Authorized_ _auth ->
            // Формируем пакет для GroupServer
            let w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TP_BGNPLAY)
            let (ChannelId_ rawId) = player.Id
            w.WriteUInt32(rawId)
            w.WriteUInt32(player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_BGNPLAY, Commands.ERR_MC_NETEXCP)

                    | Some rpkt ->
                        let errno = rpkt.ReadInt16()

                        if errno <> 0s then
                            // Ошибка — переслать ответ клиенту
                            let errW = WPacket(rpkt)
                            errW.WriteCmd(Commands.CMD_MC_BGNPLAY)
                            player.SendPacket(errW)

                            if errno = Commands.ERR_PT_KICKUSER then
                                player.Close()
                        else
                            // Успех: считать данные персонажа
                            let password2 = rpkt.ReadString()
                            let dbid = rpkt.ReadUInt32()
                            let worldid = rpkt.ReadUInt32()
                            let mapName = rpkt.ReadString()
                            let garnerWinner = rpkt.ReadInt16()

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
                                    let enter = WPacket(256)
                                    enter.WriteCmd(Commands.CMD_TM_ENTERMAP)
                                    enter.WriteUInt32(playing.Auth.ActId)
                                    enter.WriteString(password2)
                                    enter.WriteUInt32(dbid)
                                    enter.WriteUInt32(worldid)
                                    enter.WriteString(mapName)
                                    enter.WriteInt32(-1) // mapCopyNo
                                    enter.WriteUInt32(0u) // x
                                    enter.WriteUInt32(0u) // y
                                    enter.WriteUInt8(0uy) // isSwitch = false
                                    let (ChannelId_ gateAddr) = player.Id
                                    enter.WriteUInt32(gateAddr)
                                    enter.WriteInt16(garnerWinner)
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
                goout.WriteUInt8(0uy)
                let (ChannelId_ rawId) = player.Id
                goout.WriteUInt32(rawId)

                match playing.GameServerPlayerId with
                | Some(GameServerPlayerId_ gmAddr) -> goout.WriteUInt32(gmAddr)
                | None -> goout.WriteUInt32(0u)

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
            endplay.WriteUInt32(rawId2)
            endplay.WriteUInt32(playing.GroupServerPlayerId)

            this.SyncCallGroup(
                endplay,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_ENDPLAY, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let resp = WPacket(rpkt)
                        resp.WriteCmd(Commands.CMD_MC_ENDPLAY)
                        player.SendPacket(resp)

                        if rpkt.ReadInt16() = Commands.ERR_PT_KICKUSER then
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
            w.WriteUInt32(rawId)
            w.WriteUInt32(player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_NEWCHA, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let resp = WPacket(rpkt)
                        resp.WriteCmd(Commands.CMD_MC_NEWCHA)
                        player.SendPacket(resp)

                        if rpkt.ReadInt16() = Commands.ERR_PT_KICKUSER then
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
            w.WriteUInt32(rawId)
            w.WriteUInt32(player.GpAddr)

            this.SyncCallGroup(
                w,
                fun response ->
                    match response with
                    | None ->
                        this.SendError(player, Commands.CMD_MC_DELCHA, Commands.ERR_MC_NETEXCP)
                        player.Close()

                    | Some rpkt ->
                        let resp = WPacket(rpkt)
                        resp.WriteCmd(Commands.CMD_MC_DELCHA)
                        player.SendPacket(resp)

                        if rpkt.ReadInt16() = Commands.ERR_PT_KICKUSER then
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
            w.WriteUInt32(rawId)
            w.WriteUInt32(player.GpAddr)

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
                        let resp = WPacket(rpkt)
                        resp.WriteCmd(Commands.CMD_MC_CREATE_PASSWORD2)
                        player.SendPacket(resp)

                        if rpkt.ReadInt16() = Commands.ERR_PT_KICKUSER then
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
            w.WriteUInt32(rawId)
            w.WriteUInt32(player.GpAddr)

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
                        let resp = WPacket(rpkt)
                        resp.WriteCmd(Commands.CMD_MC_UPDATE_PASSWORD2)
                        player.SendPacket(resp)

                        if rpkt.ReadInt16() = Commands.ERR_PT_KICKUSER then
                            player.Close()
            )
        | _ -> this.SendError(player, Commands.CMD_MC_UPDATE_PASSWORD2, Commands.ERR_MC_NOTSELCHA)

    /// CMD_CM_REGISTER: Регистрация аккаунта.
    member private this.CM_REGISTER(player: PlayerChannelIO, packet: IRPacket) =
        let w = WPacket(packet)
        w.WriteCmd(Commands.CMD_TP_REGISTER)
        let (ChannelId_ rawId) = player.Id
        w.WriteUInt32(rawId)
        w.WriteUInt32(player.GpAddr)

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
        w.WriteUInt32(rawId)
        w.WriteUInt32(player.GpAddr)

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
            w.WriteUInt32(0u) // elapsed (в C++ GetTickCount - m_pingtime)
            let (ChannelId_ rawId) = player.Id
            w.WriteUInt32(rawId)
            w.WriteUInt32(playing.GroupServerPlayerId)
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
                check.WriteUInt32(playing.Auth.ActId)
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
            w.WriteUInt32(rawId)

            match playing.GameServerPlayerId with
            | Some(GameServerPlayerId_ gmAddr) -> w.WriteUInt32(gmAddr)
            | None -> w.WriteUInt32(0u)

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
    //  Запуск системы
    // ══════════════════════════════════════════════════════════

    /// Запустить TCP-листенер (event-driven, не блокирует).
    member _.Start(ct: CancellationToken) =
        system.Start(ct)
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

    interface IDisposable with
        member _.Dispose() =
            (system :> IDisposable).Dispose()

            if not (isNull _rsa) then
                (_rsa :> IDisposable).Dispose()
