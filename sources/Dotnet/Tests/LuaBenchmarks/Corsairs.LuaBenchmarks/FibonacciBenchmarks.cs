using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Order;
using Lua;
using Lua.Standard;

namespace Corsairs.LuaBenchmarks;

/// <summary>
/// Бенчмарки вычисления Фибоначчи: NLua (native Lua), LuaCSharp (чистый C#), KeraLua (низкоуровневый C API), чистый C#.
/// </summary>
[MemoryDiagnoser]
[Orderer(SummaryOrderPolicy.FastestToSlowest)]
[RankColumn]
public class FibonacciBenchmarks
{
    private const string LuaFibScript = """
        function fib(n)
            if n <= 1 then return n end
            return fib(n - 1) + fib(n - 2)
        end
        """;

    [Params(10, 20, 30)]
    public int N;

    // === NLua ===
    private NLua.Lua _nlua = null!;
    private NLua.LuaFunction _nluaFib = null!;

    // === LuaCSharp ===
    private LuaState _luaCSharp = null!;
    private LuaValue _luaCSharpFib;

    // === KeraLua ===
    private KeraLua.Lua _keraLua = null!;

    [GlobalSetup]
    public void Setup()
    {
        // NLua — обёртка над native Lua 5.4
        _nlua = new NLua.Lua();
        _nlua.DoString(LuaFibScript);
        _nluaFib = _nlua["fib"] as NLua.LuaFunction
            ?? throw new InvalidOperationException("NLua: fib not found");

        // LuaCSharp — чистый C# Lua 5.2
        _luaCSharp = LuaState.Create();
        _luaCSharp.OpenStandardLibraries();
        _luaCSharp.DoStringAsync(LuaFibScript).AsTask().GetAwaiter().GetResult();
        _luaCSharpFib = _luaCSharp.Environment["fib"];

        // KeraLua — прямой C API binding
        _keraLua = new KeraLua.Lua();
        _keraLua.DoString(LuaFibScript);
    }

    [GlobalCleanup]
    public void Cleanup()
    {
        _nlua?.Dispose();
        _luaCSharp?.Dispose();
        _keraLua?.Dispose();
    }

    // --- Бенчмарки ---

    [Benchmark(Description = "C# (baseline)")]
    public long CSharpFib()
    {
        return Fib(N);
    }

    [Benchmark(Description = "NLua (native Lua, high-level)")]
    public object? NLuaFib()
    {
        var result = _nluaFib.Call(N);
        return result[0];
    }

    [Benchmark(Description = "KeraLua (native Lua, low-level C API)")]
    public long KeraLuaFib()
    {
        _keraLua.GetGlobal("fib");
        _keraLua.PushInteger(N);
        _keraLua.PCall(1, 1, 0);
        var result = _keraLua.ToInteger(-1);
        _keraLua.Pop(1);
        return result;
    }

    [Benchmark(Description = "LuaCSharp (pure C# Lua)")]
    public long LuaCSharpFib()
    {
        LuaValue[] args = [N];
        var results = _luaCSharp.CallAsync(_luaCSharpFib, args)
            .AsTask().GetAwaiter().GetResult();
        return results[0].Read<long>();
    }

    // --- C# Fibonacci ---

    private static long Fib(int n)
    {
        if (n <= 1) return n;
        return Fib(n - 1) + Fib(n - 2);
    }
}
