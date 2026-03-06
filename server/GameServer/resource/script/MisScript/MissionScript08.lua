print("-- [Loading] Mission Script [08]")
--*****************************************************************************************************************************--
--################################################## Missions #################################################################--
--*****************************************************************************************************************************--
DefineMission(6114, "The First Task", 1000)
MisBeginTalk("<t>Could you do me one more favor? Please send this letter to Shaitan <bChairman Guile> at <j(873, 3545)>.Hurry up. A higher experience is waiting for you!")
MisBeginCondition(HexatlonTime)
MisBeginCondition(LvCheck, "<", 76)
MisBeginCondition(NoMission, 1000)
MisBeginCondition(NoRecord, 1000)
MisBeginAction(GiveItem, 3292, 1, 4)
MisBeginAction(AddMission, 1000)
MisCancelAction(ClearMission, 1000)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"Please send this letter to Chairman Guile in the Shaitan City")
MisHelpTalk("<t><bChairman Guile> is at <j(873,3545)> in Shaitan City. Good Luck.")
MisResultCondition(AlwaysFailure)

--Part I, task II
DefineMission(6115, "The First Task", 1000, COMPLETE_SHOW)
MisBeginCondition(AlwaysFailure)
MisResultTalk("<t>A letter for me? Thanks.")
MisResultCondition(HasMission, 1000)
MisResultCondition(HasItem, 3292, 1)
MisResultCondition(NoRecord, 1000)
MisResultAction(TakeItem, 3292, 1)
MisResultAction(SetRecord, 1000)
MisResultAction(ClearMission, 1000)
MisResultAction(AddExp_1)

--Part II, task I
DefineMission(6116, "The Second Task", 1001)
MisBeginTalk("<t>The Nurse Gina of Argent City wants a tail grass. Buy Gina a tail grass from Physician Ditto.")
MisBeginCondition(HasRecord, 1000)
MisBeginCondition(NoMission, 1001)
MisBeginCondition(NoRecord, 1001)
MisBeginAction(AddMission, 1001)
MisBeginAction(AddTrigger, 10011, TE_GETITEM, 3143, 1)
MisCancelAction(ClearMission, 1001)

MisNeed(MIS_NEED_DESP,"Go and buy a tail grass from Physician Ditto for Nurse Gina.")
MisNeed(MIS_NEED_ITEM, 3910, 1, 10, 1)

MisHelpTalk("<t>Physician Ditto sells the tail grass.")
MisResultCondition(AlwaysFailure)

--Part II, task II
DefineMission(6117, "The Second Task", 1001, COMPLETE_SHOW)
MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Your tail grass comes at the right time. I need it right now!")
MisResultCondition(HasMission, 1001)
MisResultCondition(NoRecord, 1001)
MisResultCondition(HasItem, 3910, 1)
MisResultAction(TakeItem, 3910, 1)
MisResultAction(SetRecord, 1001)
MisResultAction(ClearMission, 1001)
MisResultAction(AddExp_1)

InitTrigger()
TriggerCondition(1, IsItem, 3910)	
TriggerAction(1, AddNextFlag, 1001, 10, 1)
RegCurTrigger(10011)

--Part III, task A
DefineMission( 6118, "The Third Task", 1002)
MisBeginTalk( "<t> Recently, the Grassland Elks brought us a lot of trouble. Could you give them a lesson for us?")
MisBeginCondition( HasRecord, 1001)
MisBeginCondition( NoMission, 1002)
MisBeginCondition( NoRecord, 1002)
MisBeginCondition( NoRecord, 1003)
MisBeginCondition( LvCheck, "<", 40)
MisBeginAction(AddMission, 1002)
MisBeginAction(AddTrigger, 10021, TE_KILL, 299, 20)
MisCancelAction(ClearMission, 1002)

MisNeed(MIS_NEED_DESP,"Kill 20 Grassland Elks in Argent at <j(1390, 2658)> and come back.")
MisNeed(MIS_NEED_KILL, 299, 20, 10, 20)

MisResultTalk("<t>Well done. You completed it so quickly. I will reward you something.")
MisHelpTalk("<t>You can find Grassland Elks around the Shepherd Plains at <j(1390, 2658)>.")
MisResultCondition(HasMission, 1002)
MisResultCondition(NoRecord, 1002)
MisResultCondition(HasFlag, 1002, 29)
MisResultAction(SetRecord, 1002)
MisResultAction(SetRecord, 1003)
MisResultAction(ClearMission, 1002)
MisResultAction(AddExp, 13000, 13000)

InitTrigger()
TriggerCondition(1, IsMonster, 299)	
TriggerAction(1, AddNextFlag, 1002, 10, 20)
RegCurTrigger(10021)

--Part III, task B
DefineMission( 6119, "The Third Task", 1004)
MisBeginTalk("<t>Recently, the Pumpkin Knights brought us a lot of trouble. Could you give them a lesson for us?")
MisBeginCondition(HasRecord, 1001)
MisBeginCondition(NoMission, 1004)
MisBeginCondition(NoMission, 1002)
MisBeginCondition(NoRecord, 1003)
MisBeginCondition(NoRecord, 1004)
MisBeginCondition(LvCheck, ">", 39)
MisBeginAction(AddMission, 1004)
MisBeginAction(AddTrigger, 10041, TE_KILL, 37, 20)
MisCancelAction(ClearMission, 1004)

MisNeed(MIS_NEED_DESP,"Kill 20 Pumpkin Knights within Silver Mine 2 and Silver Mine 3.")
MisNeed(MIS_NEED_KILL, 37, 20, 10, 20)

MisResultTalk("<t>Wow~ Well done. You completed it so quickly. I will reward you something.")
MisHelpTalk("<t>The Pumpkin Knights are within Silver Mine 2 and Silver Mine 3.")
MisResultCondition(HasMission, 1004)
MisResultCondition(NoRecord, 1004)
MisResultCondition(HasFlag, 1004, 29)
MisResultAction(SetRecord, 1004)
MisResultAction(SetRecord, 1003)
MisResultAction(ClearMission, 1004)
MisResultAction(AddExp_3)

InitTrigger()
TriggerCondition(1, IsMonster, 37)	
TriggerAction(1, AddNextFlag, 1004, 10, 20 )
RegCurTrigger(10041)

--Part IV, task I
DefineMission(6120, "The Fourth Task", 1005)
MisBeginTalk("<t> Now you are qualified to the fourth task. Collect 3 Wood and 3 Iron Ore and give them to <bGreg> at <j(1882, 2805)> in Abandon Mine Haven.")
MisBeginCondition(HasRecord, 1003)
MisBeginCondition(NoMission, 1005)
MisBeginCondition(NoRecord, 1005)
MisBeginAction(AddMission, 1005)
MisBeginAction(AddTrigger, 10051, TE_GETITEM, 4543, 3)
MisBeginAction(AddTrigger, 10052, TE_GETITEM, 4545, 3)
MisBeginAction(AddTrigger, 10053, TE_GETITEM, 1478, 3)
MisCancelAction(ClearMission, 1005)

MisNeed(MIS_NEED_DESP,"Send 3 Wood, 3 Sashimi and 3 Iron Ore to <bGreg> at <j(1882, 2805)> in Abandon Mine Haven.")
MisNeed(MIS_NEED_ITEM, 4543, 3, 10, 3)
MisNeed(MIS_NEED_ITEM, 4545, 3, 20, 3)
MisNeed(MIS_NEED_ITEM, 1478, 3, 20, 3)

MisHelpTalk("<t>You can get the Wood by cutting down trees, Sashimi by fishing and Iron Ore by mining.")

MisResultCondition(AlwaysFailure)

--Part IV, task II
DefineMission(6121, "The Fourth Task", 1005, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )
MisResultTalk("<t>Your Wood and  Iron Ore come just at the right time. I need them right now.")
MisResultCondition(HasMission, 1005)
MisResultCondition(NoRecord, 1005)
MisResultCondition(HasItem, 4543, 3)
MisResultCondition(HasItem, 4545, 3)
MisResultCondition(HasItem, 1478, 3)
MisResultAction(TakeItem, 4543, 3)
MisResultAction(TakeItem, 4545, 3)
MisResultAction(TakeItem, 1478, 3)
MisResultAction(SetRecord, 1005)
MisResultAction(ClearMission, 1005)
MisResultAction(AddExp_4)

InitTrigger()
TriggerCondition(1, IsItem, 4543)	
TriggerAction(1, AddNextFlag, 1005, 10, 3)
RegCurTrigger(10051)

InitTrigger()
TriggerCondition(1, IsItem, 4545)		
TriggerAction(1, AddNextFlag, 1005, 20, 3)
RegCurTrigger(10052)

InitTrigger()
TriggerCondition( 1, IsItem, 1478)	
TriggerAction(1, AddNextFlag, 1005, 20, 3)
RegCurTrigger(10053)

--Part V, task I
DefineMission(6122, "The Fifth Task", 1006)
MisBeginTalk("<t> The next task is collecting two pink pearls and give them to <bForbei> at <j(2226,2726)> in the Argent City.")
MisBeginCondition(HasRecord, 1005)
MisBeginCondition(NoMission, 1006)
MisBeginCondition(NoRecord, 1006)
MisBeginAction(AddMission, 1006)
MisBeginAction(AddTrigger, 10061, TE_GETITEM, 2588, 5)
MisCancelAction(ClearMission, 1006)

MisNeed(MIS_NEED_DESP,"Collect five Elven Signets and give them to <bForbei> at <j(2226,2726)> in the Argent City.")
MisNeed(MIS_NEED_ITEM, 2588, 5, 10, 5)

MisHelpTalk("<t>It seems that you can only get the Elven Signets from your Pet.")
MisResultCondition(AlwaysFailure)

--Part V, task II
DefineMission(6123, "The Fifth Task", 1006, COMPLETE_SHOW)
MisBeginCondition(AlwaysFailure)
MisResultTalk("<t> You are so strong.The pearls are so beautiful. Now you have the chance to do the final task. I believe you can do it.")
MisResultCondition(HasMission, 1006)
MisResultCondition(NoRecord, 1006)
MisResultCondition(HasItem, 2588, 5)
MisResultAction(TakeItem, 2588, 5)
MisResultAction(SetRecord, 1006)
MisResultAction(ClearMission, 1006)
MisResultAction(AddExp_5)

InitTrigger()
TriggerCondition( 1, IsItem, 2588)	
TriggerAction(1, AddNextFlag, 1006, 10, 5)
RegCurTrigger(10061)

--Part VI, task I
DefineMission(6124, "The Sixth Task", 1007)
MisBeginTalk("<t>The final task is very simple. All you need to do is just finding another two people as you companions. And on you team, you must have one person whose level is from 20 to 30, another person whose level is from 31 to 40 and the other whose level is higher than 40.When you make it, you can go to find Swordsman Ray in the Lower Icicle Castle to get your reward!")
MisBeginCondition(HasRecord, 1006)
MisBeginCondition(NoMission, 1007)
MisBeginCondition(NoRecord, 1007)
MisBeginAction(AddMission, 1007)
MisCancelAction(ClearMission, 1007)
MisHelpTalk("<t>Go to find your companion")
MisResultCondition(AlwaysFailure)

--Part VI, task II
DefineMission(6125, "The Sixth Task", 1007, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure)
MisResultTalk("<t>You are so great! congratulations! I will look forward to your performance next time.")
MisResultCondition(HasMission, 1007)
MisResultCondition(NoRecord, 1007)
MisResultCondition(HexaTeamCheck)
MisResultAction(SetRecord, 1007)
MisResultAction(ClearMission, 1007)
MisResultAction(AddExp_6)

-- Start of Belts quest
DefineMission(6890, "Capture the fearsome Wang Xiao Hu", 2019)
MisBeginTalk("<t>Wang Xiao Hu attemp to invade Dream Island! There are rumors that he usually sneak in every 8 hours a day to execute his plan. Defeat Wang Xiao Hu and I will honor you a reward!")
MisBeginCondition(NoMission, 2019)
MisBeginCondition(CheckWangTime)
MisBeginAction(AddMission, 2019)
MisCancelAction(ClearMission, 2019)
MisBeginAction(AddTrigger, 20191, TE_KILL, 1281, 1)
MisNeed(MIS_NEED_KILL, 1281, 1, 10, 1)

MisResultTalk("<t>Defeat Wang Xiao Hu, I will reward honor you a reward. If not, you can abandon the quest.")
MisHelpTalk("<t>Lets hurry before someone else do it!")
MisResultCondition(HasMission, 2019)
MisResultCondition(HasFlag, 2019, 10)
MisResultAction(GiveItem, 7363, 1, 4)
MisResultAction(ClearMission, 2019)
MisResultAction(AddMoney, 100000, 100000)

InitTrigger()
TriggerCondition(1, IsMonster, 1281)	
TriggerAction(1, AddNextFlag, 2019, 10, 1)
RegCurTrigger(20191)

-- Start of Bracelet and Handguard quest 
DefineMission(6942, "Donate for EXPO!", 2020)
MisBeginTalk("<t>Hi! You're new here? A lot of pirates coming here asking on how they can obtain bracelets. It takes a lot of patience! If you are determine to get your own then I'll lead you the way!")
MisBeginCondition(NoRecord, 2020)
MisBeginCondition(NoMission, 2020)
MisBeginAction(AddMission, 2020)
MisBeginAction(ClearRecord, 2021)
MisBeginAction(ClearRecord, 2022)
MisBeginAction(ClearRecord, 2023)
MisBeginAction(ClearRecord, 2024)
MisBeginAction(ClearRecord, 2025)
MisBeginAction(ClearRecord, 2026)
MisCancelAction(ClearMission, 2020)

MisResultTalk("<t>I didn't expect that from a newbie like you. Anyway, go to Hotel Owner - Dust firstly, he will tell you more on how to obtain bracelets.")
MisHelpTalk("<t>I usually ask for donations from our visitors on this island. If you are able to contribute 100,000G then I will tell you how to get your braclets!")
MisResultCondition(HasMission, 2020)
MisResultCondition(NoRecord, 2020)
MisResultAction(TakeMoney, 100000)
MisResultAction(SetRecord, 2020)
MisResultAction(ClearMission, 2020)

-- Part 1 of the Lv55 Bracelet Quest
DefineMission(6943, "Lv55 Bracelet Quest", 2021)
MisBeginTalk("<t>Hey, seems like you are pretty free. Help me out. If you do I'll give you a surprise. <n><t> Recently business is so good that I ran out of stock. Please help me collect the raw material: <r30 Orange Grass, 30 Rookie Boxeroo's Glove, 30 Snow Heart, 30 White Elven Fruit, 30 Black Elven Fruit>, this items can be found in <jDream Island(296,494)> <bDawn Owl>, <jArgent City(1056,2921)><bBoxing Kangeroo> ,<jIcicle(1403,632)>< bYeti>, <jRegion of Demon(587,112)> <bMutated White Bobcat>, <jRegion of Demon(587,112)> <bMutated Black Bobcat>. <n><t> Come <jDream Island(437,658)> find me after you have got it all")
MisBeginCondition(LvCheck, ">", 47)
MisBeginCondition(HasRecord, 2020)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(NoRecord, 2021)
MisBeginCondition(NoMission, 2021)
MisBeginAction(AddMission, 2021)
MisCancelAction(ClearMission, 2021)

MisBeginAction(AddTrigger, 20211, TE_GETITEM, 7330, 30) -- Dawn Owl
MisBeginAction(AddTrigger, 20212, TE_GETITEM, 7331, 30) -- Rookie Boxeroo
MisBeginAction(AddTrigger, 20213, TE_GETITEM, 7332, 30) -- Yeti
MisBeginAction(AddTrigger, 20214, TE_GETITEM, 7333, 30) -- Mutated White Bobcat
MisBeginAction(AddTrigger, 20215, TE_GETITEM, 7334, 30) -- Mutated Black Bobcat

MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7330, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7331, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7332, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7333, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7334, 30, 130, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")
MisResultBagNeed(1)

MisResultCondition(HasMission, 2021)
MisResultCondition(NoRecord, 2021)
MisResultCondition(HasItem, 7330, 30)
MisResultCondition(HasItem, 7331, 30)
MisResultCondition(HasItem, 7332, 30)
MisResultCondition(HasItem, 7333, 30)
MisResultCondition(HasItem, 7334, 30)
MisResultAction(TakeItem, 7330, 30)
MisResultAction(TakeItem, 7331, 30)
MisResultAction(TakeItem, 7332, 30)
MisResultAction(TakeItem, 7333, 30)
MisResultAction(TakeItem, 7334, 30)
MisResultAction(ClearMission, 2021)
MisResultAction(SetRecord, 2021)
MisResultAction(GiveXinZhuangBei, 2021)
MisResultAction(AddMoney, 200000)

InitTrigger()
TriggerCondition(1, IsItem, 7330)
TriggerAction(1, AddNextFlag, 2021, 10, 30)
RegCurTrigger(20211)

InitTrigger()
TriggerCondition(1, IsItem, 7331)
TriggerAction(1, AddNextFlag, 2021, 40, 30)
RegCurTrigger(20212)

InitTrigger()
TriggerCondition(1, IsItem, 7332)
TriggerAction(1, AddNextFlag, 2021, 70, 30)
RegCurTrigger(20213)

InitTrigger()
TriggerCondition(2, IsItem, 7333)
TriggerAction(2, AddNextFlag, 2021, 100, 30)
RegCurTrigger(20214)

InitTrigger()
TriggerCondition(2, IsItem, 7334)
TriggerAction(2, AddNextFlag, 2021, 130, 30)
RegCurTrigger(20215)	

-- Part 2 of the Lv55 Bracelet Quest
DefineMission(6944, "Lv55 Bracelet Quest", 2022)
MisBeginTalk("<t>Hey kid, I have always love to cook some special food. But now I'm old and can't get special ingredients. Can you help me? I'll reward you after.<n><t> Recently business is so good that I ran out of stock. Please help me collect the raw material: <r30 Ice Blue Grass, 30 Mad Boar Tooth, 30 Heart of Dexterity, 30 Fruit of Ice Dragon, 30 Gold Scorpion Venom>, this items can be found in <jRegion of Demon(587,112)> <bMutated White Bobcat>, <jArgent City(865,2917)><bMad Boar> ,<jIcicle(863,811)>< bFragile Snow Doll>, <jRegion of Demon(587,112)> <bBaby Ice Dragon>, <jRegion of Demon(587,112)> <bGold Scorpion King>. <n><t> Come <jDream Island(437,658)> find me after you have got it all")
MisBeginCondition(LvCheck, ">", 47)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(HasRecord, 2021)
MisBeginCondition(NoRecord, 2022)
MisBeginCondition(NoMission, 2022)
MisBeginAction(AddMission, 2022)
MisCancelAction(ClearMission, 2022)

MisBeginAction(AddTrigger,20221, TE_GETITEM, 7335, 30) -- Mutated White Bobcat -- Ice Blue Grass
MisBeginAction(AddTrigger,20222, TE_GETITEM, 7336, 30) -- Mad Boar             -- Mad Boar Tooth
MisBeginAction(AddTrigger,20223, TE_GETITEM, 7337, 30) -- Fragile Snow Doll    -- Heart of Dexterity	
MisBeginAction(AddTrigger,20224, TE_GETITEM, 7338, 30) -- Baby Ice Dragon      -- Fruit of Ice Dragon
MisBeginAction(AddTrigger,20225, TE_GETITEM, 7339, 30) -- Gold Scorpion King   -- Gold Scorpion Venom

MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7335, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7336, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7337, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7338, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7339, 30, 130, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")

MisResultCondition(HasMission, 2022)
MisResultCondition(NoRecord, 2022)
MisResultCondition(HasItem, 7335, 30)
MisResultCondition(HasItem, 7336, 30)
MisResultCondition(HasItem, 7337, 30)
MisResultCondition(HasItem, 7338, 30)
MisResultCondition(HasItem, 7339, 30)
MisResultAction(TakeItem, 7335, 30)
MisResultAction(TakeItem, 7336, 30)
MisResultAction(TakeItem, 7337, 30)
MisResultAction(TakeItem, 7338, 30)
MisResultAction(TakeItem, 7339, 30)
MisResultAction(ClearMission, 2022)
MisResultAction(SetRecord, 2022)
MisResultAction(GiveXinZhuangBei, 2022)
MisResultAction(AddMoney, 200000)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition(1, IsItem, 7335)
TriggerAction(1, AddNextFlag, 2022, 10, 30)
RegCurTrigger(20221)

InitTrigger()
TriggerCondition(1, IsItem, 7336)
TriggerAction(1, AddNextFlag, 2022, 40, 30)
RegCurTrigger(20222)

InitTrigger()
TriggerCondition(1, IsItem, 7337)
TriggerAction(1, AddNextFlag, 2022, 70, 30)
RegCurTrigger(20223)

InitTrigger()
TriggerCondition(2, IsItem, 7338)
TriggerAction(2, AddNextFlag, 2022, 100, 30)
RegCurTrigger(20224)

InitTrigger()
TriggerCondition(2, IsItem, 7339)
TriggerAction(2, AddNextFlag, 2022, 130, 30)
RegCurTrigger(20225)

-- Final part of the Lv55 Handguard Quest
DefineMission(6945, "Lv55 Handguard Quest", 2023)
MisBeginTalk("<t>Hey kid, when I'm as you as you, I am the greatest pirate of the sea. Now that I'm old, I can only rest here. I wanted to eat something but I can hunt for it. Can you help me? The items are: <r30 Bright Red Fruit, 30 Unknown Smuggled Goods, 30 Mini Pig Tail, 30 Black Bobcat's Beard, 30 Gold Barbed Tail>. They can be found at <jDream City(411,494)> <bMutated Black Bobcat>, <jArgent City(1570,2984)> <bSmuggler>, <jIcicle(1403,529)> <bCombat Piglet>, <jRegion of Demons(587,112)> <bMutated Black Bobcat>, <jRegion of Demons(628,282)> <Gold Scorpion King>. <n><t> Come <jDream City(390,652)> and look for me after you got everything.")
MisBeginCondition(LvCheck, ">",47)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(HasRecord, 2022)
MisBeginCondition(NoRecord, 2023)
MisBeginCondition(NoMission, 2023)
MisBeginAction(AddMission, 2023)
MisCancelAction(ClearMission, 2023)

MisBeginAction(AddTrigger,20231, TE_GETITEM, 7340, 30) -- Mutated Black Bobcat
MisBeginAction(AddTrigger,20232, TE_GETITEM, 7341, 30) -- Smuggler
MisBeginAction(AddTrigger,20233, TE_GETITEM, 7342, 30) -- Combat Piglet
MisBeginAction(AddTrigger,20234, TE_GETITEM, 7343, 30) -- Mutated Black Bobcat
MisBeginAction(AddTrigger,20235, TE_GETITEM, 7344, 30) -- Gold Scorpion King
											 
MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7340, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7341, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7342, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7343, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7344, 30, 130, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")

MisResultBagNeed(1)
MisResultCondition(HasMission, 2023)
MisResultCondition(NoRecord, 2023)
MisResultCondition(HasItem, 7340, 30)
MisResultCondition(HasItem, 7341, 30)
MisResultCondition(HasItem, 7342, 30)
MisResultCondition(HasItem, 7343, 30)
MisResultCondition(HasItem, 7344, 30)
MisResultAction(TakeItem, 7340, 30)
MisResultAction(TakeItem, 7341, 30)
MisResultAction(TakeItem, 7342, 30)
MisResultAction(TakeItem, 7343, 30)
MisResultAction(TakeItem, 7344, 30)
MisResultAction(ClearMission, 2023)
MisResultAction(SetRecord, 2023)
MisResultAction(GiveXinZhuangBei, 2023)
MisResultAction(AddMoney , 200000)

InitTrigger()
TriggerCondition(1, IsItem, 7340)
TriggerAction(1, AddNextFlag, 2023, 10, 30)
RegCurTrigger(20231)

InitTrigger()
TriggerCondition(1, IsItem, 7341)
TriggerAction(1, AddNextFlag, 2023, 40, 30)
RegCurTrigger(20232)

InitTrigger()
TriggerCondition(1, IsItem, 7342)
TriggerAction(1, AddNextFlag, 2023, 70, 30)
RegCurTrigger(20233)

InitTrigger()
TriggerCondition(2, IsItem, 7343)
TriggerAction(2, AddNextFlag, 2023, 100, 30)
RegCurTrigger(20234)

InitTrigger()
TriggerCondition(2, IsItem, 7344)
TriggerAction(2, AddNextFlag, 2023, 130, 30)
RegCurTrigger(20235)

-- Part 1 of the Lv65 Bracelet Quest
DefineMission(6946, "Lv65 Bracelet Quest", 2024)
MisBeginTalk("<t>I'm troubled, I cannot sleep. Doctors told me only a special medicine can cure it. Can you help me get the ingredients? The items are: <r30 Yellow Juice,30 White Hair, 30 Heart of Giants, 30 Heart of Demons, 30 Golden Shell, 30 Dawn Feather>. You can find them at <jDream City(320,667)> <bBerserk Demon Wolf>, <jArgent City(1261,2600)><bAngelic Panda>, <jIcicle(2522,410)> <bSnowman>, <jIcicle(2810,401)> <bCumbersome Yeti>, <jRegion of Demons(628,282)> <bBatman>, <jRegion of Demons(671,140)> <Dawn Owl>. <n><t> Come <jDream City(390,652)> and look for me after you got everything.")
MisBeginCondition(LvCheck, ">",59)
MisBeginCondition(HasRecord, 2020)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(NoRecord, 2024)
MisBeginCondition(NoMission, 2024)
MisBeginAction(AddMission, 2024)
MisCancelAction(ClearMission, 2024)

MisBeginAction(AddTrigger, 20241, TE_GETITEM, 7345, 30) -- Berserk Demon Wolf
MisBeginAction(AddTrigger, 20242, TE_GETITEM, 7346, 30) -- Angelic Panda
MisBeginAction(AddTrigger, 20243, TE_GETITEM, 7347, 30) -- Snowman
MisBeginAction(AddTrigger, 20244, TE_GETITEM, 7348, 30) -- Cumbersome Yeti
MisBeginAction(AddTrigger, 20245, TE_GETITEM, 7349, 30) -- Batman
MisBeginAction(AddTrigger, 20246, TE_GETITEM, 7350, 30) -- Dawn Owl

MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7345, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7346, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7347, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7348, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7349, 30, 130, 30)
MisNeed(MIS_NEED_ITEM, 7350, 30, 160, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")

MisResultBagNeed(1)
MisResultCondition(HasMission, 2024)
MisResultCondition(NoRecord, 2024)
MisResultCondition(HasItem, 7345, 30)
MisResultCondition(HasItem, 7346, 30)
MisResultCondition(HasItem, 7347, 30)
MisResultCondition(HasItem, 7348, 30)
MisResultCondition(HasItem, 7349, 30)
MisResultCondition(HasItem, 7350, 30)
MisResultAction(TakeItem, 7345, 30)
MisResultAction(TakeItem, 7346, 30)
MisResultAction(TakeItem, 7347, 30)
MisResultAction(TakeItem, 7348, 30)
MisResultAction(TakeItem, 7349, 30)
MisResultAction(TakeItem, 7350, 30)
MisResultAction(ClearMission, 2024)
MisResultAction(SetRecord, 2024)
MisResultAction(GiveXinZhuangBei, 2024)
MisResultAction(AddMoney , 200000)

InitTrigger()
TriggerCondition(1, IsItem, 7345)
TriggerAction(1, AddNextFlag, 2024, 10, 30)
RegCurTrigger(20241)

InitTrigger()
TriggerCondition(1, IsItem, 7346)
TriggerAction(1, AddNextFlag, 2024, 40, 30)
RegCurTrigger(20242)

InitTrigger()
TriggerCondition(1, IsItem, 7347)
TriggerAction(1, AddNextFlag, 2024, 70, 30)
RegCurTrigger(20243)

InitTrigger()
TriggerCondition(2, IsItem, 7348)
TriggerAction(2, AddNextFlag, 2024, 100, 30)
RegCurTrigger(20244)

InitTrigger()
TriggerCondition(2, IsItem, 7349)
TriggerAction(2, AddNextFlag, 2024, 130, 30)
RegCurTrigger(20245)

InitTrigger()
TriggerCondition(2, IsItem, 7350)
TriggerAction(2, AddNextFlag, 2024, 160, 30)
RegCurTrigger(20246)

-- Part 2 of the Lv65 Bracelet Quest
DefineMission(6947, "Lv65 Bracelet Quest", 2025)
MisBeginTalk("<t>Tell you a secret, I love to collect funny things. But as  lord I can't go out like others. Maybe you can help me collect? The items are: <r30 Blue Grass, 30 Unknown Fruit, 30 Broken Bow, 30 Heart of Slow, 30 Dawn Beak, 30 Shimizu Pig Toys>, they can be found at <jDream City(320,667)> <bShimizu Pig>, <jArgent City(1261,2600)> <bOwlie>, <jIcicle(2522,410)> <bElite Skeletal Archer>, <jIcicle(2810,401)> <bCumbersome Snowman>, <jForgotten Island(628,282)> <bDawn Owl>, <jRegion of Demons(671,140)> <bShimizu Pig>. <n><t> Come <jDream City(390,652)> and look for me after you got everything.")
MisBeginCondition(LvCheck, ">",59)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(HasRecord, 2024)
MisBeginCondition(NoRecord, 2025)
MisBeginCondition(NoMission, 2025)
MisBeginAction(AddMission, 2025)
MisCancelAction(ClearMission, 2025)

MisBeginAction(AddTrigger,20251, TE_GETITEM, 7351, 30) -- Shimizu Pig
MisBeginAction(AddTrigger,20252, TE_GETITEM, 7352, 30) -- Owlie
MisBeginAction(AddTrigger,20253, TE_GETITEM, 7353, 30) -- Elite Skeletal Archer
MisBeginAction(AddTrigger,20254, TE_GETITEM, 7354, 30) -- Cumbersome Snowman
MisBeginAction(AddTrigger,20255, TE_GETITEM, 7355, 30) -- Dawn Owl
MisBeginAction(AddTrigger,20256, TE_GETITEM, 7356, 30) -- Shimizu Pig

MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7351, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7352, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7353, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7354, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7355, 30, 130, 30)
MisNeed(MIS_NEED_ITEM, 7356, 30, 160, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")

MisResultBagNeed(1)
MisResultCondition(HasMission, 2025)
MisResultCondition(NoRecord, 2025)
MisResultCondition(HasItem, 7351, 30)
MisResultCondition(HasItem, 7352, 30)
MisResultCondition(HasItem, 7353, 30)
MisResultCondition(HasItem, 7354, 30)
MisResultCondition(HasItem, 7355, 30)
MisResultCondition(HasItem, 7356, 30)
MisResultAction(TakeItem, 7351, 30)
MisResultAction(TakeItem, 7352, 30)
MisResultAction(TakeItem, 7353, 30)
MisResultAction(TakeItem, 7354, 30)
MisResultAction(TakeItem, 7355, 30)
MisResultAction(TakeItem, 7356, 30)
MisResultAction(ClearMission, 2025)
MisResultAction(SetRecord, 2025)
MisResultAction(GiveXinZhuangBei, 2025)
MisResultAction(AddMoney , 200000)

InitTrigger()
TriggerCondition(1, IsItem, 7351)
TriggerAction(1, AddNextFlag, 2025, 10, 30)
RegCurTrigger(20251)

InitTrigger()
TriggerCondition(1, IsItem, 7352)
TriggerAction(1, AddNextFlag, 2025, 40, 30)
RegCurTrigger(20252)

InitTrigger()
TriggerCondition(1, IsItem, 7353)
TriggerAction(1, AddNextFlag, 2025, 70, 30)
RegCurTrigger(20253)

InitTrigger()
TriggerCondition(2, IsItem, 7354)
TriggerAction(2, AddNextFlag, 2025, 100, 30)
RegCurTrigger(20254)

InitTrigger()
TriggerCondition(2, IsItem, 7355)
TriggerAction(2, AddNextFlag, 2025, 130, 30)
RegCurTrigger(20255)

InitTrigger()
TriggerCondition(2, IsItem, 7356)
TriggerAction(2, AddNextFlag, 2025, 160, 30)
RegCurTrigger(20256)

-- Final part of the Lv65 Handguard Quest
DefineMission(6948, "Lv65 Handguard Quest", 2026)
MisBeginTalk("<t>You are here means you have help a lot of people. Maybe you can also help me collect these materials? The items are: <r30 Blue Grass, 30 Unknown Fruit, 30 Broken Bow, 30 Heart of Slow, 30 Dawn Beak, 30 Shimizu Pig Toys>, they can be found at <jDream City(320,667)> <bShimizu Pig>, <jArgent City(1261,2600)> <bOwlie>, <jIcicle(2522,410)> <bElite Skeletal Archer>, <jIcicle(2810,401)> <bCumbersome Snowman>, <jForgotten Island(628,282)> <bDawn Owl>, <jRegion of Demons(671,140)> <bShimizu Pig>. <n><t> Come <jDream City(390,652)> and look for me after you got everything.")
MisBeginCondition(LvCheck, ">", 59)
MisBeginCondition(CheckSecondAdv)
MisBeginCondition(HasRecord, 2025)
MisBeginCondition(NoRecord, 2026)
MisBeginCondition(NoMission, 2026)
MisBeginAction(AddMission, 2026)
MisCancelAction(ClearMission, 2026)

MisBeginAction(AddTrigger, 20261, TE_GETITEM, 7357, 30) -- Llama -- Llama's Stamp
MisBeginAction(AddTrigger, 20262, TE_GETITEM, 7358, 30) -- Mud Monster -- Smelly Mud
MisBeginAction(AddTrigger, 20263, TE_GETITEM, 7359, 30) -- Cumbersome Yeti -- Fragments of Slow
MisBeginAction(AddTrigger, 20264, TE_GETITEM, 7360, 30) -- Horrific Snowman -- Heart of Terror
MisBeginAction(AddTrigger, 20265, TE_GETITEM, 7361, 30) -- Shimizu Pig -- Shimizu Pig
MisBeginAction(AddTrigger, 20266, TE_GETITEM, 7362, 30) -- Berserk Demon Wolf -- Demon Wolf Fang

MisNeed(MIS_NEED_DESP,"Can you collect <rthe following items>? You can find them in <pForgotten Island> and <pRegion of Demons>.")
MisNeed(MIS_NEED_ITEM, 7357, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 7358, 30, 40, 30)
MisNeed(MIS_NEED_ITEM, 7359, 30, 70, 30)
MisNeed(MIS_NEED_ITEM, 7360, 30, 100, 30)
MisNeed(MIS_NEED_ITEM, 7361, 30, 130, 30)
MisNeed(MIS_NEED_ITEM, 7362, 30, 160, 30)

MisResultTalk("<t>Thank you for your help! As we have agreed, you will get what you deserved.")
MisHelpTalk("<t>Come back soon, I won't ill-treat you with the rewards")

MisResultBagNeed(1)
MisResultCondition(HasMission, 2026)
MisResultCondition(NoRecord, 2026)
MisResultCondition(HasItem, 7357, 30)
MisResultCondition(HasItem, 7358, 30)
MisResultCondition(HasItem, 7359, 30)
MisResultCondition(HasItem, 7360, 30)
MisResultCondition(HasItem, 7361, 30)
MisResultCondition(HasItem, 7362, 30)
MisResultAction(TakeItem, 7357, 30)
MisResultAction(TakeItem, 7358, 30)
MisResultAction(TakeItem, 7359, 30)
MisResultAction(TakeItem, 7361, 30)
MisResultAction(TakeItem, 7362, 30)
MisResultAction(TakeItem, 7360, 30)
MisResultAction(ClearMission, 2026)
MisResultAction(SetRecord, 2026)
MisResultAction(GiveXinZhuangBei, 2026)
MisResultAction(AddMoney , 200000)

InitTrigger()
TriggerCondition(1, IsItem, 7357)
TriggerAction(1, AddNextFlag, 2026, 10, 30)
RegCurTrigger(20261)

InitTrigger()
TriggerCondition(1, IsItem, 7358)
TriggerAction(1, AddNextFlag, 2026, 40, 30)
RegCurTrigger(20262)

InitTrigger()
TriggerCondition(1, IsItem, 7359)
TriggerAction(1, AddNextFlag, 2026, 70, 30)
RegCurTrigger(20263)

InitTrigger()
TriggerCondition(2, IsItem, 7360)
TriggerAction(2, AddNextFlag, 2026, 100, 30)
RegCurTrigger(20264)

InitTrigger()
TriggerCondition(2, IsItem, 7361)
TriggerAction(2, AddNextFlag, 2026, 130, 30)
RegCurTrigger(20265)

InitTrigger()
TriggerCondition(2, IsItem, 7362)
TriggerAction(2, AddNextFlag, 2026, 160, 30)
RegCurTrigger(20266)
