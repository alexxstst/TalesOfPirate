module Corsairs.GateServer.Program

open Microsoft.AspNetCore.Builder
open Microsoft.AspNetCore.Hosting
open Microsoft.AspNetCore.Server.Kestrel.Core
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Configuration
open Corsairs.GateServer.Config
open Corsairs.GateServer.Services
open Corsairs.GateServer.Grpc
open Corsairs.Platform.Shared.Hosting

[<EntryPoint>]
let main args =
    let builder = ServerHost.createWebBuilder args

    // Конфигурация
    builder.Services.Configure<GateConfig>(builder.Configuration.GetSection("Gate")) |> ignore
    builder.Services.Configure<ClientConfig>(builder.Configuration.GetSection("Client")) |> ignore

    builder.Services.Configure<GameServerConfig>(builder.Configuration.GetSection("GameServer"))
    |> ignore

    builder.Services.Configure<GroupServerConfig>(builder.Configuration.GetSection("GroupServer"))
    |> ignore

    builder.Services.Configure<RecoveryConfig>(builder.Configuration.GetSection("Recovery"))
    |> ignore

    // Сервисы
    builder.Services.AddSingleton<EncryptionService>() |> ignore
    builder.Services.AddSingleton<ClientSystem>() |> ignore
    builder.Services.AddSingleton<GroupServerSystem>() |> ignore
    builder.Services.AddSingleton<GameServerSystem>() |> ignore

    // HostedServices
    builder.Services.AddSingleton<PlayerRecoverySystem>() |> ignore
    builder.Services.AddHostedService(_.GetRequiredService<PlayerRecoverySystem>()) |> ignore
    builder.Services.AddHostedService<GateHostedService>() |> ignore

    // gRPC
    builder.Services.AddGrpc() |> ignore

    // Kestrel — HTTP/2 для gRPC
    let grpcPort =
        match builder.Configuration["GrpcPort"] with
        | null
        | "" -> 15001
        | v -> int v

    builder.WebHost.ConfigureKestrel(fun opts ->
        opts.ListenAnyIP(grpcPort, fun lo -> lo.Protocols <- HttpProtocols.Http2))
    |> ignore

    let app = builder.Build()
    app.MapGrpcService<GateGrpcService>() |> ignore
    app.Run()
    0
