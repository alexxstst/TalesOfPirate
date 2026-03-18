using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Reflection;

namespace Corsairs.Platform.Msgpack.Converters.Generation;

public class PropertyProvider
{
	private readonly ConcurrentDictionary<Type, ImmutableDictionary<string, PropertyInfo>> _cacheByName = new();
	private readonly ConcurrentDictionary<Type, ImmutableArray<PropertyInfo>> _cacheByOrder = new();

	public ImmutableArray<PropertyInfo> GetProperties(Type type)
	{
		return GetPropertiesFromCache(type, _cacheByOrder);
	}

	public PropertyInfo GetPropertyByName(Type type, string name)
	{
		var cache = GetPropertiesFromCache(type, _cacheByName);
		return cache.TryGetValue(name, out var r) ? r : null;
	}

	private T GetPropertiesFromCache<T>(Type type, ConcurrentDictionary<Type, T> cache)
	{
		if (cache.TryGetValue(type, out var r))
			return r;

		Discover(type);

		return cache[type];
	}

	private void Discover(Type type)
	{
		var cacheByName = new Dictionary<string, PropertyInfo>();
		var cacheByOrder = new List<PropertyInfo>();
		foreach (var info in DiscoverProperties(type))
		{
			if (cacheByName.ContainsKey(info.Name)) continue;
			cacheByName[info.Name] = info;
			cacheByOrder.Add(info);
		}

		_cacheByName[type] = cacheByName.ToImmutableDictionary(x => x.Key, x => x.Value);
		_cacheByOrder[type] = cacheByOrder.ToImmutableArray();
	}

	private IEnumerable<PropertyInfo> DiscoverProperties(Type type)
	{
		if (type == null)
			yield break;

		var typeInfo = type.GetTypeInfo();
		foreach (var property in typeInfo.DeclaredProperties) yield return property;

		if (typeInfo.IsInterface)
			foreach (var property in typeInfo.ImplementedInterfaces.SelectMany(DiscoverProperties))
				yield return property;
		else
			foreach (var property in DiscoverProperties(typeInfo.BaseType))
				yield return property;
	}
}