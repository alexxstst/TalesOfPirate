using System;
using static System.Buffers.Binary.BinaryPrimitives;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with timestamp extensions.
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Writes unsigned 32bit seconds since unix epoch.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	public static int WriteTimestamp32(Span<byte> buffer, in Timestamp timestamp)
	{
		WriteUInt32BigEndian(buffer.Slice(2), (uint)timestamp.Seconds);
		WriteFixExtension4Header(buffer, ExtensionTypes.Timestamp);
		return DataLengths.TimeStamp32;
	}

	/// <summary>
	///     Writes the number of seconds and nanoseconds that have elapsed since unix epoch.
	/// </summary>
	/// <remarks>Nanoseconds is 30-bit unsigned int and seconds is in 34-bit unsigned int.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	public static int WriteTimestamp64(Span<byte> buffer, in Timestamp timestamp)
	{
		WriteUInt64BigEndian(buffer.Slice(2), timestamp.Epoch64);
		WriteFixExtension8Header(buffer, ExtensionTypes.Timestamp);
		return DataLengths.TimeStamp64;
	}

	/// <summary>
	///     Writes the number of seconds and nanoseconds that have elapsed since unix epoch.
	/// </summary>
	/// <remarks>Nanoseconds is 32-bit unsigned int and seconds is in 64-bit signed int.</remarks>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	public static int WriteTimestamp96(Span<byte> buffer, in Timestamp timestamp)
	{
		WriteInt64BigEndian(buffer.Slice(7), timestamp.Seconds);
		WriteUInt32BigEndian(buffer.Slice(3), timestamp.NanoSeconds);
		WriteExtension8Header(buffer, ExtensionTypes.Timestamp, 12);
		return DataLengths.TimeStamp96;
	}

	/// <summary>
	///     Writes smallest representation of timestamp.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	public static int WriteTimestamp(Span<byte> buffer, in Timestamp timestamp)
	{
		if (timestamp.Seconds >> 34 != 0)
			return WriteTimestamp96(buffer, timestamp);

		return (timestamp.Epoch64 & 0xffffffff00000000UL) == 0
			? WriteTimestamp32(buffer, timestamp)
			: WriteTimestamp64(buffer, timestamp);
	}

	/// <summary>
	///     Tries to writes unsigned 32bit seconds since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to write to.</paramref>
	/// <paramref name="timestamp">Timestamp to write.</paramref>
	/// <paramref name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="timestamp" /> ⊄ [1970-01-01 00:00:00 UTC, 2106-02-07 06:28:16 UTC)</description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryWriteTimestamp32(Span<byte> buffer, in Timestamp timestamp, out int wroteSize)
	{
		wroteSize = DataLengths.TimeStamp32;
		if (buffer.Length < wroteSize) return false;
		if (!(0 <= timestamp.Seconds && timestamp.Seconds <= uint.MaxValue)) return false;
		WriteUInt32BigEndian(buffer.Slice(2), (uint)timestamp.Seconds);
		WriteFixExtension4Header(buffer, ExtensionTypes.Timestamp);
		return true;
	}

	/// <summary>
	///     Tries to the number of seconds and nanoseconds that have elapsed since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to write to.</paramref>
	/// <paramref name="timestamp">Timestamp to write.</paramref>
	/// <paramref name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description>
	///                 <paramref name="timestamp" /> ⊄ [1970-01-01 00:00:00.000000000 UTC, 2514-05-30
	///                 01:53:04.000000000 UTC)
	///             </description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryWriteTimestamp64(Span<byte> buffer, in Timestamp timestamp, out int wroteSize)
	{
		wroteSize = DataLengths.TimeStamp64;
		if (buffer.Length < wroteSize) return false;
		if (timestamp.Seconds >> 34 != 0) return false;
		WriteUInt64BigEndian(buffer.Slice(2), timestamp.Epoch64);
		WriteFixExtension8Header(buffer, ExtensionTypes.Timestamp);
		return true;
	}

	/// <summary>
	///     Tries to write the number of seconds and nanoseconds.
	/// </summary>
	/// <paramref name="buffer">Buffer to write to.</paramref>
	/// <paramref name="timestamp">Timestamp to write.</paramref>
	/// <paramref name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description>
	///                 <paramref name="timestamp" /> ⊄ [-584554047284-02-23 16:59:44 UTC, 584554051223-11-09
	///                 07:00:16.000000000 UTC)
	///             </description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryWriteTimestamp96(Span<byte> buffer, in Timestamp timestamp, out int wroteSize)
	{
		wroteSize = DataLengths.TimeStamp96;
		if (buffer.Length < wroteSize) return false;
		WriteInt64BigEndian(buffer.Slice(7), timestamp.Seconds);
		WriteUInt32BigEndian(buffer.Slice(3), timestamp.NanoSeconds);
		WriteExtension8Header(buffer, ExtensionTypes.Timestamp, 12);
		return true;
	}

	/// <summary>
	///     Tries to write smallest possible representation of <paramref name="timestamp" /> into
	/// </summary>
	/// <paramref name="buffer">Buffer to write to.</paramref>
	/// <paramref name="timestamp">Timestamp to write.</paramref>
	/// <paramref name="wroteSize">Count of bytes, written to <paramref name="buffer" />.</paramref>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	public static bool TryWriteTimestamp(Span<byte> buffer, in Timestamp timestamp, out int wroteSize)
	{
		if (timestamp.Seconds >> 34 != 0)
			return TryWriteTimestamp96(buffer, timestamp, out wroteSize);

		return (timestamp.Epoch64 & 0xffffffff00000000UL) == 0
			? TryWriteTimestamp32(buffer, timestamp, out wroteSize)
			: TryWriteTimestamp64(buffer, timestamp, out wroteSize);
	}

	/// <summary>
	///     Reads unsigned 32bit seconds since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>Timestamp</returns>
	public static Timestamp ReadTimestamp32(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.TimeStamp32;
		var extension = ReadFixExtension4Header(buffer, out var headerSize);
		if (extension != ExtensionTypes.Timestamp)
			ThrowWrongExtensionTypeException(extension, ExtensionTypes.Timestamp);
		return new Timestamp(ReadUInt32BigEndian(buffer.Slice(headerSize)));
	}

	/// <summary>
	///     Reads unsigned 64bit seconds since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>Timestamp</returns>
	public static Timestamp ReadTimestamp64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.TimeStamp64;
		var extension = ReadFixExtension8Header(buffer, out var headerSize);
		if (extension != ExtensionTypes.Timestamp)
			ThrowWrongExtensionTypeException(extension, ExtensionTypes.Timestamp);
		return new Timestamp(ReadUInt64BigEndian(buffer.Slice(headerSize)));
	}

	/// <summary>
	///     Reads 96bit timestamp.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>Timestamp</returns>
	public static Timestamp ReadTimestamp96(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.TimeStamp96;
		var (extension, length) = ReadExtension8Header(buffer, out var headerSize);
		if (extension != ExtensionTypes.Timestamp)
			ThrowWrongExtensionTypeException(extension, ExtensionTypes.Timestamp);
		if (length != 12) ThrowWrongExtensionLengthException(length, 12);
		return new Timestamp(ReadInt64BigEndian(buffer.Slice(headerSize + 4)),
			ReadUInt32BigEndian(buffer.Slice(headerSize)));
	}

	/// <summary>
	///     Reads timestamp.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>Timestamp</returns>
	public static Timestamp ReadTimestamp(ReadOnlySpan<byte> buffer, out int readSize)
	{
		switch (buffer[0])
		{
			case DataCodes.FixExtension4:
				return ReadTimestamp32(buffer, out readSize);
			case DataCodes.FixExtension8:
				return ReadTimestamp64(buffer, out readSize);
			case DataCodes.Extension8:
				return ReadTimestamp96(buffer, out readSize);
			default:
				ThrowWrongCodeException(
					buffer[0],
					DataCodes.FixExtension4,
					DataCodes.FixExtension8,
					DataCodes.Extension8);
				readSize = 0;
				return default;
		}
	}

	/// <summary>
	///     Reads unsigned 32bit seconds since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="timestamp">Timestamp</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] is not <see cref="DataCodes.FixExtension4" /> or</description>
	///         </item>
	///         <item>
	///             <description>extension type is not <see cref="ExtensionTypes.Timestamp" /> or</description>
	///         </item>
	///         <item>
	///             <description>we could not read uint seconds from buffer.</description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryReadTimestamp32(ReadOnlySpan<byte> buffer, out Timestamp timestamp, out int readSize)
	{
		readSize = DataLengths.TimeStamp32;
		timestamp = Timestamp.Zero;
		if (buffer.Length < readSize) return false;
		if (!TryReadFixExtension4Header(buffer, out var extension, out var headerSize)) return false;
		if (extension != ExtensionTypes.Timestamp) return false;
		if (!TryReadUInt32BigEndian(buffer.Slice(headerSize), out var seconds)) return false;
		timestamp = new Timestamp(seconds);
		return true;
	}

	/// <summary>
	///     Reads unsigned 64bit seconds since unix epoch.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="timestamp">Timestamp</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] is not <see cref="DataCodes.FixExtension8" /> or</description>
	///         </item>
	///         <item>
	///             <description>extension type is not <see cref="ExtensionTypes.Timestamp" /> or</description>
	///         </item>
	///         <item>
	///             <description>we could not read ulong seconds from buffer.</description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryReadTimestamp64(ReadOnlySpan<byte> buffer, out Timestamp timestamp, out int readSize)
	{
		readSize = DataLengths.TimeStamp64;
		timestamp = Timestamp.Zero;
		if (buffer.Length < readSize) return false;
		if (!TryReadFixExtension8Header(buffer, out var extension, out var headerSize)) return false;
		if (extension != ExtensionTypes.Timestamp) return false;
		if (!TryReadUInt64BigEndian(buffer.Slice(headerSize), out var seconds)) return false;
		timestamp = new Timestamp(seconds);
		return true;
	}

	/// <summary>
	///     Reads 96bit timestamp.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="timestamp">Timestamp</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description><paramref name="buffer" />[0] is not <see cref="DataCodes.Extension8" /> or</description>
	///         </item>
	///         <item>
	///             <description>extension type is not <see cref="ExtensionTypes.Timestamp" /> or</description>
	///         </item>
	///         <item>
	///             <description>we could not read long seconds and/or uint nanoseconds from buffer.</description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryReadTimestamp96(ReadOnlySpan<byte> buffer, out Timestamp timestamp, out int readSize)
	{
		readSize = DataLengths.TimeStamp96;
		timestamp = Timestamp.Zero;
		if (buffer.Length < readSize) return false;
		if (!TryReadExtension8Header(buffer, out var extension, out var length, out var headerSize)) return false;
		if (extension != ExtensionTypes.Timestamp) return false;
		if (length != 12) return false;
		if (!TryReadUInt32BigEndian(buffer.Slice(headerSize), out var nanoseconds)) return false;
		if (!TryReadInt64BigEndian(buffer.Slice(headerSize + 4), out var seconds)) return false;
		timestamp = new Timestamp(seconds, nanoseconds);
		return true;
	}

	/// <summary>
	///     Reads timestamp.
	/// </summary>
	/// <paramref name="buffer">Buffer to read from.</paramref>
	/// <paramref name="timestamp">Timestamp</paramref>
	/// <paramref name="readSize">Count of bytes, read from <paramref name="buffer" />.</paramref>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if:
	///     <list type="bullet">
	///         <item>
	///             <description><paramref name="buffer" /> is too small or</description>
	///         </item>
	///         <item>
	///             <description>
	///                 <paramref name="buffer" />[0] is not <see cref="DataCodes.FixExtension4" />,
	///                 <see cref="DataCodes.FixExtension8" /> or <see cref="DataCodes.Extension8" /> or
	///             </description>
	///         </item>
	///         <item>
	///             <description>extension type is not <see cref="ExtensionTypes.Timestamp" /> or</description>
	///         </item>
	///         <item>
	///             <description>we could not read data from buffer.</description>
	///         </item>
	///     </list>
	/// </returns>
	public static bool TryReadTimestamp(ReadOnlySpan<byte> buffer, out Timestamp timestamp, out int readSize)
	{
		switch (buffer[0])
		{
			case DataCodes.FixExtension4:
				return TryReadTimestamp32(buffer, out timestamp, out readSize);
			case DataCodes.FixExtension8:
				return TryReadTimestamp64(buffer, out timestamp, out readSize);
			case DataCodes.Extension8:
				return TryReadTimestamp96(buffer, out timestamp, out readSize);
			default:
				readSize = 0;
				timestamp = Timestamp.Zero;
				return false;
		}
	}
}