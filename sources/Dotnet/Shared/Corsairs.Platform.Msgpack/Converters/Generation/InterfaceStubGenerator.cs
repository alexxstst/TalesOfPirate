using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

public class InterfaceStubGenerator
{
	private readonly ModuleBuilder _moduleBuilder;

	private readonly string _namespaceToPlace;

	private readonly PropertyProvider _propertyProvider = new();

	public InterfaceStubGenerator(ModuleBuilder moduleBuilder, string namespaceToPlace)
	{
		_moduleBuilder = moduleBuilder;
		_namespaceToPlace = namespaceToPlace;
	}

	public Type GenerateTypeToInstantinate(Type typeToWrap, Func<string, string> nameMangler)
	{
		var typeInfo = typeToWrap.GetTypeInfo();
		var isInterface = typeInfo.IsInterface;
		if (!isInterface) throw new ArgumentException($"{typeToWrap.Name} should be interface", nameof(typeToWrap));

		if (typeInfo.IsGenericTypeDefinition) throw new NotImplementedException("Can't generate generic implementors.");

		var typeBuilder = _moduleBuilder.DefineType(
			$"{_namespaceToPlace}.{nameMangler(typeToWrap.GetNormalizedName())}",
			TypeAttributes.Public,
			typeof(object));

		typeBuilder.AddInterfaceImplementation(typeToWrap);

		var methodToSkip = new HashSet<MethodInfo>();
		var properties = _propertyProvider.GetProperties(typeToWrap);
		foreach (var info in properties)
		{
			GenerateProperty(info, typeBuilder);
			if (info.GetMethod != null)
				methodToSkip.Add(info.GetMethod);

			if (info.SetMethod != null)
				methodToSkip.Add(info.SetMethod);
		}

		foreach (var method in typeToWrap.GetMembersFromInterface(x => x.GetTypeInfo().DeclaredMethods)
					 .Where(x => !methodToSkip.Contains(x))) GenerateNotImplementedMethod(method, typeBuilder);

		return typeBuilder.ToType();
	}

	private static void GenerateNotImplementedMethod(MethodInfo method, TypeBuilder typeBuilder)
	{
		var mb = typeBuilder.DefineMethod(
			method.Name,
			MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig,
			CallingConventions.HasThis);
		mb.SetReturnType(method.ReturnType);
		mb.SetParameters(method.GetParameters().Select(x => x.ParameterType).ToArray());
		if (method.ContainsGenericParameters) throw new NotImplementedException("Can't generate generic methods");

		var generator = mb.GetILGenerator();

		// ReSharper disable once AssignNullToNotNullAttribute
		generator.Emit(OpCodes.Newobj,
			typeof(NotImplementedException).GetTypeInfo().DeclaredConstructors
				.First(x => x.GetParameters().Length == 0));
		generator.Emit(OpCodes.Throw);
		generator.Emit(OpCodes.Ret);
	}

	private static void GenerateProperty(PropertyInfo info, TypeBuilder typeBuilder)
	{
		var propertyType = info.PropertyType;

		var field = typeBuilder.DefineField(
			$"_{info.Name.Substring(0, 1).ToLowerInvariant()}{info.Name.Substring(1)}",
			propertyType,
			FieldAttributes.Private);

		var property = typeBuilder.DefineProperty(info.Name, PropertyAttributes.None, propertyType, new Type[0]);
		property.SetGetMethod(GenerateGetter(info, typeBuilder, field));
		property.SetSetMethod(GenerateSetter(info, typeBuilder, field));
	}

	private static MethodBuilder GenerateSetter(PropertyInfo info, TypeBuilder typeBuilder, FieldInfo field)
	{
		var setter = typeBuilder.DefineMethod(
			$"set_{info.Name}",
			MethodAttributes.HideBySig | MethodAttributes.SpecialName | MethodAttributes.Public |
			MethodAttributes.Virtual,
			CallingConventions.HasThis);

		setter.SetReturnType(typeof(void));
		setter.SetParameters(field.FieldType);
		var generator = setter.GetILGenerator();
		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldarg_1);
		generator.Emit(OpCodes.Stfld, field);
		generator.Emit(OpCodes.Ret);

		return setter;
	}

	private static MethodBuilder GenerateGetter(PropertyInfo info, TypeBuilder typeBuilder, FieldBuilder field)
	{
		var getter = typeBuilder.DefineMethod(
			$"get_{info.Name}",
			MethodAttributes.HideBySig | MethodAttributes.SpecialName | MethodAttributes.Public |
			MethodAttributes.Virtual,
			CallingConventions.HasThis);

		getter.SetReturnType(field.FieldType);
		var generator = getter.GetILGenerator();
		generator.Emit(OpCodes.Ldarg_0);
		generator.Emit(OpCodes.Ldfld, field);
		generator.Emit(OpCodes.Ret);

		return getter;
	}
}