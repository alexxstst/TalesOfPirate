// CharScript.cpp Created by knight-gongjian 2004.12.13.
//---------------------------------------------------------
#include "stdafx.h"//add by alfred.shi 20080202
#include "CharScript.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include "Character.h"
#include <assert.h>
#include "lua_gamectrl.h"
#include "Expand.h"
#include "LuaAPI.h"
#include "CharBoat.h"
#include "Guild.h"
#include "Auction.h"

using namespace std;


//---------------------------------------------------------
_DBC_USING
using namespace mission;

// Add by lark.li 20080310 begin
std::string GetResString(const std::string& pszID)
{
	const char* text = const_cast<char*>(g_ResourceBundleManage.LoadResString(pszID.c_str()));
	return std::string(text ? text : "");
}
// End

int SetMap(const std::string& pszMap, int byID)
{
	BOOL bRet = g_MapID.AddInfo(pszMap.c_str(), (BYTE)byID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

void SetMapGuildWar(CMapRes* pCMap, int nGuildWar)
{
	if (!pCMap) return;
	pCMap->SetGuildWar(nGuildWar > 0);
}

int AddTrigger(CCharacter* pChar, int wMissionID, int wTriggerID, int byType, int wParam1, int wParam2, int wParam3, int wParam4, int wParam5, int wParam6)
{
	if (!pChar) return LUA_FALSE;

	TRIGGER_DATA Data;
	memset(&Data, 0, sizeof(TRIGGER_DATA));
	Data.wMissionID = (WORD)wMissionID;
	Data.wTriggerID = (WORD)wTriggerID;
	Data.byType     = (BYTE)byType;
	Data.wParam1    = (WORD)wParam1;
	Data.wParam2    = (WORD)wParam2;
	Data.wParam3    = (WORD)wParam3;
	Data.wParam4    = (WORD)wParam4;
	Data.wParam5    = (WORD)wParam5;
	Data.wParam6    = (WORD)wParam6;

	BOOL bRet = pChar->AddTrigger(Data);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearTrigger(CCharacter* pChar, int wTriggerID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearTrigger((WORD)wTriggerID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int DeleteTrigger(CCharacter* pChar, int wTriggerID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->DeleteTrigger((WORD)wTriggerID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AddMission(CCharacter* pChar, int wID, int wParam)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->AddRole((WORD)wID, (WORD)wParam);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasMission(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasRole((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearMission(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearRole((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetMisScriptID(CCharacter* pChar, int wID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	WORD wScriptID;
	BOOL bRet = pChar->GetMisScriptID((WORD)wID, wScriptID);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)wScriptID);
}

int SetMissionComplete(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetMissionComplete((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetMissionFailure(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetMissionFailure((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasMisssionFailure(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasMissionFailure((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsMissionFull(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsRoleFull();
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetFlag(CCharacter* pChar, int wID, int wFlag)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetFlag((WORD)wID, (WORD)wFlag);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearFlag(CCharacter* pChar, int wID, int wFlag)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearFlag((WORD)wID, (WORD)wFlag);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsFlag(CCharacter* pChar, int wID, int wFlag)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsFlag((WORD)wID, (WORD)wFlag);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsValidFlag(CCharacter* pChar, int wFlag)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsValidFlag((WORD)wFlag);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetRecord(CCharacter* pChar, int wRec)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetRecord((WORD)wRec);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearRecord(CCharacter* pChar, int wRec)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearRecord((WORD)wRec);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsRecord(CCharacter* pChar, int wRec)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsRecord((WORD)wRec);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsValidRecord(CCharacter* pChar, int wRec)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsValidRecord((WORD)wRec);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AddSkill(CCharacter* pChar, CTalkNpc* pTalk, int wSkillID, int byLevel)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	if (pChar->GetPlayer())
		pChar = pChar->GetPlayer()->GetMainCha();

	BOOL bRet = pChar->GetPlyMainCha()->LearnSkill((WORD)wSkillID, (BYTE)byLevel);

	if (bRet)
	{
		char szSkill[defENTITY_NAME_LEN];
		strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00002), defENTITY_NAME_LEN - 1);

		CSkillRecord* pSkill = GetSkillRecordInfo((WORD)wSkillID);
		if (pSkill)
		{
			strncpy(szSkill, pSkill->szName.c_str(), defSKILL_NAME_LEN - 1);
		}

		char szData[128];
		sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00003), szNpc, szSkill);
		pChar->SystemNotice(szData);
		ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);
	}

	return LUA_TRUE;
}

int AddLevel()
{
	return 0;
}

int AddSailExp(CCharacter* pChar, CTalkNpc* pTalk, int dwMinExp, int dwMaxExp)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	DWORD dwValue;
	if ((DWORD)dwMinExp >= (DWORD)dwMaxExp)
	{
		dwValue = (DWORD)dwMinExp;
	}
	else
	{
		dwValue = rand() % ((DWORD)dwMaxExp - (DWORD)dwMinExp) + (DWORD)dwMinExp;
	}

	pChar->GetPlyMainCha()->AddAttr(ATTR_CSAILEXP, dwValue);
	char szData[128];
	sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00004), szNpc, dwValue);
	pChar->SystemNotice(szData);
	ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);

	return LUA_TRUE;
}

int AddLifeExp(CCharacter* pChar, CTalkNpc* pTalk, int dwMinExp, int dwMaxExp)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	DWORD dwValue;
	if ((DWORD)dwMinExp >= (DWORD)dwMaxExp)
	{
		dwValue = (DWORD)dwMinExp;
	}
	else
	{
		dwValue = rand() % ((DWORD)dwMaxExp - (DWORD)dwMinExp) + (DWORD)dwMinExp;
	}

	pChar->GetPlyMainCha()->AddAttr(ATTR_CLIFEEXP, dwValue);
	char szData[128];
	sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00005), szNpc, dwValue);
	pChar->SystemNotice(szData);
	ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);

	return LUA_TRUE;
}

int AddExp(CCharacter* pChar, CTalkNpc* pTalk, int dwMinExp, int dwMaxExp)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	DWORD dwValue;
	if ((DWORD)dwMinExp >= (DWORD)dwMaxExp)
	{
		dwValue = (DWORD)dwMinExp;
	}
	else
	{
		dwValue = rand() % ((DWORD)dwMaxExp - (DWORD)dwMinExp) + (DWORD)dwMinExp;
	}

	BOOL bRet = pChar->GetPlyMainCha()->AddExpAndNotic(dwValue);
	char szData[128];
	sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00006), szNpc, dwValue);
	pChar->SystemNotice(szData);
	ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);

	return LUA_TRUE;
}

int AddExpAndType(CCharacter* pChar, CTalkNpc* pTalk, int byType, int dwMinExp, int dwMaxExp)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	DWORD dwValue;
	if ((DWORD)dwMinExp >= (DWORD)dwMaxExp)
	{
		dwValue = (DWORD)dwMinExp;
	}
	else
	{
		dwValue = rand() % ((DWORD)dwMaxExp - (DWORD)dwMinExp) + (DWORD)dwMinExp;
	}

	BOOL bRet;
	if ((BYTE)byType == mission::MIS_EXP_NOMAL)
	{
		bRet = pChar->GetPlyMainCha()->AddExpAndNotic(dwValue);
		char szData[128];
		sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00006), szNpc, dwValue);
		pChar->SystemNotice(szData);
		ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);
	}
	else if ((BYTE)byType == MIS_EXP_SAIL)
	{
		bRet = pChar->GetPlyMainCha()->AddAttr(ATTR_CSAILEXP, dwValue);
		char szData[128];
		sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00004), szNpc, dwValue);
		pChar->SystemNotice(szData);
		ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);
	}
	else if ((BYTE)byType == MIS_EXP_LIFE)
	{
		bRet = pChar->GetPlyMainCha()->AddAttr(ATTR_CLIFEEXP, dwValue);
		char szData[128];
		sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00005), szNpc, dwValue);
		pChar->SystemNotice(szData);
		ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);
	}
	else
	{
		pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00007), szNpc, (BYTE)byType, dwValue);
		bRet = FALSE;
	}

	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AddMoney(CCharacter* pChar, CTalkNpc* pTalk, int dwMoney)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	pChar->GetPlyMainCha()->AddMoney(szNpc, (DWORD)dwMoney);
	char szData[128];
	sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00008), szNpc, (DWORD)dwMoney);
	ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);

	return LUA_TRUE;
}

int TakeMoney(CCharacter* pChar, CTalkNpc* pTalk, int dwMoney)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	BOOL bRet = pChar->GetPlyMainCha()->TakeMoney(szNpc, (DWORD)dwMoney);
	char szData[128];
	sprintf(szData, RES_STRING(GM_CHARSCRIPT_CPP_00009), szNpc, (DWORD)dwMoney);
	ToLogService("trade", "[CHA_MIS] {} : {}", pChar->GetName(), szData);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasMoney(CCharacter* pChar, int dwMoney)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->GetPlyMainCha()->HasMoney((DWORD)dwMoney);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasCancelMissionMoney(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	DWORD dwMoney = (long)pChar->getAttr(ATTR_LV) * (long)pChar->getAttr(ATTR_LV) + 100;
	BOOL bRet = pChar->GetPlyMainCha()->HasMoney(dwMoney);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TakeCancelMissionMoney(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	DWORD dwMoney = (long)pChar->getAttr(ATTR_LV) * (long)pChar->getAttr(ATTR_LV) + 100;
	BOOL bRet = pChar->GetPlyMainCha()->TakeMoney(szNpc, dwMoney);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int CheckFusionItem(SItemGrid* pItem1, SItemGrid* pItem2)
{
	if (!pItem1 || !pItem2) return LUA_FALSE;
	return pItem1->FusionCheck(*pItem2) ? LUA_TRUE : LUA_FALSE;
}

//Add by sunny.sun 20080529
//Begin
int GetTicketIssue(SItemGrid* pItem)
{
	if (!pItem) return 0;
	int issue = pItem->sInstAttr[0][1] % 1000;
	return issue;
}

// GetTicketItemno uses pushstring with variable logic -- keep as raw
int GetTicketItemno_raw(lua_State* L)
{
	BOOL bValid = (lua_gettop(L) == 2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	SItemGrid* pItem1 = (SItemGrid*)lua_touserdata(L, 1);
	int index = (int)lua_tonumber(L, 2);

	if (floor((float)pItem1->sInstAttr[0][1] / 1000) > 0)
	{
		if (index >= 0 && index < 7)
		{
			short c1 = (short)floor((float)pItem1->sInstAttr[2][1] / 100);
			short c2 = pItem1->sInstAttr[2][1] % 100;

			short c3 = (short)floor((float)pItem1->sInstAttr[3][1] / 100);
			short c4 = pItem1->sInstAttr[3][1] % 100;

			short c5 = (short)floor((float)pItem1->sInstAttr[4][1] / 100);
			short c6 = pItem1->sInstAttr[4][1] % 100;

			char buffer[7];
			sprintf(buffer, "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6);
			string Ticketno = buffer;

			if (index == 0)
			{
				lua_pushstring(L, Ticketno.c_str());
			}
			else
			{
				lua_pushstring(L, Ticketno.substr(index - 1, 1).c_str());
			}
		}
		return 1;
	}
	else
		return 0;
}

SItemGrid* GetSItemGrid(CCharacter* pChar, int index)
{
	if (!pChar) return nullptr;
	return pChar->m_CKitbag.GetGridContByID(index);
}
//End

int FusionItem(SItemGrid* pItem1, SItemGrid* pItem2)
{
	if (!pItem1 || !pItem2) return LUA_FALSE;

	CItemRecord* pItem = GetItemRecordInfo(pItem2->sID);
	if (CItemRecord::IsVaildFusionID(pItem))
	{
		pItem1->lDBParam = pItem2->lDBParam;
	}
	pItem1->CopyInstAttr(*pItem2);

	pItem1->SetItemLevel(10);

	return LUA_TRUE;
}

std::tuple<int, int> MakeItem(CCharacter* pChar, int sItemID, int sCount, int byType)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);

	USHORT sItemPos = 0;
	BOOL bRet = pChar->GetPlyMainCha()->MakeItem((USHORT)sItemID, (USHORT)sCount, sItemPos, (BYTE)byType);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)sItemPos);
}

// GiveItem has variable args (5-7) -- keep as raw
int GiveItem_raw(lua_State* L)
{
	BOOL bValid = ((lua_gettop(L) >= 5 && lua_gettop(L) <= 7) && lua_islightuserdata(L, 1) &&
		lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (pChar == NULL)
	{
		E_LUANULL;
		return 0;
	}

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata(L, 2);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	USHORT sItemID = (USHORT)lua_tonumber(L, 3);
	USHORT sCount = (USHORT)lua_tonumber(L, 4);
	BYTE byType = (BYTE)lua_tonumber(L, 5);
	BOOL isTradable = 1;
	LONG expiration = 0;

	if (lua_gettop(L) >= 6 && lua_isnumber(L, 6)) {
		isTradable = static_cast<BOOL>(lua_tonumber(L, 6));
	}
	if (lua_gettop(L) == 7 && lua_isnumber(L, 7)) {
		expiration = std::time(0) + static_cast<LONG>(lua_tonumber(L, 7));
	}

	BOOL bRet = pChar->GetPlyMainCha()->AddItem(sItemID, sCount, szNpc, byType, '\a', isTradable, expiration);
	lua_pushnumber(L, (bRet) ? LUA_TRUE : LUA_FALSE);

	return 1;
}

int GiveItemX(CCharacter* pChar, CTalkNpc* pTalk, int sItemID, int sCount, int byType)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	BOOL bRet = pChar->GetPlyMainCha()->AddItem2KitbagTemp((USHORT)sItemID, (USHORT)sCount, szNpc, (BYTE)byType);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

// GiveItemY has variable args (5-7) and returns lightuserdata -- keep as raw
int GiveItemY_raw(lua_State* L)
{
	BOOL bValid = ((lua_gettop(L) >= 5 && lua_gettop(L) <= 7) && lua_islightuserdata(L, 1) &&
		lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5));
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	if (pChar == NULL)
	{
		E_LUANULL;
		return 0;
	}

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	CTalkNpc* pTalk = (CTalkNpc*)lua_touserdata(L, 2);
	if (pTalk)
	{
		strncpy(szNpc, pTalk->GetName(), defENTITY_NAME_LEN - 1);
	}

	USHORT sItemID = (USHORT)lua_tonumber(L, 3);
	USHORT sCount = (USHORT)lua_tonumber(L, 4);
	BYTE byType = (BYTE)lua_tonumber(L, 5);
	BOOL isTradable = 1;
	LONG expiration = 0;

	if (lua_gettop(L) >= 6 && lua_isnumber(L, 6)) {
		isTradable = static_cast<BOOL>(lua_tonumber(L, 6));
	}
	if (lua_gettop(L) == 7 && lua_isnumber(L, 7)) {
		expiration = std::time(0) + static_cast<LONG>(lua_tonumber(L, 7));
	}
	short posID = defKITBAG_DEFPUSH_POS;
	BOOL bRet = (pChar->GetPlyMainCha()->AddItem(sItemID, sCount, szNpc, byType, '\a', isTradable, expiration, &posID));
	SItemGrid* pItem = pChar->GetItem2(2, posID);
	luabridge::push(L, pItem);

	return 1;
}

int HasLeaveBagTempGrid(CCharacter* pChar, int sNum)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasLeaveBagTempGrid((USHORT)sNum);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TakeItemBagTemp(CCharacter* pChar, CTalkNpc* pNpc, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pNpc)
	{
		strcpy(szNpc, pNpc->GetName());
	}

	BOOL bRet = pChar->GetPlyMainCha()->TakeItemBagTemp((USHORT)sItemID, (USHORT)sCount, szNpc);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TakeItem(CCharacter* pChar, CTalkNpc* pNpc, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;

	char szNpc[defENTITY_NAME_LEN];
	strncpy(szNpc, RES_STRING(GM_CHARSCRIPT_CPP_00001), defENTITY_NAME_LEN - 1);
	if (pNpc)
	{
		strcpy(szNpc, pNpc->GetName());
	}

	BOOL bRet = pChar->GetPlyMainCha()->TakeItem((USHORT)sItemID, (USHORT)sCount, szNpc);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasItem(CCharacter* pChar, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->GetPlyMainCha()->HasItem((USHORT)sItemID, (USHORT)sCount);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BagTempHasItem(CCharacter* pChar, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->GetPlyMainCha()->HasItemBagTemp((USHORT)sItemID, (USHORT)sCount);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BankHasItem(CCharacter* pChar, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;
	USHORT sCountVal = (USHORT)sCount;
	BOOL bRet = pChar->GetPlayer()->BankHasItem((USHORT)sItemID, sCountVal);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int EquipHasItem(CCharacter* pChar, int sItemID, int sCount)
{
	if (!pChar) return LUA_FALSE;

	short sItemCount = 0;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		SItemGrid* pItem = &(pChar->m_SChaPart.SLink[i]);
		if (pItem->sID == (USHORT)sItemID && pItem->sNum > 0)
		{
			sItemCount += pItem->sNum;
		}
	}

	BOOL bRet = (sItemCount >= (USHORT)sCount) ? true : false;
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsEquip(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;

	BOOL bRet = false;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		SItemGrid* pItem = &(pChar->m_SChaPart.SLink[i]);
		if (pItem->sID > 0 && pItem->sNum > 0)
		{
			bRet = true;
			break;
		}
	}
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int KitbagLock(CCharacter* pChar, int sLock)
{
	if (!pChar) return LUA_FALSE;
	USHORT sKitbagLock = pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() ? 1 : 0;
	BOOL bRet = ((USHORT)sLock == sKitbagLock) ? true : false;
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetNumItem(CCharacter* pChar, int sItemID, int sCount)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	USHORT sCountOut = (USHORT)sCount;
	BOOL bRet = pChar->GetPlyMainCha()->GetNumItem((USHORT)sItemID, sCountOut);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)sCountOut);
}

std::tuple<int, int> GetNeedItemCount(CCharacter* pChar, int wRoleID, int sItemID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	USHORT sCount = 0;
	BOOL bRet = pChar->GetMisNeedItemCount((WORD)wRoleID, (USHORT)sItemID, sCount);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)sCount);
}

int AddMissionState(CCharacter* pChar, int dwNpcID, int byID, int byState)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->AddMissionState((DWORD)dwNpcID, (BYTE)byID, (BYTE)byState);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ResetMissionState(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->ResetMissionState(*pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetMissionState(CCharacter* pChar, int dwNpcID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	BYTE byState;
	BOOL bRet = pChar->GetMissionState((DWORD)dwNpcID, byState);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byState);
}

std::tuple<int, int> GetNumMission(CCharacter* pChar, int dwNpcID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	BYTE byNum = 0;
	BOOL bRet = pChar->GetNumMission((DWORD)dwNpcID, byNum);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byNum);
}

std::tuple<int, int, int> GetMissionInfo(CCharacter* pChar, int dwNpcID, int byIndex)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0);
	BYTE byID, byState;
	BOOL bRet = pChar->GetMissionInfo((DWORD)dwNpcID, (BYTE)byIndex, byID, byState);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byID, (int)byState);
}

std::tuple<int, int> GetCharMission(CCharacter* pChar, int dwNpcID, int byID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	BYTE byState;
	BOOL bRet = pChar->GetCharMission((DWORD)dwNpcID, (BYTE)byID, byState);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byState);
}

std::tuple<int, int, int, int> GetNextMission(CCharacter* pChar, int dwNpcID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0, 0);
	BYTE byIndex, byID, byState;
	BOOL bRet = pChar->GetNextMission((DWORD)dwNpcID, byIndex, byID, byState);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byIndex, (int)byID, (int)byState);
}

int IsMissionState(int byState, int byFlag)
{
	return ((BYTE)byFlag & (BYTE)byState) ? LUA_TRUE : LUA_FALSE;
}

int SetMissionPage(CCharacter* pChar, int dwNpcID, int byPrev, int byNext, int byState)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetMissionPage((DWORD)dwNpcID, (BYTE)byPrev, (BYTE)byNext, (BYTE)byState);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int, int, int> GetMissionPage(CCharacter* pChar, int dwNpcID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0, 0);
	BYTE byPrev, byNext, byState;
	BOOL bRet = pChar->GetMissionPage((DWORD)dwNpcID, byPrev, byNext, byState);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byPrev, (int)byNext, (int)byState);
}

int SetMissionTempInfo(CCharacter* pChar, int dwNpcID, int wID, int byState, int byType)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetTempData((DWORD)dwNpcID, (WORD)wID, (BYTE)byState, (BYTE)byType);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int, int, int> GetMissionTempInfo(CCharacter* pChar, int dwNpcID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0, 0);
	BYTE byState, byType;
	WORD wID;
	BOOL bRet = pChar->GetTempData((DWORD)dwNpcID, wID, byState, byType);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)wID, (int)byState, (int)byType);
}


void ReAllPlayEffect(CCharacter* pChar) {
	auto l_wpk = net::msg::serialize(net::msg::McChaPlayEffectMessage{pChar->GetID(), 114});
	pChar->NotiChgToEyeshot(l_wpk);
}

int ReAll(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->RestoreAll();
	ReAllPlayEffect(pChar);
	return LUA_TRUE;
}

int ReAllHp(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->RestoreAllHp();
	ReAllPlayEffect(pChar);
	return LUA_TRUE;
}

int ReHp(CCharacter* pChar, int byRate)
{
	if (!pChar) return LUA_FALSE;
	pChar->RestoreHp((BYTE)byRate);
	ReAllPlayEffect(pChar);
	return LUA_TRUE;
}

int ReAllSp(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->RestoreAllSp();
	ReAllPlayEffect(pChar);
	return LUA_TRUE;
}

int ReSp(CCharacter* pChar, int byRate)
{
	if (!pChar) return LUA_FALSE;
	pChar->RestoreSp((BYTE)byRate);
	ReAllPlayEffect(pChar);
	return LUA_TRUE;
}

int LvCheck(CCharacter* pChar, const std::string& pszData, int wLevel)
{
	if (!pChar || pszData.empty()) return LUA_FALSE;

	BOOL bRet;
	if (pszData[0] == '=')
	{
		bRet = pChar->getAttr(ATTR_LV) == (WORD)wLevel;
	}
	else if (pszData[0] == '>')
	{
		bRet = pChar->getAttr(ATTR_LV) > (WORD)wLevel;
	}
	else if (pszData[0] == '<')
	{
		bRet = pChar->getAttr(ATTR_LV) < (WORD)wLevel;
	}
	else
	{
		return LUA_FALSE;
	}

	return bRet;
}

int LvEqual(CCharacter* pChar, int wLevel)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_LV) == (WORD)wLevel;
}

int LvThan(CCharacter* pChar, int wLevel)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_LV) > (WORD)wLevel;
}

int GetCharMissionLevel(CCharacter* pChar)
{
	if (!pChar) return 0;
	return (int)(WORD)pChar->getAttr(ATTR_LV);
}

int PfEqual(CCharacter* pChar, int byPf)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_JOB) == (BYTE)byPf;
}

int HpCheck(CCharacter* pChar, const std::string& pszData, int wHp)
{
	if (!pChar || pszData.empty()) return LUA_FALSE;

	BOOL bRet;
	if (pszData[0] == '=')
	{
		bRet = pChar->getAttr(ATTR_HP) == (WORD)wHp;
	}
	else if (pszData[0] == '>')
	{
		bRet = pChar->getAttr(ATTR_HP) > (WORD)wHp;
	}
	else if (pszData[0] == '<')
	{
		bRet = pChar->getAttr(ATTR_HP) < (WORD)wHp;
	}
	else
	{
		return LUA_FALSE;
	}

	return bRet;
}

int HpEqual(CCharacter* pChar, int wHP)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_HP) == (WORD)wHP;
}

int HpThan(CCharacter* pChar, int wHP)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_HP) > (WORD)wHP;
}

int SpCheck(CCharacter* pChar, const std::string& pszData, int wSp)
{
	if (!pChar || pszData.empty()) return LUA_FALSE;

	BOOL bRet;
	if (pszData[0] == '=')
	{
		bRet = pChar->getAttr(ATTR_SP) == (WORD)wSp;
	}
	else if (pszData[0] == '>')
	{
		bRet = pChar->getAttr(ATTR_SP) > (WORD)wSp;
	}
	else if (pszData[0] == '<')
	{
		bRet = pChar->getAttr(ATTR_SP) < (WORD)wSp;
	}
	else
	{
		return LUA_FALSE;
	}

	return bRet;
}

int SpEqual(CCharacter* pChar, int wSP)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_SP) == (WORD)wSP;
}

int SpThan(CCharacter* pChar, int wSP)
{
	if (!pChar) return LUA_FALSE;
	return pChar->getAttr(ATTR_SP) > (WORD)wSP;
}

int HasRandMission(CCharacter* pChar, int wID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasRandMission((WORD)wID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AddRandMission(CCharacter* pChar, int wID, int wScriptID, int byType, int byLevel, int dwExp, int dwMoney, int sPrizeData, int sPrizeType, int byNumData)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->AddRandMission((WORD)wID, (WORD)wScriptID, (BYTE)byType, (BYTE)byLevel, (DWORD)dwExp, (DWORD)dwMoney, (USHORT)sPrizeData, (USHORT)sPrizeType, (BYTE)byNumData);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetRandMissionData(CCharacter* pChar, int wRoleID, int byIndex, int p1, int p2, int p3, int p4, int p5, int p6)
{
	if (!pChar) return LUA_FALSE;

	MISSION_DATA data;
	data.wParam1 = (WORD)p1;
	data.wParam2 = (WORD)p2;
	data.wParam3 = (WORD)p3;
	data.wParam4 = (WORD)p4;
	data.wParam5 = (WORD)p5;
	data.wParam6 = (WORD)p6;

	BOOL bRet = pChar->SetRandMissionData((WORD)wRoleID, (BYTE)byIndex, data);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int, int, int, int, int, int, int> GetRandMission(CCharacter* pChar, int wID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0, 0, 0, 0, 0, 0);

	BYTE byType, byLevel, byNumData;
	DWORD dwExp, dwMoney;
	USHORT sPrizeData, sPrizeType;
	BOOL bRet = pChar->GetRandMission((WORD)wID, byType, byLevel, dwExp, dwMoney, sPrizeData, sPrizeType, byNumData);

	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byType, (int)byLevel, (int)dwExp, (int)dwMoney, (int)sPrizeData, (int)sPrizeType, (int)byNumData);
}

std::tuple<int, int, int, int, int, int, int> GetRandMissionData(CCharacter* pChar, int wRoleID, int byIndex)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0, 0, 0, 0, 0);

	MISSION_DATA data;
	BOOL bRet = pChar->GetRandMissionData((WORD)wRoleID, (BYTE)byIndex, data);

	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)data.wParam1, (int)data.wParam2, (int)data.wParam3, (int)data.wParam4, (int)data.wParam5, (int)data.wParam6);
}

int TakeAllRandItem(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->TakeAllRandItem((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TakeRandNpcItem(CCharacter* pChar, int wRoleID, int wNpcID, const std::string& pszNpc)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->TakeRandNpcItem((WORD)wRoleID, (WORD)wNpcID, pszNpc.c_str());
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasRandMissionNpc(CCharacter* pChar, int wRoleID, int wNpcID, int wAreaID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasRandMissionNpc((WORD)wRoleID, (WORD)wNpcID, (WORD)wAreaID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasRandNpcItemFlag(CCharacter* pChar, int wRoleID, int wNpcID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasSendNpcItemFlag((WORD)wRoleID, (WORD)wNpcID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int NoRandNpcItemFlag(CCharacter* pChar, int wRoleID, int wNpcID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->NoSendNpcItemFlag((WORD)wRoleID, (WORD)wNpcID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsMisNeedItem(CCharacter* pChar, int sItemID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsMisNeedItem((USHORT)sItemID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int CompleteRandMissionCount(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->CompleteRandMission((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int FailureRandMissionCount(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->FailureRandMission((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ResetRandMissionCount(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ResetRandMission((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ResetRandMissionNum(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ResetRandMissionNum((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasRandMissionCount(CCharacter* pChar, int wRoleID, int wCount)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasRandMissionCount((WORD)wRoleID, (WORD)wCount);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AddRandMissionNum(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->AddRandMissionNum((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetRandMissionCount(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	WORD wCount = 0;
	BOOL bRet = pChar->GetRandMissionCount((WORD)wRoleID, wCount);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)wCount);
}

std::tuple<int, int> GetRandMissionNum(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	WORD wNum = 0;
	BOOL bRet = pChar->GetRandMissionNum((WORD)wRoleID, wNum);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)wNum);
}

int Hide(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->Hide();
	return LUA_TRUE;
}

int Show(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->Show();
	return LUA_TRUE;
}

int ConvoyNpc(CCharacter* pChar, int wRoleID, int byIndex, int wNpcID, int byAiType)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ConvoyNpc((WORD)wRoleID, (BYTE)byIndex, (WORD)wNpcID, (BYTE)byAiType);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearConvoyNpc(CCharacter* pChar, int wRoleID, int byIndex)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearConvoyNpc((WORD)wRoleID, (BYTE)byIndex);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ClearAllConvoyNpc(CCharacter* pChar, int wRoleID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->ClearAllConvoyNpc((WORD)wRoleID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasConvoyNpc(CCharacter* pChar, int wRoleID, int byIndex)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasConvoyNpc((WORD)wRoleID, (BYTE)byIndex);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsConvoyNpc(CCharacter* pChar, int wRoleID, int byIndex, int wNpcID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsConvoyNpc((WORD)wRoleID, (BYTE)byIndex, (WORD)wNpcID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetSpawnPos(CCharacter* pChar, const std::string& pszCity)
{
	if (!pChar) return LUA_FALSE;

	pChar->SetBirthCity(pszCity.c_str());
	pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00010), pChar->GetName(), pszCity.c_str());

	return LUA_TRUE;
}

int IsSpawnPos(CCharacter* pChar, const std::string& pszCity)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = strcmp(pszCity.c_str(), pChar->GetBirthCity()) == 0;
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetProfession(CCharacter* pChar, int byPf)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetProfession((BYTE)byPf);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetProfession(CCharacter* pChar)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	BYTE byPf = (BYTE)pChar->getAttr(ATTR_JOB);
	return std::make_tuple(LUA_TRUE, (int)byPf);
}

std::tuple<int, int> GetCategory(CCharacter* pChar)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	BYTE byCat = (BYTE)pChar->GetCat();
	return std::make_tuple(LUA_TRUE, (int)byCat);
}

std::tuple<int, int, int> GetCatAndPf(CCharacter* pChar)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0);
	BYTE byCat = (BYTE)pChar->GetCat();
	BYTE byPf = (BYTE)pChar->getAttr(ATTR_JOB);
	return std::make_tuple(LUA_TRUE, (int)byCat, (int)byPf);
}

int IsCategory(CCharacter* pChar, int byCat)
{
	if (!pChar) return LUA_FALSE;
	return ((BYTE)byCat == pChar->GetCat()) ? LUA_TRUE : LUA_FALSE;
}

int SaveMissionData(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SaveMissionData();
	return bRet ? LUA_TRUE : LUA_FALSE;
}

//==========Begin=========================================================
int HasFame(CCharacter* pChar, int dwFame)
{
	if (!pChar) return LUA_FALSE;
	return (DWORD)pChar->getAttr(ATTR_FAME) >= (DWORD)dwFame ? LUA_TRUE : LUA_FALSE;
}

int HasGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return pChar->HasGuild() ? LUA_TRUE : LUA_FALSE;
}

int HasNavyGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return (pChar->HasGuild() ? (pChar->GetValidGuildID() > 1 && pChar->GetValidGuildID() < 100) : FALSE) ? LUA_TRUE : LUA_FALSE;
}

int NoNavyGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return (pChar->HasGuild() ? (pChar->GetValidGuildID() < 1 || pChar->GetValidGuildID() >= 100) : TRUE) ? LUA_TRUE : LUA_FALSE;
}

int HasPirateGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return (pChar->HasGuild() ? (pChar->GetValidGuildID() > 99 && pChar->GetValidGuildID() < 200) : FALSE) ? LUA_TRUE : LUA_FALSE;
}

int NoPirateGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return (pChar->HasGuild() ? (pChar->GetValidGuildID() <= 99) : TRUE) ? LUA_TRUE : LUA_FALSE;
}

int CreateGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = Guild::lua_CreateGuild(pChar);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ListAllGuild(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = Guild::lua_ListAllGuild(pChar);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

//===========End========================================================
int SetPkState(CCharacter* pChar, int byPk)
{
	if (!pChar) return LUA_FALSE;
	pChar->SetEnterGymkhana((BOOL)(BYTE)byPk);
	return LUA_TRUE;
}

int IsBoatFull(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = (pChar->GetPlayer()) ? pChar->GetPlayer()->IsBoatFull() : FALSE;
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int CreateBoat(CCharacter* pChar, int sBoat, int sBerth)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = g_CharBoat.Create(*pChar, (USHORT)sBoat, (USHORT)sBerth);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int> GetBoatID(CCharacter* pChar, int byIndex)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0);
	DWORD dwBoatID;
	BOOL bRet = pChar->GetBoatID((BYTE)byIndex, dwBoatID);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)dwBoatID);
}

int IsNeedRepair(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsNeedRepair();
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsNeedSupply(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->IsNeedSupply();
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int RepairBoat(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->RepairBoat();
	return LUA_TRUE;
}

int SupplyBoat(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->SupplyBoat();
	return LUA_TRUE;
}

int BoatLuanchOut(CCharacter* pChar, int byIndex, int sBerth, int sxPos, int syPos, int sDir)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->BoatLaunch((BYTE)byIndex, (USHORT)sBerth, (USHORT)sxPos, (USHORT)syPos, (USHORT)sDir);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BoatBerth(CCharacter* pChar, int sBerth, int sxPos, int syPos, int sDir)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->BoatBerth((USHORT)sBerth, (USHORT)sxPos, (USHORT)syPos, (USHORT)sDir);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BoatBerthList(CCharacter* pChar, int dwNpcID, int byType, int sBerth, int sxPos, int syPos, int sDir)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet;
	bRet = pChar->BoatBerthList((DWORD)dwNpcID, (BYTE)byType, (USHORT)sBerth, (USHORT)sxPos, (USHORT)syPos, (USHORT)sDir);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BoatTrade(CCharacter* pChar, int sBerth)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->BoatTrade((USHORT)sBerth);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int BoatBuildCheck(CCharacter* pChar, int sBoatID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = !g_CharBoat.BoatLimit(*pChar, (USHORT)sBoatID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasAllBoatInBerth(CCharacter* pChar, int sBerthID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasAllBoatInBerth((USHORT)sBerthID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasBoatInBerth(CCharacter* pChar, int sBerthID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasBoatInBerth((USHORT)sBerthID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasDeadBoatInBerth(CCharacter* pChar, int sBerthID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasDeadBoatInBerth((USHORT)sBerthID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasLuanchOut(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = (pChar->GetPlayer()) ? pChar->GetPlayer()->IsLuanchOut() : LUA_FALSE;
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int GetSection(int sData1, int sData2)
{
	return (USHORT)sData1 / (USHORT)sData2;
}

int ToDword(int dwData)
{
	return (int)(DWORD)dwData;
}

std::tuple<int, int> PackBag(CCharacter* pChar, CCharacter* pBoat, int sItemID, int sCount, int sPileID)
{
	if (!pChar || !pBoat) return std::make_tuple(LUA_FALSE, 0);
	USHORT sNumPack = 0;
	BOOL bRet = pChar->PackBag(*pBoat, (USHORT)sItemID, (USHORT)sCount, (USHORT)sPileID, sNumPack);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)sNumPack);
}

int PackBagList(CCharacter* pChar, int sBerthID, int byType, int byLevel)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->BoatPackBagList((USHORT)sBerthID, (BYTE)byType, (BYTE)byLevel);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int HasLeaveBagGrid(CCharacter* pChar, int sNum)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->HasLeaveBagGrid((USHORT)sNum);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int AdjustTradeItemCess(CCharacter* pChar, int sCess, int sData)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->AdjustTradeItemCess((USHORT)sCess, (USHORT)sData);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetTradeItemLevel(CCharacter* pChar, int byData)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->SetTradeItemLevel((BYTE)byData);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TradeItemLevelCheck(CCharacter* pChar, const std::string& pszData, int byData)
{
	if (!pChar || pszData.empty()) return LUA_FALSE;

	BOOL bRet = FALSE;
	BYTE byLevel;
	if (pChar->GetTradeItemLevel(byLevel))
	{
		if (pszData[0] == '>')
		{
			bRet = byLevel > (BYTE)byData;
		}
		else if (pszData[0] == '<')
		{
			bRet = byLevel < (BYTE)byData;
		}
		else if (pszData[0] == '=')
		{
			bRet = (BYTE)byData == byLevel;
		}
		else
		{
			return LUA_FALSE;
		}
	}

	return bRet ? LUA_TRUE : LUA_FALSE;
}

int TradeItemDataCheck(CCharacter* pChar, const std::string& pszData, int sData)
{
	if (!pChar || pszData.empty()) return LUA_FALSE;

	BYTE byLevel;
	USHORT sCess;
	BOOL bRet = pChar->GetTradeItemData(byLevel, sCess);
	if (bRet)
	{
		if (pszData[0] == '>')
		{
			bRet = sCess > (USHORT)sData;
		}
		else if (pszData[0] == '<')
		{
			bRet = sCess < (USHORT)sData;
		}
		else if (pszData[0] == '=')
		{
			bRet = sCess == (USHORT)sData;
		}
		else
		{
			return LUA_FALSE;
		}
	}

	return bRet ? LUA_TRUE : LUA_FALSE;
}

std::tuple<int, int, int> GetTradeItemData(CCharacter* pChar)
{
	if (!pChar) return std::make_tuple(LUA_FALSE, 0, 0);
	BYTE byLevel;
	USHORT sCess;
	BOOL bRet = pChar->GetTradeItemData(byLevel, sCess);
	return std::make_tuple(bRet ? LUA_TRUE : LUA_FALSE, (int)byLevel, (int)sCess);
}

int SetAttrChangeFlag(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	pChar->m_CChaAttr.ResetChangeFlag();
	return LUA_TRUE;
}

int SyncBoat(CCharacter* pBoat, int byType)
{
	if (!pBoat) return LUA_FALSE;
	pBoat->SyncBoatAttr((BYTE)byType, false);
	return LUA_TRUE;
}

int SyncChar(CCharacter* pChar, int byType)
{
	if (!pChar) return LUA_FALSE;
	pChar->SynAttr((BYTE)byType);
	return LUA_TRUE;
}

int OpenGuildBank(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenGuildBank();
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenBank(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenBank((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenRepair(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenRepair((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenForge(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenForge((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

// Add by lark.li 20080514 begin
int OpenLottery(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenLottery((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}
// End

int OpenUnite(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenUnite((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenMilling(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenMilling((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenFusion(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenFusion((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenUpgrade(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenUpgrade((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenEidolonMetempsychosis(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenEidolonMetempsychosis((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenEidolonFusion(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenEidolonFusion((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenItemTiChun(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenPurify((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenItemFix(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenFix((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenItemEnergy(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenEnergy((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenGetStone(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenGetStone((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenTiger(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	BOOL bRet = pChar->GetPlayer()->OpenTiger((CCharacter*)pTalk);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int OpenHair(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;

	if (pChar->HasTradeAction())
	{
		{ char _buf[256]; sprintf(_buf, RES_STRING(GM_CHARSCRIPT_CPP_00011), pChar->GetName()); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
		return LUA_FALSE;
	}

	pChar->Prl_OpenHair();
	pChar->TradeAction(true);

	return LUA_TRUE;
}

int Garner2GetWiner(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	auto l_wpk = net::msg::serializeGmGarner2GetOrderCmd();
	pChar->ReflectINFof(pChar, l_wpk);
	return LUA_TRUE;
}

int Garner2RequestReorder(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;

	CCharacter* pCha = pChar->m_submap->FindCharacter(pTalk->GetID(), pChar->GetShape().centre);
	if (pCha == NULL)
	{
		return LUA_FALSE;
	}

	CCharacter* pMainCha = pChar->GetPlyMainCha();
	if (!pMainCha)
	{
		return LUA_FALSE;
	}

	short sItemID = 3849;
	short pos = -1;
	USHORT sNum = pMainCha->m_CKitbag.GetCapacity();
	SItemGrid* pGridCont;
	for (short i = 0; i < sNum; i++)
	{
		pGridCont = pMainCha->m_CKitbag.GetGridContByID(i);
		if (pGridCont)
		{
			if (sItemID == pGridCont->sID)
			{
				pos = i;
				break;
			}
		}
	}
	if (-1 == pos)
		pMainCha->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00012));
	else
		pMainCha->Cmd_Garner2_Reorder(pos);

	return LUA_TRUE;
}

int ListAuction(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	g_AuctionSystem.ListAuction(pChar, pTalk);
	return LUA_TRUE;
}

//add by ALLEN 2007-10-19
int StartAuction(int sItemID, const std::string& szName, int sCount, int lBasePrice, int lMinBid)
{
	if (szName.empty()) return LUA_FALSE;

	if (!g_AuctionSystem.StartAuction((short)sItemID, szName, (short)sCount, (uInt)lBasePrice, (uInt)lMinBid))
	{
		return LUA_FALSE;
	}
	else
	{
		return LUA_TRUE;
	}
}

//add by ALLEN 2007-10-19
int EndAuction(int sItemID)
{
	if (!g_AuctionSystem.EndAuction((short)sItemID))
	{
		return LUA_FALSE;
	}
	else
	{
		return LUA_TRUE;
	}
}

int ListChallenge(CCharacter* pChar, CTalkNpc* pTalk)
{
	if (!pChar || !pTalk) return LUA_FALSE;
	game_db.ListChallenge(pChar);
	return LUA_TRUE;
}

int HasGuildLevel(CCharacter* pChar, int byLevel)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = game_db.HasGuildLevel(pChar, (BYTE)byLevel);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsTeamLeader(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return pChar->IsTeamLeader() ? LUA_TRUE : LUA_FALSE;
}

int GetChaBody(CCharacter* pChar)
{
	if (!pChar) return 0;
	return (int)pChar->m_SChaPart.sTypeID;
}

int GetMapActivePlayer(SubMap* pSubMap)
{
	if (!pSubMap) return 0;
	return (int)pSubMap->GetActivePlayer();
}

int DealAllActivePlayerInMap(SubMap* pSubMap, const std::string& pfunctionname)
{
	if (!pSubMap) return LUA_FALSE;
	string strfun(pfunctionname);
	pSubMap->DealActivePlayer(strfun);
	return LUA_TRUE;
}

int GetMapPlayer(SubMap* pSubMap)
{
	if (!pSubMap) return 0;
	return (int)pSubMap->GetPlayerNum();
}

int DealAllPlayerInMap(SubMap* pSubMap, const std::string& pfunctionname)
{
	if (!pSubMap) return LUA_FALSE;
	string strfun(pfunctionname);
	pSubMap->DealPlayer(strfun);
	return LUA_TRUE;
}

int IsGarnerWiner(CCharacter* pChar)
{
	if (!pChar) return 0;
	if (pChar->IsPlayerCha())
	{
		return (int)pChar->GetPlayer()->IsGarnerWiner();
	}
	return 0;
}

int IsItemValid(int sItemID)
{
	CItemRecord* pItem = GetItemRecordInfo((short)sItemID);
	if (pItem == NULL)
	{
		return LUA_FALSE;
	}
	return LUA_TRUE;
}

SItemGrid* GetItemP(CCharacter* pCha, int sPosID)
{
	if (!pCha) return nullptr;
	return pCha->m_CKitbag.GetGridContByID((short)sPosID);
}

SItemGrid* GetEquipItemP(CCharacter* pCha, int sPosID)
{
	if (!pCha) return nullptr;
	SItemGrid* pItem = pCha->GetEquipItem((short)sPosID);
	return pItem;
}

int IsInTeam(CCharacter* pChar)
{
	if (!pChar) return 0;
	return pChar->GetPlayer()->HasTeam() ? 1 : 0;
}

int HasTeammate(CCharacter* pChar, int bHigherArg, int dLevel)
{
	if (!pChar) return 0;
	bool bHigher = (bHigherArg == 1);

	int ret = 0;
	if (!pChar->GetPlayer()->HasTeam())
	{
		ret = 0;
	}
	else
	{
		CPlayer* pTeamMember = NULL;
		short sLevel = (short)(pChar->GetLevel());
		pChar->GetPlayer()->BeginGetTeamPly();
		while (pTeamMember = pChar->GetPlayer()->GetNextTeamPly())
		{
			if (bHigher)
			{
				if (pTeamMember->GetMainCha()->GetLevel() >= (sLevel + (short)dLevel))
				{
					ret = 1;
					break;
				}
			}
			else
			{
				if (pTeamMember->GetMainCha()->GetLevel() <= (sLevel - (short)dLevel))
				{
					ret = 1;
					break;
				}
			}
		}
	}

	return ret;
}

int SetCopySpecialInter(SubMap* pSubMap, int Interal)
{
	if (!pSubMap) return LUA_FALSE;
	pSubMap->SetSpecialInter(Interal);
	return LUA_TRUE;
}

int AddCreditX(CCharacter* pCha, int lCredit)
{
	if (!pCha) return LUA_FALSE;
	pCha->SetCredit((long)pCha->GetCredit() + (long)lCredit);
	pCha->SynAttr(enumATTRSYN_TASK);
	return LUA_TRUE;
}

int AddMasterCredit(CCharacter* pCha, int nCredit)
{
	if (!pCha) return LUA_FALSE;
	pCha->AddMasterCredit(nCredit);
	return LUA_TRUE;
}

int DelCredit(CCharacter* pCha, int lCredit)
{
	if (!pCha) return LUA_FALSE;
	long lCreditFinal = ((long)pCha->GetCredit() - (long)lCredit >= 0) ? ((long)pCha->GetCredit() - (long)lCredit) : 0;
	pCha->SetCredit(lCreditFinal);
	pCha->SynAttr(enumATTRSYN_TASK);
	return LUA_TRUE;
}

int GetCredit(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)(long)pCha->GetCredit();
}

int HasMaster(CCharacter* pCha)
{
	if (!pCha) return LUA_FALSE;
	return (pCha->GetMasterDBID() == 0) ? LUA_FALSE : LUA_TRUE;
}

int GetMasterID(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetMasterDBID();
}

// GetRoleByID returns lightuserdata -- keep as raw
int GetRoleByID_raw(lua_State* L)
{
	BOOL bValid = lua_gettop(L) == 1 && lua_isnumber(L, 1);
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}
	DWORD dwChaDBID = (DWORD)lua_tonumber(L, 1);
	CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = (CCharacter*)pPlayer->GetMainCha();
		luabridge::push(L, static_cast<CCharacter*>(pCha));
		return 1;
	}
	return 0;
}

int LifeSkillBegin(CCharacter* pChar, CTalkNpc* pTalk, int ltype)
{
	if (!pChar || !pTalk) return LUA_FALSE;

	CCharacter* pCha = pChar->m_submap->FindCharacter(pTalk->GetID(), pChar->GetShape().centre);
	if (pCha == NULL)
	{
		return LUA_FALSE;
	}

	if (pChar->m_CKitbag.IsPwdLocked())
	{
		pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00013));
		return LUA_TRUE;
	}

	if (pChar->IsReadBook())
	{
		pChar->SystemNotice(RES_STRING(GM_CHARSCRIPT_CPP_00014));
		return LUA_TRUE;
	}

	pChar->ForgeAction();
	auto l_wpk = net::msg::serialize(net::msg::McLifeSkillShowMessage{(long)ltype});
	pChar->ReflectINFof(pChar, l_wpk);
	return LUA_TRUE;
}

int ClearAllSubMapCha(SubMap* pSubmap)
{
	if (pSubmap)
	{
		pSubmap->ClearAllCha();
	}
	return LUA_TRUE;
}

int ClearAllSubMapMonster(SubMap* pSubmap)
{
	if (pSubmap)
	{
		pSubmap->ClearAllMonster();
	}
	return LUA_TRUE;
}

int ClearFightSkill(CCharacter* pChar, int skillID)
{
	if (!pChar) return 0;

	pChar->m_CSkillBag.SetChangeFlag(false);
	CSkillRecord* pCSkill = GetSkillRecordInfo(skillID);
	if (!pCSkill || (pCSkill->chFightType != enumSKILL_FIGHT) || !pCSkill->IsShow())
	{
		return 0;
	}

	SSkillGrid* pSkillGrid = pChar->m_CSkillBag.GetSkillContByID(skillID);
	if (!pSkillGrid)
	{
		return 0;
	}

	const auto skillPoint = pSkillGrid->chLv;
	if (!pChar->m_CSkillBag.Del(skillID))
	{
		return 0;
	}

	pChar->SkillRefresh();
	pChar->SynSkillBag(enumSYN_SKILLBAG_MODI);
	return (int)skillPoint;
}

int ClearAllFightSkill(CCharacter* pChar)
{
	if (!pChar) return 0;

	pChar->m_CSkillBag.SetChangeFlag(false);

	int nSkillPoint{ 0 };
	{
		for (auto i = pChar->m_CSkillBag.GetSkillNum(); i-- > 0;)
		{
			auto skill = pChar->m_CSkillBag.GetSkillContByNum(i);
			if (!skill)
			{
				continue;
			}

			auto skillrecord = GetSkillRecordInfo(skill->sID);
			if (!skillrecord || skillrecord->chFightType != enumSKILL_FIGHT || !skillrecord->IsShow())
			{
				continue;
			}

			const auto skillLv = skill->chLv;
			if (pChar->m_CSkillBag.Del(skill->sID));
			{
				nSkillPoint += skillLv;
			}
		}
	}

	pChar->SkillRefresh();
	pChar->SynSkillBag(enumSYN_SKILLBAG_MODI);
	return nSkillPoint;
}

void RefreshCha(CCharacter* pChar)
{
	if (!pChar) return;

	pChar->SynSkillStateToEyeshot();
	pChar->SynKitbagNew(enumSYN_KITBAG_EQUIP);

	g_luaAPI.Call("AttrRecheck", pChar);
	if (pChar->GetPlayer())
	{
		pChar->GetPlayer()->RefreshBoatAttr();
		pChar->SyncBoatAttr(enumATTRSYN_ITEM_MEDICINE);
	}
	pChar->SynAttrToSelf(enumATTRSYN_ITEM_MEDICINE);
}

void ChangeJob(CCharacter* pChar, int lJob)
{
	if (!pChar) return;

	pChar->m_CChaAttr.ResetChangeFlag();
	pChar->setAttr(ATTR_JOB, (long)lJob);
	pChar->SetBoatAttrChangeFlag(false);
	g_luaAPI.Call("AttrRecheck", pChar);
	if (pChar->GetPlayer())
	{
		pChar->GetPlayer()->RefreshBoatAttr();
		pChar->SyncBoatAttr(enumATTRSYN_CHANGE_JOB);
	}
	pChar->SynAttrToSelf(enumATTRSYN_CHANGE_JOB);
}

int IsChaStall(CCharacter* pChar)
{
	if (!pChar) return LUA_FALSE;
	return pChar->GetStallData() ? LUA_TRUE : LUA_FALSE;
}

std::string GetActName(CCharacter* pChar)
{
	if (!pChar) return "";
	return std::string(pChar->GetPlayer()->GetActName());
}

int GetActID(CCharacter* pChar)
{
	if (!pChar) return 0;
	return (int)pChar->GetPlayer()->GetDBActId();
}

int GetExpState(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetExpScale();
}

int KillCha(CCharacter* pCha)
{
	if (!pCha) return 0;
	if (pCha->IsPlayerCha())
	{
		return 0;
	}
	else
	{
		pCha->Free();
		return 1;
	}
}

// kong@pkodev.net 09.22.2017
int GetGmLv(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetPlayer()->GetGMLev();
}

void SetGmLv(CCharacter* pCha, int gmLv)
{
	if (!pCha) return;
	pCha->GetPlayer()->SetGMLev(gmLv);
	Square& chaPos = (Square&)pCha->GetShape();
	pCha->GetPlayer()->GetMainCha()->Cmd_EnterMap(pCha->GetBirthMap(), -1, chaPos.centre.x, chaPos.centre.y, 0);
	game_db.SaveGmLv(pCha->GetPlayer());
}

void RequestClientPin(CCharacter* pCha, int action)
{
	if (!pCha) return;
	pCha->requestType = (char)action;
	pCha->requestPos.centre.x = pCha->GetShape().centre.x;
	pCha->requestPos.centre.y = pCha->GetShape().centre.y;
	auto l_wpk = net::msg::serializeMcRequestPinCmd();
	pCha->ReflectINFof(pCha, l_wpk);
}

// DealAllInGuild has variable args (2 or 3) with dynamic type checking -- keep as raw
int DealAllInGuild_raw(lua_State* L) {
	BOOL bValid = ((lua_gettop(L) == 3 && lua_isstring(L, 3)) || lua_gettop(L) == 2) && lua_isstring(L, 2) && lua_isnumber(L, 1);
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	int guildID = (int)lua_tonumber(L, 1);
	const char* luaFunc = lua_tostring(L, 2);

	if (lua_isstring(L, 3)) {
		const char* luaParam = lua_tostring(L, 3);
		g_pGameApp->DealAllInGuild(guildID, luaFunc, luaParam);
	}
	else {
		g_pGameApp->DealAllInGuild(guildID, luaFunc, "");
	}

	return 0;
}

// GetPlayerByName returns lightuserdata -- keep as raw
int GetPlayerByName_raw(lua_State* L) {
	BOOL bValid = (lua_gettop(L) == 1 && lua_isstring(L, 1));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	const char* chaName = lua_tostring(L, 1);
	CCharacter* pCha = g_pGameApp->FindPlayerChaByNameLua(chaName);
	if (pCha) {
		luabridge::push(L, static_cast<CCharacter*>(pCha));
		return 1;
	}
	return 0;
}

// GetPlayerByActName returns variable number of lightuserdata (1-3) -- keep as raw
int GetPlayerByActName_raw(lua_State* L) {
	BOOL bValid = (lua_gettop(L) == 1 && lua_isstring(L, 1));
	if (!bValid) {
		PARAM_ERROR
		return 0;
	}
	const char* chaName = lua_tostring(L, 1);
	CCharacter* pChas[3];
	int count = g_pGameApp->FindPlayerChaByActNameLua(chaName, pChas);
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			luabridge::push(L, static_cast<CCharacter*>(pChas[i]));
		}
		return count;
	}
	return 0;
}

int GetItemQuantity(SItemGrid* pSItem)
{
	if (!pSItem) return 0;
	return (int)pSItem->sNum;
}

int GetOriginalChaTypeID(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetIcon();
}

void TransformCha(CCharacter* pAtt, int mID)
{
	if (!pAtt) return;
	Square& chaPos = (Square&)pAtt->GetShape();
	pAtt->m_SChaPart.sTypeID = (short)mID;
	pAtt->m_cat = (short)mID;
	pAtt->GetPlayer()->GetMainCha()->Cmd_EnterMap(pAtt->GetBirthMap(), -1, chaPos.centre.x, chaPos.centre.y, 0);
}

// GetOnlineCount uses BEGINGETGATE / GETNEXTGATE macros -- keep as raw
int GetOnlineCount_raw(lua_State* pLS) {
	BEGINGETGATE();
	GateServer* pGateServer;

	int nCount = 0;
	while (pGateServer = GETNEXTGATE())
		nCount += GETPLAYERCOUNT(pGateServer);

	lua_pushnumber(pLS, nCount);
	return 1;
}

void PopupNotice(CCharacter* pChar, const std::string& pszData)
{
	if (!pChar) return;
	pChar->PopupNotice(pszData.c_str());
}

void BanActRole(CCharacter* pChar)
{
	if (!pChar) return;
	const char* actName = pChar->GetPlayer()->GetActName();
	if (actName == NULL) return;
	KICKPLAYER(pChar->GetPlayer(), 1);
	g_pGameApp->BanAccount(actName);
}

void BanActName(const std::string& actName)
{
	g_pGameApp->BanAccount(actName.c_str());
}

void UnbanAct(const std::string& actName)
{
	g_pGameApp->UnbanAccount(actName.c_str());
}

void ColourNotice(CCharacter* pChar, int rgb, const std::string& pszData)
{
	if (!pChar) return;
	pChar->ColourNotice((DWORD)rgb, pszData.c_str());
}

//send daily buff packet info to client
void SendDailyBuffInfo(CCharacter* pChar, const std::string& imgName, const std::string& LabelInfo)
{
	if (!pChar) return;
	auto l_wpk = net::msg::serialize(net::msg::McDailyBuffInfoMessage{imgName.c_str(), LabelInfo.c_str()});
	pChar->ReflectINFof(pChar, l_wpk);
}

//force data save @mothannakh
void ForcePlayerSave(CCharacter* pChar)
{
	if (!pChar) return;
	game_db.SavePlayer(pChar->GetPlayer(), enumSAVE_TYPE_TIMER);
}

// String2Item has variable args (2 or 3) with different behavior -- keep as raw
int String2Item_raw(lua_State* L) {
	if (!((lua_gettop(L) == 2 || (lua_gettop(L) == 3 && lua_isnumber(L, 3))) && lua_islightuserdata(L, 1) && lua_isstring(L, 2))) {
		return 0;
	}
	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	const char* pszData = lua_tostring(L, 2);
	SItemGrid SGridCont;
	String2Item(pszData, &SGridCont);
	short gridID = defKITBAG_DEFPUSH_POS;

	if (lua_gettop(L) == 3 && lua_tonumber(L, 3) == 1) {
		pChar->m_pCKitbagTmp->SetChangeFlag(false);
		Short sPushPos = defKITBAG_DEFPUSH_POS;
		Short sPushRet = pChar->m_pCKitbagTmp->Push(&SGridCont, sPushPos);
		pChar->SynKitbagTmpNew(enumSYN_KITBAG_SYSTEM);
	}
	else {
		pChar->KbPushItem(true, true, &SGridCont, gridID);
		pChar->SynKitbagNew(enumSYN_KITBAG_SYSTEM);
	}
	return 1;
}

int SetGlobalRates(int droprate, int exprate)
{
	if (droprate && exprate) {
		g_pGameApp->SetGlobalRates((float)droprate, (float)exprate);
		return LUA_TRUE;
	}
	return LUA_FALSE;
}

int GetPlyDropRate(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetDropRate();
}

int GetPlyExpRate(CCharacter* pCha)
{
	if (!pCha) return 0;
	return (int)pCha->GetExpRate();
}

int RemoveOfflineMode(CCharacter* pCha)
{
	if (!pCha) return LUA_FALSE;

	CPlayer* pPly = pCha->GetPlayer();
	if (!pPly) return LUA_FALSE;

	if (!pCha->IsOfflineMode()) return LUA_FALSE;

	g_pGameApp->ReleaseGamePlayer(pPly);
	return LUA_TRUE;
}

BOOL RegisterCharScript()
{
	lua_State* L = g_pLuaState;

	// Raw lua_CFunction registrations (varargs / special return)
	// LG is defined in lua_gamectrl.h as LG_raw
	lua_register(L, "LG", LG_raw);
	lua_register(L, "GetTicketItemno", GetTicketItemno_raw);
	lua_register(L, "GiveItem", GiveItem_raw);
	lua_register(L, "GiveItemY", GiveItemY_raw);
	lua_register(L, "GetRoleByID", GetRoleByID_raw);
	lua_register(L, "DealAllInGuild", DealAllInGuild_raw);
	lua_register(L, "GetPlayerByName", GetPlayerByName_raw);
	lua_register(L, "GetPlayerByActName", GetPlayerByActName_raw);
	lua_register(L, "GetOnlineCount", GetOnlineCount_raw);
	lua_register(L, "String2Item", String2Item_raw);

	// LuaBridge auto-marshaled function registrations
	// GetTickCount, Msg, Exit are defined in lua_gamectrl.h
	luabridge::getGlobalNamespace(L)
		.addFunction("GetTickCount", GetTickCount_typed)
		.addFunction("Msg", Msg)
		.addFunction("Exit", Exit_typed)

		LUABRIDGE_REGISTER_FUNC(GetResString)

		LUABRIDGE_REGISTER_FUNC(GetSection)
		LUABRIDGE_REGISTER_FUNC(ToDword)

		// Map
		LUABRIDGE_REGISTER_FUNC(SetMap)
		LUABRIDGE_REGISTER_FUNC(SetMapGuildWar)
		LUABRIDGE_REGISTER_FUNC(AddTrigger)
		LUABRIDGE_REGISTER_FUNC(AddMission)
		LUABRIDGE_REGISTER_FUNC(HasMission)
		LUABRIDGE_REGISTER_FUNC(GetMisScriptID)
		LUABRIDGE_REGISTER_FUNC(SetMissionComplete)
		LUABRIDGE_REGISTER_FUNC(SetMissionFailure)
		LUABRIDGE_REGISTER_FUNC(HasMisssionFailure)
		LUABRIDGE_REGISTER_FUNC(IsMissionFull)
		LUABRIDGE_REGISTER_FUNC(DeleteTrigger)
		LUABRIDGE_REGISTER_FUNC(ClearTrigger)
		LUABRIDGE_REGISTER_FUNC(ClearMission)
		LUABRIDGE_REGISTER_FUNC(SetFlag)
		LUABRIDGE_REGISTER_FUNC(ClearFlag)
		LUABRIDGE_REGISTER_FUNC(IsFlag)
		LUABRIDGE_REGISTER_FUNC(IsValidFlag)
		LUABRIDGE_REGISTER_FUNC(SetRecord)
		LUABRIDGE_REGISTER_FUNC(ClearRecord)
		LUABRIDGE_REGISTER_FUNC(IsRecord)
		LUABRIDGE_REGISTER_FUNC(IsValidRecord)

		LUABRIDGE_REGISTER_FUNC(IsMissionState)
		LUABRIDGE_REGISTER_FUNC(GetNumMission)
		LUABRIDGE_REGISTER_FUNC(GetMissionInfo)
		LUABRIDGE_REGISTER_FUNC(GetCharMission)
		LUABRIDGE_REGISTER_FUNC(GetNextMission)
		LUABRIDGE_REGISTER_FUNC(AddMissionState)
		LUABRIDGE_REGISTER_FUNC(ResetMissionState)
		LUABRIDGE_REGISTER_FUNC(GetMissionState)
		LUABRIDGE_REGISTER_FUNC(SetMissionPage)
		LUABRIDGE_REGISTER_FUNC(GetMissionPage)
		LUABRIDGE_REGISTER_FUNC(SetMissionTempInfo)
		LUABRIDGE_REGISTER_FUNC(GetMissionTempInfo)

		LUABRIDGE_REGISTER_FUNC(AddSkill)
		LUABRIDGE_REGISTER_FUNC(AddExp)
		LUABRIDGE_REGISTER_FUNC(AddLifeExp)
		LUABRIDGE_REGISTER_FUNC(AddSailExp)
		LUABRIDGE_REGISTER_FUNC(AddExpAndType)
		LUABRIDGE_REGISTER_FUNC(AddMoney)
		LUABRIDGE_REGISTER_FUNC(TakeMoney)
		LUABRIDGE_REGISTER_FUNC(HasMoney)
		LUABRIDGE_REGISTER_FUNC(HasCancelMissionMoney)
		LUABRIDGE_REGISTER_FUNC(TakeCancelMissionMoney)
		LUABRIDGE_REGISTER_FUNC(MakeItem)
		// GiveItem registered as raw above
		LUABRIDGE_REGISTER_FUNC(GiveItemX)
		// GiveItemY registered as raw above
		LUABRIDGE_REGISTER_FUNC(HasLeaveBagTempGrid)
		LUABRIDGE_REGISTER_FUNC(TakeItem)
		LUABRIDGE_REGISTER_FUNC(TakeItemBagTemp)
		LUABRIDGE_REGISTER_FUNC(HasItem)
		LUABRIDGE_REGISTER_FUNC(BankHasItem)
		LUABRIDGE_REGISTER_FUNC(BagTempHasItem)
		LUABRIDGE_REGISTER_FUNC(EquipHasItem)
		LUABRIDGE_REGISTER_FUNC(IsEquip)
		LUABRIDGE_REGISTER_FUNC(KitbagLock)
		LUABRIDGE_REGISTER_FUNC(GetNumItem)
		LUABRIDGE_REGISTER_FUNC(GetNeedItemCount)
		LUABRIDGE_REGISTER_FUNC(IsMisNeedItem)
		LUABRIDGE_REGISTER_FUNC(HasLeaveBagGrid)
		LUABRIDGE_REGISTER_FUNC(IsItemValid)
		LUABRIDGE_REGISTER_FUNC(GetItemP)
		LUABRIDGE_REGISTER_FUNC(GetEquipItemP)

		// Random missions
		LUABRIDGE_REGISTER_FUNC(HasRandMission)
		LUABRIDGE_REGISTER_FUNC(AddRandMission)
		LUABRIDGE_REGISTER_FUNC(SetRandMissionData)
		LUABRIDGE_REGISTER_FUNC(GetRandMission)
		LUABRIDGE_REGISTER_FUNC(GetRandMissionData)
		LUABRIDGE_REGISTER_FUNC(HasRandMissionNpc)
		LUABRIDGE_REGISTER_FUNC(HasRandNpcItemFlag)
		LUABRIDGE_REGISTER_FUNC(NoRandNpcItemFlag)
		LUABRIDGE_REGISTER_FUNC(TakeRandNpcItem)
		LUABRIDGE_REGISTER_FUNC(TakeAllRandItem)

		// Random mission counts
		LUABRIDGE_REGISTER_FUNC(CompleteRandMissionCount)
		LUABRIDGE_REGISTER_FUNC(FailureRandMissionCount)
		LUABRIDGE_REGISTER_FUNC(AddRandMissionNum)
		LUABRIDGE_REGISTER_FUNC(ResetRandMissionCount)
		LUABRIDGE_REGISTER_FUNC(ResetRandMissionNum)
		LUABRIDGE_REGISTER_FUNC(HasRandMissionCount)
		LUABRIDGE_REGISTER_FUNC(GetRandMissionCount)
		LUABRIDGE_REGISTER_FUNC(GetRandMissionNum)

		// Restore
		LUABRIDGE_REGISTER_FUNC(ReAll)
		LUABRIDGE_REGISTER_FUNC(ReAllHp)
		LUABRIDGE_REGISTER_FUNC(ReHp)
		LUABRIDGE_REGISTER_FUNC(ReAllSp)
		LUABRIDGE_REGISTER_FUNC(ReSp)

		// Spawn
		LUABRIDGE_REGISTER_FUNC(SetSpawnPos)
		LUABRIDGE_REGISTER_FUNC(IsSpawnPos)

		// Profession
		LUABRIDGE_REGISTER_FUNC(SetProfession)

		// Visibility
		LUABRIDGE_REGISTER_FUNC(Hide)
		LUABRIDGE_REGISTER_FUNC(Show)

		// Checks
		LUABRIDGE_REGISTER_FUNC(GetCharMissionLevel)
		LUABRIDGE_REGISTER_FUNC(LvEqual)
		LUABRIDGE_REGISTER_FUNC(LvThan)
		LUABRIDGE_REGISTER_FUNC(HpEqual)
		LUABRIDGE_REGISTER_FUNC(HpThan)
		LUABRIDGE_REGISTER_FUNC(SpEqual)
		LUABRIDGE_REGISTER_FUNC(SpThan)
		LUABRIDGE_REGISTER_FUNC(PfEqual)
		LUABRIDGE_REGISTER_FUNC(LvCheck)
		LUABRIDGE_REGISTER_FUNC(HpCheck)
		LUABRIDGE_REGISTER_FUNC(SpCheck)
		LUABRIDGE_REGISTER_FUNC(IsCategory)
		LUABRIDGE_REGISTER_FUNC(HasFame)

		// Info
		LUABRIDGE_REGISTER_FUNC(GetProfession)
		LUABRIDGE_REGISTER_FUNC(GetCategory)
		LUABRIDGE_REGISTER_FUNC(GetCatAndPf)
		LUABRIDGE_REGISTER_FUNC(GetChaBody)

		// NPC
		LUABRIDGE_REGISTER_FUNC(ConvoyNpc)
		LUABRIDGE_REGISTER_FUNC(ClearConvoyNpc)
		LUABRIDGE_REGISTER_FUNC(ClearAllConvoyNpc)
		LUABRIDGE_REGISTER_FUNC(HasConvoyNpc)
		LUABRIDGE_REGISTER_FUNC(IsConvoyNpc)

		LUABRIDGE_REGISTER_FUNC(SetPkState)

		// Save
		LUABRIDGE_REGISTER_FUNC(SaveMissionData)

		// Guild
		LUABRIDGE_REGISTER_FUNC(HasGuild)
		LUABRIDGE_REGISTER_FUNC(CreateGuild)
		LUABRIDGE_REGISTER_FUNC(ListAllGuild)
		LUABRIDGE_REGISTER_FUNC(ListChallenge)
		LUABRIDGE_REGISTER_FUNC(HasGuildLevel)
		LUABRIDGE_REGISTER_FUNC(HasPirateGuild)
		LUABRIDGE_REGISTER_FUNC(NoPirateGuild)
		LUABRIDGE_REGISTER_FUNC(HasNavyGuild)
		LUABRIDGE_REGISTER_FUNC(NoNavyGuild)

		// Boat
		LUABRIDGE_REGISTER_FUNC(IsBoatFull)
		LUABRIDGE_REGISTER_FUNC(CreateBoat)
		LUABRIDGE_REGISTER_FUNC(BoatLuanchOut)
		LUABRIDGE_REGISTER_FUNC(BoatBerth)
		LUABRIDGE_REGISTER_FUNC(BoatBerthList)
		LUABRIDGE_REGISTER_FUNC(BoatTrade)
		LUABRIDGE_REGISTER_FUNC(BoatBuildCheck)
		LUABRIDGE_REGISTER_FUNC(HasAllBoatInBerth)
		LUABRIDGE_REGISTER_FUNC(HasBoatInBerth)
		LUABRIDGE_REGISTER_FUNC(HasDeadBoatInBerth)
		LUABRIDGE_REGISTER_FUNC(HasLuanchOut)
		LUABRIDGE_REGISTER_FUNC(GetBoatID)
		LUABRIDGE_REGISTER_FUNC(RepairBoat)
		LUABRIDGE_REGISTER_FUNC(SupplyBoat)
		LUABRIDGE_REGISTER_FUNC(IsNeedSupply)
		LUABRIDGE_REGISTER_FUNC(IsNeedRepair)

		// Trade
		LUABRIDGE_REGISTER_FUNC(AdjustTradeItemCess)
		LUABRIDGE_REGISTER_FUNC(SetTradeItemLevel)
		LUABRIDGE_REGISTER_FUNC(TradeItemLevelCheck)
		LUABRIDGE_REGISTER_FUNC(GetTradeItemData)
		LUABRIDGE_REGISTER_FUNC(TradeItemDataCheck)

		// Pack
		LUABRIDGE_REGISTER_FUNC(PackBag)
		LUABRIDGE_REGISTER_FUNC(PackBagList)

		// Sync
		LUABRIDGE_REGISTER_FUNC(SetAttrChangeFlag)
		LUABRIDGE_REGISTER_FUNC(SyncBoat)
		LUABRIDGE_REGISTER_FUNC(SyncChar)

		// Open UI
		LUABRIDGE_REGISTER_FUNC(OpenBank)
		LUABRIDGE_REGISTER_FUNC(OpenRepair)
		LUABRIDGE_REGISTER_FUNC(OpenForge)
		LUABRIDGE_REGISTER_FUNC(OpenLottery)
		LUABRIDGE_REGISTER_FUNC(GetTicketIssue)
		// GetTicketItemno registered as raw above
		LUABRIDGE_REGISTER_FUNC(GetSItemGrid)
		LUABRIDGE_REGISTER_FUNC(OpenUnite)
		LUABRIDGE_REGISTER_FUNC(OpenMilling)
		LUABRIDGE_REGISTER_FUNC(OpenFusion)
		LUABRIDGE_REGISTER_FUNC(OpenUpgrade)
		LUABRIDGE_REGISTER_FUNC(OpenEidolonMetempsychosis)
		LUABRIDGE_REGISTER_FUNC(OpenEidolonFusion)
		LUABRIDGE_REGISTER_FUNC(OpenItemTiChun)
		LUABRIDGE_REGISTER_FUNC(OpenItemFix)
		LUABRIDGE_REGISTER_FUNC(OpenItemEnergy)
		LUABRIDGE_REGISTER_FUNC(OpenGetStone)
		LUABRIDGE_REGISTER_FUNC(OpenTiger)

		// Hair
		LUABRIDGE_REGISTER_FUNC(OpenHair)

		// Fusion
		LUABRIDGE_REGISTER_FUNC(CheckFusionItem)
		LUABRIDGE_REGISTER_FUNC(FusionItem)

		// Team
		LUABRIDGE_REGISTER_FUNC(IsTeamLeader)
		LUABRIDGE_REGISTER_FUNC(IsInTeam)
		LUABRIDGE_REGISTER_FUNC(HasTeammate)

		// Credit
		LUABRIDGE_REGISTER_FUNC(AddCreditX)
		LUABRIDGE_REGISTER_FUNC(AddMasterCredit)
		LUABRIDGE_REGISTER_FUNC(DelCredit)
		LUABRIDGE_REGISTER_FUNC(GetCredit)
		LUABRIDGE_REGISTER_FUNC(HasMaster)

		// Map player
		LUABRIDGE_REGISTER_FUNC(GetMapPlayer)
		LUABRIDGE_REGISTER_FUNC(DealAllPlayerInMap)
		LUABRIDGE_REGISTER_FUNC(GetMapActivePlayer)
		LUABRIDGE_REGISTER_FUNC(DealAllActivePlayerInMap)
		LUABRIDGE_REGISTER_FUNC(SetCopySpecialInter)

		// Garner
		LUABRIDGE_REGISTER_FUNC(Garner2GetWiner)
		LUABRIDGE_REGISTER_FUNC(Garner2RequestReorder)
		LUABRIDGE_REGISTER_FUNC(ClearAllSubMapCha)
		LUABRIDGE_REGISTER_FUNC(ClearAllSubMapMonster)
		LUABRIDGE_REGISTER_FUNC(IsGarnerWiner)

		// Skills
		LUABRIDGE_REGISTER_FUNC(LifeSkillBegin)
		LUABRIDGE_REGISTER_FUNC(ClearAllFightSkill)
		LUABRIDGE_REGISTER_FUNC(ClearFightSkill)

		LUABRIDGE_REGISTER_FUNC(RefreshCha)
		LUABRIDGE_REGISTER_FUNC(IsChaStall)
		LUABRIDGE_REGISTER_FUNC(ChangeJob)

		LUABRIDGE_REGISTER_FUNC(ListAuction)
		LUABRIDGE_REGISTER_FUNC(StartAuction)
		LUABRIDGE_REGISTER_FUNC(EndAuction)

		LUABRIDGE_REGISTER_FUNC(GetActName)
		LUABRIDGE_REGISTER_FUNC(GetActID)
		LUABRIDGE_REGISTER_FUNC(GetExpState)

		LUABRIDGE_REGISTER_FUNC(KillCha)

		LUABRIDGE_REGISTER_FUNC(GetGmLv)
		LUABRIDGE_REGISTER_FUNC(SetGmLv)
		// GetPlayerByName registered as raw above
		LUABRIDGE_REGISTER_FUNC(GetMasterID)
		// GetRoleByID registered as raw above
		// DealAllInGuild registered as raw above
		LUABRIDGE_REGISTER_FUNC(GetItemQuantity)
		LUABRIDGE_REGISTER_FUNC(GetOriginalChaTypeID)
		LUABRIDGE_REGISTER_FUNC(TransformCha)
		// GetOnlineCount registered as raw above
		LUABRIDGE_REGISTER_FUNC(PopupNotice)
		LUABRIDGE_REGISTER_FUNC(BanActRole)
		LUABRIDGE_REGISTER_FUNC(BanActName)
		LUABRIDGE_REGISTER_FUNC(UnbanAct)
		LUABRIDGE_REGISTER_FUNC(ColourNotice)
		LUABRIDGE_REGISTER_FUNC(RequestClientPin)

		LUABRIDGE_REGISTER_FUNC(OpenGuildBank)

		// String2Item registered as raw above
		// GetPlayerByActName registered as raw above
		LUABRIDGE_REGISTER_FUNC(ForcePlayerSave)
		LUABRIDGE_REGISTER_FUNC(SendDailyBuffInfo)
		LUABRIDGE_REGISTER_FUNC(SetGlobalRates)
		LUABRIDGE_REGISTER_FUNC(GetPlyDropRate)
		LUABRIDGE_REGISTER_FUNC(GetPlyExpRate)

		LUABRIDGE_REGISTER_FUNC(RemoveOfflineMode);

	// AI
	RegisterLuaAI(g_pLuaState);

	// Game logic
	RegisterLuaGameLogic(g_pLuaState);

	return TRUE;
}
