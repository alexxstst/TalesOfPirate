using System;
using System.Buffers;
using System.Runtime.CompilerServices;
using System.Text;

namespace Corsairs.Platform.Msgpack.Parser;

public static partial class MsgPackSpec
{
	/// <summary>
	///     Returns first element of <paramref name="ros" /> or throws.
	///     Takes possible empty segments into account.
	///     Has shortcut for sequences from single-element.
	/// </summary>
	/// <param name="ros">Sequence</param>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <returns>First element of sequence.</returns>
	/// <exception cref="IndexOutOfRangeException">Thrown when <paramref name="ros" /> is empty</exception>
	/// <exception cref="InvalidOperationException">
	///     When <see cref="ReadOnlySequence{T}.IsEmpty" /> is <c>false</c>, but
	///     sequence is still empty.
	/// </exception>
	public static T GetFirst<T>(this ReadOnlySequence<T> ros)
	{
		if (ros.IsEmpty) throw GetReadOnlySequenceIsTooShortException(1, 0);
		if (ros.IsSingleSegment) return ros.First.Span[0];

		foreach (var memory in ros)
			if (!memory.IsEmpty)
				return memory.Span[0];

		throw new InvalidOperationException(
			"We should never get here, because it means that non-empty sequence is empty.");
	}

	/// <summary>
	///     Tries to fill <paramref name="destination" /> by data from <paramref name="ros" />. Takes nature of
	///     <see cref="ReadOnlySequence{T}" />
	///     into account. Does not check <see cref="ReadOnlySequence{T}.Length" />, because it enumerates sequence.
	/// </summary>
	/// <param name="ros">Sequence of elements</param>
	/// <param name="destination">Span to copy data to.</param>
	/// <typeparam name="T">Type of element</typeparam>
	/// <returns><c>true</c> if span is fully filled, <c>false</c> otherwise.</returns>
	public static bool TryFillSpan<T>(this ReadOnlySequence<T> ros, Span<T> destination)
	{
		if (destination.IsEmpty) return true;

		var span = destination;
		foreach (var memory in ros)
		{
			var source = memory.Length > span.Length
				? memory.Span.Slice(0, span.Length)
				: memory.Span;
			source.CopyTo(span);
			span = span.Slice(source.Length);
			if (span.IsEmpty) return true;
		}

		return false;
	}

	/// <summary>
	///     Returns <see cref="int" /> length of <paramref name="ros" />.
	/// </summary>
	/// <returns>Casted length</returns>
	/// <exception cref="InvalidOperationException">
	///     Thrown if length of <paramref name="ros" /> is larger than
	///     <see cref="int.MaxValue" />
	/// </exception>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int GetIntLength<T>(this ReadOnlySequence<T> ros)
	{
		var length = ros.Length;
		if (length > int.MaxValue)
			return ThrowDataIsTooLarge(length);

		return (int)length;
	}

	[ThreadStatic]
	private static Encoder _perThreadEncoder;

	internal static Encoder GetThreadStatic(this Encoder nonDefault)
	{
		if (nonDefault != null)
		{
			nonDefault.Reset();
			return nonDefault;
		}

		var encoder = _perThreadEncoder;
		if (encoder == null)
			_perThreadEncoder = encoder = DefaultEncoding.GetEncoder();
		else
			encoder.Reset();
		return encoder;
	}

	[ThreadStatic]
	private static Decoder _perThreadDecoder;

	internal static Decoder GetThreadStatic(this Decoder nonDefault)
	{
		if (nonDefault != null)
		{
			nonDefault.Reset();
			return nonDefault;
		}

		var decoder = _perThreadDecoder;
		if (decoder == null)
			_perThreadDecoder = decoder = DefaultEncoding.GetDecoder();
		else
			decoder.Reset();
		return decoder;
	}

#if NETSTANDARD2_0 || NETFRAMEWORK
        private static unsafe void Convert(
            this Encoder encoder,
            ReadOnlySpan<char> chars,
            Span<byte> bytes,
            bool flush,
            out int charsUsed,
            out int bytesUsed,
            out bool completed)
        {
            fixed (char* charsPtr = chars)
            fixed (byte* bytesPtr = bytes)
                encoder.Convert(charsPtr, chars.Length, bytesPtr, bytes.Length, flush, out charsUsed, out bytesUsed, out completed);
        }

        private static unsafe int GetByteCount(this Encoder encoder, ReadOnlySpan<char> chars, bool flush)
        {
            if (chars.IsEmpty) return 0;

            fixed (char* charsPtr = chars)
            {
                return encoder.GetByteCount(charsPtr, chars.Length, flush);
            }
        }

        private static unsafe void Convert(
            this Decoder decoder,
            ReadOnlySpan<byte> bytes,
            Span<char> chars,
            bool flush,
            out int charsUsed,
            out int bytesUsed,
            out bool completed)
        {
            fixed (char* charsPtr = chars)
            fixed (byte* bytesPtr = bytes)
                decoder.Convert(bytesPtr, bytes.Length, charsPtr, chars.Length, flush, out charsUsed, out bytesUsed, out completed);
        }

        private static unsafe int GetChars(this Decoder decoder, ReadOnlySpan<byte> bytes, Span<char> chars, bool flush)
        {
            if (bytes.IsEmpty) return 0;

            fixed (byte* bytesPtr = bytes)
            fixed (char* charsPtr = chars)
            {
                return decoder.GetChars(bytesPtr, bytes.Length, charsPtr, chars.Length, flush);
            }
        }
#endif

#if NETSTANDARD1_4
        private static void Convert(
            this Encoder encoder,
            ReadOnlySpan<char> chars,
            Span<byte> bytes,
            bool flush,
            out int charsUsed,
            out int bytesUsed,
            out bool completed)
        {
            var (byteArray, charArray) = Allocate(bytes.Length, chars.Length);

            try
            {
                chars.CopyTo(charArray);
                encoder.Convert(charArray, 0, chars.Length, byteArray, 0, bytes.Length, flush, out charsUsed, out bytesUsed, out completed);
                var span = new Span<byte>(byteArray, 0, bytesUsed);
                span.CopyTo(bytes);
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(byteArray, true);
                ArrayPool<char>.Shared.Return(charArray, true);
            }
        }

        private static int GetByteCount(this Encoder encoder, ReadOnlySpan<char> chars, bool flush)
        {
            if (chars.IsEmpty) return 0;

            var charArray = ArrayPool<char>.Shared.Rent(chars.Length);
            try
            {
                chars.CopyTo(charArray);
                return encoder.GetByteCount(charArray, 0, chars.Length, flush);
            }
            finally
            {
                ArrayPool<char>.Shared.Return(charArray, true);
            }
        }

        private static int GetChars(this Decoder decoder, ReadOnlySpan<byte> bytes, Span<char> chars, bool flush)
        {
            if (bytes.IsEmpty) return 0;

            var (byteArray, charArray) = Allocate(bytes.Length, chars.Length);
            try
            {
                bytes.CopyTo(byteArray);
                var length = decoder.GetChars(byteArray, 0, bytes.Length, charArray, 0, flush);
                var span = new Span<char>(charArray, 0, length);
                span.CopyTo(chars);
                return length;
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(byteArray, true);
                ArrayPool<char>.Shared.Return(charArray, true);
            }
        }

        private static void Convert(
            this Decoder decoder,
            ReadOnlySpan<byte> bytes,
            Span<char> chars,
            bool flush,
            out int bytesUsed,
            out int charsUsed,
            out bool completed)
        {
            var (byteArray, charArray) = Allocate(bytes.Length, chars.Length);

            try
            {
                bytes.CopyTo(byteArray);
                decoder.Convert(
                    byteArray,
                    0,
                    bytes.Length,
                    charArray,
                    0,
                    chars.Length,
                    flush,
                    out bytesUsed,
                    out charsUsed,
                    out completed);
                var span = new Span<char>(charArray, 0, charsUsed);
                span.CopyTo(chars);
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(byteArray, true);
                ArrayPool<char>.Shared.Return(charArray, true);
            }
        }

        private static (byte[], char[]) Allocate(int bytesLength, int charsLength)
        {
            var bytes = ArrayPool<byte>.Shared.Rent(bytesLength);
            char[] chars;
            try
            {
                chars = ArrayPool<char>.Shared.Rent(charsLength);
            }
            catch
            {
                ArrayPool<byte>.Shared.Return(bytes, true);
                throw;
            }

            return (bytes, chars);
        }
#endif
}