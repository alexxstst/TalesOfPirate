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
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol

/// gRPC-сервис администрирования GroupServer.
type GroupGrpcService(
    registry: PlayerRegistry,
    gateSystem: IGateServerSystem,
    scopeFactory: IServiceScopeFactory,
    logger: ILogger<GroupGrpcService>) =
    inherit GroupAdmin.GroupAdminBase()

    let startTime = DateTimeOffset.UtcNow

    override _.GetOnlinePlayers(request, _context) =
        task {
            let allPlayers = registry.GetAllPlayers() |> Seq.toArray
            let filtered =
                if String.IsNullOrEmpty(request.NameFilter) then allPlayers
                else
                    allPlayers |> Array.filter (fun p ->
                        let name = p.CurrentChaName
                        not (String.IsNullOrEmpty name)
                        && name.Contains(request.NameFilter, StringComparison.OrdinalIgnoreCase))
            let page = max 1 request.Pagination.Page
            let pageSize = max 10 (min 100 request.Pagination.PageSize)
            let total = filtered.Length
            let paged = filtered |> Array.skip (min ((page - 1) * pageSize) total) |> Array.truncate pageSize

            let result = PlayerList()
            for p in paged do
                let info = PlayerInfo()
                info.AccountId <- int p.ActId
                info.CharacterId <- p.CurrentChaId
                info.CharacterName <- defaultArg (Option.ofObj p.CurrentChaName) ""
                result.Players.Add(info)

            let pag = PaginationInfo()
            pag.TotalCount <- total
            pag.Page <- page
            pag.PageSize <- pageSize
            pag.TotalPages <- (total + pageSize - 1) / pageSize
            result.Pagination <- pag
            return result
        }

    override _.GetPlayerInfo(request, _context) =
        task {
            let player =
                match request.IdentifierCase with
                | GetPlayerInfoRequest.IdentifierOneofCase.CharacterId ->
                    registry.TryGetByChaId(request.CharacterId)
                | GetPlayerInfoRequest.IdentifierOneofCase.CharacterName ->
                    registry.TryGetByChaName(request.CharacterName)
                | _ -> None
            match player with
            | None ->
                raise (RpcException(Status(StatusCode.NotFound, "Игрок не найден")))
                return Unchecked.defaultof<_>
            | Some p ->
                let info = PlayerInfo()
                info.AccountId <- int p.ActId
                info.CharacterId <- p.CurrentChaId
                info.CharacterName <- defaultArg (Option.ofObj p.CurrentChaName) ""
                return info
        }

    override _.KickPlayer(request, _context) =
        task {
            let result = OperationResult()
            match registry.TryGetByChaId(request.CharacterId) with
            | Some player ->
                gateSystem.KickUser(player.GpAddr, player.GateAddr)
                registry.Unregister(player.GpAddr)
                result.Success <- true
                result.Message <- $"Игрок {player.CurrentChaName} отключён"
                logger.LogInformation("gRPC: Kick игрока {Name} (ID:{Id})", player.CurrentChaName, request.CharacterId)
            | None ->
                result.Success <- false
                result.Message <- "Игрок не найден"
            return result
        }

    override _.GetGuilds(request, _context) =
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

    override _.DisbandGuild(request, _context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<GameDbContext>()
            let! guild = db.Guilds.FindAsync(request.GuildId)
            let result = OperationResult()
            if isNull (box guild) then
                result.Success <- false
                result.Message <- "Гильдия не найдена"
            else
                db.Guilds.Remove(guild) |> ignore
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Гильдия расформирована"
                logger.LogInformation("gRPC: Расформирована гильдия {Id}", request.GuildId)
            return result
        }

    override _.SendGlobalMessage(request, _context) =
        task {
            let result = OperationResult()
            logger.LogInformation("gRPC: Глобальное сообщение — {Message}", request.Message)

            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_PC_GM1SAY1)
            w.WriteString(request.Message)
            gateSystem.SendToAllClients(w)

            result.Success <- true
            result.Message <- "Сообщение отправлено"
            return result
        }

    override _.MutePlayer(request, _context) =
        task {
            let result = OperationResult()
            match registry.TryGetByChaId(request.CharacterId) with
            | Some player ->
                logger.LogInformation("gRPC: Мут игрока {Name} на {Sec}с", player.CurrentChaName, request.DurationSeconds)
                let gate = player.Gate
                if not (isNull gate) && gate.IsRegistered then
                    if request.DurationSeconds > 0 then
                        let mutable w = WPacket(32)
                        w.WriteCmd(Commands.CMD_PT_ESTOPUSER)
                        w.WriteInt64(int64 player.GpAddr)
                        w.WriteInt64(int64 player.GateAddr)
                        w.WriteInt64(int64 request.DurationSeconds)
                        w.WriteInt64(1L)
                        gate.SendPacket(w)
                    else
                        let mutable w = WPacket(32)
                        w.WriteCmd(Commands.CMD_PT_DEL_ESTOPUSER)
                        w.WriteInt64(int64 player.GpAddr)
                        w.WriteInt64(int64 player.GateAddr)
                        w.WriteInt64(1L)
                        gate.SendPacket(w)
                result.Success <- true
                result.Message <- $"Игрок {player.CurrentChaName} замучен на {request.DurationSeconds}с"
            | None ->
                result.Success <- false
                result.Message <- "Игрок не найден"
            return result
        }

    override _.GetServerTopology(request, _context) =
        task {
            let topo = TopologyResponse()
            topo.TotalOnline <- registry.OnlineCount
            return topo
        }

    override _.GetServerStatus(request, _context) =
        task {
            let status = ServerStatusResponse()
            status.ServerName <- "GroupServer"
            status.IsRunning <- true
            status.UptimeSeconds <- int64 (DateTimeOffset.UtcNow - startTime).TotalSeconds
            status.ActiveConnections <- registry.OnlineCount
            status.MemoryUsageBytes <- Process.GetCurrentProcess().WorkingSet64
            return status
        }
