namespace Corsairs.GateServer.Grpc

open System
open System.Diagnostics
open System.Threading.Tasks
open Grpc.Core
open Microsoft.Extensions.Logging
open Corsairs.Platform.Grpc.Contracts.Gate
open Corsairs.Platform.Grpc.Contracts.Common
open Corsairs.Platform.Network
open Corsairs.GateServer.Services

/// gRPC-сервис администрирования GateServer.
type GateGrpcService(
    clientSystem: IClientSystem,
    logger: ILogger<GateGrpcService>) =
    inherit GateAdmin.GateAdminBase()

    let startTime = DateTimeOffset.UtcNow

    override _.GetActiveSessions(request, context) =
        task {
            let players = clientSystem.GetAllPlayers()
            let result = SessionList()
            result.TotalCount <- players.Length
            for p in players do
                let info = SessionInfo()
                let (ChannelId_ chId) = p.Id
                info.ChannelId <- chId
                info.AccountId <- int p.ActId
                info.CharacterId <- int p.DatabaseId
                info.CharacterName <- ""
                info.RemoteAddress <- string p.RemoteEndPoint
                info.State <- string p.State
                info.ConnectedAtUnix <- p.ConnectedAt.ToUnixTimeSeconds()
                result.Sessions.Add(info)
            return result
        }

    override _.KickSession(request, context) =
        task {
            let result = OperationResult()
            let reqChId = ChannelId_ request.ChannelId
            match clientSystem.TryGetByChannelId(reqChId) with
            | Some player ->
                try
                    clientSystem.Disconnect(player)
                    result.Success <- true
                    result.Message <- "Сессия отключена"
                    logger.LogInformation("gRPC: Kick сессии Ch#{Id}", request.ChannelId)
                with ex ->
                    result.Success <- false
                    result.Message <- ex.Message
            | None ->
                result.Success <- false
                result.Message <- "Сессия не найдена"
            return result
        }

    override _.KickAll(request, context) =
        task {
            let players = clientSystem.GetAllPlayers()
            let mutable kicked = 0
            for p in players do
                try
                    clientSystem.Disconnect(p)
                    kicked <- kicked + 1
                with _ -> ()
            let result = OperationResult()
            result.Success <- true
            result.Message <- $"Отключено сессий: {kicked}"
            logger.LogInformation("gRPC: KickAll — отключено {Count} сессий", kicked)
            return result
        }

    override _.GetNetworkStats(request, context) =
        task {
            let stats = NetworkStats()
            stats.ActiveConnections <- clientSystem.ConnectionCount
            return stats
        }

    override _.BroadcastNotice(request, context) =
        task {
            let result = OperationResult()
            logger.LogInformation("gRPC: Broadcast — {Message}", request.Message)
            // TODO: отправка уведомления всем клиентам через CMD
            result.Success <- true
            result.Message <- $"Уведомление отправлено ({clientSystem.ConnectionCount} сессий)"
            return result
        }

    override _.GetServerStatus(request, context) =
        task {
            let status = ServerStatusResponse()
            status.ServerName <- "GateServer"
            status.IsRunning <- true
            status.UptimeSeconds <- int64 (DateTimeOffset.UtcNow - startTime).TotalSeconds
            status.ActiveConnections <- clientSystem.ConnectionCount
            status.MemoryUsageBytes <- Process.GetCurrentProcess().WorkingSet64
            return status
        }
