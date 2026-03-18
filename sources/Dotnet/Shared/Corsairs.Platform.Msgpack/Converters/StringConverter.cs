using System.Text;

namespace Corsairs.Platform.Msgpack.Converters;

internal class StringConverter : IMsgPackConverter<string>
{
	private static readonly Encoding Utf8 = new UTF8Encoding(false);
	private MsgPackContext _context;

	public void Initialize(MsgPackContext context)
	{
		_context = context;
	}

	public void Write(string value, IMsgPackWriter writer)
	{
		if (value == null)
		{
			_context.NullConverter.Write(value, writer);
			return;
		}

		var data = Utf8.GetBytes(value);

		WriteStringHeaderAndLength(writer, data.Length);

		writer.Write(data);
	}

	public string Read(IMsgPackReader reader)
	{
		var type = reader.ReadDataType();

		switch (type)
		{
			case DataTypes.Null:
				return null;

			case DataTypes.Str8:
				return ReadString(reader, NumberConverter.ReadUInt8(reader));

			case DataTypes.Str16:
				return ReadString(reader, NumberConverter.ReadUInt16(reader));

			case DataTypes.Str32:
				return ReadString(reader, NumberConverter.ReadUInt32(reader));
		}

		uint length;
		if (TryGetFixstrLength(type, out length)) return ReadString(reader, length);

		throw ExceptionUtils.BadTypeException(type, DataTypes.FixStr, DataTypes.Str8, DataTypes.Str16, DataTypes.Str32);
	}

	private string ReadString(IMsgPackReader reader, uint length)
	{
		var buffer = reader.ReadBytes(length);

		return Utf8.GetString(buffer.Array, buffer.Offset, buffer.Count);
	}

	private bool TryGetFixstrLength(DataTypes type, out uint length)
	{
		length = type - DataTypes.FixStr;
		return type.GetHighBits(3) == DataTypes.FixStr.GetHighBits(3);
	}

	private void WriteStringHeaderAndLength(IMsgPackWriter writer, int length)
	{
		if (length <= 31)
		{
			writer.Write((byte)(((byte)DataTypes.FixStr + length) % 256));
			return;
		}

		if (length <= byte.MaxValue)
		{
			writer.Write(DataTypes.Str8);
			NumberConverter.WriteByteValue((byte)length, writer);
		}
		else if (length <= ushort.MaxValue)
		{
			writer.Write(DataTypes.Str16);
			NumberConverter.WriteUShortValue((ushort)length, writer);
		}
		else
		{
			writer.Write(DataTypes.Str32);
			NumberConverter.WriteUIntValue((uint)length, writer);
		}
	}
}