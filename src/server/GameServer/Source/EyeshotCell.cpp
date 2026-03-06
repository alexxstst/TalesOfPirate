//=============================================================================
// FileName: EyeshotCell.cpp
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map Eyeshot Cell
//=============================================================================
#include "stdafx.h"
#include "EyeshotCell.h"

CEyeshotCell::CEyeshotCell()
{
	m_lActNum = 0;
	m_pCChaL = 0;
	m_pCItemL = 0;

	m_pCNext = 0;
	m_pCLast = 0;

	m_pCStateCell = 0;
	m_sStateCellNum = 0;

	m_lChaCount = 0;
	m_lItemCount = 0;
}

CEyeshotCell::~CEyeshotCell()
{
	Entity	*pCEnt, *pCHeadEnt;

	pCHeadEnt = m_pCChaL;
	while(pCEnt = pCHeadEnt)
	{
		pCHeadEnt = pCHeadEnt->m_pCEyeshotCellNext;
		pCEnt->m_pCEyeshotCellNext = 0;
		pCEnt->m_pCEyeshotCellLast = 0;
		pCEnt->Free();
	}

	pCHeadEnt = m_pCItemL;
	while(pCEnt = pCHeadEnt)
	{
		pCHeadEnt = pCHeadEnt->m_pCEyeshotCellNext;
		pCEnt->m_pCEyeshotCellNext = 0;
		pCEnt->m_pCEyeshotCellLast = 0;
		pCEnt->Free();
	}
}

// pCEnt
void CEyeshotCell::EnterEyeshot(Entity *pCEnt)
{T_B
	for (short i = 0; i < m_sStateCellNum; i++)
		if (*m_pCStateCell[i])
			(*m_pCStateCell[i])->StateBeginSeen(pCEnt);

	CCharacter	*pCCha = pCEnt->IsCharacter();
	CCharacter	*pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCEnt != pCCellCha)
		{
			if (pCCha)
			{
				if (pCCellCha->CanSeen(pCCha))
					pCEnt->BeginSee(pCCellCha);
				if (pCCha->CanSeen(pCCellCha))
					pCCellCha->BeginSee(pCEnt);
			}
			else
			{
				pCEnt->BeginSee(pCCellCha);
				pCCellCha->BeginSee(pCEnt);
			}
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
	CItem	*pCCellItem = m_pCItemL;
	while (pCCellItem)
	{
		pCEnt->BeginSee(pCCellItem);
		if (pCCellItem->m_pCEyeshotCellNext)
			pCCellItem = pCCellItem->m_pCEyeshotCellNext->IsItem();
		else
			pCCellItem = 0;
	}
T_E}

void CEyeshotCell::OutEyeshot(Entity *pCEnt)
{T_B
	for (short i = 0; i < m_sStateCellNum; i++)
		if (*m_pCStateCell[i])
			(*m_pCStateCell[i])->StateEndSeen(pCEnt);

	CCharacter	*pCCha = pCEnt->IsCharacter();
	CCharacter	*pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCEnt != pCCellCha)
		{
			if (pCCha)
			{
				if (pCCellCha->CanSeen(pCCha))
					pCEnt->EndSee(pCCellCha);
				if (pCCha->CanSeen(pCCellCha))
					pCCellCha->EndSee(pCEnt);
			}
			else
			{
				pCEnt->EndSee(pCCellCha);
				pCCellCha->EndSee(pCEnt);
			}
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
	CItem	*pCCellItem = m_pCItemL;
	while (pCCellItem)
	{
		pCEnt->EndSee(pCCellItem);
		if (pCCellItem->m_pCEyeshotCellNext)
			pCCellItem = pCCellItem->m_pCEyeshotCellNext->IsItem();
		else
			pCCellItem = 0;
	}
T_E}

void CEyeshotCell::RefreshEyeshot(Entity *pCEnt, bool bToEyeshot, bool bToNoHide, bool bToNoShow)
{T_B
	CCharacter	*pCCha = pCEnt->IsCharacter();
	if (!pCCha)
		return;
	if (pCCha->GetActControl(enumACTCONTROL_EYESHOT) == bToEyeshot
		&& pCCha->GetActControl(enumACTCONTROL_NOHIDE) == bToNoHide
		&& pCCha->GetActControl(enumACTCONTROL_NOSHOW) == bToNoShow)
		return;
	bool	bOldSeen, bNewSeen;

	CCharacter	*pCCellCha;
	pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCCha != pCCellCha)
		{
			bOldSeen = pCCha->CanSeen(pCCellCha);
			bNewSeen = pCCha->CanSeen(pCCellCha, bToEyeshot, bToNoHide, bToNoShow);
			if (bOldSeen && !bNewSeen)
				pCCellCha->EndSee(pCCha);
			if (!bOldSeen && bNewSeen)
				pCCellCha->BeginSee(pCCha);
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
T_E}

CCharacter* CEyeshotCell::GetNextCha(void)
{T_B
	CCharacter	*pRet = m_pCChaSearch;

	if (m_pCChaSearch)
		m_pCChaSearch = m_pCChaSearch->m_pCEyeshotCellNext ? m_pCChaSearch->m_pCEyeshotCellNext->IsCharacter() : NULL;

	return pRet;
T_E}
