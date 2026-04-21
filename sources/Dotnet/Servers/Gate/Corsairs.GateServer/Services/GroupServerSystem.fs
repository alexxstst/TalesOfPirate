namespace Corsairs.GateServer.Services

open System
open System.Collections.Concurrent
open System.Net
open System.Threading
open System.Threading.Tasks
open Corsairs.GateServer.Config
open Corsairs.GateServer.Domain
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options

/// Active patterns для классификации команд от GroupServer по диапазонам.
module private GroupCommandPatterns =

    /// Команда в диапазоне GroupServer → Client (PC, 5000..5499).
    let (|PcRange|_|) (cmd: uint16) =
        if cmd / 500us = Commands.CMD_PC_BASE / 500us then Some() else None

    /// Команда в диапазоне GroupServer → GameServer (PM, 4500..4999).
    let (|PmRange|_|) (cmd: uint16) =
        if cmd / 500us = Commands.CMD_PM_BASE / 500us then Some() else None

/// Система подключения к GroupServer (аналог ToGroupServer в C++).
/// Использует DirectSystemCommand с исходящим ConnectAsync и автореконнектом при OnDisconnected.
type GroupServerSystem
    (
        config: IOptions<GroupServerConfig>,
        gateConfig: IOptions<GateConfig>,
        loggerFactory: ILoggerFactory
    ) as this =

    let cfg = config.Value
    let gateCfg = gateConfig.Value
    let logger = loggerFactory.CreateLogger<GroupServerSystem>()
    let endpoint = IPEndPoint(IPAddress.Parse(cfg.Address), cfg.Port)

    let system =
        new DirectSystemCommand<GroupServerIO>(
            { Endpoints = [||] },
            (fun (socket, handler) -> GroupServerIO(socket, handler)),
            loggerFactory)

    let mutable _channel: GroupServerIO = null
    let mutable _ct = CancellationToken.None
    let mutable _clientSystem: IClientSystem = null
    let mutable _gameSystem: IGameServerSystem = null

    // ── SyncCall: SESS-корреляция запрос→ответ ──

    /// Флаг ответа в SESS (C++: SESSFLAG = 0x80000000).
    /// Запрос: SESS = 1..0x7FFFFFFE, Ответ: SESS = запрос + SESS_FLAG.
    static let SESS_FLAG = 0x80000000u

    /// Счётчик SESS ID (1..0x7FFFFFFE), аналог TGenSessID в C++.
    let mutable _nextSessId = 1

    /// Реестр ожидающих callback'ов: SESS ID → callback.
    let _pendingCalls = ConcurrentDictionary<uint32, IRPacket option -> unit>()

    /// Генерировать следующий SESS ID (потокобезопасно).
    let nextSessId () =
        let id = uint32 (Interlocked.Increment(&_nextSessId))
        if id > SESS_FLAG - 2u then
            Interlocked.Exchange(&_nextSessId, 1) |> ignore
            1u
        else
            id

    /// Обработать входящий пакет: если SESS > SESS_FLAG — это ответ на SyncCall.
    /// Возвращает true если пакет обработан как SESS-ответ.
    let processSess (packet: IRPacket) : bool =
        let sess = packet.Sess
        if sess > SESS_FLAG then
            // Ответ: извлечь оригинальный SESS
            let originalSess = sess - SESS_FLAG
            match _pendingCalls.TryRemove(originalSess) with
            | true, callback ->
                callback (Some packet)
            | _ ->
                logger.LogWarning("SyncCall: получен ответ с неизвестным SESS {Sess}", originalSess)
            true
        else
            false

    do
        system.OnConnected.Add(fun ch ->
            _channel <- ch
            logger.LogInformation("GroupServerSystem: подключён к {Endpoint}", ch.RemoteEndPoint)
            this.Login(ch))

        system.OnCommand.Add(fun (_ch, packet) ->
            try
                let sess = packet.Sess
                logger.LogDebug("IN << GroupServer {Packet}", packet)

                if sess > SESS_FLAG then
                    // Ответ на наш исходящий SyncCall
                    processSess packet |> ignore
                elif sess > 0u && sess < SESS_FLAG then
                    // Входящий RPC от GroupServer (OnServeCall)
                    this.OnServeCall(sess, packet)
                    packet.Dispose()
                else
                    // sess = 0 ИЛИ sess = SESS_FLAG → обычная команда
                    this.OnProcessData(packet)
                    packet.Dispose()
            with ex ->
                packet.Dispose()
                logger.LogError(ex, "GroupServerSystem: ошибка обработки команды {Cmd}", packet.GetCmd()))

        system.OnDisconnected.Add(fun _ch ->
            _channel <- null

            // Вызвать все ожидающие callback'и с None (аналог timeout при disconnect)
            for kvp in _pendingCalls do
                match _pendingCalls.TryRemove(kvp.Key) with
                | true, callback -> callback None
                | _ -> ()

            logger.LogWarning("GroupServerSystem: отключён от GroupServer")

            // Сбросить GpAddr у всех игроков, чтобы SyncPlayerList
            // заново зарегистрировал их после переподключения
            if not (isNull _clientSystem) then
                _clientSystem.ResetAllGpAddrs()

            Task.Run(Func<Task>(fun () -> task {
                try
                    do! Task.Delay(5000, _ct)
                    let! _ = system.ConnectAsync(endpoint, _ct)
                    ()
                with :? OperationCanceledException -> ()
            })) |> ignore)

    /// Установить ссылки на соседние системы (вызывается GateHostedService перед запуском).
    member _.SetSystems(client: IClientSystem, game: IGameServerSystem) =
        _clientSystem <- client
        _gameSystem <- game

    /// Система клиентских подключений.
    member _.ClientSystem = _clientSystem

    /// Система подключений от GameServer.
    member _.GameSystem = _gameSystem

    /// Активный канал к GroupServer (null если не подключён).
    member _.Channel = _channel

    /// Подключён ли к GroupServer.
    member _.IsConnected = not (isNull _channel)

    // ══════════════════════════════════════════════════════════
    //  Регистрация на GroupServer (CMD_TP_LOGIN)
    // ══════════════════════════════════════════════════════════

    /// Отправить CMD_TP_LOGIN после установки соединения (аналог ConnectGroupServer::Process в C++).
    /// SyncCall: отправляет версию протокола и имя GateServer, ожидает подтверждение.
    /// При ошибке — дисконнект (автореконнект сработает через OnDisconnected).
    member private this.Login(channel: GroupServerIO) =
        let w = WPacket(64)
        w.WriteCmd(Commands.CMD_TP_LOGIN)
        w.WriteInt64(int64 cfg.ProtocolVersion)
        w.WriteString(gateCfg.Name)

        (this :> IGroupServerSystem).SyncCall(channel, w, fun response ->
            match response with
            | Some rpkt ->
                let err = int16 (rpkt.ReadInt64())

                if err <> 0s then
                    logger.LogError("Login: GroupServer отклонил регистрацию, err={Err}", err)
                    channel.Close()
                else
                    logger.LogInformation("Login: успешно зарегистрирован на GroupServer как '{Name}'", gateCfg.Name)
            | None ->
                logger.LogError("Login: таймаут или потеря соединения при регистрации")
                channel.Close())

    // ══════════════════════════════════════════════════════════
    //  Утилиты
    // ══════════════════════════════════════════════════════════

    /// Отправить RPC-ответ GroupServer'у (SESS = originalSess | SESS_FLAG).
    member private _.SendRpcResponse(sess: uint32, errCode: int16) =
        let ch = _channel

        if not (isNull ch) then
            let mutable w = WPacket(16)
            w.WriteCmd(0us)
            w.WriteSess(sess ||| SESS_FLAG)
            w.WriteInt64(int64 errCode)
            ch.SendPacket(w)

    /// Найти Playing-игрока по playerId и проверить gpAddr.
    member private _.ValidatePlayer(playerId: uint32, gpAddr: int64) =
        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player when player.IsPlaying && player.GpAddr = gpAddr -> Some player
        | _ -> None

    // ══════════════════════════════════════════════════════════
    //  OnServeCall — входящие RPC от GroupServer
    // ══════════════════════════════════════════════════════════

    /// Диспатч входящих RPC (SESS > 0 && < SESS_FLAG).
    member private this.OnServeCall(sess: uint32, packet: IRPacket) =
        match packet.GetCmd() with
        | Commands.CMD_PT_KICKPLAYINGPLAYER -> this.PT_KICKPLAYINGPLAYER(sess, packet)
        // CMD_MM_DO_STRING приходит из TP_USER_LOGIN (C++ GroupServer) с ненулевым SESS,
        // т.к. LIBDBC не обнуляет SESS в новых пакетах внутри RPC-обработчика.
        // Действие то же: broadcast на все GameServer. RPC-ответ не нужен.
        | Commands.CMD_MM_DO_STRING -> _gameSystem.BroadcastAll(packet)
        | cmd ->
            logger.LogWarning("OnServeCall: неизвестная RPC-команда {Cmd} с GroupServer пришло", cmd)

    /// CMD_PT_KICKPLAYINGPLAYER: кик играющего игрока через GameServer.
    /// C++ аналог: ToGroupServer::OnServeCall, case CMD_PT_KICKPLAYINGPLAYER.
    member private this.PT_KICKPLAYINGPLAYER(sess: uint32, packet: IRPacket) =
        let playerId = uint32 (packet.ReadInt64())

        match _clientSystem.GetPlayingPlayerById(PlayerId_ playerId) with
        | Some playing ->
            // Уведомить GameServer (fire-and-forget; C++ использует SyncCall с 10с таймаутом)
            if not (isNull playing.GameServer) then
                let mutable w = WPacket(16)
                w.WriteCmd(Commands.CMD_TM_KICKCHA)
                w.WriteInt64(int64 playing.DatabaseId)
                playing.GameServer.SendPacket(w)

            // Дисконнектить клиента
            _clientSystem.Disconnect(playing.Channel)

            this.SendRpcResponse(sess, Commands.ERR_SUCCESS)
            logger.LogInformation("PT_KICKPLAYINGPLAYER: кик игрока {PlayerId}", playerId)

        | None ->
            this.SendRpcResponse(sess, Commands.ERR_PT_INERR)
            logger.LogWarning("PT_KICKPLAYINGPLAYER: игрок {PlayerId} не найден", playerId)

    // ══════════════════════════════════════════════════════════
    //  OnProcessData — входящие async-сообщения от GroupServer
    // ══════════════════════════════════════════════════════════

    /// Диспатч async-команд (SESS = 0).
    member private this.OnProcessData(packet: IRPacket) =
        let cmd = packet.GetCmd()

        match cmd with
        // Broadcast на все GameServer
        | Commands.CMD_MM_DO_STRING -> _gameSystem.BroadcastAll(packet)
        | Commands.CMD_PM_TEAM -> _gameSystem.BroadcastAll(packet)

        // Кик игрока
        | Commands.CMD_AP_KICKUSER
        | Commands.CMD_PT_KICKUSER -> this.PT_KICKUSER(packet)

        // Estop (блокировка/разблокировка)
        | Commands.CMD_PT_DEL_ESTOPUSER -> this.PT_ESTOP(packet, false)
        | Commands.CMD_PT_ESTOPUSER -> this.PT_ESTOP(packet, true)

        // CMD_MC_SYSINFO → обрабатывается как PC range
        | Commands.CMD_MC_SYSINFO -> this.ForwardPC(cmd, packet)

        // Диапазонная маршрутизация
        | GroupCommandPatterns.PcRange -> this.ForwardPC(cmd, packet)
        | GroupCommandPatterns.PmRange -> this.ForwardPM(packet)
        | _ ->
            logger.LogWarning("OnProcessData: неизвестная команда {Cmd}", cmd)

    // ── Обработчики конкретных команд ──

    /// CMD_AP_KICKUSER / CMD_PT_KICKUSER: кик клиента по gpAddr.
    member private _.PT_KICKUSER(packet: IRPacket) =
        let _aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())
        let gpAddr = packet.ReverseReadInt64()

        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player when player.GpAddr = gpAddr ->
            logger.LogInformation("PT_KICKUSER: кик игрока {PlayerId}", playerId)
            _clientSystem.Disconnect(player)
        | Some _ ->
            logger.LogWarning("PT_KICKUSER: gpAddr не совпадает для {PlayerId}", playerId)
        | None ->
            logger.LogWarning("PT_KICKUSER: игрок {PlayerId} не найден", playerId)

    /// CMD_PT_ESTOPUSER / CMD_PT_DEL_ESTOPUSER: установить/снять Estop.
    member private _.PT_ESTOP(packet: IRPacket, value: bool) =
        let _aimnum = uint16 (packet.ReverseReadInt64())
        let playerId = uint32 (packet.ReverseReadInt64())
        let gpAddr = packet.ReverseReadInt64()

        match _clientSystem.GetPlayerById(PlayerId_ playerId) with
        | Some player when player.GpAddr = gpAddr ->
            player.Estop <- value

            logger.LogInformation(
                "PT_ESTOP: {Action} для игрока {PlayerId}",
                (if value then "блокировка" else "разблокировка"),
                playerId
            )
        | Some _ ->
            logger.LogWarning("PT_ESTOP: gpAddr не совпадает для {PlayerId}", playerId)
        | None -> ()

    // ── PC range: пересылка клиентам ──

    /// Диапазон CMD_PC_BASE (5000..5499) + CMD_MC_SYSINFO: пересылка клиентам по trailer.
    /// Trailer: [body][playerId₁][gpAddr₁]...[playerIdₙ][gpAddrₙ][aimnum(2b)]
    member private _.ForwardPC(cmd: uint16, packet: IRPacket) =
        let aimnum = int (packet.ReverseReadInt64())

        // Считать все пары (playerId, gpAddr) из trailer
        let mutable lastValidPlayer: PlayerChannelIO = null

        let targets =
            Array.init aimnum (fun _ ->
                let playerId = uint32 (packet.ReverseReadInt64())
                let gpAddr = packet.ReverseReadInt64()

                match _clientSystem.GetPlayerById(PlayerId_ playerId) with
                | Some player when player.GpAddr = gpAddr -> Some player
                | _ -> None)

        // Обрезать trailer
        packet.DiscardLast(2 * aimnum + 1) |> ignore

        // Переслать каждому валидному клиенту
        for target in targets do
            match target with
            | Some player ->
                player.ForwardPacket(packet)
                lastValidPlayer <- player
            | None -> ()

        // Спецобработка: CMD_PC_CHANGE_PERSONINFO → дополнительно переслать на GameServer
        if cmd = Commands.CMD_PC_CHANGE_PERSONINFO && not (isNull lastValidPlayer) then
            let game = lastValidPlayer.Game

            if not (isNull game) then
                let mutable w = WPacket(packet)
                w.WriteCmd(Commands.CMD_TM_CHANGE_PERSONINFO)
                let (ChannelId_ rawId) = lastValidPlayer.Id
                w.WriteInt64(int64 rawId)

                match lastValidPlayer.GmAddr with
                | Some(GameServerPlayerId_ gmAddr) -> w.WriteInt64(int64 gmAddr)
                | None -> w.WriteInt64(0L)

                game.SendPacket(w)

        // Спецобработка: CMD_PC_PING → обновить время пинга
        if cmd = Commands.CMD_PC_PING && not (isNull lastValidPlayer) then
            lastValidPlayer.ConnectedAt |> ignore // NOTE: PingTime не реализован, не критично

    // ── PM range: пересылка на GameServer(s) ──

    /// Диапазон CMD_PM_BASE (4500..4999): пересылка на GameServer(s) по trailer.
    /// Trailer: [body][playerId₁][gpAddr₁]...[playerIdₙ][gpAddrₙ][aimnum(2b)]
    member private _.ForwardPM(packet: IRPacket) =
        let aimnum = int (packet.ReverseReadInt64())

        if aimnum = 0 then
            // Broadcast на все GameServer: обрезать trailer (1 элемент aimnum)
            packet.DiscardLast(1) |> ignore

            let mutable w = WPacket(packet)
            w.WriteInt64(0L)

            // Обернуть в RPacket для BroadcastAll
            use rpk = new RPacket(w.GetPacketMemory()) :> IRPacket
            _gameSystem.BroadcastAll(rpk)
            w.Dispose()
        else
            // Считать пары из trailer
            let targets =
                Array.init aimnum (fun _ ->
                    let playerId = uint32 (packet.ReverseReadInt64())
                    let gpAddr = packet.ReverseReadInt64()
                    struct (playerId, gpAddr))

            // Обрезать trailer
            packet.DiscardLast(2 * aimnum + 1) |> ignore

            // Для каждого игрока: переслать на его GameServer
            for struct (playerId, gpAddr) in targets do
                match _clientSystem.GetPlayerById(PlayerId_ playerId) with
                | Some player when player.GpAddr = gpAddr && not (isNull player.Game) ->
                    let mutable w = WPacket(packet)
                    let (ChannelId_ rawId) = player.Id
                    w.WriteInt64(int64 rawId)

                    match player.GmAddr with
                    | Some(GameServerPlayerId_ gmAddr) -> w.WriteInt64(int64 gmAddr)
                    | None -> w.WriteInt64(0L)

                    player.Game.SendPacket(w)
                | _ -> ()

    /// Запустить систему: подключение с реконнектом.
    member _.RunAsync(ct: CancellationToken) : Task =
        task {
            _ct <- ct
            system.Start(ct)
            logger.LogInformation("GroupServerSystem: подключение к {Address}:{Port}", cfg.Address, cfg.Port)

            let! _ = system.ConnectAsync(endpoint, ct)
            ()

            // Ожидание отмены
            try
                do! Task.Delay(Timeout.Infinite, ct)
            with :? OperationCanceledException -> ()

            logger.LogInformation("GroupServerSystem: остановлен")
        }
        :> Task

    interface IGroupServerSystem with
        member _.Channel = _channel
        member _.IsConnected = not (isNull _channel)

        member _.Send(packet) =
            let ch = _channel

            if not (isNull ch) then
                ch.ForwardPacket(packet)
            else
                logger.LogWarning("Send: GroupServer не подключён")

        member _.SyncCall(channel, packet, callback) =
            // Генерируем SESS, записываем в пакет, регистрируем callback
            let sessId = nextSessId ()
            packet.WriteSess(sessId)
            _pendingCalls[sessId] <- callback
            channel.SendPacket(packet)

            // Timeout: 60 секунд
            Task.Delay(60_000).ContinueWith(fun (_: Task) ->
                match _pendingCalls.TryRemove(sessId) with
                | true, cb ->
                    logger.LogWarning("SyncCall SESS={SessId}: отменён по таймауту (60с)", sessId)
                    cb None
                | _ -> ()
            ) |> ignore

    interface IDisposable with
        member _.Dispose() =
            (system :> IDisposable).Dispose()

