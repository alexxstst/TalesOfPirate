#include "stdafx.h"
#include "character.h"
#include "gameapp.h"
#include "HarmRec.h"

BOOL g_bLogHarmRec = FALSE;

// ============================================================================
// Ранее inline-методы из HarmRec.h, вынесены в .cpp 2026-04-22.
// ============================================================================

SHarmRec::SHarmRec()
	: pAtk(nullptr), sHarm(0), sHate(0), btValid(0), dwID(0), dwTime(0)
{}

bool SHarmRec::IsChaValid()
{
	if (!pAtk) return false;
	if (!g_pGameApp->IsLiveingEntity(dwID, pAtk->GetHandle())) return false;
	return true;
}

CHateMgr::CHateMgr()
	: _dwLastDecValid(0), _dwLastSortTick(0)
{}

SHarmRec* CHateMgr::GetHarmRec(int nNo) { return &_HarmRec[nNo]; }

CCharacter* CHateMgr::GetCurTarget()
{
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->btValid > 0 && pHarm->IsChaValid()) {
			return pHarm->pAtk;
		}
	}
	return nullptr;
}

void CHateMgr::ClearHarmRec()
{
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		pHarm->btValid = 0;
		pHarm->dwID    = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->pAtk    = nullptr;
		pHarm->dwTime  = 0;
	}
}

void CHateMgr::ClearHarmRecByCha(CCharacter* pAtk)
{
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pAtk && pHarm->pAtk == pAtk) {
			pHarm->btValid = 0;
			pHarm->dwID    = 0;
			pHarm->sHarm   = 0;
			pHarm->sHate   = 0;
			pHarm->pAtk    = nullptr;
			pHarm->dwTime  = 0;
		}
	}
}

void CHateMgr::AddHarm(CCharacter* pAtk, short sHarm, DWORD dwID)
{
	if (g_bLogHarmRec) {
		ToLogService("common", "begin to add harm, attacker[{}], harm{}", pAtk->GetName(), sHarm);
	}
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->pAtk == pAtk && pHarm->pAtk->GetID() == dwID) {
			pHarm->sHarm += sHarm;
			pHarm->sHate += sHarm;
			if (pHarm->btValid < MAX_VALID_CNT) {
				pHarm->btValid++;
				if (g_bLogHarmRec) {
					ToLogService("common", "attacker[{}], accunulative harm={}, valid={}", pAtk->GetName(), pHarm->sHarm, pHarm->btValid);
				}
			}
			return;
		}
	}

	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->btValid == 0) {
			pHarm->pAtk    = pAtk;
			pHarm->sHarm   = sHarm;
			pHarm->sHate   = sHarm;
			pHarm->btValid = 3;
			pHarm->dwID    = dwID;
			pHarm->dwTime  = g_pGameApp->m_dwRunCnt;
			if (g_bLogHarmRec) {
				ToLogService("common", "add new attacker[{}], harm = {}", pAtk->GetName(), pHarm->sHarm);
			}
			break;
		}
	}
}

void CHateMgr::AddHate(CCharacter* pAtk, short sHate, DWORD dwID)
{
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->pAtk == pAtk && pHarm->pAtk->GetID() == dwID) {
			pHarm->sHate += sHate;

			if (sHate > 0) {
				if (pHarm->btValid < MAX_VALID_CNT) pHarm->btValid++;
			}
			else {
				if (pHarm->btValid > 0) pHarm->btValid--;
			}

			if (pHarm->sHate < 0) pHarm->sHate = 0;
			return;
		}
	}

	if (sHate > 0) {
		for (int i = 0; i < MAX_HARM_REC; i++) {
			SHarmRec* pHarm = &_HarmRec[i];
			if (pHarm->btValid == 0) {
				pHarm->pAtk    = pAtk;
				pHarm->sHate   = sHate;
				pHarm->btValid = 3;
				pHarm->dwID    = dwID;
				pHarm->dwTime  = g_pGameApp->m_dwRunCnt;
				break;
			}
		}
	}
}

int CompareHarm(const void* p1, const void* p2)
{
	const SHarmRec* pRec1 = (const SHarmRec*)p1;
	const SHarmRec* pRec2 = (const SHarmRec*)p2;
	if (pRec1->sHate < pRec2->sHate) return 1;
	return -1;
}

void CHateMgr::UpdateHarmRec(CCharacter* pSelf)
{
	DWORD dwCurTick = GetTickCount();

	int nValid = 0;
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->btValid > 0 && pHarm->IsChaValid()) {
			memcpy(&_HarmRec[nValid], pHarm, sizeof(SHarmRec));
			nValid++;
		}
	}

	for (int j = nValid; j < MAX_HARM_REC; j++) {
		SHarmRec* pHarm = &_HarmRec[j];
		pHarm->btValid = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->dwID    = 0;
		pHarm->pAtk    = nullptr;
		pHarm->dwTime  = 0;
	}

	if ((dwCurTick - _dwLastSortTick) > 2000) {
		_dwLastSortTick = dwCurTick;
		qsort(&_HarmRec, MAX_HARM_REC, sizeof(SHarmRec), CompareHarm);
		if (g_bLogHarmRec) DebugNotice(pSelf);
	}

	if ((dwCurTick - _dwLastDecValid) > 5000) {
		_dwLastDecValid = dwCurTick;
		for (int i = 0; i < MAX_HARM_REC; i++) {
			SHarmRec* pHarm = &_HarmRec[i];
			if (pHarm->btValid > 0) {
				pHarm->btValid--;
				if (pHarm->btValid == 0) {
					pHarm->sHarm  = 0;
					pHarm->sHate  = 0;
					pHarm->dwTime = 0;
					pHarm->pAtk   = nullptr;
					pHarm->dwID   = 0;
				}
				if (g_bLogHarmRec && pHarm->pAtk) {
					ToLogService("common", "attacker[{}], valid--, valid = {}", pHarm->pAtk->GetName(), pHarm->btValid);
				}
			}
		}
	}
}

void CHateMgr::DebugNotice(CCharacter* pSelf)
{
	std::string strNotice = pSelf->GetName();
	strNotice += RES_STRING(GM_HARMREC_H_00001);
	BOOL bSend = FALSE;
	char szHate[64];
	for (int i = 0; i < MAX_HARM_REC; i++) {
		SHarmRec* pHarm = &_HarmRec[i];
		if (pHarm->btValid > 0) {
			{
				auto _s = std::format(",{}(time={})", pHarm->sHate, pHarm->dwTime);
				std::strncpy(szHate, _s.c_str(), sizeof(szHate) - 1);
				szHate[sizeof(szHate) - 1] = 0;
			}
			strNotice += "[";
			strNotice += pHarm->pAtk->GetName();
			strNotice += szHate;
			strNotice += "]";
			bSend = TRUE;
		}
	}

	if (bSend) {
		ToLogService("common", "Notice = [{}]", strNotice.c_str());
		g_pGameApp->WorldNotice((char*)(strNotice.c_str()));
	}
}
