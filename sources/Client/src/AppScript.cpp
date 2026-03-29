#include "stdafx.h"
#include "script.h"
#include "scene.h"
#include "GameApp.h"
#include "uiformmgr.h"
#include "PacketCmd.h"
#include "cameractrl.h"
#include "UISystemForm.h"
#include <LuaBridge.h>


void CGameApp::LoadScriptScene( eSceneType eType )
{
	switch( eType )
	{
	    case enumLoginScene:        LoadScriptScene("scripts/lua/scene/loginscene.lua");		break;
	    case enumSelectChaScene:    LoadScriptScene("scripts/lua/scene/selectchascene.lua");    break;
	    case enumCreateChaScene:    LoadScriptScene("scripts/lua/scene/createchascene.lua");    break;
		case enumWorldScene:        LoadScriptScene("scripts/lua/scene/mainscene.lua");			break;

	}
}

void CGameApp::LoadScriptScene( const char* script_file )
{
	LoadLuaScript(g_LuaState, script_file);
}

//---------------------------------------------------------------------------
// App_Script
//---------------------------------------------------------------------------
int GP_SetCameraPos( float ex, float ey, float ez, float rx, float ry, float rz )
{
	CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	pCam->m_EyePos.x = ex;
	pCam->m_EyePos.y = ey;
	pCam->m_EyePos.z = ez;
	pCam->m_RefPos.x = rx;
	pCam->m_RefPos.y = ry;
	pCam->m_RefPos.z = rz;

	return R_OK;
}

int GP_GotoScene( int sceneid )
{
    CGameScene * p = dynamic_cast<CGameScene*>(CScript::GetScriptObj(sceneid));
    if( !p ) return R_FAIL;

    g_pGameApp->GotoScene( p );
    return R_OK;
}

//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_App(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(GP_SetCameraPos)
		LUABRIDGE_REGISTER_FUNC(GP_GotoScene);
}
