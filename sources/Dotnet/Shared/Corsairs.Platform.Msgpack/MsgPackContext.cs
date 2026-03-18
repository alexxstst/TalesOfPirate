using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Reflection;
using Corsairs.Platform.Msgpack.Attributes;
using Corsairs.Platform.Msgpack.Converters;
using Corsairs.Platform.Msgpack.Converters.Generation;

namespace Corsairs.Platform.Msgpack;

public class MsgPackContext
{
	private static readonly IMsgPackConverter<object> SharedNullConverter = new NullConverter();
	private readonly bool _convertEnumsAsStrings;

	private readonly Dictionary<Type, IMsgPackConverter> _converters;

	private readonly ConverterGenerationContext _generatorContext = new();

	private readonly Dictionary<Type, Type> _genericConverters = new();

	private readonly Dictionary<Type, Func<object>> _objectActivators = new();

	public MsgPackContext(bool strictParseOfFloat = false, bool convertEnumsAsStrings = true,
						  bool binaryCompatibilityMode = false)
	{
		_convertEnumsAsStrings = convertEnumsAsStrings;
		var numberConverter = new NumberConverter(strictParseOfFloat);
		_converters = new Dictionary<Type, IMsgPackConverter>
		{
			{ typeof(MsgPackToken), new MsgPackTokenConverter() },
			{ typeof(bool), new BoolConverter() },
			{ typeof(string), new StringConverter() },
			{ typeof(byte[]), new BinaryConverter(binaryCompatibilityMode) },
			{ typeof(float), numberConverter },
			{ typeof(double), numberConverter },
			{ typeof(byte), numberConverter },
			{ typeof(sbyte), numberConverter },
			{ typeof(short), numberConverter },
			{ typeof(ushort), numberConverter },
			{ typeof(int), numberConverter },
			{ typeof(uint), numberConverter },
			{ typeof(long), numberConverter },
			{ typeof(ulong), numberConverter },
			{ typeof(DateTime), new DateTimeConverter() },
			{ typeof(DateTimeOffset), new DateTimeConverter() },
			{ typeof(TimeSpan), new TimeSpanConverter() },

			{ typeof(bool?), new NullableConverter<bool>() },
			{ typeof(float?), new NullableConverter<float>() },
			{ typeof(double?), new NullableConverter<double>() },
			{ typeof(byte?), new NullableConverter<byte>() },
			{ typeof(sbyte?), new NullableConverter<sbyte>() },
			{ typeof(short?), new NullableConverter<short>() },
			{ typeof(ushort?), new NullableConverter<ushort>() },
			{ typeof(int?), new NullableConverter<int>() },
			{ typeof(uint?), new NullableConverter<uint>() },
			{ typeof(long?), new NullableConverter<long>() },
			{ typeof(ulong?), new NullableConverter<ulong>() },
			{ typeof(DateTime?), new NullableConverter<DateTime>() },
			{ typeof(DateTimeOffset?), new NullableConverter<DateTimeOffset>() }
		};

		foreach (var converter in _converters) converter.Value.Initialize(this);
	}

	public IMsgPackConverter<object> NullConverter => SharedNullConverter;

#if !NETSTANDARD1_1
	public void DiscoverConverters()
	{
		foreach (var assembly in AppDomain.CurrentDomain.GetAssemblies()) DiscoverConverters(assembly);
	}
#endif

	public void DiscoverConverters<T>()
	{
		DiscoverConverters(typeof(T).GetTypeInfo().Assembly);
	}

	public void DiscoverConverters(Assembly assembly)
	{
		var generateMapConverter = GetType().GetTypeInfo().GetGenericMethod(nameof(GenerateAndRegisterMapConverter), 1);
		var generateArrayConverter =
			GetType().GetTypeInfo().GetGenericMethod(nameof(GenerateAndRegisterArrayConverter), 1);
		foreach (var type in assembly.ExportedTypes.Where(x =>
					 x.GetTypeInfo().GetCustomAttribute<MsgPackMapAttribute>() != null))
			generateMapConverter.MakeGenericMethod(type).Invoke(this, null);

		foreach (var type in assembly.ExportedTypes.Where(x =>
					 x.GetTypeInfo().GetCustomAttribute<MsgPackArrayAttribute>() != null))
			generateArrayConverter.MakeGenericMethod(type).Invoke(this, null);
	}

	public void GenerateAndRegisterMapConverter<T>()
	{
		var generator = _generatorContext.GenerateMapConverter(typeof(T));
		RegisterConverter((IMsgPackConverter<T>)generator);
	}

	public void GenerateAndRegisterMapConverter<TInterface, TImplementation>()
		where TImplementation : TInterface
	{
		var generator = _generatorContext.GenerateMapConverter(typeof(TInterface), typeof(TImplementation));
		RegisterConverter((IMsgPackConverter<TInterface>)generator);
		RegisterConverter((IMsgPackConverter<TImplementation>)generator);
	}

	public void GenerateAndRegisterArrayConverter<T>()
	{
		var generator = _generatorContext.GenerateArrayConverter(typeof(T));
		RegisterConverter((IMsgPackConverter<T>)generator);
	}

	public void GenerateAndRegisterArrayConverter<TInterface, TImplementation>()
		where TImplementation : TInterface
	{
		var generator = _generatorContext.GenerateArrayConverter(typeof(TInterface), typeof(TImplementation));
		RegisterConverter((IMsgPackConverter<TInterface>)generator);
		RegisterConverter((IMsgPackConverter<TImplementation>)generator);
	}

	public void GenerateAndRegisterEnumConverter<T>()
	{
		var generator = _generatorContext.GenerateEnumConverter<T>(typeof(T), _convertEnumsAsStrings);
		RegisterConverter((IMsgPackConverter<T>)generator);
	}

	public void RegisterConverter<T>(IMsgPackConverter<T> converter)
	{
		converter.Initialize(this);
		_converters[typeof(T)] = converter;
	}

	public void RegisterGenericConverter(Type type)
	{
		var converterType = GetGenericInterface(type, typeof(IMsgPackConverter<>));
		if (converterType == null)
			throw new ArgumentException(
				$"Error registering generic converter. Expected IMsgPackConverter<> implementation, but got {type}");

		var convertedType = converterType.GenericTypeArguments.Single().GetGenericTypeDefinition();
		_genericConverters.Add(convertedType, type);
	}

	public IMsgPackConverter<T> GetConverter<T>()
	{
		var type = typeof(T);
		var result = (IMsgPackConverter<T>)GetConverterFromCache<T>();
		if (result != null)
			return result;

		result = (IMsgPackConverter<T>)(
			TryGenerateEnumConverter<T>(type) ??
			TryGenerateConverterFromGenericConverter(type) ??
			TryGenerateArrayConverter(type) ??
			TryGenerateMapConverter(type) ??
			TryGenerateNullableConverter(type));

		if (result == null) throw ExceptionUtils.ConverterNotFound(type);

		return result;
	}

	private IMsgPackConverter TryGenerateEnumConverter<T>(Type type)
	{
		var enumTypeInfo = typeof(T).GetTypeInfo();
		if (!enumTypeInfo.IsEnum) return null;

		return _converters
			.GetOrAdd(type,
				x => CreateAndInializeConverter(() =>
					_generatorContext.GenerateEnumConverter<T>(type, _convertEnumsAsStrings)));
	}

	public Func<object> GetObjectActivator(Type type)
	{
		return _objectActivators.GetOrAdd(type, CompiledLambdaActivatorFactory.GetActivator);
	}

	public ImmutableDictionary<Type, IMsgPackConverter> DumpConvertersCache()
	{
		return _converters.ToImmutableDictionary(x => x.Key, x => x.Value);
	}

	private IMsgPackConverter CreateAndInializeConverter(Func<object> converterActivator)
	{
		var converter = (IMsgPackConverter)converterActivator();
		converter.Initialize(this);
		return converter;
	}

	private IMsgPackConverter TryGenerateConverterFromGenericConverter(Type type)
	{
		if (!type.GetTypeInfo().IsGenericType) return null;
		var genericType = type.GetGenericTypeDefinition();

		if (!_genericConverters.TryGetValue(genericType, out var genericConverterType)) return null;

		var converterType = genericConverterType.MakeGenericType(type.GenericTypeArguments);
		return _converters.GetOrAdd(type, x => CreateAndInializeConverter(GetObjectActivator(converterType)));
	}

	private IMsgPackConverter TryGenerateMapConverter(Type type)
	{
		var mapInterface = GetGenericInterface(type, typeof(IDictionary<,>));
		if (mapInterface != null)
			return _converters.GetOrAdd(type, x => CreateAndInializeConverter(GetObjectActivator(
				typeof(MapConverter<,,>).MakeGenericType(
					x,
					mapInterface.GenericTypeArguments[0],
					mapInterface.GenericTypeArguments[1]))));

		mapInterface = GetGenericInterface(type, typeof(IReadOnlyDictionary<,>));
		if (mapInterface != null)
			return _converters.GetOrAdd(type, x => CreateAndInializeConverter(GetObjectActivator(
				typeof(ReadOnlyMapConverter<,,>).MakeGenericType(
					x,
					mapInterface.GenericTypeArguments[0],
					mapInterface.GenericTypeArguments[1]))));

		return null;
	}

	private IMsgPackConverter TryGenerateNullableConverter(Type type)
	{
		var typeInfo = type.GetTypeInfo();
		if (!typeInfo.IsGenericType || typeInfo.GetGenericTypeDefinition() != typeof(Nullable<>)) return null;

		return _converters.GetOrAdd(type,
			x => CreateAndInializeConverter(
				GetObjectActivator(
					typeof(NullableConverter<>).MakeGenericType(x.GetTypeInfo().GenericTypeArguments[0]))));
	}

	private IMsgPackConverter TryGenerateArrayConverter(Type type)
	{
		var arrayInterface = GetGenericInterface(type, typeof(IList<>));
		if (arrayInterface != null)
			return _converters.GetOrAdd(type,
				x => CreateAndInializeConverter(GetObjectActivator(
					typeof(ArrayConverter<,>).MakeGenericType(x, arrayInterface.GenericTypeArguments[0]))));

		arrayInterface = GetGenericInterface(type, typeof(IReadOnlyList<>));
		return arrayInterface != null
			? _converters.GetOrAdd(type,
				x => CreateAndInializeConverter(GetObjectActivator(
					typeof(ReadOnlyListConverter<,>).MakeGenericType(x, arrayInterface.GenericTypeArguments[0]))))
			: null;
	}

	private IMsgPackConverter GetConverterFromCache<T>()
	{
		return _converters.TryGetValue(typeof(T), out var temp) ? temp : null;
	}

	private static TypeInfo GetGenericInterface(Type type, Type genericInterfaceType)
	{
		var info = type.GetTypeInfo();
		if (info.IsInterface && info.IsGenericType &&
			info.GetGenericTypeDefinition() == genericInterfaceType) return info;

		return info
			.ImplementedInterfaces
			.Select(x => x.GetTypeInfo())
			.FirstOrDefault(x => x.IsGenericType && x.GetGenericTypeDefinition() == genericInterfaceType);
	}
}