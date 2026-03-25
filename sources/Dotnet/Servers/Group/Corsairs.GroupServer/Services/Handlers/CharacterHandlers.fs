module Corsairs.GroupServer.Services.Handlers.CharacterHandlers

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

/// Резолв birthplace → имя карты через конфиг (аналог C++ m_mapBirthplace).
let private resolveMapName (cfg: Corsairs.GroupServer.Config.GroupServerConfig) (birthplace: string) =
    match cfg.Birthplaces with
    | null -> "garner"
    | bp ->
        match bp.TryGetValue(birthplace) with
        | true, map -> map
        | _ -> "garner"

// ══════════════════════════════════════════════════════════
//  TP_BGNPLAY — Выбор персонажа и вход в игру
// ══════════════════════════════════════════════════════════

let handleTpBgnplay (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config
    let registry = ctx.Registry

    let req = Deserialize.tpBgnPlayRequest packet
    let chaIndex = int req.ChaIndex

    let inline sendErr errCode =
        let w = Serialize.tpBgnPlayResponse { ErrCode = errCode; Data = ValueNone }
        ctx.SendRpcResponse ch sess w

    match player.State with
    | Authorized_ auth ->
        if auth.CurrentCha >= 0 then
            sendErr Commands.ERR_PT_INERR
        elif chaIndex < 0 || chaIndex >= cfg.MaxCharacters || chaIndex >= auth.Characters.Length then
            sendErr Commands.ERR_PT_INVALIDCHA
        elif auth.Password2.Length = 0 then
            sendErr Commands.ERR_PT_INVALID_PW2
        else
            let chaSlot = auth.Characters[chaIndex]

            // Кик если персонаж уже играет (аналог C++ CMD_PT_KICKPLAYINGPLAYER + TP_USER_LOGOUT)
            match registry.TryGetByChaId(chaSlot.ChaId) with
            | Some otherPlayer when otherPlayer.GpAddr <> player.GpAddr ->
                ctx.Logger.LogWarning("TP_BGNPLAY: персонаж {Name} уже играет, кик", chaSlot.ChaName)

                // 1. Кик через GateServer
                ctx.KickUser otherPlayer.GpAddr otherPlayer.GateAddr

                // 2. Cleanup: billing end + logout → AccountServer (аналог C++ TP_USER_LOGOUT)
                if otherPlayer.IsPlaying then
                    registry.ClearPlaying(otherPlayer.GpAddr)

                if otherPlayer.IsAuthorized then
                    let otherAcctName = otherPlayer.AccountName
                    let otherLoginId = otherPlayer.LoginId
                    let otherSessId = otherPlayer.SessionId

                    let mutable billEnd = WPacket(64)
                    billEnd.WriteCmd(Commands.CMD_PA_USER_BILLEND)
                    billEnd.WriteString(otherAcctName)
                    ctx.AccountSystem.Send(billEnd)

                    let mutable logout = WPacket(32)
                    logout.WriteCmd(Commands.CMD_PA_USER_LOGOUT)
                    logout.WriteInt64(int64 otherLoginId)
                    logout.WriteInt64(int64 otherSessId)
                    ctx.AccountSystem.Send(logout)

                    ctx.Logger.LogInformation("TP_BGNPLAY: cleanup для кикнутого {Name}", otherAcctName)

                registry.Unregister(otherPlayer.GpAddr)
            | _ -> ()

            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                let character = db.Characters.Find(chaSlot.ChaId)

                if isNull (box character) then
                    sendErr Commands.ERR_PT_INERR
                else
                    // Валидация статов
                    let maxVal = cfg.MaxStatValue
                    if character.Strength > maxVal || character.Dexterity > maxVal
                       || character.Agility > maxVal || character.Constitution > maxVal
                       || character.Spirit > maxVal || character.Luck > maxVal then
                        ctx.Logger.LogWarning("TP_BGNPLAY: {Cha} — статы превышают MaxVal!", chaSlot.ChaName)
                        sendErr Commands.ERR_PT_BADBOY

                        // Broadcast системное сообщение всем (аналог C++ CMD_MC_SYSINFO)
                        let msg = $"[Account:{auth.AccountName}, Char:{chaSlot.ChaName}] stats exceed MaxVal!"
                        let mutable sysW = WPacket(256)
                        sysW.WriteCmd(Commands.CMD_MC_SYSINFO)
                        sysW.WriteString(msg)
                        ctx.SendToAllClients sysW
                        auth.CurrentCha <- -1
                    else
                        // Обновляем данные из БД
                        chaSlot.GuildId <- if character.GuildId.HasValue then character.GuildId.Value else 0
                        chaSlot.GuildPermission <- uint32 character.GuildPermissions
                        chaSlot.ChatColour <- uint32 character.ChatColor

                        // Переход в Playing_
                        player.State <- Playing_
                            { Auth = auth
                              CurrentChaIndex = chaIndex
                              WorldChatTick = 0L
                              TradeChatTick = 0L
                              WhisperTick = 0L
                              ChangePassCooldown = 0L
                              ChatSessions = Array.empty
                              ChatSessionCount = 0
                              TeamId = 0
                              RefuseToMe = false
                              RefuseSess = false
                              IsCheat = false
                              ChatMoney = 0
                              TradeChatMoney = 0 }

                        registry.UpdatePlaying(player.GpAddr, chaSlot.ChaId, chaSlot.ChaName)

                        // Рейтинг: позиция в Top-5 (1-based) или 0 (аналог C++ swiner)
                        let mutable swiner = 0
                        for i = 0 to MaxOrderNum - 1 do
                            if ctx.Ranking[i].Nid = chaSlot.ChaId then
                                swiner <- i + 1

                        let w = Serialize.tpBgnPlayResponse
                                    { ErrCode = 0s
                                      Data = ValueSome
                                        { Password2 = auth.Password2
                                          ChaId = int64 chaSlot.ChaId
                                          WorldId = int64 chaSlot.ChaId
                                          MapName = character.MapName
                                          Swiner = int64 swiner } }
                        ctx.SendRpcResponse ch sess w

                        ctx.Logger.LogInformation("TP_BGNPLAY: {Name} вошёл на {Map}",
                            chaSlot.ChaName, character.MapName)
            with ex ->
                ctx.Logger.LogError(ex, "TP_BGNPLAY: ошибка БД для {Id}", chaSlot.ChaId)
                sendErr Commands.ERR_PT_DBEXCP
    | _ ->
        sendErr Commands.ERR_PT_INERR

// ══════════════════════════════════════════════════════════
//  TP_ENDPLAY — Возврат к выбору персонажа
// ══════════════════════════════════════════════════════════

let handleTpEndplay (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (_packet: IRPacket) =
    let cfg = ctx.Config
    let registry = ctx.Registry

    match player.State with
    | Playing_ playing ->
        let auth = playing.Auth
        registry.ClearPlaying(player.GpAddr)

        // Уведомление AccountServer
        let mutable billEnd = WPacket(64)
        billEnd.WriteCmd(Commands.CMD_PA_USER_BILLEND)
        billEnd.WriteString(auth.AccountName)
        ctx.AccountSystem.Send(billEnd)

        // Перезагрузка персонажей
        let characters =
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                let chars = db.GetCharactersByAccount(int auth.ActId, cfg.MaxCharacters)
                chars |> Array.map toCharacterSlot
            with ex ->
                ctx.Logger.LogError(ex, "TP_ENDPLAY: ошибка загрузки персонажей для {Act}", auth.ActId)
                auth.Characters

        auth.Characters <- characters
        auth.CurrentCha <- -1
        player.State <- Authorized_ auth

        // Формируем ChaSlotData для сериализации
        let chaSlots = Array.init cfg.MaxCharacters (fun i ->
            if i < characters.Length then
                let c = characters[i]
                { CommandMessages.ChaSlotData.Valid = true; ChaName = c.ChaName; Job = c.Job
                  Degree = int64 c.Level; TypeId = int64 c.TypeId; EquipIds = c.EquipIds }
            else
                { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L; EquipIds = Array.empty })

        let w = Serialize.tpEndPlayResponse
                    { ErrCode = 0s
                      Data = ValueSome
                        { MaxChaNum = int64 cfg.MaxCharacters
                          ChaNum = int64 cfg.MaxCharacters
                          Characters = chaSlots } }
        ctx.SendRpcResponse ch sess w

        ctx.Logger.LogInformation("TP_ENDPLAY: {Name} вернулся к выбору персонажа", auth.AccountName)
    | _ ->
        let w = Serialize.tpEndPlayResponse { ErrCode = Commands.ERR_PT_INERR; Data = ValueNone }
        ctx.SendRpcResponse ch sess w

// ══════════════════════════════════════════════════════════
//  TP_NEWCHA — Создание персонажа
// ══════════════════════════════════════════════════════════

let private isValidCharName (name: string) =
    name.Length >= 1 && name.Length <= 16
    && name |> Seq.forall (fun c -> Char.IsLetterOrDigit(c) || c = '_')

let private isValidAppearance (typeId: int) (hairId: int) (faceId: int) =
    let hairValid =
        match typeId with
        | 1 -> hairId >= 2000 && hairId <= 2007
        | 2 -> hairId >= 2062 && hairId <= 2069
        | 3 -> hairId >= 2124 && hairId <= 2131
        | 4 -> hairId >= 2291 && hairId <= 2294
        | _ -> false
    let faceValid = faceId >= 2554 && faceId <= 2561
    hairValid && faceValid

let handleTpNewcha (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let cfg = ctx.Config

    let req = Deserialize.tpNewChaRequest packet
    let chaName = req.ChaName
    let birthplace = req.Birth
    let typeId = int req.TypeId
    let hairId = int req.HairId
    let faceId = int req.FaceId

    let inline sendResult errCode =
        let w = Serialize.tpNewChaResponse { ErrCode = errCode }
        ctx.SendRpcResponse ch sess w

    match player.State with
    | Authorized_ auth ->
        if auth.CurrentCha >= 0 then
            sendResult Commands.ERR_PT_INERR
        elif auth.Characters.Length >= cfg.MaxCharacters then
            sendResult Commands.ERR_PT_TOMAXCHA
        elif not (isValidCharName chaName) then
            sendResult Commands.ERR_PT_ERRCHANAME
        elif typeId < 1 || typeId > 4 then
            sendResult Commands.ERR_PT_INVALIDDAT
        elif not (isValidAppearance typeId hairId faceId) then
            sendResult Commands.ERR_PT_INVALIDDAT
        else
            try
                use scope = ctx.ScopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()

                let nameExists = db.CharacterNameExists(chaName)

                if nameExists then
                    sendResult Commands.ERR_PT_SAMECHANAME
                else
                    let mapName = resolveMapName cfg birthplace
                    let lookData = buildLookData typeId hairId faceId
                    let newChar = Character()
                    newChar.AccountId <- int auth.ActId
                    newChar.Name <- chaName
                    newChar.Job <- ""
                    newChar.Level <- 0s
                    newChar.MapName <- mapName
                    newChar.MainMap <- mapName
                    newChar.BirthCity <- birthplace
                    newChar.LookData <- lookData
                    newChar.CreatedAt <- DateTimeOffset.UtcNow
                    newChar.LastLoginAt <- DateTimeOffset.UtcNow
                    newChar.MaxHp <- 100
                    newChar.Hp <- 100
                    newChar.MaxSp <- 50
                    newChar.Sp <- 50

                    db.Characters.Add(newChar) |> ignore
                    db.SaveChanges() |> ignore

                    // Перезагружаем список персонажей из БД
                    let chars = db.GetCharactersByAccount(int auth.ActId, cfg.MaxCharacters)
                    auth.Characters <- chars |> Array.map toCharacterSlot
                    sendResult Commands.ERR_SUCCESS
                    ctx.Logger.LogInformation("TP_NEWCHA: {Name} создан для аккаунта {Act}", chaName, auth.ActId)
            with ex ->
                ctx.Logger.LogError(ex, "TP_NEWCHA: ошибка создания {Name}", chaName)
                sendResult Commands.ERR_PT_SERVERBUSY
    | _ ->
        sendResult Commands.ERR_PT_INERR

// ══════════════════════════════════════════════════════════
//  TP_DELCHA — Удаление персонажа
// ══════════════════════════════════════════════════════════

let handleTpDelcha (ctx: HandlerContext) (ch: GateServerIO) (sess: uint32) (player: PlayerRecord) (packet: IRPacket) =
    let registry = ctx.Registry

    let req = Deserialize.tpDelChaRequest packet
    let chaIndex = int req.ChaIndex
    let password2 = req.Password2

    let inline sendResult errCode =
        let w = Serialize.tpDelChaResponse { ErrCode = errCode }
        ctx.SendRpcResponse ch sess w

    match player.State with
    | Authorized_ auth ->
        if auth.CurrentCha >= 0 then
            sendResult Commands.ERR_PT_INERR
        elif chaIndex < 0 || chaIndex >= auth.Characters.Length then
            sendResult Commands.ERR_PT_INVALIDCHA
        elif auth.Password2 <> password2 then
            sendResult Commands.ERR_PT_INVALID_PW2
        else
            let chaSlot = auth.Characters[chaIndex]

            match registry.TryGetByChaId(chaSlot.ChaId) with
            | Some otherPlayer when otherPlayer.GpAddr <> player.GpAddr ->
                sendResult Commands.ERR_PT_MULTICHA
            | _ ->
                let isGuildLeader =
                    chaSlot.GuildId > 0 &&
                    match ctx.Guilds.TryGetValue(chaSlot.GuildId) with
                    | true, guild -> guild.LeaderId = chaSlot.ChaId
                    | _ -> false

                if isGuildLeader then
                    sendResult Commands.ERR_PT_ISGLDLEADER
                else
                    try
                        use scope = ctx.ScopeFactory.CreateScope()
                        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
                        let character = db.Characters.Find(chaSlot.ChaId)
                        if not (isNull (box character)) then
                            character.IsDeleted <- true
                            character.DeletedAt <- Nullable(DateTimeOffset.UtcNow)
                            db.SaveChanges() |> ignore

                        auth.Characters <-
                            auth.Characters
                            |> Array.indexed
                            |> Array.filter (fun (i, _) -> i <> chaIndex)
                            |> Array.map snd

                        sendResult Commands.ERR_SUCCESS
                        ctx.Logger.LogInformation("TP_DELCHA: {Name} удалён", chaSlot.ChaName)
                    with ex ->
                        ctx.Logger.LogError(ex, "TP_DELCHA: ошибка удаления {Id}", chaSlot.ChaId)
                        sendResult Commands.ERR_PT_SERVERBUSY
    | _ ->
        sendResult Commands.ERR_PT_INERR
