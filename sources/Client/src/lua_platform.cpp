#include "stdafx.h"
#include "GameConfig.h"

#ifdef _LUA_GAME


#include "resource.h"
#include "lua_platform.h"

using namespace std;

lua_State* L = NULL;

int LuaPanicHandler(lua_State* L);

void InitLuaPlatform() {
	g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(182));

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
		LUABRIDGE_REGISTER_FUNC(uiHideAll)
		// multi-return (std::tuple)
		LUABRIDGE_REGISTER_FUNC(objGetPos)
		LUABRIDGE_REGISTER_FUNC(camGetCenter);
}

void CreateScriptDebugWindow(HINSTANCE, HWND) {
}

void ToggleScriptDebugWindow() {
}

#endif
