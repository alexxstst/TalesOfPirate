namespace rec Corsairs.GateServer.Services

open System
open Corsairs.GateServer.Domain
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Crypto
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol

// ══════════════════════════════════════════════════════════
//  GameServer — типы и IO
// ══════════════════════════════════════════════════════════

/// Данные зарегистрированного GameServer.
type GameServerOnline =
    { Name: string
      Maps: Set<MapName>
      mutable PlayerCount: int
      Channel: GameServerIO }

/// Состояние GameServerIO.
type GameServerState =
    | Connected_
    | Online_ of GameServerOnline

[<AllowNullLiteral>]
type GroupServerIO(socket, ioHandler) =
    inherit ChannelIO(socket, ioHandler)


/// Канал подключения GameServer (аналог struct GameServer в C++).
[<AllowNullLiteral>]
type GameServerIO(socket, ioHandler) =
    inherit ChannelIO(socket, ioHandler)

    let mutable _state: GameServerState = Connected_

    /// Текущее состояние.
    member _.State with get () = _state and set v = _state <- v

    /// Зарегистрирован ли (прошёл MT_LOGIN).
    member _.IsRegistered = match _state with Online_ _ -> true | _ -> false

    /// Имя GameServer (пусто если не зарегистрирован).
    member _.Name = match _state with Online_ o -> o.Name | _ -> ""

    /// Карты, обслуживаемые этим GameServer.
    member _.Maps = match _state with Online_ o -> o.Maps | _ -> Set.empty

    /// Количество игроков на этом GameServer.
    member _.PlayerCount
        with get () = match _state with Online_ o -> o.PlayerCount | _ -> 0
        and set v = match _state with Online_ o -> o.PlayerCount <- v | _ -> ()

// ══════════════════════════════════════════════════════════
//  Player — типы и IO
// ══════════════════════════════════════════════════════════

/// Данные авторизованного игрока (после логина, до выбора персонажа).
type PlayerAuthorized =
    { ActId: uint32
      LoginId: uint32
      Password: string
      GroupServerPlayerId: uint32
      Channel: PlayerChannelIO }

/// Режим выхода игрока.
[<Struct>]
type PlayerExitMode =
    | NoExit
    | ExitToCharSelect
    | ExitToLogout

/// Данные игрока в игре (выбран персонаж, вошёл на карту).
type PlayerPlaying =
    { Auth: PlayerAuthorized
      Channel: PlayerChannelIO
      mutable DatabaseId: uint32
      mutable WorldId: uint32
      mutable GameServer: GameServerIO
      mutable GameServerPlayerId: GameServerPlayerId option
      mutable GroupServerPlayerId: uint32
      mutable MapName: string
      mutable GarnerWinner: int16
      mutable ExitStatus: PlayerExitMode }

/// Состояние канала игрока.
type PlayerState =
    | PConnected_
    | Authorized_ of PlayerAuthorized
    | Playing_ of PlayerPlaying

/// Канал подключения клиента (аналог ClientConnection в C++).
/// Использует DU-состояния: Connected_ → Authorized_ → Playing_.
[<AllowNullLiteral>]
type PlayerChannelIO(socket, ioHandler) =
    inherit ChannelIO(socket, ioHandler)

    let mutable _state: PlayerState = PConnected_
    let mutable _estop = false
    let mutable _packetCounter = 0u
    let mutable _crypto: ICryptoMiddleware = null
    let _connectedAt = DateTimeOffset.UtcNow

    let _clientIp =
        let ep = socket.RemoteEndPoint :?> System.Net.IPEndPoint
        let ipBytes = ep.Address.GetAddressBytes()

        if ipBytes.Length >= 4 then
            (uint32 ipBytes[3] <<< 24)
            ||| (uint32 ipBytes[2] <<< 16)
            ||| (uint32 ipBytes[1] <<< 8)
            ||| uint32 ipBytes[0]
        else
            0u

    /// Текущее состояние.
    member _.State with get () = _state and set v = _state <- v

    /// Время подключения.
    member _.ConnectedAt = _connectedAt

    /// IP-адрес клиента как uint32 (network byte order).
    member _.ClientIp = _clientIp

    /// Флаг блокировки (эстоп). Аналог m_estop в C++.
    member _.Estop with get () = _estop and set v = _estop <- v

    /// Криптографический middleware (AES после handshake).
    member _.Crypto = _crypto

    /// Установить крипто-middleware (вызывается после RSA handshake).
    member _.SetCrypto(crypto: ICryptoMiddleware) = _crypto <- crypto

    /// Шифрование исходящих пакетов (AES-256-GCM).
    /// Формат: [2b size][4b SESS][encrypted(CMD+payload)].
    override _.EncryptOutgoing(packet: WPacket) =
        if isNull _crypto || not _crypto.IsActive || packet.GetPacketSize() <= 6 then
            packet
        else
            let frame = packet.Data.Span
            let frameLen = packet.GetPacketSize()
            let plaintextLen = frameLen - 6
            let maxEncLen = plaintextLen + _crypto.Overhead
            let mutable w = WPacket(maxEncLen)
            let dst = w.Data
            // SESS
            frame.Slice(2, 4).CopyTo(dst.Span.Slice(2))
            // Encrypt CMD+payload прямо в буфер нового пакета
            let encLen = _crypto.Encrypt(frame.Slice(6, plaintextLen), dst.Slice(6))
            if encLen < 0 then
                w.Dispose()
                packet
            else
                w.SetPacketSize(6 + encLen)
                packet.Dispose()
                w

    /// Расшифровка входящих пакетов (AES-256-GCM).
    /// Формат входа: [2b size][4b SESS][encrypted(CMD+payload)].
    override _.DecryptIncoming(packet: IRPacket) =
        if isNull _crypto || not _crypto.IsActive then
            packet
        else
            let raw = packet.GetPacketMemory()
            let pktSize = packet.GetPacketSize()
            if pktSize <= 6 then
                packet
            else
                let encLen = pktSize - 6
                let decLen = encLen - _crypto.Overhead
                if decLen < 0 then packet
                else
                    let newLen = 6 + decLen
                    let buf = Array.zeroCreate<byte> newLen
                    BinaryIO.writeUInt16 (buf.AsSpan()) (uint16 newLen)
                    raw.Span.Slice(2, 4).CopyTo(buf.AsSpan(2)) // SESS
                    // Decrypt прямо в итоговый буфер (offset 6 = CMD+payload)
                    let written = _crypto.Decrypt(raw.Span.Slice(6, encLen), Memory<byte>(buf, 6, decLen))
                    if written < 0 then packet
                    else new RPacket(Memory<byte>(buf)) :> IRPacket

    /// Счётчик пакетов для WPE-защиты (аналог m_pktn в C++).
    member _.PacketCounter = _packetCounter

    /// Инкрементировать счётчик пакетов.
    member _.IncrementPacketCounter() = _packetCounter <- _packetCounter + 1u

    /// Авторизован ли (прошёл логин).
    member _.IsAuthorized =
        match _state with Authorized_ _ | Playing_ _ -> true | _ -> false

    /// В игре ли (выбрал персонажа, вошёл на карту).
    member _.IsPlaying =
        match _state with Playing_ _ -> true | _ -> false

    // ── Convenience-свойства (pattern matching на state) ──

    /// Actor ID (доступен в Authorized_ и Playing_).
    member _.ActId =
        match _state with
        | Authorized_ a -> a.ActId
        | Playing_ p -> p.Auth.ActId
        | _ -> 0u

    /// Login ID (доступен в Authorized_ и Playing_).
    member _.LoginId =
        match _state with
        | Authorized_ a -> a.LoginId
        | Playing_ p -> p.Auth.LoginId
        | _ -> 0u

    /// Пароль (доступен в Authorized_ и Playing_).
    member _.Password =
        match _state with
        | Authorized_ a -> a.Password
        | Playing_ p -> p.Auth.Password
        | _ -> ""

    /// Database ID (только Playing_).
    member _.DatabaseId =
        match _state with Playing_ p -> p.DatabaseId | _ -> 0u

    /// World ID (только Playing_).
    member _.WorldId =
        match _state with Playing_ p -> p.WorldId | _ -> 0u

    /// Текущий GameServer (только Playing_, иначе null).
    member _.Game =
        match _state with Playing_ p -> p.GameServer | _ -> null

    /// Адрес на GameServer (только Playing_).
    member _.GmAddr =
        match _state with Playing_ p -> p.GameServerPlayerId | _ -> None

    /// Адрес на GroupServer (Authorized_ и Playing_).
    member _.GpAddr =
        match _state with
        | Playing_ p -> p.GroupServerPlayerId
        | Authorized_ a -> a.GroupServerPlayerId
        | _ -> 0u

    /// Имя текущей карты (только Playing_).
    member _.MapName =
        match _state with Playing_ p -> p.MapName | _ -> ""

    /// Garner winner (только Playing_).
    member _.GarnerWinner =
        match _state with Playing_ p -> p.GarnerWinner | _ -> 0s

    /// Режим выхода (только Playing_).
    member _.ExitStatus =
        match _state with Playing_ p -> p.ExitStatus | _ -> NoExit

// ══════════════════════════════════════════════════════════
//  Интерфейсы систем
// ══════════════════════════════════════════════════════════

/// Интерфейс системы клиентских подключений.
[<AllowNullLiteral>]
type IClientSystem =
    /// Количество подключённых клиентов.
    abstract ConnectionCount: int
    /// Ожидаемая версия клиента.
    abstract ExpectedVersion: uint16
    /// Найти игрока по PlayerId (= ChannelId из EnterMap trailer).
    abstract GetPlayerById: PlayerId -> PlayerChannelIO option
    /// Найти играющего игрока по PlayerId (только в состоянии Playing_).
    abstract GetPlayingPlayerById: PlayerId -> PlayerPlaying option
    /// Найти игрока по ChannelId.
    abstract TryGetByChannelId: ChannelId -> PlayerChannelIO option
    /// Все подключённые игроки.
    abstract GetAllPlayers: unit -> PlayerChannelIO array
    /// Отключить игрока.
    abstract Disconnect: PlayerChannelIO -> unit

/// Интерфейс системы подключения к GroupServer.
[<AllowNullLiteral>]
type IGroupServerSystem =
    /// Активный канал к GroupServer (null если не подключён).
    abstract Channel: ChannelIO
    /// Подключён ли к GroupServer.
    abstract IsConnected: bool
    /// Переслать пакет на GroupServer.
    abstract Send: packet: IRPacket -> unit
    /// Синхронный вызов: отправить пакет и обработать ответ в callback.
    /// Callback получает Some(response) при успехе, None при timeout/ошибке сети.
    abstract SyncCall: channel: ChannelIO * packet: WPacket * callback: (IRPacket option -> unit) -> unit

/// Интерфейс системы подключений от GameServer.
[<AllowNullLiteral>]
type IGameServerSystem =
    /// Активный канал от GameServer (null если не подключён).
    abstract Channel: ChannelIO
    /// Подключён ли GameServer.
    abstract IsConnected: bool
    /// Найти GameServer по карте.
    abstract FindByMap: map: MapName -> GameServerIO option
    /// Переслать пакет на все зарегистрированные GameServer.
    abstract BroadcastAll: packet: IRPacket -> unit
