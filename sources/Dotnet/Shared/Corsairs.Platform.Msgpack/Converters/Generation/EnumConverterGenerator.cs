using System;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Reflection.Emit;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

internal class EnumConverterGenerator
{
	private readonly ModuleBuilder _moduleBuilder;
	private readonly string _namespaceToPlace;

	public EnumConverterGenerator(ModuleBuilder moduleBuilder, string namespaceToPlace)
	{
		_moduleBuilder = moduleBuilder;
		_namespaceToPlace = namespaceToPlace;
	}

	public Type Generate(Type typeToWrap)
	{
		var interfaceToImplement = typeof(IMsgPackConverter<>).MakeGenericType(typeToWrap);

		var typeBuilder = _moduleBuilder.DefineType(
			$"{_namespaceToPlace}.{typeToWrap.GetNormalizedName()}_EnumConverter",
			TypeAttributes.Public,
			typeof(object),
			new[] { interfaceToImplement });

		var underlyingType = Enum.GetUnderlyingType(typeToWrap);

		var underlyingTypeConverter = typeBuilder.DefineField(
			$"_{underlyingType.Name}_Converter",
			typeof(Lazy<>).MakeGenericType(typeof(IMsgPackConverter<>).MakeGenericType(underlyingType)),
			FieldAttributes.Private);

		EmitInitializeMethod(typeBuilder, underlyingType, underlyingTypeConverter);
		EmitWriteMethod(typeBuilder, typeToWrap, underlyingType, underlyingTypeConverter);
		EmitReadMethod(typeBuilder, typeToWrap, underlyingTypeConverter);

		return typeBuilder.ToType();
	}

	private static void EmitReadMethod(
		TypeBuilder typeBuilder,
		Type typeToWrap,
		FieldBuilder underlyingTypeConverter)
	{
		var mb = typeBuilder.DefineMethod(
			$"{nameof(IMsgPackConverter<object>.Read)}",
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(typeToWrap);
		mb.SetParameters(typeof(IMsgPackReader));

		var generator = mb.GetILGenerator();
		generator.DeclareLocal(typeToWrap);
		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldfld, underlyingTypeConverter);
		generator.Emit(OpCodes.Callvirt,
			underlyingTypeConverter.FieldType.GetTypeInfo().GetProperty(nameof(Lazy<object>.Value)).GetMethod);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(
			OpCodes.Callvirt,
			underlyingTypeConverter.FieldType.GenericTypeArguments[0].GetTypeInfo()
				.GetMethod(nameof(IMsgPackConverter<object>.Read), new[] { typeof(IMsgPackReader) }));

		generator.Emit(OpCodes.Stloc_0);
		generator.Emit(OpCodes.Ldloc_0);

		generator.Emit(OpCodes.Ret);
	}

	private static void EmitWriteMethod(
		TypeBuilder typeBuilder,
		Type type,
		Type underlyingType,
		FieldBuilder underlyingTypeConverter)
	{
		var mb = typeBuilder.DefineMethod(
			$"{nameof(IMsgPackConverter<object>.Write)}",
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(typeof(void));
		mb.SetParameters(type, typeof(IMsgPackWriter));

		var generator = mb.GetILGenerator();

		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldfld, underlyingTypeConverter);
		generator.Emit(OpCodes.Callvirt,
			underlyingTypeConverter.FieldType.GetTypeInfo().GetProperty(nameof(Lazy<object>.Value)).GetMethod);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(OpCodes.Ldarg_2);
		generator.Emit(
			OpCodes.Callvirt,
			underlyingTypeConverter.FieldType.GenericTypeArguments[0].GetTypeInfo().GetMethod(
				nameof(IMsgPackConverter<object>.Write), new[] { underlyingType, typeof(IMsgPackWriter) }));

		generator.Emit(OpCodes.Ret);
	}

	[SuppressMessage("ReSharper", "AssignNullToNotNullAttribute")]
	private static void EmitInitializeMethod(
		TypeBuilder typeBuilder,
		Type underlyingType,
		FieldBuilder underlyingTypeConverter)
	{
		var mb = typeBuilder.DefineMethod(
			nameof(IMsgPackConverter.Initialize),
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(typeof(void));
		mb.SetParameters(typeof(MsgPackContext));

		var generator = mb.GetILGenerator();

		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(
			OpCodes.Ldftn,
			typeof(MsgPackContext).GetTypeInfo().GetMethod(nameof(MsgPackContext.GetConverter))
				.MakeGenericMethod(underlyingType));

		var converterType = underlyingTypeConverter.FieldType.GenericTypeArguments[0];
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
		generator.Emit(OpCodes.Stfld, underlyingTypeConverter);

		generator.Emit(OpCodes.Ret);
	}
}