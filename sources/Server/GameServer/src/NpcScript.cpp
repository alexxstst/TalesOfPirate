// NpcScript.cpp Created by knight-gongjian 2004.11.30.
//---------------------------------------------------------
#include "stdafx.h"
#include "NpcScript.h"
#include "Npc.h"
#include "WorldEudemon.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include <NpcRecord.h>
#include <CharacterRecord.h>
#include <assert.h>
#include "lua_gamectrl.h"
//---------------------------------------------------------
_DBC_USING
using namespace mission;

net::WPacket g_WritePacket;

// ���籨�Ķ�ȡ
inline int lua_GetPacket( lua_State *L )
{
	g_WritePacket = g_gmsvr->GetWPacket();
	lua_pushlightuserdata( L, &g_WritePacket );
	return 1;
}

inline int lua_ReadByte( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket *)lua_touserdata( L, 1 );
	BYTE byData(-1);
	if( pPacket )
	{
		//byData = pPacket->ReadInt64();
        byData = (*pPacket).ReadInt64();
	}
	else
	{
		E_LUANULL;
	}
	lua_pushnumber( L, byData );
	return 1;
}

inline int lua_ReadWord( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket *)lua_touserdata( L, 1 );
	WORD wData(-1);
	if( pPacket )
	{
		//wData = pPacket->ReadInt64();
        wData = (*pPacket).ReadInt64();
	}
	else
	{
		E_LUANULL;
	}
	lua_pushnumber( L, wData );
	return 1;
}

inline int lua_ReadDword( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket *)lua_touserdata( L, 1 );
	DWORD dwData(-1);
	if( pPacket )
	{
		//dwData = pPacket->ReadInt64();
        dwData = (*pPacket).ReadInt64();
	}
	else
	{
		E_LUANULL;
	}
	lua_pushnumber( L, dwData );
	return 1;
}

inline int lua_ReadString( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket *)lua_touserdata( L, 1 );
	std::string pszData;
	if( pPacket )
	{
		//pszData = pPacket->ReadString();
        pszData = (*pPacket).ReadString();
	}
	else
	{
		E_LUANULL;
	}
	//lua_pushstring( L, ( pszData ) ? pszData : "��Ч�ַ�ָ�룡" );
	lua_pushstring( L, ( !pszData.empty() ) ? pszData.c_str() : "Ineffective char pointer��" );
	return 1;
}

inline int lua_ReadCmd( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket *)lua_touserdata( L, 1 );
	WORD wData(-1);
	if( pPacket )
	{
		//wData = pPacket->ReadCmd();
        wData = (*pPacket).ReadCmd();
	}
	else
	{
		E_LUANULL;
	}
	lua_pushnumber( L, wData );
	return 1;
}

inline int lua_WriteByte( lua_State *L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid ) 
	{
		E_LUAPARAM;
		return 0;
	}

	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 1 );	
	if( pPacket )
	{
		BYTE byData = (BYTE)lua_tonumber( L, 2 );
		//pPacket->WriteInt64( byData );
        (*pPacket).WriteInt64(byData);
	}
	else
	{
		E_LUANULL;
	}
	return 0;
}

inline int lua_WriteWord( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 1 );	
	if( pPacket )
	{
		WORD wData = (WORD)lua_tonumber( L, 2 );
		//pPacket->WriteInt64( wData );
        (*pPacket).WriteInt64(wData);
	}
	else
	{
		E_LUANULL;
	}
	return 0;
}

inline int lua_WriteDword( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 1 );	
	if( pPacket )
	{
		DWORD dwData = (DWORD)lua_tonumber( L, 2 );
		//pPacket->WriteInt64( dwData );
        (*pPacket).WriteInt64(dwData);
	}
	else
	{
		E_LUANULL;

	}
	return 0;
}

inline int lua_WriteString( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 1 );
	const char* pszData = lua_tostring( L, 2 );
	if( pPacket && pszData )
	{
		//pPacket->WriteString( pszData );
        (*pPacket).WriteString(pszData);
	}
	else
	{
		E_LUANULL;
	}
	return 0;
}

inline int lua_WriteCmd( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 1 );	
	if( pPacket )
	{
		WORD wData = (WORD)lua_tonumber( L, 2 );
		//pPacket->WriteCmd( wData );
        (*pPacket).WriteCmd(wData);
	}
	else
	{
		E_LUANULL;

	}
	return 0;
}

inline int lua_SendPacket( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 2 );
	if( !pChar || !pPacket )
	{
		E_LUANULL;
		return 0;
	}

	pChar->ReflectINFof( pChar, *pPacket );
	return 0;
}

inline int lua_SynPacket( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	net::WPacket* pPacket = (net::WPacket *)lua_touserdata( L, 2 );
	if( !pChar || !pPacket )
	{
		E_LUANULL;
		return 0;
	}

	pChar->NotiChgToEyeshot( *pPacket );
	return 0;
}

inline int lua_GetCharID( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
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
	lua_pushnumber( L, pChar->GetID() );
	return 1;
}

inline int lua_GetCharName( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
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
	lua_pushstring( L, pChar->GetName() );
	return 1;
}

inline int lua_GetMonsterName( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_isnumber( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	USHORT sMonsterID = (USHORT)lua_tonumber( L, 1 );
	
	//char szName[64] = "δ֪";
	char szName[64];
	strncpy( szName, RES_STRING(GM_NPCSCRIPT_CPP_00001), 64 - 1 );

	CChaRecord* pRec = GetChaRecordInfo( sMonsterID );
	if( pRec )
	{
		strncpy( szName, pRec->szName, 64 - 1 );
	}

	lua_pushstring( L, szName );

	return 1;
}

inline int lua_GetItemName( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_isnumber( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	USHORT sItemID = (USHORT)lua_tonumber( L, 1 );

	//char szItem[64] = "δ֪";
	char szItem[64];
	strncpy( szItem, RES_STRING(GM_NPCSCRIPT_CPP_00001), 64 - 1 );

	CItemRecord* pRec = GetItemRecordInfo( sItemID );
	if( pRec )
	{
		strncpy( szItem, pRec->szName, 64 - 1 );
	}

	lua_pushstring( L, szItem );

	return 1;
}


inline int lua_GetAreaName( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_isnumber( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	USHORT sAreaID = (USHORT)lua_tonumber( L, 1 );
	//char szArea[128] = "δ֪����";
	char szArea[128];
	strncpy( szArea, RES_STRING(GM_NPCSCRIPT_CPP_00002), 128 - 1 );

	CAreaInfo* pInfo = (CAreaInfo*)GetAreaInfo( sAreaID );
	if( pInfo )
	{
		strncpy( szArea, pInfo->szDataName, 128 - 1 );
	}

	lua_pushstring( L, szArea );

	return 1;
}

inline int lua_GetMapName( lua_State* L )
{
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
	
	SubMap* pMap = pChar->GetSubMap();
	if( !pMap )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushstring( L, pMap->GetName() );
	return 1;
}

inline int lua_MoveTo( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L,  1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) && lua_isstring( L, 4 ) );
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
	
	long x = (long)lua_tonumber( L, 2 );
	long y = (long)lua_tonumber( L, 3 );
	x *= 100;
	y *= 100;
	const char* pszData = lua_tostring( L, 4 );
	pChar->SwitchMap( pChar->GetSubMap(), pszData, x, y );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_MoveCity( lua_State* L )
{
	int	nParamNum = lua_gettop( L );
	BOOL bValid = FALSE;
	if (nParamNum == 2)
	{
		if (lua_islightuserdata( L,  1 ) && lua_isstring( L, 2 ) )
			bValid = TRUE;
	}
	else if (nParamNum == 3)
	{
		if (lua_islightuserdata( L,  1 ) && lua_isstring( L, 2 ) && lua_isnumber( L, 3 ) )
			bValid = TRUE;
	}
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L,  1 );
	const char* pszData = lua_tostring( L, 2 );
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}
	Long	lMapCpyNO = 0;
	if (nParamNum == 3)
		lMapCpyNO = (Long)lua_tonumber(L, 3);
	lMapCpyNO -= 1;
	
	pChar->MoveCity( pszData, lMapCpyNO );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_Rand( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	DWORD dwMax = (DWORD)lua_tonumber( L, 1 );
	DWORD dwRand = ( dwMax > 0 ) ? rand()%dwMax : 0;

	lua_pushnumber( L, dwRand );

	return 1;
}

// �ű����Ժ���
inline int lua_DebugInfo( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	DWORD dwData = (BYTE)lua_tonumber( L, 1 );
	return 0;
}

inline int lua_BickerNotice( lua_State* L )
{
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

	pChar->BickerNotice( pszData );
	return 0;
}

inline int lua_SystemNotice( lua_State* L )
{
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

	pChar->SystemNotice( pszData );
	return 0;
}

inline int lua_SynTigerString( lua_State* L )
{
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

	pChar->SynTigerString( pszData );
	return 0;
}

inline int lua_SafeBuy( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 4 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) );
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

	WORD wItemID = (WORD)lua_tonumber( L, 2 );
	BYTE byIndex = (BYTE)lua_tonumber( L, 3 );
	BYTE byCount = (BYTE)lua_tonumber( L, 4 );
	DWORD dwMoney;
	BOOL bRet = pChar->SafeBuy( wItemID, byCount, byIndex, dwMoney );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, dwMoney );

	return 2;
}

inline int lua_ExchangeReq( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 7 && lua_islightuserdata( L, 1 ) && lua_islightuserdata( L, 2 ) &&
				lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && lua_isnumber( L, 6 )
				&& lua_isnumber( L, 7 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	CNpc* pNpc = (CNpc*)lua_touserdata( L, 2 );
	short sSrcID = (short)lua_tonumber( L, 3 );
	short sSrcNum = (short)lua_tonumber( L, 4 );
	short sTarID = (short)lua_tonumber( L, 5 );
	short sTarNum = (short)lua_tonumber( L, 6 );
	short sTimeNum = (short)lua_tonumber( L, 7 );

	pChar->ExchangeReq(sSrcID, sSrcNum, sTarID, sTarNum);

	return 1;
}

inline int lua_SafeSale( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) &&
					lua_isnumber( L, 3 ) );
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
	BYTE byCount = (BYTE)lua_tonumber( L, 3 );
	WORD wItemID;
	DWORD dwMoney;
	BOOL bRet = pChar->SafeSale( byIndex, byCount, wItemID, dwMoney );
	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wItemID );
	lua_pushnumber( L, dwMoney );

	return 3;
}

inline int lua_SafeSaleGoods( lua_State* L )
{
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

	DWORD dwBoatID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byIndex = (BYTE)lua_tonumber( L, 3 );
	BYTE  byCount = (BYTE)lua_tonumber( L, 4 );
	DWORD dwMoney = (DWORD)lua_tonumber( L, 5 );
	WORD  wItemID;
	BOOL bRet = pChar->SafeSaleGoods( dwBoatID, byIndex, byCount, wItemID, dwMoney );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wItemID );
	lua_pushnumber( L, dwMoney );

	return 3;
}

inline int lua_SafeBuyGoods( lua_State* L )
{
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

	DWORD dwBoatID = (DWORD)lua_tonumber( L, 2 );
	WORD  wItemID = (WORD)lua_tonumber( L, 3 );
	BYTE  byIndex = (BYTE)lua_tonumber( L, 4 );
	BYTE  byCount = (BYTE)lua_tonumber( L, 5 );
	DWORD dwMoney = (DWORD)lua_tonumber( L, 6 );

	BOOL bRet = pChar->SafeBuyGoods( dwBoatID, wItemID, byCount, byIndex, dwMoney );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, dwMoney );

	return 2;
}

inline int lua_GetSaleGoodsItem( lua_State* L )
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

	DWORD dwBoatID = (DWORD)lua_tonumber( L, 2 );
	BYTE  byIndex = (BYTE)lua_tonumber( L, 3 );
	WORD  wItemID;
	BOOL bRet = pChar->GetSaleGoodsItem( dwBoatID, byIndex, wItemID );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, wItemID );

	return 2;
}

inline int lua_SetNpcScriptID( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sID = (USHORT)lua_tonumber( L, 2 );
	pNpc->SetScriptID( sID );
	return 0;
}

inline int lua_GetScriptID( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sID = pNpc->GetScriptID();
	lua_pushnumber( L, ( sID != -1 ) ? LUA_TRUE : LUA_FALSE );
	lua_pushnumber( L, sID );
	return 2;
}

inline int lua_FindNpc( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_isstring( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	const char* pszNpc = lua_tostring( L, 1 );
	CNpc* pNpc = NULL;
	try { pNpc = g_pGameApp->FindNpc( pszNpc ); }
	catch(...) { ToLogService("common", "findnpc error��exception!" ); }

	if( !pNpc )
	{
		return 0;
	}

	USHORT sID = pNpc->GetScriptID();
	lua_pushnumber( L, ( sID != -1 ) ? LUA_TRUE : LUA_FALSE );
	lua_pushlightuserdata( L, pNpc );
	lua_pushnumber( L, sID );
	return 3;
}

inline int lua_ReloadNpcInfo( lua_State* L )
{
	//LoadScript();
	//if( g_pGameApp->ReloadNpcInfo( *this ) )
	//{
	//}
	return 0;
}

inline int lua_SetNpcHasMission( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc )
	{
		E_LUANULL;
		return 0;
	}

	BYTE byRet = (BYTE)lua_tonumber( L, 2 );
	pNpc->SetNpcHasMission( byRet );
	return 0;
}

inline int lua_GetNpcHasMission( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( pNpc->GetNpcHasMission() ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_IsInArea( lua_State* L )
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

	USHORT sAreaID = (USHORT)lua_tonumber( L, 2 );
	BOOL bRet = (USHORT)pChar->GetIslandID() == sAreaID;

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_IsInMap( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 6 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && lua_isnumber( L, 6 ));
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	const char* pszMap = lua_tostring( L, 2 );
	if( !pChar || !pChar->GetSubMap() || !pszMap )
	{
		E_LUANULL;
		return 0;
	}
	DWORD dwxPos = (DWORD)lua_tonumber( L, 3 );
	DWORD dwyPos = (DWORD)lua_tonumber( L, 4 );
	WORD  wWith  = (WORD)lua_tonumber( L, 5 );
	WORD  wHeight= (WORD)lua_tonumber( L, 6 );
	BOOL  bRet = FALSE;
	bRet = strcmp( pszMap, pChar->GetSubMap()->GetName() ) == 0;
	if( bRet )
	{
		const Point& pt = pChar->GetPos();
		if( (DWORD)pt.x > dwxPos && (DWORD)pt.y > dwyPos && (DWORD)pt.x < dwxPos + wWith && (DWORD)pt.y < dwyPos + wHeight )
			bRet = TRUE;
	}

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_IsMapChar( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	const char* pszMap = lua_tostring( L, 2 );
	if( !pChar || !pChar->GetSubMap() || !pszMap )
	{
		E_LUANULL;
		return 0;
	}

	lua_pushnumber( L, ( strcmp( pszMap, pChar->GetSubMap()->GetName() ) == 0 ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_IsMapNpc( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 3 && lua_islightuserdata( L, 1 ) && lua_isstring( L, 2 ) && 
					lua_isnumber( L, 3 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	const char* pszMap = lua_tostring( L, 2 );
	if( !pNpc || !pszMap )
	{
		E_LUANULL;
		return 0;
	}

	USHORT sNpcID = (USHORT)lua_tonumber( L, 3 );

	lua_pushnumber( L, ( pNpc->IsMapNpc( pszMap, sNpcID ) ) ? LUA_TRUE : LUA_FALSE );
	return 1;
}

inline int lua_AddNpcTrigger( lua_State* L )
{
	BOOL bValid = ( lua_gettop( L ) == 7 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) && 
					lua_isnumber( L, 3 ) && lua_isnumber( L, 4 ) && lua_isnumber( L, 5 ) && 
					lua_isnumber( L, 6 ) && lua_isnumber( L, 7 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc ) 
	{
		E_LUANULL;
		return 0;
	}

	WORD wID = (WORD)lua_tonumber( L, 2 );
	BYTE e   = (BYTE)lua_tonumber( L, 3 );
	WORD wParam1 = (WORD)lua_tonumber( L, 4 );
	WORD wParam2 = (WORD)lua_tonumber( L, 5 );
	WORD wParam3 = (WORD)lua_tonumber( L, 6 );
	WORD wParam4 = (WORD)lua_tonumber( L, 7 );

	BOOL bRet = pNpc->AddNpcTrigger( wID, (mission::TRIGGER_EVENT)e, wParam1, wParam2, wParam3, wParam4 );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

inline int lua_SetActive( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_islightuserdata( L, 1 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	CNpc* pNpc = (CNpc*)lua_touserdata( L, 1 );
	if( !pNpc )
	{
		E_LUANULL;
		return 0;
	}

	pNpc->SetEyeshotAbility( true );

	lua_pushnumber( L, LUA_TRUE );

	return 1;
}

inline int lua_GetEudemon( lua_State* L )
{
	lua_pushnumber( L, LUA_TRUE );
	lua_pushlightuserdata( L, &g_WorldEudemon );
	return 2;
}

inline int lua_SummonNpc( lua_State* L )
{
	BOOL bValid = lua_gettop( L ) == 4 && lua_isnumber( L, 1 ) && lua_isnumber( L, 2 ) && lua_isstring( L, 3 ) &&
				  lua_isnumber( L, 4 );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}

	BYTE byMapID = (BYTE)lua_tonumber( L, 1 );
	USHORT sAreaID = (USHORT)lua_tonumber( L, 2 );
	const char* pszNpc = lua_tostring( L, 3 );
	USHORT sTime = (USHORT)lua_tonumber( L, 4 );

	BOOL bRet = g_pGameApp->SummonNpc( byMapID, sAreaID, pszNpc, sTime );

	lua_pushnumber( L, ( bRet ) ? LUA_TRUE : LUA_FALSE );

	return 1;
}

BOOL lua_ChaPlayEffect(lua_State* L)
{
	BOOL bValid = ( lua_gettop( L ) == 2 && lua_islightuserdata( L, 1 ) && lua_isnumber( L, 2 ) );
	if( !bValid )
	{
		E_LUAPARAM;
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata( L, 1 );
	int nEffectID = (int)lua_tonumber(L, 2);
	if( !pChar )
	{
		E_LUANULL;
		return 0;
	}

	// Типизированная сериализация: визуальный эффект на персонаже
	auto l_wpk = net::msg::serialize(net::msg::McChaPlayEffectMessage{pChar->GetID(), (int64_t)nEffectID});

	pChar->NotiChgToEyeshot( l_wpk );
	return 1;
}

inline int RegisterNpcScript()
{
	lua_State* L = g_pLuaState;

	REGFN(ReloadNpcInfo);
	REGFN(FindNpc);

	// npc���纯��ע��
	REGFN(ReadCmd);
	REGFN(ReadByte);
	REGFN(ReadWord);
	REGFN(ReadDword);
	REGFN(ReadString);

	REGFN(WriteCmd);
	REGFN(WriteByte);
	REGFN(WriteWord);
	REGFN(WriteDword);
	REGFN(WriteString);

	REGFN(GetPacket);
	REGFN(SendPacket);
	REGFN(SynPacket);
	REGFN(GetCharID);
	REGFN(MoveTo);
	REGFN(MoveCity);
	REGFN(GetAreaName);
	REGFN(GetMapName);
	REGFN(GetCharName);
	REGFN(GetItemName);
	REGFN(GetMonsterName);
	REGFN(Rand);

	REGFN(SafeSale);
	REGFN(SafeBuy);
	REGFN(SafeSaleGoods);
	REGFN(SafeBuyGoods);
	REGFN(GetSaleGoodsItem);
	REGFN(ExchangeReq);

	REGFN(SetNpcScriptID);
	REGFN(GetScriptID);
	REGFN(SetNpcHasMission);
	REGFN(GetNpcHasMission);
	REGFN(IsMapChar);
	REGFN(IsMapNpc);
	REGFN(IsInMap);
	REGFN(IsInArea);
	REGFN(AddNpcTrigger);
	REGFN(SetActive);
	REGFN(GetEudemon);
	REGFN(SummonNpc);
	REGFN(ChaPlayEffect);

	REGFN(DebugInfo);
	REGFN(SystemNotice);
	REGFN(SynTigerString);
	REGFN(BickerNotice);

	return TRUE;
}





