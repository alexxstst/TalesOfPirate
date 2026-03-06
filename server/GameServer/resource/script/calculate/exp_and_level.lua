print("-- [Loading] Exp And Level")
function AskGuildItem(role)
    local gold = GetChaAttr(role, ATTR_GD)
    local fame = GetChaAttr(role, ATTR_FAME)
    local attr_guild = HasGuild(role)
    if attr_guild ~= 0 then
        HelpInfoX(role, 0, "Already in a guild")
        return 0
    end
    local Lv = Lv(role)
    if Lv < 40 then
        SystemNotice(role, "Have not reached Lv 40. Unable to create")
        return 0
    end

    if Guild_ItemMax > 0 then
        for i = 1, Guild_ItemMax, 1 do
            local K = Check_BagItem(role, Guild_item[i], Guild_count[i])
            if K == 0 then
                HelpInfoX(role, 0, "Items insufficient. Unable to create")
                return 0
            end
        end
    end
    if gold < Guild_Gold then
        HelpInfoX(role, 0, "Insufficient gold. Unable to create")
        return 0
    end
    if fame < Guild_fame then
        HelpInfoX(role, 0, "Insufficient reputation to create")
        return 0
    end

    return 1
end

function Check_BagItem(role, a, b)
    local a = CheckBagItem(role, a)
    if a >= b then
        return 1
    else
        return 0
    end
end

function DeductGuildItem(role)
    local gold = GetChaAttr(role, ATTR_GD)
    local fame = GetChaAttr(role, ATTR_FAME)
    local attr_guild = HasGuild(role)


    DelBagItem(role, 1780, 1)
    gold = gold - Guild_Gold
    fame = fame - Guild_fame
    SetAttrChangeFlag(role)

    SetChaAttr(role, ATTR_GD, gold)
    SetChaAttr(role, ATTR_FAME, fame)

    SyncChar(role, 4)

end

function AskJoinGuild(role)
    if HasGuild(role) ~= 0 then
        HelpInfoX(role, 0, "Already in a guild")
        return 0
    end
    return 1
end

function GetExp_New(Dead, Killer)
    if ValidCha(Killer) == 0 then
        return
    end
    local A = Check_Combat_Mod(Dead, Killer)
    if A == 1 then
        GetExp_PKM(Dead, Killer)
    elseif A == 2 then
        GetExp_MKP(Dead, Killer)
    elseif A == 3 then
        GetExp_PKP(Dead, Killer)
    elseif A == 4 then
        GetExp_Noexp(Dead, Killer)
    else
		return
    end
end

function Check_Combat_Mod(Dead, Killer)
    local ATKER = IsPlayer(Killer)
    local DEFER = IsPlayer(Dead)
    if (ATKER == 0) and (DEFER == 0) then
        return 4
    elseif (ATKER == 0 and DEFER == 1) then
        return 2
    elseif (ATKER == 1 and DEFER == 0) then
        return 1
    elseif (ATKER == 1 and DEFER == 1) then
        return 3
    else
        return
    end
end

function GetExp_PKM(dead, atk)
	local MonsterLv = GetChaAttrI(dead, ATTR_LV)
	local MonsterEXP = GetChaAttrI(dead, ATTR_CEXP)
	local kPlayer = {}
	local k_exp = {}
	kPlayer[0] = GetChaHarmByNo(dead, 0)
	kPlayer[1] = GetChaHarmByNo(dead, 1)
	kPlayer[2] = GetChaHarmByNo(dead, 2)
	kPlayer[3] = GetChaHarmByNo(dead, 3)
	kPlayer[4] = GetChaHarmByNo(dead, 4)
	
	for i = 0, 4, 1 do
		k_exp[i] = 0
		if ValidCha(kPlayer[i]) == 1 then
			k_exp[i] = MonsterEXP
			ShareTeamExp(dead, kPlayer[i], k_exp[i], atk)
		end
	end
	
	local item_host = 0
	local exp_max = k_exp[0]
	for i = 1, 4, 1 do
		if k_exp[i] > exp_max  then
			exp_max = k_exp[i]
			item_host = i
		end
	end
	if ChaIsBoat(kPlayer[item_host]) == 1 then
		local ShipLv = GetChaAttr(kPlayer[item_host] , ATTR_LV)
		local PlayerLv = GetChaAttr(TurnToCha(kPlayer[item_host]) , ATTR_LV)
		local ShipEXP = GetChaAttr(kPlayer[item_host], ATTR_CEXP)
		local ShipExpAdd = math.floor(math.min (7, (MonsterLv/10+2)) * Server.Rates.Global.ShipEXP)
		local LvLimit = math.min(ShipLv, PlayerLv) - 10
		if MonsterLv >= LvLimit then
			ShipEXP = ShipEXP + ShipExpAdd
			SetCharaAttr(ShipEXP, kPlayer[item_host] ,ATTR_CEXP)
		end
	end
	SetItemHost(dead, kPlayer[item_host])
end

function ValidCha(ter)
    if ter == nil or ter == 0 then
        return 0
    end
    return 1
end

function AntiLevelJump(Player, EXP)
    local Jump = 1
    local ExpNeeded = DEXP[Lv(Player) + Jump]
    local CurrentExp = GetChaAttr(Player, ATTR_CEXP)
    local RequiredExp = ExpNeeded - CurrentExp
    if EXP > RequiredExp then
        return RequiredExp
    else
        return EXP
    end
end

function ShareTeamExp(DEAD, ATKER, DEAD_EXP, KILLER)
    if ValidCha(ATKER) == 0 then
        return
    end
	
    if ChaIsBoat(ATKER) == 1 and IsChaInLand(DEAD) == 1 then
        DEAD_EXP = math.floor(DEAD_EXP / 5 + 1)
    end
	
    local t = {}
    t[0] = ATKER
    t[1] = GetTeamCha(ATKER, 0)
    t[2] = GetTeamCha(ATKER, 1)
    t[3] = GetTeamCha(ATKER, 2)
    t[4] = GetTeamCha(ATKER, 3)

    local Count = 0
    local NewPlayer_CanGet = 0
    local NewPlayer_Lv = 0
    local NewPlayer_Lv_dif = -100
    local Check_Killer = 0
    local Level_Monster = GetChaAttrI(DEAD, ATTR_LV)
    if t[0] == KILLER then
        Check_Killer = 1
    end
    for i = 0, 4, 1 do
        if ValidCha(t[i]) == 1 then
            a = CheckExpShare(t[i], ATKER)
            if a == 1 then
                Count = Count + 1
                NewPlayer_Lv = Lv(TurnToCha(t[i]))
                NewPlayer_Lv_dif = Level_Monster - NewPlayer_Lv
                if NewPlayer_Lv <= 30 and NewPlayer_Lv >= 10 and NewPlayer_Lv_dif >= -5 then
                    NewPlayer_CanGet = NewPlayer_CanGet + 1
                end
            end
        end
    end
	
    if Count == 0 then
        return
    end
	
    for i = 0, 4, 1 do
		if ValidCha(t[i]) == 1 then
			if CheckExpShare(t[i], ATKER) == 1 then
				local a = 1
				local b = 1
				local Level_Player = GetChaAttrI(TurnToCha(t[i]), ATTR_LV)
				local Level_DIF = Level_Player - Level_Monster
				local ExpAdd = DEAD_EXP
				if Level_DIF >= 4 then
					b = math.min(10, 1 + (math.abs(Level_DIF - 4) * 0.4))
				elseif Level_DIF <= -1 * 10 then
					b = math.min(4, 1 + math.abs(Level_DIF - 10) * 0.1)
				end
				
				ExpAdd = math.floor(math.max(1, ExpAdd / b)) * a
				
				if Count >= 3 and NewPlayer_CanGet <= 0 and Check_Killer == 1 then
					if Level_DIF <= 3 then
						Add_RYZ_TeamPoint(TurnToCha(t[i]), Count, 1)
					end
				end
				
				if Level_Player >= 50 and NewPlayer_CanGet > 0 and Check_Killer == 1 then
					Add_RYZ_TeamPoint(TurnToCha(t[i]), 6, NewPlayer_CanGet)
				end
				
				local LuckyStrike = 1
				local Effect = 0
				local CheckLucky = 0
				CheckLucky = CheckLuckyFinish(Effect)
				if CheckLucky == 1 then
					LuckyStrike = LuckyStrike * 2
					SystemNotice(TurnToCha(t[i]), "Lucky Strike, obtained double experience.")
				elseif CheckLucky == 2 then
					LuckyStrike = LuckyStrike * 5
					SystemNotice(TurnToCha(t[i]), "Lucky Strike, obtained 5x experience.")
				end
				
                local rate = GetPlyExpRate(t[i])
				ExpAdd = ExpAdd * LuckyStrike * rate

				local expGetNow = ExpAdd
				local expCanGive = 0
				t[i] = TurnToCha(t[i])
				local ptnItem = GetEquipItemP( t[i] , 8)	
				local IdItem = GetItemID(ptnItem)	
				local lvPerson = GetChaAttr(t[i] , ATTR_LV)
				if IdItem == 1034 and lvPerson < 41 then
					local expItemNow = GetItemAttr(ptnItem, ITEMATTR_URE) * 10
					local expItemMax = GetItemAttr(ptnItem, ITEMATTR_MAXURE) * 10
					local retIsInTeam = IsInTeam(t[i])
					if retIsInTeam ~= LUA_TRUE then
						expItemNow = expItemNow + expGetNow
						if expItemNow >= expItemMax then
							expItemNow = expItemMax
						end
					else
						if HasTeammate(t[i], 0, 5) == LUA_TRUE then
							local ptnLowLvPlayer = returnLowLVPlayer(t[i], t[0], t[1], t[2], t[3], t[4])
							if ValidCha(ptnLowLvPlayer) == 1 then
								local expCanUse = expItemNow - 1000
									if expGetNow * 2 >= expCanUse then
										expCanGive = expCanUse
										expItemNow = 1000
									else
										expItemNow = expItemNow - expGetNow*2
										expCanGive = expGetNow * 2
									end
									if expItemNow <= 1000 then
										expItemNow = 1000
									end
								ExpAdd = ExpAdd + expCanGive
							else
								expCanGive = 0
								ExpAdd = ExpAdd + expCanGive
							end
						end
					end
					expItemNow = math.floor(expItemNow / 10)
					if expItemNow == (expItemMax/10) then
						SystemNotice(t[i], GetItemName(IdItem)..' is full!')
					else
						SystemNotice(t[i], GetItemName(IdItem)..' charged up...')
					end
					SetItemAttr(ptnItem, ITEMATTR_URE, expItemNow)
					SynLook(t[i])
				end
				ExpSystem(t[i], ExpAdd)
			end
		end
    end
end

function returnLowLVPlayer(PlayerNow, Player1, Player2, Player3, Player4, Player5)
    local lvPlayerNow = GetChaAttr(PlayerNow, ATTR_LV)
    PlayerNow = TurnToCha(PlayerNow)
    Player1 = TurnToCha(Player1)
    Player2 = TurnToCha(Player2)
    Player3 = TurnToCha(Player3)
    Player4 = TurnToCha(Player4)
    Player5 = TurnToCha(Player5)

    if ValidCha(Player1) == 1 then
        local lvPlayer1 = GetChaAttr(Player1, ATTR_LV)
        if lvPlayerNow > lvPlayer1 + 5 then
            return Player1
        end
    end

    if ValidCha(Player2) == 1 then
        local lvPlayer2 = GetChaAttr(Player2, ATTR_LV)
        if lvPlayerNow > lvPlayer2 + 5 then
            return Player2
        end
    end

    if ValidCha(Player3) == 1 then
        local lvPlayer3 = GetChaAttr(Player3, ATTR_LV)
        if lvPlayerNow > lvPlayer3 + 5 then
            return Player3
        end
    end

    if ValidCha(Player4) == 1 then
        local lvPlayer4 = GetChaAttr(Player4, ATTR_LV)
        if lvPlayerNow > lvPlayer4 + 5 then
            return Player4
        end
    end

    if ValidCha(Player5) == 1 then
        local lvPlayer5 = GetChaAttr(Player5, ATTR_LV)
        if lvPlayerNow > lvPlayer5 + 5 then
            return Player5
        end
    end

    return 0
end

function CheckExpShare(ti, atk)
    if ValidCha(ti) == 0 then
        LG("luascript_err", "fucntion CheckExpShare : party member count as null\n")
        return 0
    end

    if IsInSameMap(atk, ti) == 0 then
        return 0
    end

    local pos_ti_x, pos_ti_y = GetChaPos(ti)
    if ValidCha(atk) == 0 then
        LG("luascript_err", "fucntion CheckExpShare :  Monster killer as null\n")
        return 0
    end

    local pos_atk_x, pos_atk_y = GetChaPos(atk)
    local distance = Dis(pos_ti_x, pos_ti_y, pos_atk_x, pos_atk_y)
    if distance >= 4000 then
        return 0
    end
    if IsChaInRegion(ti, 2) == 1 then
        return 0
    end
    return 1
end

function Dead_Punish(dead, atk)
    local Role_ID = GetRoleID(dead)
    BBBB[Role_ID] = 0
    local map_name = GetChaMapName(dead)
    if map_name == "leiting2" or map_name == "binglang2" or map_name == "shalan2" or map_name == "guildwar" or map_name == "guildwar2" then
        return
    end
    if map_name == "garner2" then
        SetCharaAttr(0, dead, ATTR_SP)
        return
    end
    dead = TurnToCha(dead)
    local lv = GetChaAttr(dead, ATTR_LV)
    local check_pirate = CheckItem_pirate(dead)
    local check_death = CheckItem_Death(dead)
    local Time = os.date("%H")
    local TimeNum = tonumber(Time)
    if lv <= 10 then
        return
    end
    if lv >= 70 and check_pirate == 1 then
        if TimeNum <= 6 or TimeNum >= 18 then
            SystemNotice(dead, "Received blessing from moonlight. Death penalty will be removed")
            return
        end
    end
    if lv >= 75 and check_death == 1 then
        if TimeNum <= 6 or TimeNum >= 18 then
            SystemNotice(dead, "Blessed by Death. No death penalty upon character death")
            return
        end
    end
    local exp_red
    local exp = Exp(dead)
    local nlexp = GetChaAttrI(dead, ATTR_NLEXP)
    local clexp = GetChaAttrI(dead, ATTR_CLEXP)
    if exp <= clexp then
        exp_red = 0
    else
        exp_red = math.min(math.floor((nlexp - clexp) * 0.02), math.max(exp - clexp, 0))
    end
    SetCharaAttr(0, dead, ATTR_SP)
    local i1 = CheckBagItem(dead, 3846)
    local i2 = CheckBagItem(dead, 3047)
    local i3 = CheckBagItem(dead, 5609)
    if map_name == "secretgarden" or map_name == "teampk" then
        SetCharaAttr(0, dead, ATTR_SP)
        return
    end
    local i = CheckBagItem(dead, 2954)
    if i == 1 then
        local Dead_BK = GetChaItem2(dead, 2, 2954)
        local DeadPoint = GetItemAttr(Dead_BK, ITEMATTR_VAL_STR)
        local DeadPoint = DeadPoint + 1
        SetItemAttr(Dead_BK, ITEMATTR_VAL_STR, DeadPoint)
        local DeadPoint1 = GetItemAttr(Dead_BK, ITEMATTR_VAL_STR)
        if DeadPoint >= 100 then
        end
    end
    if i1 <= 0 and i2 <= 0 and i3 <= 0 then
        exp = Exp(dead) - exp_red
        if Lv(dead) >= 80 then
            exp_red_80 = exp_red * 50
            SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
        else
            SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
        end
        SetChaAttrI(dead, ATTR_CEXP, exp)
        if lv > 20 then
            Dead_Punish_ItemURE(dead)
        end
    elseif i1 ~= 0 and i2 == 0 and i3 == 0 then
        local j1 = TakeItem(dead, 0, 3846, 1)
        if j1 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    elseif i2 ~= 0 and i1 == 0 and i3 == 0 then
        local j2 = TakeItem(dead, 0, 3047, 1)
        if j2 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    elseif i3 ~= 0 and i1 == 0 and i2 == 0 then
        local j2 = TakeItem(dead, 0, 5609, 1)
        if j2 == 0 then
            SystemNotice(dead, "Stand-in Token delection failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Stand-in Token replace death penalty")
        end
    elseif i1 ~= 0 and i2 ~= 0 and i3 == 0 then
        local j1 = TakeItem(dead, 0, 3846, 1)
        if j1 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    elseif i2 ~= 0 and i3 ~= 0 and i1 == 0 then
        local j2 = TakeItem(dead, 0, 3047, 1)
        if j2 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    elseif i1 ~= 0 and i3 ~= 0 and i2 == 0 then
        local j1 = TakeItem(dead, 0, 3846, 1)
        if j1 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    elseif i1 ~= 0 and i2 ~= 0 and i3 ~= 0 then
        local j1 = TakeItem(dead, 0, 3846, 1)
        if j1 == 0 then
            SystemNotice(dead, "Voodoo Doll deletion failed")
            exp = Exp(dead) - exp_red
            if Lv(dead) >= 80 then
                exp_red_80 = exp_red * 50
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red_80)
            else
                SystemNotice(dead, "Death penalty. EXP lost: " .. exp_red)
            end
            SetChaAttrI(dead, ATTR_CEXP, exp)
            if lv > 20 then
                Dead_Punish_ItemURE(dead)
            end
        else
            SystemNotice(dead, "Voodoo Doll replace death penalty")
        end
    end
end

function GetExp_MKP(Dead, Killer)
    Dead_Punish(Dead, Killer)
end

function GetExp_PKP(Dead, Killer)

end

function GetExp_Noexp(Dead, Killer)

end

function Relive(role)
    local mxhp = GetChaAttr(role, ATTR_MXHP)
    local mxsp = GetChaAttr(role, ATTR_MXSP)
    local hp = math.max(1, math.floor((mxhp * 0.01) + 0.5))
    local sp = math.max(1, math.floor((mxsp * 0.01) + 0.5))
    SetCharaAttr(hp, role, ATTR_HP)
    SetCharaAttr(sp, role, ATTR_SP)
end

function Relive_now(role, sklv)
    local cha_role = TurnToCha(role)
    local hp = math.max(1, math.floor(0.05 * sklv * Mxhp(cha_role)))
    local sp = math.max(1, math.floor(0.05 * sklv * Mxsp(cha_role)))
    SetCharaAttr(hp, cha_role, ATTR_HP)
    SetCharaAttr(sp, cha_role, ATTR_SP)
end

function Ship_ShipDieAttr(role)
    local bmxhp = GetChaAttr(role, ATTR_BMXHP)
    local dead_count = GetChaAttr(role, ATTR_BOAT_DIECOUNT)
    LG("shipmxhp", "___a new dead ship_____________________________________________________")
    LG("shipmxhp", "role = ", role, "dead_count = ", dead_count, "form_mxhp = ", bmxhp)
    bmxhp = bmxhp * math.max(0, (1 - 0.02 - dead_count * 0.01))
    LG("shipmxhp", "role = ", role, "now_mxhp = ", bmxhp)
    SetCharaAttr(bmxhp, role, ATTR_BMXHP)
end

function BoatLevelUpProc(cha, boat, levelup, exp, money)
    if ValidCha(cha) == 0 then
        LG("luascript_err", "function BoatLevelUpProc : cha as null")
        return 0
    end
    if ValidCha(boat) == 0 then
        LG("luascript_err", "function BoatLevelUpProc : boat as null")
        return 0
    end
    PRINT("BoatLevelUpProc: levelup = , exp = , money = ", levelup, exp, money)
    local lv_up = levelup
    local req_exp = exp
    local req_gold = money
    local boat_lv = GetChaAttr(boat, ATTR_LV)
    if boat_lv ~= lv_up - 1 then
        PRINT("BoatLevelUpProc:function BoatLevelUpProc :ship current level and upgrade level does not match")
        LG("luascript_err", "function BoatLevelUpProc :ship current level and level to upgrade does not match")
        return 0
    end

    local boat_exp = GetChaAttr(boat, ATTR_CEXP)
    if boat_exp < req_exp then
        SystemNotice(cha, "Insufficient EXP to level up ship. Please try harder")
        return 0
    end

    local cha_money = GetChaAttr(cha, ATTR_GD)
    if cha_money < req_gold then
        SystemNotice(cha, "Insufficient gold to level up ship. Please try harder")
        return 0
    end
    PRINT("BoatLevelUpProc: boat_exp, req_exp, cha_money, req_gold", boat_exp, req_exp, cha_money, req_gold)
    SetAttrChangeFlag(boat)
    SetAttrChangeFlag(cha)

    boat_exp = boat_exp - req_exp
    SetCharaAttr(boat_exp, boat, ATTR_CEXP)
    cha_money = cha_money - req_gold
    SetCharaAttr(cha_money, cha, ATTR_GD)
    SetCharaAttr(lv_up, boat, ATTR_LV)
    ALLExAttrSet(boat)
    SystemNotice(cha, "Ship level up successfully")
    SystemNotice(cha, "Gold Deducted" .. req_gold)
    SystemNotice(cha, "Consume experience: " .. req_exp)
    SyncBoat(boat, 4)
    SyncChar(cha, 4)
    PRINT("BoarLevelUpProc: return 1")
    return 1
end

function Ship_Tran(buyer, boat)
    local ship_lv = GetChaAttr(boat, ATTR_LV)
    local ship_exp = GetChaAttr(boat, ATTR_CEXP)

    SetAttrChangeFlag(boat)

    ship_lv = math.max(1, math.max(math.floor(ship_lv / 2), ship_lv - 10))
    ship_exp = 0
    SetCharaAttr(ship_exp, boat, ATTR_CEXP)
    SetCharaAttr(ship_lv, boat, ATTR_LV)
    SystemNotice(buyer, "After trade, ship level becomes " .. ship_lv)
    SystemNotice(buyer, "After trade, ship experience reduced to " .. ship_exp)

    SyncBoat(boat, 4)
end

function CheckLuckyFinish(Effect)
    if Effect >= 4 then
        local Huge_Lucky = Percentage_Random(0.01)
        if Huge_Lucky == 1 then
            return 2
        end
    end
    local a = 0.02
    local b = Percentage_Random(a)
    return b
end
