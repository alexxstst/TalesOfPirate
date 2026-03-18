using System.IO;

namespace Corsairs.Platform.Msgpack.Converters;

public class NullableConverter<T> : IMsgPackConverter<T?>
	where T : struct
{
	private MsgPackContext _context;

	private IMsgPackConverter<T> _converter;

	public T? Read(IMsgPackReader reader)
	{
		var type = reader.ReadDataType();

		if (type == DataTypes.Null)
			return null;

		reader.Seek(-1, SeekOrigin.Current);

		return _converter.Read(reader);
	}

	public void Write(T? value, IMsgPackWriter writer)
	{
		if (value.HasValue)
			_converter.Write(value.Value, writer);
		else
			_context.NullConverter.Write(null, writer);
	}

	public void Initialize(MsgPackContext context)
	{
		_context = context;
		_converter = context.GetConverter<T>();
	}
}