module Corsairs.Platform.Network.Tests.OperationPoolTests

open System
open Microsoft.Extensions.Logging.Abstractions
open Xunit
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Network

let private makePool () =
    let rangedPool = RangedPool()
    OperationPool(rangedPool, NullLogger<OperationPool>.Instance), rangedPool

// ═══════════════════════════════════════════════════════════════
//  Счётчики пула
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``Новый пул: все счётчики = 0`` () =
    let pool, _ = makePool ()
    Assert.Equal(0, pool.TotalCount)
    Assert.Equal(0, pool.UsedCount)
    Assert.Equal(0, pool.FreeCount)

[<Fact>]
let ``AllocateForReceive увеличивает TotalCount и UsedCount`` () =
    let pool, _ = makePool ()
    let _op = pool.AllocateForReceive(ChannelId_ 1u)
    Assert.Equal(1, pool.TotalCount)
    Assert.Equal(1, pool.UsedCount)
    Assert.Equal(0, pool.FreeCount)

[<Fact>]
let ``Free возвращает операцию в пул`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    pool.Free(op)
    Assert.Equal(1, pool.TotalCount)
    Assert.Equal(0, pool.UsedCount)
    Assert.Equal(1, pool.FreeCount)

[<Fact>]
let ``Повторная аллокация переиспользует объект из пула`` () =
    let pool, _ = makePool ()
    let op1 = pool.AllocateForReceive(ChannelId_ 1u)
    let id1 = op1.ObjectId
    pool.Free(op1)
    let op2 = pool.AllocateForReceive(ChannelId_ 2u)
    Assert.Equal(id1, op2.ObjectId)
    Assert.Equal(1, pool.TotalCount)

[<Fact>]
let ``Несколько аллокаций без Free создают новые объекты`` () =
    let pool, _ = makePool ()
    let op1 = pool.AllocateForReceive(ChannelId_ 1u)
    let op2 = pool.AllocateForReceive(ChannelId_ 2u)
    Assert.NotEqual(op1.ObjectId, op2.ObjectId)
    Assert.Equal(2, pool.TotalCount)
    Assert.Equal(2, pool.UsedCount)

// ═══════════════════════════════════════════════════════════════
//  AllocateForReceive — состояние операции
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``AllocateForReceive: операция в состоянии Receiving`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 42u)
    match op.State with
    | Receiving ctx ->
        Assert.Equal(ChannelId_ 42u, ctx.ChannelId)
        Assert.Equal(0, ctx.DataLastIndex)
        Assert.False(ctx.Buffer.IsEmpty)
    | other -> Assert.Fail($"Ожидался Receiving, получен: {other}")

[<Fact>]
let ``AllocateForReceive: SessionId = 1 при первом назначении`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    match op.State with
    | Receiving ctx ->
        Assert.Equal(1UL, ctx.SessionId)
    | _ -> Assert.Fail("Ожидался Receiving")

[<Fact>]
let ``AllocateForReceive: SessionId сбрасывается после Free`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    pool.Free(op)
    let op2 = pool.AllocateForReceive(ChannelId_ 1u)
    match op2.State with
    | Receiving ctx ->
        Assert.Equal(1UL, ctx.SessionId)
    | _ -> Assert.Fail("Ожидался Receiving")

// ═══════════════════════════════════════════════════════════════
//  AllocateForSend — состояние операции
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``AllocateForSend: операция в состоянии Sending`` () =
    let pool, rangedPool = makePool ()
    let buf = rangedPool.Allocate(100)
    let op = pool.AllocateForSend(ChannelId_ 7u, buf, 50, true)
    match op.State with
    | Sending ctx ->
        Assert.Equal(ChannelId_ 7u, ctx.ChannelId)
        match ctx.Buffer with
        | PoolSendBuffer _ -> ()
        | RawSendBuffer -> Assert.Fail("Ожидался PoolSendBuffer")
    | other -> Assert.Fail($"Ожидался Sending(PoolSendBuffer), получен: {other}")

[<Fact>]
let ``AllocateForSend needRelease=false: RawSendBuffer`` () =
    let pool, rangedPool = makePool ()
    let buf = rangedPool.Allocate(100)
    let op = pool.AllocateForSend(ChannelId_ 1u, buf, 50, false)
    match op.State with
    | Sending ctx ->
        match ctx.Buffer with
        | RawSendBuffer -> ()
        | PoolSendBuffer _ -> Assert.Fail("Ожидался RawSendBuffer")
    | other -> Assert.Fail($"Ожидался Sending(RawSendBuffer), получен: {other}")
    // buf не освобождается автоматически, чистим вручную
    rangedPool.Free(buf)

// ═══════════════════════════════════════════════════════════════
//  AllocateForRawSend — состояние операции
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``AllocateForRawSend: операция в состоянии Sending(RawSendBuffer)`` () =
    let pool, _ = makePool ()
    let bytes = [| 0uy; 2uy |]
    let op = pool.AllocateForRawSend(ChannelId_ 99u, bytes)
    match op.State with
    | Sending ctx ->
        Assert.Equal(ChannelId_ 99u, ctx.ChannelId)
        match ctx.Buffer with
        | RawSendBuffer -> ()
        | PoolSendBuffer _ -> Assert.Fail("Ожидался RawSendBuffer")
    | other -> Assert.Fail($"Ожидался Sending(RawSendBuffer), получен: {other}")

// ═══════════════════════════════════════════════════════════════
//  Free — освобождение ресурсов
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``Free переводит операцию в состояние Free`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    pool.Free(op)
    Assert.Equal(Free, op.State)

[<Fact>]
let ``Free освобождает PoolBuffer в RangedPool`` () =
    let rangedPool = RangedPool()
    let pool = OperationPool(rangedPool, NullLogger<OperationPool>.Instance)
    let usedBefore = rangedPool.UsedCount
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    let usedAfterAlloc = rangedPool.UsedCount
    Assert.True(usedAfterAlloc > usedBefore)
    pool.Free(op)
    Assert.Equal(usedBefore, rangedPool.UsedCount)

// ═══════════════════════════════════════════════════════════════
//  SetContinuation
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``SetContinuation устанавливает callback в контексте`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForRawSend(ChannelId_ 1u, [| 0uy |])
    let mutable called = false
    op.SetContinuation(fun _ -> called <- true)
    match op.State with
    | Sending ctx ->
        Assert.True(ctx.Continuation.IsSome)
        ctx.Continuation.Value SendOk
        Assert.True(called)
    | _ -> Assert.Fail("Ожидался Sending")

[<Fact>]
let ``SetContinuation на Free операции кидает invalidOp`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    pool.Free(op)
    Assert.Throws<InvalidOperationException>(fun () ->
        op.SetContinuation(fun _ -> ())) |> ignore

[<Fact>]
let ``SetContinuation на Receiving операции кидает invalidOp`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    Assert.Throws<InvalidOperationException>(fun () ->
        op.SetContinuation(fun _ -> ())) |> ignore

// ═══════════════════════════════════════════════════════════════
//  UpdateDataIndex
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``UpdateDataIndex обновляет DataLastIndex в state`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    op.UpdateDataIndex(42)
    match op.State with
    | Receiving ctx -> Assert.Equal(42, ctx.DataLastIndex)
    | _ -> Assert.Fail("Ожидался Receiving")

[<Fact>]
let ``UpdateDataIndex на Free кидает invalidOp`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    pool.Free(op)
    Assert.Throws<InvalidOperationException>(fun () ->
        op.UpdateDataIndex(10)) |> ignore

[<Fact>]
let ``UpdateDataIndex на Sending кидает invalidOp`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForRawSend(ChannelId_ 1u, [| 0uy |])
    Assert.Throws<InvalidOperationException>(fun () ->
        op.UpdateDataIndex(10)) |> ignore

// ═══════════════════════════════════════════════════════════════
//  ToString
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``ToString содержит ObjectId в состоянии Free`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 1u)
    let id = op.ObjectId
    pool.Free(op)
    let s = op.ToString()
    Assert.Contains($"#{id}", s)
    Assert.Contains("[free]", s)

[<Fact>]
let ``ToString содержит ObjectId и ChannelId в состоянии Receiving`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 55u)
    let s = op.ToString()
    Assert.Contains($"#{op.ObjectId}", s)
    Assert.Contains("ch:55", s)
    Assert.Contains("recv", s)

[<Fact>]
let ``ToString содержит ObjectId и ChannelId в состоянии Sending`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForRawSend(ChannelId_ 77u, [| 0uy |])
    let s = op.ToString()
    Assert.Contains($"#{op.ObjectId}", s)
    Assert.Contains("ch:77", s)
    Assert.Contains("send", s)

// ═══════════════════════════════════════════════════════════════
//  Переиспользование — чистота состояния
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``Переиспользованная операция имеет чистый контекст`` () =
    let pool, _ = makePool ()
    let op = pool.AllocateForReceive(ChannelId_ 10u)
    op.UpdateDataIndex(500)
    pool.Free(op)
    let op2 = pool.AllocateForReceive(ChannelId_ 20u)
    Assert.Equal(op.ObjectId, op2.ObjectId)
    match op2.State with
    | Receiving ctx ->
        Assert.Equal(ChannelId_ 20u, ctx.ChannelId)
        Assert.Equal(0, ctx.DataLastIndex)
    | _ -> Assert.Fail("Ожидался Receiving")

[<Fact>]
let ``Массовая аллокация и возврат — счётчики корректны`` () =
    let pool, _ = makePool ()
    let ops = [| for i in 1u..10u -> pool.AllocateForReceive(ChannelId_ i) |]
    Assert.Equal(10, pool.TotalCount)
    Assert.Equal(10, pool.UsedCount)
    for op in ops do pool.Free(op)
    Assert.Equal(10, pool.TotalCount)
    Assert.Equal(0, pool.UsedCount)
    Assert.Equal(10, pool.FreeCount)
    // повторная аллокация не создаёт новых
    let _ops2 = [| for i in 1u..10u -> pool.AllocateForReceive(ChannelId_ i) |]
    Assert.Equal(10, pool.TotalCount)
    Assert.Equal(10, pool.UsedCount)
