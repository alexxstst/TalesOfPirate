// eff2.hlsl — effect VS (упрощённый: UV берутся напрямую из вертекса, без массива).
//
// Было ASM vs.1.0. Регистры совпадают с eff1.hlsl (c0..c3 World, c4..c7 ViewProj,
// c8 AlphaColor), но вход — position + texcoord0 (без blendindices).

float4x4 World      : register(c0);
float4x4 ViewProj   : register(c4);
float4   AlphaColor : register(c8);

struct VS_IN {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
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
    o.uv      = i.uv;
    return o;
}
