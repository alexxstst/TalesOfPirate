using System.Runtime.CompilerServices;

namespace Corsairs.Platform.Msgpack.Parser;

public static class DataCodes
{
	public const byte FixPositiveMin = 0x00;
	public const byte FixPositiveMax = 0x7f;

	public const byte FixMapMin = 0x80;
	public const byte FixMapMax = 0x8f;

	public const byte FixArrayMin = 0x90;
	public const byte FixArrayMax = 0x9f;

	public const byte FixStringMin = 0xa0;
	public const byte FixStringMax = 0xbf;

	public const byte Nil = 0xc0;

	public const byte NeverUsed = 0xc1;

	public const byte False = 0xc2;
	public const byte True = 0xc3;

	public const byte Binary8 = 0xc4;
	public const byte Binary16 = 0xc5;
	public const byte Binary32 = 0xc6;

	public const byte Extension8 = 0xc7;
	public const byte Extension16 = 0xc8;
	public const byte Extension32 = 0xc9;

	public const byte Float32 = 0xca;
	public const byte Float64 = 0xcb;

	public const byte UInt8 = 0xcc;
	public const byte UInt16 = 0xcd;
	public const byte UInt32 = 0xce;
	public const byte UInt64 = 0xcf;

	public const byte Int8 = 0xd0;
	public const byte Int16 = 0xd1;
	public const byte Int32 = 0xd2;
	public const byte Int64 = 0xd3;

	public const byte FixExtension1 = 0xd4;
	public const byte FixExtension2 = 0xd5;
	public const byte FixExtension4 = 0xd6;
	public const byte FixExtension8 = 0xd7;
	public const byte FixExtension16 = 0xd8;

	public const byte String8 = 0xd9;
	public const byte String16 = 0xda;
	public const byte String32 = 0xdb;

	public const byte Array16 = 0xdc;
	public const byte Array32 = 0xdd;

	public const byte Map16 = 0xde;
	public const byte Map32 = 0xdf;

	public const byte FixNegativeMin = 0xe0;
	public const byte FixNegativeMax = 0xff;

	public const sbyte FixNegativeMinSByte = -32;
	public const sbyte FixNegativeMaxSByte = -1;

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static DataTypes FromByte(byte code)
	{
		return (DataTypes)code;
	}
}