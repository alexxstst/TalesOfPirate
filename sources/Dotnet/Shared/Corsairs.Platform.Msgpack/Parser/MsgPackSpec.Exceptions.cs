using System;
using System.Linq;
using System.Runtime.CompilerServices;
using static Corsairs.Platform.Msgpack.Parser.DataCodes;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Encode/Decode Utility of MessagePack Spec.
///     https://github.com/msgpack/msgpack/blob/master/spec.md
/// </summary>
public static partial class MsgPackSpec
{
	private static Exception GetReadOnlySequenceIsTooShortException(int expected, long sequenceLength)
	{
		return new IndexOutOfRangeException(
			$"ReadOnlySequence is too short. Expected: {expected}, actual length: {sequenceLength}"
		);
	}

	private static Exception GetInvalidStringException()
	{
		return new InvalidOperationException(
			"String conversion didn't use all bytes, buffer is corrupted"
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongRangeCodeException(byte code, byte min, byte max)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)min}(0x{min:x2}) <= code <= {(DataTypes)max}(0x{max:x2})."
		);
	}

	private static byte ThrowWrongRangeCodeException(sbyte code, sbyte min, sbyte max)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)min}(0x{min:x2}) <= code <= {(DataTypes)max}(0x{max:x2})."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongCodeException(byte code, byte expected)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)expected}(0x{expected:x2})."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongCodeException(byte code, byte a, byte b)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)a}(0x{a:x2}) or {(DataTypes)b}(0x{b:x2})."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongCodeException(byte code, byte a, byte b, byte c)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)a}(0x{a:x2}), {(DataTypes)b}(0x{b:x2}) or {(DataTypes)c}(0x{c:x2})."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongCodeException(byte code, params byte[] expected)
	{
		throw new InvalidOperationException(
			$"Wrong data code: {(DataTypes)code}(0x{code:x2}). Expected: {string.Join(", ", expected.Select(x => $"{(DataTypes)x}(0x{x:x2})"))}."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowLengthShouldBeNonNegative(int length)
	{
		throw new InvalidOperationException(
			$"Length should be nonnegative. Was {length}."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowDataIsTooLarge(long length)
	{
		throw new InvalidOperationException(
			$@"You can't create arrays or string longer than int.MaxValue in .net. Packet length was: {length}.
See https://blogs.msdn.microsoft.com/joshwil/2005/08/10/bigarrayt-getting-around-the-2gb-array-size-limit/"
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowDataIsTooLarge(int dataLength, int maxLength, string dataName, byte dataCode)
	{
		throw new InvalidOperationException(
			$"Data is {dataLength} bytes and it is too large for {dataName}(0x{dataCode:x2}). Maximum is {maxLength}."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowDataIsTooLarge(int dataLength, int maxLength, string dataName, byte minCode, byte maxCode)
	{
		throw new InvalidOperationException(
			$"Data is {dataLength} bytes and it is too large for {dataName}(0x{minCode:x2} - 0x{maxCode:x2}). Maximum is {maxLength}."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongArrayHeader(byte code)
	{
		throw new InvalidOperationException(
			$"Wrong array data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)FixArrayMin}(0x{FixArrayMin:x2}) <= code <= {(DataTypes)FixArrayMax}(0x{FixArrayMax:x2}) or {(DataTypes)Array16}(0x{Array16:x2}) or {(DataTypes)Array32}(0x{Array32:x2})."
		);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private static byte ThrowWrongMapHeader(byte code)
	{
		throw new InvalidOperationException(
			$"Wrong map data code: {(DataTypes)code}(0x{code:x2}). Expected: {(DataTypes)FixMapMin}(0x{FixMapMin:x2}) <= code <= {(DataTypes)FixMapMax}(0x{FixMapMax:x2}) or {(DataTypes)Map16}(0x{Map16:x2}) or {(DataTypes)Map32}(0x{Map32:x2})."
		);
	}

	private static byte ThrowCantReadEmptyBufferException()
	{
		throw new InvalidOperationException(
			"We need at least 1 byte to read from buffer!"
		);
	}

	private static byte ThrowValueIsTooLargeException<T1, T2>(T1 value, T2 maxValue)
	{
		throw new InvalidOperationException(
			$"Value is {value}, maximum is {maxValue}"
		);
	}

	private static byte ThrowWrongIntCodeException(byte code, params byte[] expected)
	{
		throw new InvalidOperationException(
			$"Wrong int data code: {(DataTypes)code}(0x{code:x2}). Expected: NegativeFixNum(0x{unchecked((byte)FixNegativeMinSByte):x2}) <= x <= PositiveFixNum(0x{FixPositiveMax:x2}) or x ⊂ ({string.Join(", ", expected.Select(x => $"{(DataTypes)x}(0x{x:x2})"))})."
		);
	}

	private static byte ThrowWrongUIntCodeException(byte code, params byte[] expected)
	{
		throw new InvalidOperationException(
			$"Wrong uint data code: {(DataTypes)code}(0x{code:x2}). Expected: 0 <= x <= PositiveFixNum(0x{FixPositiveMax:x2}) or x ⊂ ({string.Join(", ", expected.Select(x => $"{(DataTypes)x}(0x{x:x2})"))})."
		);
	}

	private static byte ThrowUnsignedIntException(long value)
	{
		throw new InvalidOperationException(
			$"Value {value} should be greater or equal to zero."
		);
	}

	private static byte ThrowWrongExtensionTypeException(sbyte extension, sbyte expected)
	{
		throw new InvalidOperationException(
			$"Wrong extension type: 0x{extension:x2}. Expected: 0x{expected:x2}."
		);
	}

	private static byte ThrowWrongExtensionLengthException(byte length, int expected)
	{
		throw new InvalidOperationException(
			$"Wrong extension length: {length}. Expected: {expected}."
		);
	}
}