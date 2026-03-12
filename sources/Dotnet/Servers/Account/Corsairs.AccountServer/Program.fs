module Corsairs.AccountServer.Program

open Microsoft.AspNetCore.Builder
open Microsoft.AspNetCore.Hosting
open Microsoft.AspNetCore.Server.Kestrel.Core
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Configuration
open Corsairs.AccountServer.Config
open Corsairs.AccountServer.Services
open Corsairs.AccountServer.Grpc
open Corsairs.AccountServer.Handlers
open Corsairs.Platform.Database
open Corsairs.Platform.Protocol.Routing
open Corsairs.Platform.Shared.Hosting

[<EntryPoint>]
let main args =
    let builder = ServerHost.createWebBuilder args

    // Конфигурация
    builder.Services.Configure<AccountServerConfig>(builder.Configuration.GetSection("AccountServer")) |> ignore

    // База данных
    let dbConfig = builder.Configuration.GetSection("Database")
    let provider = DatabaseServiceExtensions.ParseProvider(dbConfig["Provider"])
    let connStr = dbConfig["ConnectionString"]
    builder.Services.AddAccountDatabase(provider, connStr) |> ignore

    // Сервисы
    builder.Services.AddSingleton<AuthService>() |> ignore

    // Command handlers
    builder.Services.AddSingleton<ICommandHandler, UserLoginHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, UserLogoutHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, RegisterHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, BanHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, UnbanHandler>() |> ignore

    // Router + HostedService
    builder.Services.AddSingleton<CommandRouter>() |> ignore
    builder.Services.AddHostedService<AccountHostedService>() |> ignore

    // gRPC
    builder.Services.AddGrpc() |> ignore

    // Kestrel — HTTP/2 для gRPC
    let grpcPort =
        match builder.Configuration.GetSection("AccountServer")["GrpcPort"] with
        | null | "" -> 15000
        | v -> int v
    builder.WebHost.ConfigureKestrel(fun opts ->
        opts.ListenAnyIP(grpcPort, fun lo -> lo.Protocols <- HttpProtocols.Http2))
    |> ignore

    let app = builder.Build()
    app.MapGrpcService<AccountGrpcService>() |> ignore
    app.Run()
    0
