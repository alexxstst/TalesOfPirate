print("-- [Loading] Item Get Mission")

function CheckMisChaBoat(role, ChaType)
    local Cha_Boat = GetCtrlBoat(role)
    local ChaIsBoat = 0

    if Cha_Boat == nil then
        ChaIsBoat = 1
    else
        ChaIsBoat = 2
    end

    if ChaIsBoat == ChaType then
        return 1
    else
        return 0
    end
end

function CheckChaPos(role, Cha_x_min, Cha_x_max, Cha_y_min, Cha_y_max, MapName)
    local Cha_Boat = GetCtrlBoat(role)
    local ChaIsBoat = 0
    if Cha_Boat ~= nil then
        role = Cha_Boat
    end

    local pos_x, pos_y = GetChaPos(role)
    local map_name = GetChaMapName(role)

    if MapName ~= -1 then
        if map_name ~= MapName then
            return 0
        end
    end

    if pos_x < Cha_x_min or pos_x > Cha_x_max then
        return 0
    end

    if pos_y < Cha_y_min or pos_y > Cha_y_max then
        return 0
    end

    return 1
end

function CheckChaGuildType(role, GuildType, CheckType)
    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        return 0
    end

    if CheckType == 1 then
        if ChaGuildType_Get == GuildType then
            return 1
        else
            return 0
        end
    elseif CheckType == 2 then
        if ChaGuildType_Get == GuildType then
            return 0
        else
            return 1
        end
    else
        return 0
    end
end

function MissionMsgCheck(
    role,
    HasRecordID,
    NoRecordID,
    HasMissionID,
    No_MissionID,
    HasRecordNotice,
    NoRecordNotice,
    HasMissionNotice,
    NoMissionNotice)
    local Have_Record = 0
    local No_Record = 0
    local Have_Mission = 0
    local No_Mission = 0

    if HasRecordID ~= -1 then
        Have_Record = HasRecord(role, HasRecordID)
        if Have_Record ~= LUA_TRUE then
            SystemNotice(role, HasRecordNotice)
            return 0
        end
    end

    if NoRecordID ~= -1 then
        No_Record = NoRecord(role, NoRecordID)
        if No_Record ~= LUA_TRUE then
            SystemNotice(role, NoRecordNotice)
            return 0
        end
    end

    if HasMissionID ~= -1 then
        Have_Mission = HasMission(role, HasMissionID)
        if Have_Mission ~= LUA_TRUE then
            SystemNotice(role, HasMissionNotice)
            return 0
        end
    end

    if No_MissionID ~= -1 then
        No_Mission = HasMission(role, No_MissionID)
        if No_Mission == LUA_TRUE then
            SystemNotice(role, NoMissionNotice)
            return 0
        end
    end

    return 1
end

function ChaMsgCheck(
    role,
    ChaType,
    Need_CheckPos,
    Cha_x_max,
    Cha_x_min,
    Cha_y_max,
    Cha_y_min,
    MapName,
    GuildType,
    CheckType,
    CheckBoatNotice,
    CheckPosNotice,
    GuildTypeNotice)
    local Is_BoatOrMan = 0
    local At_Pos = 0
    local CheckGuild_Type = 0

    if ChaType ~= -1 then
        Is_BoatOrMan = CheckMisChaBoat(role, ChaType)
        if Is_BoatOrMan == 0 then
            SystemNotice(role, CheckBoatNotice)
            return 0
        end
    end

    if Need_CheckPos == 1 then
        At_Pos = CheckChaPos(role, Cha_x_min, Cha_x_max, Cha_y_min, Cha_y_max, MapName)
        if At_Pos == 0 then
            SystemNotice(role, CheckPosNotice)
            return 0
        end
    end

    if GuildType ~= -1 then
        CheckGuild_Type = CheckChaGuildType(role, GuildType, CheckType)
        if CheckGuild_Type == 0 then
            SystemNotice(role, GuildTypeNotice)
            return 0
        end
    end

    return 1
end

function ItemUse_LDADYW(role)
    local HasRecordID = 270
    local HasRecordID_1 = -1
    local NoRecordID = -1
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = -1
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 4096
    local Cha_x_min = 0
    local Cha_y_max = 4096
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Memento of Andrew shows no responese"
    local HasRecordNotice_1 = "Memento of Andrew shows no responese"
    local NoRecordNotice = "Memento of Andrew shows no responese"
    local NoRecordNotice_1 = "Memento of Andrew shows no responese"
    local HasMissionNotice = "Memento of Andrew shows no responese"
    local HasMissionNotice_1 = "Memento of Andrew shows no responese"
    local NoMissionNotice = "Memento of Andrew shows no responese"
    local NoMissionNotice_1 = "Memento of Andrew shows no responese"
    local CheckBoatNotice = "Memento of Andrew shows no responese"
    local CheckPosNotice = "Memento of Andrew shows no responese"
    local GuildTypeNotice = "Memento of Andrew shows no responese"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 9
    local GiveMisson_2 = 8
    local GiveMisson_0 = 10
    local ItemID = -1
    local ItemNum = 1
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    if ItemID ~= -1 then
        local Item_CanGet = GetChaFreeBagGridNum(role)

        if Item_CanGet < 1 then
            SystemNotice(role, "Inventory space insufficient. Usage of item failed")
            UseItemFailed(role)
            return
        end
    end

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end
function ItemUse_GLDYS(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 15
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 15
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 4096
    local Cha_x_min = 0
    local Cha_y_max = 4096
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "This ancient key has the logo of Thundoria Bank on it"
    local HasRecordNotice_1 = "This ancient key has the logo of Thundoria Bank on it"
    local NoRecordNotice = "This ancient key has the logo of Thundoria Bank on it"
    local NoRecordNotice_1 = "This ancient key has the logo of Thundoria Bank on it"
    local HasMissionNotice = "This ancient key has the logo of Thundoria Bank on it"
    local HasMissionNotice_1 = "This ancient key has the logo of Thundoria Bank on it"
    local NoMissionNotice = "This ancient key has the logo of Thundoria Bank on it"
    local NoMissionNotice_1 = "This ancient key has the logo of Thundoria Bank on it"
    local CheckBoatNotice = "This ancient key has the logo of Thundoria Bank on it"
    local CheckPosNotice = "This ancient key has the logo of Thundoria Bank on it"
    local GuildTypeNotice = "This ancient key has the logo of Thundoria Bank on it"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 15
    local GiveMisson_2 = 15
    local GiveMisson_0 = 15
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_LDADYS(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 16
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 16
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 4096
    local Cha_x_min = 0
    local Cha_y_max = 4096
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Andrew's Will has some unknown scribbling on it"
    local HasRecordNotice_1 = "Andrew's Will has some unknown scribbling on it"
    local NoRecordNotice = "Andrew's Will has some unknown scribbling on it"
    local NoRecordNotice_1 = "Andrew's Will has some unknown scribbling on it"
    local HasMissionNotice = "Andrew's Will has some unknown scribbling on it"
    local HasMissionNotice_1 = "Andrew's Will has some unknown scribbling on it"
    local NoMissionNotice = "Andrew's Will has some unknown scribbling on it"
    local NoMissionNotice_1 = "Andrew's Will has some unknown scribbling on it"
    local CheckBoatNotice = "Andrew's Will has some unknown scribbling on it"
    local CheckPosNotice = "Andrew's Will has some unknown scribbling on it"
    local GuildTypeNotice = "Andrew's Will has some unknown scribbling on it"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 16
    local GiveMisson_2 = 16
    local GiveMisson_0 = 16
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_YXYSJY(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 18
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 18
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 1845
    local Cha_x_min = 1841
    local Cha_y_max = 1719
    local Cha_y_min = 1715
    local MapName = "magicsea"
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local HasRecordNotice_1 = "Where do you wish to apply the Invisible Ink Negator?"
    local NoRecordNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local NoRecordNotice_1 = "Where do you wish to apply the Invisible Ink Negator?"
    local HasMissionNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local HasMissionNotice_1 = "Where do you wish to apply the Invisible Ink Negator?"
    local NoMissionNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local NoMissionNotice_1 = "Where do you wish to apply the Invisible Ink Negator?"
    local CheckBoatNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local CheckPosNotice = "Where do you wish to apply the Invisible Ink Negator?"
    local GuildTypeNotice = "Where do you wish to apply the Invisible Ink Negator?"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 18
    local GiveMisson_2 = 18
    local GiveMisson_0 = 18
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_RYDKL(role)
    local HasRecordID = 287
    local HasRecordID_1 = -1
    local NoRecordID = 20
    local NoRecordID_1 = -1
    local HasMissionID = 19
    local HasMissionID_1 = -1
    local No_MissionID = 20
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = 1
    local Cha_x_max = 184500
    local Cha_x_min = 184100
    local Cha_y_max = 171900
    local Cha_y_min = 171500
    local MapName = "magicsea"
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local HasRecordNotice_1 = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local NoRecordNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local NoRecordNotice_1 = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local HasMissionNotice = "Why don't you look for Jack"
    local HasMissionNotice_1 = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local NoMissionNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local NoMissionNotice_1 = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local CheckBoatNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local CheckPosNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'
    local GuildTypeNotice = 'Mermaid Carcass has an ancient word carving "(1843, 1717)"'

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )
    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )
    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 20
    local GiveMisson_2 = 20
    local GiveMisson_0 = 20

    local ItemID = 4231
    local ItemNum = 1
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Item_CanGet = GetChaFreeBagGridNum(role)
    if Item_CanGet < 1 then
        SystemNotice(role, "Inventory space insufficient. Usage of item failed")
        UseItemFailed(role)
        return
    end

    local Cha_GuildID = GetChaGuildID(role)
    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_SXTCQ(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 29
    local NoRecordID_1 = -1
    local HasMissionID = 381
    local HasMissionID_1 = -1
    local No_MissionID = 29
    local No_MissionID_1 = -1
    local ChaType = 2
    local Need_CheckPos = 1
    local Cha_x_max = 375900
    local Cha_x_min = 375500
    local Cha_y_max = 125000
    local Cha_y_min = 124600
    local MapName = "magicsea"
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Detection failed"
    local HasRecordNotice_1 = "Detection failed"
    local NoRecordNotice = "Detection failed"
    local NoRecordNotice_1 = "Detection failed"
    local HasMissionNotice = "Detection failed"
    local HasMissionNotice_1 = "Detection failed"
    local NoMissionNotice = "Detection failed"
    local NoMissionNotice_1 = "Detection failed"
    local CheckBoatNotice = "Underwater Detector can only be used while sailing"
    local CheckPosNotice = "Detection failed"
    local GuildTypeNotice = "Detection failed"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 29
    local GiveMisson_2 = 29
    local GiveMisson_0 = 29
    local ItemID = -1
    local ItemNum = 1
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    if ItemID ~= -1 then
        local Item_CanGet = GetChaFreeBagGridNum(role)

        if Item_CanGet < 1 then
            SystemNotice(role, "Inventory space insufficient. Usage of item failed")
            UseItemFailed(role)
            return
        end
    end

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_RJDYW(role)
    local HasRecordID = 290
    local HasRecordID_1 = -1
    local NoRecordID = 21
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 21
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "The diary records the mysterious journey of Andrew"
    local HasRecordNotice_1 = "The diary records the mysterious journey of Andrew"
    local NoRecordNotice = "The diary records the mysterious journey of Andrew"
    local NoRecordNotice_1 = "The diary records the mysterious journey of Andrew"
    local HasMissionNotice = "The diary records the mysterious journey of Andrew"
    local HasMissionNotice_1 = "The diary records the mysterious journey of Andrew"
    local NoMissionNotice = "The diary records the mysterious journey of Andrew"
    local NoMissionNotice_1 = "The diary records the mysterious journey of Andrew"
    local CheckBoatNotice = "The diary records the mysterious journey of Andrew"
    local CheckPosNotice = "The diary records the mysterious journey of Andrew"
    local GuildTypeNotice = "The diary records the mysterious journey of Andrew"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 21
    local GiveMisson_2 = 21
    local GiveMisson_0 = 21
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_HL(role)
    local HasRecordID = 315
    local HasRecordID_1 = -1
    local NoRecordID = 22
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 22
    local No_MissionID_1 = -1
    local ChaType = 2
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local HasRecordNotice_1 = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local NoRecordNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local NoRecordNotice_1 = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local HasMissionNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local HasMissionNotice_1 = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local NoMissionNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local NoMissionNotice_1 = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local CheckBoatNotice =
        "The compass gives out a light glow and point towards a certain direction on the sea of Ascaron"
    local CheckPosNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"
    local GuildTypeNotice = "The wheel glows and point towards the direction of Ascaron (1497, 1707)"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 22
    local GiveMisson_2 = 22
    local GiveMisson_0 = 22
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_WNYDYF(role)
    local HasRecordID = 328
    local HasRecordID_1 = -1
    local NoRecordID = 30
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 30
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Omni-Antidote Prescription seems to be sleeping"
    local HasRecordNotice_1 = "Omni-Antidote Prescription seems to be sleeping"
    local NoRecordNotice = "Omni-Antidote Prescription seems to be sleeping"
    local NoRecordNotice_1 = "Omni-Antidote Prescription seems to be sleeping"
    local HasMissionNotice = "Omni-Antidote Prescription seems to be sleeping"
    local HasMissionNotice_1 = "Omni-Antidote Prescription seems to be sleeping"
    local NoMissionNotice = "Omni-Antidote Prescription seems to be sleeping"
    local NoMissionNotice_1 = "Omni-Antidote Prescription seems to be sleeping"
    local CheckBoatNotice = "Omni-Antidote Prescription seems to be sleeping"
    local CheckPosNotice = "Omni-Antidote Prescription seems to be sleeping"
    local GuildTypeNotice = "Omni-Antidote Prescription seems to be sleeping"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 30
    local GiveMisson_2 = 30
    local GiveMisson_0 = 30
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_LZL(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 24
    local NoRecordID_1 = -1
    local HasMissionID = -1
    local HasMissionID_1 = -1
    local No_MissionID = 24
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Tear of Dragon emanate a chilling aura"
    local HasRecordNotice_1 = "Tear of Dragon emanate a chilling aura"
    local NoRecordNotice = "Tear of Dragon emanate a chilling aura"
    local NoRecordNotice_1 = "Tear of Dragon emanate a chilling aura"
    local HasMissionNotice = "Tear of Dragon emanate a chilling aura"
    local HasMissionNotice_1 = "Tear of Dragon emanate a chilling aura"
    local NoMissionNotice = "Tear of Dragon emanate a chilling aura"
    local NoMissionNotice_1 = "Tear of Dragon emanate a chilling aura"
    local CheckBoatNotice = "Tear of Dragon emanate a chilling aura"
    local CheckPosNotice = "Tear of Dragon emanate a chilling aura"
    local GuildTypeNotice = "Tear of Dragon emanate a chilling aura"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 24
    local GiveMisson_2 = 24
    local GiveMisson_0 = 24
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_BLP(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = -1
    local NoRecordID_1 = -1
    local HasMissionID = 369
    local HasMissionID_1 = -1
    local No_MissionID = -1
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = 1
    local Cha_x_max = 380200
    local Cha_x_min = 379800
    local Cha_y_max = 55200
    local Cha_y_min = 54800
    local MapName = "darkblue"
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasRecordNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoRecordNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoRecordNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasMissionNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasMissionNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoMissionNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoMissionNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local CheckBoatNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local CheckPosNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local GuildTypeNotice = "The Holy Water gives out a purifying aura and cleanses all evil"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = -1
    local GiveMisson_2 = -1
    local GiveMisson_0 = -1
    local ItemID = 4257
    local ItemNum = 1
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 1

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_SS(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 26
    local NoRecordID_1 = -1
    local HasMissionID = 370
    local HasMissionID_1 = -1
    local No_MissionID = 26
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasRecordNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoRecordNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoRecordNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasMissionNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local HasMissionNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoMissionNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local NoMissionNotice_1 = "The Holy Water gives out a purifying aura and cleanses all evil"
    local CheckBoatNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local CheckPosNotice = "The Holy Water gives out a purifying aura and cleanses all evil"
    local GuildTypeNotice = "The Holy Water gives out a purifying aura and cleanses all evil"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 26
    local GiveMisson_2 = 26
    local GiveMisson_0 = 26
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_FHSDX(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 27
    local NoRecordID_1 = -1
    local HasMissionID = 362
    local HasMissionID_1 = -1
    local No_MissionID = 27
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "A normal pink colored letter"
    local HasRecordNotice_1 = "A normal pink colored letter"
    local NoRecordNotice = "A normal pink colored letter"
    local NoRecordNotice_1 = "A normal pink colored letter"
    local HasMissionNotice = "A normal pink colored letter"
    local HasMissionNotice_1 = "A normal pink colored letter"
    local NoMissionNotice = "A normal pink colored letter"
    local NoMissionNotice_1 = "A normal pink colored letter"
    local CheckBoatNotice = "A normal pink colored letter"
    local CheckPosNotice = "A normal pink colored letter"
    local GuildTypeNotice = "A normal pink colored letter"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 27
    local GiveMisson_2 = 27
    local GiveMisson_0 = 27
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_WYJ(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 28
    local NoRecordID_1 = -1
    local HasMissionID = 375
    local HasMissionID_1 = -1
    local No_MissionID = 28
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = -1
    local Cha_x_max = 0
    local Cha_x_min = 0
    local Cha_y_max = 0
    local Cha_y_min = 0
    local MapName = -1
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "Binoculars allows you to see places far away"
    local HasRecordNotice_1 = "Binoculars allows you to see places far away"
    local NoRecordNotice = "Binoculars allows you to see places far away"
    local NoRecordNotice_1 = "Binoculars allows you to see places far away"
    local HasMissionNotice = "Binoculars allows you to see places far away"
    local HasMissionNotice_1 = "Binoculars allows you to see places far away"
    local NoMissionNotice = "Binoculars allows you to see places far away"
    local NoMissionNotice_1 = "Binoculars allows you to see places far away"
    local CheckBoatNotice = "Binoculars allows you to see places far away"
    local CheckPosNotice = "Binoculars allows you to see places far away"
    local GuildTypeNotice = "Binoculars allows you to see places far away"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 28
    local GiveMisson_2 = 28
    local GiveMisson_0 = 28
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end

function ItemUse_LDADYW2(role)
    local HasRecordID = -1
    local HasRecordID_1 = -1
    local NoRecordID = 14
    local NoRecordID_1 = -1
    local HasMissionID = 273
    local HasMissionID_1 = -1
    local No_MissionID = 14
    local No_MissionID_1 = -1
    local ChaType = -1
    local Need_CheckPos = 1
    local Cha_x_max = 7900
    local Cha_x_min = 7500
    local Cha_y_max = 397300
    local Cha_y_min = 396900
    local MapName = "garner"
    local GuildType = -1
    local CheckType = 1

    local HasRecordNotice = "There are lots of scratches on Andrew's Memento"
    local HasRecordNotice_1 = "There are lots of scratches on Andrew's Memento"
    local NoRecordNotice = "There are lots of scratches on Andrew's Memento"
    local NoRecordNotice_1 = "There are lots of scratches on Andrew's Memento"
    local HasMissionNotice = "There are lots of scratches on Andrew's Memento"
    local HasMissionNotice_1 = "There are lots of scratches on Andrew's Memento"
    local NoMissionNotice = "There are lots of scratches on Andrew's Memento"
    local NoMissionNotice_1 = "There are lots of scratches on Andrew's Memento"
    local CheckBoatNotice = "There are lots of scratches on Andrew's Memento"
    local CheckPosNotice = "Memento of Andrew shows no responese"
    local GuildTypeNotice = "There are lots of scratches on Andrew's Memento"

    local CheckMissionMsg_1 =
        MissionMsgCheck(
        role,
        HasRecordID,
        NoRecordID,
        HasMissionID,
        No_MissionID,
        HasRecordNotice,
        NoRecordNotice,
        HasMissionNotice,
        NoMissionNotice
    )

    local CheckMissionMsg_2 =
        MissionMsgCheck(
        role,
        HasRecordID_1,
        NoRecordID_1,
        HasMissionID_1,
        No_MissionID_1,
        HasRecordNotice_1,
        NoRecordNotice_1,
        HasMissionNotice_1,
        NoMissionNotice_1
    )

    local CheckChaMsg =
        ChaMsgCheck(
        role,
        ChaType,
        Need_CheckPos,
        Cha_x_max,
        Cha_x_min,
        Cha_y_max,
        Cha_y_min,
        MapName,
        GuildType,
        CheckType,
        CheckBoatNotice,
        CheckPosNotice,
        GuildTypeNotice
    )

    if CheckMissionMsg_1 ~= 1 or CheckMissionMsg_2 ~= 1 or CheckChaMsg ~= 1 then
        UseItemFailed(role)
        return
    end

    local GiveMisson_1 = 14
    local GiveMisson_2 = 14
    local GiveMisson_0 = 14
    local ItemID = -1
    local ItemNum = 0
    local Give_Exp = -1
    local Give_Money = -1
    local DelItem = 0

    local Cha_GuildID = GetChaGuildID(role)

    local ChaGuildType_Get = -1
    if Cha_GuildID == 0 then
        ChaGuildType_Get = 0
    elseif Cha_GuildID > 0 and Cha_GuildID <= 100 then
        ChaGuildType_Get = 1
    elseif Cha_GuildID > 100 and Cha_GuildID <= 200 then
        ChaGuildType_Get = 2
    else
        SystemNotice(role, "Illegal Guild ID")
    end

    if ChaGuildType_Get == 1 then
        if GiveMisson_1 ~= -1 then
            GiveMission(role, GiveMisson_1)
        end
    end

    if ChaGuildType_Get == 2 then
        if GiveMisson_2 ~= -1 then
            GiveMission(role, GiveMisson_2)
        end
    end

    if ChaGuildType_Get == 0 then
        if GiveMisson_0 ~= -1 then
            GiveMission(role, GiveMisson_0)
        end
    end

    if ItemID ~= -1 then
        GiveItem(role, 0, ItemID, ItemNum, 0)
    end

    if Give_Money > 0 then
        AddMoney(role, 0, Give_Money)
    end

    if Give_Exp > 0 then
        local exp = Exp(role)
        if Lv(TurnToCha(role)) >= 80 then
            Give_Exp = math.floor(exp_dif / 50)
        end
        local exp_new = exp + Give_Exp

        SetCharaAttr(exp_new, role, ATTR_CEXP)
    end

    if DelItem == 0 then
        UseItemFailed(role)
        return
    end
end
