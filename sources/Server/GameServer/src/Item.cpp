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
	// Типизированная сериализация: предмет появился в поле зрения
	auto pk = net::msg::serialize(net::msg::McItemCreateMessage{
		m_ID, static_cast<int64_t>(m_lHandle), static_cast<int64_t>(m_pCItemRecord->lID),
		static_cast<int64_t>(GetShape().centre.x), static_cast<int64_t>(GetShape().centre.y),
		static_cast<int64_t>(m_sAngle), static_cast<int64_t>(m_SGridContent.sNum),
		static_cast<int64_t>(m_chSpawType), static_cast<int64_t>(m_lFromEntityID),
		{static_cast<int64_t>(GetID()), IsCharacter() ? 1LL : 2LL,
		 static_cast<int64_t>(GetEvent().GetID()), std::string(GetEvent().GetName())}
	});
	pCMainCha->ReflectINFof(this,pk);
}

void CItem::OnEndSeen(CCharacter *pCMainCha)
{
	// Типизированная сериализация: предмет исчез из поля зрения
	auto pk = net::msg::serialize(net::msg::McItemDestroyMessage{m_ID});
	pCMainCha->ReflectINFof(this,pk);
}

void CItem::Run(dbc::uLong ulCurTick)
{
	if (m_ulProtID != 0)
		if (m_ulProtOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulProtOnTick) // ����ʱ����ʧ
			m_ulProtID = 0;

	if (m_ulOnTick != 0 && ulCurTick - m_ulStartTick >= m_ulOnTick)
	{
		// �ж��Ƿ񴬳�֤������
		CItemRecord* pItem = m_pCItemRecord;
		if( pItem != NULL )
		{
			// �ж϶�������֤��
			if( pItem->sType == enumItemTypeBoat )
			{
				game_db.SaveBoatDelTag( this->GetGridContent()->GetDBParam( enumITEMDBP_INST_ID ), 1 );
			}
		}
		if (!m_submap)
			//LG("������ʧ����", "���� %s(ID %u��HANDLE %u��λ��[%d %d]) ����ʧʱ�������ͼΪ��\n", GetName(), GetID(), GetHandle(), GetPos().x, GetPos().y);
			ToLogService("errors", LogLevel::Error, "item {}(ID {}��HANDLE {}��position[{} {}]) when it disappear find the map is null", GetName(), GetID(), GetHandle(), GetPos().x, GetPos().y);
		else
		{
			Free();
		}
	}
}