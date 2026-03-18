namespace rec Corsairs.GroupServer.Services

open System
open Corsairs.GroupServer.Domain
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol

// ══════════════════════════════════════════════════════════
//  GateServer — IO и состояния
// ══════════════════════════════════════════════════════════

/// Данные зарегистрированного GateServer.
type GateOnline =
    { Name: string
      mutable PlayerCount: int
      Channel: GateServerIO }

/// Состояние подключения GateServer.
type GateState =
    | GateConnected_
    | GateOnline_ of GateOnline

/// Канал подключения GateServer.
[<AllowNullLiteral>]
type GateServerIO(socket, ioHandler) =
    inherit ChannelIO(socket, ioHandler)

    let mutable _state: GateState = GateConnected_
    let mutable _index: int = -1

    /// Текущее состояние.
    member _.State with get () = _state and set v = _state <- v

    /// Индекс в массиве слотов.
    member _.GateIndex with get () = _index and set v = _index <- v

    /// Зарегистрирован ли (прошёл TP_LOGIN).
    member _.IsRegistered = match _state with GateOnline_ _ -> true | _ -> false

    /// Имя GateServer.
    member _.Name = match _state with GateOnline_ o -> o.Name | _ -> ""

// ══════════════════════════════════════════════════════════
//  AccountServer — IO
// ══════════════════════════════════════════════════════════

/// Канал подключения к AccountServer.
[<AllowNullLiteral>]
type AccountServerIO(socket, ioHandler) =
    inherit ChannelIO(socket, ioHandler)

// ══════════════════════════════════════════════════════════
//  Player — состояния
// ══════════════════════════════════════════════════════════

/// Данные игрока после логина (до выбора персонажа).
type PlayerAuthorized =
    { ActId: uint32
      LoginId: uint32
      SessionId: uint32
      AccountName: string
      Passport: string
      Password: string
      GmLevel: int8
      ClientIp: string
      Gate: GateServerIO
      GateAddr: uint32
      mutable Password2: string
      mutable Characters: CharacterSlot array
      mutable CharacterCount: int
      mutable CurrentCha: int
      mutable CanReceiveRequests: bool }

/// Данные игрока в игре (выбран персонаж).
type PlayerPlaying =
    { Auth: PlayerAuthorized
      mutable CurrentChaIndex: int
      // Rate-limiting тикеры (Environment.TickCount64)
      mutable WorldChatTick: int64
      mutable TradeChatTick: int64
      mutable WhisperTick: int64
      mutable ChangePassCooldown: int64
      // Чат-сессии (ID сессий)
      mutable ChatSessions: uint32 array
      mutable ChatSessionCount: int
      // Команда
      mutable TeamId: int
      // Флаги
      mutable RefuseToMe: bool
      mutable RefuseSess: bool
      mutable IsCheat: bool
      mutable ChatMoney: int
      mutable TradeChatMoney: int }

/// Состояние игрока.
type PlayerState =
    | PConnected_
    | Authorized_ of PlayerAuthorized
    | Playing_ of PlayerPlaying

/// Запись игрока в реестре.
[<AllowNullLiteral>]
type PlayerRecord() =
    let mutable _state: PlayerState = PConnected_
    let mutable _gpAddr: uint32 = 0u

    /// Текущее состояние.
    member _.State with get () = _state and set v = _state <- v

    /// Адрес на GroupServer (уникальный ID сессии).
    member _.GpAddr with get () = _gpAddr and set v = _gpAddr <- v

    /// В игре ли (выбран персонаж).
    member _.IsPlaying = match _state with Playing_ _ -> true | _ -> false

    /// Авторизован ли.
    member _.IsAuthorized = match _state with Authorized_ _ | Playing_ _ -> true | _ -> false

    // ── Convenience-свойства ──

    member _.ActId =
        match _state with
        | Authorized_ a -> a.ActId
        | Playing_ p -> p.Auth.ActId
        | _ -> 0u

    member _.LoginId =
        match _state with
        | Authorized_ a -> a.LoginId
        | Playing_ p -> p.Auth.LoginId
        | _ -> 0u

    member _.SessionId =
        match _state with
        | Authorized_ a -> a.SessionId
        | Playing_ p -> p.Auth.SessionId
        | _ -> 0u

    member _.AccountName =
        match _state with
        | Authorized_ a -> a.AccountName
        | Playing_ p -> p.Auth.AccountName
        | _ -> ""

    member _.GmLevel =
        match _state with
        | Authorized_ a -> a.GmLevel
        | Playing_ p -> p.Auth.GmLevel
        | _ -> 0y

    member _.Gate: GateServerIO =
        match _state with
        | Authorized_ a -> a.Gate
        | Playing_ p -> p.Auth.Gate
        | _ -> null

    member _.GateAddr =
        match _state with
        | Authorized_ a -> a.GateAddr
        | Playing_ p -> p.Auth.GateAddr
        | _ -> 0u

    member _.ClientIp =
        match _state with
        | Authorized_ a -> a.ClientIp
        | Playing_ p -> p.Auth.ClientIp
        | _ -> ""

    member _.Characters =
        match _state with
        | Authorized_ a -> a.Characters
        | Playing_ p -> p.Auth.Characters
        | _ -> Array.empty

    member _.CurrentCha =
        match _state with
        | Authorized_ a -> a.CurrentCha
        | Playing_ p -> p.CurrentChaIndex
        | _ -> -1

    member _.CurrentChaId =
        match _state with
        | Playing_ p ->
            let auth = p.Auth
            if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
                auth.Characters[p.CurrentChaIndex].ChaId
            else 0
        | _ -> 0

    member _.CurrentChaName =
        match _state with
        | Playing_ p ->
            let auth = p.Auth
            if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
                auth.Characters[p.CurrentChaIndex].ChaName
            else ""
        | _ -> ""

    member _.CurrentGuildId =
        match _state with
        | Playing_ p ->
            let auth = p.Auth
            if p.CurrentChaIndex >= 0 && p.CurrentChaIndex < auth.Characters.Length then
                auth.Characters[p.CurrentChaIndex].GuildId
            else 0
        | _ -> 0

    member _.CanReceiveRequests =
        match _state with
        | Authorized_ a -> a.CanReceiveRequests
        | Playing_ p -> p.Auth.CanReceiveRequests
        | _ -> true

// ══════════════════════════════════════════════════════════
//  Интерфейсы систем
// ══════════════════════════════════════════════════════════

/// Интерфейс системы GateServer-подключений.
[<AllowNullLiteral>]
type IGateServerSystem =
    /// Отправить пакет одному клиенту через Gate (с trailer).
    abstract SendToSingleClient: player: PlayerRecord * packet: WPacket -> unit
    /// Отправить пакет нескольким клиентам через Gate (с trailer).
    abstract SendToClients: players: PlayerRecord array * packet: WPacket -> unit
    /// Broadcast пакет всем онлайн-клиентам.
    abstract SendToAllClients: packet: WPacket -> unit
    /// Кик игрока через GateServer.
    abstract KickUser: gpAddr: uint32 * gtAddr: uint32 -> unit
    /// Найти зарегистрированный GateServer по имени.
    abstract FindGateByName: name: string -> GateServerIO option


/// Интерфейс системы AccountServer.
[<AllowNullLiteral>]
type IAccountServerSystem =
    /// Активный канал к AccountServer (null если не подключён).
    abstract Channel: AccountServerIO
    /// Подключён ли к AccountServer.
    abstract IsConnected: bool
    /// SyncCall к AccountServer (SESS-корреляция с callback).
    abstract SyncCall: packet: WPacket * callback: (IRPacket option -> unit) -> unit
    /// Fire-and-forget отправка.
    abstract Send: packet: WPacket -> unit

/// Интерфейс реестра игроков.
type IPlayerRegistry =
    /// Зарегистрировать игрока.
    abstract Register: gpAddr: uint32 * record: PlayerRecord -> unit
    /// Удалить игрока из реестра.
    abstract Unregister: gpAddr: uint32 -> unit
    /// Обновить индексы при выборе персонажа.
    abstract UpdatePlaying: gpAddr: uint32 * chaId: int * chaName: string -> unit
    /// Очистить индексы персонажа (EndPlay).
    abstract ClearPlaying: gpAddr: uint32 -> unit
    /// Найти по gpAddr.
    abstract TryGetByGpAddr: gpAddr: uint32 -> PlayerRecord option
    /// Найти по ID персонажа.
    abstract TryGetByChaId: chaId: int -> PlayerRecord option
    /// Найти по имени персонажа.
    abstract TryGetByChaName: name: string -> PlayerRecord option
    /// Найти по ID аккаунта.
    abstract TryGetByActId: actId: uint32 -> PlayerRecord option
    /// Все онлайн-игроки.
    abstract GetAllPlayers: unit -> PlayerRecord seq
    /// Количество онлайн-игроков.
    abstract OnlineCount: int
