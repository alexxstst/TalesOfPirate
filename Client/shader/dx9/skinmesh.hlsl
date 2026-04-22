// skinmesh.hlsl — мастер-исходник animated skinning VS.
//
// Permutations (defines):
//   NUM_SKIN_WEIGHTS = 1 | 2    — 1-bone (w1=1) или 2-bone blend.
//                                 Для 2 weights: w2 = 1 - v1.x - v1.y
//                                 (canonical 3rd-weight implicit form старого
//                                 movka; v1.y обычно 0 → w2 = 1 - v1.x).
//   TT_MODE = 1 | 2 | 3         — texture-transform variant:
//     TT_MODE=1 → oT0 = v.uv, oT1 = Texture * uvh   (Texture : c12..c15)
//     TT_MODE=2 → oT0 = v.uv, oT1 = v.uv,
//                 oT2 = Texture2 * uvh              (Texture2 : c16..c19)
//     TT_MODE=3 → оба выше + oT3 = v.uv
//   где uvh = float4(v.uv.x, v.uv.y, 1, v.uv.y)
//
// Constant registers (фиксированы под runtime SetVertexShaderConstantF):
//   c0   — Base (.x = 1.0, .w = bone stride = 3)
//   c1..c4 — World * ViewProj (combined)
//   c5   — LightDirection
//   c6   — Ambient
//   c7   — Diffuse
//   c12..c15 — Texture (TT_MODE=1)
//   c16..c19 — Texture2 (TT_MODE=2/3)
//   c21..c95 — BV[75] (matrix palette, 25 bones × 3 рядов float4 row-major)
//
// 6 permutations → .vsh → VSTU_* (ShaderLoad.cpp::LoadShader1):
//   NUM_SKIN_WEIGHTS=1, TT_MODE=1 → skinmesh8_1_tt1 → VSTU_SKINMESH0_TT1
//   NUM_SKIN_WEIGHTS=1, TT_MODE=2 → skinmesh8_1_tt2 → VSTU_SKINMESH0_TT2
//   NUM_SKIN_WEIGHTS=1, TT_MODE=3 → skinmesh8_1_tt3 → VSTU_SKINMESH0_TT3
//   NUM_SKIN_WEIGHTS=2, TT_MODE=1 → skinmesh8_2_tt1 → VSTU_SKINMESH1_TT1
//   NUM_SKIN_WEIGHTS=2, TT_MODE=2 → skinmesh8_2_tt2 → VSTU_SKINMESH1_TT2
//   NUM_SKIN_WEIGHTS=2, TT_MODE=3 → skinmesh8_2_tt3 → VSTU_SKINMESH1_TT3

#ifndef NUM_SKIN_WEIGHTS
#define NUM_SKIN_WEIGHTS 1
#endif

#ifndef TT_MODE
#define TT_MODE 1
#endif

float4   Base           : register(c0);
float4x4 World          : register(c1);
float3   LightDirection : register(c5);
float4   Ambient        : register(c6);
float4   Diffuse        : register(c7);

#if TT_MODE == 1
float4 Tex_Row0 : register(c12);
float4 Tex_Row1 : register(c13);
float4 Tex_Row2 : register(c14);
float4 Tex_Row3 : register(c15);
#else
float4 Tex_Row0 : register(c16);
float4 Tex_Row1 : register(c17);
float4 Tex_Row2 : register(c18);
float4 Tex_Row3 : register(c19);
#endif

// Matrix palette: 25 bones × 3 float4 rows. 75 регистров c21..c95.
float4 BV[75] : register(c21);

struct VS_IN {
    float4 pos          : POSITION;
#if NUM_SKIN_WEIGHTS == 2
    float2 weight       : BLENDWEIGHT;
#endif
    float4 boneIndices  : BLENDINDICES;
    float3 normal       : NORMAL;
    float2 uv           : TEXCOORD0;
};

struct VS_OUT {
    float4 pos      : POSITION;
    float4 diffuse  : COLOR0;
    float4 specular : COLOR1;
    float2 t0       : TEXCOORD0;
#if TT_MODE == 1
    float4 t1       : TEXCOORD1;   // = Texture * uvh
#else
    float2 t1       : TEXCOORD1;   // = uv (passthrough)
    float4 t2       : TEXCOORD2;   // = Texture2 * uvh
#endif
#if TT_MODE == 3
    float2 t3       : TEXCOORD3;   // = uv
#endif
};

// BV[idx..idx+2] — 3 row'а bone-матрицы.
// Умножаем position (float4) на 3 row'а.
float3 SkinPos(float4 pos, int idx) {
    return float3(dot(pos, BV[idx + 0]),
                  dot(pos, BV[idx + 1]),
                  dot(pos, BV[idx + 2]));
}
float3 SkinNormal(float3 n, int idx) {
    return float3(dot(n, BV[idx + 0].xyz),
                  dot(n, BV[idx + 1].xyz),
                  dot(n, BV[idx + 2].xyz));
}

VS_OUT main(VS_IN i) {
    VS_OUT o;

    // bone byte → индекс в BV: idx = bone_byte * stride (stride = c0.w = 3).
    int idx1 = (int)(i.boneIndices.x * Base.w);

#if NUM_SKIN_WEIGHTS == 1
    float3 skinned  = SkinPos(i.pos, idx1);
    float3 skinnedN = SkinNormal(i.normal, idx1);
#else
    int idx2 = (int)(i.boneIndices.y * Base.w);
    // Canonical 2-bone blend с implicit 3rd weight:
    //   w1 = i.weight.x
    //   w2 = 1 - i.weight.x - i.weight.y   (реплицирует ASM логику)
    float  w1 = i.weight.x;
    float  w2 = Base.x - i.weight.x - i.weight.y;
    float3 skinned  = w1 * SkinPos(i.pos, idx1) + w2 * SkinPos(i.pos, idx2);
    float3 skinnedN = w1 * SkinNormal(i.normal, idx1) + w2 * SkinNormal(i.normal, idx2);
#endif
    // World (combined World*ViewProj) пишется runtime'ом в c1..c4 как 4 строки
    // (row-major). HLSL column-major + mul(vector, matrix) интерпретирует это
    // корректно: mul(v, W_hlsl_colmajor) == W_runtime * v.
    // НЕЛЬЗЯ использовать World[0]/[1]/[2]/[3] — в column-major это gather
    // колонок (c1.x, c2.x, c3.x, c4.x) — перепутанная матрица.
    float4 skinnedH = float4(skinned, 1.0);
    o.pos = mul(skinnedH, World);

    // Lighting.
    float3 nrm   = normalize(skinnedN);
    float  NdotL = dot(nrm, LightDirection);
    float4 L     = lit(NdotL, 0, 0);
    o.diffuse    = min(L.y * Diffuse + Ambient, Base.x);
    o.specular   = Base.z;

    // UV streams.
    o.t0 = i.uv;
    float4 uvh = float4(i.uv.x, i.uv.y, Base.x, i.uv.y);
#if TT_MODE == 1
    o.t1.x = dot(uvh, Tex_Row0);
    o.t1.y = dot(uvh, Tex_Row1);
    o.t1.z = dot(uvh, Tex_Row2);
    o.t1.w = dot(uvh, Tex_Row3);
#else
    o.t1 = i.uv;
    o.t2.x = dot(uvh, Tex_Row0);
    o.t2.y = dot(uvh, Tex_Row1);
    o.t2.z = dot(uvh, Tex_Row2);
    o.t2.w = dot(uvh, Tex_Row3);
#endif
#if TT_MODE == 3
    o.t3 = i.uv;
#endif

    return o;
}
