module Corsairs.GateServer.Tests.RecoveryAggregateTests

open System
open Xunit
open Corsairs.GateServer.Domain
open Corsairs.GateServer.Services

let private testMap = MapName_ "garner"
let private forbiddenMap = MapName_ "unknown"
let private charId = CharacterId_ 1u
let private charId2 = CharacterId_ 2u
let private now = DateTimeOffset.UtcNow

let private mkPos map charId =
    { Character = charId; Map = map; X = 100u; Y = 200u }

let private mkState () = RecoveryAggregate.create [ testMap ]

let private addEntry (state: RecoveryState) charId =
    let pos = mkPos testMap charId

    let point =
        { CharacterPosition = pos
          SavedAt = DateTimeOffset.UtcNow }

    { state with Entries = Map.add charId point state.Entries }

// ── executeCommand: SavePosition ──

[<Fact>]
let ``SavePosition на разрешённой карте -> Ok [PositionSaved]`` () =
    let state = mkState ()
    let pos = mkPos testMap charId
    let result = RecoveryAggregate.executeCommand state (SavePosition { Position = pos; Timestamp = now })

    match result with
    | Ok [ PositionSaved e ] -> Assert.Equal(pos, e.Position)
    | other -> Assert.Fail $"Ожидался Ok [PositionSaved], получено: {other}"

[<Fact>]
let ``SavePosition на запрещённой карте -> Error MapNotAllowed`` () =
    let state = mkState ()
    let pos = mkPos forbiddenMap charId
    let result = RecoveryAggregate.executeCommand state (SavePosition { Position = pos; Timestamp = now })

    match result with
    | Error(MapNotAllowed(cid, map)) ->
        Assert.Equal(charId, cid)
        Assert.Equal(forbiddenMap, map)
    | other -> Assert.Fail $"Ожидался Error MapNotAllowed, получено: {other}"

[<Fact>]
let ``SavePosition для dropped игрока -> Error AlreadyDropped`` () =
    let state =
        { mkState () with DroppedByServer = Set.ofList [ charId ] }

    let pos = mkPos testMap charId
    let result = RecoveryAggregate.executeCommand state (SavePosition { Position = pos; Timestamp = now })

    match result with
    | Error(AlreadyDropped cid) -> Assert.Equal(charId, cid)
    | other -> Assert.Fail $"Ожидался Error AlreadyDropped, получено: {other}"

[<Fact>]
let ``SavePosition обновляет существующую запись`` () =
    let state = addEntry (mkState ()) charId
    let newPos = { mkPos testMap charId with X = 999u }
    let result = RecoveryAggregate.executeCommand state (SavePosition { Position = newPos; Timestamp = now })

    match result with
    | Ok [ PositionSaved e ] -> Assert.Equal(999u, e.Position.X)
    | other -> Assert.Fail $"Ожидался Ok [PositionSaved], получено: {other}"

// ── executeCommand: RestorePlayer ──

[<Fact>]
let ``RestorePlayer существующего -> Ok [PositionRestored]`` () =
    let state = addEntry (mkState ()) charId
    let result = RecoveryAggregate.executeCommand state (RestorePlayer { CharacterId = charId })

    match result with
    | Ok [ PositionRestored e ] -> Assert.Equal(charId, e.Point.CharacterPosition.Character)
    | other -> Assert.Fail $"Ожидался Ok [PositionRestored], получено: {other}"

[<Fact>]
let ``RestorePlayer несуществующего -> Error NotFound`` () =
    let state = mkState ()
    let result = RecoveryAggregate.executeCommand state (RestorePlayer { CharacterId = charId })

    match result with
    | Error(NotFound cid) -> Assert.Equal(charId, cid)
    | other -> Assert.Fail $"Ожидался Error NotFound, получено: {other}"

// ── executeCommand: DropByServer ──

[<Fact>]
let ``DropByServer существующего -> Ok [DroppedByServer]`` () =
    let state = addEntry (mkState ()) charId
    let result = RecoveryAggregate.executeCommand state (DropByServer { CharacterId = charId })

    match result with
    | Ok [ RecoveryEvent.DroppedByServer e ] -> Assert.Equal(charId, e.CharacterId)
    | other -> Assert.Fail $"Ожидался Ok [DroppedByServer], получено: {other}"

[<Fact>]
let ``DropByServer несуществующего -> Ok [MarkedAsDropped]`` () =
    let state = mkState ()
    let result = RecoveryAggregate.executeCommand state (DropByServer { CharacterId = charId })

    match result with
    | Ok [ MarkedAsDropped e ] -> Assert.Equal(charId, e.CharacterId)
    | other -> Assert.Fail $"Ожидался Ok [MarkedAsDropped], получено: {other}"

// ── executeCommand: ExpireTimedOut ──

[<Fact>]
let ``ExpireTimedOut удаляет просроченные`` () =
    let state = mkState ()

    let oldPoint =
        { CharacterPosition = mkPos testMap charId
          SavedAt = DateTimeOffset.UtcNow.AddMinutes(-10.0) }

    let state =
        { state with Entries = Map.add charId oldPoint state.Entries }

    let cutoff = DateTimeOffset.UtcNow.AddMinutes(-5.0)
    let result = RecoveryAggregate.executeCommand state (ExpireTimedOut { Cutoff = cutoff })

    match result with
    | Ok [ EntryExpired e ] -> Assert.Equal(charId, e.CharacterId)
    | other -> Assert.Fail $"Ожидался Ok [EntryExpired], получено: {other}"

[<Fact>]
let ``ExpireTimedOut не трогает свежие`` () =
    let state = addEntry (mkState ()) charId
    let cutoff = DateTimeOffset.UtcNow.AddMinutes(-5.0)
    let result = RecoveryAggregate.executeCommand state (ExpireTimedOut { Cutoff = cutoff })

    match result with
    | Ok [] -> ()
    | other -> Assert.Fail $"Ожидался Ok [], получено: {other}"

// ── apply ──

[<Fact>]
let ``apply PositionSaved добавляет в entries`` () =
    let state = mkState ()
    let pos = mkPos testMap charId
    let state' = RecoveryAggregate.apply state (PositionSaved { Position = pos; Timestamp = now })
    Assert.True(state'.Entries.ContainsKey(charId))

[<Fact>]
let ``apply PositionRestored удаляет из entries`` () =
    let state = addEntry (mkState ()) charId
    let point = state.Entries[charId]
    let state' = RecoveryAggregate.apply state (PositionRestored { Point = point })
    Assert.False(state'.Entries.ContainsKey(charId))

[<Fact>]
let ``apply DroppedByServer удаляет из entries`` () =
    let state = addEntry (mkState ()) charId
    let state' = RecoveryAggregate.apply state (RecoveryEvent.DroppedByServer { CharacterId = charId })
    Assert.False(state'.Entries.ContainsKey(charId))

[<Fact>]
let ``apply MarkedAsDropped добавляет в droppedByServer`` () =
    let state = mkState ()
    let state' = RecoveryAggregate.apply state (MarkedAsDropped { CharacterId = charId })
    Assert.True(state'.DroppedByServer.Contains(charId))
