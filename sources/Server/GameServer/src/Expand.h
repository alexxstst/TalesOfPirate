// EXPAND_H//=============================================================================
// FileName: Expand.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: expand
//=============================================================================
//modify by alfred.shi 20080306

#pragma once

#include "stdafx.h"   //add by alfred.shi 20080304

#include "Character.h"
#include "excp.h"
#include "lua.hpp"
#include "GameDB.h"
#include "MapEntry.h"
#include "Script.h"
#include <tuple>


void SynLook(CCharacter* pCha);
int IsAttributeEditable(SItemGrid* item, int attribute);
void SetAttributeEditable(SItemGrid* item, int slot, int attribute);
void EquipItem(CCharacter* pCha, int chEquipPos, SItemGrid* equip);
void EquipStringItem();
int GetChaGuildPermission(CCharacter* pCha);
int GetIMP(CCharacter* pCCha);
int SetIMP_raw(lua_State* pLS);
int GetChaAttr_raw(lua_State* pLS);
int SetChaAttr(CCharacter* pCCha, int sAttrIndex, int64_t lValue);
int CheckChaRole(CCharacter* pCCha);
CPlayer* GetChaPlayer(CCharacter* pCCha);
int GetPlayerTeamID(CPlayer* pCPly);
int GetPlayerID(CPlayer* pCPly);
int SetSkillRange_raw(lua_State* pLS);
void SetRangeState(int s1, int s2, int s3);
int GetSkillPos_raw(lua_State* L);
int GetSkillLv(CCharacter* pCCha, int nSkillID);
int GetChaStateLv(CCharacter* pCCha, int uchStateID);
int GetObjDire(CCharacter* pCCha);
void AddState(CCharacter* pCSrcCha, CCharacter* pCTarCha, int uchStateID, int uchStateLV, int nOnTime);
void RemoveState(CCharacter* pCCha, int uchStateID);
int GetAreaStateLevel(CCharacter* pCCha, int uchStateID);
void SkillMiss(CCharacter* pCCha);
void SkillCrt(CCharacter* pCCha);
void SkillUnable(CCharacter* pCCha);
int AddChaSkill_raw(lua_State* pLS);
void UseItemFailed(CCharacter* pCCha);
int SetItemFall_raw(lua_State* pLS);
void BeatBack(CCharacter* pCSrcCha, CCharacter* pCTarCha, int nBackLen);
int IsInGymkhana(CCharacter* pCCha);
int IsInPK(CCharacter* pCCha);
int CheckBagItem(CCharacter* pCCha, int sID);
int GetChaFreeTempBagGridNum(CCharacter* pCCha);
int GetChaFreeBagGridNum(CCharacter* pCCha);
int DelBagItem(CCharacter* pCCha, int sID, int sNum);
int DelBagItem2(CCharacter* pCCha, SItemGrid* pGridCont, int sNum);
int RemoveChaItem_raw(lua_State* pLS);
std::string GetChaMapName(CCharacter* pCCha);
int GetChaMapCopyNO(CCharacter* pCCha);
SubMap* GetChaMapCopy(CCharacter* pCCha);
CCharacter* GetMainCha(CCharacter* pCCha);
CCharacter* GetCtrlBoat(CCharacter* pCCha);
int ChaIsBoat(CCharacter* pCCha);
SItemGrid* GetChaItem(CCharacter* pCCha, int type, int id);
SItemGrid* GetChaItem2(CCharacter* pCCha, int type, int id);
int MoveToTemp(CCharacter* pCCha, SItemGrid* pSItem);
int GetItemAttr_raw(lua_State* pLS);
int GetItemStackSize(SItemGrid* pSItem);
int IsItemLocked(SItemGrid* pSItem);
int SetItemAttr_raw(lua_State* pLS);
int AddItemAttr_raw(lua_State* pLS);
int GetItemFinalAttr(SItemGrid* pSItem, int lAttrID);
int SetItemFinalAttr(SItemGrid* pSItem, int lAttrID, int sAttr);
int AddItemFinalAttr(SItemGrid* pSItem, int lAttrID, int sAttr);
int ResetItemFinalAttr(SItemGrid* pSItem);
int GetItemAttrRange(int sItemID, int sAttrID, int sType);
int GetItemForgeParam(SItemGrid* pSItem, int lType);
int SetItemForgeParam_raw(lua_State* pLS);
int AddEquipEnergy(CCharacter* pCCha, int chPos, int sItemType, int sVal);
void SetRelive(CCharacter* pCSrcCha, CCharacter* pCTarCha, int hpPercent, const std::string& mapName);
void LuaPrint();
void Stop(int seconds);
void Notice(const std::string& msg);
void GuildNotice(unsigned int guildID, const std::string& msg);
void ScrollNotice(const std::string& msg, int SetNum, unsigned int color);
void GMNotice(const std::string& gmNotice);
void ChaNotice(const std::string& cszChaName, const std::string& cszNotiStr);
void MapCopyNotice(SubMap* pCMapCopy, const std::string& msg);
void MapCopyNotice2(CMapRes* pCMap, int copyNO, const std::string& msg);
void MapChaLight();
int SetItemHost(CCharacter* pCDropCha, CCharacter* pCOwnCha);
std::string GetChaName(CCharacter* pCCha);
int SetMapEntryTime_raw(lua_State* pLS);
int MapCanSavePos(CMapRes* pCMap, int val);
void RepatriateDie(CMapRes* pCMap, int val);
int MapCanPK(CMapRes* pCMap, int val);
int MapCanTeam(CMapRes* pCMap, int val);
int MapCanStall(CMapRes* pCMap, int val);
int MapCanGuild(CMapRes* pCMap, int val);
int KillMyMonster(CCharacter* pCha, CCharacter* pChaMonster);
int KillMonsterInMapByName(SubMap* pSubmap, const std::string& pMonstername);
int MapCopyNum(CMapRes* pCMap, int num);
int MapCopyStartType(CMapRes* pCMap, int type);
int MapType(CMapRes* pCMap, int type);
int SingleMapCopyPlyNum(CMapRes* pCMap, int num);
int SetMapEntryMapName(CMapRes* pCMap, const std::string& name);
int SetMapEntryEntiID(CDynMapEntryCell* pEntry, int entiID, int eventID);
int GetMapEntryPosInfo_raw(lua_State* pLS);
int SetMapEntryEventName(CDynMapEntryCell* pEntry, const std::string& name);
int CallMapEntry(const std::string& szMapN);
int GetChaSideID(CCharacter* pCCha);
int SetChaSideID(CCharacter* pCCha, int lSideID);
int GetChaGuildID(CCharacter* pCCha);
int GetChaTeamID(CCharacter* pCCha);
int CheckChaPKState(CCharacter* pCCha);
std::string GetGuildName(int lGuildID);
int CloseMapEntry(const std::string& mapName);
int CloseMapCopy_raw(lua_State* pLS);
int SetChaMotto(CCharacter* pCCha, const std::string& cszMotto);
int IsChaInLand(CCharacter* pCCha);
int SetTeamFightMapName(const std::string& cszMapName);
int GetMapCopyParam(CMapEntryCopyCell* pCCpyMgr, int paramIdx);
int GetMapCopyParam2(SubMap* pCMapCpy, int paramIdx);
int GetMapCopyID(CMapEntryCopyCell* pCCpyMgr);
int GetMapCopyID2(SubMap* pCMapCopy);
int SetMapCopyParam(CMapEntryCopyCell* pCCpyMgr, int paramIdx, int val);
int SetMapCopyParam2(SubMap* pCMapCpy, int paramIdx, int val);
CMapEntryCopyCell* GetMapEntryCopyObj(CDynMapEntryCell* pCMapEntry, int sCopyID);
int GetMapCopyPlayerNum(SubMap* pCMapCopy);
void BeginGetMapCopyPlayerCha(SubMap* pCMapCopy);
CCharacter* GetMapCopyNextPlayerCha(SubMap* pCMapCopy);
int GetChaMapType(CCharacter* pCCha);
void SetChaKitbagChange(CCharacter* pCCha, int flag);
void SynChaKitbag(CCharacter* pCCha, int synType);
int GetChaMapOpenScale(CCharacter* pCCha);
void FinishSetMapEntryCopy(CDynMapEntryCell* pCMapEntryCell, int copyNO);
int GetItemType(SItemGrid* pSItem);
int GetItemType2(int sItemID);
int GetItemLv(SItemGrid* pSItem);
int GetItemOriginalLv(SItemGrid* pSItem);
void SetItemLv(SItemGrid* pSItem, int nItemLv);
int GetItemLv2(int sItemID);
int GetItemID(SItemGrid* pSItem);
int GetItemHoleNum(int sItemID);
int SetChaEquipValid(CCharacter* pCCha, int chEquipPos, int bValid);
int SetChaKbItemValid(CCharacter* pCCha, int chKbPos, int bValid, int bSyn);
int SetChaKbItemValid2(CCharacter* pCCha, SItemGrid* pSItem, int bValid, int bSyn);
int GetChallengeGuildID_raw(lua_State* pLS);
void EndGuildBid(int byLevel);
void EndGuildChallenge(unsigned int param1, unsigned int param2, unsigned int param3);
int AddKbCap(CCharacter* pCCha, int cap);
int GetKbCap(CCharacter* pCCha);
int IsInSameMap(CCharacter* pCCha1, CCharacter* pCCha2);
int IsInSameMapCopy(CCharacter* pCCha1, CCharacter* pCCha2);
int IsChaLiving(CCharacter* pCCha);
int SetChaParam(CCharacter* pCCha, int paramIdx, int val);
int GetChaParam(CCharacter* pCCha, int paramIdx);
int AddItemEffect(CCharacter* pCCha, SItemGrid* pItem, int bValid);
int Lua_raw(lua_State* pLS);
int LuaAll_raw(lua_State* pLS);
int ReloadCal_raw(lua_State* L);
void ReloadCal();
int GetWinLotteryItemno_raw(lua_State* pLS);
int CalWinLottery_raw(lua_State* pLS);
int GetLotteryIssue_raw(lua_State* pLS);
void AddLotteryIssue(int issue);
void DisuseLotteryIssue(int issue, int state);
int IsValidRegTeam_raw(lua_State* pLS);
int IsValidTeam_raw(lua_State* pLS);
int GetAmphitheaterSeason_raw(lua_State* pLS);
int GetAmphitheaterRound_raw(lua_State* pLS);
void AddAmphitheaterSeason(int season);
int DisuseAmphitheaterSeason(int season, int state, const std::string& winner);
int UpdateAmphitheaterRound(int season, int round);
int GetAmphitheaterTeamCount_raw(lua_State* pLS);
int GetAmphitheaterNoUseTeamID_raw(lua_State* pLS);
int AmphitheaterTeamSignUP_raw(lua_State* pLS);
int AmphitheaterTeamCancel(int teamID);
int IsAmphitheaterLogin(CCharacter* pActor);
int IsMapFull_raw(lua_State* pLS);
int UpdateMapAfterEnter(CCharacter* Captain, int MapID);
int UpdateMap(int Mapid);
int UpdateMapNum(int Teamid, int Mapid, int MapFlag);
int GetMapFlag_raw(lua_State* pLS);
int SetMaxBallotTeamRelive_raw(lua_State* pLS);
int SetMatchResult(int Teamid1, int Teamid2, int Id1state, int Id2state);
int GetCaptainByMapId_raw(lua_State* pLS);
int UpdateAbsentTeamRelive_raw(lua_State* pLS);
int UpdateWinnum(int teamid);
int GetUniqueMaxWinnum_raw(lua_State* pLS);
int SetMatchnoState(int teamid);
int UpdateState_raw(lua_State* pLS);
int CloseReliveByState_raw(lua_State* pLS);
int CleanMapFlag(int teamid1, int teamid2);
int GetStateByTeamid_raw(lua_State* pLS);
void LookEnergy(CCharacter* pCCha);
int SetExpiration_raw(lua_State* L);

void RegisterLuaGameLogic(lua_State* L);
