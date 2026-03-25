```

BenchmarkDotNet v0.15.8, Windows 11 (10.0.26200.8039/25H2/2025Update/HudsonValley2)
AMD Ryzen 9 7945HX with Radeon Graphics 2.50GHz, 1 CPU, 32 logical and 16 physical cores
.NET SDK 10.0.201
  [Host]     : .NET 10.0.5 (10.0.5, 10.0.526.15411), X64 RyuJIT x86-64-v4
  DefaultJob : .NET 10.0.5 (10.0.5, 10.0.526.15411), X64 RyuJIT x86-64-v4


```
| Method                                  | N  | Mean             | Error           | StdDev          | Median           | Rank | Gen0   | Allocated |
|---------------------------------------- |--- |-----------------:|----------------:|----------------:|-----------------:|-----:|-------:|----------:|
| &#39;C# (baseline)&#39;                         | 10 |         118.3 ns |         3.08 ns |         8.99 ns |         119.0 ns |    1 |      - |         - |
| &#39;NLua (native Lua, high-level)&#39;         | 10 |       3,063.9 ns |        60.18 ns |        76.11 ns |       3,053.9 ns |    2 | 0.0076 |     168 B |
| &#39;KeraLua (native Lua, low-level C API)&#39; | 10 |       3,204.2 ns |        48.65 ns |        43.13 ns |       3,210.6 ns |    2 |      - |         - |
| &#39;LuaCSharp (pure C# Lua)&#39;               | 10 |      11,534.1 ns |       227.80 ns |       416.54 ns |      11,411.0 ns |    3 |      - |     168 B |
| &#39;C# (baseline)&#39;                         | 20 |      14,947.0 ns |       254.14 ns |       212.22 ns |      15,022.9 ns |    4 |      - |         - |
| &#39;KeraLua (native Lua, low-level C API)&#39; | 20 |     356,796.5 ns |     5,833.07 ns |     5,170.86 ns |     357,571.0 ns |    5 |      - |         - |
| &#39;NLua (native Lua, high-level)&#39;         | 20 |     404,587.9 ns |     8,019.71 ns |    14,255.02 ns |     398,629.8 ns |    6 |      - |     168 B |
| &#39;LuaCSharp (pure C# Lua)&#39;               | 20 |   1,457,482.9 ns |    29,128.97 ns |    65,151.12 ns |   1,438,715.3 ns |    7 |      - |     168 B |
| &#39;C# (baseline)&#39;                         | 30 |   1,812,405.5 ns |    35,616.12 ns |    80,391.52 ns |   1,782,853.1 ns |    8 |      - |         - |
| &#39;KeraLua (native Lua, low-level C API)&#39; | 30 |  39,802,674.9 ns |   768,551.68 ns |   854,243.15 ns |  39,339,969.2 ns |    9 |      - |         - |
| &#39;NLua (native Lua, high-level)&#39;         | 30 |  43,816,641.0 ns |   872,376.73 ns | 1,134,336.26 ns |  43,420,516.7 ns |   10 |      - |     168 B |
| &#39;LuaCSharp (pure C# Lua)&#39;               | 30 | 174,796,327.3 ns | 3,454,680.92 ns | 5,378,522.42 ns | 172,936,787.5 ns |   11 |      - |     168 B |
