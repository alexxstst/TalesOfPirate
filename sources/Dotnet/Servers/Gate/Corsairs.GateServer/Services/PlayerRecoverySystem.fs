namespace Corsairs.GateServer.Services

open System
open System.Runtime.CompilerServices
open System.Threading
open System.Threading.Tasks
open Corsairs.GateServer
open Corsairs.GateServer.Config
open Microsoft.Extensions.Hosting
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Options

/// Позиция сохранена.
[<Struct>]
type PositionSavedEvent = { Position: CharacterPosition; Timestamp: DateTimeOffset }

/// Игрок восстановлен.
[<Struct>]
type PositionRestoredEvent = { Point: PlayerRecoveryPoint }

/// Запись удалена по запросу сервера.
[<Struct>]
type DroppedByServerEvent = { CharacterId: CharacterId }

/// ID помечен как сброшенный (запись не найдена).
[<Struct>]
type MarkedAsDroppedEvent = { CharacterId: CharacterId }

/// Запись просрочена и удалена.
[<Struct>]
type EntryExpiredEvent = { CharacterId: CharacterId }

/// Событие, порождённое командой восстановления.
type RecoveryEvent =
    | PositionSaved of PositionSavedEvent
    | PositionRestored of PositionRestoredEvent
    | DroppedByServer of DroppedByServerEvent
    | MarkedAsDropped of MarkedAsDroppedEvent
    | EntryExpired of EntryExpiredEvent


/// Аггрегат системы восстановления (CQRS).
module RecoveryAggregate =

    /// Создать начальное состояние с разрешёнными картами.
    let create (maps: MapName seq) : RecoveryState =
        { Entries = Map.empty
          DroppedByServer = Set.empty
          AllowedMaps = Set.ofSeq maps }

    /// Обработать команду -> Result с событиями или ошибкой.
    let executeCommand
        (state: RecoveryState)
        (command: RecoveryCommand)
        : Result<RecoveryEvent list, RecoveryError> =
        match command with
        | SavePosition cmd ->
            if not (state.AllowedMaps.Contains(cmd.Position.Map)) then
                Error(MapNotAllowed(cmd.Position.Character, cmd.Position.Map))
            elif state.DroppedByServer.Contains(cmd.Position.Character) then
                Error(AlreadyDropped cmd.Position.Character)
            else
                Ok [ PositionSaved { Position = cmd.Position; Timestamp = cmd.Timestamp } ]

        | RestorePlayer cmd ->
            match Map.tryFind cmd.CharacterId state.Entries with
            | Some entry -> Ok [ PositionRestored { Point = entry } ]
            | None -> Error(NotFound cmd.CharacterId)

        | DropByServer cmd ->
            if state.Entries.ContainsKey(cmd.CharacterId) then
                Ok [ RecoveryEvent.DroppedByServer { CharacterId = cmd.CharacterId } ]
            else
                Ok [ MarkedAsDropped { CharacterId = cmd.CharacterId } ]

        | ExpireTimedOut cmd ->
            let expired =
                state.Entries
                |> Map.fold
                    (fun acc id point ->
                        if point.SavedAt < cmd.Cutoff then
                            EntryExpired { CharacterId = id } :: acc
                        else
                            acc)
                    []

            Ok expired

    /// Применить событие к состоянию, вернуть новое состояние.
    let apply (state: RecoveryState) (event: RecoveryEvent) : RecoveryState =
        match event with
        | PositionSaved e ->
            let point = { CharacterPosition = e.Position; SavedAt = e.Timestamp }

            { state with Entries = Map.add e.Position.Character point state.Entries }

        | PositionRestored e ->
            { state with
                Entries = Map.remove e.Point.CharacterPosition.Character state.Entries }

        | RecoveryEvent.DroppedByServer e ->
            { state with Entries = Map.remove e.CharacterId state.Entries }

        | MarkedAsDropped e ->
            { state with DroppedByServer = Set.add e.CharacterId state.DroppedByServer }

        | EntryExpired e -> { state with Entries = Map.remove e.CharacterId state.Entries }

// ── Обработчик событий ──

module RecoveryEvents =
    /// Обработать событие восстановления (логирование).
    let handle (logger: ILogger) (event: RecoveryEvent) =
        match event with
        | PositionSaved e ->
            logger.LogInformation(
                "Recovery: сохранён {CharacterId} на {Map} ({X},{Y})",
                e.Position.Character,
                e.Position.Map,
                e.Position.X,
                e.Position.Y
            )

        | PositionRestored e ->
            let pos = e.Point.CharacterPosition

            logger.LogInformation(
                "Recovery: восстановлен {CharacterId} на {Map} ({X},{Y})",
                pos.Character,
                pos.Map,
                pos.X,
                pos.Y
            )

        | RecoveryEvent.DroppedByServer e ->
            logger.LogDebug("Recovery: удалён по запросу сервера {CharacterId}", e.CharacterId)

        | MarkedAsDropped e ->
            logger.LogDebug("Recovery: помечен как сброшенный {CharacterId}", e.CharacterId)

        | EntryExpired e -> logger.LogDebug("Recovery: таймаут {CharacterId}", e.CharacterId)

// ── BackgroundService ──

/// Система восстановления позиции игрока после дисконнекта.
/// При отключении сохраняет последнюю позицию, при реконнекте — возвращает на место.
type PlayerRecoverySystem(config: IOptions<RecoveryConfig>, logger: ILogger<PlayerRecoverySystem>) =
    inherit BackgroundService()

    let cfg = config.Value
    let _mode = parseRecoveryMode cfg.Mode
    let _timeoutMs = int64 cfg.Timeout

    let maps = cfg.Maps |> Array.map MapName_ |> Array.toSeq

    let mutable _state = RecoveryAggregate.create maps

    /// Выполнить команду: валидация -> события -> apply -> логирование.
    /// Возвращает Ok events или Error err.
    [<MethodImpl(MethodImplOptions.Synchronized)>]
    member private _.Execute(cmd: RecoveryCommand) =
        match RecoveryAggregate.executeCommand _state cmd with
        | Ok events ->
            for e in events do
                _state <- RecoveryAggregate.apply _state e
                RecoveryEvents.handle logger e

            Ok events
        | Error err -> Error err

    /// Сохранить позицию игрока (при дисконнекте).
    /// Возвращает true если сохранено, false если карта не в whitelist или сервер уже сбросил игрока.
    member this.Add(position: CharacterPosition) =
        match this.Execute(SavePosition { Position = position; Timestamp = DateTimeOffset.UtcNow }) with
        | Ok _ -> true
        | Error _ -> false

    /// Найти и забрать контекст восстановления (при реконнекте).
    /// Возвращает ValueSome если найден — запись удаляется из системы.
    member this.TryTake(characterId: CharacterId) =
        match this.Execute(RestorePlayer { CharacterId = characterId }) with
        | Ok(PositionRestored e :: _) -> ValueSome e.Point
        | _ -> ValueNone

    /// Удалить запись по запросу GameServer (CMD_MC_DROP_RESTORE).
    /// Если записи нет — помечает ID как «сброшенный», чтобы будущий Add не сохранял.
    member this.RemoveByServer(characterId: CharacterId) =
        this.Execute(DropByServer { CharacterId = characterId }) |> ignore

    /// Проверить таймауты и удалить просроченные записи.
    member this.CheckTimeouts() =
        let cutoff = DateTimeOffset.UtcNow.AddMilliseconds(float -_timeoutMs)

        this.Execute(ExpireTimedOut { Cutoff = cutoff }) |> ignore

    /// Количество записей в системе.
    member _.Count = _state.Entries.Count

    override this.ExecuteAsync(ct: CancellationToken) =
        task {
            logger.LogInformation(
                "Recovery: запущен (таймаут: {Timeout}мс, режим: {Mode})",
                _timeoutMs,
                _mode
            )

            while not ct.IsCancellationRequested do
                try
                    do! Task.Delay(1000, ct)
                    this.CheckTimeouts()
                with
                | :? OperationCanceledException -> ()
                | ex -> logger.LogError(ex, "Recovery: ошибка в цикле")

            logger.LogInformation("Recovery: остановлен")
        }
        :> Task
