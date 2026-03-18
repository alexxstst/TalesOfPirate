namespace Corsairs.Platform.Msgpack.Converters;

public class MsgPackTokenConverter : IMsgPackConverter<MsgPackToken>
{
	private MsgPackContext _context;

	public void Initialize(MsgPackContext context)
	{
		_context = context;
	}

	public void Write(MsgPackToken value, IMsgPackWriter writer)
	{
		writer.Write(value.RawBytes);
	}

	public MsgPackToken Read(IMsgPackReader reader)
	{
		var rawBytes = reader.ReadToken();
		return new MsgPackToken(_context, rawBytes);
	}
}