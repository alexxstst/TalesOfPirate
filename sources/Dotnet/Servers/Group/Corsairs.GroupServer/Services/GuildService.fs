namespace Corsairs.GroupServer.Services

open System
open System.Threading.Tasks
open System.Linq
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

/// Сервис управления гильдиями.
type GuildService(db: GameDbContext, logger: ILogger<GuildService>) =

    /// Создать гильдию.
    member _.CreateGuild(leaderChaId: int, name: string, motto: string) : Task<Result<Guild, string>> =
        task {
            let! exists = db.Guilds.AnyAsync(fun g -> g.Name = name)
            if exists then return Error "Название гильдии занято"
            else
                let guild = Guild(
                    Name = name,
                    Motto = (if String.IsNullOrEmpty(motto) then null else motto),
                    LeaderCharacterId = leaderChaId,
                    Level = 1,
                    MemberCount = 1s,
                    MaxMembers = 50s,
                    CreatedAt = DateTimeOffset.UtcNow)
                db.Guilds.Add(guild) |> ignore

                let! leader = db.Characters.FindAsync(leaderChaId)
                if not (isNull (box leader)) then
                    leader.GuildId <- Nullable guild.Id
                    leader.GuildRank <- 1uy // Лидер

                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Гильдия создана: {Name} (ID:{Id})", name, guild.Id)
                return Ok guild
        }

    /// Расформировать гильдию.
    member _.DisbandGuild(guildId: int) : Task<bool> =
        task {
            let! guild = db.Guilds.Include(fun g -> g.Members).FirstOrDefaultAsync(fun g -> g.Id = guildId)
            if isNull (box guild) then return false
            else
                for m in guild.Members do
                    m.GuildId <- Nullable()
                    m.GuildRank <- 0uy
                db.Guilds.Remove(guild) |> ignore
                let! _ = db.SaveChangesAsync()
                logger.LogInformation("Гильдия расформирована: {Name} (ID:{Id})", guild.Name, guildId)
                return true
        }

    /// Вступить в гильдию.
    member _.JoinGuild(chaId: int, guildId: int) : Task<Result<unit, string>> =
        task {
            let! guild = db.Guilds.FindAsync(guildId)
            if isNull (box guild) then return Error "Гильдия не найдена"
            elif guild.MemberCount >= guild.MaxMembers then return Error "Гильдия заполнена"
            else
                let! cha = db.Characters.FindAsync(chaId)
                if isNull (box cha) then return Error "Персонаж не найден"
                elif cha.GuildId.HasValue then return Error "Уже в гильдии"
                else
                    cha.GuildId <- Nullable guildId
                    cha.GuildRank <- 4uy // Новичок
                    guild.MemberCount <- guild.MemberCount + 1s
                    let! _ = db.SaveChangesAsync()
                    return Ok ()
        }

    /// Покинуть гильдию.
    member _.LeaveGuild(chaId: int) : Task<bool> =
        task {
            let! cha = db.Characters.FindAsync(chaId)
            if isNull (box cha) || not cha.GuildId.HasValue then return false
            else
                let guildId = cha.GuildId.Value
                let! guild = db.Guilds.FindAsync(guildId)
                cha.GuildId <- Nullable()
                cha.GuildRank <- 0uy
                if not (isNull (box guild)) then
                    guild.MemberCount <- guild.MemberCount - 1s
                let! _ = db.SaveChangesAsync()
                return true
        }
