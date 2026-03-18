namespace Corsairs.Platform.Msgpack;

public interface IMsgPackWriter
{
	void Write(DataTypes dataType);

	void Write(byte value);

	void Write(byte[] array);

	void WriteArrayHeader(uint length);

	void WriteMapHeader(uint length);
}