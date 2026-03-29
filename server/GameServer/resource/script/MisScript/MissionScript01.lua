print("--------------------------------------------------")
print("[**] Mission Files [**]")
print("-- [Loading] Mission Script [01]")

function RobinMission003()
    DefineMission(707, "Physician's Greetings", 703, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>You say you\'re new around here? Welcome! Take these <rApples>, they\'re on the house! I\'m sure they\'ll come in handy to you.<n><t>Now return to Senna at (2223, 2785).<n><t>(Ditto has given you some "Apples". Drag to F1 - F12 hotkey slot to use as a shortcut.)')
    MisResultCondition(NoRecord, 703)
    MisResultCondition(HasMission, 703)
    MisResultCondition(NoFlag, 703, 10)
    MisResultCondition(HasFlag, 703, 1)
    MisResultCondition(HasItem, 3952, 1)
    MisResultAction(TakeItem, 3952, 1)
    MisResultAction(SetFlag, 703, 10)
    MisResultAction(GiveItem, 1847, 10, 4)
    MisResultBagNeed(1)

    DefineMission(733, "Leaves Collection", 721)

    MisBeginTalk("<t>Hi! You will need to help me collect some herbs. I need a few types of leaves. They are 10 <rShriveled Leaf> and 5 <rGreen Leaf> and can be found on Mystic Shrub and Mystic Flower.")
    MisBeginCondition(NoMission, 721)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 721)
    MisBeginAction(SetFlag, 721, 1)
    MisBeginAction(AddTrigger, 7211, TE_GETITEM, 1573, 10)
    MisBeginAction(AddTrigger, 7212, TE_GETITEM, 1574, 3)
    MisCancelAction(ClearMission, 721)

    MisNeed(MIS_NEED_ITEM, 1573, 10, 10, 10)
    MisNeed(MIS_NEED_ITEM, 1574, 3, 20, 3)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great! I can continue my herbal research once again!")
    MisHelpTalk("<t>Have you not collected all the leaves?")
    MisResultCondition(HasMission, 721)
    MisResultCondition(HasItem, 1573, 10)
    MisResultCondition(HasItem, 1574, 3)
    MisResultAction(TakeItem, 1573, 10)
    MisResultAction(TakeItem, 1574, 3)
    MisResultAction(AddExp, 40, 70)
    MisResultAction(ClearMission, 721)

    InitTrigger()
    TriggerCondition(1, IsItem, 1573)
    TriggerAction(1, AddNextFlag, 721, 10, 10)
    RegCurTrigger(7211)
    InitTrigger()
    TriggerCondition(1, IsItem, 1574)
    TriggerAction(1, AddNextFlag, 721, 20, 3)
    RegCurTrigger(7212)

    DefineMission(738, "Decoction Recipe", 726)

    MisBeginTalk("<t>I have got an inspiration to make a new kind of medicine. Sorry to trouble you but could you help me to collect 2 <rGlass>, 2 <rFlower Bud> and 5 <rOctopus Ink>. You can get them from Forest Spirit, Mystic Flower and Octopus. I will reward you. Go now!")
    MisBeginCondition(NoMission, 726)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 726)
    MisBeginAction(SetFlag, 726, 1)
    MisBeginAction(AddTrigger, 7261, TE_GETITEM, 1777, 2)
    MisBeginAction(AddTrigger, 7262, TE_GETITEM, 1579, 2)
    MisBeginAction(AddTrigger, 7263, TE_GETITEM, 1705, 5)
    MisCancelAction(ClearMission, 726)

    MisNeed(MIS_NEED_ITEM, 1777, 2, 10, 2)
    MisNeed(MIS_NEED_ITEM, 1579, 2, 20, 2)
    MisNeed(MIS_NEED_ITEM, 1705, 5, 30, 5)

    MisPrize(MIS_PRIZE_MONEY, 200, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Very well. Since you have collected the nessaccery items, I can begin my research for a new potion.")
    MisHelpTalk("<t>What? Hurry up and collect those ingredients before I lose my inspiration!")
    MisResultCondition(HasMission, 726)
    MisResultCondition(HasItem, 1777, 2)
    MisResultCondition(HasItem, 1579, 2)
    MisResultCondition(HasItem, 1705, 5)
    MisResultAction(TakeItem, 1777, 2)
    MisResultAction(TakeItem, 1579, 2)
    MisResultAction(TakeItem, 1705, 5)
    MisResultAction(AddExp, 70, 95)
    MisResultAction(ClearMission, 726)

    InitTrigger()
    TriggerCondition(1, IsItem, 1777)
    TriggerAction(1, AddNextFlag, 726, 10, 2)
    RegCurTrigger(7261)
    InitTrigger()
    TriggerCondition(1, IsItem, 1579)
    TriggerAction(1, AddNextFlag, 726, 20, 2)
    RegCurTrigger(7262)
    InitTrigger()
    TriggerCondition(1, IsItem, 1705)
    TriggerAction(1, AddNextFlag, 726, 30, 5)
    RegCurTrigger(7263)

    DefineMission(739, "Mushroom Mushroom", 727)

    MisBeginTalk("<t>Speaking about it, in the past, I used to raise several <rGreedy Shrooms> outside Argent City. Recently I have been busy experimenting with the recipe so I totally forgotten all about them, seems to me now is a good time to put them to good use. Can you please retrieve 10 <rPoison Mushroom> and 5 <rMushroom>?<n><t>Err...Speaking about it, these Greedy Shroom are quite aggressive so its better to bring some healing items just in case. Other than that, these Greedy Shrooms can be found at (2220, 2564). Have a look, it might be the best time for harvest, everything is in your hands now!")
    MisBeginCondition(NoMission, 727)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 727)
    MisBeginAction(SetFlag, 727, 1)
    MisBeginAction(AddTrigger, 7271, TE_GETITEM, 3118, 5)
    MisBeginAction(AddTrigger, 7272, TE_GETITEM, 1725, 10)
    MisCancelAction(ClearMission, 727)

    MisNeed(MIS_NEED_ITEM, 3118, 5, 10, 5)
    MisNeed(MIS_NEED_ITEM, 1725, 10, 20, 10)

    MisPrize(MIS_PRIZE_MONEY, 300, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great! You have collected all the stuff! Thanks!")
    MisHelpTalk("<t>What's the matter? Did the Greedy Shrooms frighten you?")
    MisResultCondition(HasMission, 727)
    MisResultCondition(HasItem, 3118, 5)
    MisResultCondition(HasItem, 1725, 10)
    MisResultAction(TakeItem, 3118, 5)
    MisResultAction(TakeItem, 1725, 10)
    MisResultAction(AddExp, 95, 125)
    MisResultAction(ClearMission, 727)

    InitTrigger()
    TriggerCondition(1, IsItem, 3118)
    TriggerAction(1, AddNextFlag, 727, 10, 5)
    RegCurTrigger(7271)
    InitTrigger()
    TriggerCondition(1, IsItem, 1725)
    TriggerAction(1, AddNextFlag, 727, 20, 10)
    RegCurTrigger(7272)
end
RobinMission003()

function RobinMission004()
    DefineMission(748, "Survival Compass", 736)

    MisBeginTalk("<t>Are you here to become an Explorer? Explorer often encounter many dangers and have to persevere on their own. Without any proper knowledge, you will not surive in this harsh world as an Explorer. To obtain the <rSurvival Compass>, you will need to go to the <pOutskirt of Argent City>, defeat 5 <bMarsh Spirits> (North of Mine), 10 <bSea Snails> (Seaside) and collect 5 <bTortoise Blood> (Grass Tortoise).<n><t>If you can complete these task, I will give you the <bSurvival Compass>.<n><t>When you have reached level 10, you can come back to become a full fledge Explorer.")
    MisBeginCondition(NoMission, 736)
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CheckConvertProfession, MIS_RISKER)
    MisBeginAction(AddMission, 736)
    MisBeginAction(SetFlag, 736, 1)
    MisBeginAction(AddTrigger, 7361, TE_KILL, 104, 5)
    MisBeginAction(AddTrigger, 7362, TE_KILL, 39, 10)
    MisBeginAction(AddTrigger, 7363, TE_GETITEM, 1844, 5)
    MisCancelAction(ClearMission, 736)

    MisNeed(MIS_NEED_KILL, 104, 5, 10, 5)
    MisNeed(MIS_NEED_KILL, 39, 10, 20, 10)
    MisNeed(MIS_NEED_ITEM, 1844, 5, 30, 5)

    MisPrize(MIS_PRIZE_ITEM, 3962, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Quite a nice performance I must say!<n><t>You have passed the test I had set. This <rSurvival Compass> is what all new Explorer requires. <n><t>Keep it safe and come back to me when you are <pLv10>.")
    MisHelpTalk("<t>You have reached the requirement to complete this trial. It is not that easy to obtain the Survival Manual.")
    MisResultCondition(HasMission, 736)
    MisResultCondition(HasFlag, 736, 14)
    MisResultCondition(HasFlag, 736, 29)
    MisResultCondition(HasItem, 1844, 5)
    MisResultAction(TakeItem, 1844, 5)
    MisResultAction(ClearMission, 736)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 104)
    TriggerAction(1, AddNextFlag, 736, 10, 5)
    RegCurTrigger(7361)

    InitTrigger()
    TriggerCondition(1, IsMonster, 39)
    TriggerAction(1, AddNextFlag, 736, 20, 10)
    RegCurTrigger(7362)

    InitTrigger()
    TriggerCondition(1, IsItem, 1844)
    TriggerAction(1, AddNextFlag, 736, 30, 5)
    RegCurTrigger(7363)

    DefineMission(758, "Journey of the Voyager", 742)

    MisBeginTalk("<t>Since you chosen to be a <bVoyager>, you must be prepared for what is to come.<n><t>I have befriend some sailors when I was young. Now, they are in charge of harbors in some part of the world.<n><t>Take my recommendation letter to them and they will know what to do. <n><t>They are <pZephyr Isle>'s <b Burgess>, <pGlacier Isle>'s <b Sacenis>, <pOutlaw Isle>'s <b Dilady> and <pSara Haven>'s <b Whitney>.")
    MisBeginCondition(NoRecord, 742)
    MisBeginCondition(NoMission, 742)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 4)
    MisBeginCondition(CheckConvertProfession, MIS_VOYAGE)
    MisBeginAction(AddMission, 742)
    MisCancelAction(ClearMission, 742)

    MisNeed(MIS_NEED_DESP,"Burgess at <j(3254,3278)> in Zephyr Isle")
    MisNeed(MIS_NEED_DESP,"Sacenis at <j(2279,1112)> in Glacier Isle")
    MisNeed(MIS_NEED_DESP,"Dilady at <j(3595,739)> in Outlaw Isle")
    MisNeed(MIS_NEED_DESP,"Whitney at <j(3097,3530)> in Sara Haven")
    MisNeed(MIS_NEED_DESP,"Come back and talk to me in Argent City <j(2193,2730)> once you've spoken to the sailors.")

    MisResultTalk("<t>Oh you are back! You are now a full fledged <bVoyager>.<n><t>Sail the sea and make your legends today!")
    MisHelpTalk("<t>Go now! Have you not hear the call of the ocean?")
    MisResultCondition(HasMission, 742)
    MisResultCondition(HasFlag, 742, 20)
    MisResultCondition(HasFlag, 742, 30)
    MisResultCondition(HasFlag, 742, 40)
    MisResultCondition(HasFlag, 742, 50)
    MisResultAction(ClearMission, 742)
    MisResultAction(SetRecord, 742)
    MisResultAction(SetProfession, 16)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    DefineMission(760, "Journey of the Voyager", 742, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Hoho! You wish to become a Voyager?  Then you must work harder! It will be a promising future for you if you become a Voyager!")
    MisResultCondition(NoRecord, 742)
    MisResultCondition(HasMission, 742)
    MisResultCondition(NoFlag, 742, 20)
    MisResultAction(SetFlag, 742, 20)

    DefineMission(761, "Journey of the Voyager", 742, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Damn Little Daniel! Only thought of me after so long!")
    MisResultCondition(NoRecord, 742)
    MisResultCondition(HasMission, 742)
    MisResultCondition(NoFlag, 742, 30)
    MisResultAction(SetFlag, 742, 30)

    DefineMission(762, "Journey of the Voyager", 742, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, a new Voyager, welcome!<n><t>Remember to help me remind Little Daniel that he owe me 50000G...for 5 years already...")
    MisResultCondition(NoRecord, 742)
    MisResultCondition(HasMission, 742)
    MisResultCondition(NoFlag, 742, 40)
    MisResultAction(SetFlag, 742, 40)

    DefineMission(763, "Journey of the Voyager", 742, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Sailing the sea is perilous. Talk to me if you have any needs. Little Daniel used to help me a lot in the past.")
    MisResultCondition(NoRecord, 742)
    MisResultCondition(HasMission, 742)
    MisResultCondition(NoFlag, 742, 50)
    MisResultAction(SetFlag, 742, 50)
end
RobinMission004()

function RobinMission007()
    DefineMission(703, "Blacksmith's Greetings", 701, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>You\'re new here? People call me <bGoldie>, I\'m the Blacksmith of <pArgent City>. I mainly forge swords but I do make quite a bit of sales off other weapons as well. I\'m really busy these days but I\'ll make an exception to help you out since you have beautiful Senna\'s recommendation.<n><t>Don\'t forget to return to tell <bSenna> at <j(2223, 2785)> that I\'m doing all this for her sake, haha!<n><t>(Goldie has given you a pair of "Newbie Knife". Open your inventory and double click on it to equip.)')
    MisResultCondition(NoRecord, 701)
    MisResultCondition(HasMission, 701)
    MisResultCondition(NoFlag, 701, 10)
    MisResultCondition(HasItem, 3950, 1)
    MisResultAction(TakeItem, 3950, 1)
    MisResultAction(SetFlag, 701, 10)
    MisResultBagNeed(1)
end
RobinMission007()

function RobinMission017()
    DefineMission(719, "Righteous Document", 711)

    MisBeginTalk("<t>Why do you want to the <rRighteous Document>? Recently, many have tried to be a herbalist just to earn money. You have to prove to me whether you can meet the qualifications of being one.<n><t>Then I will pass you the <rRighteous Document>.<n><t>Go and decoct 1 cup of <rElven Fruit Juice> and bring me 2 <rMedicated Grass>. This is part of your training regarding recovery potions. Look for <bOuya> inside Shaitan City <j(862,3591)> if you need help.<n><t>Go to <j(1184, 3557)> and kill 10 <rBig Scorpions> too.")
    MisBeginCondition(NoMission, 711)
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(CheckConvertProfession, MIS_DOCTOR)
    MisBeginAction(AddMission, 711)
    MisBeginAction(SetFlag, 711, 1)
    MisBeginAction(AddTrigger, 7111, TE_GETITEM, 3122, 1)
    MisBeginAction(AddTrigger, 7112, TE_GETITEM, 3129, 2)
    MisBeginAction(AddTrigger, 7113, TE_KILL, 247, 10)
    MisCancelAction(ClearMission, 711)

    MisNeed(MIS_NEED_ITEM, 3122, 1, 10, 1)
    MisNeed(MIS_NEED_ITEM, 3129, 2, 20, 2)
    MisNeed(MIS_NEED_KILL, 247, 10, 30, 10)

    MisPrize(MIS_PRIZE_ITEM, 3954, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>You have done well!<n><t>Since you have passed my test, this is the <rRighteous Document> required to become a Herbalist.<n><t>Keep it well and return here after you are <pLv 10>. ")
    MisHelpTalk("<t>You have not met my requirement. Its not that easy to become a Herbalist.")
    MisResultCondition(HasMission, 711)
    MisResultCondition(HasItem, 3122, 1)
    MisResultCondition(HasItem, 3129, 2)
    MisResultCondition(HasFlag, 711, 39)
    MisResultAction(TakeItem, 3122, 1)
    MisResultAction(TakeItem, 3129, 2)
    MisResultAction(ClearMission, 711)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 3122)
    TriggerAction(1, AddNextFlag, 711, 10, 1)
    RegCurTrigger(7111)

    InitTrigger()
    TriggerCondition(1, IsItem, 3129)
    TriggerAction(1, AddNextFlag, 711, 20, 2)
    RegCurTrigger(7112)

    InitTrigger()
    TriggerCondition(1, IsMonster, 247)
    TriggerAction(1, AddNextFlag, 711, 30, 10)
    RegCurTrigger(7113)

    DefineMission(751, "Walk of the Cleric", 739)

    MisBeginTalk("<t>To become a <bCleric>, you must have faith. I hope that you will remember this. I will give you a simple task.<n><t>Collect a total of 2 <yFancy Petals>, 4 <yPanaceas> and 6 bottles of <yElven Fruit Juice>.<n><t>Pass half of each to <bGranny Dong> in Icicle Haven at <j(795,363)> and <bDoctor Chivo> in Chaldea Haven at <j(630,2091)>. Each must receive 3 bottles of Elven Fruit Juice, 2 Panaceas and 1 Fancy Petal.")
    MisBeginCondition(NoRecord, 739)
    MisBeginCondition(NoMission, 740)
    MisBeginCondition(NoMission, 739)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 5)
    MisBeginCondition(CheckConvertProfession, MIS_CLERGY)
    MisBeginAction(AddMission, 739)
    MisCancelAction(ClearMission, 739)

    MisNeed(MIS_NEED_DESP,"Bring 3 Elven Fruit Juice, 2 Panacea and 1 Fancy Petal to <bGranny Dong> in Icicle Haven at <j(795,363)>.")
    MisNeed(MIS_NEED_DESP,"Bring 3 cups of Elven Fruit Juice, 2 Panacea and 1 Fancy Petal to <bDoctor Chivo> in Chaldea Haven at <j(630,2091)>.")

    MisResultTalk("<t>Congratulations!<n><t>You are now a qualifed <bCleric>!<n><t>May Goddess Kara be with you.")
    MisHelpTalk("<t>What is the reason that is stopping you? Remember! Patience and determination is the key to success!")
    MisResultCondition(HasMission, 739)
    MisResultCondition(HasFlag, 739, 10)
    MisResultCondition(HasFlag, 739, 20)
    MisResultAction(ClearMission, 739)
    MisResultAction(SetRecord, 739)
    MisResultAction(SetProfession, 13)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    DefineMission(752, "Walk of the Cleric", 739, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("This is a gift for me? You are so kind. I will tell the High Priest about it.")
    MisResultCondition(NoRecord, 739)
    MisResultCondition(HasMission, 739)
    MisResultCondition(NoFlag, 739, 10)
    MisResultCondition(HasItem, 3122, 3)
    MisResultCondition(HasItem, 3146, 2)
    MisResultCondition(HasItem, 3130, 1)
    MisResultAction(TakeItem, 3122, 3)
    MisResultAction(TakeItem, 3146, 2)
    MisResultAction(TakeItem, 3130, 1)
    MisResultAction(SetFlag, 739, 10)

    DefineMission(753, "Walk of the Cleric", 739, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("So the high priest sent you. I lack some important herbs and you come at the right moment. I will report back to the high priest regarding this matter.")
    MisResultCondition(NoRecord, 739)
    MisResultCondition(HasMission, 739)
    MisResultCondition(NoFlag, 739, 20)
    MisResultCondition(HasItem, 3122, 3)
    MisResultCondition(HasItem, 3146, 2)
    MisResultCondition(HasItem, 3130, 1)
    MisResultAction(TakeItem, 3122, 3)
    MisResultAction(TakeItem, 3146, 2)
    MisResultAction(TakeItem, 3130, 1)
    MisResultAction(SetFlag, 739, 20)

    DefineMission(755, "Ways of the Seal", 740)

    MisBeginTalk("<t>To become a <bSeal Master>, you must have a balance of good and evil. To test your ability, I need to do something:<n><t>Collect 3 <yDingle Bells> from <rCrazy Sheep>, 10 <yDangerous Sharp Claws> from <rStinging Beak> and 3 <yHearts of Purity> from <rSnow Spirit>. Bring these to <bHoly Priestess - Ada> in Old Shaitan City at <j(862,3303)> and ask her to cleanse them with <ySoul of Purity>. <n><t>Look for <rStinging Beak> in the deep forest in <pAscaron> and the rest of monsters can be found near <pIcicle Castle>.")
    MisBeginCondition(NoRecord, 740)
    MisBeginCondition(NoMission, 740)
    MisBeginCondition(NoMission, 739)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 5)
    MisBeginCondition(CheckConvertProfession, MIS_SEALER)
    MisBeginAction(AddMission, 740)
    MisCancelAction(ClearMission, 740)

    MisNeed(MIS_NEED_DESP,"Bring the required 3 items and <yHeart of Purity> to <bHoly Priestess - Ada> in Old Shaitan City at <j(862,3303)>. Let her purify it with the <ySoul of Purity>.")

    MisResultTalk("<t>Congratulations, you are now a qualified <bSeal Master>.<n><t>May the Goddess bless you!")
    MisHelpTalk("<t>Go now! Have faith in yourself!")
    MisResultCondition(HasMission, 740)
    MisResultCondition(HasFlag, 740, 10)
    MisResultAction(ClearMission, 740)
    MisResultAction(SetRecord, 740)
    MisResultAction(SetProfession, 14)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    DefineMission(756, "Ways of the Seal", 740, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>May the Goddess bless you.<n><t>So you have brought these stuffs to be purified. Very well. I will do my utmost for you in this sacred temple.<n><t>You can go back to Gannon in Shaitan. I prefer to be alone here.<n><t>May the Goddess bless you.")
    MisResultCondition(NoRecord, 740)
    MisResultCondition(HasMission, 740)
    MisResultCondition(NoFlag, 740, 10)
    MisResultCondition(HasItem, 4471, 3)
    MisResultCondition(HasItem, 4385, 10)
    MisResultCondition(HasItem, 4481, 3)
    MisResultAction(TakeItem, 4471, 3)
    MisResultAction(TakeItem, 4385, 10)
    MisResultAction(TakeItem, 4481, 3)
    MisResultAction(SetFlag, 740, 10)
end
RobinMission017()

function RobinMission022()
    DefineMission(713, "Blacksmith's Greetings", 707, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk(
        'You\'re new here?<n><t>People call me <bSmithy>, I\'m the Blacksmith of <pShaitan City>. I forge swords and also make quite a bit of sales off other weapons as well. I\'m really busy<n><t>Don\'t forget to return to tell <bNewbie Guile - Resline> at <j(876,3572)> that I\'m doing all this for her sake, haha!<n><t>(Smithy has given you a pair of "Newbie Knife". Open your inventory and double click on it to equip.)')
    MisResultCondition(NoRecord, 707)
    MisResultCondition(HasMission, 707)
    MisResultCondition(NoFlag, 707, 10)
    MisResultCondition(HasItem, 3956, 1)
    MisResultAction(TakeItem, 3956, 1)
    MisResultAction(SetFlag, 707, 10)
    MisResultBagNeed(1)
end
RobinMission022()

function RobinMission024()
    DefineMission(701, "Welcome", 1, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t> Welcome!<n><t>Come to me if you have any questions regarding Classes and Attribute related issue.<n><t>Next I am going to tell you where to buy good weapons in <pArgent City>.<n><t>Since you have leveled up,  you can press the yellow button below your portrait (Alt + A) to open your character page to distribute your stats. Every time you level up, you will received more points for your own allocation. <n><t>You have 5 basic attributes that can be added: Strength which affects your melee attack power; Agility which increases your Attack Speed and Dodge rate; Accuracy which increases your Hit Rate and range attack power; Spirit which increases your Max SP and magical damage; Constitution which increases your defense and Max HP.")
    MisHelpTalk("<t>Hi! I am the only Newbie Guide in this city. Look for me when you feel the need to understand the basic of this game.<n><t>It will be harsh for you to survive without any help.")
    MisResultCondition(NoRecord, 1)
    MisResultCondition(HasMission, 1)

    MisResultAction(ClearMission, 1)
    MisResultAction(SetRecord, 1)
    MisResultAction(AddExp, 6, 6)

    DefineMission(702, "Blacksmith's Greetings", 701)

    MisBeginTalk("<t>You will not be able to survive in this harsh world if you are going around unarmed.<n><t>Take this letter to <bArgent's Blacksmith - Goldie> at <j(2193,2706)>. I believe he can be of help to you.<n><t>You can use the Radar (Alt + R) to locate him.")
    MisBeginCondition(HasRecord, 1)
    MisBeginCondition(NoRecord, 701)
    MisBeginCondition(NoMission, 701)
    MisBeginAction(AddMission, 701)
    MisBeginAction(SetFlag, 701, 1)
    MisBeginAction(GiveItem, 3950, 1, 4)
    MisCancelAction(ClearMission, 701)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bArgent's Blacksmith - Goldie> at <j(2193,2706)>. Return to look for <bNewbie Guide - Senna> at <j(2223,2785)>.")

    MisResultTalk("<t>I see, so you've met <bGoldie>. He's the guy to look for when you have made enough money to get a good weapon.")
    MisHelpTalk("<t>Remember to hand the letter to Goldie personally. He is at the left corner of Argent City at <j(2193, 2706)>.")
    MisResultCondition(NoRecord, 701)
    MisResultCondition(HasMission, 701)
    MisResultCondition(HasFlag, 701, 10)
    MisResultAction(ClearMission, 701)
    MisResultAction(SetRecord, 701)
    MisResultAction(AddExp, 9, 9)

    DefineMission(704, "Tailor's Greetings", 702)

    MisBeginTalk("<t>This is the second recommendation letter. Pass it to <bTailor - Granny Nila> at <j(2267,2704)>.")
    MisBeginCondition(HasRecord, 701)
    MisBeginCondition(NoRecord, 702)
    MisBeginCondition(NoMission, 702)
    MisBeginAction(AddMission, 702)
    MisBeginAction(SetFlag, 702, 1)
    MisBeginAction(GiveItem, 3951, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Hand the letter to <bTailor - Granny Nila> at <j(2267,2704)> and return to <bNewbie Guide - Senna> at <j(2223,2785)>.")

    MisResultTalk("<t>Wow! You mean <bGranny Nila> specially custom made these gloves for you? It looks perfect! I hope you take good care of it.")
    MisHelpTalk("<t>Have you met Granny Nila? She is located in North of Argent City at <j(2267,2704)>.")
    MisResultCondition(NoRecord, 702)
    MisResultCondition(HasMission, 702)
    MisResultCondition(HasFlag, 702, 10)
    MisResultAction(ClearMission, 702)
    MisResultAction(SetRecord, 702)
    MisResultAction(AddExp, 21, 21)

    DefineMission(706, "Physician's Greetings", 703)

    MisBeginTalk("<t>Now, give this final letter to <bPhysician - Ditto> at <j(2250,2770)>. I am quite sure he will be able to render his assistance to you.")
    MisBeginCondition(HasRecord, 702)
    MisBeginCondition(NoRecord, 703)
    MisBeginCondition(NoMission, 703)
    MisBeginAction(AddMission, 703)
    MisBeginAction(SetFlag, 703, 1)
    MisBeginAction(GiveItem, 3952, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bPhysician - Ditto> at <j(2250,2770)>. Return to look for <bNewbie Guide Senna> at <j(2223, 2785)>.")

    MisResultTalk("<t>Oh.. So <bDitto> gave you some <rApples>. An <rApple> a day keeps the doctor away! It can also recover a bit of HP.")
    MisHelpTalk("<t><bDitto> is at the right side of the fountain in Argent City. His coordinates are <j(2250, 2770)>.")
    MisResultCondition(NoRecord, 703)
    MisResultCondition(HasMission, 703)
    MisResultCondition(HasFlag, 703, 10)
    MisResultAction(ClearMission, 703)
    MisResultAction(SetRecord, 703)
    MisResultAction(AddExp, 66, 66)

    DefineMission(708, "Battle training", 704)

    MisBeginTalk("<t>Hmm, not bad, now that your preparations are ready. Then go ahead and leave the comforts of the city and try some fighting but of course do not travel too far, you may take this as the last test I have for you.<n><t>From this side, head left and your on the right track to exit the city.<n><t>Defeat 5 Forest Spirits and bring back a pair of Wings.<n><t>The forest spirits can be found just outside the city gates while the pair of wing can be found on any forest spirit. Complete the mission and I will give you some nice reward.<n><t>(You can enter combat by just left clicking on the enemy target, however combat cannot be initiated in the city. To pick items, left click on the items or you can use CTRL + A for quick looting.")
    MisBeginCondition(HasRecord, 703)
    MisBeginCondition(NoRecord, 704)
    MisBeginCondition(NoMission, 704)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 10)
    MisBeginAction(AddMission, 704)
    MisBeginAction(SetFlag, 704, 1)
    MisBeginAction(AddTrigger, 7041, TE_GETITEM, 1620, 1)
    MisBeginAction(AddTrigger, 7042, TE_KILL, 103, 5)
    MisCancelAction(ClearMission, 704)

    MisNeed(MIS_NEED_ITEM, 1620, 1, 10, 1)
    MisNeed(MIS_NEED_KILL, 103, 5, 20, 5)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Well done, it looks like you now have a good grasp at basic combat and also a good idea on how item drops work.<n><t>Since there is nothing much left, why don't you go and look for my friends? If fighting is your cup of tea, seek out <bMarcusa> at <j<j(2065, 2732)>>. Another person you may like to meet would be <bPhysician Ditto> at <j(2250, 2770)>. Lately, he has been looking for helpers to help her collect more ingredients.<n><t>Good luck!")
    MisHelpTalk("<t>Don't be anxious, come back after you've kill 5 <rForest Spirits> and obtained 1 of their <rWing>.")
    MisResultCondition(NoRecord, 704)
    MisResultCondition(HasMission, 704)
    MisResultCondition(HasItem, 1620, 1)
    MisResultCondition(HasFlag, 704, 24)
    MisResultAction(TakeItem, 1620, 1)
    MisResultAction(ClearMission, 704)
    MisResultAction(SetRecord, 704)
    MisResultAction(AddExp, 75, 75)

    InitTrigger()
    TriggerCondition(1, IsItem, 1620)
    TriggerAction(1, AddNextFlag, 704, 10, 1)
    RegCurTrigger(7041)
    InitTrigger()
    TriggerCondition(1, IsMonster, 103)
    TriggerAction(1, AddNextFlag, 704, 20, 5)
    RegCurTrigger(7042)
end
RobinMission024()

function RobinMission025()
    DefineMission(705, "Tailor's Greetings", 702, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>My my, this is interesting. I haven\'t seen you around here before. I am <bGranny Nila>, owner of the tailor shop in <pArgent City>. Since Senna has recommended you, I shall have these pair of <rNewbie Gloves> custom-made for you. Do drop by more often in your free time and perhaps we can have a chat.<n><t>Now return to <bNewbie Guide Senna> at <j(2223, 2785)>.<n><t>(Nila has given you a pair of "Newbie Gloves". Open your inventory and double click on it to equip.)')
    MisResultCondition(NoRecord, 702)
    MisResultCondition(HasMission, 702)
    MisResultCondition(NoFlag, 702, 10)
    MisResultCondition(HasFlag, 702, 1)
    MisResultCondition(HasItem, 3951, 1)
    MisResultAction(TakeItem, 3951, 1)
    MisResultAction(SetFlag, 702, 10)
    MisResultAction(GiveItem, 465, 1, 4)
    MisResultBagNeed(1)
end
RobinMission025()

function RobinMission026()
    DefineMission(709, "Courage Certificate", 705)

    MisBeginTalk("<t>Are you here for the Courage Certificate? You have some guts.<n><t>However courage alone is not enough, to aquire the <rCourage Certificate>, you must prove that you have the required ability and intellect.<n><t>Go to the <pOutskirt of Argent City> and defeat 10 <rCuddly Lambs> at <j(2057, 2638)>, 10 <rBubble Clams> near the northern coast, 10 <rPiglets> and then return to me.<n><t>If you are able to complete these task, I will consider you as a qualifed warrior and give you the <bCourage Certificate>.")
    MisBeginCondition(NoMission, 705)
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CheckConvertProfession, MIS_FENCER)
    MisBeginAction(AddMission, 705)
    MisBeginAction(SetFlag, 705, 1)
    MisBeginAction(AddTrigger, 7051, TE_KILL, 237, 10)
    MisBeginAction(AddTrigger, 7052, TE_KILL, 213, 10)
    MisBeginAction(AddTrigger, 7053, TE_KILL, 125, 10)
    MisCancelAction(ClearMission, 705)

    MisNeed(MIS_NEED_KILL, 237, 10, 10, 10)
    MisNeed(MIS_NEED_KILL, 213, 10, 20, 10)
    MisNeed(MIS_NEED_KILL, 125, 10, 30, 10)

    MisPrize(MIS_PRIZE_ITEM, 3953, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Friend! You have done well! This is the required <rCourage Certificate>.<n><t>Keep it well and look for me with it when you reached <pLv 10> to become a full fledged Swordsman.")
    MisHelpTalk("<t>You have not fulfilled my requirement. It is not easy to attain.")
    MisResultCondition(HasMission, 705)
    MisResultCondition(HasFlag, 705, 19)
    MisResultCondition(HasFlag, 705, 29)
    MisResultCondition(HasFlag, 705, 39)
    MisResultAction(ClearMission, 705)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 237)
    TriggerAction(1, AddNextFlag, 705, 10, 10)
    RegCurTrigger(7051)

    InitTrigger()
    TriggerCondition(1, IsMonster, 213)
    TriggerAction(1, AddNextFlag, 705, 20, 10)
    RegCurTrigger(7052)

    InitTrigger()
    TriggerCondition(1, IsMonster, 125)
    TriggerAction(1, AddNextFlag, 705, 30, 10)
    RegCurTrigger(7053)

    DefineMission(749, "Code of the Crusader", 737)

    MisBeginTalk("<t>Looks like you have become an excellent Swordsman. Since you want to be stronger, you should advance to become a <bCrusader>. I will need to test you accordingly.<n><t>In the forest west of <pSolace Haven>, there are some <rThickskin Lizards>. Get 5 <yGreasy Lizard Skins> from them and go to <pSkeleton Haven> east of Icicle City, defeat 10 <rSkeletal Warriors> and get 3 <yWarrior Certificates>. Lastly, buy a set of <yBreast Plate> and return to me.")
    MisBeginCondition(NoRecord, 737)
    MisBeginCondition(NoMission, 737)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 1)
    MisBeginCondition(CTypeCheck, {1,3})
    --MisBeginCondition(CheckConvertProfession, MIS_TWO_FENCER)
    MisBeginAction(AddMission, 737)
    MisBeginAction(AddTrigger, 7372, TE_KILL, 268, 10)
    MisBeginAction(AddTrigger, 7373, TE_GETITEM, 4474, 5)
    MisBeginAction(AddTrigger, 7374, TE_GETITEM, 4456, 3)
    MisBeginAction(AddTrigger, 7375, TE_GETITEM, 295, 1)
    MisCancelAction(ClearMission, 737)

    MisNeed(MIS_NEED_KILL, 268, 10, 20, 10)
    MisNeed(MIS_NEED_ITEM, 4474, 5, 30, 5)
    MisNeed(MIS_NEED_ITEM, 4456, 3, 40, 3)
    MisNeed(MIS_NEED_ITEM, 295, 1, 50, 1)

    MisResultTalk("<t>Congratulations!<n><t>You are now a full fledged <bCrusader>!<n><t>More challenges awaits you from now on!")
    MisHelpTalk("<t>Although the requirements are tough, but I am sure you can do it.")
    MisResultCondition(HasMission, 737)
    MisResultCondition(HasFlag, 737, 29)
    MisResultCondition(HasItem, 4474, 5)
    MisResultCondition(HasItem, 4456, 3)
    MisResultCondition(HasItem, 295, 1)
    MisResultAction(TakeItem, 4474, 5)
    MisResultAction(TakeItem, 4456, 3)
    MisResultAction(TakeItem, 295, 1)
    MisResultAction(ClearMission, 737)
    MisResultAction(SetRecord, 737)
    MisResultAction(SetProfession, 9)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 268)
    TriggerAction(1, AddNextFlag, 737, 20, 10)
    RegCurTrigger(7372)
    InitTrigger()
    TriggerCondition(1, IsItem, 4474)
    TriggerAction(1, AddNextFlag, 737, 30, 5)
    RegCurTrigger(7373)
    InitTrigger()
    TriggerCondition(1, IsItem, 4456)
    TriggerAction(1, AddNextFlag, 737, 30, 3)
    RegCurTrigger(7374)
    InitTrigger()
    TriggerCondition(1, IsItem, 295)
    TriggerAction(1, AddNextFlag, 737, 30, 1)
    RegCurTrigger(7375)

    DefineMission(750, "Champion's Valor", 738)

    MisBeginTalk("<t>Want to become a <bChampion>? Good!<n><t>However, you must bring the following items before you return: <n><t>You must have a suitable weapon of a Champion, go and buy a <yCriss Sword>. Bring me 1 <yNinja Masks> from <rNinja Mole>. 5 <rPolar Bear>'s <yGreat Bear Tooth> and 5 <ySolid Rock> from <rRock Golem>.<n><t>This way, you can prove that you have the capability to become a great Champion.<n><t>Look for <rNinja Mole> in <pAbandoned Mine>, <rRock Golem> in <pAndes Forest> and <rPolar Bear> can be found east of Icicle City")
    MisBeginCondition(NoRecord, 738)
    MisBeginCondition(NoMission, 738)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 1)
    MisBeginCondition(CheckConvertProfession, MIS_LARGE_FENCER)
    MisBeginAction(AddMission, 738)
    MisBeginAction(AddTrigger, 7381, TE_GETITEM, 15, 1)
    MisBeginAction(AddTrigger, 7382, TE_GETITEM, 4454, 1)
    MisBeginAction(AddTrigger, 7383, TE_GETITEM, 4453, 5)
    MisBeginAction(AddTrigger, 7384, TE_GETITEM, 4368, 5)
    MisCancelAction(ClearMission, 738)

    MisNeed(MIS_NEED_ITEM, 15, 1, 10, 1)
    MisNeed(MIS_NEED_ITEM, 4454, 1, 20, 1)
    MisNeed(MIS_NEED_ITEM, 4453, 5, 30, 5)
    MisNeed(MIS_NEED_ITEM, 4368, 5, 40, 5)

    MisResultTalk("<t>Congratulations! You have successfully become a qualified <bChampion>!<n><t>More challenges awaits you!")
    MisHelpTalk("<t>Any questions? <rNinja Mole> can be found in <pAbandon Mine>, <rRock Golem> in <pAndes Forest> and <rPolar Bears> at the east of <pIcicle Castle>.")
    MisResultCondition(HasMission, 738)
    MisResultCondition(HasItem, 15, 1)
    MisResultCondition(HasItem, 4454, 1)
    MisResultCondition(HasItem, 4453, 5)
    MisResultCondition(HasItem, 4368, 5)
    MisResultAction(TakeItem, 15, 1)
    MisResultAction(TakeItem, 4454, 1)
    MisResultAction(TakeItem, 4453, 5)
    MisResultAction(TakeItem, 4368, 5)
    MisResultAction(ClearMission, 738)
    MisResultAction(SetRecord, 738)
    MisResultAction(SetProfession, 8)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 15)
    TriggerAction(1, AddNextFlag, 738, 10, 1)
    RegCurTrigger(7381)
    InitTrigger()
    TriggerCondition(1, IsItem, 4454)
    TriggerAction(1, AddNextFlag, 738, 20, 1)
    RegCurTrigger(7382)
    InitTrigger()
    TriggerCondition(1, IsItem, 4453)
    TriggerAction(1, AddNextFlag, 738, 30, 5)
    RegCurTrigger(7383)
    InitTrigger()
    TriggerCondition(1, IsItem, 4368)
    TriggerAction(1, AddNextFlag, 738, 40, 5)
    RegCurTrigger(7384)
end
RobinMission026()

function RobinMission027()
    DefineMission(711, "Welcome", 2, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Ah!<n><t>Another newcomer, welcome! You can come to me if you have any questions regarding Classes and Attribute related issue.<n><t>Next I am going to tell you where to buy good weapons in <pShaitan City>.<n><t>Since you have leveled up,  you can press the yellow button below your protrait (Alt + A) to open your character page to distribute your stats. Every time you level up, you will received more points for your own allocation.<n><t>You have 5 basic attributes that can be added: Strength which affects your melee attack power; Agility which increases your attack speed and dodge rate; Accuracy which increases your hit rate and range attack power; Spirit which increases your max SP and magical damage; Constitution which increases your defense and max HP.")
    MisHelpTalk("<t>If you don't accept my assistance, you will find that you may encounter various problems along the way that you cannot solve.")
    MisResultCondition(NoRecord, 2)
    MisResultCondition(HasMission, 2)

    MisResultAction(ClearMission, 2)
    MisResultAction(SetRecord, 2)
    MisResultAction(AddExp, 6, 6)

    DefineMission(712, "Blacksmith's Greetings", 707)

    MisBeginTalk("<t>You will not be able to survive in this harsh world if you are going around unarmed. Take this letter to <bBlacksmith Smithy> at <j(902, 3495)>.<n><t>Check to see if there is any suitable weapon for you.")
    MisBeginCondition(HasRecord, 2)
    MisBeginCondition(NoRecord, 707)
    MisBeginCondition(NoMission, 707)
    MisBeginAction(AddMission, 707)
    MisBeginAction(SetFlag, 707, 1)
    MisBeginAction(GiveItem, 3956, 1, 4)
    MisCancelAction(ClearMission, 707)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Hand the letter to <bBlacksmith - Smithy> in <pShaitan City> at <j(902, 3495)> and return to <bNewbie Guide Resline> at <j(876, 3572)>.")

    MisResultTalk("<t>You have seen <bSmithy>? Look for him for better weapons when you have earned enough.")
    MisHelpTalk("<t>Remember to give this letter to <bSmithy>. He can be found towards the north-eastern corner of <pShaitan City> at <j(902, 3495)>.")
    MisResultCondition(NoRecord, 707)
    MisResultCondition(HasMission, 707)
    MisResultCondition(HasFlag, 707, 10)
    MisResultAction(ClearMission, 707)
    MisResultAction(SetRecord, 707)
    MisResultAction(AddExp, 9, 9)

    DefineMission(714, "Tailor's Greetings", 708)

    MisBeginTalk("<t>Okay, look for the tailor next. You need to send the letter over to <pShaitan City> <bTailor - Moya> at <j(894,3602)>.")
    MisBeginCondition(HasRecord, 707)
    MisBeginCondition(NoRecord, 708)
    MisBeginCondition(NoMission, 708)
    MisBeginAction(AddMission, 708)
    MisBeginAction(SetFlag, 708, 1)
    MisBeginAction(GiveItem, 3957, 1, 4)

    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Hand the letter to <bTailor Moya> in <pShaitan City> at <j(894, 3602)> and return to <bNewbie Guide Resline> at <j(876, 3572)>")

    MisResultTalk("<t>Oh? So this is the gloves <bMoya> give you? It looks good. You must treasure it well.")
    MisHelpTalk("<t>You must hand the letter to Moya.<n><t>He is at <j(894, 3602)>. Locate him with the radar.")
    MisResultCondition(NoRecord, 708)
    MisResultCondition(HasMission, 708)
    MisResultCondition(HasFlag, 708, 10)
    MisResultAction(ClearMission, 708)
    MisResultAction(SetRecord, 708)
    MisResultAction(AddExp, 21, 21)

    DefineMission(716, "Physician's Greetings", 709)

    MisBeginTalk("<t>Okay, time for you to visit <bPhysician - Shala> at <j(903, 3646)>. This is the last letter, so hurry up and go quickly.")
    MisBeginCondition(HasRecord, 708)
    MisBeginCondition(NoRecord, 709)
    MisBeginCondition(NoMission, 709)
    MisBeginAction(AddMission, 709)
    MisBeginAction(SetFlag, 709, 1)
    MisBeginAction(GiveItem, 3958, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Hand the letter to <bPhysician - Shala> in <pShaitan City> at <j(903, 3646)> and return to <bNewbie Guide Resline> at <j(876, 3572)>")

    MisResultTalk("<t>These <rApples> given to you by <bShala> are not only good for health, they have healing properties and will allow you to regain a bit of HP.")
    MisHelpTalk("<t>The Physician, Shala, is not too far away from here. She can be located around the fountain area <j(903, 3646)>.")
    MisResultCondition(NoRecord, 709)
    MisResultCondition(HasMission, 709)
    MisResultCondition(HasFlag, 709, 10)
    MisResultAction(ClearMission, 709)
    MisResultAction(SetRecord, 709)
    MisResultAction(AddExp, 66, 66)

    DefineMission(718, "Battle training", 710)

    MisBeginTalk("Well well, not bad at all! Now that you are sufficiently equipped, you are fit to venture out of town to test out your fighting skills.<n><t>Kill 5 <rBaby Scorpions> outside the eastern side of the city. While you're at it, please collect 1 <rCactus Hairball> from fighting <rMelons> as well. Of course, always remember that safety comes first!<n><t>(You can enter combat by just left clicking on the enemy target, however combat cannot be initiated in the city. To pick items, left click on the items or you can use CTRL + A for quick looting.")
    MisBeginCondition(HasRecord, 709)
    MisBeginCondition(NoRecord, 710)
    MisBeginCondition(NoMission, 710)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 10)
    MisBeginAction(AddMission, 710)
    MisBeginAction(SetFlag, 710, 1)
    MisBeginAction(AddTrigger, 7101, TE_GETITEM, 1691, 1)
    MisBeginAction(AddTrigger, 7102, TE_KILL, 188, 5)
    MisCancelAction(ClearMission, 710)

    MisNeed(MIS_NEED_ITEM, 1691, 1, 10, 1)
    MisNeed(MIS_NEED_KILL, 188, 5, 20, 5)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Well done, it looks like you now have a good grasp at basic combat and also a good idea on how item drops work.<n><t>Since there is nothing much left, why don't you go and look for my friends? If fighting is your cup of tea, seek out <bMichael>. He's a patroller and is often seen patrolling about the outskirts of this city at <j(2085, 2742)>. Another person you may like to meet would be Physician <bShala> at <j(2250, 2770)>. Lately, she has been looking for helpers to help him collect more ingredients for her medicine.")
    MisHelpTalk("<t>Don't have to be anxious, come back after you've slain 5 <rBaby Scorpions> and obtained 1 <rCactus Hairballs>.")
    MisResultCondition(NoRecord, 710)
    MisResultCondition(HasMission, 710)
    MisResultCondition(HasItem, 1691, 1)
    MisResultCondition(HasFlag, 710, 24)
    MisResultAction(TakeItem, 1691, 1)
    MisResultAction(ClearMission, 710)
    MisResultAction(SetRecord, 710)
    MisResultAction(AddExp, 75, 75)

    InitTrigger()
    TriggerCondition(1, IsItem, 1691)
    TriggerAction(1, AddNextFlag, 710, 10, 1)
    RegCurTrigger(7101)
    InitTrigger()
    TriggerCondition(1, IsMonster, 188)
    TriggerAction(1, AddNextFlag, 710, 20, 5)
    RegCurTrigger(7102)
end
RobinMission027()

function RobinMission028()
    DefineMission(715, "Tailor's Greetings", 708, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>Keke!<n><t>It is Resline\'s recommendation letter <n><t>Okay, as usual, this pair of gloves is my gift to you.<n><t>The next time round, I will charge you cheaper when you come to the shop to buy your stuff.<n><t For now, return to <bNewbie Guide Resline> at <j(876, 3572)>. She will introduce you to others.<n><t>(Moya has given you a "Newbie Glove", you can select it from your inventory and equip it).'
    )
    MisResultCondition(NoRecord, 708)
    MisResultCondition(HasMission, 708)
    MisResultCondition(NoFlag, 708, 10)
    MisResultCondition(HasItem, 3957, 1)
    MisResultAction(TakeItem, 3957, 1)
    MisResultAction(SetFlag, 708, 10)
    MisResultAction(GiveItem, 465, 1, 4)
    MisResultBagNeed(1)
end
RobinMission028()

function RobinMission030()
    DefineMission(721, "Welcome", 3, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Ah!<n><t>Another newcomer, welcome! You can come to me if you have any questions regarding Classes and Attribute related issue.<n><t>Next I am going to tell you where to buy good weapons in <pIcicle City>.<n><t>Since you have leveled up, you can press the yellow button below your protrait (Alt + A) to open your character page to distribute your stats. Every time you level up, you will received more points for your own allocation.<n><t>You have 5 basic attributes that can be added: Strength which affects your melee attack power; Agility which increases your attack speed and dodge rate; Accuracy which increases your hit rate and range attack power; Spirit which increases your max SP and magical damage; Constitution which increases your defense and max HP.")
    MisHelpTalk("<t>Hi! I am the only Newbie Guide in this city. Look for me when you feel the need to understand the basic of this game.<n><t>It will be harsh for you to survive without any help.")
    MisResultCondition(NoRecord, 3)
    MisResultCondition(HasMission, 3)

    MisResultAction(ClearMission, 3)
    MisResultAction(SetRecord, 3)
    MisResultAction(AddExp, 6, 6)

    DefineMission(722, "Blacksmith's Greetings", 713)

    MisBeginTalk("<t>You will not be able to survive in this harsh world if you are going around unarmed. Take this letter to <pArgent City>'s <bBlacksmith Goldie> at <j(2193, 2706)>. I believe he can be of help to you.")
    MisBeginCondition(HasRecord, 3)
    MisBeginCondition(NoRecord, 713)
    MisBeginCondition(NoMission, 713)
    MisBeginAction(AddMission, 713)
    MisBeginAction(SetFlag, 713, 1)
    MisBeginAction(GiveItem, 3959, 1, 4)
    MisCancelAction(ClearMission, 713)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Bring the letter to <bBlacksmith - Bash> in <pIcicle City> at <j(1344, 529)>. Return to <bNewbie Guide Angela> at <j(1315, 507)> after that.")

    MisResultTalk("<t>Have you met <bBash> yet? Did he ask you to introduce a girlfriend to him because he's still single?")
    MisHelpTalk("<t>Remember to give this letter to <bBash>. He can be found towards the south-eastern corner of <pIcicle Castle> at <j(1344, 529)>.")
    MisResultCondition(NoRecord, 713)
    MisResultCondition(HasMission, 713)
    MisResultCondition(HasFlag, 713, 10)
    MisResultAction(ClearMission, 713)
    MisResultAction(SetRecord, 713)
    MisResultAction(AddExp, 9, 9)

    DefineMission(724, "Tailor's Greetings", 714)

    MisBeginTalk("<t>Here's another letter. Please give it <pHannah> <j(1349, 539)>, owner of the tailor shop in Icicle Castle. She'll definitely be glad to help you out.")
    MisBeginCondition(HasRecord, 713)
    MisBeginCondition(NoRecord, 714)
    MisBeginCondition(NoMission, 714)
    MisBeginAction(AddMission, 714)
    MisBeginAction(SetFlag, 714, 1)
    MisBeginAction(GiveItem, 3960, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bTailor - Hannah> in <pIcicle City> at <j(1349, 539)>. Return to look for <bNewbie Guide Angela> at <j(1315, 507)>.")

    MisResultTalk("<t>So <b Hannah> made you a new glove? It looks good on you. Treasure it well.")
    MisHelpTalk("<t>This letter has to reach <bHannah> by today.<n><t>She can be found south-east of the fountain in <pIcicle Castle> at <j(1349, 539)>. You may also try using the mini-map to help you.")
    MisResultCondition(NoRecord, 714)
    MisResultCondition(HasMission, 714)
    MisResultCondition(HasFlag, 714, 10)
    MisResultAction(ClearMission, 714)
    MisResultAction(SetRecord, 714)
    MisResultAction(AddExp, 21, 21)

    DefineMission(726, "Physician's Greetings", 715)

    MisBeginTalk("<t>Lastly, remember! Potions are very important for any battle. To get started, bring this letter to <pIcicle City>'s <bPhysican - Daisha> at <j(1352, 499)>.")
    MisBeginCondition(HasRecord, 714)
    MisBeginCondition(NoRecord, 715)
    MisBeginCondition(NoMission, 715)
    MisBeginAction(AddMission, 715)
    MisBeginAction(SetFlag, 715, 1)
    MisBeginAction(GiveItem, 3961, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bPhysican - Daisha> in <pIcicle City> at <j(1352, 499)>. Return to look for <bNewbie Guide Angela> at <j(1315, 507)>.")

    MisResultTalk("<t>Hey, these <rApples> are given to you by <bDaisha>.<n><t>Eating <rApples> are beneficial as they can recover a small amount of HP.")
    MisHelpTalk("<t>Daisha can be found east of the fountain in <pIcicle Castle> <j(1352, 499)>.")
    MisResultCondition(NoRecord, 715)
    MisResultCondition(HasMission, 715)
    MisResultCondition(HasFlag, 715, 10)
    MisResultAction(ClearMission, 715)
    MisResultAction(SetRecord, 715)
    MisResultAction(AddExp, 66, 66)

    DefineMission(728, "Battle training", 716)

    MisBeginTalk("<t>Hmm, not bad.<n><t>Now that your preparations are complete, its time for the special training. Go outside the city and slay 5 <Snow Squirts>.<n><t>Oh by the way, also collect a special seed for me. The Snow Squirt can be found south of the Icicle City gates while the seed can be found on the Snowy Mystic Shrub. Look for me after you have completed your task!<n><t>(You can enter combat by just left clicking on the enemy target, however combat cannot be initiated in the city. To pick items, left click on the items or you can use CTRL + A for quick item loot.")
    MisBeginCondition(HasRecord, 715)
    MisBeginCondition(NoRecord, 716)
    MisBeginCondition(NoMission, 716)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 10)
    MisBeginAction(AddMission, 716)
    MisBeginAction(AddTrigger, 7161, TE_GETITEM, 1597, 1)
    MisBeginAction(AddTrigger, 7162, TE_KILL, 234, 5)
    MisBeginAction(SetFlag, 716, 1)
    MisCancelAction(ClearMission, 716)

    MisNeed(MIS_NEED_ITEM, 1597, 1, 10, 1)
    MisNeed(MIS_NEED_KILL, 234, 5, 20, 5)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Well done, it looks like you now have a good grasp at basic combat and also a good idea on how item drops work.")
    MisHelpTalk("<t>Remember! Defeat 5 Snow Squirt and collect 1 Seed")
    MisResultCondition(NoRecord, 716)
    MisResultCondition(HasMission, 716)
    MisResultCondition(HasItem, 1597, 1)
    MisResultCondition(HasFlag, 716, 24)
    MisResultAction(TakeItem, 1597, 1)
    MisResultAction(ClearMission, 716)
    MisResultAction(SetRecord, 716)
    MisResultAction(AddExp, 75, 75)

    InitTrigger()
    TriggerCondition(1, IsItem, 1597)
    TriggerAction(1, AddNextFlag, 716, 10, 1)
    RegCurTrigger(7161)
    InitTrigger()
    TriggerCondition(1, IsMonster, 234)
    TriggerAction(1, AddNextFlag, 716, 20, 5)
    RegCurTrigger(7162)
end
RobinMission030()

function RobinMission031()
    DefineMission(723, "Blacksmith's Greetings", 713, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>You are new around here? I am <bBash>, the Blacksmith of <pIcicle Castle>. I specialize in making weapons and sadly, still single.<n><t>Now return to <bAngela> at <j(1315, 507)>. She will introduce you to other NPCs.<n><t>(Bash has given you a pair of "Newbie Knife". Open your inventory and double click on it to equip.)')
    MisResultCondition(NoRecord, 713)
    MisResultCondition(HasMission, 713)
    MisResultCondition(NoFlag, 713, 10)
    MisResultCondition(HasItem, 3959, 1)
    MisResultAction(TakeItem, 3959, 1)
    MisResultAction(SetFlag, 713, 10)
    MisResultBagNeed(1)
end
RobinMission031()

function RobinMission032()
    DefineMission(725, "Tailor's Greetings", 714, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>You are new around here, aren\'t you? Welcome to <pIcicle Castle>. I\'m the owner of the tailor shop, <bHannah>. Thank you for the letter, please take these gloves, they are custom made for you. Don\'t forget to return to <bNewbie Guide Angela> at <j(1315, 507)> and tell her that I\'ve received her letter.<n><t>(Hannah has given you a pair of "Newbie Gloves". Open your inventory and double click on it to equip.)')
    MisResultCondition(NoRecord, 714)
    MisResultCondition(HasMission, 714)
    MisResultCondition(NoFlag, 714, 10)
    MisResultCondition(HasItem, 3960, 1)
    MisResultAction(TakeItem, 3960, 1)
    MisResultAction(SetFlag, 714, 10)
    MisResultAction(GiveItem, 465, 1, 4)
    MisResultBagNeed(1)
end
RobinMission032()

function RobinMission033()
    DefineMission(727, "Physician's Greetings", 715, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t> You say you\'re new around here? Welcome! Take these <rApples>, they\'re on the house! I\'m sure they\'ll come in handy to you.<n><t>Now return to <bAngela> at <j(1315, 507)>.<n><t>(Daisha has given you some "Apples". Drag to F1 - F12 hotkey slot to use as a shortcut.)')
    MisResultCondition(NoRecord, 715)
    MisResultCondition(HasMission, 715)
    MisResultCondition(NoFlag, 715, 10)
    MisResultCondition(HasItem, 3961, 1)
    MisResultAction(TakeItem, 3961, 1)
    MisResultAction(SetFlag, 715, 10)
    MisResultAction(GiveItem, 1847, 10, 4)
    MisResultBagNeed(1)

    DefineMission(735, "Collector's Habit", 723)

    MisBeginTalk("<t>Lately after tidying up my collection, I realized that I am missing some <rOctopus Tentacles>.<n><t>I need you to help me find 5 <rOctopus Tentacles> which can be found off the <bSnow Squirts> near the city entrance.")
    MisBeginCondition(NoMission, 723)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 723)
    MisBeginAction(SetFlag, 723, 1)
    MisBeginAction(AddTrigger, 7231, TE_GETITEM, 1704, 5)
    MisCancelAction(ClearMission, 723)

    MisNeed(MIS_NEED_ITEM, 1704, 5, 10, 5)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Good! My collection has increased again.<n><t>Thank you!")
    MisHelpTalk("<t>What's the problem? Is it that hard to find them? They are just outside the main entrance.")
    MisResultCondition(HasMission, 723)
    MisResultCondition(HasItem, 1704, 5)
    MisResultAction(TakeItem, 1704, 5)
    MisResultAction(AddExp, 40, 70)
    MisResultAction(ClearMission, 723)

    InitTrigger()
    TriggerCondition(1, IsItem, 1704)
    TriggerAction(1, AddNextFlag, 723, 10, 5)
    RegCurTrigger(7231)

    DefineMission(746, "Collector's Habit", 734)

    MisBeginTalk("<t>I have just found out that there isn't any withered branch in my collection at all! Its quite a shame. Can you please go to <j(1179, 475)> and get 5 <yWithered Branch> from the <rSnow Squidy>? Go now quickly for if I am unhappy with my collection, I won't give you any more new task!")
    MisBeginCondition(NoMission, 734)
    MisBeginCondition(NoMission, 723)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 734)
    MisBeginAction(SetFlag, 734, 1)
    MisBeginAction(AddTrigger, 7341, TE_GETITEM, 3372, 5)
    MisCancelAction(ClearMission, 734)

    MisNeed(MIS_NEED_ITEM, 3372, 5, 10, 5)

    MisPrize(MIS_PRIZE_MONEY, 200, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Hoho. I have added more to my collection.")
    MisHelpTalk("<t>Oh my...<n><t>Its only Withered Branch! Unable to complete such a simple task? Those <rSnow Squidy> just outside of Icicle Castle have it on them.")
    MisResultCondition(HasMission, 734)
    MisResultCondition(HasItem, 3372, 5)
    MisResultAction(TakeItem, 3372, 5)
    MisResultAction(AddExp, 70, 95)
    MisResultAction(ClearMission, 734)

    InitTrigger()
    TriggerCondition(1, IsItem, 3372)
    TriggerAction(1, AddNextFlag, 734, 10, 5)
    RegCurTrigger(7341)

    DefineMission(747, "Collector's Habit", 735)

    MisBeginTalk("<t>Due to the large additions to my collection of item, I have run out of space to store them! Can you please give me a hand? Go to <j(1179, 371)> and collect 5 <yMedicine Bottles> from <rSnowy Piglets>. Oh by the way, remember to bring more healing potions along, you will need them.")
    MisBeginCondition(NoMission, 735)
    MisBeginCondition(NoMission, 734)
    MisBeginCondition(NoMission, 723)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 735)
    MisBeginAction(SetFlag, 735, 1)
    MisBeginAction(AddTrigger, 7351, TE_GETITEM, 1779, 5)
    MisCancelAction(ClearMission, 735)

    MisNeed(MIS_NEED_ITEM, 1779, 5, 10, 5)

    MisPrize(MIS_PRIZE_MONEY, 300, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Hehe! <n><t>Now that I have the medicine bottle, I am able to store more goods. Haha, I am really happy now!")
    MisHelpTalk("<t>Its only a few Bbttles, go now!")
    MisResultCondition(HasMission, 735)
    MisResultCondition(HasItem, 1779, 5)
    MisResultAction(TakeItem, 1779, 5)
    MisResultAction(AddExp, 95, 125)
    MisResultAction(ClearMission, 735)

    InitTrigger()
    TriggerCondition(1, IsItem, 1779)
    TriggerAction(1, AddNextFlag, 735, 10, 5)
    RegCurTrigger(7351)
end
RobinMission033()

function RobinMission034()
    DefineMission(729, "Hunter Manual", 717)

    MisBeginTalk("<t>Are you here to aquire the Hunter Manual? You are really brave.<n><t>However bravery alone is not enough, to aquire the <rHunter Manual>, you must prove that you have the required agility and dexterity.<n><t>Go to <pIcicle City> and capture 10 <rLittle Deers> at <j(1164, 305)> and 10 <rLittle White Deers> at <j(1325, 305)>.<n><t>In additional, you need to have <rElven Fruit Juice>.<n><t>As hunter spends a long amount of time outside hunting solidary, having items that regenerate HP is a must.<n><t>If you managed to complete these tasks, I will consider you to be a qualifed hunter.")
    MisBeginCondition(NoMission, 717)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CheckConvertProfession, MIS_HUNTER)
    MisBeginAction(AddMission, 717)
    MisBeginAction(SetFlag, 717, 1)
    MisBeginAction(AddTrigger, 7171, TE_KILL, 240, 10)
    MisBeginAction(AddTrigger, 7172, TE_GETITEM, 3122, 1)
    MisBeginAction(AddTrigger, 7173, TE_KILL, 238, 10)
    MisCancelAction(ClearMission, 717)

    MisNeed(MIS_NEED_KILL, 240, 10, 10, 10)
    MisNeed(MIS_NEED_ITEM, 3122, 1, 20, 1)
    MisNeed(MIS_NEED_KILL, 238, 10, 30, 10)

    MisPrize(MIS_PRIZE_ITEM, 3955, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Friend, you have done well!<n><t>You have passed my test. This is the <rHunter Manual> that all hunter must have.<n><t>Keep it well. When you reached <pLv10>, come to me for class advancement.")
    MisHelpTalk("<t>You have not met the requirements. Its not so easy to become a Hunter.")
    MisResultCondition(HasMission, 717)
    MisResultCondition(HasItem, 3122, 1)
    MisResultCondition(HasFlag, 717, 19)
    MisResultCondition(HasFlag, 717, 39)
    MisResultAction(TakeItem, 3122, 1)
    MisResultAction(ClearMission, 717)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 240)
    TriggerAction(1, AddNextFlag, 717, 10, 10)
    RegCurTrigger(7171)
    InitTrigger()
    TriggerCondition(1, IsItem, 3122)
    TriggerAction(1, AddNextFlag, 717, 20, 1)
    RegCurTrigger(7172)
    InitTrigger()
    TriggerCondition(1, IsMonster, 238)
    TriggerAction(1, AddNextFlag, 717, 30, 10)
    RegCurTrigger(7173)

    DefineMission(757, "Sharpshooter Range", 741)

    MisBeginTalk("<t>To become a <bSharpshooter> is simple. Go to <pSkeleton Haven>, defeat 10 <rSkeletal Archers> and bring back 5 <yOld Quivers>. Also, kill 10 <rNinja Moles> in <pAscaron>'s <pAbandoned Mine> and bring back 5 <rBroken Ninja Swords>.")
    MisBeginCondition(NoRecord, 741)
    MisBeginCondition(NoMission, 741)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(PfEqual, 2)
    MisBeginCondition(CheckConvertProfession, MIS_GUNMAN)
    MisBeginAction(AddMission, 741)
    MisBeginAction(AddTrigger, 7411, TE_KILL, 269, 10)
    MisBeginAction(AddTrigger, 7412, TE_KILL, 243, 10)
    MisBeginAction(AddTrigger, 7413, TE_GETITEM, 4362, 5)
    MisBeginAction(AddTrigger, 7414, TE_GETITEM, 4367, 5)
    MisCancelAction(ClearMission, 741)

    MisNeed(MIS_NEED_KILL, 269, 10, 10, 10)
    MisNeed(MIS_NEED_KILL, 243, 10, 20, 10)
    MisNeed(MIS_NEED_ITEM, 4362, 5, 30, 5)
    MisNeed(MIS_NEED_ITEM, 4367, 5, 40, 5)

    MisResultTalk("<t>Congratulations! You have successfully become a qualified <bSharpshooter>!<n><t>Remember! The key to victory is to remain calm in all circumstances.")
    MisHelpTalk("<t>Why? No confidence in yourself?")
    MisResultCondition(HasMission, 741)
    MisResultCondition(HasFlag, 741, 19)
    MisResultCondition(HasFlag, 741, 29)
    MisResultCondition(HasItem, 4362, 5)
    MisResultCondition(HasItem, 4367, 5)
    MisResultAction(TakeItem, 4362, 5)
    MisResultAction(TakeItem, 4367, 5)
    MisResultAction(ClearMission, 741)
    MisResultAction(SetRecord, 741)
    MisResultAction(SetProfession, 12)
	MisResultAction(GiveItem, 15072, 1, 4, 0)
	MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 269)
    TriggerAction(1, AddNextFlag, 741, 10, 10)
    RegCurTrigger(7411)
    InitTrigger()
    TriggerCondition(1, IsMonster, 243)
    TriggerAction(1, AddNextFlag, 741, 20, 10)
    RegCurTrigger(7412)
    InitTrigger()
    TriggerCondition(1, IsItem, 4362)
    TriggerAction(1, AddNextFlag, 741, 30, 5)
    RegCurTrigger(7413)
    InitTrigger()
    TriggerCondition(1, IsItem, 4367)
    TriggerAction(1, AddNextFlag, 741, 30, 5)
    RegCurTrigger(7414)
end
RobinMission034()

function RobinMission035()
    DefineMission(730, "Emergency", 718)

    MisBeginTalk("<t>It's starting to get dangerous around here. There has been reports of a huge swarm of <rMini Bees>. Many have been stung, can you help us to slay 10 <rMini Bees>?<n><t>There'll be a reward.")
    MisBeginCondition(NoMission, 718)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 718)
    MisBeginAction(SetFlag, 718, 1)
    MisBeginAction(AddTrigger, 7181, TE_KILL, 206, 10)
    MisCancelAction(ClearMission, 718)

    MisNeed(MIS_NEED_KILL, 206, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 50, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>This is good! You are truly a friend. With your help, I will not get punish to wash the socks of the whole battalion again.")
    MisHelpTalk("<t>Whats wrong? Its only 10 Mini Bees. You can do it! Go now!")
    MisResultCondition(HasMission, 718)
    MisResultCondition(HasFlag, 718, 19)
    MisResultAction(AddExp, 75, 125)
    MisResultAction(ClearMission, 718)

    InitTrigger()
    TriggerCondition(1, IsMonster, 206)
    TriggerAction(1, AddNextFlag, 718, 10, 10)
    RegCurTrigger(7181)

    DefineMission(736, "Shroom Population", 724)

    MisBeginTalk("<t>I don't know which nuisance raise so many <bGreedy Shroom> ouside the city, now they have multiplied beyond control. Even though I love drinking mushroom soup, no one is willing to bitten by those <bGreedy Shroom>.<n><t>I am short of manpower here so I have to bother you to go to <j(2220, 2564)> and eradicate 8 <rGreedy Shrooms>. Please help out! Moreover if this matter isn't solved, we can't carry on to clear other tasks.")
    MisBeginCondition(NoMission, 724)
    MisBeginCondition(NoMission, 718)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 724)
    MisBeginAction(SetFlag, 724, 1)
    MisBeginAction(AddTrigger, 7241, TE_KILL, 184, 15)
    MisCancelAction(ClearMission, 724)

    MisNeed(MIS_NEED_KILL, 184, 15, 10, 15)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Is it done?<n><t>You have done a great job! My workload is now lessened thanks for your help.")
    MisHelpTalk("<t>Its only 15 Greedy Shrooms!<n><t>All depend on you now!")
    MisResultCondition(HasMission, 724)
    MisResultCondition(HasFlag, 724, 24)
    MisResultAction(AddExp, 125, 175)
    MisResultAction(ClearMission, 724)

    InitTrigger()
    TriggerCondition(1, IsMonster, 184)
    TriggerAction(1, AddNextFlag, 724, 10, 15)
    RegCurTrigger(7241)

    DefineMission(737, "Conservation Activity", 725)

    MisBeginTalk("<t>Have you noticed that the greenery surrounding Argent City has been reduced considerably. I heard from that captain that it was due to the voracious appetite of the <rGrass Tortoise> gathering near the city. Please help me kill 10 Grass Tortoise at <j(2057, 2564)>.<n><t>I will give you some reward once it is done and by the way, let me give you some warning. These grass tortoise might be slow but they do have high defence so this task is harder than the ones previously. However it should be suitable for your level.")
    MisBeginCondition(NoMission, 725)
    MisBeginCondition(NoMission, 724)
    MisBeginCondition(NoMission, 718)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 725)
    MisBeginAction(SetFlag, 725, 1)
    MisBeginAction(AddTrigger, 7251, TE_KILL, 119, 10)
    MisCancelAction(ClearMission, 725)

    MisNeed(MIS_NEED_KILL, 119, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 150, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Completed?<n><t>Well done! Although the tortoise are pitiful, they need to be killed for our survival...")
    MisHelpTalk("<t>How is it going? It's 10 Grass Tortoise!<n><t>You should be able to find them at <j(2057, 2564)>.")
    MisResultCondition(HasMission, 725)
    MisResultCondition(HasFlag, 725, 19)
    MisResultAction(AddExp, 175, 250)
    MisResultAction(ClearMission, 725)

    InitTrigger()
    TriggerCondition(1, IsMonster, 119)
    TriggerAction(1, AddNextFlag, 725, 10, 10)
    RegCurTrigger(7251)
end
RobinMission035()

function RobinMission036()
    DefineMission(717, "Physician's Greetings", 709, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t> You say you\'re new around here? No wonder I haven\'t seen you in Shaitan before. Welcome! I am <bPhysician - Shala>. Take these <rApples>, they\'re free this time but next time you\'ll have to buy them from me.<n><t>Now return to <bNewbie Guide Resline> at <j(876, 3572)>.<n><t>(Shala has given you some "Apples". Place in your shortcut which is labeled by F1 - F12 to use it)')
    MisResultCondition(NoRecord, 709)
    MisResultCondition(HasMission, 709)
    MisResultCondition(NoFlag, 709, 10)
    MisResultCondition(HasItem, 3958, 1)
    MisResultAction(TakeItem, 3958, 1)
    MisResultAction(SetFlag, 709, 10)
    MisResultAction(GiveItem, 1847, 10, 4)
    MisResultBagNeed(1)

    DefineMission(734, "Grafting Experiment", 722)

    MisBeginTalk("<t>Want to see cactus bloom? I planned to carry out an experiment so I need your help in collecting 5 <rCactus Hairball> and 5 <rSeeds>.<n><t>They can be found on Melons and Mystic Shrubs.")
    MisBeginCondition(NoMission, 722)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 722)
    MisBeginAction(SetFlag, 722, 1)
    MisBeginAction(AddTrigger, 7221, TE_GETITEM, 1691, 5)
    MisBeginAction(AddTrigger, 7222, TE_GETITEM, 1597, 5)
    MisCancelAction(ClearMission, 722)

    MisNeed(MIS_NEED_ITEM, 1691, 5, 10, 5)
    MisNeed(MIS_NEED_ITEM, 1597, 5, 20, 5)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Good! Now I can start my experiment. Come back next year to see my work!")
    MisHelpTalk("<t>Hmm, still unable to get the items I needed? Its 5 <rCactus Hairballs> and 5 <rSeeds>.")
    MisResultCondition(HasMission, 722)
    MisResultCondition(HasItem, 1691, 5)
    MisResultCondition(HasItem, 1597, 5)
    MisResultAction(TakeItem, 1691, 5)
    MisResultAction(TakeItem, 1597, 5)
    MisResultAction(AddExp, 40, 70)
    MisResultAction(ClearMission, 722)

    InitTrigger()
    TriggerCondition(1, IsItem, 1691)
    TriggerAction(1, AddNextFlag, 722, 10, 5)
    RegCurTrigger(7221)
    InitTrigger()
    TriggerCondition(1, IsItem, 1597)
    TriggerAction(1, AddNextFlag, 722, 20, 5)
    RegCurTrigger(7222)

    DefineMission(744, "Fake a Gift", 732)

    MisBeginTalk("<t>Listen to my proposal: Lately, the physician of Icicle City has been collecting the sweat of animals. I want to play a joke on him so I need some materials to make the fakes.<n><t>Help me collect 5 vials of <rMurky Water> and 5 <rGlass>. They be found on <rBaby Scorpion> at <j(1184, 3557)>. Just take this as a special request, haha!")
    MisBeginCondition(NoMission, 732)
    MisBeginCondition(NoMission, 722)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 732)
    MisBeginAction(SetFlag, 732, 1)
    MisBeginAction(AddTrigger, 7321, TE_GETITEM, 1648, 5)
    MisBeginAction(AddTrigger, 7322, TE_GETITEM, 1777, 2)
    MisCancelAction(ClearMission, 732)

    MisNeed(MIS_NEED_ITEM, 1648, 5, 10, 5)
    MisNeed(MIS_NEED_ITEM, 1777, 2, 20, 2)

    MisPrize(MIS_PRIZE_MONEY, 200, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Haha! That's great! I'll definitely be able to fool him using these.")
    MisHelpTalk("<t>It can't be! You still haven't found anything? Please hurry!")
    MisResultCondition(HasMission, 732)
    MisResultCondition(HasItem, 1648, 5)
    MisResultCondition(HasItem, 1777, 2)
    MisResultAction(TakeItem, 1648, 5)
    MisResultAction(TakeItem, 1777, 2)
    MisResultAction(AddExp, 70, 95)
    MisResultAction(ClearMission, 732)

    InitTrigger()
    TriggerCondition(1, IsItem, 1648)
    TriggerAction(1, AddNextFlag, 732, 10, 5)
    RegCurTrigger(7321)
    InitTrigger()
    TriggerCondition(1, IsItem, 1777)
    TriggerAction(1, AddNextFlag, 732, 20, 2)
    RegCurTrigger(7322)

    DefineMission(745, "Herbs Gathering", 733)

    MisBeginTalk("<t>This time round, I need to collect several medicinal herbs, especially the following 2 which I am having an acute shortage. Please locate for me 5 <rCactus Blossoms> and 2 <rRed Dates>. They definitely can be found on <rCactus> nearby at <j(1031, 3556)>. You should be able to defeat them with ease.<n><t>Oh! Those fakes managed to deceive the physician in Icicle City, it was quite amusing.")
    MisBeginCondition(NoMission, 733)
    MisBeginCondition(NoMission, 732)
    MisBeginCondition(NoMission, 722)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 733)
    MisBeginAction(SetFlag, 733, 1)
    MisBeginAction(AddTrigger, 7331, TE_GETITEM, 1692, 5)
    MisBeginAction(AddTrigger, 7332, TE_GETITEM, 3117, 2)
    MisCancelAction(ClearMission, 733)

    MisNeed(MIS_NEED_ITEM, 1692, 5, 10, 5)
    MisNeed(MIS_NEED_ITEM, 3117, 2, 20, 2)

    MisPrize(MIS_PRIZE_MONEY, 300, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>I must thanks you. I can start decocting finally.")
    MisHelpTalk("<t>I only need these as ingredient! Don't let me down!")
    MisResultCondition(HasMission, 733)
    MisResultCondition(HasItem, 1692, 5)
    MisResultCondition(HasItem, 3117, 2)
    MisResultAction(TakeItem, 1692, 5)
    MisResultAction(TakeItem, 3117, 2)
    MisResultAction(AddExp, 95, 125)
    MisResultAction(ClearMission, 733)

    InitTrigger()
    TriggerCondition(1, IsItem, 1692)
    TriggerAction(1, AddNextFlag, 733, 10, 5)
    RegCurTrigger(7331)
    InitTrigger()
    TriggerCondition(1, IsItem, 3117)
    TriggerAction(1, AddNextFlag, 733, 20, 2)
    RegCurTrigger(7332)
end
RobinMission036()

function RobinMission037()
    DefineMission(731, "Cactus Invasion", 719)

    MisBeginTalk("<t>Ah! Now the hills surrounding Shaitan city are overflowing with cactus. In my area especially, there are unusually plenty of them. Can you do me a favour? Defeat 15 <rCactus> at <j(1031, 3556)> and seems to me you are the most suited for this task.")
    MisBeginCondition(NoMission, 719)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 719)
    MisBeginAction(SetFlag, 719, 1)
    MisBeginAction(AddTrigger, 7191, TE_KILL, 95, 15)
    MisCancelAction(ClearMission, 719)

    MisNeed(MIS_NEED_KILL, 95, 15, 10, 15)

    MisPrize(MIS_PRIZE_MONEY, 50, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Thank God! You've exterminated quite a lot of them.")
    MisHelpTalk("<t>What is wrong? 15 Cactus should not pose a threat to you!")
    MisResultCondition(HasMission, 719)
    MisResultCondition(HasFlag, 719, 24)
    MisResultAction(AddExp, 75, 125)
    MisResultAction(ClearMission, 719)

    InitTrigger()
    TriggerCondition(1, IsMonster, 95)
    TriggerAction(1, AddNextFlag, 719, 10, 15)
    RegCurTrigger(7191)

    DefineMission(740, "Kicking Monster", 728)

    MisBeginTalk("<t>A lot of people have been complaining of a monster that has been kicking people. Many have been injured by this monster. I did some investigation and found out that <rHumpy Camel> did it. Can you punish the camel for us? Defeat 10 <rHumpy Camels> please.")
    MisBeginCondition(NoMission, 728)
    MisBeginCondition(NoMission, 719)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 728)
    MisBeginAction(SetFlag, 728, 1)
    MisBeginAction(AddTrigger, 7281, TE_KILL, 48, 10)
    MisCancelAction(ClearMission, 728)

    MisNeed(MIS_NEED_KILL, 48, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great. There should not be any further complaint.<n><t>Those camels are really getting out of hand.")
    MisHelpTalk("<t>What? You're not able to teach them a lesson? Try harder.")
    MisResultCondition(HasMission, 728)
    MisResultCondition(HasFlag, 728, 19)
    MisResultAction(AddExp, 125, 175)
    MisResultAction(ClearMission, 728)

    InitTrigger()
    TriggerCondition(1, IsMonster, 48)
    TriggerAction(1, AddNextFlag, 728, 10, 10)
    RegCurTrigger(7281)

    DefineMission(741, "Scorpion Crisis", 729)

    MisBeginTalk("<t>Oh! You leveled up!<n><t>I can now assign you new task according to your abilities.<n><t>Although the case of Humpy Camel hurting people is uncommon now, there is a sudden influx of big scorpions. They are much more powerful than the little scorpions. To be stung by them can be a life threatening situation. Its too scary!<n><t>Please help me kill 10 <Big Scorpions>! You can easily see them at <j(1184, 3557)> but beware of their agile movements, take care!")
    MisBeginCondition(NoMission, 729)
    MisBeginCondition(NoMission, 728)
    MisBeginCondition(NoMission, 719)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 729)
    MisBeginAction(SetFlag, 729, 1)
    MisBeginAction(AddTrigger, 7291, TE_KILL, 247, 10)
    MisCancelAction(ClearMission, 729)

    MisNeed(MIS_NEED_KILL, 247, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 150, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Hoho! Looks like I found the right person for this job!")
    MisHelpTalk("<t>What is wrong? You are unable to defeat those scorpions? Bring some potions then.")
    MisResultCondition(HasMission, 729)
    MisResultCondition(HasFlag, 729, 19)
    MisResultAction(AddExp, 175, 250)
    MisResultAction(ClearMission, 729)

    InitTrigger()
    TriggerCondition(1, IsMonster, 247)
    TriggerAction(1, AddNextFlag, 729, 10, 10)
    RegCurTrigger(7291)
end
RobinMission037()

function RobinMission038()
    DefineMission(732, "Playful Squidy", 720)

    MisBeginTalk("<t>Lately, these Snow Squidys have gotten more and more mischievous. When I was out of the city, they ambushed me as a joke.<n><t>Please teach them a lesson by defeating 15 <rSnow Squidys>.<n><t>It should be easy for you.")
    MisBeginCondition(NoMission, 720)
    MisBeginCondition(LvCheck, ">", 4)
    MisBeginCondition(LvCheck, "<", 7)
    MisBeginAction(AddMission, 720)
    MisBeginAction(SetFlag, 720, 1)
    MisBeginAction(AddTrigger, 7201, TE_KILL, 235, 15)
    MisCancelAction(ClearMission, 720)

    MisNeed(MIS_NEED_KILL, 235, 15, 10, 15)

    MisPrize(MIS_PRIZE_MONEY, 50, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Good. Now I can have a good rest.")
    MisHelpTalk("<t>What? You've not done anything yet? This shouldn't be much of a problem to you.")
    MisResultCondition(HasMission, 720)
    MisResultCondition(HasFlag, 720, 24)
    MisResultAction(AddExp, 75, 125)
    MisResultAction(ClearMission, 720)

    InitTrigger()
    TriggerCondition(1, IsMonster, 235)
    TriggerAction(1, AddNextFlag, 720, 10, 15)
    RegCurTrigger(7201)

    DefineMission(742, "There's never too much", 730)

    MisBeginTalk("<t>It is a tough time for the people in Icicle City, so the task I need to entrust you is this: The <rSnowy Piglets> consume huge amount of food and they are getting out of control. Please go to <j(1179, 371)> and destroy 10 <rSnowy Piglets>.")
    MisBeginCondition(NoMission, 730)
    MisBeginCondition(NoMission, 720)
    MisBeginCondition(LvCheck, ">", 6)
    MisBeginCondition(LvCheck, "<", 8)
    MisBeginAction(AddMission, 730)
    MisBeginAction(SetFlag, 730, 1)
    MisBeginAction(AddTrigger, 7301, TE_KILL, 239, 10)
    MisCancelAction(ClearMission, 730)

    MisNeed(MIS_NEED_KILL, 239, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 100, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Done? Thank you! Those piglets have a big appetite. Seems like I need to have BBQ pork every day.")
    MisHelpTalk("<t>What? Still nothing?")
    MisResultCondition(HasMission, 730)
    MisResultCondition(HasFlag, 730, 19)
    MisResultAction(AddExp, 125, 175)
    MisResultAction(ClearMission, 730)

    InitTrigger()
    TriggerCondition(1, IsMonster, 239)
    TriggerAction(1, AddNextFlag, 730, 10, 10)
    RegCurTrigger(7301)

    DefineMission(743, "Wrong Transfer", 731)

    MisBeginTalk("<t>Hoho, I have good news! It seems a pack of Little Deer have mistakenly migrated near to the Icicle City. With the current situation at Icicle City, this pack of deers is like a delicacy given by the gods. I want you to travel with haste to <j(1164 ,305)>. Hunt and bring back 12 <rLittle Deers> so we can have a feast tonight.")
    MisBeginCondition(NoMission, 731)
    MisBeginCondition(NoMission, 720)
    MisBeginCondition(NoMission, 730)
    MisBeginCondition(LvCheck, ">", 7)
    MisBeginCondition(LvCheck, "<", 9)
    MisBeginAction(AddMission, 731)
    MisBeginAction(SetFlag, 731, 1)
    MisBeginAction(AddTrigger, 7311, TE_KILL, 238, 10)
    MisCancelAction(ClearMission, 731)

    MisNeed(MIS_NEED_KILL, 238, 10, 10, 10)

    MisPrize(MIS_PRIZE_MONEY, 150, 1)
    MisPrizeSelAll()

    MisResultTalk("<t>Good! You have done it!<n><t>I will put aside my duties for those delicious meat! Hehe...")
    MisHelpTalk("<t>What's up? You can't even defeat those weak enemies? Go quickly!<n><t>I'm still waiting to start cooking!")
    MisResultCondition(HasMission, 731)
    MisResultCondition(HasFlag, 731, 19)
    MisResultAction(AddExp, 175, 250)
    MisResultAction(ClearMission, 731)

    InitTrigger()
    TriggerCondition(1, IsMonster, 238)
    TriggerAction(1, AddNextFlag, 731, 10, 10)
    RegCurTrigger(7311)
end
RobinMission038()

function RobinMission039()
    DefineMission(50, "Send a letter to Marcusa", 50)

    MisBeginTalk("<t>There's nothing more for me to help you with.<n><t>Take this letter to <bMarcusa>, the patroller and see if he has anymore tasks for you. He can be found outside Argent City at <j(2065, 2732)>.")
    MisBeginCondition(HasRecord, 704)
    MisBeginCondition(NoRecord, 50)
    MisBeginCondition(NoMission, 50)
    MisBeginAction(AddMission, 50)
    MisBeginAction(GiveItem, 4111, 1, 4)
    MisCancelAction(ClearMission, 50)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Patrol - Marcusa")

    MisHelpTalk("<t><bMarcusa> is near <j(2065, 2732)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(51, "Letter for Ditto", 51)

    MisBeginTalk("<t>If you like collecting stuff more than fighting, give this letter to the <bPhysician Ditto> at <j(2250, 2770)>. He has a task for you.")
    MisBeginCondition(HasRecord, 704)
    MisBeginCondition(NoRecord, 51)
    MisBeginCondition(NoMission, 51)
    MisBeginAction(AddMission, 51)
    MisBeginAction(GiveItem, 4112, 1, 4)
    MisCancelAction(ClearMission, 51)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to Physician Ditto")

    MisHelpTalk("<t>Why haven't you passed the letter to <bDitto>? He is nearby at <j(2250, 2770)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(52, "Send a letter to Marcusa", 50, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>It\'s good that you are here, I could use a helping hand.<n><t>Also, take note of your HP bar while in battle. That\'s right, the red bar shows your HP level. You will die when it reaches 0. Beware!<n><t>Beside eating "Apples", "Cakes" or other recovery potion, you can press the "Insert" key to increase HP/SP recovery rate.'
    )
    MisResultCondition(NoRecord, 50)
    MisResultCondition(HasMission, 50)
    MisResultCondition(HasItem, 4111, 1)
    MisResultAction(TakeItem, 4111, 1)
    MisResultAction(ClearMission, 50)
    MisResultAction(SetRecord, 50)
    MisResultAction(AddExp, 50, 50)

    DefineMission(53, "Emergency", 52)

    MisBeginTalk("<t>It is starting to get dangerous around here. There has been reports of a huge swarm of <rMini Bees>. Many have been stung, can you help us to slay 6 <rMini Bees>? There will be a reward.")
    MisBeginCondition(HasRecord, 50)
    MisBeginCondition(NoMission, 52)
    MisBeginCondition(NoRecord, 52)
    MisBeginAction(AddMission, 52)
    MisBeginAction(AddTrigger, 521, TE_KILL, 206, 6)
    MisCancelAction(ClearMission, 52)

    MisNeed(MIS_NEED_DESP,"Kill 6 <rMini Bee> and look for <bMarcusa>.")
    MisNeed(MIS_NEED_KILL, 206, 6, 10, 6)

    MisResultTalk("<t>This is good! You are truly a friend. With your help, I will not get punish to wash the socks of the whole battalion again.")
    MisHelpTalk("<t>What is the matter? It's only 6 Mini Bees. It should not pose a problem to you. Please hurry up and get rid of them.")
    MisResultCondition(HasMission, 52)
    MisResultCondition(HasFlag, 52, 15)
    MisResultAction(AddExp, 150, 150)
    MisResultAction(ClearMission, 52)
    MisResultAction(SetRecord, 52)

    InitTrigger()
    TriggerCondition(1, IsMonster, 206)
    TriggerAction(1, AddNextFlag, 52, 10, 6)
    RegCurTrigger(521)

    DefineMission(54, "Letter for Salvier", 53)

    MisBeginTalk("<t>The Mini Bees situation has gotten better. I have a report here regarding the whole incident.<n><t>Can I trust you to deliver this report to Argent's Secretary, <bSalvier> at <j(2219, 2749)>?")
    MisBeginCondition(HasRecord, 52)
    MisBeginCondition(NoRecord, 53)
    MisBeginCondition(NoMission, 53)
    MisBeginAction(AddMission, 53)
    MisBeginAction(GiveItem, 4113, 1, 4)
    MisCancelAction(ClearMission, 53)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to Salvier in Argent")

    MisHelpTalk("<t>Why haven't you gone to the secretary?<n><t><bSalvier's> office can be located at <j(2219, 2749)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(55, "Letter for Salvier", 53, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>According to <bMarcusa's> report, it states that you played a big role in ridding us of the bees. Well done!")
    MisResultCondition(NoRecord, 53)
    MisResultCondition(HasMission, 53)
    MisResultCondition(HasItem, 4113, 1)
    MisResultAction(TakeItem, 4113, 1)
    MisResultAction(ClearMission, 53)
    MisResultAction(SetRecord, 53)
    MisResultAction(AddExp, 80, 80)

    DefineMission(56, "Shroom Population", 54)

    MisBeginTalk("<t>I don't know which nuisance raise so many <rGreedy Shrooms> ouside the city. Now they have multiplied beyond control. Even though I love drinking mushroom soup, no one is willing to bitten by those <rGreedy Shrooms>.<n><t>I am short of manpower here so I have to trouble you to go to <j(2220, 2564)> and eradicate 8 <rGreedy Shrooms>. Please help out as, if this matter isn't solved, we can't carry on to clear other tasks.")
    MisBeginCondition(HasRecord, 53)
    MisBeginCondition(NoMission, 54)
    MisBeginCondition(NoRecord, 54)
    MisBeginAction(AddMission, 54)
    MisBeginAction(AddTrigger, 541, TE_KILL, 184, 8)
    MisCancelAction(ClearMission, 54)

    MisNeed(MIS_NEED_DESP,"Kill 8 <rGreedy Shroom> and report back to <bSalvier> in Argent City at <j(2219, 2749)>.")
    MisNeed(MIS_NEED_KILL, 184, 8, 10, 8)

    MisPrize(MIS_PRIZE_ITEM, 9, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Oh its done? You are great!<n><t>This is good. With your help, our jobs will be easier.")
    MisHelpTalk("<t>How is it? Only 8 Greedy Shrooms!<n><t>Go for it!")
    MisResultCondition(HasMission, 54)
    MisResultCondition(HasFlag, 54, 17)
    MisResultAction(AddExp, 250, 250)
    MisResultAction(ClearMission, 54)
    MisResultAction(SetRecord, 54)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 184)
    TriggerAction(1, AddNextFlag, 54, 10, 8)
    RegCurTrigger(541)

    DefineMission(57, "Letter for Goldie", 55)

    MisBeginTalk("<t>I have an official letter here to deliver to <bArgent's Blacksmith Goldie>.<n><t>You should know him. Go to <j(2193, 2706)> and look for him now.")
    MisBeginCondition(HasRecord, 54)
    MisBeginCondition(NoRecord, 55)
    MisBeginCondition(NoMission, 55)
    MisBeginAction(AddMission, 55)
    MisBeginAction(GiveItem, 4114, 1, 4)
    MisCancelAction(ClearMission, 55)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Blacksmith Goldie")

    MisHelpTalk("<t>Why haven't you given the letter to <bGoldie>? He is at <j(2193, 2706)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(58, "Letter for Goldie", 55, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, it's a letter from <bSalvier>.<n><t>I didn't expect him to dispatch someone like you to deliver this.")
    MisResultCondition(NoRecord, 55)
    MisResultCondition(HasMission, 55)
    MisResultCondition(HasItem, 4114, 1)
    MisResultAction(TakeItem, 4114, 1)
    MisResultAction(ClearMission, 55)
    MisResultAction(SetRecord, 55)
    MisResultAction(AddExp, 120, 120)

    DefineMission(59, "Conservation Activity", 56)

    MisBeginTalk("<t>Have you noticed that the greenery surrounding Argent City has been reduced considerably. I heard from that captain that it was due to the voracious appetite of the <rGrass Tortoise> gathering near the city. Please help me kill 12 <rGrass Tortoise> at <j(2057, 2564)>.<n><t>I will give you some reward once it is done and by the way, let me give you some warning. These grass tortoise might be slow but they do have high defence so this task is harder than the ones previously. However it should be suitable for your level.")
    MisBeginCondition(HasRecord, 55)
    MisBeginCondition(NoMission, 56)
    MisBeginCondition(NoRecord, 56)
    MisBeginAction(AddMission, 56)
    MisBeginAction(AddTrigger, 561, TE_KILL, 119, 12)
    MisCancelAction(ClearMission, 56)

    MisNeed(MIS_NEED_DESP,"Kill 12 <rGrass Tortoise> and return to <bGoldie> in Argent City.")
    MisNeed(MIS_NEED_KILL, 119, 12, 10, 12)

    MisPrize(MIS_PRIZE_ITEM, 4309, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Completed? Well done! Although the tortoise are pitiful, they need to be killed for our survival.")
    MisHelpTalk("<t>Hey! It is only12 grass tortoise!<n><t>You should be at <j(2057, 2564)> looking for them.")
    MisResultCondition(HasMission, 56)
    MisResultCondition(HasFlag, 56, 21)
    MisResultAction(ClearMission, 56)
    MisResultAction(AddExp, 400, 400)
    MisResultAction(SetRecord, 56)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 119)
    TriggerAction(1, AddNextFlag, 56, 10, 12)
    RegCurTrigger(561)

    DefineMission(60, "Letter for Senna", 57)

    MisBeginTalk("<t>Although we have asked you for help with many of our things, you have also progress much.<n><t>Please take this letter to <bNewbie Guide Senna> at <j(2223, 2785)>.<n><t>After locating her, she'll give you some new instructions.")
    MisBeginCondition(HasRecord, 56)
    MisBeginCondition(NoRecord, 57)
    MisBeginCondition(NoMission, 57)
    MisBeginAction(AddMission, 57)
    MisBeginAction(GiveItem, 4115, 1, 4)
    MisCancelAction(ClearMission, 57)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Argent's <bNewbie Guide - Senna>.")

    MisHelpTalk("<t><bSenna> is at <j(2223, 2785)>. Go now.")
    MisResultCondition(AlwaysFailure)

    DefineMission(61, "Letter for Senna", 57, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Argent City.")
    MisResultCondition(NoRecord, 57)
    MisResultCondition(HasMission, 57)
    MisResultCondition(HasItem, 4115, 1)
    MisResultAction(TakeItem, 4115, 1)
    MisResultAction(ClearMission, 57)
    MisResultAction(SetRecord, 57)
    MisResultAction(AddExp, 200, 200)

    DefineMission(62, "Swordsman Promotion", 58)

    MisBeginTalk("<t>Its time for you to choose what you want to be. If you want to be a Swordsman, please go to Argent City and look for Castle Guard, <bPeter>. He can be found at <j(2192, 2767)>. He will test you accordingly.<n><t>Swordsman is the strongest melee class. They can advance to be an agile Crusader and a highly defensive Champion.")
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CTypeCheck, {1,2,3})
    MisBeginCondition(NoRecord, 58)
    MisBeginCondition(NoRecord, 59)
    MisBeginCondition(NoRecord, 60)
    MisBeginCondition(NoRecord, 61)
    MisBeginCondition(NoMission, 58)
    MisBeginCondition(NoMission, 59)
    MisBeginCondition(NoMission, 60)
    MisBeginCondition(NoMission, 61)
    MisBeginAction(AddMission, 58)
    MisBeginAction(GiveItem, 4116, 1, 4)
    MisCancelAction(ClearMission, 58)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to Peter in Argent City.")

    MisHelpTalk("<t>If you want to be a Swordsman, go and speak to Peter.")
    MisResultCondition(AlwaysFailure)

    DefineMission(63, "Hunter Promotion", 59)

    MisBeginTalk("<t>Now it is time for you to decide a path of your choice. If you wish to be a hunter, you will have to pass this letter to <bRay> at Icicle City <j(1365,70)>. He will give you a trial to test your ability.<n><t>Hunter is a long ranged character class that uses either bow or firegun, suitable for Lance and Phyllis.")
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CTypeCheck, {1,3})
    MisBeginCondition(NoRecord, 58)
    MisBeginCondition(NoRecord, 59)
    MisBeginCondition(NoRecord, 60)
    MisBeginCondition(NoRecord, 61)
    MisBeginCondition(NoMission, 58)
    MisBeginCondition(NoMission, 59)
    MisBeginCondition(NoMission, 60)
    MisBeginCondition(NoMission, 61)
    MisBeginAction(AddMission, 59)
    MisBeginAction(GiveItem, 4117, 1, 4)
    MisCancelAction(ClearMission, 59)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to Ray in Icicle City")

    MisHelpTalk("<t>If you want to be a Hunter, go and speak to <bRay>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(64, "Herbalist Promotion", 60)

    MisBeginTalk("<t>It's time for you to choose what you want to be. If you want to be a Herbalist, please go to Shaitan City and look for <bHigh Priest Gannon>. He can be found at <j(862, 3500)>. He'll trial you accordingly.<n><t>Herbalist use their ability to assist others in battle and can advance to either Cleric or Seal Master.")
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CTypeCheck, {3,4})
    MisBeginCondition(NoRecord, 58)
    MisBeginCondition(NoRecord, 59)
    MisBeginCondition(NoRecord, 60)
    MisBeginCondition(NoRecord, 61)
    MisBeginCondition(NoMission, 58)
    MisBeginCondition(NoMission, 59)
    MisBeginCondition(NoMission, 60)
    MisBeginCondition(NoMission, 61)
    MisBeginAction(AddMission, 60)
    MisBeginAction(GiveItem, 4118, 1, 4)
    MisCancelAction(ClearMission, 60)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Bring this letter to <bHigh Priest Gannon> in Shaitan City.")

    MisHelpTalk("<t>If you want to become a Herbalist, look for <bHigh Priest Gannon>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(65, "Explorer Promotion", 61)

    MisBeginTalk("<t>Its time for you to choose what you want to be. If you want to be an Explorer, please go to Argent City and look for <bLittle Daniel> at <j(2193, 2730)>. Bring this letter to him.<n><t>Explorer uses coral to channel their energy and can advance to Voyager. It is the strongest class in the sea.")
    MisBeginCondition(LvCheck, ">", 8)
    MisBeginCondition(PfEqual, 0)
    MisBeginCondition(CTypeCheck, {1,3,4})
    MisBeginCondition(NoRecord, 58)
    MisBeginCondition(NoRecord, 59)
    MisBeginCondition(NoRecord, 60)
    MisBeginCondition(NoRecord, 61)
    MisBeginCondition(NoMission, 58)
    MisBeginCondition(NoMission, 59)
    MisBeginCondition(NoMission, 60)
    MisBeginCondition(NoMission, 61)
    MisBeginAction(AddMission, 61)
    MisBeginAction(GiveItem, 4119, 1, 4)
    MisCancelAction(ClearMission, 61)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Little Daniel")

    MisHelpTalk("<t>To become an Explorer, look for <bLittle Daniel> at <j(2193, 2730)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(66, "Swordsman Promotion", 58, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Since the Newbie Guide recommended you, you must have shown some potential. If you want to become a Swordsman, you'll have to finish the tasks I assign to you.")
    MisResultCondition(NoRecord, 58)
    MisResultCondition(HasMission, 58)
    MisResultCondition(HasItem, 4116, 1)
    MisResultAction(TakeItem, 4116, 1)
    MisResultAction(ClearMission, 58)
    MisResultAction(SetRecord, 58)

    DefineMission(67, "Swordsman Promotion", 62)

    MisBeginTalk("<t>So you want to be a Swordsman? You have some guts.<n><t>However courage alone is not enough. To become a Swordsman, you must prove that you have the required ability and intellect.<n><t>Go now to the <pOutskirt of Argent City> and defeat 12 <rPiglets> at <j(1950, 2563)> and then return to me.<n><t>The first test will be completed if you are able to do so.")
    MisBeginCondition(HasRecord, 58)
    MisBeginCondition(NoMission, 62)
    MisBeginCondition(NoRecord, 62)
    MisBeginAction(AddMission, 62)
    MisBeginAction(AddTrigger, 621, TE_KILL, 237, 12)
    MisCancelAction(ClearMission, 62)

    MisNeed(MIS_NEED_DESP,"Hunt 12 <rPiglets> and talk to <bPeter> in Argent City.")
    MisNeed(MIS_NEED_KILL, 237, 12, 10, 12)

    MisResultTalk("<t>You have done well.<n><t>You have passed the first test to becoming a Swordsman.")
    MisHelpTalk("<t>You have not met the requirements. Its not so easy to become a Swordsman.")
    MisResultCondition(HasMission, 62)
    MisResultCondition(HasFlag, 62, 21)
    MisResultAction(ClearMission, 62)
    MisResultAction(SetRecord, 62)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsMonster, 237)
    TriggerAction(1, AddNextFlag, 62, 10, 12)
    RegCurTrigger(621)

    DefineMission(68, "Swordsman Promotion", 63)

    MisBeginTalk("<t>Now, report to <bGeneral William> at <j(2277, 2831)> with this letter to prove that you have gotten pass the first part of the trial and he will give you the final quest to become a Swordsman.")
    MisBeginCondition(HasRecord, 62)
    MisBeginCondition(NoRecord, 63)
    MisBeginCondition(NoMission, 63)
    MisBeginAction(AddMission, 63)
    MisBeginAction(GiveItem, 4120, 1, 4)
    MisCancelAction(ClearMission, 63)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to General William of the Navy")

    MisHelpTalk("<t>Hurry up and go, you can do it!")
    MisResultCondition(AlwaysFailure)

    DefineMission(69, "Swordsman Promotion", 63, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, good. Another person interested to become a Swordsman.")
    MisResultCondition(NoRecord, 63)
    MisResultCondition(HasMission, 63)
    MisResultCondition(HasItem, 4120, 1)
    MisResultAction(TakeItem, 4120, 1)
    MisResultAction(ClearMission, 63)
    MisResultAction(SetRecord, 63)
    MisResultAction(AddExp, 100, 100)

    DefineMission(70, "Swordsman Promotion", 64)

    MisBeginTalk("<t>You wish to pass the test I have set?<n><t>Then go to the <pOutskirts of Argent City> and collect 3 <rCashmeres> and bring it back.<n><t>Once this task is completed, I shall admit that you are a qualifed Swordsman.<n><t>Oh by the way, the cashmere can be found on Cuddly Lambs at <j(2057, 2638)>.")
    MisBeginCondition(HasRecord, 63)
    MisBeginCondition(NoMission, 64)
    MisBeginCondition(NoRecord, 64)
    MisBeginAction(AddMission, 64)
    MisBeginAction(AddTrigger, 641, TE_GETITEM, 1678, 3)
    MisCancelAction(ClearMission, 64)

    MisNeed(MIS_NEED_DESP,"Collect 3 <rCashmeres> and talk to <bGeneral William> at <j(2277, 2831)>.")
    MisNeed(MIS_NEED_ITEM, 1678, 3, 10, 3)

    MisResultTalk("<t>You succeeded! I am really happy for you!")
    MisHelpTalk("<t>3 Cashmere. No more no less.")
    MisResultCondition(HasMission, 64)
    MisResultCondition(HasItem, 1678, 3)
    MisResultAction(TakeItem, 1678, 3)
    MisResultAction(ClearMission, 64)
    MisResultAction(SetRecord, 64)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsItem, 1678)
    TriggerAction(1, AddNextFlag, 64, 10, 3)
    RegCurTrigger(641)

    DefineMission(71, "Swordsman Promotion", 65)

    MisBeginTalk("<t>You've already proven your worth. Go and speak to <bPeter> and tell him that I've approved you to become a Swordsman. This <rCourage Certificate> will serve as proof.")
    MisBeginCondition(HasRecord, 64)
    MisBeginCondition(NoRecord, 65)
    MisBeginCondition(NoMission, 65)
    MisBeginAction(AddMission, 65)
    MisBeginAction(GiveItem, 3953, 1, 4)
    MisCancelAction(ClearMission, 65)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to <bPeter> in Argent City at <j(2192, 2767)>.")

    MisHelpTalk("<t>Why are you hesitating? Go and look for <bPeter>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(72, "Swordsman Promotion", 65, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Congratulations, you've obtained the Courage Certificate. You are now officially a Swordsman!<n><t>(You can now activate class quest at <bPeter>.<n><t>Also, buy your weapons from <bBlacksmith Goldie>, armor from <bTailor Nila> and skill books from <bGrocer Jimberry>).")
    MisResultCondition(NoRecord, 65)
    MisResultCondition(HasMission, 65)
    MisResultCondition(HasItem, 3953, 1)
    MisResultAction(TakeItem, 3953, 1)
    MisResultAction(ClearMission, 65)
    MisResultAction(SetRecord, 65)
    MisResultAction(AddExp, 100, 100)
    MisResultAction(ClearFightSkill, 475)
    MisResultAction(SetProfession, 1)
    MisResultAction(GiveItem, 1, 1, 4)
    MisResultAction(GiveItem, 3164, 1, 4)
    MisResultAction(GiveItem, 15073, 1, 4, 0)
    MisResultBagNeed(3)

    DefineMission(73, "Letter for Ditto", 51, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Its good that you are here, I could use a helping hand.")
    MisResultCondition(NoRecord, 51)
    MisResultCondition(HasMission, 51)
    MisResultCondition(HasItem, 4112, 1)
    MisResultAction(TakeItem, 4112, 1)
    MisResultAction(ClearMission, 51)
    MisResultAction(SetRecord, 51)
    MisResultAction(AddExp, 50, 50)

    DefineMission(74, "Decoction Recipe", 66)

    MisBeginTalk("<t>I have an inspiration to make a new kind of medicine suddenly. Sorry to trouble you but could you help me to collect 3 <rOctopus Inks>? These can be found by defeating <rLittle Squidys> near the sea. My new medicine all depends on you!")
    MisBeginCondition(HasRecord, 51)
    MisBeginCondition(NoMission, 66)
    MisBeginCondition(NoRecord, 66)
    MisBeginAction(AddMission, 66)
    MisBeginAction(AddTrigger, 661, TE_GETITEM, 1705, 3)
    MisCancelAction(ClearMission, 66)

    MisNeed(MIS_NEED_DESP,"Collect 3 vials of <rOctopus Ink> and report back to <bPhysician Ditto> at <j(2250, 2770)>.")
    MisNeed(MIS_NEED_ITEM, 1705, 3, 10, 3)

    MisResultTalk("<t>Very well. Since you have collected the nessaccery items, I can begin my research for a new potion.")
    MisHelpTalk("<t>What? Hurry up and collect those ingredients before I lose my inspiration!")
    MisResultCondition(HasMission, 66)
    MisResultCondition(HasItem, 1705, 3)
    MisResultAction(TakeItem, 1705, 3)
    MisResultAction(ClearMission, 66)
    MisResultAction(SetRecord, 66)
    MisResultAction(AddExp, 150, 150)

    InitTrigger()
    TriggerCondition(1, IsItem, 1705)
    TriggerAction(1, AddNextFlag, 66, 10, 3)
    RegCurTrigger(661)

    DefineMission(75, "Letter for Rouri", 67)

    MisBeginTalk("<t>My experiments have yet to yield any proper results.<n><t>Looks like I can't help you much. Give this letter to the chairman's assistant, <bRouri> at <j(2240, 2752)>, and see if there's anything you can help out with.")
    MisBeginCondition(HasRecord, 66)
    MisBeginCondition(NoRecord, 67)
    MisBeginCondition(NoMission, 67)
    MisBeginAction(AddMission, 67)
    MisBeginAction(GiveItem, 4121, 1, 4)
    MisCancelAction(ClearMission, 67)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Rouri")

    MisHelpTalk("<t>The chairman's assistant, <bRouri>, can be located at <j(2240, 2752)> in Argent City.")
    MisResultCondition(AlwaysFailure)

    DefineMission(76, "Letter for Rouri", 67, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>I see, so it was Ditto who recommended you to come here.")
    MisResultCondition(NoRecord, 67)
    MisResultCondition(HasMission, 67)
    MisResultCondition(HasItem, 4121, 1)
    MisResultAction(TakeItem, 4121, 1)
    MisResultAction(ClearMission, 67)
    MisResultAction(SetRecord, 67)
    MisResultAction(AddExp, 80, 80)

    DefineMission(77, "Mushroom Mushroom", 68)

    MisBeginTalk("<t>Speaking about it, in the past, I used to raise several <rGreedy Shrooms> outside Argent City. Recently I have been busy experimenting with the recipe so I totally forgotten all about them, seems to me now is a good time to put them to good use. Can you please retrieve 6 <rPoison Mushrooms>?<n><t>Err...Speaking about it, these Greedy Shroom are quite aggressive so its better to bring some healing items just in case. Other than that, these Greedy Shrooms can be found at <j(2220, 2564)>. Have a look, it might be the best time for harvest, everything is in your hands now!")
    MisBeginCondition(HasRecord, 67)
    MisBeginCondition(NoMission, 68)
    MisBeginCondition(NoRecord, 68)
    MisBeginAction(AddMission, 68)
    MisBeginAction(AddTrigger, 681, TE_GETITEM, 1725, 6)
    MisCancelAction(ClearMission, 68)

    MisNeed(MIS_NEED_DESP,"Collect 6 <rPoison Mushrooms> and look for <bRouri> in Argent City at <j(2240, 2752)>.")
    MisNeed(MIS_NEED_ITEM, 1725, 6, 10, 6)

    MisPrize(MIS_PRIZE_ITEM, 4308, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great! You have collected all the stuff! Thanks!")
    MisHelpTalk("<t>What's the matter? Did the Greedy Shrooms frighten you?")
    MisResultCondition(HasMission, 68)
    MisResultCondition(HasItem, 1725, 6)
    MisResultAction(TakeItem, 1725, 6)
    MisResultAction(ClearMission, 68)
    MisResultAction(SetRecord, 68)
    MisResultAction(AddExp, 250, 250)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1725)
    TriggerAction(1, AddNextFlag, 68, 10, 6)
    RegCurTrigger(681)

    DefineMission(78, "Letter for Coddy", 69)

    MisBeginTalk("<t>Seems that you are very capable.<n><t>Take this letter to <bSailor - Cody> in Argent Harbor at <j(2219, 2911)>.")
    MisBeginCondition(HasRecord, 68)
    MisBeginCondition(NoRecord, 69)
    MisBeginCondition(NoMission, 69)
    MisBeginAction(AddMission, 69)
    MisBeginAction(GiveItem, 4122, 1, 4)
    MisCancelAction(ClearMission, 69)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Sailor Coddy")

    MisHelpTalk("<t>What's the matter? I'm busy. Please go and look for <bSailor - Coddy>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(79, "Letter for Coddy", 69, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>I see, so it was <bDitto> who recommended you to come here.")
    MisResultCondition(NoRecord, 69)
    MisResultCondition(HasMission, 69)
    MisResultCondition(HasItem, 4122, 1)
    MisResultAction(TakeItem, 4122, 1)
    MisResultAction(ClearMission, 69)
    MisResultAction(SetRecord, 69)
    MisResultAction(AddExp, 120, 120)

    DefineMission(80, "Lost Items", 70)

    MisBeginTalk("<t>Previously, I was in charge of a batch of goods at the harbor. Unfortunately, I lost 2 vials of <rTortoise Blood>. This could spell some trouble for me!<n><t>Can you please locate and bring 2 vials of Tortoise Blood for me? It can turn out to be a life threatening situation!<n><t>I remember that the blood can be found on <rGrass Tortoise> at <j(2057, 2564)>.")
    MisBeginCondition(HasRecord, 69)
    MisBeginCondition(NoMission, 70)
    MisBeginCondition(NoRecord, 70)
    MisBeginAction(AddMission, 70)
    MisBeginAction(AddTrigger, 701, TE_GETITEM, 1844, 2)
    MisCancelAction(ClearMission, 70)

    MisNeed(MIS_NEED_DESP,"Collect 2 vials of <rTortoise Blood> and return to <bSailor - Coddy> in Argent City at <j(2219, 2911)>.")
    MisNeed(MIS_NEED_ITEM, 1844, 2, 10, 2)

    MisPrize(MIS_PRIZE_ITEM, 4310, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great!<n><t>You have saved me! Thank you so much!")
    MisHelpTalk("<t>Hurry up, before I get fired!")
    MisResultCondition(HasMission, 70)
    MisResultCondition(HasItem, 1844, 2)
    MisResultAction(TakeItem, 1844, 2)
    MisResultAction(ClearMission, 70)
    MisResultAction(SetRecord, 70)
    MisResultAction(AddExp, 400, 400)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1844)
    TriggerAction(1, AddNextFlag, 70, 10, 2)
    RegCurTrigger(701)

    DefineMission(81, "Letter for Senna", 71)

    MisBeginTalk("<t>Although we have asked you for help with many of our things, you have also progress much. Please take this letter to <bNewbie Guide Senna> at <j(2223, 2785)>. After locating her, she'll give you some new instructions.")
    MisBeginCondition(HasRecord, 70)
    MisBeginCondition(NoRecord, 71)
    MisBeginCondition(NoMission, 71)
    MisBeginAction(AddMission, 71)
    MisBeginAction(GiveItem, 4115, 1, 4)
    MisCancelAction(ClearMission, 71)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Bring this letter to Argent's <bNewbie Guide Senna>.")

    MisHelpTalk("<t><bNewbie Guide Senna> is at <j(2223, 2785)>. Go now.")
    MisResultCondition(AlwaysFailure)

    DefineMission(82, "Letter for Senna", 71, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Argent City.")
    MisResultCondition(NoRecord, 71)
    MisResultCondition(HasMission, 71)
    MisResultCondition(HasItem, 4115, 1)
    MisResultAction(TakeItem, 4115, 1)
    MisResultAction(ClearMission, 71)
    MisResultAction(SetRecord, 71)
    MisResultAction(AddExp, 200, 200)

    DefineMission(83, "Letter for Michael", 72)

    MisBeginTalk("<t>There's nothing more for me to help you with.<n><t>Take this letter to <bMichael>, the patroller and see if he has anymore tasks for you. He can be found outside Shaitan City <j(958, 3549)>.")
    MisBeginCondition(HasRecord, 710)
    MisBeginCondition(NoRecord, 72)
    MisBeginCondition(NoMission, 72)
    MisBeginAction(AddMission, 72)
    MisBeginAction(GiveItem, 4123, 1, 4)
    MisCancelAction(ClearMission, 72)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bGuard - Michael>.")

    MisHelpTalk("<t><bMichael> can be found near <j(2065, 2732)>. Hurry up and go.")
    MisResultCondition(AlwaysFailure)

    DefineMission(84, "Letter for Amos", 73)

    MisBeginTalk("<t>If you prefer collecting stuff to fighting, give this letter to <bAmos> the Grocer at <j(840, 3585)>. He has a task for you.")
    MisBeginCondition(HasRecord, 710)
    MisBeginCondition(NoRecord, 73)
    MisBeginCondition(NoMission, 73)
    MisBeginAction(AddMission, 73)
    MisBeginAction(GiveItem, 4124, 1, 4)
    MisCancelAction(ClearMission, 73)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bAmos>.")

    MisHelpTalk("<t>Why haven't you passed the letter to <bAmos>? He is located at <j(840, 3585)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(85, "Letter for Michael", 72, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>It\'s good that you are here, I could use a helping hand.<n><t>Also, take note of your HP bar while in battle. That\'s right, the red bar shows your HP level. You will die when it reaches 0. Beware!<n><t>Beside eating "Apples", "Cakes" or other recovery potion, you can press the "Insert" key to increase HP/SP recovery rate.'
    )
    MisResultCondition(NoRecord, 72)
    MisResultCondition(HasMission, 72)
    MisResultCondition(HasItem, 4123, 1)
    MisResultAction(TakeItem, 4123, 1)
    MisResultAction(ClearMission, 72)
    MisResultAction(SetRecord, 72)
    MisResultAction(AddExp, 50, 50)

    DefineMission(86, "Cactus Invasion", 74)

    MisBeginTalk("<t>Ah! Now the hills surrounding Shaitan city are overflowing with cactus. In my area especially, there are unusually plenty of them. Can you do me a favour? Defeat 15 <rCactus>, they can be found at <j(1031, 3556)> and seems to me you are the most suited for this task.")
    MisBeginCondition(HasRecord, 72)
    MisBeginCondition(NoMission, 74)
    MisBeginCondition(NoRecord, 74)
    MisBeginAction(AddMission, 74)
    MisBeginAction(AddTrigger, 741, TE_KILL, 95, 6)
    MisCancelAction(ClearMission, 74)

    MisNeed(MIS_NEED_DESP,"Kill 6 <rCactus> and return to <bMichael> at <j(2065, 2732)>.")
    MisNeed(MIS_NEED_KILL, 95, 6, 10, 6)

    MisResultTalk("<t>Thank God! You've exterminated quite a lot of them.")
    MisHelpTalk("<t>Its only 6 Cactuses, are you scared?")
    MisResultCondition(HasMission, 74)
    MisResultCondition(HasFlag, 74, 15)
    MisResultAction(AddExp, 150, 150)
    MisResultAction(ClearMission, 74)
    MisResultAction(SetRecord, 74)

    InitTrigger()
    TriggerCondition(1, IsMonster, 95)
    TriggerAction(1, AddNextFlag, 74, 10, 6)
    RegCurTrigger(741)

    DefineMission(87, "Letter for Franco", 75)

    MisBeginTalk("<t> Please deliver this letter to my good friend <bFranco> at <j(867, 3660)> in Shaitan City.<n><t>He's an interesting person and will be able to help you.")
    MisBeginCondition(HasRecord, 74)
    MisBeginCondition(NoRecord, 75)
    MisBeginCondition(NoMission, 75)
    MisBeginAction(AddMission, 75)
    MisBeginAction(GiveItem, 4125, 1, 4)
    MisCancelAction(ClearMission, 75)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Franco")

    MisHelpTalk("<t>Please hurry, I need to go patrolling.")
    MisResultCondition(AlwaysFailure)

    DefineMission(88, "Letter for Franco", 75, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, you're <bMichael's> friend? Thank you for the letter..")
    MisResultCondition(NoRecord, 75)
    MisResultCondition(HasMission, 75)
    MisResultCondition(HasItem, 4125, 1)
    MisResultAction(TakeItem, 4125, 1)
    MisResultAction(ClearMission, 75)
    MisResultAction(SetRecord, 75)
    MisResultAction(AddExp, 80, 80)

    DefineMission(89, "Kicking Monster", 76)

    MisBeginTalk("<t>Have you heard about a creature in the desert that kicks travelers? While on the way to this city, I was kicked and injured by a <rHumpy Camel>. Can you help me to teach them a lesson? Defeating 8 <rHumpy Camels> shouldn't be too hard for you.")
    MisBeginCondition(HasRecord, 75)
    MisBeginCondition(NoMission, 76)
    MisBeginCondition(NoRecord, 76)
    MisBeginAction(AddMission, 76)
    MisBeginAction(AddTrigger, 761, TE_KILL, 48, 8)
    MisCancelAction(ClearMission, 76)

    MisNeed(MIS_NEED_DESP,"Hunt 8 <rHumpy Camel> and report back to Franco at <j(867, 3660)> in Shaitan.")
    MisNeed(MIS_NEED_KILL, 48, 8, 10, 8)

    MisPrize(MIS_PRIZE_ITEM, 9, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great. There should not be any further complaint.<n><t>Those camels are really getting out of hand.")
    MisHelpTalk("<t>What? You're not able to teach them a lesson? Try harder.")
    MisResultCondition(HasMission, 76)
    MisResultCondition(HasFlag, 76, 17)
    MisResultAction(AddExp, 250, 250)
    MisResultAction(ClearMission, 76)
    MisResultAction(SetRecord, 76)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 48)
    TriggerAction(1, AddNextFlag, 76, 10, 8)
    RegCurTrigger(761)

    DefineMission(90, "Letter for Smithy", 77)

    MisBeginTalk("<t>Can you deliver this letter to <bBlacksmith Smithy> at <j(902, 3495)>. He's an interesting person and will help you.")
    MisBeginCondition(HasRecord, 76)
    MisBeginCondition(NoRecord, 77)
    MisBeginCondition(NoMission, 77)
    MisBeginAction(AddMission, 77)
    MisBeginAction(GiveItem, 4126, 1, 4)
    MisCancelAction(ClearMission, 77)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Smithy")

    MisHelpTalk("<t>Hurry up and look for <bBlacksmith Smithy>. Don't forget to tell him you're a friend of mine.")
    MisResultCondition(AlwaysFailure)

    DefineMission(91, "Letter for Smithy", 77, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, <bFranco> sent you to help? That's great! I am short of manpower now!")
    MisResultCondition(NoRecord, 77)
    MisResultCondition(HasMission, 77)
    MisResultCondition(HasItem, 4126, 1)
    MisResultAction(TakeItem, 4126, 1)
    MisResultAction(ClearMission, 77)
    MisResultAction(SetRecord, 77)
    MisResultAction(AddExp, 120, 120)

    DefineMission(92, "Scorpion Phobia", 78)

    MisBeginTalk("<t>To tell you the truth, I ain't afraid of anything but scorpions!<n><t>However my job requires me to constantly travel outside the city to collect quarry!<n><t>It seems that the many giant scorpions suddenly appeared out of nowhere and they are much more powerful than the little scorpions. To be stung by them can be a life threatening situation! Its too scary!<n><t>Please help me kill 12 <Big Scorpions>! You can easily see them at <j(1184, 3557)> but beware of their agile movements, take care!")
    MisBeginCondition(HasRecord, 77)
    MisBeginCondition(NoMission, 78)
    MisBeginCondition(NoRecord, 78)
    MisBeginAction(AddMission, 78)
    MisBeginAction(AddTrigger, 781, TE_KILL, 247, 12)
    MisCancelAction(ClearMission, 56)

    MisNeed(MIS_NEED_DESP,"Kill 12 <rBig Scorpions> and report back to <bBlacksmith Smithy> at <j(902, 3495)> in Shaitan City.")
    MisNeed(MIS_NEED_KILL, 247, 12, 10, 12)

    MisPrize(MIS_PRIZE_ITEM, 4309, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Good! You are the right person for this job. You completed it so fast. That is really brave of you.")
    MisHelpTalk("<t>What is wrong? You are unable to defeat those scorpions? Bring some potions then.")
    MisResultCondition(HasMission, 78)
    MisResultCondition(HasFlag, 78, 21)
    MisResultAction(ClearMission, 78)
    MisResultAction(AddExp, 400, 400)
    MisResultAction(SetRecord, 78)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 247)
    TriggerAction(1, AddNextFlag, 78, 10, 12)
    RegCurTrigger(781)

    DefineMission(93, "Letter for Bliss", 79)

    MisBeginTalk("<t>Although the people of Shaitan City constantly depended on you for help, however you have grown from the experiences. <n><t>Take this letter to <bNewbie Guide - Resline> at <j(876, 3572)>. She will tell you what to do next.")
    MisBeginCondition(HasRecord, 78)
    MisBeginCondition(NoRecord, 79)
    MisBeginCondition(NoMission, 79)
    MisBeginAction(AddMission, 79)
    MisBeginAction(GiveItem, 4127, 1, 4)
    MisCancelAction(ClearMission, 79)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Shaitan Newbie Guide")

    MisHelpTalk("<t><bNewbie Guide Resline> is at <j(876, 3572)>. Please hurry.")
    MisResultCondition(AlwaysFailure)

    DefineMission(94, "Letter for Bliss", 79, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Shaitan City.")
    MisResultCondition(NoRecord, 79)
    MisResultCondition(HasMission, 79)
    MisResultCondition(HasItem, 4127, 1)
    MisResultAction(TakeItem, 4127, 1)
    MisResultAction(ClearMission, 79)
    MisResultAction(SetRecord, 79)
    MisResultAction(AddExp, 200, 200)

    DefineMission(95, "Herbalist Promotion", 60, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("Since the Newbie Guide recommended you, you must have shown some potential. If you want to become a Herbalist, you'll have to finish the tasks I assign to you.")
    MisResultCondition(NoRecord, 60)
    MisResultCondition(HasMission, 60)
    MisResultCondition(HasItem, 4118, 1)
    MisResultAction(TakeItem, 4118, 1)
    MisResultAction(ClearMission, 60)
    MisResultAction(SetRecord, 60)

    DefineMission(96, "Herbalist Promotion", 80)

    MisBeginTalk("<t>Why do you want to be a herbalist? Recently, many have tried to be a herbalist just to earn money. You have to prove to me whether you can meet the qualifications of being one.<n><t>I need to test your survivability, proceed to <j(884, 3156)> and kill 2 <rKiller Cactus>. If you able to complete this task at hand, it is considered as passing this part of the test.")
    MisBeginCondition(HasRecord, 60)
    MisBeginCondition(NoMission, 80)
    MisBeginCondition(NoRecord, 80)
    MisBeginAction(AddMission, 80)
    MisBeginAction(AddTrigger, 801, TE_KILL, 43, 2)
    MisCancelAction(ClearMission, 80)

    MisNeed(MIS_NEED_DESP,"Kill 2 <rKiller Cactus> and report back to <bHigh Priest Gannon> at <j(862, 3500)>.")
    MisNeed(MIS_NEED_KILL, 43, 2, 10, 2)

    MisResultTalk("<t>Looks like you have succeeded. Well done.")
    MisHelpTalk("<t>Do you have a problem?")
    MisResultCondition(HasMission, 80)
    MisResultCondition(HasFlag, 80, 11)
    MisResultAction(ClearMission, 80)
    MisResultAction(SetRecord, 80)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsMonster, 43)
    TriggerAction(1, AddNextFlag, 80, 10, 2)
    RegCurTrigger(801)

    DefineMission(97, "Herbalist Promotion", 81)

    MisBeginTalk("<t>Please pass this letter to <bNavy HQ - Admiral Nic> at <j(865, 3648)>. She will know what to do.")
    MisBeginCondition(HasRecord, 80)
    MisBeginCondition(NoRecord, 81)
    MisBeginCondition(NoMission, 81)
    MisBeginAction(AddMission, 81)
    MisBeginAction(GiveItem, 4128, 1, 4)
    MisCancelAction(ClearMission, 81)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send letter to <bNavy HQ - Admiral Nic> at <j(865, 3648)>.")

    MisHelpTalk("<t>Hurry up and go, you can do it!")
    MisResultCondition(AlwaysFailure)

    DefineMission(98, "Herbalist Promotion", 81, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>You've passed the first part of the trial. Not bad!")
    MisResultCondition(NoRecord, 81)
    MisResultCondition(HasMission, 81)
    MisResultCondition(HasItem, 4128, 1)
    MisResultAction(TakeItem, 4128, 1)
    MisResultAction(ClearMission, 81)
    MisResultAction(SetRecord, 81)
    MisResultAction(AddExp, 100, 100)

    DefineMission(99, "Herbalist Promotion", 82)

    MisBeginTalk("<t>Here is where you take the second part of the test to become a herbalist. If you are successful here, you are likely to become a herbalist soon.<n><t>This is the task I have set for you: Gather for me 3 <yMedicated Grass>. It is as simple as that.<n><t>You can obtain them from the <rHopping Lizards> just outside the city at <j(892, 3273)>.")
    MisBeginCondition(HasRecord, 81)
    MisBeginCondition(NoMission, 82)
    MisBeginCondition(NoRecord, 82)
    MisBeginAction(AddMission, 82)
    MisBeginAction(AddTrigger, 821, TE_GETITEM, 3129, 3)
    MisCancelAction(ClearMission, 82)

    MisNeed(MIS_NEED_DESP,"Collect 3 <yMedicated Grass> and return to <bAdmiral Nic> at <j(865, 3648)>.")
    MisNeed(MIS_NEED_ITEM, 3129, 3, 10, 3)

    MisResultTalk("<t>Good. You have completed the task!")
    MisHelpTalk("<t>I know that it is dangerous. However, you still have to complete it.")
    MisResultCondition(HasMission, 82)
    MisResultCondition(HasItem, 3129, 3)
    MisResultAction(TakeItem, 3129, 3)
    MisResultAction(ClearMission, 82)
    MisResultAction(SetRecord, 82)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsItem, 3129)
    TriggerAction(1, AddNextFlag, 82, 10, 3)
    RegCurTrigger(821)

    DefineMission(150, "Herbalist Promotion", 83)

    MisBeginTalk("<t>You have proven that you are worthy. Take this letter to Gannon, I have approved for you to become a Herbalist. This <rRighteous Document> is proof of your success.")
    MisBeginCondition(HasRecord, 82)
    MisBeginCondition(NoRecord, 83)
    MisBeginCondition(NoMission, 83)
    MisBeginAction(AddMission, 83)
    MisBeginAction(GiveItem, 3954, 1, 4)
    MisCancelAction(ClearMission, 83)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Look for <bHigh Priest Gannon> in Shaitan City at <j(862, 3500)>.")

    MisHelpTalk("<t>What are you doing? Are you still hesitating?")
    MisResultCondition(AlwaysFailure)

    DefineMission(151, "Herbalist Promotion", 83, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Congratulation my child! Now you are a Herbalist! May the Goddess be with you!<n><t>(You can now activate class quest at Gannon.<n><t>Also, buy your weapons from <bBlacksmith Smithy>, armor from <bTailor Moya> and skill books from <bGrocer Amos>).")
    MisResultCondition(NoRecord, 83)
    MisResultCondition(HasMission, 83)
    MisResultCondition(HasItem, 3954, 1)
    MisResultAction(TakeItem, 3954, 1)
    MisResultAction(ClearMission, 83)
    MisResultAction(SetRecord, 83)
    MisResultAction(AddExp, 100, 100)
    MisResultAction(ClearFightSkill, 475)
    MisResultAction(SetProfession, 5)
    MisResultAction(GiveItem, 3206, 1, 4)
    MisResultAction(GiveItem, 97, 1, 4)
    MisResultAction(GiveItem, 15073, 1, 4, 0)
    MisResultBagNeed(3)

    DefineMission(152, "Letter for Amos", 73, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>It's good that you are here, I could use a helping hand.")
    MisResultCondition(NoRecord, 73)
    MisResultCondition(HasMission, 73)
    MisResultCondition(HasItem, 4124, 1)
    MisResultAction(TakeItem, 4124, 1)
    MisResultAction(ClearMission, 73)
    MisResultAction(SetRecord, 73)
    MisResultAction(AddExp, 50, 50)

    DefineMission(153, "Grafting Analysis", 84)

    MisBeginTalk("<t>Do you wish to see flowering Cactus? I'm doing an experiment can you help me gather 6 <rCactus Hairballs>?<n><t>You will be able to find them off the <rMelons> that are outside the city. For now I only need these few but I might have to ask you to get more in future.")
    MisBeginCondition(HasRecord, 73)
    MisBeginCondition(NoMission, 84)
    MisBeginCondition(NoRecord, 84)
    MisBeginAction(AddMission, 84)
    MisBeginAction(AddTrigger, 841, TE_GETITEM, 1691, 6)
    MisCancelAction(ClearMission, 84)

    MisNeed(MIS_NEED_DESP,"Collect 6 <rCactus Hairballs> and look for <bAmos> at <j(840, 3585)>.")
    MisNeed(MIS_NEED_ITEM, 1691, 6, 10, 6)

    MisResultTalk("<t>Good! Now I can start my experiment. Come back next year to see my work!")
    MisHelpTalk("<t>Hmm, still unable to get the items I need?")
    MisResultCondition(HasMission, 84)
    MisResultCondition(HasItem, 1691, 6)
    MisResultAction(TakeItem, 1691, 6)
    MisResultAction(ClearMission, 84)
    MisResultAction(SetRecord, 84)
    MisResultAction(AddExp, 150, 150)

    InitTrigger()
    TriggerCondition(1, IsItem, 1691)
    TriggerAction(1, AddNextFlag, 84, 10, 6)
    RegCurTrigger(841)

    DefineMission(154, "Letter for Lena", 85)

    MisBeginTalk("<t>My experiments have yet to yield any proper results.<n><t>Why not you pass this letter to <bLena> at <j(883, 3520)> and see if there's anything you can help out with?")
    MisBeginCondition(HasRecord, 84)
    MisBeginCondition(NoRecord, 85)
    MisBeginCondition(NoMission, 85)
    MisBeginAction(AddMission, 85)
    MisBeginAction(GiveItem, 4129, 1, 4)
    MisCancelAction(ClearMission, 85)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bLena> in Shaitan City.")

    MisHelpTalk("<t>Go! There is nothing much I can help you with right now.")
    MisResultCondition(AlwaysFailure)

    DefineMission(155, "Letter for Lena", 85, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>You are a friend of <bAmos>? Hi!")
    MisResultCondition(NoRecord, 85)
    MisResultCondition(HasMission, 85)
    MisResultCondition(HasItem, 4129, 1)
    MisResultAction(TakeItem, 4129, 1)
    MisResultAction(ClearMission, 85)
    MisResultAction(SetRecord, 85)
    MisResultAction(AddExp, 80, 80)

    DefineMission(156, "Fake Item", 86)

    MisBeginTalk("<t>Listen to my proposal: Lately, the physician of Icicle City has been collecting the sweat of animals. I want to play a joke on him so I need some materials to make the fakes.<n><t>Help me collect 3 vials of <rMurky Water>. They be found on <rBaby Scorpion> at <j(1184, 3557)>. Just take this as a special request, haha!")
    MisBeginCondition(HasRecord, 85)
    MisBeginCondition(NoMission, 86)
    MisBeginCondition(NoRecord, 86)
    MisBeginAction(AddMission, 86)
    MisBeginAction(AddTrigger, 861, TE_GETITEM, 1648, 3)
    MisCancelAction(ClearMission, 86)

    MisNeed(MIS_NEED_DESP,"Collect 3 vials of <rMurky Water> and return to <bLena> in Shaitan City at <j(883, 3520)>.")
    MisNeed(MIS_NEED_ITEM, 1648, 3, 10, 3)

    MisPrize(MIS_PRIZE_ITEM, 4308, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Haha! That's great! I'll definitely be able to fool him using these.")
    MisHelpTalk("<t>It can't be! You still haven't found anything? Please hurry!")
    MisResultCondition(HasMission, 86)
    MisResultCondition(HasItem, 1648, 3)
    MisResultAction(TakeItem, 1648, 3)
    MisResultAction(ClearMission, 86)
    MisResultAction(SetRecord, 86)
    MisResultAction(AddExp, 250, 250)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1648)
    TriggerAction(1, AddNextFlag, 86, 10, 3)
    RegCurTrigger(861)

    DefineMission(157, "Letter for Franklin", 87)

    MisBeginTalk("<t>I have a letter that needs to be delivered.<n><t>Could you take the trouble to give it to <bFranklin the Builder>? He is a friendly fellow and will certainly help you.<n><t>He can be found inside Shaitan City at <j(901, 3668)>.")
    MisBeginCondition(HasRecord, 86)
    MisBeginCondition(NoRecord, 87)
    MisBeginCondition(NoMission, 87)
    MisBeginAction(AddMission, 87)
    MisBeginAction(GiveItem, 4130, 1, 4)
    MisCancelAction(ClearMission, 87)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to <bBuilder Franklin>.")

    MisHelpTalk("<t>Go now! That helpful guy will assist you.")
    MisResultCondition(AlwaysFailure)

    DefineMission(158, "Letter for Franklin", 87, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, a letter from <bLena>! Thanks!")
    MisResultCondition(NoRecord, 87)
    MisResultCondition(HasMission, 87)
    MisResultCondition(HasItem, 4130, 1)
    MisResultAction(TakeItem, 4130, 1)
    MisResultAction(ClearMission, 87)
    MisResultAction(SetRecord, 87)
    MisResultAction(AddExp, 120, 120)

    DefineMission(159, "Medication for Nausea", 88)

    MisBeginTalk("<t>I need to collect some medicine that cure seasickness, especially <rCactus Flower> which I am having an acute shortage. Go to <j(1031, 3556)> and collect 3 <rCactus Flower> for me which definitely drop from <rCactus>. At your current level, you should have no problem dealing with them.<n><t>By the way, remember not tell anyone about this recipe for curing seasickness.")
    MisBeginCondition(HasRecord, 87)
    MisBeginCondition(NoMission, 88)
    MisBeginCondition(NoRecord, 88)
    MisBeginAction(AddMission, 88)
    MisBeginAction(AddTrigger, 881, TE_GETITEM, 1692, 3)
    MisCancelAction(ClearMission, 88)

    MisNeed(MIS_NEED_DESP,"Collect 3 <rCactus Blossom> and talk to <bBuilder Franklin> at <j(901, 3668)>.")
    MisNeed(MIS_NEED_ITEM, 1692, 3, 10, 3)

    MisPrize(MIS_PRIZE_ITEM, 4310, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>I must thanks you. I can start decocting finally.")
    MisHelpTalk("<t>I only need these as ingredient! Don't let me down!")
    MisResultCondition(HasMission, 88)
    MisResultCondition(HasItem, 1692, 3)
    MisResultAction(TakeItem, 1692, 3)
    MisResultAction(ClearMission, 88)
    MisResultAction(SetRecord, 88)
    MisResultAction(AddExp, 400, 400)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1692)
    TriggerAction(1, AddNextFlag, 88, 10, 3)
    RegCurTrigger(881)

    DefineMission(160, "Letter for Bliss", 89)

    MisBeginTalk("<t>Although we have asked you for help with many of our things, you have also progress much.<n><t>Please take this letter to <bNewbie Guide Resline> at <j(876, 3572)>.<n><t>After locating her, she will give you some new instructions.")
    MisBeginCondition(HasRecord, 88)
    MisBeginCondition(NoRecord, 89)
    MisBeginCondition(NoMission, 89)
    MisBeginAction(AddMission, 89)
    MisBeginAction(GiveItem, 4127, 1, 4)
    MisCancelAction(ClearMission, 89)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Shaitan <bNewbie Guide Resline>.")

    MisHelpTalk("<t><bNewbie Guide Resline> is at <j(876, 3572)>. Please hurry.")
    MisResultCondition(AlwaysFailure)

    DefineMission(161, "Letter for Bliss", 89, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Shaitan City.")
    MisResultCondition(NoRecord, 89)
    MisResultCondition(HasMission, 89)
    MisResultCondition(HasItem, 4127, 1)
    MisResultAction(TakeItem, 4127, 1)
    MisResultAction(ClearMission, 89)
    MisResultAction(SetRecord, 89)
    MisResultAction(AddExp, 200, 200)

    DefineMission(162, "Letter for Little Mo", 90)

    MisBeginTalk("<t>There's nothing more for me to help you with.<n><t>Take this letter to <bLittle Mo>, the patroller and see if he has anymore tasks for you. He can be found outside Icicle Castle at <j(1237, 613)>.")
    MisBeginCondition(HasRecord, 716)
    MisBeginCondition(NoRecord, 90)
    MisBeginCondition(NoMission, 90)
    MisBeginAction(AddMission, 90)
    MisBeginAction(GiveItem, 4131, 1, 4)
    MisCancelAction(ClearMission, 90)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bPatrol - Little Mo>.")

    MisHelpTalk("<t><bLittle Mo> is around <j(1237, 613)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(163, "Letter for Palpin", 91)

    MisBeginTalk("<t>If you prefer collecting stuff to fighting, give this letter to <bPalpin> the Grocer at <j(1356, 483)>. He has a task for you.")
    MisBeginCondition(HasRecord, 716)
    MisBeginCondition(NoRecord, 91)
    MisBeginCondition(NoMission, 91)
    MisBeginAction(AddMission, 91)
    MisBeginAction(GiveItem, 4132, 1, 4)
    MisCancelAction(ClearMission, 91)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Bring the letter to Grocer Palpin")

    MisHelpTalk("<t>Why haven't you passed the letter to <bPalpin>? He is nearby at <j(1356, 483)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(164, "Letter for Michael", 90, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk('<t>It\'s good that you are here, I could use a helping hand.<n><t>Also, take note of your HP bar while in battle. That\'s right, the red bar shows your HP level. You will die when it reaches 0. Beware!<n><t>Beside eating "Apples", "Cakes" or other recovery potion, you can press the "Insert" key to increase HP/SP recovery rate.'
    )
    MisResultCondition(NoRecord, 90)
    MisResultCondition(HasMission, 90)
    MisResultCondition(HasItem, 4131, 1)
    MisResultAction(TakeItem, 4131, 1)
    MisResultAction(ClearMission, 90)
    MisResultAction(SetRecord, 90)
    MisResultAction(AddExp, 50, 50)

    DefineMission(165, "Playful Squidy", 92)

    MisBeginTalk("<t>Lately those Snow Squidy are becoming more playful. They actually attacked me while I was taking a break at my post. Help me teach them a lesson, return after you defeat 6 <rSnow Squidy>.<n><t>Do not take this mission lightly!<n><t>I know your strengths and abilities well enough!")
    MisBeginCondition(HasRecord, 90)
    MisBeginCondition(NoMission, 92)
    MisBeginCondition(NoRecord, 92)
    MisBeginAction(AddMission, 92)
    MisBeginAction(AddTrigger, 921, TE_KILL, 235, 6)
    MisCancelAction(ClearMission, 92)

    MisNeed(MIS_NEED_DESP,"Kill 6 <rSnow Squidy> and report back to <bLittle Mo> at <j(1237, 613)>.")
    MisNeed(MIS_NEED_KILL, 235, 6, 10, 6)

    MisResultTalk("<t>Good. Now I can have a good rest.")
    MisHelpTalk("<t>What? You've not done anything yet? This shouldn't be much of a problem to you.")
    MisResultCondition(HasMission, 92)
    MisResultCondition(HasFlag, 92, 15)
    MisResultAction(AddExp, 150, 150)
    MisResultAction(ClearMission, 92)
    MisResultAction(SetRecord, 92)

    InitTrigger()
    TriggerCondition(1, IsMonster, 235)
    TriggerAction(1, AddNextFlag, 92, 10, 6)
    RegCurTrigger(921)

    DefineMission(166, "Letter for Ray", 93)

    MisBeginTalk("<t>There's not much I can do for you. Take this letter and give it to <bRay> at <j(1365, 570)>. He should be able to help you.")
    MisBeginCondition(HasRecord, 92)
    MisBeginCondition(NoRecord, 93)
    MisBeginCondition(NoMission, 93)
    MisBeginAction(AddMission, 93)
    MisBeginAction(GiveItem, 4133, 1, 4)
    MisCancelAction(ClearMission, 93)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bRay> in Icicle City")

    MisHelpTalk("<t><bRay> is at <j(1365, 570)>. Go now!")
    MisResultCondition(AlwaysFailure)

    DefineMission(167, "Letter for Ray", 93, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh! <bLittle Mo> send you? Great!")
    MisResultCondition(NoRecord, 93)
    MisResultCondition(HasMission, 93)
    MisResultCondition(HasItem, 4133, 1)
    MisResultAction(TakeItem, 4133, 1)
    MisResultAction(ClearMission, 93)
    MisResultAction(SetRecord, 93)
    MisResultAction(AddExp, 80, 80)

    DefineMission(168, "There's never too much", 94)

    MisBeginTalk("<t>It is a tough time for the people suffering in Icicle City, so the task I am going to entrust you is this: <rSnowy Piglet> consume huge amount of food and are getting out of control, please go to <j(1179, 371)> and destroy 8 <rSnowy Piglets>.")
    MisBeginCondition(HasRecord, 93)
    MisBeginCondition(NoMission, 94)
    MisBeginCondition(NoRecord, 94)
    MisBeginAction(AddMission, 94)
    MisBeginAction(AddTrigger, 941, TE_KILL, 239, 8)
    MisCancelAction(ClearMission, 94)

    MisNeed(MIS_NEED_DESP,"Kill 8 <rSnowy Piglets> and return to <bRay> in Icicle City at <j(1365, 570)>.")
    MisNeed(MIS_NEED_KILL, 239, 8, 10, 8)

    MisPrize(MIS_PRIZE_ITEM, 9, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Done? Thank you! Those piglets have a big appetite. Seems like I need to have BBQ pork every day!")
    MisHelpTalk("<t>What? Still nothing?")
    MisResultCondition(HasMission, 94)
    MisResultCondition(HasFlag, 94, 17)
    MisResultAction(AddExp, 250, 250)
    MisResultAction(ClearMission, 94)
    MisResultAction(SetRecord, 94)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 239)
    TriggerAction(1, AddNextFlag, 94, 10, 8)
    RegCurTrigger(941)

    DefineMission(169, "Letter for Hannah", 95)

    MisBeginTalk("<t>While I'm currently giving out food to the public, take this letter to <bHannah the Tailor> at <j(1349, 539)>. I think she might have a task for you.")
    MisBeginCondition(HasRecord, 94)
    MisBeginCondition(NoRecord, 95)
    MisBeginCondition(NoMission, 95)
    MisBeginAction(AddMission, 95)
    MisBeginAction(GiveItem, 4134, 1, 4)
    MisCancelAction(ClearMission, 95)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Tailor - Hannah")

    MisHelpTalk("<t>What are you waiting for? Hurry!")
    MisResultCondition(AlwaysFailure)

    DefineMission(170, "Letter for Hannah", 95, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh dear! I need your help so urgently!")
    MisResultCondition(NoRecord, 95)
    MisResultCondition(HasMission, 95)
    MisResultCondition(HasItem, 4134, 1)
    MisResultAction(TakeItem, 4134, 1)
    MisResultAction(ClearMission, 95)
    MisResultAction(SetRecord, 95)
    MisResultAction(AddExp, 120, 120)

    DefineMission(171, "Wrong Transfer", 96)

    MisBeginTalk("<t>Hoho, I have good news! It seems a pack of Little Deer have mistakenly migrated near to the Icicle City. With the current situation at Icicle City, this pack of deers is like a delicacy given by the gods. I want you to travel with haste to <j(1164, 305)>. Hunt and bring back 12 <rLittle Deers> so that we can have a feast tonight.")
    MisBeginCondition(HasRecord, 95)
    MisBeginCondition(NoMission, 96)
    MisBeginCondition(NoRecord, 96)
    MisBeginAction(AddMission, 96)
    MisBeginAction(AddTrigger, 961, TE_KILL, 238, 12)
    MisCancelAction(ClearMission, 56)

    MisNeed(MIS_NEED_DESP,"Hunt 12 <rLittle Deers> and return to <bHannah> at <j(1349, 539)>")
    MisNeed(MIS_NEED_KILL, 238, 12, 10, 12)

    MisPrize(MIS_PRIZE_ITEM, 4309, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Good! You have done it! I will put aside my duties for those delicious meat! Hehe.")
    MisHelpTalk("<t>Have you managed to hunt any deer yet? Please hurry up!")
    MisResultCondition(HasMission, 96)
    MisResultCondition(HasFlag, 96, 21)
    MisResultAction(ClearMission, 96)
    MisResultAction(AddExp, 400, 400)
    MisResultAction(SetRecord, 96)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsMonster, 238)
    TriggerAction(1, AddNextFlag, 96, 10, 12)
    RegCurTrigger(961)

    DefineMission(172, "Letter for Angela", 97)

    MisBeginTalk("<t>Although we have asked you for help with many of our things, you have also progress much.<n><t>Please take this letter to <bNewbie Guide Angela> at <j(1315, 507)>.<n><t>After locating her, she will give you some new instructions.")
    MisBeginCondition(HasRecord, 96)
    MisBeginCondition(NoRecord, 97)
    MisBeginCondition(NoMission, 97)
    MisBeginAction(AddMission, 97)
    MisBeginAction(GiveItem, 4135, 1, 4)
    MisCancelAction(ClearMission, 97)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Icicle <bNewbie Guide Angela>.")

    MisHelpTalk("<t><bNewbie Guide Angela> is at <j(1315, 507)>. Please hurry.")
    MisResultCondition(AlwaysFailure)

    DefineMission(173, "Letter for Angela", 97, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Icicle Castle.")
    MisResultCondition(NoRecord, 97)
    MisResultCondition(HasMission, 97)
    MisResultCondition(HasItem, 4135, 1)
    MisResultAction(TakeItem, 4135, 1)
    MisResultAction(ClearMission, 97)
    MisResultAction(SetRecord, 97)
    MisResultAction(AddExp, 200, 200)

    DefineMission(174, "Hunter Promotion", 59, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("Great! Since the Newbie Guide recommended you, you must be talented. Complete the tasks given to you if you want to become a Hunter.")
    MisResultCondition(NoRecord, 59)
    MisResultCondition(HasMission, 59)
    MisResultCondition(HasItem, 4117, 1)
    MisResultAction(TakeItem, 4117, 1)
    MisResultAction(ClearMission, 59)
    MisResultAction(SetRecord, 59)

    DefineMission(175, "Hunter Promotion", 98)

    MisBeginTalk("<t>Are you here to aquire the Hunter Manual? How brave.<n><t>However, bravery alone isn't enough. To aquire the <rHunter Manual> you are required to prove that you have the required agility and dexterity.<n><t>Go now to <pIcicle City> and capture 12 <rLittle White Deers>. They can be found at <j(1325, 305)>.<n><t>If you are able to complete the task, I will consider that you passed the first part of the test.")
    MisBeginCondition(HasRecord, 59)
    MisBeginCondition(NoMission, 98)
    MisBeginCondition(NoRecord, 98)
    MisBeginAction(AddMission, 98)
    MisBeginAction(AddTrigger, 981, TE_KILL, 240, 12)
    MisCancelAction(ClearMission, 98)

    MisNeed(MIS_NEED_DESP,"Kill 12 <rLittle White Deers> and report back to <bRay> in Icicle City at <j(1365, 570)>.")
    MisNeed(MIS_NEED_KILL, 240, 12, 10, 12)

    MisResultTalk("<t>You have not let me down. You completed the first round of the test.")
    MisHelpTalk("<t>How is it? Decided to give up?")
    MisResultCondition(HasMission, 98)
    MisResultCondition(HasFlag, 98, 21)
    MisResultAction(ClearMission, 98)
    MisResultAction(SetRecord, 98)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsMonster, 240)
    TriggerAction(1, AddNextFlag, 98, 10, 12)
    RegCurTrigger(981)

    DefineMission(176, "Hunter Promotion", 99)

    MisBeginTalk("<t>The second part of the trial is simple. Give this letter to <bLittle Mo> at <j(1237, 613)> and he will tell you what to do for the final part.")
    MisBeginCondition(HasRecord, 98)
    MisBeginCondition(NoRecord, 99)
    MisBeginCondition(NoMission, 99)
    MisBeginAction(AddMission, 99)
    MisBeginAction(GiveItem, 4136, 1, 4)
    MisCancelAction(ClearMission, 99)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to <bPatrol - Little Mo>.")

    MisHelpTalk("<t><bLittle Mo> is around <j(1237, 613)>.")
    MisResultCondition(AlwaysFailure)

    DefineMission(177, "Hunter Promotion", 99, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Wow, you've already completed the first part. Congratulations.")
    MisResultCondition(NoRecord, 99)
    MisResultCondition(HasMission, 99)
    MisResultCondition(HasItem, 4136, 1)
    MisResultAction(TakeItem, 4136, 1)
    MisResultAction(ClearMission, 99)
    MisResultAction(SetRecord, 99)
    MisResultAction(AddExp, 100, 100)

    DefineMission(178, "Hunter Promotion", 150)

    MisBeginTalk("<t>Let me tell you about the second part of your test. You are required to collect 3 <yMedicine Bottles> and return here. These medicine drop most commonly from <rSnowy Piglets> at <j(1179, 371)>.")
    MisBeginCondition(HasRecord, 99)
    MisBeginCondition(NoMission, 150)
    MisBeginCondition(NoRecord, 150)
    MisBeginAction(AddMission, 150)
    MisBeginAction(AddTrigger, 1501, TE_GETITEM, 1779, 3)
    MisCancelAction(ClearMission, 150)

    MisNeed(MIS_NEED_DESP,"Collect 3 <yBottles> and talk to <bLittle Mo> at <j(1237, 613).")
    MisNeed(MIS_NEED_ITEM, 1779, 3, 10, 3)

    MisResultTalk("<t>You succeeded! I am really happy for you!")
    MisHelpTalk("<t>What's the matter? Those piglets should be easy to deal with!")
    MisResultCondition(HasMission, 150)
    MisResultCondition(HasItem, 1779, 3)
    MisResultAction(TakeItem, 1779, 3)
    MisResultAction(ClearMission, 150)
    MisResultAction(SetRecord, 150)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsItem, 1779)
    TriggerAction(1, AddNextFlag, 150, 10, 3)
    RegCurTrigger(1501)

    DefineMission(179, "Hunter Promotion", 151)

    MisBeginTalk("<t>I think you are now fit to become a Hunter. Take this <rHunter Manual> and give it to <bRay> and you'll become a qualified Hunter.")
    MisBeginCondition(HasRecord, 150)
    MisBeginCondition(NoRecord, 151)
    MisBeginCondition(NoMission, 151)
    MisBeginAction(AddMission, 151)
    MisBeginAction(GiveItem, 3955, 1, 4)
    MisCancelAction(ClearMission, 151)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Talk to <bRay> in Icicle City at <j(1365, 570)>.")

    MisHelpTalk("<t>What are you doing? Are you still hesitating?")
    MisResultCondition(AlwaysFailure)

    DefineMission(180, "Hunter Promotion", 151, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Congratulations, you are now a qualified Hunter. Looks like things will be more peaceful here with you around.<n><t>(You can now activate class quest at <bRay>.<n><t>Also, buy your weapons from <bBlacksmith Bash>, armor from <bTailor Hannah> and skill books from <bGrocer Palpin>).")
    MisResultCondition(NoRecord, 151)
    MisResultCondition(HasMission, 151)
    MisResultCondition(HasItem, 3955, 1)
    MisResultCondition(PfEqual, 0)
    MisResultAction(TakeItem, 3955, 1)
    MisResultAction(ClearMission, 151)
    MisResultAction(SetRecord, 151)
    MisResultAction(AddExp, 100, 100)
    MisResultAction(ClearFightSkill, 475)
    MisResultAction(SetProfession, 2)
    MisResultAction(GiveItem, 3187, 1, 4)
    MisResultAction(GiveItem, 25, 1, 4)
    MisResultAction(GiveItem, 15073, 1, 4, 0)
    MisResultBagNeed(3)

    DefineMission(181, "Letter for Palpin", 91, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>It's good that you are here, I could use a helping hand.")
    MisResultCondition(NoRecord, 91)
    MisResultCondition(HasMission, 91)
    MisResultCondition(HasItem, 4132, 1)
    MisResultAction(TakeItem, 4132, 1)
    MisResultAction(ClearMission, 91)
    MisResultAction(SetRecord, 91)
    MisResultAction(AddExp, 50, 50)

    DefineMission(182, "Bangle Collection", 152)

    MisBeginTalk("<t>Recently I have been packing my own set of collectors item until I realised that I am missing <yFat Squid Tentacle>. Can you help get 3 <yFat Squid Tentacle>?<n><t>You can get them from <rSnow Squirt> at <j(1465, 402)>. They usually appear near the city gates.<n><t>This is the current task I want you to do!")
    MisBeginCondition(HasRecord, 91)
    MisBeginCondition(NoMission, 152)
    MisBeginCondition(NoRecord, 152)
    MisBeginAction(AddMission, 152)
    MisBeginAction(AddTrigger, 1521, TE_GETITEM, 1704, 3)
    MisCancelAction(ClearMission, 152)

    MisNeed(MIS_NEED_DESP,"Collect 3 <yFat Squid Tentacles> and look for <bPalpin> at <j(1356, 483)>.")
    MisNeed(MIS_NEED_ITEM, 1704, 3, 10, 3)

    MisResultTalk("<t>This is great! I have more stuff to add to my collection.<n><t>Thank you! I will need your help in the future as well.")
    MisHelpTalk("<t>What's the problem? Is it that hard to find them? They are just outside the main entrance.")
    MisResultCondition(HasMission, 152)
    MisResultCondition(HasItem, 1704, 3)
    MisResultAction(TakeItem, 1704, 3)
    MisResultAction(ClearMission, 152)
    MisResultAction(SetRecord, 152)
    MisResultAction(AddExp, 150, 150)

    InitTrigger()
    TriggerCondition(1, IsItem, 1704)
    TriggerAction(1, AddNextFlag, 152, 10, 3)
    RegCurTrigger(1521)

    DefineMission(183, "Letter for Yaskey", 153)

    MisBeginTalk("<t>I need to trouble you once again. Please send this letter to <bSidorf> at <j(1290, 540)>. He can help you too.")
    MisBeginCondition(HasRecord, 152)
    MisBeginCondition(NoRecord, 153)
    MisBeginCondition(NoMission, 153)
    MisBeginAction(AddMission, 153)
    MisBeginAction(GiveItem, 4137, 1, 4)
    MisCancelAction(ClearMission, 153)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to <bYaskey> in Icicle City at <j(1290, 540)>.")

    MisHelpTalk("<t>Good bye my friend, there's nothing more I can do to help you for now.")
    MisResultCondition(AlwaysFailure)

    DefineMission(184, "Letter for Yaskey", 153, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, <bPalpin> sent you to help out? That's great!")
    MisResultCondition(NoRecord, 153)
    MisResultCondition(HasMission, 153)
    MisResultCondition(HasItem, 4137, 1)
    MisResultAction(TakeItem, 4137, 1)
    MisResultAction(ClearMission, 153)
    MisResultAction(SetRecord, 153)
    MisResultAction(AddExp, 80, 80)

    DefineMission(185, "Concoction", 154)

    MisBeginTalk("<t>Recently, there were several incident where a lot of my bottles broke due to accidents.<n><t>Give me a hand! Go to <j(1118, 343)> and obtain 2 <yMedicine Bottles> from <rSnowy Piglets>. In this way, at least I have some replacements.")
    MisBeginCondition(HasRecord, 153)
    MisBeginCondition(NoMission, 154)
    MisBeginCondition(NoRecord, 154)
    MisBeginAction(AddMission, 154)
    MisBeginAction(AddTrigger, 1541, TE_GETITEM, 1779, 2)
    MisCancelAction(ClearMission, 154)

    MisNeed(MIS_NEED_DESP,"Collect 2 <yBottles> and return to <bYaskey> in Icicle City at <j(964, 422)>.")
    MisNeed(MIS_NEED_ITEM, 1779, 2, 10, 2)

    MisPrize(MIS_PRIZE_ITEM, 4308, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>Hoho! Now I have more space for my ingredient!")
    MisHelpTalk("<t>It's only 2 bottle! Go now.")
    MisResultCondition(HasMission, 154)
    MisResultCondition(HasItem, 1779, 2)
    MisResultAction(TakeItem, 1779, 2)
    MisResultAction(ClearMission, 154)
    MisResultAction(SetRecord, 154)
    MisResultAction(AddExp, 250, 250)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1779)
    TriggerAction(1, AddNextFlag, 154, 10, 2)
    RegCurTrigger(1541)

    DefineMission(186, "Letter for Belinda", 155)

    MisBeginTalk("<t>Please deliver this letter <bBelinda>, she can be found at <j(1360, 519)> in Icicle Castle.")
    MisBeginCondition(HasRecord, 154)
    MisBeginCondition(NoRecord, 155)
    MisBeginCondition(NoMission, 155)
    MisBeginAction(AddMission, 155)
    MisBeginAction(GiveItem, 4138, 1, 4)
    MisCancelAction(ClearMission, 155)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send the letter to <bBelinda> in Icicle Castle")

    MisHelpTalk("<t>Even though I know you but you still need to pay the rent. However I can give you a nice discount.")
    MisResultCondition(AlwaysFailure)

    DefineMission(187, "Letter for Belinda", 155, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>I'm surprised that the Innkeeper recommended you. You must be quite capable.")
    MisResultCondition(NoRecord, 155)
    MisResultCondition(HasMission, 155)
    MisResultCondition(HasItem, 4138, 1)
    MisResultAction(TakeItem, 4138, 1)
    MisResultAction(ClearMission, 155)
    MisResultAction(SetRecord, 155)
    MisResultAction(AddExp, 120, 120)

    DefineMission(188, "Tear", 156)

    MisBeginTalk("<t>Rumors has it that the tears of <rLittle Deer> at <j(1164, 305)> and <rLittle White Deer> at <j(1325, 305)> can be used to make jewellery that will bring luck to those who wear it!<n><t>Can you find 1 for me?")
    MisBeginCondition(HasRecord, 155)
    MisBeginCondition(NoMission, 156)
    MisBeginCondition(NoRecord, 156)
    MisBeginAction(AddMission, 156)
    MisBeginAction(AddTrigger, 1561, TE_GETITEM, 1681, 1)
    MisCancelAction(ClearMission, 156)

    MisNeed(MIS_NEED_DESP,"Collect 1 <yTear> and return to <bBelinda> in Icicle City at <j(1360, 519)>.")
    MisNeed(MIS_NEED_ITEM, 1681, 1, 10, 1)

    MisPrize(MIS_PRIZE_ITEM, 4310, 1, 4)
    MisPrizeSelAll()

    MisResultTalk("<t>This is great! With these I can make a great necklace!")
    MisHelpTalk("<t>You mean you cannot even find a single Tear Drop?")
    MisResultCondition(HasMission, 156)
    MisResultCondition(HasItem, 1681, 1)
    MisResultAction(TakeItem, 1681, 1)
    MisResultAction(ClearMission, 156)
    MisResultAction(SetRecord, 156)
    MisResultAction(AddExp, 400, 400)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 1681)
    TriggerAction(1, AddNextFlag, 156, 10, 1)
    RegCurTrigger(1561)

    DefineMission(189, "Letter for Angela", 157)

    MisBeginTalk("<t>Although we have asked you for help with many of our things, you have also progress much.<n><t>Please take this letter to <bNewbie Guide Angela> at <j(1315, 507)>.<n><t>After locating her, she will give you some new instructions.")
    MisBeginCondition(HasRecord, 156)
    MisBeginCondition(NoRecord, 157)
    MisBeginCondition(NoMission, 157)
    MisBeginAction(AddMission, 157)
    MisBeginAction(GiveItem, 4135, 1, 4)
    MisCancelAction(ClearMission, 157)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to Icicle <bNewbie Guide Angela>.")

    MisHelpTalk("<t><bNewbie Guide Angela> is at <j(1315, 507)>. Please hurry.")
    MisResultCondition(AlwaysFailure)

    DefineMission(190, "Letter for Angela", 157, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Thank you for the letter, it looks like you are quite well liked in Icicle Castle.")
    MisResultCondition(NoRecord, 157)
    MisResultCondition(HasMission, 157)
    MisResultCondition(HasItem, 4135, 1)
    MisResultAction(TakeItem, 4135, 1)
    MisResultAction(ClearMission, 157)
    MisResultAction(SetRecord, 157)
    MisResultAction(AddExp, 200, 200)

    DefineMission(191, "Explorer Promotion", 61, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("Good. Since you are recommended by the Newbie Guide, you should be quite capable. Complete the tasks I have for you if you want to be an Explorer.")
    MisResultCondition(NoRecord, 61)
    MisResultCondition(HasMission, 61)
    MisResultCondition(HasItem, 4119, 1)
    MisResultAction(TakeItem, 4119, 1)
    MisResultAction(ClearMission, 61)
    MisResultAction(SetRecord, 61)

    DefineMission(192, "Explorer Promotion", 158)

    MisBeginTalk("<t>Are you here to sign up for the training to be an Explorer? An Explorer often encounter dangers alone while in the seas. To be able to survive is a basic requirement. If you wish to obtain <rSurvival Guide>, proceed to the Outskirts of Argent City and defeat 12 <rPiglets> at <j(1950, 2563)>. I will tell you your next task once you complete your job.")
    MisBeginCondition(HasRecord, 61)
    MisBeginCondition(NoMission, 158)
    MisBeginCondition(NoRecord, 158)
    MisBeginAction(AddMission, 158)
    MisBeginAction(AddTrigger, 1581, TE_KILL, 237, 12)
    MisCancelAction(ClearMission, 158)

    MisNeed(MIS_NEED_DESP,"Kill 12 <rPiglets> and report back to <bLittle Daniel> in Argent City at <j(2193, 2730)>.")
    MisNeed(MIS_NEED_KILL, 237, 12, 10, 12)

    MisResultTalk("<t>You have done well!<n><t>Since you have passed my test, go forth and take up the last part of your trial now!")
    MisHelpTalk("<t>You have reached the requirement to complete this trial. It is not that easy to obtain the Survival Manual.")
    MisResultCondition(HasMission, 158)
    MisResultCondition(HasFlag, 158, 21)
    MisResultAction(ClearMission, 158)
    MisResultAction(SetRecord, 158)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsMonster, 237)
    TriggerAction(1, AddNextFlag, 158, 10, 12)
    RegCurTrigger(1581)

    DefineMission(193, "Explorer Promotion", 159)

    MisBeginTalk("<t><t>Now, report to <bGeneral William> at <j(2277,2831)> with this letter to prove that you have gotten pass the first part of the trial. He will ask you to take on the final part of the trial and complete the whole advancement.")
    MisBeginCondition(HasRecord, 158)
    MisBeginCondition(NoRecord, 159)
    MisBeginCondition(NoMission, 159)
    MisBeginAction(AddMission, 159)
    MisBeginAction(GiveItem, 4139, 1, 4)
    MisCancelAction(ClearMission, 159)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Send a letter to <bGeneral - William>.")

    MisHelpTalk("<t>Hurry! I hope you can complete it!")
    MisResultCondition(AlwaysFailure)

    DefineMission(194, "Explorer Promotion", 159, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Oh, another adventurous traveler. I hope you are well prepared.")
    MisResultCondition(NoRecord, 159)
    MisResultCondition(HasMission, 159)
    MisResultCondition(HasItem, 4139, 1)
    MisResultAction(TakeItem, 4139, 1)
    MisResultAction(ClearMission, 159)
    MisResultAction(SetRecord, 159)
    MisResultAction(AddExp, 100, 100)

    DefineMission(195, "Explorer Promotion", 160)

    MisBeginTalk("<t>Head north-east from <pSilver Mine> and obtain 2 <yPoisoned Fruits> from <rMarsh Spirit> at <j(1978,2643) and you'll pass the trial.")
    MisBeginCondition(HasRecord, 159)
    MisBeginCondition(NoMission, 160)
    MisBeginCondition(NoRecord, 160)
    MisBeginAction(AddMission, 160)
    MisBeginAction(AddTrigger, 1601, TE_GETITEM, 1595, 2)
    MisCancelAction(ClearMission, 160)

    MisNeed(MIS_NEED_DESP,"Collect 2 <yPoisoned Fruits> and return to <bWilliam> at <j(2277,2831)>.")
    MisNeed(MIS_NEED_ITEM, 1595, 2, 10, 2)

    MisResultTalk("<t>Not bad, you have succeeded!<n><t>Congratulations! A journey of dangers and mystery is waiting for you, though my only hope is that you don't become a pirate...")
    MisHelpTalk("<t>If you want to give up, talk to me directly.")
    MisResultCondition(HasMission, 160)
    MisResultCondition(HasItem, 1595, 2)
    MisResultAction(TakeItem, 1595, 2)
    MisResultAction(ClearMission, 160)
    MisResultAction(SetRecord, 160)
    MisResultAction(AddExp, 300, 300)

    InitTrigger()
    TriggerCondition(1, IsItem, 1595)
    TriggerAction(1, AddNextFlag, 160, 10, 2)
    RegCurTrigger(1601)

    DefineMission(196, "Explorer Promotion", 161)

    MisBeginTalk("<t>Take this <rSurivial Compass> and give it to <bLittle Daniel> at <j(2193,2730)>. It will prove that you have passed all of the test to be a qualified Explorer.")
    MisBeginCondition(HasRecord, 160)
    MisBeginCondition(NoRecord, 161)
    MisBeginCondition(NoMission, 161)
    MisBeginAction(AddMission, 161)
    MisBeginAction(GiveItem, 3962, 1, 4)
    MisCancelAction(ClearMission, 161)
    MisBeginBagNeed(1)

    MisNeed(MIS_NEED_DESP,"Bring <rSurvival Compass> to <bLittle Daniel> in Argent City at <j(2193, 2730)>.")

    MisHelpTalk("<t>Hurry up and go. Don't you wish to become an Explorer?")
    MisResultCondition(AlwaysFailure)

    DefineMission(197, "Explorer Promotion", 161, COMPLETE_SHOW)

    MisBeginCondition(AlwaysFailure)

    MisResultTalk("<t>Congratulations! You are now a qualified Explorer! Your adventuring will lead you to many places around the world, quite the person of envy I must say!<n><t>(From now on, you can accept class quests from <bLittle Daniel>.<n><t>Also, you can buy Explorer's weapon from <bBlacksmith Goldie>, defensive items from <bTailor Granny Nila> and also skills books from <bGrocer Jimberry>. Dont forget!)")
    MisResultCondition(NoRecord, 161)
    MisResultCondition(HasMission, 161)
    MisResultCondition(HasItem, 3962, 1)
    MisResultAction(TakeItem, 3962, 1)
    MisResultAction(ClearMission, 161)
    MisResultAction(SetRecord, 161)
    MisResultAction(AddExp, 100, 100)
    MisResultAction(ClearFightSkill, 475)
    MisResultAction(SetProfession, 4)
    MisResultAction(GiveItem, 867, 1, 4)
    MisResultAction(GiveItem, 3227, 1, 4)
    MisResultAction(GiveItem, 15073, 1, 4, 0)
    MisResultBagNeed(3)

    DefineMission(198, "Low Lv Commerce", 162)

    MisBeginTalk("<t> If you wish to obtain the Low Lv Commerce Permit, then you'll need to bring 40 pieces of Wood to me in exchange for it.<n><t>(Obtain the woods from players or chop for it provided you have learnt woodcutting. Equip an axe to start chopping in any plantation. Woodcutting skill books are sold at all Grocer<n><t>To learn woodcutting requires at least 1 lifeskill point which can be earned by doing Story Quest. Obtain the quest from the Newbie Guide of your respective starting city once you have reached Lv 10.)")
    MisBeginCondition(NoMission, 162)
    MisBeginCondition(NoItem, 4605, 1)
    MisBeginCondition(LvCheck, ">", 19)
    MisBeginAction(AddMission, 162)
    MisBeginAction(AddTrigger, 1621, TE_GETITEM, 4543, 40)
    MisCancelAction(ClearMission, 162)

    MisNeed(MIS_NEED_DESP,"Help <bTrader Sidorf> at <j(799, 3659)> to gather 40 pieces of Wood.")
    MisNeed(MIS_NEED_ITEM, 4543, 40, 10, 40)

    MisResultTalk("<t>Good! You have done what I have requested. Now this permit is yours.")
    MisHelpTalk("<t>Why haven't you started? Don't you want this Commerce Permit?")
    MisResultCondition(HasMission, 162)
    MisResultCondition(HasItem, 4543, 40)
    MisResultAction(TakeItem, 4543, 40)
    MisResultAction(ClearMission, 162)
    MisResultAction(SetRecord, 162)
    MisResultAction(GiveItem, 4605, 1, 4)
    MisResultAction(SetTradeItemLevel, 1)
    MisResultBagNeed(1)

    InitTrigger()
    TriggerCondition(1, IsItem, 4543)
    TriggerAction(1, AddNextFlag, 162, 10, 40)
    RegCurTrigger(1621)

    DefineMission(199, "Mid Lv Commerce", 163)

    MisBeginTalk("<t>If you wish to obtain the Mid Lv Commerce Permit, then you'll need to bring 40 pieces of Crystal Ores to me in exchange for it.<n><t>(Obtain the ores from players or mine for it provided you have learnt mining. Equip a pickaxe to start mining in any ore field. Mining skill books are sold at all Grocer<n><t>To learn mining requires at least 1 lifeskill point which can be earned by doing Story Quest. Obtain the quest from the Newbie Guide of your respective starting city once you have reached Lv 10.)<n><t>You can also purchase a Gold Pickaxe from the Item Mall which allow you to gather at a faster rate.")
    MisBeginCondition(NoMission, 163)
    MisBeginCondition(HasItem, 4605, 1)
    MisBeginCondition(TradeItemDataCheck, ">", 99)
    MisBeginCondition(LvCheck, ">", 39)
    MisBeginCondition(TradeItemLevelCheck, "=", 1)
    MisBeginAction(AddMission, 163)
    MisBeginAction(AddTrigger, 1631, TE_GETITEM, 4546, 40)
    MisCancelAction(ClearMission, 163)

    MisNeed(MIS_NEED_DESP,"Help <bTrader Sidorf> in Shaitan City at <j(799, 3659)> to gather 40 Crystal Ores.")
    MisNeed(MIS_NEED_ITEM, 4546, 40, 10, 40)

    MisResultTalk("<t>Ah. Since you have helped me, this Commerce Permit is yours!")
    MisHelpTalk("<t>Why haven't you started? Don't you want this Commerce Permit?")
    MisResultCondition(HasMission, 163)
    MisResultCondition(HasItem, 4546, 40)
    MisResultCondition(HasItem, 4605, 1)
    MisResultAction(TakeItem, 4546, 40)
    MisResultAction(ClearMission, 163)
    MisResultAction(SetRecord, 163)
    MisResultAction(SetTradeItemLevel, 2)

    InitTrigger()
    TriggerCondition(1, IsItem, 4546)
    TriggerAction(1, AddNextFlag, 163, 10, 40)
    RegCurTrigger(1631)

    DefineMission(149, "High Lv Commerce", 164)

    MisBeginTalk("<t>I have with me a High Lv Commerce Permit. If you want it, you will have to complete some task for me. If you can get me 40 Energy Ore, I will give it to you.")
    MisBeginCondition(NoMission, 164)
    MisBeginCondition(HasItem, 4605, 1)
    MisBeginCondition(TradeItemDataCheck, ">", 399)
    MisBeginCondition(LvCheck, ">", 59)
    MisBeginCondition(TradeItemLevelCheck, "=", 2)
    MisBeginAction(AddMission, 164)
    MisBeginAction(AddTrigger, 1641, TE_GETITEM, 4544, 40)
    MisCancelAction(ClearMission, 164)

    MisNeed(MIS_NEED_DESP,"Gather 40 Energy Ores for <bTrader Sidorf> in Shaitan City at <j(799, 3659)>.")
    MisNeed(MIS_NEED_ITEM, 4544, 40, 10, 40)

    MisResultTalk("<t>Great! Since you have done what I requested you, this High Lv Commerce Permit is yours!")
    MisHelpTalk("<t>Why haven't you started? Don't you want this Commerce Permit?")
    MisResultCondition(HasMission, 164)
    MisResultCondition(HasItem, 4544, 40)
    MisResultCondition(HasItem, 4605, 1)
    MisResultAction(TakeItem, 4544, 40)
    MisResultAction(ClearMission, 164)
    MisResultAction(SetRecord, 164)
    MisResultAction(SetTradeItemLevel, 3)

    InitTrigger()
    TriggerCondition(1, IsItem, 4544)
    TriggerAction(1, AddNextFlag, 164, 10, 40)
    RegCurTrigger(1641)
end
RobinMission039()
