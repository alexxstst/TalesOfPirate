// Script.cpp Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#include "stdafx.h"
#include "Script.h"
#include "LuaAPI.h"
#include "NpcScript.h"
#include "CharScript.h"
#include "EntityScript.h"
#include "RecordScript.h"

//---------------------------------------------------------
extern const char* GetResPath(const char *pszRes);

#include "Npc.h"
#include "WorldEudemon.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include "Player.h"
#include "MapEntry.h"
namespace mission { class CEventEntity; }

CCharacter* g_pNoticeChar = NULL;
lua_State* g_pLuaState = NULL;

void print_error(lua_State* state) {
	const char* message = lua_tostring(state, -1);
	ToLogService("lua_error", LogLevel::Error, "Lua error: {}", message ? message : "unknown");
	lua_pop(state, 1);
}

BOOL InitLuaScript()
{
	g_pLuaState = luaL_newstate(); //lua_open();
	if (!g_pLuaState)
		return 1;

	luaL_openlibs(g_pLuaState);

	// Register C++ class stubs for LuaBridge pointer passing
	// Order matters: base classes first, then derived
	luabridge::getGlobalNamespace(g_pLuaState)
		.beginClass<CCharacter>("CCharacter").endClass()
		.beginClass<SubMap>("SubMap").endClass()
		.deriveClass<mission::CEventEntity, CCharacter>("CEventEntity").endClass()
		.deriveClass<mission::CNpc, CCharacter>("CNpc").endClass()
		.deriveClass<mission::CTalkNpc, mission::CNpc>("CTalkNpc").endClass()
		.deriveClass<mission::CWorldEudemon, mission::CNpc>("CWorldEudemon").endClass()
		.beginClass<net::RPacket>("RPacket").endClass()
		.beginClass<net::WPacket>("WPacket").endClass()
		.beginClass<CMapRes>("CMapRes").endClass()
		.beginClass<SItemGrid>("SItemGrid").endClass()
		.beginClass<CItem>("CItem").endClass()
		.beginClass<CPlayer>("CPlayer").endClass()
		.beginClass<CDynMapEntryCell>("CDynMapEntryCell").endClass()
		.beginClass<CMapEntryCopyCell>("CMapEntryCopyCell").endClass();

	g_luaAPI.Init(g_pLuaState);

	if( !RegisterScript() )
		return FALSE;
	
	if( !LoadScript() )
		return FALSE;

	return TRUE;
}

BOOL CloseLuaScript()
{
	if( g_pLuaState ) lua_close( g_pLuaState );
	g_pLuaState = NULL;
	return TRUE;
}

BOOL RegisterScript()
{
	if( !RegisterCharScript() || !RegisterNpcScript() )
		return FALSE;

	if( !RegisterEntityScript() )
		return FALSE;

	if( !RegisterRecordScript() )
		return FALSE;

	return TRUE;
}

void ReloadLuaInit()
{
	luaL_dofile( g_pLuaState, GetResPath("script/initial.lua") );
}

void ReloadLuaSdk()
{	
	luaL_dofile( g_pLuaState, GetResPath("script/MisSdk/NpcSdk.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisSdk/MissionSdk.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisSdk/scriptsdk.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/ScriptDefine.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcDefine.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/templatesdk.lua") );

	// updateallai_sdk
	luaL_dofile( g_pLuaState, GetResPath("script/birth/birth_conf.lua"));
	luaL_dofile( g_pLuaState, GetResPath("script/ai/ai.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/calculate/skilleffect.lua"));
}

void ReloadNpcScript()
{
	// NPC
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript01.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript02.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript03.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript04.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript05.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript06.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript07.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/MissionScript08.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/SendMission.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/EudemonScript.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/CharBornScript.lua") );

	// NPC
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript01.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript02.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript03.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript04.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript05.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript06.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript07.lua") );
	luaL_dofile( g_pLuaState, GetResPath("script/MisScript/NpcScript08.lua") );
}

void ReloadEntity( const char szFileName[] )
{
	luaL_dofile( g_pLuaState, szFileName );
}

BOOL LoadScript()
{
	ReloadLuaInit();
	ReloadLuaSdk();
	ReloadNpcScript();
	return TRUE;
}
