using System;

namespace Corsairs.Platform.Msgpack.Parser;

public static class DataLengths
{
	public const int FixArrayHeader = 1;

	public const int Array16Header = 3;

	public const int Array32Header = 5;

	public const int Binary8Header = 2;

	public const int Binary16Header = 3;

	public const int Binary32Header = 5;

	public const int Boolean = 1;

	public const int Float64 = 9;

	public const int Extension8Header = 3;

	public const int Extension16Header = 4;

	public const int Extension32Header = 6;

	public const int FixExtensionHeader = 2;

	public const int FixExtension1 = 1;

	public const int FixExtension2 = 2;

	public const int FixExtension4 = 4;

	public const int FixExtension8 = 8;

	public const int FixExtension16 = 16;

	public const int TimeStamp32 = 6;

	public const int TimeStamp64 = 10;

	public const int TimeStamp96 = 15;

	public const int Float32 = 5;

	public const int Int16 = 3;

	public const int Int32 = 5;

	public const int Int64 = 9;

	public const int Int8 = 2;

	public const int FixMapHeader = 1;

	public const int Map16Header = 3;

	public const int Map32Header = 5;

	public const int NegativeFixInt = 1;

	public const int Nil = 1;

	public const int PositiveFixInt = 1;

	public const int FixStringHeader = 1;

	public const int String8Header = 2;

	public const int String16Header = 3;

	public const int String32Header = 5;

	public const int UInt8 = 2;

	public const int UInt16 = 3;

	public const int UInt32 = 5;

	public const int UInt64 = 9;

	public const byte FixMapMaxLength = DataCodes.FixMapMax - DataCodes.FixMapMin;

	public const byte FixArrayMaxLength = DataCodes.FixArrayMax - DataCodes.FixArrayMin;

	public const byte FixStringMaxLength = DataCodes.FixStringMax - DataCodes.FixStringMin;

	public static int GetBinaryHeaderLength(int length)
	{
		return length switch
		{
			<= byte.MaxValue => Binary8Header,
			<= ushort.MaxValue => Binary16Header,
			_ => Binary32Header
		};
	}

	public static int GetArrayHeaderLength(int length)
	{
		return length switch
		{
			<= FixArrayMaxLength => FixArrayHeader,
			<= ushort.MaxValue => Array16Header,
			_ => Array32Header
		};
	}

	public static int GetCompatibilityBinaryHeaderLength(int length)
	{
		return length switch
		{
			<= FixStringMaxLength => FixStringHeader,
			<= ushort.MaxValue => String16Header,
			_ => String32Header
		};
	}

	public static int GetStringHeaderLengthByBytesCount(int length)
	{
		return length switch
		{
			<= FixStringMaxLength => FixStringHeader,
			<= sbyte.MaxValue => String8Header,
			<= ushort.MaxValue => String16Header,
			_ => String32Header
		};
	}

	public static int GetMapHeaderLength(int count)
	{
		return count switch
		{
			<= FixMapMaxLength => FixMapHeader,
			<= ushort.MaxValue => Map16Header,
			_ => Map32Header
		};
	}

	public static (int min, int max) GetMinAndMaxLength(byte code)
	{
		switch (code)
		{
			case <= DataCodes.FixPositiveMax:
				return (PositiveFixInt, PositiveFixInt);
			case >= DataCodes.FixMapMin and <= DataCodes.FixMapMax:
				return (code - DataCodes.FixMapMin, code - DataCodes.FixMapMin);
			case >= DataCodes.FixArrayMin and <= DataCodes.FixArrayMax:
				return (code - DataCodes.FixArrayMin, code - DataCodes.FixArrayMin);
			case >= DataCodes.FixStringMin and <= DataCodes.FixStringMax:
				return (code - DataCodes.FixStringMin, code - DataCodes.FixStringMin);
			case >= DataCodes.FixNegativeMin:
				return (NegativeFixInt, NegativeFixInt);
			default:
				switch (code)
				{
					case DataCodes.Nil:
						return (Nil, Nil);

					case DataCodes.True:
					case DataCodes.False:
						return (Boolean, Boolean);

					case DataCodes.Binary8:
						return (0, byte.MaxValue);
					case DataCodes.Binary16:
						return (0, ushort.MaxValue);
					case DataCodes.Binary32:
						return (0, int.MaxValue);

					case DataCodes.Extension8:
						return (0, byte.MaxValue);
					case DataCodes.Extension16:
						return (0, ushort.MaxValue);
					case DataCodes.Extension32:
						return (0, int.MaxValue);

					case DataCodes.Float32:
						return (Float32, Float32);
					case DataCodes.Float64:
						return (Float64, Float64);

					case DataCodes.Int8:
						return (Int8, Int8);
					case DataCodes.UInt8:
						return (UInt8, UInt8);
					case DataCodes.Int16:
						return (Int16, Int16);
					case DataCodes.UInt16:
						return (UInt16, UInt16);
					case DataCodes.Int32:
						return (Int32, Int32);
					case DataCodes.UInt32:
						return (UInt32, UInt32);
					case DataCodes.Int64:
						return (Int64, Int64);
					case DataCodes.UInt64:
						return (UInt64, UInt64);

					case DataCodes.FixExtension1:
						return (FixExtension1, FixExtension1);
					case DataCodes.FixExtension2:
						return (FixExtension2, FixExtension2);
					case DataCodes.FixExtension4:
						return (FixExtension4, FixExtension4);
					case DataCodes.FixExtension8:
						return (FixExtension8, FixExtension8);
					case DataCodes.FixExtension16:
						return (FixExtension16, FixExtension16);

					case DataCodes.String8:
						return (0, byte.MaxValue);
					case DataCodes.String16:
						return (0, ushort.MaxValue);
					case DataCodes.String32:
						return (0, int.MaxValue);

					case DataCodes.Array16:
						return (0, ushort.MaxValue);
					case DataCodes.Array32:
						return (0, int.MaxValue);

					case DataCodes.Map16:
						return (0, ushort.MaxValue);
					case DataCodes.Map32:
						return (0, int.MaxValue);

					// case "NeverUsed" be here to have happy compiler
					default:
						throw new ArgumentOutOfRangeException(nameof(code),
							$"Can't get length for {DataCodes.NeverUsed}.");
				}
		}
	}

	public static int GetHeaderLength(byte code)
	{
		switch (code)
		{
			case <= DataCodes.FixPositiveMax:
				return PositiveFixInt;
			case >= DataCodes.FixMapMin and <= DataCodes.FixMapMax:
				return FixMapHeader;
			case >= DataCodes.FixArrayMin and <= DataCodes.FixArrayMax:
				return FixArrayHeader;
			case >= DataCodes.FixStringMin and <= DataCodes.FixStringMax:
				return FixStringHeader;
			case >= DataCodes.FixNegativeMin:
				return NegativeFixInt;
			default:
				switch (code)
				{
					case DataCodes.Nil: return Nil;

					case DataCodes.True:
					case DataCodes.False:
						return Boolean;

					case DataCodes.Binary8: return Binary8Header;
					case DataCodes.Binary16: return Binary16Header;
					case DataCodes.Binary32: return Binary32Header;

					case DataCodes.Extension8: return Extension8Header;
					case DataCodes.Extension16: return Extension16Header;
					case DataCodes.Extension32: return Extension32Header;

					case DataCodes.Float32: return Float32;
					case DataCodes.Float64: return Float64;

					case DataCodes.Int8: return Int8;
					case DataCodes.UInt8: return UInt8;
					case DataCodes.Int16: return Int16;
					case DataCodes.UInt16: return UInt16;
					case DataCodes.Int32: return Int32;
					case DataCodes.UInt32: return UInt32;
					case DataCodes.Int64: return Int64;
					case DataCodes.UInt64: return UInt64;

					case DataCodes.FixExtension1:
					case DataCodes.FixExtension2:
					case DataCodes.FixExtension4:
					case DataCodes.FixExtension8:
					case DataCodes.FixExtension16:
						return FixExtensionHeader;

					case DataCodes.String8: return String8Header;
					case DataCodes.String16: return String16Header;
					case DataCodes.String32: return String32Header;

					case DataCodes.Array16: return Array16Header;
					case DataCodes.Array32: return Array32Header;

					case DataCodes.Map16: return Map16Header;
					case DataCodes.Map32: return Map32Header;

					// case "NeverUsed" be here to have happy compiler
					default:
						throw new ArgumentOutOfRangeException(nameof(code),
							$"Can't get header length for {DataCodes.NeverUsed}.");
				}
		}
	}
}