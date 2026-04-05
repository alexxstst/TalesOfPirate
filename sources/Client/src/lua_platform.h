#ifdef _LUA_GAME

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge.h>


// lua return values
#define LUA_FALSE			0
#define LUA_TRUE			1

// runtime-   InternalLog
#define PARAM_ERROR        { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{}  {}", __FUNCTION__, GetLanguageString(183))); }
#define SCENE_NULL_ERROR   { g_logManager.InternalLog(LogLevel::Error, "lua", std::format("{}  {}", __FUNCTION__, GetLanguageString(184))); }

extern void InitLuaPlatform();

//  Lua VM (  Script.cpp)
extern lua_State* g_LuaState;

//   L   g_LuaState
extern lua_State* L;

inline void lua_dostringI(const char* pszCmd) {
	luaL_dostring(L, pszCmd);
}

inline void lua_dostringI(std::string str) {
	luaL_dostring(L, str.c_str());
}

inline void lua_platform_framemove() {
}

inline void lua_platform_keydown(DWORD dwKey) {
}

inline void lua_platform_mousedown(DWORD dwButton) {
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
#include "netprotocol.h"
#include "EffectObj.h"

// util
void MsgBox(const std::string& content);
int GetTickCount_Lua();
int Rand_Lua(int nRange);
void ToDebugLog(const std::string& message);

// app
void appSetCaption(const std::string& caption);
void appPlaySound(int soundId);
void appUpdateRender();
CGameScene* appGetCurScene();
void appSetCurScene(CGameScene* pScene);
CGameScene* appCreateScene(const std::string& mapName, int maxCha, int maxObj, int maxItem, int maxEff);

// scene
CSceneNode* sceAddObj(CGameScene* pScene, int nType, int nScriptID);
CSceneNode* sceGetObj(CGameScene* pScene, int nType, int nArrayID);
void sceRemoveObj(CSceneNode* pNode);
void sceSetMainCha(CGameScene* pScene, CCharacter* pCha);
CCharacter* sceGetMainCha(CGameScene* pScene);
CCharacter* sceGetHoverCha(CGameScene* pScene);
void sceEnableDefaultMouse(CGameScene* pScene, int bEnable);

// object
int objIsValid(CSceneNode* pNode);
int objGetID(CSceneNode* pNode);
void objSetPos(CSceneNode* pNode, int x, int y);
int objGetPos(lua_State* L);
float objGetFaceAngle(CSceneNode* pNode);
void objSetFaceAngle(CSceneNode* pNode, int angle);
int objGetAttr(CSceneNode* pNode, int sType);
void objSetAttr(CSceneNode* pNode, int sType, int lValue);

// character
void chaSay(CCharacter* pCha, const std::string& text);
void chaMoveTo(CCharacter* pCha, int x, int y);
void chaStop(CCharacter* pCha);
int chaChangePart(CCharacter* pCha, int nTabID);
void chaPlayPose(CCharacter* pCha, int nPoseID, int bHold);

// camera
void camSetCenter(float x, float y);
void camFollow(int bEnable);
void camMoveForward(float fStep, int bHang);
void camMoveLeft(float fStep, int bHang);
void camMoveUp(float fStep);
void camSetAngle(float fAngle);
int camGetCenter(lua_State* L);

// input
int IsKeyDown(int key);

// UI
void uiHideAll();

#endif
