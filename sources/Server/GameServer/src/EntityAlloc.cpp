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
	// 分配实体内存
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
// 取一个闲置的角色。
//=============================================================================
CCharacter* CEntityAlloc::GetNewCha()
{
	CCharacter* pChar = m_ChaAlloc.alloc();
	if( !pChar )
	{		
		//LG(g_szEntiAlloc, "msg请求角色内存时出错,请增加角色内存！！！");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00010));
		return NULL;
	}
	return pChar;
}

//=============================================================================
// 取一个闲置的道具。
//=============================================================================
CItem* CEntityAlloc::GetNewItem()
{
	CItem* pItem = m_ItemAlloc.alloc();
	if( !pItem )
	{
		//LG( g_szEntiAlloc, "msg请求道具内存时出错,请增加道具内存！！！");
		LG( g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00011));
		return NULL;
	}
	return pItem;
}

//=============================================================================
// 取一个闲置的对话NPC。
//=============================================================================
mission::CTalkNpc* CEntityAlloc::GetNewTNpc()
{
	mission::CTalkNpc* pNpc = m_TalkNpcAlloc.alloc();
	if( !pNpc )
	{
		//LG(g_szEntiAlloc, "msg请求对话NPC内存时出错,请增加对话NPC内存！！！");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00012));
		return NULL;
	}
	return pNpc;
}

//=============================================================================
// 取一个闲置的对话事件实体。
//=============================================================================
mission::CEventEntity* CEntityAlloc::GetEventEntity( BYTE byType )
{
	switch( byType )
	{
	case mission::BASE_ENTITY:			// 基本实体
		{
		}
		break;

	case mission::RESOURCE_ENTITY:		// 资源实体
		{
			return m_ResourceAlloc.alloc();
		}
		break;

	case mission::TRANSIT_ENTITY:		// 传送实体
		{
		}
		break;

	case mission::BERTH_ENTITY:			// 停泊实体
		{
			return m_BerthAlloc.alloc();
		}
		break;
	default:
		{
			//LG(g_szEntiAlloc, "msg未知的请求事件实体创建类型！Type[%d]", byType);
			LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00013), byType);
			return NULL;
		}
		break;
	}
	//LG(g_szEntiAlloc, "msg请求事件实体内存时出错！！！Type[%d]", byType);
	LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00014), byType);
	return NULL;
}

//=============================================================================
// 取一个有效实体
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
// 释放一个有效实体
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
// 取一个闲置的玩家。
//=============================================================================
CPlayer* CPlayerAlloc::GetNewPly()
{
	CPlayer* pCPly = m_PlyAlloc.alloc();
	if( !pCPly )
	{		
		//LG(g_szEntiAlloc, "msg请求玩家内存时出错,请增加玩家内存！！！");
		LG(g_szEntiAlloc, RES_STRING(GM_GAMEAPP_CPP_00015));
		return NULL;
	}
	return pCPly;
}
