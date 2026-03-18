using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with signed int 64
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write int64 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixInt64(Span<byte> buffer, long value)
	{
		BinaryPrimitives.WriteInt64BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.Int64;
		return DataLengths.Int64;
	}

	/// <summary>
	///     Tries to write int64 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixInt64(Span<byte> buffer, long value, out int wroteSize)
	{
		wroteSize = DataLengths.Int64;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.Int64;
		return BinaryPrimitives.TryWriteInt64BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads int64 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static long ReadFixInt64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Int64;
		if (buffer[0] != DataCodes.Int64) ThrowWrongCodeException(buffer[0], DataCodes.Int64);
		return BinaryPrimitives.ReadInt64BigEndian(buffer.Slice(1));
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.Int64" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixInt64(ReadOnlySpan<byte> buffer, out long value, out int readSize)
	{
		readSize = DataLengths.Int64;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.Int64;
		return BinaryPrimitives.TryReadInt64BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, long value)
	{
		if (value >= 0) return WriteUInt64(buffer, (ulong)value);
		if (value >= DataCodes.FixNegativeMinSByte) return WriteNegativeFixInt(buffer, (sbyte)value);
		if (value >= sbyte.MinValue) return WriteFixInt8(buffer, (sbyte)value);
		if (value >= short.MinValue) return WriteFixInt16(buffer, (short)value);
		if (value >= int.MinValue) return WriteFixInt32(buffer, (int)value);
		return WriteFixInt64(buffer, value);
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
	public static bool TryWriteInt64(Span<byte> buffer, long value, out int wroteSize)
	{
		if (value >= 0) return TryWriteUInt64(buffer, (ulong)value, out wroteSize);
		if (value < int.MinValue) return TryWriteFixInt64(buffer, value, out wroteSize);
		if (value < short.MinValue) return TryWriteFixInt32(buffer, (int)value, out wroteSize);
		if (value < sbyte.MinValue) return TryWriteFixInt16(buffer, (short)value, out wroteSize);
		return TryWriteInt8(buffer, (sbyte)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="long" /> values from <paramref name="buffer" />
	/// </summary>
	public static long ReadInt64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer.IsEmpty) ThrowCantReadEmptyBufferException();
		var code = buffer[0];

		switch (code)
		{
			case DataCodes.Int64:
				return ReadFixInt64(buffer, out readSize);

			case DataCodes.Int32:
				return ReadFixInt32(buffer, out readSize);

			case DataCodes.Int16:
				return ReadFixInt16(buffer, out readSize);

			case DataCodes.Int8:
				return ReadFixInt8(buffer, out readSize);

			case DataCodes.UInt64:
				var value = ReadFixUInt64(buffer, out readSize);
				if (value > long.MaxValue) ThrowValueIsTooLargeException(value, long.MaxValue);
				return (long)value;

			case DataCodes.UInt32:
				return ReadFixUInt32(buffer, out readSize);

			case DataCodes.UInt16:
				return ReadFixUInt16(buffer, out readSize);

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		if (TryReadNegativeFixInt(buffer, out var negative, out readSize)) return negative;

		return ThrowWrongIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.Int32, DataCodes.Int64,
			DataCodes.UInt8, DataCodes.UInt16, DataCodes.UInt32, DataCodes.UInt64);
	}

	/// <summary>
	///     Tries to read <see cref="long" /> value from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value, read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="headerSize">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="buffer" />[0] is not ok.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadInt64(ReadOnlySpan<byte> buffer, out long value, out int headerSize, out byte code)
	{
		if (buffer.IsEmpty)
		{
			value = default;
			headerSize = default;
			code = default;
			return false;
		}

		code = buffer[0];
		bool result;

		switch (code)
		{
			case DataCodes.Int64:
				return TryReadFixInt64(buffer, out value, out headerSize);

			case DataCodes.Int32:
				result = TryReadFixInt32(buffer, out var int32, out headerSize);
				value = int32;
				return result;

			case DataCodes.Int16:
				result = TryReadFixInt16(buffer, out var int16, out headerSize);
				value = int16;
				return result;

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out headerSize);
				value = int8;
				return result;

			case DataCodes.UInt64:
				result = TryReadFixUInt64(buffer, out var uint64, out headerSize) && uint64 <= long.MaxValue;
				value = result ? (long)uint64 : default;
				return result;

			case DataCodes.UInt32:
				result = TryReadFixUInt32(buffer, out var uint32, out headerSize);
				value = uint32;
				return result;

			case DataCodes.UInt16:
				result = TryReadFixUInt16(buffer, out var uint16, out headerSize);
				value = uint16;
				return result;

			case DataCodes.UInt8:
				result = TryReadFixUInt8(buffer, out var uint8, out headerSize);
				value = uint8;
				return result;
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out headerSize))
		{
			value = positive;
			return true;
		}

		if (TryReadNegativeFixInt(buffer, out var negative, out headerSize))
		{
			value = negative;
			return true;
		}

		value = default;
		return false;
	}
}