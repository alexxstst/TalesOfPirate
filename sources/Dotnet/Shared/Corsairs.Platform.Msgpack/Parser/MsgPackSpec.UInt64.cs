using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with unsigned int 64
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write uint64 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixUInt64(Span<byte> buffer, ulong value)
	{
		BinaryPrimitives.WriteUInt64BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.UInt64;
		return DataLengths.UInt64;
	}

	/// <summary>
	///     Tries to write uint64 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixUInt64(Span<byte> buffer, ulong value, out int wroteSize)
	{
		wroteSize = DataLengths.UInt64;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.UInt64;
		return BinaryPrimitives.TryWriteUInt64BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads int32 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong ReadFixUInt64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.UInt64;
		if (buffer[0] != DataCodes.UInt64) return ThrowWrongCodeException(buffer[0], DataCodes.UInt64);
		return BinaryPrimitives.ReadUInt64BigEndian(buffer.Slice(1));
	}

	/// <summary>
	///     Tries to read from <paramref name="buffer" />
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value, read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="readSize">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.UInt64" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixUInt64(ReadOnlySpan<byte> buffer, out ulong value, out int readSize)
	{
		readSize = DataLengths.UInt64;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.UInt64;
		return BinaryPrimitives.TryReadUInt64BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteUInt64(Span<byte> buffer, ulong value)
	{
		if (value <= DataCodes.FixPositiveMax) return WritePositiveFixInt(buffer, (byte)value);
		if (value <= byte.MaxValue) return WriteFixUInt8(buffer, (byte)value);
		if (value <= ushort.MaxValue) return WriteFixUInt16(buffer, (ushort)value);
		if (value <= uint.MaxValue) return WriteFixUInt32(buffer, (uint)value);
		return WriteFixUInt64(buffer, value);
	}

	/// <summary>
	///     Tries to write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteUInt64(Span<byte> buffer, ulong value, out int wroteSize)
	{
		if (value > uint.MaxValue) return TryWriteFixUInt64(buffer, value, out wroteSize);
		return TryWriteUInt32(buffer, (uint)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="ulong" /> values from <paramref name="buffer" />
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong ReadUInt64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer.IsEmpty)
		{
			readSize = 0;
			return ThrowCantReadEmptyBufferException();
		}

		var code = buffer[0];

		switch (code)
		{
			case DataCodes.Int64:
				var int64 = ReadFixInt64(buffer, out readSize);
				if (int64 < 0) return ThrowUnsignedIntException(int64);
				return (ulong)int64;

			case DataCodes.Int32:
				var int32 = ReadFixInt32(buffer, out readSize);
				if (int32 < 0) return ThrowUnsignedIntException(int32);
				return (ulong)int32;

			case DataCodes.Int16:
				var int16 = ReadFixInt32(buffer, out readSize);
				if (int16 < 0) return ThrowUnsignedIntException(int16);
				return (ulong)int16;

			case DataCodes.Int8:
				var int8 = ReadFixInt32(buffer, out readSize);
				if (int8 < 0) return ThrowUnsignedIntException(int8);
				return (ulong)int8;

			case DataCodes.UInt64:
				return ReadFixUInt64(buffer, out readSize);

			case DataCodes.UInt32:
				return ReadFixUInt32(buffer, out readSize);

			case DataCodes.UInt16:
				return ReadFixUInt16(buffer, out readSize);

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		return ThrowWrongUIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.Int32, DataCodes.Int64,
			DataCodes.UInt8, DataCodes.UInt16, DataCodes.UInt32, DataCodes.UInt64);
	}

	/// <summary>
	///     Tries to read <see cref="ulong" /> value from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value, read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="readSize">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="buffer" />[0] is not ok.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadUInt64(ReadOnlySpan<byte> buffer, out ulong value, out int readSize)
	{
		if (buffer.IsEmpty)
		{
			value = default;
			readSize = default;
			return false;
		}

		var code = buffer[0];
		bool result;

		switch (code)
		{
			case DataCodes.UInt64:
				return TryReadFixUInt64(buffer, out value, out readSize);

			case DataCodes.UInt32:
				result = TryReadFixUInt32(buffer, out var uint32, out readSize);
				value = uint32;
				return result;

			case DataCodes.UInt16:
				result = TryReadFixUInt16(buffer, out var uint16, out readSize);
				value = uint16;
				return result;

			case DataCodes.UInt8:
				result = TryReadFixUInt8(buffer, out var uint8, out readSize);
				value = uint8;
				return result;

			case DataCodes.Int64:
				result = TryReadFixInt64(buffer, out var int64, out readSize) && int64 >= 0;
				value = result ? (ulong)int64 : default;
				return result;

			case DataCodes.Int32:
				result = TryReadFixInt32(buffer, out var int32, out readSize) && int32 >= 0;
				value = result ? (ulong)int32 : default;
				return result;

			case DataCodes.Int16:
				result = TryReadFixInt16(buffer, out var int16, out readSize) && int16 >= 0;
				value = result ? (ulong)int16 : default;
				return result;

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out readSize) && int8 >= 0;
				value = result ? (ulong)int8 : default;
				return result;
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize))
		{
			value = positive;
			return true;
		}

		value = default;
		return false;
	}
}