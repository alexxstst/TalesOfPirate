#ifdef _LUA_GAME

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include <lauxlib.h>
    #include <lualib.h>
}

// lua��������ֵ����
#define LUA_FALSE			0 // ���ش���
#define LUA_TRUE			1 // ������ȷ

// �����������
// runtime-строки — используем InternalLog напрямую
#define PARAM_ERROR        { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{} — {}", __FUNCTION__, g_oLangRec.GetString(183))); }
#define SCENE_NULL_ERROR   { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{} — {}", __FUNCTION__, g_oLangRec.GetString(184))); }

extern void InitLuaPlatform();
extern void CreateScriptDebugWindow(HINSTANCE hInst, HWND hParent);
extern void ToggleScriptDebugWindow();

extern lua_State *L;

inline void lua_dostringI(const char *pszCmd)
{
    lua_dostring(L, pszCmd);
    //luaL_loadbuffer(L, pszCmd, nCmdSize, "line");
    //lua_pcall(L, 0, 0, 0);
}

inline void lua_dostringI(std::string str)
{
   lua_dostring(L, str.c_str());//  (int)str.size());
}

inline void lua_platform_framemove()
{
    // lua_dostring(L, "appRunTimer()");
}

inline void lua_platform_keydown(DWORD dwKey)
{
    //lua_dostring(L, szRun);
}

inline void lua_platform_mousedown(DWORD dwButton)
{
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
