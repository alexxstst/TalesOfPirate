//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwDirectX.h"
#include "lwITypes.h"
#include "lwViewFrustum.h"
#include "lwInterfaceExt.h"
LW_BEGIN

// remarks: by lsh
// 1scene manger
struct lwSortPriProcInfo
{
    lwTranspPrimitiveProc proc;
    void* param;
};
struct lwSortPriInfo
{
    float d;
    lwIPrimitive* obj;
    lwSortPriProcInfo sppi;
};


class lwSceneMgr : public lwISceneMgr
{
LW_STD_DECLARATION()

private:
    lwISysGraphics* _sys_graphics;
    lwIResourceMgr* _res_mgr;
    lwIViewFrustum* _vf;
    lwSortPriInfo* _sort_obj_seq;
    lwSortPriProcInfo _sppi;
    DWORD _sort_obj_seq_size;
    DWORD _sort_obj_num;
    DWORD _max_sort_num;

    
    lwCullPriInfo _cull_pri_info;

    lwSceneFrameInfo _frame_info;

public:
    lwSceneMgr(lwISysGraphics* sys_graphics);
    ~lwSceneMgr();

    LW_RESULT Update();
    LW_RESULT Render();
    LW_RESULT BeginRender();
    LW_RESULT EndRender();
    LW_RESULT RenderTransparentPrimitive();
    LW_RESULT AddTransparentPrimitive(lwIPrimitive* obj);
    LW_RESULT SortTransparentPrimitive();
    void SetTranspPrimitiveProc(lwTranspPrimitiveProc proc, void* param) { _sppi.proc = proc; _sppi.param = param; }
    LW_RESULT CullPrimitive(lwIPrimitive* obj);
    lwIViewFrustum* GetViewFrustum() { return _vf; }
    DWORD GetMaxSortNum() const { return _max_sort_num; }
    void GetCullPrimitiveInfo(lwCullPriInfo* info) { *info = _cull_pri_info; }
    lwSceneFrameInfo* GetSceneFrameInfo() { return &_frame_info; }
};

LW_END
