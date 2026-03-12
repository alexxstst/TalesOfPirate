namespace Corsairs.Platform.Network.Protocol

#nowarn "3391" // Span<byte> → ReadOnlySpan<byte> implicit conversion

open System
open System.Buffers.Binary
open System.Text
open Corsairs.Platform.Network.Network

/// Пакетный протокол LIBDBC: BinaryIO, WPacket, RPacket, PacketCodec.
/// Формат кадра: [2b size (BE)][4b SESS (BE)][2b CMD (BE)][payload]
[<AutoOpen>]
module rec Packets =

    // ═══════════════════════════════════════════════════════════════
    //  BinaryIO — inline-функции чтения/записи примитивов (big-endian)
    // ═══════════════════════════════════════════════════════════════

    [<RequireQualifiedAccess>]
    module BinaryIO =

        // ── Write ────────────────────────────────────────────────

        let inline writeUInt8 (span: Span<byte>) (value: byte) =
            span[0] <- value

        let inline writeInt8 (span: Span<byte>) (value: sbyte) =
            span[0] <- byte value

        let inline writeUInt16 (span: Span<byte>) (value: uint16) =
            BinaryPrimitives.WriteUInt16BigEndian(span, value)

        let inline writeInt16 (span: Span<byte>) (value: int16) =
            BinaryPrimitives.WriteUInt16BigEndian(span, uint16 value)

        let inline writeUInt32 (span: Span<byte>) (value: uint32) =
            BinaryPrimitives.WriteUInt32BigEndian(span, value)

        let inline writeInt32 (span: Span<byte>) (value: int32) =
            BinaryPrimitives.WriteUInt32BigEndian(span, uint32 value)

        let inline writeInt64 (span: Span<byte>) (value: int64) =
            let v = uint64 value
            BinaryPrimitives.WriteUInt32BigEndian(span, uint32 (v >>> 32))
            BinaryPrimitives.WriteUInt32BigEndian(span.Slice(4), uint32 (v &&& 0xFFFFFFFFUL))

        let inline writeUInt64 (span: Span<byte>) (value: uint64) =
            writeInt64 span (int64 value)

        let inline writeFloat32 (span: Span<byte>) (value: float32) =
            BinaryPrimitives.WriteSingleLittleEndian(span, value)

        // ── Read ─────────────────────────────────────────────────

        let inline readUInt8 (span: ReadOnlySpan<byte>) : byte =
            span[0]

        let inline readInt8 (span: ReadOnlySpan<byte>) : sbyte =
            sbyte span[0]

        let inline readUInt16 (span: Span<byte>) : uint16 =
            BinaryPrimitives.ReadUInt16BigEndian(span)

        let inline readInt16 (span: Span<byte>) : int16 =
            int16 (BinaryPrimitives.ReadUInt16BigEndian(span))

        let inline readUInt32 (span: ReadOnlySpan<byte>) : uint32 =
            BinaryPrimitives.ReadUInt32BigEndian(span)

        let inline readInt32 (span: ReadOnlySpan<byte>) : int32 =
            int32 (BinaryPrimitives.ReadUInt32BigEndian(span))

        let inline readInt64 (span: ReadOnlySpan<byte>) : int64 =
            let high = uint64 (BinaryPrimitives.ReadUInt32BigEndian(span))
            let low = uint64 (BinaryPrimitives.ReadUInt32BigEndian(span.Slice(4)))
            int64 ((high <<< 32) ||| low)

        let inline readUInt64 (span: ReadOnlySpan<byte>) : uint64 =
            uint64 (readInt64 span)

        let inline readFloat32 (span: ReadOnlySpan<byte>) : float32 =
            BinaryPrimitives.ReadSingleLittleEndian(span)

    // ═══════════════════════════════════════════════════════════════
    //  PacketPool — глобальный пул буферов для пакетов
    // ═══════════════════════════════════════════════════════════════

    /// Глобальный пул буферов для пакетов. Потокобезопасный (ConcurrentQueue).
    [<RequireQualifiedAccess>]
    module PacketPool =
        let Shared = RangedPool()

    // ═══════════════════════════════════════════════════════════════
    //  WPacket — запись пакетов (аналог C++ WPacket)
    // ═══════════════════════════════════════════════════════════════

    /// Запись бинарных пакетов в формате, совместимом с C++ WPacket (LIBDBC).
    /// Буфер выделяется из PacketPool.Shared — после использования вызвать Dispose().
    type WPacket =
        val mutable Data: Memory<byte>
        val mutable internal Pool: RangedPool

        /// Создать writer с указанной ёмкостью payload.
        /// Заголовок: [2b size][4b SESS][2b CMD] = 8 байт зарезервировано.
        new(capacity: int) =
            let pool = PacketPool.Shared
            let data = pool.Allocate(8 + capacity)
            BinaryIO.writeUInt16 data.Span 8us
            { Data = data; Pool = pool }

        /// Создать writer из IRPacket (полная копия кадра).
        /// GetPacketMemory() = [2b size][4b SESS][2b CMD][payload] — копируется целиком.
        new(packet: IRPacket) =
            let pool = PacketPool.Shared
            let pktMem = packet.GetPacketMemory()
            let buf = pool.Allocate(pktMem.Length)
            pktMem.Span.CopyTo(buf.Span)
            { Data = buf; Pool = pool }

        /// Проверить/расширить буфер. Возвращает текущую позицию записи (= PacketSize).
        member private this.EnsureCapacity(needed: int) : int =
            let pos = this.GetPacketSize()
            let required = pos + needed

            if required > this.Data.Length then
                let newSize = max (this.Data.Length * 2) required
                let newData = this.Pool.Allocate(newSize)
                this.Data.Span.Slice(0, pos).CopyTo(newData.Span)
                this.Pool.Free(this.Data)
                this.Data <- newData

            pos

        /// Записать размер пакета в первые 2 байта (BE).
        member this.SetPacketSize(size: int) =
            BinaryIO.writeUInt16 this.Data.Span (uint16 size)

        /// Записать команду в позицию 6 (фиксированная).
        member this.WriteCmd(cmd: uint16) =
            BinaryIO.writeUInt16 (this.Data.Span.Slice(6)) cmd

        /// Прочитать команду из позиции 6 (BE).
        member this.GetCmd() =
            if this.Data.Length >= 8 then BinaryIO.readUInt16 (this.Data.Span.Slice(6))
            else 0us

        /// Записать SESS (RPC session ID) в позицию 2 (фиксированная).
        member this.WriteSess(sess: uint32) =
            BinaryIO.writeUInt32 (this.Data.Span.Slice(2)) sess

        /// Прочитать SESS из позиции 2 (BE).
        member this.GetSess() =
            if this.Data.Length >= 6 then BinaryIO.readUInt32 (this.Data.Span.Slice(2))
            else 0u

        // ── Payload write ────────────────────────────────────────

        member this.WriteUInt8(value: byte) =
            let pos = this.EnsureCapacity(1)
            BinaryIO.writeUInt8 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 1)

        member this.WriteInt8(value: sbyte) =
            let pos = this.EnsureCapacity(1)
            BinaryIO.writeInt8 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 1)

        member this.WriteUInt16(value: uint16) =
            let pos = this.EnsureCapacity(2)
            BinaryIO.writeUInt16 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 2)

        member this.WriteInt16(value: int16) =
            let pos = this.EnsureCapacity(2)
            BinaryIO.writeInt16 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 2)

        member this.WriteUInt32(value: uint32) =
            let pos = this.EnsureCapacity(4)
            BinaryIO.writeUInt32 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 4)

        member this.WriteInt32(value: int32) =
            let pos = this.EnsureCapacity(4)
            BinaryIO.writeInt32 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 4)

        member this.WriteInt64(value: int64) =
            let pos = this.EnsureCapacity(8)
            BinaryIO.writeInt64 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 8)

        member this.WriteUInt64(value: uint64) =
            let pos = this.EnsureCapacity(8)
            BinaryIO.writeUInt64 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 8)

        member this.WriteFloat32(value: float32) =
            let pos = this.EnsureCapacity(4)
            BinaryIO.writeFloat32 (this.Data.Span.Slice(pos)) value
            this.SetPacketSize(pos + 4)

        // ── Составные типы ───────────────────────────────────────

        /// Бинарная последовательность: [2b длина (BE)][данные].
        member this.WriteSequence(data: ReadOnlySpan<byte>) =
            this.WriteUInt16(uint16 data.Length)

            if data.Length > 0 then
                let pos = this.EnsureCapacity(data.Length)
                data.CopyTo(this.Data.Span.Slice(pos))
                this.SetPacketSize(pos + data.Length)

        /// Строка в формате C++ WriteString: [2b длина (BE, включает null)][UTF-8 данные][null].
        member this.WriteString(str: string) =
            if isNull str || str.Length = 0 then
                this.WriteUInt16(1us)
                let pos = this.EnsureCapacity(1)
                this.Data.Span[pos] <- 0uy
                this.SetPacketSize(pos + 1)
            else
                let byteCount = Encoding.UTF8.GetByteCount(str)
                let totalLen = byteCount + 1
                this.WriteUInt16(uint16 totalLen)
                let pos = this.EnsureCapacity(totalLen)
                Encoding.UTF8.GetBytes(str.AsSpan(), this.Data.Span.Slice(pos)) |> ignore
                this.Data.Span[pos + byteCount] <- 0uy
                this.SetPacketSize(pos + totalLen)

        // ── Свойства ─────────────────────────────────────────────

        /// Размер payload (без заголовка 8 байт).
        member this.PayloadLength = this.GetPacketSize() - 8

        /// Полный размер пакета на проводе (читается из первых 2 байт буфера, BE).
        member this.GetPacketSize() =
            if this.Data.Length >= 2 then int (BinaryIO.readUInt16 this.Data.Span)
            else 0

        /// Полный кадр [size][SESS][CMD][payload] — готов к отправке.
        member this.GetPacketMemory() = this.Data.Slice(0, this.GetPacketSize())

        /// Данные после size prefix: [SESS][CMD][payload].
        member this.GetDataMemory() = this.Data.Slice(2, this.GetPacketSize() - 2)

        /// Только payload (без заголовка 8 байт).
        member this.GetPayloadMemory() = this.Data.Slice(8, this.GetPacketSize() - 8)

        /// Копия полного кадра в новый массив.
        member this.ToArray() : byte[] = this.Data.Slice(0, this.GetPacketSize()).ToArray()

        /// Глубокая копия writer'а с собственным буфером из пула.
        member this.Clone() : WPacket =
            let size = this.GetPacketSize()
            let mutable w = WPacket(size - 8)
            this.Data.Span.Slice(0, size).CopyTo(w.Data.Span)
            w

        /// Сбросить для повторного использования (без возврата буфера в пул).
        member this.Reset() =
            this.Data.Span.Slice(0, 8).Clear()
            this.SetPacketSize(8)

        override this.ToString() = PacketHelper.PrintShortPacketData(this)

        /// Вернуть буфер в пул. После вызова writer нельзя использовать.
        member this.Dispose() =
            if not (isNull this.Pool) then
                this.Pool.Free(this.Data)
                this.Data <- Memory.Empty
                this.Pool <- null

    // ═══════════════════════════════════════════════════════════════
    //  IRPacket — интерфейс чтения пакетов с управлением жизненным циклом
    // ═══════════════════════════════════════════════════════════════

    /// Интерфейс чтения бинарных пакетов. Наследует IDisposable для управления буфером.
    /// DirectRPacket — zero-copy, Dispose ничего не делает.
    /// BufferedRPacket — копирует данные в пул, Dispose возвращает в пул.
    type IRPacket =
        inherit IDisposable
        abstract Sess: uint32
        abstract GetCmd: unit -> uint16
        abstract RemainingBytes: int
        abstract Position: int
        abstract HasData: bool
        /// Полный размер пакета на проводе (читается из первых 2 байт буфера, BE).
        abstract GetPacketSize: unit -> int
        /// Размер payload = GetPacketSize() - 8 (без заголовка [size][SESS][CMD]).
        abstract PayloadLength: int
        abstract ReadUInt8: unit -> byte
        abstract ReadInt8: unit -> sbyte
        abstract ReadUInt16: unit -> uint16
        abstract ReadInt16: unit -> int16
        abstract ReadUInt32: unit -> uint32
        abstract ReadInt32: unit -> int32
        abstract ReadInt64: unit -> int64
        abstract ReadUInt64: unit -> uint64
        abstract ReadFloat32: unit -> float32
        abstract ReadSequence: unit -> ReadOnlyMemory<byte>
        abstract ReadString: unit -> string
        abstract ReverseReadUInt8: unit -> byte
        abstract ReverseReadInt8: unit -> sbyte
        abstract ReverseReadUInt16: unit -> uint16
        abstract ReverseReadInt16: unit -> int16
        abstract ReverseReadUInt32: unit -> uint32
        abstract ReverseReadInt32: unit -> int32
        abstract ReverseReadInt64: unit -> int64
        abstract ReverseReadUInt64: unit -> uint64
        abstract DiscardLast: int -> bool
        /// Удалить count байт из начала payload. Возвращает новый пакет с обновлённым size.
        /// Заголовок (SESS, CMD) сохраняется. Аналог C++ memmove + SetPktLen -= count.
        abstract DiscardFirst: int -> IRPacket
        abstract Skip: int -> unit
        abstract Reset: unit -> unit
        /// Полный кадр [size][SESS][CMD][payload].
        abstract GetPacketMemory: unit -> Memory<byte>
        /// Данные после size prefix: [SESS][CMD][payload].
        abstract GetDataMemory: unit -> Memory<byte>
        /// Только payload (без заголовка 8 байт).
        abstract GetPayloadMemory: unit -> Memory<byte>

    // ═══════════════════════════════════════════════════════════════
    //  RPacket — чтение пакетов (аналог C++ RPacket)
    // ═══════════════════════════════════════════════════════════════

    /// Чтение бинарных пакетов в формате, совместимом с C++ RPacket (LIBDBC).
    /// Оперирует над полным кадром: [2b size (BE)][4b SESS (BE)][2b CMD (BE)][payload].
    /// SESS и CMD парсятся автоматически, payload читается последовательно.
    /// Реализует IRPacket; Dispose по умолчанию — no-op.
    type RPacket(initialData: Memory<byte>) =
        let mutable data = initialData
        let mutable _pos = min 8 data.Length
        let mutable _revPos = 0

        let packetSize() =
            if data.Length >= 2 then int (BinaryIO.readUInt16 data.Span)
            else 0

        let ensureAvailable (count: int) =
            let ps = packetSize()
            if _pos + count > ps then
                raise (InvalidOperationException($"Недостаточно данных: нужно {count} байт, доступно {ps - _pos}"))

        let ensureReverseAvailable (count: int) =
            let ps = packetSize()
            if ps < _revPos + count then
                raise (InvalidOperationException($"Недостаточно данных для обратного чтения: нужно {count} байт, доступно {ps - _revPos}"))

        /// SESS (RPC session ID) — читается напрямую из заголовка пакета.
        member _.Sess =
            if data.Length >= 6 then BinaryIO.readUInt32 (data.Span.Slice(2))
            else 0u

        /// Команда (CMD) — читается напрямую из заголовка пакета.
        member _.GetCmd() =
            if data.Length >= 8 then BinaryIO.readUInt16 (data.Span.Slice(6))
            else 0us

        /// Оставшиеся непрочитанные байты payload.
        member _.RemainingBytes = packetSize() - _pos

        /// Текущая позиция чтения.
        member _.Position = _pos

        /// Есть ли ещё данные для чтения.
        member _.HasData = _pos < packetSize()

        /// Полный размер пакета на проводе (читается из первых 2 байт буфера, BE).
        member _.GetPacketSize() = packetSize()

        /// Размер payload = GetPacketSize() - 8 (без заголовка [size][SESS][CMD]).
        member this.PayloadLength = this.GetPacketSize() - 8

        // ── Payload read ─────────────────────────────────────────

        member _.ReadUInt8() : byte =
            ensureAvailable 1
            let v = BinaryIO.readUInt8 (data.Span.Slice(_pos))
            _pos <- _pos + 1
            v

        member _.ReadInt8() : sbyte =
            ensureAvailable 1
            let v = BinaryIO.readInt8 (data.Span.Slice(_pos))
            _pos <- _pos + 1
            v

        member _.ReadUInt16() : uint16 =
            ensureAvailable 2
            let v = BinaryIO.readUInt16 (data.Span.Slice(_pos))
            _pos <- _pos + 2
            v

        member _.ReadInt16() : int16 =
            ensureAvailable 2
            let v = BinaryIO.readInt16 (data.Span.Slice(_pos))
            _pos <- _pos + 2
            v

        member _.ReadUInt32() : uint32 =
            ensureAvailable 4
            let v = BinaryIO.readUInt32 (data.Span.Slice(_pos))
            _pos <- _pos + 4
            v

        member _.ReadInt32() : int32 =
            ensureAvailable 4
            let v = BinaryIO.readInt32 (data.Span.Slice(_pos))
            _pos <- _pos + 4
            v

        member _.ReadInt64() : int64 =
            ensureAvailable 8
            let v = BinaryIO.readInt64 (data.Span.Slice(_pos))
            _pos <- _pos + 8
            v

        member _.ReadUInt64() : uint64 =
            ensureAvailable 8
            let v = BinaryIO.readUInt64 (data.Span.Slice(_pos))
            _pos <- _pos + 8
            v

        member _.ReadFloat32() : float32 =
            ensureAvailable 4
            let v = BinaryIO.readFloat32 (data.Span.Slice(_pos))
            _pos <- _pos + 4
            v

        // ── Составные типы ───────────────────────────────────────

        /// Бинарная последовательность: [2b длина (BE)][данные].
        member this.ReadSequence() : ReadOnlyMemory<byte> =
            let len = int (this.ReadUInt16())

            if len = 0 then
                ReadOnlyMemory<byte>.Empty
            else
                ensureAvailable len
                let result = data.Slice(_pos, len)
                _pos <- _pos + len
                result

        /// Строка в формате C++ ReadString: [2b длина (BE, включает null)][UTF-8 данные + null].
        member this.ReadString() : string =
            let seq = this.ReadSequence()

            if seq.Length = 0 then
                ""
            else
                let span = seq.Span
                let strLen = if span[span.Length - 1] = 0uy then span.Length - 1 else span.Length

                if strLen = 0 then ""
                else Encoding.UTF8.GetString(span.Slice(0, strLen))

        // ── Обратное чтение (с конца пакета) ────────────────────

        member _.ReverseReadUInt8() : byte =
            ensureReverseAvailable 1
            _revPos <- _revPos + 1
            BinaryIO.readUInt8 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadInt8() : sbyte =
            ensureReverseAvailable 1
            _revPos <- _revPos + 1
            BinaryIO.readInt8 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadUInt16() : uint16 =
            ensureReverseAvailable 2
            _revPos <- _revPos + 2
            BinaryIO.readUInt16 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadInt16() : int16 =
            ensureReverseAvailable 2
            _revPos <- _revPos + 2
            BinaryIO.readInt16 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadUInt32() : uint32 =
            ensureReverseAvailable 4
            _revPos <- _revPos + 4
            BinaryIO.readUInt32 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadInt32() : int32 =
            ensureReverseAvailable 4
            _revPos <- _revPos + 4
            BinaryIO.readInt32 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadInt64() : int64 =
            ensureReverseAvailable 8
            _revPos <- _revPos + 8
            BinaryIO.readInt64 (data.Span.Slice(packetSize() - _revPos))

        member _.ReverseReadUInt64() : uint64 =
            ensureReverseAvailable 8
            _revPos <- _revPos + 8
            BinaryIO.readUInt64 (data.Span.Slice(packetSize() - _revPos))

        /// Отбросить count байт с конца пакета. Сбрасывает обратный курсор.
        /// Обновляет size-поле в заголовке, чтобы PacketSize/DataSize и WPacket(IRPacket) были корректны.
        /// Возвращает false если после отбрасывания не останется даже заголовка (8 байт).
        member _.DiscardLast(count: int) : bool =
            let newSize = packetSize() - count
            if newSize >= 8 then
                _revPos <- 0
                BinaryIO.writeUInt16 data.Span (uint16 newSize)
                true
            else
                false

        /// Удалить count байт из начала payload in-place (аналог C++ memmove + SetPktLen).
        /// [size][SESS][CMD][dropped...][rest] → [size-count][SESS][CMD][rest]
        /// Перезаписывает data — после вызова RawData отражает реальные данные.
        /// Сбрасывает курсоры. Возвращает this.
        member this.DiscardFirst(count: int) : IRPacket =
            let ps = packetSize()
            let payloadLen = ps - 8

            if count <= 0 then
                this :> IRPacket
            elif count > payloadLen then
                invalidOp $"DiscardFirst({count}): payload всего {payloadLen} байт"
            else
                let span = data.Span

                let tailLen = payloadLen - count
                if tailLen > 0 then
                    span.Slice(8 + count, tailLen).CopyTo(span.Slice(8))

                let newSize = ps - count
                BinaryIO.writeUInt16 span (uint16 newSize)
                data <- data.Slice(0, newSize)
                _pos <- min 8 newSize
                _revPos <- 0
                this :> IRPacket

        // ── Навигация ────────────────────────────────────────────

        /// Пропустить указанное количество байт.
        member _.Skip(count: int) =
            ensureAvailable count
            _pos <- _pos + count

        /// Сбросить позицию чтения на начало payload (после [size][SESS][CMD]).
        /// Восстанавливает исходную длину данных и size-поле в заголовке.
        member _.Reset() =
            _pos <- min 8 data.Length
            _revPos <- 0

            if data.Length >= 2 then
                BinaryIO.writeUInt16 data.Span (uint16 data.Length)

        /// Полный кадр [size][SESS][CMD][payload].
        member _.GetPacketMemory() =
            let ps = packetSize()
            if ps = data.Length then data else data.Slice(0, ps)

        /// Данные после size prefix: [SESS][CMD][payload].
        member _.GetDataMemory() =
            let ps = packetSize()
            data.Slice(2, ps - 2)

        /// Только payload (без заголовка 8 байт).
        member _.GetPayloadMemory() =
            let ps = packetSize()
            data.Slice(8, ps - 8)

        override this.ToString() = PacketHelper.PrintShortPacketData(this :> IRPacket)

        interface IRPacket with
            member this.Sess = this.Sess
            member this.GetCmd() = this.GetCmd()
            member this.RemainingBytes = this.RemainingBytes
            member this.Position = this.Position
            member this.HasData = this.HasData
            member this.GetPacketSize() = this.GetPacketSize()
            member this.PayloadLength = this.PayloadLength
            member this.ReadUInt8() = this.ReadUInt8()
            member this.ReadInt8() = this.ReadInt8()
            member this.ReadUInt16() = this.ReadUInt16()
            member this.ReadInt16() = this.ReadInt16()
            member this.ReadUInt32() = this.ReadUInt32()
            member this.ReadInt32() = this.ReadInt32()
            member this.ReadInt64() = this.ReadInt64()
            member this.ReadUInt64() = this.ReadUInt64()
            member this.ReadFloat32() = this.ReadFloat32()
            member this.ReadSequence() = this.ReadSequence()
            member this.ReadString() = this.ReadString()
            member this.ReverseReadUInt8() = this.ReverseReadUInt8()
            member this.ReverseReadInt8() = this.ReverseReadInt8()
            member this.ReverseReadUInt16() = this.ReverseReadUInt16()
            member this.ReverseReadInt16() = this.ReverseReadInt16()
            member this.ReverseReadUInt32() = this.ReverseReadUInt32()
            member this.ReverseReadInt32() = this.ReverseReadInt32()
            member this.ReverseReadInt64() = this.ReverseReadInt64()
            member this.ReverseReadUInt64() = this.ReverseReadUInt64()
            member this.DiscardLast(count) = this.DiscardLast(count)
            member this.DiscardFirst(count) = this.DiscardFirst(count)
            member this.Skip(count) = this.Skip(count)
            member this.Reset() = this.Reset()
            member this.GetPacketMemory() = this.GetPacketMemory()
            member this.GetDataMemory() = this.GetDataMemory()
            member this.GetPayloadMemory() = this.GetPayloadMemory()

        interface IDisposable with
            member _.Dispose() = ()

    /// Zero-copy чтение пакета. Данные ссылаются на receive-буфер.
    /// Dispose ничего не делает — буфер управляется IoHandlerImpl.
    /// Валиден только на время синхронной обработки события.
    type DirectRPacket(data: Memory<byte>) =
        inherit RPacket(data)

    /// Чтение пакета с копированием данных в PacketPool.Shared.
    /// При создании выделяет буфер и копирует данные.
    /// При Dispose возвращает буфер в пул.
    /// Используется в ChannelSystemCommand для буферизации в очереди.
    type BufferedRPacket private (poolBuf: Memory<byte>, dataLen: int) =
        inherit RPacket(poolBuf.Slice(0, dataLen))

        interface IDisposable with
            member _.Dispose() =
                PacketPool.Shared.Free(poolBuf)

        /// Создать BufferedRPacket: выделить буфер из пула, скопировать данные.
        static member Create(src: ReadOnlyMemory<byte>) =
            let buf = PacketPool.Shared.Allocate(src.Length)
            src.CopyTo(buf)
            new BufferedRPacket(buf, src.Length)

    // ═══════════════════════════════════════════════════════════════
    //  PacketHelper — утилиты логирования пакетов
    // ═══════════════════════════════════════════════════════════════

    /// Централизованное форматирование пакетов для логирования.
    type PacketHelper private () =

        /// Словарь CMD ID → мнемоника. Заполняется лениво через рефлексию по полям CMD_* модуля Commands.
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

        /// Мнемоническое имя команды по ID (например, 1us → "CM_SAY"). Если неизвестна — числовой ID.
        static member CmdName(cmd: uint16) =
            match _cmdNames.Value.TryGetValue(cmd) with
            | true, name -> name
            | _ -> string cmd

        /// Краткая информация о читаемом пакете (IRPacket).
        static member PrintShortPacketData(packet: IRPacket) =
            $"RPacket[{PacketHelper.CmdName(packet.GetCmd())} Sess={packet.Sess} PayloadLength={packet.PayloadLength}]"

        /// Краткая информация о записываемом пакете (WPacket).
        static member PrintShortPacketData(w: WPacket) =
            if w.GetPacketSize() >= 8 then
                $"WPacket[{PacketHelper.CmdName(w.GetCmd())} Sess={w.GetSess()} Size = {w.GetPacketSize()} PayloadLength={w.PayloadLength}]"
            else
                "WPacket[Empty]"

