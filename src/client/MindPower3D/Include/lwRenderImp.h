#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwClassDecl.h"
#include "lwErrorCode.h"
#include "lwDirectX.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"
#include "lwShaderMgr.h"

LW_BEGIN

// remarks
// 1shader
//    declshader
// 2shader
//    decl
//    lwShaderDeclMgr
// 3Shaderuser-defined shader


class lwRenderCtrlAgent : public lwIRenderCtrlAgent
{
LW_STD_DECLARATION()

private:
    lwMatrix44 _mat_local;
    lwMatrix44 _mat_parent;
    lwMatrix44 _mat_global;

    lwIResourceMgr* _res_mgr;
    lwIMeshAgent* _mesh_agent;
    lwIMtlTexAgent* _mtltex_agent;
    lwIAnimCtrlAgent* _anim_agent;
    lwIRenderCtrlVS* _render_ctrl;

    DWORD _decl_type;
    DWORD _vs_type;
    DWORD _ps_type;


public:
	void setPixelShader( const std::string& filename )
	{
		_render_ctrl->setPixelShader( filename );
	}

public:
    lwRenderCtrlAgent(lwIResourceMgr* res_mgr);
    ~lwRenderCtrlAgent();

    lwIResourceMgr* GetResourceMgr()
	{
		return _res_mgr;
	}

    lwIMeshAgent* GetMeshAgent()
	{
		return _mesh_agent;
	}

    lwIMtlTexAgent* GetMtlTexAgent()
	{
		return _mtltex_agent;
	}

    lwIAnimCtrlAgent* GetAnimCtrlAgent()
	{
		return _anim_agent;
	}

    lwIRenderCtrlVS* GetRenderCtrlVS()
	{
		return _render_ctrl;
	}

    void SetMatrix(const lwMatrix44* mat)
	{
		_mat_global = *mat;
	}

    void SetLocalMatrix( const lwMatrix44* mat_local );
    void SetParentMatrix( const lwMatrix44* mat_parent );
    lwMatrix44* GetLocalMatrix()
	{
		return &_mat_local;
	}

    lwMatrix44* GetParentMatrix()
	{
		return &_mat_parent;
	}

    lwMatrix44* GetGlobalMatrix()
	{
		return &_mat_global;
	}

    void BindMeshAgent(lwIMeshAgent* agent)
	{
		_mesh_agent = agent;
	}

    void BindMtlTexAgent(lwIMtlTexAgent* agent)
	{
		_mtltex_agent = agent;
	}

    void BindAnimCtrlAgent(lwIAnimCtrlAgent* agent)
	{
		_anim_agent = agent;
	}

    LW_RESULT SetRenderCtrl(DWORD ctrl_type);
    void SetRenderCtrl(lwIRenderCtrlVS* ctrl)
	{
		_render_ctrl = ctrl;
	}

    void SetVertexShader(DWORD type)
	{
		_vs_type = type;
	}

    void SetVertexDeclaration(DWORD type)
	{
		_decl_type = type;
	}

    DWORD GetVertexShader() const
	{
		return _vs_type;
	}
    DWORD GetVertexDeclaration() const
	{
		return _decl_type;
	}


    virtual LW_RESULT Clone(lwIRenderCtrlAgent** ret_obj);
    virtual LW_RESULT BeginSet();
    virtual LW_RESULT EndSet();
    virtual LW_RESULT BeginSetSubset(DWORD subset);
    virtual LW_RESULT EndSetSubset(DWORD subset);
    virtual LW_RESULT DrawSubset(DWORD subset); 
};



LW_END
