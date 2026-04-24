print("[**] SDK Files [**]")
print("-- [Loading] NPC SDK")

dofile(GetResPath("script/MisSdk/SystemSDK.lua"))

CMD_MC_BASE = 500
CMD_MC_ROLEBASE = CMD_MC_BASE + 300
CMD_MC_TALKPAGE = CMD_MC_ROLEBASE + 1
CMD_MC_FUNCPAGE = CMD_MC_ROLEBASE + 2
CMD_MC_CLOSETALK = CMD_MC_ROLEBASE + 3
CMD_MC_HELPINFO = CMD_MC_ROLEBASE + 4
CMD_MC_TRADEPAGE = CMD_MC_ROLEBASE + 5
CMD_MC_TRADERESULT = CMD_MC_ROLEBASE + 6
CMD_MC_TRADE_DATA = CMD_MC_ROLEBASE + 7
CMD_MC_TRADE_ALLDATA = CMD_MC_ROLEBASE + 8

CMD_MC_MISSION = CMD_MC_ROLEBASE + 22
CMD_MC_MISSIONLIST = CMD_MC_ROLEBASE + 23
CMD_MC_MISSIONTALK = CMD_MC_ROLEBASE + 24
CMD_MC_MISPAGE = CMD_MC_ROLEBASE + 27
CMD_MC_MISLOG = CMD_MC_ROLEBASE + 28
CMD_MC_MISLOGINFO = CMD_MC_ROLEBASE + 29
CMD_MC_BEGIN_ITEM_FORGE = CMD_MC_ROLEBASE + 35
CMD_MC_BEGIN_ITEM_UNITE = CMD_MC_ROLEBASE + 36

CMD_MC_CREATEBOAT = CMD_MC_ROLEBASE + 38
CMD_MC_UPDATEBOAT = CMD_MC_ROLEBASE + 39
CMD_MC_UPDATEBOAT_PART = CMD_MC_ROLEBASE + 40
CMD_MC_BERTH_LIST = CMD_MC_ROLEBASE + 41
CMD_MC_BOAT_LIST = CMD_MC_ROLEBASE + 42
CMD_MC_BOAT_ADD = CMD_MC_ROLEBASE + 43
CMD_MC_BOAT_CLEAR = CMD_MC_ROLEBASE + 44
CMD_MC_BOATINFO = CMD_MC_ROLEBASE + 45
CMD_MC_BOAT_BAGLIST = CMD_MC_ROLEBASE + 46

CMD_CM_BASE = 0
CMD_CM_ROLEBASE = CMD_CM_BASE + 300
CMD_CM_REQUESTTALK = CMD_CM_ROLEBASE + 1
CMD_CM_TALKPAGE = CMD_CM_ROLEBASE + 2
CMD_CM_FUNCITEM = CMD_CM_ROLEBASE + 3
CMD_CM_REQESTTRADE = CMD_CM_ROLEBASE + 8
CMD_CM_TRADEITEM = CMD_CM_ROLEBASE + 9
CMD_CM_REQUESTAGENCY = CMD_CM_ROLEBASE + 10
CMD_CM_MISSION = CMD_CM_ROLEBASE + 22
CMD_CM_MISSIONLIST = CMD_CM_ROLEBASE + 23
CMD_CM_MISSIONTALK = CMD_CM_ROLEBASE + 24
CMD_CM_MISLOG = CMD_CM_ROLEBASE + 25

CMD_MC_BLACKMARKET_EXCHANGEDATA = CMD_MC_BASE + 71
CMD_MC_BLACKMARKET_EXCHANGEUPDATE = CMD_MC_BASE + 73
CMD_MC_BLACKMARKET_TRADEUPDATE = CMD_MC_BASE + 74
CMD_MC_EXCHANGEDATA = CMD_MC_BASE + 75
CMD_CM_BLACKMARKET_EXCHANGE_REQ = CMD_CM_BASE + 51

ROLE_FIRSTPAGE = 0
ROLE_CLOSEPAGE = -1

ROLE_MAXNUM_TRADEITEM = 60
ROLE_INVALID_ID = -1

WEAPON = 0
DEFENCE = 1
OTHER = 2
SYNTHESIS = 3

TRADE_SALE = 0
TRADE_BUY = 1
TRADE_GOODS = 2

ROLE_TRADE_SALE = 0
ROLE_TRADE_BUY = 1
ROLE_TRADE_SALE_GOODS = 2
ROLE_TRADE_BUY_GOODS = 3
ROLE_TRADE_SELECT_BOAT = 4

BERTH_TRADE_LIST = 0
BERTH_LUANCH_LIST = 1
BERTH_REPAIR_LIST = 3
BERTH_SALVAGE_LIST = 4
BERTH_SUPPLY_LIST = 5
BERTH_BOATLEVEL_LIST = 6

RES_WOOD = 0
RES_MINE = 1

ROLE_MAXNUM_PAGEITEM = 9

MIS_HELP_DESP = 0
MIS_HELP_IMAGE = 1
MIS_HELP_SOUND = 2
MIS_HELP_BICKER = 3

function SendTalkPage(character, npc, pageid, desp)
    ShowTalkPage(character, npc, pageid, desp or "")
end

function SendExchangeData(character, npc)
    ShowBlackMarketExchange(character, npc, Exchange or {})
end

function SendExchangeXData(character, npc, exchangeData)
    ShowExchangeData(character, npc, exchangeData or {})
end

function SendExchangeUpdateData(character, npc)
    SyncBlackMarketExchangeUpdate(npc, Exchange or {})
end

function SendDebugPage(character, npc, info)
    ShowTalkPage(character, npc, ROLE_CLOSEPAGE, info or "")
end

--   HelpInfoX     Addons/packet.lua
--   (    ).

function SendFuncPage(character, npc, pageid, item, count)
    --      FuncItem.
    --       ( C++   "");
    --         — .
    local items = {}
    for i = 1, count or 0, 1 do
        items[i] = item[i] or "Incorrect notice option!"
    end
    ShowFuncPage(character, npc, pageid, "", items)
end

function SendPage(character, npc, pageid, talk, item, count)
    local items = {}
    for i = 1, count or 0, 1 do
        items[i] = item[i] or "Incorrect notice option!"
    end
    ShowFuncPage(character, npc, pageid, talk or "", items)
end

function SendMissionPage(character, npc, pageid, talk, item, count, misitem, misnum)
    PRINT("SendMissionPage")
    local items = {}
    for i = 1, count or 0, 1 do
        items[i] = item[i] or "Incorrect notice option!"
    end
    local missions = {}
    for n = 1, misnum or 0, 1 do
        if misitem[n] then
            missions[n] = {name = misitem[n].name, state = misitem[n].state}
        end
    end
    ShowMissionPage(character, npc, pageid, talk or "", items, missions)
end

function SendClosePage(character, npc)
    ShowClosePage(character, npc)
end

function SendMissionList(character, npc, missionlist, itemid, optype, listtype)
    PRINT("SendMissionList, itemid, optype, listtype", itemid, optype, listtype)
    local npcid = GetCharID(npc)
    local ret, num = GetNumMission(character, npcid, listtype)
    if ret ~= LUA_TRUE then
        PRINT("SendMissionList:obtain character quest list notice quantity failed!")
        return SystemNotice(character, "SendMissionList:obtain character quest list notice quantity failed!")
    end
    PRINT("itemid = %d, num = %d", itemid, num)
    if itemid >= num then
        PRINT("SendMissionList: parameter item id error!")
        return SystemNotice(character, "SendMissionList: parameter item id error!")
    end

    PRINT("SendMissionList: calculation page index notice!")
    local count, previd, nextid, itemnum = 1, 0, 0, 0
    local item = {}

    if optype == MIS_NEXT then
        previd = itemid + 1
        nextid = num - itemid
    else
        if itemid <= 8 then
            previd = 1
            if num >= 8 then
                nextid = 8
            else
                nextid = num
            end
        else
            previd = itemid - 8
            nextid = itemid
        end
    end

    PRINT("SendMissionList:previd = %d, nextid = %d", previd, nextid)
    for n = previd, nextid, 1 do
        itemnum = n
        local index = n - 1
        PRINT("index = ", index)
        local ret, id, step = GetMissionInfo(character, npcid, listtype, index)
        if ret ~= LUA_TRUE then
            break
        end
        PRINT("SendMissionList:missionid = %d, step = %d", id, step)
        if missionlist[id] == nil or missionlist[id].name == nil then
            item[count] = "Error: quest index or name does not exist!"
        else
            item[count] = missionlist[id].name
        end
        count = count + 1
    end

    PRINT("SendMissionList: send list notice")
    if count < 1 then
        PRINT("SendMissionList: does not have previous page or next page notice!")
        return SystemNotice(character, "SendMissionList: does not have previous page or next page notice!")
    end

    previd = previd - 1
    nextid = nextid - 1
    PRINT("SendMissionList:previd = %d, nextid = %d, count = %d", previd, nextid, count)

    local prevCmd
    if previd == 0 then
        prevCmd = MIS_PREV_END
    else
        prevCmd = MIS_PREV
    end
    local nextCmd
    local endid = num - 1
    if nextid == endid then
        nextCmd = MIS_NEXT_END
    else
        nextCmd = MIS_NEXT
    end

    --      "count - 1" .
    local items = {}
    for i = 1, count - 1, 1 do
        items[i] = item[i]
    end

    ShowMissionList(character, npc, listtype, previd, nextid, prevCmd, nextCmd, items)
    SetMissionPage(character, npcid, previd, nextid, listtype)
    return LUA_TRUE
end

function JumpPage(character, npc, page, param)
    if page == nil or param == nil or page[param] == nil then
        return SystemNotice(character, "JumpPage:Function parameter error!")
    end
    if page[param][1] == nil then
        return SystemNotice(character, "JumpPage: page notice cannot be as null!")
    end

    local talk = ""
    if page[param][1] ~= nil and page[param][1].talk ~= nil then
        talk = page[param][1].talk
    end

    local FuncItem = {}
    local count = 0
    PRINT("JumpPage:page item count = ", page[param].count)
    local funcnum = 8
    if page[param].count ~= nil then
        funcnum = page[param].count
    end

    for i = 1, funcnum, 1 do
        if page[param][i] == nil then
            break
        end
        if page[param][i].talk == nil then
            count = count + 1
            FuncItem[count] = page[param][i].text
            PRINT("JumpPage:", FuncItem[count])
        end
    end
    PRINT("JumpPage, ismission = ", page[param].ismission)
    if page[param].ismission == nil or GetNpcHasMission(npc) ~= LUA_TRUE then
        PRINT(
            "JumpPage3:SendMissioniPage, pageid, talk, funcitem, count, misitem, misnum",
            param,
            talk,
            FuncItem,
            count,
            nil,
            0
        )
        return SendMissionPage(character, npc, param, talk, FuncItem, count, nil, 0)
    end

    local misnum = 0
    local MisItem = {}
    local ret, NpcInfoID = GetScriptID(npc)
    PRINT("JumpPage:GetScriptID: ret =, NpcInfoID = ", ret, NpcInfoID)
    if ret ~= LUA_TRUE or NpcInfoList[NpcInfoID] == nil or NpcInfoList[NpcInfoID].missionlist == nil then
        PRINT(
            "JumpPage1:SendMissioniPage, pageid, talk, funcitem, count, misitem, misnum",
            param,
            talk,
            FuncItem,
            count,
            MisItem,
            misnum
        )
        return SendMissionPage(character, npc, param, talk, FuncItem, count, MisItem, misnum)
    end

    local missionlist = NpcInfoList[NpcInfoID].missionlist
    local npcid = GetCharID(npc)
    ret, misnum = GetNumMission(character, npcid)

    PRINT("JumpPage:misnum = ", misnum)
    for n = 1, misnum, 1 do
        local ret, id, state = GetMissionInfo(character, npcid, n - 1)
        PRINT("JumpPage:missionid = %d, state = %d", id, state)
        if missionlist[id] == nil or missionlist[id].name == nil then
            MisItem[n] = {}
            MisItem[n].name = "Error: quest index or name does not exist!"
            MisItem[n].state = state
            PRINT("JumpPage:mission name =, state = ", missionlist[id].name, state)
        else
            MisItem[n] = {}
            MisItem[n].name = missionlist[id].name
            MisItem[n].state = state
            PRINT("JumpPage:mission name =, state = ", missionlist[id].name, state)
        end
    end

    PRINT(
        "JumpPage2:SendMissioniPage, pageid, talk, funcitem, count, misitem, misnum",
        param,
        talk,
        FuncItem,
        count,
        MisItem,
        misnum
    )
    return SendMissionPage(character, npc, param, talk, FuncItem, count, MisItem, misnum)
end

function FuncPage(character, npc, page, param)
    if page == nil or param == nil or page[param] == nil or page[param][1] == nil then
        return SystemNotice(character, "FuncPage: function parameter error!")
    end

    local FuncItem = {}
    local count = 0
    for i = 1, ROLE_MAXNUM_PAGEITEM, 1 do
        if page[param][i] == nil then
            break
        end
        if page[param][i].talk == nil then
            count = count + 1
            FuncItem[count] = page[param][i].text
        end
    end

    if count == 0 then
        return SystemNotice(character, "FuncPage: does not have function item notice!")
    else
        return SendFuncPage(character, npc, param, FuncItem, count)
    end
end

function CloseTalk(character, npc)
    return SendClosePage(character, npc)
end

function GoTo(character, xpos, ypos, mapname)
    if xpos == nil or ypos == nil or mapname == nil then
        return SystemNotice(character, " GoTo:xpos or ypos or mapname level parameter cannot be as null!")
    end
    return MoveTo(character, xpos, ypos, mapname)
end

function RandToMap(character, npc, maplist, count)
    SendClosePage(character, npc)

    if maplist == nil or count == nil then
        return SystemNotice(character, "RandToMap:maplist or count level parameter cannot be as null!")
    end

    local id = Rand(count)
    id = id + 1
    if maplist[id] == nil or maplist[id].xpos == nil or maplist[id].ypos == nil or maplist[id].mapname == nil then
        return SystemNotice(character, "RandToMap: certain parameter in maplist cannot be as null!")
    end

    GoTo(character, maplist[id].xpos, maplist[id].ypos, maplist[id].mapname)
end

function IsRandInMap(character, npc, rate, mapname, x, y, w, h)
    if rate == nil or mapname == nil or x == nil or y == nil then
        PRINT("IsRandInMap: parameter error!")
        SystemNotice(character, "IsRandInMap:Function parameter error!rate, mapname, x, y", rate, mapname, x, y)
        return LUA_ERROR
    end
    if rate > Rand(100) then
        return LUA_FALSE
    end

    return IsInMap(character, mapname, x, y, w, h)
end

function PopupWindow(character, npc, page, trigger, p2)
    if trigger == nil or p2 == nil then
        return SystemNotice(character, "PopupWindow: Parameter cannot be null!")
    end

    trigger(character, npc, p2)
end

function MultiTrigger(character, npc, page, trigger, count)
    PRINT("MultiTrigger")
    if trigger == nil or count == nil then
        return SystemNotice(character, "MultiTrigger:funclist or count parameter cannot be as null!")
    end

    for n = 1, count, 1 do
        if trigger[n] == nil then
            break
        end
        PRINT("MultiTrigger:ConditionsTest, n = " .. n)
        local ret = ConditionsTest(character, trigger[n].conditions, nil, nil, npc)
        if ret == LUA_TRUE then
            PRINT("MultiTrigger:success, ActionsProc, n = " .. n)
            if trigger[n].actions ~= nil and trigger[n].actions.count ~= 0 then
                return ActionsProc(character, trigger[n].actions, npc, page)
            end
        else
            PRINT("MultiTrigger:failure, ActionsProc, n = " .. n)
            if trigger[n].failures ~= nil and trigger[n].failures.count ~= 0 then
                return ActionsProc(character, trigger[n].failures, npc, page)
            end
        end
    end
end

function MultiTrigger_0(character, npc, page, trigger, count)
    MultiTrigger(character, npc, page, trigger, count)
    Eleven_Log_0(character)
end

function MultiTrigger_1(character, npc, page, trigger, count)
    MultiTrigger(character, npc, page, trigger, count)
    Eleven_Log(character, 1)
end

function MultiTrigger_2(character, npc, page, trigger, count)
    MultiTrigger(character, npc, page, trigger, count)
    Eleven_Log(character, 2)
end

function MultiTrigger_3(character, npc, page, trigger, count)
    MultiTrigger(character, npc, page, trigger, count)
    Eleven_Log(character, 3)
end

function MultiConditions(character, npc, funclist, count)
    if funclist == nil or count == nil then
        return SystemNotice(character, "MultiConditions:funclist or count parameter cannot be as null!")
    end

    return ConditionsTest(character, funclist)
end

function MultiFunc(character, npc, page, funclist, count)
    if funclist == nil or count == nil then
        return SystemNotice(character, "MultiFunc:funclist or count parameter cannot be as null!")
    end

    for id = 1, count, 1 do
        PRINT("MultiFunc n = " .. id)
        if funclist[id].func == AddMoney then
            AddMoney(character, funclist[id].p1)
        elseif funclist[id].func == AddExp then
            AddExp(character, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == GiveItem then
            Giveitem(character, funclist[id].p1, funclist[id].p2, funclist[id].p3)
        elseif funclist[id].func == TakeItem then
            TakeItem(character, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == TakeMoney then
            TakeMoney(character, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == JumpPage then
            JumpPage(character, npc, page, funclist[id].p1)
        elseif funclist[id].func == MoveCity then
            MoveCity(character, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == RandToMap then
            RandToMap(character, npc, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == GoTo then
            GoTo(character, funclist[id].p1, funclist[id].p2, funclist[id].p3)
        elseif funclist[id].func == SetRecord then
            SetRecord(character, funclist[id].p1)
        elseif funclist[id].func == SetFlag then
            SetFlag(character, funclist[id].p1, funclist[id].p2)
        elseif funclist[id].func == CloseTalk then
            CloseTalk(character, npc)
        elseif funclist[id].func == RefreshMissionState then
            RefreshMissionState(character, npc)
        elseif funclist[id].func == OpenBank then
            OpenBank(character, npc)
        elseif funclist[id].func == OpenHair then
            OpenHair(character, npc)
        elseif funclist[id].func == CreditExchange then
            CreditExchange(character, npc, funclist[id].p1)
        else
            return SystemNotice(character, "MultiFunc:unknown functionerror!")
        end
    end
end

function RandFunction(character, npc, page, funclist, count)
    if funclist == nil or count == nil then
        return SystemNotice(character, "RandFunction:funclist or count parameter cannot be as null!")
    end

    local id = Rand(count)
    id = id + 1
    if funclist[id] == nil or funclist[id].func == nil then
        return SystemNotice(character, "RandFunction:funclist certain parameter cannot be as null!")
    end

    if funclist[id].func == JumpPage then
        return JumpPage(character, npc, page, funclist[id].p1)
    elseif funclist[id].func == RandToMap then
        return RandToMap(character, npc, funclist[id].p1, funclist[id].p2)
    elseif funclist[id].func == GoTo then
        SendClosePage(character, npc)
        return GoTo(character, funclist[id].p1, funclist[id].p2, funclist[id].p3)
    elseif funclist[id].func == GiveItem then
        PRINT("RandFunction:GiveItem, p1, p2, p3", funclist[id].p1, funclist[id].p2, funclist[id].p3)
        return GiveItem(character, npc, funclist[id].p1, funclist[id].p2, funclist[id].p3)
    elseif funclist[id].func == MultiFunc then
        PRINT("RandFunction:MultiFunc, p1, p2 ", funclist[id].p1, funclist[id].p2)
        return MultiFunc(character, npc, page, funclist[id].p1, funclist[id].p2)
    else
        return SystemNotice(character, "RandFunction: unknown function error!")
    end
end

--     . CMD  :
--   CMD_MC_BLACKMARKET_TRADEUPDATE →  ShowTradeUpdate,  → ShowTradePage.
function SendTradePage(character, npc, cmd, trade, tradetype, p1)
    if cmd == nil or trade == nil or tradetype == nil then
        return SystemNotice(character, "TradePage:Function parameter error!")
    end
    if cmd == CMD_MC_BLACKMARKET_TRADEUPDATE then
        ShowTradeUpdate(character, npc, trade, tradetype, p1 or 0)
    else
        ShowTradePage(character, npc, trade, tradetype, p1 or 0)
    end
end

function SendTradeUpdate(character, npc, trade, tradetype, p1)
    if trade == nil or tradetype == nil then
        return SystemNotice(character, "SendTradeUpdate: Function parameter error!")
    end
    SyncTradeUpdate(npc, trade, tradetype, p1 or 0)
end

function SendUpdateTradePage(character, npc, cmd, trade, tradetype, p1)
    if cmd == nil or trade == nil or tradetype == nil then
        return SystemNotice(character, "SendUpdateTradePage:Function parameter error!")
    end
    ShowTradeUpdate(character, npc, trade, tradetype, p1 or 0)
end

function TradePage(character, npc, trade, tradetype, p1)
    PRINT("TradePage (via ShowTradePage)")
    if trade == nil then
        EXLG("NpcSdk_error", "TradePage: trade table is nil! NpcName = %s", GetCharName(npc))
        return SystemNotice(character, "TradePage: npc has no trade data!")
    end
    ShowTradePage(character, npc, trade, tradetype, p1)
end

function SalePage(character, npc, trade)
    PRINT("Send sale page info!")
    TradePage(character, npc, trade, TRADE_SALE, 0)
end

function BuyPage(character, npc, trade)
    PRINT("Send buy page info!")
    TradePage(character, npc, trade, TRADE_BUY, 0)
end

function PlayEffect(npc, id)
    PRINT("PlayEffect")
    ChaPlayEffect(npc, id)
end

function TradeGoods(character, npc, trade, index)
    PRINT("Send TradeGoods page info, trade, index", trade, index)
    local ret, boatid = GetBoatID(character, index)
    if ret ~= LUA_TRUE then
        PRINT("ship trade option error, does not find target ship index p!index = " .. index)
        SystemNotice(character, "ship trade option error, does not find target ship index p!index = " .. index)
        return
    end
    PRINT("tradepage")
    TradePage(character, npc, trade, TRADE_GOODS, boatid)
end

function TradeBerthList(character, npc, berth)
    PRINT("TadeBerthList: show list of ship docked in harbor for trade!")
    local npcid = GetCharID(npc)
    return BoatBerthList(character, npcid, BERTH_TRADE_LIST, berth, 0, 0, 0)
end

function LuanchBerthList(character, npc, berth, xpos, ypos, dir)
    PRINT("LuanchBerthList: show docked ship list!")
    local npcid = GetCharID(npc)

    return BoatBerthList(character, npcid, BERTH_LUANCH_LIST, berth, xpos, ypos, dir)
end

function RepairBerthList(character, npc, berth)
    PRINT("RepairBerthList: show repair list of ships docked in harbor!")
    local npcid = GetCharID(npc)
    return BoatBerthList(character, npcid, BERTH_REPAIR_LIST, berth, 0, 0, 0)
end

function SalvageBerthList(character, npc, berth)
    PRINT("SalvageBerthList: show list of ship docked in harbor to salvage!")
    local npcid = GetCharID(npc)
    return BoatBerthList(character, npcid, BERTH_SALVAGE_LIST, berth, 0, 0, 0)
end

function SupplyBerthList(character, npc, berth)
    PRINT("SupplyBerthList: Show ship docked at harbor list for refuel!")
    local npcid = GetCharID(npc)
    return BoatBerthList(character, npcid, BERTH_SUPPLY_LIST, berth, 0, 0, 0)
end

function BoatLevelBerthList(character, npc, berth)
    PRINT("BoatLevelList: Show upgrade list of ships docked in this harbor!")
    local npcid = GetCharID(npc)
    return BoatBerthList(character, npcid, BERTH_BOATLEVEL_LIST, berth, 0, 0, 0)
end

function Sale(character, npc, index, count)
    PRINT("Sale")

    if index == nil or count == nil then
        PRINT("Sale:Function parameter error!")
        SystemNotice(character, "Sale:Function parameter error!")
        return LUA_FALSE
    end

    if
        KitbagLock(character, 1) == LUA_TRUE or GetChaStateLv(character, STATE_BAT) >= 1 or
            GetChaStateLv(character, STATE_JY) >= 1
     then
        SystemNotice(character, "Your Inventory is locked! FUCK OFF Cheater!")
        return LUA_FALSE
    end

    SafeSale(character, index, count)

    return LUA_TRUE
end

function Buy(character, npc, trade, itemtype, index1, index2, count)
	PRINT("Buy")
	itemtype = itemtype + 1
	index1 = index1 + 1
	if trade[itemtype] == nil or trade[itemtype].item == nil or trade[itemtype].item[index1] == nil then
		PRINT("Buy:function parameter error!")
		SystemNotice(character, "Buy:function parameter error!")
		return LUA_FALSE
	end
	local itemid = trade[itemtype].item[index1]
	SafeBuy(character, itemid, index2, count)
	return LUA_TRUE
end

function SaleGoods(character, npc, trade, boatid, index, count)
    PRINT("SaleGoods")
    if boatid == nil or index == nil or count == nil or trade == nil then
        PRINT("SaleGoods:Function parameter error!")
        SystemNotice(character, "SaleGoods:Function parameter error!")
        return LUA_FALSE
    end

    local ret = BoatTrade(character, trade.berth)
    if ret ~= LUA_TRUE then
        return LUA_FALSE
    end

    local ret, itemid = GetSaleGoodsItem(character, boatid, index)
    if ret ~= LUA_TRUE then
        PRINT("SaleGoods:GetSaleGoodsItem, obtain goods notice failed!index = " .. index)
        SystemNotice(character, "SaleGoods:GetSaleGoodsItem, obtain goods notice failed!index = " .. index)
        return LUA_FALSE
    end
    PRINT("SaleGoods: boatid =, index =, itemid = ", boatid, index, itemid)

    local price = 0
    for n = 1, trade[2].count, 1 do
        if trade[2].item[n].id == itemid then
            price = trade[2].price[n].curprice
            break
        end
    end
    PRINT("SaleGoods: boatid =, index =, itemid =, count =, price = ", boatid, index, itemid, count, price)

    local ret, level, cess = GetTradeItemData(character)
    if ret ~= LUA_TRUE then
        cess = 1
    end
    PRINT("SaleGoods:GetTradeItemData: level =, cess = ", level, cess)

    PRINT("SaleGoods:3-1, cess, price = ", cess, price)
    if cess == 0 then
        cess = 1
    end
    local param = (math.max(0, 30 - math.pow(cess, 0.5)) + math.pow(cess, (-0.5))) / 100
    local cessdata = math.floor(price * param + 0.5)
    PRINT("SaleGoods:3-1, cessdata, price = ", cessdata, price)
    local cessprice = price - cessdata
    PRINT("SaleGoods:3, cessprice = ", cessprice)

    local ret = SafeSaleGoods(character, boatid, index, count, cessprice)
    if ret ~= LUA_TRUE then
        PRINT("SaleGoods: sale of commerce goods failed!boatid, index, count, price", boatid, index, count, price)
        SystemNotice(character, "SaleGoods: Sales of commerce products failed!")
        LG("trade_error", "SaleGoods: sale of commerce goods failed!boatid, index, count, price", boatid, index, count, price)
        return LUA_FALSE
    end
    local allcessdata = cessdata * count
    SystemNotice(character, "Deduct Tax. Total:(" .. allcessdata .. "G). Unit:(" .. cessdata .. "G)")

    return LUA_TRUE
end

function BuyGoods(character, npc, trade, boatid, itemtype, index1, index2, count)
    PRINT("BuyGoods")
    itemtype = itemtype + 1
    index1 = index1 + 1
    if
        boatid == nil or trade[itemtype] == nil or trade[itemtype].item == nil or trade[itemtype].item[index1] == nil or
            trade[itemtype].price[index1] == nil
     then
        PRINT("BuyGoods:Function parameter error!")
        SystemNotice(character, "BuyGoods:Function parameter error!")
        return LUA_FALSE
    end

    local ret = BoatTrade(character, trade.berth)
    if ret ~= LUA_TRUE then
        return LUA_FALSE
    end

    if trade[itemtype].item[index1].count <= 0 then
        BickerNotice(character, "Item is sold out. Please wait!")
        return LUA_TRUE
    end
    PRINT("BuyGoods:1")

    if trade[itemtype].item[index1].level > 0 then
        local ret, level, cess = GetTradeItemData(character)
        if ret ~= LUA_TRUE then
            SystemNotice(
                character,
                "BuyGoods: GetTradeItemData: function transfer failed! Check if Commerce Permit exist!"
            )
            LG("trade_error", "BuyGoods: GetTradeItemData: function transfer failed! Check if Commerce Permit exist!")
            return LUA_FALSE
        end
        PRINT("BuyGoods:GetTradeItemData: level =, cess = ", level, cess)
        if level < trade[itemtype].item[index1].level then
            BickerNotice(
                character,
                "Required" .. trade[itemtype].item[index1].level .. "Lv Commerce Permit to buy this product!"
            )
            return LUA_FALSE
        end
    end

    PRINT("BuyGoods:2")

    if trade[itemtype].item[index1].count < count then
        count = trade[itemtype].item[index1].count
    end

    local itemid = trade[itemtype].item[index1].id
    local price = trade[itemtype].price[index1].curprice
    local ret = SafeBuyGoods(character, boatid, itemid, index2, count, price)
    if ret == LUA_TRUE then
        trade[itemtype].item[index1].count = trade[itemtype].item[index1].count - count
        PRINT("BuyGoods:index, id, count, price", index1, itemid, trade[itemtype].item[index1].count, price)
        SendGoodsData(npc, index1 - 1, itemid, trade[itemtype].item[index1].count, price)
    end
    PRINT("BuyGoods:4, price = ", price)
    return LUA_TRUE
end

function SendGoodsData(npc, index, id, count, price)
    if npc == nil or index == nil or id == nil or count == nil or price == nil then
        PRINT("SendGoodsData:Function parameter error!npc, index, id, count, price", npc, index, id, count, price)
        LG(
            "npctrade_error",
            "SendGoodsData:Function parameter error!npc, index, id, count, price",
            npc,
            index,
            id,
            count,
            price
        )
        return
    end
    SyncGoodsData(npc, index, id, count, price)
    return LUA_TRUE
end

function SendAllGoodsData(npc, trade)
    if trade == nil or npc == nil then
        PRINT("SendAllGoodsData: Function parameter error!")
        return LUA_ERROR
    end

    PRINT("SendAllGoodsData:")
    SendUpdateTradePage(npc, npc, CMD_MC_TRADE_ALLDATA, trade, TRADE_GOODS, 0)
    return LUA_TRUE
end

function PackBagGoods(character, boat, tp, level)
    PRINT("PackBagGoods: character, boat, tp, level ", character, boat, tp, level)
    if character == nil or boat == nil or level == nil or tp == nil then
        PRINT("PackBagGoods:functionparameter error, character, boat, tp, level ", character, boat, tp, level)
        LG(
            "goods_error",
            "PackBagGoods:functionparameter error, character, boat, tp, level ",
            character,
            boat,
            tp,
            level
        )
        return LUA_FALSE
    end

    if tp == RES_WOOD then
        if ResourceList == nil or ResourceList.wood == nil or ResourceList.wood[level] == nil then
            PRINT("PackBagGoods: invalid resource loading notice!")
            return LUA_FALSE
        end
        PRINT("PackBagGoods: 1")
        if
            ResourceList.wood[level].itemid == nil or ResourceList.wood[level].count == nil or
                ResourceList.wood[level].pileid == nil
         then
            PRINT(
                "PackBagGoods: Invalid wood resource loading notice!level, Itemid, count, pileid",
                level,
                ResourceList.wood[level].itemid,
                ResourceList.wood[level].count,
                ResourceList.wood[level].pileid
            )
            LG(
                "goods_error",
                "PackBagGoods: Invalid wood resource loading notice!level, Itemid, count, pileid",
                ResourceList.wood[level].itemid,
                ResourceList.wood[level].count,
                ResourceList.wood[level].pileid
            )
            return LUA_FALSE
        end

        PRINT(
            "PackBagGoods: PackBag:RES_WOOD: itemid =, count =, pileid = ",
            ResourceList.wood[level].itemid,
            ResourceList.wood[level].count,
            ResourceList.wood[level].pileid
        )
        local ret, numpack =
            PackBag(
            character,
            boat,
            ResourceList.wood[level].itemid,
            ResourceList.wood[level].count,
            ResourceList.wood[level].pileid
        )
        if ret ~= LUA_TRUE then
            PRINT(
                "PackBagGoods: load wood resource failed!level, itemid, count, pileid",
                level,
                ResourceList.wood[level].itemid,
                ResourceList.wood[level].count,
                ResourceList.wood[level].pileid
            )
            return LUA_FALSE
        end
        PRINT("Successfully loaded" .. numpack .. "pile" .. level .. "level Wood!")
        if numpack > 0 then
            BickerNotice(character, "Successfully loaded" .. numpack .. "pile" .. level .. "level Wood!")
        end
    elseif tp == RES_MINE then
        if
            ResourceList.mine[level].itemid == nil or ResourceList.mine[level].count == nil or
                ResourceList.mine[level].pileid == nil
         then
            PRINT(
                "PackBagGoods: Invalid ore resource loading notice! level, Itemid, count, pileid",
                ResourceList.mine[level].itemid,
                ResourceList.mine[level].count,
                ResourceList.mine[level].pileid
            )
            LG(
                "goods_error",
                "PackBagGoods: Invalid ore resource loading notice! level, Itemid, count, pileid",
                ResourceList.mine[level].itemid,
                ResourceList.mine[level].count,
                ResourceList.mine[level].pileid
            )
            return LUA_FALSE
        end

        PRINT(
            "PackBagGoods: PackBag:RES_MINE: itemid =, count =, pileid = ",
            ResourceList.mine[level].itemid,
            ResourceList.mine[level].count,
            ResourceList.mine[level].pileid
        )
        local ret, numpack =
            PackBag(
            character,
            boat,
            ResourceList.mine[level].itemid,
            ResourceList.mine[level].count,
            ResourceList.mine[level].pileid
        )
        if ret ~= LUA_TRUE then
            PRINT(
                "PackBagGoods: load wood resource failed!level, itemid, count, pileid",
                level,
                ResourceList.mine[level].itemid,
                ResourceList.mine[level].count,
                ResourceList.mine[level].pileid
            )
            return LUA_FALSE
        end
        PRINT("Successfully loaded" .. numpack .. "pile" .. level .. "level Ore!")
        if numpack > 0 then
            BickerNotice(character, "Successfully loaded" .. numpack .. "pile" .. level .. "level Ore!")
        end
    else
        PRINT("PackBagGoods: Invalid resource loading type, character, boat, tp, level ", character, boat, tp, level)
        LG(
            "goods_error",
            "PackBagGoods: Invalid resource loading type, character, boat, tp, level ",
            character,
            boat,
            tp,
            level
        )
        return LUA_FALSE
    end
    PRINT("PackBagGoods: return true")
    return LUA_TRUE
end

function AdjustTradeCess(character, cess, cessrange)
    if cess == nil or cessrange == nil then
        SystemNotice(character, "AdjustTradeCess:Function parameter error!")
        LG("mission_error", "AdjustTradeCess:Function parameter error!")
        return LUA_ERROR
    end

    local ret, level, cessdata = GetTradeItemData(character)
    if ret ~= LUA_TRUE then
        SystemNotice(
            character,
            "AdjustTradeCess:GetTradeItemData function transfer failed, check if Commerce Permit exists!"
        )
        LG(
            "mission_error",
            "AdjustTradeCess:GetTradeItemData function transfer failed, check if Commerce Permit exists!"
        )
        return LUA_FALSE
    end

    if level == 1 then
        return AdjustTradeItemCess(character, 200, cess + Rand(cessrange))
    elseif level == 2 then
        return AdjustTradeItemCess(character, 500, cess + Rand(cessrange))
    elseif level == 3 then
        return AdjustTradeItemCess(character, 60000, cess + Rand(cessrange))
    else
        SystemNotice(character, "AdjustTradeCess: Commerce Permit level exceeded! level = " .. level)
        LG(
            "mission_error",
            "AdjustTradeCess: character [" ..
                GetCharName(character) .. "] commerce permit level exceeded!level = " .. level
        )
        return LUA_FALSE
    end
end

function CreditExchange(character, npc, tp)
    if tp == nil then
        PRINT("CreditExchange: function parameter error!")
        SystemNotice(character, "CreditExchange: function parameter error!")
        return LUA_ERROR
    end

    PRINT("CreditExchange, p1 = " .. tp)
    CreditExchangeImpl(character, tp)
    return LUA_TRUE
end

function BoatLevelUp(character, boat, levelup)
    PRINT("BoatLevelUp: levelup", levelup)
    if levelup == nil or BoatLevelList == nil then
        PRINT("BoatLevelUp:Function parameter error!")
        SystemNotice(character, "BoatLevelUp:Function parameter error!")
        return LUA_ERROR
    end

    if BoatLevelList[levelup] == nil or BoatLevelList[levelup].money == nil or BoatLevelList[levelup].exp == nil then
        BickerNotice(character, "Your ship has reached maximum level!")
        return LUA_TRUE
    end

    PRINT("BoatLevelUp: levelup, exp, moeny", levelup, BoatLevelList[levelup].exp, BoatLevelList[levelup].money)

    local ret = BoatLevelUpProc(character, boat, levelup, BoatLevelList[levelup].exp, BoatLevelList[levelup].money)
    if ret == 0 then
        PRINT("BoatLevelUpProc:function transfer return failed!")
    end

    return LUA_TRUE
end

function MsgProc(character, npc, action, page, trade, missionlist, exchangeData)
    if character == nil or npc == nil or action == nil then
        PRINT("MsgProc:Function parameter error!")
        EXLG("NpcSdk_error", "MsgProc:Function parameter error!")
        return
    end
    PRINT("MsgProc:Net message process.")
    local usCmd = action.usCmd
    LogTrade("MsgProc npc=" .. tostring(GetCharName(npc)) .. " usCmd=" .. tostring(usCmd) .. " trade=" .. tostring(trade))
    if usCmd == CMD_CM_TALKPAGE then
        local byTalkID = action.talkId

        if byTalkID == ROLE_FIRSTPAGE then
            RefreshMissionState(character, npc)
            if page.start == MultiTrigger then
                PRINT("MsgProc:MultiTrigger, page, p1, p2", page, page.p1, page.p2)
                return MultiTrigger(character, npc, page, page.p1, page.p2)
            end
            if page.start == PopupWindow then
                PRINT("MsgProc:PopupWindow, page, p1, p2", page, page.p1, page.p2)
                return PopupWindow(character, npc, page, page.p1, page.p2)
            end
            return JumpPage(character, npc, page, 1)
        elseif byTalkID == ROLE_CLOSEPAGE then
            return SendClosePage(character, npc)
        elseif page[byTalkID] == nil or page[byTalkID][1] == nil then
            return SystemNotice(
                character,
                "MsgProc: request a dialogue with npc but target npc does not have page data notice!"
            )
        else
            local talkpage = page[byTalkID][1]
            local funcpage = page[byTalkID][2]

            if talkpage.func == JumpPage then
                return JumpPage(character, npc, page, talkpage.p1)
            elseif talkpage.func == FuncPage then
                return FuncPage(character, npc, page, talkpage.p1)
            elseif talkpage.func == CloseTalk then
                return CloseTalk(character, npc)
            elseif talkpage.func == HasRecord then
                local ret = HasRecord(character, talkpage.p1)
                if ret == LUA_TRUE then
                    return JumpPage(character, npc, page, talkpage.p2)
                else
                    return JumpPage(character, npc, page, talkpage.p3)
                end
            elseif funcpage ~= nil then
                return FuncPage(character, npc, page, byTalkID)
            else
                local PageID = byTalkID + 1

                if page[PageID] == nil then
                    EXLG(
                        "NpcSdk_error",
                        "MsgProc: page notice link incorrect, does not have next page or unknown link function! NpcName = %s, PageID = %d",
                        GetCharName(npc),
                        PageID
                    )
                    return SystemNotice(
                        character,
                        "MsgProc: page notice link error, does not have next page or unknown link function!"
                    )
                end
                return JumpPage(character, npc, page, PageID)
            end
        end
        return
    elseif usCmd == CMD_CM_FUNCITEM then
        local byPage = action.page
        local byItem = action.item
        PRINT("MsgProc: pageid =, itemid = ", byPage, byItem)
        LogTrade("MsgProc FUNCITEM npc=" .. tostring(GetCharName(npc)) .. " byPage=" .. tostring(byPage) .. " byItem=" .. tostring(byItem))
        if page[byPage] == nil or page[byPage][1] == nil then
            EXLG(
                "NpcSdk_error",
                "MsgProc:unknown function page index notice error or target page notice does not exist! NpcName = %s, PageID = %d, ItemID = %d",
                GetCharName(npc),
                byPage,
                byItem
            )
            return SystemNotice(
                character,
                "MsgProc:unknown function page index notice error or target page does not exist!"
            )
        end

        if page[byPage][1].talk ~= nil then
            if page[byPage][1].func ~= nil then
                PRINT("MultiTrigger")
                byItem = 1
            else
                byItem = byItem + 2
            end
        elseif page[byPage][1].text ~= nil then
            byItem = byItem + 1
        end

        local item = page[byPage][byItem]
        if item == nil or item.func == nil then
            EXLG(
                "NpcSdk_error",
                "MsgProc: target page does not have function option notice or function notice! NpcName = %s, PageID = %d, ItemID = %d",
                GetCharName(npc),
                byPage,
                byItem
            )
            return SystemNotice(
                character,
                "MsgProc: Target page does not have function page notice nor function notice!"
            )
        end

        LogTrade("MsgProc FUNCITEM resolved byItem=" .. tostring(byItem) .. " text=" .. tostring(item.text)
            .. " func=" .. tostring(item.func)
            .. " (BuyPage=" .. tostring(BuyPage) .. " SalePage=" .. tostring(SalePage)
            .. " JumpPage=" .. tostring(JumpPage) .. " CloseTalk=" .. tostring(CloseTalk)
            .. " FuncPage=" .. tostring(FuncPage) .. ")")

        if item.func == JumpPage then
            return JumpPage(character, npc, page, item.p1)
        elseif item.func == FuncPage then
            return FuncPage(character, npc, page, item.p1)
        elseif item.func == CloseTalk then
            return CloseTalk(character, npc)
        elseif item.func == GoTo then
            return GoTo(character, item.p1, item.p2, item.p3)
        elseif item.func == RandToMap then
            return RandToMap(character, npc, item.p1, item.p2)
        elseif item.func == RandFunction then
            return RandFunction(character, npc, page, item.p1, item.p2)
        elseif item.func == MultiFunc then
            return MultiFunc(character, npc, page, item.p1, item.p2)
        elseif item.func == SalePage then
            return SalePage(character, npc, trade)
        elseif item.func == BuyPage then
            return BuyPage(character, npc, trade)
        elseif item.func == ListAuction then
            return ListAuction(character, npc)
        elseif item.func == ChangeItem then
            return ChangeItem(character, npc)
        elseif item.func == TradeGoods then
            return TradeGoods(character, npc, trade, item.p1)
        elseif item.func == MultiTrigger then
            return MultiTrigger(character, npc, page, item.p1, item.p2)
        elseif item.func == MultiTrigger_0 then
            return MultiTrigger_0(character, npc, page, item.p1, item.p2)
        elseif item.func == MultiTrigger_1 then
            return MultiTrigger_1(character, npc, page, item.p1, item.p2)
        elseif item.func == MultiTrigger_2 then
            return MultiTrigger_2(character, npc, page, item.p1, item.p2)
        elseif item.func == MultiTrigger_3 then
            return MultiTrigger_3(character, npc, page, item.p1, item.p2)
        elseif item.func == OpenForge then
            return OpenForge(character, npc)
        elseif item.func == OpenUnite then
            return OpenUnite(character, npc)
        elseif item.func == OpenMilling then
            return OpenMilling(character, npc)
        elseif item.func == OpenHair then
            return OpenHair(character, npc)
        elseif item.func == OpenRepair then
            return OpenRepair(character, npc)
        elseif item.func == OpenFusion then
            return OpenFusion(character, npc)
        elseif item.func == OpenUpgrade then
            return OpenUpgrade(character, npc)
        elseif item.func == OpenEidolonMetempsychosis then
            return OpenEidolonMetempsychosis(character, npc)
        elseif item.func == OpenItemTiChun then
            return OpenItemTiChun(character, npc)
        elseif item.func == OpenItemEnergy then
            return OpenItemEnergy(character, npc)
        elseif item.func == OpenGMSend then
            return OpenGMSend(character, npc)
        elseif item.func == OpenGMRecv then
            return OpenGMRecv(character, npc)
        elseif item.func == OpenItemFix then
            return OpenItemFix(character, npc)
        elseif item.func == OpenGetStone then
            return OpenGetStone(character, npc)
        elseif item.func == OpenTiger then
            return OpenTiger(character, npc)
        elseif item.func == GoTosdBoss then
            return GoTosdBoss(character, item.p1)
        elseif item.func == SetSpawnPos then
            return SetSpawnPos(character, item.p1)
        elseif item.func == TransferDiamond then
            return TransferDiamond(character, item.p1)
        elseif item.func == ListChallenge then
            return ListChallenge(character, npc)
        elseif item.func == SendExchangeData then
            return SendExchangeData(character, npc)
        elseif item.func == SendExchangeXData then
            return SendExchangeXData(character, npc, exchangeData)
        elseif item.func == Garner2GetWiner then
            return Garner2GetWiner(character, npc)
        elseif item.func == Change_FightingBook then
            return Change_FightingBook(character, npc)
        elseif item.func == Garner2RequestReorder then
            return Garner2RequestReorder(character, npc)
        elseif item.func == Transfer_TeamStar then
            return Transfer_TeamStar(character, item.p1)
        elseif item.func == LifeSkillBegin then
            return LifeSkillBegin(character, npc, item.p1)
        elseif item.func == Change_shanyao then
            return Change_shanyao(character, npc)
        elseif item.func == Change_rongyao then
            return Change_rongyao(character, npc)
        elseif item.func == Change_huihuang then
            return Change_huihuang(character, npc)
        elseif item.func == GetChaName_0 then
            return GetChaName_0(character, npc)
        elseif item.func == GetChaName_1 then
            return GetChaName_1(character, npc)
        elseif item.func == GetChaName_2 then
            return GetChaName_2(character, npc)
        elseif item.func == GetChaName_3 then
            return GetChaName_3(character, npc)
        elseif item.func == GetChaName_4 then
            return GetChaName_4(character, npc)
        elseif item.func == GetChaName_5 then
            return GetChaName_5(character, npc)
        elseif item.func == GetChaName_21 then
            return GetChaName_21(character, npc)
        elseif item.func == GetChaName_22 then
            return GetChaName_22(character, npc)
        elseif item.func == GetChaName_23 then
            return GetChaName_23(character, npc)
        elseif item.func == GetChaName_24 then
            return GetChaName_24(character, npc)
        elseif item.func == GetChaName_25 then
            return GetChaName_25(character, npc)
        elseif item.func == GetChaName_26 then
            return GetChaName_26(character, npc)
        elseif item.func == GetChaName_27 then
            return GetChaName_27(character, npc)
        elseif item.func == GetChaName_28 then
            return GetChaName_28(character, npc)
        elseif item.func == GetChaName1_born then
            return GetChaName1_born(character, npc)
        elseif item.func == GetChaName2_born then
            return GetChaName2_born(character, npc)
        elseif item.func == GetChaName3_born then
            return GetChaName3_born(character, npc)
        elseif item.func == GetChaName4_born then
            return GetChaName4_born(character, npc)
        elseif item.func == GetChaName5_born then
            return GetChaName5_born(character, npc)
        elseif item.func == GetChaName6_born then
            return GetChaName6_born(character, npc)
        elseif item.func == GiveHoe then
            return GiveHoe(character, npc)
        elseif item.func == CheckMetal then
            return CheckMetal(character, npc)
        elseif item.func == PKPointToZero then
            return PKPointToZero(character, npc)
        elseif item.func == PlayEffect then
            return PlayEffect(npc, 361)
        elseif item.func == GetChaName1_guildwar then
            return GetChaName1_guildwar(character, npc)
        elseif item.func == GetChaName2_guildwar then
            return GetChaName2_guildwar(character, npc)
        elseif item.func == GetChaName3_guildwar then
            return GetChaName3_guildwar(character, npc)
        elseif item.func == GetChaName4_guildwar then
            return GetChaName4_guildwar(character, npc)
        elseif item.func == GetChaName5_guildwar then
            return GetChaName5_guildwar(character)
        elseif item.func == GetChaName6_guildwar then
            return GetChaName6_guildwar(character)
        elseif item.func == GetChaName7_guildwar then
            return GetChaName7_guildwar(character)
        elseif item.func == GetChaName8_guildwar then
            return GetChaName8_guildwar(character)
        elseif item.func == GetChaName9_guildwar then
            return GetChaName9_guildwar(character)
        elseif item.func == GetChaName10_guildwar then
            return GetChaName10_guildwar(character)
        elseif item.func == GetChaName11_guildwar then
            return GetChaName11_guildwar(character)
        elseif item.func == GetChaName12_guildwar then
            return GetChaName12_guildwar(character)
        elseif item.func == GetChaName13_guildwar then
            return GetChaName13_guildwar(character)
        elseif item.func == GetChaName14_guildwar then
            return GetChaName14_guildwar(character)
        elseif item.func == GetChaName15_guildwar then
            return GetChaName15_guildwar(character)
        elseif item.func == GetChaName16_guildwar then
            return GetChaName16_guildwar(character)
        elseif item.func == GetChaName17_guildwar then
            return GetChaName17_guildwar(character)
        elseif item.func == GetChaName18_guildwar then
            return GetChaName18_guildwar(character)
        elseif item.func == GetChaName19_guildwar then
            return GetChaName19_guildwar(character)
        elseif item.func == GetChaName20_guildwar then
            return GetChaName20_guildwar(character)
        elseif item.func == GetChaName21_guildwar then
            return GetChaName21_guildwar(character)
        elseif item.func == GetChaName22_guildwar then
            return GetChaName22_guildwar(character)
        elseif item.func == GetChaName23_guildwar then
            return GetChaName23_guildwar(character)
        elseif item.func == GetChaName24_guildwar then
            return GetChaName24_guildwar(character)
        elseif item.func == GetChaName25_guildwar then
            return GetChaName25_guildwar(character)
        elseif item.func == GetChaName26_guildwar then
            return GetChaName26_guildwar(character)
        elseif item.func == GetChaName27_guildwar then
            return GetChaName27_guildwar(character)
        elseif item.func == GetChaName28_guildwar then
            return GetChaName28_guildwar(character)
        elseif item.func == GetChaName29_guildwar then
            return GetChaName29_guildwar(character)
        elseif item.func == GetChaName30_guildwar then
            return GetChaName30_guildwar(character)
        elseif item.func == GetChaName31_guildwar then
            return GetChaName31_guildwar(character)
        elseif item.func == GetChaName32_guildwar then
            return GetChaName32_guildwar(character)
        elseif item.func == GetChaName33_guildwar then
            return GetChaName33_guildwar(character)
        elseif item.func == GetChaName34_guildwar then
            return GetChaName34_guildwar(character)
        elseif item.func == GetChaName35_guildwar then
            return GetChaName35_guildwar(character)
        elseif item.func == GetChaName36_guildwar then
            return GetChaName36_guildwar(character)
        elseif item.func == GetChaName37_guildwar then
            return GetChaName37_guildwar(character)
        elseif item.func == GetChaName38_guildwar then
            return GetChaName38_guildwar(character)
        elseif item.func == GetChaName39_guildwar then
            return GetChaName39_guildwar(character)
        elseif item.func == GetChaName40_guildwar then
            return GetChaName40_guildwar(character)
        elseif item.func == GetChaName41_guildwar then
            return GetChaName41_guildwar(character)
        elseif item.func == GetChaName42_guildwar then
            return GetChaName42_guildwar(character)
        elseif item.func == GetChaName43_guildwar then
            return GetChaName43_guildwar(character)
        elseif item.func == GetChaName44_guildwar then
            return GetChaName44_guildwar(character)
        elseif item.func == GetChaName45_guildwar then
            return GetChaName45_guildwar(character)
        elseif item.func == ChangeItem then
            return ChangeItem(character, npc)
        else
            return SystemNotice(character, "MsgProc: function option unknown function!")
        end
    elseif usCmd == CMD_CM_BLACKMARKET_EXCHANGE_REQ then
        local timeNum = action.timeNum
        local srcID = action.srcId
        local srcNum = action.srcNum
        local tarID = action.tarId
        local tarNum = action.tarNum
        local byIndex = action.idx

        local ret = KitbagLock(character, 0)
        if ret ~= LUA_TRUE then
            SystemNotice(character, "Inventory is binded. Unable to redeem")
            return
        end

        local retbag = HasLeaveBagGrid(character, 1)
        if retbag ~= LUA_TRUE then
            SystemNotice(character, "Insufficent inventory space to redeem")
            return
        end

        ExchangeReq(character, npc, srcID, srcNum, tarID, tarNum, timeNum)
    elseif usCmd == CMD_CM_TRADEITEM then
        local tradetype = action.tradeType
        if tradetype == ROLE_TRADE_SALE then
            if trade.tp ~= TRADE_NOMAL then
                SystemNotice(character, "Trade mode error!tp =", trade.tp)
                PRINT("trade mode error!tp =, tradetype = ", trade.tp, ROLE_TRADE_SALE)
                return
            end

            local index = action.index
            local count = action.count
            if count == 0 then
                return
            end
            PRINT("MsgProc:Sale index = %d, count = %d", index, count)
            Sale(character, npc, index, count)
        elseif tradetype == ROLE_TRADE_BUY then
            if trade.tp ~= TRADE_NOMAL then
                SystemNotice(character, "Trade mode error!tp =", trade.tp)
                PRINT("trade mode error!tp =, tradetype = ", trade.tp, ROLE_TRADE_BUY)
                return
            end

            local itemtype = action.itemType
            local index1 = action.index1
            local index2 = action.index2
            local count = action.count
            if count == 0 then
                return
            end
            PRINT("MsgProc:Buy itemtype = %d, index1 = %d, index2 = %d, count = %d", itemtype, index1, index2, count)
            Buy(character, npc, trade, itemtype, index1, index2, count)
        elseif tradetype == ROLE_TRADE_SALE_GOODS then
            if trade.tp ~= TRADE_GOODS then
                SystemNotice(character, "Trade mode error!tp =", trade.tp)
                PRINT("trade mode error!tp =, tradetype = ", trade.tp, ROLE_TRADE_SALE_GOODS)
                return
            end

            local boatid = action.boatId
            local index = action.index
            local count = action.count
            if count == 0 then
                return
            end
            PRINT("MsgProc:Sale boatid =,index =, count =", boatid, index, count)
            SaleGoods(character, npc, trade, boatid, index, count)
        elseif tradetype == ROLE_TRADE_BUY_GOODS then
            if trade.tp ~= TRADE_GOODS then
                SystemNotice(character, "Trade mode error!tp =", trade.tp)
                PRINT("trade mode error!tp =, tradetype = ", trade.tp, ROLE_TRADE_BUY_GOODS)
                return
            end

            local boatid = action.boatId
            local itemtype = action.itemType
            local index1 = action.index1
            local index2 = action.index2
            local count = action.count

            if count == 0 then
                return
            end
            PRINT(
                "MsgProc:Buy goods boatid =, itemtype =, index1 =, index2 =, count = ",
                boatid,
                itemtype,
                index1,
                index2,
                count
            )
            BuyGoods(character, npc, trade, boatid, itemtype, index1, index2, count)
        elseif tradetype == ROLE_TRADE_SELECT_BOAT then
            local index = action.index
            PRINT("MsgProc:index = ", index)
            TradeGoods(character, npc, trade, index)
        else
            return SystemNotice(character, "MsgProc: trade request command error!")
        end
    elseif usCmd == CMD_CM_MISSION then
        return MissionProc(character, npc, action, missionlist)
    else
        EXLG("NpcSdk_error", "MsgProc:unknown internet command! CMD = %d", usCmd)
        return SystemNotice(character, "MsgProc: Unknown internet command!")
    end
end

function GiveHoe(role, npc)
    local r1 = 0
    local r2 = 0
    r1, r2 = MakeItem(role, 3908, 1, 4)
    local Itemnew = GetChaItem(role, 2, r2)

    SetItemAttr(Itemnew, ITEMATTR_URE, 10)
    SetItemAttr(Itemnew, ITEMATTR_MAXURE, 10)
    RefreshCha(role)
end

function CheckMetal(role, npc)
    local hoe_Count = CheckBagItem(role, 3908)
    local hoe_temp = GetChaItem2(role, 2, 3908)
    local hoe_maxUre = GetItemAttr(hoe_temp, ITEMATTR_MAXURE)

    if hoe_Count == 1 then
        if hoe_maxUre < 50 then
            SetItemAttr(hoe_temp, ITEMATTR_MAXURE, 5000)
            SetItemAttr(hoe_temp, ITEMATTR_URE, 5000)
            SystemNotice(role, "Your Alloy Pickaxe has been restored")
        else
            SystemNotice(role, "Sorry, your Alloy Pickaxe is normal.")
        end
    else
        SystemNotice(role, "Sorry, please make sure you have a damaged Alloy Pickaxe")
    end
    RefreshCha(role)
end

function PKPointToZero(role, npc)
    local PK_Count = CheckBagItem(role, 3849)
    local PK_Book = GetChaItem2(role, 2, 3849)

    if PK_Count >= 1 then
        SetItemAttr(PK_Book, ITEMATTR_MAXENERGY, 0)
        SystemNotice(role, "Your Chaos points has been resetted")
    else
        SystemNotice(role, "Sorry, you do not have any Medal of Honor.")
    end
    RefreshCha(role)
end

function GoTosdBoss(role, value)
    local br_x = {}
    local br_y = {}
    br_x[1] = {}
    br_x[2] = {}
    br_x[3] = {}
    br_x[4] = {}
    br_x[5] = {}
    br_y[1] = {}
    br_y[2] = {}
    br_y[3] = {}
    br_y[4] = {}
    br_y[5] = {}
    br_x[1][1] = 21
    br_x[1][2] = 98
    br_x[1][3] = 98
    br_x[1][4] = 21
    br_x[2][1] = 453
    br_x[2][2] = 530
    br_x[2][3] = 530
    br_x[2][4] = 453
    br_x[3][1] = 884
    br_x[3][2] = 961
    br_x[3][3] = 961
    br_x[3][4] = 884
    br_x[4][1] = 21
    br_x[4][2] = 98
    br_x[4][3] = 98
    br_x[4][4] = 21
    br_x[5][1] = 453
    br_x[5][2] = 530
    br_x[5][3] = 530
    br_x[5][4] = 453
    br_y[1][1] = 21
    br_y[1][2] = 21
    br_y[1][3] = 93
    br_y[1][4] = 93
    br_y[2][1] = 21
    br_y[2][2] = 21
    br_y[2][3] = 93
    br_y[2][4] = 93
    br_y[3][1] = 21
    br_y[3][2] = 21
    br_y[3][3] = 93
    br_y[3][4] = 93
    br_y[4][1] = 396
    br_y[4][2] = 396
    br_y[4][3] = 468
    br_y[4][4] = 468
    br_y[5][1] = 396
    br_y[5][2] = 396
    br_y[5][3] = 468
    br_y[5][4] = 468
    local n = math.random(1, 4)
    GoTo(role, br_x[value][n], br_y[value][n], "sdBoss")
    return LUA_TRUE
end

ReputationExchange = function(Player, Amount, Item, Quantity)
    local Reputation = GetChaAttr(Player, ATTR_FAME)
    local ItemID = GetItemID(Item)
    if Reputation < Amount then
        HelpInfoX(Player, 0, "You need " .. Amount .. " reputations to purchase this item.")
        return
    else
        GiveItem(Player, 0, Item, Quantity, 4)
    end
    DelCredit(Player, Amount)
    RefreshCha(Player)
end

ClassChange = function(Player, Type, Class)
    local JobCh = {}
    JobCh[1] = {Money = 2500, Level = 9}
    JobCh[2] = {Money = 50000, Level = 40}

    local CharType = GetChaTypeID(Player)
    local Level = Lv(Player)
    local Gold = GetChaAttr(Player, ATTR_GD)
    local Job = GetChaAttr(Player, ATTR_JOB)
    if Type == 1 then
        if Level < JobCh[1].Level then
            HelpInfoX(Player, 0, "You need at least to be Level " .. JobCh[1].Level .. ".")
            return
        end
        if Gold < JobCh[1].Money then
            HelpInfoX(Player, 0, "You need at least " .. JobCh[1].Money .. " gold.")
            return
        end
        if Job ~= 0 then
            HelpInfoX(Player, 0, "You have done First Class Advancement already!.")
            return
        end
    elseif Type == 2 then
        if Level < JobCh[2].Level then
            HelpInfoX(Player, 0, "You need at least to be Level " .. JobCh[2].Level .. ".")
            return
        end
        if Gold < JobCh[2].Money then
            HelpInfoX(Player, 0, "You need at least " .. JobCh[2].Money .. " gold.")
            return
        end
        if Job == 0 then
            HelpInfoX(Player, 0, "You haven't done First Class Advancement.")
            return
        end
        if Job >= 8 then
            HelpInfoX(Player, 0, "You have done Second Class Advancement already!.")
            return
        end
    end
    if Class == 1 then
        if CharType == 4 then
            HelpInfoX(Player, 0, "Only Lance, Carsise and Phyllis can become Swordsman.")
            return
        end
    elseif Class == 2 then
        if CharType == 2 or CharType == 4 then
            HelpInfoX(Player, 0, "Only Lance and Phyllis can become Hunter.")
            return
        end
    elseif Class == 5 then
        if CharType == 1 or CharType == 2 then
            HelpInfoX(Player, 0, "Only Phyllis and Ami can become Herbalist.")
            return
        end
    elseif Class == 4 then
        if CharType == 2 then
            HelpInfoX(Player, 0, "Only Lance, Phyllis and Ami can become Explorer.")
            return
        end
    elseif Class == 8 then
        if CharType ~= 2 then
            HelpInfoX(Player, 0, "Only Carsise can become Champion.")
            return
        end
    elseif Class == 9 then
        if CharType == 2 or CharType == 4 then
            HelpInfoX(Player, 0, "Only Lance and Phyllis can become Crusader.")
            return
        end
    elseif Class == 12 then
        if CharType == 2 or CharType == 4 then
            HelpInfoX(Player, 0, "Only Lance and Phyllis can become Sharpshooter.")
            return
        end
    elseif Class == 13 then
        if CharType == 1 or CharType == 2 then
            HelpInfoX(Player, 0, "Only Phyllis and Ami can become Cleric.")
            return
        end
    elseif Class == 14 then
        if CharType == 1 or CharType == 2 then
            HelpInfoX(Player, 0, "Only Phyllis and Ami can become Seal Master.")
            return
        end
    elseif Class == 16 then
        if CharType == 2 then
            HelpInfoX(Player, 0, "Only Lance, Phyllis and Ami can become Voyager.")
            return
        end
    end
    if Type == 1 then
        Gold = Gold - JobCh[1].Money
    elseif Type == 2 then
        Gold = Gold - JobCh[2].Money
    end
    SetChaAttr(Player, ATTR_GD, Gold)
    SetChaAttr(Player, ATTR_JOB, Class)
    RefreshCha(Player)
end
function UnsJewelTrade(Player, Type)
    local Message = nil
    if KitbagLock(Player, 0) == LUA_FALSE then
        Message = "Sorry, you cannot exchange since your inventory is locked."
    end
    local Var = UnsJewelVar[Type]
    if GetChaAttr(Player, ATTR_GD) < Var.Gold then
        Message = "Sorry, you cannot exchange since you don't have " .. Var.Gold .. "G."
    end
    for i, v in pairs(Var.Items) do
        if CheckBagItem(Player, v[1]) < v[2] then
            Message = "Sorry, you cannot exchange since you don't have " .. v[2] .. "x " .. GetItemName(v[1]) .. "."
        end
    end
    if Message ~= nil then
        HelpInfoX(Player, 0, Message)
        return
    end
    for i, v in pairs(Var.Items) do
        TakeItem(Player, 0, v[1], v[2])
    end
    SetChaAttr(Player, ATTR_GD, (GetChaAttr(Player, ATTR_GD) - Var.Gold))
    GiveItem(Player, 0, Var.JewelID, 1, 4)
    RefreshCha(Player)
end
function LifeSkillUpgrade(Player, SkillID, Type)
    local EXP = GetChaAttr(Player, ATTR_CLIFEEXP)
    local Level = GetSkillLv(Player, SkillID)
    if Type == 0 then
        HelpInfoX(Player, 0, "You currently have: " .. EXP .. " experience point(s).")
        return
    elseif Type == 1 then
        if Level ~= 0 then
            HelpInfoX(
                Player,
                0,
                "You have already learned this skill before, you can only upgrade it now by using experience points."
            )
            return
        else
            AddChaSkill(Player, SkillID, 1, 1, 0)
            if SkillID == 339 or SkillID == 338 or SkillID == 340 or SkillID == 341 then
                local ItemID = 0
                if SkillID == 339 then
                    ItemID = 1067
                end
                if SkillID == 338 then
                    ItemID = 1068
                end
                if SkillID == 340 then
                    ItemID = 1069
                end
                if SkillID == 341 then
                    ItemID = 1070
                end
                local A, B = -1, -1
                A, B = MakeItem(Player, ItemID, 1, 4)
                local Item = GetChaItem(Player, 2, B)
                SetItemAttr(Item, ITEMATTR_VAL_STR, 1)
                SetItemAttr(Item, ITEMATTR_MAXENERGY, 10000)
                SetItemAttr(Item, ITEMATTR_ENERGY, 1)
                SynChaKitbag(Player, 7)
            end
            return
        end
    elseif Type == 2 then
        local Need = Level * Resources.EXP
        if Level >= 10 then
            HelpInfoX(Player, 0, "You have reached the level limit for this life skill.")
            return
        end
        if EXP < Need then
            HelpInfoX(
                Player,
                0,
                "Failed to learn a higher level of this skill. The skill level you want to attain needs " ..
                    Need .. " experience point(s) while you only have " .. EXP .. " experience point(s)."
            )
            return
        end
        EXP = EXP - Need
        SetCharaAttr(EXP, Player, ATTR_LIFETP)
        AddChaSkill(Player, SkillID, 1, (Level + 1), 0)
        return
    end
end
function Teleport(Player, Place, MapCopy)
    local Money = GetChaAttr(Player, ATTR_GD)
    if Lv(Player) <= 40 then
        if MapCopy ~= nil then
            MoveCity(Player, Place, MapCopy)
        else
            MoveCity(Player, Place)
        end
    elseif Money >= 500 then
        Money = Money - 500
        SetCharaAttr(Money, Player, ATTR_GD)
        if MapCopy ~= nil then
            MoveCity(Player, Place, MapCopy)
        else
            MoveCity(Player, Place)
        end
    else
        SystemNotice(Player, "You need at least 500G to teleport.")
    end
end
function IllusionFragmentExchange(Player, Blueprint, Quantity)
    if KitbagLock(Player, 0) == LUA_FALSE then
        HelpInfoX(Player, 0, "Sorry, you cannot exchange since your inventory is locked.")
        return
    end
    if GetChaFreeBagGridNum(Player) < 1 then
        HelpInfoX(Player, 0, "Sorry, you cannot exchange since you don't have free inventory slots.")
        return
    end
    if CheckBagItem(Player, 2324) < Quantity then
        HelpInfoX(Player, 0, "Sorry, you cannot exchange for "..GetItemName(Blueprint).." since you need "..Quantity.." Illusion Fragments.")
        return
    end
    TakeItem(Player, 0, 2324, Quantity)
    GiveItem(Player, 0, Blueprint, 1, 0)
end
function AbaddonExchange(Player, ID)
    local Items = {}
    Items[2934] = {2846, 2847, 2848, 2849, 2850, 2851, 2852, 2853}
    Items[2935] = {2854, 2855, 2856, 2857, 2927}
    Items[2936] = {2854, 2855, 2856, 2857, 2928}
    Items[2937] = {2854, 2855, 2856, 2857, 2929}
    Items[2938] = {2854, 2855, 2856, 2857, 2930}
    Items[2939] = {2854, 2855, 2856, 2857, 2931}
    Items[2940] = {2854, 2855, 2856, 2857, 2932}
    Items[2562] = {2934, 2935}
    Items[2563] = {2934, 2936}
    Items[2564] = {2934, 2937}
    Items[2565] = {2934, 2938}
    Items[2566] = {2934, 2939}
    Items[2567] = {2934, 2940}
    Items[2404] = {2933, 2934, 2562, 2563, 2564, 2565, 2566, 2567}

    if KitbagLock(Player, 0) ~= LUA_TRUE then
        SystemNotice(Player, "Please unlock your inventory in order to do the exchange.")
        return
    end
    if Items[ID] == nil then
        SystemNotice(Player, "Please contact the administrator, there seems to be an error.")
        return
    end
    for a = 1, #Items[ID], 1 do
        if CheckBagItem(Player, Items[ID][a]) < 1 then
            SystemNotice(Player, "You cannot do the exchange since you're missing [" .. GetItemName(Items[ID][a]) .. "].")
            return
        end
    end
    for a = 1, #Items[ID], 1 do
        if TakeItem(Player, 0, Items[ID][a], 1) < 1 then
            SystemNotice(Player, "You cannot do the exchange since since it failed to remove ["..GetItemName(Items[ID][a]).."], please contact administrator with a screenshot of this.")
            return
        end
    end
    GiveItem(Player, 0, ID, 1, 4)
    Notice("Congratulations! " .. GetChaDefaultName(Player) .. " has successfully obtained " .. GetItemName(ID) .. ".")
end
