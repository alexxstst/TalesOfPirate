namespace Corsairs.AccountServer.Handlers

open System
open System.Threading.Tasks
open Microsoft.EntityFrameworkCore
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Protocol.Routing
open Corsairs.Platform.Database

/// Обработчик CMD_PA_GMBANACCOUNT и CMD_PA_GMUNBANACCOUNT.
type BanHandler(db: AccountDbContext, logger: ILogger<BanHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_PA_GMBANACCOUNT

        member _.Handle(channel, packet) =
            ValueTask(task {
                let accountId = packet.ReadInt32()
                let reason = packet.ReadString()
                let durationMinutes = packet.ReadInt32()

                let! account = db.Accounts.FindAsync(accountId)
                if not (isNull (box account)) then
                    account.IsBanned <- true
                    account.BanReason <- reason
                    account.BanExpiresAt <-
                        if durationMinutes > 0 then Nullable(DateTimeOffset.UtcNow.AddMinutes(float durationMinutes))
                        else Nullable()
                    let! _ = db.SaveChangesAsync()
                    logger.LogInformation("Бан: ID:{Id}, причина: {Reason}", accountId, reason)
            })

/// Обработчик CMD_PA_GMUNBANACCOUNT.
type UnbanHandler(db: AccountDbContext, logger: ILogger<UnbanHandler>) =
    interface ICommandHandler with
        member _.CommandId = Commands.CMD_PA_GMUNBANACCOUNT

        member _.Handle(channel, packet) =
            ValueTask(task {
                let accountId = packet.ReadInt32()

                let! account = db.Accounts.FindAsync(accountId)
                if not (isNull (box account)) then
                    account.IsBanned <- false
                    account.BanReason <- null
                    account.BanExpiresAt <- Nullable()
                    let! _ = db.SaveChangesAsync()
                    logger.LogInformation("Разбан: ID:{Id}", accountId)
            })
