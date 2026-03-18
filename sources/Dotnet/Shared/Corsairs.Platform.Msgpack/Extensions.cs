using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

namespace Corsairs.Platform.Msgpack;

internal static class Extensions
{
	public static TValue GetOrAdd<TKey, TValue>([NotNull] this Dictionary<TKey, TValue> dictionary, [NotNull] TKey key,
												[NotNull] Func<TKey, TValue> creator)
	{
		TValue temp;
		if (!dictionary.TryGetValue(key, out temp))
			dictionary[key] = temp = creator(key);
		return temp;
	}
}