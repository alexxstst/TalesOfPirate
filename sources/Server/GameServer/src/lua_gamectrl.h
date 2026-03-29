#include "Character.h"
#include "script.h"
#include "GameApp.h"
#include "HarmRec.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "NPC.h"
#include <print>
#include <strstream>
#include <algorithm>
#include "ChaAttr.h"
#include "EventHandler.h"
#include "stdafx.h" //Add by alfred.shi 20080306

#pragma once
#pragma warning(disable: 4800)

//    Lua-
#define PARAM_ERROR        { ToLogService("lua", LogLevel::Error, "lua extend function[{}] param number or type error!", __FUNCTION__); }
#define MAP_NULL_ERROR     { ToLogService("lua", LogLevel::Error, "lua extend function[{}] nonce map is null", __FUNCTION__); }
#define CHECK_MAP          { if(g_pScriptMap==NULL) { MAP_NULL_ERROR return 0; }				    }
#define PARAM_LG_ERROR		 THROW_EXCP( excp, RES_STRING(GM_LUA_GAMECTRL_H_00001) );



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
void  AddHelpInfo(const std::string& key, const std::string& text);
void  AddHelpNPC_typed(const std::string& name);
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
			ToLogService("lua", LogLevel::Error, "{}", lua_tostring(L, -2));
			lua_pop(L, 2);
		}
	}
}


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
inline void EnableAI(int flag)
{
	extern BOOL g_bEnableAI;
	g_bEnableAI = (BOOL)flag;
}

//
inline int SetCurMap(const std::string& name)
{
	CMapRes *pMap = g_pGameApp->FindMapByName(name.c_str());
	if(pMap==NULL)
	{
		ToLogService("lua", "can't find pointer map[{}], keep former map!", name.c_str());
		return 0;
	}
	g_pScriptMap = pMap->GetCopy();
	return 1;
}

inline int GetChaID(CCharacter* pCha)
{
	if(!pCha)
		return 0;
	return (int)pCha->m_CChaAttr.m_lID;
}


inline CCharacter* CreateChaNearPlayer(CCharacter* pCha, int nScriptID)
{
	if(!g_pScriptMap) return nullptr;
	if(!pCha) return nullptr;
	Point Pos;
	Pos.x = (int)pCha->GetPos().x;
	Pos.y = (int)pCha->GetPos().y;

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pCha->GetSubMap()->ChaSpawn(nScriptID, enumCHACTRL_NONE, 0, &Pos);
	if (pCCha){
		return pCCha;
	}else{
		ToLogService("lua", "create character near role failed");
		return nullptr;
	}
}


//
inline CCharacter* CreateCha(int nScriptID, int x, int y, int sAngle, int lReliveTime)
{
	if(!g_pScriptMap) return nullptr;

	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbear{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = g_pScriptMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else
	{
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

inline CCharacter* CreateChaX(int nScriptID, int x, int y, int sAngle, int lReliveTime, CCharacter* pMainCha)
{
	if(!pMainCha) return nullptr;
	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbearX{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pMainCha->m_submap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else
	{
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

inline CCharacter* CreateChaEx(int nScriptID, int x, int y, int sAngle, int lReliveTime, SubMap* pMap)
{
	if(!g_pScriptMap) return nullptr;
	if(!pMap) return nullptr;

	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbearEx{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle, lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter *pCCha = pMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha)
	{
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else
	{
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

//
inline void ChaMove(CCharacter* pCCha, int x, int y)
{
	if (pCCha)
    {
		Point	Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short	sPing = 0;
		char	chPointNum = 2;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
	}
}

//
inline void ChaMoveToSleep(CCharacter* pCCha, int x, int y)
{
	if (pCCha)
    {
		Point	Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short	sPing = 0;
		char	chPointNum = 2;
		char	chStopState = enumEXISTS_SLEEPING;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
		pCCha->m_CActCache.PushParam(&chStopState, sizeof(char));
    }
}

//
inline std::tuple<int, int> GetChaSpawnPos(CCharacter* pCha)
{
	if (pCha)
    {
		return std::make_tuple(pCha->GetTerritory().centre.x, pCha->GetTerritory().centre.y);
    }
	return std::make_tuple(0, 0);
}


//
inline std::tuple<int, int> GetChaPatrolPos(CCharacter* pCha)
{
	if (pCha)
    {
		return std::make_tuple(pCha->m_nPatrolX, pCha->m_nPatrolY);
    }
	return std::make_tuple(0, 0);
}

// , AI
inline void SetChaPatrolState(CCharacter* pCha, int state)
{
	if (pCha)
		pCha->m_btPatrolState = (BYTE)state;
}

// , AI
inline int GetChaPatrolState(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)(BYTE)(pCha->m_btPatrolState);
}



//
// []
//
// Varargs: optional 4th param for immediate execution
inline int ChaUseSkill_raw(lua_State *L)
{
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
	if (nParamNum == 4 && ((int)lua_tonumber(L, 4) != 0))
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
// Varargs: optional 6th param for immediate execution
inline int ChaUseSkill2_raw(lua_State *L)
{
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
	if (nParamNum == 6 && ((int)lua_tonumber(L, 6) != 0))
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
inline int QueryChaAttr(CCharacter* pCha, int nAttr)
{
	if (pCha)
		return (int)(LONG64)pCha->getAttr(nAttr);
	return 0;
}

// ID
inline int GetChaType(CCharacter* pCha)
{
	if (pCha)
		return pCha->m_pCChaRecord->nID;
	return 0;
}

// , AI
inline int GetChaBlockCnt(CCharacter* pCha)
{
	if (pCha)
		return pCha->GetBlockCnt();
	return 0;
}

// , AI
inline void SetChaBlockCnt(CCharacter* pCha, int cnt)
{
	if (pCha)
		pCha->SetBlockCnt((BYTE)cnt);
}

// AI
inline int GetChaAIType(CCharacter* pCha)
{
	if (pCha)
		return pCha->m_AIType;
	return 0;
}

//
inline int GetChaChaseRange(CCharacter* pCha)
{
	if (pCha)
		return pCha->m_sChaseRange;
	return 0;
}

//
inline void SetChaChaseRange(CCharacter* pCha, int range)
{
	if (pCha)
		pCha->m_sChaseRange = (short)range;
}


// AI
inline void SetChaAIType(CCharacter* pCha, int nType)
{
	if (pCha)
    {
		pCha->m_AIType = nType;
		ToLogService("lua", "character[{}] be set ai type is {}", pCha->GetName(), nType);
    }
}

//
//
inline int GetChaTypeID(CCharacter* pCha)
{
	if (pCha && pCha->m_pCChaRecord)
		return (int)pCha->m_pCChaRecord->lID;
	return 0;
}

// ()
inline int GetChaVision(CCharacter* pCha)
{
	if (pCha)
		return (int)pCha->m_pCChaRecord->lVision;
	return 0;
}

// , AI
inline int GetChaSkillNum(CCharacter* pCha)
{
	if (pCha)
		return pCha->m_CSkillBag.GetSkillNum();
	return 0;
}

// ID
inline std::tuple<int, int> GetChaSkillInfo(CCharacter* pCha, int nLoc)
{
	if (pCha)
		return std::make_tuple((int)pCha->m_pCChaRecord->lSkill[nLoc][0], (int)pCha->m_pCChaRecord->lSkill[nLoc][1]);
	return std::make_tuple(0, 0);
}


//
// Dynamic type checking on arg 2 (number = clear target, lightuserdata = set target)
inline int SetChaTarget_raw(lua_State *L)
{
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
inline CCharacter* GetChaTarget(CCharacter* pCha)
{
	if(pCha && pCha->m_AITarget)
		return pCha->m_AITarget;
	return nullptr;
}

//
inline CCharacter* GetChaHost(CCharacter* pCha)
{
	if(pCha && pCha->m_HostCha)
		return pCha->m_HostCha;
	return nullptr;
}

//
// Dynamic type checking on arg 2 (number = clear host, lightuserdata = set host)
inline int SetChaHost_raw(lua_State *L)
{
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

inline int GetPetNum(CCharacter* pCha)
{
	if (pCha && pCha->GetPlyMainCha())
		return pCha->GetPlyMainCha()->GetPetNum();
	return 0;
}

//
inline CCharacter* GetChaFirstTarget(CCharacter* pCha)
{
	if(pCha)
	{
		CCharacter *pTarget = pCha->m_pHate->GetCurTarget();
		if(pTarget)
			return pTarget;
	}
	return nullptr;
}

//
inline CCharacter* GetFirstAtker(CCharacter* pCha)
{
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
			return pFirst;
	}
	return nullptr;
}

// ,
// Dynamic return: first value is either lightuserdata or number 0
inline int GetChaHarmByNo_raw(lua_State *L)
{
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
				luabridge::push(L, static_cast<CCharacter*>(pHarm->pAtk));
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
// Dynamic return: first value is either lightuserdata or number 0
inline int GetChaHateByNo_raw(lua_State *L)
{
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
				luabridge::push(L, static_cast<CCharacter*>(pHarm->pAtk));
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
inline void AddHate(CCharacter* pTarget, CCharacter* pAtk, int sHate)
{
	if(pTarget && pAtk)
		pTarget->m_pHate->AddHate(pAtk, (short)sHate, pAtk->GetID());
}



inline std::tuple<int, int> GetChaPos(CCharacter* pCha)
{
	if (pCha)
		return std::make_tuple(pCha->GetShape().centre.x, pCha->GetShape().centre.y);
	return std::make_tuple(0, 0);
}

//
inline int IsChaFighting(CCharacter* pCha)
{
	if(pCha && pCha->GetFightState()==enumFSTATE_ON)
		return 1;
	return 0;
}

//
inline int IsChaSleeping(CCharacter* pCha)
{
	if(pCha && pCha->GetExistState() == enumEXISTS_SLEEPING)
		return 1;
	return 0;
}



//
// 10
inline void ChaActEyeshot(CCharacter* pCha, int bActive)
{
	if(pCha)
		pCha->ActiveEyeshot((bool)bActive);
}

//
// First arg can be nil (pSelf=nullptr), uses global map fallback + complex search logic
inline int GetChaByRange_raw(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid)
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);

	SubMap *pMap = NULL;

	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if(pSelf)
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap();
	}
	else
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}

	int r = (int)lua_tonumber(L, 4);
	int flag = (int)lua_tonumber(L, 5);


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

		if (flag==0)
		{
			if(!pCTempCha->IsPlayerCha()) continue;
			if(pCTempCha->IsGMCha())      continue;
			if(!pCTempCha->IsLiveing())   continue;
			if(!pCTempCha->GetActControl(enumACTCONTROL_BEUSE_SKILL)) continue;
		}

		if (flag==1 && pCTempCha->IsPlayerCha()) continue;

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
	if(pCTarget)
	{
		luabridge::push(L, static_cast<CCharacter*>(pCTarget));
		return 1;
	}

	return 0;
}

//
// First arg can be nil (pSelf=nullptr), uses global map fallback
inline int ClearHideChaByRange_raw(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid)
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);

	SubMap *pMap = NULL;

	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if(pSelf)
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap();
	}
	else
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}

	int r = (int)lua_tonumber(L, 4);
	int flag = (int)lua_tonumber(L, 5);


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

		if (flag==0)
		{
			if(!pCTempCha->IsPlayerCha()) continue;
			if(pCTempCha->IsGMCha())      continue;
			if(!pCTempCha->IsLiveing())   continue;
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
				pCTarget->SystemNotice(RES_STRING(GM_LUA_GAMECTRL_H_00002));
				pCTarget->Show();
			}
		}
	}
	return 0;
}


//
// Variable number of returns (up to 12)
inline int GetChaSetByRange_raw(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==5 && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
    if(!bValid)
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pSelf = (CCharacter*)lua_touserdata(L, 1);

	SubMap *pMap = NULL;

	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if(pSelf)
	{
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap();
	}
	else
	{
		CHECK_MAP
		pMap = g_pScriptMap;
	}

	int r = (int)lua_tonumber(L, 4);
	int nMonsterType = (int)lua_tonumber(L, 5);

	if (!pMap)
		return 0;

	CCharacter *pCTarget = NULL;

	unsigned long	ulMinDist2 = r * r, ulTempDist2;
	long	lDistX, lDistY;
	CCharacter  *pCTempCha = NULL;
	CCharacter	*ChaList[12];
	Long	lRangeB[] = {x, y, 0};
	Long	lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE);
	int n = 0;
	while (pCTempCha = pMap->GetNextCharacterInRange())
	{
		if(pCTempCha==pSelf) continue;
		if (pCTempCha->IsPlayerCha()) continue;

		if(nMonsterType!=0 && nMonsterType!=pCTempCha->GetCat()) continue;

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
		luabridge::push(L, static_cast<CCharacter*>(ChaList[i]));
	}
	return n;
}


//
inline int FindItem(int x, int y, int r)
{
	// Commented out in original - always returns 0
	return 0;
}

//
inline void PickItem(CCharacter* pCha, CItem* pItem)
{
	if(pCha && pItem)
		pCha->Cmd_PickupItem(pItem->GetID(), pItem->GetHandle());
}

//
inline std::tuple<int, int> GetItemPos(CItem* pItem)
{
	if(!pItem)
		return std::make_tuple(0, 0);
	const Point &p = pItem->GetPos();
	return std::make_tuple(p.x, p.y);
}

//
inline int IsPosValid(CCharacter* pCha, int x, int y)
{
	bool bCanMove = false;
	if (pCha)
		bCanMove = pCha->GetSubMap()->IsMoveAble(pCha, x, y);
	return bCanMove ? 1 : 0;
}


//
#define PI 3.1415926
inline std::tuple<int, int> GetChaFacePos(CCharacter* pCha)
{
	if (pCha)
    {
		int x = pCha->GetShape().centre.x;
		int y = pCha->GetShape().centre.y;
		short sAngle = pCha->GetAngle();
		float fAngle = (float) sAngle / 53.3f;
		int xOff = (int)(600.0 * cos(PI / 2 - fAngle));
		int yOff = (int)(600.0 * sin( PI / 2 - fAngle));
		return std::make_tuple(x + xOff, y - yOff);
    }
	return std::make_tuple(0, 0);
}

//
inline void SetChaFaceAngle(CCharacter* pCha, int angle)
{
	if (pCha)
		pCha->SetAngle((short)angle);
}

//
inline void SetChaPatrolPos(CCharacter* pCha, int x, int y)
{
	if (pCha)
    {
		pCha->m_nPatrolX = x;
		pCha->m_nPatrolY = y;
	}
}


//
inline void SetChaEmotion(CCharacter* pCha, int emotion)
{
	if (pCha)
		g_EventHandler.Event_ChaEmotion(pCha, emotion);
}

inline void SetChaLifeTime(CCharacter* pCha, int time)
{
	if (pCha)
		pCha->ResetLifeTime(time);
}

//
inline void HarmLog(int log)
{
	if(log)
		g_bLogHarmRec = TRUE;
	g_bLogHarmRec = FALSE;
}

//
extern const char* GetResPath(const char*);
inline std::string GetResPath_typed(const std::string& path)
{
	return std::string(GetResPath(path.c_str()));
}


extern lua_State *g_pLuaState;
// FrameMove
inline void lua_FrameMove()
{
 	luaL_dostring(g_pLuaState, "RunTimer()");
}

//
inline void view(int x, int y)
{
	extern long g_lViewAtMapX;
	extern long g_lViewAtMapY;
	g_lViewAtMapX = (long)x;
	g_lViewAtMapY = (long)y;
}

inline void lua_AIRun(CCharacter *pCha, DWORD dwResumeExecTime)
{

	static int g_test[20];
	lua_getglobal(g_pLuaState, "ai_timer");
	if (!lua_isfunction(g_pLuaState, -1))
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCha));
	lua_pushnumber(g_pLuaState, (DWORD) defCHA_SCRIPT_TIMER / 1000);
	lua_pushnumber(g_pLuaState, (DWORD) dwResumeExecTime);
	int r = lua_pcall(g_pLuaState, 3, 0, 0);
	if(r!=0)
	{
		lua_callalert(g_pLuaState, r);
	}
	lua_settop(g_pLuaState, 0);
}

inline void lua_NPCRun(CCharacter *pCha)
{

	static int g_test[20];
	lua_getglobal(g_pLuaState, "npc_timer");
	if (!lua_isfunction(g_pLuaState, -1))
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCha));
	int r = lua_pcall(g_pLuaState, 1, 0, 0);
	if(r!=0)
	{
		lua_callalert(g_pLuaState, r);
	}
	lua_settop(g_pLuaState, 0);
}

inline int GetTickCount_typed()
{
    return (int)GetTickCount();
}

inline void Msg(const std::string& content)
{
    MessageBox(NULL, content.c_str(), "msg", 0);
}


inline int Exit_typed()
{
	extern BOOL g_bGameEnd;
	g_bGameEnd = TRUE;
	return 1;
}

// Varargs: variable number of params with dynamic type checking
inline int PRINT_raw( lua_State* L )
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

	std::print("{}", str.str() );
	str.freeze(false);
    return 0;
}

// Varargs: variable number of params with dynamic type checking
inline int LG_raw(lua_State *L)
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
	str << "\n";
	str << std::ends;
	ToLogService("lua", "{}", str.str());
	str.freeze(false);
	return 0;
}

// Varargs: format string + dynamic params
inline int EXLG_raw(lua_State *L)
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
				str << "[noneffective signal(" << *(pszPos + 1) << ")]";

			}
			break;
		}
		sPos1 += sNum + 2;
	}

	str << "\r\n";
	str << '\0';

	ToLogService("lua", "{}", str.str());
	str.freeze(false);
	return 0;
}



inline int GetRoleID(CCharacter* pCha)
{
	if(!pCha) return 0;
	CPlayer* pPlayer = pCha->GetPlayer();
	if(!pPlayer) return 0;
    return (int)pPlayer->GetID();
}

inline void UnlockItem(const std::string& chaName, int iItemDBID)
{
	CPlayer*	pPlayer	=	g_pGameApp->GetPlayerByMainChaName(chaName.c_str());
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
}

inline void SetMonsterAttr(CCharacter* pCha, int AttrType, int AttrVal)
{
	if (!pCha) return;
	int bRet = pCha->setAttr(AttrType, AttrVal);
	if (bRet) {
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
