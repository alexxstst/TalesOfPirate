using System.Collections.Generic;

namespace Corsairs.Platform.Msgpack.Converters;

public class ReadOnlyListConverter<TArray, TElement> : ArrayConverterBase<TArray, TElement>
	where TArray : IReadOnlyList<TElement>
{
	public override void Write(TArray value, IMsgPackWriter writer)
	{
		if (value == null)
		{
			Context.NullConverter.Write(value, writer);
			return;
		}

		writer.WriteArrayHeader((uint)value.Count);

		foreach (var element in value) ElementConverter.Write(element, writer);
	}

	public override TArray Read(IMsgPackReader reader)
	{
		throw ExceptionUtils.CantReadReadOnlyCollection(typeof(TArray));
	}
}