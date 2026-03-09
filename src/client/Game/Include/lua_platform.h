#pragma once

#ifdef _LUA_GAME

#include "lua.hpp"
#include "LuaBridge.h"

// lua
#define LUA_FALSE			0 // 
#define LUA_TRUE			1 // 

// 
#define PARAM_ERROR        { ToLogService("lua", "{}, {}", g_oLangRec.GetString(183),__FUNCTION__ ); }
#define SCENE_NULL_ERROR   { ToLogService("lua", "{}, {}", g_oLangRec.GetString(184), __FUNCTION__);         }

void print_error(lua_State* state);

#undef luaL_dofile
#undef luaL_dostring

#define luaL_dofile(L, fn) \
	if ((luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0) { \
		print_error(L);                                                   \
	}


#define luaL_dostring(L, s) \
	if ((luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0) { \
		print_error(L); \
	}

extern void InitLuaPlatform();
extern void CreateScriptDebugWindow(HINSTANCE hInst, HWND hParent);
extern void ToggleScriptDebugWindow();

extern lua_State *L;

inline void lua_dostringI(const char *pszCmd)
{
    luaL_dostring(L, pszCmd);
    //luaL_loadbuffer(L, pszCmd, nCmdSize, "line");
    //lua_pcall(L, 0, 0, 0);
}

inline void lua_dostringI(std::string str)
{
   luaL_dostring(L, str.c_str());//  (int)str.size());
}

inline void lua_platform_framemove()
{
    // lua_dostring(L, "appRunTimer()");
}

inline void lua_platform_keydown(DWORD dwKey)
{
    //char szRun[128]; sprintf(szRun, "if key_event[%d] then key_event[%d]() end", dwKey, dwKey);
    //lua_dostring(L, szRun);
}

inline void lua_platform_mousedown(DWORD dwButton)
{
    //char szRun[128]; sprintf(szRun, "if mouse_event[%d] then mouse_event[%d]() end", dwButton, dwButton);
    //lua_dostring(L, szRun);
}

#include "GameApp.h"
#include "Scene.h"
#include "worldscene.h"
#include "SceneItem.h"
#include "Character.h"
#include "Actor.h"
#include "uicozeform.h"
#include "UIFormMgr.h"
#include "cameractrl.h"

#include "lua_app.h"
#include "lua_scene.h"
#include "lua_object.h"
#include "lua_input.h"
#include "lua_ui.h"
#include "lua_network.h"
#include "lua_camera.h"
#include "lua_util.h"

#endif
