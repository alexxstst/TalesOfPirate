namespace Corsairs.Platform.Shared.Logging

open Microsoft.Extensions.Hosting
open NLog.Web

/// Настройка NLog для серверов.
[<RequireQualifiedAccess>]
module NLogSetup =

    /// Подключить NLog (конфигурация из nlog.config).
    let configure (hostBuilder: IHostBuilder) =
        hostBuilder.UseNLog()
