using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with double
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write double <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixFloat64(Span<byte> buffer, double value)
	{
		var binary = new DoubleBinary(value);

		if (BitConverter.IsLittleEndian)
		{
			buffer[8] = binary.Byte0;
			buffer[7] = binary.Byte1;
			buffer[6] = binary.Byte2;
			buffer[5] = binary.Byte3;
			buffer[4] = binary.Byte4;
			buffer[3] = binary.Byte5;
			buffer[2] = binary.Byte6;
			buffer[1] = binary.Byte7;
		}
		else
		{
			buffer[8] = binary.Byte7;
			buffer[7] = binary.Byte6;
			buffer[6] = binary.Byte5;
			buffer[5] = binary.Byte4;
			buffer[4] = binary.Byte3;
			buffer[3] = binary.Byte2;
			buffer[2] = binary.Byte1;
			buffer[1] = binary.Byte0;
		}

		buffer[0] = DataCodes.Float64;

		return DataLengths.Float64;
	}

	/// <summary>
	///     Tries to write double value into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixFloat64(Span<byte> buffer, double value, out int wroteSize)
	{
		wroteSize = DataLengths.Float64;
		if (buffer.Length < wroteSize) return false;

		WriteFixFloat64(buffer, value);

		return true;
	}

	/// <summary>
	///     Reads double value from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Double value.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static double ReadFixFloat64(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Float64;
		if (buffer[0] != DataCodes.Float64) ThrowWrongCodeException(buffer[0], DataCodes.Float64);
		return new DoubleBinary(buffer.Slice(1, 8)).Value;
	}

	/// <summary>
	///     Tries to read from <paramref name="buffer" />
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value, read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="readSize">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.Float64" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixFloat64(ReadOnlySpan<byte> buffer, out double value, out int readSize)
	{
		readSize = DataLengths.Float64;
		value = default;
		if (buffer.Length < readSize) return false;
		if (buffer[0] != DataCodes.Float64) return false;
		value = new DoubleBinary(buffer.Slice(1, 8)).Value;
		return true;
	}

	/// <summary>
	///     <see cref="WriteFixFloat64" />. Method provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteDouble(Span<byte> buffer, double value)
	{
		return WriteFixFloat64(buffer, value);
	}

	/// <summary>
	///     <see cref="TryWriteFixFloat64" />. Method provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteDouble(Span<byte> buffer, double value, out int wroteSize)
	{
		return TryWriteFixFloat64(buffer, value, out wroteSize);
	}

	/// <summary>
	///     Read double from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Double value.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static double ReadDouble(ReadOnlySpan<byte> buffer, out int readSize)
	{
		var code = buffer[0];
		switch (code)
		{
			case DataCodes.Float32:
				return ReadFixFloat32(buffer, out readSize);
			case DataCodes.Float64:
				return ReadFixFloat64(buffer, out readSize);
			default:
				readSize = 0;
				return ThrowWrongCodeException(code, DataCodes.Float64, DataCodes.Float32);
		}
	}

	/// <summary>
	///     Tries to read double value from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="dataLength">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok. <c>false</c> if buffer is too small, or <paramref name="buffer[0]" /> is not
	///     <see cref="DataCodes.Float32" /> or <see cref="DataCodes.Float64" />
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadDouble(ReadOnlySpan<byte> buffer, out double value, out int dataLength)
	{
		if (buffer.Length < 1)
		{
			value = default;
			dataLength = default;
			return false;
		}

		var code = buffer[0];
		switch (code)
		{
			case DataCodes.Float32:
				var result = TryReadFixFloat32(buffer, out var floatValue, out dataLength);
				value = floatValue;
				return result;
			case DataCodes.Float64:
				return TryReadFixFloat64(buffer, out value, out dataLength);
			default:
				value = default;
				dataLength = default;
				return false;
		}
	}

	[StructLayout(LayoutKind.Explicit)]
	private struct DoubleBinary
	{
		[FieldOffset(0)]
		public readonly double Value;

		[FieldOffset(0)]
		public readonly byte Byte0;

		[FieldOffset(1)]
		public readonly byte Byte1;

		[FieldOffset(2)]
		public readonly byte Byte2;

		[FieldOffset(3)]
		public readonly byte Byte3;

		[FieldOffset(4)]
		public readonly byte Byte4;

		[FieldOffset(5)]
		public readonly byte Byte5;

		[FieldOffset(6)]
		public readonly byte Byte6;

		[FieldOffset(7)]
		public readonly byte Byte7;

		public DoubleBinary(double f)
		{
			this = default;
			Value = f;
		}

		public DoubleBinary(ReadOnlySpan<byte> bytes)
		{
			Value = 0;
			if (BitConverter.IsLittleEndian)
			{
				Byte0 = bytes[7];
				Byte1 = bytes[6];
				Byte2 = bytes[5];
				Byte3 = bytes[4];
				Byte4 = bytes[3];
				Byte5 = bytes[2];
				Byte6 = bytes[1];
				Byte7 = bytes[0];
			}
			else
			{
				Byte7 = bytes[7];
				Byte6 = bytes[6];
				Byte5 = bytes[5];
				Byte4 = bytes[4];
				Byte3 = bytes[3];
				Byte2 = bytes[2];
				Byte1 = bytes[1];
				Byte0 = bytes[0];
			}
		}
	}
}