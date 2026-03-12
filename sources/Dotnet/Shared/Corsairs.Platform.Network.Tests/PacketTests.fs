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
        let mutable w = WPacket(16)
        Assert.Equal(8, w.GetPacketSize())
        Assert.Equal(0, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``WriteCmd записывает BE в позицию 6`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(0x1234us)
        let span = w.Data.Span
        Assert.Equal(0x12uy, span[6])
        Assert.Equal(0x34uy, span[7])
        w.Dispose()

    [<Fact>]
    let ``WriteSess записывает BE в позицию 2`` () =
        let mutable w = WPacket(16)
        w.WriteSess(0xAABBCCDDu)
        let span = w.Data.Span
        Assert.Equal(0xAAuy, span[2])
        Assert.Equal(0xBBuy, span[3])
        Assert.Equal(0xCCuy, span[4])
        Assert.Equal(0xDDuy, span[5])
        w.Dispose()

    [<Fact>]
    let ``Size field обновляется при записи`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        let size = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span)
        Assert.Equal(8us, size)
        w.WriteUInt8(0xFFuy)
        let size2 = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span)
        Assert.Equal(9us, size2)
        w.Dispose()

    [<Fact>]
    let ``WriteUInt8 записывает 1 байт`` () =
        let mutable w = WPacket(16)
        w.WriteUInt8(0xABuy)
        Assert.Equal(1, w.PayloadLength)
        Assert.Equal(0xABuy, w.Data.Span[8])
        w.Dispose()

    [<Fact>]
    let ``WriteInt8 записывает 1 знаковый байт`` () =
        let mutable w = WPacket(16)
        w.WriteInt8(-1y)
        Assert.Equal(1, w.PayloadLength)
        Assert.Equal(0xFFuy, w.Data.Span[8])
        w.Dispose()

    [<Fact>]
    let ``WriteUInt16 записывает 2 байта BE`` () =
        let mutable w = WPacket(16)
        w.WriteUInt16(0x1234us)
        Assert.Equal(2, w.PayloadLength)
        Assert.Equal(0x12uy, w.Data.Span[8])
        Assert.Equal(0x34uy, w.Data.Span[9])
        w.Dispose()

    [<Fact>]
    let ``WriteInt16 записывает 2 байта BE`` () =
        let mutable w = WPacket(16)
        w.WriteInt16(-1s)
        Assert.Equal(2, w.PayloadLength)
        Assert.Equal(0xFFuy, w.Data.Span[8])
        Assert.Equal(0xFFuy, w.Data.Span[9])
        w.Dispose()

    [<Fact>]
    let ``WriteUInt32 записывает 4 байта BE`` () =
        let mutable w = WPacket(16)
        w.WriteUInt32(0xDEADBEEFu)
        Assert.Equal(4, w.PayloadLength)
        Assert.Equal(0xDEuy, w.Data.Span[8])
        Assert.Equal(0xADuy, w.Data.Span[9])
        Assert.Equal(0xBEuy, w.Data.Span[10])
        Assert.Equal(0xEFuy, w.Data.Span[11])
        w.Dispose()

    [<Fact>]
    let ``WriteInt32 записывает 4 байта BE`` () =
        let mutable w = WPacket(16)
        w.WriteInt32(-1)
        Assert.Equal(4, w.PayloadLength)
        for i in 8..11 do
            Assert.Equal(0xFFuy, w.Data.Span[i])
        w.Dispose()

    [<Fact>]
    let ``WriteInt64 записывает 8 байт (split high/low BE)`` () =
        let mutable w = WPacket(16)
        w.WriteInt64(0x0000000100000002L)
        Assert.Equal(8, w.PayloadLength)
        Assert.Equal(0x00uy, w.Data.Span[8])
        Assert.Equal(0x00uy, w.Data.Span[9])
        Assert.Equal(0x00uy, w.Data.Span[10])
        Assert.Equal(0x01uy, w.Data.Span[11])
        Assert.Equal(0x00uy, w.Data.Span[12])
        Assert.Equal(0x00uy, w.Data.Span[13])
        Assert.Equal(0x00uy, w.Data.Span[14])
        Assert.Equal(0x02uy, w.Data.Span[15])
        w.Dispose()

    [<Fact>]
    let ``WriteUInt64 записывает 8 байт`` () =
        let mutable w = WPacket(16)
        w.WriteUInt64(0xFFFFFFFFFFFFFFFFUL)
        Assert.Equal(8, w.PayloadLength)
        for i in 8..15 do
            Assert.Equal(0xFFuy, w.Data.Span[i])
        w.Dispose()

    [<Fact>]
    let ``WriteFloat32 записывает 4 байта LE`` () =
        let mutable w = WPacket(16)
        w.WriteFloat32(1.0f)
        Assert.Equal(4, w.PayloadLength)
        let read = BinaryPrimitives.ReadSingleLittleEndian(w.Data.Span.Slice(8, 4))
        Assert.Equal(1.0f, read)
        w.Dispose()

    [<Fact>]
    let ``WriteString ASCII`` () =
        let mutable w = WPacket(64)
        w.WriteString("Hello")
        Assert.Equal(8, w.PayloadLength)
        let len = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span.Slice(8, 2))
        Assert.Equal(6us, len)
        Assert.Equal(0uy, w.Data.Span[8 + 2 + 5])
        w.Dispose()

    [<Fact>]
    let ``WriteString UTF-8 кириллица`` () =
        let mutable w = WPacket(64)
        w.WriteString("Привет")
        let len = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span.Slice(8, 2))
        Assert.Equal(13us, len)
        w.Dispose()

    [<Fact>]
    let ``WriteString пустая строка`` () =
        let mutable w = WPacket(16)
        w.WriteString("")
        Assert.Equal(3, w.PayloadLength)
        let len = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span.Slice(8, 2))
        Assert.Equal(1us, len)
        Assert.Equal(0uy, w.Data.Span[10])
        w.Dispose()

    [<Fact>]
    let ``WriteString null`` () =
        let mutable w = WPacket(16)
        w.WriteString(null)
        Assert.Equal(3, w.PayloadLength)
        let len = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span.Slice(8, 2))
        Assert.Equal(1us, len)
        w.Dispose()

    [<Fact>]
    let ``WriteSequence данные`` () =
        let mutable w = WPacket(16)
        let data = [| 1uy; 2uy; 3uy |]
        w.WriteSequence(ReadOnlySpan(data))
        Assert.Equal(5, w.PayloadLength)
        let len = BinaryPrimitives.ReadUInt16BigEndian(w.Data.Span.Slice(8, 2))
        Assert.Equal(3us, len)
        Assert.Equal(1uy, w.Data.Span[10])
        Assert.Equal(2uy, w.Data.Span[11])
        Assert.Equal(3uy, w.Data.Span[12])
        w.Dispose()

    [<Fact>]
    let ``WriteSequence пустая`` () =
        let mutable w = WPacket(16)
        w.WriteSequence(ReadOnlySpan<byte>.Empty)
        Assert.Equal(2, w.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``CloneWPacket создает независимую копию`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(42us)
        w.WriteSess(7u)
        w.WriteUInt32(0xDEADu)
        let mutable clone = w.Clone()
        Assert.Equal(w.GetPacketSize(), clone.GetPacketSize())
        Assert.Equal(w.PayloadLength, clone.PayloadLength)
        for i in 0 .. w.GetPacketSize() - 1 do
            Assert.Equal(w.Data.Span[i], clone.Data.Span[i])
        // Буферы разные — изменение клона не затрагивает оригинал
        clone.Data.Span[10] <- 0uy
        Assert.NotEqual(w.Data.Span[10], clone.Data.Span[10])
        w.Dispose()
        clone.Dispose()

    [<Fact>]
    let ``Reset сбрасывает позицию`` () =
        let mutable w = WPacket(16)
        w.WriteUInt32(123u)
        Assert.Equal(4, w.PayloadLength)
        w.Reset()
        Assert.Equal(0, w.PayloadLength)
        Assert.Equal(8, w.GetPacketSize())
        w.Dispose()

    [<Fact>]
    let ``EnsureCapacity расширяет буфер`` () =
        let mutable w = WPacket(4)
        w.WriteUInt32(1u)
        w.WriteUInt32(2u)
        w.WriteUInt32(3u)
        Assert.Equal(12, w.PayloadLength)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 2: RPacket — тесты чтения
// ═══════════════════════════════════════════════════════════════

module RPacketRead =

    /// Создать полный кадр [SIZE(BE)][SESS(BE)][CMD(BE)][payload] для RPacket.
    let makeBuffer (sess: uint32) (cmd: uint16) (payload: byte[]) =
        let totalLen = 8 + payload.Length
        let buf = Array.zeroCreate totalLen
        BinaryPrimitives.WriteUInt16BigEndian(Span(buf, 0, 2), uint16 totalLen)
        BinaryPrimitives.WriteUInt32BigEndian(Span(buf, 2, 4), sess)
        BinaryPrimitives.WriteUInt16BigEndian(Span(buf, 6, 2), cmd)
        Buffer.BlockCopy(payload, 0, buf, 8, payload.Length)
        Memory<byte>(buf)

    [<Fact>]
    let ``Sess и Cmd парсятся из заголовка`` () =
        let r = RPacket(makeBuffer 0xAABBCCDDu 0x1234us [||])
        Assert.Equal(0xAABBCCDDu, r.Sess)
        Assert.Equal(0x1234us, r.GetCmd())

    [<Fact>]
    let ``RemainingBytes, HasData, Position`` () =
        let r = RPacket(makeBuffer 0u 0us [| 1uy; 2uy; 3uy |])
        Assert.Equal(3, r.RemainingBytes)
        Assert.True(r.HasData)
        Assert.Equal(8, r.Position)
        r.ReadUInt8() |> ignore
        Assert.Equal(2, r.RemainingBytes)
        Assert.Equal(9, r.Position)

    [<Fact>]
    let ``ReadUInt8`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xABuy |])
        Assert.Equal(0xABuy, r.ReadUInt8())

    [<Fact>]
    let ``ReadInt8`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xFFuy |])
        Assert.Equal(-1y, r.ReadInt8())

    [<Fact>]
    let ``ReadUInt16 BE`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0x12uy; 0x34uy |])
        Assert.Equal(0x1234us, r.ReadUInt16())

    [<Fact>]
    let ``ReadInt16 BE`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xFFuy; 0xFFuy |])
        Assert.Equal(-1s, r.ReadInt16())

    [<Fact>]
    let ``ReadUInt32 BE`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xDEuy; 0xADuy; 0xBEuy; 0xEFuy |])
        Assert.Equal(0xDEADBEEFu, r.ReadUInt32())

    [<Fact>]
    let ``ReadInt32 BE`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xFFuy; 0xFFuy; 0xFFuy; 0xFFuy |])
        Assert.Equal(-1, r.ReadInt32())

    [<Fact>]
    let ``ReadInt64 split high/low BE`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0uy;0uy;0uy;1uy; 0uy;0uy;0uy;2uy |])
        Assert.Equal(0x0000000100000002L, r.ReadInt64())

    [<Fact>]
    let ``ReadUInt64`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0xFFuy;0xFFuy;0xFFuy;0xFFuy; 0xFFuy;0xFFuy;0xFFuy;0xFFuy |])
        Assert.Equal(0xFFFFFFFFFFFFFFFFUL, r.ReadUInt64())

    [<Fact>]
    let ``ReadFloat32 LE`` () =
        let buf = Array.zeroCreate<byte> 4
        BinaryPrimitives.WriteSingleLittleEndian(Span(buf), 3.14f)
        let r = RPacket(makeBuffer 0u 0us buf)
        Assert.Equal(3.14f, r.ReadFloat32())

    [<Fact>]
    let ``ReadString ASCII`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0uy; 6uy; 72uy; 101uy; 108uy; 108uy; 111uy; 0uy |])
        Assert.Equal("Hello", r.ReadString())

    [<Fact>]
    let ``ReadString пустая`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0uy; 1uy; 0uy |])
        Assert.Equal("", r.ReadString())

    [<Fact>]
    let ``ReadSequence`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0uy; 3uy; 1uy; 2uy; 3uy |])
        let seq = r.ReadSequence()
        Assert.Equal(3, seq.Length)
        Assert.Equal(1uy, seq.Span[0])
        Assert.Equal(2uy, seq.Span[1])
        Assert.Equal(3uy, seq.Span[2])

    [<Fact>]
    let ``ReadSequence пустая`` () =
        let r = RPacket(makeBuffer 0u 0us [| 0uy; 0uy |])
        let seq = r.ReadSequence()
        Assert.Equal(0, seq.Length)

    [<Fact>]
    let ``Skip пропускает байты`` () =
        let r = RPacket(makeBuffer 0u 0us [| 1uy; 2uy; 3uy; 4uy |])
        r.Skip(2)
        Assert.Equal(3uy, r.ReadUInt8())

    [<Fact>]
    let ``Reset сбрасывает позицию на начало payload`` () =
        let r = RPacket(makeBuffer 0u 0us [| 1uy; 2uy |])
        r.ReadUInt8() |> ignore
        r.Reset()
        Assert.Equal(8, r.Position)
        Assert.Equal(1uy, r.ReadUInt8())

    [<Fact>]
    let ``Чтение за пределами буфера бросает исключение`` () =
        let r = RPacket(makeBuffer 0u 0us [| 1uy |])
        r.ReadUInt8() |> ignore
        Assert.Throws<InvalidOperationException>(fun () -> r.ReadUInt8() |> ignore)

    [<Fact>]
    let ``PacketSize читается из буфера, DataSize = PacketSize - 8`` () =
        let r = RPacket(makeBuffer 0u 0us [| 1uy; 2uy; 3uy |])
        Assert.Equal(11, r.GetPacketSize())  // 8 (header) + 3 (payload) = 11
        Assert.Equal(3, r.PayloadLength)     // 11 - 8 = 3

// ═══════════════════════════════════════════════════════════════
//  Группа 3: Roundtrip WPacket → RPacket
// ═══════════════════════════════════════════════════════════════

module Roundtrip =

    [<Fact>]
    let ``Roundtrip Cmd и Sess`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(0x1234us)
        w.WriteSess(0xABCDu)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0x1234us, r.GetCmd())
        Assert.Equal(0xABCDu, r.Sess)
        w.Dispose()

    [<Fact>]
    let ``Roundtrip UInt8`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt8(0xABuy)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xABuy, r.ReadUInt8())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int8`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt8(-42y)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-42y, r.ReadInt8())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip UInt16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt16(12345us)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(12345us, r.ReadUInt16())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt16(-12345s)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-12345s, r.ReadInt16())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip UInt32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0xDEADBEEFu)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xDEADBEEFu, r.ReadUInt32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt32(-999999)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-999999, r.ReadInt32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int64`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0x0000000100000002L)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0x0000000100000002L, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip UInt64`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt64(0xFFFFFFFFFFFFFFFFUL)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xFFFFFFFFFFFFFFFFUL, r.ReadUInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Float32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteFloat32(3.14f)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(3.14f, r.ReadFloat32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Float32 отрицательный`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteFloat32(-99.5f)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-99.5f, r.ReadFloat32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip String ASCII`` () =
        let mutable w = WPacket(64)
        w.WriteCmd(1us)
        w.WriteString("Hello World")
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal("Hello World", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip String UTF-8 кириллица`` () =
        let mutable w = WPacket(64)
        w.WriteCmd(1us)
        w.WriteString("Привет, мир!")
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal("Привет, мир!", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip String пустая`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteString("")
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal("", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip String null`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteString(null)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal("", r.ReadString())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Sequence`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        let data = [| 10uy; 20uy; 30uy; 40uy; 50uy |]
        w.WriteSequence(ReadOnlySpan(data))
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal<byte[]>(data, r.ReadSequence().ToArray())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Sequence пустая`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteSequence(ReadOnlySpan<byte>.Empty)
        let r = RPacket(w.GetPacketMemory())
        Assert.Empty(r.ReadSequence().ToArray())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip все типы в одном пакете`` () =
        let mutable w = WPacket(256)
        w.WriteCmd(500us)
        w.WriteSess(777u)
        w.WriteUInt8(255uy)
        w.WriteInt8(-128y)
        w.WriteUInt16(65535us)
        w.WriteInt16(-32768s)
        w.WriteUInt32(4294967295u)
        w.WriteInt32(-2147483648)
        w.WriteInt64(9223372036854775807L)
        w.WriteUInt64(18446744073709551615UL)
        w.WriteFloat32(2.71828f)
        w.WriteString("Test строка")
        w.WriteSequence(ReadOnlySpan([| 0xAAuy; 0xBBuy |]))

        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(500us, r.GetCmd())
        Assert.Equal(777u, r.Sess)
        Assert.Equal(255uy, r.ReadUInt8())
        Assert.Equal(-128y, r.ReadInt8())
        Assert.Equal(65535us, r.ReadUInt16())
        Assert.Equal(-32768s, r.ReadInt16())
        Assert.Equal(4294967295u, r.ReadUInt32())
        Assert.Equal(-2147483648, r.ReadInt32())
        Assert.Equal(9223372036854775807L, r.ReadInt64())
        Assert.Equal(18446744073709551615UL, r.ReadUInt64())
        Assert.Equal(2.71828f, r.ReadFloat32())
        Assert.Equal("Test строка", r.ReadString())
        let seq = r.ReadSequence()
        Assert.Equal(2, seq.Length)
        Assert.Equal(0xAAuy, seq.Span[0])
        Assert.Equal(0xBBuy, seq.Span[1])
        Assert.False(r.HasData)
        w.Dispose()

    // ── Граничные значения ──────────────────────────────────────

    [<Fact>]
    let ``Roundtrip граничные UInt8`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt8(Byte.MinValue)
        w.WriteUInt8(Byte.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(Byte.MinValue, r.ReadUInt8())
        Assert.Equal(Byte.MaxValue, r.ReadUInt8())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int8`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt8(SByte.MinValue)
        w.WriteInt8(SByte.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(SByte.MinValue, r.ReadInt8())
        Assert.Equal(SByte.MaxValue, r.ReadInt8())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt16(Int16.MinValue)
        w.WriteInt16(Int16.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(Int16.MinValue, r.ReadInt16())
        Assert.Equal(Int16.MaxValue, r.ReadInt16())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt16(UInt16.MinValue)
        w.WriteUInt16(UInt16.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(UInt16.MinValue, r.ReadUInt16())
        Assert.Equal(UInt16.MaxValue, r.ReadUInt16())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt32(Int32.MinValue)
        w.WriteInt32(Int32.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(Int32.MinValue, r.ReadInt32())
        Assert.Equal(Int32.MaxValue, r.ReadInt32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(UInt32.MinValue)
        w.WriteUInt32(UInt32.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(UInt32.MinValue, r.ReadUInt32())
        Assert.Equal(UInt32.MaxValue, r.ReadUInt32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные Int64`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        w.WriteInt64(Int64.MinValue)
        w.WriteInt64(Int64.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(Int64.MinValue, r.ReadInt64())
        Assert.Equal(Int64.MaxValue, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip граничные UInt64`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        w.WriteUInt64(UInt64.MinValue)
        w.WriteUInt64(UInt64.MaxValue)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(UInt64.MinValue, r.ReadUInt64())
        Assert.Equal(UInt64.MaxValue, r.ReadUInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Float32 специальные значения`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        w.WriteFloat32(Single.PositiveInfinity)
        w.WriteFloat32(Single.NegativeInfinity)
        w.WriteFloat32(Single.NaN)
        w.WriteFloat32(0.0f)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(Single.PositiveInfinity, r.ReadFloat32())
        Assert.Equal(Single.NegativeInfinity, r.ReadFloat32())
        Assert.True(Single.IsNaN(r.ReadFloat32()))
        Assert.Equal(0.0f, r.ReadFloat32())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int64 отрицательный`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(-1L)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-1L, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip Int64 ноль`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0L)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0L, r.ReadInt64())
        w.Dispose()

    [<Fact>]
    let ``Roundtrip множество строк подряд`` () =
        let mutable w = WPacket(256)
        w.WriteCmd(1us)
        w.WriteString("first")
        w.WriteString("")
        w.WriteString("Третья")
        w.WriteString("fourth")
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal("first", r.ReadString())
        Assert.Equal("", r.ReadString())
        Assert.Equal("Третья", r.ReadString())
        Assert.Equal("fourth", r.ReadString())
        Assert.False(r.HasData)
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4: ReverseRead и DiscardLast
// ═══════════════════════════════════════════════════════════════

module ReverseRead =

    [<Fact>]
    let ``ReverseReadUInt8 читает последний байт`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt8(0xAAuy)
        w.WriteUInt8(0xBBuy)
        w.WriteUInt8(0xCCuy)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xCCuy, r.ReverseReadUInt8())
        Assert.Equal(0xBBuy, r.ReverseReadUInt8())
        Assert.Equal(0xAAuy, r.ReverseReadUInt8())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt8`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt8(42y)
        w.WriteInt8(-7y)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(-7y, r.ReverseReadInt8())
        Assert.Equal(42y, r.ReverseReadInt8())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt16(0x1234us)
        w.WriteUInt16(0xABCDus)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xABCDus, r.ReverseReadUInt16())
        Assert.Equal(0x1234us, r.ReverseReadUInt16())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt16`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt16(-1000s)
        w.WriteInt16(2000s)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(2000s, r.ReverseReadInt16())
        Assert.Equal(-1000s, r.ReverseReadInt16())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0xDEADBEEFu)
        w.WriteUInt32(0xCAFEBABEu)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xCAFEBABEu, r.ReverseReadUInt32())
        Assert.Equal(0xDEADBEEFu, r.ReverseReadUInt32())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt32`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt32(-999999)
        w.WriteInt32(123456)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(123456, r.ReverseReadInt32())
        Assert.Equal(-999999, r.ReverseReadInt32())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadInt64`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        w.WriteInt64(0x0000000100000002L)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0x0000000100000002L, r.ReverseReadInt64())
        w.Dispose()

    [<Fact>]
    let ``ReverseReadUInt64`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(1us)
        w.WriteUInt64(0xFFFFFFFFFFFFFFFFUL)
        let r = RPacket(w.GetPacketMemory())
        Assert.Equal(0xFFFFFFFFFFFFFFFFUL, r.ReverseReadUInt64())
        w.Dispose()

    [<Fact>]
    let ``DiscardLast отбрасывает байты с конца`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0x11111111u)
        w.WriteUInt16(0xAAAAus)   // trailer
        let origSize = w.GetPacketSize()
        let r = RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(2))
        // После DiscardLast GetPacketMemory() укорочен
        Assert.Equal(origSize - 2, r.GetPacketMemory().Length)
        // Прямое чтение видит только оставшиеся данные
        Assert.Equal(0x11111111u, r.ReadUInt32())
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardLast возвращает false если не хватит на заголовок`` () =
        let mutable w = WPacket(4)
        w.WriteCmd(1us)
        w.WriteUInt32(0x12345678u)
        let r = RPacket(w.GetPacketMemory())
        // Пакет 12 байт, попытка отбросить 5 → осталось бы 7 < 8
        Assert.False(r.DiscardLast(5))
        // Данные не изменились
        Assert.Equal(0x12345678u, r.ReadUInt32())
        w.Dispose()

    [<Fact>]
    let ``Комбинированный: reverse read + DiscardLast + forward read (паттерн GateServer)`` () =
        // Пакет: [header 8b][payload: строка "garner"][trailer: UInt32 dbid + UInt16 aimnum]
        let mutable w = WPacket(64)
        w.WriteCmd(100us)
        w.WriteString("garner")         // payload
        w.WriteUInt32(0xDEADBEEFu)      // trailer: dbid
        w.WriteUInt16(3us)              // trailer: aimnum

        let r = RPacket(w.GetPacketMemory())

        // 1) Обратное чтение trailer
        let aimnum = r.ReverseReadUInt16()
        Assert.Equal(3us, aimnum)
        let dbid = r.ReverseReadUInt32()
        Assert.Equal(0xDEADBEEFu, dbid)

        // 2) Отбросить trailer (2 + 4 = 6 байт)
        Assert.True(r.DiscardLast(6))

        // 3) Прямое чтение payload
        let name = r.ReadString()
        Assert.Equal("garner", name)
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``ReverseRead бросает исключение при нехватке данных`` () =
        let mutable w = WPacket(4)
        w.WriteCmd(1us)
        w.WriteUInt8(0xAAuy)
        w.WriteUInt8(0xBBuy) // 10 байт всего
        let r = RPacket(w.GetPacketMemory())
        r.ReverseReadUInt8() |> ignore // revPos=1
        r.ReverseReadUInt8() |> ignore // revPos=2
        // 10 < 2 + 8 = false → ещё OK, но попытка прочитать ещё 8:
        // revPos станет 10, и следующий вызов выйдет за границу
        r.ReverseReadUInt64() |> ignore // revPos=10, забирает весь буфер
        Assert.Throws<InvalidOperationException>(fun () -> r.ReverseReadUInt8() |> ignore) |> ignore
        w.Dispose()

    [<Fact>]
    let ``Reset сбрасывает обратный курсор и эффективную длину`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0x11111111u)
        w.WriteUInt16(0xAAAAus)
        let r = RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(2))
        r.Reset()
        // После Reset пакет снова полный
        Assert.Equal(w.GetPacketSize(), r.GetPacketMemory().Length)
        Assert.Equal(0x11111111u, r.ReadUInt32())
        Assert.Equal(0xAAAAus, r.ReadUInt16())
        Assert.False(r.HasData)
        w.Dispose()

    [<Fact>]
    let ``ReverseRead через IRPacket интерфейс`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0xCAFEBABEu)
        w.WriteUInt16(42us)
        let r = RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(42us, r.ReverseReadUInt16())
        Assert.Equal(0xCAFEBABEu, r.ReverseReadUInt32())
        Assert.True(r.DiscardLast(6))
        Assert.False(r.HasData)
        (r :?> IDisposable).Dispose()
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 4b: DiscardFirst
// ═══════════════════════════════════════════════════════════════

module DiscardFirst =

    [<Fact>]
    let ``DiscardFirst удаляет байты из начала payload`` () =
        // [header 8b][counter 4b][payload: UInt32]
        let mutable w = WPacket(16)
        w.WriteCmd(42us)
        w.WriteSess(7u)
        w.WriteUInt32(0xDEADu)   // counter (будет удалён)
        w.WriteUInt32(0xBEEFu)   // actual payload
        let origSize = w.GetPacketSize()
        let r = RPacket(w.GetPacketMemory())
        let stripped = r.DiscardFirst(4)
        // Заголовок сохранился
        Assert.Equal(42us, stripped.GetCmd())
        Assert.Equal(7u, stripped.Sess)
        // Payload = только 0xBEEF, counter удалён
        Assert.Equal(0xBEEFu, stripped.ReadUInt32())
        Assert.False(stripped.HasData)
        // PacketSize уменьшился на 4
        Assert.Equal(origSize - 4, stripped.GetPacketSize())
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst(0) возвращает эквивалентный пакет`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0xAABBCCDDu)
        let r = RPacket(w.GetPacketMemory())
        let same = r.DiscardFirst(0)
        Assert.Equal(1us, same.GetCmd())
        Assert.Equal(0xAABBCCDDu, same.ReadUInt32())
        Assert.False(same.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst удаляет весь payload`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(5us)
        w.WriteUInt32(0x12345678u)
        let r = RPacket(w.GetPacketMemory())
        let empty = r.DiscardFirst(4)
        Assert.Equal(5us, empty.GetCmd())
        Assert.Equal(8, empty.GetPacketSize())
        Assert.False(empty.HasData)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst бросает исключение при превышении payload`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt16(0xAAus)  // 2 байта payload
        let r = RPacket(w.GetPacketMemory())
        Assert.Throws<InvalidOperationException>(fun () -> r.DiscardFirst(4) |> ignore)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst WPE-сценарий: counter + строка`` () =
        // Имитация клиентского пакета: [header][counter(4)][string]
        let mutable w = WPacket(64)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteUInt32(42u)          // counter
        w.WriteString("Hello")      // actual payload
        let r = RPacket(w.GetPacketMemory())
        // Читаем и проверяем counter
        let counter = r.ReadUInt32()
        Assert.Equal(42u, counter)
        // Удаляем counter
        let stripped = (RPacket(w.GetPacketMemory()) :> IRPacket).DiscardFirst(4)
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
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0xAABBCCDDu) // 4 bytes payload
        w.WriteUInt16(99us)        // 2 bytes trailer
        let r = RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(14, r.GetPacketSize())  // 8 header + 6 payload
        Assert.Equal(6, r.PayloadLength)
        Assert.True(r.DiscardLast(2))
        Assert.Equal(12, r.GetPacketSize())  // должно уменьшиться
        Assert.Equal(4, r.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``DiscardLast + WPacket создаёт правильный пакет`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(100us)
        w.WriteUInt32(0x11111111u)      // payload: 4 bytes
        w.WriteUInt32(0xDEADBEEFu)      // trailer: dbid
        w.WriteUInt16(1us)              // trailer: aimnum
        let r = RPacket(w.GetPacketMemory()) :> IRPacket
        // Читаем trailer
        Assert.Equal(1us, r.ReverseReadUInt16())
        Assert.Equal(0xDEADBEEFu, r.ReverseReadUInt32())
        Assert.True(r.DiscardLast(6))
        // Создаём WPacket из обрезанного пакета
        let mutable copy = WPacket(r)
        Assert.Equal(12, copy.GetPacketSize())  // 8 header + 4 payload
        // Перечитываем копию
        let r2 = RPacket(copy.GetPacketMemory())
        Assert.Equal(100us, r2.GetCmd())
        Assert.Equal(12, r2.GetPacketSize())
        Assert.Equal(4, r2.PayloadLength)
        Assert.Equal(0x11111111u, r2.ReadUInt32())
        Assert.False(r2.HasData)
        copy.Dispose()
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst обновляет PacketSize и DataSize`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(42us)
        w.WriteUInt32(0u)              // counter: 4 bytes
        w.WriteUInt32(0xCAFEBABEu)     // payload: 4 bytes
        let r = RPacket(w.GetPacketMemory()) :> IRPacket
        Assert.Equal(16, r.GetPacketSize())  // 8 + 8
        Assert.Equal(8, r.PayloadLength)
        let stripped = r.DiscardFirst(4)
        Assert.Equal(12, stripped.GetPacketSize())  // 8 + 4
        Assert.Equal(4, stripped.PayloadLength)
        w.Dispose()

    [<Fact>]
    let ``DiscardFirst + WPacket создаёт правильный пакет`` () =
        let mutable w = WPacket(32)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteUInt32(7u)              // WPE counter
        w.WriteString("Test")          // actual payload
        let r = RPacket(w.GetPacketMemory()) :> IRPacket
        let stripped = r.DiscardFirst(4)
        let mutable copy = WPacket(stripped)
        let r2 = RPacket(copy.GetPacketMemory())
        Assert.Equal(Commands.CMD_CM_SAY, r2.GetCmd())
        Assert.Equal("Test", r2.ReadString())
        Assert.False(r2.HasData)
        Assert.Equal(copy.GetPacketSize(), r2.GetPacketSize())
        copy.Dispose()
        w.Dispose()

    [<Fact>]
    let ``Reset восстанавливает PacketSize после DiscardLast`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteUInt32(0x11111111u)
        w.WriteUInt16(0xAAAAus)
        let origSize = w.GetPacketSize()
        let r = RPacket(w.GetPacketMemory())
        Assert.True(r.DiscardLast(2))
        Assert.Equal(origSize - 2, r.GetPacketSize())
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
        // Имитация CM_LOGIN: [header][account][password][mac]
        let mutable w1 = WPacket(128)
        w1.WriteCmd(100us)
        w1.WriteString("testuser")
        w1.WriteString("secret123")
        w1.WriteString("AA:BB:CC:DD")
        // Создаём WPacket-копию и меняем CMD (как в GateServer CM_LOGIN)
        let r1 = RPacket(w1.GetPacketMemory()) :> IRPacket
        let mutable w2 = WPacket(r1)
        w2.WriteCmd(200us)
        w2.WriteUInt32(0x7F000001u)  // clientIp
        w2.WriteUInt32(42u)          // channelId
        // WrittenMemory — полный фрейм [size][SESS][CMD][payload]
        let packet = DirectRPacket(w2.GetPacketMemory().ToArray())
        Assert.Equal(200us, packet.GetCmd())
        Assert.Equal("testuser", packet.ReadString())
        Assert.Equal("secret123", packet.ReadString())
        Assert.Equal("AA:BB:CC:DD", packet.ReadString())
        Assert.Equal(0x7F000001u, packet.ReadUInt32())
        Assert.Equal(42u, packet.ReadUInt32())
        Assert.False(packet.HasData)
        w2.Dispose()
        w1.Dispose()

    [<Fact>]
    let ``DiscardFirst + копия GetPacketMemory во второй пакет — строка читается корректно`` () =
        // 1. Создаём пакет с числом (4 байта) и строкой
        let mutable w = WPacket(64)
        w.WriteCmd(1us)
        w.WriteUInt32(0xDEADu)
        w.WriteString("Hello")

        // 2. Создаём пакет для чтения
        let r1 = RPacket(w.GetPacketMemory().ToArray()) :> IRPacket

        // 3. DiscardFirst убирает 4 байта (число)
        r1.DiscardFirst(4) |> ignore

        // 4. Копируем данные из первого пакета во второй
        let r2 = RPacket(r1.GetPacketMemory().ToArray())

        // 5. Читаем строку и проверяем
        Assert.Equal("Hello", r2.ReadString())
        Assert.False(r2.HasData)
        w.Dispose()

    [<Fact>]
    let ``DataMemory пропускает size prefix и непригодна для RPacket`` () =
        // DataMemory = [SESS][CMD][payload] — без 2-байт size prefix.
        // RPacket ожидает [size][SESS][CMD][payload] — смещения сдвинуты.
        let mutable w = WPacket(32)
        w.WriteCmd(100us)
        w.WriteString("hello")
        // DataMemory: начинается с SESS, не с size
        let broken = DirectRPacket(w.GetDataMemory().ToArray())
        // CMD читается из неправильного смещения — не совпадёт с 100us
        Assert.NotEqual(100us, broken.GetCmd())
        w.Dispose()

// ═══════════════════════════════════════════════════════════════
//  Группа 5: PacketHelper — CmdName и PrintShortPacketData
// ═══════════════════════════════════════════════════════════════

module PacketHelperTests =

    [<Fact>]
    let ``CmdName возвращает мнемонику для известной команды`` () =
        // CMD_CM_SAY = 1us
        Assert.Equal("CM_SAY", PacketHelper.CmdName(Commands.CMD_CM_SAY))

    [<Fact>]
    let ``CmdName возвращает мнемонику для MC-команды`` () =
        // CMD_MC_SYSINFO = 517us
        Assert.Equal("MC_SYSINFO", PacketHelper.CmdName(Commands.CMD_MC_SYSINFO))

    [<Fact>]
    let ``CmdName возвращает числовой ID для неизвестной команды`` () =
        Assert.Equal("60000", PacketHelper.CmdName(60000us))

    [<Fact>]
    let ``CmdName не содержит BASE-команды`` () =
        // CMD_CM_BASE = 0us, CMD_MC_BASE = 500us
        // Для BASE-значений CmdName не должен возвращать мнемонику с "BASE"
        let name0 = PacketHelper.CmdName(0us)
        let name500 = PacketHelper.CmdName(500us)
        Assert.DoesNotContain("BASE", name0)
        Assert.DoesNotContain("BASE", name500)

    [<Fact>]
    let ``CmdName словарь содержит множество записей`` () =
        // Проверяем несколько разных диапазонов
        let knownCommands = [
            Commands.CMD_CM_SAY,       "CM_SAY"
            Commands.CMD_MC_SYSINFO,   "MC_SYSINFO"
        ]
        for (cmd, expected) in knownCommands do
            Assert.Equal(expected, PacketHelper.CmdName(cmd))

    [<Fact>]
    let ``PrintShortPacketData для IRPacket содержит мнемонику`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(Commands.CMD_CM_SAY)
        w.WriteSess(42u)
        let r = RPacket(w.GetPacketMemory())
        let text = PacketHelper.PrintShortPacketData(r :> IRPacket)
        Assert.Contains("CM_SAY", text)
        Assert.Contains("Sess=42", text)
        w.Dispose()

    [<Fact>]
    let ``PrintShortPacketData для WPacket содержит мнемонику`` () =
        let mutable w = WPacket(16)
        w.WriteCmd(Commands.CMD_MC_SYSINFO)
        w.WriteSess(99u)
        let text = PacketHelper.PrintShortPacketData(w)
        Assert.Contains("MC_SYSINFO", text)
        Assert.Contains("Sess=99", text)
        w.Dispose()

    [<Fact>]
    let ``PrintShortPacketData для WPacket без Cmd показывает нулевую команду`` () =
        let mutable w = WPacket(16)
        let text = PacketHelper.PrintShortPacketData(w)
        // WPacket всегда имеет заголовок 8 байт, Cmd = 0 → мнемоника или "0"
        Assert.StartsWith("WPacket[", text)
        Assert.Contains("Sess=0", text)
        w.Dispose()
