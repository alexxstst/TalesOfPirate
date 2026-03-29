#include "stdafx.h"
#include "lua_gamectrl.h"
#include "Birthplace.h"

using namespace std;

std::list<CCharacter*> g_HelpNPCList;

//
void AddBirthPoint(const std::string& location, const std::string& mapName, int x, int y)
{
	g_BirthMgr.AddBirthPoint(location.c_str(), mapName.c_str(), x, y);
}

//
void ClearAllBirthPoint()
{
	g_BirthMgr.ClearAll();
}

extern const char* GetResPath(const char *pszRes);
void ReloadAISdk()
{
	luaL_dofile( g_pLuaState, GetResPath("script/ai/ai.lua"));
}

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

		ToLogService("trade", "{:>7} [{:>17}] [{:>17}] [{}]", g_TradeName[nType], pszCha1, pszCha2, pszTrade);
		g_pGameApp->Log(g_TradeName[nType], pszCha1, "", pszCha2, "", pszTrade);
		sInit = 0;
	}
	else
	{
		// Add by lark.li 20080324 begin
		// End
	}
}

map<string, string> g_HelpList;

// , 2:
void AddHelpInfo(const std::string& key, const std::string& text)
{
	g_HelpList[key] = text;
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

	ToLogService("common", "now helplist amount = {}", g_HelpList.size());
}

void AddMonsterHelp(int nScriptID, int x, int y)
{
	CChaRecord	*pCChaRecord = GetChaRecordInfo(nScriptID);
	if (pCChaRecord == NULL) return;

	char szHelp[255]; sprintf(szHelp, RES_STRING(GM_LUA_GAMECTRL_CPP_00019), x/100, y/100);

	AddHelpInfo(pCChaRecord->szDataName, szHelp);
}

void AddHelpNPC(CCharacter *pNPC)
{
	ToLogService("common", "Succeed add HelpNPC[{}]", pNPC->GetName());
	g_HelpNPCList.push_back(pNPC);
}


// NPC
void AddHelpNPC_typed(const std::string& name)
{
	// NPC
	g_pGameApp->BeginGetTNpc();
	mission::CTalkNpc*	pCTNpc;
	while (pCTNpc = g_pGameApp->GetNextTNpc())
	{
		if (!strcmp(pCTNpc->GetName(), name.c_str()))
			AddHelpNPC(pCTNpc);
	}
}

void ClearHelpNPC()
{
	g_HelpNPCList.clear();
}

// DBLog
void TestDBLog(int nCnt)
{
	MPTimer t; t.Begin();
	for(int i = 0; i < nCnt; i++)
	{
		g_pGameApp->Log("newtest", "abcdefg", "1234567", "000000", "qqqppp", "abcdefghijklmnopqrstuvwxyz");
	}
	ToLogService("common", "Add Time = {}", t.End());
}

CMapRes* GetMapDataByName(const std::string& mpName)
{
	CMapRes *pMap = g_pGameApp->FindMapByName(mpName.c_str());
	if (pMap)
		return pMap;
	return nullptr;
}

void RegisterLuaAI(lua_State *L)
{
	// Raw lua_CFunction registrations (varargs / dynamic type checking)
	lua_register(L, "EXLG", EXLG_raw);
	lua_register(L, "PRINT", PRINT_raw);
	lua_register(L, "ChaUseSkill", ChaUseSkill_raw);
	lua_register(L, "ChaUseSkill2", ChaUseSkill2_raw);
	lua_register(L, "GetChaByRange", GetChaByRange_raw);
	lua_register(L, "GetChaSetByRange", GetChaSetByRange_raw);
	lua_register(L, "ClearHideChaByRange", ClearHideChaByRange_raw);
	lua_register(L, "GetChaHarmByNo", GetChaHarmByNo_raw);
	lua_register(L, "GetChaHateByNo", GetChaHateByNo_raw);
	lua_register(L, "SetChaTarget", SetChaTarget_raw);
	lua_register(L, "SetChaHost", SetChaHost_raw);
	lua_register(L, "GetChaAttrI", GetChaAttrI_raw);
	lua_register(L, "SetChaAttrI", SetChaAttrI_raw);
	lua_register(L, "LG", LG_raw);

	// LuaBridge auto-marshaled function registrations
	luabridge::getGlobalNamespace(L)
		// Utilities
		.addFunction("GetResPath", GetResPath_typed)

		// AI
		.addFunction("SetCurMap", SetCurMap)
		.addFunction("GetChaID", GetChaID)
		.addFunction("CreateChaNearPlayer", CreateChaNearPlayer)
		.addFunction("CreateCha", CreateCha)
		.addFunction("CreateChaX", CreateChaX)
		.addFunction("CreateChaEx", CreateChaEx)
		.addFunction("QueryChaAttr", QueryChaAttr)
		.addFunction("GetChaAIType", GetChaAIType)
		.addFunction("SetChaAIType", SetChaAIType)
		.addFunction("GetChaTypeID", GetChaTypeID)
		.addFunction("GetChaVision", GetChaVision)
		.addFunction("GetChaTarget", GetChaTarget)
		// SetChaTarget registered as raw above
		.addFunction("GetChaHost", GetChaHost)
		// SetChaHost registered as raw above
		.addFunction("GetPetNum", GetPetNum)
		.addFunction("GetChaFirstTarget", GetChaFirstTarget)
		.addFunction("GetChaPos", GetChaPos)
		.addFunction("GetChaBlockCnt", GetChaBlockCnt)
		.addFunction("SetChaBlockCnt", SetChaBlockCnt)
		.addFunction("ChaMove", ChaMove)
		.addFunction("ChaMoveToSleep", ChaMoveToSleep)
		.addFunction("GetChaSpawnPos", GetChaSpawnPos)
		.addFunction("SetChaPatrolState", SetChaPatrolState)
		.addFunction("GetChaPatrolState", GetChaPatrolState)
		.addFunction("GetChaPatrolPos", GetChaPatrolPos)
		.addFunction("SetChaPatrolPos", SetChaPatrolPos)
		.addFunction("SetChaFaceAngle", SetChaFaceAngle)
		.addFunction("GetChaChaseRange", GetChaChaseRange)
		.addFunction("SetChaChaseRange", SetChaChaseRange)
		// ChaUseSkill registered as raw above
		// ChaUseSkill2 registered as raw above
		// GetChaByRange registered as raw above
		// GetChaSetByRange registered as raw above
		// ClearHideChaByRange registered as raw above
		.addFunction("IsChaFighting", IsChaFighting)
		.addFunction("IsPosValid", IsPosValid)
		.addFunction("IsChaSleeping", IsChaSleeping)
		.addFunction("ChaActEyeshot", ChaActEyeshot)
		.addFunction("GetChaFacePos", GetChaFacePos)
		.addFunction("SetChaEmotion", SetChaEmotion)
		.addFunction("FindItem", FindItem)
		.addFunction("PickItem", PickItem)
		.addFunction("GetItemPos", GetItemPos)
		.addFunction("EnableAI", EnableAI)
		.addFunction("GetChaSkillNum", GetChaSkillNum)
		.addFunction("GetChaSkillInfo", GetChaSkillInfo)
		// GetChaHarmByNo registered as raw above
		.addFunction("GetFirstAtker", GetFirstAtker)
		.addFunction("AddHate", AddHate)
		// GetChaHateByNo registered as raw above
		.addFunction("HarmLog", HarmLog)
		.addFunction("SummonCha", SummonCha)
		.addFunction("DelCha", DelCha)
		.addFunction("SetChaLifeTime", SetChaLifeTime)

		//
		.addFunction("SetChaAttrMax", SetChaAttrMax)
		.addFunction("GetChaDefaultName", GetChaDefaultName)
		// GetChaAttrI registered as raw above
		// SetChaAttrI registered as raw above
		.addFunction("IsPlayer", IsPlayer)
		.addFunction("IsChaInRegion", IsChaInRegion)

		//
		.addFunction("IsChaInTeam", IsChaInTeam)
		.addFunction("GetTeamCha", GetTeamCha)

		//
		.addFunction("AddBirthPoint", AddBirthPoint)
		.addFunction("ClearAllBirthPoint", ClearAllBirthPoint)

		//
		.addFunction("AddWeatherRegion", AddWeatherRegion)
		.addFunction("ClearMapWeather", ClearMapWeather)

		// NPC
		.addFunction("AddHelpInfo", static_cast<void(*)(const std::string&, const std::string&)>(AddHelpInfo))
		.addFunction("AddHelpNPC", AddHelpNPC_typed)
		.addFunction("ClearHelpNPC", ClearHelpNPC)

		//
		.addFunction("SetBoatCtrlTick", SetBoatCtrlTick)
		.addFunction("GetBoatCtrlTick", GetBoatCtrlTick)

		.addFunction("GetRoleID", GetRoleID)
		.addFunction("UnlockItem", UnlockItem)
		.addFunction("SetMonsterAttr", SetMonsterAttr)
		//
		.addFunction("TestDBLog", TestDBLog)

		// Utility (not in RegisterLuaAI originally but defined in gamectrl)
		.addFunction("GetMapDataByName", GetMapDataByName)
		.addFunction("view", view);
}
