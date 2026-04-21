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
#include "AreaRecordStore.h"
#include "lua_gamectrl.h"
//---------------------------------------------------------
_DBC_USING
using namespace mission;

//   trade- (   MsgProc )
void LogTrade(const std::string& msg)
{
	ToLogService("trade", "{}", msg);
}

// ============================================================
//       Lua-
// ============================================================

//      NPC  (CTalkNpc::MsgProc).
//   Lua    .
//    action:
//    usCmd                ( 1: 1 )
//   (CMD_CM_TALKPAGE)      talkId
//   (CMD_CM_FUNCITEM)      page, item
//   (BLACKMARKET_EXCHANGE_REQ) timeNum, srcId, srcNum, tarId, tarNum, idx
//   (CMD_CM_TRADEITEM)     tradeType, +  :
//        SALE (0)         → index, count
//        BUY (1)          → itemType, index1, index2, count
//        SALE_GOODS (2)   → boatId, index, count
//        BUY_GOODS (3)    → boatId, itemType, index1, index2, count
//        SELECT_BOAT (4)  → index
//   (CMD_CM_MISSION)       misCmd, +  :
//        MIS_SEL (4)           → selIndex
//        MIS_BTNDELIVERY (7)   → param1, param2
luabridge::LuaRef BuildNpcActionTable(lua_State* L, net::RPacket& pk)
{
	luabridge::LuaRef a = luabridge::newTable(L);
	const int64_t usCmd = pk.ReadInt64();
	a["usCmd"] = usCmd;

	constexpr int CMD_CM_TALKPAGE_              = 302;
	constexpr int CMD_CM_FUNCITEM_              = 303;
	constexpr int CMD_CM_TRADEITEM_             = 309;
	constexpr int CMD_CM_MISSION_               = 322;
	constexpr int CMD_CM_BLACKMARKET_EXCHANGE_REQ_ = 51;

	switch (static_cast<int>(usCmd)) {
	case CMD_CM_TALKPAGE_: {
		a["talkId"] = pk.ReadInt64();
		break;
	}
	case CMD_CM_FUNCITEM_: {
		a["page"] = pk.ReadInt64();
		a["item"] = pk.ReadInt64();
		break;
	}
	case CMD_CM_BLACKMARKET_EXCHANGE_REQ_: {
		a["timeNum"] = pk.ReadInt64();
		a["srcId"]   = pk.ReadInt64();
		a["srcNum"]  = pk.ReadInt64();
		a["tarId"]   = pk.ReadInt64();
		a["tarNum"]  = pk.ReadInt64();
		a["idx"]     = pk.ReadInt64();
		break;
	}
	case CMD_CM_TRADEITEM_: {
		const int64_t tradeType = pk.ReadInt64();
		a["tradeType"] = tradeType;
		switch (static_cast<int>(tradeType)) {
		case 0: // ROLE_TRADE_SALE
			a["index"] = pk.ReadInt64();
			a["count"] = pk.ReadInt64();
			break;
		case 1: // ROLE_TRADE_BUY
			a["itemType"] = pk.ReadInt64();
			a["index1"]   = pk.ReadInt64();
			a["index2"]   = pk.ReadInt64();
			a["count"]    = pk.ReadInt64();
			break;
		case 2: // ROLE_TRADE_SALE_GOODS
			a["boatId"] = pk.ReadInt64();
			a["index"]  = pk.ReadInt64();
			a["count"]  = pk.ReadInt64();
			break;
		case 3: // ROLE_TRADE_BUY_GOODS
			a["boatId"]   = pk.ReadInt64();
			a["itemType"] = pk.ReadInt64();
			a["index1"]   = pk.ReadInt64();
			a["index2"]   = pk.ReadInt64();
			a["count"]    = pk.ReadInt64();
			break;
		case 4: // ROLE_TRADE_SELECT_BOAT
			a["index"] = pk.ReadInt64();
			break;
		default:
			break;
		}
		break;
	}
	case CMD_CM_MISSION_: {
		const int64_t misCmd = pk.ReadInt64();
		a["misCmd"] = misCmd;
		switch (static_cast<int>(misCmd)) {
		case 4: // ROLE_MIS_SEL
			a["selIndex"] = pk.ReadInt64();
			break;
		case 7: // ROLE_MIS_BTNDELIVERY
			a["param1"] = pk.ReadInt64();
			a["param2"] = pk.ReadInt64();
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

	return a;
}

//      Store-   Lua-.
//    :
//    cmd          (STORE_OPEN_ASK, STORE_BUY_ASK, STORE_LIST_ASK, STORE_CLOSE)
//    STORE_BUY_ASK     → id
//    STORE_LIST_ASK    → clsId, page, num
luabridge::LuaRef BuildStoreActionTable(lua_State* L, net::RPacket& pk)
{
	luabridge::LuaRef a = luabridge::newTable(L);
	const int64_t cmd = static_cast<int64_t>(pk.ReadCmd());
	a["cmd"] = cmd;

	constexpr int CMD_CM_STORE_BUY_ASK_  = 43;
	constexpr int CMD_CM_STORE_LIST_ASK_ = 42;

	switch (static_cast<int>(cmd)) {
	case CMD_CM_STORE_BUY_ASK_:
		a["id"] = pk.ReadInt64();
		break;
	case CMD_CM_STORE_LIST_ASK_:
		a["clsId"] = pk.ReadInt64();
		a["page"]  = pk.ReadInt64();
		a["num"]   = pk.ReadInt64();
		break;
	default:
		break;
	}
	return a;
}

// ============================================================
//     NPC ( Lua)
// ============================================================

namespace {
//  :  LuaRef
int64_t lua_as_int(luabridge::LuaRef ref, int64_t defaultValue = 0) {
	if (!ref.isNumber()) return defaultValue;
	return ref.cast<int64_t>().valueOr(defaultValue);
}
std::string lua_as_string(luabridge::LuaRef ref, const char* defaultValue = "") {
	if (!ref.isString()) return defaultValue;
	return ref.cast<std::string>().valueOr(defaultValue);
}
} //  anonymous

// -----------------------------------------------------------
//  CMD_MC_TALKPAGE    (  SendTalkPage / SendDebugPage)
// -----------------------------------------------------------
void ShowTalkPage(CCharacter* cha, CCharacter* npc, int pageId, const std::string& text)
{
	if (!cha || !npc) return;
	net::msg::McTalkInfoMessage msg;
	msg.npcId = static_cast<int64_t>(npc->GetID());
	msg.cmd   = static_cast<int64_t>(pageId);
	msg.text  = text;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_CLOSETALK
// -----------------------------------------------------------
void ShowClosePage(CCharacter* cha, CCharacter* npc)
{
	if (!cha || !npc) return;
	net::msg::McCloseTalkMessage msg;
	msg.npcId = static_cast<int64_t>(npc->GetID());
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_FUNCPAGE —   NPC
//   (McFuncInfoMessage):
//   npcId, page, talkText, funcItems[name], missionItems[name, state]
// -----------------------------------------------------------
void ShowFuncPage(CCharacter* cha, CCharacter* npc, int pageId,
                  const std::string& talk, luabridge::LuaRef items)
{
	if (!cha || !npc) return;
	net::msg::McFuncInfoMessage msg;
	msg.npcId    = static_cast<int64_t>(npc->GetID());
	msg.page     = static_cast<int64_t>(pageId);
	msg.talkText = talk;

	if (items.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef it = items[i];
			if (it.isNil()) break;
			net::msg::FuncInfoFuncItem f;
			f.name = it.isString() ? it.cast<std::string>().valueOr("") : "Incorrect notice option!";
			msg.funcItems.push_back(std::move(f));
		}
	}
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

//     (  SendMissionPage).
//   Lua: { [1]={name=..., state=...}, ... }
void ShowMissionPage(CCharacter* cha, CCharacter* npc, int pageId,
                     const std::string& talk,
                     luabridge::LuaRef items, luabridge::LuaRef missions)
{
	if (!cha || !npc) return;
	net::msg::McFuncInfoMessage msg;
	msg.npcId    = static_cast<int64_t>(npc->GetID());
	msg.page     = static_cast<int64_t>(pageId);
	msg.talkText = talk;

	if (items.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef it = items[i];
			if (it.isNil()) break;
			net::msg::FuncInfoFuncItem f;
			f.name = it.isString() ? it.cast<std::string>().valueOr("") : "Incorrect notice option!";
			msg.funcItems.push_back(std::move(f));
		}
	}

	if (missions.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef m = missions[i];
			if (m.isNil()) break;
			if (!m.isTable()) continue;
			net::msg::FuncInfoMissionItem mi;
			mi.name  = lua_as_string(m["name"]);
			mi.state = lua_as_int(m["state"]);
			msg.missionItems.push_back(std::move(mi));
		}
	}

	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_HELPINFO  (text)
//   serialize( McHelpInfoMessage)    —
// -----------------------------------------------------------
void ShowHelpDesc(CCharacter* cha, int type, const std::string& info)
{
	if (!cha) return;
	net::WPacket pkt;
	pkt.WriteCmd(CMD_MC_HELPINFO);
	pkt.WriteInt64(static_cast<int64_t>(type));
	pkt.WriteString(info.c_str());
	cha->ReflectINFof(cha, pkt);
}

void ShowHelpSound(CCharacter* cha, int type, int soundId)
{
	if (!cha) return;
	net::WPacket pkt;
	pkt.WriteCmd(CMD_MC_HELPINFO);
	pkt.WriteInt64(static_cast<int64_t>(type));
	pkt.WriteInt64(static_cast<int64_t>(soundId));
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  Exchange-  ( BlackMarket / )
//   Lua:
//   ex.count
//   ex.srcid[n], ex.srcnum[n], ex.tarid[n], ex.tarnum[n] [,ex.timenum[n]]
// -----------------------------------------------------------
namespace {
int read_exchange_count(luabridge::LuaRef ex) {
	if (!ex.isTable()) return 0;
	luabridge::LuaRef c = ex["count"];
	if (!c.isNumber()) return 0;
	return c.cast<int>().valueOr(0);
}
template <typename Entry>
std::vector<Entry> read_exchange_entries(luabridge::LuaRef ex, bool withTime)
{
	std::vector<Entry> out;
	int n = read_exchange_count(ex);
	if (n <= 0) return out;
	luabridge::LuaRef srcid  = ex["srcid"];
	luabridge::LuaRef srcnum = ex["srcnum"];
	luabridge::LuaRef tarid  = ex["tarid"];
	luabridge::LuaRef tarnum = ex["tarnum"];
	luabridge::LuaRef tnum   = ex["timenum"];
	out.resize(static_cast<size_t>(n));
	for (int i = 0; i < n; ++i) {
		Entry& e = out[static_cast<size_t>(i)];
		e.srcId    = lua_as_int(srcid[i + 1]);
		e.srcCount = lua_as_int(srcnum[i + 1]);
		e.tarId    = lua_as_int(tarid[i + 1]);
		e.tarCount = lua_as_int(tarnum[i + 1]);
		if constexpr (requires { e.timeValue; }) {
			if (withTime) e.timeValue = lua_as_int(tnum[i + 1]);
		}
	}
	return out;
}
} //  anonymous

//  SendExchangeData:   (   Exchange)
void ShowBlackMarketExchange(CCharacter* cha, CCharacter* npc, luabridge::LuaRef ex)
{
	if (!cha || !npc) return;
	net::msg::McBlackMarketExchangeDataMessage msg;
	msg.npcId = static_cast<int64_t>(npc->GetID());
	msg.exchanges = read_exchange_entries<net::msg::BlackMarketExchangeEntry>(ex, /*withTime=*/true);
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

//  SendExchangeXData: ,    exchangeData  timenum
void ShowExchangeData(CCharacter* cha, CCharacter* npc, luabridge::LuaRef ex)
{
	if (!cha || !npc) return;
	net::msg::McExchangeDataMessage msg;
	msg.npcId = static_cast<int64_t>(npc->GetID());
	msg.exchanges = read_exchange_entries<net::msg::ExchangeEntry>(ex, /*withTime=*/false);
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_MISPAGE / CMD_MC_MISLOGINFO —
//
//    Lua "mission":
//   mission.need.count
//   mission.need[n].tp            (MIS_NEED_ITEM/KILL/DESP)
//   mission.need[n].p1            ( param1 ,    DESP — desp-string)
//   mission.need[n].p2
//   mission.need[n].p3            ( MIS_NEED_ITEM  KILL)
//   mission.prize.seltp
//   mission.prize.count
//   mission.prize[i].tp
//   mission.prize[i].p1, p2
// -----------------------------------------------------------
namespace {
std::vector<net::msg::MisNeedEntry> read_mis_needs(luabridge::LuaRef needT)
{
	std::vector<net::msg::MisNeedEntry> out;
	if (!needT.isTable()) return out;
	int count = static_cast<int>(lua_as_int(needT["count"]));
	out.reserve(static_cast<size_t>(count));
	for (int n = 1; n <= count; ++n) {
		luabridge::LuaRef r = needT[n];
		if (!r.isTable()) continue;
		net::msg::MisNeedEntry e{};
		e.needType = lua_as_int(r["tp"]);
		if (e.needType == mission::MIS_NEED_DESP) {
			e.desp = lua_as_string(r["p1"]);
		}
		else { // MIS_NEED_ITEM / MIS_NEED_KILL
			e.param1 = lua_as_int(r["p1"]);
			e.param2 = lua_as_int(r["p2"]);
			e.param3 = lua_as_int(r["p3"]);
		}
		out.push_back(std::move(e));
	}
	return out;
}

std::vector<net::msg::MisPrizeEntry> read_mis_prizes(luabridge::LuaRef prizeT, int64_t& outSelType)
{
	std::vector<net::msg::MisPrizeEntry> out;
	outSelType = 0;
	if (!prizeT.isTable()) return out;
	outSelType = lua_as_int(prizeT["seltp"]);
	int count = static_cast<int>(lua_as_int(prizeT["count"]));
	out.reserve(static_cast<size_t>(count));
	for (int i = 1; i <= count; ++i) {
		luabridge::LuaRef r = prizeT[i];
		if (!r.isTable()) continue;
		net::msg::MisPrizeEntry p{};
		p.type   = lua_as_int(r["tp"]);
		p.param1 = lua_as_int(r["p1"]);
		p.param2 = lua_as_int(r["p2"]);
		out.push_back(p);
	}
	return out;
}
} //  anonymous

void ShowMisPage(CCharacter* cha, int cmd, int npcId, const std::string& name,
                 luabridge::LuaRef needs, luabridge::LuaRef prize,
                 const std::string& description)
{
	if (!cha) return;
	net::msg::McMisPageMessage msg;
	msg.cmd   = static_cast<int64_t>(cmd);
	msg.npcId = static_cast<int64_t>(npcId);
	msg.name  = name;
	msg.needs = read_mis_needs(needs);
	msg.prizes = read_mis_prizes(prize, msg.prizeSelType);
	msg.description = description;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

void ShowMisLogInfo(CCharacter* cha, int misId, const std::string& name,
                    luabridge::LuaRef needs, luabridge::LuaRef prize,
                    const std::string& description)
{
	if (!cha) return;
	net::msg::McMisLogInfoMessage msg;
	msg.misId = static_cast<int64_t>(misId);
	msg.name  = name;
	msg.needs = read_mis_needs(needs);
	msg.prizes = read_mis_prizes(prize, msg.prizeSelType);
	msg.description = description;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_MISSION —
// -----------------------------------------------------------
void ShowMissionList(CCharacter* cha, CCharacter* npc, int listType,
                     int prev, int next, int prevCmd, int nextCmd,
                     luabridge::LuaRef items)
{
	if (!cha || !npc) return;
	net::msg::McMissionInfoMessage msg;
	msg.npcId    = static_cast<int64_t>(npc->GetID());
	msg.listType = static_cast<int64_t>(listType);
	msg.prev     = static_cast<int64_t>(prev);
	msg.next     = static_cast<int64_t>(next);
	msg.prevCmd  = static_cast<int64_t>(prevCmd);
	msg.nextCmd  = static_cast<int64_t>(nextCmd);
	if (items.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef it = items[i];
			if (it.isNil()) break;
			msg.items.push_back(it.isString() ? it.cast<std::string>().valueOr("") : "");
		}
	}
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_TRADE_DATA — broadcast      ( )
// -----------------------------------------------------------
// -----------------------------------------------------------
//  CMD_MC_UPDATEIMP —  IMP
// -----------------------------------------------------------
void ShowUpdateImp(CCharacter* cha, int imp)
{
	if (!cha) return;
	net::msg::McUpdateImpMessage msg;
	msg.imp = static_cast<int64_t>(imp);
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_STORE_BUY_ASR —
// -----------------------------------------------------------
void ShowStoreBuyResult(CCharacter* cha, bool success, int newMoney)
{
	if (!cha) return;
	net::msg::McStoreBuyAnswerMessage msg;
	msg.success  = success ? 1 : 0;
	msg.newMoney = static_cast<int64_t>(newMoney);
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_STORE_OPEN_ASR —    (, , , tabs)
//   tabs Lua: { {id=..., title=..., parent=...}, ... }
// -----------------------------------------------------------
void ShowStoreOpen(CCharacter* cha, int vip, int moBean, int replMoney, luabridge::LuaRef tabs)
{
	if (!cha) return;
	net::msg::McStoreOpenAnswerMessage msg;
	msg.isValid   = true;
	msg.vip       = static_cast<int64_t>(vip);
	msg.moBean    = static_cast<int64_t>(moBean);
	msg.replMoney = static_cast<int64_t>(replMoney);
	if (tabs.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef t = tabs[i];
			if (t.isNil()) break;
			if (!t.isTable()) continue;
			net::msg::StoreClassEntry c;
			c.classId   = lua_as_int(t["id"], i);
			c.className = lua_as_string(t["title"]);
			c.parentId  = lua_as_int(t["parent"]);
			msg.classes.push_back(std::move(c));
		}
	}
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_STORE_LIST_ASR —
//   products Lua:
//   { comId=..., title=..., price=..., remark=..., hot=..., time=...,
//     quantity=..., expire=..., items={ {id=..., num=..., flute=...,
//     attrs={ {id=..., val=...}, ... }}, ... } }
// -----------------------------------------------------------
void ShowStoreList(CCharacter* cha, int pageTotal, int pageCurrent, luabridge::LuaRef products)
{
	if (!cha) return;
	net::msg::McStoreListAnswerMessage msg;
	msg.pageTotal   = static_cast<int64_t>(pageTotal);
	msg.pageCurrent = static_cast<int64_t>(pageCurrent);
	if (products.isTable()) {
		for (int i = 1;; ++i) {
			luabridge::LuaRef p = products[i];
			if (p.isNil()) break;
			if (!p.isTable()) continue;
			net::msg::StoreProductEntry entry;
			entry.comId    = lua_as_int(p["comId"]);
			entry.comName  = lua_as_string(p["title"]);
			entry.price    = lua_as_int(p["price"]);
			entry.remark   = lua_as_string(p["remark"]);
			entry.isHot    = (lua_as_int(p["hot"]) != 0);
			entry.time     = lua_as_int(p["time"]);
			entry.quantity = lua_as_int(p["quantity"]);
			entry.expire   = lua_as_int(p["expire"]);

			luabridge::LuaRef items = p["items"];
			if (items.isTable()) {
				for (int j = 1;; ++j) {
					luabridge::LuaRef it = items[j];
					if (it.isNil()) break;
					net::msg::StoreVariantEntry v{};
					if (it.isTable()) {
						v.itemId  = lua_as_int(it["id"]);
						v.itemNum = lua_as_int(it["num"], 1);
						v.flute   = lua_as_int(it["flute"]);
						luabridge::LuaRef attrs = it["attrs"];
						if (attrs.isTable()) {
							for (int k = 1; k <= 5; ++k) {
								luabridge::LuaRef a = attrs[k];
								if (a.isTable()) {
									v.attrs[k - 1].attrId  = lua_as_int(a["id"]);
									v.attrs[k - 1].attrVal = lua_as_int(a["val"]);
								}
							}
						}
					}
					else if (it.isNumber()) {
						v.itemId  = it.cast<int64_t>().valueOr(0);
						v.itemNum = 1;
					}
					entry.variants.push_back(v);
				}
			}

			msg.products.push_back(std::move(entry));
		}
	}
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_TALKPAGE  id  ( Lua   npc-  id)
// -----------------------------------------------------------
void ShowTalkPageById(CCharacter* cha, int npcId, int pageId, const std::string& text)
{
	if (!cha) return;
	net::msg::McTalkInfoMessage msg;
	msg.npcId = static_cast<int64_t>(npcId);
	msg.cmd   = static_cast<int64_t>(pageId);
	msg.text  = text;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MM_DO_STRING    Lua-  .
//    gate  master-.
// -----------------------------------------------------------
void SendDoStringBroadcast(CCharacter* cha, int64_t srcId, const std::string& luaCode)
{
	if (!cha) return;
	net::msg::MmDoStringMessage msg;
	msg.srcId   = srcId;
	msg.luaCode = luaCode;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MT_KICKUSER (1505) —  Gate  .
//        Lua- (   ).
// -----------------------------------------------------------
void SendKickUser(CCharacter* cha)
{
	if (!cha) return;
	net::WPacket pkt;
	pkt.WriteCmd(CMD_MT_KICKUSER);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  cmd=935   .  legacy   CloseClient
// -----------------------------------------------------------
void SendCloseClient(CCharacter* cha)
{
	if (!cha) return;
	net::WPacket pkt;
	pkt.WriteCmd(935);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_MAPCRASH — popup
// -----------------------------------------------------------
void ShowMapCrash(CCharacter* cha, const std::string& text)
{
	if (!cha) return;
	net::msg::McMapCrashMessage msg;
	msg.text = text;
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

// -----------------------------------------------------------
//  CMD_MC_SAY —  (    Lua)
// -----------------------------------------------------------
void ShowSay(CCharacter* cha, int sourceId, const std::string& content, int color)
{
	if (!cha) return;
	net::msg::McSayMessage msg;
	msg.sourceId = static_cast<int64_t>(sourceId);
	msg.content  = content;
	msg.color    = static_cast<int64_t>(color);
	auto pkt = net::msg::serialize(msg);
	cha->ReflectINFof(cha, pkt);
}

void SyncGoodsData(CCharacter* npc, int index, int itemId, int count, int price)
{
	if (!npc) return;
	net::msg::McTradeDataMessage msg;
	msg.npcId  = static_cast<int64_t>(npc->GetID());
	msg.page   = 0;
	msg.index  = static_cast<int64_t>(index);
	msg.itemId = static_cast<int64_t>(itemId);
	msg.count  = static_cast<int64_t>(count);
	msg.price  = static_cast<int64_t>(price);
	auto pkt = net::msg::serialize(msg);
	npc->NotiChgToEyeshot(pkt);
}

//  SendExchangeUpdateData: broadcast    ( SynPacket)
void SyncBlackMarketExchangeUpdate(CCharacter* npc, luabridge::LuaRef ex)
{
	if (!npc) return;
	net::msg::McBlackMarketExchangeUpdateMessage msg;
	msg.npcId = static_cast<int64_t>(npc->GetID());
	msg.exchanges = read_exchange_entries<net::msg::BlackMarketExchangeEntry>(ex, /*withTime=*/true);
	auto pkt = net::msg::serialize(msg);
	npc->NotiChgToEyeshot(pkt);
}

//   Lua    McTradeAllDataMessage.
//   trade :
//   trade[1..N].itemtype,  count,  item[],  price[]
namespace {
bool build_trade_message(luabridge::LuaRef trade, int tradeType,
                         CCharacter* npc, int p1,
                         net::msg::McTradeAllDataMessage& msg)
{
	if (!trade.isTable() || !npc) return false;
	const bool isGoods = (tradeType == static_cast<int>(mission::TRADE_GOODS));

	msg.npcId     = static_cast<int64_t>(npc->GetID());
	msg.tradeType = static_cast<int64_t>(tradeType);
	msg.param     = static_cast<int64_t>(p1);

	for (int i = 1; i <= 4; ++i) {
		luabridge::LuaRef entry = trade[i];
		if (entry.isNil()) break;
		if (!entry.isTable()) return false;

		luabridge::LuaRef refItemType = entry["itemtype"];
		luabridge::LuaRef refCount    = entry["count"];
		luabridge::LuaRef refItems    = entry["item"];
		if (!refItemType.isNumber() || !refCount.isNumber() || !refItems.isTable()) return false;

		int count = refCount.cast<int>().valueOr(0);
		if (count > 120) count = 120;

		net::msg::TradePage page;
		page.itemType = refItemType.cast<int64_t>().valueOr(0);
		page.items.resize(static_cast<size_t>(count));

		luabridge::LuaRef refPrices = entry["price"];
		for (int n = 1; n <= count; ++n) {
			auto& dst = page.items[static_cast<size_t>(n - 1)];
			luabridge::LuaRef item = refItems[n];
			if (isGoods && item.isTable()) {
				dst.itemId = item["id"].cast<int64_t>().valueOr(-1);
				dst.count  = item["count"].cast<int64_t>().valueOr(0);
				dst.level  = item["level"].cast<int64_t>().valueOr(0);
				if (refPrices.isTable()) {
					luabridge::LuaRef p = refPrices[n];
					if (p.isTable()) dst.price = p["curprice"].cast<int64_t>().valueOr(0);
				}
			}
			else if (item.isNumber()) {
				dst.itemId = item.cast<int64_t>().valueOr(-1);
			}
			else {
				dst.itemId = -1; // ROLE_INVALID_ID
			}
		}
		msg.pages.push_back(std::move(page));
	}
	return true;
}

//    McTradeAllDataMessage    CMD.
//     SC_TradeInfo / SC_TradeAllData / SC_TradeUpdate.
net::WPacket trade_message_to_packet(const net::msg::McTradeAllDataMessage& msg, uint16_t cmd)
{
	net::WPacket packet;
	packet.WriteCmd(cmd);
	packet.WriteInt64(msg.npcId);
	packet.WriteInt64(msg.tradeType);
	packet.WriteInt64(msg.param);
	packet.WriteInt64(static_cast<int64_t>(msg.pages.size()));
	for (const auto& page : msg.pages) {
		packet.WriteInt64(page.itemType);
		packet.WriteInt64(static_cast<int64_t>(page.items.size()));
		for (const auto& it : page.items) packet.WriteInt64(it.itemId);
		//   CommandMessages.h:   count/price/level   tradeType==1.
		if (msg.tradeType == 1) {
			for (const auto& it : page.items) {
				packet.WriteInt64(it.count);
				packet.WriteInt64(it.price);
				packet.WriteInt64(it.level);
			}
		}
	}
	return packet;
}
} //  anonymous

//     CMD_MC_TRADEPAGE (   ).
void ShowTradePage(CCharacter* cha, CCharacter* npc, luabridge::LuaRef trade, int tradeType, int p1)
{
	if (!cha || !npc) return;
	net::msg::McTradeAllDataMessage msg;
	if (!build_trade_message(trade, tradeType, npc, p1, msg)) {
		ToLogService("trade", LogLevel::Error, "ShowTradePage: bad trade table (cha={})", cha->GetLogName());
		return;
	}
	auto pkt = trade_message_to_packet(msg, CMD_MC_TRADEPAGE);
	ToLogService("trade", "ShowTradePage cha={} npc={} tradeType={} pages={} size={}",
		cha->GetLogName(), npc->GetLogName(), tradeType, msg.pages.size(), pkt.GetPacketSize());
	cha->ReflectINFof(cha, pkt);
}

//  (  ):   CMD_MC_BLACKMARKET_TRADEUPDATE .
void ShowTradeUpdate(CCharacter* cha, CCharacter* npc, luabridge::LuaRef trade, int tradeType, int p1)
{
	if (!cha || !npc) return;
	net::msg::McTradeAllDataMessage msg;
	if (!build_trade_message(trade, tradeType, npc, p1, msg)) return;
	auto pkt = trade_message_to_packet(msg, CMD_MC_BLACKMARKET_TRADEUPDATE);
	cha->ReflectINFof(cha, pkt);
}

//  broadcast  NPC eyeshot.
void SyncTradeUpdate(CCharacter* npc, luabridge::LuaRef trade, int tradeType, int p1)
{
	if (!npc) return;
	net::msg::McTradeAllDataMessage msg;
	if (!build_trade_message(trade, tradeType, npc, p1, msg)) return;
	auto pkt = trade_message_to_packet(msg, CMD_MC_BLACKMARKET_TRADEUPDATE);
	npc->NotiChgToEyeshot(pkt);
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
		strncpy(szName, pRec->szName.c_str(), 64 - 1);
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
		strncpy(szItem, pRec->szName.c_str(), 64 - 1);
	}

	return szItem;
}

std::string GetAreaName(int sAreaID)
{
	char szArea[128];
	strncpy(szArea, RES_STRING(GM_NPCSCRIPT_CPP_00002), 128 - 1);

	CAreaInfo* pInfo = AreaRecordStore::Instance()->Get(static_cast<int>(sAreaID));
	if (pInfo)
	{
		strncpy(szArea, pInfo->DataName.c_str(), 128 - 1);
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

	auto pCharResult = luabridge::Stack<CCharacter*>::get(L, 1);
	if (!pCharResult) { PARAM_ERROR return 0; }
	CCharacter* pChar = *pCharResult;
	const char* pszData = lua_tostring(L, 2);
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

		//     Lua .
		//    Show* / Sync*   .

		//   (   Lua)
		LUABRIDGE_REGISTER_FUNC(ShowTradePage)
		LUABRIDGE_REGISTER_FUNC(ShowTradeUpdate)
		LUABRIDGE_REGISTER_FUNC(SyncTradeUpdate)
		LUABRIDGE_REGISTER_FUNC(ShowTalkPage)
		LUABRIDGE_REGISTER_FUNC(ShowClosePage)
		LUABRIDGE_REGISTER_FUNC(ShowFuncPage)
		LUABRIDGE_REGISTER_FUNC(ShowMissionPage)
		LUABRIDGE_REGISTER_FUNC(ShowHelpDesc)
		LUABRIDGE_REGISTER_FUNC(ShowHelpSound)
		LUABRIDGE_REGISTER_FUNC(ShowMissionList)
		LUABRIDGE_REGISTER_FUNC(ShowMisPage)
		LUABRIDGE_REGISTER_FUNC(ShowMisLogInfo)
		LUABRIDGE_REGISTER_FUNC(SyncGoodsData)
		LUABRIDGE_REGISTER_FUNC(ShowMapCrash)
		LUABRIDGE_REGISTER_FUNC(ShowSay)
		LUABRIDGE_REGISTER_FUNC(ShowUpdateImp)
		LUABRIDGE_REGISTER_FUNC(ShowStoreBuyResult)
		LUABRIDGE_REGISTER_FUNC(ShowStoreOpen)
		LUABRIDGE_REGISTER_FUNC(ShowStoreList)
		LUABRIDGE_REGISTER_FUNC(ShowTalkPageById)
		LUABRIDGE_REGISTER_FUNC(SendDoStringBroadcast)
		LUABRIDGE_REGISTER_FUNC(SendKickUser)
		LUABRIDGE_REGISTER_FUNC(SendCloseClient)
		LUABRIDGE_REGISTER_FUNC(ShowBlackMarketExchange)
		LUABRIDGE_REGISTER_FUNC(ShowExchangeData)
		LUABRIDGE_REGISTER_FUNC(SyncBlackMarketExchangeUpdate)
		LUABRIDGE_REGISTER_FUNC(LogTrade)

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
