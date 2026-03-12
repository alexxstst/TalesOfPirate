namespace Corsairs.Platform.Network.Network

open System
open System.Collections.Concurrent
open System.Threading

/// Менеджер соединений. Потокобезопасное хранилище PipelineConnection.
type ConnectionManager() =

    let _connections = ConcurrentDictionary<int, PipelineConnection>()
    let mutable _nextId = 0

    /// Сгенерировать уникальный ID соединения.
    member _.NextId() =
        Interlocked.Increment(&_nextId)

    /// Зарегистрировать соединение.
    member _.Add(conn: PipelineConnection) =
        _connections.TryAdd(conn.Id, conn) |> ignore

    /// Удалить соединение по ID.
    member _.Remove(connectionId: int) =
        match _connections.TryRemove(connectionId) with
        | true, conn -> Some conn
        | _ -> None

    /// Получить соединение по ID.
    member _.TryGet(connectionId: int) =
        match _connections.TryGetValue(connectionId) with
        | true, conn -> Some conn
        | _ -> None

    /// Количество активных соединений.
    member _.Count = _connections.Count

    /// Все активные соединения.
    member _.GetAll() =
        _connections.Values |> Seq.toArray

    /// Выполнить действие для каждого соединения.
    member _.ForEach(action: PipelineConnection -> unit) =
        for kvp in _connections do
            action kvp.Value

    /// Закрыть и удалить все соединения.
    member _.CloseAll() =
        for kvp in _connections do
            try (kvp.Value :> IDisposable).Dispose() with _ -> ()
        _connections.Clear()

    interface IDisposable with
        member this.Dispose() = this.CloseAll()
