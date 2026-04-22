// minimap.hlsl — VS для рендера minimap-иконок (quad'ы).
//
// Особенность: position не лежит в vertex stream'е, а в const-array по индексу
// из BLENDWEIGHT.x. Batch из 4 позиций (4 угла quad) хранится в c9..c12.
// Runtime вызовы из SMallMap.cpp / .h:
//   SetVertexShaderConstantF(9, pos0, 1)   // c9  = TL
//   SetVertexShaderConstantF(10, pos1, 1)  // c10 = TR
//   SetVertexShaderConstantF(11, pos2, 1)  // c11 = BR
//   SetVertexShaderConstantF(12, pos3, 1)  // c12 = BL
//
// Vertex stream содержит: BLENDWEIGHT (float1 — индекс 0..3) + TEXCOORD0 (UV).
// Position берётся из Positions[idx] в const-array.
//
// Constant registers:
//   c0..c3 — World (identity в SMallMap::RenderMinimap, реально используется
//                    в _Cha.Begin/Render где tv = matrix * vers[i])
//   c4..c7 — ViewProj
//   c8     — color (diffuse)
//   c9..c12 — Positions[4]

float4x4 World       : register(c0);
float4x4 ViewProj    : register(c4);
float4   Color       : register(c8);
float4   Positions[4] : register(c9);

struct VS_IN {
    float  posIndex : BLENDWEIGHT;
    float2 uv       : TEXCOORD0;
};

struct VS_OUT {
    float4 pos     : POSITION;
    float4 diffuse : COLOR0;
    float2 uv      : TEXCOORD0;
};

VS_OUT main(VS_IN i) {
    VS_OUT o;
    float4 localPos = Positions[(int)i.posIndex];
    float4 worldPos = mul(localPos, World);
    o.pos     = mul(worldPos, ViewProj);
    o.diffuse = Color;
    o.uv      = i.uv;
    return o;
}
