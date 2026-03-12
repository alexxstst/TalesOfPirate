namespace Corsairs.AccountServer.Grpc

open System
open System.Diagnostics
open System.Linq
open System.Threading.Tasks
open Grpc.Core
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities
open Corsairs.Platform.Grpc.Contracts.Account
open Corsairs.Platform.Grpc.Contracts.Common
open Corsairs.AccountServer.Services

/// gRPC-сервис администрирования аккаунтов.
type AccountGrpcService(
    scopeFactory: IServiceScopeFactory,
    logger: ILogger<AccountGrpcService>) =
    inherit AccountAdmin.AccountAdminBase()

    let startTime = DateTimeOffset.UtcNow

    let toAccountInfo (a: Account) =
        let info = AccountInfo()
        info.Id <- a.Id
        info.Username <- a.Username
        info.Email <- if isNull a.Email then "" else a.Email
        info.IsBanned <- a.IsBanned
        info.BanReason <- if isNull a.BanReason then "" else a.BanReason
        info.BanExpiresAtUnix <-
            if a.BanExpiresAt.HasValue then a.BanExpiresAt.Value.ToUnixTimeSeconds() else 0L
        info.GmLevel <- int a.GmLevel
        info.LoginStatus <- int a.LoginStatus
        info.Credit <- float a.Credit
        info.CreatedAtUnix <- a.CreatedAt.ToUnixTimeSeconds()
        info.LastLoginAtUnix <-
            if a.LastLoginAt.HasValue then a.LastLoginAt.Value.ToUnixTimeSeconds() else 0L
        info.LastLoginIp <- if isNull a.LastLoginIp then "" else a.LastLoginIp
        info.TotalPlaytimeSeconds <- a.TotalPlaytimeSeconds
        info

    override _.GetAccount(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account =
                match request.IdentifierCase with
                | GetAccountRequest.IdentifierOneofCase.AccountId ->
                    db.Accounts.FindAsync(request.AccountId).AsTask()
                | GetAccountRequest.IdentifierOneofCase.Username ->
                    db.Accounts.FirstOrDefaultAsync(fun a -> a.Username = request.Username)
                | _ ->
                    Task.FromResult<Account>(null)
            if isNull (box account) then
                raise (RpcException(Status(StatusCode.NotFound, "Аккаунт не найден")))
            return toAccountInfo account
        }

    override _.SearchAccounts(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let query =
                if String.IsNullOrEmpty(request.Query) then
                    db.Accounts.AsQueryable()
                else
                    db.Accounts.Where(fun a -> a.Username.Contains(request.Query))
            let page = max 1 request.Pagination.Page
            let pageSize = max 10 (min 100 request.Pagination.PageSize)
            let! total = query.CountAsync()
            let! accounts = query.OrderBy(fun a -> a.Id).Skip((page - 1) * pageSize).Take(pageSize).ToListAsync()
            let result = AccountList()
            for a in accounts do
                result.Accounts.Add(toAccountInfo a)
            let pag = PaginationInfo()
            pag.TotalCount <- total
            pag.Page <- page
            pag.PageSize <- pageSize
            pag.TotalPages <- (total + pageSize - 1) / pageSize
            result.Pagination <- pag
            return result
        }

    override _.BanAccount(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.IsBanned <- true
                account.BanReason <- request.Reason
                account.BanExpiresAt <-
                    if request.ExpiresAtUnix > 0L then
                        Nullable(DateTimeOffset.FromUnixTimeSeconds(request.ExpiresAtUnix))
                    else Nullable()
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Аккаунт заблокирован"
                logger.LogInformation("gRPC: Бан аккаунта {Id}", request.AccountId)
            return result
        }

    override _.UnbanAccount(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.IsBanned <- false
                account.BanReason <- null
                account.BanExpiresAt <- Nullable()
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Аккаунт разблокирован"
                logger.LogInformation("gRPC: Разбан аккаунта {Id}", request.AccountId)
            return result
        }

    override _.ResetPassword(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let auth = scope.ServiceProvider.GetRequiredService<AuthService>()
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.PasswordHash <- auth.HashPassword(request.NewPasswordHash)
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Пароль сброшен"
                logger.LogInformation("gRPC: Сброс пароля аккаунта {Id}", request.AccountId)
            return result
        }

    override _.SetGmLevel(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.GmLevel <- byte request.GmLevel
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "GM-уровень установлен"
            return result
        }

    override _.SetCredit(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.Credit <- decimal request.Credit
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Кредит установлен"
            return result
        }

    override _.GetServerStatus(request, context) =
        task {
            let status = ServerStatusResponse()
            status.ServerName <- "AccountServer"
            status.IsRunning <- true
            status.UptimeSeconds <- int64 (DateTimeOffset.UtcNow - startTime).TotalSeconds
            status.ActiveConnections <- 0
            status.MemoryUsageBytes <- Process.GetCurrentProcess().WorkingSet64
            return status
        }
