#pragma once

//=============================================================================
// FileName: SkillState.h
// Creater: ZhangXuedong
// Date: 2005.01.13
// Comment: Skill State
//=============================================================================

#ifndef SKILLSTATE_H
#define SKILLSTATE_H

#include "GameAppNet.h"
#include "SkillStateType.h"
#include "CompCommand.h"
#include "SkillStateRecord.h"

struct SSkillStateUnit
{
	unsigned char	uchReverseID;
	unsigned char	uchStateID;
	unsigned char	uchStateLv;

	char			chCenter;		// 
	unsigned char	uchFightID;		// 
	char			chObjType;		// 
	char			chObjHabitat;	// 
	char			chEffType;		// 
	// 
	unsigned long	ulSrcWorldID;
	long			lSrcHandle;
	//

	long			lOnTick;		// -1>00
	unsigned long	ulStartTick;	// 
	unsigned long	ulLastTick;		// 

	unsigned char	GetStateID() {return uchStateID;}
	unsigned char	GetStateLv() {return uchStateLv;}
};

class CSkillState
{
public:
	CSkillState(unsigned char uchMaxState = AREA_STATE_MAXID)
	{
		Init(uchMaxState);
	}

	void	Init(unsigned char uchMaxState = AREA_STATE_MAXID);
	bool	Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
			unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter = 0);
	bool	Del(unsigned char uchStateID);
	void	Reset(void);
	bool	NeedResetState( unsigned char uchStateID );
	bool	HasState(unsigned char uchStateID, unsigned char uchStateLv);
	bool	HasState(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByID(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByNum(unsigned char uchNum);
	unsigned char		GetStateNum(void) {return m_uchStateNum;}
	unsigned char		GetReverseID(unsigned char uchStateID);

	void	SetChangeFlag();
	void	ResetChangeFlag();
	void	SetChangeBitFlag(long lBit);
	bool	GetChangeBitFlag(long lBit);
	unsigned char	GetChangeNum(void) {return m_uchChangeNum;}

	void	BeginGetState(void) {m_uchCurGetNo = m_uchStateNum;}
	SSkillStateUnit*	GetNextState(void);

	bool	WriteState(WPACKET &pk);

	SSkillStateUnit	*m_pSState[SKILL_STATE_MAXID + 1];
	unsigned char	m_uchStateNum;

protected:

private:
	unsigned char	m_uchMaxState;
	SSkillStateUnit	m_SState[SKILL_STATE_MAXID + 1];

	char			m_szChangeFlag[SSTATE_SIGN_BYTE_NUM]; // 
	unsigned char	m_uchChangeNum; // 
	unsigned char	m_uchCurGetNo;
};

inline void CSkillState::Init(unsigned char uchMaxState)
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

inline void CSkillState::Reset(void)
{
	for (unsigned char i = 0; i < m_uchStateNum; i++)
		m_pSState[i]->uchStateLv = 0;
	m_uchStateNum = 0;
	ResetChangeFlag();
}

inline bool CSkillState::Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
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

inline bool CSkillState::Del(unsigned char uchStateID)
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

inline SSkillStateUnit* CSkillState::GetSStateByID(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return 0;

	if (m_SState[uchStateID].uchStateLv > 0)
		return m_SState + uchStateID;
	else
		return 0;
}

inline SSkillStateUnit* CSkillState::GetSStateByNum(unsigned char uchNum)
{
	if (uchNum < 0 || uchNum >= m_uchStateNum)
		return 0;

	if (m_pSState[uchNum]->uchStateLv > 0)
		return m_pSState[uchNum];
	else // 
	{
		return 0;
	}
}

inline unsigned char CSkillState::GetReverseID(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return SKILL_STATE_MAXID;

	if (m_SState[uchStateID].uchStateLv <= 0)
		return SKILL_STATE_MAXID;

	return m_SState[uchStateID].uchReverseID;
}

inline bool CSkillState::NeedResetState( unsigned char uchStateID )
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return false;

	if (m_SState[uchStateID].uchStateLv <= 0)
		return false;

	if(m_SState[uchStateID].lOnTick == -1 ) 
		return true;

	return false;
}

inline bool CSkillState::HasState(unsigned char uchStateID, unsigned char uchStateLv)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return false;

	if (m_SState[uchStateID].uchStateLv <= 0 || m_SState[uchStateID].uchStateLv != uchStateLv)
		return false;

	return true;
}

inline bool CSkillState::HasState(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState)
		return false;

	if (m_SState[uchStateID].uchStateLv <= 0)
		return false;

	return true;
}

inline void CSkillState::SetChangeFlag()
{
	memset(m_szChangeFlag, 0xff, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = SKILL_STATE_MAXID;
}

inline void CSkillState::ResetChangeFlag()
{
	memset(m_szChangeFlag, 0, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = 0;
}

inline void CSkillState::SetChangeBitFlag(long lBit)
{
	if (lBit > m_uchMaxState)
		return;

	short sByteNO, sBitNO;
	sByteNO = short(lBit / 8);
	sBitNO = short(lBit % 8);

	char	chSetFlag = 0x01 << sBitNO;

	if (!(m_szChangeFlag[sByteNO] & chSetFlag))
		m_uchChangeNum++;

	m_szChangeFlag[sByteNO] |= chSetFlag;
}

inline bool	CSkillState::GetChangeBitFlag(long lBit)
{
	if (lBit > m_uchMaxState)
		return false;

	short sByteNO, sBitNO;
	sByteNO = short(lBit / 8);
	sBitNO = short(lBit % 8);

	return m_szChangeFlag[sByteNO] & (0x01 << sBitNO) ? true : false;
}

inline SSkillStateUnit* CSkillState::GetNextState(void)
{
	if (m_uchCurGetNo <= m_uchStateNum)
		return GetSStateByNum(--m_uchCurGetNo);
	return 0;
}

inline bool CSkillState::WriteState(WPACKET &pk)
{
	WRITE_CHAR(pk, m_uchStateNum);

	if (m_uchStateNum <= 0)
		return false;

	CSkillStateRecord	*pCStateRec;
	for (unsigned char j = 0; j < m_uchStateNum; j++)
	{
		pCStateRec = GetCSkillStateRecordInfo(m_pSState[j]->GetStateID());
		if (pCStateRec->IsShowCenter == 1 && m_pSState[j]->chCenter == 0)
		{
			WRITE_CHAR(pk, 0);
			continue;
		}
		WRITE_CHAR(pk, m_pSState[j]->uchStateID);
		WRITE_CHAR(pk, m_pSState[j]->uchStateLv);
		WRITE_LONG(pk, m_pSState[j]->ulSrcWorldID);
		WRITE_CHAR(pk, m_pSState[j]->uchFightID);
	}

	return true;
}

#endif // SKILLSTATE_H
