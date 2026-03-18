using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with signed int 32
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write int32 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixInt32(Span<byte> buffer, int value)
	{
		BinaryPrimitives.WriteInt32BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.Int32;
		return DataLengths.Int32;
	}

	/// <summary>
	///     Tries to write int32 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixInt32(Span<byte> buffer, int value, out int wroteSize)
	{
		wroteSize = DataLengths.Int32;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.Int32;
		return BinaryPrimitives.TryWriteInt32BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads int32 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int ReadFixInt32(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Int32;
		if (buffer[0] != DataCodes.Int32) return ThrowWrongCodeException(buffer[0], DataCodes.Int32);
		return BinaryPrimitives.ReadInt32BigEndian(buffer.Slice(1));
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.Int32" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixInt32(ReadOnlySpan<byte> buffer, out int value, out int readSize)
	{
		readSize = DataLengths.Int32;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.Int32;
		return BinaryPrimitives.TryReadInt32BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, int value)
	{
		if (value >= 0) return WriteInt64(buffer, (uint)value);
		if (value >= DataCodes.FixNegativeMinSByte) return WriteNegativeFixInt(buffer, (sbyte)value);
		if (value >= sbyte.MinValue) return WriteFixInt8(buffer, (sbyte)value);
		if (value >= short.MinValue) return WriteFixInt16(buffer, (short)value);
		return WriteFixInt32(buffer, value);
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
	public static bool TryWriteInt64(Span<byte> buffer, int value, out int wroteSize)
	{
		if (value >= 0) return TryWriteUInt32(buffer, (uint)value, out wroteSize);
		if (value < short.MinValue) return TryWriteFixInt32(buffer, value, out wroteSize);
		if (value < sbyte.MinValue) return TryWriteFixInt16(buffer, (short)value, out wroteSize);
		return TryWriteInt8(buffer, (sbyte)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="int" /> values from <paramref name="buffer" />
	/// </summary>
	public static int ReadInt32(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer.IsEmpty)
		{
			readSize = 0;
			return ThrowCantReadEmptyBufferException();
		}

		var code = buffer[0];

		switch (code)
		{
			case DataCodes.Int32:
				return ReadFixInt32(buffer, out readSize);

			case DataCodes.Int16:
				return ReadFixInt16(buffer, out readSize);

			case DataCodes.Int8:
				return ReadFixInt8(buffer, out readSize);

			case DataCodes.UInt32:
				var value = ReadFixUInt32(buffer, out readSize);
				if (value > int.MaxValue) return ThrowValueIsTooLargeException(value, int.MaxValue);
				return (int)value;

			case DataCodes.UInt16:
				return ReadFixUInt16(buffer, out readSize);

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		if (TryReadNegativeFixInt(buffer, out var negative, out readSize)) return negative;

		return ThrowWrongIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.Int32, DataCodes.UInt8,
			DataCodes.UInt16, DataCodes.UInt32);
	}

	/// <summary>
	///     Tries to read <see cref="int" /> value from <paramref name="buffer" />.
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
	public static bool TryReadInt32(ReadOnlySpan<byte> buffer, out int value, out int readSize)
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
			case DataCodes.Int32:
				return TryReadFixInt32(buffer, out value, out readSize);

			case DataCodes.Int16:
				result = TryReadFixInt16(buffer, out var int16, out readSize);
				value = int16;
				return result;

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out readSize);
				value = int8;
				return result;

			case DataCodes.UInt32:
				result = TryReadFixUInt32(buffer, out var uint32, out readSize) && uint32 <= int.MaxValue;
				value = result ? (int)uint32 : default;
				return result;

			case DataCodes.UInt16:
				result = TryReadFixUInt16(buffer, out var uint16, out readSize);
				value = uint16;
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