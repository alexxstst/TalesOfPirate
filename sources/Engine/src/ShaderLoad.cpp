//
#include "stdafx.h"

#include "ShaderLoad.h"
#include "lwgraphicsutil.h"


#define USER_SHADER_NUM             8

LW_RESULT LoadShader0(lwISysGraphics* sys_graphics)
{
    LW_RESULT ret = LW_RET_FAILED;

    // --- declara??es antecipadas (evita warnings com goto) ---
    lwISystem*        sys        = 0;
    lwIPathInfo*      path_info  = 0;
    lwIResourceMgr*   res_mgr    = 0;
    lwIShaderMgr*     shader_mgr = 0;
    lwIShaderDeclMgr* decl_mgr   = 0;

    char path[LW_MAX_PATH];

    if (!sys_graphics)
        goto __ret;

    sys = sys_graphics->GetSystem();
    if (!sys)
        goto __ret;

    if (LW_FAILED(sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO)) || !path_info)
        goto __ret;

    if (LW_FAILED(sys_graphics->GetInterface((LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR)) || !res_mgr)
        goto __ret;

    shader_mgr = res_mgr->GetShaderMgr();
    if (!shader_mgr)
        goto __ret;

#if defined(LW_USE_DX9)

    // ======== DX9: Vertex Decls ========
    // torne os arrays static para evitar o warning com goto
    static D3DVERTEXELEMENT9 ve0[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0},
        {0, 12, D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
        {0, 16, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0},
        {0, 28, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };
    static D3DVERTEXELEMENT9 ve1[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0},
        {0, 12, D3DDECLTYPE_FLOAT1,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
        {0, 16, D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
        {0, 20, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0},
        {0, 32, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };
    static D3DVERTEXELEMENT9 ve2[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0},
        {0, 12, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
        {0, 20, D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
        {0, 24, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0},
        {0, 36, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };
    static D3DVERTEXELEMENT9 ve3[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0},
        {0, 12, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
        {0, 24, D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
        {0, 28, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0},
        {0, 40, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };
    static D3DVERTEXELEMENT9 vepnt0[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
        {0, 24, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };
    static D3DVERTEXELEMENT9 vepndt0[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
        {0, 24, D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
        {0, 28, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0xFF, 0, D3DDECLTYPE_UNUSED,0, 0, 0},
    };

    static D3DVERTEXELEMENT9* ve_buf[] = {
        ve0, ve1, ve2, ve3,
        vepnt0, vepnt0, vepndt0, vepnt0, vepndt0, vepndt0, vepndt0
    };
    static const DWORD decl_type[] = {
        VDT_PU4NT0, VDT_PB1U4NT0, VDT_PB2U4NT0, VDT_PB3U4NT0,
        VDT_PNT0,   VDT_PNT0,     VDT_PNDT0,    VDT_PNT0,
        VDT_PNDT0,  VDT_PNDT0,    VDT_PNDT0,
    };
    {
        const int decl_num = (int)(sizeof(decl_type) / sizeof(decl_type[0]));
        int i;
        IDirect3DVertexDeclarationX* this_decl = 0;
        for (i = 0; i < decl_num; ++i)
        {
            this_decl = 0;
            if (LW_SUCCEEDED(shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i])))
                continue;
            if (LW_FAILED(shader_mgr->RegisterVertexDeclaration(decl_type[i], ve_buf[i])))
                goto __ret;
        }
    }

#endif // DX9


#if defined(LW_USE_DX9)

    // ======== DX9: Vertex Shaders ========
    static const DWORD shader_type[] = {
        VST_PU4NT0_LD, VST_PB1U4NT0_LD, VST_PB2U4NT0_LD, VST_PB3U4NT0_LD,
        VST_PNT0_LD_TT0, VST_PNT0_TT0, VST_PNDT0_LD_TT0,
        VST_PNT0_LD, VST_PNDT0, VST_PNDT0_LD, VST_PNDT0_TT0,
    };
    static const char* shader_file[] = {
        "pu4nt0_ld.vsh", "pb1u4nt0_ld.vsh", "pb2u4nt0_ld.vsh", "pb3u4nt0_ld.vsh",
        "vs_pnt0_ld_t0uvmat.vsh", "vs_pnt0_t0uvmat.vsh", "vs_pndt0_ld_t0uvmat.vsh",
        "vs_pnt0_ld.vsh", "vs_pndt0.vsh", "vs_pndt0_ld.vsh", "vs_pndt0_t0uvmat.vsh",
    };
    static const DWORD file_types[] = {
        VS_FILE_ASM, VS_FILE_ASM, VS_FILE_ASM, VS_FILE_ASM,
        VS_FILE_ASM, VS_FILE_ASM, VS_FILE_ASM,
        VS_FILE_ASM, VS_FILE_ASM, VS_FILE_ASM, VS_FILE_ASM,
    };
    static D3DXMACRO defines[] = {
        { "NUM_SKIN_WEIGHTS", "1"},
        { "NUM_SKIN_WEIGHTS", "2"},
        { "NUM_SKIN_WEIGHTS", "3"},
        { "NUM_SKIN_WEIGHTS", "4"},
        {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
    };
    static const int shader_num = (int)(sizeof(shader_type) / sizeof(shader_type[0]));

    {
        int i;
        for (i = 0; i < shader_num; ++i)
        {
            sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
            if (LW_FAILED(shader_mgr->RegisterVertexShader(shader_type[i], path, file_types[i], &defines[i])))
                goto __ret;
        }
    }

#elif defined(LW_USE_DX8)

    // ======== DX8: Vertex Shaders ========
    static const DWORD shader_type[] = {
        VST_PU4NT0_LD, VST_PB1U4NT0_LD, VST_PB2U4NT0_LD, VST_PB3U4NT0_LD,
        VST_PNT0_LD_TT0, VST_PNT0_TT0, VST_PNDT0_LD_TT0,
        VST_PNT0_LD, VST_PNDT0, VST_PNDT0_LD, VST_PNDT0_TT0,
    };
    static DWORD dwDecl0[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,    D3DVSDT_FLOAT3),
        // D3DVSD_REG(VSREG_V_BLENDWEIGHT,D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_BLENDINDICES,D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_NORMAL,      D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,   D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD dwDecl1[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT, D3DVSDT_FLOAT1),
        D3DVSD_REG(VSREG_V_BLENDINDICES,D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_NORMAL,      D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,   D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD dwDecl2[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT, D3DVSDT_FLOAT2),
        D3DVSD_REG(VSREG_V_BLENDINDICES,D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_NORMAL,      D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,   D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD dwDecl3[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT, D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDINDICES,D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_NORMAL,      D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,   D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD decl_pnt0[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,   D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_NORMAL,     D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,  D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD decl_pndt0[] = {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,   D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_NORMAL,     D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_DIFFUSE,    D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_TEXCOORD0,  D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    static DWORD* decl_tab[] = {
        dwDecl0, dwDecl1, dwDecl2, dwDecl3,
        decl_pnt0, decl_pnt0, decl_pndt0, decl_pnt0,
        decl_pndt0, decl_pndt0, decl_pndt0,
    };
    static DWORD decl_size[] = {
        sizeof(dwDecl0), sizeof(dwDecl1), sizeof(dwDecl2), sizeof(dwDecl3),
        sizeof(decl_pnt0), sizeof(decl_pnt0), sizeof(decl_pndt0), sizeof(decl_pnt0),
        sizeof(decl_pndt0), sizeof(decl_pndt0), sizeof(decl_pndt0),
    };
    static const char* shader_file[] = {
        "pu4nt0_ld.vso", "pb1u4nt0_ld.vso", "pb2u4nt0_ld.vso", "pb3u4nt0_ld.vso",
        "pnt0_ld_t0uvmat.vso", "pnt0_t0uvmat.vso", "pndt0_ld_t0uvmat.vso",
        "pnt0_ld.vso", "pndt0.vso", "pndt0_ld.vso", "pndt0_t0uvmat.vso",
    };
    static const int shader_num = (int)(sizeof(shader_type) / sizeof(shader_type[0]));

    {
        int i;
        for (i = 0; i < shader_num; ++i)
        {
            sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
            if (LW_FAILED(shader_mgr->RegisterVertexShader(shader_type[i], path, 0, decl_tab[i], decl_size[i], 1)))
            {
                LG_MSGBOX("Load Vertex Shader Error");
                return LW_RET_FAILED; // como no original
            }
        }
    }

#endif // DX8 / DX9

    // ======== ShaderDeclMgr ========
    decl_mgr = shader_mgr->GetShaderDeclMgr();  // <- sem inicializador na declara??o
    if (!decl_mgr)
        goto __ret;

    decl_mgr->CreateShaderDeclSet(VDT_PNT0,  8);
    decl_mgr->CreateShaderDeclSet(VDT_PNDT0, 8);

    // tambm deixe esttico o sdci_num e o array sdci
    static const DWORD sdci_num = 4;
    static lwShaderDeclCreateInfo sdci[sdci_num] = {
        SDCI_VALUE(VST_PNT0_LD_TT0,  VDT_PNT0,  VSLT_DIRECTIONAL, VSAT_TEXTURETRANSFORM0, "vs_pnt0_ld_t0uvmat.vsh"),
        SDCI_VALUE(VST_PNT0_TT0,     VDT_PNT0,  VSLT_INVALID,     VSAT_TEXTURETRANSFORM0, "vs_pnt0_t0uvmat.vsh"),
        SDCI_VALUE(VST_PNDT0_LD_TT0, VDT_PNDT0, VSLT_DIRECTIONAL, VSAT_TEXTURETRANSFORM0, "vs_pndt0_ld_t0uvmat.vsh"),
        SDCI_VALUE(VST_PNDT0_TT0,    VDT_PNDT0, VSLT_INVALID,     VSAT_TEXTURETRANSFORM0, "vs_pndt0_t0uvmat.vsh"),
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


LW_RESULT LoadShader1(lwISysGraphics* sys_graphics)
{
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

#if (defined LW_USE_DX9)

    // dx9
    //const char* shader_file[] = 
    //{
    //    "skinmesh9_1.vsh",
    //    "skinmesh9_2.vsh",
    //    "skinmesh9_3.vsh",
    //    "skinmesh9_4.vsh",
    //};

	const char* shader_file[] = 
    {
        "skinmesh8_1_tt1.vsh",
        "skinmesh8_2_tt1.vsh",
        "skinmesh8_1_tt2.vsh",
        "skinmesh8_2_tt2.vsh",
        "skinmesh8_1_tt3.vsh",
        "skinmesh8_2_tt3.vsh",
    };

    for(int i = 0; i < 6; i++)
    {
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
        if(LW_FAILED(shader_mgr->RegisterVertexShader(shader_type[i], path, VS_FILE_ASM)))
            goto __ret;
    }

#elif (defined LW_USE_DX8)

    DWORD dwDecl0[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
//        D3DVSD_REG(VSREG_V_BLENDWEIGHT,  D3DVSDT_D3DCOLOR),
        D3DVSD_REG(VSREG_V_BLENDINDICES,  D3DVSDT_D3DCOLOR),        
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0, D3DVSDT_FLOAT2),
        D3DVSD_END()
    };

    DWORD dwDecl1[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT,  D3DVSDT_FLOAT1),
        D3DVSD_REG(VSREG_V_BLENDINDICES,  D3DVSDT_D3DCOLOR),        
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0, D3DVSDT_FLOAT2),
        D3DVSD_END()
    };

    DWORD dwDecl2[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT,  D3DVSDT_FLOAT2),
        D3DVSD_REG(VSREG_V_BLENDINDICES,  D3DVSDT_D3DCOLOR),        
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0, D3DVSDT_FLOAT2),
        D3DVSD_END()
    };

    DWORD dwDecl3[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDWEIGHT,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_BLENDINDICES,  D3DVSDT_D3DCOLOR),        
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0, D3DVSDT_FLOAT2),
        D3DVSD_END()
    };

    DWORD decl_pnt0[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_TEXCOORD0,  D3DVSDT_FLOAT2),
        D3DVSD_END()
    };
    DWORD decl_pndt0[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(VSREG_V_POSITION,  D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_NORMAL,    D3DVSDT_FLOAT3),
        D3DVSD_REG(VSREG_V_DIFFUSE,   D3DVSDT_D3DCOLOR),        
        D3DVSD_REG(VSREG_V_TEXCOORD0,  D3DVSDT_FLOAT2),
        D3DVSD_END()
    };

    DWORD* decl_tab[] =
    {
        dwDecl0,
        dwDecl1,
        dwDecl2,
        dwDecl3,

        dwDecl0,
        dwDecl1,
        dwDecl2,
        dwDecl3,

        dwDecl0,
        dwDecl1,
        dwDecl2,
        dwDecl3,

        decl_pnt0,
        decl_pnt0,
        decl_pndt0,
        decl_pnt0,
        decl_pndt0,
        decl_pndt0,
        decl_pndt0,
    };
    DWORD decl_size[] =
    {
        // tt1
        sizeof(dwDecl0),
        sizeof(dwDecl1),
        sizeof(dwDecl2),
        sizeof(dwDecl3),
        // tt2
        sizeof(dwDecl0),
        sizeof(dwDecl1),
        sizeof(dwDecl2),
        sizeof(dwDecl3),
        // tt3
        sizeof(dwDecl0),
        sizeof(dwDecl1),
        sizeof(dwDecl2),
        sizeof(dwDecl3),

        // unused
        sizeof(decl_pnt0),
        sizeof(decl_pnt0),
        sizeof(decl_pndt0),
        sizeof(decl_pnt0),
        sizeof(decl_pndt0),
        sizeof(decl_pndt0),
        sizeof(decl_pndt0),
    };

	const char* shader_file[] = 
    {
        "pu4nt0_ld_tt1.vso",
        "pb1u4nt0_ld_tt1.vso",
        "pu4nt0_ld_tt2.vso",
        "pb1u4nt0_ld_tt2.vso",
        "pu4nt0_ld_tt3.vso",
        "pb1u4nt0_ld_tt3.vso",
    };


    for(int i = 0; i < 6; i++)
    {
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);

        if(LW_FAILED(shader_mgr->RegisterVertexShader(shader_type[i], path, 0, decl_tab[i], decl_size[i], 1)))
        {
            LG_MSGBOX("Load Vertex Shader Error\n%s", shader_file[i]);
            goto __ret;
        }
    }

#endif


    ret = LW_RET_OK;
__ret:
    if(ret != LW_RET_OK)
    {
        LG_MSGBOX("LoadShader1 error");
    }

    return ret;
}


