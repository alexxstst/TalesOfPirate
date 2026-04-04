//=============================================================================
// FileName: MgrUnitEx.h
// Creater: ZhangXuedong
// Date: 2005.03.04
// Comment: Map Manage Unit
//=============================================================================

#ifndef MGRUNITEX_H
#define MGRUNITEX_H

#include "GameApp.h"

#define def_MGRUNIT_ENTITY_TYPE_CHAIN		1
#define def_MGRUNIT_ENTITY_TYPE_CHACROSS	2
#define def_MGRUNIT_ENTITY_TYPE_ITEMIN		3
#define def_MGRUNIT_ENTITY_TYPE_ITEMCROSS	4

class SubMap;

class CEntityListNode
{
public:
	CEntityListNode() = default;

	char			m_chEntiType{};
	Entity			*m_pCEntity{ nullptr };
	CMgrNode		*m_pCEntMgrNode{ nullptr };

	CEntityListNode	*m_pCNext{ nullptr };
	CEntityListNode	*m_pCLast{ nullptr };
};

class CMgrUnit
{
public:
	CMgrUnit()
	{
		m_sPosX = 0;
		m_sPosY = 0;
		m_sAreaAttr = 0;
		m_chIslandID = 0;
		m_lActiveNum = 0;
		m_lEntityNum = 0;
		m_pCChaIn = 0;
		m_pCChaCross = 0;
		m_pCItemIn = 0;
		m_pCItemCross = 0;

		m_pCNext = 0;
		m_pCLast = 0;
	}

	long				GetEntityNum(void) {return m_lEntityNum;}
	long				GetStateNum(void) {return m_CSkillState.m_chStateNum;}

	CEntityListNode*	AddEntity(Entity *pCEnt, char chType);
	void				DelEntity(CEntityListNode *pCEntNode);
	void				SetEntityIn(CEntityListNode *pCEntNode, bool bIn = true);

	bool				AddState(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chEffType,
						char chStateID, char chStateLv, unsigned long ulStartTick, long lOnTick, char chType);
	void				DelState(char chStateID);
	bool				AddStateToCharacter(char chStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice = true);
	bool				AddStateToCharacter(SSkillStateUnit	*pSStateUnit, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice = true);

	void				EnterEyeshot(Entity *pCEnt);
	void				OutEyeshot(Entity *pCEnt);
	void				RefreshEyeshot(Entity *pCEnt, bool bEyeshot, bool bHide, bool bShow);
	void				Active(void);

	void				StateRun(unsigned long ulCurTick, SubMap *pCMap);

	short			m_sPosX;		// 
	short			m_sPosY;		// 
	short			m_sAreaAttr;	// 
	char			m_chIslandID;	// 
	long			m_lActiveNum;	// 
	long			m_lEntityNum;	// 

	CEntityListNode	*m_pCChaIn;		// 
	CEntityListNode	*m_pCChaCross;	// 
	CEntityListNode	*m_pCItemIn;		// 
	CEntityListNode	*m_pCItemCross;	// 

	CSkillState		m_CSkillState;	// 

	CMgrUnit		*m_pCNext;		// 
	CMgrUnit		*m_pCLast;

protected:

private:
	void	StateBeginSeen(Entity *pCEnt);
	void	StateEndSeen(Entity *pCEnt);
};

inline void CMgrUnit::EnterEyeshot(Entity *pCEnt)
{
	StateBeginSeen(pCEnt);

	CCharacter	*pCCha = pCEnt->IsCharacter();
	bool		bCanEyeshot = true, bNoHide = true, bNoShow = true;
	if (pCCha)
	{
		bCanEyeshot = pCCha->GetActControl(enumACTCONTROL_EYESHOT);
		bNoHide = pCCha->GetActControl(enumACTCONTROL_NOHIDE);
		bNoShow = pCCha->GetActControl(enumACTCONTROL_NOSHOW);
	}

	CCharacter	*pCTempCha;
	CEntityListNode	*pNode;
	pNode = m_pCChaIn;
	while (pNode)
	{
		pCTempCha = pNode->m_pCEntity->IsCharacter();
		if (pCCha != pCTempCha)
		{
			if (pCCha)
			{
				if (bCanEyeshot && (pCTempCha->GetActControl(enumACTCONTROL_NOHIDE) || !pCTempCha->GetActControl(enumACTCONTROL_NOSHOW)))
					pCCha->BeginSee(pCTempCha);
			}
			if (pCTempCha->GetActControl(enumACTCONTROL_EYESHOT) && (bNoHide || !bNoShow))
				pCTempCha->BeginSee(pCEnt);
		}
		pNode = pNode->m_pCNext;
	}
	if (pCCha)
	{
		pNode = m_pCItemIn;
		while (pNode)
		{
			pCEnt->BeginSee(pNode->m_pCEntity);
			pNode = pNode->m_pCNext;
		}
	}
}

inline void CMgrUnit::OutEyeshot(Entity *pCEnt)
{
	StateEndSeen(pCEnt);

	CCharacter	*pCCha = pCEnt->IsCharacter();
	bool		bCanEyeshot = true, bNoHide = true, bNoShow = true;
	if (pCCha)
	{
		bCanEyeshot = pCCha->GetActControl(enumACTCONTROL_EYESHOT);
		bNoHide = pCCha->GetActControl(enumACTCONTROL_NOHIDE);
		bNoShow = pCCha->GetActControl(enumACTCONTROL_NOSHOW);
	}

	CCharacter	*pCTempCha;
	CEntityListNode	*pNode;
	pNode = m_pCChaIn;
	while (pNode)
	{
		pCTempCha = pNode->m_pCEntity->IsCharacter();
		if (pCCha != pCTempCha)
		{
			if (pCCha)
			{
				if (bCanEyeshot && (pCTempCha->GetActControl(enumACTCONTROL_NOHIDE) || !pCTempCha->GetActControl(enumACTCONTROL_NOSHOW)))
					pCEnt->EndSee(pCTempCha);
			}
			if (pCTempCha->GetActControl(enumACTCONTROL_EYESHOT) && (bNoHide || !bNoShow))
				pCTempCha->EndSee(pCEnt);
		}
		pNode = pNode->m_pCNext;
	}
	if (pCCha)
	{
		pNode = m_pCItemIn;
		while (pNode)
		{
			pCEnt->EndSee(pNode->m_pCEntity);
			pNode = pNode->m_pCNext;
		}
	}
}

inline void CMgrUnit::RefreshEyeshot(Entity *pCEnt, bool bEyeshot, bool bHide, bool bShow)
{
	CCharacter	*pCCha = pCEnt->IsCharacter();
	bool		bCanEyeshot = true, bNoHide = true, bNoShow = true;
	if (pCCha)
	{
		bCanEyeshot = pCCha->GetActControl(enumACTCONTROL_EYESHOT);
		bNoHide = pCCha->GetActControl(enumACTCONTROL_NOHIDE);
		bNoShow = pCCha->GetActControl(enumACTCONTROL_NOSHOW);
	}

	CCharacter	*pCTempCha;
	CEntityListNode	*pNode;

	if (bEyeshot)
	{
		pNode = m_pCChaIn;
		while (pNode)
		{
			pCTempCha = pNode->m_pCEntity->IsCharacter();
			if (pCCha != pCTempCha)
			{
				//if (bCanEyeshot && (pCTempCha->GetActControl(enumACTCONTROL_NOHIDE) || !pCTempCha->GetActControl(enumACTCONTROL_NOSHOW)))
				//	pCEnt->BeginSee(pCTempCha);
				//else if (!bCanEyeshot && (pCTempCha->GetActControl(enumACTCONTROL_NOHIDE) || !pCTempCha->GetActControl(enumACTCONTROL_NOSHOW)))
				//	pCEnt->EndSee(pCTempCha);
			}
			pNode = pNode->m_pCNext;
		}
	}
	else if (bHide)
	{
		pNode = m_pCChaIn;
		while (pNode)
		{
			pCTempCha = pNode->m_pCEntity->IsCharacter();
			if (pCCha != pCTempCha)
			{
				if (pCTempCha->GetActControl(enumACTCONTROL_EYESHOT))
				{
					if (bNoShow)
					{
						if (bNoHide)
							pCTempCha->BeginSee(pCEnt);
						else if (!bNoHide)
							pCTempCha->EndSee(pCEnt);
					}
				}
			}
			pNode = pNode->m_pCNext;
		}
	}
	else if (bShow)
	{
		pNode = m_pCChaIn;
		while (pNode)
		{
			pCTempCha = pNode->m_pCEntity->IsCharacter();
			if (pCCha != pCTempCha)
			{
				if (pCTempCha->GetActControl(enumACTCONTROL_EYESHOT))
				{
					if (!bNoHide)
					{
						if (!bNoShow)
							pCTempCha->BeginSee(pCEnt);
						else if (bNoShow)
							pCTempCha->EndSee(pCEnt);
					}
				}
			}
			pNode = pNode->m_pCNext;
		}
	}
}

inline void CMgrUnit::Active(void)
{
	//CEntityListNode	*pNode;
	//pNode = m_pCChaIn;
	//CCharacter	*pCCha;
	//DWORD		dwExecTime;
	//while (pNode)
	//{
	//	pCCha = pNode->m_pCEntity->IsCharacter();
	//	if (pCCha && pCCha->IsLiveing() && (dwExecTime = pCCha->m_timerResume.IsOK(GetTickCount())))
	//		pCCha->OnResume(dwExecTime);

	//	pNode = pNode->m_pCNext;
	//}
}

inline void CMgrUnit::StateBeginSeen(Entity *pCEnt)
{
	if (m_CSkillState.m_chStateNum <= 0)
		return;

	CCharacter	*pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if(!pCCha->IsPlayerFocusCha()) // 
		return;

	net::WPacket pk	=g_gmsvr->GetWPacket();
	pk.WriteCmd(CMD_MC_ASTATEBEGINSEE);
	pk.WriteInt64(m_sPosX);
	pk.WriteInt64(m_sPosY);
	m_CSkillState.WriteState(pk);

	pCCha->ReflectINFof(pCCha, pk);//
}

inline void CMgrUnit::StateEndSeen(Entity *pCEnt)
{
	CCharacter	*pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if(!pCCha->IsPlayerFocusCha()) // 
		return;

	if (m_CSkillState.m_chStateNum <= 0)
		return;

	net::WPacket pk	=g_gmsvr->GetWPacket();
	pk.WriteCmd(CMD_MC_ASTATEENDSEE);
	pk.WriteInt64(m_sPosX);
	pk.WriteInt64(m_sPosY);

	pCCha->ReflectINFof(pCCha, pk);//
}


class CActiveMgrUnitL // 
{
public:
	CActiveMgrUnitL()
	{
		m_pHead  = 0;
		m_lCount = 0;
	}

	inline void		Add(CMgrUnit *pObj);
	inline void		Del(CMgrUnit *pObj);

	inline void			BeginGet(void);
	inline CMgrUnit*	GetNext(void);

	long			GetActiveNum(void) {return m_lCount;}

protected:

private:
	CMgrUnit	*m_pHead;

	CMgrUnit	*m_pCur;

	long	m_lCount;

};

inline void CActiveMgrUnitL::Add(CMgrUnit *pObj)
{
	pObj->m_pCLast = 0;
	if (pObj->m_pCNext = m_pHead)
		m_pHead->m_pCLast = pObj;
	m_pHead = pObj;

	m_lCount++;
}

inline void CActiveMgrUnitL::Del(CMgrUnit *pObj)
{
	if (pObj->m_pCLast)
		pObj->m_pCLast->m_pCNext = pObj->m_pCNext;
	if (pObj->m_pCNext)
		pObj->m_pCNext->m_pCLast = pObj->m_pCLast;
	if (m_pHead == pObj)
		m_pHead = pObj->m_pCNext;
	pObj->m_pCNext = 0;
	pObj->m_pCLast = 0;

	m_lCount--;
}

inline void CActiveMgrUnitL::BeginGet()
{
	m_pCur = m_pHead;
}

inline CMgrUnit* CActiveMgrUnitL::GetNext()
{
	CMgrUnit	*pRet = m_pCur;

	if (m_pCur)
		m_pCur = m_pCur->m_pCNext;

	return pRet;
}

class CMgrNode
{
public:
	CMgrNode() = default;

	CMgrUnit		*m_pCMgrUnit{ nullptr };
	CEntityListNode	*m_pCEntityNode{ nullptr };

	CMgrNode		*m_pCNext{ nullptr };
	CMgrNode		*m_pCLast{ nullptr };
};

#endif // MGRUNITEX_H
