#ifdef _LUA_GAME

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include <LuaBridge.h>


// lua return values
#define LUA_FALSE			0
#define LUA_TRUE			1

// runtime-   InternalLog 
#define PARAM_ERROR        { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{}  {}", __FUNCTION__, g_oLangRec.GetString(183))); }
#define SCENE_NULL_ERROR   { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{}  {}", __FUNCTION__, g_oLangRec.GetString(184))); }

extern void InitLuaPlatform();

//  Lua VM (  Script.cpp)
extern lua_State* g_LuaState;

//   L   g_LuaState
extern lua_State *L;

inline void lua_dostringI(const char *pszCmd)
{
    luaL_dostring(L, pszCmd);
}

inline void lua_dostringI(std::string str)
{
   luaL_dostring(L, str.c_str());
}

inline void lua_platform_framemove()
{
}

inline void lua_platform_keydown(DWORD dwKey)
{
}

inline void lua_platform_mousedown(DWORD dwButton)
{
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
