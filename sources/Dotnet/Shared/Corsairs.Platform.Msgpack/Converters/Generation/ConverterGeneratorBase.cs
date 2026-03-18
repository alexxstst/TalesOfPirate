using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using Corsairs.Platform.Msgpack.Converters.Generation.Exceptions;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

public abstract class ConverterGeneratorBase
{
	private readonly string _converterNameSuffix;
	private readonly ModuleBuilder _moduleBuilder;

	private readonly string _namespaceToPlace;

	private readonly PropertyProvider _propertyProvider = new();

	protected ConverterGeneratorBase(ModuleBuilder moduleBuilder, string namespaceToPlace, string converterNameSuffix)
	{
		_moduleBuilder = moduleBuilder;
		_namespaceToPlace = namespaceToPlace;
		_converterNameSuffix = converterNameSuffix;
	}

	public Type Generate(Type typeToWrap, Type typeToInstantinate)
	{
		var propsToWrap = GetDistinctProperties(typeToWrap).ToImmutableArray();

		var interfaces = typeToInstantinate == typeToWrap
			? new[] { typeof(IMsgPackConverter<>).MakeGenericType(typeToWrap) }
			: new[]
			{
				typeof(IMsgPackConverter<>).MakeGenericType(typeToWrap),
				typeof(IMsgPackConverter<>).MakeGenericType(typeToInstantinate)
			};

		var typeBuilder = _moduleBuilder.DefineType(
			$"{_namespaceToPlace}.{typeToWrap.GetNormalizedName()}_{typeToInstantinate.GetNormalizedName()}_{_converterNameSuffix}",
			TypeAttributes.Public,
			typeof(object),
			interfaces);

		var converters = propsToWrap
			.Select(x => x.PropertyType)
			.Distinct()
			.Union(new[] { typeof(string) })
			.Select<Type, ValueTuple<Type, FieldBuilder>>(
				x => (x, typeBuilder.DefineField(
					$"_{x.Name}_Converter",
					typeof(Lazy<>).MakeGenericType(typeof(IMsgPackConverter<>).MakeGenericType(x)),
					FieldAttributes.Private)))
			.ToImmutableDictionary(
				x => x.Item1,
				x => x.Item2
			);

		EmitInitializeMethod(typeBuilder, converters);
		var writeImpl = EmitWriteImplMethod(typeBuilder, typeToWrap, propsToWrap, converters);
		var readImpl = EmitReadImplMethod(typeBuilder, typeToInstantinate, propsToWrap, converters);

		foreach (var @interface in interfaces)
		{
			EmitWriteMethod(typeBuilder, @interface, writeImpl);
			EmitReadMethod(typeBuilder, @interface, readImpl);
		}

		return typeBuilder.ToType();
	}

	protected MethodInfo GetPropertySetter(Type typeToInstantinate, PropertyInfo property)
	{
		// if property has setter, we will call it. Otherwise we'll try to find it on implementation.
		if (property.SetMethod != null) return property.SetMethod;

		var implementationProperty = _propertyProvider.GetPropertyByName(typeToInstantinate, property.Name);

		return implementationProperty != null
			? implementationProperty.SetMethod
			: throw new GeneratorException(
				$"Can't find setter for property {property.PropertyType.Name} {property.Name}");
	}

	protected abstract IEnumerable<PropertyInfo> FilterProperties(Type typeToWrap,
																  ImmutableArray<PropertyInfo> properties);

	protected abstract MethodBuilder EmitReadImplMethod(
		TypeBuilder typeBuilder,
		Type typeToInstantinate,
		ImmutableArray<PropertyInfo> propsToWrap,
		ImmutableDictionary<Type, FieldBuilder> converters);

	protected abstract MethodBuilder EmitWriteImplMethod(
		TypeBuilder typeBuilder,
		Type typeToWrap,
		ImmutableArray<PropertyInfo> propsToWrap,
		ImmutableDictionary<Type, FieldBuilder> converters);

	private IEnumerable<PropertyInfo> GetDistinctProperties(Type typeToWrap)
	{
		var allProperties = _propertyProvider.GetProperties(typeToWrap);

		return FilterProperties(typeToWrap, allProperties);
	}

	private void EmitReadMethod(TypeBuilder typeBuilder, Type @interface, MethodBuilder readImpl)
	{
		var mb = typeBuilder.DefineMethod(
			$"{nameof(IMsgPackConverter<object>.Read)}",
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(@interface.GenericTypeArguments[0]);
		mb.SetParameters(typeof(IMsgPackReader));

		var generator = mb.GetILGenerator();

		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(OpCodes.Call, readImpl);
		generator.Emit(OpCodes.Ret);
	}

	private void EmitWriteMethod(TypeBuilder typeBuilder, Type @interface, MethodBuilder writeImpl)
	{
		var mb = typeBuilder.DefineMethod(
			$"{nameof(IMsgPackConverter<object>.Write)}",
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(typeof(void));
		mb.SetParameters(@interface.GenericTypeArguments[0], typeof(IMsgPackWriter));

		var generator = mb.GetILGenerator();

		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(OpCodes.Ldarg_2);
		generator.Emit(OpCodes.Call, writeImpl);
		generator.Emit(OpCodes.Ret);
	}

	[SuppressMessage("ReSharper", "AssignNullToNotNullAttribute")]
	private void EmitInitializeMethod(
		TypeBuilder typeBuilder,
		ImmutableDictionary<Type, FieldBuilder> converters)
	{
		var mb = typeBuilder.DefineMethod(
			nameof(IMsgPackConverter.Initialize),
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(typeof(void));
		mb.SetParameters(typeof(MsgPackContext));

		var generator = mb.GetILGenerator();

		foreach (var converter in converters)
		{
			generator.Emit(OpCodes.Ldarg_0);
			generator.Emit(OpCodes.Ldarg_1);
			generator.Emit(
				OpCodes.Ldftn,
				typeof(MsgPackContext).GetTypeInfo().GetMethod(nameof(MsgPackContext.GetConverter))
					.MakeGenericMethod(converter.Key));

			var converterType = converter.Value.FieldType.GenericTypeArguments[0];
			generator.Emit(
				OpCodes.Newobj,
				typeof(Func<>)
					.MakeGenericType(converterType)
					.GetTypeInfo()
					.GetConstructor(new[] { typeof(object), typeof(IntPtr) }));
			generator.Emit(
				OpCodes.Newobj,
				typeof(Lazy<>)
					.MakeGenericType(converterType)
					.GetTypeInfo()
					.GetConstructor(
						new[]
						{
							typeof(Func<>).MakeGenericType(converterType)
						}));
			generator.Emit(OpCodes.Stfld, converter.Value);
		}

		generator.Emit(OpCodes.Ret);
	}
}