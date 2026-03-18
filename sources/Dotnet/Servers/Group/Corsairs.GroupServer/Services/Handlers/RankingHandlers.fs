module Corsairs.GroupServer.Services.Handlers.RankingHandlers

open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain
open Corsairs.GroupServer.Services
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages
open Corsairs.Platform.Database

// ══════════════════════════════════════════════════════════
//  Утилиты рейтинга
// ══════════════════════════════════════════════════════════

/// Получить позицию персонажа в рейтинге (1-based) или 0 если не в топе.
/// Аналог C++ swiner-вычисления в TP_BGNPLAY.
let getSwiner (ranking: OrderInfo array) (chaId: int) : int =
    let mutable result = 0
    for i = 0 to MaxOrderNum - 1 do
        if ranking[i].Nid = chaId then
            result <- i + 1
    result

/// Обновить рейтинг (insertion sort по fightpoint, аналог C++ TBLParam::UpdateOrder).
/// Возвращает true и oldIndex если рейтинг изменился.
let updateOrder (ranking: OrderInfo array) (order: OrderInfo) : bool * int =
    let orderTemp = Array.copy ranking
    let mutable changed = false
    let mutable oldIndex = 0
    let mutable i = 0
    let mutable breakLoop = false

    while i < MaxOrderNum && not breakLoop do
        if orderTemp[i].FightPoint >= order.FightPoint then
            // Если тот же персонаж — уже на месте, выходим
            if orderTemp[i].Nid = order.Nid then
                breakLoop <- true
            else
                i <- i + 1
        else
            oldIndex <- i
            if orderTemp[i].Nid = order.Nid then
                // Тот же персонаж, только обновляем fightpoint
                ranking[i].FightPoint <- order.FightPoint
                breakLoop <- true
            else
                // Вставляем на позицию i, сдвигаем остальные вниз
                ranking[i] <- order
                let mutable a = i + 1
                let mutable n = -1
                while a < MaxOrderNum do
                    if orderTemp[a + n].Nid = order.Nid then
                        n <- n + 1
                    else
                        if a + n < MaxOrderNum then
                            ranking[a] <- orderTemp[a + n]
                        else
                            ranking[a] <- OrderInfo.empty ()
                        a <- a + 1
                changed <- true
                breakLoop <- true

    changed, oldIndex

/// Сохранить рейтинг в БД (таблица param, строка id=1).
let saveParam (ctx: HandlerContext) (ranking: OrderInfo array) =
    try
        use scope = ctx.ScopeFactory.CreateScope()
        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
        let param = db.ServerParams.Find(1)
        if not (isNull (box param)) then
            param.Param1 <- System.Nullable(ranking[0].Nid)
            param.Param2 <- System.Nullable(ranking[1].Nid)
            param.Param3 <- System.Nullable(ranking[2].Nid)
            param.Param4 <- System.Nullable(ranking[3].Nid)
            param.Param5 <- System.Nullable(ranking[4].Nid)
            param.Param6 <- System.Nullable(ranking[0].FightPoint)
            param.Param7 <- System.Nullable(ranking[1].FightPoint)
            param.Param8 <- System.Nullable(ranking[2].FightPoint)
            param.Param9 <- System.Nullable(ranking[3].FightPoint)
            param.Param10 <- System.Nullable(ranking[4].FightPoint)
            db.SaveChanges() |> ignore
    with ex ->
        ctx.Logger.LogError(ex, "SaveParam: ошибка сохранения рейтинга")

/// Загрузить рейтинг из БД при старте (аналог C++ TBLParam::InitParam).
let loadParam (ctx: HandlerContext) (ranking: OrderInfo array) =
    try
        use scope = ctx.ScopeFactory.CreateScope()
        let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
        let param = db.ServerParams.Find(1)
        if not (isNull (box param)) then
            let inline getVal (v: System.Nullable<int>) = if v.HasValue then v.Value else -1

            ranking[0].Nid <- getVal param.Param1
            ranking[1].Nid <- getVal param.Param2
            ranking[2].Nid <- getVal param.Param3
            ranking[3].Nid <- getVal param.Param4
            ranking[4].Nid <- getVal param.Param5
            ranking[0].FightPoint <- getVal param.Param6
            ranking[1].FightPoint <- getVal param.Param7
            ranking[2].FightPoint <- getVal param.Param8
            ranking[3].FightPoint <- getVal param.Param9
            ranking[4].FightPoint <- getVal param.Param10

            // Загрузка имени, уровня и класса из таблицы character
            for n = 0 to MaxOrderNum - 1 do
                if ranking[n].Nid > 0 then
                    let cha = db.Characters.Find(ranking[n].Nid)
                    if not (isNull (box cha)) then
                        ranking[n].Name <- cha.Name
                        ranking[n].Job <- defaultArg (Option.ofObj cha.Job) ""
                        ranking[n].Level <- int cha.Level

            ctx.Logger.LogInformation("LoadParam: рейтинг загружен из БД")
        else
            ctx.Logger.LogWarning("LoadParam: запись param id=1 не найдена")
    with ex ->
        ctx.Logger.LogError(ex, "LoadParam: ошибка загрузки рейтинга")

// ══════════════════════════════════════════════════════════
//  Отправка топ-5 клиенту
// ══════════════════════════════════════════════════════════

/// Отправить CMD_PC_GARNER2_ORDER клиенту (аналог C++ CP_GARNER2_GETORDER).
let sendOrderToClient (ctx: HandlerContext) (player: PlayerRecord) =
    let ranking = ctx.Ranking
    let entries =
        Array.init MaxOrderNum (fun i ->
            { PcGarner2OrderEntry.Name = ranking[i].Name
              Level = int64 ranking[i].Level
              Job = ranking[i].Job
              FightPoint = int64 ranking[i].FightPoint })
    let w = Serialize.pcGarner2OrderMessage { Entries = entries }
    ctx.SendToSingleClient player w

// ══════════════════════════════════════════════════════════
//  CMD_MP_GARNER2_UPDATE — обновление рейтинга от GameServer
// ══════════════════════════════════════════════════════════

let handleMpGarner2Update (ctx: HandlerContext) (player: PlayerRecord) (packet: IRPacket) =
    let msg = Deserialize.mpGarner2UpdateMessage packet
    let chaName = msg.ChaName
    let job = msg.Job

    // Валидация длин (аналог C++ проверок)
    if chaName.Length > 19 then
        ctx.Logger.LogWarning("MP_GARNER2_UPDATE: имя слишком длинное: {Name}", chaName)
    elif job.Length > 99 then
        ctx.Logger.LogWarning("MP_GARNER2_UPDATE: класс слишком длинный: {Job}", job)
    else
        let order =
            { Nid = int msg.Nid
              FightPoint = int msg.Fightpoint
              Name = chaName
              Level = int msg.Level
              Job = job }

        let changed, oldIndex = updateOrder ctx.Ranking order

        if changed then
            // Сохраняем в БД
            saveParam ctx ctx.Ranking

            // Broadcast CMD_PM_GARNER2_UPDATE на первый валидный GateServer (аналог C++)
            let nids = Array.init MaxOrderNum (fun i -> int64 ctx.Ranking[i].Nid)
            let w = Serialize.pmGarner2UpdateMessage { Nids = nids; OldIndex = int64 oldIndex }
            let mutable sent = false
            for gate in ctx.Gates do
                if not sent && not (isNull gate) && gate.IsRegistered then
                    gate.SendPacket(w)
                    sent <- true

            ctx.Logger.LogInformation("MP_GARNER2_UPDATE: рейтинг обновлён")

        // Отправить текущий рейтинг клиенту (C++ вызывает CP_GARNER2_GETORDER после UpdateOrder)
        sendOrderToClient ctx player

// ══════════════════════════════════════════════════════════
//  CMD_MP_GARNER2_CGETORDER — запрос рейтинга от клиента
// ══════════════════════════════════════════════════════════

let handleMpGarner2GetOrder (ctx: HandlerContext) (player: PlayerRecord) (_packet: IRPacket) =
    sendOrderToClient ctx player
