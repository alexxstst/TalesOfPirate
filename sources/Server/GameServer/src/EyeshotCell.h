//=============================================================================
// FileName: EyeshotCell.h
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map Eyeshot Cell
//=============================================================================

#ifndef EYESHOTCELL_H
#define EYESHOTCELL_H

#include "Character.h"
#include "Entity.h"
#include "Item.h"
#include "StateCell.h"

// ��Ұ��Ԫ�࣬��¼��Ԫ�ڵ����н�ɫ�͵��ߣ��Լ�״̬��Ԫ��
class CEyeshotCell
{
public:
	CEyeshotCell();
	~CEyeshotCell();

	void	AddEntity(CCharacter *pCCha);
	void	AddEntity(CItem *pCItem);
	void	DelEntity(Entity *pCEnt);
	long	GetEntityNum(void) {return m_lChaCount + m_lItemCount;}
	long	GetChaNum(void) {return m_lChaCount;}
	long	GetItemNum(void) {return m_lItemCount;}

	void	EnterEyeshot(Entity *pCEnt);
	void	OutEyeshot(Entity *pCEnt);
	void	RefreshEyeshot(Entity *pCEnt, bool bToEyeshot, bool bToNoHide, bool bToNoShow);

	void	BeginGetCha(void) {m_pCChaSearch = m_pCChaL;} // ��ʼȡ��Ԫ�ڵĽ�ɫ.
	CCharacter*	GetNextCha(void); // ȡ��Ԫ�ڵ���һ����ɫ

public:
	short			m_sPosX;	// λ��
	short			m_sPosY;
	long	 	    m_lActNum;	// �������
	CCharacter		*m_pCChaL;	// ��ɫ��
	CItem			*m_pCItemL;	// ������

	CEyeshotCell	*m_pCNext;	// ָ�򡰼���Ĺ�����Ԫ��������ָ��
	CEyeshotCell	*m_pCLast;

	CStateCell		***m_pCStateCell;	// ��Ұ��Ԫ������״̬��Ԫ��һ����Ұ������������״̬��ԪҲ�ᱻ����෴ĳһ״̬��Ԫ�ļ��Ҳ�ἤ����Ұ
	short			m_sStateCellNum;

private:
	long			m_lChaCount;
	long			m_lItemCount;

	CCharacter		*m_pCChaSearch;

};

inline void CEyeshotCell::AddEntity(CCharacter *pCCha)
{
	if (!pCCha)
		return;
	if (pCCha->m_pCEyeshotCellLast || pCCha->m_pCEyeshotCellNext)
	{
		//LG("��Ұ��Ԫ��������", "����Ұ��Ԫ���ӽ�ɫʵ�� %s ʱ��������û��������ǰ�Ĺ�����Ԫ", pCCha->GetLogName());
		ToLogService("errors", LogLevel::Error, "when add character entity to eyeshot cell {} ,find it is not break away foregone manage cell", pCCha->GetLogName());
		return;
	}

	pCCha->m_pCEyeshotCellLast = 0;
	pCCha->m_pCEyeshotCellNext = m_pCChaL;
	if(pCCha->m_pCEyeshotCellNext)
	{
		pCCha->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCCha;
	}
	m_pCChaL = pCCha;

	m_lChaCount++;
}

inline void CEyeshotCell::AddEntity(CItem *pCItem)
{
	if (!pCItem)
		return;
	if (pCItem->m_pCEyeshotCellLast || pCItem->m_pCEyeshotCellNext)
	{
		//LG("��Ұ��Ԫ��������", "����Ұ��Ԫ���ӵ���ʵ�� %s ʱ��������û��������ǰ�Ĺ�����Ԫ", pCItem->GetLogName());
		ToLogService("errors", LogLevel::Error, "when add item entity to {}, find it is not break away foregone manage cell", pCItem->GetLogName());
		return;
	}

	pCItem->m_pCEyeshotCellLast = 0;
	pCItem->m_pCEyeshotCellNext = m_pCItemL;
	if(pCItem->m_pCEyeshotCellNext)
	{
		pCItem->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCItem;
	}
	m_pCItemL = pCItem;

	m_lItemCount++;
}

inline void CEyeshotCell::DelEntity(Entity *pCEnt)
{
	if (!pCEnt)
		return;
	if (pCEnt->IsCharacter())
	{
		if (m_pCChaSearch == pCEnt)
			m_pCChaSearch = pCEnt->m_pCEyeshotCellNext ? pCEnt->m_pCEyeshotCellNext->IsCharacter() : NULL;
	}

	if(pCEnt->m_pCEyeshotCellLast)
		pCEnt->m_pCEyeshotCellLast->m_pCEyeshotCellNext = pCEnt->m_pCEyeshotCellNext;
	if(pCEnt->m_pCEyeshotCellNext)
		pCEnt->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCEnt->m_pCEyeshotCellLast;

	if(m_pCChaL == pCEnt)
	{
		if (pCEnt->m_pCEyeshotCellNext)
		{
			m_pCChaL = pCEnt->m_pCEyeshotCellNext->IsCharacter();
			m_pCChaL->m_pCEyeshotCellLast = 0;
		}
		else
			m_pCChaL = 0;
	}
	else if (m_pCItemL == pCEnt)
	{
		if (pCEnt->m_pCEyeshotCellNext)
		{
			m_pCItemL = pCEnt->m_pCEyeshotCellNext->IsItem();
			m_pCItemL->m_pCEyeshotCellLast = 0;
		}
		else
			m_pCItemL = 0;
	}

	pCEnt->m_pCEyeshotCellLast = 0;
	pCEnt->m_pCEyeshotCellNext = 0;

	if (pCEnt->IsCharacter())
		m_lChaCount--;
	else
		m_lItemCount--;
}

//=============================================================================
// �������Ұ��Ԫ����
class CActEyeshotCell
{
public:
	void			Add(CEyeshotCell *pObj);
	void			Del(CEyeshotCell *pObj);

	void			BeginGet(void); // ��ʼȡ���Ԫ.
	CEyeshotCell*	GetNext(void); // ȡ��һ�����Ԫ.
	CEyeshotCell*	GetCurrent(void);

	long			GetActiveNum(void) {return m_lCount;}
protected:

private:
	CEyeshotCell* m_pHead{};

	CEyeshotCell* m_pCur{};

	long			m_lCount{};

};

inline void CActEyeshotCell::Add(CEyeshotCell *pObj)
{
	pObj->m_pCLast = 0;
	if (pObj->m_pCNext = m_pHead)
		m_pHead->m_pCLast = pObj;
	m_pHead = pObj;

	m_lCount++;
}

inline void CActEyeshotCell::Del(CEyeshotCell *pObj)
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
	{
		if (m_pHead = pObj->m_pCNext)
			m_pHead->m_pCLast = 0;
	}
	pObj->m_pCNext = 0;
	pObj->m_pCLast = 0;

	m_lCount--;
}

inline void CActEyeshotCell::BeginGet()
{
	m_pCur = m_pHead;
}

inline CEyeshotCell* CActEyeshotCell::GetNext()
{
	CEyeshotCell	*pRet = m_pCur;

	if (m_pCur)
		m_pCur = m_pCur->m_pCNext;

	return pRet;
}

inline CEyeshotCell* CActEyeshotCell::GetCurrent()
{
	return m_pCur;
}

#endif // EYESHOTCELL_H