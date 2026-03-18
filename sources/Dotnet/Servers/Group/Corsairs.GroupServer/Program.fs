module Corsairs.GroupServer.Program

open Microsoft.AspNetCore.Builder
open Microsoft.AspNetCore.Hosting
open Microsoft.AspNetCore.Server.Kestrel.Core
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Configuration
open Corsairs.GroupServer.Config
open Corsairs.GroupServer.Services
open Corsairs.GroupServer.Grpc
open Corsairs.Platform.Database
open Corsairs.Platform.Shared.Hosting

[<EntryPoint>]
let main args =
    let builder = ServerHost.createWebBuilder args

    // Конфигурация
    builder.Services.Configure<GroupServerConfig>(builder.Configuration.GetSection("GroupServer")) |> ignore
    builder.Services.Configure<AccountConnectionConfig>(builder.Configuration.GetSection("AccountServer")) |> ignore

    // База данных (GameDb)
    let dbConfig = builder.Configuration.GetSection("Database")
    let provider = DatabaseServiceExtensions.ParseProvider(dbConfig["Provider"])
    let connName = dbConfig["ConnectionName"] |> Option.ofObj |> Option.defaultValue "GameDb"
    let connStr = builder.Configuration.GetConnectionString(connName)
    builder.Services.AddGameDatabase(provider, connStr) |> ignore

    // Singleton-сервисы
    builder.Services.AddSingleton<PlayerRegistry>() |> ignore
    builder.Services.AddSingleton<InvitationManager>() |> ignore
    builder.Services.AddSingleton<GateServerSystem>() |> ignore
    builder.Services.AddSingleton<IGateServerSystem>(fun sp -> sp.GetRequiredService<GateServerSystem>() :> IGateServerSystem) |> ignore
    builder.Services.AddSingleton<AccountServerSystem>() |> ignore

    // HostedService (оркестратор)
    builder.Services.AddHostedService<GroupHostedService>() |> ignore

    // gRPC
    builder.Services.AddGrpc() |> ignore

    // Kestrel — HTTP/2 для gRPC
    let grpcPort =
        match builder.Configuration.GetSection("GroupServer")["GrpcPort"] with
        | null | "" -> 15002
        | v -> int v
    builder.WebHost.ConfigureKestrel(fun opts ->
        opts.ListenAnyIP(grpcPort, fun lo -> lo.Protocols <- HttpProtocols.Http2))
    |> ignore

    let app = builder.Build()

    app.MapGrpcService<GroupGrpcService>() |> ignore
    app.Run()
    0
