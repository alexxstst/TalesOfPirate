//
#include "stdafx.h"

#include "ShaderLoad.h"
#include "lwgraphicsutil.h"


#define USER_SHADER_NUM             8

LW_RESULT LoadShader0(lwISysGraphics* sys_graphics) {
	LW_RESULT ret = LW_RET_FAILED;

	// --- declara??es antecipadas (evita warnings com goto) ---
	lwISystem* sys = 0;
	lwIPathInfo* path_info = 0;
	lwIResourceMgr* res_mgr = 0;
	lwIShaderMgr* shader_mgr = 0;
	lwIShaderDeclMgr* decl_mgr = 0;

	char path[LW_MAX_PATH];

	if (!sys_graphics)
		goto __ret;

	sys = sys_graphics->GetSystem();
	if (!sys)
		goto __ret;

	{
		LW_RESULT r = sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);
		if (LW_FAILED(r) || !path_info) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] sys->GetInterface(LW_GUID_PATHINFO) failed: ret={}, path_info={}",
						 __FUNCTION__, static_cast<long long>(r), static_cast<const void*>(path_info));
			goto __ret;
		}
	}

	{
		LW_RESULT r = sys_graphics->GetInterface((LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR);
		if (LW_FAILED(r) || !res_mgr) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] sys_graphics->GetInterface(LW_GUID_RESOURCEMGR) failed: ret={}, res_mgr={}",
						 __FUNCTION__, static_cast<long long>(r), static_cast<const void*>(res_mgr));
			goto __ret;
		}
	}

	shader_mgr = res_mgr->GetShaderMgr();
	if (!shader_mgr)
		goto __ret;


	// ======== DX9: Vertex Decls ========
	// torne os arrays static para evitar o warning com goto
	static D3DVERTEXELEMENT9 ve0[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};
	static D3DVERTEXELEMENT9 ve1[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};
	static D3DVERTEXELEMENT9 ve2[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};
	static D3DVERTEXELEMENT9 ve3[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};
	static D3DVERTEXELEMENT9 vepnt0[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};
	static D3DVERTEXELEMENT9 vepndt0[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	static D3DVERTEXELEMENT9* ve_buf[] = {
		ve0, ve1, ve2, ve3,
		vepnt0, vepnt0, vepndt0, vepnt0, vepndt0, vepndt0, vepndt0
	};
	static const DWORD decl_type[] = {
		VDT_PU4NT0, VDT_PB1U4NT0, VDT_PB2U4NT0, VDT_PB3U4NT0,
		VDT_PNT0, VDT_PNT0, VDT_PNDT0, VDT_PNT0,
		VDT_PNDT0, VDT_PNDT0, VDT_PNDT0,
	};
	{
		const int decl_num = (int)(sizeof(decl_type) / sizeof(decl_type[0]));
		int i;
		IDirect3DVertexDeclarationX* this_decl = 0;
		for (i = 0; i < decl_num; ++i) {
			this_decl = 0;
			if (LW_SUCCEEDED(shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i])))
				continue;
			if (LW_RESULT r = shader_mgr->RegisterVertexDeclaration(decl_type[i], ve_buf[i]); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] shader_mgr->RegisterVertexDeclaration failed: i={}, decl_type={}, ret={}",
							 __FUNCTION__, i, decl_type[i], static_cast<long long>(r));
				goto __ret;
			}
		}
	}


	// ======== DX9: Vertex Shaders ========
	// Все 11 static-mesh VS сведены к двум HLSL-мастерам:
	//   static_skin.hlsl — 4 permutations via NUM_EXPLICIT_WEIGHTS:
	//     0 → pu4nt0_ld    (single-bone, no blend)
	//     1 → pb1u4nt0_ld  (2-bone blend)
	//     2 → pb2u4nt0_ld  (3-bone blend)
	//     3 → pb3u4nt0_ld  (4-bone blend)
	//   vs_pndt0.hlsl — 7 permutations via NO_LIGHTING/NO_DIFFUSE/USE_TEX_TRANSFORM.
	static const DWORD shader_type[] = {
		VST_PU4NT0_LD, VST_PB1U4NT0_LD, VST_PB2U4NT0_LD, VST_PB3U4NT0_LD,
		VST_PNT0_LD_TT0, VST_PNT0_TT0, VST_PNDT0_LD_TT0,
		VST_PNT0_LD, VST_PNDT0, VST_PNDT0_LD, VST_PNDT0_TT0,
	};
	static const char* shader_file[] = {
		"static_skin.hlsl", "static_skin.hlsl", "static_skin.hlsl", "static_skin.hlsl",
		"vs_pndt0.hlsl", "vs_pndt0.hlsl", "vs_pndt0.hlsl",
		"vs_pndt0.hlsl", "vs_pndt0.hlsl", "vs_pndt0.hlsl", "vs_pndt0.hlsl",
	};
	// Все шейдеры HLSL после рефакторинга.
	static const D3DXMACRO defs_skin0[] = {{"NUM_EXPLICIT_WEIGHTS", "0"}, {NULL,NULL}};
	static const D3DXMACRO defs_skin1[] = {{"NUM_EXPLICIT_WEIGHTS", "1"}, {NULL,NULL}};
	static const D3DXMACRO defs_skin2[] = {{"NUM_EXPLICIT_WEIGHTS", "2"}, {NULL,NULL}};
	static const D3DXMACRO defs_skin3[] = {{"NUM_EXPLICIT_WEIGHTS", "3"}, {NULL,NULL}};
	static const D3DXMACRO defs_pnt0_ld_tt0[] = {{"USE_TEX_TRANSFORM", ""}, {"NO_DIFFUSE", ""}, {NULL,NULL}};
	static const D3DXMACRO defs_pnt0_tt0[] = {
		{"NO_LIGHTING", ""}, {"USE_TEX_TRANSFORM", ""}, {"NO_DIFFUSE", ""}, {NULL,NULL}
	};
	static const D3DXMACRO defs_pndt0_ld_tt0[] = {{"USE_TEX_TRANSFORM", ""}, {NULL,NULL}};
	static const D3DXMACRO defs_pnt0_ld[] = {{"NO_DIFFUSE", ""}, {NULL,NULL}};
	static const D3DXMACRO defs_pndt0[] = {{"NO_LIGHTING", ""}, {NULL,NULL}};
	static const D3DXMACRO defs_pndt0_ld[] = {{NULL,NULL}};
	static const D3DXMACRO defs_pndt0_tt0[] = {{"NO_LIGHTING", ""}, {"USE_TEX_TRANSFORM", ""}, {NULL,NULL}};
	static const D3DXMACRO* defines_tab[] = {
		defs_skin0, defs_skin1, defs_skin2, defs_skin3,
		defs_pnt0_ld_tt0, defs_pnt0_tt0, defs_pndt0_ld_tt0,
		defs_pnt0_ld, defs_pndt0, defs_pndt0_ld, defs_pndt0_tt0,
	};
	static const int shader_num = (int)(sizeof(shader_type) / sizeof(shader_type[0]));

	{
		int i;
		for (i = 0; i < shader_num; ++i) {
			sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
			if (LW_RESULT r = shader_mgr->RegisterVertexShader(shader_type[i], path, VS_FILE_HLSL, defines_tab[i]);
				LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] shader_mgr->RegisterVertexShader failed: i={}, shader_type={}, path={}, ret={}",
							 __FUNCTION__, i, shader_type[i], path, static_cast<long long>(r));
				goto __ret;
			}
		}
	}


	// ======== ShaderDeclMgr ========
	decl_mgr = shader_mgr->GetShaderDeclMgr(); // <- sem inicializador na declara??o
	if (!decl_mgr)
		goto __ret;

	decl_mgr->CreateShaderDeclSet(VDT_PNT0, 8);
	decl_mgr->CreateShaderDeclSet(VDT_PNDT0, 8);

	// tambm deixe esttico o sdci_num e o array sdci
	static const DWORD sdci_num = 4;
	static lwShaderDeclCreateInfo sdci[sdci_num] = {
		SDCI_VALUE(VST_PNT0_LD_TT0, VDT_PNT0, VSLT_DIRECTIONAL, VSAT_TEXTURETRANSFORM0, "vs_pnt0_ld_t0uvmat.vsh"),
		SDCI_VALUE(VST_PNT0_TT0, VDT_PNT0, VSLT_INVALID, VSAT_TEXTURETRANSFORM0, "vs_pnt0_t0uvmat.vsh"),
		SDCI_VALUE(VST_PNDT0_LD_TT0, VDT_PNDT0, VSLT_DIRECTIONAL, VSAT_TEXTURETRANSFORM0, "vs_pndt0_ld_t0uvmat.vsh"),
		SDCI_VALUE(VST_PNDT0_TT0, VDT_PNDT0, VSLT_INVALID, VSAT_TEXTURETRANSFORM0, "vs_pndt0_t0uvmat.vsh"),
	};
	{
		DWORD i;
		for (i = 0; i < sdci_num; ++i)
			decl_mgr->SetShaderDeclInfo(&sdci[i]);
	}

	ret = LW_RET_OK;

__ret:
	if (ret != LW_RET_OK)
		LG_MSGBOX("LoadShader0 error");
	return ret;
}


LW_RESULT LoadShader1(lwISysGraphics* sys_graphics) {
	LW_RESULT ret = LW_RET_FAILED;

	lwISystem* sys = sys_graphics->GetSystem();

	char path[LW_MAX_PATH];
	lwIPathInfo* path_info = 0;
	sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

	lwIResourceMgr* res_mgr;
	lwIShaderMgr* shader_mgr;

	sys_graphics->GetInterface((LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR);
	shader_mgr = res_mgr->GetShaderMgr();


	DWORD shader_type[] =
	{
		VSTU_SKINMESH0_TT1,
		VSTU_SKINMESH1_TT1,
		VSTU_SKINMESH2_TT1,
		VSTU_SKINMESH3_TT1,

		VSTU_SKINMESH0_TT2,
		VSTU_SKINMESH1_TT2,
		VSTU_SKINMESH2_TT2,
		VSTU_SKINMESH3_TT2,

		VSTU_SKINMESH0_TT3,
		VSTU_SKINMESH1_TT3,
		VSTU_SKINMESH2_TT3,
		VSTU_SKINMESH3_TT3,

	};

	int shader_num = sizeof(shader_type) / sizeof(shader_type[0]);


	// dx9
	//    "skinmesh9_1.vsh",
	//    "skinmesh9_2.vsh",
	//    "skinmesh9_3.vsh",
	//    "skinmesh9_4.vsh",

	// permutations через общий мастер-HLSL skinmesh.hlsl с defines
	// NUM_SKIN_WEIGHTS (1|2) и TT_MODE (1|2|3).
	const char* shader_file[] =
	{
		"skinmesh.hlsl", "skinmesh.hlsl", // TT1: 1w / 2w
		"skinmesh.hlsl", "skinmesh.hlsl", // TT2: 1w / 2w
		"skinmesh.hlsl", "skinmesh.hlsl", // TT3: 1w / 2w
	};
	static const D3DXMACRO defs_1w_tt1[] = {{"NUM_SKIN_WEIGHTS", "1"}, {"TT_MODE", "1"}, {NULL,NULL}};
	static const D3DXMACRO defs_2w_tt1[] = {{"NUM_SKIN_WEIGHTS", "2"}, {"TT_MODE", "1"}, {NULL,NULL}};
	static const D3DXMACRO defs_1w_tt2[] = {{"NUM_SKIN_WEIGHTS", "1"}, {"TT_MODE", "2"}, {NULL,NULL}};
	static const D3DXMACRO defs_2w_tt2[] = {{"NUM_SKIN_WEIGHTS", "2"}, {"TT_MODE", "2"}, {NULL,NULL}};
	static const D3DXMACRO defs_1w_tt3[] = {{"NUM_SKIN_WEIGHTS", "1"}, {"TT_MODE", "3"}, {NULL,NULL}};
	static const D3DXMACRO defs_2w_tt3[] = {{"NUM_SKIN_WEIGHTS", "2"}, {"TT_MODE", "3"}, {NULL,NULL}};
	const D3DXMACRO* defs_tab[] =
	{
		defs_1w_tt1, defs_2w_tt1,
		defs_1w_tt2, defs_2w_tt2,
		defs_1w_tt3, defs_2w_tt3,
	};

	for (int i = 0; i < 6; i++) {
		sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
		if (LW_RESULT r = shader_mgr->RegisterVertexShader(shader_type[i], path, VS_FILE_HLSL, defs_tab[i]);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] shader_mgr->RegisterVertexShader (skinmesh) failed: i={}, shader_type={}, path={}, ret={}",
						 __FUNCTION__, i, shader_type[i], path, static_cast<long long>(r));
			goto __ret;
		}
	}


	ret = LW_RET_OK;
__ret:
	if (ret != LW_RET_OK) {
		LG_MSGBOX("LoadShader1 error");
	}

	return ret;
}
