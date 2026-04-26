//
#include "stdafx.h"
#include "lwGuidObj.h"
#include "lwIFunc.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwShaderMgr.h"
#include "lwGraphicsUtil.h"
#include "lwShaderTypes.h"
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
            if(LW_RESULT r = (*g_lose_dev_seq[i])(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                             "[{}] lose_dev proc[{}] failed: ret={}",
                             __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
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
            if(LW_RESULT r = (*g_reset_dev_seq[i])(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                             "[{}] reset_dev proc[{}] failed: ret={}",
                             __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
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

    if(LW_RESULT r = sys->Initialize(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] sys->Initialize failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    {
        // begin init path info
        lwIPathInfo* path_info = 0;

        if (LW_RESULT r = sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] GetInterface(LW_GUID_PATHINFO) failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");

        // end

        if (LW_RESULT r = sys->CreateGraphicsSystem(&sys_graphics); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateGraphicsSystem failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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

    if(LW_RESULT r = sys->Initialize(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] sys->Initialize failed (d3d/dev overload): ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

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

        if (LW_RESULT r = sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] GetInterface(LW_GUID_PATHINFO) failed (d3d/dev overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");
        // end

        if (LW_RESULT r = sys->CreateGraphicsSystem(&sys_graphics); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateGraphicsSystem failed (d3d/dev overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

        // set golbal lose-reset dev call back
        sys_graphics->SetOutputLoseDeviceProc(__global_lose_dev_entry);
        sys_graphics->SetOutputResetDeviceProc(__global_reset_dev_entry);

        dev_obj = sys_graphics->GetDeviceObject();
        dev_obj->SetDirect3D(d3d);
        dev_obj->SetDevice(dev);

        if (LW_RESULT r = dev_obj->InitStateCache(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] InitStateCache failed (d3d/dev overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

        if (LW_RESULT r = dev_obj->InitCapsInfo(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] InitCapsInfo failed (d3d/dev overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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
        opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);

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

    if(LW_RESULT r = sys->Initialize(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] sys->Initialize failed (param overload): ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    // check current directx version
    {
        DWORD ver = sys->GetSystemInfo()->GetDirectXVersion();
        if(ver < DX_VERSION_8_1)
        {
            ret = INIT_ERR_DX_VERSION;
            goto __ret;
        }
    }

    if(LW_RESULT r = sys->CreateGraphicsSystem(&sys_graphics); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateGraphicsSystem failed (param overload): ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    // set golbal lose-reset dev call back
    sys_graphics->SetOutputLoseDeviceProc(__global_lose_dev_entry);
    sys_graphics->SetOutputResetDeviceProc(__global_reset_dev_entry);

    {
        // begin init path info
        lwIPathInfo* path_info = 0;

        if (LW_RESULT r = sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] GetInterface(LW_GUID_PATHINFO) failed (param overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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
        path_info->SetPath(PATH_TYPE_SHADER, ".\\shader\\dx9\\");
        // end


        dev_obj = sys_graphics->GetDeviceObject();

        if (LW_RESULT r = dev_obj->CreateDirect3D(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateDirect3D failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            ret = INIT_ERR_CREATE_D3D;
            goto __ret;
        }

        if (param_info)
        {
            if (LW_RESULT r = lwAdjustD3DCreateParam(dev_obj->GetDirect3D(), param, param_info); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                             "[{}] lwAdjustD3DCreateParam failed: ret={}",
                             __FUNCTION__, static_cast<long long>(r));
                goto __ret;
            }
        }

        if (LW_RESULT r = dev_obj->CreateDevice(param); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateDevice failed: adapter={}, dev_type={}, windowed={}, ret={}",
                         __FUNCTION__,
                         param ? param->adapter : 0u,
                         param ? static_cast<std::uint32_t>(param->dev_type) : 0u,
                         param ? static_cast<int>(param->present_param.Windowed) : -1,
                         static_cast<long long>(r));
            ret = INIT_ERR_CREATE_DEVICE;
            goto __ret;
        }

        if (LW_RESULT r = dev_obj->InitStateCache(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] InitStateCache failed (param overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

        if (LW_RESULT r = dev_obj->InitCapsInfo(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] InitCapsInfo failed (param overload): ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

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

        lwInitUserRenderCtrlVSProc(res_mgr);

        lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
        opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, 1);
        opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);

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

    scene_mgr->SortTransparentPrimitive();
    scene_mgr->RenderTransparentPrimitive();
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

        if(LW_RESULT r = buf_mgr->RegisterModelObjInfo(handle, path); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] RegisterModelObjInfo failed: handle={}, path={}, ret={}",
                         __FUNCTION__, handle, path, static_cast<long long>(r));
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

LW_END
