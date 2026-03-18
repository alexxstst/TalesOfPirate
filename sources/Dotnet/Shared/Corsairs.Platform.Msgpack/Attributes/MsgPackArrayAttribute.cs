using System;

namespace Corsairs.Platform.Msgpack.Attributes;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Interface | AttributeTargets.Struct, Inherited = false)]
public class MsgPackArrayAttribute : Attribute
{
}