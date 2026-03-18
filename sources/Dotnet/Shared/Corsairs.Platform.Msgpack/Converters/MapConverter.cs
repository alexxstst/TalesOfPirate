using System.Collections.Generic;

namespace Corsairs.Platform.Msgpack.Converters;

internal class MapConverter<TMap, TKey, TValue> : MapConverterBase<TMap, TKey, TValue>
	where TMap : IDictionary<TKey, TValue>
{
	public override void Write(TMap value, IMsgPackWriter writer)
	{
		if (value == null)
		{
			Context.NullConverter.Write(value, writer);
			return;
		}

		writer.WriteMapHeader((uint)value.Count);

		foreach (var element in value)
		{
			KeyConverter.Write(element.Key, writer);
			ValueConverter.Write(element.Value, writer);
		}
	}

	public override TMap Read(IMsgPackReader reader)
	{
		var length = reader.ReadMapLength();
		return length.HasValue ? ReadMap(reader, length.Value) : default;
	}

	private TMap ReadMap(IMsgPackReader reader, uint length)
	{
		var map = (TMap)Context.GetObjectActivator(typeof(TMap))();

		for (var i = 0u; i < length; i++)
		{
			var key = KeyConverter.Read(reader);
			var value = ValueConverter.Read(reader);

			map[key] = value;
		}

		return map;
	}
}