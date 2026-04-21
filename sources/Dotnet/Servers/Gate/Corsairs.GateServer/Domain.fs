[<AutoOpen>]
module rec Corsairs.GateServer.Domain

open System

[<Struct>]
type PlayerId = PlayerId_ of uint32

[<Struct>]
type CharacterId = CharacterId_ of uint32

[<Struct>]
type GameServerPlayerId = GameServerPlayerId_ of int64

[<Struct>]
type MapName = MapName_ of string

[<Struct>]
type CharacterPosition = { Character: CharacterId; Map: MapName; X: uint32; Y: uint32 }

/// Контекст восстановления игрока после дисконнекта.
[<Struct>]
type PlayerRecoveryPoint =
    {
        /// Положение персонажа.
        CharacterPosition: CharacterPosition
        /// Время сохранения.
        SavedAt: DateTimeOffset
    }

/// Режим восстановления.
[<Struct>]
type RecoveryMode =
    /// Восстановление отключено.
    | Disabled
    /// Только при обрыве соединения (не при штатном logout).
    | DisconnectOnly
    /// При любом выходе из игры.
    | All

/// Состояние системы восстановления.
type RecoveryState =
    { Entries: Map<CharacterId, PlayerRecoveryPoint>
      DroppedByServer: Set<CharacterId>
      AllowedMaps: Set<MapName> }

/// Команда для системы восстановления.
type RecoveryCommand =
    | SavePosition of SavePositionCmd
    | RestorePlayer of RestorePlayerCmd
    | DropByServer of DropByServerCmd
    | ExpireTimedOut of ExpireTimedOutCmd

// ── Ошибки Recovery ──

/// Ошибка выполнения команды восстановления.
type RecoveryError =
    /// Карта не в списке разрешённых.
    | MapNotAllowed of characterId: CharacterId * map: MapName
    /// Игрок уже сброшен сервером.
    | AlreadyDropped of characterId: CharacterId
    /// Запись не найдена.
    | NotFound of characterId: CharacterId



/// Сохранить позицию персонажа.
[<Struct>]
type SavePositionCmd = { Position: CharacterPosition; Timestamp: DateTimeOffset }

/// Восстановить игрока по ID.
[<Struct>]
type RestorePlayerCmd = { CharacterId: CharacterId }

/// Удалить запись по запросу GroupServer.
[<Struct>]
type DropByServerCmd = { CharacterId: CharacterId }

/// Удалить просроченные записи.
[<Struct>]
type ExpireTimedOutCmd = { Cutoff: DateTimeOffset }
