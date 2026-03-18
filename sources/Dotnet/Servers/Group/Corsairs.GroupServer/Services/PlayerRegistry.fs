namespace Corsairs.GroupServer.Services

open System
open System.Collections.Concurrent
open System.Threading
open Microsoft.Extensions.Logging

/// Потокобезопасный реестр онлайн-игроков с 4 индексами.
type PlayerRegistry(logger: ILogger<PlayerRegistry>) =

    let _byGpAddr = ConcurrentDictionary<uint32, PlayerRecord>()
    let _byChaId = ConcurrentDictionary<int, PlayerRecord>()
    let _byChaName = ConcurrentDictionary<string, PlayerRecord>(StringComparer.OrdinalIgnoreCase)
    let _byActId = ConcurrentDictionary<uint32, PlayerRecord>()
    let mutable _nextGpAddr = 0u

    /// Выделить уникальный gpAddr.
    member _.AllocateGpAddr() =
        Interlocked.Increment(&_nextGpAddr) |> uint32

    /// Зарегистрировать игрока.
    member _.Register(gpAddr: uint32, record: PlayerRecord) =
        record.GpAddr <- gpAddr
        _byGpAddr[gpAddr] <- record
        _byActId[record.ActId] <- record
        logger.LogDebug("Игрок зарегистрирован: gpAddr={GpAddr} actId={ActId}", gpAddr, record.ActId)

    /// Удалить игрока из реестра.
    member this.Unregister(gpAddr: uint32) =
        match _byGpAddr.TryRemove(gpAddr) with
        | true, record ->
            _byActId.TryRemove(record.ActId) |> ignore
            this.ClearPlaying(gpAddr)
            logger.LogDebug("Игрок удалён из реестра: gpAddr={GpAddr}", gpAddr)
        | _ -> ()

    /// Обновить индексы при выборе персонажа.
    member _.UpdatePlaying(gpAddr: uint32, chaId: int, chaName: string) =
        match _byGpAddr.TryGetValue(gpAddr) with
        | true, record ->
            _byChaId[chaId] <- record
            _byChaName[chaName] <- record
        | _ -> ()

    /// Очистить индексы персонажа (EndPlay).
    member _.ClearPlaying(gpAddr: uint32) =
        match _byGpAddr.TryGetValue(gpAddr) with
        | true, record ->
            let chaId = record.CurrentChaId
            let chaName = record.CurrentChaName
            if chaId <> 0 then _byChaId.TryRemove(chaId) |> ignore
            if not (String.IsNullOrEmpty chaName) then _byChaName.TryRemove(chaName) |> ignore
        | _ -> ()

    member _.TryGetByGpAddr(gpAddr: uint32) =
        match _byGpAddr.TryGetValue(gpAddr) with
        | true, v -> Some v
        | _ -> None

    member _.TryGetByChaId(chaId: int) =
        match _byChaId.TryGetValue(chaId) with
        | true, v -> Some v
        | _ -> None

    member _.TryGetByChaName(name: string) =
        match _byChaName.TryGetValue(name) with
        | true, v -> Some v
        | _ -> None

    member _.TryGetByActId(actId: uint32) =
        match _byActId.TryGetValue(actId) with
        | true, v -> Some v
        | _ -> None

    member _.GetAllPlayers() = _byGpAddr.Values :> PlayerRecord seq

    member _.OnlineCount = _byGpAddr.Count

    interface IPlayerRegistry with
        member this.Register(gpAddr, record) = this.Register(gpAddr, record)
        member this.Unregister(gpAddr) = this.Unregister(gpAddr)
        member this.UpdatePlaying(gpAddr, chaId, chaName) = this.UpdatePlaying(gpAddr, chaId, chaName)
        member this.ClearPlaying(gpAddr) = this.ClearPlaying(gpAddr)
        member this.TryGetByGpAddr(gpAddr) = this.TryGetByGpAddr(gpAddr)
        member this.TryGetByChaId(chaId) = this.TryGetByChaId(chaId)
        member this.TryGetByChaName(name) = this.TryGetByChaName(name)
        member this.TryGetByActId(actId) = this.TryGetByActId(actId)
        member this.GetAllPlayers() = this.GetAllPlayers()
        member this.OnlineCount = this.OnlineCount
