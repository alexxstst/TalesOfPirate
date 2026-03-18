namespace Corsairs.Platform.Msgpack.Parser;

public enum DataFamily
{
	Integer,
	Nil,
	Boolean,
	Float,
	String,
	Binary,
	Array,
	Map,
	Extension,
	NeverUsed
}