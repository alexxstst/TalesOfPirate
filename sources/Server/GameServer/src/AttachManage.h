//=============================================================================
// FileName: AttachManage.h
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CConjureMgr class
//=============================================================================

#ifndef ATTACHMANAGE_H
#define ATTACHMANAGE_H

#include "Attachable.h"
#include "GameAppNet.h"

class CConjureMgr
{
public:

	CConjureMgr();
	~CConjureMgr();

	void		Add(CAttachable *pCAttach);
	void		Delete(CAttachable *pCAttach);
	bool		SetLeader(CAttachable *pCAttach);
	CAttachable	*GetLeader();
	void		FreeAll();
	CAttachable	*GetTail();

	void		BeginGet(void);
	CAttachable	*GetNext(void);

protected:

private:
	CAttachable* m_pCLstHead{}; // 
	CAttachable* m_pCLstTail{};

	CAttachable* m_pCur{};
};

class CPassengerMgr
{
public:
	CPassengerMgr() = default;
	~CPassengerMgr();

	void		Add(CAttachable *pCAttach);
	void		Delete(CAttachable *pCAttach);
	bool		SetLeader(CAttachable *pCAttach);
	CAttachable	*GetLeader();

	void		BeginGet(void);
	CAttachable	*GetNext(void);

	void		FreeAll();
	void		DeleteAll();

private:
	CAttachable* m_pCLstHead{ nullptr };
	CAttachable* m_pCLstTail{ nullptr };

	long		m_lNum{};
	CAttachable* m_pCCurPess{ nullptr };
};

inline void CPassengerMgr::BeginGet(void)
{
	m_pCCurPess = m_pCLstHead;
}

inline CAttachable* CPassengerMgr::GetNext(void)
{
	CAttachable	*pRet = m_pCCurPess;

	if (m_pCCurPess)
		m_pCCurPess = m_pCCurPess->m_pCPassengerNext;

	return pRet;
}

#endif // ATTACHMANAGE_H
