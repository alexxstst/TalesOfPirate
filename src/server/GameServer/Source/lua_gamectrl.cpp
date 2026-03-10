#include "stdafx.h"
#include "lua_gamectrl.h"
#include "Birthplace.h"

using namespace std;

std::list<CCharacter*> g_HelpNPCList;

// 
int lua_AddBirthPoint(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop(L)==4 && lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4));
	if(!bValid) 
    {
		PARAM_ERROR;
		return 0;
	}

	const char *pszLocation = lua_tostring(L, 1);
	const char *pszMapName  = lua_tostring(L, 2);
	int x = (int)lua_tonumber(L, 3);
	int y = (int)lua_tonumber(L, 4);

	g_BirthMgr.AddBirthPoint(pszLocation, pszMapName, x, y);
	return 0;
}

// 
int lua_ClearAllBirthPoint(lua_State *L)
{
	g_BirthMgr.ClearAll();
	return 0;
}

extern const char* GetResPath(const char *pszRes);
void ReloadAISdk()
{
	luaL_dofile( g_pLuaState, GetResPath("script/ai/ai.lua"));
}

//char g_TradeName[][32] = 
//{
//	"",
//	"",	
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	""
//};
const char* g_TradeName[] = 
{
RES_STRING(GM_LUA_GAMECTRL_CPP_00001),
RES_STRING(GM_LUA_GAMECTRL_CPP_00002),
RES_STRING(GM_LUA_GAMECTRL_CPP_00003),
RES_STRING(GM_LUA_GAMECTRL_CPP_00004),
RES_STRING(GM_LUA_GAMECTRL_CPP_00005),
RES_STRING(GM_LUA_GAMECTRL_CPP_00006),
RES_STRING(GM_LUA_GAMECTRL_CPP_00007),
RES_STRING(GM_LUA_GAMECTRL_CPP_00008),
RES_STRING(GM_LUA_GAMECTRL_CPP_00009),
RES_STRING(GM_LUA_GAMECTRL_CPP_00010),
RES_STRING(GM_LUA_GAMECTRL_CPP_00011),
RES_STRING(GM_LUA_GAMECTRL_CPP_00012),
RES_STRING(GM_LUA_GAMECTRL_CPP_00013),
RES_STRING(GM_LUA_GAMECTRL_CPP_00014),
RES_STRING(GM_LUA_GAMECTRL_CPP_00015),
RES_STRING(GM_LUA_GAMECTRL_CPP_00016),
};

#define TL_TIME_ONE_HOUR			6*60*60*1000
void TL(int nType, const char *pszCha1, const char *pszCha2, const char *pszTrade)
{
	if(!g_Config.m_bLogDB)
	{
		static short sInit = 1;
		static std::string strName;
		static DWORD dwLastTime = -1;
		static DWORD dwCount = 10000;
		if( dwCount++ > 1000 )
		{
			DWORD dwTime = GetTickCount();
			if( dwTime - dwLastTime >= TL_TIME_ONE_HOUR || sInit == 1 )
			{
				dwCount = 0;
				strName = "trade";
				dwLastTime = dwTime;
				SYSTEMTIME st;
				GetLocalTime( &st );
				char szData[128];
				sprintf( szData, "%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour );
				strName += szData;
			}
		}

		ToLogService(strName.c_str(), "{} [{}] [{}] [{}]", g_TradeName[nType], pszCha1, pszCha2, pszTrade);
		g_pGameApp->Log(g_TradeName[nType], pszCha1, "", pszCha2, "", pszTrade); 
		sInit = 0;
	}
	else
	{
		// Add by lark.li 20080324 begin
		//static CThrdLock lock;
		//lock.Lock();
		//g_pGameApp->TradeLog(g_TradeName[nType], pszCha1, pszCha2,pszTrade);
		//lock.Unlock();
		// End
	}
}

map<string, string> g_HelpList;

// , 2:  
int lua_AddHelpInfo(lua_State *L)
{
	BOOL bValid = (lua_gettop(L)==2  && lua_isstring(L, 1) && lua_isstring(L, 2));
	if(!bValid)
	{
		return 0;
	}
	
	const char *pszKey  = (const char*)lua_tostring(L, 1);
	const char *pszText = (const char*)lua_tostring(L, 2);

	g_HelpList[pszKey] = pszText;
	
	return 0;
}

const char* FindHelpInfo(const char *pszKey)
{
	map<string, string>::iterator it = g_HelpList.find(pszKey);
	if(it==g_HelpList.end())
	{
		return NULL;
	}
	return (*it).second.c_str();
}

void AddHelpInfo(const char *pszKey, const char *pszInfo)
{
	if(strlen(pszKey)==0)  return;
	if(strlen(pszInfo)==0) return;

	g_HelpList[pszKey] = pszInfo;

	ToLogService("help", "now helplist amount = {}", g_HelpList.size());
}

void AddMonsterHelp(int nScriptID, int x, int y)
{
	CChaRecord	*pCChaRecord = GetChaRecordInfo(nScriptID);
	if (pCChaRecord == NULL) return;	

	//char szHelp[255]; sprintf(szHelp, "%d, %d!", x/100, y/100);
	char szHelp[255]; sprintf(szHelp, RES_STRING(GM_LUA_GAMECTRL_CPP_00019), x/100, y/100);

	AddHelpInfo(pCChaRecord->szDataName, szHelp);
}

void AddHelpNPC(CCharacter *pNPC)
{
	ToLogService("init", "Succeed add HelpNPC[{}]", pNPC->GetName());
	g_HelpNPCList.push_back(pNPC);
}


// NPC
int lua_AddHelpNPC(lua_State *L)
{
	BOOL bValid = (lua_gettop(L)==1 && lua_isstring(L, 1));
	if(!bValid)
	{
		return 0;
	}
	
	const char *pszName  = (const char*)lua_tostring(L, 1); // NPC
	
	// NPC
	g_pGameApp->BeginGetTNpc();
	mission::CTalkNpc*	pCTNpc;
	while (pCTNpc = g_pGameApp->GetNextTNpc())
	{
		if (!strcmp(pCTNpc->GetName(), pszName))
			AddHelpNPC(pCTNpc);
	}
	
	return 0;
}

int lua_ClearHelpNPC(lua_State *L)
{
	g_HelpNPCList.clear();
	return 0;
}

// DBLog
int lua_TestDBLog(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	int nCnt = (int)lua_tonumber(L, 1);

	MPTimer t; t.Begin();
	for(int i = 0; i < nCnt; i++)
	{
		g_pGameApp->Log("newtest", "abcdefg", "1234567", "000000", "qqqppp", "abcdefghijklmnopqrstuvwxyz");
	}
	ToLogService("dblog", "Add Time = {}", t.End());
	
	return 0;
}

int lua_GetMapDataByName(lua_State *L)
{
	BOOL bValid = (lua_gettop(L) == 1 && lua_isstring(L, 1));
	if (!bValid)
	{
		PARAM_ERROR
			return 0;
	}
	const char* mpName = lua_tostring(L, 1);
	if (mpName == NULL)
	{
		PARAM_ERROR;
		return 0;
	}
	CMapRes *pMap = g_pGameApp->FindMapByName(mpName);
	if (pMap) {
		lua_pushlightuserdata(L, (CMapRes*)pMap);
		return 1;
	}
	return 0;

}

void RegisterLuaAI(lua_State *L)
{
	
	// 
	REGFN(view);
	REGFN(EXLG);
	REGFN(PRINT);
	REGFN(GetResPath);

	// For AI
	
	REGFN(SetCurMap);
	REGFN(GetChaID);
	REGFN(CreateChaNearPlayer);
	REGFN(CreateCha);
	REGFN(CreateChaX);
	REGFN(CreateChaEx);
	REGFN(QueryChaAttr); 
	REGFN(GetChaAIType);
	REGFN(SetChaAIType);
	REGFN(GetChaTypeID);
	REGFN(GetChaVision);
	REGFN(GetChaTarget);
	REGFN(SetChaTarget);
	REGFN(GetChaHost);
	REGFN(SetChaHost);
	REGFN(GetPetNum);
	REGFN(GetChaFirstTarget);
	REGFN(GetChaPos);
	REGFN(GetChaBlockCnt);
	REGFN(SetChaBlockCnt);
	REGFN(ChaMove);
	REGFN(ChaMoveToSleep);
	REGFN(GetChaSpawnPos);
	REGFN(SetChaPatrolState);
	REGFN(GetChaPatrolState);
	REGFN(GetChaPatrolPos);
	REGFN(SetChaPatrolPos);
	REGFN(SetChaFaceAngle);
	REGFN(GetChaChaseRange);
	REGFN(SetChaChaseRange);
	REGFN(ChaUseSkill);
	REGFN(ChaUseSkill2);
	REGFN(GetChaByRange);
	REGFN(GetChaSetByRange);
	REGFN(ClearHideChaByRange);
	REGFN(IsChaFighting);
	REGFN(IsPosValid);
	REGFN(IsChaSleeping);
	REGFN(ChaActEyeshot);
	REGFN(GetChaFacePos);
	REGFN(SetChaEmotion);
	REGFN(FindItem);
	REGFN(PickItem);
	REGFN(GetItemPos);
	REGFN(EnableAI);
	REGFN(GetChaSkillNum);
	REGFN(GetChaSkillInfo);
	REGFN(GetChaHarmByNo);
	REGFN(GetFirstAtker);
	REGFN(AddHate);
	REGFN(GetChaHateByNo);
	REGFN(HarmLog);
	REGFN(SummonCha);
	REGFN(DelCha);
	REGFN(SetChaLifeTime);
	
	// 
	REGFN(SetChaAttrMax);
	REGFN(GetChaDefaultName);
	REGFN(SetChaAttrI);
	REGFN(GetChaAttrI);
	REGFN(IsPlayer);
	REGFN(IsChaInRegion);
	
	// 
	REGFN(IsChaInTeam);
	REGFN(GetTeamCha);

	// 
	REGFN(AddBirthPoint);
	REGFN(ClearAllBirthPoint);

	// 
	REGFN(AddWeatherRegion);
	REGFN(ClearMapWeather);

	// NPC
	REGFN(AddHelpInfo);
	REGFN(AddHelpNPC);
	REGFN(ClearHelpNPC);

	// 
	REGFN(SetBoatCtrlTick);
	REGFN(GetBoatCtrlTick);

    REGFN(GetRoleID);
	REGFN(UnlockItem);
	REGFN(SetMonsterAttr);
	// 
	REGFN(TestDBLog);
	
}


/*
				
  
:	, summon

: summon, 

: summon, 

: , summon
	
:	AI

    function() 
	
		if()
		{
			
		}
		else
		{
			, 
		}

		
		if()
		{
			
		}
		
	end

  
	function() 
		
		if( ||  || )
		{
			
		}
		else
		{
			
		}
	end

 
: 

    1: 
    2: 
    3: 

    1, 23



*/











































































































