namespace Corsairs.GroupServer.Services

open System
open System.Collections.Concurrent
open System.Net
open System.Threading
open System.Threading.Tasks
open Corsairs.GroupServer.Config
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options
open Corsairs.GroupServer.Services.Handlers

/// Система подключения к AccountServer (TCP-клиент с SyncCall).
/// Аналог GroupServerSystem из F# GateServer.
type AccountServerSystem
    (
        config: IOptions<AccountConnectionConfig>,
        groupConfig: IOptions<GroupServerConfig>,
        loggerFactory: ILoggerFactory
    ) as this =

    let cfg = config.Value
    let grpCfg = groupConfig.Value
    let logger = loggerFactory.CreateLogger<AccountServerSystem>()
    let endpoint = IPEndPoint(IPAddress.Parse(cfg.Address), cfg.Port)

    let system =
        DirectSystemCommand<AccountServerIO>(
            { Endpoints = [||] },
            (fun (socket, handler) -> AccountServerIO(socket, handler)),
            loggerFactory
        )

    let mutable _channel: AccountServerIO = null
    let mutable _ct = CancellationToken.None
    let mutable _gateSystem: GateServerSystem option = None

    // ── SyncCall: SESS-корреляция ──

    static let SESS_FLAG = 0x80000000u
    let mutable _nextSessId = 1
    let _pendingCalls = ConcurrentDictionary<uint32, IRPacket option -> unit>()

    let nextSessId () =
        let id = uint32 (Interlocked.Increment(&_nextSessId))
        if id > SESS_FLAG - 2u then
            Interlocked.Exchange(&_nextSessId, 1) |> ignore
            1u
        else
            id

    let processSess (packet: IRPacket) : bool =
        let sess = packet.Sess
        if sess > SESS_FLAG then
            let originalSess = sess - SESS_FLAG
            match _pendingCalls.TryRemove(originalSess) with
            | true, callback -> callback (Some packet)
            | _ -> logger.LogWarning("SyncCall: неизвестный SESS {Sess}", originalSess)
            true
        else
            false

    do
        system.OnConnected.Add(fun ch ->
            _channel <- ch
            logger.LogInformation("AccountServerSystem: подключён к {Ep}", ch.RemoteEndPoint)
            this.Login(ch))

        system.OnCommand.Add(fun (ch, packet) ->
            try
                let sess = packet.Sess
                if sess > SESS_FLAG then
                    processSess packet |> ignore
                else
                    // Async-команды от AccountServer (CMD_AP_*)
                    this.OnProcessData(packet)
            with ex ->
                logger.LogError(ex, "AccountServerSystem: ошибка обработки"))

        system.OnDisconnected.Add(fun _ch ->
            _channel <- null
            // Вызвать все ожидающие callback'и с None
            for kvp in _pendingCalls do
                match _pendingCalls.TryRemove(kvp.Key) with
                | true, callback -> callback None
                | _ -> ()
            logger.LogWarning("AccountServerSystem: отключён от AccountServer")
            Task.Run(Func<Task>(fun () -> task {
                try
                    do! Task.Delay(5000, _ct)
                    let! _ = system.ConnectAsync(endpoint, _ct)
                    ()
                with :? OperationCanceledException -> ()
            })) |> ignore)

    /// Регистрация на AccountServer (CMD_PA_LOGIN).
    member private this.Login(channel: AccountServerIO) =
        let mutable w = WPacket(64)
        w.WriteCmd(Commands.CMD_PA_LOGIN)
        w.WriteString(grpCfg.ServerName)
        w.WriteString(cfg.Password)

        this.SyncCall(w, fun response ->
            match response with
            | Some rpkt ->
                let err = int16 (rpkt.ReadInt64())
                if err <> 0s then
                    logger.LogError("Login: AccountServer отклонил, err={Err}", err)
                    channel.Close()
                else
                    logger.LogInformation("Login: успешно зарегистрирован на AccountServer")
            | None ->
                logger.LogError("Login: таймаут AccountServer")
                channel.Close())

    /// Обработка async-команд от AccountServer (AP_*).
    member private _.OnProcessData(packet: IRPacket) =
        let cmd = packet.GetCmd()
        logger.LogDebug("AccountServer async: cmd={Cmd}", cmd)
        match _gateSystem with
        | None -> ()
        | Some gate ->
            let ctx = gate.GetCtx()
            match cmd with
            | Commands.CMD_AP_KICKUSER ->
                Handlers.AdminHandlers.handleApKickuser ctx packet
            | Commands.CMD_AP_EXPSCALE ->
                Handlers.AdminHandlers.handleApExpscale ctx packet
            | _ ->
                logger.LogDebug("AccountServer: неизвестная async-команда {Cmd}", cmd)

    /// Связать с GateServerSystem.
    member _.SetSystems(gate: GateServerSystem) =
        _gateSystem <- Some gate

    /// SyncCall к AccountServer.
    member _.SyncCall(packet: WPacket, callback: IRPacket option -> unit) =
        let ch = _channel
        if isNull ch then
            callback None
        else
            let sessId = nextSessId ()
            packet.WriteSess(sessId)
            _pendingCalls[sessId] <- callback
            ch.SendPacket(packet)

    /// Fire-and-forget отправка.
    member _.Send(packet: WPacket) =
        let ch = _channel
        if not (isNull ch) then
            ch.SendPacket(packet)

    /// Активный канал.
    member _.Channel = _channel

    /// Подключён ли.
    member _.IsConnected = not (isNull _channel)

    /// Запуск (blocking event loop с реконнектом).
    member _.RunAsync(ct: CancellationToken) =
        _ct <- ct
        task {
            logger.LogInformation("AccountServerSystem: подключение к {Ep}...", endpoint)
            try
                let! _ = system.ConnectAsync(endpoint, ct)
                // Бесконечное ожидание до отмены (reconnect в OnDisconnected)
                let tcs = TaskCompletionSource()
                use _ = ct.Register(fun () -> tcs.TrySetResult() |> ignore)
                do! tcs.Task
            with
            | :? OperationCanceledException -> ()
            | ex ->
                logger.LogError(ex, "AccountServerSystem: ошибка подключения")
                // Retry
                do! Task.Delay(5000, ct)
                do! this.RunAsync(ct)
        }

    interface IAccountServerSystem with
        member _.Channel = _channel
        member _.IsConnected = not (isNull _channel)
        member this.SyncCall(packet, callback) = this.SyncCall(packet, callback)
        member this.Send(packet) = this.Send(packet)
