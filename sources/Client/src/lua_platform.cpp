#include "stdafx.h"
#include "GameConfig.h"

#ifdef _LUA_GAME


#include "resource.h"
#include "lua_platform.h"

using namespace std;

lua_State *L     = NULL;

int LuaPanicHandler(lua_State* L);

void InitLuaPlatform()
{
    g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(182));

	g_LuaState = luaL_newstate();
	if (!g_LuaState)
	{
		ToLogService("lua", LogLevel::Error, "Failed to create Lua VM (luaL_newstate)");
		return;
	}

	lua_atpanic(g_LuaState, LuaPanicHandler);
	luaL_openlibs(g_LuaState);

	ToLogService("lua", "LuaJIT VM initialized");

    //   VM  Script.cpp
    L = g_LuaState;

    //    LuaBridge 
    luabridge::getGlobalNamespace(L)
        LUABRIDGE_REGISTER_FUNC(MsgBox)
        .addFunction("GetTickCount", GetTickCount_Lua)
        .addFunction("Rand", Rand_Lua)
        // app
        LUABRIDGE_REGISTER_FUNC(appSetCaption)
        LUABRIDGE_REGISTER_FUNC(appPlaySound)
        LUABRIDGE_REGISTER_FUNC(appUpdateRender)
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
        ;

    // lua_CFunction  lightuserdata, -, arg
    // util
    LUA_REGISTER_CFUNC(L, LG);
    LUA_REGISTER_CFUNC(L, SysInfo);
    // app
    LUA_REGISTER_CFUNC(L, appGetCurScene);
    LUA_REGISTER_CFUNC(L, appSetCurScene);
    LUA_REGISTER_CFUNC(L, appCreateScene);
    // scene
    LUA_REGISTER_CFUNC(L, sceAddObj);
    LUA_REGISTER_CFUNC(L, sceRemoveObj);
    LUA_REGISTER_CFUNC(L, sceGetObj);
    LUA_REGISTER_CFUNC(L, sceSetMainCha);
    LUA_REGISTER_CFUNC(L, sceGetMainCha);
    LUA_REGISTER_CFUNC(L, sceGetHoverCha);
    LUA_REGISTER_CFUNC(L, sceEnableDefaultMouse);
    // object
    LUA_REGISTER_CFUNC(L, objSetPos);
    LUA_REGISTER_CFUNC(L, objGetPos);
    LUA_REGISTER_CFUNC(L, objSetFaceAngle);
    LUA_REGISTER_CFUNC(L, objGetFaceAngle);
    LUA_REGISTER_CFUNC(L, objSetAttr);
    LUA_REGISTER_CFUNC(L, objGetAttr);
    LUA_REGISTER_CFUNC(L, objIsValid);
    LUA_REGISTER_CFUNC(L, objGetID);
    LUA_REGISTER_CFUNC(L, chaMoveTo);
    LUA_REGISTER_CFUNC(L, chaSay);
    LUA_REGISTER_CFUNC(L, chaChangePart);
    LUA_REGISTER_CFUNC(L, chaPlayPose);
    LUA_REGISTER_CFUNC(L, chaStop);
    // camera
    LUA_REGISTER_CFUNC(L, camGetCenter);
    // network
    LUA_REGISTER_CFUNC(L, Connect);
    // UI
    LUA_REGISTER_CFUNC(L, uiGetForm);

}

void CreateScriptDebugWindow(HINSTANCE, HWND) {}
void ToggleScriptDebugWindow() {}

#endif
