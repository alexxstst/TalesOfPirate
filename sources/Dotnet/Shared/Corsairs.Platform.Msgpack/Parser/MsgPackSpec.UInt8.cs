using System;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with unsigned int 8
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write uint8 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixUInt8(Span<byte> buffer, byte value)
	{
		buffer[1] = value;
		buffer[0] = DataCodes.UInt8;
		return DataLengths.UInt8;
	}

	/// <summary>
	///     Tries to write uint8 <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixUInt8(Span<byte> buffer, byte value, out int wroteSize)
	{
		wroteSize = DataLengths.UInt8;
		if (buffer.Length < wroteSize) return false;
		buffer[1] = value;
		buffer[0] = DataCodes.UInt8;
		return true;
	}

	/// <summary>
	///     Reads uint8 from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static byte ReadFixUInt8(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.UInt8;
		if (buffer[0] != DataCodes.UInt8) return ThrowWrongCodeException(buffer[0], DataCodes.UInt8);
		return buffer[1];
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
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.UInt8" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixUInt8(ReadOnlySpan<byte> buffer, out byte value, out int readSize)
	{
		readSize = DataLengths.UInt8;
		value = default;
		if (buffer.Length < readSize) return false;
		var result = buffer[0] == DataCodes.UInt8;
		value = result ? buffer[1] : default;
		return result;
	}

	/// <summary>
	///     Write smallest possible representation of <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>See https://github.com/msgpack/msgpack/issues/164 on data code selection.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteInt64(Span<byte> buffer, byte value)
	{
		if (value <= DataCodes.FixPositiveMax) return WritePositiveFixInt(buffer, value);
		return WriteFixUInt8(buffer, value);
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
	public static bool TryWriteUInt8(Span<byte> buffer, byte value, out int wroteSize)
	{
		if (TryWritePositiveFixInt(buffer, value, out wroteSize))
			return true;
		return TryWriteFixUInt8(buffer, value, out wroteSize);
	}

	/// <summary>
	///     Read <see cref="byte" /> values from <paramref name="buffer" />
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static byte ReadUInt8(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (buffer[0] == DataCodes.UInt8) return ReadFixUInt8(buffer, out readSize);
		return ReadPositiveFixInt(buffer, out readSize);
	}

	/// <summary>
	///     Tries to read <see cref="byte" /> value from <paramref name="buffer" />.
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
	public static bool TryReadUInt8(ReadOnlySpan<byte> buffer, out byte value, out int readSize)
	{
		if (TryReadFixUInt8(buffer, out value, out readSize))
			return true;
		return TryReadPositiveFixInt(buffer, out value, out readSize);
	}
}