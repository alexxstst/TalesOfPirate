namespace Corsairs.GroupServer.Services

open System.Threading.Tasks
open System.Linq
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

/// Сервис работы с персонажами (через EF).
type CharacterService(db: GameDbContext, logger: ILogger<CharacterService>) =

    /// Получить список персонажей аккаунта.
    member _.GetCharactersByAccount(accountId: int) : Task<Character[]> =
        task {
            return! db.Characters
                        .Where(fun c -> c.AccountId = accountId)
                        .IgnoreQueryFilters()
                        .Where(fun c -> not c.IsDeleted)
                        .ToArrayAsync()
        }

    /// Получить персонажа по ID.
    member _.GetCharacter(chaId: int) : Task<Character> =
        db.Characters.FindAsync(chaId).AsTask()

    /// Создать нового персонажа.
    member _.CreateCharacter(accountId: int, name: string, job: string, lookData: string) : Task<Result<Character, string>> =
        task {
            let! exists = db.Characters.AnyAsync(fun c -> c.Name = name)
            if exists then
                return Error "Имя персонажа уже занято"
            else
                let! count = db.Characters.CountAsync(fun c -> c.AccountId = accountId)
                if count >= 4 then
                    return Error "Максимум 4 персонажа"
                else
                    let cha = Character(
                        AccountId = accountId,
                        Name = name,
                        Job = job,
                        LookData = lookData,
                        Level = 1s,
                        Hp = 100,
                        MaxHp = 100,
                        Sp = 50,
                        MaxSp = 50,
                        MapName = "garner",
                        MapX = 2150,
                        MapY = 2800,
                        CreatedAt = System.DateTimeOffset.UtcNow,
                        LastLoginAt = System.DateTimeOffset.UtcNow)
                    db.Characters.Add(cha) |> ignore
                    let! _ = db.SaveChangesAsync()
                    logger.LogInformation("Персонаж создан: {Name} (ID:{Id})", name, cha.Id)
                    return Ok cha
        }

    /// Удалить персонажа (soft-delete).
    member _.DeleteCharacter(chaId: int, accountId: int) : Task<bool> =
        task {
            let! cha = db.Characters.FindAsync(chaId)
            if isNull (box cha) || cha.AccountId <> accountId then
                return false
            else
                cha.IsDeleted <- true
                cha.DeletedAt <- System.Nullable System.DateTimeOffset.UtcNow
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Персонаж удалён: {Name} (ID:{Id})", cha.Name, cha.Id)
                return true
        }
