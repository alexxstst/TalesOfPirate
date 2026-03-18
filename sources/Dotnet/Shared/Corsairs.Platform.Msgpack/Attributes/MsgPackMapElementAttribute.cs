using System;

namespace Corsairs.Platform.Msgpack.Attributes;

[AttributeUsage(AttributeTargets.Property)]
public class MsgPackMapElementAttribute : Attribute
{
	public MsgPackMapElementAttribute(string name)
	{
		Name = name;
	}

	public string Name { get; }
}