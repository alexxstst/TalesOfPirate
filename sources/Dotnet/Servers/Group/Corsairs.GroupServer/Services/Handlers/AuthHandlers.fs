module Corsairs.GroupServer.Services.Handlers.AuthHandlers

open System
open System.Linq
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities
open Corsairs.GroupServer.LookDataParser

// ══════════════════════════════════════════════════════════
//  TP_LOGIN — Регистрация GateServer
// ══════════════════════════════════════════════════════════

let handleTpLogin (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (packet: IRPacket) =
    let version = int16 (packet.ReadInt64())
    let gateName = packet.ReadString()

    let mutable w = WPacket(64)
    w.WriteCmd(Commands.CMD_TP_LOGIN)

    if int version <> ctx.Config.ProtocolVersion then
        ctx.Logger.LogWarning("TP_LOGIN: версия {V} не совпадает с {Expected}", version, ctx.Config.ProtocolVersion)
        w.WriteInt64(int64 Commands.ERR_PT_LOGFAIL)
    else
        let gates = ctx.Gates
        let mutable slotIndex = -1
        let mutable duplicate = false
        for i in 0 .. gates.Length - 1 do
            if not (isNull gates[i]) && gates[i].IsRegistered && gates[i].Name = gateName then
                duplicate <- true
            elif slotIndex < 0 && isNull gates[i] then
                slotIndex <- i

        if duplicate then
            ctx.Logger.LogWarning("TP_LOGIN: имя {Name} уже зарегистрировано", gateName)
            w.WriteInt64(int64 Commands.ERR_PT_SAMEGATENAME)
        elif slotIndex < 0 then
            ctx.Logger.LogWarning("TP_LOGIN: нет свободных слотов для {Name}", gateName)
            w.WriteInt64(int64 Commands.ERR_PT_LOGFAIL)
        else
            ch.GateIndex <- slotIndex
            ch.State <- GateOnline_ { Name = gateName; PlayerCount = 0; Channel = ch }
            gates[slotIndex] <- ch
            ctx.Logger.LogInformation("TP_LOGIN: GateServer [{Name}] зарегистрирован в слоте {Slot}", gateName, slotIndex)
            w.WriteInt64(int64 Commands.ERR_SUCCESS)

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_USER_LOGIN — Авторизация игрока
// ══════════════════════════════════════════════════════════

let handleTpUserLogin (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (packet: IRPacket) =
    let registry = ctx.Registry
    let cfg = ctx.Config

    let msg = Deserialize.tpUserLoginRequest packet
    let acctName = msg.AcctName
    let acctPassword = msg.AcctPassword
    let acctMac = msg.AcctMac
    let clientIpRaw = msg.ClientIp
    let gtAddr = msg.GateAddr
    let _bCheat = msg.CheatMarker <> 911L

    if registry.OnlineCount >= cfg.MaxOnlinePlayers then
        let w = Serialize.tpUserLoginResponse (TpUserLoginError Commands.ERR_MC_TOOMANYPLY)
        ctx.SendRpcResponse ch sess w
    else

    let gpAddr = ctx.AllocateGpAddr()
    let player = PlayerRecord()
    player.GpAddr <- gpAddr
    registry.Register(gpAddr, player)

    let ipBytes = BitConverter.GetBytes(clientIpRaw)
    let clientIp = $"{ipBytes[0]}.{ipBytes[1]}.{ipBytes[2]}.{ipBytes[3]}"

    let loginPkt =
        Serialize.paUserLoginRequest
            { Username = acctName; Password = acctPassword; Mac = acctMac; ClientIp = int64 clientIpRaw }

    ctx.AccountSystem.SyncCall(loginPkt, fun response ->
        match response with
        | None ->
            ctx.Logger.LogWarning("TP_USER_LOGIN: таймаут AccountServer для {Name}", acctName)
            registry.Unregister(gpAddr)
            let w = Serialize.tpUserLoginResponse (TpUserLoginError Commands.ERR_PT_NETEXCP)
            ctx.SendRpcResponse ch sess w
        | Some rpkt ->
            match Deserialize.paUserLoginResponse rpkt with
            | PaUserLoginError errCode ->
                registry.Unregister(gpAddr)
                let w = Serialize.tpUserLoginResponse (TpUserLoginError errCode)
                ctx.SendRpcResponse ch sess w
            | PaUserLoginSuccess data ->
                let acctLoginId = uint32 data.AcctLoginId
                let sessId = uint32 data.SessId
                let acctId = uint32 data.AcctId
                let gmLevel = sbyte data.GmLevel

                // Кик дубликата аккаунта
                match registry.TryGetByActId(acctId) with
                | Some existing when existing.GpAddr <> gpAddr ->
                    ctx.Logger.LogWarning("TP_USER_LOGIN: аккаунт {Id} уже онлайн, кик", acctId)
                    ctx.KickUser existing.GpAddr existing.GateAddr
                    registry.Unregister(existing.GpAddr)
                | _ -> ()

                // Загрузка персонажей и аккаунта из БД
                let characters, password2 =
                    try
                        use scope = ctx.ScopeFactory.CreateScope()
                        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                        // Получить/создать запись в GameDB.dbo.account (аналог C++ FetchRowByActName + InsertRow)
                        let gameAcct = db.GetOrCreateGameAccount(int acctId, acctName)
                        let pw2 = if isNull gameAcct.Password2 then "" else gameAcct.Password2
                        let chars = db.GetCharactersByAccount(int acctId, cfg.MaxCharacters)
                        let mapped = chars |> Array.map toCharacterSlot
                        mapped, pw2
                    with ex ->
                        ctx.Logger.LogError(ex, "TP_USER_LOGIN: ошибка загрузки персонажей для {Name}", acctName)
                        Array.empty, ""

                player.State <- Authorized_
                    { ActId = acctId
                      LoginId = acctLoginId
                      SessionId = sessId
                      AccountName = acctName
                      Passport = ""
                      Password = ""
                      GmLevel = gmLevel
                      ClientIp = clientIp
                      Gate = ch
                      GateAddr = gtAddr
                      Password2 = password2
                      Characters = characters
                      CurrentCha = -1
                      CanReceiveRequests = true }

                // Ответ — формируем слоты персонажей для TpUserLoginResponse
                let chaSlots = Array.init cfg.MaxCharacters (fun i ->
                    if i < characters.Length then
                        let c = characters[i]
                        { Valid = true; ChaName = c.ChaName; Job = c.Job
                          Degree = int64 c.Level; TypeId = int64 c.TypeId
                          EquipIds = c.EquipIds }
                    else
                        { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
                          EquipIds = Array.empty })

                let w = Serialize.tpUserLoginResponse
                            (TpUserLoginSuccess
                                { MaxChaNum = int64 cfg.MaxCharacters
                                  Characters = chaSlots
                                  HasPassword2 = password2.Length > 0
                                  AcctId = int64 acctId
                                  AcctLoginId = int64 acctLoginId
                                  GpAddr = gpAddr })
                ctx.SendRpcResponse ch sess w
                ctx.Logger.LogInformation("TP_USER_LOGIN: {Name} авторизован, {Count} персонажей, hasPassword2={Pw2}",
                    acctName, characters.Length, password2.Length > 0))

// ══════════════════════════════════════════════════════════
//  TP_USER_LOGOUT — Выход игрока
// ══════════════════════════════════════════════════════════

let handleTpUserLogout (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) =
    let registry = ctx.Registry
    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_TP_USER_LOGOUT)

    if not player.IsAuthorized then
        w.WriteInt64(int64 Commands.ERR_PT_INERR)
    else
        if player.IsPlaying then
            registry.ClearPlaying(player.GpAddr)

        let acctName = player.AccountName
        let loginId = player.LoginId
        let sessId = player.SessionId

        let mutable billEnd = WPacket(64)
        billEnd.WriteCmd(Commands.CMD_PA_USER_BILLEND)
        billEnd.WriteString(acctName)
        ctx.AccountSystem.Send(billEnd)

        let mutable logout = WPacket(32)
        logout.WriteCmd(Commands.CMD_PA_USER_LOGOUT)
        logout.WriteInt64(int64 loginId)
        logout.WriteInt64(int64 sessId)
        ctx.AccountSystem.Send(logout)

        registry.Unregister(player.GpAddr)
        ctx.Logger.LogInformation("TP_USER_LOGOUT: {Name} вышел", acctName)
        w.WriteInt64(int64 Commands.ERR_SUCCESS)

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_DISC — Disconnect info
// ══════════════════════════════════════════════════════════

let handleTpDisc (ctx: HandlerContext) (_ch: GateServerIO) (packet: IRPacket) =
    let acctId = uint32 (packet.ReadInt64())
    let clientIpRaw = uint32 (packet.ReadInt64())
    let reason = packet.ReadString()

    // Преобразуем IP из uint32 в строку
    let ipBytes = System.BitConverter.GetBytes(clientIpRaw)
    let clientIp = $"{ipBytes[0]}.{ipBytes[1]}.{ipBytes[2]}.{ipBytes[3]}"

    // Сохранить в БД (аналог C++ SetDiscInfo)
    try
        use scope = ctx.ScopeFactory.CreateScope()
        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
        db.SetDisconnectInfo(int acctId, clientIp, reason)
    with ex ->
        ctx.Logger.LogError(ex, "TP_DISC: ошибка записи в БД для {Id}", acctId)

    ctx.Logger.LogDebug("TP_DISC: acctId={Id} ip={Ip} причина={Reason}", acctId, clientIp, reason)

// ══════════════════════════════════════════════════════════
//  TP_REGISTER — Регистрация аккаунта
// ══════════════════════════════════════════════════════════

let private isAlphanumeric (s: string) =
    s.Length > 0 && s |> Seq.forall (fun c -> Char.IsLetterOrDigit(c) || c = '_')

let private isEmail (s: string) =
    s.Length >= 3 && s.Length <= 254 && s.Contains('@') && s.Contains('.')

let handleTpRegister (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (packet: IRPacket) =
    let username = packet.ReadString()
    let password = packet.ReadString()
    let email = packet.ReadString()

    let mutable w = WPacket(256)
    w.WriteCmd(Commands.CMD_PT_REGISTER)

    if username.Length < 5 || username.Length > 16 || not (isAlphanumeric username) then
        w.WriteInt64(0L)
        w.WriteString("Invalid Username.")
        ctx.SendRpcResponse ch sess w
    elif not (isAlphanumeric password) then
        w.WriteInt64(0L)
        w.WriteString("Invalid Password.")
        ctx.SendRpcResponse ch sess w
    elif not (isEmail email) then
        w.WriteInt64(0L)
        w.WriteString("Invalid Email.")
        ctx.SendRpcResponse ch sess w
    else
        let mutable regPkt = WPacket(256)
        regPkt.WriteCmd(Commands.CMD_PA_REGISTER)
        regPkt.WriteString(username)
        regPkt.WriteString(password)
        regPkt.WriteString(email)
        w.Dispose()

        ctx.AccountSystem.SyncCall(regPkt, fun response ->
            let mutable resp = WPacket(128)
            resp.WriteCmd(Commands.CMD_PT_REGISTER)
            match response with
            | Some rpkt ->
                let err = int16 (rpkt.ReadInt64())
                if err = Commands.ERR_SUCCESS then
                    resp.WriteInt64(1L)
                    resp.WriteString("Account created successfully.")
                    ctx.Logger.LogInformation("TP_REGISTER: аккаунт {Name} создан", username)
                else
                    resp.WriteInt64(0L)
                    resp.WriteString("Username is taken.")
            | None ->
                resp.WriteInt64(0L)
                resp.WriteString("Server busy, try again later.")
            ctx.SendRpcResponse ch sess resp)

// ══════════════════════════════════════════════════════════
//  TP_CHANGEPASS — Смена пароля
// ══════════════════════════════════════════════════════════

let handleTpChangepass (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let req = Deserialize.tpChangePassRequest packet
    let newPassword = req.NewPass
    let pin = req.Pin

    let mutable w = WPacket(128)
    w.WriteCmd(Commands.CMD_PC_ERRMSG)

    match player.State with
    | Playing_ playing ->
        let now = Environment.TickCount64
        if now < playing.ChangePassCooldown then
            w.WriteString("Please Calm Down! do not spam!.")
        elif playing.Auth.Password2 <> pin then
            w.WriteString("Incorrect PIN.")
        elif newPassword.Length <> 32 || not (isAlphanumeric newPassword) then
            w.WriteString("Invalid Password.")
        else
            playing.ChangePassCooldown <- now + 3000L
            let mutable passPkt = WPacket(128)
            passPkt.WriteCmd(Commands.CMD_PA_CHANGEPASS)
            passPkt.WriteString(player.AccountName)
            passPkt.WriteString(newPassword)
            ctx.AccountSystem.Send(passPkt)
            w.WriteString("Password has been changed.")
            ctx.Logger.LogInformation("TP_CHANGEPASS: {Name} сменил пароль", player.AccountName)
    | _ ->
        w.WriteString("Invalid Password.")

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_CREATE_PASSWORD2 — Создание 2-го пароля
// ══════════════════════════════════════════════════════════

let handleTpCreatePassword2 (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let req = Deserialize.tpCreatePassword2Request packet
    let newPw2 = req.Password2

    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_TP_CREATE_PASSWORD2)

    match player.State with
    | Authorized_ auth when auth.Password2.Length = 0 ->
        if newPw2.Length = 0 || newPw2.Length > ctx.Config.MaxPassword2Length then
            w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)
        elif not (isAlphanumeric newPw2) then
            w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)
        else
            auth.Password2 <- newPw2
            // Сохранить в БД (аналог C++ UpdatePassword)
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                db.UpdatePassword2(int auth.ActId, newPw2)
            with ex ->
                ctx.Logger.LogError(ex, "TP_CREATE_PASSWORD2: ошибка записи в БД для {Act}", auth.ActId)
            w.WriteInt64(int64 Commands.ERR_SUCCESS)
    | _ ->
        w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_UPDATE_PASSWORD2 — Обновление 2-го пароля
// ══════════════════════════════════════════════════════════

let handleTpUpdatePassword2 (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let req = Deserialize.tpUpdatePassword2Request packet
    let oldPw2 = req.OldPassword2
    let newPw2 = req.NewPassword2

    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_TP_UPDATE_PASSWORD2)

    match player.State with
    | Authorized_ auth | Playing_ { Auth = auth } ->
        if auth.Password2 <> oldPw2 then
            w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)
        elif newPw2.Length = 0 || newPw2.Length > ctx.Config.MaxPassword2Length then
            w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)
        elif not (isAlphanumeric newPw2) then
            w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)
        else
            auth.Password2 <- newPw2
            // Сохранить в БД (аналог C++ UpdatePassword)
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                db.UpdatePassword2(int auth.ActId, newPw2)
            with ex ->
                ctx.Logger.LogError(ex, "TP_UPDATE_PASSWORD2: ошибка записи в БД для {Act}", auth.ActId)
            w.WriteInt64(int64 Commands.ERR_SUCCESS)
    | _ ->
        w.WriteInt64(int64 Commands.ERR_PT_INVALID_PW2)

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_REQPLYLST — Запрос списка игроков
// ══════════════════════════════════════════════════════════

let handleTpReqPlylst (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (_packet: IRPacket) =
    let registry = ctx.Registry
    let mutable w = WPacket(4096)
    w.WriteCmd(Commands.CMD_TP_REQPLYLST)

    let mutable count = 0
    for player in registry.GetAllPlayers() do
        if player.IsPlaying && not (isNull player.Gate) && Object.ReferenceEquals(player.Gate, ch) then
            w.WriteInt64(int64 player.GateAddr)
            w.WriteInt64(int64 player.CurrentChaId)
            count <- count + 1

    w.WriteInt64(int64 count)
    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_SYNC_PLYLST — Синхронизация списка игроков
// ══════════════════════════════════════════════════════════

let handleTpSyncPlylst (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (packet: IRPacket) =
    let registry = ctx.Registry
    let playerCount = int (packet.ReadInt64())
    let gateName = packet.ReadString()

    let mutable w = WPacket(1024)
    w.WriteCmd(Commands.CMD_TP_SYNC_PLYLST)

    let targetGate =
        ctx.Gates |> Array.tryFind (fun g -> not (isNull g) && g.IsRegistered && g.Name = gateName)

    match targetGate with
    | None ->
        w.WriteInt64(int64 Commands.ERR_PT_LOGFAIL)
        w.WriteInt64(0L)
    | Some gate ->
        w.WriteInt64(int64 Commands.ERR_SUCCESS)
        w.WriteInt64(int64 playerCount)

        for _ in 0 .. playerCount - 1 do
            let gtAddr = uint32 (packet.ReadInt64())
            let acctLoginId = uint32 (packet.ReadInt64())
            let acctId = uint32 (packet.ReadInt64())

            // Загрузить Password2 из БД
            let pw2 =
                try
                    use scope = ctx.ScopeFactory.CreateScope()
                    let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                    let acct = db.GameAccounts.Find(int acctId)
                    if isNull (box acct) || isNull acct.Password2 then "" else acct.Password2
                with _ -> ""

            let gpAddr = ctx.AllocateGpAddr()
            let player = PlayerRecord()
            player.GpAddr <- gpAddr
            player.State <- Authorized_
                { ActId = acctId
                  LoginId = acctLoginId
                  SessionId = 0u
                  AccountName = ""
                  Passport = ""
                  Password = ""
                  GmLevel = 0y
                  ClientIp = ""
                  Gate = gate
                  GateAddr = gtAddr
                  Password2 = pw2
                  Characters = Array.empty
                  CurrentCha = -1
                  CanReceiveRequests = true }
            registry.Register(gpAddr, player)
            w.WriteInt64(1L)
            w.WriteInt64(int64 gpAddr)

        ctx.Logger.LogInformation("TP_SYNC_PLYLST: синхронизировано {Count} игроков для {Gate}", playerCount, gateName)

    ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  OS_LOGIN — Авторизация монитора
// ══════════════════════════════════════════════════════════

let handleOsLogin (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (packet: IRPacket) =
    let version = int16 (packet.ReadInt64())
    let _agentName = packet.ReadString()

    let mutable w = WPacket(16)
    w.WriteCmd(Commands.CMD_OS_LOGIN)

    if int version <> ctx.Config.ProtocolVersion then
        w.WriteInt64(int64 Commands.ERR_OS_NOTMATCH_VERSION)
    else
        w.WriteInt64(int64 Commands.ERR_SUCCESS)

    ctx.SendRpcResponse ch sess w
