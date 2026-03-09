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
{
	// 
	m_ChaAlloc.create( lChaNum, defENTI_ALLOC_TYPE_CHA );
	m_ItemAlloc.create( lItemNum, defENTI_ALLOC_TYPE_ITEM );
	m_TalkNpcAlloc.create( lTNpcNum, defENTI_ALLOC_TYPE_TNPC );
	m_BerthAlloc.create( 1000, defENTI_ALLOC_TYPE_ENTBERTH );
	m_ResourceAlloc.create( 1000, defENTI_ALLOC_TYPE_ENTRESOURCE );
}

CEntityAlloc::~CEntityAlloc()
{
	m_ChaAlloc.clear();
	m_ItemAlloc.clear();
	m_TalkNpcAlloc.clear();
	m_BerthAlloc.clear();
	m_ResourceAlloc.clear();
}

//=============================================================================
// 
//=============================================================================
CCharacter* CEntityAlloc::GetNewCha()
{
	CCharacter* pChar = m_ChaAlloc.alloc();
	if( !pChar )
	{		
		//LG(g_szEntiAlloc, "msg,");
		ToLogService(g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00010));
		return NULL;
	}
	return pChar;
}

//=============================================================================
// 
//=============================================================================
CItem* CEntityAlloc::GetNewItem()
{
	CItem* pItem = m_ItemAlloc.alloc();
	if( !pItem )
	{
		//LG( g_szEntiAlloc, "msg,");
		ToLogService( g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00011));
		return NULL;
	}
	return pItem;
}

//=============================================================================
// NPC
//=============================================================================
mission::CTalkNpc* CEntityAlloc::GetNewTNpc()
{
	mission::CTalkNpc* pNpc = m_TalkNpcAlloc.alloc();
	if( !pNpc )
	{
		//LG(g_szEntiAlloc, "msgNPC,NPC");
		ToLogService(g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00012));
		return NULL;
	}
	return pNpc;
}

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
			ToLogService(g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00013));
			return NULL;
		}
		break;
	}
	//LG(g_szEntiAlloc, "msgType[%d]", byType);
	ToLogService(g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00014));
	return NULL;
}

//=============================================================================
// 
//=============================================================================
Entity* CEntityAlloc::GetEntity(long lID)
{
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
}

//=============================================================================
// 
//=============================================================================
void CEntityAlloc::ReturnEntity(long lID)
{
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
}

//=============================================================================
//=============================================================================

//=============================================================================
// 
//=============================================================================
CPlayer* CPlayerAlloc::GetNewPly()
{
	CPlayer* pCPly = m_PlyAlloc.alloc();
	if( !pCPly )
	{		
		//LG(g_szEntiAlloc, "msg,");
		ToLogService(g_szEntiAlloc, "{}", RES_STRING(GM_GAMEAPP_CPP_00015));
		return NULL;
	}
	return pCPly;
}
