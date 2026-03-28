//---------------------------------------------------------
// EntityScript.cpp Created by knight-gong in 2005.5.12.
#include "stdafx.h"//add by alfred.shi 20080203

#include "EntityScript.h"
#include "GameAppNet.h"
#include "Character.h"
#include "lua_gamectrl.h"


//---------------------------------------------------------
_DBC_USING
using namespace mission;

inline int lua_GetCurSubmap( lua_State* L )
{
	if( !g_pScriptMap )
	{
		//LG( "entity_error", "��ͼָ��Ϊ�գ�" );
		g_logManager.InternalLog(LogLevel::Error, "errors", RES_STRING(GM_ENTITYSCRIPT_CPP_00001) );
		// Заменено printf → логирование
		g_logManager.InternalLog(LogLevel::Debug, "common", RES_STRING(GM_ENTITYSCRIPT_CPP_00001));
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, LUA_TRUE );
	lua_pushlightuserdata( L, g_pScriptMap );

	return 2;
}

inline int lua_CreateEventEntity( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 8 && lua_isnumber( L, 1 ) && lua_islightuserdata( L, 2 ) && 
		lua_isstring( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && lua_isnumber( L, 6 ) &&
		lua_isnumber( L, 7 ) && lua_isnumber( L, 8 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	BOOL bRet = FALSE;
	mission::CEventEntity* pEntity = NULL;
	BYTE byType = (BYTE)lua_tonumber( L, 1 );
	SubMap* pMap = (SubMap*)lua_touserdata( L, 2 );
	const char* pszName = lua_tostring( L, 3 );
	USHORT sID = (USHORT)lua_tonumber( L, 4 );
	USHORT sInfoID = (USHORT)lua_tonumber( L, 5 );
	DWORD  dwxPos = (DWORD)lua_tonumber( L, 6 );
	DWORD  dwyPos = (DWORD)lua_tonumber( L, 7 );
	USHORT sDir = (USHORT)lua_tonumber( L, 8 );
	pEntity = g_pGameApp->CreateEntity( byType );
	if( !pMap || !pEntity )
	{
		E_LUANULL;
		return 0;
	}
	bRet = pEntity->Create( *pMap, pszName, sID, sInfoID, dwxPos, dwyPos, sDir );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushlightuserdata( L, pEntity );

	return 2;
}

int lua_SetEntityData( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) >= 1;
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	BOOL bRet = FALSE;
	mission::CEventEntity* pEntity = (mission::CEventEntity*)lua_touserdata( L, 1 );
	switch( pEntity->GetType() )
	{
	case BASE_ENTITY:			// ����ʵ��
		{
		}
		break;

	case RESOURCE_ENTITY:		// ��Դʵ��
		{
			bValid = lua_gettop( L ) >= 4;
			if( !bValid )
			{
				E_LUAPARAM;
				return 0;
			}
			USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
			USHORT sCount = (USHORT)lua_tonumber( L, 3 );
			USHORT sTime = (USHORT)lua_tonumber( L, 4 );
			bRet = ((mission::CResourceEntity*)pEntity)->SetData( sItemID, sCount, sTime );
		}
		break;

	case TRANSIT_ENTITY:		// ����ʵ��
		{
		}
		break;

	case BERTH_ENTITY:			// ͣ��ʵ��
		{
			BOOL bValid = lua_gettop( L ) >= 5;
			if( !bValid )
			{
				E_LUAPARAM;
				return 0;
			}
			USHORT sBerthID = (USHORT)lua_tonumber( L, 2 );
			USHORT sxPos = (USHORT)lua_tonumber( L, 3 );
			USHORT syPos = (USHORT)lua_tonumber( L, 4 );
			USHORT sDir = (USHORT)lua_tonumber( L, 5 );
			bRet = ((mission::CBerthEntity*)pEntity)->SetData( sBerthID, sxPos, syPos, sDir );
		}
		break;
	default:
		{
			E_LUAPARAM;
			return 0;
		}
		break;
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

BOOL RegisterEntityScript()
{
	lua_State *L = g_pLuaState;

	REGFN(GetCurSubmap);
	REGFN(CreateEventEntity);
	REGFN(SetEntityData);

	return TRUE;
}