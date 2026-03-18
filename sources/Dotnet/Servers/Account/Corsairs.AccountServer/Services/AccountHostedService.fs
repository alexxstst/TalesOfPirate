namespace Corsairs.AccountServer.Services

open System
open System.Net
open System.Threading
open System.Threading.Channels
open System.Threading.Tasks
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options
open Corsairs.AccountServer.Config
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages

/// Основной BackgroundService AccountServer.
/// Слушает подключения от GroupServer и обрабатывает команды через паттерн-матчинг.
/// Wire-совместим с C++ GroupServer: SyncCall RPC (SESS-корреляция) для PA_LOGIN и PA_USER_LOGIN.
type AccountHostedService(
    config: IOptions<AccountServerConfig>,
    auth: AuthService,
    registry: GroupServerRegistry,
    logger: ILogger<AccountHostedService>,
    loggerFactory: ILoggerFactory) as this =
    inherit BackgroundService()

    let cfg = config.Value

    let system =
        new ChannelSystemCommand<ChannelIO>(
            { Endpoints = [| IPEndPoint(IPAddress.Parse(cfg.ListenAddress), cfg.ListenPort) |]
              ChannelCapacity = 10000 },
            (fun (socket, handler) -> ChannelIO(socket, handler)),
            loggerFactory)

    /// Флаг ответа в SESS (C++: SESSFLAG = 0x80000000).
    /// Запрос: SESS = 1..0x7FFFFFFE, Ответ: SESS = запрос | SESS_FLAG.
    static let SESS_FLAG = 0x80000000u

    // ── Диспатч команд ──

    /// C++: AccountServer2::OnServeCall() + OnProcessData()
    member private _.OnProcessData(ch: ChannelIO, packet: IRPacket) =
        match packet.GetCmd() with
        // RPC (требуют ответа через SESS-корреляцию)
        | Commands.CMD_PA_LOGIN       -> this.PA_LOGIN(ch, packet)
        | Commands.CMD_PA_USER_LOGIN  -> this.PA_USER_LOGIN(ch, packet)
        // Async (fire-and-forget)
        | Commands.CMD_PA_USER_LOGOUT  -> this.PA_USER_LOGOUT(packet)
        | Commands.CMD_PA_CHANGEPASS   -> this.PA_CHANGEPASS(packet)
        | Commands.CMD_PA_USER_DISABLE -> this.PA_USER_DISABLE(packet)
        | Commands.CMD_PA_REGISTER     -> this.PA_REGISTER(packet)
        | Commands.CMD_PA_GMBANACCOUNT   -> this.PA_BAN(packet, true)
        | Commands.CMD_PA_GMUNBANACCOUNT -> this.PA_BAN(packet, false)
        | cmd ->
            logger.LogDebug("Неизвестная команда {Cmd} от {Channel}", cmd, ch)

    // ── CMD_PA_LOGIN: Регистрация GroupServer ──
    // C++: AccountServer2::Gs_Login() — SyncCall RPC.
    // Читает имя + пароль, проверяет в конфиге, регистрирует в реестре.
    // Ответ: WriteInt64(errcode). SESS = originalSess | SESS_FLAG.
    member private _.PA_LOGIN(ch: ChannelIO, packet: IRPacket) =
        let sess = packet.Sess
        let msg = Deserialize.paLoginRequest packet
        let serverName = msg.ServerName
        let serverPassword = msg.ServerPassword

        let mutable w = WPacket(64)
        w.WriteCmd(0us) // C++: RPC-ответ, cmd не используется
        w.WriteSess(sess ||| SESS_FLAG)

        if String.IsNullOrWhiteSpace(serverName) then
            logger.LogWarning("PA_LOGIN: пустое имя сервера, отклонено")
            w.WriteInt64(int64 Commands.ERR_AP_GPSAUTHFAIL)
        elif registry.ExistsByName(serverName) then
            logger.LogWarning("PA_LOGIN: GroupServer '{Name}' уже зарегистрирован", serverName)
            w.WriteInt64(int64 Commands.ERR_AP_GPSLOGGED)
        else
            let knownPassword =
                match cfg.GroupServers with
                | null -> ValueNone
                | gs ->
                    match gs.TryGetValue(serverName) with
                    | true, pwd -> ValueSome pwd
                    | _ -> ValueNone

            match knownPassword with
            | ValueNone ->
                logger.LogWarning("PA_LOGIN: сервер '{Name}' не найден в конфигурации", serverName)
                w.WriteInt64(int64 Commands.ERR_AP_GPSAUTHFAIL)
            | ValueSome pwd when pwd <> serverPassword ->
                logger.LogWarning("PA_LOGIN: неверный пароль для '{Name}'", serverName)
                w.WriteInt64(int64 Commands.ERR_AP_GPSAUTHFAIL)
            | _ ->
                registry.AddServer(serverName, ch)
                w.WriteInt64(int64 Commands.ERR_SUCCESS)
                logger.LogInformation("PA_LOGIN: GroupServer '{Name}' зарегистрирован", serverName)

        ch.SendPacket(w)

    // ── CMD_PA_USER_LOGIN: Логин аккаунта ──
    // C++: AuthThread::AccountLogin() — SyncCall RPC.
    // Чтение: string(name), string(password), string(mac), long(ip).
    // Ответ: PaUserLoginResponse (RPC) или ApKickUserMessage (same-group kick).
    member private _.PA_USER_LOGIN(ch: ChannelIO, packet: IRPacket) =
        let sess = packet.Sess
        let msg = Deserialize.paUserLoginRequest packet
        let username = msg.Username
        let password = msg.Password
        let clientIp = int32 msg.ClientIp // C++: ReadLong — in_addr (4 bytes)

        // Конвертируем IP из in_addr (network byte order) в строку
        let ipStr =
            try
                let bytes = BitConverter.GetBytes(clientIp)
                IPAddress(bytes).ToString()
            with _ -> ""

        logger.LogDebug("PA_USER_LOGIN: {Username} от {Ip}", username, ipStr)

        // Определяем имя GroupServer по каналу
        let groupName =
            match registry.FindByChannel(ch.Id) with
            | ValueSome entry -> entry.Name
            | ValueNone -> "unknown"

        /// Отправить RPC-ответ PaUserLoginResponse.
        let sendResponse (resp: PaUserLoginResponse) =
            let mutable w = Serialize.paUserLoginResponse resp
            w.WriteSess(sess ||| SESS_FLAG)
            ch.SendPacket(w)

        task {
            let! result = auth.ValidateLogin(username, password)

            match result with
            | AuthSuccess(accountId) ->
                let! _ = auth.SetOnline(accountId, groupName, ipStr)
                do! auth.CreateLoginLog(accountId, username, ipStr)
                registry.AccountLogin(accountId, groupName)

                let sessId = Random.Shared.Next()

                sendResponse (PaUserLoginSuccess
                    { AcctLoginId = int64 accountId; SessId = int64 sessId
                      AcctId = int64 accountId; GmLevel = 0L })
                logger.LogInformation("Login OK: {Username} (ID:{Id})", username, accountId)

            | AuthAlreadyOnline(accountId, _loginStatus) ->
                let! _ = auth.SetSaving(accountId)
                registry.AccountSetSaving(accountId)

                let isSameGroup =
                    match registry.TryGetSession(accountId) with
                    | ValueSome session -> String.Equals(session.GroupServerName, groupName, StringComparison.OrdinalIgnoreCase)
                    | ValueNone -> false

                if isSameGroup then
                    // Same group: CMD_AP_KICKUSER в RPC-ответе
                    let mutable w = Serialize.apKickUserMessage { ErrCode = int64 Commands.ERR_AP_ONLINE; AccountId = int64 accountId }
                    w.WriteSess(sess ||| SESS_FLAG)
                    ch.SendPacket(w)
                    logger.LogInformation("Login SameGroupKick: {Username} (ID:{Id})", username, accountId)
                else
                    // Diff group: async kick на старый GroupServer
                    match registry.TryGetSession(accountId) with
                    | ValueSome session ->
                        match registry.FindByName(session.GroupServerName) with
                        | ValueSome oldGs ->
                            let mutable kickPkt = Serialize.apKickUserMessage { ErrCode = int64 Commands.ERR_AP_LOGINTWICE; AccountId = int64 accountId }
                            oldGs.Channel.SendPacket(kickPkt)
                            logger.LogInformation("Kick отправлен на {Group} для acc:{Id}", session.GroupServerName, accountId)
                        | ValueNone -> ()
                    | ValueNone -> ()

                    // RPC-ответ текущему GroupServer: ERR_AP_ONLINE
                    sendResponse (PaUserLoginError Commands.ERR_AP_ONLINE)
                    logger.LogInformation("Login AlreadyOnline (diff group): {Username}", username)

            | AuthSaving savingAccountId ->
                if registry.IsSavingExpired(savingAccountId, cfg.SavingTimeSeconds) then
                    let! _ = auth.SetOnline(savingAccountId, groupName, ipStr)
                    do! auth.CreateLoginLog(savingAccountId, username, ipStr)
                    registry.AccountLogin(savingAccountId, groupName)

                    let sessId = Random.Shared.Next()

                    sendResponse (PaUserLoginSuccess
                        { AcctLoginId = int64 savingAccountId; SessId = int64 sessId
                          AcctId = int64 savingAccountId; GmLevel = 0L })
                    logger.LogInformation("Login OK (saving expired): {Username} (ID:{Id})", username, savingAccountId)
                else
                    sendResponse (PaUserLoginError Commands.ERR_AP_SAVING)
                    logger.LogInformation("Login Saving: {Username}", username)

            | AuthBanned _reason ->
                sendResponse (PaUserLoginError Commands.ERR_AP_BANUSER)
                logger.LogInformation("Login Banned: {Username}", username)

            | AuthInvalidCredentials ->
                sendResponse (PaUserLoginError Commands.ERR_AP_INVALIDPWD)
                logger.LogInformation("Login BadPass: {Username}", username)

            | AuthAccountNotFound ->
                sendResponse (PaUserLoginError Commands.ERR_AP_INVALIDUSER)
                logger.LogInformation("Login NotFound: {Username}", username)

            | AuthDisabled ->
                sendResponse (PaUserLoginError Commands.ERR_AP_DISABLELOGIN)
                logger.LogInformation("Login Disabled: {Username}", username)

            | AuthError _msg ->
                sendResponse (PaUserLoginError Commands.ERR_AP_UNKNOWN)
                logger.LogError("Login Error: {Username}: {Msg}", username, _msg)
        } |> ignore

    // ── CMD_PA_USER_LOGOUT: Логаут аккаунта ──
    // C++: GroupServer пишет WriteInt64(acctLoginID) + WriteInt64(sessid).
    // C++ AccountServer читает только первый WriteInt64 (nID).
    member private _.PA_USER_LOGOUT(packet: IRPacket) =
        let msg = Deserialize.paUserLogoutMessage packet
        let accountId = int32 msg.AcctLoginId
        registry.AccountLogout(accountId)
        auth.LogoutWithPlaytime(accountId) |> ignore
        logger.LogDebug("PA_USER_LOGOUT: ID:{Id}", accountId)

    // ── CMD_PA_CHANGEPASS: Смена пароля ──
    // C++: DataBaseCtrl::UpdatePassword() — fire-and-forget.
    // Чтение: string(name), string(newpass).
    member private _.PA_CHANGEPASS(packet: IRPacket) =
        let msg = Deserialize.paChangePassMessage packet
        let username = msg.Username
        let newPassword = msg.NewPassword
        auth.ChangePassword(username, newPassword) |> ignore
        logger.LogDebug("PA_CHANGEPASS: {Username}", username)

    // ── CMD_PA_USER_DISABLE: Блокировка аккаунта ──
    // C++: GroupServer пишет WriteInt64(acctLoginID) + WriteInt64(lTimes).
    // C++ AccountServer НЕ обрабатывает эту команду. F# обрабатывает для расширяемости.
    member private _.PA_USER_DISABLE(packet: IRPacket) =
        let msg = Deserialize.paUserDisableMessage packet
        let accountId = int32 msg.AcctLoginId
        auth.DisableAccount(accountId, true) |> ignore
        logger.LogDebug("PA_USER_DISABLE: ID:{Id}", accountId)

    // ── CMD_PA_REGISTER: Регистрация нового аккаунта ──
    // C++: DataBaseCtrl::InsertUser() — fire-and-forget, БЕЗ ответа.
    // Чтение: string(name), string(pass), string(email).
    member private _.PA_REGISTER(packet: IRPacket) =
        let msg = Deserialize.paRegisterMessage packet
        let username = msg.Username
        let password = msg.Password
        auth.Register(username, password) |> ignore
        logger.LogDebug("PA_REGISTER: {Username}", username)

    // ── CMD_PA_GMBANACCOUNT / CMD_PA_GMUNBANACCOUNT ──
    // C++: DataBaseCtrl::OperAccountBan() — fire-and-forget.
    // Чтение: string(actName) — по имени аккаунта, НЕ по ID.
    member private _.PA_BAN(packet: IRPacket, ban: bool) =
        let actName = (Deserialize.paGmBanMessage packet).ActName
        auth.DisableAccountByName(actName, ban) |> ignore
        logger.LogDebug("PA_BAN: Name:{Name} ban={Ban}", actName, ban)

    // ── Главный цикл ──

    override _.ExecuteAsync(ct: CancellationToken) =
        task {
            // C++: при старте AccountServer сбрасывает все ONLINE/SAVING статусы
            let! _ = auth.ResetAllOnline()

            system.Start(ct)
            logger.LogInformation("AccountServer слушает на {Address}:{Port}", cfg.ListenAddress, cfg.ListenPort)

            try
                while not ct.IsCancellationRequested do
                    let! event = system.ReadEventAsync()

                    match event with
                    | CommandReceived(ch, packet) ->
                        try
                            this.OnProcessData(ch, packet)
                        finally
                            packet.Dispose()
                    | Connected ch ->
                        logger.LogInformation("GroupServer подключён: {Endpoint}", ch.RemoteEndPoint)
                    | Disconnected ch ->
                        // Очистка: если GroupServer отключился, удаляем его из реестра
                        registry.RemoveByChannel(ch.Id)
                        logger.LogInformation("GroupServer отключён: Ch#{Id}", ch.Id)
                    | PingReceived _ -> ()
            with
            | :? OperationCanceledException -> ()
            | :? ChannelClosedException -> ()
            | ex -> logger.LogError(ex, "Ошибка в главном цикле AccountServer")

            logger.LogInformation("AccountServer остановлен")
        } :> Task

    /// Количество активных подключений.
    member _.ActiveConnections = system.ActiveCount

    override _.Dispose() =
        (system :> IDisposable).Dispose()
        base.Dispose()
