namespace Corsairs.GroupServer.Services

open System.Threading.Tasks
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Database
open Corsairs.Platform.Database.Entities

/// Сервис наставничества (мастер-ученик).
type MentorshipService(db: GameDbContext, logger: ILogger<MentorshipService>) =

    /// Создать связь наставничества.
    member _.CreateMentorship(masterId: int, prenticeId: int) : Task<Result<unit, string>> =
        task {
            let! exists = db.Mentorships.AnyAsync(fun m ->
                m.MasterCharacterId = masterId && m.PrenticeCharacterId = prenticeId && not m.IsFinished)
            if exists then return Error "Наставничество уже существует"
            else
                let mentorship = Mentorship(
                    MasterCharacterId = masterId,
                    PrenticeCharacterId = prenticeId,
                    CreatedAt = System.DateTimeOffset.UtcNow)
                db.Mentorships.Add(mentorship) |> ignore
                let! _ = db.SaveChangesAsync() in ()
                logger.LogInformation("Наставничество: мастер {Master} → ученик {Prentice}", masterId, prenticeId)
                return Ok ()
        }

    /// Завершить наставничество.
    member _.FinishMentorship(masterId: int, prenticeId: int) : Task<bool> =
        task {
            let! m = db.Mentorships.FirstOrDefaultAsync(fun m ->
                m.MasterCharacterId = masterId && m.PrenticeCharacterId = prenticeId && not m.IsFinished)
            if isNull (box m) then return false
            else
                m.IsFinished <- true
                let! _ = db.SaveChangesAsync() in ()
                return true
        }
