//=============================================================================
// FileName: StateCell.cpp
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map State Cell
//=============================================================================
#include "stdafx.h"
#include "StateCell.h"
#include "SubMap.h"

CChaListNode* CStateCell::AddCharacter(CCharacter *pCCha, bool bIn)
{
	CChaListNode	*pNode = g_pGameApp->m_ChaListHeap.Get();
	pNode->m_pCCha = pCCha;
	pNode->m_bIn = bIn;
	if (bIn)
	{
		pNode->m_pCLast = 0;
		if (pNode->m_pCNext = m_pCChaIn)
			m_pCChaIn->m_pCLast = pNode;
		m_pCChaIn = pNode;

		pNode->m_pCEntStateNode = pCCha->EnterStateCell(this, pNode, true); // ʵ���¼���ڵĹ�����Ԫ
	}
	else
	{
		pNode->m_pCLast = 0;
		if (pNode->m_pCNext = m_pCChaCross)
			m_pCChaCross->m_pCLast = pNode;
		m_pCChaCross = pNode;

		pNode->m_pCEntStateNode = pCCha->EnterStateCell(this, pNode); // ʵ���¼���ڵĹ�����Ԫ
	}
	m_lChaNum++;

	// ���ɫ����״̬
	if (m_CSkillState.GetStateNum() > 0)
	{
		pCCha->m_CChaAttr.ResetChangeFlag();
		pCCha->m_CSkillState.ResetChangeFlag();
		for (unsigned char j = 0; j < m_CSkillState.GetStateNum(); j++)
			AddStateToCharacter(j, pCCha, -1, enumSSTATE_ADD_LARGER, false);
		pCCha->SynSkillStateToEyeshot();
		pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
	}

	return pNode;
}

void CStateCell::DelCharacter(CChaListNode *pCEntNode)
{
	if (pCEntNode->m_pCLast)
		pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	if (pCEntNode->m_pCNext)
		pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	if (pCEntNode->m_bIn)
	{
		if (m_pCChaIn == pCEntNode)
			if (m_pCChaIn = pCEntNode->m_pCNext)
				m_pCChaIn->m_pCLast = 0;
	}
	else
	{
		if (m_pCChaCross == pCEntNode)
			if (m_pCChaCross = pCEntNode->m_pCNext)
				m_pCChaCross->m_pCLast = 0;
	}
	pCEntNode->m_pCLast = 0;
	pCEntNode->m_pCNext = 0;
	m_lChaNum--;
	pCEntNode->m_pCCha->OutMgrUnit(pCEntNode->m_pCEntStateNode); // ʵ���¼���ڵĹ�����Ԫ

	// ���ɫ����״̬
	CCharacter		*pCCha = pCEntNode->m_pCCha;
	if (m_CSkillState.GetStateNum() > 0)
	{
		SSkillStateUnit	*pSStateUnit;
		long			lOnTime;
		pCCha->m_CChaAttr.ResetChangeFlag();
		pCCha->m_CSkillState.ResetChangeFlag();
		for (unsigned char j = 0; j < m_CSkillState.GetStateNum(); j++)
		{
			pSStateUnit = m_CSkillState.GetSStateByNum(j);
			lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv());
			ResetStateToCharacter(j, pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER, false);
		}
		pCCha->SynSkillStateToEyeshot();
		pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
	}

	pCEntNode->Free();
}

void CStateCell::SetCharacterIn(CChaListNode *pCEntNode, bool bIn)
{
	//if (bIn)
	//{
	//	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHACROSS)
	//	{
	//		// ɾ��
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCChaCross == pCEntNode)
	//			m_pCChaCross = pCEntNode->m_pCNext;
	//		// ����
	//		if (pCEntNode->m_pCNext = m_pCChaIn)
	//			m_pCChaIn->m_pCLast = pCEntNode;
	//		m_pCChaIn = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHAIN;
	//	}
	//	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMCROSS)
	//	{
	//		// ɾ��
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCItemCross == pCEntNode)
	//			m_pCItemCross = pCEntNode->m_pCNext;
	//		// ����
	//		if (pCEntNode->m_pCNext = m_pCItemIn)
	//			m_pCItemIn->m_pCLast = pCEntNode;
	//		m_pCItemIn = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMIN;
	//	}
	//	else // �����ܵ����
	//	{}
	//	pCEntNode->m_pCEntity->SetCenterMgrNode(pCEntNode->m_pCEntStateNode);
	//}
	//else
	//{
	//	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	//	{
	//		// ɾ��
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCChaIn == pCEntNode)
	//			m_pCChaIn = pCEntNode->m_pCNext;
	//		// ����
	//		if (pCEntNode->m_pCNext = m_pCChaCross)
	//			m_pCChaCross->m_pCLast = pCEntNode;
	//		m_pCChaCross = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHACROSS;
	//	}
	//	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMIN)
	//	{
	//		// ɾ��
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCItemIn == pCEntNode)
	//			m_pCItemIn = pCEntNode->m_pCNext;
	//		// ����
	//		if (pCEntNode->m_pCNext = m_pCItemCross)
	//			m_pCItemCross->m_pCLast = pCEntNode;
	//		m_pCItemCross = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMCROSS;
	//	}
	//	else // �����ܵ����
	//	{}
	//}
}

bool CStateCell::AddState(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
						unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter)
{
	// ��ر�����״̬
	if (!m_CSkillState.Add(uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType, uchStateID, uchStateLv, ulStartTick, lOnTick, chType, chWithCenter))
		return false;

	// ���ɫ����״̬
	unsigned char	uchStateNo = m_CSkillState.GetReverseID(uchStateID);
	CChaListNode	*pNode;
	pNode = m_pCChaIn;
	while (pNode)
	{
		AddStateToCharacter(uchStateNo, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}
	pNode = m_pCChaCross;
	while (pNode)
	{
		AddStateToCharacter(uchStateNo, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}

	return true;
}

void CStateCell::DelState(unsigned char uchStateID)
{
}

bool CStateCell::ResetStateToCharacter(unsigned char uchStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice)
{
	if (!pCCha->IsLiveing())
		return false;
	CCharacter		*pCSrcCha;
	Entity			*pCSrcEnt;
	SSkillStateUnit	*pSStateUnit;
	pSStateUnit = m_CSkillState.GetSStateByNum(uchStateNo);
	if (!pSStateUnit)
		return false;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt)
	{
		pCSrcCha = pCSrcEnt->IsCharacter();
		if( pCCha->m_CSkillState.NeedResetState( pSStateUnit->GetStateID() ) )
		{
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
					pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
					pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
		}
	}
	else
	{
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
}

bool CStateCell::AddStateToCharacter(unsigned char uchStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice)
{
	if (!pCCha->IsLiveing())
		return false;
	CCharacter		*pCSrcCha;
	Entity			*pCSrcEnt;
	SSkillStateUnit	*pSStateUnit;
	pSStateUnit = m_CSkillState.GetSStateByNum(uchStateNo);
	if (!pSStateUnit)
		return false;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt)
	{
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCCha->IsRightSkillTar(pCSrcCha, pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType, true))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
					pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
					pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
	}
	else
	{
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
}

bool CStateCell::AddStateToCharacter(SSkillStateUnit	*pSStateUnit, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice)
{
	if (!pCCha->IsLiveing())
		return false;
	CCharacter		*pCSrcCha;
	Entity			*pCSrcEnt;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt)
	{
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCCha->IsRightSkillTar(pCSrcCha, pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType, true))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
					pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
					pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
	}
	else
	{
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
}

// ״̬���ڿ���
void CStateCell::StateRun(unsigned long ulCurTick, SubMap *pCMap)
{
	unsigned char	uchStateNum = m_CSkillState.GetStateNum();
	SSkillStateUnit	*pSStateUnit;
	long			lOnTime;
	CChaListNode	*pNode;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState())
	{
		if (pSStateUnit->lOnTick > 0)
		{
			if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // ״̬��ʱ���
			{
				lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv());
				pNode = m_pCChaIn;
				while (pNode)
				{
					AddStateToCharacter(pSStateUnit, pNode->m_pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode)
				{
					AddStateToCharacter(pSStateUnit, pNode->m_pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				m_CSkillState.Del(pSStateUnit->GetStateID());
				pCMap->InactiveStateCell(m_sPosX, m_sPosY);
			}
			else
			{
				// ���ɫ����״̬
				CChaListNode	*pNode;
				pNode = m_pCChaIn;
				while (pNode)
				{
					if (!pNode->m_pCCha->m_CSkillState.HasState(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv()))
						AddStateToCharacter(pSStateUnit, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode)
				{
					if (!pNode->m_pCCha->m_CSkillState.HasState(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv()))
						AddStateToCharacter(pSStateUnit, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
			}
		}
	}
	if (m_CSkillState.GetStateNum() != uchStateNum)
	{
		pCMap->NotiStateCellToEyeshot(m_sPosX, m_sPosY);
	}
}

void CStateCell::DropState(SubMap *pCMap)
{
	SSkillStateUnit	*pSStateUnit;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState())
	{
		if (pSStateUnit->lOnTick > 0)
		{
			m_CSkillState.Del(pSStateUnit->GetStateID());
			pCMap->InactiveStateCell(m_sPosX, m_sPosY);
		}
	}
}

void CStateCell::StateBeginSeen(Entity *pCEnt)
{
	if (m_CSkillState.GetStateNum() <= 0)
		return;

	CCharacter	*pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if(!pCCha->IsPlayerFocusCha()) // �ý�ɫ������ҵ�ǰ�Ŀ��ƽ���
		return;

	// Типизированная сериализация: появление состояний ячейки
	auto pk = net::msg::serialize(net::msg::McAStateBeginSeeMessage{
		static_cast<int64_t>(m_sPosX), static_cast<int64_t>(m_sPosY),
		m_CSkillState.BuildStateEntries()
	});
	pCCha->ReflectINFof(pCCha, pk);
}

void CStateCell::StateEndSeen(Entity *pCEnt)
{
	CCharacter	*pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if(!pCCha->IsPlayerFocusCha()) // �ý�ɫ������ҵ�ǰ�Ŀ��ƽ���
		return;

	if (m_CSkillState.GetStateNum() <= 0)
		return;

	// Типизированная сериализация: область навыков вышла из поля зрения
	auto pk = net::msg::serialize(net::msg::McAStateEndSeeMessage{(int64_t)m_sPosX, (int64_t)m_sPosY});

	pCCha->ReflectINFof(pCCha, pk);
}
