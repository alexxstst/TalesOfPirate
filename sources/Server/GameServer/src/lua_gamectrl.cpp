#include "stdafx.h"
#include "lua_gamectrl.h"
#include "Birthplace.h"

using namespace std;

std::list<CCharacter*> g_HelpNPCList;

// 添加出生地与出生点
int lua_AddBirthPoint(lua_State *L)
{
	// 参数合法性判别
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
	//LG("birth", "添加出生点[%s] [%s] %d %d\n", pszLocation, pszMapName, x, y);
	return 0;
}

// 清除所有出生地与出生点
int lua_ClearAllBirthPoint(lua_State *L)
{
	g_BirthMgr.ClearAll();
	//LG("birth", "清除了所有出生点\n");
	return 0;
}

extern const char* GetResPath(const char *pszRes);
void ReloadAISdk()
{
	luaL_dofile( g_pLuaState, GetResPath("script/ai/ai.lua"));
}

//char g_TradeName[][32] = 
//{
//	"人人",
//	"扔",	
//	"捡",
//	"店买",
//	"卖",
//	"任务给",
//	"任务收",
//	"买货",
//	"卖货",
//	"进游戏",
//	"出游戏",
//	"人摊",
//	"消耗",
//	"删除",
//	"银行",
//	"装备"
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

		LG(strName.c_str(), "%7s [%17s] [%17s] [%s]\n", g_TradeName[nType], pszCha1, pszCha2, pszTrade); 
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

// 添加帮助信息, 接受2个参数: 关键字 帮助文字
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

	//LG("help", "目前帮助条目数 = %d\n", g_HelpList.size());
	LG("help", "now helplist amount = %d\n", g_HelpList.size());
}

void AddMonsterHelp(int nScriptID, int x, int y)
{
	CChaRecord	*pCChaRecord = GetChaRecordInfo(nScriptID);
	if (pCChaRecord == NULL) return;	

	//char szHelp[255]; sprintf(szHelp, "听说在本海域的%d, %d附近有你打听的生物出没!", x/100, y/100);
	char szHelp[255]; sprintf(szHelp, RES_STRING(GM_LUA_GAMECTRL_CPP_00019), x/100, y/100);

	AddHelpInfo(pCChaRecord->szDataName, szHelp);
}

void AddHelpNPC(CCharacter *pNPC)
{
	//LG("init", "成功添加帮助NPC[%s]\n", pNPC->GetName());
	LG("init", "Succeed add HelpNPC[%s]\n", pNPC->GetName());
	g_HelpNPCList.push_back(pNPC);
}


// 通过脚本添加帮助NPC
int lua_AddHelpNPC(lua_State *L)
{
	BOOL bValid = (lua_gettop(L)==1 && lua_isstring(L, 1));
	if(!bValid)
	{
		return 0;
	}
	
	const char *pszName  = (const char*)lua_tostring(L, 1); // 获得帮助NPC的名字
	
	// 按名字查找NPC对象
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

// 测试DBLog
int lua_TestDBLog(lua_State *L)
{
	// 参数合法性判别
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
	LG("dblog", "Add Time = %d\n", t.End());
	
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
	
	// 通用
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
	
	// 数值计算
	REGFN(SetChaAttrMax);
	REGFN(GetChaDefaultName);
	REGFN(SetChaAttrI);
	REGFN(GetChaAttrI);
	REGFN(IsPlayer);
	REGFN(IsChaInRegion);
	
	// 组队
	REGFN(IsChaInTeam);
	REGFN(GetTeamCha);

	// 出生地与出生点
	REGFN(AddBirthPoint);
	REGFN(ClearAllBirthPoint);

	// 天气区域
	REGFN(AddWeatherRegion);
	REGFN(ClearMapWeather);

	// 帮助NPC
	REGFN(AddHelpInfo);
	REGFN(AddHelpNPC);
	REGFN(ClearHelpNPC);

	// 船只计时
	REGFN(SetBoatCtrlTick);
	REGFN(GetBoatCtrlTick);

    REGFN(GetRoleID);
	REGFN(UnlockItem);
	REGFN(SetMonsterAttr);
	// 测试脚本
	REGFN(TestDBLog);
	
}


/*
				卡片召唤宠物实现流程
  
一:	使用卡片道具, 执行summon怪物

二: 为summon出来的怪物设置定时消失的技能状态, 状态持续时间就是怪物的生命时间

三: summon出来的怪物，如果技能状态时间到后消失, 则怪物被清除

四: 主人经过跳转点时, summon出来的怪物被自动清除
	
五:	宠物AI

    function() 没有目标
	
		if(检测附近是否有合适的目标)
		{
			设置目标对象
		}
		else
		{
			如果距离主人太远, 则靠近
		}

		取出攻击主人的对象列表的第一个
		if(不为空)
		{
			设置目标对象
		}
		
	end

  
	function() 有目标
		
		if(目标距离主人太远 || 目标已死 || 目标下线已不存在)
		{
			清除目标
		}
		else
		{
			对目标使用技能
		}
	end

 
六: 卡片的属性设置

    属性1: 怪物编号
    属性2: 标识怪物等级的数值
    属性3: 可以使用的次数

    属性1当获得卡片时产生, 属性2和属性3可以动态改变



*/











































































































