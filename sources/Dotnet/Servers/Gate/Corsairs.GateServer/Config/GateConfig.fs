namespace Corsairs.GateServer.Config

open Corsairs.Platform.Shared.Config

/// Общие настройки GateServer.
[<CLIMutable>]
type GateConfig =
    { /// Имя этого GateServer.
      Name: string }

    static member Default = { Name = "GateServer1" }

/// Подсистема клиентских подключений.
[<CLIMutable>]
type ClientConfig =
    { /// Внешний IP для клиентов.
      Address: string
      /// Порт для клиентов.
      Port: int
      /// Версия клиента (проверка при подключении).
      ClientVersion: int
      /// Интервал пинга клиентов (сек, 0 = выключен).
      PingInterval: int
      // ── Шифрование ──
      /// Шифрование трафика (0 = нет, 1 = включено).
      CommEncrypt: int
      /// RSA-AES рукопожатие (0 = выключено, 1 = включено).
      RsaAes: int
      // ── Защита ──
      /// Защита от DDoS (0 = выключена, 1 = включена).
      DDoSProtection: int
      /// Окно проверки частоты команд (сек).
      CheckSpan: int
      /// Порог команд для предупреждения.
      CheckWarning: int
      /// Порог команд для отключения.
      CheckError: int
      /// Контроль флуда (0 = базовый, 1 = детекция TCP fake).
      FloodControl: int
      /// Защита от WPE (0 = выключена, 1 = включена). Клиент вставляет 4-байтный счётчик после CMD.
      WpeProtection: bool
      // ── Chaos/PK ──
      /// Включена ли Chaos-карта.
      ChaosActive: bool
      /// Имя Chaos/PK-карты.
      ChaosMap: string }

    static member Default =
        { Address = "0.0.0.0"
          Port = 1973
          ClientVersion = 32125
          PingInterval = 60
          CommEncrypt = 1
          RsaAes = 1
          DDoSProtection = 1
          CheckSpan = 2
          CheckWarning = 20
          CheckError = 40
          FloodControl = 0
          WpeProtection = false
          ChaosActive = true
          ChaosMap = "PKmap" }

/// Подсистема восстановления после дисконнекта.
[<CLIMutable>]
type RecoveryConfig =
    { /// Режим восстановления: "Disabled", "DisconnectOnly", "All".
      Mode: string
      /// Таймаут восстановления (мс).
      Timeout: int
      /// Карты, на которых работает восстановление.
      Maps: string array }

    static member Default =
        { Mode = "Disabled"
          Timeout = 30000
          Maps = [||] }

/// Подсистема подключений от GameServer.
[<CLIMutable>]
type GameServerConfig =
    { /// IP для приёма подключений от GameServer.
      Address: string
      /// TCP порт для GameServer.
      Port: int
      /// Интервал пинга (сек, 0 = выключен).
      PingInterval: int }

    static member Default =
        { Address = "127.0.0.1"
          Port = 1971
          PingInterval = 180 }

/// Подсистема подключения к GroupServer.
[<CLIMutable>]
type GroupServerConfig =
    { /// IP адрес GroupServer.
      Address: string
      /// TCP порт GroupServer.
      Port: int
      /// Интервал пинга (сек, 0 = выключен).
      PingInterval: int
      /// Версия протокола Gate↔Group.
      ProtocolVersion: int }

    static member Default =
        { Address = "127.0.0.1"
          Port = 9957
          PingInterval = 180
          ProtocolVersion = 103 }
