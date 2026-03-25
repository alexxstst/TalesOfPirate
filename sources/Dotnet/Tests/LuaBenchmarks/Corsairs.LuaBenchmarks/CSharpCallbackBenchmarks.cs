using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Order;
using Lua;
using Lua.Standard;

namespace Corsairs.LuaBenchmarks;

/// <summary>
/// Бенчмарки вызова C#-метода из Lua: замер overhead'а при пересечении границы Lua → C#.
/// Lua-скрипт вызывает зарегистрированную C#-функцию fib(n), которая считает Фибоначчи на стороне C#.
/// </summary>
[MemoryDiagnoser]
[Orderer(SummaryOrderPolicy.FastestToSlowest)]
[RankColumn]
public class CSharpCallbackBenchmarks
{
    // Lua вызывает C#-функцию fib(n) — один вызов, вся работа в C#
    private const string LuaCallCSharpScript = "return csharp_fib(N_VALUE)";

    // Lua вызывает C#-функцию add(a,b) в цикле — много вызовов, замер per-call overhead
    private const string LuaLoopCallScript = """
        local sum = 0
        for i = 1, 10000 do
            sum = sum + csharp_add(i, 1)
        end
        return sum
        """;

    [Params(10, 20, 30)]
    public int N;

    // === NLua ===
    private NLua.Lua _nlua = null!;
    private NLua.LuaFunction _nluaSingleCall = null!;
    private NLua.LuaFunction _nluaLoopCall = null!;

    // === LuaCSharp ===
    private LuaState _luaCSharp = null!;
    private LuaValue _luaCSharpSingleCall;
    private LuaValue _luaCSharpLoopCall;

    // === KeraLua ===
    private KeraLua.Lua _keraLua = null!;

    private static long Fib(int n)
    {
        if (n <= 1) return n;
        return Fib(n - 1) + Fib(n - 2);
    }

    [GlobalSetup]
    public void Setup()
    {
        SetupNLua();
        SetupKeraLua();
        SetupLuaCSharp();
    }

    private void SetupNLua()
    {
        _nlua = new NLua.Lua();

        // Регистрируем C#-функции в Lua через делегаты
        _nlua["csharp_fib"] = (Func<int, long>)(n => Fib(n));
        _nlua["csharp_add"] = (Func<long, long, long>)((a, b) => a + b);

        // Компилируем Lua-функции с подставленным N
        _nlua.DoString($"function single_call() {LuaCallCSharpScript.Replace("N_VALUE", N.ToString())} end");
        _nlua.DoString($"function loop_call() {LuaLoopCallScript} end");

        _nluaSingleCall = _nlua["single_call"] as NLua.LuaFunction
            ?? throw new InvalidOperationException("NLua: single_call not found");
        _nluaLoopCall = _nlua["loop_call"] as NLua.LuaFunction
            ?? throw new InvalidOperationException("NLua: loop_call not found");
    }

    private void SetupKeraLua()
    {
        _keraLua = new KeraLua.Lua();

        // Регистрируем C#-функцию fib через C API
        _keraLua.PushCFunction(KeraLuaFibCallback);
        _keraLua.SetGlobal("csharp_fib");

        _keraLua.PushCFunction(KeraLuaAddCallback);
        _keraLua.SetGlobal("csharp_add");

        // Компилируем Lua-функции
        _keraLua.DoString($"function single_call() {LuaCallCSharpScript.Replace("N_VALUE", N.ToString())} end");
        _keraLua.DoString($"function loop_call() {LuaLoopCallScript} end");
    }

    // KeraLua callback — вызывается из Lua через C API
    private static int KeraLuaFibCallback(nint state)
    {
        var lua = KeraLua.Lua.FromIntPtr(state);
        var n = (int)lua.ToInteger(1);
        lua.PushInteger(Fib(n));
        return 1; // количество возвращаемых значений
    }

    private static int KeraLuaAddCallback(nint state)
    {
        var lua = KeraLua.Lua.FromIntPtr(state);
        var a = lua.ToInteger(1);
        var b = lua.ToInteger(2);
        lua.PushInteger(a + b);
        return 1;
    }

    private void SetupLuaCSharp()
    {
        _luaCSharp = LuaState.Create();
        _luaCSharp.OpenStandardLibraries();

        // Регистрируем C#-функции
        _luaCSharp.Environment["csharp_fib"] = new LuaFunction((context, ct) =>
        {
            var n = context.GetArgument<int>(0);
            context.Return(Fib(n));
            return new ValueTask<int>(1);
        });

        _luaCSharp.Environment["csharp_add"] = new LuaFunction((context, ct) =>
        {
            var a = context.GetArgument<long>(0);
            var b = context.GetArgument<long>(1);
            context.Return(a + b);
            return new ValueTask<int>(1);
        });

        // Компилируем Lua-функции
        _luaCSharp.DoStringAsync($"function single_call() {LuaCallCSharpScript.Replace("N_VALUE", N.ToString())} end")
            .AsTask().GetAwaiter().GetResult();
        _luaCSharp.DoStringAsync($"function loop_call() {LuaLoopCallScript} end")
            .AsTask().GetAwaiter().GetResult();

        _luaCSharpSingleCall = _luaCSharp.Environment["single_call"];
        _luaCSharpLoopCall = _luaCSharp.Environment["loop_call"];
    }

    [GlobalCleanup]
    public void Cleanup()
    {
        _nlua?.Dispose();
        _luaCSharp?.Dispose();
        _keraLua?.Dispose();
    }

    // ====== Бенчмарки: один вызов C# fib(N) из Lua ======

    [Benchmark(Description = "C# fib(N) — baseline")]
    public long CSharpDirect()
    {
        return Fib(N);
    }

    [Benchmark(Description = "Lua→C# fib(N) via NLua")]
    public object? NLuaSingleCall()
    {
        return _nluaSingleCall.Call()[0];
    }

    [Benchmark(Description = "Lua→C# fib(N) via KeraLua")]
    public long KeraLuaSingleCall()
    {
        _keraLua.GetGlobal("single_call");
        _keraLua.PCall(0, 1, 0);
        var result = _keraLua.ToInteger(-1);
        _keraLua.Pop(1);
        return result;
    }

    [Benchmark(Description = "Lua→C# fib(N) via LuaCSharp")]
    public long LuaCSharpSingleCall()
    {
        var results = _luaCSharp.CallAsync(_luaCSharpSingleCall, [])
            .AsTask().GetAwaiter().GetResult();
        return results[0].Read<long>();
    }

    // ====== Бенчмарки: 10000 вызовов C# add(i,1) из Lua ======

    [Benchmark(Description = "Lua→C# add() x10000 via NLua")]
    public object? NLuaLoopCall()
    {
        return _nluaLoopCall.Call()[0];
    }

    [Benchmark(Description = "Lua→C# add() x10000 via KeraLua")]
    public long KeraLuaLoopCall()
    {
        _keraLua.GetGlobal("loop_call");
        _keraLua.PCall(0, 1, 0);
        var result = _keraLua.ToInteger(-1);
        _keraLua.Pop(1);
        return result;
    }

    [Benchmark(Description = "Lua→C# add() x10000 via LuaCSharp")]
    public long LuaCSharpLoopCall()
    {
        var results = _luaCSharp.CallAsync(_luaCSharpLoopCall, [])
            .AsTask().GetAwaiter().GetResult();
        return results[0].Read<long>();
    }
}
