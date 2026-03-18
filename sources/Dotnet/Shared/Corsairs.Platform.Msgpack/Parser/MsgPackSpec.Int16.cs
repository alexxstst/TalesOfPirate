using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with signed int 16
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write int16 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixInt16(Span<byte> buffer, short value)
	{
		BinaryPrimitives.WriteInt16BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.Int16;
		return DataLengths.Int16;
	}

	/// <summary>
	///     Tries to write int16 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixInt16(Span<byte> buffer, short value, out int wroteSize)
	{
		wroteSize = DataLengths.Int16;
		buffer[0] = DataCodes.Int16;
		return BinaryPrimitives.TryWriteInt16BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads int16 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static short ReadFixInt16(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Int16;
		if (buffer[0] != DataCodes.Int16) ThrowWrongCodeException(buffer[0], DataCodes.Int16);
		return BinaryPrimitives.ReadInt16BigEndian(buffer.Slice(1));
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.Int16" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixInt16(ReadOnlySpan<byte> buffer, out short value, out int readSize)
	{
		readSize = DataLengths.Int16;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.Int16;
		return BinaryPrimitives.TryReadInt16BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, short value)
	{
		if (value >= 0) return WriteUInt64(buffer, (ulong)value);
		if (value >= DataCodes.FixNegativeMinSByte) return WriteNegativeFixInt(buffer, (sbyte)value);
		if (value >= sbyte.MinValue) return WriteFixInt8(buffer, (sbyte)value);
		return WriteFixInt16(buffer, value);
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
	public static bool TryWriteInt16(Span<byte> buffer, short value, out int wroteSize)
	{
		if (value >= 0) return TryWriteUInt16(buffer, (ushort)value, out wroteSize);
		if (value < sbyte.MinValue) return TryWriteFixInt16(buffer, value, out wroteSize);
		return TryWriteInt8(buffer, (sbyte)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="short" /> values from <paramref name="buffer" />
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static short ReadInt16(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer.IsEmpty) ThrowCantReadEmptyBufferException();
		var code = buffer[0];

		switch (code)
		{
			case DataCodes.Int16:
				return ReadFixInt16(buffer, out readSize);

			case DataCodes.Int8:
				return ReadFixInt8(buffer, out readSize);

			case DataCodes.UInt16:
				var value = ReadFixUInt16(buffer, out readSize);
				if (value > short.MaxValue) ThrowValueIsTooLargeException(value, short.MaxValue);
				return (short)value;

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		if (TryReadNegativeFixInt(buffer, out var negative, out readSize)) return negative;

		return ThrowWrongIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.UInt8, DataCodes.UInt16);
	}

	/// <summary>
	///     Tries to read <see cref="short" /> value from <paramref name="buffer" />.
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
	public static bool TryReadInt16(ReadOnlySpan<byte> buffer, out short value, out int readSize)
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
			case DataCodes.Int16:
				return TryReadFixInt16(buffer, out value, out readSize);

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out readSize);
				value = int8;
				return result;

			case DataCodes.UInt16:
				result = TryReadFixUInt16(buffer, out var uint16, out readSize) && uint16 <= short.MaxValue;
				value = result ? (short)uint16 : default;
				return result;

			case DataCodes.UInt8:
				result = TryReadFixUInt8(buffer, out var uint8, out readSize);
				value = uint8;
				return result;
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize))
		{
			value = positive;
			return true;
		}

		if (TryReadNegativeFixInt(buffer, out var negative, out readSize))
		{
			value = negative;
			return true;
		}

		value = default;
		return false;
	}
}