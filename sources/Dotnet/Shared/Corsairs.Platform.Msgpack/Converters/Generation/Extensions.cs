using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using Corsairs.Platform.Msgpack.Attributes;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

public static class Extensions
{
	public static Type ToType(this TypeBuilder typeBuilder)
	{
#if NET46
            return typeBuilder.CreateType();
#else
		return typeBuilder.CreateTypeInfo().AsType();
#endif
	}

	public static string GetNormalizedName(this Type type)
	{
		return type.GetTypeInfo().IsInterface && type.Name.StartsWith("I") ? type.Name.Substring(1) : type.Name;
	}

	public static IEnumerable<T> GetMembersFromInterface<T>(this Type typeToWrap, Func<Type, IEnumerable<T>> getter)
	{
		foreach (var member in getter(typeToWrap)) yield return member;

		foreach (var @interface in typeToWrap.GetTypeInfo().ImplementedInterfaces)
		foreach (var member in GetMembersFromInterface(@interface, getter))
			yield return member;
	}

	public static ConstructorInfo GetDefaultConstructor(this TypeInfo type)
	{
		return type.GetConstructor(new Type[0]);
	}

	public static string GetMapElementName(this PropertyInfo info)
	{
		return info.GetCustomAttribute<MsgPackMapElementAttribute>().Name;
	}

	public static int GetArrayElementOrder(this PropertyInfo info)
	{
		return info.GetCustomAttribute<MsgPackArrayElementAttribute>().Order;
	}

	public static MethodInfo GetGenericMethod(this TypeInfo type, string name, byte number)
	{
		return type.DeclaredMethods.SingleOrDefault(x => x.Name == name && x.GetGenericArguments().Length == number);
	}

#if NETSTANDARD1_1
        public static ConstructorInfo GetConstructor(this TypeInfo type, Type[] parameters)
        {
            foreach (var constructor in type.DeclaredConstructors)
            {
                var declaredParameters = constructor.GetParameters();
                if (ParameterMatch(declaredParameters, parameters))
                    return constructor;
            }

            return null;
        }

        public static PropertyInfo GetProperty(this TypeInfo type, string name)
        {
            return type.DeclaredProperties.SingleOrDefault(x => x.Name == name);
        }

        public static MethodInfo GetMethod(this TypeInfo type, string name)
        {
            return type.DeclaredMethods.SingleOrDefault(x => x.Name == name);
        }

        public static MethodInfo GetMethod(this TypeInfo type, string name, Type[] parameters)
        {
            foreach (var method in type.DeclaredMethods.Where(x => x.Name == name))
            {
                var declaredParameters = method.GetParameters();
                if (ParameterMatch(declaredParameters, parameters))
                    return method;
            }

            return null;
        }

        private static bool ParameterMatch(IReadOnlyList<ParameterInfo> parameters, IReadOnlyList<Type> types)
        {
            if (parameters.Count != types.Count)
            {
                return false;
            }

            for (var i = 0; i < parameters.Count; i++)
            {
                if (parameters[i].ParameterType != types[i])
                {
                    return false;
                }
            }

            return true;
        }
#endif
}