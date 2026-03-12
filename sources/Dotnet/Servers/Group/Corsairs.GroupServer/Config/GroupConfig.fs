namespace Corsairs.GroupServer.Config

/// Конфигурация GroupServer.
[<CLIMutable>]
type GroupServerConfig =
    { /// Порт для Gate-подключений.
      GateListenPort: int
      /// Порт для Game-подключений.
      GameListenPort: int
      /// IP для прослушивания.
      ListenAddress: string
      /// gRPC порт для админ-панели.
      GrpcPort: int }

    static member Default =
        { GateListenPort = 9957
          GameListenPort = 9956
          ListenAddress = "0.0.0.0"
          GrpcPort = 15002 }

/// Конфигурация подключения к AccountServer.
[<CLIMutable>]
type AccountConnectionConfig =
    { Address: string
      Port: int }
