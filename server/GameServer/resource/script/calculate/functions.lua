print("-- [Loading] Functions")

MFRADIX = 100
CRTRADIX = 100
ATKER = 0
DEFER = 1

count_haidao = 0
count_haijun = 0
five_seconds = 0
second_five_seconds = 0
time_can_setmonster = 0
time_can_setnvsheng = 0
check_need_show = 0
create_boss_hj = 0
create_boss_hd = 0

count_haijun2 = 0
count_haidao2 = 0
five_seconds2 = 0
second_five_seconds2 = 0
time_can_setmonster2 = 0
time_can_setnvsheng2 = 0
check_need_show2 = 0
create_boss_hj2 = 0
create_boss_hd2 = 0

function get_repatriate_city_guildwar(Player)
    local map_name_role_guildwar = GetChaMapName(Player)
    return map_name_role_guildwar
end

function get_repatriate_city_guildwar2(Player)
    local map_name_role_guildwar2 = GetChaMapName(Player)
    return map_name_role_guildwar2
end

atk_statecheck = {}
def_statecheck = {}

function Reset_Statecheck()
    for i = 1, 100, 1 do
        atk_statecheck[i] = 0
        def_statecheck[i] = 0
    end
end

function EightyLv_ExpAdd(cha, expadd)
    if ValidCha(cha) == 1 then
        if ChaIsBoat(cha) == 0 then
            if Lv(cha) >= 80 then
                expadd = math.floor(expadd / 50)
            end
            if expadd == 0 then
                SystemNotice(TurnToCha(cha), "Distance is too far to obtain any EXP")
            end
        end
        exp = GetChaAttr(cha, ATTR_CEXP)
        exp = exp + expadd
        SetCharaAttr(exp, cha, ATTR_CEXP)
    end
end

function Check_State(atk_role, def_role)
    Reset_Statecheck()

    atk_statecheck[STATE_YS] = GetChaStateLv(atk_role, STATE_YS)
end

function CheckJobLegal(job)
    local check_job = 1
    if job < JOB_TYPE_XINSHOU or job > JOB_TYPE_GONGCHENGSHI then
        check_job = 0
    end
    return check_job
end

function RemoveYS(Player)
    RemoveState(Player, STATE_YS)
    return 1
end

function Rem_State_Unnormal(Player)
    RemoveState(Player, STATE_ZD)
    RemoveState(Player, STATE_MB)
    RemoveState(Player, STATE_ZZZH)
    RemoveState(Player, STATE_SYNZ)
    RemoveState(Player, STATE_SDBZ)
    RemoveState(Player, STATE_TJ)
    RemoveState(Player, STATE_SJ)

    RemoveState(Player, STATE_JNJZ)
    RemoveState(Player, STATE_GJJZ)
    RemoveState(Player, STATE_BDJ)
    RemoveState(Player, STATE_XN)
    RemoveState(Player, STATE_NT)
    RemoveState(Player, STATE_DIZ)
    RemoveState(Player, STATE_SWCX)
    RemoveState(Player, STATE_JSDD)
    RemoveState(Player, STATE_HYMH)
    RemoveState(Player, STATE_HLKJ)
    RemoveState(Player, STATE_HLLM)
    RemoveState(Player, STATE_CRXSF)
    RemoveState(Player, STATE_BlackHX)
    RemoveState(Player, STATE_HLKJ)
end

function Rem_State_StarUnnormal(Player)
    RemoveState(Player, STATE_KUANGZ)
    RemoveState(Player, STATE_QUANS)
    RemoveState(Player, STATE_QINGZ)
end
function Rem_State_NOSEA(Player)
    RemoveState(Player, STATE_KB)
    RemoveState(Player, STATE_XLZH)
    RemoveState(Player, STATE_PKJSYS)
    RemoveState(Player, STATE_PKSFYS)
    RemoveState(Player, STATE_TSHD)
    RemoveState(Player, STATE_FZLZ)
    RemoveState(Player, STATE_PKZDYS)
    RemoveState(Player, STATE_PKKBYS)
    RemoveState(Player, STATE_YSLLQH)
    RemoveState(Player, STATE_YSMJQH)
    RemoveState(Player, STATE_YSLQQH)
    RemoveState(Player, STATE_YSTZQH)
    RemoveState(Player, STATE_YSJSQH)
    RemoveState(Player, STATE_DENGLONG)
    RemoveState(Player, STATE_YSMspd)
    RemoveState(Player, STATE_PKSBYS)
    RemoveState(Player, STATE_KUANGZ)
    RemoveState(Player, STATE_QUANS)
    RemoveState(Player, STATE_QINGZ)
    RemoveState(Player, STATE_CJBBT)
    RemoveState(Player, STATE_JRQKL)
    RemoveState(Player, STATE_KALA)
    RemoveState(Player, STATE_CZZX)
    RemoveState(Player, STATE_JLFT1)
    RemoveState(Player, STATE_JLFT2)
    RemoveState(Player, STATE_JLFT3)
    RemoveState(Player, STATE_JLFT4)
    RemoveState(Player, STATE_JLFT5)
    RemoveState(Player, STATE_JLFT6)
    RemoveState(Player, STATE_JLFT7)
    RemoveState(Player, STATE_JLFT8)
end
function SetCharaAttr(a, b, c)
    local x, y = b, c
    local z = math.floor(a)

    SetChaAttr(x, y, z)
end

function Attr_ap(a)
    local attr_ap = GetChaAttr(a, ATTR_AP)
    return attr_ap
end

function Attr_tp(a)
    local attr_tp = GetChaAttr(a, ATTR_TP)
    return attr_tp
end

function CheckCha_Job(a)
    local role_attr_job = GetChaAttr(a, ATTR_JOB)
    return role_attr_job
end

function Exp(a)
    local exp = GetChaAttr(a, ATTR_CEXP)
    return exp
end

function Lv(a)
    local b = TurnToCha(a)
    local lv = GetChaAttr(b, ATTR_LV)
    return lv
end

function Hp(a)
    local hp = GetChaAttr(a, ATTR_HP)
    return hp
end

function Mxhp(a)
    local mxhp = GetChaAttr(a, ATTR_MXHP)
    return mxhp
end

function MxhpSa(a)
    local mxhpsa = GetChaAttr(a, ATTR_STATEC_MXHP) / ATTR_RADIX
    return mxhpsa
end

function MxhpSb(a)
    local mxhpsb = GetChaAttr(a, ATTR_STATEV_MXHP)
    return mxhpsb
end

function MxhpIa(a)
    local mxhpia = GetChaAttr(a, ATTR_ITEMC_MXHP) / ATTR_RADIX
    return mxhpia
end

function MxhpIb(a)
    local mxhpib = GetChaAttr(a, ATTR_ITEMV_MXHP)
    return mxhpib
end

function Sp(a)
    local sp = GetChaAttr(a, ATTR_SP)
    return sp
end

function Mxsp(a)
    local mxsp = GetChaAttr(a, ATTR_MXSP)
    return mxsp
end

function MxspSa(a)
    local mxspsa = GetChaAttr(a, ATTR_STATEC_MXSP) / ATTR_RADIX
    return mxspsa
end

function MxspSb(a)
    local mxspsb = GetChaAttr(a, ATTR_STATEV_MXSP)
    return mxspsb
end

function MxspIa(a)
    local mxspia = GetChaAttr(a, ATTR_ITEMC_MXSP) / ATTR_RADIX
    return mxspia
end

function MxspIb(a)
    local mxspib = GetChaAttr(a, ATTR_ITEMV_MXSP)
    return mxspib
end

function Mnatk(a)
    local mnatk = GetChaAttr(a, ATTR_MNATK)
    return mnatk
end

function MnatkSa(a)
    local mnatksa = GetChaAttr(a, ATTR_STATEC_MNATK) / ATTR_RADIX
    return mnatksa
end

function MnatkSb(a)
    local mnatksb = GetChaAttr(a, ATTR_STATEV_MNATK)
    return mnatksb
end

function MnatkIa(a)
    local mnatkia = GetChaAttr(a, ATTR_ITEMC_MNATK) / ATTR_RADIX
    return mnatkia
end

function MnatkIb(a)
    local mnatkib = GetChaAttr(a, ATTR_ITEMV_MNATK)
    return mnatkib
end

function Mxatk(a)
    local mxatk = GetChaAttr(a, ATTR_MXATK)
    return mxatk
end

function MxatkSa(a)
    local mxatksa = GetChaAttr(a, ATTR_STATEC_MXATK) / ATTR_RADIX
    return mxatksa
end

function MxatkSb(a)
    local mxatksb = GetChaAttr(a, ATTR_STATEV_MXATK)
    return mxatksb
end

function MxatkIa(a)
    local mxatkia = GetChaAttr(a, ATTR_ITEMC_MXATK) / ATTR_RADIX
    return mxatkia
end

function MxatkIb(a)
    local mxatkib = GetChaAttr(a, ATTR_ITEMV_MXATK)
    return mxatkib
end

function Def(a)
    local def = GetChaAttr(a, ATTR_DEF)
    return def
end

function DefSa(a)
    local defsa = GetChaAttr(a, ATTR_STATEC_DEF) / ATTR_RADIX
    return defsa
end

function DefSb(a)
    local defsb = GetChaAttr(a, ATTR_STATEV_DEF)

    return defsb
end

function DefIa(a)
    local defia = GetChaAttr(a, ATTR_ITEMC_DEF) / ATTR_RADIX
    return defia
end

function DefIb(a)
    local defib = GetChaAttr(a, ATTR_ITEMV_DEF)
    return defib
end

function Resist(a)
    local def = GetChaAttr(a, ATTR_PDEF)
    return def
end

function ResistSa(a)
    local defsa = GetChaAttr(a, ATTR_STATEC_PDEF) / ATTR_RADIX
    return defsa
end

function ResistSb(a)
    local defsb = GetChaAttr(a, ATTR_STATEV_PDEF)
    return defsb
end

function ResistIa(a)
    local defia = GetChaAttr(a, ATTR_ITEMC_PDEF) / ATTR_RADIX
    return defia
end

function ResistIb(a)
    local defib = GetChaAttr(a, ATTR_ITEMV_PDEF)
    return defib
end

function Hit(a)
    local hit = GetChaAttr(a, ATTR_HIT)
    return hit
end

function HitSa(a)
    local hitsa = GetChaAttr(a, ATTR_STATEC_HIT) / ATTR_RADIX
    return hitsa
end

function HitSb(a)
    local hitsb = GetChaAttr(a, ATTR_STATEV_HIT)
    return hitsb
end

function HitIa(a)
    local hitia = GetChaAttr(a, ATTR_ITEMC_HIT) / ATTR_RADIX
    return hitia
end

function HitIb(a)
    local hitib = GetChaAttr(a, ATTR_ITEMV_HIT)
    return hitib
end

function Flee(a)
    local flee = GetChaAttr(a, ATTR_FLEE)
    return flee
end

function FleeSa(a)
    local fleesa = GetChaAttr(a, ATTR_STATEC_FLEE) / ATTR_RADIX
    return fleesa
end

function FleeSb(a)
    local fleesb = GetChaAttr(a, ATTR_STATEV_FLEE)
    return fleesb
end

function FleeIa(a)
    local fleeia = GetChaAttr(a, ATTR_ITEMC_FLEE) / ATTR_RADIX
    return fleeia
end

function FleeIb(a)
    local fleeib = GetChaAttr(a, ATTR_ITEMV_FLEE)
    return fleeib
end

function Mf(a)
    local mf = GetChaAttr(a, ATTR_MF) / MFRADIX
    return mf
end

function MfSa(a)
    local mfsa = GetChaAttr(a, ATTR_STATEC_MF) / ATTR_RADIX
    return mfsa
end

function MfSb(a)
    local mfsb = GetChaAttr(a, ATTR_STATEV_MF)
    return mfsb
end

function MfIa(a)
    local mfia = GetChaAttr(a, ATTR_ITEMC_MF) / ATTR_RADIX
    return mfia
end

function MfIb(a)
    local mfib = GetChaAttr(a, ATTR_ITEMV_MF)
    return mfib
end

function Crt(a)
    local crt = GetChaAttr(a, ATTR_CRT) / CRTRADIX
    return crt
end

function CrtSa(a)
    local crtsa = GetChaAttr(a, ATTR_STATEC_CRT) / ATTR_RADIX
    return crtsa
end

function CrtSb(a)
    local crtsb = GetChaAttr(a, ATTR_STATEV_CRT)
    return crtsb
end

function CrtIa(a)
    local crtia = GetChaAttr(a, ATTR_ITEMC_CRT) / ATTR_RADIX
    return crtia
end

function CrtIb(a)
    local crtib = GetChaAttr(a, ATTR_ITEMV_CRT)
    return crtib
end

function Hrec(a)
    local hrec = GetChaAttr(a, ATTR_HREC)
    return hrec
end

function HrecSa(a)
    local hrecsa = GetChaAttr(a, ATTR_STATEC_HREC) / ATTR_RADIX
    return hrecsa
end

function HrecSb(a)
    local hrecsb = GetChaAttr(a, ATTR_STATEV_HREC)
    return hrecsb
end

function HrecIa(a)
    local hrecia = GetChaAttr(a, ATTR_ITEMC_HREC) / ATTR_RADIX
    return hrecia
end

function HrecIb(a)
    local hrecib = GetChaAttr(a, ATTR_ITEMV_HREC)
    return hrecib
end

function Srec(a)
    local srec = GetChaAttr(a, ATTR_SREC)
    return srec
end

function SrecSa(a)
    local srecsa = GetChaAttr(a, ATTR_STATEC_SREC) / ATTR_RADIX
    return srecsa
end

function SrecSb(a)
    local srecsb = GetChaAttr(a, ATTR_STATEV_SREC)
    return srecsb
end

function SrecIa(a)
    local srecia = GetChaAttr(a, ATTR_ITEMC_SREC) / ATTR_RADIX
    return srecia
end

function SrecIb(a)
    local srecib = GetChaAttr(a, ATTR_ITEMV_SREC)
    return srecib
end

function Aspd(a)
    local aspd = math.floor(100000 / GetChaAttr(a, ATTR_ASPD))
    return aspd
end

function AspdSa(a)
    local aspdsa = GetChaAttr(a, ATTR_STATEC_ASPD) / ATTR_RADIX
    return aspdsa
end

function AspdSb(a)
    local aspdsb = GetChaAttr(a, ATTR_STATEV_ASPD)
    return aspdsb
end

function AspdIa(a)
    local aspdia = GetChaAttr(a, ATTR_ITEMC_ASPD) / ATTR_RADIX
    return aspdia
end

function AspdIb(a)
    local aspdib = GetChaAttr(a, ATTR_ITEMV_ASPD)
    return aspdib
end

function Adis(a)
    local adis = GetChaAttr(a, ATTR_ADIS)
    return adis
end

function AdisSa(a)
    local adissa = GetChaAttr(a, ATTR_STATEC_ADIS) / ATTR_RADIX
    return adissa
end

function AdisSb(a)
    local adissb = GetChaAttr(a, ATTR_STATEV_ADIS)
    return adissb
end

function AdisIa(a)
    local adisia = GetChaAttr(a, ATTR_ITEMC_ADIS) / ATTR_RADIX
    return adisia
end

function AdisIb(a)
    local adisib = GetChaAttr(a, ATTR_ITEMV_ADIS)
    return adisib
end

function Mspd(a)
    local mspd = GetChaAttr(a, ATTR_MSPD)
    return mspd
end

function MspdSa(a)
    local mspdsa = GetChaAttr(a, ATTR_STATEC_MSPD) / ATTR_RADIX
    return mspdsa
end
function MspdSb(a)
    local mspdsb = GetChaAttr(a, ATTR_STATEV_MSPD)
    return mspdsb
end
function MspdIa(a)
    local mspdia = GetChaAttr(a, ATTR_ITEMC_MSPD) / ATTR_RADIX
    return mspdia
end

function MspdIb(a)
    local mspdib = GetChaAttr(a, ATTR_ITEMV_MSPD)
    return mspdib
end

function Col(a)
    local col = GetChaAttr(a, ATTR_COL)
    return col
end

function ColSa(a)
    local colsa = GetChaAttr(a, ATTR_STATEC_COL) / ATTR_RADIX
    return colsa
end

function ColSb(a)
    local colsb = GetChaAttr(a, ATTR_STATEV_COL)
    return colsb
end

function ColIa(a)
    local colia = GetChaAttr(a, ATTR_ITEMC_COL) / ATTR_RADIX
    return colia
end

function ColIb(a)
    local colib = GetChaAttr(a, ATTR_ITEMV_COL)
    return colib
end

function Str(a)
    local str = GetChaAttr(a, ATTR_STR)
    return str
end

function StrSa(a)
    local strsa = GetChaAttr(a, ATTR_STATEC_STR) / ATTR_RADIX
    return strsa
end

function StrSb(a)
    local strsb = GetChaAttr(a, ATTR_STATEV_STR)
    return strsb
end

function StrIa(a)
    local stria = GetChaAttr(a, ATTR_ITEMC_STR) / ATTR_RADIX
    return stria
end

function StrIb(a)
    local strib = GetChaAttr(a, ATTR_ITEMV_STR)
    return strib
end

function Dex(a)
    local dex = GetChaAttr(a, ATTR_DEX)
    return dex
end

function DexSa(a)
    local dexsa = GetChaAttr(a, ATTR_STATEC_DEX) / ATTR_RADIX
    return dexsa
end

function DexSb(a)
    local dexsb = GetChaAttr(a, ATTR_STATEV_DEX)
    return dexsb
end

function DexIa(a)
    local dexia = GetChaAttr(a, ATTR_ITEMC_DEX) / ATTR_RADIX
    return dexia
end

function DexIb(a)
    local dexib = GetChaAttr(a, ATTR_ITEMV_DEX)
    return dexib
end

function Agi(a)
    local agi = GetChaAttr(a, ATTR_AGI)
    return agi
end

function AgiSa(a)
    local agisa = GetChaAttr(a, ATTR_STATEC_AGI) / ATTR_RADIX
    return agisa
end

function AgiSb(a)
    local agisb = GetChaAttr(a, ATTR_STATEV_AGI)
    return agisb
end

function AgiIa(a)
    local agiia = GetChaAttr(a, ATTR_ITEMC_AGI) / ATTR_RADIX
    return agiia
end

function AgiIb(a)
    local agiib = GetChaAttr(a, ATTR_ITEMV_AGI)
    return agiib
end

function Con(a)
    local con = GetChaAttr(a, ATTR_CON)
    return con
end

function ConSa(a)
    local consa = GetChaAttr(a, ATTR_STATEC_CON) / ATTR_RADIX
    return consa
end

function ConSb(a)
    local consb = GetChaAttr(a, ATTR_STATEV_CON)
    return consb
end

function ConIa(a)
    local conia = GetChaAttr(a, ATTR_ITEMC_CON) / ATTR_RADIX
    return conia
end

function ConIb(a)
    local conib = GetChaAttr(a, ATTR_ITEMV_CON)
    return conib
end

function Sta(a)
    local sta = GetChaAttr(a, ATTR_STA)
    return sta
end

function StaSa(a)
    local stasa = GetChaAttr(a, ATTR_STATEC_STA) / ATTR_RADIX
    return stasa
end

function StaSb(a)
    local stasb = GetChaAttr(a, ATTR_STATEV_STA)
    return stasb
end

function StaIa(a)
    local staia = GetChaAttr(a, ATTR_ITEMC_STA) / ATTR_RADIX
    return staia
end

function StaIb(a)
    local staib = GetChaAttr(a, ATTR_ITEMV_STA)
    return staib
end

function Luk(a)
    local luk = GetChaAttr(a, ATTR_LUK)
    return luk
end

function LukSa(a)
    local luksa = GetChaAttr(a, ATTR_STATEC_LUK) / ATTR_RADIX
    return luksa
end

function LukSb(a)
    local luksb = GetChaAttr(a, ATTR_STATEV_LUK)
    return luksb
end

function LukIa(a)
    local lukia = GetChaAttr(a, ATTR_ITEMC_LUK) / ATTR_RADIX
    return lukia
end

function LukIb(a)
    local lukib = GetChaAttr(a, ATTR_ITEMV_LUK)
    return lukib
end

function BSMxhp(a)
    local bsmxhp = GetChaAttr(a, ATTR_BMXHP)
    return bsmxhp
end

function Mxhp_final(a)
    local mxhp_final = (BSMxhp(a) * MxhpIa(a) + MxhpIb(a)) * math.max(0, MxhpSa(a)) + MxhpSb(a)

    return mxhp_final
end

function Sp_final(a)
    local sp_final = (BSSp(a) * SpIa(a) + SpIb(a)) * math.max(0, SpSa(a)) + SpSb(a)

    return sp_final
end

function BSMxsp(a)
    local bsmxsp = GetChaAttr(a, ATTR_BMXSP)
    return bsmxsp
end

function Mxsp_final(a)
    local mxsp_final = (BSMxsp(a) * MxspIa(a) + MxspIb(a)) * math.max(0, MxspSa(a)) + MxspSb(a)

    return mxsp_final
end

function BSMnatk(a)
    local bsmnatk = GetChaAttr(a, ATTR_BMNATK)
    return bsmnatk
end

function Mnatk_final(a)
    local mnatk_final = math.max((BSMnatk(a) * MnatkIa(a) + MnatkIb(a)) * math.max(0, MnatkSa(a)) + MnatkSb(a), 1)

    return mnatk_final
end

function BSMxatk(a)
    local bsmxatk = GetChaAttr(a, ATTR_BMXATK)
    return bsmxatk
end

function Mxatk_final(a)
    local mxatk_final = math.max((BSMxatk(a) * MxatkIa(a) + MxatkIb(a)) * math.max(0, MxatkSa(a)) + MxatkSb(a), 1)
    return mxatk_final
end

function BSDef(a)
    local bsdef = GetChaAttr(a, ATTR_BDEF)
    return bsdef
end

function Def_final(a)
    local def_final = math.max((BSDef(a) * DefIa(a) + DefIb(a)) * math.max(0, DefSa(a)) + DefSb(a), 0)

    return def_final
end

function BSResist(a)
    local bsresist = GetChaAttr(a, ATTR_BPDEF)
    return bsresist
end

function Resist_final(a)
    local resist_final = (BSResist(a) * ResistIa(a) + ResistIb(a)) * math.max(0, ResistSa(a)) + ResistSb(a)
    return resist_final
end

function BSHit(a)
    local bshit = GetChaAttr(a, ATTR_BHIT)
    return bshit
end

function Hit_final(a)
    local hit_final = (BSHit(a) * HitIa(a) + HitIb(a)) * math.max(0, HitSa(a)) + HitSb(a)
    return hit_final
end

function BSFlee(a)
    local bsflee = GetChaAttr(a, ATTR_BFLEE)
    return bsflee
end

function Flee_final(a)
    local flee_final = (BSFlee(a) * FleeIa(a) + FleeIb(a)) * math.max(0, FleeSa(a)) + FleeSb(a)
    return flee_final
end

function BSMf(a)
    local bsmf = GetChaAttr(a, ATTR_BMF)
    return bsmf
end

function Mf_final(a)
    local mf_final = (BSMf(a) * MfIa(a) + MfIb(a)) * math.max(0, MfSa(a)) + MfSb(a)
    return mf_final
end

function BSCrt(a)
    local bscrt = GetChaAttr(a, ATTR_BCRT)
    return bscrt
end

function Crt_final(a)
    local crt_final = (BSCrt(a) * CrtIa(a) + CrtIb(a)) * math.max(0, CrtSa(a)) + CrtSb(a)
    return crt_final
end

function BSHrec(a)
    local bshrec = GetChaAttr(a, ATTR_BHREC)
    return bshrec
end

function Hrec_final(a)
    local hrec_final = (BSHrec(a) * HrecIa(a) + HrecIb(a)) * math.max(0, HrecSa(a)) + HrecSb(a)
    return hrec_final
end

function BSSrec(a)
    local bssrec = GetChaAttr(a, ATTR_BSREC)
    return bssrec
end

function Srec_final(a)
    local srec_final = (BSSrec(a) * SrecIa(a) + SrecIb(a)) * math.max(0, SrecSa(a)) + SrecSb(a)
    return srec_final
end

function BSAspd(a)
    local bsaspd = math.floor(100000 / GetChaAttr(a, ATTR_BASPD))
    return bsaspd
end

function Aspd_final(a)
    local aspd_final = (BSAspd(a) * AspdIa(a) + AspdIb(a)) * math.max(0, AspdSa(a)) + AspdSb(a)

    return aspd_final
end

function BSAdis(a)
    local bsadis = GetChaAttr(a, ATTR_BADIS)
    return bsadis
end

function Adis_final(a)
    local adis_final = (BSAdis(a) * AdisIa(a) + AdisIb(a)) * math.max(0, AdisSa(a)) + AdisSb(a)
    return adis_final
end

function BSMspd(a)
    local bsmspd = GetChaAttr(a, ATTR_BMSPD)
    return bsmspd
end

function Mspd_final(a)
    local mspd_final =
        math.max(BSMspd(a) * 0.3, ((BSMspd(a) * MspdIa(a) + MspdIb(a)) * math.max(0.3, MspdSa(a)) + MspdSb(a)))
    return mspd_final
end

function BSCol(a)
    local bscol = GetChaAttr(a, ATTR_BCOL)
    return bscol
end

function Col_final(a)
    local col_final = (BSCol(a) * ColIa(a) + ColIb(a)) * math.max(0, ColSa(a)) + ColSb(a)
    return col_final
end

function BSStr(a)
    local bsstr = GetChaAttr(a, ATTR_BSTR)
    return bsstr
end

function Str_final(a)
    local str_final = (BSStr(a) * StrIa(a) + StrIb(a)) * math.max(0, StrSa(a)) + StrSb(a)
    return str_final
end

function BSDex(a)
    local bsdex = GetChaAttr(a, ATTR_BDEX)
    return bsdex
end

function Dex_final(a)
    local dex_final = (BSDex(a) * DexIa(a) + DexIb(a)) * math.max(0, DexSa(a)) + DexSb(a)
    return dex_final
end

function BSAgi(a)
    local bsagi = GetChaAttr(a, ATTR_BAGI)
    return bsagi
end

function Agi_final(a)
    local agi_final = (BSAgi(a) * AgiIa(a) + AgiIb(a)) * math.max(0, AgiSa(a)) + AgiSb(a)
    return agi_final
end

function BSCon(a)
    local bscon = GetChaAttr(a, ATTR_BCON)
    return bscon
end

function Con_final(a)
    local con_final = (BSCon(a) * ConIa(a) + ConIb(a)) * math.max(0, ConSa(a)) + ConSb(a)
    return con_final
end

function BSSta(a)
    local bssta = GetChaAttr(a, ATTR_BSTA)
    return bssta
end

function Sta_final(a)
    local sta_final = (BSSta(a) * StaIa(a) + StaIb(a)) * math.max(0, StaSa(a)) + StaSb(a)
    return sta_final
end

function BSLuk(a)
    local bsluk = GetChaAttr(a, ATTR_BLUK)
    return bsluk
end

function Luk_final(a)
    local luk_final = (BSLuk(a) * LukIa(a) + LukIb(a)) * math.max(0, LukSa(a)) + LukSb(a)
    return luk_final
end

function Ship_BSMnatk(ship_role)
    local ship_bsmnatk = GetChaAttr(ship_role, ATTR_BMNATK)
    return ship_bsmnatk
end

function Ship_BSMxatk(ship_role)
    local ship_bsmxatk = GetChaAttr(ship_role, ATTR_BMXATK)
    return ship_bsmxatk
end

function Ship_BSAdis(ship_role)
    local ship_bsadis = GetChaAttr(ship_role, ATTR_BADIS)
    return ship_bsadis
end

function Ship_BSCspd(ship_role)
    local ship_bscspd = GetChaAttr(ship_role, ATTR_BOAT_BCSPD)
    return ship_bscspd
end

function Ship_BSAspd(ship_role)
    local ship_bsaspd = GetChaAttr(ship_role, ATTR_BASPD)
    return ship_bsaspd
end

function Ship_BSCrange(ship_role)
    local ship_bscrange = GetChaAttr(ship_role, ATTR_BOAT_BCRANGE)
    return ship_bscrange
end

function Ship_BSDef(ship_role)
    local ship_bsdef = GetChaAttr(ship_role, ATTR_BDEF)
    return ship_bsdef
end

function Ship_BSResist(ship_role)
    local ship_bsresist = GetChaAttr(ship_role, ATTR_BPDEF)
    return ship_bsresist
end

function Ship_BSMxhp(ship_role)
    local ship_bsmxhp = GetChaAttr(ship_role, ATTR_BMXHP)
    return ship_bsmxhp
end

function Ship_BSHrec(ship_role)
    local ship_bshrec = GetChaAttr(ship_role, ATTR_BHREC)
    return ship_bshrec
end

function Ship_BSSrec(ship_role)
    local ship_bssrec = GetChaAttr(ship_role, ATTR_BSREC)
    return ship_bssrec
end

function Ship_BSMspd(ship_role)
    local ship_bsmspd = GetChaAttr(ship_role, ATTR_BMSPD)
    return ship_bsmspd
end

function Ship_BSMxsp(ship_role)
    local ship_bsmxsp = GetChaAttr(ship_role, ATTR_BMXSP)
    return ship_bsmxsp
end

function Ship_MnatkSa(cha_role)
    local ship_mnatksa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_MNATK) / ATTR_RADIX
    return ship_mnatksa
end

function Ship_MnatkSb(cha_role)
    local ship_mnatksb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_MNATK)
    return ship_mnatksb
end

function Ship_MxatkSa(cha_role)
    local ship_mxatksa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_MXATK) / ATTR_RADIX
    return ship_mxatksa
end

function Ship_MxatkSb(cha_role)
    local ship_mxatksb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_MXATK)
    return ship_mxatksb
end

function Ship_AdisSa(cha_role)
    local ship_adissa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_ADIS) / ATTR_RADIX
    return ship_adissa
end

function Ship_AdisSb(cha_role)
    local ship_adissb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_ADIS)
    return ship_adissb
end

function Ship_CspdSa(cha_role)
    local ship_cspdsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_CSPD) / ATTR_RADIX
    return ship_cspdsa
end

function Ship_CspdSb(cha_role)
    local ship_cspdsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_CSPD)
    return ship_cspdsb
end

function Ship_AspdSa(cha_role)
    local ship_aspdsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_ASPD) / ATTR_RADIX
    return ship_aspdsa
end

function Ship_AspdSb(cha_role)
    local ship_aspdsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_ASPD)
    return ship_aspdsb
end

function Ship_CrangeSa(cha_role)
    local ship_crangesa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_CRANGE) / ATTR_RADIX
    return ship_crangesa
end

function Ship_CrangeSb(cha_role)
    local ship_crangesb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_CRANGE)
    return ship_crangesb
end

function Ship_DefSa(cha_role)
    local ship_defsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_DEF) / ATTR_RADIX
    return ship_defsa
end

function Ship_DefSb(cha_role)
    local ship_defsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_DEF)
    return ship_defsb
end

function Ship_ResistSa(cha_role)
    local ship_resistsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_RESIST) / ATTR_RADIX
    return ship_resistsa
end

function Ship_ResistSb(cha_role)
    local ship_resistsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_RESIST)
    return ship_resistsb
end

function Ship_MxhpSa(cha_role)
    local ship_mxhpsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_MXUSE) / ATTR_RADIX
    return ship_mxhpsa
end

function Ship_MxhpSb(cha_role)
    local ship_mxhpsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_MXUSE)
    return ship_mxhpsb
end

function Ship_HrecSa(cha_role)
    local ship_hrecsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_USEREC) / ATTR_RADIX
    return ship_hrecsa
end

function Ship_HrecSb(cha_role)
    local ship_hrecsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_USEREC)
    return ship_hrecsb
end

function Ship_SrecSa(cha_role)
    local ship_srecsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_EXP) / ATTR_RADIX
    return ship_srecsa
end

function Ship_SrecSb(cha_role)
    local ship_srecsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_EXP)
    return ship_srecsb
end

function Ship_MspdSa(cha_role)
    local ship_mspdsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_MSPD) / ATTR_RADIX
    return ship_mspdsa
end

function Ship_MspdSb(cha_role)
    local ship_mspdsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_MSPD)
    return ship_mspdsb
end

function Ship_MxspSa(cha_role)
    local ship_mxspsa = GetChaAttr(cha_role, ATTR_BOAT_SKILLC_MXSPLY) / ATTR_RADIX
    return ship_mxspsa
end

function Ship_MxspSb(cha_role)
    local ship_mxspsb = GetChaAttr(cha_role, ATTR_BOAT_SKILLV_MXSPLY)
    return ship_mxspsb
end

function Ship_Mnatk_final(cha_role, ship_role)
    local ship_mnatk =
        math.floor(
        (Ship_BSMnatk(ship_role) * Ship_MnatkSa(cha_role) + Ship_MnatkSb(cha_role)) * MnatkSa(ship_role) +
            MnatkSb(ship_role)
    )
    return ship_mnatk
end

function Ship_Mnatk(ship_role)
    local ship_mnatk = GetChaAttr(ship_role, ATTR_MNATK)
    return ship_mnatk
end

function Ship_Mxatk_final(cha_role, ship_role)
    local ship_mxatk =
        math.floor(
        (Ship_BSMxatk(ship_role) * Ship_MxatkSa(cha_role) + Ship_MxatkSb(cha_role)) * MxatkSa(ship_role) +
            MxatkSb(ship_role)
    )
    return ship_mxatk
end

function Ship_Mxatk(ship_role)
    local ship_mxatk = GetChaAttr(ship_role, ATTR_MXATK)
    return ship_mxatk
end

function Ship_Adis_final(cha_role, ship_role)
    local ship_adis = math.floor((Ship_BSAdis(ship_role) * Ship_AdisSa(cha_role) + Ship_AdisSb(cha_role)))
    return ship_adis
end

function Ship_Adis(ship_role)
    local ship_adis = GetChaAttr(ship_role, ATTR_ADIS)
    return ship_adis
end

function Ship_Cspd_final(cha_role, ship_role)
    local ship_cspd = math.floor((Ship_BSCspd(ship_role) * Ship_CspdSa(cha_role) + Ship_CspdSb(cha_role)))
    return ship_cspd
end

function Ship_Cspd(ship_role)
    local ship_cspd = GetChaAttr(ship_role, ATTR_BOAT_CSPD)
    return ship_cspd
end

function Ship_Aspd_final(cha_role, ship_role)
    local ship_aspd =
        math.floor(
        (Ship_BSAspd(ship_role) * Ship_AspdSa(cha_role) + Ship_AspdSb(cha_role)) * AspdSa(ship_role) + AspdSb(ship_role)
    )
    return ship_aspd
end

function Ship_Aspd(ship_role)
    local ship_aspd = GetChaAttr(ship_role, ATTR_ASPD)
    return ship_aspd
end

function Ship_Crange_final(cha_role, ship_role)
    local ship_crange = math.floor((Ship_BSCrange(ship_role) * Ship_CrangeSa(cha_role) + Ship_CrangeSb(cha_role)))
    return ship_crange
end

function Ship_Crange(ship_role)
    local ship_crange = GetChaAttr(ship_role, ATTR_CRANGE)
    return ship_crange
end

function Ship_Def_final(cha_role, ship_role)
    local ship_def =
        math.floor(
        (Ship_BSDef(ship_role) * Ship_DefSa(cha_role) + Ship_DefSb(cha_role)) * DefSa(ship_role) + DefSb(ship_role)
    )
    return ship_def
end

function Ship_Def(ship_role)
    local ship_def = GetChaAttr(ship_role, ATTR_DEF)
    return ship_def
end

function Ship_Resist_final(cha_role, ship_role)
    local ship_resist =
        math.floor(
        (Ship_BSResist(ship_role) * Ship_ResistSa(cha_role) + Ship_ResistSb(cha_role)) * ResistSa(ship_role) +
            ResistSb(ship_role)
    )
    return ship_resist
end

function Ship_Resistl(ship_role)
    local ship_resist = GetChaAttr(ship_role, ATTR_PDEF)
    return ship_resist
end

function Ship_Mxhp_final(cha_role, ship_role)
    local ship_mxhp =
        math.floor(
        (Ship_BSMxhp(ship_role) * Ship_MxhpSa(cha_role) + Ship_MxhpSb(cha_role)) * MxhpSa(ship_role) + MxhpSb(ship_role)
    )
    return ship_mxhp
end

function Ship_Mxhp(ship_role)
    local ship_mxhp = GetChaAttr(ship_role, ATTR_MXHP)
    return ship_mxhp
end

function Ship_Hp(ship_role)
    local ship_hp = GetChaAttr(ship_role, ATTR_HP)
    return ship_hp
end

function Ship_Hrec_final(cha_role, ship_role)
    local ship_hrec =
        math.floor(
        (Ship_BSHrec(ship_role) * Ship_HrecSa(cha_role) + Ship_HrecSb(cha_role)) * HrecSa(ship_role) + HrecSb(ship_role)
    )
    return ship_hrec
end

function Ship_Hrec(ship_role)
    local ship_hrec = GetChaAttr(ship_role, ATTR_HREC)
    return ship_hrec
end

function Ship_Srec_final(cha_role, ship_role)
    local ship_srec =
        math.floor(
        (Ship_BSSrec(ship_role) * Ship_SrecSa(cha_role) + Ship_SrecSb(cha_role)) * SrecSa(ship_role) + SrecSb(ship_role)
    )
    return ship_srec
end

function Ship_Srec(ship_role)
    local ship_srec = GetChaAttr(ship_role, ATTR_SREC)
    return ship_srec
end

function Ship_Mspd_final(cha_role, ship_role)
    local ship_mspd =
        math.floor(
        (Ship_BSMspd(ship_role) * Ship_MspdSa(cha_role) + Ship_MspdSb(cha_role)) * MspdSa(ship_role) + MspdSb(ship_role)
    )
    return ship_mspd
end

function Ship_Mspd(ship_role)
    local ship_mspd = GetChaAttr(ship_role, ATTR_MSPD)
    return ship_mspd
end

function Ship_Mxsp_final(cha_role, ship_role)
    local ship_mxsp =
        math.floor(
        (Ship_BSMxsp(ship_role) * Ship_MxspSa(cha_role) + Ship_MxspSb(cha_role)) * MxspSa(ship_role) + MxspSb(ship_role)
    )
    return ship_mxsp
end

function Ship_Mxsp(ship_role)
    local ship_mxsp = GetChaAttr(ship_role, ATTR_MXSP)
    return ship_mxsp
end

function Ship_Sp(ship_role)
    local ship_sp = GetChaAttr(ship_role, ATTR_SP)
    return ship_sp
end
function Percentage_Random(A)
    local B = A * 1000000000
    local C = math.random(0, 1000000000)
    local D = 0
    if C <= B then
        D = 1
    else
        D = 0
    end
    return D
end

function Dis(a, b, c, d)
    local x1, y1, x2, y2 = a, b, c, d
    local dis = math.pow(math.pow(x1 - x2, 2) + math.pow(y1 - y2, 2), 0.5)
    return dis
end
function Check_Direction(Player)
    local Angle = {}
    Angle[1] = {Min = 0, Max = 30}
    Angle[2] = {Min = 30, Max = 60}
    Angle[3] = {Min = 60, Max = 90}
    Angle[4] = {Min = 90, Max = 120}
    Angle[5] = {Min = 120, Max = 150}
    Angle[6] = {Min = 150, Max = 180}
    Angle[7] = {Min = 180, Max = 210}
    Angle[8] = {Min = 210, Max = 240}
    Angle[9] = {Min = 240, Max = 270}
    Angle[10] = {Min = 270, Max = 300}
    Angle[11] = {Min = 300, Max = 330}
    Angle[12] = {Min = 330, Max = 360}

    for i, v in pairs(Angle) do
        if GetChaAttr(Player, ATTR_DIREC) >= v.Min and GetChaAttr(Player, ATTR_DIREC) < v.Max then
            return i
        end
    end
end
function Hp_Dmg(Player, Damage)
    local HP = Hp(Player)
    local SP = Sp(Player)
    local SP_CHANGE = 1
    if Damage <= 0 then
        HP = HP - Damage
        SetCharaAttr(HP, Player, ATTR_HP)
        return
    end	
	
	-- Chaos Argent
    if GetChaMapName(Player) == "garner2" then
        if Is_NormalMonster(Player) == 0 then
            Damage = Damage * 0.25
        end
    end
	
	-- Fairy Protection
    if CheckHaveElf(Player) ~= 0 then
        if CheckElfHaveSkill(GetItemForgeParam(CheckHaveElf(Player), 1), 0, 1) == 2 then
            if ElfSKill_PowerSheild(Player, CheckHaveElf(Player), GetItemForgeParam(CheckHaveElf(Player), 1), Damage) ~= 0 then
                SystemNotice(Player, "Fairy activated Protection. Absorbed damage from opponent.")
            end
            Damage = Damage - (ElfSKill_PowerSheild(Player, CheckHaveElf(Player), GetItemForgeParam(CheckHaveElf(Player), 1), Damage))
        end
    end
	
	-- Chaos Frame Equipment Set
    if Server.EqSet["Chaos"](Player) == 1 and IsGarnerWiner(Player) == 1 and Percentage_Random(0.5) == 1 then
        Damage = Damage * 0.5
        SystemNotice(Player, "Obtained ability of Chaos equipment. Damage reduced.")
    elseif Server.EqSet["Chaos"](Player) == 1 and IsGarnerWiner(Player) == 2 and Percentage_Random(0.5) == 1 then
        Damage = Damage * 0.6
        SystemNotice(Player, "Obtained ability of Chaos equipment. Damage reduced.")
    elseif Server.EqSet["Chaos"](Player) == 1 and IsGarnerWiner(Player) == 3 and Percentage_Random(0.5) == 1 then
        Damage = Damage * 0.7
        SystemNotice(Player, "Obtained ability of Chaos equipment. Damage reduced.")
    elseif Server.EqSet["Chaos"](Player) == 1 and IsGarnerWiner(Player) == 4 and Percentage_Random(0.5) == 1 then
        Damage = Damage * 0.8
        SystemNotice(Player, "Obtained ability of Chaos equipment. Damage reduced.")
    elseif Server.EqSet["Chaos"](Player) == 1 and IsGarnerWiner(Player) == 5 and Percentage_Random(0.3) == 1 then
        Damage = Damage * 0.9
        SystemNotice(Player, "Obtained ability of Chaos equipment. Damage reduced.")
    end

	-- Rebirth Mystic Power
	if GetChaAttr(Player, ATTR_CSAILEXP) > 0 and GetChaAttr(Player, ATTR_CSAILEXP) < 100 then
		Damage = Damage * 0.945
	end
	if GetChaAttr(Player, ATTR_CSAILEXP) >= 100 and GetChaAttr(Player, ATTR_CSAILEXP) < 12100 then
		Damage = Damage * (0.95 - math.floor(math.pow((GetChaAttr(Player, ATTR_CSAILEXP) / 100), 0.5)) * 0.005)
	end

	-- Energy Shield
    local EnergyShield = GetChaStateLv(Player, STATE_MFD)
    if EnergyShield >= 1 then
		SP_CHANGE = EnergyShield * 0.25 + 0.5
		-- Black Dragon Equipment Set
		if Server.EqSet["BlackDragon"](Player) == 1 and Percentage_Random(0.5) == 1 then
			SP_CHANGE = SP_CHANGE * 1.5
			SystemNotice(Player, "Obtain power from Black Dragon set. Skill effect enhanced.")
		end
        if (Damage / SP_CHANGE) <= SP then
            SP = math.floor(SP - Damage / SP_CHANGE)
        else
            HP = math.floor(HP - (Damage / SP_CHANGE - SP))
            SP = 0
            RemoveState(Player, STATE_MFD)
        end
    else
		-- Kylin Equipment Set
        if Server.EqSet["Kylin"](Player) == 1 and Percentage_Random(0.1) == 1 then
            Damage = Damage * -1
            SystemNotice(Player, "Received blessing from Goddess. Damage was turned into a blessing.")
        end
        HP = Hp(Player) - Damage
    end
    SetCharaAttr(HP, Player, ATTR_HP)
	SetCharaAttr(SP, Player, ATTR_SP)
end
function Endure_Dmg(Player, Damage)
    local HP = Hp(Player) - Damage
    if HP < 0 then
        HP = -1
    end
    SetCharaAttr(HP, Player, ATTR_HP)
end
function HP_Red_Melee(ATKER, DEFER, Damage, IgnoreDef)
    if IgnoreDef == nil or IgnoreDef == false then
        if ChaIsBoat(DEFER) == 0 then
            Hp_Dmg(DEFER, Damage)
            if GetChaStateLv(DEFER, STATE_YS) >= 1 then
                RemoveState(DEFER, STATE_YS)
            end
        else
            Endure_Dmg(DEFER, Damage)
        end
    elseif IgnoreDef == true then
        SetCharaAttr((GetChaAttr(DEFER, ATTR_HP) - Damage), DEFER, ATTR_HP)
    end

    if CursedEquip.Active == true then
        if IsPlayer(ATKER) == 1 then
        end
        if IsPlayer(DEFER) == 1 then
        end
    end

    if GetChaStateLv(ATKER, STATE_YS) ~= 0 then
        RemoveState(ATKER, STATE_YS)
    end
end
function HP_Red_Skill(ATKER, DEFER, Damage, IgnoreDef)
    if IgnoreDef == nil or IgnoreDef == false then
        if ChaIsBoat(DEFER) == 0 then
            Hp_Dmg(DEFER, Damage)
            if GetChaStateLv(DEFER, STATE_YS) >= 1 then
                RemoveState(DEFER, STATE_YS)
            end
        else
            Endure_Dmg(DEFER, Damage)
        end
    elseif IgnoreDef == true then
        SetCharaAttr((GetChaAttr(DEFER, ATTR_HP) - Damage), DEFER, ATTR_HP)
    end
end
function HP_Red_SkillS(ATKER, DEFER, Damage, IgnoreDef)
    if IgnoreDef == nil or IgnoreDef == false then
        if ChaIsBoat(DEFER) == 0 then
            Hp_Dmg(DEFER, Damage)
            if GetChaStateLv(DEFER, STATE_YS) >= 1 then
                RemoveState(DEFER, STATE_YS)
            end
        else
            Endure_Dmg(DEFER, Damage)
        end
    elseif IgnoreDef == true then
        SetCharaAttr((GetChaAttr(DEFER, ATTR_HP) - Damage), DEFER, ATTR_HP)
    end
end
function Hp_Endure_Dmg(Player, dmg)
    if ChaIsBoat(Player) == 0 then
        Hp_Dmg(Player, dmg)
        local state_ys_lv = GetChaStateLv(Player, STATE_YS)
        if state_ys_lv >= 1 then
            RemoveState(Player, STATE_YS)
        end
    else
        Endure_Dmg(Player, dmg)
    end
end
function Sp_Endure_Dmg(Player, dmg)
    local sp = Sp(Player) - dmg
    SetCharaAttr(sp, Player, ATTR_SP)
end
function Sp_Red(Player, sp_reduce)
    local sp = Sp(Player) - sp_reduce

    SetCharaAttr(sp, Player, ATTR_SP)
end
function Coefficientadjust_Steady_atk()
    local steady_atk_maxreduce = 0.9
    local steady_atk_maxreducepoint = 0.99
    local steady_atk_maxluk = 1500
    local a = steady_atk_maxreduce / steady_atk_maxluk
    local b = steady_atk_maxreducepoint / steady_atk_maxluk

    return a, b, steady_atk_maxreduce, steady_atk_maxreducepoint
end
function SetSteady_atk(a)
    local x, y, m, n = Coefficientadjust_Steady_atk()
    local sum = 1
    local atkstep = {}
    local atk = {}
    atk[Mnatk_final(a) - 1] = 0
    for i = Mnatk_final(a), Mxatk_final(a), 1 do
        atkstep[i] = 1
    end
    local maxreduce = math.min(m, Luk_final(a) * x)
    local reducepoint = math.min(n, Luk_final(a) * y) * (Mxatk_final(a) - Mnatk_final(a))
    if reducepoint == 0 then
        atk[Mnatk_final(a)] = 1
        return atk, sum
    end
    local steady_step = maxreduce / reducepoint
    for i = 0, Mxatk_final(a) - Mnatk_final(a), 1 do
        atkstep[i + Mnatk_final(a)] = atkstep[i + Mnatk_final(a)] - math.max(0, (maxreduce - i * steady_step))
        sum = sum + atkstep[i + Mnatk_final(a)]
        atk[i + Mnatk_final(a)] = sum
    end

    return atk, sum
end
function CheckSteady_atk(a)
    local atk, sum = SetSteady_atk(a)
    local x = math.random(0, 10000)
    local y = x * sum / 10000
    for i = Mnatk_final(a), Mxatk_final(a), 1 do
        if y <= atk[i] then
            return i
        end
    end

    return Mnatk_fianl(a)
end
function Lefthand_Atk(Player, atk)
    local sklv = GetSkillLv(Player, STATE_FSZ)
    add_atk = math.floor(atk * (1.2 + sklv * 0.08))
    return add_atk
end
function Check_Zmyj(Player, dmg_mul)
    local statelv = GetChaStateLv(Player, STATE_ZMYJ)
    local crt_rad = 0.2 + statelv * 0.02
    if dmg_mul == 2 or dmg_mul == 1 then
        a = Percentage_Random(crt_rad)
        if a == 1 then
            dmg_mul = 2 + statelv / 2
        end
    end
    return dmg_mul
end
function Check_Smyb(Player)
    local statelv = GetChaStateLv(Player, STATE_SMYB)
    local hp = Hp(Player)
    local mxhp = Mxhp(Player)
    local atk_sa = 1
    if hp <= mxhp * 0.2 and hp > 0 then
        atk_sa = 1 + statelv * 0.1
    end
    local mnatk = Mnatk(Player) * atk_sa
    local mxatk = Mxatk(Player) * atk_sa
    return mnatk, mxatk
end
function Check_Ys_Rem(role_atk, role_def)
    Check_State(role_atk, role_def)
    if atk_statecheck[STATE_YS] >= 1 then
        RemoveState(role_atk, STATE_YS)
    end
end
function Check_Bshd(statelv)
    local statetime = 3 + statelv * 1
    return statetime
end
function Atk_Raise(rad, atker, defer)
    local atk = rad * math.random(Mnatk(atker), Mxatk(atker))
    local defer_def = Def(defer)
    local defer_resist = Resist(defer)
    dmg = Phy_Dmg(atk, defer_def, defer_resist)
    return dmg
end
function TurnToCha(Player)
    local x_role = Player
    if ChaIsBoat(Player) == 1 then
        x_role = GetMainCha(Player)
    end
    return x_role
end
function TurnToShip(Player)
    local x_role = Player
    if ChaIsBoat(Player) == 0 then
        x_role = GetCtrlBoat(Player)
        if x_role == nil then
            LG("getshipid_err", " get a nil shipid ")
        end
    end
    return x_role
end
function ALLExAttrSet(Player)
    if IsPlayer(Player) == 0 then
        ExAttrSet(Player)
        return
    end
    if ChaIsBoat(Player) == 0 then
        AttrRecheck(Player)
    else
        cha_role = GetMainCha(Player)
        ShipAttrRecheck(cha_role, Player)
    end
end
function Boat_plus_MNATk(Lv, mnatk)
    local mnatk_new = 0

    if Lv < 1 then
        LG("Boat_plus_MNatk", "Ship level is lower than 1")
        LG("Boat_plus_MNatk", "1")
        return mnatk
    end

    if Lv > 100 then
        LG("Boat_plus_MNatk", "Ship level higher than 100")
        LG("Boat_plus_MNatk", "Ship level lower than 2")

        return mnatk
    end

    if Lv < 60 then
        LG("Boat_plus_MNatk", mnatk)
        mnatk_new = (1 + (Lv - 20) / 120) * mnatk
        LG("Boat_plus_MNatk", "Ship level smaller than 3")

        return mnatk_new
    end

    if Lv >= 60 then
        mnatk_new = (1 + (60 - 20) / 120) * mnatk + (Lv - 60) * 5
        LG("Boat_plus_MNatk", "Ship level lower than 4")

        return mnatk_new
    end
end

function Boat_plus_MXATk(Lv, mxatk)
    local mxatk_new = 0

    if Lv < 1 then
        LG("Boat_plus_MXatk", "Ship level is lower than 1")
        return mxatk_new
    end

    if Lv > 100 then
        LG("Boat_plus_MXatk", "Ship level higher than 100")
        return mxatk_new
    end

    if Lv < 60 then
        mxatk_new = (1 + (Lv - 20) / 120) * mxatk
        return mxatk_new
    end

    if Lv >= 60 then
        mxatk_new = (1 + (60 - 20) / 120) * mxatk + (Lv - 60) * 5
        return mxatk_new
    end
end

function Boat_plus_def(Lv, def)
    local def_new = 0

    if Lv < 1 then
        LG("Boat_plus_def", "Ship level is lower than 1")
        return def
    end

    if Lv > 100 then
        LG("Boat_plus_def", "Ship level higher than 100")
        return def
    end

    if Lv < 60 then
        def_new = (1 + (Lv - 20) / 120) * def
        LG("Boat_plus_def", "def_new1 = ", def_new)
        return def_new
    end

    if Lv >= 60 then
        def_new = (1 + (60 - 20) / 120) * def + (Lv - 60) * 3
        LG("Boat_plus_def", "def_new2 = ", def_new)
        return def_new
    end
end

function Boat_plus_Mxhp(Lv, Mxhp)
    local Mxhp_new = 0

    if Lv < 1 then
        LG("Boat_plus_Mxhp", "Ship level is lower than 1")
        return Mxhp
    end

    if Lv > 100 then
        LG("Boat_plus_Mxhp", "Ship level higher than 100")
        return Mxhp
    end

    if Lv < 60 then
        Mxhp_new = (1 + (Lv - 20) / 120) * Mxhp
        LG("Boat_plus_def", "def_new2 = ", Mxhp_new)
        return Mxhp_new
    end

    if Lv >= 60 then
        Mxhp_new = (1 + (60 - 20) / 120) * Mxhp + (Lv - 60) * 20
        LG("Boat_plus_def", "def_new2 = ", Mxhp_new)
        return Mxhp_new
    end
end

function Boat_plus_Mspd(Lv, Mspd)
    local Mspd_new = 0

    if Lv < 1 then
        LG("Boat_plus_Mspd", "Ship level is lower than 1")
        return Mspd
    end

    if Lv > 100 then
        LG("Boat_plus_Mspd", "Ship level higher than 100")
        return Mspd
    end

    if Lv < 60 then
        Mspd_new = (1 + (Lv - 30) / 300) * Mspd
        return Mspd_new
    end

    if Lv >= 60 then
        Mspd_new = (1 + (60 - 30) / 300) * Mspd
        return Mspd_new
    end
end

function get_cha_guild_id(cha)
    local ply_cha = CheckChaRole(cha)
    if ply_cha == 1 then
        return GetChaGuildID(cha)
    else
        local map_copy = GetChaMapCopy(cha)
        local side_id = GetChaSideID(cha)
        if side_id == 1 then
            return GetMapCopyParam2(map_copy, 4)
        elseif side_id == 2 then
            return GetMapCopyParam2(map_copy, 3)
        end
    end
end

function is_teammate(cha1, cha2)
    if cha1 == 0 or cha2 == 0 then
        return 0
    end
    if cha1 == cha2 then
        return 1
    end
    local ply1 = GetChaPlayer(cha1)
    local ply2 = GetChaPlayer(cha2)
    if ply1 ~= 0 and ply2 ~= 0 then
        if ply1 == ply2 then
            return 1
        end
        local team_id1, team_id2
        team_id1 = GetChaTeamID(cha1)
        team_id2 = GetChaTeamID(cha2)
        if team_id1 ~= 0 and team_id2 ~= 0 and team_id1 == team_id2 then
            return 1
        end
    end

    return 0
end

function is_friend(A, B)
    local friend_target = 1
    local MapType = GetChaMapType(A)

    if CheckChaRole(A) == 0 and MapType ~= 2 then
        if CheckChaRole(B) == 0 then
            return 1
        else
            return 0
        end
    end
	
    if MapType == 1 then
        if CheckChaRole(A) == 1 then
            if CheckChaRole(B) == 0 then
                return 0
            else
                return 1
            end
        end
    end
    if MapType == 4 then
        if Is_NormalMonster(A) == 1 and Is_NormalMonster(B) == 1 then
            return 1
        end
        if is_teammate(A, B) == 1 then
            return 1
        else
            if get_cha_guild_id(A) ~= 0 and get_cha_guild_id(B) ~= 0 and get_cha_guild_id(A) == get_cha_guild_id(B) then
                return 1
            else
                return 0
            end
        end
    end
    if MapType == 3 then
        if is_teammate(A, B) == 1 then
            return 1
        else
            return 0
        end
    end
    if A == 0 or B == 0 then
        return 0
    end
    if MapType == 2 then
        if Is_NormalMonster(A) == 1 then
            if Is_NormalMonster(B) == 1 then
                return 1
            end
        end
        if get_cha_guild_id(A) ~= 0 and get_cha_guild_id(B) ~= 0 and get_cha_guild_id(A) == get_cha_guild_id(B) then
            return 1
        else
            return 0
        end
    end
    if MapType == 5 then
        if GetChaSideID(A) == GetChaSideID(B) then
            return 1
        else
            return 0
        end
    end
    return friend_target
end

function Is_NormalMonster(Player)
    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    for i = 0, UnNormalMonster_Num, 1 do
        if Cha_Num == UnNormalMonster_ID[i] then
            return 0
        end
    end

    return 1
end

function CheckMonsterDead(Player)
    if Player == nil then
        return 1
    end
    if Hp(Player) <= 0 then
        return 1
    end
    return 0
end

function ReCheck_Skill_Dmg(MaxDmg, MinDmg, Chance)
    local a = 0
    a = Percentage_Random(Chance / 100)
    if a == 1 then
        return MaxDmg
    else
        return MinDmg
    end
end

function ReCheck_PK_Lv(ATKER, DEFER)
    local Lv_atker = Lv(ATKER)
    local Lv_defer = Lv(DEFER)
    return Lv_atker - Lv_defer
end

function GetMapRealName(Map)
	local MapTag = {}
	MapTag["garner"] = "Ascaron"
	MapTag["magicsea"] = "Magic Sea"
	MapTag["darkblue"] = "Deep Blue"
	MapTag["garner2"] = "Chaos Argent"
	MapTag["puzzleworld"] = "Demonic World"
	MapTag["puzzleworld2"] = "Demonic World 2"
	MapTag["abandonedcity"] = "Abandoned City" 
	MapTag["abandonedcity2"] = "Abandoned City 2"  
	MapTag["abandonedcity3"] = "Abandoned City 3"  
	MapTag["darkswamp"] = "Dark Swamp"  
	MapTag["heilong"]	= "Black Dragon Lair"
	if (MapTag[Map] ~= nil) then
		return MapTag[Map]
	else
		return Map
	end
end

function GiveHonor(Player, Num)
	local MedalOfValor = GetChaItem2(Player, 2, 3849)
	local Honor = GetItemAttr(MedalOfValor, ITEMATTR_VAL_STR)
	local HonorAdd = Num
	local HonorMinLimit = 0
	local HonorMaxLimit = 30000
	local FinalHonor = Honor + HonorAdd
	SetChaKitbagChange(Player, 0)
	if FinalHonor >= HonorMaxLimit then
		SetItemAttr(MedalOfValor, ITEMATTR_VAL_STR, HonorMaxLimit)
	elseif FinalHonor <= HonorMinLimit then
		SetItemAttr(MedalOfValor, ITEMATTR_VAL_STR, HonorMinLimit)
	else
		SetItemAttr(MedalOfValor, ITEMATTR_VAL_STR, FinalHonor)
	end
	SynChaKitbag(Player, 7)
	RefreshCha(Player)
end

function GiveChaosPoint(Player, Num)
	local MedalOfValor = GetChaItem2(Player, 2, 3849)
	local Chaos = GetItemAttr(MedalOfValor, ITEMATTR_MAXENERGY)
	local ChaosAdd = Num
	local ChaosMinLimit = 0
	local ChaosMaxLimit = 30000
	local FinalChaos = Chaos + ChaosAdd
	SetChaKitbagChange(Player, 0)
	if FinalChaos >= ChaosMaxLimit then
		SetItemAttr(MedalOfValor, ITEMATTR_MAXENERGY, ChaosMaxLimit)
	elseif FinalChaos <= ChaosMinLimit then
		SetItemAttr(MedalOfValor, ITEMATTR_MAXENERGY, ChaosMinLimit)
	else
		SetItemAttr(MedalOfValor, ITEMATTR_MAXENERGY, FinalChaos)
	end
	SynChaKitbag(Player, 7)
	RefreshCha(Player)
end

function after_player_kill_player(ATKER, DEFER)
    ATKER = TurnToCha(ATKER)
    DEFER = TurnToCha(DEFER)

    SetCharaAttr(0, DEFER, ATTR_SP)
    local MapName_ATKER = GetChaMapName(ATKER)
    local MapName_DEFER = GetChaMapName(DEFER)
    local Name_ATKER = GetChaDefaultName(ATKER)
    local Name_DEFER = GetChaDefaultName(DEFER)
    local MapCopy = GetChaMapCopy(ATKER)
    local LvDIF = Lv(ATKER) - Lv(DEFER)
    local MapNamePK = {}

    MapNamePK[0] = "puzzleworld"
    MapNamePK[1] = "puzzleworld2"
    MapNamePK[2] = "abandonedcity"
    MapNamePK[3] = "abandonedcity2"
    MapNamePK[4] = "abandonedcity3"
    MapNamePK[5] = "darkswamp"
    MapNamePK[6] = "hell"
    MapNamePK[7] = "hell2"
    MapNamePK[8] = "hell3"
    MapNamePK[9] = "hell4"
    MapNamePK[10] = "hell5"
	MapNamePK[11] = "heilong"

    local ATKER_Get_Ry = 0
    local DEFER_Get_Ry = 0
    local ATKER_Get1_LD = 0
    local DEFER_Get1_Ry = 0

	if MapName_ATKER == "garner2" or MapName_DEFER == "garner2" then
		local ATKER_LV = Lv(ATKER)
		local DEFER_LV = Lv(DEFER)
		local LV_DIF = (ATKER_LV - DEFER_LV)
		local ATKER_CA_PTS = 0
		local DEFER_HONOR_PTS = 0    
		if (LV_DIF < 15 and LV_DIF > -15) then
			ATKER_CA_PTS = 1
			DEFER_HONOR_PTS = -1
			SystemNotice(ATKER, "defeats opponent and obtain 1 Chaos point")
			SystemNotice(DEFER, "was defeated by opponent and loses 1 Honor point")
		end
		if (LV_DIF >= 15) then
			ATKER_CA_PTS = 0
			DEFER_HONOR_PTS = 0
			SystemNotice(ATKER, "Defeated low level opponent. No Chaos point obtained")
			SystemNotice(DEFER, "No honor points will be deducted when defeated by a higher level opponent.")
		end
		if (LV_DIF <= -14) then
			ATKER_CA_PTS = 2
			DEFER_HONOR_PTS = -1
			SystemNotice(ATKER, "Defeated high level opponent. Obtained 2 additional Honor points")
			SystemNotice(DEFER, "Defeated by low level opponent. Lost 2 additional Honor points")
		end
		if (ATKER == DEFER) then
			ATKER_CA_PTS = ATKER_CA_PTS - 1
		end
		GiveChaosPoint(ATKER, ATKER_CA_PTS)
		GiveHonor(DEFER, DEFER_HONOR_PTS)
		Notice("[Chaos Argent]: ["..Name_DEFER.."] was defeated by ["..Name_ATKER.."] in Chaos Argent!")
	end
	
    if MapName_ATKER == "teampk" or MapName_DEFER == "teampk" then
		local ATKER_RYZ = GetChaItem2(ATKER, 2, 3849)
		local DEFER_RYZ = GetChaItem2(DEFER, 2, 3849)
		local Kill = 1
        local Death = 1
        if LvDIF < 10 and LvDIF > -5 then
            ATKER_Get_Ry = 1
            DEFER_Get_Ry = -1
            SystemNotice(ATKER, "Defeated opponent and obtained [" .. ATKER_Get_Ry .. "] Honor Points.")
            SystemNotice(DEFER, "Defeated by opponent and lost [" .. ATKER_Get_Ry .. "] Honor Points.")
        end
        if LvDIF >= 10 then
            ATKER_Get_Ry = 0
            DEFER_Get_Ry = 0
            SystemNotice(ATKER, "Defeated low level opponent. No Honor Points obtained.")
        end
        if LvDIF < -5 then
            ATKER_Get_Ry = 2
            DEFER_Get_Ry = -2
            SystemNotice(ATKER, "Defeated high level opponent and obtained: [" .. ATKER_Get_Ry .. "] Honor Points.")
            SystemNotice(DEFER, "Defeated by low level opponent and obtained: [" .. ATKER_Get_Ry .. "] Honor Points.")
        end
		Add_ItemAttr_RYZ(ATKER, ATKER_RYZ, ITEMATTR_VAL_STR, ATKER_Get_Ry)
		Add_ItemAttr_RYZ(ATKER, ATKER_RYZ, ITEMATTR_VAL_AGI, 1)
		Add_ItemAttr_RYZ(DEFER, DEFER_RYZ, ITEMATTR_VAL_STR, DEFER_Get_Ry)
		Add_ItemAttr_RYZ(DEFER, DEFER_RYZ, ITEMATTR_VAL_DEX, 1)
        MapCopyNotice(MapCopy, "[" .. Name_DEFER .. "] was defeated by [" .. Name_ATKER .. "].")
    end
	
    if MapName_ATKER == "PKmap" or MapName_DEFER == "PKmap" then
        MapCopyNotice(MapCopy, "[PK Map]: [" .. Name_DEFER .. "] was defeated by [" .. Name_ATKER .. "]")
    end

    local C_Map = 0
    for C_Map = 0, #MapNamePK, 1 do
        if MapName_ATKER == MapNamePK[C_Map] then
			Notice("[" .. Name_DEFER .. "] was defeated by [" .. Name_ATKER .. "] in "..GetMapRealName(GetChaMapName(ATKER)).."!")
            if CheckBagItem(DEFER, 3846) <= 0 then
				BickerNotice(DEFER, "I died!")
                Dead_Punish_ItemURE(DEFER)
                MGPK_Dead_Punish_Exp(DEFER)
            else
                local j = DelBagItem(DEFER, 3846, 1)
                if j == 0 then
                    LG("NewItem", "Voodoo Doll deletion failed.")
                else
                    SystemNotice(DEFER, "Voodoo Doll replaces death penalty.")
                end
            end
        end
    end
    local Item_WWZ = GetChaItem2(ATKER, 2, 5803)
    if ValidCha(Item_WWZ) == 0 then
        return
    end
    local Kill_WWZ_Num = GetItemAttr(Item_WWZ, ITEMATTR_VAL_AGI) + 1
    SetItemAttr(Item_WWZ, ITEMATTR_VAL_AGI, Kill_WWZ_Num)
    if Kill_WWZ_Num > 11 then
    end
end

function MGPK_Dead_Punish_Exp(dead)
    local map_name = GetChaMapName(dead)
    dead = TurnToCha(dead)
    local lv = GetChaAttr(dead, ATTR_LV)
    local exp_red
    local exp = Exp(dead)
    local nlexp = GetChaAttrI(dead, ATTR_NLEXP)
    local clexp = GetChaAttrI(dead, ATTR_CLEXP)
    local exp_per = math.min(math.floor((nlexp - clexp) * 0.02), math.max(exp - clexp, 0))
    if exp <= clexp then
        exp_red = 0
    else
        exp_red = math.pow(lv, 2) * 20
    end

    if exp_red > exp_per then
        exp_red = exp_per
    end

    if Lv(dead) >= 80 then
        exp_red = math.floor(exp_red / 50)
        exp_red_80 = exp_red * 50
        SystemNotice(dead, "Death penalty. EXP lost:" .. exp_red_80)
    else
        SystemNotice(dead, "Death penalty. EXP lost:" .. exp_red)
    end

    exp = Exp(dead) - exp_red

    SetChaAttrI(dead, ATTR_CEXP, exp)

    local name = GetChaDefaultName(dead)

    LG("PKdie_exp", "Character Name", name, "Current Lv= ", lv, "Death EXP penalty= ", exp_red)
end

function Add_ItemAttr_RYZ(Cha_role, Player, attrtype, Num)
    local i = 0
    local attr_num = GetItemAttr(Player, attrtype)

    attr_num = attr_num + Num
    i = SetItemAttr(Player, attrtype, attr_num)
    local attr_num_1 = GetItemAttr(Player, attrtype)

    if i == 0 then
        LG("RYZ_PK", "add Honor attribute failed")
    end
end

function Get_ItemAttr_Join(Cha_role)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Cha_role, 3849)

    if RYZ_Num <= 0 then
        return 0
    end

    local role_RYZ = GetChaItem2(Cha_role, 2, 3849)
    local attrtype = ITEMATTR_VAL_CON
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    return attr_num
end

function Get_ItemAttr_Win(Cha_role)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Cha_role, 3849)

    if RYZ_Num <= 0 then
        return 0
    end

    local role_RYZ = GetChaItem2(Cha_role, 2, 3849)
    local attrtype = ITEMATTR_VAL_STA
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    return attr_num
end

function AddYongYuZhi(Player, value)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Player, 3849)

    if RYZ_Num <= 0 then
        SystemNotice(Player, "Does not possess Medal of Valor")
        return 0
    end

    local role_RYZ = GetChaItem2(Player, 2, 3849)
    local attrtype = ITEMATTR_VAL_STR
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    local attr_num = attr_num + value
    local i = 0
    SetChaKitbagChange(Player, 0)
    i = SetItemAttr(role_RYZ, attrtype, attr_num)

    if i == 0 then
        return 0
    end
    if value > 0 then
        SystemNotice(Player, "Increases Honor points by " .. value)
    end
    if value < 0 then
        local a = -1 * value
        SystemNotice(Player, "Deduct Honor Point" .. a)
    end

    SynChaKitbag(Player, 7)
    return 1
end

function TakeZuDuiGongXianDu(Player, value)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Player, 3849)

    if RYZ_Num <= 0 then
        SystemNotice(Player, "Does not possess Medal of Valor")
        return 0
    end

    local role_RYZ = GetChaItem2(Player, 2, 3849)
    local attrtype = ITEMATTR_MAXURE
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    local attr_num = attr_num - value
    local i = 0
    SetChaKitbagChange(Player, 0)
    i = SetItemAttr(role_RYZ, attrtype, attr_num)

    if i == 0 then
        return 0
    end
    if value < 0 then
        local a = value * -1
        SystemNotice(Player, "Obtained Party Contribution points" .. a)
    end
    if value > 0 then
        SystemNotice(Player, "Party Contribution points deducted:" .. value)
        LG("RYZ_Take_Zdgx", " uses Team Contribution points, deducts " .. value .. "point")
    end
    SynChaKitbag(Player, 7)
    return 1
end

function HasZuDuiGongXianDu(Player, value)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Player, 3849)

    if RYZ_Num <= 0 then
        return 0
    end

    local role_RYZ = GetChaItem2(Player, 2, 3849)
    local attrtype = ITEMATTR_MAXURE
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    if attr_num >= value then
        return 1
    else
        return 0
    end
end

function LessYongYuZhi(Player, str, value)
    local RYZ_Num = 0
    RYZ_Num = CheckBagItem(Player, 3849)

    if RYZ_Num <= 0 then
        return 0
    end

    local role_RYZ = GetChaItem2(Player, 2, 3849)
    local attrtype = ITEMATTR_VAL_STR
    local attr_num = GetItemAttr(role_RYZ, attrtype)
    if str == ">" then
        if attr_num > value then
            return 1
        end
    elseif str == "<" then
        if attr_num < value then
            return 1
        end
    elseif str == "=" then
        if attr_num == value then
            return 1
        end
    else
        LG("RYZ_PK", "determine Honor character error")
    end
end

function Add_RYZ_TeamPoint(role, count_num, add_num)
    -- local attr_num = GetChaAttr(role, ATTR_EXTEND5)
    -- if attr_num >= 1000 then
        -- return
    -- end
    -- attr_num = attr_num + add_num
    -- local a = math.min(1, (count_num - 2) * 0.3)
    -- local b = 0
    -- b = Percentage_Random(a)
    -- local i = 0
    -- if b == 1 then
        -- i = AddTeamContribution(role, attr_num)
        -- if i == 0 then
            -- LG("RYZ_PK", "Increase Party Contribution value failed")
            -- return
        -- end
        -- SystemNotice(role, "Obtain " .. add_num .. " point(s) of Team Contribution point")
    -- end
	local RYZ_Num = 0
	RYZ_Num = CheckBagItem( role,3849 )

	if RYZ_Num <= 0 then
		return
	end
	local role_RYZ = GetChaItem2 ( role , 2 , 3849 )
	local attrtype = ITEMATTR_MAXURE
	local attr_num = GetItemAttr ( role_RYZ , attrtype )
	if attr_num >= 1000 then
		return
	end
	attr_num = attr_num + add_num
	local a = math.min ( 1 , ( count_num - 2 ) * 0.3 )
	local b = 0
	b = Percentage_Random ( a )
	local i = 0
	if b == 1 then

		SetChaKitbagChange ( role , 0 )
		i = SetItemAttr ( role_RYZ ,attrtype , attr_num )

		if i == 0 then
			LG("RYZ_PK","Increase Party Contribution value failed")
			return
		end
		SynChaKitbag ( role , 7 )
	
		SystemNotice ( role ,"Obtain "..add_num.." point(s) of Team Contribution point")
	end
end

function Take_Atk_ItemURE(Player)
    local Atk = IsPlayer(Player)
    local boat = ChaIsBoat(Player)
    if Atk == 1 and boat == 0 then
        local Item_1 = 0
        local Item_2 = 0
        Item_1 = GetChaItem(Player, 1, 6)
        Item_2 = GetChaItem(Player, 1, 9)

        local Item_URE = 0
        local Take_Num = 1
        local i = 0
        local Item1_Type = Check_Repair_ItemType(Item_1)
        local Item2_Type = Check_Repair_ItemType(Item_2)

        if Item_1 ~= 0 and Item_1 ~= nil then
            local a = 0.03
            local b = Percentage_Random(a)

            if b == 1 and Item1_Type == 1 then
                Item_URE = GetItemAttr(Item_1, ITEMATTR_URE)
                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_1, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "Weapon imbue failed")
                end
                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 6, 0)
                end
            end
        end

        Item_URE = 0
        Take_Num = 1
        i = 0

        if Item_2 ~= 0 and Item_2 ~= nil then
            local a = 0.03
            local b = Percentage_Random(a)
            if b == 1 and Item2_Type == 1 then
                Item_URE = GetItemAttr(Item_2, ITEMATTR_URE)

                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_2, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "Weapon imbue failed")
                end
                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 9, 0)
                end
            end
        end
    end
end

function Take_Def_ItemURE(Player)
    local def = IsPlayer(Player)
    local boat = ChaIsBoat(Player)
    if def == 1 and boat == 0 then
        local Item_1 = 0
        local Item_2 = 0
        local Item_3 = 0
        local Item_4 = 0

        Item_1 = GetChaItem(Player, 1, 0)
        Item_2 = GetChaItem(Player, 1, 2)
        Item_3 = GetChaItem(Player, 1, 3)
        Item_4 = GetChaItem(Player, 1, 4)

        local Item_URE = 0
        local Take_Num = 1
        local i = 0

        if Item_1 ~= 0 and Item_1 ~= nil then
            local a = 0.015
            local b = Percentage_Random(a)
            if b == 1 then
                Item_URE = GetItemAttr(Item_1, ITEMATTR_URE)

                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_1, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "add armor attribute failed")
                end

                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 0, 0)
                end
            end
        end

        Item_URE = 0
        Take_Num = 1
        i = 0

        if Item_2 ~= 0 and Item_2 ~= nil then
            local a = 0.015
            local b = Percentage_Random(a)
            if b == 1 then
                Item_URE = GetItemAttr(Item_2, ITEMATTR_URE)

                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_2, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "add armor attribute failed")
                end

                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 2, 0)
                end
            end
        end

        Item_URE = 0
        Take_Num = 1
        i = 0

        if Item_3 ~= 0 and Item_3 ~= nil then
            local a = 0.015
            local b = Percentage_Random(a)
            if b == 1 then
                Item_URE = GetItemAttr(Item_3, ITEMATTR_URE)

                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_3, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "add armor attribute failed")
                end

                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 3, 0)
                end
            end
        end

        Item_URE = 0
        Take_Num = 1
        i = 0

        if Item_4 ~= 0 and Item_4 ~= nil then
            local a = 0.015
            local b = Percentage_Random(a)
            if b == 1 then
                Item_URE = GetItemAttr(Item_4, ITEMATTR_URE)

                if Item_URE < 50 then
                    Take_Num = 0
                end

                Item_URE = Item_URE - Take_Num

                i = SetItemAttr(Item_4, ITEMATTR_URE, Item_URE)
                if i == 0 then
                    LG("Item_URE", "add armor attribute failed")
                end

                if Item_URE < 50 and Take_Num ~= 0 then
                    SetChaEquipValid(Player, 4, 0)
                end
            end
        end
    end
end

function Dead_Punish_ItemURE(role)
    local Player = IsPlayer(role)
    local boat = ChaIsBoat(role)
    local Punish = 0.05
    Dead_Punish_Item_Num = 5
    local Dead_Punish_Item_WZ = {}
    Dead_Punish_Item_WZ[0] = 0
    Dead_Punish_Item_WZ[1] = 2
    Dead_Punish_Item_WZ[2] = 3
    Dead_Punish_Item_WZ[3] = 4
    Dead_Punish_Item_WZ[4] = 6
    Dead_Punish_Item_WZ[5] = 9
    local Dead_Punish_Item = {}
    Dead_Punish_Item[0] = GetChaItem(role, 1, Dead_Punish_Item_WZ[0])
    Dead_Punish_Item[1] = GetChaItem(role, 1, Dead_Punish_Item_WZ[1])
    Dead_Punish_Item[2] = GetChaItem(role, 1, Dead_Punish_Item_WZ[2])
    Dead_Punish_Item[3] = GetChaItem(role, 1, Dead_Punish_Item_WZ[3])
    Dead_Punish_Item[4] = GetChaItem(role, 1, Dead_Punish_Item_WZ[4])
    Dead_Punish_Item[5] = GetChaItem(role, 1, Dead_Punish_Item_WZ[5])
    if Player == 1 and boat == 0 then
        local Item_URE = 0
        local Item_MAXURE = 0
        local Take_Num = 0
        local i = 0
        local j = 0
        local k = 0
        for j = 0, Dead_Punish_Item_Num, 1 do
            if Dead_Punish_Item[j] ~= 0 and Dead_Punish_Item[j] ~= nil then
                local ItemType_Check = Check_Repair_ItemType(Dead_Punish_Item[j])
                if ItemType_Check == 1 then
                    Item_URE = GetItemAttr(Dead_Punish_Item[j], ITEMATTR_URE)
                    Item_MAXURE = GetItemAttr(Dead_Punish_Item[j], ITEMATTR_MAXURE)
                    Take_Num = math.floor(Item_MAXURE * Punish)
                    if Item_URE >= 50 then
                        k = 1
                    end
                    Item_URE = Item_URE - Take_Num
                    if Item_URE < 50 then
                        Item_URE = 49
                    end
                    i = SetItemAttr(Dead_Punish_Item[j], ITEMATTR_URE, Item_URE)
                    if i == 0 then
                        LG("Item_URE", "Normal death deducts attribute failed " .. j)
                    end
                    if k == 1 and Item_URE == 49 then
                        SetChaEquipValid(role, Dead_Punish_Item_WZ[j], 0)
                    end
                end
            end
        end
        SystemNotice(role, "Death penalty: lost 5%% equipment durability")
    end
end

function PK_Dead_Punish_ItemURE(Player)
    local Player = IsPlayer(Player)
    local boat = ChaIsBoat(Player)
    local Punish = 0.05
    PK_Dead_Punish_Item_Num = 5

    local PK_Dead_Punish_Item_WZ = {}
    PK_Dead_Punish_Item_WZ[0] = 0
    PK_Dead_Punish_Item_WZ[1] = 2
    PK_Dead_Punish_Item_WZ[2] = 3
    PK_Dead_Punish_Item_WZ[3] = 4
    PK_Dead_Punish_Item_WZ[4] = 6
    PK_Dead_Punish_Item_WZ[5] = 9

    local PK_Dead_Punish_Item = {}
    PK_Dead_Punish_Item[0] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[0])
    PK_Dead_Punish_Item[1] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[1])
    PK_Dead_Punish_Item[2] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[2])
    PK_Dead_Punish_Item[3] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[3])
    PK_Dead_Punish_Item[4] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[4])
    PK_Dead_Punish_Item[5] = GetChaItem(Player, 1, PK_Dead_Punish_Item_WZ[5])
    if Player == 1 and boat == 0 then
        local Item_URE = 0
        local Item_MAXURE = 0
        local Take_Num = 0
        local i = 0
        local j = 0
        local k = 0
        for j = 0, PK_Dead_Punish_Item_Num, 1 do
            if PK_Dead_Punish_Item[j] ~= 0 and PK_Dead_Punish_Item[j] ~= nil then
                local ItemType_Check = Check_Repair_ItemType(PK_Dead_Punish_Item[j])

                if ItemType_Check == 1 then
                    Item_URE = GetItemAttr(PK_Dead_Punish_Item[j], ITEMATTR_URE)
                    Item_MAXURE = GetItemAttr(PK_Dead_Punish_Item[j], ITEMATTR_MAXURE)
                    Take_Num = math.floor(Item_MAXURE * Punish)

                    if Item_URE >= 50 then
                        k = 1
                    end

                    Item_URE = Item_URE - Take_Num
                    if Item_URE < 50 then
                        Item_URE = 49
                    end

                    i = SetItemAttr(PK_Dead_Punish_Item[j], ITEMATTR_URE, Item_URE)
                    if i == 0 then
                        LG("Item_URE", "Normal death deducts attribute failed" .. j)
                    end

                    if k == 1 and Item_URE == 49 then
                        SetChaEquipValid(Player, Dead_Punish_Item_WZ[j], 0)
                    end
                end
            end
        end
        SystemNotice(Player, "PK death penalty: Item durability dropped by 5%")
    end
end

function can_repair_item(role_repair, role_want_repair, Item)
    local Check = 0
    local Sklv = 1
    Check = can_repair_itemLua(role_repair, role_want_repair, Item, Sklv)
    return Check
end

function can_repair_itemLua(role_repair, role_want_repair, Item, Sklv)
    local re_type = IsPlayer(role_repair)
    local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)
    local Item_URE = GetItemAttr(Item, ITEMATTR_URE)
    local Money_Need = get_item_repair_money(Item)
    local Money_Have = GetChaAttr(role_want_repair, ATTR_GD)

    if Item_MAXURE <= 2500 then
        SystemNotice(role_want_repair, "Items durability too low. Unable to repair")
        return 0
    end
    if Item_MAXURE == Item_URE then
        SystemNotice(role_want_repair, "Full durability. No need to repair")
        return 0
    end

    if Money_Have < Money_Need then
        SystemNotice(role_want_repair, "Insufficient gold. Unable to repair")
        return 0
    end

    local i = 0
    i = Check_Repair_ItemType(Item)
    if i == 1 then
        return 1
    end
    SystemNotice(role_want_repair, "Non-repairable item")

    return 0
end

function get_item_repair_money(Item)
    local Money = 0
    local Sklv = 1
    Money = get_item_repair_moneyLua(Item, Sklv)

    if Money < 1 then
        Money = 1
    end

    return Money
end

function get_item_repair_moneyLua(Item, Sklv)
    local Item_Lv = GetItemOriginalLv(Item)
    local RepairPoint = math.floor(math.pow((Item_Lv / 10), 1.7)) - 1
    local Item_URE = GetItemAttr(Item, ITEMATTR_URE)
    local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)

    local URE_repair = math.floor(Item_MAXURE / 50) - math.floor(Item_URE / 50)
    local Money_Need = math.max((URE_repair * RepairPoint), 1)
    return Money_Need
end

function begin_repair_item(role_repair, role_want_repair, Item)
    local Sklv = 1
    begin_repair_itemLua(role_repair, role_want_repair, Item, Sklv)
end

function begin_repair_itemLua(role_repair, role_want_repair, Item, Sklv)
    local Item_MAXURE = GetItemAttr(Item, ITEMATTR_MAXURE)
    local Item_URE = GetItemAttr(Item, ITEMATTR_URE)
    local Money_Need = get_item_repair_money(Item)
    local Money_Have = GetChaAttr(role_want_repair, ATTR_GD)

    Money_Have = Money_Have - Money_Need
    SetCharaAttr(Money_Have, role_want_repair, ATTR_GD)
    ALLExAttrSet(role_want_repair)

    local i = 0
    local j = 0

    Item_URE = Item_MAXURE
    i = SetItemAttr(Item, ITEMATTR_URE, Item_URE)

    if i == 0 then
        LG("Item_URE", "Repair durability failed")
    end
    SystemNotice(role_want_repair, "Repair completed")

    return 1
end

function Check_Repair_ItemType(Item)
    local Item_Type = GetItemType(Item)

    local i = 0
    for i = 0, Item_CanRepair_Num, 1 do
        if Item_CanRepair_ID[i] == Item_Type then
            return 1
        end
    end
    return 0
end

function GetNum_Part1(Num)
    local a = 0
    a = math.floor(Num / 1000000000)
    return a
end

function GetNum_Part2(Num)
    local a = 0
    local b = 0
    a = Num - GetNum_Part1(Num) * 1000000000
    b = math.floor(a / 10000000)
    return b
end

function GetNum_Part3(Num)
    local a = 0
    local b = 0
    a = Num - math.floor(Num / 10000000) * 10000000
    b = math.floor(a / 1000000)
    return b
end

function GetNum_Part4(Num)
    local a = 0
    local b = 0
    a = Num - math.floor(Num / 1000000) * 1000000
    b = math.floor(a / 10000)
    return b
end

function GetNum_Part5(Num)
    local a = 0
    local b = 0
    a = Num - math.floor(Num / 10000) * 10000
    b = math.floor(a / 1000)
    return b
end

function GetNum_Part6(Num)
    local a = 0
    local b = 0
    a = Num - math.floor(Num / 1000) * 1000
    b = math.floor(a / 10)
    return b
end

function GetNum_Part7(Num)
    local a = 0
    local b = 0
    a = Num - math.floor(Num / 10) * 10
    b = math.floor(a / 1)
    return b
end

function SetNum_Part1(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part1(Num)
    b = Part_Num - a
    Num = Num + b * 1000000000
    return Num
end

function SetNum_Part2(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part2(Num)
    b = Part_Num - a
    Num = Num + b * 10000000
    return Num
end

function SetNum_Part3(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part3(Num)
    b = Part_Num - a
    Num = Num + b * 1000000
    return Num
end

function SetNum_Part4(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part4(Num)
    b = Part_Num - a
    Num = Num + b * 10000
    return Num
end

function SetNum_Part5(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part5(Num)
    b = Part_Num - a
    Num = Num + b * 1000
    return Num
end

function SetNum_Part6(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part6(Num)
    b = Part_Num - a
    Num = Num + b * 10
    return Num
end

function SetNum_Part7(Num, Part_Num)
    local a = 0
    local b = 0
    a = GetNum_Part7(Num)
    b = Part_Num - a
    Num = Num + b * 1
    return Num
end

function SetItemForgeParam_MonsterBaoliao(item, Num)
    local i = 0
    local j = 0
    local a = math.random(1, 100)
    local k = 0
    local ItemID = GetItemID(item)
    local MaxHole = GetItemHoleNum(ItemID)
    local Item_Type = GetItemType(item)

    for j = 0, 3, 1 do
        if a <= Item_HoleNum_Monster[j] then
            k = j
            a = 200
        end
    end

    if k > MaxHole then
        k = MaxHole
    end

    if Item_Type == 49 or Item_Type == 50 then
        k = 0
    end

    Num = SetNum_Part1(Num, k)

    i = SetItemForgeParam(item, 1, Num)
    if i == 0 then
        LG("Creat_Item", "set forging content failed")
    end
end

function SetItemForgeParam_PlayerHecheng(item, Num)
    local i = 0
    local j = 0
    local a = math.random(1, 100)
    local k = 0
    local ItemID = GetItemID(item)
    local MaxHole = GetItemHoleNum(ItemID)
    local Item_Type = GetItemType(item)

    for j = 0, 3, 1 do
        if a <= Item_HoleNum_Hecheng[j] then
            k = j
            a = 200
        end
    end

    if k > MaxHole then
        k = MaxHole
    end

    if Item_Type == 49 or Item_Type == 50 then
        k = 0
    end

    Num = SetNum_Part1(Num, k)

    i = SetItemForgeParam(item, 1, Num)
    if i == 0 then
        LG("Creat_Item", "set forging content failed")
    end
end

function SetItemForgeParam_QuestAward(item, Num, item_event)
    local i = 0
    local j = 0
    local a = math.random(1, 100)
    local k = 0
    local ItemID = GetItemID(item)
    local MaxHole = GetItemHoleNum(ItemID)
    local Item_Type = GetItemType(item)

    for j = 0, 3, 1 do
        if a <= Item_HoleNum_Mission_1[j] then
            k = j
            a = 200
        end
    end

    if k > MaxHole then
        k = MaxHole
    end

    if item_event == QUEST_AWARD_SDJ then
        if k == 0 then
            k = 1
        end
    end

    if item_event == QUEST_AWARD_SCBOX then
        k = 2
    end

    if Item_Type == 49 or Item_Type == 50 then
        k = 0
    end

    Num = SetNum_Part1(Num, k)

    i = SetItemForgeParam(item, 1, Num)
    if i == 0 then
        LG("Creat_Item", "set forging content failed")
    end
end

function SetItemForgeParam_Npc_Sale(item, Num)
    local i = 0
    local j = 0
    local a = math.random(1, 100)
    local k = 0

    Num = SetNum_Part1(Num, k)

    i = SetItemForgeParam(item, 1, Num)
    if i == 0 then
        LG("Creat_Item", "set forging content failed")
    end
end

function GetFightGuildLevel()
    local Lv = 0
    local Now_Week = GetNowWeek()
    local Now_Time = GetNowTime()
    local CheckNum = Now_Week * 100 + Now_Time
    if CheckNum >= 422 and CheckNum < 522 then
        Lv = 2
    elseif CheckNum >= 522 and CheckNum < 622 then
        Lv = 1
    elseif CheckNum >= 622 and CheckNum < 700 then
        Lv = 3
    elseif CheckNum >= 0 and CheckNum < 22 then
        Lv = 3
    end

    return Lv
end

function GetFightGuildID(GuildLevel)
    local RedSide = 0
    local BlueSide = 0
    RedSide, BlueSide = GetChallengeGuildID(GuildLevel)
    return RedSide, BlueSide
end

function GetNowWeek()
    local Now_Week = os.date("%w")
    local Now_WeekNum = tonumber(Now_Week)
    return Now_WeekNum
end

function GetNowTime()
    local Now_Time = os.date("%H")
    local NowTimeNum = tonumber(Now_Time)
    return NowTimeNum
end

function CheckItem_Nianshou(Player)
    local Atk = IsPlayer(Player)
    local boat = ChaIsBoat(Player)
    if Atk == 0 or boat == 1 then
        return 0
    end

    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    local head = GetChaItem(Player, 1, 0)
    local body = GetChaItem(Player, 1, 2)
    local hand = GetChaItem(Player, 1, 3)
    local foot = GetChaItem(Player, 1, 4)

    local Head_ID = GetItemID(head)
    local Body_ID = GetItemID(body)
    local Hand_ID = GetItemID(hand)
    local Foot_ID = GetItemID(foot)

    if Body_ID ~= 0825 and Body_ID ~= 2549 then
        return 0
    end

    if Hand_ID ~= 0826 and Hand_ID ~= 2550 then
        return 0
    end

    if Foot_ID ~= 0827 and Foot_ID ~= 2551 then
        return 0
    end

    if Cha_Num == 4 then
        if Head_ID ~= 0828 and Head_ID ~= 2552 then
            return 0
        end
    end

    return 1
end

function CheckItem_Heilong(Player)
    local Atk = IsPlayer(Player)
    local boat = ChaIsBoat(Player)
    if Atk == 0 or boat == 1 then
        return 0
    end

    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    local body = GetChaItem(Player, 1, 2)
    local hand = GetChaItem(Player, 1, 3)
    local foot = GetChaItem(Player, 1, 4)

    local Body_ID = GetItemID(body)
    local Hand_ID = GetItemID(hand)
    local Foot_ID = GetItemID(foot)

    if Body_ID ~= 0845 and Body_ID ~= 2367 then
        return 0
    end

    if Hand_ID ~= 0846 and Hand_ID ~= 2368 then
        return 0
    end

    if Foot_ID ~= 0847 and Foot_ID ~= 2369 then
        return 0
    end

    return 1
end

function CheckItem_pirate(Player)
    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    local body = GetChaItem(Player, 1, 2)
    local hand = GetChaItem(Player, 1, 3)
    local foot = GetChaItem(Player, 1, 4)

    local Body_ID = GetItemID(body)
    local Hand_ID = GetItemID(hand)
    local Foot_ID = GetItemID(foot)
    local body_gem_id = GetItemAttr(body, ITEMATTR_VAL_FUSIONID)
    local hand_gem_id = GetItemAttr(hand, ITEMATTR_VAL_FUSIONID)
    local foot_gem_id = GetItemAttr(foot, ITEMATTR_VAL_FUSIONID)
    if Body_ID < 5000 or Hand_ID < 5000 or Foot_ID < 5000 then
        return 0
    end
    if
        body_gem_id ~= 2530 and body_gem_id ~= 2533 and body_gem_id ~= 2536 and body_gem_id ~= 2539 and
            body_gem_id ~= 2542 and
            body_gem_id ~= 2545
     then
        return 0
    end
    if
        hand_gem_id ~= 2531 and hand_gem_id ~= 2534 and hand_gem_id ~= 2537 and hand_gem_id ~= 2540 and
            hand_gem_id ~= 2543 and
            hand_gem_id ~= 2546
     then
        return 0
    end
    if
        foot_gem_id ~= 2532 and foot_gem_id ~= 2535 and foot_gem_id ~= 2538 and foot_gem_id ~= 2541 and
            foot_gem_id ~= 2544 and
            foot_gem_id ~= 2547
     then
        return 0
    end
    return 1
end

function CheckItem_Death(Player)
    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    local body = GetChaItem(Player, 1, 2)
    local hand = GetChaItem(Player, 1, 3)
    local foot = GetChaItem(Player, 1, 4)

    local Body_ID = GetItemID(body)
    local Hand_ID = GetItemID(hand)
    local Foot_ID = GetItemID(foot)
    local body_gem_id = GetItemAttr(body, ITEMATTR_VAL_FUSIONID)
    local hand_gem_id = GetItemAttr(hand, ITEMATTR_VAL_FUSIONID)
    local foot_gem_id = GetItemAttr(foot, ITEMATTR_VAL_FUSIONID)
    if Body_ID < 5000 or Hand_ID < 5000 or Foot_ID < 5000 then
        body_gem_id = Body_ID
        hand_gem_id = Hand_ID
        foot_gem_id = Foot_ID
        if
            body_gem_id ~= 2817 and body_gem_id ~= 2820 and body_gem_id ~= 2823 and body_gem_id ~= 2826 and
                body_gem_id ~= 2829 and
                body_gem_id ~= 2832
         then
            return 0
        end
        if
            hand_gem_id ~= 2818 and hand_gem_id ~= 2821 and hand_gem_id ~= 2824 and hand_gem_id ~= 2827 and
                hand_gem_id ~= 2830 and
                hand_gem_id ~= 2833
         then
            return 0
        end
        if
            foot_gem_id ~= 2819 and foot_gem_id ~= 2822 and foot_gem_id ~= 2825 and foot_gem_id ~= 2828 and
                foot_gem_id ~= 2831 and
                foot_gem_id ~= 2834
         then
            return 0
        end
    else
        if
            body_gem_id ~= 2817 and body_gem_id ~= 2820 and body_gem_id ~= 2823 and body_gem_id ~= 2826 and
                body_gem_id ~= 2829 and
                body_gem_id ~= 2832
         then
            return 0
        end
        if
            hand_gem_id ~= 2818 and hand_gem_id ~= 2821 and hand_gem_id ~= 2824 and hand_gem_id ~= 2827 and
                hand_gem_id ~= 2830 and
                hand_gem_id ~= 2833
         then
            return 0
        end
        if
            foot_gem_id ~= 2819 and foot_gem_id ~= 2822 and foot_gem_id ~= 2825 and foot_gem_id ~= 2828 and
                foot_gem_id ~= 2831 and
                foot_gem_id ~= 2834
         then
            return 0
        end
    end
    return 1
end

function CheckItem_fighting(Player)
    local cha = TurnToCha(Player)
    local Cha_Num = GetChaTypeID(cha)

    local body = GetChaItem(Player, 1, 2)
    local hand = GetChaItem(Player, 1, 3)
    local foot = GetChaItem(Player, 1, 4)

    local Body_ID = GetItemID(body)
    local Hand_ID = GetItemID(hand)
    local Foot_ID = GetItemID(foot)
    local body_gem_id = GetItemAttr(body, ITEMATTR_VAL_FUSIONID)
    local hand_gem_id = GetItemAttr(hand, ITEMATTR_VAL_FUSIONID)
    local foot_gem_id = GetItemAttr(foot, ITEMATTR_VAL_FUSIONID)
    if Body_ID < 5000 or Hand_ID < 5000 or Foot_ID < 5000 then
        return 0
    end
    if body_gem_id ~= 1124 then
        return 0
    end
    if hand_gem_id ~= 1125 then
        return 0
    end
    if foot_gem_id ~= 1126 then
        return 0
    end

    return 1
end

function Suanming_Money(Player)
    local a = CheckSuanmingType(Player)
    if a == 1 then
        SystemNotice(Player, "Lady Luck shines on you! You have obtained the best Lot!")
        ShangShangQian_Money(Player)
    elseif a == 2 then
        SystemNotice(Player, "Seems to be lucky today. You have obtained a good Lot")
        ShangQian_Money(Player)
    elseif a == 3 then
        ZhongQian_Money(Player)
    elseif a == 4 then
        SystemNotice(Player, "Very unlucky. You have obtained a bad Lot")
        XiaQian_Money(Player)
    elseif a == 5 then
        SystemNotice(Player, "Oh dear! Bad luck has befallen you! You have obtained the worst Lot!")
        XiaXiaQian_Money(Player)
    end
end

function Suanming_Work(Player)
    local a = CheckSuanmingType(Player)
    if a == 1 then
        SystemNotice(Player, "Lady Luck shines on you! You have obtained the best Lot!")
        ShangShangQian_Work(Player)
    elseif a == 2 then
        SystemNotice(Player, "Seems to be lucky today. You have obtained a good Lot")
        ShangQian_Work(Player)
    elseif a == 3 then
        ZhongQian_Work(Player)
    elseif a == 4 then
        SystemNotice(Player, "Very unlucky. You have obtained a bad Lot")
        XiaQian_Work(Player)
    elseif a == 5 then
        SystemNotice(Player, "Oh dear! Bad luck has befallen you! You have obtained the worst Lot!")
        XiaXiaQian_Work(Player)
    end
end

function CheckSuanmingType(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 10
    qian[1] = 35
    qian[2] = 55
    qian[3] = 85
    qian[4] = 100

    for i = 0, 4, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    return b
end

function ShangShangQian_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 10
    qian[1] = 50
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local GiveMoneyNum = 0.01 * math.random(1, 5)
        local GiveMoneyNum_Notice = GiveMoneyNum * 100
        SystemNotice(Player, "Obtain some Gold by luck "..GiveMoneyNum_Notice.."% of extra gold")
        QianAddMoney(Player, 1, GiveMoneyNum)
    elseif b == 2 then
        local statelv = 1
        local time_Bei = math.random(1, 60)
        local statetime = time_Bei * 60
        AddState(Player, Player, STATE_SBBLGZ, statelv, statetime)
        SystemNotice(Player, "Recieved blessing from Goddess Kara. Current region obtained "..time_Bei.." minutes of bonus increased drop rate")
    elseif b == 3 then
        GiveItem(Player, 0, 1092, 1, 0)
    end
end

function ShangQian_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 20
    qian[1] = 60
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local GiveMoneyNum = 0.001 * math.random(1, 9)
        local GiveMoneyNum_Notice = GiveMoneyNum * 100
        SystemNotice(Player, "Obtain some Gold by luck "..GiveMoneyNum_Notice.."% of extra gold")
        QianAddMoney(Player, 1, GiveMoneyNum)
    elseif b == 2 then
        local Give_Money = 1000 * math.random(1, 15)
        QianAddMoney(Player, 2, Give_Money)
    elseif b == 3 then
        local hp_role = Hp(Player)
        local hp_dmg = math.floor(hp_role * 0.9)
        local Give_Money = math.random(10000, 20000)
        Hp_Endure_Dmg(Player, hp_dmg)
        QianAddMoney(Player, 2, Give_Money)
        SystemNotice(Player, "Smash by some gold coins. Almost die! Who throw these coins!")
    end
end

function ZhongQian_Money(Player)
    SystemNotice(Player, "Today is so boring, nothing ever happens...")
end

function XiaQian_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 20
    qian[1] = 100

    for i = 0, 1, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Give_Money = math.random(100, 5000)
        Give_Money = Give_Money * -1
        QianAddMoney(Player, 2, Give_Money)
    elseif b == 2 then
        local GiveMoneyNum = 0.001 * math.random(1, 9)
        local GiveMoneyNum_Notice = GiveMoneyNum * 100
        SystemNotice(Player, "Accidentally loses "..GiveMoneyNum_Notice.."%% of gold")
        GiveMoneyNum = GiveMoneyNum * -1
        QianAddMoney(Player, 1, GiveMoneyNum)
    end
end

function XiaXiaQian_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 20
    qian[1] = 100

    for i = 0, 1, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Give_Money = math.random(10000, 30000)
        Give_Money = Give_Money * -1
        QianAddMoney(Player, 2, Give_Money)
    elseif b == 2 then
        local GiveMoneyNum = 0.01 * math.random(1, 2)
        local GiveMoneyNum_Notice = GiveMoneyNum * 100
        SystemNotice(Player, "Accidentally loses "..GiveMoneyNum_Notice.."%% of gold")
        GiveMoneyNum = GiveMoneyNum * -1
        QianAddMoney(Player, 1, GiveMoneyNum)
    end
end

function ShangShangQian_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 20
    qian[1] = 50
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local GiveExpNum = 0.01 * math.random(1, 5)
        local GiveExpNum_Notice = GiveExpNum * 100
        SystemNotice(Player, "Accidentally obtained "..GiveExpNum_Notice.."%% EXP")
        QianAddExp(Player, GiveExpNum, 1)
    elseif b == 2 then
        local statelv = 1
        local time_Bei = math.random(1, 60)
        local statetime = time_Bei * 60
        AddState(Player, Player, STATE_SBJYGZ, statelv, statetime)
        SystemNotice(Player, "Recieved blessing from Goddess Kara. Current region obtained "..time_Bei .." minutes of bonus experience increase")
    elseif b == 3 then
        QianAddState(Player, 1)
    end
end

function ShangQian_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 100

    for i = 0, 0, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Lv_role = Lv(Player)
        local GiveExpNum = Lv_role * math.random(50, 500)
        QianAddExp(Player, GiveExpNum, 2)
    end
end

function ZhongQian_Work(Player)
    SystemNotice(Player, "Today is so boring, nothing ever happens...")
end

function XiaQian_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 100

    for i = 0, 0, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Lv_role = Lv(Player)
        local GiveExpNum = Lv_role * math.random(50, 500)
        GiveExpNum = GiveExpNum * -1
        QianAddExp(Player, GiveExpNum, 2)
    end
end

function XiaXiaQian_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 30
    qian[1] = 70
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local GiveExpNum = 0.01 * math.random(1, 2)
        local GiveExpNum_Notice = GiveExpNum * 100
        SystemNotice(Player, "Accidentally lost "..GiveExpNum_Notice.."%% EXP")
        GiveExpNum = GiveExpNum * -1
        QianAddExp(Player, GiveExpNum, 1)
    elseif b == 2 then
        local Lv_role = Lv(Player)
        local GiveExpNum = Lv_role * math.random(100, 1000)
        GiveExpNum = GiveExpNum * -1
        QianAddExp(Player, GiveExpNum, 2)
    elseif b == 3 then
        local hp = Hp(Player)
        local hp_dmg = math.floor(hp * 0.9)
        Hp_Endure_Dmg(Player, hp_dmg)
        SystemNotice(Player, "Character burst and strucked by lightning and almost die....")
    end
end

function QianAddMoney(Player, Type, Num)
    if Type == 1 then
        local Money_Have = GetChaAttr(Player, ATTR_GD)
        local Money_Add = Money_Have * Num

        Money_Have = Money_Have + Money_Add
        Money_Add = math.floor(Money_Add)
        Money_Have = math.floor(Money_Have)

        SetCharaAttr(Money_Have, Player, ATTR_GD)
        ALLExAttrSet(Player)

        if Money_Add > 0 then
            Num = Num * 100
            SystemNotice(Player, "God of Wealth drops a a bag of gold coins into your coin pouch "..Num.."%% of your gold")
            if Money_Add >= 200000 then
                Notice(GetChaDefaultName(Player).." draws a lot and obtained "..AddComma(Money_Add))
            end
        elseif Money_Add < 0 then
            Num = Num * -100
            SystemNotice(Player, "God of Misfortune struck you! Loses "..Num.."%% gold")
        end
    elseif Type == 2 then
        local Money_Have = GetChaAttr(Player, ATTR_GD)
        local Money_Add = Num

        Money_Have = Money_Have + Money_Add

        if Money_Have < 0 then
            Money_Have = 0
        end

        SetCharaAttr(Money_Have, Player, ATTR_GD)
        ALLExAttrSet(Player)
        if Num > 0 then
            SystemNotice(Player, "Found "..AddComma(Num))
            if Num >= 200000 then
                Notice(GetChaDefaultName(Player).. " draws a lot and obtained "..AddComma(Num))
            end
        elseif Num < 0 then
            Num = Num * -1
            SystemNotice(Player, "You found out that you have lost "..AddComma(Num))
        end
    end
end

function QianAddExp(Player, Num, type)
    local lv = GetChaAttr(Player, ATTR_LV)
    local exp = Exp(Player)
    local nlexp = GetChaAttrI(Player, ATTR_NLEXP)
    local clexp = GetChaAttrI(Player, ATTR_CLEXP)
    local ThisLvexp = nlexp - clexp
    local ExpGet = ThisLvexp * Num

    if type == 2 then
        ExpGet = Num
        if lv >= 80 then
            ExpGet = math.floor(ExpGet / 50)
        end
    end

    ExpGet = math.floor(ExpGet)

    exp = exp + ExpGet

    if exp > nlexp then
        exp = nlexp + math.floor((exp - nlexp) / 50)
    end

    if exp < 0 then
        exp = 0
    end

    SetChaAttrI(Player, ATTR_CEXP, exp)

    if lv >= 80 then
        ExpGet = ExpGet * 50
    end

    if ExpGet > 0 then
        SystemNotice(Player, "Accidentally obtained "..ExpGet.." EXP")
        if ExpGet >= 200000 then
            local cha_name = GetChaDefaultName(Player)
            Notice(cha_name .. " draws a lot and obtained "..ExpGet.." EXP")
        end
    elseif ExpGet < 0 then
        ExpGet = ExpGet * -1
        SystemNotice(Player, "Experience lost: "..ExpGet.." EXP")
    end
end

function QianAddState(Player, Type)
    local State = {}
    local StateName = {}

    State[0] = STATE_PKZDYS
    State[1] = STATE_PKSFYS
    State[2] = STATE_PKMNYS
    State[3] = STATE_PKJZYS
    State[4] = STATE_PKKBYS

    StateName[0] = "Attack"
    StateName[1] = "Defense"
    StateName[2] = "Max HP"
    StateName[3] = "Hit Rate"
    StateName[4] = "Attack Speed"

    local i = math.random(0, 4)
    local statelv = 0
    local TimeRange = 0
    if Type == 1 then
        TimeRange = 60
    elseif Type == 2 then
        TimeRange = 30
    end
    local statetime = math.random(1, TimeRange)
    statetime = statetime * 60

    statelv = 10
    AddState(Player, Player, State[i], statelv, statetime)
    SystemNotice(Player, "You sense that your "..StateName[i].." increased")
end

function CheckGetMapPos(Player, pos_x, pos_y, MapName)
    local Cha_Boat = GetCtrlBoat(Player)
    local ChaIsBoat = 0
    if Cha_Boat ~= nil then
        Player = Cha_Boat
    end

    local Cha_pos_x, Cha_pos_y = GetChaPos(Player)
    local map_name = GetChaMapName(Player)

    local Cha_x_min = pos_x * 100 - 400
    local Cha_x_max = pos_x * 100 + 400
    local Cha_y_min = pos_y * 100 - 400
    local Cha_y_max = pos_y * 100 + 400

    if map_name ~= MapName then
        return 0
    end

    if Cha_pos_x < Cha_x_min or Cha_pos_x > Cha_x_max then
        return 0
    end

    if Cha_pos_y < Cha_y_min or Cha_pos_y > Cha_y_max then
        return 0
    end

    return 1
end

function SuanmingTeshu_Money(Player)
    local a = CheckSuanmingTypeTeshu(Player)
    if a == 1 then
        SystemNotice(Player, "Lady Luck shines on you! You have obtained the best Lot!")
        ShangShangQianTeshu_Money(Player)
    elseif a == 2 then
        SystemNotice(Player, "Seems to be lucky today. You have obtained a good Lot")
        ShangQianTeshu_Money(Player)
    elseif a == 3 then
        ZhongQian_Money(Player)
    end
end

function SuanmingTeshu_Work(Player)
    local a = CheckSuanmingTypeTeshu(Player)
    if a == 1 then
        SystemNotice(Player, "Lady Luck shines on you! You have obtained the best Lot!")
        ShangShangQianTeshu_Work(Player)
    elseif a == 2 then
        SystemNotice(Player, "Seems to be lucky today. You have obtained a good Lot")
        ShangQianTeshu_Work(Player)
    elseif a == 3 then
        ZhongQian_Work(Player)
    end
end

function CheckSuanmingTypeTeshu(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 30
    qian[1] = 80
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    return b
end

function ShangShangQianTeshu_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 5
    qian[1] = 60
    qian[2] = 100

    for i = 0, 2, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local GiveMoneyNum = 10000 * math.random(10, 100)
        QianAddMoney(Player, 2, GiveMoneyNum)
    elseif b == 2 then
        local statelv = 1
        local time_Bei = math.random(1, 60)
        local statetime = time_Bei * 60
        AddState(Player, Player, STATE_SBBLGZ, statelv, statetime)
        SystemNotice(Player, "Recieved blessing from Goddess Kara. Current region obtained "..time_Bei.." minutes of bonus increased drop rate")
    elseif b == 3 then
        GiveItem(Player, 0, 1092, 1, 0)
    end
end

function ShangQianTeshu_Money(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 60
    qian[1] = 100

    for i = 0, 1, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Give_Money = 1000 * math.random(1, 20)
        QianAddMoney(Player, 2, Give_Money)
    elseif b == 2 then
        local hp = Hp(Player)
        local hp_dmg = math.floor(hp * 0.9)
        local Give_Money = math.random(10000, 30000)
        Hp_Endure_Dmg(Player, hp_dmg)
        QianAddMoney(Player, 2, Give_Money)
        SystemNotice(Player, "Smash by some gold coins. Almost die! Who throw these coins!")
    end
end

function ShangShangQianTeshu_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 30
    qian[1] = 60
    qian[2] = 90
    qian[3] = 100

    for i = 0, 3, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Lv_role = Lv(Player)
        local GiveExpNum = (Lv_role * Lv_role * math.random(10, 100)) * (1 / math.max(1, (50 - Lv_role)))
        QianAddExp(Player, GiveExpNum, 2)
    elseif b == 2 then
        local statelv = 1
        local time_Bei = math.random(1, 60)
        local statetime = time_Bei * 60
        AddState(Player, Player, STATE_SBJYGZ, statelv, statetime)
        SystemNotice(Player, "Recieved blessing from Goddess Kara. Current region obtained "..time_Bei .." minutes of bonus experience increase")
    elseif b == 3 then
        QianAddState(Player, 1)
    elseif b == 4 then
        QianAddState(Player, 1)
    end
end

function ShangQianTeshu_Work(Player)
    local a = math.random(1, 100)
    local i = 0
    local b = 0
    local qian = {}
    qian[0] = 50
    qian[1] = 100

    for i = 0, 1, 1 do
        if qian[i] >= a then
            b = i + 1
            break
        end
    end

    if b == 1 then
        local Lv_role = Lv(Player)
        local GiveExpNum = Lv_role * Lv_role * math.random(5, 50) * (1 / math.max(1, (50 - Lv_role)))
        QianAddExp(Player, GiveExpNum, 2)
    elseif b == 2 then
        QianAddState(Player, 2)
    end
end

function QianAddStatePoint(Player, Num)
    local CheckNum = CheckStatePointHasGet(Player)
    local a = 1 / math.pow(2, (CheckNum - 1))
    local check = Percentage_Random(a)
    if check == 1 then
        local attr_ap = Attr_ap(Player)
        local ap_extre = Num
        attr_ap = attr_ap + ap_extre
        SetCharaAttr(attr_ap, Player, ATTR_AP)
        Notice(GetChaDefaultName(Player).." draws a lot and obtained 1 bonus stat point")
        LG("Add_StatePoint", GetChaDefaultName(Player).." obtained Attributes point: "..Num.." point")
    else
        local Lv_role = Lv(Player)
        local GiveExpNum = Lv_role * math.random(100, 1000)
        QianAddExp(Player, GiveExpNum, 2)
    end
end

function CheckStatePointHasGet(Player)
    local str = GetChaAttr(Player, ATTR_BSTR)
    local con = GetChaAttr(Player, ATTR_BCON)
    local agi = GetChaAttr(Player, ATTR_BAGI)
    local dex = GetChaAttr(Player, ATTR_BDEX)
    local sta = GetChaAttr(Player, ATTR_BSTA)
    local ap = GetChaAttr(Player, ATTR_AP)
    local StatePointTotal_Have = str + con + agi + dex + sta + ap - 25
    local Lv_role = Lv(Player)
    local StatePointTotal = 3 + Lv_role + math.floor(Lv_role / 10) * 4 + math.max(0, (Lv_role - 59))
    local Check = StatePointTotal_Have - StatePointTotal
    return Check
end

function GetTheMapPos(Player, type)
    local MapList = {}

    MapList[0] = "NoMap"
    MapList[1] = "garner"
    MapList[2] = "magicsea"
    MapList[3] = "darkblue"

    local PosDateNum = 8
    local PosDateMap = {}
    local PosDateMax_X = {}
    local PosDateMin_X = {}
    local PosDateMax_Y = {}
    local PosDateMin_Y = {}

    PosDateMap[0] = 1
    PosDateMap[1] = 1
    PosDateMap[2] = 1
    PosDateMap[3] = 1
    PosDateMap[4] = 2
    PosDateMap[5] = 3
    PosDateMap[6] = 1
    PosDateMap[7] = 2
    PosDateMap[8] = 1

    PosDateMax_X[0] = 800
    PosDateMax_X[1] = 940
    PosDateMax_X[2] = 1023
    PosDateMax_X[3] = 1012
    PosDateMax_X[4] = 850
    PosDateMax_X[5] = 2810
    PosDateMax_X[6] = 380
    PosDateMax_X[7] = 1420
    PosDateMax_X[8] = 1614

    PosDateMin_X[0] = 700
    PosDateMin_X[1] = 840
    PosDateMin_X[2] = 919
    PosDateMin_X[3] = 912
    PosDateMin_X[4] = 750
    PosDateMin_X[5] = 2710
    PosDateMin_X[6] = 280
    PosDateMin_X[7] = 1320
    PosDateMin_X[8] = 1514

    PosDateMax_Y[0] = 1766
    PosDateMax_Y[1] = 1350
    PosDateMax_Y[2] = 1054
    PosDateMax_Y[3] = 2950
    PosDateMax_Y[4] = 3083
    PosDateMax_Y[5] = 691
    PosDateMax_Y[6] = 1725
    PosDateMax_Y[7] = 3000
    PosDateMax_Y[8] = 2695

    PosDateMin_Y[0] = 1666
    PosDateMin_Y[1] = 1250
    PosDateMin_Y[2] = 1017
    PosDateMin_Y[3] = 2850
    PosDateMin_Y[4] = 2982
    PosDateMin_Y[5] = 591
    PosDateMin_Y[6] = 1675
    PosDateMin_Y[7] = 2900
    PosDateMin_Y[8] = 2615

    local PosDateGet = math.random(0, PosDateNum)

    local Pos_Map = PosDateMap[PosDateGet]

    local Pos_X = math.random(PosDateMin_X[PosDateGet], PosDateMax_X[PosDateGet])

    local Pos_Y = math.random(PosDateMin_Y[PosDateGet], PosDateMax_Y[PosDateGet])

    return Pos_X, Pos_Y, Pos_Map
end

function GetTheMapPos_JLB(Player, type)
    local MapList = {}
    MapList[0] = "NoMap"
    MapList[1] = "jialebi"

    local PosDateNum = 3
    local PosDateMap = {}
    local PosDateMax_X = {}
    local PosDateMin_X = {}
    local PosDateMax_Y = {}
    local PosDateMin_Y = {}

    PosDateMap[0] = 1
    PosDateMap[1] = 1
    PosDateMap[2] = 1
    PosDateMap[3] = 1

    PosDateMax_X[0] = 476
    PosDateMax_X[1] = 460
    PosDateMax_X[2] = 469
    PosDateMax_X[3] = 477

    PosDateMin_X[0] = 466
    PosDateMin_X[1] = 452
    PosDateMin_X[2] = 462
    PosDateMin_X[3] = 470

    PosDateMax_Y[0] = 1052
    PosDateMax_Y[1] = 980
    PosDateMax_Y[2] = 1000
    PosDateMax_Y[3] = 1048

    PosDateMin_Y[0] = 1000
    PosDateMin_Y[1] = 954
    PosDateMin_Y[2] = 980
    PosDateMin_Y[3] = 1036

    local PosDateGet = math.random(0, PosDateNum)
    local Pos_Map = PosDateMap[PosDateGet]
    local Pos_X = math.random(PosDateMin_X[PosDateGet], PosDateMax_X[PosDateGet])
    local Pos_Y = math.random(PosDateMin_Y[PosDateGet], PosDateMax_Y[PosDateGet])
    return Pos_X, Pos_Y, Pos_Map
end

function XianJing(Player, type)
    if type == 1 then
        local hp = Hp(Player)
        local hp_dmg = math.floor(hp * 0.9)
        Hp_Endure_Dmg(Player, hp_dmg)
        SystemNotice(Player, "Seriously injured by traps laid by pirates")
    elseif type == 2 then
        local hp = Hp(Player)
        local hp_dmg = math.floor(hp * 0.3)
        Hp_Endure_Dmg(Player, hp_dmg)
        SystemNotice(Player, "Almost poisoned by pirate trap. Escaped in timeLuckily")
    end
end

function MapRandomtele(Player)
    local Birth = {}
    Birth[0] = "Argent City"
    Birth[1] = "Thundoria Castle"
    Birth[2] = "Shaitan City"
    Birth[3] = "Icicle Castle"
    Birth[4] = "Chaldea Haven"
    Birth[5] = "Andes Forest Haven"
    Birth[6] = "Abandon Mine Haven"
    Birth[7] = "Rockery Haven"
    Birth[8] = "Valhalla Haven"
    Birth[9] = "Solace Haven"
    Birth[10] = "Oasis Haven"
    Birth[11] = "Babul Haven"
    Birth[12] = "Icicle Haven"
    Birth[13] = "Atlantis Haven"
    Birth[14] = "Skeleton Haven"
    Birth[15] = "Icespire Haven"
    Birth[16] = "Zephyr Isle"
    Birth[17] = "Glacier Isle"
    Birth[18] = "Outlaw Isle"
    Birth[19] = "Isle of Chill"
    Birth[20] = "Canary Isle"
    Birth[21] = "Cupid Isle"
    Birth[22] = "Isle of Fortune"
    Birth[23] = "Thundoria Castle"
    Birth[23] = "Thundoria Harbor"
    Birth[24] = "Sacred Snow Mountain"
    Birth[25] = "Andes Forest Haven"
    Birth[26] = "Oasis Haven"
    Birth[27] = "Icespire Haven"
    Birth[28] = "Lone Tower Entrace"
    Birth[29] = "Barren Cavern Entrance"
    Birth[30] = "Abandon Mine 2"
    Birth[31] = "Silver Mine 2"
    Birth[32] = "Silver Mine 3"
    Birth[33] = "Abandon Mine 1"
    Birth[34] = "Lone Tower 2"
    Birth[35] = "Lone Tower 3"
    Birth[36] = "Lone Tower 4"
    Birth[37] = "Lone Tower 5"
    Birth[38] = "Lone Tower 6"
    Birth[39] = "Argent Bar"

    local PosRandom = math.random(0, 39)
    DelBagItem(Player, 1093, 1)
    MoveCity(Player, Birth[PosRandom])
end

function check_item_valid(Player, Item)
    local Item_type = GetItemType(Item)
    local Item_URE = GetItemAttr(Item, ITEMATTR_URE)

    if Item_type <= 29 or Item_type == 59 then
        if Item_URE ~= 0 and Item_URE <= 49 then
            return 0
        else
            return 1
        end
    end
    return 1
end

function ai_timer(c, freq, time)
    local ResumeFreq = 10
    local Tick = GetChaParam(c, 1)
    local IsLiving = -1
    SetChaParam(c, 1, Tick + freq * time)
    if math.fmod(Tick, ResumeFreq) == 0 and Tick > 0 then
        if IsLiving == -1 then
            IsLiving = IsChaLiving(c)
        end
        if IsLiving == 1 then
            if GetChaAttr(c, ATTR_HP) < GetChaAttr(c, ATTR_MXHP) then
                Resume(c)
            end
        end
    end
    ai_loop(c)
end

function cha_timer(Player, freq, time)
    local ResumeFreq = 5
    local Tick = GetChaParam(Player, 1)
    local IsLiving = -1
    SetChaParam(Player, 1, Tick + freq * time)
    if math.fmod(Tick, ResumeFreq) == 0 and Tick > 0 then
		if IsLiving == -1 then
			IsLiving = IsChaLiving(Player)
		end
		if IsLiving == 1 then
			Resume(Player)
		end
    end
    if IsPlayer(Player) == 1 then
        FairySys(Player, Tick)
        if Server.Sys.MasterAccount == true then
            MasterAccount(Player)
        end
    end
end

function Give_ElfEXP_MISSION(Player, num)
    local Item = GetChaItem(Player, 2, 2)
    local Elf_EXP = GetItemAttr(Item, ITEMATTR_ENERGY)
    local Elf_MaxEXP = GetItemAttr(Item, ITEMATTR_MAXENERGY)
    local Elf_URE = GetItemAttr(Item, ITEMATTR_URE)
    if Elf_URE <= 50 then
        SystemNotice(Player, "Fairy cannot gain any Growth as it is low on stamina")
        return 0
    else
        Elf_URE = Elf_URE - 40
        SetItemAttr(Item, ITEMATTR_URE, Elf_URE)
        num = math.min(num, Elf_MaxEXP - Elf_EXP)
        SetItemAttr(Item, ITEMATTR_ENERGY, num)
    end
    return 1
end

function GetElfSkillLv(Param, Skill)
    local Skid2 = GetNum_Part2(Param)
    local Sklv3 = GetNum_Part3(Param)
    local Skid4 = GetNum_Part4(Param)
    local Sklv5 = GetNum_Part5(Param)
    local Skid6 = GetNum_Part6(Param)
    local Sklv7 = GetNum_Part7(Param)
    if Skill == Skid2 then
        return Sklv3
    elseif Skill == Skid4 then
        return Sklv5
    elseif Skill == Skid6 then
        return Sklv7
    end
    return 0
end

function CheckElfHaveSkill(Param, SkillLv, SkillID)
    local Skid2 = GetNum_Part2(Param)
    local Sklv3 = GetNum_Part3(Param)
    local Skid4 = GetNum_Part4(Param)
    local Sklv5 = GetNum_Part5(Param)
    local Skid6 = GetNum_Part6(Param)
    local Sklv7 = GetNum_Part7(Param)
    if Sklv3 == SkillLv and Skid2 == SkillID then
        return 1
    elseif Skid2 == SkillID then
        return 2
    end
    if Sklv5 == SkillLv and Skid4 == SkillID then
        return 1
    elseif Skid4 == SkillID then
        return 2
    end
    if Sklv7 == SkillLv and Skid6 == SkillID then
        return 1
    elseif Skid6 == SkillID then
        return 2
    end
    return 0
end

function CheckHaveElf(Player)
    local Item = GetEquipItemP(Player, 16)
    if Item == nil then
        return 0
    end

    local Item_Type = GetItemType(Item)

    if Item_Type ~= 59 then
        return 0
    else
        local ELf_URE = GetItemAttr(Item, ITEMATTR_URE)

        if ELf_URE <= 49 then
            return 0
        end
    end

    return Item
end

function ElfSKill_PowerSheild(Player, Elf_Item, Num, Damage)
    if Damage <= 0 then
        return 0
    end
    local role_hp = Hp(Player)
    local role_maxhp = Mxhp(Player)
    local havehp = role_maxhp / role_hp
    if havehp < 5 then
        return 0
    end
    local Elf_SheildLv = GetElfSkill_Lv(Num, 1)
    Damage = math.floor(Damage * (0.3 + Elf_SheildLv * 0.15))

    local Item_URE = GetItemAttr(Elf_Item, ITEMATTR_URE)
    local Dmg_Take_rad = 10
    local Elf_Dmg_CanTake = (Item_URE - 50) / Dmg_Take_rad

    if Elf_Dmg_CanTake >= Damage then
        local Elf_URE_Take = math.floor(Damage * Dmg_Take_rad)
        local Elf_URE_Notice = math.floor(Elf_URE_Take / 50)
        Take_ElfURE(Player, Elf_Item, Elf_URE_Take)
        SystemNotice(Player, "Fairy absorbed damage: " .. Damage)
        return Damage
    else
        SystemNotice(Player, "Fairy does not have enough stamina to activate Protection")
        return 0
    end
end

function GetElfSkill_Lv(Num, SkillNum)
    local Part2 = GetNum_Part2(Num)
    local Part3 = GetNum_Part3(Num)
    local Part4 = GetNum_Part4(Num)
    local Part5 = GetNum_Part5(Num)
    local Part6 = GetNum_Part6(Num)
    local Part7 = GetNum_Part7(Num)

    if SkillNum == Part2 then
        return Part3
    elseif SkillNum == Part4 then
        return Part5
    elseif SkillNum == Part6 then
        return Part7
    end
    return 0
end

function ElfSKill_ElfCrt(Player, Elf_Item, Num)
    local Elf_SkillLv = GetElfSkill_Lv(Num, 2)
    local Item_URE = GetItemAttr(Elf_Item, ITEMATTR_URE)
    if Item_URE < 50 then
        SystemNotice(Player, "Fairy does not have enough stamina to activate Berserk")
        return 0
    end
    local b = (Elf_SkillLv * 2 + 1) * 0.01
    local a = Percentage_Random(b)
    if a == 1 then
        Take_ElfURE(Player, Elf_Item, Elf_SkillLv)
        return 1
    else
        return 0
    end
end

function ElfSkill_MagicAtk(dmg, Player)
    local Elf_Item = CheckHaveElf(Player)
    if Elf_Item ~= 0 then
        local Num = GetItemForgeParam(Elf_Item, 1)
        local CheckElfSkill = CheckElfHaveSkill(Num, 0, 3)
        if CheckElfSkill == 2 then
            local Elf_SkillLv = GetElfSkill_Lv(Num, 3)
            local Item_URE = GetItemAttr(Elf_Item, ITEMATTR_URE)
            if Item_URE <= 50 then
                SystemNotice(Player, "Fairy does not have enough stamina to activate any skill")
                return 0
            else
                if Elf_SkillLv == 1 then
                    Take_ElfURE(Player, Elf_Item, 1)
                    return dmg * 0.05 + 5
                elseif Elf_SkillLv == 2 then
                    Take_ElfURE(Player, Elf_Item, 2)
                    return dmg * 0.08 + 8
                elseif Elf_SkillLv == 3 then
                    Take_ElfURE(Player, Elf_Item, 3)
                    return dmg * 0.1 + 10
                end
            end
        end
    end
    return 0
end

function ElfSkill_HpResume(Player)
    local Elf_Item = CheckHaveElf(Player)
    if Elf_Item ~= 0 then
        local Num = GetItemForgeParam(Elf_Item, 1)
        local CheckElfSkill = CheckElfHaveSkill(Num, 0, 4)
        if CheckElfSkill == 2 then
            local Elf_SkillLv = GetElfSkill_Lv(Num, 4)
            local Item_URE = GetItemAttr(Elf_Item, ITEMATTR_URE)
            if Item_URE <= 50 then
                SystemNotice(Player, "Fairy does not have enough stamina to activate any skill")
                return 0
            else
                if Elf_SkillLv == 1 then
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 10
                elseif Elf_SkillLv == 2 then
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 20
                elseif Elf_SkillLv == 3 then
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 35
                end
            end
        end
    end
    return 0
end

function ElfSkill_SpResume(Player)
    local Elf_Item = CheckHaveElf(Player)
    if Elf_Item ~= 0 then
        local Num = GetItemForgeParam(Elf_Item, 1)
        local CheckElfSkill = CheckElfHaveSkill(Num, 0, 5)
        if CheckElfSkill == 2 then
            local Elf_SkillLv = GetElfSkill_Lv(Num, 5)
            local Item_URE = GetItemAttr(Elf_Item, ITEMATTR_URE)
            if Item_URE <= 50 then
                SystemNotice(Player, "Fairy does not have enough stamina to activate any skill")
                return 0
            else
                if Elf_SkillLv == 1 then
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 10                     
                elseif Elf_SkillLv == 2 then      
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 20                     
                elseif Elf_SkillLv == 3 then      
                    Take_ElfURE(Player, Elf_Item, 2)
                    return 35
                end
            end
        end
    end
    return 0
end

function CreditExchangeImpl(Player, tp)
    local i = CheckBagItem(Player, 3849)
    if i == 1 then
        local Item = GetChaItem2(Player, 2, 3849)
        local rongyu_num = GetItemAttr(Item, ITEMATTR_VAL_STR)
        if rongyu_num <= 0 then
            SystemNotice(Player, "You will not receive any blessing without a postive Honor value!")
            return
        end
        local middle = 0
        if rongyu_num < 30000 then
            middle = 30000 - rongyu_num
        end
        middle = math.floor(middle / 2)
        local exp_star = GetChaAttr(Player, ATTR_CEXP)
        local job = GetChaAttr(Player, ATTR_JOB)
        local lv = GetChaAttr(Player, ATTR_LV)
        local money_num = rongyu_num * 100
        local exp_num = rongyu_num * 5 + exp_star
        local rad = math.random(1, 30000)
        local cha_type = GetChaTypeID(Player)
        local cha_namea = GetChaDefaultName(Player)
        LG("star_rongyuzhichange_lg", cha_namea, tp, lv, exp_star, job, cha_type)
        if tp == 0 or tp == 1 or tp == 2 then
            if lv >= 15 and lv <= 40 then
                money_num = rongyu_num * 200
            elseif lv >= 41 and lv <= 60 then
                money_num = rongyu_num * 250
            elseif lv >= 61 then
                money_num = rongyu_num * 300
            end
            AddMoney(Player, 0, money_num)
            LG("star_rongyuzhichange_lg", cha_namea .. "tp==0 or tp==1 or tp==2 obtain gold" .. money_num)
        elseif tp == 3 or tp == 4 or tp == 5 then
            local dif_exp = rongyu_num * 20 + exp_star - DEXP[lv + 1]
            if lv >= 15 and lv <= 30 then
                exp_num = rongyu_num * 10 + exp_star
                local a1 = math.floor(rongyu_num * 10)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 31 and lv <= 40 then
                exp_num = rongyu_num * 13 + exp_star
                local a1 = math.floor(rongyu_num * 13)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 41 and lv <= 50 then
                exp_num = rongyu_num * 22 + exp_star
                local a1 = math.floor(rongyu_num * 22)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 51 and lv <= 60 then
                exp_num = rongyu_num * 44 + exp_star
                local a1 = math.floor(rongyu_num * 44)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 61 and lv <= 70 then
                exp_num = rongyu_num * 102 + exp_star
                local a1 = math.floor(rongyu_num * 102)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 71 and lv <= 78 then
                exp_num = rongyu_num * 270 + exp_star
                local a1 = math.floor(rongyu_num * 270)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv == 79 and dif_exp <= 0 then
                exp_num = rongyu_num * 270 + exp_star
                local a1 = math.floor(rongyu_num * 270)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv == 79 and dif_exp > 0 then
                exp_num = dif_exp * 0.02 + DEXP[lv + 1]
                local a1 = math.floor(rongyu_num * 270)
                SystemNotice(Player, "Obtained EXP " .. a1)
            elseif lv >= 80 then
                exp_num = rongyu_num * 270 * 0.02 + exp_star
                local a1 = math.floor(rongyu_num * 270)
                SystemNotice(Player, "Obtained EXP " .. a1)
            end
            SetChaAttrI(Player, ATTR_CEXP, exp_num)
            local lg_exp = exp_num - exp_star
            LG("star_rongyuzhichange_lg", cha_namea .. "tp==3 or tp==4 or tp==5 obtain experience" .. lg_exp)
        elseif tp == 6 or tp == 7 or tp == 8 then
            if lv >= 15 and lv <= 40 then
                if rad <= rongyu_num or rongyu_num >= 30000 then
                    GiveItem(Player, 0, 3458, 1, 4)
                    LG("star_rongyuzhichange_lg", cha_namea .. "Lv>=15 and Lv<=40 will obtained equipment ID=" .. 3458)
                elseif rad > rongyu_num and rad <= middle then
                    AddMoney(Player, 0, money_num)
                elseif rad > middle and rad <= 30000 then
                    SetChaAttrI(Player, ATTR_CEXP, exp_num)
                    local a1 = math.floor(rongyu_num * 5)
                    SystemNotice(Player, "Obtained EXP " .. a1)
                end
            elseif lv >= 41 and lv <= 60 then
                if rad <= rongyu_num or rongyu_num >= 30000 then
                    local rad1 = math.random(1, 12)
                    local Lg_ID = 787
                    if rad1 == 1 then
                        Lg_ID = 787
                    elseif rad1 == 2 then
                        Lg_ID = 791
                    elseif rad1 == 3 then
                        Lg_ID = 794
                    elseif rad1 == 4 then
                        Lg_ID = 801
                    elseif rad1 == 5 then
                        Lg_ID = 805
                    elseif rad1 == 6 then
                        Lg_ID = 797
                    elseif rad1 == 7 then
                        Lg_ID = 765
                    elseif rad1 == 8 then
                        Lg_ID = 768
                    elseif rad1 == 9 then
                        Lg_ID = 772
                    elseif rad1 == 10 then
                        Lg_ID = 775
                    elseif rad1 == 11 then
                        Lg_ID = 779
                    elseif rad1 == 12 then
                        Lg_ID = 783
                    end
                    GiveItem(Player, 0, Lg_ID, 1, 4)
                    LG("star_rongyuzhichange_lg", cha_namea .. "lv>=41 and lv<=60receive apparelID=" .. Lg_ID)
                elseif rad > rongyu_num and rad <= middle then
                    AddMoney(Player, 0, money_num)
                elseif rad > middle and rad <= 30000 then
                    SetChaAttrI(Player, ATTR_CEXP, exp_num)
                    local a1 = math.floor(rongyu_num * 5)
                    SystemNotice(Player, "Obtained EXP " .. a1)
                end
            elseif lv >= 61 then
                if rad <= rongyu_num or rongyu_num >= 30000 then
                    local rad2 = math.random(1, 3)
                    local rad3 = math.random(1, 4)
                    local Lg_ID = 2530
                    if job == 8 then
                        if rad2 == 1 then
                            Lg_ID = 2530
                        elseif rad2 == 2 then
                            Lg_ID = 2531
                        elseif rad2 == 3 then
                            Lg_ID = 2532
                        end
                    elseif job == 9 then
                        if rad2 == 1 then
                            Lg_ID = 2533
                        elseif rad2 == 2 then
                            Lg_ID = 2534
                        elseif rad2 == 3 then
                            Lg_ID = 2535
                        end
                    elseif job == 12 then
                        if rad2 == 1 then
                            Lg_ID = 2536
                        elseif rad2 == 2 then
                            Lg_ID = 2537
                        elseif rad2 == 3 then
                            Lg_ID = 2538
                        end
                    elseif job == 16 then
                        if cha_type ~= 4 then
                            if rad2 == 1 then
                                Lg_ID = 2539
                            elseif rad2 == 2 then
                                Lg_ID = 2540
                            elseif rad2 == 3 then
                                Lg_ID = 2541
                            end
                        else
                            if rad3 == 1 then
                                Lg_ID = 2539
                            elseif rad3 == 2 then
                                Lg_ID = 2540
                            elseif rad3 == 3 then
                                Lg_ID = 2541
                            elseif rad3 == 4 then
                                Lg_ID = 2548
                            end
                        end
                    elseif job == 13 then
                        if cha_type ~= 4 then
                            if rad2 == 1 then
                                Lg_ID = 2542
                            elseif rad2 == 2 then
                                Lg_ID = 2543
                            elseif rad2 == 3 then
                                Lg_ID = 2544
                            end
                        else
                            if rad3 == 1 then
                                Lg_ID = 2542
                            elseif rad3 == 2 then
                                Lg_ID = 2543
                            elseif rad3 == 3 then
                                Lg_ID = 2544
                            elseif rad3 == 4 then
                                Lg_ID = 2548
                            end
                        end
                    elseif job == 14 then
                        if cha_type ~= 4 then
                            if rad2 == 1 then
                                Lg_ID = 2545
                            elseif rad2 == 2 then
                                Lg_ID = 2546
                            elseif rad2 == 3 then
                                Lg_ID = 2547
                            end
                        else
                            if rad3 == 1 then
                                Lg_ID = 2545
                            elseif rad3 == 2 then
                                Lg_ID = 2546
                            elseif rad3 == 3 then
                                Lg_ID = 2547
                            elseif rad3 == 4 then
                                Lg_ID = 2548
                            end
                        end
                    end
                    GiveItem(Player, 0, Lg_ID, 1, 4)
                    LG("star_rongyuzhichange_lg", cha_namea .. "Lv>61 obtain equipment ID=" .. Lg_ID)
                elseif rad > rongyu_num and rad <= middle then
                    AddMoney(Player, 0, money_num)
                elseif rad > middle and rad <= 30000 then
                    SetChaAttrI(Player, ATTR_CEXP, exp_num)
                    local a1 = math.floor(rongyu_num * 5)
                    SystemNotice(Player, "Obtained EXP " .. a1)
                end
            end
        end
        SetItemAttr(Item, ITEMATTR_VAL_STR, 0)
    end
end

function Elf_Attr_cs(Player, Item_JLone, Item_JLother)
    local Item_JLone_num = {}
    local Item_JLother_num = {}

    Item_JLone_num[1] = GetItemAttr(Item_JLone, ITEMATTR_VAL_STR)
    Item_JLone_num[2] = GetItemAttr(Item_JLone, ITEMATTR_VAL_AGI)
    Item_JLone_num[3] = GetItemAttr(Item_JLone, ITEMATTR_VAL_DEX)
    Item_JLone_num[4] = GetItemAttr(Item_JLone, ITEMATTR_VAL_CON)
    Item_JLone_num[5] = GetItemAttr(Item_JLone, ITEMATTR_VAL_STA)
    Item_JLone_num[6] = GetItemAttr(Item_JLone, ITEMATTR_URE)
    Item_JLone_num[7] = GetItemAttr(Item_JLone, ITEMATTR_MAXURE)
    Item_JLone_num[8] =
        Item_JLone_num[1] + Item_JLone_num[2] + Item_JLone_num[3] + Item_JLone_num[4] + Item_JLone_num[5]

    Item_JLother_num[1] = GetItemAttr(Item_JLother, ITEMATTR_VAL_STR)
    Item_JLother_num[2] = GetItemAttr(Item_JLother, ITEMATTR_VAL_AGI)
    Item_JLother_num[3] = GetItemAttr(Item_JLother, ITEMATTR_VAL_DEX)
    Item_JLother_num[4] = GetItemAttr(Item_JLother, ITEMATTR_VAL_CON)
    Item_JLother_num[5] = GetItemAttr(Item_JLother, ITEMATTR_VAL_STA)
    Item_JLother_num[6] = GetItemAttr(Item_JLother, ITEMATTR_URE)
    Item_JLother_num[7] = GetItemAttr(Item_JLother, ITEMATTR_MAXURE)
    Item_JLother_num[8] =
        Item_JLother_num[1] + Item_JLother_num[2] + Item_JLother_num[3] + Item_JLother_num[4] + Item_JLother_num[5]

    local m = 0
    local n = 0
    local num_jlone = 26
    local num_jlother = 26
    local max_JLone_temp = Item_JLone_num[1]
    local max_JLother_temp = Item_JLother_num[1]
    for m = 1, 4, 1 do
        if Item_JLone_num[m + 1] > max_JLone_temp then
            max_JLone_temp = Item_JLone_num[m + 1]
            num_jlone = m + 26
        end
    end
    for n = 1, 4, 1 do
        if Item_JLother_num[n + 1] > max_JLother_temp then
            max_JLother_temp = Item_JLother_num[n + 1]
            num_jlother = n + 26
        end
    end

    max_JLone_temp = max_JLone_temp - 4
    max_JLother_temp = max_JLother_temp - 4
    local new_JLone_MAXENERGY = 240 * (Item_JLone_num[8] + 1 - 4)
    if new_JLone_MAXENERGY > 6480 then
        new_JLone_MAXENERGY = 6480
    end
    local new_JLone_MAXURE = 5000 + 1000 * (Item_JLone_num[8] - 4)
    if new_JLone_MAXURE > 32000 then
        new_JLone_MAXURE = 32000
    end
    local new_JLother_MAXENERGY = 240 * (Item_JLother_num[8] + 1 - 4)
    if new_JLother_MAXENERGY > 6480 then
        new_JLother_MAXENERGY = 6480
    end
    local new_JLother_MAXURE = 5000 + 1000 * (Item_JLother_num[8] - 4)
    if new_JLother_MAXURE > 32000 then
        new_JLother_MAXURE = 32000
    end

    SetItemAttr(Item_JLone, num_jlone, max_JLone_temp)
    SetItemAttr(Item_JLone, ITEMATTR_ENERGY, 240)
    SetItemAttr(Item_JLone, ITEMATTR_MAXENERGY, new_JLone_MAXENERGY)
    SetItemAttr(Item_JLone, ITEMATTR_URE, 5000)
    SetItemAttr(Item_JLone, ITEMATTR_MAXURE, new_JLone_MAXURE)

    SetItemAttr(Item_JLother, num_jlother, max_JLother_temp)
    SetItemAttr(Item_JLother, ITEMATTR_ENERGY, 240)
    SetItemAttr(Item_JLother, ITEMATTR_MAXENERGY, new_JLother_MAXENERGY)
    SetItemAttr(Item_JLother, ITEMATTR_URE, 5000)
    SetItemAttr(Item_JLother, ITEMATTR_MAXURE, new_JLother_MAXURE)
end

function TigerStart(...)
    local arg = {...}
    if #arg ~= 4 then
        SystemNotice(arg[1], "Illegal value parameter: " .. #arg)
        return
    end
    if KitbagLock(arg[1], 0) == LUA_FALSE or GetChaFreeBagGridNum(arg[1]) < 5 or GetChaAttr(arg[1], ATTR_GD) > JackpotM.UserGold then
        SystemNotice(arg[1], "Your inventory needs to be unlocked, have at least 5 free slots and less than " ..JackpotM.UserGold .. "G in your inventory.")
        return 0
    end
    local Slot, A, B, C = {}, 0, 0, 0
    for A = 2, 4, 1 do
        if arg[A] == 1 and TakeItem(arg[1], 0, JackpotM.Bet, 5) == 0 then
            SystemNotice(arg[1], "Failed to delete " .. GetItemName(JackpotM.Bet) .. ".")
            return
        end
    end
    for A = 1, 9, 1 do
        D = 0
        while D == 0 do
            D = JackpotM.Items[math.random(1, #JackpotM.Items)]
            if IsItemValid(D) == LUA_FALSE then
                D = 0
            end
        end
        Slot[A] = D
    end
    if math.random(1, 100) <= 40 then
        if Slot[1] == Slot[4] and Slot[1] ~= Slot[7] then
            Slot[7] = Slot[1]
        end
        if Slot[2] == Slot[5] and Slot[2] ~= Slot[8] then
            Slot[8] = Slot[2]
        end
        if Slot[3] == Slot[6] and Slot[3] ~= Slot[9] then
            Slot[9] = Slot[3]
        end
        if Slot[1] == Slot[2] and Slot[1] ~= Slot[3] then
            Slot[3] = Slot[1]
        end
        if Slot[4] == Slot[5] and Slot[4] ~= Slot[6] then
            Slot[6] = Slot[4]
        end
        if Slot[7] == Slot[8] and Slot[7] ~= Slot[9] then
            Slot[9] = Slot[7]
        end
        if Slot[1] == Slot[5] and Slot[1] ~= Slot[9] then
            Slot[9] = Slot[1]
        end
        if Slot[3] == Slot[5] and Slot[3] ~= Slot[7] then
            Slot[7] = Slot[3]
        end
    end
    return Slot[1], Slot[2], Slot[3], Slot[4], Slot[5], Slot[6], Slot[7], Slot[8], Slot[9]
end
function TigerStop(...)
    local arg = {...}
    if #arg ~= 13 then
        SystemNotice(arg[1], "Illegal Parameter Value: " .. #arg)
        return
    end
    for Yb = 1, 10, 1 do
        if arg[Yb] == 0 or arg[Yb] == nil then
            Yc = 1
            SystemNotice(arg[1], "Illegal Lucky Chance Parameter.")
            return
        end
    end
    local Money, Message = 0, nil
    local Items = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    if arg[11] == 1 and arg[2] == arg[5] and arg[5] == arg[8] then
        Money = Money + 2930
        Message = "Safe Voyage"
        Items[1] = arg[2]
    end
    if arg[12] == 1 and arg[3] == arg[6] and arg[6] == arg[9] then
        Money = Money + 2930
        Message = "Safe Voyage"
        Items[2] = arg[3]
    end
    if arg[13] == 1 and arg[4] == arg[7] and arg[7] == arg[10] then
        Money = Money + 2930
        Message = "Safe Voyage"
        Items[3] = arg[4]
    end
    if arg[11] == 1 and arg[12] == 1 and arg[13] == 1 then
        if arg[5] == arg[6] and arg[6] == arg[7] then
            Money = Money + 5860
            Message = "The One"
            Items[5] = arg[5]
        end
        if arg[2] == arg[6] and arg[6] == arg[10] then
            Money = Money + 11719
            Message = "White Diagonal"
            Items[7] = arg[2]
        end
        if arg[4] == arg[6] and arg[6] == arg[8] then
            Money = Money + 23438
            Message = "Golden Diagonal"
            Items[8] = arg[4]
        end
        if arg[3] == arg[5] and arg[5] == arg[7] and arg[7] == arg[9] then
            Money = Money + 46875
            Message = "Zesty"
            Items[9] = arg[3]
        end
        if arg[2] == arg[4] and arg[4] == arg[8] and arg[8] == arg[10] then
            Money = Money + 93750
            Message = "4 Seasons"
            Items[10] = arg[2]
        end
        if arg[2] == arg[4] and arg[4] == arg[6] and arg[6] == arg[8] and arg[8] == arg[10] then
            Money = Money + 187500
            Message = "Five Fortune"
            Items[11] = arg[2]
        end
        if arg[3] == arg[5] and arg[5] == arg[6] and arg[6] == arg[7] and arg[7] == arg[9] then
            Money = Money + 375000
            Message = "Perfect 10"
            Items[12] = arg[3]
        end
        if
            arg[2] == arg[3] and arg[3] == arg[4] and arg[4] == arg[5] and arg[5] == arg[7] and arg[7] == arg[8] and
                arg[8] == arg[9] and
                arg[9] == arg[10]
         then
            Money = Money + 750000
            Message = "All Round Celebration"
            Items[13] = arg[2]
        end
        if
            arg[2] == arg[3] and arg[3] == arg[4] and arg[4] == arg[5] and arg[5] == arg[6] and arg[6] == arg[7] and
                arg[7] == arg[8] and
                arg[8] == arg[9] and
                arg[9] == arg[10]
         then
            Money = Money + 1500000
            Message = "Immense Wealth"
            Items[14] = arg[2]
        end
    end
    if Message ~= nil then
        Notice("Congratulations [" ..GetChaDefaultName(arg[1]) .. "]! Struck [" .. Message .. "] and won " .. Money .. "G.")
        SynTigerString(arg[1], "Congratulations! Struck [" .. Message .. "] and won " .. Money .. "G.")
    end
    if Money > 0 then
        AddMoney(arg[1], 0, Money)
    end
    for A = 1, #Items, 1 do
        if Items[A] ~= 0 and Items[A] ~= 3360 then
            GiveItem(arg[1], 0, Items[A], 1, 4)
        end
    end
    LG("Jackpot Machine", "|", "Player Name:", GetChaDefaultName(arg[1]))
    LG("Jackpot Machine", "|", "Player ID:", "", GetRoleID(arg[1]))
    LG("Jackpot Machine", "|", "Money Gained:", Money)
    LG(
        "Jackpot Machine",
        "|",
        "[" .. GetItemName(arg[2]) .. "]",
        "",
        "[" .. GetItemName(arg[5]) .. "]",
        "",
        "[" .. GetItemName(arg[8]) .. "]"
    )
    LG(
        "Jackpot Machine",
        "|",
        "[" .. GetItemName(arg[3]) .. "]",
        "",
        "[" .. GetItemName(arg[6]) .. "]",
        "",
        "[" .. GetItemName(arg[9]) .. "]"
    )
    LG(
        "Jackpot Machine",
        "|",
        "[" .. GetItemName(arg[4]) .. "]",
        "",
        "[" .. GetItemName(arg[7]) .. "]",
        "",
        "[" .. GetItemName(arg[10]) .. "]"
    )
    LG("Jackpot Machine", "--------------------------------------------------")
end

function Change_shanyao(character, npc)
    local NocLock = KitbagLock(character, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(character, "Your inventory is being binded")
        return 0
    end
	
	local checkMedal = 0
	local checkMedal = CheckBagItem(character, 3849)
	if checkMedal < 1 then
		SystemNotice(character , "Sorry but you do not have Medal of Valor")
		return 0
	end
	
	local getMedal = GetChaItem2(character, 2, 3849)
    local HonorPoint = GetChaAttr(getMedal, ITEMATTR_VAL_STR)
    if HonorPoint < 200 then
        SystemNotice(character, "You do not have sufficient Honor points")
        return 0
    end
	
    HonorPoint = HonorPoint - 200
    GiveHonor(character, HonorPoint)

    if GetChaFreeBagGridNum(character) <= 0 then
        SystemNotice(character, "You do not have enough slots")
        UseItemFailed(character)
        return
    end
    GiveItem(character, 0, 2614, 1, 4)
end

function Change_rongyao(character, npc)
    local NocLock = KitbagLock(character, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(character, "Your inventory is being binded")
        return 0
    end

	if CheckBagItem(character, 3849) < 1 then
		SystemNotice(character, "You do not have Mark of Honor")
		return 0
	end	
	
	local GetBookPoint = GetChaItem2(character, 2, 3849)
	local HonorPoint = GetItemAttr(GetBookPoint, ITEMATTR_VAL_STR)
    if HonorPoint < 2000 then
        SystemNotice(character, "You do not have sufficient Honor points")
        return 0
    end

    HonorPoint = HonorPoint - 2000
    GiveHonor(character, HonorPoint)

    if GetChaFreeBagGridNum(character) <= 0 then
        SystemNotice(character, "You do not have enough slots")
        UseItemFailed(character)
        return
    end
    GiveItem(character, 0, 2615, 1, 4)
end

function Change_huihuang(character, npc)
    local NocLock = KitbagLock(character, 0)
    if NocLock == LUA_FALSE then
        SystemNotice(character, "Your inventory is being binded")
        return 0
    end
	
	local checkMedal = 0
	local checkMedal = CheckBagItem(character, 3849)
	if checkMedal < 1 then
		SystemNotice(character , "Sorry but you do not have Medal of Valor")
		return 0
	end
	
	local getMedal = GetChaItem2(character, 2, 3849)
    local HonorPoint = GetChaAttr(getMedal, ITEMATTR_VAL_STR)
    if HonorPoint < 200 then
        SystemNotice(character, "You do not have sufficient Honor points")
        return 0
    end
	
    HonorPoint = HonorPoint - 200
    GiveHonor(character, HonorPoint)

    if GetChaFreeBagGridNum(character) <= 0 then
        SystemNotice(character, "You do not have enough slots")
        UseItemFailed(character)
        return
    end
    GiveItem(character, 0, 2616, 1, 4)
end

function Eleven_Log_0(Player)
    local cha_name = GetChaDefaultName(Player)
    local job = GetChaAttr(Player, ATTR_JOB)
    local lv = GetChaAttr(Player, ATTR_LV)
    LG("Eleven_Log_0", cha_name, lv, job)
end
function Eleven_Log(Player, typ)
    local cha_name = GetChaDefaultName(Player)
    local job = GetChaAttr(Player, ATTR_JOB)
    local lv = GetChaAttr(Player, ATTR_LV)
    LG("Eleven_Log", cha_name, lv, job, typ)
end

function WGPrizeBegin(Player, rightCount)
    local rightCountTemp = rightCount
    if rightCountTemp > 6 then
        rightCountTemp = 6
    end

    local isPrizeRandom = math.random(rightCountTemp, 10)
    local retRandom = math.random(1, 100)
    local ret = 1

    if isPrizeRandom > 5 then
        if math.fmod(retRandom, 6) == 0 then
            ret = WGprize_1(Player)
        end

        if math.fmod(retRandom, 6) == 1 then
            ret = WGprize_2(Player)
        end

        if math.fmod(retRandom, 6) == 2 then
            ret = WGprize_3(Player)
        end

        if math.fmod(retRandom, 6) == 3 then
            ret = WGprize_4(Player)
        end

        if math.fmod(retRandom, 6) == 4 then
            ret = WGprize_5(Player)
        end

        if math.fmod(retRandom, 6) == 5 then
            ret = WGprize_6(Player)
        end

        if ret == 1 then
            SystemNotice(Player, "Return value error")
        end
    else
        SystemNotice(Player, "Correct! You have failed to win any prize")
    end
end
function WGprize_1(Player)
    local expNow = GetChaAttr(Player, ATTR_CEXP)
    local lvNow = GetChaAttr(Player, ATTR_LV)

    SystemNotice(Player, "Correct! You have obtained " .. lvNow * 10 .. " experience points")
    SetChaAttrI(Player, ATTR_CEXP, expNow + lvNow * 10)
    RefreshCha(Player)
    return 0
end

function WGprize_2(Player)
    local bloodMaxNow = GetChaAttr(Player, ATTR_MXHP)
    SystemNotice(Player, "Correct! Your HP is restored")
    SetChaAttrI(Player, ATTR_HP, bloodMaxNow)
    RefreshCha(Player)
    return 0
end

function WGprize_3(Player)
    local SPMaxNow = GetChaAttr(Player, ATTR_MXSP)
    SystemNotice(Player, " Correct answer will restore your SP to the max")
    SetChaAttrI(Player, ATTR_SP, SPMaxNow)
    RefreshCha(Player)
    return 0
end

function WGprize_4(Player)
    local lvNow = GetChaAttr(Player, ATTR_LV)
    SystemNotice(Player, "Answered correctly and obtained " .. lvNow .. " cake(s)")

    GiveItem(Player, 0, 1849, lvNow, 4)
    return 0
end

function WGprize_5(Player)
    SystemNotice(Player, "Correct answer will give you 1 Rusty Ticket")

    GiveItem(Player, 0, 19500, 1, 4)
    return 0
end

function WGprize_6(Player)
    local bloodMaxNow = GetChaAttr(Player, ATTR_MXHP)
    local SPMaxNow = GetChaAttr(Player, ATTR_MXSP)

    SystemNotice(Player, "Correct answer will restore HP & SP to the max")

    SetChaAttrI(Player, ATTR_HP, bloodMaxNow)
    SetChaAttrI(Player, ATTR_SP, SPMaxNow)
    RefreshCha(Player)
    return 0
end

function CheckTeam1(role, value)
    local player = {}
    player[1] = role
    player[2] = GetTeamCha(role, 0)
    player[3] = GetTeamCha(role, 1)
    player[4] = GetTeamCha(role, 2)
    player[5] = GetTeamCha(role, 3)
    local n = 0
    for j = 0, 5, 1 do
        if ValidCha(player[j]) == 1 then
            n = n + 1
        end
    end
    if n >= value then
        return LUA_TRUE
    end
end

function CheckTeam(Player)
    local player = {}
    player[1] = Player
    player[2] = GetTeamCha(Player, 0)
    player[3] = GetTeamCha(Player, 1)
    player[4] = GetTeamCha(Player, 2)
    player[5] = GetTeamCha(Player, 3)
    local n1 = 0
    local n2 = 0
    local n3 = 0

    for j = 0, 5, 1 do
        if ValidCha(player[j]) == 1 then
            local lv_p = GetChaAttr(player[j], ATTR_LV)

            if lv_p >= 20 and lv_p <= 30 then
                n1 = n1 + 1
            elseif lv_p > 30 and lv_p <= 40 then
                n2 = n2 + 1
            elseif lv_p > 40 then
                n3 = n3 + 1
            end
        end
    end

    if n1 >= 1 and n2 >= 1 and n3 >= 1 then
        return LUA_TRUE
    end
end

function CheckTime(Player)
    local now_week = os.date("%w")
    local now_hour = os.date("%H")
    now_week = tonumber(now_week)
    now_hour = tonumber(now_hour)

    if now_week == 6 then
        if now_hour >= 9 and now_hour < 12 then
            return LUA_TRUE
        elseif now_hour >= 21 then
            return LUA_TRUE
        end
    end
end

function WGPrizeBegin(Player, rightCount)
    local rightCountTemp = rightCount
    if rightCountTemp > 6 then
        rightCountTemp = 6
    end

    local isPrizeRandom = math.random(rightCountTemp, 10)
    local retRandom = math.random(1, 100)
    local ret = 1

    if isPrizeRandom > 5 then
        if math.fmod(retRandom, 6) == 0 then
            ret = WGprize_1(Player)
        end

        if math.fmod(retRandom, 6) == 1 then
            ret = WGprize_2(Player)
        end

        if math.fmod(retRandom, 6) == 2 then
            ret = WGprize_3(Player)
        end

        if math.fmod(retRandom, 6) == 3 then
            ret = WGprize_4(Player)
        end

        if math.fmod(retRandom, 6) == 4 then
            ret = WGprize_5(Player)
        end

        if math.fmod(retRandom, 6) == 5 then
            ret = WGprize_6(Player)
        end

        if ret == 1 then
            SystemNotice(Player, "error")
        end
    else
        SystemNotice(Player, "Correct! You have failed to win any prize")
    end
end
function WGprize_1(Player)
    local expNow = GetChaAttr(Player, ATTR_CEXP)
    local lvNow = GetChaAttr(Player, ATTR_LV)

    SystemNotice(Player, "Correct! You have obtained " .. lvNow * 10 .. " experience points")
    SetChaAttrI(Player, ATTR_CEXP, expNow + lvNow * 10)
    RefreshCha(Player)
    return 0
end

function WGprize_2(Player)
    local bloodMaxNow = GetChaAttr(Player, ATTR_MXHP)
    SystemNotice(Player, "Correct! Your HP is restored")
    SetChaAttrI(Player, ATTR_HP, bloodMaxNow)
    RefreshCha(Player)
    return 0
end

function WGprize_3(Player)
    local SPMaxNow = GetChaAttr(Player, ATTR_MXSP)
    SystemNotice(Player, "Correct! Your SP is restored")
    SetChaAttrI(Player, ATTR_SP, SPMaxNow)
    RefreshCha(Player)
    return 0
end

function WGprize_4(Player)
    local lvNow = GetChaAttr(Player, ATTR_LV)
    SystemNotice(Player, "Correct! You have obtained " .. lvNow .. " cakes")

    GiveItem(Player, 0, 1849, lvNow, 4)
    return 0
end

function WGprize_5(Player)
    SystemNotice(Player, "Correct! You have obtained 1 Old Ticket")

    GiveItem(Player, 0, 19500, 1, 4)
    return 0
end

function WGprize_6(Player)
    local bloodMaxNow = GetChaAttr(Player, ATTR_MXHP)
    local SPMaxNow = GetChaAttr(Player, ATTR_MXSP)

    SystemNotice(Player, "Correct! Your HP and SP is restored")

    SetChaAttrI(Player, ATTR_HP, bloodMaxNow)
    SetChaAttrI(Player, ATTR_SP, SPMaxNow)
    RefreshCha(Player)
    return 0
end

function Can_Exchange(sSrcItem,sSrcNum,sTagItem,sTagNum)
	local Data
	for Data in pairs(ChangeItemList) do
		 if ChangeItemList[Data][1] == sSrcItem and ChangeItemList[Data][2] == sSrcNum and ChangeItemList[Data][3] == sTagItem and ChangeItemList[Data][4] == sTagNum then
			return LUA_TRUE
		 end
	end
	return LUA_FALSE
end

function CheckRightNum(role)
    local i = CheckBagItem(role, 579)
    if i == 1 then
        local Item = GetChaItem2(role, 2, 579)
        local n = GetItemAttr(Item, ITEMATTR_MAXENERGY)
        if n == 8 then
            return LUA_TRUE
        end
    end
end

function CheckErroNum(role)
    local i = CheckBagItem(role, 579)
    if i == 1 then
        local Item = GetChaItem2(role, 2, 579)
        local n = GetItemAttr(Item, ITEMATTR_MAXENERGY)
        if n < 8 then
            return LUA_TRUE
        end
    end
end

function CheckRealNpc(role, value)
    local now_hour = os.date("%H")
    now_hour = tonumber(now_hour)
    local n = (now_hour / 4 - math.floor(now_hour / 4)) * 4
    if n == value then
        return LUA_TRUE
    else
        SystemNotice(role, ",")
    end
end

function Givecrab(character)
    local c1 = 0
    local c2 = 0
    c1, c2 = MakeItem(character, 58, 1, 4)
    local Item_CRAB = GetChaItem(character, 2, c2)

    local CRAB_NOW = 7200

    SetItemAttr(Item_CRAB, ITEMATTR_MAXENERGY, CRAB_NOW)
    SetItemAttr(Item_CRAB, ITEMATTR_ENERGY, CRAB_NOW)
    RefreshCha(character)
    SystemNotice(character, "2?")
end

function crablife(character)
    local Crab_Num = 0
    Crab_Num = CheckBagItem(character, 58)

    if Crab_Num == 1 then
        local crab = GetChaItem2(character, 2, 58)
        local ENERGY = GetItemAttr(crab, ITEMATTR_ENERGY)
        if ENERGY == 0 then
            return LUA_TRUE
        end
    else
        SystemNotice(character, "")
    end
end

function GiveZNZItem(role)
    SystemNotice(role, "")
    local cha_name = GetChaDefaultName(role)
    local star = math.random(1, 10000)

    if star <= 8000 then
        GiveItem(role, 0, 2999, 1, 4)
    elseif star >= 8001 and star <= 9500 then
        local el = math.random(1, 15)
        if el <= 5 then
            GiveItem(role, 0, 0227, 1, 4)
        elseif el >= 6 and el <= 7 then
            GiveItem(role, 0, 3111, 1, 4)
        elseif el >= 8 and el <= 9 then
            GiveItem(role, 0, 3109, 1, 4)
        elseif el >= 10 and el <= 11 then
            GiveItem(role, 0, 3110, 1, 4)
        elseif el >= 12 and el <= 13 then
            GiveItem(role, 0, 3112, 1, 4)
        elseif el >= 14 and el <= 15 then
            GiveItem(role, 0, 3113, 1, 4)
        end
    elseif star >= 9501 and star <= 9800 then
        local el1 = math.random(1, 5)
        if el1 == 1 then
            GiveItem(role, 0, 0863, 1, 4)
            local message = cha_name .. ",?"
            Notice(message)
        elseif el1 == 2 then
            GiveItem(role, 0, 0860, 1, 4)
            local message1 = cha_name .. ",?"
            Notice(message1)
        elseif el1 == 3 then
            GiveItem(role, 0, 0861, 1, 4)
            local message2 = cha_name .. ",?"
            Notice(message2)
        elseif el1 == 4 then
            GiveItem(role, 0, 0862, 1, 4)
            local message3 = cha_name .. ",?"
            Notice(message3)
        elseif el1 == 5 then
            GiveItem(role, 0, 1028, 1, 4)
            local message4 = cha_name .. ",?"
            Notice(message4)
        end
    elseif star >= 9801 and star <= 9998 then
        local el2 = math.random(1, 100)
        if el2 <= 50 then
            GiveItem(role, 0, 0992, 1, 4)
            local message5 = cha_name .. ",??"
            Notice(message5)
        elseif el2 >= 51 and el2 <= 74 then
            GiveItem(role, 0, 0853, 1, 4)
            local message6 = cha_name .. ",?"
            Notice(message6)
        elseif el2 >= 75 and el2 <= 100 then
            GiveItem(role, 0, 1012, 1, 4)
            local message7 = cha_name .. ",?"
            Notice(message7)
        end
    elseif star >= 9899 and star <= 10000 then
        local el3 = math.random(1, 100)
        if el3 == 63 then
            GiveItem(role, 0, 0096, 1, 4)
            local message8 = cha_name .. ",?"
            Notice(message8)
        elseif el3 == 98 then
            GiveItem(role, 0, 0094, 1, 4)
            local message9 = cha_name .. ",?"
            Notice(message9)
        end
    end
end

function Check_Skill_Rad(Skill_Level)
    local Skill_Rad
    if Skill_Level == 2 then
        Skill_Rad = 0.05
    elseif Skill_Level == 3 then
        Skill_Rad = 0.10
    elseif Skill_Level == 4 then
        Skill_Rad = 0.15
    elseif Skill_Level == 5 then
        Skill_Rad = 0.2
    else
        Skill_Rad = 0
    end

    return Skill_Rad
end

function Check_Equip_Rad(Buff_Equip_ID)
    local Equip_Rad
    if Buff_Equip_ID == 3285 then
        Equip_Rad = 0.05
    elseif Buff_Equip_ID == 3287 then
        Equip_Rad = 0.1
    else
        Equip_Rad = 0
    end

    return Equip_Rad
end

function Check_Item_Rad(role)
    local stateLV_Apple
    local Item_Rad

    stateLV_Apple = GetChaStateLv(role, STATE_APPLE)

    if stateLV_Apple == 1 then
        Item_Rad = 0.3
    elseif stateLV_Apple == 2 then
        Item_Rad = 0.5
    else
        Item_Rad = 0
    end

    return Item_Rad
end

function Check_Exp_Increase(Book_ID)
    local Book_ID_mod
    local Book_Exp_Increase

    if Book_ID >= 3243 and Book_ID <= 3246 then
        Book_Exp_Increase = 1
    elseif Book_ID >= 3247 and Book_ID <= 3250 then
        Book_Exp_Increase = 3
    elseif Book_ID >= 3251 and Book_ID <= 3254 then
        Book_Exp_Increase = 5
    elseif Book_ID >= 3255 and Book_ID <= 3260 then
        Book_Exp_Increase = 9
    elseif Book_ID >= 3261 and Book_ID <= 3266 then
        Book_Exp_Increase = 13
    elseif Book_ID >= 3267 and Book_ID <= 3272 then
        Book_Exp_Increase = 18
    elseif Book_ID >= 3273 and Book_ID <= 3278 then
        Book_Exp_Increase = 24
    else
        Book_Exp_Increase = 0
    end

    return Book_Exp_Increase
end

function Add_BookEXP(role, Certificate, Book_ID, Skill_Level)
    local Role_Level
    local Buff_Equip
    local Buff_Equip_ID
    local Exp_Increase
    local Skill_Rad
    local Equip_Rad
    local Item_Rad
    local DoubleEffect

    Role_Level = Lv(role)
    Buff_Equip = GetChaItem(role, 1, 6)
    Buff_Equip_ID = GetItemID(Buff_Equip)

    Skill_Rad = Check_Skill_Rad(Skill_Level)
    Equip_Rad = Check_Equip_Rad(Buff_Equip_ID)
    Item_Rad = Check_Item_Rad(role)
    Exp_Increase = Check_Exp_Increase(Book_ID)

    Exp_Increase = Exp_Increase * (1 + Skill_Rad + Equip_Rad + Item_Rad)

    local Book_Exp_Now = GetItemAttr(Certificate, ITEMATTR_ENERGY)
    local Book_Exp_Max = GetItemAttr(Certificate, ITEMATTR_MAXENERGY)

    Book_Exp_Now = Book_Exp_Now + Exp_Increase

    if Book_Exp_Now >= Book_Exp_Max then
        Book_Exp_Now = Book_Exp_Max
    end

    SetItemAttr(Certificate, ITEMATTR_ENERGY, Book_Exp_Now)
end

function Take_BookDurability(role, Book, Certificate)
    local Durability_Reduce = 250
    local Book_Dur_Now = GetItemAttr(Book, ITEMATTR_URE)
    local Book_Dur_Max = GetItemAttr(Book, ITEMATTR_MAXURE)

    local Certificate_Exp_Now = GetItemAttr(Certificate, ITEMATTR_ENERGY)
    local Certificate_Exp_Max = GetItemAttr(Certificate, ITEMATTR_MAXENERGY)

    if Certificate_Exp_Now == Certificate_Exp_Max then
        SystemNotice(role, "Your Student Card's EXP is full. Please take the Graduation Quest.")
        return 0
    end
    SystemNotice(role, "Through the efforts of Study, your Student Card has increased EXP.")
    Book_Dur_Now = Book_Dur_Now - Durability_Reduce

    if Book_Dur_Now <= 0 then
        Book_Dur_Now = 0
    end

    SetItemAttr(Book, ITEMATTR_URE, Book_Dur_Now)
end

function Reading_Book(role, Skill_Level)
    local Book
    local Book_ID

    local Certificate
    local Certificate_ID

    Book = GetChaItem(role, 1, 9)
    Book_ID = GetItemID(Book)

    Certificate = GetChaItem(role, 1, 5)
    Certificate_ID = GetItemID(Certificate)
    local Book_Dur = GetItemAttr(Book, ITEMATTR_URE)
    if Book_Dur > 0 then
        if Certificate_ID == 3289 then
            if Book_ID >= 3243 and Book_ID <= 3278 then
                Take_BookDurability(role, Book, Certificate)
                Add_BookEXP(role, Certificate, Book_ID, Skill_Level)
                Refreshcha(role)
            else
                SystemNotice(role, "You don't have a book")
            end
        else
            SystemNotice(role, "You don't have a Student Card")
        end
    else
        SystemNotice(role, "Book's durability reached 0, please get a new Book!")
    end
end

Reading_Credit = {}
Reading_Credit[0] = 150
Reading_Credit[1] = 250
Reading_Credit[2] = 400
Reading_Credit[3] = 800
Reading_Credit[4] = 4500

Reading_EXP = {}
Reading_EXP[0] = 120
Reading_EXP[1] = 700
Reading_EXP[2] = 1700
Reading_EXP[3] = 3000
Reading_EXP[4] = 5000

function CheckXSZExp(character)
    local xsz_num = 0
    xsz_num = CheckBagItem(character, 3289)
    if xsz_num ~= 1 then
        SystemNotice(character, "Put the Student Card in your inventory!")
        return 0
    end
    local role_xsz = GetChaItem2(character, 2, 3289)
    local exp_xsz = GetItemAttr(role_xsz, ITEMATTR_ENERGY)
    local mexp_xsz = GetItemAttr(role_xsz, ITEMATTR_MAXENERGY)

    if exp_xsz == mexp_xsz then
        return LUA_TRUE
    end

    return LUA_FALSE
end

function CheckXSZCh(character)
    local xsz_num = 0
    xsz_num = CheckBagItem(character, 3289)
    if xsz_num ~= 1 then
        SystemNotice(character, "Put the Student Card in your inventory!")
        return 0
    end
    local role_xsz = GetChaItem2(character, 2, 3289)
    local ch_xsz = GetItemAttr(role_xsz, ITEMATTR_URE)
    local mch_xsz = GetItemAttr(role_xsz, ITEMATTR_MAXURE)
    if ch_xsz == mch_xsz then
        return LUA_TRUE
    end

    return LUA_FALSE
end

function ReadBookTime()
    return 600 * 1000
end

function ReadBookSkillId()
    return 461
end

function AuctionEnd(role)
    local sc = CheckBagItem(role, 3025)
    if sc <= 0 then
        SystemNotice(role, "")
        return 0
    end

    local item_number = CheckBagItem(role, 3066)
    if item_number >= 1 then
        SystemNotice(role, "")
        return 0
    end
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "2?")
        return 0
    end
    GiveItem(role, 0, 3666, 10, 4)
    DelBagItem(role, 3025, 1)
    local r1 = 0
    local r2 = 0
    r1, r2 = MakeItem(role, 3066, 1, 4)
    local Item_new = GetChaItem(role, 2, r2)

    local now_month = os.date("%m")
    local now_day = os.date("%d")
    local now_hour = os.date("%H")
    local now_miniute = os.date("%M")
    local now_day1 = 0
    local now_month1 = 0
    local now_hour1 = 0
    local now_miniute1 = 0

    now_month = tonumber(now_month)
    now_day = tonumber(now_day)
    now_hour = tonumber(now_hour)
    now_miniute = tonumber(now_miniute)
    local CheckDateNum = now_hour * 100 + now_miniute

    if CheckDateNum == 1830 then
        now_hour1 = 18
        now_miniute1 = 0
        if
            now_month == 1 or now_month == 3 or now_month == 5 or now_month == 7 or now_month == 8 or now_month == 10 or
                now_month == 12
         then
            if now_day <= 26 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 26 then
                now_day1 = (now_day + 5) - 31
                now_month1 = now_month + 1
            end
        end

        if now_month == 4 or now_month == 6 or now_month == 9 or now_month == 11 then
            if now_day <= 25 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 25 then
                now_day1 = (now_day + 5) - 30
                now_month1 = now_month + 1
            end
        end

        if now_month == 2 then
            if now_day <= 23 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 23 then
                now_day1 = (now_day + 5) - 28
                now_month1 = now_month + 1
            end
        end
    elseif CheckDateNum == 1910 then
        now_hour1 = 20
        now_miniute1 = 10
        if
            now_month == 1 or now_month == 3 or now_month == 5 or now_month == 7 or now_month == 8 or now_month == 10 or
                now_month == 12
         then
            if now_day <= 26 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 26 then
                now_day1 = (now_day + 5) - 31
                now_month1 = now_month + 1
            end
        end

        if now_month == 4 or now_month == 6 or now_month == 9 or now_month == 11 then
            if now_day <= 25 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 25 then
                now_day1 = (now_day + 5) - 30
                now_month1 = now_month + 1
            end
        end

        if now_month == 2 then
            if now_day <= 23 then
                now_day1 = now_day + 5
                now_month1 = now_month
            elseif now_day > 23 then
                now_day1 = (now_day + 5) - 28
                now_month1 = now_month + 1
            end
        end
    elseif CheckDateNum == 1950 then
        now_hour1 = 18
        now_miniute1 = 0
        if
            now_month == 1 or now_month == 3 or now_month == 5 or now_month == 7 or now_month == 8 or now_month == 10 or
                now_month == 12
         then
            if now_day <= 25 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 25 then
                now_day1 = (now_day + 6) - 31
                now_month1 = now_month + 1
            end
        end

        if now_month == 4 or now_month == 6 or now_month == 9 or now_month == 11 then
            if now_day <= 24 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 24 then
                now_day1 = (now_day + 6) - 30
                now_month1 = now_month + 1
            end
        end

        if now_month == 2 then
            if now_day <= 22 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 22 then
                now_day1 = (now_day + 6) - 28
                now_month1 = now_month + 1
            end
        end
    elseif CheckDateNum == 2030 then
        now_hour1 = 20
        now_miniute1 = 10
        if
            now_month == 1 or now_month == 3 or now_month == 5 or now_month == 7 or now_month == 8 or now_month == 10 or
                now_month == 12
         then
            if now_day <= 25 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 25 then
                now_day1 = (now_day + 6) - 31
                now_month1 = now_month + 1
            end
        end

        if now_month == 4 or now_month == 6 or now_month == 9 or now_month == 11 then
            if now_day <= 24 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 24 then
                now_day1 = (now_day + 6) - 30
                now_month1 = now_month + 1
            end
        end

        if now_month == 2 then
            if now_day <= 22 then
                now_day1 = now_day + 6
                now_month1 = now_month
            elseif now_day > 22 then
                now_day1 = (now_day + 6) - 28
                now_month1 = now_month + 1
            end
        end
    end

    SetItemAttr(Item_new, ITEMATTR_VAL_STA, now_month1)
    SetItemAttr(Item_new, ITEMATTR_VAL_STR, now_day1)
    SetItemAttr(Item_new, ITEMATTR_VAL_CON, now_hour1)
    SetItemAttr(Item_new, ITEMATTR_VAL_DEX, now_miniute1)
    SynChaKitbag(role, 13)
end

function YORN(role)
    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 2 then
        SystemNotice(role, "2?")
        return 0
    end

    local item_number1 = CheckBagItem(role, 3066)
    if item_number1 >= 1 then
        SystemNotice(role, "")
        return 0
    end

    local item_number2 = CheckBagItem(role, 3078)
    if item_number2 >= 1 then
        SystemNotice(role, "?")
        return 0
    end

    local item_number3 = CheckBagItem(role, 3025)
    if item_number3 < 1 then
        SystemNotice(role, "")
        return 0
    end
    return 1
end

function HasReadExp(role)
    local xsz_num = 0
    xsz_num = CheckBagItem(role, 3289)
    if xsz_num ~= 1 then
        SystemNotice(role, "Put the Student Card in your inventory!")
        return 0
    end
    local role_xsz = GetChaItem2(role, 2, 3289)
    local exp_xsz = GetItemAttr(role_xsz, ITEMATTR_ENERGY)

    if exp_xsz > 0 then
        return LUA_TRUE
    end

    return LUA_FALSE
end

function AddReadingBook(role)
    local job = GetChaAttr(role, ATTR_JOB)
    local Book_Id = 0
    if job == 1 then
        Book_Id = 3243
    elseif job == 2 then
        Book_Id = 3244
    elseif job == 4 then
        Book_Id = 3246
    elseif job == 5 then
        Book_Id = 3245
    elseif job == 8 then
        Book_Id = 3256
    elseif job == 9 then
        Book_Id = 3255
    elseif job == 12 then
        Book_Id = 3257
    elseif job == 13 then
        Book_Id = 3258
    elseif job == 14 then
        Book_Id = 3259
    elseif job == 16 then
        Book_Id = 3260
    else
        Book_Id = 3288
    end
    GiveItem(role, 0, Book_Id, 1, 4)
    return LUA_TRUE
end

function AddExpPer(role, value)
    local CLv = GetChaAttr(role, ATTR_LV)
    local CExp = GetChaAttrI(role, ATTR_CEXP)
    local GExp = (CalExp[CLv + 1] - CalExp[CLv]) * (value / 100)
    local FExp = math.floor(CExp + GExp)
    SetChaAttrI(role, ATTR_CEXP, FExp)
    return LUA_TRUE
end

function AddExpAll(role, value1, value2, type)
    local exp_now = GetChaAttr(role, ATTR_CEXP)
    if type == 1 then
        local exp_add = math.random(value1, value2)
        local lv = GetChaAttr(role, ATTR_LV)
        if lv < 80 then
            exp_new = exp_now + exp_add
            SetChaAttrI(role, ATTR_CEXP, exp_new)
        else
            exp_add = exp_add / 50
            exp_new = exp_now + exp_add
            SetChaAttrI(role, ATTR_CEXP, exp_new)
        end
    elseif type == 2 then
        local per_exp = math.random(value1, value2)
        local lv = GetChaAttr(role, ATTR_LV)
        if lv < 80 then
            local lv_next = lv + 1
            local exp_up = CalExp[lv_next] - CalExp[lv]
            local exp_add = math.floor((exp_up * per_exp) / 100)
            exp_new = exp_now + exp_add
            SetChaAttrI(role, ATTR_CEXP, exp_new)
        elseif lv >= 80 and lv < 100 then
            local lv_next = lv + 1
            local exp_up = CalExp[lv_next] - CalExp[lv]
            local exp_add = math.floor((exp_up * per_exp) / 5000)
            exp_new = exp_now + exp_add
            SetChaAttrI(role, ATTR_CEXP, exp_new)
        else
            exp_new = exp_now + 10000
            SetChaAttrI(role, ATTR_CEXP, exp_new)
        end
    end
end

function AddChaHJ(character)
    local c1 = 0
    local c2 = 0
    c1, c2 = MakeItem(character, 2967, 1, 4)
    local Item_Rwine = GetChaItem(character, 2, c2)

    local Rwine_NOW = 1440

    SetItemAttr(Item_Rwine, ITEMATTR_MAXENERGY, Rwine_NOW)
    SetItemAttr(Item_Rwine, ITEMATTR_ENERGY, Rwine_NOW)
    RefreshCha(character)
    SystemNotice(character, "?")
end

function CheckHJ(character)
    local Rwine_Num = 0
    Rwine_Num = CheckBagItem(character, 2967)
    if Rwine_Num == 1 then
        local Rwine = GetChaItem2(character, 2, 2967)
        local ENERGY = GetItemAttr(Rwine, ITEMATTR_ENERGY)
        if ENERGY == 0 then
            return LUA_TRUE
        end
    else
        SystemNotice(character, "")
    end
end

function CreatBBBB(role, MonsterID)
    local x, y = GetChaPos(role)
    x = x + 10
    y = y + 10
    local Refresh = 3600
    local life = 3600000
    local new = CreateChaX(MonsterID, x, y, 145, Refresh, role)
    SetChaHost(new, role)
    SetChaLifeTime(new, life)
    SetChaTarget(new, role)
    local Role_ID = GetRoleID(role)
    BBBB[Role_ID] = new
    local hit = GetChaAttr(new, ATTR_HIT)

    hit = 225
    SetCharaAttr(hit, new, ATTR_HIT)

    return LUA_TRUE
end

function CheckBBBB(role)
    local Role_ID = GetRoleID(role)
    local BBBB = BBBB[Role_ID]
    if BBBB ~= nil and BBBB ~= 0 then
        local ISLive = ValidCha(BBBB)

        local x01, y01 = GetChaPos(role)
        local x02, y02 = GetChaPos(BBBB)
        local X_red = math.abs(x01 - x02)
        local Y_red = math.abs(y01 - y02)
        if X_red <= 2000 and Y_red <= 2000 then
            KillCha(BBBB)

            return LUA_TRUE
        else
            SystemNotice(role, "?")
            return LUA_FALSE
        end
    else
        SystemNotice(role, "?")
        return LUA_FALSE
    end
end

function AddExpNextLv1(role)
    local exp_add = GetChaAttr(role, ATTR_NLEXP)
    local cha_name = GetChaDefaultName(role)
    SetChaAttrI(role, ATTR_CEXP, exp_add)
    RefreshCha(role)
    Notice("?" .. cha_name .. "1")
    return LUA_TRUE
end

function AddExpNextLv2(role)
    local exp_add = GetChaAttr(role, ATTR_NLEXP)
    local cha_name = GetChaDefaultName(role)
    SetChaAttrI(role, ATTR_CEXP, exp_add)
    RefreshCha(role)
    Notice("?" .. cha_name .. "1")
    return LUA_TRUE
end

function ValentinesRingJudge(role)
    local Ring_Num = 0
    local t = {}
    t[0] = role
    t[1] = GetTeamCha(role, 0)
    t[2] = GetTeamCha(role, 1)
    t[3] = GetTeamCha(role, 2)
    t[4] = GetTeamCha(role, 3)
    local t_Num = {}
    t_Num[0] = 0
    t_Num[1] = 0
    t_Num[2] = 0
    t_Num[3] = 0
    t_Num[4] = 0
    local i = 1
    for i = 1, 4, 1 do
        if t[i] ~= nil then
            local Ring_Num_Add = CheckBagItem(t[i], 2521)
            if Ring_Num_Add == 1 then
                local retbag = HasLeaveBagGrid(t[i], 1)
                if retbag ~= LUA_TRUE then
                    SystemNotice(role, "Your Sweethearts Inventory is full!")
                    SystemNotice(t[i], "You need at least 1 free slot!")
                    return LUA_FALSE
                end
                local NocLock = KitbagLock(t[i], 0)
                if NocLock == LUA_FALSE then
                    SystemNotice(role, "Your Sweethearts Inventory is full!")
                    SystemNotice(t[i], "You need at least 1 free slot!")
                    return LUA_FALSE
                end

                t_Num[i] = 1
                Ring_Num = Ring_Num + Ring_Num_Add
                local USED_Ring_Num = CheckBagItem(t[i], 2520)
                if USED_Ring_Num >= 1 then
                    SystemNotice(role, "Your Team mate is already married!")
                    SystemNotice(t[i], "You can only marry once! Get divorced!")
                    return LUA_FALSE
                end
            end
        end
    end

    if Ring_Num == 1 then
        return LUA_TRUE
    elseif Ring_Num > 1 then
        SystemNotice(role, "You need one Valentines Day Ring!")
        return LUA_FALSE
    else
        SystemNotice(role, "Please find someone to marry first!")
        return LUA_FALSE
    end
end

function ValentinesRing(role)
    local Ring_Num = 0
    local t = {}
    t[0] = role
    t[1] = GetTeamCha(role, 0)
    t[2] = GetTeamCha(role, 1)
    t[3] = GetTeamCha(role, 2)
    t[4] = GetTeamCha(role, 3)
    local t_Num = {}
    t_Num[0] = 0
    t_Num[1] = 0
    t_Num[2] = 0
    t_Num[3] = 0
    t_Num[4] = 0
    local i = 1
    for i = 1, 4, 1 do
        if t[i] ~= nil then
            local Ring_Num_Add = CheckBagItem(t[i], 2521)
            if Ring_Num_Add == 1 then
                t_Num[i] = 1
                Ring_Num = Ring_Num + Ring_Num_Add
                local USED_Ring_Num = CheckBagItem(t[i], 2520)
                if USED_Ring_Num >= 1 then
                    SystemNotice(role, "Your team-mate is already married!")
                    SystemNotice(t[i], "You can marry only once! Get divorced!")
                    return LUA_FALSE
                end
            end
        end
    end

    if Ring_Num == 1 then
        local i = 1
        for i = 1, 4, 1 do
            if t_Num[i] == 1 then
                local RoleType = GetChaID(role)
                local TeamerType = GetChaID(t[i])
                if (RoleType <= 2 and TeamerType >= 3) or (RoleType >= 3 and TeamerType <= 2) then
                    local ID_Num = GetRoleID(role)

                    GiveItem(t[i], 0, 2520, 1, 4)
                    local a = DelBagItem(t[i], 2521, 1)

                    local Item = GetChaItem2(t[i], 2, 2520)

                    local Num_JZ = GetItemForgeParam(Item, 1)
                    Num_JZ = TansferNum(Num_JZ)
                    Num_JZ = ID_Num
                    SetItemForgeParam(Item, 1, Num_JZ)
                    AddChaSkill(t[i], SK_QLZX, 1, 1, 0)

                    local ID_Num1 = GetRoleID(t[i])

                    GiveItem(role, 0, 2520, 1, 4)
                    local b = DelBagItem(role, 2521, 1)

                    local Item1 = GetChaItem2(role, 2, 2520)

                    local Num_JZ1 = GetItemForgeParam(Item1, 1)
                    Num_JZ1 = TansferNum(Num_JZ1)
                    Num_JZ1 = ID_Num1

                    SetItemForgeParam(Item1, 1, Num_JZ1)
                    AddChaSkill(role, SK_QLZX, 1, 1, 0)
                else
                    SystemNotice(role, "Please note about your gender!")
                end
            end
        end
        return LUA_TRUE
    elseif Ring_Num > 1 then
        SystemNotice(role, "You can only marry one!")
        return LUA_FALSE
    else
        SystemNotice(role, "Please find someone to marry first!")
        return LUA_FALSE
    end
end

function Checksailexpless(role, value)
    local sail_role = GetChaAttr(role, ATTR_CSAILEXP)
    if sail_role < value then
        return LUA_TRUE
    end
end

function Checksailexpmore(role, value)
    local sail_role = GetChaAttr(role, ATTR_CSAILEXP)
    if sail_role >= value then
        return LUA_TRUE
    end
end

function FairySys(Player, Tick)
	local IsLiving = IsChaLiving(Player)
	if IsLiving == -1 then
		return
	end
	local Fairy = GetEquipItemP(Player, 16)
	if Fairy == nil then
		return
	end
	local FairyType = GetItemType(Fairy)
	if FairyType == 59 then
		local Level = GetFairyLevel(Fairy)
		local Timer = FairyFreq(Level)
		local CheckState = GetChaStateLv(Player, STATE_JLJSGZ)
		if CheckState > 0 then
			Timer   = math.floor(Timer * 0.5)
		end        
		if math.fmod(Tick, Timer) == 0 and Tick > 0 then
			local Stamina = GetItemAttr(Fairy, ITEMATTR_URE)
			if Stamina <= 49 then
				SetChaKbItemValid2(Player, Fairy, 0, 1)
			else
				SetChaKbItemValid2(Player, Fairy, 1, 1)
			end
			FairyCoins(Player, Fairy)
			Take_ElfURE(Player, Fairy, 50)
			Give_ElfEXP(Player, Fairy)
			FairyAF(Player, Fairy)
		end
		FairyCheatCheck(Player, Fairy, Level)
	end
end

function FairyAF(Player, Fairy)
    local Level = GetFairyLevel(Fairy)
    local Ration = GetChaItem2(Player, 2, 2312)
    if Ration ~= nil and Level <= Server.Fairy.Level.Maximum then
        local Percentage = GetItemAttr(Fairy, ITEMATTR_URE) / GetItemAttr(Fairy, ITEMATTR_MAXURE)
        local RationID = GetItemID(Ration)
        if RationID == Server.Fairy.AutoFeed.Normal or RationID == Server.Fairy.AutoFeed.Great then
            if Percentage <= Server.Fairy.AutoFeed.Stamina then
                local j = TakeItem(Player, 0, RationID, 1)
                if j == 0 then
                    SystemNotice(Player, "Deleting of fairy ration failed!")
                else
                    local Stamina = GetItemAttr(Fairy, ITEMATTR_URE)
                    Stamina = Stamina + Server.Fairy.Ration[RationID] * 50
                    SystemNotice(Player, "Fairy auto feed successful, [" .. GetItemName(RationID) .. "] was consumed.")
                    SetItemAttr(Fairy, ITEMATTR_URE, Stamina)
                end
                return
            end
        end
    end
    SynLook(Player)
    RefreshCha(Player)
end

function GetElfUre(Player)
    local item = GetChaItem(Player, 2, 1)
    if item == nil then
        return 0
    end
    local itemType = GetItemType(item)
    if itemType ~= 59 then
        return 0
    end
    local ure = GetItemAttr(item, ITEMATTR_URE)
    return ure
end

function FairyFreq(Level)
    local Frequency = 60
    if Level > 27 then
        Frequency = Frequency + (Level - 27) * 5
    end
    return Frequency
end

function FairyCoins(Player, Fairy)
	local Fairy = GetEquipItemP(Player, 16)
    if Fairy == nil then
        return
    end
    local Level = GetFairyLevel(Fairy)
    local Tick = GetItemAttr(Fairy, ITEMATTR_VAL_FUSIONID)
    local Stamina = GetItemAttr(Fairy, ITEMATTR_URE)
    Tick = Tick + 1
    if GetFairyLevel(Fairy) >= Server.Fairy.Level.Minimum then
        -- if math.fmod(Tick, 1) == 0 and Stamina >= 50 then
            -- GiveItemX(Player, 0, 855, 1, 4)
        -- end
        if math.fmod(Tick, 3) == 0 and Stamina >= 50 then
            GiveItemX(Player, 0, 855, 1, 4)
        end
        if math.fmod(Tick, 30) == 0 and Stamina >= 50 then
            GiveItemX(Player, 0, 2588, 1, 4)
        end
        if math.fmod(Tick, 60) == 0 and Stamina >= 50 then
            GiveItemX(Player, 0, 2588, 1, 4)
        end
        if math.fmod(Tick, 120) == 0 and Stamina >= 50 then
            GiveItemX(Player, 0, 2588, 1, 4)
        end
        if math.fmod(Tick, 1200) == 0 and Stamina >= 50 then
            GiveItemX(Player, 0, 2589, 1, 4)
        end
        if Tick == 1200 then
            Tick = 0
        end
        SetItemAttr(Fairy, ITEMATTR_VAL_FUSIONID, Tick)
    end
end

function AddElfSkill(Player, Fairy, Item, SkillID, SkillLevel)
    local Num = GetItemForgeParam(Fairy, 1)
    local Skill = {GetNum_Part2(Num), GetNum_Part4(Num), GetNum_Part6(Num)}
    local SkillLV = {GetNum_Part3(Num), GetNum_Part5(Num), GetNum_Part7(Num)}
    local FairyName = GetItemName(GetItemID(Fairy))
    local SkillName = GetItemName(GetItemID(Item))

    if Skill[1] == SkillID then
        Num = SetNum_Part3(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
    if Skill[2] == SkillID then
        Num = SetNum_Part5(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
    if Skill[3] == SkillID then
        Num = SetNum_Part7(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
    if Skill[1] == 0 then
        Num = SetNum_Part2(Num, SkillID)
        Num = SetNum_Part3(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
    if Skill[2] == 0 then
        Num = SetNum_Part4(Num, SkillID)
        Num = SetNum_Part5(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
    if Skill[3] == 0 then
        Num = SetNum_Part6(Num, SkillID)
        Num = SetNum_Part7(Num, SkillLevel)
        SetItemForgeParam(Fairy, 1, Num)
        BickerNotice(Player, FairyName .. " successfully learned " .. SkillName .. "!")
        return
    end
end
function FairyCheatCheck(Player, Fairy, Level)
    local PlayerName = GetChaDefaultName(Player)
    local FairyID = GetItemID(Fairy)
    if Level > Server.Fairy.Level.Maximum then
        SetItemAttr(Fairy, ITEMATTR_URE, 0)
        RemoveChaItem(Player, FairyID, 1, 1, 16, 2, 1)
        KickCha(Player)
        Notice("Player [" ..PlayerName .."] has been caught cheating with a fairy higher than maximum level on server, the fairy has been taken away by system. Please avoid trying to cheat with fairies, thanks.")
        LG("Fairy System", "Type:[Cheat], Player:[" .. PlayerName .. "], Fairy:[" .. FairyID .. "], Level:[" .. Level .. "].")
    end
end
function GiveFairyStamina(Player, Item, Num)
	local StaminaOriginal = GetItemAttr(Item, ITEMATTR_URE)
	local Elf_MaxURE = GetItemAttr(Item, ITEMATTR_MAXURE)
	local recStam = Num / 50;
	if (StaminaOriginal + Num) >= Elf_MaxURE then
		recStam = (function(num, idp)
					local mult = 10^(idp or 0)
					return math.floor(num * mult + 0.5) / mult;
		end)((Elf_MaxURE - StaminaOriginal) / 50);
	end
	Stamina = StaminaOriginal + Num
	if Stamina >= Elf_MaxURE then
		Stamina = Elf_MaxURE
	end
	SetItemAttr(Item, ITEMATTR_URE, Stamina)
	SystemNotice(Player, GetItemName(GetItemID(Item))..' recovers '..recStam..' stamina!')
	if StaminaOriginal < 50 then
		SetChaEquipValid(Player, 16, 1)
	end
	SynLook(Player)
	RefreshCha(Player)
end
function CheckFairyEXP(Player, Item)
    if GetItemAttr(Item, ITEMATTR_ENERGY) >= GetItemAttr(Item, ITEMATTR_MAXENERGY) then
        return 1
    else
        return 0
    end
end
function Take_ElfURE(Player, Item, Num)
    local Stamina = GetItemAttr(Item, ITEMATTR_URE)
	if Stamina > 49 then
		Stamina = math.max((Stamina - Num), 49)
		SetItemAttr(Item, ITEMATTR_URE, Stamina)
	else
		SetChaKbItemValid2(Player, Item, 0, 1)
	end
	if Stamina < 50 then
		SetChaEquipValid(Player, 16, 0)
	end
    SynLook(Player)
    RefreshCha(Player)
end

function GetFairyLevel(Fairy)
    local STR = GetItemAttr(Fairy, ITEMATTR_VAL_STR)
    local CON = GetItemAttr(Fairy, ITEMATTR_VAL_CON)
    local AGI = GetItemAttr(Fairy, ITEMATTR_VAL_AGI)
    local ACC = GetItemAttr(Fairy, ITEMATTR_VAL_DEX)
    local SPR = GetItemAttr(Fairy, ITEMATTR_VAL_STA)
    local FairyLv = STR + CON + AGI + ACC + SPR
    return FairyLv
end
function GetFairyStats(Fairy)
    local STR = GetItemAttr(Fairy, ITEMATTR_VAL_STR)
    local CON = GetItemAttr(Fairy, ITEMATTR_VAL_CON)
    local AGI = GetItemAttr(Fairy, ITEMATTR_VAL_AGI)
    local ACC = GetItemAttr(Fairy, ITEMATTR_VAL_DEX)
    local SPR = GetItemAttr(Fairy, ITEMATTR_VAL_STA)
    return STR, CON, AGI, ACC, SPR
end
function FairyLevel_Normal(Player, Fairy)
    local FairyLv = GetFairyLevel(Fairy)
    if FairyLv >= Server.Fairy.Level.Normal then
        return 0
    else
        return 1
    end
end
function FairyLevel_Imprvd(Player, Fairy)
    local FairyLv = GetFairyLevel(Fairy)
    if FairyLv < Server.Fairy.Level.Normal or FairyLv >= Server.Fairy.Level.Improved then
        return 0
    else
        return 1
    end
end
function FairyLevel(Player, Fairy, AttrType, Level, False)
    local FairyLv = GetFairyLevel(Fairy)
    local AttrNum = GetItemAttr(Fairy, AttrType)
    local a = 1 / (math.floor((1 + (math.pow((FairyLv / 10), 3))) * 10) / 10 * math.max(0.01, (1 - AttrNum * 0.05)))
    if FairyLv >= 42 then
        a = a
    end
    local b = Percentage_Random(a)
    local FairyEXP = GetItemAttr(Fairy, ITEMATTR_ENERGY)
    if b == 1 then
        AddItemEffect(Player, Fairy, 0)
        FairyEXP = 0
        SystemNotice(Player, "Fairy levelled up successfully, growth depleted.")
        AttrNum = AttrNum + Level
        SetItemAttr(Fairy, AttrType, AttrNum)
        local Item_MAXENERGY = 240 * (FairyLv + Level)
        if Item_MAXENERGY > 6480 then
            Item_MAXENERGY = 6480
        end
        local Item_MAXURE_NUM = GetItemAttr(Fairy, ITEMATTR_MAXURE) + 1000 * Level
        if Item_MAXURE_NUM > 32000 then
            Item_MAXURE_NUM = 32000
        end
        if Item_MAXURE_NUM == 25000 then
            Item_MAXURE_NUM = 25000 + 1
        end
        SetItemAttr(Fairy, ITEMATTR_MAXENERGY, Item_MAXENERGY)
        SetItemAttr(Fairy, ITEMATTR_MAXURE, Item_MAXURE_NUM)
        ResetItemFinalAttr(Fairy)
        AddItemEffect(Player, Fairy, 1)
    else
        if False == 1 then
            FairyEXP = 0
            SystemNotice(Player, "Fairy level up failed, growth depleted.")
        else
            FairyEXP = 0.5 * FairyEXP
            SystemNotice(Player, "Fairy level up failed, growth reduced by half.")
        end
    end
    if False == 1 then
        SetItemAttr(Fairy, ITEMATTR_URE, 0)
    end
    SetItemAttr(Fairy, ITEMATTR_ENERGY, FairyEXP)
    SynLook(Player)
end
function Give_ElfEXP(Player, Fairy)
    local FairyEXP = GetItemAttr(Fairy, ITEMATTR_ENERGY)
    local FairyMaxEXP = GetItemAttr(Fairy, ITEMATTR_MAXENERGY)
    if GetItemAttr(Fairy, ITEMATTR_URE) > 49 then
        FairyEXP = FairyEXP + Server.Rates.Global.FairyEXP
        if FairyEXP >= FairyMaxEXP then
            FairyEXP = FairyMaxEXP
        end
        SetItemAttr(Fairy, ITEMATTR_ENERGY, FairyEXP)
        RefreshCha(Player)
    end
    SynLook(Player)
end
function MapLevelCheck(MapCopy, minLv, maxLv)
    local Alive = GetMapActivePlayer(MapCopy)
    BeginGetMapCopyPlayerCha(MapCopy)
    for i = 0, Alive - 1, 1 do
        local Player = GetMapCopyNextPlayerCha(MapCopy)
        if (Player ~= 0 or Player ~= nil) then
            if Lv(Player) < minLv or Lv(Player) > maxLv then
                KickCha(Player)
            end
        end
    end
end
function Abaddon_Init()
    Abaddon = {}
    Abaddon.Tick = 4
    Abaddon.Time = {}
    Abaddon.Boss = {}
    Abaddon.AbyssSkill = {}

    Abaddon.Time[5] = 300
    Abaddon.Time[9] = 240
    Abaddon.Time[10] = 180
    Abaddon.Time[18] = 120
    Abaddon.Time[19] = 60

    Abaddon.Boss[5] = {ID = 974, X = 7200, Y = 6200, Life = 21600, Role = nil, Check = 0, Skill = {0, 100, 100, 100, 100, 100}, Message = "Saro: This feeling of suffering is the power of Despair. I'll revive and you all will fall into depths more deeper than despair!!!"}
    Abaddon.Boss[6] = {ID = 975, X = 19100, Y = 6200, Life = 21600, Role = nil, Check = 0, Skill = {0, 100, 100, 100, 100, 100}, Message = "Karu: You might have passed this stage, but prepare for the worst!"}
    Abaddon.Boss[7] = {ID = 976, X = 19000, Y = 17900, Life = 21600, Role = nil, Check = 0, Skill = {0, 100, 100, 100, 100, 100}, Message = "Aruthur: I've been defeated. Please forgive me, my lord, I don't want to go back to hell, Ah!!!"}
    Abaddon.Boss[8] = {ID = 977, X = 7200, Y = 17900, Life = 21600, Role = nil, Check = 0, Skill = {0, 100, 100, 100, 100, 100}, Message = "Sacrois: Foolish humans, Death has bestowed upon us the gift of immortality. We will meet again! Ha-ha! Ha-ha!"}
    Abaddon.Boss[9] = {ID = 978, X = 14211, Y = 9723, Life = 21600, Role = nil, Check = 0, Skill = {0, 100, 100, 100, 100, 100}, Message = "Kuroo: The ancient curse has already activated. You will awaken the most deepest evil when you pass through. May death bring you peace!"}

    Abaddon.Boss[10] = {ID = 979, X = 7300, Y = 18200, Life = 21600, Role = nil, Check = 0, Skill = {0, 1, 8, 8, 12, 12}, Message = "Phantom Baron: How is it possible for the human to counter over my Sword of Phantom. Ain't I your favourite man?"}
    Abaddon.Boss[11] = {ID = 980, X = 7300, Y = 6400, Life = 21600, Role = nil, Check = 0, Skill = {0, 12, 1, 1, 12, 12}, Message = "Demon Flame: Is this chill feeling 'death'? The feeling is still so wonderful, even after three thousands years!"}
    Abaddon.Boss[12] = {ID = 981, X = 18900, Y = 6400, Life = 21600, Role = nil, Check = 0, Skill = {0, 6, 6, 4, 16, 16}, Message = "Evil Beast: My strength is being drained from me! No! I do not wish to be defeated! Ahh...!"}
    Abaddon.Boss[13] = {ID = 982, X = 31100, Y = 6400, Life = 21600, Role = nil, Check = 0, Skill = {0, 4, 4, 4, 16, 16},Message = "Tyran: Is this the pain feeling of being defeated? Seems like we need more training.. Ahh!"}
    Abaddon.Boss[14] = {ID = 983, X = 31100, Y = 18100, Life = 21600, Role = nil, Check = 0, Skill = {0, 12, 12, 12, 12, 12}, Message = "Phoenix: The death of a phoenix is only a beginning! Rebirth of a phoenix will bring death upon you!"}
    Abaddon.Boss[15] = {ID = 984, X = 31100, Y = 30300, Life = 21600, Role = nil, Check = 0, Skill = {0, 16, 16, 16, 4, 4}, Message = "Despair: Am I really wrong to betray the Goddess? Perhaps the power of despair is over riding me. Help me Goddess-!!"}
    Abaddon.Boss[16] = {ID = 985, X = 19000, Y = 30300, Life = 21600, Role = nil, Check = 0, Skill = {0, 16, 16, 16, 1, 1}, Message = "Drakan: Supreme Lord and Harbourer of eternal life. Please take my life, and in return, grant these arrogant fools death!"}
    Abaddon.Boss[17] = {ID = 986, X = 6900, Y = 30300, Life = 21600, Role = nil, Check = 0, Skill = {0, 12, 12, 12, 6, 6}, Message = "Tidal: Red! Why can't my blood be as blue as the sea that I love!!!"}
    Abaddon.Boss[18] = {ID = 987, X = 5600, Y = 5600, Life = 21600, Role = nil, Check = 0, Skill = {0, 4, 4, 4, 4, 4}, Message = "Hardin: Foolish humans, Death has granted us the power of immortality. We will meet again. Wahahaha!!!"}
    Abaddon.Boss[19] = {ID = 988, X = 20200, Y = 10500, Life = 21600, Role = nil, Check = 0, Skill = {0, 4, 4, 4, 4, 4}, Message = "Abyss Supreme: Imbecile mortal! Killing me does not change anything! There will always be greed, despair and misery! I will be back one day! Ha-ha!"}

    Abaddon.AbyssSkill[1] = {ID = 979, Skill = 0, Percentage = 0.9, Message = "Abyss Supreme: Supreme lord of Illusion, awaken!"}
    Abaddon.AbyssSkill[2] = {ID = 980, Skill = 0, Percentage = 0.8, Message = "Abyss Supreme: Flames of the Abyss Demon, burn anew!"}
    Abaddon.AbyssSkill[3] = {ID = 981, Skill = 0, Percentage = 0.7, Message = "Abyss Supreme: Evil Beast! Rip them to shreds!"}
    Abaddon.AbyssSkill[4] = {ID = 982, Skill = 0, Percentage = 0.6, Message = "Abyss Supreme: Supreme shield! Guard me from these puny humans!"}
    Abaddon.AbyssSkill[5] = {ID = 983, Skill = 0, Percentage = 0.5, Message = "Abyss Supreme: Phoenix! Bring your flame of death to these ignorant mortal!"}
    Abaddon.AbyssSkill[6] = {ID = 984, Skill = 0, Percentage = 0.4, Message = "Abyss Supreme: Despair of the deep, show your power once more!"}
    Abaddon.AbyssSkill[7] = {ID = 985, Skill = 0, Percentage = 0.3, Message = "Abyss Supreme: Drakan! Leave the hell of death and listen to my summon!"}
    Abaddon.AbyssSkill[8] = {ID = 986, Skill = 0, Percentage = 0.2, Message = "Abyss Supreme: Tidal! Get rid of all of this rubbish!"}
    Abaddon.AbyssSkill[9] = {ID = 987, Skill = 0, Percentage = 0.1, Message = "Abyss Supreme: My loyal servant! The top warrior of the Abyss! Spread your wings and bring death among the mortals!"}
end

function CheckEquipmentSet(Player, Head, Body, Glove, Shoes, Neck, RHand, LHand, Ring1, Ring2, Bracelet1, Bracelet2, Handguard, Belt)
    if Head ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 0)) ~= Head then
            if GetItemAttr(GetChaItem(Player, 1, 0), ITEMATTR_VAL_FUSIONID) ~= Head then
                return 0
            end
        end
    end
    if Body ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 2)) ~= Body then
            if GetItemAttr(GetChaItem(Player, 1, 2), ITEMATTR_VAL_FUSIONID) ~= Body then
                return 0
            end
        end
    end
    if Glove ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 3)) ~= Glove then
            if GetItemAttr(GetChaItem(Player, 1, 3), ITEMATTR_VAL_FUSIONID) ~= Glove then
                return 0
            end
        end
    end
    if Shoes ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 4)) ~= Shoes then
            if GetItemAttr(GetChaItem(Player, 1, 4), ITEMATTR_VAL_FUSIONID) ~= Shoes then
                return 0
            end
        end
    end
    if Neck ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 5)) ~= Neck then
            return 0
        end
    end
    if RHand ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 9)) ~= RHand then
            if GetItemAttr(GetChaItem(Player, 1, 9), ITEMATTR_VAL_FUSIONID) ~= RHand then
                return 0
            end
        end
    end
    if LHand ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 6)) ~= LHand then
            if GetItemAttr(GetChaItem(Player, 1, 6), ITEMATTR_VAL_FUSIONID) ~= LHand then
                return 0
            end
        end
    end
    if Ring1 ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 7)) ~= Ring1 then
            return 0
        end
    end
    if Ring2 ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 8)) ~= Ring2 then
            return 0
        end
    end
    if Bracelet1 ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 10)) ~= Bracelet1 then
            return 0
        end
    end
    if Bracelet2 ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 11)) ~= Bracelet2 then
            return 0
        end
    end
    if Handguard ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 13)) ~= Handguard then
            return 0
        end
    end
    if Belt ~= 0 then
        if GetItemID(GetChaItem(Player, 1, 12)) ~= Belt then
            return 0
        end
    end
    return 1
end
Server.EqSet["Kylin"] = function(Player)
    if CheckEquipmentSet(Player, 0, 0825, 0826, 0827, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0, 0831, 0832, 0833, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0, 0834, 0835, 0836, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0, 0837, 0838, 0839, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0843, 0840, 0841, 0842, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0, 2549, 2550, 2551, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    return 0
end
Server.EqSet["Chaos"] = function(Player)
    if CheckEquipmentSet(Player, 0, 1124, 1125, 1126, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    return 0
end
Server.EqSet["BlackDragon"] = function(Player)
    if CheckEquipmentSet(Player, 0, 0845, 0846, 0847, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    if CheckEquipmentSet(Player, 0, 2367, 2368, 2369, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 1 then
        return 1
    end
    return 0
end
function DropItemEvent(Player, PosX, PosY, ItemID, Amount, MapCopyID)
    if PosX == nil and PosY == nil then
        PosX, PosY = GetChaPos(Player)
    end
    local MapCopy = 1
    if Player ~= nil then
        MapCopy = GetChaMapCopy(Player)
    end
    if MapCopyID ~= nil then
        MapCopy = MapCopyID
    end
    local Monster = CreateChaEx(1, PosX, PosY, 145, 50, MapCopy)
    SetChaLifeTime(Monster, 1)
    GiveItem(Monster, 0, ItemID, Amount, 4)

    RemoveChaItem(Monster, ItemID, Amount, 2, -1, 0, 1)
end
function StatBind(Player)
    local Point_A = 0
    local Point_S = 0
    SetChaAttr(Player, ATTR_BSTR, 5)
    SetChaAttr(Player, ATTR_BDEX, 5)
    SetChaAttr(Player, ATTR_BAGI, 5)
    SetChaAttr(Player, ATTR_BCON, 5)
    SetChaAttr(Player, ATTR_BSTA, 5)
    for Level = 1, Lv(Player), 1 do
        if (math.floor((Level) / 10) - math.floor((Level - 1) / 10)) == 1 then
            Point_A = Point_A + 5
        else
            Point_A = Point_A + 1
        end
        if Level >= 60 then
            Point_A = Point_A + 1
        end
        if Level > 9 then
            Point_S = Point_S + 1
        end
        if Level >= 65 then
            if (math.floor((Level) / 5) - math.floor((Level - 1) / 5)) == 1 then
                Point_S = Point_S + 2
            else
                Point_S = Point_S + 1
            end
        end
    end
    SetChaAttr(Player, ATTR_AP, Point_A)
    SyncChar(Player, 4)
end
function ExpSystem(Player, Amount)
    if Server.Sys.Level.Active == false then
        return
    end
    Player = TurnToCha(Player)
    local EXP = GetChaAttrI(Player, ATTR_CEXP)
    local Level = GetChaAttrI(Player, ATTR_LV)
    if Server.Sys.Rates.Map.Active == true and Server.Rates.Map[GetChaMapName(Player)] ~= nil then
        Amount = Amount * Server.Rates.Map[GetChaMapName(Player)]
    end
	if Server.Sys.Rates.Day.Active == true and Server.Rates.Day[GetNowWeek()] ~= nil then
		Amount = Amount * Server.Rates.Day[GetNowWeek()]
	end
    if EXP < DEXP[Server.Level.Limit] and (EXP + Amount) >= DEXP[Server.Level.Limit] then
        Amount = math.floor(DEXP[Server.Level.Limit] - EXP)
    end
	local ExpStone = GetChaItem2(Player, 2, 9621)
	local ON = GetItemAttr(ExpStone, 55)
	if ON == 1 then
		Amount = 0
	end
    if GetChaAttrI(Player, ATTR_LV) < Server.Level.Limit and Amount > 0 then
        EXP = EXP + Amount
        SetChaAttrI(Player, ATTR_CEXP, EXP)
        SyncChar(Player, 4)
    end
    return LUA_TRUE
end

ForgeR = {}
ForgeR.ItemTypes = {}
ForgeR.Maps = {}
ForgeR.Places = {Equip = true, Equipped = {0, 2, 3, 4, 6, 9}, Inventory = true}
ForgeR.Maps["garner2"] = {Level = 5, Gems = {12}}

function ItemForgeLevel(Item)
    if GetItemForgeParam(Item, 1) == 0 then
        return 0
    end
    local Num = TansferNum(GetItemForgeParam(Item, 1))
    local Stone = {Info = {}, Level = {}}
    Stone.Info[0], Stone.Info[1], Stone.Info[2] = 0, 0, 0
    Stone.Level[0], Stone.Level[1], Stone.Level[2] = 0, 0, 0
    Stone.Info[0], Stone.Info[1], Stone.Info[2], Stone.Level[0], Stone.Level[1], Stone.Level[2] = CheckStoneInfo(Num)
    return (Stone.Level[0] + Stone.Level[1] + Stone.Level[2]), Stone.Info[0], Stone.Info[1], Stone.Info[2]
end
function RestrictForgeLevel(Player, Map)
    if ForgeR.Maps[Map] ~= nil then
        for A = 1, #ForgeR.Places.Equipped, 1 do
            local Item = GetChaItem(Player, 1, ForgeR.Places.Equipped[A])
            local ForgeLevel, Gem1, Gem2, Gem3 = ItemForgeLevel(Item)
            if ForgeR.Maps[Map].Level ~= nil and ForgeR.Maps[Map].Level < ForgeLevel then
                return 0
            end
            for B = 1, #ForgeR.Maps[Map].Gems, 1 do
                if
                    ForgeR.Maps[Map].Gems[B] == Gem1 or ForgeR.Maps[Map].Gems[B] == Gem2 or
                        ForgeR.Maps[Map].Gems[B] == Gem3
                 then
                    return 0
                end
            end
        end
        for C = 0, GetKbCap(Player), 1 do
            local Item = GetChaItem(Player, 2, C)
            local ForgeLevel, Gem1, Gem2, Gem3 = ItemForgeLevel(Item)
            if ForgeR.Maps[Map].Level ~= nil and ForgeR.Maps[Map].Level < ForgeLevel then
                return 0
            end
            for B = 1, #ForgeR.Maps[Map].Gems, 1 do
                if ForgeR.Maps[Map].Gems[B] == Gem1 or ForgeR.Maps[Map].Gems[B] == Gem2 or ForgeR.Maps[Map].Gems[B] == Gem3 then
                    return 0
                end
            end
        end
    end
    return 1
end
function GetItemForge(Item)
    local Forge = {}
    local Param = GetItemForgeParam(Item, 1)
    Param = TansferNum(Param)
    Forge[1] = GetNum_Part2(Param)
    Forge[2] = GetNum_Part3(Param)
    Forge[3] = GetNum_Part4(Param)
    Forge[4] = GetNum_Part5(Param)
    Forge[5] = GetNum_Part6(Param)
    Forge[6] = GetNum_Part7(Param)
    return Forge
end
function GetGemID(StoneID)
    for A = 0, #GemVar, 1 do
        if GemVar[A].ID == StoneID then
            return A
        end
    end
    return 0
end
function GetForgeGemID(Player, Item)
	for A = 0, #GemVar, 1 do
		local Forge = GetItemForge(Item)
		if GemVar[A].ID == Forge[1] then
			return Forge[1]
		end
	end
    return 0
end
function GetGem(Item, StoneID)
    local Forge = GetItemForge(Item)
    if Forge[1] == StoneID or Forge[3] == StoneID or Forge[5] == StoneID then
        return 1
    end
    return 0
end
function GetGemLevel(Item, StoneID)
    local Forge = GetItemForge(Item)
    if Forge[1] == StoneID then
        return Forge[2]
    end
    if Forge[3] == StoneID then
        return Forge[4]
    end
    if Forge[5] == StoneID then
        return Forge[6]
    end
end
function GetItemOriginalID(Equipment)
    local ItemID = GetItemID(Equipment)
    if GetItemAttr(Equipment, ITEMATTR_VAL_FUSIONID) ~= 0 then
        ItemID = GetItemAttr(Equipment, ITEMATTR_VAL_FUSIONID)
    end
    return ItemID
end
function Say(character, text)
    local map_copy = GetChaMapCopy(character)
    local ply_num = GetMapCopyPlayerNum(map_copy)
    local ps = {}
    local i = 1
    BeginGetMapCopyPlayerCha(map_copy)
    for i = 1, ply_num, 1 do
        ps = GetMapCopyNextPlayerCha(map_copy)
    end

    for i = 1, ply_num, 1 do
        if (ps ~= 0 and ps ~= nil) then
            CharSay(ps, character, text)
        end
    end
end
function MissingLines()
    local Path = GetResPath("script/var/Logs/")
    local File = Path .. "MissingLines.txt"
    local Value = 0
    for i = 1, 5000, 1 do
        if GetItemName(i) == "Unknown" then
            LogFile(Path, File, "Item ID " .. i .. " is free.")
            Value = Value + 1
        end
    end
    LogFile(Path, File, "A total of " .. Value .. " free lines.")
end

GetExp = {}
GetExp[1] = 0
GetExp[2] = 5
GetExp[3] = 15
GetExp[4] = 35
GetExp[5] = 101
GetExp[6] = 250
GetExp[7] = 500
GetExp[8] = 1000
GetExp[9] = 1974
GetExp[10] = 3208
GetExp[11] = 4986
GetExp[12] = 7468
GetExp[13] = 10844
GetExp[14] = 15338
GetExp[15] = 21210
GetExp[16] = 28766
GetExp[17] = 38356
GetExp[18] = 50382
GetExp[19] = 65306
GetExp[20] = 83656
GetExp[21] = 106032
GetExp[22] = 133112
GetExp[23] = 165668
GetExp[24] = 204564
GetExp[25] = 250780
GetExp[26] = 305412
GetExp[27] = 369692
GetExp[28] = 444998
GetExp[29] = 532870
GetExp[30] = 635026
GetExp[31] = 753378
GetExp[32] = 890062
GetExp[33] = 1047438
GetExp[34] = 1228138
GetExp[35] = 1435074
GetExp[36] = 1671470
GetExp[37] = 1940892
GetExp[38] = 2247288
GetExp[39] = 2595010
GetExp[40] = 2988860
GetExp[41] = 3434132
GetExp[42] = 3936658
GetExp[43] = 4502856
GetExp[44] = 5139778
GetExp[45] = 5855180
GetExp[46] = 6657576
GetExp[47] = 7556310
GetExp[48] = 8561630
GetExp[49] = 9684764
GetExp[50] = 10938016
GetExp[51] = 12334856
GetExp[52] = 13890020
GetExp[53] = 15619622
GetExp[54] = 17541282
GetExp[55] = 19674240
GetExp[56] = 22039516
GetExp[57] = 24660044
GetExp[58] = 27560852
GetExp[59] = 30769230
GetExp[60] = 37746418
GetExp[61] = 45876427
GetExp[62] = 59571153
GetExp[63] = 75703638
GetExp[64] = 94615279
GetExp[65] = 116688304
GetExp[66] = 155291059
GetExp[67] = 186418013
GetExp[68] = 238159614
GetExp[69] = 298622278
GetExp[70] = 368975850
GetExp[71] = 450525549
GetExp[72] = 568409779
GetExp[73] = 679324744
GetExp[74] = 806544569
GetExp[75] = 952091724
GetExp[76] = 1188099236
GetExp[77] = 1480429211
GetExp[78] = 1776125584
GetExp[79] = 2091634902
GetExp[80] = 2425349810
GetExp[81] = 2440895086
GetExp[82] = 2458896515
GetExp[83] = 2479742169
GetExp[84] = 2503881436
GetExp[85] = 2531834707
GetExp[86] = 2564204594
GetExp[87] = 2601688923
GetExp[88] = 2645095775
GetExp[89] = 2695360909
GetExp[90] = 2753567934
GetExp[91] = 2820971668
GetExp[92] = 2899025191
GetExp[93] = 2989411170
GetExp[94] = 3094078133
GetExp[95] = 3215282476
GetExp[96] = 3355637105
GetExp[97] = 3518167765
GetExp[98] = 3706378269
GetExp[99] = 3924326032
GetExp[100] = 4176709541

HexatlonActive = true
function HexatlonTime(character)
    local week, hour = os.date("%w"), os.date("%H")
    week = tonumber(week)
    hour = tonumber(hour)
    if HexatlonActive == true then
        if week == 7 then
            if hour >= 6 and hour < 7 then
                return LUA_TRUE
            elseif hour >= 15 and hour < 16 then
                return LUA_TRUE
            end
        end
    else
        return LUA_FALSE
    end
end

function HexaTeamCheck(Player)
    local player = {}
    player[1] = Player
    player[2] = GetTeamCha(Player, 0)
    player[3] = GetTeamCha(Player, 1)
    player[4] = GetTeamCha(Player, 2)
    player[5] = GetTeamCha(Player, 3)
    local n1 = 0
    local n2 = 0
    local n3 = 0
    for j = 0, 5, 1 do
        if (ValidCha(player[j]) == 1) then
            local lv_p = GetChaAttr(player[j], ATTR_LV)
            if lv_p >= 20 and lv_p <= 30 then
                n1 = n1 + 1
            elseif lv_p > 30 and lv_p <= 40 then
                n2 = n2 + 1
            elseif lv_p > 40 then
                n3 = n3 + 1
            end
        end
    end
    if n1 >= 1 and n2 >= 1 and n3 >= 1 then
        return LUA_TRUE
    end
end

function AddExp_1(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = expUp
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 0.8)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 0.2)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.2)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.1)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.05)
    else
        expAdd = math.floor(expUp * 0.03)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function AddExp_2(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = expUp
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 0.9)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 0.3)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.22)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.11)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.055)
    else
        expAdd = math.floor(expUp * 0.033)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function AddExp_3(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = math.floor(expUp * 1.2)
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 1)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 0.5)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.24)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.078)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.0083)
    else
        expAdd = math.floor(expUp * 0.0045)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function AddExp_4(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = math.floor(expUp * 1.4)
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 1)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 0.8)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.4)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.2)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.10)
    else
        expAdd = math.floor(expUp * 0.06)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function AddExp_5(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = math.floor(expUp * 1.6)
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 1.1)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 1)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.44)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.22)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.11)
    else
        expAdd = math.floor(expUp * 0.066)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function AddExp_6(Player)
    local chaLv = GetChaAttr(Player, ATTR_LV)
    local lvNext = chaLv + 1
    local expUp = GetExp[lvNext] - GetExp[chaLv]
    local expAdd = 0
    if chaLv >= 1 and chaLv <= 20 then
        expAdd = math.floor(expUp * 1.8)
    elseif chaLv > 20 and chaLv <= 30 then
        expAdd = math.floor(expUp * 1.2)
    elseif chaLv > 30 and chaLv <= 40 then
        expAdd = math.floor(expUp * 1.2)
    elseif chaLv > 40 and chaLv <= 50 then
        expAdd = math.floor(expUp * 0.5)
    elseif chaLv > 50 and chaLv <= 60 then
        expAdd = math.floor(expUp * 0.25)
    elseif chaLv > 60 and chaLv <= 70 then
        expAdd = math.floor(expUp * 0.125)
    else
        expAdd = math.floor(expUp * 0.075)
    end
    AddExp(Player, npc, expAdd, expAdd)
    return LUA_TRUE
end

function GetOsTime()
    local Hour = tonumber(os.date("%H"))
    local Minute = tonumber(os.date("%M"))
    local Second = tonumber(os.date("%S"))
    return Hour, Minute, Second
end

function ForgeItem(Item, Sockets, Gem1, Level1, Gem2, Level2, Gem3, Level3)
    local param = GetItemForgeParam(Item, 1)
    local Item_Stone = {}
    local Item_StoneLv = {}
    if Gem1 ~= nil then
        param = TansferNum(param)
        Item_Stone[0] = GetNum_Part2(param)
        Item_StoneLv[0] = GetNum_Part3(param)
        Item_Stone[0] = Gem1
        Item_StoneLv[0] = Level1
        param = SetNum_Part2(param, Item_Stone[0])
        param = SetNum_Part3(param, Item_StoneLv[0])
        SetItemForgeParam(Item, 1, param)
    end
    if Gem2 ~= nil then
        param = TansferNum(param)
        Item_Stone[1] = GetNum_Part2(param)
        Item_StoneLv[1] = GetNum_Part3(param)
        Item_Stone[1] = Gem2
        Item_StoneLv[1] = Level2
        param = SetNum_Part4(param, Item_Stone[1])
        param = SetNum_Part5(param, Item_StoneLv[1])
        SetItemForgeParam(Item, 1, param)
    end
    if Gem3 ~= nil then
        param = TansferNum(param)
        Item_Stone[2] = GetNum_Part2(param)
        Item_StoneLv[2] = GetNum_Part3(param)
        Item_Stone[2] = Gem3
        Item_StoneLv[2] = Level3
        param = SetNum_Part6(param, Item_Stone[2])
        param = SetNum_Part7(param, Item_StoneLv[2])
        SetItemForgeParam(Item, 1, param)
    end

    local Socket = GetItemForgeParam(Item, 1)
    Socket = TansferNum(Socket)
    Socket = SetNum_Part1(Socket, Sockets)
    SetItemForgeParam(Item, 1, Socket)
end

function MakeForgedItems(Player, ItemID, Quality, Sockets, Gem1, Level1, Gem2, Level2, Gem3, Level3)
    local r1, r2 = MakeItem(Player, ItemID, 1, Quality)
    local Item = GetChaItem(Player, 2, r2)
    ForgeItem(Item, Sockets, Gem1, Level1, Gem2, Level2, Gem3, Level3)
    RefreshCha(Player)
end

function GetRebirthLV(role)
	local CSAILEXP = GetChaAttr(role, ATTR_CSAILEXP)
	if CSAILEXP < 2000 then 
		return 0
	elseif CSAILEXP > 2000 and CSAILEXP <= 9000 then 
		return 1
	elseif CSAILEXP > 9000 and CSAILEXP <= 10000 then 
		return 2
	elseif CSAILEXP >= 10000 then return 3
	end
end

function SkillReset(Player)
    local Skills = {
        {0453, 0},
        {0454, 0},
        {0455, 0},
        {0456, 0},
        {0457, 0},
        {0458, 0},
        {0459, 0},
        {0256, 0},
        {0255, 0},
        {0467, 0}
    }
    local Points = 0
    for A = 1, #Skills, 1 do
        Skills[A][2] = GetSkillLv(Player, Skills[A][1])
        Points = Points - Skills[A][2]
    end
    Points = Points + GetChaAttr(Player, ATTR_TP)
    Points = Points + ClearAllFightSkill(Player)
    if Points >= 200 then
        Points = 200
    end
    for B = 1, #Skills, 1 do
        if Skills[B][2] ~= 0 then
            AddChaSkill(Player, Skills[B][1], Skills[B][2], Skills[B][2], 0)
        end
    end
    SetChaAttr(Player, ATTR_TP, Points)
    RefreshCha(Player)
end

function PlayerReset(Player)
    for i, v in pairs({ATTR_BSTR, ATTR_BDEX, ATTR_BAGI, ATTR_BCON, ATTR_BSTA}) do
        SyncChar(Player, 4)
        SetChaAttr(Player, v, 5)
    end
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_CEXP, 0)
    SyncChar(Player, 4)
    SetChaAttr(Player, ATTR_LV, 1)
    SyncChar(Player, 4)
    local HP = GetChaAttr(Player, ATTR_MXHP)
    SetCharaAttr(HP, Player, ATTR_HP)
    SyncChar(Player, 4)
    local SP = GetChaAttr(Player, ATTR_MXSP)
    SetCharaAttr(SP, Player, ATTR_SP)
    SyncChar(Player, 4)
    local AP = GetChaAttr(Player, ATTR_AP) + 4
    SetCharaAttr(AP, Player, ATTR_AP)
    SyncChar(Player, 4)
    SkillReset(Player)
    SetChaAttr(Player, ATTR_TP, 0)
    SetChaAttr(Player, ATTR_JOB, 0)
    RefreshCha(Player)
end

function MasterAccount(Player)
    local AccountName = GetActName(Player)
    if (IsPlayer(Player) == 1) then
        if GetGmLv(Player) == 99 then
            if not AuthorizedGM[AccountName] then
                LG("Unauthorized gm attempt", "" .. AccountName .. " attempted to login in game.")
                KickCha(Player)
                BanActRole(Player)
            end
        end
    end
end

function CanLevelFairySkill(Player, Fairy, Item, SkillID, SkillLevel)
    local Num = GetItemForgeParam(Fairy, 1)
    local Skill = {GetNum_Part2(Num), GetNum_Part4(Num), GetNum_Part6(Num)}
    local SkillLV = {GetNum_Part3(Num), GetNum_Part5(Num), GetNum_Part7(Num)}
    local FairyName = GetItemName(GetItemID(Fairy))
    local SkillName = GetItemName(GetItemID(Item))

    for i = 1, #Skill do
        if SkillID == Skill[i] then
            if SkillLevel <= SkillLV[i] then
                BickerNotice(Player, FairyName .. " already learned " .. SkillName .. "!")
                return false
            else
                if SkillLevel ~= (SkillLV[i] + 1) then
                    BickerNotice(Player, FairyName .. " must learn " .. GetItemName((GetItemID(Item) - 1)) .. " first!")
                    return false
                else
                    return true
                end
            end
        end
    end
    local Name
    if SkillLevel ~= 1 then
        for i,v in pairs(Server.Fairy.Skill) do
            if v.Skill == SkillID and v.Level == 1 then
                Name = GetItemName(i)
                break
            end
        end
        BickerNotice(Player, FairyName.." must learn "..Name.." first!")
        return false
    end
    return true
end
