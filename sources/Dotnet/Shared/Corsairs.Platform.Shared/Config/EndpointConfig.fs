namespace Corsairs.Platform.Shared.Config

/// Конфигурация сетевого endpoint'а.
[<CLIMutable>]
type EndpointConfig =
    { Address: string
      Port: int }

/// Конфигурация подключения к БД.
[<CLIMutable>]
type DatabaseConfig =
    { /// Тип провайдера: sqlserver, postgresql, mysql, sqlite
      Provider: string
      /// Строка подключения.
      ConnectionString: string }
