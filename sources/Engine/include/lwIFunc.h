//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwClassDecl.h"
#include "lwInterface.h"
#include "lwITypes.h"



LW_BEGIN

enum lwInitMeshLibErrorType
{
    INIT_ERR_CREATE_D3D =           -1000,
    INIT_ERR_CREATE_DEVICE =        -1001,
    INIT_ERR_DX_VERSION =           -1002,
};

void lwSetActiveIGraphicsSystem( lwISysGraphics* sys_graphics );
lwISysGraphics* lwGetActiveIGraphicsSystem();

LW_RESULT lwAdjustD3DCreateParam(IDirect3DX* d3d, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* adjust_info);
LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics);
LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, IDirect3DX* d3d, IDirect3DDeviceX* dev, HWND hwnd );
LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* param_info);
LW_RESULT lwReleaseMeshLibSystem();

void lwSetActiveISystem( lwISystem* sys );
lwISystem* lwGetActiveISystem();

LW_RESULT lwReleaseD3DObject( lwISystem* sys, lwISysGraphics* sys_graphics );

LW_RESULT lwHitTestBox( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwBox* box, const lwMatrix44* mat );
void lwHelperSetForceIgnoreTexFlag( DWORD flag );

void lwWorldToScreen( int* x, int* y, float* z, const lwVector3* vec, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view );
void lwUpdateSceneTransparentObject();

LW_RESULT lwRegisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc);
LW_RESULT lwRegisterOutputResetDeviceProc(lwOutputResetDeviceProc proc);
LW_RESULT lwUnregisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc);
LW_RESULT lwUnregisterOutputResetDeviceProc(lwOutputResetDeviceProc proc);

LW_RESULT LoadResModelBuf(lwIResourceMgr* res_mgr, const char* file);

struct lwInterfaceMgr
{
    lwISystem* sys;
    lwISysGraphics* sys_graphics;
    lwIDeviceObject* dev_obj;
    lwIResourceMgr* res_mgr;
    lwIThreadPool* tp_loadres;
};

LW_END
