// eff1.hlsl — effect VS (базовый, с массивом UV для углов quad).
//
// Было ASM vs.1.1. После рефакторинга — HLSL, компилируется runtime'ом через
// lwShaderMgr9::RegisterVertexShader(VS_FILE_HLSL) → D3DXCompileShader + vs_2_0.
//
// Регистры зафиксированы явно — C++ runtime выставляет их через
// SetVertexShaderConstantF по абсолютным индексам:
//   c0..c3 — World
//   c4..c7 — View * Projection
//   c8     — AlphaColor (diffuse)
//   c9..c12 — UVs[4] (углы quad, выбор через BLENDINDICES.x)

float4x4 World      : register(c0);
float4x4 ViewProj   : register(c4);
float4   AlphaColor : register(c8);
float4   UVs[4]     : register(c9);

struct VS_IN {
    float4 pos : POSITION;
    float  idx : BLENDINDICES;
};

struct VS_OUT {
    float4 pos     : POSITION;
    float4 diffuse : COLOR0;
    float2 uv      : TEXCOORD0;
};

VS_OUT main(VS_IN i) {
    VS_OUT o;
    float4 worldPos = mul(i.pos, World);
    o.pos     = mul(worldPos, ViewProj);
    o.diffuse = AlphaColor;
    o.uv      = UVs[(int)i.idx].xy;
    return o;
}
