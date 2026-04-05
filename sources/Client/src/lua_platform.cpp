#include "stdafx.h"
#include "GameConfig.h"

#ifdef _LUA_GAME


#include "resource.h"
#include "lua_platform.h"

using namespace std;

lua_State* L = NULL;

int LuaPanicHandler(lua_State* L);

void InitLuaPlatform() {
	g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(182).c_str());

	g_LuaState = luaL_newstate();
	if (!g_LuaState) {
		ToLogService("lua", LogLevel::Error, "Failed to create Lua VM (luaL_newstate)");
		return;
	}

	lua_atpanic(g_LuaState, LuaPanicHandler);
	luaL_openlibs(g_LuaState);

	ToLogService("lua", "LuaJIT VM initialized");

	//   VM  Script.cpp
	L = g_LuaState;

	//  LuaBridge
	luabridge::getGlobalNamespace(L)
		.beginClass<CGameScene>("CGameScene").endClass()
		.beginClass<CSceneNode>("CSceneNode").endClass()
		.beginClass<CCharacter>("CCharacter").endClass();

	//    LuaBridge
	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(MsgBox)
		LUABRIDGE_REGISTER_FUNC(ToDebugLog)
		.addFunction("GetTickCount", GetTickCount_Lua)
		.addFunction("Rand", Rand_Lua)
		// app
		LUABRIDGE_REGISTER_FUNC(appSetCaption)
		LUABRIDGE_REGISTER_FUNC(appPlaySound)
		LUABRIDGE_REGISTER_FUNC(appUpdateRender)
		LUABRIDGE_REGISTER_FUNC(appGetCurScene)
		LUABRIDGE_REGISTER_FUNC(appSetCurScene)
		LUABRIDGE_REGISTER_FUNC(appCreateScene)
		// scene
		LUABRIDGE_REGISTER_FUNC(sceAddObj)
		LUABRIDGE_REGISTER_FUNC(sceGetObj)
		LUABRIDGE_REGISTER_FUNC(sceRemoveObj)
		LUABRIDGE_REGISTER_FUNC(sceSetMainCha)
		LUABRIDGE_REGISTER_FUNC(sceGetMainCha)
		LUABRIDGE_REGISTER_FUNC(sceGetHoverCha)
		LUABRIDGE_REGISTER_FUNC(sceEnableDefaultMouse)
		// object
		LUABRIDGE_REGISTER_FUNC(objIsValid)
		LUABRIDGE_REGISTER_FUNC(objGetID)
		LUABRIDGE_REGISTER_FUNC(objSetPos)
		LUABRIDGE_REGISTER_FUNC(objGetFaceAngle)
		LUABRIDGE_REGISTER_FUNC(objSetFaceAngle)
		LUABRIDGE_REGISTER_FUNC(objGetAttr)
		LUABRIDGE_REGISTER_FUNC(objSetAttr)
		LUABRIDGE_REGISTER_FUNC(chaSay)
		LUABRIDGE_REGISTER_FUNC(chaMoveTo)
		LUABRIDGE_REGISTER_FUNC(chaStop)
		LUABRIDGE_REGISTER_FUNC(chaChangePart)
		LUABRIDGE_REGISTER_FUNC(chaPlayPose)
		// camera
		LUABRIDGE_REGISTER_FUNC(camSetCenter)
		LUABRIDGE_REGISTER_FUNC(camFollow)
		LUABRIDGE_REGISTER_FUNC(camMoveForward)
		LUABRIDGE_REGISTER_FUNC(camMoveLeft)
		LUABRIDGE_REGISTER_FUNC(camMoveUp)
		LUABRIDGE_REGISTER_FUNC(camSetAngle)
		// input
		LUABRIDGE_REGISTER_FUNC(IsKeyDown)
		// UI
		LUABRIDGE_REGISTER_FUNC(uiHideAll);

	// multi-return (lua_CFunction, значения в стек напрямую)
	lua_register(L, "objGetPos", objGetPos);
	lua_register(L, "camGetCenter", camGetCenter);
}

//-----------------
// util
//-----------------

void MsgBox(const std::string& content) {
	MessageBox(NULL, content.c_str(), "msg", 0);
}

int GetTickCount_Lua() {
	return (int)GetTickCount();
}

int Rand_Lua(int nRange) {
	return rand() % nRange;
}

void ToDebugLog(const std::string& message) {
	g_logManager.InternalLog(LogLevel::Debug, "lua", message);
}

//-----------------
// app
//-----------------

void appSetCaption(const std::string& caption) {
	g_pGameApp->SetCaption(caption.c_str());
}

void appPlaySound(int soundId) {
	g_pGameApp->PlaySound(soundId);
}

void appUpdateRender() {
	MPTerrain* pTerrain = g_pGameApp->GetCurScene()->GetTerrain();
	if (pTerrain) {
		pTerrain->UpdateRender();
	}
}

CGameScene* appGetCurScene() {
	return g_pGameApp->GetCurScene();
}

void appSetCurScene(CGameScene* pScene) {
	if (!pScene) {
		return;
	}
	g_pGameApp->GotoScene(pScene, false);
}

CGameScene* appCreateScene(const std::string& mapName, int maxCha, int maxObj, int maxItem, int maxEff) {
	CGameScene* pScene = NULL;
	return pScene;
}

//-----------------
// scene
//-----------------

CSceneNode* sceAddObj(CGameScene* pScene, int nType, int nScriptID) {
	if (!pScene) {
		return nullptr;
	}

	CSceneNode* pNode = NULL;
	switch (nType) {
	case 0: {
		pNode = pScene->AddCharacter(nScriptID);
		break;
	}
	case 1: {
		stNetItemCreate info{};
		info.lID = nScriptID;
		pNode = NetCreateItem(info);
		break;
	}
	case 2: {
		pNode = pScene->AddSceneEffect(nScriptID);
		break;
	}
	}
	return pNode;
}

CSceneNode* sceGetObj(CGameScene* pScene, int nType, int nArrayID) {
	if (!pScene) {
		return nullptr;
	}

	CSceneNode* pNode = NULL;
	switch (nType) {
	case 0: {
		pNode = pScene->GetCha(nArrayID);
		break;
	}
	case 1: {
		pNode = pScene->GetSceneItem(nArrayID);
		break;
	}
	}
	return pNode;
}

void sceRemoveObj(CSceneNode* pNode) {
	if (!pNode) {
		return;
	}
	pNode->SetValid(FALSE);
}

void sceSetMainCha(CGameScene* pScene, CCharacter* pCha) {
	if (!pScene) {
		return;
	}
	if (!pCha) {
		return;
	}
	pScene->SetMainCha(pCha->getID());
}

CCharacter* sceGetMainCha(CGameScene* pScene) {
	if (!pScene) {
		return nullptr;
	}
	return pScene->GetMainCha();
}

CCharacter* sceGetHoverCha(CGameScene* pScene) {
	if (!pScene) {
		return nullptr;
	}
	return pScene->GetSelectCha();
}

void sceEnableDefaultMouse(CGameScene* pScene, int bEnable) {
	if (!pScene) {
		return;
	}
	if (bEnable) {
		pScene->GetUseLevel().SetTrue(LEVEL_MOUSE_RUN);
	}
	else {
		pScene->GetUseLevel().SetFalse(LEVEL_MOUSE_RUN);
	}
}

//-----------------
// object
//-----------------

int objIsValid(CSceneNode* pNode) {
	if (!pNode) {
		return 0;
	}
	return pNode->IsValid();
}

int objGetID(CSceneNode* pNode) {
	if (!pNode) {
		return 0;
	}
	return pNode->getID();
}

void objSetPos(CSceneNode* pNode, int x, int y) {
	if (!pNode) {
		return;
	}
	pNode->setPos(x, y);
}

int objGetPos(lua_State* L) {
	auto pNode = luabridge::Stack<CSceneNode*>::get(L, 1);
	if (!pNode) {
		lua_pushnumber(L, 0.0);
		lua_pushnumber(L, 0.0);
		return 2;
	}


	CSceneNode* node = *pNode;
	lua_pushnumber(L, node->GetCurX());
	lua_pushnumber(L, node->GetCurY());
	return 2;
}

float objGetFaceAngle(CSceneNode* pNode) {
	if (!pNode) {
		return 0.0f;
	}
	return pNode->getYaw();
}

void objSetFaceAngle(CSceneNode* pNode, int angle) {
	if (!pNode) {
		return;
	}
	pNode->setYaw(angle);
}

int objGetAttr(CSceneNode* pNode, int sType) {
	if (!pNode) {
		return 0;
	}
	return pNode->getGameAttr()->get((short)sType);
}

void objSetAttr(CSceneNode* pNode, int sType, int lValue) {
	if (!pNode) {
		return;
	}
	pNode->getGameAttr()->set((short)sType, (long)lValue);
	pNode->RefreshUI();
}

//-----------------
// character
//-----------------

void chaSay(CCharacter* pCha, const std::string& text) {
	if (!pCha) {
		return;
	}
	CGameScene* pScene = g_pGameApp->GetCurScene();
	if (!pScene) {
		return;
	}
	CCozeForm::GetInstance()->OnSightMsg(pCha, text.c_str());
}

void chaMoveTo(CCharacter* pCha, int x, int y) {
	if (!pCha) {
		return;
	}
	CGameScene* pScene = g_pGameApp->GetCurScene();
	if (!pScene) {
		return;
	}
	CWorldScene* pWorld = dynamic_cast<CWorldScene*>(pScene);
	if (pWorld) {
		pWorld->GetMouseDown().ActMove(pCha, x, y);
	}
}

void chaStop(CCharacter* pCha) {
	if (!pCha) {
		return;
	}
	pCha->GetActor()->Stop();
}

int chaChangePart(CCharacter* pCha, int nTabID) {
	if (!pCha) {
		return 0;
	}
	return pCha->ChangePart(nTabID);
}

void chaPlayPose(CCharacter* pCha, int nPoseID, int bHold) {
	if (!pCha) {
		return;
	}
	pCha->GetActor()->PlayPose(nPoseID, bHold != 0);
}

//-----------------
// camera
//-----------------

void camSetCenter(float x, float y) {
	float ref_x = g_pGameApp->GetMainCam()->m_RefPos.x;
	float ref_y = g_pGameApp->GetMainCam()->m_RefPos.y;
	float dis_x = x - ref_x;
	float dis_y = y - ref_y;
	g_pGameApp->GetMainCam()->m_RefPos.x = x;
	g_pGameApp->GetMainCam()->m_RefPos.y = y;
	g_pGameApp->GetMainCam()->m_EyePos.x += dis_x;
	g_pGameApp->GetMainCam()->m_EyePos.y += dis_y;
}

void camFollow(int bEnable) {
	g_pGameApp->EnableCameraFollow((BOOL)bEnable);
}

void camMoveForward(float fStep, int bHang) {
	g_pGameApp->GetMainCam()->MoveForward(fStep, (BOOL)bHang);
}

void camMoveLeft(float fStep, int bHang) {
	g_pGameApp->GetMainCam()->MoveRight(fStep, (BOOL)bHang);
}

void camMoveUp(float fStep) {
	g_pGameApp->GetMainCam()->m_RefPos.z -= fStep;
}

void camSetAngle(float fAngle) {
	//g_pGameApp->GetMainCam()->InitAngle(fAngle);
}

int camGetCenter(lua_State* L) {
	auto* cam = g_pGameApp->GetMainCam();
	lua_pushnumber(L, cam->m_RefPos.x);
	lua_pushnumber(L, cam->m_RefPos.y);
	return 2;
}

//-----------------
// input
//-----------------

int IsKeyDown(int key) {
	BYTE btKey = (BYTE)key;
	if (g_pGameApp->IsKeyContinue(btKey)) {
		return 1;
	}
	return 0;
}

//-----------------
// UI
//-----------------

void uiHideAll() {
	CFormMgr::s_Mgr.SetEnabled(FALSE);
}

//-----------------

void CreateScriptDebugWindow(HINSTANCE, HWND) {
}

void ToggleScriptDebugWindow() {
}

#endif
