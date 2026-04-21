#include "stdafx.h"
#include "lua_gamectrl.h"

#include <strstream>

#include "Birthplace.h"
#include "EventHandler.h"
#include "Expand.h"
#include "HarmRec.h"

using namespace std;

std::list<CCharacter*> g_HelpNPCList;

//
void AddBirthPoint(const std::string& location, const std::string& mapName, int x, int y) {
	g_BirthMgr.AddBirthPoint(location.c_str(), mapName.c_str(), x, y);
}

//
void ClearAllBirthPoint() {
	g_BirthMgr.ClearAll();
}

extern const char* GetResPath(const char* pszRes);

void ReloadAISdk() {
	luaL_dofile(g_pLuaState, GetResPath("script/ai/ai.lua"));
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

void TL(int nType, const char* pszCha1, const char* pszCha2, const char* pszTrade) {
	if (!g_Config.m_bLogDB) {
		static short sInit = 1;
		static std::string strName;
		static DWORD dwLastTime = -1;
		static DWORD dwCount = 10000;
		if (dwCount++ > 1000) {
			DWORD dwTime = GetTickCount();
			if (dwTime - dwLastTime >= TL_TIME_ONE_HOUR || sInit == 1) {
				dwCount = 0;
				strName = "trade";
				dwLastTime = dwTime;
				SYSTEMTIME st;
				GetLocalTime(&st);
				char szData[128];
				sprintf(szData, "%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour);
				strName += szData;
			}
		}

		ToLogService("trade", "{:>7} [{:>17}] [{:>17}] [{}]", g_TradeName[nType], pszCha1, pszCha2, pszTrade);
		g_pGameApp->Log(g_TradeName[nType], pszCha1, "", pszCha2, "", pszTrade);
		sInit = 0;
	}
	else {
		// Add by lark.li 20080324 begin
		// End
	}
}

map<string, string> g_HelpList;

// , 2:
void AddHelpInfo(const std::string& key, const std::string& text) {
	g_HelpList[key] = text;
}

const char* FindHelpInfo(const char* pszKey) {
	map<string, string>::iterator it = g_HelpList.find(pszKey);
	if (it == g_HelpList.end()) {
		return NULL;
	}
	return (*it).second.c_str();
}

void AddHelpInfo(const char* pszKey, const char* pszInfo) {
	if (strlen(pszKey) == 0) return;
	if (strlen(pszInfo) == 0) return;

	g_HelpList[pszKey] = pszInfo;

	ToLogService("common", "now helplist amount = {}", g_HelpList.size());
}

void AddMonsterHelp(int nScriptID, int x, int y) {
	CChaRecord* pCChaRecord = GetChaRecordInfo(nScriptID);
	if (pCChaRecord == NULL) return;

	char szHelp[255];
	sprintf(szHelp, RES_STRING(GM_LUA_GAMECTRL_CPP_00019), x / 100, y / 100);

	AddHelpInfo(pCChaRecord->DataName.c_str(), szHelp);
}

void AddHelpNPC(CCharacter* pNPC) {
	ToLogService("common", "Succeed add HelpNPC[{}]", pNPC->GetName());
	g_HelpNPCList.push_back(pNPC);
}


// NPC
void AddHelpNPC_typed(const std::string& name) {
	GamePool::Instance().ForEachTalkNpc([&name](mission::CTalkNpc* pCTNpc) {
		if (!strcmp(pCTNpc->GetName(), name.c_str())) {
			AddHelpNPC(pCTNpc);
		}
	});
}

void ClearHelpNPC() {
	g_HelpNPCList.clear();
}

// DBLog
void TestDBLog(int nCnt) {
	MPTimer t;
	t.Begin();
	for (int i = 0; i < nCnt; i++) {
		g_pGameApp->Log("newtest", "abcdefg", "1234567", "000000", "qqqppp", "abcdefghijklmnopqrstuvwxyz");
	}
	ToLogService("common", "Add Time = {}", t.End());
}

CMapRes* GetMapDataByName(const std::string& mpName) {
	CMapRes* pMap = g_pGameApp->FindMapByName(mpName.c_str());
	if (pMap)
		return pMap;
	return nullptr;
}

//------------------------------------------------------------------------------------------------------------
// Функции, перенесённые из lua_gamectrl.h
//------------------------------------------------------------------------------------------------------------

// lua pcall
void lua_callalert(lua_State* L, int status) {
	if (status != 0) {
		lua_getglobal(L, "_ALERT");
		if (lua_isfunction(L, -1)) {
			lua_insert(L, -2);
			lua_call(L, 1, 0);
		}
		else {
			// no _ALERT function; print it on stderr
			ToLogService("lua", LogLevel::Error, "{}", lua_tostring(L, -2));
			lua_pop(L, 2);
		}
	}
}

void EnableAI(int flag) {
	extern BOOL g_bEnableAI;
	g_bEnableAI = (BOOL)flag;
}

//
int SetCurMap(const std::string& name) {
	CMapRes* pMap = g_pGameApp->FindMapByName(name.c_str());
	if (pMap == NULL) {
		ToLogService("lua", "can't find pointer map[{}], keep former map!", name.c_str());
		return 0;
	}
	g_pScriptMap = pMap->GetCopy();
	return 1;
}

int GetChaID(CCharacter* pCha) {
	if (!pCha)
		return 0;
	return (int)pCha->m_CChaAttr.m_lID;
}


CCharacter* CreateChaNearPlayer(CCharacter* pCha, int nScriptID) {
	if (!g_pScriptMap) return nullptr;
	if (!pCha) return nullptr;
	Point Pos;
	Pos.x = (int)pCha->GetPos().x;
	Pos.y = (int)pCha->GetPos().y;

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter* pCCha = pCha->GetSubMap()->ChaSpawn(nScriptID, enumCHACTRL_NONE, 0, &Pos);
	if (pCCha) {
		return pCCha;
	}
	else {
		ToLogService("lua", "create character near role failed");
		return nullptr;
	}
}


//
CCharacter* CreateCha(int nScriptID, int x, int y, int sAngle, int lReliveTime) {
	if (!g_pScriptMap) return nullptr;

	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbear{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle,
				 lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter* pCCha = g_pScriptMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha) {
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else {
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

CCharacter* CreateChaX(int nScriptID, int x, int y, int sAngle, int lReliveTime, CCharacter* pMainCha) {
	if (!pMainCha) return nullptr;
	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbearX{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle,
				 lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter* pCCha = pMainCha->m_submap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha) {
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else {
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

CCharacter* CreateChaEx(int nScriptID, int x, int y, int sAngle, int lReliveTime, SubMap* pMap) {
	if (!g_pScriptMap) return nullptr;
	if (!pMap) return nullptr;

	Point Pos;
	Pos.x = x;
	Pos.y = y;

	ToLogService("common", "create bugbearEx{}  pos = {} {}, angle = {}, rTime = {}", nScriptID, Pos.x, Pos.y, sAngle,
				 lReliveTime);

	AddMonsterHelp(nScriptID, Pos.x, Pos.y);

	CCharacter* pCCha = pMap->ChaSpawn(nScriptID, enumCHACTRL_NONE, (short)sAngle, &Pos);
	if (pCCha) {
		pCCha->SetResumeTime(lReliveTime * 1000);
		return pCCha;
	}
	else {
		ToLogService("lua", "create character failed");
		return nullptr;
	}
}

//
void ChaMove(CCharacter* pCCha, int x, int y) {
	if (pCCha) {
		Point Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short sPing = 0;
		char chPointNum = 2;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
	}
}

//
void ChaMoveToSleep(CCharacter* pCCha, int x, int y) {
	if (pCCha) {
		Point Path[2] = {pCCha->GetPos(), {x, y}};
		pCCha->m_CActCache.AddCommand(enumCACHEACTION_MOVE);
		short sPing = 0;
		char chPointNum = 2;
		char chStopState = enumEXISTS_SLEEPING;
		pCCha->m_CActCache.PushParam(&sPing, sizeof(short));
		pCCha->m_CActCache.PushParam(&chPointNum, sizeof(char));
		pCCha->m_CActCache.PushParam(Path, sizeof(Point) * 2);
		pCCha->m_CActCache.PushParam(&chStopState, sizeof(char));
	}
}

//
int GetChaSpawnPos_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	lua_pushinteger(L, pCha->GetTerritory().centre.x);
	lua_pushinteger(L, pCha->GetTerritory().centre.y);
	return 2;
}

int GetChaPatrolPos_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	lua_pushinteger(L, pCha->m_nPatrolX);
	lua_pushinteger(L, pCha->m_nPatrolY);
	return 2;
}

// , AI
void SetChaPatrolState(CCharacter* pCha, int state) {
	if (pCha)
		pCha->m_btPatrolState = (BYTE)state;
}

// , AI
int GetChaPatrolState(CCharacter* pCha) {
	if (!pCha) return 0;
	return (int)(BYTE)(pCha->m_btPatrolState);
}


//
// []
//
// Varargs: optional 4th param for immediate execution
int ChaUseSkill_raw(lua_State* L) {
	if (lua_isnil(L, 1) || lua_isnil(L, 2)) {
		return 0;
	}
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParamPtr(L, 2, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}
	int nParamNum = lua_gettop(L);
	if (nParamNum != 3 && !(nParamNum == 4 && lua_isnumber(L, 4))) {
		if (!LuaCheckParamCountRange(L, 3, 4, __FUNCTION__)) {
			return 0;
		}
		LuaCheckParam(L, 4, LUA_TNUMBER, __FUNCTION__);
		return 0;
	}
	bool bExecNow = false;
	if (nParamNum == 4 && ((int)lua_tonumber(L, 4) != 0))
		bExecNow = true;

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		return 0;
	}
	CCharacter* pCha = *pChaResult;

	auto pTargetResult = luabridge::Stack<CCharacter*>::get(L, 2);
	if (!pTargetResult) {
		return 0;
	}
	CCharacter* pTarget = *pTargetResult;

	if (pCha && pTarget) {
		long lSkillID = (int)lua_tonumber(L, 3);
		if (bExecNow) {
			pCha->Cmd_BeginSkillDirect(lSkillID, pTarget);
		}
		else {
			pCha->m_CActCache.AddCommand(enumCACHEACTION_SKILL);
			pCha->m_CActCache.PushParam(&lSkillID, sizeof(long));
			pCha->m_CActCache.PushParam(&pTarget, sizeof(CCharacter*));
		}
	}

	return 0;
}

//
// [x,y][]
//
// Varargs: optional 6th param for immediate execution
int ChaUseSkill2_raw(lua_State* L) {
	if (lua_isnil(L, 1)) {
		return 0;
	}
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 4, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 5, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}
	int nParamNum = lua_gettop(L);
	if (nParamNum != 5 && !(nParamNum == 6 && lua_isnumber(L, 6))) {
		if (!LuaCheckParamCountRange(L, 5, 6, __FUNCTION__)) {
			return 0;
		}
		LuaCheckParam(L, 6, LUA_TNUMBER, __FUNCTION__);
		return 0;
	}
	bool bExecNow = false;
	if (nParamNum == 6 && ((int)lua_tonumber(L, 6) != 0))
		bExecNow = true;

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pCha = *pChaResult;

	if (pCha) {
		long lSkillID = (int)lua_tonumber(L, 2);
		long lSkillLv = (int)lua_tonumber(L, 3);
		long lPosX = (int)lua_tonumber(L, 4);
		long lPosY = (int)lua_tonumber(L, 5);
		if (bExecNow) {
			pCha->Cmd_BeginSkillDirect2(lSkillID, lSkillLv, lPosX, lPosY);
		}
		else {
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
int QueryChaAttr(CCharacter* pCha, int nAttr) {
	if (pCha)
		return (int)(LONG64)pCha->getAttr(nAttr);
	return 0;
}

// ID
int GetChaType(CCharacter* pCha) {
	if (pCha)
		return pCha->m_pCChaRecord->Id;
	return 0;
}

// , AI
int GetChaBlockCnt(CCharacter* pCha) {
	if (pCha)
		return pCha->GetBlockCnt();
	return 0;
}

// , AI
void SetChaBlockCnt(CCharacter* pCha, int cnt) {
	if (pCha)
		pCha->SetBlockCnt((BYTE)cnt);
}

// AI
int GetChaAIType(CCharacter* pCha) {
	if (pCha)
		return pCha->m_AIType;
	return 0;
}

//
int GetChaChaseRange(CCharacter* pCha) {
	if (pCha)
		return pCha->m_sChaseRange;
	return 0;
}

//
void SetChaChaseRange(CCharacter* pCha, int range) {
	if (pCha)
		pCha->m_sChaseRange = (short)range;
}


// AI
void SetChaAIType(CCharacter* pCha, int nType) {
	if (pCha) {
		pCha->m_AIType = nType;
		ToLogService("lua", "character[{}] be set ai type is {}", pCha->GetName(), nType);
	}
}

//
//
int GetChaTypeID(CCharacter* pCha) {
	if (pCha && pCha->m_pCChaRecord)
		return (int)pCha->m_pCChaRecord->lID;
	return 0;
}

// ()
int GetChaVision(CCharacter* pCha) {
	if (pCha)
		return (int)pCha->m_pCChaRecord->lVision;
	return 0;
}

// , AI
int GetChaSkillNum(CCharacter* pCha) {
	if (pCha)
		return pCha->m_CSkillBag.GetSkillNum();
	return 0;
}

// ID
int GetChaSkillInfo_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	if (!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto nLocResult = luabridge::Stack<int>::get(L, 2);
	if (!nLocResult) {
		LuaLogCastFailed(L, 2, "int", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	int nLoc = *nLocResult;
	lua_pushinteger(L, pCha->m_pCChaRecord->lSkill[nLoc][0]);
	lua_pushinteger(L, pCha->m_pCChaRecord->lSkill[nLoc][1]);
	return 2;
}


//
// Dynamic type checking on arg 2 (number = clear target, lightuserdata = set target)
int SetChaTarget_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 2, __FUNCTION__)) {
		return 0;
	}
	if (lua_isnil(L, 1)) {
		return 0;
	}
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		return 0;
	}

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pCha = *pChaResult;

	CCharacter* pTarget = nullptr;
	if (lua_isnumber(L, 2) || lua_isnil(L, 2)) {
		pTarget = nullptr;
	}
	else if (LuaIsPtr(L, 2)) {
		auto pTargetResult = luabridge::Stack<CCharacter*>::get(L, 2);
		if (!pTargetResult) {
			LuaLogCastFailed(L, 2, "CCharacter*", __FUNCTION__);
			return 0;
		}
		pTarget = *pTargetResult;
	}
	else {
		LuaCheckParamPtr(L, 2, __FUNCTION__);
		return 0;
	}
	if (pCha) {
		if (pTarget) {
			pCha->m_AITarget = pTarget;
			pCha->m_SFightInit.chTarType = 1;
			pCha->m_SFightInit.lTarInfo1 = pTarget->GetID();
			pCha->m_SFightInit.lTarInfo2 = pTarget->GetHandle();
			pCha->m_SMoveInit.STargetInfo.chType = 1;
			pCha->m_SMoveInit.STargetInfo.lInfo1 = pTarget->GetID();
			pCha->m_SMoveInit.STargetInfo.lInfo2 = pTarget->GetHandle();
		}
		else {
			pCha->m_AITarget = nullptr;
			pCha->m_SFightInit.chTarType = 0;
			pCha->m_SMoveInit.STargetInfo.chType = 0;
		}
	}

	return 0;
}

//
CCharacter* GetChaTarget(CCharacter* pCha) {
	if (pCha && pCha->m_AITarget)
		return pCha->m_AITarget;
	return nullptr;
}

//
CCharacter* GetChaHost(CCharacter* pCha) {
	if (pCha && pCha->m_HostCha)
		return pCha->m_HostCha;
	return nullptr;
}

//
// Dynamic type checking on arg 2 (number = clear host, lightuserdata = set host)
int SetChaHost_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 2, __FUNCTION__)) {
		return 0;
	}
	if (lua_isnil(L, 1)) {
		return 0;
	}
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		return 0;
	}

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pCha = *pChaResult;

	CCharacter* pHost = nullptr;
	if (lua_isnumber(L, 2) || lua_isnil(L, 2)) {
		pHost = nullptr;
	}
	else if (LuaIsPtr(L, 2)) {
		auto pHostResult = luabridge::Stack<CCharacter*>::get(L, 2);
		if (!pHostResult) {
			LuaLogCastFailed(L, 2, "CCharacter*", __FUNCTION__);
			return 0;
		}
		pHost = *pHostResult;
	}
	else {
		LuaCheckParamPtr(L, 2, __FUNCTION__);
		return 0;
	}
	if (pCha) {
		pCha->m_HostCha = pHost;
		if (pHost && pHost->IsPlayerCha()) {
			int nPetNum = pHost->GetPlyMainCha()->GetPetNum();
			pHost->GetPlyMainCha()->SetPetNum(nPetNum + 1);
		}
	}
	return 0;
}

int GetPetNum(CCharacter* pCha) {
	if (pCha && pCha->GetPlyMainCha())
		return pCha->GetPlyMainCha()->GetPetNum();
	return 0;
}

//
CCharacter* GetChaFirstTarget(CCharacter* pCha) {
	if (pCha) {
		CCharacter* pTarget = pCha->m_pHate->GetCurTarget();
		if (pTarget)
			return pTarget;
	}
	return nullptr;
}

//
CCharacter* GetFirstAtker(CCharacter* pCha) {
	if (pCha) {
		CCharacter* pFirst = nullptr;
		DWORD dwMinTime = 0xFFFFFFFF;
		for (int i = 0; i < MAX_HARM_REC; i++) {
			SHarmRec* pHarm = pCha->m_pHate->GetHarmRec(i);
			if (pHarm->btValid) {
				if (pHarm->IsChaValid()) {
					if (pHarm->dwTime < dwMinTime) {
						dwMinTime = pHarm->dwTime;
						pFirst = pHarm->pAtk;
					}
				}
			}
		}
		if (pFirst)
			return pFirst;
	}
	return nullptr;
}

// ,
// Dynamic return: first value is either lightuserdata or number 0
int GetChaHarmByNo_raw(lua_State* L) {
	if (lua_isnil(L, 1)) {
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}
	if (!LuaCheckParamCount(L, 2, __FUNCTION__) ||
		!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__)) {
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	int nNo = (int)(lua_tonumber(L, 2));
	SHarmRec* pHarm = pCha->m_pHate->GetHarmRec(nNo);
	if (pHarm->btValid > 0) {
		if (pHarm->IsChaValid()) {
			luabridge::push(L, static_cast<CCharacter*>(pHarm->pAtk));
		}
		else {
			lua_pushnumber(L, 0);
		}
		lua_pushnumber(L, pHarm->sHarm);
		return 2;
	}
	lua_pushnumber(L, 0);
	lua_pushnumber(L, 0);
	return 2;
}

//
// Dynamic return: first value is either lightuserdata or number 0
int GetChaHateByNo_raw(lua_State* L) {
	if (lua_isnil(L, 1)) {
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}
	if (!LuaCheckParamCount(L, 2, __FUNCTION__) ||
		!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__)) {
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}

	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	int nNo = (int)(lua_tonumber(L, 2));
	SHarmRec* pHarm = pCha->m_pHate->GetHarmRec(nNo);
	if (pHarm->btValid > 0) {
		if (pHarm->IsChaValid()) {
			luabridge::push(L, static_cast<CCharacter*>(pHarm->pAtk));
		}
		else {
			lua_pushnumber(L, 0);
		}
		lua_pushnumber(L, pHarm->sHate);
		return 2;
	}
	lua_pushnumber(L, 0);
	lua_pushnumber(L, 0);
	return 2;
}

//
void AddHate(CCharacter* pTarget, CCharacter* pAtk, int sHate) {
	if (pTarget && pAtk)
		pTarget->m_pHate->AddHate(pAtk, (short)sHate, pAtk->GetID());
}


int GetChaPos_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}

	CCharacter* pCha = *pChaResult;
	lua_pushinteger(L, pCha->GetShape().centre.x);
	lua_pushinteger(L, pCha->GetShape().centre.y);
	return 2;
}

//
int IsChaFighting(CCharacter* pCha) {
	if (pCha && pCha->GetFightState() == enumFSTATE_ON)
		return 1;
	return 0;
}

//
int IsChaSleeping(CCharacter* pCha) {
	if (pCha && pCha->GetExistState() == enumEXISTS_SLEEPING)
		return 1;
	return 0;
}


//
// 10
void ChaActEyeshot(CCharacter* pCha, int bActive) {
	if (pCha)
		pCha->ActiveEyeshot((bool)bActive);
}

//
// First arg can be nil (pSelf=nullptr), uses global map fallback + complex search logic
int GetChaByRange_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 5, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 4, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 5, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}

	auto pSelfResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pSelfResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pSelf = *pSelfResult;

	SubMap* pMap = nullptr;

	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (pSelf) {
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap();
	}
	else {
		CHECK_MAP
		pMap = g_pScriptMap;
	}

	int r = (int)lua_tonumber(L, 4);
	int flag = (int)lua_tonumber(L, 5);


	CCharacter* pCTarget = nullptr;

	unsigned long ulMinDist2 = r * r, ulTempDist2;
	long lDistX, lDistY;
	CCharacter* pCTempCha;
	Long lRangeB[] = {x, y, 0};
	Long lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE);
	while (pCTempCha = pMap->GetNextCharacterInRange()) {
		if (pCTempCha == pSelf) continue;

		if (flag == 0) {
			if (!pCTempCha->IsPlayerCha()) continue;
			if (pCTempCha->IsGMCha()) continue;
			if (!pCTempCha->IsLiveing()) continue;
			if (!pCTempCha->GetActControl(enumACTCONTROL_BEUSE_SKILL)) continue;
		}

		if (flag == 1 && pCTempCha->IsPlayerCha()) continue;

		if (pSelf && pCTempCha->IsFriend(pSelf)) {
			continue;
		}

		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2) {
			pCTarget = pCTempCha;
			ulMinDist2 = ulTempDist2;
		}
	}
	if (pCTarget) {
		luabridge::push(L, static_cast<CCharacter*>(pCTarget));
		return 1;
	}

	return 0;
}

//
// First arg can be nil (pSelf=nullptr), uses global map fallback
int ClearHideChaByRange_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 5, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 4, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 5, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}

	auto pSelfResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pSelfResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pSelf = *pSelfResult;

	SubMap* pMap = nullptr;

	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (pSelf) {
		x = pSelf->GetShape().centre.x;
		y = pSelf->GetShape().centre.y;
		pMap = pSelf->GetSubMap();
	}
	else {
		CHECK_MAP
		pMap = g_pScriptMap;
	}

	int r = (int)lua_tonumber(L, 4);
	int flag = (int)lua_tonumber(L, 5);


	CCharacter* pCTarget = nullptr;

	unsigned long ulMinDist2 = r * r, ulTempDist2;
	long lDistX, lDistY;
	CCharacter* pCTempCha;
	Long lRangeB[] = {x, y, 0};
	Long lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE, true);
	while (pCTempCha = pMap->GetNextCharacterInRange()) {
		if (pCTempCha == pSelf) continue;

		if (flag == 0) {
			if (!pCTempCha->IsPlayerCha()) continue;
			if (pCTempCha->IsGMCha()) continue;
			if (!pCTempCha->IsLiveing()) continue;
		}

		if (flag == 1 && pCTempCha->IsPlayerCha()) continue;

		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2) {
			pCTarget = pCTempCha;
			if (pCTarget->m_CSkillState.HasState(SSTATE_HIDE)) {
				pCTarget->SystemNotice(RES_STRING(GM_LUA_GAMECTRL_H_00002));
				pCTarget->Show();
			}
		}
	}
	return 0;
}


//
// Variable number of returns (up to 12)
int GetChaSetByRange_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 5, __FUNCTION__) ||
		!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 4, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 5, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}

	auto pSelfResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pSelfResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pSelf = *pSelfResult;
	SubMap* pMap = pSelf->GetSubMap();
	int x = pSelf->GetShape().centre.x;
	int y = pSelf->GetShape().centre.y;
	int r = (int)lua_tonumber(L, 4);
	int nMonsterType = (int)lua_tonumber(L, 5);

	CCharacter* pCTarget = nullptr;

	unsigned long ulMinDist2 = r * r, ulTempDist2;
	long lDistX, lDistY;
	CCharacter* pCTempCha = nullptr;
	CCharacter* ChaList[12];
	Long lRangeB[] = {x, y, 0};
	Long lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
	pMap->BeginSearchInRange(lRangeB, lRangeE);
	int n = 0;
	while (pCTempCha = pMap->GetNextCharacterInRange()) {
		if (pCTempCha == pSelf) continue;
		if (pCTempCha->IsPlayerCha()) continue;

		if (nMonsterType != 0 && nMonsterType != pCTempCha->GetCat()) continue;

		lDistX = pCTempCha->GetShape().centre.x - x;
		lDistY = pCTempCha->GetShape().centre.y - y;
		ulTempDist2 = lDistX * lDistX + lDistY * lDistY;
		if (ulTempDist2 <= ulMinDist2) {
			ChaList[n] = pCTempCha;
			n++;
			if (n >= 12) break;
		}
	}

	for (int i = 0; i < n; i++) {
		luabridge::push(L, static_cast<CCharacter*>(ChaList[i]));
	}
	return n;
}


//
int FindItem(int x, int y, int r) {
	// Commented out in original - always returns 0
	return 0;
}

//
void PickItem(CCharacter* pCha, CItem* pItem) {
	if (pCha && pItem)
		pCha->Cmd_PickupItem(pItem->GetID(), pItem->GetHandle());
}

//
int GetItemPos_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pItemResult = luabridge::Stack<CItem*>::get(L, 1);
	if (!pItemResult) {
		LuaLogCastFailed(L, 1, "CItem*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	CItem* pItem = *pItemResult;
	const Point& p = pItem->GetPos();
	lua_pushinteger(L, p.x);
	lua_pushinteger(L, p.y);
	return 2;
}

//
int IsPosValid(CCharacter* pCha, int x, int y) {
	bool bCanMove = false;
	if (pCha)
		bCanMove = pCha->GetSubMap()->IsMoveAble(pCha, x, y);
	return bCanMove ? 1 : 0;
}


int GetChaFacePos_raw(lua_State* L) {
	if (!LuaCheckParamPtr(L, 1, __FUNCTION__)) {
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	auto pChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		return 2;
	}
	CCharacter* pCha = *pChaResult;
	int x = pCha->GetShape().centre.x;
	int y = pCha->GetShape().centre.y;
	short sAngle = pCha->GetAngle();
	float fAngle = (float)sAngle / 53.3f;
	int xOff = (int)(600.0 * cos(PI / 2 - fAngle));
	int yOff = (int)(600.0 * sin(PI / 2 - fAngle));
	lua_pushinteger(L, x + xOff);
	lua_pushinteger(L, y - yOff);
	return 2;
}

//
void SetChaFaceAngle(CCharacter* pCha, int angle) {
	if (pCha)
		pCha->SetAngle((short)angle);
}

//
void SetChaPatrolPos(CCharacter* pCha, int x, int y) {
	if (pCha) {
		pCha->m_nPatrolX = x;
		pCha->m_nPatrolY = y;
	}
}


//
void SetChaEmotion(CCharacter* pCha, int emotion) {
	if (pCha)
		g_EventHandler.Event_ChaEmotion(pCha, emotion);
}

void SetChaLifeTime(CCharacter* pCha, int time) {
	if (pCha)
		pCha->ResetLifeTime(time);
}

//
void HarmLog(int log) {
	if (log)
		g_bLogHarmRec = TRUE;
	g_bLogHarmRec = FALSE;
}

std::string GetResPath_typed(const std::string& path) {
	return std::string(GetResPath(path.c_str()));
}

// FrameMove
void lua_FrameMove() {
	luaL_dostring(g_pLuaState, "RunTimer()");
}

//
void view(int x, int y) {
	extern long g_lViewAtMapX;
	extern long g_lViewAtMapY;
	g_lViewAtMapX = (long)x;
	g_lViewAtMapY = (long)y;
}

void lua_AIRun(CCharacter* pCha, DWORD dwResumeExecTime) {
	static int g_test[20];
	lua_getglobal(g_pLuaState, "ai_timer");
	if (!lua_isfunction(g_pLuaState, -1)) {
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCha));
	lua_pushnumber(g_pLuaState, (DWORD)defCHA_SCRIPT_TIMER / 1000);
	lua_pushnumber(g_pLuaState, (DWORD)dwResumeExecTime);
	int r = lua_pcall(g_pLuaState, 3, 0, 0);
	if (r != 0) {
		lua_callalert(g_pLuaState, r);
	}
	lua_settop(g_pLuaState, 0);
}

void lua_NPCRun(CCharacter* pCha) {
	static int g_test[20];
	lua_getglobal(g_pLuaState, "npc_timer");
	if (!lua_isfunction(g_pLuaState, -1)) {
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCha));
	int r = lua_pcall(g_pLuaState, 1, 0, 0);
	if (r != 0) {
		lua_callalert(g_pLuaState, r);
	}
	lua_settop(g_pLuaState, 0);
}

int GetTickCount_typed() {
	return (int)GetTickCount();
}

void Msg(const std::string& content) {
	MessageBox(nullptr, content.c_str(), "msg", 0);
}


int Exit_typed() {
	extern BOOL g_bGameEnd;
	g_bGameEnd = TRUE;
	return 1;
}

// Varargs: variable number of params with dynamic type checking
int PRINT_raw(lua_State* L) {
	if (g_Config.m_bLogMission == FALSE) {
		return 0;
	}

	int count = lua_gettop(L);
	if (count < 1) {
		return 0;
	}

	std::ostrstream str;
	for (int i = 1; i <= count; i++) {
		switch (lua_type(L, i)) {
		case LUA_TNIL: {
			str << "nil";
		}
		break;
		case LUA_TBOOLEAN: {
			(lua_toboolean(L, i) == 0) ? str << "FALSE" : str << "TRUE";
		}
		break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA: {
			str << "userdata:";
			const void* p = lua_touserdata(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TNUMBER:
		case LUA_TSTRING: {
			const char* pszData = lua_tostring(L, i);
			str << (pszData) ? pszData : "nil";
		}
		break;
		case LUA_TTABLE: {
			str << "table:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TFUNCTION: {
			str << "function:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TTHREAD: {
			str << "thread:";
			str << lua_tothread(L, i);
		}
		break;
		}
		str << "  ";
	}

	str << "\r\n";
	str << '\0';

	std::print("{}", str.str());
	str.freeze(false);
	return 0;
}

// Varargs: variable number of params with dynamic type checking
int LG_raw(lua_State* L) {
	int count = lua_gettop(L);
	if (count <= 1) {
		ToLogService("lua", LogLevel::Error, "[{}] expected at least 2 params, got {}", __FUNCTION__, count);
		return 0;
	}
	const char* pszFile = lua_tostring(L, 1);
	if (g_Config.m_bLogAI == FALSE) {
		if (strcmp(pszFile, "lua_ai") == 0) {
			return 0;
		}
	}
	if (strcmp(pszFile, "exp") == 0) {
		return 0;
	}
	if (g_Config.m_bLogMission == FALSE) {
		if (strcmp(pszFile, "mission") == 0 || strcmp(pszFile, "mission_error") == 0 ||
			strcmp(pszFile, "trigger") == 0 || strcmp(pszFile, "trigger_error") == 0 ||
			strcmp(pszFile, "randmission_init") == 0 || strcmp(pszFile, "randmission_init2") == 0 ||
			strcmp(pszFile, "randmission_error") == 0) {
			return 0;
		}
	}
	char szBuf[1024 * 2] = {0};
	std::ostrstream str(szBuf, sizeof(szBuf));
	str << lua_tostring(L, 2);
	str << " ";
	for (int i = 3; i <= count; i++) {
		switch (lua_type(L, i)) {
		case LUA_TNIL: {
			str << "nil";
		}
		break;
		case LUA_TBOOLEAN: {
			(lua_toboolean(L, i) == 0) ? str << "FALSE" : str << "TRUE";
		}
		break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA: {
			str << "userdata:";
			const void* p = lua_touserdata(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TNUMBER:
		case LUA_TSTRING: {
			const char* pszData = lua_tostring(L, i);
			str << (pszData) ? pszData : "nil";
		}
		break;
		case LUA_TTABLE: {
			str << "table:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TFUNCTION: {
			str << "function:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case LUA_TTHREAD: {
			str << "thread:";
			str << lua_tothread(L, i);
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
int EXLG_raw(lua_State* L) {
	int nNumParam = lua_gettop(L);
	if (nNumParam <= 1) {
		ToLogService("lua", LogLevel::Error, "[{}] expected at least 2 params, got {}", __FUNCTION__, nNumParam);
		return 0;
	}

	const char* pszFile = lua_tostring(L, 1);
	const char* pszTemp = lua_tostring(L, 2);
	if (!pszFile || !pszTemp) {
		if (!pszFile) { LuaCheckParam(L, 1, LUA_TSTRING, __FUNCTION__); }
		if (!pszTemp) { LuaCheckParam(L, 2, LUA_TSTRING, __FUNCTION__); }
		return 0;
	}
	char szData[1024] = {0};

	std::ostrstream str;
	USHORT sPos1 = 0, sNum = 0;
	for (int i = 3; i <= nNumParam; i++) {
		const char* pszPos = strstr(pszTemp + sPos1, "%");
		if (pszPos == nullptr) {
			str << pszTemp + sPos1;
			break;
		}

		sNum = USHORT(pszPos - (pszTemp + sPos1));
		strncpy_s(szData, (sNum > 1020) ? 1020 : sNum, pszTemp + sPos1,_TRUNCATE);
		szData[(sNum > 1020) ? 1020 : sNum] = 0;
		if (sNum > 1020)
			strncat_s(szData, sizeof(szData), "...", _TRUNCATE);

		str << szData;
		switch (*(pszPos + 1)) {
		case 'd':
		case 's': {
			const char* pszData = lua_tostring(L, i);
			(pszData) ? str << pszData : str << "nil";
		}
		break;
		case 'b': {
			(lua_toboolean(L, i) == 0) ? str << "FALSE" : str << "TRUE";
		}
		break;
		case 'u': {
			str << "userdata:";
			const void* p = lua_touserdata(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case 'f': {
			str << "function:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		case 't': {
			str << "table:";
			const void* p = lua_topointer(L, i);
			(p) ? str << p : str << "nil";
		}
		break;
		default: {
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


int GetRoleID(CCharacter* pCha) {
	if (!pCha) return 0;
	CPlayer* pPlayer = pCha->GetPlayer();
	if (!pPlayer) return 0;
	return (int)pPlayer->GetID();
}

void UnlockItem(const std::string& chaName, int iItemDBID) {
	CPlayer* pPlayer = g_pGameApp->GetPlayerByMainChaName(chaName.c_str());
	if (pPlayer) {
		int iCapacity = pPlayer->GetMainCha()->m_CKitbag.GetCapacity();

		for (int i = 0; i < iCapacity; i++) {
			SItemGrid* sig = pPlayer->GetMainCha()->m_CKitbag.GetGridContByID(i);

			if (sig) {
				if (sig->dwDBID == iItemDBID) {
					sig->dwDBID = 0;
					sig->SetChange(true);
					pPlayer->GetMainCha()->SynKitbagNew(enumSYN_KITBAG_ATTR);
					break;
				}
			};
		};
	};
}

void SetMonsterAttr(CCharacter* pCha, int AttrType, int AttrVal) {
	if (!pCha) return;
	int bRet = pCha->setAttr(AttrType, AttrVal);
	if (bRet) {
		pCha->SynAttr(enumATTRSYN_TASK);
	}
}


//------------------------------------------------------------------------------------------------------------
// Функции, перенесённые из lua_gamectrl2.h
//------------------------------------------------------------------------------------------------------------

// , 0
int IsChaInTeam(CCharacter* pCha) {
	if (pCha) {
		CPlayer* pPlayer = pCha->GetPlayer();
		if (pPlayer)
			return pPlayer->GetTeamMemberCnt();
	}
	return 0;
}

// , 0
CCharacter* GetTeamCha(CCharacter* pCha, int nNo) {
	if (!pCha)
		return nullptr;
	CPlayer* pPlayer = pCha->GetPlayer();
	if (pPlayer == NULL) return nullptr;

	if (nNo >= pPlayer->GetTeamMemberCnt())
		return nullptr;

	CPlayer* pMember = g_pGameApp->GetPlayerByDBID(pPlayer->GetTeamMemberDBID(nNo));
	if (!pMember)
		return nullptr;

	if (pMember->GetCtrlCha()->IsLiveing() == false)
		return nullptr;

	return pMember->GetCtrlCha();
}

//
int IsChaInRegion(CCharacter* pCha, int nRegionDef) {
	if (pCha) {
		if (pCha->IsInArea(nRegionDef))
			return 1;
		return 0;
	}
	return 0;
}


//
std::string GetChaDefaultName(CCharacter* pCha) {
	if (pCha)
		return std::string(pCha->GetName());
	return std::string();
}

//
// Delegates to raw GetChaAttr_raw from Expand.h
int GetChaAttrI_raw(lua_State* L) {
	return GetChaAttr_raw(L);
}

//
// Inlined version of SetChaAttr logic (original lua_SetChaAttr was removed)
int SetChaAttrI_raw(lua_State* L) {
	if (!LuaCheckParamCount(L, 3, __FUNCTION__) ||
		!LuaCheckParamPtr(L, 1, __FUNCTION__) ||
		!LuaCheckParam(L, 2, LUA_TNUMBER, __FUNCTION__) ||
		!LuaCheckParam(L, 3, LUA_TNUMBER, __FUNCTION__)) {
		return 0;
	}

	auto pCChaResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pCChaResult) {
		LuaLogCastFailed(L, 1, "CCharacter*", __FUNCTION__);
		return 0;
	}
	CCharacter* pCCha = *pCChaResult;

	short sAttrIndex = static_cast<int64_t>(lua_tonumber(L, 2));
	LONG32 lValue = static_cast<int64_t>(lua_tonumber(L, 3));

	if (!pCCha) return 0;
	if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM) return 0;

	long lSetRet = pCCha->setAttr(sAttrIndex, lValue);
	if (lSetRet == 0) return 0;
	return 1;
}

//
int IsPlayer(CCharacter* pCha) {
	if (!pCha) return 0;
	if (pCha->GetPlayer())
		return 1;
	return 0;
}

// ,
void SetChaAttrMax(int nNo, unsigned int lValue) {
	if (nNo < ATTR_MAX_NUM) {
		g_lMaxChaAttr[nNo] = (LONG32)lValue;
		g_pGameApp->ChaAttrMaxValInit(true);
	}
}

// , ,
void AddWeatherRegion(int btType, int dwFre, int dwLastTime, int sx, int sy, int w, int h) {
	sx = sx / 2;
	sy = sy / 2;

	w = w / 2 + w % 2;
	h = h / 2 + h % 2;

	CWeather* pNew = new CWeather((BYTE)btType);
	pNew->SetFrequence(dwFre + 10 + rand() % 20);
	pNew->SetRange(sx, sy, w, h);
	pNew->SetStateLastTime(dwLastTime);

	g_pScriptMap->m_WeatherMgr.AddWeatherRange(pNew);

	ToLogService("common", "add weather area[{}], occur time limit = {}, duration = {}, location = {} {}, {} {}",
				 btType, dwFre, dwLastTime, sx, sy, w, h);
}


//
void ClearMapWeather() {
	if (!g_pScriptMap) return;

	g_pScriptMap->m_WeatherMgr.ClearAll();
	ToLogService("common", "weed out map[{}] upon all weather area!", g_pScriptMap->GetName());
}

//
void SetBoatCtrlTick(CCharacter* pCha, int tick) {
	if (pCha)
		pCha->m_dwBoatCtrlTick = tick;
}

//
int GetBoatCtrlTick(CCharacter* pCha) {
	if (pCha)
		return (int)pCha->m_dwBoatCtrlTick;
	return 0;
}

// ,
//  : , (1   2),
//
CCharacter* SummonCha(CCharacter* pHost, int sType, int sChaInfoID) {
	if (!pHost) return nullptr;

	Point Pos = pHost->GetPos();

	CCharacter* pCha = NULL;

	if (sType == 1) {
		pCha = pHost->GetSubMap()->ChaSpawn((short)sChaInfoID, enumCHACTRL_PLAYER_PET, rand() % 360, &Pos);
		if (pCha) {
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 0;
		}
	}
	else if (sType == 2) {
		Pos.move(rand() % 360, 3 * 100);
		pCha = pHost->GetSubMap()->ChaSpawn((short)sChaInfoID, enumCHACTRL_PLAYER_PET, rand() % 360, &Pos);
		if (pCha) {
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 5;
		}
	}

	if (pCha == NULL) {
		pHost->SystemNotice("call character[%d %d]failed", sType, sChaInfoID);
		return nullptr;
	}

	return pCha;
}

//
//  :
//
void DelCha(CCharacter* pCTarCha) {
	if (!pCTarCha)
		return;
	if (pCTarCha->IsPlayerCtrlCha())
		return;
	pCTarCha->Free();
}


void RegisterLuaAI(lua_State* L) {
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
	lua_register(L, "GetChaPos", GetChaPos_raw);
	lua_register(L, "GetChaSpawnPos", GetChaSpawnPos_raw);
	lua_register(L, "GetChaPatrolPos", GetChaPatrolPos_raw);
	lua_register(L, "GetChaFacePos", GetChaFacePos_raw);
	lua_register(L, "GetChaSkillInfo", GetChaSkillInfo_raw);
	lua_register(L, "GetItemPos", GetItemPos_raw);

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
		// GetChaPos registered as raw above
		.addFunction("GetChaBlockCnt", GetChaBlockCnt)
		.addFunction("SetChaBlockCnt", SetChaBlockCnt)
		.addFunction("ChaMove", ChaMove)
		.addFunction("ChaMoveToSleep", ChaMoveToSleep)
		// GetChaSpawnPos registered as raw above
		.addFunction("SetChaPatrolState", SetChaPatrolState)
		.addFunction("GetChaPatrolState", GetChaPatrolState)
		// GetChaPatrolPos registered as raw above
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
		// GetChaFacePos registered as raw above
		.addFunction("SetChaEmotion", SetChaEmotion)
		.addFunction("FindItem", FindItem)
		.addFunction("PickItem", PickItem)
		// GetItemPos registered as raw above
		.addFunction("EnableAI", EnableAI)
		.addFunction("GetChaSkillNum", GetChaSkillNum)
		// GetChaSkillInfo registered as raw above
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
