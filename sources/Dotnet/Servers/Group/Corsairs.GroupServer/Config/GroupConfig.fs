namespace Corsairs.GroupServer.Config

/// Конфигурация GroupServer.
[<CLIMutable>]
type GroupServerConfig =
    { /// Порт для Gate-подключений.
      GateListenPort: int
      /// IP для прослушивания.
      ListenAddress: string
      /// gRPC порт для админ-панели.
      GrpcPort: int
      /// Имя сервера.
      ServerName: string
      /// Максимум GateServer подключений.
      MaxGates: int
      /// Максимум персонажей на аккаунт.
      MaxCharacters: int
      /// Максимум значения иконки.
      MaxIconValue: int
      /// Максимум онлайн-игроков.
      MaxOnlinePlayers: int
      /// Максимум друзей.
      MaxFriends: int
      /// Максимум групп друзей.
      MaxFriendGroups: int
      /// Таймаут приглашения в друзья (мс).
      FriendInviteTimeoutMs: int
      /// Максимум участников команды.
      MaxTeamMembers: int
      /// Таймаут приглашения в команду (мс).
      TeamInviteTimeoutMs: int
      /// Максимум наставников.
      MaxMasters: int
      /// Максимум учеников.
      MaxPrentices: int
      /// Таймаут приглашения наставника (мс).
      MasterInviteTimeoutMs: int
      /// Интервал мирового чата (мс).
      WorldChatIntervalMs: int
      /// Интервал торгового чата (мс).
      TradeChatIntervalMs: int
      /// Интервал личных сообщений (мс).
      WhisperIntervalMs: int
      /// Стоимость мирового сообщения.
      WorldChatFee: int
      /// Интервал восстановления стоимости (мс).
      WorldChatFeeInterval: int
      /// Версия протокола.
      ProtocolVersion: int
      /// Максимальное допустимое значение характеристики (защита от читов).
      MaxStatValue: int
      /// Максимальный размер 2-го пароля.
      MaxPassword2Length: int }

    static member Default =
        { GateListenPort = 1975
          ListenAddress = "0.0.0.0"
          GrpcPort = 15002
          ServerName = "GroupServer"
          MaxGates = 10
          MaxCharacters = 10
          MaxIconValue = 9999
          MaxOnlinePlayers = 50
          MaxFriends = 100
          MaxFriendGroups = 10
          FriendInviteTimeoutMs = 30000
          MaxTeamMembers = 5
          TeamInviteTimeoutMs = 30000
          MaxMasters = 1
          MaxPrentices = 4
          MasterInviteTimeoutMs = 30000
          WorldChatIntervalMs = 10000
          TradeChatIntervalMs = 10000
          WhisperIntervalMs = 0
          WorldChatFee = 1000
          WorldChatFeeInterval = 1000
          ProtocolVersion = 103
          MaxStatValue = 999
          MaxPassword2Length = 32 }

/// Конфигурация подключения к AccountServer.
[<CLIMutable>]
type AccountConnectionConfig =
    { Address: string
      Port: int
      Password: string
      PingIntervalSec: int }
