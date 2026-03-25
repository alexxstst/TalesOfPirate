namespace rec Corsairs.GateServer.Services

open System
open System.Collections.Concurrent
open System.Net
open System.Threading
open Corsairs.GateServer.Domain
open Corsairs.GateServer.Config
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options

/// Система подключений от GameServer (аналог ToGameServer в C++).
/// Владеет ChannelSystemCommand для входящих подключений.
/// Управляет реестром GameServer и маршрутизацией по картам.
type GameServerSystem
    (
        gateConfig: IOptions<GateConfig>,
        config: IOptions<GameServerConfig>,
        loggerFactory: ILoggerFactory
    ) as this =

    let gateName = gateConfig.Value.Name
    let cfg = config.Value
    let logger = loggerFactory.CreateLogger<GameServerSystem>()

    let system =
        new DirectSystemCommand<GameServerIO>(
            { Endpoints = [| IPEndPoint(IPAddress.Parse(cfg.Address), cfg.Port) |] },
            (fun (socket, handler) -> GameServerIO(socket, handler)),
            loggerFactory
        )

    // ── Реестр GameServer ──
    let _gameServers = ConcurrentDictionary<ChannelId, GameServerIO>()
    let _mapToGame = ConcurrentDictionary<MapName, GameServerIO>()
    let mutable _clientSystem: IClientSystem = null
    let mutable _groupSystem: IGroupServerSystem = null

    // ── Таймер пинга ──
    let _pingTimer =
        if cfg.PingInterval > 0 then
            new Timer(
                (fun _ ->
                    for game in _gameServers.Values do
                        game.SendPing()

                    logger.LogTrace("Ping → {Count} GameServer(s)", _gameServers.Count)),
                null,
                Timeout.Infinite,
                Timeout.Infinite
            )
        else
            null

    do
        system.OnConnected.Add this.OnConnect
        system.OnDisconnected.Add this.OnDisconnect

        system.OnCommand.Add(fun (ch, packet) ->
            try
                logger.LogDebug("IN << GameServer {Packet} Ch={Ch}", packet, ch.Id)
                this.OnProcessData(ch, packet)
            finally
                packet.Dispose())

    // ── Утилиты ──

    /// Найти GameServer по карте.
    member _.FindByMap map =
        match _mapToGame.TryGetValue(map) with
        | true, game -> Some game
        | _ -> None

    /// Количество зарегистрированных GameServer.
    member _.GameServerCount = _gameServers.Count

    member private _.ExistsByName(name: string) =
        _mapToGame.Values |> Seq.exists (fun g -> g.Name = name)

    member private _.MapExists(map: MapName) = _mapToGame.ContainsKey(map)

    member private _.RegisterGameServer(game: GameServerIO, name: string, maps: Set<MapName>) =
        game.State <- Online_ { Name = name; Maps = maps; PlayerCount = 0; Channel = game }
        _gameServers[game.Id] <- game

        for map in maps do
            _mapToGame[map] <- game

        logger.LogInformation(
            "Зарегистрирован GameServer [{Name}] с {MapCount} картами",
            name,
            maps.Count
        )

    member private _.UnregisterGame(online: GameServerOnline) =
        _gameServers.TryRemove(online.Channel.Id) |> ignore

        for map in online.Maps do
            _mapToGame.TryRemove(map) |> ignore

        logger.LogInformation(
            "Удалён GameServer [{Name}], карт удалено: {MapCount}",
            online.Name,
            online.Maps.Count
        )

    // ── Связывание систем ──

    /// Установить ссылки на соседние системы (вызывается GateHostedService перед запуском).
    member _.SetSystems(client: IClientSystem, group: IGroupServerSystem) =
        _clientSystem <- client
        _groupSystem <- group

    member _.ClientSystem = _clientSystem
    member _.GroupSystem = _groupSystem

    // ── Сетевые события ──

    member private _.OnConnect(ch: GameServerIO) =
        ch.State <- Connected_

        logger.LogInformation(
            "GameServer подключён: Ch#{ChannelId} с {Endpoint}",
            ch.Id,
            ch.RemoteEndPoint
        )

    /// Отправить CMD_MC_MAPCRASH и отключить всех игроков на данном GameServer.
    /// Аналог post_mapcrash_msg + Disconnect в C++ ToGameServer::OnDisconnect.
    member private _.DisconnectPlayersOnGame(game: GameServerIO) =
        if isNull _clientSystem then () else

        let players = _clientSystem.GetAllPlayers()
        let mutable count = 0

        for player in players do
            match player.State with
            | Playing_ playing when playing.GameServer = game ->
                // Отправить сообщение о крэше карты (CMD_MC_MAPCRASH)
                let mutable w = WPacket(64)
                w.WriteCmd(Commands.CMD_MC_MAPCRASH)
                w.WriteString("Map server has crashed, please wait...")
                player.SendPacket(w)

                // Сбросить ссылку на GameServer и отключить клиента
                playing.GameServer <- null
                playing.GameServerPlayerId <- None
                _clientSystem.Disconnect(player)
                count <- count + 1
            | _ -> ()

        if count > 0 then
            logger.LogWarning(
                "Отключено {Count} игроков из-за крэша GameServer",
                count
            )

    member private this.OnDisconnect(ch: GameServerIO) =
        match ch.State with
        | Connected_ ->
            logger.LogWarning("GameServer отключён (незарегистрированный): Ch#{ChannelId}", ch.Id)
        | Online_ online ->
            this.UnregisterGame(online)
            this.DisconnectPlayersOnGame(online.Channel)
            logger.LogWarning("GameServer [{Name}] отключён: Ch#{ChannelId}", online.Name, ch.Id)

    // ══════════════════════════════════════════════════════════
    //  Вспомогательные методы
    // ══════════════════════════════════════════════════════════

    /// Отправить CMD_TM_ENTERMAP на GameServer (аналог GameServer::EnterMap в C++).
    member private _.EnterMap
        (
            game: GameServerIO,
            player: PlayerChannelIO,
            map: string,
            mapCopyNo: int,
            x: uint32,
            y: uint32,
            isSwitch: bool
        ) =
        let (ChannelId_ rawId) = player.Id
        let w = Serialize.tmEnterMapMessage
                    { ActId = player.ActId
                      Password = player.Password
                      DatabaseId = player.DatabaseId
                      WorldId = player.WorldId
                      MapName = map
                      MapCopyNo = mapCopyNo
                      X = x; Y = y
                      IsSwitch = isSwitch
                      GateAddr = uint32 rawId
                      GarnerWinner = player.GarnerWinner }
        game.SendPacket(w)

    /// Отправить пакет на GroupServer (с проверкой подключения).
    member private _.SendToGroup(w: WPacket) =
        let groupCh = _groupSystem.Channel

        if not (isNull groupCh) then
            groupCh.SendPacket(w)
        else
            w.Dispose()
            logger.LogWarning("GroupServer не подключён, пакет потерян")

    member private _.SyncCall(w: WPacket, f: IRPacket option -> unit) =
        let groupCh = _groupSystem.Channel

        if not (isNull groupCh) then
            _groupSystem.SyncCall(groupCh, w, f)
        else
            w.Dispose()
            f None

    // ══════════════════════════════════════════════════════════
    //  Обработчики команд
    // ══════════════════════════════════════════════════════════

    /// CMD_MT_LOGIN: GameServer регистрируется с именем и списком карт.
    member private this.MT_LOGIN(ch: GameServerIO, packet: IRPacket) =
        let gameName = packet.ReadString()
        let mapListStr = packet.ReadString()

        let maps =
            mapListStr.Split(
                ';',
                StringSplitOptions.RemoveEmptyEntries ||| StringSplitOptions.TrimEntries
            )
            |> Array.map MapName_
            |> Set.ofArray

        let mutable w = WPacket(256)
        w.WriteCmd(Commands.CMD_TM_LOGIN_ACK)

        match (gameName, maps) with
        | LoginValidation.EmptyMaps ->
            logger.LogWarning("MT_LOGIN: некорректная строка карт от [{Name}]", gameName)
            w.WriteInt64(int64 Commands.ERR_TM_MAPERR)
        | LoginValidation.DupName this.ExistsByName ->
            logger.LogWarning("MT_LOGIN: дублирующееся имя GameServer: {Name}", gameName)
            w.WriteInt64(int64 Commands.ERR_TM_OVERNAME)
        | LoginValidation.DupMap this.MapExists dupMap ->
            logger.LogWarning("MT_LOGIN: дублирующаяся карта [{Map}]", dupMap)
            w.WriteInt64(int64 Commands.ERR_TM_OVERMAP)
        | _ ->
            this.RegisterGameServer(ch, gameName, maps)
            w.WriteInt64(int64 Commands.ERR_SUCCESS)
            w.WriteString(gateName)

        ch.SendPacket(w)

    /// CMD_MT_SWITCHMAP: Игрок переключается между картами.
    member private this.MT_SWITCHMAP(online: GameServerOnline, packet: IRPacket) =
        let aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())
        let playerDbid = uint32 (packet.ReverseReadInt64())
        match _clientSystem.GetPlayingPlayerById(PlayerId_ playerId) with
        | Some playing when playing.DatabaseId = playerDbid ->
            let returnFlag = byte (packet.ReverseReadInt64())
            // DiscardLast: returnFlag(1) + addr+dbid пары (2*aimnum) + aimnum(1) элементов
            packet.DiscardLast(2 + 2 * int aimnum) |> ignore

            let srcMapStr = packet.ReadString()
            let srcMapCopyNo = int32 (packet.ReadInt64())
            let srcX = uint32 (packet.ReadInt64())
            let srcY = uint32 (packet.ReadInt64())
            let destMapStr = packet.ReadString()
            let destMapCopyNo = int32 (packet.ReadInt64())
            let destX = uint32 (packet.ReadInt64())
            let destY = uint32 (packet.ReadInt64())

            match this.FindByMap(MapName_ destMapStr) with
            | Some targetGame ->
                // Перемещение на новый GameServer
                online.PlayerCount <- online.PlayerCount - 1

                playing.GameServer <- null
                playing.GameServerPlayerId <- None
                this.EnterMap(targetGame, playing.Channel, destMapStr, destMapCopyNo, destX, destY, true)
                targetGame.PlayerCount <- targetGame.PlayerCount + 1

            | None when returnFlag = 0uy ->
                // Карта не найдена — возврат на исходную
                let mutable sysInfo = WPacket(128)
                sysInfo.WriteCmd(Commands.CMD_MC_SYSINFO)
                sysInfo.WriteString("Cannot find the destination map")
                playing.Channel.SendPacket(sysInfo)

                if not (isNull playing.GameServer) then
                    this.EnterMap(playing.GameServer, playing.Channel, srcMapStr, srcMapCopyNo, srcX, srcY, true)
            | None ->
                // Возврат невозможен — дисконнект
                _clientSystem.Disconnect(playing.Channel)
        | _ ->
            logger.LogWarning(
                "MT_SWITCHMAP: игрок {PlayerId} не найден или не в Playing",
                playerId
            )

        logger.LogDebug("MT_SWITCHMAP от [{Name}]", online.Name)

    /// CMD_MC_ENTERMAP: Подтверждение входа игрока на карту от GameServer.
    member private this.MC_ENTERMAP(online: GameServerOnline, packet: IRPacket) =
        let aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())
        let playerDbid = uint32 (packet.ReverseReadInt64())
        match _clientSystem.GetPlayingPlayerById(PlayerId_ playerId) with
        | Some playing when playing.DatabaseId = playerDbid ->
            let retcode = int16 (packet.ReadInt64())

            if retcode <> Commands.ERR_SUCCESS then
                // Ошибка входа на карту → возврат к Authorized
                playing.Channel.State <- Authorized_ playing.Auth
                online.PlayerCount <- online.PlayerCount - 1
                _clientSystem.Disconnect(playing.Channel)

                logger.LogWarning(
                    "MC_ENTERMAP: ошибка {RetCode} для игрока {PlayerId}",
                    retcode,
                    playerId
                )
            else
                // Успех: считать доп. поля из trailer
                let gameServerPlayerId = uint32 (packet.ReverseReadInt64())
                let plyNum = uint32 (packet.ReverseReadInt64())
                let isSwitch = byte (packet.ReverseReadInt64())

                playing.GameServer <- online.Channel
                playing.GameServerPlayerId <- Some(GameServerPlayerId_ gameServerPlayerId)
                online.PlayerCount <- int plyNum

                // DiscardLast: aimnum(1) + addr+dbid(2*aimnum) + gmAddr(1) + plyNum(1) + isSwitch(1) элементов
                packet.DiscardLast(4 + 2 * int aimnum) |> ignore

                // Переслать клиенту (пакет без trailer)
                playing.Channel.ForwardPacket(packet)

                // Уведомить GroupServer (CMD_MP_ENTERMAP)
                let (ChannelId_ rawId) = playing.Channel.Id
                let enterMapPkt = Serialize.mpEnterMapMessage
                                    { IsSwitch = int64 isSwitch
                                      GateAddr = uint32 rawId
                                      GpAddr = playing.GroupServerPlayerId }
                this.SendToGroup(enterMapPkt)
        | _ ->
            logger.LogWarning(
                "MC_ENTERMAP: игрок {PlayerId} не найден или не в Playing",
                playerId
            )

        logger.LogDebug("MC_ENTERMAP от [{Name}]", online.Name)

    /// CMD_MC_STARTEXIT: Пересылка начала выхода клиенту.
    member private this.MC_STARTEXIT(_online: GameServerOnline, packet: IRPacket) =
        let _aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())

        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player ->
            // Пересылаем пакет как есть (trailer не обрезаем — как в C++)
            player.ForwardPacket(packet)
        | None -> ()

        logger.LogDebug("MC_STARTEXIT: playerId={PlayerId}", playerId)

    /// CMD_MC_CANCELEXIT: Пересылка отмены выхода клиенту.
    member private this.MC_CANCELEXIT(_online: GameServerOnline, packet: IRPacket) =
        let _aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())

        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player ->
            match player.State with
            | Playing_ playing when playing.ExitStatus <> NoExit ->
                playing.ExitStatus <- NoExit
                player.ForwardPacket(packet)
            | _ -> ()
        | None -> ()

        logger.LogDebug("MC_CANCELEXIT: playerId={PlayerId}", playerId)

    /// CMD_MT_PALYEREXIT: Обработка выхода игрока(ов).
    member private this.MT_PALYEREXIT(_online: GameServerOnline, packet: IRPacket) =
        let aimnum = int (packet.ReverseReadInt64())

        for _ in 0 .. aimnum - 1 do
            let playerId = uint32 (packet.ReverseReadInt64())
            let playerDbid = uint32 (packet.ReverseReadInt64())

            match _clientSystem.GetPlayingPlayerById(PlayerId_ playerId) with
            | Some playing when playing.DatabaseId = playerDbid ->

                match playing.ExitStatus with
                | ExitToCharSelect ->
                    // Выход к выбору персонажа → SyncCall CMD_TP_ENDPLAY к GroupServer
                    let mutable endplayGrp = WPacket(32)
                    endplayGrp.WriteCmd(Commands.CMD_TP_ENDPLAY)
                    let (ChannelId_ rawId) = playing.Channel.Id
                    endplayGrp.WriteInt64(int64 rawId)
                    endplayGrp.WriteInt64(int64 playing.GroupServerPlayerId)

                    this.SyncCall(endplayGrp, fun response ->
                        match response with
                        | None ->
                            // Timeout/ошибка сети → отправить ошибку клиенту
                            let mutable errPkt = WPacket(16)
                            errPkt.WriteCmd(Commands.CMD_MC_ENDPLAY)
                            errPkt.WriteInt64(int64 Commands.ERR_MC_NETEXCP)
                            playing.Channel.SendPacket(errPkt)

                        | Some rpkt ->
                            // C++: ответ от GroupServer → заменить CMD на CMD_MC_ENDPLAY → клиенту
                            let mutable w = WPacket(rpkt)
                            w.WriteCmd(Commands.CMD_MC_ENDPLAY)
                            playing.Channel.SendPacket(w)

                            // Декремент и переход в Authorized
                            if not (isNull playing.GameServer) then
                                playing.GameServer.PlayerCount <- playing.GameServer.PlayerCount - 1

                            playing.Channel.State <- Authorized_ playing.Auth

                            // GroupServer может запросить кик
                            if int16 (rpkt.ReadInt64()) = Commands.ERR_PT_KICKUSER then
                                _clientSystem.Disconnect(playing.Channel)
                    )

                | ExitToLogout ->
                    // Полный logout → CMD_TP_DISC (async) + SyncCall CMD_TP_USER_LOGOUT
                    let mutable disc = WPacket(64)
                    disc.WriteCmd(Commands.CMD_TP_DISC)
                    disc.WriteInt64(int64 playing.Auth.ActId)
                    disc.WriteInt64(int64 playing.Channel.ClientIp)
                    disc.WriteString("normal logout")
                    this.SendToGroup(disc)

                    let mutable logout = WPacket(32)
                    logout.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
                    let (ChannelId_ rawId) = playing.Channel.Id
                    logout.WriteInt64(int64 rawId)
                    logout.WriteInt64(int64 playing.GroupServerPlayerId)

                    this.SyncCall(logout, fun _response ->
                        // Ответ от GroupServer (не проверяется, disconnect в любом случае)
                        if not (isNull playing.GameServer) then
                            playing.GameServer.PlayerCount <- playing.GameServer.PlayerCount - 1

                        _clientSystem.Disconnect(playing.Channel)
                    )

                | NoExit -> ()
            | _ -> ()

        logger.LogDebug("MT_PALYEREXIT: aimnum={AimNum}", aimnum)

    /// CMD_MT_KICKUSER: GameServer запрашивает кик игрока.
    member private this.MT_KICKUSER(online: GameServerOnline, packet: IRPacket) =
        let _aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())
        let playerDbid = uint32 (packet.ReverseReadInt64())
        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player when player.DatabaseId = playerDbid ->
            _clientSystem.Disconnect(player)

            logger.LogInformation(
                "MT_KICKUSER: кик игрока {PlayerId} от [{Name}]",
                playerId,
                online.Name
            )
        | _ ->
            logger.LogWarning(
                "MT_KICKUSER: игрок {PlayerId} не найден или dbid не совпадает [{Name}]",
                playerId,
                online.Name
            )

    /// CMD_MT_MAPENTRY: Маршрутизация входа на карту к нужному GameServer.
    /// C++: дублирует весь пакет и перезаписывает CMD.
    member private this.MT_MAPENTRY(online: GameServerOnline, packet: IRPacket) =
        let mapStr = packet.ReadString()
        let map = MapName_ mapStr

        match this.FindByMap(map) with
        | Some targetGame ->
            let mutable w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TM_MAPENTRY)
            targetGame.SendPacket(w)
        | None ->
            let mutable w = WPacket(packet)
            w.WriteCmd(Commands.CMD_TM_MAPENTRY_NOMAP)
            online.Channel.SendPacket(w)

        logger.LogDebug(
            "MT_MAPENTRY: карта [{Map}], найден: {Found}",
            mapStr,
            this.FindByMap(map).IsSome
        )

    /// CMD_MP_GM1SAY: Пересылка GM-команды на GroupServer.
    member private this.MP_GM1SAY(_online: GameServerOnline, packet: IRPacket) =
        _groupSystem.Send(packet)

    // ── Маршрутизация по диапазонам ──

    /// MC диапазон: пересылка клиентам по routing info из trailer пакета.
    member private this.ForwardMC(_online: GameServerOnline, packet: IRPacket) =
        if not packet.HasData then
            logger.LogWarning("Commands {Packet} is empty!", packet)
        else

        let aimnum = int (packet.ReverseReadInt64())

        let targets =
            Array.init aimnum (fun _ ->
                let playerId = uint32 (packet.ReverseReadInt64())
                let dbid = uint32 (packet.ReverseReadInt64())

                _clientSystem.GetPlayerById(PlayerId_ playerId)
                |> Option.filter (fun p -> p.DatabaseId = dbid))

        packet.DiscardLast(2 * aimnum + 1) |> ignore

        for player in targets do
            match player with
            | Some p -> p.ForwardPacket(packet)
            | None -> ()

        logger.LogTrace("ForwardMC: CMD {Cmd}, targets={Count}", packet, aimnum)

    /// MP диапазон: пересылка на GroupServer с заменой trailer.
    /// C++: DiscardLast до if/else; при aimnum>0 — отдельный пакет на каждого игрока.
    member private this.ForwardMP(_online: GameServerOnline, packet: IRPacket) =
        let aimnum = int (packet.ReverseReadInt64())

        let players =
            Array.init aimnum (fun _ ->
                let gateAddr = uint32 (packet.ReverseReadInt64())
                let dbid = uint32 (packet.ReverseReadInt64())

                _clientSystem.GetPlayerById(PlayerId_ gateAddr)
                |> Option.filter (fun p -> p.DatabaseId = dbid)
                |> Option.map (fun p -> struct (gateAddr, p.GpAddr)))

        // C++: DiscardLast вызывается ДО проверки aimnum (при aimnum=0 удаляет 1 элемент aimnum)
        packet.DiscardLast(2 * aimnum + 1) |> ignore

        if aimnum = 0 then
            _groupSystem.Send(packet)
        else
            // C++: для каждого совпавшего игрока — отдельный пакет (копия тела + одна пара)
            for player in players do
                match player with
                | Some struct (gateAddr, gpAddr) ->
                    let mutable w = WPacket(packet)
                    w.WriteInt64(int64 gateAddr)
                    w.WriteInt64(int64 gpAddr)
                    this.SendToGroup(w)
                | None -> ()

        logger.LogTrace("ForwardMP: CMD {Cmd}, aimnum={AimNum}", packet.GetCmd(), aimnum)

    /// MM диапазон: рассылка всем зарегистрированным GameServer.
    member private this.BroadcastMM(_online: GameServerOnline, packet: IRPacket) =
        for game in _gameServers.Values do
            game.ForwardPacket(packet)

        logger.LogTrace("BroadcastMM: CMD {Cmd}", packet.GetCmd())

    // ══════════════════════════════════════════════════════════
    //  Диспатч команд (OnProcessData)
    // ══════════════════════════════════════════════════════════

    member private this.OnProcessData(ch: GameServerIO, packet: IRPacket) =
        match ch.State with
        | Connected_ ->
            match packet.GetCmd() with
            | Commands.CMD_MT_LOGIN -> this.MT_LOGIN(ch, packet)
            | cmd ->
                logger.LogWarning(
                    "Команда {Cmd} от незарегистрированного GameServer Ch#{Id}",
                    cmd,
                    ch.Id
                )
        | Online_ online ->
            match packet.GetCmd() with
            | Commands.CMD_MT_LOGIN ->
                logger.LogWarning(
                    "MT_LOGIN: повторная регистрация от [{Name}], игнорируем",
                    online.Name
                )
            | Commands.CMD_MP_GM1SAY -> this.MP_GM1SAY(online, packet)
            | Commands.CMD_MT_SWITCHMAP -> this.MT_SWITCHMAP(online, packet)
            | Commands.CMD_MC_ENTERMAP -> this.MC_ENTERMAP(online, packet)
            | Commands.CMD_MC_STARTEXIT -> this.MC_STARTEXIT(online, packet)
            | Commands.CMD_MC_CANCELEXIT -> this.MC_CANCELEXIT(online, packet)
            | Commands.CMD_MT_PALYEREXIT -> this.MT_PALYEREXIT(online, packet)
            | Commands.CMD_MT_KICKUSER -> this.MT_KICKUSER(online, packet)
            | Commands.CMD_MT_MAPENTRY -> this.MT_MAPENTRY(online, packet)
            | CommandPatterns.McRange -> this.ForwardMC(online, packet)
            | CommandPatterns.MpRange -> this.ForwardMP(online, packet)
            | CommandPatterns.MmRange -> this.BroadcastMM(online, packet)
            | 0us -> () // ACK-ответ от GameServer, игнорируем
            | cmd ->
                logger.LogWarning(
                    "Неизвестная команда от GameServer [{Name}]: {Cmd}",
                    online.Name,
                    cmd
                )

    // ══════════════════════════════════════════════════════════
    //  Запуск системы
    // ══════════════════════════════════════════════════════════

    /// Запустить TCP-листенер и таймер пинга.
    member _.Start(ct: CancellationToken) =
        system.Start(ct)

        if not (isNull _pingTimer) then
            let interval = cfg.PingInterval * 1000
            _pingTimer.Change(interval, interval) |> ignore
            logger.LogInformation("GameServerSystem: пинг каждые {Interval} сек", cfg.PingInterval)

        logger.LogInformation(
            "GameServerSystem: ожидает на {Address}:{Port}",
            cfg.Address,
            cfg.Port
        )

    interface IGameServerSystem with
        member this.Channel =
            if _gameServers.Count > 0 then _gameServers.Values |> Seq.head :> ChannelIO else null

        member _.IsConnected = _gameServers.Count > 0
        member this.FindByMap map = this.FindByMap map

        member _.BroadcastAll(packet) =
            for game in _gameServers.Values do
                game.ForwardPacket(packet)

    interface IDisposable with
        member _.Dispose() =
            if not (isNull _pingTimer) then
                _pingTimer.Dispose()

            (system :> IDisposable).Dispose()

/// Active patterns для классификации команд по диапазонам.
module private CommandPatterns =

    /// Команда в диапазоне GameServer → Client (MC, 500..999).
    let (|McRange|_|) (cmd: uint16) =
        if cmd / 500us = Commands.CMD_MC_BASE / 500us then Some() else None

    /// Команда в диапазоне GameServer → GroupServer (MP, 5500..5999).
    let (|MpRange|_|) (cmd: uint16) =
        if cmd / 500us = Commands.CMD_MP_BASE / 500us then Some() else None

    /// Команда в диапазоне GameServer → GameServer (MM, 4000..4499).
    let (|MmRange|_|) (cmd: uint16) =
        if cmd / 500us = Commands.CMD_MM_BASE / 500us then Some() else None

/// Active patterns для валидации MT_LOGIN.
module private LoginValidation =

    let (|EmptyMaps|_|) (_name: string, maps: Set<MapName>) : unit option =
        if maps.IsEmpty then Some() else None

    let (|DupName|_|)
        (existsByName: string -> bool)
        (name: string, _maps: Set<MapName>)
        : unit option =
        if existsByName name then Some() else None

    let (|DupMap|_|)
        (mapExists: MapName -> bool)
        (_name: string, maps: Set<MapName>)
        : MapName option =
        maps |> Set.toSeq |> Seq.tryFind mapExists
