// Script.h Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#pragma once

#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "lua.hpp"
#include <LuaBridge.h>

void print_error(lua_State* state);

// luaL_dofile / luaL_dostring with built-in error logging
#undef luaL_dofile
#define luaL_dofile(L, fn) \
	((luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0 ? (print_error(L), 1) : 0)

#undef luaL_dostring
#define luaL_dostring(L, s) \
	((luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0 ? (print_error(L), 1) : 0)

// Legacy compat
#define lua_dofile(L, fn) luaL_dofile(L, fn)

// Register a C++ function in Lua via LuaBridge (name matches symbol)
#define LUABRIDGE_REGISTER_FUNC(fn) .addFunction(#fn, fn)

// Register a raw lua_CFunction directly
#define LUA_REGISTER_CFUNC(L, fn) lua_register(L, #fn, fn)

#include "dbccommon.h"
#include "Character.h"

extern lua_State* g_pLuaState;
extern CCharacter* g_pNoticeChar;

extern BOOL	InitLuaScript();
extern BOOL CloseLuaScript();
extern BOOL	RegisterScript();
extern BOOL LoadScript();
extern void ReloadMission();
extern void ReloadLuaSdk();
extern void ReloadLuaInit();
extern void ReloadEntity( const char szFileName[] );


//#define E_LUAPARAM		LG( "luamis_error", "lua[%s]!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua[%s]!", __FUNCTION__ );
//#define E_LUANULL		LG( "luamis_error", "lua[%s]!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua[%s]!", __FUNCTION__ );
//#define E_LUACOMPARE	LG( "luamis_error", "lua[%s]!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua[%s]!", __FUNCTION__ );
//    Lua- (NPC-)
#define E_LUAPARAM		ToLogService("lua", LogLevel::Error, "lua function[{}] param number or type error!", __FUNCTION__); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua function[%s]param number or type error!", __FUNCTION__ );
#define E_LUANULL		ToLogService("lua", LogLevel::Error, "lua function[{}] pass param pointer is null and error!", __FUNCTION__); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua function[%s]pass param pointer is null and error!", __FUNCTION__ );
#define E_LUACOMPARE	ToLogService("lua", LogLevel::Error, "lua function[{}] param error is unknown of compara character!", __FUNCTION__); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua function[%s]param error is unknow of compara character!", __FUNCTION__ );


#define LUA_TRUE		1	// 
#define LUA_FALSE		0	// 
#define LUA_ERROR		-1	// 

#endif // _SCRIPT_H_

//---------------------------------------------------------
