// eff1.hlsl — базовый effect VS.
//
// Pipeline: position * World * ViewProj → oPos.
//           Diffuse = AlphaColor (const). UV выбирается из массива UVs
//           по индексу BLENDINDICES (один quad использует 4 угловых UV).
//
// Регистры зафиксированы через :register(...), чтобы ASM-вывод совпадал с
// layout, который выставляет runtime через SetVertexShaderConstantF
// (c0..c3 World, c4..c7 ViewProj, c8 Alpha, c9..c12 UVs[4]).
//
// Компиляция: fxc /T vs_1_1 /E main /Gec /Fc ../dx9/eff1.vsh eff1.hlsl

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
