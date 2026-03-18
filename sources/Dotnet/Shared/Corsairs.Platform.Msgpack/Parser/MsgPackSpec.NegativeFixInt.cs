using System;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with NegativeFixInt
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write fix negative int <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteNegativeFixInt(Span<byte> buffer, sbyte value)
	{
		if (value < DataCodes.FixNegativeMinSByte || value > DataCodes.FixNegativeMaxSByte)
			ThrowWrongRangeCodeException(value, DataCodes.FixNegativeMinSByte, DataCodes.FixNegativeMaxSByte);
		buffer[0] = unchecked((byte)value);
		return DataLengths.NegativeFixInt;
	}

	/// <summary>
	///     Tries to write fix negative int <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteNegativeFixInt(Span<byte> buffer, sbyte value, out int wroteSize)
	{
		wroteSize = DataLengths.NegativeFixInt;
		if (buffer.Length < wroteSize) return false;
		if (value < DataCodes.FixNegativeMinSByte || value > DataCodes.FixNegativeMaxSByte) return false;

		buffer[0] = unchecked((byte)value);
		return true;
	}

	/// <summary>
	///     Reads negative fix int from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Read value</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadNegativeFixInt(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.NegativeFixInt;
		var result = unchecked((sbyte)buffer[0]);
		if (result >= DataCodes.FixNegativeMinSByte && result <= DataCodes.FixNegativeMaxSByte)
			return result;
		ThrowWrongRangeCodeException(result, DataCodes.FixNegativeMinSByte, DataCodes.FixNegativeMaxSByte);
		return result;
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
	///     <paramref name="buffer" />[0]
	///     is not between <see cref="DataCodes.FixNegativeMinSByte" /> and <see cref="DataCodes.FixNegativeMaxSByte" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadNegativeFixInt(ReadOnlySpan<byte> buffer, out sbyte value, out int readSize)
	{
		readSize = DataLengths.NegativeFixInt;
		value = default;
		if (buffer.Length < readSize) return false;
		value = unchecked((sbyte)buffer[0]);
		return DataCodes.FixNegativeMinSByte <= value && value <= DataCodes.FixNegativeMaxSByte;
	}
}