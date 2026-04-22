// eff4.hlsl — функционально идентичен eff1.hlsl/eff3.hlsl.
// Отдельный файл для регистрации под VSTU_EFFECT_E4.

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
