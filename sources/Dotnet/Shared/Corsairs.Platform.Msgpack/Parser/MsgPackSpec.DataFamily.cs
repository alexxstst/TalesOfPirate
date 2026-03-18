using System;
using System.Buffers;
using static Corsairs.Platform.Msgpack.Parser.DataCodes;

namespace Corsairs.Platform.Msgpack.Parser;

public static partial class MsgPackSpec
{
	/// <summary>
	///     Provides mapping of first byte of <paramref name="buffer" /> high level <see cref="DataFamily" />.
	///     Will be useful for writing converters for complex types.
	/// </summary>
	public static DataFamily GetDataFamily(ReadOnlySpan<byte> buffer)
	{
		return GetDataFamily(buffer[0]);
	}

	/// <summary>
	///     Provides mapping of first byte of <paramref name="sequence" /> high level <see cref="DataFamily" />.
	///     Will be useful for writing converters for complex types.
	/// </summary>
	public static DataFamily GetDataFamily(ReadOnlySequence<byte> sequence)
	{
		return GetDataFamily(sequence.GetFirst());
	}

	/// <summary>
	///     Provides mapping to high level <see cref="DataFamily" />. Will be useful for writing converters for complex types.
	/// </summary>
	public static DataFamily GetDataFamily(byte code)
	{
		if (code <= FixPositiveMax) return DataFamily.Integer;
		if (FixMapMin <= code && code <= FixMapMax) return DataFamily.Map;
		if (FixArrayMin <= code && code <= FixArrayMax) return DataFamily.Array;
		if (FixStringMin <= code && code <= FixStringMax) return DataFamily.String;
		if (FixNegativeMin <= code) return DataFamily.Integer;
		switch (code)
		{
			case Nil: return DataFamily.Nil;
			case True:
			case False:
				return DataFamily.Boolean;

			case Binary8:
			case Binary16:
			case Binary32:
				return DataFamily.Binary;

			case Extension8:
			case Extension16:
			case Extension32:
				return DataFamily.Extension;

			case Float32:
			case Float64:
				return DataFamily.Float;

			case UInt8:
			case DataCodes.UInt16:
			case DataCodes.UInt32:
			case DataCodes.UInt64:
			case Int8:
			case DataCodes.Int16:
			case DataCodes.Int32:
			case DataCodes.Int64:
				return DataFamily.Integer;

			case FixExtension1:
			case FixExtension2:
			case FixExtension4:
			case FixExtension8:
			case FixExtension16:
				return DataFamily.Extension;

			case String8:
			case String16:
			case String32:
				return DataFamily.String;

			case Array16:
			case Array32:
				return DataFamily.Array;

			case Map16:
			case Map32:
				return DataFamily.Map;

			// case "NeverUsed" be here to have happy compiler
			default:
				return DataFamily.NeverUsed;
		}
	}
}