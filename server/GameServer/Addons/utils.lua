function SendMoney(cha, amount)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        local Money_Have = GetChaAttr(cha, ATTR_GD)
        if (Money_Have + amount) > 2000000000 then
            SystemNotice(cha, "Cant add more money !")
            return
        end
        AddMoney(cha, 0, amount)
    end
end

function DeleteMoney(cha, amount)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        local Money_Have = GetChaAttr(cha, ATTR_GD)
        if (Money_Have - amount) <= 0 then
            SystemNotice(cha, "Cant take more money !")
            return
        end
        TakeMoney(cha, 0, amount)
    end
end

function SendToPrison(cha)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        local chaname = GetChaDefaultName(cha)
        MoveCity(cha, "Island Prison")
        SystemNotice(cha, "Good Night, stay here for Unknown hours !")
    end
end

function KillPlayer(cha_id)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        Hp_Endure_Dmg(cha, 10000000)
        RefreshCha(cha)
    end
end

function RevivePlayer(cha)
	if(type(cha) ~= 'userdata') then
		cha = GetPlayerByName(cha)
	end
	if(cha ~= nil)then
		local ChaName = GetChaDefaultName(cha)
		SetRelive(cha, cha,  10, "Player"..ChaName.."\n\n wish to revive you. Accept?")
	end
end

function SendItem(role, player, item_id, qty, qlt)
    if (type(player) ~= "userdata") then
        player = GetPlayerByName(player)
    end
    if player == nil then
        HelpInfoX(role, 0, "role is offline or in another gameserver try again later!")
        return 0
    end
    if (player ~= nil) then
        if HasLeaveBagGrid(player, 1) ~= LUA_TRUE or KitbagLock(player, 0) == LUA_FALSE then
            GiveItemX(player, 0, item_id, qty, qlt)
            HelpInfoX(role, 0, "Storage box:You gave role ["..GetChaDefaultName(player).."]["..GetItemName(item_id).."x["..qty.."]")
            RefreshCha(player)
        else
            MakeItem(player, item_id, qty, qlt)
            HelpInfoX(role, 0, "You gave role ["..GetChaDefaultName(player).."]["..GetItemName(item_id).."x["..qty.."]")
        end
        local admin = GetChaDefaultName(role)
        LG("SendItem", "GM:["..admin.."] gave player: ["..GetChaDefaultName(player).."], Item: ["..GetItemName(item_id).."] , qty: "..qty.."x , successfuly")
        RefreshCha(player)
    end
end
--[[
function TakeItem(role, player, item_id, amount)
    if (type(role) ~= "userdata" and type(player) ~= "userdata") then
        role = GetPlayerByName(role)
        player = GetPlayerByName(player)
    end
    if (role ~= nil and player ~= nil) then
        local CheckHasItem = CheckBagItem(player, item_id)
        if (CheckHasItem < amount) then
            SystemNotice(role, "Item amount too large than player inventory !")
            return
        else
            TakeItem(player, 0, item_id, amount)
            SystemNotice(player, "Administrator deleted your "..GetItemName(item_id).." item.")
            SystemNotice(role, "Succesfully deleted item !")
        end
        RefreshCha(player)
    end
end
--]]
function FullBuffs(role)
    if (type(role) ~= "userdata") then
        role = GetPlayerByName(role)
    end
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Not usable on the sea.")
        return 0
    end
    if GetChaStateLv(role, STATE_GAMEMASTER) == 1 then
        RemoveState(role, STATE_SHPF)
        RemoveState(role, STATE_XLZH)
        RemoveState(role, STATE_TSHD)
        RemoveState(role, STATE_FZLZ)
        RemoveState(role, STATE_MLCH)
        RemoveState(role, STATE_JSFB)
        RemoveState(role, STATE_GAMEMASTER)
        RefreshCha(role)
        ReAll(role)
    else
        AddState(role, role, STATE_SHPF, 10, 3600)
        AddState(role, role, STATE_XLZH, 10, 3600)
        AddState(role, role, STATE_TSHD, 10, 3600)
        AddState(role, role, STATE_FZLZ, 10, 3600)
        AddState(role, role, STATE_MLCH, 10, 3600)
        AddState(role, role, STATE_JSFB, 10, 3600)
        AddState(role, role, STATE_GAMEMASTER, 1, 3600)
        RefreshCha(role)
        ReAll(role)
    end
end

function SetPlayerRecord(cha, record_id)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        SetRecord(cha, record_id)
    end
end

function DelPlayerRecord(cha, record_id)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        ClearRecord(cha, record_id)
    end
end

function ClearPlayerMission(cha, record_id)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        ClearMission(cha, record_id)
    end
end

function GivePlayerLv(cha, exp)
	if(type(cha) ~= 'userdata') then
		cha = GetPlayerByName(cha)
	end
	if(cha ~= nil)then
		AddExp(cha, cha, exp, exp)
		SystemNotice(cha,"Congratulations!")
		RefreshCha(cha)
	end
end

function GivePlayerSkill(cha, skill_id, skill_lv, skill_type)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if (cha ~= nil) then
        AddChaSkill(cha, skill_id, skill_lv, 1, skill_type)
        RefreshCha(cha)
    end
end

function KickPlayer(cha)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
    if cha ~= nil then
        KickCha(cha)
    end
end

function BanPlayer(role, player)
    if (type(player) ~= "userdata") then
        player = GetPlayerByName(player)
    end
	if player ~= nil then
		BanActRole(player)
		KickCha(player)
		LG("BanPlayer", "admin: ["..GetChaDefaultName(role).."] banned player: ["..GetChaDefaultName(player).."]")
	else 
		return
	end
end

function SummonMonster(role, monster_id, count)
    local x, y = GetChaPos(role)
    local r = math.random(0, 360)
    for i = 1, count, 1 do
        CreateCha(monster_id, x, y, r, 216000)
    end
end

function ClearSlots(role, count)
    for i = 0, count - 1, 1 do
        local Item = GetChaItem(role, 2, i)
        local item_id = GetItemID(Item)
        local itemNum = CheckBagItem(role, item_id)
        if itemNum > 0 then
            RemoveChaItem(role, item_id, itemNum, 2, -1, 2, 1)
        end
    end
end

function ClosePlayerClient(cha)
    if (type(cha) ~= "userdata") then
        cha = GetPlayerByName(cha)
    end
	CloseClient(cha)
	LG("ClosePlayerClient", "player: ["..GetChaDefaultName(cha).."] was disconnected by force!")
end

function Logs(Filename, Text)
	local LogFile = {}
	LogFile.path = GetResPath("script/cache/")
	local file = LogFile.path..Filename..".txt"
	LogFile = io.open(file,'a')
	LogFile:write(""..Text.."\n")
	LogFile:close()
end

function RebuildItemInfo(a,b)
	local var = {}
	for k = a,b do
		var[k] = GetItemName(k)
		if var[k] == "Unknown" then
			local data = {}
			data[2] = "DELETE ME"
			data[3] = "error"
			data[4] = 10130005
			data[5] = 0
			data[6] = 0
			data[7] = 0
			data[8] = 0
			data[9] = 0
			data[10] = 0
			data[11] = 0
			data[12] = 0
			data[13] = 0
			data[14] = 0
			data[15] = 0
			data[16] = 0
			data[17] = 0
			data[18] = 1
			data[19] = 0
			data[20] = 1
			data[21] = 1
			data[22] = 0
			data[23] = 0
			data[24] = "-1,-2,-2,-2"
			data[25] = 0
			data[26] = "-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2"
			data[27] = 0
			data[28] = 0
			data[29] = "-1,-2,-2,-2,-2,-2,-2,-2,-2,-2"
			data[30] = "-1,-2,-2,-2,-2,-2,-2,-2,-2,-2"
			data[31] = 0
			data[32] = 0
			data[33] = 0
			data[34] = 0
			data[35] = 0
			data[36] = 0
			data[37] = 0
			data[38] = 0
			data[39] = 0
			data[40] = 0
			data[41] = 0
			data[42] = 0
			data[43] = 0
			data[44] = 0
			data[45] = 0
			data[46] = 0
			data[47] = 0
			data[48] = 0
			data[49] = 0
			data[50] = 0
			data[51] = 0
			data[52] = 0
			data[53] = "0,0"
			data[54] = "0,0"
			data[55] = "0,0"
			data[56] = "0,0"
			data[57] = "0,0"
			data[58] = "0,0"
			data[59] = "0,0"
			data[60] = "0,0"
			data[61] = "0,0"
			data[62] = "0,0"
			data[63] = "0,0"
			data[64] = "0,0"
			data[65] = "0,0"
			data[66] = "0,0"
			data[67] = "0,0"
			data[68] = "0,0"
			data[69] = "0,0"
			data[70] = "0,0"
			data[71] = "0,0" 
			data[72] = "0,0"
			data[73] = "0,0"
			data[74] = "0,0"
			data[75] = 0
			data[76] = "0,0"
			data[77] = "0,0"
			data[78] = 0
			data[79] = 0
			data[80] = 0
			data[81] = 0
			data[82] = 0
			data[83] = 0
			data[84] = 0
			data[85] = 0
			data[86] = 0
			data[87] = 0
			data[88] = 0
			data[89] = "0,0,0,0,0,0,0,0"
			data[90] = "0,0,0,0,0,0,0,0"
			data[91] = 0,0
			data[92] = 0,0
			data[93] = 0,0
			data[94] = "This is junk item. Please delete this item."
			data[95] = 0
			Logs("RebuildItemInfo",""..k.."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\t"..data[95].."")
		else
			local ItemList	= {}
			local fp = assert(io.open (GetResPath("/ItemInfo.txt")))
			for line in fp:lines() do
				local position = string.find(line, "[ \t]*//")
				if (position ~= 1) then
					local data	= split(line,"\t")
					local ItemID	= tonumber(data[1])
					if k == ItemID then
						Logs("ItemInfo",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\t"..data[95].."")
					end
				end
			end
		end
	end
end

function RebuildCharacterInfo(a,b)
    local file = GetResPath("/script/cache/CharacterInfo.txt")
    LogFile = io.open(file, "a")
    local ret = {}
    local fp = assert(io.open(GetResPath("/CharacterInfo.txt")))
    for line in fp:lines() do
        local position = string.find(line, "[ \t]*//")
        if (position ~= 1) then
            local data = Split(line, "\t")
            local mob_id = tonumber(data[1])
            for r = a, b do
                if r == mob_id then
                    print(r)
                    for k = 1,124 do
                        ret[k] = data[k]
                        local asd = ""..ret[k].."\t"
                        LogFile:write(asd)
                    end
                    LogFile:write("\n")
                end
            end
        end
    end
    LogFile:close()
end

function CreateCacheItemInfo(First, Last)
	local file 	= GetResPath("/script/cache/ItemInfoCache.txt")
	LogFile 	= io.open(file,'a')

	local ret	= {}
	local ItemList	= {}

	local fp = assert(io.open (GetResPath("/ItemInfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID = tonumber(data[1])
			local ItemName = data[2]

			for r = First, Last do
				if r == ItemID then
					local itemcache = ""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\n"
					LogFile:write(itemcache)
				end
			end
		end
	end
	LogFile:close()
end

function FindItemByItemType(ItemStart,ItemEnd,Type)
	local ItemList	= {}
	local fp = assert(io.open (GetResPath("/ItemInfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID	= tonumber(data[1])
			local ItemName	= data[2]
			local ItemType	= tonumber(data[11])

			for i = tonumber(ItemStart),tonumber(ItemEnd) do
				if i == ItemID then
					ItemList[ItemID]	= ItemID
					ItemList[ItemName] 	= ItemName
					ItemList[ItemType]	= ItemType

					if ItemList[ItemType] == Type then
						Logs("ItemType("..ItemList[ItemType]..")", "Item ID: "..ItemList[ItemID].."	Name: "..ItemList[ItemName].."")
					end
				end
			end
		end
	end
end

function FindItemHasFunction(a,b)
    local file = GetResPath("/script/cache/ItemInfoHasFunction.txt")
    LogFile = io.open(file, "a")

    local ret = {}
    local ItemList = {}

    local fp = assert(io.open(GetResPath("/ItemInfo.txt")))
    for line in fp:lines() do
        local position = string.find(line, "[ \t]*//")
        if (position ~= 1) then
            local data = split(line, "\t")
            local ItemID = tonumber(data[1])
            local ItemType = data[87]

            for r = a,b do
                if r == ItemID then
                    if ItemType ~= "0" then
                        for k = 1, 95 do
                            ret[k] = data[k]
                            local asd = ""..ret[k].." \t"
                            LogFile:write(asd)
                        end
                        LogFile:write("\n")
                    end
                end
            end
        end
    end
    LogFile:close()
end

function CreateCacheCharInfo(a,b)
    local file = GetResPath("/script/cache/C_CharacterInfo.txt")
    LogFile = io.open(file, "a")

    local ret = {}
    local ItemList = {}

    local fp = assert(io.open(GetResPath("/CharacterInfo.txt")))
    for line in fp:lines() do
        local position = string.find(line, "[ \t]*//")
        if (position ~= 1) then
            local data = split(line, "\t")
            local MonsterID = tonumber(data[1])
            local MonsterName = data[2]

            for r = a,b do
                if r == MonsterID then
                    local monstercache = ""..data[1].."\t"..data[2].."\n"
                    LogFile:write(monstercache)
                end
            end
        end
    end
    LogFile:close()
end

function MissingLinesInfo(a,b)
	local ItemList	= {}
	local fp = assert(io.open (GetResPath("/ItemInfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID	= tonumber(data[1])
			local ItemName	= data[2]
			local ItemType	= tonumber(data[11])

			for i = tonumber(a),tonumber(b) do
				if i == ItemID then
				print(i)
					ItemList[ItemID]	= ItemID
					ItemList[ItemName] 	= ItemName
					Logs("MissingLines",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\t"..data[95].."")
				end
			end
		end
	end
end

PKO = {}
function RebuildSkillEff(ItemStart,ItemEnd)
	for k = tonumber(ItemStart),tonumber(ItemEnd) do
		print(k)
		if PKO.SkillEff[k] == nil then
			local data = {}
			data[1]= k
			data[2]= "Empty"	
			data[3]= 1	
			data[4]= "-1,1;-2,-2;-2,-2;-2,-2"	
			data[5]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[6]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[7]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[8]= "-1,-1,0"	
			data[9]= 0	
			data[10]= 0	
			data[11]= 0	
			data[12]= -1	
			data[13]= "-1,-1;-2,-2;-2,-2"	
			data[14]= 1	
			data[15]= 1	
			data[16]= 1	
			data[17]= 0	
			data[18]= 0	
			data[19]= 0	
			data[20]= 0	
			data[21]= 0	
			data[22]= 0	
			data[23]= 0	
			data[24]= 0	
			data[25]= 0	
			data[26]= 0	
			data[27]= 0	
			data[28]= 0	
			data[29]= 0	
			data[30]= 0	
			data[31]= 0	
			data[32]= 0
			Logs("RebuildSkillEff",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."")
		else
			local fp = assert(io.open (GetResPath("/skilleff.txt") ) )
			for line in fp:lines() do
				local position = string.find(line, "[ \t]*//")
				if (position ~= 1) then
					local data		= split(line,"\t")
					local SkillID	= tonumber(data[1])
					if k == SkillID then
						Logs("RebuildSkillEff",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."")
					end
				end
			end
		end
	end
end

function FixSkillInfo(ItemStart,ItemEnd)
	for k = tonumber(ItemStart),tonumber(ItemEnd) do
		print(k)
		if PKO.SkillInfo[k] == nil then
			local data = {}
			data[1]= k
			data[2]= "Empty"	
			data[3]= 1	
			data[4]= "-1,1;-2,-2;-2,-2;-2,-2"	
			data[5]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[6]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[7]= "1,-1;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2;-2,-2"	
			data[8]= "-1,-1,0"	
			data[9]= 0	
			data[10]= 0	
			data[11]= 0	
			data[12]= -1	
			data[13]= "-1,-1;-2,-2;-2,-2"	
			data[14]= 1	
			data[15]= 1	
			data[16]= 1	
			data[17]= 0	
			data[18]= 0	
			data[19]= 0	
			data[20]= 0	
			data[21]= 0	
			data[22]= 0	
			data[23]= 0	
			data[24]= 0	
			data[25]= 0	
			data[26]= 0	
			data[27]= 0	
			data[28]= 0	
			data[29]= 0	
			data[30]= 0	
			data[31]= 0	
			data[32]= 0	
			data[33]= 0	
			data[34]= 0	
			data[35]= 0	
			data[36]= 0	
			data[37]= 0	
			data[38]= 0	
			data[39]= 0	
			data[40]= 0	
			data[41]= 0	
			data[42]= 0	
			data[43]= 0	
			data[44]= 0	
			data[45]= 0	
			data[46]= 1	
			data[47]= 1	
			data[48]= "-1,0,0,0,0,0,0,0,0,0"	
			data[49]= 0	
			data[50]= -1	
			data[51]= "-1,0"	
			data[52]= "0,0"	
			data[53]= "0,0"	
			data[54]= -1	
			data[55]= "0,0"	
			data[56]= "0,0"	
			data[57]= -1	
			data[58]= -1	
			data[59]= -1	
			data[60]= 0	
			data[61]= 0	
			data[62]= -1	
			data[63]= 0	
			data[64]= 0	
			data[65]= 0	
			data[66]= 0	
			data[67]= 0	
			data[68]= "error.tga"	
			data[69]= 0	
			data[70]= "0,0"	
			data[71]= 0	
			data[72]= 0	
			data[73]= 0
			Logs("RebuildSkillInfo",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."")
		else
			local fp = assert(io.open (GetResPath("/SkillInfo.txt") ) )
			for line in fp:lines() do
				local position = string.find(line, "[ \t]*//")
				if (position ~= 1) then
					local data		= split(line,"\t")
					local SkillID	= tonumber(data[1])
					if k == SkillID then
						Logs("RebuildSkillInfo",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."")
					end
				end
			end
		end
	end
end

PKO.SkillInfo = {}
local fp = assert(io.open(GetResPath('SkillInfo.txt')))
for line in fp:lines() do
	local position = string.find(line, '[ \t]*//')
	if position ~= 1 then
		local column	= split(line, '\t')
		local id		= tonumber(column[1])
		if(PKO.SkillInfo[id] == nil)then
			PKO.SkillInfo[id] = {}
			for k = 1,73 do
				PKO.SkillInfo[id][k] = column[k]
			end
		end
	end
end

function GetSkillName(skill)
	local SkillName = 0
	if PKO.SkillInfo[skill] ~= nil then
		SkillName = PKO.SkillInfo[skill][2]
	end
	return SkillName
end

function FetchDataFromItemInfo(ItemStart, ItemEnd)
	local count = 0
	local fp = assert(io.open (GetResPath("/ItemInfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID	= tonumber(data[1])
			local ItemType	= data[11]
			local Race		= data[24]
			local Class		= data[26]
			local Durability = data[77]
			local Function	= data[87]
			for i = tonumber(ItemStart),tonumber(ItemEnd) do
				if i == ItemID then
					if Function == "GiveRandomItems" then
						Logs("GiveRandomItems",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..data[11].."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..data[24].."\t"..data[25].."\t"..data[26].."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\t"..data[95].."")
						count = count + 1
					end
				end
			end
		end
	end
	print(""..count.." Items(s) found!")
end

function EditDataFromItemInfo(ItemStart, ItemEnd)
	local count = 0
	local fp = assert(io.open (GetResPath("/script/cache/CrusaderBoots.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID	= tonumber(data[1])
			local ItemType	= data[11]
			local Race		= data[24]
			local Class		= data[26]
			local Durability = data[77]
			for i = tonumber(ItemStart),tonumber(ItemEnd) do
				if i == ItemID then
					if Durability ~= "25000,25000" then
						if ItemType == "24" and Race == "1" and Class == "9" then
							Race = "1,2,3"
							Logs("CrusaderBootsEdited",""..data[1].."\t"..data[2].."\t"..data[3].."\t"..data[4].."\t"..data[5].."\t"..data[6].."\t"..data[7].."\t"..data[8].."\t"..data[9].."\t"..data[10].."\t"..ItemType.."\t"..data[12].."\t"..data[13].."\t"..data[14].."\t"..data[15].."\t"..data[16].."\t"..data[17].."\t"..data[18].."\t"..data[19].."\t"..data[20].."\t"..data[21].."\t"..data[22].."\t"..data[23].."\t"..Race.."\t"..data[25].."\t"..Class.."\t"..data[27].."\t"..data[28].."\t"..data[29].."\t"..data[30].."\t"..data[31].."\t"..data[32].."\t"..data[33].."\t"..data[34].."\t"..data[35].."\t"..data[36].."\t"..data[37].."\t"..data[38].."\t"..data[39].."\t"..data[40].."\t"..data[41].."\t"..data[42].."\t"..data[43].."\t"..data[44].."\t"..data[45].."\t"..data[46].."\t"..data[47].."\t"..data[48].."\t"..data[49].."\t"..data[50].."\t"..data[51].."\t"..data[52].."\t"..data[53].."\t"..data[54].."\t"..data[55].."\t"..data[56].."\t"..data[57].."\t"..data[58].."\t"..data[59].."\t"..data[60].."\t"..data[61].."\t"..data[62].."\t"..data[63].."\t"..data[64].."\t"..data[65].."\t"..data[66].."\t"..data[67].."\t"..data[68].."\t"..data[69].."\t"..data[70].."\t"..data[71].."\t"..data[72].."\t"..data[73].."\t"..data[74].."\t"..data[75].."\t"..data[76].."\t"..data[77].."\t"..data[78].."\t"..data[79].."\t"..data[80].."\t"..data[81].."\t"..data[82].."\t"..data[83].."\t"..data[84].."\t"..data[85].."\t"..data[86].."\t"..data[87].."\t"..data[88].."\t"..data[89].."\t"..data[90].."\t"..data[91].."\t"..data[92].."\t"..data[93].."\t"..data[94].."\t"..data[95].."")
							count = count + 1
						end
					end
				end
			end
		end
	end
	print("Edited "..count.." Items(s)!")
end

-- Website Database .txt generator
function SQLiteCacheSkillInfo(First, Last)
	local file 	= GetResPath("/script/cache/skillinfo.txt")
	LogFile 	= io.open(file,'a')
	local ret	= {}
	local ItemList	= {}

	local fp = assert(io.open (GetResPath("/skillinfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID = tonumber(data[1])
			local ItemName = data[2]

			for r = First, Last do
				if r == ItemID then
					local skillcache = ""..data[1].."\t"..data[2].."\t"..data[10].."\t"..data[71].."\t"..data[68].."\n"
					LogFile:write(skillcache)
				end
			end
		end
	end
	LogFile:close()
end

function SQLiteCacheCharacterInfo(First, Last)
	local file 	= GetResPath("/script/cache/characterinfo.txt")
	LogFile 	= io.open(file,'a')
	local ret	= {}
	local ItemList	= {}

	local fp = assert(io.open (GetResPath("/CharacterInfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID = tonumber(data[1])
			local ItemName = data[2]

			for r = First, Last do
				if r == ItemID then
					local charcache = ""..data[1].."\t "..data[2].."\t "..data[61].."\t "..data[62].."\t "..data[64].."\t "..data[66].."\t "..data[67].."\t "..data[68].."\t "..data[69].."\t "..data[70].."\t "..data[71].."\t "..data[72].."\t "..data[74].."\t "..data[75].."\t "..data[76].."\t "..data[77].."\t "..data[56].."\t "..data[79].."\t "..data[81].."\t "..data[82].."\t "..data[83].."\t "..data[84].."\t "..data[85].."\t "..data[74].."\t "..data[78].."\t "..data[54].."\t "..data[91].."\n"
					LogFile:write(charcache)
				end
			end
		end
	end
	LogFile:close()
end

function SQLiteCacheItemInfo(First, Last)
	local file 	= GetResPath("/script/cache/iteminfo.txt")
	LogFile 	= io.open(file,'a')
	local ret	= {}
	local ItemList	= {}

	local fp = assert(io.open (GetResPath("/iteminfo.txt") ) )
	for line in fp:lines() do
	local position = string.find(line, "[ \t]*//")
		if (position ~= 1) then
			local data	= split(line,"\t")
			local ItemID = tonumber(data[1])
			local ItemName = data[2]

			for r = First, Last do
				if r == ItemID then
					local itemcache = ""..data[1].."\t "..data[2].."\t "..data[94].."\t "..data[3].."\t "..data[87].."\t "..data[24].."\t "..data[26].."\t "..data[25].."\t "..data[23].."\t "..data[21].."\t "..data[17].."\t "..data[19].."\t "..data[20].."\t "..data[18].."\t "..data[32].."\t "..data[33].."\t "..data[34].."\t "..data[35].."\t "..data[36].."\t "..data[38].."\t "..data[40].."\t "..data[41].."\t "..data[42].."\t "..data[43].."\t "..data[44].."\t "..data[45].."\t "..data[46].."\t "..data[47].."\t "..data[49].."\t "..data[50].."\t "..data[51].."\t "..data[53].."\t "..data[54].."\t "..data[55].."\t "..data[56].."\t "..data[57].."\t "..data[59].."\t "..data[61].."\t "..data[62].."\t "..data[63].."\t "..data[64].."\t "..data[65].."\t "..data[66].."\t "..data[67].."\t "..data[68].."\t "..data[70].."\t "..data[71].."\t "..data[72].."\t "..data[74].."\t "..data[11].."\n"
					LogFile:write(itemcache)
				end
			end
		end
	end
	LogFile:close()
end
