#pragma once

#include "Character.h"
#include "script.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "stdafx.h"

#pragma warning(disable: 4800)

//    Lua-
#define PARAM_ERROR        { ToLogService("lua", LogLevel::Error, "lua extend function[{}] param number or type error!", __FUNCTION__); }
#define MAP_NULL_ERROR     { ToLogService("lua", LogLevel::Error, "lua extend function[{}] nonce map is null", __FUNCTION__); }
#define CHECK_MAP          { if(g_pScriptMap==NULL) { MAP_NULL_ERROR return 0; }				    }
#define PARAM_LG_ERROR		 THROW_EXCP( excp, RES_STRING(GM_LUA_GAMECTRL_H_00001) );


//--------------------------NPC-----------------------------
struct SHelpNPC {
	char szName[32];
	CCharacter* pNPC;

	SHelpNPC() {
		strcpy(szName, "");
		pNPC = NULL;
	}
};

extern std::list<CCharacter*> g_HelpNPCList;
const char* FindHelpInfo(const char* pszKey);
void AddHelpInfo(const char* pszKey, const char* pszInfo);
void AddHelpInfo(const std::string& key, const std::string& text);
void AddHelpNPC_typed(const std::string& name);
void AddMonsterHelp(int nScriptID, int x, int y);
void AddHelpNPC(CCharacter* pNPC);
//--------------------------------------------------------------------

#define PI 3.1415926

void lua_callalert(lua_State* L, int status);
void EnableAI(int flag);
int SetCurMap(const std::string& name);
int GetChaID(CCharacter* pCha);
CCharacter* CreateChaNearPlayer(CCharacter* pCha, int nScriptID);
CCharacter* CreateCha(int nScriptID, int x, int y, int sAngle, int lReliveTime);
CCharacter* CreateChaX(int nScriptID, int x, int y, int sAngle, int lReliveTime, CCharacter* pMainCha);
CCharacter* CreateChaEx(int nScriptID, int x, int y, int sAngle, int lReliveTime, SubMap* pMap);
void ChaMove(CCharacter* pCCha, int x, int y);
void ChaMoveToSleep(CCharacter* pCCha, int x, int y);
int GetChaSpawnPos_raw(lua_State* L);
int GetChaPatrolPos_raw(lua_State* L);
void SetChaPatrolState(CCharacter* pCha, int state);
int GetChaPatrolState(CCharacter* pCha);
int ChaUseSkill_raw(lua_State* L);
int ChaUseSkill2_raw(lua_State* L);
int QueryChaAttr(CCharacter* pCha, int nAttr);
int GetChaType(CCharacter* pCha);
int GetChaBlockCnt(CCharacter* pCha);
void SetChaBlockCnt(CCharacter* pCha, int cnt);
int GetChaAIType(CCharacter* pCha);
int GetChaChaseRange(CCharacter* pCha);
void SetChaChaseRange(CCharacter* pCha, int range);
void SetChaAIType(CCharacter* pCha, int nType);
int GetChaTypeID(CCharacter* pCha);
int GetChaVision(CCharacter* pCha);
int GetChaSkillNum(CCharacter* pCha);
int GetChaSkillInfo_raw(lua_State* L);
int SetChaTarget_raw(lua_State* L);
CCharacter* GetChaTarget(CCharacter* pCha);
CCharacter* GetChaHost(CCharacter* pCha);
int SetChaHost_raw(lua_State* L);
int GetPetNum(CCharacter* pCha);
CCharacter* GetChaFirstTarget(CCharacter* pCha);
CCharacter* GetFirstAtker(CCharacter* pCha);
int GetChaHarmByNo_raw(lua_State* L);
int GetChaHateByNo_raw(lua_State* L);
void AddHate(CCharacter* pTarget, CCharacter* pAtk, int sHate);
int GetChaPos_raw(lua_State* L);
int IsChaFighting(CCharacter* pCha);
int IsChaSleeping(CCharacter* pCha);
void ChaActEyeshot(CCharacter* pCha, int bActive);
int GetChaByRange_raw(lua_State* L);
int ClearHideChaByRange_raw(lua_State* L);
int GetChaSetByRange_raw(lua_State* L);
int FindItem(int x, int y, int r);
void PickItem(CCharacter* pCha, CItem* pItem);
int GetItemPos_raw(lua_State* L);
int IsPosValid(CCharacter* pCha, int x, int y);
int GetChaFacePos_raw(lua_State* L);
void SetChaFaceAngle(CCharacter* pCha, int angle);
void SetChaPatrolPos(CCharacter* pCha, int x, int y);
void SetChaEmotion(CCharacter* pCha, int emotion);
void SetChaLifeTime(CCharacter* pCha, int time);
void HarmLog(int log);
std::string GetResPath_typed(const std::string& path);
void lua_FrameMove();
void view(int x, int y);
void lua_AIRun(CCharacter* pCha, DWORD dwResumeExecTime);
void lua_NPCRun(CCharacter* pCha);
int GetTickCount_typed();
void Msg(const std::string& content);
int Exit_typed();
int PRINT_raw(lua_State* L);
int LG_raw(lua_State* L);
int EXLG_raw(lua_State* L);
int GetRoleID(CCharacter* pCha);
void UnlockItem(const std::string& chaName, int iItemDBID);
void SetMonsterAttr(CCharacter* pCha, int AttrType, int AttrVal);

extern const char* GetResPath(const char*);
extern lua_State* g_pLuaState;

void RegisterLuaAI(lua_State* L);
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
void TL(int nType, const char* pszCha1, const char* pszCha2, const char* pszTrade);
