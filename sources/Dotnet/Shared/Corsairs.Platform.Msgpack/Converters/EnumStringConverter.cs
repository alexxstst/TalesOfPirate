using System;

namespace Corsairs.Platform.Msgpack.Converters;

internal class EnumStringConverter<T> : IMsgPackConverter<T>
{
	private Lazy<IMsgPackConverter<string>> _stringConverter;

	public void Initialize(MsgPackContext context)
	{
		_stringConverter = new Lazy<IMsgPackConverter<string>>(context.GetConverter<string>);
	}

	public void Write(T value, IMsgPackWriter writer)
	{
		_stringConverter.Value.Write(value.ToString(), writer);
	}

	public T Read(IMsgPackReader reader)
	{
		return (T)Enum.Parse(typeof(T), _stringConverter.Value.Read(reader));
	}
}