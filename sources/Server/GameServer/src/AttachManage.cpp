//=============================================================================
// FileName: AttachManage.cpp
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CConjureMgr class
//=============================================================================
#include "stdafx.h"
#include "AttachManage.h"
#include "SubMap.h"
#include "GameAppNet.h"

_DBC_USING;

//=============================================================================
CConjureMgr::CConjureMgr()
{
	m_pCLstHead = 0;
	m_pCLstTail = 0;
}

CConjureMgr::~CConjureMgr()
{
	m_pCLstHead = 0;
	m_pCLstTail = 0;
}

void CConjureMgr::Add(CAttachable *pCAttach)
{
	if (m_pCLstTail)
	{
		pCAttach->m_pCConjureLast = m_pCLstTail;
		pCAttach->m_pCConjureNext = 0;
		m_pCLstTail->m_pCConjureNext = pCAttach;
		m_pCLstTail = pCAttach;
	}
	else // 
	{
		pCAttach->m_pCConjureLast = 0;
		pCAttach->m_pCConjureNext = 0;
		m_pCLstHead = pCAttach;
		m_pCLstTail = pCAttach;
	}
}

void CConjureMgr::Delete(CAttachable *pCAttach)
{
	if (pCAttach)
	{
		if (m_pCur == pCAttach)
			m_pCur = pCAttach->m_pCConjureNext;
		if (pCAttach->m_pCConjureLast)
			pCAttach->m_pCConjureLast->m_pCConjureNext = pCAttach->m_pCConjureNext;
		if (pCAttach->m_pCConjureNext)
			pCAttach->m_pCConjureNext->m_pCConjureLast = pCAttach->m_pCConjureLast;
		if (pCAttach == m_pCLstHead)
			if (m_pCLstHead = pCAttach->m_pCConjureNext)
				m_pCLstHead->m_pCConjureLast = 0;
		if (pCAttach == m_pCLstTail)
			if (m_pCLstTail = pCAttach->m_pCConjureLast)
				m_pCLstTail->m_pCConjureNext = 0;
		pCAttach->m_pCConjureLast = 0;
		pCAttach->m_pCConjureNext = 0;
	}
}

bool CConjureMgr::SetLeader(CAttachable *pCAttach)
{
	bool	bRet = false;

	if (!m_pCLstHead)
	{
		bRet = false;
	}
	else if (pCAttach != m_pCLstHead)
	{
		if (pCAttach == m_pCLstTail)
		{
			m_pCLstTail = pCAttach->m_pCConjureLast;
			m_pCLstTail->m_pCConjureNext = 0;
			pCAttach->m_pCConjureLast = 0;
			pCAttach->m_pCConjureNext = m_pCLstHead;
			m_pCLstHead = pCAttach;
			bRet = true;
		}
		else if (pCAttach->m_pCConjureLast && pCAttach->m_pCConjureNext)
		{
			pCAttach->m_pCConjureLast->m_pCConjureNext = pCAttach->m_pCConjureNext;
			pCAttach->m_pCConjureNext->m_pCConjureLast = pCAttach->m_pCConjureLast;
			pCAttach->m_pCConjureLast = 0;
			pCAttach->m_pCConjureNext = m_pCLstHead;
			m_pCLstHead = pCAttach;
			bRet = true;
		}
		else
			bRet = false;
	}
	else
		bRet = true;

	return bRet;
}

CAttachable *CConjureMgr::GetLeader()
{
	//CAttachable *pCAttach;

	return m_pCLstHead;
	//if (!IsBadReadPtr(m_pCLstHead, 4))
	//	pCAttach = m_pCLstHead;
	//else
	//{
	//	LG("error", "msgCConjureMgr::GetLeader() m_pCLstHead = %p\n", m_pCLstHead);
	//}

	//return pCAttach;
}

void CConjureMgr::FreeAll()
{
	CAttachable *pCTemp;
	SubMap *pSubMap;
	while (pCTemp = m_pCLstHead)
	{
		pCTemp->m_pCConjureLast = 0;
		pCTemp->m_pCConjureNext = 0;
		pSubMap = pCTemp->GetSubMap();
		if (pSubMap)
			pSubMap->GoOut(pCTemp);
		m_pCLstHead = m_pCLstHead->m_pCConjureNext;
		pCTemp->Free();
	}
	m_pCLstHead = 0;
	m_pCLstTail = 0;
}

CAttachable *CConjureMgr::GetTail()
{
	CAttachable *pCAttach;

	pCAttach = m_pCLstTail;

	return pCAttach;
}

void CConjureMgr::BeginGet(void)
{
	m_pCur = m_pCLstHead;
}

CAttachable	*CConjureMgr::GetNext(void)
{
	CAttachable	*pRet = m_pCur;

	if (m_pCur)
		m_pCur = m_pCur->m_pCConjureNext;

	return pRet;
}

//=============================================================================
CPassengerMgr::CPassengerMgr(uLong) : PreAllocStru(1)
{
	m_pCLstHead = 0;
	m_pCLstTail = 0;
	m_lNum = 0;
}

CPassengerMgr::~CPassengerMgr()
{
}

void CPassengerMgr::Initially()
{
	m_pCLstHead = 0;
	m_pCLstTail = 0;
	m_lNum = 0;
}

void CPassengerMgr::Finally()
{
	DeleteAll();
}

void CPassengerMgr::Add(CAttachable *pCAttach)
{
	if (m_pCLstTail)
	{
		pCAttach->m_pCPassengerLast = m_pCLstTail;
		pCAttach->m_pCPassengerNext = 0;
		m_pCLstTail->m_pCPassengerNext = pCAttach;
		m_pCLstTail = pCAttach;
	}
	else // 
	{
		pCAttach->m_pCPassengerLast = 0;
		pCAttach->m_pCPassengerNext = 0;
		m_pCLstHead = pCAttach;
		m_pCLstTail = pCAttach;
	}
	m_lNum++;
}

void CPassengerMgr::Delete(CAttachable *pCAttach)
{
	if (pCAttach && m_pCLstHead)
	{
		if (m_pCCurPess == pCAttach)
			m_pCCurPess = pCAttach->m_pCPassengerNext;
		if (pCAttach->m_pCPassengerLast)
			pCAttach->m_pCPassengerLast->m_pCPassengerNext = pCAttach->m_pCPassengerNext;
		if (pCAttach->m_pCPassengerNext)
			pCAttach->m_pCPassengerNext->m_pCPassengerLast = pCAttach->m_pCPassengerLast;
		if (pCAttach == m_pCLstHead)
			if (m_pCLstHead = m_pCLstHead->m_pCPassengerNext)
				m_pCLstHead->m_pCPassengerLast = 0;
		if (pCAttach == m_pCLstTail)
			if (m_pCLstTail = m_pCLstTail->m_pCPassengerLast)
				m_pCLstTail->m_pCPassengerNext = 0;
		pCAttach->m_pCPassengerLast = pCAttach->m_pCPassengerNext = 0;

		m_lNum--;
	}
}

bool CPassengerMgr::SetLeader(CAttachable *pCAttach)
{
	bool	bRet = false;

	if (!m_pCLstHead)
	{
		bRet = false;
	}
	else if (pCAttach == m_pCLstHead)
	{
		bRet = true;
	}
	else if (pCAttach == m_pCLstTail)
	{
		m_pCLstTail = pCAttach->m_pCPassengerLast;
		m_pCLstTail->m_pCPassengerNext = 0;
		pCAttach->m_pCPassengerLast = 0;
		pCAttach->m_pCPassengerNext = m_pCLstHead;
		m_pCLstHead = pCAttach;
		bRet = true;
	}
	else if (pCAttach->m_pCPassengerLast && pCAttach->m_pCPassengerNext)
	{
		pCAttach->m_pCPassengerLast->m_pCPassengerNext = pCAttach->m_pCPassengerNext;
		pCAttach->m_pCPassengerNext->m_pCPassengerLast = pCAttach->m_pCPassengerLast;
		pCAttach->m_pCPassengerLast = 0;
		pCAttach->m_pCPassengerNext = m_pCLstHead;
		m_pCLstHead = pCAttach;
		bRet = true;
	}
	else
		bRet = false;

	return bRet;
}

CAttachable *CPassengerMgr::GetLeader()
{
	CAttachable *pCAttach;

	pCAttach = m_pCLstHead;

	return pCAttach;
}

void CPassengerMgr::FreeAll()
{
	CAttachable *pCTemp;
	SubMap *pSubMap;
	while (pCTemp = m_pCLstHead)
	{
		pCTemp->m_pCPassengerLast = 0;
		pCTemp->m_pCPassengerNext = 0;
		pSubMap = pCTemp->GetSubMap();
		if (pSubMap)
			pSubMap->GoOut(pCTemp);
		m_pCLstHead = m_pCLstHead->m_pCPassengerNext;
		pCTemp->Free();
	}
	m_pCLstHead = 0;
	m_pCLstTail = 0;
}

void CPassengerMgr::DeleteAll()
{
	CAttachable *pCTemp;
	while (pCTemp = m_pCLstHead)
	{
		m_pCLstHead = m_pCLstHead->m_pCPassengerNext;
		pCTemp->m_pCPassengerLast = 0;
		pCTemp->m_pCPassengerNext = 0;
	}
	m_pCLstHead = 0;
	m_pCLstTail = 0;
}
