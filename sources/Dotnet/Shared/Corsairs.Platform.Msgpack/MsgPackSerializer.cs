using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Runtime.Serialization;

namespace Corsairs.Platform.Msgpack;

public static class MsgPackSerializer
{
	public static byte[] Serialize<T>(T data)
	{
		return Serialize(data, new MsgPackContext());
	}

	public static byte[] Serialize<T>(T data, [NotNull] MsgPackContext context)
	{
		var memoryStream = new MemoryStream();
		using (var writer = new MsgPackMemoryStreamWriter(memoryStream))
		{
			var converter = GetConverter<T>(context);
			converter.Write(data, writer);
			return memoryStream.ToArray();
		}
	}

	public static void Serialize<T>(T data, MemoryStream stream)
	{
		Serialize(data, stream, new MsgPackContext());
	}

	public static void Serialize<T>(T data, MemoryStream stream, [NotNull] MsgPackContext context)
	{
		using (var writer = new MsgPackMemoryStreamWriter(stream, false))
		{
			var converter = GetConverter<T>(context);
			converter.Write(data, writer);
		}
	}

	public static MsgPackToken SerializeToToken<T>(T data, [NotNull] MsgPackContext context)
	{
		return new MsgPackToken(context, Serialize(data, context));
	}

	public static MsgPackToken SerializeToToken<T>(T data)
	{
		return SerializeToToken(data, new MsgPackContext());
	}

	public static T Deserialize<T>(byte[] data)
	{
		return Deserialize<T>(data, new MsgPackContext());
	}

	public static T Deserialize<T>(MsgPackToken token)
	{
		return Deserialize<T>(token, new MsgPackContext());
	}

	public static T Deserialize<T>(MsgPackToken token, [NotNull] MsgPackContext context)
	{
		return Deserialize<T>(token.RawBytes, context);
	}

	public static T Deserialize<T>(byte[] data, [NotNull] MsgPackContext context)
	{
		var reader = new MsgPackByteArrayReader(data);
		var converter = GetConverter<T>(context);
		return converter.Read(reader);
	}

	public static T Deserialize<T>(MemoryStream stream)
	{
		return Deserialize<T>(stream, new MsgPackContext());
	}

	public static T Deserialize<T>(MemoryStream stream, [NotNull] MsgPackContext context)
	{
		using (var reader = new MsgPackMemoryStreamReader(stream, false))
		{
			var converter = GetConverter<T>(context);
			return converter.Read(reader);
		}
	}

	private static IMsgPackConverter<T> GetConverter<T>(MsgPackContext context)
	{
		var converter = context.GetConverter<T>();

		if (converter == null) throw new SerializationException($"Provide converter for {typeof(T).Name}");

		return converter;
	}
}