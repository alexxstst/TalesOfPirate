using System.Runtime.Serialization;

namespace Corsairs.Platform.Msgpack.Converters.Generation.Exceptions;

public class GeneratorException : SerializationException
{
	public GeneratorException(string message)
		: base(message)
	{
	}
}