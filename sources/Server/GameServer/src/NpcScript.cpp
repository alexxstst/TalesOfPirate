// NpcScript.cpp Created by knight-gongjian 2004.11.30.
//---------------------------------------------------------
#include "stdafx.h"
#include "NpcScript.h"
#include "Npc.h"
#include "WorldEudemon.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include <NpcRecord.h>
#include <CharacterRecord.h>
#include <assert.h>
#include "lua_gamectrl.h"
//---------------------------------------------------------
_DBC_USING
using namespace mission;

net::WPacket g_WritePacket;

// Packet I/O

net::WPacket* GetPacket()
{
	g_WritePacket = g_gmsvr->GetWPacket();
	return &g_WritePacket;
}

int ReadByte(net::RPacket* pPacket)
{
	if (!pPacket) return -1;
	return static_cast<BYTE>((*pPacket).ReadInt64());
}

int ReadWord(net::RPacket* pPacket)
{
	if (!pPacket) return -1;
	return static_cast<WORD>((*pPacket).ReadInt64());
}

int ReadDword(net::RPacket* pPacket)
{
	if (!pPacket) return -1;
	return static_cast<int>((*pPacket).ReadInt64());
}

std::string ReadString(net::RPacket* pPacket)
{
	if (!pPacket) return "Ineffective char pointer";
	std::string pszData = (*pPacket).ReadString();
	return (!pszData.empty()) ? pszData : "Ineffective char pointer";
}

int ReadCmd(net::RPacket* pPacket)
{
	if (!pPacket) return -1;
	return static_cast<WORD>((*pPacket).ReadCmd());
}

void WriteByte(net::WPacket* pPacket, int byData)
{
	if (!pPacket) return;
	(*pPacket).WriteInt64(static_cast<BYTE>(byData));
}

void WriteWord(net::WPacket* pPacket, int wData)
{
	if (!pPacket) return;
	(*pPacket).WriteInt64(static_cast<WORD>(wData));
}

void WriteDword(net::WPacket* pPacket, int dwData)
{
	if (!pPacket) return;
	(*pPacket).WriteInt64(static_cast<DWORD>(dwData));
}

void WriteString(net::WPacket* pPacket, const std::string& pszData)
{
	if (!pPacket) return;
	(*pPacket).WriteString(pszData.c_str());
}

void WriteCmd(net::WPacket* pPacket, int wData)
{
	if (!pPacket) return;
	(*pPacket).WriteCmd(static_cast<WORD>(wData));
}

void SendPacket(CCharacter* pChar, net::WPacket* pPacket)
{
	if (!pChar || !pPacket) return;
	pChar->ReflectINFof(pChar, *pPacket);
}

void SynPacket(CCharacter* pChar, net::WPacket* pPacket)
{
	if (!pChar || !pPacket) return;
	pChar->NotiChgToEyeshot(*pPacket);
}

// Character info

int GetCharID(CCharacter* pChar)
{
	if (!pChar) return 0;
	return pChar->GetID();
}

std::string GetCharName(CCharacter* pChar)
{
	if (!pChar) return "";
	return pChar->GetName();
}

std::string GetMonsterName(int sMonsterID)
{
	char szName[64];
	strncpy(szName, RES_STRING(GM_NPCSCRIPT_CPP_00001), 64 - 1);

	CChaRecord* pRec = GetChaRecordInfo(static_cast<USHORT>(sMonsterID));
	if (pRec)
	{
		strncpy(szName, pRec->szName, 64 - 1);
	}

	return szName;
}

std::string GetItemName(int sItemID)
{
	char szItem[64];
	strncpy(szItem, RES_STRING(GM_NPCSCRIPT_CPP_00001), 64 - 1);

	CItemRecord* pRec = GetItemRecordInfo(static_cast<USHORT>(sItemID));
	if (pRec)
	{
		strncpy(szItem, pRec->szName, 64 - 1);
	}

	return szItem;
}

std::string GetAreaName(int sAreaID)
{
	char szArea[128];
	strncpy(szArea, RES_STRING(GM_NPCSCRIPT_CPP_00002), 128 - 1);

	CAreaInfo* pInfo = (CAreaInfo*)GetAreaInfo(static_cast<USHORT>(sAreaID));
	if (pInfo)
	{
		strncpy(szArea, pInfo->szDataName, 128 - 1);
	}

	return szArea;
}

std::string GetMapName(CCharacter* pChar)
{
	if (!pChar) return "";
	SubMap* pMap = pChar->GetSubMap();
	if (!pMap) return "";
	return pMap->GetName();
}

// Movement

int MoveTo(CCharacter* pChar, int x, int y, const std::string& pszData)
{
	if (!pChar) return LUA_FALSE;
	long lx = static_cast<long>(x) * 100;
	long ly = static_cast<long>(y) * 100;
	pChar->SwitchMap(pChar->GetSubMap(), pszData.c_str(), lx, ly);
	return LUA_TRUE;
}

// MoveCity — variable args (2 or 3), kept as lua_CFunction
int MoveCity_raw(lua_State* L)
{
	int nParamNum = lua_gettop(L);
	BOOL bValid = FALSE;
	if (nParamNum == 2)
	{
		if (lua_islightuserdata(L, 1) && lua_isstring(L, 2))
			bValid = TRUE;
	}
	else if (nParamNum == 3)
	{
		if (lua_islightuserdata(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3))
			bValid = TRUE;
	}
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	CCharacter* pChar = (CCharacter*)lua_touserdata(L, 1);
	const char* pszData = lua_tostring(L, 2);
	if (!pChar)
	{
		E_LUANULL;
		return 0;
	}
	Long lMapCpyNO = 0;
	if (nParamNum == 3)
		lMapCpyNO = (Long)lua_tonumber(L, 3);
	lMapCpyNO -= 1;

	pChar->MoveCity(pszData, lMapCpyNO);

	lua_pushnumber(L, LUA_TRUE);
	return 1;
}

// Utility

int Rand(int dwMax)
{
	return (dwMax > 0) ? rand() % dwMax : 0;
}

void DebugInfo(int dwData)
{
	// no-op
}

// Notices

void BickerNotice(CCharacter* pChar, const std::string& pszData)
{
	if (!pChar) return;
	pChar->BickerNotice(pszData.c_str());
}

void SystemNotice(CCharacter* pChar, const std::string& pszData)
{
	if (!pChar) return;
	pChar->SystemNotice(pszData.c_str());
}

void SynTigerString(CCharacter* pChar, const std::string& pszData)
{
	if (!pChar) return;
	pChar->SynTigerString(pszData.c_str());
}

// Trading

std::tuple<int, int> SafeBuy(CCharacter* pChar, int wItemID, int byIndex, int byCount)
{
	if (!pChar) return {LUA_FALSE, 0};
	DWORD dwMoney;
	BOOL bRet = pChar->SafeBuy(static_cast<WORD>(wItemID), static_cast<BYTE>(byCount), static_cast<BYTE>(byIndex), dwMoney);
	return {bRet ? LUA_TRUE : LUA_FALSE, static_cast<int>(dwMoney)};
}

int ExchangeReq(CCharacter* pChar, CNpc* pNpc, int sSrcID, int sSrcNum, int sTarID, int sTarNum, int sTimeNum)
{
	pChar->ExchangeReq(static_cast<short>(sSrcID), static_cast<short>(sSrcNum), static_cast<short>(sTarID), static_cast<short>(sTarNum));
	return LUA_TRUE;
}

std::tuple<int, int, int> SafeSale(CCharacter* pChar, int byIndex, int byCount)
{
	if (!pChar) return {LUA_FALSE, 0, 0};
	WORD wItemID;
	DWORD dwMoney;
	BOOL bRet = pChar->SafeSale(static_cast<BYTE>(byIndex), static_cast<BYTE>(byCount), wItemID, dwMoney);
	return {bRet ? LUA_TRUE : LUA_FALSE, static_cast<int>(wItemID), static_cast<int>(dwMoney)};
}

std::tuple<int, int, int> SafeSaleGoods(CCharacter* pChar, int dwBoatID, int byIndex, int byCount, int dwMoney)
{
	if (!pChar) return {LUA_FALSE, 0, 0};
	WORD wItemID;
	DWORD dwMoneyOut = static_cast<DWORD>(dwMoney);
	BOOL bRet = pChar->SafeSaleGoods(static_cast<DWORD>(dwBoatID), static_cast<BYTE>(byIndex), static_cast<BYTE>(byCount), wItemID, dwMoneyOut);
	return {bRet ? LUA_TRUE : LUA_FALSE, static_cast<int>(wItemID), static_cast<int>(dwMoneyOut)};
}

std::tuple<int, int> SafeBuyGoods(CCharacter* pChar, int dwBoatID, int wItemID, int byIndex, int byCount, int dwMoney)
{
	if (!pChar) return {LUA_FALSE, 0};
	DWORD dwMoneyOut = static_cast<DWORD>(dwMoney);
	BOOL bRet = pChar->SafeBuyGoods(static_cast<DWORD>(dwBoatID), static_cast<WORD>(wItemID), static_cast<BYTE>(byCount), static_cast<BYTE>(byIndex), dwMoneyOut);
	return {bRet ? LUA_TRUE : LUA_FALSE, static_cast<int>(dwMoneyOut)};
}

std::tuple<int, int> GetSaleGoodsItem(CCharacter* pChar, int dwBoatID, int byIndex)
{
	if (!pChar) return {LUA_FALSE, 0};
	WORD wItemID;
	BOOL bRet = pChar->GetSaleGoodsItem(static_cast<DWORD>(dwBoatID), static_cast<BYTE>(byIndex), wItemID);
	return {bRet ? LUA_TRUE : LUA_FALSE, static_cast<int>(wItemID)};
}

// NPC management

void SetNpcScriptID(CNpc* pNpc, int sID)
{
	if (!pNpc) return;
	pNpc->SetScriptID(static_cast<USHORT>(sID));
}

std::tuple<int, int> GetScriptID(CNpc* pNpc)
{
	if (!pNpc) return {LUA_FALSE, 0};
	USHORT sID = pNpc->GetScriptID();
	return {(sID != static_cast<USHORT>(-1)) ? LUA_TRUE : LUA_FALSE, static_cast<int>(sID)};
}

std::tuple<int, CNpc*, int> FindNpc(const std::string& pszNpc)
{
	CNpc* pNpc = NULL;
	try { pNpc = g_pGameApp->FindNpc(pszNpc.c_str()); }
	catch (...) { ToLogService("common", "findnpc errorexception!"); }

	if (!pNpc)
	{
		return {LUA_FALSE, nullptr, 0};
	}

	USHORT sID = pNpc->GetScriptID();
	return {(sID != static_cast<USHORT>(-1)) ? LUA_TRUE : LUA_FALSE, pNpc, static_cast<int>(sID)};
}

void ReloadNpcInfo()
{
	//LoadScript();
	//if( g_pGameApp->ReloadNpcInfo( *this ) )
	//{
	//}
}

void SetNpcHasMission(CNpc* pNpc, int byRet)
{
	if (!pNpc) return;
	pNpc->SetNpcHasMission(static_cast<BYTE>(byRet));
}

int GetNpcHasMission(CNpc* pNpc)
{
	if (!pNpc) return LUA_FALSE;
	return (pNpc->GetNpcHasMission()) ? LUA_TRUE : LUA_FALSE;
}

// Area / Map checks

int IsInArea(CCharacter* pChar, int sAreaID)
{
	if (!pChar) return LUA_FALSE;
	BOOL bRet = (USHORT)pChar->GetIslandID() == static_cast<USHORT>(sAreaID);
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsInMap(CCharacter* pChar, const std::string& pszMap, int dwxPos, int dwyPos, int wWith, int wHeight)
{
	if (!pChar || !pChar->GetSubMap()) return LUA_FALSE;
	BOOL bRet = strcmp(pszMap.c_str(), pChar->GetSubMap()->GetName()) == 0;
	if (bRet)
	{
		const Point& pt = pChar->GetPos();
		if ((DWORD)pt.x > (DWORD)dwxPos && (DWORD)pt.y > (DWORD)dwyPos &&
			(DWORD)pt.x < (DWORD)dwxPos + (WORD)wWith && (DWORD)pt.y < (DWORD)dwyPos + (WORD)wHeight)
			bRet = TRUE;
	}
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int IsMapChar(CCharacter* pChar, const std::string& pszMap)
{
	if (!pChar || !pChar->GetSubMap()) return LUA_FALSE;
	return (strcmp(pszMap.c_str(), pChar->GetSubMap()->GetName()) == 0) ? LUA_TRUE : LUA_FALSE;
}

int IsMapNpc(CNpc* pNpc, const std::string& pszMap, int sNpcID)
{
	if (!pNpc) return LUA_FALSE;
	return (pNpc->IsMapNpc(pszMap.c_str(), static_cast<USHORT>(sNpcID))) ? LUA_TRUE : LUA_FALSE;
}

int AddNpcTrigger(CNpc* pNpc, int wID, int e, int wParam1, int wParam2, int wParam3, int wParam4)
{
	if (!pNpc) return LUA_FALSE;
	BOOL bRet = pNpc->AddNpcTrigger(
		static_cast<WORD>(wID), static_cast<mission::TRIGGER_EVENT>(e),
		static_cast<WORD>(wParam1), static_cast<WORD>(wParam2),
		static_cast<WORD>(wParam3), static_cast<WORD>(wParam4));
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int SetActive(CNpc* pNpc)
{
	if (!pNpc) return LUA_FALSE;
	pNpc->SetEyeshotAbility(true);
	return LUA_TRUE;
}

std::tuple<int, CWorldEudemon*> GetEudemon()
{
	return {LUA_TRUE, &g_WorldEudemon};
}

int SummonNpc(int byMapID, int sAreaID, const std::string& pszNpc, int sTime)
{
	BOOL bRet = g_pGameApp->SummonNpc(
		static_cast<BYTE>(byMapID), static_cast<USHORT>(sAreaID),
		pszNpc.c_str(), static_cast<USHORT>(sTime));
	return bRet ? LUA_TRUE : LUA_FALSE;
}

int ChaPlayEffect(CCharacter* pChar, int nEffectID)
{
	if (!pChar) return LUA_FALSE;

	auto l_wpk = net::msg::serialize(net::msg::McChaPlayEffectMessage{pChar->GetID(), (int64_t)nEffectID});
	pChar->NotiChgToEyeshot(l_wpk);
	return LUA_TRUE;
}

inline int RegisterNpcScript()
{
	lua_State* L = g_pLuaState;

	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(ReloadNpcInfo)
		LUABRIDGE_REGISTER_FUNC(FindNpc)

		// Packet read
		LUABRIDGE_REGISTER_FUNC(ReadCmd)
		LUABRIDGE_REGISTER_FUNC(ReadByte)
		LUABRIDGE_REGISTER_FUNC(ReadWord)
		LUABRIDGE_REGISTER_FUNC(ReadDword)
		LUABRIDGE_REGISTER_FUNC(ReadString)

		// Packet write
		LUABRIDGE_REGISTER_FUNC(WriteCmd)
		LUABRIDGE_REGISTER_FUNC(WriteByte)
		LUABRIDGE_REGISTER_FUNC(WriteWord)
		LUABRIDGE_REGISTER_FUNC(WriteDword)
		LUABRIDGE_REGISTER_FUNC(WriteString)

		// Packet send
		LUABRIDGE_REGISTER_FUNC(GetPacket)
		LUABRIDGE_REGISTER_FUNC(SendPacket)
		LUABRIDGE_REGISTER_FUNC(SynPacket)

		// Character info
		LUABRIDGE_REGISTER_FUNC(GetCharID)
		LUABRIDGE_REGISTER_FUNC(MoveTo)
		LUABRIDGE_REGISTER_FUNC(GetAreaName)
		LUABRIDGE_REGISTER_FUNC(GetMapName)
		LUABRIDGE_REGISTER_FUNC(GetCharName)
		LUABRIDGE_REGISTER_FUNC(GetItemName)
		LUABRIDGE_REGISTER_FUNC(GetMonsterName)
		LUABRIDGE_REGISTER_FUNC(Rand)

		// Trading
		LUABRIDGE_REGISTER_FUNC(SafeSale)
		LUABRIDGE_REGISTER_FUNC(SafeBuy)
		LUABRIDGE_REGISTER_FUNC(SafeSaleGoods)
		LUABRIDGE_REGISTER_FUNC(SafeBuyGoods)
		LUABRIDGE_REGISTER_FUNC(GetSaleGoodsItem)
		LUABRIDGE_REGISTER_FUNC(ExchangeReq)

		// NPC management
		LUABRIDGE_REGISTER_FUNC(SetNpcScriptID)
		LUABRIDGE_REGISTER_FUNC(GetScriptID)
		LUABRIDGE_REGISTER_FUNC(SetNpcHasMission)
		LUABRIDGE_REGISTER_FUNC(GetNpcHasMission)

		// Area / Map
		LUABRIDGE_REGISTER_FUNC(IsMapChar)
		LUABRIDGE_REGISTER_FUNC(IsMapNpc)
		LUABRIDGE_REGISTER_FUNC(IsInMap)
		LUABRIDGE_REGISTER_FUNC(IsInArea)

		// NPC triggers & misc
		LUABRIDGE_REGISTER_FUNC(AddNpcTrigger)
		LUABRIDGE_REGISTER_FUNC(SetActive)
		LUABRIDGE_REGISTER_FUNC(GetEudemon)
		LUABRIDGE_REGISTER_FUNC(SummonNpc)
		LUABRIDGE_REGISTER_FUNC(ChaPlayEffect)

		// Notices
		LUABRIDGE_REGISTER_FUNC(DebugInfo)
		LUABRIDGE_REGISTER_FUNC(SystemNotice)
		LUABRIDGE_REGISTER_FUNC(SynTigerString)
		LUABRIDGE_REGISTER_FUNC(BickerNotice);

	// Variable args — kept as lua_CFunction
	lua_register(L, "MoveCity", MoveCity_raw);

	return TRUE;
}
