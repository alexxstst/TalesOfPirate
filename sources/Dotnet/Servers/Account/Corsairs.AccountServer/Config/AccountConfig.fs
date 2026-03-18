namespace Corsairs.AccountServer.Config

open System.Collections.Generic

/// Конфигурация AccountServer.
[<CLIMutable>]
type AccountServerConfig =
    { /// Порт для приёма подключений от GroupServer.
      ListenPort: int
      /// IP для прослушивания.
      ListenAddress: string
      /// Порт для gRPC admin API.
      GrpcPort: int
      /// Время "сохранения" аккаунта после логаута (секунды). По умолчанию 15.
      SavingTimeSeconds: int
      /// Зарегистрированные GroupServer'ы: имя → пароль.
      GroupServers: Dictionary<string, string> }

    static member Default =
        { ListenPort = 9958
          ListenAddress = "0.0.0.0"
          GrpcPort = 15000
          SavingTimeSeconds = 15
          GroupServers = Dictionary<string, string>() }
