module Corsairs.GroupServer.Program

open Microsoft.AspNetCore.Builder
open Microsoft.AspNetCore.Hosting
open Microsoft.AspNetCore.Server.Kestrel.Core
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Configuration
open Corsairs.GroupServer.Config
open Corsairs.GroupServer.Services
open Corsairs.GroupServer.Grpc
open Corsairs.GroupServer.Handlers.Gate
open Corsairs.GroupServer.Handlers.Game
open Corsairs.GroupServer.Handlers.Account
open Corsairs.Platform.Database
open Corsairs.Platform.Protocol.Routing
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
    let connStr = dbConfig["ConnectionString"]
    builder.Services.AddGameDatabase(provider, connStr) |> ignore

    // Сервисы
    builder.Services.AddSingleton<PlayerRegistry>() |> ignore
    builder.Services.AddScoped<CharacterService>() |> ignore
    builder.Services.AddScoped<GuildService>() |> ignore
    builder.Services.AddSingleton<TeamService>() |> ignore
    builder.Services.AddScoped<FriendService>() |> ignore
    builder.Services.AddSingleton<ChatService>() |> ignore
    builder.Services.AddScoped<MentorshipService>() |> ignore

    // Command handlers
    builder.Services.AddSingleton<ICommandHandler, GateLoginHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, NewCharacterHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, DeleteCharacterHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, GuildCommandHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, FriendCommandHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, ChatCommandHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, GameEnterMapHandler>() |> ignore
    builder.Services.AddSingleton<ICommandHandler, AccountLoginResponseHandler>() |> ignore

    // Router + HostedService
    builder.Services.AddSingleton<CommandRouter>() |> ignore
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
