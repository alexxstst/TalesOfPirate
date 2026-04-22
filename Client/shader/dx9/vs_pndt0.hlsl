// vs_pndt0.hlsl — мастер-исходник семейства static-mesh VS.
//
// Собирается в 7 permutations через D3DXMACRO'ы (см. ShaderLoad.cpp). Ранее
// existed как 7 отдельных .vsh, все были fxc-output'ом этого одного файла
// (комментарии fxc в заголовках .vsh → "../client/shader/dx9_hlsl/vs_pndt0.hlsl").
//
//   define             | effect
//   ------------------ | -------
//   NO_LIGHTING        | без ambient+diffuse light, diffuse = color input или 0
//   NO_DIFFUSE         | цвет не берётся из вертекса (vertex без COLOR0)
//   USE_TEX_TRANSFORM  | UV транспонируется через float4x3 UVMatrix0 (c7..c8)
//
// 7 permutations × файл → VST_* тип:
//   vs_pndt0.vsh             : NO_LIGHTING                          → VST_PNDT0
//   vs_pndt0_ld.vsh          : (default)                            → VST_PNDT0_LD
//   vs_pndt0_t0uvmat.vsh     : NO_LIGHTING + USE_TEX_TRANSFORM      → VST_PNDT0_TT0
//   vs_pndt0_ld_t0uvmat.vsh  : USE_TEX_TRANSFORM                    → VST_PNDT0_LD_TT0
//   vs_pnt0_ld.vsh           : NO_DIFFUSE                           → VST_PNT0_LD
//   vs_pnt0_t0uvmat.vsh      : NO_LIGHTING + USE_TEX_TRANSFORM      → VST_PNT0_TT0
//                                         + NO_DIFFUSE
//   vs_pnt0_ld_t0uvmat.vsh   : USE_TEX_TRANSFORM + NO_DIFFUSE       → VST_PNT0_LD_TT0
//
// Constant-register layout (fixed, runtime выставляет через SetVertexShaderConstantF):
//   c0..c3 — World * ViewProj (combined) — соответствует dp4 oPos, v0, c0..c3
//   c4     — LightDir (.xyz)  [only if !NO_LIGHTING]
//   c5     — Ambient (.rgba)  [only if !NO_LIGHTING]
//   c7..c8 — UVMatrix0 (float4x3, row-major) [only if USE_TEX_TRANSFORM]

float4x4 World : register(c0);

#ifndef NO_LIGHTING
float3 LightDir : register(c4);
float4 Ambient  : register(c5);
#endif

#ifdef USE_TEX_TRANSFORM
// float4x3 кладётся в 2 float4 регистра (строки, по .xyz компоненты).
float4 UVMatrix0_Row0 : register(c7);
float4 UVMatrix0_Row1 : register(c8);
#endif

struct VS_IN {
    float4 pos      : POSITION;
#ifndef NO_LIGHTING
    float3 normal   : NORMAL;
#endif
#ifndef NO_DIFFUSE
    float4 color    : COLOR0;
#endif
    float2 uv       : TEXCOORD0;
};

struct VS_OUT {
    float4 pos      : POSITION;
    float4 diffuse  : COLOR0;
    float4 specular : COLOR1;
    float2 uv       : TEXCOORD0;
};

VS_OUT main(VS_IN i) {
    VS_OUT o;
    o.pos = mul(i.pos, World);

#ifdef NO_LIGHTING
    #ifndef NO_DIFFUSE
    o.diffuse = i.color;
    #else
    // Оригинальный ASM для pnt0_t0uvmat (NO_LIGHTING+NO_DIFFUSE): oD0 = c4.x = 1.0
    o.diffuse = float4(1, 0, 0, 0);
    #endif
#else
    // Lambertian diffuse: lit(NdotL, 0, 0).y = max(0, NdotL).
    float  NdotL   = dot(i.normal, LightDir);
    float4 lit_out = lit(NdotL, 0, 0);
    #ifndef NO_DIFFUSE
    float4 light = lit_out.y * i.color + Ambient;
    #else
    float4 light = lit_out.y + Ambient;
    #endif
    o.diffuse = saturate(light);
#endif

    o.specular = 0;

#ifdef USE_TEX_TRANSFORM
    // UV → homogeneous (u, v, 1) * UVMatrix0 (float4x3 в 2 строки).
    float3 uv3 = float3(i.uv.xy, 1);
    o.uv.x = dot(uv3, UVMatrix0_Row0.xyz);
    o.uv.y = dot(uv3, UVMatrix0_Row1.xyz);
#else
    o.uv = i.uv;
#endif

    return o;
}
