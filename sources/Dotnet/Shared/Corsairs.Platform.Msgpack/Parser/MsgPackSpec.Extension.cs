using System;
using System.Buffers.Binary;
using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with extensions
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Writes extension header. Extension length is 1 byte.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension1Header(Span<byte> buffer, sbyte type)
	{
		return WriteFixExtensionHeader(buffer, DataCodes.FixExtension1, type);
	}

	/// <summary>
	///     Writes 1-byte extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension1(Span<byte> buffer, sbyte type, byte extension)
	{
		buffer[2] = extension;
		return WriteFixExtension1Header(buffer, type) + sizeof(byte);
	}

	/// <summary>
	///     Writes extension header. Extension length is 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension2Header(Span<byte> buffer, sbyte type)
	{
		return WriteFixExtensionHeader(buffer, DataCodes.FixExtension2, type);
	}

	/// <summary>
	///     Writes 2-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension2(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		buffer[3] = extension[1];
		buffer[2] = extension[0];
		return WriteFixExtension2Header(buffer, type) + 2;
	}

	/// <summary>
	///     Writes extension header. Extension length is 4 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension4Header(Span<byte> buffer, sbyte type)
	{
		return WriteFixExtensionHeader(buffer, DataCodes.FixExtension4, type);
	}

	/// <summary>
	///     Writes 4-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension4(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		var result = WriteFixExtension4Header(buffer, type);
		const int extLength = 4;
		extension.Slice(0, extLength).CopyTo(buffer.Slice(result));
		return result + extLength;
	}

	/// <summary>
	///     Writes extension header. Extension length is 8 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension8Header(Span<byte> buffer, sbyte type)
	{
		return WriteFixExtensionHeader(buffer, DataCodes.FixExtension8, type);
	}

	/// <summary>
	///     Writes 8-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension8(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		var result = WriteFixExtension8Header(buffer, type);
		const int extLength = 8;
		extension.Slice(0, extLength).CopyTo(buffer.Slice(result));
		return result + extLength;
	}

	/// <summary>
	///     Writes extension header. Extension length is 16 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension16Header(Span<byte> buffer, sbyte type)
	{
		return WriteFixExtensionHeader(buffer, DataCodes.FixExtension16, type);
	}

	/// <summary>
	///     Writes 16-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixExtension16(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		var result = WriteFixExtension16Header(buffer, type);
		const int extLength = 16;
		extension.Slice(0, extLength).CopyTo(buffer.Slice(result));
		return result + extLength;
	}

	/// <summary>
	///     Writes extension header. Extension length is 8-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteExtension8Header(Span<byte> buffer, sbyte type, byte length)
	{
		buffer[2] = unchecked((byte)type);
		buffer[1] = length;
		buffer[0] = DataCodes.Extension8;
		return DataLengths.Extension8Header;
	}

	/// <summary>
	///     Writes extension. Extension length is 8-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteExtension8(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		if (extension.Length > byte.MaxValue)
			return ThrowDataIsTooLarge(extension.Length, byte.MaxValue, "extension", DataCodes.Extension8);
		var result = WriteExtension8Header(buffer, type, (byte)extension.Length);
		extension.CopyTo(buffer.Slice(result));
		return result + extension.Length;
	}

	/// <summary>
	///     Writes extension header. Extension length is 16-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteExtension16Header(Span<byte> buffer, sbyte type, ushort length)
	{
		buffer[3] = unchecked((byte)type);
		BinaryPrimitives.WriteUInt16BigEndian(buffer.Slice(1), length);
		buffer[0] = DataCodes.Extension16;
		return DataLengths.Extension16Header;
	}

	/// <summary>
	///     Writes extension. Extension length is 16-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	public static int WriteExtension16(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		if (extension.Length > ushort.MaxValue)
			return ThrowDataIsTooLarge(extension.Length, ushort.MaxValue, "extension", DataCodes.Extension16);
		var result = WriteExtension16Header(buffer, type, (ushort)extension.Length);
		extension.CopyTo(buffer.Slice(result));
		return result + extension.Length;
	}

	/// <summary>
	///     Writes extension header. Extension length is 32-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteExtension32Header(Span<byte> buffer, sbyte type, uint length)
	{
		buffer[5] = unchecked((byte)type);
		BinaryPrimitives.WriteUInt32BigEndian(buffer.Slice(1), length);
		buffer[0] = DataCodes.Extension32;
		return DataLengths.Extension32Header;
	}

	/// <summary>
	///     Writes extension. Extension length is 32-bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension value.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	public static int WriteExtension32(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		var result = WriteExtension32Header(buffer, type, (uint)extension.Length);
		extension.CopyTo(buffer.Slice(result));
		return result + extension.Length;
	}

	/// <summary>
	///     Writes smallest possible extension header.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteExtensionHeader(Span<byte> buffer, sbyte type, uint length)
	{
		switch (length)
		{
			case 1:
				return WriteFixExtension1Header(buffer, type);
			case 2:
				return WriteFixExtension2Header(buffer, type);
			case 4:
				return WriteFixExtension4Header(buffer, type);
			case 8:
				return WriteFixExtension8Header(buffer, type);
			case 16:
				return WriteFixExtension16Header(buffer, type);
		}

		if (length <= byte.MaxValue)
			return WriteExtension8Header(buffer, type, (byte)length);

		if (length <= ushort.MaxValue)
			return WriteExtension16Header(buffer, type, (ushort)length);

		return WriteExtension32Header(buffer, type, length);
	}

	/// <summary>
	///     Writes smallest possible extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension.</param>
	/// <returns>Count of bytes, written into <paramref name="buffer" />.</returns>
	public static int WriteExtension(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension)
	{
		var result = WriteExtensionHeader(buffer, type, (uint)extension.Length);
		extension.CopyTo(buffer.Slice(result));
		return result + extension.Length;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 1 byte.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension1Header(Span<byte> buffer, sbyte type, out int wroteSize)
	{
		return TryWriteFixExtensionHeader(buffer, DataCodes.FixExtension1, type, out wroteSize);
	}

	/// <summary>
	///     Tries to write extension. Extension length is 1 byte.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension1(Span<byte> buffer, sbyte type, byte extension, out int wroteSize)
	{
		wroteSize = 0;
		const int size = 3;
		if (buffer.Length < size) return false;

		WriteFixExtension1(buffer, type, extension);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension2Header(Span<byte> buffer, sbyte type, out int wroteSize)
	{
		return TryWriteFixExtensionHeader(buffer, DataCodes.FixExtension2, type, out wroteSize);
	}

	/// <summary>
	///     Tries to write extension. Extension length is 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is not 2.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension2(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
											 out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length != 2) return false;
		const int size = 3;
		if (buffer.Length < size) return false;

		WriteFixExtension2(buffer, type, extension);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 4 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension4Header(Span<byte> buffer, sbyte type, out int wroteSize)
	{
		return TryWriteFixExtensionHeader(buffer, DataCodes.FixExtension4, type, out wroteSize);
	}

	/// <summary>
	///     Tries to write extension. Extension length is 4 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is not 4.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension4(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
											 out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length != 4) return false;
		const int size = 6;
		if (buffer.Length < size) return false;

		WriteFixExtension4(buffer, type, extension);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 8 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension8Header(Span<byte> buffer, sbyte type, out int wroteSize)
	{
		return TryWriteFixExtensionHeader(buffer, DataCodes.FixExtension8, type, out wroteSize);
	}

	/// <summary>
	///     Tries to write extension. Extension length is 8 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is not 8.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension8(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
											 out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length != 8) return false;
		const int size = 10;
		if (buffer.Length < size) return false;

		WriteFixExtension8(buffer, type, extension);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 16 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension16Header(Span<byte> buffer, sbyte type, out int wroteSize)
	{
		return TryWriteFixExtensionHeader(buffer, DataCodes.FixExtension16, type, out wroteSize);
	}

	/// <summary>
	///     Tries to write extension. Extension length is 16 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is not 16.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixExtension16(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
											  out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length != 16) return false;
		const int size = 18;
		if (buffer.Length < size) return false;

		WriteFixExtension16(buffer, type, extension);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 8 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extensions.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteExtension8Header(Span<byte> buffer, sbyte type, byte length, out int wroteSize)
	{
		wroteSize = 0;
		const int size = 3;
		if (buffer.Length < size) return false;

		WriteExtension8Header(buffer, type, length);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension. Extension length is 8 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is greater than <see cref="byte.MaxValue" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteExtension8(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
										  out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length > byte.MaxValue) return false;
		if (buffer.Length < extension.Length + DataLengths.Extension8Header) return false;

		wroteSize = WriteExtension8(buffer, type, extension);
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 16 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteExtension16Header(Span<byte> buffer, sbyte type, ushort length, out int wroteSize)
	{
		wroteSize = 0;
		const int size = 4;
		if (buffer.Length < size) return false;

		WriteExtension16Header(buffer, type, length);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension. Extension length is 16 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="extension" /> length is greater than <see cref="ushort.MaxValue" />.
	/// </returns>
	public static bool TryWriteExtension16(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
										   out int wroteSize)
	{
		wroteSize = 0;
		if (extension.Length > ushort.MaxValue) return false;
		if (buffer.Length < extension.Length + DataLengths.Extension16Header) return false;

		wroteSize = WriteExtension16(buffer, type, extension);
		return true;
	}

	/// <summary>
	///     Tries to write extension header. Extension length is 32bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteExtension32Header(Span<byte> buffer, sbyte type, uint length, out int wroteSize)
	{
		wroteSize = 0;
		const int size = 6;
		if (buffer.Length < size) return false;

		WriteExtension32Header(buffer, type, length);
		wroteSize = size;
		return true;
	}

	/// <summary>
	///     Tries to write extension. Extension length is 32 bits.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	public static bool TryWriteExtension32(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension,
										   out int wroteSize)
	{
		wroteSize = 0;
		if (buffer.Length < extension.Length + DataLengths.Extension32Header) return false;

		wroteSize = WriteExtension32(buffer, type, extension);
		return true;
	}

	/// <summary>
	///     Tries to write smallest possible extension header.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteExtensionHeader(Span<byte> buffer, sbyte type, uint length, out int wroteSize)
	{
		switch (length)
		{
			case 1:
				return TryWriteFixExtension1Header(buffer, type, out wroteSize);
			case 2:
				return TryWriteFixExtension2Header(buffer, type, out wroteSize);
			case 4:
				return TryWriteFixExtension4Header(buffer, type, out wroteSize);
			case 8:
				return TryWriteFixExtension8Header(buffer, type, out wroteSize);
			case 16:
				return TryWriteFixExtension16Header(buffer, type, out wroteSize);
		}

		if (length <= byte.MaxValue)
			return TryWriteExtension8Header(buffer, type, (byte)length, out wroteSize);

		if (length <= ushort.MaxValue)
			return TryWriteExtension16Header(buffer, type, (ushort)length, out wroteSize);

		return TryWriteExtension32Header(buffer, type, length, out wroteSize);
	}

	/// <summary>
	///     Tries to write smallest possible extension.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="type">Type code of extension.</param>
	/// <param name="extension">Extension.</param>
	/// <param name="wroteSize">Count of bytes, written into <paramref name="buffer" />.</param>
	/// <returns><c>true</c> if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	public static bool TryWriteExtension(Span<byte> buffer, sbyte type, ReadOnlySpan<byte> extension, out int wroteSize)
	{
		if (!TryWriteExtensionHeader(buffer, type, (uint)extension.Length, out wroteSize)) return false;
		if (wroteSize + extension.Length > buffer.Length) return false;
		extension.CopyTo(buffer.Slice(wroteSize));
		return true;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 1 byte.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension1Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixExtensionHeader(buffer, DataCodes.FixExtension1, out readSize);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 1 byte.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static (sbyte type, byte extension) ReadFixExtension1(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = 3;
		return (ReadFixExtension1Header(buffer, out _), buffer[2]);
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type,</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension2Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixExtensionHeader(buffer, DataCodes.FixExtension2, out readSize);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 2 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension2(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var type = ReadFixExtension2Header(buffer, out readSize);
		extension = ReadExtension(buffer, 2, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 4 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension4Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixExtensionHeader(buffer, DataCodes.FixExtension4, out readSize);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 4 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension4(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var type = ReadFixExtension4Header(buffer, out readSize);
		extension = ReadExtension(buffer, 4, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 8 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension8Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixExtensionHeader(buffer, DataCodes.FixExtension8, out readSize);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 8 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension8(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var type = ReadFixExtension8Header(buffer, out readSize);
		extension = ReadExtension(buffer, 8, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 16 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension16Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixExtensionHeader(buffer, DataCodes.FixExtension16, out readSize);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 16 bytes.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadFixExtension16(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension,
										   ref int readSize)
	{
		var type = ReadFixExtension16Header(buffer, out readSize);
		extension = ReadExtension(buffer, 16, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 8 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static (sbyte type, byte length) ReadExtension8Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Extension8Header;
		var type = unchecked((sbyte)buffer[2]);
		var length = buffer[1];
		if (buffer[0] != DataCodes.Extension8) ThrowWrongCodeException(buffer[0], DataCodes.Extension8);
		return (type, length);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 8 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static sbyte ReadExtension8(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var (type, size) = ReadExtension8Header(buffer, out readSize);
		extension = ReadExtension(buffer, size, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 16 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static (sbyte type, ushort length) ReadExtension16Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Extension16Header;
		var type = unchecked((sbyte)buffer[3]);
		var length = BinaryPrimitives.ReadUInt16BigEndian(buffer.Slice(1));
		if (buffer[0] != DataCodes.Extension16) ThrowWrongCodeException(buffer[0], DataCodes.Extension16);
		return (type, length);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 16 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	public static sbyte ReadExtension16(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var (type, size) = ReadExtension16Header(buffer, out readSize);
		extension = ReadExtension(buffer, size, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header. Length of extension should be 32 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static (sbyte type, uint length) ReadExtension32Header(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Extension32Header;
		var type = unchecked((sbyte)buffer[5]);
		var length = BinaryPrimitives.ReadUInt32BigEndian(buffer.Slice(1));
		if (buffer[0] != DataCodes.Extension32) ThrowWrongCodeException(buffer[0], DataCodes.Extension32);
		return (type, length);
	}

	/// <summary>
	///     Reads extension. Length of extension should be 32 bits uint.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	public static sbyte ReadExtension32(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var (type, uintSize) = ReadExtension32Header(buffer, out readSize);
		if (uintSize > int.MaxValue) ThrowDataIsTooLarge(uintSize);
		extension = ReadExtension(buffer, (int)uintSize, ref readSize);
		return type;
	}

	/// <summary>
	///     Reads extension header.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and length</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static (sbyte type, uint length) ReadExtensionHeader(ReadOnlySpan<byte> buffer, out int readSize)
	{
		switch (buffer[0])
		{
			case DataCodes.FixExtension1:
				return (ReadFixExtension1Header(buffer, out readSize), 1);

			case DataCodes.FixExtension2:
				return (ReadFixExtension2Header(buffer, out readSize), 2);

			case DataCodes.FixExtension4:
				return (ReadFixExtension2Header(buffer, out readSize), 4);

			case DataCodes.FixExtension8:
				return (ReadFixExtension2Header(buffer, out readSize), 8);

			case DataCodes.FixExtension16:
				return (ReadFixExtension2Header(buffer, out readSize), 16);

			case DataCodes.Extension8:
				return ReadExtension8Header(buffer, out readSize);

			case DataCodes.Extension16:
				return ReadExtension16Header(buffer, out readSize);

			case DataCodes.Extension32:
				return ReadExtension32Header(buffer, out readSize);

			default:
				readSize = 0;
				ThrowWrongCodeException(
					buffer[0],
					DataCodes.FixExtension1,
					DataCodes.FixExtension2,
					DataCodes.FixExtension4,
					DataCodes.FixExtension8,
					DataCodes.FixExtension16,
					DataCodes.Extension8,
					DataCodes.Extension16,
					DataCodes.Extension32);
				return default;
		}
	}

	/// <summary>
	///     Reads extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>Extension code type and extension.</returns>
	public static sbyte ReadExtension(ReadOnlySpan<byte> buffer, out ReadOnlySpan<byte> extension, ref int readSize)
	{
		var (type, uintSize) = ReadExtensionHeader(buffer, out readSize);
		if (uintSize > int.MaxValue) ThrowDataIsTooLarge(uintSize);
		extension = ReadExtension(buffer, (int)uintSize, ref readSize);
		return type;
	}

	/// <summary>
	///     Tries to read header of 1-byte extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.FixExtension1" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension1Header(ReadOnlySpan<byte> buffer, out sbyte type, out int readSize)
	{
		return TryReadFixExtensionHeader(buffer, DataCodes.FixExtension1, out type, out readSize);
	}

	/// <summary>
	///     Tries to read 1-byte extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadFixExtension1Header" /> returned false.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension1(ReadOnlySpan<byte> buffer, out sbyte type, out byte extension,
											out int readSize)
	{
		extension = 0;
		if (!TryReadFixExtension1Header(buffer, out type, out readSize)) return false;
		var slice = buffer.Slice(2);
		if (slice.IsEmpty) return false;
		readSize += 1;
		extension = slice[0];
		return true;
	}

	/// <summary>
	///     Tries to read header of 2-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.FixExtension2" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension2Header(ReadOnlySpan<byte> buffer, out sbyte type, out int readSize)
	{
		return TryReadFixExtensionHeader(buffer, DataCodes.FixExtension2, out type, out readSize);
	}

	/// <summary>
	///     Tries to read 2-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadFixExtension2Header" /> returned false.
	/// </returns>
	public static bool TryReadFixExtension2(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
											out int readSize)
	{
		extension = null;
		return TryReadFixExtension2Header(buffer, out type, out readSize) &&
			   TryReadExtension(buffer, 2, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 4-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.FixExtension4" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension4Header(ReadOnlySpan<byte> buffer, out sbyte type, out int readSize)
	{
		return TryReadFixExtensionHeader(buffer, DataCodes.FixExtension4, out type, out readSize);
	}

	/// <summary>
	///     Tries to read 4-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadFixExtension4Header" /> returned false.
	/// </returns>
	public static bool TryReadFixExtension4(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
											out int readSize)
	{
		extension = null;
		return TryReadFixExtension4Header(buffer, out type, out readSize) &&
			   TryReadExtension(buffer, 4, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 8-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.FixExtension8" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension8Header(ReadOnlySpan<byte> buffer, out sbyte type, out int readSize)
	{
		return TryReadFixExtensionHeader(buffer, DataCodes.FixExtension8, out type, out readSize);
	}

	/// <summary>
	///     Tries to read 8-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadFixExtension8Header" /> returned false.
	/// </returns>
	public static bool TryReadFixExtension8(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
											out int readSize)
	{
		extension = null;
		return TryReadFixExtension8Header(buffer, out type, out readSize) &&
			   TryReadExtension(buffer, 8, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 16-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.FixExtension16" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixExtension16Header(ReadOnlySpan<byte> buffer, out sbyte type, out int readSize)
	{
		return TryReadFixExtensionHeader(buffer, DataCodes.FixExtension16, out type, out readSize);
	}

	/// <summary>
	///     Tries to read 16-bytes extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadFixExtension16Header" /> returned false.
	/// </returns>
	public static bool TryReadFixExtension16(ReadOnlySpan<byte> buffer, out sbyte type,
											 out ReadOnlySpan<byte> extension, out int readSize)
	{
		extension = null;
		return TryReadFixExtension16Header(buffer, out type, out readSize) &&
			   TryReadExtension(buffer, 16, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 8 bit length extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.Extension8" />
	/// </returns>
	public static bool TryReadExtension8Header(ReadOnlySpan<byte> buffer, out sbyte type, out byte length,
											   out int readSize)
	{
		length = 0;
		type = 0;
		readSize = DataLengths.Extension8Header;
		if (buffer.Length < DataLengths.Extension8Header) return false;
		type = unchecked((sbyte)buffer[2]);
		length = buffer[1];
		return buffer[0] == DataCodes.Extension8;
	}

	/// <summary>
	///     Tries to read 8 bit extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadExtension8Header" /> returned false.
	/// </returns>
	public static bool TryReadExtension8(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
										 out int readSize)
	{
		extension = null;
		return TryReadExtension8Header(buffer, out type, out var length, out readSize)
			   && TryReadExtension(buffer, length, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 16 bit length extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.Extension16" />
	/// </returns>
	public static bool TryReadExtension16Header(ReadOnlySpan<byte> buffer, out sbyte type, out ushort length,
												out int readSize)
	{
		length = 0;
		type = 0;
		readSize = DataLengths.Extension16Header;
		if (buffer.Length < DataLengths.Extension16Header) return false;
		type = unchecked((sbyte)buffer[3]);
		if (!BinaryPrimitives.TryReadUInt16BigEndian(buffer.Slice(1, 2), out length)) return false;
		return buffer[0] == DataCodes.Extension16;
	}

	/// <summary>
	///     Tries to read 16 bit extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadExtension16Header" /> returned false.
	/// </returns>
	public static bool TryReadExtension16(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
										  out int readSize)
	{
		extension = null;
		return TryReadExtension16Header(buffer, out type, out var length, out readSize)
			   && TryReadExtension(buffer, length, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of 16 bit length extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     <see cref="DataCodes.Extension16" />
	/// </returns>
	public static bool TryReadExtension32Header(ReadOnlySpan<byte> buffer, out sbyte type, out uint length,
												out int readSize)
	{
		length = 0;
		type = 0;
		readSize = DataLengths.Extension32Header;
		if (buffer.Length < DataLengths.Extension32Header) return false;
		type = unchecked((sbyte)buffer[5]);
		if (!BinaryPrimitives.TryReadUInt32BigEndian(buffer.Slice(1, 4), out length)) return false;
		return buffer[0] == DataCodes.Extension32;
	}

	/// <summary>
	///     Tries to read 32 bit extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadExtension32Header" /> returned false.
	/// </returns>
	public static bool TryReadExtension32(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
										  out int readSize)
	{
		extension = null;
		return TryReadExtension32Header(buffer, out type, out var length, out readSize)
			   && length <= int.MaxValue
			   && TryReadExtension(buffer, (int)length, ref extension, ref readSize);
	}

	/// <summary>
	///     Tries to read header of extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="length">Length of extension.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or first byte isn't
	///     one of extension codes.
	/// </returns>
	public static bool TryReadExtensionHeader(ReadOnlySpan<byte> buffer, out sbyte type, out uint length,
											  out int readSize)
	{
		if (buffer.IsEmpty)
		{
			type = 0;
			length = 0;
			readSize = 0;
			return false;
		}

		if (TryReadFixExtension1Header(buffer, out type, out readSize))
		{
			length = 1;
			return true;
		}

		if (TryReadFixExtension2Header(buffer, out type, out readSize))
		{
			length = 2;
			return true;
		}

		if (TryReadFixExtension4Header(buffer, out type, out readSize))
		{
			length = 4;
			return true;
		}

		if (TryReadFixExtension8Header(buffer, out type, out readSize))
		{
			length = 8;
			return true;
		}

		if (TryReadFixExtension16Header(buffer, out type, out readSize))
		{
			length = 16;
			return true;
		}

		if (TryReadExtension8Header(buffer, out type, out var byteLength, out readSize))
		{
			length = byteLength;
			return true;
		}

		if (TryReadExtension16Header(buffer, out type, out var shortLength, out readSize))
		{
			length = shortLength;
			return true;
		}

		return TryReadExtension32Header(buffer, out type, out length, out readSize);
	}

	/// <summary>
	///     Tries to read extension.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="type">Type of extension.</param>
	/// <param name="extension">Extension data.</param>
	/// <param name="readSize">Count of bytes read from <paramref name="buffer" />.</param>
	/// <returns>
	///     <c>true</c> if everything is ok or <c>false</c> if <paramref name="buffer" /> is too short or
	///     <see cref="TryReadExtensionHeader" /> returned false, or length is greater than <see cref="int.MaxValue" />.
	/// </returns>
	public static bool TryReadExtension(ReadOnlySpan<byte> buffer, out sbyte type, out ReadOnlySpan<byte> extension,
										out int readSize)
	{
		extension = null;
		return TryReadExtensionHeader(buffer, out type, out var length, out readSize)
			   && length <= int.MaxValue
			   && TryReadExtension(buffer, (int)length, ref extension, ref readSize);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static int WriteFixExtensionHeader(Span<byte> buffer, byte code, sbyte type)
	{
		buffer[1] = unchecked((byte)type);
		buffer[0] = code;
		return DataLengths.FixExtensionHeader;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static bool TryWriteFixExtensionHeader(Span<byte> buffer, byte code, sbyte type, out int wroteSize)
	{
		wroteSize = 0;
		if (buffer.Length < DataLengths.FixExtensionHeader) return false;

		WriteFixExtensionHeader(buffer, code, type);
		wroteSize = DataLengths.FixExtensionHeader;
		return true;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static bool TryReadExtension(ReadOnlySpan<byte> buffer, int size, ref ReadOnlySpan<byte> extension,
										 ref int readSize)
	{
		if (buffer.Length - readSize < size) return false;

		extension = buffer.Slice(readSize);
		readSize += size;
		return true;
		//extension = FixedLengthMemoryPool<byte>.Shared.Rent(size);
		// if (buffer.Slice(readSize).TryCopyTo(extension.Memory.Span))
		// {
		//     readSize += size;
		//     return true;
		// }
		//
		// return false;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static bool TryReadFixExtensionHeader(ReadOnlySpan<byte> buffer, byte code, out sbyte type,
												  out int readSize)
	{
		type = 0;
		readSize = DataLengths.FixExtensionHeader;
		if (buffer.Length < DataLengths.FixExtensionHeader) return false;
		if (buffer[0] != code) return false;
		type = unchecked((sbyte)buffer[1]);
		return true;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static ReadOnlySpan<byte> ReadExtension(
		ReadOnlySpan<byte> buffer,
		int size,
		ref int readSize)
	{
		//var owner = FixedLengthMemoryPool<byte>.Shared.Rent(size);
		var owner = buffer.Slice(readSize, size);
		readSize += size;
		return owner;
	}

	// ReSharper disable once ParameterOnlyUsedForPreconditionCheck.Local
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static sbyte ReadFixExtensionHeader(ReadOnlySpan<byte> buffer, byte code, out int readSize)
	{
		readSize = DataLengths.FixExtensionHeader;
		var type = unchecked((sbyte)buffer[1]);
		if (buffer[0] != code) ThrowWrongCodeException(buffer[0], code);
		return type;
	}
}