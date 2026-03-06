//=============================================================================
// FileName: EntityAlloc.cpp
// Creater: ZhangXuedong
// Date: 2005.01.18
// Comment: EntityAlloc class

// modifed by knight.gong 2005.5.16. (To alloc all entities by the template of allocer)
//=============================================================================
#include "stdafx.h"
#include "EntityAlloc.h"

char g_szEntiAlloc[256] = "EntityAlloc";

CEntityAlloc::CEntityAlloc(long lChaNum, long lItemNum, long lTNpcNum)
{T_B
	// 
	m_ChaAlloc.create( lChaNum, defENTI_ALLOC_TYPE_CHA );
	m_ItemAlloc.create( lItemNum, defENTI_ALLOC_TYPE_ITEM );
	m_TalkNpcAlloc.create( lTNpcNum, defENTI_ALLOC_TYPE_TNPC );
	m_BerthAlloc.create( 1000, defENTI_ALLOC_TYPE_ENTBERTH );
	m_ResourceAlloc.create( 1000, defENTI_ALLOC_TYPE_ENTRESOURCE );
T_E}

CEntityAlloc::~CEntityAlloc()
{T_B
	m_ChaAlloc.clear();
	m_ItemAlloc.clear();
	m_TalkNpcAlloc.clear();
	m_BerthAlloc.clear();
	m_ResourceAlloc.clear();
T_E}

//=============================================================================
// 
//=============================================================================
CCharacter* CEntityAlloc::GetNewCha()
{T_B
	CCharacter* pChar = m_ChaAlloc.alloc();
	if( !pChar )
	{		
		//LG(g_szEntiAlloc, "msg,");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00010));
		return NULL;
	}
	return pChar;
T_E}

//=============================================================================
// 
//=============================================================================
CItem* CEntityAlloc::GetNewItem()
{T_B
	CItem* pItem = m_ItemAlloc.alloc();
	if( !pItem )
	{
		//LG( g_szEntiAlloc, "msg,");
		LG( g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00011));
		return NULL;
	}
	return pItem;
T_E}

//=============================================================================
// NPC
//=============================================================================
mission::CTalkNpc* CEntityAlloc::GetNewTNpc()
{T_B
	mission::CTalkNpc* pNpc = m_TalkNpcAlloc.alloc();
	if( !pNpc )
	{
		//LG(g_szEntiAlloc, "msgNPC,NPC");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00012));
		return NULL;
	}
	return pNpc;
T_E}

//=============================================================================
// 
//=============================================================================
mission::CEventEntity* CEntityAlloc::GetEventEntity( BYTE byType )
{
	switch( byType )
	{
	case mission::BASE_ENTITY:			// 
		{
		}
		break;

	case mission::RESOURCE_ENTITY:		// 
		{
			return m_ResourceAlloc.alloc();
		}
		break;

	case mission::TRANSIT_ENTITY:		// 
		{
		}
		break;

	case mission::BERTH_ENTITY:			// 
		{
			return m_BerthAlloc.alloc();
		}
		break;
	default:
		{
			//LG(g_szEntiAlloc, "msgType[%d]", byType);
			LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00013), byType);
			return NULL;
		}
		break;
	}
	//LG(g_szEntiAlloc, "msgType[%d]", byType);
	LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00014), byType);
	return NULL;
}

//=============================================================================
// 
//=============================================================================
Entity* CEntityAlloc::GetEntity(long lID)
{T_B
	long	lType = lID & 0xff000000;
	long	lEntiID = lID & 0x00ffffff;

	if (lType == defENTI_ALLOC_TYPE_CHA)
	{
		return m_ChaAlloc.getinfo( lEntiID );
	}
	else if (lType == defENTI_ALLOC_TYPE_ITEM)
	{
		return m_ItemAlloc.getinfo( lEntiID );
	}
	else if (lType == defENTI_ALLOC_TYPE_TNPC)
	{
		return m_TalkNpcAlloc.getinfo( lEntiID );
	}
	else if( lType == defENTI_ALLOC_TYPE_ENTBERTH )
	{
		return m_BerthAlloc.getinfo( lEntiID );
	}
	else if( lType == defENTI_ALLOC_TYPE_ENTRESOURCE )
	{
		return m_ResourceAlloc.getinfo( lEntiID );
	}
	else
		return 0;
T_E}

//=============================================================================
// 
//=============================================================================
void CEntityAlloc::ReturnEntity(long lID)
{T_B
	long	lType = lID & 0xff000000;
	long	lEntiID = lID & 0x00ffffff;

	if (lType == defENTI_ALLOC_TYPE_CHA)
	{
		return m_ChaAlloc.destroy( lEntiID );
	}
	else if (lType == defENTI_ALLOC_TYPE_ITEM)
	{
		return m_ItemAlloc.destroy( lEntiID );
	}
	else if (lType == defENTI_ALLOC_TYPE_TNPC)
	{
		return m_TalkNpcAlloc.destroy( lEntiID );
	}
	else if( lType == defENTI_ALLOC_TYPE_ENTBERTH )
	{
		return m_BerthAlloc.destroy( lEntiID );
	}
	else if( lType == defENTI_ALLOC_TYPE_ENTRESOURCE )
	{
		return m_ResourceAlloc.destroy( lEntiID );
	}
T_E}

//=============================================================================
//=============================================================================

//=============================================================================
// 
//=============================================================================
CPlayer* CPlayerAlloc::GetNewPly()
{T_B
	CPlayer* pCPly = m_PlyAlloc.alloc();
	if( !pCPly )
	{		
		//LG(g_szEntiAlloc, "msg,");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00015));
		return NULL;
	}
	return pCPly;
T_E}
