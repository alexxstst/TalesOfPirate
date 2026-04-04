print("-- [Loading] Mission Script [07]")

DefineMission( 900, "Pet Event", 900)

MisBeginTalk( "<t>Want a beautiful pet? Then you will have to take my test.<n><t>Go outside of town to kill 10 Fox Taoists and bring back the Exorcism Bell.<n><t>I will give you a chance to obtain a beautiful pet.")
MisBeginCondition(LvCheck, ">", 59 )
MisBeginCondition(HasItem, 0844, 1)
MisBeginCondition(NoMission, 900)
MisBeginAction(AddMission, 900)
MisBeginAction(AddTrigger, 9001, TE_KILL, 748, 10 )
MisCancelAction(ClearMission, 900)

MisNeed(MIS_NEED_DESP,"Kill 10 Fox Taoist for the fortune teller.")	
MisNeed(MIS_NEED_KILL, 748, 10, 10, 10)
 
MisHelpTalk("<t>Go now! For the sake of a new pet!")	
MisResultTalk("<t>You are quite fast!<n><t>If you are lucky, the chest will contain the pet you wanted.")
MisResultCondition(HasMission, 900)
MisResultCondition(HasFlag, 900, 19 )
MisResultCondition(HasItem,0844,1)
MisResultAction(TakeItem, 0844,1)
MisResultAction(ClearMission,900)
MisResultAction(GiveItem, 1852, 1, 4)
MisResultBagNeed(1)
	
InitTrigger() 
TriggerCondition( 1, IsMonster, 748 )	
TriggerAction( 1, AddNextFlag, 900, 10, 10 )
RegCurTrigger(9001)

-------------------------------------------------pet story(2)
DefineMission(901,"Pet Event",901)

MisBeginTalk("<t>Want a beautiful pet? Then you will have to take my test. Go outside of town to kill  10 Fox Taoists and bring back the Exorcism Bell. I will give you a chance to obtain a beautiful pet.")

MisBeginCondition(LvCheck, "<", 60 )
MisBeginCondition(HasItem, 0844, 1)
MisBeginCondition(NoMission,901)
MisBeginAction(AddMission, 901) 
MisBeginAction(AddTrigger, 9011, TE_KILL, 103, 20)
MisBeginAction(AddTrigger, 9012, TE_KILL, 70, 20)
MisBeginAction(AddTrigger, 9013, TE_KILL, 253, 10)
MisBeginAction(AddTrigger, 9014, TE_KILL, 85, 10)
MisBeginAction(AddTrigger, 9015, TE_KILL, 76, 10)
MisCancelAction(ClearMission, 901)

MisNeed(MIS_NEED_DESP,"Help <bOld Man Blurry> at <j(2272, 2700)> to kill 20 <rForest Spirit>, 20 <rLittle Squidy>, 10 <rMud Monster>, 10 <rStramonium> and 10 <rRookie Boxeroo>.")
MisNeed(MIS_NEED_KILL, 103, 20, 10, 20)
MisNeed(MIS_NEED_KILL, 70, 20, 30, 20)
MisNeed(MIS_NEED_KILL, 253, 10, 50, 10)
MisNeed(MIS_NEED_KILL, 85, 10, 60, 10)
MisNeed(MIS_NEED_KILL, 76, 10, 70, 10)

MisHelpTalk("<t>Hurry up for your beautiful pet!")
MisResultTalk("<t>You are quite fast!<n><t>If you are lucky, the chest will contain the pet you wanted.")
MisResultCondition(HasMission, 901)
MisResultCondition(HasFlag, 901, 29)
MisResultCondition(HasFlag, 901, 49)
MisResultCondition(HasFlag, 901, 59)
MisResultCondition(HasFlag, 901, 69)
MisResultCondition(HasFlag, 901, 79)
MisResultCondition(HasItem, 0844, 1)
MisResultAction(TakeItem, 0844, 1)
MisResultAction(ClearMission, 901)
MisResultAction(GiveItem, 1852, 1, 4)
MisResultBagNeed(1) 

InitTrigger() 
TriggerCondition( 1, IsMonster, 103 )	
TriggerAction( 1, AddNextFlag, 901, 10, 20 )
RegCurTrigger(9011)

InitTrigger() 
TriggerCondition( 1, IsMonster, 70 )	
TriggerAction( 1, AddNextFlag, 901, 30, 20 )
RegCurTrigger(9012)

InitTrigger() 
TriggerCondition( 1, IsMonster, 253 )	
TriggerAction( 1, AddNextFlag, 901, 50, 10 )
RegCurTrigger(9013)

InitTrigger() 
TriggerCondition( 1, IsMonster, 85 )	
TriggerAction( 1, AddNextFlag, 901, 60, 10 )
RegCurTrigger(9014)

InitTrigger() 
TriggerCondition( 1, IsMonster, 76 )	
TriggerAction( 1, AddNextFlag, 901, 70, 10 )
RegCurTrigger(9015)


-------------------------------------------------Eating Zongzi during the Dragon Boat Festival
DefineMission( 902, "Fight Evil, Eat Dumplings!", 902)

MisBeginTalk("<t>Oh dear! The river monsters are moving again. I need some things to throw down to the river. Can you help me collect them?")

MisBeginCondition(LvCheck, ">",15  )
MisBeginCondition(NoMission,902)
MisBeginCondition(NoRecord,902)
MisBeginAction(AddMission,902)
MisBeginAction(AddTrigger, 9021, TE_GETITEM, 3116, 10 )		--Elven Fruit
MisBeginAction(AddTrigger, 9022, TE_GETITEM, 3131, 10 )		--Strange fruit
MisBeginAction(AddTrigger, 9023, TE_GETITEM, 4419, 5 )		--Fermented Honey
MisCancelAction(ClearMission, 902)

MisNeed(MIS_NEED_DESP,"Help Qu Yuan find 10 <yElven Fruit>, 10 <yStrange Fruit> and 5 <yFermented Honey>.")
MisNeed(MIS_NEED_ITEM, 3116, 10, 10, 10)
MisNeed(MIS_NEED_ITEM, 3131, 10, 20, 10)
MisNeed(MIS_NEED_ITEM, 4419, 5, 30, 5)

MisHelpTalk("<t>Hurry! I'll be waiting for you! It's 10 <yElven Fruits>, 10 <yStrange Fruits> and 5 <yFermented Honey>.")
MisResultTalk("<t>Hope that these can feed them well and prevent them from creating anymore mischief.")
MisResultCondition(HasMission, 902)
MisResultCondition(NoRecord,902)
MisResultCondition(HasItem, 3116, 10)
MisResultCondition(HasItem, 3131, 10 )
MisResultCondition(HasItem, 4419, 5 )
MisResultAction(TakeItem, 3116, 10 )
MisResultAction(TakeItem, 3131, 10 )
MisResultAction(TakeItem, 4419, 5 )
MisResultAction(ClearMission, 902)
MisResultAction(SetRecord, 902 )


InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 902, 10, 10 )
RegCurTrigger( 9021 )
InitTrigger()
TriggerCondition( 1, IsItem, 3131)	
TriggerAction( 1, AddNextFlag, 902, 20, 10 )
RegCurTrigger( 9022 )
InitTrigger()
TriggerCondition( 1, IsItem, 4419)	
TriggerAction( 1, AddNextFlag, 902, 30, 5 )
RegCurTrigger( 9023 )

-------------------------------------------------- Eating Zongzi during the Dragon Boat Festival
DefineMission( 903, "Fight Evil, Eat Dumplings!", 903)

MisBeginTalk("<t>Oh dear! Those items are not enough! I need more. Can you help me again?")
MisBeginCondition(NoMission,902)
MisBeginCondition(NoMission,903)
MisBeginCondition(HasRecord,902)
MisBeginCondition(NoRecord,903)
MisBeginAction(AddMission,903)
MisBeginAction(AddTrigger, 9031, TE_GETITEM, 1779, 10 )		--Bottle 
MisBeginAction(AddTrigger, 9032, TE_GETITEM, 1584, 20 )		--Poisoned Thorn 
MisBeginAction(AddTrigger, 9033, TE_GETITEM, 1650, 10 )		--Healing Water
MisCancelAction(ClearMission, 903)

MisNeed(MIS_NEED_DESP,"Help Qu Yuan find 10 <yBottle>, 20 <yPoisoned Thorn> and 10 <yHealing Water>.")
MisNeed(MIS_NEED_ITEM, 1779, 10, 10, 10)
MisNeed(MIS_NEED_ITEM, 1584, 20, 20, 20)
MisNeed(MIS_NEED_ITEM, 1650, 10, 40, 10)

MisHelpTalk("<t>Sigh These river creatures")
MisResultTalk("<t>They shouldn't dare to try anything funny this time.")
MisResultCondition(HasMission, 903)
MisResultCondition(NoRecord,903)
MisResultCondition(HasItem, 1779, 10)
MisResultCondition(HasItem, 1584, 20 )
MisResultCondition(HasItem, 1650, 10 )
MisResultAction(TakeItem, 1779, 10 )
MisResultAction(TakeItem, 1584, 20 )
MisResultAction(TakeItem, 1650, 10 )
MisResultAction(ClearMission, 903)
MisResultAction(SetRecord, 903 )


InitTrigger()
TriggerCondition( 1, IsItem, 1779)	
TriggerAction( 1, AddNextFlag, 903, 10, 10 )
RegCurTrigger( 9031 )
InitTrigger()
TriggerCondition( 1, IsItem, 1584)	
TriggerAction( 1, AddNextFlag, 903, 20, 20 )
RegCurTrigger( 9032 )
InitTrigger()
TriggerCondition( 1, IsItem, 1650)	
TriggerAction( 1, AddNextFlag, 903, 40, 10 )
RegCurTrigger( 9033 )


-------------------------------------------------- Eating Zongzi during the Dragon Boat Festival
DefineMission( 904, "Fight Evil, Eat Dumplings!", 904)

MisBeginTalk("<t>Sigh To appease those river monsters, I spent so much effort, resulting in me getting seriously ill. Although I know what can cure my illness, I am unable to get them on my own. Can you get those items back for me to make a cure?")

MisBeginCondition(NoMission,904)
MisBeginCondition(NoRecord,904)
MisBeginCondition(HasRecord,903)
MisBeginAction(AddMission,904)
MisBeginAction(AddTrigger, 9041, TE_GETITEM, 3129, 5 )		--Medicated Grass
MisBeginAction(AddTrigger, 9042, TE_GETITEM, 1681, 5 )		--Tears
MisCancelAction(ClearMission, 904)

MisNeed(MIS_NEED_DESP,"Help Qu Yuan collect 5 Medicated Grass and 5 Tears.")
MisNeed(MIS_NEED_ITEM, 3129, 5, 10, 5)
MisNeed(MIS_NEED_ITEM, 1681, 5, 20, 5)

MisHelpTalk("<t>Sigh I have so much illness")
MisResultTalk("<t>Thank you for helping me. Please take these as rewards.")
MisResultCondition(HasMission, 904)
MisResultCondition(NoRecord,904)
MisResultCondition(HasItem, 3129, 5)
MisResultCondition(HasItem, 1681, 5 )
MisResultAction(TakeItem, 3129, 5 )
MisResultAction(TakeItem, 1681, 5 )
MisResultAction(GiveItem, 263, 3, 4)
MisResultAction(GiveItem, 264, 3, 4)
MisResultAction(GiveItem, 265, 3 ,4)
MisResultAction(ClearMission, 904)
MisResultAction(SetRecord,  904 )
MisResultBagNeed(3)


InitTrigger()
TriggerCondition( 1, IsItem, 3129)	
TriggerAction( 1, AddNextFlag, 904, 10, 5 )
RegCurTrigger( 9041 )
InitTrigger()
TriggerCondition( 1, IsItem, 1681)	
TriggerAction( 1, AddNextFlag, 904, 20, 5 )
 RegCurTrigger( 9042 )
 
-------------------------------------------------- pirate king birthday
DefineMission( 905, "Pirate King Anniversary", 905)
MisBeginTalk("<t>Do you know that not only <bDrunky> loves to eat cakes baked by <bGranny Beldi>, <bPirate King Roland> loves them too! Rumor has it that whoever eats the cake will get 3 bonus stat points! No wonder the <bPirate King> is invincible.<n><t>Don't you want to try too? Not everybody has a chance though. You need to take a 'Cake Sampling Voucher' and go to <bGranny Beldi> for exchange of a slice.")
MisBeginCondition(NoMission,905)
MisBeginCondition(HasItem, 1097, 1 )
MisBeginCondition(NoRecord,905)
MisBeginAction(AddMission,905)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Look for <bGranny Beldi>.")
MisHelpTalk("<t>Hurry and look for the old granny else no more cakes will be left!")

MisResultCondition(AlwaysFailure )

 -------------------------------------------------- pirate king birthday
DefineMission ( 906, "Pirate King Anniversary", 905,COMPLETE_SHOW)
MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Its <bTintin> who recommended you to come. Hmm... He loves to eat my bread. Haha. I'll take the 'Cake Voucher'. Here is your cake.")
MisResultCondition(HasMission,905)
MisResultCondition(HasItem, 1097, 1 )
MisResultAction(ClearMission, 905 )
MisResultAction(TakeItem, 1097, 1 )
MisResultAction(GiveItem, 3338, 1, 4 )
MisResultAction(SetRecord, 905 )
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
MisBeginBagNeed(1)	

-------------------------------Looking for the lost fish of love, looking for the petals
DefineMission( 907, "Search for Flower", 906)

MisBeginTalk( "<t>I miss her a lot! I beg you, please help me find her. I can't do without her. I know she was last sighted at Deep Blue(1333,558). Can you go look around in that area?")
MisBeginCondition(NoRecord, 913 )
MisBeginCondition(NoMission, 906 )
MisBeginAction(AddMission, 906 )
MisCancelAction(ClearMission, 906)
	
MisNeed(MIS_NEED_DESP,"Locate <bFlower> at <j(1333, 558)>.")
MisHelpTalk("<t>Her last known coordinates are <j(1333, 558)> in Deep Blue region.")
MisResultCondition(AlwaysFailure )

---------------------------------------Looking for the lost fish of love, looking for the petals

DefineMission(908,"Search for Flower",906,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>You are a friend of <bLittle Fish>? I'm <bFlower's> friend, the one he has been looking for!")
MisResultCondition(HasMission, 906)
MisBeginCondition(NoRecord, 913)
MisResultAction(ClearMission, 906)
MisResultAction(SetRecord, 913)

----------------------------------Looking for the lost love, the dancing butterfly likes to eat moon cakes
DefineMission ( 909, "Mooncake Affinity", 907)

MisBeginTalk("<t>How can I so easily believe that you were sent by <bLittle Fish>? How about this, I really love to eat mooncake. Help me find 10 mooncakes.")
MisBeginCondition(HasRecord,913)
MisBeginCondition(NoMission,907)
MisBeginCondition(NoRecord,907)
MisBeginAction(AddMission,907)
MisBeginAction(AddTrigger, 9071, TE_GETITEM, 3915, 10)
MisCancelAction(ClearMission, 907)


MisNeed(MIS_NEED_DESP,"Help <bButterfly> to collect 10 <yMooncakes>.")
MisNeed(MIS_NEED_ITEM, 3915, 10, 10, 10)

MisHelpTalk("<t>Need 10 <ymooncakes>!")
MisResultTalk("<t>You seems to be a nice fellow. I love to eat mooncake, thank you!") 

MisResultCondition(HasMission, 907)
MisResultCondition(NoRecord,907)
MisResultCondition(HasItem, 3915, 10)
MisResultAction(TakeItem, 3915, 10 )
MisResultAction(ClearMission, 907)
MisResultAction(SetRecord, 907 )

InitTrigger()
TriggerCondition( 1, IsItem, 3915)	
TriggerAction( 1, AddNextFlag, 907, 10, 10 )
RegCurTrigger( 9071 )

-------------------------------------------------------	The Last Letter of Finding the Lost Petal of Love
DefineMission(910,"Flower's Letter",908)

MisBeginTalk("<t>Not bad, I think I can trust you now so deliver a message to him for me, <bFlower> was a friend of my mine who passed away 2 days ago due to failed treatment of his leukemia. Before <bFlower's> passing, she wrote a letter. Please pass the letter to him so as to let him grief in peace.")

MisBeginCondition(HasRecord, 907)
MisBeginCondition(NoRecord, 908)
MisBeginCondition(NoMission, 908)
MisBeginBagNeed(1)

MisBeginAction(AddMission, 908)
MisBeginAction(GiveItem, 1005,1,4)

MisHelpTalk("<t>May the Goddess bless those sorrowful people. <bLittle Fish> is in Shaitan City at <j(917, 3572)>.")
MisNeed(MIS_NEED_DESP,"Help <bButterfly> to pass <bFlower's> letter to <bLittle Fish>.")

MisCancelAction(ClearMission, 908)

MisResultCondition(AlwaysFailure)

-----------------------------------The Last Letter of Finding the Lost Petal of Love
DefineMission( 911, "Flower's Letter", 908, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )
	
MisResultTalk("<t>What? It is really a letter from her.<n><t>Contents of the letter as follows: My <bLittle Fish>, by the time you have read this letter, I am already watching you from above. I do not wish for you to see my pale face, nor do I wish that you feel anguish, hurt and pain because of me. It is then I decided to leave you so that you will forget me. The only thing that you can do now is to hold me dear in your heart forever.<n><t>Little Fish, I will give you my blessings . Time passes by but yet love remains eternal.")
MisResultCondition(HasMission, 908)
MisResultCondition(NoRecord,908)
MisResultCondition(HasItem, 1005, 1)
MisResultAction(TakeItem, 1005, 1)
MisResultAction(GiveItem, 1006, 1, 4)
MisResultAction(ClearMission, 908)
MisResultAction(SetRecord, 908)

-------------------------------------------------------------Looking for the lost fish of love
DefineMission ( 912, "Reverse Love Potion", 909)

MisBeginTalk("<t>I heard that theres a thing called <yReverse Love Potion>, and only <bDitto> at <j(2250, 2770)> in Argent City knows how to concoct it. How I wish for a taste of it to wipe all my woes away..")

MisBeginCondition(HasRecord,908)
MisBeginCondition(NoRecord,909)
MisBeginCondition(NoMission,909)
MisBeginAction(AddMission,909)
MisCancelAction(ClearMission, 909)

MisHelpTalk("<t>Sigh... How to forgot the sorrow? Only by using the <yLove Reverse Potion>.")
MisNeed(MIS_NEED_DESP,"Help <bLittle Fish> to look for <yReverse Love Potion>.")

MisResultCondition(AlwaysFailure)

------------------------------------------------------------------------Looking for the lost fish of love
DefineMission(913,"Reverse Love Potion", 909,COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)
MisResultTalk("<t>You are a friend of <bLittle Fish>?<n><t>I have heard their story... Sad indeed. You are a helpful person.")
MisResultCondition(HasMission, 909)
MisResultCondition(NoRecord, 909)
MisResultAction(ClearMission, 909)
MisResultAction(SetRecord, 909)

-------------------------------------------------------------Looking for lost love, making forgetful water
DefineMission ( 914, "Decoct Reverse Love Potion", 910)

MisBeginTalk("<t>You are here to seek <yReverse Love Potion>? The recipe for <yReverse Love Potion> is complicated. Get me 1 <yvial of Pure Water>, 1 <yHeart of Naiad>, 2 <yStrands of Medicated Grass>, 3 <yVials of Healing Water> and return here.")

MisBeginCondition(NoMission,910)
MisBeginCondition(HasRecord,909)
MisBeginCondition(NoRecord,910)
MisBeginAction(AddMission,910)
MisBeginAction(AddTrigger, 9101, TE_GETITEM, 1649, 1)
MisBeginAction(AddTrigger, 9102, TE_GETITEM, 4418, 1)
MisBeginAction(AddTrigger, 9103, TE_GETITEM, 3129, 2)
MisBeginAction(AddTrigger, 9104, TE_GETITEM, 1650, 3)	
MisCancelAction(ClearMission, 910)

MisNeed(MIS_NEED_DESP,"You need 1 <yPure Water>, 1 <yHeart of Naiad>, 2 <yMedicated Grass> and 3 <yHealing Water>.")
MisNeed(MIS_NEED_ITEM, 1649, 1, 10, 1)
MisNeed(MIS_NEED_ITEM, 4418, 1, 20, 1)
MisNeed(MIS_NEED_ITEM, 3129, 2, 30, 2)
MisNeed(MIS_NEED_ITEM, 1650, 3, 40, 3)

MisHelpTalk("<t>Please find 1 <yPure Water>, 1 <yHeart of Naiad>, 2 <yMedicated Grass> and 3 <yHealing Water>.")
MisResultTalk("<t>Very well, these are the materials needed to create the <yReverse Love Potion>.")
MisResultCondition(HasMission, 910)
MisResultCondition(NoRecord,910)
MisResultCondition(HasItem, 1649, 1)
MisResultCondition(HasItem, 4418, 1)
MisResultCondition(HasItem, 3129, 2)
MisResultCondition(HasItem, 1650, 3)
MisResultAction(TakeItem, 1649, 1 )
MisResultAction(TakeItem, 4418, 1)
MisResultAction(TakeItem, 3129, 2 )
MisResultAction(TakeItem,1650, 3 )
MisResultAction(ClearMission, 910)
MisResultAction(SetRecord, 910 )

InitTrigger()
TriggerCondition( 1, IsItem, 1649)	
TriggerAction( 1, AddNextFlag, 910, 10, 1 )
RegCurTrigger( 9101 )
InitTrigger()
TriggerCondition( 1, IsItem, 4418)	
TriggerAction( 1, AddNextFlag, 910, 20, 1 )
RegCurTrigger( 9102 )
InitTrigger()
TriggerCondition( 1, IsItem, 3129)	
TriggerAction( 1, AddNextFlag, 910, 30, 2 )
RegCurTrigger( 9103 )
InitTrigger()
TriggerCondition( 1, IsItem, 1650)	
TriggerAction( 1, AddNextFlag, 910, 40, 3 )
RegCurTrigger( 9104 )


-----------------------------------------------------------------Looking for Lost Love - Purchasing Jade Gold Bottle
DefineMission ( 915, "Purchase Golden Jade Bottle", 911)

MisBeginTalk("<t>The <yReverse Love Potion> is a special kind of liquid. If it is placed in any kind of container, the potion will quickly evapourate. Only the <yGolden Jade Bottle> is able to prevent the potion from evaporating.")

MisBeginCondition(NoMission,911)
MisBeginCondition(HasRecord,910)
MisBeginCondition(NoRecord,911)
MisBeginAction(AddMission,911)
MisBeginAction(AddTrigger, 9111, TE_GETITEM, 1007, 1)
MisCancelAction(ClearMission, 911)
MisNeed(MIS_NEED_DESP,"Requires a <yGolden Jade Bottle>.")
MisNeed(MIS_NEED_ITEM, 1007, 1, 80, 1)

MisHelpTalk("<t>Purchase the <yGolden Jade Bottle> from Item Mall.")
MisResultTalk("<t>I can help you make the <yReverse Love Potion> if I have the <yGolden Jade Bottle>.")


MisResultCondition(HasMission, 911)
MisResultCondition(NoRecord,911)
MisResultCondition(HasItem, 1007, 1)
MisResultAction(TakeItem, 1007, 1 )
MisResultAction(ClearMission, 911)
MisResultAction(SetRecord,  911 )


InitTrigger()
TriggerCondition( 1, IsItem, 1007)	
TriggerAction( 1, AddNextFlag, 911, 80, 1 )
RegCurTrigger( 9111 )

-------------------------------------------------------------------Looking for lost love
DefineMission(916,"Love in the Past",912)

MisBeginTalk("<t>Take it. The <yLove Reversal Potion> is ready. Hopefull it will heal <yLittle Fish> at <y(917, 3572)>.")

MisBeginCondition(HasRecord, 911)
MisBeginCondition(NoRecord, 912)
MisBeginCondition(NoMission, 912)
MisBeginBagNeed(1)

MisBeginAction(AddMission, 912)
MisBeginAction(GiveItem, 1008,1,4)

MisHelpTalk("<t>Hurry and save the heart-broken person!")
MisNeed(MIS_NEED_DESP,"Bring the <yReverse Love Potion> to <bLittle Fish> to heal his sorrow.")

MisCancelAction(ClearMission, 912)
MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------Looking for lost love
DefineMission( 917, "Love in the Past", 912, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )


MisResultTalk("<t>So this is the mythical <yReverse Love Potion>! Take this <yHeart of Innocence> as a gift from me to you, it represents true love.<n><t>Sigh! Will I really forget her if I use this potion?")
MisResultCondition(HasMission, 912)
MisResultCondition(NoRecord,912)
MisResultCondition(HasItem, 1008, 1)
MisResultAction(TakeItem, 1008, 1 )
MisResultAction(GiveItem, 1009, 1 ,4)
MisResultAction(ClearMission, 912)
MisResultAction(SetRecord, 912 )
MisResultAction(ClearRecord, 909)
MisResultAction(ClearRecord, 910)
MisResultAction(ClearRecord, 911)
MisResultAction(ClearRecord, 912)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 1008)	
TriggerAction( 1, AddNextFlag, 912, 10, 1 )
RegCurTrigger( 9121 ) 
--------------------------------------------------------------------love reborn

DefineMission( 918, "Love Revival", 914 )
MisBeginTalk("<t>Child, if you heart is empty and lifeless, look for the <yReverse Love Potion>. I will restore your heart to its former glory!")
MisBeginCondition(NoMission, 914)
MisBeginCondition(HasItem,1010,1)------------With a pure heart, the exhausted heart after use
MisBeginCondition(NoRecord,914)
MisBeginAction(AddMission,914)
MisBeginAction(AddTrigger, 9141, TE_GETITEM, 1008, 1)
MisCancelAction(ClearMission, 914)

MisNeed(MIS_NEED_DESP,"Requires a <yReverse Love Potion> to moist a <yWithered Heart>.")
MisNeed(MIS_NEED_ITEM, 1008, 1, 10, 1)

MisHelpTalk("<t>Find a <yReverse Love Potion>, bring it to <bMysterious Granny> along with <yWithered Heart>.")
MisResultTalk("<t>Very good child, this is the <yLove Reversal Potion>. Drink it, and face rebirth bravely.")

MisResultCondition(HasMission, 914)
MisResultCondition(NoRecord,914)
MisResultCondition(HasItem, 1008, 1)
MisResultCondition(HasItem, 1010, 1)------------Withered Heart
MisResultAction(TakeItem, 1008, 1 )
MisResultAction(TakeItem, 1010, 1 )
MisResultAction(GiveItem, 1013,1,4)------------Heart of Reborn
MisResultAction(ClearMission, 914)
MisResultAction(SetRecord, 914)
MisResultAction(ClearRecord, 914)---------------can be repeated

InitTrigger()
TriggerCondition( 1, IsItem, 1008)	
TriggerAction( 1, AddNextFlag, 914, 10, 1 )
RegCurTrigger( 9141 )

-----------------------------------------------------------------------bloody heels
DefineMission( 919, "Bloodied High Heels", 915)

MisBeginTalk( "<t>OuchI got hit on the head by a high heel shoe thrown by my boss... Look at the wound What?! You don't believe me? I still have the medical report I got from <bNurse - Gina> in Argent City at <j(2244, 2770)>. The dressing is done by her. Check it out. Ouch...Ouch...")
MisBeginCondition(NoRecord, 915 )
MisBeginCondition(NoMission, 915 )
MisBeginAction(AddMission, 915 )
MisBeginAction(GiveItem, 1026,1,4)------Baiyin Hospital Injury Form
MisCancelAction(ClearMission, 915)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"Look for <bNurse Gina> in Argent City to verify.")
MisHelpTalk("<t>The <bNurse Gina> is located at <j(2244, 2770)>.")
MisResultCondition(AlwaysFailure )

------------------------------------------------------------------------------bloody heels
DefineMission( 920, "Bloodied High Heels", 915, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Why is there always someone getting hurt recently!")
MisResultCondition(HasMission, 915)
MisResultCondition(NoRecord,915)
MisResultCondition(HasItem, 1026, 1)
MisResultAction(TakeItem, 1026, 1 )
MisResultAction(ClearMission, 915)
MisResultAction(SetRecord, 915 )
 
-------------------------------------------------------------------------------Ask the patrolman

DefineMission( 921, "Verify with the Patroller", 916 )

MisBeginTalk("<t>Yes, this Medical Report comes from me. The injury seems to be caused by a high heel shoe and the victim is a skinny man. However, I do not know the cause of the injury. It seems that <bGuard - Michael> in Shaitan City at <j(959, 3549)> is also investigating this matter.")
MisBeginCondition(NoMission, 916)
MisBeginCondition(HasRecord,915)
MisBeginCondition(NoRecord,916)
MisBeginAction(AddMission, 916 )
MisCancelAction(ClearMission, 916)
MisResultCondition(AlwaysFailure )
-------------------------------------------------------------------Ask the patrolman
DefineMission( 922, "Verify with the Patroller", 916, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Its not easy being a patrol guard, theres always trouble and little pay!")
MisResultCondition(HasMission, 916)
MisResultCondition(NoRecord,916)
MisResultAction(ClearMission, 916)
MisResultAction(SetRecord, 916 )

 
---------------------------------------------------------------High heels case investigation
DefineMission( 923, "Investigation to the Case of the High Heels", 917)

MisBeginTalk( "<t>Hmm...I have been investigating for some time about the incident you brought up. This is a very troublesome issue. Both parties claim to be the victim, insisting that the opposite party started the fight. What a problematic case! Look, this is the medical report of the lady boss, although the details are abit vague, they still can be used as clues.")
MisBeginCondition(NoRecord, 917 )
MisBeginCondition(NoMission, 917)
MisBeginCondition(HasRecord, 916 )
MisBeginAction(AddMission, 917 )
MisBeginAction(GiveItem, 1027,1,4)------Female Boss's Injury Form
MisBeginAction(AddTrigger, 9171, TE_GETITEM, 1030, 1)
MisBeginBagNeed(1)

MisCancelAction(ClearMission, 917)
MisNeed(MIS_NEED_ITEM, 1030, 1, 10, 1)

MisResultTalk("<t>Only those who are brave dare to uphold rightness!")
MisHelpTalk("<t>Use the Medical Report to summon a monster and defeat it.")
MisResultCondition(HasMission,  917)
MisResultCondition(NoRecord , 917)
MisResultCondition(HasItem,1030,1 )
MisResultAction(TakeItem, 1030, 1 )-------Bloodied High Heels
MisResultAction(ClearMission,   917)
MisResultAction(SetRecord,  917 )

InitTrigger()
TriggerCondition( 1, IsItem, 1030)	
TriggerAction( 1, AddNextFlag, 917, 10, 1 )
RegCurTrigger( 9171 )
-------------------------------------------------------------------------High heels case murder weapon
DefineMission( 924, "Weapon of the Case of the High Heels", 918)

MisBeginTalk( "<t>Surprisingly the weapon used is this High Heels. I will safekeep it with me to keep it from harming other people. Thank you!")
MisBeginCondition(NoRecord, 918 )
--MisBeginCondition(NoMission, 918 )
MisBeginCondition(HasRecord, 917 )
--MisBeginAction(AddMission, 918 )
MisBeginAction(GiveItem, 1029,1,4)---------- Red High Heels
MisBeginAction(AddExp,500,500)
--MisBeginAction(ClearMission, 918 )
MisBeginAction(SetRecord, 918 )
MisCancelAction(ClearMission, 918)
MisBeginBagNeed(1)

MisHelpTalk("<t>Theres justice in the world.")

MisResultCondition(AlwaysFailure )
-------------------------------------------------------------------1

DefineMission( 6000, "Phoenix Rebirth", 1300 )
MisBeginTalk("<t>Mortal, have you heard of Phoenix Rebirth? A Phoenix will be reborn every 500 years. All aspiring pirates also wish to be reborn. However, since the last Sacred War, the Rebirth Stone is broken up into pieces and lost in the mortal world. These fragments are being guarded by some people. If you can collect these fragments, I will help you attain rebirth. There is a <rTower in Ascaron> which seems suspicious. You might want to start from there.")
MisBeginCondition(NoMission, 1300)
MisBeginCondition(NoRecord, 1300)
MisBeginCondition(HasCredit, 9999)
MisBeginAction(AddMission,1300)
MisBeginAction(AddTrigger, 13001, TE_GETITEM, 2226, 1)
MisBeginAction(AddTrigger, 13002, TE_GETITEM, 2227, 1)
MisBeginAction(AddTrigger, 13003, TE_GETITEM, 2228, 1)
MisBeginAction(AddTrigger, 13004, TE_GETITEM, 2229, 1)
MisBeginAction(AddTrigger, 13005, TE_GETITEM, 2230, 1)
MisBeginAction(AddTrigger, 13006, TE_GETITEM, 2231, 1)
MisBeginAction(AddTrigger, 13007, TE_GETITEM, 2232, 1)
MisBeginAction(AddTrigger, 13008, TE_GETITEM, 2233, 1)

MisCancelAction(ClearMission, 1300)

MisNeed(MIS_NEED_DESP,"Bring back 8 Fragments of the Rebirth Stone.")
MisNeed(MIS_NEED_ITEM, 2226, 1, 10, 1)
MisNeed(MIS_NEED_ITEM, 2227, 1, 20, 1)
MisNeed(MIS_NEED_ITEM, 2228, 1, 30, 1)
MisNeed(MIS_NEED_ITEM, 2229, 1, 40, 1)
MisNeed(MIS_NEED_ITEM, 2230, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 2231, 1, 60, 1)
MisNeed(MIS_NEED_ITEM, 2232, 1, 70, 1)
MisNeed(MIS_NEED_ITEM, 2233, 1, 80, 1)


MisHelpTalk("<t>The 8 pieces were scattered all over the world.You have to be mentally prepared to embark on a long journey.")
MisResultTalk("<t>You are God's miracle, wait for the grand moment of rebirth.")

MisResultCondition(HasMission, 1300)
MisResultCondition(NoRecord,1300)
MisResultCondition(HasItem, 2226, 1)
MisResultCondition(HasItem, 2227, 1)
MisResultCondition(HasItem, 2228, 1)
MisResultCondition(HasItem, 2229, 1)
MisResultCondition(HasItem, 2230, 1)
MisResultCondition(HasItem, 2231, 1)
MisResultCondition(HasItem, 2232, 1)
MisResultCondition(HasItem, 2233, 1)


MisResultAction(TakeItem, 2226, 1 )
MisResultAction(TakeItem, 2227, 1 )
MisResultAction(TakeItem, 2228, 1 )
MisResultAction(TakeItem, 2229, 1 )
MisResultAction(TakeItem, 2230, 1 )
MisResultAction(TakeItem, 2231, 1 )
MisResultAction(TakeItem, 2232, 1 )
MisResultAction(TakeItem, 2233, 1 )


MisResultAction(GiveItem, 2235,1,4)------------Rebirth Stone
MisResultAction(ClearMission, 1300)
MisResultAction(SetRecord, 1300)
MisResultBagNeed(1)



InitTrigger()
TriggerCondition( 1, IsItem, 2226)	
TriggerAction( 1, AddNextFlag, 1300, 10, 1 )
RegCurTrigger( 13001 )

InitTrigger()
TriggerCondition( 1, IsItem, 2227)	
TriggerAction( 1, AddNextFlag, 1300, 20, 1 )
RegCurTrigger( 13002 )
InitTrigger()
TriggerCondition( 1, IsItem, 2228)	
TriggerAction( 1, AddNextFlag, 1300, 30, 1 )
RegCurTrigger( 13003 )
InitTrigger()
TriggerCondition( 1, IsItem, 2229)	
TriggerAction( 1, AddNextFlag, 1300, 40, 1 )
RegCurTrigger( 13004 )
InitTrigger()
TriggerCondition( 1, IsItem, 2230)	
TriggerAction( 1, AddNextFlag, 1300, 50, 1 )
RegCurTrigger( 13005 )
InitTrigger()
TriggerCondition( 1, IsItem, 2231)	
TriggerAction( 1, AddNextFlag, 1300, 60, 1 )
RegCurTrigger( 13006 )
InitTrigger()
TriggerCondition( 1, IsItem, 2232)	
TriggerAction( 1, AddNextFlag, 1300, 70, 1 )
RegCurTrigger( 13007 )

InitTrigger()
TriggerCondition( 1, IsItem, 2233)	
TriggerAction( 1, AddNextFlag, 1300, 80, 1 )
RegCurTrigger( 13008 )

-------------------------------------------------Snowball Fight	
DefineMission (5500, "Snowball Fight", 818)

MisBeginTalk("<t>What a wonderful Christmas Day! There is snow all over! You want to have a snowball fight? You need to kill 20 <rDry Mystic Shrub>, then I can make you some.<ySnowballs>.")

MisBeginCondition(NoMission,818)
MisBeginCondition(NoRecord,818)
MisBeginAction(AddMission,818)
MisBeginAction(AddTrigger, 8181, TE_KILL, 218, 20 )
MisCancelAction(ClearMission, 818)

MisNeed(MIS_NEED_DESP,"Kill 20 <rMystic Shrub>.")
MisNeed(MIS_NEED_KILL, 218, 20, 10, 20)

MisHelpTalk("<t>Christmas is coming!")
MisResultTalk("<t>Not bad, you have already done the work. These snowballs were made for you. Merry Christmas!")


MisResultCondition(HasMission, 818)
MisResultCondition(NoRecord,818)
MisResultCondition(HasFlag, 818, 29 )
MisResultAction(ClearMission, 818)
MisResultAction(SetRecord,  818 )
MisResultAction(ClearRecord, 818)---------------can be repeated
MisResultAction(GiveItem, 2896, 10, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 218)	
TriggerAction( 1, AddNextFlag, 818, 10, 20 )
RegCurTrigger( 8181 )

-------------------------------------------------snowball fight	
DefineMission (5501, "Snowball Fight", 819)

MisBeginTalk("<t> What a wonderful Christmas Day! There is snow all over! You want to have a snowball fight? You need to kill 20 <rMystic Shrub>, then I can make you some.<ySnowballs>.")

MisBeginCondition(NoMission,819)
MisBeginCondition(NoRecord,819)
MisBeginAction(AddMission,819)
MisBeginAction(AddTrigger, 8191, TE_KILL, 75, 20 )
MisCancelAction(ClearMission, 819)

MisNeed(MIS_NEED_DESP,"Kill 20 <rDry Mystic Shrub>.")
MisNeed(MIS_NEED_KILL, 75, 20, 10, 20)

MisHelpTalk("<t> Christmas is coming.")
MisResultTalk("<t> Not bad, you have already done the work. These snowballs were made for you. Merry Christmas!")


MisResultCondition(HasMission, 819)
MisResultCondition(NoRecord,819)
MisResultCondition(HasFlag, 819, 29 )
MisResultAction(ClearMission, 819)
MisResultAction(SetRecord,  819 )
MisResultAction(ClearRecord, 819)---------------can be repeated
MisResultAction(GiveItem, 2896, 10, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 75)	
TriggerAction( 1, AddNextFlag, 819, 10, 20 )
RegCurTrigger( 8191 )

-------------------------------------------------snowball fight	
DefineMission (5502, "Snowball Fight", 820)

MisBeginTalk("<t> What a wonderful Christmas Day! There is snow all over! You want to have a snowball fight? You need to kill 20 <rSnowy Mystic Shrub>, then I can make you some.<ySnowballs>.")

MisBeginCondition(NoMission,820)
MisBeginCondition(NoRecord,820)
MisBeginAction(AddMission,820)
MisBeginAction(AddTrigger, 8201, TE_KILL, 216, 20 )
MisCancelAction(ClearMission, 820)

MisNeed(MIS_NEED_DESP,"Kill 20 <rSnowy Mystic Shrub>.")
MisNeed(MIS_NEED_KILL, 216, 20, 10, 20)

MisHelpTalk("<t> Christmas is coming.")
MisResultTalk("<t> Not bad, you have already done the work. These snowballs were made for you. Merry Christmas!")


MisResultCondition(HasMission, 820)
MisResultCondition(NoRecord,820)
MisResultCondition(HasFlag, 820, 29 )
MisResultAction(ClearMission, 820)
MisResultAction(SetRecord,  820 )
MisResultAction(ClearRecord, 820)---------------can be repeated
MisResultAction(GiveItem, 2896, 10, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 216)	
TriggerAction( 1, AddNextFlag, 820, 10, 20 )
RegCurTrigger( 8201 )

-----------------------------------------------Warrior's Proof
DefineMission(6001,"Mark of a Warrior",1302)

  MisBeginTalk("<t>Use your wisdom and bravery to eradicate these demons from this world! 15 <rNimble Forest Hunter>, 15 <rNimble Shadow Hunter>, 15 <rVicious Grassland Elder>, 15 <rEvil Guardian Angel>, 15 <rRuthless Shadow Hunter> and 15 <rRuthless Forest Hunter>.")

  MisBeginCondition(HasRecord,1301)
  MisBeginCondition(NoRecord,1302)
  MisBeginCondition(NoMission,1302)
  MisBeginCondition(HasCredit,9999 )
 -- MisBeginAction(TakeCredit, 9999 )
  --MisBeginAction(DelRoleCredit, 9999 )  
  MisBeginAction(AddMission,1302)
  MisBeginAction(AddTrigger, 13021, TE_KILL,525, 15)
  MisBeginAction(AddTrigger, 13022, TE_KILL,526, 15)
  MisBeginAction(AddTrigger, 13023, TE_KILL, 532, 15)
  MisBeginAction(AddTrigger, 13024, TE_KILL, 550, 15)
  MisBeginAction(AddTrigger, 13025, TE_KILL, 554, 15)
  MisBeginAction(AddTrigger, 13026, TE_KILL, 553, 15)
  MisCancelAction(ClearMission, 1302)

  MisNeed(MIS_NEED_DESP,"Kill 15 <rNimble Forest Hunter>, 15 <rNimble Shadow Hunter>, 15 <rVicious Grassland Elder>, 15 <rEvil Guardian Angel>, 15 <rRuthless Shadow Hunter> and 15 <rRuthless Forest Hunter>!")
  MisNeed(MIS_NEED_KILL, 525, 15, 10, 15)
  MisNeed(MIS_NEED_KILL, 526, 15, 30, 15)
  MisNeed(MIS_NEED_KILL, 532, 15, 50, 15)
  MisNeed(MIS_NEED_KILL, 550, 15, 70, 15)
  MisNeed(MIS_NEED_KILL, 554, 15, 90, 15)
  MisNeed(MIS_NEED_KILL, 553, 15, 110, 15)

MisPrize(MIS_PRIZE_ITEM, 2228, 1, 4)
MisPrizeSelAll()
  
  MisHelpTalk("<t>Kill them.")  
  MisResultTalk("<t>You have good skills. Learn from me from now on. This fragment represents Wisdom. Take it to fulfill your dream. Rumored that rebirth allows you to reselect class")
  MisResultCondition(HasMission,1302 )
  MisResultCondition(NoRecord,1302)
  MisResultCondition(HasFlag, 1302, 24)
  MisResultCondition(HasFlag, 1302, 44)
  MisResultCondition(HasFlag, 1302, 64)
  MisResultCondition(HasFlag, 1302, 84)
  MisResultCondition(HasFlag, 1302, 104)
  MisResultCondition(HasFlag, 1302, 124)
  --MisResultAction(GiveItem, 2228, 1, 4 )
  MisResultAction(ClearMission, 1302 )
  MisResultAction(SetRecord, 1302)
  MisResultBagNeed(3)

  InitTrigger()
  TriggerCondition( 1, IsMonster, 525 )
  TriggerAction( 1, AddNextFlag, 1302, 10, 15 )
  RegCurTrigger( 13021 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 526 )
  TriggerAction( 1, AddNextFlag, 1302, 30, 15 )
  RegCurTrigger( 13022 )
   InitTrigger()
  TriggerCondition( 1, IsMonster,532  )
  TriggerAction( 1, AddNextFlag, 1302, 50, 15 )
  RegCurTrigger( 13023 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 550 )
  TriggerAction( 1, AddNextFlag, 1302, 70, 15 )
  RegCurTrigger( 13024 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 554 )
  TriggerAction( 1, AddNextFlag, 1302, 90, 15 )
  RegCurTrigger( 13025 )
   InitTrigger()
  TriggerCondition( 1, IsMonster,553)
  TriggerAction( 1, AddNextFlag, 1302, 110, 15 )
  RegCurTrigger( 13026 )

------------------------------------------------------little town mystery
DefineMission( 6002, "Little Mystery Man", 1303 )

MisBeginTalk( "<t>Judging from your reputation, I am sure that guy will not reject you...However, I did not say that he has part of the fragment.")
MisBeginCondition(NoRecord,   1303)
MisBeginCondition(HasRecord, 1302)
MisBeginCondition(NoMission,  1303)
MisBeginAction(AddMission,  1303)
MisCancelAction(ClearMission, 1303)
MisNeed(MIS_NEED_DESP,"Search for Eastern town's mysterious guardian.")
MisHelpTalk("<t>I can only tell you this much.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------little town mystery
DefineMission( 6003, "Little Mystery Man", 1303, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )
	
MisResultTalk("<t>I'm already this fat, and yet I'm still recognizable?!")
MisResultCondition(NoRecord,  1303)
MisResultCondition(HasMission,  1303)
MisResultAction(SetRecord,  1303)
MisResultAction(ClearMission,  1303)

------------------------------------------------------------simple task
DefineMission( 6004, "Simple Mission", 1304 )

MisBeginTalk( "<t>I never like to trouble others, help me find 30 <yGigantic Stramonium Flower>, 30 <yQuality Caviar>, 30 <yCompressed Energy III> as a gift, hehe...")
MisBeginCondition(NoRecord, 1304)
MisBeginCondition(HasRecord, 1303)
MisBeginCondition(NoMission, 1304)
MisBeginAction(AddMission, 1304)
MisBeginAction(AddTrigger, 13041, TE_GETITEM, 4730, 30 )
MisBeginAction(AddTrigger, 13042, TE_GETITEM, 1358, 30 )
MisBeginAction(AddTrigger, 13043, TE_GETITEM, 2619, 30 )
MisCancelAction(ClearMission, 1304)

MisNeed(MIS_NEED_ITEM, 4730, 30, 10, 30)
MisNeed(MIS_NEED_ITEM, 1358, 30, 50, 30)
MisNeed(MIS_NEED_ITEM, 2619, 30, 90, 30)

MisResultTalk("<t>These are the ingredients in making Mao Wine! I gave those away when I quit drinking to go on a diet.")
MisHelpTalk("<t>I discovered that quit drinking doesn't help in getting thin so...")
MisResultCondition(HasMission, 1304)
MisResultCondition(HasItem, 4730, 30 )
MisResultCondition(HasItem, 1358, 30 )
MisResultCondition(HasItem, 2619, 30 )
MisResultAction(TakeItem, 4730, 30 )
MisResultAction(TakeItem, 1358, 30 )
MisResultAction(TakeItem, 2619, 30 )
MisResultAction(ClearMission, 1304)
MisResultAction(SetRecord, 1304 )


InitTrigger()
TriggerCondition( 1, IsItem, 4730)	
TriggerAction( 1, AddNextFlag, 1304, 10, 30 )
RegCurTrigger( 13041 )

InitTrigger()
TriggerCondition( 1, IsItem, 1358)	
TriggerAction( 1, AddNextFlag, 1304, 50, 30 )
RegCurTrigger( 13042 )

InitTrigger()
TriggerCondition( 1, IsItem, 2619)	
TriggerAction( 1, AddNextFlag, 1304, 90, 30 )
RegCurTrigger( 13043 )




------------------------------------------------------------quit drinking
DefineMission( 6005, "Day of Abstinence", 1305)

MisBeginTalk( "<t>This is really a good wine...But it is too troublesome to brew on my own. Go get me a few bottles more.")
MisBeginCondition(NoRecord, 1305)
MisBeginCondition(HasRecord, 1304)
MisBeginCondition(NoMission, 1305)
MisBeginAction(AddMission, 1305)
MisBeginAction(AddTrigger, 13051, TE_GETITEM, 1087, 30 )		
MisCancelAction(ClearMission, 1305)

MisNeed(MIS_NEED_ITEM, 1087, 30, 10, 30)

MisResultTalk("<t>Good wine! Good wine!")
MisHelpTalk("<t>Be quick,if my urge to drink passes, don't blame me for being nasty.")
MisResultCondition(HasMission, 1305)
MisResultCondition(HasItem, 1087, 30 )
MisResultAction(TakeItem, 1087, 30 )
MisResultAction(ClearMission, 1305)
MisResultAction(SetRecord, 1305 )


InitTrigger()
TriggerCondition( 1, IsItem, 1087)	
TriggerAction( 1, AddNextFlag, 1305, 10, 30 )
RegCurTrigger( 13051 )

------------------------------------------------------------quit drinking
DefineMission( 6006, "Day of Abstinence", 1306)

MisBeginTalk( "<t>Someone famous once told me, to see how the taste of <yDukan Wine> and <yMao Wine> differs, you need to taste it yourself. You don't mind getting me a few bottles of <yDukan Wine>, do you?")
MisBeginCondition(NoRecord, 1306)
MisBeginCondition(HasRecord, 1305)
MisBeginCondition(NoMission, 1306)
MisBeginAction(AddMission, 1306)
MisBeginAction(AddTrigger, 13061, TE_GETITEM, 1088, 20 )		--
MisCancelAction(ClearMission, 1306)

MisNeed(MIS_NEED_ITEM, 1088, 20, 10, 20)

MisResultTalk("<t>Only <yDukan Wine> can relieve woes!")
MisHelpTalk("<t>I advise against giving me any fake items. I just drank some wine, and I may do something nasty.")
MisResultCondition(HasMission, 1306)
MisResultCondition(HasItem, 1088, 20 )
MisResultAction(TakeItem, 1088, 20 )
MisResultAction(ClearMission, 1306)
MisResultAction(SetRecord, 1306 )


InitTrigger()
TriggerCondition( 1, IsItem, 1088)	
TriggerAction( 1, AddNextFlag, 1306, 10, 20 )
RegCurTrigger( 13061 )

------------------------------------------------------------quit drinking
DefineMission( 6007, "Day of Abstinence", 1307)

MisBeginTalk( "<t>After tasting them, I think Mao Wine would suit me more. I think I would need to stock up 20 bottles in the wine cellar. You think so too, don't you?")
MisBeginCondition(NoRecord, 1307)
MisBeginCondition(HasRecord, 1306)
MisBeginCondition(NoMission, 1307)
MisBeginAction(AddMission, 1307)
MisBeginAction(AddTrigger, 13071, TE_GETITEM, 1087, 20 )		--
MisCancelAction(ClearMission, 1307)

MisNeed(MIS_NEED_ITEM, 1087, 20, 10, 20)

MisResultTalk("<t>Wine gets better as it gets older... Ah...")
MisHelpTalk("<t>Famous quotes: Never argue with a drunk.")
MisResultCondition(HasMission, 1307)
MisResultCondition(HasItem, 1087, 20 )
MisResultAction(TakeItem, 1087, 20 )
MisResultAction(ClearMission, 1307)
MisResultAction(SetRecord, 1307 )

InitTrigger()
TriggerCondition( 1, IsItem, 1087)	
TriggerAction( 1, AddNextFlag, 1307, 10, 20 )
RegCurTrigger( 13071 )

------------------------------------------------------------quit drinking
DefineMission( 6008, "Day of Abstinence", 1008)

MisBeginTalk( "<t>I need to quit drinking! I think I have been drinking too much lately. I have signs of internal bleeding. Nobody stop me! I heard that Ginseng Wine is a good remedy for internal bleeding. Hmm...You know what i mean?")
MisBeginCondition(NoRecord, 1008)
MisBeginCondition(HasRecord, 1307)
MisBeginCondition(NoMission, 1008)
MisBeginAction(AddMission, 1008)
MisBeginAction(AddTrigger, 10081, TE_GETITEM, 1089, 15 )		--
MisCancelAction(ClearMission, 1008)

MisNeed(MIS_NEED_ITEM, 1089, 15, 10, 15)

MisResultTalk("<t>I have not tasted Tiger Bone Tonic in a long time")
MisHelpTalk("<t>Young man, its not good to drink too much,it might hinder you")
MisResultCondition(HasMission, 1008)
MisResultCondition(HasItem, 1089, 15 )
MisResultAction(TakeItem, 1089, 15 )
MisResultAction(ClearMission, 1008)
MisResultAction(SetRecord, 1008 )

InitTrigger()
TriggerCondition( 1, IsItem, 1089)	
TriggerAction( 1, AddNextFlag, 1008, 10, 15 )
RegCurTrigger( 10081 )


------------------------------------------------------Seeking Rubik's Cube Guide
DefineMission( 6009, "Seek out Demonic Guide", 1009 )

MisBeginTalk( "<t>To express my thanks, I will give you this piece of fragment that symbolize Virtue. In my drunken state, I remember seeing a guy in <pDemonic World> having part of this fragment as well. You will have to look around to complete the piece. I heard that rebirth enable you to redistribute your stats...")
MisBeginCondition(NoRecord,   1009)
MisBeginCondition(HasRecord, 1008)
MisBeginCondition(NoMission,  1009)

--MisPrize(MIS_PRIZE_ITEM, 2227, 1, 4)
--MisPrizeSelAll()

MisBeginAction(AddMission,  1009)
MisBeginAction(GiveItem, 2227, 1, 4 )
MisCancelAction(ClearMission, 1009)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"Search for <pDemonic World's> <rMysterious Guardian>.")
MisHelpTalk("<t>I really can't recall.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------Seeking Rubik's Cube Guide
DefineMission( 6010, "Seek out Demonic Guide", 1009, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )
	
MisResultTalk("<t>I may be the guide of <pDemonic World> but I do not conduct tours. I only give directions, as I often get lost...")
MisResultCondition(NoRecord,  1009)
MisResultCondition(HasMission,  1009)
MisResultAction(SetRecord,  1009)
MisResultAction(ClearMission,  1009)
----------------------------------------------------------Caribbean Sightseeing Tour
DefineMission( 6011, "Caribbean 1 day tour", 1010 )
MisBeginTalk("<t>The incident happened a long time ago. This fragment has given me endless nightmares. I wish to return it before I retire, however, I have no means to get to <pHeaven>. Have you been to the <pCaribbean>? If you can defeat <rDeathsoul Commander>, I will consider giving this fragment to you.")
			
MisBeginCondition(NoMission, 1010)
MisBeginCondition(HasRecord, 1009)
MisBeginCondition(NoRecord,1010)	
MisBeginAction(AddMission,1010)
MisBeginAction(AddTrigger, 10101, TE_KILL, 807, 1)--Undead Commander(807)  
MisCancelAction(ClearMission, 1010)

MisNeed(MIS_NEED_DESP,"Kill <rDeathsoul Commander>.")
MisNeed(MIS_NEED_KILL, 807,1, 10, 1)


MisResultTalk("<t>The scenery of Caribbean is beautiful, but the monsters are too fearsome!")
MisHelpTalk("<t>This task should be simple.")
MisResultCondition(HasMission,  1010)
MisResultCondition(HasFlag, 1010, 10)
MisResultCondition(NoRecord , 1010)
MisResultAction(ClearMission,  1010)
MisResultAction(SetRecord,  1010 )

InitTrigger()
TriggerCondition( 1, IsMonster, 807)	
TriggerAction( 1, AddNextFlag, 1010, 10, 1 )
RegCurTrigger( 10101 )

----------------------------------------------------------Caribbean Sightseeing Tour
DefineMission( 6012, "Carribean 2 days tour", 1011 )
MisBeginTalk("<t>I wonder why do I hate those from the Caribbean. It would be better off if all are dead... Especially that <rBarborosa>.")
			
MisBeginCondition(NoMission, 1011)
MisBeginCondition(NoRecord,1011)
MisBeginCondition(HasRecord, 1010)
MisBeginAction(AddMission,1011)
MisBeginAction(AddTrigger, 10111, TE_KILL, 805, 1)--Barborosa(805)
MisCancelAction(ClearMission, 1011)

MisNeed(MIS_NEED_DESP,"Kill <rBarborosa>!")
MisNeed(MIS_NEED_KILL, 805,1, 10, 1)


MisResultTalk("<t>I knew you would want to start a killing spree in Carribean.")
MisHelpTalk("Leave none alive!")
MisResultCondition(HasMission,  1011)
MisResultCondition(HasFlag, 1011, 10)
MisResultCondition(NoRecord , 1011)
MisResultAction(ClearMission,  1011)
MisResultAction(SetRecord,  1011 )

InitTrigger()
TriggerCondition( 1, IsMonster, 805)	
TriggerAction( 1, AddNextFlag, 1011, 10, 1 )
RegCurTrigger( 10111 )

----------------------------------------------------------Caribbean Sightseeing Tour
DefineMission( 6013, "Caribbean Tour 2", 1012 )
MisBeginTalk("<t>I can see that you aren't satisfied with a 2 days tour of Carribean Island. No need to thank me, I've already applied for another tour for you. Hehe! Kill the irritating <rKraken>!")
			
MisBeginCondition(NoMission, 1012)
MisBeginCondition(NoRecord,1012)
MisBeginCondition(HasRecord, 1011)
MisBeginAction(AddMission,1012)
MisBeginAction(AddTrigger, 10121, TE_KILL, 796, 1)---Kraken

MisCancelAction(ClearMission, 1012)

MisNeed(MIS_NEED_DESP,"Kill <rKraken>!")
MisNeed(MIS_NEED_KILL, 796,1, 10, 1)

MisPrize(MIS_PRIZE_ITEM, 2226, 1, 4)
MisPrizeSelAll()

MisResultTalk("<t>The world was never this beautiful. Your ability has convinced me. Here take this Shard of Love and I hope that you will find your purest love.")
MisHelpTalk("<t>Let that octopus disappear from this world!")
MisResultCondition(HasMission,  1012)
MisResultCondition(HasFlag, 1012, 10)
MisResultCondition(NoRecord , 1012)
--MisResultAction(GiveItem, 2226, 1, 4 )
MisResultAction(ClearMission, 1012)
MisResultAction(SetRecord, 1012)

InitTrigger()
TriggerCondition( 1, IsMonster, 796)	
TriggerAction( 1, AddNextFlag, 1012, 10, 1 )
RegCurTrigger( 10121 )

------------------------------------------------------who is the guardian
DefineMission( 6014, "Who is the Guardian", 1013 )

MisBeginTalk( "<t>My friend, seek it out fast, rebirth will allow you to soar above the rest! I vaguely remembered that Icicle City do have a guardian")
MisBeginCondition(NoRecord,   1013)
MisBeginCondition(HasRecord, 1012)
MisBeginCondition(NoMission,  1013)
MisBeginAction(AddMission,  1013)
MisCancelAction(ClearMission, 1013)


MisNeed(MIS_NEED_DESP,"Search for the next guardian.")
MisHelpTalk("<t>Still not moving out? You want another 3 days tour?")

MisResultCondition(AlwaysFailure)

-------------------------------------------------Valentine's Day gift----------Sand spring supply station to talk to NPC Feifei: (male character)
DefineMission (5503, "Valentine's Day gift", 825)

MisBeginTalk("<t>Want to leave some pleasant memories for your love one? Help me collect 11 <yRose> and 10 <yHeart of Naiad>. I'll give you a surprise that your love one will definitely like.")

MisBeginCondition(NoMission,825)
MisBeginCondition(NoRecord,825)
MisBeginCondition(LvCheck, ">",30)
MisBeginCondition(NoChaType,3)
MisBeginCondition(NoChaType,4)
MisBeginAction(AddMission,825)
MisBeginAction(AddTrigger, 8251, TE_GETITEM, 3343, 11)
MisBeginAction(AddTrigger, 8252, TE_GETITEM, 4418, 10)
MisCancelAction(ClearMission, 825)

MisNeed(MIS_NEED_DESP,"Collect 11 <yRose> and 10 <yHeart of Naiad>.")
MisNeed(MIS_NEED_ITEM, 3343, 11, 10, 11)
MisNeed(MIS_NEED_ITEM, 4418, 10, 30, 10 )

MisHelpTalk("<t>Will it snow this Valentine's Day?")
MisResultTalk("<t>Very good, this is Gift of the Beauty. Give it to the person you love. She will definitely be touched.")


MisResultCondition(HasMission, 825)
MisResultCondition(NoRecord,825)
MisResultCondition(HasItem, 3343, 11)
MisResultCondition(HasItem, 4418, 10)
MisResultAction(TakeItem, 3343, 11 )
MisResultAction(TakeItem, 4418, 10 )
MisResultAction(ClearMission, 825)
MisResultAction(SetRecord,  825 )
MisResultAction(GiveItem, 2904, 1, 4)
MisResultBagNeed(1)


InitTrigger()
TriggerCondition( 1, IsItem, 3343)	
TriggerAction( 1, AddNextFlag, 825, 10, 11 )
RegCurTrigger( 8251 )

InitTrigger()
TriggerCondition( 1, IsItem, 4418)	
TriggerAction( 1, AddNextFlag, 825, 30, 10 )
RegCurTrigger( 8252 )

-------------------------------------------------Valentine's Day gift----------Ice pole supply station to talk to NPC Fadir: (female character)
DefineMission (5504, "Valentine's Day gift", 826)

MisBeginTalk("<t>Want to leave some pleasant memories for your love one? Help me collect 11 <yRose> and 10 <yHeart of Naiad>. I'll give you a surprise that your love one will definitely like.")

MisBeginCondition(NoMission,826)
MisBeginCondition(NoRecord,826)
MisBeginCondition(LvCheck, ">",30)
MisBeginCondition(NoChaType,1)
MisBeginCondition(NoChaType,2)
MisBeginAction(AddMission,826)
MisBeginAction(AddTrigger, 8261, TE_GETITEM, 3343, 11)
MisBeginAction(AddTrigger, 8262, TE_GETITEM, 4418, 10)
MisCancelAction(ClearMission, 826)

MisNeed(MIS_NEED_DESP,"Collect 11 <yRose> and 10 <yHeart of Naiad>.")
MisNeed(MIS_NEED_ITEM, 3343, 11, 10, 11)
MisNeed(MIS_NEED_ITEM, 4418, 10, 30, 10 )

MisHelpTalk("<t>Will it snow this Valentine's Day?")
MisResultTalk("<t>Very good, heres Gift of the Hunk. Give this to your love one and he will be moved!")


MisResultCondition(HasMission, 826)
MisResultCondition(NoRecord,826)
MisResultCondition(HasItem, 3343, 11)
MisResultCondition(HasItem, 4418, 10)
MisResultAction(TakeItem, 3343, 11 )
MisResultAction(TakeItem, 4418, 10 )
MisResultAction(ClearMission, 826)
MisResultAction(SetRecord,  826 )
MisResultAction(GiveItem, 2905, 1, 4)
MisResultBagNeed(1)


InitTrigger()
TriggerCondition( 1, IsItem, 3343)	
TriggerAction( 1, AddNextFlag, 826, 10, 11 )
RegCurTrigger( 8261 )

InitTrigger()
TriggerCondition( 1, IsItem, 4418)	
TriggerAction( 1, AddNextFlag, 826, 30, 10 )
RegCurTrigger( 8262 )


-----------------------------------------------Seven Pigs Kaitai---------Mara Yilan
DefineMission(5505,"Fortune of 7 Pigs",827)

  MisBeginTalk("<t>Want to gain the favor of the <bPiggy God>? Go forth and hunt 1 of each: <rSnowy Piglet>, <rPiglet>, <rTusk Battle Boar>, <rAir Porky>, <rMad Boar>, <rCombat Piglet> and <rSnowy Tusk Boar>.")

  MisBeginCondition(NoRecord,827)
  MisBeginCondition(NoMission,827)
  MisBeginAction(AddMission,827)
  MisBeginAction(AddTrigger, 8271, TE_KILL,239, 1)
  MisBeginAction(AddTrigger, 8272, TE_KILL,237, 1)
  MisBeginAction(AddTrigger, 8273, TE_KILL, 264, 1)
  MisBeginAction(AddTrigger, 8274, TE_KILL, 295, 1)
  MisBeginAction(AddTrigger, 8275, TE_KILL, 64, 1)
  MisBeginAction(AddTrigger, 8276, TE_KILL, 296, 1)
  MisBeginAction(AddTrigger, 8277, TE_KILL, 144, 1)
  MisCancelAction(ClearMission, 827)

  MisNeed(MIS_NEED_DESP,"<rSnowy Piglet>, <rPiglet>, <rTusk Battle Boar>, <rAir Porky>, <rMad Boar>, <rCombat Piglet> and <rSnowy Tusk Boar>, 1 of each.")
  MisNeed(MIS_NEED_KILL, 239, 1, 10, 1)
  MisNeed(MIS_NEED_KILL, 237, 1, 20, 1)
  MisNeed(MIS_NEED_KILL, 264, 1, 30, 1)
  MisNeed(MIS_NEED_KILL, 295, 1, 40, 1)
  MisNeed(MIS_NEED_KILL, 64, 1, 50, 1)
  MisNeed(MIS_NEED_KILL, 296, 1, 60, 1)
  MisNeed(MIS_NEED_KILL, 144, 1, 70, 1)


  MisHelpTalk("<t>Happy New Year, Fortune of 7 Pigs.")  
  MisResultTalk("<t>Well done!")
  MisResultCondition(HasMission,827 )
  MisResultCondition(NoRecord,827)
  MisResultCondition(HasFlag, 827, 10)
  MisResultCondition(HasFlag, 827, 20)
  MisResultCondition(HasFlag, 827, 30)
  MisResultCondition(HasFlag, 827, 40)
  MisResultCondition(HasFlag, 827, 50)
  MisResultCondition(HasFlag, 827, 60)
  MisResultCondition(HasFlag, 827, 70)
  MisResultAction(GiveItem, 855, 10, 4 )
  MisResultAction(ClearMission, 827 )
  MisResultAction(SetRecord, 827)
  MisResultBagNeed(1)

  InitTrigger()
  TriggerCondition( 1, IsMonster, 239 )
  TriggerAction( 1, AddNextFlag, 827, 10, 1 )
  RegCurTrigger( 8271 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 237 )
  TriggerAction( 1, AddNextFlag, 827, 20, 1 )
  RegCurTrigger( 8272 )
   InitTrigger()
  TriggerCondition( 1, IsMonster,264  )
  TriggerAction( 1, AddNextFlag, 827, 30, 1 )
  RegCurTrigger( 8273 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 295 )
  TriggerAction( 1, AddNextFlag, 827, 40, 1 )
  RegCurTrigger( 8274 )
   InitTrigger()
  TriggerCondition( 1, IsMonster, 64 )
  TriggerAction( 1, AddNextFlag, 827, 50, 1 )
  RegCurTrigger( 8275 )
   InitTrigger()
  TriggerCondition( 1, IsMonster,296)
  TriggerAction( 1, AddNextFlag, 827, 60, 1 )
  RegCurTrigger( 8276 )

  InitTrigger()
  TriggerCondition( 1, IsMonster,144)
  TriggerAction( 1, AddNextFlag, 827, 70, 1 )
  RegCurTrigger( 8277 )

-------------------------------------------------Lucky Pig----------Mara Yilan
DefineMission (5506, "Lucky Piggy", 828)

MisBeginTalk("<t>But without <yLucky Piggy Clover>, <bPiggy God> will not bother with you! Go collect <yLucky Piggy Clover>! <bObtainable from Item Mall>. You will stand a chance to obtain <yGem of Colossus>, <yGem of Rage>, <yGem of Striking>, <yGem of the Wind>, <yGem of Soul>, <yGoddess's Pouch>, <yFirst Prize>, etc! There will be a <r30,000G handling fee>.")

MisBeginCondition(NoMission,828)
MisBeginCondition(HasRecord,827)
MisBeginCondition(NoRecord,828)
MisBeginAction(AddMission,828)
MisBeginAction(AddTrigger, 8281, TE_GETITEM, 2908, 1)
MisCancelAction(ClearMission, 828)

MisNeed(MIS_NEED_ITEM, 2908, 1, 10, 1)


MisHelpTalk("<t>Hurry on, surprise wait for no one.")
MisResultTalk("<t>Very well. The <bPiggy God> has bestowed upon you this mystery treasure chest. Open it to take a look!")

MisResultCondition(HasMission, 828)
MisResultCondition(NoRecord,828)
MisResultCondition(HasItem, 2908, 1)
MisResultCondition(HasMoney, 30000)
MisResultAction(TakeItem, 2908, 1 )
MisResultAction(TakeMoney,30000 )
MisResultAction(ClearMission, 828)
MisResultAction(SetRecord,  828 )
MisResultAction(GiveItem, 2909, 1, 4)
MisResultAction(ClearRecord, 828)---------------can be repeated
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 2908)	
TriggerAction( 1, AddNextFlag, 828, 10, 1 )
RegCurTrigger( 8281 )


-----------------------------------------------Pirate King New Year's Gift--------- Beginner's Guide to Silver Mall
DefineMission(5507,"Auspicious Gift",829)

  MisBeginTalk("<t> It's the Year of The Pig. Go catch 20 <rPiglets> and you can receive a <yTOP's Auspicious packet>.")

  MisBeginCondition(NoRecord,829)
  MisBeginCondition(NoMission,829)
  MisBeginCondition(LvCheck, ">",45)
  MisBeginAction(AddMission,829)
  MisBeginAction(AddTrigger, 8291, TE_KILL,237, 20)
  MisCancelAction(ClearMission, 829)

  MisNeed(MIS_NEED_DESP,"Catch 20 <rPiglets>.")
  MisNeed(MIS_NEED_KILL, 237, 20, 10, 20)
 
  MisHelpTalk("<t>Time waits for no one.")  
  MisResultTalk("<t>Not bad! This is Tales of Pirates's Piggy Year <yAuspicious Bag>! If you open it on Lunar New Year's Eve on the 17th of February from 23:00 to 01:00 of 18th February, you stand a chance to win a super gift.")
  MisResultCondition(HasMission,829 )
  MisResultCondition(NoRecord,829)
  MisResultCondition(HasFlag, 829, 29)
  MisResultAction(GiveItem, 2910, 1, 4 )
  MisResultAction(ClearMission, 829 )
  MisResultAction(SetRecord, 829)
  MisResultBagNeed(1)

  InitTrigger()
  TriggerCondition( 1, IsMonster, 237 )
  TriggerAction( 1, AddNextFlag, 829, 10, 20 )
  RegCurTrigger( 8291 )
   
-------------------------------------------------What is love----------Mara Yilan
DefineMission (5508, "What is love", 830)

MisBeginTalk("<t>Love transends all things! If you believe in fate, I can help you fulfil your long cherished wish! But before that, you must first pass my test, to prove your worth.")

MisBeginCondition(NoMission,830)
MisBeginCondition(LvCheck, ">",30)
MisBeginCondition(NoRecord,830)
MisBeginAction(AddMission,830)
MisBeginAction(AddTrigger, 8301, TE_GETITEM, 4418, 10 )
MisCancelAction(ClearMission, 830)

MisNeed(MIS_NEED_ITEM, 4418, 10, 10, 10 )


MisHelpTalk("<t>Don't let your love ones wait too long!")
MisResultTalk("<t>Very well! You have done well! This is a Chest of Fate. Open it and you will not be alone anymore!")

MisResultCondition(HasMission, 830)
MisResultCondition(NoRecord,830)
MisResultCondition(HasItem, 4418, 10)
MisResultAction(TakeItem, 4418, 10 )
MisResultAction(ClearMission, 830)
MisResultAction(SetRecord,  830 )
MisResultAction(GiveItem, 2916, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4418)	
TriggerAction( 1, AddNextFlag, 830, 10, 10 )
RegCurTrigger( 8301 )


-------------------------------------------------Love in a Fallen City ------- Mara Yilan
DefineMission (5509, "Beautiful Love", 831)

MisBeginTalk("<t>Youngster! Seek out your true love today! Look for the other half with a similar <yLove Number> and also a <yRed Rope> to tie your fate together. I will await for both of you to bring me <yLove Amulets>.")

MisBeginCondition(NoMission,831)
MisBeginCondition(NoMission,832)
MisBeginCondition(HasRecord,830)
MisBeginCondition(NoRecord,831)
MisBeginCondition(NoRecord,832)
MisBeginCondition(NoChaType,1)
MisBeginCondition(NoChaType,2)
MisBeginCondition(HasItem, 2902, 1)
MisBeginAction(AddTrigger, 8311, TE_GETITEM, 2903, 1 )
MisBeginAction(AddTrigger, 8312, TE_GETITEM, 2845, 1 )
MisBeginAction(AddMission,831)
MisCancelAction(ClearMission, 831)


MisNeed(MIS_NEED_ITEM, 2903, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 2845, 1, 20, 1 )


MisHelpTalk("<t>Remember that you and your loved one need to bring <yAmulets with the same Love Number>.")
MisResultTalk("<t>Both of you are a matching couple! Congratulations! Allow me to award both of you with 2 <yChest of Gown>. A person can only open it once! May God bless you!")

MisResultCondition(HasMission, 831)
MisResultCondition(NoRecord,831)
MisResultCondition(HasItem, 2902, 1)
MisResultCondition(HasItem, 2903, 1)
MisResultCondition(HasItem, 2845, 1)
MisResultCondition(CheckItem,2902,2903)
MisResultAction(TakeItem, 2902, 1 )
MisResultAction(TakeItem, 2903, 1 )
MisResultAction(TakeItem, 2845, 1 )
MisResultAction(ClearMission, 831)
MisResultAction(SetRecord,  831 )
MisResultAction(GiveItem, 2915, 2, 4)
MisResultAction(Starteffect,  370 )
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 2903)	
TriggerAction( 1, AddNextFlag, 831, 10, 1 )
RegCurTrigger( 8311 )

InitTrigger()
TriggerCondition( 1, IsItem, 2845)	
TriggerAction( 1, AddNextFlag, 831, 20, 1 )
RegCurTrigger( 8312 )
	-------------------------------------------------Love in a Fallen City ------- Mara Yilan
DefineMission (5536, "Beautiful Love", 832)

MisBeginTalk("<t>Youngster! Seek out your true love today! Look for the other half with a similar <yLove Number> and also a <yRed Rope> to tie your fate together. I will await for both of you to bring me <yLove Amulets>.")

MisBeginCondition(NoMission,832)
MisBeginCondition(NoMission,831)
MisBeginCondition(HasRecord,830)
MisBeginCondition(NoRecord,831)
MisBeginCondition(NoRecord,832)
MisBeginCondition(NoChaType,3)
MisBeginCondition(NoChaType,4)
MisBeginCondition(HasItem, 2903, 1)
MisBeginAction(AddTrigger, 8321, TE_GETITEM, 2902, 1 )
MisBeginAction(AddTrigger, 8322, TE_GETITEM, 2845, 1 )
MisBeginAction(AddMission,832)
MisCancelAction(ClearMission, 832)


MisNeed(MIS_NEED_ITEM, 2902, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 2845, 1, 20, 1 )


MisHelpTalk("<t>Remember that you and your loved one need to bring <yAmulets with the same Love Number>.")
MisResultTalk("<t>Both of you are a matching couple! Congratulations! Allow me to award both of you with 2 <yChest of Gown>. A person can only open it once! May God bless you!")

MisResultCondition(HasMission, 832)
MisResultCondition(NoRecord,832)
MisResultCondition(HasItem, 2902, 1)
MisResultCondition(HasItem, 2903, 1)
MisResultCondition(HasItem, 2845, 1)
MisResultCondition(CheckItem,2902,2903)
MisResultAction(TakeItem, 2902, 1 )
MisResultAction(TakeItem, 2903, 1 )
MisResultAction(TakeItem, 2845, 1 )
MisResultAction(ClearMission, 832)
MisResultAction(SetRecord,  832 )
MisResultAction(GiveItem, 2915, 2, 4)
MisResultAction(Starteffect,  370 )
MisResultBagNeed(2)


InitTrigger()
TriggerCondition( 1, IsItem, 2902)	
TriggerAction( 1, AddNextFlag, 832, 10, 1 )
RegCurTrigger( 8321 )

InitTrigger()
TriggerCondition( 1, IsItem, 2845)	
TriggerAction( 1, AddNextFlag, 832, 20, 1 )
RegCurTrigger( 8322 )



-------------------------------------------------Challenge Guinness 1	
DefineMission (5510, "Challenge Genesis", 743)

MisBeginTalk("<t>Want to be the number 1 pirate warrior? Feel like challenging your limit? Then come experience the Pirate King 07 Sea Course! You can start from Argent City and travel to <pSara Haven>. From there, the Harbor Operator will inform you of your next location. <rThis Challenge Letter can record your time. Now this letter have to be placed in the first slot of your bag, and never must it be moved throughout the whole journey>, take it.")

MisBeginCondition(NoMission,743)
MisBeginCondition(NoRecord,743)
MisBeginCondition(CheckBagEmp,0)
MisBeginAction(AddMission,743)
MisBeginAction(AddChaItem1, 2911)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"First stop is <pSara Haven>. <bHarbor Operator Whitney> is the one you should look for.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon Equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 1

DefineMission(5511,"Challenge Genesis",743,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Thanks for your trouble!")
MisResultCondition(HasMission, 743)
MisResultCondition(CheckBag, 2911,0,1)
MisResultCondition(NoRecord,743)
MisResultAction(ClearMission, 743)
MisResultAction(SetRecord,743 )

-------------------------------------------------Challenge Guinness 2	
DefineMission (5512, "Challenge Genesis 2", 744)

MisBeginTalk("<t>You are not that fast, someone already pass by here. Here's your pass. Be on your way now, <pHubble Haven's> <bHarbor Operator Dannis> is waiting for you!")

MisBeginCondition(NoMission,744)
MisBeginCondition(HasRecord, 743)
MisBeginCondition(NoRecord,744)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,744)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pHubble Haven>. You will need to locate <bHarbor Operator - Dannis> at <j(136, 3432)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 2

DefineMission(5513,"Challenge Genesis 2",744,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Your sense of timing is really not strong.")
MisResultCondition(HasMission, 744)
MisResultCondition(NoRecord,744)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 744)
MisResultAction(SetRecord,  744 )

-------------------------------------------------Challenge Guinness 3	
DefineMission (5514, "Challenge Genesis", 745)

MisBeginTalk("<t>This is your official letter! Go! <bDidance> at <pGelada Haven> awaits you!")

MisBeginCondition(NoMission,745)
MisBeginCondition(HasRecord, 744)
MisBeginCondition(NoRecord,745)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,745)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pGelada Haven>, you need to locate <bHarbor Operator Didane> at <j(1544, 3712)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 3

DefineMission(5515,"Challenge Genesis",745,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Was your voyage smooth?")
MisResultCondition(HasMission, 745)
MisResultCondition(NoRecord,745)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 745)
MisResultAction(SetRecord,  745 )

-------------------------------------------------Challenge Guinness 4	
DefineMission (5516, "Challenge Genesis 4", 746)

MisBeginTalk("<t>I have been to busy to meet up with <bHarbor Operator - Soc> of <pEthio Haven> lately.")

MisBeginCondition(NoMission,746)
MisBeginCondition(HasRecord, 745)
MisBeginCondition(NoRecord,746)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,746)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pEthio Haven>. You will need to locate <bHarbor Operator Soc> at <j(3194, 3507)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 4

DefineMission(5517,"Challenge Genesis 4",746,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>You should have salvaged some treasures throughout your journey. I heard that the Titanic sunk around here.")
MisResultCondition(HasMission, 746)
MisResultCondition(NoRecord,746)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 746)
MisResultAction(SetRecord,  746 )

-------------------------------------------------Challenge Guinness 5	
DefineMission (5518, "Challenge Genesis 5", 747)

MisBeginTalk("<t>Although the scenary on the sea is great, but time is precious! There's only 1 set of <yBlack Dragon equipment>! <pKaralas Haven> is still far.")

MisBeginCondition(NoMission,747)
MisBeginCondition(HasRecord,746)
MisBeginCondition(NoRecord,747)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,747)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pKarmas Haven>. You will need to locate <bHarbor Operator Odie> at <j(738, 3803)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 5

DefineMission(5519,"Challenge Genesis 5",747,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>You are as slow as a Sandy Tortoise!")
MisResultCondition(HasMission, 747)
MisResultCondition(NoRecord,747)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 747)
MisResultAction(SetRecord,  747 )

-------------------------------------------------Challenge Guinness 6	
DefineMission (5520, "Challenge Genesis6", 748)

MisBeginTalk("<t>The nearby islands may have <rSirens>. My advice is to sail straight to <pSalva Haven>. If you hear any beautiful music, be sure to cover your ears.")

MisBeginCondition(NoMission,748)
MisBeginCondition(HasRecord,747)
MisBeginCondition(NoRecord,748)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,748)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pSalva Haven>, you need to locate <bHarbor Operator Gregory> at <j(194, 1715)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 6

DefineMission(5521,"Challenge Genesis6",748,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Sail and courage is a must for a sea voyage.")
MisResultCondition(HasMission, 748)
MisResultCondition(NoRecord,748)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 748)
MisResultAction(SetRecord,  748 )

-------------------------------------------------Challenge Guinness 7	
DefineMission (5522, "Challenge Genesis 7", 749)

MisBeginTalk("<t>Work harder! Do what an adventurer have to do! Next stop: <pLahu Haven>.")

MisBeginCondition(NoMission,749)
MisBeginCondition(HasRecord,748)
MisBeginCondition(NoRecord,749)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,749)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is at <pLahu Haven>. Look for <bHarbor Operator - Domoru> <j(3498, 923)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 7

DefineMission(5523,"Challenge Genesis 7",749,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Heard there is a lot of challengers? But...")
MisResultCondition(HasMission, 749)
MisResultCondition(NoRecord,749)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 749)
MisResultAction(SetRecord,  749 )



-------------------------------------------------Challenge Guinness 8	
DefineMission (5524, "Challenge Genesis 8", 750)

MisBeginTalk("<t>There is not much adventurer left on <pLahu Haven>. Did any misfortune befallen them? <pAerase Haven's> <bHarbor Operator - Buni> should be all alone!")

MisBeginCondition(NoMission,750)
MisBeginCondition(HasRecord,749)
MisBeginCondition(NoRecord,750)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,750)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is at <pAerase Haven>. Look for <bHarbor Operator - Buni> at <j(2042, 635)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 8

DefineMission(5525,"Challenge Genesis 8",750,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Looks like you are pretty lucky. You didn't met with a whirlpool nor get eaten by a sea monster.")
MisResultCondition(HasMission, 750)
MisResultCondition(NoRecord,750)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 750)
MisResultAction(SetRecord,  750 )

-------------------------------------------------Challenge Guinness 9	
DefineMission (5526, "Challenge Genesis 9", 751)

MisBeginTalk("<t>Thinking of the days that I sailed in the sea... Oh sorry, forgot that you are in a hurry. <bHarbor Operator - Luigi> is waiting for you at <pNorite Harbor Haven>.")

MisBeginCondition(NoMission,751)
MisBeginCondition(HasRecord,750)
MisBeginCondition(NoRecord,751)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,751)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pNorite Harbor Haven>. Look for <bHarbor Operator Luigi> at <j(464, 468)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 9

DefineMission(5527,"Challenge Genesis 9",751,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Buni always like to waste time! I hate this type of person. Don't be wishy washy like him! Okay... I shall say no more")
MisResultCondition(HasMission, 751)
MisResultCondition(NoRecord,751)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 751)
MisResultAction(SetRecord,  751 )
-------------------------------------------------Challenge Guinness 10	
DefineMission (5528, "Challenge Genesis10", 752)

MisBeginTalk("<t>Next stop: <pReagen Haven>.")

MisBeginCondition(NoMission,752)
MisBeginCondition(HasRecord,751)
MisBeginCondition(NoRecord,752)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,752)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <pReagen Haven>. You will have to locate <bHarbor Operator Fardell> at <j(3153, 674)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )
---------------------------------------Challenge Guinness 10

DefineMission(5529,"Challenge Genesis10",752,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>I can't help but mention that you need to move faster!")
MisResultCondition(HasMission, 752)
MisResultCondition(NoRecord,752)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 752)
MisResultAction(SetRecord,  752 )
-------------------------------------------------Challenge Guinness 11	
DefineMission (5530, "Challenge Genesis 11", 753)

MisBeginTalk("<t>Somebody might have reached <yHafta Haven>! Please hurry!")

MisBeginCondition(NoMission,753)
MisBeginCondition(HasRecord,752)
MisBeginCondition(NoRecord,753)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,753)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <yHafta Haven>. You need to find <bHarbor Operator - Whitcombe> at <j(2041, 1355)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 11

DefineMission(5531,"Challenge Genesis 11",753,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Welcome to <yHafta Haven>!")
MisResultCondition(HasMission, 753)
MisResultCondition(NoRecord,753)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 753)
MisResultAction(SetRecord,  753 )

-------------------------------------------------Challenge Guinness 12	
DefineMission (5532, "Challenge Genesis 12", 754)

MisBeginTalk("<t>For honor and glory, you will have to work fast. Now go to <yAlbania Haven>!")

MisBeginCondition(NoMission,754)
MisBeginCondition(HasRecord,753)
MisBeginCondition(NoRecord,754)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,754)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is <yAlbania Haven>. You will need to locate <bHarbor Operator - Daruka> at <j(1497, 1707)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 12

DefineMission(5533,"Challenge Genesis 12",754,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Are you sure that you have tried your best?")
MisResultCondition(HasMission, 754)
MisResultCondition(NoRecord,754)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(ClearMission, 754)
MisResultAction(SetRecord,  754 )
-------------------------------------------------Challenge Guinness 13	
DefineMission (5534, "Challenge Genesis 13", 755)

MisBeginTalk("<t>The goddess of victory is smiling at you! Rush towards the ending point now!")

MisBeginCondition(NoMission,755)
MisBeginCondition(HasRecord,754)
MisBeginCondition(NoRecord,755)
MisBeginCondition(CheckBag, 2911,0,1)
MisBeginAction(AddMission,755)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"Next stop is at <pThundoria Harbor>. Look for <bSailor Dio> at <j(1083, 1285)>.")

MisHelpTalk("<t>Everyone who wishes to obtain <yBlack Dragon equipment> are all fighting for it.")
MisResultCondition(AlwaysFailure )

---------------------------------------Challenge Guinness 13

DefineMission(5535,"Challenge Genesis 13",755,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>You are very brave. You are definitely cut out to be a Pirate. It will be a waste if you don't be one. This <yBawcock Letter> records the timing of your entire journey. Don't forget to collect your prize.")
MisResultCondition(HasMission, 755)
MisResultCondition(NoRecord,755)
MisResultCondition(HasItem,2911,1)
MisResultCondition(CheckBag, 2911,0,1)
MisResultAction(AddChaItem2, 2912)
MisResultAction(ClearMission, 755)
MisResultAction(SetRecord,  755 )
MisResultBagNeed(2)
MisResultAction(ClearRecord, 743)---------------can be repeated
MisResultAction(ClearRecord, 744)---------------can be repeated
MisResultAction(ClearRecord, 745)---------------can be repeated
MisResultAction(ClearRecord, 746)---------------can be repeated
MisResultAction(ClearRecord, 747)---------------can be repeated
MisResultAction(ClearRecord, 748)---------------can be repeated
MisResultAction(ClearRecord, 749)---------------can be repeated
MisResultAction(ClearRecord, 750)---------------can be repeated
MisResultAction(ClearRecord, 751)---------------can be repeated
MisResultAction(ClearRecord, 752)---------------can be repeated
MisResultAction(ClearRecord, 753)---------------can be repeated
MisResultAction(ClearRecord, 754)---------------can be repeated
MisResultAction(ClearRecord, 755)---------------can be repeated

-------------------------------------------------Brawl Hero----------Girl Xindi
DefineMission (5536, "Vampiric Aries Battle Hero", 1060)

MisBeginTalk("<t>A challenging Hero must be a Chaos expert. 10 <yChaos points> should not daunt you.")

MisBeginCondition(NoMission,1060)
MisBeginCondition(HasRecord,833)
MisBeginCondition(NoRecord,1060)
MisBeginAction(AddMission,1060)
MisCancelAction(ClearMission, 1060)

MisHelpTalk("<t>It's only 10 <yChaos points>. Work harder!")
MisResultTalk("<t>I knew 10 <yChaos points> shouldn't be a problem for you.")

MisResultCondition(HasMission, 1060)
MisResultCondition(NoRecord,1060)
MisResultCondition(HasFightingPoint,10 )
MisResultAction(TakeFightingPoint, 10 )
MisResultAction(ClearMission, 1060)
MisResultAction(SetRecord,  1060 )
MisResultAction(GiveItem, 2944, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Hero of Fame----------Girl Xindi
DefineMission (5537, "Vampiric Aries Renown Hero", 1061)

MisBeginTalk("<t>How to do a challenge without 500 reputation points?! I believe you will have a way to gain reputation points, such as getting a disciple")

MisBeginCondition(NoMission,1061)
MisBeginCondition(HasRecord,833)
MisBeginCondition(NoRecord,1061)
MisBeginAction(AddMission,1061)
MisCancelAction(ClearMission, 1061)

MisHelpTalk("<t>Go now! I still have other important stuff to do!")
MisResultTalk("<t>You sure have a way with this.")

MisResultCondition(HasMission, 1061)
MisResultCondition(NoRecord,1061)
MisResultCondition(HasCredit,500 )
MisResultAction(DelRoleCredit, 500 )
MisResultAction(ClearMission, 1061)
MisResultAction(SetRecord,  1061 )
MisResultAction(GiveItem, 2945, 1, 4)
MisResultBagNeed(1)


-------------------------------------------------Level Hero----------Girl Xindi
DefineMission (5538, "Vampiric Aries Hero", 1062)

MisBeginTalk("<t>You need to be at least Lv 40. Don't tell me you cannot do it.")

MisBeginCondition(NoMission,1062)
MisBeginCondition(HasRecord,833)
MisBeginCondition(NoRecord,1062)
MisBeginAction(AddMission,1062)
MisCancelAction(ClearMission, 1062)

MisHelpTalk("<t>You better not return if you are lower than Lv 40.")
MisResultTalk("<t>You are so slow, I need to go out for a date soon.")

MisResultCondition(HasMission, 1062)
MisResultCondition(NoRecord,1062)
MisResultCondition(LvCheck, ">", 39 )
MisResultAction(ClearMission, 1062)
MisResultAction(SetRecord,  1062 )
MisResultAction(GiveItem, 2946, 1, 4)
MisResultBagNeed(1)


-------------------------------------------------Honorary Hero----------Girl Xindi	
DefineMission (5539, "Vampiric Aries Honorable Hero", 1063)

MisBeginTalk("<t>Honor is the combination of both courage and wisdom. I believe you will not let me down. Earn yourself 100 Honor points.")

MisBeginCondition(NoMission,1063)
MisBeginCondition(HasRecord,833)
MisBeginCondition(NoRecord,1063)
MisBeginAction(AddMission,1063)
MisCancelAction(ClearMission, 1063)

MisHelpTalk("<t>I have a love letter to read. Remember to come back with Honor.")
MisResultTalk("<t>How should I reward you How about a kiss?")

MisResultCondition(HasMission, 1063)
MisResultCondition(NoRecord,1063)
MisResultCondition(HasHonorPoint,100 )
MisResultAction(TakeHonorPoint, 100 )
MisResultAction(ClearMission, 1063)
MisResultAction(SetRecord,  1063 )
MisResultAction(GiveItem, 2947, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Gathering Ambassador----------Girl Xindi	
DefineMission (5540, "Vampiric Aries Gatherer Ambassador", 1064)

MisBeginTalk("<t>Did you know that my friend has a <yGatherer Emblem>? If you wish to get it, prepare for yourself for some test.")

MisBeginCondition(NoMission,1064)
MisBeginCondition(HasRecord,833)
MisBeginCondition(NoRecord,1064)
MisBeginAction(AddMission,1064)
MisBeginAction(AddTrigger, 10641, TE_GETITEM, 3116, 15 )---------------Elven Fruit
MisBeginAction(AddTrigger, 10642, TE_GETITEM, 1678, 15 )---------------Cashmere
MisBeginAction(AddTrigger, 10643, TE_GETITEM, 4809, 15 )---------------Pumpkin Head
MisBeginAction(AddTrigger, 10644, TE_GETITEM, 0855, 20 )---------------Fairy Coins
MisBeginAction(AddTrigger, 10645, TE_GETITEM, 4503, 1 )----------------Crown
MisBeginAction(AddTrigger, 10646, TE_GETITEM, 1848, 50 )---------------Bread
MisCancelAction(ClearMission, 1064)


MisNeed(MIS_NEED_ITEM, 3116, 15, 1, 15 )
MisNeed(MIS_NEED_ITEM, 1678, 15, 16, 15 )
MisNeed(MIS_NEED_ITEM, 4809, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 0855, 20, 46, 20 )
MisNeed(MIS_NEED_ITEM, 4503, 1, 66, 1 )
MisNeed(MIS_NEED_ITEM, 1848, 50, 67, 50 )

MisHelpTalk("<t>These items are not tough to find. Go now!")
MisResultTalk("<t>I suspect you could actually gather alien matter.")

MisResultCondition(HasMission, 1064)
MisResultCondition(NoRecord,1064)
MisResultCondition(HasItem, 3116, 15 )
MisResultCondition(HasItem, 1678, 15 )
MisResultCondition(HasItem, 4809, 15 )
MisResultCondition(HasItem, 0855, 20 )
MisResultCondition(HasItem, 4503, 1 )
MisResultCondition(HasItem, 1848, 50 )

MisResultAction(TakeItem, 3116, 15 )
MisResultAction(TakeItem, 1678, 15 )
MisResultAction(TakeItem, 4809, 15 )
MisResultAction(TakeItem, 0855, 20 )
MisResultAction(TakeItem, 4503, 1 )
MisResultAction(TakeItem, 1848, 50 )
MisResultAction(ClearMission, 1064)
MisResultAction(SetRecord,  1064 )
MisResultAction(GiveItem, 2948, 1, 4)
MisResultAction(GiveItem, 2990, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1064, 1, 15 )
RegCurTrigger( 10641 )

InitTrigger()
TriggerCondition( 1, IsItem, 1678)	
TriggerAction( 1, AddNextFlag, 1064, 16, 15 )
RegCurTrigger( 10642 )

InitTrigger()
TriggerCondition( 1, IsItem, 4809)	
TriggerAction( 1, AddNextFlag, 1064, 31, 15 )
RegCurTrigger( 10643 )

InitTrigger()
TriggerCondition( 1, IsItem, 0855)	
TriggerAction( 1, AddNextFlag, 1064, 46, 20 )
RegCurTrigger( 10644 )

InitTrigger()
TriggerCondition( 1, IsItem, 4503)	
TriggerAction( 1, AddNextFlag, 1064, 66, 1 )
RegCurTrigger( 10645 )

InitTrigger()
TriggerCondition( 1, IsItem, 1848)	
TriggerAction( 1, AddNextFlag, 1064, 67, 50 )
RegCurTrigger( 10646 )

----------------------------------------------------------Sleeping Aries----------Girl Xindi
DefineMission( 5541, "Vampiric Aries C Sleeping Aries", 1065 )
MisBeginTalk("<t>There is a bad guy in shiny Aries armor nearby. Please help the villagers of <pCupid Isle> to get rid of him.")
			
MisBeginCondition(NoMission, 1065)
MisBeginCondition(HasRecord,836)
MisBeginCondition(NoRecord,1065)
MisBeginAction(AddMission,1065)
MisBeginAction(AddTrigger, 10651, TE_KILL, 1009, 1)---Aries Guardian

MisCancelAction(ClearMission, 1065)

MisNeed(MIS_NEED_DESP,"Kill the <rAries Guardian> on <pCupid Isle> at <j(2566, 2454)>!")
MisNeed(MIS_NEED_KILL, 1009,1, 10, 1)


MisResultTalk("<t>Saving a damsel in distress does not happen daily. Moreover, he seldom appears.")
MisHelpTalk("<t>Are you blessed by the gods? You seem invincible.")
MisResultCondition(HasMission,  1065)
MisResultCondition(HasFlag, 1065, 10)
MisResultCondition(NoRecord , 1065)
MisResultAction(GiveItem, 2950, 1, 4 )
MisResultAction(ClearMission,  1065)
MisResultAction(SetRecord,  1065 )
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 1009)	
TriggerAction( 1, AddNextFlag, 1065, 10, 1 )
RegCurTrigger( 10651 )
-------------------------------------------------Brawl Hero----------Girl Xindi
DefineMission (5542, "Vampiric Aries Battle Hero", 1066)

MisBeginTalk("<t>Challenging Hero needs to be an expert in Chaos combat. 20 <yChaos points> should not deter you.")

MisBeginCondition(NoMission,1066)
MisBeginCondition(HasRecord,834)
MisBeginCondition(NoRecord,1066)
MisBeginAction(AddMission,1066)
MisCancelAction(ClearMission, 1066)

MisHelpTalk("<t>It's only 20 <yChaos points>! Work harder!")
MisResultTalk("<t>I knew that 20 <yChaos points> should not pose any difficulty for you.")

MisResultCondition(HasMission, 1066)
MisResultCondition(NoRecord,1066)
MisResultCondition(HasFightingPoint,20 )
MisResultAction(TakeFightingPoint, 20 )
MisResultAction(ClearMission, 1066)
MisResultAction(SetRecord,  1066 )
MisResultAction(GiveItem, 2944, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Hero of Fame----------Girl Xindi
DefineMission (5543, "Vampiric Aries Renown Hero", 1067)

MisBeginTalk("<t>How can you do a challenge when you have less than 1,000 reputation points? Try getting a disciple to increase your reputation points.")

MisBeginCondition(NoMission,1067)
MisBeginCondition(HasRecord,834)
MisBeginCondition(NoRecord,1067)
MisBeginAction(AddMission,1067)
MisCancelAction(ClearMission, 1067)

MisHelpTalk("<t>Go now! I still have other important stuff to do!")
MisResultTalk("<t>You sure have a way with this.")

MisResultCondition(HasMission, 1067)
MisResultCondition(NoRecord,1067)
MisResultCondition(HasCredit,1000 )
MisResultAction(DelRoleCredit, 1000 )
MisResultAction(ClearMission, 1067)
MisResultAction(SetRecord,  1067 )
MisResultAction(GiveItem, 2945, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Level Hero----------Girl Xindi
DefineMission (5544, "Vampiric Aries Hero", 1068)

MisBeginTalk("<t>You need to be at least Lv 45. Don't tell me you cannot do it.")

MisBeginCondition(NoMission,1068)
MisBeginCondition(HasRecord,834)
MisBeginCondition(NoRecord,1068)
MisBeginAction(AddMission,1068)
MisCancelAction(ClearMission, 1068)

MisHelpTalk("<t>Do not return if you are lower than Lv 45.")
MisResultTalk("<t>You are so slow, I need to go out for a date soon.")

MisResultCondition(HasMission, 1068)
MisResultCondition(NoRecord,1068)
MisResultCondition(LvCheck, ">", 44 )
MisResultAction(ClearMission, 1068)
MisResultAction(SetRecord,  1068 )
MisResultAction(GiveItem, 2946, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Honorary Hero----------Girl Xindi	
DefineMission (5545, "Vampiric Aries Honorable Hero", 1069)

MisBeginTalk("<t> Honor is the combination of both courage and wisdom. I believe you will not let me down. Earn yourself 200 Honor points ")

MisBeginCondition(NoMission,1069)
MisBeginCondition(HasRecord,834)
MisBeginCondition(NoRecord,1069)
MisBeginAction(AddMission,1069)
MisCancelAction(ClearMission, 1069)

MisHelpTalk("<t>I have a love letter to read. Remember to come back with Honor.")
MisResultTalk("<t>How should I reward youHow about a kiss?")

MisResultCondition(HasMission, 1069)
MisResultCondition(NoRecord,1069)
MisResultCondition(HasHonorPoint,200 )
MisResultAction(TakeHonorPoint, 200 )
MisResultAction(ClearMission, 1069)
MisResultAction(SetRecord,  1069 )
MisResultAction(GiveItem, 2947, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Gathering Ambassador----------Girl Xindi	
DefineMission (5546, "Vampiric Aries Gatherer Ambassador", 1070)

MisBeginTalk("<t>Do you know that my friend has a Gatherer Emblem? If you wish to get it, prepare for yourself for some test.")

MisBeginCondition(NoMission,1070)
MisBeginCondition(HasRecord,834)
MisBeginCondition(NoRecord,1070)
MisBeginAction(AddMission,1070)
MisBeginAction(AddTrigger, 10701, TE_GETITEM, 3116, 15 )---------------Elven Fruit
MisBeginAction(AddTrigger, 10702, TE_GETITEM, 1678, 15 )---------------Cashmere
MisBeginAction(AddTrigger, 10703, TE_GETITEM, 4809, 15 )---------------Pumpkin Head
MisBeginAction(AddTrigger, 10704, TE_GETITEM, 0855, 20 )---------------Fairy Coins
MisBeginAction(AddTrigger, 10705, TE_GETITEM, 4503, 1 )----------------Crown
MisBeginAction(AddTrigger, 10706, TE_GETITEM, 1848, 40 )---------------Bread
MisBeginAction(AddTrigger, 10707, TE_GETITEM, 2673, 10 )---------------Mirage Generator Lv1
MisCancelAction(ClearMission, 1070)


MisNeed(MIS_NEED_ITEM, 3116, 15, 1, 15 )
MisNeed(MIS_NEED_ITEM, 1678, 15, 16, 15 )
MisNeed(MIS_NEED_ITEM, 4809, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 0855, 20, 46, 20 )
MisNeed(MIS_NEED_ITEM, 4503, 1, 66, 1 )
MisNeed(MIS_NEED_ITEM, 1848, 40, 67, 40 )
MisNeed(MIS_NEED_ITEM, 2673, 10, 107, 10 )

MisHelpTalk("<t>These items are not tough to find. Go now!")
MisResultTalk("<t>I suspect you could actually gather alien matter.")

MisResultCondition(HasMission, 1070)
MisResultCondition(NoRecord,1070)
MisResultCondition(HasItem, 3116, 15 )
MisResultCondition(HasItem, 1678, 15 )
MisResultCondition(HasItem, 4809, 15 )
MisResultCondition(HasItem, 0855, 20 )
MisResultCondition(HasItem, 4503, 1 )
MisResultCondition(HasItem, 1848, 40 )
MisResultCondition(HasItem, 2673, 10 )

MisResultAction(TakeItem, 3116, 15 )
MisResultAction(TakeItem, 1678, 15 )
MisResultAction(TakeItem, 4809, 15 )
MisResultAction(TakeItem, 0855, 20 )
MisResultAction(TakeItem, 4503, 1 )
MisResultAction(TakeItem, 1848, 40 )
MisResultAction(TakeItem, 2673, 10 )
MisResultAction(ClearMission, 1070)
MisResultAction(SetRecord,  1070 )
MisResultAction(GiveItem, 2948, 1, 4)
MisResultAction(GiveItem, 2990, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1070, 1, 15 )
RegCurTrigger( 10701 )

InitTrigger()
TriggerCondition( 1, IsItem, 1678)	
TriggerAction( 1, AddNextFlag, 1070, 16, 15 )
RegCurTrigger( 10702 )

InitTrigger()
TriggerCondition( 1, IsItem, 4809)	
TriggerAction( 1, AddNextFlag, 1070, 31, 15 )
RegCurTrigger( 10703 )

InitTrigger()
TriggerCondition( 1, IsItem, 0855)	
TriggerAction( 1, AddNextFlag, 1070, 46, 20 )
RegCurTrigger( 10704 )

InitTrigger()
TriggerCondition( 1, IsItem, 4503)	
TriggerAction( 1, AddNextFlag, 1070, 66, 1 )
RegCurTrigger( 10705 )

InitTrigger()
TriggerCondition( 1, IsItem, 1848)	
TriggerAction( 1, AddNextFlag, 1070, 67, 40 )
RegCurTrigger( 10706 )

InitTrigger()
TriggerCondition( 1, IsItem, 2673)	
TriggerAction( 1, AddNextFlag, 1070, 107, 10 )
RegCurTrigger( 10707 )

-------------------------------------------------Brawl Hero----------Girl Xindi
DefineMission (5547, "Vampiric Aries Battle Hero", 1071)

MisBeginTalk("<t>Challenging Hero must be a Chaos expert. 50 Chaos points should not be an issue.")

MisBeginCondition(NoMission,1071)
MisBeginCondition(HasRecord,835)
MisBeginCondition(NoRecord,1071)
MisBeginAction(AddMission,1071)
MisCancelAction(ClearMission, 1071)

MisHelpTalk("<t>Its only 50 Chaos points. Work harder!")
MisResultTalk("<t>I knew that 50 Chaos points shouldn't be a problem for you.")

MisResultCondition(HasMission, 1071)
MisResultCondition(NoRecord,1071)
MisResultCondition(HasFightingPoint,50 )
MisResultAction(TakeFightingPoint, 50 )
MisResultAction(ClearMission, 1071)
MisResultAction(SetRecord,  1071 )
MisResultAction(GiveItem, 2944, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Hero of Fame----------Girl Xindi
DefineMission (5548, "Vampiric Aries Renown Hero", 1072)

MisBeginTalk("<t>How can a hero challenge with less than 3,000 reputation points? I believe that you will have a way to gain those reputation, such as getting a disciple")

MisBeginCondition(NoMission,1072)
MisBeginCondition(HasRecord,835)
MisBeginCondition(NoRecord,1072)
MisBeginAction(AddMission,1072)
MisCancelAction(ClearMission, 1072)

MisHelpTalk("<t>Go now! I still have other important stuff to do!")
MisResultTalk("<t>You sure have a way with this.")

MisResultCondition(HasMission, 1072)
MisResultCondition(NoRecord,1072)
MisResultCondition(HasCredit,3000 )
MisResultAction(DelRoleCredit, 3000 )
MisResultAction(ClearMission, 1072)
MisResultAction(SetRecord,  1072 )
MisResultAction(GiveItem, 2945, 1, 4)
MisResultBagNeed(1)


-------------------------------------------------Level Hero----------Girl Xindi
DefineMission (5549, "Vampiric Aries Hero", 1073)

MisBeginTalk("<t>The minimum is Lv 50. Do not tell me you cannot do it.")

MisBeginCondition(NoMission,1073)
MisBeginCondition(HasRecord,835)
MisBeginCondition(NoRecord,1073)
MisBeginAction(AddMission,1073)
MisCancelAction(ClearMission, 1073)

MisHelpTalk("<t>You dare to return without reaching Lv 50?")
MisResultTalk("<t>You are so slow, I need to go out for a date soon.")

MisResultCondition(HasMission, 1073)
MisResultCondition(NoRecord,1073)
MisResultCondition(LvCheck, ">", 49 )
MisResultAction(ClearMission, 1073)
MisResultAction(SetRecord,  1073 )
MisResultAction(GiveItem, 2946, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Honorary Hero----------Girl Xindi	
DefineMission (5550, "Vampiric Aries Honorable Hero", 1074)

MisBeginTalk("<t>Honor is the combination of both courage and wisdom. I believe you will not let me down. Earn yourself 500 Honor points.")

MisBeginCondition(NoMission,1074)
MisBeginCondition(HasRecord,835)
MisBeginCondition(NoRecord,1074)
MisBeginAction(AddMission,1074)
MisCancelAction(ClearMission, 1074)

MisHelpTalk("<t>I have a love letter to read. Remember to come back with Honor.")
MisResultTalk("<t>How should I reward youHow about a kiss?")

MisResultCondition(HasMission, 1074)
MisResultCondition(NoRecord,1074)
MisResultCondition(HasHonorPoint,500 )
MisResultAction(TakeHonorPoint, 500 )
MisResultAction(ClearMission, 1074)
MisResultAction(SetRecord,  1074 )
MisResultAction(GiveItem, 2947, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Gathering Ambassador----------Girl Xindi	
DefineMission (5551, "Vampiric Aries Gatherer Ambassador", 1075)

MisBeginTalk("<t>Do you know that my friend has a Gatherer Emblem? If you wish to get it, prepare for yourself for some test.")

MisBeginCondition(NoMission,1075)
MisBeginCondition(HasRecord,835)
MisBeginCondition(NoRecord,1075)
MisBeginAction(AddMission,1075)
MisBeginAction(AddTrigger, 10751, TE_GETITEM, 3116, 15 )---------------Elven Fruit
MisBeginAction(AddTrigger, 10752, TE_GETITEM, 1678, 15 )---------------Cashmere
MisBeginAction(AddTrigger, 10753, TE_GETITEM, 4809, 15 )---------------Pumpkin Head
MisBeginAction(AddTrigger, 10754, TE_GETITEM, 0855, 20 )---------------Fairy Coin
MisBeginAction(AddTrigger, 10755, TE_GETITEM, 4503, 1 )----------------Crown
MisBeginAction(AddTrigger, 10756, TE_GETITEM, 1848, 40 )---------------Bread
MisBeginAction(AddTrigger, 10757, TE_GETITEM, 2673, 10 )---------------Mirage Generator Lv1
MisBeginAction(AddTrigger, 10758, TE_GETITEM, 0227, 4 )----------------Fairy Ration
MisCancelAction(ClearMission, 1075)


MisNeed(MIS_NEED_ITEM, 3116, 15, 1, 15 )
MisNeed(MIS_NEED_ITEM, 1678, 15, 16, 15 )
MisNeed(MIS_NEED_ITEM, 4809, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 0855, 20, 46, 20 )
MisNeed(MIS_NEED_ITEM, 4503, 1, 66, 1 )
MisNeed(MIS_NEED_ITEM, 1848, 40, 67, 40 )
MisNeed(MIS_NEED_ITEM, 2673, 10, 107, 10 )
MisNeed(MIS_NEED_ITEM, 0227, 4, 117, 4 )

MisHelpTalk("<t>These items are not tough to find. Go now!")
MisResultTalk("<t>I suspect you could actually gather alien matter.")

MisResultCondition(HasMission, 1075)
MisResultCondition(NoRecord,1075)
MisResultCondition(HasItem, 3116, 15 )
MisResultCondition(HasItem, 1678, 15 )
MisResultCondition(HasItem, 4809, 15 )
MisResultCondition(HasItem, 0855, 20 )
MisResultCondition(HasItem, 4503, 1 )
MisResultCondition(HasItem, 1848, 40 )
MisResultCondition(HasItem, 2673, 10 )
MisResultCondition(HasItem, 0227, 4 )

MisResultAction(TakeItem, 3116, 15 )
MisResultAction(TakeItem, 1678, 15 )
MisResultAction(TakeItem, 4809, 15 )
MisResultAction(TakeItem, 0855, 20 )
MisResultAction(TakeItem, 4503, 1 )
MisResultAction(TakeItem, 1848, 40 )
MisResultAction(TakeItem, 2673, 10 )
MisResultAction(TakeItem, 0227, 4 )
MisResultAction(ClearMission, 1075)
MisResultAction(SetRecord,  1075 )
MisResultAction(GiveItem, 2948, 1, 4)
MisResultAction(GiveItem, 2990, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1075, 1, 15 )
RegCurTrigger( 10751 )

InitTrigger()
TriggerCondition( 1, IsItem, 1678)	
TriggerAction( 1, AddNextFlag, 1075, 16, 15 )
RegCurTrigger( 10752 )

InitTrigger()
TriggerCondition( 1, IsItem, 4809)	
TriggerAction( 1, AddNextFlag, 1075, 31, 15 )
RegCurTrigger( 10753 )

InitTrigger()
TriggerCondition( 1, IsItem, 0855)	
TriggerAction( 1, AddNextFlag, 1075, 46, 20 )
RegCurTrigger( 10754 )

InitTrigger()
TriggerCondition( 1, IsItem, 4503)	
TriggerAction( 1, AddNextFlag, 1075, 66, 1 )
RegCurTrigger( 10755 )

InitTrigger()
TriggerCondition( 1, IsItem, 1848)	
TriggerAction( 1, AddNextFlag, 1075, 67, 40 )
RegCurTrigger( 10756 )

InitTrigger()
TriggerCondition( 1, IsItem, 2673)	
TriggerAction( 1, AddNextFlag, 1075, 107, 10 )
RegCurTrigger( 10757 )

InitTrigger()
TriggerCondition( 1, IsItem, 0227)	
TriggerAction( 1, AddNextFlag, 1075, 117, 4 )
RegCurTrigger( 10758 )
----------------------------------------------------------Social Ambassador----------Girl Xindi
DefineMission( 5552, "Vampiric Aries TOP Ambassador", 1076 )
MisBeginTalk("<t>It takes intellect to become a pirate, to go around eating for free and having fun along the way, so you have to differentiate between important person and those who are not.")
			
MisBeginCondition(NoMission, 1076)
MisBeginCondition(NoRecord,1076)
MisBeginCondition(HasRecord, 836)
MisBeginAction(AddMission,1076)
MisCancelAction(ClearMission, 1076)

MisNeed(MIS_NEED_DESP,"Look for <bMerman Prince Hassan> in Shaitan at <j(1254, 3491)>.")

MisHelpTalk("<t>You have to go even if you are familiar with that place. This is a quest.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador-----Prince Mermaid Hasshat
DefineMission(5553, "TOP Ambassador", 1076, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Do you know that I am the most handsome fellow here, and also the fastest swimmer!")
MisResultCondition(NoRecord, 1076)
MisResultCondition(HasMission,1076)
MisResultAction(ClearMission,1076)
MisResultAction(SetRecord, 1076)

----------------------------------------------------------Social Ambassador 1----------The Mermaid Prince Hashat
DefineMission( 5554, "TOP Ambassador 2", 1077 )
MisBeginTalk("<t>I prayed to the gods to turn me into a tree that she will pass by and rest upon daily. Indeed she do pass by that tree daily, but I have been turned into a fish")
			
MisBeginCondition(NoMission, 1077)
MisBeginCondition(NoRecord,1077)
MisBeginCondition(HasRecord, 1076)
MisBeginAction(AddMission,1077)
MisCancelAction(ClearMission, 1077)

MisNeed(MIS_NEED_DESP,"Look for <bStrawberry> in Icicle at <j(1010, 350)>.")

MisHelpTalk("<t>I am only a little fish Air in the water")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 1------------- Strawberry Youyou
DefineMission(5555, "TOP Ambassador 2", 1077, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Hi! My ambassador!")
MisResultCondition(NoRecord, 1077)
MisResultCondition(HasMission,1077)
MisResultAction(ClearMission,1077)
MisResultAction(SetRecord, 1077)

----------------------------------------------------------Social Ambassador 2---------- Strawberry Youyou
DefineMission( 5556, "TOP Ambassador 3", 1078 )
MisBeginTalk("<t>I look like a normal being on the outside. However, if you believe it to be so, may the lord bless you.")
			
MisBeginCondition(NoMission, 1078)
MisBeginCondition(NoRecord,1078)
MisBeginCondition(HasRecord, 1077)
MisBeginAction(AddMission,1078)
MisCancelAction(ClearMission, 1078)

MisNeed(MIS_NEED_DESP,"Look for <bOldman Blurry> in Ascaron at <j(2272, 2700)>.")

MisHelpTalk("<t>That adorable old man has an amazing talent.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 2-------Old Man, Confused Mountain Man
DefineMission(5557, "TOP Ambassador 3", 1078, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>A simple person can always find happiness")
MisResultCondition(NoRecord, 1078)
MisResultCondition(HasMission,1078)
MisResultAction(ClearMission,1078)
MisResultAction(SetRecord, 1078)

----------------------------------------------------------Social Ambassador 3----------Old Man  Confused Mountain Man
DefineMission( 5558, "TOP Ambassador 4", 1079 )
MisBeginTalk("<t>Jealous of me? The wise are often inspired by music. Visit <bMusician - Shamel>. He is wiser than me.")
			
MisBeginCondition(NoMission, 1079)
MisBeginCondition(NoRecord,1079)
MisBeginCondition(HasRecord, 1078)
MisBeginAction(AddMission,1079)
MisCancelAction(ClearMission, 1079)

MisNeed(MIS_NEED_DESP,"Look for <bMusician Shamel> in Shaitan at <j(664, 3093)>.")

MisHelpTalk("<t>Music can create miracle, remember this!")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 3-------Musical Instrument Expert Shamei'er
DefineMission(5559, "TOP Ambassador 4", 1079, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Blurry is the Enlightened One!")
MisResultCondition(NoRecord, 1079)
MisResultCondition(HasMission,1079)
MisResultAction(ClearMission,1079)
MisResultAction(SetRecord, 1079)


----------------------------------------------------------Social Ambassador 4----------Musical Instrument ExpertSha Mei'er
DefineMission( 5560, "TOP Ambassador 5", 1080 )
MisBeginTalk("<t>My dream is to create a paradise on earth with music. Do you have a dream as well? Is it the same as <bLulu's>?")
			
MisBeginCondition(NoMission, 1080)
MisBeginCondition(NoRecord,1080)
MisBeginCondition(HasRecord, 1079)
MisBeginAction(AddMission,1080)
MisCancelAction(ClearMission, 1080)

MisNeed(MIS_NEED_DESP,"Look for <bLulu> in Icicle at <j(2668, 634)>.")

MisHelpTalk("<t>Dream is a shadow of reality")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 4-------Lulu
DefineMission(5561, "TOP Ambassador 5", 1080, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I wish to have lollipops daily")
MisResultCondition(NoRecord, 1080)
MisResultCondition(HasMission,1080)
MisResultAction(ClearMission,1080)
MisResultAction(SetRecord, 1080)
----------------------------------------------------------Social Ambassador 5------------Lulu
DefineMission( 5562, "TOP Ambassador 6", 1081 )
MisBeginTalk("<t>If only I have lollipops to eat daily and the beautiful sister next door to accompany me to Ascaron to visit a friend whom I have never meet")
			
MisBeginCondition(NoMission, 1081)
MisBeginCondition(NoRecord,1081)
MisBeginCondition(HasRecord, 1080)
MisBeginAction(AddMission,1081)
MisCancelAction(ClearMission, 1081)

MisNeed(MIS_NEED_DESP,"Look for <bGregg> in Ascaron at <j(526, 2432)>.")

MisHelpTalk("<t>Please send my blessing for <bGregg>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 5------- Cougar
DefineMission(5563, "TOP Ambassador 6", 1081, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You are a friend of <bLulu>? Is <bLulu? well?")
MisResultCondition(NoRecord, 1081)
MisResultCondition(HasMission,1081)
MisResultAction(ClearMission,1081)
MisResultAction(SetRecord, 1081)

----------------------------------------------------------Social Ambassador 6------------ Courage
DefineMission( 5564, "TOP Ambassador 7", 1082 )
MisBeginTalk("<t>I love to get to know strangers from afar. But there is this guy <bWelly> who keep telling me that he is a lamb. Could you check it out for me?")
			
MisBeginCondition(NoMission, 1082)
MisBeginCondition(NoRecord,1082)
MisBeginCondition(HasRecord, 1081)
MisBeginAction(AddMission,1082)
MisCancelAction(ClearMission, 1082)

MisNeed(MIS_NEED_DESP,"Look for <bLamb Welly> in Shaitan at <j(898, 3683)>.")

MisHelpTalk("<t>How I wish Welly is a great knight")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 6-------Lamb Welly
DefineMission(5565, "TOP Ambassador 7", 1082, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You never tell her? I used to be a normal lamb. It is only recently that I learnt how to speak human.")
MisResultCondition(NoRecord, 1082)
MisResultCondition(HasMission,1082)
MisResultAction(ClearMission,1082)
MisResultAction(SetRecord, 1082)
----------------------------------------------------------Social Ambassador 7------------Lamb Welly
DefineMission( 5566, "TOP Ambassador 8", 1083 )
MisBeginTalk("<t>Do you want to be a TOP Hero? Let me introduce you to somebody. He is a challenging person!")
			
MisBeginCondition(NoMission, 1083)
MisBeginCondition(NoRecord,1083)
MisBeginCondition(HasRecord, 1082)
MisBeginAction(AddMission,1083)
MisCancelAction(ClearMission, 1083)

MisNeed(MIS_NEED_DESP,"Look for <bDoctor Minoseva> in Ascaron at <j(1004, 2973)>.")

MisHelpTalk("<t>Good luck, human!")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 8-------Travel Doctor Menethil
DefineMission(5567, "TOP Ambassador 8", 1083, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>It is ok that human misunderstood me, as they always quick to judge by the appearances. However, even sheeps are the same now. What is happening in the world now?")
MisResultCondition(NoRecord, 1083)
MisResultCondition(HasMission,1083)
MisResultAction(ClearMission,1083)
MisResultAction(SetRecord, 1083)

----------------------------------------------------------Social Ambassador 8----------Travel Doctor Menethil
DefineMission( 5568, "TOP Ambassador 9", 1084 )
MisBeginTalk("<t>My kindness often goes unnoticed. I do not have a pair of charming eyes like <bFey Fey>. Please send my regards to her.")
			
MisBeginCondition(NoMission, 1084)
MisBeginCondition(NoRecord,1084)
MisBeginCondition(HasRecord, 1083)
MisBeginAction(AddMission,1084)
MisCancelAction(ClearMission, 1084)

MisNeed(MIS_NEED_DESP,"Look for <bFey Fey> in Shaitan at <j(797, 3129)>.")

MisHelpTalk("<t>I want you to change your opinion of me.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 8-------Fifi
DefineMission(5569, "TOP Ambassador 9", 1084, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t><bDoctor C Minoseva> is a quiet but nice person.")
MisResultCondition(NoRecord, 1084)
MisResultCondition(HasMission,1084)
MisResultAction(ClearMission,1084)
MisResultAction(SetRecord, 1084)

----------------------------------------------------------Social Ambassador 10----------Feifei
DefineMission( 5572, "TOP Ambassador 10", 1086 )
MisBeginTalk("<t>Don't talk about her anymore. I wish to meet this person named <bElizabeth>. I have picked up a floating bottle a couple of days ago and in it contains an invitation to a dance.")
			
MisBeginCondition(NoMission, 1086)
MisBeginCondition(NoRecord,1086)
MisBeginCondition(HasRecord, 1084)
MisBeginAction(AddMission,1086)
MisCancelAction(ClearMission, 1086)

MisNeed(MIS_NEED_DESP,"Look for <bElizabeth> in the <pTreasure Gulf> at <j(616, 965)>.")

MisHelpTalk("<t>Romantic dance")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 10-------Elizabeth
DefineMission(5573, "TOP Ambassador 10", 1086, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Somebody got my invitation? This is great! Thank you for bring me this good news.")
MisResultCondition(NoRecord, 1086)
MisResultCondition(HasMission,1086)
MisResultAction(ClearMission,1086)
MisResultAction(SetRecord, 1086)

----------------------------------------------------------Social Ambassador 11----------Elizabeth
DefineMission( 5574, "TOP Ambassador 11", 1087 )
MisBeginTalk("<t>My dance is going to start soon! Yet I have to fetch <bWang Rong>. Can you help me invite <bNavy HQ - General Ken>?")
			
MisBeginCondition(NoMission, 1087)
MisBeginCondition(NoRecord,1087)
MisBeginCondition(HasRecord, 1086)
MisBeginAction(AddMission,1087)
MisCancelAction(ClearMission, 1087)

MisNeed(MIS_NEED_DESP,"Look for Thundoria Castle's <bNavy HQ - General Ken> at <j(808, 1521)>.")

MisHelpTalk("<t>I want to get to know those girls")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 11------- Admiral Key Essie, Commander of the Admiralty
DefineMission(5575, "TOP Ambassador 11", 1087, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I never reject invitation from a beautiful lady, hehe")
MisResultCondition(NoRecord, 1087)
MisResultCondition(HasMission,1087)
MisResultAction(ClearMission,1087)
MisResultAction(SetRecord, 1087)

----------------------------------------------------------Social Ambassador 12------- Admiral Key Essey, Commander of the Admiralty
DefineMission( 5576, "TOP Ambassador 12", 1088 )
MisBeginTalk("<t>I promised to write a poem for <bMomo> in Shaitan. However, I am sick at the moment so could you tell her that I will send her a draft after I am well enough to start composing.")
			
MisBeginCondition(NoMission, 1088)
MisBeginCondition(NoRecord,1088)
MisBeginCondition(HasRecord, 1087)
MisBeginAction(AddMission,1088)
MisCancelAction(ClearMission, 1088)

MisNeed(MIS_NEED_DESP,"Look for <bMomo> in Shaitan City at <j(1209, 3196)>.")

MisHelpTalk("<t>A romantic dance will give me a perfect chance to woo her. Ohmy lovely <bElizabeth>")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 12---------Mushroom
DefineMission(5577, "TOP Ambassador 12", 1088, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>He is so tough yet he has fallen sick. Poor general.")
MisResultCondition(NoRecord, 1088)
MisResultCondition(HasMission,1088)
MisResultAction(ClearMission,1088)
MisResultAction(SetRecord, 1088)
----------------------------------------------------------Social Ambassador 13------------Mushroom
DefineMission( 5578, "TOP Ambassador 13", 1089 )
MisBeginTalk("<t>Actually my friend idolize the general and wishes to get a copy of his manuscript. If you are free could you inform him for me? Thank you.")
			
MisBeginCondition(NoMission, 1089)
MisBeginCondition(NoRecord,1089)
MisBeginCondition(HasRecord, 1088)
MisBeginAction(AddMission,1089)
MisCancelAction(ClearMission, 1089)

MisNeed(MIS_NEED_DESP,"Look for <bBerry> in Ascaron at <j(1893, 2812)>.")

MisHelpTalk("<t>Berry will become a great poet one day.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 13-------TaoTao
DefineMission(5579, "TOP Ambassador 13", 1089, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>So the General also do falls ill?")
MisResultCondition(NoRecord, 1089)
MisResultCondition(HasMission,1089)
MisResultAction(ClearMission,1089)
MisResultAction(SetRecord, 1089)

----------------------------------------------------------Social Ambassador 14----------TaoTao
DefineMission( 5580, "TOP Ambassador 14", 1090 )
MisBeginTalk("<t>How I wish that the general will recover faster. Thank you. Let me introduce you to <bWynne>.")
			
MisBeginCondition(NoMission, 1090)
MisBeginCondition(NoRecord,1090)
MisBeginCondition(HasRecord, 1089)
MisBeginAction(AddMission,1090)
MisCancelAction(ClearMission, 1090)

MisNeed(MIS_NEED_DESP,"Talk to <bWynne> of Icicle City at <j(1380, 523)>.")

MisHelpTalk("<t><bWynne> is in Icicle City at <j(1380, 523)>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 14-------Wen Li Bingquan       
DefineMission(5581, "TOP Ambassador 14", 1090, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I used to be beautiful before I fallen out of love.")
MisResultCondition(NoRecord, 1090)
MisResultCondition(HasMission,1090)
MisResultAction(ClearMission,1090)
MisResultAction(SetRecord, 1090)

----------------------------------------------------------Social Ambassador 15----------Wen Li Ice Spring
DefineMission( 5582, "TOP Ambassador 15", 1091 )
MisBeginTalk("<t>I am very  weak now. Please help me deliever a message to <bDon Pitt>. I will put in a good word for you at <bCindy> if you will do me the favor.")
			
MisBeginCondition(NoMission, 1091)
MisBeginCondition(NoRecord,1091)
MisBeginCondition(HasRecord, 1090)
MisBeginAction(AddMission,1091)
MisCancelAction(ClearMission, 1091)

MisNeed(MIS_NEED_DESP,"Look for <bDon Pitt> in Ascaron at <j(1113, 2779)>.")

MisHelpTalk("<t>I beg of you, I love him dearly.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 15------- Red Pig
DefineMission(5583, "TOP Ambassador 15", 1091, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>My dear <bWynne> is looking pale! What should I do?")
MisResultCondition(NoRecord, 1091)
MisResultCondition(HasMission,1091)
MisResultAction(ClearMission,1091)
MisResultAction(SetRecord, 1091)

----------------------------------------------------------Social Ambassador 16----------Red Pig
DefineMission( 5584, "TOP Ambassador 16", 1092 )
MisBeginTalk("<t>Actually I haven already forgiven her. However, I have been so busy that I forgot to write to her. Please help me inform <bOracle C Moonlight> that I will look for her soon.")
			
MisBeginCondition(NoMission, 1092)
MisBeginCondition(NoRecord,1092)
MisBeginCondition(HasRecord, 1091)
MisBeginAction(AddMission,1092)
MisCancelAction(ClearMission, 1092)

MisNeed(MIS_NEED_DESP,"Look for <bOracle Moonlight> in Icicle at <j(2134, 555)>.")

MisHelpTalk("<t>I believe <bOracle Moonlight> will understand.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 16------- Psychic Moonlight Eye
DefineMission(5585, "TOP Ambassador 16", 1092, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>He has done right. If I could give in in the past, I would not have lost my loved one.")
MisResultCondition(NoRecord, 1092)
MisResultCondition(HasMission,1092)
MisResultAction(ClearMission,1092)
MisResultAction(SetRecord, 1092)


----------------------------------------------------------Social Ambassador 17------------ Psychic Moonlight Eyes
DefineMission( 5586, "TOP Ambassador 17", 1093 )
MisBeginTalk("<t>All is in the past. What worries me most is <bDurian's> greedy appetite.")
			
MisBeginCondition(NoMission, 1093)
MisBeginCondition(NoRecord,1093)
MisBeginCondition(HasRecord, 1092)
MisBeginAction(AddMission,1093)
MisCancelAction(ClearMission, 1093)

MisNeed(MIS_NEED_DESP,"Look for <bDurian> in Ascaron at <j(1535, 3071)>.")

MisHelpTalk("<t><bDurian> seems like a kid. He is always daydreaming.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 17------- Durian
DefineMission(5587, "TOP Ambassador 17", 1093, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I am satisfied with my illness. It is keeping me slim.")
MisResultCondition(NoRecord, 1093)
MisResultCondition(HasMission,1093)
MisResultAction(ClearMission,1093)
MisResultAction(SetRecord, 1093)

----------------------------------------------------------Social Ambassador 18------------Durian
DefineMission( 5588, "TOP Ambassador 18", 1094 )
MisBeginTalk("<t>Having some many to be concern over my illness can be irritating at times. Please help me send a message to <bNana> in <pIcicle Haven> and tell her that I am well.")
			
MisBeginCondition(NoMission, 1094)
MisBeginCondition(NoRecord,1094)
MisBeginCondition(HasRecord, 1093)
MisBeginAction(AddMission,1094)
MisCancelAction(ClearMission, 1094)

MisNeed(MIS_NEED_DESP,"Look for <bNana> in Icicle at <j(798, 369)>.")

MisHelpTalk("<t>How I wish to be full.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 18------- Nana
DefineMission(5589, "TOP Ambassador 18", 1094, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Actually I am quite jealous of her.")
MisResultCondition(NoRecord, 1094)
MisResultCondition(HasMission,1094)
MisResultAction(ClearMission,1094)
MisResultAction(SetRecord, 1094)

----------------------------------------------------------Social Ambassador 19----------Nana
DefineMission( 5590, "TOP Ambassador 19", 1095 )
MisBeginTalk("<t>I have been busy composing poems nowadays. Please help me send a message to <bAzur Breeze >.")
			
MisBeginCondition(NoMission, 1095)
MisBeginCondition(NoRecord,1095)
MisBeginCondition(HasRecord, 1094)
MisBeginAction(AddMission,1095)
MisCancelAction(ClearMission, 1095)

MisNeed(MIS_NEED_DESP,"Look for <bAzur Breeze> in Ascaron at <j(624, 2105)>.")

MisHelpTalk("<t>Homeric is the greatest poet of the world.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 19------- Ocean Blue Breeze
DefineMission(5591, "TOP Ambassador 19", 1095, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>AhWrinkles! Ah")
MisResultCondition(NoRecord, 1095)
MisResultCondition(HasMission,1095)
MisResultAction(ClearMission,1095)
MisResultAction(SetRecord, 1095)

	----------------------------------------------------------Social Ambassador 20----------Sea Blue Breeze
DefineMission( 5592, "TOP Ambassador 20", 1096 )
MisBeginTalk("<t>If I am so wise, I would have knew how did that sea monster find our ship during that time. And I would not have request the help of that personPlease help me tell him that I have not forgotten about him.")
			
MisBeginCondition(NoMission, 1096)
MisBeginCondition(NoRecord,1096)
MisBeginCondition(HasRecord, 1095)
MisBeginAction(AddMission,1096)
MisCancelAction(ClearMission, 1096)

MisNeed(MIS_NEED_DESP,"Look for <bAbaddon Teleporter> at <j(690, 1043)>.")

MisHelpTalk("<t>Thank you, go now.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 20------- Hell Teleporter
DefineMission(5593, "TOP Ambassador 20", 1096, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>That happens many years ago, I am surprised that he still remembers.")
MisResultCondition(NoRecord, 1096)
MisResultCondition(HasMission,1096)
MisResultAction(ClearMission,1096)
MisResultAction(SetRecord, 1096)

----------------------------------------------------------Social Ambassador 21---------- Heaven Teleporter
DefineMission( 5594, "TOP Ambassador 21", 1097 )
MisBeginTalk("<t>Want to do me a favor? HmmI miss the Heaven Teleporter And also the money he stole from me. Please remind him so.")
			
MisBeginCondition(NoMission, 1097)
MisBeginCondition(NoRecord,1097)
MisBeginCondition(HasRecord, 1096)
MisBeginAction(AddMission,1097)
MisCancelAction(ClearMission, 1097)

MisNeed(MIS_NEED_DESP,"Look for <bHeaven Teleporter> at <j(474, 1054)>.")

MisHelpTalk("<t>I am the one in debt at this time of the year.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Ambassador 21------- Heaven Teleporter
DefineMission(5595, "TOP Ambassador 21", 1097, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>So petty, its only a small sum.")
MisResultCondition(NoRecord, 1097)
MisResultCondition(HasMission,1097)
MisResultAction(ClearMission,1097)
MisResultAction(SetRecord, 1097)

----------------------------------------------------------Social Hero 22---------- Heaven Teleporter
DefineMission( 5596, "TOP Hero 22", 1098 )
MisBeginTalk("<t>Congratulations, you have completed the arduous TOP quest. I heard <bCindy> is looking for you.")
			
MisBeginCondition(NoMission, 1098)
MisBeginCondition(NoRecord,1098)
MisBeginCondition(HasRecord, 1097)
MisBeginAction(AddMission,1098)
MisCancelAction(ClearMission, 1098)

MisNeed(MIS_NEED_DESP,"Look for <bCindy> at <j(740, 1451)>.")

MisHelpTalk("<t>I am the one in debt at this time of the year.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------Social Hero 22 ------- Cindy
DefineMission(5597, "TOP Hero 22", 1098, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You have passed TOP test, I will award you with an emblem.")
MisResultCondition(NoRecord, 1098)
MisResultCondition(HasMission,1098)
MisResultAction(ClearMission,1098)
MisResultAction(SetRecord, 1098)
MisResultAction(GiveItem, 2949, 1, 4)
MisResultBagNeed(1)

-------------------------------------------------Special missions
DefineMission (5600, "Special Operation C Vampiric Aries", 1101)

MisBeginTalk("<t>This quest is specially for <pAries Palace>. You can choose not to participate but there will be no prizes for you. Kill 99 <rMystic Shrubs> in 15 minutes is not an easy task.>.")

MisBeginCondition(NoMission,1101)
MisBeginCondition(NoRecord,1101)
MisBeginCondition(HasRecord,835)
MisBeginCondition(HasRecord,1071)
MisBeginCondition(HasRecord,1072)
MisBeginCondition(HasRecord,1073)
MisBeginCondition(HasRecord,1074)
MisBeginCondition(HasRecord,1075)
MisBeginCondition(HasRecord,1065)
MisBeginCondition(HasRecord,1098)
MisBeginAction(AddMission,1101)
MisBeginAction(AddChaItem3, 2952)---------special operations card
MisBeginAction(AddTrigger, 11011, TE_KILL, 75, 99 )
MisCancelAction(ClearMission, 1101)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_KILL, 75, 99, 1, 99)

MisHelpTalk("<t>Go now! You only have 15 minutes.")
MisResultTalk("<t>Not too bad, you will not regret it.")


MisResultCondition(HasMission, 1101)
MisResultCondition(NoRecord,1101)
MisResultCondition(HasFlag, 1101, 99 )
MisResultAction(AddChaItem4, 2952)----special operations card
MisResultAction(ClearMission, 1101)
MisResultAction(SetRecord,  1101 )
MisResultAction(GiveItem, 2955, 1, 4)------------Aries Captain Apparel Card
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 75)	
TriggerAction( 1, AddNextFlag, 1101, 1, 99 )
RegCurTrigger( 11011 )

----------------------------------------------------------april fool's fruit
DefineMission( 5601, "Fruit of April's Fool", 1102)

MisBeginTalk( "<t>Today is Aprils Fool day! I love to make a fool out of others. Do you dare to take up my challenge? I got some surprising rewards to be claimed. Ok, the first task: Collect 100 <yElven Fruits>!")
MisBeginCondition(NoRecord, 1102)
MisBeginCondition(NoMission, 1102)
MisBeginCondition(LvCheck, ">", 10 )
MisBeginAction(AddMission, 1102)
MisBeginAction(AddTrigger, 11021, TE_GETITEM, 3116, 100 )		
MisCancelAction(ClearMission, 1102)

MisNeed(MIS_NEED_ITEM, 3116, 100, 10, 100)

MisResultTalk("<t>You are quite honest. I shall give you an Aprils Fool Gift.")
MisHelpTalk("<t>What use does 100 <yCute Fruit of Aprils Fool> have? I am also unsure")
MisResultCondition(HasMission, 1102)
MisResultCondition(HasItem, 3116, 100 )
MisResultAction(TakeItem, 3116, 100 )
MisResultAction(ClearMission, 1102)
MisResultAction(SetRecord, 1102 )
MisResultAction(GiveItem, 2953, 1, 4)------------Gift of April's Fool
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1102, 10, 100 )
RegCurTrigger( 11021 )

------------------------------------------------------generosity for april fools
DefineMission( 5602, "Aprils Fool Bounty", 1103)

MisBeginTalk( "<t>I would like to borrow some gold from you, please do not reject me! My bunny baby is going to school soon and I need 1,000,000 gold! Lend me please!")
MisBeginCondition(NoRecord, 1103)
MisBeginCondition(HasRecord, 1102)
MisBeginCondition(NoMission, 1103)
MisBeginAction(AddMission, 1103)	
MisCancelAction(ClearMission, 1103)

MisResultTalk("<t>You do not expect me to return you the gold?!")
MisHelpTalk("<t>Do not be so petty, I might have a surprise for you.")
MisResultCondition(HasMission, 1103)
MisResultCondition(HasMoney, 1000000 )
MisResultAction(TakeMoney, 1000000 )
MisResultAction(ClearMission, 1103)
MisResultAction(SetRecord, 1103 )

------------------------------------------------------death of april fools
DefineMission( 5603, "Death of Aprils Fool", 1104)

MisBeginTalk( "<t>How can you be so naive to believe this? Haha! Want me to return the money? No possible, unless you die 41 times again and I will reconsider.")
MisBeginCondition(NoRecord, 1104)
MisBeginCondition(HasRecord, 1103)
MisBeginCondition(NoMission, 1104)
MisBeginCondition(HaveNoItem, 2954)
MisBeginAction(AddMission, 1104)	
MisBeginAction(GiveItem, 2954, 1, 4)------------Proof of Death
MisCancelAction(ClearMission, 1104)
MisBeginBagNeed(1)

MisResultTalk("<t>You are indeed the most blur and adorable person in Pirate King Online. Never mindI will tell give you some consolation.")
MisHelpTalk("<t>What is Death? Is there no fear?")
MisResultCondition(HasMission, 1104)
MisResultCondition(CheckPoint, 2954 )
MisResultAction(TakeItem, 2954,1,4)
MisResultAction(GiveItem, 0853,1,4)
MisResultAction(ClearMission, 1104)
MisResultAction(SetRecord, 1104 )
MisResultBagNeed(1)

----------------------------------------------------------
DefineMission( 6015, "Who is the Guardian", 1013, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t><bTutu> is always creating trouble for me.")
MisResultCondition(NoRecord, 1013)
MisResultCondition(HasMission, 1013)
MisResultAction(SetRecord, 1013)
MisResultAction(ClearMission, 1013)

---------------------------------------------------------
DefineMission( 6016, "Tough? Or troublesome?", 1014)
MisBeginTalk("<t>Path to rebirth is never simple. Go and inquire from <bClan Chief Albuda>.")
MisBeginCondition(NoRecord, 1014)
MisBeginCondition(NoMission, 1014)
MisBeginCondition(HasRecord, 1013)
MisBeginAction(AddMission, 1014)
MisBeginAction(ZSSTART)
MisCancelAction(ClearMission, 1014)

MisNeed(MIS_NEED_DESP,"<t>Look for <bClan Chief Albuda> to understand about the problem of rebirth.")
MisHelpTalk("<t>Look for <bClan Chief Albuda> in <pShaitan City>.")

MisResultCondition(AlwaysFailure)



-------------------------------------------------------------.
DefineMission( 6017, "Tough? Or irritating?", 1014, COMPLETE_SHOW )
MisBeginCondition(AlwaysFailure )
MisResultTalk("<t>Have you made your decision? If you wish to change your decision in the future, look for me again but there will be a price to pay.")
MisResultCondition(NoRecord, 1014)
MisResultCondition(HasMission, 1014)
MisResultCondition(HasRecord, 1059)
MisResultAction(ClearMission, 1014)
MisResultAction(SetRecord, 1014)


--------------------------------------------------------------.
DefineMission( 6018, "Continue the path of rebirth", 1017)
MisBeginTalk("<t>You have chosen your own path. Now look for Tink. He will guide you.")
MisBeginCondition(NoRecord, 1017)
MisBeginCondition(NoMission, 1017)
MisBeginCondition(HasRecord, 1014)
MisBeginAction(AddMission, 1017)
MisCancelAction(ClearMission, 1017)

MisNeed(MIS_NEED_DESP,"<t>Look for Passerby - Tink.")
MisHelpTalk("<t>Look for Tink now.")

MisResultCondition(AlwaysFailure)

-----------------------------------------------------------------
DefineMission( 6019, "Continue the path of rebirth", 1017, COMPLETE_SHOW )
MisBeginCondition(AlwaysFailure)
MisResultTalk("<t>Have you selected your path? Let us continue!")
MisResultCondition(NoRecord, 1017)
MisResultCondition(HasMission, 1017)
MisResultAction(ClearMission, 1017)
MisResultAction(SetRecord, 1017)

------------------------------------------------------------
DefineMission( 6020, "Arduous Quest", 1018)
MisBeginTalk("<t>Since you have selected the tough difficulty, I will give you a pointer or two. However, help me teach some guys a lesson first.")
MisBeginCondition(NoRecord,1000)
MisBeginCondition(HasRecord,1017)
MisBeginCondition(HasRecord,1015)
MisBeginCondition(NoRecord,1018)
MisBeginCondition(NoMission,1018)
MisBeginAction(AddMission,1018)
MisBeginAction(AddTrigger, 10181, TE_KILL,678, 1)
MisBeginAction(AddTrigger, 10182, TE_KILL,679, 1)
MisBeginAction(AddTrigger, 10183, TE_KILL,789, 1)
MisCancelAction(ClearMission,1018)

MisNeed(MIS_NEED_DESP,"Defeat <rSnowman Warlord, Wandering Soul and Black Dragon>.")
MisNeed(MIS_NEED_KILL, 678, 1, 10, 1)
MisNeed(MIS_NEED_KILL, 679, 1, 20, 1)
MisNeed(MIS_NEED_KILL, 789, 1, 30, 1)

--MisPrize(MIS_PRIZE_ITEM, 2229, 1, 4)
--MisPrize(MIS_PRIZE_ITEM, 2230, 1, 4)
--MisPrize(MIS_PRIZE_ITEM, 2231, 1, 4)
--MisPrize(MIS_PRIZE_ITEM, 2232, 1, 4)
--MisPrize(MIS_PRIZE_ITEM, 2233, 1, 4)
--MisPrizeSelAll()

MisHelpTalk("<t>One person is not enough. Look for some friends!")
MisResultTalk("<t>Thank you my friend, this is what you needed.")
MisResultCondition(HasMission,1018)
MisResultCondition(NoRecord,1018)
MisResultCondition(HasFlag, 1018, 10)
MisResultCondition(HasFlag, 1018, 20)
MisResultCondition(HasFlag, 1018, 30)

MisResultAction(SetRecord,1018)
MisResultAction(SetRecord,1056)
MisResultAction(ClearMission,1018)

MisResultAction(GiveItem, 2230, 1, 4)
MisResultAction(GiveItem, 2231, 1, 4)
MisResultAction(GiveItem, 2232, 1, 4)
MisResultAction(GiveItem, 2233, 1, 4)
MisResultAction(GiveItem, 2229, 1, 4)
MisResultBagNeed(5)

InitTrigger()
TriggerCondition( 1, IsMonster, 678 )
TriggerAction( 1, AddNextFlag, 1018, 10, 1 )
RegCurTrigger( 10181 )
InitTrigger()
TriggerCondition( 1, IsMonster, 679 )
TriggerAction( 1, AddNextFlag, 1018, 20, 1 )
RegCurTrigger( 10182 )
InitTrigger()
TriggerCondition( 1, IsMonster, 789 )
TriggerAction( 1, AddNextFlag, 1018, 30, 1 )
RegCurTrigger( 10183 )

-------------------------------------------------------
DefineMission( 6021, "Time based Challenge", 1019)
MisBeginTalk("<t>You selected the troublesome path? Ok, have you heard about the Genesis Challenge? You will have to complete it in 30 minutes.")
MisBeginCondition( NoRecord, 1000)
MisBeginCondition( NoRecord, 1019)
MisBeginCondition( NoMission, 1019)
MisBeginCondition( HasRecord, 1017)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1019)
MisCancelAction(ClearMission,1019)

MisNeed(MIS_NEED_DESP,"Complete Genesis challenge in 30 minutes.")

MisHelpTalk("<t>If the timing recorded on the <yBawcock Letter> is less than 1800 secs, I will consider you to be through. Please note to bring only 1 <yBawcock Letter>!")
MisResultTalk("<t>Its so fun.")
MisResultCondition( HasItem, 2912, 1)
MisResultCondition( LessTime, 1800)
MisResultCondition( NoRecord, 1019)
MisResultCondition( HasMission, 1019)
MisResultAction( SetRecord, 1019)
MisResultAction( ClearMission, 1019)


---------------------------------------------------------
DefineMission( 6022, "Time based Challenge", 1020)
MisBeginTalk("<t>Go for another round. However, please take your time to do it. Try to finish it between 7 hours to 8 hours.")
MisBeginCondition( NoRecord, 1020)
MisBeginCondition( NoMission, 1020)
MisBeginCondition( HasRecord, 1019)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1020)
MisCancelAction( ClearMission, 1020)

MisNeed(MIS_NEED_DESP,"You need to complete Genesis Challenge between 7 hours to 8 hours.")

MisHelpTalk("<t>The recorded timing on the <yBawcock Letter> must be higher than 25200 secs and lower than 28800 secs. I only accept if you have 1 <yBawcock Letter>!")
MisResultTalk("<t>Isn't the scenery nice?")
MisResultCondition( MoreTime, 25200)
MisResultCondition( LessTime, 28800)
MisResultCondition( NoRecord, 1020)
MisResultCondition( HasMission, 1020)
MisResultAction( SetRecord, 1020)
MisResultAction( ClearMission, 1020)



-----------------------------------------------------------
DefineMission( 6023, "Time based Challenge", 1021)
MisBeginTalk("<t>Do the run once more. You will need to complete the journey between 5 hours to 5.5 hours.")
MisBeginCondition( NoRecord, 1021)
MisBeginCondition( NoMission, 1021)
MisBeginCondition( HasRecord, 1020)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1021)
MisCancelAction( ClearMission, 1021)

MisNeed(MIS_NEED_DESP,"Complete Genesis Challenge must be between 5 hours to 5 hours 30 minutes.")

MisHelpTalk("<t>The recorded timing on the <yBawcock Letter> must be between 18000 secs to 19800 secs. Please note to bring only 1 <yBawcock Letter>!")
MisResultTalk("<t>I recognize your effort on this!")
MisResultCondition( LessTime, 19800)
MisResultCondition( MoreTime, 18000)
MisResultCondition( NoRecord, 1021)
MisResultCondition( HasMission, 1021)

MisResultAction( SetRecord, 1021)
MisResultAction( ClearMission, 1021)

---------------------------------------------------
DefineMission( 6024, "Visit the guardian of Thundoria", 1022)
MisBeginTalk("<t>Next guardian is at Thundoria Castle. Look for him there.")
MisBeginCondition( NoRecord, 1022)
MisBeginCondition( NoMission, 1022)
MisBeginCondition( HasRecord, 1021)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1022)
MisCancelAction( ClearMission, 1022)

MisNeed(MIS_NEED_DESP,"Look for the guardian of Thundoria Castle.")
MisHelpTalk("<t>Look for him in Thundoria Castle.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------
DefineMission( 6025, "Visit the guardian of Thundoria", 1022,COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You are finally here!")
MisResultCondition( NoRecord, 1022)
MisResultCondition( HasMission, 1022)
MisResultAction( SetRecord, 1022)
MisResultAction( ClearMission, 1022)

----------------------------------------------------------------
DefineMission( 6026, "Battle of Aries Palace", 1023)
MisBeginTalk("<t>The wheel of fate has started to turn! Enter the gate of the <pHoroscope Palace> now and seek the <ySeal of Aries> in <pAries Palace>.")
MisBeginCondition( NoRecord, 1023)
MisBeginCondition( NoMission, 1023)
MisBeginCondition( HasRecord, 1022)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1023)
MisBeginAction(AddTrigger, 10231, TE_GETITEM, 2942, 1 )
MisCancelAction( ClearMission, 1023)

MisNeed(MIS_NEED_DESP,"Wear a full set of <yAries apparel> and bring the <ySeal of Aries> to <bArgent Ambassador C Yata> in Thundoria.")
MisNeed(MIS_NEED_ITEM, 2942, 1, 10 ,1)

MisHelpTalk("<t>Wear the apparel set of Aries Palace to welcome me.")
MisResultTalk("<t>You are indeed the chosen one.")
MisResultCondition( HasItem, 2942, 1)
MisResultCondition( BaiyangOn )
MisResultAction( SetRecord, 1023)
MisResultAction( ClearMission, 1023)

InitTrigger()
TriggerCondition( 1, IsItem, 2942)	
TriggerAction( 1, AddNextFlag, 1023, 10, 1 )
RegCurTrigger( 10231 )

----------------------------------------------------------------------
DefineMission( 6027, "Visit guardian of Shaitan", 1024)
MisBeginTalk("<t>Look for the next guardian at a sandy town.")
MisBeginCondition( NoRecord, 1024)
MisBeginCondition( NoMission, 1024)
MisBeginCondition( HasRecord, 1023)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1024)
MisCancelAction( ClearMission, 1024)

MisNeed(MIS_NEED_DESP,"Visit the guardian of Shaitan City.")
MisHelpTalk("<t>Go to a sandy city.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------
DefineMission( 6028, "Visit guardian of Shaitan", 1024, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

--	MisResultTalk("<t>Do not ignore me because I look weak!")
MisResultCondition( NoRecord, 1024)
MisResultCondition( HasMission, 1024)
MisResultAction( SetRecord, 1024)
MisResultAction( ClearMission, 1024)

---------------------------------------------------------------------------
DefineMission( 6029, "World Tour", 1025)
MisBeginTalk("<t>Are you familiar with the world of Pirate King Online? Let me bring you on a world tour. There are many interesting people to meet along the way and they will take a portion of your reputation points for their help. First stop is <pAbandoned Mine Haven>. Look for <bKentaro>.")
MisBeginCondition( NoMission, 1025)
MisBeginCondition( NoRecord, 1025)
MisBeginCondition( HasRecord, 1024)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1025)
MisCancelAction( ClearMission, 1025)

MisNeed(MIS_NEED_DESP,"Look for <bKentaro> when you have at least 500 reputation points.")
MisHelpTalk("<t><bKentaro> is at <pAbandon Mine Haven>.")

MisResultCondition(AlwaysFailure)

------------------------------------------------------------------------
DefineMission( 6030, "World Tour", 1025, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Tourist? Remember to rest well!")
MisResultCondition( NoRecord, 1025)
MisResultCondition( HasMission, 1025)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1025)
MisResultAction( ClearMission, 1025)

--------------------------------------------------------------------------
DefineMission( 6031, "World Tour", 1026)
MisBeginTalk("<t>Next stop is <pBabul Haven>. Look for <bMinelli>.")
MisBeginCondition( NoMission, 1026)
MisBeginCondition( NoRecord, 1026)
MisBeginCondition( HasRecord, 1025)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1026)
MisCancelAction( ClearMission, 1026)

MisNeed(MIS_NEED_DESP,"Look for <bMinelli> when you have at least 500 reputation points.")
MisHelpTalk("<t><bMinelli> is at <pBabul Haven>.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------
DefineMission( 6032, "World Tour", 1026, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Tourist? Remember to rest well!")
MisResultCondition( NoRecord, 1026)
MisResultCondition( HasMission, 1026)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1026)
MisResultAction( ClearMission, 1026)

---------------------------------------------------------------------------
DefineMission( 6033, "World Tour", 1027)
MisBeginTalk("<t>Next stop is <pAtlantis Haven>. Look for <bWilli>.")
MisBeginCondition( NoMission, 1027)
MisBeginCondition( NoRecord, 1027)
MisBeginCondition( HasRecord, 1026)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1027)
MisCancelAction( ClearMission, 1027)

MisNeed(MIS_NEED_DESP,"Look for <bWilli> when you have at least 500 reputation points.")
MisHelpTalk("<t>You cannot take it anymore? Look for Clan Chief Albuda to reselect your path of rebirth.")

MisResultCondition(AlwaysFailure)

----------------------------------------------------------------------------
DefineMission( 6034, "World Tour", 1027, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1027)
MisResultCondition( HasMission, 1027)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1027)
MisResultAction( ClearMission, 1027)

-----------------------------------------------------------------------------
DefineMission( 6035, "World Tour", 1028)
MisBeginTalk("<t>Next stop is <pValhalla Haven>. Look for <bProfessor Fenny>.")
MisBeginCondition( NoMission, 1028)
MisBeginCondition( NoRecord, 1028)
MisBeginCondition( HasRecord, 1027)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1028)
MisCancelAction( ClearMission, 1028)

MisNeed(MIS_NEED_DESP,"Look for <bProfessor Fenny> when you have 500 reputation points.")
MisHelpTalk("<t><bProfessor Fenny> is at <pValhalla Haven>.")

MisResultCondition(AlwaysFailure)

---------------------------------------------------------------------------
DefineMission( 6036, "World Tour", 1028, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1028)
MisResultCondition( HasMission, 1028)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1028)
MisResultAction( ClearMission, 1028)

---------------------------------------------------------------------------
DefineMission( 6037, "World Tour", 1029)
MisBeginTalk("<t>Next stop is <pOasis Haven>. Look for <bDitaro>.")
MisBeginCondition( NoMission, 1029)
MisBeginCondition( NoRecord, 1029)
MisBeginCondition( HasRecord, 1028)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1029)
MisCancelAction( ClearMission, 1029)

MisNeed(MIS_NEED_DESP,"Look for <bDitaro> when you have 500 reputation points.")
MisHelpTalk("<t>Ditaro is at Oasis Haven.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------
DefineMission( 6038, "World Tour", 1029, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1029)
MisResultCondition( HasMission, 1029)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1029)
MisResultAction( ClearMission, 1029)

-------------------------------------------------------------------------
DefineMission( 6039, "World Tour", 1030)
MisBeginTalk("<t>Next stop is <pIcespire Haven>. Look for <bLulu>.")
MisBeginCondition( NoMission, 1030)
MisBeginCondition( NoRecord, 1030)
MisBeginCondition( HasRecord, 1029)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1030)
MisCancelAction( ClearMission, 1030)

MisNeed(MIS_NEED_DESP,"Look for <bLulu> when you have 500 reputation points.")
MisHelpTalk("<t>Lulu is at Icespire Haven.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------
DefineMission( 6040, "World Tour", 1030, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1030)
MisResultCondition( HasMission, 1030)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1030)
MisResultAction( ClearMission, 1030)

-------------------------------------------------------------------------
DefineMission( 6041, "World Tour", 1031)
MisBeginTalk("<t>Next stop is <pRockery Haven>. Look for <bDurian>.")
MisBeginCondition( NoMission, 1031)
MisBeginCondition( NoRecord, 1031)
MisBeginCondition( HasRecord, 1030)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1031)
MisCancelAction( ClearMission, 1031)

MisNeed(MIS_NEED_DESP,"Look for <bDurian> when you have 500 reputation points.")
MisHelpTalk("<t><bDurian> is at <pRockery Haven>.")

MisResultCondition(AlwaysFailure)

---------------------------------------------------------------------------
DefineMission( 6042, "World Tour", 1031, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1031)
MisResultCondition( HasMission, 1031)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1031)
MisResultAction( ClearMission, 1031)

---------------------------------------------------------------------------
DefineMission( 6043, "World Tour", 1032)
MisBeginTalk("<t>Next stop is <pSolace Haven>. Look for <bLinda>.")
MisBeginCondition( NoMission, 1032)
MisBeginCondition( NoRecord, 1032)
MisBeginCondition( HasRecord, 1031)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1032)
MisCancelAction( ClearMission, 1032)

MisNeed(MIS_NEED_DESP," Look for <bLinda> when you have at least 500 reputation points.")
MisHelpTalk("<t><bLinda> is at <pSolace Haven>.")

MisResultCondition(AlwaysFailure)

-----------------------------------------------------------------------------
DefineMission( 6044, "World Tour", 1032, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1032)
MisResultCondition( HasMission, 1032)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( GiveItem, 3128, 10, 4)
MisResultAction( SetRecord, 1032)
MisResultAction( ClearMission, 1032)
MisResultBagNeed(1)


-----------------------------------------------------------------------------
DefineMission( 6045, "World Tour", 1033)
MisBeginTalk("<t>Next stop is at <pSkeletar Haven>. Look for <bXeus>.")
MisBeginCondition( NoMission, 1033)
MisBeginCondition( NoRecord, 1033)
MisBeginCondition( HasRecord, 1032)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1033)
MisCancelAction( ClearMission, 1033)

MisNeed(MIS_NEED_DESP,"Look for <bXeus> when you have at least 500 reputation points.")
MisHelpTalk("<t><bXeus> is at <pSkeletar Haven>.")

MisResultCondition(AlwaysFailure)

------------------------------------------------------------------------------
DefineMission( 6046, "World Tour", 1033, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1033)
MisResultCondition( HasMission, 1033)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1033)
MisResultAction( ClearMission, 1033)

------------------------------------------------------------------------------
DefineMission( 6047, "World Tour", 1034)
MisBeginTalk("<t>Next stop is at <pChaldea Haven>. Look for <bLove Yuri>.")
MisBeginCondition( NoMission, 1034)
MisBeginCondition( NoRecord, 1034)
MisBeginCondition( HasRecord, 1033)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1034)
MisCancelAction( ClearMission, 1034)

MisNeed(MIS_NEED_DESP,"Look for <bLove Yuri> when you have at least 500 reputation points.")
MisHelpTalk("<t><bLove Yuri> is at <pChaldea Haven>.")

MisResultCondition(AlwaysFailure)

------------------------------------------------------------------------------
DefineMission( 6048, "World Tour", 1034, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1034)
MisResultCondition( HasMission, 1034)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1034)
MisResultAction( ClearMission, 1034)

------------------------------------------------------------------------------
DefineMission( 6049, "World Tour", 1035)
MisBeginTalk("<t>Next stop is <bNana> at <pIcicle Haven>.")
MisBeginCondition( NoMission, 1035)
MisBeginCondition( NoRecord, 1035)
MisBeginCondition( HasRecord, 1034)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1035)
MisCancelAction( ClearMission, 1035)

MisNeed(MIS_NEED_DESP,"Look for Nana when you have 500 reputation points.")
MisHelpTalk("<t><bNana> is at <pIcicle Haven>.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------------
DefineMission( 6050, "World Tour", 1035, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1035)
MisResultCondition( HasMission, 1035)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1035)
MisResultAction( ClearMission, 1035)

-------------------------------------------------------------------------------
DefineMission( 6051, "World Tour", 1036)
MisBeginTalk("<t>Next stop is at <pAndes Forest Haven>. Look for <bDoctor Minoseva>.")
MisBeginCondition( NoMission, 1036)
MisBeginCondition( NoRecord, 1036)
MisBeginCondition( HasRecord, 1035)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1036)
MisCancelAction( ClearMission, 1036)

MisNeed(MIS_NEED_DESP,"Look for <bMinoseva> when you have 500 reputation points.")
MisHelpTalk("<t><bMinoseva> is at <pAndes Forest Haven>.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------------
DefineMission( 6052, "World Tour", 1036, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>")
MisResultCondition( NoRecord, 1036)
MisResultCondition( HasMission, 1036)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( GiveItem, 3139, 10, 4)
MisResultAction( SetRecord, 1036)
MisResultAction( ClearMission, 1036)
MisResultBagNeed(1)


-----------------------------------------------------------------------------------
DefineMission( 6053, "World Tour", 1037)
MisBeginTalk("<t>The tour has ended. Go back and look for <bPasserby Wowo>.")
MisBeginCondition( NoMission, 1037)
MisBeginCondition( NoRecord, 1037)
MisBeginCondition( HasRecord, 1036)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1037)
MisCancelAction( ClearMission, 1037)

MisNeed(MIS_NEED_DESP,"Look for <bWowo>.")
MisHelpTalk("<t>Look for <bWowo>.")

MisResultCondition(AlwaysFailure)

-----------------------------------------------------------------------------------
DefineMission( 6054, "World Tour", 1037, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>How is it? Is the tour interesting? Do you want to do it once again? Haha, I am only joking. Do not worry!")
MisResultCondition( NoRecord, 1037)
MisResultCondition( HasMission, 1037)
MisResultCondition( HasCredit, 500)
MisResultAction( DelRoleCredit, 500)
MisResultAction( SetRecord, 1037)
MisResultAction( ClearMission, 1037)

----------------------------------------------------------------------------
DefineMission( 6055, "Visit the Guardian of Heaven", 1038)
MisBeginTalk("<t>Look for the next guardian at the entrance of <pHeaven>.")
MisBeginCondition( NoMission, 1038)
MisBeginCondition( NoRecord, 1038)
MisBeginCondition( HasRecord, 1037)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission,1038)
MisCancelAction( ClearMission, 1038)

MisNeed(MIS_NEED_DESP,"Look for the next guardian.")
MisHelpTalk("<t>Next guardian is at the entrance to <pHeaven>.")

MisResultCondition(AlwaysFailure)

---------------------------------------------------------------------------
DefineMission( 6056, "Visit the Guardian of Heaven", 1038, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Have you meet the <bGoddess>?")
MisResultCondition( NoRecord, 1038)
MisResultCondition( HasMission, 1038)
MisResultAction( SetRecord, 1038)
MisResultAction( ClearMission, 1038)

-------------------------------------------------------------------------------
DefineMission( 6057, "Testament of the Piety", 1039)
MisBeginTalk("<t>Want to gain my recognition? No problem, but you have to show me your faithfulness to the <bGoddess>. There are 9 <ySacred Candles>. Light them up!")
MisBeginCondition( NoRecord, 1039)
MisBeginCondition( NoMission, 1039)
MisBeginCondition( HasRecord, 1038)
MisBeginCondition( HasRecord, 1016)
MisBeginAction(AddTrigger, 10391, TE_GETITEM, 3007, 9 )
MisBeginAction( AddMission, 1039)
MisBeginAction( GiveItem, 3006, 9, 4)
MisBeginBagNeed(1)
MisCancelAction( ClearMission, 1039)

MisNeed(MIS_NEED_ITEM, 3007, 9, 10, 9)

MisResultTalk("<t>Well done.")
MisHelpTalk("<t>Use these <ySacred Candles> to light them.")
MisResultCondition( NoRecord, 1039)
MisResultCondition( HasMission, 1039)
MisResultCondition( HasItem, 3007, 9)
MisResultAction( TakeItem, 3007, 9)
MisResultAction( SetRecord, 1039)
MisResultAction( ClearMission, 1039)

InitTrigger()
TriggerCondition( 1, IsItem, 3007)	
TriggerAction( 1, AddNextFlag, 1039, 10, 9 )
RegCurTrigger( 10391 )


-------------------------------------------------------------------------------
DefineMission( 6058, "Testament of the Piety", 1040)
MisBeginTalk("<t>Since you are so determined, light these 99 candles for me.")
MisBeginCondition( NoRecord, 1040)
MisBeginCondition( NoMission, 1040)
MisBeginCondition( HasRecord, 1039)
MisBeginCondition( HasRecord, 1016)
MisBeginAction(AddTrigger, 10401, TE_GETITEM, 3007, 99 )
MisBeginAction( AddMission, 1040)
MisBeginAction( GiveItem, 3006, 99, 4)
MisBeginBagNeed(1)
MisCancelAction( ClearMission, 1040)

MisNeed(MIS_NEED_ITEM, 3007, 99, 10, 99)

MisResultTalk("<t>Well done!")
MisHelpTalk("<t>Use these <ySacred Candles> to light them.")
MisResultCondition( NoRecord, 1040)
MisResultCondition( HasMission, 1040)
MisResultCondition( HasItem, 3007, 99)
MisResultAction( TakeItem, 3007, 99)
MisResultAction( SetRecord, 1040)
MisResultAction( ClearMission, 1040)

InitTrigger()
TriggerCondition( 1, IsItem, 3007)	
TriggerAction( 1, AddNextFlag, 1040, 10, 99 )
RegCurTrigger( 10401 )

-------------------------------------------------------------------------------
DefineMission( 6059, "Testament of the Piety", 1041)
MisBeginTalk("<t>This is the last test. Here is a Goddess Statue. You will infuse 1 point of energy everytime you use it. Come back to me when you maximise the energy. Note that I only accept 1 Goddess Statue!")
MisBeginCondition( NoRecord, 1041)
MisBeginCondition( NoMission, 1041)
MisBeginCondition( HasRecord, 1040)
MisBeginCondition( HasRecord, 1016)
MisBeginAction(AddTrigger, 10411, TE_GETITEM, 3010, 1 )
MisBeginAction( AddMission, 1041)
MisBeginAction( GiveNSDX, 3010 )
MisBeginBagNeed(1)
MisCancelAction( ClearMission, 1041)

MisNeed(MIS_NEED_ITEM, 3010, 1, 10, 1)

MisResultTalk("<t>The <bGoddess> will always be at your side.")
MisHelpTalk("<t>You cannot take it anymore? Look for Clan Chief Albuda to reselect your path of rebirth.")
MisResultCondition( NoRecord, 1041)
MisResultCondition( HasMission, 1041)
MisResultCondition( CheckEnergy )
MisResultAction( TakeItem, 3010, 1)
MisResultAction( SetRecord, 1041)
MisResultAction( ClearMission, 1041)

InitTrigger()
TriggerCondition( 1, IsItem, 3010)	
TriggerAction( 1, AddNextFlag, 1041, 10, 1 )
RegCurTrigger( 10411 )

------------------------------------------------------------------------
DefineMission( 6060, "Visit the guardian of Argent", 1042)
MisBeginTalk("<t> Look for the next guardian in Argent City. He is not someone to mess with.")
MisBeginCondition( NoRecord, 1042)
MisBeginCondition( NoMission, 1042)
MisBeginCondition( HasRecord, 1041)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1042)
MisCancelAction( ClearMission, 1042)

MisNeed(MIS_NEED_DESP,"Look for the next guardian in Argent City.")
MisHelpTalk("<t>Visit the next guardian in Argent City.")

MisResultCondition(AlwaysFailure)

-------------------------------------------------------------------------
DefineMission( 6061, "Visit the guardian of Argent", 1042, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>Want to play a game?")
MisResultCondition( NoRecord, 1042)
MisResultCondition( HasMission, 1042)
MisResultAction( SetRecord, 1042)
MisResultAction( ClearMission, 1042)

-----------------------------------------------------------------------------
DefineMission( 6062, "Game of Madness", 1043)
MisBeginTalk("<t>Lets play a little game. Kill 110x <rMystic Shrubs> and return to answer my question.")
MisBeginCondition( NoRecord, 1043)
MisBeginCondition( NoMission, 1043)
MisBeginCondition( HasRecord, 1042)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1043)
MisBeginAction( AddTrigger, 10431, TE_KILL, 75, 110)
MisCancelAction( ClearMission, 1043)

MisNeed(MIS_NEED_DESP,"Kill 110x <rMystic Shrubs>.")
MisNeed(MIS_NEED_KILL, 75, 110, 10, 110)

MisResultTalk("<t>You kill fast.")
MisHelpTalk("<t>There are many <rMystic Shrubs> surrounding <pArgent City>. You will need to answer a question after killing them!")
MisResultCondition( NoRecord, 1043)
MisResultCondition( HasMission, 1043)
MisResultCondition( HasFlag, 1043, 10)
MisResultCondition( HasRecord, 1058)
MisResultAction( SetRecord, 1043)
MisResultAction( ClearMission, 1043)

InitTrigger()
TriggerCondition( 1, IsMonster, 75 )
TriggerAction( 1, AddNextFlag, 1043, 10, 110 )
RegCurTrigger( 10431 )

-----------------------------------------------------------------------------
DefineMission( 6063, "Game of Madness", 1046)
MisBeginTalk("<t>Kill 110x <rDry Mystic Shrubs>!")
MisBeginCondition( NoRecord, 1046)
MisBeginCondition( NoMission, 1046)
MisBeginCondition( HasRecord, 1043)
MisBeginCondition( HasRecord, 1044)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1046)
MisBeginAction( AddTrigger, 10461, TE_KILL, 218, 110)
MisCancelAction( ClearMission, 1046)

MisNeed(MIS_NEED_DESP,"Kill 110x <rDry Mystic Shrubs>.")
MisNeed(MIS_NEED_KILL, 218, 110, 10, 110)

MisResultTalk("<t>You kill fast.")
MisHelpTalk("<t><rDry Mystic Shrub> can be found in the surrounding of <pShaitan City>.")
MisResultCondition( NoRecord, 1046)
MisResultCondition( HasMission, 1046)
MisResultCondition( HasFlag, 1046, 10)
MisResultCondition( HasRecord, 1058)
MisResultAction( SetRecord, 1046)
MisResultAction( SetRecord, 1057)
MisResultAction( ClearMission, 1046)

InitTrigger()
TriggerCondition( 1, IsMonster, 218 )
TriggerAction( 1, AddNextFlag, 1046, 10, 110 )
RegCurTrigger( 10461 )

---------------------------------------------------------------------------------
DefineMission( 6064, "Game of Madness", 1047)
MisBeginTalk("<t>Kill 110x <rSnowy Mystic Shrubs>!")
MisBeginCondition( NoRecord, 1047)
MisBeginCondition( NoMission, 1047)
MisBeginCondition( HasRecord, 1045)
MisBeginCondition( HasRecord, 1043)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1047)
MisBeginAction( AddTrigger, 10471, TE_KILL, 216, 110)
MisCancelAction( ClearMission, 1047)

MisNeed(MIS_NEED_DESP,"Kill 110x <rSnowy Mystic Shrubs>.")
MisNeed(MIS_NEED_KILL, 216, 110, 10, 110)

MisResultTalk("<t>You kill fast.")
MisHelpTalk("<t><rSnowy Mystic Shrub> can be found in the surrounding of <pIcicle City>.")
MisResultCondition( NoRecord, 1047)
MisResultCondition( HasMission, 1047)
MisResultCondition( HasFlag, 1047, 10)
MisResultCondition( HasRecord, 1058)
MisResultAction( SetRecord, 1047)
MisResultAction( SetRecord, 1057)
MisResultAction( ClearMission, 1047)

InitTrigger()
TriggerCondition( 1, IsMonster, 216 )
TriggerAction( 1, AddNextFlag, 1047, 10, 110 )
RegCurTrigger( 10471 )

---------------------------------------------------------------------------------
DefineMission( 6065, "Game of Madness", 1048)
MisBeginTalk("<t>Next target is <rSailor Squidy>. Kill 110x of them too.")
MisBeginCondition( NoRecord, 1048)
MisBeginCondition( NoMission, 1048)
MisBeginCondition( HasRecord, 1057)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1048)
MisBeginAction( AddTrigger, 10481, TE_KILL, 233, 110)
MisCancelAction( ClearMission, 1048)

MisNeed(MIS_NEED_DESP,"Kill 110x <rSailor Squidy>.")
MisNeed(MIS_NEED_KILL, 233, 110, 10, 110)

MisResultTalk("<t>Isn't this game fun?")
MisHelpTalk("<t><rSailor Squidy> is near <pArgent City>.")
MisResultCondition( NoRecord, 1048)
MisResultCondition( HasMission, 1048)
MisResultCondition( HasFlag, 1048, 10)
MisResultAction( SetRecord, 1048)
MisResultAction( ClearMission, 1048)

InitTrigger()
TriggerCondition( 1, IsMonster, 233)
TriggerAction( 1, AddNextFlag, 1048, 10, 110 )
RegCurTrigger( 10481 )
--Stopped for now -Phai --
---------------------------------------------------------------------------------
DefineMission( 6066, "Game of Madness", 1049)
MisBeginTalk("<t>Next target is Snow Squidy, kill 110 of them.")
MisBeginCondition( NoRecord, 1049)
MisBeginCondition( NoMission, 1049)
MisBeginCondition( HasRecord, 1048)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1049)
MisBeginAction( AddTrigger, 10491, TE_KILL, 235, 110)
MisCancelAction( ClearMission, 1049)

MisNeed(MIS_NEED_DESP,"Kill 110 Snow Squidy.")
MisNeed(MIS_NEED_KILL, 235, 110, 10, 110)

MisResultTalk("<t>Isn't this game fun?")
MisHelpTalk("<t>Snow Squidy can be found near Icicle City.")
MisResultCondition( NoRecord, 1049)
MisResultCondition( HasMission, 1049)
MisResultCondition( HasFlag, 1049, 10)
MisResultAction( SetRecord, 1049)
MisResultAction( ClearMission, 1049)

InitTrigger()
TriggerCondition( 1, IsMonster, 235)
TriggerAction( 1, AddNextFlag, 1049, 10, 110 )
RegCurTrigger( 10491 )

---------------------------------------------------------------------------------
DefineMission( 6067, "Game of Madness", 1050)
MisBeginTalk("<t>Next target is Sailor Squirt. Kill 110 of them.")
MisBeginCondition( NoRecord, 1050)
MisBeginCondition( NoMission, 1050)
MisBeginCondition( HasRecord, 1049)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1050)
MisBeginAction( AddTrigger, 10501, TE_KILL, 232, 110)
MisCancelAction( ClearMission, 1050)

MisNeed(MIS_NEED_DESP,"Kill 110 Sailor Squirt.")
MisNeed(MIS_NEED_KILL, 232, 110, 10, 110)

MisResultTalk("<t>Isn't this game fun?")
MisHelpTalk("<t>Don't you find this game fun?")
MisResultCondition( NoRecord, 1050)
MisResultCondition( HasMission, 1050)
MisResultCondition( HasFlag, 1050, 10)
MisResultAction( SetRecord, 1050)
MisResultAction( ClearMission, 1050)

InitTrigger()
TriggerCondition( 1, IsMonster, 232)
TriggerAction( 1, AddNextFlag, 1050, 10, 110 )
RegCurTrigger( 10501 )

---------------------------------------------------------------------------------
DefineMission( 6068, "Game of Madness", 1051)
MisBeginTalk("<t>The last target is Snow Squirt. Kill 110 of them.")
MisBeginCondition( NoRecord, 1051)
MisBeginCondition( NoMission, 1051)
MisBeginCondition( HasRecord, 1050)
MisBeginCondition( HasRecord, 1016)
MisBeginAction( AddMission, 1051)
MisBeginAction( AddTrigger, 10511, TE_KILL, 234, 110)
MisCancelAction( ClearMission, 1051)

MisNeed(MIS_NEED_DESP,"Kill 110Snow Squirt.")
MisNeed(MIS_NEED_KILL, 234, 110, 10, 110)

MisResultTalk("<t>Is the game over? What a pity. Your determination has moved me. This is the reward you deserved.")
MisHelpTalk("<t>Go for it!")
MisResultCondition( NoRecord, 1051)
MisResultCondition( HasMission, 1051)
MisResultCondition( HasFlag, 1051, 10)
MisResultAction( SetRecord, 1051)
MisResultAction( SetRecord, 1056)
MisResultAction( ClearMission, 1051)
MisResultAction(GiveItem, 2229, 1, 4)
MisResultAction(GiveItem, 2230, 1, 4)
MisResultAction(GiveItem, 2231, 1, 4)
MisResultAction(GiveItem, 2232, 1, 4)
MisResultAction(GiveItem, 2233, 1, 4)
MisResultBagNeed(5)

InitTrigger()
TriggerCondition( 1, IsMonster, 234)
TriggerAction( 1, AddNextFlag, 1051, 10, 110 )
RegCurTrigger( 10511 )

-----------------------------------------------------------	
DefineMission (5604, "Ingredient to make sushi", 1107)

MisBeginTalk("<t>Dear friend, do you wish to have some sushi? You will never taste anything as good as these! Get me some ingredients if you wish to have it! ")

MisBeginCondition(NoMission,1107)
MisBeginCondition(NoRecord,1107)
MisBeginAction(AddMission,1107)
MisBeginAction(AddTrigger, 11071, TE_GETITEM, 1649, 1 )---------------1
MisBeginAction(AddTrigger, 11072, TE_GETITEM, 1690, 1 )---------------1
MisBeginAction(AddTrigger, 11073, TE_GETITEM, 3116, 1 )-------------1
MisCancelAction( ClearMission, 1107)

MisNeed(MIS_NEED_ITEM, 1649, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 1690, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 3116, 1, 30, 1 )

MisHelpTalk("<t>These items are not tough to find. Go now!")
MisResultTalk("<t>This is great, I will let you see the greatest work of the Kitchen God.")

MisResultCondition(HasMission, 1107)
MisResultCondition(NoRecord,1107)
MisResultCondition(HasItem, 1649, 1 )
MisResultCondition(HasItem, 1690, 1 )
MisResultCondition(HasItem, 3116, 1 )

MisResultAction(TakeItem, 1649, 1 )
MisResultAction(TakeItem, 1690, 1 )
MisResultAction(TakeItem, 3116, 1 )

MisResultAction(ClearMission, 1107)
MisResultAction(SetRecord,  1107 )


InitTrigger()
TriggerCondition( 1, IsItem, 1649)	
TriggerAction( 1, AddNextFlag, 1107, 10, 1 )
RegCurTrigger( 11071 )

InitTrigger()
TriggerCondition( 1, IsItem, 1690)	
TriggerAction( 1, AddNextFlag, 1107, 20, 1 )
RegCurTrigger( 11072 )

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1107, 30, 1 )
RegCurTrigger( 11073 )


-----------------------------------------------------------	
DefineMission (5605, "Super Sushi", 1108)

MisBeginTalk("<t>Bring 2 Cake Sampling Voucher if you wish to taste it! ")

MisBeginCondition(NoMission,1108)
MisBeginCondition(HasRecord,1107)
MisBeginCondition(NoRecord,1108)
MisBeginAction(AddMission,1108)
MisBeginAction(AddTrigger, 11081, TE_GETITEM, 1097, 2 )---------------2
MisCancelAction( ClearMission, 1108)

MisNeed(MIS_NEED_ITEM, 1097, 2, 10, 2 )

MisHelpTalk("<t>Faster, if not it will get cold.")
MisResultTalk("<t>Do not praise me! I am sick of such words")

MisResultCondition(HasMission, 1108)
MisResultCondition(NoRecord,1108)
MisResultCondition(HasItem, 1097, 2 )
MisResultAction(TakeItem, 1097, 2 )
MisResultAction(GiveItem, 2989,1,4)-------
MisResultAction(ClearMission, 1108)
MisResultAction(SetRecord,  1108 )
MisResultAction(ClearRecord, 1107)---------------can be repeated
MisResultAction(ClearRecord, 1108)---------------can be repeated

InitTrigger()
TriggerCondition( 1, IsItem, 1097)	
TriggerAction( 1, AddNextFlag, 1108, 10, 2 )
RegCurTrigger( 11081 )

-----------------------------------------------------------	
DefineMission (5606, "Cake Warrior", 1109)

MisBeginTalk("<t>Dear friend, my deserts are the most delicious in the market! Others will envy you should you ever taste my cakes. In order to sample it, you will have to obtain a Medal of Honor from the Arena Administrator! ")

MisBeginCondition(NoMission,1109)
MisBeginCondition(NoRecord,1109)
MisBeginAction(AddMission,1109)
MisBeginAction(AddTrigger, 11091, TE_GETITEM, 3849, 1 )---------------1
MisCancelAction( ClearMission, 1109)

MisNeed(MIS_NEED_ITEM, 3849, 1, 10, 1 )

MisHelpTalk("<t>Faster, if not it will get cold.")
MisResultTalk("<t>Do not praise me! I am sick of such words")

MisResultCondition(HasMission, 1109)
MisResultCondition(NoRecord,1109)
MisResultCondition(HasItem, 3849, 1 )
MisResultAction(ClearMission, 1109)
MisResultAction(SetRecord,  1109 )

InitTrigger()
TriggerCondition( 1, IsItem, 3849)	
TriggerAction( 1, AddNextFlag, 1109, 10, 1 )
RegCurTrigger( 11091 )


-----------------------------------------------------------	
DefineMission (5607, "Famous Cake", 1110)

MisBeginTalk("<t>You need to have a Cake Sampling Voucher as well! ")

MisBeginCondition(NoMission,1110)
MisBeginCondition(HasRecord,1109)
MisBeginCondition(NoRecord,1110)
MisBeginAction(AddMission,1110)
MisBeginAction(AddTrigger, 11101, TE_GETITEM, 1097, 1 )---------------1
MisCancelAction( ClearMission, 1110)

MisNeed(MIS_NEED_ITEM, 1097, 1, 10, 1 )
MisHelpTalk("<t>You need to have a Cake Sampling Voucher to taste my cake.")
MisResultTalk("<t>I have high hopes for you.")


MisResultCondition(HasMission, 1110)
MisResultCondition(NoRecord,1110)
MisResultCondition(HasItem, 1097, 1 )
MisResultAction(TakeItem, 1097, 1 )
MisResultAction(GiveItem, 2988,1,4)------
MisResultAction(ClearMission, 1110)
MisResultAction(SetRecord,  1110 )
MisResultAction(ClearRecord, 1109)---------------can be repeated
MisResultAction(ClearRecord, 1110)---------------can be repeated

InitTrigger()
TriggerCondition( 1, IsItem, 1097)	
TriggerAction( 1, AddNextFlag, 1110, 10, 1 )
RegCurTrigger( 11101 )


--	-----------------------------------------------------
--	DefineMission( 6069, "Server opening ceremony", 1400)
--
--	MisBeginTalk( "<t>Feeling shy lately? If you help me do a small flavour, I'll give you a rich reward in return ")
--	MisBeginCondition( LvCheck, ">", 10)
--	MisBeginCondition( NoRecord, 1400)
--	MisBeginCondition( NoMission, 1400)
--	MisBeginAction( AddMission, 1400 )
--	MisBeginAction( AddTrigger, 14001, TE_GETITEM, 1604, 10 )
--	MisBeginAction( AddTrigger, 14002, TE_GETITEM, 1777, 5 )
--	MisBeginAction( AddTrigger, 14002, TE_GETITEM, 1692, 5 )
--	MisBeginAction( AddTrigger, 14002, TE_GETITEM, 4334, 5 )
--	MisBeginAction( AddTrigger, 14002, TE_GETITEM, 4511, 1 )
--	MisCancelAction(ClearMission, 1400)
--
--	MisNeed(MIS_NEED_ITEM, 1604, 10, 10, 10)
--	MisNeed(MIS_NEED_ITEM, 1779, 5, 20, 5)
--	MisNeed(MIS_NEED_ITEM, 1692, 5, 30, 5)
--	MisNeed(MIS_NEED_ITEM, 4334, 5, 40, 5)
--	MisNeed(MIS_NEED_ITEM, 4511, 1, 50, 1)
--
--	MisResultTalk("<t>Nice job, I won't forget your reward!")
--	MisHelpTalk("<t>Mane is dropped by Bear Cub (Ascaron 1905, 2953), Glass is dropped by Forest Spirit (Ascaron 2220, 2638), Cactus is dropped by Killer Cactus (Magical Ocean 884, 3156), Pengiun Pelt is dropped by Sailor Penguin (Deep Blue 994, 365), and Glistening Shrub is from water shrub (Ascaron Sea Region 3000, 2566).")
--	MisResultCondition( HasMission, 1400)
--	MisResultCondition( NoRecord, 1400)
--	MisResultCondition( HasItem, 1604, 10)
--	MisResultCondition( HasItem, 1777, 5)
--	MisResultCondition( HasItem, 1692, 5)
--	MisResultCondition( HasItem, 4334, 5)
--	MisResultCondition( HasItem, 4511, 1)
--	MisResultAction( TakeItem, 1604, 10)
--	MisResultAction( TakeItem, 1777, 5)
--	MisResultAction( TakeItem, 1692, 5)
--	MisResultAction( TakeItem, 4334, 5)
--	MisResultAction( TakeItem, 4511, 1)
--	MisResultAction( SetRecord, 1400)
--	MisResultAction( ClearMission, 1400)
--	MisResultAction( AddMoney, 10000, 10000)
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 1604)	
--	TriggerAction( 1, AddNextFlag, 1400, 10, 10 )
--	RegCurTrigger( 14001 )
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 1777)	
--	TriggerAction( 1, AddNextFlag, 1400, 20, 5 )
--	RegCurTrigger( 14002 )
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 1692)	
--	TriggerAction( 1, AddNextFlag, 1400, 30, 5 )
--	RegCurTrigger( 14003 )
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4334)	
--	TriggerAction( 1, AddNextFlag, 1400, 40, 5 )
--	RegCurTrigger( 14004 )
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4511)	
--	TriggerAction( 1, AddNextFlag, 1400, 50, 1 )
--	RegCurTrigger( 14005 )
--
--
--	----------------------------------------------------------
--	DefineMission ( 6070 , "Golden Chest" , 1401 )
--	MisBeginTalk("<t>Want to obtain Big Lucky chest? If you want one help me kill the following monsters: 30 Bandit, 20 Miner mole, 10 Tribal warrior, 5 Bandit Leader - Adder, 5 Ruby dolphin, 5 Fish bone, 20 Sluggish Squid, and Man Eating Sea Jelly.")
--	MisBeginCondition( NoMission, 1401)
--	MisBeginCondition( NoRecord, 1401)
--	MisBeginAction( AddMission, 1401)
--	MisBeginAction( AddTrigger, 14011, TE_KILL, 93, 30)
--	MisBeginAction( AddTrigger, 14012, TE_KILL, 88, 20)
--	MisBeginAction( AddTrigger, 14013, TE_KILL, 248, 10)
--	MisBeginAction( AddTrigger, 14014, TE_KILL, 211, 5)
--	MisBeginAction( AddTrigger, 14015, TE_KILL, 58, 5)
--	MisBeginAction( AddTrigger, 14016, TE_KILL, 242, 5)
--	MisBeginAction( AddTrigger, 14017, TE_KILL, 578, 20)
--	MisBeginAction( AddTrigger, 14018, TE_KILL, 596, 20)
--	MisCancelAction(ClearMission, 1401)
--
--	MisNeed(MIS_NEED_KILL, 93, 30, 10, 30)
--	MisNeed(MIS_NEED_KILL, 88, 20, 20, 20)
--	MisNeed(MIS_NEED_KILL, 248, 10, 30, 10)
--	MisNeed(MIS_NEED_KILL, 211, 5, 40, 5)
--	MisNeed(MIS_NEED_KILL, 58, 5, 50, 5)
--	MisNeed(MIS_NEED_KILL, 242, 5, 60, 5)
--	MisNeed(MIS_NEED_KILL, 578, 20, 70, 20)
--	MisNeed(MIS_NEED_KILL, 596, 20, 80, 20)
--
--	MisResultTalk("<t>~")
--	MisHelpTalk("<t>The monsters on the sea must be defeated on a ship.")
--	MisResultCondition( HasMission, 1401)
--	MisResultCondition( NoRecord, 1401)
--	MisResultCondition( HasFlag, 1401, 10)
--	MisResultCondition( HasFlag, 1401, 20)
--	MisResultCondition( HasFlag, 1401, 30)
--	MisResultCondition( HasFlag, 1401, 40)
--	MisResultCondition( HasFlag, 1401, 50)
--	MisResultCondition( HasFlag, 1401, 60)
--	MisResultCondition( HasFlag, 1401, 70)
--	MisResultCondition( HasFlag, 1401, 80)
--	MisResultAction( SetRecord, 1401)
--	MisResultAction( ClearMission, 1401)
--	MisResultAction( GiveItem, 4312)
--	MisResultBagNeed(1)
--
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 93 )
--	TriggerAction( 1, AddNextFlag, 1401, 10, 30 )
--	RegCurTrigger( 14011 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 88 )
--	TriggerAction( 1, AddNextFlag, 1401, 20, 20 )
--	RegCurTrigger( 14012 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 248 )
--	TriggerAction( 1, AddNextFlag, 1401, 30, 10 )
--	RegCurTrigger( 14013 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 211 )
--	TriggerAction( 1, AddNextFlag, 1401, 40, 5 )
--	RegCurTrigger( 14014 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 58 )
--	TriggerAction( 1, AddNextFlag, 1401, 50, 5 )
--	RegCurTrigger( 14015 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 242 )
--	TriggerAction( 1, AddNextFlag, 1401, 60, 5 )
--	RegCurTrigger( 14016 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 578 )
--	TriggerAction( 1, AddNextFlag, 1401, 70, 20 )
--	RegCurTrigger( 14017 )
--	InitTrigger()
--	TriggerCondition( 1, IsMonster, 596 )
--	TriggerAction( 1, AddNextFlag, 1401, 80, 30 )
--	RegCurTrigger( 14018 )


---------------------------------------------------------------------
DefineMission( 6071, "New challenge", 1402)
MisBeginTalk("<t>You still need more training. Lately around Argent City the Mystic Shrub has been causing trouble, please go investigate!")
MisBeginCondition( CheckZS )
MisBeginCondition( NoRecord, 1402)
MisBeginCondition( NoMission, 1402)
MisBeginAction( AddMission, 1402)
MisCancelAction( ClearMission ,1402)

MisNeed( MIS_NEED_DESP, "Investigate the Mystic shrub around Argent city area.")
MisHelpTalk("<t>Go around Argent City and see if anything special is happening.")

MisResultCondition( AlwaysFailure )

----------------------------------------------------------------------
DefineMission( 6072, "New challenge", 1402, COMPLETE_SHOW)

MisBeginCondition( AlwaysFailure )

MisResultTalk("<t>Hatred, hatred")
MisResultCondition( HasMission, 1402)
MisResultCondition( NoRecord, 1402)
MisResultAction( AddSailExp , 10, 10)
MisResultAction( SetRecord, 1402)
MisResultAction( ClearMission, 1402)

---------------------------------------------------------------------
DefineMission( 6073, "Mystic Shrub hatred", 1403)

MisBeginTalk("<t>Answer me one question!")
MisBeginCondition( CheckZS )
MisBeginCondition( HasRecord, 1402)
MisBeginCondition( NoRecord, 1403)
MisBeginCondition( NoMission, 1403)
MisBeginAction( AddMission, 1403)
MisCancelAction( ClearMission, 1403)

MisNeed( MIS_NEED_DESP, "Answer hatred shrub question.")

MisResultTalk( "<t>I want to have revenge!")
MisHelpTalk( "<t>Do not escape and answer my question.")
MisResultCondition( HasRecord, 1404)
MisResultCondition( HasMission, 1403)
MisResultAction( SetRecord, 1403)
MisResultAction( ClearMission, 1403)
MisResultAction( AddSailExp, 10, 10)

---------------------------------------------------------------------
DefineMission( 6074, "Hatred Shrub request", 1408)

MisBeginTalk("<t>I will not accept this, so I'll create a quest too, you go and kill people for me!! After you slain them, help me get 20 Blood Contracts!")
MisBeginCondition( NoMission, 1408)
MisBeginCondition( NoRecord, 1408)
MisBeginCondition( HasRecord, 1403)
MisBeginCondition( HasRecord, 1405)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1408)
MisBeginAction( AddTrigger, 14081, TE_GETITEM, 2383, 20 )
MisCancelAction( ClearMission, 1408)

MisNeed( MIS_NEED_ITEM, 2383 , 20, 10, 20)

MisResultTalk( "<t>Haha, feel good after killing people right? Please keep up the good work!")
MisHelpTalk( "<t>Blood contract can be obtained by killing people in Sacred war, haha.")
MisResultCondition( HasMission, 1408)
MisResultCondition( NoRecord, 1408)
MisResultCondition( HasItem, 2383, 20)
MisResultAction( TakeItem ,2383, 20)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( SetRecord, 1408)
MisResultAction( SetRecord, 1409)
MisResultAction( ClearMission, 1408)

InitTrigger()
TriggerCondition( 1, IsItem, 2383)	
TriggerAction( 1, AddNextFlag, 1408, 10, 20 )
RegCurTrigger( 14081 )

-------------------------------------------------------------------
DefineMission( 6075, "Hatred Shrub request", 1410)

MisBeginTalk( "<t>You don't even care about us, hmph, I am too lazy to answer you too! (Looks like he's angry, maybe some water will help him cool down).")
MisBeginCondition( NoRecord, 1410)
MisBeginCondition( NoMission, 1410)
MisBeginCondition( HasRecord, 1406)
MisBeginCondition( HasRecord, 1403)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1410)
MisBeginAction( AddTrigger, 14101, TE_GETITEM, 1649, 10 )
MisCancelAction( ClearMission, 1410)

MisNeed( MIS_NEED_ITEM, 1649 , 10, 10, 10)

MisResultTalk( "<t>Seeing you did so much, I will give you one more chance.")
MisHelpTalk( "Ignore me!")
MisResultCondition( HasMission, 1410)
MisResultCondition( NoRecord, 1410)
MisResultCondition( HasItem, 1649, 10)
MisResultAction( SetRecord, 1410)
MisResultAction( SetRecord, 1405)
MisResultAction( TakeItem, 1649, 10)
MisResultAction( ClearMission, 1410)

InitTrigger()
TriggerCondition( 1, IsItem, 1649)	
TriggerAction( 1, AddNextFlag, 1410, 10, 10 )
RegCurTrigger( 14101 )

-------------------------------------------------------
DefineMission( 6076, "Hatred Shrub request", 1411)
MisBeginTalk( "<t>Grrr, you must have killed a lot of my grasslings too. Give me 1, 000, 000 as compensation or I won't let you off!")
MisBeginCondition( NoRecord, 1411)
MisBeginCondition( NoMission, 1411)
MisBeginCondition( HasRecord, 1403)
MisBeginCondition( HasRecord, 1407)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1411)
MisCancelAction( ClearMission, 1411)

MisNeed( MIS_NEED_DESP, "<t>Give Hatred Shrub 1million as a compensate.")

MisResultTalk( "<t>The money does not cover up my mental pain, but since I'm generous, I'll forgive you this time.")
MisHelpTalk("<t>Hurry up and give me money!")
MisResultCondition( HasMission, 1411)
MisResultCondition( NoRecord, 1411)
MisResultCondition( HasMoney, 1000000)
MisResultAction( TakeMoney, 1000000)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( SetRecord, 1411)
MisResultAction( SetRecord, 1409)
MisResultAction( ClearMission, 1411)

----------------------------------------------------------------
DefineMission( 6077, "Report situation", 1412)
MisBeginTalk( "<t>Since you know whom you're dealing with, I'll forgive you today, but you better be more careful next time!")
MisBeginCondition( NoRecord, 1412)
MisBeginCondition( NoMission, 1412)
MisBeginCondition( HasRecord, 1409)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1412)
MisCancelAction( ClearMission, 1412)

MisNeed( MIS_NEED_DESP, "Go find Castle Guard - Peter and report the information.")
MisHelpTalk( "<t>Hurry up and go away, don't walk back and forth infront of me!")

MisResultCondition( AlwaysFailure )

---------------------------------------------------------------
DefineMission( 6078, "Report situation", 1412, COMPLETE_SHOW)

MisBeginCondition( AlwaysFailure)

MisResultTalk( "<t>What? Mystic Shrub war? This tiny issue shouldn't be our concern, I can crush them all with one hand.")
MisResultCondition( HasMission, 1412)
MisResultCondition( NoRecord, 1412)
MisResultAction( SetRecord, 1412)
MisResultAction( ClearMission, 1412)
MisResultAction( AddSailExp, 5, 5)

----------------------------------------------------------------
DefineMission( 6079, "Gather feathers", 1413)
MisBeginTalk( "<t>Now I'll give you a severe quest: Lately we are running out of feathers to make the rebirth wings, please help me find 100 Snowy White Plume, and 100 Black Feather.")
MisBeginCondition( NoMission, 1413)
MisBeginCondition( NoRecord, 1413)
MisBeginCondition( HasRecord, 1412)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1413)
MisBeginAction( AddTrigger, 14131, TE_GETITEM, 4364, 100)
MisBeginAction( AddTrigger, 14132, TE_GETITEM, 4347, 100)
MisCancelAction( ClearMission, 1413)

MisNeed( MIS_NEED_ITEM, 4364, 100, 10, 100)
MisNeed( MIS_NEED_ITEM, 4347, 100, 20, 100)

MisResultTalk( "<t>Good job!")
MisHelpTalk( "Snowy White Plume is dropped by White Owlie (Ascaron 1360, 2683), and Black Feather is dropped by Owlie (Ascaron 1461, 3018).")
MisResultCondition( HasMission, 1413)
MisResultCondition( NoRecord, 1413)
MisResultCondition( HasItem, 4364, 100)
MisResultCondition( HasItem, 4347, 100)
MisResultAction( TakeItem, 4364, 100)
MisResultAction( TakeItem, 4347, 100)
MisResultAction( AddSailExp, 40, 40)
MisResultAction( SetRecord, 1413)
MisResultAction( ClearMission, 1413)

InitTrigger()
TriggerCondition( 1, IsItem, 4364)	
TriggerAction( 1, AddNextFlag, 1413, 10, 100 )
RegCurTrigger( 14131 )

InitTrigger()
TriggerCondition( 1, IsItem, 4347)	
TriggerAction( 1, AddNextFlag, 1413, 20, 100 )
RegCurTrigger( 14132 )

----------------------------------------------------------------
DefineMission( 6080, "Help other people", 1414)
MisBeginTalk("0")
MisBeginCondition( HasRecord, 1413)
MisBeginCondition( NoRecord, 1414)
MisBeginCondition( NoMission, 1414)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1414)
MisCancelAction( ClearMission, 1414)

MisNeed( MIS_NEED_DESP, "<t>Head inside the city and see if anyone needs help.")

MisResultTalk( "<t>You helped so many people, good for you.")
MisHelpTalk( "<t>There's a lot of people that needs help inside the city, please go help them.")
MisResultCondition( HasMission, 1414)
MisResultCondition( NoRecord, 1414)
MisResultCondition( HasRecord, 1428)
MisResultCondition( HasRecord, 1417)
MisResultCondition( HasRecord, 1420)
MisResultCondition( HasRecord, 1422)
MisResultCondition( HasRecord, 1424)
MisResultAction( AddSailExp, 10,10)
MisResultAction( SetRecord, 1414)
MisResultAction( ClearMission, 1414)

-------------------------------------------------------------
DefineMission( 6081, "Charm of the rose", 1415)
MisBeginTalk("<t>I've been a young boy holding a rose in his mouth, that look is totally handsome ( Her eyes is full of stars), why don't you try it and let me see?")
MisBeginCondition( NoChaType, 3)
MisBeginCondition( NoChaType, 4)
MisBeginCondition( HasMission, 1414)
MisBeginCondition( CheckZS )
MisBeginCondition( NoMission, 1415)
MisBeginCondition( NoRecord, 1415)
MisBeginAction( AddMission, 1415)
MisCancelAction( ClearMission, 1415)

MisNeed( MIS_NEED_DESP, "Barmaid.Donna would like to see you with a Rose in your mouth.")

MisResultTalk( "<t>Not bad, anyhow hit also can get 80 points.")
MisHelpTalk( "<t>aren't roses beautiful?")
MisResultCondition( HasState, 110)
MisResultCondition( HasMission, 1415)
MisResultCondition( NoRecord, 1415)
MisResultAction( SetRecord, 1415)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( SetRecord, 1428)
MisResultAction( ClearMission, 1415)


-------------------------------------------------------------
DefineMission( 6082, "Charm of the rose", 1416)
MisBeginTalk( "<t>I've seen a beautiful women once with a rose holding in her mouth, it was so sexy ( He seems to have something shiny below his lips), why don't you try it and let me see?")
MisBeginCondition( HasMission, 1414)
MisBeginCondition( NoRecord, 1416)
MisBeginCondition( NoMission, 1416)
MisBeginCondition( NoChaType, 1)
MisBeginCondition( NoChaType, 2)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1416)
MisCancelAction( ClearMission, 1416)

MisNeed( MIS_NEED_DESP, "Drunkyard - Anthony wants to see you holding a rose with your mouth.")

MisResultTalk( "<t>Not bad, quite sexy, have to admit you're quite beautiful.")
MisHelpTalk( "<t>aren't roses beautiful?")
MisResultCondition( HasState, 110)
MisResultCondition( HasMission, 1416)
MisResultCondition( NoRecord, 1416)
MisResultAction( SetRecord, 1416)
MisResultAction( SetRecord, 1428)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( ClearMission, 1416)
-------------------------------------------------------------
DefineMission( 6085, "Unlucky Model", 1417)
MisBeginTalk( "<t>Lately I've thought of a new hairstyle, but I don't have enough combs and scissors, please go help me find some!")
MisBeginCondition( NoMission, 1417)
MisBeginCondition( NoRecord, 1417)
MisBeginCondition( HasMission, 1414)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1417)
MisBeginAction( AddTrigger, 14171, TE_GETITEM, 1804, 5)
MisBeginAction( AddTrigger, 14172, TE_GETITEM, 1805, 5)
MisCancelAction( ClearMission, 1417)

MisNeed( MIS_NEED_ITEM, 1804, 5, 10, 5)
MisNeed( MIS_NEED_ITEM, 1805, 5, 20, 5)

MisResultTalk( "<t>Thank you very much, I will design a new hairstyle soon!")
MisResultCondition( HasMission, 1417)
MisResultCondition( HasItem, 1804, 5)
MisResultCondition( HasItem, 1805, 5)
MisResultCondition( NoRecord, 1417)
MisResultAction( TakeItem, 1804, 5)
MisResultAction( TakeItem, 1805, 5)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( SetRecord, 1417)
MisResultAction( ClearMission, 1417)

InitTrigger()
TriggerCondition( 1, IsItem, 1804)	
TriggerAction( 1, AddNextFlag, 1417, 10, 5 )
RegCurTrigger( 14171 )

InitTrigger()
TriggerCondition( 1, IsItem, 1805)	
TriggerAction( 1, AddNextFlag, 1417, 20, 5 )
RegCurTrigger( 14172 )


--------------------------------------------------------------
DefineMission( 6086, "The Dream for Loric Dragon Set", 1418)
MisBeginTalk( "<t>I don't want to be an utility man, I also want to be the main character, please help me talk to Navy Commander - Dessaro about I want to be a navy!")
MisBeginCondition( NoMission, 1418)
MisBeginCondition( NoRecord, 1418)
MisBeginCondition( CheckZS )
MisBeginCondition( HasMission, 1414)
MisBeginAction( AddMission, 1418)
MisCancelAction( ClearMission, 1418)

MisNeed( MIS_NEED_DESP, "Go find Navy Commander - Dessaro and tell him the dream of Passerby - Ja.")

MisResultCondition( AlwaysFailure )

----------------------------------------------------------------
DefineMission( 6087, "The Dream for Loric Dragon Set", 1418, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Passerby - Ja wants to be in the navy? You sure I haven't heard you wrong?")
MisResultCondition( HasMission, 1418)
MisResultCondition( NoRecord, 1418)
MisResultAction( SetRecord, 1418)
MisResultAction( AddSailExp, 5, 5)
MisResultAction( ClearMission, 1418)

-----------------------------------------------------------------
DefineMission( 6088, "The Dream for Loric Dragon Set", 1419)
MisBeginTalk( "<t>Tell Passerby - Ja if he wants to be in the Navy, go defeat 5 Beardy Pirate Support Ship and 5 Beardy Pirate Warship.")
MisBeginCondition( NoMission, 1419)
MisBeginCondition( NoRecord, 1419)
MisBeginCondition( HasMission, 1414)
MisBeginCondition( HasRecord, 1418)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1419)
MisCancelAction( ClearMission, 1419)

MisNeed( MIS_NEED_DESP, "Tell Passerby - Ja the request from Navy Commander - Dessaro.")

MisResultCondition( AlwaysFailure )


------------------------------------------------------------------
DefineMission( 6089, "The Dream for Loric Dragon Set", 1419, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Is this what Dessaro means? Let me think.")
MisResultCondition( HasMission, 1419)
MisResultCondition( NoRecord, 1419)
MisResultAction( AddSailExp, 5, 5)
MisResultAction( SetRecord, 1419)
MisResultAction( ClearMission, 1419)

-----------------------------------------------------------------
DefineMission( 6090, "The Dream for Loric Dragon Set", 1420)
MisBeginTalk( "<t>This is too hard for me, why don't you help until the end and eliminate them!")
MisBeginCondition( NoMission, 1420)
MisBeginCondition( NoRecord, 1420)
MisBeginCondition( HasMission, 1414)
MisBeginCondition( HasRecord, 1419)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1420)
MisBeginAction( AddTrigger, 14201, TE_KILL, 630, 5)
MisBeginAction( AddTrigger, 14202, TE_KILL, 631, 5)
MisCancelAction( ClearMission, 1420)

MisNeed( MIS_NEED_DESP, "kill 5 Beardy Pirate Support Ship and 5 Beardy Pirate Warship.")
MisNeed( MIS_NEED_KILL, 630, 5, 10, 5)
MisNeed( MIS_NEED_KILL, 631, 5, 20, 5)

MisResultTalk( "<t>I have received the Navy recommendation letter, Thank you so much.")
MisResultCondition( HasMission, 1420)
MisResultCondition( NoRecord, 1420)
MisResultCondition( HasFlag, 1420, 14)
MisResultCondition( HasFlag, 1420, 24)
MisResultAction( SetRecord, 1420)
MisResultAction( ClearMission, 1420)
MisResultAction( AddSailExp, 20, 20)

InitTrigger()
TriggerCondition( 1, IsMonster, 630 )
TriggerAction( 1, AddNextFlag, 1420, 10, 5 )
RegCurTrigger( 14201 )

InitTrigger()
TriggerCondition( 1, IsMonster, 631 )
TriggerAction( 1, AddNextFlag, 1420, 20, 5 )
RegCurTrigger( 14202 )


--------------------------------------------------------------------------
DefineMission( 6091, "Express delivery", 1421)
MisBeginTalk( "<t>I have a package here for Banker - Belinda at Icicle city, but I need a person who's fast, can you help me?")
MisBeginCondition( NoMission, 1421)
MisBeginCondition( NoRecord, 1421)
MisBeginCondition( CheckZS )
MisBeginCondition( HasMission, 1414)
MisBeginAction( AddMission, 1421)
MisCancelAction( ClearMission, 1421)

MisNeed( MIS_NEED_DESP, "<t>Wear your speed gear then find Monica, because she needs a person that is very speed for delivery!")

MisResultTalk( "<t>Looks like you can really run fast!")
MisHelpTalk( "<t>You run too slow, go find some equipment that boosts your speed!")
MisResultCondition( HasMission, 1421)
MisResultCondition( NoRecord, 1421)
MisResultCondition( CheckSpeed, 550)
MisResultAction( SetRecord, 1421)
MisResultAction( AddSailExp, 20, 20)
MisResultAction( ClearMission, 1421)

--------------------------------------------------------------------------
DefineMission( 6092, "Express delivery", 1422)
MisBeginTalk( "<t>I believe in your speed, please help me deliever this package to Banker - Belinda at Icicle City.")
MisBeginCondition( NoMission, 1422)
MisBeginCondition( NoRecord, 1422)
MisBeginCondition( HasRecord, 1421)
MisBeginAction( AddMission, 1422)
MisBeginAction( GiveItem, 956, 1, 4)
MisCancelAction( ClearMission, 1422)

MisNeed( MIS_NEED_DESP, "Please send the package to Banker - Belinda at Icicle City.")
MisHelpTalk( "<t>Banker - Belinda is at Icicle city.")

MisResultCondition( AlwaysFailure )

--------------------------------------------------------------------------
DefineMission( 6093, "Express delivery", 1422, COMPLETE)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Wah! The package was so heavy and you delievered it in time, thank you very much!")
MisResultCondition( HasMission, 1422)
MisResultCondition( NoRecord, 1422)
MisResultCondition( HasItem, 956, 1)
MisResultAction( TakeItem, 956, 1)
MisResultAction( SetRecord, 1422)
MisResultAction( ClearMission, 1422)
MisResultAction( AddSailExp, 5, 5)

--------------------------------------------------------------------------
DefineMission( 6094, "opening scroll is good for you", 1423)
MisBeginTalk( "<t>You should read more books. I got a book here, I'll lend it to you. Please help me deliever the book to Tomas Tutu when you're done reading.")
MisBeginCondition( NoRecord, 1423)
MisBeginCondition( NoMission, 1423)
MisBeginCondition( HasMission, 1414)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1423)
MisBeginAction( GiveItem, 957, 1, 4)
MisCancelAction( ClearMission, 1423)

MisNeed( MIS_NEED_DESP, "Send the book to Tomas.Tutu.")
MisHelpTalk( "<t>Remember to look at the book yourself first.")

MisResultCondition( AlwaysFailure )

----------------------------------------------------------------------------
DefineMission( 6095, "opening scroll is good for you", 1423)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>You've seen this book already? I'll take it away then.")
MisResultCondition( HasMission, 1423)
MisResultCondition( NoRecord, 1423)
MisResultCondition( HasItem, 957, 1)
MisResultAction( TakeItem, 957, 1)
MisResultAction( AddSailExp, 5, 5)
MisResultAction( SetRecord, 1423)
MisResultAction( ClearMission, 1423)

----------------------------------------------------------------------------
DefineMission( 6096, "opening scroll is good for you", 1424)
MisBeginTalk( "<t>Since you said you finished reading the book, let me test you with some questions.")
MisBeginCondition( NoMission, 1424)
MisBeginCondition( NoRecord, 1424)
MisBeginCondition( HasRecord, 1423)
MisBeginAction( AddMission, 1424)
MisCancelAction( ClearMission, 1424)

MisNeed( MIS_NEED_DESP, "Answer Tomas Tutu a few questions.")
MisHelpTalk( "<t>These questions are so easy, there shouldn't be any reason why you couldn't answer them?")
	
MisResultTalk( "<t>Read more books is good for you!")
MisResultCondition( HasMission, 1424)
MisResultCondition( NoRecord, 1424)
MisResultCondition( HasRecord, 1429)
MisResultAction( AddSailExp, 5, 5)
MisResultAction( SetRecord, 1424)
MisResultAction( ClearMission, 1424)

-------------------------------------------------------------------------
DefineMission( 6097, "Spy for army information", 1425)
MisBeginTalk( "<t>After my observation, you're a person that can be rely on. Now, I will give you an important mission. We've sent a a group of soldier to the Black Dragon lair to investigate military situation, but they haven't come back since, can you help me look for them?")
MisBeginCondition( NoMission, 1425)
MisBeginCondition( NoRecord, 1425)
MisBeginCondition( HasRecord, 1414)
MisBeginCondition( CheckZS )
MisBeginAction( AddMission, 1425)
MisCancelAction( ClearMission, 1425)

MisNeed( MIS_NEED_DESP, "Head down to Black dragon lair 2 to investigate the military situation.")
MisHelpTalk( "<t>Hurry and go, please becareful!")

MisResultCondition( AlwaysFailure )

-------------------------------------------------------------------------
DefineMission( 6098, "Spy for army information", 1425, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Wegot attacked by massive spawns of baby dragons, and everyonedied")
MisResultCondition( HasMission, 1425)
MisResultCondition( NoRecord, 1425)
MisResultAction( SetRecord, 1425)
MisResultAction( AddSailExp, 10, 10)
MisResultAction( ClearMission, 1425)

--------------------------------------------------------------------------
DefineMission( 6099, "Spy for army information", 1426)
MisBeginCondition( NoMission, 1426)
MisBeginCondition( NoRecord, 1426)
MisBeginCondition( HasRecord, 1425)
MisBeginAction( AddMission, 1426)
MisBeginAction( GiveItem, 958, 1, 4)
MisCancelAction( ClearMission, 1426)

MisNeed( MIS_NEED_DESP, "Help me give this letter of information to Castle Guard - Peter, this letter has saraficed a lot of troops lifes in exchange.")
MisHelpTalk( "<t>I'm counting on you!")

MisResultCondition( AlwaysFailure )

-------------------------------------------------------------------------
DefineMission( 6100, "Spy for army information", 1426, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>What? The whole army got defeated? How is this possible!")
MisResultCondition( HasMission, 1426)
MisResultCondition( NoRecord, 1426)
MisResultCondition( HasItem, 958, 1)
MisResultAction( TakeItem, 958, 1)
MisResultAction( AddSailExp, 10, 10)
MisResultAction( SetRecord, 1426)
MisResultAction( ClearMission, 1426)

----------------------------------------------------------------------
DefineMission( 6101, "Peter's Revenge", 1427)
MisBeginTalk( "<t>I have to kill those winged big lizards, go, go help them revenge. Go and prove your bravery!")
MisBeginCondition( NoMission, 1427)
MisBeginCondition( NoRecord, 1427)
MisBeginCondition( HasRecord, 1426)
MisBeginAction( AddMission, 1427)
MisBeginAction( AddTrigger, 14271, TE_KILL, 791, 1)
MisBeginAction( AddTrigger, 14272, TE_KILL, 793, 1)
MisBeginAction( AddTrigger, 14273, TE_KILL, 794, 1)
MisCancelAction( ClearMission, 1427)

MisNeed( MIS_NEED_KILL, 791, 1, 10, 1)
MisNeed( MIS_NEED_KILL, 793, 1, 30, 1)
MisNeed( MIS_NEED_KILL, 794, 1, 50, 1)

MisResultTalk( "<t>Thank you very much, you've proven yourself to be a true warrior. Now you can go to Shaitan City NPC to take quest to raise your Rebirth Level.")
MisHelpTalk( "<t>Becareful, those big capillas are not easy to handle.")
MisResultCondition( HasMission, 1427)
MisResultCondition( NoRecord, 1427)
MisResultCondition( HasFlag, 1427, 10)
MisResultCondition( HasFlag, 1427, 30)
MisResultCondition( HasFlag, 1427, 50)
MisResultAction( AddSailExp, 50, 50)
MisResultAction( SetRecord, 1427)
MisResultAction( ClearMission, 1427)

InitTrigger()
TriggerCondition( 1, IsMonster, 791 )
TriggerAction( 1, AddNextFlag, 1427, 10, 1 )
RegCurTrigger( 14271 )

InitTrigger()
TriggerCondition( 1, IsMonster, 793 )
TriggerAction( 1, AddNextFlag, 1427, 30, 1 )
RegCurTrigger( 14272 )

InitTrigger()
TriggerCondition( 1, IsMonster, 794 )
TriggerAction( 1, AddNextFlag, 1427, 50, 1 )
RegCurTrigger( 14273 )


--------------------------------------------------------------BOSS
DefineMission( 6102, "Challenge Black Dragon", 1430)
MisBeginTalk( "<t>Black Dragon is located at Black Dragon Lair 2, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1430)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1430)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14301, TE_KILL, 789, 1)
MisCancelAction( ClearMission, 1430)

MisNeed( MIS_NEED_KILL, 789, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1430, 10)
MisResultCondition( HasMission, 1430)
MisResultAction( AddSailExp, 120, 120)
MisResultAction( ClearMission, 1430)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 789 )
TriggerAction( 1, AddNextFlag, 1430, 10, 1 )
RegCurTrigger( 14301 )

--------------------------------------------------------------BOSS
DefineMission( 6103, "Challenge Wandering Soul", 1432)
MisBeginTalk( "<t>Wandering Soul is located at Demonic World Level 1, and is one of the strong bosses in the game, you sure you want to challenge her?")
MisBeginCondition( NoMission, 1432)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1432)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14321, TE_KILL, 679, 1)
MisCancelAction( ClearMission, 1432)

MisNeed( MIS_NEED_KILL, 679, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1432, 10)
MisResultCondition( HasMission, 1432)
MisResultAction( AddSailExp, 50, 50)
MisResultAction( ClearMission, 1432)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 679 )
TriggerAction( 1, AddNextFlag, 1432, 10, 1 )
RegCurTrigger( 14321 )

--------------------------------------------------------------BOSS
DefineMission( 6104, "Challenge Snowman Warlord", 1433)
MisBeginTalk( "<t>Snowman Warlord is located at Demonic World Level 2, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1433)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1433)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14331, TE_KILL, 678, 1)
MisCancelAction( ClearMission, 1433)

MisNeed( MIS_NEED_KILL, 679, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1433, 10)
MisResultCondition( HasMission, 1433)
MisResultAction( AddSailExp, 50, 50)
MisResultAction( ClearMission, 1433)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 678 )
TriggerAction( 1, AddNextFlag, 1433, 10, 1 )
RegCurTrigger( 14331 )

---------------------------------------------------------------BOSS
DefineMission( 6105, "Challenge Snowman Warlord", 1434)
MisBeginTalk( "<t>Barborosa is located at Skeletar Isle, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1434)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1434)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14341, TE_KILL, 805, 1)
MisCancelAction( ClearMission, 1434)

MisNeed( MIS_NEED_KILL, 805, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1434, 10)
MisResultCondition( HasMission, 1434)
MisResultAction( AddSailExp, 75, 75)
MisResultAction( ClearMission, 1434)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 805 )
TriggerAction( 1, AddNextFlag, 1434, 10, 1 )
RegCurTrigger( 14341 )

---------------------------------------------------------------BOSS
DefineMission( 6106, "Challenge Deathsoul Commander", 1435)
MisBeginTalk( "<t>Deathsoul Commander is located at Research Shelter, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1435)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1435)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14351, TE_KILL, 807, 1)
MisCancelAction( ClearMission, 1435)

MisNeed( MIS_NEED_KILL, 807, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1435, 10)
MisResultCondition( HasMission, 1435)
MisResultAction( AddSailExp, 75, 75)
MisResultAction( ClearMission, 1435)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 807 )
TriggerAction( 1, AddNextFlag, 1435, 10, 1 )
RegCurTrigger( 14351 )

---------------------------------------------------------------BOSS
DefineMission( 6107, "Challenge Kraken", 1436)
MisBeginTalk( "<t>Kraken is located at Skeletar Isle, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1436)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1436)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14361, TE_KILL, 795, 1)
MisCancelAction( ClearMission, 1436)

MisNeed( MIS_NEED_KILL, 795, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1436, 10)
MisResultCondition( HasMission, 1436)
MisResultAction( AddSailExp, 75, 75)
MisResultAction( ClearMission, 1436)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 795 )
TriggerAction( 1, AddNextFlag, 1436, 10, 1 )
RegCurTrigger( 14361 )

---------------------------------------------------------------BOSS
DefineMission( 6108, "Challenge Black Jewel", 1437)
MisBeginTalk( "<t>Black Jewel is located at Caribbean Sea, and is one of the strong bosses in the game, you sure you want to challenge?")
MisBeginCondition( NoMission, 1437)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1437)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14371, TE_KILL, 815, 1)
MisCancelAction( ClearMission, 1437)

MisNeed( MIS_NEED_KILL, 815, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1437, 10)
MisResultCondition( HasMission, 1437)
MisResultAction( AddSailExp, 75, 75)
MisResultAction( ClearMission, 1437)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 815 )
TriggerAction( 1, AddNextFlag, 1437, 10, 1 )
RegCurTrigger( 14371 )

--------------------------------------------------------------BOSS
DefineMission( 6109, "Challenge Despair Knight - Saro", 1438)
MisBeginTalk( "<t>Despair Knight - Saro is located at Abaddon Level 5, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1438)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1438)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14381, TE_KILL, 974, 1)
MisCancelAction( ClearMission, 1438)

MisNeed( MIS_NEED_KILL, 974, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1438, 10)
MisResultCondition( HasMission, 1438)
MisResultAction( AddSailExp, 90, 90)
MisResultAction( ClearMission, 1438)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 974 )
TriggerAction( 1, AddNextFlag, 1438, 10, 1 )
RegCurTrigger( 14381 )

--------------------------------------------------------------BOSS
DefineMission( 6110, "Challenge Abyss Mudmonster - Karu", 1439)
MisBeginTalk( "<t>Abyss Mudmonster - Karu is located at Abaddon Level 6, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1439)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1439)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14391, TE_KILL, 975, 1)
MisCancelAction( ClearMission, 1439)

MisNeed( MIS_NEED_KILL, 975, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1439, 10)
MisResultCondition( HasMission, 1439)
MisResultAction( AddSailExp, 90, 90)
MisResultAction( ClearMission, 1439)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 975 )
TriggerAction( 1, AddNextFlag, 1439, 10, 1 )
RegCurTrigger( 14391 )

--------------------------------------------------------------BOSS
DefineMission( 6111, "Challenge Abyss Prisoner - Aruthur", 1440)
MisBeginTalk( "<t>Abyss Prisoner - Aruthur is located at Abaddon Level 7, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1440)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1440)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14401, TE_KILL, 976, 1)
MisCancelAction( ClearMission, 1440)

MisNeed( MIS_NEED_KILL, 976, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1440, 10)
MisResultCondition( HasMission, 1440)
MisResultAction( AddSailExp, 90, 90)
MisResultAction( ClearMission, 1440)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 976 )
TriggerAction( 1, AddNextFlag, 1440, 10, 1 )
RegCurTrigger( 14401 )

--------------------------------------------------------------BOSS
DefineMission( 6112, "Challenge Abyss Demon - Sacrois", 1441)
MisBeginTalk( "<t>Abyss Demon - Sacrois is located at Abaddon Level 8, and is one of the strong bosses in the game, you sure you want to challenge him?")
MisBeginCondition( NoMission, 1441)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1441)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14411, TE_KILL, 977, 1)
MisCancelAction( ClearMission, 1441)

MisNeed( MIS_NEED_KILL, 977, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1441, 10)
MisResultCondition( HasMission, 1441)
MisResultAction( AddSailExp, 90, 90)
MisResultAction( ClearMission, 1441)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 977 )
TriggerAction( 1, AddNextFlag, 1441, 10, 1 )
RegCurTrigger( 14411 )

--------------------------------------------------------------BOSS
DefineMission( 6113, "Challenge Abyss Beast - Kuroo", 1442)
MisBeginTalk( "0")
MisBeginCondition( NoMission, 1442)
MisBeginCondition( NoRecord, 1431)
MisBeginCondition( HasRecord, 1427)
MisBeginAction(	AddMission, 1442)
MisBeginAction( SetRecord, 1431)
MisBeginAction( AddTrigger, 14421, TE_KILL, 978, 1)
MisCancelAction( ClearMission, 1442)

MisNeed( MIS_NEED_KILL, 978, 1, 10, 1)

MisResultTalk( "<t>Every battle will make us stronger.")
MisHelpTalk( "<t>please becareful.")
MisResultCondition( HasFlag, 1442, 10)
MisResultCondition( HasMission, 1442)
MisResultAction( AddSailExp, 90, 90)
MisResultAction( ClearMission, 1442)
MisResultAction( ClearRecord, 1431)

InitTrigger()
TriggerCondition( 1, IsMonster, 978 )
TriggerAction( 1, AddNextFlag, 1442, 10, 1 )
RegCurTrigger( 14412 )

-----------------------------------------------------------	
DefineMission (5608, "Blood Shed Taurus Hero of Chaos", 1114)

MisBeginTalk("<t>Taurus always have represent luck, why don't you go to the Chaos arena to try your luck.")

MisBeginCondition(NoMission,1114)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(NoRecord,1114)
MisBeginAction(AddMission,1114)
MisCancelAction(ClearMission, 1114)

MisNeed(MIS_NEED_DESP,"Obtained 30 Chaos point.")
MisHelpTalk("<t>30 chao points, not hard!")
MisResultTalk("<t>You came back pretty fast, but don't let you guard down. Your luck isn't so bad.")

MisResultCondition(HasMission, 1114)
MisResultCondition(NoRecord,1114)
MisResultCondition(HasFightingPoint,30 )
MisResultAction(TakeFightingPoint, 30 )
MisResultAction(ClearMission, 1114)
MisResultAction(SetRecord,  1114 )
MisResultAction(GiveItem, 3028, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5609, "Bloody Taurus - Reputed Hero", 1115)

MisBeginTalk("<t>To pass Taurus Palace, raising your reputation is very important.")

MisBeginCondition(NoMission,1115)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(NoRecord,1115)
MisBeginAction(AddMission,1115)
MisCancelAction(ClearMission, 1115)

MisNeed(MIS_NEED_DESP,"Gained 1000 reputation points.")
MisHelpTalk("<t>Other people say my name is very poetic, what do you think?")
MisResultTalk("<t>I bet you'll come back for this Reputation Emblem, I can tell you're a person who won't give up so easily.")

MisResultCondition(HasMission, 1115)
MisResultCondition(NoRecord,1115)
MisResultCondition(HasCredit,1000 )
MisResultAction(TakeCredit, 1000 )
MisResultAction(ClearMission, 1115)
MisResultAction(SetRecord,  1115 )
MisResultAction(GiveItem, 3029, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5610, "Blood Shed Taurus Level Hero", 1116)

MisBeginTalk("All the warriors participating for Taurus Palace must be above level 55. Although you give off a special aura, I cannot give you an exception.")

MisBeginCondition(NoMission,1116)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(NoRecord,1116)
MisBeginAction(AddMission,1116)
MisCancelAction(ClearMission, 1116)

MisNeed(MIS_NEED_DESP,"Level reached 55")
MisHelpTalk("<t>Level 55 could be hard for some people.")
MisResultTalk("<t>Now I gift this emblem to you. You're worthy of this emblem.")

MisResultCondition(HasMission, 1116)
MisResultCondition(NoRecord,1116)
MisResultCondition(LvCheck, ">", 54 )
MisResultAction(ClearMission, 1116)
MisResultAction(SetRecord,  1116 )
MisResultAction(GiveItem, 3030, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5611, "Blood Shed Taurus Hero of Honor", 1117)

MisBeginTalk("<t>300 honor points is the minimum I require for heros challenging Taurus Palace. A talented warrior like you should also know the importance on honor points.")

MisBeginCondition(NoMission,1117)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(NoRecord,1117)
MisBeginAction(AddMission,1117)
MisCancelAction(ClearMission, 1117)

MisNeed(MIS_NEED_DESP,"Obtained 300 honor points.")
MisHelpTalk("<t>Remember you must finish the quest to get your present.")
MisResultTalk("<t>Your type usually gets very popular here at the bar, handsome looking and bravery. Looks like you're not far from Gemini Palace.")

MisResultCondition(HasMission, 1117)
MisResultCondition(NoRecord,1117)
MisResultCondition(HasHonorPoint,300 )
MisResultAction(TakeHonorPoint, 300 )
MisResultAction(ClearMission, 1117)
MisResultAction(SetRecord,  1117 )
MisResultAction(GiveItem, 3031, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5612, "Blood Shed Taurus Gathering ambassador", 1118)

MisBeginTalk("<t>Gathering can test a warrior's patience. Patience is very important for success you know. If you're willing to work hard and not slack, I'll keep that gathering emblem for you.")

MisBeginCondition(NoMission,1118)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(NoRecord,1118)
MisBeginAction(AddMission,1118)
MisBeginAction(AddTrigger, 11181, TE_GETITEM, 4804, 10 )---------------10
MisBeginAction(AddTrigger, 11182, TE_GETITEM, 4720, 10 )---------------10
MisBeginAction(AddTrigger, 11183, TE_GETITEM, 3129, 10 )-------------10
MisBeginAction(AddTrigger, 11184, TE_GETITEM, 2588, 5 )--------------5
MisBeginAction(AddTrigger, 11185, TE_GETITEM, 4494, 1 )--------------1
MisBeginAction(AddTrigger, 11186, TE_GETITEM, 1682, 50 )-------------50
MisBeginAction(AddTrigger, 11187, TE_GETITEM, 1138, 10 )--------------LV1  10
MisBeginAction(AddTrigger, 11188, TE_GETITEM, 0227, 1 )-------------1
MisCancelAction(ClearMission, 1118)


MisNeed(MIS_NEED_ITEM, 4804, 10, 1, 10 )
MisNeed(MIS_NEED_ITEM, 4720, 10, 11, 10 )
MisNeed(MIS_NEED_ITEM, 3129, 10, 21, 10 )
MisNeed(MIS_NEED_ITEM, 2588, 5, 31, 5 )
MisNeed(MIS_NEED_ITEM, 4494, 1, 36, 1 )
MisNeed(MIS_NEED_ITEM, 1682, 50, 37, 50 )
MisNeed(MIS_NEED_ITEM, 1138, 10, 87, 10 )
MisNeed(MIS_NEED_ITEM, 0227, 1, 97, 1 )

MisHelpTalk("<t>those are commonly seen things, shouldn't be hard for you.")
MisResultTalk("<t>This world exists a lot of people with weird hobbies, I don't really know the purpose of me wanting Canine Tooth and Crab Pincer.")

MisResultCondition(HasMission, 1118)
MisResultCondition(NoRecord,1118)
MisResultCondition(HasItem, 4804, 10 )
MisResultCondition(HasItem, 4720, 10 )
MisResultCondition(HasItem, 3129, 10 )
MisResultCondition(HasItem, 2588, 5 )
MisResultCondition(HasItem, 4494, 1 )
MisResultCondition(HasItem, 1682, 50 )
MisResultCondition(HasItem, 1138, 10 )
MisResultCondition(HasItem, 0227, 1 )

MisResultAction(TakeItem, 4804, 10 )
MisResultAction(TakeItem, 4720, 10 )
MisResultAction(TakeItem, 3129, 10 )
MisResultAction(TakeItem, 2588, 5 )
MisResultAction(TakeItem, 4494, 1 )
MisResultAction(TakeItem, 1682, 50 )
MisResultAction(TakeItem, 1138, 10 )
MisResultAction(TakeItem, 0227, 1 )
MisResultAction(ClearMission, 1118)
MisResultAction(SetRecord,  1118 )
MisResultAction(GiveItem, 3032, 1, 4)
MisResultAction(GiveItem, 3036, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 4804)	
TriggerAction( 1, AddNextFlag, 1118, 1, 10 )
RegCurTrigger( 11181 )

InitTrigger()
TriggerCondition( 1, IsItem, 4720)	
TriggerAction( 1, AddNextFlag, 1118, 11, 10 )
RegCurTrigger( 11182 )

InitTrigger()
TriggerCondition( 1, IsItem, 3129)	
TriggerAction( 1, AddNextFlag, 1118, 21, 10 )
RegCurTrigger( 11183 )

InitTrigger()
TriggerCondition( 1, IsItem, 2588)	
TriggerAction( 1, AddNextFlag, 1118, 31, 5 )
RegCurTrigger( 11184 )

InitTrigger()
TriggerCondition( 1, IsItem, 4494)	
TriggerAction( 1, AddNextFlag, 1118, 36, 1 )
RegCurTrigger( 11185 )

InitTrigger()
TriggerCondition( 1, IsItem, 1682)	
TriggerAction( 1, AddNextFlag, 1118, 37, 50 )
RegCurTrigger( 11186 )

InitTrigger()
TriggerCondition( 1, IsItem, 1138)	
TriggerAction( 1, AddNextFlag, 1118, 87, 10 )
RegCurTrigger( 11187 )

InitTrigger()
TriggerCondition( 1, IsItem, 0227)	
TriggerAction( 1, AddNextFlag, 1118, 97, 1 )
RegCurTrigger( 11188 )


--------------------------------------------------------------------
DefineMission( 5613, "Bloody Taurus - Taurus Doomsday", 1119 )
MisBeginTalk("<t>Seems like every item that lure people has something to do with beasts. Boss Emblem is a good example for that.")
			
MisBeginCondition(NoMission, 1119)
MisBeginCondition(HasRecord,1152)
MisBeginCondition(NoRecord,1119)
MisBeginAction(AddMission,1119)
MisBeginAction(AddTrigger, 11191, TE_KILL, 1038, 1)---

MisCancelAction(ClearMission, 1119)

MisNeed(MIS_NEED_DESP,"Slain Taurus Protector (2436, 2405)!")
MisNeed(MIS_NEED_KILL, 1038,1, 10, 1)


MisResultTalk("<t>All scary and haunted adventures is a very good treasure of your memory. When you gain more of this treasure, you'll be seeing a world that people will never be able to see.")
MisHelpTalk("<t>That scary guy owns a cute beer belly, but this doesn't mean you can let your guard down.")
MisResultCondition(HasMission,  1119)
MisResultCondition(HasFlag, 1119, 10)
MisResultCondition(NoRecord , 1119)
MisResultAction(GiveItem, 3034, 1, 4 )
MisResultAction(ClearMission,  1119)
MisResultAction(SetRecord,  1119 )
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 1038)	
TriggerAction( 1, AddNextFlag, 1119, 10, 1 )
RegCurTrigger( 11191 )






-----------------------------------------------------------	
DefineMission (5614, "Blood Shed Taurus Hero of Chaos", 1120)

MisBeginTalk("<t>Taurus always have represent luck, why don't you go to the Chaos arena to try your luck.")

MisBeginCondition(NoMission,1120)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(NoRecord,1120)
MisBeginAction(AddMission,1120)
MisCancelAction(ClearMission, 1120)

MisNeed(MIS_NEED_DESP,"Obtained 60 Chaos points.")
MisHelpTalk("<t>60 chao points is not that hard!")
MisResultTalk("<t>You came back pretty fast, but don't let you guard down. Your luck isn't so bad.")

MisResultCondition(HasMission, 1120)
MisResultCondition(NoRecord,1120)
MisResultCondition(HasFightingPoint,60 )
MisResultAction(TakeFightingPoint, 60 )
MisResultAction(ClearMission, 1120)
MisResultAction(SetRecord,  1120 )
MisResultAction(GiveItem, 3028, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5615, "Bloody Taurus - Reputed Hero", 1121)

MisBeginTalk("<t>To pass Taurus Palace, raising reputation is very important.")

MisBeginCondition(NoMission,1121)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(NoRecord,1121)
MisBeginAction(AddMission,1121)
MisCancelAction(ClearMission, 1121)

MisNeed(MIS_NEED_DESP,"Obtain 3000 points of reputation.")
MisHelpTalk("<t>Other people say my name is very poetic, what do you think?")
MisResultTalk("<t>I bet you'll come back for this Reputation Emblem, I can tell you're a person who won't give up so easily.")

MisResultCondition(HasMission, 1121)
MisResultCondition(NoRecord,1121)
MisResultCondition(HasCredit,3000 )
MisResultAction(TakeCredit,3000  )
MisResultAction(ClearMission, 1121)
MisResultAction(SetRecord,  1121 )
MisResultAction(GiveItem, 3029, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5616, "Blood Shed Taurus Level Hero", 1122)

MisBeginTalk("All the warriors participating for Taurus Palace must be above level 60. Although you give off a special aura, I cannot give you an exception.")

MisBeginCondition(NoMission,1122)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(NoRecord,1122)
MisBeginAction(AddMission,1122)
MisCancelAction(ClearMission, 1122)

MisNeed(MIS_NEED_DESP,"Level reached 60")
MisHelpTalk("<t>Level 60 to other players may be a bit hard")
MisResultTalk("<t>Now I gift this emblem to you. You're worthy of this emblem.")

MisResultCondition(HasMission, 1122)
MisResultCondition(NoRecord,1122)
MisResultCondition(LvCheck, ">", 59 )
MisResultAction(ClearMission, 1122)
MisResultAction(SetRecord,  1122 )
MisResultAction(GiveItem, 3030, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5617, "Blood Shed Taurus Hero of Honor", 1123)

MisBeginTalk("<t>500 Honor points is the minimum requirement for a hero challenging Taurus Palace in my rules. I think a skilled warrior like you would look heavily on your honor points too.")

MisBeginCondition(NoMission,1123)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(NoRecord,1123)
MisBeginAction(AddMission,1123)
MisCancelAction(ClearMission, 1123)

MisNeed(MIS_NEED_DESP,"Gained 500 honor points.")
MisHelpTalk("<t>Remember you must finish the quest to get your present.")
MisResultTalk("<t>Your type usually gets very popular here at the bar, handsome looking and bravery. Looks like you're not far from Gemini Palace.")

MisResultCondition(HasMission, 1123)
MisResultCondition(NoRecord,1123)
MisResultCondition(HasHonorPoint,500 )
MisResultAction(TakeHonorPoint, 500 )
MisResultAction(ClearMission, 1123)
MisResultAction(SetRecord,  1123 )
MisResultAction(GiveItem, 3031, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5618, "Blood Shed Taurus Gathering ambassador", 1124)

MisBeginTalk("<t>Gathering can test a warrior patience. Patience is very important to succeeding you know. If you're willing to work hard and not slack, I'll keep that gathering emblem for you.")

MisBeginCondition(NoMission,1124)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(NoRecord,1124)
MisBeginAction(AddMission,1124)
MisBeginAction(AddTrigger, 11241, TE_GETITEM, 4804, 15 )-------------------15    
MisBeginAction(AddTrigger, 11242, TE_GETITEM, 4720, 15 )-------------------15          
MisBeginAction(AddTrigger, 11243, TE_GETITEM, 3129, 15 )-----------------15               
MisBeginAction(AddTrigger, 11244, TE_GETITEM, 2588, 10 )----------------10               
MisBeginAction(AddTrigger, 11245, TE_GETITEM, 4494, 1 )------------------1            
MisBeginAction(AddTrigger, 11246, TE_GETITEM, 1682, 40 )-----------------40                  
MisBeginAction(AddTrigger, 11247, TE_GETITEM, 1138, 20 )------------------LV1  20 
MisBeginAction(AddTrigger, 11248, TE_GETITEM, 0227, 3 )---------------3          
MisCancelAction(ClearMission, 1124)


MisNeed(MIS_NEED_ITEM, 4804, 15, 1, 15 )
MisNeed(MIS_NEED_ITEM, 4720, 15, 16, 15 )
MisNeed(MIS_NEED_ITEM, 3129, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 2588, 10, 46, 10 )
MisNeed(MIS_NEED_ITEM, 4494, 1, 56, 1 )
MisNeed(MIS_NEED_ITEM, 1682, 40, 57, 40 )
MisNeed(MIS_NEED_ITEM, 1138, 20, 97, 20 )
MisNeed(MIS_NEED_ITEM, 0227, 3, 117, 3 )

MisHelpTalk("<t>those are commonly seen things, shouldn't be hard for you.")
MisResultTalk("<t>This world exists a lot of people with weird hobbies, I don't really know the purpose of me wanting Canine Tooth and Crab Pincer.")

MisResultCondition(HasMission, 1124)
MisResultCondition(NoRecord,1124)
MisResultCondition(HasItem, 4804, 15 )
MisResultCondition(HasItem, 4720, 15 )
MisResultCondition(HasItem, 3129, 15 )
MisResultCondition(HasItem, 2588, 10 )
MisResultCondition(HasItem, 4494, 1 )
MisResultCondition(HasItem, 1682, 40 )
MisResultCondition(HasItem, 1138, 20 )
MisResultCondition(HasItem, 0227, 3 )

MisResultAction(TakeItem, 4804, 15 )
MisResultAction(TakeItem, 4720, 15 )
MisResultAction(TakeItem, 3129, 15 )
MisResultAction(TakeItem, 2588, 10 )
MisResultAction(TakeItem, 4494, 1 )
MisResultAction(TakeItem, 1682, 40 )
MisResultAction(TakeItem, 1138, 20 )
MisResultAction(TakeItem, 0227, 3 )
MisResultAction(ClearMission, 1124)
MisResultAction(SetRecord,  1124 )
MisResultAction(GiveItem, 3032, 1, 4)
MisResultAction(GiveItem, 3036, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 4804)	
TriggerAction( 1, AddNextFlag, 1124, 1, 15 )
RegCurTrigger( 11241 )

InitTrigger()
TriggerCondition( 1, IsItem, 4720)	
TriggerAction( 1, AddNextFlag, 1124, 16, 15 )
RegCurTrigger( 11242 )

InitTrigger()
TriggerCondition( 1, IsItem, 3129)	
TriggerAction( 1, AddNextFlag, 1124, 31, 15 )
RegCurTrigger( 11243 )

InitTrigger()
TriggerCondition( 1, IsItem, 2588)	
TriggerAction( 1, AddNextFlag, 1124, 46, 10 )
RegCurTrigger( 11244 )

InitTrigger()
TriggerCondition( 1, IsItem, 4494)	
TriggerAction( 1, AddNextFlag, 1124, 56, 1 )
RegCurTrigger( 11245 )

InitTrigger()
TriggerCondition( 1, IsItem, 1682)	
TriggerAction( 1, AddNextFlag, 1124, 57, 40 )
RegCurTrigger( 11246 )

InitTrigger()
TriggerCondition( 1, IsItem, 1138)	
TriggerAction( 1, AddNextFlag, 1124, 97, 20 )
RegCurTrigger( 11247 )

InitTrigger()
TriggerCondition( 1, IsItem, 0227)	
TriggerAction( 1, AddNextFlag, 1124, 117, 3 )
RegCurTrigger( 11248 )





-----------------------------------------------------------	
DefineMission (5619, "Blood Shed Taurus Hero of Chaos", 1125)

MisBeginTalk("<t>, ")

MisBeginCondition(NoMission,1125)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(NoRecord,1125)
MisBeginAction(AddMission,1125)
MisCancelAction(ClearMission, 1125)

MisNeed(MIS_NEED_DESP,"Obtained 100 chaos points.")
MisHelpTalk("<t>100 chao points, not hard!")
MisResultTalk("<t>You came back pretty fast, but don't let you guard down. Your luck isn't so bad.")

MisResultCondition(HasMission, 1125)
MisResultCondition(NoRecord,1125)
MisResultCondition(HasFightingPoint,100 )
MisResultAction(TakeFightingPoint, 100 )
MisResultAction(ClearMission, 1125)
MisResultAction(SetRecord,  1125 )
MisResultAction(GiveItem, 3028, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5620, "Bloody Taurus - Reputed Hero", 1126)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1126)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(NoRecord,1126)
MisBeginAction(AddMission,1126)
MisCancelAction(ClearMission, 1126)

MisNeed(MIS_NEED_DESP,"Obtained 5000 points of reputation.")
MisHelpTalk("<t>Other people say my name is very poetic, what do you think?")
MisResultTalk("<t>I bet you'll come back for this Reputation Emblem, I can tell you're a person who won't give up so easily.")

MisResultCondition(HasMission, 1126)
MisResultCondition(NoRecord,1126)
MisResultCondition(HasCredit,5000 )
MisResultAction(TakeCredit, 5000 )
MisResultAction(ClearMission, 1126)
MisResultAction(SetRecord,  1126 )
MisResultAction(GiveItem, 3029, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5621, "Blood Shed Taurus Level Hero", 1127)

MisBeginTalk("All the warriors participating for Taurus Palace must be above level 65. Although you give off a special aura, I cannot give you an exception.")

MisBeginCondition(NoMission,1127)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(NoRecord,1127)
MisBeginAction(AddMission,1127)
MisCancelAction(ClearMission, 1127)

MisNeed(MIS_NEED_DESP,"Reached Level 65")
MisHelpTalk("<t>Level 65 may be a bit hard for someone else")
MisResultTalk("<t>Now I gift this emblem to you. You're worthy of this emblem.")

MisResultCondition(HasMission, 1127)
MisResultCondition(NoRecord,1127)
MisResultCondition(LvCheck, ">", 64 )
MisResultAction(ClearMission, 1127)
MisResultAction(SetRecord,  1127 )
MisResultAction(GiveItem, 3030, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5622, "Blood Shed Taurus Hero of Honor", 1128)

MisBeginTalk("<t>800 honor points is the lowest I'll accept for a hero challenging the Taurus Palace. For a talented warrior like you, honor points should be very important to you.")

MisBeginCondition(NoMission,1128)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(NoRecord,1128)
MisBeginAction(AddMission,1128)
MisCancelAction(ClearMission, 1128)

MisNeed(MIS_NEED_DESP,"Gained 800 honor points.")
MisHelpTalk("<t>Remember you must finish the quest to get your present.")
MisResultTalk("<t>Your type usually gets very popular here at the bar, handsome looking and bravery. Looks like you're not far from Gemini Palace.")

MisResultCondition(HasMission, 1128)
MisResultCondition(NoRecord,1128)
MisResultCondition(HasHonorPoint,800 )
MisResultAction(TakeHonorPoint, 800 )
MisResultAction(ClearMission, 1128)
MisResultAction(SetRecord,  1128 )
MisResultAction(GiveItem, 3031, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5623, "Blood Shed Taurus Gathering ambassador", 1129)

MisBeginTalk("<t>Gathering can test a warrior patience. Patience is very important to succeeding you know. If you're willing to work hard and not slack, I'll keep that gathering emblem for you.")

MisBeginCondition(NoMission,1129)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(NoRecord,1129)
MisBeginAction(AddMission,1129)
MisBeginAction(AddTrigger, 11291, TE_GETITEM, 4804, 10 )---------------------10    
MisBeginAction(AddTrigger, 11292, TE_GETITEM, 4720, 10 )---------------------10          
MisBeginAction(AddTrigger, 11293, TE_GETITEM, 3129, 10 )-------------------10               
MisBeginAction(AddTrigger, 11294, TE_GETITEM, 2588, 15 )------------------15             
MisBeginAction(AddTrigger, 11295, TE_GETITEM, 4494, 5 )--------------------5             
MisBeginAction(AddTrigger, 11296, TE_GETITEM, 1682, 40 )-------------------40                  
MisBeginAction(AddTrigger, 11297, TE_GETITEM, 1138, 25 )--------------------LV1  25 
MisBeginAction(AddTrigger, 11298, TE_GETITEM, 0227, 5 )----------------------5            
MisCancelAction(ClearMission, 1129)						                                     


MisNeed(MIS_NEED_ITEM, 4804, 10, 1, 10 )
MisNeed(MIS_NEED_ITEM, 4720, 10, 11, 10 )
MisNeed(MIS_NEED_ITEM, 3129, 10, 21, 10 )
MisNeed(MIS_NEED_ITEM, 2588, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 4494, 5, 46, 5)
MisNeed(MIS_NEED_ITEM, 1682, 40, 51, 40 )
MisNeed(MIS_NEED_ITEM, 1138, 25, 91, 25 )
MisNeed(MIS_NEED_ITEM, 0227, 5, 116, 5 )

MisHelpTalk("<t>those are commonly seen things, shouldn't be hard for you.")
MisResultTalk("<t>This world exists a lot of people with weird hobbies, I don't really know the purpose of me wanting Canine Tooth and Crab Pincer.")

MisResultCondition(HasMission, 1129)
MisResultCondition(NoRecord,1129)
MisResultCondition(HasItem, 4804, 10 )
MisResultCondition(HasItem, 4720, 10 )
MisResultCondition(HasItem, 3129, 10 )
MisResultCondition(HasItem, 2588, 15 )
MisResultCondition(HasItem, 4494,  5 )
MisResultCondition(HasItem, 1682, 40 )
MisResultCondition(HasItem, 1138, 25 )
MisResultCondition(HasItem, 0227, 5 )

MisResultAction(TakeItem, 4804, 10 )
MisResultAction(TakeItem, 4720, 10 )
MisResultAction(TakeItem, 3129, 10 )
MisResultAction(TakeItem, 2588, 15 )
MisResultAction(TakeItem, 4494, 5 )
MisResultAction(TakeItem, 1682, 40 )
MisResultAction(TakeItem, 1138, 25 )
MisResultAction(TakeItem, 0227, 5 )
MisResultAction(ClearMission, 1129)
MisResultAction(SetRecord,  1129 )
MisResultAction(GiveItem, 3032, 1, 4)
MisResultAction(GiveItem, 3036, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 4804)	
TriggerAction( 1, AddNextFlag, 1129, 1, 10 )
RegCurTrigger( 11291 )

InitTrigger()
TriggerCondition( 1, IsItem, 4720)	
TriggerAction( 1, AddNextFlag, 1129, 11, 10 )
RegCurTrigger( 11292 )

InitTrigger()
TriggerCondition( 1, IsItem, 3129)	
TriggerAction( 1, AddNextFlag, 1129, 21, 10 )
RegCurTrigger( 11293 )

InitTrigger()
TriggerCondition( 1, IsItem, 2588)	
TriggerAction( 1, AddNextFlag, 1129, 31, 15 )
RegCurTrigger( 11294 )

InitTrigger()
TriggerCondition( 1, IsItem, 4494)	
TriggerAction( 1, AddNextFlag, 1129, 46, 5 )
RegCurTrigger( 11295 )

InitTrigger()
TriggerCondition( 1, IsItem, 1682)	
TriggerAction( 1, AddNextFlag, 1129, 51, 40 )
RegCurTrigger( 11296 )

InitTrigger()
TriggerCondition( 1, IsItem, 1138)	
TriggerAction( 1, AddNextFlag, 1129, 91, 25 )
RegCurTrigger( 11297 )

InitTrigger()
TriggerCondition( 1, IsItem, 0227)	
TriggerAction( 1, AddNextFlag, 1129, 116, 5 )
RegCurTrigger( 11298 )




--------------------------------------------------------------------
DefineMission( 5624, "Blood Shed Taurus Public Relations Ambassador", 1130 )
MisBeginTalk("<t>The bar isn't doing too well lately, we need to advertise but we don't have the money. Can you please help us advertise a bit while making new friends outside town?")
			
MisBeginCondition(NoMission, 1130)
MisBeginCondition(NoRecord,1130)
MisBeginCondition(HasRecord, 1152)-------------------id,id
MisBeginAction(AddMission,1130)
MisCancelAction(ClearMission, 1130)

MisNeed(MIS_NEED_DESP,"Talk to Sa Mori (628, 2095) in Region Ascaron.")

MisHelpTalk("<t>When she was young, she's our frequent customer.")
MisResultCondition(AlwaysFailure)	
----------------------------------------------------------

DefineMission(5625, "TOP Ambassador", 1130, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Bar? You've found the right person. Look at my body shape and you can tell I can drink a lot, I'm very proud of this.")
MisResultCondition(NoRecord, 1130)
MisResultCondition(HasMission,1130)
MisResultAction(ClearMission,1130)
MisResultAction(SetRecord, 1130)

----------------------------------------------------------2----------
DefineMission( 5626, "TOP Ambassador 2", 1131 )
MisBeginTalk("<t>Thank you for you help. I'm not sure if you'll be passing by Shaitan, but I'm very worried about my friend Ada, she's always staying in a place full of beasts and pray for them to turn good. She wants the beast to eat vegetable not meat, hope she'll succeed.")
			
MisBeginCondition(NoMission, 1131)
MisBeginCondition(NoRecord,1131)
MisBeginCondition(HasRecord, 1130)
MisBeginAction(AddMission,1131)
MisCancelAction(ClearMission, 1131)

MisNeed(MIS_NEED_DESP,"Find Shaitan's Holy Priestess Ada(862, 3303) for a chat.")

MisHelpTalk("<t>Holy people are just too na?ve, how can they compare to us drunker freedom?...")
MisResultCondition(AlwaysFailure)	
-----------------------------------------2-------------
DefineMission(5627, "TOP Ambassador 2", 1131, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Hi! May Kara bless you! May the world best things be blessed upon you. May happiness be always around you..")
MisResultCondition(NoRecord, 1131)
MisResultCondition(HasMission,1131)
MisResultAction(ClearMission,1131)
MisResultAction(SetRecord, 1131)

----------------------------------------------------------3----------
DefineMission( 5628, "TOP Ambassador 3", 1132 )
MisBeginTalk("<t>All thanks to the Goddess's blessing that allows me to live in harmony with these wild animals. I'm not a expert on drinking, why don't you ask Tae.")
			
MisBeginCondition(NoMission, 1132)
MisBeginCondition(NoRecord,1132)
MisBeginCondition(HasRecord, 1131)
MisBeginAction(AddMission,1132)
MisCancelAction(ClearMission, 1132)

MisNeed(MIS_NEED_DESP,"Head to Skeleton Haven and talk to Tae (2128, 540).")

MisHelpTalk("<t>His Dreams is full of rich poetic ideas, but it'll be hard to archieve.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------3--------
DefineMission(5629, "TOP Ambassador 3", 1132, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>The dream where I want to change into a sea creature has no chance to come true at all, is kind of regretful. I'm a fish, the air in the water.. Swimming back and forth..")
MisResultCondition(NoRecord, 1132)
MisResultCondition(HasMission,1132)
MisResultAction(ClearMission,1132)
MisResultAction(SetRecord, 1132)



----------------------------------------------------------4----------
DefineMission( 5630, "TOP Ambassador 4", 1133 )
MisBeginTalk("<t>Use wine to get rid of my problems? NO! This does not fit my sunshine boy image. If Granny - Beldi knows about this, she'll no longer send me any cakes. For that tasty cake, I hold my grudge.")
			
MisBeginCondition(NoMission, 1133)
MisBeginCondition(NoRecord,1133)
MisBeginCondition(HasRecord, 1132)
MisBeginAction(AddMission,1133)
MisCancelAction(ClearMission, 1133)

MisNeed(MIS_NEED_DESP,"Help Tae to visit Granny - Beldi (2277, 2769).")

MisHelpTalk("<t>Talking about cakes makes me want to drool..")
MisResultCondition(AlwaysFailure)	
-----------------------------------------4--------
DefineMission(5631, "TOP Ambassador 4", 1133, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Really? Tae thought of me again? I think he misses the cakes I bake more, hoho.")
MisResultCondition(NoRecord, 1133)
MisResultCondition(HasMission,1133)
MisResultAction(ClearMission,1133)
MisResultAction(SetRecord, 1133)


----------------------------------------------------------5----------
DefineMission( 5632, "TOP Ambassador 5", 1134 )
MisBeginTalk("<t>I'll hurry and bake some cakes for Tae, but when it comes to bar, is not my age anymore. Thank you for asking though, maybe Trader - Yuka would be interested.")
			
MisBeginCondition(NoMission, 1134)
MisBeginCondition(NoRecord,1134)
MisBeginCondition(HasRecord, 1133)
MisBeginAction(AddMission,1134)
MisCancelAction(ClearMission, 1134)

MisNeed(MIS_NEED_DESP,"Talk to Trader - Yuka (2519, 2397) at Cupid Isle ")

MisHelpTalk("<t>Thinking of tae drooling makes me want to rush baking my cake more.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------5--------
DefineMission(5633, "TOP Ambassador 5", 1134, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I'm scared to miss good things like drinking wine, but good thing granny still thinks of me, haha. I really miss eating granny's cake as I grow up.")
MisResultCondition(NoRecord, 1134)
MisResultCondition(HasMission,1134)
MisResultAction(ClearMission,1134)
MisResultAction(SetRecord, 1134)


----------------------------------------------------------6----------
DefineMission( 5634, "TOP Ambassador 6", 1135 )
MisBeginTalk("<t>Once I'm finished with moving these goodies I'll be going to the bar, hope my drinking friend Harbor Operator - Odie also can come.")
			
MisBeginCondition(NoMission, 1135)
MisBeginCondition(NoRecord,1135)
MisBeginCondition(HasRecord, 1134)
MisBeginAction(AddMission,1135)
MisCancelAction(ClearMission, 1135)

MisNeed(MIS_NEED_DESP,"Talk to Harbor Operator - Odie (738, 3803) at Karmas Haven.")

MisHelpTalk("<t>Not sure if Babara is more beautiful.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------6--------
DefineMission(5635, "TOP Ambassador 6", 1135, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>To a man, drinking means a chance to go on a date with a girl..")
MisResultCondition(NoRecord, 1135)
MisResultCondition(HasMission,1135)
MisResultAction(ClearMission,1135)
MisResultAction(SetRecord, 1135)



----------------------------------------------------------7----------
DefineMission( 5636, "TOP Ambassador 7", 1136 )
MisBeginTalk("<t>What are you laughing at?Humph! Help me inform the Conch Merchant . Lamon to come and collect his Conch, I need to go to the bar now.")
			
MisBeginCondition(NoMission, 1136)
MisBeginCondition(NoRecord,1136)
MisBeginCondition(HasRecord, 1135)
MisBeginAction(AddMission,1136)
MisCancelAction(ClearMission, 1136)

MisNeed(MIS_NEED_DESP,"Talk to Coral Vendor - Lamon (2061, 2543) at Region Ascaron.")

MisHelpTalk("<t>I always help those cute merchants, if not how to have money to stay in bar?")
MisResultCondition(AlwaysFailure)	
-----------------------------------------7--------
DefineMission(5637, "TOP Ambassador 7", 1136, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Odie has always been a drunk with no money, I can only use this to make him help me.")
MisResultCondition(NoRecord, 1136)
MisResultCondition(HasMission,1136)
MisResultAction(ClearMission,1136)
MisResultAction(SetRecord, 1136)


----------------------------------------------------------8----------
DefineMission( 5638, "TOP Ambassador 8", 1137 )
MisBeginTalk("<t>Why do you look at me like that? We merchants all look like this.")
			
MisBeginCondition(NoMission, 1137)
MisBeginCondition(NoRecord,1137)
MisBeginCondition(HasRecord, 1136)
MisBeginAction(AddMission,1137)
MisCancelAction(ClearMission, 1137)

MisNeed(MIS_NEED_DESP,"Talk to Captain Jack (1672, 3777) on Canary Isle.")

MisHelpTalk("<t>Don't say I didn't remind you, its not safe there, be careful!")
MisResultCondition(AlwaysFailure)	
-----------------------------------------8--------
DefineMission(5639, "TOP Ambassador 8", 1137, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I am already used to being adored by girls, and boys being jealous.")
MisResultCondition(NoRecord, 1137)
MisResultCondition(HasMission,1137)
MisResultAction(ClearMission,1137)
MisResultAction(SetRecord, 1137)


----------------------------------------------------------9----------
DefineMission( 5640, "TOP Ambassador 9", 1138 )
MisBeginTalk("<t>Scar is a man badge, beer is a pirate's partner, I will not miss out on any beer. I wonder if my friend Northern Pirate - Yakamoto will join, although not a lot of people can understand him when he talks.")
			
MisBeginCondition(NoMission, 1138)
MisBeginCondition(NoRecord,1138)
MisBeginCondition(HasRecord, 1137)
MisBeginAction(AddMission,1138)
MisCancelAction(ClearMission, 1138)

MisNeed(MIS_NEED_DESP,"Head to Autumn Island and talk toNorthern Pirate - Yakamoto (2221, 3285).")

MisHelpTalk("<t>Northern Pirate - Yakamoto has always want to exchange experience with other pirates.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------9--------
DefineMission(5641, "TOP Ambassador 9", 1138, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I am a pirate from a faraway land who just learnt your language.##@@#$, I love this beautiful island.&*&%, Hmmbut being a pirate is often misleading. I had some issue with Captain Jack when I first came here.")
MisResultCondition(NoRecord, 1138)
MisResultCondition(HasMission,1138)
MisResultAction(ClearMission,1138)
MisResultAction(SetRecord, 1138)

----------------------------------------------------------10----------
DefineMission( 5642, "TOP Ambassador 10", 1139 )
MisBeginTalk("<t>Jack is going also? Please let me think about it then. I think I'll just go fishing with my friend Trader - Sacenis.")
			
MisBeginCondition(NoMission, 1139)
MisBeginCondition(NoRecord,1139)
MisBeginCondition(HasRecord, 1138)
MisBeginAction(AddMission,1139)
MisCancelAction(ClearMission, 1139)

MisNeed(MIS_NEED_DESP,"Talk to Trader - Sacenis (2279, 1112) at Glacier Isle.")

MisHelpTalk("<t>Think I'll just wait for the date with Sacenis.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------10--------
DefineMission(5643, "TOP Ambassador 10", 1139, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I really love Northern Pirate - Yakamoto, especially his romantic trait and his appearance switch.")
MisResultCondition(NoRecord, 1139)
MisResultCondition(HasMission,1139)
MisResultAction(ClearMission,1139)
MisResultAction(SetRecord, 1139)

----------------------------------------------------------11----------
DefineMission( 5644, "TOP Ambassador 11", 1140 )
MisBeginTalk("<t>If Jack likes going to the bar, then I'll go to the sea side with Northern Pirate - Yakamoto. Enemies should meet less. Yes, I think that boy Ham will be interested in going to the bar.")
			
MisBeginCondition(NoMission, 1140)
MisBeginCondition(NoRecord,1140)
MisBeginCondition(HasRecord, 1139)
MisBeginAction(AddMission,1140)
MisCancelAction(ClearMission, 1140)

MisNeed(MIS_NEED_DESP,"Talk to ham (826, 3347) at Old Shaitan City.")

MisHelpTalk("<t>Please don't teach the bad things to kids. We're mainly scared they'll come for the excitement")
MisResultCondition(AlwaysFailure)	
-----------------------------------------11--------
DefineMission(5645, "TOP Ambassador 11", 1140, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Kids shouldn't be going to the bar, buthandsome boys are an expection! I will go for sure, thank you for the good news.")
MisResultCondition(NoRecord, 1140)
MisResultCondition(HasMission,1140)
MisResultAction(ClearMission,1140)
MisResultAction(SetRecord, 1140)

----------------------------------------------------------12----------
DefineMission( 5646, "TOP Ambassador 12", 1141 )
MisBeginTalk("<t>Gentlemen will not enjoy the wine himself, and that's my rule. I am going to call my friend Zurbi.")
			
MisBeginCondition(NoMission, 1141)
MisBeginCondition(NoRecord,1141)
MisBeginCondition(HasRecord, 1140)
MisBeginAction(AddMission,1141)
MisCancelAction(ClearMission, 1141)

MisNeed(MIS_NEED_DESP,"Talk to Zurbi (1037, 671) in Atlantis Haven.")

MisHelpTalk("<t>I have never went to the bar before, do I need to bring ID card? If I get drunk will there be people sending me home?")
MisResultCondition(AlwaysFailure)	
-----------------------------------------12--------
DefineMission(5647, "TOP Ambassador 12", 1141, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>To speak the truth, everytime Ham gives a suggestion, it is very exciting. However, is because of this I get scold by my mother every time that I'm not mature enough.")
MisResultCondition(NoRecord, 1141)
MisResultCondition(HasMission,1141)
MisResultAction(ClearMission,1141)
MisResultAction(SetRecord, 1141)


----------------------------------------------------------13----------
DefineMission( 5648, "TOP Ambassador 13", 1142 )
MisBeginTalk("<t>Although I've sworn to never listen to his suggestions again, but going to the bar is my goal and sign to become mature, so..hehe, if you pass by Andes Forest Haven please help me ask Accessory - Rikka if he has any beautiful buttons. This will be my first time going to the bar, I cannot dress too poorly.")
			
MisBeginCondition(NoMission, 1142)
MisBeginCondition(NoRecord,1142)
MisBeginCondition(HasRecord, 1141)
MisBeginAction(AddMission,1142)
MisCancelAction(ClearMission, 1142)

MisNeed(MIS_NEED_DESP,"Talk to Accessory - Rikka (996, 2969) in Andes Forest Haven.")

MisHelpTalk("<t>Accessory - Rikka has a sharp eye for looking at jewelry. When I was small, my mother always brought me there to buy jewelry. Then, he'll say my mother is very beautiful each time and gives me candy.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------13--------
DefineMission(5649, "TOP Ambassador 13", 1142, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Selling buttons to go to the bar? Forbei has grow up now, but his mother face doesn't seem to get affected at all, she still processes the best charm as a customer.")
MisResultCondition(NoRecord, 1142)
MisResultCondition(HasMission,1142)
MisResultAction(ClearMission,1142)
MisResultAction(SetRecord, 1142)

----------------------------------------------------------14----------
DefineMission( 5650, "TOP Ambassador 14", 1143 )
MisBeginTalk("<t>Remembering back when I was young I met Pirate Jeremy, and our friendship is very good. My Jewlery business is so successful thanks to his help. If you're free to go Isle of Chill, please help me give my greetings to him.")
			
MisBeginCondition(NoMission, 1143)
MisBeginCondition(NoRecord,1143)
MisBeginCondition(HasRecord, 1142)
MisBeginAction(AddMission,1143)
MisCancelAction(ClearMission, 1143)

MisNeed(MIS_NEED_DESP,"Talk to Pirate Jeremy (2362, 657) on Isle of Chill.")

MisHelpTalk("Isle of Chill is located at Region Magical Ocean.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------14--------        
DefineMission(5651, "TOP Ambassador 14", 1143, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Haha, that was my first time going to the bar. I remember that time I was only a tiny pirate, and I drank to make myself become more brave while sailing at sea. I don't know why I'm so fateful with Rikka, sometimes I think if Rikka is a girl, then maybe I won't be single anymore.")
MisResultCondition(NoRecord, 1143)
MisResultCondition(HasMission,1143)
MisResultAction(ClearMission,1143)
MisResultAction(SetRecord, 1143)

----------------------------------------------------------15----------
DefineMission( 5652, "TOP Ambassador 15", 1144 )
MisBeginTalk("<t>This reminds me I want to join the pirate group Lessie, but too bad he's too tiny. If you want to travel out to the sea, you have to drink beer to become more brave.")
			
MisBeginCondition(NoMission, 1144)
MisBeginCondition(NoRecord,1144)
MisBeginCondition(HasRecord, 1143)
MisBeginAction(AddMission,1144)
MisCancelAction(ClearMission, 1144)

MisNeed(MIS_NEED_DESP,"Talk to Lessie (1379, 612) at Icicle City.")

MisHelpTalk("<t>I beg of you, I love him dearly.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------15-------- 
DefineMission(5653, "TOP Ambassador 15", 1144, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>My goal is to become a Golden Pirate like Huckinson.")
MisResultCondition(NoRecord, 1144)
MisResultCondition(HasMission,1144)
MisResultAction(ClearMission,1144)
MisResultAction(SetRecord, 1144)


----------------------------------------------------------16----------
DefineMission( 5654, "TOP Ambassador 16", 1145 )
MisBeginTalk("<t>To achieve my dreams, I think I should start learning to swim, sword skills, and operate a boat. I think Harbor Operator - Whitcombe can help me learn how to operate a boat.")
			
MisBeginCondition(NoMission, 1145)
MisBeginCondition(NoRecord,1145)
MisBeginCondition(HasRecord, 1144)
MisBeginAction(AddMission,1145)
MisCancelAction(ClearMission, 1145)

MisNeed(MIS_NEED_DESP,"Talk to Harbor Operator - Whitcombe (2041, 1355) in <yHafta Haven>")

MisHelpTalk("<t>I'm a very stubborn person, for recovering the peaceful sea for trading I'm willing to be a good willed pirate.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------16-------- 
DefineMission(5655, "TOP Ambassador 16", 1145, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>He wants to learn operating a boat from me? Haha, my boat skills are very famous you know.")
MisResultCondition(NoRecord, 1145)
MisResultCondition(HasMission,1145)
MisResultAction(ClearMission,1145)
MisResultAction(SetRecord, 1145)


----------------------------------------------------------17----------
DefineMission( 5656, "TOP Ambassador 17", 1146 )
MisBeginTalk("<t>I don't go to the bar anymore. There seem to be a growing of dark aura around here, If I don't help the ships that came from afar, I don't know how many people will become shark's buffet. Maybe you can go ask Solaru, I remember he said he was going to go for a vacation, maybe he can intro me a place or two.")
			
MisBeginCondition(NoMission, 1146)
MisBeginCondition(NoRecord,1146)
MisBeginCondition(HasRecord, 1145)
MisBeginAction(AddMission,1146)
MisCancelAction(ClearMission, 1146)

MisNeed(MIS_NEED_DESP,"Talk to Solaru (1202, 3179) in Babu Haven.")

MisHelpTalk("<t>Solaru is a youngster full of passion, go and talk to him.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------17-------- 
DefineMission(5657, "TOP Ambassador 17", 1146, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>I'm a person who is willing to give up life just for my dreams and honour, even girls agree.")
MisResultCondition(NoRecord, 1146)
MisResultCondition(HasMission,1146)
MisResultAction(ClearMission,1146)
MisResultAction(SetRecord, 1146)

----------------------------------------------------------18----------
DefineMission( 5658, "TOP Ambassador 18", 1147 )
MisBeginTalk("<t>I'm about to go out, if you have anything to say please hurry. Bar? That's a good place to meet people, I think I am going to change my plan now. Please help me tell Ramus that this haven came a lot of traders lately and he should look over his port as well, thank you very much.")
			
MisBeginCondition(NoMission, 1147)
MisBeginCondition(NoRecord,1147)
MisBeginCondition(HasRecord, 1146)
MisBeginAction(AddMission,1147)
MisCancelAction(ClearMission, 1147)

MisNeed(MIS_NEED_DESP,"Talk to Harbor Operator - Ramus (2297, 3723) at Muse Haven.")

MisHelpTalk("<t>Compared to sleeping, young people should spent time noticing their growth. Why are you looking at me like that? I'm a youth with great passion in other people eyes.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------18-------- 
DefineMission(5659, "TOP Ambassador 18", 1147, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Thank you for coming and bringing me this message. I'm short of man right now, do you want to stay here and be my assistant?")
MisResultCondition(NoRecord, 1147)
MisResultCondition(HasMission,1147)
MisResultAction(ClearMission,1147)
MisResultAction(SetRecord, 1147)

----------------------------------------------------------19----------
DefineMission( 5660, "TOP Ambassador 19", 1148 )
MisBeginTalk("<t>Habour Operator is a high rank and busy job. Speaking of busy I haven't seen cute Elizabeth for awhile, I wonder if her style is still the same.")
			
MisBeginCondition(NoMission, 1148)
MisBeginCondition(NoRecord,1148)
MisBeginCondition(HasRecord, 1147)
MisBeginAction(AddMission,1148)
MisCancelAction(ClearMission, 1148)

MisNeed(MIS_NEED_DESP,"Look for Elizabeth in the Treasure Gulf at (616, 965).")

MisHelpTalk("<t>When elizabeth was small, she was pretty like a mess.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------19-------- 
DefineMission(5661, "TOP Ambassador 19", 1148, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>Waiting for one person is very painful and lonely, but I enjoy my standing ground for this passion.")
MisResultCondition(NoRecord, 1148)
MisResultCondition(HasMission,1148)
MisResultAction(ClearMission,1148)
MisResultAction(SetRecord, 1148)


	----------------------------------------------------------20----------
DefineMission( 5662, "TOP Ambassador 20", 1149 )
MisBeginTalk("<t>I always relay on some strange shining jewlery to waste time, but Miner Drunky told me he dugged up one Ancient Emerald, and I'm in need of money, I must make him stay and sell it to me. Strange though, how can digging dug up an emerald?")
			
MisBeginCondition(NoMission, 1149)
MisBeginCondition(NoRecord,1149)
MisBeginCondition(HasRecord, 1148)
MisBeginAction(AddMission,1149)
MisCancelAction(ClearMission, 1149)

MisNeed(MIS_NEED_DESP,"Talk to Miner Drunky (296, 57) down at Silver Mine B2")

MisHelpTalk("<t>")
MisResultCondition(AlwaysFailure)	
-----------------------------------------20-------- 
DefineMission(5663, "TOP Ambassador 20", 1149, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>An outstanding man and expensive treasure is what beautiful women loves the most, like me and Ancient Emerald.")
MisResultCondition(NoRecord, 1149)
MisResultCondition(HasMission,1149)
MisResultAction(ClearMission,1149)
MisResultAction(SetRecord, 1149)

	----------------------------------------------------------21----------
DefineMission( 5664, "TOP Ambassador 21", 1150 )
MisBeginTalk("<t>Congratlations for completing this super long Community quest, think now you will know more people around the world, this will help in your exploring a lot. I also heard the beautiful Bar Waitress - <bBabara> is looking for you.")
			
MisBeginCondition(NoMission, 1150)
MisBeginCondition(NoRecord,1150)
MisBeginCondition(HasRecord, 1149)
MisBeginAction(AddMission,1150)
MisCancelAction(ClearMission, 1150)

MisNeed(MIS_NEED_DESP,"Talk to Babara (1310, 530).")

MisHelpTalk("<t>Babara has a present for you.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------21------- 
DefineMission(5667, "Community Hero 21", 1150, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You have passed TOP test, I will award you with an emblem.")
MisResultCondition(NoRecord, 1150)
MisResultCondition(HasMission,1150)
MisResultAction(ClearMission,1150)
MisResultAction(SetRecord, 1150)
MisResultAction(GiveItem, 3033, 1, 4)
MisResultBagNeed(1)

--	-------------------------------------------------	--------
--	DefineMission (5668, "Blood Shed Taurus Special Operation", 1153)
--	
--	MisBeginTalk("<t>This quest is Taurus Special Operation, you don't have to participate if you don't want to, but you don't get the prize. Sail to <b Aerase Haven and talk to Harbor Operator - Buni (2042, 6351) in 10 minutes>.")
--
--	MisBeginCondition(NoMission,1153)
--	MisBeginCondition(NoRecord,1153)
--	MisBeginCondition(HasRecord,1113)
--	MisBeginCondition(HasRecord,1114)
--	MisBeginCondition(HasRecord,1115)
--	MisBeginCondition(HasRecord,1116)
--	MisBeginCondition(HasRecord,1117)
--	MisBeginCondition(HasRecord,1118)
--	MisBeginCondition(HasRecord,1150)
--	MisBeginCondition(HasRecord,1119)
--	MisBeginAction(AddMission,1153)
--	MisBeginAction(AddChaItem3, 2952)---------
--	MisCancelAction(ClearMission, 1153)
--	MisBeginBagNeed(1)
--	
--	MisNeed(MIS_NEED_DESP,"Sail to Aerase Haven and talk to Harbor Operator - Buni (2042, 635).")
--	MisHelpTalk("<t>Go now! You only have 15 minutes.")
--
--	MisResultCondition(AlwaysFailure)	
--
--	------------------------------------------
--
--	DefineMission(5669, "Blood Shed Taurus Special Operation", 1153, COMPLETE_SHOW )
--	
--	MisBeginCondition(AlwaysFailure )
--
--	MisResultTalk("<t>Not too bad, you will not regret it.")
--	MisResultCondition(HasMission, 1153)
--	MisResultCondition(NoRecord,1153)
--	MisResultAction(AddChaItem5, 2952)----
--	MisResultAction(ClearMission, 1153)
--	MisResultAction(SetRecord,  1153 )
--	MisResultAction(GiveItem, 3035, 1, 4)------------
--	MisResultBagNeed(1)

---------------------------------------------------	--------
--	DefineMission (5673, "Blood Shed Taurus Special Operation", 1159)
--	
--	MisBeginTalk("<t>This quest is Taurus Special Operation, you don't have to participate if you don't want to, but you don't get the prize. Sail to <b Aerase Haven and talk to Harbor Operator - Buni (2042, 6351) in 10 minutes>.")
--
--	MisBeginCondition(NoMission,1159)
--	MisBeginCondition(NoRecord,1159)
--	MisBeginCondition(HasRecord,1112)
--	MisBeginCondition(HasRecord,1120)
--	MisBeginCondition(HasRecord,1121)
--	MisBeginCondition(HasRecord,1122)
--	MisBeginCondition(HasRecord,1123)
--	MisBeginCondition(HasRecord,1124)
--	MisBeginCondition(HasRecord,1150)
--	MisBeginCondition(HasRecord,1119)
--	MisBeginAction(AddMission,1159)
--	MisBeginAction(AddChaItem3, 2952)---------
--	MisCancelAction(ClearMission, 1159)
--	MisBeginBagNeed(1)
--	
--	MisNeed(MIS_NEED_DESP,"Sail to Aerase Haven and talk to Harbor Operator - Buni (2042, 635).")
--	MisHelpTalk("<t>Go now! You only have 15 minutes.")
--
--	MisResultCondition(AlwaysFailure)	
--
--	------------------------------------------
--
--	DefineMission(5674, "Blood Shed Taurus Special Operation", 1159, COMPLETE_SHOW )
--	
--	MisBeginCondition(AlwaysFailure )
--
--	MisResultTalk("<t>Not too bad, you will not regret it.")
--	MisResultCondition(HasMission, 1159)
--	MisResultCondition(NoRecord,1159)
--	MisResultAction(AddChaItem5, 2952)----
--	MisResultAction(ClearMission, 1159)
--	MisResultAction(SetRecord,  1159 )
--	MisResultAction(GiveItem, 3035, 1, 4)------------
--	MisResultBagNeed(1)
--	
-------------------------------------------------	--------
DefineMission (5675, "Blood Shed Taurus Special Operation", 1160)

MisBeginTalk("<t>This quest is a Taurus Palace special event. You don't have to play the quest, but of course you don't get a present. Test your knowledge now, Talk to <b Jack Arrow (230, 579) at Skeletar Isle within 7 minutes>.")

MisBeginCondition(NoMission,1160)
MisBeginCondition(NoRecord,1160)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(HasRecord,1125)
MisBeginCondition(HasRecord,1126)
MisBeginCondition(HasRecord,1127)
MisBeginCondition(HasRecord,1128)
MisBeginCondition(HasRecord,1129)
MisBeginCondition(HasRecord,1150)
MisBeginCondition(HasRecord,1119)
MisBeginAction(AddMission,1160)
MisBeginAction(AddChaItem3, 2952)---------
MisCancelAction(ClearMission, 1160)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"Talk to Jack Arrow (230, 579).")
MisHelpTalk("<t>Hurry up and go, you only have 7 minutes.")

MisResultCondition(AlwaysFailure)	

------------------------------------------

DefineMission(5676, "Blood Shed Taurus Special Operation", 1160, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>You dare to challenge Skeletar Isle? You got guts! However, I hate people like you who have guts, only give me nothing but trouble and making me busy for nothing.")
MisResultCondition(HasMission, 1160)
MisResultCondition(NoRecord,1160)
MisResultAction(AddChaItem5, 2952)----
MisResultAction(ClearMission, 1160)
MisResultAction(SetRecord,  1160 )
MisResultAction(GiveItem, 3035, 1, 4)------------
MisResultBagNeed(1)



-----------------------------------------------------------	
DefineMission (5670, "Prize for completing Sailor of Taurus", 1156)

MisBeginTalk("<t>Collect all 7 Taurus Emblem to exchange with me for Taurus Protector Seal and Gemini Gate Ticket. There are also more prizes available.")

MisBeginCondition(NoMission,1156)
MisBeginCondition(HasRecord,1111)
MisBeginCondition(HasRecord,1114)
MisBeginCondition(HasRecord,1115)
MisBeginCondition(HasRecord,1116)
MisBeginCondition(HasRecord,1117)
MisBeginCondition(HasRecord,1118)
MisBeginCondition(HasRecord,1150)
MisBeginCondition(HasRecord,1119)
MisBeginCondition(NoRecord,1156)
MisBeginAction(AddMission,1156)  
MisBeginAction(AddTrigger, 11561, TE_GETITEM, 3028, 1 )		
MisBeginAction(AddTrigger, 11562, TE_GETITEM, 3029, 1 )		
MisBeginAction(AddTrigger, 11563, TE_GETITEM, 3030, 1 )		
MisBeginAction(AddTrigger, 11564, TE_GETITEM, 3031, 1 )		
MisBeginAction(AddTrigger, 11565, TE_GETITEM, 3032, 1 )		
MisBeginAction(AddTrigger, 11566, TE_GETITEM, 3033, 1 )		
MisBeginAction(AddTrigger, 11567, TE_GETITEM, 3034, 1 )	
MisCancelAction(ClearMission, 1156)						                                     

MisNeed(MIS_NEED_ITEM, 3028, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 3029, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 3030, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 3031, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 3032, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 3033, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 3034, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>Next Palace is Gemini Palace, more exciting challenges and prizes awaits you.")

MisResultCondition(HasMission, 1156)
MisResultCondition(NoRecord,1156)
MisResultCondition(HasItem, 3028, 1 )
MisResultCondition(HasItem, 3029, 1 )
MisResultCondition(HasItem, 3030, 1 )
MisResultCondition(HasItem, 3031, 1 )
MisResultCondition(HasItem, 3032, 1 )
MisResultCondition(HasItem, 3033, 1 )
MisResultCondition(HasItem, 3034, 1 )

MisResultAction(TakeItem, 3028, 1 )
MisResultAction(TakeItem, 3029, 1 )
MisResultAction(TakeItem, 3030, 1 )
MisResultAction(TakeItem, 3031, 1 )
MisResultAction(TakeItem, 3032, 1 )
MisResultAction(TakeItem, 3033, 1 )
MisResultAction(TakeItem, 3034, 1 )

MisResultAction(ClearMission, 1156)
MisResultAction(SetRecord,  1156 )
MisResultAction(GiveItem, 3026, 1, 4)
MisResultAction(GiveItem, 3027, 1, 4)
MisResultAction(GiveItem, 0227, 10, 4)
MisResultAction(AddMoney,100000,100000)
MisResultAction(JINNiuSS)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 3028)	
TriggerAction( 1, AddNextFlag, 1156, 10, 1 )
RegCurTrigger( 11561 )

InitTrigger()
TriggerCondition( 1, IsItem, 3029)	
TriggerAction( 1, AddNextFlag, 1156, 20, 1 )
RegCurTrigger( 11562 )

InitTrigger()
TriggerCondition( 1, IsItem, 3030)	
TriggerAction( 1, AddNextFlag, 1156, 30, 1 )
RegCurTrigger( 11563 )

InitTrigger()
TriggerCondition( 1, IsItem, 3031)	
TriggerAction( 1, AddNextFlag, 1156, 40, 1 )
RegCurTrigger( 11564 )

InitTrigger()
TriggerCondition( 1, IsItem, 3032)	
TriggerAction( 1, AddNextFlag, 1156, 50, 1 )
RegCurTrigger( 11565 )

InitTrigger()
TriggerCondition( 1, IsItem, 3033)	
TriggerAction( 1, AddNextFlag, 1156, 60, 1 )
RegCurTrigger( 11566 )

InitTrigger()
TriggerCondition( 1, IsItem, 3034)	
TriggerAction( 1, AddNextFlag, 1156, 70, 1 )
RegCurTrigger( 11567 )
--------------------------------------------------------	
DefineMission (5671, "Prize of completing Pirate of Taurus", 1157)

MisBeginTalk("<t>Collect all 7 Taurus Emblem to exchange with me for Taurus Protector Seal and Gemini Gate Ticket. There are also more prizes available.")

MisBeginCondition(NoMission,1157)
MisBeginCondition(HasRecord,1112)
MisBeginCondition(HasRecord,1120)
MisBeginCondition(HasRecord,1121)
MisBeginCondition(HasRecord,1122)
MisBeginCondition(HasRecord,1123)
MisBeginCondition(HasRecord,1124)
MisBeginCondition(HasRecord,1150)
MisBeginCondition(HasRecord,1119)
MisBeginCondition(NoRecord,1157)
MisBeginAction(AddMission,1157)
MisBeginAction(AddTrigger, 11571, TE_GETITEM, 3028, 1 )		
MisBeginAction(AddTrigger, 11572, TE_GETITEM, 3029, 1 )		
MisBeginAction(AddTrigger, 11573, TE_GETITEM, 3030, 1 )		
MisBeginAction(AddTrigger, 11574, TE_GETITEM, 3031, 1 )		
MisBeginAction(AddTrigger, 11575, TE_GETITEM, 3032, 1 )		
MisBeginAction(AddTrigger, 11576, TE_GETITEM, 3033, 1 )		
MisBeginAction(AddTrigger, 11577, TE_GETITEM, 3034, 1 )		
MisCancelAction(ClearMission, 1157)						                                     

MisNeed(MIS_NEED_ITEM, 3028, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 3029, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 3030, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 3031, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 3032, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 3033, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 3034, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>Next Palace is Gemini Palace, more exciting challenges and prizes awaits you.")

MisResultCondition(HasMission, 1157)
MisResultCondition(NoRecord,1157)
MisResultCondition(HasItem, 3028, 1 )
MisResultCondition(HasItem, 3029, 1 )
MisResultCondition(HasItem, 3030, 1 )
MisResultCondition(HasItem, 3031, 1 )
MisResultCondition(HasItem, 3032, 1 )
MisResultCondition(HasItem, 3033, 1 )
MisResultCondition(HasItem, 3034, 1 )

MisResultAction(TakeItem, 3028, 1 )
MisResultAction(TakeItem, 3029, 1 )
MisResultAction(TakeItem, 3030, 1 )
MisResultAction(TakeItem, 3031, 1 )
MisResultAction(TakeItem, 3032, 1 )
MisResultAction(TakeItem, 3033, 1 )
MisResultAction(TakeItem, 3034, 1 )

MisResultAction(ClearMission, 1157)
MisResultAction(SetRecord,  1157 )
MisResultAction(GiveItem, 3026, 1, 4)
MisResultAction(GiveItem, 3027, 1, 4)
MisResultAction(GiveItem, 0227, 20, 4)
MisResultAction(AddMoney,200000,200000)
MisResultAction(JINNiuHD)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 3028)	
TriggerAction( 1, AddNextFlag, 1157, 10, 1 )
RegCurTrigger( 11571 )

InitTrigger()
TriggerCondition( 1, IsItem, 3029)	
TriggerAction( 1, AddNextFlag, 1157, 20, 1 )
RegCurTrigger( 11572 )

InitTrigger()
TriggerCondition( 1, IsItem, 3030)	
TriggerAction( 1, AddNextFlag, 1157, 30, 1 )
RegCurTrigger( 11573 )

InitTrigger()
TriggerCondition( 1, IsItem, 3031)	
TriggerAction( 1, AddNextFlag, 1157, 40, 1 )
RegCurTrigger( 11574 )

InitTrigger()
TriggerCondition( 1, IsItem, 3032)	
TriggerAction( 1, AddNextFlag, 1157, 50, 1 )
RegCurTrigger( 11575 )

InitTrigger()
TriggerCondition( 1, IsItem, 3033)	
TriggerAction( 1, AddNextFlag, 1157, 60, 1 )
RegCurTrigger( 11576 )

InitTrigger()
TriggerCondition( 1, IsItem, 3034)	
TriggerAction( 1, AddNextFlag, 1157, 70, 1 )
RegCurTrigger( 11577 )

-------------------------------------------------------	
DefineMission (5672, "Prize for completing Captain of Taurus", 1158)

MisBeginTalk("<t>Collect all 7 Taurus Emblem to exchange with me for Taurus Protector Seal and Gemini Gate Ticket. There are also more prizes available.")

MisBeginCondition(NoMission,1158)
MisBeginCondition(HasRecord,1113)
MisBeginCondition(HasRecord,1125)
MisBeginCondition(HasRecord,1126)
MisBeginCondition(HasRecord,1127)
MisBeginCondition(HasRecord,1128)
MisBeginCondition(HasRecord,1129)
MisBeginCondition(HasRecord,1150)
MisBeginCondition(HasRecord,1119)
MisBeginCondition(NoRecord,1158)
MisBeginAction(AddMission,1158)   
MisBeginAction(AddTrigger, 11581, TE_GETITEM, 3028, 1 )		
MisBeginAction(AddTrigger, 11582, TE_GETITEM, 3029, 1 )		
MisBeginAction(AddTrigger, 11583, TE_GETITEM, 3030, 1 )		
MisBeginAction(AddTrigger, 11584, TE_GETITEM, 3031, 1 )		
MisBeginAction(AddTrigger, 11585, TE_GETITEM, 3032, 1 )		
MisBeginAction(AddTrigger, 11586, TE_GETITEM, 3033, 1 )		
MisBeginAction(AddTrigger, 11587, TE_GETITEM, 3034, 1 )	
MisCancelAction(ClearMission, 1158)						                                     

MisNeed(MIS_NEED_ITEM, 3028, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 3029, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 3030, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 3031, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 3032, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 3033, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 3034, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>Next Palace is Gemini Palace, more exciting challenges and prizes awaits you.")

MisResultCondition(HasMission, 1158)
MisResultCondition(NoRecord,1158)
MisResultCondition(HasItem, 3028, 1 )
MisResultCondition(HasItem, 3029, 1 )
MisResultCondition(HasItem, 3030, 1 )
MisResultCondition(HasItem, 3031, 1 )
MisResultCondition(HasItem, 3032, 1 )
MisResultCondition(HasItem, 3033, 1 )
MisResultCondition(HasItem, 3034, 1 )

MisResultAction(TakeItem, 3028, 1 )
MisResultAction(TakeItem, 3029, 1 )
MisResultAction(TakeItem, 3030, 1 )
MisResultAction(TakeItem, 3031, 1 )
MisResultAction(TakeItem, 3032, 1 )
MisResultAction(TakeItem, 3033, 1 )
MisResultAction(TakeItem, 3034, 1 )

MisResultAction(ClearMission, 1158)
MisResultAction(SetRecord,  1158 )
MisResultAction(GiveItem, 3026, 1, 4)
MisResultAction(GiveItem, 3027, 1, 4)
MisResultAction(GiveItem, 0227, 30, 4)
MisResultAction(AddMoney,300000,300000)
MisResultAction(JINNiuCZ)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 3028)	
TriggerAction( 1, AddNextFlag, 1158, 10, 1 )
RegCurTrigger( 11581 )

InitTrigger()
TriggerCondition( 1, IsItem, 3029)	
TriggerAction( 1, AddNextFlag, 1158, 20, 1 )
RegCurTrigger( 11582 )

InitTrigger()
TriggerCondition( 1, IsItem, 3030)	
TriggerAction( 1, AddNextFlag, 1158, 30, 1 )
RegCurTrigger( 11583 )

InitTrigger()
TriggerCondition( 1, IsItem, 3031)	
TriggerAction( 1, AddNextFlag, 1158, 40, 1 )
RegCurTrigger( 11584 )

InitTrigger()
TriggerCondition( 1, IsItem, 3032)	
TriggerAction( 1, AddNextFlag, 1158, 50, 1 )
RegCurTrigger( 11585 )

InitTrigger()
TriggerCondition( 1, IsItem, 3033)	
TriggerAction( 1, AddNextFlag, 1158, 60, 1 )
RegCurTrigger( 11586 )

InitTrigger()
TriggerCondition( 1, IsItem, 3034)	
TriggerAction( 1, AddNextFlag, 1158, 70, 1 )
RegCurTrigger( 11587 )

--	----------------------------------------------------------------
--
--       DefineMission(6126,"",1452)
--
--	MisBeginTalk("<t>,,.")
--	MisBeginCondition(NoMission, 1452)
--	MisBeginCondition(NoRecord,1452)
--	MisBeginAction(AddMission,1452)
--	MisBeginAction(AddTrigger, 14521, TE_GETITEM, 4325, 5)
--	MisBeginAction(AddTrigger, 14522, TE_GETITEM, 1680, 5)
--	MisBeginAction(AddTrigger, 14523, TE_GETITEM, 4433, 5)
--	MisBeginAction(AddTrigger, 14524, TE_GETITEM, 4357, 5)
--	MisBeginAction(AddTrigger, 14525, TE_GETITEM, 4461, 40)
--	MisBeginAction(AddTrigger, 14526, TE_GETITEM, 4462, 40)
--
--
--	MisCancelAction(ClearMission, 1452)
--
--	MisNeed(MIS_NEED_DESP," 5(1179,371), 5(1950,2563), 5(1384,3065), 5(910,2971), 40 1455,560)  40(2266,590).")
--	MisNeed(MIS_NEED_ITEM, 4325, 5, 10, 5)
--	MisNeed(MIS_NEED_ITEM, 1680, 5, 15, 5)
--	MisNeed(MIS_NEED_ITEM, 4433, 5, 20, 5)
--	MisNeed(MIS_NEED_ITEM, 4357, 5, 25, 5)
--	MisNeed(MIS_NEED_ITEM, 4461, 40, 30, 40)
--	MisNeed(MIS_NEED_ITEM, 4462, 40, 70, 40)
--
--	
--	
--	MisHelpTalk("<t>,")
--	MisResultTalk("<t>,,.")
--
--	MisResultCondition(HasMission, 1452)
--	MisResultCondition(NoRecord,1452)
--	MisResultCondition(HasItem, 4325, 5)
--	MisResultCondition(HasItem, 1680, 5)
--	MisResultCondition(HasItem, 4433, 5)
--	MisResultCondition(HasItem, 4357, 5)
--	MisResultCondition(HasItem, 4461, 40)
--	MisResultCondition(HasItem, 4462, 40)
--
--	
--	
--	MisResultAction(TakeItem, 4325, 5 )
--	MisResultAction(TakeItem, 1680, 5 )
--	MisResultAction(TakeItem, 4433, 5 )
--	MisResultAction(TakeItem, 4357, 5 )
--	MisResultAction(TakeItem, 4461, 40 )
--	MisResultAction(TakeItem, 4462, 40 )
--
--	
--	
--	MisResultAction(GiveItem, 2909,1,4)------------
--	MisResultAction(ClearMission, 1452)
--	MisResultAction(SetRecord, 1452)
--	MisResultAction(ClearRecord, 1452)---------------can be repeated
--	MisResultBagNeed(1)
--	
--
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4325)	
--	TriggerAction( 1, AddNextFlag, 1452, 10, 5 )
--	RegCurTrigger( 14521 )
--
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 1680)	
--	TriggerAction( 1, AddNextFlag, 1452, 15, 5 )
--	RegCurTrigger( 14522 )
--	
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4433)	
--	TriggerAction( 1, AddNextFlag, 1452, 20, 5 )
--	RegCurTrigger( 14523 )
--	
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4357)	
--	TriggerAction( 1, AddNextFlag, 1452, 25, 5 )
--	RegCurTrigger( 14524 )
--	
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4461)	
--	TriggerAction( 1, AddNextFlag, 1452, 30, 40 )
--	RegCurTrigger( 14525 )
--	
--	InitTrigger()
--	TriggerCondition( 1, IsItem, 4462)	
--	TriggerAction( 1, AddNextFlag, 1452, 70, 40 )
--	RegCurTrigger( 14526 )

-----------------------------------------------------------	--------
DefineMission (5673, "", 1163)

MisBeginTalk("<t>,,.? ")

MisBeginCondition(NoMission,1163)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(NoRecord,1163)
MisBeginAction(AddMission,1163)
MisCancelAction(ClearMission, 1163)

MisNeed(MIS_NEED_DESP,"5")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1163)
MisResultCondition(NoRecord,1163)
MisResultCondition(HasFightingPoint,5 )
MisResultAction(TakeFightingPoint, 5 )
MisResultAction(ClearMission, 1163)
MisResultAction(SetRecord,  1163 )
MisResultAction(GiveItem, 1874, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	--------
DefineMission (5674, "", 1164)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1164)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(NoRecord,1164)
MisBeginAction(AddMission,1164)
MisCancelAction(ClearMission, 1164)

MisNeed(MIS_NEED_DESP,"2000")
MisHelpTalk("<t>,?2000?")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1164)
MisResultCondition(NoRecord,1164)
MisResultCondition(HasCredit,2000 )
MisResultAction(TakeCredit, 2000 )
MisResultAction(ClearMission, 1164)
MisResultAction(SetRecord,  1164 )
MisResultAction(GiveItem, 1875, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	---------
DefineMission (5741, "", 1165)

MisBeginTalk("<t>,,,...")

MisBeginCondition(NoMission,1165)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(NoRecord,1165)
MisBeginAction(AddMission,1165)
MisCancelAction(ClearMission, 1165)

MisNeed(MIS_NEED_DESP,"Level reached 60")
MisHelpTalk("<t>,60.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1165)
MisResultCondition(NoRecord,1165)
MisResultCondition(LvCheck, ">", 59 )
MisResultAction(ClearMission, 1165)
MisResultAction(SetRecord,  1165 )
MisResultAction(GiveItem, 1876, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	----------
DefineMission (5742, "", 1166)

MisBeginTalk("<t>..,,.")

MisBeginCondition(NoMission,1166)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(NoRecord,1166)
MisBeginAction(AddMission,1166)
MisCancelAction(ClearMission, 1166)

MisNeed(MIS_NEED_DESP,"Gained 500 honor points.")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1166)
MisResultCondition(NoRecord,1166)
MisResultCondition(HasHonorPoint,500 )
MisResultAction(TakeHonorPoint, 500 )
MisResultAction(ClearMission, 1166)
MisResultAction(SetRecord,  1166 )
MisResultAction(GiveItem, 1877, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	------------
DefineMission (5677, "", 1167)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1167)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(NoRecord,1167)
MisBeginAction(AddMission,1167)
MisBeginAction(AddTrigger, 11671, TE_GETITEM, 1346, 10 )---------------10
MisBeginAction(AddTrigger, 11672, TE_GETITEM, 4526, 10 )--------------- 10
MisBeginAction(AddTrigger, 11673, TE_GETITEM, 1608, 10 )-------------10--------
MisBeginAction(AddTrigger, 11674, TE_GETITEM, 4495, 1 )--------------1
MisBeginAction(AddTrigger, 11675, TE_GETITEM, 1612, 20 )--------------20
MisBeginAction(AddTrigger, 11676, TE_GETITEM, 1140, 20 )-------------LV1 20
MisBeginAction(AddTrigger, 11677, TE_GETITEM, 3094, 5 )--------------5
MisCancelAction(ClearMission, 1167)


MisNeed(MIS_NEED_ITEM, 1346, 10, 1, 10 )
MisNeed(MIS_NEED_ITEM, 4526, 10, 11, 10 )
MisNeed(MIS_NEED_ITEM, 1608, 10, 21, 10 )
MisNeed(MIS_NEED_ITEM, 4495, 1, 31, 1 )
MisNeed(MIS_NEED_ITEM, 1612, 20, 32, 20 )
MisNeed(MIS_NEED_ITEM, 1140, 20, 52, 20 )
MisNeed(MIS_NEED_ITEM, 3094, 5, 72, 5 )


MisHelpTalk("<t>,")
MisResultTalk("<t>,,,.")

MisResultCondition(HasMission, 1167)
MisResultCondition(NoRecord,1167)
MisResultCondition(HasItem, 1346, 10 )
MisResultCondition(HasItem, 4526, 10 )
MisResultCondition(HasItem, 1608, 10 )
MisResultCondition(HasItem, 4495, 1 )
MisResultCondition(HasItem, 1612, 20 )
MisResultCondition(HasItem, 1140, 20 )
MisResultCondition(HasItem, 3094, 5 )


MisResultAction(TakeItem, 1346, 10 )
MisResultAction(TakeItem, 4526, 10 )
MisResultAction(TakeItem, 1608, 10 )
MisResultAction(TakeItem, 4495, 1)
MisResultAction(TakeItem, 1612, 20 )
MisResultAction(TakeItem, 1140, 20 )
MisResultAction(TakeItem, 3094, 5 )

MisResultAction(ClearMission, 1167)
MisResultAction(SetRecord,  1167 )
MisResultAction(GiveItem, 1878, 1, 4)
MisResultAction(GiveItem, 1882, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1346)	
TriggerAction( 1, AddNextFlag, 1167, 1, 10 )
RegCurTrigger( 11671 )

InitTrigger()
TriggerCondition( 1, IsItem, 4526)	
TriggerAction( 1, AddNextFlag, 1167, 11, 10 )
RegCurTrigger( 11672 )

InitTrigger()
TriggerCondition( 1, IsItem, 1608)	
TriggerAction( 1, AddNextFlag, 1167, 21, 10 )
RegCurTrigger( 11673 )

InitTrigger()
TriggerCondition( 1, IsItem, 4495)	
TriggerAction( 1, AddNextFlag, 1167, 31, 1 )
RegCurTrigger( 11674 )

InitTrigger()
TriggerCondition( 1, IsItem, 1612)	
TriggerAction( 1, AddNextFlag, 1167, 36, 20 )
RegCurTrigger( 11675 )

InitTrigger()
TriggerCondition( 1, IsItem, 1140)	
TriggerAction( 1, AddNextFlag, 1167, 37, 20 )
RegCurTrigger( 11676 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1167, 87, 5 )
RegCurTrigger( 11677 )


--------------------------------------------------------------------
DefineMission( 5678, "", 1168 )
MisBeginTalk("<t>BOSS,.")
			
MisBeginCondition(NoMission, 1168)
MisBeginCondition(HasRecord,1162)
MisBeginCondition(NoRecord,1168)
MisBeginAction(AddMission,1168)
MisBeginAction(AddTrigger, 11681, TE_KILL, 1039, 1)---

MisCancelAction(ClearMission, 1168)

MisNeed(MIS_NEED_DESP,"(2527, 2467)!")
MisNeed(MIS_NEED_KILL, 1039,1, 10, 1)


MisResultTalk("<t>.!")
MisHelpTalk("<t>,,.")
MisResultCondition(HasMission,  1168)
MisResultCondition(HasFlag, 1168, 10)
MisResultCondition(NoRecord , 1168)
MisResultAction(GiveItem, 1880, 1, 4 )
MisResultAction(ClearMission,  1168)
MisResultAction(SetRecord,  1168 )
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 1039)	
TriggerAction( 1, AddNextFlag, 1168, 10, 1 )
RegCurTrigger( 11681 )

-----------------------------------------------------------	
DefineMission (5679, "", 1169)

MisBeginTalk("<t>,,.? ")

MisBeginCondition(NoMission,1169)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(NoRecord,1169)
MisBeginAction(AddMission,1169)
MisCancelAction(ClearMission, 1169)

MisNeed(MIS_NEED_DESP,"10")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1169)
MisResultCondition(NoRecord,1169)
MisResultCondition(HasFightingPoint,10 )
MisResultAction(TakeFightingPoint, 10 )
MisResultAction(ClearMission, 1169)
MisResultAction(SetRecord,  1169 )
MisResultAction(GiveItem, 1874, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5680, "", 1170)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1170)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(NoRecord,1170)
MisBeginAction(AddMission,1170)
MisCancelAction(ClearMission, 1170)

MisNeed(MIS_NEED_DESP,"Obtained 5000 points of reputation.")
MisHelpTalk("<t>,?5000?")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1170)
MisResultCondition(NoRecord,1170)
MisResultCondition(HasCredit,5000 )
MisResultAction(TakeCredit,5000  )
MisResultAction(ClearMission, 1170)
MisResultAction(SetRecord,  1170 )
MisResultAction(GiveItem, 1875, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5681, "", 1171)

MisBeginTalk("<t>,,,...")

MisBeginCondition(NoMission,1171)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(NoRecord,1171)
MisBeginAction(AddMission,1171)
MisCancelAction(ClearMission, 1171)

MisNeed(MIS_NEED_DESP,"Reached Level 65")
MisHelpTalk("<t>,65.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1171)
MisResultCondition(NoRecord,1171)
MisResultCondition(LvCheck, ">", 64 )
MisResultAction(ClearMission, 1171)
MisResultAction(SetRecord,  1171 )
MisResultAction(GiveItem, 1876, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5682, "", 1172)

MisBeginTalk("<t>..,,.")

MisBeginCondition(NoMission,1172)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(NoRecord,1172)
MisBeginAction(AddMission,1172)
MisCancelAction(ClearMission, 1172)

MisNeed(MIS_NEED_DESP,"700")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1172)
MisResultCondition(NoRecord,1172)
MisResultCondition(HasHonorPoint,700 )
MisResultAction(TakeHonorPoint, 700 )
MisResultAction(ClearMission, 1172)
MisResultAction(SetRecord,  1172 )
MisResultAction(GiveItem, 1877, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5683, "", 1173)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1173)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(NoRecord,1173)
MisBeginAction(AddMission,1173)
MisBeginAction(AddTrigger, 11731, TE_GETITEM, 1346, 15 )-------------------15
MisBeginAction(AddTrigger, 11732, TE_GETITEM, 4526, 15 )-------------------15          
MisBeginAction(AddTrigger, 11733, TE_GETITEM, 1608, 15 )-----------------15               
MisBeginAction(AddTrigger, 11734, TE_GETITEM, 4495, 2 )----------------2               
MisBeginAction(AddTrigger, 11735, TE_GETITEM, 1612, 30)------------------30            
MisBeginAction(AddTrigger, 11736, TE_GETITEM, 2724, 20 )-----------------LV2 20                  
MisBeginAction(AddTrigger, 11737, TE_GETITEM, 3094, 10 )------------------10          
MisCancelAction(ClearMission, 1173)


MisNeed(MIS_NEED_ITEM, 1346, 15, 1, 15 )
MisNeed(MIS_NEED_ITEM, 4526, 15, 16, 15 )
MisNeed(MIS_NEED_ITEM, 1608, 15, 31, 15 )
MisNeed(MIS_NEED_ITEM, 4495,  2, 46, 2 )
MisNeed(MIS_NEED_ITEM, 1612, 30, 48, 30 )
MisNeed(MIS_NEED_ITEM, 2724, 20, 78, 20 )
MisNeed(MIS_NEED_ITEM, 3094, 10, 98, 10 )


MisHelpTalk("<t>,")
MisResultTalk("<t>,,,..")

MisResultCondition(HasMission, 1173)
MisResultCondition(NoRecord,1173)
MisResultCondition(HasItem, 1346, 15 )
MisResultCondition(HasItem, 4526, 15 )
MisResultCondition(HasItem, 1608, 15 )
MisResultCondition(HasItem, 4495, 2 )
MisResultCondition(HasItem, 1612, 30 )
MisResultCondition(HasItem, 2724, 20 )
MisResultCondition(HasItem, 3094, 10 )


MisResultAction(TakeItem, 1346, 15 )
MisResultAction(TakeItem, 4526, 15 )
MisResultAction(TakeItem, 1608, 15 )
MisResultAction(TakeItem, 4495, 2 )
MisResultAction(TakeItem, 1612, 30 )
MisResultAction(TakeItem, 2724, 20 )
MisResultAction(TakeItem, 3094, 10 )

MisResultAction(ClearMission, 1173)
MisResultAction(SetRecord,  1173 )
MisResultAction(GiveItem, 1878, 1, 4)
MisResultAction(GiveItem, 1882, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1346)	
TriggerAction( 1, AddNextFlag, 1173, 1, 15 )
RegCurTrigger( 11731 )

InitTrigger()
TriggerCondition( 1, IsItem, 4526)	
TriggerAction( 1, AddNextFlag, 1173, 16, 15 )
RegCurTrigger( 11732 )

InitTrigger()
TriggerCondition( 1, IsItem, 1608)	
TriggerAction( 1, AddNextFlag, 1173, 31, 15 )
RegCurTrigger( 11733 )

InitTrigger()
TriggerCondition( 1, IsItem, 4495)	
TriggerAction( 1, AddNextFlag, 1173, 46, 2 )
RegCurTrigger( 11734 )

InitTrigger()
TriggerCondition( 1, IsItem, 1612)	
TriggerAction( 1, AddNextFlag, 1173, 48, 30 )
RegCurTrigger( 11735 )

InitTrigger()
TriggerCondition( 1, IsItem, 2724)	
TriggerAction( 1, AddNextFlag, 1173,78, 20 )
RegCurTrigger( 11736 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1173, 98, 10 )
RegCurTrigger( 11737 )


-----------------------------------------------------------	
DefineMission (5684, "", 1174)

MisBeginTalk("<t>,,.? ")

MisBeginCondition(NoMission,1174)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(NoRecord,1174)
MisBeginAction(AddMission,1174)
MisCancelAction(ClearMission, 1174)

MisNeed(MIS_NEED_DESP,"15")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1174)
MisResultCondition(NoRecord,1174)
MisResultCondition(HasFightingPoint,15 )
MisResultAction(TakeFightingPoint, 15 )
MisResultAction(ClearMission, 1174)
MisResultAction(SetRecord,  1174 )
MisResultAction(GiveItem, 1874, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5685, "", 1175)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1175)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(NoRecord,1175)
MisBeginAction(AddMission,1175)
MisCancelAction(ClearMission, 1175)

MisNeed(MIS_NEED_DESP,"8000")
MisHelpTalk("<t>,?8000?")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1175)
MisResultCondition(NoRecord,1175)
MisResultCondition(HasCredit,8000 )
MisResultAction(TakeCredit, 8000 )
MisResultAction(ClearMission, 1175)
MisResultAction(SetRecord,  1175 )
MisResultAction(GiveItem, 1875, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5686, "", 1176)

MisBeginTalk("<t>,,,....")

MisBeginCondition(NoMission,1176)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(NoRecord,1176)
MisBeginAction(AddMission,1176)
MisCancelAction(ClearMission, 1176)

MisNeed(MIS_NEED_DESP,"70")
MisHelpTalk("<t>,70.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1176)
MisResultCondition(NoRecord,1176)
MisResultCondition(LvCheck, ">", 69 )
MisResultAction(ClearMission, 1176)
MisResultAction(SetRecord,  1176 )
MisResultAction(GiveItem, 1876, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	
DefineMission (5687, "", 1177)

MisBeginTalk("<t>..,,.")

MisBeginCondition(NoMission,1177)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(NoRecord,1177)
MisBeginAction(AddMission,1177)
MisCancelAction(ClearMission, 1177)

MisNeed(MIS_NEED_DESP,"1000")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1177)
MisResultCondition(NoRecord,1177)
MisResultCondition(HasHonorPoint,1000 )
MisResultAction(TakeHonorPoint, 1000 )
MisResultAction(ClearMission, 1177)
MisResultAction(SetRecord,  1177 )
MisResultAction(GiveItem, 1877, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	
DefineMission (5688, "", 1178)

MisBeginTalk("<t>,.")

MisBeginCondition(NoMission,1178)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(NoRecord,1178)
MisBeginAction(AddMission,1178)
MisBeginAction(AddTrigger, 11781, TE_GETITEM, 1346, 20 )---------------------20 
MisBeginAction(AddTrigger, 11782, TE_GETITEM, 3433, 1 )---------------------1          
MisBeginAction(AddTrigger, 11783, TE_GETITEM, 4495, 3 )-------------------3               
MisBeginAction(AddTrigger, 11784, TE_GETITEM, 1612, 30 )------------------30             
MisBeginAction(AddTrigger, 11785, TE_GETITEM, 2724, 30 )--------------------LV2 30             
MisBeginAction(AddTrigger, 11786, TE_GETITEM, 3094, 30 )-------------------30                              
MisCancelAction(ClearMission, 1178)						                                     


MisNeed(MIS_NEED_ITEM, 1346, 20, 1, 20 )
MisNeed(MIS_NEED_ITEM, 3433, 1, 21, 1 )
MisNeed(MIS_NEED_ITEM, 4495, 3, 22, 3 )
MisNeed(MIS_NEED_ITEM, 1612, 30, 25, 30 )
MisNeed(MIS_NEED_ITEM, 2724, 30, 55, 30)
MisNeed(MIS_NEED_ITEM, 3094, 30, 85, 30 )


MisHelpTalk("<t>,")
MisResultTalk("<t>,,,.")

MisResultCondition(HasMission, 1178)
MisResultCondition(NoRecord,1178)
MisResultCondition(HasItem, 1346, 20 )
MisResultCondition(HasItem, 3433, 1 )
MisResultCondition(HasItem, 4495, 3 )
MisResultCondition(HasItem, 1612, 30 )
MisResultCondition(HasItem, 2724, 30 )
MisResultCondition(HasItem, 3094, 30 )


MisResultAction(TakeItem, 1346, 20 )
MisResultAction(TakeItem, 3433, 1 )
MisResultAction(TakeItem, 4495, 3 )
MisResultAction(TakeItem, 1612, 30 )
MisResultAction(TakeItem, 2724, 30 )
MisResultAction(TakeItem, 3094, 30 )

MisResultAction(ClearMission, 1178)
MisResultAction(SetRecord,  1178 )
MisResultAction(GiveItem, 1878, 1, 4)
MisResultAction(GiveItem, 1882, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1346)	
TriggerAction( 1, AddNextFlag, 1178, 1, 20 )
RegCurTrigger( 11781 )

InitTrigger()
TriggerCondition( 1, IsItem, 3433)	
TriggerAction( 1, AddNextFlag, 1178, 21, 1 )
RegCurTrigger( 11782 )

InitTrigger()
TriggerCondition( 1, IsItem, 4495)	
TriggerAction( 1, AddNextFlag, 1178, 22, 3 )
RegCurTrigger( 11783 )

InitTrigger()
TriggerCondition( 1, IsItem, 1612)	
TriggerAction( 1, AddNextFlag, 1178, 25, 30 )
RegCurTrigger( 11784 )

InitTrigger()
TriggerCondition( 1, IsItem, 2724)	
TriggerAction( 1, AddNextFlag, 1178, 55, 30 )
RegCurTrigger( 11785 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1178, 85, 30 )
RegCurTrigger( 11786 )

--------------------------------------------------------------------
DefineMission( 5689, "", 1179 )
MisBeginTalk("<t>,??,.,.")
			
MisBeginCondition(NoMission, 1179)
MisBeginCondition(NoRecord,1179)
MisBeginCondition(HasRecord, 1162)-------------------id,id
MisBeginAction(AddMission,1179)
MisCancelAction(ClearMission, 1179)

MisNeed(MIS_NEED_DESP,"(711,1414)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
----------------------------------------------------------

DefineMission(5690, "TOP Ambassador", 1179, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,.")
MisResultCondition(NoRecord, 1179)
MisResultCondition(HasMission,1179)
MisResultAction(ClearMission,1179)
MisResultAction(SetRecord, 1179)

----------------------------------------------------------2----------
DefineMission( 5691, "TOP Ambassador 2", 1180 )
MisBeginTalk("<t>18,...,")
			
MisBeginCondition(NoMission, 1180)
MisBeginCondition(NoRecord,1180)
MisBeginCondition(HasRecord, 1179)
MisBeginAction(AddMission,1180)
MisCancelAction(ClearMission, 1180)

MisNeed(MIS_NEED_DESP,"(958,3549)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------2-------------
DefineMission(5692, "TOP Ambassador 2", 1180, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>!,?")
MisResultCondition(NoRecord, 1180)
MisResultCondition(HasMission,1180)
MisResultAction(ClearMission,1180)
MisResultAction(SetRecord, 1180)

----------------------------------------------------------3----------
DefineMission( 5693, "TOP Ambassador 3", 1181 )
MisBeginTalk("<t>?,.,")
			
MisBeginCondition(NoMission, 1181)
MisBeginCondition(NoRecord,1181)
MisBeginCondition(HasRecord, 1180)
MisBeginAction(AddMission,1181)
MisCancelAction(ClearMission, 1181)

MisNeed(MIS_NEED_DESP,"(2138,545)")

MisHelpTalk("<t>,..")
MisResultCondition(AlwaysFailure)	
-----------------------------------------3--------
DefineMission(5694, "TOP Ambassador 3", 1181, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,.")
MisResultCondition(NoRecord, 1181)
MisResultCondition(HasMission,1181)
MisResultAction(ClearMission,1181)
MisResultAction(SetRecord, 1181)



----------------------------------------------------------4----------
DefineMission( 5695, "TOP Ambassador 4", 1182 )
MisBeginTalk("<t>,.")
			
MisBeginCondition(NoMission, 1182)
MisBeginCondition(NoRecord,1182)
MisBeginCondition(HasRecord, 1181)
MisBeginAction(AddMission,1182)
MisCancelAction(ClearMission, 1182)

MisNeed(MIS_NEED_DESP,"(2065,2732)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------4--------
DefineMission(5696, "TOP Ambassador 4", 1182, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,,..")
MisResultCondition(NoRecord, 1182)
MisResultCondition(HasMission,1182)
MisResultAction(ClearMission,1182)
MisResultAction(SetRecord, 1182)


----------------------------------------------------------5----------
DefineMission( 5697, "TOP Ambassador 5", 1183 )
MisBeginTalk("<t>?,,,..,,.")
			
MisBeginCondition(NoMission, 1183)
MisBeginCondition(NoRecord,1183)
MisBeginCondition(HasRecord, 1182)
MisBeginAction(AddMission,1183)
MisCancelAction(ClearMission, 1183)

MisNeed(MIS_NEED_DESP,"(1131,3153)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------5--------
DefineMission(5698, "TOP Ambassador 5", 1183, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>??")
MisResultCondition(NoRecord, 1183)
MisResultCondition(HasMission,1183)
MisResultAction(ClearMission,1183)
MisResultAction(SetRecord, 1183)


----------------------------------------------------------6----------
DefineMission( 5699, "TOP Ambassador 6", 1184 )
MisBeginTalk("<t>,,,..")
			
MisBeginCondition(NoMission, 1184)
MisBeginCondition(NoRecord,1184)
MisBeginCondition(HasRecord, 1183)
MisBeginAction(AddMission,1184)
MisCancelAction(ClearMission, 1184)

MisNeed(MIS_NEED_DESP,"(798,369)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------6--------
DefineMission(5700, "TOP Ambassador 6", 1184, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.")
MisResultCondition(NoRecord, 1184)
MisResultCondition(HasMission,1184)
MisResultAction(ClearMission,1184)
MisResultAction(SetRecord, 1184)


----------------------------------------------------------7----------
DefineMission( 5701, "TOP Ambassador 7", 1185 )
MisBeginTalk("<t>,.")
			
MisBeginCondition(NoMission, 1185)
MisBeginCondition(NoRecord,1185)
MisBeginCondition(HasRecord, 1184)
MisBeginAction(AddMission,1185)
MisCancelAction(ClearMission, 1185)

MisNeed(MIS_NEED_DESP,"(1507,3105)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------7--------
DefineMission(5702, "TOP Ambassador 7", 1185, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?..")
MisResultCondition(NoRecord, 1185)
MisResultCondition(HasMission,1185)
MisResultAction(ClearMission,1185)
MisResultAction(SetRecord, 1185)


----------------------------------------------------------8----------
DefineMission( 5703, "TOP Ambassador 8", 1186 )
MisBeginTalk("<t>,,.,,.?.")
			
MisBeginCondition(NoMission, 1186)
MisBeginCondition(NoRecord,1186)
MisBeginCondition(HasRecord, 1185)
MisBeginAction(AddMission,1186)
MisCancelAction(ClearMission, 1186)

MisNeed(MIS_NEED_DESP,"(2372,737)")

MisHelpTalk("<t>!")
MisResultCondition(AlwaysFailure)	
-----------------------------------------8--------
DefineMission(5704, "TOP Ambassador 8", 1186, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,.,.")
MisResultCondition(NoRecord, 1186)
MisResultCondition(HasMission,1186)
MisResultAction(ClearMission,1186)
MisResultAction(SetRecord, 1186)


----------------------------------------------------------9----------
DefineMission( 5705, "TOP Ambassador 9", 1187 )
MisBeginTalk("<t>,,.,....")
			
MisBeginCondition(NoMission, 1187)
MisBeginCondition(NoRecord,1187)
MisBeginCondition(HasRecord, 1186)
MisBeginAction(AddMission,1187)
MisCancelAction(ClearMission, 1187)

MisNeed(MIS_NEED_DESP,"(1010,350)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------9--------
DefineMission(5706, "TOP Ambassador 9", 1187, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?.")
MisResultCondition(NoRecord, 1187)
MisResultCondition(HasMission,1187)
MisResultAction(ClearMission,1187)
MisResultAction(SetRecord, 1187)

----------------------------------------------------------10----------
DefineMission( 5707, "TOP Ambassador 10", 1188 )
MisBeginTalk("<t>??...?,,..")
			
MisBeginCondition(NoMission, 1188)
MisBeginCondition(NoRecord,1188)
MisBeginCondition(HasRecord, 1187)
MisBeginAction(AddMission,1188)
MisCancelAction(ClearMission, 1188)

MisNeed(MIS_NEED_DESP,"(1136,2778)")

MisHelpTalk("<t>...")
MisResultCondition(AlwaysFailure)	
-----------------------------------------10--------
DefineMission(5708, "TOP Ambassador 10", 1188, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?!")
MisResultCondition(NoRecord, 1188)
MisResultCondition(HasMission,1188)
MisResultAction(ClearMission,1188)
MisResultAction(SetRecord, 1188)

----------------------------------------------------------11----------
DefineMission( 5709, "TOP Ambassador 11", 1189 )
MisBeginTalk("<t>Party,.,,,?.")
			
MisBeginCondition(NoMission, 1189)
MisBeginCondition(NoRecord,1189)
MisBeginCondition(HasRecord, 1188)
MisBeginAction(AddMission,1189)
MisCancelAction(ClearMission, 1189)

MisNeed(MIS_NEED_DESP,"(1739,3748)")

MisHelpTalk("<t>..")
MisResultCondition(AlwaysFailure)	
-----------------------------------------11--------
DefineMission(5710, "TOP Ambassador 11", 1189, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>!,...")
MisResultCondition(NoRecord, 1189)
MisResultCondition(HasMission,1189)
MisResultAction(ClearMission,1189)
MisResultAction(SetRecord, 1189)

----------------------------------------------------------12----------
DefineMission( 5711, "TOP Ambassador 12", 1190 )
MisBeginTalk("<t>.,.?,,.")
			
MisBeginCondition(NoMission, 1190)
MisBeginCondition(NoRecord,1190)
MisBeginCondition(HasRecord, 1189)
MisBeginAction(AddMission,1190)
MisCancelAction(ClearMission, 1190)

MisNeed(MIS_NEED_DESP,"(2673,657)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------12--------
DefineMission(5712, "TOP Ambassador 12", 1190, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,..")
MisResultCondition(NoRecord, 1190)
MisResultCondition(HasMission,1190)
MisResultAction(ClearMission,1190)
MisResultAction(SetRecord, 1190)


----------------------------------------------------------13----------
DefineMission( 5713, "TOP Ambassador 13", 1191 )
MisBeginTalk("<t>.,.")
			
MisBeginCondition(NoMission, 1191)
MisBeginCondition(NoRecord,1191)
MisBeginCondition(HasRecord, 1190)
MisBeginAction(AddMission,1191)
MisCancelAction(ClearMission, 1191)

MisNeed(MIS_NEED_DESP,"(626,2100)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------13--------
DefineMission(5714, "TOP Ambassador 13", 1191, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,.")
MisResultCondition(NoRecord, 1191)
MisResultCondition(HasMission,1191)
MisResultAction(ClearMission,1191)
MisResultAction(SetRecord, 1191)

----------------------------------------------------------14----------
DefineMission( 5715, "TOP Ambassador 14", 1192 )
MisBeginTalk("<t>?,,.,.")
			
MisBeginCondition(NoMission, 1192)
MisBeginCondition(NoRecord,1192)
MisBeginCondition(HasRecord, 1191)
MisBeginAction(AddMission,1192)
MisCancelAction(ClearMission, 1192)

MisNeed(MIS_NEED_DESP,"Talk to Trader - Yuka (2519, 2397) at Cupid Isle ")

MisHelpTalk("<t>,?")
MisResultCondition(AlwaysFailure)	
-----------------------------------------14--------        
DefineMission(5716, "TOP Ambassador 14", 1192, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,..")
MisResultCondition(NoRecord, 1192)
MisResultCondition(HasMission,1192)
MisResultAction(ClearMission,1192)
MisResultAction(SetRecord, 1192)

----------------------------------------------------------15----------
DefineMission( 5717, "TOP Ambassador 15", 1193 )
MisBeginTalk("<t>,,.")
			
MisBeginCondition(NoMission, 1193)
MisBeginCondition(NoRecord,1193)
MisBeginCondition(HasRecord, 1192)
MisBeginAction(AddMission,1193)
MisCancelAction(ClearMission, 1193)

MisNeed(MIS_NEED_DESP,"(1059,661)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------15-------- 
DefineMission(5718, "TOP Ambassador 15", 1193, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.")
MisResultCondition(NoRecord, 1193)
MisResultCondition(HasMission,1193)
MisResultAction(ClearMission,1193)
MisResultAction(SetRecord, 1193)


----------------------------------------------------------16----------
DefineMission( 5719, "TOP Ambassador 16", 1194 )
MisBeginTalk("<t>?,,.")
			
MisBeginCondition(NoMission, 1194)
MisBeginCondition(NoRecord,1194)
MisBeginCondition(HasRecord, 1193)
MisBeginAction(AddMission,1194)
MisCancelAction(ClearMission, 1194)

MisNeed(MIS_NEED_DESP,"(1907,2798)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------16-------- 
DefineMission(5720, "TOP Ambassador 16", 1194, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,.")
MisResultCondition(NoRecord, 1194)
MisResultCondition(HasMission,1194)
MisResultAction(ClearMission,1194)
MisResultAction(SetRecord, 1194)


----------------------------------------------------------17----------
DefineMission( 5721, "TOP Ambassador 17", 1195 )
MisBeginTalk("<t>.,.")
			
MisBeginCondition(NoMission, 1195)
MisBeginCondition(NoRecord,1195)
MisBeginCondition(HasRecord, 1194)
MisBeginAction(AddMission,1195)
MisCancelAction(ClearMission, 1195)

MisNeed(MIS_NEED_DESP,"(1244,3186)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------17-------- 
DefineMission(5722, "TOP Ambassador 17", 1195, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>!.,.")
MisResultCondition(NoRecord, 1195)
MisResultCondition(HasMission,1195)
MisResultAction(ClearMission,1195)
MisResultAction(SetRecord, 1195)

----------------------------------------------------------18----------
DefineMission( 5723, "TOP Ambassador 18", 1196 )
MisBeginTalk("<t>,?,.,..")
			
MisBeginCondition(NoMission, 1196)
MisBeginCondition(NoRecord,1196)
MisBeginCondition(HasRecord, 1195)
MisBeginAction(AddMission,1196)
MisCancelAction(ClearMission, 1196)

MisNeed(MIS_NEED_DESP,"Talk to Harbor Operator - Ramus (2297, 3723) at Muse Haven.")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------18-------- 
DefineMission(5724, "TOP Ambassador 18", 1196, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.,.....")
MisResultCondition(NoRecord, 1196)
MisResultCondition(HasMission,1196)
MisResultAction(ClearMission,1196)
MisResultAction(SetRecord, 1196)

----------------------------------------------------------19----------
DefineMission( 5725, "TOP Ambassador 19", 1197 )
MisBeginTalk("<t>,...")
			
MisBeginCondition(NoMission, 1197)
MisBeginCondition(NoRecord,1197)
MisBeginCondition(HasRecord, 1196)
MisBeginAction(AddMission,1197)
MisCancelAction(ClearMission, 1197)

MisNeed(MIS_NEED_DESP,"Look for Heaven Teleporter at (474, 1054).")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------19-------- 
DefineMission(5726, "TOP Ambassador 19", 1197, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.")
MisResultCondition(NoRecord, 1197)
MisResultCondition(HasMission,1197)
MisResultAction(ClearMission,1197)
MisResultAction(SetRecord, 1197)


	----------------------------------------------------------20----------
DefineMission( 5727, "TOP Ambassador 20", 1198 )
MisBeginTalk("<t>,,.")
			
MisBeginCondition(NoMission, 1198)
MisBeginCondition(NoRecord,1198)
MisBeginCondition(HasRecord, 1197)
MisBeginAction(AddMission,1198)
MisCancelAction(ClearMission, 1198)

MisNeed(MIS_NEED_DESP,"(1003,1306)")

MisHelpTalk("<t>Thank you, go now.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------20-------- 
DefineMission(5728, "TOP Ambassador 20", 1198, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,.")
MisResultCondition(NoRecord, 1198)
MisResultCondition(HasMission,1198)
MisResultAction(ClearMission,1198)
MisResultAction(SetRecord, 1198)

	----------------------------------------------------------21----------
DefineMission( 5729, "TOP Ambassador 21", 1199 )
MisBeginTalk("<t>,,.,.")
			
MisBeginCondition(NoMission, 1199)
MisBeginCondition(NoRecord,1199)
MisBeginCondition(HasRecord, 1198)
MisBeginAction(AddMission,1199)
MisCancelAction(ClearMission, 1199)

MisNeed(MIS_NEED_DESP,"(794,3669)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------21------- 
DefineMission(5730, "Community Hero 21", 1199, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.")
MisResultCondition(NoRecord, 1199)
MisResultCondition(HasMission,1199)
MisResultAction(ClearMission,1199)
MisResultAction(SetRecord, 1199)
MisResultAction(GiveItem, 1879, 1, 4)
MisResultBagNeed(1)


-------------------------------------------------	--------
DefineMission (5731, "", 1200)

MisBeginTalk("<t>,,.<b>?,.")

MisBeginCondition(NoMission,1200)
MisBeginCondition(NoRecord,1200)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(HasRecord,1174)
MisBeginCondition(HasRecord,1175)
MisBeginCondition(HasRecord,1176)
MisBeginCondition(HasRecord,1177)
MisBeginCondition(HasRecord,1178)
MisBeginCondition(HasRecord,1199)
MisBeginCondition(HasRecord,1168)
MisBeginAction(AddMission,1200)
MisBeginAction(AddTrigger, 12001, TE_GETITEM, 0854, 1 )--------------------- 
MisCancelAction(ClearMission, 1200)


MisNeed(MIS_NEED_ITEM, 0854, 1, 1, 1 )
MisHelpTalk("<t>,...")

MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1200)
MisResultCondition(NoRecord,1200)
MisResultCondition(HasItem, 0854, 1 )
MisResultAction(TakeItem, 0854, 1 )
MisResultAction(GiveItem, 1881, 1, 4)
MisResultAction(ClearMission, 1200)
MisResultAction(SetRecord,  1200 )
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 0854)	
TriggerAction( 1, AddNextFlag, 1200, 1, 1 )
RegCurTrigger( 12001 )	



-----------------------------------------------------------	
DefineMission (5733, "", 1201)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1201)
MisBeginCondition(HasRecord,1159)
MisBeginCondition(HasRecord,1163)
MisBeginCondition(HasRecord,1164)
MisBeginCondition(HasRecord,1165)
MisBeginCondition(HasRecord,1166)
MisBeginCondition(HasRecord,1167)
MisBeginCondition(HasRecord,1168)
MisBeginCondition(HasRecord,1199)
MisBeginCondition(NoRecord,1201)
MisBeginAction(AddMission,1201)  
MisBeginAction(AddTrigger, 12011, TE_GETITEM, 1874, 1 )		
MisBeginAction(AddTrigger, 12012, TE_GETITEM, 1875, 1 )		
MisBeginAction(AddTrigger, 12013, TE_GETITEM, 1876, 1 )		
MisBeginAction(AddTrigger, 12014, TE_GETITEM, 1877, 1 )		
MisBeginAction(AddTrigger, 12015, TE_GETITEM, 1878, 1 )		
MisBeginAction(AddTrigger, 12016, TE_GETITEM, 1879, 1 )		
MisBeginAction(AddTrigger, 12017, TE_GETITEM, 1880, 1 )	
MisCancelAction(ClearMission, 1201)						                                     

MisNeed(MIS_NEED_ITEM, 1874, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 1875, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 1876, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 1877, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 1878, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 1879, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 1880, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1201)
MisResultCondition(NoRecord,1201)
MisResultCondition(HasItem, 1874, 1 )
MisResultCondition(HasItem, 1875, 1 )
MisResultCondition(HasItem, 1876, 1 )
MisResultCondition(HasItem, 1877, 1 )
MisResultCondition(HasItem, 1878, 1 )
MisResultCondition(HasItem, 1879, 1 )
MisResultCondition(HasItem, 1880, 1 )

MisResultAction(TakeItem, 1874, 1 )
MisResultAction(TakeItem, 1875, 1 )
MisResultAction(TakeItem, 1876, 1 )
MisResultAction(TakeItem, 1877, 1 )
MisResultAction(TakeItem, 1878, 1 )
MisResultAction(TakeItem, 1879, 1 )
MisResultAction(TakeItem, 1880, 1 )

MisResultAction(ClearMission, 1201)
MisResultAction(SetRecord,  1201 )
MisResultAction(GiveItem, 1865, 1, 4)
MisResultAction(GiveItem, 1866, 1, 4)
MisResultAction(GiveItem, 1002, 30, 4)
MisResultAction(AddMoney,500000,500000)
MisResultAction(ShuangZiSS)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 1874)	
TriggerAction( 1, AddNextFlag, 1201, 10, 1 )
RegCurTrigger( 12011 )

InitTrigger()
TriggerCondition( 1, IsItem, 1875)	
TriggerAction( 1, AddNextFlag, 1201, 20, 1 )
RegCurTrigger( 12012 )

InitTrigger()
TriggerCondition( 1, IsItem, 1876)	
TriggerAction( 1, AddNextFlag, 1201, 30, 1 )
RegCurTrigger( 12013 )

InitTrigger()
TriggerCondition( 1, IsItem, 1877)	
TriggerAction( 1, AddNextFlag, 1201, 40, 1 )
RegCurTrigger( 12014 )

InitTrigger()
TriggerCondition( 1, IsItem, 1878)	
TriggerAction( 1, AddNextFlag, 1201, 50, 1 )
RegCurTrigger( 12015 )

InitTrigger()
TriggerCondition( 1, IsItem, 1879)	
TriggerAction( 1, AddNextFlag, 1201, 60, 1 )
RegCurTrigger( 12016 )

InitTrigger()
TriggerCondition( 1, IsItem, 1880)	
TriggerAction( 1, AddNextFlag, 1201, 70, 1 )
RegCurTrigger( 12017 )
--------------------------------------------------------	
DefineMission (5734, "", 1202)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1202)
MisBeginCondition(HasRecord,1204)
MisBeginCondition(HasRecord,1169)
MisBeginCondition(HasRecord,1170)
MisBeginCondition(HasRecord,1171)
MisBeginCondition(HasRecord,1172)
MisBeginCondition(HasRecord,1173)
MisBeginCondition(HasRecord,1168)
MisBeginCondition(HasRecord,1199)
MisBeginCondition(NoRecord,1202)
MisBeginAction(AddMission,1202)
MisBeginAction(AddTrigger, 11571, TE_GETITEM, 1874, 1 )		
MisBeginAction(AddTrigger, 11572, TE_GETITEM, 1875, 1 )		
MisBeginAction(AddTrigger, 11573, TE_GETITEM, 1876, 1 )		
MisBeginAction(AddTrigger, 11574, TE_GETITEM, 1877, 1 )		
MisBeginAction(AddTrigger, 11575, TE_GETITEM, 1878, 1 )		
MisBeginAction(AddTrigger, 11576, TE_GETITEM, 1879, 1 )		
MisBeginAction(AddTrigger, 11577, TE_GETITEM, 1880, 1 )		
MisCancelAction(ClearMission, 1202)						                                     

MisNeed(MIS_NEED_ITEM, 1874, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 1875, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 1876, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 1877, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 1878, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 1879, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 1880, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1202)
MisResultCondition(NoRecord,1202)
MisResultCondition(HasItem, 1874, 1 )
MisResultCondition(HasItem, 1875, 1 )
MisResultCondition(HasItem, 1876, 1 )
MisResultCondition(HasItem, 1877, 1 )
MisResultCondition(HasItem, 1878, 1 )
MisResultCondition(HasItem, 1879, 1 )
MisResultCondition(HasItem, 1880, 1 )

MisResultAction(TakeItem, 1874, 1 )
MisResultAction(TakeItem, 1875, 1 )
MisResultAction(TakeItem, 1876, 1 )
MisResultAction(TakeItem, 1877, 1 )
MisResultAction(TakeItem, 1878, 1 )
MisResultAction(TakeItem, 1879, 1 )
MisResultAction(TakeItem, 1880, 1 )

MisResultAction(ClearMission, 1202)
MisResultAction(SetRecord,  1202 )
MisResultAction(GiveItem, 1865, 1, 4)
MisResultAction(GiveItem, 1866, 1, 4)
MisResultAction(GiveItem, 1002, 60, 4)
MisResultAction(AddMoney,700000,700000)
MisResultAction(ShuangZiHD)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 1874)	
TriggerAction( 1, AddNextFlag, 1157, 10, 1 )
RegCurTrigger( 12021 )

InitTrigger()
TriggerCondition( 1, IsItem, 1875)	
TriggerAction( 1, AddNextFlag, 1202, 20, 1 )
RegCurTrigger( 12022 )

InitTrigger()
TriggerCondition( 1, IsItem, 1876)	
TriggerAction( 1, AddNextFlag, 1202, 30, 1 )
RegCurTrigger( 12023 )

InitTrigger()
TriggerCondition( 1, IsItem, 1877)	
TriggerAction( 1, AddNextFlag, 1202, 40, 1 )
RegCurTrigger( 12024 )

InitTrigger()
TriggerCondition( 1, IsItem, 1878)	
TriggerAction( 1, AddNextFlag, 1202, 50, 1 )
RegCurTrigger( 12025 )

InitTrigger()
TriggerCondition( 1, IsItem, 1879)	
TriggerAction( 1, AddNextFlag, 1202, 60, 1 )
RegCurTrigger( 12026 )

InitTrigger()
TriggerCondition( 1, IsItem, 1880)	
TriggerAction( 1, AddNextFlag, 1202, 70, 1 )
RegCurTrigger( 12027 )

------------------------------------------------------	
DefineMission (5735, "", 1203)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1203)
MisBeginCondition(HasRecord,1161)
MisBeginCondition(HasRecord,1174)
MisBeginCondition(HasRecord,1175)
MisBeginCondition(HasRecord,1176)
MisBeginCondition(HasRecord,1177)
MisBeginCondition(HasRecord,1178)
MisBeginCondition(HasRecord,1199)
MisBeginCondition(HasRecord,1168)
MisBeginCondition(NoRecord,1203)
MisBeginAction(AddMission,1203)   
MisBeginAction(AddTrigger, 11581, TE_GETITEM, 1874, 1 )		
MisBeginAction(AddTrigger, 11582, TE_GETITEM, 1875, 1 )		
MisBeginAction(AddTrigger, 11583, TE_GETITEM, 1876, 1 )		
MisBeginAction(AddTrigger, 11584, TE_GETITEM, 1877, 1 )		
MisBeginAction(AddTrigger, 11585, TE_GETITEM, 1878, 1 )		
MisBeginAction(AddTrigger, 11586, TE_GETITEM, 1879, 1 )		
MisBeginAction(AddTrigger, 11587, TE_GETITEM, 1880, 1 )	
MisCancelAction(ClearMission, 1203)						                                     

MisNeed(MIS_NEED_ITEM, 1874, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 1875, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 1876, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 1877, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 1878, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 1879, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 1880, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1203)
MisResultCondition(NoRecord,1203)
MisResultCondition(HasItem, 1874, 1 )
MisResultCondition(HasItem, 1875, 1 )
MisResultCondition(HasItem, 1876, 1 )
MisResultCondition(HasItem, 1877, 1 )
MisResultCondition(HasItem, 1878, 1 )
MisResultCondition(HasItem, 1879, 1 )
MisResultCondition(HasItem, 1880, 1 )

MisResultAction(TakeItem, 1874, 1 )
MisResultAction(TakeItem, 1875, 1 )
MisResultAction(TakeItem, 1876, 1 )
MisResultAction(TakeItem, 1877, 1 )
MisResultAction(TakeItem, 1878, 1 )
MisResultAction(TakeItem, 1879, 1 )
MisResultAction(TakeItem, 1880, 1 )

MisResultAction(ClearMission, 1203)
MisResultAction(SetRecord,  1203 )
MisResultAction(GiveItem, 1865, 1, 4)
MisResultAction(GiveItem, 1866, 1, 4)
MisResultAction(GiveItem, 1002, 90, 4)
MisResultAction(AddMoney,900000,900000)
MisResultAction(ShuangZiCZ)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 1874)	
TriggerAction( 1, AddNextFlag, 1203, 10, 1 )
RegCurTrigger( 12031 )

InitTrigger()
TriggerCondition( 1, IsItem, 1875)	
TriggerAction( 1, AddNextFlag, 1203, 20, 1 )
RegCurTrigger( 12032 )

InitTrigger()
TriggerCondition( 1, IsItem, 1876)	
TriggerAction( 1, AddNextFlag, 1203, 30, 1 )
RegCurTrigger( 12033 )

InitTrigger()
TriggerCondition( 1, IsItem, 1877)	
TriggerAction( 1, AddNextFlag, 1203, 40, 1 )
RegCurTrigger( 12034 )

InitTrigger()
TriggerCondition( 1, IsItem, 1878)	
TriggerAction( 1, AddNextFlag, 1203, 50, 1 )
RegCurTrigger( 12035 )

InitTrigger()
TriggerCondition( 1, IsItem, 1879)	
TriggerAction( 1, AddNextFlag, 1203, 60, 1 )
RegCurTrigger( 12036 )

InitTrigger()
TriggerCondition( 1, IsItem, 1880)	
TriggerAction( 1, AddNextFlag, 1203, 70, 1 )
RegCurTrigger( 12037 )

------------------------------------------
DefineMission ( 5736, "Legend of the Fairy Couple", 1205)

MisBeginTalk("<t>,,.?10.")

MisBeginCondition(NoMission,1205)
MisBeginCondition(NoRecord,1205)
MisBeginAction(AddMission,1205)
MisBeginAction(AddTrigger, 12051, TE_GETITEM, 4418, 10)
MisCancelAction(ClearMission, 1205)


MisNeed(MIS_NEED_DESP,"10")
MisNeed(MIS_NEED_ITEM, 4418, 10, 10, 10)

MisHelpTalk("<t>10")
MisResultTalk("<t>,..") 

MisResultCondition(HasMission, 1205)
MisResultCondition(NoRecord,1205)
MisResultCondition(HasItem, 4418, 10)
MisResultAction(TakeItem, 4418, 10 )
MisResultAction(ClearMission, 1205)
MisResultAction(SetRecord, 1205 )


InitTrigger()
TriggerCondition( 1, IsItem, 4418)	
TriggerAction( 1, AddNextFlag, 1205, 10, 10 )
RegCurTrigger( 12051 )

---------------------------------------
DefineMission( 5737, "Magpie Bridge Romance", 1206)

MisBeginTalk( "<t>.............<n><t>!.")
MisBeginCondition(HasRecord, 1205 )
MisBeginCondition(NoRecord, 1206 )
MisBeginCondition(NoMission, 1206 )
MisBeginAction(AddMission, 1206 )
MisCancelAction(ClearMission, 1206)

MisNeed(MIS_NEED_DESP,"(3670,2636).")
MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure )

----------------------------------------------

DefineMission(5738,"Magpie Bridge Romance",1206,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>,!,,.,.")
MisResultCondition(HasMission, 1206)
MisBeginCondition(NoRecord, 1206)
MisResultAction(ClearMission, 1206)
MisResultAction(SetRecord, 1206)


-------------------------------------
DefineMission( 5739, "Letter from a Thousand Miles", 1207)

MisBeginTalk( "<t>,,.?!!")
MisBeginCondition(HasRecord, 1206 )
MisBeginCondition(NoRecord, 1207 )
MisBeginCondition(NoMission, 1207 )
MisBeginAction(AddMission, 1207 )
MisBeginAction(GiveItem, 2669, 1, 4)----------
MisCancelAction(ClearMission, 1207)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"(1599,909).")
MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure )

-------------------------------------------

DefineMission(5740,"Letter from a Thousand Miles",1207,COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure)

MisResultTalk("<t>?,.,..,().<n><t>,.")
MisResultCondition(HasMission, 1207)
MisBeginCondition(NoRecord, 1207)
MisResultCondition(HasItem,2669,1)---------
MisResultAction(TakeItem, 2669,1)
MisResultAction(GiveItem, 2670, 1, 4)----------
MisResultAction(ClearMission, 1207)
MisResultAction(SetRecord, 1207)
MisResultAction(ClearRecord, 1205 )
MisResultAction(ClearRecord, 1206 )
MisResultAction(ClearRecord, 1207 )
MisResultBagNeed(1)

-----------------------------------------------------------------
DefineMission( 6127, "", 1453)
MisBeginTalk( "<t>,?~~,,,2.??,99,.")
MisBeginCondition( NoMission, 1453)
MisBeginCondition( NoRecord, 1453)
MisBeginAction( AddMission, 1453)
MisBeginAction(AddTrigger, 14531, TE_GETITEM, 3909, 99)
MisCancelAction( ClearMission, 1453)

MisNeed( MIS_NEED_DESP, "99(2232,2781).")
MisNeed( MIS_NEED_ITEM, 3909, 99, 10, 99)

MisHelpTalk( "<t>")
MisResultTalk( "<t>,!")
MisResultCondition( HasMission, 1453)
MisResultCondition( NoRecord, 1453)
MisResultCondition( HasItem, 3909, 99)
MisResultAction( ClearMission, 1453)
MisResultAction( TakeItem, 3909, 99)
MisResultAction( SetRecord, 1453)

InitTrigger()
TriggerCondition( 1, IsItem, 3909)	
TriggerAction( 1, AddNextFlag, 1453, 10, 99 )
RegCurTrigger( 14531 )

-----------------------------------------------------------------
DefineMission( 6128, "", 1454)
MisBeginTalk( "<t>,,4,.")
MisBeginCondition( NoMission, 1454)
MisBeginCondition( NoRecord, 1454)
MisBeginCondition( HasRecord, 1453)
MisBeginAction( AddMission, 1454)
MisCancelAction( ClearMission, 1454)

MisNeed( MIS_NEED_DESP, "4(2232,2781)")

MisHelpTalk( "<t>4!")
MisResultTalk( "<t>,,.")
MisResultCondition( HasMission, 1454)
MisResultCondition( NoRecord, 1454)
MisResultCondition( CheckTeam1, 5)					------
MisResultAction( ClearMission, 1454)
MisResultAction( SetRecord, 1454)

-----------------------------------------------------------------

DefineMission( 6129, "", 1455)
MisBeginTalk( "<t>,10.")
MisBeginCondition( NoMission, 1455)
MisBeginCondition( NoRecord, 1455)
MisBeginCondition( HasRecord, 1454)
MisBeginAction( AddMission, 1455)
MisBeginAction(AddTrigger, 14551, TE_KILL, 75, 10 )
MisCancelAction( ClearMission, 1455)

MisNeed( MIS_NEED_DESP, "10(2118,2638).")
MisNeed( MIS_NEED_KILL, 75, 10, 10, 10)

MisHelpTalk( "<t>.")
MisResultTalk( "<t>.")
MisResultCondition( HasMission, 1455)
MisResultCondition( NoRecord, 1455)
MisResultCondition( HasFlag, 1455, 19)
MisResultAction( ClearMission, 1455)
MisResultAction( SetRecord, 1455)

InitTrigger() 
TriggerCondition( 1, IsMonster, 75 )	
TriggerAction( 1, AddNextFlag, 1455, 10, 10 )
RegCurTrigger(14551)

-----------------------------------------------------------------
DefineMission( 6130, "", 1456)
MisBeginTalk( "<t>,,,,,.")
MisBeginCondition( NoMission, 1456)
MisBeginCondition( NoRecord, 1456)
MisBeginCondition( HasRecord, 1455)
MisBeginAction( AddMission, 1456)
MisBeginAction(AddTrigger, 14561, TE_GETITEM, 854, 1)
MisCancelAction( ClearMission, 1456)

MisNeed( MIS_NEED_DESP, "(2232,2781).")
MisNeed( MIS_NEED_ITEM, 854, 1, 10, 1)

MisHelpTalk( "<t>,,")
MisResultTalk( "<t>?.")
MisResultCondition( HasMission, 1456)
MisResultCondition( NoRecord, 1456)
MisResultCondition( HasItem, 854, 1)
MisResultAction( ClearMission, 1456)
MisResultAction( TakeItem, 854, 1)
MisResultAction( GiveItem, 579, 1, 4)
MisResultAction( SetRecord, 1456)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 854)	
TriggerAction( 1, AddNextFlag, 1456, 10, 1 )
RegCurTrigger( 14561 )

-----------------------------------------------------------------
DefineMission( 6131, "", 1457)
MisBeginTalk( "<t>(871,3582)")
MisBeginCondition( NoMission, 1457)
MisBeginCondition( NoRecord, 1457)
MisBeginCondition( HasRecord, 1456)
MisBeginAction( AddMission, 1457)
MisCancelAction( ClearMission, 1457)

MisNeed( MIS_NEED_DESP, "(871,3582)")
MisHelpTalk( "<t>(871,3582),.")
MisResultCondition( AlwaysFailure )

-----------------------------------------------------------------
DefineMission( 6132, "", 1457, COMPLETE_SHOW)

MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>.")
MisResultCondition( HasMission, 1457)
MisResultCondition( NoRecord, 1457)
MisResultCondition( HasItem, 579, 1)
MisResultAction( ClearMission, 1457)
MisResultAction( SetRecord, 1457)

-----------------------------------------------------------------
DefineMission( 6133, "", 1458)
MisBeginTalk( "<t>?,30,.")
MisBeginCondition( NoMission, 1458)
MisBeginCondition( NoRecord, 1458)
MisBeginCondition( HasRecord, 1457)
MisBeginAction( AddMission, 1458)
MisCancelAction( ClearMission, 1458)

MisNeed( MIS_NEED_DESP, "")

MisHelpTalk( "<t>,,.")
MisResultCondition( AlwaysFailure )

-----------------------------------------------------------------
DefineMission( 6134, "", 1458, COMPLETE_SHOW)

MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>,,,,.")
MisResultCondition( HasMission, 1458)
MisResultCondition( NoRecord, 1463)
MisResultCondition( HasRecord, 1461)
MisResultCondition( CheckRightNum )
MisResultAction( SetRecord, 1463)
MisResultAction( SetRecord, 1458)
MisResultAction( TakeItem, 579, 1)
MisResultAction( GiveItem, 47, 1, 4)
MisResultAction( ClearMission, 1458)
MisResultBagNeed(1)

-----------------------------------------------------------------
DefineMission( 6135, "", 1458, COMPLETE_SHOW)

MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>,,.")
MisResultCondition( HasMission, 1458)
MisResultCondition( NoRecord, 1463)
MisResultCondition( HasRecord, 1461)
MisResultCondition( CheckErroNum )
MisResultAction( SetRecord, 1463)
MisResultAction( SetRecord, 1458)
MisResultAction( SetRecord, 1460)
MisResultAction( TakeItem, 579, 1)
MisResultAction( ClearMission, 1458)

-----------------------------------------------------------------
DefineMission( 6136, "", 1464)
MisBeginTalk( "<t>.,3,,!!,10")
MisBeginCondition( NoMission, 1464)
MisBeginCondition( NoRecord, 1464)
MisBeginCondition( HasRecord, 1460)
MisBeginAction( AddMission, 1464)
MisBeginAction(AddTrigger, 14641, TE_GETITEM, 3050, 10)
MisBeginAction( GiveItem, 500, 1, 4)
MisCancelAction( ClearMission, 1464)
MisBeginBagNeed(1)

MisNeed( MIS_NEED_DESP, ",10,")
MisHelpTalk( "<t>,")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------------
DefineMission( 6137, "", 1464, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>,,,.")
MisResultCondition( HasMission, 1464)
MisResultCondition( NoRecord, 1464)
MisResultCondition( HasRecord, 1465)
MisResultCondition( HasItem, 500, 1)
MisResultCondition( HasItem, 3050, 10)
MisResultAction( SetRecord, 1464)
MisResultAction( TakeItem, 500, 1)
MisResultAction( TakeItem, 3050, 10)
MisResultAction( ClearMission, 1464)
MisResultAction( GiveItem, 47, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 3050)	
TriggerAction( 1, AddNextFlag, 1464, 10, 10 )
RegCurTrigger( 14641 )

-----------------------------------------------------------
DefineMission (5800, "", 1470)

MisBeginTalk("<t>,,.?")

MisBeginCondition(NoMission,1470)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(NoRecord,1470)
MisBeginAction(AddMission,1470)
MisCancelAction(ClearMission, 1470)

MisNeed(MIS_NEED_DESP,"80")
MisHelpTalk("<t>!")
MisResultTalk("<t>,.!")

MisResultCondition(HasMission, 1470)
MisResultCondition(NoRecord,1470)
MisResultCondition(HasFightingPoint,80 )
MisResultAction(TakeFightingPoint, 80 )
MisResultAction(ClearMission, 1470)
MisResultAction(SetRecord,  1470 )
MisResultAction(GiveItem, 2568, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	--------
DefineMission (5801, "Tender Cancer: Fame Hero", 1471)

MisBeginTalk("<t>,..")

MisBeginCondition(NoMission,1471)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(NoRecord,1471)
MisBeginAction(AddMission,1471)
MisCancelAction(ClearMission, 1471)

MisNeed(MIS_NEED_DESP,"Obtain 3000 points of reputation.")
MisHelpTalk("<t>,3000? ")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1471)
MisResultCondition(NoRecord,1471)
MisResultCondition(HasCredit,3000 )
MisResultAction(TakeCredit, 3000 )
MisResultAction(ClearMission, 1471)
MisResultAction(SetRecord,  1471 )
MisResultAction(GiveItem, 2569, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	---------
DefineMission (5802, "Tender Cancer: Level Hero", 1472)

MisBeginTalk("<t>,,...")

MisBeginCondition(NoMission,1472)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(NoRecord,1472)
MisBeginAction(AddMission,1472)
MisCancelAction(ClearMission, 1472)

MisNeed(MIS_NEED_DESP,"Reached Level 65")
MisHelpTalk("<t>,65.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1472)
MisResultCondition(NoRecord,1472)
MisResultCondition(LvCheck, ">", 64 )
MisResultAction(ClearMission, 1472)
MisResultAction(SetRecord,  1472 )
MisResultAction(GiveItem, 2570, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	----------
DefineMission (5803, "Tender Cancer: Honor Hero", 1473)

MisBeginTalk("<t>,,!,.")

MisBeginCondition(NoMission,1473)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(NoRecord,1473)
MisBeginAction(AddMission,1473)
MisCancelAction(ClearMission, 1473)

MisNeed(MIS_NEED_DESP,"600")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1473)
MisResultCondition(NoRecord,1473)
MisResultCondition(HasHonorPoint,600 )
MisResultAction(TakeHonorPoint, 600 )
MisResultAction(ClearMission, 1473)
MisResultAction(SetRecord,  1473 )
MisResultAction(GiveItem, 2571, 1, 4)
MisResultBagNeed(1)	

-----------------------------------------------------------	----------	
DefineMission (5804, "Tender Cancer: Gathering Ambassador", 1474)

MisBeginTalk("<t>!....")

MisBeginCondition(NoMission,1474)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(NoRecord,1474)
MisBeginAction(AddMission,1474)
MisBeginAction(AddTrigger, 14741, TE_GETITEM, 1693, 10 )-------------------10
MisBeginAction(AddTrigger, 14742, TE_GETITEM, 2677, 10 )-------------------LV1 10          
MisBeginAction(AddTrigger, 14743, TE_GETITEM, 3909, 1 )-----------------1               
MisBeginAction(AddTrigger, 14744, TE_GETITEM, 2589, 5 )----------------5               
MisBeginAction(AddTrigger, 14745, TE_GETITEM, 3094, 5)------------------5            
MisBeginAction(AddTrigger, 14746, TE_GETITEM, 3827, 1 )-----------------1             
MisCancelAction(ClearMission, 1474)


MisNeed(MIS_NEED_ITEM, 1693, 10,  1, 10 )
MisNeed(MIS_NEED_ITEM, 2677, 10, 11, 10 )
MisNeed(MIS_NEED_ITEM, 3909, 1,  21, 1 )
MisNeed(MIS_NEED_ITEM, 2589, 5,  22, 5 )
MisNeed(MIS_NEED_ITEM, 3094, 5,  27, 5 )
MisNeed(MIS_NEED_ITEM, 3827, 1,  32, 1 )


MisHelpTalk("<t>,.")
MisResultTalk("<t>,,,,,!")

MisResultCondition(HasMission, 1474)
MisResultCondition(NoRecord,1474)
MisResultCondition(HasItem, 1693, 10 )
MisResultCondition(HasItem, 2677, 10 )
MisResultCondition(HasItem, 3909, 1 )
MisResultCondition(HasItem, 2589, 5 )
MisResultCondition(HasItem, 3094, 5 )
MisResultCondition(HasItem, 3827, 1 )


MisResultAction(TakeItem, 1693, 10 )
MisResultAction(TakeItem, 2677, 10 )
MisResultAction(TakeItem, 3909, 1 )
MisResultAction(TakeItem, 2589, 5 )
MisResultAction(TakeItem, 3094, 5 )
MisResultAction(TakeItem, 3827, 1 )

MisResultAction(ClearMission, 1474)
MisResultAction(SetRecord,  1474 )
MisResultAction(GiveItem, 2572, 1, 4)
MisResultAction(GiveItem, 2576, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1693)	
TriggerAction( 1, AddNextFlag, 1474, 1, 10 )
RegCurTrigger( 14741 )

InitTrigger()
TriggerCondition( 1, IsItem, 2677)	
TriggerAction( 1, AddNextFlag, 1474, 11, 10 )
RegCurTrigger( 14742 )

InitTrigger()
TriggerCondition( 1, IsItem, 3909)	
TriggerAction( 1, AddNextFlag, 1474, 21, 1 )
RegCurTrigger( 14743 )

InitTrigger()
TriggerCondition( 1, IsItem, 2589)	
TriggerAction( 1, AddNextFlag, 1474, 22, 5 )
RegCurTrigger( 14744 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1474, 27, 5 )
RegCurTrigger( 14745 )

InitTrigger()
TriggerCondition( 1, IsItem, 3827)	
TriggerAction( 1, AddNextFlag, 1474,32, 1 )
RegCurTrigger( 14746 )

--------------------------------------------------------------------
DefineMission( 5805, "Tender Cancer: Cancer Doomsday", 1475 )
MisBeginTalk("<t>BOSS,.")
			
MisBeginCondition(NoMission, 1475)
MisBeginCondition(HasRecord,1469)
MisBeginCondition(NoRecord,1475)
MisBeginAction(AddMission,1475)
MisBeginAction(AddTrigger, 14751, TE_KILL, 1040, 1)---

MisCancelAction(ClearMission, 1475)

MisNeed(MIS_NEED_DESP,",(1637,3751)!")
MisNeed(MIS_NEED_KILL, 1040,1, 10, 1)


MisResultTalk("<t>.!")
MisHelpTalk("<t>,.")
MisResultCondition(HasMission,  1475)
MisResultCondition(HasFlag, 1475, 10)
MisResultCondition(NoRecord , 1475)
MisResultAction(GiveItem, 2574, 1, 4 )
MisResultAction(ClearMission,  1475)
MisResultAction(SetRecord,  1475 )
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsMonster, 1040)	
TriggerAction( 1, AddNextFlag, 1475, 10, 1 )
RegCurTrigger( 14751 )


-----------------------------------------------------------
DefineMission (5806, "Tender Cancer: Brawl Hero", 1476)

MisBeginTalk("<t>,,.?")

MisBeginCondition(NoMission,1476)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(NoRecord,1476)
MisBeginAction(AddMission,1476)
MisCancelAction(ClearMission, 1476)

MisNeed(MIS_NEED_DESP,"150")
MisHelpTalk("<t>!")
MisResultTalk("<t>,.!")

MisResultCondition(HasMission, 1476)
MisResultCondition(NoRecord,1476)
MisResultCondition(HasFightingPoint,150 )
MisResultAction(TakeFightingPoint, 150 )
MisResultAction(ClearMission, 1476)
MisResultAction(SetRecord,  1476 )
MisResultAction(GiveItem, 2568, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	--------
DefineMission (5807, "Tender Cancer: Fame Hero", 1477)

MisBeginTalk("<t>,..")

MisBeginCondition(NoMission,1477)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(NoRecord,1477)
MisBeginAction(AddMission,1477)
MisCancelAction(ClearMission, 1477)

MisNeed(MIS_NEED_DESP,"6000")
MisHelpTalk("<t>,6000? ")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1477)
MisResultCondition(NoRecord,1477)
MisResultCondition(HasCredit,6000 )
MisResultAction(TakeCredit, 6000 )
MisResultAction(ClearMission, 1477)
MisResultAction(SetRecord,  1477 )
MisResultAction(GiveItem, 2569, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	---------
DefineMission (5808, "Tender Cancer: Level Hero", 1600)

MisBeginTalk("<t>,,...")

MisBeginCondition(NoMission,1600)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(NoRecord,1600)
MisBeginAction(AddMission,1600)
MisCancelAction(ClearMission, 1600)

MisNeed(MIS_NEED_DESP,"70")
MisHelpTalk("<t>,70.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1600)
MisResultCondition(NoRecord,1600)
MisResultCondition(LvCheck, ">", 69 )
MisResultAction(ClearMission, 1600)
MisResultAction(SetRecord,  1600 )
MisResultAction(GiveItem, 2570, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	----------
DefineMission (5809, "Tender Cancer: Honor Hero", 1479)

MisBeginTalk("<t>,,!,.")

MisBeginCondition(NoMission,1479)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(NoRecord,1479)
MisBeginAction(AddMission,1479)
MisCancelAction(ClearMission, 1479)

MisNeed(MIS_NEED_DESP,"900")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1479)
MisResultCondition(NoRecord,1479)
MisResultCondition(HasHonorPoint,900 )
MisResultAction(TakeHonorPoint, 900 )
MisResultAction(ClearMission, 1479)
MisResultAction(SetRecord,  1479 )
MisResultAction(GiveItem, 2571, 1, 4)
MisResultBagNeed(1)	

-----------------------------------------------------------	----------	
DefineMission (5810, "Tender Cancer: Gathering Ambassador", 1480)

MisBeginTalk("<t>!....")

MisBeginCondition(NoMission,1480)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(NoRecord,1480)
MisBeginAction(AddMission,1480)
MisBeginAction(AddTrigger, 14801, TE_GETITEM, 1693, 20 )-------------------20
MisBeginAction(AddTrigger, 14802, TE_GETITEM, 2677, 20 )-------------------LV1 20          
MisBeginAction(AddTrigger, 14803, TE_GETITEM, 3909, 5 )-----------------5               
MisBeginAction(AddTrigger, 14804, TE_GETITEM, 2589, 5 )----------------5               
MisBeginAction(AddTrigger, 14805, TE_GETITEM, 3094, 10)------------------10         
MisBeginAction(AddTrigger, 14806, TE_GETITEM, 3827, 1 )-----------------1     
MisBeginAction(AddTrigger, 14807, TE_GETITEM, 0271, 1 )-----------------1         
MisCancelAction(ClearMission, 1480)


MisNeed(MIS_NEED_ITEM, 1693, 20,  1, 20 )
MisNeed(MIS_NEED_ITEM, 2677, 20, 21, 20 )
MisNeed(MIS_NEED_ITEM, 3909, 5,  41, 5 )
MisNeed(MIS_NEED_ITEM, 2589, 5,  46, 5 )
MisNeed(MIS_NEED_ITEM, 3094, 10, 51, 10 )
MisNeed(MIS_NEED_ITEM, 3827, 1,  61, 1 )
MisNeed(MIS_NEED_ITEM, 0271, 1,  62, 1 )

MisHelpTalk("<t>,.")
MisResultTalk("<t>,,,,,!")

MisResultCondition(HasMission, 1480)
MisResultCondition(NoRecord,1480)
MisResultCondition(HasItem, 1693, 20 )
MisResultCondition(HasItem, 2677, 20 )
MisResultCondition(HasItem, 3909, 5 )
MisResultCondition(HasItem, 2589, 5 )
MisResultCondition(HasItem, 3094, 10 )
MisResultCondition(HasItem, 3827, 1 )
MisResultCondition(HasItem, 0271, 1 )

MisResultAction(TakeItem, 1693, 20 )
MisResultAction(TakeItem, 2677, 20 )
MisResultAction(TakeItem, 3909, 5 )
MisResultAction(TakeItem, 2589, 5 )
MisResultAction(TakeItem, 3094, 10 )
MisResultAction(TakeItem, 3827, 1 )
MisResultAction(TakeItem, 0271, 1 )

MisResultAction(ClearMission, 1480)
MisResultAction(SetRecord,  1480 )
MisResultAction(GiveItem, 2572, 1, 4)
MisResultAction(GiveItem, 2576, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1693)	
TriggerAction( 1, AddNextFlag, 1480, 1, 20 )
RegCurTrigger( 14801 )

InitTrigger()
TriggerCondition( 1, IsItem, 2677)	
TriggerAction( 1, AddNextFlag, 1480, 21, 20 )
RegCurTrigger( 14802 )

InitTrigger()
TriggerCondition( 1, IsItem, 3909)	
TriggerAction( 1, AddNextFlag, 1480, 41, 5 )
RegCurTrigger( 14803 )

InitTrigger()
TriggerCondition( 1, IsItem, 2589)	
TriggerAction( 1, AddNextFlag, 1480, 46, 5 )
RegCurTrigger( 14804 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1480, 51, 10 )
RegCurTrigger( 14805 )

InitTrigger()
TriggerCondition( 1, IsItem, 3827)	
TriggerAction( 1, AddNextFlag, 1480,61, 1 )
RegCurTrigger( 14806 )

InitTrigger()
TriggerCondition( 1, IsItem, 0271)	
TriggerAction( 1, AddNextFlag, 1480,62, 1 )
RegCurTrigger( 14807 )


-----------------------------------------------------------
DefineMission (5811, "Tender Cancer: Brawl Hero", 1481)

MisBeginTalk("<t>,,.?")

MisBeginCondition(NoMission,1481)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(NoRecord,1481)
MisBeginAction(AddMission,1481)
MisCancelAction(ClearMission, 1481)

MisNeed(MIS_NEED_DESP,"200")
MisHelpTalk("<t>!")
MisResultTalk("<t>,.!")

MisResultCondition(HasMission, 1481)
MisResultCondition(NoRecord,1481)
MisResultCondition(HasFightingPoint,200 )
MisResultAction(TakeFightingPoint, 200 )
MisResultAction(ClearMission, 1481)
MisResultAction(SetRecord,  1481 )
MisResultAction(GiveItem, 2568, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	--------
DefineMission (5812, "Tender Cancer: Fame Hero", 1482)

MisBeginTalk("<t>,..")

MisBeginCondition(NoMission,1482)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(NoRecord,1482)
MisBeginAction(AddMission,1482)
MisCancelAction(ClearMission, 1482)

MisNeed(MIS_NEED_DESP,"10000")
MisHelpTalk("<t>,10000? ")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1482)
MisResultCondition(NoRecord,1482)
MisResultCondition(HasCredit,10000 )
MisResultAction(TakeCredit, 10000 )
MisResultAction(ClearMission, 1482)
MisResultAction(SetRecord,  1482 )
MisResultAction(GiveItem, 2569, 1, 4)
MisResultBagNeed(1)


-----------------------------------------------------------	---------
DefineMission (5813, "Tender Cancer: Level Hero", 1483)

MisBeginTalk("<t>,,...")

MisBeginCondition(NoMission,1483)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(NoRecord,1483)
MisBeginAction(AddMission,1483)
MisCancelAction(ClearMission, 1483)

MisNeed(MIS_NEED_DESP,"75")
MisHelpTalk("<t>,75.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1483)
MisResultCondition(NoRecord,1483)
MisResultCondition(LvCheck, ">", 74 )
MisResultAction(ClearMission, 1483)
MisResultAction(SetRecord,  1483 )
MisResultAction(GiveItem, 2570, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------------------	----------
DefineMission (5814, "Tender Cancer: Honor Hero", 1484)

MisBeginTalk("<t>,,!,.")

MisBeginCondition(NoMission,1484)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(NoRecord,1484)
MisBeginAction(AddMission,1484)
MisCancelAction(ClearMission, 1484)

MisNeed(MIS_NEED_DESP,"1200")
MisHelpTalk("<t>!")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1484)
MisResultCondition(NoRecord,1484)
MisResultCondition(HasHonorPoint,1200 )
MisResultAction(TakeHonorPoint, 1200 )
MisResultAction(ClearMission, 1484)
MisResultAction(SetRecord,  1484 )
MisResultAction(GiveItem, 2571, 1, 4)
MisResultBagNeed(1)	


-----------------------------------------------------------	----------	
DefineMission (5815, "Tender Cancer: Gathering Ambassador", 1485)

MisBeginTalk("<t>!....")

MisBeginCondition(NoMission,1485)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(NoRecord,1485)
MisBeginAction(AddMission,1485)
MisBeginAction(AddTrigger, 14851, TE_GETITEM, 1693, 30 )-------------------30
MisBeginAction(AddTrigger, 14852, TE_GETITEM, 2677, 30 )-------------------LV1 30          
MisBeginAction(AddTrigger, 14853, TE_GETITEM, 3909, 10 )-----------------10               
MisBeginAction(AddTrigger, 14854, TE_GETITEM, 2589, 10 )----------------10               
MisBeginAction(AddTrigger, 14855, TE_GETITEM, 3094, 20)------------------20         
MisBeginAction(AddTrigger, 14856, TE_GETITEM, 3827, 1 )-----------------1     
MisBeginAction(AddTrigger, 14857, TE_GETITEM, 0271, 2 )-----------------2         
MisCancelAction(ClearMission, 1485)


MisNeed(MIS_NEED_ITEM, 1693, 30,  1, 30 )
MisNeed(MIS_NEED_ITEM, 2677, 30, 31, 30 )
MisNeed(MIS_NEED_ITEM, 3909, 10, 61, 10 )
MisNeed(MIS_NEED_ITEM, 2589, 10, 71, 10 )
MisNeed(MIS_NEED_ITEM, 3094, 20, 81, 20 )
MisNeed(MIS_NEED_ITEM, 3827, 1,  101, 1 )
MisNeed(MIS_NEED_ITEM, 0271, 2,  102, 2 )

MisHelpTalk("<t>,.")
MisResultTalk("<t>,,,,,!")

MisResultCondition(HasMission, 1485)
MisResultCondition(NoRecord,1485)
MisResultCondition(HasItem, 1693, 30 )
MisResultCondition(HasItem, 2677, 30 )
MisResultCondition(HasItem, 3909, 10 )
MisResultCondition(HasItem, 2589, 10 )
MisResultCondition(HasItem, 3094, 20 )
MisResultCondition(HasItem, 3827, 1 )
MisResultCondition(HasItem, 0271, 2 )

MisResultAction(TakeItem, 1693, 30 )
MisResultAction(TakeItem, 2677, 30 )
MisResultAction(TakeItem, 3909, 10 )
MisResultAction(TakeItem, 2589, 10 )
MisResultAction(TakeItem, 3094, 20 )
MisResultAction(TakeItem, 3827, 1 )
MisResultAction(TakeItem, 0271, 2 )

MisResultAction(ClearMission, 1485)
MisResultAction(SetRecord,  1485 )
MisResultAction(GiveItem, 2572, 1, 4)
MisResultAction(GiveItem, 2576, 1, 4)
MisResultBagNeed(2)

InitTrigger()
TriggerCondition( 1, IsItem, 1693)	
TriggerAction( 1, AddNextFlag, 1485, 1, 30 )
RegCurTrigger( 14851 )

InitTrigger()
TriggerCondition( 1, IsItem, 2677)	
TriggerAction( 1, AddNextFlag, 1485, 31, 30 )
RegCurTrigger( 14852 )

InitTrigger()
TriggerCondition( 1, IsItem, 3909)	
TriggerAction( 1, AddNextFlag, 1485, 61, 10 )
RegCurTrigger( 14853 )

InitTrigger()
TriggerCondition( 1, IsItem, 2589)	
TriggerAction( 1, AddNextFlag, 1485, 71, 10 )
RegCurTrigger( 14854 )

InitTrigger()
TriggerCondition( 1, IsItem, 3094)	
TriggerAction( 1, AddNextFlag, 1485, 81, 20 )
RegCurTrigger( 14855 )

InitTrigger()
TriggerCondition( 1, IsItem, 3827)	
TriggerAction( 1, AddNextFlag, 1485,101, 1 )
RegCurTrigger( 14856 )

InitTrigger()
TriggerCondition( 1, IsItem, 0271)	
TriggerAction( 1, AddNextFlag, 1485,102, 2 )
RegCurTrigger( 14857 )


--------------------------------------------------------------------
DefineMission( 5816, "1", 1486 )
MisBeginTalk("<t>,,,,,.")
			
MisBeginCondition(NoMission, 1486)
MisBeginCondition(NoRecord,1486)
MisBeginCondition(HasRecord, 1469)
MisBeginAction(AddMission,1486)
MisCancelAction(ClearMission, 1486)

MisNeed(MIS_NEED_DESP,"(3316,2516)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
----------------------------------------------------------

DefineMission(5817, "1", 1486, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,,.")
MisResultCondition(NoRecord, 1486)
MisResultCondition(HasMission,1486)
MisResultAction(ClearMission,1486)
MisResultAction(SetRecord, 1486)

----------------------------------------------------------2----------
DefineMission( 5818, "TOP Ambassador 2", 1487 )
MisBeginTalk("<t>,,,.")
			
MisBeginCondition(NoMission, 1487)
MisBeginCondition(NoRecord,1487)
MisBeginCondition(HasRecord, 1486)
MisBeginAction(AddMission,1487)
MisCancelAction(ClearMission, 1487)

MisNeed(MIS_NEED_DESP,"(3302,2501)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------2-------------
DefineMission(5819, "TOP Ambassador 2", 1487, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,?")
MisResultCondition(NoRecord, 1487)
MisResultCondition(HasMission,1487)
MisResultAction(ClearMission,1487)
MisResultAction(SetRecord, 1487)

----------------------------------------------------------3----------
DefineMission( 5820, "TOP Ambassador 3", 1488 )
MisBeginTalk("<t>?,!,.!")
			
MisBeginCondition(NoMission, 1488)
MisBeginCondition(NoRecord,1488)
MisBeginCondition(HasRecord, 1487)
MisBeginAction(AddMission,1488)
MisCancelAction(ClearMission, 1488)

MisNeed(MIS_NEED_DESP,"(3279,2501)")

MisHelpTalk("<t>,,..")
MisResultCondition(AlwaysFailure)	
-----------------------------------------3--------
DefineMission(5821, "TOP Ambassador 3", 1488, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,,.")
MisResultCondition(NoRecord, 1488)
MisResultCondition(HasMission,1488)
MisResultAction(ClearMission,1488)
MisResultAction(SetRecord, 1488)

----------------------------------------------------------4----------
DefineMission( 5822, "TOP Ambassador 4", 1489 )
MisBeginTalk("<t>,,!!,.")
			
MisBeginCondition(NoMission, 1489)
MisBeginCondition(NoRecord,  1489)
MisBeginCondition(HasRecord, 1488)
MisBeginAction(AddMission, 1489)
MisCancelAction(ClearMission, 1489)

MisNeed(MIS_NEED_DESP,"(513,269)")

MisHelpTalk("<t>,!")
MisResultCondition(AlwaysFailure)	
-----------------------------------------4--------
DefineMission(5823, "TOP Ambassador 4", 1489, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,?,!....,.")
MisResultCondition(NoRecord, 1489)
MisResultCondition(HasMission,1489)
MisResultAction(ClearMission,1489)
MisResultAction(SetRecord, 1489)


----------------------------------------------------------5----------
DefineMission( 5824, "TOP Ambassador 5", 1490 )
MisBeginTalk("<t>,,,!.?.")
			
MisBeginCondition(NoMission, 1490)
MisBeginCondition(NoRecord,  1490)
MisBeginCondition(HasRecord, 1489)
MisBeginAction(AddMission,1490)
MisCancelAction(ClearMission, 1490)

MisNeed(MIS_NEED_DESP,"(3195,2506)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------5--------
DefineMission(5825, "TOP Ambassador 5", 1490, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,,?")
MisResultCondition(NoRecord, 1490)
MisResultCondition(HasMission,1490)
MisResultAction(ClearMission,1490)
MisResultAction(SetRecord, 1490)


----------------------------------------------------------6----------
DefineMission( 5826, "TOP Ambassador 6", 1491)
MisBeginTalk("<t>,!,,!!,,.")
			
MisBeginCondition(NoMission, 1491)
MisBeginCondition(NoRecord,1491)
MisBeginCondition(HasRecord, 1490)
MisBeginAction(AddMission,1491)
MisCancelAction(ClearMission, 1491)

MisNeed(MIS_NEED_DESP,"(3326,2511)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------6--------
DefineMission(5827, "TOP Ambassador 6", 1491, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.")
MisResultCondition(NoRecord, 1491)
MisResultCondition(HasMission,1491)
MisResultAction(ClearMission,1491)
MisResultAction(SetRecord, 1491)


----------------------------------------------------------7----------
DefineMission( 5828, "TOP Ambassador 7", 1492 )
MisBeginTalk("<t>.,,!.")
			
MisBeginCondition(NoMission, 1492)
MisBeginCondition(NoRecord,1492)
MisBeginCondition(HasRecord, 1491)
MisBeginAction(AddMission,1492)
MisCancelAction(ClearMission, 1492)

MisNeed(MIS_NEED_DESP,"(3262,2502)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------7--------
DefineMission(5829, "TOP Ambassador 7", 1492, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,.")
MisResultCondition(NoRecord, 1492)
MisResultCondition(HasMission,1492)
MisResultAction(ClearMission,1492)
MisResultAction(SetRecord, 1492)


----------------------------------------------------------8----------
DefineMission( 5830, "TOP Ambassador 8", 1493 )
MisBeginTalk("<t>,,!.")
			
MisBeginCondition(NoMission, 1493)
MisBeginCondition(NoRecord,  1493)
MisBeginCondition(HasRecord, 1492)
MisBeginAction(AddMission,1493)
MisCancelAction(ClearMission, 1493)

MisNeed(MIS_NEED_DESP,"(3287,2501)")

MisHelpTalk("<t>!")
MisResultCondition(AlwaysFailure)

-----------------------------------------8--------
DefineMission(5831, "TOP Ambassador 8", 1493, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,,.")
MisResultCondition(NoRecord, 1493)
MisResultCondition(HasMission,1493)
MisResultAction(ClearMission,1493)
MisResultAction(SetRecord, 1493)


----------------------------------------------------------9----------
DefineMission( 5832, "TOP Ambassador 9", 1494 )
MisBeginTalk("<t>....")
			
MisBeginCondition(NoMission, 1494)
MisBeginCondition(NoRecord,  1494)
MisBeginCondition(HasRecord, 1493)
MisBeginAction(AddMission,1494)
MisCancelAction(ClearMission, 1494)

MisNeed(MIS_NEED_DESP,"(3409,2560)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------9--------
DefineMission(5833, "TOP Ambassador 9", 1494, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,!.")
MisResultCondition(NoRecord, 1494)
MisResultCondition(HasMission,1494)
MisResultAction(ClearMission,1494)
MisResultAction(SetRecord, 1494)

----------------------------------------------------------10----------
DefineMission( 5834, "TOP Ambassador 10", 1495 )
MisBeginTalk("<t>,,,.,.")
			
MisBeginCondition(NoMission, 1495)
MisBeginCondition(NoRecord, 1495)
MisBeginCondition(HasRecord, 1494)
MisBeginAction(AddMission, 1495)
MisCancelAction(ClearMission, 1495)

MisNeed(MIS_NEED_DESP,"(3303,2533)")

MisHelpTalk("<t>...")
MisResultCondition(AlwaysFailure)	
-----------------------------------------10--------
DefineMission(5835, "TOP Ambassador 10", 1495, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.?,!!")
MisResultCondition(NoRecord, 1495)
MisResultCondition(HasMission,1495)
MisResultAction(ClearMission,1495)
MisResultAction(SetRecord, 1495)

----------------------------------------------------------11----------
DefineMission( 5836, "TOP Ambassador 11", 1496 )
MisBeginTalk("<t>,.")
			
MisBeginCondition(NoMission, 1496)
MisBeginCondition(NoRecord,1496)
MisBeginCondition(HasRecord, 1495)
MisBeginAction(AddMission,1496)
MisCancelAction(ClearMission, 1496)

MisNeed(MIS_NEED_DESP,"(3298,2534)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------11--------
DefineMission(5837, "TOP Ambassador 11", 1496, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.?.")
MisResultCondition(NoRecord, 1496)
MisResultCondition(HasMission,1496)
MisResultAction(ClearMission,1496)
MisResultAction(SetRecord, 1496)

----------------------------------------------------------12----------
DefineMission( 5838, "TOP Ambassador 12", 1497 )
MisBeginTalk("<t>,,.,,,.")
			
MisBeginCondition(NoMission, 1497)
MisBeginCondition(NoRecord,1497)
MisBeginCondition(HasRecord, 1496)
MisBeginAction(AddMission,1497)
MisCancelAction(ClearMission, 1497)

MisNeed(MIS_NEED_DESP,"(3290,2512)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------12--------
DefineMission(5839, "TOP Ambassador 12", 1497, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,.")
MisResultCondition(NoRecord, 1497)
MisResultCondition(HasMission,1497)
MisResultAction(ClearMission,1497)
MisResultAction(SetRecord, 1497)


----------------------------------------------------------13----------
DefineMission( 5840, "TOP Ambassador 13", 1498 )
MisBeginTalk("<t>,,,.")
			
MisBeginCondition(NoMission, 1498)
MisBeginCondition(NoRecord,1498)
MisBeginCondition(HasRecord, 1497)
MisBeginAction(AddMission,1498)
MisCancelAction(ClearMission, 1498)

MisNeed(MIS_NEED_DESP,"(3275,2467)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------13--------
DefineMission(5841, "TOP Ambassador 13", 1498, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,!?.")
MisResultCondition(NoRecord, 1498)
MisResultCondition(HasMission,1498)
MisResultAction(ClearMission,1498)
MisResultAction(SetRecord, 1498)

----------------------------------------------------------14----------
DefineMission( 5842, "TOP Ambassador 14", 1499 )
MisBeginTalk("<t>,!,,.")
			
MisBeginCondition(NoMission, 1499)
MisBeginCondition(NoRecord,1499)
MisBeginCondition(HasRecord, 1498)
MisBeginAction(AddMission,1499)
MisCancelAction(ClearMission, 1499)

MisNeed(MIS_NEED_DESP,"(3241,2533)")

MisHelpTalk("<t>......")
MisResultCondition(AlwaysFailure)	
-----------------------------------------14--------        
DefineMission(5843, "TOP Ambassador 14", 1499, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?....,....")
MisResultCondition(NoRecord, 1499)
MisResultCondition(HasMission,1499)
MisResultAction(ClearMission,1499)
MisResultAction(SetRecord, 1499)

----------------------------------------------------------15----------
DefineMission( 5844, "TOP Ambassador 15", 1601 )
MisBeginTalk("<t>,,.,..")
			
MisBeginCondition(NoMission, 1601)
MisBeginCondition(NoRecord,1601)
MisBeginCondition(HasRecord, 1499)
MisBeginAction(AddMission,1601)
MisCancelAction(ClearMission, 1601)

MisNeed(MIS_NEED_DESP,"(3265,2547)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------15-------- 
DefineMission(5845, "TOP Ambassador 15", 1601, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,.,.")
MisResultCondition(NoRecord, 1601)
MisResultCondition(HasMission,1601)
MisResultAction(ClearMission,1601)
MisResultAction(SetRecord, 1601)


----------------------------------------------------------16----------
DefineMission( 5846, "TOP Ambassador 16", 1602 )
MisBeginTalk("<t>,?,.,.")
			
MisBeginCondition(NoMission, 1602)
MisBeginCondition(NoRecord,1602)
MisBeginCondition(HasRecord, 1601)
MisBeginAction(AddMission,1602)
MisCancelAction(ClearMission, 1602)

MisNeed(MIS_NEED_DESP,"(3235,2550)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------16-------- 
DefineMission(5847, "TOP Ambassador 16", 1602, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,,,!")
MisResultCondition(NoRecord, 1602)
MisResultCondition(HasMission,1602)
MisResultAction(ClearMission,1602)
MisResultAction(SetRecord, 1602)


----------------------------------------------------------17----------
DefineMission( 5848, "TOP Ambassador 17", 1502 )
MisBeginTalk("<t>??,.")
			
MisBeginCondition(NoMission, 1502)
MisBeginCondition(NoRecord,1502)
MisBeginCondition(HasRecord, 1602)
MisBeginAction(AddMission,1502)
MisCancelAction(ClearMission, 1502)

MisNeed(MIS_NEED_DESP,"(2219,3286)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------17-------- 
DefineMission(5849, "TOP Ambassador 17", 1502, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,,.")
MisResultCondition(NoRecord, 1502)
MisResultCondition(HasMission,1502)
MisResultAction(ClearMission,1502)
MisResultAction(SetRecord, 1502)

----------------------------------------------------------18----------
DefineMission( 5850, "TOP Ambassador 18", 1603 )
MisBeginTalk("<t>,.,,,!,,,.")
			
MisBeginCondition(NoMission, 1603)
MisBeginCondition(NoRecord,1603)
MisBeginCondition(HasRecord, 1502)
MisBeginAction(AddMission,1603)
MisCancelAction(ClearMission, 1603)

MisNeed(MIS_NEED_DESP,"(3300,2513)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------18-------- 
DefineMission(5851, "TOP Ambassador 18", 1603, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,!")
MisResultCondition(NoRecord, 1603)
MisResultCondition(HasMission,1603)
MisResultAction(ClearMission,1603)
MisResultAction(SetRecord, 1603)

----------------------------------------------------------19----------
DefineMission( 5852, "TOP Ambassador 19", 1604 )
MisBeginTalk("<t>,,??,,,!")
			
MisBeginCondition(NoMission, 1604)
MisBeginCondition(NoRecord,1604)
MisBeginCondition(HasRecord, 1603)
MisBeginAction(AddMission,1604)
MisCancelAction(ClearMission, 1604)

MisNeed(MIS_NEED_DESP,"(3685,2652)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------19-------- 
DefineMission(5853, "TOP Ambassador 19", 1604, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,!,...?,,.")
MisResultCondition(NoRecord, 1604)
MisResultCondition(HasMission,1604)
MisResultAction(ClearMission,1604)
MisResultAction(SetRecord, 1604)


----------------------------------------------------------20----------
DefineMission( 5854, "TOP Ambassador 20", 1605 )
MisBeginTalk("<t>,.,,,,.")
			
MisBeginCondition(NoMission, 1605)
MisBeginCondition(NoRecord,1605)
MisBeginCondition(HasRecord, 1604)
MisBeginAction(AddMission,1605)
MisCancelAction(ClearMission, 1605)

MisNeed(MIS_NEED_DESP,"(3337,3523)")

MisHelpTalk("<t>,.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------20-------- 
DefineMission(5855, "TOP Ambassador 20", 1605, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.,?,,.,....")
MisResultCondition(NoRecord, 1605)
MisResultCondition(HasMission,1605)
MisResultAction(ClearMission,1605)
MisResultAction(SetRecord, 1605)

----------------------------------------------------------21----------
DefineMission( 5856, "TOP Ambassador 21", 1606 )
MisBeginTalk("<t>?.,,.!,,.")
			
MisBeginCondition(NoMission, 1606)
MisBeginCondition(NoRecord,1606)
MisBeginCondition(HasRecord, 1605)
MisBeginAction(AddMission,1606)
MisCancelAction(ClearMission, 1606)

MisNeed(MIS_NEED_DESP,"(2247,2858)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-----------------------------------------21------- 
DefineMission(5857, "Community Hero 21", 1606, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,!,,.")
MisResultCondition(NoRecord, 1606)
MisResultCondition(HasMission,1606)
MisResultAction(ClearMission,1606)
MisResultAction(SetRecord, 1606)
MisResultAction(GiveItem, 2573, 1, 4)
MisResultBagNeed(1)


---------------------------------------------------------
DefineMission (5858, "", 1607)

MisBeginTalk("<t>,?,,,.")

MisBeginCondition(NoMission,1607)
MisBeginCondition(NoRecord,1607)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(HasRecord,1481)
MisBeginCondition(HasRecord,1482)
MisBeginCondition(HasRecord,1483)
MisBeginCondition(HasRecord,1484)
MisBeginCondition(HasRecord,1485)
MisBeginCondition(HasRecord,1475)
MisBeginCondition(HasRecord,1606)
MisBeginAction(AddMission,1607)
MisCancelAction(ClearMission, 1607)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"(1335,469)")
MisHelpTalk("<t>.")

MisResultCondition(AlwaysFailure)	


---------------------------------------------------------
DefineMission (5859, "", 1607, COMPLETE_SHOW)

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?,,!!,!..")	
MisResultCondition(NoRecord, 1607)
MisResultCondition(HasMission,1607)
MisResultAction(ClearMission,1607)
MisResultAction(SetRecord, 1607)


------------------------------------------

DefineMission(5860, "", 1608 )	

MisBeginTalk("<t>,,.,..<b15(2041,1355)>.")

MisBeginCondition(NoMission,1608)
MisBeginCondition(NoRecord,1608)
MisBeginCondition(HasRecord,1607)
MisBeginAction(AddMission,1608)
MisBeginAction(AddChaItem3, 2952)---------
MisCancelAction(ClearMission, 1608)
MisBeginBagNeed(1)

MisNeed(MIS_NEED_DESP,"(2041,1355)")
MisHelpTalk("<t>Go now! You only have 15 minutes.")

MisResultCondition(AlwaysFailure)	

------------------------------------------

DefineMission(5861, "", 1608, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>......,,.")
MisResultCondition(HasMission, 1608)
MisResultCondition(NoRecord,1608)
MisResultAction(AddChaItem4, 2952)----
MisResultAction(ClearMission, 1608)
MisResultAction(SetRecord,  1608 )
MisResultAction(GiveItem, 2575, 1, 4)------------
MisResultBagNeed(1)

-----------------------------------------------------------
DefineMission (5862, "", 1609)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1609)
MisBeginCondition(HasRecord,1466)
MisBeginCondition(HasRecord,1470)
MisBeginCondition(HasRecord,1471)
MisBeginCondition(HasRecord,1472)
MisBeginCondition(HasRecord,1473)
MisBeginCondition(HasRecord,1474)
MisBeginCondition(HasRecord,1475)
MisBeginCondition(HasRecord,1606)
MisBeginCondition(NoRecord,1609)
MisBeginAction(AddMission,1609)  
MisBeginAction(AddTrigger, 16091, TE_GETITEM, 2568, 1 )		
MisBeginAction(AddTrigger, 16092, TE_GETITEM, 2569, 1 )		
MisBeginAction(AddTrigger, 16093, TE_GETITEM, 2570, 1 )		
MisBeginAction(AddTrigger, 16094, TE_GETITEM, 2571, 1 )		
MisBeginAction(AddTrigger, 16095, TE_GETITEM, 2572, 1 )		
MisBeginAction(AddTrigger, 16096, TE_GETITEM, 2573, 1 )		
MisBeginAction(AddTrigger, 16097, TE_GETITEM, 2574, 1 )	
MisCancelAction(ClearMission, 1609)						                                     

MisNeed(MIS_NEED_ITEM, 2568, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 2569, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 2570, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 2571, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 2572, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 2573, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 2574, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1609)
MisResultCondition(NoRecord,1609)
MisResultCondition(HasItem, 2568, 1 )
MisResultCondition(HasItem, 2569, 1 )
MisResultCondition(HasItem, 2570, 1 )
MisResultCondition(HasItem, 2571, 1 )
MisResultCondition(HasItem, 2572, 1 )
MisResultCondition(HasItem, 2573, 1 )
MisResultCondition(HasItem, 2574, 1 )

MisResultAction(TakeItem, 2568, 1 )
MisResultAction(TakeItem, 2569, 1 )
MisResultAction(TakeItem, 2570, 1 )
MisResultAction(TakeItem, 2571, 1 )
MisResultAction(TakeItem, 2572, 1 )
MisResultAction(TakeItem, 2573, 1 )
MisResultAction(TakeItem, 2574, 1 )

MisResultAction(ClearMission, 1609)
MisResultAction(SetRecord,  1609 )
MisResultAction(GiveItem, 2273, 1, 4)
MisResultAction(GiveItem, 2274, 1, 4)
MisResultAction(GiveItem, 3877, 1, 4)
MisResultAction(AddMoney,1000000,1000000)
MisResultAction(ShuangZiSS)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 1874)	
TriggerAction( 1, AddNextFlag, 1609, 10, 1 )
RegCurTrigger( 16091 )

InitTrigger()
TriggerCondition( 1, IsItem, 1875)	
TriggerAction( 1, AddNextFlag, 1609, 20, 1 )
RegCurTrigger( 16092 )

InitTrigger()
TriggerCondition( 1, IsItem, 1876)	
TriggerAction( 1, AddNextFlag, 1609, 30, 1 )
RegCurTrigger( 16093 )

InitTrigger()
TriggerCondition( 1, IsItem, 1877)	
TriggerAction( 1, AddNextFlag, 1609, 40, 1 )
RegCurTrigger( 16094 )

InitTrigger()
TriggerCondition( 1, IsItem, 1878)	
TriggerAction( 1, AddNextFlag, 1609, 50, 1 )
RegCurTrigger( 16095 )

InitTrigger()
TriggerCondition( 1, IsItem, 1879)	
TriggerAction( 1, AddNextFlag, 1609, 60, 1 )
RegCurTrigger( 16096 )

InitTrigger()
TriggerCondition( 1, IsItem, 1880)	
TriggerAction( 1, AddNextFlag, 1609, 70, 1 )
RegCurTrigger( 16097 )
--------------------------------------------------------	
DefineMission (5863, "", 1610)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1610)
MisBeginCondition(HasRecord,1467)
MisBeginCondition(HasRecord,1476)
MisBeginCondition(HasRecord,1477)
MisBeginCondition(HasRecord,1600)
MisBeginCondition(HasRecord,1479)
MisBeginCondition(HasRecord,1480)
MisBeginCondition(HasRecord,1475)
MisBeginCondition(HasRecord,1606)
MisBeginCondition(NoRecord,1610)
MisBeginAction(AddMission,1610)
MisBeginAction(AddTrigger, 16101, TE_GETITEM, 2568, 1 )		
MisBeginAction(AddTrigger, 16102, TE_GETITEM, 2569, 1 )		
MisBeginAction(AddTrigger, 16103, TE_GETITEM, 2570, 1 )		
MisBeginAction(AddTrigger, 16104, TE_GETITEM, 2571, 1 )		
MisBeginAction(AddTrigger, 16105, TE_GETITEM, 2572, 1 )		
MisBeginAction(AddTrigger, 16106, TE_GETITEM, 2573, 1 )		
MisBeginAction(AddTrigger, 16107, TE_GETITEM, 2574, 1 )		
MisCancelAction(ClearMission, 1610)						                                     

MisNeed(MIS_NEED_ITEM, 2568, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 2569, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 2570, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 2571, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 2572, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 2573, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 2574, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1610)
MisResultCondition(NoRecord,1610)
MisResultCondition(HasItem, 2568, 1 )
MisResultCondition(HasItem, 2569, 1 )
MisResultCondition(HasItem, 2570, 1 )
MisResultCondition(HasItem, 2571, 1 )
MisResultCondition(HasItem, 2572, 1 )
MisResultCondition(HasItem, 2573, 1 )
MisResultCondition(HasItem, 2574, 1 )

MisResultAction(TakeItem, 2568, 1 )
MisResultAction(TakeItem, 2569, 1 )
MisResultAction(TakeItem, 2570, 1 )
MisResultAction(TakeItem, 2571, 1 )
MisResultAction(TakeItem, 2572, 1 )
MisResultAction(TakeItem, 2573, 1 )
MisResultAction(TakeItem, 2574, 1 )

MisResultAction(ClearMission, 1610)
MisResultAction(SetRecord,  1610 )
MisResultAction(GiveItem, 2273, 1, 4)
MisResultAction(GiveItem, 2274, 1, 4)
MisResultAction(GiveItem, 3877, 2, 4)
MisResultAction(AddMoney,2000000,2000000)
MisResultAction(ShuangZiHD)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 2568)	
TriggerAction( 1, AddNextFlag, 1610, 10, 1 )
RegCurTrigger( 16101 )

InitTrigger()
TriggerCondition( 1, IsItem, 2569)	
TriggerAction( 1, AddNextFlag, 1610, 20, 1 )
RegCurTrigger( 16102 )

InitTrigger()
TriggerCondition( 1, IsItem, 2570)	
TriggerAction( 1, AddNextFlag, 1610, 30, 1 )
RegCurTrigger( 16103 )

InitTrigger()
TriggerCondition( 1, IsItem, 2571)	
TriggerAction( 1, AddNextFlag, 1610, 40, 1 )
RegCurTrigger( 16104 )

InitTrigger()
TriggerCondition( 1, IsItem, 2572)	
TriggerAction( 1, AddNextFlag, 1610, 50, 1 )
RegCurTrigger( 16105 )

InitTrigger()
TriggerCondition( 1, IsItem, 2573)	
TriggerAction( 1, AddNextFlag, 1610, 60, 1 )
RegCurTrigger( 16106 )

InitTrigger()
TriggerCondition( 1, IsItem, 2574)	
TriggerAction( 1, AddNextFlag, 1610, 70, 1 )
RegCurTrigger( 16107 )

------------------------------------------------------	
DefineMission (5864, "", 1611)

MisBeginTalk("<t>7. ")

MisBeginCondition(NoMission,1611)
MisBeginCondition(HasRecord,1468)
MisBeginCondition(HasRecord,1481)
MisBeginCondition(HasRecord,1482)
MisBeginCondition(HasRecord,1483)
MisBeginCondition(HasRecord,1484)
MisBeginCondition(HasRecord,1485)
MisBeginCondition(HasRecord,1475)
MisBeginCondition(HasRecord,1606)
MisBeginCondition(NoRecord,1611)
MisBeginAction(AddMission,1611)   
MisBeginAction(AddTrigger, 16111, TE_GETITEM, 2568, 1 )		
MisBeginAction(AddTrigger, 16112, TE_GETITEM, 2569, 1 )		
MisBeginAction(AddTrigger, 16113, TE_GETITEM, 2570, 1 )		
MisBeginAction(AddTrigger, 16114, TE_GETITEM, 2571, 1 )		
MisBeginAction(AddTrigger, 16115, TE_GETITEM, 2572, 1 )		
MisBeginAction(AddTrigger, 16116, TE_GETITEM, 2573, 1 )		
MisBeginAction(AddTrigger, 16117, TE_GETITEM, 2574, 1 )	
MisCancelAction(ClearMission, 1611)						                                     

MisNeed(MIS_NEED_ITEM, 2568, 1, 10, 1 )
MisNeed(MIS_NEED_ITEM, 2569, 1, 20, 1 )
MisNeed(MIS_NEED_ITEM, 2570, 1, 30, 1 )
MisNeed(MIS_NEED_ITEM, 2571, 1, 40, 1 )
MisNeed(MIS_NEED_ITEM, 2572, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 2573, 1, 60, 1 )
MisNeed(MIS_NEED_ITEM, 2574, 1, 70, 1 )

MisHelpTalk("<t>What are you still waiting for? Come now to exchange for prizes.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1611)
MisResultCondition(NoRecord,1611)
MisResultCondition(HasItem, 2568, 1 )
MisResultCondition(HasItem, 2569, 1 )
MisResultCondition(HasItem, 2570, 1 )
MisResultCondition(HasItem, 2571, 1 )
MisResultCondition(HasItem, 2572, 1 )
MisResultCondition(HasItem, 2573, 1 )
MisResultCondition(HasItem, 2574, 1 )

MisResultAction(TakeItem, 2568, 1 )
MisResultAction(TakeItem, 2569, 1 )
MisResultAction(TakeItem, 2570, 1 )
MisResultAction(TakeItem, 2571, 1 )
MisResultAction(TakeItem, 2572, 1 )
MisResultAction(TakeItem, 2573, 1 )
MisResultAction(TakeItem, 2574, 1 )

MisResultAction(ClearMission, 1611)
MisResultAction(SetRecord,  1611 )
MisResultAction(GiveItem, 2273, 1, 4)
MisResultAction(GiveItem, 2274, 1, 4)
MisResultAction(GiveItem, 3877, 3, 4)
MisResultAction(AddMoney,3000000,3000000)
MisResultAction(ShuangZiCZ)
MisResultBagNeed(3)

InitTrigger()
TriggerCondition( 1, IsItem, 2568)	
TriggerAction( 1, AddNextFlag, 1611, 10, 1 )
RegCurTrigger( 16111 )

InitTrigger()
TriggerCondition( 1, IsItem, 2569)	
TriggerAction( 1, AddNextFlag, 1611, 20, 1 )
RegCurTrigger( 16112 )

InitTrigger()
TriggerCondition( 1, IsItem, 2570)	
TriggerAction( 1, AddNextFlag, 1611, 30, 1 )
RegCurTrigger( 16113 )

InitTrigger()
TriggerCondition( 1, IsItem, 2571)	
TriggerAction( 1, AddNextFlag, 1611, 40, 1 )
RegCurTrigger( 16114 )

InitTrigger()
TriggerCondition( 1, IsItem, 2572)	
TriggerAction( 1, AddNextFlag, 1611, 50, 1 )
RegCurTrigger( 16115 )

InitTrigger()
TriggerCondition( 1, IsItem, 2573)	
TriggerAction( 1, AddNextFlag, 1611, 60, 1 )
RegCurTrigger( 16116 )

InitTrigger()
TriggerCondition( 1, IsItem, 2574)	
TriggerAction( 1, AddNextFlag, 1611, 70, 1 )
RegCurTrigger( 16117 )


-------------------------------------------------2277,2769

DefineMission(5865,"Golden October: Eating Hairy Crabs",1208)

MisBeginTalk("<t><n><t>")

MisBeginCondition(NoMission, 1208)
MisBeginCondition(NoRecord, 1208)
MisBeginAction(AddMission, 1208)
MisBeginAction(AddTrigger, 12081, TE_GETITEM, 4490, 1)          
MisBeginAction(AddTrigger, 12082, TE_GETITEM, 4426, 2)
MisBeginAction(AddTrigger, 12083, TE_GETITEM, 4393, 8)

MisCancelAction(ClearMission,1208)                         ---------

MisNeed(MIS_NEED_DESP,"<t>(1773,2517)1<y>(1783,2507)2<y>8(994,857)<y>")
MisNeed(MIS_NEED_ITEM, 4490, 1, 10, 1)               ---------
MisNeed(MIS_NEED_ITEM, 4426, 2, 20, 2)               ---------
MisNeed(MIS_NEED_ITEM, 4393, 8, 30, 8)               ---------

MisHelpTalk("<t>")
MisResultTalk("<t>14")

MisResultCondition(HasMission, 1208)
MisResultCondition(NoRecord, 1208)
MisResultCondition(HasItem, 4490, 1)
MisResultCondition(HasItem, 4426, 2)
MisResultCondition(HasItem, 4393, 8)

MisResultAction(TakeItem, 4490, 1)
MisResultAction(TakeItem, 4426, 2)
MisResultAction(TakeItem, 4393, 8)

MisResultAction(GiveItem, 0048, 1, 4)                          ---------
MisResultAction(ClearMission, 1208)                        
MisResultAction(SetRecord, 1208)
MisResultAction(ClearRecord, 1208)                          ----------can be repeated
MisResultBagNeed(1)


InitTrigger()
TriggerCondition(1, IsItem, 4490)
TriggerAction(1, AddNextFlag, 1208, 10, 1)
RegCurTrigger(12081)


InitTrigger()
TriggerCondition(1, IsItem, 4426)
TriggerAction(1, AddNextFlag, 1208, 20, 2)
RegCurTrigger(12082)


InitTrigger()
TriggerCondition(1, IsItem, 4393)
TriggerAction(1, AddNextFlag, 1208, 30, 8)
RegCurTrigger(12083)


-------------------------------------------------2277,2769

DefineMission(5866,"Golden October: Savory Mitten Crabs",1209)

MisBeginTalk("<t>")


MisBeginCondition(NoMission, 1209)
MisBeginCondition(NoRecord, 1209)
MisBeginAction(AddMission,1209)
MisBeginAction(AddTrigger, 12091, TE_GETITEM, 4342, 1)
MisBeginAction(AddTrigger, 12092, TE_GETITEM, 4793, 2)
MisBeginAction(AddTrigger, 12093, TE_GETITEM, 4500, 1)
MisBeginAction(AddTrigger, 12094, TE_GETITEM, 0057, 1)


MisCancelAction(ClearMission, 1209)

MisNeed(MIS_NEED_DESP,"<t>(1783,2507)1<y>(349, 376)2<y>(1783,2507)1<y>1<y>")
   

MisNeed(MIS_NEED_ITEM, 4342, 1, 10, 1)                       ---------
MisNeed(MIS_NEED_ITEM, 4793, 2, 20, 2)                       ---------
MisNeed(MIS_NEED_ITEM, 4500, 1, 30, 1)                       ---------
MisNeed(MIS_NEED_ITEM, 0057, 1, 40, 1)                       ---------

MisHelpTalk("<t>")
MisResultTalk("<t>20")

MisResultCondition(HasMission, 1209)
MisResultCondition(NoRecord, 1209)
MisResultCondition(HasItem, 4342, 1)
MisResultCondition(HasItem, 4793, 2)
MisResultCondition(HasItem, 4500, 1)
MisResultCondition(HasItem, 0057, 1)

MisResultAction(TakeItem, 4342, 1)
MisResultAction(TakeItem, 4793, 2)
MisResultAction(TakeItem, 4500, 1)
MisResultAction(TakeItem, 0057, 1)

MisResultAction(GiveItem, 0056, 1, 4)                                   ---------
MisResultAction(ClearMission, 1209)
MisResultAction(SetRecord, 1209)
MisResultAction(ClearRecord, 1209)                                   ----------can be repeated
MisResultBagNeed(1)


InitTrigger()
TriggerCondition(1, IsItem, 4342)
TriggerAction(1, AddNextFlag, 1209, 10, 1)
RegCurTrigger(12091)


InitTrigger()
TriggerCondition(1, IsItem, 4793)
TriggerAction(1, AddNextFlag, 1209, 20, 2)
RegCurTrigger(12092)

InitTrigger()
TriggerCondition(1, IsItem, 4500)
TriggerAction(1, AddNextFlag, 1209, 30, 1)
RegCurTrigger(12093)

InitTrigger()
TriggerCondition(1, IsItem, 0057)
TriggerAction(1, AddNextFlag, 1209, 40, 1)
RegCurTrigger(12094)


-------------------------------------------------2277,2769

DefineMission(5867, "Golden October: Raising Crab Fry", 1210)

MisBeginTalk("<t>")

MisBeginCondition(NoMission, 1210)
MisBeginCondition(NoRecord, 1210)
MisBeginAction(AddMission, 1210)
MisBeginAction(AddTrigger, 12101, TE_KILL, 273, 10)
MisBeginAction(AddTrigger, 12102, TE_KILL, 186, 10)
MisBeginAction(AddTrigger, 12103, TE_GETITEM, 4259, 5)
MisBeginAction(AddTrigger, 12104, TE_GETITEM, 4890, 5)


MisCancelAction(ClearMission, 1210)

MisNeed(MIS_NEED_DESP,"<t>(349, 376)10<y>(1341,3010)10<y>5<y>5<y>")
MisNeed(MIS_NEED_KILL, 273, 10, 10, 10)                  ----------
MisNeed(MIS_NEED_KILL, 186, 10, 20, 10)                  ----------
MisNeed(MIS_NEED_ITEM, 4259, 5, 30, 5)                  -----------
MisNeed(MIS_NEED_ITEM, 4890, 5, 40, 5)                  -----------



MisHelpTalk("<t>1<y>")
MisResultTalk("<t><y2>()<y5>(7200)<y>1000")

MisResultCondition(HasMission, 1210)
MisResultCondition(NoRecord, 1210)
MisResultCondition(HasFlag, 1210, 19)
MisResultCondition(HasFlag, 1210, 29)
MisResultCondition(HasItem, 4259, 5)
MisResultCondition(HasItem, 4890, 5)

MisResultAction(TakeItem, 4259, 5)
MisResultAction(TakeItem, 4890, 5)


MisResultAction(ClearMission, 1210)
MisResultAction(SetRecord, 1210)
MisResultAction(ClearRecord, 1210)                                   ----------can be repeated
MisResultAction(Givecrab, 0058)                                   ---------
MisResultBagNeed(1)


InitTrigger()
TriggerCondition(1, IsMonster, 273)
TriggerAction(1, AddNextFlag, 1210, 10, 10)
RegCurTrigger(12101)

InitTrigger()
TriggerCondition(1, IsMonster, 186)
TriggerAction(1, AddNextFlag, 1210, 20, 10)
RegCurTrigger(12102)

InitTrigger()
TriggerCondition(1, IsItem, 4259)
TriggerAction(1, AddNextFlag, 1210, 30, 5)
RegCurTrigger(12103)

InitTrigger()
TriggerCondition(1, IsItem, 4890)
TriggerAction(1, AddNextFlag, 1210, 40, 5)
RegCurTrigger(12104)



-----------------------------kokora---------------------------------------
-------------------------------------------------------------------------------

-------------------------------------------1	
DefineMission( 6138, "", 1211)
MisBeginTalk( "<t>30")
MisBeginCondition( CheckXSZCh,2 )				------
MisBeginCondition( HasItem , 3280,1 )
MisBeginCondition( HasNoItem, 3282)
MisBeginCondition( NoMission ,1211)
MisBeginAction( AddMission, 1211)
MisBeginAction(AddTrigger, 12111, TE_GETITEM, 3116, 30 )
MisCancelAction( ClearMission, 1211)

MisNeed(MIS_NEED_ITEM, 3116, 30, 10, 30 )
MisHelpTalk( "<t>")

MisResultTalk( "<t>")
MisResultCondition( HasMission, 1211)
MisResultCondition( HasItem, 3116, 30)
MisResultAction( TakeItem, 3116, 30)
MisResultAction( ClearMission, 1211)
MisResultAction( GiveItem, 3282, 1, 4)

InitTrigger()
TriggerCondition( 1, IsItem, 3116)	
TriggerAction( 1, AddNextFlag, 1211, 10, 30 )
RegCurTrigger( 12111 )


--------------------------------------------------------------------

DefineMission( 6139, "Rescue Swordsman Pei Meng", 1212 )
MisBeginTalk("<t>.,<bP-E-I-M-E-N-G>..<b(271,1775)>")
MisBeginCondition(NoMission, 1212)
MisBeginCondition(NoRecord,1212)
MisBeginAction(AddMission,1212)
MisBeginAction(AddTrigger, 12121, TE_GETITEM, 3854, 2)
MisBeginAction(AddTrigger, 12122, TE_GETITEM, 3858, 1)
MisBeginAction(AddTrigger, 12123, TE_GETITEM, 3863, 1)
MisBeginAction(AddTrigger, 12124, TE_GETITEM, 3865, 1)
MisBeginAction(AddTrigger, 12125, TE_GETITEM, 3862, 1)
MisBeginAction(AddTrigger, 12126, TE_GETITEM, 3856, 1)

MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"<bP-E-I-M-E-N-G>.<b(271,1775)>.(2229,2782)")
MisNeed(MIS_NEED_ITEM, 3854, 2, 10, 2)
MisNeed(MIS_NEED_ITEM, 3858, 1, 20, 1)
MisNeed(MIS_NEED_ITEM, 3863, 1, 30, 1)
MisNeed(MIS_NEED_ITEM, 3865, 1, 40, 1)
MisNeed(MIS_NEED_ITEM, 3862, 1, 50, 1)
MisNeed(MIS_NEED_ITEM, 3856, 1, 60, 1)

MisHelpTalk("<t>.")
MisResultTalk("<t>!,,.")

MisResultCondition(HasMission, 1212)
MisResultCondition(NoRecord,1212)
MisResultCondition(HasItem, 3854, 2)
MisResultCondition(HasItem, 3858, 1)
MisResultCondition(HasItem, 3863, 1)
MisResultCondition(HasItem, 3865, 1)
MisResultCondition(HasItem, 3862, 1)
MisResultCondition(HasItem, 3856, 1)

MisResultAction(TakeItem, 3854, 2 )
MisResultAction(TakeItem, 3858, 1 )
MisResultAction(TakeItem, 3863, 1 )
MisResultAction(TakeItem, 3865, 1 )
MisResultAction(TakeItem, 3862, 1 )
MisResultAction(TakeItem, 3856, 1 )


MisResultAction(GiveItem, 3673,1,4)------------
MisResultAction(ClearMission, 1212)
--MisResultAction(ZSSTOP)
MisResultAction(SetRecord, 1212)
MisResultBagNeed(1)



InitTrigger()
TriggerCondition( 1, IsItem, 3854)	
TriggerAction( 1, AddNextFlag, 1212, 10, 2 )
RegCurTrigger( 12121 )

InitTrigger()
TriggerCondition( 1, IsItem, 3858)	
TriggerAction( 1, AddNextFlag, 1212, 20, 1 )
RegCurTrigger( 12122 )

InitTrigger()
TriggerCondition( 1, IsItem, 3863)	
TriggerAction( 1, AddNextFlag, 1212, 30, 1 )
RegCurTrigger( 12123 )

InitTrigger()
TriggerCondition( 1, IsItem, 3865)	
TriggerAction( 1, AddNextFlag, 1212, 40, 1 )
RegCurTrigger( 12124 )

InitTrigger()
TriggerCondition( 1, IsItem, 3862)	
TriggerAction( 1, AddNextFlag, 1212, 50, 1 )
RegCurTrigger( 12125 )

InitTrigger()
TriggerCondition( 1, IsItem, 3856)	
TriggerAction( 1, AddNextFlag, 1212, 60, 1 )
RegCurTrigger( 12126 )

--------------------------------------------------------------------
DefineMission( 6140, "Information from the Gatekeeper", 1213 )
MisBeginTalk("<t>,.,1(263,260)")
			
MisBeginCondition(NoMission, 1213)
MisBeginCondition(NoRecord,1213)
MisBeginCondition(HasMission, 1212)
MisBeginCondition(NoRecord, 1212)
MisBeginAction(AddMission,1213)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"1(263,260)")

MisHelpTalk("<t>,?")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6141, "Information from the Gatekeeper", 1213, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>.?")
MisResultCondition(NoRecord, 1213)
MisResultCondition(HasMission,1213)
MisResultAction(ClearMission,1213)
MisResultAction(SetRecord, 1213)


------------------------------------------------------
DefineMission(6142,"The Manservant's Discontent",1214)

  MisBeginTalk("<t> ,.?,.,.")

  MisBeginCondition(NoRecord,1214)
  MisBeginCondition(NoMission,1214)
  MisBeginCondition(HasRecord, 1213)
  MisBeginAction(AddMission,1214)
  MisBeginAction(AddTrigger, 12141, TE_GETITEM, 4739, 25 )
  MisBeginAction(AddTrigger, 12142, TE_GETITEM, 4740, 25 )          
  MisBeginAction(AddTrigger, 12143, TE_GETITEM, 4741, 25 )               
  MisBeginAction(AddTrigger, 12144, TE_GETITEM, 1486, 25 )               
  MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

  MisNeed(MIS_NEED_DESP,"25!")
  MisNeed(MIS_NEED_ITEM, 4739, 25,  1, 25 )
  MisNeed(MIS_NEED_ITEM, 4740, 25, 26, 25 )
  MisNeed(MIS_NEED_ITEM, 4741, 25,  51, 25 )
  MisNeed(MIS_NEED_ITEM, 1486, 25,  76, 25 )

  MisHelpTalk("<t>,.")  
  MisResultTalk("<t>,.,P.")
  MisResultCondition(HasMission,1214 )
  MisResultCondition(NoRecord,1214)
  MisResultCondition(HasItem, 4739, 25 )
  MisResultCondition(HasItem, 4740, 25 )
  MisResultCondition(HasItem, 4741, 25 )
  MisResultCondition(HasItem, 1486, 25 )

	MisResultAction(TakeItem, 4739, 25 )
MisResultAction(TakeItem, 4740, 25 )
MisResultAction(TakeItem, 4741, 25 )
MisResultAction(TakeItem, 1486, 25 )

  MisResultAction(GiveItem, 3865, 1, 4 )
  MisResultAction(ClearMission, 1214 )
  MisResultAction(SetRecord, 1214)
  MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4739)	
TriggerAction( 1, AddNextFlag, 1214, 1, 25 )
RegCurTrigger( 12141 )

InitTrigger()
TriggerCondition( 1, IsItem, 4740)	
TriggerAction( 1, AddNextFlag, 1214, 26, 25 )
RegCurTrigger( 12142 )

InitTrigger()
TriggerCondition( 1, IsItem, 4741)	
TriggerAction( 1, AddNextFlag, 1214, 51, 25 )
RegCurTrigger( 12143 )

InitTrigger()
TriggerCondition( 1, IsItem, 1486)	
TriggerAction( 1, AddNextFlag, 1214, 76, 25 )
RegCurTrigger( 12144 )

--------------------------------------------------------------------
DefineMission( 6143, "The Strange Maid", 1215 )
MisBeginTalk("<t>,2(151,134)..")
			
MisBeginCondition(NoMission, 1215)
MisBeginCondition(NoRecord,1215)
MisBeginCondition(HasRecord, 1214)
MisBeginAction(AddMission,1215)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"2(151,134)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6144, "The Strange Maid", 1215, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.")
MisResultCondition(NoRecord, 1215)
MisResultCondition(HasMission,1215)
MisResultAction(ClearMission,1215)
MisResultAction(SetRecord, 1215)

------------------------------------------------------
DefineMission(6145,"Cure for Loose Lips",1216)

  MisBeginTalk("<t> ,.,,..")

  MisBeginCondition(NoRecord,1216)
  MisBeginCondition(NoMission,1216)
  MisBeginCondition(HasRecord, 1215)
  MisBeginAction(AddMission,1216)
  MisBeginAction(AddTrigger, 12161, TE_GETITEM, 4742, 35 )
  MisBeginAction(AddTrigger, 12162, TE_GETITEM, 4743, 35 )          
  MisBeginAction(AddTrigger, 12163, TE_GETITEM, 4745, 35 )                           
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

  MisNeed(MIS_NEED_DESP,"35!")
  MisNeed(MIS_NEED_ITEM, 4742, 35,  1, 35 )
  MisNeed(MIS_NEED_ITEM, 4743, 35, 36, 35 )
  MisNeed(MIS_NEED_ITEM, 4745, 35,  71, 35 )

  MisHelpTalk("<t>.")  
  MisResultTalk("<t>..")
  MisResultCondition(HasMission,1216 )
  MisResultCondition(NoRecord,1216)
  MisResultCondition(HasItem, 4742, 35 )
  MisResultCondition(HasItem, 4743, 35 )
  MisResultCondition(HasItem, 4745, 35 )

	MisResultAction(TakeItem, 4742, 35 )
MisResultAction(TakeItem, 4743, 35 )
MisResultAction(TakeItem, 4745, 35 )


  MisResultAction(GiveItem, 3854, 1, 4 )
  MisResultAction(ClearMission, 1216 )
  MisResultAction(SetRecord, 1216)
  MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4742)	
TriggerAction( 1, AddNextFlag, 1216, 1, 35 )
RegCurTrigger( 12161 )

InitTrigger()
TriggerCondition( 1, IsItem, 4743)	
TriggerAction( 1, AddNextFlag, 1216, 36, 35 )
RegCurTrigger( 12162 )

InitTrigger()
TriggerCondition( 1, IsItem, 4745)	
TriggerAction( 1, AddNextFlag, 1216, 71, 35 )
RegCurTrigger(12163 )
--------------------------------------------------------------------
DefineMission( 6146, "Locke's Little Secretary", 1217 )
MisBeginTalk("<t>,3(63,311).,,,,.")
			
MisBeginCondition(NoMission, 1217)
MisBeginCondition(NoRecord,1217)
MisBeginCondition(HasRecord, 1216)
MisBeginAction(AddMission,1217)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"3(63,311)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6147, "Locke's Little Secretary", 1217, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,.")
MisResultCondition(NoRecord, 1217)
MisResultCondition(HasMission,1217)
MisResultAction(ClearMission,1217)
MisResultAction(SetRecord, 1217)

--------------------------------------------------------------------------

DefineMission( 6148, "The Secretary's Dream", 1218 )
MisBeginTalk("<t>,..")
MisBeginCondition(NoMission, 1218)
MisBeginCondition(NoRecord,1218)
MisBeginCondition(HasRecord, 1217)
MisBeginAction(AddMission,1218)
MisBeginAction(AddTrigger, 12181, TE_GETITEM, 1501, 20)
MisBeginAction(AddTrigger, 12182, TE_GETITEM, 1490, 20)
MisBeginAction(AddTrigger, 12183, TE_GETITEM, 4748, 20)
MisBeginAction(AddTrigger, 12184, TE_GETITEM, 4749, 20)
MisBeginAction(AddTrigger, 12185, TE_GETITEM, 4725, 20)
MisBeginAction(AddTrigger, 12186, TE_GETITEM, 4747, 20)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"20.")
MisNeed(MIS_NEED_ITEM, 1501, 20, 1, 20)
MisNeed(MIS_NEED_ITEM, 1490, 20, 21, 20)
MisNeed(MIS_NEED_ITEM, 4748, 20, 41, 20)
MisNeed(MIS_NEED_ITEM, 4749, 20, 61, 20)
MisNeed(MIS_NEED_ITEM, 4725, 20, 81, 20)
MisNeed(MIS_NEED_ITEM, 4747, 20, 101, 20)

MisHelpTalk("<t>.")
MisResultTalk("<t>,,.")

MisResultCondition(HasMission, 1218)
MisResultCondition(NoRecord,1218)
MisResultCondition(HasItem, 1501, 20)
MisResultCondition(HasItem, 1490, 20)
MisResultCondition(HasItem, 4748, 20)
MisResultCondition(HasItem, 4749, 20)
MisResultCondition(HasItem, 4725, 20)
MisResultCondition(HasItem, 4747, 20)

MisResultAction(TakeItem, 1501, 20 )
MisResultAction(TakeItem, 1490, 20 )
MisResultAction(TakeItem, 4748, 20 )
MisResultAction(TakeItem, 4749, 20 )
MisResultAction(TakeItem, 4725, 20 )
MisResultAction(TakeItem, 4747, 20 )


MisResultAction(GiveItem, 3858,1,4)------------i
MisResultAction(ClearMission, 1218)
MisResultAction(SetRecord, 1218)
MisResultBagNeed(1)



InitTrigger()
TriggerCondition( 1, IsItem, 1501)	
TriggerAction( 1, AddNextFlag, 1218, 1, 20 )
RegCurTrigger( 12181 )

InitTrigger()
TriggerCondition( 1, IsItem, 1490)	
TriggerAction( 1, AddNextFlag, 1218, 21, 20 )
RegCurTrigger( 12182 )

InitTrigger()
TriggerCondition( 1, IsItem, 4748)	
TriggerAction( 1, AddNextFlag, 1218, 41, 20 )
RegCurTrigger( 12183 )

InitTrigger()
TriggerCondition( 1, IsItem, 4749)	
TriggerAction( 1, AddNextFlag, 1218, 61, 20 )
RegCurTrigger( 12184 )

InitTrigger()
TriggerCondition( 1, IsItem, 4725)	
TriggerAction( 1, AddNextFlag, 1218, 81, 20 )
RegCurTrigger( 12185 )

InitTrigger()
TriggerCondition( 1, IsItem, 4747)	
TriggerAction( 1, AddNextFlag, 1218, 101, 20 )
RegCurTrigger( 12186 )

--------------------------------------------------------------------
DefineMission( 6149, "The Stern Guardian", 1219 )
MisBeginTalk("<t>4(261,70),.")
			
MisBeginCondition(NoMission, 1219)
MisBeginCondition(NoRecord,1219)
MisBeginCondition(HasRecord, 1218)
MisBeginAction(AddMission,1219)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"4(261,70)")

MisHelpTalk("<t>.")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6150, "The Stern Guardian", 1219, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,.")
MisResultCondition(NoRecord, 1219)
MisResultCondition(HasMission,1219)
MisResultAction(ClearMission,1219)
MisResultAction(SetRecord, 1219)

--------------------------------------------------------------------------

DefineMission( 6151, "Favor After Favor", 1220 )
MisBeginTalk("<t>?!.,10,.,.")
MisBeginCondition(NoMission, 1220)
MisBeginCondition(NoRecord,1220)
MisBeginCondition(HasRecord, 1219)
MisBeginAction(AddMission,1220)
MisBeginAction(AddTrigger, 12201, TE_GETITEM, 4750,50)
MisBeginAction(AddTrigger, 12202, TE_GETITEM, 4763, 50)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"50.")
MisNeed(MIS_NEED_ITEM, 4750, 50, 1, 50)
MisNeed(MIS_NEED_ITEM, 4763, 50, 51, 50)


MisHelpTalk("<t>.")
MisResultTalk("<t>,,,.")

MisResultCondition(HasMission, 1220)
MisResultCondition(NoRecord,1220)
MisResultCondition(HasItem, 4750, 50)
MisResultCondition(HasItem, 4763,50)
MisResultAction(TakeItem, 4750, 50 )
MisResultAction(TakeItem, 4763, 50 )

MisResultAction(GiveItem, 3862,1,4)----------m
MisResultAction(ClearMission, 1220)
MisResultAction(SetRecord, 1220)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4750)	
TriggerAction( 1, AddNextFlag, 1220, 1, 50 )
RegCurTrigger( 12201 )

InitTrigger()
TriggerCondition( 1, IsItem, 4763)	
TriggerAction( 1, AddNextFlag, 1220, 21, 50 )
RegCurTrigger( 12202 )

--------------------------------------------------------------------
DefineMission( 6152, "Yet Another Favor", 1221 )
MisBeginTalk("<t>,,.<n><t>5(542,54)")
			
MisBeginCondition(NoMission, 1221)
MisBeginCondition(NoRecord,1221)
MisBeginCondition(HasRecord, 1220)
MisBeginAction(AddMission,1221)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"5(542,54)")

MisHelpTalk("<t>,,....")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6153, "Yet Another Favor", 1221, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,?")
MisResultCondition(NoRecord, 1221)
MisResultCondition(HasMission,1221)
MisResultAction(ClearMission,1221)
MisResultAction(SetRecord, 1221)


--------------------------------------------------------------------------

DefineMission( 6154, "The Butler's Troubles", 1222 )
MisBeginTalk("<t>?.,.,,,")
MisBeginCondition(NoMission, 1222)
MisBeginCondition(NoRecord,1222)
MisBeginCondition(HasRecord, 1221)
MisBeginAction(AddMission,1222)
MisBeginAction(AddTrigger, 12221, TE_GETITEM, 4770, 35)
MisBeginAction(AddTrigger, 12222, TE_GETITEM, 4766, 35)
MisBeginAction(AddTrigger, 12223, TE_GETITEM, 4772, 35)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP," 35.")
MisNeed(MIS_NEED_ITEM, 4770, 35, 1, 35)
MisNeed(MIS_NEED_ITEM, 4766, 35, 36, 35)
MisNeed(MIS_NEED_ITEM, 4772, 35, 71, 35)

MisHelpTalk("<t>,,.")
MisResultTalk("<t>.")

MisResultCondition(HasMission, 1222)
MisResultCondition(NoRecord,1222)
MisResultCondition(HasItem, 4770, 35)
MisResultCondition(HasItem, 4766, 35)
MisResultCondition(HasItem, 4772, 35)

MisResultAction(TakeItem, 4770, 35 )
MisResultAction(TakeItem, 4766, 35 )
MisResultAction(TakeItem, 4772, 35 )

MisResultAction(GiveItem, 3863,1,4)----------n
MisResultAction(ClearMission, 1222)
MisResultAction(SetRecord, 1222)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4770)	
TriggerAction( 1, AddNextFlag, 1222, 1, 35 )
RegCurTrigger( 12221 )

InitTrigger()
TriggerCondition( 1, IsItem, 4766)	
TriggerAction( 1, AddNextFlag, 1222, 36, 35 )
RegCurTrigger( 12222 )

InitTrigger()
TriggerCondition( 1, IsItem, 4772)	
TriggerAction( 1, AddNextFlag, 1222, 71, 35 )
RegCurTrigger( 12223 )

-------------------------------------------------------------------
DefineMission( 6155, "The Butler's Revenge", 1223 )
MisBeginTalk("<t>~~,,,...,,.,,,~~")
			
MisBeginCondition(NoMission, 1223)
MisBeginCondition(NoRecord,1223)
MisBeginCondition(HasRecord, 1222)
MisBeginAction(AddMission,1223)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"6(541,268)")

MisHelpTalk("<t>,,,...")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6156, "The Butler's Revenge", 1223, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?")
MisResultCondition(NoRecord, 1223)
MisResultCondition(HasMission,1223)
MisResultAction(ClearMission,1223)
MisResultAction(SetRecord, 1223)
--------------------------------------------------------------------------

DefineMission( 6157, "The Beauty's Strange Hobby", 1224 )
MisBeginTalk("<t>,.,.,,.")
MisBeginCondition(NoMission, 1224)
MisBeginCondition(NoRecord,1224)
MisBeginCondition(HasRecord, 1223)
MisBeginAction(AddMission,1224)
MisBeginAction(AddTrigger, 12241, TE_GETITEM, 1503, 110)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"110.")
MisNeed(MIS_NEED_ITEM, 1503, 110, 1, 110)

MisHelpTalk("<t>,.")
MisResultTalk("<t>,,,.")

MisResultCondition(HasMission, 1224)
MisResultCondition(NoRecord,1224)
MisResultCondition(HasItem, 1503, 110)
MisResultAction(TakeItem, 1503, 110 )

MisResultAction(GiveItem, 3856,1,4)----------G
MisResultAction(ClearMission, 1224)
MisResultAction(SetRecord, 1224)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 1503)	
TriggerAction( 1, AddNextFlag, 1224, 1, 110 )
RegCurTrigger( 12241 )

----------------------------------------------------------------
DefineMission( 6158, "Wangcai's Grudge", 1225 )
MisBeginTalk("<t>.,.,..")
			
MisBeginCondition(NoMission, 1225)
MisBeginCondition(NoRecord,1225)
MisBeginCondition(HasRecord, 1224)
MisBeginAction(AddMission,1225)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"(154,912)")

MisHelpTalk("<t>...")
MisResultCondition(AlwaysFailure)	
-------------------------------------------------
DefineMission(6159, "Wangcai's Grudge", 1225, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>!???")
MisResultCondition(NoRecord, 1225)
MisResultCondition(HasMission,1225)
MisResultAction(ClearMission,1225)
MisResultAction(SetRecord, 1225)


--------------------------------------------------------------------------

DefineMission( 6160, "Don't Mess with Wangcai!", 1226 )
MisBeginTalk("<t>,,..")
MisBeginCondition(NoMission, 1226)
MisBeginCondition(NoRecord,1226)
MisBeginCondition(HasRecord, 1225)
MisBeginAction(AddMission,1226)
MisBeginAction(AddTrigger, 12261, TE_GETITEM, 4884, 99)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"99.")
MisNeed(MIS_NEED_ITEM, 4884, 99, 1, 99)

MisHelpTalk("<t>?")
MisResultTalk("<t><b(2229,2782)>.")

MisResultCondition(HasMission, 1226)
MisResultCondition(NoRecord,1226)
MisResultCondition(HasItem, 4884, 99)
MisResultAction(TakeItem, 4884, 99 )

MisResultAction(GiveItem, 3854,1,4)----------E
MisResultAction(ClearMission, 1226)
MisResultAction(SetRecord, 1226)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4884)	
TriggerAction( 1, AddNextFlag, 1226, 1, 99 )
RegCurTrigger( 12261 )


-----------------------------------------------------!----------
DefineMission( 6161, "Oh! No Key", 1227 )
MisBeginTalk("<t>!.(2423,3186),,")
			
MisBeginCondition(NoMission, 1227)
MisBeginCondition(NoRecord,1227)
MisBeginCondition(HasRecord, 1212)
MisBeginAction(AddMission,1227)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"(2423,3186)")

MisHelpTalk("<t>!...")
MisResultCondition(AlwaysFailure)	
-----------------------------------------!--------
DefineMission(6162, "Oh! No Key", 1227, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>?")
MisResultCondition(NoRecord, 1227)
MisResultCondition(HasMission,1227)
MisResultAction(ClearMission,1227)
MisResultAction(SetRecord, 1227)



--------------------------------------------------------------------------

DefineMission( 6163, "The Tour Guide's Request", 1228 )
MisBeginTalk("<t>,.,")
MisBeginCondition(NoMission, 1228)
MisBeginCondition(NoRecord,1228)
MisBeginCondition(HasRecord, 1227)
MisBeginAction(AddMission,1228)
MisBeginAction(AddTrigger, 12281, TE_GETITEM, 0154, 20)
MisBeginAction(AddTrigger, 12282, TE_GETITEM, 0156, 20)
MisBeginAction(AddTrigger, 12283, TE_GETITEM, 0158, 20)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"20.")
MisNeed(MIS_NEED_ITEM, 0154, 20, 1, 20)
MisNeed(MIS_NEED_ITEM, 0156, 20, 21, 20)
MisNeed(MIS_NEED_ITEM, 0158, 20, 41, 20)

MisHelpTalk("<t>,.")
MisResultTalk("<t>,.")

MisResultCondition(HasMission, 1228)
MisResultCondition(NoRecord,1228)
MisResultCondition(HasItem, 0154, 20)
MisResultCondition(HasItem, 0156, 20)
MisResultCondition(HasItem, 0158, 20)

MisResultAction(TakeItem, 0154, 20 )
MisResultAction(TakeItem, 0156, 20 )
MisResultAction(TakeItem, 0158, 20 )

MisResultAction(ClearMission, 1228)
MisResultAction(SetRecord, 1228)


InitTrigger()
TriggerCondition( 1, IsItem, 0154)	
TriggerAction( 1, AddNextFlag, 1228, 1, 20 )
RegCurTrigger( 12281 )

InitTrigger()
TriggerCondition( 1, IsItem, 0156)	
TriggerAction( 1, AddNextFlag, 1228, 36, 20 )
RegCurTrigger( 12282 )

InitTrigger()
TriggerCondition( 1, IsItem, 0158)	
TriggerAction( 1, AddNextFlag, 1228, 71, 20 )
RegCurTrigger( 12283 )


	--------------------------------------------------------------------------

DefineMission( 6164, "Consoling the Spirits", 1229 )
MisBeginTalk("<t>,,.")
MisBeginCondition(NoMission, 1229)
MisBeginCondition(NoRecord,1229)
MisBeginCondition(HasRecord, 1228)
MisBeginAction(AddMission,1229)
MisBeginAction(AddTrigger, 12291, TE_GETITEM, 3436, 50)
MisBeginAction(AddTrigger, 12292, TE_GETITEM, 3434, 50)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"50.")
MisNeed(MIS_NEED_ITEM, 3436, 50, 1, 50)
MisNeed(MIS_NEED_ITEM, 3434, 50, 51, 50)


MisHelpTalk("<t>.")
MisResultTalk("<t>,..")

MisResultCondition(HasMission, 1229)
MisResultCondition(NoRecord,1229)
MisResultCondition(HasItem, 3436, 50)
MisResultCondition(HasItem, 3434, 50)

MisResultAction(TakeItem, 3436, 50 )
MisResultAction(TakeItem, 3434, 50 )

MisResultAction(GiveItem, 3674,1,4)----------
MisResultAction(ClearMission, 1229)
MisResultAction(SetRecord, 1229)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 3436)	
TriggerAction( 1, AddNextFlag, 1229, 1, 50 )
RegCurTrigger( 12291 )

InitTrigger()
TriggerCondition( 1, IsItem, 3434)	
TriggerAction( 1, AddNextFlag, 1229, 51, 50 )
RegCurTrigger( 12292 )

-------------------------------------------------------------
DefineMission( 6165, "One Key Opens One Lock", 1230 )
MisBeginTalk("<t>.")
			
MisBeginCondition(NoMission, 1230)
MisBeginCondition(NoRecord,1230)
MisBeginCondition(HasRecord, 1229)
MisBeginAction(AddMission,1230)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed(MIS_NEED_DESP,"(2229,2782).")

MisHelpTalk("<t>...")
MisResultCondition(AlwaysFailure)	
--------------------------------------------------
DefineMission(6166, "One Key Opens One Lock", 1230, COMPLETE_SHOW )

MisBeginCondition(AlwaysFailure )

MisResultTalk("<t>,,..,.,?")
MisResultCondition(NoRecord, 1230)
MisResultCondition(HasMission,1230)
MisResultCondition(HasItem, 3673, 1)
MisResultCondition(HasItem, 3674, 1)
MisResultAction(TakeItem, 3673, 1 )
MisResultAction(TakeItem, 3674, 1 )
MisResultAction(GiveItem, 3672,1,4)----------
MisResultAction(ClearMission,1230)
MisResultAction(SetRecord, 1230)
MisResultBagNeed(1)

MisResultAction(ClearRecord, 1212)---------------can be repeated
MisResultAction(ClearRecord, 1213)---------------can be repeated
MisResultAction(ClearRecord, 1214)---------------can be repeated
MisResultAction(ClearRecord, 1215)---------------can be repeated
MisResultAction(ClearRecord, 1216)---------------can be repeated
MisResultAction(ClearRecord, 1217)---------------can be repeated
MisResultAction(ClearRecord, 1218)---------------can be repeated
MisResultAction(ClearRecord, 1219)---------------can be repeated
MisResultAction(ClearRecord, 1220)---------------can be repeated
MisResultAction(ClearRecord, 1221)---------------can be repeated
MisResultAction(ClearRecord, 1222)---------------can be repeated
MisResultAction(ClearRecord, 1223)---------------can be repeated
MisResultAction(ClearRecord, 1224)---------------can be repeated
MisResultAction(ClearRecord, 1225)---------------can be repeated
MisResultAction(ClearRecord, 1226)---------------can be repeated
MisResultAction(ClearRecord, 1227)---------------can be repeated
MisResultAction(ClearRecord, 1228)---------------can be repeated
MisResultAction(ClearRecord, 1229)---------------can be repeated
MisResultAction(ClearRecord, 1230)---------------can be repeated


--------------------------------------------------------	
DefineMission( 6167, "The Heros Relic", 1231)
MisBeginTalk( "<t> You are not a kid anymore, its time to leave this place...<n><t> Here is the treasured relic of a brave fighter, do you want it? <n><t>You want it? If you want it, just tell me. I can see your eyes are filled with desire. I am serious, you really need to tell me if you want it I ask you again, do you really want it? Ok, here you are.")
MisBeginCondition( LvCheck, ">", 44)
MisBeginCondition( LvCheck, "<", 66)
MisBeginCondition( NoMission, 1231)
MisBeginCondition( NoRecord, 1231)
MisBeginAction( AddMission, 1231)
MisBeginAction( AddTrigger, 12311, TE_KILL, 514, 30 )
MisCancelAction(ClearMission, 1231)

MisNeed(MIS_NEED_KILL, 514, 30, 10, 30)

MisResultTalk("<t>Muwahhaha.you killed so many monsters. I told you the wrong number before, actually, 10 are enough. But you have already killed 30 monsters, which is ok too. <n><t>Well, I guess Im too old. Why do you stand here?<n><t>Oh, I almost forgot, I havent give you the rewards.")
MisHelpTalk("<t>Why dont you get a move on? The Huge Spiky Stramonium is in Ascaron (440,1320).")
MisResultCondition( HasFlag, 1231, 39 )
MisResultCondition( HasMission, 1231)
MisResultCondition( NoRecord, 1231)
MisResultAction( ClearMission, 1231)
MisResultAction( SetRecord, 1231)
MisResultAction( AddExpPer, 1)
MisResultAction( AddMoney, 1000)

InitTrigger() 
TriggerCondition( 1, IsMonster, 514 )	
TriggerAction( 1, AddNextFlag, 1231, 10, 30 )
RegCurTrigger(12311)

----------------------------------------
DefineMission( 6168, "The Heros Relic", 1232)
MisBeginTalk( "<t>You are a very brave gambler. High risk, high return<n><t>But I must warn you, if you go adventuring without abilities, all your efforts will be in vain. <n><t> This is your next test, and I will be waiting for you to return.")
MisBeginCondition( NoMission, 1232)
MisBeginCondition( NoRecord, 1232)
MisBeginCondition( HasRecord, 1231)
MisBeginAction( AddMission, 1232)
MisBeginAction( AddTrigger, 12321, TE_KILL, 284, 50 )
MisCancelAction( ClearMission, 1232)

MisNeed( MIS_NEED_KILL, 284, 50, 10, 50)

MisResultTalk( "<t>Welcome back! Your performance has been recognized. But I want to tell you that all these tasks were designed by me to test you.<n><t>Dont be mad at me! I knew the location of the treasure, but the lock keeping the treasure from being stolen had rusted, and I heard that only Terra Gold Ore can be used to open the lock. You must go and find some for me. <n><t>I know I shouldnt have lied to you, ok, I promise, once you give me the Terra Gold Ore, I will give you all the treasure.")
MisHelpTalk( "<t> The Guardian Angel usually walk around Ascaron (904,1280).")
MisResultCondition( HasMission, 1232)
MisResultCondition( NoRecord, 1232)
MisResultCondition( HasFlag, 1232, 59)
MisResultAction( ClearMission, 1232)
MisResultAction( SetRecord, 1232)
MisResultAction( AddExpPer, 1)
MisResultAction( AddMoney, 3000)

InitTrigger() 
TriggerCondition( 1, IsMonster, 284 )	
TriggerAction( 1, AddNextFlag, 1232, 10, 50 )
RegCurTrigger(12321)

----------------------------------------
DefineMission( 6169, "The Heros Relic", 1233)
MisBeginTalk( "<t>Young man, you are very kind! God bless you, this is the location of Terra Gold Ore (Magical Ocean 1381,3134), here you are, I think it will be helpful.")
MisBeginCondition( NoMission, 1233)
MisBeginCondition( NoRecord, 1233)
MisBeginCondition( HasRecord, 1232)
MisBeginAction( AddMission, 1233)
MisBeginAction( AddTrigger, 12331, TE_KILL, 65, 10 )
MisBeginAction( AddTrigger, 12332, TE_GETITEM, 1783, 50)
MisCancelAction( ClearMission, 1233)

MisNeed( MIS_NEED_KILL, 65, 10, 10, 10)
MisNeed( MIS_NEED_ITEM, 1783, 50, 20, 50)

MisResultTalk( "<t>I have finally found the legendary treasure! <n><t> What the hell is this? I dont get it! <n><t> Damn it! Why do I only get this crap after Ive made such an effort? <n><t> The treasure is useless to me so you can have it.")
MisHelpTalk( "<t>Go go go!!")
MisResultCondition( HasMission, 1233)
MisResultCondition( NoRecord, 1233)
MisResultCondition( HasFlag, 1233, 19)
MisResultCondition( HasItem, 1783, 50)
MisResultAction( TakeItem, 1783, 50)
MisResultAction( ClearMission, 1233)
MisResultAction( SetRecord, 1233)
MisResultAction( AddExpPer, 1)
MisResultAction( AddMoney , 5000)
MisResultAction( AddExpAndType, 2, 80000, 80000)

InitTrigger()
TriggerCondition(1, IsMonster, 65)
TriggerAction(1, AddNextFlag, 1233, 10, 10)
RegCurTrigger(12331)

InitTrigger()
TriggerCondition(1, IsItem, 1783)
TriggerAction(1, AddNextFlag, 1233, 20, 50)
RegCurTrigger(12332)

----------------------------------------------
DefineMission( 6170, "The Brave Love", 1234)
MisBeginTalk( "<t> Afterwards, I found out that the so-called treasure was the love letter of a hero.<n><t> Since then many loathsome monsters had been bothering our village, and the hero had to hide his love for the girl as he fought against those monsters.<n><t> After many years, we were able to lead a peaceful life again. We all respect this hero, because to save our village he gave up that girl. <n><t> Can you deliver this love letter to that girl? <n><t> It has been said that the girl lives on a small island in the Treasure Gulf.<n><t> Hey, dont be so impatient, I havent finished talking A gift prepared by the hero was stolen by monsters, you should get it back and give it to his girl.")
MisBeginCondition( NoMission, 1234)
MisBeginCondition( NoRecord, 1234)
MisBeginCondition( HasRecord, 1233)
MisBeginAction( AddMission, 1234)
MisBeginAction( GiveItem, 2671, 1, 4)
MisBeginAction( AddTrigger, 12341, TE_GETITEM, 2671, 1)
MisBeginAction( AddTrigger, 12342, TE_GETITEM, 4503, 1)
MisBeginAction( AddTrigger, 12343, TE_GETITEM, 3361, 89)
MisCancelAction( ClearMission, 1234)
MisBeginBagNeed(1)

MisHelpTalk( "<t>Now go!")
MisNeed( MIS_NEED_DESP, " Give the heros relic,89 pearls and 1 Crown to Elizabeth in the Treasure Gulf.")
MisNeed( MIS_NEED_ITEM, 2671, 1, 10, 1)
MisNeed( MIS_NEED_ITEM, 4503, 1, 20, 1)
MisNeed( MIS_NEED_ITEM, 3361, 89, 30, 89)

MisResultCondition( AlwaysFailure )
---------------------------------------------
DefineMission( 6171, "The Brave Love", 1234, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Thank you so much, so many years have passed to finally hear from him <n><t>Please accept this reward for bringing me so much joy.")
MisResultCondition( HasItem, 2671, 1)
MisResultCondition( HasItem, 4503, 1)
MisResultCondition( HasItem, 3361, 89)
MisResultCondition( HasMission, 1234)
MisResultCondition( NoRecord, 1234)
MisResultAction( TakeItem, 2671, 1)
MisResultAction( TakeItem, 4503, 1)
MisResultAction( TakeItem, 3361, 89)
MisResultAction( ClearMission, 1234)
MisResultAction( SetRecord, 1234)
MisResultAction( AddExpPer, 2)
MisResultAction( AddMoney, 300000)
MisResultAction( AddReadingBook )                       ------

InitTrigger()
TriggerCondition(1, IsItem, 2671)
TriggerAction(1, AddNextFlag, 1234, 10, 1)
RegCurTrigger(12341)

InitTrigger()
TriggerCondition(1, IsItem, 4503)
TriggerAction(1, AddNextFlag, 1234, 20, 1)
RegCurTrigger(12342)

InitTrigger()
TriggerCondition(1, IsItem, 3361)
TriggerAction(1, AddNextFlag, 1234, 30, 89)
RegCurTrigger(12343)


-------------------------------------------------------
DefineMission( 6172, "The Brave Love", 1235)
MisBeginTalk( "<t> There is still much room for improvement. Youd better visit Blurry, he will teach you a lot.")
MisBeginCondition( NoMission, 1235)
MisBeginCondition( NoRecord, 1235)
MisBeginCondition( HasRecord, 1234)
MisBeginAction( AddMission, 1235)
MisCancelAction( ClearMission, 1235)

MisHelpTalk( "<t> Blurry will give you some directions.")
MisNeed( MIS_NEED_DESP, "To Find Blurry in Argent City.")

MisResultCondition( AlwaysFailure )
--------------------------------------------------------
DefineMission( 6173, "The Brave Love", 1235, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>Well done, I should have more tasks to assign to you.")
MisResultCondition( HasMission, 1235)
MisResultCondition( NoRecord, 1235)
MisResultAction( ClearMission, 1235)
MisResultAction( SetRecord, 1235)
MisResultAction( AddMoney, 88)

MisResultAction(ClearRecord, 1231)---------------can be repeated
MisResultAction(ClearRecord, 1232)---------------can be repeated
MisResultAction(ClearRecord, 1233)---------------can be repeated
MisResultAction(ClearRecord, 1234)---------------can be repeated
MisResultAction(ClearRecord, 1235)---------------can be repeated

-----------------------------------------
DefineMission( 6174, "Fortune in All Seasons", 1236)
MisBeginTalk( "<t>4(2722,3137),,.")
MisBeginCondition( NoMission, 1236)
MisBeginCondition( NoRecord, 1236)
MisBeginCondition( LvCheck, "<", 80)
MisBeginAction( AddMission, 1236)
MisBeginAction( AddTrigger, 12361, TE_GETITEM, 2969, 1)
MisBeginAction( AddTrigger, 12362, TE_GETITEM, 2970, 1)
MisBeginAction( AddTrigger, 12363, TE_GETITEM, 2971, 1)
MisBeginAction( AddTrigger, 12364, TE_GETITEM, 2972, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(),(),(),()(3302,2501),(2722,3137).")
MisNeed( MIS_NEED_ITEM, 2969, 1, 10, 1)
MisNeed( MIS_NEED_ITEM, 2970, 1, 20, 1)
MisNeed( MIS_NEED_ITEM, 2971, 1, 30, 1)
MisNeed( MIS_NEED_ITEM, 2972, 1, 40, 1)

MisResultTalk( "<t>!4,!.")
MisHelpTalk( "<t>,!")
MisResultCondition( HasMission, 1236)
MisResultCondition( NoRecord, 1236)
MisResultCondition( HasItem, 2969, 1)
MisResultCondition( HasItem, 2970, 1)
MisResultCondition( HasItem, 2971, 1)
MisResultCondition( HasItem, 2972, 1)
MisResultAction( TakeItem, 2969, 1)
MisResultAction( TakeItem, 2970, 1)
MisResultAction( TakeItem, 2971, 1)
MisResultAction( TakeItem, 2972, 1)
MisResultAction( ClearMission, 1236)
MisResultAction( SetRecord, 1236)
MisResultAction( AddExpNextLv1 )

InitTrigger()
TriggerCondition( 1, IsItem, 2969)	
TriggerAction( 1, AddNextFlag, 1236, 10, 1 )
RegCurTrigger( 12361 )
InitTrigger()
TriggerCondition( 1, IsItem, 2970)	
TriggerAction( 1, AddNextFlag, 1236, 20, 1 )
RegCurTrigger( 12362 )
InitTrigger()
TriggerCondition( 1, IsItem, 2971)	
TriggerAction( 1, AddNextFlag, 1236, 30, 1 )
RegCurTrigger( 12363 )
InitTrigger()
TriggerCondition( 1, IsItem, 2972)	
TriggerAction( 1, AddNextFlag, 1236, 40, 1 )
RegCurTrigger( 12364 )

---------------------------------------------
DefineMission( 6175, "Treasures from All Directions", 1237)
MisBeginTalk( "<t>4(2722,3137),,,,(3734,2661),,!")
MisBeginCondition( NoMission, 1237)
MisBeginCondition( NoRecord, 1237)
MisBeginCondition( LvCheck, "<", 90)
MisBeginAction( AddMission, 1237)
MisBeginAction( AddTrigger, 12371, TE_GETITEM, 2969, 1)
MisBeginAction( AddTrigger, 12372, TE_GETITEM, 2970, 1)
MisBeginAction( AddTrigger, 12373, TE_GETITEM, 2971, 1)
MisBeginAction( AddTrigger, 12374, TE_GETITEM, 2972, 1)
MisBeginAction( AddTrigger, 12375, TE_GETITEM, 2973, 1)
MisBeginAction( AddTrigger, 12376, TE_GETITEM, 2974, 1)
MisBeginAction( AddTrigger, 12377, TE_GETITEM, 2975, 1)
MisBeginAction( AddTrigger, 12378, TE_GETITEM, 2976, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(3734,2661),8(3302,2501).")
MisNeed( MIS_NEED_ITEM, 2969, 1, 10, 1)
MisNeed( MIS_NEED_ITEM, 2970, 1, 20, 1)
MisNeed( MIS_NEED_ITEM, 2971, 1, 30, 1)
MisNeed( MIS_NEED_ITEM, 2972, 1, 40, 1)
MisNeed( MIS_NEED_ITEM, 2973, 1, 50, 1)
MisNeed( MIS_NEED_ITEM, 2974, 1, 60, 1)
MisNeed( MIS_NEED_ITEM, 2975, 1, 70, 1)
MisNeed( MIS_NEED_ITEM, 2976, 1, 80, 1)

MisResultTalk( "<t>,!!")
MisHelpTalk( "<t>,8,.")
MisResultCondition( HasMission, 1237)
MisResultCondition( NoRecord, 1237)
MisResultCondition( HasItem, 2969, 1)
MisResultCondition( HasItem, 2970, 1)
MisResultCondition( HasItem, 2971, 1)
MisResultCondition( HasItem, 2972, 1)
MisResultCondition( HasItem, 2973, 1)
MisResultCondition( HasItem, 2974, 1)
MisResultCondition( HasItem, 2975, 1)
MisResultCondition( HasItem, 2976, 1)
MisResultAction( TakeItem, 2969, 1)
MisResultAction( TakeItem, 2970, 1)
MisResultAction( TakeItem, 2971, 1)
MisResultAction( TakeItem, 2972, 1)
MisResultAction( TakeItem, 2973, 1)
MisResultAction( TakeItem, 2974, 1)
MisResultAction( TakeItem, 2975, 1)
MisResultAction( TakeItem, 2976, 1)
MisResultAction( ClearMission, 1237)
MisResultAction( SetRecord, 1237)
MisResultAction( AddExpNextLv2 )
MisResultAction(ClearRecord, 1236)---------------can be repeated
MisResultAction(ClearRecord, 1237)---------------can be repeated
MisResultAction(ClearRecord, 1238)---------------can be repeated
MisResultAction(ClearRecord, 1239)---------------can be repeated
MisResultAction(ClearRecord, 1240)---------------can be repeated
MisResultAction(ClearRecord, 1241)---------------can be repeated
MisResultAction(ClearRecord, 1242)---------------can be repeated
MisResultAction(ClearRecord, 1243)---------------can be repeated
MisResultAction(ClearRecord, 1244)---------------can be repeated
MisResultAction(ClearRecord, 1245)---------------can be repeated
MisResultAction(ClearRecord, 1246)---------------can be repeated

InitTrigger()
TriggerCondition( 1, IsItem, 2969)	
TriggerAction( 1, AddNextFlag, 1237, 10, 1 )
RegCurTrigger( 12371 )
InitTrigger()
TriggerCondition( 1, IsItem, 2970)	
TriggerAction( 1, AddNextFlag, 1237, 20, 1 )
RegCurTrigger( 12372 )
InitTrigger()
TriggerCondition( 1, IsItem, 2971)	
TriggerAction( 1, AddNextFlag, 1237, 30, 1 )
RegCurTrigger( 12373 )
InitTrigger()
TriggerCondition( 1, IsItem, 2972)	
TriggerAction( 1, AddNextFlag, 1237, 40, 1 )
RegCurTrigger( 12374 )
InitTrigger()
TriggerCondition( 1, IsItem, 2973)	
TriggerAction( 1, AddNextFlag, 1237, 50, 1 )
RegCurTrigger( 12375 )
InitTrigger()
TriggerCondition( 1, IsItem, 2974)	
TriggerAction( 1, AddNextFlag, 1237, 60, 1 )
RegCurTrigger( 12376 )
InitTrigger()
TriggerCondition( 1, IsItem, 2975)	
TriggerAction( 1, AddNextFlag, 1237, 70, 1 )
RegCurTrigger( 12377 )
InitTrigger()
TriggerCondition( 1, IsItem, 2976)	
TriggerAction( 1, AddNextFlag, 1237,80, 1 )
RegCurTrigger( 12378 )

--------------------------------------------
DefineMission( 6176, "Lock of the Heart", 1238)
MisBeginTalk( "<t>,,(2256,2707).")
MisBeginCondition( NoMission, 1238)
MisBeginCondition( NoRecord, 1238)
MisBeginCondition( HasMission, 1237)
MisBeginAction( AddMission, 1238)
MisBeginAction( AddTrigger, 12381, TE_GETITEM, 2965, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP,"(2256,2707),(3734,2661).")
MisNeed( MIS_NEED_ITEM, 2965, 1, 10, 1)

MisResultTalk( "<t>Oh,baby!,.")
MisHelpTalk( "<t>")
MisResultCondition( HasMission, 1238)
MisResultCondition( NoRecord, 1238)
MisResultCondition( HasItem, 2965, 1)
MisResultAction( TakeItem, 2965, 1)
MisResultAction( ClearMission, 1238)
MisResultAction( SetRecord, 1238)
MisResultAction( GiveItem, 2974, 1, 4)
MisResultBagNeed(1)


InitTrigger()
TriggerCondition( 1, IsItem, 2965)	
TriggerAction( 1, AddNextFlag, 1238, 10, 1 )
RegCurTrigger( 12381 )

--------------------------------------------
DefineMission( 6177, "Lock of the Heart", 1239)
MisBeginTalk( "<t>,2,,,2,!!")
MisBeginCondition( HasMission, 1238)
MisBeginCondition( NoRecord, 1239)
MisBeginCondition( NoMission, 1239)
MisBeginAction( AddMission, 1239)
MisBeginAction( AddTrigger, 12391, TE_GETITEM, 3362, 2)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "2")
MisNeed( MIS_NEED_ITEM, 3362, 2, 10, 2)

MisResultTalk( "<t>,,.")
MisHelpTalk( "<t>.")
MisResultCondition( HasMission, 1239)
MisResultCondition( NoRecord, 1239)
MisResultCondition( HasItem, 3362, 2)
MisResultAction( TakeItem, 3362, 2)
MisResultAction( ClearMission, 1239)
MisResultAction( SetRecord, 1239)
MisResultAction( GiveItem, 2965, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 3362)	
TriggerAction( 1, AddNextFlag, 1239, 10, 2 )
RegCurTrigger( 12391 )

--------------------------------------------
DefineMission( 6178, "Aged Red Wine", 1240)
MisBeginTalk( "<t>,(1291,541)1,?")
MisBeginCondition( NoMission, 1240)
MisBeginCondition( NoRecord, 1240)
MisBeginCondition( HasMission, 1237)
MisBeginCondition( HasRecord, 1238)
MisBeginAction( AddMission, 1240)
MisBeginAction( AddTrigger, 12401, TE_GETITEM, 2977, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(1291,541)(3734,2661)")
MisNeed( MIS_NEED_ITEM, 2977, 1, 10, 1)

MisResultTalk( "<t>,.")
MisHelpTalk( "<t>")
MisResultCondition( HasMission, 1240)
MisResultCondition( NoRecord, 1240)
MisResultCondition( HasItem, 2977, 1)
MisResultAction( TakeItem, 2977, 1)
MisResultAction( ClearMission, 1240)
MisResultAction( SetRecord, 1240)
MisResultAction( GiveItem, 2976, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 2977)	
TriggerAction( 1, AddNextFlag, 1240, 10, 1 )
RegCurTrigger( 12401 )

---------------------------------------------
DefineMission( 6179, "Aged Red Wine", 1241)
MisBeginTalk( "<t>,?!!<n><t>3,3,3,70.")
MisBeginCondition( NoMission, 1241)
MisBeginCondition( NoRecord, 1241)
MisBeginCondition( HasMission, 1240)
MisBeginAction( AddMission, 1241)

MisBeginAction( AddTrigger, 12411, TE_GETITEM, 0172, 3)
MisBeginAction( AddTrigger, 12412, TE_GETITEM, 0173, 3)
MisBeginAction( AddTrigger, 12413, TE_GETITEM, 0174, 3)
MisBeginAction( AddTrigger, 12414, TE_GETITEM, 3916, 70)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, ",,")

MisNeed( MIS_NEED_ITEM, 0172, 3, 5, 3)
MisNeed( MIS_NEED_ITEM, 0173, 3, 10, 3)
MisNeed( MIS_NEED_ITEM, 0174, 3, 15, 3)
MisNeed( MIS_NEED_ITEM, 3916, 70, 20, 70)

MisResultTalk( "<t>,.")
MisHelpTalk( "<t>.")
MisResultCondition( HasMission, 1241)
MisResultCondition( NoRecord, 1241)
MisResultCondition( HasItem, 3916, 70)
MisResultCondition( HasItem, 172, 3)
MisResultCondition( HasItem, 173, 3)
MisResultCondition( HasItem, 174, 3)
MisResultAction( TakeItem, 3916, 70)
MisResultAction( TakeItem, 172, 3)
MisResultAction( TakeItem, 173, 3)
MisResultAction( TakeItem, 174, 3)
MisResultAction( ClearMission, 1241)
MisResultAction( SetRecord, 1241)




InitTrigger()
TriggerCondition( 1, IsItem, 0172)	
TriggerAction( 1, AddNextFlag, 1241, 5, 3 )
RegCurTrigger( 12411 )
InitTrigger()
TriggerCondition( 1, IsItem, 0173)	
TriggerAction( 1, AddNextFlag, 1241, 10, 3)
RegCurTrigger( 12412 )
InitTrigger()
TriggerCondition( 1, IsItem, 0174)	
TriggerAction( 1, AddNextFlag, 1241, 15, 3 )
RegCurTrigger( 12413 )
InitTrigger()
TriggerCondition( 1, IsItem, 3916)	
TriggerAction( 1, AddNextFlag, 1241, 20, 70 )
RegCurTrigger( 12414 )


--------------------------------------------
DefineMission( 6180, "Aged Red Wine", 1242)
MisBeginTalk( "<t>,,,,!")
MisBeginCondition( NoMission, 1242)
MisBeginCondition( NoRecord, 1242)
MisBeginCondition( HasRecord, 1241)
MisBeginAction( AddMission, 1242)
MisBeginAction( AddChaHJ )--------1440
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
MisBeginBagNeed(1)

MisNeed( MIS_NEED_DESP, ",.")

MisResultTalk( "<t>,.")
MisHelpTalk( "<t>.")
MisResultCondition( CheckHJ )-----11,0
MisResultCondition( NoRecord, 1242)
MisResultCondition( HasMission, 1242)
MisResultAction( ClearMission, 1242)
MisResultAction( SetRecord, 1242)
MisResultAction( TakeItem, 2967, 1)
MisResultAction( GiveItem, 2977, 1, 4)
MisResultBagNeed(1)

-----------------------------------------------
DefineMission( 6181, "Retrieve the Rose Petals", 1243)
MisBeginTalk( "<t>,(1280,478),?")
MisBeginCondition( NoMission, 1243)
MisBeginCondition( NoRecord, 1243)
MisBeginCondition( HasMission, 1237)
MisBeginCondition( HasRecord, 1238)
MisBeginCondition( HasRecord, 1240)
MisBeginAction( AddMission, 1243)
MisBeginAction( AddTrigger, 12431, TE_GETITEM, 2968, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(1280,478)")
MisNeed( MIS_NEED_ITEM, 2968, 1, 10, 1)

MisResultTalk( "<t>!!!,!")
MisHelpTalk( "<t>!")
MisResultCondition( HasMission, 1243)
MisResultCondition( NoRecord, 1243)
MisResultCondition( HasItem, 2968, 1)
MisResultAction( ClearMission, 1243)
MisResultAction( SetRecord, 1243)
MisResultAction( TakeItem, 2968, 1)
MisResultAction( GiveItem, 2973, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 2968)	
TriggerAction( 1, AddNextFlag, 1243, 10, 1 )
RegCurTrigger( 12431 )

-----------------------------------------------
DefineMission( 6182, "Retrieve the Rose Petals", 1244)
MisBeginTalk( "<t>,,.3,3,3,..")
MisBeginCondition( NoMission, 1244)
MisBeginCondition( NoRecord, 1244)
MisBeginCondition( HasMission, 1243)
MisBeginAction( AddMission, 1244)
MisBeginAction( AddTrigger, 12441, TE_GETITEM, 4259, 3)
MisBeginAction( AddTrigger, 12442, TE_GETITEM, 1774, 3)
MisBeginAction( AddTrigger, 12443, TE_GETITEM, 1632, 3)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "3,3,3")
MisNeed( MIS_NEED_ITEM, 4259, 3, 10, 3)
MisNeed( MIS_NEED_ITEM, 1774, 3, 20, 3)
MisNeed( MIS_NEED_ITEM, 1632, 3, 30, 3)

MisResultTalk( "<t>,.")
MisHelpTalk( "<t>,,,.")
MisResultCondition( HasMission, 1244)
MisResultCondition( NoRecord, 1244)
MisResultCondition( HasItem, 4259, 3)
MisResultCondition( HasItem, 1774, 3)
MisResultCondition( HasItem, 1632, 3)
MisResultAction( TakeItem, 4259, 3)
MisResultAction( TakeItem, 1774, 3)
MisResultAction( TakeItem, 1632, 3)
MisResultAction( SetRecord, 1244)
MisResultAction( ClearMission, 1244)
MisResultAction( GiveItem, 2968, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 4259)	
TriggerAction( 1, AddNextFlag, 1244, 10, 3 )
RegCurTrigger( 12441 )

InitTrigger()
TriggerCondition( 1, IsItem, 1774)	
TriggerAction( 1, AddNextFlag, 1244, 10, 1 )
RegCurTrigger( 12442 )

InitTrigger()
TriggerCondition( 1, IsItem, 1632)	
TriggerAction( 1, AddNextFlag, 1244, 10, 1 )
RegCurTrigger( 12443 )

--------------------------------------------
DefineMission( 6183, "Retrieve the Fine Candles", 1245)
MisBeginTalk( "<t>,,(897,3683)?")
MisBeginCondition( NoMission, 1245)
MisBeginCondition( NoRecord, 1245)
MisBeginCondition( HasMission, 1237)
MisBeginCondition( HasRecord, 1238)
MisBeginCondition( HasRecord, 1240)
MisBeginCondition( HasRecord, 1243)
MisBeginAction( AddMission, 1245)
MisBeginAction( AddTrigger, 12451, TE_GETITEM, 2966, 1)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(897,3683)(3734,2661).")
MisNeed( MIS_NEED_ITEM, 2966, 1, 10, 1)

MisResultTalk( "<t>,?~")
MisHelpTalk( "<t>?")
MisResultCondition( HasMission, 1245)
MisResultCondition( NoRecord, 1245)
MisResultCondition( HasItem, 2966, 1)
MisResultAction( TakeItem, 2966, 1)
MisResultAction( SetRecord, 1245)
MisResultAction( ClearMission, 1245)
MisResultAction( GiveItem, 2975, 1, 4)
MisResultBagNeed(1)

InitTrigger()
TriggerCondition( 1, IsItem, 2966)	
TriggerAction( 1, AddNextFlag, 1245, 10, 1 )
RegCurTrigger( 12451 )

----------------------------------------------
DefineMission( 6184, "Retrieve the Fine Candles", 1246)
MisBeginTalk( "<t>,2,2,,.")
MisBeginCondition( NoMission, 1246)
MisBeginCondition( NoRecord, 1246)
MisBeginCondition( HasMission, 1245)
MisBeginAction( AddMission, 1246)
MisBeginAction( AddTrigger, 12461, TE_GETITEM, 3367,2)
MisBeginAction( AddTrigger, 12462, TE_GETITEM, 3380,2)
MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

MisNeed( MIS_NEED_DESP, "(897,3683)2,2.<r(3734,2661)>.")
MisNeed( MIS_NEED_ITEM, 3367, 2, 10, 2)
MisNeed( MIS_NEED_ITEM, 3380, 2, 20, 2)

MisResultTalk( "<t>!~")
MisHelpTalk( "<t>..")
MisResultCondition( HasMission, 1246)
MisResultCondition( NoRecord, 1246)
MisResultCondition( HasItem, 3367, 2)
MisResultCondition( HasItem, 3380, 2)
MisResultAction( TakeItem, 3380, 2)
MisResultAction( TakeItem, 3367, 2)
MisResultAction( SetRecord, 1246)
MisResultAction( ClearMission, 1246)
MisResultAction( GiveItem, 2966, 1, 4)

MisResultBagNeed(1)

	InitTrigger()
TriggerCondition( 1, IsItem, 3367)	
TriggerAction( 1, AddNextFlag, 1246, 10, 2 )
RegCurTrigger( 12461 )

 InitTrigger()
TriggerCondition( 1, IsItem, 3380)	
TriggerAction( 1, AddNextFlag, 1246, 20, 2 )
RegCurTrigger( 12462 )

------------------------------------ 	NPC1216550
DefineMission( 6185, "Rescue Santa Claus", 1247)
MisBeginTalk( "<t>,?.,,.,.,..")
MisBeginCondition( NoMission, 1247)
MisBeginCondition( NoRecord, 1247)
MisBeginCondition( HasItem, 2878, 1)
MisBeginAction( TakeItem, 2878, 1)
MisBeginAction( AddMission, 1247)
MisBeginAction( CreatBBBB, 929)
MisCancelAction( ClearMission, 1247)

MisHelpTalk( "<t><r>,<r>(2111,557),<rNPC>.!")
MisNeed( MIS_NEED_DESP, "<r>,<r>(2111,557).,<rNPC>.<b,,>.")

MisResultCondition( AlwaysFailure )


----------------------------------------------------------------(2111,557)
DefineMission( 6186, "Rescue Santa Claus", 1247, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>..")
MisResultCondition( HasMission, 1247)
MisResultCondition( NoRecord, 1247)
MisResultAction( CheckBBBB)
MisResultAction( ClearMission, 1247)
MisResultAction( SetRecord, 1247)
MisResultAction( GiveItem, 2888, 1, 4)
MisResultAction( GiveItem, 2889, 1, 4)
MisResultAction( GiveItem, 3240, 1, 4)
MisResultBagNeed(3)

------------------------------	
DefineMission( 6187, "Great Turkey Hunt", 1248)
MisBeginTalk( "<t>.?,,,100.")
MisBeginCondition( NoMission, 1248)
MisBeginCondition( NoRecord, 1248)
MisBeginAction( AddMission, 1248)
MisBeginAction(AddTrigger, 12481, TE_GETITEM, 2879, 100 )
MisCancelAction( ClearMission, 1248)

MisNeed(MIS_NEED_ITEM, 2879, 100, 10, 100 )
MisHelpTalk( "<t>,")

MisResultTalk( "<t>,?")
MisResultCondition( HasMission, 1248)
MisResultCondition( NoRecord, 1248)
MisResultCondition( HasItem, 2879, 100)
MisResultAction( TakeItem, 2879, 100)
MisResultAction( GiveItem, 2882, 1, 4)
MisResultAction( CpHuojiNum )
MisResultAction( ClearMission, 1248)
MisResultAction( SetRecord, 1248)
MisResultAction( ClearRecord, 1248)


InitTrigger()
TriggerCondition( 1, IsItem, 2879)	
TriggerAction( 1, AddNextFlag, 1248, 10, 100 )
RegCurTrigger( 12481 )

------------------------------	
DefineMission( 6188, "Great Reindeer Hunt", 1249)
MisBeginTalk( "<t>.?,,,100.")
MisBeginCondition( NoMission, 1249)
MisBeginCondition( NoRecord, 1249)
MisBeginAction( AddMission, 1249)
MisBeginAction(AddTrigger, 12491, TE_GETITEM, 2881, 100 )
MisCancelAction( ClearMission, 1249)

MisNeed(MIS_NEED_ITEM, 2881, 100, 10, 100 )
MisHelpTalk( "<t>,")

MisResultTalk( "<t>,?")
MisResultCondition( HasMission, 1249)
MisResultCondition( NoRecord, 1249)
MisResultCondition( HasItem, 2881, 100)
MisResultAction( TakeItem, 2881, 100)
MisResultAction( GiveItem, 2882, 1, 4)
MisResultAction( CpMiluNum )
MisResultAction( ClearMission, 1249)
MisResultAction( SetRecord, 1249)
MisResultAction( ClearRecord, 1249)


InitTrigger()
TriggerCondition( 1, IsItem, 2881)	
TriggerAction( 1, AddNextFlag, 1249, 10, 100 )
RegCurTrigger( 12491 )

------------------------------	
DefineMission( 6189, "Great Snowman Hunt", 1250)
MisBeginTalk( "<t>.?,,,100.")
MisBeginCondition( NoMission, 1250)
MisBeginCondition( NoRecord, 1250)
MisBeginAction( AddMission, 1250)
MisBeginAction(AddTrigger, 12501, TE_GETITEM, 2880, 100 )
MisCancelAction( ClearMission, 1250)

MisNeed(MIS_NEED_ITEM, 2880, 100, 10, 100 )
MisHelpTalk( "<t>,")

MisResultTalk( "<t>,?")
MisResultCondition( HasMission, 1250)
MisResultCondition( NoRecord, 1250)
MisResultCondition( HasItem, 2880, 100)
MisResultAction( TakeItem, 2880, 100)
MisResultAction( GiveItem, 2882, 1, 4)
MisResultAction( CpXuerenNum )
MisResultAction( ClearMission, 1250)
MisResultAction( SetRecord, 1250)
MisResultAction( ClearRecord, 1250)


InitTrigger()
TriggerCondition( 1, IsItem, 2880)	
TriggerAction( 1, AddNextFlag, 1250, 10, 100 )
RegCurTrigger( 12501 )


----------------------------------------------------------()--------------
DefineMission( 6190, "()", 1251)
MisBeginTalk( "<t>,,.")
MisBeginCondition( NoMission, 1251)
MisBeginCondition( NoRecord, 1251)
MisBeginAction( AddMission, 1251)
MisCancelAction( ClearMission, 1251)

MisNeed( MIS_NEED_DESP, "(220,41).")
MisHelpTalk( "<t>,.")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()--------------(220,41)
DefineMission( 6191, "()", 1251, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>,!,,.")
MisResultCondition( HasMission, 1251)
MisResultCondition( NoRecord, 1251)
MisResultAction( SetRecord, 1251)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1251)
MisResultBagNeed(1)


--------------------------------------------------------()-----------------
DefineMission( 6192, "()", 1252)
MisBeginTalk( "<t>,.,.")
MisBeginCondition( NoMission, 1252)
MisBeginCondition( NoRecord, 1252)
MisBeginAction( AddMission, 1252)
MisCancelAction( ClearMission, 1252)

MisNeed( MIS_NEED_DESP, "(144,252).")
MisHelpTalk( "<t>?")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()--------------(144,252)
DefineMission( 6193, "()", 1252, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>!???")
MisResultCondition( HasMission, 1252)
MisResultCondition( NoRecord, 1252)
MisResultAction( SetRecord, 1252)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1252)
MisResultBagNeed(1)

--------------------------------------------------------()--------------
DefineMission( 6194, "()", 1253)
MisBeginTalk( "<t>.,,.")
MisBeginCondition( NoMission, 1253)
MisBeginCondition( NoRecord, 1253)
MisBeginAction( AddMission, 1253)
MisCancelAction( ClearMission, 1253)

MisNeed( MIS_NEED_DESP, "(84,37).")
MisHelpTalk( "<t>~......")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()--------------(84,37)
DefineMission( 6195, "()", 1253, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>,?.")
MisResultCondition( HasMission, 1253)
MisResultCondition( NoRecord, 1253)
MisResultAction( SetRecord, 1253)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1253)
MisResultBagNeed(1)

--------------------------------------------------------()--------------
DefineMission( 6196, "()", 1254)
MisBeginTalk( "<t>?,,.")
MisBeginCondition( NoMission, 1254)
MisBeginCondition( NoRecord, 1254)
MisBeginAction( AddMission, 1254)
MisCancelAction( ClearMission, 1254)

MisNeed( MIS_NEED_DESP, "(197,251).")
MisHelpTalk( "<t>.")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()--------------(197,251)
DefineMission( 6197, "()", 1254, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>!,.")
MisResultCondition( HasMission, 1254)
MisResultCondition( NoRecord, 1254)
MisResultAction( SetRecord, 1254)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1254)
MisResultBagNeed(1)


--------------------------------------------------------()--------------
DefineMission( 6198, "()", 1255)
MisBeginTalk( "<t>.")
MisBeginCondition( NoMission, 1255)
MisBeginCondition( NoRecord, 1255)
MisBeginAction( AddMission, 1255)
MisCancelAction( ClearMission, 1255)

MisNeed( MIS_NEED_DESP, "(125,253).")
MisHelpTalk( "<t>!")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()--------------(125,253)
DefineMission( 6199, "()", 1255, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>!,,,.")
MisResultCondition( HasMission, 1255)
MisResultCondition( NoRecord, 1255)
MisResultAction( SetRecord, 1255)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1255)
MisResultBagNeed(1)


--------------------------------------------------------()--------------
DefineMission( 6200, "()", 1256)
MisBeginTalk( "<t>?!,,,.")
MisBeginCondition( NoMission, 1256)
MisBeginCondition( NoRecord, 1256)
MisBeginAction( AddMission, 1256)
MisCancelAction( ClearMission, 1256)

MisNeed( MIS_NEED_DESP, "(144,166).")
MisHelpTalk( "<t>merry christmas!")

MisResultCondition( AlwaysFailure )

-----------------------------------------------------------()-------------(144,166)
DefineMission( 6201, "()", 1256, COMPLETE_SHOW)
MisBeginCondition( AlwaysFailure )

MisResultTalk( "<t>merry christmas!?,.")
MisResultCondition( HasMission, 1256)
MisResultCondition( NoRecord, 1256)
MisResultAction( SetRecord, 1256)
MisResultAction( GiveItem, 2883, 2, 4)
MisResultAction( ClearMission, 1256)
MisResultBagNeed(1)

-----------------------------------------------------------------------
DefineMission( 6202, "", 1257)		------------

MisBeginTalk( "<t>.")
MisBeginCondition( XmasNotice, 1 )
MisBeginCondition( AlwaysFailure )
MisBeginAction( AddMission, 1257)
MisCancelAction( ClearMission, 1257)

MisResultCondition( AlwaysFailure )

DefineMission( 6203, "", 1258)		------------

MisBeginTalk( "<t>.")
MisBeginCondition( XmasNotice, 2 )
MisBeginCondition( AlwaysFailure )
MisBeginAction( AddMission, 1258)
MisCancelAction( ClearMission, 1258)

MisResultCondition( AlwaysFailure )

DefineMission( 6204, "", 1259)		------------

MisBeginTalk( "<t>.")
MisBeginCondition( XmasNotice, 3 )
MisBeginCondition( AlwaysFailure )
MisBeginAction( AddMission, 1259)
MisCancelAction( ClearMission, 1259)

MisResultCondition( AlwaysFailure )

DefineMission( 6205, "", 1260)		------------

MisBeginTalk( "<t>.")
MisBeginCondition( XmasNotice, 4 )
MisBeginCondition( AlwaysFailure )
MisBeginAction( AddMission, 1260)
MisCancelAction( ClearMission, 1260)

MisResultCondition( AlwaysFailure )

 ---------------------------------------------------------------------------------------------------- ---------------------------------------- 08 Year of the Rat Chinese New Year activities kokora ------------------------------------------------ -------------------------------------------------------------------------- 

 ---------------------------------------------------------------------------------------------------- silver city -------------------------------------------- [new guide ???] (2223,2785 )---------------------------- ------------------------------------------------------------------ 

         ------------------------------------------------------------------------------------ Andrew??gift pack rat (a Silver City, New guidance )------------------ 

 DefineMission (6205, "Andrew??gift pack rat", 1257) 

 MisBeginTalk ( "<t> invites rat Ying--yu, Delicacies out gold. <n> <t> Dear friend, pirates auspicious Year of the Rat King to congratulate you, all things to one's liking! Lovely few days ago a small mouse in my town??Li for 30--64 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! task silver go to the city after the new guidance ???(2223,2785) to receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 29) 
 MisBeginCondition (LvCheck, "<", 65) 
 MisBeginAction (AddMission, 1257) 
 MisBeginAction (AddTrigger, 12571, TE_GETITEM, 3116, 99) 

 MisCancelAction (ClearMission, 1257) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Institute of content, learning to share, in order to feel happy! Li Ang mice for each player only has prepared a red, your New Year of the Rat red packets to determine that you need to apply for it? Then we begin ! major available outside the main Wizard delicious fruit, the fruit is sweet and sour Ang Li's favorites, will be for the full 99 Li Ang the Year of the Rat hand sewing novice red, take action as soon as possible! ") 
 MisNeed (MIS_NEED_ITEM, 3116, 99, 10, 99) ------------------ fruit Wizard 


 MisHelpTalk ( "<t> the Wizard said to the various types of grass will grow onto the wizard delicious sweet fruit, 99 fruit Wizard witness is also your patience! Filling it!") 
 MisResultTalk ( "<t> this beautiful red Rat send you new you, open probability after the package was mini Heilonggang Oh!") 

 MisResultCondition (HasMission, 1257) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (HasItem, 3116, 99) 

 MisResultAction (TakeItem, 3116, 99) 

 MisResultAction (GiveItem, 5648, 1, 4) ------------------ New Year of the Rat red packets 
 MisResultAction (ClearMission, 1257) 
 MisResultAction (SetRecord, 1257) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 3116) 
 TriggerAction (1, AddNextFlag, 1257, 10, 99) 
 RegCurTrigger (12571) 

        ------------------------------------------------------------------------------------ Andrew??rat gift to ( Second, new guidance )------------------ Silver City 

 DefineMission (6206, "Andrew??gift to the rat", 1258) 

 MisBeginTalk ( "<t> pirates????Wang, Yu--feng wind tune--year--old rats. <n> <t> Dear friend, pirates Rat Hongyun Wang congratulate your head, money rolling in! Lovely few days ago a small mouse Ang Li asked me in the city for 65--84 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! task silver go to the city after the new guidance ???(2223,2785) that receive incentives.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 64) 
 MisBeginCondition (LvCheck, "<", 85) 
 MisBeginAction (AddMission, 1258) 
 MisBeginAction (AddTrigger, 12581, TE_GETITEM, 4495, 1) 

 MisCancelAction (ClearMission, 1258) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> good and evil, the heroes of the world to do the pirates in order to understand vertical and horizontal corners of the pride! Mice Ang Li in the Chinese New Year is approaching, ask your leader ? ???snake brigands and get him brigands belt! by this as you will be Li Ang, senior Rat elaborate red, the warriors! the hands of the sword against you, act now! ") 
 MisNeed (MIS_NEED_ITEM, 4495, 1, 10, 1) ------------------ brigands belt 


 MisHelpTalk ( "<t> brigands said to the head of the snake usually ? ? (Ascaron 1038,3039) around, starting immediately, my Warrior!") 
 MisResultTalk ( "<t> Oh! God, did not expect such a short time you have completed the task. The Senior Year of the Rat is red Ang Li reward you, open it there will be an unexpected surprise Oh!") 

 MisResultCondition (HasMission, 1258) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (HasItem, 4495, 1) 

 MisResultAction (TakeItem, 4495, 1) 

 MisResultAction (GiveItem, 5649, 1, 4) ------------------ Senior Year of the Rat red packets 
 MisResultAction (ClearMission, 1258) 
 MisResultAction (SetRecord, 1258) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 4495) 
 TriggerAction (1, AddNextFlag, 1258, 10, 1) 
 RegCurTrigger (12581) 

 --Qing Xiang??------------------------------------------------------------------------------------ rat Festival (c) New guidance ------------------ Silver City 

 DefineMission (6207, "Xiang Qing??rat Festival", 1259) 

 MisBeginTalk ( "<t> Mao Feng??Tim pirates, rats to be wonderful point pen spring. <n> <t> Dear friend, pirates King Rat????congratulate you, best for the future! Lovely few days ago a small rat Li??in my town for 85--100 players sending red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! task silver go to the city after the new guidance ???(2223,2785) receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 84) 
 MisBeginCondition (LvCheck, "<", 101) 
 MisBeginAction (AddMission, 1259) 
 MisBeginAction (AddTrigger, 12591, TE_GETITEM, 0179, 10) 

 MisCancelAction (ClearMission, 1259) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Li Ang recent mice learned in magic, a summer magic magic, the main city of the production of three magic energy shield, blocking the invasion of evil. 10 is still a lack of mechanical perpetual motion machine, will be Super Rat red packets received favors of you must not refuse this small request, right? ") 
 MisNeed (MIS_NEED_ITEM, 0179, 10, 10, 10) ------------------ mechanical perpetual motion machine 


 MisHelpTalk ( "<t> evil giant mechanical body forces, a powerful blue energy ama phantom aircraft Kawasaki, Kawasaki dual Jianshi Blue phantom, phantom???risk--takers, Amy Pharmacist phantom, phantom Amy seal was found in possession of this material, in order to the three main city of tranquility and peace, these fixtures quickly and brought to 10 armed Li Ang perpetual motion machine.") 
 MisResultTalk ( "<t> as a symbol of justice and peace, small mice Ang Li told that I must send to the Super Rat red packets you can open a bag of the New Year pretty handsome package?! Waiting for what? Friends quickly open and see! ") 

 MisResultCondition (HasMission, 1259) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (HasItem, 0179, 10) 

 MisResultAction (TakeItem, 0179, 10) 

 MisResultAction (GiveItem, 5650, 1, 4) ------------------ Super Rat red packets 
 MisResultAction (ClearMission, 1259) 
 MisResultAction (SetRecord, 1259) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 0179) 
 TriggerAction (1, AddNextFlag, 1259, 10, 10) 
 RegCurTrigger (12591) 


 ---------------------------------------------------------------------------------------------------- ice??-------------------------------------------- [new guide ? Angel Lu Ya] (1315,507) -------------------- repeat -------------------------------------------------------------------------- 
       
        ------------------------------------------------------------------------------------ Andrew??gift pack rat (a New guidance ice??)------------------ 

 DefineMission (6208, "Andrew??gift pack rat", 1260) 

 MisBeginTalk ( "<t> invites rat Ying--yu, Delicacies out gold. <n> <t> Dear friend, pirates auspicious Year of the Rat King to congratulate you, all things to one's liking! Lovely few days ago a small mouse in my town??Li for 30--64 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to the ice after the completion of the task to guide novice??Angel ? Lu Ya (1315,507) to receive the award .") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 29) 
 MisBeginCondition (LvCheck, "<", 65) 
 MisBeginAction (AddMission, 1260) 
 MisBeginAction (AddTrigger, 12601, TE_GETITEM, 3116, 99) 

 MisCancelAction (ClearMission, 1260) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Institute of content, learning to share, in order to feel happy! Li Ang mice for each player only has prepared a red, your New Year of the Rat red packets to determine that you need to apply for it? Then we begin ! major available outside the main Wizard delicious fruit, the fruit is sweet and sour Ang Li's favorites, will be for the full 99 Li Ang the Year of the Rat hand sewing novice red, take action as soon as possible! ") 
 MisNeed (MIS_NEED_ITEM, 3116, 99, 10, 99) ------------------ fruit Wizard 


 MisHelpTalk ( "<t> the Wizard said to the various types of grass will grow onto the wizard delicious sweet fruit, 99 fruit Wizard witness is also your patience! Filling it!") 
 MisResultTalk ( "<t> this beautiful red Rat send you new you, open probability after the package was mini Heilonggang Oh!") 
 MisResultCondition (HasMission, 1260) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 

 MisResultCondition (HasItem, 3116, 99) 

 MisResultAction (TakeItem, 3116, 99) 

 MisResultAction (GiveItem, 5648, 1, 4) ------------------ New Year of the Rat red packets 
 MisResultAction (ClearMission, 1260) 
 MisResultAction (SetRecord, 1260) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 3116) 
 TriggerAction (1, AddNextFlag, 1260, 10, 99) 
 RegCurTrigger (12601) 

        ------------------------------------------------------------------------------------ Andrew??rat gift to ( Second, new guidance??ice )------------------ 

 DefineMission (6209, "Andrew??gift to the rat", 1261) 

 MisBeginTalk ( "<t> pirates????Wang, Yu--feng wind tune--year--old rats. <n> <t> Dear friend, pirates Rat Hongyun Wang congratulate your head, money rolling in! Lovely few days ago a small mouse Ang Li asked me in the city for 65--84 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to the ice after the completion of the task to guide novice??Angel ? Lu Ya (1315,507 ) to receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 64) 
 MisBeginCondition (LvCheck, "<", 85) 
 MisBeginAction (AddMission, 1261) 
 MisBeginAction (AddTrigger, 12611, TE_GETITEM, 4495, 1) 

 MisCancelAction (ClearMission, 1261) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> good and evil, the heroes of the world to do the pirates in order to understand vertical and horizontal corners of the pride! Mice Ang Li in the Chinese New Year is approaching, ask your leader ? ???snake brigands and get him brigands belt! by this as you will be Li Ang, senior Rat elaborate red, the warriors! the hands of the sword against you, act now! ") 
 MisNeed (MIS_NEED_ITEM, 4495, 1, 10, 1) ------------------ brigands belt 


 MisHelpTalk ( "<t> brigands said to the head of the snake usually ? ? (Ascaron 1038,3039) around, starting immediately, my Warrior!") 
 MisResultTalk ( "<t> Oh! God, did not expect such a short time you have completed the task. The Senior Year of the Rat is red Ang Li reward you, open it there will be an unexpected surprise Oh!") 

 MisResultCondition (HasMission, 1261) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 

 MisResultCondition (HasItem, 4495, 1) 

 MisResultAction (TakeItem, 4495, 1) 

 MisResultAction (GiveItem, 5649, 1, 4) ------------------ Senior Year of the Rat red packets 
 MisResultAction (ClearMission, 1261) 
 MisResultAction (SetRecord, 1261) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 4495) 
 TriggerAction (1, AddNextFlag, 1261, 10, 1) 
 RegCurTrigger (12611) 

 --Qing Xiang??------------------------------------------------------------------------------------ rat Festival (c) ------------------ Ice??new guidance 

 DefineMission (6210, "Xiang Qing??rat Festival", 1262) 

 MisBeginTalk ( "<t> Mao Feng??Tim pirates, rats to be wonderful point pen spring. <n> <t> Dear friend, pirates King Rat????congratulate you, best for the future! Lovely few days ago a small rat Li??in my town for 85--100 players sending red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to the ice after the completion of the task to guide novice??Angel ? Lu Ya (1315 , 507) to receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (LvCheck, ">", 84) 
 MisBeginCondition (LvCheck, "<", 101) 
 MisBeginAction (AddMission, 1262) 
 MisBeginAction (AddTrigger, 12621, TE_GETITEM, 0179, 10) 

 MisCancelAction (ClearMission, 1262) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Li Ang recent mice learned in magic, a summer magic magic, the main city of the production of three magic energy shield, blocking the invasion of evil. 10 is still a lack of mechanical perpetual motion machine, will be Super Rat red packets received favors of you must not refuse this small request, right? ") 
 MisNeed (MIS_NEED_ITEM, 0179, 10, 10, 10) ------------------ mechanical perpetual motion machine 


 MisHelpTalk ( "<t> evil giant mechanical body forces, a powerful blue energy ama phantom aircraft Kawasaki, Kawasaki dual Jianshi Blue phantom, phantom???risk--takers, Amy Pharmacist phantom, phantom Amy seal was found in possession of this material, in order to the three main city of tranquility and peace, these fixtures quickly and brought to 10 armed Li Ang perpetual motion machine.") 
 MisResultTalk ( "<t> as a symbol of justice and peace, small mice Ang Li told I must be the Year of the Rat red packets are given to the super you have a chance of bag out of the New Year pretty handsome package?! Waiting for what? Friend quickly open and see! ") 

 MisResultCondition (HasMission, 1262) 
 MisResultCondition (NoRecord, 1262) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 

 MisResultCondition (HasItem, 0179, 10) 

 MisResultAction (TakeItem, 0179, 10) 

 MisResultAction (GiveItem, 5650, 1, 4) ------------------ Super Rat red packets 
 MisResultAction (ClearMission, 1262) 
 MisResultAction (SetRecord, 1262) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 0179) 
 TriggerAction (1, AddNextFlag, 1262, 10, 10) 
 RegCurTrigger (12621) 

 ---------------------------------------------------------------------------------------------------- Lan Sha city -------------------------------------------- [new guide ????] (876,3572) ------------------------ repeat ---------------------------------------------------------------------- 
       
        ------------------------------------------------------------------------------------ Andrew??gift pack rat (a Lan Sha )------------------ new city guide 

 DefineMission (6211, "Andrew??gift pack rat", 1263) 

 MisBeginTalk ( "<t> invites rat Ying--yu, Delicacies out gold. <n> <t> Dear friend, pirates auspicious Year of the Rat King to congratulate you, all things to one's liking! Lovely few days ago a small mouse in my town??Li for 30--64 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to sand after the completion of tasks new city guide ? Lan???(876,3572) to receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (LvCheck, ">", 29) 
 MisBeginCondition (LvCheck, "<", 65) 
 MisBeginAction (AddMission, 1263) 
 MisBeginAction (AddTrigger, 12631, TE_GETITEM, 3116, 99) 

 MisCancelAction (ClearMission, 1263) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Institute of content, learning to share, in order to feel happy! Li Ang mice for each player only has prepared a red, your New Year of the Rat red packets to determine that you need to apply for it? Then we begin ! major available outside the main Wizard delicious fruit, the fruit is sweet and sour Ang Li's favorites, will be for the full 99 Li Ang the Year of the Rat hand sewing novice red, take action as soon as possible! ") 
 MisNeed (MIS_NEED_ITEM, 3116, 99, 10, 99) ------------------ fruit Wizard 


 MisHelpTalk ( "<t> the Wizard said to the various types of grass will grow onto the wizard delicious sweet fruit, 99 fruit Wizard witness is also your patience! Filling it!") 
 MisResultTalk ( "<t> this beautiful red Rat send you new you, open probability after the package was mini Heilonggang Oh!") 

 MisResultCondition (HasMission, 1263) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 

 MisResultCondition (HasItem, 3116, 99) 

 MisResultAction (TakeItem, 3116, 99) 

 MisResultAction (GiveItem, 5648, 1, 4) ------------------ New Year of the Rat red packets 
 MisResultAction (ClearMission, 1263) 
 MisResultAction (SetRecord, 1263) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 3116) 
 TriggerAction (1, AddNextFlag, 1263, 10, 99) 
 RegCurTrigger (12631) 

        ------------------------------------------------------------------------------------ Andrew??rat gift to ( Sha Lan b )------------------ new city guide 

 DefineMission (6212, "Andrew??gift to the rat", 1264) 

 MisBeginTalk ( "<t> pirates????Wang, Yu--feng wind tune--year--old rats. <n> <t> Dear friend, pirates Rat Hongyun Wang congratulate your head, money rolling in! Lovely few days ago a small mouse Ang Li asked me in the city for 65--84 players for the delivery of the red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to sand after the completion of tasks new city guide ? Lan???(876,3572) that receive incentives.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (LvCheck, ">", 64) 
 MisBeginCondition (LvCheck, "<", 85) 
 MisBeginAction (AddMission, 1264) 
 MisBeginAction (AddTrigger, 12641, TE_GETITEM, 4495, 1) 

 MisCancelAction (ClearMission, 1264) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> good and evil, the heroes of the world to do the pirates in order to understand vertical and horizontal corners of the pride! Mice Ang Li in the Chinese New Year is approaching, ask your leader ? ???snake brigands and get him brigands belt! by this as you will be Li Ang, senior Rat elaborate red, the warriors! the hands of the sword against you, act now! ") 
 MisNeed (MIS_NEED_ITEM, 4495, 1, 10, 1) ------------------ brigands belt 


 MisHelpTalk ( "<t> brigands said to the head of the snake usually ? ? (Ascaron 1038,3039) around, starting immediately, my Warrior!") 
 MisResultTalk ( "<t> Oh! God, did not expect such a short time you have completed the task. The Senior Year of the Rat is red Ang Li reward you, open it there will be an unexpected surprise Oh!") 

 MisResultCondition (HasMission, 1264) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 


 MisResultCondition (HasItem, 4495, 1) 

 MisResultAction (TakeItem, 4495, 1) 

 MisResultAction (GiveItem, 5649, 1, 4) ------------------ Senior Year of the Rat red packets 
 MisResultAction (ClearMission, 1264) 
 MisResultAction (SetRecord, 1264) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 4495) 
 TriggerAction (1, AddNextFlag, 1264, 10, 1) 
 RegCurTrigger (12641) 

 --Qing Xiang??------------------------------------------------------------------------------------ rat Festival (c) Lan Sha ------------------ New City guide 

 DefineMission (6213, "Xiang Qing??rat Festival", 1265) 

 MisBeginTalk ( "<t> Mao Feng??Tim pirates, rats to be wonderful point pen spring. <n> <t> Dear friend, pirates King Rat????congratulate you, best for the future! Lovely few days ago a small rat Li??in my town for 85--100 players sending red, one is only one opportunity to the best of luck not to be missed! quickly joined the ranks of sending it! Go to sand after the completion of tasks new city guide ? Lan???(876,3572 ) to receive the award.") 

 MisBeginCondition (NoMission, 1257) 
 MisBeginCondition (NoMission, 1258) 
 MisBeginCondition (NoMission, 1259) 
 MisBeginCondition (NoMission, 1260) 
 MisBeginCondition (NoMission, 1261) 
 MisBeginCondition (NoMission, 1262) 
 MisBeginCondition (NoMission, 1263) 
 MisBeginCondition (NoMission, 1264) 
 MisBeginCondition (NoMission, 1265) 
 -- MisBeginCondition (NoMission, 1265) 
 MisBeginCondition (NoRecord, 1265) 
 MisBeginCondition (NoRecord, 1263) 
 MisBeginCondition (NoRecord, 1264) 
 MisBeginCondition (NoRecord, 1257) 
 MisBeginCondition (NoRecord, 1258) 
 MisBeginCondition (NoRecord, 1259) 
 MisBeginCondition (NoRecord, 1260) 
 MisBeginCondition (NoRecord, 1261) 
 MisBeginCondition (NoRecord, 1262) 
 MisBeginCondition (LvCheck, ">", 84) 
 MisBeginCondition (LvCheck, "<", 101) 
 MisBeginAction (AddMission, 1265) 
 MisBeginAction (AddTrigger, 12651, TE_GETITEM, 0179, 10) 

 MisCancelAction (ClearMission, 1265) ------------------ may cancel this task 

 MisNeed (MIS_NEED_DESP, "<t> Li Ang recent mice learned in magic, a summer magic magic, the main city of the production of three magic energy shield, blocking the invasion of evil. 10 is still a lack of mechanical perpetual motion machine, will be Super Rat red packets received favors of you must not refuse this small request, right? ") 
 MisNeed (MIS_NEED_ITEM, 0179, 10, 10, 10) ------------------ mechanical perpetual motion machine 


 MisHelpTalk ( "<t> evil giant mechanical body forces, a powerful blue energy ama phantom aircraft Kawasaki, Kawasaki dual Jianshi Blue phantom, phantom???risk--takers, Amy Pharmacist phantom, phantom Amy seal was found in possession of this material, in order to the three main city of tranquility and peace, these fixtures quickly and brought to 10 armed Li Ang perpetual motion machine.") 
 MisResultTalk ( "<t> as a symbol of justice and peace, small mice Ang Li told that I must send to the Super Rat red packets you can open a bag of the New Year pretty handsome package?! Waiting for what? Friends quickly open and see! ") 

 MisResultCondition (HasMission, 1265) 
 MisResultCondition (NoRecord, 1265) 
 MisResultCondition (NoRecord, 1263) 
 MisResultCondition (NoRecord, 1264) 
 MisResultCondition (NoRecord, 1257) 
 MisResultCondition (NoRecord, 1258) 
 MisResultCondition (NoRecord, 1259) 
 MisResultCondition (NoRecord, 1260) 
 MisResultCondition (NoRecord, 1261) 
 MisResultCondition (NoRecord, 1262) 

 MisResultCondition (HasItem, 0179, 10) 

 MisResultAction (TakeItem, 0179, 10) 

 MisResultAction (GiveItem, 5650, 1, 4) ------------------ Super Rat red packets 
 MisResultAction (ClearMission, 1265) 
 MisResultAction (SetRecord, 1265) 
 MisResultBagNeed (1) 


 InitTrigger () 
 TriggerCondition (1, IsItem, 0179) 
 TriggerAction (1, AddNextFlag, 1265, 10, 10) 
 RegCurTrigger (12651) 

 ---------------------------------------------------------------------------------------------------- ------------------------ kokora (end )-------------------------------------------------------------------- ---------------------------------------------------------------- 

  ---Part I Mission---
 
 DefineMission (6214, "Revival I", 1266) 
 MisBeginTalk ( "<t>I am really touched by your bravery and courage. If you can succeed in rescuing my saint fighters, I am willing to give you the position of the pope as well as our talisman <rReviving Stone.> It's said that the stone is able to grant its holder the power of a deity. The first rune can be found from the Despair Knight - Saro, The guardian of Abaddon 5, who is the embodiment of Aries--Mu (59, 51). Go for it! Athena and I will pray for you! ") 
 MisBeginCondition (NoMission, 1266) 
 MisBeginCondition (NoRecord, 1266) 
 MisBeginCondition (Checksailexpmore, 9000) 
 MisBeginAction (AddMission, 1266) 
 MisBeginAction (AddTrigger, 12661, TE_GETITEM, 5753, 1) 
 MisBeginAction (AddTrigger, 12662, TE_GETITEM, 5754, 1) 
 MisBeginAction (AddTrigger, 12663, TE_GETITEM, 5755, 1) 
 MisBeginAction (AddTrigger, 12664, TE_GETITEM, 5756, 1) 
 MisBeginAction (AddTrigger, 12665, TE_GETITEM, 5757, 1) 
 MisBeginAction (AddTrigger, 12666, TE_GETITEM, 5758, 1) 
 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_DESP, "You have entered Abaddon 5 to Abaddon 11 to rescue the saint fighters and gained the  6 Sait  Fighter's Runes.") 
 MisNeed (MIS_NEED_ITEM, 5753, 1, 10, 1) 
 MisNeed (MIS_NEED_ITEM, 5754, 1, 15, 1) 
 MisNeed (MIS_NEED_ITEM, 5755, 1, 20, 1) 
 MisNeed (MIS_NEED_ITEM, 5756, 1, 25, 1) 
 MisNeed (MIS_NEED_ITEM, 5757, 1, 30, 1) 
 MisNeed (MIS_NEED_ITEM, 5758, 1, 35, 1) 
 
 MisHelpTalk ( "<t>The first rune can be found from the Despair Knight - Saro, The guardian of Abaddon 5, who is the embodiment of Aries--Mu (59, 51). Go for it! Athena and I will pray for you! ") 
 MisResultTalk ( "<t>You are really brave! Go ahead to the deeper part of Abaddon to rescue the other 6 saint fighters. I am counting on you.") 

 MisResultCondition (HasMission, 1266) 
 MisResultCondition (NoRecord, 1266) 
 MisResultCondition (HasItem, 5753, 1) 
 MisResultCondition (HasItem, 5754, 1) 
 MisResultCondition (HasItem, 5755, 1) 
 MisResultCondition (HasItem, 5756, 1) 
 MisResultCondition (HasItem, 5757, 1) 
 MisResultCondition (HasItem, 5758, 1) 
 
 MisResultAction (TakeItem, 5753, 1) 
 MisResultAction (TakeItem, 5754, 1) 
 MisResultAction (TakeItem, 5755, 1) 
 MisResultAction (TakeItem, 5756, 1) 
 MisResultAction (TakeItem, 5757, 1) 
 MisResultAction (TakeItem, 5758, 1) 
 
 
 MisResultAction (ClearMission, 1266) 
 MisResultAction (SetRecord, 1266) 
 

 InitTrigger () 
 TriggerCondition (1, IsItem, 5753) 
 TriggerAction (1, AddNextFlag, 1266, 10, 1) 
 RegCurTrigger (12661) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5754) 
 TriggerAction (1, AddNextFlag, 1266, 15, 1) 
 RegCurTrigger (12662) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5755) 
 TriggerAction (1, AddNextFlag, 1266, 20, 1) 
 RegCurTrigger (12663) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5756) 
 TriggerAction (1, AddNextFlag, 1266, 25, 1) 
 RegCurTrigger (12664) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5757) 
 TriggerAction (1, AddNextFlag, 1266, 30, 1) 
 RegCurTrigger (12665) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5758) 
 TriggerAction (1, AddNextFlag, 1266, 35, 1) 
 RegCurTrigger (12666) 
 
 ---Mu's Rune---
 
 DefineMission (6215, "Mu's Rune", 1267) 
 MisBeginTalk ( "<t>If only the Despair Knight - Saro can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1267) 
 MisBeginCondition (NoRecord, 1267) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginAction (AddMission, 1267) 
 MisBeginAction (AddTrigger, 12671, TE_KILL, 974, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 974, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Mudmonster - Karu, the guardian of Abaddon 6. He is the embodiment of Taurus--Aldebaran (178, 77). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Despair Knight - Saro, release the soul of Mu.") 
 MisResultCondition (HasFlag, 1267, 10) 
 MisResultAction (GiveItem, 5753,1,4 )---MU's Rune
 MisResultCondition (HasMission, 1267) 
 MisResultAction (ClearMission, 1267) 
 MisResultAction (SetRecord, 1267) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 974) 
 TriggerAction (1, AddNextFlag, 1267, 10, 1) 
 RegCurTrigger (12671) 

 ---Aldebaran's Rune---

 DefineMission (6216, "Aldebaran's Rune", 1268) 
 MisBeginTalk ( "<t>If only the Abyss Mudmonster - Karu can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1268) 
 MisBeginCondition (NoRecord, 1268) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginCondition (HasRecord, 1267) 
 MisBeginAction (AddMission, 1268) 
 MisBeginAction (AddTrigger, 12681, TE_KILL, 975, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 975, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Prisoner - Aruthur, the guardian of Abaddon 7. He is the embodiment of Gemini--Saga (205, 170). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Mudmonster - Karu, release the soul of Aldebaran.") 
 MisResultCondition (HasFlag, 1268, 10) 
 MisResultAction (GiveItem, 5754,1,4 )---ALDEBA's Rune 
 MisResultCondition (HasMission, 1268) 
 MisResultAction (ClearMission, 1268) 
 MisResultAction (SetRecord, 1268) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 975) 
 TriggerAction (1, AddNextFlag, 1268, 10, 1) 
 RegCurTrigger (12681) 
 
 ---Saga's Rune---

 DefineMission (6217, "Saga's Rune", 1269) 
 MisBeginTalk ( "<t>If only the Abyss Prisoner - Aruthur can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1269) 
 MisBeginCondition (NoRecord, 1269) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginCondition (HasRecord, 1268) 
 MisBeginAction (AddMission, 1269) 
 MisBeginAction (AddTrigger, 12691, TE_KILL, 976, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 976, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Demon - Sacriose, the guardian of Abaddon 8. He is the embodiment of Cancer--Death Mask (85, 198). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Prisoner - Aruthur, release the soul of Saga.") 
 MisResultCondition (HasFlag, 1269, 10) 
 MisResultAction (GiveItem, 5755,1,4 )---ALDEBA's Rune
 MisResultCondition (HasMission, 1269) 
 MisResultAction (ClearMission, 1269) 
 MisResultAction (SetRecord, 1269) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 976) 
 TriggerAction (1, AddNextFlag, 1269, 10, 1) 
 RegCurTrigger (12691) 
 
 ---Death Mask's Rune---

 DefineMission (6218, "Death Mask's Rune", 1270) 
 MisBeginTalk ( "<t>If only the Abyss Demon - Sacrios can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1270) 
 MisBeginCondition (NoRecord, 1270) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginCondition (HasRecord, 1269) 
 MisBeginAction (AddMission, 1270) 
 MisBeginAction (AddTrigger, 12701, TE_KILL, 977, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 977, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Phantom Baron, the guardian of Abaddon 10. He is the embodiment of Leo--Aeolia (80, 190). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Demon - Sacrios, release the soul of Death Mask.") 
 MisResultCondition (HasFlag, 1270, 10) 
 MisResultAction (GiveItem, 5756,1,4 )---DEATH's Rune
 MisResultCondition (HasMission, 1270) 
 MisResultAction (ClearMission, 1270) 
 MisResultAction (SetRecord, 1270) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 977) 
 TriggerAction (1, AddNextFlag, 1270, 10, 1) 
 RegCurTrigger (12701) 
 
 ---Aeolia's Rune---

 DefineMission (6219, "Aeolia's Rune", 1271) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Phantom Baron can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1271) 
 MisBeginCondition (NoRecord, 1271) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginCondition (HasRecord, 1270) 
 MisBeginAction (AddMission, 1271) 
 MisBeginAction (AddTrigger, 12711, TE_KILL, 979, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 979, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Demon Flame, the guardian of Abaddon 11. He is the embodiment of Virgo--Shaka (79, 78). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Phantom Baron, release the soul of Aeolia.") 
 MisResultCondition (HasFlag, 1271, 10) 
 MisResultAction (GiveItem, 5757,1,4 )---AIOLIA's Rune
 MisResultCondition (HasMission, 1271) 
 MisResultAction (ClearMission, 1271) 
 MisResultAction (SetRecord, 1271) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 979) 
 TriggerAction (1, AddNextFlag, 1271, 10, 1) 
 RegCurTrigger (12711) 
 
 ---Shaka's Rune---

 DefineMission (6220, "Shaka's Rune", 1272) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Demon Flame can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1272) 
 MisBeginCondition (NoRecord, 1272) 
 MisBeginCondition (HasMission, 1266) 
 MisBeginCondition (HasRecord, 1271) 
 MisBeginAction (AddMission, 1272) 
 MisBeginAction (AddTrigger, 12721, TE_KILL, 980, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 980, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. Hurry to rescue the Holy Father, Shion (1749, 902) with the rune.") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Demon Flame, release the soul of Shaka.") 
 MisResultCondition (HasFlag, 1272, 10) 
 MisResultAction (GiveItem, 5758,1,4 )---SHAKA's Rune 
 MisResultCondition (HasMission, 1272) 
 MisResultAction (ClearMission, 1272) 
 MisResultAction (SetRecord, 1272) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 980) 
 TriggerAction (1, AddNextFlag, 1272, 10, 1) 
 RegCurTrigger (12721) 
 
 ---Dohko's Rune---

 DefineMission (6221, "Dohko's Rune", 1273) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Evil Beast can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1273) 
 MisBeginCondition (NoRecord, 1273) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1272) 
 MisBeginAction (AddMission, 1273) 
 MisBeginAction (AddTrigger, 12731, TE_KILL, 981, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 981, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Tyran, the guardian of Abaddon 13. He is the embodiment of Scorpion--Milo (326, 76). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Evil Beast, release the soul of Dohko.") 
 MisResultCondition (HasFlag, 1273, 10) 
 MisResultAction (GiveItem, 5759,1,4 )---DOHKO's Rune
 MisResultCondition (HasMission, 1273) 
 MisResultAction (ClearMission, 1273) 
 MisResultAction (SetRecord, 1273) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 981) 
 TriggerAction (1, AddNextFlag, 1273, 10, 1) 
 RegCurTrigger (12731) 
 
 ---Milo's Rune---

 DefineMission (6222, "Milo's Rune", 1274) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Tyran can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1274) 
 MisBeginCondition (NoRecord, 1274) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1273) 
 MisBeginAction (AddMission, 1274) 
 MisBeginAction (AddTrigger, 12741, TE_KILL, 982, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 982, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Phoenix, the guardian of Abaddon 14. He is the embodiment of Sagittarius--Aiolos (320, 195). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Tyran, release the soul of Milo.") 
 MisResultCondition (HasFlag, 1274, 10) 
 MisResultAction (GiveItem, 5760,1,4 )---MILO's Rune 
 MisResultCondition (HasMission, 1274) 
 MisResultAction (ClearMission, 1274) 
 MisResultAction (SetRecord, 1274) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 982) 
 TriggerAction (1, AddNextFlag, 1274, 10, 1) 
 RegCurTrigger (12741) 
 
 ---Aiolos's Rune---
 
 DefineMission (6223, "Aiolos's Rune", 1275) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Phoenix can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1275) 
 MisBeginCondition (NoRecord, 1275) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1274) 
 MisBeginAction (AddMission, 1275) 
 MisBeginAction (AddTrigger, 12751, TE_KILL, 983, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 983, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Despair, the guardian of Abaddon 15. He is the embodiment of Capricorn--Shura (318, 317). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Phoenix, release the soul of Aiolos.") 
 MisResultCondition (HasFlag, 1275, 10) 
 MisResultAction (GiveItem, 5761,1,4 )---AIOLOS's Rune 
 MisResultCondition (HasMission, 1275) 
 MisResultAction (ClearMission, 1275) 
 MisResultAction (SetRecord, 1275) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 983) 
 TriggerAction (1, AddNextFlag, 1275, 10, 1) 
 RegCurTrigger (12751) 
 
 ---Shura's Rune---
 
 DefineMission (6224, "Shura's Rune", 1276) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Despair can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1276) 
 MisBeginCondition (NoRecord, 1276) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1275) 
 MisBeginAction (AddMission, 1276) 
 MisBeginAction (AddTrigger, 12761, TE_KILL, 984, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 984, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Drakan, the guardian of Abaddon 16. He is the embodiment of Aquarius--Camus (198, 310). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Despair, release the soul of Shura.") 
 MisResultCondition (HasFlag, 1276, 10) 
 MisResultAction (GiveItem, 5762,1,4 )---SHURA's Rune 
 MisResultCondition (HasMission, 1276) 
 MisResultAction (ClearMission, 1276) 
 MisResultAction (SetRecord, 1276) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 984) 
 TriggerAction (1, AddNextFlag, 1276, 10, 1) 
 RegCurTrigger (12761) 
 
 ---Camus's Rune---
 
 DefineMission (6225, "Camus's Rune", 1277) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Drakan can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1277) 
 MisBeginCondition (NoRecord, 1277) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1276) 
 MisBeginAction (AddMission, 1277) 
 MisBeginAction (AddTrigger, 12771, TE_KILL, 985, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 985, 1, 10, 1) 

 MisResultTalk( "<t>You are really a honorable warrior. This rune is now yours as the reward. The next rune can be found on the Abyss Lord - Tidal, the guardian of Abaddon 17. He is the embodiment of Pisces--Ahrodite (80, 315). Go ahead! I will pray for you! ") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Drakan, release the soul of Camus.") 
 MisResultCondition (HasFlag, 1277, 10) 
 MisResultAction (GiveItem, 5763,1,4 )---ACMUS's Rune 
 MisResultCondition (HasMission, 1277) 
 MisResultAction (ClearMission, 1277) 
 MisResultAction (SetRecord, 1277) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 985) 
 TriggerAction (1, AddNextFlag, 1277, 10, 1) 
 RegCurTrigger (12771) 
 
 ---Aphrodite's Rune---

 DefineMission (6226, "Aphrodite's Rune", 1278) 
 MisBeginTalk ( "<t>If only the Abyss Lord - Tidal can be defeated, then my soul will be released so that I can return to the Holy Land. I believe you are able to defeat him.") 
 MisBeginCondition (NoMission, 1278) 
 MisBeginCondition (NoRecord, 1278) 
 MisBeginCondition (HasMission, 1279) 
 MisBeginCondition (HasRecord, 1277) 
 MisBeginAction (AddMission, 1278) 
 MisBeginAction (AddTrigger, 12781, TE_KILL, 986, 1) 
 -- MisCancelAction (ClearMission, 1266) 

 MisNeed (MIS_NEED_KILL, 986, 1, 10, 1) 

 MisResultTalk ( "<t>You are really a honorable warrior. This is the last rune, now its yours as the reward for rescuing my 12 sait fighters. Hurry to rescue the Holy Father, Shion (1749, 902) with the rune.") 
 MisHelpTalk ( "<t>Defeat the Abyss Lord - Tidal, release the soul of Aphrodite.") 
 MisResultCondition (HasFlag, 1278, 10) 
 MisResultAction (GiveItem, 5764,1,4 )---APHRO's Rune
 MisResultCondition (HasMission, 1278) 
 MisResultAction (ClearMission, 1278) 
 MisResultAction (SetRecord, 1278) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 986) 
 TriggerAction (1, AddNextFlag, 1278, 10, 1) 
 RegCurTrigger (12781) 
 
 ---Part II Mission--

 DefineMission (6227, "Revival II", 1279) 
 MisBeginTalk ( "<t>I am really touched by your bravery and courage. If you can succeed in rescuing my saint fighters, I am willing to give the position of the pope as well as our talisman <rReviving Stone> to you. It's said that the stone is able to grant its holder the power of a deity. The seventh rune can be found on the Abyss Lord - Evil Beast, the guardian of Abaddon 12, who is the embodiment of Balance Rest--Dohko (210, 80). Go for it! Athena and I will pray for you! ") 
 MisBeginCondition (NoMission, 1279) 
 MisBeginCondition (NoRecord, 1279) 
 MisBeginCondition (HasRecord, 1266) 
 MisBeginAction (AddMission, 1279) 
 MisBeginAction (AddTrigger, 12791, TE_GETITEM, 5759, 1) 
 MisBeginAction (AddTrigger, 12792, TE_GETITEM, 5760, 1) 
 MisBeginAction (AddTrigger, 12793, TE_GETITEM, 5761, 1) 
 MisBeginAction (AddTrigger, 12794, TE_GETITEM, 5762, 1) 
 MisBeginAction (AddTrigger, 12795, TE_GETITEM, 5763, 1) 
 MisBeginAction (AddTrigger, 12796, TE_GETITEM, 5764, 1) 

 -- MisCancelAction (ClearMission, 1279) 

 MisNeed (MIS_NEED_DESP, "You have entered Abaddon 12 to Abaddon 17 to rescue the saint fighters and gained the last 6 Saint Fighter's Runes.") 
 MisNeed (MIS_NEED_ITEM, 5759, 1, 10, 1) 
 MisNeed (MIS_NEED_ITEM, 5760, 1, 15, 1) 
 MisNeed (MIS_NEED_ITEM, 5761, 1, 20, 1) 
 MisNeed (MIS_NEED_ITEM, 5762, 1, 25, 1) 
 MisNeed (MIS_NEED_ITEM, 5763, 1, 30, 1) 
 MisNeed (MIS_NEED_ITEM, 5764, 1, 35, 1) 
 
 MisHelpTalk ( "<t>The seventh rune can be found on the Abyss Lord - Evil Beast, the guardian of Abaddon 12, who is the embodiment of Balance Rest--Dohko (210, 80). Go for it! Athena and I will pray for you! ") 
 MisResultTalk ( "Hurrah!    I have finally found the real pirate king who is eligible to become the pirate pope--you. The talisman    <rReviving Stone> is now yours. You can also claim to raise your attributes great deal.") 

 MisResultCondition (HasMission, 1279) 
 MisResultCondition (NoRecord, 1279) 
 MisResultCondition (HasItem, 5759, 1) 
 MisResultCondition (HasItem, 5760, 1) 
 MisResultCondition (HasItem, 5761, 1) 
 MisResultCondition (HasItem, 5762, 1) 
 MisResultCondition (HasItem, 5763, 1) 
 MisResultCondition (HasItem, 5764, 1) 
 
 MisResultAction (TakeItem, 5759, 1) 
 MisResultAction (TakeItem, 5760, 1) 
 MisResultAction (TakeItem, 5761, 1) 
 MisResultAction (TakeItem, 5762, 1) 
 MisResultAction (TakeItem, 5763, 1) 
 MisResultAction (TakeItem, 5764, 1) 
 
 
 MisResultAction (GiveItem, 5765,1,4 )---Revive Stone 
 MisResultAction (ClearMission, 1279) 
 MisResultAction (SetRecord, 1279) 
 

 InitTrigger () 
 TriggerCondition (1, IsItem, 5759) 
 TriggerAction (1, AddNextFlag, 1279, 10, 1) 
 RegCurTrigger (12791) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5760) 
 TriggerAction (1, AddNextFlag, 1279, 15, 1) 
 RegCurTrigger (12792) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5761) 
 TriggerAction (1, AddNextFlag, 1279, 20, 1) 
 RegCurTrigger (12793) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5762) 
 TriggerAction (1, AddNextFlag, 1279, 25, 1) 
 RegCurTrigger (12794) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5763) 
 TriggerAction (1, AddNextFlag, 1279, 30, 1) 
 RegCurTrigger (12795) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5764) 
 TriggerAction (1, AddNextFlag, 1279, 35, 1) 
 RegCurTrigger (12796) 
 
 --Guidelines ------------------------------------ ------------------ prison warden 
 DefineMission (6230, "Way to be Disimprisoned", 1280) 
 
 MisBeginTalk ( "<t> No matter why you were sent here, you should obey the rules here since you are here. If you want to know how to leave this place, you can ask the Prison Guard at (281,218).") 
 MisBeginCondition (NoMission, 1280) 
 MisBeginCondition (NoRecord, 1280) 
 MisBeginAction (AddMission, 1280) 
 MisCancelAction (ClearMission, 1280) 

 MisNeed (MIS_NEED_DESP, "Ask the Prison Guard at (316,208) how to leave the jail.") 

 MisHelpTalk ( "Go now!") 
 MisResultCondition (AlwaysFailure) 

 --Prison guards -------------------- ------------------------------------ Guidelines 
 DefineMission (6231, "Way to be Disimprisoned", 1280, COMPLETE_SHOW) 

 MisBeginCondition (AlwaysFailure) 

 MisResultTalk ( "<t> Eager to leave? I don't think it's so boring here, haw--haw. If you realy want to leave, you can learn how to from me.") 
 MisResultCondition (HasMission, 1280) 
 MisResultCondition (NoRecord, 1280) 
 MisResultAction (ClearMission, 1280) 
 MisResultAction (SetRecord, 1280) 

 
 ------------------------------------ 1 ---------------- challenge level waste king treasure maze of Hope 
 DefineMission (6232, "the challenge of waste maze of Hope", 1281) 
 
 MisBeginTalk ( "<t> Hope you have from the waste back to the maze of things I want, then I will tell you how to do that.") 
 MisBeginCondition (NoMission, 1281) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5776, 1) 
 MisBeginCondition (NoItem, 5786, 1) 
 MisBeginCondition (NoItem, 5787, 1) 
 MisBeginCondition (NoItem, 5788, 1) 
 MisBeginCondition (NoItem, 5789, 1) 
 MisBeginCondition (NoItem, 5790, 1) 
 MisBeginAction (TakeItem, 5776, 1) 
 MisBeginAction (AddMission, 1281) 
 
 MisBeginAction (AddTrigger, 12811, TE_GETITEM, 3434, 15) 
 MisBeginAction (AddTrigger, 12812, TE_GETITEM, 3435, 10) 
 MisBeginAction (AddTrigger, 12813, TE_GETITEM, 3436, 15) 
 MisCancelAction (ClearMission, 1281) 
 MisNeed (MIS_NEED_ITEM, 3434, 15, 10, 15) 
 MisNeed (MIS_NEED_ITEM, 3435, 10, 20, 10) 
 MisNeed (MIS_NEED_ITEM, 3436, 15, 30, 15) 
 
 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1281) 
 MisResultCondition (HasItem, 3434, 15) 
 MisResultCondition (HasItem, 3435, 10) 
 MisResultCondition (HasItem, 3436, 15) 
 MisResultAction (SetRecord, 1281) -- Add record1281 
 MisResultAction (TakeItem, 3434, 15) 
 MisResultAction (TakeItem, 3435, 10) 
 MisResultAction (TakeItem, 3436, 15) 
 MisResultAction (ClearMission, 1281) 
 
 MisResultAction (GiveItem, 5786, 1, 4) -------------------- used to give players the????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 3434) 
 TriggerAction (1, AddNextFlag, 1281, 10, 15) 
 RegCurTrigger (12811) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3435) 
 TriggerAction (1, AddNextFlag, 1281, 20, 10) 
 RegCurTrigger (12812) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3436) 
 TriggerAction (1, AddNextFlag, 1281, 30, 15) 
 RegCurTrigger (12813) 

 





 ------------------------------------ 1 ---------------- challenge level swamp king treasure maze 
 DefineMission (6233, "swamp maze challenges", 1282) 
 
 MisBeginTalk ( "<t> you need to bring back from the swamp in the maze of things I want, then I will tell you how to do that.") 
 MisBeginCondition (NoMission, 1282) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5791, 1) --????the confidential letter B 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5791, 1) 
 MisBeginAction (AddMission, 1282) 
 
 MisBeginAction (AddTrigger, 12821, TE_GETITEM, 3445, 15) 
 MisBeginAction (AddTrigger, 12822, TE_GETITEM, 3443, 10) 
 MisBeginAction (AddTrigger, 12823, TE_GETITEM, 3444, 15) 
 MisCancelAction (ClearMission, 1282) 
 MisNeed (MIS_NEED_ITEM, 3445, 15, 10, 15) 
 MisNeed (MIS_NEED_ITEM, 3443, 10, 20, 10) 
 MisNeed (MIS_NEED_ITEM, 3444, 15, 30, 15) 
 
 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1282) 
 MisResultCondition (HasItem, 3445, 15) 
 MisResultCondition (HasItem, 3443, 10) 
 MisResultCondition (HasItem, 3444, 15) 
 MisResultAction (SetRecord, 1282) -- Add record1282 
 MisResultAction (TakeItem, 3445, 15) 
 MisResultAction (TakeItem, 3443, 10) 
 MisResultAction (TakeItem, 3444, 15) 
 MisResultAction (ClearMission, 1282) 
 
 
 MisResultAction (GiveItem, 5786, 1, 4) -------------------- waste of????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 3445) 
 TriggerAction (1, AddNextFlag, 1282, 10, 15) 
 RegCurTrigger (12821) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3443) 
 TriggerAction (1, AddNextFlag, 1282, 20, 10) 
 RegCurTrigger (12822) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3444) 
 TriggerAction (1, AddNextFlag, 1282, 30, 15) 
 RegCurTrigger (12823) 
 
 
 ------------------------------------ 1 ---------------- challenge level treasure king Cube Maze 
 DefineMission (6234, "Rubik's cube maze challenges", 1283) 
 
 MisBeginTalk ( "<t> you need to bring back from the cube maze of things I want, then I will tell you how to do that.") 
 MisBeginCondition (NoMission, 1283) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5792, 1) --????the confidential letter C 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5792, 1) 
 MisBeginAction (AddMission, 1283) 
 MisBeginAction (AddTrigger, 12831, TE_GETITEM, 3820, 30) 
 MisBeginAction (AddTrigger, 12832, TE_GETITEM, 3821, 30) 
 MisBeginAction (AddTrigger, 12833, TE_GETITEM, 3822, 30) 
 MisCancelAction (ClearMission, 1283) 
 MisNeed (MIS_NEED_ITEM, 3820, 30, 50, 30) 
 MisNeed (MIS_NEED_ITEM, 3821, 30, 60, 30) 
 MisNeed (MIS_NEED_ITEM, 3822, 30, 70, 30) 
 
 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1283) 
 MisResultCondition (HasItem, 3820, 30) 
 MisResultCondition (HasItem, 3821, 30) 
 MisResultCondition (HasItem, 3822, 30) 
 MisResultAction (TakeItem, 3820, 30) 
 MisResultAction (TakeItem, 3821, 30) 
 MisResultAction (TakeItem, 3822, 30) 
 MisResultAction (ClearMission, 1283) 
 MisResultAction (SetRecord, 1283) -- Add record1283 
 
 MisResultAction (GiveItem, 5786, 1, 4) -------------------- waste of????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 3820) 
 TriggerAction (1, AddNextFlag, 1283, 50, 30) 
 RegCurTrigger (12831) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3821) 
 TriggerAction (1, AddNextFlag, 1283, 60, 30) 
 RegCurTrigger (12832) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3822) 
 TriggerAction (1, AddNextFlag, 1283, 70, 30) 
 RegCurTrigger (12833) 

 ------------------------------------ 2 ---------------- key level king treasures 
 DefineMission (6235, "2 level king treasure key", 1284) 
 
 MisBeginTalk ( "<t> dear friend, Welcome to the mysterious and dangerous world. The existence of this secret world to discover you. As long as you help me to collect the items I need, I will tell you on the king's treasures secret ") 
 MisBeginCondition (NoMission, 1284) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5793, 1) -- 2 level key secret treasures 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5793, 1) -- removal of the key props secret treasure 2 
 MisBeginAction (AddMission, 1284) 
 MisBeginAction (AddTrigger, 12841, TE_GETITEM, 2588, 10) 
 MisBeginAction (AddTrigger, 12842, TE_GETITEM, 0855, 99) 
 MisCancelAction (ClearMission, 1284) 
 MisNeed (MIS_NEED_ITEM, 2588, 10, 10, 10) 
 MisNeed (MIS_NEED_ITEM, 0855, 99, 20, 99) 

 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1284) 
 MisResultCondition (HasItem, 2588, 10) 
 MisResultCondition (HasItem, 0855, 99) 
 MisResultAction (TakeItem, 2588, 10) 
 MisResultAction (TakeItem, 0855, 99) 
 MisResultAction (ClearMission, 1284) 
 MisResultAction (SetRecord, 1284) -- Add record1283 
 
 MisResultAction (GiveItem, 5787, 1, 4) -------------------- old????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 2588) 
 TriggerAction (1, AddNextFlag, 1284, 10, 10) 
 RegCurTrigger (12841) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 0855) 
 TriggerAction (1, AddNextFlag, 1284, 20, 99) 
 RegCurTrigger (12842) 
 



 ------------------------------------ 3 ---------------- key level king treasures 
 DefineMission (6236, "3 level king treasure key", 1285) 
 
 MisBeginTalk ( "<t> dear friend, Welcome to the mysterious and dangerous world. The existence of this secret world to discover you. This in addition to help me collect the items I need to also ask you to have 1000 points <r???> and 50 points <r???>, as long as these conditions are met I told you a secret treasure on the king ") 
 MisBeginCondition (NoMission, 1285) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5794, 1) -- 3 level key secret treasures 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5794, 1) -- removed the props 3 key secret treasures 
 MisBeginAction (AddMission, 1285) 
 MisBeginAction (AddTrigger, 12851, TE_GETITEM, 4511, 30) 
 MisBeginAction (AddTrigger, 12852, TE_GETITEM, 0855, 99) 
 MisCancelAction (ClearMission, 1285) 
 MisNeed (MIS_NEED_ITEM, 4511, 30, 10, 30) 
 MisNeed (MIS_NEED_ITEM, 0855, 99, 20, 99) 

 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Do not forget <r???> need 1000 points and 50 points <r???>, even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1285) 
 MisResultCondition (HasItem, 4511, 30) 
 MisResultCondition (HasItem, 0855, 99) 
 MisResultCondition (HasCredit, 1000) -- the need for the reputation of 1000 
 MisResultCondition (HasHonorPoint, 50) -- need to honor 50 
 MisResultAction (TakeItem, 4511, 30) 
 MisResultAction (TakeItem, 0855, 99) 
 MisResultAction (ClearMission, 1285) 
 MisResultAction (SetRecord, 1285) -- Add record1283 
 
 MisResultAction (GiveItem, 5788, 1, 4) -------------------- new????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 4511) 
 TriggerAction (1, AddNextFlag, 1285, 10, 30) 
 RegCurTrigger (12851) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 0855) 
 TriggerAction (1, AddNextFlag, 1285, 20, 99) 
 RegCurTrigger (12852) 


 ------------------------------------ 4 ---------------- key level king treasures 
 DefineMission (6237, "4 level king treasure key", 1286) 
 
 MisBeginTalk ( "<t> dear friend, Welcome to the mysterious and dangerous world. The existence of this secret world to discover you. As long as you help me to collect the items I need, I will tell you on the king's treasures secret ") 
 MisBeginCondition (NoMission, 1286) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5795, 1) -- 4 level key secret treasures 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5795, 1) -- removal of the key props secret treasures 4 
 MisBeginAction (AddMission, 1286) 
 MisBeginAction (AddTrigger, 12861, TE_GETITEM, 0266, 1) 
 MisBeginAction (AddTrigger, 12862, TE_GETITEM, 2589, 1) 
 MisBeginAction (AddTrigger, 12863, TE_GETITEM, 3000, 1) 
 MisCancelAction (ClearMission, 1286) 
 MisNeed (MIS_NEED_ITEM, 0266, 1, 10, 1) 
 MisNeed (MIS_NEED_ITEM, 2589, 1, 20, 1) 
 MisNeed (MIS_NEED_ITEM, 3000, 1, 30, 1) 
 
 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1286) 
 MisResultCondition (HasItem, 0266, 1) 
 MisResultCondition (HasItem, 2589, 1) 
 MisResultCondition (HasItem, 3000, 1) 
 MisResultAction (TakeItem, 0266, 1) 
 MisResultAction (TakeItem, 2589, 1) 
 MisResultAction (TakeItem, 3000, 1) 
 MisResultAction (ClearMission, 1286) 
 MisResultAction (SetRecord, 1286) -- Add record1286 
 
 MisResultAction (GiveItem, 5789, 1, 4) ------------------ exquisite????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 0266) 
 TriggerAction (1, AddNextFlag, 1286, 10, 1) 
 RegCurTrigger (12861) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 2589) 
 TriggerAction (1, AddNextFlag, 1286, 20, 1) 
 RegCurTrigger (12862) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3000) 
 TriggerAction (1, AddNextFlag, 1286, 40, 1) 
 RegCurTrigger (12863) 

 ------------------------------------ 5 ---------------- key level treasure king 
 DefineMission (6238, "5 king treasure level key", 1287) 
 
 MisBeginTalk ( "<t> dear friend, Welcome to the mysterious and dangerous world. The existence of this secret world to discover you. As long as you help me to collect the items I need, I will tell you on the king's treasures secret ") 
 MisBeginCondition (NoMission, 1287) 
 MisBeginCondition (NoRecord, 1281) 
 MisBeginCondition (NoRecord, 1282) 
 MisBeginCondition (NoRecord, 1283) 
 MisBeginCondition (NoRecord, 1284) 
 MisBeginCondition (NoRecord, 1285) 
 MisBeginCondition (NoRecord, 1286) 
 MisBeginCondition (NoRecord, 1287) 
 MisBeginCondition (HasItem, 5796, 1) -- 5 level key secret treasures 
 MisBeginCondition (NoItem, 5786, 1) -- Waste of????
 MisBeginCondition (NoItem, 5787, 1) -- old????
 MisBeginCondition (NoItem, 5788, 1) -- new????
 MisBeginCondition (NoItem, 5789, 1) -- exquisite????
 MisBeginCondition (NoItem, 5790, 1) -- excellent????
 MisBeginAction (TakeItem, 5796, 1) -- removal of the key props secret treasures 4 
 MisBeginAction (AddMission, 1287) 
 MisBeginAction (AddTrigger, 12871, TE_GETITEM, 2589, 5) 
 MisBeginAction (AddTrigger, 12872, TE_GETITEM, 5703, 2) 
 MisBeginAction (AddTrigger, 12873, TE_GETITEM, 3000, 1) 
 MisBeginAction (AddTrigger, 12874, TE_GETITEM, 3122, 5) 
 MisCancelAction (ClearMission, 1287) 
 MisNeed (MIS_NEED_ITEM, 2589, 5, 10, 5) 
 MisNeed (MIS_NEED_ITEM, 5703, 2, 20, 2) 
 MisNeed (MIS_NEED_ITEM, 3000, 1, 30, 1) 
 MisNeed (MIS_NEED_ITEM, 3122, 5, 40, 5) 
 
 MisResultTalk ( "<t> well, you get king's treasures from a huge step forward! Do next is to the energy consumption of <r???????> End come to see me come back!") 
 MisHelpTalk ( "<t> how, and not to collect Qi? Even these things can be done, what you would like to treasure the king ... ...") 
 MisResultCondition (HasMission, 1287) 
 MisResultCondition (HasItem, 2589, 5) 
 MisResultCondition (HasItem, 5703, 2) 
 MisResultCondition (HasItem, 3000, 1) 
 MisResultCondition (HasItem, 3122, 5) 
 MisResultAction (TakeItem, 2589, 5) 
 MisResultAction (TakeItem, 5703, 2) 
 MisResultAction (TakeItem, 3000, 1) 
 MisResultAction (TakeItem, 3122, 5) 
 MisResultAction (ClearMission, 1287) 
 MisResultAction (SetRecord, 1287) -- Add record1287 
 
 MisResultAction (GiveItem, 5790, 1, 4) ------------------ excellent????
 MisResultBagNeed (1) ------------------ because the task is completed to give props to the players, so pay attention to the space left by backpack 

 InitTrigger () 
 TriggerCondition (1, IsItem, 2589) 
 TriggerAction (1, AddNextFlag, 1287, 10, 5) 
 RegCurTrigger (12871) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 5703) 
 TriggerAction (1, AddNextFlag, 1287, 20, 2) 
 RegCurTrigger (12872) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3000) 
 TriggerAction (1, AddNextFlag, 1287, 30, 1) 
 RegCurTrigger (12873) 
 InitTrigger () 
 TriggerCondition (1, IsItem, 3122) 
 TriggerAction (1, AddNextFlag, 1287, 40, 5) 
 RegCurTrigger (12874) 

 ---------------------------------------- St. fighters cosplay -------------------- 
 DefineMission (6239, "St. fighters cosplay", 1288) 
 MisBeginTalk ( "<t> Zodiac gold I'd like to see articles???play ...... but the arts, said that he would live, injured????crusade against brigands,??crazy, went to Beijing to see?????the Olympic ...... you can replace them with me if I fell down to see what evil the Pope? ") 
 MisBeginCondition (NoMission, 1288) 
 MisBeginCondition (NoRecord, 1288) 
 MisBeginCondition (NoRecord, 1300) 
 MisBeginCondition (NoRecord, 1303) 
 MisBeginCondition (NoRecord, 1304) 
 MisBeginAction (AddMission, 1288) 
 MisBeginAction (SetRecord, 1288) 
 MisCancelAction (SystemNotice, "the task can not be canceled.") 

 MisNeed (MIS_NEED_DESP, "Go find the city's guardian of households??it ......??mainland in Ascaron (2110,2677), Zilongjin sea in the Magicsea (1126, 3416), the ice in the dark blue of the sea (1533,699), blink in the Caribbean (619, 965) ") 
 
 MisHelpTalk ( "<t>??it? Zilongjin it? Ice it? Ah it blink? Yihui it?") 
 MisResultCondition (AlwaysFailure) 
 -------------------------------------- St. fighters cosplay -- --??-------------------- 
 DefineMission (6240, "St. fighters cosplay", 1288) 
 MisHelpTalk ( "<t> to collect all the certificates to find COSPLAY Yihui.") 
 MisBeginCondition (AlwaysFailure) 
 
 MisResultTalk ( "ah! You come, I just need you here to do so.") 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasMission, 1288) 
 MisResultCondition (NoRecord, 1289) 
 MisResultAction (SetRecord, 1289) 
 
 --Test??------------------------------ ------------------ 
 DefineMission (6241, "??test", 1289) 
 MisBeginTalk ( "<t>????brigands in the crusade in the craft when the error was injured, and you immediately go to 30 Wizard fruit healing for him and get killed by the brigands that the root of all evil leader, he used as proof of the belt.") 
 
 MisBeginCondition (HasMission, 1288) 
 MisBeginCondition (HasRecord, 1288) 
 MisBeginCondition (HasRecord, 1289) 
 MisBeginCondition (NoRecord, 1291) 
 MisBeginAction (AddMission, 1289) 
 MisBeginAction (SetRecord, 1290) 
 MisCancelAction (SystemNotice, "the task can not be canceled.") 
 MisNeed (MIS_NEED_DESP, "<t> to collect 30 Elven brigands fruit, and a belt!") 
 MisHelpTalk ( "<t> what? You told me to go? Hey, if it were not for me and Athena???. Not ...... is to protect Athena, how do I let you go!") 
 MisResultTalk ( "<t> ha ha! ? with the????will be able to let him have been raised to produce beef cattle to make beef??the pot! This is for you, and I approved of your brave!") 

 MisResultCondition (HasMission, 1289) 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasRecord, 1289) 
 MisResultCondition (HasRecord, 1290) 

 MisResultCondition (HasItem, 3116, 30) 
 MisResultCondition (HasItem, 4495, 1) 
 

 MisResultAction (TakeItem, 3116, 30) 
 MisResultAction (TakeItem, 4495, 1) 
 MisResultAction (ClearRecord, 1290) 
 

 MisResultAction (GiveItem, 5813, 1, 4) 
 MisResultAction (GiveItem, 3094, 1, 4) 
 MisResultAction (ClearMission, 1289) 
 MisResultAction (SetRecord, 1291) 
 MisResultBagNeed (2) 

 -------------------------------------- St. fighters cosplay -- -- Zilongjin -------------------- 
 DefineMission (6242, "St. fighters cosplay", 1288) 
 MisBeginCondition (AlwaysFailure) 
 MisHelpTalk ( "<t> to collect all the certificates to find COSPLAY Yihui.") 
 MisResultTalk ( "ah! You come, I just need you here to do so.") 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasMission, 1288) 
 MisResultCondition (NoRecord, 1292) 
 MisResultAction (SetRecord, 1292) 
 --Test Zilongjin ------------------------------ ------------------ 
 DefineMission (6243, "Zilongjin test", 1290) 
 MisBeginTalk ( "<t> I have been to persuade the President of the Good Shepherd, but he said the night view of the non--telling, what poured soy sauce sugar beet, and in any case, there are five topics, you should answer them all correctly, and I give you recognition .") 
 MisBeginCondition (HasMission, 1288) 
 MisBeginCondition (HasRecord, 1288) 
 MisBeginCondition (HasRecord, 1292) 
 MisBeginCondition (NoRecord, 1294) 
 MisBeginCondition (NoRecord, 1295) 
 MisBeginAction (AddMission, 1290) 
 MisBeginAction (SetRecord, 1294) 
 MisCancelAction (SystemNotice, "the task can not be canceled.") 

 MisHelpTalk ( "<t> knowledge is power, but their own arrogant eyes would have been fooled.") 
 MisResultTalk ( "<t> Tu, indeed, you are the beet ...... This is my recognition to you.") 
 
 
 MisResultCondition (HasMission, 1290) 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasRecord, 1292) 
 MisResultCondition (HasRecord, 1293) 
 MisResultCondition (HasRecord, 1294) 
 
 
 MisResultAction (GiveItem, 5814, 1, 4) 
 MisResultAction (GiveItem, 3094, 1, 4) 
 MisResultAction (ClearMission, 1290) 
 MisResultAction (ClearRecord, 1294) 
 MisResultAction (ClearRecord, 1293) 
 MisResultAction (SetRecord, 1295) 
 MisResultBagNeed (2) 

 --St. ---------------------- fighters cosplay -- -- ice ------------------ 
 DefineMission (6244, "St. fighters cosplay", 1288) 
 MisBeginCondition (AlwaysFailure) 
 MisHelpTalk ( "<t> to collect all the certificates to find COSPLAY Yihui.") 
 MisResultTalk ( "ah! You come, I just need you here to do so.") 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasMission, 1288) 
 MisResultCondition (NoRecord, 1296) 
 MisResultAction (SetRecord, 1296) 
 ------------------ ------------------------------ The test of the ice 
 DefineMission (6245, "ice test", 1291) 
 MisBeginTalk ( "<t>??crazy, he two places at once into 3, I did not understand a sense of the ninth,???him, evidently, do you rely on. You need to call me a coupon for call out I kill him in order to gain recognition. However, the production of tickets call free, oh no. Also, if you are even here in my coupon did not call a win, it would not get recognized, oh my! ") 
 MisBeginCondition (HasMission, 1288) 
 MisBeginCondition (HasRecord, 1288) 
 MisBeginCondition (HasRecord, 1296) 
 MisBeginCondition (NoRecord, 1297) 
 MisBeginCondition (NoRecord, 1298) 
 MisBeginAction (AddMission, 1291) 
 MisBeginAction (SetRecord, 1297) 
 MisBeginAction (AddTrigger, 12911, TE_KILL, 1070, 1) 
 MisCancelAction (SystemNotice, "the task can not be canceled.") 
 
 MisHelpTalk ( "<t> not I buy a ticket, you can not get to prove Oh!") 
 MisResultTalk ( "<t> ...... really appreciate your sense of the ninth, this is my proof to you.") 
 MisResultCondition (HasFlag, 1291, 10) 
 MisResultCondition (HasMission, 1291) 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasRecord, 1297) 
 MisResultCondition (HasRecord, 1296) 
 MisResultCondition (HasRecord, 1298) 
 

 MisResultAction (GiveItem, 5815, 1, 4) 
 MisResultAction (ClearMission, 1291) 
 MisResultAction (ClearRecord, 1297) 
 MisResultAction (ClearRecord, 1298) 
 MisResultAction (SetRecord, 1299) 
 MisResultBagNeed (1) 
 
 InitTrigger () 
 TriggerCondition (1, IsMonster, 1070) 
 TriggerAction (1, AddNextFlag, 1291, 10, 1) 
 RegCurTrigger (12911) 
 ------------------ ------------------------------ Blink test 
 DefineMission (6246, "St. fighters cosplay", 1288) 
 MisBeginCondition (AlwaysFailure) 
     MisHelpTalk ( "<t> to collect all the certificates to find COSPLAY Yihui.") 
 MisResultTalk ( "ah! You good to collect the other three individuals to the recognition of, ah, I am here to give you direct your cosplay certificate. Baekyangsa End collect, Taurus, Gemini,???certification of cosplay, another skull Island to find my brother Yihui, you can enter the latest challenge. In addition, they can complete all three cases, the task was repeated once again to find me the task to obtain the qualifications they oh! ") 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasMission, 1288) 
 MisResultCondition (NoMission, 1289) 
 MisResultCondition (NoMission, 1290) 
 MisResultCondition (NoMission, 1291) 
 MisResultCondition (HasRecord, 1289) 
 MisResultCondition (HasRecord, 1291) 
 MisResultCondition (HasRecord, 1292) 
 MisResultCondition (HasRecord, 1295) 
 MisResultCondition (HasRecord, 1296) 
 MisResultCondition (HasRecord, 1299) 
 MisResultAction (ClearRecord, 1289) 
 MisResultAction (ClearRecord, 1291) 
 MisResultAction (ClearRecord, 1292) 
 MisResultAction (ClearRecord, 1295) 
 MisResultAction (ClearRecord, 1296) 
 MisResultAction (ClearRecord, 1299) 
 MisResultAction (SetRecord, 1300) 
 MisResultAction (SetRecord, 1301) 
 MisResultAction (GiveItem, 5806, 1, 4) 
 MisResultAction (TakeItem, 5815, 1, 4) 
 MisResultAction (TakeItem, 5814, 1, 4) 
 MisResultAction (TakeItem, 5813, 1, 4) 
 MisResultBagNeed (1) 

 ------------------------------------------------------------------------------------ ---------- Test Yihui ------ 
 DefineMission (6247, "St. fighters cosplay", 1288) 
 MisBeginCondition (AlwaysFailure) 

 MisResultTalk ( "ah, you come, will begin performances! To fight it, gold saint fighter! Started from the moment you can not find a bronze warrior saint!") 
 MisResultCondition (HasRecord, 1288) 
 MisResultCondition (HasMission, 1288) 
 MisResultCondition (HasRecord, 1300) 
 MisResultCondition (HasRecord, 1301) 
 MisResultAction (ClearRecord, 1288) 
 MisResultAction (ClearRecord, 1301) 
 MisResultAction (ClearMission, 1288) 
 MisResultAction (SetRecord, 1302) 
 ---------------------------------------------------------------------------- ---------------- Kill the evil Pope ---- 
 DefineMission (6248, "kill the Pope's evil", 1292) 
 MisBeginTalk ( "<t> Well, now, do you use one of four recognized certificate cosplay to me, I can give you to call for the Pope's evil things, but here you have not changed anything in my words, even if Pope to kill the evil, will not recognize the??! ") 
 MisHelpTalk ( "<t> a kill as long as the Pope, you can find the prize was??!") 
 MisBeginCondition (HasRecord, 1302) 
 MisBeginCondition (LvCheck, ">", 54) 
 MisBeginCondition (NoRecord, 1303) 
 MisBeginCondition (NoRecord, 1304) 
 MisBeginCondition (NoMission, 1291) 
 MisBeginAction (AddMission, 1292) 
 MisBeginAction (SetRecord, 1303) 
 MisBeginAction (AddTrigger, 12921, TE_KILL, 1071, 1) 
 MisNeed (MIS_NEED_KILL, 1071, 1, 10, 1) 

 InitTrigger () 
 TriggerCondition (1, IsMonster, 1071) 
 TriggerAction (1, AddNextFlag, 1292, 10, 1) 
 RegCurTrigger (12921) 

 MisResultCondition (AlwaysFailure) 
 --Exchange ------------------------------------------------------------------------ VI. A gift ------------------ ------ 
 DefineMission (6249, "kill the Pope's evil", 1292) 
 MisBeginCondition (AlwaysFailure) 
 MisResultTalk ( "Thank you, I finally want to see this has been the scene. This is a gift to you, we have to remember to put the gold shrine inside the backpack to get good things Oh, and later more constellations, Helena to better things. However, only 1 set in place inside the backpack, oh.") 

 MisResultCondition (HasFlag, 1292, 10) 
 MisResultCondition (HasMission, 1292) 
 MisResultCondition (HasRecord, 1303) 
 MisResultCondition (HasRecord, 1305) 
 
 

 MisResultAction (GiveItem, 5812, 1, 4) 
 MisResultAction (ClearMission, 1292) 
 MisResultAction (ClearRecord, 1303) 
 MisResultAction (SetRecord, 1304) 
 MisResultBagNeed (1) 

 ------------------------------------W\  }------------  }f
	DefineMission( 6250, "Missing Sacred Flame", 1306 )
	MisBeginTalk("<t>I guess the Sacred Flame must have been robbed by the<r Sand Bandit (1065,3137)>, who are outside the Shaitan City, hurry and investigate.")
	MisBeginCondition(NoMission, 1306)
	MisBeginCondition(NoRecord, 1306)
	MisBeginCondition(NoRecord, 1307)
	MisBeginCondition(NoRecord, 1308)
	MisBeginCondition(NoRecord, 1309)
	MisBeginCondition(NoRecord, 1314)
	MisBeginCondition(NoRecord, 1315)
	MisBeginCondition(NoRecord, 1316)
	MisBeginCondition(NoRecord, 1317)
	MisBeginCondition(NoRecord, 1318)
	MisBeginCondition(NoRecord, 1319)
	MisBeginCondition(NoRecord, 1321)
	MisBeginCondition(NoRecord, 1322)
	MisBeginCondition(NoRecord, 1323)
	MisBeginAction(AddMission, 1306)
	MisCancelAction(ClearMission, 1306)
	MisBeginAction(AddTrigger, 13061, TE_KILL, 45, 10)

	MisNeed(MIS_NEED_DESP,"Relay Officer (871,3580) asks you to defeat 10 Sand Bandits.")
	MisNeed(MIS_NEED_KILL, 45, 10, 10, 10)

	MisHelpTalk("<t>Defeat 10 Sand Bandits and recover the Missing Sacred Flame.")	
	MisResultTalk("<t>Well done, young guy! Now bring the Tinder and a letter to the Blacksmith - Bash at Icicle Castle (1344,529), only him can make another Sacred Torch.")
	MisResultCondition(HasMission, 1306)
	MisResultCondition(HasFlag, 1306, 19)	
	
	MisResultAction(ClearMission,1306)
	MisResultAction(SetRecord,1306)
	MisResultBagNeed(2)
	MisResultAction(GiveItem, 5802, 1, 4)
	MisResultAction(GiveItem, 5841, 1, 4)
	MisResultAction( AddMoney , 5000)
	
	
	InitTrigger()
	TriggerCondition( 1, IsMonster, 45)	
	TriggerAction( 1, AddNextFlag, 1306, 10, 10 )
	RegCurTrigger( 13061 )
	
	
	----------------------------------W\  T}------------ F
	DefineMission( 6251, "Make a new Torch",1307 )
	MisBeginTalk("<t>Well done, we have finally found the Tinder which was missing for several long years, but to make a new Torch, I need more stuff, can you collect them for me?")
	MisBeginCondition(NoMission, 1307)
	MisBeginCondition(HasRecord, 1306)
	MisBeginCondition(HasItem, 5802, 1)
	MisBeginCondition(HasItem, 5841, 1)
	
	MisBeginAction(AddMission, 1307)
	MisCancelAction(ClearMission, 1307)
	MisBeginAction(TakeItem, 5841, 1, 4)
	
	MisNeed( MIS_NEED_DESP, "Collect 15 Ash Wood Log and 10 Crystal Ore, and take along the<r Tinder> with you to the Blacksmith - Bash at Icicle Castle (1344,529).")
	
	MisBeginAction( AddTrigger, 13071, TE_GETITEM, 3989, 15)
	MisBeginAction( AddTrigger, 13072, TE_GETITEM, 4546, 10)
	
	MisNeed( MIS_NEED_ITEM, 3989, 15, 10, 15)
	MisNeed( MIS_NEED_ITEM, 4546, 10, 20, 10)
	
	MisResultTalk( "<t>Let me have a look.")
	MisHelpTalk( "<t>Collect 15 Ash Wood Log and 10 Crystal Ore.")
	MisResultCondition( HasMission, 1307)
	MisResultCondition( HasRecord, 1306)
	MisResultCondition( HasItem, 3989, 15)
	MisResultCondition( HasItem, 4546, 10)
	MisResultCondition( HasItem, 5802, 1)


	MisResultAction( TakeItem, 3989, 15)
	MisResultAction( TakeItem, 4546, 10)
	MisResultAction( TakeItem, 5802, 1)
	MisResultAction( AddMoney , 5000)
	MisResultAction( ClearMission, 1307)
	MisResultAction( ClearRecord, 1306)
	MisResultAction( SetRecord, 1307)
	
	
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 3989)	
	TriggerAction( 1, AddNextFlag, 1307, 10, 15)
	RegCurTrigger( 13071 )
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 4546)	
	TriggerAction( 1, AddNextFlag, 1307, 20, 10)
	RegCurTrigger( 13072 )
	
	----------------------------------W\  F------------  F
	DefineMission( 6252, "Make a new Torch",1308 )
	MisBeginTalk("<t>The stuff is enough, you see, I'm getting old, I'm hurry and thirsty now, can you find 10 Red Dates and 10 Snowy Soft Bud for me? After eating, I can work.")
	MisBeginCondition(NoMission, 1308)
	MisBeginCondition(NoRecord, 1308)
	MisBeginCondition(HasRecord, 1307)
	
	MisBeginAction(AddMission, 1308)
	MisCancelAction(ClearMission, 1308)
	
	
	MisNeed( MIS_NEED_DESP, "<r Blacksmith - Bash(1344,529)> at Icicle Castle ask you to collect <r10 Red Dates> and <r10 Snowy Soft Bud>, and bring them to him after collecting.")
	
	MisBeginAction( AddTrigger, 13081, TE_GETITEM, 3117, 10)
	MisBeginAction( AddTrigger, 13082, TE_GETITEM, 3136, 10)
	
	MisNeed( MIS_NEED_ITEM, 3117, 10, 10, 10)
	MisNeed( MIS_NEED_ITEM, 3136, 10, 20, 10)
	
	MisResultTalk( "<t>You are really a big help.")
	MisHelpTalk( "<t>You can't do it, right?")
	MisResultCondition( HasMission, 1308)
	MisResultCondition( HasRecord, 1307)
	MisResultCondition( HasItem, 3117, 10)
	MisResultCondition( HasItem, 3136, 10)

	MisResultBagNeed(1)
	MisResultAction( TakeItem, 3117, 10)
	MisResultAction( TakeItem, 3136, 10)
	MisResultAction(GiveItem, 5842, 1, 4)       
	MisResultAction( ClearMission, 1308)
	MisResultAction( ClearRecord, 1307)
	MisResultAction( SetRecord, 1308)
	MisResultAction( AddMoney , 5000)
	
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 3117)	
	TriggerAction( 1, AddNextFlag, 1308, 10, 10)
	RegCurTrigger( 13081 )
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 3136)	
	TriggerAction( 1, AddNextFlag, 1308, 20, 10)
	RegCurTrigger( 13082 )
	
----------------------------------W\  c}------------F_
	DefineMission( 6253, "Ignite Sacred Flame",1309 )
	MisBeginTalk( "<t>The new Sacred Torch is almost ready, now look back Relay Officer (871,3580) at Shaitan.")
	MisBeginCondition(NoRecord, 1309 )
	MisBeginCondition(NoMission, 1309 )
	MisBeginCondition(HasRecord, 1308 )
	MisBeginAction(AddMission, 1309 )
	MisCancelAction(ClearMission, 1309)
		
	MisNeed(MIS_NEED_DESP,"Go Shaitan and search the Relay Officer(871,3580).")
	MisHelpTalk("<t>The new Sacred Torch is almost ready, now look back Relay Officer (871,3580) at Shaitan.")
	MisResultCondition( AlwaysFailure )
	
----------------------------------W\  c}------------}fY
	DefineMission( 6254, "Ignite Sacred Flame",1309, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Hey! Nice job, you finally made the Sacred Flame.")
	MisResultCondition( HasMission, 1309)
	MisResultCondition( NoRecord, 1309)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1309)
	MisResultAction( SetRecord, 1309)
	MisResultAction( AddMoney , 5000)

----------------------------------W\  ------------}f	_
	DefineMission( 6255, "Loving Heart",1310 )
	MisBeginTalk( "<t>Ok, we are almost complete, now go to Thundoria Harbor and look for Tourist - Barbi (994,1234).")
	MisBeginCondition(NoRecord, 1310 )
	MisBeginCondition(NoMission, 1310 )
	MisBeginCondition(HasRecord, 1309 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1310)
	MisCancelAction(ClearMission, 1310)
	
	MisNeed(MIS_NEED_DESP,"Ok, we are almost complete, now go to Thundoria Harbor and look for Tourist - Barbi (994,1234).")
	MisHelpTalk("<t>Go talk Tourist - Barbi(994,1234) at Thundoria Harbor.")
	MisResultCondition( AlwaysFailure )
----------------------------------W\  ------------?	Y
	DefineMission( 6256, "Loving Heart",1310, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Finally, you are here!")
	MisResultCondition( HasMission, 1310)
	MisResultCondition( NoRecord, 1310)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1310)
	MisResultAction( SetRecord, 1310)
	MisResultAction( AddMoney , 5000)
----------------------------------W\  2------------?	_
	DefineMission( 6257, "Loving Heart 2",1311 )
	MisBeginTalk( "<t>I have another mission for you!")
	MisBeginCondition(NoRecord, 1311 )
	MisBeginCondition(NoMission, 1311 )
	MisBeginCondition(HasRecord, 1310 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1311)
	MisCancelAction(ClearMission, 1311)
	
	MisNeed(MIS_NEED_DESP,"Go search the Merman Prince - Hassan at 1254,3491.")
	MisHelpTalk("<t>Talk with the Merman Prince - Hassan at 1254,3491.")
	MisResultCondition( AlwaysFailure )
----------------------------------W\  2------------~?	Y
    DefineMission( 6258, "Loving Heart 2",1311, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Hello i am the Merman Prince - Hassan, seems that Barbi has sent you.")
	MisResultCondition( HasMission, 1311)
	MisResultCondition( NoRecord, 1311)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1311)
	MisResultAction( SetRecord, 1311)
	MisResultAction( AddMoney , 5000)
----------------------------------W\  3------------~?	_
    DefineMission( 6259, "Loving Heart 3",1312 )
	MisBeginTalk( "<t>I know somebody that can help you, Harbor Operator - Gregory.")
	MisBeginCondition(NoRecord, 1312 )
	MisBeginCondition(NoMission, 1312)
	MisBeginCondition(HasRecord, 1311 )
	MisBeginCondition( HasItem, 5842, 1)
   	MisBeginAction(AddMission, 1312)
	MisCancelAction(ClearMission, 1312)
	
	MisNeed(MIS_NEED_DESP,"Search for Harbor Operator - Gregory at 194,1715.")
	MisHelpTalk("<t>Search for Harbor Operator - Gregory at 194,1715.")
	MisResultCondition( AlwaysFailure )
----------------------------------W\  3------------]?	Y
	DefineMission( 6260, "Loving Heart 3",1312, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Wow! ! ! ! You do look at my")
	MisResultCondition( HasMission, 1312)
	MisResultCondition( NoRecord, 1312)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1312)
	MisResultAction( SetRecord, 1312)
	MisResultAction( AddMoney , 5000)
----------------------------------W\  4------------]?	_
	DefineMission( 6261, "",1313 )
	MisBeginTalk( "<t>In spring than in the town there is a call to build a middle-aged men, all day long kept muttering, Oh, may need help.")
	MisBeginCondition(NoRecord, 1313 )
	MisBeginCondition(NoMission, 1313)
	MisBeginCondition(HasRecord, 1312 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1313)
	MisCancelAction(ClearMission, 1313)
	
	MisNeed(MIS_NEED_DESP,"Spring to the town of middle-aged man looking for more than covered - (3235,2550), are you concerned about!")
	MisHelpTalk("<t>This is also how you, go to town to find the spring than the middle-aged man covered - (3235,2550) it!")
	MisResultCondition( AlwaysFailure )
----------------------------------W\  4------------?w	 Y
	DefineMission( 6262, "",1313, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>I also thought it was really good, there with, oh.")
	MisResultCondition( HasMission, 1313)
	MisResultCondition( NoRecord, 1313)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1313)
	MisResultAction( SetRecord, 1313)
	MisResultAction( AddMoney , 5000)
----------------------------------W\  5------------?w	 _
	DefineMission( 6263, "",1314 )
	MisBeginTalk( "<t>Oh, I understand, if you want to prove that they have a caring heart, you still have to find a hell sent to make.")
	MisBeginCondition(NoRecord, 1314 )
	MisBeginCondition(NoMission, 1314)
	MisBeginCondition(HasRecord, 1313 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1314)
	MisCancelAction(ClearMission, 1314)
	
	MisNeed(MIS_NEED_DESP,"Go find the Caribbean sent to hell to make (690,1043) it!")
	MisHelpTalk("<t>Let the children pay close attention to time, send hell to make the Caribbean(690,1043)!")
	MisResultCondition( AlwaysFailure )
----------------------------------W\  5------------z	 Y
	
	DefineMission( 6264, "",1314, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Haha, seen here every day Many brave to risk finally met today, but also a caring person.")
	MisResultCondition( HasMission, 1314)
	MisResultCondition( NoRecord, 1314)
	MisResultCondition( HasItem, 5842, 1)
	MisResultAction( ClearMission, 1314)
	MisResultAction( SetRecord, 1314)
	MisResultAction( AddMoney , 5000)
----------------------------------W\  6-----------z	 _
	DefineMission( 6265, "K",1315 )
	MisBeginTalk( "<t>It seems that you have fully proved that you love, but having a caring heart is not enough, fast return big torch relay. So there, perhaps he will give you further inspiration!")
	MisBeginCondition(NoRecord, 1315 )
	MisBeginCondition(NoMission, 1315)
	MisBeginCondition(HasRecord, 1314 )
	MisBeginCondition( HasItem, 5842, 1)
    	MisBeginAction(AddMission, 1315)
	MisCancelAction(ClearMission, 1315)
	
	MisNeed(MIS_NEED_DESP,"Back to the torch relay fast Ambassador (871,3580), where you take a look at what inspired him to give it!")
	MisHelpTalk("<t>Ambassador torch relay in (871,3580), where you speed up the pace, hastened to leave it!")
	MisResultCondition( AlwaysFailure )

----------------------------------W\  6------------}f	 Y
	
	DefineMission( 6266, "K",1315, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>Whether the game or life, I hope that you can treat themselves, love others, and seriously every day! There are more tests in the waiting for you, hope you can adhere to in the end!")
	MisResultCondition( HasMission, 1315)
	MisResultCondition( NoRecord, 1315)
	MisResultCondition( HasItem, 5842, 1)
	--MisResultCondition( ItemAttrNum, 5825, 6, 10000, 0)
	MisResultBagNeed(1)
	MisResultAction( ClearMission, 1315)
	MisResultAction( SetRecord, 1315)

	MisResultAction( GiveItem, 5797 , 1 , 4)
	MisResultAction( AddMoney , 5000)

	
	----------------------------------W\   o^------------}f	 _
	DefineMission( 6267, "",1316 )
	MisBeginTalk( "<t>To prove yourself, let me see how brave you are in the end!")
	MisBeginCondition(NoRecord, 1316)
	MisBeginCondition(NoMission, 1316)
	MisBeginCondition(HasRecord, 1315 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1316)
	MisCancelAction(ClearMission, 1316)
	
	MisNeed(MIS_NEED_DESP,"With the rough-and-tumble ice Manager (1374,529) to talk about, she will tell you how to complete the test!")
	MisHelpTalk("<t>Ice before you go to the rough-and-tumble Manager (1374,529) you complete the test to return to me.")
	MisResultCondition( AlwaysFailure )
	
	---------------------------------W\  o^------------yT	 Y	
	DefineMission( 6268, "",1316, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk( "<t>If I guess right, you are also recommended to the Ambassador of the torch relay of it! Then we begin.")
	MisResultCondition( HasMission, 1316)
	MisResultCondition( NoRecord, 1316)
	MisResultCondition( HasItem, 5842, 1)	
	MisResultAction( ClearMission, 1316)
	MisResultAction( ClearRecord, 1315)
	MisResultAction( SetRecord, 1316)
	MisResultAction( AddMoney , 5000)
	
	----------------------------------W\   o^------------yT	 _
	DefineMission( 6269, "o",1317 )
	MisBeginTalk( "<t>The test is to see if you brave enough. I will give you evidence of a fearless, it will record your war Bucket Information, do you have to do is successful enemy 10 times, and then the card with fearless ambassador went to see the torch relay (871,3580). You ready?")
	MisResultBagNeed(1)
	MisBeginCondition(NoRecord, 1317)
	MisBeginCondition(NoMission, 1317)
	MisBeginCondition(HasRecord, 1316 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1317)
	
	MisBeginAction(GiveItem, 5803, 1 ,41)
	MisCancelAction(SysteamNotice, "The task can not be interrupted.")
	MisResultCondition( AlwaysFailure )
	MisNeed(MIS_NEED_DESP,"When the certificate of fearless killing a total of 10 after a few to look for sand-lan City torch relay, Ambassador (871,3580) to complete any Services.")
	MisHelpTalk("<t>Fearless only when the card number over the murder in order to complete the task of 10:00 Oh, you must be with the other torch.")
	MisBeginBagNeed(1)
	
	---------------------------------W\  o^------------}f	 Y
	DefineMission( 6270, "o",1317)
	MisBeginCondition(AlwaysFailure )
	MisResultTalk("<t>Good good! Did not expect so soon you will be able to complete the test is really good! Continue to the next test it.")	
	
	MisResultCondition(HasMission, 1317)
	MisResultCondition(HasRecord, 1316)	
	MisResultCondition(HasItem, 5842, 1)
	MisResultCondition(CheckPoint, 5803)

	MisResultBagNeed(2)
	MisResultAction(TakeItem, 5803, 1)
	MisResultAction(ClearMission, 1317) 
	MisResultAction(ClearRecord, 1316)
	MisResultAction(GiveItem, 5798, 1, 4)
	MisResultAction(GiveItem, 3096, 2, 4)
	MisResultAction(ClearRecord, 1316)
	MisResultAction(SetRecord, 1317) 
	MisResultAction( AddMoney , 5000)
	MisResultBagNeed(1)
	
	---------------------------------W\  I------------}f	 _
	DefineMission( 6271, "I",1318 )
	MisBeginTalk( "<t>The world's greatest selfless dedication is ...")
	MisBeginCondition(NoRecord, 1318)
	MisBeginCondition(NoMission, 1318)
	MisBeginCondition(HasRecord, 1317 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1318)
	MisCancelAction(ClearMission, 1318)
	MisNeed(MIS_NEED_DESP,"Silver miners and the second floor of (296,57) to talk about, he will tell you how to complete the test!")
	MisHelpTalk("<t>Before you go to the second floor of the silver miners (296,57) it completed a test to return to me.")
	MisResultCondition( AlwaysFailure )
	
	---------------------------------W\  I------------V	 Y
	DefineMission( 6272, "I",1318, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk("<t>How do you come so late, the moon was nearly down the...")	
	MisResultCondition(HasMission, 1318)
	MisResultCondition(HasRecord, 1317)	
	MisResultCondition(HasItem, 5842, 1)
	MisResultAction(ClearMission, 1318)
	MisResultAction(ClearRecord, 1317)
	MisResultAction(SetRecord, 1318) 
	MisResultAction( AddMoney , 5000)
	
	---------------------------------W\  I------------V	
	DefineMission( 6273, "I",1319 )
	MisBeginTalk( "<t>The test is to you for the torch relay activities point to donate supplies, you can not hope to get something valuable to muddle through! I need five sacred Indian Wizard fai!")
	MisBeginCondition(NoRecord, 1319)
	MisBeginCondition(NoMission, 1319)
	MisBeginCondition(HasRecord, 1318 )	
	MisBeginAction(AddMission, 1319)
	MisCancelAction(ClearMission, 1319)

	MisNeed(MIS_NEED_DESP,"Miners (296,57) 5 is asking you to donate to his wizard-hui India.")	
	MisHelpTalk("<t>You see, we work so hard every day is to greet the torch relay activities, you should also be funded by some of our East West now, we just want to print 5 more than the wizard-hui!")
	
	MisResultTalk("<t>Good, our favorite wizard-hui and India, too Thank you, I will tell you the performance of the torch relay Ambassador!")	
	
	MisResultCondition(HasMission, 1319)
	MisResultCondition(HasRecord, 1318)
	MisResultCondition(HasItem, 2588, 5)
	
	MisResultAction(ClearMission, 1319)	
	MisResultAction(TakeItem, 2588,5)
	MisResultAction(ClearRecord, 1318)
	MisResultAction(SetRecord, 1319) 
	MisResultAction( AddMoney , 5000)


	---------------------------------W\  I------------V	 _
	DefineMission( 6274, "I",1320 )
	MisBeginTalk( "<t>Torch relay to go back quickly to find you, Ambassador, do you already know the performance of his, and he was looking forward to meeting you again!")
	MisBeginCondition(NoRecord, 1320)
	MisBeginCondition(NoMission, 1320)
	MisBeginCondition(HasRecord, 1319 )
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1320)
	MisCancelAction(ClearMission, 1320)
	MisNeed(MIS_NEED_DESP,"And sand to the city of the torch relay lan Ambassador (871,3580) talk about it, he was very satisfied with the performance of your.")
	MisHelpTalk("<t>Ambassador torch relay in the sand on the city-lan (871,3580).")
	MisResultCondition( AlwaysFailure )
	
	---------------------------------W\  I------------}f	 Y
	DefineMission( 6275, "I",1320, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk("<t>We have also met, the performance of you I have heard, I am glad you have such a rare dedication, hope Can you build a good performance!")	
	MisResultCondition(HasMission, 1320)
	MisResultCondition(HasRecord, 1319)	
	MisResultCondition(HasItem, 5842, 1)

	MisResultBagNeed(1)
	MisResultAction(ClearMission, 1320)
	MisResultAction(ClearRecord, 1319)
	MisResultAction(SetRecord, 1320) 
	MisResultAction(GiveItem, 5800,1,4)
	MisResultAction( AddMoney , 5000)

	
	---------------------------------W\  -----------}f _
	DefineMission( 6276, "",1321 )
	MisBeginTalk( "<t>Pirate King in the world with great wisdom, a wise man, and she is to live in paradise (1755,908) of the goddess, I have mentioned you and her, she would also like to see you. Go fast, not everyone has the opportunity to see and goddess Surface.")
	MisBeginCondition(NoRecord, 1321)
	MisBeginCondition(NoMission, 1321)
	MisBeginCondition(HasRecord, 1320)
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1321)
	MisCancelAction(ClearMission, 1321)
	MisNeed(MIS_NEED_DESP,"To heaven (1755,908) to find her talk about it, she wants to see you.")
	MisHelpTalk("<t>(1755,908)sl")
	MisResultCondition( AlwaysFailure )
	
	---------------------------------W\  ------------	 Y
	DefineMission( 6277, "",1321, COMPLETE_SHOW )
	MisBeginCondition(AlwaysFailure )
	MisResultTalk("<t>Ambassador you are referred to the young people? Am glad to see you...")	
	MisResultCondition(HasMission, 1321)
	MisResultCondition(HasRecord, 1320)	
	MisResultCondition(HasItem, 5842, 1)
	MisResultAction(ClearMission, 1321)
	MisResultAction(ClearRecord, 1320)
	MisResultAction(SetRecord, 1321) 
	MisResultAction( AddMoney , 5000)
	
	---------------------------------W\  ------------	 _
	DefineMission( 6278, "",1322 )
	MisBeginTalk( "<t>Ambassador torch you ask me to be responsible for the test, you'll prepare Oh! I need you to help me with some of the East West to come back ... a <legend can be used to treat difficult to heal the wounds have a good effect of> items and a <Fist Essential goods hit fans> items. Even if this were thought to show that you can also stupid than it Jiugui.")
	MisBeginCondition(NoRecord, 1322)
	MisBeginCondition(NoMission, 1322)
	MisBeginCondition(HasRecord, 1321)
	MisBeginCondition( HasItem, 5842, 1)
    MisBeginAction(AddMission, 1322)
	MisCancelAction(ClearMission, 1322)
	
	MisHelpTalk("<t>Goddess (1755,908) of the items need and items, in the end not think of what it? Are you also more than Jiugui ")
	MisNeed(MIS_NEED_DESP,"Goddess need for a goods and a <r Fist Essential goods hit fans> articles.")
	MisBeginAction(AddTrigger, 13221, TE_GETITEM, 4435, 1)          
	MisBeginAction(AddTrigger, 13222, TE_GETITEM, 4468, 1)	
	
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 4435)	
	TriggerAction( 1, AddNextFlag, 1322, 10, 1)
	RegCurTrigger( 13221 )
	
	InitTrigger()
	TriggerCondition( 1, IsItem, 4468)	
	TriggerAction( 1, AddNextFlag, 1322, 20, 1)
	RegCurTrigger( 13222 )
	
	MisResultTalk("<t>Really smart! It is no wonder that so good ambassador not to proud of you ... Oh, I also need you to answer some of the wisdom of the heart problem, only you to be fully answered correctly to complete the test!")

	MisResultCondition(HasMission, 1322)
	MisResultCondition(NoRecord, 1322)
	MisResultCondition(HasRecord, 1321)
	MisResultCondition(HasItem, 5842, 1)
	MisResultCondition(HasItem, 4435, 1)
	MisResultCondition(HasItem, 4468, 1)
	MisResultAction(TakeItem, 4435, 1)
	MisResultAction(TakeItem, 4468, 1)	
	MisResultAction(ClearMission, 1322)
	MisResultAction( AddMoney , 5000)
	MisResultAction(ClearRecord, 1321)
	MisResultAction(SetRecord, 1322)  ----NPCrecord  |l
	
	
	---------------------------------W\  }------------}f	 _
	DefineMission( 6279, "}",1323 )	
	MisBeginTalk( "<t>You have completed the test in front of four, and I here there is a strong heart, he can only insist on the completion of the previous Four tests, and take it, young man! With five hearts and find the flame torch to torch lit Taiwan it! Flame Taiwan. Should be on the lan port in the sand near the.")
	
	MisResultBagNeed(1)
	MisBeginCondition(NoRecord, 1322)
	
	MisBeginCondition(HasItem, 5801, 1)
	MisBeginCondition(NoMission, 1323)
	MisBeginAction(AddMission, 1323)		
	MisBeginAction(GiveItem, 5799, 1, 4)
	MisBeginBagNeed(1)
	MisCancelAction(SystemNotice, "The task can not be interrupted.")
	MisNeed(MIS_NEED_DESP,"With 5 hearts and find the flame torch to Taiwan.") 
	MisHelpTalk("<t>Must not lose heart, as well as a lesson to any torch, otherwise you will fall short of the.")
	MisResultCondition( AlwaysFailure )
	
	---------------------------------W\  }------------}	 Y
	DefineMission( 6280, "}",1323, COMPLETE_SHOW )	
	MisBeginCondition( AlwaysFailure )
	MisResultTalk("<t>Congratulations! You have successfully passed the test of five hearts!")	
	MisResultCondition(HasMission, 1323)		
	MisResultCondition(HasItem, 5842, 1)   --
	MisResultCondition(HasItem, 5797, 1)   -- 
	MisResultCondition(HasItem, 5798, 1)   --o
	MisResultCondition(HasItem, 5799, 1)   --
	MisResultCondition(HasItem, 5800, 1)   --I
	MisResultCondition(HasItem, 5801, 1)   --
	
	
	MisResultAction(ClearMission, 1323)	
	MisResultAction(SetRecord, 1324)
	MisResultAction(ClearRecord, 1323)	
	MisResultAction( AddMoney , 5000)
	
	---------------------------------------------	 _
	DefineMission (6628, "", 1860)
	MisBeginTalk( "<t>Read Ascaron Food King TV right? The reason why the legendary master chef has been on everybody's lips is because he has a Magic skills can be a kind of cooking is called Feast of Ascaron, the dessert that can enhance the properties of permanent!")
	MisBeginCondition(NoRecord, 1860)
	MisBeginCondition(NoRecord, 1863)
	MisBeginCondition(NoMission, 1860)
	MisBeginAction(AddMission, 1860)		
	MisNeed(MIS_NEED_DESP,"Knowledge of food, however, Wang has not found a few people, I heard that silver bar with alcoholic - Fender (2222,2889). There are him.") 
	MisHelpTalk("<t>Heard silver bar with alcoholic - Fender (2222,2889), where his message, go to ask it.")
	MisCancelAction(ClearMission, 1860)
	MisResultCondition( AlwaysFailure )
	
	---------------------------------------------	 Y
	DefineMission( 6623, "",1860, COMPLETE_SHOW )	
	MisBeginCondition( AlwaysFailure )
	MisResultTalk("<t>Do you want to know the news of the legendary master chef? I first meet the requirements of a small bar!")	
	MisResultCondition(HasMission, 1860)
	MisResultAction(SetRecord, 1860)
	MisResultAction(ClearMission, 1860)
	
	---------------------------------2------------
	DefineMission (6624, "{E", 1861)
	MisBeginTalk( "<t>I do not know, the drink, I may have more than enough, if you help me in the bar girl - Tina where to buy bottles of red wine. So, I might think of it, of course, if I Louis XVI, I may be able to think faster.")
	MisBeginCondition(HasRecord, 1860)
	MisBeginCondition(NoRecord, 1861)
	MisBeginCondition(NoMission, 1861)
	MisBeginAction(AddMission, 1861)
		
	MisNeed(MIS_NEED_DESP,"Durable to 0, only the red wine, or Louis XVI on the second box backpack Oh to be valid.") 
	MisHelpTalk("<t>Young people, alcohol is not bad, but I do not think of it, please go to the bar I bought some!")
	MisResultTalk("<t>News for red wine to the success rate is 10%! News for Louis XVI to the success rate of 20%! A close look at the bottom left corner of the. Tips to see you are a success or failure of the.")

	MisResultCondition(Jiu_Check)
	MisResultAction(Jiu_Action)
	MisCancelAction(ClearMission, 1861)
	
	-------------------------------3----------- _
	DefineMission (6625, "{", 1862)
	MisBeginTalk( "<t>Young people, Ascaron Food, Wang has been a sea adventure to the east, go left over the next before the age of the paintings of strange ingredients in small volumes. Son, perhaps the only residents - Margaret (2279,2746) in order to understand it.")
	MisBeginCondition(NoRecord, 1862)	
	MisBeginCondition(HasRecord, 1861)
	MisBeginCondition(NoMission, 1862)
	MisBeginAction(AddMission, 1862)		
	MisNeed(MIS_NEED_DESP,"Silver City residents - Margaret (2279,2746) may be able to understand these things, go look at her, perhaps a non - Price of the treasures of this.") 
	MisHelpTalk("<t>Silver City residents looking for Margaret - (2279,2746) it, but her Guru food, this may make things. She made it absolutely delicious goods!")
	MisCancelAction(ClearMission, 1862)

	MisResultCondition( AlwaysFailure )
	
	---------------------------------3------------	 Y
	DefineMission( 6626, "{",1862, COMPLETE_SHOW )	
	MisBeginCondition( AlwaysFailure )
	MisResultTalk("<t>King of Ascaron food tips! Alas, alas, the production of desserts, Feast of Ascaron, a detailed record of the need for flour, salt, Special spices, deep-sea shark ... .... Light of the purchase cost of materials is very alarming!")	
	MisResultCondition(HasMission, 1862)
	MisResultAction(SetRecord, 1862)
	MisResultAction(ClearMission, 1862)	

	---------------------------------4-----------
	DefineMission (6627, "c{", 1863)
	MisBeginTalk( "<t>I have a rough calculation, it is necessary to spend in order to successfully produce 1,888,888 dessert Feast of Ascaron.")
	MisBeginCondition(HasRecord, 1862)
	MisBeginCondition(NoRecord, 1863)
	MisBeginCondition(NoMission, 1863)
	MisBeginAction(AddMission, 1863)
	
	MisNeed(MIS_NEED_DESP,"Cost can be successfully produced 1,888,888 dessert Feast of Ascaron.")
	MisHelpTalk("<t>Young people, money should not come to fool me, although I am not a small age, but not old!")
	MisResultTalk("<t>To! This is the magic of the dessert, Feast of Ascaron, said to increase the consumption of the property at Oh! While it is hot to eat, million a failure that can be!")
	
	MisResultBagNeed(1)
	MisResultCondition(HasMoney, 1888888)
	MisResultAction(TakeMoney, 1888888)
	MisResultAction(GiveItem, 6610, 1, 4)
	MisResultAction(SetRecord, 1863)
	MisResultAction(ClearMission, 1863)
	MisCancelAction(ClearMission, 1863)	
		
		---------------------------------C------------S
	DefineMission( 6629, "C", 1920 )
	
	MisBeginTalk( "<t>Young people want a single proof of forgery is very simple, as long as six kinds of treasures ready to use the material as a forgery, and then branch. Some fees can be paid manually. False statements to help you fake But I am a great risk to Tam, TU. . . Close your pirate 1,000,000. Currency is not too much, just look at the acquaintance's sake to make a 9.9 fold you, only you 990,000.")
	MisBeginCondition(NoMission, 1920)
	MisBeginCondition(NoRecord, 1920)
	MisBeginAction(AddMission, 1920)

	MisBeginAction(AddTrigger, 19201, TE_GETITEM, 1638, 20 )
	MisBeginAction(AddTrigger, 19202, TE_GETITEM, 1641, 20 )
	MisBeginAction(AddTrigger, 19203, TE_GETITEM, 3363, 20 )
	MisBeginAction(AddTrigger, 19204, TE_GETITEM, 1644, 20 )
	MisBeginAction(AddTrigger, 19205, TE_GETITEM, 3362, 20 )
	MisBeginAction(AddTrigger, 19206, TE_GETITEM, 3360, 1 )
	MisCancelAction(ClearMission, 1920)


	MisNeed(MIS_NEED_ITEM, 1638, 20, 0, 20)
	MisNeed(MIS_NEED_ITEM, 1641, 20, 20, 20)
	MisNeed(MIS_NEED_ITEM, 3363, 20, 40, 20)
	MisNeed(MIS_NEED_ITEM, 1644, 20, 60, 20)
	MisNeed(MIS_NEED_ITEM, 3362, 20, 80, 20)
	MisNeed(MIS_NEED_ITEM, 3360, 1, 100, 1)
		
	MisHelpTalk("<t>]C!")
	MisResultTalk("<t>So many materials you have been a homogeneous collection.")
	MisResultCondition(NoRecord, 1920)
	MisResultCondition(HasMission, 1920)
	MisResultCondition(HasItem, 3360, 1)
	MisResultCondition(HasItem, 1638, 20)
	MisResultCondition(HasItem, 1641, 20)
	MisResultCondition(HasItem, 3363, 20)
	MisResultCondition(HasItem, 1644, 20)
	MisResultCondition(HasItem, 3362, 20)
	MisResultCondition(HasMoney, 990000)
	MisResultAction(TakeItem, 3360, 1)
	MisResultAction(TakeItem, 1638, 20)
	MisResultAction(TakeItem, 1641, 20)
	MisResultAction(TakeItem, 3363, 20)
	MisResultAction(TakeItem, 1644, 20)
	MisResultAction(TakeItem, 3362, 20)
	MisResultAction(TakeMoney, 990000)
	MisResultAction(GiveItem, 6703, 1,4)    --r]u   oy
	MisResultAction(ClearMission, 1920)
	MisResultAction(SetRecord, 1920)
	--MisResultAction(AddExp, 800, 800)
--	MisResultAction(AddMoney,270,270)



	InitTrigger()
	TriggerCondition( 1, IsItem, 1638 )	
	TriggerAction( 1, AddNextFlag, 1920, 20, 20 )
	RegCurTrigger( 19201 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 1641 )	
	TriggerAction( 1, AddNextFlag, 1920, 40, 20 )
	RegCurTrigger( 19202 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3363 )	
	TriggerAction( 1, AddNextFlag, 1920, 60, 20 )
	RegCurTrigger( 19203 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 1644 )	
	TriggerAction( 1, AddNextFlag, 1920, 80, 20 )
	RegCurTrigger( 19204 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3362 )	
	TriggerAction( 1, AddNextFlag, 1920, 100, 20 )
	RegCurTrigger( 19205 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3360 )	
	TriggerAction( 1, AddNextFlag, 1920, 110, 1 )
	RegCurTrigger( 19206 )
----2009.2.3  zI ID6629_ record1920_  uguM|_
