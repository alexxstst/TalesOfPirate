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
	// 
	WRITE_LONG(pk, m_ID);							// world ID
	WRITE_LONG(pk, m_lHandle);
	WRITE_LONG(pk, m_pCItemRecord->lID);			// ID
	WRITE_LONG(pk, GetShape().centre.x);			// x
	WRITE_LONG(pk, GetShape().centre.y);			// y
	WRITE_SHORT(pk, m_sAngle);					// 
	WRITE_SHORT(pk, m_SGridContent.sNum);			// 
	//
	WRITE_CHAR(pk, m_chSpawType);
	WRITE_LONG(pk, m_lFromEntityID);
	// 
	WriteEventInfo(pk);

	pCMainCha->ReflectINFof(this,pk);//
}

void CItem::OnEndSeen(CCharacter *pCMainCha)
{
	WPACKET pk =GETWPACKET();
	WRITE_CMD(pk, CMD_MC_ITEMENDSEE);
	WRITE_LONG(pk, m_ID);				//ID
	pCMainCha->ReflectINFof(this,pk);	//
}

void CItem::Run(dbc::uLong ulCurTick)
{
	if (m_ulProtID != 0)
		if (m_ulProtOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulProtOnTick) // 
			m_ulProtID = 0;

	if (m_ulOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulOnTick)
	{
		// 
		CItemRecord* pItem = m_pCItemRecord;
		if( pItem != NULL )
		{
			// 
			if( pItem->sType == enumItemTypeBoat )
			{
				game_db.SaveBoatDelTag( this->GetGridContent()->GetDBParam( enumITEMDBP_INST_ID ), 1 );
			}
		}
		if (!m_submap)
			ToLogService("Item disappear error", "item {}(ID {}HANDLE {}position[{} {}]) when it disappear find the map is null", GetName(), GetID(), GetHandle(), GetPos().x, GetPos().y);
		else
		{
			Free();
		}
	}
}
