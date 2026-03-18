namespace Corsairs.AccountServer.Services

open System
open System.Linq
open System.Security.Cryptography
open System.Text
open System.Threading.Tasks
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

/// Результат аутентификации.
/// Коды совпадают с C++ AccountServer:
///   0 = error, 1 = success, 2 = banned, 3 = bad password,
///   4 = not found, 5 = already online, 6 = saving, 7 = disabled
[<Struct>]
type AuthResult =
    | AuthSuccess of accountId: int
    | AuthBanned of reason: string
    | AuthInvalidCredentials
    | AuthAccountNotFound
    | AuthAlreadyOnline of onlineAccountId: int * loginStatus: int
    | AuthSaving of savingAccountId: int
    | AuthDisabled
    | AuthError of error: string

/// Сервис аутентификации.
/// Использует IServiceScopeFactory вместо прямого DbContext (singleton-safe).
type AuthService(scopeFactory: IServiceScopeFactory, logger: ILogger<AuthService>) =

    /// Проверить пароль (plaintext, как в legacy C++).
    /// Legacy-схема хранит password + salt в varchar(255).
    static member VerifyPasswordLegacy(password: string, storedPassword: string, _salt: string option) =
        // Legacy C++: plaintext comparison (password = stored password)
        String.Equals(password, storedPassword, StringComparison.Ordinal)

    /// Только проверка учётных данных (не меняет login_status).
    /// C++: AuthThread::QueryAccount() + начало AccountLogin().
    member _.ValidateLogin(username: string, password: string) : Task<AuthResult> =
        task {
            try
                use scope = scopeFactory.CreateScope()
                let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
                let! account = db.Accounts.FirstOrDefaultAsync(fun a -> a.Username = username)
                if isNull (box account) then
                    return AuthAccountNotFound
                elif account.Ban.GetValueOrDefault(false) then
                    // Проверяем enable_login_time: если в будущем — бан активен
                    if account.EnableLoginTime.HasValue && account.EnableLoginTime.Value > DateTime.UtcNow then
                        let reason = "Заблокирован до " + account.EnableLoginTime.Value.ToString("yyyy-MM-dd HH:mm")
                        return AuthBanned(reason)
                    elif account.EnableLoginTime.HasValue then
                        // Бан истёк — снимаем
                        account.Ban <- Nullable false
                        let! _ = db.SaveChangesAsync()
                        if not (AuthService.VerifyPasswordLegacy(password, account.PasswordHash, Option.ofObj account.Salt)) then
                            return AuthInvalidCredentials
                        else
                            return AuthSuccess(account.Id)
                    else
                        return AuthBanned("Заблокирован навсегда")
                elif not (AuthService.VerifyPasswordLegacy(password, account.PasswordHash, Option.ofObj account.Salt)) then
                    return AuthInvalidCredentials
                else
                    // Проверяем login_status но НЕ меняем его
                    // C++: ACCOUNT_ONLINE=1, ACCOUNT_SAVING=2
                    match account.LoginStatus with
                    | 0 -> return AuthSuccess(account.Id)
                    | 2 -> return AuthSaving(account.Id)
                    | status -> return AuthAlreadyOnline(account.Id, status)
            with ex ->
                logger.LogError(ex, "Ошибка аутентификации {Username}", username)
                return AuthError(ex.Message)
        }

    /// Установить аккаунт в статус ONLINE.
    /// C++: UPDATE account_login SET login_status=1, login_group='...', enable_login_time=getdate(), last_login_time=getdate(), last_login_mac=..., last_login_ip=...
    member _.SetOnline(accountId: int, loginGroup: string, clientIp: string) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(accountId)
            if isNull (box account) then return false
            else
                account.LoginStatus <- 1
                account.LoginGroup <- loginGroup
                account.LastLoginTime <- Nullable DateTime.UtcNow
                account.LastLoginIp <- clientIp
                account.EnableLoginTime <- Nullable DateTime.UtcNow
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Вход: ID:{Id} на {Group}", accountId, loginGroup)
                return true
        }

    /// Установить аккаунт в статус SAVING.
    /// C++: UPDATE account_login SET login_status=2, enable_login_time=dateadd(s, 15, getdate())
    member _.SetSaving(accountId: int) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(accountId)
            if isNull (box account) then return false
            else
                account.LoginStatus <- 2
                account.EnableLoginTime <- Nullable(DateTime.UtcNow.AddSeconds(15.0))
                let! _ = db.SaveChangesAsync()
                return true
        }

    /// Логаут с обновлением playtime.
    /// C++: total_live_time += datediff(second, last_login_time, getdate()), login_status=0
    member _.LogoutWithPlaytime(accountId: int) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(accountId)
            if isNull (box account) then return false
            else
                // Подсчёт времени сессии
                if account.LastLoginTime.HasValue then
                    let sessionSeconds = int64 (DateTime.UtcNow - account.LastLoginTime.Value).TotalSeconds
                    account.TotalLiveTime <- account.TotalLiveTime + sessionSeconds

                account.LoginStatus <- 0
                account.LoginGroup <- null
                account.LastLogoutTime <- Nullable DateTime.UtcNow
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Выход: ID:{Id}", accountId)
                return true
        }

    /// Сменить пароль.
    /// C++: UPDATE account_login SET password='newpass' WHERE name='username'
    member _.ChangePassword(username: string, newPassword: string) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FirstOrDefaultAsync(fun a -> a.Username = username)
            if isNull (box account) then return false
            else
                account.PasswordHash <- newPassword
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Пароль изменён: {Username}", username)
                return true
        }

    /// Заблокировать/разблокировать аккаунт по ID.
    member _.DisableAccount(accountId: int, disabled: bool) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FindAsync(accountId)
            if isNull (box account) then return false
            else
                account.Ban <- Nullable disabled
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("{Action}: ID:{Id}", (if disabled then "Бан" else "Разбан"), accountId)
                return true
        }

    /// Заблокировать/разблокировать аккаунт по имени.
    /// C++: OperAccountBan — UPDATE account_login SET ban=iban WHERE name='actname'
    member _.DisableAccountByName(username: string, disabled: bool) : Task<bool> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! account = db.Accounts.FirstOrDefaultAsync(fun a -> a.Username = username)
            if isNull (box account) then return false
            else
                account.Ban <- Nullable disabled
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("{Action}: {Username}", (if disabled then "Бан" else "Разбан"), username)
                return true
        }

    /// Записать лог входа.
    /// C++: INSERT INTO user_log (user_id, user_name, login_time, login_ip) VALUES (...)
    member _.CreateLoginLog(accountId: int, username: string, ipAddress: string) : Task =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let log = LoginLog(
                AccountId = accountId,
                UserName = username,
                LoginTime = Nullable DateTime.UtcNow,
                LoginIp = ipAddress)
            db.LoginLogs.Add(log) |> ignore
            let! _ = db.SaveChangesAsync()
            ()
        } :> Task

    /// Сбросить все online-статусы при старте.
    /// C++: при старте AccountServer сбрасывает все login_status != 0.
    member _.ResetAllOnline() : Task<int> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! count = db.Accounts.Where(fun a -> a.LoginStatus <> 0).ExecuteUpdateAsync(fun s ->
                s.SetProperty((fun a -> a.LoginStatus), 0)
                 .SetProperty((fun (a: Account) -> a.LoginGroup), (null: string)) |> ignore)
            if count > 0 then
                logger.LogInformation("Сброшено {Count} online-статусов при старте", count)
            return count
        }

    /// Создать новый аккаунт.
    /// C++: InsertUser — INSERT INTO account_login(name, password, sid, total_live_time, email) VALUES(...)
    member _.Register(username: string, password: string) : Task<Result<int, string>> =
        task {
            use scope = scopeFactory.CreateScope()
            let db = scope.ServiceProvider.GetRequiredService<AccountDbContext>()
            let! exists = db.Accounts.AnyAsync(fun a -> a.Username = username)
            if exists then
                return Error "Имя пользователя уже занято"
            else
                let account = Account(
                    Username = username,
                    PasswordHash = password)
                db.Accounts.Add(account) |> ignore
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Регистрация: {Username} (ID:{Id})", username, account.Id)
                return Ok account.Id
        }
