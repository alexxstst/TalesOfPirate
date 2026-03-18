module Corsairs.Platform.Network.Tests.PacketTests

open System
open System.Buffers.Binary
open Xunit
open Corsairs.Platform.Network.Protocol

// ═══════════════════════════════════════════════════════════════
//  Группа 1: WPacket — тесты записи
// ═══════════════════════════════════════════════════════════════

module WPacketWrite =

    [<Fact>]
    let ``Пустой WPacket: FrameLength = 8, PayloadLength = 0`` () =
        let mutable w = new WPacket(16)
        Assert.Equal(8, w.GetPacketSize())
        Assert.Equal(0, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteCmd записывает BE в позицию 6`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(0x1234us)
        let span = w.Data.Span
        Assert.Equal(0x12uy, span[6])
        Assert.Equal(0x34uy, span[7])
        w.Dispose()

    [<Fact>]
    let ``WriteSess записывает BE в позицию 2`` () =
        let mutable w = new WPacket(16)
        w.WriteSess(0xAABBCCDDu)
        let span = w.Data.Span
        Assert.Equal(0xAAuy, span[2])
        Assert.Equal(0xBBuy, span[3])
        Assert.Equal(0xCCuy, span[4])
        Assert.Equal(0xDDuy, span[5])
        w.Dispose()

    [<Fact>]
    let ``Size field обновляется при записи`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        let size = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span)
        Assert.Equal(8us, size)
        w.WriteInt64(42L) // positive fixint → 1 byte
        let size2 = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span)
        Assert.Equal(9us, size2)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 записывает msgpack int`` () =
        let mutable w = new WPacket(16)
        w.WriteInt64(42L)
        Assert.True(w.PayloadLength > 0)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 маленькое значение — compact (1 байт)`` () =
        let mutable w = new WPacket(16)
        w.WriteInt64(42L) // positive fixint: 0..127 → 1 byte
        Assert.Equal(1, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 значение 255 — 2 байта`` () =
        let mutable w = new WPacket(16)
        w.WriteInt64(255L) // uint8: cc ff → 2 bytes
        Assert.Equal(2, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 значение 256 — 3 байта`` () =
        let mutable w = new WPacket(16)
        w.WriteInt64(256L) // uint16: cd 01 00 → 3 bytes
        Assert.Equal(3, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 значение 0 — compact 1 байт`` () =
        let mutable w = new WPacket(16)
        w.WriteInt64(0L) // positive fixint 0 → 1 byte
        Assert.Equal(1, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 large — 9 байт`` () =
        let mutable w = new WPacket(32)
        w.WriteInt64(Int64.MaxValue) // int64: cf 7f ff ... → 9 bytes
        Assert.Equal(9, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteFloat32 записывает 5 байт`` () =
        let mutable w = new WPacket(16)
        w.WriteFloat32(1.0f) // float32: ca xx xx xx xx → 5 bytes
        Assert.Equal(5, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteString ASCII`` () =
        let mutable w = new WPacket(64)
        w.WriteString("Hello")
        // msgpack str: "Hello\0" = 6 chars → fixstr + 6 bytes = 7 bytes payload
        Assert.True(w.PayloadLength > 0)
        w.Dispose()

    [<Fact>]
    let ``WriteString пустая строка`` () =
        let mutable w = new WPacket(32)
        w.WriteString("")
        Assert.True(w.PayloadLength > 0)
        w.Dispose()

    [<Fact>]
    let ``WriteString null`` () =
        let mutable w = new WPacket(32)
        w.WriteString(null)
        Assert.True(w.PayloadLength > 0)
        w.Dispose()

    [<Fact>]
    let ``WriteString UTF-8 кириллица`` () =
        let mutable w = new WPacket(64)
        w.WriteString("Привет")
        Assert.True(w.PayloadLength > 0)
        w.Dispose()

    [<Fact>]
    let ``WriteSequence данные`` () =
        let mutable w = new WPacket(32)
        w.WriteSequence(ReadOnlySpan<byte>([| 1uy; 2uy; 3uy; 4uy |]))
        // msgpack bin: bin8 header + 4 bytes data
        Assert.True(w.PayloadLength > 4)
        w.Dispose()

    [<Fact>]
    let ``EnsureCapacity расширяет буфер`` () =
        let mutable w = new WPacket(8) // маленький
        for _ in 0..99 do
            w.WriteInt64(42L)
        Assert.True(w.GetPacketSize() > 8)
        w.Dispose()

    [<Fact>]
    let ``Reset сбрасывает позицию`` () =
        let mutable w = new WPacket(32)
        w.WriteInt64(12345L)
        Assert.True(w.PayloadLength > 0)
        w.Reset()
        Assert.Equal(0, w.PayloadLength)
        Assert.Equal(8, w.GetPacketSize())
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 2: RPacket — тесты чтения
// ═══════════════════════════════════════════════════════════════

module RPacketRead =

    [<Fact>]
    let ``ReadUInt8 roundtrip`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xABL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xABuy, byte (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReadUInt32 roundtrip`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xDEADBEEFL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xDEADBEEFu, uint32 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReadFloat32 roundtrip`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteFloat32(3.14f)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(3.14f, r.ReadFloat32(), 5)
        w.Dispose()

    [<Fact>]
    let ``ReadString пустая`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteString("")
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal("", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``ReadString null`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteString(null)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal("", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``ReadSequence roundtrip`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        let data = [| 0xAAuy; 0xBBuy; 0xCCuy |]
        w.WriteSequence(ReadOnlySpan<byte>(data))
        let r = new RPacket(w.GetPacketMemory())
        let result = r.ReadSequence()
        Assert.Equal(3, result.Length)
        Assert.Equal(0xAAuy, result.Span[0])
        Assert.Equal(0xBBuy, result.Span[1])
        Assert.Equal(0xCCuy, result.Span[2])
        w.Dispose()

    [<Fact>]
    let ``ReadSequence пустая`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteSequence(ReadOnlySpan<byte>.Empty)
        let r = new RPacket(w.GetPacketMemory())
        let result = r.ReadSequence()
        Assert.Equal(0, result.Length)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 3: Roundtrip WPacket → RPacket
// ═══════════════════════════════════════════════════════════════

module WPacketToRPacket =

    [<Fact>]
    let ``Roundtrip все целочисленные типы`` () =
        let mutable w = new WPacket(64)
        w.WriteCmd(1us)
        w.WriteInt64(255L)
        w.WriteInt64(-128L)
        w.WriteInt64(65535L)
        w.WriteInt64(-32768L)
        w.WriteInt64(0xFFFFFFFFL)
        w.WriteInt64(int64 Int32.MinValue)
        w.WriteInt64(Int64.MinValue)
        w.WriteUInt64(UInt64.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(255uy, byte (r.ReadInt64()))
        Assert.Equal(-128y, sbyte (r.ReadInt64()))
        Assert.Equal(65535us, uint16 (r.ReadInt64()))
        Assert.Equal(-32768s, int16 (r.ReadInt64()))
        Assert.Equal(0xFFFFFFFFu, uint32 (r.ReadInt64()))
        Assert.Equal(Int32.MinValue, int32 (r.ReadInt64()))
        Assert.Equal(Int64.MinValue, r.ReadInt64())
        Assert.Equal(UInt64.MaxValue, r.ReadUInt64())
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt8`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(int64 Byte.MinValue)
        w.WriteInt64(int64 Byte.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(Byte.MinValue, byte (r.ReadInt64()))
        Assert.Equal(Byte.MaxValue, byte (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int16`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(int64 Int16.MinValue)
        w.WriteInt64(int64 Int16.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(Int16.MinValue, int16 (r.ReadInt64()))
        Assert.Equal(Int16.MaxValue, int16 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt16`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(int64 UInt16.MinValue)
        w.WriteInt64(int64 UInt16.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(UInt16.MinValue, uint16 (r.ReadInt64()))
        Assert.Equal(UInt16.MaxValue, uint16 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int32`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(int64 Int32.MinValue)
        w.WriteInt64(int64 Int32.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(Int32.MinValue, int32 (r.ReadInt64()))
        Assert.Equal(Int32.MaxValue, int32 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt32`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(int64 UInt32.MinValue)
        w.WriteInt64(int64 UInt32.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(UInt32.MinValue, uint32 (r.ReadInt64()))
        Assert.Equal(UInt32.MaxValue, uint32 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int64`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        w.WriteInt64(Int64.MinValue)
        w.WriteInt64(Int64.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(Int64.MinValue, r.ReadInt64())
        Assert.Equal(Int64.MaxValue, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt64`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        w.WriteUInt64(UInt64.MinValue)
        w.WriteUInt64(UInt64.MaxValue)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(UInt64.MinValue, r.ReadUInt64())
        Assert.Equal(UInt64.MaxValue, r.ReadUInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Float32 специальные значения`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        w.WriteFloat32(Single.PositiveInfinity)
        w.WriteFloat32(Single.NegativeInfinity)
        w.WriteFloat32(Single.NaN)
        w.WriteFloat32(0.0f)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(Single.PositiveInfinity, r.ReadFloat32())
        Assert.Equal(Single.NegativeInfinity, r.ReadFloat32())
        Assert.True(Single.IsNaN(r.ReadFloat32()))
        Assert.Equal(0.0f, r.ReadFloat32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int64 отрицательный`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(-1L)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(-1L, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip множество строк подряд`` () =
        let mutable w = new WPacket(256)
        w.WriteCmd(1us)
        w.WriteString("first")
        w.WriteString("")
        w.WriteString("Третья")
        w.WriteString("fourth")
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal("first", r.ReadString())
        Assert.Equal("", r.ReadString())
        Assert.Equal("Третья", r.ReadString())
        Assert.Equal("fourth", r.ReadString())
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``Roundtrip комбинированный пакет`` () =
        let mutable w = new WPacket(256)
        w.WriteCmd(200us)
        w.WriteInt64(42L)
        w.WriteInt64(-100L)
        w.WriteInt64(999999L)
        w.WriteInt64(-1234567890L)
        w.WriteFloat32(2.5f)
        w.WriteString("test")
        w.WriteSequence(ReadOnlySpan<byte>([| 1uy; 2uy; 3uy |]))
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(200us, r.GetCmd())
        Assert.Equal(42uy, byte (r.ReadInt64()))
        Assert.Equal(-100s, int16 (r.ReadInt64()))
        Assert.Equal(999999u, uint32 (r.ReadInt64()))
        Assert.Equal(-1234567890L, r.ReadInt64())
        Assert.Equal(2.5f, r.ReadFloat32())
        Assert.Equal("test", r.ReadString())
        let seq = r.ReadSequence()
        Assert.Equal(3, seq.Length)
        Assert.Equal(1uy, seq.Span[0])
        Assert.False(r.HasData)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4: ReverseRead и DiscardLast
// ═══════════════════════════════════════════════════════════════

module ReverseRead =

    [<Fact>]
    let ``ReverseReadUInt8 читает последний элемент`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xAAL)
        w.WriteInt64(0xBBL)
        w.WriteInt64(0xCCL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xCCuy, byte (r.ReverseReadInt64()))
        Assert.Equal(0xBBuy, byte (r.ReverseReadInt64()))
        Assert.Equal(0xAAuy, byte (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt8`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(42L)
        w.WriteInt64(-7L)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(-7y, sbyte (r.ReverseReadInt64()))
        Assert.Equal(42y, sbyte (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt16`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0x1234L)
        w.WriteInt64(0xABCDL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xABCDus, uint16 (r.ReverseReadInt64()))
        Assert.Equal(0x1234us, uint16 (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt16`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(-1000L)
        w.WriteInt64(2000L)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(2000s, int16 (r.ReverseReadInt64()))
        Assert.Equal(-1000s, int16 (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt32`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xDEADBEEFL)
        w.WriteInt64(0xCAFEBABEL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xCAFEBABEu, uint32 (r.ReverseReadInt64()))
        Assert.Equal(0xDEADBEEFu, uint32 (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt32`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(-999999L)
        w.WriteInt64(123456L)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(123456, int32 (r.ReverseReadInt64()))
        Assert.Equal(-999999, int32 (r.ReverseReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt64`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        w.WriteInt64(0x0000000100000002L)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0x0000000100000002L, r.ReverseReadInt64())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt64`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(1us)
        w.WriteUInt64(0xFFFFFFFFFFFFFFFFUL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.Equal(0xFFFFFFFFFFFFFFFFUL, r.ReverseReadUInt64())
        w.Dispose()

    [<Fact>]
    let ``DiscardLast отбрасывает элементы с конца`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0x11111111L)
        w.WriteInt64(0xAAAAL)   // trailer (1 элемент)
        let r = new RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(1)) // удалить 1 элемент (aimnum)
        Assert.Equal(0x11111111u, uint32 (r.ReadInt64()))
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardLast возвращает false если не хватает элементов`` () =
        let mutable w = new WPacket(4)
        w.WriteCmd(1us)
        w.WriteInt64(0x12345678L) // 1 элемент
        let r = new RPacket(w.GetPacketMemory())
        Assert.False(r.DiscardLast(5)) // слишком много
        Assert.Equal(0x12345678u, uint32 (r.ReadInt64()))
        w.Dispose()

    [<Fact>]
    let ``Комбинированный: reverse read + DiscardLast + forward read (паттерн GateServer)`` () =
        let mutable w = new WPacket(64)
        w.WriteCmd(100us)
        w.WriteString("garner")         // payload
        w.WriteInt64(0xDEADBEEFL)      // trailer: dbid
        w.WriteInt64(3L)              // trailer: aimnum

        let r = new RPacket(w.GetPacketMemory())

        // 1) Обратное чтение trailer
        let aimnum = uint16 (r.ReverseReadInt64())
        Assert.Equal(3us, aimnum)
        let dbid = uint32 (r.ReverseReadInt64())
        Assert.Equal(0xDEADBEEFu, dbid)

        // 2) Отбросить trailer (2 элемента: dbid + aimnum)
        r.DiscardLastByReverseIndex()

        // 3) Прямое чтение payload
        let name = r.ReadString()
        Assert.Equal("garner", name)
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``Reset сбрасывает обратный курсор и эффективную длину`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0x11111111L)
        w.WriteInt64(0xAAAAL)
        let r = new RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(1)) // удалить aimnum
        r.Reset()
        // После Reset пакет снова полный
        Assert.Equal(w.GetPacketSize(), r.GetPacketMemory().Length)
        Assert.Equal(0x11111111u, uint32 (r.ReadInt64()))
        Assert.Equal(0xAAAAus, uint16 (r.ReadInt64()))
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``ReverseRead через IRPacket интерфейс`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xCAFEBABEL)
        w.WriteInt64(42L)
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(42us, uint16 (r.ReverseReadInt64()))
        Assert.Equal(0xCAFEBABEu, uint32 (r.ReverseReadInt64()))
        // DiscardLastByReverseIndex вместо DiscardLast(6)
        r.DiscardLastByReverseIndex()
        Assert.False(r.HasData)
        (r :> IDisposable).Dispose()
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4b: DiscardFirst
// ═══════════════════════════════════════════════════════════════

module DiscardFirst =

    [<Fact>]
    let ``DiscardFirst удаляет элементы из начала payload`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(42us)
        w.WriteSess(7u)
        w.WriteInt64(0xDEADL)   // counter (будет удалён)
        w.WriteInt64(0xBEEFL)   // actual payload
        let r = new RPacket(w.GetPacketMemory())
        let stripped = r.DiscardFirst(1) // 1 элемент = counter
        Assert.Equal(42us, stripped.GetCmd())
        Assert.Equal(7u, stripped.Sess)
        Assert.Equal(0xBEEFu, uint32 (stripped.ReadInt64()))
        Assert.False(stripped.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst(0) возвращает эквивалентный пакет`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xAABBCCDDL)
        let r = new RPacket(w.GetPacketMemory())
        let same = r.DiscardFirst(0)
        Assert.Equal(1us, same.GetCmd())
        Assert.Equal(0xAABBCCDDu, uint32 (same.ReadInt64()))
        Assert.False(same.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst удаляет весь payload`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(5us)
        w.WriteInt64(0x12345678L) // 1 элемент
        let r = new RPacket(w.GetPacketMemory())
        let empty = r.DiscardFirst(1)
        Assert.Equal(5us, empty.GetCmd())
        Assert.Equal(8, empty.GetPacketSize())
        Assert.False(empty.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst WPE-сценарий: counter + строка`` () =
        let mutable w = new WPacket(64)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteInt64(42L)          // counter
        w.WriteString("Hello")      // actual payload
        let r = new RPacket(w.GetPacketMemory())
        let counter = uint32 (r.ReadInt64())
        Assert.Equal(42u, counter)
        // Удаляем counter (1 элемент)
        let stripped = (new RPacket(w.GetPacketMemory()) :> IRPacket).DiscardFirst(1)
        Assert.Equal(Commands.CMD_CM_SAY, stripped.GetCmd())
        Assert.Equal("Hello", stripped.ReadString())
        Assert.False(stripped.HasData)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4c: DiscardLast/DiscardFirst — PacketSize, DataSize, WPacket
// ═══════════════════════════════════════════════════════════════

module DiscardLastDiscardFirstSizes =

    [<Fact>]
    let ``DiscardLast обновляет PacketSize и DataSize`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xAABBCCDDL) // элемент 1
        w.WriteInt64(99L)        // элемент 2
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        let origSize = r.GetPacketSize()
        let origPayload = r.PayloadLength
        Assert.True(origPayload > 0)
        Assert.True(r.DiscardLast(1)) // удалить 1 элемент (99us)
        Assert.True(r.GetPacketSize() < origSize)
        Assert.True(r.PayloadLength < origPayload)
        w.Dispose()

    [<Fact>]
    let ``DiscardLast + WPacket создаёт правильный пакет`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(100us)
        w.WriteInt64(0x11111111L)      // payload
        w.WriteInt64(0xDEADBEEFL)      // trailer: dbid
        w.WriteInt64(1L)              // trailer: aimnum
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        // Читаем trailer
        Assert.Equal(1us, uint16 (r.ReverseReadInt64()))
        Assert.Equal(0xDEADBEEFu, uint32 (r.ReverseReadInt64()))
        r.DiscardLastByReverseIndex()
        // Создаём WPacket из обрезанного пакета
        let mutable copy = new WPacket(r)
        let r2 = new RPacket(copy.GetPacketMemory())
        Assert.Equal(100us, r2.GetCmd())
        Assert.Equal(0x11111111u, uint32 (r2.ReadInt64()))
        Assert.False(r2.HasData)
        copy.Dispose()
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst обновляет PacketSize и DataSize`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(42us)
        w.WriteInt64(0L)              // counter (1 элемент)
        w.WriteInt64(0xCAFEBABEL)     // payload (1 элемент)
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        let origSize = r.GetPacketSize()
        let origPayload = r.PayloadLength
        let stripped = r.DiscardFirst(1) // удалить counter (modifies r in-place)
        Assert.True(stripped.GetPacketSize() < origSize)
        Assert.True(stripped.PayloadLength < origPayload)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst + WPacket создаёт правильный пакет`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteInt64(7L)              // WPE counter
        w.WriteString("Test")          // actual payload
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        let stripped = r.DiscardFirst(1) // удалить counter
        let mutable copy = new WPacket(stripped)
        let r2 = new RPacket(copy.GetPacketMemory())
        Assert.Equal(Commands.CMD_CM_SAY, r2.GetCmd())
        Assert.Equal("Test", r2.ReadString())
        Assert.False(r2.HasData)
        Assert.Equal(copy.GetPacketSize(), r2.GetPacketSize())
        copy.Dispose()
        w.Dispose()

    [<Fact>]
    let ``Reset восстанавливает PacketSize после DiscardLast`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0x11111111L)
        w.WriteInt64(0xAAAAL)
        let origSize = w.GetPacketSize()
        let r = new RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(1)) // удалить 1 элемент
        Assert.True(r.GetPacketSize() < origSize)
        r.Reset()
        Assert.Equal(origSize, r.GetPacketSize())
        Assert.Equal(origSize, r.GetPacketMemory().Length)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4d: DirectRPacket из WPacket — WrittenMemory vs DataMemory
// ═══════════════════════════════════════════════════════════════

module WPacketToRPacketRoundtrip =

    [<Fact>]
    let ``DirectRPacket из WrittenMemory корректно читает данные`` () =
        let mutable w1 = new WPacket(128)
        w1.WriteCmd(100us)
        w1.WriteString("testuser")
        w1.WriteString("secret123")
        w1.WriteString("AA:BB:CC:DD")
        let r1 = new RPacket(w1.GetPacketMemory()) :> IRPacket
        let mutable w2 = new WPacket(r1)
        w2.WriteCmd(200us)
        w2.WriteInt64(0x7F000001L)  // clientIp
        w2.WriteInt64(42L)          // channelId
        let packet = new DirectRPacket(w2.GetPacketMemory().ToArray())
        Assert.Equal(200us, packet.GetCmd())
        Assert.Equal("testuser", packet.ReadString())
        Assert.Equal("secret123", packet.ReadString())
        Assert.Equal("AA:BB:CC:DD", packet.ReadString())
        Assert.Equal(0x7F000001u, uint32 (packet.ReadInt64()))
        Assert.Equal(42u, uint32 (packet.ReadInt64()))
        Assert.False(packet.HasData)
        w2.Dispose()
        w1.Dispose()

    [<Fact>]
    let ``DiscardFirst + копия GetPacketMemory во второй пакет — строка читается корректно`` () =
        let mutable w = new WPacket(64)
        w.WriteCmd(1us)
        w.WriteInt64(0xDEADL)
        w.WriteString("Hello")
        let r1 = new RPacket(w.GetPacketMemory().ToArray()) :> IRPacket
        r1.DiscardFirst(1) |> ignore // удалить 1 элемент (число)
        let r2 = new RPacket(r1.GetPacketMemory().ToArray())
        Assert.Equal("Hello", r2.ReadString())
        Assert.False(r2.HasData)
        w.Dispose()

    [<Fact>]
    let ``DataMemory пропускает size prefix и непригодна для RPacket`` () =
        let mutable w = new WPacket(32)
        w.WriteCmd(100us)
        w.WriteString("hello")
        let broken = new DirectRPacket(w.GetDataMemory().ToArray())
        Assert.NotEqual(100us, broken.GetCmd())
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 5: PacketHelper — CmdName и PrintShortPacketData
// ═══════════════════════════════════════════════════════════════

module PacketHelperTests =

    [<Fact>]
    let ``CmdName возвращает мнемонику для известной команды`` () =
        Assert.Equal("CM_SAY", PacketHelper.CmdName(Commands.CMD_CM_SAY))

    [<Fact>]
    let ``CmdName возвращает мнемонику для MC-команды`` () =
        Assert.Equal("MC_SYSINFO", PacketHelper.CmdName(Commands.CMD_MC_SYSINFO))

    [<Fact>]
    let ``CmdName возвращает числовой ID для неизвестной команды`` () =
        Assert.Equal("60000", PacketHelper.CmdName(60000us))

    [<Fact>]
    let ``CmdName не содержит BASE-команды`` () =
        let name0 = PacketHelper.CmdName(0us)
        let name500 = PacketHelper.CmdName(500us)
        Assert.DoesNotContain("BASE", name0)
        Assert.DoesNotContain("BASE", name500)

    [<Fact>]
    let ``CmdName словарь содержит множество записей`` () =
        let knownCommands = [
            Commands.CMD_CM_SAY,       "CM_SAY"
            Commands.CMD_MC_SYSINFO,   "MC_SYSINFO"
        ]
        for (cmd, expected) in knownCommands do
            Assert.Equal(expected, PacketHelper.CmdName(cmd))

    [<Fact>]
    let ``PrintShortPacketData для IRPacket содержит мнемонику`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteSess(42u)
        let r = new RPacket(w.GetPacketMemory())
        let text = PacketHelper.PrintShortPacketData(r :> IRPacket)
        Assert.Contains("CM_SAY", text)
        Assert.Contains("Sess=42", text)
        w.Dispose()

    [<Fact>]
    let ``PrintShortPacketData для WPacket содержит мнемонику`` () =
        let mutable w = new WPacket(16)
        w.WriteCmd(Commands.CMD_MC_SYSINFO)
        w.WriteSess(99u)
        let text = PacketHelper.PrintShortPacketData(w)
        Assert.Contains("MC_SYSINFO", text)
        Assert.Contains("Sess=99", text)
        w.Dispose()

    [<Fact>]
    let ``PrintShortPacketData для WPacket без Cmd показывает нулевую команду`` () =
        let mutable w = new WPacket(16)
        let text = PacketHelper.PrintShortPacketData(w)
        Assert.StartsWith("WPacket[", text)
        Assert.Contains("Sess=0", text)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 6: DiscardLastByReverseIndex
// ═══════════════════════════════════════════════════════════════

module DiscardLastByReverseIndex =

    [<Fact>]
    let ``DiscardLastByReverseIndex удаляет прочитанные элементы`` () =
        let mutable w = new WPacket(64)
        w.WriteCmd(1us)
        w.WriteInt64(111L)
        w.WriteInt64(222L)
        w.WriteInt64(333L)
        w.WriteInt64(444L)
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(444u, uint32 (r.ReverseReadInt64()))
        Assert.Equal(333u, uint32 (r.ReverseReadInt64()))
        r.DiscardLastByReverseIndex()
        // Осталось 111 и 222
        Assert.Equal(111u, uint32 (r.ReadInt64()))
        Assert.Equal(222u, uint32 (r.ReadInt64()))
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``Паттерн GateServer: body + trailer`` () =
        let mutable w = new WPacket(128)
        w.WriteCmd(500us)
        w.WriteString("body_data")
        w.WriteInt64(100L)   // gpAddr
        w.WriteInt64(200L)   // loginId
        w.WriteInt64(300L)   // actId
        w.WriteInt64(1L)     // byPassword
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(500us, r.GetCmd())
        let byPassword = byte (r.ReverseReadInt64())
        let actId = uint32 (r.ReverseReadInt64())
        let loginId = uint32 (r.ReverseReadInt64())
        let gpAddr = uint32 (r.ReverseReadInt64())
        Assert.Equal(1uy, byPassword)
        Assert.Equal(300u, actId)
        Assert.Equal(200u, loginId)
        Assert.Equal(100u, gpAddr)
        r.DiscardLastByReverseIndex()
        Assert.Equal("body_data", r.ReadString())
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``Паттерн ForwardMC: routing pairs + aimnum`` () =
        let mutable w = new WPacket(128)
        w.WriteCmd(600us)
        w.WriteString("action_data")
        w.WriteInt64(1001L)  // pair 0: playerId
        w.WriteInt64(2001L)  // pair 0: dbid
        w.WriteInt64(1002L)  // pair 1: playerId
        w.WriteInt64(2002L)  // pair 1: dbid
        w.WriteInt64(2L)    // aimnum
        let r = new RPacket(w.GetPacketMemory()) :> IRPacket
        let aimnum = int (uint16 (r.ReverseReadInt64()))
        Assert.Equal(2, aimnum)
        // Читаем пары для маршрутизации
        for _ in 0 .. aimnum - 1 do
            uint32 (r.ReverseReadInt64()) |> ignore
            uint32 (r.ReverseReadInt64()) |> ignore
        // DiscardLastByReverseIndex вместо DiscardLast(2*aimnum+1)
        r.DiscardLastByReverseIndex()
        Assert.Equal("action_data", r.ReadString())
        Assert.False(r.HasData)
        w.Dispose()
