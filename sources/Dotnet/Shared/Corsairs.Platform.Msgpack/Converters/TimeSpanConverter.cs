using System;

namespace Corsairs.Platform.Msgpack.Converters;

public class TimeSpanConverter : IMsgPackConverter<TimeSpan>
{
	private Lazy<IMsgPackConverter<long>> _longConverter = new();

	public void Initialize(MsgPackContext context)
	{
		_longConverter = new Lazy<IMsgPackConverter<long>>(context.GetConverter<long>);
	}

	public void Write(TimeSpan value, IMsgPackWriter writer)
	{
		_longConverter.Value.Write(value.Ticks, writer);
	}

	public TimeSpan Read(IMsgPackReader reader)
	{
		var longValue = _longConverter.Value.Read(reader);

		return TimeSpan.FromTicks(longValue);
	}
}