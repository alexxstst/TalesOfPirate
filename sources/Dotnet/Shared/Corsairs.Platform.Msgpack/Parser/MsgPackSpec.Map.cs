using System;
using System.Runtime.CompilerServices;
using static Corsairs.Platform.Msgpack.Parser.DataCodes;
using static System.Buffers.Binary.BinaryPrimitives;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with maps (dictionaries)
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Writes FixMap header into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>
	///     DOES NOT check if <paramref name="length" /> is less or equal that <see cref="DataLengths.FixMapMaxLength" />.
	///     If you need that check, use <see cref="TryWriteFixMapHeader" />.
	/// </remarks>
	/// <param name="buffer">Buffer to write. Ensure that is at least 1 byte long.</param>
	/// <param name="length">Length of map. Should be less or equal to <see cref="DataLengths.FixMapMaxLength" />.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" /></returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixMapHeader(Span<byte> buffer, byte length)
	{
		buffer[0] = (byte)(FixMapMin + length);
		return DataLengths.FixMapHeader;
	}

	/// <summary>
	///     Tries to write FixMap header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of map. Should be less or equal to <see cref="DataLengths.FixMapMaxLength" />.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="length" /> is greater <see cref="DataLengths.FixMapMaxLength" />.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixMapHeader(Span<byte> buffer, byte length, out int wroteSize)
	{
		wroteSize = DataLengths.FixMapHeader;
		if (buffer.Length < wroteSize) return false;
		if (length < DataLengths.FixMapMaxLength) return false;
		buffer[0] = (byte)(FixMapMin + length);
		return true;
	}

	/// <summary>
	///     Writes Map16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 3 bytes long.</param>
	/// <param name="length">Length of map.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteMap16Header(Span<byte> buffer, ushort length)
	{
		WriteUInt16BigEndian(buffer.Slice(1), length);
		buffer[0] = Map16;
		return DataLengths.Map16Header;
	}

	/// <summary>
	///     Tries to write Map16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteMap16Header(Span<byte> buffer, ushort length, out int wroteSize)
	{
		wroteSize = DataLengths.Map16Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = Map16;
		return TryWriteUInt16BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Writes Map32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 5 bytes long.</param>
	/// <param name="length">Length of map.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteMap32Header(Span<byte> buffer, uint length)
	{
		WriteUInt32BigEndian(buffer.Slice(1), length);
		buffer[0] = Map32;
		return DataLengths.Map32Header;
	}

	/// <summary>
	///     Tries to write Map32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteMap32Header(Span<byte> buffer, uint length, out int wroteSize)
	{
		wroteSize = DataLengths.Map32Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = Map32;
		return TryWriteUInt32BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Reads FixMap header and return length of that map.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of map</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static byte ReadFixMapHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.FixMapHeader;
		if (FixMapMin <= buffer[0] && buffer[0] <= FixMapMax)
			return (byte)(buffer[0] - FixMapMin);

		return ThrowWrongRangeCodeException(buffer[0], FixMapMin, FixMapMax);
	}

	/// <summary>
	///     Tries to read FixMap header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of map. Should be less or equal to <see cref="DataLengths.FixMapMaxLength" />.</param>
	/// <param name="readSize">
	///     Count of bytes read from <paramref name="buffer" />. If return value is <c>false</c>, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="length" /> is greater <see cref="DataLengths.FixMapMaxLength" />.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixMapHeader(ReadOnlySpan<byte> buffer, out byte length, out int readSize)
	{
		readSize = DataLengths.FixMapHeader;
		length = default;
		if (buffer.Length < readSize) return false;
		var result = FixMapMin <= buffer[0] && buffer[0] <= FixMapMax;
		length = (byte)(buffer[0] - FixMapMin);
		return result;
	}

	/// <summary>
	///     Reads Map16 header and return length of that map.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of map</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ushort ReadMap16Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Map16Header;
		return ReadUInt16BigEndian(buffer.Slice(1));
	}

	/// <summary>
	///     Tries to read Map16 header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="readSize">
	///     Count of bytes read from <paramref name="buffer" />. If return value is <c>false</c>, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadMap16Header(ReadOnlySpan<byte> buffer, out ushort length, out int readSize)
	{
		readSize = DataLengths.Map16Header;
		length = default;
		if (buffer.Length < readSize) return false;
		return TryReadUInt16BigEndian(buffer.Slice(1), out length) && buffer[0] == Map16;
	}

	/// <summary>
	///     Reads Map32 header and return length of that map.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of map</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadMap32Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Map32Header;
		return ReadUInt32BigEndian(buffer.Slice(1));
	}

	/// <summary>
	///     Tries to read Map32 header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="readSize">
	///     Count of bytes read from <paramref name="buffer" />. If return value is <c>false</c>, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadMap32Header(ReadOnlySpan<byte> buffer, out uint length, out int readSize)
	{
		readSize = DataLengths.Map32Header;
		length = default;
		if (buffer.Length < readSize) return false;
		return TryReadUInt32BigEndian(buffer.Slice(1), out length) && buffer[0] == Map32;
	}

	/// <summary>
	///     Writes smallest possible map header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Depends on <paramref name="length" /> you'll need 1, 3 or 5 bytes in it.</param>
	/// <param name="length">Length of map.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteMapHeader(Span<byte> buffer, int length)
	{
		if (length < 0) return ThrowLengthShouldBeNonNegative(length);

		if (length <= DataLengths.FixMapMaxLength) return WriteFixMapHeader(buffer, (byte)length);

		if (length <= ushort.MaxValue) return WriteMap16Header(buffer, (ushort)length);

		return WriteMap32Header(buffer, (uint)length);
	}

	/// <summary>
	///     Tries to write smallest possible map header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteMapHeader(Span<byte> buffer, int length, out int wroteSize)
	{
		if (length < 0)
		{
			wroteSize = 0;
			return false;
		}

		if (length <= DataLengths.FixMapMaxLength) return TryWriteFixMapHeader(buffer, (byte)length, out wroteSize);

		if (length <= ushort.MaxValue) return TryWriteMap16Header(buffer, (ushort)length, out wroteSize);

		return TryWriteMap32Header(buffer, (uint)length, out wroteSize);
	}

	/// <summary>
	///     Reads map header and return length of that map.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of map</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadMapHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		switch (buffer[0])
		{
			case Map16:
				return ReadMap16Header(buffer, out readSize);
			case Map32:
				return ReadMap32Header(buffer, out readSize);
		}

		if (FixMapMin <= buffer[0] && buffer[0] <= FixMapMax)
			return ReadFixMapHeader(buffer, out readSize);

		ThrowWrongMapHeader(buffer[0]);
		readSize = 0;
		return default;
	}

	/// <summary>
	///     Tries to read map header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of map.</param>
	/// <param name="readSize">
	///     Count of bytes read from <paramref name="buffer" />. If return value is <c>false</c>, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description>length of map is greater than <see cref="int.MaxValue" /></description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadMapHeader(ReadOnlySpan<byte> buffer, out int length, out int readSize)
	{
		if (TryReadFixMapHeader(buffer, out var byteValue, out readSize))
		{
			length = byteValue;
			return true;
		}

		if (TryReadMap16Header(buffer, out var uint16Value, out readSize))
		{
			length = uint16Value;
			return true;
		}

		if (TryReadMap32Header(buffer, out var uint32Value, out readSize))
			// .net array size limitation
			// https://blogs.msdn.microsoft.com/joshwil/2005/08/10/bigarrayt-getting-around-the-2gb-map-size-limit/
			if (uint32Value <= int.MaxValue)
			{
				length = (int)uint32Value;
				return true;
			}

		length = 0;
		return false;
	}
}