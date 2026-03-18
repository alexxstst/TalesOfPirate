using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Corsairs.Platform.Msgpack.Parser;

/// <summary>
///     Methods for working with float
/// </summary>
public static partial class MsgPackSpec
{
	/// <summary>
	///     Write float <paramref name="value" /> into <paramref name="buffer" />.
	/// </summary>
	/// <returns>Count of bytes, written to <paramref name="buffer" />.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFixFloat32(Span<byte> buffer, float value)
	{
		var binary = new FloatBinary(value);

		if (BitConverter.IsLittleEndian)
		{
			buffer[4] = binary.Byte0;
			buffer[3] = binary.Byte1;
			buffer[2] = binary.Byte2;
			buffer[1] = binary.Byte3;
		}
		else
		{
			buffer[4] = binary.Byte3;
			buffer[3] = binary.Byte2;
			buffer[2] = binary.Byte1;
			buffer[1] = binary.Byte0;
		}

		buffer[0] = DataCodes.Float32;

		return DataLengths.Float32;
	}

	/// <summary>
	///     Tries to write float value into <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to write.</param>
	/// <param name="value">Value to write</param>
	/// <param name="wroteSize">
	///     Count of bytes, written to <paramref name="buffer" />. If return value is <c>false</c>, value
	///     is unspecified.
	/// </param>
	/// <returns><c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFixFloat32(Span<byte> buffer, float value, out int wroteSize)
	{
		wroteSize = DataLengths.Float32;
		if (buffer.Length < wroteSize) return false;

		WriteFixFloat32(buffer, value);

		return true;
	}

	/// <summary>
	///     Reads float value from <paramref name="buffer" />.
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="readSize">Count of bytes, read from <paramref name="buffer" />.</param>
	/// <returns>Float value.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static float ReadFixFloat32(ReadOnlySpan<byte> buffer, out int readSize)
	{
		readSize = DataLengths.Float32;
		if (buffer[0] != DataCodes.Float32) ThrowWrongCodeException(buffer[0], DataCodes.Float32);
		return new FloatBinary(buffer.Slice(1)).Value;
	}

	/// <summary>
	///     Tries to read from <paramref name="buffer" />
	/// </summary>
	/// <param name="buffer">Buffer to read from.</param>
	/// <param name="value">Value, read from <paramref name="buffer" />. If return value is false, value is unspecified.</param>
	/// <param name="dataLength">
	///     Count of bytes, read from <paramref name="buffer" />. If return value is false, value is
	///     unspecified.
	/// </param>
	/// <returns>
	///     <c>true</c>, if everything is ok, <c>false</c> if <paramref name="buffer" /> is too small or
	///     <paramref name="buffer" />[0] is not <see cref="DataCodes.Float32" />.
	/// </returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFixFloat32(ReadOnlySpan<byte> buffer, out float value, out int dataLength)
	{
		dataLength = DataLengths.Float32;
		value = default;
		if (buffer.Length < dataLength) return false;
		var result = buffer[0] == DataCodes.Float32;
		var binary = new FloatBinary(buffer.Slice(1));
		value = binary.Value;
		return result;
	}

	/// <summary>
	///     Calls <see cref="WriteFixFloat32" />. Provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WriteFloat(Span<byte> buffer, float value)
	{
		return WriteFixFloat32(buffer, value);
	}

	/// <summary>
	///     Calls <see cref="TryWriteFixFloat32" />. Provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWriteFloat(Span<byte> buffer, float value, out int wroteSize)
	{
		return TryWriteFixFloat32(buffer, value, out wroteSize);
	}

	/// <summary>
	///     Calls <see cref="ReadFixFloat32" />. Provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static float ReadFloat(ReadOnlySpan<byte> buffer, out int readSize)
	{
		return ReadFixFloat32(buffer, out readSize);
	}

	/// <summary>
	///     Calls <see cref="TryReadFixFloat32" />. Provided for consistency with other types.
	/// </summary>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryReadFloat(ReadOnlySpan<byte> buffer, out float value, out int readSize)
	{
		return TryReadFixFloat32(buffer, out value, out readSize);
	}

	[StructLayout(LayoutKind.Explicit)]
	private struct FloatBinary
	{
		[FieldOffset(0)]
		public readonly float Value;

		[FieldOffset(0)]
		public readonly byte Byte0;

		[FieldOffset(1)]
		public readonly byte Byte1;

		[FieldOffset(2)]
		public readonly byte Byte2;

		[FieldOffset(3)]
		public readonly byte Byte3;

		public FloatBinary(float f)
		{
			this = default;
			Value = f;
		}

		public FloatBinary(ReadOnlySpan<byte> bytes)
		{
			Value = 0;
			if (BitConverter.IsLittleEndian)
			{
				Byte0 = bytes[3];
				Byte1 = bytes[2];
				Byte2 = bytes[1];
				Byte3 = bytes[0];
			}
			else
			{
				Byte3 = bytes[3];
				Byte2 = bytes[2];
				Byte1 = bytes[1];
				Byte0 = bytes[0];
			}
		}
	}
}