using System;
using System.Collections.Generic;

namespace Corsairs.Platform.Msgpack;

public class MsgPackToken
{
	private static readonly MsgPackContext DefaultContext = new();

	private readonly MsgPackContext _context;

	public MsgPackToken(MsgPackContext context, byte[] rawBytes)
	{
		RawBytes = rawBytes;
		_context = context;
	}

	public MsgPackToken(byte[] value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(bool value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(string value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(ulong value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(long value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(float value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(double value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(DateTime value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(DateTimeOffset value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(TimeSpan value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(MsgPackToken[] value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	public MsgPackToken(Dictionary<MsgPackToken, MsgPackToken> value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(bool? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(ulong? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(uint value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(long? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(uint? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(float? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(double? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(DateTime? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(DateTimeOffset? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	private MsgPackToken(TimeSpan? value, MsgPackContext context = null)
	{
		_context = context;
		RawBytes = MsgPackSerializer.Serialize(value, _context ?? DefaultContext);
	}

	internal byte[] RawBytes { get; }

	private T CastTokenToValue<T>()
	{
		return MsgPackSerializer.Deserialize<T>(RawBytes, _context ?? DefaultContext);
	}

	#region Bool type conversion

	public static explicit operator MsgPackToken(bool b)
	{
		return new MsgPackToken(b);
	}

	public static explicit operator bool(MsgPackToken token)
	{
		return token.CastTokenToValue<bool>();
	}

	#endregion

	#region String type conversion

	public static explicit operator MsgPackToken(string str)
	{
		return new MsgPackToken(str);
	}

	public static explicit operator string(MsgPackToken token)
	{
		return token.CastTokenToValue<string>();
	}

	#endregion

	#region byte[] type conversion

	public static explicit operator MsgPackToken(byte[] data)
	{
		return new MsgPackToken(data);
	}

	public static explicit operator byte[](MsgPackToken token)
	{
		return token.CastTokenToValue<byte[]>();
	}

	#endregion

	#region ulong type conversion

	public static explicit operator MsgPackToken(ulong value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator ulong(MsgPackToken token)
	{
		return token.CastTokenToValue<ulong>();
	}

	#endregion

	#region uint type conversion

	public static explicit operator MsgPackToken(uint value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator uint(MsgPackToken token)
	{
		return token.CastTokenToValue<uint>();
	}

	#endregion

	#region ushort type conversion

	public static explicit operator MsgPackToken(ushort value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator ushort(MsgPackToken token)
	{
		return token.CastTokenToValue<ushort>();
	}

	#endregion

	#region byte type conversion

	public static explicit operator MsgPackToken(byte value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator byte(MsgPackToken token)
	{
		return token.CastTokenToValue<byte>();
	}

	#endregion

	#region long type conversion

	public static explicit operator MsgPackToken(long value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator long(MsgPackToken token)
	{
		return token.CastTokenToValue<long>();
	}

	#endregion

	#region int type conversion

	public static explicit operator MsgPackToken(int value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator int(MsgPackToken token)
	{
		return token.CastTokenToValue<int>();
	}

	#endregion

	#region short type conversion

	public static explicit operator MsgPackToken(short value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator short(MsgPackToken token)
	{
		return token.CastTokenToValue<short>();
	}

	#endregion

	#region sbyte type conversion

	public static explicit operator MsgPackToken(sbyte value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator sbyte(MsgPackToken token)
	{
		return token.CastTokenToValue<sbyte>();
	}

	#endregion

	#region float type conversion

	public static explicit operator MsgPackToken(float value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator float(MsgPackToken token)
	{
		return token.CastTokenToValue<float>();
	}

	#endregion

	#region double type conversion

	public static explicit operator MsgPackToken(double value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator double(MsgPackToken token)
	{
		return token.CastTokenToValue<double>();
	}

	#endregion

	#region DateTime type conversion

	public static explicit operator MsgPackToken(DateTime value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator DateTime(MsgPackToken token)
	{
		return token.CastTokenToValue<DateTime>();
	}

	#endregion

	#region DateTimeOffset type conversion

	public static explicit operator MsgPackToken(DateTimeOffset value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator DateTimeOffset(MsgPackToken token)
	{
		return token.CastTokenToValue<DateTimeOffset>();
	}

	#endregion

	#region DateTimeOffset type conversion

	public static explicit operator MsgPackToken(TimeSpan value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator TimeSpan(MsgPackToken token)
	{
		return token.CastTokenToValue<TimeSpan>();
	}

	#endregion

	#region Nullable<Bool> type conversion

	public static explicit operator MsgPackToken(bool? b)
	{
		return new MsgPackToken(b);
	}

	public static explicit operator bool?(MsgPackToken token)
	{
		return token.CastTokenToValue<bool?>();
	}

	#endregion

	#region Nullable<ulong> type conversion

	public static explicit operator MsgPackToken(ulong? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator ulong?(MsgPackToken token)
	{
		return token.CastTokenToValue<ulong?>();
	}

	#endregion

	#region Nullable<uint> type conversion

	public static explicit operator MsgPackToken(uint? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator uint?(MsgPackToken token)
	{
		return token.CastTokenToValue<uint?>();
	}

	#endregion

	#region Nullable<ushort> type conversion

	public static explicit operator MsgPackToken(ushort? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator ushort?(MsgPackToken token)
	{
		return token.CastTokenToValue<ushort?>();
	}

	#endregion

	#region Nullable<byte> type conversion

	public static explicit operator MsgPackToken(byte? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator byte?(MsgPackToken token)
	{
		return token.CastTokenToValue<byte?>();
	}

	#endregion

	#region Nullable<long> type conversion

	public static explicit operator MsgPackToken(long? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator long?(MsgPackToken token)
	{
		return token.CastTokenToValue<long?>();
	}

	#endregion

	#region Nullable<int> type conversion

	public static explicit operator MsgPackToken(int? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator int?(MsgPackToken token)
	{
		return token.CastTokenToValue<int?>();
	}

	#endregion

	#region Nullable<short> type conversion

	public static explicit operator MsgPackToken(short? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator short?(MsgPackToken token)
	{
		return token.CastTokenToValue<short?>();
	}

	#endregion

	#region Nullable<sbyte> type conversion

	public static explicit operator MsgPackToken(sbyte? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator sbyte?(MsgPackToken token)
	{
		return token.CastTokenToValue<sbyte?>();
	}

	#endregion

	#region Nullable<float> type conversion

	public static explicit operator MsgPackToken(float? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator float?(MsgPackToken token)
	{
		return token.CastTokenToValue<float?>();
	}

	#endregion

	#region Nullable<double> type conversion

	public static explicit operator MsgPackToken(double? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator double?(MsgPackToken token)
	{
		return token.CastTokenToValue<double?>();
	}

	#endregion

	#region Nullable<DateTime> type conversion

	public static explicit operator MsgPackToken(DateTime? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator DateTime?(MsgPackToken token)
	{
		return token.CastTokenToValue<DateTime?>();
	}

	#endregion

	#region Nullable<DateTimeOffset> type conversion

	public static explicit operator MsgPackToken(DateTimeOffset? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator DateTimeOffset?(MsgPackToken token)
	{
		return token.CastTokenToValue<DateTimeOffset?>();
	}

	#endregion

	#region Nullable<DateTimeOffset> type conversion

	public static explicit operator MsgPackToken(TimeSpan? value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator TimeSpan?(MsgPackToken token)
	{
		return token.CastTokenToValue<TimeSpan?>();
	}

	#endregion

	#region MsgPackToken[] type conversion

	public static explicit operator MsgPackToken(MsgPackToken[] value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator MsgPackToken[](MsgPackToken token)
	{
		return token.CastTokenToValue<MsgPackToken[]>();
	}

	#endregion

	#region Dictionary<MsgPackToken,MsgPackToken> type conversion

	public static explicit operator MsgPackToken(Dictionary<MsgPackToken, MsgPackToken> value)
	{
		return new MsgPackToken(value);
	}

	public static explicit operator Dictionary<MsgPackToken, MsgPackToken>(MsgPackToken token)
	{
		return token.CastTokenToValue<Dictionary<MsgPackToken, MsgPackToken>>();
	}

	#endregion
}