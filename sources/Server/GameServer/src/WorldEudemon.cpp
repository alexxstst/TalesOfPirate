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

		// 初始化NPC脚本全局变量信息
		lua_getglobal( g_pLuaState, "ResetNpcInfo" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "ResetNpcInfo" );
			return FALSE;
		}

		lua_pushlightuserdata( g_pLuaState, this );
		lua_pushstring( g_pLuaState, szName );		

		int nStatus = lua_pcall( g_pLuaState, 2, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s]的脚本初始化处理函数[ResetNpcInfo]调用失败!", szName );
			LG( "NpcInit", "npc[%s]'s script init dispose function[ResetNpcInfo]transfer error!", szName );
			//printf( "npc[%s]的脚本初始化处理函数[ResetNpcInfo]调用失败!\n", szName );
			printf( RES_STRING(GM_WORLDEUDEMON_CPP_00001), szName );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}
		lua_settop(g_pLuaState, 0);

		// 调用NPC初始化全局变量信息入口函数
		lua_getglobal( g_pLuaState, szFunc );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", szFunc );
			return FALSE;
		}

		nStatus = lua_pcall( g_pLuaState, 0, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s]的脚本数据处理函数[%s]调用失败!", szName, szFunc );
			LG( "NpcInit", "npc[%s]'s script data dispose function[%s]transfer failed!", szName, szFunc );
			//printf( "npc[%s]的脚本数据处理函数[%s]调用失败!\n", szName, szFunc );
			printf( RES_STRING(GM_WORLDEUDEMON_CPP_00002), szName, szFunc );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}
		lua_settop(g_pLuaState, 0);

		// 获取NPC的对话信息和交易信息
		lua_getglobal( g_pLuaState, "GetNpcInfo" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "GetNpcInfo" );
			return FALSE;
		}

		lua_pushlightuserdata( g_pLuaState, this );
		lua_pushstring( g_pLuaState, szName );

		nStatus = lua_pcall( g_pLuaState, 2, 0, 0 );
		if( nStatus )
		{
			//LG( "NpcInit", "npc[%s]的脚本初始化处理函数[GetNpcInfo]调用失败!", szName );
			LG( "NpcInit", "npc[%s]'s script init dispose fuction[GetNpcInfo]transfer failed!", szName );
			//printf( "npc[%s]的脚本初始化处理函数[GetNpcInfo]调用失败!\n", szName );
			printf( RES_STRING(GM_WORLDEUDEMON_CPP_00003), szName );
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

		// 初始化npc脚本信息
		InitScript( szMsgProc, szName );
		
		// npc配置表编号
		m_sNpcID = 0;

		// 
		strncpy( m_name, szName, 32 - 1 );
		strncpy( m_szMsgProc, szMsgProc, ROLE_MAXSIZE_MSGPROC - 1 );

		m_ID = ulID;
		Char szLogName[defLOG_NAME_LEN] = "";
		sprintf(szLogName, "Cha-%s+%u", GetName(), GetID());
		m_CLog.SetLogName(szLogName);

		// 默认使用第一个角色信息
		m_pCChaRecord = (CChaRecord*)GetChaRecordInfo( 1 );
		m_cat = (SHORT)m_pCChaRecord->lID;

		m_CChaAttr.Init( 1 );		
		setAttr(ATTR_CHATYPE, enumCHACTRL_NPC);

		return TRUE;
	}
    
	HRESULT CWorldEudemon::MsgProc( CCharacter& character, RPACKET packet )
	{
		// 调用NPC脚本对话处理函数
		lua_getglobal( g_pLuaState, "NpcProc" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "NpcProc" );
			return FALSE;
		}

		lua_pushlightuserdata( g_pLuaState, (void*)&character );
		lua_pushlightuserdata( g_pLuaState, (void*)this );
		lua_pushlightuserdata( g_pLuaState, (void*)&packet );
		lua_pushnumber( g_pLuaState, m_sScriptID );

		int nStatus = lua_pcall( g_pLuaState, 4, 0, 0 );
		if( nStatus )
		{
			//character.SystemNotice( "npc[%s]的脚本消息处理函数[NpcProc]调用失败!", m_name );
			character.SystemNotice( RES_STRING(GM_WORLDEUDEMON_CPP_00004), m_name );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return EN_FAILER;
		}
		lua_settop(g_pLuaState, 0);

		return EN_OK;
	}


}

