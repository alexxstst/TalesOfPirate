namespace Corsairs.Platform.Network.Network

open System
open System.Collections.Concurrent
open System.Threading

// ═══════════════════════════════════════════════════════════════
//  SimplePool<'T> — generic пул объектов
// ═══════════════════════════════════════════════════════════════

/// Потокобезопасный пул объектов с ленивым созданием.
type SimplePool<'T>(create: unit -> 'T) =
    let _queue = ConcurrentQueue<'T>()
    let mutable _used = 0
    let mutable _total = 0

    member _.Allocate() =
        Interlocked.Increment(&_used) |> ignore
        match _queue.TryDequeue() with
        | true, v -> v
        | _ ->
            Interlocked.Increment(&_total) |> ignore
            create()

    member _.Free(value: 'T) =
        Interlocked.Decrement(&_used) |> ignore
        _queue.Enqueue(value)

    member _.UsedCount = _used
    member _.FreeCount = _queue.Count
    member _.TotalCount = _total

    interface IDisposable with
        member _.Dispose() =
            _queue.Clear()
            _used <- 0
            _total <- 0

// ═══════════════════════════════════════════════════════════════
//  SlabPool — пул буферов фиксированного размера (slab allocation)
// ═══════════════════════════════════════════════════════════════

/// Slab allocation: выделяет один большой byte[] и раздаёт Memory<byte> слайсы.
type SlabPool(itemSize: int, itemCount: int) =
    let _slabs = ConcurrentQueue<byte[]>()
    let _buffers = ConcurrentQueue<Memory<byte>>()
    let mutable _used = 0
    let mutable _total = 0

    let reAllocate () =
        let slab = Array.zeroCreate<byte>(itemSize * itemCount)
        _slabs.Enqueue(slab)
        for i in 0 .. itemCount - 1 do
            Interlocked.Increment(&_total) |> ignore
            _buffers.Enqueue(Memory(slab, i * itemSize, itemSize))

    member _.Allocate() =
        let mutable result = Unchecked.defaultof<Memory<byte>>
        while not (_buffers.TryDequeue(&result)) do
            reAllocate()
        Interlocked.Increment(&_used) |> ignore
        result

    member _.Free(item: Memory<byte>) =
        Interlocked.Decrement(&_used) |> ignore
        _buffers.Enqueue(item)

    member _.UsedCount = _used
    member _.FreeCount = _buffers.Count
    member _.TotalCount = _total
    member _.Size = itemSize

    interface IDisposable with
        member _.Dispose() =
            _buffers.Clear()
            _slabs.Clear()
            _used <- 0
            _total <- 0

// ═══════════════════════════════════════════════════════════════
//  RangedPool — мульти-размерный пул (степени двойки 32..32768)
// ═══════════════════════════════════════════════════════════════

/// Пул буферов с автоматическим подбором размера (степени двойки).
/// Размеры: 32, 64, 128, 256, 512, 1K, 2K, 4K, 8K, 16K, 32K.
[<AllowNullLiteral>]
type RangedPool() =
    let _pool32    = new SlabPool(32, 500)
    let _pool64    = new SlabPool(64, 500)
    let _pool128   = new SlabPool(128, 500)
    let _pool256   = new SlabPool(256, 500)
    let _pool512   = new SlabPool(512, 500)
    let _pool1024  = new SlabPool(1024, 500)
    let _pool2048  = new SlabPool(2048, 500)
    let _pool4096  = new SlabPool(4096, 500)
    let _pool8192  = new SlabPool(8192, 500)
    let _pool16384 = new SlabPool(16384, 500)
    let _pool32768 = new SlabPool(32768, 500)

    let _pools = [| _pool32; _pool64; _pool128; _pool256; _pool512
                    _pool1024; _pool2048; _pool4096; _pool8192; _pool16384; _pool32768 |]

#if DEBUG
    let _usedBuffers = System.Collections.Generic.HashSet<Memory<byte>>()
    let _debugLock = obj()
#endif

    let getBlockSize (size: int) =
        if   size <= 32    then 32
        elif size <= 64    then 64
        elif size <= 128   then 128
        elif size <= 256   then 256
        elif size <= 512   then 512
        elif size <= 1024  then 1024
        elif size <= 2048  then 2048
        elif size <= 4096  then 4096
        elif size <= 8192  then 8192
        elif size <= 16384 then 16384
        elif size <= 32768 then 32768
        else invalidArg "size" $"Размер {size} должен быть в [1..32768]"

    let getPool (poolSize: int) =
        match poolSize with
        | 32    -> _pool32
        | 64    -> _pool64
        | 128   -> _pool128
        | 256   -> _pool256
        | 512   -> _pool512
        | 1024  -> _pool1024
        | 2048  -> _pool2048
        | 4096  -> _pool4096
        | 8192  -> _pool8192
        | 16384 -> _pool16384
        | 32768 -> _pool32768
        | _     -> invalidArg "poolSize" $"Размер {poolSize} должен быть степенью двойки [32..32768]"

    member _.Allocate(size: int) =
        let buffer = getPool(getBlockSize size).Allocate()
#if DEBUG
        lock _debugLock (fun () ->
            if not (_usedBuffers.Add(buffer)) then
                invalidOp "Буфер уже используется!")
#endif
        buffer

    member _.Free(buffer: Memory<byte>) =
#if DEBUG
        lock _debugLock (fun () ->
            if not (_usedBuffers.Remove(buffer)) then
                invalidOp "Буфер уже освобождён!")
#endif
        getPool(getBlockSize buffer.Length).Free(buffer)

    member _.UsedCount  = _pools |> Array.sumBy (fun p -> p.UsedCount)
    member _.FreeCount  = _pools |> Array.sumBy (fun p -> p.FreeCount)
    member _.TotalCount = _pools |> Array.sumBy (fun p -> p.TotalCount)

#if DEBUG
    member _.PrintUsedBuffers() =
        for p in _pools do
            if p.UsedCount > 0 then
                printfn $"Pool[{p.Size}]: used {p.UsedCount}"
#endif
