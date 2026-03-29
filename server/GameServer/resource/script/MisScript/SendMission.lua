print("--------------------------------------------------")
print("[**] Extra Files [**]")
print("-- [Loading] Send Mission")

function TestDefMission(id, name, misid, npcid, areaid)
    DefineMission(id, name, misid, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisReultTalk("Thank you for sending my parcel over!")

    MisResultCondition(HasRandMissionNpc, misid, npcid, areaid)
    MisResultCondition(NoRandNpcItemFlag, misid, npcid)

    MisResultAction(TakeRandNpcItem, misid, npcid)
end
