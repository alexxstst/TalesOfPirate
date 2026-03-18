namespace Corsairs.Platform.Msgpack.Converters;

internal class BoolConverter : IMsgPackConverter<bool>
{
	public void Initialize(MsgPackContext context)
	{
	}

	public void Write(bool value, IMsgPackWriter writer)
	{
		writer.Write(value ? DataTypes.True : DataTypes.False);
	}

	public bool Read(IMsgPackReader reader)
	{
		var type = reader.ReadDataType();

		switch (type)
		{
			case DataTypes.True:
				return true;

			case DataTypes.False:
				return false;

			default:
				throw ExceptionUtils.BadTypeException(type, DataTypes.True, DataTypes.False);
		}
	}
}