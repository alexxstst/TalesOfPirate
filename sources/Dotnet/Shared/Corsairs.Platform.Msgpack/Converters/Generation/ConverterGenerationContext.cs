using System;
using System.Collections.Concurrent;
using System.Reflection;
using System.Reflection.Emit;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

internal class ConverterGenerationContext
{
	private readonly ArrayConverterGenerator _arrayGenerator;
	private readonly AssemblyBuilder _asmBuilder;

	private readonly ConcurrentDictionary<(Type, Type), Type> _converterCaches = new();

	private readonly EnumConverterGenerator _enumConverterGenerator;

	private readonly ConcurrentDictionary<Type, Type> _implementationCaches = new();

	private readonly InterfaceStubGenerator _interfaceStubGenerator;

	private readonly MapConverterGenerator _mapGenerator;

	private readonly string _name;

	private readonly string _namespace = "ProGaudi.MsgPack.Light.Generated";

	public ConverterGenerationContext()
	{
		_name = Guid.NewGuid().ToString("N");
#if NET46 || NET45
            _asmBuilder =
					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               AssemblyBuilder.DefineDynamicAssembly(new AssemblyName(_name), AssemblyBuilderAccess.RunAndSave);
            var moduleBuilder = _asmBuilder.DefineDynamicModule("main", $"{_name}.dll");
#else
		_asmBuilder =
			AssemblyBuilder.DefineDynamicAssembly(new AssemblyName(_name), AssemblyBuilderAccess.RunAndCollect);
		var moduleBuilder = _asmBuilder.DefineDynamicModule("main");
#endif
		_mapGenerator = new MapConverterGenerator(moduleBuilder, _namespace);
		_arrayGenerator = new ArrayConverterGenerator(moduleBuilder, _namespace);
		_enumConverterGenerator = new EnumConverterGenerator(moduleBuilder, _namespace);

		_interfaceStubGenerator = new InterfaceStubGenerator(moduleBuilder, _namespace);
	}

	public IMsgPackConverter GenerateMapConverter(Type type)
	{
		return GenerateMapConverter(type, DetectImplementation(type));
	}

	public IMsgPackConverter GenerateMapConverter(Type @interface, Type implementation)
	{
		var generatorType =
			_converterCaches.GetOrAdd((@interface, implementation), x => _mapGenerator.Generate(x.Item1, x.Item2));
		return (IMsgPackConverter)generatorType.GetActivator()();
	}

	public IMsgPackConverter GenerateArrayConverter(Type type)
	{
		return GenerateArrayConverter(type, DetectImplementation(type));
	}

	public IMsgPackConverter GenerateArrayConverter(Type @interface, Type implementation)
	{
		var generatorType = _converterCaches.GetOrAdd((@interface, implementation),
			x => _arrayGenerator.Generate(x.Item1, x.Item2));
		return (IMsgPackConverter)generatorType.GetActivator()();
	}

	public IMsgPackConverter GenerateEnumConverter<T>(Type type, bool convertEnumsAsStrings)
	{
		if (convertEnumsAsStrings) return new EnumStringConverter<T>();

		var generatorType = _converterCaches.GetOrAdd((type, type), x => _enumConverterGenerator.Generate(x.Item1));
		return (IMsgPackConverter)generatorType.GetActivator()();
	}

	private Type DetectImplementation(Type @interface)
	{
		return _implementationCaches.GetOrAdd(@interface, GenerateInterfaceImplementation);
	}

	private Type GenerateInterfaceImplementation(Type type)
	{
		var typeInfo = type.GetTypeInfo();
		var isInterface = typeInfo.IsInterface;
		if (!isInterface)
		{
			if (typeInfo.IsAbstract) throw new NotImplementedException("Can't generate child type for abstract class");

			if (typeInfo.GetDefaultConstructor() == null)
				throw new NotImplementedException("Can't generate child type for type without default ctor");
		}

		if (typeInfo.IsGenericTypeDefinition) throw new NotImplementedException("Can't generate generic implementors.");

		return typeInfo.IsInterface
			? _interfaceStubGenerator.GenerateTypeToInstantinate(type, x => $"{x}Implementation")
			: type;
	}

	public void Dump()
	{
#if NET46 || NET45
            _asmBuilder.Save($"{_name}.dll");
#else
		throw new NotSupportedException();
#endif
	}
}