//
#include "stdafx.h"


#include "lwRenderImp.h"
#include "lwDeviceObject.h"
#include "lwResourceMgr.h"
#include "lwShaderMgr.h"
#include "lwD3D.h"

LW_BEGIN

//
////////////////////////////
// lwRenderCtrlAgent
LW_STD_IMPLEMENTATION(lwRenderCtrlAgent)

lwRenderCtrlAgent::lwRenderCtrlAgent(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _mesh_agent(0), _mtltex_agent(0), _anim_agent(0),
  _render_ctrl(0), _decl_type(VDT_INVALID), _vs_type(VST_INVALID)
{
    lwMatrix44Identity(&_mat_local);
    lwMatrix44Identity(&_mat_parent);
    lwMatrix44Identity(&_mat_global);
}
lwRenderCtrlAgent::~lwRenderCtrlAgent()
{
    LW_IF_RELEASE(_render_ctrl);
}


void lwRenderCtrlAgent::SetLocalMatrix(const lwMatrix44* mat_local)
{
    _mat_local = *mat_local;
    lwMatrix44Multiply(&_mat_global, &_mat_local, &_mat_parent);
}

void lwRenderCtrlAgent::SetParentMatrix(const lwMatrix44* mat_parent)
{
    _mat_parent = *mat_parent;
    lwMatrix44Multiply(&_mat_global, &_mat_local, &_mat_parent);
}
LW_RESULT lwRenderCtrlAgent::SetRenderCtrl(DWORD ctrl_type)
{
    LW_RESULT ret = LW_RET_FAILED;

    LW_IF_RELEASE(_render_ctrl);

    if(LW_RESULT r = _res_mgr->CreateRenderCtrlVS(&_render_ctrl, ctrl_type); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _res_mgr->CreateRenderCtrlVS failed: ctrl_type={}, ret={}",
                     __FUNCTION__, ctrl_type, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _render_ctrl->Initialize(this); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _render_ctrl->Initialize failed: ctrl_type={}, ret={}",
                     __FUNCTION__, ctrl_type, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwRenderCtrlAgent::Clone(lwIRenderCtrlAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrlAgent* o;

    if(LW_RESULT r = _res_mgr->CreateRenderCtrlAgent(&o); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _res_mgr->CreateRenderCtrlAgent failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    o->SetRenderCtrl(_render_ctrl->GetType());

    o->SetLocalMatrix(&_mat_local);
    o->SetParentMatrix(&_mat_parent);
    o->SetVertexDeclaration(_decl_type);
    o->SetVertexShader(_vs_type);

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:   
    return ret;
}
LW_RESULT lwRenderCtrlAgent::BeginSet()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_mesh_agent == NULL || _render_ctrl == NULL)
        goto __ret;

    if(LW_RESULT r = _mesh_agent->BeginSet(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _mesh_agent->BeginSet failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _render_ctrl->BeginSet(this); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _render_ctrl->BeginSet failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwRenderCtrlAgent::EndSet()
{
     LW_RESULT ret = LW_RET_FAILED;

     if(_mesh_agent == NULL || _render_ctrl == NULL)
         goto __ret;

     if(LW_RESULT r = _mesh_agent->EndSet(); LW_FAILED(r))
     {
         ToLogService("errors", LogLevel::Error,
                      "[{}] _mesh_agent->EndSet failed: ret={}",
                      __FUNCTION__, static_cast<long long>(r));
         goto __ret;
     }

     if(LW_RESULT r = _render_ctrl->EndSet(this); LW_FAILED(r))
     {
         ToLogService("errors", LogLevel::Error,
                      "[{}] _render_ctrl->EndSet failed: ret={}",
                      __FUNCTION__, static_cast<long long>(r));
         goto __ret;
     }

     ret = LW_RET_OK;
__ret:
     return ret;
}
LW_RESULT lwRenderCtrlAgent::BeginSetSubset(DWORD subset)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_mtltex_agent == 0 || _render_ctrl == 0)
        goto __ret;

    if(LW_RESULT r = _mtltex_agent->BeginSet(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _mtltex_agent->BeginSet failed: subset={}, ret={}",
                     __FUNCTION__, subset, static_cast<long long>(r));
        goto __ret;
    }


    if(LW_RESULT r = _render_ctrl->BeginSetSubset(subset, this); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _render_ctrl->BeginSetSubset failed: subset={}, ret={}",
                     __FUNCTION__, subset, static_cast<long long>(r));
        goto __ret;
    }


    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwRenderCtrlAgent::EndSetSubset(DWORD subset)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_mtltex_agent == 0 || _render_ctrl == 0)
        goto __ret;

    if(LW_RESULT r = _mtltex_agent->EndSet(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _mtltex_agent->EndSet failed: subset={}, ret={}",
                     __FUNCTION__, subset, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _render_ctrl->EndSetSubset(subset, this); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _render_ctrl->EndSetSubset failed: subset={}, ret={}",
                     __FUNCTION__, subset, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwRenderCtrlAgent::DrawSubset(DWORD subset)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_mesh_agent == NULL)
        goto __ret;

    if(LW_RESULT r = _mesh_agent->DrawSubset(subset); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _mesh_agent->DrawSubset failed: subset={}, ret={}",
                     __FUNCTION__, subset, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}



LW_END
