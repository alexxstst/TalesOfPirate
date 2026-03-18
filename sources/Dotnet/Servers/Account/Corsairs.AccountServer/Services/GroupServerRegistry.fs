namespace Corsairs.AccountServer.Services

open System
open System.Collections.Concurrent
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network

/// Запись о подключённом GroupServer.
[<AllowNullLiteral>]
type GroupServerEntry(name: string, channel: ChannelIO) =
    member _.Name = name
    member _.Channel = channel
    member val MemberCount = 0 with get, set

    override _.ToString() = $"GroupServer[{name}]"

/// Сессия онлайн-аккаунта.
/// Статусы в C++: ACCOUNT_OFFLINE=0, ACCOUNT_ONLINE=1, ACCOUNT_SAVING=2.
[<Struct; NoComparison>]
type AccountSession =
    { GroupServerName: string
      LoginTime: DateTimeOffset
      SavingStartedAt: DateTimeOffset voption }

/// In-memory реестр GroupServer'ов и онлайн-аккаунтов.
type GroupServerRegistry(logger: ILogger<GroupServerRegistry>) =
    let _servers = ConcurrentDictionary<string, GroupServerEntry>(StringComparer.OrdinalIgnoreCase)
    let _byChannel = ConcurrentDictionary<ChannelId, GroupServerEntry>()
    let _onlineAccounts = ConcurrentDictionary<int, AccountSession>()

    // ── GroupServer управление ──

    member _.AddServer(name: string, channel: ChannelIO) =
        let entry = GroupServerEntry(name, channel)
        _servers[name] <- entry
        _byChannel[channel.Id] <- entry
        logger.LogInformation("GroupServer зарегистрирован: {Name}", name)

    member _.RemoveServer(name: string) =
        match _servers.TryRemove(name) with
        | true, entry ->
            _byChannel.TryRemove(entry.Channel.Id) |> ignore
            logger.LogInformation("GroupServer удалён: {Name}", name)
        | _ -> ()

    member _.RemoveByChannel(channelId: ChannelId) =
        match _byChannel.TryRemove(channelId) with
        | true, entry ->
            _servers.TryRemove(entry.Name) |> ignore
            logger.LogInformation("GroupServer удалён при отключении: {Name}", entry.Name)
        | _ -> ()

    member _.FindByName(name: string) =
        match _servers.TryGetValue(name) with
        | true, entry -> ValueSome entry
        | _ -> ValueNone

    member _.FindByChannel(channelId: ChannelId) =
        match _byChannel.TryGetValue(channelId) with
        | true, entry -> ValueSome entry
        | _ -> ValueNone

    member _.ExistsByName(name: string) = _servers.ContainsKey(name)

    member _.GetAllServers() = _servers.Values |> Seq.toArray

    // ── Онлайн-аккаунты ──

    member _.AccountLogin(accountId: int, groupServerName: string) =
        _onlineAccounts[accountId] <-
            { GroupServerName = groupServerName
              LoginTime = DateTimeOffset.UtcNow
              SavingStartedAt = ValueNone }

    member _.AccountSetSaving(accountId: int) =
        match _onlineAccounts.TryGetValue(accountId) with
        | true, session ->
            _onlineAccounts[accountId] <- { session with SavingStartedAt = ValueSome DateTimeOffset.UtcNow }
        | _ -> ()

    member _.AccountLogout(accountId: int) =
        _onlineAccounts.TryRemove(accountId) |> ignore

    member _.TryGetSession(accountId: int) =
        match _onlineAccounts.TryGetValue(accountId) with
        | true, session -> ValueSome session
        | _ -> ValueNone

    member _.IsSavingExpired(accountId: int, savingTimeSeconds: int) =
        match _onlineAccounts.TryGetValue(accountId) with
        | true, { SavingStartedAt = ValueSome started } ->
            (DateTimeOffset.UtcNow - started).TotalSeconds >= float savingTimeSeconds
        | _ -> false

    member _.OnlineCount = _onlineAccounts.Count

    member _.ClearAllSessions() =
        let count = _onlineAccounts.Count
        _onlineAccounts.Clear()
        if count > 0 then
            logger.LogInformation("Очищено {Count} онлайн-сессий", count)
