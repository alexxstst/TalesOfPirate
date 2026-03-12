namespace Corsairs.GroupServer.Grpc

open System
open System.Diagnostics
open System.Linq
open System.Threading.Tasks
open Grpc.Core
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Grpc.Contracts.Group
open Corsairs.Platform.Grpc.Contracts.Common
open Corsairs.GroupServer.Services

/// gRPC-сервис администрирования GroupServer.
type GroupGrpcService(
    registry: PlayerRegistry,
    scopeFactory: IServiceScopeFactory,
    logger: ILogger<GroupGrpcService>) =
    inherit GroupAdmin.GroupAdminBase()

    let startTime = DateTimeOffset.UtcNow

    override _.GetOnlinePlayers(request, context) =
        task {
            let allPlayers = registry.GetAllPlayers()
            let filtered =
                if String.IsNullOrEmpty(request.NameFilter) then allPlayers
                else
                    allPlayers |> Array.filter (fun p ->
                        p.CharacterName.Contains(request.NameFilter, StringComparison.OrdinalIgnoreCase))
            let page = max 1 request.Pagination.Page
            let pageSize = max 10 (min 100 request.Pagination.PageSize)
            let total = filtered.Length
            let paged = filtered |> Array.skip (min ((page - 1) * pageSize) total) |> Array.truncate pageSize

            let result = PlayerList()
            for p in paged do
                let info = PlayerInfo()
                info.AccountId <- p.AccountId
                info.CharacterId <- p.CharacterId
                info.CharacterName <- if isNull p.CharacterName then "" else p.CharacterName
                info.Level <- int p.Level
                info.Job <- if isNull p.Job then "" else p.Job
                info.MapName <- if isNull p.MapName then "" else p.MapName
                info.GuildName <- if isNull p.GuildName then "" else p.GuildName
                info.GateChannelId <- p.GateChannelId
                info.GameChannelId <- p.GameChannelId
                result.Players.Add(info)

            let pag = PaginationInfo()
            pag.TotalCount <- total
            pag.Page <- page
            pag.PageSize <- pageSize
            pag.TotalPages <- (total + pageSize - 1) / pageSize
            result.Pagination <- pag
            return result
        }

    override _.GetPlayerInfo(request, context) =
        task {
            let player =
                match request.IdentifierCase with
                | GetPlayerInfoRequest.IdentifierOneofCase.CharacterId ->
                    match registry.TryGetByCharacter(request.CharacterId) with
                    | ValueSome p -> p | ValueNone -> null
                | GetPlayerInfoRequest.IdentifierOneofCase.CharacterName ->
                    match registry.TryGetByName(request.CharacterName) with
                    | ValueSome p -> p | ValueNone -> null
                | _ -> null
            if isNull player then
                raise (RpcException(Status(StatusCode.NotFound, "Игрок не найден")))
            let info = PlayerInfo()
            info.AccountId <- player.AccountId
            info.CharacterId <- player.CharacterId
            info.CharacterName <- if isNull player.CharacterName then "" else player.CharacterName
            info.Level <- int player.Level
            info.Job <- if isNull player.Job then "" else player.Job
            info.MapName <- if isNull player.MapName then "" else player.MapName
            info.GuildName <- if isNull player.GuildName then "" else player.GuildName
            info.GateChannelId <- player.GateChannelId
            info.GameChannelId <- player.GameChannelId
            return info
        }

    override _.KickPlayer(request, context) =
        task {
            let result = OperationResult()
            match registry.TryGetByCharacter(request.CharacterId) with
            | ValueSome player ->
                registry.Unregister(player.AccountId)
                result.Success <- true
                result.Message <- $"Игрок {player.CharacterName} отключён"
                logger.LogInformation("gRPC: Kick игрока {Name} (ID:{Id})", player.CharacterName, request.CharacterId)
            | ValueNone ->
                result.Success <- false
                result.Message <- "Игрок не найден"
            return result
        }

    override _.GetGuilds(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
            let query =
                if String.IsNullOrEmpty(request.NameFilter) then
                    db.Guilds.AsQueryable()
                else
                    db.Guilds.Where(fun g -> g.Name.Contains(request.NameFilter))
            let page = max 1 request.Pagination.Page
            let pageSize = max 10 (min 100 request.Pagination.PageSize)
            let! total = query.CountAsync()
            let! guilds = query.OrderBy(fun g -> g.Id).Skip((page - 1) * pageSize).Take(pageSize).ToListAsync()
            let result = GuildList()
            for g in guilds do
                let info = GuildInfo()
                info.GuildId <- g.Id
                info.Name <- g.Name
                info.LeaderName <- string g.LeaderCharacterId
                info.Level <- g.Level
                info.MemberCount <- int g.MemberCount
                info.CreatedAtUnix <- g.CreatedAt.ToUnixTimeSeconds()
                result.Guilds.Add(info)
            let pag = PaginationInfo()
            pag.TotalCount <- total
            pag.Page <- page
            pag.PageSize <- pageSize
            pag.TotalPages <- (total + pageSize - 1) / pageSize
            result.Pagination <- pag
            return result
        }

    override _.DisbandGuild(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let guildService = scope.ServiceProvider.GetRequiredService<GuildService>()
            let! success = guildService.DisbandGuild(request.GuildId)
            let result = OperationResult()
            result.Success <- success
            result.Message <- if success then "Гильдия расформирована" else "Гильдия не найдена"
            if success then
                logger.LogInformation("gRPC: Расформирована гильдия {Id}", request.GuildId)
            return result
        }

    override _.SendGlobalMessage(request, context) =
        task {
            let result = OperationResult()
            logger.LogInformation("gRPC: Глобальное сообщение — {Message}", request.Message)
            // TODO: отправка через ChatService
            result.Success <- true
            result.Message <- "Сообщение отправлено"
            return result
        }

    override _.MutePlayer(request, context) =
        task {
            let result = OperationResult()
            logger.LogInformation("gRPC: Мут игрока {Id} на {Sec}с", request.CharacterId, request.DurationSeconds)
            // TODO: реализовать систему мутов
            result.Success <- true
            result.Message <- "Игрок замучен"
            return result
        }

    override _.GetServerTopology(request, context) =
        task {
            let topo = TopologyResponse()
            topo.TotalOnline <- registry.OnlineCount
            // TODO: заполнить данные о подключённых серверах
            return topo
        }

    override _.GetServerStatus(request, context) =
        task {
            let status = ServerStatusResponse()
            status.ServerName <- "GroupServer"
            status.IsRunning <- true
            status.UptimeSeconds <- int64 (DateTimeOffset.UtcNow - startTime).TotalSeconds
            status.ActiveConnections <- registry.OnlineCount
            status.MemoryUsageBytes <- Process.GetCurrentProcess().WorkingSet64
            return status
        }
