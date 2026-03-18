using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static Corsairs.Platform.Msgpack.Parser.DataCodes;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with binary blobs
/// </summary>
public static partial class MsgPackSpec
{
	[UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) }, EntryPoint = "ExportAttrCallMsgPack")]
	public static int ExportFunctionApp()
	{
		return 1000;
	}

	/// <summary>
	///     Writes FixArray header into <paramref name="buffer" />.
	/// </summary>
	/// <remarks>
	///     DOES NOT check if <paramref name="length" /> is less or equal that <see cref="DataLengths.FixArrayMaxLength" />.
	///     If you need that check, use <see cref="TryWriteFixArrayHeader" />.
	/// </remarks>
	/// <param name="buffer">Buffer to write. Ensure that is at least 1 byte long.</param>
	/// <param name="length">Length of array. Should be less or equal to <see cref="DataLengths.FixArrayMaxLength" />.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" /></returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixArrayHeader(Span<byte> buffer, byte length)
	{
		buffer[0] = (byte)(FixArrayMin + length);
		return DataLengths.FixArrayHeader;
	}

	/// <summary>
	///     Tries to write FixArray header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of array. Should be less or equal to <see cref="DataLengths.FixArrayMaxLength" />.</param>
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
	///             <description><paramref name="length" /> is greater <see cref="DataLengths.FixArrayMaxLength" />.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixArrayHeader(Span<byte> buffer, byte length, out int wroteSize)
	{
		wroteSize = DataLengths.FixArrayHeader;
		if (length >= DataLengths.FixArrayMaxLength || buffer.Length < wroteSize) return false;
		buffer[0] = (byte)(FixArrayMin + length);
		return true;
	}

	/// <summary>
	///     Writes Array16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 3 bytes long.</param>
	/// <param name="length">Length of array.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteArray16Header(Span<byte> buffer, ushort length)
	{
		BinaryPrimitives.WriteUInt16BigEndian(buffer.Slice(1), length);
		buffer[0] = Array16;
		return DataLengths.Array16Header;
	}

	/// <summary>
	///     Tries to write Array16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of array.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteArray16Header(Span<byte> buffer, ushort length, out int wroteSize)
	{
		wroteSize = DataLengths.Array16Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = Array16;
		return BinaryPrimitives.TryWriteUInt16BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Writes Array32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 5 bytes long.</param>
	/// <param name="length">Length of array.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteArray32Header(Span<byte> buffer, uint length)
	{
		BinaryPrimitives.WriteUInt32BigEndian(buffer.Slice(1), length);
		buffer[0] = Array32;
		return DataLengths.Array32Header;
	}

	/// <summary>
	///     Tries to write Array32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of array.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteArray32Header(Span<byte> buffer, uint length, out int wroteSize)
	{
		wroteSize = DataLengths.Array32Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = Array32;
		return BinaryPrimitives.TryWriteUInt32BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Reads FixArray header and return length of that array.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of array</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static byte ReadFixArrayHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.FixArrayHeader;
		if (FixArrayMin <= buffer[0] && buffer[0] <= FixArrayMax)
			return (byte)(buffer[0] - FixArrayMin);

		return ThrowWrongRangeCodeException(buffer[0], FixArrayMin, FixArrayMax);
	}

	/// <summary>
	///     Tries to read FixArray header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of array. Should be less or equal to <see cref="DataLengths.FixArrayMaxLength" />.</param>
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
	///             <description><paramref name="length" /> is greater <see cref="DataLengths.FixArrayMaxLength" />.</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixArrayHeader(ReadOnlySpan<byte> buffer, out byte length, out int readSize)
	{
		readSize = DataLengths.FixArrayHeader;
		length = 0;
		if (buffer.Length < readSize) return false;
		var result = FixArrayMin <= buffer[0] && buffer[0] <= FixArrayMax;
		length = (byte)(buffer[0] - FixArrayMin);
		return result;
	}

	/// <summary>
	///     Reads Array16 header and return length of that array.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of array</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ushort ReadArray16Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Array16Header;
		if (buffer[0] == Array16)
			return BinaryPrimitives.ReadUInt16BigEndian(buffer.Slice(1));
		return ThrowWrongCodeException(buffer[0], Array16);
	}

	/// <summary>
	///     Tries to read Array16 header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of array.</param>
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
	public static bool TryReadArray16Header(ReadOnlySpan<byte> buffer, out ushort length, out int readSize)
	{
		readSize = DataLengths.Array16Header;
		length = 0;
		if (buffer.Length < readSize) return false;
		return buffer[0] == Array16 && BinaryPrimitives.TryReadUInt16BigEndian(buffer.Slice(1), out length);
	}

	/// <summary>
	///     Reads Array32 header and return length of that array.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of array</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadArray32Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Array32Header;
		if (buffer[0] == Array32)
			return BinaryPrimitives.ReadUInt32BigEndian(buffer.Slice(1));
		return ThrowWrongCodeException(buffer[0], Array32);
	}

	/// <summary>
	///     Tries to read Array32 header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="length">Length of array.</param>
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
	public static bool TryReadArray32Header(ReadOnlySpan<byte> buffer, out uint length, out int readSize)
	{
		readSize = DataLengths.Array32Header;
		length = 0;
		if (buffer.Length < readSize) return false;
		return buffer[0] == Array32 && BinaryPrimitives.TryReadUInt32BigEndian(buffer.Slice(1), out length);
	}

	/// <summary>
	///     Writes smallest possible array header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Depends on <paramref name="length" /> you'll need 1, 3 or 5 bytes in it.</param>
	/// <param name="length">Length of array.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteArrayHeader(Span<byte> buffer, int length)
	{
		if (length < 0) return ThrowLengthShouldBeNonNegative(length);

		if (length <= DataLengths.FixArrayMaxLength) return WriteFixArrayHeader(buffer, (byte)length);

		if (length <= ushort.MaxValue) return WriteArray16Header(buffer, (ushort)length);

		return WriteArray32Header(buffer, (uint)length);
	}

	/// <summary>
	///     Tries to write smallest possible array header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of array.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteArrayHeader(Span<byte> buffer, int length, out int wroteSize)
	{
		if (length < 0)
		{
			wroteSize = 0;
			return false;
		}

		if (length <= DataLengths.FixArrayMaxLength) return TryWriteFixArrayHeader(buffer, (byte)length, out wroteSize);

		if (length <= ushort.MaxValue) return TryWriteArray16Header(buffer, (ushort)length, out wroteSize);

		return TryWriteArray32Header(buffer, (uint)length, out wroteSize);
	}

	/// <summary>
	///     Reads array header and return length of that array.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" /></param>
	/// <returns>Length of array</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int ReadArrayHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		if (FixArrayMin <= buffer[0] && buffer[0] <= FixArrayMax) return ReadFixArrayHeader(buffer, out readSize);

		if (buffer[0] == Array16) return ReadArray16Header(buffer, out readSize);

		if (buffer[0] == Array32)
		{
			var uint32Value = ReadArray32Header(buffer, out readSize);
			if (uint32Value <= int.MaxValue) return (int)uint32Value;

			return ThrowDataIsTooLarge(uint32Value);
		}

		readSize = 0;
		return ThrowWrongArrayHeader(buffer[0]);
	}

	/// <summary>
	///     Tries to read array header from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read.</param>
	/// <param name="dataLength">Length of array.</param>
	/// <param name="headerLength">
	///     Count of bytes read from <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
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
	///             <description>length of array is greater than <see cref="int.MaxValue" /></description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadArrayHeader(ReadOnlySpan<byte> buffer, out int dataLength, out int headerLength)
	{
		if (TryReadFixArrayHeader(buffer, out var byteValue, out headerLength))
		{
			dataLength = byteValue;
			return true;
		}

		if (TryReadArray16Header(buffer, out var uint16Value, out headerLength))
		{
			dataLength = uint16Value;
			return true;
		}

		if (TryReadArray32Header(buffer, out var uint32Value, out headerLength))
			// .net array size limitation
			// https://blogs.msdn.microsoft.com/joshwil/2005/08/10/bigarrayt-getting-around-the-2gb-array-size-limit/
			if (uint32Value <= int.MaxValue)
			{
				dataLength = (int)uint32Value;
				return true;
			}

		dataLength = 0;
		return false;
	}
}