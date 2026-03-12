namespace Corsairs.AccountServer.Services

open System
open System.Security.Cryptography
open System.Text
open System.Threading.Tasks
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities
open System.Linq

/// Результат аутентификации.
[<Struct>]
type AuthResult =
    | AuthSuccess of accountId: int * gmLevel: byte
    | AuthBanned of reason: string
    | AuthInvalidCredentials
    | AuthAccountNotFound
    | AuthAlreadyOnline
    | AuthError of error: string

/// Сервис аутентификации (Argon2id).
type AuthService(db: AccountDbContext, logger: ILogger<AuthService>) =

    /// Хешировать пароль (Argon2id).
    member _.HashPassword(password: string) =
        use argon2 = new Konscious.Security.Cryptography.Argon2id(Encoding.UTF8.GetBytes(password))
        let salt = RandomNumberGenerator.GetBytes(16)
        argon2.Salt <- salt
        argon2.DegreeOfParallelism <- 4
        argon2.MemorySize <- 65536
        argon2.Iterations <- 3
        let hash = argon2.GetBytes(32)
        $"$argon2id$v=19$m=65536,t=3,p=4${Convert.ToBase64String(salt)}${Convert.ToBase64String(hash)}"

    /// Проверить пароль против хэша.
    member _.VerifyPassword(password: string, storedHash: string) =
        try
            let parts = storedHash.Split('$', StringSplitOptions.RemoveEmptyEntries)
            if parts.Length < 5 || parts[0] <> "argon2id" then false
            else
                let paramParts = parts[2].Split(',')
                let mutable m = 65536
                let mutable t = 3
                let mutable p = 4
                for param in paramParts do
                    let kv = param.Split('=')
                    if kv.Length = 2 then
                        match kv[0] with
                        | "m" -> m <- int kv[1]
                        | "t" -> t <- int kv[1]
                        | "p" -> p <- int kv[1]
                        | _ -> ()
                let salt = Convert.FromBase64String(parts[3])
                let expectedHash = Convert.FromBase64String(parts[4])
                use argon2 = new Konscious.Security.Cryptography.Argon2id(Encoding.UTF8.GetBytes(password))
                argon2.Salt <- salt
                argon2.DegreeOfParallelism <- p
                argon2.MemorySize <- m
                argon2.Iterations <- t
                let computedHash = argon2.GetBytes(expectedHash.Length)
                CryptographicOperations.FixedTimeEquals(
                    ReadOnlySpan<byte>(computedHash),
                    ReadOnlySpan<byte>(expectedHash))
        with ex ->
            logger.LogError(ex, "Ошибка проверки пароля")
            false

    /// Аутентификация пользователя.
    member this.ValidateLogin(username: string, password: string) : Task<AuthResult> =
        task {
            try
                let! account = db.Accounts.FirstOrDefaultAsync(fun a -> a.Username = username)
                if isNull (box account) then
                    return AuthAccountNotFound
                elif account.IsBanned then
                    let banExpires = account.BanExpiresAt
                    if banExpires.HasValue && banExpires.Value > DateTimeOffset.UtcNow then
                        let expiresStr = banExpires.Value.ToString("yyyy-MM-dd HH:mm")
                        let reason = $"Заблокирован до {expiresStr}: {account.BanReason}"
                        return AuthBanned(reason)
                    elif banExpires.HasValue then
                        // Бан истёк — снимаем
                        account.IsBanned <- false
                        account.BanReason <- null
                        account.BanExpiresAt <- Nullable()
                        let! _ = db.SaveChangesAsync()
                        return AuthSuccess(account.Id, account.GmLevel)
                    else
                        let reason = $"Заблокирован навсегда: {account.BanReason}"
                        return AuthBanned(reason)
                elif account.LoginStatus > 0uy then
                    return AuthAlreadyOnline
                elif not (this.VerifyPassword(password, account.PasswordHash)) then
                    return AuthInvalidCredentials
                else
                    account.LoginStatus <- 1uy
                    account.LastLoginAt <- Nullable DateTimeOffset.UtcNow
                    account.SessionId <- Nullable (Guid.NewGuid())
                    let! _ = db.SaveChangesAsync()

                    let log = LoginLog(
                        AccountId = account.Id,
                        IpAddress = null,
                        LoginAt = DateTimeOffset.UtcNow)
                    db.LoginLogs.Add(log) |> ignore
                    let! _ = db.SaveChangesAsync()

                    logger.LogInformation("Вход: {Username} (ID:{Id})", username, account.Id)
                    return AuthSuccess(account.Id, account.GmLevel)
            with ex ->
                logger.LogError(ex, "Ошибка аутентификации {Username}", username)
                return AuthError(ex.Message)
        }

    /// Выход пользователя.
    member _.Logout(accountId: int) : Task<bool> =
        task {
            let! account = db.Accounts.FindAsync(accountId)
            if isNull (box account) then return false
            else
                account.LoginStatus <- 0uy
                account.SessionId <- Nullable()
                account.LastLogoutAt <- Nullable DateTimeOffset.UtcNow
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Выход: ID:{Id}", accountId)
                return true
        }

    /// Создать новый аккаунт.
    member this.Register(username: string, password: string) : Task<Result<int, string>> =
        task {
            let! exists = db.Accounts.AnyAsync(fun a -> a.Username = username)
            if exists then
                return Error "Имя пользователя уже занято"
            else
                let account = Account(
                    Username = username,
                    PasswordHash = this.HashPassword(password),
                    CreatedAt = DateTimeOffset.UtcNow)
                db.Accounts.Add(account) |> ignore
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Регистрация: {Username} (ID:{Id})", username, account.Id)
                return Ok account.Id
        }
