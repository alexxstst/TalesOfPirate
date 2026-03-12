namespace Corsairs.GroupServer.Services

open System.Threading.Tasks
open System.Linq
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

/// Сервис друзей.
type FriendService(db: GameDbContext, logger: ILogger<FriendService>) =

    /// Добавить друга.
    member _.AddFriend(chaId1: int, chaId2: int) : Task<Result<unit, string>> =
        task {
            let! exists = db.Friendships.AnyAsync(fun f ->
                (f.CharacterId1 = chaId1 && f.CharacterId2 = chaId2) ||
                (f.CharacterId1 = chaId2 && f.CharacterId2 = chaId1))
            if exists then return Error "Уже в списке друзей"
            else
                let friendship = Friendship(
                    CharacterId1 = min chaId1 chaId2,
                    CharacterId2 = max chaId1 chaId2,
                    RelationType = 0uy,
                    CreatedAt = System.DateTimeOffset.UtcNow)
                db.Friendships.Add(friendship) |> ignore
                let! _ = db.SaveChangesAsync() in ()
                return Ok ()
        }

    /// Удалить из друзей.
    member _.RemoveFriend(chaId1: int, chaId2: int) : Task<bool> =
        task {
            let! friendship = db.Friendships.FirstOrDefaultAsync(fun f ->
                (f.CharacterId1 = chaId1 && f.CharacterId2 = chaId2) ||
                (f.CharacterId1 = chaId2 && f.CharacterId2 = chaId1))
            if isNull (box friendship) then return false
            else
                db.Friendships.Remove(friendship) |> ignore
                let! _ = db.SaveChangesAsync() in ()
                return true
        }

    /// Получить список друзей.
    member _.GetFriends(chaId: int) : Task<Friendship[]> =
        task {
            return! db.Friendships
                        .Where(fun f -> f.CharacterId1 = chaId || f.CharacterId2 = chaId)
                        .ToArrayAsync()
        }
