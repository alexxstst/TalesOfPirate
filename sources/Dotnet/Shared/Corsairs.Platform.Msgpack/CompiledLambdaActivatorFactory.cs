using System;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;

namespace Corsairs.Platform.Msgpack;

public static class CompiledLambdaActivatorFactory
{
	public static Func<object> GetActivator(this Type type)
	{
		var ctor = type.GetTypeInfo().DeclaredConstructors.First(x => x.GetParameters().Length == 0 && !x.IsStatic);

		//make a NewExpression that calls the
		//ctor with the args we just created
		var newExp = Expression.New(ctor);

		//create a lambda with the New
		//Expression as body and our param object[] as arg
		var lambda = Expression.Lambda(typeof(Func<object>), newExp);

		//compile it
		return (Func<object>)lambda.Compile();
	}
}