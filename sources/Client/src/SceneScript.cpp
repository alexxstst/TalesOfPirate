#include "stdafx.h"
#include "script.h"
#include "scene.h"
#include "GameApp.h"
#include "ChaClientAttr.h"
#include "WorldScene.h"
#include <LuaBridge.h>


SClientAttr g_ClientAttr[2000];

SCameraMode CameraMode[4];

//---------------------------------------------------------------------------
// Scene_Script
//---------------------------------------------------------------------------
int SN_CreateScene( int type, const std::string& name, const std::string& map_name, int ui, int max_cha, int max_obj, int max_item, int max_eff )
{
	if( type>=0 && type<enumSceneEnd )
	{
		stSceneInitParam param;
		param.nTypeID = type;
		param.strName = name;
		param.strMapFile = map_name;
		param.nUITemplete = ui;
		param.nMaxCha = max_cha;
		param.nMaxObj = max_obj;
		param.nMaxItem = max_item;
		param.nMaxEff = max_eff;

		CGameScene* s = g_pGameApp->CreateScene( &param );

		if( s ) return s->GetScriptID();
	}
	return R_FAIL;
}

int SN_SetIsShow3DCursor( BOOL isShow )
{
	g_pGameApp->GetCursor()->SetIsVisible( isShow==TRUE );
	return R_OK;
}

int SN_SetTerrainShowCenter( int sceneid, int x, int y )
{
	CGameScene * p =  dynamic_cast<CGameScene*>(CScript::GetScriptObj(sceneid));
	if( !p ) return R_FAIL;

    MPTerrain *t = p->GetTerrain();
	if( t )
	{
		t->SetShowCenter( (float)x, (float)y );
		return R_OK;
	}

	return R_FAIL;
}

int SN_SetIsShowMinimap( int isShow )
{
	CGameScene::ShowMinimap( isShow );
	return R_OK;
}

int SN_SetAttackChaColor( int r, int g, int b )
{
	CWorldScene::SetAttackChaColor( r, g, b );
	return R_OK;
}

int CHA_SetClientAttr(int nScriptID, int nAngle, float fDis, float fHei)
{
	SClientAttr *pAttr = &g_ClientAttr[nScriptID];
	pAttr->sTeamAngle  = (short)nAngle;
	pAttr->fTeamDis    = fDis;
	pAttr->fTeamHei    = fHei;
	return 0;
}

int CameraRangeXY(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminxy = fMin;
	CameraMode[nMode].m_fmaxxy = fMax;
	return 0;
}

int CameraRangeZ(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminHei = fMin;
	CameraMode[nMode].m_fmaxHei = fMax;
	return 0;
}

int CameraRangeFOV(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminfov = fMin;
	CameraMode[nMode].m_fmaxfov = fMax;
	return 0;
}

int CameraEnableRotate(int nMode, int nEnable)
{
	CameraMode[nMode].m_bRotate = nEnable;
	return 0;
}

int CameraShowSize(int nMode, int w, int h)
{
	CameraMode[nMode].m_nShowWidth  = w;
	CameraMode[nMode].m_nShowHeight = h;
	return 0;
}

int CameraShowSize1024(int nMode, int w, int h)
{
	CameraMode[nMode].m_nShowWidth  = w;
	CameraMode[nMode].m_nShowHeight = h;
	return 0;
}

//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_Scene(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(SN_CreateScene)
		LUABRIDGE_REGISTER_FUNC(SN_SetTerrainShowCenter)
		LUABRIDGE_REGISTER_FUNC(SN_SetIsShow3DCursor)
		LUABRIDGE_REGISTER_FUNC(SN_SetIsShowMinimap)
		LUABRIDGE_REGISTER_FUNC(SN_SetAttackChaColor)
		LUABRIDGE_REGISTER_FUNC(CHA_SetClientAttr)
		LUABRIDGE_REGISTER_FUNC(CameraRangeXY)
		LUABRIDGE_REGISTER_FUNC(CameraRangeZ)
		LUABRIDGE_REGISTER_FUNC(CameraRangeFOV)
		LUABRIDGE_REGISTER_FUNC(CameraEnableRotate)
		LUABRIDGE_REGISTER_FUNC(CameraShowSize);
}
