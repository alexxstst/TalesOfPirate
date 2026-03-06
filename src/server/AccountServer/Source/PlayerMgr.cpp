#define _CRTDBG_MAP_ALLOC
#include "atltime.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "AccountServer2.h"
#include ".\playermgr.h"
//#include "MyThread.h"
#include "GlobalVariable.h"

struct sPlayerData
{
	CTime ctLoginTime;
};


CPlayerMgr::CPlayerMgr(void)
{
}

CPlayerMgr::~CPlayerMgr(void)
{
}

void CPlayerMgr::Initial()
{
	m_mapPlayers.clear();
}

void CPlayerMgr::PlayerLogin(std::string strPlayerName)
{
	if (!strPlayerName.c_str())
	{
		printf("User name is <NULL> where login!\r\n");
		return;
	}
	sPlayerData sData;
	sData.ctLoginTime=CTime::GetCurrentTime();
	m_mapPlayers[strPlayerName.c_str()] = sData;
}

void CPlayerMgr::PlayerLogout(std::string strPlayerName)
{
	if (!strPlayerName.c_str())
	{
		printf("User name is <NULL> where logout!\r\n");
		return;
	}
	StringMap::const_iterator iter=m_mapPlayers.find(strPlayerName.c_str());
	if (iter==m_mapPlayers.end())
	{
		printf("Unable update the user live time when logout! User=[%s]\r\n", strPlayerName.c_str());
		return;
	}
	AuthThread* pThread = (AuthThread *)(g_TlsIndex.GetPointer());
	if (!pThread)
	{
		printf("Get AuthThread Error!\r\n");
		return;
	}
	CSQLDatabase *pDB = pThread->GetSQLDatabase();

	sPlayerData sData=iter->second;
	CTimeSpan ctSpan=CTime::GetCurrentTime() - sData.ctLoginTime;
	m_mapPlayers.erase(strPlayerName.c_str());
}
