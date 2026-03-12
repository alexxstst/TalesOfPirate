namespace Corsairs.Platform.Protocol.Routing

/// Состояние сессии на GateServer.
[<Struct>]
type SessionState =
    /// Подключён, ожидает аутентификации.
    | Connecting
    /// Прошёл аутентификацию, выбирает персонажа.
    | Authenticated
    /// В игре.
    | InGame
    /// Переключает карту (между GameServer'ами).
    | Switching
    /// Отключается.
    | Disconnecting

/// Источник команды — от какого типа соединения пришла.
[<Struct>]
type CommandSource =
    | FromClient
    | FromGateServer
    | FromGroupServer
    | FromGameServer
    | FromAccountServer
