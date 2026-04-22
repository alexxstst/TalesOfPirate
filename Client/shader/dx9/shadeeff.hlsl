// shadeeff.hlsl — VS для shade-map (блики/тени на grid'е).
//
// Особенность: позиции и UV не лежат в vertex stream'е, а batch'ем хранятся
// в const-array runtime (из MPShadeMap::RenderVS):
//   SetVertexShaderConstantF(0, t_mat,      4);  // c0..c3 — identity (НЕ используется)
//   SetVertexShaderConstantF(4, ViewProj,   4);  // c4..c7
//   SetVertexShaderConstantF(8, Color,      1);  // c8
//   SetVertexShaderConstantF(9+0, UV0,      1);  // c9
//   SetVertexShaderConstantF(9+1, Pos0,     1);  // c10
//   SetVertexShaderConstantF(9+2, UV1,      1);  // c11
//   SetVertexShaderConstantF(9+3, Pos1,     1);  // c12  … и т.д. до _iVerNum*2 регистров.
//
// Vertex stream: TEXCOORD1 (float2 indices):
//   indices.x — абсолютный индекс c-регистра с UV (9, 11, 13, ...)
//   indices.y — абсолютный индекс c-регистра с Pos (10, 12, 14, ...)
//
// Layout в runtime alternates UV/Pos начиная с c9. Поэтому в HLSL массив
// Data[] хранит как есть, а индекс пересчитывается: array_idx = absolute_reg - 9.
//
// Размер Data: _iVerNum = (_iGridCrossNum+1)^2. При default radius=0.8,
// TILESIZE=1.0 → _iGridCrossNum ≈ 2, _iVerNum ≈ 9. Реалистичный максимум
// _iGridCrossNum = 10 → _iVerNum = 121 → 242 регистра (UV+Pos). В vs_2_0
// максимум 256 float4 constants → от c9 доступно 247. Берём Data[240].

float4x4 ViewProj : register(c4);
float4   Color    : register(c8);
float4   Data[240] : register(c9);

struct VS_IN {
    float2 indices : TEXCOORD1;   // .x = UV reg-idx, .y = Pos reg-idx
};

struct VS_OUT {
    float4 pos     : POSITION;
    float4 diffuse : COLOR0;
    float2 uv      : TEXCOORD0;
};

VS_OUT main(VS_IN i) {
    VS_OUT o;
    int posIdx = (int)i.indices.y - 9;
    int uvIdx  = (int)i.indices.x - 9;
    float4 pos = Data[posIdx];
    o.pos      = mul(pos, ViewProj);
    o.diffuse  = Color;
    o.uv       = Data[uvIdx].xy;
    return o;
}
