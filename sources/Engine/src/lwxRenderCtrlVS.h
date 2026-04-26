#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwDirectX.h"
#include "lwITypes.h"
#include "lwInterface.h"

LW_BEGIN

// define base const register
//; c1-c4   = projection matrix
//; c5      = light direction
//; c6      = ambient color
//; c7      = diffuse * light.diffuse
//; c8-c11  = oT0 uv matrix
//; c12-c15 = oT1 uv matrix
//; c21     = world-view matrix palette

enum
{
    VS_CONST_REG_BASE =         0,
    VS_CONST_REG_VIEWPROJ =     1,
    VS_CONST_REG_LIGHT_DIR =    5,
    VS_CONST_REG_LIGHT_AMB =    6,
    VS_CONST_REG_LIGHT_DIF =    7,
    VS_CONST_REG_TS0_UVMAT =    8,
    VS_CONST_REG_TS1_UVMAT =    12,
    VS_CONST_REG_TS2_UVMAT =    16,
    VS_CONST_REG_MAT_PALETTE =  21,
};

LW_RESULT lwInitUserRenderCtrlVSProc(lwIResourceMgr* mgr);

class lwxRenderCtrlVSVertexBlend_dx8 : public lwIRenderCtrlVS
{
    typedef lwxRenderCtrlVSVertexBlend_dx8 this_type;

    LW_STD_DECLARATION();

private:
    D3DLIGHTX _lgt;
    lwColorValue4f _rs_amb;
    DWORD _rs_lgt;
    DWORD _rs_fog;
    BOOL _lgt_enable;

public:
	lwxRenderCtrlVSVertexBlend_dx8();
    DWORD GetType() { return RENDERCTRL_VS_VERTEXBLEND; }
    LW_RESULT Clone(lwIRenderCtrlVS** obj);
    LW_RESULT Initialize(lwIRenderCtrlAgent* agent);
    LW_RESULT BeginSet(lwIRenderCtrlAgent* agent);
    LW_RESULT EndSet(lwIRenderCtrlAgent* agent);
    LW_RESULT BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);
    LW_RESULT EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);


private:
	// added by clp
	std::string mPixelShaderName;
	bool mPixelShaderReload;
	DWORD mPixelShader;
public:
	void setPixelShader( const std::string& filename )
	{
		mPixelShaderName = filename;
		mPixelShaderReload = true;
	}

};


class lwxRenderCtrlVSVertexBlend : public lwIRenderCtrlVS
{
    typedef lwxRenderCtrlVSVertexBlend this_type;

    LW_STD_DECLARATION();

private:
    D3DLIGHTX _lgt;
    lwColorValue4f _rs_amb;
    ID3DXConstantTable* _const_tab;
    DWORD _rs_lgt;
    DWORD _rs_fog;
    BOOL _lgt_enable;

public:
    DWORD GetType() { return RENDERCTRL_VS_VERTEXBLEND; }
    LW_RESULT Clone(lwIRenderCtrlVS** obj);
    LW_RESULT Initialize(lwIRenderCtrlAgent* agent);
    LW_RESULT BeginSet(lwIRenderCtrlAgent* agent);
    LW_RESULT EndSet(lwIRenderCtrlAgent* agent);
    LW_RESULT BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);
    LW_RESULT EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);
};


LW_END
