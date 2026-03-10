#include "Character.h"
#include "script.h"
#include "GameApp.h"
#include "HarmRec.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "NPC.h"
#include <strstream>
#include <algorithm>

#include "ChaAttr.h"
#include "EventHandler.h"
#include "stdafx.h" //Add by alfred.shi 20080306

#pragma once
#pragma warning(disable: 4800)

#define PARAM_ERROR        { ToLogService("lua_ai", "lua extend function[{}]param number or type error!", __FUNCTION__ ); }
#define MAP_NULL_ERROR     { ToLogService("lua_ai", "lua extend function[{}]nonce map is null", __FUNCTION__);          }
#define CHECK_MAP          { if(g_pScriptMap==NULL) { MAP_NULL_ERROR return 0; }				    }
#define PARAM_LG_ERROR		 THROW_EXCP( excp, RES_STRING(GM_LUA_GAMECTRL_H_00001) );

extern std::list<std::string> g_luaFNList;

#define REGFN_INIT g_luaFNList.clear();

#define REGFN(fn) \
{ \
	lua_pushstring(L, "" #fn ""); \
	lua_pushcfunction(L, lua_##fn); \
	lua_settable(L, LUA_GLOBALSINDEX); \
	if(find(g_luaFNList.begin(), g_luaFNList.end(), ""#fn"") != g_luaFNList.end()) \
		ToLogService("lua", "msgind register lua the same functing[{}]", ""#fn""); \
	else \
	  g_luaFNList.push_back(""#fn""); \
} 


//--------------------------NPC-----------------------------
struct SHelpNPC
{
	char		szName[32];
	CCharacter	*pNPC;
	SHelpNPC()
	{
		strcpy(szName, "");
		pNPC = NULL;
	}
};

extern std::list<CCharacter*> g_HelpNPCList;
const char* FindHelpInfo(const char *pszKey);
void  AddHelpInfo(const char *pszKey, const char *pszInfo);
void  AddMonsterHelp(int nScriptID, int x, int y);
void  AddHelpNPC(CCharacter *pNPC);
//--------------------------------------------------------------------




// lua pcall
inline void lua_callalert(lua_State* L, int status)
{
	if (status != 0)
	{
		lua_getglobal(L, "_ALERT");
		if (lua_isfunction(L, -1))
		{
			lua_insert(L, -2);
			lua_call(L, 1, 0);
		}
		else
		{ // no _ALERT function; print it on stderr
			ToLogService("lua_err", "{}", lua_tostring(L, -2));
			lua_pop(L, 2);
		}
	}
}


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
inline int lua_EnableAI(lua_State *L)
{
	BOOL bValid = (lua_gettop(L)==1 && lua_isstring(L, 1));
	if(!bValid)
	{
		PARAM_ERROR
		return 0;
	}
	extern BOOL g_bEnableAI;
	g_bEnableAI = (BOOL)(lua_tonumber(L, 1));
	return 0;
}

// 
inline int lua_SetCurMap(lua_State *L)
{
	BOOL bValid = (lua_gettop(L)==1 && lua_isstring(L, 1));
	if(!bValid)
	{
		PARAM_ERROR
		return 0;
	}

	const char *pszName = (const char*)lua_tostring(L, 1);
	CMapRes *pMap = g_pGameApp->FindMapByName(pszName);
	if(pMap==NULL)
	{
		ToLogService("lua_ai", "can't find pointer map[{}], keep former map!", pszName);
		lua_pushnumber(L, 0);
		return 1;
	}
	lua_pushnumber(L, 1);
	g_pScriptMap = pMap->GetCopy();
	return 1;
}

inline int lua_GetChaID(lua_State *L)
{
	BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
	{
		PARAM_ERROR
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL
		return 0;
	}

	long id = pCha->m_CChaAttr.m_lID;
	lua_pushnumber(L, id);

	return 1;
}


inline int lua_CreateChaNearPlayer(lua_State *L){ CHECK_MAP
	BOOL bValid = (lua_gettop(L) == 2 && lua_isuserdata(L, 1) && lua_isnumber(L, 2));
	if (!bValid){
		PARAM_ERROR
		return 0;
	}
	CCharacter* pCha = (CCharacter*)lua_touserdata(L, 1);
	int		nScriptID = (int)lua_tonumber(L, 2);
	Point	Pos;
	Pos.x = (int)pCha->GetPos().x;
	Pos.y = (int)pCha->GetPos().y;
   
	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pCha->GetSubMap()->ChaSpawn(nScriptID, enumCHACTRL_NONE, 0, &Pos);
	if (pCCha){
		//pCCha->SetResumeTime(-1);
		lua_pushlightuserdata(L, pCCha);
		return 1;
	}else{
		ToLogService("lua_ai", "create character near role failed");
		return 0;
	}
}


// 
inline int lua_CreateCha(lua_State *L)
{
	CHECK_MAP

	// 
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 1) && lua_isnumber (L, 2) && 
	              lua_isnumber (L, 3) && lua_isnumber (L, 4) && lua_isnumber(L, 5));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    int		nScriptID = (int)lua_tonumber(L, 1);
	Point	Pos;
    Pos.x = (int)lua_tonumber(L, 2);               // 
	Pos.y = (int)lua_tonumber(L, 3);
	short sAngle = (short)lua_tonumber(L, 4);      // 
	long  lReliveTime = (int)lua_tonumber(L, 5);   // 

	ToLogService("create_cha", "create bugbear{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);
    
	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = g_pScriptMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		lua_pushlightuserdata(L, pCCha);
		return 1;
	}
	else
	{
		ToLogService("lua_ai", "create character failed");
		return 0;
	}
}

inline int lua_CreateChaX(lua_State *L)
{
	// 
	BOOL bValid = (lua_gettop (L)==6 && lua_isnumber(L, 1) && lua_isnumber (L, 2)
				&& lua_isnumber (L, 3) && lua_isnumber (L, 4) && lua_isnumber(L, 5)
				&& lua_islightuserdata(L, 6));
	if(!bValid) 
	{
		PARAM_ERROR
			return 0;
	}

	int		nScriptID = (int)lua_tonumber(L, 1);
	Point	Pos;
	Pos.x = (int)lua_tonumber(L, 2);               // 
	Pos.y = (int)lua_tonumber(L, 3);
	short sAngle = (short)lua_tonumber(L, 4);      // 
	long  lReliveTime = (int)lua_tonumber(L, 5);   // 
	CCharacter *pMainCha = (CCharacter *)lua_touserdata(L, 6);

	ToLogService("create_chaX", "create bugbear{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pMainCha->m_submap->ChaSpawn(nScriptID, enumCHACTRL_NONE, sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		lua_pushlightuserdata(L, pCCha);
		return 1;
	}
	else
	{
		ToLogService("lua_ai", "create character failed");
		return 0;
	}
}

inline int lua_CreateChaEx(lua_State *L)
{
	// 
	CHECK_MAP

	// 
	BOOL bValid = (lua_gettop (L)==6 && lua_isnumber(L, 1) && lua_isnumber (L, 2) && 
	lua_isnumber (L, 3) && lua_isnumber (L, 4) && lua_isnumber(L, 5) && lua_islightuserdata(L,6));
	if(!bValid) 
	{
		PARAM_ERROR
			return 0;
	}

	int		nScriptID = (int)lua_tonumber(L, 1);
	Point	Pos;
	Pos.x = (int)lua_tonumber(L, 2);               // 
	Pos.y = (int)lua_tonumber(L, 3);
	short sAngle = (short)lua_tonumber(L, 4);      // 
	long  lReliveTime = (int)lua_tonumber(L, 5);   // 
	SubMap * pMap = (SubMap *)lua_touserdata(L,6);
	if(!pMap)
	{
		E_LUANULL
		return 0;
	}
	ToLogService("create_chaex", "create bugbear{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		lua_pushlightuserdata(L, pCCha);
		return 1;
	}
	else
	{
		ToLogService("lua_ai", "create character failed");
		return 0;
	}
}

//
inline int lua_ChaMove(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCCha)
    {
		int x = (int)lua_tonumber(L, 2);
		int y = (int)lua_tonumber(L, 3);
		Point	Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short	sPing = 0;
		char	chPointNum = 2;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
		//pCCha->Cmd_BeginMove(0, Path, 2);
	}

	return 0;
}

// 
inline int lua_ChaMoveToSleep(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCCha)
    {
		// char szInfo[255]; sprintf(szInfo, "%s,ChaMoveToSleep\n", pCCha->GetName());
		// g_pGameApp->WorldNotice(szInfo);
		int x = (int)lua_tonumber(L, 2);
		int y = (int)lua_tonumber(L, 3);
		Point	Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short	sPing = 0;
		char	chPointNum = 2;
		char	chStopState = enumEXISTS_SLEEPING;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
		pCCha->m_CActCache.PushParam(&chStopState, sizeof(char));
		//pCCha->Cmd_BeginMove(0, Path, 2, enumEXISTS_SLEEPING);
    }

	return 0;
}

// 
inline int lua_GetChaSpawnPos(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int x = pCha->GetTerritory().centre.x;
		int y = pCha->GetTerritory().centre.y;
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;	
    }
	return 0;
}


// 
inline int lua_GetChaPatrolPos(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int x = pCha->m_nPatrolX;
		int y = pCha->m_nPatrolY;
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;	
    }
	return 0;
}

// , AI
inline int lua_SetChaPatrolState(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		pCha->m_btPatrolState = ((BYTE)lua_tonumber(L,2));

	return 0;
}

// , AI
inline int lua_GetChaPatrolState(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	lua_pushnumber(L,(BYTE)(pCha->m_btPatrolState));
	return 1;
}



// 
// []
// 
inline int lua_ChaUseSkill(lua_State *L)
{
	// 
    BOOL bValid = (lua_islightuserdata(L, 1) && lua_islightuserdata (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
	int	nParamNum = lua_gettop (L);
	if (nParamNum != 3 && !(nParamNum == 4 && lua_isnumber(L, 4)))
    {
        PARAM_ERROR
        return 0;
    }
	bool	bExecNow = false;
	if (nParamNum == 4 && ((int)lua_tonumber(L, 4) != 0)) // 
		bExecNow = true;

	CCharacter *pCha    = (CCharacter*)lua_touserdata(L, 1);
	CCharacter *pTarget = (CCharacter*)lua_touserdata(L, 2);
	if (pCha && pTarget)
    {
		long lSkillID = (int)lua_tonumber(L, 3);
		if (bExecNow)
		{
			pCha->Cmd_BeginSkillDirect(lSkillID, pTarget);
		}
		else
		{
			pCha->m_CActCache.AddCommand(enumCACHEACTION_SKILL);
			pCha->m_CActCache.PushParam(&lSkillID, sizeof(long));
			pCha->m_CActCache.PushParam(&pTarget, sizeof(CCharacter *));
		}
    }

	return 0;
}

// 
// [x,y][]
// 
inline int lua_ChaUseSkill2(lua_State *L)
{
	// 
    BOOL bValid = (lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3) &&  lua_isnumber (L, 4) &&  lua_isnumber (L, 5));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
	int	nParamNum = lua_gettop (L);
	if (nParamNum != 5 && !(nParamNum == 6 && lua_isnumber(L, 6)))
    {
        PARAM_ERROR
        return 0;
    }
	bool	bExecNow = false;
	if (nParamNum == 6 && ((int)lua_tonumber(L, 6) != 0)) // 
		bExecNow = true;

	CCharacter *pCha    = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		long lSkillID = (int)lua_tonumber(L, 2);
		long lSkillLv = (int)lua_tonumber(L, 3);
		long lPosX = (int)lua_tonumber(L, 4);
		long lPosY = (int)lua_tonumber(L, 5);
		if (bExecNow)
		{
			pCha->Cmd_BeginSkillDirect2(lSkillID, lSkillLv, lPosX, lPosY);
		}
		else
		{
			pCha->m_CActCache.AddCommand(enumCACHEACTION_SKILL2);
			pCha->m_CActCache.PushParam(&lSkillID, sizeof(long));
			pCha->m_CActCache.PushParam(&lSkillLv, sizeof(long));
			pCha->m_CActCache.PushParam(&lPosX, sizeof(long));
			pCha->m_CActCache.PushParam(&lPosY, sizeof(long));
		}
    }

	return 0;
}

// 
inline int lua_QueryChaAttr(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int nAttr = (int)lua_tonumber(L, 2);
		lua_pushnumber(L, (LONG64)pCha->getAttr(nAttr));
    }
	else
		lua_pushnumber(L, 0);

	return 1;
}

// ID
inline int lua_GetChaType(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		lua_pushnumber(L, pCha->m_pCChaRecord->nID);
	else
		lua_pushnumber(L, 0);

	return 1;
}

// , AI
inline int lua_GetChaBlockCnt(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		lua_pushnumber(L, pCha->GetBlockCnt());
	else
		lua_pushnumber(L, 0);

	return 1;
}

// , AI
inline int lua_SetChaBlockCnt(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		pCha->SetBlockCnt((BYTE)lua_tonumber(L,2));

	return 0;
}

// AI
inline int lua_GetChaAIType(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		lua_pushnumber(L, pCha->m_AIType);
	else
		lua_pushnumber(L, 0);

	return 1;
}

// 
inline int lua_GetChaChaseRange(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
		lua_pushnumber(L, pCha->m_sChaseRange);
	else
		lua_pushnumber(L, 0);

	return 1;
}

// 
inline int lua_SetChaChaseRange(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{
		pCha->m_sChaseRange = (short)lua_tonumber(L, 2);
	}
	return 0;
}


// AI
inline int lua_SetChaAIType(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int nType = (int)lua_tonumber(L, 2);
		pCha->m_AIType = nType;
		ToLogService("lua_ai", "character[{}]be set ai type is{}", pCha->GetName(), nType);
    }

	return 0;
}

// 
// 
inline int lua_GetChaTypeID(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{
		if (pCha->m_pCChaRecord)
			lua_pushnumber(L, pCha->m_pCChaRecord->lID);
		else
			lua_pushnumber(L, 0);
		return 1;
	}
	else
		return 0;
}

// ()
inline int lua_GetChaVision(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{
		lua_pushnumber(L, pCha->m_pCChaRecord->lVision);
	}
	return 1;
}

// , AI
inline int lua_GetChaSkillNum(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{
		lua_pushnumber(L, pCha->m_CSkillBag.GetSkillNum());
		return 1;
	}
	return 0;
}

// ID
inline int lua_GetChaSkillInfo(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	int nLoc = (int)lua_tonumber(L, 2);
	if (pCha)
	{
		lua_pushnumber(L, pCha->m_pCChaRecord->lSkill[nLoc][0]);
		lua_pushnumber(L, pCha->m_pCChaRecord->lSkill[nLoc][1]);
		return 2;
	}
	return 0;
}


// 
inline int lua_SetChaTarget(lua_State *L)
{
	// , target0, 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha    = (CCharacter*)lua_touserdata(L, 1);
	CCharacter *pTarget = NULL; 
	if(lua_isnumber(L, 2))
	{
		pTarget = NULL;
	}
	else
	{
		pTarget = (CCharacter*)lua_touserdata(L, 2);
	}
	if (pCha)
    {
		if (pTarget)
		{
			pCha->m_AITarget = pTarget;
			pCha->m_SFightInit.chTarType = 1;
			pCha->m_SFightInit.lTarInfo1 = pTarget->GetID();
			pCha->m_SFightInit.lTarInfo2 = pTarget->GetHandle();
			pCha->m_SMoveInit.STargetInfo.chType = 1;
			pCha->m_SMoveInit.STargetInfo.lInfo1 = pTarget->GetID();
			pCha->m_SMoveInit.STargetInfo.lInfo2 = pTarget->GetHandle();
		}
		else
		{
			pCha->m_AITarget = 0;
			pCha->m_SFightInit.chTarType = 0;
			pCha->m_SMoveInit.STargetInfo.chType = 0;
		}
    }

	return 0;
}

// 
inline int lua_GetChaTarget(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha && pCha->m_AITarget)
	{
		lua_pushlightuserdata(L, pCha->m_AITarget);
		return 1;
	}
	return 0;
}

// 
inline int lua_GetChaHost(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha && pCha->m_HostCha)
	{
		lua_pushlightuserdata(L, pCha->m_HostCha);
		return 1;
	}
	return 0;
}

// 
inline int lua_SetChaHost(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	
	CCharacter *pCha  = (CCharacter*)lua_touserdata(L, 1);
	CCharacter *pHost = NULL; 
	if(lua_isnumber(L, 2))
	{
		pHost = NULL;
	}
	else
	{
		pHost = (CCharacter*)lua_touserdata(L, 2);
	}
	if (pCha)
    {
		pCha->m_HostCha = pHost;
		if(pHost && pHost->IsPlayerCha())
		{
			int nPetNum = pHost->GetPlyMainCha()->GetPetNum();
			pHost->GetPlyMainCha()->SetPetNum(nPetNum + 1);
		}
	}
	return 0;
}

inline int lua_GetPetNum(lua_State *L)
{
	// 
	BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
	{
		PARAM_ERROR
		return 0;
	}


	CCharacter *pCha  = (CCharacter*)lua_touserdata(L, 1);
	if (pCha && pCha->GetPlyMainCha())
	{
		int nPetNum = pCha->GetPlyMainCha()->GetPetNum();
		lua_pushnumber(L, nPetNum);
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}

// 
inline int lua_GetChaFirstTarget(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		CCharacter *pTarget = pCha->m_pHate->GetCurTarget();
		if(pTarget)
		{
			lua_pushlightuserdata(L, pTarget);
			return 1;
		}
	}
	return 0;
}

// 
inline int lua_GetFirstAtker(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		CCharacter *pFirst = NULL;
		DWORD dwMinTime = 0xFFFFFFFF;
		for(int i = 0; i < MAX_HARM_REC; i++)
		{
			SHarmRec *pHarm = pCha->m_pHate->GetHarmRec(i);
			if(pHarm->btValid)
			{
				if(pHarm->IsChaValid())
				{
					if(pHarm->dwTime < dwMinTime)
					{
						dwMinTime = pHarm->dwTime;
						pFirst = pHarm->pAtk;
					}
				}
			}
		}
		if(pFirst)
		{
			lua_pushlightuserdata(L, pFirst);
			return 1;
		}
	}
	return 0;
}

// , 
inline int lua_GetChaHarmByNo(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		int nNo = (int)(lua_tonumber(L, 2));
	    SHarmRec *pHarm = pCha->m_pHate->GetHarmRec(nNo);
		if(pHarm->btValid > 0)
		{
			if(pHarm->IsChaValid())
			{
				lua_pushlightuserdata(L, pHarm->pAtk);
			}
			else
			{
				lua_pushnumber(L, 0);
			}
			lua_pushnumber(L, pHarm->sHarm);
			return 2;
		}
	}
	lua_pushnumber(L, 0);
	lua_pushnumber(L, 0);
	return 2;
}

// 
inline int lua_GetChaHateByNo(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		int nNo = (int)(lua_tonumber(L, 2));
	    SHarmRec *pHarm = pCha->m_pHate->GetHarmRec(nNo);
		if(pHarm->btValid > 0)
		{
			if(pHarm->IsChaValid())
			{
				lua_pushlightuserdata(L, pHarm->pAtk);
			}
			else
			{
				lua_pushnumber(L, 0);
			}
			lua_pushnumber(L, pHarm->sHate);
			return 2;
		}
	}
	lua_pushnumber(L, 0);
	lua_pushnumber(L, 0);
	return 2;
}

// 
inline int lua_AddHate(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_islightuserdata(L, 2) && lua_isnumber(L, 3));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pTarget = (CCharacter*)lua_touserdata(L, 1);
	CCharacter *pAtk    = (CCharacter*)lua_touserdata(L, 2);
	short sHate         = (short)lua_tonumber(L, 3);
	if(pTarget)
	{
		pTarget->m_pHate->AddHate(pAtk, sHate, pAtk->GetID());
	} 
	return 0;
}



inline int lua_GetChaPos(lua_State *L)
{
	// 
	int nPNum = lua_gettop (L);
    BOOL bValid = (nPNum==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int x = pCha->GetShape().centre.x;
		int y = pCha->GetShape().centre.y;
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;	
    }
	return 0;
}

// 
inline int lua_IsChaFighting(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha && pCha->GetFightState()==enumFSTATE_ON)
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 1;	
}

// 
inline int lua_IsChaSleeping(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha && pCha->GetExistState() == enumEXISTS_SLEEPING)
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 1;	
}





// 
// 10
inline int lua_ChaActEyeshot(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	BOOL bActive = (int)lua_tonumber(L, 2);
	if(pCha)
		pCha->ActiveEyeshot((bool)bActive);

	return 0;
}

// 
inline int lua_GetChaByRange(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);
	
	SubMap *pMap = NULL;
	
	// 
	int x = (int)lua_tonumber(L, 2); // 
	int y = (int)lua_tonumber(L, 3);
	if(pSelf) // , 
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap(); 
	}
	else // 
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}
	
	int r = (int)lua_tonumber(L, 4);    // 
	int flag = (int)lua_tonumber(L, 5); // , 0  1
	
	
	CCharacter *pCTarget = NULL;

	unsigned long	ulMinDist2 = r * r, ulTempDist2;
	long	lDistX, lDistY;
	CCharacter	*pCTempCha;
	Long	lRangeB[] = {x, y, 0};
	Long	lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE);
	while (pCTempCha = pMap->GetNextCharacterInRange())
	{
		if(pCTempCha==pSelf) continue;
		
		if (flag==0) // 
		{
			if(!pCTempCha->IsPlayerCha()) continue;
			if(pCTempCha->IsGMCha())      continue; // GM
			if(!pCTempCha->IsLiveing())   continue; //    
			if(!pCTempCha->GetActControl(enumACTCONTROL_BEUSE_SKILL)) continue; // 		
		}
		
		if (flag==1 && pCTempCha->IsPlayerCha()) continue;

		// , pk
		if(pSelf && pCTempCha->IsFriend(pSelf))
		{
			continue;
		}
		
		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2)
		{
			pCTarget = pCTempCha;
			ulMinDist2 = ulTempDist2;
		}
	}
	//...
	if(pCTarget)
	{
		lua_pushlightuserdata(L, pCTarget);
		return 1;
	}

	return 0;
}

// 
inline int lua_ClearHideChaByRange(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);
	
	SubMap *pMap = NULL;
	
	// 
	int x = (int)lua_tonumber(L, 2); // 
	int y = (int)lua_tonumber(L, 3);
	if(pSelf) // , 
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap(); 
	}
	else // 
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}
	
	int r = (int)lua_tonumber(L, 4);    // 
	int flag = (int)lua_tonumber(L, 5); // , 0  1
	
	
	CCharacter *pCTarget = NULL;

	unsigned long	ulMinDist2 = r * r, ulTempDist2;
	long	lDistX, lDistY;
	CCharacter	*pCTempCha;
	Long	lRangeB[] = {x, y, 0};
	Long	lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE, true);
	while (pCTempCha = pMap->GetNextCharacterInRange())
	{
		if(pCTempCha==pSelf) continue;
		
		if (flag==0) // 
		{
			if(!pCTempCha->IsPlayerCha()) continue;
			if(pCTempCha->IsGMCha())      continue; // GM
			if(!pCTempCha->IsLiveing())   continue; //    
		}
		
		if (flag==1 && pCTempCha->IsPlayerCha()) continue;
		
		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2)
		{
			pCTarget = pCTempCha;
			if(pCTarget->m_CSkillState.HasState(SSTATE_HIDE))
			{
				//pCTarget->SystemNotice("!");
				pCTarget->SystemNotice(RES_STRING(GM_LUA_GAMECTRL_H_00002));
				pCTarget->Show();
			}
		}
	}
	return 0;
}


// 
inline int lua_GetChaSetByRange(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);
	
	SubMap *pMap = NULL;
	
	// 
	int x = (int)lua_tonumber(L, 2); // 
	int y = (int)lua_tonumber(L, 3);
	if(pSelf) // , 
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap(); 
	}
	else // 
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}
	
	int r = (int)lua_tonumber(L, 4);            // 
	int nMonsterType = (int)lua_tonumber(L, 5); // 
	
	if (!pMap)
		return 0;
	
	CCharacter *pCTarget = NULL;

	unsigned long	ulMinDist2 = r * r, ulTempDist2;
	long	lDistX, lDistY;
	CCharacter  *pCTempCha = NULL;
	CCharacter	*ChaList[12]; // 12, idle, 412
	Long	lRangeB[] = {x, y, 0};
	Long	lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE);
	int n = 0;
	while (pCTempCha = pMap->GetNextCharacterInRange())
	{
		if(pCTempCha==pSelf) continue;
		if (pCTempCha->IsPlayerCha()) continue;

		if(nMonsterType!=0 && nMonsterType!=pCTempCha->GetCat()) continue; // 
		
		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2)
		{
			ChaList[n] = pCTempCha;
			n++;
			if(n>=12) break;
		}
	}
	
	for(int i = 0; i < n; i++)
	{
		lua_pushlightuserdata(L, ChaList[i]);
	}
	return n;
}


// 
inline int lua_FindItem(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
	if(!bValid) 
	{
		PARAM_ERROR
        return 0;
	}

	int x = (int)lua_tonumber(L, 1);
	int y = (int)lua_tonumber(L, 2);
	int r = (int)lua_tonumber(L, 3);
	Long lRangeB[] = { x, y, 0 };				  // 
	Long lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};  // 
	SubMap *pMap = g_pScriptMap;
	//pMap->BeginSearchInRange(lRangeB, lRangeE);
	//CItem* pCItem;
	//pCItem = pMap->GetNextItemInRange();
	//if(pCItem)
	//{
	//	lua_pushlightuserdata(L, (void*)pCItem);
	//	return 1;
	//}
	return 0;
}

// 
inline int lua_PickItem(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_islightuserdata(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha  = (CCharacter*)lua_touserdata(L, 1);
	CItem      *pItem = (CItem*)lua_touserdata(L, 2);

	pCha->Cmd_PickupItem(pItem->GetID(), pItem->GetHandle());
	return 0;
}

// 
inline int lua_GetItemPos(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CItem  *pItem = (CItem*)lua_touserdata(L, 1);
	const Point &p = pItem->GetPos();
	lua_pushnumber(L, p.x);
	lua_pushnumber(L, p.y);
	return 2;
}

// 
inline int lua_IsPosValid(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	bool bCanMove = false;
	if (pCha)
		bCanMove = pCha->GetSubMap()->IsMoveAble(pCha, (int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
	if(bCanMove)
		lua_pushnumber(L, 1);	
	else
		lua_pushnumber(L, 0);	

	return 1;
}


// 
#define PI 3.1415926
inline int lua_GetChaFacePos(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		int x = pCha->GetShape().centre.x;
		int y = pCha->GetShape().centre.y;
		short sAngle = pCha->GetAngle();
		float fAngle = (float) sAngle / 53.3f;
		int xOff = (int)(600.0 * cos(PI / 2 - fAngle));
		int yOff = (int)(600.0 * sin( PI / 2 - fAngle));
		lua_pushnumber(L, x + xOff);
		lua_pushnumber(L, y - yOff);
		return 2;
    }
	return 0;
}

// 
inline int lua_SetChaFaceAngle(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		pCha->SetAngle((short)lua_tonumber(L, 2));
	}
	return 0;
}

// 
inline int lua_SetChaPatrolPos(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		pCha->m_nPatrolX = (int)lua_tonumber(L,2);
		pCha->m_nPatrolY = (int)lua_tonumber(L,3);
	}
	return 0;
}


//  
inline int lua_SetChaEmotion(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		g_EventHandler.Event_ChaEmotion(pCha, (int)lua_tonumber(L, 2));
	}
	return 0;
}

inline int lua_SetChaLifeTime(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		pCha->ResetLifeTime((int)lua_tonumber(L, 2));
	}
	return 0;
}

// 
inline  int lua_HarmLog(lua_State *L)
{
	// 
    int log = (int)lua_tonumber(L, 1);
	if(log) 
	{
		g_bLogHarmRec = TRUE;
	}
	g_bLogHarmRec = FALSE;
	return 0;
}

// 
extern const char* GetResPath(const char*);
inline int lua_GetResPath(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop(L)==1 && lua_isstring(L, 1));
	if(!bValid) 
    {
		PARAM_ERROR;
		return 0;
	}
	char *pszPath = (char*)lua_tostring(L, 1);
	lua_pushstring(L, GetResPath(pszPath));
	return 1;
}


extern lua_State *g_pLuaState;
// FrameMove
inline void lua_FrameMove()
{
 	luaL_dostring(g_pLuaState, "RunTimer()");
}

// 
inline int lua_view(lua_State *L)
{
	// 
    BOOL bValid = (lua_gettop(L)==2 && lua_isnumber(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
	
	extern long g_lViewAtMapX;
	extern long g_lViewAtMapY;
	g_lViewAtMapX = (long)lua_tonumber(L, 1);
	g_lViewAtMapY = (long)lua_tonumber(L, 2);
	return 0;
}

inline void lua_AIRun(CCharacter *pCha, DWORD dwResumeExecTime)
{
	
	static int g_test[20];
	lua_getglobal(g_pLuaState, "ai_timer");
	if (!lua_isfunction(g_pLuaState, -1)) // 
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	lua_pushlightuserdata(g_pLuaState, (void *)pCha);
	lua_pushnumber(g_pLuaState, (DWORD) defCHA_SCRIPT_TIMER / 1000);
	lua_pushnumber(g_pLuaState, (DWORD) dwResumeExecTime);
	int r = lua_pcall(g_pLuaState, 3, 0, 0); 
	if(r!=0) // 
	{
		lua_callalert(g_pLuaState, r); 	
	}
	lua_settop(g_pLuaState, 0);
	
	/*
	static map<CCharacter*, int> g_ChaItem;

	if(g_ChaItem[pCha]==0)
	{
		pCha->ItemCount(pCha);
		g_ChaItem[pCha] = 1;
	}*/

	
	// clua, 
	/*
	LG("return", "begin:\n");
	for(int i = 0; i < 3; i++)
	{
		// if(lua_isnumber(g_pLuaState, - 1 - i))
		{
			LG("return", "return  = %d\n", (int)(lua_tonumber(g_pLuaState, - 1 - i))); 
		}
	}
	g_test[-1] = 0;
	lua_pop(g_pLuaState, 3);
	LG("return", "end\n\n");
*/
}

inline void lua_NPCRun(CCharacter *pCha)
{
	
	static int g_test[20];
	lua_getglobal(g_pLuaState, "npc_timer");
	if (!lua_isfunction(g_pLuaState, -1)) // 
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	lua_pushlightuserdata(g_pLuaState, (void *)pCha);
	int r = lua_pcall(g_pLuaState, 1, 0, 0); 
	if(r!=0) // 
	{
		lua_callalert(g_pLuaState, r); 	
	}
	lua_settop(g_pLuaState, 0);
}

inline int lua_GetTickCount(lua_State *L)
{
    lua_pushnumber(L, GetTickCount());
    return 1;
}

inline int lua_Msg(lua_State *L)
{
    const char *pszContent = lua_tostring(L, 1);
    MessageBox(NULL, pszContent, "msg", 0);
    return 0;
}


inline int lua_Exit(lua_State *L)
{
	extern BOOL g_bGameEnd;
	g_bGameEnd = TRUE;
	return 1;
}

inline int lua_PRINT( lua_State* L )
{
	if( g_Config.m_bLogMission == FALSE )
	{
		return 0;
	}

    int count = lua_gettop(L);
    if( count < 1 ) 
    {
		return 0;
    }

	std::ostrstream str;
    for( int i = 1; i <= count; i++ )
    {
		switch( lua_type( L, i ) )
		{
		case LUA_TNIL:
			{
				str << "nil"; 
			}
			break;
		case LUA_TBOOLEAN:
			{
				( lua_toboolean( L, i ) == 0 ) ? str << "FALSE" : str << "TRUE";
			}
			break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA:
			{
				str << "userdata:";
				const void* p = lua_touserdata( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TNUMBER:
		case LUA_TSTRING:
			{			
				const char* pszData = lua_tostring( L, i );		
				str << ( pszData ) ? pszData : "nil"; 
			}
			break;
		case LUA_TTABLE:
			{
				str << "table:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TFUNCTION:
			{
				str << "function:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;		
		case LUA_TTHREAD:
			{			
				str << "thread:";
				str << lua_tothread( L, i );
			}
			break;
		}
		str << "  ";
    }
    
    str << "\r\n";
    str << '\0';

    printf( "%s", str.str() );
	str.freeze(false);
    return 0;
}

inline int lua_LG(lua_State *L)
{
	int count = lua_gettop(L);
	if( count<=1 ) 
	{
		PARAM_ERROR;
		return 0;
	}
	const char *pszFile = lua_tostring(L, 1);
	if(g_Config.m_bLogAI==FALSE)
	{
		if(strcmp(pszFile, "lua_ai")==0)
		{
			return 0;
		}
	}
	if(strcmp(pszFile, "exp")==0)
	{
		return 0;
	}
	if( g_Config.m_bLogMission == FALSE )
	{
		if( strcmp( pszFile, "mission" ) == 0 || strcmp( pszFile, "mission_error" ) == 0 ||
			strcmp( pszFile, "trigger" ) == 0 || strcmp( pszFile, "trigger_error" ) == 0 ||
			strcmp( pszFile, "randmission_init" ) == 0 || strcmp( pszFile, "randmission_init2" ) == 0 ||
			strcmp( pszFile, "randmission_error" ) == 0 )
		{
			return 0;
		}
	}
	char szBuf[1024 * 2] = { 0 };
	std::ostrstream str( szBuf, sizeof(szBuf) );
	str << lua_tostring(L, 2);
	str << " ";
	for( int i=3; i<=count; i++ )
	{
		switch( lua_type( L, i ) )
		{
		case LUA_TNIL:
			{
				str << "nil"; 
			}
			break;
		case LUA_TBOOLEAN:
			{
				( lua_toboolean( L, i ) == 0 ) ? str << "FALSE" : str << "TRUE";
			}
			break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA:
			{
				str << "userdata:";
				const void* p = lua_touserdata( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TNUMBER:
		case LUA_TSTRING:
			{			
				const char* pszData = lua_tostring( L, i );		
				str << ( pszData ) ? pszData : "nil"; 
			}
			break;
		case LUA_TTABLE:
			{
				str << "table:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TFUNCTION:
			{
				str << "function:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;		
		case LUA_TTHREAD:
			{			
				str << "thread:";
				str << lua_tothread( L, i );
			}
			break;
		}
		str << "  ";
	}

	str << std::ends;
	ToLogService( (char*)pszFile, "{}", str.str() );
	str.freeze(false);
	return 0;
}

inline int lua_EXLG(lua_State *L)
{
	int nNumParam = lua_gettop(L);
	if( nNumParam <=1 ) 
	{
		PARAM_ERROR;
		return 0;
	}
	
	const char* pszFile = lua_tostring(L, 1);
	const char* pszTemp = lua_tostring(L, 2);
	if( !pszFile || !pszTemp )
	{
		PARAM_ERROR;
		return 0;
	}
	char  szData[1024] = {0};

	std::ostrstream str;
	USHORT sPos1 = 0, sNum = 0;
	for( int i = 3; i <= nNumParam; i++ )
	{
		const char* pszPos = strstr( pszTemp + sPos1, "%" );
		if( pszPos == NULL )
		{
			str << pszTemp + sPos1;
			break;
		}

		sNum = USHORT(pszPos - (pszTemp + sPos1));
		strncpy_s( szData,( sNum > 1020 ) ? 1020 : sNum ,pszTemp + sPos1,_TRUNCATE);
		szData[( sNum > 1020 ) ? 1020 : sNum] = 0;
		if( sNum > 1020 )
			strncat_s( szData, sizeof(szData), "...", _TRUNCATE);

		str << szData;
		switch( *(pszPos + 1) )
		{
		case 'd':
		case 's':
			{
				const char* pszData = lua_tostring( L, i );		
				( pszData ) ? str << pszData : str << "nil"; 
			}
			break;		
		case 'b':
			{
				( lua_toboolean( L, i ) == 0 ) ? str << "FALSE" : str << "TRUE";
			}
			break;
		case 'u':
			{
				str << "userdata:";
				const void* p = lua_touserdata( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case 'f':
			{
				str << "function:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case 't':
			{
				str << "table:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		default:
			{
				//str << "[(" << *(pszPos + 1) << ")]";
				str << "[noneffective signal(" << *(pszPos + 1) << ")]";
				
			}
			break;
		}
		sPos1 += sNum + 2;
	}

	str << "\r\n";
	str << '\0';
	
	ToLogService( (char*)pszFile, "{}", str.str() );
	str.freeze(false);
	return 0;
}



inline int lua_GetRoleID(lua_State *L)
{
	BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
	{
		PARAM_ERROR
		return 0;
	}

	CCharacter *pCha = (CCharacter *)lua_touserdata(L, 1);
	if(!pCha)
	{
		E_LUANULL
		return 0;
	}

    long id = pCha->GetPlayer()->GetID();
	lua_pushnumber(L, id);

	return 1;
}

inline	int	lua_UnlockItem(	lua_State*	L)
{
	BOOL bValid = (lua_gettop(L)==2 && lua_isstring(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
	{
		PARAM_ERROR
		return 0;
	}
	char	*pszChaName =	(char*)lua_tostring(	L,	1	);
	int		iItemDBID	=	(long)lua_tonumber(		L,	2	);

	CPlayer*	pPlayer	=	g_pGameApp->GetPlayerByMainChaName(	pszChaName	);
	if(	pPlayer	)
	{
		int		iCapacity	=	pPlayer->GetMainCha()->m_CKitbag.GetCapacity();

		for(	int	i	=	0;	i	<	iCapacity;	i	++	)
		{
			SItemGrid*	sig	=	pPlayer->GetMainCha()->m_CKitbag.GetGridContByID(	i	);

			if(	sig	)
			{
				if(	sig->dwDBID	==	iItemDBID	)
				{
					sig->dwDBID	=	0;
					sig->SetChange( true );
					pPlayer->GetMainCha()->SynKitbagNew(	enumSYN_KITBAG_ATTR	);
					break;
				}
			};
		};
	};
	return	0;
}

inline int lua_SetMonsterAttr(lua_State* L)
{
	BOOL bValid = (lua_gettop(L) == 3 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
	if (!bValid)
	{
		PARAM_ERROR
		return 0;
	}

	CCharacter* pCha = static_cast<CCharacter*> (lua_touserdata(L, 1));
	int AttrType = lua_tonumber(L, 2);
	int AttrVal = lua_tonumber(L, 3);
	int bRet = pCha->setAttr(AttrType, AttrVal);
	if (bRet) {
		//g_CParser.DoString("ALLExAttrSet", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END);
		pCha->SynAttr(enumATTRSYN_TASK);
	}
}


#include "lua_gamectrl2.h"

void RegisterLuaAI(lua_State *L);
void ReloadAISdk();

#define CHA_CHA     0 //  
#define CHA_SYS     1 //     (, )
#define SYS_CHA     2 //  1  (, )
#define CHA_BUY     3 //  2  (NPC)
#define CHA_SELL    4 //  NPC    ()
#define CHA_MIS     5 //  3  ()
#define MIS_CHA     6 //     
#define SYS_BOAT    7 //   ()
#define BOAT_SYS    8 //   ()
#define CHA_ENTER   9 //  
#define CHA_OUT    10 //  
#define CHA_VENDOR 11 //   
#define CHA_EXPEND 12 //  
#define CHA_DELETE 13 //  
#define CHA_BANK   14 //  
#define CHA_EQUIP  15 //  

// Log
void TL(int nType, const char *pszCha1, const char *pszCha2, const char *pszTrade);










