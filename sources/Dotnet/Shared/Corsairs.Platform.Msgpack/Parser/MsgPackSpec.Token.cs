using System;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with tokens
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	/// Copies token from buffer.
	/// </summary>
	/// <returns>Returns memory with length equal to read data.</returns>
	// public static ReadOnlySpan<byte> CopyToken(ReadOnlySpan<byte> buffer)
	// {
	//     var token = ReadToken(buffer);
	//     var result = FixedLengthMemoryPool<byte>.Shared.Rent(token.Length);
	//     token.CopyTo(result.Memory.Span);
	//     return result;
	// }

	/// <summary>
	///     Reads token from buffer. Just ignore value for skipping it.
	/// </summary>
	public static ReadOnlySpan<byte> ReadToken(ReadOnlySpan<byte> buffer)
	{
		if (buffer.IsEmpty) throw new ArgumentOutOfRangeException(nameof(buffer), "EOF: Buffer is empty.");

		var offset = 0;
		var elementsToRead = 1L;
		do
		{
			var code = buffer[offset];
			switch (code)
			{
				case <= DataCodes.FixPositiveMax:
					elementsToRead--;
					offset++;
					continue;
				case >= DataCodes.FixMapMin and <= DataCodes.FixMapMax:
				{
					var length = ReadFixMapHeader(buffer.Slice(offset), out var readSize);
					elementsToRead += 2 * length - 1;
					offset += readSize;
					continue;
				}
				case >= DataCodes.FixArrayMin and <= DataCodes.FixArrayMax:
				{
					var length = ReadFixArrayHeader(buffer.Slice(offset), out var readSize);
					elementsToRead += length - 1;
					offset += readSize;
					continue;
				}
				case >= DataCodes.FixStringMin and <= DataCodes.FixStringMax:
				{
					uint length = ReadFixStringHeader(buffer.Slice(offset), out var readSize);
					elementsToRead--;
					offset += (int)(length + readSize);
					continue;
				}
				case >= DataCodes.FixNegativeMin:
					elementsToRead--;
					offset++;
					continue;
				default:
					switch (code)
					{
						case DataCodes.Nil:
						case DataCodes.True:
						case DataCodes.False:
							elementsToRead--;
							offset++;
							continue;

						case DataCodes.Binary8:
							uint length = ReadBinary8Header(buffer.Slice(offset), out var readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Binary16:
							length = ReadBinary16Header(buffer.Slice(offset), out readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Binary32:
							length = ReadBinary32Header(buffer.Slice(offset), out readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Extension8:
							length = ReadExtension8Header(buffer.Slice(offset), out readSize).length;
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Extension16:
							length = ReadExtension16Header(buffer.Slice(offset), out readSize).length;
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Extension32:
							length = ReadExtension32Header(buffer.Slice(offset), out readSize).length;
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Float32:
							elementsToRead--;
							offset += DataLengths.Float32;
							continue;

						case DataCodes.Float64:
							elementsToRead--;
							offset += DataLengths.Float64;
							continue;

						case DataCodes.UInt8:
							elementsToRead--;
							offset += DataLengths.UInt8;
							continue;

						case DataCodes.UInt16:
							elementsToRead--;
							offset += DataLengths.UInt16;
							continue;

						case DataCodes.UInt32:
							elementsToRead--;
							offset += DataLengths.UInt32;
							continue;

						case DataCodes.UInt64:
							elementsToRead--;
							offset += DataLengths.UInt64;
							continue;

						case DataCodes.Int8:
							elementsToRead--;
							offset += DataLengths.Int8;
							continue;

						case DataCodes.Int16:
							elementsToRead--;
							offset += DataLengths.Int16;
							continue;

						case DataCodes.Int32:
							elementsToRead--;
							offset += DataLengths.Int32;
							continue;

						case DataCodes.Int64:
							elementsToRead--;
							offset += DataLengths.Int64;
							continue;

						case DataCodes.FixExtension1:
							elementsToRead--;
							offset += DataLengths.FixExtensionHeader + 1;
							continue;

						case DataCodes.FixExtension2:
							elementsToRead--;
							offset += DataLengths.FixExtensionHeader + 2;
							continue;

						case DataCodes.FixExtension4:
							elementsToRead--;
							offset += DataLengths.FixExtensionHeader + 4;
							continue;

						case DataCodes.FixExtension8:
							elementsToRead--;
							offset += DataLengths.FixExtensionHeader + 8;
							continue;

						case DataCodes.FixExtension16:
							elementsToRead--;
							offset += DataLengths.FixExtensionHeader + 16;
							continue;

						case DataCodes.String8:
							length = ReadString8Header(buffer.Slice(offset), out readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.String16:
							length = ReadString16Header(buffer.Slice(offset), out readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.String32:
							length = ReadString32Header(buffer.Slice(offset), out readSize);
							elementsToRead--;
							offset += (int)(length + readSize);
							continue;

						case DataCodes.Array16:
							length = ReadArray16Header(buffer.Slice(offset), out readSize);
							elementsToRead += length - 1;
							offset += readSize;
							continue;

						case DataCodes.Array32:
							length = ReadArray32Header(buffer.Slice(offset), out readSize);
							elementsToRead += length - 1;
							offset += readSize;
							continue;

						case DataCodes.Map16:
							length = ReadMap16Header(buffer.Slice(offset), out readSize);
							elementsToRead += 2 * length - 1;
							offset += readSize;
							continue;

						case DataCodes.Map32:
							length = ReadMap32Header(buffer.Slice(offset), out readSize);
							elementsToRead += 2 * length - 1;
							offset += readSize;
							continue;

						// case "NeverUsed" be here to have happy compiler
						default:
							throw new ArgumentOutOfRangeException(nameof(buffer),
								$"Data code at {nameof(buffer)}[{offset}] is 0xc1 and it is invalid data code.");
					}
			}
		} while (elementsToRead > 0);

		return buffer.Slice(0, offset);
	}

	/// <summary>
	///     Reads token from buffer. Just ignore value for skipping it.
	/// </summary>
	public static (byte code, int finishIndex, int length) ReadTokenData(ReadOnlySpan<byte> buffer)
	{
		if (buffer.IsEmpty) throw new ArgumentOutOfRangeException(nameof(buffer), "EOF: Buffer is empty.");

		var code = buffer[0];
		var readSize = 0;
		var length = 0;
		switch (code)
		{
			case DataCodes.Nil:
			case DataCodes.True:
			case DataCodes.False:
				return (code, 1, 0);

			case DataCodes.Binary8:
			case DataCodes.Binary16:
			case DataCodes.Binary32:
				length = ReadBinaryHeader(buffer, out readSize);
				return (code, readSize + length, length);

			case DataCodes.Extension8:
			case DataCodes.Extension16:
			case DataCodes.Extension32:
				length = (int)ReadExtensionHeader(buffer, out readSize).length;
				return (code, readSize, 0);

			case DataCodes.FixExtension1:
				return (code, DataLengths.FixExtensionHeader + 1, 0);

			case DataCodes.FixExtension2:
				return (code, DataLengths.FixExtensionHeader + 2, 0);

			case DataCodes.FixExtension4:
				return (code, DataLengths.FixExtensionHeader + 4, 0);

			case DataCodes.FixExtension8:
				return (code, DataLengths.FixExtensionHeader + 8, 0);

			case DataCodes.FixExtension16:
				return (code, DataLengths.FixExtensionHeader + 16, 0);

			case DataCodes.String8:
			case DataCodes.String16:
			case DataCodes.String32:
				length = ReadStringHeader(buffer, out readSize);
				return (code, readSize, length);

			case DataCodes.Array16:
			case DataCodes.Array32:
				length = ReadArrayHeader(buffer, out readSize);
				return (code, readSize, length);

			case DataCodes.Map16:
			case DataCodes.Map32:
				length = (int)ReadMapHeader(buffer, out readSize);
				return (code, readSize, length);
			case >= DataCodes.FixMapMin and <= DataCodes.FixMapMax:
			{
				length = ReadFixMapHeader(buffer, out readSize);
				return (code, readSize, length);
			}
			case >= DataCodes.FixArrayMin and <= DataCodes.FixArrayMax:
			{
				length = ReadFixArrayHeader(buffer, out readSize);
				return (0x90 /*DataTypes.FixArray*/, readSize, length);
				//return (code, readSize, length);
			}
			case >= DataCodes.FixStringMin and <= DataCodes.FixStringMax:
			{
				length = ReadFixStringHeader(buffer, out readSize);
				return (code, readSize + length, length);
			}
			case <= DataCodes.FixPositiveMax and > 0:
				return ((byte)DataTypes.Int8, 1, 0);
			case >= DataCodes.FixNegativeMin:
				return ((byte)DataTypes.Int8, 1, 0);

			default:
				return (code, DataLengths.GetHeaderLength(code), 0);
		}
	}
}