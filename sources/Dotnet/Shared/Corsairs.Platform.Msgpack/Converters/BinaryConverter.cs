using System;

namespace Corsairs.Platform.Msgpack.Converters;

internal class BinaryConverter : IMsgPackConverter<byte[]>
{
	private readonly bool _compatibilityMode;

	private MsgPackContext _context;

	public BinaryConverter(bool compatibilityMode)
	{
		_compatibilityMode = compatibilityMode;
	}

	public void Initialize(MsgPackContext context)
	{
		_context = context;
	}

	public void Write(byte[] value, IMsgPackWriter writer)
	{
		if (value == null)
		{
			_context.NullConverter.Write(value, writer);
			return;
		}

		var valueLength = (uint)value.Length;
		if (_compatibilityMode)
			WriteStringHeaderAndLength(valueLength, writer);
		else
			WriteBinaryHeaderAndLength(valueLength, writer);

		writer.Write(value);
	}

	// We will have problem with binary blobs greater than int.MaxValue bytes.
	public byte[] Read(IMsgPackReader reader)
	{
		var type = reader.ReadDataType();

		uint length;
		switch (type)
		{
			case DataTypes.Null:
				return null;

			case DataTypes.Bin8:
				length = NumberConverter.ReadUInt8(reader);
				break;

			case DataTypes.Bin16:
				length = NumberConverter.ReadUInt16(reader);
				break;

			case DataTypes.Bin32:
				length = NumberConverter.ReadUInt32(reader);
				break;

			case DataTypes.Str8:
				if (_compatibilityMode)
					length = NumberConverter.ReadUInt8(reader);
				else
					throw ExceptionUtils.CantReadStringAsBinary();
				break;

			case DataTypes.Str16:
				if (_compatibilityMode)
					length = NumberConverter.ReadUInt16(reader);
				else
					throw ExceptionUtils.CantReadStringAsBinary();
				break;

			case DataTypes.Str32:
				if (_compatibilityMode)
					length = NumberConverter.ReadUInt32(reader);
				else
					throw ExceptionUtils.CantReadStringAsBinary();
				break;

			default:
				if ((type & DataTypes.FixStr) == DataTypes.FixStr)
				{
					if (_compatibilityMode)
						length = (uint)(type & ~DataTypes.FixStr);
					else
						throw ExceptionUtils.CantReadStringAsBinary();
				}
				else
				{
					throw ExceptionUtils.BadTypeException(type, DataTypes.Bin8, DataTypes.Bin16, DataTypes.Bin32,
						DataTypes.Null);
				}

				break;
		}

		var segment = reader.ReadBytes(length);
		var array = new byte[segment.Count];
		Array.Copy(segment.Array, segment.Offset, array, 0, segment.Count);
		return array;
	}

	private void WriteBinaryHeaderAndLength(uint length, IMsgPackWriter writer)
	{
		if (length <= byte.MaxValue)
		{
			writer.Write(DataTypes.Bin8);
			NumberConverter.WriteByteValue((byte)length, writer);
		}
		else if (length <= ushort.MaxValue)
		{
			writer.Write(DataTypes.Bin16);
			NumberConverter.WriteUShortValue((ushort)length, writer);
		}
		else
		{
			writer.Write(DataTypes.Bin32);
			NumberConverter.WriteUIntValue(length, writer);
		}
	}

	private void WriteStringHeaderAndLength(uint length, IMsgPackWriter writer)
	{
		if (length <= 31)
		{
			writer.Write((byte)(((byte)DataTypes.FixStr + length) % 256));
			return;
		}

		if (length <= ushort.MaxValue)
		{
			writer.Write(DataTypes.Str16);
			NumberConverter.WriteUShortValue((ushort)length, writer);
		}
		else
		{
			writer.Write(DataTypes.Str32);
			NumberConverter.WriteUIntValue(length, writer);
		}
	}
}