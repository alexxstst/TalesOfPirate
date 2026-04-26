namespace Corsairs.Platform.Network.Protocol

#nowarn "3391" // Span<byte> → ReadOnlySpan<byte> implicit conversion

open System
open System.Buffers.Binary
open System.Text
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Msgpack.Parser

/// Пакетный протокол: BinaryIO (для заголовка), WPacket, RPacket, PacketCodec.
/// Формат кадра: [2b size (BE)][4b SESS (BE)][2b CMD (BE)][msgpack payload]
[<AutoOpen>]
module rec Packets =

    // ═══════════════════════════════════════════════════════════════
    //  BinaryIO — inline-функции чтения/записи заголовка (big-endian)
    // ═══════════════════════════════════════════════════════════════

    [<RequireQualifiedAccess>]
    module BinaryIO =

        // ── Write ────────────────────────────────────────────────

        let inline writeUInt16 (span: Span<byte>) (value: uint16) =
            BinaryPrimitives.WriteUInt16BigEndian(span, value)

        let inline writeUInt32 (span: Span<byte>) (value: uint32) =
            BinaryPrimitives.WriteUInt32BigEndian(span, value)

        // ── Read ─────────────────────────────────────────────────

        let inline readUInt16 (span: Span<byte>) : uint16 =
            BinaryPrimitives.ReadUInt16BigEndian(span)

        let inline readUInt32 (span: ReadOnlySpan<byte>) : uint32 =
            BinaryPrimitives.ReadUInt32BigEndian(span)

    // ═══════════════════════════════════════════════════════════════
    //  PacketPool — глобальный пул буферов для пакетов
    // ═══════════════════════════════════════════════════════════════

    [<RequireQualifiedAccess>]
    module PacketPool =
        let Shared = RangedPool()

    // ═══════════════════════════════════════════════════════════════
    //  MsgpackUtil — вспомогательные функции для работы с msgpack payload
    // ═══════════════════════════════════════════════════════════════

    [<RequireQualifiedAccess>]
    module MsgpackUtil =

        /// Подсчёт количества msgpack-элементов верхнего уровня в payload.
        let sequenceLength (span: ReadOnlySpan<byte>) : int =
            let mutable count = 0
            let mutable offset = 0
            while offset < span.Length do
                let token = MsgPackSpec.ReadToken(span.Slice(offset))
                offset <- offset + token.Length
                count <- count + 1
            count

        /// Пропустить paramCount элементов от начала, вернуть offset следующего.
        let skipParams (span: ReadOnlySpan<byte>) (paramCount: int) : int =
            let mutable offset = 0
            for _ in 0 .. paramCount - 1 do
                let token = MsgPackSpec.ReadToken(span.Slice(offset))
                offset <- offset + token.Length
            offset

        /// Представить msgpack payload в виде строки: |value|str(len)data|bin(len)|...
        let sequenceToString (span: ReadOnlySpan<byte>) : string =
            if span.Length = 0 then ""
            else
                let sb = System.Text.StringBuilder()
                let mutable offset = 0
                while offset < span.Length do
                    let slice = span.Slice(offset)
                    let family = MsgPackSpec.GetDataFamily(slice)
                    sb.Append('|') |> ignore
                    match family with
                    | DataFamily.Nil ->
                        sb.Append("nil") |> ignore
                        let token = MsgPackSpec.ReadToken(slice)
                        offset <- offset + token.Length
                    | DataFamily.Boolean ->
                        let mutable rs = 0
                        let v = MsgPackSpec.ReadBoolean(slice, &rs)
                        sb.Append(if v then "true" else "false") |> ignore
                        offset <- offset + rs
                    | DataFamily.Integer ->
                        let mutable rs = 0
                        let v = MsgPackSpec.ReadInt64(slice, &rs)
                        sb.Append(v) |> ignore
                        offset <- offset + rs
                    | DataFamily.Float ->
                        let code = slice.[0]
                        if code = 0xcauy then // float32
                            let mutable rs = 0
                            let v = MsgPackSpec.ReadFloat(slice, &rs)
                            sb.Append(v) |> ignore
                            offset <- offset + rs
                        else // float64
                            let mutable rs = 0
                            let v = MsgPackSpec.ReadDouble(slice, &rs)
                            sb.Append(v) |> ignore
                            offset <- offset + rs
                    | DataFamily.String ->
                        let mutable rs = 0
                        let str = MsgPackSpec.ReadString(slice, &rs)
                        // Строки содержат null-terminator — убираем при выводе
                        let display =
                            if isNull str || str.Length = 0 then ""
                            elif str.[str.Length - 1] = '\000' then str.Substring(0, str.Length - 1)
                            else str
                        sb.Append("str(").Append(if isNull str then 0 else str.Length).Append(')').Append(display) |> ignore
                        offset <- offset + rs
                    | DataFamily.Binary ->
                        let mutable rs = 0
                        let bin = MsgPackSpec.ReadBinary(slice, &rs)
                        sb.Append("bin(").Append(bin.Length).Append(')') |> ignore
                        offset <- offset + rs
                    | _ ->
                        // Неизвестный тип — пропускаем элемент
                        let token = MsgPackSpec.ReadToken(slice)
                        sb.Append("?(").Append(int slice.[0]).Append(')') |> ignore
                        offset <- offset + token.Length
                sb.ToString()

    // ═══════════════════════════════════════════════════════════════
    //  WPacket — запись пакетов (msgpack payload)
    // ═══════════════════════════════════════════════════════════════

    type WPacket =
        val mutable Data: Memory<byte>
        val mutable internal Pool: RangedPool

        new(capacity: int) =
            let pool = PacketPool.Shared
            let data = pool.Allocate(8 + capacity)
            BinaryIO.writeUInt16 data.Span 8us
            { Data = data; Pool = pool }

        new(packet: IRPacket) =
            let pool = PacketPool.Shared
            let pktMem = packet.GetPacketMemory()
            let buf = pool.Allocate(pktMem.Length)
            pktMem.Span.CopyTo(buf.Span)
            { Data = buf; Pool = pool }

        member private this.EnsureCapacity(needed: int) : int =
            let pos = this.GetPacketSize()
            let required = pos + needed + 4 // +4 запас для msgpack тегов

            if required > this.Data.Length then
                let newSize = max (this.Data.Length * 2) required
                let newData = this.Pool.Allocate(newSize)
                this.Data.Span.Slice(0, pos).CopyTo(newData.Span)
                this.Pool.Free(this.Data)
                this.Data <- newData

            pos

        member this.SetPacketSize(size: int) =
            BinaryIO.writeUInt16 this.Data.Span (uint16 size)

        member this.WriteCmd(cmd: uint16) =
            BinaryIO.writeUInt16 (this.Data.Span.Slice(6)) cmd

        member this.GetCmd() =
            if this.Data.Length >= 8 then BinaryIO.readUInt16 (this.Data.Span.Slice(6))
            else 0us

        member this.WriteSess(sess: uint32) =
            BinaryIO.writeUInt32 (this.Data.Span.Slice(2)) sess

        member this.GetSess() =
            if this.Data.Length >= 6 then BinaryIO.readUInt32 (this.Data.Span.Slice(2))
            else 0u

        // ── Payload write (все целочисленные → WriteInt64) ──────

        member this.WriteInt64(value: int64) =
            let pos = this.EnsureCapacity(9)
            let written = MsgPackSpec.WriteInt64(this.Data.Span.Slice(pos), value)
            this.SetPacketSize(pos + written)

        member this.WriteUInt64(value: uint64)  = this.WriteInt64(int64 value)

        member this.WriteFloat32(value: float32) =
            let pos = this.EnsureCapacity(5)
            let written = MsgPackSpec.WriteFloat(this.Data.Span.Slice(pos), value)
            this.SetPacketSize(pos + written)

        // ── Составные типы ───────────────────────────────────────

        /// Бинарная последовательность → msgpack bin.
        member this.WriteSequence(data: ReadOnlySpan<byte>) =
            let pos = this.EnsureCapacity(5 + data.Length)
            let written = MsgPackSpec.WriteBinary(this.Data.Span.Slice(pos), data)
            this.SetPacketSize(pos + written)

        /// Строка → msgpack str (с null-terminator для совместимости с C++).
        member this.WriteString(str: string) =
            if isNull str || str.Length = 0 then
                // Пустая строка = msgpack str с 1 байтом null
                let pos = this.EnsureCapacity(6)
                let written = MsgPackSpec.WriteString(this.Data.Span.Slice(pos), "\000".AsSpan())
                this.SetPacketSize(pos + written)
            else
                let withNull = str + "\000"
                let maxBytes = 5 + Encoding.UTF8.GetMaxByteCount(withNull.Length)
                let pos = this.EnsureCapacity(maxBytes)
                let written = MsgPackSpec.WriteString(this.Data.Span.Slice(pos), withNull.AsSpan())
                this.SetPacketSize(pos + written)

        // ── Свойства ─────────────────────────────────────────────

        member this.PayloadLength = this.GetPacketSize() - 8

        member this.GetPacketSize() =
            if this.Data.Length >= 2 then int (BinaryIO.readUInt16 this.Data.Span)
            else 0

        member this.GetPacketMemory() = this.Data.Slice(0, this.GetPacketSize())
        member this.GetDataMemory() = this.Data.Slice(2, this.GetPacketSize() - 2)
        member this.GetPayloadMemory() = this.Data.Slice(8, this.GetPacketSize() - 8)
        member this.ToArray() : byte[] = this.Data.Slice(0, this.GetPacketSize()).ToArray()

        member this.Clone() : WPacket =
            let size = this.GetPacketSize()
            let mutable w = WPacket(size - 8)
            this.Data.Span.Slice(0, size).CopyTo(w.Data.Span)
            w

        member this.Reset() =
            this.Data.Span.Slice(0, 8).Clear()
            this.SetPacketSize(8)

        override this.ToString() = PacketHelper.PrintCommand(this)

        member this.Dispose() =
            if not (isNull this.Pool) then
                this.Pool.Free(this.Data)
                this.Data <- Memory.Empty
                this.Pool <- null

    // ═══════════════════════════════════════════════════════════════
    //  IRPacket — интерфейс чтения пакетов
    // ═══════════════════════════════════════════════════════════════

    type IRPacket =
        inherit IDisposable
        abstract Sess: uint32
        abstract GetCmd: unit -> uint16
        abstract RemainingBytes: int
        abstract Position: int
        abstract HasData: bool
        abstract GetPacketSize: unit -> int
        abstract PayloadLength: int
        abstract ReadInt64: unit -> int64
        abstract ReadUInt64: unit -> uint64
        abstract ReadFloat32: unit -> float32
        abstract ReadSequence: unit -> ReadOnlyMemory<byte>
        abstract ReadString: unit -> string
        abstract ReverseReadInt64: unit -> int64
        abstract ReverseReadUInt64: unit -> uint64
        /// count = количество msgpack-элементов для удаления с конца (НЕ байт!)
        abstract DiscardLast: int -> bool
        abstract DiscardLastByReverseIndex: unit -> unit
        abstract DiscardFirst: int -> IRPacket
        abstract Skip: int -> unit
        abstract Reset: unit -> unit
        abstract GetPacketMemory: unit -> Memory<byte>
        abstract GetDataMemory: unit -> Memory<byte>
        abstract GetPayloadMemory: unit -> Memory<byte>

    // ═══════════════════════════════════════════════════════════════
    //  RPacket — чтение пакетов (msgpack payload)
    // ═══════════════════════════════════════════════════════════════

    type RPacket(initialData: Memory<byte>) =
        let mutable data = initialData
        let mutable _pos = min 8 data.Length
        let mutable _paramIdx = 0
        let mutable _reverseCurrentIndex = -1
        let mutable _allParameters = 0

        let packetSize() =
            if data.Length >= 2 then int (BinaryIO.readUInt16 data.Span)
            else 0

        let payloadOffset = 8
        let payloadLength() = packetSize() - payloadOffset

        let updateReverseIndex() =
            let pl = payloadLength()
            let total =
                if pl > 0 then MsgpackUtil.sequenceLength (data.Span.Slice(payloadOffset, pl))
                else 0
            _allParameters <- total
            _reverseCurrentIndex <- total

        // ── Заголовок ──────────────────────────────────────────

        member _.Sess =
            if data.Length >= 6 then BinaryIO.readUInt32 (data.Span.Slice(2))
            else 0u

        member _.GetCmd() =
            if data.Length >= 8 then BinaryIO.readUInt16 (data.Span.Slice(6))
            else 0us

        member _.RemainingBytes = packetSize() - _pos
        member _.Position = _pos
        member _.HasData = _pos < packetSize()
        member _.GetPacketSize() = packetSize()
        member this.PayloadLength = this.GetPacketSize() - 8

        // ── Payload read (все целочисленные → ReadInt64) ────────

        member _.ReadInt64() : int64 =
            let mutable readSize = 0
            let v = MsgPackSpec.ReadInt64(data.Span.Slice(_pos), &readSize)
            _pos <- _pos + readSize
            _paramIdx <- _paramIdx + 1
            v

        member this.ReadUInt64() : uint64  = uint64 (this.ReadInt64())

        member _.ReadFloat32() : float32 =
            let mutable readSize = 0
            let v = MsgPackSpec.ReadFloat(data.Span.Slice(_pos), &readSize)
            _pos <- _pos + readSize
            _paramIdx <- _paramIdx + 1
            v

        // ── Составные типы ───────────────────────────────────────

        member _.ReadSequence() : ReadOnlyMemory<byte> =
            let mutable readSize = 0
            let binSpan = MsgPackSpec.ReadBinary(data.Span.Slice(_pos), &readSize)
            // readSize = total bytes consumed (header + data)
            let dataStart = _pos + readSize - binSpan.Length
            _pos <- _pos + readSize
            _paramIdx <- _paramIdx + 1
            if binSpan.Length = 0 then ReadOnlyMemory.Empty
            else data.Slice(dataStart, binSpan.Length)

        member _.ReadString() : string =
            let mutable readSize = 0
            let str = MsgPackSpec.ReadString(data.Span.Slice(_pos), &readSize)
            _pos <- _pos + readSize
            _paramIdx <- _paramIdx + 1
            if isNull str || str.Length = 0 then ""
            elif str[str.Length - 1] = '\000' then str.Substring(0, str.Length - 1)
            else str

        // ── Обратное чтение (по элементам) ────────────────────

        member _.ReverseReadInt64() : int64 =
            if _reverseCurrentIndex = -1 then updateReverseIndex()
            _reverseCurrentIndex <- _reverseCurrentIndex - 1
            let pl = payloadLength()
            let payloadSlice = data.Span.Slice(payloadOffset, pl)
            let offset = MsgpackUtil.skipParams payloadSlice _reverseCurrentIndex
            let mutable readSize = 0
            MsgPackSpec.ReadInt64(payloadSlice.Slice(offset), &readSize)

        member this.ReverseReadUInt64() : uint64  = uint64 (this.ReverseReadInt64())

        // ── Мутации ──────────────────────────────────────────────

        /// count = количество msgpack-элементов для удаления с конца (НЕ байт!)
        member _.DiscardLast(count: int) : bool =
            let pl = payloadLength()
            if pl = 0 && count = 0 then true
            elif pl = 0 then false
            else
                let payloadSlice = data.Span.Slice(payloadOffset, pl)
                let totalElements = MsgpackUtil.sequenceLength payloadSlice
                if totalElements < count then false
                else
                    let keepElements = totalElements - count
                    let newPayloadEnd =
                        if keepElements = 0 then 0
                        else MsgpackUtil.skipParams payloadSlice keepElements
                    BinaryIO.writeUInt16 data.Span (uint16 (8 + newPayloadEnd))
                    _reverseCurrentIndex <- -1
                    true

        member _.DiscardLastByReverseIndex() =
            if _reverseCurrentIndex = -1 then updateReverseIndex()
            let toDiscard = _allParameters - _reverseCurrentIndex
            if toDiscard > 0 then
                let pl = payloadLength()
                let payloadSlice = data.Span.Slice(payloadOffset, pl)
                let keepEnd = MsgpackUtil.skipParams payloadSlice _reverseCurrentIndex
                BinaryIO.writeUInt16 data.Span (uint16 (8 + keepEnd))
            updateReverseIndex()

        /// count = количество msgpack-элементов для удаления с начала payload.
        member this.DiscardFirst(count: int) : IRPacket =
            let pl = payloadLength()

            if count <= 0 then
                this :> IRPacket
            else
                let payloadSlice = data.Span.Slice(payloadOffset, pl)
                let totalElements = MsgpackUtil.sequenceLength payloadSlice
                if count > totalElements then
                    invalidOp $"DiscardFirst({count}): payload содержит {totalElements} элементов"
                else
                    let byteOffset = MsgpackUtil.skipParams payloadSlice count
                    let tailLen = pl - byteOffset
                    let span = data.Span
                    if tailLen > 0 then
                        span.Slice(payloadOffset + byteOffset, tailLen).CopyTo(span.Slice(payloadOffset))
                    let newSize = payloadOffset + tailLen
                    BinaryIO.writeUInt16 span (uint16 newSize)
                    data <- data.Slice(0, newSize)
                    _pos <- min payloadOffset newSize
                    _paramIdx <- 0
                    _reverseCurrentIndex <- -1
                    this :> IRPacket

        member _.Skip(count: int) =
            let mutable offset = 0
            for _ in 0 .. count - 1 do
                let token = MsgPackSpec.ReadToken(data.Span.Slice(_pos + offset))
                offset <- offset + token.Length
            _pos <- _pos + offset
            _paramIdx <- _paramIdx + count

        member _.Reset() =
            _pos <- min 8 data.Length
            _paramIdx <- 0
            _reverseCurrentIndex <- -1
            _allParameters <- 0
            if data.Length >= 2 then
                BinaryIO.writeUInt16 data.Span (uint16 data.Length)

        member _.GetPacketMemory() =
            let ps = packetSize()
            if ps = data.Length then data else data.Slice(0, ps)

        member _.GetDataMemory() =
            let ps = packetSize()
            data.Slice(2, ps - 2)

        member _.GetPayloadMemory() =
            let ps = packetSize()
            data.Slice(8, ps - 8)

        override this.ToString() = PacketHelper.PrintCommand(this :> IRPacket)

        interface IRPacket with
            member this.Sess = this.Sess
            member this.GetCmd() = this.GetCmd()
            member this.RemainingBytes = this.RemainingBytes
            member this.Position = this.Position
            member this.HasData = this.HasData
            member this.GetPacketSize() = this.GetPacketSize()
            member this.PayloadLength = this.PayloadLength
            member this.ReadInt64() = this.ReadInt64()
            member this.ReadUInt64() = this.ReadUInt64()
            member this.ReadFloat32() = this.ReadFloat32()
            member this.ReadSequence() = this.ReadSequence()
            member this.ReadString() = this.ReadString()
            member this.ReverseReadInt64() = this.ReverseReadInt64()
            member this.ReverseReadUInt64() = this.ReverseReadUInt64()
            member this.DiscardLast(count) = this.DiscardLast(count)
            member this.DiscardLastByReverseIndex() = this.DiscardLastByReverseIndex()
            member this.DiscardFirst(count) = this.DiscardFirst(count)
            member this.Skip(count) = this.Skip(count)
            member this.Reset() = this.Reset()
            member this.GetPacketMemory() = this.GetPacketMemory()
            member this.GetDataMemory() = this.GetDataMemory()
            member this.GetPayloadMemory() = this.GetPayloadMemory()

        interface IDisposable with
            member _.Dispose() = ()

    type DirectRPacket(data: Memory<byte>) =
        inherit RPacket(data)

    type BufferedRPacket private (poolBuf: Memory<byte>, dataLen: int) =
        inherit RPacket(poolBuf.Slice(0, dataLen))

        interface IDisposable with
            member _.Dispose() =
                PacketPool.Shared.Free(poolBuf)

        static member Create(src: ReadOnlyMemory<byte>) =
            let buf = PacketPool.Shared.Allocate(src.Length)
            src.CopyTo(buf)
            new BufferedRPacket(buf, src.Length)

    // ═══════════════════════════════════════════════════════════════
    //  PacketHelper — утилиты логирования пакетов
    // ═══════════════════════════════════════════════════════════════

    type PacketHelper private () =

        static let _cmdNames =
            lazy (
                let dict = System.Collections.Generic.Dictionary<uint16, string>()
                let asm = Reflection.Assembly.GetExecutingAssembly()
                let t = asm.GetType("Corsairs.Platform.Network.Protocol.Commands")
                if not (isNull t) then
                    for f in t.GetFields(Reflection.BindingFlags.Public ||| Reflection.BindingFlags.Static) do
                        if f.IsLiteral && f.FieldType = typeof<uint16> && f.Name.StartsWith("CMD_") && not (f.Name.EndsWith("BASE")) then
                            dict[f.GetValue(null) :?> uint16] <- f.Name.Substring(4)
                dict)

        static member CmdName(cmd: uint16) =
            match _cmdNames.Value.TryGetValue(cmd) with
            | true, name -> name
            | _ -> string cmd

        /// Команды, шумящие в логах при штатной работе. Печать пакетного ToString
        /// возвращает "" для них; коллер сам должен пропустить лог-строку, если
        /// не хочет видеть пустое "IN << " в консоли.
        static member IsMuted(cmd: uint16) =
            match cmd with
            | Commands.CMD_MT_MAPENTRY -> true
            | Commands.CMD_TM_MAPENTRY -> true
            | _-> false

        static member PrintShortPacketData(packet: IRPacket) =
            $"RPacket[{PacketHelper.CmdName(packet.GetCmd())} Sess={packet.Sess} PayloadLength={packet.PayloadLength}]"

        static member PrintShortPacketData(w: WPacket) =
            if w.GetPacketSize() >= 8 then
                $"WPacket[{PacketHelper.CmdName(w.GetCmd())}({w.GetCmd()}) Sess={w.GetSess()} Size = {w.GetPacketSize()} PayloadLength={w.PayloadLength}]"
            else
                "WPacket[Empty]"

        static member PrintCommand(r: IRPacket) =
            if PacketHelper.IsMuted(r.GetCmd()) then ""
            elif r.GetPacketSize() < 8 then
                "<< RPacket[Empty]"
            else
                let payload =
                    if r.PayloadLength > 0 then
                        MsgpackUtil.sequenceToString (r.GetPayloadMemory().Span)
                    else ""
                $"<< RPacket[Cmd={PacketHelper.CmdName(r.GetCmd())}({r.GetCmd()}) Sess={r.Sess} Size={r.GetPacketSize()}{payload}]"

        static member PrintCommand(w: WPacket) =
            if PacketHelper.IsMuted(w.GetCmd()) then ""
            elif w.GetPacketSize() < 8 then
                ">> WPacket[Empty]"
            else
                let payload =
                    if w.PayloadLength > 0 then
                        MsgpackUtil.sequenceToString (w.GetPayloadMemory().Span)
                    else ""
                $">> WPacket[Cmd={PacketHelper.CmdName(w.GetCmd())}({w.GetCmd()}) Sess={w.GetSess()} Size={w.GetPacketSize()}{payload}]"
