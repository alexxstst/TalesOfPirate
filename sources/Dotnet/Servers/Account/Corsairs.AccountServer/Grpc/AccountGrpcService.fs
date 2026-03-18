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
    auth: AuthService,
    logger: ILogger<AccountGrpcService>) =
    inherit AccountAdmin.AccountAdminBase()

    let startTime = DateTimeOffset.UtcNow

    let toAccountInfo (a: Account) =
        let info = AccountInfo()
        info.Id <- a.Id
        info.Username <- a.Username
        info.Email <- if isNull a.Email then "" else a.Email
        info.IsBanned <- a.Ban.GetValueOrDefault(false)
        info.BanReason <- "" // legacy-схема не хранит причину бана
        info.BanExpiresAtUnix <-
            if a.EnableLoginTime.HasValue then DateTimeOffset(a.EnableLoginTime.Value, TimeSpan.Zero).ToUnixTimeSeconds()
            else 0L
        info.GmLevel <- 0 // GmLevel нет в account_login, хранится на уровне персонажей
        info.LoginStatus <- a.LoginStatus
        info.Credit <-
            if isNull (box a.Details) then 0.0
            else a.Details.Credit
        info.CreatedAtUnix <-
            if isNull (box a.Details) then 0L
            else DateTimeOffset(a.Details.CreateTime, TimeSpan.Zero).ToUnixTimeSeconds()
        info.LastLoginAtUnix <-
            if a.LastLoginTime.HasValue then DateTimeOffset(a.LastLoginTime.Value, TimeSpan.Zero).ToUnixTimeSeconds()
            else 0L
        info.LastLoginIp <- if isNull a.LastLoginIp then "" else a.LastLoginIp
        info.TotalPlaytimeSeconds <- a.TotalLiveTime
        info

    override _.GetAccount(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account =
                match request.IdentifierCase with
                | GetAccountRequest.IdentifierOneofCase.AccountId ->
                    db.Accounts.Include(fun a -> a.Details).FirstOrDefaultAsync(fun a -> a.Id = request.AccountId)
                | GetAccountRequest.IdentifierOneofCase.Username ->
                    db.Accounts.Include(fun a -> a.Details).FirstOrDefaultAsync(fun a -> a.Username = request.Username)
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
                    db.Accounts.Include(fun a -> a.Details).AsQueryable()
                else
                    db.Accounts.Include(fun a -> a.Details).Where(fun a -> a.Username.Contains(request.Query))
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
                account.Ban <- Nullable true
                account.EnableLoginTime <-
                    if request.ExpiresAtUnix > 0L then
                        Nullable(DateTimeOffset.FromUnixTimeSeconds(request.ExpiresAtUnix).UtcDateTime)
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
                account.Ban <- Nullable false
                account.EnableLoginTime <- Nullable()
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
            let! account = db.Accounts.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box account) then
                result.Success <- false
                result.Message <- "Аккаунт не найден"
            else
                account.PasswordHash <- request.NewPasswordHash
                let! _ = db.SaveChangesAsync()
                result.Success <- true
                result.Message <- "Пароль сброшен"
                logger.LogInformation("gRPC: Сброс пароля аккаунта {Id}", request.AccountId)
            return result
        }

    override _.SetGmLevel(request, context) =
        task {
            // GmLevel не хранится в account_login legacy-схемы
            let result = OperationResult()
            result.Success <- false
            result.Message <- "GM-уровень хранится на уровне персонажей, не аккаунтов"
            return result
        }

    override _.SetCredit(request, context) =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! details = db.AccountDetails.FindAsync(request.AccountId)
            let result = OperationResult()
            if isNull (box details) then
                result.Success <- false
                result.Message <- "Детали аккаунта не найдены"
            else
                details.Credit <- request.Credit
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
