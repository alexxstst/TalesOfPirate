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
	// ����ʵ���ڴ�
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
// ȡһ�����õĽ�ɫ��
//=============================================================================
CCharacter* CEntityAlloc::GetNewCha()
{
	CCharacter* pChar = m_ChaAlloc.alloc();
	if( !pChar )
	{		
		//LG(g_szEntiAlloc, "msg�����ɫ�ڴ�ʱ����,�����ӽ�ɫ�ڴ棡����");
		ToLogService("common", LogLevel::Error, "Character memory alloc error, need to add character memory!");
		return NULL;
	}
	return pChar;
}

//=============================================================================
// ȡһ�����õĵ��ߡ�
//=============================================================================
CItem* CEntityAlloc::GetNewItem()
{
	CItem* pItem = m_ItemAlloc.alloc();
	if( !pItem )
	{
		//LG( g_szEntiAlloc, "msg��������ڴ�ʱ����,�����ӵ����ڴ棡����");
		ToLogService("common", LogLevel::Error, "Item memory alloc error, need to add item memory!");
		return NULL;
	}
	return pItem;
}

//=============================================================================
// ȡһ�����õĶԻ�NPC��
//=============================================================================
mission::CTalkNpc* CEntityAlloc::GetNewTNpc()
{
	mission::CTalkNpc* pNpc = m_TalkNpcAlloc.alloc();
	if( !pNpc )
	{
		//LG(g_szEntiAlloc, "msg����Ի�NPC�ڴ�ʱ����,�����ӶԻ�NPC�ڴ棡����");
		ToLogService("common", LogLevel::Error, "TalkNPC memory alloc error, need to add TalkNPC memory!");
		return NULL;
	}
	return pNpc;
}

//=============================================================================
// ȡһ�����õĶԻ��¼�ʵ�塣
//=============================================================================
mission::CEventEntity* CEntityAlloc::GetEventEntity( BYTE byType )
{
	switch( byType )
	{
	case mission::BASE_ENTITY:			// ����ʵ��
		{
		}
		break;

	case mission::RESOURCE_ENTITY:		// ��Դʵ��
		{
			return m_ResourceAlloc.alloc();
		}
		break;

	case mission::TRANSIT_ENTITY:		// ����ʵ��
		{
		}
		break;

	case mission::BERTH_ENTITY:			// ͣ��ʵ��
		{
			return m_BerthAlloc.alloc();
		}
		break;
	default:
		{
			//LG(g_szEntiAlloc, "msgδ֪�������¼�ʵ�崴�����ͣ�Type[%d]", byType);
			ToLogService("common", LogLevel::Error, "Unknown event entity creation type, Type[{}]", byType);
			return NULL;
		}
		break;
	}
	//LG(g_szEntiAlloc, "msg�����¼�ʵ���ڴ�ʱ����������Type[%d]", byType);
	ToLogService("common", LogLevel::Error, "Event entity memory alloc error, Type[{}]", byType);
	return NULL;
}

//=============================================================================
// ȡһ����Чʵ��
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
// �ͷ�һ����Чʵ��
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
// ȡһ�����õ���ҡ�
//=============================================================================
CPlayer* CPlayerAlloc::GetNewPly()
{
	CPlayer* pCPly = m_PlyAlloc.alloc();
	if( !pCPly )
	{		
		//LG(g_szEntiAlloc, "msg��������ڴ�ʱ����,����������ڴ棡����");
		ToLogService("common", LogLevel::Error, "Player memory alloc error, need to add player memory!");
		return NULL;
	}
	return pCPly;
}
