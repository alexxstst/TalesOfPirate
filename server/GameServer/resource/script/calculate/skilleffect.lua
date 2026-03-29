print("--------------------------------------------------")
print("[**] Calculate Files [**]")
print("-- [Loading] SkillEffect")

dofile(GetResPath("script\\calculate\\exp_and_level.lua"))
dofile(GetResPath("script\\calculate\\JobType.lua"))
dofile(GetResPath("script\\calculate\\AttrType.lua"))
dofile(GetResPath("script\\calculate\\Init_Attr.lua"))
dofile(GetResPath("script\\calculate\\ItemAttrType.lua"))
dofile(GetResPath("script\\calculate\\functions.lua"))
dofile(GetResPath("script\\calculate\\AttrCalculate.lua"))
dofile(GetResPath("script\\calculate\\ItemEffect.lua"))
dofile(GetResPath("script\\calculate\\variable.lua"))
dofile(GetResPath("script\\calculate\\Look.lua"))
dofile(GetResPath("script\\calculate\\forge.lua"))
dofile(GetResPath("script\\calculate\\ItemGetMission.lua"))

CheckDmgChaNameTest = {}
CheckDmgChaNameTest[0] = "ReYK"
CheckDmgChaNameTest[1] = "Carsise"
CheckDmgChaNameTest[2] = "I am rubbish"
CheckDmgChaNameTest[3] = "CG mao mao"
CheckDmgChaNameTest[4] = "Chief mate against"

BOSSXYSJ = {}
BOSSXYSJ[979] = 1
BOSSXYSJ[980] = 12
BOSSXYSJ[981] = 6
BOSSXYSJ[982] = 4
BOSSXYSJ[983] = 12
BOSSXYSJ[984] = 16
BOSSXYSJ[985] = 16
BOSSXYSJ[986] = 12
BOSSXYSJ[987] = 4
BOSSXYSJ[988] = 4

BOSSSJSJ = {}
BOSSSJSJ[979] = 8
BOSSSJSJ[980] = 1
BOSSSJSJ[981] = 6
BOSSSJSJ[982] = 4
BOSSSJSJ[983] = 12
BOSSSJSJ[984] = 16
BOSSSJSJ[985] = 16
BOSSSJSJ[986] = 12
BOSSSJSJ[987] = 4
BOSSSJSJ[988] = 4

BOSSTJSJ = {}
BOSSTJSJ[979] = 8
BOSSTJSJ[980] = 1
BOSSTJSJ[981] = 6
BOSSTJSJ[982] = 4
BOSSTJSJ[983] = 12
BOSSTJSJ[984] = 16
BOSSTJSJ[985] = 16
BOSSTJSJ[986] = 12
BOSSTJSJ[987] = 4
BOSSTJSJ[988] = 4

BOSSXZSJ = {}
BOSSXZSJ[979] = 12
BOSSXZSJ[980] = 12
BOSSXZSJ[981] = 16
BOSSXZSJ[982] = 16
BOSSXZSJ[983] = 12
BOSSXZSJ[984] = 4
BOSSXZSJ[985] = 1
BOSSXZSJ[986] = 6
BOSSXZSJ[987] = 4
BOSSXZSJ[988] = 4

BOSSAYSJ = {}
BOSSAYSJ[979] = 12
BOSSAYSJ[980] = 12
BOSSAYSJ[981] = 16
BOSSAYSJ[982] = 16
BOSSAYSJ[983] = 12
BOSSAYSJ[984] = 4
BOSSAYSJ[985] = 1
BOSSAYSJ[986] = 6
BOSSAYSJ[987] = 4
BOSSAYSJ[988] = 4

Magic_rate1 = {}
Magic_rate1[JOB_TYPE_XINSHOU] = 1
Magic_rate1[JOB_TYPE_JIANSHI] = 1
Magic_rate1[JOB_TYPE_LIEREN] = 1
Magic_rate1[JOB_TYPE_SHUISHOU] = 1
Magic_rate1[JOB_TYPE_MAOXIANZHE] = 1.5
Magic_rate1[JOB_TYPE_QIYUANSHI] = 1.5
Magic_rate1[JOB_TYPE_JISHI] = 1
Magic_rate1[JOB_TYPE_SHANGREN] = 1
Magic_rate1[JOB_TYPE_JUJS] = 1
Magic_rate1[JOB_TYPE_SHUANGJS] = 1
Magic_rate1[JOB_TYPE_JIANDUNSHI] = 1
Magic_rate1[JOB_TYPE_XUNSHOUSHI] = 1
Magic_rate1[JOB_TYPE_JUJISHOU] = 1
Magic_rate1[JOB_TYPE_SHENGZHIZHE] = 2
Magic_rate1[JOB_TYPE_FENGYINSHI] = 3
Magic_rate1[JOB_TYPE_CHUANZHANG] = 1
Magic_rate1[JOB_TYPE_HANGHAISHI] = 2
Magic_rate1[JOB_TYPE_BAOFAHU] = 1
Magic_rate1[JOB_TYPE_GONGCHENGSHI] = 1

Magic_rate2 = {}
Magic_rate2[JOB_TYPE_XINSHOU] = 0.4
Magic_rate2[JOB_TYPE_JIANSHI] = 0.4
Magic_rate2[JOB_TYPE_LIEREN] = 0.4
Magic_rate2[JOB_TYPE_SHUISHOU] = 0.4
Magic_rate2[JOB_TYPE_MAOXIANZHE] = 0.3
Magic_rate2[JOB_TYPE_QIYUANSHI] = 0.3
Magic_rate2[JOB_TYPE_JISHI] = 0.4
Magic_rate2[JOB_TYPE_SHANGREN] = 0.4
Magic_rate2[JOB_TYPE_JUJS] = 0.4
Magic_rate2[JOB_TYPE_SHUANGJS] = 0.4
Magic_rate2[JOB_TYPE_JIANDUNSHI] = 0.4
Magic_rate2[JOB_TYPE_XUNSHOUSHI] = 0.4
Magic_rate2[JOB_TYPE_JUJISHOU] = 0.4
Magic_rate2[JOB_TYPE_SHENGZHIZHE] = 0.35
Magic_rate2[JOB_TYPE_FENGYINSHI] = 0.45
Magic_rate2[JOB_TYPE_CHUANZHANG] = 0.4
Magic_rate2[JOB_TYPE_HANGHAISHI] = 0.35
Magic_rate2[JOB_TYPE_BAOFAHU] = 0.4
Magic_rate2[JOB_TYPE_GONGCHENGSHI] = 0.4

function Check_Baoliao(ATKER, DEFER, ...)
	local arg = {...}
	local ArgCount = #arg
    local LevelDIF = Lv(TurnToCha(DEFER)) - Lv(TurnToCha(ATKER))
    local Count = 0
    local Member = 0
    local MF_RAID_STATE = 1
    local MF_RAID_PARTY = 1

    item = {}
    if IsPlayer(DEFER) == 1 then
        if IsInGymkhana(DEFER) == 1 then
            Count = 1
            if LevelDIF >= 5 then
                item[Count] = 1
            elseif LevelDIF <= (-5) then
                item[Count] = 3
            else
                item[Count] = 2
            end
            SetItemFall(Count, item[1], item[2], item[3], item[4], item[5], item[6], item[7], item[8], item[9], item[10], item[11], item[12], item[13], item[14], item[15])
        end
    else
        if ArgCount <= 0 or ArgCount > 15 then
            return
        end
        local plyRate = GetPlyDropRate(ATKER)
        for i = 1, ArgCount, 1 do
            if arg[i] >= 100 then
                mf = math.min(1, 100 / arg[i]) * plyRate
                local retExpState = GetExpState(ATKER)
                mf = mf * GetExpState(ATKER) / 100
                a = Percentage_Random(mf)
                if a == 1 then
                    Count = Count + 1
                    item[Count] = i
                end
            end
        end		
		SetItemFall(Count, item[1], item[2], item[3], item[4], item[5], item[6], item[7], item[8], item[9], item[10], item[11], item[12], item[13], item[14], item[15])
	end
end

function Check_SpawnResource(ATKER, DEFER, LvSkill, DropCount, ...)
	local arg = {...}
    local Item = {}
    local Rate = 1
    local Count = 0
    local Percentage = 0
    if DropCount <= 0 or DropCount > 10 then
        return
    end
	
    for A = 1, #Resources.GoldItems, 1 do
        local item = GetEquipItemP(ATKER, 9)
        if item ~= nil then
            if GetItemID(item) == Resources.GoldItems[A] then
                Rate = 2
            end
        end
    end

    for ID, Type in pairs(Resources.Uncommon) do
        for B = 1, #Type do
            if GetChaTypeID(DEFER) == Type[B] then
                LvSkill = 0
            end
        end
    end
	
	local Increase = 0.01
    local Seconds = 5        
    local PosX, PosY = GetChaPos(DEFER)
    local ResID = tostring(GetChaTypeID(DEFER))..tostring(PosX)..tostring(PosY)
    local PlayerID = GetRoleID(ATKER)
    local PlayerCount = 0
    ResourceCheck = ResourceCheck or {}
    ResourceCheck[ResID] = ResourceCheck[ResID] or {}
    ResourceCheck[ResID][PlayerID] = os.time()
    for Name, Time in pairs(ResourceCheck[ResID]) do
        if (os.time() - Time) <= Seconds then
            PlayerCount = PlayerCount + 1
        end
    end
	
    Rate = Rate * (Resource_RAID_ADJUST + (PlayerCount * Increase))
    for i = 1, DropCount, 1 do
        if arg[i] >= 100 then
            Percentage = math.min(1, 100 / arg[i] * (1 + LvSkill * 0.1)) * Rate
            if Percentage_Random(Percentage) == 1 then
                Count = Count + 1
                Item[Count] = i
            end
        end
    end
	
    if Count >= 1 then
        Item[1] = Item[Count]
        Count = 1
    end

	SetItemFall(Count, Item[1], Item[2], Item[3], Item[4], Item[5], Item[6], Item[7], Item[8], Item[9], Item[10], Item[11], Item[12], Item[13], Item[14], Item[15])
end

function CheckCha_ResourceItemUse(role)
    local Item_Use = GetChaItem(role, 1, 9)
    local ItemID_Use = GetItemID(Item_Use)

    if ItemID_Use == 207 or ItemID_Use == 208 then
        return 1
    end
    return 0
end

function SetSus(role, sus)
    if sus == 0 then
        SkillMiss(role)
    elseif sus == 2 then
        SkillCrt(role)
    end
end

function Skill_Melee_Begin(Player, SkillLevel)
end

function Skill_Melee_End(ATKER, DEFER, SkillLevel)
    if ValidCha(ATKER) == 0 or ValidCha(DEFER) == 0 then
        return
    end
    local Damage, Sus, DamageSa = nil, nil, nil
    Damage = Atk_Dmg(ATKER, DEFER)
    Sus, DamageSa = Check_MisorCrt(ATKER, DEFER)
    SetSus(DEFER, Sus)
    if DamageSa == 1 then
        local Fairy = CheckHaveElf(ATKER)
        if Fairy ~= 0 then
            local Skills = GetItemForgeParam(Fairy, 1)
            if CheckElfHaveSkill(Skills, 0, 2) == 2 and ElfSKill_ElfCrt(ATKER, Fairy, Skills) == 1 then
                SystemNotice(ATKER, "Fairy's berserk skill activated, attack bonus granted!")
                SystemNotice(DEFER, "Opponent fairy's berserk skill activated and was granted an attack bonus.")
                DamageSa = 2
                SetSus(DEFER, Sus)
            end
        end
    end
    Damage = math.floor(Damage * DamageSa)
    Hp_Endure_Dmg(DEFER, Damage)

    Take_Atk_ItemURE(ATKER)
    Take_Def_ItemURE(DEFER)
    if Server.EqSet["Kylin"](ATKER) == 1 then
        local Random = math.random(1, 100)
        local Chance = 10
        local Seconds = 1
        if Random <= Chance then
            AddState(ATKER, DEFER, STATE_XY, 1, Seconds)
            SystemNotice(ATKER, "Received blessing from the Goddess, target has been knoced out for " .. Seconds .. " second(s).")
        end
    end
    Check_Ys_Rem(ATKER, DEFER)
end

function Skill_Staff_End(ATKER, DEFER, SkillLevel)
    if ValidCha(ATKER) == 0 or ValidCha(DEFER) == 0 then
        return
    end
    local Damage = 0
    Damage = Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, Damage)

    Take_Atk_ItemURE(ATKER)
    Take_Def_ItemURE(DEFER)
    if Server.EqSet["Kylin"](ATKER) == 1 then
        local Random = math.random(1, 100)
        local Chance = 10
        local Seconds = 1
        if Random <= Chance then
            AddState(ATKER, DEFER, STATE_XY, 1, Seconds)
            SystemNotice(ATKER, "Received blessing from the Goddess, target has been knoced out for " .. Seconds .. " second(s).")
        end
    end
    Check_Ys_Rem(ATKER, DEFER)
end

function Skill_Range_Begin(Player, SkillLevel)
end

function Skill_Range_End(ATKER, DEFER, SkillLevel)
    if ValidCha(ATKER) == 0 or ValidCha(DEFER) == 0 then
        return
    end
    local Damage, Sus, DamageSa = nil, nil, nil
    Damage = Atk_Dmg(ATKER, DEFER)
    Sus, DamageSa = Check_MisorCrt(ATKER, DEFER)
    SetSus(DEFER, Sus)
    if DamageSa == 1 then
        local Fairy = CheckHaveElf(ATKER)
        if Fairy ~= 0 then
            local Skills = GetItemForgeParam(Fairy, 1)
            if CheckElfHaveSkill(Skills, 0, 2) == 2 and ElfSKill_ElfCrt(ATKER, Fairy, Skills) == 1 then
                SystemNotice(ATKER, "Fairy's berserk skill activated, attack bonus granted!")
                SystemNotice(DEFER, "Opponent fairy's berserk skill activated and was granted an attack bonus.")
                DamageSa = 2
                SetSus(DEFER, Sus)
            end
        end
    end
    Damage = math.floor(Damage * DamageSa)
    Hp_Endure_Dmg(DEFER, Damage)

    Take_Atk_ItemURE(ATKER)
    Take_Def_ItemURE(DEFER)
    if Server.EqSet["Kylin"](ATKER) == 1 then
        local Random = math.random(1, 100)
        local Chance = 10
        local Seconds = 1
        if Random <= Chance then
            AddState(ATKER, DEFER, STATE_XY, 1, Seconds)
            SystemNotice(ATKER, "Received blessing from the Goddess, target has been knoced out for " .. Seconds .. " second(s).")
        end
    end
    Check_Ys_Rem(ATKER, DEFER)
end

function Mis_or_Crt(a, b)
    local m = Percentage_Random(a)

    local n = Percentage_Random(b)

    local rom, dmgsa = 1, 1
    if m == 1 then
        rom = 0
        dmgsa = 0
    elseif n == 1 then
        rom = 2
        dmgsa = 2
    end

    return rom, dmgsa
end

function Phy_Dmg(Attack, Defense, PhysicalDef)
    local Damage = 1
    if PhysicalDef < 70 then
        Damage = math.min(0.85, (PhysicalDef / 100))
    else
        Damage = math.min(0.85, (PhysicalDef / 150))
    end
    Damage = 1 - Damage
    Damage = Attack * Damage
    Damage = math.floor(Damage - Defense)
    return Damage
end

function Phy_Dmg_A(a, b, atk, def, resist)
    local phy_atk = atk
    local phy_def = def
    local phy_resist = resist
    local map_name_ATKER = GetChaMapName(a)
    local map_name_DEFER = GetChaMapName(b)
    local Can_Pk_Garner2 = Is_NormalMonster(b)

    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            dmg = math.floor(phy_atk - phy_def) * (1 - math.min(0.85, phy_resist / 100))
            return dmg
        end
    end

    dmg = math.floor(phy_atk * (1 - math.min(0.85, phy_resist / 100)) - phy_def)

    return dmg
end

function Pow_Dmg(atk, def, resist)
    local pow_atk = atk
    local pow_def = def
    local pow_resist = resist

    dmg = math.floor(pow_atk * (1 - math.min(0.85, pow_resist / 100)) - pow_def)
    return dmg
end

function Atk_Dmg(ATKER, DEFER)
    local MNATK = Mnatk(ATKER)
    local MXATK = Mxatk(ATKER)
	if MNATK >= MXATK then
		MNATK = MXATK - 100
	end
    local ATTACK = math.random(MNATK, MXATK)
    local DEF_DEFER = Def(DEFER)
    local PR_DEFER = Resist(DEFER)
    local DAMAGE = Phy_Dmg(ATTACK, DEF_DEFER, PR_DEFER)
    local LV_EFF, LV_DIF = 1, (Lv(TurnToCha(ATKER)) - Lv(TurnToCha(DEFER)))
    if math.abs(LV_DIF) >= 1 then
        LV_EFF = math.min(math.max(0.8, 1 + 0.025 * LV_DIF), 1.2)
    end
    if GetChaMapName(ATKER) == "garner2" or GetChaMapName(DEFER) == "garner2" then
        if Is_NormalMonster(DEFER) == 0 then
            DAMAGE = Phy_Dmg_A(ATKER, DEFER, ATTACK, Def(DEFER), Resist(DEFER))
        end
    end
    DAMAGE = math.max((LV_EFF * DAMAGE), (math.floor(Lv(TurnToCha(ATKER)) * 0.25)))
    if GetChaAttr(ATKER, ATTR_CSAILEXP) > 0 and GetChaAttr(ATKER, ATTR_CSAILEXP) < 100 then
        DAMAGE = DAMAGE * 1.055
    end
    if GetChaAttr(ATKER, ATTR_CSAILEXP) >= 100 and GetChaAttr(ATKER, ATTR_CSAILEXP) < 12100 then
        DAMAGE = DAMAGE * (1.05 + math.floor(math.pow((GetChaAttr(ATKER, ATTR_CSAILEXP) / 100), 0.5)) * 0.005)
    end
    return DAMAGE
end

function Fire_Dmg(a, b)
    local defer_def = Def(b)
    local defer_resist = Resist(b)
    local atker_lv = Lv(TurnToCha(a))
    local defer_lv = Lv(TurnToCha(b))
    local lv_dis = atker_lv - defer_lv
    local lv_eff = 1
    if math.abs(lv_dis) >= 5 then
        lv_eff = math.min(math.max(0.8, 1 + 0.025 * lv_dis), 1.2)
    end

    local atk = math.random(Mnatk(a), Mxatk(a))
    local dmg = Pow_Dmg(atk, defer_def, defer_resist)
    local mndmg = math.floor(Lv(TurnToCha(a)) * 0.25 + Mnatk(a) * 0) + 1
    dmg = math.max(dmg, mndmg)
	
    return dmg
end

function Check_MisorCrt(ATKER, DEFER)
    local DODGE_DEFER = Flee(DEFER)
    local HR_ATKER = Hit(ATKER)
    local LevelDif = Lv(TurnToCha(ATKER)) - Lv(TurnToCha(DEFER))
    local LevelEffect = 0
    if math.abs(LevelDif) >= 1 then
        LevelEffect = math.min(math.max(0, 0.03 * LevelDif), 0.15)
    end
    local bsmiss = math.max(((DODGE_DEFER - HR_ATKER) + 10) / 100, 0)
    local miss = math.min(0.9, bsmiss)
    local crt = math.min(LevelEffect + Crt(ATKER), 1)
    local sus, dmgsa = Mis_or_Crt(miss, crt)
	
    return sus, dmgsa
end

function Magic_Dmg(atk, def, resist)
    local magic_atk = atk
    local magic_def = def
    local magic_resist = resist
    dmg = math.floor((magic_atk - magic_def) * (1 - math.min(0.85, magic_resist / 100)))
	
    return dmg
end

function MAGIC_Atk_Dmg(a, b)
    local job = GetChaAttr(a, ATTR_JOB)
    local sta_atker = Sta(a)
    local sta_defer = Sta(b)
    local atk_mnatk = math.floor(MnatkIb(a) + sta_atker * Magic_rate1[job] + Magic_rate2[job] * math.pow(math.floor(sta_atker * 4 / 20), 2))
    local atk_mxatk = math.floor(MxatkIb(a) + sta_atker * Magic_rate1[job] + Magic_rate2[job] * math.pow(math.floor(sta_atker * 4 / 20), 2))
    local defer_mgic_def = sta_defer * 2
    local defer_resist = Resist(b)
    local atker_lv = Lv(a)
    local defer_lv = Lv(b)
    local lv_dis = atker_lv - defer_lv
    local lv_eff = 1
    if math.abs(lv_dis) >= 1 then
        lv_eff = math.min(math.max(0.5, 1 + 0.025 * lv_dis), 1.5)
    end

    local atk = math.random(atk_mnatk, atk_mxatk)
    local dmg = Magic_Dmg(atk, defer_mgic_def, defer_resist)
    local mndmg = math.floor(Lv(a) * 0.25 + Mnatk(a) * 0) + 1
    dmg = math.max(lv_eff * dmg, mndmg)
	
    return dmg
end

function Skill_Jdzz_Use(role, sklv)
    local statelv = sklv
    local hitsb_dif = 1 * statelv
    local hitsb = HitSb(role) + hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function Skill_Jdzz_Unuse(role, sklv)
    local statelv = sklv
    local hitsb_dif = 1 * statelv
    local hitsb = HitSb(role) - hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function Skill_Jssl_Use(role, sklv)
    local statelv = sklv
    local atksb_dif = 4 * statelv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function Skill_Jssl_Unuse(role, sklv)
    local statelv = sklv
    local atksb_dif = 4 * statelv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillSp_Gtyz(sklv)
	local sp_reduce = 15
    return sp_reduce
end

function SkillCooldown_Gtyz(sklv)
	local cooldown = 15000
    return cooldown
end

function Skill_Gtyz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Gtyz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Gtyz_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 15
    AddState(atker, defer, STATE_GTYZ, statelv, statetime)

    Check_Ys_Rem(atker, defer)
end

function State_Gtyz_Add(role, statelv)
    local def_dif = statelv * 3
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Gtyz_Rem(role, statelv)
    local def_dif = statelv * 3
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Hyz(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Hyz(sklv)
	local cooldown = 5000
    return cooldown
end

function Skill_Hyz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hyz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hyz_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = 0
    Damage = (1.5 + 0.1 * sklv) * (math.min(3, (math.max(1, math.floor(Aspd(atker) / 70))))) * Atk_Dmg(atker, defer)
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        Damage = (1 + 0.1 * sklv) * (math.min(3, (math.max(1, math.floor(Aspd(atker) / 70))))) * Atk_Dmg(atker, defer)
    end
    if IsPlayer(atker) == 1 and IsPlayer(defer) == 1 then
    end
    HP_Red_Skill(atker, defer, Damage, false)
    Check_Ys_Rem(atker, defer)
end

function SkillSp_Kb(sklv)
	local sp_reduce = 15
    return sp_reduce
end

function SkillCooldown_Kb(sklv)
	local cooldown = 35000
    return cooldown
end

function Skill_Kb_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Kb(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Kb_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 20
    AddState(atker, defer, STATE_KB, statelv, statetime)
end

function State_Kb_Add(role, statelv)
    local aspdsa_dif = 0.2 + (statelv * 0.015)
    local aspdsa = (AspdSa(role) + aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_Kb_Rem(role, statelv)
    local aspdsa_dif = 0.2 + (statelv * 0.015)
    local aspdsa = (AspdSa(role) - aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function SkillArea_Circle_Hx(sklv)
    local side = 200
    SetSkillRange(4, side)
end

function SkillSp_Hx(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Hx(sklv)
	local cooldown = 20000
    return cooldown
end

function Skill_Hx_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hx(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hx_End(atker, defer, sklv)
    local hp = Hp(defer)
    local statelv = sklv
    local statetime = 15
    AddState(atker, defer, STATE_HX, statelv, statetime)
end

function State_Hx_Add(role, statelv)
    local mxatksb_dif = 3 * statelv
    local mnatksb_dif = 3 * statelv
    local mspdsa_dif = 0.015 * statelv
    local mxatksb = (MxatkSb(role) - mxatksb_dif)
    local mnatksb = (MnatkSb(role) - mxatksb_dif)
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Hx_Rem(role, statelv)
    local mxatksb_dif = 3 * statelv
    local mnatksb_dif = 3 * statelv
    local mspdsa_dif = 0.015 * statelv
    local mxatksb = (MxatkSb(role) + mxatksb_dif)
    local mnatksb = (MnatkSb(role) + mxatksb_dif)
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillSp_Pj(sklv)
	local sp_reduce = 25
    return sp_reduce
end

function SkillCooldown_Pj(sklv)
	local cooldown = 25000
    return cooldown
end

function Skill_Pj_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Pj(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Pj_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 15
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local agi_atker = Agi(atker)
    local Can_Pk_Garner2 = Is_NormalMonster(defer)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            statetime = agi_atker / 10
            if statetime < 1 then
                statetime = 1
            end
        end
    end
    AddState(atker, defer, STATE_PJ, statelv, statetime)
    Check_Ys_Rem(atker, defer)
end

function State_Pj_Add(role, statelv)
    local def_dif = statelv * 4
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = statelv * 4
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_JLPj_Add(role, statelv)
    local def_dif = statelv * 60
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_JLPj_Rem(role, statelv)
    local def_dif = statelv * 60
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_CHF(sklv)
	local sp_reduce = 10
    return 10
end

function SkillCooldown_CHF(sklv)
	local cooldown = 5000
    return cooldown
end

function Skill_CHF_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_CHF(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_CHF_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 10 + sklv
    local mxhp = Mxhp(defer)
    local hate = mxhp

    if ValidCha(atker) == 0 then
        LG("Skill_CHF", "Attacker as null")
        SkillUnable(atker)
        return
    end

    if ValidCha(defer) == 0 then
        LG("Skill_CHF", "Attacked target as nil")
        SkillUnable(atker)
        return
    end

    if IsPlayer(defer) == 1 then
        LG("Skill_CHF", "Victim as")
        SkillUnable(atker)
        return
    end
    AddState(atker, defer, STATE_CHF, statelv, statetime)
    AddHate(defer, atker, hate)

    Check_Ys_Rem(atker, defer)
end

function State_Chf_Add(role, statelv)
    local map_name_DEFER = GetChaMapName(role)
    local Can_Pk_Garner2 = Is_NormalMonster(role)

    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local sklv = statelv - 1
        local CfLv = (statelv - sklv) * 10

        if CfLv ~= 0 then
            local defsa_dif = 0.02 * CfLv
            local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
            SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
            ALLExAttrSet(role)
        end
    end
end

function State_Chf_Rem(role, statelv)
    local HateList = {}
    local Hate = {}
    local i = 0
    local HateMax = 0
    local atker = role
    local mxhp = Mxhp(role)
    local hate = mxhp * -1
    local map_name_DEFER = GetChaMapName(role)
    local Can_Pk_Garner2 = Is_NormalMonster(role)

    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local sklv = statelv - 1
        local CfLv = (statelv - sklv) * 10
        Notice("CfLv =" .. CfLv)

        if CfLv ~= 0 then
            local defsa_dif = 0.02 * CfLv
            local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
            SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
            ALLExAttrSet(role)
        end
    end
    for i = 1, 5, 1 do
        HateList[i], Hate[i] = GetChaHateByNo(role, i - 1)
    end
    for i = 1, 5, 1 do
        if Hate[i] > HateMax then
            HateMax = Hate[i]
        end
    end
    for i = 1, 5, 1 do
        if Hate[i] == HateMax then
            atker = HateList[i]
        end
    end

    if ValidCha(role) == 0 then
        LG("Skill_CHF", "Target as null")
        SkillUnable(role)
        return
    end

    if ValidCha(atker) == 0 then
        LG("Skill_CHF", "Target vengeance list as null")

        return
    end

    AddHate(role, atker, hate)
end

function Skill_Gjsl_Use(role, sklv)
    local statelv = sklv
    local mnatksb_dif = 2 * statelv
    local mxatksb_dif = 2 * statelv
    local mnatksb = MnatkSb(role) + mnatksb_dif
    local mxatksb = MxatkSb(role) + mxatksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function Skill_Gjsl_Unuse(role, sklv)
    local statelv = sklv
    local mnatksb_dif = 2 * statelv
    local mxatksb_dif = 2 * statelv
    local mnatksb = MnatkSb(role) - mnatksb_dif
    local mxatksb = MxatkSb(role) - mxatksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function Skill_Jfb_Use(role, sklv)
    local statelv = sklv
    local mspdsa_dif = statelv * 0.02
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function Skill_Jfb_Unuse(role, sklv)
    local statelv = sklv
    local mspdsa_dif = statelv * 0.02
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillSp_Fnq(sklv)
	local sp_reduce = 35
    return sp_reduce
end

function SkillCooldown_Fnq(sklv)
    local cooldown = 25000
    return cooldown
end

function Skill_Fnq_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Fnq(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Fnq_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 20

    if GetChaTypeID(atker) == 983 then
        statetime = 120
        statelv = 10
    end

    AddState(atker, defer, STATE_FNQ, statelv, statetime)
end

function State_Fnq_Add(role, statelv)
    local aspd_dif = 10 + (statelv * 1)
    local aspdsb = (AspdSb(role) + aspd_dif)
    SetCharaAttr(aspdsb, role, ATTR_STATEV_ASPD)
    ALLExAttrSet(role)
end

function State_Fnq_Rem(role, statelv)
    local aspd_dif = 10 + (statelv * 1)
    local aspdsb = (AspdSb(role) - aspd_dif)
    SetCharaAttr(aspdsb, role, ATTR_STATEV_ASPD)
    ALLExAttrSet(role)
end


function SkillSp_Lzj(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Lzj(sklv)
	local cooldown = 5000
    return cooldown
end

function Skill_Lzj_Begin(role, sklv)
    if Sp(role) - SkillSp_Lzj(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Lzj(sklv))
end

function Skill_Lzj_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = (1.5 + sklv * 0.15) * Atk_Dmg(atker, defer)
    HP_Red_Skill(atker, defer, Damage, false)
    Check_Ys_Rem(atker, defer)
end

function SkillSp_Bdj(sklv)
	local sp_reduce = 15
    return 15
end

function SkillCooldown_Bdj(sklv)
	local cooldown = 15000
    return cooldown
end

function Skill_Bdj_Begin(role, sklv)
    if Sp(role) - SkillSp_Bdj(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Bdj(sklv))
end

function Skill_Bdj_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = 1.2 * Atk_Dmg(atker, defer)
    HP_Red_SkillS(atker, defer, Damage, false)
    if GetChaTypeID(atker) == 983 then
        sklv = 10
    end
    AddState(atker, defer, STATE_BDJ, sklv, 5)
end

function State_Bdj_Add(role, statelv)
    local MSPD_DIF = 0.2 + statelv * 0.03
    local MSPD = (MspdSa(role) - MSPD_DIF) * ATTR_RADIX
    SetCharaAttr(MSPD, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Bdj_Rem(role, statelv)
    local MSPD_DIF = 0.2 + statelv * 0.03
    local MSPD = (MspdSa(role) + MSPD_DIF) * ATTR_RADIX
    SetCharaAttr(MSPD, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillArea_Square_Lxjy(sklv)
    local Area = 180 + (22 * sklv)
    SetSkillRange(4, Area)
end

function SkillPre_Lxjy(sklv)
end

function SkillCooldown_Lxjy(sklv)
	local cooldown = 25000
    return cooldown
end

function SkillSp_Lxjy(sklv)
	local sp_reduce = 46
    return sp_reduce
end

function Skill_Lxjy_Begin(role, sklv)
    if Sp(role) - SkillSp_Lxjy(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Lxjy(sklv))
end

function Skill_Lxjy_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = (0.5 + sklv * 0.1) * Atk_Dmg(atker, defer)
    HP_Red_SkillS(atker, defer, Damage, false)
end

function SkillSp_Dj(sklv)
	local sp_reduce = 15
    return sp_reduce
end

function SkillCooldown_Dj(sklv)
	local cooldown = 25000
    return 25000
end

function Skill_Dj_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Dj(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Dj_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 20
    AddState(atker, defer, STATE_DJ, statelv, statetime)

    Check_Ys_Rem(atker, defer)
end

function State_Dj_Add(role, statelv)
    local hpdmg = 10 + (statelv * 2)
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_Dj_Rem(role, statelv)
end

function SkillSp_Xzy(sklv)
    local sp_reduce = 27 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Xzy(sklv)
    local cooldown = 7000 - (sklv * 300)
    return cooldown
end

function Skill_Xzy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xzy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xzy_End(atker, defer, sklv)
    local sta = Sta(atker)
    local exp = Exp(atker)
    local hpdmg = (-1) * math.floor(50 + 50 * sklv + sta * 7)
    local mxhp_def = Mxhp(defer)
    local hp_def = Hp(defer)
    local hp_recover = math.max(math.min((mxhp_def - hp_def), (-1) * hpdmg), 0)
    local exp_add = hp_recover / 8
    local Lv = Lv(atker)
    if Lv >= 80 then
        exp_add = exp_add / 50
    end
    local rolemode_defer = IsPlayer(defer)
    local ChaList = {}
    local i = 0
    local Hate = 0
    local role = atker
    local HateNum = 3
    local HateAddNum = 0
    if rolemode_defer == 1 and defer ~= atker then
        exp = exp + exp_add
        SetCharaAttr(exp, atker, ATTR_CEXP)
    end
    Hp_Endure_Dmg(defer, hpdmg)
    ChaList[1], ChaList[2], ChaList[3], ChaList[4], ChaList[5], ChaList[6], ChaList[7], ChaList[8], ChaList[9], ChaList[10], ChaList[11], ChaList[12] = GetChaSetByRange(defer, 0, 0, 800, 0)
    Hate = math.floor(hpdmg / -2)
    for i = 1, 12, 1 do
        if ChaList[i] ~= nil then
            role = GetChaTarget(ChaList[i])
            if role == defer then
                if HateAddNum < HateNum then
                    AddHate(ChaList[i], atker, Hate)
                    HateAddNum = HateAddNum + 1
                end
            end
        end
    end
end

function Skill_Jsjc_Use(role, sklv)
    local statelv = sklv
    local mxspsb_dif = statelv * 40
    local mxspsb = math.floor(MxspSb(role) + mxspsb_dif)
    SetCharaAttr(mxspsb, role, ATTR_STATEV_MXSP)
    ALLExAttrSet(role)
end

function Skill_Jsjc_Unuse(role, sklv)
    local statelv = sklv
    local mxspsb_dif = statelv * 40
    local mxspsb = math.floor(MxspSb(role) - mxspsb_dif)
    SetCharaAttr(mxspsb, role, ATTR_STATEV_MXSP)
    ALLExAttrSet(role)
end

function SkillSp_Xlcz(sklv)
	local sp_reduce = 32 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Xlcz(sklv)
	local cooldown = 6000 - (sklv * 300)
    return cooldown
end

function Skill_Xlcz_Begin(role, sklv)
    if Sp(role) - SkillSp_Xlcz(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Xlcz(sklv))
end

function Skill_Xlcz_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local LV_DIF = math.max((-1) * 10, math.min(10, Lv(TurnToCha(atker)) - Lv(TurnToCha(defer))))
    local Damage = math.floor((10 + Sta(atker) * 2) * (1 + sklv * 0.25) * (1 + LV_DIF * 0.025)) * 1.1
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        Damage = math.floor((MAGIC_Atk_Dmg(atker, defer)) * (1 + sklv * 0.1))
    end
    Damage = Cuihua_Mofa(Damage, GetChaStateLv(atker, STATE_MLCH))
    Damage = Damage + ElfSkill_MagicAtk(Damage, atker)
    if GetChaAttr(atker, ATTR_CSAILEXP) > 0 and GetChaAttr(atker, ATTR_CSAILEXP) < 100 then
        Damage = Damage * 1.055
    end
    if GetChaAttr(atker, ATTR_CSAILEXP) >= 100 and GetChaAttr(atker, ATTR_CSAILEXP) < 12100 then
        Damage = Damage * (1.05 + math.floor(math.pow((GetChaAttr(atker, ATTR_CSAILEXP) / 100), 0.5)) * 0.005)
    end
    HP_Red_Skill(atker, defer, Damage, false)
    Check_Ys_Rem(atker, defer)
end

function SkillSp_Xlzh(sklv)
    local sp_reduce = 44 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_Xlzh(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Xlzh_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xlzh(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xlzh_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local statelv = sklv
    local statetime = 180 + (sklv * 18)
    AddState(atker, defer, STATE_XLZH, statelv, statetime)
end

function State_Xlzh_Add(role, statelv)
    local mnatksa_dif = 0.1 + 0.01 * statelv
    local mxatksa_dif = 0.1 + 0.01 * statelv
    local mnatksa = math.floor((MnatkSa(role) + mnatksa_dif) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) + mxatksa_dif) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_Xlzh_Rem(role, statelv)
    local mnatksa_dif = 0.1 + 0.01 * statelv
    local mxatksa_dif = 0.1 + 0.01 * statelv
    local mnatksa = math.floor((MnatkSa(role) - mnatksa_dif) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) - mxatksa_dif) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function SkillSp_Fzlz(sklv)
    local sp_reduce = 44 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_Fzlz(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Fzlz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Fzlz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Fzlz_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
	
    local statelv = sklv
    local statetime = 180 + (sklv * 18)
    if GetChaTypeID(atker) == 984 then
        statetime = 360
        statelv = 10
    end
    AddState(atker, defer, STATE_FZLZ, statelv, statetime)
end

function State_Fzlz_Add(role, statelv)
    local mspdsa_dif = 0.05 + (statelv * 0.01)
    local aspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    local aspdsa = math.floor((AspdSa(role) + aspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Fzlz_Rem(role, statelv)
    local mspdsa_dif = 0.05 + (statelv * 0.01)
    local aspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    local aspdsa = math.floor((AspdSa(role) - aspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillSp_Shpf(sklv)
    local sp_reduce = 44 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_Shpf(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Shpf_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Shpf(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Shpf_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local statelv = sklv
    local statetime = 180
    AddState(atker, defer, STATE_SHPF, statelv, statetime)
end

function State_Shpf_Add(role, statelv)
    local def_dif = 14 + (statelv * 4)
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Shpf_Rem(role, statelv)
    local def_dif = 14 + (statelv * 4)
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Hfs(sklv)
	local sp_reduce = 25
    return sp_reduce
end

function SkillCooldown_Hfs(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Hfs_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hfs(sklv)
    local hp_recover = 50 + (sklv * 100)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    local hp = Hp(role) + hp_recover
    SetCharaAttr(hp, role, ATTR_HP)
    Sp_Red(role, sp_reduce)
end

function Skill_Hfs_End(atker, defer, sklv)
    Rem_State_Unnormal(defer)
end

function SkillSp_Fh(sklv)
	local sp_reduce = 50
    return sp_reduce
end

function SkillCooldown_Fh(sklv)
    local cooldown = 60000 - (sklv * 1500)
    return cooldown
end

function Skill_Fh_Begin(role, sklv)
    local map_name_ATKER = GetChaMapName(role)
    if map_name_ATKER == "garner2" then
        SystemNotice(role, "Unable to use Revival skills here.")
        SkillUnable(role)
    end

    local item_count = CheckBagItem(role, ITEM_RELIFE)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	
	local a = DelBagItem(role, ITEM_RELIFE, 1)
end

function Skill_Fh_End(atker, defer, sklv)
    local ChaName = GetChaDefaultName(atker)
    SetRelive(atker, defer, sklv, "role "..ChaName.."\n\n wish to revive you. Accept?")
end

function Skill_Jr_Use(role, sklv)
    local statelv = sklv
    local srecsb_dif = 1 + statelv * 1
    local srecsb = SrecSb(role) + srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    ALLExAttrSet(role)
end

function Skill_Jr_Unuse(role, sklv)
    local statelv = sklv
    local srecsb_dif = 1 + statelv * 1
    local srecsb = SrecSb(role) - srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    ALLExAttrSet(role)
end

function Skill_Sl_Use(role, sklv)
    local statelv = sklv
    local ship_mspdsa_dif = 0.05 + (statelv * 0.01)
    local ship_mspdsa = (Ship_MspdSa(role) + ship_mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(ship_mspdsa, role, ATTR_BOAT_SKILLC_MSPD)
    ALLExAttrSet(role)
end

function Skill_Sl_Unuse(role, sklv)
    local statelv = sklv
    local ship_mspdsa_dif = 0.05 + (statelv * 0.01)
    local ship_mspdsa = (Ship_MspdSa(role) - ship_mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(ship_mspdsa, role, ATTR_BOAT_SKILLC_MSPD)
    ALLExAttrSet(role)
end

function Skill_Bkzj_Use(role, sklv)
    local statelv = sklv
    local ship_defsb_dif = statelv * 8
    local ship_defsb = (Ship_DefSb(role) + ship_defsb_dif)
    SetCharaAttr(ship_defsb, role, ATTR_BOAT_SKILLV_DEF)
    ALLExAttrSet(role)
end

function Skill_Bkzj_Unuse(role, sklv)
    local statelv = sklv
    local ship_defsb_dif = statelv * 8
    local ship_defsb = (Ship_DefSb(role) - ship_defsb_dif)
    SetCharaAttr(ship_defsb, role, ATTR_BOAT_SKILLV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Lj(sklv)
	local sp_reduce = 25 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Lj(sklv)
	local cooldown = 8400 - (sklv * 400)
    return cooldown
end

function SkillEnergy_Lj(sklv)
    local energy_reduce = 3 + math.floor(sklv * 0.5)
    return energy_reduce
end

function Skill_Lj_Begin(role, sklv)
    if Sp(role) < SkillSp_Lj(sklv) then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Lj(sklv))
end

function Skill_Lj_End(atker, defer, sklv)
    local Damage = math.floor((80 + sklv * 10 + (Sta(atker)) * 6) + 3 * (Lv(atker)))
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        Damage = math.floor(MAGIC_Atk_Dmg(atker, defer) * math.pow(sklv, 1 / 2))
    end
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 80 then
        Damage = Damage * 1.5
        SystemNotice(atker, "Damage was increased due to the power of the Black Dragon equipment.")
    end
    Damage = Cuihua_Mofa(Damage, (GetChaStateLv(atker, STATE_MLCH)))
    Damage = Damage + ElfSkill_MagicAtk(Damage, atker)
    if GetChaAttr(atker, ATTR_CSAILEXP) > 0 and GetChaAttr(atker, ATTR_CSAILEXP) < 100 then
        Damage = Damage * 1.055
    end
    if GetChaAttr(atker, ATTR_CSAILEXP) >= 100 and GetChaAttr(atker, ATTR_CSAILEXP) < 12100 then
        Damage = Damage * (1.05 + math.floor(math.pow((GetChaAttr(atker, ATTR_CSAILEXP) / 100), 0.5)) * 0.005)
    end
    AddState(atker, defer, STATE_MB, 2, 3)
    HP_Red_SkillS(atker, defer, Damage, false)
	LookEnergy(atker)
end

function SkillSp_Jf(sklv)
    local sp_reduce = 27 + sklv * 2
    return sp_reduce
end

function SkillCooldown_Jf(sklv)
	local cooldown = 12000
    return cooldown
end

function SkillEnergy_Jf(sklv)
    local energy_reduce = math.floor(1 + sklv * 0.25)
    return energy_reduce
end

function Skill_Jf_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Jf(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Jf_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = math.floor(3 + sklv * 0.5)
    local a = 1
    local hp_defer = Hp(defer)
    local MxHp_defer = Mxhp(defer)

    if MxHp_defer >= 100000 then
        SetSus(defer, 0)
        return
    end

    if hp_defer >= 50000 then
        a = Percentage_Random(0.2)
        statetime = math.floor(statetime / 2) + 1
    end
    if a == 1 then
        if GetChaTypeID(atker) == 986 then
            statetime = 12
            statelv = 10
        end

        AddState(atker, defer, STATE_JF, statelv, statetime)
    else
        SetSus(defer, 0)
    end
	LookEnergy(atker)
end

function State_Jf_Add(role, statelv)
end

function State_jf_Rem(role, statelv)
end

function SkillSp_Hzcr(sklv)
    local sp_reduce = 20 + (sklv * 1)
    return sp_reduce
end

function SkillCooldown_Hzcr(sklv)
	local cooldown = 12000
    return cooldown
end

function Skill_Hzcr_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hzcr(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hzcr_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 6 + (sklv * 0.5)
    local role1 = TurnToCha(atker)
    local Check_Heilong = CheckItem_Heilong(role1)
    if Check_Heilong == 1 then
        local Percentage = Percentage_Random(0.8)
        if Percentage == 1 then
            statetime = statetime * 2
            SystemNotice(atker, "Obtain power from Black Dragon set. Duration of skill extended")
        end
    end
    local hp_defer = Mxhp(defer)
    if hp_defer >= 1000000 then
        local a = Percentage_Random(0.5)
        if a == 1 then
            statetime = 6 + (sklv * 0.5)
        else
            SetSus(defer, 0)
            SystemNotice(atker, "Alga Entanglement failed")
            return
        end
    end
    AddState(atker, defer, STATE_HZCR, statelv, statetime)
end

function State_Hzcr_Add(role, statelv)
    local dmg = 10 + statelv * 2
    Endure_Dmg(role, dmg)
end

function State_Hzcr_Rem(role, statelv)
end

function Skill_Jjsl_Use(role, sklv)
    local statelv = sklv
    local atksb_dif = 7 * statelv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    local map_name_ATKER = GetChaMapName(role)

    local JianLv = GetSkillLv(role, 62)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local hitsa_dif = (1) * (0.02 * JianLv)
        local hitsa = math.floor((HitSa(role) + hitsa_dif) * ATTR_RADIX)
        SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    end
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function Skill_Jjsl_Unuse(role, sklv)
    local statelv = sklv
    local atksb_dif = 7 * statelv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    local map_name_ATKER = GetChaMapName(role)

    local JianLv = GetSkillLv(role, 62)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local hitsa_dif = (1) * (0.02 * JianLv)
        local hitsa = math.floor((HitSa(role) - hitsa_dif) * ATTR_RADIX)
        SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    end
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillArea_Circle_Pax(sklv)
    local side = 200 + (sklv * 20)
    SetSkillRange(4, side)
end

function SkillCooldown_Pax(sklv)
    local cooldown = 4000 - (sklv * 200)
    return cooldown
end

function SkillPre_Pax(sklv)
end

function SkillSp_Pax(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function Skill_Pax_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Pax(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Pax_End(atker, defer, sklv)
    local HateList = {}
    local Hate = {}
    local i = 0
    local HateMax = 0
    local Hate_dif = 0
    local Hate_fin = 0
    local statelv = sklv
    local statetime = 3
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local Can_Pk_Garner2 = Is_NormalMonster(defer)
    local CfLv = GetSkillLv(atker, 242)
    local PxLv = GetSkillLv(atker, 243)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        statelv = sklv + CfLv / 10
    end
    if ValidCha(atker) == 0 then
        LG("Skill_PAX", "Attacker as null")
        SkillUnable(atker)
        return
    end

    if ValidCha(defer) == 0 then
        LG("Skill_PAX", "Attacked target as nil")
        SkillUnable(atker)
        return
    end

    if IsPlayer(defer) == 0 then
        for i = 1, 5, 1 do
            HateList[i], Hate[i] = GetChaHateByNo(defer, i - 1)
        end
        for i = 1, 5, 1 do
            if Hate[i] > HateMax then
                HateMax = Hate[i]
            end
        end
        for i = 1, 5, 1 do
            if HateList[i] == atker then
                Hate_dif = Hate[i]
            end
        end

        local mxhp = Mxhp(defer)
        local hate = mxhp

        AddState(atker, defer, STATE_CHF, statelv, statetime)
        AddHate(defer, atker, hate)
        Check_Ys_Rem(atker, defer)
    end
end

function Skill_Qhtz_Use(role, sklv)
    local statelv = sklv
    local mxhpsb_dif = 20 * statelv + Con(role) * 3
    local mxhpsb = MxhpSb(role) + mxhpsb_dif
    local map_name_ATKER = GetChaMapName(role)

    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local GTYZ_Lv = GetSkillLv(role, 63)
        local resistsb_dif = 1 * GTYZ_Lv
        local resistsb = ResistSb(role) + resistsb_dif
        SetCharaAttr(resistsb, role, ATTR_STATEV_PDEF)
    end

    SetCharaAttr(mxhpsb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function Skill_Qhtz_Unuse(role, sklv)
    local statelv = sklv
    local mxhpsb_dif = 20 * statelv + Con(role) * 3
    local mxhpsb = MxhpSb(role) - mxhpsb_dif
    local map_name_ATKER = GetChaMapName(role)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local GTYZ_Lv = GetSkillLv(role, 63)
        local resistsb_dif = 1 * GTYZ_Lv
        local resistsb = ResistSb(role) - resistsb_dif
        SetCharaAttr(resistsb, role, ATTR_STATEV_PDEF)
    end
    SetCharaAttr(mxhpsb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function Skill_Mnrx_Use(role, sklv)
    local statelv = sklv
    local mxhpsa_dif = 0.1 + 0.02 * statelv
    local defsa_dif = 0.1 + 0.02 * statelv
    local mxhpsa = math.floor((MxhpSa(role) + mxhpsa_dif) * ATTR_RADIX)
    local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
    local map_name_ATKER = GetChaMapName(role)

    local GangTieLv = GetSkillLv(role, 63)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local hrecsb_dif = 10 * GangTieLv
        local hrecsb = math.floor((HrecSb(role) + hrecsb_dif))
        SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)
    end
	
    SetCharaAttr(mxhpsa, role, ATTR_STATEC_MXHP)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function Skill_Mnrx_Unuse(role, sklv)
    local statelv = sklv
    local mxhpsa_dif = 0.1 + (statelv * 0.02)
    local defsa_dif = 0.1 + (statelv * 0.02)
    local mxhpsa = math.floor((MxhpSa(role) - mxhpsa_dif) * ATTR_RADIX)
    local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
    local map_name_ATKER = GetChaMapName(role)

    local GangTieLv = GetSkillLv(role, 63)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        local hrecsb_dif = 10 * GangTieLv
        local hrecsb = math.floor((HrecSb(role) + hrecsb_dif))
        SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)
    end
	
    SetCharaAttr(mxhpsa, role, ATTR_STATEC_MXHP)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Zj(sklv)
    local sp_reduce = 8 + (sklv * 1)
    return sp_reduce
end

function SkillCooldown_Zj(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Zj_Begin(role, sklv)
    if (Sp(role) - SkillSp_Zj(sklv)) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Zj(sklv))
end

function Skill_Zj_End(atker, defer, sklv)
    local Damage = Atk_Raise((1.2 + sklv * 0.05), atker, defer)
    HP_Red_SkillS(atker, defer, Damage, false)
    Check_Ys_Rem(atker, defer)
end

function SkillArea_Circle_Lh(sklv)
    local Area = 300 + math.floor(sklv * 20)
    SetSkillRange(4, Area)
end

function SkillCooldown_Lh(sklv)
	local cooldown = 5000
    return cooldown
end

function SkillPre_Lh(sklv)
end

function SkillSp_Lh(sklv)
	local sp_reduce = 20 + sklv
    return sp_reduce
end

function Skill_Lh_Begin(role, sklv)
    if Sp(role) - SkillSp_Lh(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Lh(sklv))
end

function Skill_Lh_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = (1 + sklv * 0.05) * Atk_Dmg(atker, defer)
    HP_Red_SkillS(atker, defer, Damage, false)
    Check_Ys_Rem(atker, defer)
end

function SkillSp_Swzq(sklv)
	local sp_reduce = 53 + (sklv * 3)
    return sp_reduce
end

function SkillCooldown_Swzq(sklv)
	local cooldown = 45000
    return cooldown
end

function Skill_Swzq_Begin(role, sklv)
    if (Sp(role) - SkillSp_Swzq(sklv)) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Swzq(sklv))
end

function Skill_Swzq_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Chance = 0
    local Damage = 0
    Damage = (math.floor(Atk_Dmg(atker, defer) * 3.5) + sklv)
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 30 then
        Damage = Damage * 3
        SystemNotice(atker, "Attack was increased due to the power of the Black Dragon equipment!")
    end
    if IsPlayer(defer) == 1 and IsPlayer(atker) == 1 then
        local LEVEL_DIF = Lv(atker) - Lv(defer)
        Chance = math.min(90, math.max(1, (30 + LEVEL_DIF * 2)))
        if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") then
            Chance = Chance * (1 + (GetSkillLv(atker, 67)) * 0.1)
        end
        if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 50 then
            Chance = Chance * 1.5
            SystemNotice(atker, "Hit Rate was increased due to the power of the Black Dragon equipment!")
        end
    end
    HP_Red_SkillS(atker, defer, Damage, false)
    local statetime = 1
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 50 then
        statetime = statetime * 3
        SystemNotice(atker, "Knock out duration was increased due to the power of the Black Dragon equipment!")
    end
    AddState(atker, defer, STATE_XY, sklv, statetime)
end

function State_Xy_Add(role, statelv)
end

function State_Xy_Rem(role, statelv)
end

function SkillSp_ShouWangS(sklv)
	local sp_reduce = 125
    return sp_reduce
end

function SkillCooldown_ShouWangS(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillArea_Circle_ShouWangS(sklv)
    SetSkillRange(4,800)
end

function Skill_ShouWangS_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_ShouWangS(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_ShouWangS_End(atker, defer, sklv)
    local statetime = 15
    local Damage = 2000 + sklv * 500
    Hp_Endure_Dmg(defer, Damage)
    AddState(atker, defer, STATE_DZFS, sklv, statetime)
end

function Skill_Fsz_Use(role, sklv)
    local statelv = sklv
    local lefthand_rad_dif = statelv * 8
    local lefthand_rad = GetChaAttr(role, ATTR_LHAND_ITEMV) + lefthand_rad_dif
    SetCharaAttr(lefthand_rad, role, ATTR_LHAND_ITEMV)
end

function Skill_Fsz_Unuse(role, sklv)
    local statelv = sklv
    local lefthand_rad_dif = statelv * 8
    local lefthand_rad = GetChaAttr(role, ATTR_LHAND_ITEMV) - lefthand_rad_dif
    SetCharaAttr(lefthand_rad, role, ATTR_LHAND_ITEMV)
end

function Skill_Lqhb_Use(role, sklv)
    local statelv = sklv
    local fleesb_dif = 3 * statelv
    local fleesb = FleeSb(role) + fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Lqhb_Unuse(role, sklv)
    local statelv = sklv
    local fleesb_dif = 3 * statelv
    local fleesb = FleeSb(role) - fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Pxkg_Use(role, sklv)
    local statelv = sklv
    local aspdsa_dif = 0.1 + (statelv * 0.01)
    local map_name_ATKER = GetChaMapName(role)

    local str_atker = Str(role)

    local aspdsa = math.floor((AspdSa(role) + aspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function Skill_Pxkg_Unuse(role, sklv)
    local statelv = sklv
    local aspdsa_dif = 0.1 + (statelv * 0.01)
    local aspdsa = math.floor((AspdSa(role) - aspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function SkillSp_Db(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Db(sklv)
	local cooldown = 20000
    return cooldown
end

function Skill_Db_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Db(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Db_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 5 + (sklv * 4)
    AddState(atker, defer, STATE_ZD, statelv, statetime)

    Check_Ys_Rem(atker, defer)
end

function State_Zd_Add(role, statelv)
    local hpdmg = 10 + (statelv * 2)
    local map_name_ATKER = GetChaMapName(role)
    local agi_atker = Agi(role)
    local Can_Pk_Garner2 = Is_NormalMonster(role)
    if map_name_ATKER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            hpdmg = math.max(5, 320)
        end
    end
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_Zd_Rem(role, statelv)
end

function SkillSp_Guz(sklv)
	local sp_reduce = 23 + (sklv * 3)
    return sp_reduce
end

function SkillCooldown_Guz(sklv)
	local cooldown = 30000
    return cooldown
end

function Skill_Guz_Begin(role, sklv)
    if (Sp(role) - (SkillSp_Guz(sklv))) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Guz(sklv))
end

function Skill_Guz_End(atker, defer, sklv)
    if ValidCha(atker) == 0 or ValidCha(defer) == 0 then
        return
    end
    local Damage = (1 + sklv * 0.1) * Atk_Dmg(atker, defer)
    local statetime = 3 + math.floor(sklv * 0.5)
    local Stun = true
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        statetime = statetime * Agi(atker) / 200
    end
    local DIR_DIF = GetObjDire(atker) - GetObjDire(defer)
    if math.abs(DIR_DIF) < 90 or math.abs(DIR_DIF) > 180 then
        statetime = statetime * 2
    end
    if GetChaAttr(defer, ATTR_MXHP) >= 100000 then
        if math.random(1, 100) > 80 then
            Chance = false
        end
        statetime = math.floor(statetime / 2) + 1
    end
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 50 then
        statetime = statetime * 2
        SystemNotice(atker, "Knock out duration was increased due to the power of the Black Dragon equipment.")
    end
    if Stun == true then
        if GetChaTypeID(atker) == 979 then
            statetime = 8
        end
        if GetChaMapName(defer) == "hell" or GetChaMapName(defer) == "hell2" or GetChaMapName(defer) == "hell3" or GetChaMapName(defer) == "hell4" or GetChaMapName(defer) == "hell5" then
            for i = 5, 19, 1 do
                if Abaddon.Boss[i].ID == GetChaTypeID(defer) and GetChaAIType(defer) >= 21 then
                    if Abaddon.Boss[i].Skill[2] == 0 then
                        Notice(GetChaDefaultName(defer)..": Do you really think I can be defeated by the same skill? How foolish warriors!")
                        return
                    else
                        Abaddon.Boss[i].Skill[2] = Abaddon.Boss[i].Skill[2] - 1
                    end
                end
            end
        end
        HP_Red_SkillS(atker, defer, Damage, false)
        AddState(atker, defer, STATE_XY, sklv, statetime)
    else
        SetSus(defer, 0)
    end
    Check_Ys_Rem(atker, defer)
end

function SkillSp_Ys(sklv)
	local sp_reduce = 10
    return sp_reduce
end

function SkillCooldown_Ys(sklv)
	local cooldown = 30000
    return cooldown
end

function Skill_Ys_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Ys(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Ys_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 20 + sklv
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local agi_atker = Agi(atker)
    local Can_Pk_Garner2 = Is_NormalMonster(defer)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            statetime = 20 + agi_atker / 4 + (sklv * 5)
        end
    end
    AddState(atker, defer, STATE_YS, statelv, statetime)
end

function State_Ys_Add(role, statelv)
    local sp = Sp(role)
    local sp_reduce = 10 - math.floor(statelv * 0.5)
    sp = sp - sp_reduce
    if sp <= 0 then
        RemoveState(role, STATE_YS)
        return
    end
    SetCharaAttr(sp, role, ATTR_SP)
end

function State_Ys_Rem(role, statelv)
end

function SkillArea_Circle_WuYin(sklv)
    local Area = 600 + (sklv * 200)
    SetSkillRange(4, Area)
end

function SkillCooldown_WuYin(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillSp_WuYin(sklv)
	local sp_reduce = 100 + (sklv * 25)
    return sp_reduce
end

function Skill_WuYin_Begin(role, sklv)
    if Sp(role) - SkillSp_WuYin(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_WuYin(sklv))
end

function Skill_WuYin_End(atker, defer, sklv)
    local Damage = math.floor(math.random(1500, 2000) + sklv * math.random(400, 600))
    HP_Red_SkillS(atker, defer, Damage, false)
end

function Skill_Hqsl_Use(role, sklv)
    local statelv = sklv
    local mnatksb_dif = 6 * statelv
    local mxatksb_dif = 10 * statelv
    local mnatksb = MnatkSb(role) + mnatksb_dif
    local mxatksb = MxatkSb(role) + mxatksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function Skill_Hqsl_Unuse(role, sklv)
    local statelv = sklv
    local mnatksb_dif = 6 * statelv
    local mxatksb_dif = 10 * statelv
    local mnatksb = MnatkSb(role) - mnatksb_dif
    local mxatksb = MxatkSb(role) - mxatksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillPre_Rsd(sklv)
end

function SkillArea_Square_Rsd(sklv)
    local side = 250
    SetSkillRange(4, side)
end

function SkillCooldown_Rsd(sklv)
	local cooldown = 15000
    return cooldown
end

function SkillArea_State_Rsd(sklv)
    local statetime = 10
    local statelv = sklv
    SetRangeState(STATE_RS, statelv, statetime)
end

function SkillSp_Rsd(sklv)
    local sp_reduce = 17 + (sklv * 2)
    return sp_reduce
end

function Skill_Rsd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Rsd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Rsd_End(atker, defer, sklv)
end

function State_Rs_Add(role, statelv)
    local arealv = GetAreaStateLevel(role, STATE_RS)
    local hp = GetChaAttr(role, ATTR_HP)
    local hpdmg = 10
    if arealv >= 1 then
        hpdmg = 30 + (arealv * 3)
    end
    Hp_Endure_Dmg(role, hpdmg)
end

function State_Rs_Rem(role, statelv)
end

function State_Rs_Tran(statelv)
    return 10
end

function SkillSp_Tj(sklv)
	local sp_reduce = 10 + math.floor(sklv * 0.5)
    return sp_reduce
end

function SkillCooldown_Tj(sklv)
	local cooldown = 8000 - (sklv * 200)
    return cooldown
end

function Skill_Tj_Begin(role, sklv)
    if Sp(role) - SkillSp_Tj(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Tj(sklv))
end

function Skill_Tj_End(atker, defer, sklv)
    local statetime = 5 + (sklv * 0.5)
    if GetChaAttr(defer, ATTR_MXHP) >= 100000 then
        statetime = math.floor(statetime / 3) + 1
    end
    local Damage = 1 + (sklv * 0.05) * Atk_Dmg(atker, defer)
    HP_Red_SkillS(atker, defer, Damage, false)
    if GetChaTypeID(atker) == 980 then
        statetime = 5
    end
    if GetChaMapName(defer) == "hell" or GetChaMapName(defer) == "hell2" or GetChaMapName(defer) == "hell3" or GetChaMapName(defer) == "hell4" or GetChaMapName(defer) == "hell5" then
        for i = 5, 19, 1 do
            if Abaddon.Boss[i].ID == GetChaTypeID(defer) and GetChaAIType(defer) >= 21 then
                if Abaddon.Boss[i].Skill[4] == 0 then
                    Notice(GetChaDefaultName(defer)..": Do you really think I can be defeated by the same skill? How foolish warrios!")
                    return
                else
                    Abaddon.Boss[i].Skill[4] = Abaddon.Boss[i].Skill[4] - 1
                end
            end
        end
    end
    AddState(atker, defer, STATE_TJ, sklv, statetime)
end

function State_Tj_Add(role, statelv)
    local DODGE_DIF = (-1) * 0.2
    local MSPD_DIF = (-1) * 0.5 + (statelv * 0.025)
    local DODGE = (FleeSa(role) + DODGE_DIF) * ATTR_RADIX
    local MSPD = (MspdSa(role) + MSPD_DIF) * ATTR_RADIX
    SetCharaAttr(DODGE, role, ATTR_STATEC_FLEE)
    SetCharaAttr(MSPD, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Tj_Rem(role, statelv)
    local DODGE_DIF = (-1) * 0.2
    local MSPD_DIF = (-1) * 0.5 + (statelv * 0.025)
    local DODGE = (FleeSa(role) - DODGE_DIF) * ATTR_RADIX
    local MSPD = (MspdSa(role) - MSPD_DIF) * ATTR_RADIX
    SetCharaAttr(DODGE, role, ATTR_STATEC_FLEE)
    SetCharaAttr(MSPD, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillSp_Sj(sklv)
	local sp_reduce = 26 + (sklv * 1)
    return sp_reduce
end

function SkillCooldown_Sj(sklv)
	local cooldown = 15000
    return cooldown
end

function Skill_Sj_Begin(role, sklv)
    if Sp(role) - SkillSp_Sj(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Sj(sklv))
end

function Skill_Sj_End(atker, defer, sklv)
    local statetime = 5 + math.floor(sklv * 0.5)
    local Damage = math.floor(100 + sklv * 10)
    HP_Red_SkillS(atker, defer, Damage, false)
    if GetChaAttr(defer, ATTR_MXHP) >= 100000 then
        if math.random(1, 100) <= 99 then
            statetime = math.floor(3 + sklv * 0.3)
        else
            SetSus(defer, 0)
            SystemNotice(atker, "Enfeeble has been evaded!")
            return
        end
    end
    if GetChaTypeID(atker) == 980 then
        statetime = 5
    end
    if GetChaMapName(defer) == "hell" or GetChaMapName(defer) == "hell2" or GetChaMapName(defer) == "hell3" or GetChaMapName(defer) == "hell4" or GetChaMapName(defer) == "hell5" then
        for i = 5, 19, 1 do
            if Abaddon.Boss[i].ID == GetChaTypeID(defer) and GetChaAIType(defer) >= 21 then
                if Abaddon.Boss[i].Skill[3] == 0 then
                    Notice(GetChaDefaultName(defer)..": Do you really think I can be defeated by the same skill? How foolish warrios!")
                    return
                else
                    Abaddon.Boss[i].Skill[3] = Abaddon.Boss[i].Skill[3] - 1
                end
            end
        end
    end
    AddState(atker, defer, STATE_SJ, sklv, statetime)
    AddState(atker, defer, STATE_JNJZ, sklv, statetime)
end

function State_Sj_Add(role, statelv)
    local MNATK_DIF = (-1) * 0.2
    local MXATK_DIF = (-1) * 0.2
    local MNATK = (MnatkSa(role) + MNATK_DIF) * ATTR_RADIX
    local MXATK = (MxatkSa(role) + MXATK_DIF) * ATTR_RADIX
    SetCharaAttr(MNATK, role, ATTR_STATEC_MNATK)
    SetCharaAttr(MXATK, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_Sj_Rem(role, statelv)
    local MNATK_DIF = (-1) * 0.2
    local MXATK_DIF = (-1) * 0.2
    local MNATK = (MnatkSa(role) - MNATK_DIF) * ATTR_RADIX
    local MXATK = (MxatkSa(role) - MXATK_DIF) * ATTR_RADIX
    SetCharaAttr(MNATK, role, ATTR_STATEC_MNATK)
    SetCharaAttr(MXATK, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function SkillSp_Bt(sklv)
	local sp_reduce = 32 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Bt(sklv)
	local cooldown = 25000
    return cooldown
end

function Skill_Bt_Begin(role, sklv)
    if Sp(role) - SkillSp_Bt(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Bt(sklv))
end

function Skill_Bt_End(atker, defer, sklv)
    local Damage = math.floor(320 + 30 * sklv + GetChaAttr(defer, ATTR_HP) * (0.05 + 0.005 * sklv))
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        Damage = math.floor(Dex(atker) * 2 + 30 * sklv + GetChaAttr(defer, ATTR_HP) * (0.05 + 0.005 * sklv))
    end
    if IsPlayer(atker) == 1 then
        if IsPlayer(defer) == 1 and Damage > 2500 then
            Damage = 2500
        elseif IsPlayer(defer) == 0 and Damage > 5000 then
            Damage = 5000
        end
    end
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 10 then
        Damage = Damage * 10
        SystemNotice(atker, "Damage was increased due to the power of the Black Dragon equipment.")
    end
    if IsPlayer(atker) == 1 and IsPlayer(defer) == 0 then
        if Damage > 5000 then
            Damage = 5000
        end
    end
    HP_Red_Skill(atker, defer, Damage, true)
end

function SkillArea_Line_ArfGX(sklv)
    local L = 850 + sklv * 75
    local W = 200 + sklv * 50
    SetSkillRange(1, L, W)
end

function SkillCooldown_ArfGX(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillSp_ArfGX(sklv)
	local sp_reduce = 90 + (sklv * 10)
    return sp_reduce
end

function Skill_ArfGX_Begin(role, sklv)
    if Sp(role) - SkillSp_ArfGX(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_ArfGX(sklv))
end

function Skill_ArfGX_End(atker, defer, sklv)
    local Damage = math.random(1750, 2250) + sklv * math.random(400, 600)
    HP_Red_SkillS(atker, defer, Damage, false)
end

function Skill_Sy_Use(role, sklv)
    local statelv = sklv
    local srecsb_dif = 1 + (statelv * 1)
    local srecsb = SrecSb(role) + srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    ALLExAttrSet(role)
end

function Skill_Sy_Unuse(role, sklv)
    local statelv = sklv
    local srecsb_dif = 2 + (statelv * 1)
    local srecsb = SrecSb(role) - srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    ALLExAttrSet(role)
end

function SkillSp_Syzy(sklv)
    local sp_reduce = 13 + (sklv * 3)
    return sp_reduce
end

function SkillCooldown_Syzy(sklv)
	local cooldown = 5000
    return cooldown
end

function SkillArea_Square_Syzy(sklv)
    local side = 600 + (sklv * 20)
    SetSkillRange(3, side)
end

function SkillArea_State_Syzy(sklv)
	local statetime = 35 + (15 * sklv)
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_Syzy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Syzy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Syzy_End(atker, defer, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillSp_Jsfb(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Jsfb(sklv)
	local cooldown = 5000
    return cooldown
end

function Skill_Jsfb_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Jxwb(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Jsfb_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local statelv = sklv
    local statetime = 33 + (sklv * 3)
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local str_atker = Str(atker)
    local Can_Pk_Garner2 = Is_NormalMonster(defer)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            statetime = math.max(30, math.floor(str_atker / 5)) + (sklv * 3)
        end
    end
    AddState(atker, defer, STATE_JSFB, statelv, statetime)
end

function State_Jsfb_Add(role, statelv)
    local crtsb_dif = 5 + (statelv * 1)
    local crtsb = math.floor((CrtSb(role) + crtsb_dif))
    SetCharaAttr(crtsb, role, ATTR_STATEV_CRT)
    ALLExAttrSet(role)
end

function State_Jsfb_Rem(role, statelv)
    local crtsb_dif = 5 + (statelv * 1)
    local crtsb = math.floor((CrtSb(role) - crtsb_dif))
    SetCharaAttr(crtsb, role, ATTR_STATEV_CRT)
    ALLExAttrSet(role)
end

function SkillSp_Tshd(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Tshd(sklv)
	local cooldown = 5000
    return cooldown
end

function Skill_Tshd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Tshd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Tshd_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local statelv = sklv
    local statetime = 33 + (sklv * 3)
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local sta_atker = Sta(atker)
    local Can_Pk_Garner2 = Is_NormalMonster(atker)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            statetime = math.max(30, math.floor(sta_atker / 5)) + sklv * 3
        end
    end
    if GetChaTypeID(atker) == 984 then
        statetime = 360
        statelv = 10
    end
    AddState(atker, defer, STATE_TSHD, statelv, statetime)
end

function State_Tshd_Add(role, statelv)
    local defsa_dif = 0.03 * statelv
    local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function State_Tshd_Rem(role, statelv)
    local defsa_dif = 0.03 * statelv
    local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Xlpz(sklv)
	local sp_reduce = 0
    return sp_reduce
end

function SkillCooldown_Xlpz(sklv)
	local cooldown = 1000
    return cooldown
end

function Skill_Xlpz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xlpz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xlpz_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = -1
    if GetChaTypeID(atker) == 984 then
        statelv = 10
    end
    AddState(atker, defer, STATE_MFD, statelv, statetime)
end

function SkillSp_Hfwq(sklv)
    local sp_reduce = 32 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Hfwq(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillArea_Square_Hfwq(sklv)
    local side = 400 + (sklv * 40)
    SetSkillRange(3, side)
end

function SkillArea_State_Hfwq(sklv)
    local statetime = 17 + (sklv * 2)
    local statelv = sklv
    SetRangeState(STATE_HFWQ, statelv, statetime)
end

function Skill_Hfwq_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_Hfwq(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hfwq_End(atker, defer, sklv)
end

function State_Hfwq_Add(role, statelv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local dmg = -1 * (50 + statelv * 15)
    Hp_Endure_Dmg(role, dmg)
end

function State_Hfwq_Rem(role, statelv)
end

function State_Hfwq_Tran(statelv)
    return 3
end

function SkillSp_BingX(sklv)
    local sp_reduce = 40 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_BingX(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_BingX_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BingX(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BingX_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local i = CheckBagItem(atker, 3463)
    if i <= 0 then
        SystemNotice(atker, "Each summon requires 1 Icy Crystal")
        return
    end
    local j = DelBagItem(atker, 3463, 1)
    if j == 1 then
        local statelv = sklv
        local statetime = 8 + (sklv * 2)
        local map_name_ATKER = GetChaMapName(atker)
        local map_name_DEFER = GetChaMapName(defer)
        local sta_atker = Sta(atker)
        local Can_Pk_Garner2 = Is_NormalMonster(defer)
        if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
            if Can_Pk_Garner2 == 0 then
                statetime = math.max(8, math.floor(sta_atker / 15)) + sklv * 2
            end
        end
        AddState(atker, defer, STATE_BIW, statelv, statetime)
    else
        LG("Skill_Item", "Delete Icy Crystal failed")
    end
end

function SkillArea_Circle_SSSP(sklv)
    local Area = 800 + (sklv * 200)
    SetSkillRange(4, Area)
end

function SkillCooldown_SSSP(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillSp_SSSP(sklv)
	local sp_reduce = 120 + (sklv * 20)
    return sp_reduce
end

function Skill_SSSP_Begin(role, sklv)
    if Sp(role) - SkillSp_SSSP(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_SSSP(sklv))
end

function Skill_SSSP_End(atker, defer, sklv)
	local Heal = 500 + (sklv * 150)
	local Damage = -200 - (sklv * 150)
	if IsChaInRegion(atker, 2) == 1 and IsChaInRegion(defer, 2) == 1 then
		Heal = 0
	end
	if is_friend(atker, defer) == 0 then
	   HP_Red_SkillS(atker, defer, Heal, false)
	else
		if is_friend(atker, defer) == 1 then
			local Damage = -200 - (sklv * 150)
			HP_Red_SkillS(atker, defer, Damage, false)
		end
	end
end

function SkillSp_Zzzh(sklv)
	local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Zzzh(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillArea_Square_Zzzh(sklv)
    local side = 300
    SetSkillRange(3, side)
end

function SkillArea_State_Zzzh(sklv)
    local statelv = sklv
    local statetime = 5 + sklv * 1
    local map_name_ATKER = GetChaMapName(atker)
    local map_name_DEFER = GetChaMapName(defer)
    local sta_atker = Sta(atker)
    local Can_Pk_Garner2 = Is_NormalMonster(defer)
    if map_name_ATKER == "garner2" or map_name_DEFER == "garner2" then
        if Can_Pk_Garner2 == 0 then
            statetime = math.max(5, math.floor(sta_atker / 30)) + sklv
        end
    end
    SetRangeState(STATE_ZZZH, statelv, statetime)
end

function Skill_Zzzh_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_Zzzh(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Zzzh_End(atker, defer, sklv)
end

function State_Zzzh_Add(role, statelv)
    local defsa_dif = (-1) * (0.1 + statelv * 0.02)
    local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function State_Zzzh_Rem(role, statelv)
    local defsa_dif = (-1) * (0.1 + statelv * 0.02)
    local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function State_Zzzh_Tran(statelv)
    local statetime = 20 + (sklv * 2)
    return statetime
end

function SkillSp_Ayzz(sklv)
	local sp_reduce = 33 + (sklv * 3)
    return sp_reduce
end

function SkillCooldown_Ayzz(sklv)
	local cooldown = 30000
    return cooldown
end

function Skill_Ayzz_Begin(role, sklv)
    if Sp(role) - SkillSp_Ayzz(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Ayzz(sklv))
end

function Skill_Ayzz_End(atker, defer, sklv)
    local statetime = 6 + (sklv * 1)
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        statetime = math.max(5, math.floor(Con(atker) / 30)) + sklv
    end
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 70 then
        statetime = statetime * 1.5
        SystemNotice(atker, "Skill effect was enhanced due to the power of the Black Dragon equipment.")
    end
    if GetChaAttr(defer, ATTR_MXHP) >= 100000 and GetChaAttr(defer, ATTR_MXHP) < 1000000 then
        if math.random(1, 100) <= 70 then
            statetime = 9
        else
            SetSus(defer, 0)
            return
        end
    elseif GetChaAttr(defer, ATTR_MXHP) >= 1000000 and math.random(1, 100) <= 70 then
        if math.random(1, 100) <= 70 then
            statetime = 4
        else
            SetSus(defer, 0)
            return
        end
    end
    if GetChaTypeID(atker) == 985 then
        statetime = 15
        sklv = 10
    end
    if GetChaMapName(defer) == "hell" or GetChaMapName(defer) == "hell2" or GetChaMapName(defer) == "hell3" or GetChaMapName(defer) == "hell4" or GetChaMapName(defer) == "hell5" then
        for i = 5, 19, 1 do
            if Abaddon.Boss[i].ID == GetChaTypeID(defer) and GetChaAIType(defer) >= 21 then
                if Abaddon.Boss[i].Skill[6] == 0 then
                    Notice(GetChaDefaultName(defer)..": Do you really think I can be defeated by the same skill? How foolish warrios!")
                    return
                else
                    Abaddon.Boss[i].Skill[6] = Abaddon.Boss[i].Skill[6] - 1
                end
            end
        end
    end
    AddState(atker, defer, STATE_GJJZ, sklv, statetime)
end

function SkillSp_Synz(sklv)
    local sp_reduce = 21 + (sklv * 1)
    return sp_reduce
end

function SkillCooldown_Synz(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillArea_Square_Synz(sklv)
    local side = 500
    SetSkillRange(3, side)
end

function SkillArea_State_Synz(sklv)
    local statetime = 20 + (sklv * 2)
    local statelv = sklv
    if GetChaTypeID(atker) == 985 then
        statetime = 40
        statelv = 10
    end
    SetRangeState(STATE_SYNZ, statelv, statetime)
end

function Skill_Synz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_Synz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Synz_End(atker, defer, sklv)
end

function State_Synz_Add(role, statelv)
    local mspdsa_dif = (-1) * (0.20 + statelv * 0.015)
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Synz_Rem(role, statelv)
    local mspdsa_dif = (-1) * (0.20 + statelv * 0.015)
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Synz_Tran(statelv)
    local statetime = 3
    return statetime
end

function SkillSp_Xzfy(sklv)
	local sp_reduce = 30 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_Xzfy(sklv)
	local cooldown = 20000
    return cooldown
end

function Skill_Xzfy_Begin(role, sklv)
    if Sp(role) - SkillSp_Xzfy(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Xzfy(sklv))
end

function Skill_Xzfy_End(atker, defer, sklv)
    local statetime = 10.5 + (sklv * 0.5)
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        statetime = math.max(10, math.floor(Con(atker) / 15)) + sklv * 0.5
    end
    if Server.EqSet["BlackDragon"](atker) == 1 and math.random(1, 100) <= 70 then
        statetime = statetime * 1.5
        SystemNotice(atker, "Skill effect was enhanced due to the power of the Black Dragon equipment.")
    end
    if GetChaAttr(defer, ATTR_MXHP) >= 100000 then
        if math.random(1, 100) <= 80 then
            statetime = 5 + (sklv * 0.3)
        else
            SetSus(defer, 0)
            SystemNotice(atker, "Usage of [Seal of Elder] failed!")
            return
        end
    end
    if GetChaTypeID(atker) == 985 then
        statetime = 15
        sklv = 10
    end
    if GetChaMapName(defer) == "hell" or GetChaMapName(defer) == "hell2" or GetChaMapName(defer) == "hell3" or GetChaMapName(defer) == "hell4" or GetChaMapName(defer) == "hell5" then
        for i = 5, 19, 1 do
            if Abaddon.Boss[i].ID == GetChaTypeID(defer) and GetChaAIType(defer) >= 21 then
                if Abaddon.Boss[i].Skill[5] == 0 then
                    Notice(GetChaDefaultName(defer)..": Do you really think I can be defeated by the same skill? How foolish warrios!")
                    return
                else
                    Abaddon.Boss[i].Skill[5] = Abaddon.Boss[i].Skill[5] - 1
                end
            end
        end
    end
    AddState(atker, defer, STATE_JNJZ, sklv, statetime)
end

function SkillSp_Mlch(sklv)
    local sp_reduce = 40 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_Mlch(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_Mlch_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Mlch(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Mlch_End(atker, defer, sklv)
    local Cha_Boat = 0
    Cha_Boat = GetCtrlBoat(defer)
    if Cha_Boat ~= nil then
        BickerNotice(atker, "Cannot use while sailing")
        return
    end
    local i = CheckBagItem(atker, 3462)
    if i <= 0 then
        SystemNotice(atker, "Intensify requires one Magical Clover")
        return
    end
    local b = (sklv - 1) * 0.05
    local a = Percentage_Random(b)
    local j = 0
    if a == 0 then
        j = DelBagItem(atker, 3462, 1)
    elseif a == 1 then
        j = 1
        SystemNotice(atker, "Entering skill discharge, does not consume a Magical Clover")
    end
    if j == 1 then
        local statelv = sklv
        local statetime = 90 + (sklv * 90) 
        AddState(atker, defer, STATE_MLCH, statelv, statetime)
    else
        LG("Skill_Item", "Delete Magical Clover failed")
    end
end

function State_Mlch_Add(role, statelv)
end

function State_Mlch_Rem(role, statelv)
end

function SkillSp_EmoYuYan(sklv)
	local sp_reduce = 160 + (sklv * 20)
    return sp_reduce
end

function SkillArea_Circle_EmoYuYan(sklv)
    local Area = 500 + sklv * 75
    SetSkillRange(4, Area)
end

function SkillCooldown_EmoYuYan(sklv)
	local cooldown = 30000
    return cooldown
end

function Skill_EmoYuYan_Begin(role, sklv)
    if Sp(role) - SkillSp_EmoYuYan(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_EmoYuYan(sklv))
end

function Skill_EmoYuYan_End(atker, defer, sklv)
    local statetime = 25
    local statelv = sklv
    local Damage = math.random(900, 1100) + sklv * math.random(200, 400)
    HP_Red_SkillS(atker, defer, Damage, false)
    AddState(atker, defer, STATE_EMYY, statelv, statetime)
    AddState(atker, defer, STATE_MOWQ, statelv, statetime)
end

function State_EmoYuYan_Add(role, statelv)
    local Damage = 20 + (statelv * 15)
    Hp_Endure_Dmg(role, Damage)
    ALLExAttrSet(role)
end

function State_EmoYuYan_Rem(role, statelv)
end

function State_EmoYuYan2_Add(role, statelv)
    local Defense = 0.05
    local Movement = 0.15
    local Attack = 0.1
    local Speed = 0.25
    Defense = Defense * statelv
    Movement = Movement * statelv
    Attack = Attack * statelv
    Speed = Speed * statelv

    local DEF = math.floor((DefSa(role) + (-1 * Defense)) * ATTR_RADIX)
    local MOV = math.floor((MspdSa(role) + (-1 * Movement)) * ATTR_RADIX)
    local MNATK = math.floor((MnatkSa(role) + (-1 * Attack)) * ATTR_RADIX)
    local MXATK = math.floor((MxatkSa(role) + (-1 * Attack)) * ATTR_RADIX)
    local ASPD = math.floor((AspdSa(role) + (-1 * Speed)) * ATTR_RADIX)
    SetCharaAttr(DEF, role, ATTR_STATEC_DEF)
    SetCharaAttr(MOV, role, ATTR_STATEC_MSPD)
    SetCharaAttr(MNATK, role, ATTR_STATEC_MNATK)
    SetCharaAttr(MXATK, role, ATTR_STATEC_MXATK)
    SetCharaAttr(ASPD, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_EmoYuYan2_Rem(role, statelv)
    local Defense = 0.05
    local Movement = 0.15
    local Attack = 0.1
    local Speed = 0.25
    Defense = Defense * statelv
    Movement = Movement * statelv
    Attack = Attack * statelv
    Speed = Speed * statelv

    local DEF = math.floor((DefSa(role) - (-1 * Defense)) * ATTR_RADIX)
    local MOV = math.floor((MspdSa(role) - (-1 * Movement)) * ATTR_RADIX)
    local MNATK = math.floor((MnatkSa(role) - (-1 * Attack)) * ATTR_RADIX)
    local MXATK = math.floor((MxatkSa(role) - (-1 * Attack)) * ATTR_RADIX)
    local ASPD = math.floor((AspdSa(role) - (-1 * Speed)) * ATTR_RADIX)
    SetCharaAttr(DEF, role, ATTR_STATEC_DEF)
    SetCharaAttr(MOV, role, ATTR_STATEC_MSPD)
    SetCharaAttr(MNATK, role, ATTR_STATEC_MNATK)
    SetCharaAttr(MXATK, role, ATTR_STATEC_MXATK)
    SetCharaAttr(ASPD, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function SkillArea_Line_Bkcj(sklv)
    local L = 500 + sklv * 30
    local W = 100 + sklv * 10
    SetSkillRange(1, L, W)
end

function SkillSp_Bkcj(sklv)
	local sp_reduce = 23 + (sklv * 3)
    return sp_reduce
end

function SkillCooldown_Bkcj(sklv)
	local cooldown = 8000 - (sklv * 500)
    return cooldown
end

function SkillEnergy_Bkcj(sklv)
    local energy_reduce = 3 + math.floor(sklv * 0.5)
    return energy_reduce
end

function Skill_Bkcj_Begin(role, sklv)
    if Sp(role) - SkillSp_Bkcj(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_Bkcj(sklv))
end

function Skill_Bkcj_End(atker, defer, sklv)
    local Damage = math.floor(200 + sklv * 30 + Sta(atker) * 6) * 1.1
    if (GetChaMapName(atker) == "garner2" or GetChaMapName(defer) == "garner2") and Is_NormalMonster(defer) == 0 then
        Damage = math.floor(MAGIC_Atk_Dmg(atker, defer) + sklv * 200)
        if math.random(1, 200) >= Con(defer) then
            local statetime = 1
            AddState(atker, defer, STATE_XY, sklv, statetime)
        end
    end
    Damage = Cuihua_Mofa(Damage, (GetChaStateLv(atker, STATE_MLCH)))
    Damage = Damage + ElfSkill_MagicAtk(Damage, atker)
    if GetChaAttr(atker, ATTR_CSAILEXP) > 0 and GetChaAttr(atker, ATTR_CSAILEXP) < 100 then
        Damage = Damage * 1.055
    end
    if GetChaAttr(atker, ATTR_CSAILEXP) >= 100 and GetChaAttr(atker, ATTR_CSAILEXP) < 12100 then
        Damage = Damage * (1.05 + math.floor(math.pow((GetChaAttr(atker, ATTR_CSAILEXP) / 100), 0.5)) * 0.005)
    end
    HP_Red_Skill(atker, defer, Damage, false)
	LookEnergy(atker)
end

function SkillArea_Circle_Lm(sklv)
    local side = 300
    SetSkillRange(3, side)
end

function SkillCooldown_Lm(sklv)
	local cooldown = 10000
    return cooldown
end

function SkillSp_Lm(sklv)
    local sp_reduce = 21 + (sklv * 1)
    return sp_reduce
end

function SkillEnergy_Lm(sklv)
    local energy_reduce = 3 + math.floor(sklv * 0.5)
    return energy_reduce
end

function SkillArea_State_Lm(sklv)
    local statetime = 15 + sklv
    local statelv = sklv
    SetRangeState(STATE_LM, statelv, statetime)
end

function Skill_Lm_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Lm(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Lm_End(atker, defer, sklv)
end

function State_Lm_Add(role, statelv)
    local dmg = 160 + (statelv * 20)
    Hp_Endure_Dmg(role, dmg)
end

function State_Lm_Rem(role, statelv)
end

function State_Lm_Tran(statelv)
    return 1
end

function SkillArea_Circle_Sf(sklv)
    local side = 1000
    SetSkillRange(3, side)
end

function SkillCooldown_Sf(sklv)
	local cooldown = 10000
    return cooldown
end

function SkillSp_Sf(sklv)
    local sp_reduce = 23 + (sklv * 3)
    return sp_reduce
end

function SkillEnergy_Sf(sklv)
    local energy_reduce = 3 + math.floor(sklv * 0.5)
    return energy_reduce
end

function Skill_Sf_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Sf(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Sf_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 150 + (sklv * 10)
    AddState(atker, defer, STATE_SF, statelv, statetime)
end

function State_Sf_Add(role, statelv)
    local mspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_Sf_Rem(role, statelv)
    local mspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillArea_Circle_Mw(sklv)
    local side = 300 + sklv * 50
    SetSkillRange(3, side)
end

function SkillCooldown_Mw(sklv)
	local cooldown = 10000
    return cooldown
end

function SkillSp_Mw(sklv)
    local sp_reduce = 21 + (sklv * 1)
    return sp_reduce
end

function SkillArea_State_Mw(sklv)
    local statetime = 20
    local statelv = sklv
    SetRangeState(STATE_MW, statelv, statetime)
end

function SkillEnergy_Mw(sklv)
    local energy_reduce = 3 + math.floor(sklv * 0.5)
    return energy_reduce
end

function Skill_Mw_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Mw(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Mw_End(atker, defer, sklv)
	LookEnergy(atker)
end

function State_Mw_Add(role, statelv)
    local mnatksa_dif = 0.05 + (statelv * 0.01)
    local mxatksa_dif = 0.05 + (statelv * 0.01)
    local mnatksa = (MnatkSa(role) - mnatksa_dif) * ATTR_RADIX
    local mxatksa = (MxatkSa(role) - mxatksa_dif) * ATTR_RADIX
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_Mw_Rem(role, statelv)
    local mnatksa_dif = 0.05 + (statelv * 0.01)
    local mxatksa_dif = 0.05 + (statelv * 0.01)
    local mnatksa = (MnatkSa(role) + mnatksa_dif) * ATTR_RADIX
    local mxatksa = (MxatkSa(role) + mxatksa_dif) * ATTR_RADIX
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_Mw_Tran(statelv)
    return 1
end

function SkillArea_Circle_Xw(sklv)
    local side = 300
    SetSkillRange(3, side)
end

function SkillCooldown_Xw(sklv)
	local cooldown = 10000
    return cooldown
end

function SkillSp_Xw(sklv)
    local sp_reduce = 20 + (sklv * 1)
    return sp_reduce
end

function SkillArea_State_Xw(sklv)
    local statetime = 20 + (sklv * 1)
    local statelv = sklv
    SetRangeState(STATE_XW, statelv, statetime)
end

function Skill_Xw_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xw(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xw_End(atker, defer, sklv)
end

function State_Xw_Add(role, statelv)
    local mspdsa_dif = 0.1 + (statelv * 0.02)
    local aspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    local aspdsa = (AspdSa(role) - aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_Xw_Rem(role, statelv)
    local mspdsa_dif = 0.1 + (statelv * 0.02)
    local aspdsa_dif = 0.05 + (statelv * 0.01)
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    local aspdsa = (AspdSa(role) + aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_Xw_Tran(statelv)
    return 1
end

function SkillSp_XYSYF(sklv)
	local sp_reduce = 140 + (sklv * 20)
    return sp_reduce
end

function SkillCooldown_XYSYF(sklv)
	local cooldown = 30000
    return cooldown
end

function SkillArea_Circle_XYSYF(sklv)
    local Area = 600 + (sklv * 200)
    SetSkillRange(4, Area)
end

function SkillArea_State_XYSYF(sklv)
    local statetime = 15
    SetRangeState(STATE_YNZL, sklv, statetime)
end

function Skill_XYSYF_Begin(role, sklv)
    if Sp(role) - SkillSp_XYSYF(sklv) < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, SkillSp_XYSYF(sklv))
end

function Skill_XYSYF_End(atker, defer, sklv)
end

function State_XYSYF_Add(role, statelv)
    local Damage = math.floor(math.random(500, 700) + statelv * math.random(200, 400))
    Hp_Endure_Dmg(role, Damage)
end

function State_XYSYF_Rem(role, statelv)
end

function State_XYSYF_Tran(statelv)
    return 3
end

function SkillCooldown_HLKJ(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_HLKJ_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_HLKJ(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_HLKJ_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 30
    AddState(atker, defer, STATE_HLKJ, statelv, statetime)
end

function State_HLKJ_Add(role, statelv)
end

function State_HLKJ_Rem(role, statelv)
end

function SkillArea_Circle_HLLM(sklv)
    local side = 500
    SetSkillRange(4, side)
end

function SkillCooldown_HLLM(sklv)
	local cooldown = 1000
    return cooldown
end

function SkillPre_HLLM(sklv)
end

function SkillSp_HLLM(sklv)
    local sp_reduce = 20 - math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_HLLM_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_HLLM(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_HLLM_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 120
    AddState(atker, defer, STATE_HLLM, statelv, statetime)
end

function State_HLLM_Add(role, statelv)
    local mxatksa_dif = 0.5
    local mnatksa_dif = 0.5
    local mxatksa = (MxatkSa(role) - mxatksa_dif * ATTR_RADIX)
    local mnatksa = (MnatkSa(role) - mnatksa_dif * ATTR_RADIX)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    ALLExAttrSet(role)
end

function State_HLLM_Rem(role, statelv)
    local mxatksa_dif = 0.5
    local mnatksa_dif = 0.5
    local mxatksa = (MxatkSa(role) + mxatksa_dif * ATTR_RADIX)
    local mnatksa = (MnatkSa(role) + mnatksa_dif * ATTR_RADIX)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    ALLExAttrSet(role)
end

function SkillArea_Circle_BlackDrgWing(sklv)
    local side = 3000
    SetSkillRange(4, side)
end

function SkillCooldown_BlackDrgWing(sklv)
	local cooldown = 1000
    return cooldown
end

function SkillPre_BlackDrgWing(sklv)
end

function SkillSp_BlackDrgWing(sklv)
    local sp_reduce = 20 - math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_BlackDrgWing_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BlackDrgWing(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BlackDrgWing_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(100, (150 - Sta_role)) * 15
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_BlackDrgDeadHit(sklv)
	local sp_reduce = 15
    return sp_reduce
end

function SkillArea_Sector_BlackDrgDeadHit(sklv)
    local angle = 120
    local radius = 600
    SetSkillRange(2, radius, angle)
end

function SkillCooldown_BlackDrgDeadHit(sklv)
	local cooldown = 1500
    return cooldown
end

function Skill_BlackDrgDeadHit_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BlackDrgDeadHit(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BlackDrgDeadHit_End(atker, defer, sklv)
    local hpdmg = 1.5 * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_BlackHeal(sklv)
    local sp_reduce = 30 + (sklv * 4)
    return sp_reduce
end

function SkillCooldown_BlackHeal(sklv)
    local cooldown = 7000 - (sklv * 300)
    return cooldown
end

function Skill_BlackHeal_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BlackHeal(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BlackHeal_End(atker, defer, sklv)
    local hpdmg = -50000
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillCooldown_BlackYq(sklv)
	local cooldown = 10000
    return cooldown
end

function Skill_BlackYq_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = 2500 + math.max(50, (150 - Sta_role)) * 20
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillCooldown_BlackZh(sklv)
	local cooldown = 1200000
    return cooldown
end

function SkillArea_Circle_BlackZh(sklv)
    local side = 1000
    SetSkillRange(4, side)
end

function Skill_BlackZh_End(atker, defer, sklv)
    local x, y = GetChaPos(atker)
    local x1 = x
    local x2 = x + 700
    local x3 = x - 700
    local y1 = y + 700
    local y2 = y - 700
    local y3 = y - 700
    local new1 = CreateCha(791, x1, y1, 145, 50)
    local new2 = CreateCha(793, x2, y2, 145, 50)
    local new3 = CreateCha(794, x3, y3, 145, 50)
    SetChaLifeTime(new1, 900000)
    SetChaLifeTime(new2, 900000)
    SetChaLifeTime(new3, 900000)
end

function SkillArea_Circle_BlackHx(sklv)
    local side = 5000
    SetSkillRange(4, side)
end

function SkillCooldown_BlackHx(sklv)
	local cooldown = 10000
    return cooldown
end

function Skill_BlackHx_End(atker, defer, sklv)
    local hp = Hp(defer)
    local statelv = sklv
    local statetime = 10
    AddState(atker, defer, STATE_BlackHX, statelv, statetime)
    dmg = 0.8 * math.random(Mnatk(atker), Mxatk(atker))
    Hp_Endure_Dmg(defer, dmg)
end

function State_BlackHx_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    local aspda_dif = 0.3
    local aspdsa = (AspdSa(role) - aspda_dif) * ATTR_RADIX
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_BlackHx_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    local aspda_dif = 0.3
    local aspdsa = (AspdSa(role) + aspda_dif) * ATTR_RADIX
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillCooldown_BlackLj(sklv)
	local cooldown = 10000
    return cooldown
end

function Skill_BlackLj_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 3
    local Sta_role = Sta(defer)
    local statelv = 4
    hpdmg = 1000 + math.max(50, (150 - Sta_role)) * 10
    Hp_Endure_Dmg(defer, hpdmg)
    AddState(atker, defer, STATE_HLKJ, statelv, statetime)
end

function State_BlackLj_Add(role, statelv)
end

function State_BlackLj_Rem(role, statelv)
end


function SkillCooldown_BlackHyz(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_BlackHyz_End(atker, defer, sklv)
    atk = 2 * math.random(Mnatk(atker), Mxatk(atker))
    defer_def = Def(defer)
    defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    dmg1 = math.max(20, dmg)
    Hp_Endure_Dmg(defer, dmg1)
    Check_Ys_Rem(atker, defer)
end

function SkillCooldown_jsfd(atker, defer, sklv)
	local cooldown = 1000
    return cooldown
end

function Skill_jsfd_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = 500 + math.max(50, (100 - Sta_role)) * 8
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillCooldown_wlfd(sklv)
	local cooldown = 1000
    return cooldown
end

function Skill_wlfd_End(atker, defer, sklv)
    local atk = math.random(Mnatk(atker), Mxatk(atker))
    local defer_def = Def(defer)
    local defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    dmg1 = math.max(20, dmg)
    Hp_Endure_Dmg(defer, dmg1)
end

function SkillCooldown_zzzx(atker, defer, sklv)
	local cooldown = 2000
    return cooldown
end

function SkillArea_Circle_zzzx(sklv)
    local side = 2000
    SetSkillRange(4, side)
end

function Skill_zzzx_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 10
    AddState(atker, defer, STATE_ZZZX, statelv, statetime)
end

function State_wlcx_Add(role, sklv)
    local hpdmg = 100
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_wlcx_Rem(role, sklv)
    ALLExAttrSet(role)
end

function SkillCooldown_yghf(atker, defer, sklv)
	local cooldown = 2000
    return cooldown
end

function Skill_yghf_End(atker, defer, sklv)
    local hp = Hp(atker)
    local mxhp = Mxhp(atker)
    local hp_dif = mxhp - hp
    local hp_rec = hp_dif * 0.2
    local hp_now = hp + hp_rec
    if hp_now < 100000 then
        local hp_rec = 100000
    end
    SetCharaAttr(hp_now, atker, ATTR_HP)
end

function SkillCooldown_wlzh(sklv)
	local cooldown = 500000
    return cooldown
end

function SkillArea_Circle_wlzh(sklv)
    local side = 1000
    SetSkillRange(4, side)
end

function Skill_wlzh_End(atker, defer, sklv)
    local x, y = GetChaPos(atker)
    local x1 = x + 200
    local x2 = x + 200
    local x3 = x - 200
    local x4 = x - 200
    local y1 = y + 200
    local y2 = y - 200
    local y3 = y + 200
    local y4 = y - 200
    local new1 = CreateCha(799, x1, y1, 145, 50)
    local new2 = CreateCha(799, x2, y2, 145, 50)
    local new3 = CreateCha(799, x3, y3, 145, 50)
    local new4 = CreateCha(799, x4, y4, 145, 50)
    SetChaLifeTime(new1, 900000)
    SetChaLifeTime(new2, 900000)
    SetChaLifeTime(new3, 900000)
    SetChaLifeTime(new4, 900000)
end

function SkillCooldown_ycbp(atker, defer, sklv)
	local cooldown = 2000
    return cooldown
end

function Skill_ycbp_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = 1000 + math.max(50, (150 - Sta_role)) * 20
    Hp_Endure_Dmg(defer, hpdmg)
end

function State_wldb_Add(role, statelv)
    local hpdmg = 160
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_wldb_Rem(role, statelv)
end

function Skill_wlsj_End(atker, defer, sklv)
    atk = 800 + math.random(Mnatk(atker), Mxatk(atker))
    defer_def = Def(defer)
    defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    dmg1 = math.max(20, dmg)
    Hp_Endure_Dmg(defer, dmg1)
end

function SkillArea_Circle_wllm(sklv)
    local side = 500
    SetSkillRange(3, side)
end

function SkillCooldown_wllm(atker, defer, sklv)
	local cooldown = 2000
    return cooldown
end

function SkillArea_State_wllm(sklv)
    local statetime = 15
    local statelv = 7
    SetRangeState(STATE_LM, statelv, statetime)
end

function Skill_wllm_End(atker, defer, sklv)
end

function State_Lm_Add(role, statelv)
    local dmg = 160 + statelv * 20
    Hp_Endure_Dmg(role, dmg)
end

function State_Lm_Rem(role, statelv)
end

function State_Lm_Tran(statelv)
    return 1
end

function SkillCooldown_wllk(atker, defer, sklv)
	local cooldown = 2000
    return cooldown
end

function Skill_wllk_End(atker, defer, sklv)
    atk = math.random(Mnatk(atker), Mxatk(atker))
    defer_def = Def(defer)
    defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    dmg1 = math.max(20, dmg)
    Hp_Endure_Dmg(defer, dmg1)
end

function SkillCooldown_zdtz(atker, defer, sklv)
	local cooldown = 4000
    return cooldown
end

function Skill_zdtz_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = 500 + math.max(50, (100 - Sta_role)) * 8
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillCooldown_wlrsd(sklv)
    return 2000
end

function Skill_wlrsd_End(atker, defer, sklv)
    local hp = Hp(defer)
    local statelv = sklv
    local statetime = 6
    AddState(atker, defer, STATE_WLRSD, statelv, statetime)
    dmg = 2 * math.random(Mnatk(atker), Mxatk(atker))
    Hp_Endure_Dmg(defer, dmg)
end

function State_wlrsd_Add(role, statelv)
    local hpdmg = 60
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_wlrsd_Rem(role, statelv)
end

function SkillArea_Circle_Paodan(sklv)
    local side = 400
    SetSkillRange(4, side)
end

function Skill_Paodan_Begin(role, sklv)
end

function Skill_Paodan_End(atker, defer, sklv)
    skr_posx, skr_posy = GetSkillPos(atker)

    if ValidCha(defer) == 0 then
        LG("luascript_err", "fucntion Skill_Paodan_End : Cannon attack, send target index as nil\n")
        return
    end
    role_posx, role_posy = GetChaPos(defer)

    local dmg = Fire_Dmg(atker, defer)
    local dis = Dis(skr_posx, skr_posy, role_posx, role_posy)
    local dis_eff = dis / 100 * 0.1
    dmg = math.floor(dmg * (1 - math.min(dis_eff, 1)))
    Hp_Endure_Dmg(defer, dmg)
end

function SkillCooldown_hqgj(sklv)
	local cooldown = 3000
    return cooldown
end

function Skill_hqgj_End(atker, defer, sklv)
    local atk = 1.5 * math.random(Mnatk(atker), Mxatk(atker))
    local defer_def = Def(defer)
    local defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    dmg1 = math.max(20, dmg)
    Hp_Endure_Dmg(defer, dmg1)
end

function SkillCooldown_wljy(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_wljy_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 3
    local statelv = 4
    local Sta_role = Sta(defer)
    hpdmg = 400 + math.max(50, (150 - Sta_role)) * 8
    Hp_Endure_Dmg(defer, hpdmg)
    AddState(atker, defer, STATE_WLJY, statelv, statetime)
end

function State_wljy_Add(role, statelv)
end

function State_wljy_Rem(role, statelv)
end

function SkillCooldown_xegj(atker, defer, sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_xegj_End(atker, defer, sklv)
    local statelv = sklv
    local Sta_role = Sta(defer)
    hpdmg = 400 + math.max(50, (150 - Sta_role)) * 5
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillCooldown_klcs(atker, defer, sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_klcs_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 6
    AddState(atker, defer, STATE_KLCS, statelv, statetime)
end

function State_klcs_Add(role, statelv)
    local dmg = math.random(60, 100)
    Endure_Dmg(role, dmg)
end

function State_klcs_Rem(role, statelv)
end

function SkillCooldown_lyyd(atker, defer, sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_lyyd_End(atker, defer, sklv)
    local x, y = GetChaPos(defer)
    local map_name = GetChaMapName(defer)
    local x = math.floor(x / 100)
    local y = math.floor(y / 100)
    GoTo(atker, x, y, map_name)
end

function SkillSp_JSDD(sklv)
    local sp_reduce = 5
    return sp_reduce
end

function SkillCooldown_JSDD(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_JSDD_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_JSDD(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_JSDD_End(atker, defer, sklv)
    local statelv = 1
    local statetime = 30
    dmg = Atk_Dmg(atker, defer)
    sus, dmgsa = Check_MisorCrt(atker, defer)
    SetSus(defer, sus)
    hpdmg = math.floor(dmg * dmgsa)
    Hp_Endure_Dmg(defer, hpdmg)

    local StateLv = GetChaStateLv(defer, STATE_TTISW)
    if StateLv ~= 4 then
        AddState(atker, defer, STATE_JSDD, statelv, statetime)
    end
    Check_Ys_Rem(atker, defer)
end

function State_JSDD_Add(role, statelv)
    if statelv == 3 then
    end
    if statelv == 4 then
    end
    local hpdmg = 100 * statelv
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_JSDD_Rem(role, statelv)
end

function SkillSp_QuanX(sklv)
    local sp_reduce = 50 + (sklv * 5)
    return sp_reduce
end

function SkillCooldown_QuanX(sklv)
    local cooldown = 3000
    return cooldown
end

function SkillArea_Circle_QuanX(sklv)
    local side = 1000
    SetSkillRange(4, side)
end

function Skill_QuanX_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_QuanX(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function SkillSp_SD(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_SD(sklv)
    local cooldown = 20000
    return cooldown
end

function Skill_SD_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_SD(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_SD_End(atker, defer, sklv)
    local statelv = 3
    local statetime = 60
    local hpdmg = 300
    Hp_Endure_Dmg(defer, hpdmg)
    AddState(atker, defer, STATE_JSDD, statelv, statetime)
    Check_Ys_Rem(atker, defer)
end

function SkillSp_XBLBD(sklv)
    local sp_reduce = 50 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_XBLBD(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_XBLBD_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_XBLBD(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_XBLBD_End(atker, defer, sklv)
    local statelv = 5
    local statetime = 180
    local StateLv = GetChaStateLv(defer, STATE_TTISW)
    if StateLv ~= 1 then
        AddState(atker, defer, STATE_BDJ, statelv, statetime)
    end
    Check_Ys_Rem(atker, defer)
end

function SkillSp_CRXSF(sklv)
    local sp_reduce = 50 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_CRXSF(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_CRXSF_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_CRXSF(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_CRXSF_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 15
    local StateLv = GetChaStateLv(defer, STATE_TTISW)
    if StateLv ~= 3 then
        AddState(atker, defer, STATE_CRXSF, statelv, statetime)
    end
    Check_Ys_Rem(atker, defer)
end

function State_CRXSF_Add(role, statelv)
end

function State_CRXSF_Rem(role, statelv)
end

function SkillSp_SXZZZ(sklv)
    local sp_reduce = 50 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_SXZZZ(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_SXZZZ_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_SXZZZ(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_SXZZZ_End(atker, defer, sklv)
    local statelv = 10
    local statetime = 180
    local StateLv = GetChaStateLv(defer, STATE_TTISW)
    if StateLv ~= 2 then
        AddState(atker, defer, STATE_ZZZH, statelv, statetime)
    end
    Check_Ys_Rem(atker, defer)
end

function SkillSp_JSMF(sklv)
    local sp_reduce = 3 + (sklv * 2)
    return sp_reduce
end

function SkillArea_Line_JSMF(sklv)
    local lenth = 800
    local width = 200 + (sklv * 10)
    SetSkillRange(1, lenth, width)
end

function SkillCooldown_JSMF(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_JSMF_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_JSMF(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_JSMF_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(1, math.max(30, math.floor((150 - Sta_role)) * 2.8))
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_Swcx(sklv)
    local sp_reduce = 50
    return sp_reduce
end

function SkillCooldown_Swcx(sklv)
    local cooldown = 6000
    return cooldown
end

function Skill_Swzq_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Swcx(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Swcx_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 30
    AddState(atker, defer, STATE_SWCX, statelv, statetime)
end

function State_Swcx_Add(role, statelv)
end

function State_Swcx_Rem(role, statelv)
end

function SkillSp_Xn(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Xn(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_Xn_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xn(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xn_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(atker, defer, STATE_XN, statelv, statetime)
    Check_Ys_Rem(atker, defer)
end

function State_Xn_Add(role, statelv)
    local hpdmg = 300
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_Xn_Rem(role, statelv)
end

function SkillSp_Nt(sklv)
    local sp_reduce = 40
    return sp_reduce
end

function SkillCooldown_Nt(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_Nt_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Nt(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Nt_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 60
    local hpdmg = 3 * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, hpdmg)
    AddState(atker, defer, STATE_NT, statelv, statetime)
end

function State_Nt_Add(role, statelv)
    local mspdsa_dif = 0.5
    local hitsa_dif = 0.5
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    local hitsa = math.floor((HitSa(role) - hitsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    ALLExAttrSet(role)
end

function State_Nt_Rem(role, statelv)
    local mspdsa_dif = 0.5
    local hitsa_dif = 0.5
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    local hitsa = math.floor((HitSa(role) + hitsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    ALLExAttrSet(role)
    ALLExAttrSet(role)
end

function SkillSp_DiZ(sklv)
    local sp_reduce = 50 + (sklv * 5)
    return sp_reduce
end

function SkillCooldown_DiZ(sklv)
    local cooldown = 3000
    return cooldown
end

function SkillArea_Square_DiZ(sklv)
    local side = 1000
    SetSkillRange(4, side)
end

function SkillArea_State_DiZ(sklv)
    local statetime = 20
    local statelv = sklv
    SetRangeState(STATE_DIZ, statelv, statetime)
end

function Skill_DiZ_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_DiZ(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_DiZ_End(atker, defer, sklv)
    local statetime = 20
    local statelv = 10
    AddState(atker, defer, STATE_DIZ, statelv, statetime)
    AddState(atker, defer, STATE_XY, statelv, 10)
end

function State_DiZ_Add(role, statelv)
    local mspdsa_dif = (-1) * 0.30
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_DiZ_Rem(role, statelv)
    local mspdsa_dif = (-1) * 0.30
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_DiZ_Tran(statelv)
    local statetime = 10
    return statetime
end

function SkillSp_XiK(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Xik(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_Xik_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Xik(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Xik_End(atker, defer, sklv)
    local aspd = Aspd(atker)
    local dmg = 20 * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, dmg)

    Check_Ys_Rem(atker, defer)
end

function SkillArea_Circle_Kdzb(sklv)
    local side = 400
    SetSkillRange(4, side)
end

function SkillCooldown_Sgjn1(sklv)
    local cooldown = 6000
    return cooldown
end

function SkillCooldown_Sgjn2(sklv)
    local cooldown = 2500
    return cooldown
end

function SkillCooldown_Kdzb(sklv)
    local cooldown = 1000
    return cooldown
end

function SkillPre_Kdzb(sklv)
end

function SkillSp_Kdzb(sklv)
    local sp_reduce = 20 - math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_Kdzb_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Wzxf(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Kdzb_End(atker, defer, sklv)
    dmg = 3 * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, dmg)
end

function SkillCooldown_HyzHX(sklv)
    local cooldown = 1000
    return cooldown
end

function Skill_HyzHX_End(atker, defer, sklv)
    if ValidCha(atker) == 0 then
        LG("luascript_err", "function Skill_Hyz_End : atker as null")
        return
    end
    if ValidCha(defer) == 0 then
        LG("luascript_err", "function Skill_Hyz_End : defer as null")
        return
    end
    local aspd = Aspd(atker)
    local sklv = 10
    local dmg = (1.5 + 0.1 * sklv) * (math.min(3, (math.max(1, math.floor(aspd / 70))))) * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, dmg)
    Check_Ys_Rem(atker, defer)
end

function Skill_JSBT_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = 300 + math.max(50, (150 - Sta_role)) * 10
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_Biw(sklv)
    local sp_reduce = 50
    return sp_reduce
end

function SkillCooldown_Biw(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_Biw_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Swcx(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Biw_End(atker, defer, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(atker, defer, STATE_BIW, statelv, statetime)
end

function State_Biw_Add(role, statelv)
end

function State_Biw_Rem(role, statelv)
end

function SkillArea_Circle_Fer(sklv)
    local side = 1000
    SetSkillRange(4, side)
end

function SkillSp_Fer(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Fer(sklv)
    local cooldown = 5000
    return cooldown
end

function SkillPre_Fer(sklv)
end

function Skill_Fer_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Fer(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Fer_End(atker, defer, sklv)
    local hp = Hp(defer)
    dmg = 2 * Atk_Dmg(atker, defer)
    Hp_Endure_Dmg(defer, dmg)
    Check_Ys_Rem(atker, defer)
end

function SkillArea_Line_BkcjHX(sklv)
    local sklv = 8
    local lenth = 500 + sklv * 30
    local width = 100 + sklv * 10
    SetSkillRange(1, lenth, width)
end

function SkillCooldown_BkcjHX(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_BkcjHX_End(atker, defer, sklv)
    local sklv = 8
    local sta_atk = Sta(atker)
    local sta_def = Sta(defer)
    local AddStateLv = 0
    AddStateLv = GetChaStateLv(atker, STATE_MLCH)

    local dmg = math.floor(200 + sklv * 30 + sta_atk * 6)
    local dmg_fin = Cuihua_Mofa(dmg, AddStateLv)
    local dmg_ElfSkill = ElfSkill_MagicAtk(dmg, atker)
    dmg_fin = dmg_fin + dmg_ElfSkill
    Hp_Endure_Dmg(defer, dmg_fin)
end

function SkillCooldown_BtHX(sklv)
    local cooldown = 5000
    return cooldown
end

function Skill_BtHX_End(atker, defer, sklv)
    local sklv = 10
    local hp = GetChaAttr(defer, ATTR_HP)
    local dmg = math.floor(320 + 30 * sklv + hp * (0.05 + 0.005 * sklv))
    if dmg > 2500 then
        dmg = 2500
    end
    local Check_Heilong = CheckItem_Heilong(atker)
    if Check_Heilong == 1 then
        local Percentage = Percentage_Random(0.1)
        if Percentage == 1 then
            dmg = dmg * 10
            SystemNotice(atker, "Obtain power from Black Dragon set. Damage bonus")
        end
    end
    hp = hp - dmg
    SetCharaAttr(hp, defer, ATTR_HP)
end

function SkillCooldown_XlczHX(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_XlczHX_End(atker, defer, sklv)
    if ValidCha(atker) == 0 then
        LG("luascript_err", "function Skill_Xlcz_End : atker as null")
        return
    end
    if ValidCha(defer) == 0 then
        LG("luascript_err", "function Skill_Xlcz_End : defer as null")
        return
    end

    local lv_atker = Lv(TurnToCha(atker))
    local lv_defer = Lv(TurnToCha(defer))
    local sta_atker = Sta(atker)
    local sklv = 10
    local sta_defer = Sta(defer)
    local lv_dif = math.max((-1) * 10, math.min(10, lv_atker - lv_defer))
    local AddStateLv = 0
    AddStateLv = GetChaStateLv(atker, STATE_MLCH)
    hpdmg = math.floor((10 + sta_atker * 2) * (1 + sklv * 0.7) * (1 + lv_dif * 0.025))
    
	local dmg_fin = Cuihua_Mofa(hpdmg, AddStateLv)
    local dmg_ElfSkill = ElfSkill_MagicAtk(hpdmg, atker)
    dmg_fin = dmg_fin + dmg_ElfSkill
    Hp_Endure_Dmg(defer, dmg_fin)
end

function SkillArea_Circle_TZQZMagic(sklv)
    local side = 100
    SetSkillRange(4, side)
end

function SkillSp_TZQZMagic(sklv)
    local sp_reduce = 10 + sklv * 2
    return sp_reduce
end

function SkillCooldown_TZQZMagic(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_TZQZMagic_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_TZQZMagic(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_TZQZMagic_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    local statelv = 4
    local statetime = 15
    hpdmg = math.max(50, (150 - Sta_role)) * 5
    Hp_Endure_Dmg(defer, hpdmg)
    AddState(atker, defer, STATE_JSDD, statelv, statetime)
end

function SkillSp_FoxMagic(sklv)
    local sp_reduce = 10 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_FoxMagic(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_FoxMagic_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_FoxMagic(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_FoxMagic_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(50, (150 - Sta_role)) * 10
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillArea_Circle_FoxSquareMagic(sklv)
    local side = 100
    SetSkillRange(4, side)
end

function SkillCooldown_FoxSquareMagic(sklv)
    local cooldown = 7000 - (sklv * 500)
    return cooldown
end

function SkillSp_FoxSquareMagic(sklv)
    local sp_reduce = 20 + sklv * 3
    return sp_reduce
end

function Skill_FoxSquareMagic_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_FoxSquareMagic(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_FoxSquareMagic_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(30, (150 - math.floor(Sta_role / 2))) * 5 + 300
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_HYMF(sklv)
    local sp_reduce = 10 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_HYMF(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_HYMF_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_HYMF(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_HYMF_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(1, math.floor(math.max(50, (150 - Sta_role)) * 3.5))
    Hp_Endure_Dmg(defer, hpdmg)
end

function SkillSp_HYMH(sklv)
    local sp_reduce = 50 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_HYMH(sklv)
    local cooldown = 2000
    return cooldown
end

function Skill_HYMH_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_HYMH(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_HYMH_End(atker, defer, sklv)
    local cha_type = GetChaTypeID(defer)
    local statelv = sklv
    local statetime = 6 + sklv * 1
    if cha_type == 1 or cha_type == 2 then
        AddState(atker, defer, STATE_HYMH, statelv, statetime)
        Check_Ys_Rem(atker, defer)
    end
end

function State_HYMH_Add(role, statelv)
end

function State_HYMH_Rem(role, statelv)
end

function SkillSp_HDSMF(sklv)
    local sp_reduce = 30 + (sklv * 2)
    return sp_reduce
end

function SkillCooldown_HDSMF(sklv)
    local cooldown = 3000
    return cooldown
end

function Skill_HDSMF_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_HDSMF(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_HDSMF_End(atker, defer, sklv)
    local Sta_role = Sta(defer)
    hpdmg = math.max(1, math.max(30, (150 - Sta_role)) * 4)
    Hp_Endure_Dmg(defer, hpdmg)
end

function Skill_Huoqiang_Begin(role, sklv)
end

function Skill_Huoqiang_End(ATKER, DEFER, sklv)
    local js_dmg = 1
    dmg = Fire_Dmg(ATKER, DEFER) * js_dmg

    sus, dmgsa = Check_MisorCrt(ATKER, DEFER)

    SetSus(DEFER, sus)
    hpdmg = math.floor(dmg * dmgsa)

    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillSp_Tsqy(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Tsqy(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Tsqy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Tsqy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Tsqy_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 5 + sklv * 2

    AddState(ATKER, DEFER, STATE_TSQY, statelv, statetime)
end

function State_Tsqy_Add(role, statelv)
    local hrecsa_dif = 0.03 * statelv
    local hrecsa = (HrecSa(role) + hrecsa_dif) * ATTR_RADIX

    SetCharaAttr(hrecsa, role, ATTR_STATEC_HREC)
    ALLExAttrSet(role)
end

function State_Tsqy_Rem(role, statelv)
    local hrecsa_dif = 0.03 * statelv
    local hrecsa = (HrecSa(role) - hrecsa_dif) * ATTR_RADIX
    if hrecsa < 0 then
        return
    end

    SetCharaAttr(hrecsa, role, ATTR_STATEC_HREC)
    ALLExAttrSet(role)
end

function SkillSp_Jd(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Jd(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Jd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Jd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Jd_End(ATKER, DEFER, sklv)
    local jd_statelv = sklv
    local zd_statelv = GetChaStateLv(DEFER, STATE_ZD)
    RemoveState(DEFER, STATE_ZD)

    Check_Ys_Rem(ATKER, DEFER)
end

function SkillSp_Zjcm(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Zjcm(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Zjcm_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Zjcm(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Zjcm_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 5 + sklv * 2
    local zjcm_rad = 0.3 + sklv * 0.05
    local atk_dire = GetObjDire(ATKER)
    local def_dire = GetObjDire(DEFER)
    dif_dire = atk_dire - def_dire
    if math.abs(dif_dire) < 90 or math.abs(dif_dire) > 180 then
        zjcm_rad = xy_rad * 1.25
    end
    a = Percentage_Random(zjcm_rad)
    if a == 1 then
        AddState(ATKER, DEFER, STATE_SM, statelv, statetime)
    end

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Sm_Add(role, statelv)
end

function State_Sm_Rem(role, statelv)
end

function SkillSp_Bshd(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Bshd(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Bshd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Bshd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Bshd_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 5 + sklv * 2
    AddState(ATKER, DEFER, STATE_BSHD, statelv, statetime)
end

function State_Bshd_Add(role, statelv)
    local defsb_dif = 5 + statelv * 2
    local defsb = DefSb(role) + defsb_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Bshd_Rem(role, statelv)
    local defsb_dif = 5 + statelv * 2
    local defsb = DefSb(role) - defsb_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_Lyzy(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Lyzy(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Lyzy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Lyzy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Lyzy_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10 + sklv * 2
    AddState(ATKER, DEFER, STATE_LYZY, statelv, statetime)
end

function SkillSp_Shzg(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Shzg(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Shzg_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Shzg(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Shzg_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10 + sklv * 2
    AddState(ATKER, DEFER, STATE_SHZG, statelv, statetime)
end

function SkillSp_Clcy(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function SkillCooldown_Clcy(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Clcy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Clcy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Clcy_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 3 + sklv * 2
    AddState(ATKER, DEFER, STATE_CLCY, statelv, statetime)
end

function State_Clcy_Add(role, statelv)
    local mspdsb_dif = 100 + statelv * 10
    local mspdsb = MspdSb(role) + mspdsb_dif
    SetCharaAttr(mspdsb, role, ATTR_STATEV_MSPD)
    ALLExAttrSet(role)
end

function State_Clcy_Rem(role, statelv)
    local mspdsb_dif = 100 + statelv * 10
    local mspdsb = MspdSb(role) - mspdsb_dif
    SetCharaAttr(mspdsb, role, ATTR_STATEV_MSPD)
    ALLExAttrSet(role)
end

function SkillPre_Hyps(sklv)
end

function SkillCooldown_Hyps(sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillArea_Square_Hyps(sklv)
    local side = 250
    local angle = 90
    SetSkillRange(2, side, angle)
end

function SkillArea_State_Hyps(sklv)
    local statetime = 10 + sklv * 5
    local statelv = sklv

    SetRangeState(STATE_RS, statelv, statetime)
end

function SkillSp_Hyps(sklv)
    local sp_reduce = sklv * 1
    return sp_reduce
end

function Skill_Hyps_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hyps(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hyps_End(ATKER, DEFER, sklv)
    local hpdmg = sklv * 100
    local hp = GetChaAttr(DEFER)
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function State_Hyps_Add(role, statelv)
    local arealv = GetAreaStateLevel(role, STATE_HYPS)
    local hp = GetChaAttr(role, ATTR_HP)
    local hpdmg = statelv * 10
    if arealv >= 1 then
        hpdmg = statelv * 50
    end
    Hp_Endure_Dmg(role, hpdmg)
end

function State_Hyps_Rem(role, statelv)
end

function State_Hyps_Tran(statelv)
    return 2
end

function SkillSp_Ks(SkillLevel)
    return 0
end
function SkillCooldown_Ks(SkillLevel)
    local Cooldown = 1500
    return Cooldown
end
function Skill_Ks_Begin(role, SkillLevel)
end
function Skill_Ks_End(ATKER, DEFER, SkillLevel)
    SystemNotice(ATKER, "Woodcutting ...")
    if SkillLevel < GetChaAttr(DEFER, ATTR_LV) then
        SystemNotice(ATKER, "Your [Woodcutting] is too low!")
        return
    end
    local Points = 0
    local Min = math.random(1, SkillLevel)
    local Max = math.random(SkillLevel, (GetChaAttr(DEFER, ATTR_LV) * SkillLevel))
    Points = math.random(Min, Max)
    local Damage = 1
    for i = 1, #Resources.Uncommon.Tree, 1 do
        if GetChaTypeID(DEFER) == Resources.Uncommon.Tree[i] then
            Points = 1
            if Hp(DEFER) <= 750 then
                Damage = 0
                SystemNotice(ATKER, "Seems that nothing will come out anymore. Time to let the money tree rest before it really falls.")
            end
        end
    end

    local HP = Hp(DEFER) - Damage
    SetCharaAttr(HP, DEFER, ATTR_HP)
    LifeSkillPoint(ATKER, Points)
end
function SkillSp_Wk(SkillLevel)
    return 0
end
function SkillCooldown_Wk(SkillLevel)
    local Cooldown = 1500
    return Cooldown
end
function Skill_Wk_Begin(role, SkillLevel)
end
function Skill_Wk_End(ATKER, DEFER, SkillLevel)
    if SkillLevel < GetChaAttr(DEFER, ATTR_LV) then
        SystemNotice(ATKER, "Your [Mining] is too low!")
        return
    end
    SystemNotice(ATKER, "Mining ...")
    local Points = 0
    local Min = math.random(1, SkillLevel)
    local Max = math.random(SkillLevel, (GetChaAttr(DEFER, ATTR_LV) * SkillLevel))
    Points = math.random(Min, Max)
    local Damage = 1
    local Weapon = GetItemID(GetChaItem(ATKER, 1, 9))
    for i = 1, #Resources.Uncommon.Rock, 1 do
        if GetChaTypeID(DEFER) == Resources.Uncommon.Rock[i] then
            if Weapon ~= 3908 and Weapon ~= 3108 then
                SystemNotice(ATKER, "Only Alloy Pickaxe can be used to mine.")
                return
            end
            Points = 1
            if Hp(DEFER) <= 800 then
                Damage = 0
                SystemNotice(ATKER, "Looks like the Meteorite is exhausted. Let it have some rest.")
            end
            if GetItemAttr(GetChaItem(ATKER, 1, 9), ITEMATTR_URE) < 50 then
                SystemNotice(ATKER, "Pickaxe is damaged. Unable to continue using.")
                SetChaEquipValid(ATKER, 9, 0)
                return
            end
            if math.random(1, 100) <= 100 then
                local Durability = GetItemAttr(GetChaItem(ATKER, 1, 9), ITEMATTR_URE) - 1
                SetItemAttr(GetChaItem(ATKER, 1, 9), ITEMATTR_URE, Durability)
            end
        end
    end
    local HP = Hp(DEFER) - Damage
    SetCharaAttr(HP, DEFER, ATTR_HP)
    LifeSkillPoint(ATKER, Points)
end
function SkillSp_By(SkillLevel)
    return 0
end

function SkillCooldown_By(SkillLevel)
    local Cooldown = 1500
    return Cooldown
end

function Skill_By_Begin(role, SkillLevel)
end

function Skill_By_End(ATKER, DEFER, SkillLevel)
    if SkillLevel < GetChaAttr(DEFER, ATTR_LV) then
        SystemNotice(ATKER, "Your [Fishing] is too low!")
        return
    end
    SystemNotice(ATKER, "Fishing ...")
    local Points = 0
    local Min = math.random(1, SkillLevel)
    local Max = math.random(SkillLevel, (GetChaAttr(DEFER, ATTR_LV) * SkillLevel))
    Points = math.random(Min, Max)
    local Damage = 1
    for i = 1, #Resources.Uncommon.Fish, 1 do
        if GetChaTypeID(DEFER) == Resources.Uncommon.Fish[i] then
            Points = 1
            if Hp(DEFER) <= 750 then
                Damage = 0
                SystemNotice(
                    ATKER,
                    "Seems that nothing will come out anymore. Time to let the fish rest so they can group again."
                )
            end
        end
    end
    local HP = Hp(DEFER) - Damage
    SetCharaAttr(HP, DEFER, ATTR_HP)
    LifeSkillPoint(ATKER, Points)
end

function SkillSp_Dl(SkillLevel)
    return 0
end

function SkillCooldown_Dl(SkillLevel)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Dl_Begin(role, SkillLevel)
end

function Skill_Dl_End(ATKER, DEFER, SkillLevel)
    if SkillLevel < GetChaAttr(DEFER, ATTR_LV) then
        SystemNotice(ATKER, "Your [Salvage] is too low!")
        return
    end
    SystemNotice(ATKER, "Salvaging ...")

    local Points = 0
    local Min = math.random(1, SkillLevel)
    local Max = math.random(SkillLevel, (GetChaAttr(DEFER, ATTR_LV) * SkillLevel))
    Points = math.random(Min, Max)
    local Damage = 1
    for i = 1, #Resources.Uncommon.Boat, 1 do
        if GetChaTypeID(DEFER) == Resources.Uncommon.Boat[i] then
            Points = 1
            if Hp(DEFER) <= 750 then
                Damage = 0
                SystemNotice(
                    ATKER,
                    "Seems that nothing will come out anymore. Leave that boat alone before it breaks!."
                )
            end
        end
    end
    local HP = Hp(DEFER) - Damage
    SetCharaAttr(HP, DEFER, ATTR_HP)
    LifeSkillPoint(ATKER, Points)
end
function LifeSkillPoint(Player, Points)
	local LifeEXP = GetChaAttr(Player, ATTR_CLIFEEXP)
	SetCharaAttr((LifeEXP + Points), Player, ATTR_CLIFEEXP)
	ALLExAttrSet(Player)
end

function SkillSp_Yy(sklv)
    local sp_reduce = 10
    return sp_reduce
end

function SkillCooldown_Yy(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Yy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Yy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Yy_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 20 + sklv * 10
    AddState(ATKER, DEFER, STATE_YY, statelv, statetime)
end

function State_Yy_Add(role, statelv)
    local hitsb_dif = statelv * 3
    local hitsb = HitSb(role) + hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function State_Yy_Rem(role, statelv)
    local hitsb_dif = statelv * 3
    local hitsb = HitSb(role) - hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function SkillSp_Hxqj(sklv)
    local sp_reduce = 20 + sklv * 2
    return sp_reduce
end

function SkillCooldown_Hxqj(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Hxqj_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hxqj(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hxqj_End(ATKER, DEFER, sklv)
    local back_dis = 300 + sklv * 30
    atk = (1.5 + sklv * 0.1) * math.random(Mnatk(ATKER), Mxatk(ATKER))
    defer_def = Def(DEFER)
    defer_resist = Resist(DEFER)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    Hp_Endure_Dmg(DEFER, dmg)
    BeatBack(ATKER, DEFER, back_dis)
end

function SkillCooldown_TestLimit(SkillLevel)
    local Cooldown = 5000
    return Cooldown
end
function Skill_TestLimit_Begin(role, SkillLevel)
    local sp = Sp(role)
    local sp_reduce = TestLimit(SkillLevel)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end
function Skill_TestLimit_End(ATKER, DEFER, SkillLevel)
    local statelv = SkillLevel
    local statetime = 30 + SkillLevel * 3
    local map_name_ATKER = GetChaMapName(ATKER)
    local map_name_DEFER = GetChaMapName(DEFER)
    local sta_atker = Sta(ATKER)
    local Can_Pk_Garner2 = Is_NormalMonster(ATKER)
    if
        map_name_ATKER == "garner2" or map_name_DEFER == "garner2" or map_name_ATKER == "starena1" or
            map_name_DEFER == "starena1" or
            map_name_ATKER == "starena2" or
            map_name_DEFER == "starena2" or
            map_name_ATKER == "starena3" or
            map_name_DEFER == "starena3"
     then
        if Can_Pk_Garner2 == 0 then
            statetime = math.max(30, math.floor(sta_atker / 5)) + SkillLevel * 3
        end
    end
    if GetChaTypeID(ATKER) == 984 then
        statetime = 360
        statelv = 10
    end
    AddState(ATKER, DEFER, 37, statelv, statetime)
end
function State_TestLimit_Add(role, statelv)
    local defsb = DefSb(role) + 50
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
    RefreshCha(role)
end
function State_TestLimit_Rem(role, statelv)
    local defsb = DefSb(role) - 50
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
    RefreshCha(role)
end

function Cuihua_Mofa(dmg, statelv)
    local dmg_fin = math.floor(dmg * (1.4 + statelv * 0.02) + statelv * 30)
    return dmg_fin
end

function SkillSp_Bc(sklv)
    local sp_reduce = 15 + sklv * 2
    return sp_reduce
end

function SkillCooldown_Bc(sklv)
    local Cooldown = 60000
    return Cooldown
end

function Skill_Bc_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Bc(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Bc_End(ATKER, DEFER, sklv)
    local atk_rad = 1 + sklv * 0.05
    local atk_dire = GetObjDire(ATKER)
    local def_dire = GetObjDire(DEFER)
    dif_dire = atk_dire - def_dire
    if math.abs(dif_dire) < 90 or math.abs(dif_dire) > 180 then
        hpdmg = MxatkIb(role) * sklv * 0.3
    else
        hpdmg = Atk_Raise(atk_rad, ATKER, DEFER)
    end
    Hp_Endure_Dmg(DEFER, hpdmg)
    Check_Ys_Rem(ATKER, DEFER)
end

function SkillCooldown_Mb(sklv)
    local Cooldown = 20000
    return Cooldown
end

function State_Mb_Add(role, statelv)
    local aspdsa_dif = (-1) * (0.1 + statelv * 0.03)
    local mspdsa_dif = (-1) * (0.2 + statelv * 0.03)

    local aspdsa = math.floor((AspdSa(role) + aspdsa_dif) * ATTR_RADIX)
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)

    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_Mb_Rem(role, statelv)
    local aspdsa_dif = (-1) * (0.1 + statelv * 0.03)
    local mspdsa_dif = (-1) * (0.2 + statelv * 0.03)
    local aspdsa = math.floor((AspdSa(role) - aspdsa_dif) * ATTR_RADIX)
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillSp_Ldc(sklv)
    local sp_reduce = 10
    return sp_reduce
end

function SkillCooldown_Ldc(sklv)
    local Cooldown = 30000
    return Cooldown
end

function Skill_Ldc_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Ldc(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Ldc_End(ATKER, DEFER, sklv)
    if ValidCha(ATKER) == 0 then
        LG("luascript_err", "function Skill_Ldc_End : ATKER as null")
        return
    end
    if ValidCha(DEFER) == 0 then
        LG("luascript_err", "function Skill_Ldc_End : DEFER as null")
        return
    end
    dmg = (2 + sklv * 0.2) * Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, dmg)
end

function Skill_Dpsl_Use(role, sklv)
    local statelv = sklv
    local defsb_dif = 3 * statelv
    local resistsb_dif = 1 * statelv
    local mspdsa_dif = (-1) * 0.02 * statelv
    local defsb = DefSb(role) + defsb_dif
    local resistsb = ResistSb(role) + resistsb_dif
    local mspdsa = MspdSa(role) + mspdsa_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(resistsb, role, ATTR_STATEV_PDEF)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function Skill_Dpsl_Unuse(role, sklv)
    local statelv = sklv
    local defsb_dif = 3 * statelv
    local resistsb_dif = 1 * statelv
    local mspdsa_dif = (-1) * 0.02 * statelv
    local defsb = DefSb(role) - defsb_dif
    local resistsb = ResistSb(role) - resistsb_dif
    local mspdsa = MspdSa(role) - mspdsa_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(resistsb, role, ATTR_STATEV_PDEF)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function Skill_Shtz_Use(role, sklv)
    local statelv = sklv
    local mxhpsb_dif = 50 * statelv
    local mxhpsb = MxhpSb(role) + mxhpsb_dif
    SetCharaAttr(mxhpsb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function Skill_Shtz_Unuse(role, sklv)
    local statelv = sklv
    local mxhpsb_dif = 50 * statelv
    local mxhpsb = MxhpSb(role) - mxhpsb_dif
    SetCharaAttr(mxhpsb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function Skill_Hys_Use(role, sklv)
    local statelv = sklv
    local fleesb_dif = 4 * statelv
    local fleesb = math.floor((FleeSb(role) + fleesb_dif))
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Hys_Unuse(role, sklv)
    local statelv = sklv
    local fleesb_dif = 4 * statelv
    local fleesb = math.floor((FleeSb(role) - fleesb_dif))
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Tzhf_Use(role, sklv)
    local statelv = sklv
    local hrecsb_dif = statelv
    local hrecsb = math.floor((HrecSb(role) + hrecsb_dif) * ATTR_RADIX)
    SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)
    ALLExAttrSet(role)
end

function Skill_Tzhf_Unuse(role, sklv)
    local statelv = sklv
    local hrecsb_dif = statelv
    local hrecsb = math.floor((HrecSb(role) - hrecsb_dif) * ATTR_RADIX)
    SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)
    ALLExAttrSet(role)
end

function Skill_Zjft_Use(role, sklv)
    local statelv = sklv
    AddState(role, role, STATE_ZJFT, statelv, -1)
end

function Skill_Zjft_Unuse(role, sklv)
    local statelv = sklv
    RemoveState(role, STATE_ZJFT)
end

function Skill_Hpsl_Use(role, sklv)
    local statelv = sklv
    local ship_mxatk_dif = statelv * 25
    local ship_mnatk_dif = statelv * 25
    local ship_mxatk = (Ship_Mxatk(role) + ship_mxatk_dif)
    local ship_mnatk = (Ship_Mnatk(role) + ship_mnatk_dif)
    SetCharaAttr(ship_mxatk, role, ATTR_BOAT_SKILLV_MXATK)
    SetCharaAttr(ship_mnatk, role, ATTR_BOAT_SKILLV_MNATK)
    ALLExAttrSet(role)
end

function Skill_Hpsl_Unuse(role, sklv)
    local statelv = sklv
    local ship_mxatk_dif = statelv * 25
    local ship_mnatk_dif = statelv * 25
    local ship_mxatk = (Ship_Mxatk(role) - ship_mxatk_dif)
    local ship_mnatk = (Ship_Mnatk(role) - ship_mnatk_dif)
    SetCharaAttr(ship_mxatk, role, ATTR_BOAT_SKILLV_MXATK)
    SetCharaAttr(ship_mnatk, role, ATTR_BOAT_SKILLV_MNATK)
    ALLExAttrSet(role)
end

function Skill_Jbjg_Use(role, sklv)
    LG("skill_Jbjg", "enter function Skill_Oper_Jbjg:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_def_dif = statelv * 12
    local ship_def = (Ship_Def(role) + ship_def_dif)
    SetCharaAttr(ship_def, role, ATTR_BOAT_SKILLV_DEF)
    ALLExAttrSet(role)
end

function Skill_Jbjg_Unuse(role, sklv)
    LG("skill_Jbjg", "enter function Skill_Oper_Jbjg:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_def_dif = statelv * 12
    local ship_def = (Ship_Def(role) - ship_def_dif)
    SetCharaAttr(ship_def, role, ATTR_BOAT_SKILLV_DEF)
    ALLExAttrSet(role)
end

function Skill_Cfs_Use(role, sklv)
    LG("skill_Cfs", "enter function Skill_Oper_Cfs:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_aspdsa_dif = 0.1 + statelv * 0.02
    local ship_aspdsa = (Ship_AspdSa(role) + ship_aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(ship_aspdsa, role, ATTR_BOAT_SKILLC_ASPD)
    ALLExAttrSet(role)
end

function Skill_Cfs_Unuse(role, sklv)
    LG("skill_Cfs", "enter function Skill_Oper_Cfs:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_aspdsa_dif = 0.1 + statelv * 0.02
    local ship_aspdsa = (Ship_AspdSa(role) - ship_aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(ship_aspdsa, role, ATTR_BOAT_SKILLC_ASPD)
    ALLExAttrSet(role)
end

function Skill_Ctqh_Use(role, sklv)
    LG("skill_Ctqh", "enter function Skill_Oper_Ctqh:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_hp_dif = statelv * 400
    local ship_hp = (Ship_Mxhp(role) + ship_hp_dif)
    SetCharaAttr(ship_hp, role, ATTR_BOAT_SKILLV_MXUSE)
    ALLExAttrSet(role)
end

function Skill_Ctqh_Unuse(role, sklv)
    LG("skill_Ctqh", "enter function Skill_Oper_Ctqh:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_hp_dif = statelv * 400
    local ship_hp = (Ship_Mxhp(role) - ship_hp_dif)
    SetCharaAttr(ship_hp, role, ATTR_BOAT_SKILLV_MXUSE)
    ALLExAttrSet(role)
end

function Skill_Bjcr_Use(role, sklv)
    LG("skill_Bjcr", "enter function Skill_Oper_Bjcr:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_sp_dif = statelv * 30
    local ship_sp = (Ship_Mxsp(role) + ship_sp_dif)
    SetCharaAttr(ship_sp, role, ATTR_BOAT_SKILLV_MXSPLY)
    ALLExAttrSet(role)
end

function Skill_Bjcr_Unuse(role, sklv)
    LG("skill_Bjcr", "enter function Skill_Oper_Bjcr:", "sklv = ", sklv, "role = ", role, "\n")
    local statelv = sklv
    local ship_sp_dif = statelv * 30
    local ship_sp = (Ship_Mxsp(role) - ship_sp_dif)
    SetCharaAttr(ship_sp, role, ATTR_BOAT_SKILLV_MXSPLY)
    ALLExAttrSet(role)
end

function Skill_Clxz_Use(role, sklv)
    local statelv = sklv
    local fleesb_dif = 3 * statelv
    local fleesb = FleeSb(role) + fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Clxz_Unuse(role, sklv)
    local statelv = sklv
    local fleesb_dif = 3 * statelv
    local fleesb = FleeSb(role) - fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Lrwz_Use(role, sklv)
    local statelv = sklv
    local fleesb_dif = 2 * statelv
    local fleesb = FleeSb(role) + fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Lrwz_Unuse(role, sklv)
    local statelv = sklv
    local fleesb_dif = 2 * statelv
    local fleesb = FleeSb(role) - fleesb_dif
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function Skill_Zx_Use(role, sklv)
    local ys_statelv = GetChaStateLv(role, STATE_YS)
    if ys_statelv >= 1 then
        RemoveState(role, STATE_YS)
    end
    local hrecsb_dif = 15
    local hrecsa_dif = 5
    local hrecsa = math.floor((HrecSa(role) + hrecsa_dif) * ATTR_RADIX)
    local hrecsb = math.floor((HrecSb(role) + hrecsb_dif))

    SetCharaAttr(hrecsa, role, ATTR_STATEC_HREC)
    SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)

    local srecsb_dif = 5
    local srecsa_dif = 5
    local srecsa = math.floor((SrecSa(role) + srecsa_dif) * ATTR_RADIX)
    local srecsb = math.floor((SrecSb(role) + srecsb_dif))

    SetCharaAttr(srecsa, role, ATTR_STATEC_SREC)
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    Check_Ys_Rem(role, role)
    ALLExAttrSet(role)
end

function Skill_Zx_Unuse(role, sklv)
    local hrecsb_dif = 15
    local hrecsa_dif = 5
    local hrecsa = math.floor((HrecSa(role) - hrecsa_dif) * ATTR_RADIX)
    local hrecsb = math.floor((HrecSb(role) - hrecsb_dif))

    SetCharaAttr(hrecsa, role, ATTR_STATEC_HREC)
    SetCharaAttr(hrecsb, role, ATTR_STATEV_HREC)

    local srecsb_dif = 5
    local srecsa_dif = 5
    local srecsa = math.floor((SrecSa(role) - srecsa_dif) * ATTR_RADIX)
    local srecsb = math.floor((SrecSb(role) - srecsb_dif))

    SetCharaAttr(srecsa, role, ATTR_STATEC_SREC)
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)

    ALLExAttrSet(role)
end

function SkillArea_Sector_Hxdj(sklv)
    local radius = 400 + math.floor(sklv * 10)
    local angle = 100 + math.floor(sklv / 5) * 20
    SetSkillRange(2, radius, angle)
end

function SkillCooldown_Hxdj(sklv)
    local Cooldown = 5000
    return Cooldown
end

function SkillPre_Hxdj(sklv)
end

function SkillSp_Hxdj(sklv)
    local sp_reduce = 20 + sklv * 2
    return sp_reduce
end

function Skill_Hxdj_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hxdj(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hxdj_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 5
    local back_dis = 500
    dmg = math.floor(150 + sklv * 20)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_XY, statelv, statetime)
    BeatBack(ATKER, DEFER, back_dis)
end

function SkillArea_Circle_Ymsl(sklv)
    local side = 200 + math.floor(sklv * 10)

    SetSkillRange(4, side)
end

function SkillCooldown_Ymsl(sklv)
    local Cooldown = 5000
    return Cooldown
end

function SkillPre_Ymsl(sklv)
end

function SkillSp_Ymsl(sklv)
    local sp_reduce = 10 + sklv * 1
    return sp_reduce
end

function Skill_Ymsl_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Ymsl(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
end

function Skill_Ymsl_End(ATKER, DEFER, sklv)
    local hp = Hp(DEFER)

    atk_rad = 1.5 + sklv * 0.1
    hpdmg = Atk_Raise(atk_rad, ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillArea_Circle_Dzy(sklv)
    local side = 300 + math.floor(sklv * 30)
    SetSkillRange(4, side)
end

function SkillPre_Dzy(sklv)
end

function SkillCooldown_Dzy(sklv)
    local Cooldown = 10000
    return Cooldown
end

function SkillSp_Dzy(sklv)
    local sp_reduce = 30 + sklv * 1
    return sp_reduce
end

function Skill_Dzy_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Dzy(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Dzy_End(ATKER, DEFER, sklv)
    local dmg = (-1) * math.floor(10 + 15 * sklv + math.floor(Sta(ATKER) * 0.5))
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_Dhfs(sklv)
    local side = 300 + math.floor(sklv * 30)
    SetSkillRange(4, side)
end

function SkillCooldown_Dhfs(sklv)
    local Cooldown = 5000
    return Cooldown
end

function SkillPre_Dhfs(sklv)
end

function SkillSp_Dhfs(sklv)
    local sp_reduce = 15 + math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_Dhfs_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Dhfs(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Dhfs_End(ATKER, DEFER, sklv)
    dmg = (-1) * (5 + sklv * 3)
    Hp_Endure_Dmg(DEFER, dmg)
    Rem_State_Unnormal(DEFER)
end

function SkillSp_Sdbz(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_Sdbz(sklv)
    local Cooldown = 20000
    return Cooldown
end

function SkillArea_Square_Sdbz(sklv)
    local side = 300
    SetSkillRange(3, side)
end

function SkillArea_State_Sdbz(sklv)
    local statetime = 5
    local statelv = sklv

    SetRangeState(STATE_SDBZ, statelv, statetime)
end

function Skill_Sdbz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce
    sp_reduce = SkillSp_Sdbz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end

    Sp_Red(role, sp_reduce)
end

function Skill_Sdbz_End(ATKER, DEFER, sklv)
end

function State_Sdbz_Add(role, statelv)
    local hitsa_dif = (-1) * (0.02 * statelv)
    local hitsa = math.floor((HitSa(role) + hitsa_dif) * ATTR_RADIX)
    SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    ALLExAttrSet(role)
end

function State_Sdbz_Rem(role, statelv)
    local hitsa_dif = (-1) * (0.02 * statelv)
    local hitsa = math.floor((HitSa(role) - hitsa_dif) * ATTR_RADIX)
    SetCharaAttr(hitsa, role, ATTR_STATEC_HIT)
    ALLExAttrSet(role)
end

function State_Sdbz_Tran(statelv)
    local statetime = 30 + sklv * 3
    return statetime
end

function SkillArea_Line_Ctd(sklv)
    local lenth = 1500 + sklv * 50
    local width = 50
    SetSkillRange(1, lenth, width)
end

function SkillCooldown_Ctd(sklv)
    local Cooldown = 5000
    return Cooldown
end

function SkillPre_Ctd(sklv)
end

function SkillSp_Ctd(sklv)
    local sp_reduce = 10 + sklv * 1
    return sp_reduce
end

function Skill_Ctd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Ctd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Ctd_End(ATKER, DEFER, sklv)
    local hp = Hp(DEFER)
    if ValidCha(ATKER) == 0 then
        LG("luascript_err", "function Skill_Ctd_End : ATKER as null")
        return
    end
    if ValidCha(DEFER) == 0 then
        LG("luascript_err", "function Skill_Ctd_End : DEFER as null")
        return
    end
    dmg = (1 + sklv * 0.2) * Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Sector_Ssd(sklv)
    local radius = 600 + math.floor(sklv * 20)
    local angle = 90 + math.floor(sklv / 5) * 15
    SetSkillRange(2, radius, angle)
end

function SkillCooldown_Ssd(sklv)
    local Cooldown = 15000
    return Cooldown
end

function SkillPre_Ssd(sklv)
end

function SkillSp_Ssd(sklv)
    local sp_reduce = sklv * 1 + 15
    return sp_reduce
end

function Skill_Ssd_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Ssd(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)

        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Ssd_End(ATKER, DEFER, sklv)
    atk_rad = 1.2 + sklv * 0.15
    dmg = Atk_Raise(atk_rad, ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_Larea_Tran(statelv)
    return 1
end

function State_Larea_Add(role, statelv)
    LG("LeiQu", " role = ", role, " statelv = ", statelv)
    local role_type = IsPlayer(role)
    if role_type == 0 then
        return
    end
    local hp = Hp(role)
    dmg = 5 + statelv * 1
    Hp_Endure_Dmg(role, dmg)
    local cha_role = TurnToCha(role)
    local a = AddEquipEnergy(cha_role, enumEQUIP_HAND1, 29, 50)
end

function State_Larea_Rem(role, statelv)
end

function State_Warea_Tran(statelv)
    return 1
end

function State_Warea_Add(role, statelv)
    local cha_role = TurnToCha(role)
    local a = AddEquipEnergy(cha_role, enumEQUIP_HAND2, 29, 50)
end

function State_Warea_Rem(role, statelv)
end

function State_Farea_Tran(statelv)
    return 1
end

function State_Farea_Add(role, statelv)
    local cha_role = TurnToCha(role)
    local a = AddEquipEnergy(cha_role, enumEQUIP_NECK, 29, 50)
end

function State_Farea_Rem(role, statelv)
end

function SkillCooldown_Gwptjn(sklv)
    local Cooldown = 2500
    return Cooldown
end

function SkillCooldown_Zcmtl(sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillSp_Fuz(sklv)
    local sp_reduce = 10
    return sp_reduce
end

function SkillCooldown_Fuz(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_Fuz_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Fuz(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Fuz_End(ATKER, DEFER, sklv)
end

function SkillArea_Circle_Hztx(sklv)
    local side = 300
    SetSkillRange(4, side)
end

function SkillCooldown_Hztx(sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillSp_Hztx(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function Skill_Hx_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Hztx(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Hztx_End(ATKER, DEFER, sklv)
    local statelv = 10
    local statetime = 30
    AddState(ATKER, DEFER, STATE_HZCR, statelv, statetime)
end

function SkillArea_Circle_Smdj(sklv)
    local side = 400
    SetSkillRange(3, side)
end

function SkillCooldown_Smdj(sklv)
    local Cooldown = 1000
    return Cooldown
end

function SkillSp_Smdj(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_State_Smdj(sklv)
    local statetime = 25
    local statelv = 10
    SetRangeState(STATE_LM, statelv, statetime)
end

function Skill_Smdj_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Smdj(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Smdj_End(ATKER, DEFER, sklv)
end

function SkillArea_Circle_Wzxf(sklv)
    local side = 400
    SetSkillRange(4, side)
end

function SkillCooldown_Wzxf(sklv)
    local Cooldown = 1000
    return Cooldown
end

function SkillPre_Wzxf(sklv)
end

function SkillSp_Wzxf(sklv)
    local sp_reduce = 20 - math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_Wzxf_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Wzxf(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Wzxf_End(ATKER, DEFER, sklv)
    local hp = Hp(DEFER)
    dmg = 3 * Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillSp_Syzm(sklv)
    local sp_reduce = sklv * 2 + 30
    return sp_reduce
end

function SkillCooldown_Syzm(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_Syzm_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Syzm(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Syzm_End(ATKER, DEFER, sklv)
    local Mxhp = Mxhp(DEFER)
    local dmg = math.floor(Mxhp / 2)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_Slzb_Add(role, statelv)
    local hp = Hp(role)
    if hp <= 0 then
        RemoveState(role, STATE_BOMB)
    end
end

function State_Slzb_Rem(role, statelv)
    local x, y = GetChaPos(role)
    ChaUseSkill2(role, SK_BOMB, 1, x, y)
    Notice("after use skill")
    DelCha(role)
    Notice("after delcha")
end

function SkillArea_Circle_Slzb(sklv)
    local side = 1200 + math.floor(sklv * 20)

    SetSkillRange(4, side)
end

function SkillCooldown_Slzb(sklv)
    local Cooldown = 1000
    return Cooldown
end

function SkillPre_Slzb(sklv)
end

function SkillSp_Slzb(sklv)
    local sp_reduce = 0
    return sp_reduce
end

function Skill_Slzb_Begin(role, sklv)
end

function Skill_Slzb_End(ATKER, DEFER, sklv)
    local atker_type = GetChaTypeID(ATKER)
    local defer_type = GetChaTypeID(DEFER)

    local hp = Hp(DEFER)
    Notice("defer_hp = " .. hp)
    if ValidCha(ATKER) == 0 then
        LG("luascript_err", "function Skill_Slzb_End : ATKER as null")
        return
    end
    if ValidCha(DEFER) == 0 then
        LG("luascript_err", "function Skill_Slzb_End : DEFER as null")
        return
    end
    dmg = 1500
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(-1, ATKER, ATTR_HP)
end

function State_PKDYK_Add(role, statelv)
    local mnatksa_dif = -0.8
    local mxatksa_dif = -0.8
    local mnatksa = math.floor((MnatkSa(role) + mnatksa_dif) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) + mxatksa_dif) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_PKDYK_Rem(role, statelv)
    local mnatksa_dif = -0.8
    local mxatksa_dif = -0.8
    local mnatksa = math.floor((MnatkSa(role) - mnatksa_dif) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) - mxatksa_dif) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    ALLExAttrSet(role)
end

function State_PKLC_Add(role, statelv)
    local def_dif = -200
    local def = DefSb(role) + def_dif
    local Res_sa = ResistSa(role)
    local Res_sa_dif = -0.5
    local Res = Res_sa + Res_sa_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    SetCharaAttr(Res, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
end

function State_PKLC_Rem(role, statelv)
    local def_dif = -200
    local def = DefSb(role) - def_dif
    local Res_sa = ResistSa(role)
    local Res_sa_dif = -0.5
    local Res = Res_sa - Res_sa_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    SetCharaAttr(Res, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
    ALLExAttrSet(role)
end

function SkillSp_PKXL(sklv)
    return 0
end

function SkillCooldown_PKXL(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PKXL_Begin(role, sklv)
end

function Skill_PKXL_End(ATKER, DEFER, sklv)
    local i = CheckBagItem(ATKER, 4661)
    if i <= 0 then
        SystemNotice(ATKER, "Does not have wood to repairwhat do you use?")
        return
    end
    local j = DelBagItem(ATKER, 4661, 1)
    if j == 1 then
        SystemNotice(ATKER, "Repairing")
        local hpdmg = 200 + sklv * 20
        local hp = Hp(DEFER) + hpdmg
        SetCharaAttr(hp, DEFER, ATTR_HP)
    else
        LG("PK_repair", "Delete Wood failed")
    end
end

function State_PKMNYS_Add(role, statelv)
    local MxhpSb_dif = 1000
    local MxhpSb = MxhpSb(role) + MxhpSb_dif
    SetCharaAttr(MxhpSb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function State_PKMNYS_Rem(role, statelv)
    local MxhpSb_dif = 1000
    local MxhpSb = MxhpSb(role) - MxhpSb_dif
    SetCharaAttr(MxhpSb, role, ATTR_STATEV_MXHP)
    ALLExAttrSet(role)
end

function State_PKZDYS_Add(role, statelv)
    local atksb_dif = 150
    if statelv == 1 then
        atksb_dif = 30
    end
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PKZDYS_Rem(role, statelv)
    local atksb_dif = 150
    if statelv == 1 then
        atksb_dif = 30
    end
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_KUANGZ_Add(role, statelv)
    local atksb_dif = 50
    local def_dif = 25
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    local defsb = DefSb(role) - def_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_KUANGZ_Rem(role, statelv)
    local atksb_dif = 50
    local def_dif = 25
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    local defsb = DefSb(role) + def_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_PKKBYS_Add(role, statelv)
    local aspd_dif = 140
    local aspdsb = (AspdSb(role) + aspd_dif)
    SetCharaAttr(aspdsb, role, ATTR_STATEV_ASPD)
    ALLExAttrSet(role)
end

function State_PKKBYS_Rem(role, statelv)
    local aspd_dif = 140
    local aspdsb = (AspdSb(role) - aspd_dif)
    SetCharaAttr(aspdsb, role, ATTR_STATEV_ASPD)
    ALLExAttrSet(role)
end

function State_PKJSYS_Add(role, statelv)
    local sta_dif = 30
    local stasb = StaSb(role) + sta_dif
    SetCharaAttr(stasb, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function State_PKJSYS_Rem(role, statelv)
    local sta_dif = 30
    local stasb = StaSb(role) - sta_dif
    SetCharaAttr(stasb, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function State_PKSFYS_Add(role, statelv)
    local def_dif = 150
    if statelv == 1 then
        def_dif = 30
    end
    local defsb = DefSb(role) + def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_PKSFYS_Rem(role, statelv)
    local def_dif = 150
    if statelv == 1 then
        def_dif = 30
    end
    local defsb = DefSb(role) - def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_QUANS_Add(role, statelv)
    local def_dif = 70
    local atksb_dif = 30
    local defsb = DefSb(role) + def_dif
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_QUANS_Rem(role, statelv)
    local def_dif = 70
    local atksb_dif = 30
    local defsb = DefSb(role) - def_dif
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PKJZYS_Add(role, statelv)
    local hit_dif = 30
    local hitsb = HitSb(role) + hit_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function State_PKJZYS_Rem(role, statelv)
    local hit_dif = 30
    local hitsb = HitSb(role) - hit_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function State_PKSBYS_Add(role, statelv)
    local Flee_dif = 10
    if statelv == 1 then
        Flee_dif = 10
    end
    local Flee = FleeSb(role) + Flee_dif
    SetCharaAttr(Flee, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function State_PKSBYS_Rem(role, statelv)
    local Flee_dif = 10
    if statelv == 1 then
        Flee_dif = 10
    end
    local Flee = FleeSb(role) - Flee_dif
    SetCharaAttr(Flee, role, ATTR_STATEV_FLEE)
    ALLExAttrSet(role)
end

function SkillCooldown_Wudiyaoshui(sklv)
    local Cooldown = 20000
    return Cooldown
end
function Skill_Wudiyaoshui_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 1860)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1860, 1)
end

function Skill_Wudiyaoshui_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local statelv = 10
    local statetime = 5

    AddState(ATKER, DEFER, STATE_PKWD, statelv, statetime)
	
    local Message = GetChaDefaultName(ATKER).." entering invincible mode for 5 secs"
    Notice(Message)
end

function State_PKWd_Add(role, statelv)
end

function State_PKWd_Rem(role, statelv)
end

function State_YSLLQH_Add(role, statelv)
    local str_dif = 5
    local strsb = StrSb(role) + str_dif
    SetCharaAttr(strsb, role, ATTR_STATEV_STR)
    ALLExAttrSet(role)
end

function State_YSLLQH_Rem(role, statelv)
    local str_dif = 5
    local strsb = StrSb(role) - str_dif
    SetCharaAttr(strsb, role, ATTR_STATEV_STR)
    ALLExAttrSet(role)
end

function State_YSMJQH_Add(role, statelv)
    local agi_dif = 5
    local agisb = AgiSb(role) + agi_dif
    SetCharaAttr(agisb, role, ATTR_STATEV_AGI)
    ALLExAttrSet(role)
end

function State_YSMJQH_Rem(role, statelv)
    local agi_dif = 5
    local agisb = AgiSb(role) - agi_dif
    SetCharaAttr(agisb, role, ATTR_STATEV_AGI)
    ALLExAttrSet(role)
end

function State_YSLQQH_Add(role, statelv)
    local dex_dif = 5
    local dexsb = DexSb(role) + dex_dif
    SetCharaAttr(dexsb, role, ATTR_STATEV_DEX)
    ALLExAttrSet(role)
end

function State_YSLQQH_Rem(role, statelv)
    local dex_dif = 5
    local dexsb = DexSb(role) - dex_dif
    SetCharaAttr(dexsb, role, ATTR_STATEV_DEX)
    ALLExAttrSet(role)
end

function State_YSTZQH_Add(role, statelv)
    local con_dif = 5
    local consb = ConSb(role) + con_dif
    SetCharaAttr(consb, role, ATTR_STATEV_CON)
    ALLExAttrSet(role)
end

function State_YSTZQH_Rem(role, statelv)
    local con_dif = 5
    local consb = ConSb(role) - con_dif
    SetCharaAttr(consb, role, ATTR_STATEV_CON)
    ALLExAttrSet(role)
end

function State_YSJSQH_Add(role, statelv)
    local sta_dif = 5
    local stasb = StaSb(role) + sta_dif
    SetCharaAttr(stasb, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function State_YSJSQH_Rem(role, statelv)
    local sta_dif = 5
    local stasb = StaSb(role) - sta_dif
    SetCharaAttr(stasb, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function State_YSMspd_Add(role, statelv)
    local mspdsa_dif = 0
    if statelv == 1 then
        mspdsa_dif = 0.15
    end
    local mspdsa = MspdSa(role)
    local mspdsa_fin = (mspdsa + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa_fin, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_YSMspd_Rem(role, statelv)
    local mspdsa_dif = 0
    if statelv == 1 then
        mspdsa_dif = 0.15
    end
    local mspdsa = MspdSa(role)
    local mspdsa_fin = (mspdsa - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa_fin, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_QINGZ_Add(role, statelv)
    local mspdsa_dif = 0.3
    local def_dif = 50
    local mspdsa = MspdSa(role)
    local mspdsa_fin = (mspdsa + mspdsa_dif) * ATTR_RADIX
    local defsb = DefSb(role) - def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(mspdsa_fin, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_QINGZ_Rem(role, statelv)
    local mspdsa_dif = 0.3
    local def_dif = 50
    local mspdsa = MspdSa(role)
    local mspdsa_fin = (mspdsa - mspdsa_dif) * ATTR_RADIX
    local defsb = DefSb(role) + def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    SetCharaAttr(mspdsa_fin, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_YSBoatMspd_Add(role, statelv)
    local mspdsa_dif = 0
    if statelv == 1 then
        mspdsa_dif = 0.15
    end
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_YSBoatMspd_Rem(role, statelv)
    local mspdsa_dif = 0
    if statelv == 1 then
        mspdsa_dif = 0.15
    end
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_YSBoatDEF_Add(role, statelv)
    local defsb_dif = 0
    if statelv == 1 then
        defsb_dif = 200
    end
    local defsb = (DefSb(role) + defsb_dif)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_YSBoatDEF_Rem(role, statelv)
    local defsb_dif = 0
    if statelv == 1 then
        defsb_dif = 200
    end
    local defsb = (DefSb(role) - defsb_dif)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_DengLong_Add(role, statelv)
    local def_dif = 50
    local defsb = DefSb(role) + def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_DengLong_Rem(role, statelv)
    local def_dif = 50
    local defsb = DefSb(role) - def_dif
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillSp_TZJSMagic(sklv)
    local sp_reduce = 10 + sklv * 2
    return sp_reduce
end

function SkillCooldown_TZJSMagic(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_TZJSMagic_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_TZJSMagic(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_TZJSMagic_End(ATKER, DEFER, sklv)
    local Sta_role = Sta(DEFER)
    hpdmg = math.max(50, (150 - Sta_role)) * 5
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function Skill_QuanX_End(ATKER, DEFER, sklv)
    local statetime = 5
    local statelv = 10
    AddState(ATKER, DEFER, STATE_XY, statelv, statetime)
    local hpdmg = 200
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillArea_Line_JXJBFW(sklv)
    local lenth = 500
    local width = 200
    SetSkillRange(1, lenth, width)
end

function SkillCooldown_JXJBFW(sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillSp_JXJBFW(sklv)
    local sp_reduce = 20 + sklv * 3
    return sp_reduce
end

function Skill_JXJBFW_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_JXJBFW(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_JXJBFW_End(ATKER, DEFER, sklv)
    local dmg = 150
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_JBXZSB(sklv)
    local side = 300
    SetSkillRange(4, side)
end

function SkillCooldown_JBXZSB(sklv)
    local Cooldown = 5000
    return Cooldown
end

function SkillPre_JBXZSB(sklv)
end

function SkillSp_JBXZSB(sklv)
    local sp_reduce = 20 - math.floor(sklv * 0.5)
    return sp_reduce
end

function Skill_JBXZSB_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_JBXZSB(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_JBXZSB_End(ATKER, DEFER, sklv)
    local hp = Hp(DEFER)

    dmg = Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, dmg)
    Check_Ys_Rem(ATKER, DEFER)
end

function SkillSp_BLGJ(sklv)
    local sp_reduce = 15
    return sp_reduce
end

function SkillArea_Sector_BLGJ(sklv)
    local angle = 120
    local radius = 800
    SetSkillRange(2, radius, angle)
end

function SkillCooldown_BLGJ(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_BLGJ_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BLGJ(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BLGJ_End(ATKER, DEFER, sklv)
    local hpdmg = 1.5 * Atk_Dmg(ATKER, DEFER)
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillArea_Circle_BHSD(sklv)
    local side = 300
    SetSkillRange(4, side)
end

function SkillSp_BHSD(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillCooldown_BHSD(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_BHSD_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_BHSD(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_BHSD_End(ATKER, DEFER, sklv)
    local statelv = 10
    local statetime = 15
    AddState(ATKER, DEFER, STATE_BDJ, statelv, statetime)
    local hpdmg = 500
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillSp_HLKJ(sklv)
    local sp_reduce = 200
    return sp_reduce
end

function State_MarchElf_Add(role, statelv)
    local def_dif = 50
    local srecsb_dif = 20
    local defsb = DefSb(role) + def_dif
    local srecsb = SrecSb(role) + srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_MarchElf_Rem(role, statelv)
    local def_dif = 50
    local srecsb_dif = 20
    local defsb = DefSb(role) - def_dif
    local srecsb = SrecSb(role) - srecsb_dif
    SetCharaAttr(srecsb, role, ATTR_STATEV_SREC)
    SetCharaAttr(defsb, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_JLDS_Add(role, statelv)
    local hpdmg = 30 * statelv
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_JLDS_Rem(role, statelv)
end

function State_CJBBT_Add(role, statelv)
    local str = GetChaAttr(role, ATTR_STR)
    SetCharaAttr(str, role, ATTR_STATEV_STR)
    ALLExAttrSet(role)
end
function State_CJBBT_Rem(role, statelv)
    SetCharaAttr(0, role, ATTR_STATEV_STR)
    ALLExAttrSet(role)
end

function State_JRQKL_Add(role, statelv)
    local con = GetChaAttr(role, ATTR_CON)
    SetCharaAttr(con, role, ATTR_STATEV_CON)
    ALLExAttrSet(role)
end
function State_JRQKL_Rem(role, statelv)
    SetCharaAttr(0, role, ATTR_STATEV_CON)
    ALLExAttrSet(role)
end

function SkillCooldown_wljs(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_wljs_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 6
    local statelv = 4
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_wljy(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_wljy_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 3
    local statelv = 4
    local Sta_role = Sta(DEFER)
    hpdmg = 400 + math.max(50, (150 - Sta_role)) * 8
    Hp_Endure_Dmg(DEFER, hpdmg)
    AddState(ATKER, DEFER, STATE_WLJY, statelv, statetime)
end

function State_wljy_Add(role, statelv)
end

function State_wljy_Rem(role, statelv)
end

function SkillCooldown_jgs(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_jgs_End(ATKER, DEFER, sklv)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = math.floor(500 + hp * 0.05)
    if dmg > 2000 then
        dmg = 2000
    end
    hp = hp - dmg
    SetCharaAttr(hp, DEFER, ATTR_HP)
end

function SkillCooldown_wldb(sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_wldb_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10

    local defer_def = Def(DEFER)

    Hp_Endure_Dmg(DEFER, hpdmg)
    AddState(ATKER, DEFER, STATE_WLDB, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function SkillArea_Circle_ywgj(sklv)
    local side = 300
    SetSkillRange(3, side)
end

function SkillCooldown_ywgj(sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillArea_State_ywgj(sklv)
    local statetime = 20
    local statelv = sklv
    SetRangeState(STATE_MW, statelv, statetime)
end

function Skill_ywgj_End(ATKER, DEFER, sklv)
end

function State_ywgj_Add(role, statelv)
    local agisb = AgiSb(role)
    local hitsb_dif = 10 + math.floor(math.max(5, agisb / 20))
    local hitsb = HitSb(role) - hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function State_ywgj_Rem(role, statelv)
    local agisb = AgiSb(role)
    local hitsb_dif = 10 + math.floor(math.max(5, agisb / 20))
    local hitsb = HitSb(role) + hitsb_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    ALLExAttrSet(role)
end

function State_ywgj_Tran(statelv)
    return 1
end

function SkillCooldown_klhd(ATKER, DEFER, sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_klhd_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10
    AddState(ATKER, DEFER, STATE_KLHD, statelv, statetime)
end

function State_klhd_Add(role, statelv)
    local defsa_dif = 0.8
    local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function State_klhd_Rem(role, statelv)
    local defsa_dif = 0.8
    local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_xegj(ATKER, DEFER, sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_xegj_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local Sta_role = Sta(DEFER)
    hpdmg = 400 + math.max(50, (150 - Sta_role)) * 5
    Hp_Endure_Dmg(DEFER, hpdmg)
end

function SkillCooldown_wlnh(ATKER, DEFER, sklv)
    local Cooldown = 2000
    return Cooldown
end

function Skill_wlnh_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10
    AddState(ATKER, DEFER, STATE_WLNH, statelv, statetime)
end

function State_wlnh_Add(role, statelv)
    local atksb_dif = 800
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_wlnh_Rem(role, statelv)
    local atksb_dif = 800
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_wlcx(ATKER, DEFER, sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillArea_Circle_wlcx(sklv)
    local side = 2000
    SetSkillRange(4, side)
end

function Skill_wlcx_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 10
    AddState(ATKER, DEFER, STATE_WLCX, statelv, statetime)
end

function State_wlcx_Add(role, statelv)
    local hitsb_dif = 30
    local hitsb = HitSb(role) - hitsb_dif
    local flee_dif = 10
    local fleesb = FleeSb(role) - flee_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)

    ALLExAttrSet(role)
end

function State_wlcx_Rem(role, statelv)
    local hitsb_dif = 30
    local hitsb = HitSb(role) + hitsb_dif
    local flee_dif = 10
    local fleesb = FleeSb(role) + flee_dif
    SetCharaAttr(hitsb, role, ATTR_STATEV_HIT)
    SetCharaAttr(fleesb, role, ATTR_STATEV_FLEE)

    ALLExAttrSet(role)
end

function SkillCooldown_wlxw(ATKER, DEFER, sklv)
    local Cooldown = 2000
    return Cooldown
end

function SkillArea_Circle_wlxw(sklv)
    local side = 1000
    SetSkillRange(3, side)
end

function SkillArea_State_wlxw(sklv)
    local statetime = 10
    local statelv = sklv
    SetRangeState(STATE_XW, statelv, statetime)
end

function Skill_wlxw_End(ATKER, DEFER, sklv)
end

function State_wlxw_Add(role, statelv)
    local mspdsa_dif = 0.5
    local aspdsa_dif = 0.3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    local aspdsa = (AspdSa(role) - aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_wlxw_Rem(role, statelv)
    local mspdsa_dif = 0.5
    local aspdsa_dif = 0.3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    local aspdsa = (AspdSa(role) + aspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    SetCharaAttr(aspdsa, role, ATTR_STATEC_ASPD)
    ALLExAttrSet(role)
end

function State_wlxw_Tran(statelv)
    return 1
end

function SkillCooldown_JLFT(Lv)
    local Cooldown = 180000
    return Cooldown
end

function SkillSp_JLFT(Lv)
    local SP = 20
    return SP
end

function Skill_JLFT_BEGIN(Player, Lv)
    local Fairy = GetEquipItemP(Player, 16)
    if Fairy == nil then
        SkillUnable(Player)
        SystemNotice(Player, "Sorry but you don't have any fairy on your slot.")
        return
    end
	local FairyHP = GetItemAttr(Fairy, ITEMATTR_URE)
    if FairyHP < (Server.Fairy.Possession.Stamina * 50) then
        SkillUnable(Player)
        SystemNotice(Player, "Fairy stamina below " .. Server.Fairy.Possession.Stamina .. ", cannot use Fairy Possession.")
        return
    end
    local FairyLv = GetFairyLevel(Fairy)
    FairyHP = FairyHP - (6 * FairyLv / Lv) * 50
    SetItemAttr(Fairy, ITEMATTR_URE, FairyHP)
	if FairyHP < 50 then
		SetChaEquipValid(Player, 16, 0)
	end
	RefreshCha(Player)
    SynLook(Player)
end

function Skill_JLFT_End(ATKER, DEFER, Lv)
    local Time = 190 - Lv * 10
    local Fairy = GetEquipItemP(ATKER, 16)
    if Fairy ~= nil then
        local FairyID = GetItemID(Fairy)
        if FairyID == Server.Fairy.ID["Fairy of Luck"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Luck, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Strength"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Strength, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Constitution"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Constitution, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Spirit"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Spirit, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Accuracy"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Accuracy, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Agility"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Agility, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Fairy of Evil"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.Evil, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Mordo JR"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.MordoJR, Lv, Time)
        elseif FairyID == Server.Fairy.ID["Angela JR"] then
            AddState(ATKER, ATKER, Server.Fairy.Effect.AngelaJR, Lv, Time)
        end
    end
end

function State_JLFT_Add(Player, Lv)
    local Fairy = GetEquipItemP(Player, 16)
	local FairyType = GetItemType(Fairy)
	local FairyID = GetItemID(Fairy)
    local PlayerID = GetCharID(Player)
    if Server.Player[PlayerID] == nil then
        Server.Player[PlayerID] = {}
    end
    if Server.Player[PlayerID].Possession == nil then
        Server.Player[PlayerID].Possession = {FAIRY = FairyID, STR = 0, CON = 0, ACC = 0, AGI = 0, SPR = 0, PDEF = 0}
    end
    if Fairy ~= nil then
        if FairyType == 59 then
			if FairyID == Server.Fairy.ID["Fairy of Strength"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.STR = GetItemAttr(Fairy, ITEMATTR_VAL_STR)
			end
			if FairyID == Server.Fairy.ID["Fairy of Constitution"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.CON = GetItemAttr(Fairy, ITEMATTR_VAL_CON)
			end
			if FairyID == Server.Fairy.ID["Fairy of Spirit"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.SPR = GetItemAttr(Fairy, ITEMATTR_VAL_STA)
			end
			if FairyID == Server.Fairy.ID["Fairy of Accuracy"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.ACC = GetItemAttr(Fairy, ITEMATTR_VAL_DEX)
			end
			if FairyID == Server.Fairy.ID["Fairy of Agility"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.AGI = GetItemAttr(Fairy, ITEMATTR_VAL_AGI)
			end
			if FairyID == Server.Fairy.ID["Mordo JR"] and FairyID == Server.Player[PlayerID].Possession.FAIRY then
				Server.Player[PlayerID].Possession.STR = GetItemAttr(Fairy, ITEMATTR_VAL_STR)
				Server.Player[PlayerID].Possession.CON = GetItemAttr(Fairy, ITEMATTR_VAL_CON)
				Server.Player[PlayerID].Possession.AGI = GetItemAttr(Fairy, ITEMATTR_VAL_AGI)
				Server.Player[PlayerID].Possession.ACC = GetItemAttr(Fairy, ITEMATTR_VAL_DEX)
				Server.Player[PlayerID].Possession.SPR = GetItemAttr(Fairy, ITEMATTR_VAL_STA)
			end
			local STR = GetChaAttr(Player, ATTR_STATEV_STR) + Server.Player[PlayerID].Possession.STR
			local CON = GetChaAttr(Player, ATTR_STATEV_CON) + Server.Player[PlayerID].Possession.CON
			local AGI = GetChaAttr(Player, ATTR_STATEV_AGI) + Server.Player[PlayerID].Possession.AGI
			local ACC = GetChaAttr(Player, ATTR_STATEV_DEX) + Server.Player[PlayerID].Possession.ACC
			local SPR = GetChaAttr(Player, ATTR_STATEV_STA) + Server.Player[PlayerID].Possession.SPR
			SetCharaAttr(STR, Player, ATTR_STATEV_STR)
			SetCharaAttr(CON, Player, ATTR_STATEV_CON)
			SetCharaAttr(AGI, Player, ATTR_STATEV_AGI)
			SetCharaAttr(ACC, Player, ATTR_STATEV_DEX)
			SetCharaAttr(SPR, Player, ATTR_STATEV_STA)
        end
    end
    ALLExAttrSet(Player)
    RefreshCha(Player)
end

function State_JLFT_Rem(Player, Lv)
	local PlayerID = GetCharID(Player)
    local STR = GetChaAttr(Player, ATTR_STATEV_STR) - Server.Player[PlayerID].Possession.STR
    local CON = GetChaAttr(Player, ATTR_STATEV_CON) - Server.Player[PlayerID].Possession.CON
    local AGI = GetChaAttr(Player, ATTR_STATEV_AGI) - Server.Player[PlayerID].Possession.AGI
    local ACC = GetChaAttr(Player, ATTR_STATEV_DEX) - Server.Player[PlayerID].Possession.ACC
    local SPR = GetChaAttr(Player, ATTR_STATEV_STA) - Server.Player[PlayerID].Possession.SPR
	Server.Player[PlayerID].Possession = nil
    SetCharaAttr(STR, Player, ATTR_STATEV_STR)
    SetCharaAttr(CON, Player, ATTR_STATEV_CON)
    SetCharaAttr(AGI, Player, ATTR_STATEV_AGI)
    SetCharaAttr(ACC, Player, ATTR_STATEV_DEX)
    SetCharaAttr(SPR, Player, ATTR_STATEV_STA)
    ALLExAttrSet(Player)
    RefreshCha(Player)
end

function SkillCooldown_jlzb(sklv)
    local Cooldown = 180000
    return Cooldown
end

function SkillArea_Circle_jlzb(sklv)
    local side = 1500
    SetSkillRange(4, side)
end

function Skill_jlzb_Begin(role, sklv)
    local Fairy = GetEquipItemP(Player, 16)
    if Fairy == nil then
        SkillUnable(Player)
        SystemNotice(Player, "Sorry but you don't have any fairy on your slot.")
        return
    end

    local item_elf_type = GetItemType(Fairy)
    local Num_JL = GetItemForgeParam(Fairy, 1)
    local Part1 = GetNum_Part1(Num_JL)

    if item_elf_type ~= 59 or Part1 ~= 1 then
        SkillUnable(role)
        SystemNotice(role, "Current skill is only available if the new generation of pet is equipped!")
        return
    end

    local item_elf_hp = GetItemAttr(Fairy, ITEMATTR_URE)
    if item_elf_hp < 50 then
        SkillUnable(role)
        SystemNotice(role, "Fairy's HP must be more than 0 to use this skill")
        return
    end

    local elf_str = GetItemAttr(Fairy, ITEMATTR_VAL_STR)
    local elf_con = GetItemAttr(Fairy, ITEMATTR_VAL_CON)
    local elf_agi = GetItemAttr(Fairy, ITEMATTR_VAL_AGI)
    local elf_dex = GetItemAttr(Fairy, ITEMATTR_VAL_DEX)
    local elf_sta = GetItemAttr(Fairy, ITEMATTR_VAL_STA)
    local elf_lv = elf_str + elf_con + elf_agi + elf_dex + elf_sta

    item_elf_hp = item_elf_hp - (9 * elf_lv / sklv) * 50

    SetItemAttr(Fairy, ITEMATTR_URE, item_elf_hp)
end

function Skill_jlzb_End(ATKER, DEFER, sklv)
    local dmg_fin = 1
    local item_elf = GetEquipItemP(ATKER, 16)
    if item_elf == nil then 
        return
    end

    local item_elf_type = GetItemType(item_elf)
    local ptnRoleType = GetChaAttr(DEFER, ATTR_CHATYPE)
    if ptnRoleType == 1 or ptnRoleType == 5 or ptnRoleType == 17 then
        if item_elf_type == 59 then
            local elf_str = GetItemAttr(item_elf, ITEMATTR_VAL_STR)
            local elf_con = GetItemAttr(item_elf, ITEMATTR_VAL_CON)
            local elf_agi = GetItemAttr(item_elf, ITEMATTR_VAL_AGI)
            local elf_dex = GetItemAttr(item_elf, ITEMATTR_VAL_DEX)
            local elf_sta = GetItemAttr(item_elf, ITEMATTR_VAL_STA)
            local elf_lv = elf_str + elf_con + elf_agi + elf_dex + elf_sta
            local elf_ure = GetItemAttr(item_elf, ITEMATTR_URE) * -1
            local elf_maxure = GetItemAttr(item_elf, ITEMATTR_MAXURE)

            local str = GetChaAttr(DEFER, ATTR_STR)
            local con = GetChaAttr(DEFER, ATTR_CON)
            local sta = GetChaAttr(DEFER, ATTR_STA)
            local agi = GetChaAttr(DEFER, ATTR_AGI)
            local dex = GetChaAttr(DEFER, ATTR_DEX)
            local Defer_Sum = str + con + sta + agi + dex

            dmg_fin = elf_lv * 200 - (Defer_Sum * Defer_Sum * Defer_Sum / 10000)
            if dmg_fin < 0 then
                dmg_fin = 0
            end

            if is_friend(ATKER, DEFER) ~= 1 then
                Hp_Endure_Dmg(DEFER, dmg_fin)
            end

            local statetime = 20
            local statelv = 10
            local atker_hp = GetChaAttr(ATKER, ATTR_HP)

            if dmg_fin >= atker_hp then
                AddState(ATKER, ATKER, STATE_XY, statelv, statetime)
            else
                local star_hp_differ = atker_hp - dmg_fin
                SetCharaAttr(star_hp_differ, ATKER, ATTR_HP)
            end
        end
    end
end

function State_5MBS_Add(role, sklv)
    local role_mxhp = GetChaAttr(role, ATTR_MXHP)
    Hp_Endure_Dmg(role, role_mxhp * 0.95)
    SystemNotice(role, "The Almighty is angry with your actions! Prepare to be punished!")
end

function State_5MBS_Rem(role, sklv)
end

function JLTX_usu(role)
    local Fairy = GetEquipItemP(role, 16)
    if Fairy == nil or GetItemType(Fairy) ~= 59 then
        SkillUnable(role)
        BickerNotice(role, "You don't have a fairy on your slot")
        return 0        
    end
    
    local FairyHP = GetItemAttr(Fairy, ITEMATTR_URE)
    local Endure = FairyHP - 100
    if FairyHP < 50 then
        SkillUnable(role)
        BickerNotice(role, "Fairy's HP must be more than 0 to use this skill")
        return
    end
    local item_count = CheckBagItem(role, 855)
    if item_count <= 0 then
        SkillUnable(role)
        BickerNotice(role, "You do not have the required Fairy Coin to use the skill")
        return 0
    end
    SetItemAttr(Fairy, ITEMATTR_URE, Endure)
    return 1
end

function SkillCooldown_Jltx1(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx1_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx1_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX1, statelv, statetime)
end

function State_jltx1_Add(role, sklv)
end

function State_jltx1_Rem(role, sklv)
end

function SkillCooldown_Jltx2(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx2_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx2_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX2, statelv, statetime)
end

function State_jltx2_Add(role, sklv)
end

function State_jltx2_Rem(role, sklv)
end

function SkillCooldown_Jltx3(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx3_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx3_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX3, statelv, statetime)
end

function State_jltx3_Add(role, sklv)
end

function State_jltx3_Rem(role, sklv)
end

function SkillCooldown_Jltx4(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx4_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx4_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX4, statelv, statetime)
end

function State_jltx4_Add(role, sklv)
end

function State_jltx4_Rem(role, sklv)
end

function SkillCooldown_Jltx5(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx5_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx5_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX5, statelv, statetime)
end

function State_jltx5_Add(role, sklv)
end

function State_jltx5_Rem(role, sklv)
end

function SkillCooldown_Jltx6(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx6_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx6_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX6, statelv, statetime)
end

function State_jltx6_Add(role, sklv)
end

function State_jltx6_Rem(role, sklv)
end

function SkillCooldown_Jltx7(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx7_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx7_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX7, statelv, statetime)
end

function State_jltx7_Add(role, sklv)
end

function State_jltx7_Rem(role, sklv)
end

function SkillCooldown_Jltx8(sklv)
    local Cooldown = 10000
    return Cooldown
end

function Skill_Jltx8_Begin(role, sklv)
    local ret = JLTX_usu(role)
end

function Skill_Jltx8_End(ATKER, DEFER, sklv)
    local statelv = sklv
    local statetime = 60
    AddState(ATKER, DEFER, STATE_JLTX8, statelv, statetime)
end

function State_jltx8_Add(role, sklv)
end

function State_jltx8_Rem(role, sklv)
end

function State_CZZX_Add(role, statelv)
    local str = GetChaAttr(role, ATTR_STR)
    local con = GetChaAttr(role, ATTR_CON)
    local sta = GetChaAttr(role, ATTR_STA)
    local agi = GetChaAttr(role, ATTR_AGI)
    local dex = GetChaAttr(role, ATTR_DEX)
    str = math.floor(str * 0.3)
    con = math.floor(con * 0.3)
    sta = math.floor(sta * 0.3)
    agi = math.floor(agi * 0.3)
    dex = math.floor(dex * 0.3)
    SetCharaAttr(str, role, ATTR_STATEV_STR)
    SetCharaAttr(con, role, ATTR_STATEV_CON)
    SetCharaAttr(sta, role, ATTR_STATEV_STA)
    SetCharaAttr(agi, role, ATTR_STATEV_AGI)
    SetCharaAttr(dex, role, ATTR_STATEV_DEX)
    ALLExAttrSet(role)
end
function State_CZZX_Rem(role, statelv)
    SetCharaAttr(0, role, ATTR_STATEV_STR)
    SetCharaAttr(0, role, ATTR_STATEV_CON)
    SetCharaAttr(0, role, ATTR_STATEV_STA)
    SetCharaAttr(0, role, ATTR_STATEV_AGI)
    SetCharaAttr(0, role, ATTR_STATEV_DEX)
    ALLExAttrSet(role)
end

function State_KALA_Add(role, statelv)
    local sta = GetChaAttr(role, ATTR_STA)
    SetCharaAttr(sta, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end
function State_KALA_Rem(role, statelv)
    SetCharaAttr(0, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function State_XUEYU_Add(Player, StateLevel)
    local PhysicalResist = 0
    if StateLevel == 1 then
        PhysicalResist = 1
    elseif StateLevel == 2 or StateLevel == 3 then
        PhysicalResist = 2
    elseif StateLevel == 4 or StateLevel == 5 then
        PhysicalResist = 3
    elseif StateLevel == 6 then
        PhysicalResist = 4
    elseif StateLevel == 7 then
        PhysicalResist = 5
    elseif StateLevel == 8 or StateLevel == 9 then
        PhysicalResist = 6
    elseif StateLevel == 10 then
        PhysicalResist = 8
    end
    local CurrentHP = GetChaAttr(Player, ATTR_HP)
    local BonusHP = StateLevel * StateLevel * 100
    SetCharaAttr(CurrentHP, Player, ATTR_HP)
    SetCharaAttr(BonusHP, Player, ATTR_STATEV_MXHP)
    SetCharaAttr(PhysicalResist, Player, ATTR_STATEV_PDEF)
    ALLExAttrSet(Player)
end
function State_XUEYU_Rem(Player, StateLevel)
    SetCharaAttr(0, Player, ATTR_STATEV_PDEF)
    SetCharaAttr(0, Player, ATTR_STATEV_MXHP)
    ALLExAttrSet(Player)
end

function State_MANTOU_Add(role, statelv)
    local atksb_dif = 50 + (statelv - 1) * 100
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end
function State_MANTOU_Rem(role, statelv)
    local atksb_dif = 50 + (statelv - 1) * 100
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_NVER_Add(role, statelv)
    local sta = statelv * 5
    SetCharaAttr(sta, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end
function State_NVER_Rem(role, statelv)
    SetCharaAttr(0, role, ATTR_STATEV_STA)
    ALLExAttrSet(role)
end

function Skill_xzlw_End(ATKER, DEFER, sklv)
    local dmg = math.random(15, 35)
    Hp_Endure_Dmg(DEFER, dmg)
end

function Skill_Cooking_End(ATKER, DEFER, sklv)
end

function Skill_Making_End(ATKER, DEFER, sklv)
end

function Skill_Founding_End(ATKER, DEFER, sklv)
end

function Skill_Dismissing_End(ATKER, DEFER, sklv)
end

function SkillCooldown_Shoulei1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Shoulei1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 1135)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 1135, 1) 
end

function Skill_Shoulei1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1
    local hp = GetChaAttr(DEFER, ATTR_HP)
    dmg = 4 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 1200
    if dmg > 1000 then
        dmg = 1000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_ShanGD1(sklv)
    local side = 800
    SetSkillRange(4, side)
end

function SkillCooldown_ShanGD1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_ShanGD1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 1136)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 1136, 1) 
end

function Skill_ShanGD1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1
    local statelv = sklv
    local statetime = sklv
    local hp_defer = Mxhp(DEFER)
    if hp_defer >= 1000000 then
        SystemNotice(ATKER, "Flash Bomb loses effect")
        return
    end

    if GetChaTypeID(ATKER) == 979 then
        statetime = 8
    end

	if GetChaStateLv(DEFER, 159) > 0 then
		return 0
	end
	
    if GetChaAIType(DEFER) >= 21 then
        if BOSSXYSJ[GetChaTypeID(DEFER)] == 0 then
            SystemNotice("As a Boss, how can I be defeated by ths same skill. Beware warriors!")
            return
        else
            BOSSXYSJ[GetChaTypeID(DEFER)] = BOSSXYSJ[GetChaTypeID(DEFER)] - 1
        end
    end

    AddState(ATKER, DEFER, STATE_ShanGD, statelv, statetime)
end

function State_ShanGD_Add(role, statelv)
end

function State_ShanGD_Rem(role, statelv)
end

function SkillArea_Circle_FuShe1(sklv)
    local sklv = 1
    local side = 800 + sklv * 100
    SetSkillRange(4, side)
end

function SkillCooldown_FuShe1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_FuShe1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 1137)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 1137, 1) 	
end

function Skill_FuShe1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1
    local hp = Hp(DEFER)
    local statelv = sklv
    local statetime = 15 + sklv * 2
    AddState(ATKER, DEFER, STATE_ZD, statelv, statetime)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = 2 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 600
    if dmg > 500 then
        dmg = 500
    end

    Hp_Endure_Dmg(DEFER, dmg)
end

function State_FuShe_Add(role, statelv)
    local hpdmg = 20
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_FuShe_Rem(role, statelv)
end

function SkillCooldown_YouL1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_YouL1(sklv)
    local sklv = 1
    local side = 600 + sklv * 20
    SetSkillRange(3, side)
end

function SkillArea_State_YouL1(sklv)
    local sklv = 1

    local statetime = 40 + sklv * 9
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_YouL1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 1138)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 1138, 1) 
end

function Skill_YouL1_End(ATKER, DEFER, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillCooldown_JiaSuQi(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_JiaSuQi1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1139)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1139, 1)
end

function Skill_JiaSuQi1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PengSheQi1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PengSheQi1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1140)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1140, 1)
end

function Skill_PengSheQi1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1

    local statetime = 3 + sklv * 2
    local statelv = sklv
    AddState(ATKER, DEFER, STATE_PSQ, statelv, statetime)
end

function State_PengSheQi_Add(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_PengSheQi_Rem(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PoJiaDan(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoJiaDan1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1141)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1141, 1)
end

function Skill_PoJiaDan1_End(ATKER, DEFER, sklv)
    local sklv = 1

    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PJ, statelv, statetime)

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Pj_Add(role, statelv)
    local def_dif = 30
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = 30
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_PoRenDan1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoRenDan1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1142)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1142, 1)
end

function Skill_PoRenDan1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PRD, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function State_PoRenDan_Add(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PoRenDan_Rem(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_RanShaoDan1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_RanShaoDan1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1143)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1143, 1)
end

function Skill_RanShaoDan1_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 1

    local statelv = sklv
    local statetime = 4 + sklv * 2
    AddState(ATKER, DEFER, STATE_CZRSD, statelv, statetime)
    dmg = math.random(100, 200) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_RanShaoDan_Add(role, statelv)
    local hpdmg = 150
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_RanShaoDan_Rem(role, statelv)
end

function SkillCooldown_Shoulei2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Shoulei2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2719)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2719, 1) 	
end

function Skill_Shoulei2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 2
    local hp = GetChaAttr(DEFER, ATTR_HP)
    dmg = 4 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 1200
    if dmg > 2000 then
        dmg = 2000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_ShanGD2(sklv)
    local side = 800
    SetSkillRange(4, side)
end

function SkillCooldown_ShanGD2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_ShanGD2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2720)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2720, 1)
end

function Skill_ShanGD2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 2
    local statelv = sklv
    local statetime = sklv
    local hp_defer = Mxhp(DEFER)
    if hp_defer >= 1000000 then
        SystemNotice(ATKER, "Flash Bomb loses effect")
        return
    end

    if GetChaTypeID(ATKER) == 979 then
        statetime = 8
    end

	if GetChaStateLv(DEFER, 159) > 0 then
		return 0
	end
	
    if GetChaAIType(DEFER) >= 21 then
        if BOSSXYSJ[GetChaTypeID(DEFER)] == 0 then
            SystemNotice("As a Boss, how can I be defeated by ths same skill. Beware warriors!")
            return
        else
            BOSSXYSJ[GetChaTypeID(DEFER)] = BOSSXYSJ[GetChaTypeID(DEFER)] - 1
        end
    end

    AddState(ATKER, DEFER, STATE_ShanGD, statelv, statetime)
end

function State_ShanGD_Add(role, statelv)
end

function State_ShanGD_Rem(role, statelv)
end

function SkillArea_Circle_FuShe2(sklv)
    local sklv = 2
    local side = 800 + sklv * 100
    SetSkillRange(4, side)
end

function SkillCooldown_FuShe2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_FuShe2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2721)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2721, 1)
end

function Skill_FuShe2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 2
    local hp = Hp(DEFER)
    local statelv = sklv
    local statetime = 15 + sklv * 2
    AddState(ATKER, DEFER, STATE_ZD, statelv, statetime)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = 2 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 600
    if dmg > 1000 then
        dmg = 1000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillCooldown_YouL2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_YouL2(sklv)
    local sklv = 2
    local side = 600 + sklv * 20
    SetSkillRange(3, side)
end

function SkillArea_State_YouL2(sklv)
    local sklv = 2

    local statetime = 40 + sklv * 9
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_YouL2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2722)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2722, 1)
end

function Skill_YouL2_End(ATKER, DEFER, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillCooldown_JiaSuQi2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_JiaSuQi2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2723)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2723, 1)
end

function Skill_JiaSuQi2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 2
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PengSheQi2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PengSheQi2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2724)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2724, 1)
end

function Skill_PengSheQi2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 2

    local statetime = 3 + sklv * 2
    local statelv = sklv
    AddState(ATKER, DEFER, STATE_PSQ, statelv, statetime)
end

function State_PengSheQi_Add(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_PengSheQi_Rem(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PoJiaDan2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoJiaDan2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2725)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2725, 1)
end

function Skill_PoJiaDan2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 2

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PJ, statelv, statetime)

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Pj_Add(role, statelv)
    local def_dif = 30
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = 30
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_PoRenDan2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoRenDan2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2726)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2726, 1)
end

function Skill_PoRenDan2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 2

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PRD, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function State_PoRenDan_Add(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PoRenDan_Rem(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_RanShaoDan1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_RanShaoDan2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2727)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2727, 1)
end

function Skill_RanShaoDan2_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 2

    local statelv = sklv
    local statetime = 4 + sklv * 2
    AddState(ATKER, DEFER, STATE_CZRSD, statelv, statetime)
    dmg = math.random(100, 200) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_RanShaoDan_Add(role, statelv)
    local hpdmg = 150
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_RanShaoDan_Rem(role, statelv)
end

function SkillCooldown_Shoulei3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Shoulei3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2743)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2743, 1)
end

function Skill_Shoulei3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 3
    local hp = GetChaAttr(DEFER, ATTR_HP)
    dmg = 4 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 1200
    if dmg > 3000 then
        dmg = 3000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_ShanGD3(sklv)
    local side = 800
    SetSkillRange(4, side)
end

function SkillCooldown_ShanGD3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_ShanGD3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2744)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2744, 1)
end

function Skill_ShanGD3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 3
    local statelv = sklv
    local statetime = sklv
    local hp_defer = Mxhp(DEFER)
    if hp_defer >= 1000000 then
        SystemNotice(ATKER, "Flash Bomb loses effect")
        return
    end

    if GetChaTypeID(ATKER) == 979 then
        statetime = 8
    end

	if GetChaStateLv(DEFER, 159) > 0 then
		return 0
	end
	
    if GetChaAIType(DEFER) >= 21 then
        if BOSSXYSJ[GetChaTypeID(DEFER)] == 0 then
            SystemNotice("As a Boss, how can I be defeated by ths same skill. Beware warriors!")
            return
        else
            BOSSXYSJ[GetChaTypeID(DEFER)] = BOSSXYSJ[GetChaTypeID(DEFER)] - 1
        end
    end

    AddState(ATKER, DEFER, STATE_ShanGD, statelv, statetime)
end

function State_ShanGD_Add(role, statelv)
end

function State_ShanGD_Rem(role, statelv)
end

function SkillArea_Circle_FuShe3(sklv)
    local sklv = 3
    local side = 800 + sklv * 100
    SetSkillRange(4, side)
end

function SkillCooldown_FuShe3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_FuShe3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2745)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2745, 1)
end

function Skill_FuShe3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 3
    local hp = Hp(DEFER)
    local statelv = sklv
    local statetime = 15 + sklv * 2
    AddState(ATKER, DEFER, STATE_ZD, statelv, statetime)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = 2 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 600
    if dmg > 1500 then
        dmg = 1500
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillCooldown_YouL3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_YouL3(sklv)
    local sklv = 3
    local side = 600 + sklv * 20
    SetSkillRange(3, side)
end

function SkillArea_State_YouL3(sklv)
    local sklv = 3

    local statetime = 40 + sklv * 9
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_YouL3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2746)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2746, 1)
end

function Skill_YouL3_End(ATKER, DEFER, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillCooldown_JiaSuQi3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_JiaSuQi3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2747)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2747, 1)
end

function Skill_JiaSuQi3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 3
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PengSheQi3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PengSheQi3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2748)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2748, 1)
end

function Skill_PengSheQi3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 3

    local statetime = 3 + sklv * 2
    local statelv = sklv
    AddState(ATKER, DEFER, STATE_PSQ, statelv, statetime)
end

function State_PengSheQi_Add(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_PengSheQi_Rem(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PoJiaDan3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoJiaDan3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2749)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2749, 1)
end

function Skill_PoJiaDan3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 3

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PJ, statelv, statetime)

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Pj_Add(role, statelv)
    local def_dif = 30
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = 30
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_PoRenDan3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoRenDan3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2750)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2750, 1)
end

function Skill_PoRenDan3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 3

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PRD, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function State_PoRenDan_Add(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PoRenDan_Rem(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_RanShaoDan3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_RanShaoDan3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2751)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2751, 1)
end

function Skill_RanShaoDan3_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 3

    local statelv = sklv
    local statetime = 4 + sklv * 2
    AddState(ATKER, DEFER, STATE_CZRSD, statelv, statetime)
    dmg = math.random(100, 200) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_RanShaoDan_Add(role, statelv)
    local hpdmg = 150
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_RanShaoDan_Rem(role, statelv)
end

function SkillCooldown_Shoulei4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Shoulei4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2767)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2767, 1)
end

function Skill_Shoulei4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 4
    local hp = GetChaAttr(DEFER, ATTR_HP)
    dmg = 4 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 1200
    if dmg > 4000 then
        dmg = 4000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_ShanGD4(sklv)
    local side = 800
    SetSkillRange(4, side)
end

function SkillCooldown_ShanGD4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_ShanGD4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2768)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2768, 1)
end

function Skill_ShanGD4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 4
    local statelv = sklv
    local statetime = sklv
    local hp_defer = Mxhp(DEFER)
    if hp_defer >= 1000000 then
        SystemNotice(ATKER, "Flash Bomb loses effect")
        return
    end

    if GetChaTypeID(ATKER) == 979 then
        statetime = 8
    end

	if GetChaStateLv(DEFER, 159) > 0 then
		return 0
	end
	
    if GetChaAIType(DEFER) >= 21 then
        if BOSSXYSJ[GetChaTypeID(DEFER)] == 0 then
            SystemNotice("As a Boss, how can I be defeated by ths same skill. Beware warriors!")
            return
        else
            BOSSXYSJ[GetChaTypeID(DEFER)] = BOSSXYSJ[GetChaTypeID(DEFER)] - 1
        end
    end

    AddState(ATKER, DEFER, STATE_ShanGD, statelv, statetime)
end

function State_ShanGD_Add(role, statelv)
end

function State_ShanGD_Rem(role, statelv)
end

function SkillArea_Circle_FuShe4(sklv)
    local sklv = 4
    local side = 800 + sklv * 100
    SetSkillRange(4, side)
end

function SkillCooldown_FuShe4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_FuShe4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2769)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2769, 1)
end

function Skill_FuShe4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 4
    local hp = Hp(DEFER)
    local statelv = sklv
    local statetime = 15 + sklv * 2
    AddState(ATKER, DEFER, STATE_ZD, statelv, statetime)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = 2 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 600
    if dmg > 2000 then
        dmg = 2000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillCooldown_YouL4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_YouL4(sklv)
    local sklv = 4
    local side = 600 + sklv * 20
    SetSkillRange(3, side)
end

function SkillArea_State_YouL4(sklv)
    local sklv = 4

    local statetime = 40 + sklv * 9
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_YouL4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2770)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2770, 1)
end

function Skill_YouL4_End(ATKER, DEFER, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillCooldown_JiaSuQi4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_JiaSuQi4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2771)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2771, 1)
end

function Skill_JiaSuQi4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 4
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PengSheQi4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PengSheQi4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2772)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2772, 1)
end

function Skill_PengSheQi4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 4

    local statetime = 3 + sklv * 2
    local statelv = sklv
    AddState(ATKER, DEFER, STATE_PSQ, statelv, statetime)
end

function State_PengSheQi_Add(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_PengSheQi_Rem(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PoJiaDan4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoJiaDan4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2773)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2773, 1)
end

function Skill_PoJiaDan4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 4

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PJ, statelv, statetime)

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Pj_Add(role, statelv)
    local def_dif = 30
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = 30
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_PoRenDan4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoRenDan4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2774)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2774, 1)
end

function Skill_PoRenDan4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 4

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PRD, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function State_PoRenDan_Add(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PoRenDan_Rem(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_RanShaoDan4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_RanShaoDan4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2775)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2775, 1)
end

function Skill_RanShaoDan4_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 4

    local statelv = sklv
    local statetime = 4 + sklv * 2
    AddState(ATKER, DEFER, STATE_CZRSD, statelv, statetime)
    dmg = math.random(100, 200) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_RanShaoDan_Add(role, statelv)
    local hpdmg = 150
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_RanShaoDan_Rem(role, statelv)
end

function SkillCooldown_Shoulei5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_Shoulei5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2791)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2791, 1)
end

function Skill_Shoulei5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5
    local hp = GetChaAttr(DEFER, ATTR_HP)
    dmg = 4 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 1200
    if dmg > 5000 then
        dmg = 5000
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillArea_Circle_ShanGD5(sklv)
    local side = 800
    SetSkillRange(4, side)
end

function SkillCooldown_ShanGD5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_ShanGD5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2792)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2792, 1)
end

function Skill_ShanGD5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5
    local statelv = sklv
    local statetime = sklv
    local hp_defer = Mxhp(DEFER)
    if hp_defer >= 1000000 then
        SystemNotice(ATKER, "Flash Bomb loses effect")
        return
    end
	if GetChaStateLv(DEFER, 159) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_ShanGD, statelv, statetime)
end

function State_ShanGD_Add(role, statelv)
end

function State_ShanGD_Rem(role, statelv)
end

function SkillArea_Circle_FuShe5(sklv)
    local sklv = 5
    local side = 800 + sklv * 100
    SetSkillRange(4, side)
end

function SkillCooldown_FuShe5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_FuShe5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2793)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2793, 1)
end

function Skill_FuShe5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 5
    local hp = Hp(DEFER)
    local statelv = sklv
    local statetime = 15 + sklv * 2
    AddState(ATKER, DEFER, STATE_ZD, statelv, statetime)
    local hp = GetChaAttr(DEFER, ATTR_HP)
    local dmg = 2 * math.floor(320 + 30 * sklv + hp * (0.05 + 0.008 * sklv)) - 600
    if dmg > 2500 then
        dmg = 2500
    end
    Hp_Endure_Dmg(DEFER, dmg)
end

function SkillCooldown_YouL5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_YouL5(sklv)
    local sklv = 5
    local side = 600 + sklv * 20
    SetSkillRange(3, side)
end

function SkillArea_State_YouL5(sklv)
    local sklv = 5

    local statetime = 40 + sklv * 9
    local statelv = sklv
    SetRangeState(STATE_SYZY, statelv, statetime)
end

function Skill_YouL5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2794)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
	local A = DelBagItem(role, 2794, 1)
end

function Skill_YouL5_End(ATKER, DEFER, sklv)
end

function State_Syzy_Add(role, statelv)
end

function State_Syzy_Rem(role, statelv)
end

function State_Syzy_Tran(statelv)
    return 1
end

function SkillCooldown_JiaSuQi5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_JiaSuQi5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2795)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2795, 1)
end

function Skill_JiaSuQi5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end

    local sklv = 5
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_WLJS, statelv, statetime)
end

function State_wljs_Add(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_wljs_Rem(role, statelv)
    local mspdsa_dif = 1
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PengSheQi5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PengSheQi5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2796)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2796, 1)
end

function Skill_PengSheQi5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5

    local statetime = 3 + sklv * 2
    local statelv = sklv
    AddState(ATKER, DEFER, STATE_PSQ, statelv, statetime)
end

function State_PengSheQi_Add(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) + mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function State_PengSheQi_Rem(role, statelv)
    local mspdsa_dif = 3
    local mspdsa = (MspdSa(role) - mspdsa_dif) * ATTR_RADIX
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)

    ALLExAttrSet(role)
end

function SkillCooldown_PoJiaDan5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoJiaDan5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2797)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2797, 1)
end

function Skill_PoJiaDan5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PJ, statelv, statetime)

    Check_Ys_Rem(ATKER, DEFER)
end

function State_Pj_Add(role, statelv)
    local def_dif = 30
    local def = DefSb(role) - def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function State_Pj_Rem(role, statelv)
    local def_dif = 30
    local def = DefSb(role) + def_dif
    SetCharaAttr(def, role, ATTR_STATEV_DEF)
    ALLExAttrSet(role)
end

function SkillCooldown_PoRenDan5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_PoRenDan5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2798)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2798, 1)
end

function Skill_PoRenDan5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5

    local statelv = sklv
    local statetime = 10 + sklv
    dmg = math.random(400, 500) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
    AddState(ATKER, DEFER, STATE_PRD, statelv, statetime)
    Check_Ys_Rem(ATKER, DEFER)
end

function State_PoRenDan_Add(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) - atksb_dif
    local mxatksb = MxatkSb(role) - atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function State_PoRenDan_Rem(role, sklv)
    local statelv = sklv
    local atksb_dif = 50 * sklv
    local mnatksb = MnatkSb(role) + atksb_dif
    local mxatksb = MxatkSb(role) + atksb_dif
    SetCharaAttr(mnatksb, role, ATTR_STATEV_MNATK)
    SetCharaAttr(mxatksb, role, ATTR_STATEV_MXATK)
    ALLExAttrSet(role)
end

function SkillCooldown_RanShaoDan5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function Skill_RanShaoDan5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2799)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2799, 1)
end

function Skill_RanShaoDan5_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local sklv = 5

    local statelv = sklv
    local statetime = 4 + sklv * 2
    AddState(ATKER, DEFER, STATE_CZRSD, statelv, statetime)
    dmg = math.random(100, 200) * (1 + sklv * 0.5)
    Hp_Endure_Dmg(DEFER, dmg)
end

function State_RanShaoDan_Add(role, statelv)
    local hpdmg = 150
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end

function State_RanShaoDan_Rem(role, statelv)
end

function SkillCooldown_Xiaoxueqiu(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_Xiaoxueqiu_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local item_count = CheckBagItem(role, 2896)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end
function Skill_Xiaoxueqiu_End(ATKER, DEFER, sklv)
    local NocLock = KitbagLock(ATKER, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(ATKER, "Inventory has been binded")
        return 0
    end
    local statetime = 1
    local statelv = 10
    local rad_star = math.random(1, 20)
    if rad_star == 1 then
        AddState(ATKER, DEFER, STATE_XY, statelv, statetime)
    end
end

function SkillCooldown_FuShiZhiQiu1(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_FuShiZhiQiu1_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(atk_role)
        return 0
    end
    local item_count = CheckBagItem(atk_role, 1146)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 1146, 1)
end

function Skill_FuShiZhiQiu1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statelv = sklv
    local statetime = 5 + 4 * sklv
	if GetChaStateLv(DEFER, 176) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_FSZQ, statelv, statetime)
end

function State_FuShiZhiQiu_Add(role, statelv)
    local bd = GetChaAttr(role, ATTR_BPDEF)
    local defsa_dif = (-1) * (0.03 * statelv)
    local defsa = math.floor((DefSa(role) + defsa_dif) * ATTR_RADIX)
    local bd_dif = (-1) * (0.02 * statelv)
    local bd_fin = math.floor((ResistSa(role) + bd_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    SetCharaAttr(bd_fin, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
end

function State_FuShiZhiQiu_Rem(role, statelv)
    local bd = GetChaAttr(role, ATTR_BPDEF)
    local defsa_dif = (-1) * (0.03 * statelv)
    local defsa = math.floor((DefSa(role) - defsa_dif) * ATTR_RADIX)
    local bd_dif = (-1) * (0.02 * statelv)
    local bd_fin = math.floor((ResistSa(role) - bd_dif) * ATTR_RADIX)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    SetCharaAttr(bd_fin, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
end

function SkillCooldown_FuShiZhiQiu2(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_FuShiZhiQiu2_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(atk_role)
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2730)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2730, 1)
end

function Skill_FuShiZhiQiu2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statelv = sklv
    local statetime = 5 + 4 * sklv
	if GetChaStateLv(DEFER, 176) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_FSZQ, statelv, statetime)
end

function SkillCooldown_FuShiZhiQiu3(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_FuShiZhiQiu3_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(atk_role)
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2754)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2754, 1)
end

function Skill_FuShiZhiQiu3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statelv = sklv
    local statetime = 5 + 4 * sklv
	if GetChaStateLv(DEFER, 176) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_FSZQ, statelv, statetime)
end

function SkillCooldown_FuShiZhiQiu4(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_FuShiZhiQiu4_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(atk_role)
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2778)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2778, 1)
end

function Skill_FuShiZhiQiu4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statelv = sklv
    local statetime = 5 + 4 * sklv
	if GetChaStateLv(DEFER, 176) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_FSZQ, statelv, statetime)
end

function SkillCooldown_FuShiZhiQiu5(sklv)
    local Cooldown = 5000
    return Cooldown
end

function Skill_FuShiZhiQiu5_Begin(role, sklv)
    local atk_role = TurnToCha(role)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(atk_role)
        return 0
    end
    local item_count = CheckBagItem(atk_role, 2802)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2802, 1)
end

function Skill_FuShiZhiQiu5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statelv = sklv
    local statetime = 5 + 4 * statelv
	if GetChaStateLv(DEFER, 176) > 0 then
		return 0
	end
    AddState(ATKER, DEFER, STATE_FSZQ, statelv, statetime)
end

function SkillCooldown_ZaoYinZhiZao1(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_ZaoYinZhiZao1(sklv)
    local sklv = 1
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_ZaoYinZhiZao1(sklv)
    local sklv = 1
    local statetime = 15 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_ZYZZ, statelv, statetime)
end

function Skill_ZaoYinZhiZao1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 1147)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_ZaoYinZhiZao1_End(ATKER, DEFER, sklv)
end

function State_ZaoYinZhiZao_Add(role, statelv)
    local sp_sum = -15 * statelv
    local sp = GetChaAttr(role, ATTR_SP)
    sp = sp + sp_sum
    if sp < 0 then
        sp = 0
    end
    SetCharaAttr(sp, role, ATTR_SP)
    ALLExAttrSet(role)
end

function State_ZaoYinZhiZao_Rem(role, statelv)
end

function State_ZaoYinZhiZao_Tran(statelv)
    return 1
end

function SkillCooldown_ZaoYinZhiZao2(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_ZaoYinZhiZao2(sklv)
    local sklv = 2
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_ZaoYinZhiZao2(sklv)
    local sklv = 2
    local statetime = 15 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_ZYZZ, statelv, statetime)
end

function Skill_ZaoYinZhiZao2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2731)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_ZaoYinZhiZao2_End(ATKER, DEFER, sklv)
end

function SkillCooldown_ZaoYinZhiZao3(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_ZaoYinZhiZao3(sklv)
    local sklv = 1
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_ZaoYinZhiZao3(sklv)
    local sklv = 3
    local statetime = 15 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_ZYZZ, statelv, statetime)
end

function Skill_ZaoYinZhiZao3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2755)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_ZaoYinZhiZao3_End(ATKER, DEFER, sklv)
end

function SkillCooldown_ZaoYinZhiZao4(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_ZaoYinZhiZao4(sklv)
    local sklv = 1
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_ZaoYinZhiZao4(sklv)
    local sklv = 4
    local statetime = 15 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_ZYZZ, statelv, statetime)
end

function Skill_ZaoYinZhiZao4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2779)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_ZaoYinZhiZao4_End(ATKER, DEFER, sklv)
end

function SkillCooldown_ZaoYinZhiZao5(sklv)
    local Cooldown = 1500
    return Cooldown
end

function SkillArea_Square_ZaoYinZhiZao5(sklv)
    local sklv = 5
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_ZaoYinZhiZao5(sklv)
    local sklv = 5
    local statetime = 15 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_ZYZZ, statelv, statetime)
end

function Skill_ZaoYinZhiZao5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2803)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_ZaoYinZhiZao5_End(ATKER, DEFER, sklv)
end

function SkillCooldown_DiZhenFaSheng1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function SkillArea_Circle_DiZhenFaSheng1(sklv)
    local sklv = 1
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_DiZhenFaSheng1(sklv)
    local sklv = 1
    local statetime = 10 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_DZFS, statelv, statetime)
end

function Skill_DiZhenFaSheng1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 1148)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_DiZhenFaSheng1_End(ATKER, DEFER, sklv)
end

function State_DiZhenFaSheng_Add(role, statelv)
    local mspdsa_dif = (-1) * (0.2 * statelv + 0.1)
    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_DiZhenFaSheng_Rem(role, statelv)
    local mspdsa_dif = (-1) * (0.2 * statelv + 0.1)
    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_DiZhenFaSheng_Tran(statelv)
    return 1
end

function SkillCooldown_DiZhenFaSheng2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function SkillArea_Circle_DiZhenFaSheng2(sklv)
    local sklv = 2
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_DiZhenFaSheng2(sklv)
    local sklv = 2
    local statetime = 10 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_DZFS, statelv, statetime)
end

function Skill_DiZhenFaSheng2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2732)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_DiZhenFaSheng2_End(ATKER, DEFER, sklv)
end

function SkillCooldown_DiZhenFaSheng3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function SkillArea_Circle_DiZhenFaSheng3(sklv)
    local sklv = 3
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_DiZhenFaSheng3(sklv)
    local sklv = 3
    local statetime = 10 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_DZFS, statelv, statetime)
end

function Skill_DiZhenFaSheng3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2756)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_DiZhenFaSheng3_End(ATKER, DEFER, sklv)
end

function SkillCooldown_DiZhenFaSheng4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function SkillArea_Circle_DiZhenFaSheng4(sklv)
    local sklv = 4
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_DiZhenFaSheng4(sklv)
    local sklv = 4
    local statetime = 10 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_DZFS, statelv, statetime)
end

function Skill_DiZhenFaSheng4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2780)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_DiZhenFaSheng4_End(ATKER, DEFER, sklv)
end

function SkillCooldown_DiZhenFaSheng5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function SkillArea_Circle_DiZhenFaSheng5(sklv)
    local sklv = 5
    local side = 550 + sklv * 40
    SetSkillRange(4, side)
end

function SkillArea_State_DiZhenFaSheng5(sklv)
    local sklv = 5
    local statetime = 10 + sklv * 2
    local statelv = sklv
    SetRangeState(STATE_DZFS, statelv, statetime)
end

function Skill_DiZhenFaSheng5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local item_count = CheckBagItem(role, 2804)
    if item_count <= 0 then
        SkillUnable(role)
        SystemNotice(role, "Does not possess required item to use skill")
    end
end

function Skill_DiZhenFaSheng5_End(ATKER, DEFER, sklv)
end

function SkillCooldown_LianDan1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_LianDan1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2677)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2677, 1)
end

function Skill_LianDan1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_LD, statelv, statetime)
end

function State_LianDan_Add(role, statelv)
    local mspdsa_dif = 0.3 + 0.09 * statelv

    local mspdsa = math.floor((MspdSa(role) - mspdsa_dif) * ATTR_RADIX)

    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function State_LianDan_Rem(role, statelv)
    local mspdsa_dif = 0.3 + 0.09 * statelv

    local mspdsa = math.floor((MspdSa(role) + mspdsa_dif) * ATTR_RADIX)
    SetCharaAttr(mspdsa, role, ATTR_STATEC_MSPD)
    ALLExAttrSet(role)
end

function SkillCooldown_LianDan2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_LianDan2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2741)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2741, 1)
end

function Skill_LianDan2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_LD, statelv, statetime)
end

function SkillCooldown_LianDan3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_LianDan3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2765)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2765, 1)
end

function Skill_LianDan3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_LD, statelv, statetime)
end

function SkillCooldown_LianDan4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_LianDan4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2789)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2789, 1)
end

function Skill_LianDan4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_LD, statelv, statetime)
end

function SkillCooldown_LianDan5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_LianDan5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2813)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2813, 1)
end

function Skill_LianDan5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statelv = sklv
    local statetime = 30 + sklv * 20
    AddState(ATKER, DEFER, STATE_LD, statelv, statetime)
end

function SkillCooldown_HuanYinFaSheng1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_HuanYinFaSheng1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2673)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return 0
    end
    local a = DelBagItem(atk_role, 2673, 1)
end

function Skill_HuanYinFaSheng1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statelv = sklv
    local statetime = 2 + sklv * 3
    AddState(ATKER, DEFER, STATE_HYFS, statelv, statetime)
end
function State_HuanYinFaSheng_Add(role, statelv)
end
function State_HuanYinFaSheng_Rem(role, statelv)
end

function SkillCooldown_HuanYinFaSheng2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_HuanYinFaSheng2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2737)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return 0
    end
    local a = DelBagItem(atk_role, 2737, 1)
end

function Skill_HuanYinFaSheng2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statelv = sklv
    local statetime = 2 + sklv * 3
    AddState(ATKER, DEFER, STATE_HYFS, statelv, statetime)
end

function SkillCooldown_HuanYinFaSheng3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_HuanYinFaSheng3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2761)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return 0
    end
    local a = DelBagItem(atk_role, 2761, 1)
end

function Skill_HuanYinFaSheng3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statelv = sklv
    local statetime = 2 + sklv * 3
    AddState(ATKER, DEFER, STATE_HYFS, statelv, statetime)
end

function SkillCooldown_HuanYinFaSheng4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_HuanYinFaSheng4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2785)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return 0
    end
    local a = DelBagItem(atk_role, 2785, 1)
end

function Skill_HuanYinFaSheng4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statelv = sklv
    local statetime = 2 + sklv * 3
    AddState(ATKER, DEFER, STATE_HYFS, statelv, statetime)
end

function SkillCooldown_HuanYinFaSheng5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_HuanYinFaSheng5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2809)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return 0
    end
    local a = DelBagItem(atk_role, 2809, 1)
end

function Skill_HuanYinFaSheng5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statelv = sklv
    local statetime = 2 + sklv * 3
    AddState(ATKER, DEFER, STATE_HYFS, statelv, statetime)
end

function SkillCooldown_ChuanZhiQianXing1(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_ChuanZhiQianXing1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2675)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2675, 1)
end
function Skill_ChuanZhiQianXing1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statelv = sklv
    local statetime = 5 + sklv * 15
    AddState(ATKER, DEFER, STATE_CZQX, statelv, statetime)
end

function State_ChuanZhiQianXing_Add(role, statelv)
end
function State_ChuanZhiQianXing_Rem(role, statelv)
end

function SkillCooldown_ChuanZhiQianXing2(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_ChuanZhiQianXing2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2739)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2739, 1)
end
function Skill_ChuanZhiQianXing2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statelv = sklv
    local statetime = 5 + sklv * 15
    AddState(ATKER, DEFER, STATE_CZQX, statelv, statetime)
end

function SkillCooldown_ChuanZhiQianXing3(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_ChuanZhiQianXing3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2763)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2763, 1)
end
function Skill_ChuanZhiQianXing3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statelv = sklv
    local statetime = 5 + sklv * 15
    AddState(ATKER, DEFER, STATE_CZQX, statelv, statetime)
end

function SkillCooldown_ChuanZhiQianXing4(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_ChuanZhiQianXing4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2787)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2787, 1)
end
function Skill_ChuanZhiQianXing4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statelv = sklv
    local statetime = 5 + sklv * 15
    AddState(ATKER, DEFER, STATE_CZQX, statelv, statetime)
end

function SkillCooldown_ChuanZhiQianXing5(sklv)
    local Cooldown = 3000
    return Cooldown
end
function Skill_ChuanZhiQianXing5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2811)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2811, 1)
end
function Skill_ChuanZhiQianXing5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statelv = sklv
    local statetime = 5 + sklv * 15
    AddState(ATKER, DEFER, STATE_CZQX, statelv, statetime)
end

function SkillCooldown_LeiDa1(sklv)
    local Cooldown = 3000
    return Cooldown
end
function SkillArea_Square_LeiDa1(sklv)
    local sklv = 1
    local side = 50 + sklv * 300
    SetSkillRange(3, side)
end
function SkillArea_State_LeiDa1(sklv)
    local sklv = 1
    local statetime = 15 + sklv * 55
    local statelv = sklv
    SetRangeState(STATE_LEIDA, statelv, statetime)
end
function Skill_LeiDa1_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SkillUnable(role)
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2676)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2676, 1)
end
function Skill_LeiDa1_End(ATKER, DEFER, sklv)
end
function State_LeiDa_Add(role, statelv)
end
function State_LeiDa_Rem(role, statelv)
end
function State_LeiDa_Tran(statelv)
    return 1
end

function SkillCooldown_LeiDa2(sklv)
    local Cooldown = 3000
    return Cooldown
end
function SkillArea_Square_LeiDa2(sklv)
    local sklv = 2
    local side = 50 + sklv * 300
    SetSkillRange(3, side)
end
function SkillArea_State_LeiDa2(sklv)
    local sklv = 2
    local statetime = 15 + sklv * 55
    local statelv = sklv
    SetRangeState(STATE_LEIDA, statelv, statetime)
end
function Skill_LeiDa2_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SkillUnable(role)
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2740)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2740, 1)
end
function Skill_LeiDa2_End(ATKER, DEFER, sklv)
end

function SkillCooldown_LeiDa3(sklv)
    local Cooldown = 3000
    return Cooldown
end
function SkillArea_Square_LeiDa3(sklv)
    local sklv = 3
    local side = 50 + sklv * 300
    SetSkillRange(3, side)
end
function SkillArea_State_LeiDa3(sklv)
    local sklv = 3
    local statetime = 15 + sklv * 55
    local statelv = sklv
    SetRangeState(STATE_LEIDA, statelv, statetime)
end
function Skill_LeiDa3_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SkillUnable(role)
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2764)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2764, 1)
end
function Skill_LeiDa3_End(ATKER, DEFER, sklv)
end

function SkillCooldown_LeiDa4(sklv)
    local Cooldown = 3000
    return Cooldown
end
function SkillArea_Square_LeiDa4(sklv)
    local sklv = 4
    local side = 50 + sklv * 300
    SetSkillRange(3, side)
end
function SkillArea_State_LeiDa4(sklv)
    local sklv = 4
    local statetime = 15 + sklv * 55
    local statelv = sklv
    SetRangeState(STATE_LEIDA, statelv, statetime)
end
function Skill_LeiDa4_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SkillUnable(role)
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2788)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2788, 1)
end
function Skill_LeiDa4_End(ATKER, DEFER, sklv)
end

function SkillCooldown_LeiDa5(sklv)
    local Cooldown = 3000
    return Cooldown
end
function SkillArea_Square_LeiDa5(sklv)
    local sklv = 5
    local side = 50 + sklv * 300
    SetSkillRange(3, side)
end
function SkillArea_State_LeiDa5(sklv)
    local sklv = 5
    local statetime = 15 + sklv * 55
    local statelv = sklv
    SetRangeState(STATE_LEIDA, statelv, statetime)
end
function Skill_LeiDa5_Begin(role, sklv)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SkillUnable(role)
        SystemNotice(role, "Inventory has been binded")
        return 0
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2812)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
    end
    local a = DelBagItem(atk_role, 2812, 1)
end
function Skill_LeiDa5_End(ATKER, DEFER, sklv)
end

function SkillCooldown_ChuanTiXiuFu1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ChuanTiXiuFu1_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1150)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 1150, 1)
end
function Skill_ChuanTiXiuFu1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local hpdmg = -math.random(450, 650) * (sklv * 1.5)
    Hp_Endure_Dmg(DEFER, hpdmg)
    ALLExAttrSet(DEFER)
end
function State_ChuanTiXiuFu_Add(role, statelv)
end
function State_ChuanTiXiuFu_Rem(role, statelv)
end

function SkillCooldown_ChuanTiXiuFu2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ChuanTiXiuFu2_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2734)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2734, 1)
end
function Skill_ChuanTiXiuFu2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local hpdmg = -math.random(450, 650) * (sklv * 1.5)
    Hp_Endure_Dmg(DEFER, hpdmg)
    ALLExAttrSet(role)
end

function SkillCooldown_ChuanTiXiuFu3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ChuanTiXiuFu3_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2758)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2758, 1)
end
function Skill_ChuanTiXiuFu3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local hpdmg = -math.random(450, 650) * (sklv * 1.5)
    Hp_Endure_Dmg(DEFER, hpdmg)
    ALLExAttrSet(role)
end

function SkillCooldown_ChuanTiXiuFu4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ChuanTiXiuFu4_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2782)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2782, 1)
end
function Skill_ChuanTiXiuFu4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local hpdmg = -math.random(450, 650) * (sklv * 1.5)
    Hp_Endure_Dmg(DEFER, hpdmg)
    ALLExAttrSet(role)
end

function SkillCooldown_ChuanTiXiuFu5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ChuanTiXiuFu5_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2806)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2806, 1)
end
function Skill_ChuanTiXiuFu5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local hpdmg = -math.random(450, 650) * (sklv * 1.5)
    Hp_Endure_Dmg(DEFER, hpdmg)
    ALLExAttrSet(role)
end

function SkillCooldown_ShiWuZaiSheng1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ShiWuZaiSheng1_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1151)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 1151, 1)
end

function Skill_ShiWuZaiSheng1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local sp_sum = 650 * sklv
    local sp = GetChaAttr(DEFER, ATTR_SP)
    sp = sp + sp_sum
    mxsp = GetChaAttr(DEFER, ATTR_MXSP)
    if sp > mxsp then
        sp = mxsp
    end
    SetCharaAttr(sp, DEFER, ATTR_SP)
end

function State_ShiWuZaiSheng_Add(role, statelv)
end
function State_ShiWuZaiSheng_Rem(role, statelv)
end

function SkillCooldown_ShiWuZaiSheng2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ShiWuZaiSheng2_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2735)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2735, 1)
end

function Skill_ShiWuZaiSheng2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local sp_sum = 650 * sklv
    local sp = GetChaAttr(DEFER, ATTR_SP)
    sp = sp + sp_sum
    mxsp = GetChaAttr(DEFER, ATTR_MXSP)
    if sp > mxsp then
        sp = mxsp
    end
    SetCharaAttr(sp, DEFER, ATTR_SP)
end

function SkillCooldown_ShiWuZaiSheng3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ShiWuZaiSheng3_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2759)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2759, 1)
end

function Skill_ShiWuZaiSheng3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local sp_sum = 650 * sklv
    local sp = GetChaAttr(DEFER, ATTR_SP)
    sp = sp + sp_sum
    mxsp = GetChaAttr(DEFER, ATTR_MXSP)
    if sp > mxsp then
        sp = mxsp
    end
    SetCharaAttr(sp, DEFER, ATTR_SP)
end

function SkillCooldown_ShiWuZaiSheng4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ShiWuZaiSheng4_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2783)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2783, 1)
end

function Skill_ShiWuZaiSheng4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local sp_sum = 650 * sklv
    local sp = GetChaAttr(DEFER, ATTR_SP)
    sp = sp + sp_sum
    mxsp = GetChaAttr(DEFER, ATTR_MXSP)
    if sp > mxsp then
        sp = mxsp
    end
    SetCharaAttr(sp, DEFER, ATTR_SP)
end

function SkillCooldown_ShiWuZaiSheng5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_ShiWuZaiSheng5_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2807)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2807, 1)
end

function Skill_ShiWuZaiSheng5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local sp_sum = 650 * sklv
    local sp = GetChaAttr(DEFER, ATTR_SP)
    sp = sp + sp_sum
    mxsp = GetChaAttr(DEFER, ATTR_MXSP)
    if sp > mxsp then
        sp = mxsp
    end
    SetCharaAttr(sp, DEFER, ATTR_SP)
end

function SkillCooldown_FuShiDan1(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_FuShiDan1_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 1152)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 1152, 1)
end

function Skill_FuShiDan1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statelv = sklv
    local statetime = 2 + sklv * 8
    AddState(ATKER, DEFER, STATE_FSD, statelv, statetime)
end

function State_FuShiDan_Add(role, statelv)
    local sp_sum = -80
    local sp = GetChaAttr(role, ATTR_SP)
    sp = sp + sp_sum
    if sp < 0 then
        sp = 0
    end
    SetCharaAttr(sp, role, ATTR_SP)
    ALLExAttrSet(role)
end

function State_FuShiDan_Rem(role, statelv)
end

function SkillCooldown_FuShiDan2(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_FuShiDan2_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2736)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2736, 1)
end

function Skill_FuShiDan2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statelv = sklv
    local statetime = 2 + sklv * 8
    AddState(ATKER, DEFER, STATE_FSD, statelv, statetime)
end

function SkillCooldown_FuShiDan3(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_FuShiDan3_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2760)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2760, 1)
end

function Skill_FuShiDan3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statelv = sklv
    local statetime = 2 + sklv * 8
    AddState(ATKER, DEFER, STATE_FSD, statelv, statetime)
end

function SkillCooldown_FuShiDan4(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_FuShiDan4_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2784)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2784, 1)
end

function Skill_FuShiDan4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statelv = sklv
    local statetime = 2 + sklv * 8
    AddState(ATKER, DEFER, STATE_FSD, statelv, statetime)
end

function SkillCooldown_FuShiDan5(sklv)
    local Cooldown = 3000
    return Cooldown
end

function Skill_FuShiDan5_Begin(role, Item)
    local NocLock = KitbagLock(role, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(role, "Inventory has been binded")
        SkillUnable(role)
        return
    end
    local atk_role = TurnToCha(role)
    local item_count = CheckBagItem(atk_role, 2808)
    if item_count <= 0 then
        SkillUnable(atk_role)
        SystemNotice(atk_role, "Does not possess required item to use skill")
        return
    end
    local a = DelBagItem(atk_role, 2808, 1)
end

function Skill_FuShiDan5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statelv = sklv
    local statetime = 2 + sklv * 8
    AddState(ATKER, DEFER, STATE_FSD, statelv, statetime)
end

function SkillArea_Circle_Czsl1(sklv)
    local sklv = 1
    local side = 650 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Czsl1(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Czsl1_Begin(role, sklv)
end

function Skill_Czsl1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statetime = sklv + 3
    local statelv = sklv
    if ValidCha(ATKER) == 0 then
        return
    end
    if ValidCha(DEFER) == 0 then
        return
    end
    local dmg = 20
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(4, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Slrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Slrs, statelv, statetime)
end
function State_Slrs_Add(role, statelv)
    local hpdmg = statelv * 2
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end
function State_Slrs_Rem(role, statelv)
end
function State_Slrs_Tran(statelv)
    return 1
end

function SkillArea_Circle_Czsl2(sklv)
    local sklv = 2
    local side = 650 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Czsl2(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Czsl2_Begin(role, sklv)
end

function Skill_Czsl2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statetime = sklv + 3
    local statelv = sklv
    if ValidCha(ATKER) == 0 then
        return
    end
    if ValidCha(DEFER) == 0 then
        return
    end
    local dmg = 40
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(8, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Slrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Slrs, statelv, statetime)
end

function SkillArea_Circle_Czsl3(sklv)
    local sklv = 3
    local side = 650 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Czsl3(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Czsl3_Begin(role, sklv)
end

function Skill_Czsl3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statetime = sklv + 3
    local statelv = sklv
    if ValidCha(ATKER) == 0 then
        return
    end
    if ValidCha(DEFER) == 0 then
        return
    end
    local dmg = 60
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(12, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Slrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Slrs, statelv, statetime)
end

function SkillArea_Circle_Czsl4(sklv)
    local sklv = 4
    local side = 650 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Czsl4(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Czsl4_Begin(role, sklv)
end

function Skill_Czsl4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statetime = sklv + 3
    local statelv = sklv
    if ValidCha(ATKER) == 0 then
        return
    end
    if ValidCha(DEFER) == 0 then
        return
    end
    local dmg = 80
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(16, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Slrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Slrs, statelv, statetime)
end

function SkillArea_Circle_Czsl5(sklv)
    local sklv = 5
    local side = 650 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Czsl5(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Czsl5_Begin(role, sklv)
end

function Skill_Czsl5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statetime = sklv + 3
    local statelv = sklv

    if ValidCha(ATKER) == 0 then
        return
    end
    if ValidCha(DEFER) == 0 then
        return
    end
    local dmg = 100
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(20, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Slrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Slrs, statelv, statetime)
end

function SkillSp_Myzb1(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_Circle_Myzb1(sklv)
    local sklv = 1
    local side = 550 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Myzb1(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Myzb1_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Myzb1(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Myzb1_End(ATKER, DEFER, sklv)
    local sklv = 1
    local statetime = sklv + 3
    local statelv = sklv
    local dmg = 20
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(2, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Myrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Myrs, statelv, statetime)
end

function State_Myrs_Add(role, statelv)
    local hpdmg = statelv * 2
    Hp_Endure_Dmg(role, hpdmg)
    ALLExAttrSet(role)
end
function State_Myrs_Rem(role, statelv)
end
function State_Myrs_Tran(statelv)
    return 1
end

function SkillSp_Myzb2(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_Circle_Myzb2(sklv)
    local sklv = 2
    local side = 550 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Myzb2(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Myzb2_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Myzb1(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Myzb2_End(ATKER, DEFER, sklv)
    local sklv = 2
    local statetime = sklv + 3
    local statelv = sklv
    local dmg = 40
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(8, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Myrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Myrs, statelv, statetime)
end

function SkillSp_Myzb3(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_Circle_Myzb3(sklv)
    local sklv = 3
    local side = 550 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Myzb3(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Myzb3_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Myzb1(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Myzb3_End(ATKER, DEFER, sklv)
    local sklv = 3
    local statetime = sklv + 3
    local statelv = sklv
    local dmg = 60
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(12, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Myrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Myrs, statelv, statetime)
end

function SkillSp_Myzb4(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_Circle_Myzb4(sklv)
    local sklv = 4
    local side = 550 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Myzb4(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Myzb4_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Myzb1(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Myzb4_End(ATKER, DEFER, sklv)
    local sklv = 4
    local statetime = sklv + 3
    local statelv = sklv
    local dmg = 80
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(16, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Myrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Myrs, statelv, statetime)
end

function SkillSp_Myzb5(sklv)
    local sp_reduce = 20
    return sp_reduce
end

function SkillArea_Circle_Myzb5(sklv)
    local sklv = 5
    local side = 400 + math.floor(sklv * 50)
    SetSkillRange(4, side)
end

function SkillCooldown_Myzb5(sklv)
    local Cooldown = 6000
    return Cooldown
end

function Skill_Myzb5_Begin(role, sklv)
    local sp = Sp(role)
    local sp_reduce = SkillSp_Myzb1(sklv)
    if sp - sp_reduce < 0 then
        SkillUnable(role)
        return
    end
    Sp_Red(role, sp_reduce)
end

function Skill_Myzb5_End(ATKER, DEFER, sklv)
    local sklv = 5
    local statetime = sklv + 3
    local statelv = sklv
    local dmg = 100
    Hp_Endure_Dmg(DEFER, dmg)
    SetCharaAttr(20, ATKER, ATTR_HP)
    AddState(ATKER, ATKER, STATE_Myrs, statelv, statetime)
    AddState(ATKER, DEFER, STATE_Myrs, statelv, statetime)
end

function SkillCooldown_ArrestWarrant(SkillLevel)
    local Cooldown = 5000
    return Cooldown
end
function Skill_ArrestWarrant_End(ATKER, DEFER, SkillLevel)
    local Name_ATKER = GetChaDefaultName(ATKER)
    local Name_DEFER = GetChaDefaultName(DEFER)

    if GetChaStateLv(DEFER, STATE_BAT) >= 1 or GetChaStateLv(DEFER, STATE_JY) >= 1 then
        SystemNotice(ATKER, "Cannot sendto Prison Island a player that has a stall or is trading.")
        return
    end
    if CheckBagItem(ATKER, 5717) <= 0 or KitbagLock(ATKER, 1) == LUA_TRUE then
        SystemNotice(ATKER, "You do not posses the Arrest Warrant or your inventory is locked.")
        return
    end
    local DeleteWarrant = DelBagItem(ATKER, 5717, 1)
    if DeleteWarrant == 1 then
        Notice("Player " .. Name_ATKER .. " has sent " .. Name_DEFER .. " to Prison Island for 60 minutes.")
        KickCha(Player)
    end
end

function State_CookingPotionHP_Add(Player, Level)
    local HP = (Level * Level) * 100
    local PR = 0
    if Level == 1 then
        PR = 1
    elseif Level == 2 or Level == 3 then
        PR = 2
    elseif Level == 4 or Level == 5 then
        PR = 3
    elseif Level == 6 then
        PR = 4
    elseif Level == 7 then
        PR = 5
    elseif Level == 8 then
        PR = 6
    elseif Level == 9 then
        PR = 7
    elseif Level == 10 then
        PR = 8
    end
    HP = GetChaAttr(Player, ATTR_STATEV_MXHP) + HP
    PR = GetChaAttr(Player, ATTR_STATEV_PDEF) + PR
    SetCharaAttr(HP, Player, ATTR_STATEV_MXHP)
    SetCharaAttr(PR, Player, ATTR_STATEV_PDEF)
    ALLExAttrSet(Player)
end
function State_CookingPotionHP_Rem(Player, Level)
    local HP = (Level * Level) * 100
    local PR = 0
    if Level == 1 then
        PR = 1
    elseif Level == 2 or Level == 3 then
        PR = 2
    elseif Level == 4 or Level == 5 then
        PR = 3
    elseif Level == 6 then
        PR = 4
    elseif Level == 7 then
        PR = 5
    elseif Level == 8 then
        PR = 6
    elseif Level == 9 then
        PR = 7
    elseif Level == 10 then
        PR = 8
    end
    HP = GetChaAttr(Player, ATTR_STATEV_MXHP) - HP
    PR = GetChaAttr(Player, ATTR_STATEV_PDEF) - PR
    SetCharaAttr(HP, Player, ATTR_STATEV_MXHP)
    SetCharaAttr(PR, Player, ATTR_STATEV_PDEF)
    ALLExAttrSet(Player)
end
function State_CookingPotionATK_Add(Player, Level)
    local HP = GetChaAttr(Player, ATTR_HP) + (Level * 200)
    local ATK = 50 + (Level - 1) * 100
    if HP > GetChaAttr(Player, ATTR_MXHP) then
        HP = GetChaAttr(Player, ATTR_MXHP)
    end

    local MnATK = GetChaAttr(Player, ATTR_STATEV_MNATK) + ATK
    local MXATK = GetChaAttr(Player, ATTR_STATEV_MXATK) + ATK

    SetCharaAttr(HP, Player, ATTR_HP)
    SetCharaAttr(MnATK, Player, ATTR_STATEV_MNATK)
    SetCharaAttr(MXATK, Player, ATTR_STATEV_MXATK)
    ALLExAttrSet(Player)
end
function State_CookingPotionATK_Rem(Player, Level)
    local ATK = 50 + (Level - 1) * 100

    local MnATK = GetChaAttr(Player, ATTR_STATEV_MNATK) - ATK
    local MXATK = GetChaAttr(Player, ATTR_STATEV_MXATK) - ATK

    SetCharaAttr(MnATK, Player, ATTR_STATEV_MNATK)
    SetCharaAttr(MXATK, Player, ATTR_STATEV_MXATK)
    ALLExAttrSet(Player)
end
function State_CookingPotionSPR_Add(Player, Level)
    local SP = GetChaAttr(Player, ATTR_SP) + (Level * 100)
    local SPR = Level * 5

    if SP > GetChaAttr(Player, ATTR_MXSP) then
        SP = GetChaAttr(Player, ATTR_MXSP)
    end
    SPR = GetChaAttr(Player, ATTR_STATEV_STA) + SPR

    SetCharaAttr(SPR, Player, ATTR_STATEV_STA)
    SetCharaAttr(SP, Player, ATTR_SP)
    ALLExAttrSet(Player)
end
function State_CookingPotionSPR_Rem(Player, Level)
    local SPR = Level * 5
    SPR = GetChaAttr(Player, ATTR_STATEV_STA) - SPR
    SetCharaAttr(SPR, Player, ATTR_STATEV_STA)
    if GetChaAttr(Player, ATTR_SP) > GetChaAttr(Player, ATTR_MXSP) then
        SetCharaAttr(GetChaAttr(Player, ATTR_MXSP), Player, ATTR_SP)
    end
    ALLExAttrSet(Player)
end

function Skill_Xs_Begin(role, sklv)
end

function Skill_Xs_End(ATKER, DEFER, sklv)
    hpdmg = 75
    Hp_Endure_Dmg(DEFER, hpdmg)
    Check_Ys_Rem(ATKER, DEFER)
end

function SkillCooldown_Xs(sklv)
    local Cooldown = 10000
    return Cooldown
end

function State_GameMaster_Add(Player)
    local AttackVal = 100000
    local DefenseVal = 10000
    local HitVal = 1000
    local DodgeVal = 1000
    local HPVal = 1000000
    local SPVal = 1000000
    local SpeedVal = 5000
    local PhysicalVal = 1000
    local MovementVal = 1500

    local AttackMin = MnatkSb(Player) + AttackVal
    local AttackMax = MxatkSb(Player) + AttackVal
    local Defense = DefSb(Player) + DefenseVal
    local Hit = HitSb(Player) + HitVal
    local Dodge = FleeSb(Player) + DodgeVal
    local HP = MxhpSb(Player) + HPVal
    local SP = MxspSb(Player) + SPVal
    local Speed = AspdSb(Player) + SpeedVal
    local PhysicalResist = ResistSb(Player) + PhysicalVal
    local Movement = MspdSb(Player) + MovementVal

    SetCharaAttr(AttackMin, Player, ATTR_STATEV_MNATK)
    SetCharaAttr(AttackMax, Player, ATTR_STATEV_MXATK)
    SetCharaAttr(Defense, Player, ATTR_STATEV_DEF)
    SetCharaAttr(Hit, Player, ATTR_STATEV_HIT)
    SetCharaAttr(Dodge, Player, ATTR_STATEV_FLEE)
    SetCharaAttr(HP, Player, ATTR_STATEV_MXHP)
    SetCharaAttr(SP, Player, ATTR_STATEV_MXSP)
    SetCharaAttr(Speed, Player, ATTR_STATEV_ASPD)
    SetCharaAttr(PhysicalResist, Player, ATTR_STATEV_PDEF)
    SetCharaAttr(Movement, Player, ATTR_STATEV_MSPD)

    ALLExAttrSet(Player)
end

function State_GameMaster_Rem(Player)
    local AttackVal = 100000
    local DefenseVal = 10000
    local HitVal = 1000
    local DodgeVal = 1000
    local HPVal = 1000000
    local SPVal = 1000000
    local SpeedVal = 5000
    local PhysicalVal = 1000
    local MovementVal = 1500

    local AttackMin = MnatkSb(Player) - AttackVal
    local AttackMax = MxatkSb(Player) - AttackVal
    local Defense = DefSb(Player) - DefenseVal
    local Hit = HitSb(Player) - HitVal
    local Dodge = FleeSb(Player) - DodgeVal
    local HP = MxhpSb(Player) - HPVal
    local SP = MxspSb(Player) - SPVal
    local Speed = AspdSb(Player) - SpeedVal
    local PhysicalResist = ResistSb(Player) - PhysicalVal
    local Movement = MspdSb(Player) - MovementVal

    SetCharaAttr(AttackMin, Player, ATTR_STATEV_MNATK)
    SetCharaAttr(AttackMax, Player, ATTR_STATEV_MXATK)
    SetCharaAttr(Defense, Player, ATTR_STATEV_DEF)
    SetCharaAttr(Hit, Player, ATTR_STATEV_HIT)
    SetCharaAttr(Dodge, Player, ATTR_STATEV_FLEE)
    SetCharaAttr(HP, Player, ATTR_STATEV_MXHP)
    SetCharaAttr(SP, Player, ATTR_STATEV_MXSP)
    SetCharaAttr(Speed, Player, ATTR_STATEV_ASPD)
    SetCharaAttr(PhysicalResist, Player, ATTR_STATEV_PDEF)
    SetCharaAttr(Movement, Player, ATTR_STATEV_MSPD)

    ALLExAttrSet(Player)
end

function Skill_Rbmp_Use(role, sklv)
    local statelv = sklv
    local rb_bonus = (-1) * (0.050 + 0.005 * statelv)
    local mnatksa = math.floor((MnatkSa(role) - rb_bonus) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) - rb_bonus) * ATTR_RADIX)
    local defsa = math.floor((DefSa(role) - rb_bonus) * ATTR_RADIX)
    local ResistSa = math.floor((ResistSa(role) - rb_bonus) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    SetCharaAttr(ResistSa, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
end

function Skill_Rbmp_Unuse(role, sklv)
    local statelv = sklv
    local rb_bonus = (-1) * (0.050 + 0.005 * statelv)
    local mnatksa = math.floor((MnatkSa(role) + rb_bonus) * ATTR_RADIX)
    local mxatksa = math.floor((MxatkSa(role) + rb_bonus) * ATTR_RADIX)
    local defsa = math.floor((DefSa(role) + rb_bonus) * ATTR_RADIX)
    local ResistSa = math.floor((ResistSa(role) + rb_bonus) * ATTR_RADIX)
    SetCharaAttr(mnatksa, role, ATTR_STATEC_MNATK)
    SetCharaAttr(mxatksa, role, ATTR_STATEC_MXATK)
    SetCharaAttr(defsa, role, ATTR_STATEC_DEF)
    SetCharaAttr(ResistSa, role, ATTR_STATEC_PDEF)
    ALLExAttrSet(role)
end

function Skill_Shenshenglazhu_End(ATKER, DEFER, sklv)
	local NocLock =	KitbagLock(ATKER, 0)
	if NocLock == LUA_FALSE then
		SystemNotice(ATKER , "Inventory has been bound")
		return 0
	end
	local Item_CanGet = GetChaFreeBagGridNum(ATKER)	
	if Item_CanGet <= 0 then
		SystemNotice(ATKER , "You do not have enough space, failed to use Holy Candle.")
		UseItemFailed(ATKER)
		return
	end
	local A = DelBagItem (ATKER, 3006, 1) 
	if A ~= 1 then
		SystemNotice(ATKER, "Delete Holy candles failed")
		return 0
	end
	GiveItem(ATKER, 0, 3007, 1 , 41) 
end

function Skill_NSDX_End(ATKER, DEFER, sklv)
	local NocLock =	KitbagLock(ATKER, 0)
	if NocLock == LUA_FALSE then
		SystemNotice(ATKER, "Inventory has been bound")
		return 0
	end
	local role_NSDX = GetChaItem2(ATKER, 2, 3010)
	local Energy_NSDX = GetItemAttr(role_NSDX, ITEMATTR_ENERGY)
	if Energy_NSDX < 999 then
		local Energy_NSDX = Energy_NSDX + 1
		SetItemAttr(role_NSDX , ITEMATTR_ENERGY, Energy_NSDX)
		RefreshCha(ATKER)
	end
end
