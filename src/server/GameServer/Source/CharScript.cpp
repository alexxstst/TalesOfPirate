// CharScript.cpp Created by knight-gongjian 2004.12.13.
//---------------------------------------------------------
#include "stdafx.h"//add by alfred.shi 20080202
#include "CharScript.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include "Character.h"
#include <assert.h>
#include "lua_gamectrl.h"
#include "Expand.h"
#include "CharBoat.h"
#include "Guild.h"
#include "Auction.h"

using namespace std;

list<string> g_luaFNList;

//---------------------------------------------------------
_DBC_USING
using namespace mission;

// Add by lark.li 20080310 begin
inline int lua_GetResString( lua_State *L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_isstring( L, 1 );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	const char* pszID = lua_tostring( L, 1 );
	if( pszID == NULL )
	{
		PARAM_ERROR;
		return 0;
	}

	const char* text = ConvertResString(pszID);

	lua_pushstring(L, text);

	return 1;
T_E}
// End

inline int lua_SetMap( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_isstring( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	const char* pszMap = lua_tostring( L, 1 );
	if( pszMap == NULL )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byID = (BYTE)lua_tonumber( L, 2 );
	BOOL bRet = g_MapID.AddInfo( pszMap, byID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetMapGuildWar( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CMapRes *pCMap = (CMapRes *)lua_touserdata(L, 1);
	if( pCMap == NULL )
	{
		E_LUANULL;
		return 0;
	}

	int nGuildWar = (int)lua_tonumber( L, 2 );
	if(nGuildWar > 0)
	{
		pCMap->SetGuildWar(true);
	}
	else
	{
		pCMap->SetGuildWar(false);
	}

	return 1;
T_E}

inline int lua_AddTrigger( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 10 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) &&	lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) &&
					lua_isnumber( L, 6 ) && lua_isnumber( L, 7 ) && lua_isnumber( L, 8 ) &&
					lua_isnumber( L, 9 ) && lua_isnumber( L, 10 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	TRIGGER_DATA Data;
	memset( &Data, 0, sizeof(TRIGGER_DATA) );
	Data.wMissionID = (WORD)lua_tonumber( L, 2 );
	Data.wTriggerID = (WORD)lua_tonumber( L, 3 );
	Data.byType     = (BYTE)lua_tonumber( L, 4 );
	Data.wParam1    = (WORD)lua_tonumber( L, 5 );
	Data.wParam2    = (WORD)lua_tonumber( L, 6 );
	Data.wParam3    = (WORD)lua_tonumber( L, 7 );
	Data.wParam4    = (WORD)lua_tonumber( L, 8 );
	Data.wParam5    = (WORD)lua_tonumber( L, 9 );
	Data.wParam6    = (WORD)lua_tonumber( L, 10 );

	BOOL bRet = pChar->AddTrigger( Data );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	
	return 1;
T_E}

inline int lua_ClearTrigger( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wTriggerID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->ClearTrigger( wTriggerID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	
	return 1;
T_E}

inline int lua_DeleteTrigger( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wTriggerID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->DeleteTrigger( wTriggerID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	
	return 1;	
}

inline int lua_AddMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ));
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	WORD wParam = (WORD)lua_tonumber( L, 3 );
	BOOL bRet = pChar->AddRole( wID, wParam );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasRole( wID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ClearMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->ClearRole( wID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetMisScriptID( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 &&lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	WORD wScriptID;
	BOOL bRet = pChar->GetMisScriptID( wID, wScriptID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wScriptID );

	return 2;
T_E}

inline int lua_SetMissionComplete( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->SetMissionComplete( wID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetMissionFailure( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->SetMissionFailure( wID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasMisssionFailure( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasMissionFailure( wID );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_IsMissionFull( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->IsRoleFull();
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetFlag( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	WORD wFlag = (WORD)lua_tonumber( L, 3 );
	BOOL bRet = pChar->SetFlag( wID, wFlag );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ClearFlag( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	WORD wFlag = (WORD)lua_tonumber( L, 3 );
	BOOL bRet = pChar->ClearFlag( wID, wFlag );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsFlag( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	WORD wFlag = (WORD)lua_tonumber( L, 3 );
	BOOL bRet = pChar->IsFlag( wID, wFlag );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsValidFlag( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wFlag = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->IsValidFlag( wFlag );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetRecord( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRec = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->SetRecord( wRec );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ClearRecord( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRec = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->ClearRecord( wRec );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsRecord( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRec = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->IsRecord( wRec );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsValidRecord( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRec = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->IsValidRecord( wRec );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_AddSkill( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) && */
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>( RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	if (pChar->GetPlayer())
		pChar = pChar->GetPlayer()->GetMainCha(); // 

	WORD  wSkillID = (WORD)lua_tonumber( L, 3 );
	BYTE  byLevel   = (BYTE)lua_tonumber( L, 4 );
	BOOL bRet = pChar->GetPlyMainCha()->LearnSkill( wSkillID, byLevel );	
	lua_pushnumber( L, LUA_TRUE );

	if( bRet )
	{
		//char szSkill[defSKILL_NAME_LEN] = "";
		//char* szSkill = const_cast<char*>( RES_STRING(GM_CHARSCRIPT_CPP_00002));
		char szSkill[defENTITY_NAME_LEN];
		strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00002), defENTITY_NAME_LEN - 1 );

		CSkillRecord* pSkill = GetSkillRecordInfo( wSkillID );
		if( pSkill )
		{
			strncpy( szSkill, pSkill->szName, defSKILL_NAME_LEN - 1 );
		}

		char szData[128];
		//sprintf( szData, "%s%s", szNpc, szSkill );
		sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00003), szNpc, szSkill );
		pChar->SystemNotice( szData );
		TL( CHA_MIS, pChar->GetName(), "", szData );
	}

	return 1;
T_E}

inline int lua_AddLevel( lua_State* L )
{T_B
	return 0;
T_E}

inline int lua_AddSailExp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) && */
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );
	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwValue;
	DWORD dwMinExp = (DWORD)lua_tonumber( L, 3 );
	DWORD dwMaxExp = (DWORD)lua_tonumber( L, 4 );
	if( dwMinExp >= dwMaxExp )
	{
		dwValue = dwMinExp;
	}
	else
	{
		dwValue = rand()%(dwMaxExp - dwMinExp) + dwMinExp;
	}

	pChar->GetPlyMainCha()->AddAttr( ATTR_CSAILEXP, dwValue );
	char szData[128];
	//sprintf( szData, "%s%d", szNpc, dwValue );
	sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00004), szNpc, dwValue );
	pChar->SystemNotice( szData );
	TL( CHA_MIS, pChar->GetName(), "", szData );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_AddLifeExp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) && */
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );
	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwValue;
	DWORD dwMinExp = (DWORD)lua_tonumber( L, 3 );
	DWORD dwMaxExp = (DWORD)lua_tonumber( L, 4 );
	if( dwMinExp >= dwMaxExp )
	{
		dwValue = dwMinExp;
	}
	else
	{
		dwValue = rand()%(dwMaxExp - dwMinExp) + dwMinExp;
	}
	
	pChar->GetPlyMainCha()->AddAttr( ATTR_CLIFEEXP, dwValue );
	char szData[128];
	//sprintf( szData, "%s%d", szNpc, dwValue );
	sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00005), szNpc, dwValue );
	pChar->SystemNotice( szData );
	TL( CHA_MIS, pChar->GetName(), "", szData );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_AddExp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/ 
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwValue;
	DWORD dwMinExp = (DWORD)lua_tonumber( L, 3 );
	DWORD dwMaxExp = (DWORD)lua_tonumber( L, 4 );
	if( dwMinExp >= dwMaxExp )
	{
		dwValue = dwMinExp;
	}
	else
	{
		dwValue = rand()%(dwMaxExp - dwMinExp) + dwMinExp;
	}
	
	BOOL bRet = pChar->GetPlyMainCha()->AddExpAndNotic( dwValue );	
	char szData[128];
	//sprintf( szData, "%s%d", szNpc, dwValue );
	sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00006), szNpc, dwValue );
	pChar->SystemNotice( szData );
	TL( CHA_MIS, pChar->GetName(), "", szData );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_AddExpAndType( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) && */
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwValue;
	BYTE byType = (BYTE)lua_tonumber( L, 3 );
	DWORD dwMinExp = (DWORD)lua_tonumber( L, 4 );
	DWORD dwMaxExp = (DWORD)lua_tonumber( L, 5 );
	if( dwMinExp >= dwMaxExp )
	{
		dwValue = dwMinExp;
	}
	else
	{
		dwValue = rand()%(dwMaxExp - dwMinExp) + dwMinExp;
	}
	
	BOOL bRet;
	if( byType == mission::MIS_EXP_NOMAL )// 
	{
		bRet = pChar->GetPlyMainCha()->AddExpAndNotic( dwValue );
		char szData[128];
		//sprintf( szData, "%s%d", szNpc, dwValue );
		sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00006), szNpc, dwValue );
		pChar->SystemNotice( szData );
		TL( CHA_MIS, pChar->GetName(), "", szData );
	}
	else if( byType == MIS_EXP_SAIL ) 	// 
	{
		bRet = pChar->GetPlyMainCha()->AddAttr( ATTR_CSAILEXP, dwValue );
		char szData[128];
		//sprintf( szData, "%s%d", szNpc, dwValue );
		sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00004), szNpc, dwValue );
		pChar->SystemNotice( szData );
		TL( CHA_MIS, pChar->GetName(), "", szData );
	}
	else if( byType == MIS_EXP_LIFE	)	// 
	{
		bRet = pChar->GetPlyMainCha()->AddAttr( ATTR_CLIFEEXP, dwValue );
		char szData[128];
		//sprintf( szData, "%s%d", szNpc, dwValue );
		sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00005), szNpc, dwValue );
		pChar->SystemNotice( szData );
		TL( CHA_MIS, pChar->GetName(), "", szData );
	}
	else
	{
		//pChar->SystemNotice( "Name[%s], Type[%d], Exp[%d]", szNpc, byType, dwValue );
		pChar->SystemNotice( RES_STRING(GM_CHARSCRIPT_CPP_00007), szNpc, byType, dwValue );
		bRet = FALSE;
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );	

	return 1;
T_E}

inline int lua_AddMoney( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] ="";
	char szNpc[defENTITY_NAME_LEN];
	strncpy( szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );
	
	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwMoney = (DWORD)lua_tonumber( L, 3 );
	pChar->GetPlyMainCha()->AddMoney( szNpc, dwMoney );
	char szData[128];
	//sprintf( szData, "%s%d", szNpc, dwMoney );
	sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00008), szNpc, dwMoney );
	TL( CHA_MIS, pChar->GetName(), "", szData );
	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_TakeMoney( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwMoney = (DWORD)lua_tonumber( L, 3 );
	BOOL bRet = pChar->GetPlyMainCha()->TakeMoney( szNpc, dwMoney );
	char szData[128];
	//sprintf( szData, "%s%d", szNpc, dwMoney );
	sprintf( szData, RES_STRING(GM_CHARSCRIPT_CPP_00009), szNpc, dwMoney );
	TL( CHA_MIS, pChar->GetName(), "", szData );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
T_E}

inline int lua_HasMoney( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwMoney = (DWORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->GetPlyMainCha()->HasMoney( dwMoney );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasCancelMissionMoney( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwMoney = (long)pChar->getAttr( ATTR_LV ) * (long)pChar->getAttr( ATTR_LV ) + 100;
	BOOL bRet = pChar->GetPlyMainCha()->HasMoney( dwMoney );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_TakeCancelMissionMoney( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) /*&& lua_islightuserdata( L, 2 )*/);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	DWORD dwMoney = (long)pChar->getAttr( ATTR_LV ) * (long)pChar->getAttr( ATTR_LV ) + 100;
	BOOL bRet = pChar->GetPlyMainCha()->TakeMoney( szNpc, dwMoney );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
T_E}

inline int lua_CheckFusionItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	SItemGrid* pItem1 = (SItemGrid*)lua_touserdata( L, 1 );	
	SItemGrid* pItem2 = (SItemGrid*)lua_touserdata( L, 2 );	
	if( !pItem1 || !pItem2 )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pItem1->FusionCheck( *pItem2 ) ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

	//Add by sunny.sun 20080529
	//Begin
	inline int lua_GetTicketIssue( lua_State * L)//
	{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	SItemGrid * pItem = (SItemGrid*)lua_touserdata( L, 1 );

	int issue = pItem->sInstAttr[0][1] % 1000;

	lua_pushnumber( L, (long)issue );

	return 1;
	T_E}

	inline int lua_GetTicketItemno( lua_State * L)//
	{T_B
		BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
		if( !bValid )
		{
			E_LUAPARAM;
			return 0;
		}
		
		SItemGrid * pItem1 = (SItemGrid*)lua_touserdata( L, 1 );
		int index = (int)lua_tonumber( L, 2 );

		if( floor( (float)pItem1->sInstAttr[0][1] / 1000) > 0)
		{
			if(index >= 0 && index < 7)
			{
				short c1 = (short)floor( (float)pItem1->sInstAttr[2][1] / 100);
				short c2 = pItem1->sInstAttr[2][1] % 100;

				short c3 = (short)floor( (float)pItem1->sInstAttr[3][1] / 100);
				short c4 = pItem1->sInstAttr[3][1] % 100;

				short c5 = (short)floor( (float)pItem1->sInstAttr[4][1] / 100);
				short c6 = pItem1->sInstAttr[4][1] % 100;
				
				char buffer[7];
				sprintf( buffer, "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6);
				string Ticketno = buffer;	
				
				if( index == 0)
				{	
					lua_pushstring(L, Ticketno.c_str());
				}
				else
				{
					lua_pushstring( L, Ticketno.substr(index - 1, 1).c_str());
				}
			} 
			return 1;
		}
		else
			return 0;
	T_E}

inline int lua_GetSItemGrid( lua_State * L)//
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	int index = (int)lua_tonumber( L, 2 );
	SItemGrid * pGrid = pChar->m_CKitbag.GetGridContByID( index );
	lua_pushlightuserdata( L, pGrid );
	return 1;
T_E}
	//End
inline int lua_FusionItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	SItemGrid* pItem1 = (SItemGrid*)lua_touserdata( L, 1 );	
	SItemGrid* pItem2 = (SItemGrid*)lua_touserdata( L, 2 );	
	if( !pItem1 || !pItem2 )
	{
		E_LUANULL;
		return 0;
	}

	//
	//Modify by ning.yan 20080821  begin
	//if(pItem2->sID >= CItemRecord::enumItemFusionStart)
	CItemRecord * pItem = GetItemRecordInfo(pItem2->sID);
	if(CItemRecord::IsVaildFusionID(pItem))  //end  ning.yan
	{
		pItem1->lDBParam = pItem2->lDBParam;
	}
	pItem1->CopyInstAttr( *pItem2 );

	//10
	pItem1->SetItemLevel(10);
	
	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_MakeItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) &&
		lua_isnumber( L, 2 ) && lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );
	BYTE byType    = (BYTE)lua_tonumber( L, 4 );

	USHORT sItemPos = 0;
	BOOL bRet = pChar->GetPlyMainCha()->MakeItem( sItemID, sCount, sItemPos, byType );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, sItemPos );

	return 2;
T_E}

inline int lua_GiveItem( lua_State* L )
{T_B
	BOOL bValid = (( lua_gettop( L ) >= 5 && lua_gettop(L) <= 7) && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
		lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) ) ;
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );
	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 4 );
	BYTE byType    = (BYTE)lua_tonumber( L, 5 );
	BOOL isTradable = 1;
	LONG expiration = 0;

	if (lua_gettop(L) >= 6 && lua_isnumber(L,6)) {
		isTradable = static_cast<BOOL> (lua_tonumber(L, 6));
	}
	if(lua_gettop(L) == 7 && lua_isnumber(L,7)) {
		expiration = std::time(0) + static_cast<LONG> (lua_tonumber(L, 7));
	}

	BOOL bRet = pChar->GetPlyMainCha()->AddItem( sItemID, sCount, szNpc, byType ,'\a', isTradable, expiration );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GiveItemX( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
				lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[defENTITY_NAME_LEN] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc* pTalk = ( CTalkNpc*)lua_touserdata( L, 2 );
	if( pTalk )
	{
		strncpy( szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1 );
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 4 );
	BYTE byType    = (BYTE)lua_tonumber( L, 5 );

	BOOL bRet = pChar->GetPlyMainCha()->AddItem2KitbagTemp( sItemID, sCount, szNpc, byType );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GiveItemY(lua_State* L)
// similar to lua_GiveItem, but returns a pointer to the SItemGrid.
{
	T_B
		BOOL bValid = ((lua_gettop(L) >= 5 && lua_gettop(L) <= 7) && lua_islightuserdata(L, 1) && /*lua_islightuserdata( L, 2 ) &&*/
			lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (pChar == NULL)
	{
		E_LUANULL;
		return 0;
	}


	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata(L, 2);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	USHORT sItemID = (USHORT)lua_tonumber(L, 3);
	USHORT sCount = (USHORT)lua_tonumber(L, 4);
	BYTE byType = (BYTE)lua_tonumber(L, 5);
	BOOL isTradable = 1;
	LONG expiration = 0;

	if (lua_gettop(L) >= 6 && lua_isnumber(L, 6)) {
		isTradable = static_cast<BOOL> (lua_tonumber(L, 6));
	}
	if (lua_gettop(L) == 7 && lua_isnumber(L, 7)) {
		expiration = std::time(0) + static_cast<LONG> (lua_tonumber(L, 7));
	}
	short posID = defKITBAG_DEFPUSH_POS;
	BOOL bRet = (pChar->GetPlyMainCha()->AddItem(sItemID, sCount, szNpc, byType, '\a', isTradable, expiration, &posID));
	SItemGrid* pItem = pChar->GetItem2(2, posID);
	lua_pushlightuserdata(L, pItem);

	return 1;
	T_E
}

inline int lua_HasLeaveBagTempGrid( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sNum = (USHORT)lua_tonumber( L, 2 );

	BOOL bRet = pChar->HasLeaveBagTempGrid( sNum );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_TakeItemBagTemp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
				lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[128] = "";
	//char* szNpc = const_cast<char*>(RES_STRING(GM_CHARSCRIPT_CPP_00001));
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc*   pNpc  = (CTalkNpc*)lua_touserdata( L, 2 );
	if( pNpc )
	{
		strcpy( szNpc, pNpc->GetName() );
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 4 );

	BOOL bRet = pChar->GetPlyMainCha()->TakeItemBagTemp( sItemID, sCount, szNpc );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_TakeItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && /*lua_islightuserdata( L, 2 ) &&*/
		lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	//char szNpc[128] = "";
	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1 );

	CTalkNpc*   pNpc  = (CTalkNpc*)lua_touserdata( L, 2 );
	if( pNpc )
	{
		strcpy( szNpc, pNpc->GetName() );
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 4 );

	BOOL bRet = pChar->GetPlyMainCha()->TakeItem( sItemID, sCount, szNpc );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );

	BOOL bRet = pChar->GetPlyMainCha()->HasItem( sItemID, sCount );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BagTempHasItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );

	BOOL bRet = pChar->GetPlyMainCha()->HasItemBagTemp( sItemID, sCount );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BankHasItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );

	BOOL bRet = pChar->GetPlayer()->BankHasItem( sItemID, sCount );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_EquipHasItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );

	short sItemCount = 0;
	for(int i = 0; i < enumEQUIP_NUM; i++)
	{
		SItemGrid *pItem = &(pChar->m_SChaPart.SLink[i]);
		if(pItem->sID == sItemID && pItem->sNum > 0)
		{
			sItemCount += pItem->sNum;
		}
	}

	BOOL bRet = (sItemCount >= sCount) ? true : false;
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsEquip( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = false;
	for(int i = 0; i < enumEQUIP_NUM; i++)
	{
		SItemGrid *pItem = &(pChar->m_SChaPart.SLink[i]);
		if(pItem->sID > 0 && pItem->sNum > 0)
		{
			bRet = true;
			break;
		}
	}
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_KitbagLock( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );

	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	USHORT sLock = (USHORT)lua_tonumber( L, 2 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sKitbagLock = pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() ? 1 : 0;
	BOOL bRet = (sLock == sKitbagLock) ? true : false;
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetNumItem( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 2 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 3 );

	BOOL bRet = pChar->GetPlyMainCha()->GetNumItem( sItemID, sCount );
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, sCount );

	return 2;
T_E}

inline int lua_GetNeedItemCount( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (USHORT)lua_tonumber( L, 2 );
	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount = 0;

	BOOL bRet = pChar->GetMisNeedItemCount( wRoleID, sItemID, sCount );
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, sCount );

	return 2;
}

inline int lua_AddMissionState( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
		lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byID = (BYTE)lua_tonumber( L, 3 );
	BYTE  byState = (BYTE)lua_tonumber( L, 4 );
	BOOL bRet = pChar->AddMissionState( dwNpcID, byID, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ResetMissionState( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	mission::CTalkNpc* pTalk = (mission::CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->ResetMissionState( *pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetMissionState( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byState;
	BOOL bRet = pChar->GetMissionState( dwNpcID, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byState );	

	return 2;
T_E}

inline int lua_GetNumMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byNum = 0;
	BOOL  bRet = pChar->GetNumMission( dwNpcID, byNum );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byNum );

	return 2;
T_E}

inline int lua_GetMissionInfo( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byIndex = (BYTE)lua_tonumber( L, 3 );

	BYTE byID, byState;
	BOOL bRet = pChar->GetMissionInfo( dwNpcID, byIndex, byID, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byID );
	lua_pushnumber( L, byState );

	return 3;
T_E}

inline int lua_GetCharMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byID = (BYTE)lua_tonumber( L, 3 );

	BYTE byState;
	BOOL bRet = pChar->GetCharMission( dwNpcID, byID, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byState );

	return 2;
T_E}

inline int lua_GetNextMission( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );

	BYTE byIndex, byID, byState;
	BOOL bRet = pChar->GetNextMission( dwNpcID, byIndex, byID, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byIndex );
	lua_pushnumber( L, byID );
	lua_pushnumber( L, byState );

	return 4;
T_E}

inline int lua_IsMissionState( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_isnumber( L , 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	BYTE byState = (BYTE)lua_tonumber( L, 1 );
	BYTE byFlag  = (BYTE)lua_tonumber( L, 2 );
	lua_pushnumber( L, ( byFlag&byState ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetMissionPage( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE byPrev = (BYTE)lua_tonumber( L, 3 );
	BYTE byNext = (BYTE)lua_tonumber( L, 4 );
	BYTE byState = (BYTE)lua_tonumber( L, 5 );

	BOOL bRet = pChar->SetMissionPage( dwNpcID, byPrev, byNext, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetMissionPage( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE byPrev, byNext, byState;
	BOOL bRet = pChar->GetMissionPage( dwNpcID, byPrev, byNext, byState );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byPrev );
	lua_pushnumber( L, byNext );
	lua_pushnumber( L, byState );

	return 4;
T_E}

inline int lua_SetMissionTempInfo( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
		lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	WORD  wID = (WORD)lua_tonumber( L, 3 );
	BYTE  byState = (BYTE)lua_tonumber( L, 4 );
	BYTE  byType = (BYTE)lua_tonumber( L, 5 );

	BOOL  bRet = pChar->SetTempData( dwNpcID, wID, byState, byType );
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetMissionTempInfo( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE byState, byType;
	WORD wID;
	BOOL bRet = pChar->GetTempData( dwNpcID, wID, byState, byType );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wID );
	lua_pushnumber( L, byState );
	lua_pushnumber( L, byType );
	return 4;
T_E}


void ReAllPlayEffect(CCharacter* pChar) {
	WPacket l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MC_CHAPLAYEFFECT);
	WRITE_LONG(l_wpk, pChar->GetID());
	WRITE_LONG(l_wpk, 114);
	pChar->NotiChgToEyeshot(l_wpk);
}

// 
inline int lua_ReAll( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	// 
	pChar->RestoreAll();
	ReAllPlayEffect(pChar);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
T_E}

inline int lua_ReAllHp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	// 
	pChar->RestoreAllHp();
	ReAllPlayEffect(pChar);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
T_E}

inline int lua_ReHp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byRate = (BYTE)lua_tonumber( L, 2 );
	// 
	pChar->RestoreHp( byRate );
	ReAllPlayEffect(pChar);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
T_E}

inline int lua_ReAllSp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	// 
	pChar->RestoreAllSp();
	ReAllPlayEffect(pChar);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
T_E}

inline int lua_ReSp( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byRate = (BYTE)lua_tonumber( L, 2 );
	// 
	pChar->RestoreSp( byRate );
	ReAllPlayEffect(pChar);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
T_E}

inline int lua_LvCheck( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	WORD wLevel = (WORD)lua_tonumber( L, 3 );
	BOOL bRet;
	if( pszData[0] == '=' )
	{
		bRet = pChar->getAttr( ATTR_LV ) == wLevel;
	}
	else if( pszData[0] == '>' )
	{
		bRet = pChar->getAttr( ATTR_LV ) > wLevel;
	}
	else if( pszData[0] == '<' )
	{
		bRet = pChar->getAttr( ATTR_LV ) < wLevel;
	}
	else
	{
		E_LUACOMPARE;
		return 0;
	}
	
	lua_pushnumber( L, bRet );
	return 1;
T_E}

inline int lua_LvEqual( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wLevel = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_LV ) == wLevel );
	return 1;
T_E}

inline int lua_LvThan( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wLevel = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_LV ) > wLevel );
	return 1;
T_E}

inline int lua_GetCharMissionLevel( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wLevel = (WORD)pChar->getAttr( ATTR_LV );

	lua_pushnumber( L, wLevel );
	
	return 1;
T_E}

inline int lua_PfEqual( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byPf = (BYTE)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_JOB ) == byPf );
	return 1;
T_E}

inline int lua_HpCheck( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	WORD wHp = (WORD)lua_tonumber( L, 3 );
	BOOL bRet;
	if( pszData[0] == '=' )
	{
		bRet = pChar->getAttr( ATTR_HP ) == wHp;
	}
	else if( pszData[0] == '>' )
	{
		bRet = pChar->getAttr( ATTR_HP ) > wHp;
	}
	else if( pszData[0] == '<' )
	{
		bRet = pChar->getAttr( ATTR_HP ) < wHp;
	}
	else
	{
		E_LUACOMPARE;
		return 0;
	}

	lua_pushnumber( L, bRet );
	return 1;
T_E}

inline int lua_HpEqual( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wHP = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_HP ) == wHP );
	return 1;
T_E}

inline int lua_HpThan( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wHP = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_HP ) > wHP );
	return 1;
T_E}

inline int lua_SpCheck( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
		lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	WORD wSp = (WORD)lua_tonumber( L, 3 );
	BOOL bRet;
	if( pszData[0] == '=' )
	{
		bRet = pChar->getAttr( ATTR_SP ) == wSp;
	}
	else if( pszData[0] == '>' )
	{
		bRet = pChar->getAttr( ATTR_SP ) > wSp;
	}
	else if( pszData[0] == '<' )
	{
		bRet = pChar->getAttr( ATTR_SP ) < wSp;
	}
	else
	{
		E_LUACOMPARE;
		return 0;
	}

	lua_pushnumber( L, bRet );
	return 1;
T_E}

inline int lua_SpEqual( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wSP = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_SP ) == wSP );
	return 1;
T_E}

inline int lua_SpThan( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wSP = (WORD)lua_tonumber( L, 2 );
	lua_pushnumber( L, pChar->getAttr( ATTR_SP ) > wSP );
	return 1;
T_E}

//inline int lua_HasRandMission( lua_State* L )
//{T_B
//	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 );
//	if( !bValid )
//	{
//		E_LUAPARAM;
//		return 0;
//	}
//
//	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
//	if( !pChar )
//	{
//		E_LUANULL;
//		return 0;
//	}
//
//	lua_pushnumber( L, ( pChar->HasRandMission() ) ? LUA_TRUE : LUA_FALSE );
//	
//	return 1;
//T_E}

inline int lua_HasRandMission( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasRandMission( wID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	
	return 1;
T_E}

inline int lua_AddRandMission( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 10 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 ) &&
				  lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && lua_isnumber( L, 6 ) && lua_isnumber( L, 7 ) &&
				  lua_isnumber( L, 8 ) && lua_isnumber( L, 9 ) && lua_isnumber( L, 10 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID	= (WORD)lua_tonumber( L, 2 );
	WORD wScriptID = (WORD)lua_tonumber( L, 3 );
	BYTE byType = (BYTE)lua_tonumber( L, 4 );
	BYTE byLevel = (BYTE)lua_tonumber( L, 5 );
	DWORD dwExp = (DWORD)lua_tonumber( L, 6 );
	DWORD dwMoney = (DWORD)lua_tonumber( L, 7 );
	USHORT sPrizeData = (USHORT)lua_tonumber( L, 8 );
	USHORT sPrizeType = (USHORT)lua_tonumber( L, 9 );
	BYTE byNumData = (BYTE)lua_tonumber( L, 10 );
	BOOL bRet = pChar->AddRandMission( wID, wScriptID, byType, byLevel, dwExp, dwMoney, sPrizeData, sPrizeType, byNumData );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	
	return 1;
T_E}

inline int lua_SetRandMissionData( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 9 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
				  lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && 
				  lua_isnumber( L, 6 ) && lua_isnumber( L, 7 ) && lua_isnumber( L, 8 ) &&
				  lua_isnumber( L, 9 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );

	MISSION_DATA data;
	data.wParam1 = (WORD)lua_tonumber( L, 4 );
	data.wParam2 = (WORD)lua_tonumber( L, 5 );
	data.wParam3 = (WORD)lua_tonumber( L, 6 );
	data.wParam4 = (WORD)lua_tonumber( L, 7 );
	data.wParam5 = (WORD)lua_tonumber( L, 8 );
	data.wParam6 = (WORD)lua_tonumber( L, 9 );

	BOOL bRet = pChar->SetRandMissionData( wRoleID, byIndex, data );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetRandMission( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );

	BYTE  byType, byLevel, byNumData;
	DWORD dwExp, dwMoney;
	USHORT sPrizeData, sPrizeType;
	BOOL bRet = pChar->GetRandMission( wID, byType, byLevel, dwExp, dwMoney, sPrizeData, sPrizeType, byNumData );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byType );
	lua_pushnumber( L, byLevel );
	lua_pushnumber( L, dwExp );
	lua_pushnumber( L, dwMoney );
	lua_pushnumber( L, sPrizeData );
	lua_pushnumber( L, sPrizeType );
	lua_pushnumber( L, byNumData );		
	
	return 8;
T_E}

inline int lua_GetRandMissionData( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	MISSION_DATA data;
	BOOL bRet = pChar->GetRandMissionData( wRoleID, byIndex, data );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, data.wParam1 );
	lua_pushnumber( L, data.wParam2 );
	lua_pushnumber( L, data.wParam3 );
	lua_pushnumber( L, data.wParam4 );
	lua_pushnumber( L, data.wParam5 );
	lua_pushnumber( L, data.wParam6 );

	return 7;
T_E}

inline int lua_TakeAllRandItem( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->TakeAllRandItem( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_TakeRandNpcItem( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 ) &&
				  lua_isstring( L, 4 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wNpcID = (WORD)lua_tonumber( L, 3 );
	const char* pszNpc = (char*)lua_tostring( L, 4 );
	if( !pszNpc )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->TakeRandNpcItem( wRoleID, wNpcID, pszNpc );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasRandMissionNpc( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 ) &&
				  lua_isnumber( L, 4 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wNpcID  = (WORD)lua_tonumber( L, 3 );
	WORD wAreaID  = (WORD)lua_tonumber( L, 4 );

	BOOL bRet = pChar->HasRandMissionNpc( wRoleID, wNpcID, wAreaID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasRandNpcItemFlag( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wNpcID  = (WORD)lua_tonumber( L, 3 );

	BOOL bRet = pChar->HasSendNpcItemFlag( wRoleID, wNpcID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_NoRandNpcItemFlag( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wNpcID  = (WORD)lua_tonumber( L, 3 );

	BOOL bRet = pChar->NoSendNpcItemFlag( wRoleID, wNpcID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_IsMisNeedItem( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->IsMisNeedItem( sItemID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_CompleteRandMissionCount( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->CompleteRandMission( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_FailureRandMissionCount( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->FailureRandMission( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ResetRandMissionCount( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->ResetRandMission( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}


inline int lua_ResetRandMissionNum( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->ResetRandMissionNum( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasRandMissionCount( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wCount  = (WORD)lua_tonumber( L, 3 );

	BOOL bRet = pChar->HasRandMissionCount( wRoleID, wCount );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_AddRandMissionNum( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );

	BOOL bRet = pChar->AddRandMissionNum( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_GetRandMissionCount( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wCount = 0;
	BOOL bRet = pChar->GetRandMissionCount( wRoleID, wCount );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wCount );

	return 2;
}

inline int lua_GetRandMissionNum( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	WORD wNum = 0;
	BOOL bRet = pChar->GetRandMissionNum( wRoleID, wNum );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wNum );

	return 2;
}

inline int lua_Hide( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	pChar->Hide();

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_Show( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	pChar->Show();

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_ConvoyNpc( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	WORD wNpcID = (WORD)lua_tonumber( L, 4 );
	BYTE byAiType = (BYTE)lua_tonumber( L, 5 );

	BOOL bRet = pChar->ConvoyNpc( wRoleID, byIndex, wNpcID, byAiType );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_ClearConvoyNpc( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}
	
	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	BOOL bRet = pChar->ClearConvoyNpc( wRoleID, byIndex );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_ClearAllConvoyNpc( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}
	
	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	
	BOOL bRet = pChar->ClearAllConvoyNpc( wRoleID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_HasConvoyNpc( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}
		
	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	BOOL bRet = pChar->HasConvoyNpc( wRoleID, byIndex );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_IsConvoyNpc( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && lua_isnumber( L, 3 ) &&
				  lua_isnumber( L, 4 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	 
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( pChar == NULL )
	{
		E_LUANULL;
		return 0;
	}

	WORD wRoleID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	WORD wNpcID = (WORD)lua_tonumber( L, 4 );
	BOOL bRet = pChar->IsConvoyNpc( wRoleID, byIndex, wNpcID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetSpawnPos( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	const char* pszCity = lua_tostring( L, 2 );
	if( !pszCity )
	{
		E_LUANULL;
		return 0;
	}

	pChar->SetBirthCity( pszCity );
	//pChar->SystemNotice( "%s%s", pChar->GetName(), pszCity );
	pChar->SystemNotice( RES_STRING(GM_CHARSCRIPT_CPP_00010), pChar->GetName(), pszCity );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
T_E}

inline int lua_IsSpawnPos( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	const char* pszCity = lua_tostring( L, 2 );
	if( !pszCity )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = strcmp( pszCity, pChar->GetBirthCity() ) == 0;

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SetProfession( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BYTE byPf = (BYTE)lua_tonumber( L, 2 );
	BOOL bRet = pChar->SetProfession( byPf );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetProfession( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L  ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byPf = (BYTE)pChar->getAttr( ATTR_JOB );

	lua_pushnumber( L, LUA_TRUE );
	lua_pushnumber( L, byPf );

	return 2;
T_E}

inline int lua_GetCategory( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BYTE byCat = (BYTE)pChar->GetCat();

	lua_pushnumber( L, LUA_TRUE );
	lua_pushnumber( L, byCat );

	return 2;
T_E}

inline int lua_GetCatAndPf( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BYTE byCat = (BYTE)pChar->GetCat();
	BYTE byPf  = (BYTE)pChar->getAttr( ATTR_JOB );
	lua_pushnumber( L, LUA_TRUE );
	lua_pushnumber( L, byCat );
	lua_pushnumber( L, byPf );

	return 3;
T_E}

inline int lua_IsCategory( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BYTE byCat = (BYTE)lua_tonumber( L, 2 );

	lua_pushnumber( L, ( byCat == pChar->GetCat() ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_SaveMissionData( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->SaveMissionData();

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}
//==========Begin=========================================================
inline int lua_HasFame( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwFame = (DWORD)lua_tonumber( L, 2 );

	lua_pushnumber( L, (DWORD)pChar->getAttr( ATTR_FAME ) >= dwFame ? LUA_TRUE : LUA_FALSE);

	return 1;	
T_E}

inline int lua_HasGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pChar->HasGuild() ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_HasNavyGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pChar->HasGuild() ? (pChar->GetValidGuildID() > 1 && pChar->GetValidGuildID() < 100) : FALSE ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_NoNavyGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pChar->HasGuild() ? (pChar->GetValidGuildID() < 1 || pChar->GetValidGuildID() >= 100) : TRUE ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_HasPirateGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pChar->HasGuild() ? (pChar->GetValidGuildID() > 99 && pChar->GetValidGuildID() < 200) : FALSE ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_NoPirateGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pChar->HasGuild() ? (pChar->GetValidGuildID() <= 99) : TRUE ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_CreateGuild( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ); 
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = Guild::lua_CreateGuild(pChar);
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}
inline int lua_ListAllGuild( lua_State* L )
{T_B
BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = Guild::lua_ListAllGuild( pChar);
	
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

//===========End========================================================
inline int lua_SetPkState( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	BYTE byPk = (BYTE)lua_tonumber( L, 2 );

	pChar->SetEnterGymkhana( (BOOL)byPk );

	lua_pushnumber( L, LUA_TRUE );

	return 1;	
T_E}

inline int lua_IsBoatFull( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = ( pChar->GetPlayer() ) ? pChar->GetPlayer()->IsBoatFull() : FALSE;
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;	
T_E}

inline int lua_CreateBoat( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBoat = (USHORT)lua_tonumber( L, 2 );
	USHORT sBerth = (USHORT)lua_tonumber( L, 3 );
	BOOL bRet = g_CharBoat.Create( *pChar, sBoat, sBerth );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetBoatID( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );;
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byIndex = (BYTE)lua_tonumber( L, 2 );
	DWORD dwBoatID;
	BOOL bRet = pChar->GetBoatID( byIndex, dwBoatID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, dwBoatID );

	return 2;
T_E}

inline int lua_IsNeedRepair( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	BOOL bRet = pChar->IsNeedRepair();

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_IsNeedSupply( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	BOOL bRet = pChar->IsNeedSupply();

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_RepairBoat( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	pChar->RepairBoat();

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_SupplyBoat( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	pChar->SupplyBoat();

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_BoatLuanchOut( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 6 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && 
				  lua_isnumber( L, 6 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byIndex = (BYTE)lua_tonumber( L, 2 );
	USHORT sBerth = (USHORT)lua_tonumber( L, 3 );
	USHORT sxPos = (USHORT)lua_tonumber( L, 4 );
	USHORT syPos = (USHORT)lua_tonumber( L, 5 );
	USHORT sDir = (USHORT)lua_tonumber( L, 6 );
	BOOL bRet = pChar->BoatLaunch( byIndex, sBerth, sxPos, syPos, sDir );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BoatBerth( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerth = (USHORT)lua_tonumber( L, 2 );
	USHORT sxPos = (USHORT)lua_tonumber( L, 3 );
	USHORT syPos = (USHORT)lua_tonumber( L, 4 );
	USHORT sDir = (USHORT)lua_tonumber( L, 5 );
	BOOL bRet = pChar->BoatBerth( sBerth, sxPos, syPos, sDir );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BoatBerthList( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 7 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				  lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && 
				  lua_isnumber( L, 6 ) && lua_isnumber( L, 7 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	DWORD dwNpcID = (DWORD)lua_tonumber( L, 2 );
	BYTE byType = (BYTE)lua_tonumber( L, 3 );
	USHORT sBerth = (USHORT)lua_tonumber( L, 4 );
	USHORT sxPos = (USHORT)lua_tonumber( L, 5 );
	USHORT syPos = (USHORT)lua_tonumber( L, 6 );
	USHORT sDir = (USHORT)lua_tonumber( L, 7 );
	//
	//BOOL bRet = pChar->BoatBerthList( dwNpcID, byType, sBerth, sxPos, syPos, sDir );
	BOOL bRet;

	//if(byType == mission::BERTH_TRADE_LIST)
	//{
	//	bRet = FALSE;
	//}
	//else
		bRet = pChar->BoatBerthList( dwNpcID, byType, sBerth, sxPos, syPos, sDir );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BoatTrade( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerth = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = pChar->BoatTrade( sBerth );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_BoatBuildCheck( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBoatID = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = !g_CharBoat.BoatLimit( *pChar, sBoatID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasAllBoatInBerth( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerthID = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasAllBoatInBerth( sBerthID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}


inline int lua_HasBoatInBerth( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerthID = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasBoatInBerth( sBerthID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasDeadBoatInBerth( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerthID = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = pChar->HasDeadBoatInBerth( sBerthID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_HasLuanchOut( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = ( pChar->GetPlayer() ) ? pChar->GetPlayer()->IsLuanchOut() : LUA_FALSE;

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
T_E}

inline int lua_GetSection( lua_State* L )
{T_B
	BOOL bValid = lua_gettop( L ) == 2 && lua_isnumber( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	USHORT sData1 = (USHORT)lua_tonumber( L, 1 );
	USHORT sData2 = (USHORT)lua_tonumber( L, 2 );

	lua_pushnumber( L, sData1/sData2 );

	return 1;
T_E}

inline int lua_ToDword( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_isnumber( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	DWORD dwData = (DWORD)lua_tonumber( L, 1 );

	lua_pushnumber( L, dwData );

	return 1;
}

inline int lua_PackBag( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 5 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) && 
				lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CCharacter* pBoat = (CCharacter*)lua_touserdata( L, 2 );
	if( !pChar || !pBoat )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 3 );
	USHORT sCount  = (USHORT)lua_tonumber( L, 4 );
	USHORT sPileID = (USHORT)lua_tonumber( L, 5 );
	USHORT sNumPack = 0;
	BOOL bRet = pChar->PackBag( *pBoat, sItemID, sCount, sPileID, sNumPack );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, sNumPack );

	return 2;
}

inline int lua_PackBagList( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
				lua_isnumber( L, 3 ) && lua_isnumber( L, 4 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sBerthID = (USHORT)lua_tonumber( L, 2 );
	BYTE byType = (BYTE)lua_tonumber( L, 3 );
	BYTE byLevel = (BYTE)lua_tonumber( L, 4 );

	BOOL bRet = pChar->BoatPackBagList( sBerthID, byType, byLevel );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_HasLeaveBagGrid( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sNum = (USHORT)lua_tonumber( L, 2 );

	BOOL bRet = pChar->HasLeaveBagGrid( sNum );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_AdjustTradeItemCess( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sCess = (USHORT)lua_tonumber( L, 2 );
	USHORT sData = (USHORT)lua_tonumber( L, 3 );

	BOOL bRet = pChar->AdjustTradeItemCess( sCess, sData );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_SetTradeItemLevel( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byData = (BYTE)lua_tonumber( L, 2 );

	BOOL bRet = pChar->SetTradeItemLevel( byData );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

// 20050730
inline int lua_TradeItemLevelCheck( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
					lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byData = (BYTE)lua_tonumber( L, 3 );
	BOOL bRet = FALSE;
	BYTE byLevel;
	if( pChar->GetTradeItemLevel( byLevel ) )
	{		
		if( pszData[0] == '>' )
		{
			bRet = byLevel > byData;
		}
		else if( pszData[0] == '<' )
		{
			bRet = byLevel < byData;
		}
		else if( pszData[0] == '=' )
		{
			bRet = byData == byLevel;
		}
		else
		{
			E_LUAPARAM;
			return 0;
		}
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

/* 
inline int lua_TradeItemLevelCheck( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byData = (BYTE)lua_tonumber( L, 3 );

	BOOL bRet;
	if( pszData[0] == '>' )
	{
		bRet = pChar->HasTradeItemLevel( byData );
	}
	else if( pszData[0] == '<' )
	{
		bRet = !pChar->HasTradeItemLevel( byData );
	}
	else if( pszData[0] == '=' )
	{
		BYTE byLevel;
		if( pChar->GetTradeItemLevel( byLevel ) )
		{
			bRet = byData == byLevel;
		}
		else
		{
			bRet = FALSE;
		}
	}
	else
	{
		E_LUAPARAM;
		return 0;
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}*/

inline int lua_TradeItemDataCheck( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
					lua_isnumber( L, 3 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sData = (USHORT)lua_tonumber( L, 3 );
	BYTE byLevel;
	USHORT sCess;
	BOOL bRet = pChar->GetTradeItemData( byLevel, sCess );
	if( bRet )
	{
		if( pszData[0] == '>' )
		{
			bRet = sCess > sData;
		}
		else if( pszData[0] == '<' )
		{
			bRet = sCess < sData;
		}
		else if( pszData[0] == '=' )
		{
			bRet = sCess == sData;
		}
		else
		{
			E_LUAPARAM;
			return 0;
		}
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_GetTradeItemData( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byLevel;
	USHORT sCess;
	BOOL bRet = pChar->GetTradeItemData( byLevel, sCess );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, byLevel );
	lua_pushnumber( L, sCess );

	return 3;
}

inline int lua_SetAttrChangeFlag( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	pChar->m_CChaAttr.ResetChangeFlag();

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_SyncBoat( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pBoat = (CCharacter*)lua_touserdata( L, 1 );
	if( !pBoat )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byType = (BYTE)lua_tonumber( L, 2 );

	pBoat->SyncBoatAttr( byType , false );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_SyncChar( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byType = (BYTE)lua_tonumber( L, 2 );

	pChar->SynAttr( byType );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_OpenGuildBank(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	
	if (!pChar )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenGuildBank();
	lua_pushnumber(L, (bRet) ? LUA_TRUE : LUA_FALSE);
	return 1;
}

inline int lua_OpenBank( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenBank( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenRepair( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenRepair( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenForge( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenForge( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

// Add by lark.li 20080514 begin
inline int lua_OpenLottery( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenLottery( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}
// End

inline int lua_OpenUnite( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenUnite( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenMilling( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenMilling( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenFusion( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenFusion( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenUpgrade( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenUpgrade( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenEidolonMetempsychosis( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	BOOL bRet = pChar->GetPlayer()->OpenEidolonMetempsychosis( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenEidolonFusion( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenEidolonFusion( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenItemTiChun( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenPurify( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenItemFix( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenFix( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenItemEnergy( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenEnergy( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenGMSend( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenGMSend( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenGMRecv( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenGMRecv( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenGetStone( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenGetStone( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_OpenTiger( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}

	BOOL bRet = pChar->GetPlayer()->OpenTiger( (CCharacter*)pTalk );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

// 
inline int lua_OpenHair(lua_State *L)
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	if(pChar->HasTradeAction()) // 
	{
		//LG("hair", "[%s], !\n", pChar->GetName());
		LG("hair", RES_STRING(GM_CHARSCRIPT_CPP_00011), pChar->GetName());
		lua_pushnumber( L, LUA_FALSE);
		return 1;
	}

	pChar->Prl_OpenHair();
	pChar->TradeAction(true); // 
	
	lua_pushnumber( L, LUA_TRUE);
	return 1;
}

inline int lua_Garner2GetWiner(lua_State *L)
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	WPacket 	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GARNER2_CGETORDER);
	pChar->ReflectINFof(pChar,l_wpk);

	lua_pushnumber( L, LUA_TRUE);
	return 1;
}

inline int lua_Garner2RequestReorder(lua_State *L)
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	if(pTalk)
	{
		CCharacter* pCha = pChar->m_submap->FindCharacter( pTalk->GetID(), pChar->GetShape().centre );
		if( pCha == NULL )
		{
			E_LUAPARAM;
			return 0;
		}
	}
	CCharacter *pMainCha = pChar->GetPlyMainCha();
	if(!pMainCha)
	{
		E_LUAPARAM;
		return 0;
	}

	short sItemID = 3849;
	short pos = -1;
	USHORT sNum = pMainCha->m_CKitbag.GetCapacity();
	SItemGrid *pGridCont;
	for( short i = 0; i < sNum; i++ )
	{
		pGridCont = pMainCha->m_CKitbag.GetGridContByID( i );
		if( pGridCont )
		{
			if( sItemID == pGridCont->sID )
			{
				pos = i;
				break;
			}
		}
	}
	if(-1 == pos)
		//pMainCha->SystemNotice(",,");
		pMainCha->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00012));
	else
		pMainCha->Cmd_Garner2_Reorder(pos);

	//WPacket 	l_wpk	=GETWPACKET();

	//pChar->ReflectINFof(pChar,l_wpk);

	lua_pushnumber( L, LUA_TRUE);
	return 1;
}

inline int lua_ListAuction(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
//add by ALLEN 2007-10-19
	g_AuctionSystem.ListAuction(pChar, pTalk);
	lua_pushnumber( L, LUA_TRUE);

	return 1;
}

//add by ALLEN 2007-10-19
inline int lua_StartAuction(lua_State* L)
{

	BOOL bValid = lua_gettop( L ) == 5 && lua_isnumber( L, 1 ) && lua_isstring( L, 2 ) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	short sItemID = (short)lua_tonumber(L, 1);
	cChar *szName = (cChar *)lua_tostring(L, 2);
	short sCount = (short)lua_tonumber(L, 3);
	long lBasePrice = (long)lua_tonumber(L, 4);
	long lMinBid = (long)lua_tonumber(L, 5);

	if( szName == NULL )
	{
		lua_pushnumber( L, LUA_FALSE);
		return 1;
	}

	if(!g_AuctionSystem.StartAuction(sItemID, std::string(szName), sCount, (uInt)lBasePrice, (uInt)lMinBid))
	{
		lua_pushnumber( L, LUA_FALSE);
	}
	else
	{
		lua_pushnumber( L, LUA_TRUE);
	}
		
	return 1;
}
//add by ALLEN 2007-10-19
inline int lua_EndAuction(lua_State* L)
{BOOL bValid = lua_gettop( L ) == 1 && lua_isnumber( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	short sItemID = (short)lua_tonumber(L, 1);

	if(!g_AuctionSystem.EndAuction(sItemID))
	{
		lua_pushnumber( L, LUA_FALSE);
	}
	else
	{
		lua_pushnumber( L, LUA_TRUE);
	}
	return 1;
}

// 
inline int lua_ListChallenge( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	
	game_db.ListChallenge( pChar );	
	lua_pushnumber( L, LUA_TRUE);
	return 1;
}

// 
inline int lua_HasGuildLevel( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	BYTE byLevel = (BYTE)lua_tonumber( L, 2 );

	BOOL bRet = game_db.HasGuildLevel( pChar, byLevel );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_IsTeamLeader( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	
	lua_pushnumber( L, ( pChar->IsTeamLeader() ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_GetChaBody( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, pChar->m_SChaPart.sTypeID );
	return 1;
}

inline int lua_GetMapActivePlayer( lua_State* L )//2006.10.12wsj
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap * pSubMap = (SubMap *)lua_touserdata(L,1);
	if(!pSubMap)
	{
		E_LUANULL;
		return 0;
	}
	lua_pushnumber(L,pSubMap->GetActivePlayer());
	return 1;
}
inline int lua_DealAllActivePlayerInMap( lua_State* L )//2006.10.12wsj
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring(L,2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap * pSubMap = (SubMap *)lua_touserdata(L,1);
	if(!pSubMap)
	{
		E_LUANULL;
		return 0;
	}
	const char * pfunctionname = lua_tostring(L,2);
	string strfun(pfunctionname);

	pSubMap->DealActivePlayer(strfun);
	lua_pushnumber(L,LUA_TRUE);
	return 1;
}

inline int lua_GetMapPlayer( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap * pSubMap = (SubMap *)lua_touserdata(L,1);
	if(!pSubMap)
	{
		E_LUANULL;
		return 0;
	}
	lua_pushnumber(L,pSubMap->GetPlayerNum());
	return 1;
}
inline int lua_DealAllPlayerInMap( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring(L,2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap * pSubMap = (SubMap *)lua_touserdata(L,1);
	if(!pSubMap)
	{
		E_LUANULL;
		return 0;
	}
	const char * pfunctionname = lua_tostring(L,2);
	string strfun(pfunctionname);

	pSubMap->DealPlayer(strfun);
	lua_pushnumber(L,LUA_TRUE);
	return 1;
}

inline int lua_IsGarnerWiner( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L,1);
	if(!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	if(pChar->IsPlayerCha())
	{
		lua_pushnumber(L,pChar->GetPlayer()->IsGarnerWiner());
	}

	return 1;
}

inline int lua_IsItemValid( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 1 && lua_isnumber(L, 1);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	short sItemID = (short)lua_tonumber(L, 1);

	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
	{
		lua_pushnumber(L,LUA_FALSE);
		return 0;
	}

	lua_pushnumber(L,LUA_TRUE);

	return 1;
}

inline int lua_GetItemP( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	short sPosID = (short)lua_tonumber(L, 2);

	if( !pCha )
	{
		E_LUANULL;
		return 0;
	}

	SItemGrid *pItem = pCha->m_CKitbag.GetGridContByID(sPosID);
	lua_pushlightuserdata(L, pItem);

	return 1;
}

inline int lua_GetEquipItemP( lua_State* L )
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	const short sPosID = (short)lua_tonumber(L, 2);

	if( !pCha )
	{
		E_LUANULL;
		return 0;
	}

	SItemGrid* pItem = pCha->GetEquipItem(sPosID);
	if (!pItem)
	{
		return 0;
	}
	
	lua_pushlightuserdata(L, pItem);

	return 1;
}

inline int lua_IsInTeam(lua_State* L)
{
	BOOL bValid = (lua_gettop(L) == 1 && lua_islightuserdata(L,1));
	if(!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );	
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	int ret = pChar->GetPlayer()->HasTeam() ? 1 : 0;

	lua_pushnumber(L, ret);
	return 1;
}

inline int lua_HasTeammate(lua_State* L)
{
	BOOL bValid = (lua_gettop(L) == 3 && lua_islightuserdata(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3));
	if(!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	bool bHigher = (((int)lua_tonumber(L,2) == 1) ? true : false);
	short dLevel = (short)lua_tonumber(L,3);
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	int ret = 0;
	if (!pChar->GetPlayer()->HasTeam())
	{
		ret = 0;
	}
	else
	{
		CPlayer *pTeamMember = NULL;
		short sLevel = (short)(pChar->GetLevel());
		pChar->GetPlayer()->BeginGetTeamPly();
		while(pTeamMember = pChar->GetPlayer()->GetNextTeamPly())
		{
			if(bHigher)
			{
				if(pTeamMember->GetMainCha()->GetLevel() >= (sLevel + dLevel))
				{
					ret = 1;
					break;
				}
			}
			else
			{
				if(pTeamMember->GetMainCha()->GetLevel() <= (sLevel - dLevel))
				{
					ret = 1;
					break;
				}
			}
		}
	}

	lua_pushnumber(L, ret);
	return 1;
}

inline int lua_SetCopySpecialInter( lua_State* L )//2006.10.12wsj
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	SubMap * pSubMap = (SubMap *)lua_touserdata(L,1);
	if(!pSubMap)
	{
		E_LUANULL;
		return 0;
	}
	int Interal = (int)lua_tonumber(L, 2);

	pSubMap->SetSpecialInter(Interal);

	lua_pushnumber(L,LUA_TRUE);
	return 1;
}

inline int lua_AddCreditX(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL;
		return 0;
	}

	long lCredit = (int)lua_tonumber(L, 2);
	pCha->SetCredit((long)pCha->GetCredit() + lCredit);
	pCha->SynAttr(enumATTRSYN_TASK);

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_AddMasterCredit(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL;
		return 0;
	}

	int nCredit = (int)lua_tonumber(L, 2);
	pCha->AddMasterCredit(nCredit);

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_DelCredit(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL;
		return 0;
	}

	long lCredit = (long)lua_tonumber(L, 2);
	long lCreditFinal = ((long)pCha->GetCredit() - lCredit >= 0) ? ((long)pCha->GetCredit() - lCredit) : 0;
	pCha->SetCredit(lCreditFinal);
	pCha->SynAttr(enumATTRSYN_TASK);

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_GetCredit(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL;
		return 0;
	}

	long lCredit = (long)pCha->GetCredit();

	lua_pushnumber( L, lCredit );

	return 1;
}

inline int lua_HasMaster(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL;
		return 0;
	}

	int ret = (pCha->GetMasterDBID() == 0) ? LUA_FALSE : LUA_TRUE;

	lua_pushnumber( L, ret );

	return 1;
}

inline int lua_GetMasterID(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if (!pCha)
	{
		E_LUANULL;
		return 0;
	}
	lua_pushnumber(L, pCha->GetMasterDBID());
	return 1;
}

inline int lua_GetRoleByID(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_isnumber(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}
	DWORD dwChaDBID = (DWORD)lua_tonumber(L, 1);
	CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = (CCharacter*)pPlayer->GetMainCha();
		lua_pushlightuserdata(L, (CCharacter*)pCha);
		return 1;
	}
	//lua_pushlightuserdata(L, NULL);
	return 0;
}

inline int lua_LifeSkillBegin(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) && lua_isnumber(L,3);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata( L, 2 );
	long  ltype = (long)lua_tonumber(L,3);
	if( !pChar || !pTalk )
	{
		E_LUANULL;
		return 0;
	}
	if(pTalk)
	{
		CCharacter* pCha = pChar->m_submap->FindCharacter( pTalk->GetID(), pChar->GetShape().centre );
		if( pCha == NULL )
		{
			E_LUAPARAM;
			return 0;
		}
	}

	if(pChar->m_CKitbag.IsPwdLocked())
	{
		//pChar->SystemNotice("");
		pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00013));
		return 1;
	}

	//add by ALLEN 2007-10-16
		if(pChar->IsReadBook())
	{
		//pChar->SystemNotice("");
		pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00014));
		return 1;
	}

	pChar->ForgeAction();
	WPacket 	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MC_LIFESKILL_BGING);
	WRITE_LONG(l_wpk,ltype);
	pChar->ReflectINFof(pChar,l_wpk);
	lua_pushnumber( L, LUA_TRUE );
	return 1;
}

inline int lua_ClearAllSubMapCha(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap* pSubmap = (SubMap*)lua_touserdata( L, 1 );
	if(pSubmap)
	{
		pSubmap->ClearAllCha();
	}
	lua_pushnumber( L, LUA_TRUE );
	return 1;
}

inline int lua_ClearAllSubMapMonster(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	SubMap* pSubmap = (SubMap*)lua_touserdata( L, 1 );
	if(pSubmap)
	{
		pSubmap->ClearAllMonster();
	}
	lua_pushnumber( L, LUA_TRUE );
	return 1;
}


inline int lua_ClearFightSkill(lua_State* L)
{
	const auto valid = lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2);
	if (!valid)
	{
		E_LUAPARAM;
		return 0;
	}

	auto pChar = static_cast<CCharacter*>(lua_touserdata(L, 1));
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}

	const auto skillID = static_cast<int>(lua_tonumber(L, 2));
	pChar->m_CSkillBag.SetChangeFlag(false);
	CSkillRecord* pCSkill = GetSkillRecordInfo(skillID);
	if (!pCSkill || (pCSkill->chFightType != enumSKILL_FIGHT) || !pCSkill->IsShow())
	{
		lua_pushnumber(L, 0);
		return 0;
	}

	SSkillGrid* pSkillGrid = pChar->m_CSkillBag.GetSkillContByID(skillID);
	if (!pSkillGrid)
	{
		lua_pushnumber(L, 0);
		return 0;
	}

	const auto skillPoint = pSkillGrid->chLv;
	if (!pChar->m_CSkillBag.Del(skillID))
	{
		lua_pushnumber(L, 0);
		return 0;
	}

	pChar->SkillRefresh();
	pChar->SynSkillBag(enumSYN_SKILLBAG_MODI);
	lua_pushnumber(L, skillPoint);
	return 1;
}

inline int lua_ClearAllFightSkill(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}

	pChar->m_CSkillBag.SetChangeFlag(false);

	int nSkillPoint{ 0 };
	{
		for (auto i = pChar->m_CSkillBag.GetSkillNum(); i-- > 0;)
		{
			auto skill = pChar->m_CSkillBag.GetSkillContByNum(i);
			if (!skill)
			{
				continue;
			}

			auto skillrecord = GetSkillRecordInfo(skill->sID);
			if (!skillrecord || skillrecord->chFightType != enumSKILL_FIGHT || !skillrecord->IsShow())
			{
				continue;
			}

			const auto skillLv = skill->chLv;
			if (pChar->m_CSkillBag.Del(skill->sID));
			{
				nSkillPoint += skillLv;
			}
		}
	}

	pChar->SkillRefresh();
	pChar->SynSkillBag(enumSYN_SKILLBAG_MODI);
	lua_pushnumber(L, nSkillPoint);
	return 1;
}

inline int lua_RefreshCha(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}

	pChar->SynSkillStateToEyeshot();
	pChar->SynKitbagNew(enumSYN_KITBAG_EQUIP);

	g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pChar, DOSTRING_PARAM_END);
	if (pChar->GetPlayer())
	{
		pChar->GetPlayer()->RefreshBoatAttr();
		pChar->SyncBoatAttr(enumATTRSYN_ITEM_MEDICINE);
	}
	pChar->SynAttrToSelf(enumATTRSYN_ITEM_MEDICINE);

	return 1;
}

inline int lua_ChangeJob(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber(L, 2);
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	long lJob = (long)lua_tonumber(L,2);
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	pChar->m_CChaAttr.ResetChangeFlag();
	pChar->setAttr(ATTR_JOB, lJob);
	pChar->SetBoatAttrChangeFlag(false);
	g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pChar, DOSTRING_PARAM_END);
	if (pChar->GetPlayer())
	{
		pChar->GetPlayer()->RefreshBoatAttr();
		pChar->SyncBoatAttr(enumATTRSYN_CHANGE_JOB);
	}
	pChar->SynAttrToSelf(enumATTRSYN_CHANGE_JOB);
	return 1;
}

inline int lua_IsChaStall(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	if(pChar->GetStallData())
	{
		lua_pushnumber( L, LUA_TRUE );
	}
	else
	{
		lua_pushnumber( L, LUA_FALSE );
	}
	
	return 1;
}

inline int lua_GetActName(lua_State* L)
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushstring(L, pChar->GetPlayer()->GetActName());

	return 1;
}


inline int lua_GetActID(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber(L, pChar->GetPlayer()->GetDBActId());

	return 1;
}

//  
inline int lua_GetExpState(lua_State *L)
{T_B
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);

    if(pCha)
    {
        lua_pushnumber(L, pCha->GetExpScale());
        return 1;
    }

    return 0;

T_E}


inline int lua_KillCha( lua_State *L )
{T_B
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);

    if(pCha)
    {
        if(pCha->IsPlayerCha())
        {
            lua_pushnumber(L, 0);
        }
        else
        {
            pCha->Free();
            lua_pushnumber(L, 1);
        }
    }
    else
    {
        lua_pushnumber(L, 0);
    }

    return 1;
T_E}

// kong@pkodev.net 09.22.2017
inline int lua_GetGmLv(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 1 && lua_islightuserdata(L, 1));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha) {
		lua_pushnumber(L, pCha->GetPlayer()->GetGMLev());
		return 1;
	}
	return 0;
T_E }

inline int lua_SetGmLv(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	int gmLv = (int)lua_tonumber(L, 2);
	if (pCha) {
		pCha->GetPlayer()->SetGMLev(gmLv);
		Square& chaPos = (Square&)pCha->GetShape();
		pCha->GetPlayer()->GetMainCha()->Cmd_EnterMap(pCha->GetBirthMap(), -1, chaPos.centre.x, chaPos.centre.y, 0);

		game_db.SaveGmLv(pCha->GetPlayer());
		return 1;
	}
	return 0;
T_E }

inline int lua_RequestClientPin(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	char action = lua_tonumber(L, 2);
	if (pCha) {
		pCha->requestType = action;
		pCha->requestPos.centre.x = pCha->GetShape().centre.x;
		pCha->requestPos.centre.y = pCha->GetShape().centre.y;
		WPacket l_wpk = GETWPACKET();
		WRITE_CMD(l_wpk, CMD_MC_REQUESTPIN);
		pCha->ReflectINFof(pCha, l_wpk);
		return 1;
	}
	return 0;
	T_E
}

inline int lua_DealAllInGuild(lua_State *L) {T_B
	BOOL bValid = ((lua_gettop(L) == 3 && lua_isstring(L, 3)) || lua_gettop(L) == 2) && lua_isstring(L, 2) && lua_isnumber(L, 1);
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	int guildID = lua_tonumber(L, 1);
	const char* luaFunc = lua_tostring(L, 2);
	
	if (lua_isstring(L, 3)){
		const char* luaParam = lua_tostring(L, 3);
		g_pGameApp->DealAllInGuild(guildID, luaFunc, luaParam);
	}else{
		g_pGameApp->DealAllInGuild(guildID, luaFunc, "");
	}
	
	return 0;
	T_E
}

inline int lua_GetPlayerByName(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 1 && lua_isstring(L, 1));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	const char* chaName = lua_tostring(L, 1);
	CCharacter *pCha = g_pGameApp->FindPlayerChaByNameLua(chaName);
	if(pCha) {
		lua_pushlightuserdata(L, (CCharacter*) pCha);
		return 1;
	}
	return 0;
T_E}

inline int lua_GetPlayerByActName(lua_State *L) {
	T_B
		BOOL bValid = (lua_gettop(L) == 1 && lua_isstring(L, 1));
	if (!bValid) {
		PARAM_ERROR
			return 0;
	}
	const char* chaName = lua_tostring(L, 1);
	CCharacter *pChas[3];
	int count = g_pGameApp->FindPlayerChaByActNameLua(chaName, pChas);
	if (count > 0) {
		for (int i = 0; i < count; i++){
			lua_pushlightuserdata(L, (CCharacter*)pChas[i]);
		}
		return count;
	}
	return 0;
	T_E
}


inline int lua_GetItemQuantity(lua_State* pLS) {
	T_B
		bool bSuccess = true;
	long ret = 0; 
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 1) {
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);
		if (!pSItem) {
			bSuccess = false;
			goto End;
		}
		ret = pSItem->sNum;
	}
End:
	if (bSuccess)
		lua_pushnumber(pLS, ret);
	else
		lua_pushnumber(pLS, 0);
	return 1;
T_E }

int lua_GetOriginalChaTypeID(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 1 && lua_islightuserdata(L, 1));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha) {
		lua_pushnumber(L, pCha->GetIcon());
		return 1;
	}
	return 0;
T_E }

int lua_TransformCha(lua_State *L) { T_B
	BOOL bValid = (lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	CCharacter *pAtt = (CCharacter*)lua_touserdata(L, 1);
	short mID = (short)lua_tonumber(L, 2);
	Square& chaPos = (Square&)pAtt->GetShape();
	pAtt->m_SChaPart.sTypeID = mID;
	pAtt->m_cat = mID;
	pAtt->GetPlayer()->GetMainCha()->Cmd_EnterMap(pAtt->GetBirthMap(), -1, chaPos.centre.x, chaPos.centre.y, 0);
	return 1;
T_E }

int lua_GetOnlineCount(lua_State *pLS) { T_B
	BEGINGETGATE();
	GateServer *pGateServer;

	int nCount = 0;
	while (pGateServer = GETNEXTGATE())
		nCount += GETPLAYERCOUNT(pGateServer);

	lua_pushnumber(pLS, nCount);
	return 1;
T_E }

int lua_PopupNotice( lua_State* L )
{T_B
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring( L , 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar || !pszData )
	{
		E_LUANULL;
		return 0;
	}
	pChar->PopupNotice( pszData );
	return 0;
T_E}

int lua_BanActRole( lua_State* L)
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L , 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* actName = pChar->GetPlayer()->GetActName();
	if (actName == NULL)
	{
		E_LUANULL;
		return 0;
	}
	//pChar->PopupNotice("Ban! Reason: Violation of server rules.");
	KICKPLAYER(pChar->GetPlayer(), 1);
	g_pGameApp->BanAccount(actName);
	return 0;
}

int lua_BanActName( lua_State* L)
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_isstring( L , 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	const char* actName = lua_tostring(L, 1);
	if (actName == NULL)
	{
		E_LUANULL;
		return 0;
	}
	g_pGameApp->BanAccount(actName);
	return 0;
}

int lua_UnbanAct( lua_State* L)
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_isstring( L , 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	const char* actName = lua_tostring(L, 1);
	if (actName == NULL)
	{
		E_LUANULL;
		return 0;
	}
	g_pGameApp->UnbanAccount(actName);
	return 0;
}

int lua_MuteCha( lua_State* L)
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L , 1 ) && lua_isnumber( L, 2) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pCCha = (CCharacter*)lua_touserdata( L,  1 );
	long lmuteTime = (long)lua_tonumber(L, 2);
	if (!pCCha || !lmuteTime)
	{
		E_LUANULL;
		return 0;
	}
	//g_pGameApp->UnbanAccount(pCCha->GetPlyMainCha(), );
	return 0;
}

inline int lua_ColourNotice( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber(L, 2) && lua_isstring( L , 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	DWORD rgb = (DWORD)lua_tonumber( L, 2);
	const char* pszData = lua_tostring( L, 3 );
	if( !pChar || !pszData || !rgb )
	{
		E_LUANULL;
		return 0;
	}
	pChar->ColourNotice( rgb, pszData );
	return 0;
}
//send daily buff packet info to client 
inline int lua_SendDailyBuffInfo(lua_State* L) {
	if (BOOL bValid = (lua_gettop(L) == 3 && lua_islightuserdata(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3)); !bValid)
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	const auto imgName = lua_tostring(L, 2);
	const auto LabelInfo = lua_tostring(L, 3);
	if (!pChar || !imgName || !LabelInfo)
	{
		E_LUANULL;
		return 0;
	}
	//send info to client 
	WPacket l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MC_RecDailyBuffInfo);
	WRITE_STRING(l_wpk, imgName);
	WRITE_STRING(l_wpk, LabelInfo);
	pChar->ReflectINFof(pChar, l_wpk);

}

// 
//force data save @mothannakh
inline int lua_ForcePlayerSave(lua_State* L) {
T_B
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}
	game_db.SavePlayer(pChar->GetPlayer(), enumSAVE_TYPE_TIMER);
	return 0;
T_E
}
//end


inline int lua_String2Item(lua_State* L){
	if (!((lua_gettop(L) == 2 || (lua_gettop(L) == 3 && lua_isnumber(L, 3))) && lua_islightuserdata(L, 1) && lua_isstring(L, 2))){
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	const char* pszData = lua_tostring(L, 2);
	SItemGrid SGridCont;
	String2Item(pszData, &SGridCont);
	short gridID = defKITBAG_DEFPUSH_POS;

	if (lua_gettop(L) == 3 && lua_tonumber(L,3) == 1){
		pChar->m_pCKitbagTmp->SetChangeFlag(false);
		Short sPushPos = defKITBAG_DEFPUSH_POS;
		Short sPushRet = pChar->m_pCKitbagTmp->Push(&SGridCont, sPushPos);
		pChar->SynKitbagTmpNew(enumSYN_KITBAG_SYSTEM);
	}else{
		pChar->KbPushItem(true, true, &SGridCont, gridID);
		pChar->SynKitbagNew(enumSYN_KITBAG_SYSTEM);
	}
	return 1;
}

inline int lua_SetGlobalRates(lua_State* L) {
	if (lua_gettop(L) != 2 || !lua_isnumber(L,1) || !lua_isnumber(L,2)) {
		return 0;
	}
	float droprate = lua_tonumber(L, 1);
	float exprate = lua_tonumber(L, 2);
	if (droprate && exprate) {
		g_pGameApp->SetGlobalRates(droprate, exprate);
		return 1;
	}
	return 0;
}

inline int lua_GetPlyDropRate(lua_State* L) {
T_B
	if (lua_gettop(L) != 1 || !lua_islightuserdata(L, 1)) {
		return 0;
	}
	CCharacter* pCha = (CCharacter*)lua_touserdata(L, 1);
	lua_pushnumber(L, pCha->GetDropRate());
	return 1;
T_E
}

inline int lua_GetPlyExpRate(lua_State* L) {
	T_B
		if (lua_gettop(L) != 1 || !lua_islightuserdata(L, 1)) {
			return 0;
		}
	CCharacter* pCha = (CCharacter*)lua_touserdata(L, 1);
	lua_pushnumber(L, pCha->GetExpRate());
	return 1;
	T_E
}

inline int lua_RemoveOfflineMode(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_islightuserdata(L, 1);
	if(!bValid) {
		E_LUAPARAM;
		return 0;
	}
	
	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha) {
		lua_pushnumber(L, LUA_FALSE);
		return 0;
	}

	CPlayer *pPly = pCha->GetPlayer();
	if(!pPly) {
		lua_pushnumber(L, LUA_FALSE);
		return 0;
	}

	if(!pCha->IsOfflineMode()) {
		lua_pushnumber(L, LUA_FALSE);
		return 0;		
	}

	g_pGameApp->ReleaseGamePlayer(pPly);
	lua_pushnumber(L, LUA_TRUE);
	return 1;
}

BOOL RegisterCharScript()
{T_B
	lua_State *L = g_pLuaState;

	// 
	REGFN(GetTickCount);
	REGFN(Msg);
	REGFN(Exit);
	REGFN(LG);
	REGFN(GetSection);
	REGFN(ToDword);

	// 
	REGFN(SetMap);
	REGFN(SetMapGuildWar);
	REGFN(AddTrigger);
	REGFN(AddMission);
	REGFN(HasMission);
	REGFN(GetMisScriptID);
	REGFN(SetMissionComplete);
	REGFN(SetMissionFailure);
	REGFN(HasMisssionFailure);
	REGFN(IsMissionFull);
	REGFN(DeleteTrigger);
	REGFN(ClearTrigger);
	REGFN(ClearMission);
	REGFN(SetFlag);
	REGFN(ClearFlag);
	REGFN(IsFlag);
	REGFN(IsValidFlag);
	REGFN(SetRecord);
	REGFN(ClearRecord);
	REGFN(IsRecord);
	REGFN(IsValidRecord);
	
	REGFN(IsMissionState);
	REGFN(GetNumMission);
	REGFN(GetMissionInfo);
	REGFN(GetCharMission);
	REGFN(GetNextMission);
	REGFN(AddMissionState);
	REGFN(ResetMissionState);
	REGFN(GetMissionState);
	REGFN(SetMissionPage);
	REGFN(GetMissionPage);
	REGFN(SetMissionTempInfo);
	REGFN(GetMissionTempInfo);

	REGFN(AddSkill);
	REGFN(AddExp);
	REGFN(AddLifeExp);
	REGFN(AddSailExp);
	REGFN(AddExpAndType);
	REGFN(AddMoney);
	REGFN(TakeMoney);
	REGFN(HasMoney);
	REGFN(HasCancelMissionMoney);
	REGFN(TakeCancelMissionMoney);
	REGFN(MakeItem);
	REGFN(GiveItem);
	REGFN(GiveItemX);
	REGFN(GiveItemY);
	REGFN(HasLeaveBagTempGrid);
	REGFN(TakeItem);
	REGFN(TakeItemBagTemp);
	REGFN(HasItem);
	REGFN(BankHasItem);
	REGFN(BagTempHasItem);
	REGFN(EquipHasItem);
	REGFN(IsEquip);
	REGFN(KitbagLock);
	REGFN(GetNumItem);
	REGFN(GetNeedItemCount);
	REGFN(IsMisNeedItem);
	REGFN(HasLeaveBagGrid);
	REGFN(IsItemValid);
	REGFN(GetItemP);
	REGFN(GetEquipItemP);

	// 
	REGFN(HasRandMission);
	REGFN(AddRandMission);
	REGFN(SetRandMissionData);
	REGFN(GetRandMission);
	REGFN(GetRandMissionData);
	REGFN(HasRandMissionNpc);
	REGFN(HasRandNpcItemFlag);
	REGFN(NoRandNpcItemFlag);
	REGFN(TakeRandNpcItem);
	REGFN(TakeAllRandItem);

	// 
	REGFN(CompleteRandMissionCount);
	REGFN(FailureRandMissionCount);
	REGFN(AddRandMissionNum);
	REGFN(ResetRandMissionCount);
	REGFN(ResetRandMissionNum);
	REGFN(HasRandMissionCount);
	REGFN(GetRandMissionCount);
	REGFN(GetRandMissionNum);

	// 
	REGFN(ReAll);
	REGFN(ReAllHp);
	REGFN(ReHp);
	REGFN(ReAllSp);
	REGFN(ReSp);

	// 
	REGFN(SetSpawnPos);
	
	// 
	REGFN(IsSpawnPos);

	// 
	REGFN(SetProfession);

	// 
	REGFN(Hide);
	REGFN(Show);

	// 
	REGFN(GetCharMissionLevel);
	REGFN(LvEqual);
	REGFN(LvThan);
	REGFN(HpEqual);
	REGFN(HpThan);
	REGFN(SpEqual);
	REGFN(SpThan);
	REGFN(PfEqual);
	REGFN(LvCheck);
	REGFN(HpCheck);
	REGFN(SpCheck);
	REGFN(IsCategory);
	REGFN(HasFame);	

	// 
	REGFN(GetProfession);
	REGFN(GetCategory);
	REGFN(GetCatAndPf);
	REGFN(GetChaBody);
	
	// NPC
	REGFN(ConvoyNpc);
	REGFN(ClearConvoyNpc);
	REGFN(ClearAllConvoyNpc);
	REGFN(HasConvoyNpc);
	REGFN(IsConvoyNpc);

	REGFN(SetPkState);

	// 
	REGFN(SaveMissionData); 

	// 
	REGFN(HasGuild);
	REGFN(CreateGuild);
	REGFN(ListAllGuild);
	REGFN(ListChallenge);
	REGFN(HasGuildLevel);
	REGFN(HasPirateGuild);
	REGFN(NoPirateGuild);
	REGFN(HasNavyGuild);
	REGFN(NoNavyGuild);

	// 
	REGFN(IsBoatFull);
	REGFN(CreateBoat);
	REGFN(BoatLuanchOut);
	REGFN(BoatBerth);
	REGFN(BoatBerthList);
	REGFN(BoatTrade);
	REGFN(BoatBuildCheck);
	REGFN(HasAllBoatInBerth);
	REGFN(HasBoatInBerth);
	REGFN(HasDeadBoatInBerth);
	REGFN(HasLuanchOut);
	REGFN(GetBoatID);
	REGFN(RepairBoat);
	REGFN(SupplyBoat);
	REGFN(IsNeedSupply);
	REGFN(IsNeedRepair);

	// 
	REGFN(AdjustTradeItemCess);
	REGFN(SetTradeItemLevel);
	REGFN(TradeItemLevelCheck);
	REGFN(GetTradeItemData);
	REGFN(TradeItemDataCheck);

	// 
	REGFN(PackBag);
	REGFN(PackBagList);

	// 
	REGFN(SetAttrChangeFlag);
	REGFN(SyncBoat);
	REGFN(SyncChar);

	// 
	REGFN(OpenBank);

	REGFN(OpenRepair);
	REGFN(OpenForge);
	// Add by lark.li 20080514
	REGFN(OpenLottery);
	//Add by sunny.sun 20080529
	//Begin
	REGFN(GetTicketItemno);
	REGFN(GetTicketIssue);
	REGFN(GetSItemGrid);
	//End
	REGFN(OpenUnite);
	REGFN(OpenMilling);
	REGFN(OpenFusion);
	REGFN(OpenUpgrade);
	REGFN(OpenEidolonMetempsychosis);
	REGFN(OpenEidolonFusion);
	REGFN(OpenItemTiChun);
	REGFN(OpenItemFix);
	REGFN(OpenItemEnergy);
	REGFN(OpenGetStone);
	REGFN(OpenTiger);
	REGFN(OpenGMSend);
	REGFN(OpenGMRecv);

	// 
	REGFN(OpenHair);


	// 
	REGFN(CheckFusionItem);
	REGFN(FusionItem);
	
	// 
	REGFN(IsTeamLeader);
	REGFN(IsInTeam);
	REGFN(HasTeammate);

	//
	REGFN(AddCreditX);
	REGFN(AddMasterCredit);
	REGFN(DelCredit);
	REGFN(GetCredit);
	REGFN(HasMaster);

	//
	REGFN(GetMapPlayer);
	REGFN(DealAllPlayerInMap);
	REGFN(GetMapActivePlayer);
	REGFN(DealAllActivePlayerInMap);
	REGFN(SetCopySpecialInter);

	//
	REGFN(Garner2GetWiner);
	REGFN(Garner2RequestReorder);
	REGFN(ClearAllSubMapCha);
	REGFN(ClearAllSubMapMonster);
	REGFN(IsGarnerWiner);
	//
	REGFN(LifeSkillBegin);
	REGFN(ClearAllFightSkill);
	REGFN(ClearFightSkill);

	REGFN(RefreshCha);
	REGFN(IsChaStall);
	REGFN(ChangeJob);

	REGFN(ListAuction);
	REGFN(StartAuction);
	REGFN(EndAuction);

	REGFN(GetActName);
	REGFN(GetActID);
    //  , 
    REGFN(GetExpState);

    REGFN(KillCha);

	// 
	REGFN(GetResString);

	REGFN(GetGmLv);
	REGFN(SetGmLv);
	REGFN(GetPlayerByName);
	REGFN(GetMasterID);
	REGFN(GetRoleByID);
	REGFN(DealAllInGuild);
	REGFN(GetItemQuantity);
	REGFN(GetOriginalChaTypeID);
	REGFN(TransformCha);
	REGFN(GetOnlineCount); // kong@pkodev.net 09.22.2017 end
	REGFN(PopupNotice); // kong@pkodev.net 10.10.2017
	REGFN(BanActRole);
	REGFN(BanActName);
	REGFN(UnbanAct);
	REGFN(ColourNotice);
	REGFN(RequestClientPin);

	REGFN(OpenGuildBank);
	
	REGFN(String2Item);

	REGFN(GetPlayerByActName);
	REGFN(ForcePlayerSave);	// @mothannakh
	REGFN(SendDailyBuffInfo) //@mothannakh
	REGFN(SetGlobalRates);
	REGFN(GetPlyDropRate);
	REGFN(GetPlyExpRate);

	REGFN(RemoveOfflineMode);
	// AI
	RegisterLuaAI(g_pLuaState);
	

	// 
	RegisterLuaGameLogic(g_pLuaState);

	return TRUE;
T_E}
