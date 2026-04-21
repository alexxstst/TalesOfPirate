//
#include "stdafx.h"
#include "lwGuidObj.h"
#include "lwIFunc.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwShaderMgr.h"
#include "lwGraphicsUtil.h"
#include "lwShaderTypes.h"
#include "lwPreDefinition.h"
#include "lwxRenderCtrlVS.h"


LW_BEGIN

static lwISystem* g_system = 0;
static lwISysGraphics* g_sys_graphics = 0;

struct lwLoseResetDevInfo
{
    lwOutputLoseDeviceProc lose_proc;
    lwOutputResetDeviceProc reset_proc;
};


#define MAX_LOSE_RESET_DEV_PROC_NUM 8
static LW_RESULT __global_lose_dev_entry();
static LW_RESULT __global_reset_dev_entry();
static lwOutputLoseDeviceProc g_lose_dev_seq[MAX_LOSE_RESET_DEV_PROC_NUM];
static lwOutputResetDeviceProc g_reset_dev_seq[MAX_LOSE_RESET_DEV_PROC_NUM];


static LW_RESULT __global_lose_dev_entry()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_lose_dev_seq[i])
        {
            if(LW_FAILED((*g_lose_dev_seq[i])()))
                goto __ret;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
};

static LW_RESULT __global_reset_dev_entry()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_reset_dev_seq[i])
        {
            if(LW_FAILED((*g_reset_dev_seq[i])()))
                goto __ret;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
};

LW_RESULT lwRegisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc)
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_lose_dev_seq[i])
            continue;

        g_lose_dev_seq[i] = proc;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

LW_RESULT lwRegisterOutputResetDeviceProc(lwOutputResetDeviceProc proc)
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_reset_dev_seq[i])
            continue;

        g_reset_dev_seq[i] = proc;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

LW_RESULT lwUnregisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc)
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_lose_dev_seq[i] != proc)
            continue;

        g_lose_dev_seq[i] = 0;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}
LW_RESULT lwUnregisterOutputResetDeviceProc(lwOutputResetDeviceProc proc)
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_LOSE_RESET_DEV_PROC_NUM; i++)
    {
        if(g_reset_dev_seq[i] != proc)
            continue;

        g_reset_dev_seq[i] = 0;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

void lwSetActiveIGraphicsSystem(lwISysGraphics* sys_graphics)
{
    lwSysGraphics::SetActiveIGraphicsSystem(sys_graphics);
}

lwISysGraphics* lwGetActiveIGraphicsSystem()
{
    return lwSysGraphics::GetActiveIGraphicsSystem();
}

//
LW_RESULT lwAdjustD3DCreateParam(IDirect3DX* d3d, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* adjust_info)
{
    if(param->present_param.BackBufferFormat == D3DFMT_UNKNOWN)
    {
        D3DDISPLAYMODE d3ddm;
        d3d->GetAdapterDisplayMode(param->adapter, &d3ddm);
        param->present_param.BackBufferFormat = d3ddm.Format;
    }

    if(adjust_info->multi_sample_type)
    {
        while(adjust_info->multi_sample_type > 0)
        {
            if(SUCCEEDED(d3d->CheckDeviceMultiSampleTypeX(
                param->adapter, 
                param->dev_type, 
                param->present_param.BackBufferFormat,
                param->present_param.Windowed, 
                adjust_info->multi_sample_type, 
                NULL
                )))
            {
                param->present_param.MultiSampleType = adjust_info->multi_sample_type;
                param->present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
                break;
            }

            adjust_info->multi_sample_type = (D3DMULTISAMPLE_TYPE)((DWORD)adjust_info->multi_sample_type - 1);
        }
    }

    return LW_RET_OK;
}

LW_RESULT lwInitMeshLibSystem(lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwISystem* sys = 0;
    lwISysGraphics* sys_graphics = 0;
    lwIDeviceObject* dev_obj = 0;

    sys = LW_NEW(lwSystem);
    if(!sys) {
        goto __ret;
    }

    if(LW_FAILED(sys->Initialize()))
        goto __ret;

    {
        // begin init path info
        lwIPathInfo* path_info = 0;

        if (LW_FAILED(sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO)))
            goto __ret;

        path_info->SetPath(PATH_TYPE_WORKINGDIRECTORY, ".\\");

        path_info->SetPath(PATH_TYPE_MODEL, ".\\model\\");
        path_info->SetPath(PATH_TYPE_MODEL_CHARACTER, ".\\model\\character\\");
        path_info->SetPath(PATH_TYPE_MODEL_SCENE, ".\\model\\scene\\");
        path_info->SetPath(PATH_TYPE_MODEL_ITEM, ".\\model\\item\\");

        path_info->SetPath(PATH_TYPE_TEXTURE, ".\\texture\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_CHARACTER, ".\\texture\\character\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_SCENE, ".\\texture\\scene\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_ITEM, ".\\texture\\item\\");

        path_info->SetPath(PATH_TYPE_ANIMATION, ".\\animation\\");
#if(defined LW_USE_DX8)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx8\\");
#endif
#if(defined LW_USE_DX9)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");
#endif

        // end

        if (LW_FAILED(sys->CreateGraphicsSystem(&sys_graphics)))
            goto __ret;

        lwInitUserRenderCtrlVSProc(sys_graphics->GetResourceMgr());

        lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
        opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, 0);

        // begin set default active system
        lwSetActiveISystem(sys);
        lwSetActiveIGraphicsSystem(sys_graphics);
        // end


        g_system = sys;
        g_sys_graphics = sys_graphics;

        if (ret_sys)
        {
            *ret_sys = sys;
        }

        if (ret_sys_graphics)
        {
            *ret_sys_graphics = sys_graphics;
        }

        sys = 0;
        sys_graphics = 0;

        ret = LW_RET_OK;
    }
__ret:
    LW_SAFE_RELEASE(sys);
    LW_SAFE_RELEASE(sys_graphics);

    return ret;

}

LW_RESULT lwInitMeshLibSystem(lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, IDirect3DX* d3d, IDirect3DDeviceX* dev, HWND hwnd)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwISystem* sys = 0;
    lwISysGraphics* sys_graphics = 0;
    lwIDeviceObject* dev_obj = 0;

    sys = LW_NEW(lwSystem);
    if(!sys) {
        goto __ret;
    }

    if(LW_FAILED(sys->Initialize()))
        goto __ret;

    // check current directx version
    {
        DWORD ver = sys->GetSystemInfo()->GetDirectXVersion();
        if(ver < DX_VERSION_8_1)
        {
            ret = INIT_ERR_DX_VERSION;
            goto __ret;
        }
    }
    {
        // begin init path info
        lwIPathInfo* path_info = 0;

        if (LW_FAILED(sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO)))
            goto __ret;

        path_info->SetPath(PATH_TYPE_WORKINGDIRECTORY, ".\\");

        path_info->SetPath(PATH_TYPE_MODEL, ".\\model\\");
        path_info->SetPath(PATH_TYPE_MODEL_CHARACTER, ".\\model\\character\\");
        path_info->SetPath(PATH_TYPE_MODEL_SCENE, ".\\model\\scene\\");
        path_info->SetPath(PATH_TYPE_MODEL_ITEM, ".\\model\\item\\");

        path_info->SetPath(PATH_TYPE_TEXTURE, ".\\texture\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_CHARACTER, ".\\texture\\character\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_SCENE, ".\\texture\\scene\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_ITEM, ".\\texture\\item\\");

        path_info->SetPath(PATH_TYPE_ANIMATION, ".\\animation\\");
#if(defined LW_USE_DX8)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx8\\");
#endif
#if(defined LW_USE_DX9)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");
#endif
        // end

        if (LW_FAILED(sys->CreateGraphicsSystem(&sys_graphics)))
            goto __ret;

        // set golbal lose-reset dev call back
        sys_graphics->SetOutputLoseDeviceProc(__global_lose_dev_entry);
        sys_graphics->SetOutputResetDeviceProc(__global_reset_dev_entry);

        dev_obj = sys_graphics->GetDeviceObject();
        dev_obj->SetDirect3D(d3d);
        dev_obj->SetDevice(dev);

        if (LW_FAILED(dev_obj->InitStateCache()))
            goto __ret;

        if (LW_FAILED(dev_obj->InitCapsInfo()))
            goto __ret;

        lwIResourceMgr* res_mgr = sys_graphics->GetResourceMgr();
        lwIStaticStreamMgr* ssm = res_mgr->GetStaticStreamMgr();
        ssm->CreateStreamEntitySeq(4096, 4096);
        ssm->CreateVertexBufferStream(0, 1024 * 1024);
        ssm->CreateVertexBufferStream(1, 1024 * 1024);
        ssm->CreateIndexBufferStream(0, 1024 * 1024);
        lwIDynamicStreamMgr* dsm = res_mgr->GetDynamicStreamMgr();
        dsm->Create(512 * 1024, 1024 * 512);

        // begin set default active system
        lwSetActiveISystem(sys);
        lwSetActiveIGraphicsSystem(sys_graphics);
        // end

        lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
        opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, 1);
#if(defined OPT_CULLPRIMITIVE)
        opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);
#endif

        //
        lwInitUserRenderCtrlVSProc(res_mgr);

        g_system = sys;
        g_sys_graphics = sys_graphics;

        if (ret_sys)
        {
            *ret_sys = sys;
        }

        if (ret_sys_graphics)
        {
            *ret_sys_graphics = sys_graphics;
        }

        sys = 0;
        sys_graphics = 0;

        ret = LW_RET_OK;
    }
__ret:
    LW_SAFE_RELEASE(sys);
    LW_SAFE_RELEASE(sys_graphics);

    return ret;
}

LW_RESULT lwInitMeshLibSystem(lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* param_info)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwISystem* sys = 0;
    lwISysGraphics* sys_graphics = 0;
    lwIDeviceObject* dev_obj = 0;

    sys = LW_NEW(lwSystem);
    if(!sys) {
        goto __ret;
    }

    if(LW_FAILED(sys->Initialize()))
        goto __ret;

    // check current directx version
    {
        DWORD ver = sys->GetSystemInfo()->GetDirectXVersion();
        if(ver < DX_VERSION_8_1)
        {
            ret = INIT_ERR_DX_VERSION;
            goto __ret;
        }
    }

    if(LW_FAILED(sys->CreateGraphicsSystem(&sys_graphics)))
        goto __ret;

    // set golbal lose-reset dev call back
    sys_graphics->SetOutputLoseDeviceProc(__global_lose_dev_entry);
    sys_graphics->SetOutputResetDeviceProc(__global_reset_dev_entry);

    {
        // begin init path info
        lwIPathInfo* path_info = 0;

        if (LW_FAILED(sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO)))
            goto __ret;

        path_info->SetPath(PATH_TYPE_WORKINGDIRECTORY, ".\\");

        path_info->SetPath(PATH_TYPE_MODEL, ".\\model\\");
        path_info->SetPath(PATH_TYPE_MODEL_CHARACTER, ".\\model\\character\\");
        path_info->SetPath(PATH_TYPE_MODEL_SCENE, ".\\model\\scene\\");
        path_info->SetPath(PATH_TYPE_MODEL_ITEM, ".\\model\\item\\");

        path_info->SetPath(PATH_TYPE_TEXTURE, ".\\texture\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_CHARACTER, ".\\texture\\character\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_SCENE, ".\\texture\\scene\\");
        path_info->SetPath(PATH_TYPE_TEXTURE_ITEM, ".\\texture\\item\\");

        path_info->SetPath(PATH_TYPE_ANIMATION, ".\\animation\\");
#if(defined LW_USE_DX8)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx8\\");
#endif
#if(defined LW_USE_DX9)
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");
#endif
        // end


        dev_obj = sys_graphics->GetDeviceObject();

        if (LW_FAILED(dev_obj->CreateDirect3D()))
        {
            ret = INIT_ERR_CREATE_D3D;
            goto __ret;
        }

        if (param_info)
        {
            if (LW_FAILED(lwAdjustD3DCreateParam(dev_obj->GetDirect3D(), param, param_info)))
                goto __ret;
        }

        if (LW_FAILED(dev_obj->CreateDevice(param)))
        {
            ret = INIT_ERR_CREATE_DEVICE;
            goto __ret;
        }

        if (LW_FAILED(dev_obj->InitStateCache()))
            goto __ret;

        if (LW_FAILED(dev_obj->InitCapsInfo()))
            goto __ret;

        lwIResourceMgr* res_mgr = sys_graphics->GetResourceMgr();
        lwIStaticStreamMgr* ssm = res_mgr->GetStaticStreamMgr();
        ssm->CreateStreamEntitySeq(4096, 4096);
        ssm->CreateVertexBufferStream(0, 1024 * 1024);
        ssm->CreateVertexBufferStream(1, 1024 * 1024);
        ssm->CreateVertexBufferStream(2, 1024 * 1024);
        ssm->CreateIndexBufferStream(0, 1024 * 1024);
        lwIDynamicStreamMgr* dsm = res_mgr->GetDynamicStreamMgr();
        dsm->Create(512 * 1024, 1024 * 512);

        // begin set default active system
        lwSetActiveISystem(sys);
        lwSetActiveIGraphicsSystem(sys_graphics);
        // end

        //
        lwInitUserRenderCtrlVSProc(res_mgr);

        /*
        // set render state
        dev_obj->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        dev_obj->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        dev_obj->SetRenderState(D3DRS_LIGHTING, TRUE);

        ////Enable alpha blending so we can use transparent textures
        dev_obj->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        dev_obj->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        //dev_obj->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
        //dev_obj->SetRenderState(D3DRS_ALPHAREF, 200);

        //Set how the texture should be blended (use alpha)
        dev_obj->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        dev_obj->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        dev_obj->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(128, 128, 128));
        // end
        */


        lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
        opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, 1);
#if(defined OPT_CULLPRIMITIVE)
        opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);
#endif

        g_system = sys;
        g_sys_graphics = sys_graphics;

        if (ret_sys)
        {
            *ret_sys = sys;
            sys = 0;
        }

        if (ret_sys_graphics)
        {
            *ret_sys_graphics = sys_graphics;
            sys_graphics = 0;
        }


        ret = LW_RET_OK;
    }
__ret:
    LW_SAFE_RELEASE(sys);
    LW_SAFE_RELEASE(sys_graphics);

    return ret;
}

LW_RESULT lwReleaseMeshLibSystem()
{
    LW_ULONG ref_cnt = 0;
    lwISystem* sys = 0;
    lwISysGraphics* sys_graphics = 0;

    if((sys = lwGetActiveISystem()) == 0)
        goto __ret;

    if((sys_graphics = lwGetActiveIGraphicsSystem()) == 0)
        goto __ret;

    sys_graphics->Release();

    sys->Release();

    return LW_RET_OK;
__ret:
    assert(0 && "release mesh lib error");
    return LW_RET_FAILED;
}

void lwSetActiveISystem(lwISystem* sys)
{
    lwSystem::SetActiveISystem(sys);
}

lwISystem* lwGetActiveISystem()
{
    return lwSystem::__system;
}


LW_RESULT lwReleaseD3DObject(lwISystem* sys, lwISysGraphics* sys_graphics)
{
    sys->Release();
    sys_graphics->Release();

    return LW_RET_OK;
}

void lwHelperSetForceIgnoreTexFlag(DWORD flag)
{
    lwISystem* sys = lwGetActiveISystem();
    if(sys == NULL)
        return;

    lwIOptionMgr* opt_mgr;
    sys->GetInterface((LW_VOID**)&opt_mgr, LW_GUID_OPTIONMGR);
    opt_mgr->SetIgnoreModelTexFlag((BYTE)flag);
}

void lwUpdateSceneTransparentObject()
{
    lwISceneMgr* scene_mgr = g_sys_graphics->GetSceneMgr();

    //scene_mgr->Update();
    scene_mgr->SortTransparentPrimitive();
    scene_mgr->RenderTransparentPrimitive();
}

LW_RESULT LoadResBuf(lwIResourceMgr* res_mgr, const char* file)
{
    lwIResBufMgr* buf_mgr = res_mgr->GetResBufMgr();
    lwIPathInfo* path_info = 0;
    res_mgr->GetSysGraphics()->GetSystem()->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

	std::unique_ptr<FILE, decltype(&fclose)> fp( fopen(file, "rt"), &fclose );
    if (!fp)
    {
		return LW_RET_FAILED;
    }

    char path[LW_MAX_PATH];
    char model_file[LW_MAX_PATH];
    DWORD model_num;
    LW_HANDLE handle;
    lwModelObjInfo* model_info;
    lwGeomObjInfo* geom_info;
    lwTexInfo* tex_info;
    lwSysMemTexInfo smti;

    fscanf(fp.get(), "%d\n", &model_num);

    for(DWORD i = 0; i < model_num; i++)
    {
		fscanf(fp.get(), "%s\n", model_file);
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_MODEL_SCENE), model_file);

        if(LW_FAILED(buf_mgr->RegisterModelObjInfo(&handle, path)))
        {
            LG_MSGBOX("cannot find model file: %s", path);
            continue;

            //goto __ret;
        }

        if (LW_FAILED(buf_mgr->GetModelObjInfo((lwIModelObjInfo**)&model_info, handle)))
        {
            return LW_RET_FAILED;
        }

        for(DWORD j = 0; j < model_info->geom_obj_num; j++)
        {
            geom_info = model_info->geom_obj_seq[j];

            for(DWORD k = 0; k < geom_info->mtl_num; k++)
            {
                tex_info = &geom_info->mtl_seq[k].tex_seq[0];

                smti.colorkey = tex_info->colorkey.color;
                smti.level = tex_info->level;
                smti.format = tex_info->format == D3DFMT_UNKNOWN ? D3DFMT_A8R8G8B8 : tex_info->format;
                smti.filter = D3DX_DEFAULT;
                smti.mip_filter = D3DX_DEFAULT;
                sprintf(smti.file_name, "%s%s", path_info->GetPath(PATH_TYPE_TEXTURE_SCENE), tex_info->file_name);

                if(LW_FAILED(buf_mgr->RegisterSysMemTex(&handle, &smti)))
                {
                    LG_MSGBOX("cannot find texture file: %s in model file: path", smti.file_name, path);
                    continue;
                    //goto __ret;
                }
            }
                
        }
    }

	return  LW_RET_OK;
}

LW_RESULT LoadResModelBuf(lwIResourceMgr* res_mgr, const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResBufMgr* buf_mgr = res_mgr->GetResBufMgr();
    lwIPathInfo* path_info = 0;
    res_mgr->GetSysGraphics()->GetSystem()->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

    FILE* fp = fopen(file, "rt");
    if(fp == 0)
        goto __ret;

    char path[LW_MAX_PATH];
    char model_file[LW_MAX_PATH];
    DWORD model_num;
    LW_HANDLE handle;

    fscanf(fp, "%d\n", &model_num);

    for(DWORD i = 0; i < model_num; i++)
    {
        fscanf(fp, "%d\t%s\n", &handle, model_file);
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_MODEL_SCENE), model_file);

        if(LW_FAILED(buf_mgr->RegisterModelObjInfo(handle, path)))
        {
            LG_MSGBOX("cannot find model file: %s", path);
            continue;
        }
    }

    ret = LW_RET_OK;

__ret:
    if(fp)
    {
        fclose(fp);
    }
    
    return ret;
}

// dump function
LW_RESULT lwDumpMeshInfo(const char* file, const lwMeshInfo* info)
{
	FILE* fp = fopen(file, "wt");
    if(fp == 0)
        return LW_RET_FAILED;

    fprintf(fp, "Dump Mesh Information\n\n\n");

    int vertex_subset_flag = info->subset_num > 0 ? 1 : 0;
    int vertex_normal_flag = info->normal_seq ? 1 : 0;
    int texture_coordinate_flag = info->texcoord0_seq ? 1 : 0;
    int vertex_color_flag = info->vercol_seq ? 1 : 0;
    int vertex_blend_flag = info->blend_seq ? 1 : 0;
    int bone_index_flag = info->bone_index_num > 0 ? 1 : 0;
    int vertex_index_flag = info->index_seq ? 1 : 0;

    fprintf(fp, "VertexSubset Flag:        %d\n", vertex_subset_flag);
    fprintf(fp, "VertexNormal Flag:        %d\n", vertex_normal_flag);
    fprintf(fp, "VertexColor Flag:         %d\n", vertex_color_flag);
    fprintf(fp, "TextureCoordinate Flag:   %d\n", texture_coordinate_flag);
    fprintf(fp, "VertexIndex Flag:         %d\n", vertex_index_flag);
    fprintf(fp, "VertexBlend Flag:         %d\n", vertex_blend_flag);
    fprintf(fp, "BoneIndex Flag:           %d\n", bone_index_flag);

    DWORD i;
    lwVector2* v2;
    lwVector3* v3;
    DWORD c, *n;
    const lwBlendInfo* b;
    const lwSubsetInfo* s;


    if(vertex_subset_flag)
    {
        fprintf(fp, "Begin VertexSubset %d\n", info->subset_num);

        for(i = 0; i < info->subset_num; i++)
        {            
            s = &info->subset_seq[i];
            fprintf(fp, "%5d\t min_index: %5d vertex_num: %5d start_index: %5d primitive_num: %5d\n", i, 
                s->min_index, s->vertex_num, s->start_index, s->primitive_num);
        }
        fprintf(fp, "End VertexSubset\n\n\n");        
    }

    fprintf(fp, "Begin Vertex %d\n", info->vertex_num);

    for(i = 0; i < info->vertex_num; i++)
    {
        v3 = &info->vertex_seq[i];
        fprintf(fp, "%5d\t %10.5f %10.5f %10.5f\n", i, v3->x, v3->y, v3->z);
    }

    fprintf(fp, "End Vertex\n\n\n");

    if(vertex_normal_flag)
    {
        fprintf(fp, "Begin VertexNormal %d\n", info->vertex_num);

        for(i = 0; i < info->vertex_num; i++)
        {
            v3 = &info->normal_seq[i];
            fprintf(fp, "%5d\t %10.5f %10.5f %10.5f\n", i, v3->x, v3->y, v3->z);
        }

        fprintf(fp, "End VertexNormal\n\n\n");
    }

    if(vertex_color_flag)
    {
        fprintf(fp, "Begin VertexColor %d\n", info->vertex_num);

        for(i = 0; i < info->vertex_num; i++)
        {
            c = info->vercol_seq[i];
            fprintf(fp, "%5d\t %03d %03d %03d %03d\n", i, 
                LW_COLOR_R(c), LW_COLOR_G(c), LW_COLOR_B(c), LW_COLOR_A(c));
        }

        fprintf(fp, "End VertexColor\n\n\n");        
    }

    if(texture_coordinate_flag)
    {
        fprintf(fp, "Begin TextureCoordinate %d\n", info->vertex_num);

        for(i = 0; i < info->vertex_num; i++)
        {
            v2 = &info->texcoord0_seq[i];
            fprintf(fp, "%5d\t %10.5f %10.5f\n", i, v2->x, v2->y);
        }

        fprintf(fp, "End TextureCoordinate\n\n\n");
    }

    if(vertex_index_flag)
    {
        fprintf(fp, "Begin VertexIndex %d\n", info->index_num);

        for(i = 0; i < info->index_num / 3; i++)
        {
            n = &info->index_seq[i * 3];
            fprintf(fp, "%5d\t %5d %5d %5d\n", i, n[0], n[1], n[2]);
        }
        fprintf(fp, "End VertexIndex\n\n\n");        
    }
    
    
    if(vertex_blend_flag)
    {
        fprintf(fp, "Begin VertexBlend %d\n", info->vertex_num);

        for(i = 0; i < info->vertex_num; i++)
        {
            b = &info->blend_seq[i];
            fprintf(fp, "%5d\t %03d %03d %03d %03d %10.5f %10.5f %10.5f %10.5f\n", i,
                b->index[0], b->index[1], b->index[2], b->index[3], 
                b->weight[0], b->weight[1], b->weight[2], b->weight[3]);
        }
        fprintf(fp, "End VertexBlend\n\n\n");        
    }

    if(bone_index_flag)
    {
        fprintf(fp, "Begin BoneIndex %d\n", info->bone_index_num);

        for(i = 0; i < info->bone_index_num; i++)
        {
            fprintf(fp, "%5d\t %03d\n", i, info->bone_index_seq[i]);
        }
        fprintf(fp, "End BoneIndex\n\n\n");        
    }
    
    
    if(fp)
    {
        fclose(fp);
    }

    return LW_RET_OK;
}


LW_END
