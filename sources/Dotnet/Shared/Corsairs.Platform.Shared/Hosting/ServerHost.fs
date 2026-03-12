namespace Corsairs.Platform.Shared.Hosting

open System
open Microsoft.Extensions.Configuration
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Microsoft.Extensions.Hosting
open Microsoft.AspNetCore.Builder
open Corsairs.Platform.Shared.Logging

/// Базовый хост для всех серверов.
[<RequireQualifiedAccess>]
module ServerHost =

    /// Запустить хост (async).
    let runAsync (builder: IHostBuilder) =
        task {
            let host = builder.Build()
            do! host.RunAsync()
        }

    /// Создать WebApplicationBuilder с gRPC поддержкой.
    let createWebBuilder (args: string[]) =
        let builder = WebApplication.CreateBuilder(args)
        builder.Configuration
            .SetBasePath(AppContext.BaseDirectory)
            .AddJsonFile("appsettings.json", optional = false, reloadOnChange = true)
            .AddJsonFile("appsettings.local.json", optional = true)
            .AddEnvironmentVariables("CORSAIRS_")
        |> ignore
        builder.Logging.ClearProviders() |> ignore
        NLogSetup.configure builder.Host |> ignore
        builder
