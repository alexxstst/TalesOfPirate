using System;
using System.Linq;
using System.Reflection;

namespace Corsairs.Platform.Msgpack.Converters.Generation.Exceptions;

public class DuplicateMapElementException : GeneratorException
{
	public DuplicateMapElementException(Type type, string key, PropertyInfo[] properties)
		: base(
			$"Duplicate key '{key}' in type '{type}'. Properties: {string.Join(", ", properties.Select(x => $"{x.DeclaringType.Name}.{x.Name}"))}.")
	{
		Type = type;
		Key = key;
		Properties = properties;
	}

	public Type Type { get; }

	public string Key { get; }

	public PropertyInfo[] Properties { get; }
}