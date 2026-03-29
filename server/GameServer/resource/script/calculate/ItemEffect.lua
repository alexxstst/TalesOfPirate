print("-- [Loading] Item Effect")
function ItemUse_IDBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    local cha_name = GetChaDefaultName(role)
    local count = 1

    if lv < 60 then
        SystemNotice(role, "Currently lower then Lv 60. Item usage failed!")
        UseItemFailed(role)
    else
        SystemNotice(role, "Event have ended, Thank You for your participation. Please refer to Pirate King Online official website for future events!")
    end
end
function ItemUse_SaintCloth(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 3 then
        SystemNotice(role, "Requires 4 slots in inventory to open Chest of Kylin")
        UseItemFailed(role)
        return
    end
    if cha_type == 4 then
        GiveItem(cha, 0, 828, 1, 0)
    end
    GiveItem(cha, 0, 825, 1, 0)
    GiveItem(cha, 0, 826, 1, 0)
    GiveItem(cha, 0, 827, 1, 0)
end
function ItemUse_SCBoxYXTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 2 then
        SystemNotice(role, "To open the chest requires 3 empty slots")
        UseItemFailed(role)
        return
    end

    if cha_type == 1 then
        GiveItem(cha, 0, 0395, 1, 95)
        GiveItem(cha, 0, 0587, 1, 95)
        GiveItem(cha, 0, 0747, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxLSTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 3 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open treasure chest")
        UseItemFailed(role)
        return
    end
    if cha_type == 2 then
        GiveItem(cha, 0, 0397, 1, 95)
        GiveItem(cha, 0, 0829, 1, 95)
        GiveItem(cha, 0, 0603, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxHYTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 2 then
        SystemNotice(role, "To open the chest requires 3 empty slots")
        UseItemFailed(role)
        return
    end

    if cha_type == 1 or cha_type == 3 then
        GiveItem(cha, 0, 0399, 1, 95)
        GiveItem(cha, 0, 0589, 1, 95)
        GiveItem(cha, 0, 0749, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxFYSTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 3 then
        SystemNotice(role, "Lack of 4 inventory slots. Unable to open chest.")
        UseItemFailed(role)
        return
    end

    if cha_type == 3 then
        GiveItem(cha, 0, 0401, 1, 95)
        GiveItem(cha, 0, 0591, 1, 95)
        GiveItem(cha, 0, 0751, 1, 95)
    elseif cha_type == 4 then
        GiveItem(cha, 0, 0403, 1, 95)
        GiveItem(cha, 0, 0593, 1, 95)
        GiveItem(cha, 0, 0753, 1, 95)
        GiveItem(cha, 0, 2218, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxSZZTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 3 then
        SystemNotice(role, "Lack of 4 inventory slots. Unable to open chest.")
        UseItemFailed(role)
        return
    end

    if cha_type == 3 then
        GiveItem(cha, 0, 0405, 1, 95)
        GiveItem(cha, 0, 0595, 1, 95)
        GiveItem(cha, 0, 0755, 1, 95)
    elseif cha_type == 4 then
        GiveItem(cha, 0, 0407, 1, 95)
        GiveItem(cha, 0, 0597, 1, 95)
        GiveItem(cha, 0, 0757, 1, 95)
        GiveItem(cha, 0, 2220, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxHHSTZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet <= 3 then
        SystemNotice(role, "Lack of 4 inventory slots. Unable to open chest.")
        UseItemFailed(role)
        return
    end

    if cha_type == 1 or cha_type == 3 then
        GiveItem(cha, 0, 0409, 1, 95)
        GiveItem(cha, 0, 0599, 1, 95)
        GiveItem(cha, 0, 0759, 1, 95)
    elseif cha_type == 4 then
        GiveItem(cha, 0, 0412, 1, 95)
        GiveItem(cha, 0, 0601, 1, 95)
        GiveItem(cha, 0, 0761, 1, 95)
        GiveItem(cha, 0, 2222, 1, 95)
    else
        SystemNotice(role, "Body size does not match. Unable to open chest")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SCBoxRYBZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end
    GiveItem(cha, 0, 0109, 1, 95)
end
function ItemUse_SCBoxXMC(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0111, 1, 95)
end
function ItemUse_SCBoxLQJ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0113, 1, 95)
end
function ItemUse_SCBoxLYJ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0115, 1, 95)
end
function ItemUse_SCBoxFHG(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0117, 1, 95)
end
function ItemUse_SCBoxLXHZ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0119, 1, 95)
end
function ItemUse_SCBoxYCJ(role, Item)
    local cha = TurnToCha(role)
    local cha_type = GetChaTypeID(cha)

    local Item_CanGet = GetChaFreeBagGridNum(cha)

    if Item_CanGet == 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    GiveItem(cha, 0, 0150, 1, 95)
end
function FairyFruit(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use fairy fruits while using boat.")
        UseItemFailed(role)
        return
    end
    local Fairy = GetEquipItemP(role, 16)
    if Fairy == nil then 
        SystemNotice(role, "Your fairy slot is empty!")
        UseItemFailed(role)
        return
    end

    local LvCheck = FairyLevel_Normal(role, Fairy)
    if LvCheck == 0 then
        SystemNotice(role, "Fairy reached maximum level, cannot raise any more.")
        UseItemFailed(role)
        return
    end
    if GetFairyLevel(Fairy) >= Server.Fairy.Level.Maximum then
        SystemNotice(role, "Reached server fairy maximum level, cannot raise any more.")
        UseItemFailed(role)
        return
    end
    local FairyID = GetItemID(Fairy)
    if FairyID == Server.Fairy.ID["Angela"] or FairyID == Server.Fairy.ID["Angela JR"] then
        SystemNotice(role, "Cannot feed this fruit to this fairy.")
        UseItemFailed(role)
        return
    end
    local ItemID = GetItemID(Item)
    local Check_Exp = 0
    local Elf_MaxEXP = GetItemAttr(Fairy, ITEMATTR_MAXENERGY)
    if GetItemType(Item) == 58 and GetItemType(Fairy) == 59 then
        Check_Exp = CheckFairyEXP(role, Fairy)
        if Check_Exp == 0 then
            SystemNotice(role, "Fairy hasn't reached maximum growth, cannot level up.")
            UseItemFailed(role)
        else
            FairyLevel(role, Fairy, Server.Fairy.Fruit[ItemID].ATTR, Server.Fairy.Fruit[ItemID].LV, 0)
        end
    end
end
function FairyRation(role, Item)
    local Boat = GetCtrlBoat(role)
    if Boat ~= nil then
        SystemNotice(role, "Cannot feed fairy while using boat.")
        UseItemFailed(role)
        return
    end
    local Fairy = GetEquipItemP(role, 16)
    if Fairy == nil then
        SystemNotice(role, "Your fairy slot is empty!,")
        UseItemFailed(role)
    end

    local FairyID = GetItemID(Fairy)
    local ItemID = GetItemID(Item)
    if FairyID == Server.Fairy.ID["Angela"] or FairyID == Server.Fairy.ID["Angela JR"] then
        if ItemID < 5644 then
            SystemNotice(role, "Cannot feed this fruit to this fairy.")
            UseItemFailed(role)
            return
        end
    end
    local Num = Server.Fairy.Ration[ItemID] * 50
    if GetItemType(Item) == 57 and GetItemType(Fairy) == 59 then
        if GetItemAttr(Fairy, ITEMATTR_URE) < GetItemAttr(Fairy, ITEMATTR_MAXURE) then
            GiveFairyStamina(role, Fairy, Num)
        else
            SystemNotice(role, "Fairy stamina is full.")
            UseItemFailed(role)
            return
        end
    end
    SynLook(role)
    RefreshCha(role)
end
function FairySkillRemoval(role, Item)
	local Fairy = GetEquipItemP(role, 16)
	local FairyType = GetItemType(Fairy)
	local ItemID = GetItemID(Item)
	if FairyType == 59 then
		local Num = GetItemForgeParam(Fairy, 1)
		local SkillID = {GetNum_Part2(Num), GetNum_Part4(Num), GetNum_Part6(Num)}
		local Book = {}
		Book[9609] = {Param = 1}
		Book[9610] = {Param = 2}
		Book[9611] = {Param = 3}
		if (Book[ItemID]) then
			if (Book[ItemID].Param == 1) then
				if (SkillID[1] > 0) then
					Num	= SetNum_Part2(Num, 0)
					Num	= SetNum_Part3(Num, 0)
					SetItemForgeParam(Fairy, 1, Num)	
				end
			end	
			if (Book[ItemID].Param == 2) then
				if (SkillID[2] > 0) then
					Num	= SetNum_Part4(Num, 0)
					Num	= SetNum_Part5(Num, 0)
					SetItemForgeParam(Fairy, 1, Num)						
				end
			end	
			if (Book[ItemID].Param == 3) then
				if (SkillID[3] > 0) then
					Num	= SetNum_Part6(Num, 0)
					Num	= SetNum_Part7(Num, 0)
					SetItemForgeParam(Fairy, 1, Num)						
				end
			end
			BickerNotice(role, "Succesfully removed fairy skill slot "..Book[ItemID].Param.."!")			
			SynLook(role)
			RefreshCha(role)
		else
			SystemNotice(role, "Internal error!")
			UseItemFailed(role)
			return
		end
	else
		SystemNotice(role, "Item is not a Fairy!")
		UseItemFailed(role)
	end
end
function ItemUse_CJBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "To open a Chest requires 1 empty slot")
        UseItemFailed(role)
        return
    end
    local r1, r2 = MakeItem(role, C1, 1, 4)
    local Item_newJL = GetChaItem(role, 2, r2)
    local Item_newJLID = GetItemID(Item_newJL)

    local str_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_STR)
    local con_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_CON)
    local agi_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_AGI)
    local dex_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_DEX)
    local sta_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_STA)

    local Num_JL = GetItemForgeParam(Item_newJL, 1)
    Num_JL = TansferNum(Num_JL)
    local Part1_JL = GetNum_Part1(Num_JL)
    local Part2_JL = GetNum_Part2(Num_JL)
    local Part3_JL = GetNum_Part3(Num_JL)
    local Part4_JL = GetNum_Part4(Num_JL)
    local Part5_JL = GetNum_Part5(Num_JL)
    local Part6_JL = GetNum_Part6(Num_JL)
    local Part7_JL = GetNum_Part7(Num_JL)
    if
        Item_newJLID == 231 or Item_newJLID == 232 or Item_newJLID == 233 or Item_newJLID == 234 or Item_newJLID == 235 or
            Item_newJLID == 236 or
            Item_newJLID == 237 or
            Item_newJLID == 681
     then
        Part1_JL = 1
        Num_JL = SetNum_Part1(Num_JL, 1)
        SetItemForgeParam(Item_newJL, 1, Num_JL)
    end
    str_JLone = N1
    con_JLone = N2
    agi_JLone = N3
    dex_JLone = N4
    sta_JLone = N5
    local new_lv = N1 + N2 + N3 + N4 + N5
    local new_MAXENERGY = 240 * (new_lv + 1)
    if new_MAXENERGY > 6480 then
        new_MAXENERGY = 6480
    end
    local new_MAXURE = 5000 + 1000 * new_lv
    if new_MAXURE > 32000 then
        new_MAXURE = 32000
    end
    SetItemAttr(Item_newJL, ITEMATTR_VAL_STR, str_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_CON, con_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_AGI, agi_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_DEX, dex_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_STA, sta_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_MAXENERGY, new_MAXENERGY)
    SetItemAttr(Item_newJL, ITEMATTR_MAXURE, new_MAXURE)
    SetItemAttr(Item_newJL, ITEMATTR_ENERGY, new_MAXENERGY)
    SetItemAttr(Item_newJL, ITEMATTR_URE, new_MAXURE)
    local cha_name = GetChaDefaultName(role)

    LG("star_CJBOX", cha_name, C1, N1, N2, N3, N4, N5)
end
function ItemUse_wxlh(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "To open a Chest requires 1 empty slot")
        UseItemFailed(role)
        return
    end
    local r1, r2 = MakeItem(role, SI, SN, SE)
    local Item_star = GetChaItem(role, 2, r2)
    local num = {}
    local numAttr = {}
    local b = 0
    local a = {}
    for b = 1, 5, 1 do
        num[b] = 0
        numAttr[b] = 0
    end
    b = 0
    for i = 1, 47, 1 do
        a[i] = GetItemAttr(Item_star, i)
        if a[i] ~= 0 then
            b = b + 1
            num[b] = i
            numAttr[b] = a[i]
        end
    end
    SetItemAttr(Item_star, num[1], SA1)
    SetItemAttr(Item_star, num[2], SA2)
    SetItemAttr(Item_star, num[3], SA3)
    SetItemAttr(Item_star, num[4], SA4)
    SetItemAttr(Item_star, num[5], SA5)
end
function ItemUse_65BBBox(role, Item)
    local job = GetChaAttr(role, ATTR_JOB)
    local cha_type = GetChaTypeID(role)
    if job == 0 then
        SystemNotice(role, "")
        UseItemFailed(role)
        return
    else
        if job == 12 or job == 2 then
            GiveItem(role, 0, 780, 1, 4)
        elseif job == 9 then
            GiveItem(role, 0, 769, 1, 4)
        elseif job == 16 or job == 4 then
            GiveItem(role, 0, 806, 1, 4)
        elseif job == 8 then
            GiveItem(role, 0, 766, 1, 4)
        elseif job == 13 or job == 5 then
            GiveItem(role, 0, 792, 1, 4)
        elseif job == 14 then
            GiveItem(role, 0, 798, 1, 4)
        elseif job == 1 then
            if cha_type == 1 then
                GiveItem(role, 0, 769, 1, 4)
            elseif cha_type == 2 then
                GiveItem(role, 0, 766, 1, 4)
            end
        end
    end
end
function ItemUse_JingLingBOX(role, Item)
	if GetChaFreeBagGridNum(role) <= 0 then
		SystemNotice(role, "Insufficient space in inventory!")
		UseItemFailed(role)
		return
	end
	local ItemID = GetItemID(Item)
	local General = 0

	for i = 1 , #BaoXiang_JingLingBOX, 1 do
		if BaoXiang_JingLingBOX[i].Active == 1 then
			General = BaoXiang_JingLingBOX[i].Rad + General
		end
	end
	local a = math.random(1, General)
	local b = 0
	local d = 0 
	local c = -1
	for k = 1 , #BaoXiang_JingLingBOX, 1 do
		if BaoXiang_JingLingBOX[k].Active == 1 then
			d = BaoXiang_JingLingBOX[k].Rad + b
			if a <= d and a > b then
				c = k
				break
			end 
			b = d
		end
	end
	if c ~= -1 then
		local FairyID		= BaoXiang_JingLingBOX[c].ItemID 
		local ItemCount		= BaoXiang_JingLingBOX[c].Quantity
		local ItemQuality	= BaoXiang_JingLingBOX[c].Quality
		local GoodItem		= BaoXiang_JingLingBOX[c].GoodItem
		local r1,r2 = MakeItem(role, FairyID, ItemCount, ItemQuality)
		local Fairy = GetChaItem(role, 2, r2)
		local Param	= GetItemForgeParam(Fairy, 1)
		Param = SetNum_Part1(Param, 0)
		SetItemForgeParam(Fairy, 1, Param)
		if GoodItem == 1 then
			local message = ""..GetChaDefaultName(role).." opens a "..GetItemName(ItemID).." and obtains "..GetItemName(FairyID)..""
			Notice(message)
		end
	end
end
function ItemMedKit(role, Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
    local ItemID = GetItemID(Item)
	local PlayerID = GetCharID(role)
	if ItemCooldown == nil then
		ItemCooldown = {}
	end
	if ItemCooldown[PlayerID] == nil then
		ItemCooldown[PlayerID] = {}
	end
	if ItemCooldown[PlayerID][ItemID] == nil then
		ItemCooldown[PlayerID][ItemID] = {}
		ItemCooldown[PlayerID][ItemID] = os.time()
	end
	if Server.Restore.Life[ItemID].Cooldown ~= 0 then
		local Cooldown = ItemCooldown[PlayerID][ItemID] - os.time()
		if Cooldown > 0 then
			SystemNotice(role, string.format("Item:[%s] in cooldown mode, remaining time is %d sec(s)", GetItemName(GetItemID(Item)), Cooldown))
			UseItemFailed(role)
			return
		end
	end
    if Server.Restore.Life[ItemID] ~= nil then
		if GetChaAttr(role, ATTR_HP) <= 0 or GetChaAttr(role, ATTR_SP) <= 0 then
			UseItemFailed(role)
			return
		end
		if Server.Restore.Life[ItemID].Amount.HP ~= 0 or Server.Restore.Life[ItemID].Percentage.HP ~= 0 then
			local HP = GetChaAttr(role, ATTR_HP) + Server.Restore.Life[ItemID].Amount.HP + (GetChaAttr(role, ATTR_MXHP) * (Server.Restore.Life[ItemID].Percentage.HP))
			if HP > GetChaAttr(role, ATTR_MXHP) then
				HP = GetChaAttr(role, ATTR_MXHP)
			end
			SetCharaAttr(HP, role, ATTR_HP)
		end
		if Server.Restore.Life[ItemID].Amount.SP ~= 0 or Server.Restore.Life[ItemID].Percentage.SP ~= 0  then
			local SP = GetChaAttr(role, ATTR_SP) + Server.Restore.Life[ItemID].Amount.SP + (GetChaAttr(role, ATTR_MXSP) * (Server.Restore.Life[ItemID].Percentage.SP))   
			if SP > GetChaAttr(role, ATTR_MXSP) then
				SP = GetChaAttr(role, ATTR_MXSP)
			end
			SetCharaAttr(SP, role, ATTR_SP)
		end
		if Server.Restore.Life[ItemID].State ~= nil then
			local ChaStateLevel = GetChaStateLv(role, Server.Restore.Life[ItemID].State)
			if ChaStateLevel >= Server.Restore.Life[ItemID].StateLevel then
				SystemNotice(role, "You have used the same or higher state of food. Unable to use "..GetItemName(GetItemID(Item)).."!")
				UseItemFailed(role)
				return
			end
			AddState(role, role, Server.Restore.Life[ItemID].State, Server.Restore.Life[ItemID].StateLevel, Server.Restore.Life[ItemID].StateTime)                 
		end
		ItemCooldown[PlayerID][ItemID] = os.time() + Server.Restore.Life[ItemID].Cooldown
	end
end
function ItemStat(role, Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
    local ItemID = GetItemID(Item)
    if Server.Restore.Attr[ItemID] == nil then
        SystemNotice(role, GetItemName(ItemID) .. " is not usable, please contact administrator.")
        UseItemFailed(role)
        return
    end
    local Stat = GetChaAttr(role, Server.Restore.Attr[ItemID].Attribute)
    if (Stat + Server.Restore.Attr[ItemID].Amount) < 5 or (Stat + Server.Restore.Attr[ItemID].Amount) > 130 then
        SystemNotice(role, "You cannot use [" ..GetItemName(ItemID) .."] to alter your [" ..Server.Restore.Attr[ItemID].Name .. "], since it will leave you below/above the limit of points.")
        UseItemFailed(role)
        return
    end
    SystemNotice(role, "You have successfully used [" ..GetItemName(ItemID) .."] to alter your [" ..Server.Restore.Attr[ItemID].Name .. "] by " .. Server.Restore.Attr[ItemID].Amount .. " point(s).")
    SetCharaAttr((Stat + Server.Restore.Attr[ItemID].Amount), role, Server.Restore.Attr[ItemID].Attribute)
    if Server.Restore.Attr[ItemID].Amount < 0 then
        local Points = GetChaAttr(role, ATTR_AP) - Server.Restore.Attr[ItemID].Amount
        SetCharaAttr(Points, role, ATTR_AP)
    end
end
function ItemUse_XiongHJ(role, Item)
    local statelv = 1
    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        AddState(role, role, STATE_PKSFYS, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_AiCao(role, Item)
    local statelv = 1
    local statetime = 180
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        AddState(role, role, STATE_PKSBYS, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function BlackDragon_Altar(role, Item)
	if Server.Sys.BlackDragonAltar.Active == false then
        SystemNotice(role, "Sorry, Black Dragon Altar is not currently available for usage.")
        UseItemFailed(role)
        return		
	end
    if GetItemAttr(Item, ITEMATTR_URE) ~= 0 or GetItemAttr(Item, ITEMATTR_ENERGY) > Server.AltarBD.MaxCurse then
        SystemNotice(role, "There are too many curses or the seal on [" .. GetItemName(GetItemID(Item)) .. "] hasn't been broken yet.")
        UseItemFailed(role)
        return
    end
    if GetChaFreeBagGridNum(role) <= 0 then
        SystemNotice(role, "You don't have free inventory slots, cannot unseal [" .. GetItemName(GetItemID(Item)) .. "] yet!")
        UseItemFailed(role)
        return
    end
    if GetItemAttr(Item, ITEMATTR_VAL_STA) == 0 then
        SystemNotice(role, "There's no sacrificial item on [" .. GetItemName(GetItemID(Item)) .. "], cannot unseal yet!")
        UseItemFailed(role)
        return
    end
    if GetItemAttr(Item, ITEMATTR_ENERGY) ~= 0 then
        SystemNotice(role, "The remaining " ..GetItemAttr(Item, ITEMATTR_ENERGY) .." Curse Point(s)have damaged the quality of equipment obtained from [" ..GetItemName(GetItemID(Item)) .. "].")
    end
    local Equipment = Server.AltarBD.Trib[GetItemAttr(Item, ITEMATTR_VAL_STA)].Equipment
    local Quality = 20 - GetItemAttr(Item, ITEMATTR_ENERGY)
    GiveItem(role, 0, Equipment, 1, Quality)
    Notice("[" ..GetItemName(GetItemID(Item)) .."] has been unsealed! [" ..GetChaDefaultName(role) .."] has obtained [" ..GetItemName(Equipment) .."] with " .. GetItemAttr(Item, ITEMATTR_ENERGY) .. " Curse Point(s).")
    LG(GetItemName(GetItemID(Item)), GetChaDefaultName(role), GetItemName(Equipment), GetItemAttr(Item, ITEMATTR_ENERGY))
end
function BlackDragon_Gem(role, Item, Target)
    if GetItemID(Target) ~= 266 or GetCtrlBoat(role) ~= nil then
        SystemNotice(role, "The sacrificial item [" ..GetItemName(GetItemID(Item)) .. "] can only be used [" .. GetItemName(266) .. "] while being in land!")
        UseItemFailed(role)
        return
    end
    if GetItemAttr(Target, ITEMATTR_VAL_STA) ~= 0 and Server.AltarBD.Trib[GetItemAttr(Target, ITEMATTR_VAL_STA)].Gem == GetItemID(Item) then
        SystemNotice(role, "[" ..GetItemName(GetItemID(Target)) .."] already has [" .. GetItemName(GetItemID(Item)) .. "] as a sacrificial item.")
        UseItemFailed(role)
        return
    end
    if GetItemType(Item) == 49 and GetItemType(Target) == 65 then
        for X = 1, 3, 1 do
            if Server.AltarBD.Trib[X].Gem == GetItemID(Item) then
                SetItemAttr(Target, ITEMATTR_VAL_STA, X)
                SystemNotice(role, "You have used [" ..GetItemName(GetItemID(Item)) .."] as a sacrificial item on [" ..GetItemName(GetItemID(Target)) .."], the result will be [" .. GetItemName(Server.AltarBD.Trib[X].Equipment) .. "].")
                LG(GetItemName(GetItemID(Item)), GetChaDefaultName(role), GetItemName(GetItemID(Item)))
                break
            end
            if X == 3 then
                UseItemFailed(role)
                return
            end
        end
    end
end
function BlackDragon_Power(role, Item, Target)
    if GetItemID(Target) ~= 266 or GetCtrlBoat(role) ~= nil then
        SystemNotice(role, "The sacrificial item [" ..GetItemName(GetItemID(Item)) .. "] can only be used [" .. GetItemName(266) .. "] while being in land!")
        UseItemFailed(role)
        return
    end
    if GetItemType(Item) == 66 and GetItemType(Target) == 65 then
        if GetItemAttr(Target, Server.AltarBD.Power[GetItemID(Item)]) >= 5 then
            UseItemFailed(role)
            return
        end
        if Percentage_Random(Server.AltarBD.PowerPercentage) == 1 then
            if GetItemAttr(Target, ITEMATTR_URE) >= 50 then
                SetItemAttr(Target, ITEMATTR_URE, (GetItemAttr(Target, ITEMATTR_URE) - 50))
            end
			SystemNotice(role, "Congratulations, the "..GetItemName(GetItemID(Item)).." took effect.")
            SetItemAttr(Target, Server.AltarBD.Power[GetItemID(Item)], (GetItemAttr(Target, Server.AltarBD.Power[GetItemID(Item)]) + 1))
        else
            SystemNotice(role, "Unfortunately, the [" ..GetItemName(GetItemID(Target)) .. "] has been cursed by the soul of the Black Dragon.")
            SetItemAttr(Target, ITEMATTR_MAXENERGY, (GetItemAttr(Target, ITEMATTR_MAXENERGY) + 1))
            SetItemAttr(Target, ITEMATTR_ENERGY, (GetItemAttr(Target, ITEMATTR_ENERGY) + 1))
        end
    end
end
function BlackDragon_Dice(role, Item, Target)
    if GetItemID(Target) ~= 266 or GetCtrlBoat(role) ~= nil then
        SystemNotice(role, "The cleansing item [" ..GetItemName(GetItemID(Item)) .. "] can only be used [" .. GetItemName(266) .. "] while being in land!")
        UseItemFailed(role)
        return
    end
    if GetItemAttr(Target, ITEMATTR_ENERGY) == 0 then
        SystemNotice(role, "There's no need to use the cleansing item [" ..GetItemName(GetItemID(Item)) .. "] on [" .. GetItemName(266) .. "], it has no Curse Point(s).")
        UseItemFailed(role)
        return
    end
    local Random = math.random(1, 100)
    for a = 1, #Server.AltarBD.Dice, 1 do
        if GetItemAttr(Target, ITEMATTR_ENERGY) >= Server.AltarBD.Dice[a].Min and GetItemAttr(Target, ITEMATTR_ENERGY) <= Server.AltarBD.Dice[a].Max then
            if Random <= Server.AltarBD.Dice[a].Percentage then
                SetItemAttr(Target, ITEMATTR_MAXENERGY, (GetItemAttr(Target, ITEMATTR_MAXENERGY) - 1))
                SetItemAttr(Target, ITEMATTR_ENERGY, (GetItemAttr(Target, ITEMATTR_ENERGY) - 1))
                SystemNotice(role, "The [" .. GetItemName(GetItemID(Item)) .. "] you used was of a good quality, the curse was lifted.")
            else
                SystemNotice(role, "The [" ..GetItemName(GetItemID(Item)) .. "] you used was of a bad quality, the curse was not lifted.")
            end
        end
    end
end
function ItemUse_tedengjiang(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 3 then
        SystemNotice(role, "Insufficient slots in inventory")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 845, 1, 16)
    GiveItem(role, 0, 846, 1, 16)
    GiveItem(role, 0, 847, 1, 16)
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " opens the Grand Prize and obtain Black Dragon set"
    Notice(message)
end
function ItemUse_yidengjiang(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Insufficient slots in inventory")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0853, 1, 4)
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " opens the First Prize and obtain a Happy Holiday Magazine"
    Notice(message)
end
function ItemUse_erdengjiang(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 14 then
        SystemNotice(role, "Insufficient slots in inventory")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0456, 10, 4)
    local rad = math.random(1, 8)
    if rad == 1 then
        GiveItem(role, 0, 5013, 1, 4)
        GiveItem(role, 0, 5021, 1, 4)
        GiveItem(role, 0, 5029, 1, 4)
    elseif rad == 2 then
        GiveItem(role, 0, 5014, 1, 4)
        GiveItem(role, 0, 5022, 1, 4)
        GiveItem(role, 0, 5030, 1, 4)
    elseif rad == 3 then
        GiveItem(role, 0, 5015, 1, 4)
        GiveItem(role, 0, 5023, 1, 4)
        GiveItem(role, 0, 5031, 1, 4)
    elseif rad == 4 then
        GiveItem(role, 0, 5020, 1, 4)
        GiveItem(role, 0, 5024, 1, 4)
        GiveItem(role, 0, 5032, 1, 4)
        GiveItem(role, 0, 5037, 1, 4)
    elseif rad == 5 then
        GiveItem(role, 0, 5017, 1, 4)
        GiveItem(role, 0, 5025, 1, 4)
        GiveItem(role, 0, 5033, 1, 4)
    elseif rad == 6 then
        GiveItem(role, 0, 5018, 1, 4)
        GiveItem(role, 0, 5026, 1, 4)
        GiveItem(role, 0, 5034, 1, 4)
    elseif rad == 7 then
        GiveItem(role, 0, 5019, 1, 4)
        GiveItem(role, 0, 5027, 1, 4)
        GiveItem(role, 0, 5035, 1, 4)
    elseif rad == 8 then
        GiveItem(role, 0, 5016, 1, 4)
        GiveItem(role, 0, 5028, 1, 4)
        GiveItem(role, 0, 5036, 1, 4)
        GiveItem(role, 0, 5038, 1, 4)
    end
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " opens the Second Prize and obtain 10 Strengthening Crystal and 1 set of Apparel"
    Notice(message)
end
function ItemUse_sandengjiang(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "Insufficient slots in inventory")
        UseItemFailed(role)
        return
    end
    local rad = math.random(1, 2)
    if rad == 1 then
        GiveItem(role, 0, 3094, 1, 4)
        GiveItem(role, 0, 3096, 1, 4)
    else
        GiveItem(role, 0, 1094, 1, 4)
    end
end
function ItemUse_MonsterSummon(role, Item, Item_Traget)
    local Monster_GetID = {}
    Monster_GetID[1] = math.random(40, 80)
    Monster_GetID[2] = math.random(98, 146)
    Monster_GetID[3] = math.random(194, 241)
    Monster_GetID[4] = math.random(500, 571)
    Monster_GetID[5] = 789

    local ID_Get = math.random(1, 5)
    local x, y = GetChaPos(role)
    local MonsterID = Monster_GetID[ID_Get]
    local Refresh = 50
    local life = 40000
    local new = CreateCha(MonsterID, x, y, 145, Refresh)
    SetChaLifeTime(new, life)
end
function ItemUse_70FYZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 70 Seal Master Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 4204, 1, 22)
end
function Ticket(role, Item)
    if Server.Sys.Ticket.Active ~= true then
        SystemNotice(role, "The [Ticket System] is currently not available, please try again later.")
        UseItemFailed(role)
        return
    end
    local ItemID = GetItemID(Item)
    if TicketSys.Ticket[ItemID] == nil then
        SystemNotice(role, "[" .. GetItemName(ItemID) .. "] is not usable, please contact administrator.")
        UseItemFailed(role)
        return
    end
    if TicketSys.Ticket[ItemID].Level.Min ~= nil and GetChaAttr(role, ATTR_LV) < TicketSys.Ticket[ItemID].Level.Min then
        SystemNotice(role, "[" ..GetItemName(ItemID) .."] is only usable for players above level " .. TicketSys.Ticket[ItemID].Level.Min .. ".")
        UseItemFailed(role)
        return
    end
    if TicketSys.Ticket[ItemID].Level.Max ~= nil and GetChaAttr(role, ATTR_LV) > TicketSys.Ticket[ItemID].Level.Max then
        SystemNotice(role, "[" ..GetItemName(ItemID) .."] is only usable for players below level " .. TicketSys.Ticket[ItemID].Level.Max .. ".")
        UseItemFailed(role)
        return
    end
    if (Hp(role) < (Mxhp(role) * 0.5)) or (Sp(role) < (Mxsp(role) * 0.5)) or ChaIsBoat(role) == 1 then
        SystemNotice(role, "You must not be in a boat and have at least half of your maximum HP and SP in order to teleport.")
        UseItemFailed(role)
        return
    end
    if TicketSys.Prohibited[GetChaMapName(role)] ~= nil and TicketSys.Prohibited[GetChaMapName(role)].Active == true then
        SystemNotice(role, "Cannot use tickets inside [" .. TicketSys.Prohibited[GetChaMapName(role)].Name .. "].")
        UseItemFailed(role)
        return
    end
    if DelBagItem(role, ItemID, 1) == 1 then
        MoveCity(role, TicketSys.Ticket[ItemID].Name)
    else
        SystemNotice(role, "Failed to use [" .. GetItemName(ItemID) .. "].")
        UseItemFailed(role)
        return
    end
    LG("Ticket System", "role[" .. GetChaDefaultName(role) .. "] used [" .. GetItemName(ItemID) .. "].")
end
function ItemUse_Egg(role, Item)
    local EggID = GetItemID(Item)
    if GetChaFreeBagGridNum(role) < 2 then
        SystemNotice(role, "You need at least two free slot.")
        UseItemFailed(role)
        return
    end
    local r1, r2 = MakeItem(role, Server.Fairy.Egg[EggID].ID, 1, 4)
    local Fairy = GetChaItem(role, 2, r2)
end
function NewbieChest(role, Item)
    if GetCtrlBoat(role) ~= nil then
        SystemNotice(role, "Cannot open while using boat.")
        UseItemFailed(role)
        return
    end
    if GetItemID(Item) == 436 then
        if Lv(role) < 5 or GetChaFreeBagGridNum(role) < 4 then
            SystemNotice(role, "You need to be at least Level 5 and have at least 4 free inventory slots.")
            UseItemFailed(role)
            return
        end
        GiveItem(role, 0, 437, 1, 4)
        GiveItem(role, 0, 9, 1, 4)   
        GiveItem(role, 0, 4308, 1, 4)
        GiveItem(role, 0, 444, 1, 4) 
        GiveItem(role, 0, 6050, 1, 4)
        AddMoney(role, 0, 2500)
    end
    if GetItemID(Item) == 437 then
        if Lv(role) < 10 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 9 then
            SystemNotice(role, "You need to be at least Level 10, have done First Class Advancement and have at least 9 free inventory slots.")
            UseItemFailed(role)
            return
        end
        if GetChaAttr(role, ATTR_JOB) == 1 or GetChaAttr(role, ATTR_JOB) == 8 or GetChaAttr(role, ATTR_JOB) == 9 then
            GiveItem(role, 0, 10, 1, 4)
            GiveItem(role, 0, 296, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 2 or GetChaAttr(role, ATTR_JOB) == 12 then
            GiveItem(role, 0, 32, 1, 4)
            GiveItem(role, 0, 311, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 4 or GetChaAttr(role, ATTR_JOB) == 16 then
            GiveItem(role, 0, 80, 1, 4)
            if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 336, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 351, 1, 4)
                GiveItem(role, 0, 2202, 1, 4)
            end
        elseif
            GetChaAttr(role, ATTR_JOB) == 5 or GetChaAttr(role, ATTR_JOB) == 13 or GetChaAttr(role, ATTR_JOB) == 14 then
            GiveItem(role, 0, 104, 1, 4)
            if GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 372, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 359, 1, 4)
                GiveItem(role, 0, 2205, 1, 4)
            end
        end
        GiveItem(role, 0, 438, 1, 4)
        GiveItem(role, 0, 4602, 10, 4)
        GiveItem(role, 0, 4603, 10, 4)
        GiveItem(role, 0, 4604, 10, 4)
		GiveItem(role, 0, 445, 1, 4)
        AddMoney(role, 0, 5000)
    end
    if GetItemID(Item) == 438 then
        if Lv(role) < 15 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 6 then
            SystemNotice(role, "You need to be at least Level 15, have done First Class Advancement and have at least 6 free inventory slots.")
            UseItemFailed(role)
            return
        end
        if GetChaAttr(role, ATTR_JOB) == 1 or GetChaAttr(role, ATTR_JOB) == 8 or GetChaAttr(role, ATTR_JOB) == 9 then
            GiveItem(role, 0, 2, 1, 4)
            GiveItem(role, 0, 291, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 2 or GetChaAttr(role, ATTR_JOB) == 12 then
            GiveItem(role, 0, 26, 1, 4)
            GiveItem(role, 0, 306, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 4 or GetChaAttr(role, ATTR_JOB) == 16 then
            GiveItem(role, 0, 74, 1, 4)
            if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 338, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 386, 1, 4)
            end
        elseif
            GetChaAttr(role, ATTR_JOB) == 5 or GetChaAttr(role, ATTR_JOB) == 13 or
                GetChaAttr(role, ATTR_JOB) == 14
         then
            GiveItem(role, 0, 98, 1, 4)
            if GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 366, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 381, 1, 4)
            end
        end

        GiveItem(role, 0, 439, 1, 4)
		GiveItem(role, 0, 4264, 1, 4)
		GiveItem(role, 0, 3351, 1, 4)
		GiveItem(role, 0, 3352, 1, 4)
		GiveItem(role, 0, 3353, 1, 4)
		GiveItem(role, 0, 446, 1, 4)
        AddMoney(role, 0, 7500)
    end
    if GetItemID(Item) == 439 then
        if Lv(role) < 20 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 6 then
            SystemNotice(role, "You need to be at least Level 20, have done First Class Advancement and have at least 6 free inventory slots.")
            UseItemFailed(role)
            return
        end
        if GetChaAttr(role, ATTR_JOB) == 1 or GetChaAttr(role, ATTR_JOB) == 8 or GetChaAttr(role, ATTR_JOB) == 9 then
            GiveItem(role, 0, 14, 1, 4)
            GiveItem(role, 0, 297, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 2 or GetChaAttr(role, ATTR_JOB) == 12 then
            GiveItem(role, 0, 33, 1, 4)
            GiveItem(role, 0, 313, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 4 or GetChaAttr(role, ATTR_JOB) == 16 then
            GiveItem(role, 0, 81, 1, 4)
            if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 337, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 352, 1, 4)
            end
        elseif GetChaAttr(role, ATTR_JOB) == 5 or GetChaAttr(role, ATTR_JOB) == 13 or GetChaAttr(role, ATTR_JOB) == 14 then
            GiveItem(role, 0, 105, 1, 4)
            if GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 373, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 360, 1, 4)
            end
        end
        GiveItem(role, 0, 447, 1, 4)
        GiveItem(role, 0, 440, 1, 4)
        GiveItem(role, 0, 3844, 1, 4)
        AddMoney(role, 0, 10000)
    end
    if GetItemID(Item) == 440 then
        if Lv(role) < 25 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 5 then
            SystemNotice(role, "You need to be at least Level 25, have done First Class Advancement and have at least 5 free inventory slots.")
            UseItemFailed(role)
            return
        end
        if GetChaAttr(role, ATTR_JOB) == 1 or GetChaAttr(role, ATTR_JOB) == 8 or GetChaAttr(role, ATTR_JOB) == 9 then
            GiveItem(role, 0, 3, 1, 11)
            GiveItem(role, 0, 293, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 2 or GetChaAttr(role, ATTR_JOB) == 12 then
            GiveItem(role, 0, 27, 1, 11)
            GiveItem(role, 0, 307, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 4 or GetChaAttr(role, ATTR_JOB) == 16 then
            GiveItem(role, 0, 75, 1, 11)
            if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 340, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 350, 1, 4)
            end
        elseif
            GetChaAttr(role, ATTR_JOB) == 5 or GetChaAttr(role, ATTR_JOB) == 13 or
                GetChaAttr(role, ATTR_JOB) == 14
         then
            GiveItem(role, 0, 99, 1, 11)
            if GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 368, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 389, 1, 4)
            end
        end
        GiveItem(role, 0, 441, 1, 4)
        GiveItem(role, 0, 448, 1, 4)
        GiveItem(role, 0, 262, 1, 4)
        AddMoney(role, 0, 12500)
    end
    if GetItemID(Item) == 441 then
        if Lv(role) < 30 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 5 then
            SystemNotice(role, "You need to be at least Level 30, have done First Class Advancement and have at least 5 free inventory slots.")
            UseItemFailed(role)
            return
        end
        if GetChaAttr(role, ATTR_JOB) == 1 or GetChaAttr(role, ATTR_JOB) == 8 or GetChaAttr(role, ATTR_JOB) == 9 then
            GiveItem(role, 0, 12, 1, 4)
            GiveItem(role, 0, 298, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 2 or GetChaAttr(role, ATTR_JOB) == 12 then
            GiveItem(role, 0, 34, 1, 4)
            GiveItem(role, 0, 314, 1, 4)
        elseif GetChaAttr(role, ATTR_JOB) == 4 or GetChaAttr(role, ATTR_JOB) == 16 then
            GiveItem(role, 0, 82, 1, 4)
            if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 339, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 354, 1, 4)
            end
        elseif
            GetChaAttr(role, ATTR_JOB) == 5 or GetChaAttr(role, ATTR_JOB) == 13 or
                GetChaAttr(role, ATTR_JOB) == 14
         then
            GiveItem(role, 0, 106, 1, 4)
            if GetChaTypeID(role) == 3 then
                GiveItem(role, 0, 374, 1, 4)
            elseif GetChaTypeID(role) == 4 then
                GiveItem(role, 0, 361, 1, 4)
            end
        end
        GiveItem(role, 0, 442, 1, 4)
        GiveItem(role, 0, 449, 1, 4)
        AddMoney(role, 0, 15000)
    end
    if GetItemID(Item) == 442 then
        if Lv(role) < 35 or GetChaAttr(role, ATTR_JOB) == 0 or GetChaFreeBagGridNum(role) < 3 then
            SystemNotice(role, "You need to be at least Level 35, have done First Class Advancement and have at least 3 free inventory slots.")
            UseItemFailed(role)
            return
        end
        GiveItem(role, 0, 443, 1, 4)
        GiveItem(role, 0, 451, 1, 4)
        AddMoney(role, 0, 17500)
    end
    if GetItemID(Item) == 443 then
        if Lv(role) < 40 or GetChaFreeBagGridNum(role) < 4 or
                (GetChaAttr(role, ATTR_JOB) >= 0 and GetChaAttr(role, ATTR_JOB) <= 5)
         then
            SystemNotice(role, "You need to be at least Level 40 to open this chest, have promoted to Second Class Advancement and have 4 free inventory slots available.")
            UseItemFailed(role)
            return
        end
		if GetChaAttr(role, ATTR_JOB) == 8 then
			GiveItem(role, 0, 300, 1, 95)
			GiveItem(role, 0, 15, 1, 95) 
			GiveItem(role, 0, 301, 1, 95)
			local R1,R2 = MakeItem(role, 20, 1, 11)
			local R3 = GetChaItem(role, 2, R2)
		elseif GetChaAttr(role, ATTR_JOB) == 9 then
			GiveItem(role, 0, 295, 1, 95)
			GiveItem(role, 0, 4, 1, 95) 
			GiveItem(role, 0, 302, 1, 95)
			local R1,R2 = MakeItem(role, 22, 1, 11)
			local R3 = GetChaItem(role, 2, R2)
		elseif GetChaAttr(role, ATTR_JOB) == 12 then 
			GiveItem(role, 0, 39, 1, 95) 
			GiveItem(role, 0, 310, 1, 95)
			GiveItem(role, 0, 315, 1, 95)
			local R1,R2 = MakeItem(role, 44, 1, 11)
			local R3 = GetChaItem(role, 2, R2)
		elseif GetChaAttr(role, ATTR_JOB)  == 13 then
			GiveItem(role, 0, 100, 1, 95)
			if GetChaTypeID(role) == 3 then 
				GiveItem(role, 0, 370, 1, 95)
				GiveItem(role, 0, 378, 1, 95)
			else 
				GiveItem(role, 0, 392, 1, 95) 
				GiveItem(role, 0, 388, 1, 95) 
			end
			local R1,R2 = MakeItem(role, 1440, 1, 11)
			local R3 = GetChaItem(role, 2, R2)
		elseif GetChaAttr(role, ATTR_JOB)  == 14 then
			GiveItem(role, 0, 101, 1, 95) 
			if GetChaTypeID(role) == 3 then 
				GiveItem(role, 0, 367, 1, 95) 
				GiveItem(role, 0, 375, 1, 95) 
			else  
				GiveItem(role, 0, 390, 1, 95) 
				GiveItem(role, 0, 362, 1, 95)
			end
			local R1,R2 = MakeItem(role, 107, 1, 11)
			local R3 = GetChaItem(role, 2, R2)
		elseif GetChaAttr(role, ATTR_JOB)  == 16 then
			GiveItem(role, 0, 76, 1, 95) 
			if GetChaTypeID(role) == 1 or GetChaTypeID(role) == 3 then 
				GiveItem( role, 0, 341, 1, 95)
				GiveItem( role, 0, 342, 1, 95)
			else 
				GiveItem( role, 0, 353, 1, 95)
				GiveItem( role, 0, 356, 1, 95)
			end
			local R1,R2 = MakeItem(role, 83 , 1 , 11)
			local R3 = GetChaItem(role, 2, R2)
        end
		GiveItem(role, 0, 450, 1, 4)
        GiveItem(role, 0, 855, 49, 4)
        AddMoney(role, 0, 200000)
    end
end
function ItemUse_FLeiBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    local BNUM = B1 + B2 + B3 + B4 + B5 + B6 + B7 + B8 + B9 + B10 + B11 + B12 + B13 + B14 + B15
    if Item_CanGet < BNUM then
        SystemNotice(role, "To open a Tempest Chest requires" .. BNUM .. "space ")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, A1, B1, 4)
    GiveItem(role, 0, A2, B2, 4)
    GiveItem(role, 0, A3, B3, 4)
    GiveItem(role, 0, A4, B4, 4)
    GiveItem(role, 0, A5, B5, 4)
    GiveItem(role, 0, A6, B6, 4)
    GiveItem(role, 0, A7, B7, 4)
    GiveItem(role, 0, A8, B8, 4)
    GiveItem(role, 0, A9, B9, 4)
    GiveItem(role, 0, A10, B10, 4)
    GiveItem(role, 0, A11, B11, 4)
    GiveItem(role, 0, A12, B12, 4)
    GiveItem(role, 0, A13, B13, 4)
    GiveItem(role, 0, A14, B14, 4)
    GiveItem(role, 0, A15, B15, 4)
    GiveItem(role, 0, A16, B16, 4)
    GiveItem(role, 0, A17, B17, 4)
    GiveItem(role, 0, A18, B18, 4)
    GiveItem(role, 0, A19, B19, 4)
    GiveItem(role, 0, A20, B20, 4)
    GiveItem(role, 0, A21, B21, 4)
    GiveItem(role, 0, A22, B22, 4)
    GiveItem(role, 0, A23, B23, 4)
    GiveItem(role, 0, A24, B24, 4)
end
function ItemUse_YZheBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 8 then
        SystemNotice(role, "To open a Champion's Chest requires 8 empty slots")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 453, 1, 4)
    GiveItem(role, 0, 454, 1, 4)
    GiveItem(role, 0, 455, 1, 4)
    GiveItem(role, 0, 456, 1, 4)
    local rad = math.random(1, 8)
    if rad == 1 then
        GiveItem(role, 0, 5013, 1, 4)
        GiveItem(role, 0, 5021, 1, 4)
        GiveItem(role, 0, 5029, 1, 4)
    elseif rad == 2 then
        GiveItem(role, 0, 5014, 1, 4)
        GiveItem(role, 0, 5022, 1, 4)
        GiveItem(role, 0, 5030, 1, 4)
    elseif rad == 3 then
        GiveItem(role, 0, 5015, 1, 4)
        GiveItem(role, 0, 5023, 1, 4)
        GiveItem(role, 0, 5031, 1, 4)
    elseif rad == 4 then
        GiveItem(role, 0, 5016, 1, 4)
        GiveItem(role, 0, 5024, 1, 4)
        GiveItem(role, 0, 5032, 1, 4)
        GiveItem(role, 0, 5037, 1, 4)
    elseif rad == 5 then
        GiveItem(role, 0, 5017, 1, 4)
        GiveItem(role, 0, 5025, 1, 4)
        GiveItem(role, 0, 5033, 1, 4)
    elseif rad == 6 then
        GiveItem(role, 0, 5018, 1, 4)
        GiveItem(role, 0, 5026, 1, 4)
        GiveItem(role, 0, 5034, 1, 4)
    elseif rad == 7 then
        GiveItem(role, 0, 5019, 1, 4)
        GiveItem(role, 0, 5027, 1, 4)
        GiveItem(role, 0, 5035, 1, 4)
    elseif rad == 8 then
        GiveItem(role, 0, 5020, 1, 4)
        GiveItem(role, 0, 5028, 1, 4)
        GiveItem(role, 0, 5036, 1, 4)
        GiveItem(role, 0, 5038, 1, 4)
    end
end
function ItemUse_70JJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 70 Champion Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1375, 1, 22)
end
function ItemUse_Sxl(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 5 then
        SystemNotice(role, "Inventory requires at least 5 empty slots to use Unique Necklace Voucher")
        UseItemFailed(role)
        return
    end
    local rad = math.random(1, 2)
    if rad == 1 then
        GiveItem(role, 0, 418, 1, 4)
        GiveItem(role, 0, 420, 1, 4)
        GiveItem(role, 0, 739, 1, 4)
        GiveItem(role, 0, 462, 1, 4)
        GiveItem(role, 0, 495, 1, 4)
    end
    if rad == 2 then
        GiveItem(role, 0, 419, 1, 4)
        GiveItem(role, 0, 421, 1, 4)
        GiveItem(role, 0, 461, 1, 4)
        GiveItem(role, 0, 463, 1, 4)
        GiveItem(role, 0, 497, 1, 4)
    end
end
function ItemUse_Crbbt(role, Item)
    local statelv = 1
    local statetime = 20
    AddState(role, role, STATE_CJBBT, statelv, statetime)
end
function ItemUse_Jrqkl(role, Item)
    local statelv = 1
    local statetime = 20
    AddState(role, role, STATE_JRQKL, statelv, statetime)
end
function Corsairs_WeaponChest(role, Item)
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat(role)
	if Cha_Boat ~= nil then
		SystemNotice(role, "Cannot use while sailing!")
		UseItemFailed(role)
		return
	end
	local Chest = {}
	Chest[322] = {Active = 1, Item = 4204, Quantity = 1, Quality = 22}	-- Lv 70 Seal Master Chest of Magenta
	Chest[525] = {Active = 1, Item = 1375, Quantity = 1, Quality = 22}	-- Lv 70 Champion Chest of Magenta
	Chest[613] = {Active = 1, Item = 1394, Quantity = 1, Quality = 22}	-- Lv 70 Crusader Chest of Magenta
	Chest[614] = {Active = 1, Item = 0042, Quantity = 1, Quality = 22}	-- Lv 70 Sharpshooter Chest of Magenta
	Chest[615] = {Active = 1, Item = 1421, Quantity = 1, Quality = 22}	-- Lv 70 Voyager Chest of Magenta
	Chest[616] = {Active = 1, Item = 4198, Quantity = 1, Quality = 22}	-- Lv 70 Cleric Chest of Magenta
	Chest[939] = {Active = 1, Item = 1392, Quantity = 1, Quality = 22}	-- Lv 50 Crusader Chest of Magenta
	Chest[940] = {Active = 1, Item = 1373, Quantity = 1, Quality = 22}	-- Lv 50 Champion Chest of Magenta
	Chest[941] = {Active = 1, Item = 0040, Quantity = 1, Quality = 22}	-- Lv 50 Sharpshooter Chest of Magent
	Chest[942] = {Active = 1, Item = 1419, Quantity = 1, Quality = 22}	-- Lv 50 Voyager Chest of Magenta
	Chest[943] = {Active = 1, Item = 0103, Quantity = 1, Quality = 22}	-- Lv 50 Cleric Chest of Magenta
	Chest[944] = {Active = 1, Item = 0102, Quantity = 1, Quality = 22}	-- Lv 50 Seal Master Chest of Magenta
	Chest[945] = {Active = 1, Item = 1393, Quantity = 1, Quality = 22}	-- Lv 60 Crusader Chest of Magenta
	Chest[946] = {Active = 1, Item = 1374, Quantity = 1, Quality = 22}	-- Lv 60 Champion Chest of Magenta
	Chest[947] = {Active = 1, Item = 0041, Quantity = 1, Quality = 22}	-- Lv 60 Sharpshooter Chest of Magenta
	Chest[950] = {Active = 1, Item = 1420, Quantity = 1, Quality = 22}	-- Lv 60 Voyager Chest of Magenta
	Chest[951] = {Active = 1, Item = 4303, Quantity = 1, Quality = 22}	-- Lv 60 Cleric Chest of Magenta
	Chest[952] = {Active = 1, Item = 4300, Quantity = 1, Quality = 22}	-- Lv 60 Seal Master Chest of Magenta
	local ItemID = GetItemID(Item)
	if Chest[ItemID] ~= nil then
		local Item_CanGet = GetChaFreeBagGridNum(role)	
		if Item_CanGet <= Chest[ItemID].Quantity then
			SystemNotice(role ,"Insufficient slot in inventory. Failed to open "..GetItemName(ItemID).."!")
			UseItemFailed(role)
			return
		end
		if Chest[ItemID].Active == 0 then
			SystemNotice(role, ""..GetItemName(ItemID).." is currently disabled!")
			UseItemFailed(role)
			return
		end
		GiveItem(role, 0, Chest[ItemID].Item, Chest[ItemID].Quantity, Chest[ItemID].Quality)
		SystemNotice(role, "Obtains "..GetItemName(Chest[ItemID].Item)..".")
	end
end
function Corsairs_WeaponChest2(role, Item)
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat(role)
	if Cha_Boat ~= nil then
		SystemNotice(role, "Cannot use while sailing!")
		UseItemFailed(role)
		return
	end
	local Chest = {}
	Chest[6350] = {Active = 1, Item = {0004,0015,0039,0076,0100,0101}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 40
	Chest[6351] = {Active = 1, Item = {0022,0020,0044,0083,4305,0107}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 45
	Chest[6352] = {Active = 1, Item = {1392,1373,0040,0077,0103,0102}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 50
	Chest[6353] = {Active = 1, Item = {0023,0021,0045,0084,1476,0108}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 55
	Chest[6354] = {Active = 1, Item = {1393,1374,0041,0078,1442,1439}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 60
	Chest[6355] = {Active = 1, Item = {4212,4209,4214,4216,4197,4203}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 65
	Chest[6356] = {Active = 1, Item = {0113,0115,0119,0150,0109,0111}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 70
	Chest[6357] = {Active = 1, Item = {0114,0116,0120,0151,0110,0112}, Quantity = 1, Quality = 22}	-- Pirate Weapon Box Level 75

	local ItemID = GetItemID(Item)
	if Chest[ItemID] ~= nil then
		local Item_CanGet = GetChaFreeBagGridNum(role)	
		if Item_CanGet <= Chest[ItemID].Quantity then
			SystemNotice(role ,"Insufficient slot in inventory. Failed to open "..GetItemName(ItemID).."!")
			UseItemFailed(role)
			return
		end
		if Chest[ItemID].Active == 0 then
			SystemNotice(role, ""..GetItemName(ItemID).." is currently disabled!")
			UseItemFailed(role)
			return
		end
		GiveItem(role, 0, Chest[ItemID].Item[math.random(1, #Chest[ItemID].Item)], Chest[ItemID].Quantity, Chest[ItemID].Quality)
		SystemNotice(role, "Obtains "..GetItemName(Chest[ItemID].Item)..".")
	end
end
function Corsairs_EquipmentChest(role, Item)
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat(role)
	if Cha_Boat ~= nil then
		SystemNotice(role, "Cannot use while sailing!")
		UseItemFailed(role)
		return
	end
	local Chest = {}
	Chest[617] = {Active = 1, Item = {299,475,651}, Quantity = {1,1,1}, Quality = {22,22,22}}				-- Lv 50 Crusader Chest	
	Chest[620] = {Active = 1, Item = {229,653}, Quantity = {1,1}, Quality = {22,22}}				    	-- Lv 50 Champion Chest
	Chest[623] = {Active = 1, Item = {371,547,723}, Quantity = {1,1,1}, Quality = {22,22,22}}				-- Lv 50 Cleric Chest	
	Chest[626] = {Active = 1, Item = {382,558,734,2204}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}		-- Lv 50 Ami Cleric Chest			
	Chest[699] = {Active = 1, Item = {369,545,721}, Quantity = {1,1,1}, Quality = {22,22,22}}				-- Lv 50 Seal Master Chest	
	Chest[629] = {Active = 1, Item = {385,561,737,2207}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}		-- Lv 50 Ami Seal Master Chest			
	Chest[676] = {Active = 1, Item = {345,521,697}, Quantity = {1,1,1}, Quality = {22,22,22}}				-- Lv 50 Voyager Chest	
	Chest[632] = {Active = 1, Item = {355,531,707,2192}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}		-- Lv 50 Ami Voyager Chest			
	Chest[671] = {Active = 1, Item = {312,488,664}, Quantity = {1,1,1}, Quality = {22,22,22}}				-- Lv 50 Sharpshooter Chest
	Chest[618] = {Active = 1, Item = {304,480,656}, Quantity = {1,1,1}, Quality = {22,22,22}}               -- Lv 60 Crusader Chest
	Chest[621] = {Active = 1, Item = {230,477}, Quantity = {1,1}, Quality = {22,22}}                        -- Lv 60 Champion Chest
	Chest[624] = {Active = 1, Item = {394,570,746}, Quantity = {1,1,1}, Quality = {22,22,22}}               -- Lv 60 Cleric Chest
	Chest[627] = {Active = 1, Item = {393,569,745,2215}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}     -- Lv 60 Ami Cleric Chest	
	Chest[700] = {Active = 1, Item = {377,553,729}, Quantity = {1,1,1}, Quality = {22,22,22}}               -- Lv 60 Seal Master Chest	
	Chest[630] = {Active = 1, Item = {364,540,716,2201}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}     -- Lv 60 Ami Seal Master Chest
	Chest[686] = {Active = 1, Item = {344,520,696}, Quantity = {1,1,1}, Quality = {22,22,22}}               -- Lv 60 Voyager Chest
	Chest[633] = {Active = 1, Item = {358,534,710,2195}, Quantity = {1,1,1,1}, Quality = {22,22,22,22}}     -- Lv 60 Ami Voyager Chest
	Chest[673] = {Active = 1, Item = {317,493,669}, Quantity = {1,1,1}, Quality = {22,22,22}}               -- Lv 60 Sharpshooter Chest
	Chest[619] = {Active = 1, Item = {4150,4166,4182}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Crusader Chest
	Chest[622] = {Active = 1, Item = {4148,653,477}, Quantity = {1,1,1}, Quality = {22,22,22}}              -- Lv 70 Champion Chest
	Chest[625] = {Active = 1, Item = {4159,4175,4191}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Cleric Chest
	Chest[628] = {Active = 1, Item = {4160,4176,4192}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Ami Cleric Chest
	Chest[701] = {Active = 1, Item = {4163,4179,4195}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Seal Master Chest
	Chest[631] = {Active = 1, Item = {4164,4180,4196}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Ami Seal Master Chest
	Chest[698] = {Active = 1, Item = {4155,4171,4187}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Voyager Chest
	Chest[634] = {Active = 1, Item = {4156,4172,4188}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Ami Voyager Chest
	Chest[674] = {Active = 1, Item = {4152,4168,4184}, Quantity = {1,1,1}, Quality = {22,22,22}}            -- Lv 70 Sharpshooter Chest
	local ItemID = GetItemID(Item)
	if Chest[ItemID] ~= nil then
		local Item_CanGet = GetChaFreeBagGridNum(role)
		local Num = #Chest[ItemID].Item
		if Item_CanGet < Num then
			SystemNotice(role ,"Insufficient slot in inventory. Failed to open "..GetItemName(ItemID).."!")
			UseItemFailed(role)
			return
		end
		if Chest[ItemID].Active == 0 then
			SystemNotice(role, ""..GetItemName(ItemID).." is currently disabled!")
			UseItemFailed(role)
			return
		end
		for k,v in pairs(Chest[ItemID].Item) do
			if Chest[ItemID].Item[k] ~= nil and Chest[ItemID].Quantity[k] ~= nil and Chest[ItemID].Quality[k] ~= nil then
				GiveItem(role, 0, Chest[ItemID].Item[k], Chest[ItemID].Quantity[k], Chest[ItemID].Quality[k])
			end
		end
	end
end
function ItemUse_Map_JLBYPJ(role, Item)
	local Item_CanGet = GetChaFreeBagGridNum(role)
	 if Item_CanGet < 1 then
		SystemNotice(role, "You need to have at least 1 empty inventory slot")
		UseItemFailed(role)
		return
	end 
	local Has_GoldenMap = CheckBagItem(role, 682)
	if Has_GoldenMap >= 1 then
		SystemNotice(role, "You can only bring 1 Treasure Map at a time")
		UseItemFailed(role)
		return
	end
	GiveItem(role, 0, 682, 1, 0)
end
function ItemUse_JLB_GoldenMap(role, Item)
 	local Item_CanGet = GetChaFreeBagGridNum(role)
	if Item_CanGet < 1 then
		SystemNotice(role ,"You need to have at least 1 empty inventory slot")
		UseItemFailed(role)
		return
	end 
	local lv= GetChaAttr(role, ATTR_LV) 
	if lv < 15 or  lv > 100 then
		SystemNotice(role, "Characters lower than Lv 15 or higher than Lv 40 cannot use this treasure map")
		UseItemFailed(role)
		return	
	end
	local Has_GoldenMap = CheckBagItem(role, 682)
	if Has_GoldenMap >= 2 then
		SystemNotice(role, "You can only have 1 treasure map in inventory to seek for treasure. Please deposit the rest into your vault")
		UseItemFailed(role)
		return
	end
	local Item = GetChaItem2(role, 2, 682)
	local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)
	local Item_URE = GetItemAttr(Item, ITEMATTR_URE)
	local Item_MAXENERGY = GetItemAttr(Item, ITEMATTR_MAXENERGY)
	local pos_x = Item_MAXURE
	local pos_y = Item_MAXENERGY
	local Themap = Item_URE
	local MapList = {}	
	MapList[0] = "NoMap"
	MapList[1] = "jialebi"
	local MapNameList = {}
	MapNameList[0] = "No map"
	MapNameList[1] = "Treasure Gulf"
	if pos_x == 0 or pos_y == 0 or Themap == 0 then
		pos_x, pos_y, Themap = GetTheMapPos_JLB(role, 1)	
		Item_MAXURE = pos_x
		Item_URE = Themap
		Item_MAXENERGY = pos_y
		SetItemAttr(Item, ITEMATTR_MAXENERGY, Item_MAXENERGY)
		SetItemAttr(Item, ITEMATTR_MAXURE, Item_MAXURE)
		SetItemAttr(Item, ITEMATTR_URE, Item_URE)
	end
	local GetPos = CheckGetMapPos(role, pos_x, pos_y, MapList[Themap])
	if GetPos == 0 then
		SystemNotice(role, "Treasure is hidden at "..MapNameList[Themap].." Region. "..pos_x..","..pos_y.." nearby")
		UseItemFailed(role)
		return
	elseif GetPos == 1 then
		local getrandom = math.random(1,3)
		if getrandom == 1 then
			GiveGoldenMapItem_JLB(role)
		else
			SystemNotice(role, "Looks like nothing is dug out. Look again nearby")
			UseItemFailed(role)
			return
		end
	end
end
function GiveGoldenMapItem_JLB(role)
	local CheckRandom = math.random(1,100)
	local x, y = GetChaPos(role)
	local monsterID = math.random(829,836)
	if CheckRandom >= 1 and CheckRandom <= 20 then
		local GiveMoney = 10000 * math.random (1, 20)
		SystemNotice (role, "Dug out Caribbean Treasure and obtain "..GiveMoney.."G")
		AddMoney (role, 0, GiveMoney)
	elseif CheckRandom > 20 and CheckRandom <= 24 then
		XianJing(role, 1)
	elseif CheckRandom > 24 and CheckRandom <= 28 then
		XianJing(role, 2)
	elseif CheckRandom > 28 and CheckRandom <= 33 then
		SystemNotice(role, "Today seems to be spining about. Don't know where it will spin to")
		MapRandomtele(role)
	elseif CheckRandom > 33 and CheckRandom <= 40 then
		SystemNotice(role , "Oh no! Who let the dogs out! Help...")
		local new1 = CreateCha(monsterID, x, y, 145, 30)
		SetChaLifeTime(new1, 90000)
	else
		SystemNotice (role, "dug out a hidden pirate treasure")
		local General = 0		
		for i = 1 , #BaoXiang_JLBCBTBOX, 1 do
			if BaoXiang_JLBCBTBOX[i].Active == 1 then
				General = BaoXiang_JLBCBTBOX[i].Rad + General
			end
		end
		local a = math.random(1, General)
		local b = 0
		local d = 0 
		local c = -1
		for k = 1 , #BaoXiang_JLBCBTBOX, 1 do
			if BaoXiang_JLBCBTBOX[k].Active == 1 then
				d = BaoXiang_JLBCBTBOX[k].Rad + b
				if a <= d and a > b then
					c = k
					break
				end 
				b = d
			end
		end
		if c ~= -1 then
			local Item2Give = BaoXiang_JLBCBTBOX[c].ItemID 
			local ItemCount = BaoXiang_JLBCBTBOX[c].Quantity
			local ItemQuality = BaoXiang_JLBCBTBOX[c].Quality
			local GoodItem = BaoXiang_JLBCBTBOX[c].GoodItem
			GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
			if GoodItem == 1 then
				local message = ""..GetChaDefaultName(role).." Dug out a treasure and obtained "..GetItemName(Item2Give)..""
				Notice(message)
			end
		end
	end
end
function ItemUse_MspdYSB(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        Rem_State_StarUnnormal(role)
        AddState(role, role, STATE_QINGZ, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_XEZJB(role, Item)
    Rem_State_StarUnnormal(role)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        Rem_State_StarUnnormal(role)
        AddState(role, role, STATE_KUANGZ, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_DenglongB(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        Rem_State_StarUnnormal(role)
        AddState(role, role, STATE_QUANS, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_HappyBook(role, Item)
    local Cha_Boat = 0
    local charLv = Lv(role)
    local dif_exp_one = DEXP[charLv + 1]
    local dif_exp_three = DEXP[charLv + 3]
    local dif_exp_five = DEXP[charLv + 5]
    local Exp_star = GetChaAttr(role, ATTR_CEXP)
    local dif_exp_half = (DEXP[charLv + 1] - DEXP[charLv]) * 0.5 + Exp_star + 10
    local dif_exp_thalf = (DEXP[charLv + 1] - DEXP[charLv]) * 0.3 + Exp_star + 10
    local dif_exp_thalf_b = dif_exp_thalf - DEXP[charLv + 1]
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    elseif charLv >= 1 and charLv <= 9 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_five)
    elseif charLv >= 10 and charLv <= 29 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_three)
    elseif charLv >= 30 and charLv <= 59 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_one)
    elseif charLv >= 60 and charLv <= 75 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_half)
    elseif charLv >= 76 and charLv <= 85 and charLv ~= 79 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv == 79 and dif_exp_thalf_b <= 0 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv == 79 and dif_exp_thalf_b > 0 then
        dif_exp_thalf = dif_exp_thalf_b * 0.02 + DEXP[charLv + 1]
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv >= 86 then
        SystemNotice(role, "Your level is too high to use")
        UseItemFailed(role)
        return
    end
end
function ItemUse_CZHe(role, Item)
    local Money_add = 1000000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    if Money_Have >= 999900000 then
        SystemNotice(role, "Your account is saturated. Unable to use item")
        UseItemFailed(role)
        return
    end
    AddMoney(role, 0, Money_add)
end
function ItemUse_lieyanBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Fiery Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0878, 1, 4)
end
function ItemUse_zhiyanBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Furious Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0879, 1, 4)
end
function ItemUse_huoyaoBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Explosive Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0880, 1, 4)
end
function ItemUse_manaoBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lustrious Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0881, 1, 4)
end
function ItemUse_Ant_Hzcr(role, Item, Item_Traget)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        RemoveState(Cha_Boat, STATE_HZCR)
    else
        SystemNotice(role, "Unable to use on the shore")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SPLhyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSLQQH)
    State[1] = GetChaStateLv(role, STATE_YSTZQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSMJQH, statelv, statetime)
end
function ItemUse_SPLhyj( role , Item )
	local OtherStateLv = 0
	local i = 0
	local State_Num = 3
	local State = {}
	State [0] = GetChaStateLv ( role , STATE_YSLQQH )
	State [1] = GetChaStateLv ( role , STATE_YSTZQH )
	State [2] = GetChaStateLv ( role , STATE_YSJSQH )
	State [3] = GetChaStateLv ( role , STATE_YSLLQH )

	for i = 0 , State_Num , 1 do
		if State[i] >= 1 then
			OtherStateLv = OtherStateLv + 1
		end
	end

	if OtherStateLv > 0 then
		SystemNotice(role ,"Potions effect for attribute bonus cannot be stacked")
		UseItemFailed ( role )
		return
	end
	
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~= nil then
		SystemNotice( role , "Cannot use while sailing" )
		UseItemFailed ( role )
		return
	end

	local statelv = 1
	local statetime = 3600
	AddState( role , role , STATE_YSMJQH , statelv , statetime )

end
function ItemUse_SPYyyj(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSTZQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 3600
    AddState(role, role, STATE_YSLQQH, statelv, statetime)
end
function ItemUse_SPMnyj(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 3600
    AddState(role, role, STATE_YSTZQH, statelv, statetime)
end
function ItemUse_SPSlyj(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSTZQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 3600
    AddState(role, role, STATE_YSJSQH, statelv, statetime)
end
function ItemUse_SPXsyj(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSTZQH)
    State[3] = GetChaStateLv(role, STATE_YSJSQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 3600
    AddState(role, role, STATE_YSLLQH, statelv, statetime)
end
function ItemUse_shengguangBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Spirit Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0887, 1, 4)
end
function ItemUse_hanyuBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Glowing Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0882, 1, 4)
end
function ItemUse_yuezhixinBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Shining Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0883, 1, 4)
end
function ItemUse_xianlingBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Shadow Gem Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0884, 1, 4)
end
function ItemUse_fenglingBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Gem of the Wind Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0860, 1, 4)
end
function ItemUse_yingyanBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Gem of Striking Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0861, 1, 4)
end
function ItemUse_yanyuBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Gem of Colossus Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0862, 1, 4)
end
function ItemUse_tanyuBook(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Gem of Rage Voucher")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0863, 1, 4)
end
function ItemUse_50SJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Crusader Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1392, 1, 22)
end
function ItemUse_50JJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Champion Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1373, 1, 22)
end
function ItemUse_50JUJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Sharpshooter Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0040, 1, 22)
end
function ItemUse_50HHZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Voyager Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1419, 1, 22)
end
function ItemUse_50SZZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Cleric Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0103, 1, 22)
end
function ItemUse_50FYZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 50 Seal Master Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0102, 1, 22)
end
function ItemUse_60SJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Crusader Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1393, 1, 22)
end
function ItemUse_60JJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Champion Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1374, 1, 22)
end
function ItemUse_60JUJZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Sharpshooter Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 0041, 1, 22)
end
function ItemUse_60HHZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Voyager Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1420, 1, 22)
end
function ItemUse_60SZZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Cleric Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 4303, 1, 22)
end
function ItemUse_60FYZWBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to use Lv 60 Seal Master Chest of Magenta")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 4300, 1, 22)
end
function ItemUse_SANWUBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local job = GetChaAttr(role, ATTR_JOB)
    local cha_type = GetChaTypeID(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Newbie Chest requires 4 empty slots")
        UseItemFailed(role)
        return
    end
    if lv < 10 then
        SystemNotice(role, "Currently lower than Lv 10. Unable to use item!")
        UseItemFailed(role)
        return
    end
    if cha_type ~= 2 and job == 4 then
        GiveItem(role, 0, 0803, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 2 and job == 16 then
        GiveItem(role, 0, 0803, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type == 2 and job ~= 0 then
        GiveItem(role, 0, 0763, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 2 and cha_type ~= 4 and job == 2 then
        GiveItem(role, 0, 0777, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 2 and cha_type ~= 4 and job == 12 then
        GiveItem(role, 0, 0777, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 1 and cha_type ~= 2 and job == 5 then
        GiveItem(role, 0, 0789, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 1 and cha_type ~= 2 and job == 13 then
        GiveItem(role, 0, 0789, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type ~= 1 and cha_type ~= 2 and job == 14 then
        GiveItem(role, 0, 0789, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    elseif cha_type == 1 and job == 1 then
        GiveItem(role, 0, 1928, 1, 4)
        GiveItem(role, 0, 0986, 1, 4)
    else
        SystemNotice(role, "Cannot be used right now, please find the class instructors to change class.")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SIWUBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local job = GetChaAttr(role, ATTR_JOB)
    local cha_type = GetChaTypeID(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Newbie Chest requires 4 empty slots")
        UseItemFailed(role)
        return
    end
    if lv < 35 then
        SystemNotice(role, "Currently lower than Lv 35. Unable to use item!")
        UseItemFailed(role)
        return
    end
    if cha_type == 1 and job == 9 then
        GiveItem(role, 0, 0767, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 2 and job == 8 then
        GiveItem(role, 0, 0764, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 1 and job == 12 then
        GiveItem(role, 0, 778, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 3 and job == 12 then
        GiveItem(role, 0, 778, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 4 and job == 13 then
        GiveItem(role, 0, 0790, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 3 and job == 13 then
        GiveItem(role, 0, 0790, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 3 and job == 14 then
        GiveItem(role, 0, 0796, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 4 and job == 14 then
        GiveItem(role, 0, 0796, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 3 and job == 16 then
        GiveItem(role, 0, 0804, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 1 and job == 16 then
        GiveItem(role, 0, 0804, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    elseif cha_type == 4 and job == 16 then
        GiveItem(role, 0, 0804, 1, 4)
        GiveItem(role, 0, 0987, 1, 4)
    else
        SystemNotice(role, "Cannot be used right now, please find the class instructors to change class.")
        UseItemFailed(role)
        return
    end
end
function ItemUse_WUWUBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local job = GetChaAttr(role, ATTR_JOB)
    local cha_type = GetChaTypeID(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Newbie Chest requires 4 empty slots")
        UseItemFailed(role)
        return
    end
    if lv < 45 then
        SystemNotice(role, "Current level is below 45, failed to use item!")
        UseItemFailed(role)
        return
    end
    if cha_type == 1 and job == 9 then
        GiveItem(role, 0, 0768, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 2 and job == 8 then
        GiveItem(role, 0, 0765, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 3 and job == 12 then
        GiveItem(role, 0, 0779, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 1 and job == 12 then
        GiveItem(role, 0, 0779, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 3 and job == 13 then
        GiveItem(role, 0, 0791, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 4 and job == 13 then
        GiveItem(role, 0, 0791, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 3 and job == 14 then
        GiveItem(role, 0, 0797, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 4 and job == 14 then
        GiveItem(role, 0, 0797, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 1 and job == 16 then
        GiveItem(role, 0, 0805, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 3 and job == 16 then
        GiveItem(role, 0, 0805, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    elseif cha_type == 4 and job == 16 then
        GiveItem(role, 0, 0805, 1, 4)
        GiveItem(role, 0, 0988, 1, 4)
    else
        SystemNotice(role, "Cannot be used right now, please find the class instructors to change class.")
        UseItemFailed(role)
        return
    end
end
function ItemUse_LIUWUBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local job = GetChaAttr(role, ATTR_JOB)
    local cha_type = GetChaTypeID(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Newbie Chest requires 4 empty slots")
        UseItemFailed(role)
        return
    end
    if lv < 55 then
        SystemNotice(role, "Current level below 55, unable to use item!")
        UseItemFailed(role)
        return
    end
    if cha_type == 1 and job == 9 then
        GiveItem(role, 0, 0769, 1, 4)
    elseif cha_type == 2 and job == 8 then
        GiveItem(role, 0, 0766, 1, 4)
    elseif cha_type == 3 and job == 12 then
        GiveItem(role, 0, 0780, 1, 4)
    elseif cha_type == 1 and job == 12 then
        GiveItem(role, 0, 0780, 1, 4)
    elseif cha_type == 3 and job == 13 then
        GiveItem(role, 0, 0792, 1, 4)
    elseif cha_type == 4 and job == 13 then
        GiveItem(role, 0, 0792, 1, 4)
    elseif cha_type == 3 and job == 14 then
        GiveItem(role, 0, 0798, 1, 4)
    elseif cha_type == 4 and job == 14 then
        GiveItem(role, 0, 0798, 1, 4)
    elseif cha_type == 1 and job == 16 then
        GiveItem(role, 0, 0806, 1, 4)
    elseif cha_type == 3 and job == 16 then
        GiveItem(role, 0, 0806, 1, 4)
    elseif cha_type == 4 and job == 16 then
        GiveItem(role, 0, 0806, 1, 4)
    else
        SystemNotice(role, "Cannot be used right now, please find the class instructors to change class.")
        UseItemFailed(role)
        return
    end
end
function ItemUse_CZKC(role, Item)
    local Cha_Boat = 0
    local charLv = Lv(role)
    local Exp_star = GetChaAttr(role, ATTR_CEXP)
    local dif_exp_thalf_c = (DEXP[charLv + 1] - DEXP[charLv]) * 0.01 + Exp_star
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    if charLv < 86 then
        SystemNotice(role, "86")
        UseItemFailed(role)
        return
    end
    if charLv >= 86 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf_c)
    end
end
function Blueprint(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "To open blueprint requires at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    local Itemnew_ID = 2300
    local rad_ID = math.random(1, 100)
    if rad_ID <= 40 then
        Itemnew_ID = 2300
    elseif rad_ID >= 41 and rad_ID <= 61 then
        Itemnew_ID = 2301
    else
        Itemnew_ID = 2302
    end
    local rad_energy = math.random(1, 100)
    local energy = 1
    local item_ID = GetItemID(Item)
    if item_ID == 1000 then
        energy = 1
    elseif item_ID == 1001 then
        if rad_energy <= 5 then
            energy = 4
        elseif rad_energy <= 30 and rad_energy >= 6 then
            energy = 3
        else
            energy = 2
        end
    elseif item_ID == 1002 then
        if rad_energy <= 5 then
            energy = 6
        elseif rad_energy <= 30 and rad_energy >= 6 then
            energy = 5
        else
            energy = 4
        end
    elseif item_ID == 1003 then
        if rad_energy <= 5 and Itemnew_ID ~= 2301 then
            energy = 8
        elseif rad_energy <= 30 and rad_energy >= 6 then
            energy = 7
        else
            energy = 6
        end
    end
    local Product_ID = 0
    local Product_LV = 0
    local Material1 = 0
    local Material2 = 0
    local Material3 = 0
    local General = 0
    local Blueprint = 0

    if Itemnew_ID == 2300 then
        local v = {}
        v[1] = {1, 28}
        v[2] = {29, 56}
        v[3] = {57, 84}
        v[4] = {85, 112}
        v[5] = {113, 140}
        v[6] = {141, 168}
        v[7] = {169, 196}
        v[8] = {197, 224}
        v[9] = {225, 252}
        v[10] = {253, 280}
        for i = v[energy][1], v[energy][2], 1 do
            if Manufacturing[i].Active == 1 then
                General = Manufacturing[i].Rad + General
            end
        end
        local a = math.random(1, General)
        local b = 0
        local d = 0
        local c = -1
        for k = v[energy][1], v[energy][2], 1 do
            if Manufacturing[k].Active == 1 then
                d = Manufacturing[k].Rad + b
                if a <= d and a > b then
                    c = k
                    break
                end
                b = d
            end
        end
        if c ~= -1 then
            Product_ID = Manufacturing[c].ItemID
            Product_LV = Manufacturing[c].ItemLv
            Material1 = Manufacturing[c].Material1
            Material2 = Manufacturing[c].Material2
            Material3 = Manufacturing[c].Material3
            Blueprint = 1
        end
    end

    if Itemnew_ID == 2301 then
        local v = {}
        v[1] = {1, 77}
        v[2] = {78, 166}
        v[3] = {167, 278}
        v[4] = {279, 434}
        v[5] = {435, 596}
        v[6] = {597, 732}
        v[7] = {733, 871}
        v[8] = {895, 902}
        for i = v[energy][1], v[energy][2], 1 do
            if Crafting[i].Active == 1 then
                General = Crafting[i].Rad + General
            end
        end
        local a = math.random(1, General)
        local b = 0
        local d = 0
        local c = -1
        for k = v[energy][1], v[energy][2], 1 do
            if Crafting[k].Active == 1 then
                d = Crafting[k].Rad + b
                if a <= d and a > b then
                    c = k
                    break
                end
                b = d
            end
        end
        if c ~= -1 then
            Product_ID = Crafting[c].ItemID
            Product_LV = Crafting[c].ItemLv
            Material1 = Crafting[c].Material1
            Material2 = Crafting[c].Material2
            Material3 = Crafting[c].Material3
            Blueprint = 1
        end
    end

    if Itemnew_ID == 2302 then
        local v = {}
        v[1] = {1, 7}
        v[2] = {8, 12}
        v[3] = {13, 17}
        v[4] = {18, 26}
        v[5] = {27, 31}
        v[6] = {32, 36}
        v[7] = {37, 38}
        v[8] = {39, 40}
        for i = v[energy][1], v[energy][2], 1 do
            if Cooking[i].Active == 1 then
                General = Cooking[i].Rad + General
            end
        end
        local a = math.random(1, General)
        local b = 0
        local d = 0
        local c = -1
        for k = v[energy][1], v[energy][2], 1 do
            if Cooking[k].Active == 1 then
                d = Cooking[k].Rad + b
                if a <= d and a > b then
                    c = k
                    break
                end
                b = d
            end
        end
        if c ~= -1 then
            Product_ID = Cooking[c].ItemID
            Product_LV = Cooking[c].ItemLv
            Material1 = Cooking[c].Material1
            Material2 = Cooking[c].Material2
            Material3 = Cooking[c].Material3
            Blueprint = 1
        end
    end
    if Blueprint == 1 then
        local r1, r2 = 0, 0
        r1, r2 = MakeItem(role, Itemnew_ID, 1, 4)
        local Itemnew = GetChaItem(role, 2, r2)

        local star_number = energy * 10
        local yingbi_num = math.random(10, star_number)
        if energy <= 3 then
            SetItemAttr(Itemnew, ITEMATTR_MAXURE, energy)
            SetItemAttr(Itemnew, ITEMATTR_URE, energy)
        else
            SetItemAttr(Itemnew, ITEMATTR_MAXURE, yingbi_num)
            SetItemAttr(Itemnew, ITEMATTR_URE, energy)
        end

        local sta = math.random(1, 10)
        if Itemnew_ID == 2301 then
            sta = sta * 0.5
            sta = math.max(1, sta)
        end
        SetItemAttr(Itemnew, ITEMATTR_VAL_STA, sta)

        local quality = math.floor(Product_LV * 0.1) + 100
        SetItemAttr(Itemnew, ITEMATTR_MAXENERGY, quality)

        local ure = 10 - Product_LV * 0.1
        SetItemAttr(Itemnew, ITEMATTR_ENERGY, ure)

        SetItemAttr(Itemnew, ITEMATTR_VAL_STR, Material1)
        SetItemAttr(Itemnew, ITEMATTR_VAL_CON, Material2)
        SetItemAttr(Itemnew, ITEMATTR_VAL_DEX, Material3)
        SetItemAttr(Itemnew, ITEMATTR_VAL_AGI, Product_ID)

        local Num_new = GetItemForgeParam(Itemnew, 1)
        local Part2_new = GetNum_Part2(Num_new)
        local Part4_new = GetNum_Part4(Num_new)
        local Part6_new = GetNum_Part6(Num_new)
        Part2_new = math.random(3, 20)
        Part4_new = math.random(2, 9)
        Part6_new = math.random(1, 3)
        Part6_new = Part6_new * math.max(1, (energy - 2))
        Num_new = SetNum_Part2(Num_new, Part2_new)
        Num_new = SetNum_Part4(Num_new, Part4_new)
        Num_new = SetNum_Part6(Num_new, Part6_new)
        SetItemForgeParam(Itemnew, 1, Num_new)
    end
end
function ItemUse_CZZX(role, Item)
    local statelv = 1
    local statetime = 30
    local Item_ID = GetItemID(Item)
    AddState(role, role, STATE_CZZX, statelv, statetime)
    if Item_ID ~= 1013 then
        GiveItem(role, 0, 1010, 1, 4)
    end
end
function ItemUse_HHLP(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    local star = 0

    if CheckDateNum >= 10122 and CheckDateNum <= 10123 then
        ItemUse_XNBOX(role, Item)
    end
end
function ItemUse_NSSN(role, Item)
    local cha_name = GetChaDefaultName(role)
    LG("KaiXiaoBaoDeSB", cha_name)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local star = 0
    star = IsChaInRegion(role, 2)
    if star == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 857
    local Refresh = 3700
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_JNJLD(role, Item)
    local star = math.random(1, 10)
    local r1 = 0
    local r2 = 0
    if star == 1 then
        r1, r2 = MakeItem(role, 183, 1, 4)
    elseif star == 2 then
        r1, r2 = MakeItem(role, 184, 1, 4)
    elseif star == 3 then
        r1, r2 = MakeItem(role, 185, 1, 4)
    elseif star == 4 then
        r1, r2 = MakeItem(role, 186, 1, 4)
    elseif star == 5 then
        r1, r2 = MakeItem(role, 187, 1, 4)
    elseif star == 6 then
        r1, r2 = MakeItem(role, 188, 1, 4)
    elseif star == 7 then
        r1, r2 = MakeItem(role, 189, 1, 4)
    elseif star == 8 then
        r1, r2 = MakeItem(role, 190, 1, 4)
    elseif star == 9 then
        r1, r2 = MakeItem(role, 191, 1, 4)
    else
        r1, r2 = MakeItem(role, 199, 1, 4)
    end
    local Item_newJL = GetChaItem(role, 2, r2)
    local Num_newJL = GetItemForgeParam(Item_newJL, 1)
    local Part1_newJL = GetNum_Part1(Num_newJL)
    local Part2_newJL = GetNum_Part2(Num_newJL)
    local Part3_newJL = GetNum_Part3(Num_newJL)
    local Part4_newJL = GetNum_Part4(Num_newJL)
    local Part5_newJL = GetNum_Part5(Num_newJL)
    local Part6_newJL = GetNum_Part6(Num_newJL)
    local Part7_newJL = GetNum_Part7(Num_newJL)
    local t = {}
    t[0] = 1
    t[1] = 2
    t[2] = 3
    t[3] = 4
    t[4] = 5
    local eleven = math.random(1, 3)
    Part2_newJL = t[eleven - 1]
    Part4_newJL = t[eleven]
    Part6_newJL = t[eleven + 1]
    Num_newJL = SetNum_Part2(Num_newJL, Part2_newJL)
    Num_newJL = SetNum_Part3(Num_newJL, 1)
    Num_newJL = SetNum_Part4(Num_newJL, Part4_newJL)
    Num_newJL = SetNum_Part5(Num_newJL, 1)
    Num_newJL = SetNum_Part6(Num_newJL, Part6_newJL)
    Num_newJL = SetNum_Part7(Num_newJL, 1)
    SetItemForgeParam(Item_newJL, 1, Num_newJL)
end
function ItemUse_NSDXB(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient slot in inventory. Failed to activate Goddess's Pouch")
        UseItemFailed(role)
        return
    end
    local star = math.random(1, 12)
    local r1 = 0
    local r2 = 0
    if star == 1 then
        r1, r2 = MakeItem(role, 619, 1, 4)
    elseif star == 2 then
        r1, r2 = MakeItem(role, 625, 1, 4)
    elseif star == 3 then
        r1, r2 = MakeItem(role, 628, 1, 4)
    elseif star == 4 then
        r1, r2 = MakeItem(role, 631, 1, 4)
    elseif star == 5 then
        r1, r2 = MakeItem(role, 634, 1, 4)
    elseif star == 6 then
        r1, r2 = MakeItem(role, 674, 1, 4)
    elseif star == 7 then
        r1, r2 = MakeItem(role, 698, 1, 4)
    elseif star == 8 then
        r1, r2 = MakeItem(role, 701, 1, 4)
    else
        local eleven = math.random(2530, 2548)
        r1, r2 = MakeItem(role, eleven, 1, 4)
    end
    local Item_new = GetChaItem(role, 2, r2)
    local Item_ID = GetItemID(Item_new)
    local itemname = GetItemName(Item_ID)
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " opens Goddess's Pouch and obtain " .. itemname
    Notice(message)
end
function ItemUse_JaNaBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1012, 1, 4)
    local itemname = GetItemName(1012)
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " opens Chest of Ascaron and obtain " .. itemname
    Notice(message)
end
function ItemUse_minibh(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Inventory requires at least 1 empty slot")
        UseItemFailed(role)
        return
    end
    local cha_name = GetChaDefaultName(role)
    local star = math.random(1, 10000)
    if star == 1 then
        GiveItem(role, 0, 272, 1, 4)
        local message = cha_name .. " opens a Mini Black Dragon Bag and obtained the Grand Prize: Black Dragon Set"
        Notice(message)
    elseif star > 1 and star <= 51 then
        GiveItem(role, 0, 273, 1, 4)
        local message1 =
            cha_name .. " opens a Mini Black Dragon Bag and obtained the First Prize: Happy Holiday Magazine"
        Notice(message1)
    elseif star > 51 and star < 1652 then
        GiveItem(role, 0, 274, 1, 4)
    else
        GiveItem(role, 0, 275, 1, 4)
    end
end
function ItemUse_kala(role, Item)
    local statelv = 1
    local statetime = 20
    AddState(role, role, STATE_KALA, statelv, statetime)
end
function ItemUse_MHDYSD(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local star = 0
    star = IsChaInRegion(role, 2)
    if star == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end

    local x, y = GetChaPos(role)
    local MonsterID = 858
    local Refresh = 190
    local life = 180000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)

    SetChaLifeTime(new, life)
end
function ItemUse_TunDuiZhiXing(role, Item)
   	local r1 = 0
	local r2 = 0
	r1, r2 = MakeItem ( role , 1034 , 1 , 4 )
	local Itemnew = GetChaItem ( role , 2 , r2 )
	SetItemAttr(Itemnew, ITEMATTR_URE, 100)
	SetItemAttr(Itemnew, ITEMATTR_MAXURE, 20100)
end
function ItemUse_MoHuanBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Inventory requires at least 1 empty slot")
        UseItemFailed(role)
        return
    end
    local star = math.random(1, 4663)
    if star >= 1 and star <= 3 then
        GiveItem(role, 0, 3866, 1, 4)
    elseif star >= 4 and star <= 14 then
        GiveItem(role, 0, 3864, 1, 4)
    elseif star >= 15 and star <= 65 then
        GiveItem(role, 0, 3858, 1, 4)
    else
        local star = math.random(3850, 3875)
        if star == 3858 or star == 3864 or star == 3866 then
            GiveItem(role, 0, 3850, 1, 4)
        else
            GiveItem(role, 0, star, 1, 4)
        end
    end
end
function ItemUse_mohuan(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Inventory requires at least 1 empty slot")
        UseItemFailed(role)
        return
    end
    local cha_name = GetChaDefaultName(role)
    local star = math.random(1, 1000)
    SystemNotice(role, "star==" .. star)
    if star <= 8 then
        GiveItem(role, 0, 1014, 1, 4)
        local message = cha_name .. " opens a Enchanting Goddess Card and obtained Goddess's Trainee Ceremony"
        Notice(message)
    elseif star >= 9 and star <= 17 then
        GiveItem(role, 0, 271, 1, 4)
        local message1 = cha_name .. " opens an Enchanting Goddess Card and obtained Angelic Dice"
        Notice(message1)
    elseif star >= 18 and star <= 27 then
        GiveItem(role, 0, 1012, 1, 4)
        local message3 = cha_name .. " opens Enchanting Goddess Card and obtain a Gem of Soul"
        Notice(message3)
    elseif star >= 28 and star <= 227 then
        GiveItem(role, 0, 3886, 1, 4)
    else
        GiveItem(role, 0, 19525, 5, 4)
    end
end
function Sk_Script_DBs(role, Item)
    local sk_add = SK_JLTX1
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_NKs(role, Item)
    local sk_add = SK_JLTX2
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_XZs(role, Item)
    local sk_add = SK_JLTX3
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function ItemUse_Map_YPJ(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need to have at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    local Has_GoldenMap = CheckBagItem(role, 1093)
    if Has_GoldenMap >= 1 then
        SystemNotice(role, "You can only bring 1 Treasure Map at a time")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 1093, 1, 0)
end
function ItemUse_GoldenMap(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need to have at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    local Has_GoldenMap = CheckBagItem(role, 1093)
    if Has_GoldenMap >= 2 then
        SystemNotice(role, "Brought more than 1 Treasure Map. Digging failed")
        UseItemFailed(role)
        return
    end
    local Item = GetChaItem2(role, 2, 1093)
    local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)
    local Item_URE = GetItemAttr(Item, ITEMATTR_URE)
    local Item_MAXENERGY = GetItemAttr(Item, ITEMATTR_MAXENERGY)
    local pos_x = Item_MAXURE
    local pos_y = Item_MAXENERGY
    local Themap = Item_URE
    local MapList = {}

    MapList[0] = "NoMap"
    MapList[1] = "garner"
    MapList[2] = "magicsea"
    MapList[3] = "darkblue"

    local MapNameList = {}

    MapNameList[0] = "No map"
    MapNameList[1] = "Ascaron"
    MapNameList[2] = "Magical Ocean"
    MapNameList[3] = "Deep Blue"
    if pos_x == 0 or pos_y == 0 or Themap == 0 then
        pos_x, pos_y, Themap = GetTheMapPos(role, 1)
        Item_MAXURE = pos_x
        Item_URE = Themap
        Item_MAXENERGY = pos_y

        SetItemAttr(Item, ITEMATTR_MAXENERGY, Item_MAXENERGY)
        SetItemAttr(Item, ITEMATTR_MAXURE, Item_MAXURE)
        SetItemAttr(Item, ITEMATTR_URE, Item_URE)
    end
    local GetPos = CheckGetMapPos(role, pos_x, pos_y, MapList[Themap])
    if GetPos == 0 then
        SystemNotice(role, "Treasure is hidden at "..MapNameList[Themap].." Region "..pos_x..","..pos_y.." nearby")
        UseItemFailed(role)
        return
    elseif GetPos == 1 then
        local getrandom = math.random(1, 3)
        if getrandom == 1 then
            GiveGoldenMapItem(role)
        else
            SystemNotice(role, "Looks like nothing is dug out. Look again nearby")
            UseItemFailed(role)
            return
        end
    end
end
function GiveGoldenMapItem(role)
	local CheckRandom = math.random(1,100)
	if CheckRandom >= 1 and CheckRandom <= 23 then
		local GiveMoney = 1000 * math.random(1, 20)
		SystemNotice(role, "Dug out pirates treasure. Obtained"..GiveMoney.."G")
		AddMoney(role, 0 , GiveMoney)
	elseif CheckRandom > 23 and CheckRandom <= 28 then
		XianJing(role, 1)
	elseif CheckRandom > 28 and CheckRandom <= 33 then
		XianJing(role, 2)
	elseif CheckRandom > 33 and CheckRandom <= 40 then
		SystemNotice(role, "Today seems to be spining about. Don't know where it will spin to")
		MapRandomtele(role)
	else
		SystemNotice (role, "dug out a hidden pirate treasure")
		local General = 0		
		for i = 1 , #BaoXiang_CBTBOX, 1 do
			if BaoXiang_CBTBOX[i].Active == 1 then
				General = BaoXiang_CBTBOX[i].Rad + General
			end
		end
		local a = math.random(1, General)
		local b = 0
		local d = 0 
		local c = -1
		for k = 1 , #BaoXiang_CBTBOX, 1 do
			if BaoXiang_CBTBOX[k].Active == 1 then
				d = BaoXiang_CBTBOX[k].Rad + b
				if a <= d and a > b then
					c = k
					break
				end 
				b = d
			end
		end
		if c ~= -1 then
			local Item2Give = BaoXiang_CBTBOX[c].ItemID 
			local ItemCount = BaoXiang_CBTBOX[c].Quantity
			local ItemQuality = BaoXiang_CBTBOX[c].Quality
			local GoodItem = BaoXiang_CBTBOX[c].GoodItem
			GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
			if GoodItem == 1 then
				local message = ""..GetChaDefaultName(role).." Dug out a treasure and obtained "..GetItemName(Item2Give)..""
				Notice(message)
			end
		end
	end
end
function Sk_Script_JQs(role, Item)
    local sk_add = SK_JLTX4
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_BCs(role, Item)
    local sk_add = SK_JLTX5
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_BSs(role, Item)
    local sk_add = SK_JLTX6
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_PZs(role, Item)
    local sk_add = SK_JLTX7
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_SZs(role, Item)
    local sk_add = SK_JLTX8
    local form_sklv = GetSkillLv(role, sk_add)
    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function ItemUse_FightingBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 empty inventory slots to open the Chaos Chest")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2610, 1, 4)
    local rad = math.random(1,4)
    if rad == 1 then
        GiveItem(role, 0, 1124, 1, 15)
    end

    if rad == 2 then
        GiveItem(role, 0, 1125, 1, 15)
    end

    if rad == 3 then
        GiveItem(role, 0, 1126, 1, 15)
    end
	
	if rad == 4 then
		GiveItem(role, 0, 1127, 1, 15)
	end
end
function ItemUse_FaSheng1(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Summon a monster in a safe zone? Please be considerate!")
        UseItemFailed(role)
        return
    end
    local radID = math.random(1, 8)
    local MonsterID = 0

    if radID == 1 then
        MonsterID = 841
    elseif radID == 2 then
        MonsterID = 842
    elseif radID == 3 then
        MonsterID = 843
    elseif radID == 4 then
        MonsterID = 843
    elseif radID == 5 then
        MonsterID = 845
    elseif radID == 6 then
        MonsterID = 846
    elseif radID == 7 then
        MonsterID = 229
    elseif radID == 8 then
        MonsterID = 274
    end

    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local Refresh = 3700000
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_ShaBao1(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Training in Safe Zone? Dream on!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local MonsterID = 937
    local Refresh = 1900000
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_SMBX(role, Item)
    local item_type = BaoXiang_SMBX
    local item_type_rad = BaoXiang_SMBX_Rad
    local item_type_count = BaoXiang_SMBX_Count
    local maxitem = BaoXiang_SMBX_Mxcount
    local item_quality = BaoXiang_SMBX_Qua
    local General = 0
    local ItemId = 0

    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end
    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
            break
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end
    GiveItem(role, 0, ItemId, ItemCount, item_quality)
end
function ItemUse_HLBX(role, Item)
    local item_type = BaoXiang_HLBX
    local item_type_rad = BaoXiang_HLBX_Rad
    local item_type_count = BaoXiang_HLBX_Count
    local maxitem = BaoXiang_HLBX_Mxcount
    local item_quality = BaoXiang_HLBX_Qua
    local General = 0
    local ItemId = 0
    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
            break
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end
    GiveItem(role, 0, ItemId, ItemCount, item_quality)
end
function ItemUse_WZX(role, Item)
    local item_type = BaoXiang_WZX
    local item_type_rad = BaoXiang_WZX_Rad
    local item_type_count = BaoXiang_WZX_Count
    local maxitem = BaoXiang_WZX_Mxcount
    local item_quality = BaoXiang_WZX_Qua
    local General = 0
    local ItemId = 0
    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
            break
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end
    GiveItem(role, 0, ItemId, ItemCount, item_quality)
end
function ItemUse_PKMNYS(role, Item)
    local statelv = 10
    local statetime = 1800
    AddState(role, role, STATE_PKMNYS, statelv, statetime)
end
function ItemUse_PKZDYS(role, Item)
    local statelv = 10
    local statetime = 150
    AddState(role, role, STATE_PKZDYS, statelv, statetime)
end
function ItemUse_PKKBYS(role, Item)
    local statelv = 10
    local statetime = 20
    AddState(role, role, STATE_PKKBYS, statelv, statetime)
end
function ItemUse_PKJSYS(role, Item)
    local statelv = 10
    local statetime = 180
    AddState(role, role, STATE_PKJSYS, statelv, statetime)
end
function ItemUse_PKSFYS(role, Item)
    local statelv = 10
    local statetime = 300
    AddState(role, role, STATE_PKSFYS, statelv, statetime)
end
function ItemUse_PKJZYS(role, Item)
    local statelv = 10
    local statetime = 900
    AddState(role, role, STATE_PKJZYS, statelv, statetime)
end
function ItemUse_PKWDYS(role, Item)
    local statelv = 10
    local statetime = 15
    AddState(role, role, STATE_PKWD, statelv, statetime)
    local cha_name = GetChaDefaultName(role)
    local message = cha_name .. " becomes invunerable for 15 seconds"
    Notice(message)
end
function ItemUse_PKMCK(role, Item)
    local map_name = GetChaMapName(role)
    if map_name == "secretgarden" then
        local hpdmg = -500
        Hp_Endure_Dmg(role, hpdmg)
        ALLExAttrSet(role)
    else
        SystemNotice(role, "Item can only be used in Garden of Edel")
    end
end
CoinChest                   = {}
CoinChest.Conf              = {}
CoinChest.Conf.ChestID      = 1872  -- Fairy Coin Chest id from iteminfo.txt.
CoinChest.Conf.CoinID       = 855   -- Fairy Coin id from iteminfo.txt.
CoinChest.Conf.MinLv        = 41    -- Minimum level required to open chest.
CoinChest.Conf.BagNeed      = 4     -- Inventory space needed to open chest.
CoinChest.Conf.CoinInit     = 20    -- This is the number of fairy coins required on first opening.
CoinChest.Conf.CoinSum      = 5     -- This is number of fairy coins which will increase every time chest is opened.
function ItemUse_YingbiBox(Player, Item)
    local chaLv = Lv(Player)
    if chaLv < CoinChest.Conf.MinLv then
        SystemNotice(Player, "Character need to be Lv"..CoinChest.Conf.MinLv.."+ to open "..GetItemName(CoinChest.Conf.ChestID)..".")
        UseItemFailed(Player)
        return
    end
    local emptySlot = GetChaFreeBagGridNum(Player)
    if emptySlot < CoinChest.Conf.BagNeed then
        SystemNotice(Player, "Requires at least "..CoinChest.Conf.BagNeed.." empty inventory slots.")
        UseItemFailed(Player)
        return
    end
    local PID = GetPlayerID(GetChaPlayer(Player))   
    local Dir = GetResPath(string.format('../PlayerData/FairyCoinChest/%d.dat', PID))
    local Day = (tonumber(os.date("%Y")) * 10000) + (tonumber(os.date("%m")) * 100) + (tonumber(os.date("%d")))
    local Table = DataFile:Init(Dir, Table):Load()
    if (Table[Day] == nil) then
        Table[Day] = {}
        Table[Day] = {Count = 0}
        DataFile:Init(Dir, Table):Save()
    end
    local OpenedNum = Table[Day].Count
    local CoinNeed = (CoinChest.Conf.CoinInit + OpenedNum)
    if (CheckBagItem(Player, CoinChest.Conf.CoinID) < CoinNeed) then
        BickerNotice(Player, "Requires "..CoinNeed.." Fairy Coin(s) to Open!")
        UseItemFailed(Player)
        return
    end
    local Rem = TakeItem(Player, 0, CoinChest.Conf.CoinID, CoinNeed)
    if (Rem == 1) then
        Table[Day].Count = Table[Day].Count + CoinChest.Conf.CoinSum        
        GiveBragiItem(Player)
        GiveItem(Player, 0, CoinChest.Conf.ChestID, 1, 4)
        DataFile:Init(Dir, Table):Save()       
    else
        SystemNotice(Player, "Internal Error!")
        UseItemFailed(Player)
        return
    end
end
function GiveBragiItem(role)
	local General = 0		
	for i = 1 , #BaoXiang_CHESTCOIN, 1 do
		if BaoXiang_CHESTCOIN[i].Active == 1 then
			General = BaoXiang_CHESTCOIN[i].Rad + General
		end
	end
	local a = math.random(1, General)
	local b = 0
	local d = 0 
	local c = -1
	for k = 1 , #BaoXiang_CHESTCOIN, 1 do
		if BaoXiang_CHESTCOIN[k].Active == 1 then
			d = BaoXiang_CHESTCOIN[k].Rad + b
			if a <= d and a > b then
				c = k
				break
			end 
			b = d
		end
	end
	if c ~= -1 then
		local Item2Give		= BaoXiang_CHESTCOIN[c].ItemID 
		local ItemCount		= BaoXiang_CHESTCOIN[c].Quantity
		local ItemQuality	= BaoXiang_CHESTCOIN[c].Quality
		local GoodItem		= BaoXiang_CHESTCOIN[c].GoodItem
		GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
	end
end
function ItemUse_XiDianBook(role, item)
    local zsskill_lv = {}
    zsskill_lv[0] = GetSkillLv(role, 453)
    zsskill_lv[1] = GetSkillLv(role, 454)
    zsskill_lv[2] = GetSkillLv(role, 455)
    zsskill_lv[3] = GetSkillLv(role, 456)
    zsskill_lv[4] = GetSkillLv(role, 457)
    zsskill_lv[5] = GetSkillLv(role, 458)
    local n = 0
    local item_canget = GetChaFreeBagGridNum(role)
    if item_canget < 2 then
        SystemNotice(role, "2")
        UseItemFailed(role)
    else
        for i = 0, 5, 1 do
            if zsskill_lv[i] >= 1 then
                n = n + 1
            end
        end
        local cha_skill_num = GetChaAttr(role, ATTR_TP)
        local clear_skill_num = ClearAllFightSkill(role)
        cha_skill_num = cha_skill_num + clear_skill_num
        if n > 0 then
            local job = GetChaAttr(role, ATTR_JOB)
            local item_id = {}
            item_id[8] = 2957
            item_id[9] = 2956
            item_id[12] = 2961
            item_id[13] = 2959
            item_id[14] = 2958
            item_id[16] = 2960
            GiveItem(role, 0, item_id[job], 1, 4)
            GiveItem(role, 0, 1572, 1, 4)
            cha_skill_num = cha_skill_num - 2
        end
        SetChaAttr(role, ATTR_TP, cha_skill_num)
    end
end
function ItemUse_MOLIBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 4 empty inventory slots to open the Chest")
        UseItemFailed(role)
        return
    end
    local rad = math.random(1, 12)
    if rad == 1 then
        GiveItem(role, 0, 5107, 1, 4)
        GiveItem(role, 0, 5108, 1, 4)
        GiveItem(role, 0, 5109, 1, 4)
    elseif rad == 2 then
        GiveItem(role, 0, 5111, 1, 4)
        GiveItem(role, 0, 5112, 1, 4)
        GiveItem(role, 0, 5113, 1, 4)
    elseif rad == 3 then
        GiveItem(role, 0, 5115, 1, 4)
        GiveItem(role, 0, 5116, 1, 4)
        GiveItem(role, 0, 5117, 1, 4)
    elseif rad == 4 then
        GiveItem(role, 0, 5119, 1, 4)
        GiveItem(role, 0, 5120, 1, 4)
        GiveItem(role, 0, 5121, 1, 4)
    elseif rad == 5 then
        GiveItem(role, 0, 5123, 1, 4)
        GiveItem(role, 0, 5124, 1, 4)
        GiveItem(role, 0, 5125, 1, 4)
    elseif rad == 6 then
        GiveItem(role, 0, 5127, 1, 4)
        GiveItem(role, 0, 5128, 1, 4)
        GiveItem(role, 0, 5129, 1, 4)
    elseif rad == 7 then
        GiveItem(role, 0, 5130, 1, 4)
        GiveItem(role, 0, 5131, 1, 4)
        GiveItem(role, 0, 5132, 1, 4)
        GiveItem(role, 0, 5133, 1, 4)
    elseif rad == 8 then
        GiveItem(role, 0, 5134, 1, 4)
        GiveItem(role, 0, 5135, 1, 4)
        GiveItem(role, 0, 5136, 1, 4)
        GiveItem(role, 0, 5137, 1, 4)
    elseif rad == 9 then
        GiveItem(role, 0, 5138, 1, 4)
        GiveItem(role, 0, 5139, 1, 4)
        GiveItem(role, 0, 5140, 1, 4)
        GiveItem(role, 0, 5141, 1, 4)
    elseif rad == 10 then
        GiveItem(role, 0, 5143, 1, 4)
        GiveItem(role, 0, 5144, 1, 4)
        GiveItem(role, 0, 5145, 1, 4)
    elseif rad == 11 then
        GiveItem(role, 0, 5147, 1, 4)
        GiveItem(role, 0, 5148, 1, 4)
        GiveItem(role, 0, 5149, 1, 4)
    elseif rad == 12 then
        GiveItem(role, 0, 5151, 1, 4)
        GiveItem(role, 0, 5152, 1, 4)
        GiveItem(role, 0, 5153, 1, 4)
    end
end
function ItemUse_XNLP(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    local star = 0
    if CheckDateNum >= 10122 and CheckDateNum <= 10123 then
        ItemUse_XINBOX(role, Item)
    end
end
function ItemUse_SZF(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Unable to use Pirate Voucher 8. Requires at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2306, 1, 4)
    local cha_name = GetChaDefaultName(role)
    local message = "Congratulations" .. cha_name .. "Obtained 3k RMB worth of IPOD prizes"
    Notice(message)
end
function ItemUse_XNBOX(role, Item)
    local lv = GetChaAttr(role, ATTR_LV)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if lv < 40 then
        SystemNotice(role, "Currently lower than Lv 40. Unable to use item!")
        UseItemFailed(role)
        return
    end
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 free slot to open chest")
        UseItemFailed(role)
        return
    end
    local el = math.random(1, 30000)
    if el >= 29700 and el < 30000 then
        GiveItem(role, 0, 2240, 1, 4)
    elseif el >= 28700 and el < 29700 then
        GiveItem(role, 0, 2237, 1, 4)
    elseif el >= 25700 and el < 28700 then
        GiveItem(role, 0, 2239, 1, 4)
    elseif el >= 15700 and el < 25700 then
        GiveItem(role, 0, 2241, 1, 4)
    else
        local EID = math.random(2242, 2245)
        GiveItem(role, 0, EID, 1, 4)
    end
end
function ItemUse_CJBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "To open a Chest requires 1 empty slot")
        UseItemFailed(role)
        return
    end
    local r1, r2 = MakeItem(role, C1, 1, 4)
    local Item_newJL = GetChaItem(role, 2, r2)
    local Item_newJLID = GetItemID(Item_newJL)

    local str_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_STR)
    local con_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_CON)
    local agi_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_AGI)
    local dex_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_DEX)
    local sta_JLone = GetItemAttr(Item_newJL, ITEMATTR_VAL_STA)

    local Num_JL = GetItemForgeParam(Item_newJL, 1)
    Num_JL = TansferNum(Num_JL)
    local Part1_JL = GetNum_Part1(Num_JL)
    local Part2_JL = GetNum_Part2(Num_JL)
    local Part3_JL = GetNum_Part3(Num_JL)
    local Part4_JL = GetNum_Part4(Num_JL)
    local Part5_JL = GetNum_Part5(Num_JL)
    local Part6_JL = GetNum_Part6(Num_JL)
    local Part7_JL = GetNum_Part7(Num_JL)
    if
        Item_newJLID == 231 or Item_newJLID == 232 or Item_newJLID == 233 or Item_newJLID == 234 or Item_newJLID == 235 or
            Item_newJLID == 236 or
            Item_newJLID == 237 or
            Item_newJLID == 681
     then
        Part1_JL = 1
        Num_JL = SetNum_Part1(Num_JL, 1)
        SetItemForgeParam(Item_newJL, 1, Num_JL)
    end
    str_JLone = N1
    con_JLone = N2
    agi_JLone = N3
    dex_JLone = N4
    sta_JLone = N5
    local new_lv = N1 + N2 + N3 + N4 + N5
    local new_MAXENERGY = 240 * (new_lv + 1)
    if new_MAXENERGY > 6480 then
        new_MAXENERGY = 6480
    end
    local new_MAXURE = 5000 + 1000 * new_lv
    if new_MAXURE > 32000 then
        new_MAXURE = 32000
    end
    SetItemAttr(Item_newJL, ITEMATTR_VAL_STR, str_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_CON, con_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_AGI, agi_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_DEX, dex_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_VAL_STA, sta_JLone)
    SetItemAttr(Item_newJL, ITEMATTR_MAXENERGY, new_MAXENERGY)
    SetItemAttr(Item_newJL, ITEMATTR_MAXURE, new_MAXURE)
    SetItemAttr(Item_newJL, ITEMATTR_ENERGY, new_MAXENERGY)
    SetItemAttr(Item_newJL, ITEMATTR_URE, new_MAXURE)
    local cha_name = GetChaDefaultName(role)

    LG("star_CJBOX", cha_name, C1, N1, N2, N3, N4, N5)
end
function ItemUse_ALDXB(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end

    local cha_name = GetChaDefaultName(role)
    local star = math.random(1, 20000)

    if star <= 3700 then
        GiveItem(role, 0, 2440, 10, 4)
    elseif star >= 3701 and star <= 5700 then
        GiveItem(role, 0, 3885, 1, 4)
    elseif star >= 5701 and star <= 7100 then
        GiveItem(role, 0, 3094, 1, 4)
    elseif star >= 7101 and star <= 13100 then
        local el = math.random(1, 5)
        if el == 1 then
            GiveItem(role, 0, 19540, 3, 4)
        elseif el == 2 then
            GiveItem(role, 0, 19538, 3, 4)
        elseif el == 3 then
            GiveItem(role, 0, 19546, 3, 4)
        elseif el == 4 then
            GiveItem(role, 0, 19539, 3, 4)
        elseif el == 5 then
            GiveItem(role, 0, 19525, 3, 4)
        end
    elseif star >= 13101 and star <= 15100 then
        local el = math.random(1, 2)
        if el == 1 then
            GiveItem(role, 0, 0849, 1, 4)
        elseif el == 2 then
            GiveItem(role, 0, 0680, 1, 4)
        end
    elseif star >= 15101 and star <= 19300 then
        local el1 = math.random(1, 6)
        if el1 == 1 then
            GiveItem(role, 0, 2438, 5, 4)
        elseif el1 == 2 then
            GiveItem(role, 0, 2419, 3, 4)
        elseif el1 == 3 then
            GiveItem(role, 0, 2386, 4, 4)
        elseif el1 == 4 then
            GiveItem(role, 0, 0179, 1, 4)
        elseif el1 == 5 then
            GiveItem(role, 0, 3084, 1, 4)
        elseif el1 == 6 then
            GiveItem(role, 0, 3085, 1, 4)
        end
    elseif star >= 19301 and star <= 19600 then
        local el1 = math.random(1, 5)
        if el1 == 1 then
            GiveItem(role, 0, 0863, 1, 4)
            local message = cha_name .. "open the Aladdin Parcel,he get the Gem of Rage"
            Notice(message)
        elseif el1 == 2 then
            GiveItem(role, 0, 0860, 1, 4)
            local message1 = cha_name .. "open the Aladdin Parcel,he get the Gem of the Wind"
            Notice(message1)
        elseif el1 == 3 then
            GiveItem(role, 0, 0861, 1, 4)
            local message2 = cha_name .. "open the Aladdin Parcel,he get the Gem of Striking"
            Notice(message2)
        elseif el1 == 4 then
            GiveItem(role, 0, 0862, 1, 4)
            local message3 = cha_name .. "open the Aladdin Parcel,he get the Gem of Colossus"
            Notice(message3)
        elseif el1 == 5 then
            GiveItem(role, 0, 1012, 1, 4)
            local message4 = cha_name .. "open the Aladdin Parcel,he get the Gem of Soul"
            Notice(message4)
        end
    elseif star == 19601 then
        GiveItem(role, 0, 0192, 1, 4)
        local message8 = cha_name .. "open the Aladdin Parcel,he get the Chest of Kylin"
        Notice(message8)
    elseif star >= 19601 and star <= 20000 then
        GiveItem(role, 0, 2224, 1, 4)
        local message8 = cha_name .. "open the Aladdin Parcel,he get the Modern Apparel Chest"
        Notice(message8)
    end
end
function ItemUse_SummonBigBOSS(role, Item)
    local map_name_role = GetChaMapName(role)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(Cha_Boat, "Not usable on the sea")
        UseItemFailed(Cha_Boat)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end
    if map_name_role == "guildwar" then
        local x, y = GetChaPos(role)
        if GetChaGuildID(role) <= 100 and GetChaGuildID(role) > 0 then
            local MonsterID = 1007
            local Refresh = 1300
            local life = 1200000
            local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
            SetChaLifeTime(new, life)
            SetChaSideID(new, 1)
        end

        if GetChaGuildID(role) > 100 and GetChaGuildID(role) <= 200 then
            local MonsterID = 1008
            local Refresh = 1300
            local life = 1200000
            local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
            SetChaLifeTime(new, life)
            SetChaSideID(new, 2)
        end
    elseif map_name_role == "guildwar2" then
        local x, y = GetChaPos(role)
        if GetChaGuildID(role) <= 100 and GetChaGuildID(role) > 0 then
            local MonsterID = 1007
            local Refresh = 1300
            local life = 12600000
            local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
            SetChaLifeTime(new, life)
            SetChaSideID(new, 1)
        end

        if GetChaGuildID(role) > 100 and GetChaGuildID(role) <= 200 then
            local MonsterID = 1008
            local Refresh = 1300
            local life = 1200000
            local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
            SetChaLifeTime(new, life)
            SetChaSideID(new, 2)
        end
    else
        SystemNotice(role, "This ticket can only be used in Sacred War map")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SL1(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        SystemNotice(role, "Can only be used on the sea.")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(Cha_Boat)
    local x_move = 0
    local y_move = 0
    local fx_move = math.random(1, 8)
    if fx_move == 1 then
        x_move = 1200
        y_move = 0
    elseif fx_move == 2 then
        x_move = 1200
        y_move = -1200
    elseif fx_move == 3 then
        x_move = 0
        y_move = -1000
    elseif fx_move == 4 then
        x_move = -1000
        y_move = -1000
    elseif fx_move == 5 then
        x_move = 1000
        y_move = 0
    elseif fx_move == 6 then
        x_move = -1000
        y_move = 1000
    elseif fx_move == 7 then
        x_move = 0
        y_move = 1000
    else
        x_move = 1000
        y_move = 1000
    end
    x = x_move + x
    y = y_move + y

    local MonsterID = 942
    local Refresh = 700000
    local life = 600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, Cha_Boat)
    SetChaLifeTime(new, life)
end
function ItemUse_FaSheng2(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Summon a monster in a safe zone? Please be considerate!")
        UseItemFailed(role)
        return
    end
    local radID = math.random(1, 15)
    local MonsterID = 0

    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    if radID == 1 then
        MonsterID = 847
    elseif radID == 2 then
        MonsterID = 848
    elseif radID == 3 then
        MonsterID = 849
    elseif radID == 4 then
        MonsterID = 850
    elseif radID == 5 then
        MonsterID = 851
    elseif radID == 6 then
        MonsterID = 852
    elseif radID == 7 then
        MonsterID = 211
    elseif radID == 8 then
        MonsterID = 706
    elseif radID == 9 then
        MonsterID = 673
    elseif radID == 10 then
        MonsterID = 690
    elseif radID == 11 then
        MonsterID = 691
    elseif radID == 12 then
        MonsterID = 692
    elseif radID == 13 then
        MonsterID = 693
    elseif radID == 14 then
        MonsterID = 106
    elseif radID == 15 then
        MonsterID = 289
    end
    local Refresh = 3700000
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_ShaBao2(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Training in Safe Zone? Dream on!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local MonsterID = 938
    local Refresh = 1900000
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_SL2(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        SystemNotice(role, "Can only be used on the sea.")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(Cha_Boat)
    local x_move = 0
    local y_move = 0
    local fx_move = math.random(1, 8)
    if fx_move == 1 then
        x_move = 1200
        y_move = 0
    elseif fx_move == 2 then
        x_move = 1200
        y_move = -1200
    elseif fx_move == 3 then
        x_move = 0
        y_move = -1000
    elseif fx_move == 4 then
        x_move = -1000
        y_move = -1000
    elseif fx_move == 5 then
        x_move = 1000
        y_move = 0
    elseif fx_move == 6 then
        x_move = -1000
        y_move = 1000
    elseif fx_move == 7 then
        x_move = 0
        y_move = 1000
    else
        x_move = 1000
        y_move = 1000
    end
    x = x_move + x
    y = y_move + y
    local MonsterID = 943
    local Refresh = 700000
    local life = 600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, Cha_Boat)
    SetChaLifeTime(new, life)
end
function ItemUse_FaSheng3(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Summon a monster in a safe zone? Please be considerate!")
        UseItemFailed(role)
        return
    end
    local radID = math.random(1, 7)
    local MonsterID = 0

    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    if radID == 1 then
        MonsterID = 757
    elseif radID == 2 then
        MonsterID = 679
    elseif radID == 3 then
        MonsterID = 678
    elseif radID == 4 then
        MonsterID = 707
    elseif radID == 5 then
        MonsterID = 708
    elseif radID == 6 then
        MonsterID = 776
    elseif radID == 7 then
        MonsterID = 74
    end
    local Refresh = 7300000
    local life = 7200000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_ShaBao3(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Training in Safe Zone? Dream on!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local MonsterID = 939
    local Refresh = 1900000
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_SL3(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        SystemNotice(role, "Can only be used on the sea.")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(Cha_Boat)
    local x_move = 0
    local y_move = 0
    local fx_move = math.random(1, 8)
    if fx_move == 1 then
        x_move = 1200
        y_move = 0
    elseif fx_move == 2 then
        x_move = 1200
        y_move = -1200
    elseif fx_move == 3 then
        x_move = 0
        y_move = -1000
    elseif fx_move == 4 then
        x_move = -1000
        y_move = -1000
    elseif fx_move == 5 then
        x_move = 1000
        y_move = 0
    elseif fx_move == 6 then
        x_move = -1000
        y_move = 1000
    elseif fx_move == 7 then
        x_move = 0
        y_move = 1000
    else
        x_move = 1000
        y_move = 1000
    end
    x = x_move + x
    y = y_move + y
    local MonsterID = 944
    local Refresh = 700000
    local life = 600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, Cha_Boat)
    SetChaLifeTime(new, life)
end
function ItemUse_FaSheng4(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Summon a monster in a safe zone? Please be considerate!")
        UseItemFailed(role)
        return
    end
    local radID = math.random(1, 5)
    local MonsterID = 0
    local Refresh = 0
    local life = 0

    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    if radID == 1 then
        MonsterID = 952
    elseif radID == 2 then
        MonsterID = 805
    elseif radID == 3 then
        MonsterID = 807
    elseif radID == 4 then
        MonsterID = 786
    elseif radID == 5 then
        MonsterID = 788
    end

    if MonsterID == 952 then
        Refresh = 10900000
        life = 10800000
    else
        Refresh = 7300000
        life = 7200000
    end
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_ShaBao4(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Training in Safe Zone? Dream on!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local MonsterID = 940
    local Refresh = 1900000
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_ShaBao5(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Training in Safe Zone? Dream on!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local x_move = 5
    local y_move = 5
    x = x_move + x
    y = y_move + y
    local MonsterID = 941
    local Refresh = 1900000
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_SL5(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        SystemNotice(role, "Can only be used on the sea.")
        UseItemFailed(role)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(Cha_Boat)
    local x_move = 0
    local y_move = 0
    local fx_move = math.random(1, 8)
    if fx_move == 1 then
        x_move = 1200
        y_move = 0
    elseif fx_move == 2 then
        x_move = 1200
        y_move = -1200
    elseif fx_move == 3 then
        x_move = 0
        y_move = -1000
    elseif fx_move == 4 then
        x_move = -1000
        y_move = -1000
    elseif fx_move == 5 then
        x_move = 1000
        y_move = 0
    elseif fx_move == 6 then
        x_move = -1000
        y_move = 1000
    elseif fx_move == 7 then
        x_move = 0
        y_move = 1000
    else
        x_move = 1000
        y_move = 1000
    end
    x = x_move + x
    y = y_move + y

    local MonsterID = 946
    local Refresh = 700000
    local life = 600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, Cha_Boat)

    SetChaLifeTime(new, life)
end
--Exploding Lamb Lv1-----------------------------------------------------------
function ItemUse_ZBML1 ( role , Item  )
 
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~=  nil then
		SystemNotice( Cha_Boat , "Not usable on the sea" )
		UseItemFailed ( Cha_Boat )
		return
	end
	local reg = 0
	      reg =IsChaInRegion( role, 2 )
	if reg == 1 then
		SystemNotice( role , "Unable to use in Safe Zone" )
		UseItemFailed ( role )
                return
	end
	
	local x,y = GetChaPos(role)
	local x_move=0
	local y_move=0
        local fx_move = math.random ( 1,8 )
	   if  fx_move == 1 then
	       x_move = 800
	       y_move = 0
	 elseif fx_move == 2 then
	       x_move=800
	       y_move=-800
	 elseif fx_move == 3 then
	       x_move=0
	       y_move=-800
	 elseif fx_move == 4 then
	       x_move=-800
	       y_move=-800
	 elseif fx_move == 5 then
	       x_move=800
	       y_move=0
	 elseif fx_move == 6 then
	       x_move=-800
	       y_move=800
	 elseif fx_move == 7 then
	       x_move=0
	       y_move=800
	 else  x_move=800
	       y_move=800
	 end
	      x =x_move + x
	      y =y_move + y
	local MonsterID = 947
	local Refresh = 700000					--
	local life = 600000					--
	local new = CreateChaX( MonsterID , x , y , 145 , Refresh,role )
	SetChaLifeTime( new, life )
end
--Exploding Lamb Lv2-----------------------------------------------------------

function ItemUse_ZBML2 ( role , Item  )
 
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~=  nil then
		SystemNotice( Cha_Boat , "Not usable on the sea" )
		UseItemFailed ( Cha_Boat )
		return
	end
	local reg = 0
	      reg =IsChaInRegion( role, 2 )
	if reg == 1 then
		SystemNotice( role , "Unable to use in Safe Zone" )
		UseItemFailed ( role )
                return
	end
	
	local x,y = GetChaPos(role)
	local x_move=0
	local y_move=0
        local fx_move = math.random ( 1,8 )
	   if  fx_move == 1 then
	       x_move = 800
	       y_move = 0
	 elseif fx_move == 2 then
	       x_move=800
	       y_move=-800
	 elseif fx_move == 3 then
	       x_move=0
	       y_move=-800
	 elseif fx_move == 4 then
	       x_move=-800
	       y_move=-800
	 elseif fx_move == 5 then
	       x_move=800
	       y_move=0
	 elseif fx_move == 6 then
	       x_move=-800
	       y_move=800
	 elseif fx_move == 7 then
	       x_move=0
	       y_move=800
	 else  x_move=800
	       y_move=800
	 end
	      x =x_move + x
	      y =y_move + y
	local MonsterID = 948
	local Refresh = 700000					--
	local life = 600000					--
	local new = CreateChaX( MonsterID , x , y , 145 , Refresh,role )
	SetChaLifeTime( new, life )
end
--Exploding Lamb Lv3-----------------------------------------------------------

function ItemUse_ZBML3 ( role , Item  )
 
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~=  nil then
		SystemNotice( Cha_Boat , "Not usable on the sea" )
		UseItemFailed ( Cha_Boat )
		return
	end
	local reg = 0
	      reg =IsChaInRegion( role, 2 )
	if reg == 1 then
		SystemNotice( role , "Unable to use in Safe Zone" )
		UseItemFailed ( role )
                return
	end
	
	local x,y = GetChaPos(role)
	local x_move=0
	local y_move=0
        local fx_move = math.random ( 1,8 )
	   if  fx_move == 1 then
	       x_move = 800
	       y_move = 0
	 elseif fx_move == 2 then
	       x_move=800
	       y_move=-800
	 elseif fx_move == 3 then
	       x_move=0
	       y_move=-800
	 elseif fx_move == 4 then
	       x_move=-800
	       y_move=-800
	 elseif fx_move == 5 then
	       x_move=800
	       y_move=0
	 elseif fx_move == 6 then
	       x_move=-800
	       y_move=800
	 elseif fx_move == 7 then
	       x_move=0
	       y_move=800
	 else  x_move=800
	       y_move=800
	 end
	      x =x_move + x
	      y =y_move + y
	local MonsterID = 949
	local Refresh = 700000					--
	local life = 600000					--
	local new = CreateChaX( MonsterID , x , y , 145 , Refresh,role )
	SetChaLifeTime( new, life )
end
--Exploding Lamb Lv4-----------------------------------------------------------

function ItemUse_ZBML4 ( role , Item  )
 
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~=  nil then
		SystemNotice( Cha_Boat , "Not usable on the sea" )
		UseItemFailed ( Cha_Boat )
		return
	end
	local reg = 0
	      reg =IsChaInRegion( role, 2 )
	if reg == 1 then
		SystemNotice( role , "Unable to use in Safe Zone" )
		UseItemFailed ( role )
                return
	end
	
	local x,y = GetChaPos(role)
	local x_move=0
	local y_move=0
        local fx_move = math.random ( 1,8 )
	   if  fx_move == 1 then
	       x_move = 800
	       y_move = 0
	 elseif fx_move == 2 then
	       x_move=800
	       y_move=-800
	 elseif fx_move == 3 then
	       x_move=0
	       y_move=-800
	 elseif fx_move == 4 then
	       x_move=-800
	       y_move=-800
	 elseif fx_move == 5 then
	       x_move=800
	       y_move=0
	 elseif fx_move == 6 then
	       x_move=-800
	       y_move=800
	 elseif fx_move == 7 then
	       x_move=0
	       y_move=800
	 else  x_move=800
	       y_move=800
	 end
	      x =x_move + x
	      y =y_move + y
	local MonsterID = 950
	local Refresh = 700000					--
	local life = 600000					--
	local new = CreateChaX( MonsterID , x , y , 145 , Refresh,role )
	SetChaLifeTime( new, life )
end
--Exploding Lamb Lv5-----------------------------------------------------------

function ItemUse_ZBML5 ( role , Item  )
 
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat ( role )
	if Cha_Boat ~=  nil then
		SystemNotice( Cha_Boat , "Not usable on the sea" )
		UseItemFailed ( Cha_Boat )
		return
	end
	local reg = 0
	      reg =IsChaInRegion( role, 2 )
	if reg == 1 then
		SystemNotice( role , "Unable to use in Safe Zone" )
		UseItemFailed ( role )
                return
	end
	
	local x,y = GetChaPos(role)
	local x_move=0
	local y_move=0
        local fx_move = math.random ( 1,8 )
	   if  fx_move == 1 then
	       x_move = 800
	       y_move = 0
	 elseif fx_move == 2 then
	       x_move=800
	       y_move=-800
	 elseif fx_move == 3 then
	       x_move=0
	       y_move=-800
	 elseif fx_move == 4 then
	       x_move=-800
	       y_move=-800
	 elseif fx_move == 5 then
	       x_move=800
	       y_move=0
	 elseif fx_move == 6 then
	       x_move=-800
	       y_move=800
	 elseif fx_move == 7 then
	       x_move=0
	       y_move=800
	 else  x_move=800
	       y_move=800
	 end
	      x =x_move + x
	      y =y_move + y
	local MonsterID = 951
	local Refresh = 700000					--
	local life = 600000					--
	local new = CreateChaX( MonsterID , x , y , 145 , Refresh,role )
	SetChaLifeTime( new, life )
end
function ItemUse_HonorPoint(role, Item)
	local HonorBook_Num = 0
	local HonorBook_Num = CheckBagItem( role,3849 )
		if HonorBook_Num < 1 then
		SystemNotice( role , "You do not have Mark of Honor")
		return 0
		end
	local Book2 =  GetChaItem2 ( role , 2 , 3849 )
	local HonorPoint=GetItemAttr ( Book2 , ITEMATTR_VAL_STR)
	
	local HonorPoint_X=HonorPoint+100
	SetItemAttr ( Book2 , ITEMATTR_VAL_STR,HonorPoint_X)
	SynChaKitbag(role, 13)
end
function ItemUse_Hadisi(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 empty slots to open Chest of Hardin")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2817, 1, 4)
    GiveItem(role, 0, 2818, 1, 4)
    GiveItem(role, 0, 2819, 1, 4)
end
function ItemUse_Anhei(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 empty slots to open Chest of Darkness")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2820, 1, 4)
    GiveItem(role, 0, 2821, 1, 4)
    GiveItem(role, 0, 2822, 1, 4)
end
function ItemUse_Diyu(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Chest of Abaddon requires at least 3 empty inventory slots")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2823, 1, 4)
    GiveItem(role, 0, 2824, 1, 4)
    GiveItem(role, 0, 2825, 1, 4)
end
function ItemUse_Xiuluo(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 free slots to open Chest of Asura")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2826, 1, 4)
    GiveItem(role, 0, 2827, 1, 4)
    GiveItem(role, 0, 2828, 1, 4)
end
function ItemUse_Youming(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 empty slots to open Chest of Abyss")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2829, 1, 4)
    GiveItem(role, 0, 2830, 1, 4)
    GiveItem(role, 0, 2831, 1, 4)
end
function ItemUse_Minghe(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 empty inventory slots to open Chest of Styx")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2832, 1, 4)
    GiveItem(role, 0, 2833, 1, 4)
    GiveItem(role, 0, 2834, 1, 4)
end
function ItemUse_Sishen(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "To open the Carcass of Death requires at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    local job = GetChaAttr(role, ATTR_JOB)
    local lv = GetChaAttr(role, ATTR_LV)
    local star_rad = math.random(1, 2)
    if lv < 40 then
        SystemNotice(role, "Currently lower than Lv 40. Unable to use item!")
        UseItemFailed(role)
    elseif job == 9 then
        if star_rad == 1 then
            GiveItem(role, 0, 2331, 1, 4)
        else
            GiveItem(role, 0, 2332, 1, 4)
        end
    elseif job == 8 then
        if star_rad == 1 then
            GiveItem(role, 0, 2333, 1, 4)
        else
            GiveItem(role, 0, 2334, 1, 4)
        end
    elseif job == 12 then
        local eleven_rad = math.random(1, 4)
        if eleven_rad == 1 then
            GiveItem(role, 0, 2337, 1, 4)
        elseif eleven_rad == 2 then
            GiveItem(role, 0, 2338, 1, 4)
        elseif eleven_rad == 3 then
            GiveItem(role, 0, 2339, 1, 4)
        else
            GiveItem(role, 0, 2340, 1, 4)
        end
    elseif job == 16 then
        if star_rad == 1 then
            GiveItem(role, 0, 2335, 1, 4)
        else
            GiveItem(role, 0, 2336, 1, 4)
        end
    elseif job == 13 then
        if star_rad == 1 then
            GiveItem(role, 0, 2341, 1, 4)
        else
            GiveItem(role, 0, 2342, 1, 4)
        end
    elseif job == 14 then
        if star_rad == 1 then
            GiveItem(role, 0, 2343, 1, 4)
        else
            GiveItem(role, 0, 2344, 1, 4)
        end
    else
        SystemNotice(role, "Class mismatch. Item can only be used after second class advancement!")
        UseItemFailed(role)
    end
end
function ItemUse_Zhenheilong(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 5 then
        SystemNotice(role, "You need at least 4 empty slots to open Rightful Chest of Black Dragon")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2367, 1, 16)
    GiveItem(role, 0, 2368, 1, 16)
    GiveItem(role, 0, 2369, 1, 16)
    local cha_type = GetChaTypeID(role)
    if cha_type == 4 then
        GiveItem(role, 0, 2370, 1, 16)
    end
end
function ItemUse_HJZHQ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Not usable on the sea.")
        UseItemFailed(role)
        return
    end
    local pet_num = GetPetNum(role)
    if pet_num >= 5 then
        SystemNotice(role, "Max of limit of 5 pets reached. Please try again later!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    x_resume = 5
    y_resume = 5
    x = x + x_resume
    y = y + y_resume
    local MonsterID = 930
    local Refresh = 1800
    local life = 1800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaHost(new, role)
    SetChaLifeTime(new, life)
    SetChaTarget(new, role)
    PlayEffect(new, 361)
end
function ItemUse_MLZHQ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Not usable on the sea.")
        UseItemFailed(role)
        return
    end
    local pet_num = GetPetNum(role)
    if pet_num >= 5 then
        SystemNotice(role, "Max of limit of 5 pets reached. Please try again later!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    x_resume = 5
    y_resume = 5
    x = x + x_resume
    y = y + y_resume
    local MonsterID = 931
    local Refresh = 3600
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaHost(new, role)
    SetChaLifeTime(new, life)
    SetChaTarget(new, role)
    PlayEffect(new, 361)
end
function ItemUse_XRZHQ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Not usable on the sea.")
        UseItemFailed(role)
        return
    end
    local pet_num = GetPetNum(role)
    if pet_num >= 5 then
        SystemNotice(role, "Max of limit of 5 pets reached. Please try again later!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    x_resume = 5
    y_resume = 5
    x = x + x_resume
    y = y + y_resume
    local MonsterID = 932
    local Refresh = 7200
    local life = 7200000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaHost(new, role)
    SetChaLifeTime(new, life)
    SetChaTarget(new, role)
    PlayEffect(new, 361)
end
function ItemUse_SDZHQ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Not usable on the sea.")
        UseItemFailed(role)
        return
    end
    local pet_num = GetPetNum(role)
    if pet_num >= 5 then
        SystemNotice(role, "Max of limit of 5 pets reached. Please try again later!")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    x_resume = 5
    y_resume = 5
    x = x + x_resume
    y = y + y_resume
    local MonsterID = 929
    local Refresh = 10800
    local life = 10800000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaHost(new, role)
    SetChaLifeTime(new, life)
    SetChaTarget(new, role)
    PlayEffect(new, 361)
end
function ItemUse_JMSDBOX(role, Item)
    local item_type = BoxXiang_BaoZhaBOX
    local item_type_rad = BoxXiang_BaoZhaBOX_Rad
    local item_type_count = BoxXiang_BaoZhaBOX_Count
    local maxitem = BoxXiang_baozhabao_Mxcount
    local item_quality = BoxXiang_baozhabao_Qua
    local General = 0
    local ItemId = 0
    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Not enough slots. Fail to open Posh Christmas Box")
        UseItemFailed(role)
        return
    end
    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
            break
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end

    GiveItem(role, 0, ItemId, ItemCount, item_quality)
    local GoodItem = {}
    GoodItem[0] = 3111
    GoodItem[1] = 3110
    GoodItem[2] = 3112
    GoodItem[3] = 3886
    GoodItem[4] = 3093
    GoodItem[5] = 3090
    GoodItem[6] = 430
    GoodItem[7] = 179
    GoodItem[8] = 3084
    GoodItem[9] = 3085
    GoodItem[10] = 0244
    GoodItem[11] = 0250
    GoodItem[12] = 0253
    GoodItem[13] = 0260
    GoodItem[14] = 0860
    GoodItem[15] = 0861
    GoodItem[16] = 0862
    GoodItem[17] = 3458
    GoodItem[18] = 0247
    GoodItem[19] = 0271
    local Good_C = 0
    for Good_C = 0, 19, 1 do
        if ItemId == GoodItem[Good_C] then
            local itemname = GetItemName(ItemId)
            local cha_name = GetChaDefaultName(role)
            local message = cha_name .. " opens a Posh Christmas Box and obtained " .. itemname
            Notice(message)
        end
    end
end
function ItemUse_MWHJ(role, Item)
    local el_exp = GetChaAttr(role, ATTR_CEXP)
    local exp1 = el_exp
    local charLv = Lv(role)
    local exp_resume = 5000
    local exp_resume_1 = 100
    el_exp = el_exp + exp_resume
    if charLv >= 80 then
        el_exp = exp1 + exp_resume_1
    end
    SetCharaAttr(el_exp, role, ATTR_CEXP)
end
function ItemUse_SDDLB(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 5 then
        SystemNotice(role, "You require at least 6 empty inventory slots. Failed to open Christmas Box.")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2894, 1, 4)
    GiveItem(role, 0, 2893, 10, 4)
    GiveItem(role, 0, 2889, 1, 4)
    GiveItem(role, 0, 2890, 1, 4)
    GiveItem(role, 0, 2891, 1, 4)
    GiveItem(role, 0, 2896, 99, 4)
end
function ItemUse_SDDC(role, Item)
    local Cha_Boat = 0
    local charLv = Lv(role)
    local dif_exp_one = DEXP[charLv + 1]
    local dif_exp_three = DEXP[charLv + 3]
    local dif_exp_five = DEXP[charLv + 5]
    local Exp_star = GetChaAttr(role, ATTR_CEXP)
    local dif_exp_half = (DEXP[charLv + 1] - DEXP[charLv]) * 0.5 + Exp_star + 10
    local dif_exp_thalf = (DEXP[charLv + 1] - DEXP[charLv]) * 0.3 + Exp_star + 10
    local dif_exp_thalf_b = dif_exp_thalf - DEXP[charLv + 1]
    local dif_exp_thalf_c = (DEXP[charLv + 1] - DEXP[charLv]) * 0.01 + Exp_star
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    elseif charLv >= 1 and charLv <= 9 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_five)
    elseif charLv >= 10 and charLv <= 29 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_three)
    elseif charLv >= 30 and charLv <= 59 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_one)
    elseif charLv >= 60 and charLv <= 75 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_half)
    elseif charLv >= 76 and charLv <= 85 and charLv ~= 79 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv == 79 and dif_exp_thalf_b <= 0 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv == 79 and dif_exp_thalf_b > 0 then
        dif_exp_thalf = dif_exp_thalf_b * 0.02 + DEXP[charLv + 1]
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf)
    elseif charLv >= 86 then
        SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf_c)
    end
end
function ItemUse_GWZHQ(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet <= 3 then
        SystemNotice(role, "Not enough slots. You need at least 4 empty slots. Failed to open Posh Christmas Box")
        UseItemFailed(role)
        return
    end
    GiveItem(role, 0, 2888, 1, 4)
    GiveItem(role, 0, 2889, 1, 4)
    GiveItem(role, 0, 2890, 1, 4)
    GiveItem(role, 0, 2891, 1, 4)
end
function ItemUse_JRDQBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 10 then
        SystemNotice(role, "To open Gift of the Beauty requires at least 10 empty inventory slots")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    if cha_type == 1 or cha_type == 2 then
        SystemNotice(role, "Gift of the Beauty can only be opened by female")
        UseItemFailed(role)
        return
    end

    local el = math.random(1, 100)
    if el >= 1 and el <= 20 then
        GiveItem(role, 0, 3343, 1, 4)
    elseif el >= 21 and el <= 40 then
        GiveItem(role, 0, 3354, 1, 4)
        GiveItem(role, 0, 3355, 1, 4)
        GiveItem(role, 0, 3356, 1, 4)
        GiveItem(role, 0, 3357, 1, 4)
        GiveItem(role, 0, 3358, 1, 4)
        GiveItem(role, 0, 3359, 1, 4)
    elseif el >= 41 and el <= 45 then
        GiveItem(role, 0, 937, 1, 4)
    elseif el >= 46 and el <= 65 then
        GiveItem(role, 0, 4264, 1, 4)
        GiveItem(role, 0, 4265, 1, 4)
        GiveItem(role, 0, 4266, 1, 4)
        GiveItem(role, 0, 4267, 1, 4)
        GiveItem(role, 0, 4268, 1, 4)
        GiveItem(role, 0, 4269, 1, 4)
        GiveItem(role, 0, 4270, 1, 4)
        GiveItem(role, 0, 4271, 1, 4)
        GiveItem(role, 0, 4272, 1, 4)
        GiveItem(role, 0, 4273, 1, 4)
    elseif el >= 66 and el <= 75 then
        GiveItem(role, 0, 3094, 3, 4)
    elseif el >= 76 and el <= 85 then
        GiveItem(role, 0, 855, 10, 4)
    elseif el >= 86 and el < 87 then
        GiveItem(role, 0, 1012, 1, 4)
    elseif el >= 87 and el < 89 and cha_type == 3 then
        GiveItem(role, 0, 5244, 1, 4)
        GiveItem(role, 0, 5245, 1, 4)
        GiveItem(role, 0, 5246, 1, 4)
        GiveItem(role, 0, 5247, 1, 4)
    elseif el >= 89 and el <= 90 and cha_type == 4 then
        GiveItem(role, 0, 5252, 1, 4)
        GiveItem(role, 0, 5253, 1, 4)
        GiveItem(role, 0, 5254, 1, 4)
        GiveItem(role, 0, 5255, 1, 4)
    else
        local EID = math.random(1808, 1811)
        GiveItem(role, 0, EID, 1, 4)
    end
end
function ItemUse_CJDQBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 10 then
        SystemNotice(role, "you need at least 10 free slot to open Gift of the Hunk")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    if cha_type == 3 or cha_type == 4 then
        SystemNotice(role, "Gift of the Hunk can only be opened by male characters")
        UseItemFailed(role)
        return
    end
    local el = math.random(1, 100)
    if el >= 1 and el <= 20 then
        GiveItem(role, 0, 3077, 1, 4)
    elseif el >= 21 and el <= 40 then
        GiveItem(role, 0, 3354, 1, 4)
        GiveItem(role, 0, 3355, 1, 4)
        GiveItem(role, 0, 3356, 1, 4)
        GiveItem(role, 0, 3357, 1, 4)
        GiveItem(role, 0, 3358, 1, 4)
        GiveItem(role, 0, 3359, 1, 4)
    elseif el >= 41 and el <= 45 then
        GiveItem(role, 0, 0936, 1, 4)
    elseif el >= 46 and el <= 65 then
        GiveItem(role, 0, 4264, 1, 4)
        GiveItem(role, 0, 4265, 1, 4)
        GiveItem(role, 0, 4266, 1, 4)
        GiveItem(role, 0, 4267, 1, 4)
        GiveItem(role, 0, 4268, 1, 4)
        GiveItem(role, 0, 4269, 1, 4)
        GiveItem(role, 0, 4270, 1, 4)
        GiveItem(role, 0, 4271, 1, 4)
        GiveItem(role, 0, 4272, 1, 4)
        GiveItem(role, 0, 4273, 1, 4)
    elseif el >= 66 and el <= 75 then
        GiveItem(role, 0, 3094, 3, 4)
    elseif el >= 76 and el <= 85 then
        GiveItem(role, 0, 855, 10, 4)
    elseif el >= 86 and el < 87 then
        GiveItem(role, 0, 0862, 1, 4)
    elseif el >= 87 and el < 89 and cha_type == 1 then
        GiveItem(role, 0, 5221, 1, 4)
        GiveItem(role, 0, 5222, 1, 4)
        GiveItem(role, 0, 5223, 1, 4)
    elseif el >= 89 and el <= 90 and cha_type == 2 then
        GiveItem(role, 0, 5238, 1, 4)
        GiveItem(role, 0, 5239, 1, 4)
        GiveItem(role, 0, 5240, 1, 4)
    else
        local EID = math.random(1808, 1811)
        GiveItem(role, 0, EID, 1, 4)
    end
end
function ItemUse_XTBOX(role, Item)
    local item_type = BoxXiang_BaoZhaBOX
    local item_type_rad = BoxXiang_BaoZhaBOX_Rad
    local item_type_count = BoxXiang_BaoZhaBOX_Count
    local maxitem = BoxXiang_baozhabao_Mxcount
    local item_quality = BoxXiang_baozhabao_Qua
    local General = 0
    local ItemId = 0
    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient inventory slots. Failed to use Wedding Candy")
        UseItemFailed(role)
        return
    end
    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
            break
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end

    GiveItem(role, 0, ItemId, ItemCount, item_quality)
    local GoodItem = {}
    GoodItem[0] = 3111
    GoodItem[1] = 3110
    GoodItem[2] = 3112
    GoodItem[3] = 3886
    GoodItem[4] = 3093
    GoodItem[5] = 3090
    GoodItem[6] = 430
    GoodItem[7] = 179
    GoodItem[8] = 3084
    GoodItem[9] = 3085
    GoodItem[10] = 0244
    GoodItem[11] = 0250
    GoodItem[12] = 0253
    GoodItem[13] = 0260
    GoodItem[14] = 0860
    GoodItem[15] = 0861
    GoodItem[16] = 0862
    GoodItem[17] = 3458
    GoodItem[18] = 0247
    GoodItem[19] = 0271
    local Good_C = 0
    for Good_C = 0, 19, 1 do
        if ItemId == GoodItem[Good_C] then
            local itemname = GetItemName(ItemId)
            local cha_name = GetChaDefaultName(role)
            local message = cha_name .. " uses a Wedding Candy and obtained " .. itemname
            Notice(message)
        end
    end
end
function ItemUse_HQBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 3 free slots to open Wedding Gift Parcel")
        UseItemFailed(role)
        return
    end
    local el = math.random(1, 100)
    if el >= 1 and el <= 20 then
        GiveItem(role, 0, 1012, 1, 4)
    elseif el >= 21 and el <= 40 then
        GiveItem(role, 0, 1016, 1, 4)
    elseif el >= 41 and el < 60 then
        GiveItem(role, 0, 0333, 1, 4)
    elseif el >= 61 and el <= 100 then
        GiveItem(role, 0, 0273, 1, 4)
    end
end
function ItemUse_XYPIGBOX(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 4 empty slots to open Lucky Piggy Chest")
        UseItemFailed(role)
        return
    end
    local el = math.random(1, 100)
    if el >= 1 and el <= 4 then
        GiveItem(role, 0, 1012, 1, 4)
    elseif el == 5 then
        GiveItem(role, 0, 1016, 1, 4)
    elseif el >= 6 and el <= 30 then
        GiveItem(role, 0, 0861, 1, 4)
    elseif el >= 31 and el <= 60 then
        GiveItem(role, 0, 885, 1, 4)
    elseif el >= 61 and el <= 75 then
        GiveItem(role, 0, 0860, 1, 4)
    elseif el >= 76 and el <= 88 then
        GiveItem(role, 0, 0862, 1, 4)
    elseif el >= 89 and el <= 100 then
        GiveItem(role, 0, 0863, 1, 4)
    end
end
function ItemUse_YSB(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 empty slots to open Auspicious Bag")
        UseItemFailed(role)
        return
    end
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    local star = 0
    if CheckDateNum >= 21723 and CheckDateNum <= 21801 then
        ItemUse_YSBOX(role, Item)
    else
        SystemNotice(role, "It is not time yet. Do not try to cheat!")
        UseItemFailed(role)
        return
    end
end
function ItemUse_MarryBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "you need at least 4 free slot to open Chest of Gown")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    if cha_type == 3 then
        GiveItem(role, 0, 5244, 1, 4)
        GiveItem(role, 0, 5245, 1, 4)
        GiveItem(role, 0, 5246, 1, 4)
        GiveItem(role, 0, 5247, 1, 4)
    elseif cha_type == 4 then
        GiveItem(role, 0, 5252, 1, 4)
        GiveItem(role, 0, 5253, 1, 4)
        GiveItem(role, 0, 5254, 1, 4)
        GiveItem(role, 0, 5255, 1, 4)
    elseif cha_type == 1 then
        GiveItem(role, 0, 5221, 1, 4)
        GiveItem(role, 0, 5222, 1, 4)
        GiveItem(role, 0, 5223, 1, 4)
    elseif cha_type == 2 then
        GiveItem(role, 0, 5238, 1, 4)
        GiveItem(role, 0, 5239, 1, 4)
        GiveItem(role, 0, 5240, 1, 4)
    end
end
function ItemUse_LoveBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 free slot to open Chest of Fate")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    local count = 0
    if cha_type == 3 or cha_type == 4 then
        count = 1
    end
    if cha_type == 1 or cha_type == 2 then
        count = 2
    end
    local el = math.random(1, 35)
    if count == 1 then
        if el == 35 then
            local r1 = 0
            local r2 = 0
            r1, r2 = MakeItem(role, 2902, 1, 4)
            local Item_girl = GetChaItem(role, 2, r2)
            local new_el = math.random(1, 200)
            SetItemAttr(Item_girl, ITEMATTR_VAL_STR, new_el)
        else
            local el1 = math.random(1, 12)
            if el1 == 1 then
                GiveItem(role, 0, 3343, 1, 4)
            elseif el1 == 2 then
                GiveItem(role, 0, 3077, 1, 4)
            else
                local EID = math.random(4264, 4273)
                GiveItem(role, 0, EID, 1, 4)
            end
        end
    end
    local el2 = math.random(1, 7)
    if count == 2 then
        if el2 == 7 then
            local r1 = 0
            local r2 = 0
            r1, r2 = MakeItem(role, 2903, 1, 4)
            local Item_boy = GetChaItem(role, 2, r2)
            local new_el = math.random(1, 1000)

            SetItemAttr(Item_boy, ITEMATTR_VAL_STR, new_el)
        else
            local el3 = math.random(1, 12)
            if el3 == 1 then
                GiveItem(role, 0, 3343, 1, 4)
            elseif el3 == 2 then
                GiveItem(role, 0, 3077, 1, 4)
            else
                local EID = math.random(4264, 4273)
                GiveItem(role, 0, EID, 1, 4)
            end
        end
    end
end
function ItemUse_DathBagA(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2846, 1, 16)
    else
        GiveItem(role, 0, 2928, 1, 16)
    end
end
function ItemUse_DathBagB(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2847, 1, 16)
    else
        GiveItem(role, 0, 2929, 1, 16)
    end
end
function ItemUse_DathBagC(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2848, 1, 16)
    else
        GiveItem(role, 0, 2927, 1, 16)
    end
end
function ItemUse_DathBagD(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2849, 1, 16)
    else
        GiveItem(role, 0, 2927, 1, 16)
    end
end
function ItemUse_DathBagE(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2850, 1, 16)
    else
        GiveItem(role, 0, 2929, 1, 16)
    end
end
function ItemUse_DathBagF(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2851, 1, 16)
    else
        GiveItem(role, 0, 2931, 1, 16)
    end
end
function ItemUse_DathBagG(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2852, 1, 16)
    else
        GiveItem(role, 0, 2932, 1, 16)
    end
end
function ItemUse_DathBagH(role, Item)
    local Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "you need at least 1 free slot to open Death's Burden")
        UseItemFailed(role)
        return
    end
    local star_rad = math.random(1, 4)
    if star_rad == 4 then
        GiveItem(role, 0, 2930, 1, 16)
    else
        GiveItem(role, 0, 2930, 1, 16)
    end
end
function ItemUse_ZSCard(role, Item)
    local i = CheckBagItem(role, 2941)
    local k = ChaIsBoat(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "To use a Rebirth Card requires at least 1 empty inventory slot")
        UseItemFailed(role)
        return
    end
    if k == 0 then
        if i > 0 then
            local j = DelBagItem(role, 2941, 1)
            if j == 1 then
                SystemNotice(
                    role,
                    "Use of Rebirth Card successful, you can now speak with the Reibrth angel to directly rebirth."
                )
                GiveItem(role, 0, 2235, 1, 42)
                GoTo(role, 1750, 909, "jialebi")
                return
            end
        end
    else
        UseItemsFailed(role)
    end
end
function ItemUse_FightingPoint(role, Item)
    local HonorBook_Num = 0
    local HonorBook_Num = CheckBagItem(role, 3849)
    if HonorBook_Num < 1 then
        SystemNotice(role, "You do not have Mark of Honor")
        return 0
    end
    local Book2 = GetChaItem2(role, 2, 3849)
    local FightingPoint = GetItemAttr(Book2, ITEMATTR_MAXENERGY)

    local FightingPoint_X = FightingPoint + 100
    SetItemAttr(Book2, ITEMATTR_MAXENERGY, FightingPoint_X)
end
function ItemUse_Foolish(role, Item)
    local charLv = Lv(role)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Exp_el = GetChaAttr(role, ATTR_CEXP)
    local dif_exp_thalf_c = (DEXP[charLv + 1] - DEXP[charLv]) * 0.03 + Exp_el
    SetChaAttrI(role, ATTR_CEXP, dif_exp_thalf_c)
end
function ItemUse_CZBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "To open a Aries Apparel Chest requires at least 4 empty inventory slots")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    if cha_type == 3 then
        GiveItem(role, 0, 5525, 1, 4)
        GiveItem(role, 0, 5526, 1, 4)
        GiveItem(role, 0, 5527, 1, 4)
    elseif cha_type == 4 then
        GiveItem(role, 0, 5525, 1, 4)
        GiveItem(role, 0, 5526, 1, 4)
        GiveItem(role, 0, 5527, 1, 4)
        GiveItem(role, 0, 5528, 1, 4)
    elseif cha_type == 1 then
        GiveItem(role, 0, 5525, 1, 4)
        GiveItem(role, 0, 5526, 1, 4)
        GiveItem(role, 0, 5527, 1, 4)
    elseif cha_type == 2 then
        GiveItem(role, 0, 5525, 1, 4)
        GiveItem(role, 0, 5526, 1, 4)
        GiveItem(role, 0, 5527, 1, 4)
    end
end
function Sk_Script_Wyz(role, Item)
    local sk_add = SK_WYZ
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_Bsj(role, Item)
    local sk_add = SK_BSJ
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_Emzz(role, Item)
    local sk_add = SK_EMZZ
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_Sssp(role, Item)
    local sk_add = SK_SSSP
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function Sk_Script_Cyn(role, Item)
    local sk_add = SK_CYN
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv >= 1 then
        UseItemFailed(role)
        return
    end
    local zs_exp = GetChaAttr(role, ATTR_CSAILEXP)
    if zs_exp <= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 0)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function ItemUse_moveDX(role, Item)
    local i = CheckBagItem(role, 2986)

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(Cha_Boat, "Not usable on the sea")
        UseItemFailed(Cha_Boat)
        return
    end
    local reg = 0
    reg = IsChaInRegion(role, 2)
    if reg == 1 then
        SystemNotice(role, "Unable to use in Safe Zone")
        UseItemFailed(role)
        return
    end

    if GetChaGuildID(role) == 0 then
        SystemNotice(role, "You do not have a guild. Unable to use the pass")
        UseItemFailed(role)
        return
    end

    local map_name_role = GetChaMapName(role)
    if map_name_role == "guildwar" then
        if GetChaGuildID(role) <= 100 and GetChaGuildID(role) > 0 and map_name_role == "guildwar" then
            if i > 0 then
                local j = DelBagItem(role, 2986, 1)
                if j == 1 then
                    MoveTo(role, 305, 87, "guildwar")
                    return
                end
            end
        elseif GetChaGuildID(role) > 100 and GetChaGuildID(role) <= 200 and map_name_role == "guildwar" then
            if i > 0 then
                local j = DelBagItem(role, 2986, 1)
                if j == 1 then
                    MoveTo(role, 309, 539, "guildwar")
                    return
                end
            end
        else
            UseItemFailed(role)
        end
    elseif map_name_role == "guildwar2" then
        if GetChaGuildID(role) <= 100 and GetChaGuildID(role) > 0 and map_name_role == "guildwar2" then
            if i > 0 then
                local j = DelBagItem(role, 2986, 1)
                if j == 1 then
                    MoveTo(role, 305, 87, "guildwar2")
                    return
                end
            end
        elseif GetChaGuildID(role) > 100 and GetChaGuildID(role) <= 200 and map_name_role == "guildwar2" then
            if i > 0 then
                local j = DelBagItem(role, 2986, 1)
                if j == 1 then
                    MoveTo(role, 309, 539, "guildwar2")
                    return
                end
            end
        else
            UseItemFailed(role)
        end
    else
        SystemNotice(role, "This ticket can only be used in Sacred War map")
        UseItemFailed(role)
        return
    end
end
function ItemUse_NiceCake(role, Item)
    local HonorBook_Num = 0
    local HonorBook_Num = CheckBagItem(role, 3849)
    if HonorBook_Num < 1 then
        SystemNotice(role, "You do not have Mark of Honor")
        UseItemFailed(role)
        return 0
    end
    local Book2 = GetChaItem2(role, 2, 3849)
    local HonorPoint = GetItemAttr(Book2, ITEMATTR_VAL_STR)
    local el_fame = GetChaAttr(role, ATTR_FAME)

    if HonorPoint < 27000 and el_fame < 99990001 then
        local HonorPoint_X = HonorPoint + 3000
        SetItemAttr(Book2, ITEMATTR_VAL_STR, HonorPoint_X)

        local fame_resume = 9999
        el_fame = el_fame + fame_resume
        SetCharaAttr(el_fame, role, ATTR_FAME)
    else
        SystemNotice(role, "Your Honor or Reputation points are too high. This cake can no longer satisfied you")
        UseItemFailed(role)
    end
end
function ItemUse_BYSHJZ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local star = 0
    star = IsChaInRegion(role, 2)
    if star == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 1009
    local Refresh = 3700
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_XingYunBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Inventory requires at least 1 empty slot")
        UseItemFailed(role)
        return
    end
    local el = math.random(1, 241920)
    if el == 1 then
        GiveItem(role, 0, 0961, 1, 4)
    elseif el == 2 then
        GiveItem(role, 0, 0969, 1, 4)
    elseif el >= 3 and el <= 4 then
        GiveItem(role, 0, 0973, 1, 4)
    elseif el >= 5 and el <= 6 then
        GiveItem(role, 0, 0980, 1, 4)
    elseif el >= 7 and el <= 8 then
        GiveItem(role, 0, 0979, 1, 4)
    else
        local el = math.random(0959, 0984)
        if el == 0961 or el == 0969 or el == 0973 or el == 0980 or el == 0979 then
            GiveItem(role, 0, 0959, 1, 4)
        else
            GiveItem(role, 0, el, 1, 4)
        end
    end
end
function ItemUse_XYZSTL(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local el = 0
    el = IsChaInRegion(role, 2)
    if el == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 786
    local Refresh = 9000
    local life = 9000000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_HDCZ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local el = 0
    el = IsChaInRegion(role, 2)
    if el == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 757
    local Refresh = 9000
    local life = 9000000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_HY(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local el = 0
    el = IsChaInRegion(role, 2)
    if el == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 761
    local Refresh = 9000
    local life = 9000000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_HLHT(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local el = 0
    el = IsChaInRegion(role, 2)
    if el == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 952
    local Refresh = 9000
    local life = 9000000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_JNCZBox(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 4 then
        SystemNotice(role, "You need at least 4 space to use the Taurus Apparel Chest")
        UseItemFailed(role)
        return
    end
    local cha_type = GetChaTypeID(role)
    if cha_type == 3 then
        GiveItem(role, 0, 5364, 1, 4)
        GiveItem(role, 0, 5365, 1, 4)
        GiveItem(role, 0, 5366, 1, 4)
        GiveItem(role, 0, 5367, 1, 4)
    elseif cha_type == 4 then
        GiveItem(role, 0, 5368, 1, 4)
        GiveItem(role, 0, 5369, 1, 4)
        GiveItem(role, 0, 5370, 1, 4)
        GiveItem(role, 0, 5371, 1, 4)
    elseif cha_type == 1 then
        GiveItem(role, 0, 5356, 1, 4)
        GiveItem(role, 0, 5357, 1, 4)
        GiveItem(role, 0, 5358, 1, 4)
        GiveItem(role, 0, 5359, 1, 4)
    elseif cha_type == 2 then
        GiveItem(role, 0, 5360, 1, 4)
        GiveItem(role, 0, 5361, 1, 4)
        GiveItem(role, 0, 5362, 1, 4)
        GiveItem(role, 0, 5363, 1, 4)
    end
end
function ItemUse_JNSHZ(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local el = 0
    el = IsChaInRegion(role, 2)
    if el == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local x, y = GetChaPos(role)
    local MonsterID = 1038
    local Refresh = 10900
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaLifeTime(new, life)
end
function ItemUse_WQZZ(role, Item)
    SystemNotice(role, "The item has gone rusty and has lost its effect, don't be too sad")
end
function ItemUse_DHZ(role, Item)
    SystemNotice(role, "The item has gone rusty and has lost its effect, don't be too sad")
end
function ItemUse_DSZ(role, Item)
    SystemNotice(role, "The item has gone rusty and has lost its effect, don't be too sad")
end
function ItemUse_NMZ(role, Item)
    SystemNotice(role, "The item has gone rusty and has lost its effect, don't be too sad")
end
function ItemUse_SanJiaoFan(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        AddState(Cha_Boat, Cha_Boat, STATE_YSBoatMspd, statelv, statetime)
    else
        SystemNotice(role, "Only can be used while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_LSDZG(role, Item)
	local statelv = 4
	local statetime = 60
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat(role)
	if Cha_Boat ==  nil then
		AddState(role, role, STATE_JLGLJB, statelv, statetime)
	else
		SystemNotice(role , "Cannot use while sailing")
		UseItemFailed(role)
		return
	end
end
function ItemUse_HSDZG(role, Item)
	local statelv = 2
	local statetime = 60
	local Cha_Boat = 0
	Cha_Boat = GetCtrlBoat(role)
	if Cha_Boat == nil then
		AddState(role, role, STATE_HCGLJB, statelv, statetime)
	else
		SystemNotice(role, "Cannot use while sailing")
		UseItemFailed(role)
		return
	end
end
function ItemUse_BZ(role, Item)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    local Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SuanmingWork(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        local lv = Lv(role)
        local exp = Exp(role)
        local clexp = GetChaAttrI(role, ATTR_CLEXP)
		
		if lv >= Server.Level.Limit then
			SystemNotice(role, "You have reached the maximum level. Unable to use lot.")
			UseItemFailed(role)
			return		
		end
		
		if exp <= clexp then
			SystemNotice(role, "Experience lower than level. Unable to use lot.")
			UseItemFailed(role)
			return
		end

        local Has_GoldenMap = CheckBagItem(role, 3336)
        if Has_GoldenMap >= 1 then
            SystemNotice(role, "Used Mystic Clover")
            SuanmingTeshu_Work(role)
            DelBagItem(role, 3336, 1)
        else
            Suanming_Work(role)
        end
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SuanmingMoney(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        local Has_GoldenMap = CheckBagItem(role, 3336)
        if Has_GoldenMap >= 1 then
            SystemNotice(role, "Used Mystic Clover")
            SuanmingTeshu_Money(role)
            DelBagItem(role, 3336, 1)
        else
            Suanming_Money(role)
        end
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function InventorySlot(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing!")
        UseItemFailed(role)
        return
    end
    local Inv = {}
    Inv[3088] = {Num = 24, Name = "28 Inventory Slot"}
    Inv[3089] = {Num = 28, Name = "32 Inventory Slot"}
    Inv[3090] = {Num = 32, Name = "36 Inventory Slot"}
    Inv[3091] = {Num = 36, Name = "40 Inventory Slot"}
    Inv[3092] = {Num = 40, Name = "44 Inventory Slot"}
    Inv[3093] = {Num = 44, Name = "48 Inventory Slot"}
    local ItemID = GetItemID(Item)
    if Inv[ItemID] ~= nil then
        local bagnum = GetKbCap(role)
        if bagnum ~= Inv[ItemID].Num then
            SystemNotice(role, "Cannot use item " .. Inv[ItemID].Name .. "!")
            UseItemFailed(role)
            return
        end
        AddKbCap(role, 4)
    else
        SystemNotice(role, "Internal error!")
        UseItemFailed(role)
        return
    end
end
function Book_SkillReset(role, Item)
	role = TurnToCha(role)       
	if ChaIsBoat(role) == 1 then
		SystemNotice(role, "Cannot use while sailing!")
		UseItemFailed(role)
		return
	end
	
	local SkillPoints = GetChaAttr(role, ATTR_TP)
	local ResetSkill = ClearAllFightSkill(role)
	local RebirthLv = GetRebirthLV(role)
	local Class = GetChaAttr(role, ATTR_JOB)
	local SkillID = 0
	if (RebirthLv > 0) then
		local MysticPower = SK_ZSSL   	-- Rebirth Mystic Power
		if (Class == 8) then           
				SkillID = SK_BSJ    	-- Beast Legion Smash
			elseif (Class == 9) then   
				SkillID = SK_WYZ    	-- Ethereal Slash
			elseif (Class == 12) then  
				SkillID = SK_HLP    	-- Red Thunder Cannon
			elseif (Class == 13) then  
				SkillID = SK_SSSP   	-- Holy Judgement
			elseif (Class == 14) then  
				SkillID = SK_EMZZ   	-- Devil Curse
			elseif (Class == 16) then  
				SkillID = SK_CYN    	-- Super consciousness
		end
		AddChaSkill(role, MysticPower, RebirthLv, RebirthLv, 0)
		AddChaSkill(role, SkillID, RebirthLv, RebirthLv, 0)
	end
	local RebirthSkillLV = GetSkillLv(role, SkillID)
	local MysticPowerLV = GetSkillLv(role, SK_ZSSL)
	SkillPoints = SkillPoints - (RebirthSkillLV + MysticPowerLV)
	SkillPoints = SkillPoints + ResetSkill
	SetChaAttr(role, ATTR_TP, SkillPoints)
	BickerNotice(role, "Succesfully reseted skills!")
	RefreshCha(role)
end
function Book_StatReset(role, Item)
	role = TurnToCha(role)
	if ChaIsBoat(role) == 1 then
		SystemNotice(role, "Cannot use while sailing!")
		UseItemFailed(role)
		return
	end
	local STR,CON,AGI,ACC,SPR = GetChaAttr(role, ATTR_BSTR),GetChaAttr(role, ATTR_BCON),GetChaAttr(role, ATTR_BAGI),GetChaAttr(role, ATTR_BDEX),GetChaAttr(role, ATTR_BSTA)
	if (STR < 5 or CON < 5 or AGI < 5 or ACC < 5 or SPR < 5) then
		SystemNotice(role, "You don\'t have enough points to reset stats.")
		UseItemFailed(role)
		return
	end
	local PlayerStr,PlayerCon,PlayerAgi,PlayerAcc,PlayerSpr = STR,CON,AGI,ACC,SPR
	local StatPoints = GetChaAttr(role, ATTR_AP)
	local PointDif = StatPoints
	STR,CON,AGI,ACC,SPR = STR-PlayerStr+5, CON-PlayerCon+5, AGI-PlayerAgi+5, ACC-PlayerAcc+5, SPR-PlayerSpr+5
	PointDif = PointDif - StatPoints + 5
	StatPoints = StatPoints + PlayerStr + PlayerCon + PlayerAgi + PlayerAcc + PlayerSpr - 25
	SetCharaAttr(StatPoints, role, ATTR_AP)
	SetCharaAttr(STR, role, ATTR_BSTR)
	SetCharaAttr(CON, role, ATTR_BCON)
	SetCharaAttr(AGI, role, ATTR_BAGI)
	SetCharaAttr(ACC, role, ATTR_BDEX)
	SetCharaAttr(SPR, role, ATTR_BSTA)
	BickerNotice(role, 'Succesfully reset stat point(s)!')
	RefreshCha(role)    
end
function ItemUse_SPXsyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSTZQH)
    State[3] = GetChaStateLv(role, STATE_YSJSQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSLLQH, statelv, statetime)
end
function ItemUse_SPLhyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSLQQH)
    State[1] = GetChaStateLv(role, STATE_YSTZQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSMJQH, statelv, statetime)
end
function ItemUse_SPMnyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSTZQH, statelv, statetime)
end
function ItemUse_SPYyyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSTZQH)
    State[2] = GetChaStateLv(role, STATE_YSJSQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSLQQH, statelv, statetime)
end
function ItemUse_SPSlyjA(role, Item)
    local OtherStateLv = 0
    local i = 0
    local State_Num = 3
    local State = {}
    State[0] = GetChaStateLv(role, STATE_YSMJQH)
    State[1] = GetChaStateLv(role, STATE_YSLQQH)
    State[2] = GetChaStateLv(role, STATE_YSTZQH)
    State[3] = GetChaStateLv(role, STATE_YSLLQH)

    for i = 0, State_Num, 1 do
        if State[i] >= 1 then
            OtherStateLv = OtherStateLv + 1
        end
    end

    if OtherStateLv > 0 then
        SystemNotice(role, "Potions effect for attribute bonus cannot be stacked")
        UseItemFailed(role)
        return
    end

    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local statetime = 5400
    AddState(role, role, STATE_YSJSQH, statelv, statetime)
end
function ItemUse_MspdYSA(role, Item)
    local statelv = 1
    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        AddState(role, role, STATE_YSMspd, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_GJZhuangJiaA(role, Item)
    local statelv = 1
    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        AddState(Cha_Boat, Cha_Boat, STATE_YSBoatDEF, statelv, statetime)
    else
        SystemNotice(role, "Only can be used while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_DenglongA(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat == nil then
        AddState(role, role, STATE_DENGLONG, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_Ant_Hzcr(role, Item, Item_Traget)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        RemoveState(Cha_Boat, STATE_HZCR)
    else
        SystemNotice(role, "Unable to use on the shore")
        UseItemFailed(role)
        return
    end
end
function Sk_Script_DS(role, Item)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        UseItemFailed(role)
        SystemNotice(role, "")
        return
    end
    local sk_add = SK_DS
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv ~= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 1)
    if a == 0 then
        UseItemFailed(role)
        return
    end

    local r1 = 0
    local r2 = 0
    r1, r2 = MakeItem(role, 3289, 1, 4)
    local Itemnew = GetChaItem(role, 2, r2)

    SetItemAttr(Itemnew, ITEMATTR_MAXENERGY, 120)
    SetItemAttr(Itemnew, ITEMATTR_ENERGY, 0)
    SetItemAttr(Itemnew, ITEMATTR_URE, 0)
    SetItemAttr(Itemnew, ITEMATTR_MAXURE, 150)
    SetItemAttr(Itemnew, ITEMATTR_FORGE, 0)

    LiveSkillLearnLog(role, 461)
end
function ItemUse_WisdomApple(role, Item)
    local Lv = Lv(role)
    if Lv < 40 then
        SystemNotice(role, "Only players Lv 40 and above may use")
        UseItemFailed(role)
        return
    end

    local statelv = 1
    local ChaStateLv = GetChaStateLv(role, STATE_APPLE)

    if ChaStateLv > statelv then
        SystemNotice(role, "Better fruit in effect. Please use it later")
        UseItemFailed(role)
        return
    end

    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_APPLE, statelv, statetime)
    else
        UseItemFailed(role)
        SystemNotice(role, "")
    end
end
function ItemUse_GoldApple(role, Item)
    local Lv = Lv(role)
    if Lv < 60 then
        SystemNotice(role, "Only Lv 60 and above may use")
        UseItemFailed(role)
        return
    end

    local statelv = 2
    local ChaStateLv = GetChaStateLv(role, STATE_APPLE)

    if ChaStateLv > statelv then
        SystemNotice(role, "Better fruit in effect. Please use it later")
        UseItemFailed(role)
        return
    end

    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_APPLE, statelv, statetime)
    else
        UseItemFailed(role)
        SystemNotice(role, "")
    end
end
function Sk_Script_Hx(role, Item)
    local sk_add = SK_HX
    local form_sklv = GetSkillLv(role, sk_add)

    if form_sklv ~= 0 then
        UseItemFailed(role)
        return
    end
    a = AddChaSkill(role, sk_add, 1, 1, 1)
    if a == 0 then
        UseItemFailed(role)
        return
    end
end
function AddPlayerSkill(role, Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
    local ItemID = GetItemID(Item)
	if Server.Player.Skill[ItemID] ~= nil then
		local Skill = Server.Player.Skill[ItemID].Skill
		if Skill == nil then
			SystemNotice(role, "Please contact administrator since "..GetItemName(GetItemID(Item)).." currently has no use.")
			UseItemFailed(role)
			return
		end
		local SkillLevel = GetSkillLv(role, Skill)
		if SkillLevel ~= 0 then
			SystemNotice(role, "You already learn the skill. You are not able to use the book again.")
			UseItemFailed(role)
			return
		end
		local MinimumLv = Server.Player.Skill[ItemID].MinLevel
		local MaximumLv = Server.Player.Skill[ItemID].MaxLevel
		if MinimumLv ~= nil or MaximumLv ~= nil then
			if Lv(role) < MinimumLv or Lv(role) > MaximumLv then
				SystemNotice(role, "You didn't meet the level requirement to learn this skill.")
				UseItemFailed(role)
				return		
			end
		end
		local AddSkill = AddChaSkill(role, Skill, 1, 1, 1)
		if AddSkill == 0 then
			SystemNotice(role, "Something went wrong! Please report to administrator.")
			UseItemFailed(role)
			return
		end
	end
end
function AddFairySkill(role, Item, Fairy)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
	local ItemID = GetItemID(Item)
	if Server.Fairy.Skill[ItemID] ~= nil then
		local Fairy = GetEquipItemP(role, 16)
		if Fairy == nil then
			SystemNotice(role, "Your fairy slot is empty!")
			UseItemFailed(role)
			return
		end
		local FairyID = GetItemID(Fairy)
		local Check = CanLevelFairySkill(role, Fairy, Item, Server.Fairy.Skill[ItemID].Skill, Server.Fairy.Skill[ItemID].Level)
		if GetItemType(Item) == 58 and GetItemType(Fairy) == 59 then
			if not Check then
				UseItemFailed(role)
				return
			else
				AddElfSkill(role, Fairy, Item, Server.Fairy.Skill[ItemID].Skill, Server.Fairy.Skill[ItemID].Level)
				PlayEffect(role, 345)
				SynChaKitbag(role, 13)
				RefreshCha(role)
				LG("FairySkills","" ..GetChaDefaultName(role).." used "..GetItemName(ItemID).." for his/her "..GetItemName(FairyID)..".")
			end
			SynLook(role)
		else
			UseItemFailed(role)
			return
		end
    end
end
function AddLifeSkill(role, Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
	local ItemID = GetItemID(Item)
	if Server.Life.Skill[ItemID] ~= nil then
		if GetSkillLv(role, Server.Life.Skill[ItemID].Skill) ~= Server.Life.Skill[ItemID].Level then
			SystemNotice(role, "Prerequisite skill level didn't complete!")
			UseItemFailed(role)
			return
		end
		if Server.Life.Skill[ItemID].Prerequisite.Skill ~= nil then
			if GetSkillLv(role, Server.Life.Skill[ItemID].Prerequisite.Skill) < Server.Life.Skill[ItemID].Prerequisite.Level then
				SystemNotice(role, "Requires you to learn Lv"..Server.Life.Skill[ItemID].Prerequisite.Level.." "..GetSkillName(Server.Life.Skill[ItemID].Prerequisite.Skill).." skill to read this book")
				UseItemFailed(role)
				return
			end
		end
		if GetChaAttr(role, ATTR_LIFETP) < 1 then
			SystemNotice(role, "Not enough life skill points!")
			UseItemFailed(role)
			return
		end
		local AddSkill = AddChaSkill(role, Server.Life.Skill[ItemID].Skill, Server.Life.Skill[ItemID].AddLevel, 1, 1) 
		if AddSkill == 0 then 
			SystemNotice(role, "Internal error!") 
			UseItemFailed(role)  
			return 
		end
		if Server.Life.Skill[ItemID].Give ~= nil then
			local R1,R2 = -1,-1
			R1,R2 = MakeItem(role, Server.Life.Skill[ItemID].Give, 1, 4)
			local Itemfinal = GetChaItem(role, 2, R2)
			SetItemAttr(Itemfinal, ITEMATTR_VAL_STR, 1)
			SetItemAttr(Itemfinal, ITEMATTR_MAXENERGY, 10000)
			SetItemAttr(Itemfinal, ITEMATTR_ENERGY, 1)
		end
	end
end
function AddFairyBody(role,Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
	local ItemID = GetItemID(Item)
	if Server.Fairy.Possession.Skill[ItemID] ~= nil then
		if GetSkillLv(role, Server.Fairy.Possession.Skill[ItemID].Skill) >= Server.Fairy.Possession.Skill[ItemID].Level then
			SystemNotice(role, "You cannot learn the same or better skill.")
			UseItemFailed(role)
			return
		end
		local AddSkill = AddChaSkill(role, Server.Fairy.Possession.Skill[ItemID].Skill, Server.Fairy.Possession.Skill[ItemID].Level, 1, 0) 
		if AddSkill == 0 then 
			SystemNotice(role, "Internal error!")
			UseItemFailed(role)  
			return 
		end
	end
end
function AddFunnySkill(role,Item)
	if KitbagLock(role, 0) == LUA_FALSE then
		PopupNotice(role, "Inventory is locked.")
		return
	end
    if IsChaStall(role) == LUA_TRUE then
        SystemNotice(role, "Cannot use while your stall is opened!")
        UseItemFailed(role)
        return
    end
	if GetCtrlBoat(role) ~= nil then 
		SystemNotice(role, "Cannot use while sailing!") 
		UseItemFailed(role) 
		return 
	end
	local ItemID = GetItemID(Item)
	if Server.Fairy.Funny.Skill[ItemID] ~= nil then
		if GetSkillLv(role, Server.Fairy.Funny.Skill[ItemID].Skill) >= Server.Fairy.Funny.Skill[ItemID].Level then
			UseItemFailed(role)
			return
		end
		local AddSkill = AddChaSkill(role, Server.Fairy.Funny.Skill[ItemID].Skill, Server.Fairy.Funny.Skill[ItemID].Level, 1, 0) 
		if AddSkill == 0 then 
			SystemNotice(role, "Internal error!")
			UseItemFailed(role)  
			return 
		end
	end
end
function BlackMarket(role, Item)
	local Market = {}
	Market[0088] = BaoXiang_jsmzcqa
	Market[0089] = BaoXiang_jsmzcqb
	Market[3302] = BaoXiang_jsyla
	Market[3303] = BaoXiang_jsylb
	Market[3304] = BaoXiang_jsmzlra
	Market[3305] = BaoXiang_jsmzlrb
	Market[3306] = BaoXiang_jsjqa
	Market[3307] = BaoXiang_jsjqb
	Market[3308] = BaoXiang_jsmzcja
	Market[3309] = BaoXiang_jsmzcja
	Market[3310] = BaoXiang_jssjkja
	Market[3311] = BaoXiang_jssjkjb
	Market[3312] = BaoXiang_jszjkja
	Market[3313] = BaoXiang_jszjkjb
	Market[3314] = BaoXiang_jsszkja
	Market[3315] = BaoXiang_jsszkjb
	Market[3316] = BaoXiang_jsfykja
	Market[3317] = BaoXiang_jsfykjb
	Market[3318] = BaoXiang_jshhkja
	Market[3319] = BaoXiang_jshhkjb
	Market[3320] = BaoXiang_jsjjkja
	Market[3321] = BaoXiang_jsjjkjb
	Market[3322] = BaoXiang_jshlza
	Market[3323] = BaoXiang_jshlzb
	Market[3324] = BaoXiang_jshlta
	Market[3325] = BaoXiang_jshlsa
	Market[3326] = BaoXiang_jshlsb
	Market[3327] = BaoXiang_jshlya
	Market[3328] = BaoXiang_jshlyb
	Market[3329] = BaoXiang_jsmhzca
	Market[3330] = BaoXiang_jsmhzcb
	Market[3331] = BaoXiang_jsmzfza
	Market[3332] = BaoXiang_jsmzfzb
	Market[3333] = BaoXiang_jsmfzza
	Market[3334] = BaoXiang_jsmfzzb
	local ItemID = GetItemID(Item)
	local General = 0
	if Market[ItemID] ~= nil then
		for i = 1 , #Market[ItemID], 1 do
			if Market[ItemID][i].Active == 1 then
				General = Market[ItemID][i].Rad + General
			end
		end
		local a = math.random(1, General)
		local b = 0
		local d = 0 
		local c = -1
		for k = 1 , #Market[ItemID], 1 do
			if Market[ItemID][k].Active == 1 then
				d = Market[ItemID][k].Rad + b
				if a <= d and a > b then
					c = k
					break
				end 
				b = d
			end
		end
		if c ~= -1 then
			local Item2Give		= Market[ItemID][c].ItemID 
			local ItemCount		= Market[ItemID][c].Quantity
			local ItemQuality	= Market[ItemID][c].Quality
			local GoodItem		= Market[ItemID][c].GoodItem
			GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
			if GoodItem == 1 then
				local message = ""..GetChaDefaultName(role).." opens a "..GetItemName(ItemID).." and obtains "..GetItemName(Item2Give)..""
				Notice(message)
			end
		end
	end
end
function ItemUse_MarchElf(role, Item)
    local statelv = 1
    local statetime = 600
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        local cha_type = GetChaTypeID(role)
        if cha_type == 3 or cha_type == 4 then
            AddState(role, role, STATE_MarchElf, statelv, statetime)
        else
            SystemNotice(role, "Fairy March can only be used by female")
            UseItemFailed(role)
            return
        end
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_BirthCake(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        local attr_ap = Attr_ap(role)
        local ap_extre = 3
        attr_ap = attr_ap + ap_extre
        SetCharaAttr(attr_ap, role, ATTR_AP)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_MspdYS(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_YSMspd, statelv, statetime)
        SystemNotice(role, "A")
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SanJiaoFan(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        AddState(Cha_Boat, Cha_Boat, STATE_YSBoatMspd, statelv, statetime)
    else
        SystemNotice(role, "Only can be used while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_GJZhuangJia(role, Item)
    local statelv = 1
    local statetime = 900
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        AddState(Cha_Boat, Cha_Boat, STATE_YSBoatDEF, statelv, statetime)
    else
        SystemNotice(role, "Only can be used while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_Denglong(role, Item)
    local statelv = 1
    local statetime = 600
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_DENGLONG, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_Denglong(role, Item)
    local statelv = 1
    local statetime = 600
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_DENGLONG, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_MeiGui(role, Item)
    local statelv = 1
    local statetime = 600
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_MEIGUI, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_BZ(role, Item)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    local Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_HYBOX(role, Item)
	local Item_CanGet = GetChaFreeBagGridNum(role)
	if Item_CanGet <= 0 then
		SystemNotice(role, "Insufficient space in inventory!")
		UseItemFailed(role)
		return
	end
	local vItem = {}
	vItem[3901] = BaoXiang_HYBOX
	local ItemID = GetItemID(Item)
	local General = 0
	if vItem[ItemID] ~= nil then
		local Money_Add = 8
		local exp_dif = 0
		local exp_random = math.random(1,1000)
		if exp_random == 1000 then
			Money_Add = 8888
			exp_dif = 66600
		else
			exp_dif = math.random(2,6)
			exp_dif = exp_dif * 50
		end
		local pexp = Exp(role)
		if Lv(TurnToCha(role) ) >= 80 then
			exp_dif = math.floor(exp_dif/50)
		end
		local exp_new = pexp + exp_dif
		AddMoney(role, 0, Money_Add)
		SetCharaAttr(exp_new, role, ATTR_CEXP)
		local Lucky = math.random(1, 500)
		if  Lucky == 500 then
			for i = 1 , #vItem[ItemID], 1 do
				if vItem[ItemID][i].Active == 1 then
					General = vItem[ItemID][i].Rad + General
				end
			end
			local a = math.random(1, General)
			local b = 0
			local d = 0 
			local c = -1
			for k = 1 , #vItem[ItemID], 1 do
				if vItem[ItemID][k].Active == 1 then
					d = vItem[ItemID][k].Rad + b
					if a <= d and a > b then
						c = k
						break
					end 
					b = d
				end
			end
			if c ~= -1 then
				local Item2Give		= vItem[ItemID][c].ItemID 
				local ItemCount		= vItem[ItemID][c].Quantity
				local ItemQuality	= vItem[ItemID][c].Quality
				local GoodItem		= vItem[ItemID][c].GoodItem
				GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
				if GoodItem == 1 then
					local message = ""..GetChaDefaultName(role).." opens a "..GetItemName(ItemID).." and obtains "..GetItemName(Item2Give)..""
					Notice(message)
				end
			end
		end
		GiveItem(role, 0, 3904, 1, 5)
	end
end
function GiveRandomItems(role,Item)
	local Item_CanGet = GetChaFreeBagGridNum(role)
	if Item_CanGet <= 0 then
		SystemNotice(role, "Insufficient space in inventory!")
		UseItemFailed(role)
		return
	end
	local rItem = {}
	rItem[3905] = BaoXiang_ADBOX		-- Dark Wishing Stone
	rItem[3906] = BaoXiang_SGBOX		-- Sparkling Wishing Stone
	rItem[1094] = BoxXiang_YiYuanBOX	-- Lucky Give Parcel
	rItem[1815] = BaoXiang_HLBX			-- Beautiful Chest
	rItem[1814] = BaoXiang_SMBX			-- Mystic Chest
	rItem[1852] = BaoXiang_SYBOX		-- Whammy Chest
	rItem[2442] = BaoXiang_SYBOX		-- Sunken Cupboard
	rItem[2443] = BaoXiang_SYBOX		-- Sunken Cupboard
	rItem[1851] = BaoXiang_WZX			-- Daily Supply (Rewards for Daily Quest)
	rItem[3400] = BaoXiang_KLJS			-- Skeletar Chest of Swordsman
	rItem[3401] = BaoXiang_KLLR			-- Skeletar Chest of Hunter	
	rItem[3402] = BaoXiang_KLYS			-- Skeletar Chest of Herbalist	
	rItem[3403] = BaoXiang_KLMX			-- Skeletar Chest of Explorer	
	rItem[3404] = BaoXiang_ZSSJ			-- Incantation Chest of Crusader
	rItem[3405] = BaoXiang_ZSJS			-- Incantation Chest of Champion
	rItem[3406] = BaoXiang_ZSJJ			-- Incantation Chest of Sharpshooter
	rItem[3407] = BaoXiang_ZSSZ			-- Incantation Chest of Cleric
	rItem[3408] = BaoXiang_ZSFY			-- Incantation Chest of Seal Master
	rItem[3409] = BaoXiang_ZSHH			-- Incantation Chest of Voyager
	rItem[3410] = BaoXiang_HLSJ			-- Evanescence Chest of Crusader
	rItem[3411] = BaoXiang_HLJS			-- Evanescence Chest of Champion
	rItem[3412] = BaoXiang_HLJJ			-- Evanescence Chest of Sharpshooter
	rItem[3413] = BaoXiang_HLSZ			-- Evanescence Chest of Cleric
	rItem[3414] = BaoXiang_HLFY			-- Evanescence Chest of Seal Master
	rItem[3415] = BaoXiang_HLHH			-- Evanescence Chest of Voyager
	rItem[3416] = BaoXiang_MSJ			-- Enigma Chest of Crusader
	rItem[3417] = BaoXiang_MJS			-- Enigma Chest of Champion
	rItem[3418] = BaoXiang_MJJ			-- Enigma Chest of Sharpshooter
	rItem[3419] = BaoXiang_MSZ			-- Enigma Chest of Cleric
	rItem[3420] = BaoXiang_MFY			-- Enigma Chest of Seal Master
	rItem[3421] = BaoXiang_MHH			-- Enigma Chest of Voyager
	rItem[3422] = BaoXiang_ZZBX			-- Chest of Forsaken City
	rItem[3423] = BaoXiang_MFBX			-- Chest of Dark Swamp 
	rItem[3424] = BaoXiang_MZBX			-- Chest of Demonic World
	rItem[3458] = BaoXiang_MZBX2		-- Chest of Enigma
	rItem[3895] = BaoXiang_SDWZBOX		-- Christmas Sock
	rItem[3898] = BaoXiang_SDLHBOX		-- A gift box
	rItem[3902] = BaoXiang_FGBOX		-- Fortune Packet
	rItem[3903] = BaoXiang_HYUNBOX		-- Prosperous Packet
	rItem[1095] = BoxXiang_BaoZhaBOX	-- 99 Parcel
	rItem[2893] = BoxXiang_BaoZhaBOX	-- Exquisite Christmas case
	rItem[2906] = BoxXiang_BaoZhaBOX	-- Wedding Candy
	rItem[1096] = BoxXiang_ZhousSuiBOX	-- Anniversary Chest
	rItem[1119] = BaoXiang_98BOX		-- Paradise Pouch
	rItem[1103] = BaoXiang_BenteBOX		-- Lv20 Unique Ring Voucher
	rItem[1104] = BaoXiang_TrentaBOX	-- Lv30 Unique Ring Voucher
	rItem[1105] = BaoXiang_QuarentaBOX	-- Lv40 Unique Ring Voucher
	rItem[1106] = BaoXiang_LimaBOX		-- Lv50 Unique Ring Voucher
	rItem[1107] = BaoXiang_AnimBOX		-- Lv60 Unique Ring Voucher
	rItem[1108] = BaoXiang_Quintas2BOX	-- Lv20 Unique Necklace Voucher
	rItem[1109] = BaoXiang_Quintas3BOX	-- Lv30 Unique Necklace Voucher
	rItem[1110] = BaoXiang_Quintas4BOX	-- Lv40 Unique Necklace Voucher
	rItem[1111] = BaoXiang_Quintas5BOX	-- Lv50 Unique Necklace Voucher
	rItem[1112] = BaoXiang_Quintas6BOX	-- Lv60 Unique Necklace Voucher
	rItem[0582] = BaoXiang_DeCoralBOX	-- Unique Coral Voucher
	rItem[9600] = BaoXiang_MysteryBOX	-- Mystery Box
	
	local ItemID = GetItemID(Item)
	local General = 0
	if rItem[ItemID] ~= nil then
		for i = 1 , #rItem[ItemID], 1 do
			if rItem[ItemID][i].Active == 1 then
				General = rItem[ItemID][i].Rad + General
			end
		end
		local a = math.random(1, General)
		local b = 0
		local d = 0 
		local c = -1
		for k = 1 , #rItem[ItemID], 1 do
			if rItem[ItemID][k].Active == 1 then
				d = rItem[ItemID][k].Rad + b
				if a <= d and a > b then
					c = k
					break
				end 
				b = d
			end
		end
		if c ~= -1 then
			local Item2Give		= rItem[ItemID][c].ItemID 
			local ItemCount		= rItem[ItemID][c].Quantity
			local ItemQuality	= rItem[ItemID][c].Quality
			local GoodItem		= rItem[ItemID][c].GoodItem
			GiveItem(role, 0, Item2Give, ItemCount, ItemQuality)
			local Add = {}
			Add[3902] = {Item = 3904, Quantity = 1, Quality = 5}
			Add[3903] = {Item = 3904, Quantity = 1, Quality = 5}
			if Add[ItemID] ~= nil then
				GiveItem(role, 0, Add[ItemID].Item, Add[ItemID].Quantity, Add[ItemID].Quality)
			end
			if GoodItem == 1 then
				local message = ""..GetChaDefaultName(role).." opens a "..GetItemName(ItemID).." and obtains "..GetItemName(Item2Give)..""
				Notice(message)
			end
		end
	end
end
function ItemUse_BOMB(role, Item)
    local bomb = SummonCha(role, 1, 704)
    local statetime = 10
    local statelv = 1
    AddState(role, bomb, STATE_BOMB, statelv, statetime)
    AddChaSkill(bomb, SK_BOMB, 1, 1, 0)
    SystemNotice(role, "mount Water Mine successful")
end
function ItemUse_DSSDM(role, Item)
    local statelv = 1
    local statetime = 300
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_PKSFYS, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SDWZBOX(role, Item)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum

    if CheckDateNum < 122423 then
        SystemNotice(
            role,
            "Please be patient. Santa Claus will be very punctual with his gift. Please use the item between 24th December 11.00pm and 25th December 6.00am"
        )
        UseItemFailed(role)
        return
    end

    if CheckDateNum > 122506 then
        SystemNotice(role, "Santa Claus has left. Please wait for next year")
        UseItemFailed(role)
        return
    end

    local item_type = BaoXiang_SDWZBOX
    local item_type_rad = BaoXiang_SDWZBOX_Rad
    local item_type_count = BaoXiang_SDWZBOX_Count
    local maxitem = BaoXiang_SDWZBOX_Mxcount
    local item_quality = BaoXiang_SDWZBOX_Qua
    local General = 0
    local ItemId = 0

    local Item_CanGet = GetChaFreeBagGridNum(role)

    if Item_CanGet <= 0 then
        SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
        UseItemFailed(role)
        return
    end
    for i = 1, maxitem, 1 do
        General = item_type_rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = 1, maxitem, 1 do
        d = item_type_rad[k] + b

        if a <= d and a > b then
            c = k
        end
        b = d
    end
    if c == -1 then
        ItemId = 3124
    else
        ItemId = item_type[c]
        ItemCount = item_type_count[c]
    end
    GiveItem(role, 0, ItemId, ItemCount, item_quality)
end
function ItemUse_XEZJ(role, Item)
    local statelv = 1
    local statetime = 1800
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        AddState(role, role, STATE_PKZDYS, statelv, statetime)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_SDLHBOX(role, Item)
    local Check = math.random(1, 100)
    if Check <= 35 then
        local exp = Exp(role)
        local exp_dif = math.random(100, 300)
        if Lv(TurnToCha(role)) >= 80 then
            exp_dif = math.floor(exp_dif / 50)
        end
        local exp_new = exp + exp_dif

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    elseif Check > 35 and Check <= 70 then
        local Money_add = math.random(200, 600)
        AddMoney(role, 0, Money_add)
    else
        local item_type = BaoXiang_SDLHBOX
        local item_type_rad = BaoXiang_SDLHBOX_Rad
        local item_type_count = BaoXiang_SDLHBOX_Count
        local maxitem = BaoXiang_SDLHBOX_Mxcount
        local item_quality = BaoXiang_SDLHBOX_Qua
        local General = 0
        local ItemId = 0

        local Item_CanGet = GetChaFreeBagGridNum(role)

        if Item_CanGet <= 0 then
            SystemNotice(role, "Insufficient space in inventory. Unable to open chest")
            UseItemFailed(role)
            return
        end
        for i = 1, maxitem, 1 do
            General = item_type_rad[i] + General
        end
        local a = math.random(1, General)
        local b = 0
        local d = 0
        local c = -1
        for k = 1, maxitem, 1 do
            d = item_type_rad[k] + b

            if a <= d and a > b then
                c = k
            end
            b = d
        end
        if c == -1 then
            ItemId = 3124
        else
            ItemId = item_type[c]
            ItemCount = item_type_count[c]
        end
        GiveItem(role, 0, ItemId, ItemCount, item_quality)
    end
end
function ItemUse_YB(role, Item)
    local exp = Exp(role)
    local charLv = Lv(role)
    local exp_new = 0
    if charLv >= 80 then
        exp_new = exp + 6
    else
        exp_new = exp + 300
    end
    SetCharaAttr(exp_new, role, ATTR_CEXP)
end
function ItemUse_HuFuSW(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        RemoveState(role, STATE_JSDD)
        AddState(role, role, STATE_TTISW, 4, 1200)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_HuoRongSW(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        RemoveState(role, STATE_BDJ)
        AddState(role, role, STATE_TTISW, 1, 300)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_ZhouGUSW(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        RemoveState(role, STATE_ZZZH)
        AddState(role, role, STATE_TTISW, 2, 300)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_ShuiMangSW(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)

    if Cha_Boat == nil then
        RemoveState(role, STATE_CRXSF)
        AddState(role, role, STATE_TTISW, 3, 300)
    else
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
end
function ItemUse_Wood(role, Item)
    local k = ChaIsBoat(role)
    if k == 0 then
        UseItemFailed(role)
        SystemNotice(role, "Item can only be used while sailing")
        return
    end
    local hp = GetChaAttr(role, ATTR_HP)
    local mxhp = GetChaAttr(role, ATTR_MXHP)
    if hp < 0 then
        UseItemFailed(role)
        SystemNotice(role, "Ship has sunken. Unable to use item")
    end
    local statelv = 1
    local statetime = 63
    AddState(role, role, STATE_MCK, statelv, statetime)
end
function ItemUse_Fish(role, Item)
    local k = ChaIsBoat(role)
    if k == 0 then
        UseItemFailed(role)
        SystemNotice(role, "Item can only be used while sailing")
        return
    end
    local sp = GetChaAttr(role, ATTR_SP)
    local mxsp = GetChaAttr(role, ATTR_MXSP)
    local hp = GetChaAttr(role, ATTR_HP)
    local mxhp = GetChaAttr(role, ATTR_MXHP)
    if hp < 0 then
        UseItemFailed(role)
        SystemNotice(role, "Ship has sunken. Unable to use item")
    end
    local sp_resume = 50
    sp = math.min(mxsp, sp + sp_resume)
    SetCharaAttr(sp, role, ATTR_SP)
end
function ItemUse_Ant_Hzcr(role, Item, Item_Traget)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        RemoveState(Cha_Boat, STATE_HZCR)
    else
        SystemNotice(role, "Unable to use on the shore")
        UseItemFailed(role)
        return
    end
end
function FairyFruitImprvd(role, Item, Fairy)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use fairy fruits while using boat.")
        UseItemFailed(role)
        return
    end
    local Fairy = GetEquipItemP(role, 16)
    if Fairy == nil then
        SystemNotice(role, "Your fairy slot is empty!")
        UseItemFailed(role)
        return
    end

    local FairyID = GetItemID(Fairy)
    local LvCheck = FairyLevel_Imprvd(role, Fairy)
    if LvCheck == 0 then
        if FairyID ~= 7125 and FairyID ~= 7126 then
            SystemNotice(role, "Cannot use this fruit on this fairy yet.")
            UseItemFailed(role)
            return
        end
    end
    if GetFairyLevel(Fairy) >= Server.Fairy.Level.Maximum then
        SystemNotice(role, "Reached server fairy maximum level, cannot raise any more.")
        UseItemFailed(role)
        return
    end
    local ItemType = GetItemType(Item)
    local FairyType = GetItemType(Fairy)
    local ItemID = GetItemID(Item)
    local Check_Exp = 0
    if ItemType == 58 and FairyType == 59 then
        Check_Exp = CheckFairyEXP(role, Fairy)
        if Check_Exp == 0 then
            SystemNotice(role, "Fairy hasn't reached maximum growth, cannot level up.")
            UseItemFailed(role)
        else
            FairyLevel(role, Fairy, Server.Fairy.Fruit[ItemID].ATTR, Server.Fairy.Fruit[ItemID].LV, 0)
        end
    end
end
function GemVouchers(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing!")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Requires 1 slot on inventory!")
        UseItemFailed(role)
        return
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    local FreeBagNeed = 1
    if Item_CanGet < FreeBagNeed then
        SystemNotice(role, "To open a " .. GetItemName(ItemID) .. " requires " .. FreeBagNeed .. " empty slots")
        UseItemFailed(role)
        return
    end
    local VoucherVar = {}
    VoucherVar[9612] = {6832, 6833, 6834}
    VoucherVar[9613] = {6817, 6820, 6823, 6826, 6829}
    VoucherVar[9614] = {6818, 6821, 6824, 6827, 6830}
    VoucherVar[9615] = {6836, 6837, 6838, 6839, 6840}
    VoucherVar[9616] = {0860, 0861, 0862, 0863, 1012}
    VoucherVar[9617] = {0864, 0865, 0866}

    local ItemID = GetItemID(Item)
    if VoucherVar[ItemID] ~= nil then
        local Key = math.random(1, #VoucherVar[ItemID])
        GiveItem(role, 0, VoucherVar[ItemID][Key], 1, 4)
        local ChaName = GetChaDefaultName(role)
        Notice(ChaName .. " opened " .. GetItemName(ItemID) .. " and obtained " .. GetItemName(VoucherVar[ItemID][Key]))
    else
        SystemNotice(role, "Internal error!")
        UseItemFailed(role)
        return
    end
end
function SummonAries(role, Item)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(role)
    if Cha_Boat ~= nil then
        SystemNotice(role, "Cannot use while sailing")
        UseItemFailed(role)
        return
    end
    local Area = 0
    Area = IsChaInRegion(role, 2)
    if Area == 1 then
        SystemNotice(role, "Unable to summon monster in Safe Zone")
        UseItemFailed(role)
        return
    end
    local PosX, PosY = GetChaPos(role)
    local MonsterID = 1009
    local Refresh = 3600
    local Life = 3600000
    local Monster = CreateChaX(MonsterID, PosX, PosY, 145, Refresh, role)
    SetChaLifeTime(Monster, Life)
end
function ApparelConverter(role,item,target)
	if not target or GetItemAttr(target,ITEMATTR_MAXURE) == 25000 then
		return UseItemFailed(role)
	end
	local itemType = GetItemType(target)
	if Apparel_CanConvert_ID[itemType] then
		local ID = GetItemID(target)
		for i = 0,47 do
			local item2 = GetChaItem(role,2,i)
			if item2 == target then
				RemoveChaItem ( role , GetItemID(target) , 1 , 2 , i , 2 , 1 , 0)
				break
			end
		end
		local appTab = {
			ID = Apparel_CanConvert_ID[itemType],
			Quantity = 1,
			MaxDurability = 25000,
			Durability = 25000,
			FuseID = ID,
		}
		AddItem(role,appTab)
	else
		SystemNotice(role,"Invalid equipment.")
		return UseItemFailed(role)
	end
end
function ItemUse_PartyFruit(role, Item)
	if IsInTeam(role) == 1 then
		local StateLevel = 1
		local StateTime = 900	
		local ChaStateLv = GetChaStateLv(role, STATE_ZDSBJYGZ)
		if ChaStateLv >= StateLevel then
			BickerNotice(role, "Better or same fruit in effect. Please use it later")
			UseItemFailed(role)
			return
		end	
		local Team = {role, GetTeamCha(role, 0), GetTeamCha(role, 1), GetTeamCha(role, 2), GetTeamCha(role, 3)}	
		for i = 1, #Team do
			local Boat = 0
			Boat = GetCtrlBoat(Team[i])	
			if Boat == nil then
				AddState(Team[i], Team[i], STATE_ZDSBJYGZ, StateLevel, StateTime)
				SystemNotice(Team[i], GetChaDefaultName(role).." has consumed "..GetItemName(GetItemID(Item))..". Received 1.5x Team Experience bonus for 15 mins.")
			else
				AddState(Boat, Boat, STATE_ZDSBJYGZ, StateLevel, StateTime)
				SystemNotice(Boat, GetChaDefaultName(role).." has consumed "..GetItemName(GetItemID(Item))..". Received 1.5x Team Experience bonus for 15 mins.")
			end
		end
	else
		BickerNotice(role, "You must be in a team!")
		return UseItemFailed(role)		
	end
end
function Corsairs_WangChest(role, Item)
	local Level = Lv(role)
	local Chance = math.random(1,6)
	local Name = GetChaDefaultName(role)
	if Level >= 50 and Level <= 55 then
		if Chance == 1 then
			GiveItem(role, 0, 7294, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Champion's Belt]", 1)
			return
		elseif Chance == 2 then
			GiveItem(role, 0, 7297, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Crusader's Belt]", 1)
			return
		elseif Chance == 3 then
			GiveItem(role, 0, 7300 ,1 , 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Cleric's Belt]", 1)
			return
		elseif Chance == 4 then
			GiveItem(role, 0, 7303, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Seal Master's Belt]", 1)
			return
		elseif Chance == 5 then
			GiveItem(role, 0, 7306, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Voyager's Belt]", 1)
			return
		elseif Chance == 6 then
			GiveItem(role, 0, 7309, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv55 Sharpshooter's Belt]", 1)
			return
		end
	end
	if Level >= 60 then
		if Chance == 1 then -- Lv65 Belt
			GiveItem(role, 0, 7312, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Champion's Belt]", 1)
			return
		elseif Chance == 2 then
			GiveItem(role, 0, 7315, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Crusader's Belt]", 1)
			return
		elseif Chance == 3 then
			GiveItem(role, 0, 7318, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Cleric's Belt]", 1)
			return
		elseif Chance == 4 then
			GiveItem(role, 0, 7321, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Seal Master's Belt]", 1)
			return
		elseif Chance == 5 then
			GiveItem(role, 0, 7324, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Voyager's Belt]", 1)
			return
		elseif Chance == 6 then
			GiveItem(role, 0, 7327, 1, 4)
			Notice("Congratulations to player "..Name.." for defeating the Thief - Wang Xiao Hu and obtained [Wang Xiao Hu's Chest]. Wow, you're lucky to receive a [Lv65 Sharpshooter's Belt]", 1)
			return
		end
	end
end
function ExpCheckSys_Toggle(role, Item)
	local ON = GetItemAttr(Item, 55);
	if ON == 1 then
		BickerNotice(role, "You have enabled the experience system for monsters.")
		SetItemAttr(Item, 55, 0)
	else
		BickerNotice(role, "You have disabled the experience system for monsters.")
		SetItemAttr(Item, 55, 1)
	end
	SynChaKitbag(role, 13)
end
function ItemUse_FlyWings(role, Item, Item_Traget)
	if KitbagLock(role, 0) == LUA_FALSE then
		SystemNotice( role , "Inventory has been binded")
		return
	end
	local isBoat = 0
	isBoat = GetCtrlBoat(role)
	if isBoat ~= nil then
		SystemNotice(role, "Cannot use while sailing")
		UseItemFailed(role)
		return
	end
	if GetItemType(Item_Traget) ~= 44 then
		SystemNotice(role, "Only wings can be upgraded.")
		UseItemFailed(role)
		return
	end
	if GetItemPrefix(Item_Traget) == 95 then
		SystemNotice(role, GetItemName(GetItemID(Item_Traget)).." already upgraded!")
		UseItemFailed(role)
		return
	end
	SystemNotice(role, 'Successfully upgrade '..GetItemName(GetItemID(Item_Traget))..'! You can now use your wings to fly')
	SetItemPrefix(Item_Traget, 95)
	PlayEffect(role, 345)
	SynChaKitbag(role, 13)
end
function ItemUse_Admiral(role, Item)
	if GetCtrlBoat(role) ~= nil then
		SystemNotice(role, "Cannot use while sailing.")
		UseItemFailed(role)
		return
	end
	if KitbagLock(role, 1) == LUA_TRUE or GetChaStateLv(role, STATE_BAT) >= 1 or GetChaStateLv(role, STATE_JY) >= 1 then
		PopupNotice(role, "Inventory Locked! unlock it!")
		UseItemFailed(role)
		return
	end
	local ItemID = GetItemID(Item)
	if GetChaFreeBagGridNum(role) < 2 then
		SystemNotice(role, "Failed to use ["..GetItemName(ItemID).."]. Requires 2 slots in inventory")
		UseItemFailed(role)
		return 
	else
		local Type = GetChaTypeID(role)
		GiveItem(role, 0, Server.Cloak.Item.ID, 1, 88)
		Notice(GetChaDefaultName(role)..' successfully Unsealed a '..GetItemName(ItemID)..'!');
		ScrollNotice("Congratulations! role "..GetChaDefaultName(role).." has obtained Lv1 Admiral Cloak", 1, 0XFFF6E58D)
	end
end
function Cloak_Upgrade(role, Item, Target)
	if GetCtrlBoat(role) ~= nil then
		SystemNotice(role, "Cannot use while sailing.")
		UseItemFailed(role)
		return
	end
	if KitbagLock(role, 1) == LUA_TRUE or GetChaStateLv(role, STATE_BAT) >= 1 or GetChaStateLv(role, STATE_JY) >= 1 then
		BickerNotice(role, "Inventory is locked!")
		UseItemFailed(role)
		return
	end
	local CloakID = GetItemID(Target)
	if  GetItemType(Target) ~= 88 then
		SystemNotice(role, "You can only use this item to cloaks")
		UseItemFailed(role)
		return
	end
	local Level = GetItemAttr(Target, 55)
	if Level >= Server.Cloak.Level then
		BickerNotice(role, ""..GetItemName(GetItemID(Target)).." already reached max level!")
		UseItemFailed(role)
		return
	end
	if Percentage_Random(Server.Cloak.Rate[Level]) == 1 then
		for i = ITEMATTR_VAL_STR, ITEMATTR_VAL_STA, 1 do
			SetItemAttr(Target, i, GetItemAttr(Target, i) + Server.Cloak.StatPerLevel)
		end
		SetItemAttr(Target, 55, Level + 1)
		SetItemLv(Target, Level + 1)
		PlayEffect(role, 345)
		RefreshCha(role)
		SynChaKitbag(role, 13)
		SynLook(role)
		SystemNotice(role, "Congratulations! You have succesfully enchanced your Admiral Cloak to LV "..math.floor(Level+1)..".")		
	else
		SystemNotice(role, "Sorry, upgrade has failed! Luckily ["..GetItemName(GetItemID(Target)).."] is not damaged...")
		PlayEffect(role, 346)
	end
end
