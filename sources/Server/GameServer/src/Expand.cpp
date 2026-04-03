// Expand.cpp — function implementations moved from Expand.h

#include "Character.h"
#include "excp.h"
#include "GameDB.h"
#include "MapEntry.h"
#include "Script.h"
#include "GameApp.h"
#include "GameServerApp.h"
#include "SubMap.h"
#include "lua_gamectrl.h"
#include <tuple>

_DBC_USING

extern const char* GetResPath(const char* pszRes);

void SynLook(CCharacter* pCha) {
	if (!pCha) return;
	pCha->SynSkillStateToEyeshot();
	pCha->SynLook();
}

int IsAttributeEditable(SItemGrid* item, int attribute) {
	if (!item) return -1;
	for (int i = 0; i < 5; i++) {
		if (item->sInstAttr[i][0] == attribute) {
			return i;
		}
	}
	return -1;
}

void SetAttributeEditable(SItemGrid* item, int slot, int attribute) {
	if (!item) return;
	if (slot >= 0 && slot < 5) {
		item->sInstAttr[slot][0] = attribute;
		item->sInstAttr[slot][1] = g_itemAttrMap[item->sID].GetAttr(attribute, false);
	}
}

void EquipItem(CCharacter* pCha, int chEquipPos, SItemGrid* equip) {
	if (!pCha || !equip) return;
	pCha->m_SChaPart.SLink[chEquipPos] = *equip;
	pCha->m_SChaPart.SLink[chEquipPos].SetChange();
	pCha->ChangeItem(true, equip, chEquipPos);
	pCha->SynSkillStateToEyeshot();
	pCha->SynLook();
}

void EquipStringItem() {
	// Commented out in original
}

int GetChaGuildPermission(CCharacter* pCha) {
	if (!pCha) return 0;
	return pCha->guildPermission;
}

int GetIMP(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->GetIMP();
}

// Variable args: 2 or 3 params with different behavior
int SetIMP_raw(lua_State* pLS) {
	if ((lua_gettop(pLS) == 2 || lua_gettop(pLS) == 3) && lua_islightuserdata(pLS, 1) && lua_isnumber(pLS, 2)) {
		auto pCChaResult = luabridge::Stack<CCharacter*>::get(pLS, 1);
		if (!pCChaResult) { PARAM_ERROR return 0; }
		CCharacter* pCCha = *pCChaResult;
		int IMP = lua_tonumber(pLS, 2);
		if (lua_gettop(pLS) == 3) {
			pCCha->SetIMP(IMP, false);
		}
		else {
			pCCha->SetIMP(IMP, true);
		}
	}
	return 0;
}

// GetChaAttr has special unsigned handling
int GetChaAttr_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum > 2) return 0;

	auto pCChaResult = luabridge::Stack<CCharacter*>::get(pLS, 1);
	if (!pCChaResult) { PARAM_ERROR return 0; }
	CCharacter* pCCha = *pCChaResult;
	short sAttrIndex = (unsigned __int64)lua_tonumber(pLS, 2);
	if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM) return 0;

	int nAttrVal = pCCha->getAttr(sAttrIndex);

	if (nAttrVal < 0 && g_IsNosignChaAttr(sAttrIndex))
		lua_pushnumber(pLS, (double)(unsigned int)nAttrVal);
	else
		lua_pushnumber(pLS, (double)nAttrVal);
	return 1;
}

int SetChaAttr(CCharacter* pCCha, int sAttrIndex, int64_t lValue) {
	if (!pCCha) return 0;
	if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM) return 0;
	auto clamped = std::clamp(lValue, static_cast<int64_t>(INT32_MIN), static_cast<int64_t>(INT32_MAX));
	long lSetRet = pCCha->setAttr(sAttrIndex, static_cast<LONG32>(clamped));
	return lSetRet != 0 ? 1 : 0;
}

int CheckChaRole(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->IsPlayerCha() ? 1 : 0;
}

CPlayer* GetChaPlayer(CCharacter* pCCha) {
	if (!pCCha) return nullptr;
	return pCCha->GetPlayer();
}

int GetPlayerTeamID(CPlayer* pCPly) {
	if (!pCPly) return 0;
	return pCPly->getTeamLeaderID();
}

int GetPlayerID(CPlayer* pCPly) {
	if (!pCPly) return 0;
	return pCPly->GetID();
}

// Variable args: 1..defSKILL_RANGE_EXTEP_NUM params
int SetSkillRange_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum < 1 || nParaNum >= defSKILL_RANGE_EXTEP_NUM)
		return 0;

	short sRangeE[defSKILL_RANGE_EXTEP_NUM];
	for (int i = 0; i < nParaNum; i++)
		sRangeE[i] = (short)lua_tonumber(pLS, i + 1);
	g_pGameApp->SetSkillTDataRange(sRangeE);
	return 0;
}

void SetRangeState(int s1, int s2, int s3) {
	short sState[defSKILL_STATE_PARAM_NUM];
	sState[0] = (short)s1;
	sState[1] = (short)s2;
	sState[2] = (short)s3;
	g_pGameApp->SetSkillTDataState(sState);
}

int GetSkillPos_raw(lua_State* L) {
	lua_pushinteger(L, g_SSkillPoint.x);
	lua_pushinteger(L, g_SSkillPoint.y);
	return 2;
}

int GetSkillLv(CCharacter* pCCha, int nSkillID) {
	if (!pCCha) return 0;
	SSkillGrid* pSkill = pCCha->m_CSkillBag.GetSkillContByID(nSkillID);
	if (pSkill)
		return pSkill->chLv;
	return 0;
}

int GetChaStateLv(CCharacter* pCCha, int uchStateID) {
	if (!pCCha) return 0;
	SSkillStateUnit* pState = pCCha->m_CSkillState.GetSStateByID((unsigned char)uchStateID);
	if (pState)
		return pState->GetStateLv();
	return 0;
}

int GetObjDire(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->GetAngle();
}

void AddState(CCharacter* pCSrcCha, CCharacter* pCTarCha, int uchStateID, int uchStateLV, int nOnTime) {
	if (!pCSrcCha || !pCTarCha) return;
	pCTarCha->AddSkillState(g_uchFightID, pCSrcCha->GetID(), pCSrcCha->GetHandle(),
							enumSKILL_TYPE_SELF, enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL,
							(unsigned char)uchStateID, (unsigned char)uchStateLV, nOnTime,
							enumSSTATE_ADD_UNDEFINED, false);
}

void RemoveState(CCharacter* pCCha, int uchStateID) {
	if (!pCCha) return;
	pCCha->DelSkillState((unsigned char)uchStateID);
}

int GetAreaStateLevel(CCharacter* pCCha, int uchStateID) {
	unsigned char uchStateLv = 0;
	if (!pCCha) return 0;
	SSkillStateUnit* pState = pCCha->GetAreaState((unsigned char)uchStateID);
	if (pState)
		uchStateLv = pState->GetStateLv();
	return uchStateLv;
}

void SkillMiss(CCharacter* pCCha) {
	if (!pCCha) return;
	pCCha->m_SFightProc.bMiss = true;
}

void SkillCrt(CCharacter* pCCha) {
	if (!pCCha) return;
	pCCha->m_SFightProc.bCrt = true;
}

void SkillUnable(CCharacter* pCCha) {
	if (!pCCha) return;
	pCCha->m_SFightProc.sState = enumFSTATE_NO_EXPEND;
	ToLogService("errors", LogLevel::Error, "skill failed, role's name {}", pCCha->GetLogName());
}

// Variable args: 5 or 6 params
int AddChaSkill_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum < 5) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	auto pCChaResult = luabridge::Stack<CCharacter*>::get(pLS, 1);
	if (!pCChaResult) { PARAM_ERROR lua_pushnumber(pLS, 0); return 1; }
	CCharacter* pCCha = *pCChaResult;
	int nSkillID = (int)lua_tonumber(pLS, 2);
	int nSkillLv = (int)lua_tonumber(pLS, 3);
	bool bSetLv = (int)lua_tonumber(pLS, 4) == 1 ? true : false;
	bool bUsePoint = (int)lua_tonumber(pLS, 5) == 1 ? true : false;
	bool checkReq = true;
	if (nParaNum == 6) {
		checkReq = (int)lua_tonumber(pLS, 6) == 1 ? false : true;
	}
	if (pCCha->GetPlayer())
		pCCha = pCCha->GetPlayer()->GetMainCha();

	if (!pCCha->LearnSkill((short)nSkillID, (char)nSkillLv, bSetLv, bUsePoint, checkReq)) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	lua_pushnumber(pLS, 1);
	return 1;
}

void UseItemFailed(CCharacter* pCCha) {
	if (!pCCha) return;
	g_chUseItemFailed[0] = 1;
}

// Variable args: 1..N params
int SetItemFall_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum < 1) return 0;

	g_chItemFall[0] = (char)lua_tonumber(pLS, 1);
	if (g_chItemFall[0] > defCHA_INIT_ITEM_NUM) return 0;
	for (int i = 0; i < g_chItemFall[0]; i++)
		g_chItemFall[i + 1] = (char)lua_tonumber(pLS, i + 2);
	return 0;
}

void BeatBack(CCharacter* pCSrcCha, CCharacter* pCTarCha, int nBackLen) {
	if (!pCSrcCha || !pCTarCha) return;

	Point SSrcPos, STarPos;
	Point STarNewPos;
	SSrcPos = pCSrcCha->GetPos();
	STarPos = pCTarCha->GetPos();
	int nDist1 = (int)sqrt(double(
		(SSrcPos.x - STarPos.x) * (SSrcPos.x - STarPos.x) + (SSrcPos.y - STarPos.y) * (SSrcPos.y - STarPos.y)));
	int nDist2 = nDist1 + nBackLen;
	STarNewPos.x = nDist2 * (STarPos.x - SSrcPos.x) / nDist1 + SSrcPos.x;
	STarNewPos.y = nDist2 * (STarPos.y - SSrcPos.y) / nDist1 + SSrcPos.y;
	unsigned long ulElapse;
	pCTarCha->LinearAttemptMove(STarNewPos, nBackLen, &ulElapse);
	STarNewPos = pCTarCha->GetPos();
	pCTarCha->SetPos(STarPos);
	pCTarCha->m_submap->MoveTo(pCTarCha, STarNewPos);
	g_bBeatBack = true;
}

int IsInGymkhana(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->IsInGymkhana() ? 1 : 0;
}

int IsInPK(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->IsInPK() ? 1 : 0;
}

int CheckBagItem(CCharacter* pCCha, int sID) {
	short sItemNum = 0;
	if (!pCCha) return 0;

	short sGridNum = pCCha->m_CKitbag.GetUseGridNum();
	SItemGrid* pGridCont;
	for (short i = 0; i < sGridNum; i++) {
		pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
		if (!pGridCont || pGridCont->sID != (short)sID)
			continue;
		sItemNum += pGridCont->sNum;
	}
	return sItemNum;
}

int GetChaFreeTempBagGridNum(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->m_pCKitbagTmp->GetCapacity() - pCCha->m_pCKitbagTmp->GetUseGridNum();
}

int GetChaFreeBagGridNum(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->m_CKitbag.GetCapacity() - pCCha->m_CKitbag.GetUseGridNum();
}

int DelBagItem(CCharacter* pCCha, int sID, int sNum) {
	if (!pCCha) return 0;

	SItemGrid *pGridCont, DelCont;
	short sLeftNum = (short)sNum;
	short sGridNum = pCCha->m_CKitbag.GetUseGridNum();
	for (short i = 0; i < sGridNum; i++) {
		pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
		if (!pGridCont || pGridCont->sID != (short)sID)
			continue;
		sLeftNum -= pGridCont->sNum;
		if (sLeftNum <= 0)
			break;
	}
	if (sLeftNum > 0) return 0;

	sLeftNum = (short)sNum;
	pCCha->m_CKitbag.SetChangeFlag(false);
	DelCont.sID = (short)sID;
	for (short i = 0; i < sGridNum; i++) {
		pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
		if (!pGridCont || pGridCont->sID != (short)sID)
			continue;
		if (pGridCont->sNum >= sLeftNum)
			DelCont.sNum = sLeftNum;
		else
			DelCont.sNum = pGridCont->sNum;
		pCCha->KbPopItem(true, true, &DelCont, pCCha->m_CKitbag.GetPosIDByNum(i));
		sLeftNum -= pGridCont->sNum;
		if (sLeftNum <= 0)
			break;
	}
	pCCha->SynKitbagNew(enumSYN_KITBAG_TO_NPC);
	return 1;
}

int DelBagItem2(CCharacter* pCCha, SItemGrid* pGridCont, int sNum) {
	if (!pCCha || !pGridCont) return 0;
	if (sNum == 0)
		sNum = pGridCont->sNum;
	if (pGridCont->sNum < (short)sNum) return 0;

	pCCha->m_CKitbag.SetChangeFlag(false);
	pCCha->KbClearItem(true, true, pGridCont, (short)sNum);
	pCCha->SynKitbagNew(enumSYN_KITBAG_TO_NPC);
	return 1;
}

// Variable args: 7 or 8 params
int RemoveChaItem_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum < 7) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	auto pCChaResult = luabridge::Stack<CCharacter*>::get(pLS, 1);
	if (!pCChaResult) { PARAM_ERROR lua_pushnumber(pLS, 0); return 1; }
	CCharacter* pCCha = *pCChaResult;
	if (pCCha->m_CKitbag.IsLock()) {
		pCCha->SystemNotice("Unable to remove item. Inventory locked!");
		lua_pushnumber(pLS, 0);
		return 1;
	}

	long lItemID = (long)lua_tonumber(pLS, 2);
	long lItemNum = (long)lua_tonumber(pLS, 3);
	char chFromType = (char)lua_tonumber(pLS, 4);
	short sFromID = (short)lua_tonumber(pLS, 5);
	char chToType = (char)lua_tonumber(pLS, 6);
	char chForcible = (char)lua_tonumber(pLS, 7);
	bool bNotice = true;
	if (nParaNum == 8)
		bNotice = (char)lua_tonumber(pLS, 8) != 0 ? true : false;

	if (pCCha->Cmd_RemoveItem(lItemID, lItemNum, chFromType, sFromID, chToType, 0, bNotice, chForcible) !=
		enumITEMOPT_SUCCESS) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	lua_pushnumber(pLS, 1);
	return 1;
}

std::string GetChaMapName(CCharacter* pCCha) {
	if (!pCCha) return "";
	if (!pCCha->GetSubMap()) return "";
	return std::string(pCCha->GetSubMap()->GetName());
}

int GetChaMapCopyNO(CCharacter* pCCha) {
	if (!pCCha) return 0;
	if (!pCCha->GetSubMap()) return 0;
	return pCCha->GetSubMap()->GetCopyNO() + 1;
}

SubMap* GetChaMapCopy(CCharacter* pCCha) {
	if (!pCCha) return nullptr;
	return pCCha->GetSubMap();
}

CCharacter* GetMainCha(CCharacter* pCCha) {
	if (!pCCha || !pCCha->GetPlayer()) return nullptr;
	return pCCha->GetPlayer()->GetMainCha();
}

CCharacter* GetCtrlBoat(CCharacter* pCCha) {
	if (!pCCha || !pCCha->GetPlayer()) return nullptr;
	CCharacter* pCCtrlBoat = pCCha->GetPlayer()->GetCtrlCha();
	if (!pCCtrlBoat->IsBoat()) return nullptr;
	return pCCtrlBoat;
}

int ChaIsBoat(CCharacter* pCCha) {
	if (!pCCha || !pCCha->GetPlayer()) return 0;
	return pCCha->IsBoat() ? 1 : 0;
}

SItemGrid* GetChaItem(CCharacter* pCCha, int type, int id) {
	if (!pCCha) return nullptr;
	return pCCha->GetItem2((char)type, (long)id);
}

SItemGrid* GetChaItem2(CCharacter* pCCha, int type, int id) {
	if (!pCCha) return nullptr;
	return pCCha->GetItem((char)type, (long)id);
}

int MoveToTemp(CCharacter* pCCha, SItemGrid* pSItem) {
	if (!pCCha || !pSItem) return LUA_FALSE;

	short sPushPos = defKITBAG_DEFPUSH_POS;
	SItemGrid Grid;
	Grid.sNum = 0;

	short slot = -1;
	SItemGrid* pSItemCont;
	for (int i = 0; i < 48; i++) {
		pSItemCont = pCCha->m_CKitbag.GetGridContByID(i);
		if (pSItemCont == pSItem) {
			slot = i;
			break;
		}
	}
	if (slot == -1) return LUA_FALSE;

	pCCha->m_pCKitbagTmp->Push(pSItem, sPushPos);
	pCCha->m_CKitbag.Pop(&Grid, slot);
	pCCha->SynKitbagNew(enumSYN_KITBAG_SWITCH);

	if (Grid.sNum > 0) {
		pCCha->m_CKitbag.Push(&Grid, slot);
		pCCha->SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
		pCCha->SynKitbagNew(enumSYN_KITBAG_SWITCH);
		return LUA_FALSE;
	}

	pCCha->SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
	return LUA_TRUE;
}

// GetItemAttr has complex branching on attrID
int GetItemAttr_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	long lItemAttr = 0;

	if (nParaNum != 2) return 0;

	auto pSItemResult = luabridge::Stack<SItemGrid*>::get(pLS, 1);
	if (!pSItemResult) { PARAM_ERROR return 0; }
	SItemGrid* pSItem = *pSItemResult;

	long lAttrID = (int)lua_tonumber(pLS, 2);
	if (lAttrID == ITEMATTR_VAL_PARAM1)
		lItemAttr = pSItem->GetDBParam(0);
	else if (lAttrID == ITEMATTR_VAL_PARAM2)
		lItemAttr = pSItem->GetDBParam(1);
	else if (lAttrID == ITEMATTR_VAL_LEVEL)
		lItemAttr = pSItem->GetItemLevel();
	else if (lAttrID == ITEMATTR_VAL_FUSIONID)
		lItemAttr = pSItem->GetFusionItemID();
	else
		lItemAttr = pSItem->GetInstAttr(lAttrID);

	lua_pushnumber(pLS, lItemAttr);
	return 1;
}

int GetItemStackSize(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	return pSItem->sNum;
}

int IsItemLocked(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	return pSItem->dwDBID > 0 ? 1 : 0;
}

// SetItemAttr has complex branching on attrID
int SetItemAttr_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	bool bSuccess = true;

	if (nParaNum != 3) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	auto pSItemResult = luabridge::Stack<SItemGrid*>::get(pLS, 1);
	if (!pSItemResult) { PARAM_ERROR lua_pushnumber(pLS, 0); return 1; }
	SItemGrid* pSItem = *pSItemResult;

	long lAttrID = (int)lua_tonumber(pLS, 2);
	short sAttr = (short)lua_tonumber(pLS, 3);
	if (lAttrID == ITEMATTR_VAL_PARAM1)
		pSItem->SetDBParam(0, sAttr);
	else if (lAttrID == ITEMATTR_VAL_PARAM2)
		pSItem->SetDBParam(1, sAttr);
	else if (lAttrID == ITEMATTR_VAL_LEVEL)
		pSItem->SetItemLevel(char(sAttr));
	else if (lAttrID == ITEMATTR_VAL_FUSIONID)
		pSItem->SetFusionItemID(sAttr);
	else {
		if (!pSItem->SetInstAttr(lAttrID, sAttr))
			bSuccess = false;
	}

	lua_pushnumber(pLS, bSuccess ? 1 : 0);
	return 1;
}

// AddItemAttr has complex branching on attrID
int AddItemAttr_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	bool bSuccess = true;

	if (nParaNum != 3) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	auto pSItemResult = luabridge::Stack<SItemGrid*>::get(pLS, 1);
	if (!pSItemResult) { PARAM_ERROR lua_pushnumber(pLS, 0); return 1; }
	SItemGrid* pSItem = *pSItemResult;

	long lAttrID = (int)lua_tonumber(pLS, 2);
	short sAttr = (short)lua_tonumber(pLS, 3);

	if (lAttrID == ITEMATTR_VAL_PARAM1) {
	}
	else if (lAttrID == ITEMATTR_VAL_PARAM2) {
	}
	else if (lAttrID == ITEMATTR_VAL_LEVEL)
		pSItem->AddItemLevel(char(sAttr));
	else if (lAttrID == ITEMATTR_VAL_FUSIONID) {
	}
	else {
		if (!pSItem->SetInstAttr(lAttrID, sAttr))
			bSuccess = false;
	}

	lua_pushnumber(pLS, bSuccess ? 1 : 0);
	return 1;
}

int GetItemFinalAttr(SItemGrid* pSItem, int lAttrID) {
	if (!pSItem) return 0;
	return pSItem->GetAttr(lAttrID);
}

int SetItemFinalAttr(SItemGrid* pSItem, int lAttrID, int sAttr) {
	if (!pSItem) return 0;
	if (!pSItem->SetAttr(lAttrID, (short)sAttr)) return 0;
	return 1;
}

int AddItemFinalAttr(SItemGrid* pSItem, int lAttrID, int sAttr) {
	if (!pSItem) return 0;
	if (!pSItem->AddAttr(lAttrID, (short)sAttr)) return 0;
	return 1;
}

int ResetItemFinalAttr(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	if (!pSItem->InitAttr()) return 0;
	return 1;
}

int GetItemAttrRange(int sItemID, int sAttrID, int sType) {
	CItemRecord* pCItemRec = GetItemRecordInfo((short)sItemID);
	if (!pCItemRec) return 0;
	bool bMax = (sType == 0) ? false : true;
	return g_itemAttrMap[(short)sItemID].GetAttr((short)sAttrID, bMax);
}

int GetItemForgeParam(SItemGrid* pSItem, int lType) {
	if (!pSItem) return 0;
	if (lType == 0)
		return pSItem->GetForgeLv();
	else
		return pSItem->GetDBParam(enumITEMDBP_FORGE);
}

// SetItemForgeParam has a cast to unsigned __int64 for param 3
int SetItemForgeParam_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 3) {
		lua_pushnumber(pLS, 1);
		return 1;
	}

	auto pSItemResult = luabridge::Stack<SItemGrid*>::get(pLS, 1);
	if (!pSItemResult) { PARAM_ERROR lua_pushnumber(pLS, 1); return 1; }
	SItemGrid* pSItem = *pSItemResult;

	long lType = (int)lua_tonumber(pLS, 2);
	if (lType == 0)
		pSItem->SetForgeLv((char)lua_tonumber(pLS, 3));
	else
		pSItem->SetDBParam(enumITEMDBP_FORGE, (unsigned __int64)lua_tonumber(pLS, 3));

	lua_pushnumber(pLS, 1);
	return 1;
}

int AddEquipEnergy(CCharacter* pCCha, int chPos, int sItemType, int sVal) {
	if (!pCCha || !pCCha->GetPlayer()) return 0;
	if (pCCha->GetPlayer())
		pCCha = pCCha->GetPlayer()->GetMainCha();

	if (chPos < enumEQUIP_HEAD || chPos >= enumEQUIP_NUM) return 0;

	SItemGrid* pEquip = &pCCha->m_SChaPart.SLink[chPos];
	if (pEquip->sID > 0) {
		if (GetItemRecordInfo(pEquip->sID)->sType == (short)sItemType)
			pEquip->AddInstAttr(ITEMATTR_ENERGY, (short)sVal);
	}
	return 1;
}

void SetRelive(CCharacter* pCSrcCha, CCharacter* pCTarCha, int hpPercent, const std::string& mapName) {
	if (!pCSrcCha || !pCSrcCha->GetPlayer() || !pCTarCha || !pCTarCha->GetPlayer()) return;
	if (pCTarCha->GetChaRelive()) {
		pCSrcCha->SystemNotice(RES_STRING(GM_EXPAND_H_00007));
		return;
	}
	if (!pCTarCha->IsBoat()) {
		pCTarCha->SetRelive(enumEPLAYER_RELIVE_ORIGIN, hpPercent, mapName.c_str());
	}
}

void LuaPrint() {
	// No-op
}

void Stop(int seconds) {
	g_pGameApp->m_CTimerReset.Begin(1000);
	g_pGameApp->m_ulLeftSec = seconds;
}

void Notice(const std::string& msg) {
	g_pGameApp->WorldNotice(msg.c_str());
	if (strstr(msg.c_str(), RES_STRING(GM_EXPAND_H_00102)))
		if (g_cchLogMapEntry)
			ToLogService("common", "system notice : {}", msg.c_str());
}

void GuildNotice(unsigned int guildID, const std::string& msg) {
	g_pGameApp->GuildNotice((DWORD)guildID, msg.c_str());
}

void ScrollNotice(const std::string& msg, int SetNum, unsigned int color) {
	g_pGameApp->ScrollNotice(msg.c_str(), SetNum, (DWORD)color);
	if (strstr(msg.c_str(), RES_STRING(GM_EXPAND_H_00102)))
		if (g_cchLogMapEntry)
			ToLogService("common", "system notice : {}", msg.c_str());
}

void GMNotice(const std::string& gmNotice) {
	g_pGameApp->GMNotice(gmNotice.c_str());
	if (strstr(gmNotice.c_str(), RES_STRING(GM_EXPAND_H_00102)))
		if (g_cchLogMapEntry)
			ToLogService("common", "system notice : {}", gmNotice.c_str());
}

void ChaNotice(const std::string& cszChaName, const std::string& cszNotiStr) {
	g_pGameApp->ChaNotice(cszNotiStr.c_str(), cszChaName.c_str());
	if (strstr(cszNotiStr.c_str(), RES_STRING(GM_EXPAND_H_00102)))
		if (g_cchLogMapEntry)
			ToLogService("common", "system notice : {}", cszNotiStr.c_str());
}

void MapCopyNotice(SubMap* pCMapCopy, const std::string& msg) {
	if (!pCMapCopy) return;
	pCMapCopy->Notice(msg.c_str());
}

void MapCopyNotice2(CMapRes* pCMap, int copyNO, const std::string& msg) {
	if (!pCMap) return;
	pCMap->CopyNotice(msg.c_str(), (short)copyNO - 1);
}

void MapChaLight() {
	// No-op
}

int SetItemHost(CCharacter* pCDropCha, CCharacter* pCOwnCha) {
	if (!pCDropCha || !pCOwnCha) return 0;
	pCDropCha->SetItemHostObj(pCOwnCha);
	return 1;
}

std::string GetChaName(CCharacter* pCCha) {
	if (!pCCha) return "";
	return std::string(pCCha->GetLogName());
}

// SetMapEntryTime has complex string parsing
int SetMapEntryTime_raw(lua_State* pLS) {
	bool bSuccess = true;
	std::string strList[5];
	int nParaNum = lua_gettop(pLS);

	if (nParaNum != 5) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	auto pCMapResult = luabridge::Stack<CMapRes*>::get(pLS, 1);
	if (!pCMapResult) { PARAM_ERROR lua_pushnumber(pLS, 0); return 1; }
	auto pCMap = *pCMapResult;

	struct tm time_set, *time_get;
	time_t timep;
	time(&timep);
	time_get = localtime(&timep);
	const char* szTime = (const char*)lua_tostring(pLS, 2);
	int n = Util_ResolveTextLine(szTime, strList, 5, '/');
	if (n != 5) {
		MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
		lua_pushnumber(pLS, 0);
		return 1;
	}
	time_set.tm_year = Str2Int(strList[0]) - 1900;
	time_set.tm_mon = Str2Int(strList[1]) - 1;
	time_set.tm_mday = Str2Int(strList[2]);
	time_set.tm_hour = Str2Int(strList[3]);
	time_set.tm_min = Str2Int(strList[4]);
	time_set.tm_sec = 0;
	time_set.tm_isdst = time_get->tm_isdst;
	pCMap->m_tEntryFirstTm = mktime(&time_set);

	szTime = (const char*)lua_tostring(pLS, 3);
	n = Util_ResolveTextLine(szTime, strList, 4, '/');
	if (n != 3) {
		MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
		lua_pushnumber(pLS, 0);
		return 1;
	}
	pCMap->m_tEntryTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;

	szTime = (const char*)lua_tostring(pLS, 4);
	n = Util_ResolveTextLine(szTime, strList, 4, '/');
	if (n != 3) {
		MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
		lua_pushnumber(pLS, 0);
		return 1;
	}
	pCMap->m_tEntryOutTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;

	szTime = (const char*)lua_tostring(pLS, 5);
	n = Util_ResolveTextLine(szTime, strList, 4, '/');
	if (n != 3) {
		MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
		lua_pushnumber(pLS, 0);
		return 1;
	}
	pCMap->m_tMapClsTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;

	lua_pushnumber(pLS, 1);
	return 1;
}

int MapCanSavePos(CMapRes* pCMap, int val) {
	if (!pCMap) return 0;
	pCMap->SetCanSavePos(val != 0);
	return 1;
}

void RepatriateDie(CMapRes* pCMap, int val) {
	if (!pCMap) return;
	pCMap->SetRepatriateDie(val != 0);
}

int MapCanPK(CMapRes* pCMap, int val) {
	if (!pCMap) return 0;
	pCMap->SetCanPK(val != 0);
	return 1;
}

int MapCanTeam(CMapRes* pCMap, int val) {
	if (!pCMap) return 0;
	pCMap->SetCanTeam(val != 0);
	return 1;
}

int MapCanStall(CMapRes* pCMap, int val) {
	if (!pCMap) return 0;
	pCMap->SetCanStall(val != 0);
	return 1;
}

int MapCanGuild(CMapRes* pCMap, int val) {
	if (!pCMap) return 0;
	pCMap->SetCanGuild(val != 0);
	return 1;
}

int KillMyMonster(CCharacter* pCha, CCharacter* pChaMonster) {
	if (!pCha || !pChaMonster) return 0;
	pChaMonster->Free();
	return 1;
}

int KillMonsterInMapByName(SubMap* pSubmap, const std::string& pMonstername) {
	if (!pSubmap) return 0;
	pSubmap->ClearAllMonsterByName(pMonstername.c_str());
	return 1;
}

int MapCopyNum(CMapRes* pCMap, int num) {
	if (!pCMap) return 0;
	pCMap->SetCopyNum((short)num);
	return 1;
}

int MapCopyStartType(CMapRes* pCMap, int type) {
	if (!pCMap) return 0;
	pCMap->SetCopyStartType((char)type);
	return 1;
}

int MapType(CMapRes* pCMap, int type) {
	if (!pCMap) return 0;
	pCMap->SetType((char)type);
	return 1;
}

int SingleMapCopyPlyNum(CMapRes* pCMap, int num) {
	if (!pCMap) return 0;
	pCMap->SetCopyPlyNum((short)num);
	return 1;
}

int SetMapEntryMapName(CMapRes* pCMap, const std::string& name) {
	if (!pCMap) return 0;
	pCMap->SetEntryMapName(name.c_str());
	return 1;
}

int SetMapEntryEntiID(CDynMapEntryCell* pEntry, int entiID, int eventID) {
	if (!pEntry) return 0;
	pEntry->SetEntiID(entiID);
	pEntry->SetEventID(eventID);
	return 1;
}

// GetMapEntryPosInfo returns 4 values
int GetMapEntryPosInfo_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 1) {
		ToLogService("errors", LogLevel::Error, "\tThe parameter numbers [{}] is unlawful, transfer failed!", nParaNum);
		return 0;
	}

	auto pEntryResult = luabridge::Stack<CDynMapEntryCell*>::get(pLS, 1);
	if (!pEntryResult) { PARAM_ERROR return 0; }
	CDynMapEntryCell* pEntry = *pEntryResult;

	const char* pMapN = "";
	const char* pTMapN = "";
	long lPosX = 0, lPosY = 0;
	pEntry->GetPosInfo(&pMapN, &lPosX, &lPosY, &pTMapN);

	lua_pushstring(pLS, pMapN);
	lua_pushnumber(pLS, lPosX / 100);
	lua_pushnumber(pLS, lPosY / 100);
	lua_pushstring(pLS, pTMapN);
	return 4;
}

int SetMapEntryEventName(CDynMapEntryCell* pEntry, const std::string& name) {
	if (!pEntry) return 0;
	pEntry->SetEventName(name.c_str());
	return 1;
}

int CallMapEntry(const std::string& szMapN) {
	CMapRes* pCMap = g_pGameApp->FindMapByName(szMapN.c_str());
	if (!pCMap) return 0;
	pCMap->CreateEntry();
	return 1;
}

int GetChaSideID(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->GetSideID();
}

int SetChaSideID(CCharacter* pCCha, int lSideID) {
	if (!pCCha) return 0;
	pCCha->SetSideID(lSideID);
	return 1;
}

int GetChaGuildID(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->GetValidGuildID();
}

int GetChaTeamID(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->GetTeamID();
}

int CheckChaPKState(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->CanPK() ? 1 : 0;
}

std::string GetGuildName(int lGuildID) {
	std::string strGuildName;
	game_db.GetGuildName(lGuildID, strGuildName);
	return strGuildName;
}

int CloseMapEntry(const std::string& mapName) {
	CMapRes* pCMap = g_pGameApp->FindMapByName(mapName.c_str());
	if (!pCMap || !pCMap->CloseEntry()) return 0;
	return 1;
}

// Variable args: 1 or 2 params
int CloseMapCopy_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 2 && nParaNum != 1) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	short sCopyNO = -1;
	if (nParaNum == 2)
		sCopyNO = (short)lua_tonumber(pLS, 2) - 1;
	if (sCopyNO < 0)
		sCopyNO = 0;
	CMapRes* pCMap = g_pGameApp->FindMapByName(lua_tostring(pLS, 1));
	if (!pCMap || !pCMap->ReleaseCopy(sCopyNO)) {
		lua_pushnumber(pLS, 0);
		return 1;
	}

	lua_pushnumber(pLS, 1);
	return 1;
}

int SetChaMotto(CCharacter* pCCha, const std::string& cszMotto) {
	if (!pCCha) return 0;
	pCCha->SetMotto(cszMotto.c_str());
	return 1;
}

int IsChaInLand(CCharacter* pCCha) {
	if (!pCCha) return 0;
	if (!pCCha->GetSubMap()) return 0;
	short sAreaAttr = pCCha->GetSubMap()->GetAreaAttr(pCCha->GetPos());
	return g_IsLand(sAreaAttr) ? 1 : 0;
}

int SetTeamFightMapName(const std::string& cszMapName) {
	g_SetTeamFightMapName(cszMapName.c_str());
	return 1;
}

int GetMapCopyParam(CMapEntryCopyCell* pCCpyMgr, int paramIdx) {
	if (!pCCpyMgr) return 0;
	return pCCpyMgr->GetParam((char)paramIdx - 1);
}

int GetMapCopyParam2(SubMap* pCMapCpy, int paramIdx) {
	if (!pCMapCpy) return 0;
	return pCMapCpy->GetInfoParam((char)paramIdx - 1);
}

int GetMapCopyID(CMapEntryCopyCell* pCCpyMgr) {
	if (!pCCpyMgr) return 0;
	return pCCpyMgr->GetPosID() + 1;
}

int GetMapCopyID2(SubMap* pCMapCopy) {
	if (!pCMapCopy) return 0;
	return pCMapCopy->GetCopyNO() + 1;
}

int SetMapCopyParam(CMapEntryCopyCell* pCCpyMgr, int paramIdx, int val) {
	if (!pCCpyMgr) return 0;
	if (!pCCpyMgr->SetParam((char)paramIdx - 1, (long)val)) return 0;
	return 1;
}

int SetMapCopyParam2(SubMap* pCMapCpy, int paramIdx, int val) {
	if (!pCMapCpy) return 0;
	if (!pCMapCpy->SetInfoParam((char)paramIdx - 1, (long)val)) return 0;
	return 1;
}

CMapEntryCopyCell* GetMapEntryCopyObj(CDynMapEntryCell* pCMapEntry, int sCopyID) {
	if (!pCMapEntry) return nullptr;
	short copyIdx = (short)sCopyID - 1;
	CMapEntryCopyCell* pCCopyCell = pCMapEntry->GetCopy(copyIdx);
	if (!pCCopyCell) {
		CMapEntryCopyCell CCopyCell(pCMapEntry->GetCopyPlyNum());
		CCopyCell.SetPosID(copyIdx);
		pCCopyCell = pCMapEntry->AddCopy(&CCopyCell);
	}
	return pCCopyCell;
}

int GetMapCopyPlayerNum(SubMap* pCMapCopy) {
	if (!pCMapCopy) return 0;
	return pCMapCopy->GetPlayerNum();
}

void BeginGetMapCopyPlayerCha(SubMap* pCMapCopy) {
	if (!pCMapCopy) return;
	pCMapCopy->BeginGetPlyCha();
}

CCharacter* GetMapCopyNextPlayerCha(SubMap* pCMapCopy) {
	if (!pCMapCopy) return nullptr;
	return pCMapCopy->GetNextPlyCha();
}

int GetChaMapType(CCharacter* pCCha) {
	if (!pCCha) return 0;
	char chMapType = enumMAPTYPE_NORMAL;
	CMapRes* pCMap = nullptr;
	if (pCCha->GetSubMap())
		pCMap = pCCha->GetSubMap()->GetMapRes();
	if (!pCMap)
		pCMap = g_pGameApp->FindMapByName(pCCha->GetBirthMap(), true);
	if (pCMap)
		chMapType = pCMap->GetType();
	return chMapType;
}

void SetChaKitbagChange(CCharacter* pCCha, int flag) {
	if (!pCCha) return;
	pCCha->m_CKitbag.SetChangeFlag(flag != 0);
}

void SynChaKitbag(CCharacter* pCCha, int synType) {
	if (!pCCha) return;
	pCCha->SynKitbagNew((char)synType);
}

int GetChaMapOpenScale(CCharacter* pCCha) {
	if (!pCCha) return 0;
	if (!pCCha->IsPlayerCha() || !pCCha->GetSubMap()) return 0;
	return (int)pCCha->GetPlayer()->GetMapMaskOpenScale(pCCha->GetSubMap()->GetName());
}

void FinishSetMapEntryCopy(CDynMapEntryCell* pCMapEntryCell, int copyNO) {
	if (!pCMapEntryCell) return;
	pCMapEntryCell->SynCopyParam((short)copyNO - 1);
}

int GetItemType(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	CItemRecord* pCItemRec = GetItemRecordInfo(pSItem->sID);
	if (!pCItemRec) return 0;
	return pCItemRec->sType;
}

int GetItemType2(int sItemID) {
	CItemRecord* pCItemRec = GetItemRecordInfo((short)sItemID);
	if (!pCItemRec) return 0;
	return pCItemRec->sType;
}

int GetItemLv(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	return pSItem->sNeedLv;
}

int GetItemOriginalLv(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	CItemRecord* pCItemRec = GetItemRecordInfo(pSItem->sID);
	if (!pCItemRec) return 0;
	return pCItemRec->sNeedLv;
}

void SetItemLv(SItemGrid* pSItem, int nItemLv) {
	if (!pSItem) return;
	pSItem->sNeedLv = (short)nItemLv;
}

int GetItemLv2(int sItemID) {
	CItemRecord* pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec) return 0;
	return pCItemRec->sNeedLv;
}

int GetItemID(SItemGrid* pSItem) {
	if (!pSItem) return 0;
	return pSItem->sID;
}

int GetItemHoleNum(int sItemID) {
	CItemRecord* pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec) return 0;
	return pCItemRec->sHole;
}

int SetChaEquipValid(CCharacter* pCCha, int chEquipPos, int bValid) {
	if (!pCCha) return 0;
	if (pCCha->IsReadBook()) return 0;
	return pCCha->SetEquipValid((char)chEquipPos, bValid != 0) ? 1 : 0;
}

int SetChaKbItemValid(CCharacter* pCCha, int chKbPos, int bValid, int bSyn) {
	if (!pCCha) return 0;
	return pCCha->SetKitbagItemValid((char)chKbPos, bValid != 0, true, bSyn != 0) ? 1 : 0;
}

int SetChaKbItemValid2(CCharacter* pCCha, SItemGrid* pSItem, int bValid, int bSyn) {
	if (!pCCha) return 0;
	return pCCha->SetKitbagItemValid(pSItem, bValid != 0, true, bSyn != 0) ? 1 : 0;
}

// GetChallengeGuildID returns 2 values
int GetChallengeGuildID_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 1) return 0;

	DWORD dwHostID = 0, dwReqID = 0, dwMoney;
	if (!game_db.GetChall((BYTE)lua_tonumber(pLS, 1), dwHostID, dwReqID, dwMoney))
		return 0;

	lua_pushnumber(pLS, dwHostID);
	lua_pushnumber(pLS, dwReqID);
	return 2;
}

void EndGuildBid(int byLevel) {
	if (!game_db.HasChall((BYTE)byLevel)) {
		game_db.StartChall((BYTE)byLevel);
	}
}

void EndGuildChallenge(unsigned int param1, unsigned int param2, unsigned int param3) {
	game_db.EndChall((DWORD)param1, (DWORD)param2, param3 ? TRUE : FALSE);
}

int AddKbCap(CCharacter* pCCha, int cap) {
	if (!pCCha) return 0;
	if (!pCCha->AddKitbagCapacity((short)cap)) return 0;
	return 1;
}

int GetKbCap(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->m_CKitbag.GetCapacity();
}

int IsInSameMap(CCharacter* pCCha1, CCharacter* pCCha2) {
	if (!pCCha1 || !pCCha2) return 0;
	if (pCCha1->GetSubMap() && pCCha2->GetSubMap()) {
		if (pCCha1->GetSubMap()->GetMapID() == pCCha2->GetSubMap()->GetMapID())
			return 1;
	}
	return 0;
}

int IsInSameMapCopy(CCharacter* pCCha1, CCharacter* pCCha2) {
	if (!pCCha1 || !pCCha2) return 0;
	if (pCCha1->GetSubMap() && pCCha2->GetSubMap()) {
		if (pCCha1->GetSubMap() == pCCha2->GetSubMap())
			return 1;
	}
	return 0;
}

int IsChaLiving(CCharacter* pCCha) {
	if (!pCCha) return 0;
	return pCCha->IsLiveing() ? 1 : 0;
}

int SetChaParam(CCharacter* pCCha, int paramIdx, int val) {
	if (!pCCha) return 0;
	if (!pCCha->SetScriptParam((char)paramIdx - 1, (long)val)) return 0;
	return 1;
}

int GetChaParam(CCharacter* pCCha, int paramIdx) {
	if (!pCCha) return 0;
	return pCCha->GetScriptParam((char)paramIdx - 1);
}

int AddItemEffect(CCharacter* pCCha, SItemGrid* pItem, int bValid) {
	if (!pCCha || !pItem) return 0;
	pCCha->ChangeItem(bValid != 0, pItem, enumEQUIP_HEAD);
	return 1;
}

// Lua needs lua_State* for luaL_dostring
int Lua_raw(lua_State* pLS) {
	if (!lua_isstring(pLS, 1))
		return 0;
	luaL_dostring(pLS, lua_tostring(pLS, 1));
	return 1;
}

// LuaAll needs lua_State* for lua_tostring
int LuaAll_raw(lua_State* pLS) {
	if (!lua_isstring(pLS, 1))
		return 0;

	net::WPacket WtPk = g_gmsvr->GetWPacket();
	WtPk.WriteCmd(CMD_MM_DO_STRING);
	WtPk.WriteInt64(0);
	WtPk.WriteString(lua_tostring(pLS, 1));

	BEGINGETGATE();
	GateServer* pGateServer = NULL;
	while (pGateServer = GETNEXTGATE()) {
		pGateServer->SendData(WtPk);
		break;
	}
	return 1;
}

// ReloadCal needs lua_State* for luaL_dofile
int ReloadCal_raw(lua_State* L) {
	luaL_dofile(L, GetResPath("script\\calculate\\skilleffect.lua"));
	return 1;
}

void ReloadCal() {
	ReloadCal_raw(g_pLuaState);
}

// GetWinLotteryItemno has dynamic return behavior
int GetWinLotteryItemno_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 2) return 0;

	int issue = (int)lua_tonumber(pLS, 1);
	int index = (int)lua_tonumber(pLS, 2);

	if (index >= 0 && index < 7) {
		std::string itemno;
		if (game_db.GetWinItemno(issue, itemno)) {
			if (index == 0)
				lua_pushstring(pLS, itemno.c_str());
			else
				lua_pushstring(pLS, itemno.substr(index - 1, 1).c_str());
			return 1;
		}
	}
	return 0;
}

// CalWinLottery has dynamic return behavior
int CalWinLottery_raw(lua_State* pLS) {
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 2) return 0;

	int issue = (int)lua_tonumber(pLS, 1);
	int index = (int)lua_tonumber(pLS, 2);

	if (index >= 0 && index < 7) {
		size_t i = (size_t)index;
		std::string itemno;
		if (game_db.GetWinItemno(issue, itemno)) {
			if (index == 0)
				lua_pushstring(pLS, itemno.c_str());
			else {
				if (itemno.size() >= i)
					lua_pushstring(pLS, itemno.substr(index - 1, 1).c_str());
				else
					return 0;
			}
			return 1;
		}
	}
	return 0;
}

int GetLotteryIssue_raw(lua_State* pLS) {
	int issue;
	if (game_db.GetLotteryIssue(issue)) {
		lua_pushnumber(pLS, (long)issue);
		return 1;
	}
	return 0;
}

void AddLotteryIssue(int issue) {
	game_db.AddIssue(issue);
}

void DisuseLotteryIssue(int issue, int state) {
	game_db.DisuseIssue(issue, state);
}

// IsValidRegTeam uses 4 userdata params
int IsValidRegTeam_raw(lua_State* pLS) {
	int teamID = (int)lua_tonumber(pLS, 1);
	auto pCaptainResult = luabridge::Stack<CCharacter*>::get(pLS, 2);
	if (!pCaptainResult) { PARAM_ERROR lua_pushnumber(pLS, (long)0); return 1; }
	CCharacter* pCaptain = *pCaptainResult;
	auto pMember1Result = luabridge::Stack<CCharacter*>::get(pLS, 3);
	if (!pMember1Result) { PARAM_ERROR lua_pushnumber(pLS, (long)0); return 1; }
	CCharacter* pMember1 = *pMember1Result;
	auto pMember2Result = luabridge::Stack<CCharacter*>::get(pLS, 4);
	if (!pMember2Result) { PARAM_ERROR lua_pushnumber(pLS, (long)0); return 1; }
	CCharacter* pMember2 = *pMember2Result;

	if (game_db.IsValidAmphitheaterTeam(teamID, pCaptain->GetID(), pMember1->GetID(), pMember2->GetID()))
		lua_pushnumber(pLS, (long)1);
	else
		lua_pushnumber(pLS, (long)0);
	return 1;
}

// IsValidTeam has complex multi-return logic
int IsValidTeam_raw(lua_State* pLS) {
	auto pCChaResult = luabridge::Stack<CCharacter*>::get(pLS, 1);
	if (!pCChaResult) { PARAM_ERROR lua_pushnumber(pLS, (long)-4); return 1; }
	CCharacter* pCCha = *pCChaResult;
	CPlayer* pTeamPlayer = pCCha->GetPlayer();
	int masterID = pTeamPlayer->GetDBChaId();

	if (pTeamPlayer == NULL) {
		lua_pushnumber(pLS, (long)-4);
		return 1;
	}

	if (!pTeamPlayer->IsTeamLeader()) {
		lua_pushnumber(pLS, (long)-1);
		return 1;
	}

	if (pTeamPlayer->GetTeamMemberCnt() != 2) {
		lua_pushnumber(pLS, (long)-2);
		return 1;
	}

	DWORD prenticeID1 = pTeamPlayer->GetTeamMemberDBID(0);
	DWORD prenticeID2 = pTeamPlayer->GetTeamMemberDBID(1);

	if (game_db.IsMasterRelation(masterID, prenticeID1) && game_db.IsMasterRelation(masterID, prenticeID2)) {
		lua_pushnumber(pLS, (long)1);
		return 1;
	}

	lua_pushnumber(pLS, (long)-3);
	return 1;
}

int GetAmphitheaterSeason_raw(lua_State* pLS) {
	int season = -1;
	int round = -1;
	if (game_db.GetAmphitheaterSeasonAndRound(season, round)) {
		lua_pushnumber(pLS, (long)season);
		return 1;
	}
	else {
		lua_pushnumber(pLS, (long)0);
		return 0;
	}
}

int GetAmphitheaterRound_raw(lua_State* pLS) {
	int season = -1;
	int round = -1;
	if (game_db.GetAmphitheaterSeasonAndRound(season, round)) {
		lua_pushnumber(pLS, (long)round);
		return 1;
	}
	else {
		lua_pushnumber(pLS, (long)0);
		return 1;
	}
}

void AddAmphitheaterSeason(int season) {
	game_db.AddAmphitheaterSeason(season);
}

int DisuseAmphitheaterSeason(int season, int state, const std::string& winner) {
	if (game_db.DisuseAmphitheaterSeason(season, state, winner.c_str()))
		return 1;
	return 0;
}

int UpdateAmphitheaterRound(int season, int round) {
	if (game_db.UpdateAmphitheaterRound(season, round))
		return 1;
	return 0;
}

int GetAmphitheaterTeamCount_raw(lua_State* pLS) {
	int count = 0;
	if (game_db.GetAmphitheaterTeamCount(count)) {
		lua_pushnumber(pLS, (long)count);
		return 1;
	}
	lua_pushnumber(pLS, (long)0);
	return 0;
}

int GetAmphitheaterNoUseTeamID_raw(lua_State* pLS) {
	int teamID = 0;
	if (game_db.GetAmphitheaterNoUseTeamID(teamID)) {
		lua_pushnumber(pLS, (long)teamID);
		return 1;
	}
	lua_pushnumber(pLS, (long)0);
	return 1;
}

// AmphitheaterTeamSignUP uses multiple userdata
int AmphitheaterTeamSignUP_raw(lua_State* pLS) {
	int teamID = (int)lua_tonumber(pLS, 1);
	auto pCaptainResult = luabridge::Stack<CCharacter*>::get(pLS, 2);
	if (!pCaptainResult) { PARAM_ERROR return 0; }
	CCharacter* pCaptain = *pCaptainResult;
	auto pMember1Result = luabridge::Stack<CCharacter*>::get(pLS, 3);
	if (!pMember1Result) { PARAM_ERROR return 0; }
	CCharacter* pMember1 = *pMember1Result;
	auto pMember2Result = luabridge::Stack<CCharacter*>::get(pLS, 4);
	if (!pMember2Result) { PARAM_ERROR return 0; }
	CCharacter* pMember2 = *pMember2Result;

	if (game_db.AmphitheaterTeamSignUP(teamID, pCaptain->GetPlayer()->GetDBChaId(), pMember1->GetPlayer()->GetDBChaId(),
									   pMember2->GetPlayer()->GetDBChaId()))
		return 1;
	return 0;
}

int AmphitheaterTeamCancel(int teamID) {
	if (game_db.AmphitheaterTeamCancel(teamID))
		return 1;
	return 0;
}

int IsAmphitheaterLogin(CCharacter* pActor) {
	if (!pActor) return 1;
	if (game_db.IsAmphitheaterLogin(pActor->GetID()))
		return 0;
	return 1;
}

int IsMapFull_raw(lua_State* pLS) {
	int PActorIDNum = 0;
	int MapID = (int)lua_tonumber(pLS, 1);
	if (game_db.IsMapFull(MapID, PActorIDNum)) {
		lua_pushnumber(pLS, PActorIDNum);
		return 1;
	}
	return 0;
}

int UpdateMapAfterEnter(CCharacter* Captain, int MapID) {
	if (!Captain) return 0;
	int CaptainID = Captain->GetID();
	if (game_db.UpdateMapAfterEnter(CaptainID, MapID))
		return 1;
	return 0;
}

int UpdateMap(int Mapid) {
	if (game_db.UpdateMap(Mapid))
		return 1;
	return 0;
}

int UpdateMapNum(int Teamid, int Mapid, int MapFlag) {
	if (game_db.UpdateMapNum(Teamid, Mapid, MapFlag))
		return 1;
	return 0;
}

int GetMapFlag_raw(lua_State* pLS) {
	int Mapflag = 0;
	int Teamid = (int)lua_tonumber(pLS, 1);
	if (game_db.GetMapFlag(Teamid, Mapflag)) {
		lua_pushnumber(pLS, Mapflag);
		return 1;
	}
	return 0;
}

int SetMaxBallotTeamRelive_raw(lua_State* pLS) {
	if (game_db.SetMaxBallotTeamRelive())
		return 1;
	return 0;
}

int SetMatchResult(int Teamid1, int Teamid2, int Id1state, int Id2state) {
	if (game_db.SetMatchResult(Teamid1, Teamid2, Id1state, Id2state))
		return 1;
	return 0;
}

// GetCaptainByMapId returns 2 lightuserdata
int GetCaptainByMapId_raw(lua_State* pLS) {
	int Mapid = (int)lua_tonumber(pLS, 1);
	std::string Captainid1;
	std::string Captainid2;
	DWORD Capid1 = 0;
	DWORD Capid2 = 0;
	CCharacter* pCCha1 = NULL;
	CCharacter* pCCha2 = NULL;
	CPlayer* player1 = NULL;
	CPlayer* player2 = NULL;
	const char* nocaptain = "";
	if (game_db.GetCaptainByMapId(Mapid, Captainid1, Captainid2)) {
		if (strcmp(nocaptain, Captainid2.c_str()) == 0) {
			Capid1 = atoi(Captainid1.c_str());
			Capid2 = 0;
		}
		else {
			Capid1 = atoi(Captainid1.c_str());
			Capid2 = atoi(Captainid2.c_str());
		}

		player1 = g_pGameApp->GetPlayerByDBID(Capid1);
		player2 = g_pGameApp->GetPlayerByDBID(Capid2);

		if (player2 == NULL) {
			pCCha1 = player1->GetMainCha();
			if (pCCha1 == NULL)
				return 0;
		}
		else {
			if (player1 == NULL)
				return 0;
			pCCha1 = player1->GetMainCha();
			pCCha2 = player2->GetMainCha();
			if (pCCha1 == NULL || pCCha2 == NULL)
				return 0;
		}
		luabridge::push(pLS, static_cast<CCharacter*>(pCCha1));
		luabridge::push(pLS, static_cast<CCharacter*>(pCCha2));
		return 2;
	}
	return 0;
}

int UpdateAbsentTeamRelive_raw(lua_State* pLS) {
	if (game_db.UpdateAbsentTeamRelive())
		return 1;
	return 0;
}

int UpdateWinnum(int teamid) {
	if (game_db.UpdateWinnum(teamid))
		return 1;
	return 0;
}

int GetUniqueMaxWinnum_raw(lua_State* pLS) {
	int teamid = 0;
	if (game_db.GetUniqueMaxWinnum(teamid)) {
		lua_pushnumber(pLS, teamid);
		return 1;
	}
	return 0;
}

int SetMatchnoState(int teamid) {
	if (game_db.SetMatchnoState(teamid))
		return 1;
	return 0;
}

int UpdateState_raw(lua_State* pLS) {
	if (game_db.UpdateState())
		return 1;
	return 0;
}

int CloseReliveByState_raw(lua_State* pLS) {
	int statenum = 0;
	if (game_db.CloseReliveByState(statenum)) {
		lua_pushnumber(pLS, statenum);
		return 1;
	}
	return 0;
}

int CleanMapFlag(int teamid1, int teamid2) {
	if (game_db.CleanMapFlag(teamid1, teamid2))
		return 1;
	return 0;
}

int GetStateByTeamid_raw(lua_State* pLS) {
	int teamid = (int)lua_tonumber(pLS, 1);
	int state = 0;
	if (game_db.GetStateByTeamid(teamid, state)) {
		lua_pushnumber(pLS, state);
		return 1;
	}
	return 0;
}

void LookEnergy(CCharacter* pCCha) {
	if (!pCCha) return;
	pCCha->SynLookEnergy();
}

// SetExpiration uses lua_touserdata for the second param (long* cast)
int SetExpiration_raw(lua_State* L) {
	int nParaNum = lua_gettop(L);
	if (nParaNum != 2) {
		lua_pushnumber(L, 0);
		return 1;
	}

	auto pSItemResult = luabridge::Stack<SItemGrid*>::get(L, 1);
	if (!pSItemResult) { PARAM_ERROR lua_pushnumber(L, 0); return 1; }
	SItemGrid* pSItem = *pSItemResult;
	long expiration = *static_cast<long*>(lua_touserdata(L, 2));
	if (expiration == -1 || expiration == 0)
		pSItem->expiration = 0;
	else
		pSItem->expiration = std::time(0) + expiration;

	lua_pushnumber(L, 1);
	return 1;
}

void RegisterLuaGameLogic(lua_State* L) {
	lua_register(L, "GetSkillPos", GetSkillPos_raw);

	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(SetChaAttr)
		LUABRIDGE_REGISTER_FUNC(CheckChaRole)
		LUABRIDGE_REGISTER_FUNC(SetRangeState)
		LUABRIDGE_REGISTER_FUNC(GetSkillLv)
		LUABRIDGE_REGISTER_FUNC(GetChaStateLv)
		LUABRIDGE_REGISTER_FUNC(GetObjDire)
		LUABRIDGE_REGISTER_FUNC(GetChaName)
		LUABRIDGE_REGISTER_FUNC(AddState)
		LUABRIDGE_REGISTER_FUNC(RemoveState)
		LUABRIDGE_REGISTER_FUNC(GetAreaStateLevel)
		LUABRIDGE_REGISTER_FUNC(SkillMiss)
		LUABRIDGE_REGISTER_FUNC(SkillCrt)
		LUABRIDGE_REGISTER_FUNC(SkillUnable)
		LUABRIDGE_REGISTER_FUNC(UseItemFailed)
		LUABRIDGE_REGISTER_FUNC(BeatBack)
		LUABRIDGE_REGISTER_FUNC(IsInGymkhana)
		LUABRIDGE_REGISTER_FUNC(IsInPK)
		LUABRIDGE_REGISTER_FUNC(CheckBagItem)
		LUABRIDGE_REGISTER_FUNC(GetChaFreeBagGridNum)
		LUABRIDGE_REGISTER_FUNC(DelBagItem)
		LUABRIDGE_REGISTER_FUNC(DelBagItem2)
		LUABRIDGE_REGISTER_FUNC(GetChaMapName)
		LUABRIDGE_REGISTER_FUNC(GetChaMapCopyNO)
		LUABRIDGE_REGISTER_FUNC(GetChaMapCopy)
		LUABRIDGE_REGISTER_FUNC(GetMainCha)
		LUABRIDGE_REGISTER_FUNC(GetCtrlBoat)
		LUABRIDGE_REGISTER_FUNC(ChaIsBoat)
		LUABRIDGE_REGISTER_FUNC(GetChaItem)
		LUABRIDGE_REGISTER_FUNC(GetChaItem2)
		LUABRIDGE_REGISTER_FUNC(MoveToTemp)
		LUABRIDGE_REGISTER_FUNC(GetItemFinalAttr)
		LUABRIDGE_REGISTER_FUNC(SetItemFinalAttr)
		LUABRIDGE_REGISTER_FUNC(AddItemFinalAttr)
		LUABRIDGE_REGISTER_FUNC(ResetItemFinalAttr)
		LUABRIDGE_REGISTER_FUNC(GetItemAttrRange)
		LUABRIDGE_REGISTER_FUNC(GetItemForgeParam)
		LUABRIDGE_REGISTER_FUNC(AddEquipEnergy)
		LUABRIDGE_REGISTER_FUNC(SetRelive)
		LUABRIDGE_REGISTER_FUNC(LuaPrint)
		LUABRIDGE_REGISTER_FUNC(Stop)
		LUABRIDGE_REGISTER_FUNC(Notice)
		LUABRIDGE_REGISTER_FUNC(GuildNotice)
		LUABRIDGE_REGISTER_FUNC(ChaNotice)
		LUABRIDGE_REGISTER_FUNC(MapChaLight)
		LUABRIDGE_REGISTER_FUNC(SetItemHost)
		LUABRIDGE_REGISTER_FUNC(MapCanSavePos)
		LUABRIDGE_REGISTER_FUNC(MapCanPK)
		LUABRIDGE_REGISTER_FUNC(MapCanTeam)
		LUABRIDGE_REGISTER_FUNC(MapCopyNum)
		LUABRIDGE_REGISTER_FUNC(MapCopyStartType)
		LUABRIDGE_REGISTER_FUNC(MapType)
		LUABRIDGE_REGISTER_FUNC(SingleMapCopyPlyNum)
		LUABRIDGE_REGISTER_FUNC(SetMapEntryMapName)
		LUABRIDGE_REGISTER_FUNC(SetMapEntryEntiID)
		LUABRIDGE_REGISTER_FUNC(SetMapEntryEventName)
		LUABRIDGE_REGISTER_FUNC(CallMapEntry)
		LUABRIDGE_REGISTER_FUNC(GetChaSideID)
		LUABRIDGE_REGISTER_FUNC(SetChaSideID)
		LUABRIDGE_REGISTER_FUNC(GetChaGuildID)
		LUABRIDGE_REGISTER_FUNC(GetChaTeamID)
		LUABRIDGE_REGISTER_FUNC(CheckChaPKState)
		LUABRIDGE_REGISTER_FUNC(GetChaPlayer)
		LUABRIDGE_REGISTER_FUNC(GetPlayerTeamID)
		LUABRIDGE_REGISTER_FUNC(GetPlayerID)
		LUABRIDGE_REGISTER_FUNC(GetGuildName)
		LUABRIDGE_REGISTER_FUNC(CloseMapEntry)
		LUABRIDGE_REGISTER_FUNC(SetChaMotto)
		LUABRIDGE_REGISTER_FUNC(IsChaInLand)
		LUABRIDGE_REGISTER_FUNC(SetTeamFightMapName)
		LUABRIDGE_REGISTER_FUNC(RepatriateDie)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyParam)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyParam2)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyID)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyID2)
		LUABRIDGE_REGISTER_FUNC(GetMapEntryCopyObj)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyPlayerNum)
		LUABRIDGE_REGISTER_FUNC(BeginGetMapCopyPlayerCha)
		LUABRIDGE_REGISTER_FUNC(GetMapCopyNextPlayerCha)
		LUABRIDGE_REGISTER_FUNC(GetChaMapType)
		LUABRIDGE_REGISTER_FUNC(SetChaKitbagChange)
		LUABRIDGE_REGISTER_FUNC(SynChaKitbag)
		LUABRIDGE_REGISTER_FUNC(MapCopyNotice)
		LUABRIDGE_REGISTER_FUNC(MapCopyNotice2)
		LUABRIDGE_REGISTER_FUNC(GetChaMapOpenScale)
		LUABRIDGE_REGISTER_FUNC(FinishSetMapEntryCopy)
		LUABRIDGE_REGISTER_FUNC(GetItemType)
		LUABRIDGE_REGISTER_FUNC(GetItemType2)
		LUABRIDGE_REGISTER_FUNC(GetItemLv)
		LUABRIDGE_REGISTER_FUNC(GetItemOriginalLv)
		LUABRIDGE_REGISTER_FUNC(SetItemLv)
		LUABRIDGE_REGISTER_FUNC(GetItemLv2)
		LUABRIDGE_REGISTER_FUNC(GetItemID)
		LUABRIDGE_REGISTER_FUNC(GetItemHoleNum)
		LUABRIDGE_REGISTER_FUNC(SetChaEquipValid)
		LUABRIDGE_REGISTER_FUNC(SetChaKbItemValid)
		LUABRIDGE_REGISTER_FUNC(SetChaKbItemValid2)
		LUABRIDGE_REGISTER_FUNC(AddKbCap)
		LUABRIDGE_REGISTER_FUNC(GetKbCap)
		LUABRIDGE_REGISTER_FUNC(IsInSameMap)
		LUABRIDGE_REGISTER_FUNC(IsInSameMapCopy)
		LUABRIDGE_REGISTER_FUNC(IsChaLiving)
		LUABRIDGE_REGISTER_FUNC(SetChaParam)
		LUABRIDGE_REGISTER_FUNC(GetChaParam)
		LUABRIDGE_REGISTER_FUNC(AddItemEffect)
		LUABRIDGE_REGISTER_FUNC(MapCanStall)
		LUABRIDGE_REGISTER_FUNC(MapCanGuild)
		LUABRIDGE_REGISTER_FUNC(KillMyMonster)
		LUABRIDGE_REGISTER_FUNC(KillMonsterInMapByName)
		LUABRIDGE_REGISTER_FUNC(AddLotteryIssue)
		LUABRIDGE_REGISTER_FUNC(DisuseLotteryIssue)
		LUABRIDGE_REGISTER_FUNC(AddAmphitheaterSeason)
		LUABRIDGE_REGISTER_FUNC(DisuseAmphitheaterSeason)
		LUABRIDGE_REGISTER_FUNC(UpdateAmphitheaterRound)
		LUABRIDGE_REGISTER_FUNC(AmphitheaterTeamCancel)
		LUABRIDGE_REGISTER_FUNC(IsAmphitheaterLogin)
		LUABRIDGE_REGISTER_FUNC(UpdateMapAfterEnter)
		LUABRIDGE_REGISTER_FUNC(UpdateMap)
		LUABRIDGE_REGISTER_FUNC(UpdateMapNum)
		LUABRIDGE_REGISTER_FUNC(SetMatchResult)
		LUABRIDGE_REGISTER_FUNC(UpdateWinnum)
		LUABRIDGE_REGISTER_FUNC(SetMatchnoState)
		LUABRIDGE_REGISTER_FUNC(CleanMapFlag)
		LUABRIDGE_REGISTER_FUNC(ScrollNotice)
		LUABRIDGE_REGISTER_FUNC(GMNotice)
		LUABRIDGE_REGISTER_FUNC(GetChaGuildPermission)
		LUABRIDGE_REGISTER_FUNC(GetItemStackSize)
		LUABRIDGE_REGISTER_FUNC(IsItemLocked)
		LUABRIDGE_REGISTER_FUNC(EquipItem)
		LUABRIDGE_REGISTER_FUNC(SynLook)
		LUABRIDGE_REGISTER_FUNC(EquipStringItem)
		LUABRIDGE_REGISTER_FUNC(IsAttributeEditable)
		LUABRIDGE_REGISTER_FUNC(SetAttributeEditable)
		LUABRIDGE_REGISTER_FUNC(GetChaFreeTempBagGridNum)
		LUABRIDGE_REGISTER_FUNC(GetIMP)
		LUABRIDGE_REGISTER_FUNC(LookEnergy)
		LUABRIDGE_REGISTER_FUNC(EndGuildBid)
		LUABRIDGE_REGISTER_FUNC(EndGuildChallenge);

	// Raw lua_CFunction registrations (variable args / dynamic type checking / need lua_State*)
	lua_register(L, "GetChaAttr", GetChaAttr_raw);
	lua_register(L, "SetIMP", SetIMP_raw);
	lua_register(L, "SetSkillRange", SetSkillRange_raw);
	lua_register(L, "AddChaSkill", AddChaSkill_raw);
	lua_register(L, "SetItemFall", SetItemFall_raw);
	lua_register(L, "RemoveChaItem", RemoveChaItem_raw);
	lua_register(L, "GetItemAttr", GetItemAttr_raw);
	lua_register(L, "SetItemAttr", SetItemAttr_raw);
	lua_register(L, "AddItemAttr", AddItemAttr_raw);
	lua_register(L, "SetItemForgeParam", SetItemForgeParam_raw);
	lua_register(L, "SetMapEntryTime", SetMapEntryTime_raw);
	lua_register(L, "GetMapEntryPosInfo", GetMapEntryPosInfo_raw);
	lua_register(L, "CloseMapCopy", CloseMapCopy_raw);
	lua_register(L, "GetChallengeGuildID", GetChallengeGuildID_raw);
	lua_register(L, "Lua", Lua_raw);
	lua_register(L, "LuaAll", LuaAll_raw);
	lua_register(L, "ReloadCal", ReloadCal_raw);
	lua_register(L, "GetLotteryIssue", GetLotteryIssue_raw);
	lua_register(L, "GetWinLotteryItemno", GetWinLotteryItemno_raw);
	lua_register(L, "CalWinLottery", CalWinLottery_raw);
	lua_register(L, "IsValidRegTeam", IsValidRegTeam_raw);
	lua_register(L, "IsValidTeam", IsValidTeam_raw);
	lua_register(L, "GetAmphitheaterSeason", GetAmphitheaterSeason_raw);
	lua_register(L, "GetAmphitheaterRound", GetAmphitheaterRound_raw);
	lua_register(L, "GetAmphitheaterTeamCount", GetAmphitheaterTeamCount_raw);
	lua_register(L, "GetAmphitheaterNoUseTeamID", GetAmphitheaterNoUseTeamID_raw);
	lua_register(L, "AmphitheaterTeamSignUP", AmphitheaterTeamSignUP_raw);
	lua_register(L, "IsMapFull", IsMapFull_raw);
	lua_register(L, "GetMapFlag", GetMapFlag_raw);
	lua_register(L, "SetMaxBallotTeamRelive", SetMaxBallotTeamRelive_raw);
	lua_register(L, "GetCaptainByMapId", GetCaptainByMapId_raw);
	lua_register(L, "UpdateAbsentTeamRelive", UpdateAbsentTeamRelive_raw);
	lua_register(L, "GetUniqueMaxWinnum", GetUniqueMaxWinnum_raw);
	lua_register(L, "UpdateState", UpdateState_raw);
	lua_register(L, "CloseReliveByState", CloseReliveByState_raw);
	lua_register(L, "GetStateByTeamid", GetStateByTeamid_raw);
	lua_register(L, "SetExpiration", SetExpiration_raw);
}
