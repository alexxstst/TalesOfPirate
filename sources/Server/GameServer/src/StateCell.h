//=============================================================================
// FileName: StateCell.h
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map State Cell
//=============================================================================

#ifndef STATECELL_H
#define STATECELL_H

#include "PreAlloc.h"
#include "Character.h"

class CChaListNode : public dbc::PreAllocStru
{
public:
	CChaListNode(unsigned long ulSize = 1) : PreAllocStru(1)
	{
		m_pCCha = 0;
		m_pCNext = 0;
		m_pCLast = 0;
	}

	bool			m_bIn{ false };	// 在单元内
	CCharacter* m_pCCha{  };
	CStateCellNode* m_pCEntStateNode{  };

	CChaListNode* m_pCNext{ nullptr };
	CChaListNode* m_pCLast{ nullptr };

protected:
	void	Initially();
	void	Finally();
};

inline void CChaListNode::Initially()
{
	m_pCCha = nullptr;
	m_pCNext = nullptr;
	m_pCLast = nullptr;
}

inline void CChaListNode::Finally()
{
}

class CEyeshotCell;

// 状态单元类
class CStateCell : public dbc::PreAllocStru
{
public:
	CStateCell(unsigned long ulSize = 1) : PreAllocStru(1)
	{
		m_sPosX = 0;
		m_sPosY = 0;
		m_lActNum = 0;
		m_lChaNum = 0;
		m_pCChaIn = 0;
		m_pCChaCross = 0;

		m_pCNext = 0;
		m_pCLast = 0;
	}

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

	short			m_sPosX;		// 位置
	short			m_sPosY;		// 位置
	long			m_lActNum;		// 激活计数
	long			m_lChaNum;		// 角色总数

	CChaListNode	*m_pCChaIn;		// 记录中心点在范围内的角色
	CChaListNode	*m_pCChaCross;	// 记录中心点不在范围内，但与范围相交的角色

	CSkillState		m_CSkillState;	// 地表的技能状态

	CEyeshotCell	*m_pCEyeshotCell;	// 所在的视野单元指针

	CStateCell		*m_pCNext;		// 指向“激活的状态单元链表“的指针
	CStateCell		*m_pCLast;

protected:

private:
	void	Initially();
	void	Finally();

};

inline void CStateCell::Initially()
{
	m_lActNum = 0;
	m_lChaNum = 0;
	m_pCChaIn = 0;
	m_pCChaCross = 0;
	m_CSkillState.Init();
	m_pCEyeshotCell = 0;

	m_pCNext = 0;
	m_pCLast = 0;
}

inline void CStateCell::Finally()
{
}

class CActStateCell // 激活的状态单元链表
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
		//LG("状态单元操作错误", "向状态单元增加实体 [%d,%d] 时，发现其没有脱离先前的单元", pObj->m_sPosX, pObj->m_sPosY);
		LG("state cell operator error", "when add entity[%d,%d] to state cell，find it is not break away foregone manage cell", pObj->m_sPosX, pObj->m_sPosY);
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

class CStateCellNode : public dbc::PreAllocStru
{
public:
	CStateCellNode(unsigned long ulSize = 1) : PreAllocStru(1)
	{
		m_pCStateCell = 0;
		m_pCChaNode = 0;
		m_pCNext = 0;
		m_pCLast = 0;
	}

	CStateCell		*m_pCStateCell;
	CChaListNode	*m_pCChaNode;

	CStateCellNode	*m_pCNext;
	CStateCellNode	*m_pCLast;

protected:
	void	Initially();
	void	Finally();

};

inline void CStateCellNode::Initially()
{
	m_pCStateCell = 0;
	m_pCChaNode = 0;
	m_pCNext = 0;
	m_pCLast = 0;
}

inline void CStateCellNode::Finally()
{
}

#endif // STATECELL_H