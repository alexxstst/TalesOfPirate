// WorldEudemon.cpp Created by knight-gongjian 2005.3.9.
//---------------------------------------------------------
#include "stdafx.h"
#include "WorldEudemon.h"
#include "SubMap.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "Script.h"
#include "lua_gamectrl.h"
//---------------------------------------------------------

namespace mission
{
	CWorldEudemon g_WorldEudemon;

	CWorldEudemon::CWorldEudemon()
	: CNpc()
	{		 
	}

	CWorldEudemon::~CWorldEudemon()
	{
	}

	BOOL CWorldEudemon::InitScript( const char szFunc[], const char szName[] )
	{
		if( szFunc[0] == '0' ) return TRUE;

		// NPC
		lua_getglobal( g_pLuaState, "ResetNpcInfo" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			ToLogService("common", "ResetNpcInfo" );
			return FALSE;
		}

		luabridge::push( g_pLuaState, static_cast<mission::CNpc*>(this) );
		lua_pushstring( g_pLuaState, szName );

		int nStatus = lua_pcall( g_pLuaState, 2, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s][ResetNpcInfo]!", szName );
			ToLogService("common", "npc[{}]'s script init dispose function[ResetNpcInfo]transfer error!", szName );
			//  printf    snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_WORLDEUDEMON_CPP_00001), szName); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}
		lua_settop(g_pLuaState, 0);

		// NPC
		lua_getglobal( g_pLuaState, szFunc );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			g_logManager.InternalLog(LogLevel::Debug, "common", szFunc );
			return FALSE;
		}

		nStatus = lua_pcall( g_pLuaState, 0, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s][%s]!", szName, szFunc );
			ToLogService("common", "npc[{}]'s script data dispose function[{}]transfer failed!", szName, szFunc );
			//  printf    snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_WORLDEUDEMON_CPP_00002), szName, szFunc); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}
		lua_settop(g_pLuaState, 0);

		// NPC
		lua_getglobal( g_pLuaState, "GetNpcInfo" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			ToLogService("common", "GetNpcInfo" );
			return FALSE;
		}

		luabridge::push( g_pLuaState, static_cast<mission::CNpc*>(this) );
		lua_pushstring( g_pLuaState, szName );

		nStatus = lua_pcall( g_pLuaState, 2, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s][GetNpcInfo]!", szName );
			ToLogService("common", "npc[{}]'s script init dispose fuction[GetNpcInfo]transfer failed!", szName );
			//  printf    snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_WORLDEUDEMON_CPP_00003), szName); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}
		lua_settop(g_pLuaState, 0);

		return TRUE;
	}

	BOOL CWorldEudemon::Load( const char szMsgProc[], const char szName[], dbc::uLong ulID )
	{
		Clear();

		// npc
		InitScript( szMsgProc, szName );
		
		// npc
		m_sNpcID = 0;

		// 
		strncpy( m_name, szName, 32 - 1 );
		strncpy( m_szMsgProc, szMsgProc, ROLE_MAXSIZE_MSGPROC - 1 );

		m_ID = ulID;
		Char szLogName[defLOG_NAME_LEN] = "";
		sprintf(szLogName, "Cha-%s+%u", GetName(), GetID());
		SetLogName(szLogName);

		// 
		m_pCChaRecord = (CChaRecord*)GetChaRecordInfo( 1 );
		m_cat = (SHORT)m_pCChaRecord->lID;

		m_CChaAttr.Init( 1 );		
		setAttr(ATTR_CHATYPE, enumCHACTRL_NPC);

		return TRUE;
	}
    
	HRESULT CWorldEudemon::MsgProc( CCharacter& character, net::RPacket& packet )
	{
		// NPC
		lua_getglobal( g_pLuaState, "NpcProc" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			ToLogService("common", "NpcProc" );
			return FALSE;
		}

		luabridge::push( g_pLuaState, &character );
		luabridge::push( g_pLuaState, static_cast<mission::CNpc*>(this) );
		luabridge::push( g_pLuaState, &packet );
		lua_pushnumber( g_pLuaState, m_sScriptID );

		int nStatus = lua_pcall( g_pLuaState, 4, 0, 0 );
		if( nStatus )
		{
			//character.SystemNotice( "npc[%s][NpcProc]!", m_name );
			character.SystemNotice( RES_STRING(GM_WORLDEUDEMON_CPP_00004), m_name );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return EN_FAILER;
		}
		lua_settop(g_pLuaState, 0);

		return EN_OK;
	}


}

