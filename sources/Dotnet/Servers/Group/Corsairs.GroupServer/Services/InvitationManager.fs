namespace Corsairs.GroupServer.Services

open System
open System.Collections.Concurrent
open System.Threading
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Domain

/// Менеджер приглашений с таймаутами (замена C++ RunCtrlMgr<Invitation>).
type InvitationManager(logger: ILogger<InvitationManager>) =

    // Ключ: (toGpAddr, тип приглашения) → список ожидающих
    let _pending = ConcurrentDictionary<struct(uint32 * InvitationType), Invitation list>()
    let _cleanupTimer = new Timer(
        (fun _ ->
            let now = DateTimeOffset.UtcNow
            for kv in _pending do
                let filtered = kv.Value |> List.filter (fun inv -> inv.ExpiresAt > now)
                if filtered.IsEmpty then
                    _pending.TryRemove(kv.Key) |> ignore
                else
                    _pending[kv.Key] <- filtered),
        null, 10000, 10000)

    /// Добавить приглашение.
    member _.Add(inv: Invitation) =
        let key = struct(inv.ToGpAddr, inv.Type)
        _pending.AddOrUpdate(key, [inv], fun _ existing -> inv :: existing) |> ignore
        logger.LogDebug("Приглашение добавлено: {Type} от gpAddr={From} к gpAddr={To}",
                        inv.Type, inv.FromGpAddr, inv.ToGpAddr)

    /// Попытаться принять первое приглашение указанного типа.
    member _.TryAccept(toGpAddr: uint32, invType: InvitationType) : Invitation option =
        let key = struct(toGpAddr, invType)
        let now = DateTimeOffset.UtcNow
        match _pending.TryGetValue(key) with
        | true, invitations ->
            match invitations |> List.tryFind (fun i -> i.ExpiresAt > now) with
            | Some inv ->
                let remaining = invitations |> List.filter (fun i -> not (Object.ReferenceEquals(i, inv)))
                if remaining.IsEmpty then _pending.TryRemove(key) |> ignore
                else _pending[key] <- remaining
                Some inv
            | None ->
                _pending.TryRemove(key) |> ignore
                None
        | _ -> None

    /// Попытаться принять приглашение от конкретного игрока.
    member _.TryAcceptFrom(toGpAddr: uint32, fromGpAddr: uint32, invType: InvitationType) : Invitation option =
        let key = struct(toGpAddr, invType)
        let now = DateTimeOffset.UtcNow
        match _pending.TryGetValue(key) with
        | true, invitations ->
            match invitations |> List.tryFind (fun i -> i.FromGpAddr = fromGpAddr && i.ExpiresAt > now) with
            | Some inv ->
                let remaining = invitations |> List.filter (fun i -> not (Object.ReferenceEquals(i, inv)))
                if remaining.IsEmpty then _pending.TryRemove(key) |> ignore
                else _pending[key] <- remaining
                Some inv
            | None -> None
        | _ -> None

    /// Отклонить приглашение.
    member _.TryRefuse(toGpAddr: uint32, invType: InvitationType) : Invitation option =
        let key = struct(toGpAddr, invType)
        match _pending.TryGetValue(key) with
        | true, invitations ->
            match invitations with
            | inv :: rest ->
                if rest.IsEmpty then _pending.TryRemove(key) |> ignore
                else _pending[key] <- rest
                Some inv
            | [] ->
                _pending.TryRemove(key) |> ignore
                None
        | _ -> None

    /// Есть ли ожидающее приглашение от конкретного игрока.
    member _.HasPendingFrom(fromGpAddr: uint32, invType: InvitationType) =
        _pending.Values
        |> Seq.exists (fun invs ->
            invs |> List.exists (fun i ->
                i.FromGpAddr = fromGpAddr && i.Type = invType && i.ExpiresAt > DateTimeOffset.UtcNow))

    /// Количество ожидающих приглашений для игрока.
    member _.PendingCountFor(toGpAddr: uint32, invType: InvitationType) =
        let key = struct(toGpAddr, invType)
        match _pending.TryGetValue(key) with
        | true, invs -> invs |> List.filter (fun i -> i.ExpiresAt > DateTimeOffset.UtcNow) |> List.length
        | _ -> 0

    /// Удалить все приглашения для игрока (при disconnect).
    member _.CancelAll(gpAddr: uint32) =
        // Удалить приглашения ОТ этого игрока
        for kv in _pending do
            let filtered = kv.Value |> List.filter (fun i -> i.FromGpAddr <> gpAddr)
            if filtered.IsEmpty then
                _pending.TryRemove(kv.Key) |> ignore
            else
                _pending[kv.Key] <- filtered
        // Удалить приглашения К этому игроку
        for invType in [FriendInvite; TeamInvite; MasterInvite] do
            let key = struct(gpAddr, invType)
            _pending.TryRemove(key) |> ignore

    interface IDisposable with
        member _.Dispose() = _cleanupTimer.Dispose()
