using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with unsigned int 32
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write uint32 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixUInt32(Span<byte> buffer, uint value)
	{
		BinaryPrimitives.WriteUInt32BigEndian(buffer.Slice(1), value);
		buffer[0] = DataCodes.UInt32;
		return DataLengths.UInt32;
	}

	/// <summary>
	///     Tries to write uint32 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixUInt32(Span<byte> buffer, uint value, out int wroteSize)
	{
		wroteSize = DataLengths.UInt32;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.UInt32;
		return BinaryPrimitives.TryWriteUInt32BigEndian(buffer.Slice(1), value);
	}

	/// <summary>
	///     Reads uint32 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadFixUInt32(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.UInt32;
		if (buffer[0] != DataCodes.UInt32) return ThrowWrongCodeException(buffer[0], DataCodes.UInt32);
		return BinaryPrimitives.ReadUInt32BigEndian(buffer.Slice(1));
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.UInt32" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixUInt32(ReadOnlySpan<byte> buffer, out uint value, out int readSize)
	{
		readSize = DataLengths.UInt32;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.UInt32;
		return BinaryPrimitives.TryReadUInt32BigEndian(buffer.Slice(1), out value) && result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, uint value)
	{
		if (value <= DataCodes.FixPositiveMax) return WritePositiveFixInt(buffer, (byte)value);
		if (value <= byte.MaxValue) return WriteFixUInt8(buffer, (byte)value);
		if (value <= ushort.MaxValue) return WriteFixUInt16(buffer, (ushort)value);
		return WriteFixUInt32(buffer, value);
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
	public static bool TryWriteUInt32(Span<byte> buffer, uint value, out int wroteSize)
	{
		if (value > ushort.MaxValue) return TryWriteFixUInt32(buffer, value, out wroteSize);
		return TryWriteUInt16(buffer, (ushort)value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="uint" /> values from <paramref name="buffer" />
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadUInt32(ReadOnlySpan<byte> buffer, out int readSize)
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
				var int32 = ReadFixInt32(buffer, out readSize);
				if (int32 < 0) return ThrowUnsignedIntException(int32);
				return (uint)int32;

			case DataCodes.Int16:
				var int16 = ReadFixInt32(buffer, out readSize);
				if (int16 < 0) return ThrowUnsignedIntException(int16);
				return (uint)int16;

			case DataCodes.Int8:
				var int8 = ReadFixInt32(buffer, out readSize);
				if (int8 < 0) return ThrowUnsignedIntException(int8);
				return (uint)int8;

			case DataCodes.UInt32:
				return ReadFixUInt32(buffer, out readSize);

			case DataCodes.UInt16:
				return ReadFixUInt16(buffer, out readSize);

			case DataCodes.UInt8:
				return ReadFixUInt8(buffer, out readSize);
		}

		if (TryReadPositiveFixInt(buffer, out var positive, out readSize)) return positive;

		return ThrowWrongUIntCodeException(code, DataCodes.Int8, DataCodes.Int16, DataCodes.Int32, DataCodes.UInt8,
			DataCodes.UInt16, DataCodes.UInt32);
	}

	/// <summary>
	///     Tries to read <see cref="uint" /> value from <paramref name="buffer" />.
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
	public static bool TryReadUInt32(ReadOnlySpan<byte> buffer, out uint value, out int readSize)
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
			case DataCodes.UInt32:
				return TryReadFixUInt32(buffer, out value, out readSize);

			case DataCodes.UInt16:
				result = TryReadFixUInt16(buffer, out var uint16, out readSize);
				value = uint16;
				return result;

			case DataCodes.UInt8:
				result = TryReadFixUInt8(buffer, out var uint8, out readSize);
				value = uint8;
				return result;

			case DataCodes.Int32:
				result = TryReadFixInt32(buffer, out var int32, out readSize) && int32 >= 0;
				value = result ? (uint)int32 : default;
				return result;

			case DataCodes.Int16:
				result = TryReadFixInt16(buffer, out var int16, out readSize) && int16 >= 0;
				value = result ? (uint)int16 : default;
				return result;

			case DataCodes.Int8:
				result = TryReadFixInt8(buffer, out var int8, out readSize) && int8 >= 0;
				value = result ? (uint)int8 : default;
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