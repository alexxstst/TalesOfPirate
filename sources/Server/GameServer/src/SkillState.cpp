// SkillState.cpp — реализации ранее inline-методов из SkillState.h.
#include "stdafx.h"
#include "SkillState.h"

unsigned char SSkillStateUnit::GetStateID() { return uchStateID; }
unsigned char SSkillStateUnit::GetStateLv() { return uchStateLv; }

CSkillState::CSkillState(unsigned char uchMaxState) {
	Init(uchMaxState);
}

void CSkillState::Init(unsigned char uchMaxState)
{
	if (uchMaxState > SKILL_STATE_MAXID)
		uchMaxState = SKILL_STATE_MAXID;
	m_uchMaxState = uchMaxState;
	m_uchStateNum = 0;
	for (unsigned char i = 0; i <= m_uchMaxState; i++)
	{
		m_SState[i].uchStateID = i;
		m_SState[i].uchStateLv = 0;
	}
}

void CSkillState::Reset(void)
{
	for (unsigned char i = 0; i < m_uchStateNum; i++)
		m_pSState[i]->uchStateLv = 0;
	m_uchStateNum = 0;
	ResetChangeFlag();
}

bool CSkillState::Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
					  unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return false;
	if (uchStateLv <= 0)
		return false;

	if (m_SState[uchStateID].uchStateLv == 0)
	{
		if (m_uchStateNum >= m_uchMaxState)
			return false;

		m_pSState[m_uchStateNum] = m_SState + uchStateID;
		m_SState[uchStateID].uchReverseID = m_uchStateNum;
		m_SState[uchStateID].chCenter = chWithCenter;
		m_SState[uchStateID].uchFightID = uchFightID;
		m_SState[uchStateID].ulSrcWorldID = ulSrcWorldID;
		m_SState[uchStateID].lSrcHandle = lSrcHandle;
		m_SState[uchStateID].chObjType = chObjType;
		m_SState[uchStateID].chObjHabitat = chObjHabitat;
		m_SState[uchStateID].chEffType = chEffType;
		m_SState[uchStateID].uchStateLv = uchStateLv;
		m_SState[uchStateID].ulStartTick = ulStartTick;
		m_SState[uchStateID].ulLastTick = ulStartTick;
		m_SState[uchStateID].lOnTick = lOnTick;
		m_uchStateNum++;

		SetChangeBitFlag(uchStateID);
	}
	else if (chType == enumSSTATE_ADD_EQUALORLARGER)
	{
		if (uchStateLv < m_SState[uchStateID].uchStateLv)
			return false;

		m_SState[uchStateID].ulSrcWorldID = ulSrcWorldID;
		m_SState[uchStateID].chCenter = chWithCenter;
		m_SState[uchStateID].uchFightID = uchFightID;
		m_SState[uchStateID].lSrcHandle = lSrcHandle;
		m_SState[uchStateID].chObjType = chObjType;
		m_SState[uchStateID].chObjHabitat = chObjHabitat;
		m_SState[uchStateID].chEffType = chEffType;
		m_SState[uchStateID].uchStateLv = uchStateLv;
		m_SState[uchStateID].ulStartTick = ulStartTick;
		m_SState[uchStateID].ulLastTick = ulStartTick;
		m_SState[uchStateID].lOnTick = lOnTick;

		SetChangeBitFlag(uchStateID);
	}
	else if (chType == enumSSTATE_ADD_LARGER)
	{
		if (uchStateLv <= m_SState[uchStateID].uchStateLv)
			return false;

		m_SState[uchStateID].ulSrcWorldID = ulSrcWorldID;
		m_SState[uchStateID].chCenter = chWithCenter;
		m_SState[uchStateID].uchFightID = uchFightID;
		m_SState[uchStateID].lSrcHandle = lSrcHandle;
		m_SState[uchStateID].chObjType = chObjType;
		m_SState[uchStateID].chObjHabitat = chObjHabitat;
		m_SState[uchStateID].chEffType = chEffType;
		m_SState[uchStateID].uchStateLv = uchStateLv;
		m_SState[uchStateID].ulStartTick = ulStartTick;
		m_SState[uchStateID].ulLastTick = ulStartTick;
		m_SState[uchStateID].lOnTick = lOnTick;

		SetChangeBitFlag(uchStateID);
	}
	else if (chType == enumSSTATE_NOTADD)
	{
		return false;
	}
	else if (chType == enumSSTATE_ADD)
	{
		m_SState[uchStateID].ulSrcWorldID = ulSrcWorldID;
		m_SState[uchStateID].chCenter = chWithCenter;
		m_SState[uchStateID].uchFightID = uchFightID;
		m_SState[uchStateID].lSrcHandle = lSrcHandle;
		m_SState[uchStateID].chObjType = chObjType;
		m_SState[uchStateID].chObjHabitat = chObjHabitat;
		m_SState[uchStateID].chEffType = chEffType;
		m_SState[uchStateID].uchStateLv = uchStateLv;
		m_SState[uchStateID].ulStartTick = ulStartTick;
		m_SState[uchStateID].ulLastTick = ulStartTick;
		m_SState[uchStateID].lOnTick = lOnTick;

		SetChangeBitFlag(uchStateID);
	}

	return true;
}

bool CSkillState::Del(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState || m_uchStateNum < 1)
		return false;

	if (m_SState[uchStateID].uchStateLv != 0)
	{
		if (m_uchCurGetNo == m_uchStateNum)
			m_uchCurGetNo--;
		m_uchStateNum--;
		m_SState[uchStateID].uchStateLv = 0;
		m_pSState[m_SState[uchStateID].uchReverseID] = m_pSState[m_uchStateNum];
		m_pSState[m_uchStateNum]->uchReverseID = m_SState[uchStateID].uchReverseID;

		SetChangeBitFlag(uchStateID);
	}

	return true;
}

SSkillStateUnit* CSkillState::GetSStateByID(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) return nullptr;
	if (m_SState[uchStateID].uchStateLv > 0) return m_SState + uchStateID;
	return nullptr;
}

SSkillStateUnit* CSkillState::GetSStateByNum(unsigned char uchNum)
{
	if (uchNum >= m_uchStateNum) return nullptr;
	if (m_pSState[uchNum]->uchStateLv > 0) return m_pSState[uchNum];
	return nullptr;
}

unsigned char CSkillState::GetStateNum(void) { return m_uchStateNum; }

unsigned char CSkillState::GetReverseID(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)        return SKILL_STATE_MAXID;
	if (m_SState[uchStateID].uchStateLv <= 0)                return SKILL_STATE_MAXID;
	return m_SState[uchStateID].uchReverseID;
}

bool CSkillState::NeedResetState(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) return false;
	if (m_SState[uchStateID].uchStateLv <= 0) return false;
	if (m_SState[uchStateID].lOnTick == -1) return true;
	return false;
}

bool CSkillState::HasState(unsigned char uchStateID, unsigned char uchStateLv)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) return false;
	if (m_SState[uchStateID].uchStateLv <= 0 || m_SState[uchStateID].uchStateLv != uchStateLv) return false;
	return true;
}

bool CSkillState::HasState(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) return false;
	if (m_SState[uchStateID].uchStateLv <= 0) return false;
	return true;
}

void CSkillState::SetChangeFlag()
{
	memset(m_szChangeFlag, 0xff, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = SKILL_STATE_MAXID;
}

void CSkillState::ResetChangeFlag()
{
	memset(m_szChangeFlag, 0, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = 0;
}

void CSkillState::SetChangeBitFlag(long lBit)
{
	if (lBit > m_uchMaxState) return;

	short sByteNO = short(lBit / 8);
	short sBitNO  = short(lBit % 8);

	char  chSetFlag = 0x01 << sBitNO;

	if (!(m_szChangeFlag[sByteNO] & chSetFlag))
		m_uchChangeNum++;

	m_szChangeFlag[sByteNO] |= chSetFlag;
}

bool CSkillState::GetChangeBitFlag(long lBit)
{
	if (lBit > m_uchMaxState) return false;
	short sByteNO = short(lBit / 8);
	short sBitNO  = short(lBit % 8);
	return (m_szChangeFlag[sByteNO] & (0x01 << sBitNO)) != 0;
}

unsigned char CSkillState::GetChangeNum(void) { return m_uchChangeNum; }

void CSkillState::BeginGetState(void) { m_uchCurGetNo = m_uchStateNum; }

SSkillStateUnit* CSkillState::GetNextState(void)
{
	if (m_uchCurGetNo <= m_uchStateNum)
		return GetSStateByNum(--m_uchCurGetNo);
	return nullptr;
}

bool CSkillState::WriteState(net::WPacket& pk)
{
	pk.WriteInt64(m_uchStateNum);

	if (m_uchStateNum <= 0) return false;

	CSkillStateRecord* pCStateRec;
	for (unsigned char j = 0; j < m_uchStateNum; j++)
	{
		pCStateRec = GetCSkillStateRecordInfo(m_pSState[j]->GetStateID());
		if (pCStateRec->IsShowCenter == 1 && m_pSState[j]->chCenter == 0)
		{
			pk.WriteInt64(0);
			continue;
		}
		pk.WriteInt64(m_pSState[j]->uchStateID);
		pk.WriteInt64(m_pSState[j]->uchStateLv);
		pk.WriteInt64(m_pSState[j]->ulSrcWorldID);
		pk.WriteInt64(m_pSState[j]->uchFightID);
	}

	return true;
}

std::vector<net::msg::AStateBeginSeeEntry> CSkillState::BuildStateEntries()
{
	std::vector<net::msg::AStateBeginSeeEntry> entries;
	entries.resize(m_uchStateNum);

	for (unsigned char j = 0; j < m_uchStateNum; j++)
	{
		CSkillStateRecord* pCStateRec = GetCSkillStateRecordInfo(m_pSState[j]->GetStateID());
		if (pCStateRec->IsShowCenter == 1 && m_pSState[j]->chCenter == 0)
		{
			entries[j] = {0, 0, 0, 0};
			continue;
		}
		entries[j] = {
			static_cast<int64_t>(m_pSState[j]->uchStateID),
			static_cast<int64_t>(m_pSState[j]->uchStateLv),
			static_cast<int64_t>(m_pSState[j]->ulSrcWorldID),
			static_cast<int64_t>(m_pSState[j]->uchFightID)
		};
	}
	return entries;
}
