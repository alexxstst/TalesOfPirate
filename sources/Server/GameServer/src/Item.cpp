//=============================================================================
// FileName: Item.cpp
// Creater: ZhangXuedong
// Date: 2004.09.21
// Comment: CItem class
//=============================================================================
#include "stdafx.h"
#include "Item.h"
#include "GameCommon.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "gamedb.h"

_DBC_USING

CItem::CItem()
{
	chValid = 0;
	m_pCItemRecord = 0;
	m_SGridContent.sID = 0;
	m_lFromEntityID = 0;
	m_chSpawType = enumITEM_APPE_NATURAL;
}

void CItem::Initially()
{
	Entity::Initially();

	chValid = 0;
	m_pCItemRecord = 0;
	m_SGridContent.sID = 0;
	m_chSpawType = enumITEM_APPE_NATURAL;
	m_ulStartTick = GetTickCount();
	m_ulOnTick = g_Config.m_lItemShowTime * 1000;
	m_ulProtOnTick = g_Config.m_lItemProtTime * 1000;
	m_ulProtID = 0;
	m_ulProtHandle = 0;
	m_chProtType = enumITEM_PROT_OWN;
}

void CItem::Finally()
{
	if (m_submap)
		m_submap->GoOut(this);
	Entity::Finally();
}

void CItem::OnBeginSeen(CCharacter *pCMainCha)
{
	WPACKET pk =GETWPACKET();
	WRITE_CMD(pk, CMD_MC_ITEMBEGINSEE);
	// 基本数据
	WRITE_LONG(pk, m_ID);							// world ID
	WRITE_LONG(pk, m_lHandle);
	WRITE_LONG(pk, m_pCItemRecord->lID);			// ID
	WRITE_LONG(pk, GetShape().centre.x);			// 当前x位置
	WRITE_LONG(pk, GetShape().centre.y);			// 当前y位置
	WRITE_SHORT(pk, m_sAngle);					// 方向
	WRITE_SHORT(pk, m_SGridContent.sNum);			// 个数
	//
	WRITE_CHAR(pk, m_chSpawType);
	WRITE_LONG(pk, m_lFromEntityID);
	// 事件信息
	WriteEventInfo(pk);

	pCMainCha->ReflectINFof(this,pk);//通告
}

void CItem::OnEndSeen(CCharacter *pCMainCha)
{
	WPACKET pk =GETWPACKET();
	WRITE_CMD(pk, CMD_MC_ITEMENDSEE);
	WRITE_LONG(pk, m_ID);				//ID
	pCMainCha->ReflectINFof(this,pk);	//通告
}

void CItem::Run(dbc::uLong ulCurTick)
{
	if (m_ulProtID != 0)
		if (m_ulProtOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulProtOnTick) // 保护时间消失
			m_ulProtID = 0;

	if (m_ulOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulOnTick)
	{
		// 判断是否船长证明道具
		CItemRecord* pItem = m_pCItemRecord;
		if( pItem != NULL )
		{
			// 判断丢弃船长证明
			if( pItem->sType == enumItemTypeBoat )
			{
				game_db.SaveBoatDelTag( this->GetGridContent()->GetDBParam( enumITEMDBP_INST_ID ), 1 );
			}
		}
		if (!m_submap)
			//LG("道具消失错误", "道具 %s(ID %u，HANDLE %u，位置[%d %d]) 在消失时发现其地图为空\n", GetName(), GetID(), GetHandle(), GetPos().x, GetPos().y);
			LG("Item disappear error", "item %s(ID %u，HANDLE %u，position[%d %d]) when it disappear find the map is null\n", GetName(), GetID(), GetHandle(), GetPos().x, GetPos().y);
		else
		{
			Free();
		}
	}
}