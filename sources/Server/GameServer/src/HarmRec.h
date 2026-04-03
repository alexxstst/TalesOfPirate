#pragma once

#include "GameApp.h"

//  -

#define MAX_HARM_REC    5
#define MAX_VALID_CNT   8


extern BOOL g_bLogHarmRec;

struct SHarmRec // 
{
	CCharacter *pAtk;		// 
	DWORD		sHarm;		// 
	DWORD       sHate;      // 
	BYTE		btValid;	// 
	DWORD		dwID;
	DWORD		dwTime;		// 

	SHarmRec(): pAtk(0), sHarm(0), sHate(0), btValid(0), dwID(0), dwTime(0)
	{
	
	}
	
	bool IsChaValid() // 
	{
		if (!pAtk)
		{
			return false;
		}
		if (!g_pGameApp->IsLiveingEntity(dwID, pAtk->GetHandle()) )
		{
			return false;
		}
		return true;
	}
};


class CHateMgr
{
public:

	CHateMgr()
	:_dwLastDecValid(0),
	 _dwLastSortTick(0)
	{
	}

	CCharacter*	GetCurTarget();
	void		AddHarm(CCharacter *pAtk, short sHarm, DWORD dwID);
	void		AddHate(CCharacter *pAtk, short sHate, DWORD dwID);
	void		UpdateHarmRec(CCharacter *pSelf);
	void		ClearHarmRec();
	void		ClearHarmRecByCha(CCharacter *pAtk);
	void		DebugNotice(CCharacter *pSelf);
	SHarmRec*   GetHarmRec(int nNo) { return &_HarmRec[nNo]; }

protected:

	SHarmRec	_HarmRec[MAX_HARM_REC];
	DWORD		_dwLastDecValid;
	DWORD		_dwLastSortTick;
};

inline CCharacter* CHateMgr::GetCurTarget()
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0 && pHarm->IsChaValid())
		{
			return pHarm->pAtk;
		}
	}
	return NULL;
}

inline void CHateMgr::ClearHarmRec()
{
	// 
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		pHarm->btValid = 0;
		pHarm->dwID    = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->pAtk    = NULL;
		pHarm->dwTime  = 0;
	}
}

inline void CHateMgr::ClearHarmRecByCha(CCharacter *pAtk)
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pAtk && pHarm->pAtk==pAtk)
		{
			pHarm->btValid = 0;
			pHarm->dwID    = 0;
			pHarm->sHarm   = 0;
			pHarm->sHate   = 0;
			pHarm->pAtk    = NULL;
			pHarm->dwTime  = 0;
		}
	}
}


inline void CHateMgr::AddHarm(CCharacter *pAtk, short sHarm, DWORD dwID)
{
	if(g_bLogHarmRec)
	{
		//LG("harm", ", [%s], %d\n", pAtk->GetName(), sHarm);
		ToLogService("common", "begin to add harm, attacker[{}], harm{}", pAtk->GetName(), sHarm);
	}
	// 
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->pAtk==pAtk && pHarm->pAtk->GetID()==dwID)
		{
			pHarm->sHarm+=sHarm;
			pHarm->sHate+=sHarm; // , 
			if(pHarm->btValid < MAX_VALID_CNT)
			{
				pHarm->btValid++;
				if(g_bLogHarmRec)
				{
					//LG("harm", "[%s], =%dvalid=%d\n", pAtk->GetName(), pHarm->sHarm, pHarm->btValid);
					ToLogService("common", "attacker[{}], accunulative harm={}, valid={}", pAtk->GetName(), pHarm->sHarm, pHarm->btValid);
				}
			}
			return;
		}
	}

	// 
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid==0)
		{
			pHarm->pAtk    = pAtk;
			pHarm->sHarm   = sHarm;
			pHarm->sHate   = sHarm;
			pHarm->btValid = 3;
			pHarm->dwID    = dwID;
			pHarm->dwTime  = g_pGameApp->m_dwRunCnt;
			if(g_bLogHarmRec)
			{
				//LG("harm", "[%s],  = %d\n", pAtk->GetName(), pHarm->sHarm);
				ToLogService("common", "add new attacker[{}], harm = {}", pAtk->GetName(), pHarm->sHarm);
			}
			break;
		}
	}
}

// 
inline void CHateMgr::AddHate(CCharacter *pAtk, short sHate, DWORD dwID)
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->pAtk==pAtk && pHarm->pAtk->GetID()==dwID)
		{
			pHarm->sHate+=sHate;
			
			if(sHate > 0)
			{
				if(pHarm->btValid < MAX_VALID_CNT)
				{
					pHarm->btValid++;
				}
			}
			else
			{
				if(pHarm->btValid > 0)
				{
					pHarm->btValid--;
				}
			}

			if(pHarm->sHate < 0) 
			{
				pHarm->sHate = 0;
			}
			return;
		}
	}

	if(sHate > 0)
	{
		// hate
		for(int i = 0; i < MAX_HARM_REC; i++)
		{
			SHarmRec *pHarm = &_HarmRec[i];
			if(pHarm->btValid==0)
			{
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

inline int CompareHarm(const void *p1, const void *p2)
{
	SHarmRec *pRec1 = (SHarmRec*)p1;
	SHarmRec *pRec2 = (SHarmRec*)p2;
	
	if(pRec1->sHate < pRec2->sHate)
	{
		return 1;
	}
	return -1;
}

inline void CHateMgr::UpdateHarmRec(CCharacter *pSelf)
{
	DWORD dwCurTick = GetTickCount();
	
	// HarmRec
	int nValid = 0;
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0 && pHarm->IsChaValid() )
		{
			memcpy(&_HarmRec[nValid], pHarm, sizeof(SHarmRec));
			nValid++;
		}
	}

	// , HarmRec, 
	for(int j = nValid; j < MAX_HARM_REC; j++)
	{
		SHarmRec *pHarm = &_HarmRec[j];
		pHarm->btValid = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->dwID    = 0;
		pHarm->pAtk    = NULL;
		pHarm->dwTime  = 0;
	}
	
	//  2 ,
	if((dwCurTick - _dwLastSortTick) > 2000)
	{
		_dwLastSortTick = dwCurTick;

		qsort(&_HarmRec, MAX_HARM_REC, sizeof(SHarmRec), CompareHarm);
		if(g_bLogHarmRec)
		{
			DebugNotice(pSelf);
		}
	}

	//  5  btValid - 1
	if((dwCurTick - _dwLastDecValid) > 5000)
	{
		_dwLastDecValid = dwCurTick;	
		for(int i = 0; i < MAX_HARM_REC; i++)
		{
			SHarmRec *pHarm = &_HarmRec[i];
			if(pHarm->btValid > 0)
			{
				pHarm->btValid--;
				if(pHarm->btValid==0) // 
				{
					pHarm->sHarm  = 0;		// 
					pHarm->sHate  = 0;		// 
					pHarm->dwTime = 0;		// 0
					pHarm->pAtk   = NULL;
					pHarm->dwID   = 0;
				}
				if(g_bLogHarmRec)
				{
					//LG("harm", "[%s]valid--, valid = %d\n", pHarm->pAtk->GetName(), pHarm->btValid);
					ToLogService("common", "attacker[{}], valid--, valid = {}", pHarm->pAtk->GetName(), pHarm->btValid);
				}
			}
		}
	}
}

inline void CHateMgr::DebugNotice(CCharacter *pSelf)
{
	std::string strNotice = pSelf->GetName();
	//strNotice+=":";
	strNotice+=RES_STRING(GM_HARMREC_H_00001);
	BOOL bSend = FALSE;
	char szHate[64];
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0)
		{
			sprintf(szHate, ",%d(time=%d)", pHarm->sHate, pHarm->dwTime);
			strNotice+="[";
			strNotice+=pHarm->pAtk->GetName();
			strNotice+=szHate;
			strNotice+="]";
			bSend = TRUE;
		}
	}

	if(bSend)
	{
		ToLogService("common", "Notice = [{}]", strNotice.c_str());
		g_pGameApp->WorldNotice((char*)(strNotice.c_str()));
	}
}

