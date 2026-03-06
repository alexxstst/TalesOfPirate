// Script.cpp Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#include "stdafx.h"
#include "Script.h"
#include "Parser.h"
#include "LuaFunc.h"
#include <string>
#include <list>
using namespace std;

//---------------------------------------------------------

const char* GetResPath(const char *pszRes)
{
	static char g_szTableName[255];
	string str = "resource";
	if(str.size() > 0)
	{
		str+="/";
	}
	str+=pszRes;
	strcpy(g_szTableName, str.c_str());
	return g_szTableName;
}

//CCharacter* g_pNoticeChar = NULL;
lua_State*	g_pLuaState = NULL;

BOOL InitLuaScript()
{
	g_pLuaState = lua_open();
	luaL_openlibs(g_pLuaState);

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

extern list<string> g_luaFNList;
BOOL RegisterScript()
{
	lua_State *L = g_pLuaState;

	//Lua
	if( !RegisterLuaFunc() )
		return FALSE;

	return TRUE;
}

void ReloadLuaInit()
{
	
}

void ReloadLuaSdk()
{	

}

void ReloadScript()
{
	// NPC
	luaL_dofile( g_pLuaState, GetResPath("script/Script01.lua") );
}

void ReloadEntity( const char szFileName[] )
{
	luaL_dofile( g_pLuaState, szFileName );
}

BOOL LoadScript()
{
	ReloadLuaInit();
	ReloadLuaSdk();
	ReloadScript();

	//Lua

	return TRUE;
}
