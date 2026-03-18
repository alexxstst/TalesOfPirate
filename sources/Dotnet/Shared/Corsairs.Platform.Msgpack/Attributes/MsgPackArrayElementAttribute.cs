using System;

namespace Corsairs.Platform.Msgpack.Attributes;

[AttributeUsage(AttributeTargets.Property)]
public class MsgPackArrayElementAttribute : Attribute
{
	public MsgPackArrayElementAttribute(int order)
	{
		Order = order;
	}

	public int Order { get; }
}