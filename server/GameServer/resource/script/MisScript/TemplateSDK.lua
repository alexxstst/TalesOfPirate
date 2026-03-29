print("-- [Loading] TemplateSDK")

function MisKillMonster(Talk, Param, Prize)
    DefineMission(Param.CurMissionID, Param.MissionName, Param.CurMissionID)

    MisBeginTalk(Talk.Begin)
    MisBeginCondition(NoMission, Param.CurMissionID)
    MisBeginCondition(NoRecord, Param.CurMissionID)

    if Param.NeedRecord ~= nil then
        MisBeginCondition(HasRecord, Param.NeedRecord)
    end

    MisBeginCondition(LvCheck, ">", Param.MissionMinLv)
    MisBeginCondition(LvCheck, "<", Param.MissionMaxLv)

    MisBeginAction(AddMission, Param.CurMissionID)

    MisBeginAction(AddTrigger, Param.CurTriggerID1, TE_KILL, Param.Kill_Number1, Param.CurMonsterID1)
    MisNeed(MIS_NEED_KILL, Param.CurMonsterID1, Param.Kill_Number1)

    if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
        MisBeginAction(AddTrigger, Param.CurTriggerID2, TE_KILL, Param.Kill_Number2, Param.CurMonsterID2)
        MisNeed(MIS_NEED_KILL, Param.CurMonsterID2, Param.Kill_Number2)
    end
    if Param.CurMonsterID3 ~= nil and Param.CurTriggerID3 ~= nil and Param.Kill_Number3 ~= nil then
        MisBeginAction(AddTrigger, Param.CurTriggerID3, TE_KILL, Param.Kill_Number3, Param.CurMonsterID3)
        MisNeed(MIS_NEED_KILL, Param.CurMonsterID3, Param.Kill_Number3)
    end
    if Param.CurMonsterID4 ~= nil and Param.CurTriggerID4 ~= nil and Param.Kill_Number4 ~= nil then
        MisBeginAction(AddTrigger, Param.CurTriggerID4, TE_KILL, Param.Kill_Number4, Param.CurMonsterID4)
        MisNeed(MIS_NEED_KILL, Param.CurMonsterID4, Param.Kill_Number4)
    end

    if Prize.Mode == nil then
        MisPrizeSelOne()
    else
        MisPrizeSelAll()
    end

    if Prize.Select1 ~= nil and Prize.ID1 ~= nil and Prize.Num1 ~= nil then
        MisPrize(Prize.Select1, Prize.ID1, Prize.Num1)
    end
    if Prize.Select2 ~= nil and Prize.ID2 ~= nil and Prize.Num2 ~= nil then
        MisPrize(Prize.Select2, Prize.ID2, Prize.Num2)
    end
    if Prize.Select3 ~= nil and Prize.ID3 ~= nil and Prize.Num3 ~= nil then
        MisPrize(Prize.Select3, Prize.ID3, Prize.Num3)
    end
    if Prize.Select4 ~= nil and Prize.ID4 ~= nil and Prize.Num4 ~= nil then
        MisPrize(Prize.Select4, Prize.ID4, Prize.Num4)
    end
    if Prize.Select5 ~= nil and Prize.ID5 ~= nil and Prize.Num5 ~= nil then
        MisPrize(Prize.Select5, Prize.ID5, Prize.Num5)
    end
    if Prize.Select6 ~= nil and Prize.ID6 ~= nil and Prize.Num6 ~= nil then
        MisPrize(Prize.Select6, Prize.ID6, Prize.Num6)
    end
    if Prize.Select7 ~= nil and Prize.ID7 ~= nil and Prize.Num7 ~= nil then
        MisPrize(Prize.Select7, Prize.ID7, Prize.Num7)
    end
    if Prize.Select8 ~= nil and Prize.ID8 ~= nil and Prize.Num8 ~= nil then
        MisPrize(Prize.Select8, Prize.ID8, Prize.Num8)
    end

    MisResultTalk(Talk.Result)
    MisHelpTalk(Talk.Help)

    MisResultCondition(HasMission, Param.CurMissionID)
    MisResultCondition(NoRecord, Param.CurMissionID)
    if Param.NeedRecord ~= nil then
        MisResultCondition(HasRecord, Param.NeedRecord)
    end
    MisResultCondition(HasFlag, Param.CurMissionID, 79 + Param.Kill_Number1)
    if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
        MisResultCondition(HasFlag, Param.CurMissionID, 99 + Param.Kill_Number2)
    end

    MisResultAction(AddExp, Param.MaxAddExp, Param.MinAddExp)
    MisResultAction(ClearMission, Param.CurMissionID)
    if Param.SetMisRecord ~= nil then
        MisResultAction(SetRecord, Param.CurMissionID)
    end

    InitTrigger()
    TriggerCondition(1, IsMonster, Param.CurMonsterID1)
    TriggerAction(1, AddNextFlag, Param.CurMissionID, 80, Param.Kill_Number1)
    RegCurTrigger(Param.CurTriggerID1)

    if Param.CurMonsterID2 ~= nil and Param.CurTriggerID2 ~= nil and Param.Kill_Number2 ~= nil then
        InitTrigger()
        TriggerCondition(1, IsMonster, Param.CurMonsterID2)
        TriggerAction(1, AddNextFlag, Param.CurMissionID, 100, Param.Kill_Number2)
        RegCurTrigger(Param.CurTriggerID2)
    end
end

function DefSendItemMission(id, name, misid, npcid, areaid, sid)
    DefineMission(id, name, misid, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("Thank you for sending my parcel over!")

    MisResultCondition(HasRandMissionNpc, misid, npcid, areaid)
    MisResultCondition(NoRandNpcItemFlag, misid, npcid)

    MisResultAction(TakeRandNpcItem, misid, npcid, GetNpcName(npcid))
    MisResultAction(RefreshCompleteFlag, sid)
end

function ConvertProfession()
    AddPfTable(1, 8)
    AddPfTable(1, 9)
    AddPfTable(1, 10)

    AddPfTable(2, 11)
    AddPfTable(2, 12)

    AddPfTable(3, 15)

    AddPfTable(4, 16)

    AddPfTable(5, 13)
    AddPfTable(5, 14)

    AddPfTable(6, 18)

    AddPfTable(7, 17)

    AddCatTable(1, 1)
    AddCatTable(1, 2)
    AddCatTable(1, 4)
    AddCatTable(1, 7)
    AddCatTable(1, 9)
    AddCatTable(1, 10)
    AddCatTable(1, 11)
    AddCatTable(1, 12)
    AddCatTable(1, 16)
    AddCatTable(1, 17)

    AddCatTable(2, 1)
    AddCatTable(2, 3)
    AddCatTable(2, 6)
    AddCatTable(2, 7)
    AddCatTable(2, 8)
    AddCatTable(2, 10)
    AddCatTable(2, 17)

    AddCatTable(3, 2)
    AddCatTable(3, 3)
    AddCatTable(3, 4)
    AddCatTable(3, 5)
    AddCatTable(3, 7)
    AddCatTable(3, 11)
    AddCatTable(3, 12)
    AddCatTable(3, 13)
    AddCatTable(3, 14)
    AddCatTable(3, 15)
    AddCatTable(3, 16)
    AddCatTable(3, 17)

    AddCatTable(4, 4)
    AddCatTable(4, 5)
    AddCatTable(4, 6)
    AddCatTable(4, 7)
    AddCatTable(4, 13)
    AddCatTable(4, 14)
    AddCatTable(4, 16)
    AddCatTable(4, 17)
    AddCatTable(4, 18)
end
ConvertProfession()
