using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with unsigned int 16
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write uint16 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixUInt16(Span<byte> buffer, ushort value)
	{
		BinaryPrimitives.WriteUInt16BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.UInt16;
		return DataLengths.UInt16;
	}

	/// <summary>
	///     Tries to write uint16 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixUInt16(Span<byte> buffer, ushort value, out int wroteSize)
	{
		wroteSize = DataLengths.UInt16;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.UInt16;
		return BinaryPrimitives.TryWriteUInt16BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads uint16 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ushort ReadFixUInt16(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.UInt16;
		if (buffer[0] != DataCodes.UInt16) ThrowWrongCodeException(buffer[0], DataCodes.UInt16);
		return BinaryPrimitives.ReadUInt16BigEndian(buffer.Slice(1));
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.UInt16" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixUInt16(ReadOnlySpan<byte> buffer, out ushort value, out int readSize)
	{
		readSize = DataLengths.UInt16;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.UInt16;
		return BinaryPrimitives.TryReadUInt16BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, ushort value)
	{
		if (value <= DataCodes.FixPositiveMax) return WritePositiveFixInt(buffer, (byte)value);
		if (value <= byte.MaxValue) return WriteFixUInt8(buffer, (byte)value);
		return WriteFixUInt16(buffer, value);
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
	public static bool TryWriteUInt16(Span<byte> buffer, ushort value, out int wroteSize)
	{
		if (value > byte.MaxValue) return TryWriteFixUInt16(buffer, value, out wroteSize);
		return TryWriteUInt8(buffer, (byte)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="ushort" /> values from <paramref name="buffer" />
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ushort ReadUInt16(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer.IsEmpty) ThrowCantReadEmptyBufferException();
		var code = buffer[0];

		switch (code)
		{
			case DataCodes.Int16:
				var int16 = ReadFixInt32(buffer, out readSize);
				if (int16 < 0) ThrowUnsignedIntException(int16);
				return (ushort)int16;

			case DataCodes.Int8:
				var int8 = ReadFixInt32(buffer, out readSize);
				if (int8 < 0) ThrowUnsignedIntException(int8);
				return (ushort)int8;

			case DataCodes.UInt16:
				return ReadFixUInt16(buffer, out readSize);

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		return ThrowWrongUIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.UInt8, DataCodes.UInt16,
			DataCodes.UInt32);
	}

	/// <summary>
	///     Tries to read <see cref="ushort" /> value from <paramref name="buffer" />.
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
	public static bool TryReadUInt16(ReadOnlySpan<byte> buffer, out ushort value, out int readSize)
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
			case DataCodes.UInt16:
				return TryReadFixUInt16(buffer, out value, out readSize);

			case DataCodes.UInt8:
				result = TryReadFixUInt8(buffer, out var uint8, out readSize);
				value = uint8;
				return result;

			case DataCodes.Int16:
				result = TryReadFixInt16(buffer, out var int16, out readSize) && int16 >= 0;
				value = result ? (ushort)int16 : default;
				return result;

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out readSize) && int8 >= 0;
				value = result ? (ushort)int8 : default;
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