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

	SHarmRec();

	bool IsChaValid(); //
};


class CHateMgr
{
public:
	CHateMgr();

	CCharacter*	GetCurTarget();
	void		AddHarm(CCharacter *pAtk, short sHarm, DWORD dwID);
	void		AddHate(CCharacter *pAtk, short sHate, DWORD dwID);
	void		UpdateHarmRec(CCharacter *pSelf);
	void		ClearHarmRec();
	void		ClearHarmRecByCha(CCharacter *pAtk);
	void		DebugNotice(CCharacter *pSelf);
	SHarmRec*   GetHarmRec(int nNo);

protected:
	SHarmRec	_HarmRec[MAX_HARM_REC];
	DWORD		_dwLastDecValid;
	DWORD		_dwLastSortTick;
};

int CompareHarm(const void* p1, const void* p2);
