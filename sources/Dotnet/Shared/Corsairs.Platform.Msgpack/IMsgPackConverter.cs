using System.Diagnostics.CodeAnalysis;

namespace Corsairs.Platform.Msgpack;

public interface IMsgPackConverter
{
	void Initialize([NotNull] MsgPackContext context);
}

public interface IMsgPackConverter<T> : IMsgPackConverter
{
	void Write(T value, [NotNull] IMsgPackWriter writer);

	T Read([NotNull] IMsgPackReader reader);
}