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

function GetSkillName(skill)
	local rec = GetSkillRecord(skill)
	if rec ~= nil then
		return rec:GetName()
	end
	return 0
end
