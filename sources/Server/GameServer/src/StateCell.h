//=============================================================================
// FileName: StateCell.h
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map State Cell
//=============================================================================

#ifndef STATECELL_H
#define STATECELL_H

#include "Character.h"

class CChaListNode
{
public:
	CChaListNode() = default;

	bool			m_bIn{ false };
	CCharacter*		m_pCCha{ nullptr };
	CStateCellNode* m_pCEntStateNode{ nullptr };

	CChaListNode*	m_pCNext{ nullptr };
	CChaListNode*	m_pCLast{ nullptr };
};

class CEyeshotCell;

// 
class CStateCell
{
public:
	CStateCell() = default;

	long			GetChaNum(void) {return m_lChaNum;}
	long			GetStateNum(void) {return m_CSkillState.m_uchStateNum;}

	CChaListNode*	AddCharacter(CCharacter *pCCha, bool bIn);
	void			DelCharacter(CChaListNode *pCEntNode);
	void			SetCharacterIn(CChaListNode *pCEntNode, bool bIn = true);

	bool			HasState(unsigned char uchStateID) {return m_CSkillState.HasState(uchStateID);}
	bool			HasState(unsigned char uchStateID, unsigned char uchStateLv) {return m_CSkillState.HasState(uchStateID, uchStateLv);}
	bool			AddState(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
						unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter);
	void			DelState(unsigned char uchStateID);

	bool			AddStateToCharacter(unsigned char uchStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice = true);
	bool			ResetStateToCharacter(unsigned char uchStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice = true );

	bool			AddStateToCharacter(SSkillStateUnit	*pSStateUnit, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice = true);

	void			DropState(SubMap *pCMap);
	void			StateRun(unsigned long ulCurTick, SubMap *pCMap);

	void			StateBeginSeen(Entity *pCEnt);
	void			StateEndSeen(Entity *pCEnt);

	short			m_sPosX{};
	short			m_sPosY{};
	long			m_lActNum{};
	long			m_lChaNum{};

	CChaListNode	*m_pCChaIn{ nullptr };
	CChaListNode	*m_pCChaCross{ nullptr };

	CSkillState		m_CSkillState;	// 

	CEyeshotCell	*m_pCEyeshotCell{ nullptr };

	CStateCell		*m_pCNext{ nullptr };
	CStateCell		*m_pCLast{ nullptr };
};

class CActStateCell // 
{
public:
	CActStateCell()
	{
		m_pHead  = 0;
		m_lCount = 0;
	}

	void		Add(CStateCell *pObj);
	void		Del(CStateCell *pObj);

	void		BeginGet(void);
	CStateCell*	GetNext(void);

	long		GetActiveNum(void) {return m_lCount;}

protected:

private:
	CStateCell	*m_pHead;

	CStateCell	*m_pCur;

	long		m_lCount;

};

inline void CActStateCell::Add(CStateCell *pObj)
{
	if (pObj->m_pCLast || pObj->m_pCNext)
	{
		//LG("", " [%d,%d] ", pObj->m_sPosX, pObj->m_sPosY);
		ToLogService("errors", LogLevel::Error, "when add entity[{},{}] to state cell, find it is not break away foregone manage cell", pObj->m_sPosX, pObj->m_sPosY);
		return;
	}

	pObj->m_pCLast = 0;
	if (pObj->m_pCNext = m_pHead)
		m_pHead->m_pCLast = pObj;
	m_pHead = pObj;

	m_lCount++;
}

inline void CActStateCell::Del(CStateCell *pObj)
{
	if (!pObj)
		return;
	if (m_pCur == pObj)
		m_pCur = pObj->m_pCNext;

	if (pObj->m_pCLast)
		pObj->m_pCLast->m_pCNext = pObj->m_pCNext;
	if (pObj->m_pCNext)
		pObj->m_pCNext->m_pCLast = pObj->m_pCLast;
	if (m_pHead == pObj)
		if (m_pHead = pObj->m_pCNext)
			m_pHead->m_pCLast = 0;
	pObj->m_pCNext = 0;
	pObj->m_pCLast = 0;

	m_lCount--;
}

inline void CActStateCell::BeginGet()
{
	m_pCur = m_pHead;
}

inline CStateCell* CActStateCell::GetNext()
{
	CStateCell	*pRet = m_pCur;

	if (m_pCur)
		m_pCur = m_pCur->m_pCNext;

	return pRet;
}

class CStateCellNode
{
public:
	CStateCellNode() = default;

	CStateCell		*m_pCStateCell{ nullptr };
	CChaListNode	*m_pCChaNode{ nullptr };

	CStateCellNode	*m_pCNext{ nullptr };
	CStateCellNode	*m_pCLast{ nullptr };
};

#endif // STATECELL_H
