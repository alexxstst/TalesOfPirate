//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwClassDecl.h"
#include "lwInterface.h"
#include "lwITypes.h"

#include "lwFrontAPI.h"


LW_BEGIN

enum lwInitMeshLibErrorType
{
    INIT_ERR_CREATE_D3D =           -1000,
    INIT_ERR_CREATE_DEVICE =        -1001,
    INIT_ERR_DX_VERSION =           -1002,
};

LW_FRONT_API void lwSetActiveIGraphicsSystem( lwISysGraphics* sys_graphics );
LW_FRONT_API lwISysGraphics* lwGetActiveIGraphicsSystem();

LW_FRONT_API LW_RESULT lwAdjustD3DCreateParam(IDirect3DX* d3d, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* adjust_info);
LW_FRONT_API LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics);
LW_FRONT_API LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, IDirect3DX* d3d, IDirect3DDeviceX* dev, HWND hwnd );
LW_FRONT_API LW_RESULT lwInitMeshLibSystem( lwISystem** ret_sys, lwISysGraphics** ret_sys_graphics, lwD3DCreateParam* param, lwD3DCreateParamAdjustInfo* param_info);
LW_FRONT_API LW_RESULT lwReleaseMeshLibSystem();

LW_FRONT_API void lwSetActiveISystem( lwISystem* sys );
LW_FRONT_API lwISystem* lwGetActiveISystem();

LW_FRONT_API LW_RESULT lwReleaseD3DObject( lwISystem* sys, lwISysGraphics* sys_graphics );

LW_FRONT_API LW_RESULT lwHitTestBox( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwBox* box, const lwMatrix44* mat );
LW_FRONT_API void lwHelperSetForceIgnoreTexFlag( DWORD flag );

LW_FRONT_API void lwWorldToScreen( int* x, int* y, float* z, const lwVector3* vec, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view );
LW_FRONT_API void lwUpdateSceneTransparentObject();

LW_FRONT_API LW_RESULT lwRegisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc);
LW_FRONT_API LW_RESULT lwRegisterOutputResetDeviceProc(lwOutputResetDeviceProc proc);
LW_FRONT_API LW_RESULT lwUnregisterOutputLoseDeviceProc(lwOutputLoseDeviceProc proc);
LW_FRONT_API LW_RESULT lwUnregisterOutputResetDeviceProc(lwOutputResetDeviceProc proc);

LW_FRONT_API LW_RESULT LoadResBuf(lwIResourceMgr* res_mgr, const char* file);
LW_FRONT_API LW_RESULT LoadResModelBuf(lwIResourceMgr* res_mgr, const char* file);

struct lwInterfaceMgr
{
    lwISystem* sys;
    lwISysGraphics* sys_graphics;
    lwIDeviceObject* dev_obj;
    lwIResourceMgr* res_mgr;
    lwIThreadPool* tp_loadres;
};

LW_FRONT_API LW_RESULT lwDumpMeshInfo(const char* file, const lwMeshInfo* info);

LW_END
