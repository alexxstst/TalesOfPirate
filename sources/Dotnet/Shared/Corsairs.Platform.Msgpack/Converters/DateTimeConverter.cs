using System;

namespace Corsairs.Platform.Msgpack.Converters;

public class DateTimeConverter : IMsgPackConverter<DateTime>, IMsgPackConverter<DateTimeOffset>
{
	private Lazy<IMsgPackConverter<long>> _longConverter;
	private Lazy<IMsgPackConverter<ulong>> _ulongConverter;

	public void Initialize(MsgPackContext context)
	{
		_ulongConverter = new Lazy<IMsgPackConverter<ulong>>(context.GetConverter<ulong>);
		_longConverter = new Lazy<IMsgPackConverter<long>>(context.GetConverter<long>);
	}

	public void Write(DateTime value, IMsgPackWriter writer)
	{
		var longValue = DateTimeUtils.FromDateTime(value);

		_longConverter.Value.Write(longValue, writer);
	}

	DateTime IMsgPackConverter<DateTime>.Read(IMsgPackReader reader)
	{
		var ulongValue = _ulongConverter.Value.Read(reader);
		return DateTimeUtils.ToDateTime((long)ulongValue);
	}

	public void Write(DateTimeOffset value, IMsgPackWriter writer)
	{
		var longValue = DateTimeUtils.FromDateTimeOffset(value);

		_longConverter.Value.Write(longValue, writer);
	}

	DateTimeOffset IMsgPackConverter<DateTimeOffset>.Read(IMsgPackReader reader)
	{
		var ulongValue = _ulongConverter.Value.Read(reader);
		return DateTimeUtils.ToDateTimeOffset((long)ulongValue);
	}
}