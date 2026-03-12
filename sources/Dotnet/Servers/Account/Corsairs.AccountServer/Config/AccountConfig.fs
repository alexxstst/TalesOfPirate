namespace Corsairs.AccountServer.Config

/// Конфигурация AccountServer.
[<CLIMutable>]
type AccountServerConfig =
    { /// Порт для приёма подключений от GroupServer.
      ListenPort: int
      /// IP для прослушивания.
      ListenAddress: string
      /// Порт для gRPC admin API.
      GrpcPort: int }

    static member Default =
        { ListenPort = 9958
          ListenAddress = "0.0.0.0"
          GrpcPort = 15000 }
