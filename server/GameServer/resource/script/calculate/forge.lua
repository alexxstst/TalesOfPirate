print("-- [Loading] Forge")

function can_unite_item(...)
    local arg = {...}
    if #arg ~= 12 then
        return 0
    end
    if can_unite_item_main(arg) == 1 then
        return 1
    else
        return 0
    end
end
function can_unite_item_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local Get_Count = 4
    local ItemReadCount = 0
    local ItemReadNow = 1
    local ItemReadNext = 0
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    local i = 0
    for i = 0, 2, 1 do
        if ItemBagCount[i] ~= 1 or ItemCount[i] ~= 1 then
            SystemNotice(Player, "Item target unit and item unit illegal.")
            return 0
        end
    end
    local Scroll = GetChaItem(Player, 2, ItemBag[0])
    local LeftGem = GetChaItem(Player, 2, ItemBag[1])
    local RightGem = GetChaItem(Player, 2, ItemBag[2])
    if GetItemType(Scroll) ~= 47 then
        SystemNotice(Player, "This is not a combining scroll.")
        return 0
    end
    if GetItemType(LeftGem) ~= 49 or GetItemType(RightGem) ~= 49 then
        if GetItemType(LeftGem) ~= 50 or GetItemType(RightGem) ~= 50 then
            SystemNotice(Player, "This is not a Gem.")
            return 0
        end
    end
    if Get_StoneLv(LeftGem) ~= Get_StoneLv(RightGem) or GetItemID(LeftGem) ~= GetItemID(RightGem) then
        SystemNotice(Player, "Both gems must be the same level and type in other to combine them.")
        return 0
    end
    for i = 0, #GemVar, 1 do
        if GetItemID(LeftGem) == GemVar[i].ID then
            if Get_StoneLv(LeftGem) >= GemVar[i].Level or Get_StoneLv(RightGem) >= GemVar[i].Level then
                SystemNotice( Player, "The maximum level for [" .. GetItemName(GemVar[i].ID) .. "] is Level " .. GemVar[i].Level .. ".")
                return 0
            end
        end
    end
    if getunite_money_main(Table) > GetChaAttr(Player, ATTR_GD) then
        SystemNotice(Player, "Insufficient gold, unable to combine gems.")
        return 0
    end
    return 1
end

function Combine_Success_Rate(GemLv, GemType, BonusFruit)
    local GemLv = GemLv - 1
    local StateLv = BonusFruit * 0.1
    local a = 0
    local b = 0

    if GemType == 49 then
        local Rate = {}
        Rate[0] = 1
        Rate[1] = .9
        Rate[2] = .8
        Rate[3] = .7
        Rate[4] = .6
        Rate[5] = .5
        Rate[6] = .4
        Rate[7] = .3
        Rate[8] = .2
        if Rate[GemLv] ~= nil then
            a = Rate[GemLv] + StateLv
            b = Percentage_Random(a)
        else
            b = 0
        end
        return b
    elseif GemType == 50 then
        local Rate = {}
        Rate[0] = 1
        Rate[1] = 1
        Rate[2] = 1
        Rate[3] = .95
        Rate[4] = .9
        Rate[5] = .85
        Rate[6] = .8
        Rate[7] = .75
        Rate[8] = .7
        if Rate[GemLv] ~= nil then
            a = Rate[GemLv] + StateLv
            b = Percentage_Random(a)
        else
            b = 0
        end
        return b
    else
        return 0
    end
end
function begin_unite_item(...)
    local arg = {...}
    if can_unite_item_main(arg) == 0 then
        return 0
    end
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local Get_Count = 4
    local ItemReadCount = 0
    local ItemReadNow = 1
    local ItemReadNext = 0
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(arg)
    local BagItem1 = ItemBag[0]
    local BagItem2 = ItemBag[1]
    local BagItem3 = ItemBag[2]
    local Scroll = GetChaItem(Player, 2, BagItem1)
    local LeftGem = GetChaItem(Player, 2, BagItem2)
    local RightGem = GetChaItem(Player, 2, BagItem3)
    local LeftGemID = GetItemID(LeftGem)
    local LeftGemType = GetItemType(LeftGem)
    local LeftGemLevel = Get_StoneLv(LeftGem)
    RemoveChaItem(Player, GetItemID(Scroll), 1, 2, BagItem1, 2, 1, 0)
    RemoveChaItem(Player, GetItemID(RightGem), 1, 2, BagItem3, 2, 1, 0)
    Set_StoneLv(LeftGem, (LeftGemLevel + 1))
    SetCharaAttr((GetChaAttr(Player, ATTR_GD) - getunite_money_main(arg)), Player, ATTR_GD)
    ALLExAttrSet(Player)

    local fruitBonus = 0
    local stateLv = GetChaStateLv(Player, STATE_HCGLJB)
    fruitBonus = fruitBonus + stateLv

    local Succes_Rate = Combine_Success_Rate(LeftGemLevel, LeftGemType, fruitBonus)
    if Succes_Rate == 0 then
        for i = 0, GetKbCap(Player), 1 do
            if GetItemID(GetChaItem(Player, 2, i)) == 3075 then
                RemoveChaItem(Player, 3075, 1, 2, i, 2, 1, 0)
            end
            break
        end
        RemoveChaItem(Player, LeftGemID, 1, 2, BagItem2, 2, 1, 0)
        SystemNotice(Player, "Sorry, the combination has failed and the gems has vanished.")
        return 2
    end
    return 1
end
function get_item_unite_money(...)
    local arg = {...}
    local Money = getunite_money_main(arg)
    return 0
end
function getunite_money_main(Table)
    return 5000
end
function CheckStoneType(Item, Stone1, Stone2)
    if GetItemType(Stone2) == 49 then
        Stone1 = Stone2
    end
    for a = 1, #GemVar, 1 do
        if GemVar[a].ID == GetItemID(Stone1) then
            for b = 0, #GemVar[a].Equip, 1 do
                if GemVar[a].Equip[b] == GetItemType(Item) then
                    return 1
                end
                if GemVar[a].Equip[b] == 0 then
                    return 0
                end
            end
        end
    end
    return 0
end
function can_forge_item(...)
    local arg = {...}
    if #arg ~= 12 then
        return 0
    end
    if can_forge_item_main(arg) == 1 then
        return 1
    else
        return 0
    end
end
function can_forge_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    local ItemBagCount_Jinglian = ItemBagCount[0]
    local ItemBag_Jinglian = ItemBag[0]
    local ItemNum_Jinglian = ItemCount[0]
    local Equipment = GetChaItem(role, 2, ItemBag_Jinglian)
    local Check = 0
    if ItemBagCount_Jinglian ~= 1 then
        SystemNotice(role, "Forging item related slot illegal.")
        return 0
    end
    if ItemNum_Jinglian ~= 1 then
        SystemNotice(role, "Forging item quantity illegal")
        return 0
    end
    Check = CheckItem_CanJinglian(Equipment)
    if Check == 0 then
        SystemNotice(role, "Item cannot be forged")
        return 0
    end
    if ItemCount[1] ~= 1 or ItemCount[2] ~= 1 or ItemBagCount[1] ~= 1 or ItemBagCount[2] ~= 1 then
        SystemNotice(role, "Illegal Gem Quantity")
        return 0
    end
    local GemL = GetChaItem(role, 2, ItemBag[1])
    local GemR = GetChaItem(role, 2, ItemBag[2])
    for i = 0, #GemVar, 1 do
        if GetItemID(GemL) == GemVar[i].ID then
            if Get_StoneLv(GemL) > GemVar[i].Level then
                SystemNotice(role, "The maximum level for [" .. GetItemName(GemVar[i].ID) .. "] is Level " .. GemVar[i].Level .. ".")
                return 0
            end
        end
    end
    if CheckItem_HaveHole(Equipment, GemL, GemR) == 0 then
        SystemNotice(role, "Does not have enough socket to forge")
        return 0
    end
    if GetItemAttr(Equipment, ITEMATTR_MAXURE) == 25000 then
        SystemNotice(role, "You cannot forge an apparel")
        return 0
    end
    local Check_Stone = Check_StoneLv(Equipment, GemL, GemR)
    if Check_Stone == 0 then
        SystemNotice(role, "Gem or Refining Gem level does not match")
        return 0
    end
    if Check_Stone == -1 then
        SystemNotice(role, "Refining gem can only composite with refining gem")
        return 0
    end
    if CheckStoneType(Equipment, GemL, GemR) == 0 then
        SystemNotice(role, "Gem and forging item does not match")
        return 0
    end
    if getforge_money_main(Table) > GetChaAttr(role, ATTR_GD) then
        SystemNotice(role, "Insufficient Gold. Unable to forge.")
        return 0
    end
    return 1
end

function Forging_Success_Rate(GemLv, BonusFruit)
    local GemLv = GemLv
    local StateLv = BonusFruit * 0.1
    local a = 0
    local b = 0
    local Rate = {}
    Rate[1] = 1
    Rate[2] = 1
    Rate[3] = 1
    Rate[4] = .45
    Rate[5] = .35
    Rate[6] = .25
    Rate[7] = .15
    Rate[8] = .05
    Rate[9] = .02
    if Rate[GemLv] ~= nil then
        a = Rate[GemLv] + StateLv
        b = Percentage_Random(a)
    else
        b = 0
    end
    return b
end

function begin_forge_item(...)
    local arg = {...}
    if can_forge_item_main(arg) == 0 then
        return 0
    end
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)
    local Equipment = GetChaItem(role, 2, ItemBag[0])
    local GemL = GetChaItem(role, 2, ItemBag[1])
    local GemR = GetChaItem(role, 2, ItemBag[2])
    local EquipmentID = GetItemID(Equipment)
    local GemLID = GetItemID(GemL)
    local GemRID = GetItemID(GemR)
    local Forge = TansferNum(GetItemForgeParam(Equipment, 1))
    local GemAttr = {}
    local Final = {Gem = nil, ID = 0, LevelT = 0, LevelN = 1}
    GemAttr[0] = {ID = GetNum_Part2(Forge), Level = GetNum_Part3(Forge)}
    GemAttr[1] = {ID = GetNum_Part4(Forge), Level = GetNum_Part5(Forge)}
    GemAttr[2] = {ID = GetNum_Part6(Forge), Level = GetNum_Part7(Forge)}

    if GetItemType(GemL) == 49 then
        Final.Gem = GemL
    elseif GetItemType(GemR) == 49 then
        Final.Gem = GemR
    end
    for i = 0, 2, 1 do
        Final.LevelT = Final.LevelT + GemAttr[i].Level
        if GetGemID(GetItemID(Final.Gem)) == GemAttr[i].ID then
            Final.LevelN = GemAttr[i].Level + 1
        end
    end
    SetCharaAttr((GetChaAttr(role, ATTR_GD) - getforge_money_main(arg)), role, ATTR_GD)
    ALLExAttrSet(role)

    local fruitBonus = 0
    local stateLv = GetChaStateLv(role, STATE_JLGLJB)
    fruitBonus = fruitBonus + stateLv

    local Succes_Rate = Forging_Success_Rate(Final.LevelN, fruitBonus)
    if Succes_Rate == 1 then
        Jinglian_Item(Equipment, GemL, GemR)
        SynChaKitbag(role, 13)
        RemoveChaItem(role, GemLID, 1, 2, ItemBag[1], 2, 1, 0)
        RemoveChaItem(role, GemRID, 1, 2, ItemBag[2], 2, 1, 0)
        check_item_final_data(Equipment)
        return 1
    else
        SystemNotice(role, "Sorry, forging failed. Your equipments is still intact and you only lost the gems.")
        RemoveChaItem(role, GemLID, 1, 2, ItemBag[1], 2, 1, 0)
        RemoveChaItem(role, GemRID, 1, 2, ItemBag[2], 2, 1, 0)
        check_item_final_data(Equipment)
        return 2
    end
end
function get_item_forge_money(...)
    local arg = {...}
    return getforge_money_main(arg)
end
function getforge_money_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local Equipment = GetChaItem(role, 2, ItemBag[0])
    local Forge = TansferNum(GetItemForgeParam(Equipment, 1))
    local EquipmentLevel = 1 + GetNum_Part3(Forge) + GetNum_Part5(Forge) + GetNum_Part7(Forge)
    return (EquipmentLevel * 100000)
end

function Get_StoneLv(Item)
    local Lv = GetItemAttr(Item, ITEMATTR_VAL_BaoshiLV)
    return Lv
end

function Set_StoneLv(Item, Item_Lv)
    local i = 0
    i = SetItemAttr(Item, ITEMATTR_VAL_BaoshiLV, Item_Lv)
    if i == 0 then
        LG("Hecheng_BS", "Failed to set gem level")
    end
end

function CheckItem_CanJinglian(Item)
    local Item_Type = GetItemType(Item)
    local i = 0
    for i = 0, Item_CanJinglian_Num, 1 do
        if Item_Type == Item_CanJinglian_ID[i] then
            return 1
        end
    end
    return 0
end

function CheckItem_HaveHole(Item, Stone1, Stone2)
    local Num = GetItemForgeParam(Item, 1)
    Num = TansferNum(Num)
    local Hole = GetNum_Part1(Num)
    local Item_Stone = {}
    local Stone1TypeID = 0
    local Stone2TypeID = 0

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    local i = 0
    local Hole_empty = 0

    for i = 0, 2, 1 do
        if Item_Stone[i] == 0 then
            Hole_empty = Hole_empty + 1
        end

        Stone1TypeID = GetStone_TypeID(Stone1)
        Stone2TypeID = GetStone_TypeID(Stone2)

        if Item_Stone[i] == Stone1TypeID or Item_Stone[i] == Stone2TypeID then
            return 1
        end
    end

    local Hole_Used = 3 - Hole_empty

    if Hole_Used >= Hole then
        return 0
    else
        return 1
    end
end

function Check_StoneLv(Item, Stone1, Stone2)
    local Num = GetItemForgeParam(Item, 1)
    Num = TansferNum(Num)

    local Jinglian_Lv = GetItem_JinglianLv(Item)

    local Stone1Type = GetItemType(Stone1)
    local Stone2Type = GetItemType(Stone2)

    local Jinglianshi = 0
    local Jinglianshi_Lv = 0
    local Baoshi = 0
    local Baoshi_Lv = 0
    local Baoshi_NeedLv = 0

    if Stone1Type == 50 then
        Jinglianshi = Stone1
    elseif Stone2Type == 50 then
        Jinglianshi = Stone2
    end

    if Stone1Type == 49 then
        Baoshi = Stone1
    elseif Stone2Type == 49 then
        Baoshi = Stone2
    end

    Jinglianshi_Lv = Get_StoneLv(Jinglianshi)
    Baoshi_Lv = Get_StoneLv(Baoshi)

    local Item_Stone = {}
    local Item_StoneLv = {}

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    BaoshiType = GetStone_TypeID(Baoshi)

    local i = 0

    for i = 0, 2, 1 do
        if BaoshiType == Item_Stone[i] then
            Baoshi_NeedLv = Item_StoneLv[i] + 1
        end
    end

    local Jinglianshi_NeedLv = Baoshi_NeedLv

    if Baoshi_Lv < Baoshi_NeedLv then
        return 0
    end

    if Jinglianshi_Lv < Jinglianshi_NeedLv then
        return 0
    end

    return 1
end

function Jinglian_Item(Item, Stone1, Stone2)
    local Num = GetItemForgeParam(Item, 1)
    Num = TansferNum(Num)
    local Jinglian_Lv = GetItem_JinglianLv(Item)
    local Stone1Type = GetItemType(Stone1)
    local Stone2Type = GetItemType(Stone2)
    local Baoshi = 0
    local Num_New = Num

    if Stone1Type == 49 then
        Baoshi = Stone1
    elseif Stone2Type == 49 then
        Baoshi = Stone2
    end

    Num_New = SetJinglian_Lv(Baoshi, Baoshi_Lv, Num)

    local i = 0

    i = SetItemForgeParam(Item, 1, Num_New)

    if i == 0 then
        LG("Jinglian", "set forging content failed")
    end

    local Item_URE_Add = 0
    local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)
    if Item_MAXURE < 600 then
        Item_MAXURE = math.min((Item_MAXURE + Item_URE_Add), 600)
    end

    local j = 0
    j = SetItemAttr(Item, ITEMATTR_MAXURE, Item_MAXURE)

    if j == 0 then
        LG("Jinglian", "Forge setting maximum durability failed")
    end

    if Num_New == Num then
    end

    return 1
end

function SetJinglian_Lv(Baoshi, Baoshi_Lv, Num)
    local Baoshi_Lv = 0
    Baoshi_Lv = Get_StoneLv(Baoshi)

    local Item_Stone = {}
    local Item_StoneLv = {}

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    BaoshiType = GetStone_TypeID(Baoshi)

    local i = 0
    local Stone_Check = 0

    for i = 0, 2, 1 do
        if BaoshiType == Item_Stone[i] then
            Item_StoneLv[i] = Item_StoneLv[i] + 1
            Stone_Check = i + 1
        end
    end

    if Stone_Check == 1 then
        Num = SetNum_Part3(Num, Item_StoneLv[0])
    elseif Stone_Check == 2 then
        Num = SetNum_Part5(Num, Item_StoneLv[1])
    elseif Stone_Check == 3 then
        Num = SetNum_Part7(Num, Item_StoneLv[2])
    elseif Stone_Check == 0 then
        local Check_empty = 0
        for i = 2, 0, -1 do
            if Item_Stone[i] == 0 then
                Check_empty = i + 1
            end
        end

        if Check_empty == 1 then
            Num = SetNum_Part2(Num, BaoshiType)
            Num = SetNum_Part3(Num, 1)
        elseif Check_empty == 2 then
            Num = SetNum_Part4(Num, BaoshiType)
            Num = SetNum_Part5(Num, 1)
        elseif Check_empty == 3 then
            Num = SetNum_Part6(Num, BaoshiType)
            Num = SetNum_Part7(Num, 1)
        end
    end

    return Num
end

function GetStone_TypeID(Stone)
    for i = 0, #GemVar, 1 do
        if GemVar[i].ID == GetItemID(Stone) then
            return i
        end
    end
    return -1
end

function Read_Table(Table)
    local role = Table[1]

    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local Get_Count = 4
    local ItemReadCount = 0
    local ItemReadNow = 2
    local ItemReadNext = 0
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_New = 0
    local i = 0
    local j = 0

    for i = 0, Get_Count, 1 do
        if ItemReadNow <= #Table then
            ItemBagCount[i] = Table[ItemReadNow]

            ItemBagCount_New = ItemBagCount_New + 1

            ItemReadNow = ItemReadNow + 1
            ItemReadNext = ItemReadNow + 2 * (ItemBagCount[i] - 1)
            ItemReadCount = ItemReadNow
            if ItemBagCount[i] ~= 0 then
                for j = ItemReadCount, ItemReadNext, 2 do
                    ItemBag[ItemBag_Now] = Table[j]

                    ItemBag_Now = ItemBag_Now + 1
                    ItemCount[ItemCount_Now] = Table[j + 1]

                    ItemCount_Now = ItemCount_Now + 1
                    ItemReadNow = ItemReadNow + 2
                end
            end
        else
            ItemBagCount[i] = 0
        end
    end

    return role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_New
end

function check_item_final_data(Item)
    if GetItemType(Item) == 59 then
        return
    end
    Forge = TansferNum(GetItemForgeParam(Item, 1))
    local StoneID,StoneLv = {},{}
    StoneID[0], StoneID[1], StoneID[2] = GetNum_Part2(Forge), GetNum_Part4(Forge), GetNum_Part6(Forge)
    StoneLv[0], StoneLv[1], StoneLv[2] = GetNum_Part3(Forge), GetNum_Part5(Forge), GetNum_Part7(Forge)
    local ResetCheck = 0
    ResetCheck = ResetItemFinalAttr(Item)
    if ResetCheck == 0 then
        LG("check_item_final","ResetCheck Failed")
        return
    end
	
    local AddCheck = 0
    local i = 0
    for i = 0, 2, 1 do
        if StoneID[i] ~= nil and StoneID[i] ~= 0 then
            local GemID = StoneID[i]
			if GemVar[GemID] ~= nil and GemVar[GemID] ~= 0 then
				if GemVar[GemID].Attribute == ITEMATTR_VAL_MNATK then
					local ItemAttrOne = GemVar[GemID].Attribute
					local ItemAttrTwo = ItemAttrOne + 1
					local ItemAttrEff = 0
					local GemLv = 0
					if GemLv > 0 or GemLv <= 9 then
						GemLv = StoneLv[i]
					end
					if GemID >= 0 and GemID <= #GemVar then
						ItemAttrEff = GemVar[GemID].Effect * GemLv
					end
					AddCheck = AddItemFinalAttr(Item, ItemAttrOne, ItemAttrEff)
					if AddCheck == 0 then
						LG("check_item_final","AddCheck Failed")
					end
					AddCheck = AddItemFinalAttr(Item, ItemAttrTwo, ItemAttrEff)
					if AddCheck == 0 then
						LG("check_item_final","AddCheck Failed")
					end
				else
					local ItemAttrOne = GemVar[GemID].Attribute
					local ItemAttrEff = 0
					local GemLv = 0
					if GemLv > 0 or GemLv <= 9 then
						GemLv = StoneLv[i]
					end
					if GemID >= 0 and GemID <= #GemVar then
						ItemAttrEff = GemVar[GemID].Effect * GemLv
					end
					AddCheck = AddItemFinalAttr(Item, ItemAttrOne, ItemAttrEff)
					if AddCheck == 0 then
						LG("check_item_final","AddCheck Failed")
					end
				end
			end
        end
    end
end

function Check_StoneItemType(Item, Stone1, Stone2)
    local Stone1Type = GetItemType(Stone1)
    local Stone2Type = GetItemType(Stone2)
    local Baoshi = 0
    local ItemType = GetItemType(Item)

    if Stone1Type == 49 then
        Baoshi = Stone1
    elseif Stone2Type == 49 then
        Baoshi = Stone2
    end

    local Baoshi_ID = GetItemID(Baoshi)
    local i = 0
    local Baoshi_TypeID = 0

    for i = 1, StoneAttrType_Num, 1 do
        if Baoshi_ID == StoneTpye_ID[i] then
            Baoshi_TypeID = i
        end
    end

    for i = 0, 15, 1 do
        if ItemType == StoneItemType[Baoshi_TypeID][i] then
            return 1
        end
        if StoneItemType[Baoshi_TypeID][i] == 0 then
            return 0
        end
    end

    return 0
end

function GetItem_JinglianLv(Item)
    local Num = GetItemForgeParam(Item, 1)
    Num = TansferNum(Num)
    local Item_StoneLv = {}
    local JinglianLv = 0

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    JinglianLv = Item_StoneLv[0] + Item_StoneLv[1] + Item_StoneLv[2]

    return JinglianLv
end

function CheckStoneInfo(Num)
    local Item_Stone = {}
    local Item_StoneLv = {}

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    return Item_Stone[0], Item_Stone[1], Item_Stone[2], Item_StoneLv[0], Item_StoneLv[1], Item_StoneLv[2]
end

function Check_CG_HechengBS(Item_Lv, Item_Type, Sklv)
    local a = 0
    local b = 0
    Item_Lv = Item_Lv - 1
    if Item_Type == 49 then
        a = math.max(0, math.min(1, (1 - Item_Lv * 0.10 + Sklv * 0.10)))
        b = Percentage_Random(a)
        if Item_Lv < 3 then
            b = 1
        end
        return b
    elseif Item_Type == 50 then
        a = math.max(0, math.min(1, (1 - Item_Lv * 0.05 + Sklv * 0.15)))
        b = Percentage_Random(a)
        return b
    else
        LG("Hecheng_BS", "probability check determine item type is not a gem")
        return 0
    end
end

function Check_CG_Jinglian(Jinglian_Lv, Stone_Lv, Sklv)
    local b = 0

    b = 1

    return b
end

function Roll_DiamondId(cha)
    local a = math.random(1, 8)
    local DiamondId = StoneTpye_ID[a]
    return DiamondId
end

function Transfer_DiamondScript_Lv1(role)
    local cha = TurnToCha(role)
    local x_give = 0
    local y_give = 0
    local script_count = CheckBagItem(cha, 3877)
    local DiamondId = Roll_DiamondId(cha)
    if script_count >= 1 then
        x_del = DelBagItem(cha, 3877, 1)
        if x_del == 1 then
            x_give = GiveItem(cha, 0, DiamondId, 1, 101)

            y_give = GiveItem(cha, 0, 885, 1, 101)
        else
            SystemNotice(cha, "Unable to deduct Gem Voucher")
        end
    else
        SystemNotice(cha, "You need to have a Lv 1 Gem Voucher in order to redeem")
    end
    if x_give == 1 and y_give == 1 then
        return 1
    else
        return 0
    end
end

function Transfer_DiamondScript_Lv2(role)
    local cha = TurnToCha(role)
    local x_give = 0
    local y_give = 0
    local script_count = CheckBagItem(cha, 3878)
    local DiamondId = Roll_DiamondId(cha)
    if script_count >= 1 then
        x_del = DelBagItem(cha, 3878, 1)
        if x_del == 1 then
            x_give = GiveItem(cha, 0, DiamondId, 1, 102)

            y_give = GiveItem(cha, 0, 885, 1, 102)
        else
            SystemNotice(cha, "Unable to deduct Gem Voucher")
        end
    else
        SystemNotice(cha, "Requires Lv 2 Refining Gem Voucher to redeem")
    end
    if x_give == 1 and y_give == 1 then
        return 1
    else
        return 0
    end
end

function GetChaName_0(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "You does not seems to bring any Christmas Greeting Cards and gold")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice(
            " comes from " ..
                cha_name ..
                    "'s blessing: May God bless you on this Christmas season! The Goddess of Mercy protect you! God of Fortune hugs you! Cupid shoots you!God of Cookery bites you!"
        )
    else
    end
end

function GetChaName_1(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "You does not seems to bring any Christmas Greeting Cards and gold")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice(
            " comes from " ..
                cha_name ..
                    "'s blessing: Having realized that tons of wishes will jam the net connection after several days, I , the most intelligent and ambitious genius wish everyone...Merry Christmas!"
        )
    else
    end
end

function GetChaName_2(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "You does not seems to bring any Christmas Greeting Cards and gold")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice(
            " comes from " ..
                cha_name ..
                    "'s beautiful wish: To spend Christmas eve with you, holding your hands and listen to the chimes of the midnight clock. Do you wish to fulfill the wish?"
        )
    else
    end
end

function GetChaName_3(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "You does not seems to bring any Christmas Greeting Cards and gold")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice(
            " comes from " ..
                cha_name ..
                    "'s well wishes: After a year has past, we once again welcome this Christmas Night. This moment is the time to be happy; to reminisce old friends; to care; and for dreams to come true... Merry Christmas!"
        )
    else
    end
end

function GetChaName_26(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice(
            "" .. cha_name .. ",,,,"
        )
    else
    end
end

function GetChaName_27(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice("" .. cha_name .. ":,,")
    else
    end
end

function GetChaName_28(role, npc)
    local cha_name = GetChaDefaultName(role)
    local Money_Need = 1000
    local Money_Have = GetChaAttr(role, ATTR_GD)
    local item_num = CheckBagItem(role, 2887)
    if Money_Need > Money_Have or item_num <= 0 then
        SystemNotice(role, "")
    else
    end
    if Money_Have >= Money_Need and item_num > 0 then
        TakeMoney(role, nil, Money_Need)
        TakeItem(role, 0, 2887, 1)
        PlayEffect(npc, 361)
        Notice("" .. cha_name .. "")
    else
    end
end

TEAMSTAR_EQ = {}
TEAMSTAR_EQ[08] = 1382
TEAMSTAR_EQ[09] = 1392
TEAMSTAR_EQ[12] = 1409
TEAMSTAR_EQ[13] = 1433
TEAMSTAR_EQ[14] = 1467
TEAMSTAR_EQ[16] = 1419

function Transfer_TeamStar(role, level)
    local TEAMSTAR_EQ = TEAMSTAR_EQ
    local cha = TurnToCha(role)
    local ItemCount = CheckBagItem(cha, 1034)
    local Level = GetChaAttr(cha, ATTR_LV)
    local JOB = GetChaAttr(role, ATTR_JOB)
    if Level < 41 then
        SystemNotice(role, "Please come back after Lv41.")
        return
    end
    if ItemCount < 1 then
        SystemNotice(cha, "Need to possess Star of Unity to receive")
        return
    end
    if ItemCount > 1 then
        SystemNotice(cha, "Too many Star of Unity was found!")
        return
    end
    if ItemCount == 1 then
        if BankNoItem(role, 1034, 1) ~= LUA_TRUE then
            SystemNotice(cha, "Existing Star of Unity was found in bank!")
            return
        end
        if EquipNoItem(role, 1034, 1) ~= LUA_TRUE then
            SystemNotice(cha, "Existing Star of Unity was found in equipment slot!")
            return
        end
        if TEAMSTAR_EQ[JOB] == nil then
            SystemNotice(role, "Can only be used by characters that have completed second advancement. Please look for class trainers NPC of each city to complete your rebirth quest before redemption.")
            return
        end
        local x_del = DelBagItem(cha, 1034, 1)
        if x_del == 1 then
            GiveItem(role, 0, TEAMSTAR_EQ[JOB], 1, 22)
        else
            SystemNotice(cha, "Unable to deduct Star of Unity")
        end
    end
end

function TransferDiamond(role, level)
    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag

    if level == 1 then
        retbag = HasLeaveBagGrid(role, 2)
        if retbag ~= LUA_TRUE then
            SystemNotice(role, "Inventory requires at least 2 empty slots to redeem")
            return
        end
        Transfer_DiamondScript_Lv1(role)
    elseif level == 2 then
        retbag = HasLeaveBagGrid(role, 2)
        if retbag ~= LUA_TRUE then
            SystemNotice(role, "Inventory requires at least 2 empty slots to redeem")
            return
        end
        Transfer_DiamondScript_Lv2(role)
    elseif level == 3 then
        retbag = HasLeaveBagGrid(role, 1)
        if retbag ~= LUA_TRUE then
            SystemNotice(role, "Insufficent inventory space to redeem")
            return
        end
        Transfer_OneStoneScript(role)
    elseif level == 4 then
        retbag = HasLeaveBagGrid(role, 1)
        if retbag ~= LUA_TRUE then
            SystemNotice(role, "Insufficent inventory space to redeem")
            return
        end
        Transfer_OneDiamondScript(role)
    else
        LG("BSduihuan", "Wrong coupon used")
    end
end

function Transfer_OneStoneScript(role)
    local cha = TurnToCha(role)
    local y_give = 0

    local script_count = CheckBagItem(cha, 3885)

    if script_count >= 1 then
        x_del = DelBagItem(cha, 3885, 1)
        if x_del == 1 then
            y_give = GiveItem(cha, 0, 885, 1, 101)
        else
            SystemNotice(cha, "Unable to deduct Gem Voucher")
        end
    else
        SystemNotice(cha, "You must have Refining Gem Voucher in your inventory to redeem")
    end
    if y_give == 1 then
        return 1
    else
        return 0
    end
end

function Transfer_OneDiamondScript(role)
    local cha = TurnToCha(role)
    local x_give = 0
    local y_give = 0
    local script_count = CheckBagItem(cha, 3886)
    local DiamondId = Roll_DiamondId(cha)
    if script_count >= 1 then
        x_del = DelBagItem(cha, 3886, 1)
        if x_del == 1 then
            x_give = GiveItem(cha, 0, DiamondId, 1, 101)
        else
            SystemNotice(cha, "Unable to deduct Gem Voucher")
        end
    else
        SystemNotice(cha, "You need to have a Gem Voucher in order to redeem")
    end
    if x_give == 1 then
        return 1
    else
        return 0
    end
end

function TansferNum(Num)
    if Num < 0 then
        Num = Num + 4294967296
    end
    return Num
end

function can_milling_item(...)
    local arg = {...}

    if #arg ~= 12 then
        return 0
    end

    local kkk = 0

    local Check = 0

    Check = can_milling_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end

function can_milling_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local Get_Count = 4
    local ItemReadCount = 0
    local ItemReadNow = 1
    local ItemReadNext = 0
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)

    local ItemBag_damo = ItemBag[0]
    local Item_damo = GetChaItem(role, 2, ItemBag_damo)
    local Item_Cailiao1 = GetChaItem(role, 2, ItemBag[1])
    local Item_Cailiao2 = GetChaItem(role, 2, ItemBag[2])

    local Check_Cailiao1 = 0
    local Check_Cailiao2 = 0

    Check_Cailiao1 = Check_Jiaguji(Item_Cailiao1, Item_Cailiao2)
    Check_Cailiao2 = Check_Cuihuafen(Item_Cailiao1, Item_Cailiao2)

    if Check_Cailiao1 == 0 then
        SystemNotice(role, "Fusion requires Equipment Stabilizer")
        return 0
    end

    if Check_Cailiao2 == 0 then
        SystemNotice(role, "Requires Equipment Catalyst for fusion")
        return 0
    end

    if GetItemAttr(Item_damo, ITEMATTR_MAXURE) == 25000 then
        SystemNotice(role, "You cannot put a socket on an apparel")
        return 0
    end
	
    local Check_Hole = 0

    Check_Hole = Check_HasHole(Item_damo)

    if Check_Hole >= Server.Socket.Limit then
        SystemNotice(role, "Socket slots are max. Unable to continue Fusion")
        return 0
    end

    local Money_Need = get_milling_money_main(Table)
    local Money_Have = GetChaAttr(role, ATTR_GD)
    if Money_Need > Money_Have then
        SystemNotice(role, "Insufficient gold. Unable to undergo fusion")
        return 0
    end

    return 1
end

function get_item_milling_money(...)
    local arg = {...}

    local Money = get_milling_money_main(arg)
    return Money
end

function get_milling_money_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local Equipment = GetChaItem(Player, 2, ItemBag[0])
    local Sockets = GetNum_Part1(TansferNum(GetItemForgeParam(Equipment, 1)))
    return ((Sockets + 1) * Server.Socket.Cost)
end

function begin_milling_item(...)
    local arg = {...}

    local Check_CanMilling = 0
    Check_CanMilling = can_milling_item_main(arg)

    if Check_CanMilling == 0 then
        return 0
    end

    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local Get_Count = 4
    local ItemReadCount = 0
    local ItemReadNow = 1
    local ItemReadNext = 0
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)

    local ItemBag_damo = ItemBag[0]
    local Item_damo = GetChaItem(role, 2, ItemBag_damo)
    local Item_cailiao1 = GetChaItem(role, 2, ItemBag[1])
    local Item_cailiao2 = GetChaItem(role, 2, ItemBag[2])

    local Money_Need = get_milling_money_main(arg)
    local Money_Have = GetChaAttr(role, ATTR_GD)

    Money_Have = Money_Have - Money_Need
    SetCharaAttr(Money_Have, role, ATTR_GD)
    ALLExAttrSet(role)

    local ItemID_Cailiao1 = GetItemID(Item_cailiao1)
    local ItemID_Cailiao2 = GetItemID(Item_cailiao2)

    local R1 = 0
    local R2 = 0

    R1 = RemoveChaItem(role, ItemID_Cailiao1, 1, 2, ItemBag[1], 2, 1, 0)
    R2 = RemoveChaItem(role, ItemID_Cailiao2, 1, 2, ItemBag[2], 2, 1, 0)

    if R1 == 0 or R2 == 0 then
        LG("Damo", "Delete resource failed")
    end

    local Sklv = 1

    local b = Check_CG_damo(Item_damo, Sklv)
    if b == 0 then
        Damo_Shibai(role, Item_damo)
        return 2
    end

    Damo_ChengGong(role, Item_damo)
    local cha_name = GetChaDefaultName(role)
    LG("JingLian_ShiBai", "Player" .. cha_name .. "Fusion successful")

    return 1
end

function Check_Jiaguji(Item_Cailiao1, Item_Cailiao2)
    local ItemID_Cailiao1 = GetItemID(Item_Cailiao1)
    local ItemID_Cailiao2 = GetItemID(Item_Cailiao2)

    if ItemID_Cailiao1 == 890 then
        return 1
    elseif ItemID_Cailiao2 == 890 then
        return 1
    end

    return 0
end

function Check_Cuihuafen(Item_Cailiao1, Item_Cailiao2)
    local ItemID_Cailiao1 = GetItemID(Item_Cailiao1)
    local ItemID_Cailiao2 = GetItemID(Item_Cailiao2)

    if ItemID_Cailiao1 == 891 then
        return 1
    elseif ItemID_Cailiao2 == 891 then
        return 1
    end

    return 0
end
function Check_HasHole(Item_damo)
    local Num = GetItemForgeParam(Item_damo, 1)
    Num = TansferNum(Num)
    local Sockets = GetNum_Part1(Num)
    return Sockets
end
function Check_CG_damo(Item, SkillLevel)
    local Chance = 0
    local Sockets = Check_HasHole(Item)
    if Sockets == 0 then
        Chance = 1.00
    elseif Sockets == 1 then
        Chance = 1.00
    elseif Sockets == 2 then
        Chance = 1.00
    end
    Chance = Percentage_Random(Chance)
    return Chance
end

function Damo_Shibai(role, Item_damo)
    local cha_name = GetChaDefaultName(role)
    LG("JingLian_ShiBai", "Player" .. cha_name .. "Fusion failed")
    SystemNotice(role, "Fusion failed. Luckily item is still intact")
end

function Damo_ChengGong(role, Item_damo)
    local Num = TansferNum(GetItemForgeParam(Item_damo, 1))
    local Sockets = GetNum_Part1(Num)
    if Sockets <= 3 then
        SystemNotice(role, "Fusion successful. Obtain new socket in equipment.")
        Sockets = Sockets + 1
    else
        SystemNotice(role, "Item sockets max. Unable to make new socket")
    end
    Num = SetNum_Part1(Num, Sockets)

    local A = SetItemForgeParam(Item_damo, 1, Num)
    if A == 0 then
        LG("Damo", "Set forging content failed.")
    end
end

function Delete_Forge_Eff(role, Item_damo)
    local Jinglian_Lv = GetItem_JinglianLv(Item_damo)
    if Jinglian_Lv == 0 then
        return
    end

    local Num = GetItemForgeParam(Item_damo, 1)

    local Item_Stone = {}
    local Item_StoneLv = {}

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    local j = 0
    local Del = 0
    for j = 2, 0, -1 do
        if Del == 0 then
            if Item_Stone[j] ~= 0 or Item_StoneLv[j] ~= 0 then
                Item_Stone[j] = 0
                Item_StoneLv[j] = 0
                Del = 1
            end
        end
    end

    Num = SetNum_Part2(Num, Item_Stone[0])
    Num = SetNum_Part4(Num, Item_Stone[1])
    Num = SetNum_Part6(Num, Item_Stone[2])

    Num = SetNum_Part3(Num, Item_StoneLv[0])
    Num = SetNum_Part5(Num, Item_StoneLv[1])
    Num = SetNum_Part7(Num, Item_StoneLv[2])

    local i = 0
    i = SetItemForgeParam(Item_damo, 1, Num)
    if i == 0 then
        LG("Damo", "set forging content failed")
    end

    SystemNotice(role, "Forging bonus effect ended")
end

function can_fusion_item(...)
	--[[
    local arg = {...}
    if #arg ~= 12 and #arg ~= 14 then
        return 0
    end
    if can_fusion_item_main(arg) == 1 then
        return 1
    else
        return 0
    end
	--]]
	return 0
end
function can_fusion_item_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    if ItemCount[1] ~= 1 or ItemCount[2] ~= 1 or ItemBagCount[1] ~= 1 or ItemBagCount[2] ~= 1 then
        return 0
    end
    local Apparel = GetChaItem(Player, 2, ItemBag[1])
    local Equipment = GetChaItem(Player, 2, ItemBag[2])
    local EquipmentID = GetItemID(Equipment)
    local EquipmentType = GetItemType(Equipment)
    local ApparelType = GetItemType(Apparel)
    if GetItemType(GetChaItem(Player, 2, ItemBag[0])) ~= 60 then
        SystemNotice(Player, "Scroll usage error")
        return 0
    end
    if GetItemAttr(Apparel, ITEMATTR_MAXURE) ~= 23000 then
        return 0
    end
    if GetItemAttr(Apparel, ITEMATTR_URE) < GetItemAttr(Apparel, ITEMATTR_MAXURE) then
        SystemNotice(Player, "Apparel must have full durability.")
        return 0
    end
    if GetItemAttr(Equipment, ITEMATTR_VAL_FUSIONID) == 0 and GetItemAttr(Equipment, ITEMATTR_MAXURE) == 25000 then
        SystemNotice(Player, "Equipment on right slot has not attribute. Unable to fuse!")
        return 0
    end

    if ItemBagCount[3] == 0 then
        local Slot = GetChaItem(Player, 2, ItemBag[2])
        local HasGem = GetItemForgeParam(Slot, 1)
        if HasGem ~= 0 then
            BickerNotice(Player, "Your equipment have gems and sockets. Use Fusion Catalyst to continue.")
            return 0
        end
    end

    if EquipmentType == 27 then
        EquipmentType = 22
    end
    if ApparelType == 27 then
        ApparelType = 22
    end
    if EquipmentType ~= ApparelType then
        SystemNotice(Player, "Apparel and Equipment type are not the same.")
        return 0
    end
    if CheckFusionItem(Apparel, Equipment) == LUA_FALSE then
        SystemNotice(Player, "Both equipment type or class requirement does not match")
        return 0
    end
    if getfusion_money_main(Table) > GetChaAttr(Player, ATTR_GD) then
        SystemNotice(Player, "Insufficient gold. Unable to undergo Fusion.")
        return 0
    end
    return 1
end
function begin_fusion_item(...)
    local arg = {...}
    if can_fusion_item_main(arg) == 0 then
        return 0
    end
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)
    SetCharaAttr((GetChaAttr(Player, ATTR_GD) - getfusion_money_main(arg)), Player, ATTR_GD)
    ALLExAttrSet(Player)
    if ronghe_item(arg) == 0 then
        SystemNotice(Player, "Fusion failed, please check program.")
    end
    SynChaKitbag(Player, 4)
    SynChaKitbag(Player, 13)
    RefreshCha(Player)
    if HasMission(Player, 1994) == 1 then
        Checkfusion[GetChaDefaultName(Player)] = 1
    end
    return 1
end
function get_item_fusion_money(...)
    local arg = {...}
    return (getfusion_money_main(arg))
end
function getfusion_money_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local Equipment = GetChaItem(Player, 2, ItemBag[2])
    local Money_Need = GetItemOriginalLv(Equipment) * 1000
    return Money_Need
end
function ronghe_item(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local Scroll = GetChaItem(Player, 2, ItemBag[0])
    local Apparel = GetChaItem(Player, 2, ItemBag[1])
    local Equipment = GetChaItem(Player, 2, ItemBag[2])
    local EquipmentID = GetItemID(Equipment)
    local Sockets = Check_HasHole(Equipment)

    local Forge = GetItemForgeParam(Equipment, 1)
    if GetItemAttr(Equipment, ITEMATTR_MAXURE) == 25000 then
        EquipmentID = GetItemAttr(Equipment, ITEMATTR_VAL_FUSIONID)

        SetItemAttr(Apparel, ITEMATTR_VAL_FUSIONID, EquipmentID)
    else
        SetItemAttr(Apparel, ITEMATTR_VAL_FUSIONID, EquipmentID)
    end
    if FusionItem(Apparel, Equipment) == 0 then
        SystemNotice(Player, "Fusion failed.")
        return
    end
    if ItemBagCount[3] ~= 0 then
        local CatalystID = GetItemID(GetChaItem(Player, 2, ItemBag[3]))
        if GetItemAttr(Equipment, ITEMATTR_MAXURE) < 25000 then
            SetItemAttr(Apparel, ITEMATTR_VAL_LEVEL, 10)
        else
            SetItemAttr(Apparel, ITEMATTR_VAL_LEVEL, (GetItemAttr(Equipment, ITEMATTR_VAL_LEVEL)))
        end
    else
        SetItemAttr(Apparel, ITEMATTR_VAL_LEVEL, 10)
    end
    SetItemAttr(Apparel, ITEMATTR_MAXURE, 25000)
    SetItemAttr(Apparel, ITEMATTR_URE, 25000)
    RemoveChaItem(Player, GetItemID(Scroll), 1, 2, ItemBag[0], 2, 1, 0)
    RemoveChaItem(Player, EquipmentID, 1, 2, ItemBag[2], 2, 1, 0)
    if ItemBagCount[3] ~= 0 then
        RemoveChaItem(Player, CatalystID, 1, 2, ItemBag[3], 2, 1, 0)
        SetItemForgeParam(Apparel, 1, Forge)
    else
        local Part1_Forge = GetNum_Part1(Forge)
        local Part2_Forge = GetNum_Part2(Forge)
        local Part3_Forge = GetNum_Part3(Forge)
        local Part4_Forge = GetNum_Part4(Forge)
        local Part5_Forge = GetNum_Part5(Forge)
        local Part6_Forge = GetNum_Part6(Forge)
        local Part7_Forge = GetNum_Part7(Forge)
        Forge = SetNum_Part1(Forge, Sockets)
        Forge = SetNum_Part2(Forge, 0)
        Forge = SetNum_Part3(Forge, 0)
        Forge = SetNum_Part4(Forge, 0)
        Forge = SetNum_Part5(Forge, 0)
        Forge = SetNum_Part6(Forge, 0)
        Forge = SetNum_Part7(Forge, 0)
        SetItemForgeParam(Apparel, 1, Forge)
    end
end

function can_upgrade_item(...)
    local arg = {...}
    if #arg ~= 12 then
        SystemNotice(arg[1], "Illegal parameter value." .. #arg)
        return 0
    end
    if can_beuplv_item_main(arg) == 1 then
        return 1
    else
        return 0
    end
end
function can_beuplv_item_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    local Item = GetChaItem(Player, 2, ItemBag[1])
    local ItemID = GetItemID(Item)
    local Scroll = GetChaItem(Player, 2, ItemBag[0])
    local Crystal = GetChaItem(Player, 2, ItemBag[2])
    local ScrollID = GetItemID(Scroll)
    local CrystalID = GetItemID(Crystal)
    if KitbagLock(Player, 0) == LUA_FALSE then
        SystemNotice(Player, "Your inventory must be unlocked.")
        return 0
    end
    if Get_Itembeuplv_Lv(Item) >= Server.Equipment.Upgrade.Limit then
        SystemNotice(Player, "Item has reached maximum upgrade limit.")
        return 0
    end
    if ItemBagCount[1] ~= 1 then
        SystemNotice(Player, "Strengthening item level slot illegal.")
        return 0
    end
    if GetItemType(Scroll) ~= 62 or GetItemType(Crystal) ~= 63 then
        SystemNotice(Player, "Invalid Strengthening Equipment/Crystal type.")
        return 0
    end
    if GetItemAttr(Item, ITEMATTR_MAXURE) == 25000 then
        SystemNotice(Player, "You cannot strengthen an apparel")
        return 0
    end
    if ItemCount[1] ~= 1 then
        SystemNotice(Player, "Illegal force item upgrade number.")
        return 0
    end
    if CheckItem_CanJinglian(Item) ~= 1 then
        SystemNotice(Player, "Equipment can not be strengthened.")
        return 0
    end
    if ItemCount[0] ~= 1 or ItemCount[2] ~= 1 or ItemBagCount[0] ~= 1 or ItemBagCount[2] ~= 1 then
        SystemNotice(Player, "Item mall items or game items error.")
        return 0
    end
    if getupgrade_money_main(Table) > GetChaAttr(Player, ATTR_GD) then
        SystemNotice(Player, "Insufficient gold, unable to increase equipment levels.")
        return 0
    end
    return 1
end
function ApparelUpg_Success_Rate(Player, Item_Lv)
    local A = 0
    local B = 0
    appUpgRate = {}
    appUpgRate[0] = 1
    appUpgRate[1] = .9
    appUpgRate[2] = .8
    appUpgRate[3] = .7
    appUpgRate[4] = .6
    appUpgRate[5] = .5
    appUpgRate[6] = .4
    appUpgRate[7] = .3
    appUpgRate[8] = .2
    appUpgRate[9] = .1
    if appUpgRate[Item_Lv] ~= nil then
        --SystemNotice(Player, "Apparel Level = " ..(Item_Lv + 1).. "\t Rate = " .. appUpgRate[Item_Lv])
        A = appUpgRate[Item_Lv]
        B = Percentage_Random(A)
    else
        B = 0
    end
    return B
end
function begin_upgrade_item(...)
    local arg = {...}
    if can_beuplv_item_main(arg) == 0 then
        return 0
    end
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)
    local Item = GetChaItem(Player, 2, ItemBag[1])
    local ScrollID = GetItemID(GetChaItem(Player, 2, ItemBag[0]))
    local CrystalID = GetItemID(GetChaItem(Player, 2, ItemBag[2]))
    local ItemLevel = GetItemAttr(Item, ITEMATTR_VAL_LEVEL)
    SetCharaAttr((GetChaAttr(Player, ATTR_GD) - getupgrade_money_main(arg)), Player, ATTR_GD)
    ALLExAttrSet(Player)
    local Succes_Rate = ApparelUpg_Success_Rate(Player, ItemLevel)
    if Succes_Rate == 1 then
        RemoveChaItem(Player, ScrollID, 1, 2, ItemBag[0], 2, 1, 0)
        RemoveChaItem(Player, CrystalID, 1, 2, ItemBag[2], 2, 1, 0)
        SetChaKitbagChange(Player, 1)
        Set_Itembeuplv_Lv(Item, (ItemLevel + 1))
        SynChaKitbag(Player, 4)
        SynChaKitbag(Player, 13)
        RefreshCha(Player)
        SystemNotice(Player, "Equipment upgrade successful.")
        return 1
    else
        RemoveChaItem(Player, ScrollID, 1, 2, ItemBag[0], 2, 1, 0)
        RemoveChaItem(Player, CrystalID, 1, 2, ItemBag[2], 2, 1, 0)
        SystemNotice(Player, "Equipment upgrade failed. Better luck next time!")
        return
    end
end
function Get_Itembeuplv_Lv(Item)
    local Lv = GetItemAttr(Item, ITEMATTR_VAL_LEVEL)
    return Lv
end
function Set_Itembeuplv_Lv(Item, Item_Lv)
    local i = 0
    i = SetItemAttr(Item, ITEMATTR_VAL_LEVEL, Item_Lv)
    if i == 0 then
        LG("Hecheng_BS", "Failed to set gem level")
    end
end
function get_item_upgrade_money(...)
    local arg = {...}
    local Money = getupgrade_money_main(arg)
    return Money
end
function getupgrade_money_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local Waiguan_Lv = 0
    local Waiguan_Lv = Get_Itembeuplv_Lv(GetChaItem(Player, 2, ItemBag[1]))
    local Money_Need = (Waiguan_Lv + 1) * (Waiguan_Lv + 1) * Server.Equipment.Upgrade.Cost
    return Money_Need
end

function can_jlborn_item(...)
    local arg = {...}
    if #arg ~= 12 then
        SystemNotice(arg[1], "parameter value illegal" .. #arg)
        return 0
    end
    local Check = 0
    Check = can_jlborn_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end
function can_jlborn_item_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    if ItemCount[1] ~= 1 or ItemCount[2] ~= 1 or ItemBagCount[1] ~= 1 or ItemBagCount[2] ~= 1 then
        SystemNotice(Player, "Equipment quantity illegal.")
        return 0
    end
    local MarriageFruit = GetChaItem(Player, 2, ItemBag[0])

    local Fairy1 = GetChaItem(Player, 2, ItemBag[1])
    local Fairy1_ID = GetItemID(Fairy1)
    local HP_Fairy1 = GetItemAttr(Fairy1, ITEMATTR_URE)
    local MxHP_Fairy1 = GetItemAttr(Fairy1, ITEMATTR_MAXURE)
    local Lv_Fairy1 = GetFairyLevel(Fairy1)
    local Num_Fairy1 = GetItemForgeParam(Fairy1, 1)
    local Part1_Fairy1 = GetNum_Part1(Num_Fairy1)

    local Fairy2 = GetChaItem(Player, 2, ItemBag[2])
    local Fairy2_ID = GetItemID(Fairy2)
    local HP_Fairy2 = GetItemAttr(Fairy2, ITEMATTR_URE)
    local MxHP_Fairy2 = GetItemAttr(Fairy2, ITEMATTR_MAXURE)
    local Lv_Fairy2 = GetFairyLevel(Fairy2)
    local Num_Fairy2 = GetItemForgeParam(Fairy2, 1)
    local Part1_Fairy2 = GetNum_Part1(Num_Fairy2)
    if GetChaFreeBagGridNum(Player) < 2 then
        SystemNotice(Player, "You need at least two slots free.")
        return 0
    end
    local MarriageFruit_ID = GetItemID(MarriageFruit)
    if
        MarriageFruit_ID ~= 3918 and MarriageFruit_ID ~= 3919 and MarriageFruit_ID ~= 3920 and MarriageFruit_ID ~= 3921 and
            MarriageFruit_ID ~= 3922 and
            MarriageFruit_ID ~= 3924 and
            MarriageFruit_ID ~= 3925
     then
        SystemNotice(Player, "Please insert a Demonic Fruit.")
        return 0
    end
    for i = 1, #Server.Fairy.Marriage do
        if MarriageFruit_ID == Server.Fairy.Marriage[i].ID then
            if
                CheckBagItem(Player, Server.Fairy.Marriage[i].Item1) < 10 or
                    CheckBagItem(Player, Server.Fairy.Marriage[i].Item2) < 10
             then
                SystemNotice(Player, "You do not posses the required items to marry fairies.")
                return 0
            end
        end
    end
    if GetItemType(Fairy1) ~= 59 or GetItemType(Fairy2) ~= 59 then
        SystemNotice(Player, "You did not insert a pet fairy.")
        return 0
    end
    if ItemBag[1] == ItemBag[2] then
        SystemNotice(Player, "My dear child, how can one marry oneself?")
        return 0
    end
    if Part1_Fairy1 ~= 0 or Part1_Fairy2 ~= 0 then
        SystemNotice(Player, "Only normal fairy can get married at the moment")
        return 0
    end
    if Lv_Fairy1 < 20 or Lv_Fairy2 < 20 then
        SystemNotice(Player, "Both fairies need to bee at least Lv20 to marry.")
        return 0
    end
    if HP_Fairy1 < MxHP_Fairy1 or HP_Fairy2 < MxHP_Fairy2 then
        SystemNotice(Player, "Both fairies stamina must be full.")
        return 0
    end
    if getjlborn_money_main(Table) > GetChaAttr(Player, ATTR_GD) then
        SystemNotice(Player, "Not enough gold to marry pets.")
        return 0
    end
    return 1
end
function begin_jlborn_item(...)
    local arg = {...}

    local Check_Canjlborn = 0
    Check_Canjlborn = can_jlborn_item_main(arg)
    if Check_Canjlborn == 0 then
        return 0
    end

    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)

    local Item_EMstone = GetChaItem(role, 2, ItemBag[0])
    local Item_JLone = GetChaItem(role, 2, ItemBag[1])
    local Item_JLother = GetChaItem(role, 2, ItemBag[2])

    local Money_Need = getjlborn_money_main(arg)
    local Money_Have = GetChaAttr(role, ATTR_GD)
    Money_Have = Money_Have - Money_Need
    SetCharaAttr(Money_Have, role, ATTR_GD)
    ALLExAttrSet(role)

    Check_JLBorn_Item = jlborn_item(arg)
    if Check_JLBorn_Item == 0 then
        SystemNotice(role, "Marriage has failed. Please check procedure")
    end
    local cha_name = GetChaDefaultName(role)
    SystemNotice(role, "Marriage successful")
    LG("JLBorn_ShiBai", "Player" .. cha_name .. "'s pet fairy has gotten married successfully")
    return 1
end
function get_item_jlborn_money(...)
    local arg = {...}
    local Money = getjlborn_money_main(arg)
    return Money
end
function getjlborn_money_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_JLone = GetChaItem(role, 2, ItemBag[1])
    local Item_JLother = GetChaItem(role, 2, ItemBag[2])

    local str_JLone = GetItemAttr(Item_JLone, ITEMATTR_VAL_STR)
    local con_JLone = GetItemAttr(Item_JLone, ITEMATTR_VAL_CON)
    local agi_JLone = GetItemAttr(Item_JLone, ITEMATTR_VAL_AGI)
    local dex_JLone = GetItemAttr(Item_JLone, ITEMATTR_VAL_DEX)
    local sta_JLone = GetItemAttr(Item_JLone, ITEMATTR_VAL_STA)
    local lv_JLone = str_JLone + con_JLone + agi_JLone + dex_JLone + sta_JLone

    local str_JLother = GetItemAttr(Item_JLother, ITEMATTR_VAL_STR)
    local con_JLother = GetItemAttr(Item_JLother, ITEMATTR_VAL_CON)
    local agi_JLother = GetItemAttr(Item_JLother, ITEMATTR_VAL_AGI)
    local dex_JLother = GetItemAttr(Item_JLother, ITEMATTR_VAL_DEX)
    local sta_JLother = GetItemAttr(Item_JLother, ITEMATTR_VAL_STA)
    local lv_JLother = str_JLother + con_JLother + agi_JLother + dex_JLother + sta_JLother
    local Money_Need = (60 - lv_JLone) * (60 - lv_JLother) * 100
    if lv_JLone > 60 or lv_JLother > 60 then
        Money_Need = 0
    end
    return Money_Need
end
function jlborn_item(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local MarriageFruit = GetChaItem(Player, 2, ItemBag[0])
    local MarriageFruitID = GetItemID(MarriageFruit)

    local Fairy1 = GetChaItem(Player, 2, ItemBag[1])
    local Fairy1_ID = GetItemID(Fairy1)
    local HP_Fairy1 = GetItemAttr(Fairy1, ITEMATTR_URE)
    local MxHP_Fairy1 = GetItemAttr(Fairy1, ITEMATTR_MAXURE)
    local Lv_Fairy1 = GetFairyLevel(Fairy1)
    local Fairy1_STR, Fairy1_CON, Fairy1_AGI, Fairy1_ACC, Fairy1_SPR = GetFairyStats(Fairy1)

    local Fairy2 = GetChaItem(Player, 2, ItemBag[2])
    local Fairy2_ID = GetItemID(Fairy2)
    local HP_Fairy2 = GetItemAttr(Fairy2, ITEMATTR_URE)
    local MxHP_Fairy2 = GetItemAttr(Fairy2, ITEMATTR_MAXURE)
    local Lv_Fairy2 = GetFairyLevel(Fairy2)
    local Fairy2_STR, Fairy2_CON, Fairy2_AGI, Fairy2_ACC, Fairy2_SPR = GetFairyStats(Fairy2)

    local FairyNew_STR = math.floor((Fairy1_STR + Fairy2_STR) * 0.125)
    local FairyNew_CON = math.floor((Fairy1_CON + Fairy2_CON) * 0.125)
    local FairyNew_AGI = math.floor((Fairy1_AGI + Fairy2_AGI) * 0.125)
    local FairyNew_ACC = math.floor((Fairy1_ACC + Fairy2_ACC) * 0.125)
    local FairyNew_SPR = math.floor((Fairy1_SPR + Fairy2_SPR) * 0.125)
    local Lv_NewFairy = FairyNew_STR + FairyNew_CON + FairyNew_AGI + FairyNew_ACC + FairyNew_SPR
   
	local FairyNew_Growth = 240 * (Lv_NewFairy + 1)
    if FairyNew_Growth > 6480 then
        FairyNew_Growth = 6480
    end
    local FairyNew_MxHP = 5000 + 1000 * Lv_NewFairy
    if FairyNew_MxHP > 32000 then
        FairyNew_MxHP = 32000
    end
    if FairyNew_MxHP == 25000 then
        FairyNew_MxHP = 25000 + 1
    end
    for i = 1, #Server.Fairy.Marriage do
        if MarriageFruitID == Server.Fairy.Marriage[i].ID then
            if TakeItem(Player, 0, Server.Fairy.Marriage[i].Item1, 10) == 0 or TakeItem(Player, 0, Server.Fairy.Marriage[i].Item2, 10) == 0 then
                SystemNotice(Player, "Could not remove Fairy Marriage items.")
                return
            end
            local Random = math.random(1, 100)
            local r1 = 0
            local r2 = 0
            if Fairy1_ID == Server.Fairy.ID["Mordo"] or Fairy2_ID == Server.Fairy.ID["Mordo"] then
                if Fairy1_ID == Fairy2_ID then
                    r1, r2 = MakeItem(Player, 910, 1, 4)
                elseif Lv_Fairy1 >= 20 and Lv_Fairy1 < 25 and Lv_Fairy2 >= 20 and Lv_Fairy2 < 25 and Random >= 88 then
                    r1, r2 = MakeItem(Player, 910, 1, 4)
                elseif Lv_Fairy1 >= 25 and Lv_Fairy1 < 35 and Lv_Fairy2 >= 25 and Lv_Fairy2 < 35 and Random >= 50 then
                    r1, r2 = MakeItem(Player, 910, 1, 4)
                elseif Lv_Fairy1 >= 35 and Lv_Fairy2 >= 35 and Random >= 10 then
                    r1, r2 = MakeItem(Player, 910, 1, 4)
                else
                    r1, r2 = MakeItem(Player, Server.Fairy.Marriage[i].Fairy, 1, 4)
                end
            elseif Fairy1_ID == Server.Fairy.ID["Angela"] or Fairy2_ID == Server.Fairy.ID["Angela"] then
                if Fairy1_ID == Fairy2_ID then
                    r1, r2 = MakeItem(Player, 7131, 1, 4)
                elseif Lv_Fairy1 >= 20 and Lv_Fairy1 < 25 and Lv_Fairy2 >= 20 and Lv_Fairy2 < 25 and Random >= 88 then
                    r1, r2 = MakeItem(Player, 7131, 1, 4)
                elseif Lv_Fairy1 >= 25 and Lv_Fairy1 < 35 and Lv_Fairy2 >= 25 and Lv_Fairy2 < 35 and Random >= 50 then
                    r1, r2 = MakeItem(Player, 7131, 1, 4)
                elseif Lv_Fairy1 >= 35 and Lv_Fairy2 >= 35 and Random >= 10 then
                    r1, r2 = MakeItem(Player, 7131, 1, 4)
                else
                    r1, r2 = MakeItem(Player, Server.Fairy.Marriage[i].Fairy, 1, 4)
                end
            else
                r1, r2 = MakeItem(Player, Server.Fairy.Marriage[i].Fairy, 1, 4)
            end
        end
    end
	local Param = 0
	if Lv_Fairy1 >= 20 and Lv_Fairy2 >= 20 then
		Param = 1
	end
	if Lv_Fairy1 >= 25 and Lv_Fairy2 >= 25 then
		Param = 2
	end
	if Lv_Fairy1 >= 35 and Lv_Fairy2 >= 35 then
		Param = 3
	end
	local Random = math.random(1, 100)
	if Param == 1 then
		if Random <= 90 then
			GiveItem(Player, 0, 239, 1, 4)			-- 90% Novice Fairy Possession
		elseif Random > 90 and Random <= 98 then       
			GiveItem(Player, 0, 608, 1, 4)			-- 8% Standard Fairy Possession
		elseif Random > 98 and Random <= 100 then      
			GiveItem(Player, 0, 609, 1, 4)			-- 2% Expert Fairy Possession
		end                                            
	end                                                
	if Param == 2 then                                 
		if Random <= 95 then                           
			GiveItem(Player, 0, 608, 1, 4)			-- 95% Standard Fairy Possession
		elseif Random > 95 and Random <= 100 then      
			GiveItem(Player, 0, 609, 1, 4)			-- 5% Expert Fairy Possession
		end                                            
	end                                                
	if Param == 3 then                                 
		GiveItem(Player, 0, 609, 1, 4)				-- Expert Fairy Possession
	end
    R1 = RemoveChaItem(Player, MarriageFruitID, 1, 2, ItemBag[0], 2, 1, 0)
    if R1 == 0 then
        SystemNotice(Player, "Deletion of Demonic Fruit failed.")
        return
    end
    Elf_Attr_cs(Player, Fairy1, Fairy2)
end
function can_tichun_item(...)
    local arg = {...}
    if #arg ~= 10 and #arg ~= 14 then
        SystemNotice(arg[1], "Parameter value illegal." .. #arg)
        return 0
    end
    local Check = 0
    Check = can_tichun_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end
function can_tichun_item_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)
    if ItemCount[0] ~= 1 or ItemCount[1] ~= 1 or ItemBagCount[0] ~= 1 or ItemBagCount[1] ~= 1 then
        SystemNotice(Player, "Equipment quantity illegal.")
        return 0
    end
    local ItemMain = GetChaItem(Player, 2, ItemBag[0])
    local ItemCatalyst = GetChaItem(Player, 2, ItemBag[1])
    local ItemType_mainitem = GetItemType(ItemMain)
    local ItemType_otheritem = GetItemType(ItemCatalyst)
    local ItemMainID = GetItemID(ItemMain)
    local ItemCatalystID = GetItemID(ItemCatalyst)
    local ItemMainID_Lv = GetItemOriginalLv(ItemMain)
    local ItemCatalystID_Lv = GetItemOriginalLv(ItemCatalyst)
    local MainID = ItemMainID
    local CatalystID = ItemCatalystID
    if GetItemAttr(Equipment, ITEMATTR_MAXURE) == 25000 then
        MainID = GetItemAttr(ItemMain, ITEMATTR_VAL_FUSIONID)
    end
    local Check = 0
    for i = 1, #Upgrade, 1 do
        if MainID == Upgrade[i].ID then
            if CatalystID == Upgrade[i].Catalyst then
                Check = 1
            end
        end
    end
    if Check == 0 then
        SystemNotice(Player, "Please use the correct equipment and upgrade stone.")
        return 0
    end
    if gettichun_money_main(Table) > GetChaAttr(Player, ATTR_GD) then
        SystemNotice(Player, "Insufficient gold, unable to upgrade equipment.")
        return 0
    end
    return 1
end
function begin_tichun_item(...)
    local arg = {...}
    local Check_Cantichun = 0
    Check_Cantichun = can_tichun_item_main(arg)
    if Check_Cantichun == 0 then
        return 0
    end
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)
    local ItemMain = GetChaItem(Player, 2, ItemBag[0])
    local ItemCatalyst = GetChaItem(Player, 2, ItemBag[1])
    local Money_Need = gettichun_money_main(arg)
    TakeMoney(Player, nil, Money_Need)
    Check_TiChun_Item = tichun_item(arg)
    if Check_TiChun_Item == 0 then
        SystemNotice(Player, "Upgrade failed, please check the script.")
    end
    return 1
end
function get_item_tichun_money(...)
    local arg = {...}
    local Money = gettichun_money_main(arg)
    return Money
end
function gettichun_money_main(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local ItemMain = GetChaItem(Player, 2, ItemBag[0])
    local ItemMainLv = GetItemOriginalLv(ItemMain)
    local Money_Need = Upgrade.Money
    return Money_Need
end
function tichun_item(Table)
    local Player = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0
    Player, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)
    local ItemMain = GetChaItem(Player, 2, ItemBag[0])
    local ItemCatalyst = GetChaItem(Player, 2, ItemBag[1])
    local ItemType_mainitem = GetItemType(ItemMain)
    local ItemType_otheritem = GetItemType(ItemCatalyst)
    local ItemMainID = GetItemID(ItemMain)
    local ItemCatalystID = GetItemID(ItemCatalyst)
    local ItemMainID_Lv = GetItemOriginalLv(ItemMain)
    local ItemCatalystID_Lv = GetItemOriginalLv(ItemCatalyst)
    local MainID = ItemMainID
    local CatalystID = ItemCatalystID
    if GetItemAttr(Equipment, ITEMATTR_MAXURE) == 25000 then
        MainID = GetItemAttr(ItemMain, ITEMATTR_VAL_FUSIONID)
    end
    local Forge = GetItemForgeParam(ItemMain, 1)
    local r1 = 0
    local r2 = 0
    local ItemEnergy = GetItemAttr(ItemMain, ITEMATTR_ENERGY)
    local ItemQuality = 12
    for k = 1, #Upgrade do
        if MainID == Upgrade[k].ID and CatalystID == Upgrade[k].Catalyst then
            if ItemEnergy < 1000 then
                ItemQuality = 2
            elseif ItemEnergy >= 1000 and ItemEnergy < 2000 then
                ItemQuality = 12
            elseif ItemEnergy >= 2000 and ItemEnergy < 3000 then
                ItemQuality = 13
            elseif ItemEnergy >= 3000 and ItemEnergy < 4000 then
                ItemQuality = 14
            elseif ItemEnergy >= 4000 and ItemEnergy < 5000 then
                ItemQuality = 15
            elseif ItemEnergy >= 5000 and ItemEnergy < 6000 then
                ItemQuality = 16
            elseif ItemEnergy >= 6000 and ItemEnergy < 7000 then
                ItemQuality = 17
            elseif ItemEnergy >= 7000 and ItemEnergy < 8000 then
                ItemQuality = 18
            elseif ItemEnergy >= 8000 and ItemEnergy < 9000 then
                ItemQuality = 19
            elseif ItemEnergy >= 9000 and ItemEnergy < 10000 then
                ItemQuality = 20
            end
            MainID = Upgrade[k].Result
            r1, r2 = MakeItem(Player, MainID, 1, ItemQuality)
        end
    end
    local RemMain = 0
    local RemCata = 0
    RemMain = RemoveChaItem(Player, ItemMainID, 1, 2, ItemBag[0], 2, 1, 1)
    RemCata = RemoveChaItem(Player, ItemCatalystID, 1, 2, ItemBag[1], 2, 1, 1)
    if RemMain == 0 or RemCata == 0 then
        SystemNotice(Player, "Remove item failed.")
        return
    end
    local NewItem = GetChaItem(Player, 2, r2)
    local CheckForge = SetItemForgeParam(NewItem, 1, Forge)
    if CheckForge == 0 then
        SystemNotice(Player, "Fail to set forging attribute settings.")
        return
    end
    Notice("Congratulations, player "..GetChaDefaultName(Player).." upgraded ["..GetItemName(ItemMainID).."] with [" .. GetItemName(ItemCatalystID) .. "] and obtained [" .. GetItemName(MainID) .. "].")
    LG("Upgrade System", "Player: ["..GetChaDefaultName(Player).."], Equipment: ["..GetItemName(ItemMainID).."], Catalyst: [" .. GetItemName(ItemCatalystID) .. "], Result: ["..GetItemName(MainID).."]")
    SynChaKitbag(Player, 13)
end

function can_energy_item(...)
    local arg = {...}

    if #arg ~= 10 and #arg ~= 14 then
        SystemNotice(arg[1], "parameter value illegal" .. #arg)
        return 0
    end
    local Check = 0
    Check = can_energy_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end

function can_energy_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)

    if ItemCount[0] ~= 1 or ItemCount[1] ~= 1 or ItemBagCount[0] ~= 1 or ItemBagCount[1] ~= 1 then
        SystemNotice(role, "equipment quantity illegal ")
        return 0
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 slot in your inventory")
        UseItemFailed(role)
        return
    end

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local Item_mainitem_Lv = GetItemOriginalLv(Item_mainitem)

    local item_energy = GetItemAttr(Item_mainitem, ITEMATTR_ENERGY)
    local item_maxenergy = GetItemAttr(Item_mainitem, ITEMATTR_MAXENERGY)

    if ItemType_mainitem ~= 29 then
        SystemNotice(role, "Only Corals can be recharged")
        return 0
    end

    if ItemID_otheritem ~= 1022 and ItemID_otheritem ~= 1024 then
        SystemNotice(role, "Need Battery to charge")
        return 0
    end

    if item_energy == item_maxenergy then
        SystemNotice(role, "Coral energy is not depleted")
        return 0
    end

    local Money_Need = get_item_energy_money(Table)
    local Money_Have = GetChaAttr(role, ATTR_GD)
    if Money_Need > Money_Have then
        SystemNotice(role, "Not enough Gold. Unable to charge Coral")
        return 0
    end

    return 1
end

function begin_energy_item(...)
    local arg = {...}

    local Check_Canenergy = 0
    Check_Canenergy = can_energy_item_main(arg)
    if Check_Canenergy == 0 then
        return 0
    end

    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local Money_Need = get_item_energy_money(arg)
    local Money_Have = GetChaAttr(role, ATTR_GD)

    TakeMoney(role, nil, Money_Need)

    Check_Energy_Item = energy_item(arg)
    if Check_Energy_Item == 0 then
        SystemNotice(role, "Coral recharge fail. Please check your procedure.")
    end

    return 1
end

function get_item_energy_money(...)
    local arg = {...}

    local Money = energy_money_main(arg)
    return Money
end

function energy_money_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemID_otheritem = GetItemID(Item_otheritem)

    if ItemID_otheritem == 1022 then
        Money_Need = 300
    else
        Money_Need = 1000
    end

    return Money_Need
end

function energy_item(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local item_energy = GetItemAttr(Item_mainitem, ITEMATTR_ENERGY)

    local item_maxenergy = GetItemAttr(Item_mainitem, ITEMATTR_MAXENERGY)

    local energy_differ = 0
    local star = math.random(1, 20)

    if ItemID_otheritem == 1022 then
        energy_differ = star * 50
    else
        energy_differ = 1500
    end

    item_energy = item_maxenergy

    SetItemAttr(Item_mainitem, ITEMATTR_ENERGY, item_energy)

    local cha_name = GetChaDefaultName(role)
    LG("star_CHONGDIAN_lg", cha_name, ItemID_mainitem, ItemID_otheritem)

    local R1 = 0
    R1 = RemoveChaItem(role, Item_otheritem, 1, 2, ItemBag[1], 2, 1, 0)
    if R1 == 0 then
        SystemNotice(role, "moved item failed ")
        return
    end
end

function can_getstone_item(...)
    local arg = {...}

    if #arg ~= 10 and #arg ~= 14 then
        SystemNotice(arg[1], "parameter value illegal" .. #arg)
        return 0
    end
    local Check = 0
    Check = can_getstone_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end

function can_getstone_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)

    if ItemCount[0] ~= 1 or ItemCount[1] ~= 1 or ItemBagCount[0] ~= 1 or ItemBagCount[1] ~= 1 then
        SystemNotice(role, "equipment quantity illegal ")
        return 0
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 slot in your inventory")
        UseItemFailed(role)
        return
    end

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local Item_mainitem_Lv = GetItemOriginalLv(Item_mainitem)

    local Item_Stone = {}
    local Item_StoneLv = {}
    local Jinglianxinxi = GetItemForgeParam(Item_mainitem, 1)
    Jinglianxinxi = TansferNum(Jinglianxinxi)
    Item_Stone[0] = GetNum_Part2(Jinglianxinxi)
    Item_Stone[1] = GetNum_Part4(Jinglianxinxi)
    Item_Stone[2] = GetNum_Part6(Jinglianxinxi)

    Item_StoneLv[0] = GetNum_Part3(Jinglianxinxi)
    Item_StoneLv[1] = GetNum_Part5(Jinglianxinxi)
    Item_StoneLv[2] = GetNum_Part7(Jinglianxinxi)

    local checkstar = CheckItem_CanJinglian(Item_mainitem)
    if checkstar == 0 then
        SystemNotice(role, "Item type mismatch")
        return 0
    end
    if Item_Stone[0] == 0 and Item_Stone[1] == 0 and Item_Stone[2] == 0 then
        SystemNotice(role, "Equipment is not forged with gem")
        return 0
    end

    local flag = 0
    if ItemID_otheritem == 1020 or ItemID_otheritem == 6870 or ItemID_otheritem == 6871 then
        flag = 1
    end

    if flag == 0 then
        SystemNotice(role, "Please use Blacksmith's Pliers")
        return 0
    end

    local Money_Need = getstone_money_main(Table)
    local Money_Have = GetChaAttr(role, ATTR_GD)
    if Money_Need > Money_Have then
        SystemNotice(role, "Insufficient gold. Unable to extract gem")
        return 0
    end

    return 1
end

function begin_getstone_item(...)
    local arg = {...}

    local Check_Cangetstone = 0
    Check_Cangetstone = can_getstone_item_main(arg)
    if Check_Cangetstone == 0 then
        return 0
    end

    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local Money_Need = getstone_money_main(arg)
    local Money_Have = GetChaAttr(role, ATTR_GD)

    TakeMoney(role, nil, Money_Need)

    Check_TiChun_Item = getstone_item(arg)
    if Check_TiChun_Item == 0 then
        SystemNotice(role, "Extraction of gem has failed. Please check your process")
    end

    return 1
end

function get_item_getstone_money(...)
    local arg = {...}

    local Money = getstone_money_main(arg)
    return Money
end

function getstone_money_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])

    local Item_StoneLv = {}
    local Jinglianxinxi = GetItemForgeParam(Item_mainitem, 1)
    Jinglianxinxi = TansferNum(Jinglianxinxi)

    Item_StoneLv[0] = GetNum_Part3(Jinglianxinxi)
    Item_StoneLv[1] = GetNum_Part5(Jinglianxinxi)
    Item_StoneLv[2] = GetNum_Part7(Jinglianxinxi)

    local Money_Need = (Item_StoneLv[0] + Item_StoneLv[1] + Item_StoneLv[2]) * 10000

    return Money_Need
end

function getstone_item(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local Item_mainitem_Lv = GetItemOriginalLv(Item_mainitem)
    local Item_otheritem_Lv = GetItemOriginalLv(Item_otheritem)

    local Num = GetItemForgeParam(Item_mainitem, 1)
    Num = TansferNum(Num)
    local lg_Num = Num

    local Item_Stone = {}
    local Item_StoneLv = {}
    local Item_StoneID = {}

    Item_Stone[0] = GetNum_Part2(Num)
    Item_Stone[1] = GetNum_Part4(Num)
    Item_Stone[2] = GetNum_Part6(Num)

    Item_StoneLv[0] = GetNum_Part3(Num)
    Item_StoneLv[1] = GetNum_Part5(Num)
    Item_StoneLv[2] = GetNum_Part7(Num)

    Item_StoneID[0] = GemVar[Item_Stone[0]].ID
    Item_StoneID[1] = GemVar[Item_Stone[1]].ID
    Item_StoneID[2] = GemVar[Item_Stone[2]].ID

    local r1 = 0
    local r2 = 0
    local Item_Lv = 0
    local item_tureID = 0

    if ItemID_otheritem == 1020 then
        if Item_StoneID[0] ~= 0 then
            item_tureID = Item_StoneID[0]
            Item_Lv = Item_StoneLv[0]
            Item_StoneLv[0] = Item_StoneLv[0] - 1
            if Item_StoneLv[0] == 0 then
                Item_Stone[0] = 0
            end
        else
            SystemNotice(role, "Equipment doesn't posses any gems to extract.")
            return
        end
    end

    if ItemID_otheritem == 6870 then
        if Item_StoneID[1] ~= 0 then
            item_tureID = Item_StoneID[1]
            Item_Lv = Item_StoneLv[1]
            Item_StoneLv[1] = Item_StoneLv[1] - 1
            if Item_StoneLv[1] == 0 then
                Item_Stone[1] = 0
            end
        else
            SystemNotice(role, "There's no gem on 2nd socket of equipment!")
            return
        end
    end

    if ItemID_otheritem == 6871 then
        if Item_StoneID[2] ~= 0 then
            item_tureID = Item_StoneID[2]
            Item_Lv = Item_StoneLv[2]
            Item_StoneLv[2] = Item_StoneLv[2] - 1
            if Item_StoneLv[2] == 0 then
                Item_Stone[2] = 0
            end
        else
            SystemNotice(role, "There's no gem on 3rd socket of equipment!")
            return
        end
    end

    -- r1, r2 = MakeItem(role, item_tureID, 1, 2)
    -- local Item_ture = GetChaItem(role, 2, r2)
    -- SetItemAttr(Item_ture, ITEMATTR_VAL_BaoshiLV, Item_Lv)

    local ItemLevel = Item_Lv + 100
    GiveItem(role, 0, item_tureID, 1, ItemLevel)

    Num = SetNum_Part2(Num, Item_Stone[0])
    Num = SetNum_Part3(Num, Item_StoneLv[0])
    Num = SetNum_Part4(Num, Item_Stone[1])
    Num = SetNum_Part5(Num, Item_StoneLv[1])
    Num = SetNum_Part6(Num, Item_Stone[2])
    Num = SetNum_Part7(Num, Item_StoneLv[2])
    SetItemForgeParam(Item_mainitem, 1, Num)

    local cha_name = GetChaDefaultName(role)
    LG("star_tiqu_lg", cha_name, item_tureID, Item_Lv, lg_Num, Num)

    local R1 = 0
    R1 = RemoveChaItem(role, ItemID_otheritem, 1, 2, ItemBag[1], 2, 1, 0)
    if R1 == 0 then
        SystemNotice(role, "moved item failed ")
        return
    end
end

function can_manufacture_item(...)
    local arg = {...}

    local ItemBagCount = arg[2]

    local Length = ItemBagCount + 3
    if #arg ~= Length then
        Notice("parameter value illegal" .. #arg)
        return 0
    end
    local Check = 0

    Check = can_manufacture_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end

function can_manufacture_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(Table)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 slot in your inventory")
        UseItemFailed(role)
        return
    end
    local i = 0
    local Item = {}
    local ItemID = {}
    local ItemType = {}
    for i = 1, ItemBagCount, 1 do
        Item[i] = GetChaItem(role, 2, ItemBag[i])
        ItemID[i] = GetItemID(Item[i])
        ItemType[i] = GetItemType(Item[i])
    end

    if ItemType[1] ~= 59 then
        SystemNotice(role, "Almighty: My child, please at least look for a fairy.")
        return 0
    end
    local URE_JLone = GetItemAttr(Item[1], ITEMATTR_URE)
    if URE_JLone <= 0 then
        SystemNotice(role, "Almighty: This is too cruel! Please feed your pet fairy!")
        return 0
    end

    local Num_JL = GetItemForgeParam(Item[1], 1)
    Num_JL = TansferNum(Num_JL)
    local Part1_JL = GetNum_Part1(Num_JL)
    local Part2_JL = GetNum_Part2(Num_JL)
    local Part3_JL = GetNum_Part3(Num_JL)
    local Part4_JL = GetNum_Part4(Num_JL)
    local Part5_JL = GetNum_Part5(Num_JL)
    local Part6_JL = GetNum_Part6(Num_JL)
    local Part7_JL = GetNum_Part7(Num_JL)
    local JL_jineng = 0
    local JL_jineng_lv = 0
    local life_lv = 0
    if ItemID[2] == 2300 then
        if Part2_JL == 13 then
            JL_jineng = Part2_JL
            JL_jineng_lv = Part3_JL
        elseif Part4_JL == 13 then
            JL_jineng = Part4_JL
            JL_jineng_lv = Part5_JL
        elseif Part6_JL == 13 then
            JL_jineng = Part6_JL
            JL_jineng_lv = Part7_JL
        end
        life_lv = GetSkillLv(role, SK_ZHIZAO)
    end
    if ItemID[2] == 2301 then
        if Part2_JL == 14 then
            JL_jineng = Part2_JL
            JL_jineng_lv = Part3_JL
        elseif Part4_JL == 14 then
            JL_jineng = Part4_JL
            JL_jineng_lv = Part5_JL
        elseif Part6_JL == 14 then
            JL_jineng = Part6_JL
            JL_jineng_lv = Part7_JL
        end
        life_lv = GetSkillLv(role, SK_ZHUZAO)
    end
    if ItemID[2] == 2302 then
        if Part2_JL == 16 then
            JL_jineng = Part2_JL
            JL_jineng_lv = Part3_JL
        elseif Part4_JL == 16 then
            JL_jineng = Part4_JL
            JL_jineng_lv = Part5_JL
        elseif Part6_JL == 16 then
            JL_jineng = Part6_JL
            JL_jineng_lv = Part7_JL
        end
        life_lv = GetSkillLv(role, SK_PENGREN)
    end
    if ItemID[3] ~= 1067 and ItemID[3] ~= 1068 and ItemID[3] ~= 1069 then
        SystemNotice(role, "Please use tool")
        return 0
    end
    if ItemID[3] == 1067 or ItemID[3] == 1068 or ItemID[3] == 1069 or ItemID[3] == 1070 then
        local Gj_ure = GetItemAttr(Item[3], ITEMATTR_URE)
        if Gj_ure <= 0 then
            Gj_ure = 0
            SystemNotice(
                role,
                "Durability of tool is too low. I suggest you to bring along some repair tools to Furnace of Immortality at Spring Town for repairs."
            )
            return 0
        end
        if ItemID[3] == 1068 and ItemID[2] ~= 2300 then
            SystemNotice(role, "Tool and Blueprint mismatch")
            return 0
        end
        if ItemID[3] == 1069 and ItemID[2] ~= 2301 then
            SystemNotice(role, "Tool and Blueprint mismatch")
            return 0
        end
        if ItemID[3] == 1067 and ItemID[2] ~= 2302 then
            SystemNotice(role, "Tool and Blueprint mismatch")
            return 0
        end
        local Gj_lv = GetItemAttr(Item[3], ITEMATTR_VAL_STR)
        JL_jineng_lv = 3 * JL_jineng_lv + 1
        if JL_jineng_lv < Gj_lv then
            SystemNotice(role, "Fairy skill level do not match tool level")
            return 0
        end
    end

    if ItemType[2] ~= 69 then
        SystemNotice(role, "Almighty: You dare to use fake blueprint?")
        return 0
    end

    local paper_lv = GetItemAttr(Item[2], ITEMATTR_URE)

    if life_lv < paper_lv then
        SystemNotice(role, "Character skill level does not match Blueprint level")
        return 0
    end
    local paper_id1 = GetItemAttr(Item[2], ITEMATTR_VAL_STR)

    local paper_id2 = GetItemAttr(Item[2], ITEMATTR_VAL_CON)

    local paper_id3 = GetItemAttr(Item[2], ITEMATTR_VAL_DEX)

    if ItemID[4] ~= paper_id1 or ItemID[5] ~= paper_id2 or ItemID[6] ~= paper_id3 then
        SystemNotice(
            role,
            "Please check the type of Material and placement position to match that of blueprint's requirement."
        )
        return 0
    end

    local Num_paper = GetItemForgeParam(Item[2], 1)

    Num_paper = TansferNum(Num_paper)
    local Part1_paper = GetNum_Part1(Num_paper)
    local Part2_paper = GetNum_Part2(Num_paper)

    local Part3_paper = GetNum_Part3(Num_paper)
    local Part4_paper = GetNum_Part4(Num_paper)

    local Part5_paper = GetNum_Part5(Num_paper)
    local Part6_paper = GetNum_Part6(Num_paper)

    local Part7_paper = GetNum_Part7(Num_paper)
    local i1 = CheckBagItem(role, ItemID[4])
    local i2 = CheckBagItem(role, ItemID[5])
    local i3 = CheckBagItem(role, ItemID[6])

    if i1 < Part2_paper or i2 < Part4_paper or i3 < Part6_paper then
        SystemNotice(role, "Lack of quantity for certain required item")
        return 0
    end
    local paper_num = GetItemAttr(Item[2], ITEMATTR_VAL_STA)

    if paper_num == 0 then
        SystemNotice(role, "Blueprint is not longer usable. Discard it")
        return 0
    end
    local a1 = CheckBagItem(role, 855)
    local a1_num = GetItemAttr(Item[2], ITEMATTR_MAXURE)

    if a1 < a1_num then
        SystemNotice(role, "Not enough Fairy Coins in backpack.")
        return 0
    end

    return 1
end

function Read_manufacture(Table)
    local role = Table[1]
    local ItemBagCount = Table[2]
    local ItemBag = {}

    local i = 0

    if ItemBagCount == 0 then
        return role, ItemBag, ItemBagCount
    end
    for i = 1, ItemBagCount, 1 do
        local ReadNow = i + 2
        ItemBag[i] = Table[ReadNow]
    end

    return role, ItemBag, ItemBagCount
end

function begin_manufacture_item(...)
    local arg = {...}

    local role = 0
    local ItemBag = {}

    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(arg)

    local Check1 = can_manufacture_item_main(arg)
    if Check1 ~= 1 then
        return 0
    end

    local i = 0
    local j = 0

    local Item = {}
    local ItemID = {}
    local ItemType = {}
    for j = 1, ItemBagCount, 1 do
        Item[j] = GetChaItem(role, 2, ItemBag[j])
        ItemID[j] = GetItemID(Item[j])
        ItemType[j] = GetItemType(Item[j])
    end
    local Gj_lv = 0

    if ItemID[3] == 1068 then
        Gj_lv = GetItemAttr(Item[3], ITEMATTR_VAL_STR)
    end
    local life_lv = GetSkillLv(role, SK_ZHIZAO)

    local paper_lv = GetItemAttr(Item[2], ITEMATTR_URE)

    local paper_energy = GetItemAttr(Item[2], ITEMATTR_MAXENERGY) - 100

    local star_good = (math.min(life_lv, paper_lv) * 0.03 + Gj_lv * 0.05 + (100 - paper_energy * 10) * 0.01) * 100
    local star_radom = math.random(1, 100)

    local eleven = 2
    local a1 = star_radom + 7
    local a2 = star_radom + 14
    local a3 = star_radom + 21
    local a4 = star_radom + 28
    local a5 = star_radom + 35
    local a6 = star_radom + 42
    local a7 = star_radom + 49
    local a8 = star_radom + 56
    local a9 = star_radom + 63

    if star_good < star_radom then
        eleven = 1
    elseif star_good >= 98 then
        eleven = 11
    elseif star_good >= a9 then
        eleven = 10
    elseif star_good >= a8 then
        eleven = 9
    elseif star_good >= a7 then
        eleven = 8
    elseif star_good >= a6 then
        eleven = 7
    elseif star_good >= a5 then
        eleven = 6
    elseif star_good >= a4 then
        eleven = 5
    elseif star_good >= a3 then
        eleven = 4
    elseif star_good >= a2 then
        eleven = 3
    elseif star_good >= a1 then
        eleven = 2
    end
    local star_begin = 3 * (1 + paper_lv)
    local star_end = 5 * (1 + paper_lv)
    local star = math.random(star_begin, star_end)
    if star > 64 then
        star = 64
    end
    local run_time = star

    return 2, run_time, eleven
end
function begin_manufacture1_item(...)
    local arg = {...}

    local role = 0
    local ItemBag = {}

    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(arg)

    local Check1 = can_manufacture_item_main(arg)
    if Check1 ~= 1 then
        return 0
    end

    local i = 0
    local j = 0

    local Item = {}
    local ItemID = {}
    local ItemType = {}

    for j = 1, ItemBagCount, 1 do
        Item[j] = GetChaItem(role, 2, ItemBag[j])
        ItemID[j] = GetItemID(Item[j])
        ItemType[j] = GetItemType(Item[j])
    end

    local paper_lv = GetItemAttr(Item[2], ITEMATTR_URE)

    local star_begin = 3 * (1 + paper_lv)
    local star_end = 5 * (1 + paper_lv)
    local star = math.random(star_begin, star_end)
    if star > 64 then
        star = 64
    end
    local run_time = star
    local WORD1 = math.random(1, 6)
    local WORD2 = math.random(1, 6)
    local WORD3 = math.random(1, 6)
    local str = "" .. WORD1 .. "," .. WORD2 .. "," .. WORD3

    return 2, run_time, str
end
function begin_manufacture2_item(...)
    local arg = {...}

    local role = 0
    local ItemBag = {}

    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(arg)

    local Check1 = can_manufacture_item_main(arg)
    if Check1 ~= 1 then
        return 0
    end

    local i = 0
    local j = 0

    local Item = {}
    local ItemID = {}
    local ItemType = {}

    for j = 1, ItemBagCount, 1 do
        Item[j] = GetChaItem(role, 2, ItemBag[j])
        ItemID[j] = GetItemID(Item[j])
        ItemType[j] = GetItemType(Item[j])
    end

    local paper_lv = GetItemAttr(Item[2], ITEMATTR_URE)

    local star_begin = 3 * (1 + paper_lv)
    local star_end = 4 * (1 + paper_lv)
    local star = math.random(star_begin, star_end)
    if star > 64 then
        star = 64
    end
    local run_time = star

    local star_ok = 12

    return 2, run_time, star_ok
end
function begin_manufacture3_item(...)
    local arg = {...}

    local role = 0
    local ItemBag = {}

    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(arg)

    local Check1 = can_fenjie_item_main(arg)
    if Check1 ~= 1 then
        return 0
    end

    local i = 0
    local j = 0

    local Item = {}
    local ItemID = {}
    local ItemType = {}
    for j = 1, ItemBagCount, 1 do
        Item[j] = GetChaItem(role, 2, ItemBag[j])
        ItemID[j] = GetItemID(Item[j])
        ItemType[j] = GetItemType(Item[j])
    end
    local Item_Lv = GetItemOriginalLv(Item[3])
    if ItemID[3] >= 5000 then
        local tmd_rad = math.random(1, 10)
        if tmd_rad == 1 then
            Item_Lv = 80
        elseif tmd_rad == 2 then
            Item_Lv = 70
        elseif tmd_rad == 3 then
            Item_Lv = 60
        elseif tmd_rad == 4 then
            Item_Lv = 50
        elseif tmd_rad == 5 then
            Item_Lv = 40
        elseif tmd_rad == 6 then
            Item_Lv = 30
        elseif tmd_rad == 7 then
            Item_Lv = 20
        else
            Item_Lv = 10
        end
    end
    local base_rad = 0
    base_rad = math.max((80 - math.max(Item_Lv, 10)) * 0.01, 0.15)

    local Num_JL = GetItemForgeParam(Item[1], 1)
    Num_JL = TansferNum(Num_JL)
    local Part1_JL = GetNum_Part1(Num_JL)
    local Part2_JL = GetNum_Part2(Num_JL)
    local Part3_JL = GetNum_Part3(Num_JL)
    local Part4_JL = GetNum_Part4(Num_JL)
    local Part5_JL = GetNum_Part5(Num_JL)
    local Part6_JL = GetNum_Part6(Num_JL)
    local Part7_JL = GetNum_Part7(Num_JL)
    local JL_jineng = 0
    local JL_jineng_lv = 0
    if Part2_JL == 16 then
        JL_jineng = Part2_JL
        JL_jineng_lv = Part3_JL
    elseif Part4_JL == 16 then
        JL_jineng = Part4_JL
        JL_jineng_lv = Part5_JL
    elseif Part6_JL == 16 then
        JL_jineng = Part6_JL
        JL_jineng_lv = Part7_JL
    end

    local Gj_lv = 0
    if ItemID[2] == 1070 then
        Gj_lv = GetItemAttr(Item[2], ITEMATTR_VAL_STR)
    end

    local life_lv = 0
    life_lv = GetSkillLv(role, SK_FENJIE)

    local run_time = math.random(4, 8)
    local word_test = math.floor((JL_jineng_lv * 0.05 + life_lv * 0.02 + Gj_lv * 0.03 + base_rad) * 100000)
    if word_test > 99999 then
        word_test = 99999
    end
    local word_radom = math.random(10000, 99999)
    local str = "" .. word_test .. "," .. word_radom

    return 2, run_time, str
end
function end_manufacture_item(...)
    local arg = {...}

    local role = 0
    local ItemBag = {}

    local ItemBagCount = 0

    role, ItemBag, ItemBagCount = Read_manufacture(arg)

    local i = 0
    local j = 0

    local star_check = 0

    star_check = arg[#arg]

    local Item = {}
    local ItemID = {}
    local ItemType = {}
    local check = {}
    for j = 1, ItemBagCount, 1 do
        Item[j] = GetChaItem(role, 2, ItemBag[j])
        ItemID[j] = GetItemID(Item[j])
        ItemType[j] = GetItemType(Item[j])
    end
    local paper_id1 = GetItemAttr(Item[2], ITEMATTR_VAL_STR)

    local paper_id2 = GetItemAttr(Item[2], ITEMATTR_VAL_CON)

    local paper_id3 = GetItemAttr(Item[2], ITEMATTR_VAL_DEX)

    local Num_paper = GetItemForgeParam(Item[2], 1)

    Num_paper = TansferNum(Num_paper)
    local Part2_paper = GetNum_Part2(Num_paper)
    local Part4_paper = GetNum_Part4(Num_paper)
    local Part6_paper = GetNum_Part6(Num_paper)

    local life_lv = 0

    local Gj_lv = 0

    local paper_lv = GetItemAttr(Item[2], ITEMATTR_URE)

    local num_x = 1
    local star_num_qulity = 4
    if ItemID[2] == 2300 then
        life_lv = GetSkillLv(role, SK_ZHIZAO)
        if star_check == 1 then
            num_x = 0
        elseif star_check == 2 or star_check == 3 or star_check == 4 then
            num_x = 1
        elseif star_check == 5 or star_check == 6 or star_check == 7 then
            num_x = 2
        elseif star_check == 8 or star_check == 9 or star_check == 10 then
            num_x = 3
        elseif star_check == 11 then
            num_x = 4
        end
    end
    if ItemID[2] == 2301 then
        life_lv = GetSkillLv(role, SK_ZHUZAO)
    end
    if ItemID[2] == 2302 then
        life_lv = GetSkillLv(role, SK_PENGREN)
        local differ_check = math.abs(star_check - 75)
        if differ_check == 0 then
            num_x = 5
        elseif differ_check == 1 then
            num_x = 4
        elseif differ_check == 2 then
            num_x = 3
        elseif differ_check >= 3 and differ_check <= 6 and star_check <= 77 then
            num_x = 2
        elseif differ_check >= 7 and differ_check <= 25 and star_check <= 77 then
            num_x = 1
        else
            num_x = 0
        end
    end
    if ItemID[3] == 1067 or ItemID[3] == 1068 or ItemID[3] == 1069 or ItemID[3] == 1070 then
        Gj_lv = GetItemAttr(Item[3], ITEMATTR_VAL_STR)
    end

    local i1 = 0
    local i2 = 0
    local i3 = 0
    i1 = TakeItem(role, 0, paper_id1, Part2_paper)

    i2 = TakeItem(role, 0, paper_id2, Part4_paper)

    i3 = TakeItem(role, 0, paper_id3, Part6_paper)

    if i1 == 0 or i2 == 0 or i3 == 0 then
        LG("Hecheng_BS", "Delete item failed")
    end
    local a1_num = GetItemAttr(Item[2], ITEMATTR_MAXURE)

    local a1 = TakeItem(role, 0, 855, a1_num)
    if a1 == 0 then
        SystemNotice(role, "Failed to delete Fairy's coin")
        return
    end

    local new_num = GetItemAttr(Item[2], ITEMATTR_VAL_AGI)

    if ItemID[2] == 2300 then
        if new_num == 1067 or new_num == 1068 or new_num == 1069 or new_num == 1070 or new_num == 2236 then
            num_x = 1
        end
    end
    local paper_energy = GetItemAttr(Item[2], ITEMATTR_MAXENERGY) - 100

    local star_good = (math.min(life_lv, paper_lv) * 0.03 + Gj_lv * 0.05 + (100 - paper_energy * 10) * 0.01) * 100
    local star_radom = math.random(1, 100)
    local m1 = -1
    local m2 = -1
    if ItemID[2] == 2300 then
        if star_check >= 2 then
            star_good = 100
            star_radom = 1
        else
            star_good = 1
            star_radom = 100
        end
    end
    local star_check_chenggong = 0
    if star_check ~= 0 and star_good > star_radom and num_x ~= 0 then
        star_check_chenggong = 1
        m1, m2 = MakeItem(role, new_num, num_x, star_num_qulity)
        local Itemfinal = GetChaItem(role, 2, m2)
        if ItemID[2] == 2301 and CheckItem_CanJinglian(Itemfinal) == 1 then
            local Itemfinal_energy = GetItemAttr(Itemfinal, ITEMATTR_ENERGY)
            local itemfinal_maxenergy = GetItemAttrRange(new_num, ITEMATTR_MAXENERGY, 1)

            local itemfinal_minenergy = GetItemAttrRange(new_num, ITEMATTR_MAXENERGY, 0)

            if paper_energy > 7 then
                paper_energy = 7
            end
            if itemfinal_maxenergy ~= itemfinal_minenergy then
                Itemfinal_energy = math.fmod(Itemfinal_energy, 1000) + paper_energy * 1000
                SetItemAttr(Itemfinal, ITEMATTR_MAXENERGY, Itemfinal_energy)
                SetItemAttr(Itemfinal, ITEMATTR_ENERGY, Itemfinal_energy)
            end
        end
        local item_final_ID = GetItemID(Itemfinal)

        if item_final_ID == 1067 or item_final_ID == 1068 or item_final_ID == 1069 or item_final_ID == 1070 then
            SetItemAttr(Itemfinal, ITEMATTR_VAL_STR, 1)

            SetItemAttr(Itemfinal, ITEMATTR_MAXENERGY, 10000)

            SetItemAttr(Itemfinal, ITEMATTR_ENERGY, 1)
        end
        if item_final_ID == 2236 then
            SetItemAttr(Itemfinal, ITEMATTR_VAL_STR, paper_lv)
        end
    else
        SystemNotice(
            role,
            "Your process might have been wrong or your level is too low. Your hard work have gone to the drain and some items have disappeared."
        )
    end

    local paper_num = GetItemAttr(Item[2], ITEMATTR_VAL_STA)

    paper_num = paper_num - 1

    SetItemAttr(Item[2], ITEMATTR_VAL_STA, paper_num)

    local Gj_ure = 0
    if ItemID[3] == 1067 or ItemID[3] == 1068 or ItemID[3] == 1069 or ItemID[3] == 1070 then
        Gj_ure = GetItemAttr(Item[3], ITEMATTR_URE)
        local star_gjlv_num = GetItemAttr(Item[3], ITEMATTR_VAL_STR)
        Gj_ure = Gj_ure - 50 * star_gjlv_num
        if Gj_ure <= 0 then
            Gj_ure = 0
        end

        local star_lv_num = GetItemAttr(Item[3], ITEMATTR_ENERGY)
        if star_check_chenggong == 1 then
            star_lv_num = star_lv_num + paper_lv
        else
            star_lv_num = star_lv_num + 1
        end
        if star_lv_num >= 10000 then
            star_lv_num = 10000
        end
        SystemNotice(role, "Your tool currently has " .. star_lv_num .. " point(s) of experience")
        if star_lv_num >= star_gjlv_num * star_gjlv_num * 100 then
            star_gjlv_num = star_gjlv_num + 1
            SetItemAttr(Item[3], ITEMATTR_VAL_STR, star_gjlv_num)

            SystemNotice(role, "Congratulations, your tool has increase in level!")
            star_lv_num = 0
        end
        SetItemAttr(Item[3], ITEMATTR_ENERGY, star_lv_num)
        SetItemAttr(Item[3], ITEMATTR_URE, Gj_ure)
    end

    local cha_name = GetChaDefaultName(role)
    LG(
        "star_SHENGHUO_lg",
        cha_name,
        star_check,
        ItemID[2],
        paper_lv,
        paper_id1,
        Part2_paper,
        paper_id2,
        Part4_paper,
        paper_id3,
        Part6_paper,
        ItemID[3],
        Gj_lv,
        life_lv
    )
    SynChaKitbag(role, 13)

    return m2
end

function GetEquipLvAnalyze(ItemLv)
	local Key = 0
	if (ItemLv >= 1 and ItemLv <= 19) then            
		Key = 0
	elseif (ItemLv >= 20 and ItemLv <= 29) then        
		Key = 20
	elseif (ItemLv >= 30 and ItemLv <= 39) then        
		Key = 30            
	elseif (ItemLv >= 40 and ItemLv <= 49) then        
		Key = 40    
	elseif (ItemLv >= 50 and ItemLv <= 59) then        
		Key = 50         
	elseif (ItemLv >= 60 and ItemLv <= 69) then        
		Key = 60     
	elseif (ItemLv >= 70 and ItemLv <= 79) then     
		Key = 70      
	elseif (ItemLv >= 80 and ItemLv <= 89) then        
		Key = 80         
	elseif (ItemLv >= 90 and ItemLv <= 99) then        
		Key = 90  
	end        
	return Key
end 

function can_fenjie_item(...)
    local arg = {...}
	local ItemBagCount = arg[2]
	local Length = ItemBagCount + 3
	if #arg ~= Length then
		Notice("Parameter value illegal "..#arg)
		return 0
	end	
	local Check = 0
	Check = can_fenjie_item_main(arg)
	if Check == 1 then		
		return 1
	else
		return 0
	end
end

function can_fenjie_item_main(Table)
	local Player = 0
	local ItemBag = {}									
	local ItemBagCount = 0								
	Player, ItemBag, ItemBagCount = Read_manufacture(Table)
	Player, ItemBag, ItemBagCount = Read_manufacture(Table)
	-- Checking if player has empty inventory slots.
	if GetChaFreeBagGridNum(Player) < 1 then
		SystemNotice(Player, "You need at least 1 empty slots in Inventory!")
		return
	end
	local i = 0
	local Item = {}
	local ItemID = {}
	local ItemType = {}
	for i = 1, ItemBagCount, 1 do							
		Item[i] = GetChaItem(Player, 2, ItemBag[i])			
		ItemID[i] = GetItemID(Item[i])						
		ItemType[i] = GetItemType(Item[i])					
	end
	-- Checking if item type on cell 1 is from fairy.
	if ItemType[1] ~= 59 then
		SystemNotice(Player, "Please put your Fairy in 1st Cell!")
		return 0
	end
	-- Checking if fairy is dead.
	local FairyStamina = GetItemAttr(Item[1], ITEMATTR_URE)
	if FairyStamina <= 0 then
		SystemNotice(Player, "Your Fairy stamina is too low, unable to proceed!")
		return 0	
	end
	-- Checking if player trying to analyze equipment/weapon.
	local ItemTypeCheck = 0
	ItemTypeCheck = CheckItem_CanJinglian(Item[3])
	if ItemTypeCheck == 0 then
		SystemNotice(Player, "Only equipments are able to be Analized!")
		return 0		
	end
	-- Checking if the equipment is locked
	if IsItemLocked(Item[3]) == true then
		SystemNotice(Player, "Please unlock "..GetItemName(GetItemID(Item[3])).." to analyze")
		return 0	
	end
	
	local Num = GetItemForgeParam(Item[1], 1)
	Num = TansferNum(Num)
	local Sockets = GetNum_Part1(Num)	
	local Skill_1_ID = GetNum_Part2(Num)	
	local Skill_1_LV = GetNum_Part3(Num)
	local Skill_2_ID = GetNum_Part4(Num)
	local Skill_2_LV = GetNum_Part5(Num)
	local Skill_3_ID = GetNum_Part6(Num)
	local Skill_3_LV = GetNum_Part7(Num)
	local FairySkill_ID = 0
	local FairySkill_Lv = 0
	local AnalyzeSkill_Lv = 0
	AnalyzeSkill_Lv = GetSkillLv(Player, SK_FENJIE)
	if Skill_1_ID == 15 then
		FairySkill_ID = Skill_1_ID
		FairySkill_Lv = Skill_1_LV
	elseif Skill_2_ID == 15 then
		FairySkill_ID = Skill_2_ID
		FairySkill_Lv = Skill_2_LV
	elseif Skill_3_ID == 15 then
		FairySkill_ID = Skill_3_ID
		FairySkill_Lv = Skill_3_LV
	end
	-- Checking if not tool (Particle Crystal).
	if ItemID[2] ~= 1070 then
		SystemNotice(Player, "Please put "..GetItemName(1070).." in 2nd Cell")
		return 0	
	else
		-- Checking if tool durability.
		local Tool_Ure = GetItemAttr(Item[2], ITEMATTR_URE)
		if Tool_Ure <= 0 then
			Tool_Ure = 0
			SystemNotice(Player, "Tool has been worn out. It cannot be used again.")
			return 0
		end
		-- Checking if tool Lv and Fairy skill Lv matchs.
		local Tool_Lv = GetItemAttr(Item[2], ITEMATTR_VAL_STR)
		FairySkill_Lv = 3 * FairySkill_Lv + 1
		if FairySkill_Lv < Tool_Lv then
			SystemNotice(Player, "Fairy skill level do not match tool level")
			return 0			
		end
	end
	-- Checking if player using correct catalyst.
	if (Analyze.Catalyst[ItemID[4]] == nil) then
		 SystemNotice(Player, "Please insert the Analyze catalyst")
		return 0
	end
	return 1
end

function end_fenjie_item(...)
    local arg = {...}
	local Player = 0
	local ItemBag = {}		
	local ItemBagCount = 0						
	Player, ItemBag, ItemBagCount = Read_manufacture ( arg )
	local i = 0
	local j = 0
	local Item = {}
	local ItemID = {}
	local ItemType = {}
	for j = 1, ItemBagCount, 1 do
		Item[j] = GetChaItem(Player, 2, ItemBag[j])	
		ItemID[j] = GetItemID(Item[j])		
		ItemType[j] = GetItemType(Item[j])	
	end

	local EquipLv = GetItemOriginalLv(Item[3])
	local Num = GetItemForgeParam(Item[1], 1)
	Num = TansferNum(Num)
	local Sockets = GetNum_Part1(Num)
	local Skill_1_ID = GetNum_Part2(Num)	
	local Skill_1_LV = GetNum_Part3(Num)
	local Skill_2_ID = GetNum_Part4(Num)
	local Skill_2_LV = GetNum_Part5(Num)
	local Skill_3_ID = GetNum_Part6(Num)
	local Skill_3_LV = GetNum_Part7(Num)
	local FairySkill_Lv = 0
	if Skill_1_ID == 16 then
		FairySkill_Lv = Skill_1_LV
	elseif Skill_2_ID == 16 then
		FairySkill_Lv = Skill_2_LV
	elseif Skill_3_ID == 16 then
		FairySkill_Lv = Skill_3_LV
	end

	-- Checking if tool (Particle Crystal).
	local Tool_Lv = 0
	if ItemID[2] == 1070 then
		Tool_Lv = GetItemAttr(Item[2], ITEMATTR_VAL_STR)
	end
	local AnalyzeSkLv = 0
	AnalyzeSkLv = GetSkillLv( Player , SK_FENJIE )
	local Reward_QTY = math.min(math.max(1, math.floor((FairySkill_Lv * 0.1 + AnalyzeSkLv * 0.05 + Tool_Lv * 0.05) *10)), 10)
	local i1 = 0
	local i2 = 0
	i1 = RemoveChaItem(Player, ItemID[3], 1, 2, ItemBag[3], 2, 1, 1)	
	i2 = RemoveChaItem(Player, ItemID[4], 1, 2, ItemBag[4], 2, 1, 1)
	if i1 == 0 or i2 == 0 then
		LG("Item Analyze", "Failed to remove items..")
	end

    -- Setting up rewards, according to catalyst and equipment level.
    local Reward_ID = 1346
    local ItemLv = GetEquipLvAnalyze(EquipLv)
    if Analyze.Catalyst[ItemID[4]] ~= nil then
        Reward_ID = Analyze.Catalyst[ItemID[4]][ItemLv][math.random(1, #Analyze.Catalyst[ItemID[4]][ItemLv])]
    end

	-- Gives reward to player.
	GiveItem(Player, 0, Reward_ID, Reward_QTY, 4 ) 
	
	-- Decreasing tool durability.
    if ItemID[2] == 1070 then
		local Tool_Ure = GetItemAttr(Item[2], ITEMATTR_URE)
		Tool_Ure = Tool_Ure - 50 * GetItemAttr(Item[2], ITEMATTR_VAL_STR)
		if Tool_Ure <= 0 then
			Tool_Ure = 0
		end

		-- Increases tool EXP.
		local ToolEXP = GetItemAttr(Item[2], ITEMATTR_ENERGY)
		ToolEXP = ToolEXP + 1
		if ToolEXP >= 10000 then
			ToolEXP = 10000
		end

	 	SystemNotice(Player, GetItemName(1070).." Experience increased to "..ToolEXP.." point(s)!")
		
		-- Level up tool.
        local Tool_Lv = GetItemAttr(Item[2], ITEMATTR_VAL_STR)
		if ToolEXP >= Tool_Lv * Tool_Lv * 100 then 
			Tool_Lv = Tool_Lv + 1
			SetItemAttr(Item[2], ITEMATTR_VAL_STR, Tool_Lv)
	 		SystemNotice(Player, GetItemName(1070).." Level Up to "..Tool_Lv.."!")
			ToolEXP = 0
		end
		SetItemAttr(Item[2], ITEMATTR_ENERGY, ToolEXP)
		SetItemAttr(Item[2], ITEMATTR_URE, Tool_Ure)
	end
	LG("Item Analyze", GetChaDefaultName(Player)..", succesfully analysis ["..GetItemName(ItemID[3]).."] with Lv"..Tool_Lv.." "..GetItemName(1070).." and obtains ["..GetItemName(Reward_ID).."]!")
	SynChaKitbag(Player, 13)
	return 1
end

function can_shtool_item(...)
    local arg = {...}

    if #arg ~= 10 and #arg ~= 14 then
        SystemNotice(arg[1], "parameter value illegal" .. #arg)
        return 0
    end
    local Check = 0
    Check = can_shtool_item_main(arg)
    if Check == 1 then
        return 1
    else
        return 0
    end
end

function can_shtool_item_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Now = 0
    local ItemCount_Now = 0
    local ItemBagCount_Num = 0
    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Now, ItemCount_Now, ItemBagCount_Num = Read_Table(Table)

    if ItemCount[0] ~= 1 or ItemCount[1] ~= 1 or ItemBagCount[0] ~= 1 or ItemBagCount[1] ~= 1 then
        SystemNotice(role, "equipment quantity illegal ")
        return 0
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "You need at least 1 slot in your inventory")
        UseItemFailed(role)
        return
    end

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local Item_mainitem_Lv = GetItemAttr(Item_mainitem, ITEMATTR_VAL_STR)

    local Item_otheritem_Lv = GetItemAttr(Item_otheritem, ITEMATTR_VAL_STR)

    local item_shtool_ure = GetItemAttr(Item_mainitem, ITEMATTR_URE)
    local item_shtool_maxure = GetItemAttr(Item_mainitem, ITEMATTR_MAXURE)

    if ItemType_mainitem ~= 70 then
        SystemNotice(
            role,
            "Damaged Crystal Cauldron, Black Hole Crystal, Anti Matter Crystal and Particle Crystal can be repaired here."
        )
        return 0
    end
    if ItemID_mainitem ~= 1067 and ItemID_mainitem ~= 1068 and ItemID_mainitem ~= 1069 and ItemID_mainitem ~= 1070 then
        SystemNotice(
            role,
            "Damaged Crystal Cauldron, Black Hole Crystal, Anti Matter Crystal and Particle Crystal can be repaired here."
        )
        return 0
    end

    if ItemType_otheritem ~= 70 or ItemID_otheritem ~= 2236 then
        SystemNotice(role, "Please use the correct repair tool.")
        return 0
    end

    if item_shtool_ure >= item_shtool_maxure then
        SystemNotice(role, "Tool has not been damaged beyond repair")
        return 0
    end

    if Item_mainitem_Lv > Item_otheritem_Lv then
        SystemNotice(role, "Repair level cannot be lower than the level of tool being repaired")
        return 0
    end

    local Money_Need = get_item_shtool_money(Table)
    local Money_Have = GetChaAttr(role, ATTR_GD)
    if Money_Need > Money_Have then
        SystemNotice(role, "Insufficient gold. Unable to repair cauldron")
        return 0
    end

    return 1
end

function begin_shtool_item(...)
    local arg = {...}

    local Check_Canshtool = 0
    Check_Canshtool = can_shtool_item_main(arg)
    if Check_Canshtool == 0 then
        return 0
    end

    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(arg)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local Money_Need = get_item_shtool_money(arg)
    local Money_Have = GetChaAttr(role, ATTR_GD)

    TakeMoney(role, nil, Money_Need)

    Check_shtool_Item = shtool_item(arg)
    if Check_shtool_Item == 0 then
        SystemNotice(role, "Faild to repair Lifeskill tools. Please recheck process")
    end

    return 1
end

function get_item_shtool_money(...)
    local arg = {...}

    local Money = shtool_money_main(arg)
    return Money
end

function shtool_money_main(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Money_Need = 200

    return Money_Need
end

function shtool_item(Table)
    local role = 0
    local ItemBag = {}
    local ItemCount = {}
    local ItemBagCount = {}
    local ItemBag_Num = 0
    local ItemCount_Num = 0
    local ItemBagCount_Num = 0
    local ItemID_Cuihuaji = 0

    role, ItemBag, ItemCount, ItemBagCount, ItemBag_Num, ItemCount_Num, ItemBagCount_Num = Read_Table(Table)

    local Item_mainitem = GetChaItem(role, 2, ItemBag[0])
    local Item_otheritem = GetChaItem(role, 2, ItemBag[1])

    local ItemType_mainitem = GetItemType(Item_mainitem)
    local ItemType_otheritem = GetItemType(Item_otheritem)

    local ItemID_mainitem = GetItemID(Item_mainitem)
    local ItemID_otheritem = GetItemID(Item_otheritem)

    local item_shtool_ure = GetItemAttr(Item_mainitem, ITEMATTR_URE)
    local item_shtool_maxure = GetItemAttr(Item_mainitem, ITEMATTR_MAXURE)

    SetItemAttr(Item_mainitem, ITEMATTR_URE, item_shtool_maxure)

    local cha_name = GetChaDefaultName(role)
    LG("star_xiuguo_lg", cha_name, ItemID_mainitem, ItemID_otheritem)

    local R1 = 0
    R1 = RemoveChaItem(role, Item_otheritem, 1, 2, ItemBag[1], 2, 1, 0)
    if R1 == 0 then
        SystemNotice(role, "moved item failed ")
        return
    end
    SynChaKitbag(role, 13)
end

function GetChaName_4(role, npc)
    local el1 = CheckBagItem(role, 2242)
    local el2 = CheckBagItem(role, 2243)
    local el3 = CheckBagItem(role, 2244)
    local el4 = CheckBagItem(role, 2245)
    if el1 ~= 0 then
        TakeItem(role, 0, 2242, 1)
        GiveItem(role, 0, 3077, 10, 4)
    elseif el2 ~= 0 then
        TakeItem(role, 0, 2243, 1)
        GiveItem(role, 0, 3077, 10, 4)
    elseif el3 ~= 0 then
        TakeItem(role, 0, 2244, 1)
        GiveItem(role, 0, 3077, 10, 4)
    elseif el4 ~= 0 then
        TakeItem(role, 0, 2245, 1)
        GiveItem(role, 0, 3077, 10, 4)
    else
        SystemNotice(role, "You do not have the suitable voucher.")
    end
end

function GetChaName_5(role, npc)
    local cha_name = GetChaDefaultName(role)

    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local am1 = CheckBagItem(role, 2240)
    if am1 < 1 then
        SystemNotice(role, "You don't seem to have Pirate Voucher 6.")
        return
    else
        local am2 = TakeItem(role, 0, 2240, 1)
        if am2 == 0 then
            SystemNotice(role, "Collection of Pirate Voucher 6 failed")
            return
        end
    end
    GiveItem(role, 0, 1028, 1, 4)
    Notice("Congratulations" .. cha_name .. "Exchanged 1 Morph Runestone")
end

function GetChaName_21(role, npc)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum

    if CheckDateNum > 31421 then
        SystemNotice(role, "Redemption timing is over. You will have to wait till next year")
        UseItemFailed(role)
        return
    end

    local cha_name = GetChaDefaultName(role)
    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local am1 = CheckBagItem(role, 1649)
    am2 = CheckBagItem(role, 3130)
    am3 = CheckBagItem(role, 1641)
    am4 = CheckBagItem(role, 4418)
    if am1 < 1 or am2 < 1 or am3 < 1 or am4 < 1 then
        SystemNotice(role, "You do not seem to have enough items for redemption")
        return
    else
        local am5 = TakeItem(role, 0, 1649, 1)
        am6 = TakeItem(role, 0, 3130, 1)
        am7 = TakeItem(role, 0, 1641, 1)
        am8 = TakeItem(role, 0, 4418, 1)
        if am5 == 0 or am6 == 0 or am7 == 0 or am8 == 0 then
            SystemNotice(role, "Collection of required item for redemption failed")
            return
        end
    end
    GiveItem(role, 0, 1074, 1, 4)
    LG("ZAZZ", "Player" .. cha_name .. "Redeem 1 Seed of Love")
end

function GetChaName_22(role, npc)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum

    if CheckDateNum < 31420 then
        SystemNotice(
            role,
            "Do not worry, love needs time. Please exchange it on 14th March itself between 2000 hrs to 2200 hrs"
        )
        UseItemFailed(role)
        return
    end

    if CheckDateNum > 31421 then
        SystemNotice(role, "Redemption timing is over. You will have to wait till next year")
        UseItemFailed(role)
        return
    end

    local cha_name = GetChaDefaultName(role)

    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local am1 = CheckBagItem(role, 1074)
    if am1 < 10 then
        SystemNotice(role, "You do not seems to have sufficient Seeds of Love")
        return
    else
        local am2 = TakeItem(role, 0, 1074, 10)
        if am2 == 0 then
            SystemNotice(role, "Collection of 10 Seeds of Love failed")
            return
        end
    end
    GiveItem(role, 0, 3077, 1, 4)
end

function GetChaName_23(role, npc)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum

    if CheckDateNum < 31420 then
        SystemNotice(
            role,
            "Do not worry, love needs time. Please exchange it on 14th March itself between 2000 hrs to 2200 hrs"
        )
        UseItemFailed(role)
        return
    end

    if CheckDateNum > 31421 then
        SystemNotice(role, "Redemption timing is over. You will have to wait till next year")
        UseItemFailed(role)
        return
    end

    local cha_name = GetChaDefaultName(role)
    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local am1 = CheckBagItem(role, 1074)
    if am1 < 100 then
        SystemNotice(role, "You do not seems to have sufficient Seeds of Love")
        return
    else
        local am2 = TakeItem(role, 0, 1074, 100)
        if am2 == 0 then
            SystemNotice(role, "Collection of 100 Seeds of Love failed")
            return
        end
    end
    GiveItem(role, 0, 3094, 3, 4)
end

function GetChaName_24(role, npc)
    local Now_Day = os.date("%d")
    local Now_Month = os.date("%m")
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    local NowDayNum = tonumber(Now_Day)
    local NowMonthNum = tonumber(Now_Month)
    local CheckDateNum = NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum

    if CheckDateNum < 31420 then
        SystemNotice(
            role,
            "Do not worry, love needs time. Please exchange it on 14th March itself between 2000 hrs to 2200 hrs"
        )
        UseItemFailed(role)
        return
    end

    if CheckDateNum > 31421 then
        SystemNotice(role, "Redemption timing is over. You will have to wait till next year")
        UseItemFailed(role)
        return
    end

    local cha_name = GetChaDefaultName(role)
    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local am1 = CheckBagItem(role, 1074)
    if am1 < 1000 then
        SystemNotice(role, "You do not seems to have sufficient Seeds of Love")
        return
    else
        local am2 = TakeItem(role, 0, 1074, 1000)
        if am2 == 0 then
            SystemNotice(role, "Collection of 1000 Seeds of Love failed")
            return
        end
    end
    GiveItem(role, 0, 19541, 1, 4)
end

function GetChaName_25(role, npc)
    local ret = KitbagLock(role, 0)
    if ret ~= LUA_TRUE then
        SystemNotice(role, "Inventory is binded. Unable to redeem")
        return
    end
    local retbag = HasLeaveBagGrid(role, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(role, "Insufficent inventory space to redeem")
        return
    end
    local cha_name = GetChaDefaultName(role)
    local am1 = CheckBagItem(role, 2235)

    if am1 < 1 then
        SystemNotice(role, "You do not seem to have any Rebirth Stone")
        return
    end
    local an1 = TakeItem(role, 0, 2235, 1)

    if an1 == 0 then
        SystemNotice(role, "Collection of redemption item failed")
    else
        GiveItem(role, 0, 2941, 1, 4)
        LG("ZSK", cha_name, "Rebirth Stone" .. am1 .. " ")
    end
end

function GetChaName1_guildwar(role, npc)
    local num_count = CheckBagItem(role, 2859)
    if num_count >= 20 then
        TakeItem(role, 0, 2859, 20)
        local a = math.random(1, 4)
        if a == 1 then
            AddState(role, role, STATE_QINGZ, 10, 300)
        elseif a == 2 then
            AddState(role, role, STATE_YS, 10, 300)
        elseif a == 3 then
            AddState(role, role, STATE_HFZQ, 10, 10)
        else
            AddState(role, role, STATE_JRQKL, 10, 180)
        end
    end
    if num_count < 20 then
        SystemNotice(role, "You do not have enough eye patch!")
    end
end

function GetChaName2_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)

    if map_name_role == "guildwar" then
        local bs_def = Def(haijunSide_BaseRole)
        local bs_reseist = Resist(haijunSide_BaseRole)
        local def_20 = 20
        local pedf_1 = 1
        local bs_def_after = bs_def + def_20
        local bs_reseist_after = bs_reseist + pedf_1

        local num_count = CheckBagItem(role, 4546)
        if num_count >= 30 then
            TakeItem(role, 0, 4546, 30)
            SetChaAttrI(haijunSide_BaseRole, ATTR_DEF, bs_def_after)
            SetChaAttrI(haijunSide_BaseRole, ATTR_PDEF, bs_reseist_after)
            SystemNotice(role, "Great! Navy Statue defense rose!")
        end
        if num_count < 30 then
            SystemNotice(role, "You haven't collect enough crystal ore")
        end
    elseif map_name_role == "guildwar2" then
        local bs_def = Def(di_haijunSide_BaseRole)
        local bs_reseist = Resist(di_haijunSide_BaseRole)
        local def_20 = 20
        local pedf_1 = 1
        local bs_def_after = bs_def + def_20
        local bs_reseist_after = bs_reseist + pedf_1

        local num_count = CheckBagItem(role, 4546)
        if num_count >= 30 then
            TakeItem(role, 0, 4546, 30)
            SetChaAttrI(di_haijunSide_BaseRole, ATTR_DEF, bs_def_after)
            SetChaAttrI(di_haijunSide_BaseRole, ATTR_PDEF, bs_reseist_after)
            SystemNotice(role, "Great! Navy Statue defense rose!")
        end
        if num_count < 30 then
            SystemNotice(role, "You haven't collect enough crystal ore")
        end
    end
end

function GetChaName3_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)
    if map_name_role == "guildwar" then
        local num_count_1 = CheckBagItem(role, 1684)

        local num_count_2 = CheckBagItem(role, 4012)

        if num_count_1 >= 4 and num_count_2 >= 9 then
            TakeItem(role, 0, 1684, 4)
            TakeItem(role, 0, 4012, 9)
            AddState(haijunSide_BaseRole, haijunSide_BaseRole, STATE_PKWD, 10, 180)
            SystemNotice(role, "Haha! Statue is now invincible for 3 minutes!")
        else
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
    if map_name_role == "guildwar2" then
        local num_count_1 = CheckBagItem(role, 1684)

        local num_count_2 = CheckBagItem(role, 4012)

        if num_count_1 >= 4 and num_count_2 >= 9 then
            TakeItem(role, 0, 1684, 4)
            TakeItem(role, 0, 4012, 9)
            AddState(di_haijunSide_BaseRole, di_haijunSide_BaseRole, STATE_PKWD, 10, 180)
            SystemNotice(role, "Haha! Statue is now invincible for 3 minutes!")
        else
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
end

function GetChaName4_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)
    if map_name_role == "guildwar" then
        local num_count_1 = CheckBagItem(role, 4011)

        local num_count_2 = CheckBagItem(role, 1720)

        if num_count_1 >= 12 and num_count_2 >= 12 then
            TakeItem(role, 0, 4011, 12)
            TakeItem(role, 0, 1720, 12)
            local min_atk = Mnatk(haijunSide_JTRole_1)
            local max_atk = Mxatk(haijunSide_JTRole_1)

            if min_atk == 0 then
                local min_atk = Mnatk(haijunSide_JTRole_2)
                local max_atk = Mxatk(haijunSide_JTRole_2)
            end
            local min_atk_after = min_atk + 25
            local max_atk_after = max_atk + 25
            SetChaAttrI(haijunSide_JTRole_1, ATTR_MNATK, min_atk_after)
            SetChaAttrI(haijunSide_JTRole_1, ATTR_MXATK, max_atk_after)
            SetChaAttrI(haijunSide_JTRole_2, ATTR_MNATK, min_atk_after)
            SetChaAttrI(haijunSide_JTRole_2, ATTR_MXATK, max_atk_after)
            SystemNotice(role, "Great ! Arrow Tower attack has rose!")
        elseif num_count_1 < 12 and num_count_2 < 12 then
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end

    if map_name_role == "guildwar2" then
        local num_count_1 = CheckBagItem(role, 4011)

        local num_count_2 = CheckBagItem(role, 1720)

        if num_count_1 >= 12 and num_count_2 >= 12 then
            TakeItem(role, 0, 4011, 12)
            TakeItem(role, 0, 1720, 12)
            local min_atk = Mnatk(di_haijunSide_JTRole_1)
            local max_atk = Mxatk(di_haijunSide_JTRole_1)

            if min_atk == 0 then
                local min_atk = Mnatk(di_haijunSide_JTRole_2)
                local max_atk = Mxatk(di_haijunSide_JTRole_2)
            end
            local min_atk_after = min_atk + 25
            local max_atk_after = max_atk + 25
            SetChaAttrI(di_haijunSide_JTRole_1, ATTR_MNATK, min_atk_after)
            SetChaAttrI(di_haijunSide_JTRole_1, ATTR_MXATK, max_atk_after)
            SetChaAttrI(di_haijunSide_JTRole_2, ATTR_MNATK, min_atk_after)
            SetChaAttrI(di_haijunSide_JTRole_2, ATTR_MXATK, max_atk_after)
            SystemNotice(role, "Great ! Arrow Tower attack has rose!")
        elseif num_count_1 < 12 and num_count_2 < 12 then
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
end

function GetChaName5_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 30641, 51702, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 30941, 51702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 30641, 51702, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 30941, 51702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName6_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName7_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName8_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName9_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName10_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(999, 11300, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11241, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 15)

        local Monster1 = CreateChaX(1025, 11300, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11241, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName11_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName12_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName13_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName14_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 30700, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 30841, 10902, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 30741, 11102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 30741, 11002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 30700, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 30841, 10902, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 30741, 11102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 30741, 11002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName15_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 51200, 10000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 51241, 10102, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 51341, 10002, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 51441, 10102, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 51200, 10000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 51241, 10102, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 51341, 10002, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 51441, 10102, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName16_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(999, 11100, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 11041, 11702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11141, 11902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 11100, 11800, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 11241, 11902, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 11241, 11602, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 30)

        local Monster1 = CreateChaX(1025, 11100, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11141, 11502, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 11100, 11900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 11241, 11802, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 11241, 11502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName17_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)
        local Monster2 = CreateChaX(999, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)
        local Monster3 = CreateChaX(999, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)
        local Monster5 = CreateChaX(999, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)
        local Monster6 = CreateChaX(999, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)
        local Monster8 = CreateChaX(999, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 30841, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 30741, 51302, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 30741, 51202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)
        local Monster2 = CreateChaX(1025, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)
        local Monster3 = CreateChaX(1025, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)
        local Monster5 = CreateChaX(1025, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)
        local Monster6 = CreateChaX(1025, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)
        local Monster8 = CreateChaX(1025, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 30841, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 30741, 51302, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 30741, 51202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName18_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(999, 12041, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 12141, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 12641, 52202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(1025, 12041, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 12141, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 12641, 52202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName19_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(999, 52441, 52502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 52641, 52702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 52641, 52202, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 52641, 52302, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(1025, 52441, 52502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 52641, 52702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 52641, 52202, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 52641, 52302, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName20_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 30700, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 30841, 10902, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 30741, 11102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 30741, 11002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(999, 30841, 11102, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 30941, 10702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 30541, 11102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 30441, 10702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 30700, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 30741, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 30700, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 30841, 10902, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 30741, 11102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 30741, 11002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(1025, 30841, 11102, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 30941, 10702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 30541, 11102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 30441, 10702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName21_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 51200, 10000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 51241, 10102, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 51341, 10002, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 51441, 10102, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(999, 51241, 10002, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 51241, 10202, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 51041, 9802, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 51141, 9902, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 51341, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 51200, 10000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 51241, 10102, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 51341, 10002, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 51441, 10102, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(1025, 51241, 10002, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 51141, 10002, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 51041, 9802, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 51141, 9902, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName22_guildwar(role)
    local num_count_1 = CheckBagItem(role, 2964)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(999, 11100, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(999, 11041, 11502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(999, 11141, 11402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(999, 11100, 11500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(999, 11241, 11502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(999, 11241, 11602, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(999, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(999, 11141, 11902, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(999, 11441, 11902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(999, 11441, 11802, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(999, 10941, 11802, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 2964, 45)
        local Monster1 = CreateChaX(1025, 11100, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 1)

        local Monster2 = CreateChaX(1025, 11041, 11502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 1)

        local Monster3 = CreateChaX(1025, 11141, 11402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 1)
        local Monster4 = CreateChaX(1025, 11100, 11500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 1)

        local Monster5 = CreateChaX(1025, 11241, 11502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 1)

        local Monster6 = CreateChaX(1025, 11241, 11602, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 1)
        local Monster7 = CreateChaX(1025, 11341, 11602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 1)

        local Monster8 = CreateChaX(1025, 11141, 11902, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 1)
        local Monster9 = CreateChaX(1025, 11441, 11902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 1)
        local Monster10 = CreateChaX(1025, 11441, 11802, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 1)
        local Monster11 = CreateChaX(1025, 10941, 11802, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 1)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough [Navy Token] ")
    end
end

function GetChaName23_guildwar(role, npc)
    local num_count = CheckBagItem(role, 2858)
    if num_count >= 20 then
        TakeItem(role, 0, 2858, 20)
        local a = math.random(1, 4)
        if a == 1 then
            AddState(role, role, STATE_QINGZ, 10, 300)
        elseif a == 2 then
            AddState(role, role, STATE_YS, 10, 300)
        elseif a == 3 then
            AddState(role, role, STATE_HFZQ, 10, 10)
        else
            AddState(role, role, STATE_JRQKL, 10, 180)
        end
    end
    if num_count < 20 then
        SystemNotice(role, "You do not have enough eye patch!")
    end
end

function GetChaName24_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)

    if map_name_role == "guildwar" then
        local bs_def = Def(haidaoSide_BaseRole)
        local bs_reseist = Resist(haidaoSide_BaseRole)
        local def_20 = 20
        local pedf_1 = 1
        local bs_def_after = bs_def + def_20
        local bs_reseist_after = bs_reseist + pedf_1
        local num_count = CheckBagItem(role, 4546)
        if num_count >= 30 then
            TakeItem(role, 0, 4546, 30)
            SetChaAttrI(haidaoSide_BaseRole, ATTR_DEF, bs_def_after)

            SetChaAttrI(haidaoSide_BaseRole, ATTR_PDEF, bs_reseist_after)
            SystemNotice(role, "Great! Pirate Status defense power has raised!")
        end
        if num_count < 30 then
            SystemNotice(role, "You haven't collect enough crystal ore")
        end
    elseif map_name_role == "guildwar2" then
        local bs_def = Def(di_haidaoSide_BaseRole)
        local bs_reseist = Resist(di_haidaoSide_BaseRole)
        local def_20 = 20
        local pedf_1 = 1
        local bs_def_after = bs_def + def_20
        local bs_reseist_after = bs_reseist + pedf_1
        local num_count = CheckBagItem(role, 4546)
        if num_count >= 30 then
            TakeItem(role, 0, 4546, 30)
            SetChaAttrI(di_haidaoSide_BaseRole, ATTR_DEF, bs_def_after)

            SetChaAttrI(di_haidaoSide_BaseRole, ATTR_PDEF, bs_reseist_after)
            SystemNotice(role, "Great! Pirate Status defense power has raised!")
        end
        if num_count < 30 then
            SystemNotice(role, "You haven't collect enough crystal ore")
        end
    end
end

function GetChaName25_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)
    if map_name_role == "guildwar" then
        local num_count_1 = CheckBagItem(role, 4013)

        local num_count_2 = CheckBagItem(role, 1683)

        if num_count_1 >= 4 and num_count_2 >= 9 then
            TakeItem(role, 0, 4013, 4)
            TakeItem(role, 0, 1683, 9)
            AddState(haidaoSide_BaseRole, haidaoSide_BaseRole, STATE_PKWD, 10, 180)
            SystemNotice(role, "Haha ! Statue is now temporary invincible!")
        else
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
    if map_name_role == "guildwar2" then
        local num_count_1 = CheckBagItem(role, 4013)

        local num_count_2 = CheckBagItem(role, 1683)

        if num_count_1 >= 4 and num_count_2 >= 9 then
            TakeItem(role, 0, 4013, 4)
            TakeItem(role, 0, 1683, 9)
            AddState(di_haidaoSide_BaseRole, di_haidaoSide_BaseRole, STATE_PKWD, 10, 180)
            SystemNotice(role, "Haha ! Statue is now temporary invincible!")
        else
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
end

function GetChaName26_guildwar(role, npc)
    local map_name_role = GetChaMapName(role)
    if map_name_role == "guildwar" then
        local num_count_1 = CheckBagItem(role, 4011)

        local num_count_2 = CheckBagItem(role, 1720)

        if num_count_1 >= 12 and num_count_2 >= 12 then
            TakeItem(role, 0, 4011, 12)
            TakeItem(role, 0, 1720, 12)
            local min_atk = Mnatk(haidaoSide_JTRole_1)
            local max_atk = Mxatk(haidaoSide_JTRole_1)
            if min_atk == 0 then
                local min_atk = Mnatk(haidaoSide_JTRole_2)
                local max_atk = Mxatk(haidaoSide_JTRole_2)
            end
            local min_atk_after = min_atk + 25
            local max_atk_after = max_atk + 25
            SetChaAttrI(haidaoSide_JTRole_1, ATTR_MNATK, min_atk_after)
            SetChaAttrI(haidaoSide_JTRole_1, ATTR_MXATK, max_atk_after)
            SetChaAttrI(haidaoSide_JTRole_2, ATTR_MNATK, min_atk_after)
            SetChaAttrI(haidaoSide_JTRole_2, ATTR_MXATK, max_atk_after)
            SystemNotice(role, "Great ! Arrow Tower attack has rose!")
        elseif num_count_1 < 12 and num_count_2 < 12 then
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
    if map_name_role == "guildwar2" then
        local num_count_1 = CheckBagItem(role, 4011)

        local num_count_2 = CheckBagItem(role, 1720)

        if num_count_1 >= 12 and num_count_2 >= 12 then
            TakeItem(role, 0, 4011, 12)
            TakeItem(role, 0, 1720, 12)
            local min_atk = Mnatk(di_haidaoSide_JTRole_1)
            local max_atk = Mxatk(di_haidaoSide_JTRole_1)
            if min_atk == 0 then
                local min_atk = Mnatk(di_haidaoSide_JTRole_2)
                local max_atk = Mxatk(di_haidaoSide_JTRole_2)
            end
            local min_atk_after = min_atk + 25
            local max_atk_after = max_atk + 25
            SetChaAttrI(di_haidaoSide_JTRole_1, ATTR_MNATK, min_atk_after)
            SetChaAttrI(di_haidaoSide_JTRole_1, ATTR_MXATK, max_atk_after)
            SetChaAttrI(di_haidaoSide_JTRole_2, ATTR_MNATK, min_atk_after)
            SetChaAttrI(di_haidaoSide_JTRole_2, ATTR_MXATK, max_atk_after)
            SystemNotice(role, "Great ! Arrow Tower attack has rose!")
        elseif num_count_1 < 12 and num_count_2 < 12 then
            SystemNotice(role, "You havcen't collect enough resources!")
        end
    end
end

function GetChaName27_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 30741, 10702, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 30941, 10702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 30741, 10702, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 30841, 10702, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 30941, 10702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName28_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 51141, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 51200, 9800, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 51141, 9802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 51341, 9902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName29_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 11300, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 11241, 11802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11141, 11802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 11300, 11700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 11241, 11802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11141, 11802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName30_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 30700, 51700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 30841, 51602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 30941, 51702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 30700, 51770, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 30841, 51670, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 30941, 51702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName31_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 12000, 52100, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11900, 52000, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName32_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 15 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1000, 52300, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 52441, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 52541, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 15 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 15)

        local Monster1 = CreateChaX(1026, 52300, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 52200, 52000, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 52100, 52100, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName33_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 30600, 10200, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 30641, 10302, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 30541, 10102, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 30500, 10400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 30741, 10202, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 30741, 10202, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 30541, 10002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 30600, 10200, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 30641, 10302, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 30541, 10102, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 30500, 10400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 30741, 10202, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 30741, 10202, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 30541, 10002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName34_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 51200, 10900, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 51141, 10802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 51241, 10902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 51000, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 51141, 10702, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 51341, 10902, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 51041, 10802, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 51200, 10900, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 51141, 10802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 51241, 10902, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 51000, 10900, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 51141, 10702, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 51341, 10902, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 51041, 10802, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName35_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 11400, 11600, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 11541, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11541, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 11400, 11700, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 11341, 11802, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 11341, 11702, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 11441, 11802, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 11400, 11600, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 11541, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11541, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 11400, 11700, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 11341, 11802, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 11341, 11702, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 11441, 11802, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName36_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 30841, 51600, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 30741, 51600, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 30700, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 30641, 51500, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 30741, 51400, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 30741, 51300, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName37_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName38_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 30 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1000, 52100, 52200, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 52241, 52302, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 52041, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 52400, 52100, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 51941, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 51941, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 30 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 30)

        local Monster1 = CreateChaX(1026, 52100, 52200, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 52241, 52302, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 52041, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 52400, 52100, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 51941, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 51941, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName39_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 30700, 10500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)
        local Monster2 = CreateChaX(1000, 30641, 10502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)
        local Monster3 = CreateChaX(1000, 30541, 10602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 30500, 10400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)
        local Monster5 = CreateChaX(1000, 30741, 10402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)
        local Monster6 = CreateChaX(1000, 30541, 10502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 30641, 10402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)
        local Monster8 = CreateChaX(1000, 30841, 10402, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 30841, 10502, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 30741, 10502, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 30641, 10502, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1026, 30700, 10500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)
        local Monster2 = CreateChaX(1026, 30641, 10502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)
        local Monster3 = CreateChaX(1026, 30541, 10602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 30500, 10400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)
        local Monster5 = CreateChaX(1026, 30741, 10402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)
        local Monster6 = CreateChaX(1026, 30541, 10502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 30641, 10402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)
        local Monster8 = CreateChaX(1026, 30841, 10402, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 30841, 10502, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 30741, 10502, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 30641, 10502, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName40_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 52100, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 52241, 10802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 52141, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 52200, 10700, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 52041, 10702, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 52141, 10602, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 52341, 10602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1000, 52341, 10502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 52041, 10502, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 52041, 10602, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 52341, 10702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1026, 52100, 10700, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 52241, 10802, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 52141, 10802, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 52200, 10700, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 52041, 10702, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 52141, 10602, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 52341, 10602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1026, 52341, 10502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 52041, 10502, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 52041, 10602, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 52341, 10702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName41_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 11500, 11600, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 11441, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11541, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 11400, 11500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 11541, 11502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 11541, 11402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 11641, 11402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1000, 11641, 11502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 11641, 11702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 11641, 11402, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 11641, 11702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1026, 11500, 11600, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 11441, 11602, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11541, 11702, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 11400, 11500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 11541, 11502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 11541, 11402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 11641, 11402, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1026, 11641, 11502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 11641, 11702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 11641, 11402, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 11641, 11702, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName42_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)
        local Monster2 = CreateChaX(1000, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)
        local Monster3 = CreateChaX(1000, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)
        local Monster5 = CreateChaX(1000, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)
        local Monster6 = CreateChaX(1000, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)
        local Monster8 = CreateChaX(1000, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 30841, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 30741, 51302, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 30741, 51202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1026, 30700, 51500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)
        local Monster2 = CreateChaX(1026, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)
        local Monster3 = CreateChaX(1026, 30541, 51602, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 30500, 51400, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)
        local Monster5 = CreateChaX(1026, 30741, 51402, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)
        local Monster6 = CreateChaX(1026, 30541, 51502, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 30641, 51502, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)
        local Monster8 = CreateChaX(1026, 30841, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 30841, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 30741, 51302, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 30741, 51202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName43_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1000, 12041, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 12141, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 12641, 52202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 11900, 52100, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 11941, 52002, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 12000, 52000, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 12141, 52002, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 11841, 52102, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 11841, 52002, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1026, 12041, 51702, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 12141, 51902, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 12141, 52102, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 12641, 52202, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName44_guildwar(role)
    local num_count_1 = CheckBagItem(role, 3001)
    local map_name_cha = GetChaMapName(role)
    if num_count_1 >= 45 and map_name_cha == "guildwar" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1000, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1000, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1000, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1000, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1000, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1000, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1000, 52441, 52502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1000, 52641, 52702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1000, 52641, 52202, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1000, 52641, 52302, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    elseif num_count_1 >= 45 and map_name_cha == "guildwar2" then
        TakeItem(role, 0, 3001, 45)
        local Monster1 = CreateChaX(1000, 52300, 52500, 145, 310, role)
        SetChaLifeTime(Monster1, 300000)
        SetChaSideID(Monster1, 2)

        local Monster2 = CreateChaX(1026, 52241, 52502, 145, 310, role)
        SetChaLifeTime(Monster2, 300000)
        SetChaSideID(Monster2, 2)

        local Monster3 = CreateChaX(1026, 52341, 52402, 145, 310, role)
        SetChaLifeTime(Monster3, 300000)
        SetChaSideID(Monster3, 2)
        local Monster4 = CreateChaX(1026, 52000, 52500, 145, 310, role)
        SetChaLifeTime(Monster4, 300000)
        SetChaSideID(Monster4, 2)

        local Monster5 = CreateChaX(1026, 52141, 52502, 145, 310, role)
        SetChaLifeTime(Monster5, 300000)
        SetChaSideID(Monster5, 2)

        local Monster6 = CreateChaX(1026, 52241, 52402, 145, 310, role)
        SetChaLifeTime(Monster6, 300000)
        SetChaSideID(Monster6, 2)
        local Monster7 = CreateChaX(1026, 52241, 52602, 145, 310, role)
        SetChaLifeTime(Monster7, 300000)
        SetChaSideID(Monster7, 2)

        local Monster8 = CreateChaX(1026, 52441, 52502, 145, 310, role)
        SetChaLifeTime(Monster8, 300000)
        SetChaSideID(Monster8, 2)
        local Monster9 = CreateChaX(1026, 52641, 52702, 145, 310, role)
        SetChaLifeTime(Monster9, 300000)
        SetChaSideID(Monster9, 2)
        local Monster10 = CreateChaX(1026, 52641, 52202, 145, 310, role)
        SetChaLifeTime(Monster10, 300000)
        SetChaSideID(Monster10, 2)
        local Monster11 = CreateChaX(1026, 52641, 52302, 145, 310, role)
        SetChaLifeTime(Monster11, 300000)
        SetChaSideID(Monster11, 2)
        SystemNotice(role, "Succuessfully launched ambush!")
    else
        SystemNotice(role, "You do not have enough <<Pirate Token>> !")
    end
end

function GetChaName45_guildwar(character, npc)
    local i = CheckBagItem(character, 3849)

    if i ~= 1 then
        SystemNotice(character, "Please ensure that you have a Medal of Honor")
        return 0
    end

    local s = CheckBagItem(character, 2382)
    if s >= 1 then
        SystemNotice(character, "You can only carry one Life and Death token at one time")
        return 0
    end

    local retbag = HasLeaveBagGrid(character, 1)
    if retbag ~= LUA_TRUE then
        SystemNotice(character, "Insufficent inventory space to redeem")
        return
    end
    local role_RY = GetChaItem2(character, 2, 3849)
    local HonorPoint = GetItemAttr(role_RY, ITEMATTR_VAL_STR)
    local HonorPoint_now = HonorPoint - 15
    if HonorPoint < 15 then
        SystemNotice(character, "You do not have enough honor points to exchange!")
        return 0
    else
        SetItemAttr(role_RY, ITEMATTR_VAL_STR, HonorPoint_now)
        GiveItem(character, 0, 2382, 1, 4)
    end
end

RebirthVar = {}
RebirthVar.Level = {}
RebirthVar.Class = {}
RebirthVar.Wings = {}
RebirthVar.Skills = {467}

RebirthVar.Level[1] = {Name = "Phoenix Rebirth", EXP = {Min = 0, Max = 9000}, Stone = 2235}
RebirthVar.Level[2] = {Name = "Athene Rebirth", EXP = {Min = 9000, Max = 10000}, Stone = 5765}
RebirthVar.Level[3] = {Name = "Goddess Rebirth", EXP = {Min = 10000, Max = 12000}, Stone = 2235}

RebirthVar.Class[0] = {Skill = 459, Type = {0}}
RebirthVar.Class[8] = {Skill = 455, Type = {2}}
RebirthVar.Class[9] = {Skill = 453, Type = {1,3}}
RebirthVar.Class[12] = {Skill = 456, Type = {1,3}}
RebirthVar.Class[13] = {Skill = 458, Type = {3,4}}
RebirthVar.Class[14] = {Skill = 457, Type = {3,4}}
RebirthVar.Class[16] = {Skill = 454, Type = {1,3,4}}

RebirthVar.Wings[1] = {134, 136, 0}
RebirthVar.Wings[2] = {138, 139, 0}
RebirthVar.Wings[3] = {128, 129, 0}
RebirthVar.Wings[4] = {131, 132, 0}

function PlayerRebirth(Player, Level, Class)
    local PlayerName = GetChaDefaultName(Player)
    if IsEquip(Player) == LUA_TRUE or KitbagLock(Player, 0) ~= LUA_TRUE or GetChaFreeBagGridNum(Player) < 5 then
        SystemNotice(Player, "You must remove your equipment, unlock your inventory and have 5 free inventory slots.")
        LG("Player Rebirth", "[" ..PlayerName .."][" ..Level .."][" ..Class .. "]: Player is wearing equipment, inventory is locked or not enough free slots.")
        return 0
    end
    for i = 1, #RebirthVar.Class[Class].Type, 1 do
        if RebirthVar.Class[Class].Type[i] == GetChaTypeID(Player) then
            break
        elseif i == #RebirthVar.Class[Class].Type then
            SystemNotice(Player, "Character type mismatch for chosen class.")
            LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Character type mismatch for chosen class.")
            return 0
        end
    end
    if RebirthVar.Level[Level].EXP.Min ~= nil and GetChaAttr(Player, ATTR_CSAILEXP) < RebirthVar.Level[Level].EXP.Min then
        SystemNotice(Player, "You need at least " ..RebirthVar.Level[Level].EXP.Min .. " SEXP to do " .. RebirthVar.Level[Level].Name .. ".")
        LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Not enough SEXP.")
        return 0
    end
    if RebirthVar.Level[Level].EXP.Max ~= nil and GetChaAttr(Player, ATTR_CSAILEXP) >= RebirthVar.Level[Level].EXP.Max then
        SystemNotice(Player, "You have already done " .. RebirthVar.Level[Level].Name .. ".")
        LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Too much SEXP.")
        return 0
    end
    if CheckBagItem(Player, RebirthVar.Level[Level].Stone) < 1 then
        SystemNotice(Player, "You do not have " .. GetItemName(RebirthVar.Level[Level].Stone) .. ".")
        LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Missing Rebirth Stone.")
        return 0
    end
    if Level > 1 then
        if CheckBagItem(Player, RebirthVar.Wings[GetChaTypeID(Player)][Level - 1]) < 1 then
            SystemNotice(Player, "You do not have " .. GetItemName(RebirthVar.Wings[GetChaTypeID(Player)][Level - 1]) .. ".")
            LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Missing Rebirth Stone.")
            return 0
        elseif TakeItem(Player, 0, (RebirthVar.Wings[GetChaTypeID(Player)][Level - 1]), 1) == 0 then
            SystemNotice(Player, "Failed to remove " ..GetItemName(RebirthVar.Wings[GetChaTypeID(Player)][Level - 1]) .. ", please check again.")
            LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Failed to remove previous wings.")
            return 0
        end
    end
    if TakeItem(Player, 0, RebirthVar.Level[Level].Stone, 1) == 0 then
        SystemNotice(Player, "Failed to remove Rebirth Stone, please check again.")
        LG("Player Rebirth", "[" .. PlayerName .. "][" .. Level .. "][" .. Class .. "]: Failed to remove Rebirth Stone.")
        return 0
    end
    local Skills, Points = {}, GetChaAttr(Player, ATTR_TP)
    for i = 1, #RebirthVar.Skills, 1 do
        Skills[i] = {ID = RebirthVar.Skills[i], Level = 0}
        if GetSkillLv(Player, RebirthVar.Skills[i]) >= 1 then
            Skills[i].Level = GetSkillLv(Player, RebirthVar.Skills[i])
            Points = Points - GetSkillLv(Player, RebirthVar.Skills[i])
        end
    end
    for i = 0, 16, 1 do
        if RebirthVar.Class[i] ~= nil and GetSkillLv(Player, RebirthVar.Class[i].Skill) > 0 then
            Points = Points - GetSkillLv(Player, RebirthVar.Class[i].Skill)
        end
    end
    Points = Points + ClearAllFightSkill(Player)
    for i = 1, #Skills, 1 do
        if Skills[i].Level ~= 0 then
            AddChaSkill(Player, Skills[i].ID, Skills[i].Level, Skills[i].Level, 0)
        end
    end
    SetChaAttr(Player, ATTR_TP, Points)
    local STATS = (GetChaAttr(Player, ATTR_AP) + GetChaAttr(Player, ATTR_BSTR) + GetChaAttr(Player, ATTR_BDEX) + GetChaAttr(Player, ATTR_BAGI) + GetChaAttr(Player, ATTR_BCON) + GetChaAttr(Player, ATTR_BSTA)) - 25
    SetChaAttr(Player, ATTR_BSTR, 5)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_BDEX, 5)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_BAGI, 5)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_BCON, 5)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_BSTA, 5)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_AP, STATS)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_JOB, Class)
    RefreshCha(Player)
    AddChaSkill(Player, 459, Level, Level, 0)
    AddChaSkill(Player, RebirthVar.Class[Class].Skill, Level, Level, 0)
    GiveItem(Player, 0, RebirthVar.Wings[GetChaTypeID(Player)][Level], 1, 4)
    SetChaAttr(Player, ATTR_CSAILEXP, RebirthVar.Level[Level].EXP.Max)
    GMNotice("Extreme celebration, "..PlayerName .." has completed " ..RebirthVar.Level[Level].Name.." successful. Blessing from the whole server! "..PlayerName .." hope you have a safe journey and everything goes your way!")
end

function ChangeItem(character, npc)
    local Item_CanGet = GetChaFreeBagGridNum(character)
    if Item_CanGet < 1 then
        SystemNotice(character, "Please make sure you have one space left")
        return 0
    end

    local am1 = CheckBagItem(character, 3066)
    if am1 < 1 then
        SystemNotice(character, "")
        return 0
    end
    local Money_Need = 50000
    local Money_Have = GetChaAttr(character, ATTR_GD)
    if Money_Need > Money_Have then
        SystemNotice(character, "")
        return 0
    else
        TakeMoney(character, nil, Money_Need)
    end

    local r1 = 0
    local r2 = 0
    r1, r2 = MakeItem(character, 3666, 10, 4)
    local Item_el = GetChaItem(character, 2, r2)

    local item_old = GetChaItem2(character, 2, 3066)

    local old_month = GetItemAttr(item_old, ITEMATTR_VAL_STA)
    local old_day = GetItemAttr(item_old, ITEMATTR_VAL_STR)
    local old_hour = GetItemAttr(item_old, ITEMATTR_VAL_CON)
    local old_miniute = GetItemAttr(item_old, ITEMATTR_VAL_DEX)

    SetItemAttr(Item_el, ITEMATTR_VAL_STA, old_month)
    SetItemAttr(Item_el, ITEMATTR_VAL_STR, old_day)
    SetItemAttr(Item_el, ITEMATTR_VAL_CON, old_hour)
    SetItemAttr(Item_el, ITEMATTR_VAL_DEX, old_miniute)

    local old_month2 = GetItemAttr(Item_el, ITEMATTR_VAL_STA)
    local old_day2 = GetItemAttr(Item_el, ITEMATTR_VAL_STR)
    local old_hour2 = GetItemAttr(Item_el, ITEMATTR_VAL_CON)
    local old_miniute2 = GetItemAttr(Item_el, ITEMATTR_VAL_DEX)

    SynChaKitbag(character, 13)
end
