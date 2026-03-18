using System;
using System.Buffers;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with binary blobs
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Writes Binary8 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 2 bytes long.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary8Header(Span<byte> buffer, byte length)
	{
		buffer[1] = length;
		buffer[0] = DataCodes.Binary8;
		return DataLengths.Binary8Header;
	}

	/// <summary>
	///     Tries to write Binary8 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary8Header(Span<byte> buffer, byte length, out int wroteSize)
	{
		wroteSize = DataLengths.Binary8Header;
		if (buffer.Length < wroteSize) return false;
		buffer[1] = length;
		buffer[0] = DataCodes.Binary8;
		return true;
	}

	/// <summary>
	///     Writes Binary16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 3 bytes long.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary16Header(Span<byte> buffer, ushort length)
	{
		BinaryPrimitives.WriteUInt16BigEndian(buffer.Slice(1), length);
		buffer[0] = DataCodes.Binary16;
		return DataLengths.Binary16Header;
	}

	/// <summary>
	///     Tries to write Binary16 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary16Header(Span<byte> buffer, ushort length, out int wroteSize)
	{
		wroteSize = DataLengths.Binary16Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.Binary16;
		return BinaryPrimitives.TryWriteUInt16BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Writes Binary32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Ensure that is at least 5 bytes long.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary32Header(Span<byte> buffer, uint length)
	{
		BinaryPrimitives.WriteUInt32BigEndian(buffer.Slice(1), length);
		buffer[0] = DataCodes.Binary32;
		return DataLengths.Binary32Header;
	}

	/// <summary>
	///     Tries to write Binary32 header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary32Header(Span<byte> buffer, uint length, out int wroteSize)
	{
		wroteSize = DataLengths.Binary32Header;
		if (buffer.Length < wroteSize) return false;
		buffer[0] = DataCodes.Binary32;
		return BinaryPrimitives.TryWriteUInt32BigEndian(buffer.Slice(1), length);
	}

	/// <summary>
	///     Reads binary8 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from buffer</param>
	/// <returns>Binary length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static byte ReadBinary8Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Binary8Header;
		if (buffer[0] == DataCodes.Binary8)
			return buffer[1];

		return ThrowWrongCodeException(buffer[0], DataCodes.Binary8);
	}

	/// <summary>
	///     Reads binary8 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="length">Binary length.</param>
	/// <param name="readSize">Count of bytes read from buffer.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" /> is too small</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary8Header(ReadOnlySpan<byte> buffer, out byte length, out int readSize)
	{
		readSize = DataLengths.Binary8Header;
		length = 0;
		if (buffer.Length < readSize) return false;
		length = buffer[1];
		return buffer[0] == DataCodes.Binary8;
	}

	/// <summary>
	///     Reads binary16 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from buffer</param>
	/// <returns>Binary length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ushort ReadBinary16Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Binary16Header;
		if (buffer[0] == DataCodes.Binary16)
			return BinaryPrimitives.ReadUInt16BigEndian(buffer.Slice(1));

		return ThrowWrongCodeException(buffer[0], DataCodes.Binary16);
	}

	/// <summary>
	///     Reads binary16 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="length">Binary length.</param>
	/// <param name="readSize">Count of bytes read from buffer.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" /> is too small</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary16Header(ReadOnlySpan<byte> buffer, out ushort length, out int readSize)
	{
		readSize = DataLengths.Binary16Header;
		length = 0;
		return buffer.Length >= readSize
			   && buffer[0] == DataCodes.Binary16
			   && BinaryPrimitives.TryReadUInt16BigEndian(buffer.Slice(1), out length);
	}

	/// <summary>
	///     Reads binary32 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from buffer</param>
	/// <returns>Binary length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static uint ReadBinary32Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Binary32Header;
		if (buffer[0] == DataCodes.Binary32)
			return BinaryPrimitives.ReadUInt32BigEndian(buffer.Slice(1));

		return ThrowWrongCodeException(buffer[0], DataCodes.Binary32);
	}

	/// <summary>
	///     Reads binary32 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="dataLength">Binary length.</param>
	/// <param name="headerLength">Count of bytes read from buffer.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" /> is too small</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary32Header(ReadOnlySpan<byte> buffer, out uint dataLength, out int headerLength)
	{
		headerLength = DataLengths.Binary32Header;
		dataLength = 0;
		return buffer.Length >= headerLength
			   && buffer[0] == DataCodes.Binary32
			   && BinaryPrimitives.TryReadUInt32BigEndian(buffer.Slice(1), out dataLength);
	}

	/// <summary>
	///     Writes smallest possible binary header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Depending on <paramref name="length" /> you'll need 2, 3 or 5 bytes in buffer.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinaryHeader(Span<byte> buffer, int length)
	{
		if (length < 0) return ThrowLengthShouldBeNonNegative(length);

		if (length <= byte.MaxValue) return WriteBinary8Header(buffer, (byte)length);

		if (length <= ushort.MaxValue) return WriteBinary16Header(buffer, (ushort)length);

		return WriteBinary32Header(buffer, (uint)length);
	}

	/// <summary>
	///     Tries to write smallest possible binary header into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="length">Length of binary blob.</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <see cref="length" /> is below zero.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinaryHeader(Span<byte> buffer, int length, out int wroteSize)
	{
		if (length < 0)
		{
			wroteSize = 0;
			return false;
		}

		if (length <= byte.MaxValue) return TryWriteBinary8Header(buffer, (byte)length, out wroteSize);

		if (length <= ushort.MaxValue) return TryWriteBinary16Header(buffer, (ushort)length, out wroteSize);

		return TryWriteBinary32Header(buffer, (uint)length, out wroteSize);
	}

	/// <summary>
	///     Reads binary header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from buffer</param>
	/// <returns>Binary length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int ReadBinaryHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		switch (buffer[0])
		{
			case DataCodes.Binary8:
				return ReadBinary8Header(buffer, out readSize);
			case DataCodes.Binary16:
				return ReadBinary16Header(buffer, out readSize);
			case DataCodes.Binary32:
				var uint32Value = ReadBinary32Header(buffer, out readSize);
				if (uint32Value <= int.MaxValue) return (int)uint32Value;

				return ThrowDataIsTooLarge(uint32Value);
			default:
				readSize = 0;
				return ThrowWrongCodeException(buffer[0], DataCodes.Binary8, DataCodes.Binary16, DataCodes.Binary32);
		}
	}

	/// <summary>
	///     Reads binary8 header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="length">Binary length.</param>
	/// <param name="readSize">Count of bytes read from buffer.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" />[0] contains wrong data code or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" /> is too small</description>
	///         </item>
	///     </list>
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinaryHeader(ReadOnlySpan<byte> buffer, out uint length, out int readSize)
	{
		length = 0;

		if (TryReadBinary8Header(buffer, out var byteValue, out readSize))
		{
			length = byteValue;
			return true;
		}

		if (TryReadBinary16Header(buffer, out var uint16Value, out readSize))
		{
			length = uint16Value;
			return true;
		}

		return TryReadBinary32Header(buffer, out length, out readSize);
	}

	/// <summary>
	///     Writes Binary8 into <paramref name="buffer" />. Overhead is 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob. Should be shorter or equal to <see cref="byte.MaxValue" />.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary8(Span<byte> buffer, ReadOnlySpan<byte> binary)
	{
		var length = binary.Length;
		if (length > byte.MaxValue)
			return ThrowDataIsTooLarge(binary.Length, byte.MaxValue, nameof(DataCodes.Binary8), DataCodes.Binary8);

		var result = WriteBinary8Header(buffer, (byte)length);
		binary.CopyTo(buffer.Slice(result));
		return result + length;
	}

	/// <summary>
	///     Tries to write Binary8 into <paramref name="buffer" />. Overhead is 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob. Should be shorter or equal to <see cref="byte.MaxValue" />.</param>
	/// <param name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too short or
	///     <paramref name="binary" /> is too long.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary8(Span<byte> buffer, ReadOnlySpan<byte> binary, out int wroteSize)
	{
		var length = binary.Length;
		if (length > byte.MaxValue)
		{
			wroteSize = 0;
			return false;
		}

		if (TryWriteBinary8Header(buffer, (byte)length, out wroteSize))
			if (binary.TryCopyTo(buffer.Slice(wroteSize)))
			{
				wroteSize += length;
				return true;
			}

		return false;
	}

	/// <summary>
	///     Writes Binary16 into <paramref name="buffer" />. Overhead is 3 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob. Should be shorter or equal to <see cref="ushort.MaxValue" />.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary16(Span<byte> buffer, ReadOnlySpan<byte> binary)
	{
		var length = binary.Length;
		if (length > ushort.MaxValue)
			return ThrowDataIsTooLarge(binary.Length, ushort.MaxValue, nameof(DataCodes.Binary16), DataCodes.Binary16);

		var result = WriteBinary16Header(buffer, (ushort)length);
		binary.CopyTo(buffer.Slice(result));
		return result + length;
	}

	/// <summary>
	///     Tries to write Binary16 into <paramref name="buffer" />. Overhead is 3 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob. Should be shorter or equal to <see cref="ushort.MaxValue" />.</param>
	/// <param name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too short or
	///     <paramref name="binary" /> is too long.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary16(Span<byte> buffer, ReadOnlySpan<byte> binary, out int wroteSize)
	{
		var length = binary.Length;
		if (length > ushort.MaxValue)
		{
			wroteSize = 0;
			return false;
		}

		if (TryWriteBinary16Header(buffer, (ushort)length, out wroteSize))
			if (binary.TryCopyTo(buffer.Slice(wroteSize)))
			{
				wroteSize += length;
				return true;
			}

		return false;
	}

	/// <summary>
	///     Writes Binary32 into <paramref name="buffer" />. Overhead is 5 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob.</param>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary32(Span<byte> buffer, ReadOnlySpan<byte> binary)
	{
		var result = WriteBinary32Header(buffer, (uint)binary.Length);
		binary.CopyTo(buffer.Slice(result));
		return result + binary.Length;
	}

	/// <summary>
	///     Tries to write Binary32 into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Overhead is 5 bytes.</param>
	/// <param name="binary">Binary blob.</param>
	/// <param name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too short or
	///     <paramref name="binary" /> is too long.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary32(Span<byte> buffer, ReadOnlySpan<byte> binary, out int wroteSize)
	{
		var length = binary.Length;
		if (TryWriteBinary32Header(buffer, (uint)length, out wroteSize))
			if (binary.TryCopyTo(buffer.Slice(wroteSize)))
			{
				wroteSize += length;
				return true;
			}

		return false;
	}

	/// <summary>
	///     Writes binary with smallest possible overhead into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write. Overhead is 5 bytes.</param>
	/// <param name="binary">Binary blob.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too short or
	///     <paramref name="binary" /> is too long.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteBinary(Span<byte> buffer, ReadOnlySpan<byte> binary)
	{
		if (binary.Length <= byte.MaxValue) return WriteBinary8(buffer, binary);
		if (binary.Length <= ushort.MaxValue) return WriteBinary16(buffer, binary);
		return WriteBinary32(buffer, binary);
	}

	/// <summary>
	///     Tries to write binary with smallest possible overhead into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="binary">Binary blob.</param>
	/// <param name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too short or
	///     <paramref name="binary" /> is too long.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteBinary(Span<byte> buffer, ReadOnlySpan<byte> binary, out int wroteSize)
	{
		return TryWriteBinary8(buffer, binary, out wroteSize)
			   || TryWriteBinary16(buffer, binary, out wroteSize)
			   || TryWriteBinary32(buffer, binary, out wroteSize);
	}

	/// <summary>
	///     Read Binary8 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Length of result is guaranteed to be equal of read length.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReadOnlySpan<byte> ReadBinary8(ReadOnlySpan<byte> buffer, ref int readSize)
	{
		var resultLength = ReadBinary8Header(buffer, out readSize);
		return ReadBinaryBlob(buffer, ref readSize, resultLength);
	}

	/// <summary>
	///     Tries to read Binary8 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="result">Rented buffer from pool. If returned value is <c>false</c>, it will be null.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if not.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary8(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> result, ref int headerLength)
	{
		result = null;

		if (TryReadBinary8Header(buffer, out var byteLength, out headerLength))
			return TryReadBinary(buffer, out result, byteLength, ref headerLength);

		return false;
	}

	/// <summary>
	///     Read Binary16 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Length of result is guaranteed to be equal of read length.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReadOnlySpan<byte> ReadBinary16(ReadOnlySpan<byte> buffer, ref int headerLength)
	{
		var resultLength = ReadBinary8Header(buffer, out headerLength);
		return ReadBinaryBlob(buffer, ref headerLength, resultLength);
	}

	/// <summary>
	///     Tries to read Binary16 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="result">Rented buffer from pool. If returned value is <c>false</c>, it will be null.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if not.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary16(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> result, ref int headerLength)
	{
		result = null;

		if (TryReadBinary16Header(buffer, out var ushortLength, out headerLength))
			return TryReadBinary(buffer, out result, ushortLength, ref headerLength);

		return false;
	}

	/// <summary>
	///     Read Binary32 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Length of result is guaranteed to be equal of read length.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReadOnlySpan<byte> ReadBinary32(ReadOnlySpan<byte> buffer, ref int headerLength)
	{
		var length = ReadBinary32Header(buffer, out headerLength);
		if (length > int.MaxValue) ThrowDataIsTooLarge(length);
		var resultLength = (int)length;
		return ReadBinaryBlob(buffer, ref headerLength, resultLength);
	}

	/// <summary>
	///     Tries to read Binary32 into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="result">Rented buffer from pool. If returned value is <c>false</c>, it will be null.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if not.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary32(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> result, ref int headerLength)
	{
		result = null;

		if (TryReadBinary32Header(buffer, out var dataLength, out headerLength) && dataLength <= int.MaxValue)
			return TryReadBinary(buffer, out result, (int)dataLength, ref headerLength);

		return false;
	}

	/// <summary>
	///     Read binary into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Length of result is guaranteed to be equal of read length.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReadOnlySpan<byte> ReadBinary(ReadOnlySpan<byte> buffer, ref int headerLength)
	{
		var dataLength = ReadBinaryHeader(buffer, out headerLength);
		return ReadBinaryBlob(buffer, ref headerLength, dataLength);
	}

	/// <summary>
	///     Tries to read binary into buffer, rented from <see cref="MemoryPool{T}.Shared" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="result">Rented buffer from pool. If returned value is <c>false</c>, it will be null.</param>
	/// <param name="headerLength">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if not.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadBinary(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> result, ref int headerLength)
	{
		return TryReadBinary8(buffer, out result, ref headerLength)
			   || TryReadBinary16(buffer, out result, ref headerLength)
			   || TryReadBinary32(buffer, out result, ref headerLength);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static ReadOnlySpan<byte> ReadBinaryBlob(ReadOnlySpan<byte> buffer, ref int headerLength, int dataLength)
	{
		// var result = FixedLengthMemoryPool<byte>.Shared.Rent(length);
		// buffer.Slice(readSize, length).CopyTo(result.Memory.Span);
		var result = buffer.Slice(headerLength, dataLength);
		headerLength += dataLength;
		return result;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static bool TryReadBinary(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> result, int dataLength,
									  ref int headerSize)
	{
		result = buffer.Slice(headerSize, dataLength);

		//result = FixedLengthMemoryPool<byte>.Shared.Rent(resultLength);
		//if (buffer.Slice(readSize, resultLength).TryCopyTo(result.Memory.Span))
		//{
		//    readSize += resultLength;
		//    return true;
		//}

		//result.Dispose();
		//result = null;
		return true;
	}
}
