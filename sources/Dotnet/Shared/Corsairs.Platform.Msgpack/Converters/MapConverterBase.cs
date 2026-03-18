namespace Corsairs.Platform.Msgpack.Converters;

public abstract class MapConverterBase<TMap, TKey, TValue> : IMsgPackConverter<TMap>
{
	protected MsgPackContext Context { get; private set; }

	protected IMsgPackConverter<TValue> ValueConverter { get; private set; }

	protected IMsgPackConverter<TKey> KeyConverter { get; private set; }

	public void Initialize(MsgPackContext context)
	{
		var keyConverter = context.GetConverter<TKey>();
		var valueConverter = context.GetConverter<TValue>();
		if (keyConverter == null) throw ExceptionUtils.NoConverterForCollectionElement(typeof(TKey), "key");

		if (valueConverter == null) throw ExceptionUtils.NoConverterForCollectionElement(typeof(TValue), "value");

		KeyConverter = keyConverter;
		ValueConverter = valueConverter;
		Context = context;
	}

	public abstract void Write(TMap value, IMsgPackWriter writer);

	public abstract TMap Read(IMsgPackReader reader);
}