using System;
using System.Linq;
using System.Reflection;

namespace Corsairs.Platform.Msgpack.Converters.Generation.Exceptions;

public class DuplicateArrayElementException : GeneratorException
{
	public DuplicateArrayElementException(Type type, int order, PropertyInfo[] properties)
		: base(
			$"Duplicate order '{order}' in type '{type}'. Properties: {string.Join(", ", properties.Select(x => $"{x.DeclaringType.Name}.{x.Name}"))}.")
	{
		Type = type;
		Order = order;
		Properties = properties;
	}

	public Type Type { get; }

	public int Order { get; }

	public PropertyInfo[] Properties { get; }
}
