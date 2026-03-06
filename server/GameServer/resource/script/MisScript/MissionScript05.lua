print("-- [Loading] Mission Script [05]")

----------------------------------------------------------
--							--
--							--
--			 				--
--							--
--							--
----------------------------------------------------------
	-->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
function AreaMission001()

-----------------------------------
	DefineMission( 600, "Honey Combs Heist", 600 )
	
	MisBeginTalk( "<t>As the saying goes, nab the leader and you will get the rest. Killing those <rBarbaric Bee> will not help much but it's better than not doing anything about them at all.<n><t>Can you go to their lair and steal 5 <yBeehives>?")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 600)
	MisBeginCondition(NoRecord, 600)
	MisBeginAction(AddMission, 600)
	MisBeginAction(AddTrigger, 6001, TE_GETITEM, 4085, 5 )
	MisCancelAction(ClearMission, 600)

	MisNeed(MIS_NEED_ITEM, 4085, 5, 10, 5)
		
	MisHelpTalk("<t>The sting of <rBarbaric Bees> are very painful! Please be careful!")
	MisResultTalk("<t>Hehe! My idea is useful, right?")
	MisResultCondition(NoRecord, 600)
	MisResultCondition(HasMission, 600)
	MisResultCondition(HasItem, 4085, 5)
	MisResultAction(TakeItem, 4085, 5)
	MisResultAction(ClearMission, 600)
	MisResultAction(SetRecord, 600)
	MisResultAction(AddExp, 800, 800)
	MisResultAction(AddMoney,270,270)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4085 )	
	TriggerAction( 1, AddNextFlag, 600, 10, 5 )
	RegCurTrigger( 6001 )

-----------------------------------
	DefineMission( 601, "Bee Eradication", 601 )
	
	MisBeginTalk( "<t>Oh my...These <rBarbaric Bees> are becoming wilder. I only passed by the flowerbed where they have been collecting their nectar when they started attacking me.<n><t>Now my face is swollen badly, it simply peeves me!<n><t>Can you please destroy 10 <rBarbaric Bees>? Teach these wild bees a lesson!<n><t>They can be found around <j(1623, 3139)>!")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(HasRecord, 600)
	MisBeginCondition(NoMission, 601)
	MisBeginCondition(NoRecord, 601)
	MisBeginAction(AddMission, 601)
	MisBeginAction(AddTrigger, 6011, TE_KILL, 139, 10 )
	MisCancelAction(ClearMission, 601)

	MisNeed(MIS_NEED_KILL, 139, 10, 10, 10)
	
	MisHelpTalk("<t>Ouch! My face hurts! Have you destroyed those <rBarbaric Bees>?")
	MisResultTalk("<t>Haha! This should teach those <rBarbaric Bees> that I am not somebody to be trifle with!")
	MisResultCondition(NoRecord, 601)
	MisResultCondition(HasMission, 601)
	MisResultCondition(HasFlag, 601, 19 )
	MisResultAction(ClearMission, 601)
	MisResultAction(SetRecord, 601)
	MisResultAction(AddExp, 800, 800)
	MisResultAction(AddMoney,270,270)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 139 )	
	TriggerAction( 1, AddNextFlag, 601, 10, 10 )
	RegCurTrigger( 6011 )

-----------------------------------
	DefineMission( 602, "Unsettling Dream", 602 )
	
	MisBeginTalk( "<t>Ah, I am just about to look for you! For the past 2 days, I have been unable to sleep because of the sounds made by <rOwlie>. Every time I lie down on bed I would hear the \"Hoot\" of the <rOwlie>.<n><t>I am old and I needs sleep. This can't go on, please help me get rid of 10 <rOwlies>. They can be found at <j(1384, 3065)>.")
	MisBeginCondition(LvCheck, ">", 20 )
	MisBeginCondition(HasRecord, 603)
	MisBeginCondition(NoMission, 602)
	MisBeginCondition(NoRecord, 602)
	MisBeginAction(AddMission, 602)
	MisBeginAction(AddTrigger, 6021, TE_KILL, 224, 10 )
	MisCancelAction(ClearMission, 602)

	MisNeed(MIS_NEED_KILL, 224, 10, 10, 10)
	
	MisHelpTalk("<t>Hunt 10 <rOwlies> will do.")
	MisResultTalk("<t>Thank you. I think I will be able to sleep peacefully from now on.")
	MisResultCondition(NoRecord, 602)
	MisResultCondition(HasMission, 602)
	MisResultCondition(HasFlag, 602, 19 )
	MisResultAction(ClearMission, 602)
	MisResultAction(SetRecord, 602)
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney,285,285)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 224 )	
	TriggerAction( 1, AddNextFlag, 602, 10, 10 )
	RegCurTrigger( 6021 )


-----------------------------------
	DefineMission( 603, "Scholar Owlie", 603 )
	
	MisBeginTalk( "<t>My eyes are must be seeing things...Yesterday I actually saw an <rOwlie> holding a book with its claw! It can't be that its reading the book? This is all too weird!<n><ts>orry to bother, but could you obtain 5 <yOwlie's Claws> and return  here? I wish to observe this strange occurence!<n><t>Usually, these <Owlies> appear around <j(1384, 3065)>.")
	MisBeginCondition(LvCheck, ">", 20 )
	MisBeginCondition(NoMission, 603)
	MisBeginCondition(NoRecord, 603)
	MisBeginAction(AddMission, 603)
	MisBeginAction(AddTrigger, 6031, TE_GETITEM, 4432, 5 )
	MisCancelAction(ClearMission, 603)

	MisNeed(MIS_NEED_ITEM, 4432, 5, 10, 5)
	
	MisHelpTalk("<t>Please bring me 5 <yOwl Talons> for research..")
	MisResultTalk("<t>Hmm? This talon is similar to talons of other owls. Strangeam I going nuts?")
	MisResultCondition(NoRecord, 603)
	MisResultCondition(HasMission, 603)
	MisResultCondition(HasItem, 4432, 5)
	MisResultAction(TakeItem, 4432, 5 )
	MisResultAction(ClearMission, 603)
	MisResultAction(SetRecord, 603)
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney,571,571)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4432 )	
	TriggerAction( 1, AddNextFlag, 603, 10, 5 )
	RegCurTrigger( 6031 )

-----------------------------------
	DefineMission( 604, "Cunning Owl", 604 )
	
	MisBeginTalk( "<ts>orry, can you help me? I was reading a book two days back when an <rOwlie> flew pass and tore out a few pages of my book. I need these <yLost Pages> urgently.<n><t>Can you retrieve them back for me?<n><t>It should be lying around their nest itself.")
	MisBeginCondition(LvCheck, ">", 21 )
	MisBeginCondition(NoMission, 604)
	MisBeginCondition(NoRecord, 604)
	MisBeginAction(AddMission, 604)
	MisBeginAction(AddTrigger, 6041, TE_GETITEM, 4086, 5 )
	MisCancelAction(ClearMission, 604)

	MisNeed(MIS_NEED_ITEM, 4086, 5, 10, 5)
	
	MisHelpTalk("<t>Please bring me the <yLost Pages>!")
	MisResultTalk("<t>This is great! Thank you!")
	MisResultCondition(NoRecord, 604)
	MisResultCondition(HasMission, 604)
	MisResultCondition(HasItem, 4086, 5)
	MisResultAction(TakeItem, 4086, 5 )
	MisResultAction(ClearMission, 604)
	MisResultAction(SetRecord, 604)
	MisResultAction(AddExp, 1100, 1100)
	MisResultAction(AddMoney,300,300)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4086 )	
	TriggerAction( 1, AddNextFlag, 604, 10, 5 )
	RegCurTrigger( 6041 )

-----------------------------------
	DefineMission( 605, "Temptation of BBQ Tails", 605 )
	
	MisBeginTalk( "<t>This is the biggest favour I have ever asked! You must promise me!<n><t>Really? You agreed? Then get me 5 <yShort Boar Tail>! I have been drooling over the notion of eating some <yShort Boar Tail> for a very long time!<n><t><rTusk Battle Boar> can be found at <j(1384, 3065)>.")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(NoMission, 605)
	MisBeginCondition(NoRecord, 605)
	MisBeginAction(AddMission, 605)
	MisBeginAction(AddTrigger, 6051, TE_GETITEM, 4433, 5 )
	MisCancelAction(ClearMission, 605)

	MisNeed(MIS_NEED_ITEM, 4433, 5, 10, 5)
	
	MisHelpTalk("<t><t> You already promised to help me get some <yShort Boar Tail>, please do not go back on your words.")
	MisResultTalk("<t>Oh my! Thanks a lot! Hehe!.")
	MisResultCondition(NoRecord, 605)
	MisResultCondition(HasMission, 605)
	MisResultCondition(HasItem, 4433, 5)
	MisResultAction(TakeItem, 4433, 5 )
	MisResultAction(ClearMission, 605)
	MisResultAction(SetRecord, 605)
	MisResultAction(AddExp, 1300, 1300)
	MisResultAction(AddMoney,632,632)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4433 )	
	TriggerAction( 1, AddNextFlag, 605, 10, 5 )
	RegCurTrigger( 6051 )

-----------------------------------
	DefineMission( 606, "Tusk Boar's Resistance", 606 )
	
	MisBeginTalk( "<t>I had a shock this morning when I opened the window, there were many <rTusk Battle Boar> running around my yard!<n><t>Look at these big sized beasts, running around in packs and spoiling everything in my yard! I can't take this lying down!<n><t>Please help me kill 10 <rTusk Battle Boars>! They can be found at <j(1384, 3065)>.")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(HasRecord, 605)
	MisBeginCondition(NoMission, 606)
	MisBeginCondition(NoRecord, 606)
	MisBeginAction(AddMission, 606)
	MisBeginAction(AddTrigger, 6061, TE_KILL, 264, 10 )
	MisCancelAction(ClearMission, 606)

	MisNeed(MIS_NEED_KILL, 264, 10, 10, 10)
	
	MisHelpTalk("<t>You only need to hunt down 10 <rTusk Battle Boars>.")
	MisResultTalk("<t>Ha! Now my garden is free of those pesks. Thank you!")
	MisResultCondition(NoRecord, 606)
	MisResultCondition(HasMission, 606)
	MisResultCondition(HasFlag, 606, 19 )
	MisResultAction(ClearMission, 606)
	MisResultAction(SetRecord, 606)
	MisCancelAction(ClearMission, 607)
	MisResultAction(AddExp, 1300, 1300)
	MisResultAction(AddMoney,316,316)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 264 )	
	TriggerAction( 1, AddNextFlag, 606, 10, 10 )
	RegCurTrigger( 6061 )

-----------------------------------
	DefineMission( 607, "No More Odour!", 607 )
	
	MisBeginTalk( "<t>Hey friend! There is a weird scent around here, can you smell it? Oh, it must the scent of those <rAir Porky> nearby.<n><t>These pigs have become strange lately, they no longer emanate fragrant smell but instead gives off a weird odour, I think its best that we get rid of them now.<n><t>Can you please hunt and kill 10 <rAir Porky> for me? They can be found at <j(1414, 2896)>.")
	MisBeginCondition(HasRecord, 608)
	MisBeginCondition(LvCheck, ">", 23 )
	MisBeginCondition(NoMission, 607)
	MisBeginCondition(NoRecord, 607)
	MisBeginAction(AddMission, 607)
	MisBeginAction(AddTrigger, 6071, TE_KILL, 295, 10 )
	MisCancelAction(ClearMission, 607)

	MisNeed(MIS_NEED_KILL, 295, 10, 10, 10)
	
	MisHelpTalk("<t>Don't tell me you are unable to complete such a simple task! Please go and hunt down 10 <rAir Porky>!")
	MisResultTalk("<t>Well done!")
	MisResultCondition(NoRecord, 607)
	MisResultCondition(HasMission, 607)
	MisResultCondition(HasFlag, 607, 19 )
	MisResultAction(ClearMission, 607)
	MisResultAction(SetRecord, 607)
	MisResultAction(AddExp, 1500, 1500)
	MisResultAction(AddMoney,332,332)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 295 )	
	TriggerAction( 1, AddNextFlag, 607, 10, 10 )
	RegCurTrigger( 6071 )

-----------------------------------
	DefineMission( 608, "Unusual Satchet", 608 )
	
	MisBeginTalk( "<t>I believe that the incidents where the <rTusk Battle Boar> became aggressive and the weird odour of the <rAir Porky> are closely related.<n><t>I still require more evidence to prove it. Help me collect 5 <yUnusual Satchets> from the <rAir Porky> so I can study it. <rAir Porky> can only be found at <j(1414, 2896)>.")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 608)
	MisBeginCondition(NoRecord, 608)
	MisBeginAction(AddMission, 608)
	MisBeginAction(AddTrigger, 6081, TE_GETITEM, 4460, 5 )
	MisCancelAction(ClearMission, 608)

	MisNeed(MIS_NEED_ITEM, 4460, 5, 10, 5)
	
	MisHelpTalk("<t>Have you obtain 5 <yUnusual Satchets>?")
	MisResultTalk("<t>I finally understand. Its this <yUnusual Satchet> that makes the nearby boars go wild!")
	MisResultCondition(NoRecord, 608)
	MisResultCondition(HasMission, 608)
	MisResultCondition(HasItem, 4460, 5)
	MisResultAction(TakeItem, 4460, 5 )
	MisResultAction(ClearMission, 608)
	MisResultAction(SetRecord, 608)
	MisResultAction(AddExp, 1500, 1500)
	MisResultAction(AddMoney,664,664)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4460 )	
	TriggerAction( 1, AddNextFlag, 608, 10, 5 )
	RegCurTrigger( 6081 )

-----------------------------------
	DefineMission( 609, "Missing Gold Coin", 609 )
	
	MisBeginTalk( "<t>My friend, you should know me! I am always a honest merchant! However, <rBandits> have rob me of my <yGold Coin Pouch>!<n><t>I am no match for them! Could you help me get back my <yGold Coin Pouch> from their hideout?<n><t>I guess its hidden within one of their treasure chest.")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 609)
	MisBeginCondition(NoRecord, 609)
	MisBeginAction(AddMission, 609)
	MisBeginAction(AddTrigger, 6091, TE_GETITEM, 4087, 1 )
	MisCancelAction(ClearMission, 609)

	MisNeed(MIS_NEED_ITEM, 4087, 1, 10, 1)
	
	MisHelpTalk("<t>Those <rBandits> might have hidden my <yGold Coin Pouch> in a chest. Please look carefully!")
	MisResultTalk("<t>You are my saviour!<n><t>I wonder how am I going to survive without my pouch!")
	MisResultCondition(NoRecord, 609)
	MisResultCondition(HasMission, 609)
	MisResultCondition(HasItem, 4087, 1)
	MisResultAction(TakeItem, 4087, 1 )
	MisResultAction(ClearMission, 609)
	MisResultAction(SetRecord, 609)
	MisResultAction(AddExp, 1800, 1800)
	MisResultAction(AddMoney,1394,1394)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4087 )	
	TriggerAction( 1, AddNextFlag, 609, 10, 1 )
	RegCurTrigger( 6091 )


-----------------------------------
	DefineMission( 610, "The Terrible Bandits", 610 )
	
	MisBeginTalk( "<t>Hey friend! Its time for you to show your skills! A group of menacing <rBandits> encrouches in the area nearby, robbing everyone of their money and goods!<n><t>Quickly capture 10 <rBandits> and bring them here to claim your reward!<n><t>Those outlaws can be found at <j(1043, 3066)>.")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 610)
	MisBeginCondition(NoRecord, 610)
	MisBeginAction(AddMission, 610)
	MisBeginAction(AddTrigger, 6101, TE_KILL, 93, 10 )
	MisCancelAction(ClearMission, 610)

	MisNeed(MIS_NEED_KILL, 93, 10, 10, 10)
	
	MisHelpTalk("<t>Kill 10 <rBandits> and return for your rewards!")
	MisResultTalk("<t>Woah! Nicely done! Here are your rewards!")
	MisResultCondition(NoRecord, 610)
	MisResultCondition(HasMission, 610)
	MisResultCondition(HasFlag, 610, 19 )
	MisResultAction(ClearMission, 610)
	MisResultAction(SetRecord, 610)
	MisResultAction(AddExp, 1800, 1800)
	MisResultAction(AddMoney,348,348)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 93 )	
	TriggerAction( 1, AddNextFlag, 610, 10, 10 )
	RegCurTrigger( 6101 )


-----------------------------------
	DefineMission( 611, "Burning of Stramonium", 611 )
	
	MisBeginTalk( "<t>Rumor has it that <rStramonium> is a kind of flower that possess intelligence. They sway and dance in tune with the music played.<n><t>I don't believe such things exist as I never seen it before.<n><t>Can you please obtain 2 <yStramonium Sharp Spike> so that I can conduct my research on it. These weird plants are rumored to be found at <j(1414, 2896)>.")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 611)
	MisBeginCondition(NoRecord, 611)
	MisBeginAction(AddMission, 611)
	MisBeginAction(AddTrigger, 6111, TE_GETITEM, 4088, 2 )
	MisCancelAction(ClearMission, 611)

	MisNeed(MIS_NEED_ITEM, 4088, 2, 10, 2)
	
	MisHelpTalk("<t>2 <yStramonium Sharp Spike> will do.")
	MisResultTalk("<t>Thank you! With these, I will be able to carry on with my research.")
	MisResultCondition(NoRecord, 611)
	MisResultCondition(HasMission, 611)
	MisResultCondition(HasItem, 4088, 2)
	MisResultAction(TakeItem, 4088, 2 )
	MisResultAction(ClearMission, 611)
	MisResultAction(SetRecord, 611)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,730,730)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4088 )	
	TriggerAction( 1, AddNextFlag, 611, 10, 2 )
	RegCurTrigger( 6111 )

-----------------------------------
	DefineMission( 612, "Eradicate Stramonium", 612 )
	
	MisBeginTalk( "<t>The people of the Shepherd Plains regard the <rStramonium> as a demonic flower. Its branches are filled with thorns and it is able to move freely, quite an eerie sight.<n><t>I hope you are willing to destroy 20 <rStramoniums> so as to let the people here live in peace. It is rumored that these weird flowers are found at <j(1414, 2896)>.")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 612)
	MisBeginCondition(NoRecord, 612)
	MisBeginAction(AddMission, 612)
	MisBeginAction(AddTrigger, 6121, TE_KILL, 85, 20 )
	MisCancelAction(ClearMission, 612)

	MisNeed(MIS_NEED_KILL, 85, 20, 10, 20)
	
	MisHelpTalk("<t>Exterminate 20 <rStramoniums> please.")
	MisResultTalk("<t>Well done! The villagers are grateful to you!")
	MisResultCondition(NoRecord, 612)
	MisResultCondition(HasMission, 612)
	MisResultCondition(HasFlag, 612, 29 )
	MisResultAction(ClearMission, 612)
	MisResultAction(SetRecord, 612)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,365,365)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 85 )	
	TriggerAction( 1, AddNextFlag, 612, 10, 20 )
	RegCurTrigger( 6121 )


-----------------------------------
	DefineMission( 613, "The Contest", 613 )
	
	MisBeginTalk( "<t>Hey friend! You came at the right time! A new exciting competition is being held here!<n><t>You only need to defeat 10 <rRookie Boxeroos> to win prizes!<n><t>Now that we are short of manpower, can you proceed to the location alone? The competition  location is at <j(1117, 2923)>.")
	MisBeginCondition(HasRecord, 614 )
	MisBeginCondition(LvCheck, ">", 26 )
	MisBeginCondition(NoMission, 613)
	MisBeginCondition(NoRecord, 613)
	MisBeginAction(AddMission, 613)
	MisBeginAction(AddTrigger, 6131, TE_KILL, 76, 10 )
	MisCancelAction(ClearMission, 613)

	MisNeed(MIS_NEED_KILL, 76, 10, 10, 10)
	
	MisHelpTalk("<t>You must defeated 10 <rRookie Boxeroos> in order to claim your rewards.")
	MisResultTalk("<t>Woah! Nicely done! Here are your rewards!")
	MisResultCondition(NoRecord, 613)
	MisResultCondition(HasMission, 613)
	MisResultCondition(HasFlag, 613, 19 )
	MisResultAction(ClearMission, 613)
	MisResultAction(SetRecord, 613)
	MisResultAction(AddExp, 2300, 2300)
	MisResultAction(AddMoney,382,382)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 76 )	
	TriggerAction( 1, AddNextFlag, 613, 10, 10 )
	RegCurTrigger( 6131 )

-----------------------------------
	DefineMission( 614, "Boxing Gloves", 614 )
	
	MisBeginTalk( "<t>Friend, can you lend me a hand?  Its my brother's birthday in the next few days and he wishes to have a <yBoxing Gloves> as a birthday present.<n><t>But I can't get it in such a rural place. There's no other choice, can you please snatch 1 <yBoxing Gloves> from <rRookie Boxeroo>.<n><t>Can you help me please? I heard these <rRookie Boxeroo> are having a boxing competition at <j(1117, 2923)>! This is a golden opportunity!")
	MisBeginCondition(LvCheck, ">", 26 )
	MisBeginCondition(NoMission, 614)
	MisBeginCondition(NoRecord, 614)
	MisBeginAction(AddMission, 614)
	MisBeginAction(AddTrigger, 6141, TE_GETITEM, 4435, 1 )
	MisCancelAction(ClearMission, 614)

	MisNeed(MIS_NEED_ITEM, 4435, 1, 10, 1)
	
	MisHelpTalk("<t>I only need 1 pair of <yBoxing Gloves>!")
	MisResultTalk("<t>Thank you! It must have been hard on you to rob a <rRookie Boxeroo>.")
	MisResultCondition(NoRecord, 614)
	MisResultCondition(HasMission, 614)
	MisResultCondition(HasItem, 4435, 1)
	MisResultAction(TakeItem, 4435, 1 )
	MisResultAction(ClearMission, 614)
	MisResultAction(SetRecord, 614)
	MisResultAction(AddExp, 2300, 2300)
	MisResultAction(AddMoney,382,382)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4435 )	
	TriggerAction( 1, AddNextFlag, 614, 10, 1 )
	RegCurTrigger( 6141 )

-----------------------------------
	DefineMission( 615, "Grass Tortoise's Tradegy", 615 )
	
	MisBeginTalk( "<t>I hate the <rMature Grass Tortoise>! Every time I see these slow lumbering creatures, I get very irritated.<n><t>They are generally a waste of time!<n><t>I would if I could help them walk faster! Woah, these creatures shouldn't even exist!<n><t>Go kill 10 <rMature Grass Tortoise> now! Those creatures have been crawling around for half a century already but are still at <j(1198, 3116)>!")
	MisBeginCondition(LvCheck, ">", 27 )
	MisBeginCondition(NoMission, 615)
	MisBeginCondition(NoRecord, 615)
	MisBeginAction(AddMission, 615)
	MisBeginAction(AddTrigger, 6151, TE_KILL, 135, 10 )
	MisCancelAction(ClearMission, 615)

	MisNeed(MIS_NEED_KILL, 135, 10, 10, 10)
	
	MisHelpTalk("<t>Kill! Kill! Kill all the <rSlowpoke Snail>!")
	MisResultTalk("<t>This feels great! Thanks!")
	MisResultCondition(NoRecord, 615)
	MisResultCondition(HasMission, 615)
	MisResultCondition(HasFlag, 615, 19 )
	MisResultAction(ClearMission, 615)
	MisResultAction(SetRecord, 615)
	MisResultAction(AddExp, 2600, 2600)
	MisResultAction(AddMoney,400,400)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 135 )	
	TriggerAction( 1, AddNextFlag, 615, 10, 10 )
	RegCurTrigger( 6151 )

-----------------------------------
	DefineMission( 616, "Stealing Tortoise Eggs", 616 )
	
	MisBeginTalk( "<t>Hey friend, let me tell you a secret! The price of <rMature Grass Tortoise> is very high in the current market now!<n><t>However, I can't handle these large monsters myself.<n><t>Can you please go to where <rMature Grass Tortoise> lay their eggs and steal 10 <yTortoise Egg>.<n><t>When the money arrives, I will share half of it with you, how about it? It is said they can be found at <j(1198, 3116)>.<n><t>(Search carefully when you are at the area of the mature grass tortoise, you should be able to find their nest within those bushes, the eggs are inside the nest.)")
	MisBeginCondition(LvCheck, ">", 27 )
	MisBeginCondition(NoMission, 616)
	MisBeginCondition(NoRecord, 616)
	MisBeginAction(AddMission, 616)
	MisBeginAction(AddTrigger, 6161, TE_GETITEM, 4089, 10 )
	MisCancelAction(ClearMission, 616)

	MisNeed(MIS_NEED_ITEM, 4089, 10, 10, 10)
	
	MisHelpTalk("<t>Have you collected 10 <yTortoise Eggs>? I need them to earn money.")
	MisResultTalk("<t>Haha! I will make a fortune with these! I will not forget your help.")
	MisResultCondition(NoRecord, 616)
	MisResultCondition(HasMission, 616)
	MisResultCondition(HasItem, 4089, 10)
	MisResultAction(TakeItem, 4089, 10 )
	MisResultAction(ClearMission, 616)
	MisResultAction(SetRecord, 616)
	MisResultAction(AddExp, 2600, 2600)
	MisResultAction(AddMoney,400,400)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4089 )	
	TriggerAction( 1, AddNextFlag, 616, 10, 10 )
	RegCurTrigger( 6161 )

-----------------------------------
	DefineMission( 617, "Smuggle Tortoise Shells", 617 )
	
	MisBeginTalk( "<t>For the past few days, I been using <yTortoise Egg> from the <rMature Grass Tortoise> to raise many grass tortoise and then selling them for a high price.<n><t>Now, turtle shells are becoming the market hottest commodity.<n><t>Can you please collect 5 <yWell-Formed Tortoise Shells> for me? I will prepare to smuggle them to <pIcicle City>.<n><t>These guys move really slow and I think they can be found at <j(1198, 3116)>.")
	MisBeginCondition(HasRecord, 616)
	MisBeginCondition(LvCheck, ">", 28 )
	MisBeginCondition(NoMission, 617)
	MisBeginCondition(NoRecord, 617)
	MisBeginAction(AddMission, 617)
	MisBeginAction(AddTrigger, 6171, TE_GETITEM, 4465, 5 )
	MisCancelAction(ClearMission, 617)

	MisNeed(MIS_NEED_ITEM, 4465, 5, 10, 5)
	
	MisHelpTalk("<t>Collect 5 <yWell-Formed Tortoise Shells> for me!")
	MisResultTalk("<t>You are very efficient! Thanks!")
	MisResultCondition(NoRecord, 617)
	MisResultCondition(HasMission, 617)
	MisResultCondition(HasItem, 4465, 5)
	MisResultAction(TakeItem, 4465, 5 )
	MisResultAction(ClearMission, 617)
	MisResultAction(SetRecord, 617)
	MisResultAction(AddExp, 3000, 3000)
	MisResultAction(AddMoney,835,835)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4465 )	
	TriggerAction( 1, AddNextFlag, 617, 10, 5 )
	RegCurTrigger( 6171 )

-----------------------------------
	DefineMission( 618, "A Terrible Curse", 618 )
	
	MisBeginTalk( "<t>Hehe, my friend, let me tell you a secret. Recently I have learnt a new kind of curse and I am going to use it on my enemy <bKentaro>.<n><t>However, I am still lacking some <yRazor Sharp Tusks>. Can you get 10 <yRazor Sharp Tusks> from the <rMad Boar> for me so that I can finish this curse!<n><t>These <rMad Boar> can be found at <j(910, 2971)>.")
	MisBeginCondition(LvCheck, ">", 28 )
	MisBeginCondition(NoMission, 618)
	MisBeginCondition(NoRecord, 618)
	MisBeginAction(AddMission, 618)
	MisBeginAction(AddTrigger, 6181, TE_GETITEM, 4443, 10 )
	MisCancelAction(ClearMission, 618)

	MisNeed(MIS_NEED_ITEM, 4443, 10, 10, 10)
	
	MisHelpTalk("<t>I need 10 <yRazor Sharp Tusks>. No more, no less!")
	MisResultTalk("<t>Thunder! Fire! Hulala! Haha! I finally completed the curse! You are doomed, <bKentaro>!")
	MisResultCondition(NoRecord, 618)
	MisResultCondition(HasMission, 618)
	MisResultCondition(HasItem, 4443, 10)
	MisResultAction(TakeItem, 4443, 10 )
	MisResultAction(ClearMission, 618)
	MisResultAction(SetRecord, 618)
	MisResultAction(AddExp, 3000, 3000)
	MisResultAction(AddMoney,835,835)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4443 )	
	TriggerAction( 1, AddNextFlag, 618, 10, 10 )
	RegCurTrigger( 6181 )

-----------------------------------
	DefineMission( 619, "Boar Independence Day", 619 )
	
	MisBeginTalk( "<t>Ah! I simply hate these <rMad Boars>! These boars are organising some independence day activity! <n><t>To commerate their escape from the slaugterhouse, groups of them have been marching around in the streets!<n><t>Can you please go kill 10 <rMad Boar> so we can have some peace around here!<t>They are gathering at <j(910,2971)> at this very moment!")
	MisBeginCondition(HasRecord, 618)
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 619)
	MisBeginCondition(NoMission, 620)
	MisBeginCondition(NoRecord, 619)
	MisBeginAction(AddMission, 619)
	MisBeginAction(AddTrigger, 6191, TE_KILL, 64, 10 )
	MisCancelAction(ClearMission, 619)

	MisNeed(MIS_NEED_KILL, 64, 10, 10, 10)
	
	MisHelpTalk("<t>Hunt down those <rMad Boars> and stop their rampage!")
	MisResultTalk("<t>The world is a better place to be in now! Thank you!")
	MisResultCondition(NoRecord, 619)
	MisResultCondition(HasMission, 619)
	MisResultCondition(HasFlag, 619, 19 )
	MisResultAction(ClearMission, 619)
	MisResultAction(SetRecord, 619)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,424,424)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 64 )	
	TriggerAction( 1, AddNextFlag, 619, 10, 10 )
	RegCurTrigger( 6191 )

-----------------------------------
	DefineMission( 620, "Boar Challenge", 620 )
	
	MisBeginTalk( "<t>Hi, have you just arrived here? Don't you wish to display your strength to the people here?<n><t>I have a way for you to prove your valor to the villagers.<n><t>Kill 5 <rMad Boars> at <j(911, 2973)> that has been plaguing our village.")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 620)
	MisBeginCondition(NoMission, 619)
	MisBeginCondition(NoRecord, 620)
	MisBeginAction(AddMission, 620)
	MisBeginAction(AddTrigger, 6201, TE_KILL, 64, 5 )
	MisCancelAction(ClearMission, 620)

	MisNeed(MIS_NEED_KILL, 64, 5, 10, 5)
	
	MisHelpTalk("<t>Kill 5 <rMad Boars> and prove your prowess to the villagers!")
	MisResultTalk("<t>Well done, my friend!")
	MisResultCondition(NoRecord, 620)
	MisResultCondition(HasMission, 620)
	MisResultCondition(HasFlag, 620, 14 )
	MisResultAction(ClearMission, 620)
	MisResultAction(SetRecord, 620)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,424,424)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 64 )	
	TriggerAction( 1, AddNextFlag, 620, 10, 5 )
	RegCurTrigger( 6201 )

-----------------------------------
	DefineMission( 621, "Expensive Herbs", 621 )
	
	MisBeginTalk( "<t>Hey, my friend, I lost several expensive herbs while on the way here.<n><t>These herbs were meant to replace the shortages in the warehouse.<n><t>But now I have nothing to show for!<n><t>Please take a pity on me and go to the plains where <rGrassland Elk> resides and pick 6 <yPrecious Herbs> for me. Wheres the plains?...I remembered!<n><t>It is just nearby at <j(1360, 2683)>.")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 621)
	MisBeginCondition(NoRecord, 621)
	MisBeginAction(AddMission, 621)
	MisBeginAction(AddTrigger, 6211, TE_GETITEM, 4090, 6 )
	MisCancelAction(ClearMission, 621)

	MisNeed(MIS_NEED_ITEM, 4090, 6, 10, 6)
	
	MisHelpTalk("<t>Have you found 6 <yPrecious Herbs>?")
	MisResultTalk("<t>Thank you for your kindness! I will remember it always!")
	MisResultCondition(NoRecord, 621)
	MisResultCondition(HasMission, 621)
	MisResultCondition(HasItem, 4090, 6)
	MisResultAction(TakeItem, 4090, 6 )
	MisResultAction(ClearMission, 621)
	MisResultAction(SetRecord, 621)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,424,424)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4090 )	
	TriggerAction( 1, AddNextFlag, 621, 10, 6 )
	RegCurTrigger( 6211 )

-----------------------------------
	DefineMission( 622, "Secret of the Grassland Elk", 622 )
	
	MisBeginTalk( "<t>I don't get it, why does the <rGrassland Elk> here run so fast and their reaction is so agile?<n><t>Please go and collect 5 <yRigid Deer Hoofs> for me as I wish to see whats the difference between it and the other elks. These springly animals seems to be living at <j(1360, 2683)>.")
	MisBeginCondition(LvCheck, ">", 30 )
	MisBeginCondition(NoMission, 622)
	MisBeginCondition(NoRecord, 622)
	MisBeginAction(AddMission, 622)
	MisBeginAction(AddTrigger, 6221, TE_GETITEM, 4372, 5 )
	MisCancelAction(ClearMission, 622)

	MisNeed(MIS_NEED_ITEM, 4372, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yRigid Deer Hoofs>! Where are they?")
	MisResultTalk("<t>I cannot see any differences. They look just like any other hoofs")
	MisResultCondition(NoRecord, 622)
	MisResultCondition(HasMission, 622)
	MisResultCondition(HasItem, 4372, 5)
	MisResultAction(TakeItem, 4372, 5 )
	MisResultAction(ClearMission, 622)
	MisResultAction(SetRecord, 622)
	MisResultAction(AddExp, 3800, 3800)
	MisResultAction(AddMoney,863,863)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4372 )	
	TriggerAction( 1, AddNextFlag, 622, 10, 5 )
	RegCurTrigger( 6221 )

-----------------------------------
	DefineMission( 623, "Deer Skin Coat", 623 )
	
	MisBeginTalk( "<t>Hey friend, to be honest, I was once a pirate before. I only escaped here because I was defeated by the <rJack Black>'s pirate crew!<n><t>I wish to wear a suit made of <yTop Grade Deer Skin> so I believe you will help me get 5 <yTop Grade Deer Skin>.<n><t>Why not? Because if you refuse to, I am going to give you a lesson you will never forget!<n><t>Go now to <j(1360, 2683)> and get the deer skin from <rGrassland Elk>.")
	MisBeginCondition(LvCheck, ">", 30 )
	MisBeginCondition(NoMission, 623)
	MisBeginCondition(NoRecord, 623)
	MisBeginAction(AddMission, 623)
	MisBeginAction(AddTrigger, 6231, TE_GETITEM, 4091, 5 )
	MisCancelAction(ClearMission, 623)

	MisNeed(MIS_NEED_ITEM, 4091, 5, 10, 5)
	
	MisHelpTalk("<t>Go get 5 <yTop Grade Deer Skins> now! Or I beat the hell out of you!")
	MisResultTalk("<t>Hehe! You are skillful indeed! Well done!")
	MisResultCondition(NoRecord, 623)
	MisResultCondition(HasMission, 623)
	MisResultCondition(HasItem, 4091, 5)
	MisResultAction(TakeItem, 4091, 5 )
	MisResultAction(ClearMission, 623)
	MisResultAction(SetRecord, 623)
	MisResultAction(AddExp, 3800, 3800)
	MisResultAction(AddMoney,863,863)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4091 )	
	TriggerAction( 1, AddNextFlag, 623, 10, 5 )
	RegCurTrigger( 6231 )

-----------------------------------
	DefineMission( 624, "White Owlie", 624 )
	
	MisBeginTalk( "<t>I hate those <rWhite Owlies>! They are making loads of noise during the day, giving me a big headache!<n><t>I will go crazy if this continues! If you don't mind, help me kill 5 <rWhite Owlies> and let me have a quiet time for a few days. Those evil creatures can be found near <j(1360, 2683)>.")
	MisBeginCondition(LvCheck, ">", 31 )
	MisBeginCondition(NoMission, 624)
	MisBeginCondition(NoRecord, 624)
	MisBeginAction(AddMission, 624)
	MisBeginAction(AddTrigger, 6241, TE_KILL, 225, 5 )
	MisCancelAction(ClearMission, 624)

	MisNeed(MIS_NEED_KILL, 225, 5, 10, 5)
	
	MisHelpTalk("<t>You only need to hunt 5 <rWhite Owlies>.")
	MisResultTalk("<t>Thank you very much! I can lead back my peaceful life again.")
	MisResultCondition(NoRecord, 624)
	MisResultCondition(HasMission, 624)
	MisResultCondition(HasFlag, 624, 14 )
	MisResultAction(ClearMission, 624)
	MisResultAction(SetRecord, 624)
	MisResultAction(AddExp, 4300, 4300)
	MisResultAction(AddMoney,438,438)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 225 )	
	TriggerAction( 1, AddNextFlag, 624, 10, 5 )
	RegCurTrigger( 6241 )

-----------------------------------
	DefineMission( 625, "Revenge", 625 )
	
	MisBeginTalk( "<t>Hey, I am glad you came, I have a favour to ask of you! I lost a batch of expensive herbs but do you know why? It because of those <rWhite Owlies> who using their <ySharp Beak> to tear open the luggages, taking the medicine back to their nest!<n><t>To exact revenge on these <rWhite Owlie>, please kill them for me and remove their <ySharp Beak> and return it to me!<n><t>Their nest seems to be at <j(1360, 2683)>.")
	MisBeginCondition(LvCheck, ">", 31 )
	MisBeginCondition(HasRecord, 621)
	MisBeginCondition(NoMission, 625)
	MisBeginCondition(NoRecord, 625)
	MisBeginAction(AddMission, 625)
	MisBeginAction(AddTrigger, 6251, TE_GETITEM, 4451, 5 )
	MisCancelAction(ClearMission, 625)

	MisNeed(MIS_NEED_ITEM, 4451, 5, 10, 5)
	
	MisHelpTalk("<t> Have you got 5 <ySharp Beaks> for me?")
	MisResultTalk("<t>Hahaha! Now I am satisfied!")
	MisResultCondition(NoRecord, 625)
	MisResultCondition(HasMission, 625)
	MisResultCondition(HasItem, 4451, 5)
	MisResultAction(TakeItem, 4451, 5 )
	MisResultAction(ClearMission, 625)
	MisResultAction(SetRecord, 625)
	MisResultAction(AddExp, 4300, 4300)
	MisResultAction(AddMoney,877,877)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4451 )	
	TriggerAction( 1, AddNextFlag, 625, 10, 5 )
	RegCurTrigger( 6251 )

-----------------------------------
	DefineMission( 626, "Pursue of the Wolf", 626 )
	
	MisBeginTalk( "<t><rGrassland Wolf> are fearsome animals, I have never seen anything more threatening to the safety of human than them!<n><t>I am requesting you to kill 5 <rGrassland Wolves> so as to drive away the wolves packs and make this a safe place!<n><t>It seems that the wolve packs are gathered at <j(1143, 2705)>.<n><t>Remember, don't not get surrounded by them as these wolves attack in packs.")
	MisBeginCondition(LvCheck, ">", 32 )
	MisBeginCondition(NoMission, 626)
	MisBeginCondition(NoRecord, 626)
	MisBeginAction(AddMission, 626)
	MisBeginAction(AddTrigger, 6261, TE_KILL, 136, 5 )
	MisCancelAction(ClearMission, 626)

	MisNeed(MIS_NEED_KILL, 136, 5, 10, 5)
	
	MisHelpTalk("<t>Have not kill 5 <rGrassland Wolf>? Hurry!")
	MisResultTalk("<t>Woah! Nicely done!")
	MisResultCondition(NoRecord, 626)
	MisResultCondition(HasMission, 626)
	MisResultCondition(HasFlag, 626, 14 )
	MisResultAction(ClearMission, 626)
	MisResultAction(SetRecord, 626)
	MisResultAction(AddExp, 4800, 4800)
	MisResultAction(AddMoney,446,446)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 136 )	
	TriggerAction( 1, AddNextFlag, 626, 10, 5 )
	RegCurTrigger( 6261 )


-----------------------------------
	DefineMission( 627, "Cruel Massacre", 627 )
	
	MisBeginTalk( "<t>I told you before I was a evil pirate! Now im still as cruel as before.<n><t>I have taken a dislike to <rGrassland Wolf>. I want you to kill these wolves and retrieve 5 <ySwift Wolve Claws> for me to decorate my room, haha!<n><t>It is said that these wolves pack can be found at <j(1143, 2705)>.")
	MisBeginCondition(HasRecord, 623)
	MisBeginCondition(LvCheck, ">", 32 )
	MisBeginCondition(NoMission, 627)
	MisBeginCondition(NoRecord, 627)
	MisBeginAction(AddMission, 627)
	MisBeginAction(AddTrigger, 6271, TE_GETITEM, 4469, 5 )
	MisCancelAction(ClearMission, 627)

	MisNeed(MIS_NEED_ITEM, 4469, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <ySwift Wolf Claws>! Get it fast!")
	MisResultTalk("<t>HmmThis fits nicely into my room! Heh!")
	MisResultCondition(NoRecord, 627)
	MisResultCondition(HasMission, 627)
	MisResultCondition(HasItem, 4469, 5)
	MisResultAction(TakeItem, 4469, 5 )
	MisResultAction(ClearMission, 627)
	MisResultAction(SetRecord, 627)
	MisResultAction(AddExp, 4800, 4800)
	MisResultAction(AddMoney,892,892)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4469 )	
	TriggerAction( 1, AddNextFlag, 627, 10, 5 )
	RegCurTrigger( 6271 )

-----------------------------------
	DefineMission( 628, "Boxing Trouble", 628 )
	
	MisBeginTalk( "<t>Wait a moment! Can you help me? While I was walking home from work last night, I was attacked by a bunch of <rBerserk Boxeroos>!<n><t>They will attack anybody aimlessly. Please put a stop to them by hunting 5 <rBerserk Boxeroos> at <j(1161, 2639)> please.")
	MisBeginCondition(LvCheck, ">", 33 )
	MisBeginCondition(NoMission, 628)
	MisBeginCondition(NoRecord, 628)
	MisBeginAction(AddMission, 628)
	MisBeginAction(AddTrigger, 6281, TE_KILL, 80, 5 )
	MisCancelAction(ClearMission, 628)

	MisNeed(MIS_NEED_KILL, 80, 5, 10, 5)
	
	MisHelpTalk("<t>Have you gotten rid of the 5 <rBerserk Boxeroos>?")
	MisResultTalk("<t>Oh! Thank you very much!")
	MisResultCondition(NoRecord, 628)
	MisResultCondition(HasMission, 628)
	MisResultCondition(HasFlag, 628, 14 )
	MisResultAction(ClearMission, 628)
	MisResultAction(SetRecord, 628)
	MisResultAction(AddExp, 5400, 5400)
	MisResultAction(AddMoney,453,453)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 80 )	
	TriggerAction( 1, AddNextFlag, 628, 10, 5 )
	RegCurTrigger( 6281 )

-----------------------------------
	DefineMission( 629, "Lucky Magical Stone", 629 )
	
	MisBeginTalk( "<tss>hh, this is a secret between me and you, I don't wish for a third party to know!<n><t>Where the <rBerserk Boxeroo> resides lies a miraculous <yLucky Magical Stone>, it is said to be a very powerful talisman capable of protecting the wearer.<n><t>Go quietly and retrieve 3 <yLucky Magical Stones> and pass it to me. I think the stones can be found at <j(1161, 2639)>. Search carefully among the bushes.")
	MisBeginCondition(LvCheck, ">", 33 )
	MisBeginCondition(NoMission, 629)
	MisBeginCondition(NoRecord, 629)
	MisBeginAction(AddMission, 629)
	MisBeginAction(AddTrigger, 6291, TE_GETITEM, 4092, 3 )
	MisCancelAction(ClearMission, 629)

	MisNeed(MIS_NEED_ITEM, 4092, 3, 10, 3)
	
	MisHelpTalk("<t>Have you retrieved the 3 <yLucky Magical Stones> that I need?")
	MisResultTalk("<t>Oh! So these are the <yLucky Magical Stone>! Thank you!")
	MisResultCondition(NoRecord, 629)
	MisResultCondition(HasMission, 629)
	MisResultCondition(HasItem, 4092, 3)
	MisResultAction(TakeItem, 4092, 3 )
	MisResultAction(ClearMission, 629)
	MisResultAction(SetRecord, 629)
	MisResultAction(AddExp, 5400, 5400)
	MisResultAction(AddMoney,453,453)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4092 )	
	TriggerAction( 1, AddNextFlag, 629, 10, 3 )
	RegCurTrigger( 6291 )

-----------------------------------
	DefineMission( 630, "Investigation of Slowpoke Snail", 630 )
	
	MisBeginTalk( "<t>I am surprised that even though <rSlowpoke Snails> move very slowly, they are still able to avoid danger all the time.<n><t>Do they have radar on their body or some kind of sensor? Could you collect 5 <yShort Snail Feelers> from <j(1227, 2742)> for me to investigate?")
	MisBeginCondition(LvCheck, ">", 34 )
	MisBeginCondition(NoMission, 630)
	MisBeginCondition(NoRecord, 630)
	MisBeginAction(AddMission, 630)
	MisBeginAction(AddTrigger, 6301, TE_GETITEM, 4473, 5 )
	MisCancelAction(ClearMission, 630)

	MisNeed(MIS_NEED_ITEM, 4473, 5, 10, 5)
	
	MisHelpTalk("<t>Hurry up! Collect 5 <yShort Snail Feelers> for me!")
	MisResultTalk("<t>Oh! I see! The <yShort Snail Feeler> itself acts as a radar for the snail!")
	MisResultCondition(NoRecord, 630)
	MisResultCondition(HasMission, 630)
	MisResultCondition(HasItem, 4473, 5)
	MisResultAction(TakeItem, 4473, 5 )
	MisResultAction(ClearMission, 630)
	MisResultAction(SetRecord, 630)
	MisResultAction(AddExp, 6100, 6100)
	MisResultAction(AddMoney,923,923)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4473 )	
	TriggerAction( 1, AddNextFlag, 630, 10, 5 )
	RegCurTrigger( 6301 )

-----------------------------------
	DefineMission( 631, "The Slowpoke Snails", 631 )
	
	MisBeginTalk( "<t>I hate <rSlowpoke Snails>!<n><t>They are so SLOW! Time is so precious for a busy merchant like me yet they are practically wasting precious time!<n><t>They should not exist in this world!<n><t>Kill 10 <rSlowpoke Snails> at <j(1227, 2742)> for me and I will reward you greatly!")
	MisBeginCondition(LvCheck, ">", 34 )
	MisBeginCondition(NoMission, 631)
	MisBeginCondition(NoRecord, 631)
	MisBeginAction(AddMission, 631)
	MisBeginAction(AddTrigger, 6311, TE_KILL, 127, 10 )
	MisCancelAction(ClearMission, 631)

	MisNeed(MIS_NEED_KILL, 127, 10, 10, 10)
	
	MisHelpTalk("<t>Kill them! Kill them all! <rMature Grass Tortoises> must be eradicated!")
	MisResultTalk("<t>This feels great! Thanks!")
	MisResultCondition(NoRecord, 631)
	MisResultCondition(HasMission, 631)
	MisResultCondition(HasFlag, 631, 19 )
	MisResultAction(ClearMission, 631)
	MisResultAction(SetRecord, 631)
	MisResultAction(AddExp, 6100, 6100)
	MisResultAction(AddMoney,461,461)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 127 )	
	TriggerAction( 1, AddNextFlag, 631, 10, 10 )
	RegCurTrigger( 6311 )


-----------------------------------
	DefineMission( 632, "Fallen", 632 )
	
	MisBeginTalk( "<t>Everything in this world is a gift from Goddess Kara. But, these people have turned their back on her teachings, ignoring the spritual energies and focusing only on technology.<n><t>I have decided to use the power of curse to punish those who have sinned against the Goddess's ideals even though this will lead me to fall away from the path.<n><t>Friend, please obtain 5 <yStrange Candles> from the Mud Monster to help me finish the last rites. These weird monsters can be found at <j(934, 2747)>.")
	MisBeginCondition(LvCheck, ">", 35 )
	MisBeginCondition(NoMission, 632)
	MisBeginCondition(NoRecord, 632)
	MisBeginAction(AddMission, 632)
	MisBeginAction(AddTrigger, 6321, TE_GETITEM, 4450, 5 )
	MisCancelAction(ClearMission, 632)

	MisNeed(MIS_NEED_ITEM, 4450, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yStrange Candles> to complete the ritual.")
	MisResultTalk("<t>Thank you for all that you have done for me!")
	MisResultCondition(NoRecord, 632)
	MisResultCondition(HasMission, 632)
	MisResultCondition(HasItem, 4450, 5)
	MisResultAction(TakeItem, 4450, 5 )
	MisResultAction(ClearMission, 632)
	MisResultAction(SetRecord, 632)
	MisResultAction(AddExp, 6800, 6800)
	MisResultAction(AddMoney,939,939)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4450 )	
	TriggerAction( 1, AddNextFlag, 632, 10, 5 )
	RegCurTrigger( 6321 )

-----------------------------------
	DefineMission( 633, "Song of the Stinging Beak", 633 )
	
	MisBeginTalk( "<t>Hey friend, listen up, I have a cruel but rewarding job for you.<n><t>An unamed person from a rich and powerful owns a <rStinging Beak>. He does not wish to see any <rStinging Beak> other than the one in his cage so he ordered the destruction of the others.<n><t>You only need to kill 10 of them and you will be rewarded greatly. <rStinging Beaks> can be found at <j(935, 2687)>!<n><t>How about it, are you willing to take this job?")
	MisBeginCondition(LvCheck, ">", 36 )
	MisBeginCondition(NoMission, 633)
	MisBeginCondition(NoRecord, 633)
	MisBeginAction(AddMission, 633)
	MisBeginAction(AddTrigger, 6331, TE_KILL, 128, 10 )
	MisCancelAction(ClearMission, 633)

	MisNeed(MIS_NEED_KILL, 128, 10, 10, 10)
	
	MisHelpTalk("<t>You have to kill 10 <rStinging Beaks> to claim the rewards.")
	MisResultTalk("<t>Very good! This is your reward. I will look for you again if there are anymore tasks for you.")
	MisResultCondition(NoRecord, 633)
	MisResultCondition(HasMission, 633)
	MisResultCondition(HasFlag, 633, 19 )
	MisResultAction(ClearMission, 633)
	MisResultAction(SetRecord, 633)
	MisResultAction(AddExp, 7600, 7600)
	MisResultAction(AddMoney,477,477)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 128 )	
	TriggerAction( 1, AddNextFlag, 633, 10, 10 )
	RegCurTrigger( 6331 )

-----------------------------------
	DefineMission( 634, "Mysterious Cape", 634 )
	
	MisBeginTalk( "<t>I wish to make a mantle out of pure feathers! Hey, this idea ain't too bad after all!<n><t>I was once a pirate and everything that I need was done by my followers, so I won't have to go and collect these feathers myself.<n><t>Although you might be clumsy, I will let it pass so go and collect for me 5 <yGlossy Feathers>.<n><t>You can start looking for it at <j(935, 2687)>.")
	MisBeginCondition(LvCheck, ">", 36 )
	MisBeginCondition(NoMission, 634)
	MisBeginCondition(NoRecord, 634)
	MisBeginAction(AddMission, 634)
	MisBeginAction(AddTrigger, 6341, TE_GETITEM, 4472, 5 )
	MisCancelAction(ClearMission, 634)

	MisNeed(MIS_NEED_ITEM, 4472, 5, 10, 5)
	
	MisHelpTalk("<t><yGlossy Feathers> can only be found on <rStinging Beaks>.")
	MisResultTalk("<t>Yeah! I got a new mantle to wear now!")
	MisResultCondition(NoRecord, 634)
	MisResultCondition(HasMission, 634)
	MisResultCondition(HasItem, 4472, 5)
	MisResultAction(TakeItem, 4472, 5 )
	MisResultAction(ClearMission, 634)
	MisResultAction(SetRecord, 634)
	MisResultAction(AddExp, 7600, 7600)
	MisResultAction(AddMoney,955,955)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4472 )	
	TriggerAction( 1, AddNextFlag, 634, 10, 5 )
	RegCurTrigger( 6341 )

-----------------------------------
	DefineMission( 635, "Trading Fake Gems", 635 )
	
	MisBeginTalk( "<t>Frankly speaking, most merchants are not honest and neither am I.<n><t>Recently someone tried to order some crystallized egg from the boars.<n><t>However, I do not have the means to obtain such an item.<n><t>Can you please get me 3 <yFearsome Tortoise Egg Shell>.<n><t>I will add some finishing touches to them and sell these fakes to the ignorant customer, haha! <n><t>These tortoise can be found at <j(969, 2587)>.")
	MisBeginCondition(LvCheck, ">", 37 )
	MisBeginCondition(NoMission, 635)
	MisBeginCondition(NoRecord, 635)
	MisBeginAction(AddMission, 635)
	MisBeginAction(AddTrigger, 6351, TE_GETITEM, 4093, 3 )
	MisCancelAction(ClearMission, 635)

	MisNeed(MIS_NEED_ITEM, 4093, 3, 10, 3)
	
	MisHelpTalk("<t>Have you found 3 <yFearsome Tortoise Egg Shells>?")
	MisResultTalk("<t>Hoho! This is your share of reward! Please keep this a secret.")
	MisResultCondition(NoRecord, 635)
	MisResultCondition(HasMission, 635)
	MisResultCondition(HasItem, 4093, 3)
	MisResultAction(TakeItem, 4093, 3 )
	MisResultAction(ClearMission, 635)
	MisResultAction(SetRecord, 635)
	MisResultAction(AddExp, 8500, 8500)
	MisResultAction(AddMoney,486,486)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4093 )	
	TriggerAction( 1, AddNextFlag, 635, 10, 3 )
	RegCurTrigger( 6351 )

-----------------------------------
	DefineMission( 636, "Emerald Fog", 636 )
	
	MisBeginTalk( "<t>The weather is too hot for me and ulcers are growing all over in my mouth. I can't even eat properly, its really irritating.<n><t>Please obtain 5 <yGreen Moss> from the <rThickskin Lizard>, from which I will apply on the ulcers for relief. They can found at <j(732, 2697)>.")
	MisBeginCondition(LvCheck, ">", 38 )
	MisBeginCondition(NoMission, 636)
	MisBeginCondition(NoRecord, 636)
	MisBeginAction(AddMission, 636)
	MisBeginAction(AddTrigger, 6361, TE_GETITEM, 4094, 5 )
	MisCancelAction(ClearMission, 636)

	MisNeed(MIS_NEED_ITEM, 4094, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yGreen Moss> to make the syrup.")
	MisResultTalk("<t>Thank you! I am feeling much better now.")
	MisResultCondition(NoRecord, 636)
	MisResultCondition(HasMission, 636)
	MisResultCondition(HasItem, 4094, 5)
	MisResultAction(TakeItem, 4094, 5 )
	MisResultAction(ClearMission, 636)
	MisResultAction(SetRecord, 636)
	MisResultAction(AddExp, 9500, 9500)
	MisResultAction(AddMoney,495,495)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4094 )	
	TriggerAction( 1, AddNextFlag, 636, 10, 5 )
	RegCurTrigger( 6361 )


-----------------------------------
	DefineMission( 637, "Search for Velvet Stone", 637 )
	
	MisBeginTalk( "<t>My friend <bMas> is a very cultured person, recently he has been trying to renovate his suite at <pIcicle City>.<n><t>However, the temperature at <pIcicle City> is chilling and he is unable to find what he needs.<n><t>I think the <ySlippery Rock> from the <rRock Golem> may suit his needs so can you get for me 5 <ySlippery Rocks> from the <rRock Golems>? It is said that these fearsome creatures appear at <j(682, 2592)>.")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(NoMission, 637)
	MisBeginCondition(NoRecord, 637)
	MisBeginAction(AddMission, 637)
	MisBeginAction(AddTrigger, 6371, TE_GETITEM, 4455, 5 )
	MisCancelAction(ClearMission, 637)

	MisNeed(MIS_NEED_ITEM, 4455, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <ySlippery Rocks>!")
	MisResultTalk("<t>These should be the rocks <bMarcusa> is looking for!")
	MisResultCondition(NoRecord, 637)
	MisResultCondition(HasMission, 637)
	MisResultCondition(HasItem, 4455, 5)
	MisResultAction(TakeItem, 4455, 5 )
	MisResultAction(ClearMission, 637)
	MisResultAction(SetRecord, 637)
	MisResultAction(AddExp, 10000, 10000)
	MisResultAction(AddMoney,1008,1008)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4455 )	
	TriggerAction( 1, AddNextFlag, 637, 10, 5 )
	RegCurTrigger( 6371 )



-----------------------------------
	DefineMission( 638, "Weird Collector", 638 )
	
	MisBeginTalk( "<t>This world is filled up all kinds of people.<n><t>I know a guy named <bChang> who is a collector of body parts.<n><t>He likes to collect all kinds of body parts.<n><t>This time round, he wants me to find 5 <yLong Lizard Tongue>.<n><t>Where can I find it! My friend, can you go instead and help me get 5 <yLong Lizard Tongue>? I heard from others that these tongues can be found at <j(892, 3273)>.")
	MisBeginCondition(LvCheck, ">", 9 )
	MisBeginCondition(NoMission, 638)
	MisBeginCondition(NoRecord, 638)
	MisBeginAction(AddMission, 638)
	MisBeginAction(AddTrigger, 6381, TE_GETITEM, 4415, 5 )
	MisCancelAction(ClearMission, 638)

	MisNeed(MIS_NEED_ITEM, 4415, 5, 10, 5)
	
	MisHelpTalk("<t>I will need 5 <yLong Lizard Tongues> to complete my task.")
	MisResultTalk("<t>Now everything is completed. Thank you!")
	MisResultCondition(NoRecord, 638)
	MisResultCondition(HasMission, 638)
	MisResultCondition(HasItem, 4415, 5)
	MisResultAction(TakeItem, 4415, 5 )
	MisResultAction(ClearMission, 638)
	MisResultAction(SetRecord, 638)
	MisResultAction(AddExp, 120, 120)
	MisResultAction(AddMoney,299,299)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4415 )	
	TriggerAction( 1, AddNextFlag, 638, 10, 5 )
	RegCurTrigger( 6381 )

-----------------------------------
	DefineMission( 639, "Playful Lizard", 639 )
	
	MisBeginTalk( "My friend! I can no longer stand those <rHopping Lizards>. They are urinating all over the desert and making the place stink!<n><t>Can you kill 10 <rHopping Lizards> for me? Those dirty creatures can be found near <j(892, 3273)>.")
	MisBeginCondition(LvCheck, ">", 9 )
	MisBeginCondition(NoMission, 639)
	MisBeginCondition(NoRecord, 639)
	MisBeginAction(AddMission, 639)
	MisBeginAction(AddTrigger, 6391, TE_KILL, 62, 10 )
	MisCancelAction(ClearMission, 639)

	MisNeed(MIS_NEED_KILL, 62, 10, 10, 10)
	
	MisHelpTalk("<t>They should not pose any problem for you. Please complete it fast.")
	MisResultTalk("<t>Thank you! Thank you so much!")
	MisResultCondition(NoRecord, 639)
	MisResultCondition(HasMission, 639)
	MisResultCondition(HasFlag, 639, 19 )
	MisResultAction(ClearMission, 639)
	MisResultAction(SetRecord, 639)
	MisResultAction(AddExp, 120, 120)
	MisResultAction(AddMoney,149,149)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 62 )	
	TriggerAction( 1, AddNextFlag, 639, 10, 10 )
	RegCurTrigger( 6391 )


-----------------------------------
	DefineMission( 640, "Hopping Lizard Secretion", 640 )
	
	MisBeginTalk( "<t>I discovered that the weird smells from the desert are from the <rHopping Lizard>. However it is not their droppings that emitted the odour.<n><t>Currently as of now, I still do not understand the entire issue, can you please go to where the <rHopping Lizard> appears and get me 5 bottles of <yHopping Lizard Secretion> so that I can study it.<n><t>They can be found at <j(892,3273)>.")
	MisBeginCondition(LvCheck, ">", 10 )
	MisBeginCondition(NoMission, 640)
	MisBeginCondition(NoRecord, 640)
	MisBeginAction(AddMission, 640)
	MisBeginAction(AddTrigger, 6401, TE_GETITEM, 4095, 5 )
	MisCancelAction(ClearMission, 640)

	MisNeed(MIS_NEED_ITEM, 4095, 5, 10, 5)
	
	MisHelpTalk("<t>Have you collected 5 bottles of <yHopping Lizard Secretion>?")
	MisResultTalk("<t>Oh my! It's the mating season for <rHopping Lizard>. That explains the smell.")
	MisResultCondition(NoRecord, 640)
	MisResultCondition(HasMission, 640)
	MisResultCondition(HasItem, 4095, 5)
	MisResultAction(TakeItem, 4095, 5 )
	MisResultAction(ClearMission, 640)
	MisResultAction(SetRecord, 640)
	MisResultAction(AddExp, 150, 150)
	MisResultAction(AddMoney,318,318)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4095 )	
	TriggerAction( 1, AddNextFlag, 640, 10, 5 )
	RegCurTrigger( 6401 )


-----------------------------------
	DefineMission( 641, "Expel the Wolves", 641 )
	
	MisBeginTalk( "<t>I am terrified of <rWolf Cubs>, they often appear in packs in the desert and that leaves me shivering with fear.<n><t>Can you please think of a way to reduce the <rWolf Cubs> population? Try killing 10 <rWolf Cubs>.<n><t>Heh, lets just do it this way, so will you help me hunt 10 <rWolf Cubs>! They usually appear at <j(687, 3093)>.")
	MisBeginCondition(LvCheck, ">", 10 )
	MisBeginCondition(NoMission, 641)
	MisBeginCondition(NoRecord, 641)
	MisBeginAction(AddMission, 641)
	MisBeginAction(AddTrigger, 6411, TE_KILL, 100, 10 )
	MisCancelAction(ClearMission, 641)

	MisNeed(MIS_NEED_KILL, 100, 10, 10, 10)
	
	MisHelpTalk("<t>Have you killed 10 <rWolf Cubs> yet? Please try harder!")
	MisResultTalk("<t>Thank you so much! Reducing their population will make traveling safer.")
	MisResultCondition(NoRecord, 641)
	MisResultCondition(HasMission, 641)
	MisResultCondition(HasFlag, 641, 19 )
	MisResultAction(ClearMission, 641)
	MisResultAction(SetRecord, 641)
	MisResultAction(AddExp, 150, 150)
	MisResultAction(AddMoney,159,159)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 100 )	
	TriggerAction( 1, AddNextFlag, 641, 10, 10 )
	RegCurTrigger( 6411 )

-----------------------------------
	DefineMission( 642, "Wolves Massacre", 642 )
	
	MisBeginTalk( "<t>Ah, I have a friend <bChang> who is willing to spend large amounts of money to collect weird body parts. He self proclaims as a living arts collector, something which I don't agree with.<n><t>This time round he is paying a high price for 5 <yWolf Cub Claws> from <rWolf Cub>. What a cruel job!<n><t>I can't bring myself to do this. However, he is paying a very high price for it so if you are interested, I will hand over the job to you.<n><t>These pitiful animals can be found at <j(687, 3093)>.")
	MisBeginCondition(LvCheck, ">", 11 )
	MisBeginCondition(LvCheck, ">", 11 )
	MisBeginCondition(NoMission, 642)
	MisBeginCondition(NoRecord, 642)
	MisBeginAction(AddMission, 642)
	MisBeginAction(AddTrigger, 6421, TE_GETITEM, 4096, 5 )
	MisCancelAction(ClearMission, 642)

	MisNeed(MIS_NEED_ITEM, 4096, 5, 10, 5)
	
	MisHelpTalk("<t>You need to collect 5 <yWolf Cub Claws>.")
	MisResultTalk("<t>Have you done so already? Here is your much deserved reward.")
	MisResultCondition(NoRecord, 642)
	MisResultCondition(HasMission, 642)
	MisResultCondition(HasItem, 4096, 5)
	MisResultAction(TakeItem, 4096, 5 )
	MisResultAction(ClearMission, 642)
	MisResultAction(SetRecord, 642)
	MisResultAction(AddExp, 190, 190)
	MisResultAction(AddMoney,339,339)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4096 )	
	TriggerAction( 1, AddNextFlag, 642, 10, 5 )
	RegCurTrigger( 6421 )

-----------------------------------
	DefineMission( 643, "Clearance of Killer Cactus", 643 )
	
	MisBeginTalk( "<t>Abomination! This is definitely an abomination.<n><t>As a merchant I travel a lot, when I first arrived here I saw so many walking <rKiller Cactus>.<n><t>This is too weird! I can't believe that plants can actually walk! Oh my god, please kill 10 <rKiller Cactus> so that I can have a peace of mind.<n><t>They can be found at <j(884, 3156)>.")
	MisBeginCondition(LvCheck, ">", 12 )
	MisBeginCondition(NoMission, 643)
	MisBeginCondition(NoRecord, 643)
	MisBeginAction(AddMission, 643)
	MisBeginAction(AddTrigger, 6431, TE_KILL, 43, 10 )
	MisCancelAction(ClearMission, 643)

	MisNeed(MIS_NEED_KILL, 43, 10, 10, 10)
	
	MisHelpTalk("<t>Have you gotten rid of 10 <rKiller Cactuses>?")
	MisResultTalk("<t>Thank you! I feel so much better!")
	MisResultCondition(NoRecord, 643)
	MisResultCondition(HasMission, 643)
	MisResultCondition(HasFlag, 643, 19 )
	MisResultAction(ClearMission, 643)
	MisResultAction(SetRecord, 643)
	MisResultAction(AddExp, 240, 240)
	MisResultAction(AddMoney,180,180)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 43 )	
	TriggerAction( 1, AddNextFlag, 643, 10, 10 )
	RegCurTrigger( 6431 )



-----------------------------------
	DefineMission( 644, "Bitter Medication", 644 )
	
	MisBeginTalk( "<t>The dry air of the desert makes people uncomfortable. I do have a secret recipe that when used, it can make travellers feel a tad bit better.<n><t>However this remedy requires <yLarge Cactus Tuber> from <rKiller Cactus>.<n><t>I don't have the capability to defeat these monsters so can you please obtain 5 <yLarge Cactus Tubers> for me? These monsters are often found at <j(884, 3156)> frolicking under the sun.")
	MisBeginCondition(LvCheck, ">", 13 )
	MisBeginCondition(NoMission, 644)
	MisBeginCondition(NoRecord, 644)
	MisBeginAction(AddMission, 644)
	MisBeginAction(AddTrigger, 6441, TE_GETITEM, 4421, 5 )
	MisCancelAction(ClearMission, 644)

	MisNeed(MIS_NEED_ITEM, 4421, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yLarge Cactus Tubers> to make medicine.")
	MisResultTalk("<t>Thank you! Now we can help those tourists!")
	MisResultCondition(NoRecord, 644)
	MisResultCondition(HasMission, 644)
	MisResultCondition(HasItem, 4421, 5)
	MisResultAction(TakeItem, 4421, 5 )
	MisResultAction(ClearMission, 644)
	MisResultAction(SetRecord, 644)
	MisResultAction(AddExp, 290, 290)
	MisResultAction(AddMoney,384,384)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4421 )	
	TriggerAction( 1, AddNextFlag, 644, 10, 5 )
	RegCurTrigger( 6441 )


-----------------------------------
	DefineMission( 645, "Cactus Water Pouch", 645 )
	
	MisBeginTalk( "<t>Hey, look what I discovered! The secret of <rKiller Cactus>'s ability to survive in the desert without drinking for long periods of time!<n><t>It because they have a <yCactus Water Pouch> which has very good elastcity to contain water which helps the <rKiller Cactus> through the periods of dry spells.<n><t>However i don't fully understand the materials which the bag is made of, can you help me get 5 <yCactus Water Pouch> back? <rKiller Cactus> can be found at <j(884,3156)>.")
	MisBeginCondition(LvCheck, ">", 13 )
	MisBeginCondition(NoMission, 645)
	MisBeginCondition(HasRecord, 644)
	MisBeginCondition(NoRecord, 645)
	MisBeginAction(AddMission, 645)
	MisBeginAction(AddTrigger, 6451, TE_GETITEM, 4097, 5 )
	MisCancelAction(ClearMission, 645)

	MisNeed(MIS_NEED_ITEM, 4097, 5, 10, 5)
	
	MisHelpTalk("<t>When can you get me 5 <yCactus Water Pouches>?")
	MisResultTalk("<t>This research might be useful to mankind!")
	MisResultCondition(NoRecord, 645)
	MisResultCondition(HasMission, 645)
	MisResultCondition(HasItem, 4097, 5)
	MisResultAction(TakeItem, 4097, 5 )
	MisResultAction(ClearMission, 645)
	MisResultAction(SetRecord, 645)
	MisResultAction(AddExp, 290, 290)
	MisResultAction(AddMoney,384,384)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4097 )	
	TriggerAction( 1, AddNextFlag, 645, 10, 5 )
	RegCurTrigger( 6451 )

----------------------------------
	DefineMission( 646, "Rolling Melons", 646 )
	
	MisBeginTalk( "<t>I wonder who planted so many <rGigantic Melons>. Now hat they are overrunning the desert, what a sight for sore eyes.<n><t>Can you please get rid of 10 <rGigantic Melons> at <j(687, 3093)>?")
	MisBeginCondition(LvCheck, ">", 14 )
	MisBeginCondition(NoMission, 646)
	MisBeginCondition(NoRecord, 646)
	MisBeginAction(AddMission, 646)
	MisBeginAction(AddTrigger, 6461, TE_KILL, 294, 10 )
	MisCancelAction(ClearMission, 646)

	MisNeed(MIS_NEED_KILL, 294, 10, 10, 10)
	
	MisHelpTalk("<t>Have you gotten rid of 10 <rGigantic Melons>?")
	MisResultTalk("<t>Thank you! You are skillful indeed!")
	MisResultCondition(NoRecord, 646)
	MisResultCondition(HasMission, 646)
	MisResultCondition(HasFlag, 646, 19 )
	MisResultAction(ClearMission, 646)
	MisResultAction(SetRecord, 646)
	MisResultAction(AddExp, 360, 360)
	MisResultAction(AddMoney,204,204)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 294 )	
	TriggerAction( 1, AddNextFlag, 646, 10, 10 )
	RegCurTrigger( 6461 )

-----------------------------------
	DefineMission( 647, "Bitter Fruit", 647 )
	
	MisBeginTalk( "<t>The <yBitter Fruit> from <rGigantic Melons> is bitter and extremely juicy.<n><t>Recently, using bitter fruit as a beverage seems to be the current trend for youngsters.<n><t> Everything I put up is sold immediately and I have run out of stock. Can you please get me 5 <yBitter Fruits>?<n><t>You can get them from the <rGigantic Melons> at <j(687, 3093)>.")
	MisBeginCondition(LvCheck, ">", 14 )
	MisBeginCondition(NoMission, 647)
	MisBeginCondition(NoRecord, 647)
	MisBeginAction(AddMission, 647)
	MisBeginAction(AddTrigger, 6471, TE_GETITEM, 4475, 5 )
	MisCancelAction(ClearMission, 647)

	MisNeed(MIS_NEED_ITEM, 4475, 5, 10, 5)
	
	MisHelpTalk("<t>Have you collected 5 <Bitter Fruits>?")
	MisResultTalk("<t>Thank you! These are your rewards!")
	MisResultCondition(NoRecord, 647)
	MisResultCondition(HasMission, 647)
	MisResultCondition(HasItem, 4475, 5)
	MisResultAction(TakeItem, 4475, 5 )
	MisResultAction(ClearMission, 647)
	MisResultAction(SetRecord, 647)
	MisResultAction(AddExp, 360, 360)
	MisResultAction(AddMoney,408,408)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4475 )	
	TriggerAction( 1, AddNextFlag, 647, 10, 5 )
	RegCurTrigger( 6471 )

----------------------------------
	DefineMission( 648, "Legend of Phantom Tree", 648 )
	
	MisBeginTalk( "<t>Rumor has it that <rPhantom Tree> symbolizes impending doom.<n><t>Although it might not be true, these <rPhantom Trees> still reek of evil. Please chop down 10 <rPhantom Trees> at <j(885, 3027)> and put the villagers to ease.")
	MisBeginCondition(LvCheck, ">", 15 )
	MisBeginCondition(NoMission, 648)
	MisBeginCondition(NoRecord, 648)
	MisBeginAction(AddMission, 648)
	MisBeginAction(AddTrigger, 6481, TE_KILL, 203, 10 )
	MisCancelAction(ClearMission, 648)

	MisNeed(MIS_NEED_KILL, 203, 10, 10, 10)
	
	MisHelpTalk("<t>Chop down 10 <rPhantom Trees>!")
	MisResultTalk("<t>Well done!")
	MisResultCondition(NoRecord, 648)
	MisResultCondition(HasMission, 648)
	MisResultCondition(HasFlag, 648, 19 )
	MisResultAction(ClearMission, 648)
	MisResultAction(SetRecord, 648)
	MisResultAction(AddExp, 450, 450)
	MisResultAction(AddMoney,216,216)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 203 )	
	TriggerAction( 1, AddNextFlag, 648, 10, 10 )
	RegCurTrigger( 6481 )


-----------------------------------
	DefineMission( 649, "Treant Holiday", 649 )
	
	MisBeginTalk( "<t>When the roots of the <rPhantom Tree> is near a water source, it will expand and absorb all of the water it can hold, so the tree can survive in the desert. I am curious about this ability.<n><t>Can you find 5 <yPhantom Tree Roots> for my research? They normally grow at <j(885, 3027)>.")
	MisBeginCondition(LvCheck, ">", 16 )
	MisBeginCondition(NoMission, 649)
	MisBeginCondition(NoRecord, 649)
	MisBeginAction(AddMission, 649)
	MisBeginAction(AddTrigger, 6491, TE_GETITEM, 4098, 5 )
	MisCancelAction(ClearMission, 649)

	MisNeed(MIS_NEED_ITEM, 4098, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yPhantom Tree Roots> for research.")
	MisResultTalk("<t>This Phantom Tree Root is too complicated! I cannot analyze it!")
	MisResultCondition(NoRecord, 649)
	MisResultCondition(HasMission, 649)
	MisResultCondition(HasItem, 4098, 5)
	MisResultAction(TakeItem, 4098, 5 )
	MisResultAction(ClearMission, 649)
	MisResultAction(SetRecord, 649)
	MisResultAction(AddExp, 550, 550)
	MisResultAction(AddMoney,459,459)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4098 )	
	TriggerAction( 1, AddNextFlag, 649, 10, 5 )
	RegCurTrigger( 6491 )

-----------------------------------
	DefineMission( 650, "Unparallel Evil", 650 )
	
	MisBeginTalk( "<ts>obsobI am only a helpless woman. Yet <rSand Brigands> robbed me of my family treasure.<n><t>That <yJade Bangle> is a gift from my deceased grandma. Please get it back for me! It's very important to me! They have an encampment at <j(716, 3290)>.<n><t>It might be hidden inside one of their barrel.")
	MisBeginCondition(LvCheck, ">", 17 )
	MisBeginCondition(NoMission, 650)
	MisBeginCondition(NoRecord, 650)
	MisBeginAction(AddMission, 650)
	MisBeginAction(AddTrigger, 6501, TE_GETITEM, 4099, 1 )
	MisCancelAction(ClearMission, 650)

	MisNeed(MIS_NEED_ITEM, 4099, 1, 10, 1)
	
	MisHelpTalk("<ts>ob..sobHave you found my <yJade Bangle>?")
	MisResultTalk("<t>Oh dear! I don't know how I can ever repay you!")
	MisResultCondition(NoRecord, 650)
	MisResultCondition(HasMission, 650)
	MisResultCondition(HasItem, 4099, 1)
	MisResultAction(TakeItem, 4099, 1 )
	MisResultAction(ClearMission, 650)
	MisResultAction(SetRecord, 650)
	MisResultAction(AddExp, 650, 650)
	MisResultAction(AddMoney,242,242)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4099 )	
	TriggerAction( 1, AddNextFlag, 650, 10, 1 )
	RegCurTrigger( 6501 )


----------------------------------
	DefineMission( 651, "Flush out the Bandits", 651 )
	
	MisBeginTalk( "<t><rSand Brigands> are getting out of hand nowadays. Not only did they rob merchants, they also robbed helpless women and children.<n><t>The villagers have come up with a reward to get rid of them. Will you help us to hunt down 10 <rSand Brigands> at <j(716, 3290)>?")
	MisBeginCondition(LvCheck, ">", 18 )
	MisBeginCondition(NoMission, 651)
	MisBeginCondition(NoRecord, 651)
	MisBeginAction(AddMission, 651)
	MisBeginAction(AddTrigger, 6511, TE_KILL, 131, 10 )
	MisCancelAction(ClearMission, 651)

	MisNeed(MIS_NEED_KILL, 131, 10, 10, 10)
	
	MisHelpTalk("<t>Kill <r10 Sand Brigands> to claim the bounty reward!")
	MisResultTalk("<t>Great! Here is your reward.")
	MisResultCondition(NoRecord, 651)
	MisResultCondition(HasMission, 651)
	MisResultCondition(HasFlag, 651, 19 )
	MisResultAction(ClearMission, 651)
	MisResultAction(SetRecord, 651)
	MisResultAction(AddExp, 750, 750)
	MisResultAction(AddMoney,256,256)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 131 )	
	TriggerAction( 1, AddNextFlag, 651, 10, 10 )
	RegCurTrigger( 6511 )

----------------------------------
	DefineMission( 652, "Vicious Hungry Wolves", 652 )
	
	MisBeginTalk( "<t>I am recruiting warriors to fight against <rStarving Wolves>.<n><t>They are keeping travelers away!<n><t>If you can kill 10 <rStarving Wolves> at <j(718, 2938)>, I will reward you accordingly.")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 652)
	MisBeginCondition(NoRecord, 652)
	MisBeginAction(AddMission, 652)
	MisBeginAction(AddTrigger, 6521, TE_KILL, 101, 10 )
	MisCancelAction(ClearMission, 652)

	MisNeed(MIS_NEED_KILL, 101, 10, 10, 10)
	
	MisHelpTalk("<t>I will reward anybody who can kill 10 <rStarving Wolves>.")
	MisResultTalk("<t>Great! This is your reward.")
	MisResultCondition(NoRecord, 652)
	MisResultCondition(HasMission, 652)
	MisResultCondition(HasFlag, 652, 19 )
	MisResultAction(ClearMission, 652)
	MisResultAction(SetRecord, 652)
	MisResultAction(AddExp, 880, 880)
	MisResultAction(AddMoney,270,270)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 101 )	
	TriggerAction( 1, AddNextFlag, 652, 10, 10 )
	RegCurTrigger( 6521 )

----------------------------------
	DefineMission( 653, "Rebellion of Sandy Tortoise", 653 )
	
	MisBeginTalk( "<t>I don't know what happened to the <rSandy Tortoises>, they are on a rampage and are attacking anything they see!<n><t>Can you help me kill 10 <rSandy Tortoises> at <j(1197, 3270)>?")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 653)
	MisBeginCondition(NoRecord, 653)
	MisBeginAction(AddMission, 653)
	MisBeginAction(AddTrigger, 6531, TE_KILL, 134, 10 )
	MisCancelAction(ClearMission, 653)

	MisNeed(MIS_NEED_KILL, 134, 10, 10, 10)
	
	MisHelpTalk("<t>Kill 10 <rSandy Tortoise> please!")
	MisResultTalk("<t>Well done!")
	MisResultCondition(NoRecord, 653)
	MisResultCondition(HasMission, 653)
	MisResultCondition(HasFlag, 653, 19 )
	MisResultAction(ClearMission, 653)
	MisResultAction(SetRecord, 653)
	MisResultAction(AddExp, 880, 880)
	MisResultAction(AddMoney,270,270)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 134 )	
	TriggerAction( 1, AddNextFlag, 653, 10, 10 )
	RegCurTrigger( 6531 )

-----------------------------------
	DefineMission( 654, "Moving Garden", 654 )
	
	MisBeginTalk( "<t><rSandy Tortoise> is a slow but huge creature. Their hard shells provide a shelter for many plants in this desert.<n><t>I need a type of seed that can be found in the shell. Can you collect 5 <yDesert Seeds> from <j(1197, 3270)> for me?")
	MisBeginCondition(LvCheck, ">", 20 )
	MisBeginCondition(NoMission, 654)
	MisBeginCondition(NoRecord, 654)
	MisBeginAction(AddMission, 654)
	MisBeginAction(AddTrigger, 6541, TE_GETITEM, 4466, 5 )
	MisCancelAction(ClearMission, 654)

	MisNeed(MIS_NEED_ITEM, 4466, 5, 10, 5)
	
	MisHelpTalk("<t>Please hurry! I need 5 <yDesert Seeds>.")
	MisResultTalk("<t>Ya! These are the seeds I am looking for!")
	MisResultCondition(NoRecord, 654)
	MisResultCondition(HasMission, 654)
	MisResultCondition(HasItem, 4466, 5)
	MisResultAction(TakeItem, 4466, 5 )
	MisResultAction(ClearMission, 654)
	MisResultAction(SetRecord, 654)
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney,571,571)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4466 )	
	TriggerAction( 1, AddNextFlag, 654, 10, 5 )
	RegCurTrigger( 6541 )

----------------------------------
	DefineMission( 655, "Preparation", 655 )
	
	MisBeginTalk( "<t>I am preparing to send a caravan to <pArgent City> for trade.<n><t>However, I have to get pass a Sand Bandit camp midway. I am afraid that they might rob me.<n><t>Can you get rid of 10 <rSand Bandits> at <j(1339, 3300)> for me?")
	MisBeginCondition(LvCheck, ">", 21 )
	MisBeginCondition(NoMission, 655)
	MisBeginCondition(NoRecord, 655)
	MisBeginAction(AddMission, 655)
	MisBeginAction(AddTrigger, 6551, TE_KILL, 45, 10 )
	MisCancelAction(ClearMission, 655)

	MisNeed(MIS_NEED_KILL, 45, 10, 10, 10)
	
	MisHelpTalk("<t>Have you gone to the <rSand Bandits> camp yet?")
	MisResultTalk("<t>Thank you! Now I can get to Argent City safely to do some trading.")
	MisResultCondition(NoRecord, 655)
	MisResultCondition(HasMission, 655)
	MisResultCondition(HasFlag, 655, 19 )
	MisResultAction(ClearMission, 655)
	MisResultAction(SetRecord, 655)
	MisResultAction(AddExp, 1200, 1200)
	MisResultAction(AddMoney,300,300)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 45 )	
	TriggerAction( 1, AddNextFlag, 655, 10, 10 )
	RegCurTrigger( 6551 )

-----------------------------------
	DefineMission( 656, "Mushroom Soup", 656 )
	
	MisBeginTalk( "<t>Ah...Everytime I see these <rSandy Shrooms> jumping around, my mind will instinctively conjure up an image of boiling mushroom soup!<n><t>I love that thick sweet smelling aroma of that soup! Please go and obtain 5 <yUmbrella Mushrooms> from the <rSandy Shrooms> and let me have a taste!<n><t><rSandy Shrooms> can be found at <j(1334, 3438)>.")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(NoMission, 656)
	MisBeginCondition(NoRecord, 656)
	MisBeginAction(AddMission, 656)
	MisBeginAction(AddTrigger, 6561, TE_GETITEM, 4476, 5 )
	MisCancelAction(ClearMission, 656)

	MisNeed(MIS_NEED_ITEM, 4476, 5, 10, 5)
	
	MisHelpTalk("<t>This type of <yUmbrella Mushroom> can only be found on <rSandy Shroom>.")
	MisResultTalk("<t>Cream of mushroom soup made from <yUmbrella Mushrooms> is delicious!")
	MisResultCondition(NoRecord, 656)
	MisResultCondition(HasMission, 656)
	MisResultCondition(HasItem, 4476, 5)
	MisResultAction(TakeItem, 4476, 5 )
	MisResultAction(ClearMission, 656)
	MisResultAction(SetRecord, 656)
	MisResultAction(AddExp, 1400, 1400)
	MisResultAction(AddMoney,632,632)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4476 )	
	TriggerAction( 1, AddNextFlag, 656, 10, 5 )
	RegCurTrigger( 6561 )

-----------------------------------
	DefineMission( 657, "The Runaway Shroom", 657 )
	
	MisBeginTalk( "<t>I love doing my own gardening.<n><t>For the past 2 days, while I was strolling in my gardens, I felt that something was missing.<n><t>Then I realised it was my <rSandy Shrooms>! I have no idea where they ran to but I can't have my garden without any Sandy Shrooms!<n><t>Can you please go to where the <rSandy Shrooms> are and get me 5 <yDesert Spore>? These <rSandy Shrooms> are usually found at <j(1334, 3438)>.<n><t>These spores are rather easy to find. Look for the white patches within the bushes.")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(NoMission, 657)
	MisBeginCondition(NoRecord, 657)
	MisBeginAction(AddMission, 657)
	MisBeginAction(AddTrigger, 6571, TE_GETITEM, 4100, 5 )
	MisCancelAction(ClearMission, 657)

	MisNeed(MIS_NEED_ITEM, 4100, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yDesert Spores> urgently!")
	MisResultTalk("<t>This is great!<n><ts>oon I will have <rSandy Shrooms> in my garden again!")
	MisResultCondition(NoRecord, 657)
	MisResultCondition(HasMission, 657)
	MisResultCondition(HasItem, 4100, 5)
	MisResultAction(TakeItem, 4100, 5 )
	MisResultAction(ClearMission, 657)
	MisResultAction(SetRecord, 657)
	MisResultAction(AddExp, 1400, 1400)
	MisResultAction(AddMoney,316,316)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4100 )	
	TriggerAction( 1, AddNextFlag, 657, 10, 5 )
	RegCurTrigger( 6571 )

-----------------------------------
	DefineMission( 658, "Magical Usage for Mud", 658 )
	
	MisBeginTalk( "<t>Hey, my friend, you come at the right time! The desert's yearly mud slinging contest is about to start, please help me aquire 5 <yPolluted Mud> from the <rMudman> at <j(1438, 3414)>.The result of the mud contest means a lot to me for this is most important event in the desert!<n><t>I will definitely prepare enough mud for battle.")
	MisBeginCondition(LvCheck, ">", 23 )
	MisBeginCondition(NoMission, 658)
	MisBeginCondition(NoRecord, 658)
	MisBeginAction(AddMission, 658)
	MisBeginAction(AddTrigger, 6581, TE_GETITEM, 4436, 5 )
	MisCancelAction(ClearMission, 658)

	MisNeed(MIS_NEED_ITEM, 4436, 5, 10, 5)
	
	MisHelpTalk("<t>It is not enough! I need 5 pieces of <yPolluted Mud>!")
	MisResultTalk("<t>Goddess bless you!")
	MisResultCondition(NoRecord, 658)
	MisResultCondition(HasMission, 658)
	MisResultCondition(HasItem, 4436, 5)
	MisResultAction(TakeItem, 4436, 5 )
	MisResultAction(ClearMission, 658)
	MisResultAction(SetRecord, 658)
	MisResultAction(AddExp, 1600, 1600)
	MisResultAction(AddMoney,664,664)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4436 )	
	TriggerAction( 1, AddNextFlag, 658, 10, 5 )
	RegCurTrigger( 6581 )

----------------------------------
	DefineMission( 659, "Mudman Sleepy Head", 659 )
	
	MisBeginTalk( "<t>Grr, my whole body is full of mud, all because of <rMudman>'s range attacks!<n><t>These despicable <rMudman> not only occupied the limited water sources in the desert, they also abuse their status and power, attacking anyone who comes close to the water source.<n><ts>uch corrupted beings! Will you go and teach these Mudman a lesson? Hunt 10 <rMudman> at <j(1438, 3413)>.")
	MisBeginCondition(LvCheck, ">", 23 )
	MisBeginCondition(NoMission, 659)
	MisBeginCondition(NoRecord, 659)
	MisBeginAction(AddMission, 659)
	MisBeginAction(AddTrigger, 6591, TE_KILL, 251, 10 )
	MisCancelAction(ClearMission, 659)

	MisNeed(MIS_NEED_KILL, 251, 10, 10, 10)
	
	MisHelpTalk("<t>Hurry up and punish those <rMudman>!")
	MisResultTalk("<t>Well done!")
	MisResultCondition(NoRecord, 659)
	MisResultCondition(HasMission, 659)
	MisResultCondition(HasFlag, 659, 19 )
	MisResultAction(ClearMission, 659)
	MisResultAction(SetRecord, 659)
	MisResultAction(AddExp, 1600, 1600)
	MisResultAction(AddMoney,332,332)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 251 )	
	TriggerAction( 1, AddNextFlag, 659, 10, 10 )
	RegCurTrigger( 6591 )

----------------------------------
	DefineMission( 660, "Dangerous Raiders", 660 )
	
	MisBeginTalk( "<t>The <rSand Raiders> have ruled the desert for many years and have yet to meet someone of their match.<n><t>I plead with you, my brave warrior! Remove this threat for us! Defeat 10 <rSand Raiders> at <j(1027, 3011)> please!")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 660)
	MisBeginCondition(NoRecord, 660)
	MisBeginAction(AddMission, 660)
	MisBeginAction(AddTrigger, 6601, TE_KILL, 49, 10 )
	MisCancelAction(ClearMission, 660)

	MisNeed(MIS_NEED_KILL, 49, 10, 10, 10)
	
	MisHelpTalk("<t>To defeat 10 <rSand Raiders> is no easy task but I have faith in you.")
	MisResultTalk("<t>Brave adventurer! Welcome back!")
	MisResultCondition(NoRecord, 660)
	MisResultCondition(HasMission, 660)
	MisResultCondition(HasFlag, 660, 19 )
	MisResultAction(ClearMission, 660)
	MisResultAction(SetRecord, 660)
	MisResultAction(AddExp, 1800, 1800)
	MisResultAction(AddMoney,348,348)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 49 )	
	TriggerAction( 1, AddNextFlag, 660, 10, 10 )
	RegCurTrigger( 6601 )

----------------------------------
	DefineMission( 661, "Spider Clearance", 661 )
	
	MisBeginTalk( "<t>Oh no! While I was playing along the beach just now, a huge <rMan Eating Spider> came crawling towards me!<n><t>Please save me from them and defeat 10 <rMan Eating Spiders> at <j(1093, 2948)>!")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 661)
	MisBeginCondition(NoRecord, 661)
	MisBeginAction(AddMission, 661)
	MisBeginAction(AddTrigger, 6611, TE_KILL, 210, 10 )
	MisCancelAction(ClearMission, 661)

	MisNeed(MIS_NEED_KILL, 210, 10, 10, 10)
	
	MisHelpTalk("<t>Oh my god! The <rMan Eating Spider> is really huge!")
	MisResultTalk("<t>You are great! Even such a huge <rMan Eating Spider> is not your match!")
	MisResultCondition(NoRecord, 661)
	MisResultCondition(HasMission, 661)
	MisResultCondition(HasFlag, 661, 19 )
	MisResultAction(ClearMission, 661)
	MisResultAction(SetRecord, 661)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,365,365)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 210 )	
	TriggerAction( 1, AddNextFlag, 661, 10, 10 )
	RegCurTrigger( 6611 )

-----------------------------------
	DefineMission( 662, "Walking Wolf", 662 )
	
	MisBeginTalk( "<t>I am curious why the <rFeral Wolf> at <j(1473, 3295)> can move on two legs like a human. Also, their fur is red.<n><t>I believed they are an evolved species. Can you help me find <y5 Huge Wolf Claws> for my research?")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 662)
	MisBeginCondition(NoRecord, 662)
	MisBeginAction(AddMission, 662)
	MisBeginAction(AddTrigger, 6621, TE_GETITEM, 4439, 5 )
	MisCancelAction(ClearMission, 662)

	MisNeed(MIS_NEED_ITEM, 4439, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yHuge Wolf Claws> for research.")
	MisResultTalk("<t>AhI still cannot find the reason behind the change!")
	MisResultCondition(NoRecord, 662)
	MisResultCondition(HasMission, 662)
	MisResultCondition(HasItem, 4439, 5)
	MisResultAction(TakeItem, 4439, 5 )
	MisResultAction(ClearMission, 662)
	MisResultAction(SetRecord, 662)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,730,730)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4439 )	
	TriggerAction( 1, AddNextFlag, 662, 10, 5 )
	RegCurTrigger( 6621 )

-----------------------------------
	DefineMission( 663, "Golden Spider", 663 )
	
	MisBeginTalk( "<t>Dear friend! You've come at the right moment! <rMan Eating Spiders> snatched away my <yGold Pouch>!<n><t>It seems that they have a love for gold. Please help me retrieve my <yGold Pouch> from them!<n><t>They must have hide the pouch in the money box near <j(1093, 2948)>.")
	MisBeginCondition(LvCheck, ">", 26 )
	MisBeginCondition(NoMission, 663)
	MisBeginCondition(NoRecord, 663)
	MisBeginAction(AddMission, 663)
	MisBeginAction(AddTrigger, 6631, TE_GETITEM, 4101, 1 )
	MisCancelAction(ClearMission, 663)

	MisNeed(MIS_NEED_ITEM, 4101, 1, 10, 1)
	
	MisHelpTalk("<t>Have you recovered the <yGold Pouch> stolen by those <rMan Eating Spiders>?")
	MisResultTalk("<t>At last, I've gotten back my stolen Gold Pouch! Thanks!")
	MisResultCondition(NoRecord, 663)
	MisResultCondition(HasMission, 663)
	MisResultCondition(HasItem, 4101, 1)
	MisResultAction(TakeItem, 4101, 1 )
	MisResultAction(ClearMission, 663)
	MisResultAction(SetRecord, 663)
	MisResultAction(AddExp, 2400, 2400)
	MisResultAction(AddMoney,382,382)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4101 )	
	TriggerAction( 1, AddNextFlag, 663, 10, 1 )
	RegCurTrigger( 6631 )

-----------------------------------
	DefineMission( 664, "Master Key", 664 )
	
	MisBeginTalk( "<ts>hh! Please be quiet about this.<n><t>My friend is going to elope with Nana due to her parents disapproval of their marriage.<n><t>I wish to give them my blessings with a gift. I believe that some Master Keys will be handy in their escape. Please help me find 2 <yMaster Keys> from <rCavalier> at <j(1255, 2974)>!")
	MisBeginCondition(LvCheck, ">", 26 )
	MisBeginCondition(NoMission, 664)
	MisBeginCondition(NoRecord, 664)
	MisBeginAction(AddMission, 664)
	MisBeginAction(AddTrigger, 6641, TE_GETITEM, 4478, 2 )
	MisCancelAction(ClearMission, 664)

	MisNeed(MIS_NEED_ITEM, 4478, 2, 10, 2)
	
	MisHelpTalk("<t>Have you found 2 <yMaster Keys>? Time is precious!")
	MisResultTalk("<t>Thank you my friend!")
	MisResultCondition(NoRecord, 664)
	MisResultCondition(HasMission, 664)
	MisResultCondition(HasItem, 4478, 2)
	MisResultAction(TakeItem, 4478, 2 )
	MisResultAction(ClearMission, 664)
	MisResultAction(SetRecord, 664)
	MisResultAction(AddExp, 2400, 2400)
	MisResultAction(AddMoney,765,765)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4478 )	
	TriggerAction( 1, AddNextFlag, 664, 10, 2 )
	RegCurTrigger( 6641 )

----------------------------------
	DefineMission( 665, "Capture the Head", 665 )
	
	MisBeginTalk( "<t>As the saying goes, nab the leader and you will get the rest.<n><t>Arresting those <rSand Bandits> will not help much. However, if you nab the <rSand Bandit Leader> at <j(1170, 3026)>, it will throw the rest of them into confusion disarray!")
	MisBeginCondition(LvCheck, ">", 27 )
	MisBeginCondition(NoMission, 665)
	MisBeginCondition(NoRecord, 665)
	MisBeginAction(AddMission, 665)
	MisBeginAction(AddTrigger, 6651, TE_KILL, 106, 1 )
	MisCancelAction(ClearMission, 665)

	MisNeed(MIS_NEED_KILL, 106, 1, 10, 1)
	
	MisHelpTalk("<t>The <rSand Bandit Leader> is a clever and cunning man. Please be careful!")
	MisResultTalk("<t>Hehe! My idea is useful, right?")
	MisResultCondition(NoRecord, 665)
	MisResultCondition(HasMission, 665)
	MisResultCondition(HasFlag, 665, 10 )
	MisResultAction(ClearMission, 665)
	MisResultAction(SetRecord, 665)
	MisResultAction(AddExp, 2700, 2700)
	MisResultAction(AddMoney,400,400)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 106 )	
	TriggerAction( 1, AddNextFlag, 665, 10, 1 )
	RegCurTrigger( 6651 )

----------------------------------
	DefineMission( 666, "Challenge Cavalier", 666 )
	
	MisBeginTalk( "<ts>and Raiders are feared in the desert. However, <rCavaliers> are much more ferocious than them.<n><t>Prove your valor by defeating 10 <rCavaliers> at <j(1255, 2974)>!")
	MisBeginCondition(LvCheck, ">", 28 )
	MisBeginCondition(NoMission, 666)
	MisBeginCondition(NoRecord, 666)
	MisBeginAction(AddMission, 666)
	MisBeginAction(AddTrigger, 6661, TE_KILL, 200, 10 )
	MisCancelAction(ClearMission, 666)

	MisNeed(MIS_NEED_KILL, 200, 10, 10, 10)
	
	MisHelpTalk("<t>Are you up to it? Its only 10 <rCavaliers>.")
	MisResultTalk("<t>It must has been tough for you. Well done nevertheless!")
	MisResultCondition(NoRecord, 666)
	MisResultCondition(HasMission, 666)
	MisResultCondition(HasFlag, 666, 19 )
	MisResultAction(ClearMission, 666)
	MisResultAction(SetRecord, 666)
	MisResultAction(AddExp, 3000, 3000)
	MisResultAction(AddMoney,417,417)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 200 )	
	TriggerAction( 1, AddNextFlag, 666, 10, 10 )
	RegCurTrigger( 6661 )

-----------------------------------
	DefineMission( 667, "Lizard Crown", 667 )
	
	MisBeginTalk( "<t>Although I hate people who wear clothes made from the skin of <rLizard King>, I need to find myself 10 <yLizard Crowns> as the market has a demand for them now. Will you help me?<n><t>Those lizards can be found at <j(1507, 2970)>. However the drop rate are quite low so you might need more patient.")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 667)
	MisBeginCondition(NoRecord, 667)
	MisBeginAction(AddMission, 667)
	MisBeginAction(AddTrigger, 6671, TE_GETITEM, 1757, 10 )
	MisCancelAction(ClearMission, 667)

	MisNeed(MIS_NEED_ITEM, 1757, 10, 10, 10)
	
	MisHelpTalk("<t>Can you hurry about it?<n><t>Time is money!")
	MisResultTalk("<t>Thank you! This is your share of profit!")
	MisResultCondition(NoRecord, 667)
	MisResultCondition(HasMission, 667)
	MisResultCondition(HasItem, 1757, 10)
	MisResultAction(TakeItem, 1757, 10 )
	MisResultAction(ClearMission, 667)
	MisResultAction(SetRecord, 667)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,849,849)


	InitTrigger()
	TriggerCondition( 1, IsItem, 1757 )	
	TriggerAction( 1, AddNextFlag, 667, 10, 10 )
	RegCurTrigger( 6671 )

-----------------------------------
	DefineMission( 668, "Sailer Cap", 668 )
	
	MisBeginTalk( "<t>Hey! Hold on for a moment! Can you lend me hand? Yesterday I saw a cute and cuddly <rSailor Penguin> nearby. I fell in love with its <ySailor Penguin Cap> and I have been thinking about it ever since!<n><t>Can you please get me 2 <ySailor Penguin Caps> from <rSailor Penguins> at <j(995, 365)>. I really love that cap!")
	MisBeginCondition(LvCheck, ">", 9 )
	MisBeginCondition(NoMission, 668)
	MisBeginCondition(NoRecord, 668)
	MisBeginAction(AddMission, 668)
	MisBeginAction(AddTrigger, 6681, TE_GETITEM, 4102, 2 )
	MisCancelAction(ClearMission, 668)

	MisNeed(MIS_NEED_ITEM, 4102, 2, 10, 2)
	
	MisHelpTalk("<t>How I wish to wear a <ySailor Penguin Cap> now.")
	MisResultTalk("<t>Oh my! This is cute! Thank you!")
	MisResultCondition(NoRecord, 668)
	MisResultCondition(HasMission, 668)
	MisResultCondition(HasItem, 4102, 2)
	MisResultAction(TakeItem, 4102, 2 )
	MisResultAction(ClearMission, 668)
	MisResultAction(SetRecord, 668)
	MisResultAction(AddExp, 120, 120)
	MisResultAction(AddMoney,299,299)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4102 )	
	TriggerAction( 1, AddNextFlag, 668, 10, 2 )
	RegCurTrigger( 6681 )

-----------------------------------
	DefineMission( 669, "Heart of Naiad", 669 )
	
	MisBeginTalk( "<t><rNaiad> is a peaceful creature. It does not disturb travelers nor like to be disturbed.<n><t>The <yHeart of Naiad> can bring serenity to anyone who possesses it. Can you bring 2 <yHearts of Naiad> to me?<n><t>Naiad often resides near <j(1079, 518)>.")
	MisBeginCondition(LvCheck, ">", 9 )
	MisBeginCondition(NoMission, 669)
	MisBeginCondition(NoRecord, 669)
	MisBeginAction(AddMission, 669)
	MisBeginAction(AddTrigger, 6691, TE_GETITEM, 4418, 2 )
	MisCancelAction(ClearMission, 669)

	MisNeed(MIS_NEED_ITEM, 4418, 2, 10, 2)
	
	MisHelpTalk("<t>I only need 2 <yHearts of Naiad>.")
	MisResultTalk("<t>Thank you. This is the <yHeart of Naiad > I am looking for.")
	MisResultCondition(NoRecord, 669)
	MisResultCondition(HasMission, 669)
	MisResultCondition(HasItem, 4418, 2)
	MisResultAction(TakeItem, 4418, 2 )
	MisResultAction(ClearMission, 669)
	MisResultAction(SetRecord, 669)
	MisResultAction(AddExp, 120, 120)
	MisResultAction(AddMoney,299,299)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4418 )	
	TriggerAction( 1, AddNextFlag, 669, 10, 2 )
	RegCurTrigger( 6691 )

-----------------------------------
	DefineMission( 670, "Perfect Crystal", 670 )
	
	MisBeginTalk( "<t>Crystal dug from mine often have flaws on them. However, <rNaiad> is able to form a <yFlawless Crystal>. These <yFlawless Crystals> are high in demand in the black market.<n><t>Can you get 5 <yFlawless Crystals> for me? I pay dearly for them!<n><t><rNaiad> can be found near <j(1079, 518)>.")
	MisBeginCondition(LvCheck, ">", 10 )
	MisBeginCondition(NoMission, 670)
	MisBeginCondition(NoRecord, 670)
	MisBeginAction(AddMission, 670)
	MisBeginAction(AddTrigger, 6701, TE_GETITEM, 4103, 5 )
	MisCancelAction(ClearMission, 670)

	MisNeed(MIS_NEED_ITEM, 4103, 5, 10, 5)
	
	MisHelpTalk("<t><rNaiads> protect their treasures very carefully! Please be careful!")
	MisResultTalk("<t>Look at the glow of these <yFlawless Crystals>!")
	MisResultCondition(NoRecord, 670)
	MisResultCondition(HasMission, 670)
	MisResultCondition(HasItem, 4103, 5)
	MisResultAction(TakeItem, 4103, 5 )
	MisResultAction(ClearMission, 670)
	MisResultAction(SetRecord, 670)
	MisResultAction(AddExp, 150, 150)
	MisResultAction(AddMoney,318,318)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4103 )	
	TriggerAction( 1, AddNextFlag, 670, 10, 5 )
	RegCurTrigger( 6701 )

----------------------------------
	DefineMission( 671, "Suffering of Elk", 671 )
	
	MisBeginTalk( "<t>I have a strange habit. I love to listen to the groans of <rElk> in suffering. The more they groan, the happier I will be.<n><t>I will reward you greatly to kill 10 <rElks> at <j(885, 333)>.")
	MisBeginCondition(LvCheck, ">", 11 )
	MisBeginCondition(NoMission, 671)
	MisBeginCondition(NoRecord, 671)
	MisBeginAction(AddMission, 671)
	MisBeginAction(AddTrigger, 6711, TE_KILL, 266, 10 )
	MisCancelAction(ClearMission, 671)

	MisNeed(MIS_NEED_KILL, 266, 10, 10, 10)
	
	MisHelpTalk("<t>Why are you not concentrating? 10 <rElks> should be easy for you.")
	MisResultTalk("<t>Good work! This is your reward.")
	MisResultCondition(NoRecord, 671)
	MisResultCondition(HasMission, 671)
	MisResultCondition(HasFlag, 671, 19 )
	MisResultAction(ClearMission, 671)
	MisResultAction(SetRecord, 671)
	MisResultAction(AddExp, 190, 190)
	MisResultAction(AddMoney,169,169)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 266 )	
	TriggerAction( 1, AddNextFlag, 671, 10, 10 )
	RegCurTrigger( 6711 )

----------------------------------
	DefineMission( 672, "Overweight", 672 )
	
	MisBeginTalk( "<t>I will not let you hurt those Horned Penguins if I can help it.<n><t>But they have been a pest to the villagers around here. They always sneak into kitchens and steal the food there.<n><t>Please kill 10 Horned Penguins at <j(885, 333)> as a warning to them.")
	MisBeginCondition(LvCheck, ">", 12 )
	MisBeginCondition(NoMission, 672)
	MisBeginCondition(NoRecord, 672)
	MisBeginAction(AddMission, 672)
	MisBeginAction(AddTrigger, 6721, TE_KILL, 34, 10 )
	MisCancelAction(ClearMission, 672)

	MisNeed(MIS_NEED_KILL, 34, 10, 10, 10)
	
	MisHelpTalk("<t>You only need to kill 10 Horned Penguins.")
	MisResultTalk("<t>I believed that the Horned Penguins will be careful of whom they steal from next time.")
	MisResultCondition(NoRecord, 672)
	MisResultCondition(HasMission, 672)
	MisResultCondition(HasFlag, 672, 19 )
	MisResultAction(ClearMission, 672)
	MisResultAction(SetRecord, 672)
	MisResultAction(AddExp, 240, 240)
	MisResultAction(AddMoney,180,180)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 34 )	
	TriggerAction( 1, AddNextFlag, 672, 10, 10 )
	RegCurTrigger( 6721 )

-----------------------------------
	DefineMission( 673, "Think Too Much", 673 )
	
	MisBeginTalk( "<t>There is a legend on this frozen land which speaks of hanging 2 <ySquirt Handkerchiefs> by the window and within 2 years, the person you missed most will return to your side.<n><t>Due to objections by my family, I have not met my love for a long time.<n><t>Can you help me get 2 <ySquirt Handkerchiefs> so that I can wait for the day that my love returns to me? You can search for them at <j(738, 426)>.")
	MisBeginCondition(LvCheck, ">", 13 )
	MisBeginCondition(NoMission, 673)
	MisBeginCondition(NoRecord, 673)
	MisBeginAction(AddMission, 673)
	MisBeginAction(AddTrigger, 6731, TE_GETITEM, 1839, 2 )
	MisCancelAction(ClearMission, 673)

	MisNeed(MIS_NEED_ITEM, 1839, 2, 10, 2)
	
	MisHelpTalk("<t>If possible I would like to have 2 <yWhite Squirt Bandanas> now!")
	MisResultTalk("<t>Thank you! I will hang them on my window right away!")
	MisResultCondition(NoRecord, 673)
	MisResultCondition(HasMission, 673)
	MisResultCondition(HasItem, 1839, 2)
	MisResultAction(TakeItem, 1839, 2 )
	MisResultAction(ClearMission, 673)
	MisResultAction(SetRecord, 673)
	MisResultAction(AddExp, 290, 290)
	MisResultAction(AddMoney,384,384)


	InitTrigger()
	TriggerCondition( 1, IsItem, 1839 )	
	TriggerAction( 1, AddNextFlag, 673, 10, 2 )
	RegCurTrigger( 6731 )


----------------------------------
	DefineMission( 674, "Troublesome Bat", 674 )
	
	MisBeginTalk( "<t><rSnowy Bats> nearby have been emitting a ultrasonic sound wave. These sound waves make lots noises causing the nearby <rSnowmen> to go berserk. For the safety of the village, we have to get rid of 10 <rSnowy Bats>.")
	MisBeginCondition(LvCheck, ">", 14 )
	MisBeginCondition(NoMission, 674)
	MisBeginCondition(NoRecord, 674)
	MisBeginAction(AddMission, 674)
	MisBeginAction(AddTrigger, 6741, TE_KILL, 46, 10 )
	MisCancelAction(ClearMission, 674)

	MisNeed(MIS_NEED_KILL, 46, 10, 10, 10)
	
	MisHelpTalk("<t>You only need to kill 10 <rSnowy Bats>. No need to kill more than that.")
	MisResultTalk("<t>Its for our own protection that we have to resort to this.")
	MisResultCondition(NoRecord, 674)
	MisResultCondition(HasMission, 674)
	MisResultCondition(HasFlag, 674, 19 )
	MisResultAction(ClearMission, 674)
	MisResultAction(SetRecord, 674)
	MisResultAction(AddExp, 360, 360)
	MisResultAction(AddMoney,204,204)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 46 )	
	TriggerAction( 1, AddNextFlag, 674, 10, 10 )
	RegCurTrigger( 6741 )

-----------------------------------
	DefineMission( 675, "Witchcraft Ingredient", 675 )
	
	MisBeginTalk( "<t>I have been trying to master a spell that allow me to see the future.<n><t>However, I still lack of ingredients. Can you get 5 <yBat Fangs> at <j(743, 358)> for me?")
	MisBeginCondition(LvCheck, ">", 14 )
	MisBeginCondition(NoMission, 675)
	MisBeginCondition(NoRecord, 675)
	MisBeginAction(AddMission, 675)
	MisBeginAction(AddTrigger, 6751, TE_GETITEM, 4427, 5 )
	MisCancelAction(ClearMission, 675)

	MisNeed(MIS_NEED_ITEM, 4427, 5, 10, 5)
	
	MisHelpTalk("<t>When can you get me 5 <yBat Fangs>?")
	MisResultTalk("<t>Now, everything is completed.")
	MisResultCondition(NoRecord, 675)
	MisResultCondition(HasMission, 675)
	MisResultCondition(HasItem, 4427, 5)
	MisResultAction(TakeItem, 4427, 5 )
	MisResultAction(ClearMission, 675)
	MisResultAction(SetRecord, 675)
	MisResultAction(AddExp, 360, 360)
	MisResultAction(AddMoney,408,408)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4427 )	
	TriggerAction( 1, AddNextFlag, 675, 10, 5 )
	RegCurTrigger( 6751 )


----------------------------------
	DefineMission( 676, "Polution of Squity", 676 )
	
	MisBeginTalk( "<t>The <rSailor Squidys> are getting out of hand! They keep polluting our water supply with their ink! How are we supposed to drink from the lake when the water has turned black with their ink!<n><t>Can you get rid of 10 <rSailor Squidy> at <j(657, 411)> please?")
	MisBeginCondition(LvCheck, ">", 15 )
	MisBeginCondition(NoMission, 676)
	MisBeginCondition(NoRecord, 676)
	MisBeginAction(AddMission, 676)
	MisBeginAction(AddTrigger, 6761, TE_KILL, 233, 10 )
	MisCancelAction(ClearMission, 676)

	MisNeed(MIS_NEED_KILL, 233, 10, 10, 10)
	
	MisHelpTalk("<t>Have you done the deed? You need to get rid of 10 Sailor Squidys for there to be any effect.")
	MisResultTalk("<t>I think it worked out in the end.<n><t>Thanks!")
	MisResultCondition(NoRecord, 676)
	MisResultCondition(HasMission, 676)
	MisResultCondition(HasFlag, 676, 19 )
	MisResultAction(ClearMission, 676)
	MisResultAction(SetRecord, 676)
	MisResultAction(AddExp, 450, 450)
	MisResultAction(AddMoney,216,216)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 233 )	
	TriggerAction( 1, AddNextFlag, 676, 10, 10 )
	RegCurTrigger( 6761 )

-----------------------------------
	DefineMission( 677, "Squidy Cap", 677 )
	
	MisBeginTalk( "<t>You can say that I am a greedy person. I still want <ySquidy Caps> even after you have given me those <ySailor Penguin Caps>.<n><t>They just look so cute! Please help me again by finding 2 more <ySquidy Caps> at <j(657, 411)>.")
	MisBeginCondition(LvCheck, ">", 15 )
	MisBeginCondition(NoMission, 677)
	MisBeginCondition(NoRecord, 677)
	MisBeginCondition(HasRecord, 668)
	MisBeginAction(AddMission, 677)
	MisBeginAction(AddTrigger, 6771, TE_GETITEM, 1840, 2 )
	MisCancelAction(ClearMission, 677)

	MisNeed(MIS_NEED_ITEM, 1840, 2, 10, 2)
	
	MisHelpTalk("<t>Where are the 2 Squidy Caps you promised me?")
	MisResultTalk("<t>Oh! This <ySquidy Cap > is so cute!")
	MisResultCondition(NoRecord, 677)
	MisResultCondition(HasMission, 677)
	MisResultCondition(HasItem, 1840, 2)
	MisResultAction(TakeItem, 1840, 2 )
	MisResultAction(ClearMission, 677)
	MisResultAction(SetRecord, 677)
	MisResultAction(AddExp, 450, 450)
	MisResultAction(AddMoney,433,433)


	InitTrigger()
	TriggerCondition( 1, IsItem, 1840 )	
	TriggerAction( 1, AddNextFlag, 677, 10, 2 )
	RegCurTrigger( 6771 )

----------------------------------
	DefineMission( 678, "Clearance of Shrooms", 678 )
	
	MisBeginTalk( "<t>I hate all type of Shrooms! I will cook all of them if given the chance!<n><t>There is a bunch of these pesky Shrooms nearby at <j(952, 550)>. Can you get rid of 10 <rSnowy Shrooms> for me please?")
	MisBeginCondition(LvCheck, ">", 16 )
	MisBeginCondition(NoMission, 678)
	MisBeginCondition(NoRecord, 678)
	MisBeginAction(AddMission, 678)
	MisBeginAction(AddTrigger, 6781, TE_KILL, 130, 10 )
	MisCancelAction(ClearMission, 678)

	MisNeed(MIS_NEED_KILL, 130, 10, 10, 10)
	
	MisHelpTalk("<t>Getting rid of 10 Snowy Shrooms should be simple for you.")
	MisResultTalk("<t>Thank you! I feel better now.")
	MisResultCondition(NoRecord, 678)
	MisResultCondition(HasMission, 678)
	MisResultCondition(HasFlag, 678, 19 )
	MisResultAction(ClearMission, 678)
	MisResultAction(SetRecord, 678)
	MisResultAction(AddExp, 550, 550)
	MisResultAction(AddMoney,229,229)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 130 )	
	TriggerAction( 1, AddNextFlag, 678, 10, 10 )
	RegCurTrigger( 6781 )

-----------------------------------
	DefineMission( 679, "Snowy Mushroom", 679 )
	
	MisBeginTalk( "<t>My appetite is not so good as I am getting old. Therefore, I am unable to take anything oily or I'll fall ill.<n><t>Can you get 5 <ySnowy Mushrooms> at <j(952, 550)> for me to cook a nutritious meal?")
	MisBeginCondition(LvCheck, ">", 16 )
	MisBeginCondition(NoMission, 679)
	MisBeginCondition(NoRecord, 679)
	MisBeginAction(AddMission, 679)
	MisBeginAction(AddTrigger, 6791, TE_GETITEM, 4104, 5 )
	MisCancelAction(ClearMission, 679)

	MisNeed(MIS_NEED_ITEM, 4104, 5, 10, 5)
	
	MisHelpTalk("<t>Have you found the <ySnowy Mushrooms>? Do you want me to die of hunger?")
	MisResultTalk("<t>Oh this taste good! You should try it too!")
	MisResultCondition(NoRecord, 679)
	MisResultCondition(HasMission, 679)
	MisResultCondition(HasItem, 4104, 5)
	MisResultAction(TakeItem, 4104, 5 )
	MisResultAction(ClearMission, 679)
	MisResultAction(SetRecord, 679)
	MisResultAction(AddExp, 550, 550)
	MisResultAction(AddMoney,459,459)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4104 )	
	TriggerAction( 1, AddNextFlag, 679, 10, 5 )
	RegCurTrigger( 6791 )


----------------------------------
	DefineMission( 680, "Slow Slow Snail!", 680 )
	
	MisBeginTalk( "<t><rSnowy Snails> are so slow yet they always like to move infront of me. They almost make an old person like me tripped for a few occasion.<n><t>Can you kill 10 <rSnowy Snails> at <j(657, 334)> for me please?")
	MisBeginCondition(LvCheck, ">", 17 )
	MisBeginCondition(NoMission, 680)
	MisBeginCondition(NoRecord, 680)
	MisBeginAction(AddMission, 680)
	MisBeginAction(AddTrigger, 6801, TE_KILL, 228, 10 )
	MisCancelAction(ClearMission, 680)

	MisNeed(MIS_NEED_KILL, 228, 10, 10, 10)
	
	MisHelpTalk("<t>Look for <rSnowy Snail> now!")
	MisResultTalk("<t>Thank you! This time round there will not be any <rSnowy Snails> blocking the way!")
	MisResultCondition(NoRecord, 680)
	MisResultCondition(HasMission, 680)
	MisResultCondition(HasFlag, 680, 19 )
	MisResultAction(ClearMission, 680)
	MisResultAction(SetRecord, 680)
	MisResultAction(AddExp, 650, 650)
	MisResultAction(AddMoney,242,242)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 228 )	
	TriggerAction( 1, AddNextFlag, 680, 10, 10 )
	RegCurTrigger( 6801 )

-----------------------------------
	DefineMission( 681, "Flowery Stone", 681 )
	
	MisBeginTalk( "<t>Beneath the shell of <rSnowy Snail> lies a very rare <yFlowery Stone>.<n><t>It is a magical stone that Goddess Kara created to allow human to understand the language of flowers.<n><t>Can you get 5 <yFlowery Stones> for my research? Those snails appears near <j(657, 334)>.")
	MisBeginCondition(LvCheck, ">", 18 )
	MisBeginCondition(NoMission, 681)
	MisBeginCondition(NoRecord, 681)
	MisBeginAction(AddMission, 681)
	MisBeginAction(AddTrigger, 6811, TE_GETITEM, 4105, 5 )
	MisCancelAction(ClearMission, 681)

	MisNeed(MIS_NEED_ITEM, 4105, 5, 10, 5)
	
	MisHelpTalk("<t>Have you collected 5 <yFlowery Stones>? They are hidden beneath the shells of <rSnowy Snails>.")
	MisResultTalk("<t>Thank you! I can continue the research once more!")
	MisResultCondition(NoRecord, 681)
	MisResultCondition(HasMission, 681)
	MisResultCondition(HasItem, 4105, 5)
	MisResultAction(TakeItem, 4105, 5 )
	MisResultAction(ClearMission, 681)
	MisResultAction(SetRecord, 681)
	MisResultAction(AddExp, 750, 750)
	MisResultAction(AddMoney,513,513)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4105 )	
	TriggerAction( 1, AddNextFlag, 681, 10, 5 )
	RegCurTrigger( 6811 )

-----------------------------------
	DefineMission( 682, "Snail Feeler", 682 )
	
	MisBeginTalk( "<t>The deep forest is so big, yet the weird collector, <bChang>, requested me to look for <yFrozen Snail Feelers>.<n><t>It's like looking for a needle in a haystack! Can you help me look for 5 <yFrozen Snail Feelers>?<n><t>The snails might appear near <j(657, 334)>.")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 682)
	MisBeginCondition(NoRecord, 682)
	MisBeginAction(AddMission, 682)
	MisBeginAction(AddTrigger, 6821, TE_GETITEM, 4428, 5 )
	MisCancelAction(ClearMission, 682)

	MisNeed(MIS_NEED_ITEM, 4428, 5, 10, 5)
	
	MisHelpTalk("<t>I need 5 <yFrozen Snail Feelers> to hand to <bChang>.")
	MisResultTalk("<t>Now everything is completed. Thank you!")
	MisResultCondition(NoRecord, 682)
	MisResultCondition(HasMission, 682)
	MisResultCondition(HasItem, 4428, 5)
	MisResultAction(TakeItem, 4428, 5 )
	MisResultAction(ClearMission, 682)
	MisResultAction(SetRecord, 682)
	MisResultAction(AddExp, 880, 880)
	MisResultAction(AddMoney,541,541)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4428 )	
	TriggerAction( 1, AddNextFlag, 682, 10, 5 )
	RegCurTrigger( 6821 )

-----------------------------------
	DefineMission( 683, "Search for Snowy Heart", 683 )
	
	MisBeginTalk( "<t>Wait please! My nephew has contracted a strange illness recently and is dying. Only the <yGlacier Heart> from a <rNaive Snow Doll> at <j(1055, 738)> can save him.<n><t>Please be so kind and get me 5 <yGlacier Hearts>!")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 683)
	MisBeginCondition(NoRecord, 683)
	MisBeginAction(AddMission, 683)
	MisBeginAction(AddTrigger, 6831, TE_GETITEM, 4431, 5 )
	MisCancelAction(ClearMission, 683)

	MisNeed(MIS_NEED_ITEM, 4431, 5, 10, 5)
	
	MisHelpTalk("<t>Go and collect 5 <yGlacier Hearts> please! Hurry!")
	MisResultTalk("<t>Oh you are back! Thanks a lot!")
	MisResultCondition(NoRecord, 683)
	MisResultCondition(HasMission, 683)
	MisResultCondition(HasItem, 4431, 5)
	MisResultAction(TakeItem, 4431, 5 )
	MisResultAction(ClearMission, 683)
	MisResultAction(SetRecord, 683)
	MisResultAction(AddExp, 880, 880)
	MisResultAction(AddMoney,541,541)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4431 )	
	TriggerAction( 1, AddNextFlag, 683, 10, 5 )
	RegCurTrigger( 6831 )


----------------------------------
	DefineMission( 684, "Naive Snow Doll", 684 )
	
	MisBeginTalk( "<t>It is rumored that <rNaive Snow Doll> can summon the evil <rSnow Lady> to bring disaster.<n><t>Even though there is no truth to this, it is better to be safe.<n><t>Can you kill 10 <rNaive Snow Dolls> around this village at <j(1055, 738)>?")
	MisBeginCondition(LvCheck, ">", 19 )
	MisBeginCondition(NoMission, 684)
	MisBeginCondition(NoRecord, 684)
	MisBeginAction(AddMission, 684)
	MisBeginAction(AddTrigger, 6841, TE_KILL, 255, 10 )
	MisCancelAction(ClearMission, 684)

	MisNeed(MIS_NEED_KILL, 255, 10, 10, 10)
	
	MisHelpTalk("<t>You have not killed 10 <rNa?ve Snow Dolls>?")
	MisResultTalk("<t>Thank you! You are very trustworthy. I will look for you again.")
	MisResultCondition(NoRecord, 684)
	MisResultCondition(HasMission, 684)
	MisResultCondition(HasFlag, 684, 19 )
	MisResultAction(ClearMission, 684)
	MisResultAction(SetRecord, 684)
	MisResultAction(AddExp, 880, 880)
	MisResultAction(AddMoney,270,270)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 255 )	
	TriggerAction( 1, AddNextFlag, 684, 10, 10 )
	RegCurTrigger( 6841 )

----------------------------------
	DefineMission( 685, "Cavern Little Bears", 685 )
	
	MisBeginTalk( "<ts>cientifically speaking, animals like bears usually hibernate during extreme cold temperature. However, as <rSnowy Bear Cub> has been living in such cold temperature for many years, they have slowly evolved to the point where by they don't need to hibernate anymore.<n><t>This evolution have brought much problems to those who are placed in charge to look after the forest such as us. We were attacked by these agressive <rSnowy Bear Cub> whenever we patrol the forest.<n><t>Can you help us solve the problem by killing 10 <rSnowy Bear Cub> at <j(958, 661)>?")
	MisBeginCondition(LvCheck, ">", 20 )
	MisBeginCondition(NoMission, 685)
	MisBeginCondition(NoRecord, 685)
	MisBeginAction(AddMission, 685)
	MisBeginAction(AddTrigger, 6851, TE_KILL, 142, 10 )
	MisCancelAction(ClearMission, 685)

	MisNeed(MIS_NEED_KILL, 142, 10, 10, 10)
	
	MisHelpTalk("<t>Why are you still daydreaming? Go and hunt 10 <rSnowy Bear Cubs>!")
	MisResultTalk("<t>I never knew you are so skillful!")
	MisResultCondition(NoRecord, 685)
	MisResultCondition(HasMission, 685)
	MisResultCondition(HasFlag, 685, 19 )
	MisResultAction(ClearMission, 685)
	MisResultAction(SetRecord, 685)
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney,285,285)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 142 )	
	TriggerAction( 1, AddNextFlag, 685, 10, 10 )
	RegCurTrigger( 6851 )

-----------------------------------
	DefineMission( 686, "Snow Tears", 686 )
	
	MisBeginTalk( "<t>I came from the desert looking for the <rSnow Lady>. It is said that they look like the Goddess Kara. I wish to see one but I can't find any trails leading to them.<n><t>Can you find me 5 <yPerfect Snowflakes> to prove that they existed?<n><t>It is rumored that they appeared near <j(873, 646)>.")
	MisBeginCondition(LvCheck, ">", 21 )
	MisBeginCondition(NoMission, 686)
	MisBeginCondition(NoRecord, 686)
	MisBeginAction(AddMission, 686)
	MisBeginAction(AddTrigger, 6861, TE_GETITEM, 4438, 5 )
	MisCancelAction(ClearMission, 686)

	MisNeed(MIS_NEED_ITEM, 4438, 5, 10, 5)
	
	MisHelpTalk("<t>Have you found 5 <yPerfect Snowflakes>?")
	MisResultTalk("<ts>o this is the legendary <yPerfect Snowflake>. Its so beautiful!")
	MisResultCondition(NoRecord, 686)
	MisResultCondition(HasMission, 686)
	MisResultCondition(HasItem, 4438, 5)
	MisResultAction(TakeItem, 4438, 5 )
	MisResultAction(ClearMission, 686)
	MisResultAction(SetRecord, 686)
	MisResultAction(AddExp, 1200, 1200)
	MisResultAction(AddMoney,601,601)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4438 )	
	TriggerAction( 1, AddNextFlag, 686, 10, 5 )
	RegCurTrigger( 6861 )


-----------------------------------
	DefineMission( 687, "Snow Crystal", 687 )
	
	MisBeginTalk( "<t>Hey friend, have you heard the latest news? Now the most profitable item on the black market is <ySnowy Crystal> used by <rSnow Lady>to create snowstorms! I know stealing from <rSnow Lady> isn't a good thing. However, money matters more than anything else!<n><t>I am offering a high price for you to go <j(873, 646)> to steal 5 <ySnowy Crystals> from the <rSnow Lady>. Are you willing to go? (The crystals are within the forest where the snow lady appears, pick it up immediately if you see one!)")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(NoMission, 687)
	MisBeginCondition(NoRecord, 687)
	MisBeginAction(AddMission, 687)
	MisBeginAction(AddTrigger, 6871, TE_GETITEM, 4106, 5 )
	MisCancelAction(ClearMission, 687)

	MisNeed(MIS_NEED_ITEM, 4106, 5, 10, 5)
	
	MisHelpTalk("<t>You need to look for <rSnow Lady> in order to obtain the <ySnowy Crystals>.")
	MisResultTalk("<t>You are great even though you are not that experienced!")
	MisResultCondition(NoRecord, 687)
	MisResultCondition(HasMission, 687)
	MisResultCondition(HasItem, 4106, 5)
	MisResultAction(TakeItem, 4106, 5 )
	MisResultAction(ClearMission, 687)
	MisResultAction(SetRecord, 687)
	MisResultAction(AddExp, 1400, 1400)
	MisResultAction(AddMoney,316,316)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4106 )	
	TriggerAction( 1, AddNextFlag, 687, 10, 5 )
	RegCurTrigger( 6871 )

----------------------------------
	DefineMission( 688, "Wailing Snow Lady", 688 )
	
	MisBeginTalk( "<t>Its good that you are here! Have you seen the <rSnow Ladies> nearby? They are so dangerous!<n><t>When they gets angry, they may even summon a snow storm upon this land!<n><t>Please get rid of these menace by killing 10 <ySnow Ladies>! They are near <j(873, 646)>.")
	MisBeginCondition(LvCheck, ">", 22 )
	MisBeginCondition(NoMission, 688)
	MisBeginCondition(NoRecord, 688)
	MisBeginAction(AddMission, 688)
	MisBeginAction(AddTrigger, 6881, TE_KILL, 281, 10 )
	MisCancelAction(ClearMission, 688)

	MisNeed(MIS_NEED_KILL, 281, 10, 10, 10)
	
	MisHelpTalk("<t>You only need to kill 10 <rSnow Ladies>. Don't not try to be a hero and defeat them all.")
	MisResultTalk("<t>It has been hard on you. Thank you!")
	MisResultCondition(NoRecord, 688)
	MisResultCondition(HasMission, 688)
	MisResultCondition(HasFlag, 688, 19 )
	MisResultAction(ClearMission, 688)
	MisResultAction(SetRecord, 688)
	MisResultAction(AddExp, 1400, 1400)
	MisResultAction(AddMoney,316,316)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 281 )	
	TriggerAction( 1, AddNextFlag, 688, 10, 10 )
	RegCurTrigger( 6881 )

----------------------------------
	DefineMission( 689, "Punish the Wolves", 689 )
	
	MisBeginTalk( "<t>Help! I have been chased by a pack of <rSnowy Wolves>! This is so scary!<n><t>Please save me by killing 10 <rSnowy Wolves> at <j(965, 775)>!")
	MisBeginCondition(LvCheck, ">", 23 )
	MisBeginCondition(NoMission, 689)
	MisBeginCondition(NoRecord, 689)
	MisBeginAction(AddMission, 689)
	MisBeginAction(AddTrigger, 6891, TE_KILL, 137, 10 )
	MisCancelAction(ClearMission, 689)

	MisNeed(MIS_NEED_KILL, 137, 10, 10, 10)
	
	MisHelpTalk("<t>Have you killed the 10 <rSnowy Wolves>?")
	MisResultTalk("<t>Oh, thank you! You are my saviour!")
	MisResultCondition(NoRecord, 689)
	MisResultCondition(HasMission, 689)
	MisResultCondition(HasFlag, 689, 19 )
	MisResultAction(ClearMission, 689)
	MisResultAction(SetRecord, 689)
	MisResultAction(AddExp, 1600, 1600)
	MisResultAction(AddMoney,332,332)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 137 )	
	TriggerAction( 1, AddNextFlag, 689, 10, 10 )
	RegCurTrigger( 6891 )

-----------------------------------
	DefineMission( 690, "Nutritious Tortoise", 690 )
	
	MisBeginTalk( "<t>You've come at the right time my friend!<n><t>I feel like eating Herbal Jelly recently but they can only be found on Battle Tortoises at <j(891, 750)>.<n><t>I am no match for them anyway. Can you help me get 5 Herbal Jelly back?")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 690)
	MisBeginCondition(NoRecord, 690)
	MisBeginAction(AddMission, 690)
	MisBeginAction(AddTrigger, 6901, TE_GETITEM, 4442, 5 )
	MisCancelAction(ClearMission, 690)

	MisNeed(MIS_NEED_ITEM, 4442, 5, 10, 5)
	
	MisHelpTalk("<t>When will you bring the Herbal Jelly over?")
	MisResultTalk("<t>Yummy! This is the how good Herbal Jelly should taste like.")
	MisResultCondition(NoRecord, 690)
	MisResultCondition(HasMission, 690)
	MisResultCondition(HasItem, 4442, 5)
	MisResultAction(TakeItem, 4442, 5 )
	MisResultAction(ClearMission, 690)
	MisResultAction(SetRecord, 690)
	MisResultAction(AddExp, 1800, 1800)
	MisResultAction(AddMoney,697,697)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4442 )	
	TriggerAction( 1, AddNextFlag, 690, 10, 5 )
	RegCurTrigger( 6901 )

----------------------------------
	DefineMission( 691, "Long Nose Tortoise", 691 )
	
	MisBeginTalk( "<t>My friend! I have just seen a fearsome creature loitering around here.<n><t>They look like tortoises yet have a elephant tusk on their back. I heard from the native here that they are called <rBattle Tortoises>.<n><t>Can you kill 10 Battle Tortoises at <j(891, 75)> to curb my fear?")
	MisBeginCondition(LvCheck, ">", 24 )
	MisBeginCondition(NoMission, 691)
	MisBeginCondition(NoRecord, 691)
	MisBeginAction(AddMission, 691)
	MisBeginAction(AddTrigger, 6911, TE_KILL, 265, 10 )
	MisCancelAction(ClearMission, 691)

	MisNeed(MIS_NEED_KILL, 265, 10, 10, 10)
	
	MisHelpTalk("<t>Have you not killed 10 Battle Tortoises?")
	MisResultTalk("<t>You are a trustworthy person! Thanks!")
	MisResultCondition(NoRecord, 691)
	MisResultCondition(HasMission, 691)
	MisResultCondition(HasFlag, 691, 19 )
	MisResultAction(ClearMission, 691)
	MisResultAction(SetRecord, 691)
	MisResultAction(AddExp, 1800, 1800)
	MisResultAction(AddMoney,348,348)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 265 )	
	TriggerAction( 1, AddNextFlag, 691, 10, 10 )
	RegCurTrigger( 6911 )

----------------------------------
	DefineMission( 692, "Brute Crabby", 692 )
	
	MisBeginTalk( "<t>Oh my god! A bunch of wild <rArmored Crabs> have robbed me of my goods!<n><t>How dare they! Can you help me teach them a lesson?<n><t>Kill 10 <rArmored Crabs> at <j(994, 857)> as a warning to them please!")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 692)
	MisBeginCondition(NoRecord, 692)
	MisBeginAction(AddMission, 692)
	MisBeginAction(AddTrigger, 6921, TE_KILL, 143, 10 )
	MisCancelAction(ClearMission, 692)

	MisNeed(MIS_NEED_KILL, 143, 10, 10, 10)
	
	MisHelpTalk("<t>Please do not forget to kill 10 <rArmored Crabs.>.")
	MisResultTalk("<t>Hoho! Now those <rArmored Crab> will fear us.")
	MisResultCondition(NoRecord, 692)
	MisResultCondition(HasMission, 692)
	MisResultCondition(HasFlag, 692, 19 )
	MisResultAction(ClearMission, 692)
	MisResultAction(SetRecord, 692)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,365,365)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 143 )	
	TriggerAction( 1, AddNextFlag, 692, 10, 10 )
	RegCurTrigger( 6921 )

-----------------------------------
	DefineMission( 693, "Crab Egg Sauce", 693 )
	
	MisBeginTalk( "<t> When I moved to this chilling place, I was hoping to eat some fantastic egg sauce.<n><t>I heard that <yArmored Crab Eggs> make a great sauce. Please get 5 <yArmored Crab Eggs> for me. Their lair is at <j(994, 857)>.")
	MisBeginCondition(LvCheck, ">", 25 )
	MisBeginCondition(NoMission, 693)
	MisBeginCondition(NoRecord, 693)
	MisBeginAction(AddMission, 693)
	MisBeginAction(AddTrigger, 6931, TE_GETITEM, 4107, 5 )
	MisCancelAction(ClearMission, 693)

	MisNeed(MIS_NEED_ITEM, 4107, 5, 10, 5)
	
	MisHelpTalk("<t>Have you found the <yArmored Crab Eggs> I am looking for?")
	MisResultTalk("<t>Thank you! You are such a nice person!")
	MisResultCondition(NoRecord, 693)
	MisResultCondition(HasMission, 693)
	MisResultCondition(HasItem, 4107, 5)
	MisResultAction(TakeItem, 4107, 5 )
	MisResultAction(ClearMission, 693)
	MisResultAction(SetRecord, 693)
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney,730,730)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4107 )	
	TriggerAction( 1, AddNextFlag, 693, 10, 5 )
	RegCurTrigger( 6931 )

-----------------------------------
	DefineMission( 694, "Gourmet Critic", 694 )
	
	MisBeginTalk( "<t>As the <rNorthern Snail> stay on this iceland, when it die the meat will be frozen forming into <yFrozen Conch Meat>. It is actually a very delicious dish that I wish to eat. <n><t>Can you get 5 pieces of <yFrozen Conch Meat> near <j(802, 750)> for me?")
	MisBeginCondition(LvCheck, ">", 26 )
	MisBeginCondition(NoMission, 694)
	MisBeginCondition(NoRecord, 694)
	MisBeginAction(AddMission, 694)
	MisBeginAction(AddTrigger, 6941, TE_GETITEM, 4464, 5 )
	MisCancelAction(ClearMission, 694)

	MisNeed(MIS_NEED_ITEM, 4464, 5, 10, 5)
	
	MisHelpTalk("<t>Where is the 5 <yFrozen Conch Meat> I entasked you to get for me?")
	MisResultTalk("<t>Thank you! This is just what I needed!")
	MisResultCondition(NoRecord, 694)
	MisResultCondition(HasMission, 694)
	MisResultCondition(HasItem, 4464, 5)
	MisResultAction(TakeItem, 4464, 5 )
	MisResultAction(ClearMission, 694)
	MisResultAction(SetRecord, 694)
	MisResultAction(AddExp, 2400, 2400)
	MisResultAction(AddMoney,765,765)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4464 )	
	TriggerAction( 1, AddNextFlag, 694, 10, 5 )
	RegCurTrigger( 6941 )

-----------------------------------
	DefineMission( 695, "Collect Heart of Purity", 695 )
	
	MisBeginTalk( "<t>Hey friend, can I have some of your time? My friend is becoming interested with mythical beings, he asked me to collect 5 Heart of Purity from the Snow Spirit. Can you go and help me collect 5 of it?<n><t>You can find the Heart of Purity from the <rSnow Spirit> at <j(1060, 805)>.")
	MisBeginCondition(LvCheck, ">", 27 )
	MisBeginCondition(NoMission, 695)
	MisBeginCondition(NoRecord, 695)
	MisBeginAction(AddMission, 695)
	MisBeginAction(AddTrigger, 6951, TE_GETITEM, 4481, 5 )
	MisCancelAction(ClearMission, 695)

	MisNeed(MIS_NEED_ITEM, 4481, 5, 10, 5)
	
	MisHelpTalk("<t><yHeart of Purity> is the soul of <rSnow Spirit>.")
	MisResultTalk("<t>Thank you! Nowadays people are no longer as helpful as you.")
	MisResultCondition(NoRecord, 695)
	MisResultCondition(HasMission, 695)
	MisResultCondition(HasItem, 4481, 5)
	MisResultAction(TakeItem, 4481, 5 )
	MisResultAction(ClearMission, 695)
	MisResultAction(SetRecord, 695)
	MisResultAction(AddExp, 2700, 2700)
	MisResultAction(AddMoney,800,800)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4481 )	
	TriggerAction( 1, AddNextFlag, 695, 10, 5 )
	RegCurTrigger( 6951 )

----------------------------------
	DefineMission( 696, "Buggy Snow Doll", 696 )
	
	MisBeginTalk( "<t><rFragile Snow Dolls> are constantly posing a threat to the villagers.<n><t>Can you kill 10 <rFragile Snow Dolls> at <j(908, 842)> for us?")
	MisBeginCondition(LvCheck, ">", 28 )
	MisBeginCondition(NoMission, 696)
	MisBeginCondition(NoRecord, 696)
	MisBeginAction(AddMission, 696)
	MisBeginAction(AddTrigger, 6961, TE_KILL, 256, 10 )
	MisCancelAction(ClearMission, 696)

	MisNeed(MIS_NEED_KILL, 256, 10, 10, 10)
	
	MisHelpTalk("<t>I need you to hunt 10 <rFragile Snow Dolls> please")
	MisResultTalk("<t>Heh! You have done well. We will work together again in the future.")
	MisResultCondition(NoRecord, 696)
	MisResultCondition(HasMission, 696)
	MisResultCondition(HasFlag, 696, 19 )
	MisResultAction(ClearMission, 696)
	MisResultAction(SetRecord, 696)
	MisResultAction(AddExp, 3000, 3000)
	MisResultAction(AddMoney,417,417)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 256 )	
	TriggerAction( 1, AddNextFlag, 696, 10, 10 )
	RegCurTrigger( 6961 )

----------------------------------
	DefineMission( 697, "Primal Snowman", 697 )
	
	MisBeginTalk( "<t>Hi! Want to prove your valor? Now there is a chance! Villagers are terrified of the huge <rYetis> nearby.<n><t>Kill 10 <rYetis> at <j(1495, 667)> to prove your courage!")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 697)
	MisBeginCondition(NoRecord, 697)
	MisBeginAction(AddMission, 697)
	MisBeginAction(AddTrigger, 6971, TE_KILL, 98, 10 )
	MisCancelAction(ClearMission, 697)

	MisNeed(MIS_NEED_KILL, 98, 10, 10, 10)
	
	MisHelpTalk("<t>No worry! <rYetis> are clumsy by nature. You will not get injure if you are fast.")
	MisResultTalk("<t>Heh! You have done well. We will work together again in the future.")
	MisResultCondition(NoRecord, 697)
	MisResultCondition(HasMission, 697)
	MisResultCondition(HasFlag, 697, 19 )
	MisResultAction(ClearMission, 697)
	MisResultAction(SetRecord, 697)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,424,424)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 98 )	
	TriggerAction( 1, AddNextFlag, 697, 10, 10 )
	RegCurTrigger( 6971 )

-----------------------------------
	DefineMission( 698, "Lost Mystery", 698 )
	
	MisBeginTalk( "<t>There have been a lot of missing cases near <pIcicle City> and I suspect that it has something to do with the <rYetis> at <j(1495, 667)>.<n><t>Can you help me collect 5 <rYeti Nails> for my investigation?")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 698)
	MisBeginCondition(NoRecord, 698)
	MisBeginAction(AddMission, 698)
	MisBeginAction(AddTrigger, 6981, TE_GETITEM, 4446, 5 )
	MisCancelAction(ClearMission, 698)

	MisNeed(MIS_NEED_ITEM, 4446, 5, 10, 5)
	
	MisHelpTalk("<t>You have not completed such a simple task?")
	MisResultTalk("<ts>trangethese bloodstains does not belong to a human")
	MisResultCondition(NoRecord, 698)
	MisResultCondition(HasMission, 698)
	MisResultCondition(HasItem, 4446, 5)
	MisResultAction(TakeItem, 4446, 5 )
	MisResultAction(ClearMission, 698)
	MisResultAction(SetRecord, 698)
	MisResultAction(AddExp, 3400, 3400)
	MisResultAction(AddMoney,849,849)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4446 )	
	TriggerAction( 1, AddNextFlag, 698, 10, 5 )
	RegCurTrigger( 6981 )

----------------------------------
	DefineMission( 699, "Win Win Pig!", 699 )
	
	MisBeginTalk( "<t>Ouch!Ouch! Those cuts on my face are caused by <rCombat Piglets>.<n><t>They get aggressive over anybody with a weapon. Please put a stop to these by killing 10 <rCombat Piglets> at <j(1453, 560)>!")
	MisBeginCondition(LvCheck, ">", 30 )
	MisBeginCondition(NoMission, 699)
	MisBeginCondition(NoRecord, 699)
	MisBeginAction(AddMission, 699)
	MisBeginAction(AddTrigger, 6991, TE_KILL, 296, 10 )
	MisCancelAction(ClearMission, 699)

	MisNeed(MIS_NEED_KILL, 296, 10, 10, 10)
	
	MisHelpTalk("<t>Go and hunt 10 <rCombat Piglets>. Hurry!")
	MisResultTalk("<t>Nicely done! This is my reward for you!")
	MisResultCondition(NoRecord, 699)
	MisResultCondition(HasMission, 699)
	MisResultCondition(HasFlag, 699, 19 )
	MisResultAction(ClearMission, 699)
	MisResultAction(SetRecord, 699)
	MisResultAction(AddExp, 3900, 3900)
	MisResultAction(AddMoney,431,431)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 296 )	
	TriggerAction( 1, AddNextFlag, 699, 10, 10 )
	RegCurTrigger( 6991 )

----------------------------------
	DefineMission( 800, "Playful Snow Doll", 800 )
	
	MisBeginTalk( "<t>I am the chairman of <pIcicle city> and have to take responsibility for the safety of travelers.<n><t>Recently, <rPlayful Snow Dolls> have been attacking anybody they see.<n><t>Please kill 10 <rPlayful Snow Dolls> at <j(1746, 530)> to make our road safe once more!")
	MisBeginCondition(LvCheck, ">", 32 )
	MisBeginCondition(NoMission, 800)
	MisBeginCondition(NoRecord, 800)
	MisBeginAction(AddMission, 800)
	MisBeginAction(AddTrigger, 8001, TE_KILL, 257, 10 )
	MisCancelAction(ClearMission, 800)

	MisNeed(MIS_NEED_KILL, 257, 10, 10, 10)
	
	MisHelpTalk("<t>You must kill 10 <rPlayful Snow Dolls>.")
	MisResultTalk("<t>Now the road is safe without these <rPlayful Snow Dolls>.")
	MisResultCondition(NoRecord, 800)
	MisResultCondition(HasMission, 800)
	MisResultCondition(HasFlag, 800, 19 )
	MisResultAction(ClearMission, 800)
	MisResultAction(SetRecord, 800)
	MisResultAction(AddExp, 4900, 4900)
	MisResultAction(AddMoney,446,446)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 257 )	
	TriggerAction( 1, AddNextFlag, 800, 10, 10 )
	RegCurTrigger( 8001 )

-----------------------------------
	DefineMission( 801, "Soul Gem", 801 )
	
	MisBeginTalk( "<t>The amount missing cases is increasing! We need to do something! People seems to disappear near places where <rPlayful Snow Dolls> appeared.<n><t>Can you get 5 <ySoul Gems> from the <rPlayful Snow Dolls> at <j(1746, 530)> for my investigation?")
	MisBeginCondition(LvCheck, ">", 33 )
	MisBeginCondition(NoMission, 801)
	MisBeginCondition(NoRecord, 801)
	MisBeginAction(AddMission, 801)
	MisBeginAction(AddTrigger, 8011, TE_GETITEM, 4108, 5 )
	MisCancelAction(ClearMission, 801)

	MisNeed(MIS_NEED_ITEM, 4108, 5, 10, 5)
	
	MisHelpTalk("<t>Have you been able to find the 5 <ySoul Gems>?")
	MisResultTalk("<t>Oh my god! So those missing people are being sucked into these <rSoul Gems>!<n><t>I need to warn the city!")
	MisResultCondition(NoRecord, 801)
	MisResultCondition(HasMission, 801)
	MisResultCondition(HasItem, 4108, 5)
	MisResultAction(TakeItem, 4108, 5 )
	MisResultAction(ClearMission, 801)
	MisResultAction(SetRecord, 801)
	MisResultAction(AddExp, 5500, 5500)
	MisResultAction(AddMoney,907,907)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4108 )	
	TriggerAction( 1, AddNextFlag, 801, 10, 5 )
	RegCurTrigger( 8011 )

----------------------------------
	DefineMission( 802, "Slaughter of Innocent", 802 )
	
	MisBeginTalk( "<t>I want you to kill 10 <rBattle Tortoises> for me! No special reasons! I just don't like the looks of them.<n><t>I will reward you greatly.")
	MisBeginCondition(LvCheck, ">", 34 )
	MisBeginCondition(NoMission, 802)
	MisBeginCondition(NoRecord, 802)
	MisBeginAction(AddMission, 802)
	MisBeginAction(AddTrigger, 8021, TE_KILL, 141, 10 )
	MisCancelAction(ClearMission, 802)

	MisNeed(MIS_NEED_KILL, 141, 10, 10, 10)
	
	MisHelpTalk("<t>You want the rewards? Kill 10 <rBattle Tortoises> then.")
	MisResultTalk("<t>Well done. This is your reward.")
	MisResultCondition(NoRecord, 802)
	MisResultCondition(HasMission, 802)
	MisResultCondition(HasFlag, 802, 19 )
	MisResultAction(ClearMission, 802)
	MisResultAction(SetRecord, 802)
	MisResultAction(AddExp, 6200, 6200)
	MisResultAction(AddMoney,461,461)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 141 )	
	TriggerAction( 1, AddNextFlag, 802, 10, 10 )
	RegCurTrigger( 8021 )

-----------------------------------
	DefineMission( 803, "Unhealing Wound", 803 )
	
	MisBeginTalk( "<t>I have been attacked by <rSkeletal Warriors> recently. Even though my wounds are not deep, it hurts greatly in this chilling place.<n><t>I heard that <yHeated Tortoise Shell> from <rSnowy Tortoise> at <j(2165, 462)> is effective in curing this kind of wounds. Please get 5 <yHeated Tortoise Shells> for me please!")
	MisBeginCondition(LvCheck, ">", 34 )
	MisBeginCondition(NoMission, 803)
	MisBeginCondition(NoRecord, 803)
	MisBeginAction(AddMission, 803)
	MisBeginAction(AddTrigger, 8031, TE_GETITEM, 4468, 5 )
	MisCancelAction(ClearMission, 803)

	MisNeed(MIS_NEED_ITEM, 4468, 5, 10, 5)
	
	MisHelpTalk("<t>The wound starting to hurt badly again! Get 5 <yHeated Tortoise Shells> for me please!")
	MisResultTalk("Thank you! Now I feel much better.")
	MisResultCondition(NoRecord, 803)
	MisResultCondition(HasMission, 803)
	MisResultCondition(HasItem, 4468, 5)
	MisResultAction(TakeItem, 4468, 5 )
	MisResultAction(ClearMission, 803)
	MisResultAction(SetRecord, 803)
	MisResultAction(AddExp, 6900, 6900)
	MisResultAction(AddMoney,939,939)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4468 )	
	TriggerAction( 1, AddNextFlag, 803, 10, 5 )
	RegCurTrigger( 8031 )

----------------------------------
	DefineMission( 804, "Fallen Lamb", 804 )
	
	MisBeginTalk( "<t>Hey, have you seen those <rCrazy Sheeps>? I heard that they were mutated animals totally different from that created by the female goddess Kara!<n><t>They can be taken as the embodiment of evil.<n><t>My friend, if you believe in Goddess Kara, please help to destroy 10 <rCrazy sheeps>, they can be found at <j(226, 590)>.")
	MisBeginCondition(LvCheck, ">", 36 )
	MisBeginCondition(NoMission, 804)
	MisBeginCondition(NoRecord, 804)
	MisBeginAction(AddMission, 804)
	MisBeginAction(AddTrigger, 8041, TE_KILL, 297, 10 )
	MisCancelAction(ClearMission, 804)

	MisNeed(MIS_NEED_KILL, 297, 10, 10, 10)
	
	MisHelpTalk("<t>You promised to kill 10 <rCrazy Sheeps>. Have you done so?")
	MisResultTalk("<t>Thanks for your help!")
	MisResultCondition(NoRecord, 804)
	MisResultCondition(HasMission, 804)
	MisResultCondition(HasFlag, 804, 19 )
	MisResultAction(ClearMission, 804)
	MisResultAction(SetRecord, 804)
	MisResultAction(AddExp, 7700, 7700)
	MisResultAction(AddMoney,477,477)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 297 )	
	TriggerAction( 1, AddNextFlag, 804, 10, 10 )
	RegCurTrigger( 8041 )

-----------------------------------
	DefineMission( 805, "Scorpion Poison", 805 )
	
	MisBeginTalk( "<t>Hi! You have come at the right time! My girlfriend, <bNana> has fallen into a coma after getting stung by a <rFerocious Scorpion> at <j(2370, 548)>.<n><t>I need 5 vials of <yScorpion Blood> to revive her but I cannot leave as I need to look after her. Can you help me?")
	MisBeginCondition(LvCheck, ">", 37 )
	MisBeginCondition(NoMission, 805)
	MisBeginCondition(NoRecord, 805)
	MisBeginAction(AddMission, 805)
	MisBeginAction(AddTrigger, 8051, TE_GETITEM, 4482, 5 )
	MisCancelAction(ClearMission, 805)

	MisNeed(MIS_NEED_ITEM, 4482, 5, 10, 5)
	
	MisHelpTalk("<t>Time is precious! Please collect 5 vials of <yScorpion Blood now>!")
	MisResultTalk("<t>This is great! <yScorpion Blood > is effective! Nana has awakened!")
	MisResultCondition(NoRecord, 805)
	MisResultCondition(HasMission, 805)
	MisResultCondition(HasItem, 4482, 5)
	MisResultAction(TakeItem, 4482, 5 )
	MisResultAction(ClearMission, 805)
	MisResultAction(SetRecord, 805)
	MisResultAction(AddExp, 8600, 8600)
	MisResultAction(AddMoney,972,972)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4482 )	
	TriggerAction( 1, AddNextFlag, 805, 10, 5 )
	RegCurTrigger( 8051 )

----------------------------------
	DefineMission( 806, "Pioneer", 806 )
	
	MisBeginTalk( "<t>Hi! You have come at the right time!<n><t>I plan to bring my girlfriend, <bNana> to <pIcicle City> for a holiday. However, our route has been blocked by a bunch of ferocious <rPolar Bears>.<n><t>Can you kill 10 <rPolar Bears> at <j(1981, 530)> for us? 189=<t>You must kill 10 <rPlayful Snow Dolls>.")
	MisBeginCondition(LvCheck, ">", 38 )
	MisBeginCondition(NoMission, 806)
	MisBeginCondition(NoRecord, 806)
	MisBeginAction(AddMission, 806)
	MisBeginAction(AddTrigger, 8061, TE_KILL, 259, 10 )
	MisCancelAction(ClearMission, 806)

	MisNeed(MIS_NEED_KILL, 259, 10, 10, 10)
	
	MisHelpTalk("<t>I need you to kill 10 <rPolar Bears>.")
	MisResultTalk("<t>Thank you! I can go on my vacation now!")
	MisResultCondition(NoRecord, 806)
	MisResultCondition(HasMission, 806)
	MisResultCondition(HasFlag, 806, 19 )
	MisResultAction(ClearMission, 806)
	MisResultAction(SetRecord, 806)
	MisResultAction(AddExp, 9600, 9600)
	MisResultAction(AddMoney,495,495)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 259 )	
	TriggerAction( 1, AddNextFlag, 806, 10, 10 )
	RegCurTrigger( 8061 )

-----------------------------------
	DefineMission( 807, "Phantom Sword", 807 )
	
	MisBeginTalk( "<t> Hi! Do you know why these <rSnowy Tusk Boars> are always roaming around <j(2269, 590)>? It is because they are the guardians of a legendary weapon, the <yPhantom Sword>.<n><t>I am very interested in this sword. Please get it for me!<n><t>It should be stuck inside a rock.")
	MisBeginCondition(LvCheck, ">", 38 )
	MisBeginCondition(NoMission, 807)
	MisBeginCondition(NoRecord, 807)
	MisBeginAction(AddMission, 807)
	MisBeginAction(AddTrigger, 8071, TE_GETITEM, 4109, 1 )
	MisCancelAction(ClearMission, 807)

	MisNeed(MIS_NEED_ITEM, 4109, 1, 10, 1)
	
	MisHelpTalk("<t>Have you found the <yPhantom Sword>?")
	MisResultTalk("<t>Woah! This is an amazing sword!")
	MisResultCondition(NoRecord, 807)
	MisResultCondition(HasMission, 807)
	MisResultCondition(HasItem, 4109, 1)
	MisResultAction(TakeItem, 4109, 1 )
	MisResultAction(ClearMission, 807)
	MisResultAction(SetRecord, 807)
	MisResultAction(AddExp, 9600, 9600)
	MisResultAction(AddMoney,495,495)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4109 )	
	TriggerAction( 1, AddNextFlag, 807, 10, 1 )
	RegCurTrigger( 8071 )

----------------------------------
	DefineMission( 808, "Warrior Soul", 808 )
	
	MisBeginTalk( "<t>Maybe its because I can see things that normal people can't, I often hear wails from the spirits of the <rUndead Warriors>.<n><t>Thsee sound of suffering comes from the souls trapped in the body of those <rSkeletal Warriors>.<n><t>By destroying their skeletal body forms, we will be able to free their souls.<n><t>My friend, are you willing to help me? Kill 10 <rSkeletal Warriors> at <j(2269 ,470)> to save their souls.")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(NoMission, 808)
	MisBeginCondition(NoRecord, 808)
	MisBeginAction(AddMission, 808)
	MisBeginAction(AddTrigger, 8081, TE_KILL, 268, 10 )
	MisCancelAction(ClearMission, 808)

	MisNeed(MIS_NEED_KILL, 268, 10, 10, 10)
	
	MisHelpTalk("<t>Hey you promised to kill 10 <rSkeletal Warriors>! Don't break it!")
	MisResultTalk("<t>I am sure that those <rSkeletal Warriors> will thank you for their release.")
	MisResultCondition(NoRecord, 808)
	MisResultCondition(HasMission, 808)
	MisResultCondition(HasFlag, 808, 19 )
	MisResultAction(ClearMission, 808)
	MisResultAction(SetRecord, 808)
	MisResultAction(AddExp, 10600, 10600)
	MisResultAction(AddMoney,504,504)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 268 )	
	TriggerAction( 1, AddNextFlag, 808, 10, 10 )
	RegCurTrigger( 8081 )

----------------------------------
	DefineMission( 809, "Vampiric Monster", 809 )
	
	MisBeginTalk( "<t>In the <pSilver Mine>, there is a creature that lives off blood of human. They are the <rVampire Bats>.<n><t>The miners are terrified of them. Can you do us a favor by killing 10 <rVampire Bats> in Silver Mine 2 so that we could mine in peace?")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 809)
	MisBeginCondition(NoRecord, 809)
	MisBeginAction(AddMission, 809)
	MisBeginAction(AddTrigger, 8091, TE_KILL, 82, 10 )
	MisCancelAction(ClearMission, 809)

	MisNeed(MIS_NEED_KILL, 82, 10, 10, 10)
	
	MisHelpTalk("<t>You will need to kill 10 <rVampire Bats>.")
	MisResultTalk("<t>Thank you for your help!")
	MisResultCondition(NoRecord, 809)
	MisResultCondition(HasMission, 809)
	MisResultCondition(HasFlag, 809, 19 )
	MisResultAction(ClearMission, 809)
	MisResultAction(SetRecord, 809)
	MisResultAction(AddExp, 3500, 3500)
	MisResultAction(AddMoney,424,424)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 82 )	
	TriggerAction( 1, AddNextFlag, 809, 10, 10 )
	RegCurTrigger( 8091 )

-----------------------------------
	DefineMission( 810, "Miner Mole Safety Helmet", 810 )
	
	MisBeginTalk( "<t><bLulu> and me want to go to the <pSilver Mine> to have fun.<n><t>However, both of us do not have any safety helmets so the worker refused to let us in.<n><t>Can you please go to Silver Mine 2 and get us 2 <yMiner Mole Safety Helmets> from those <rMiner Moles>? Help us please, we really want to go inside <pSilver Mine> to play!")
	MisBeginCondition(LvCheck, ">", 32 )
	MisBeginCondition(NoMission, 810)
	MisBeginCondition(NoRecord, 810)
	MisBeginAction(AddMission, 810)
	MisBeginAction(AddTrigger, 8101, TE_GETITEM, 4448, 2 )
	MisCancelAction(ClearMission, 810)

	MisNeed(MIS_NEED_ITEM, 4448, 2, 10, 2)
	
	MisHelpTalk("<t>When can I get the 2 <yMiner Mole Safety Helmet>?")
	MisResultTalk("<t>Thank you! I can play with <bLulu> now!")
	MisResultCondition(NoRecord, 810)
	MisResultCondition(HasMission, 810)
	MisResultCondition(HasItem, 4448, 2)
	MisResultAction(TakeItem, 4448, 2 )
	MisResultAction(ClearMission, 810)
	MisResultAction(SetRecord, 810)
	MisResultAction(AddExp, 4900, 4900)
	MisResultAction(AddMoney,892,892)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4448 )	
	TriggerAction( 1, AddNextFlag, 810, 10, 2 )
	RegCurTrigger( 8101 )


-----------------------------------
	DefineMission( 811, "Weird Mud", 811 )
	
	MisBeginTalk( "<t>It was such a shock! I was attacked by a mould of mud as I was entering <pAbandoned Mine>. My God, I never knew even mud has a life of their own!<n><t>Can you please collect 5 <ySticky Mud Cakes> from them as I wish to study it. These weird creatures can be found at <j(934, 2747)>.")
	MisBeginCondition(LvCheck, ">", 33 )
	MisBeginCondition(NoMission, 811)
	MisBeginCondition(NoRecord, 811)
	MisBeginAction(AddMission, 811)
	MisBeginAction(AddTrigger, 8111, TE_GETITEM, 4363, 5 )
	MisCancelAction(ClearMission, 811)

	MisNeed(MIS_NEED_ITEM, 4363, 5, 10, 5)
	
	MisHelpTalk("<t>Have you collected 5 chunk of <ySticky Mud Cake> yet?")
	MisResultTalk("<t>Oh dear! These <rMud Monster> have such complicated internal!")
	MisResultCondition(NoRecord, 811)
	MisResultCondition(HasMission, 811)
	MisResultCondition(HasItem, 4363, 5)
	MisResultAction(TakeItem, 4363, 5 )
	MisResultAction(ClearMission, 811)
	MisResultAction(SetRecord, 811)
	MisResultAction(AddExp, 5500, 5500)
	MisResultAction(AddMoney,907,907)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4363 )	
	TriggerAction( 1, AddNextFlag, 811, 10, 5 )
	RegCurTrigger( 8111 )

----------------------------------
	DefineMission( 812, "Revenge of the Mud", 812 )
	
	MisBeginTalk( "<t>I went to <pSilver Mine> to dig for stones.<n><t>Out of nowhere, a <rMud Monster> appeared and flung mud on my new shirt.<n><t>Argh, I can't take it! Can you kill 10 <rMud Monsters> at <j(934, 2747)> as a revenge for me?")
	MisBeginCondition(LvCheck, ">", 34 )
	MisBeginCondition(NoMission, 812)
	MisBeginCondition(NoRecord, 812)
	MisBeginAction(AddMission, 812)
	MisBeginAction(AddTrigger, 8121, TE_KILL, 253, 10 )
	MisCancelAction(ClearMission, 812)

	MisNeed(MIS_NEED_KILL, 253, 10, 10, 10)
	
	MisHelpTalk("<t>You have not killed 10 <rMud Monsters>? I am waiting")
	MisResultTalk("<t>Thank you! Thank you!")
	MisResultCondition(NoRecord, 812)
	MisResultCondition(HasMission, 812)
	MisResultCondition(HasFlag, 812, 19 )
	MisResultAction(ClearMission, 812)
	MisResultAction(SetRecord, 812)
	MisResultAction(AddExp, 6200, 6200)
	MisResultAction(AddMoney,461,461)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 253 )	
	TriggerAction( 1, AddNextFlag, 812, 10, 10 )
	RegCurTrigger( 8121 )


----------------------------------
	DefineMission( 813, "Miner Mole", 813 )
	
	MisBeginTalk( "<t>When I am exploring <pAbandoned Mine>, some <rMiner Moles> attacked me suddenly! Their razor sharp claws wounded me deeply.<n><t>Can you take a revenge for me by getting rid of those <rMiner Moles>? Please kill 10 <rMiner Moles>!")
	MisBeginCondition(LvCheck, ">", 35 )
	MisBeginCondition(NoMission, 813)
	MisBeginCondition(NoRecord, 813)
	MisBeginAction(AddMission, 813)
	MisBeginAction(AddTrigger, 8131, TE_KILL, 88, 10 )
	MisCancelAction(ClearMission, 813)

	MisNeed(MIS_NEED_KILL, 88, 10, 10, 10)
	
	MisHelpTalk("<t>You have not killed 10 <rMiner Moles>.")
	MisResultTalk("<t>Thank you. I feel so much happier!")
	MisResultCondition(NoRecord, 813)
	MisResultCondition(HasMission, 813)
	MisResultCondition(HasFlag, 813, 19 )
	MisResultAction(ClearMission, 813)
	MisResultAction(SetRecord, 813)
	MisResultAction(AddExp, 6900, 6900)
	MisResultAction(AddMoney,469,469)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 88 )	
	TriggerAction( 1, AddNextFlag, 813, 10, 10 )
	RegCurTrigger( 8131 )

-----------------------------------
	DefineMission( 814, "Ninja Sword", 814 )
	
	MisBeginTalk( "<t><rNinja Mole> uses a very unique weapon. They called it the <yNinja Sword>.<n><t>It is a highly accurate and damaging weapon which I am very interested in.<n><t>Can you go to <pAbandoned Mine 2> and get 10 <yNinja Swords> for me?")
	MisBeginCondition(LvCheck, ">", 33 )
	MisBeginCondition(NoMission, 814)
	MisBeginCondition(NoRecord, 814)
	MisBeginAction(AddMission, 814)
	MisBeginAction(AddTrigger, 8141, TE_GETITEM, 3935, 10 )
	MisCancelAction(ClearMission, 814)

	MisNeed(MIS_NEED_ITEM, 3935, 10, 10, 10)
	
	MisHelpTalk("<t>Have you obtain 10 <yNinja Swords>?")
	MisResultTalk("<t>Oh! Thank you! I love this <yNinja Sword >!")
	MisResultCondition(NoRecord, 814)
	MisResultCondition(HasMission, 814)
	MisResultCondition(HasItem, 3935, 10)
	MisResultAction(TakeItem, 3935, 10 )
	MisResultAction(ClearMission, 814)
	MisResultAction(SetRecord, 814)
	MisResultAction(AddExp, 8600, 8600)
	MisResultAction(AddMoney,972,972)


	InitTrigger()
	TriggerCondition( 1, IsItem, 3935 )	
	TriggerAction( 1, AddNextFlag, 814, 10, 10 )
	RegCurTrigger( 8141 )

-----------------------------------
	DefineMission( 837, "Test", 837 )
	
	MisBeginTalk( "<ts>ince this is your first time to Chaldea, I am sure that you are looking around for a challenge.<n><t>Why don't you kill 5 <rTribal Warriors> to prove your valor? They are at the southeast of this Haven.")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(NoMission, 837)
	MisBeginCondition(NoRecord, 837)
	MisBeginAction(AddMission, 837)
	MisBeginAction(AddTrigger, 8371, TE_KILL, 248, 5 )
	MisCancelAction(ClearMission, 837)

	MisNeed(MIS_NEED_DESP,"Help <bSimon Gilter> at <j(646, 2088)> to get rid of 5 <rTribal Warriors>.")
	MisNeed(MIS_NEED_KILL, 248, 5, 10, 5)

	MisHelpTalk("Its only 5 <rTribal Warriors>! What are you waiting for? Go!")
	MisResultTalk("Good! At least you are not a coward.")
	MisResultCondition(NoRecord, 837)
	MisResultCondition(HasMission, 837)
	MisResultCondition(HasFlag, 837, 14)
	MisResultAction(ClearMission, 837)
	MisResultAction(SetRecord, 837)
	MisResultAction(AddExp,15692,15692)
	MisResultAction(AddMoney,1026,1026)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 248 )	
	TriggerAction( 1, AddNextFlag, 837, 10, 5 )
	RegCurTrigger( 8371 )


-----------------------------------
	DefineMission( 838, "Test", 838 )
	
	MisBeginTalk( "<t>Killing 5 <rTribal Warriors> is easy. Can you kill 30 <rTribal Warriors>?<n><t>You can reject if you are afraid.")
	--MisBeginCondition(LvCheck, ">", 40 )
	MisBeginCondition(HasRecord, 837)
	MisBeginCondition(NoMission, 838)
	MisBeginCondition(NoRecord, 838)
	MisBeginAction(AddMission, 838)
	MisBeginAction(AddTrigger, 8381, TE_KILL, 248, 30 )
	MisCancelAction(ClearMission, 838)
	
	MisNeed(MIS_NEED_DESP,"Help <bSimon Gilter> at <j(646, 2088)> to get rid of 30 <rTribal Warriors>.")
	MisNeed(MIS_NEED_KILL, 248, 30, 10, 30)
	
	MisHelpTalk("The target is to kill 30 <rTribal Warriors>. Are you afraid?")
	MisResultTalk("You are great! This is the reward you deserve!")
	MisResultCondition(NoRecord, 838)
	MisResultCondition(HasMission, 838)
	MisResultCondition(HasFlag, 838, 39)
	MisResultAction(ClearMission, 838)
	MisResultAction(SetRecord, 838)
	MisResultAction(AddExp,15692,15692)
	MisResultAction(AddMoney,1026,1026)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 248 )	
	TriggerAction( 1, AddNextFlag, 838, 10, 30 )
	RegCurTrigger( 8381 )


-----------------------------------
	DefineMission( 839, "Hidden Agenda", 839 )
	
	MisBeginTalk( "<t>Good! Good! I can see your potiential from the previous tasks that I've entrust you to. I believe that you have also collected some <yTribal Shields> as well.<n><t>Are you willing to give me 12 <yTribal Shields> for my collection?")
	--MisBeginCondition(LvCheck, ">", 40 )
	MisBeginCondition(HasRecord, 838)
	MisBeginCondition(NoMission, 839)
	MisBeginCondition(NoRecord, 839)
	MisBeginAction(AddMission, 839)
	MisBeginAction(AddTrigger, 8391, TE_GETITEM, 4914, 12 )
	MisCancelAction(ClearMission, 839)
	
	MisNeed(MIS_NEED_DESP,"Bring back 12 <yTribal Shields> for <bSimon Gilter> at <j(646, 2088)>.")
	MisNeed(MIS_NEED_ITEM, 4914, 12, 10, 12)
	
	MisHelpTalk("What are you waiting for? I need 12 <yTribal Shields>.")
	MisResultTalk("<t>Oh my! This is so beautifully crafted from the hands of those cruel <rTribal Warriors>.<n><t>Let me admire these...")
	MisResultCondition(NoRecord, 839)
	MisResultCondition(HasMission, 839)
	MisResultCondition(HasItem, 4914, 12)
	MisResultAction(TakeItem, 4914, 12)
	MisResultAction(ClearMission, 839)
	MisResultAction(SetRecord, 839)
	MisResultAction(AddExp,15692,15692)
	MisResultAction(AddMoney,1026,1026)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4914 )	
	TriggerAction( 1, AddNextFlag, 839, 10, 12 )
	RegCurTrigger( 8391 )



-----------------------------------
	DefineMission( 840, "Enthusiast", 840 )
	
	MisBeginTalk( "<t>Hi! I am a artist who carve figurines out of old roots. However, the forest nearby is full of monster and I cannot find any suitable roots for my art!<n><t>Please help me collect 5 <yWithered Roots>!")
	MisBeginCondition(LvCheck, ">", 42 )
	--MisBeginCondition(HasRecord, 838)
	MisBeginCondition(NoMission, 840)
	MisBeginCondition(NoRecord, 840)
	MisBeginAction(AddMission, 840)
	MisBeginAction(AddTrigger, 8401, TE_GETITEM, 4915, 5 )
	MisCancelAction(ClearMission, 840)
	
	MisNeed(MIS_NEED_DESP,"Bring 5 <yWithered Roots> to <bAzur Breeze> at <j(622, 2106)>.")
	MisNeed(MIS_NEED_ITEM, 4915, 5, 10, 5)
	
	MisHelpTalk("Thanks for your help. I need 5 <yWithered Roots>.")
	MisResultTalk("I can feel my inspiration coming now!")
	MisResultCondition(NoRecord, 840)
	MisResultCondition(HasMission, 840)
	MisResultCondition(HasItem, 4915, 5)
	MisResultAction(TakeItem, 4915, 5)
	MisResultAction(ClearMission, 840)
	MisResultAction(SetRecord, 840)
	MisResultAction(AddExp,19294,19294)	
	MisResultAction(AddMoney,1064,1064)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4915 )	
	TriggerAction( 1, AddNextFlag, 840, 10, 5 )
	RegCurTrigger( 8401 )

-----------------------------------
	DefineMission( 841, "Sampling", 841 )
	
	MisBeginTalk( "<t>Do you know how to make a candle of unending flame? It is made from a special root that can only be found in the forest east of this haven. However, there are many monsters along the way.<n><t>Could you kindly collect 1 <yWithered Root> and kill 5 <rTreants> please?")
	--MisBeginCondition(LvCheck, ">", 42 )
	MisBeginCondition(HasRecord, 844)
	MisBeginCondition(NoMission, 841)
	MisBeginCondition(NoRecord, 841)
	MisBeginAction(AddMission, 841)
	MisBeginAction(AddTrigger, 8411, TE_GETITEM, 4915, 1 )
	MisBeginAction(AddTrigger, 8412, TE_KILL, 107, 5 )
	MisCancelAction(ClearMission, 841)
	
	MisNeed(MIS_NEED_DESP,"Kill 5 Treant and bring 1 <yWithered Root> back to <bSa Mori> at <j(628, 2095)>.")
	MisNeed(MIS_NEED_ITEM, 4915, 1, 20, 1)
	MisNeed(MIS_NEED_KILL, 107, 5, 10, 5)
	
	MisHelpTalk("Kill 5 <rTreants> and find 1 <yWithered Root> for me")
	MisResultTalk("This is just what I needed!")
	MisResultCondition(NoRecord, 841)
	MisResultCondition(HasMission, 841)
	MisResultCondition(HasItem, 4915, 1)
	MisResultCondition(HasFlag, 841, 14)
	MisResultAction(TakeItem, 4915, 1)
	MisResultAction(ClearMission, 841)
	MisResultAction(SetRecord, 841)
	MisResultAction(AddExp,19294,19294)
	MisResultAction(AddMoney,1064,1064)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4915 )	
	TriggerAction( 1, AddNextFlag, 841, 20, 1 )
	RegCurTrigger( 8411 )
	InitTrigger()
	TriggerCondition( 1, IsMonster,	107 )	
	TriggerAction( 1, AddNextFlag, 841, 10, 5 )
	RegCurTrigger( 8412 )

-----------------------------------
	DefineMission( 842, "Never Dying Candle", 842 )
	
	MisBeginTalk( "<t>I have just completed the research of making a candle of unending flame. I am prepared to start mass producing these candles.<n><t>Can you get me 15 <yWithered Roots>? I promise to share the profits with you.")
	--MisBeginCondition(LvCheck, ">", 42 )
	MisBeginCondition(HasRecord, 841)
	MisBeginCondition(NoMission, 842)
	MisBeginCondition(NoRecord, 842)
	MisBeginAction(AddMission, 842)
	MisBeginAction(AddTrigger, 8421, TE_GETITEM, 4915, 15 )
	MisCancelAction(ClearMission, 842)
	
	MisNeed(MIS_NEED_DESP,"Bring 15 <yWithered Roots> for <bSa Mori> at <j(628, 2095)>.")
	MisNeed(MIS_NEED_ITEM, 4915, 15, 10, 15)
	
	MisHelpTalk("I need 15 <yWithered Roots>. Have you gotten any?")
	MisResultTalk("Haha! I can start production with these roots right away! Thanks!")
	MisResultCondition(NoRecord, 842)
	MisResultCondition(HasMission, 842)
	MisResultCondition(HasItem, 4915, 15)
	MisResultAction(TakeItem, 4915, 15)
	MisResultAction(ClearMission, 842)
	MisResultAction(SetRecord, 842)
	MisResultAction(AddExp,19294,19294)	
	MisResultAction(AddMoney,1064,1064)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4915 )	
	TriggerAction( 1, AddNextFlag, 842, 10, 15 )
	RegCurTrigger( 8421 )

-----------------------------------
	DefineMission( 843, "Black Monster", 843 )
	
	MisBeginTalk( "<t>Recently, I saw a lot of dark monsters with candles on their head. Is it a new form of cult for the monster? I believe this isn't any good news. However, I am more interested in their candles.<n><t>Can you kill some <rDark Mud Monsters> and get me their <yUsed Candle>?")
	MisBeginCondition(LvCheck, ">", 42 )
	--MisBeginCondition(HasRecord, 844)
	MisBeginCondition(NoMission, 843)
	MisBeginCondition(NoRecord, 843)
	MisBeginAction(AddMission, 843)
	MisBeginAction(AddTrigger, 8431, TE_GETITEM, 4823, 1 )
	MisBeginAction(AddTrigger, 8432, TE_KILL, 503, 5 )
	MisCancelAction(ClearMission, 843)
	
	MisNeed(MIS_NEED_DESP,"Kill 5 <rDark Mud Monsters> and bring 1 <yUsed Candle> to <bSa Mori> at <j(628, 2095)>.")
	MisNeed(MIS_NEED_ITEM, 4823, 1, 20, 1)
	MisNeed(MIS_NEED_KILL, 503, 5, 10, 5)
	
	MisHelpTalk("<rDark Mud Monsters> are located north of this haven. Get me some <yUsed Candles> please.")
	MisResultTalk("This is weird. The candle burns without ever burning itself out! How mysterious!")
	MisResultCondition(NoRecord, 843)
	MisResultCondition(HasMission, 843)
	MisResultCondition(HasItem, 4823, 1)
	MisResultCondition(HasFlag, 843, 14)
	MisResultAction(TakeItem, 4823, 1)
	MisResultAction(ClearMission, 843)
	MisResultAction(SetRecord, 843)
	MisResultAction(AddExp,19294,19294)
	MisResultAction(AddMoney,1064,1064)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4823 )	
	TriggerAction( 1, AddNextFlag, 843, 20, 1 )
	RegCurTrigger( 8431 )
	InitTrigger()
	TriggerCondition( 1, IsMonster,	503 )	
	TriggerAction( 1, AddNextFlag, 843, 10, 5 )
	RegCurTrigger( 8432 )

-----------------------------------
	DefineMission( 844, "Candle Research", 844 )
	
	MisBeginTalk( "<t>Hey! You're still around! Thanks for your help the last time. I have finished up the <yUsed Candle> for research.<n><t>Can you get 10 more <yUsed Candles>?")
	--MisBeginCondition(LvCheck, ">", 42 )
	MisBeginCondition(HasRecord, 843)
	MisBeginCondition(NoMission, 844)
	MisBeginCondition(NoRecord, 844)
	MisBeginAction(AddMission, 844)
	MisBeginAction(AddTrigger, 8441, TE_GETITEM, 4823, 10 )
	MisCancelAction(ClearMission, 844)
	
	MisNeed(MIS_NEED_DESP,"<t>Help <bSa Mori> at <j(628, 2095)> to obtain 10 candles for the research")
	MisNeed(MIS_NEED_ITEM, 4823, 10, 10, 10)
	
	MisHelpTalk("I need 10 <yUsed Candles> for my research. Please hurry!")
	MisResultTalk("This is great! I can continue my research again!")
	MisResultCondition(NoRecord, 844)
	MisResultCondition(HasMission, 844)
	MisResultCondition(HasItem, 4823, 10)
	MisResultAction(TakeItem, 4823, 10)
	MisResultAction(ClearMission, 844)
	MisResultAction(SetRecord, 844)
	MisResultAction(AddExp,19294,19294)
	MisResultAction(AddMoney,1064,1064)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4823 )	
	TriggerAction( 1, AddNextFlag, 844, 10, 10 )
	RegCurTrigger( 8441 )


-----------------------------------
	DefineMission( 845, "Rock Golem", 845 )
	
	MisBeginTalk( "<t>Recently, <rSturdy Rock Golems> are attacking everybody they encounter and many villagers have been hurt.<n><t>Dear adventurer! Please help me dismantle some <rSturdy Rock Golems> and collect some sample back for investigation!")
	MisBeginCondition(LvCheck, ">", 45 )
	--MisBeginCondition(HasRecord, 844)
	MisBeginCondition(NoMission, 845)
	MisBeginCondition(NoRecord, 845)
	MisBeginAction(AddMission, 845)
	MisBeginAction(AddTrigger, 8451, TE_GETITEM, 4825, 1 )
	MisBeginAction(AddTrigger, 8452, TE_KILL, 505, 5 )
	MisCancelAction(ClearMission, 845)
	
	MisNeed(MIS_NEED_DESP,"Kill 5 <rSturdy Rock Golems> and bring 1 <yShimmering Rock Fragment> back to <bCarin Livingstone> at <j(626, 2100)>.")
	MisNeed(MIS_NEED_ITEM, 4825, 1, 20, 1)
	MisNeed(MIS_NEED_KILL, 505, 5, 10, 5)
	
	MisHelpTalk("You cannot find the <rSturdy Rock Golems>? They are further north of this haven.")
	MisResultTalk("You found this <yShimmering Rock Fragment> on them?")
	MisResultCondition(NoRecord, 845)
	MisResultCondition(HasMission, 845)
	MisResultCondition(HasItem, 4825, 1)
	MisResultCondition(HasFlag, 845, 14)
	MisResultAction(TakeItem, 4825, 1)
	MisResultAction(ClearMission, 845)
	MisResultAction(SetRecord, 845)
	MisResultAction(AddExp,26112,26112)	
	MisResultAction(AddMoney,1125,1125)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4825 )	
	TriggerAction( 1, AddNextFlag, 845, 20, 1 )
	RegCurTrigger( 8451 )
	InitTrigger()
	TriggerCondition( 1, IsMonster,	505 )	
	TriggerAction( 1, AddNextFlag, 845, 10, 5 )
	RegCurTrigger( 8452 )

-----------------------------------
	DefineMission( 846, "Velvet Stone Research", 846 )
	
	MisBeginTalk( "<ts>orry, I was too to notice you! Regarding the <yShimmering Rock Fragments> you brought back the other time, I accidentally lost it as it is too small.<n><t>Can you get me another 6 <yShimmering Rock Fragments>?")
	--MisBeginCondition(LvCheck, ">", 42 )
	MisBeginCondition(HasRecord, 845)
	MisBeginCondition(NoMission, 846)
	MisBeginCondition(NoRecord, 846)
	MisBeginAction(AddMission, 846)
	MisBeginAction(AddTrigger, 8461, TE_GETITEM, 4825, 6 )
	MisCancelAction(ClearMission, 846)
	
	MisNeed(MIS_NEED_DESP,"Bring 6 <yShimmering Rock Fragments> to <bCarin Livingstone> at <j(626, 2100)>.")
	MisNeed(MIS_NEED_ITEM, 4825, 6, 10, 6)
	
	MisHelpTalk("Have you found 6 <yShimmering Rock Fragments>?")
	MisResultTalk("I am sure I have seen these <yShimmering Rock Fragments> somewhere")
	MisResultCondition(NoRecord, 846)
	MisResultCondition(HasMission, 846)
	MisResultCondition(HasItem, 4825, 6)
	MisResultAction(TakeItem, 4825, 6)
	MisResultAction(ClearMission, 846)
	MisResultAction(SetRecord, 846)
	MisResultAction(AddExp,26112,26112)	
	MisResultAction(AddMoney,1125,1125)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4825 )	
	TriggerAction( 1, AddNextFlag, 846, 10, 6 )
	RegCurTrigger( 8461 )


-----------------------------------
	DefineMission( 847, "Validity", 847 )
	
	MisBeginTalk( "<t>I believed that I have found some evidence regarding the strange behavior of <rSturdy Rock Golems>.<n><t>However, I cannot confirm this and I need a <yUnyielding Helmet> from <rUndead Warrior>. Can you collect 1 <yUnyielding Helmet> and kill 5 <rUndead Warriors> for me?")
	--MisBeginCondition(LvCheck, ">", 45 )
	MisBeginCondition(HasRecord, 846)
	MisBeginCondition(NoMission, 847)
	MisBeginCondition(NoRecord, 847)
	MisBeginAction(AddMission, 847)
	MisBeginAction(AddTrigger, 8471, TE_GETITEM, 4917, 1 )
	MisBeginAction(AddTrigger, 8472, TE_KILL, 267, 5 )
	MisCancelAction(ClearMission, 847)
	
	MisNeed(MIS_NEED_DESP,"Kill 5 <rUndead Warriors> and bring their helment back to <bCarin Livingstone> at <j(626, 2100)>.")
	MisNeed(MIS_NEED_ITEM, 4917, 1, 20, 1)
	MisNeed(MIS_NEED_KILL, 267, 5, 10, 5)
	
	MisHelpTalk("Kill more <rUndead Warriors> please.")
	MisResultTalk("I knew you can do it! Pass me the <yUnyielding Helmet> now.")
	MisResultCondition(NoRecord, 847)
	MisResultCondition(HasMission, 847)
	MisResultCondition(HasItem, 4917, 1)
	MisResultCondition(HasFlag, 847, 14)
	MisResultAction(TakeItem, 4917, 1)
	MisResultAction(ClearMission, 847)
	MisResultAction(SetRecord, 847)
	MisResultAction(AddExp,28832,28832)
	MisResultAction(AddMoney,1146,1146)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4917 )	
	TriggerAction( 1, AddNextFlag, 847, 20, 1 )
	RegCurTrigger( 8471 )
	InitTrigger()
	TriggerCondition( 1, IsMonster,	267 )	
	TriggerAction( 1, AddNextFlag, 847, 10, 5 )
	RegCurTrigger( 8472 )


-----------------------------------
	DefineMission( 848, "Unyielding Helmet", 848 )
	
	MisBeginTalk( "<t>You've come at the right time! Look at this helmet!<n><t>I bet you cannot recognize it. It is the <yUnyielding Helmet> you bought me the other time. I have done some enhancements with the <yShimmering Rock Fragment> and this is the final product! Get me 10 more <yUnyielding Helmets> and I can start producing more!")
	--MisBeginCondition(LvCheck, ">", 42 )
	MisBeginCondition(HasRecord, 847)
	MisBeginCondition(NoMission, 848)
	MisBeginCondition(NoRecord, 848)
	MisBeginAction(AddMission, 848)
	MisBeginAction(AddTrigger, 8481, TE_GETITEM, 4917, 10 )
	MisCancelAction(ClearMission, 848)
	
	MisNeed(MIS_NEED_DESP,"Bring 10 Immortral Helms to <bCarin Livingstone> at <j(626, 2100)>.")
	MisNeed(MIS_NEED_ITEM, 4917, 10, 10, 10)
	
	MisHelpTalk("Have you found 10 <yUnyielding Helmets>? I am anxious.")
	MisResultTalk("With these, I can make more shimmering helmets! Hee! How can I allow <bSa Mori> at <j(628, 2095)> to reap the profits alone.")
	MisResultCondition(NoRecord, 848)
	MisResultCondition(HasMission, 848)
	MisResultCondition(HasItem, 4917, 10)
	MisResultAction(TakeItem, 4917, 10)
	MisResultAction(ClearMission, 848)
	MisResultAction(SetRecord, 848)
	MisResultAction(AddExp,28832,28832)
	MisResultAction(AddMoney,1146,1146)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4917 )	
	TriggerAction( 1, AddNextFlag, 848, 10, 10 )
	RegCurTrigger( 8481 )

-----------------------------------
	DefineMission( 849, "Disintegration", 849 )
	
	MisBeginTalk( "<t>Are you here for visiting? If yes, you have come at the wrong time. Thundoria is under seige from monsters, especially those <rElite <rWerewolf Warriors>>.<n><t>If possible, can you help us kill 20 <rElite <rWerewolf Warriors>> to reduce their strength?")
	MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(NoMission, 849)
	MisBeginCondition(NoRecord, 849)
	MisBeginAction(AddMission, 849)
	MisBeginAction(AddTrigger, 8491, TE_KILL, 513, 20 )
	MisCancelAction(ClearMission, 849)

	MisNeed(MIS_NEED_DESP,"Kill 20 <rElite <rWerewolf Warriors>> to prove your valor to <bColonel Maxi> at <j(653, 1505)>.")
	MisNeed(MIS_NEED_KILL, 513, 20, 10, 20)

	MisHelpTalk("Kill 20 <rElite <rWerewolf Warriors>> to prove your bravery!")
	MisResultTalk("Well done! You have successfully vanquished the enemies!")
	MisResultCondition(NoRecord, 849)
	MisResultCondition(HasMission, 849)
	MisResultCondition(HasFlag, 849, 29)
	MisResultAction(ClearMission, 849)
	MisResultAction(SetRecord, 849)
	MisResultAction(AddExp,31809,31809)
	MisResultAction(AddMoney,1167,1167)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 513 )	
	TriggerAction( 1, AddNextFlag, 849, 10, 20 )
	RegCurTrigger( 8491 )

-----------------------------------
	DefineMission( 850, "Bloodied Hammer", 850 )
	
	MisBeginTalk( "<t>I am a weapon collector who only collects high quality weapons.<n><t>Recently, I saw <rElite <rWerewolf Warriors>> wielding <yBloodied Hammers> as weapons. I wish to have them for my collection.<n><t>Please collect 10 <yBloodied Hammers> for me!")
	MisBeginCondition(LvCheck, ">", 48 )
	--MisBeginCondition(HasRecord, 847)
	MisBeginCondition(NoMission, 850)
	MisBeginCondition(NoRecord, 850)
	MisBeginAction(AddMission, 850)
	MisBeginAction(AddTrigger, 8501, TE_GETITEM, 4833, 10 )
	MisCancelAction(ClearMission, 850)
	
	MisNeed(MIS_NEED_DESP,"Bring 10 <yBloody Hammers> to <bWesley> at <j(741, 1563)>.")
	MisNeed(MIS_NEED_ITEM, 4833, 10, 10, 10)
	
	MisHelpTalk("I want 10 <yBloodied Hammers>! Get them to me quick!")
	MisResultTalk("Look at these <yBloodied Hammers>. Only weapons collected from battlefield are of value.")
	MisResultCondition(NoRecord, 850)
	MisResultCondition(HasMission, 850)
	MisResultCondition(HasItem, 4833, 10)
	MisResultAction(TakeItem, 4833, 10)
	MisResultAction(ClearMission, 850)
	MisResultAction(SetRecord, 850)
	MisResultAction(AddExp,35066,35066)	
	MisResultAction(AddMoney,1189,1189)



	InitTrigger()
	TriggerCondition( 1, IsItem, 4833 )	
	TriggerAction( 1, AddNextFlag, 850, 10, 10 )
	RegCurTrigger( 8501 )


-----------------------------------
	DefineMission( 851, "First Ingredient", 851 )
	
	MisBeginTalk( "<t>If you have no business here please leave. Do not disrupt my experiment! The people who share the same passion me have already died. But never mind, I just need a few important ingredients for my experiment to be a success!<n><t>Go get me 8 <yMummy Nails> for the first type of ingredients for the experiment now!")
	MisBeginCondition(LvCheck, ">", 47 )
	--MisBeginCondition(HasRecord, 847)
	MisBeginCondition(NoMission, 851)
	MisBeginCondition(NoRecord, 851)
	MisBeginAction(AddMission, 851)
	MisBeginAction(AddTrigger, 8511, TE_GETITEM, 4883, 8 )
	MisCancelAction(ClearMission, 851)
	
	MisNeed(MIS_NEED_DESP,"<bFreya> at <j(651, 1585)> requires 8 <yMummy Nails> for research")
	MisNeed(MIS_NEED_ITEM, 4883, 8, 10, 8)
	
	MisHelpTalk("8 <yMummy Nails> or you as my ingredient")
	MisResultTalk("Take your hands off, do not mess around with my experiment! You can go now.")
	MisResultCondition(NoRecord, 851)
	MisResultCondition(HasMission, 851)
	MisResultCondition(HasItem, 4883, 8)
	MisResultAction(TakeItem, 4883, 8)
	MisResultAction(ClearMission, 851)
	MisResultAction(SetRecord, 851)
	MisResultAction(AddExp,31809,31809)	
	MisResultAction(AddMoney,1167,1167)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4883 )	
	TriggerAction( 1, AddNextFlag, 851, 10, 8 )
	RegCurTrigger( 8511 )

-----------------------------------
	DefineMission( 852, "Vengence", 852 )
	
	MisBeginTalk( "Damn those <rIron Mummies>! They killed my wife and turned her into one of themselves! Because of this, I cannot bear to kill them as I do not know wish to harm what was formerly wife!<n><t>Please help me release her soul! Kill 15 <rIron Mummies> please!")
	MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(NoMission, 852)
	MisBeginCondition(NoRecord, 852)
	MisBeginAction(AddMission, 852)
	MisBeginAction(AddTrigger, 8521, TE_KILL, 41, 15 )
	MisCancelAction(ClearMission, 852)

	MisNeed(MIS_NEED_DESP,"Kill 15 <rIron Mummies> on behalf of <bMallack> at <j(706, 1465)>.")
	MisNeed(MIS_NEED_KILL, 41, 15, 10, 15)

	MisHelpTalk("Do you think <rIron Mummies> feel pain?")
	MisResultTalk("Thank you! Finally, my wife can rest in peace.")
	MisResultCondition(NoRecord, 852)
	MisResultCondition(HasMission, 852)
	MisResultCondition(HasFlag, 852, 24)
	MisResultAction(ClearMission, 852)
	MisResultAction(SetRecord, 852)
	MisResultAction(AddExp,31809,31809)	
	MisResultAction(AddMoney,1167,1167)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 41 )	
	TriggerAction( 1, AddNextFlag, 852, 10, 15 )
	RegCurTrigger( 8521 )

-----------------------------------
	DefineMission( 853, "Third Ingredient", 853 )
	
	MisBeginTalk( "<t>This is not the first time we're talking to each other so I'll spare you the formalities.<n><t>I need the third type of ingredients now. Get me 10 <yStramonium Thorn> now. If you don't know where to find, go north-west of the city and search.")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 854)
	MisBeginCondition(NoMission, 853)
	MisBeginCondition(NoRecord, 853)
	MisBeginAction(AddMission, 853)
	MisBeginAction(AddTrigger, 8531, TE_GETITEM, 4834, 10 )
	MisCancelAction(ClearMission, 853)
	
	MisNeed(MIS_NEED_DESP,"Collect 8 <yHuge Stramonium Thorns> as the 3rd ingredient for <bFreya>'s research at <j(651, 1585)>.")
	MisNeed(MIS_NEED_ITEM, 4834, 10, 10, 10)
	
	MisHelpTalk("Is collecting 10 <yHuge Stramonium Thorn> such a hard task for you?")
	MisResultTalk("Good! Two more to go before completion. You can go now. I'll call on you when I need help.")
	MisResultCondition(NoRecord, 853)
	MisResultCondition(HasMission, 853)
	MisResultCondition(HasItem, 4834, 10)
	MisResultAction(TakeItem, 4834, 10)
	MisResultAction(ClearMission, 853)
	MisResultAction(SetRecord, 853)
	MisResultAction(AddExp,35066,35066)	
	MisResultAction(AddMoney,1189,1189)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4834 )	
	TriggerAction( 1, AddNextFlag, 853, 10, 10 )
	RegCurTrigger( 8531 )

-----------------------------------
	DefineMission( 854, "Second Ingredient", 854 )
	
	MisBeginTalk( "<t>Just like what I've said before. I need a few ingredients for my experiment. <yMummy Nails> are just one of them.<n><t>I need the second type of ingredients now. Go get me 10 strands of <yMummy Hair>.<n><t>Remember! Only those from <rSteel Mummies>.")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 851)
	MisBeginCondition(NoMission, 854)
	MisBeginCondition(NoRecord, 854)
	MisBeginAction(AddMission, 854)
	MisBeginAction(AddTrigger, 8541, TE_GETITEM, 4884, 10 )
	MisCancelAction(ClearMission, 854)
	
	MisNeed(MIS_NEED_DESP,"<bFreya> at <j(651, 1585)> requires the 2nd ingredient. Get her 10 strands of <yMummy Hair>.")
	MisNeed(MIS_NEED_ITEM, 4884, 10, 10, 10)
	
	MisHelpTalk("This time round I need 10 strands of <yMummy Hair>. I will not repeat myself!")
	MisResultTalk("Don't move! Give me that strand of hair that's stuck on your clothes!")
	MisResultCondition(NoRecord, 854)
	MisResultCondition(HasMission, 854)
	MisResultCondition(HasItem, 4884, 10)
	MisResultAction(TakeItem, 4884, 10)
	MisResultAction(ClearMission, 854)
	MisResultAction(SetRecord, 854)
	MisResultAction(AddExp,35066,35066)
	MisResultAction(AddMoney,1189,1189)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4884 )	
	TriggerAction( 1, AddNextFlag, 854, 10, 10 )
	RegCurTrigger( 8541 )

-----------------------------------
	DefineMission( 855, "Repel Spirit", 855 )
	
	MisBeginTalk( "<t>My anger has ceased as I only wished for my wife to find peace in heaven. However, as long as those mummies exist, the villagers will never have a peaceful day.<n><t>Please kill 15 <rSteel Mummies>!")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 852)
	MisBeginCondition(NoMission, 855)
	MisBeginCondition(NoRecord, 855)
	MisBeginAction(AddMission, 855)
	MisBeginAction(AddTrigger, 8551, TE_KILL, 42, 15 )
	MisCancelAction(ClearMission, 855)

	MisNeed(MIS_NEED_DESP,"Help <bMallack> at <j(706, 1465)> to kill 15 <rSteel Mummies>.")
	MisNeed(MIS_NEED_KILL, 42, 15, 10, 15)

	MisHelpTalk("How is it? Did those <rSteel Mummies> get a taste of your power?")
	MisResultTalk("You've done it! I am impressed with your courage and skill.  Here is your reward.")
	MisResultCondition(NoRecord, 855)
	MisResultCondition(HasMission, 855)
	MisResultCondition(HasFlag, 855, 24)
	MisResultAction(ClearMission, 855)
	MisResultAction(SetRecord, 855)
	MisResultAction(AddExp,35066,35066)	
	MisResultAction(AddMoney,1189,1189)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 42 )	
	TriggerAction( 1, AddNextFlag, 855, 10, 15 )
	RegCurTrigger( 8551 )

-----------------------------------
	DefineMission( 856, "Tribal Invasion", 856 )
	
	MisBeginTalk( "<t><rElite <rWerewolf Warriors>> have steered clear of our city for now. However, the <rtribesmen> are still lurking around and disrupting our peace!<n><t>Help us by killing 5 <rTribal Shamans> and 15 <rAgile Tribal Villagers>.")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 849)
	MisBeginCondition(NoMission, 856)
	MisBeginCondition(NoRecord, 856)
	MisBeginAction(AddMission, 856)
	MisBeginAction(AddTrigger, 8561, TE_KILL, 515, 15 )
	MisBeginAction(AddTrigger, 8562, TE_KILL, 38, 5 )
	MisCancelAction(ClearMission, 856)

	MisNeed(MIS_NEED_DESP,"Get rid of 5 <rTribal Shamans> and 15 <rAgile Tribal Villagers> and report back to <bColonel Maxi> at <j(653, 1505)>.")
	MisNeed(MIS_NEED_KILL, 515, 15, 10, 15)
	MisNeed(MIS_NEED_KILL, 38, 5, 30, 5)

	MisHelpTalk("Those <rtribesmen> are still taunting the city, what are you doing here?")
	MisResultTalk("Haha! Lets see what those <rtribesmen> do can now!")
	MisResultCondition(NoRecord, 856)
	MisResultCondition(HasMission, 856)
	MisResultCondition(HasFlag, 856, 24)
	MisResultCondition(HasFlag, 856, 34)
	MisResultAction(ClearMission, 856)
	MisResultAction(SetRecord, 856)
	MisResultAction(AddExp,38628,38628)
	MisResultAction(AddMoney,1212,1212)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 515 )	
	TriggerAction( 1, AddNextFlag, 856, 10, 15 )
	RegCurTrigger( 8561 )
	InitTrigger()
	TriggerCondition( 1, IsMonster, 38 )	
	TriggerAction( 1, AddNextFlag, 856, 30, 5 )
	RegCurTrigger( 8562 )

-----------------------------------
	DefineMission( 857, "Battle Reward", 857 )
	
	MisBeginTalk( "<t>You tell me that you are the hero that defeated those <rElite <rWerewolf Warriors>> and <rtribesmen>?<n><t>Don't make me laugh! Show me proof then! Bring me 5 <yTribal Masks> and 5 <yMysterious Bones> as evident!")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 856)
	MisBeginCondition(NoMission, 857)
	MisBeginCondition(NoRecord, 857)
	MisBeginAction(AddMission, 857)
	MisBeginAction(AddTrigger, 8571, TE_GETITEM, 4919, 5 )
	MisBeginAction(AddTrigger, 8572, TE_GETITEM, 4835, 5 )
	MisCancelAction(ClearMission, 857)
	
	MisNeed(MIS_NEED_DESP,"<t><bGuard - Sonny> at <j(711, 1414)> requires 5 <yTribal Masks> and 5 <yMysterious Bones>. Bring to him to prove your worth")
	MisNeed(MIS_NEED_ITEM, 4919, 5, 10, 5)
	MisNeed(MIS_NEED_ITEM, 4835, 5, 20, 5)
	
	MisHelpTalk("Go away! You do not have what I wanted yet!")
	MisResultTalk("You are really a hero! This is what you deserved!")
	MisResultCondition(NoRecord, 857)
	MisResultCondition(HasMission, 857)
	MisResultCondition(HasItem, 4919, 5)
	MisResultCondition(HasItem, 4835, 5)
	MisResultAction(TakeItem, 4835, 5)
	MisResultAction(TakeItem, 4919, 5)
	MisResultAction(ClearMission, 857)
	MisResultAction(SetRecord, 857)
	MisResultAction(AddExp,42522,42522)	
	MisResultAction(AddMoney,1235,1235)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4919 )	
	TriggerAction( 1, AddNextFlag, 857, 10, 5 )
	RegCurTrigger( 8571 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4835 )	
	TriggerAction( 1, AddNextFlag, 857, 20, 5 )
	RegCurTrigger( 8572 )

-----------------------------------
	DefineMission( 858, "New Weapon", 858 )
	
	MisBeginTalk( "<t>As the war with the monsters continues, we need to invent new weapons to deal with them effectively. We need 1 <yBroken Angel Wand> and 1 <yDented Sacred Bow> for the research.<n><t>Can you get them from the <rGuardian Angels> and <rForest Hunters>?")
	--MisBeginCondition(LvCheck, ">", 47 )
	MisBeginCondition(HasRecord, 856)
	MisBeginCondition(NoMission, 858)
	MisBeginCondition(NoRecord, 858)
	MisBeginAction(AddMission, 858)
	MisBeginAction(AddTrigger, 8581, TE_GETITEM, 4918, 1 )
	MisBeginAction(AddTrigger, 8582, TE_GETITEM, 4921, 1 )
	MisCancelAction(ClearMission, 858)
	
	MisNeed(MIS_NEED_DESP,"Get <bColonel Maxi> at <j(653, 1505)> the required <yBroken Angel Wand> and <yDented Sacred Bow> for the weapon research")
	MisNeed(MIS_NEED_ITEM, 4918, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 4921, 1, 20, 1)
	
	MisHelpTalk("I need a <yBroken Angel Wand> and a <yDented Sacred Bow>.")
	MisResultTalk("Although these weapons are damaged, it is more than enough for our research.")
	MisResultCondition(NoRecord, 858)
	MisResultCondition(HasMission, 858)
	MisResultCondition(HasItem, 4918, 1)
	MisResultCondition(HasItem, 4921, 1)
	MisResultAction(TakeItem, 4921, 1)
	MisResultAction(TakeItem, 4918, 1)
	MisResultAction(ClearMission, 858)
	MisResultAction(SetRecord, 858)
	MisResultAction(AddExp,42522,42522)	
	MisResultAction(AddMoney,1235,1235)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4918 )	
	TriggerAction( 1, AddNextFlag, 858, 10, 1 )
	RegCurTrigger( 8581 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4921 )	
	TriggerAction( 1, AddNextFlag, 858, 20, 1 )
	RegCurTrigger( 8582 )

-----------------------------------
	DefineMission( 859, "Fallen Angel", 859 )
	
	MisBeginTalk( "<t>Have you seen the <rGuardian Angels> outside our city?<n><t>Do you think that they are even fit to be called angels? Please put these fallen angels to rest.")
	MisBeginCondition(LvCheck, ">", 50 )
	--MisBeginCondition(HasRecord, 849)
	MisBeginCondition(NoMission, 859)
	MisBeginCondition(NoRecord, 859)
	MisBeginAction(AddMission, 859)
	MisBeginAction(AddTrigger, 8591, TE_KILL, 284, 12 )
	MisCancelAction(ClearMission, 859)

	MisNeed(MIS_NEED_DESP,"Kill 12 <rGuardian Angels> and report back to <bGuard Nisson> at <j(711, 1414)>.")
	MisNeed(MIS_NEED_KILL, 284, 12, 10, 12)

	MisHelpTalk("Go forth and put those fallen angels to eternal rest.")
	MisResultTalk("Poor angels! I hope that they can return to heaven one day.")
	MisResultCondition(NoRecord, 859)
	MisResultCondition(HasMission, 859)
	MisResultCondition(HasFlag, 859, 21)
	MisResultAction(ClearMission, 859)
	MisResultAction(SetRecord, 859)
	MisResultAction(AddExp,42522,42522)
	MisResultAction(AddMoney,1235,1235)

	InitTrigger()
	TriggerCondition( 1, IsMonster, 284 )	
	TriggerAction( 1, AddNextFlag, 859, 10, 12 )
	RegCurTrigger( 8591 )

-----------------------------------
	DefineMission( 860, "Expedition", 860 )
	
	MisBeginTalk( "<t>The monsters outside the city have been defeated. However, enemies hiding in the <bSacred Snow Mountain> are still on the move. We will organise another war expedition! Are there any brave man who are willing to join?<n><t>Our targets this time round are the <rUndead Warrior> and <rSkeletal Archer>.")
	MisBeginCondition(LvCheck, ">", 51 )
	MisBeginCondition(HasRecord, 858)
	MisBeginCondition(NoMission, 860)
	MisBeginCondition(NoRecord, 860)
	MisBeginAction(AddMission, 860)
	MisBeginAction(AddTrigger, 8601, TE_KILL, 521, 8 )
	MisBeginAction(AddTrigger, 8602, TE_KILL, 541, 8 )
	MisCancelAction(ClearMission, 860)

	MisNeed(MIS_NEED_DESP,"Kill 8 <rVicious Undead Warriors> and 8 <rDeadly Skeletal Archers> and report back to <bColonel Maxi> at <j(653, 1505)>.")
	MisNeed(MIS_NEED_KILL, 521, 8, 10, 8)
	MisNeed(MIS_NEED_KILL, 541, 8, 30, 8)

	MisHelpTalk("Why are you still here? The battle horn has already been sounded!")
	MisResultTalk("We were succesfully thanks to you, our hero!")
	MisResultCondition(NoRecord, 860)
	MisResultCondition(HasMission, 860)
	MisResultCondition(HasFlag, 860, 17)
	MisResultCondition(HasFlag, 860, 37)
	MisResultAction(ClearMission, 860)
	MisResultAction(SetRecord, 860)
	MisResultAction(AddExp,46776,46776)
	MisResultAction(AddMoney,1258,1258)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 521 )	
	TriggerAction( 1, AddNextFlag, 860, 10, 8 )
	RegCurTrigger( 8601 )
	InitTrigger()
	TriggerCondition( 1, IsMonster, 541 )	
	TriggerAction( 1, AddNextFlag, 860, 30, 8 )
	RegCurTrigger( 8602 )

-----------------------------------
	DefineMission( 861, "Fallen Helmet", 861 )
	
	MisBeginTalk( "<t>Did you participate in the war expedition recently?<n><t>The <rVicious Undead Warriors> that were killed drop <yFallen Helmets> which is just what I've been looking for. Can you get me 5 <yFallen Helmets>?")
	MisBeginCondition(LvCheck, ">", 51 )
	MisBeginCondition(HasRecord, 850)
	MisBeginCondition(NoMission, 861)
	MisBeginCondition(NoRecord, 861)
	MisBeginAction(AddMission, 861)
	MisBeginAction(AddTrigger, 8611, TE_GETITEM, 4837, 5 )
	MisCancelAction(ClearMission, 861)
	
	MisNeed(MIS_NEED_DESP,"Help <bWesley> at <j(741, 1563)> to collect 5 <yFallen Helms>.")
	MisNeed(MIS_NEED_ITEM, 4837, 5, 10, 5)
	
	MisHelpTalk("<yFallen Helmet>! <yFallen Helmet>! How many times do I have to repeat myself?")
	MisResultTalk("They are a such beauties! Haha!")
	MisResultCondition(NoRecord, 861)
	MisResultCondition(HasMission, 861)
	MisResultCondition(HasItem, 4837, 5)
	MisResultAction(TakeItem, 4837, 5)
	MisResultAction(ClearMission, 861)
	MisResultAction(SetRecord, 861)
	MisResultAction(AddExp,46776,46776)
	MisResultAction(AddMoney,1258,1258)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4837 )	
	TriggerAction( 1, AddNextFlag, 861, 10, 5 )
	RegCurTrigger( 8611 )


-----------------------------------
	DefineMission( 862, "Fourth Ingredient", 862 )
	
	MisBeginTalk( "<t>The fourth type of ingredient is a <yComplete Rib>. It can only be found off <rDeadly Skeletal Archers>.<n><t>Not a single person has returned from their lair. Are you brave enough to bring me their <yComplete Ribs>?")
	MisBeginCondition(LvCheck, ">", 52 )
	MisBeginCondition(HasRecord, 853)
	MisBeginCondition(NoMission, 862)
	MisBeginCondition(NoRecord, 862)
	MisBeginAction(AddMission, 862)
	MisBeginAction(AddTrigger, 8621, TE_GETITEM, 4858, 8 )
	MisCancelAction(ClearMission, 862)
	
	MisNeed(MIS_NEED_DESP,"<bMorpheus - Freya> at <j(651, 1585)> requires a new ingredient now. Get her 8 <yComplete Ribs>...")
	MisNeed(MIS_NEED_ITEM, 4858, 8, 10, 8)
	
	MisHelpTalk("You dare return empty handed? Maybe your ribs will do as well")
	MisResultTalk("I am impressed with your capability. Good!")
	MisResultCondition(NoRecord, 862)
	MisResultCondition(HasMission, 862)
	MisResultCondition(HasItem, 4858, 8)
	MisResultAction(TakeItem, 4858, 8)
	MisResultAction(ClearMission, 862)
	MisResultAction(SetRecord, 862)
	MisResultAction(AddExp,51423,51423)
	MisResultAction(AddMoney,1282,1282)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4858 )	
	TriggerAction( 1, AddNextFlag, 862, 10, 8 )
	RegCurTrigger( 8621 )

-----------------------------------
	DefineMission( 863, "Test of the Sacred Forest", 863 )
	
	MisBeginTalk( "<t>There is a challenge waiting for you. Are you brave enough to defeat 10 <rForest Hunters>? May the goddess be with you!")
	MisBeginCondition(LvCheck, ">", 52 )
	--MisBeginCondition(HasRecord, 863)
	MisBeginCondition(NoMission, 863)
	MisBeginCondition(NoRecord, 863)
	MisBeginAction(AddMission, 863)
	MisBeginAction(AddTrigger, 8631, TE_KILL, 261, 10 )
	MisCancelAction(ClearMission, 863)

	MisNeed(MIS_NEED_DESP,"Kill 10 <rForest Hunters> to pass the test of <bCindy> at <j(740, 1451)>.")
	MisNeed(MIS_NEED_KILL, 261, 10, 10, 10)

	MisHelpTalk("Are you still hesitating? If you want to give up there is still time.")
	MisResultTalk("Congratulations! You have passed the test!")
	MisResultCondition(NoRecord, 863)
	MisResultCondition(HasMission, 863)
	MisResultCondition(HasFlag, 863, 19)
	MisResultAction(ClearMission, 863)
	MisResultAction(SetRecord, 863)
	MisResultAction(AddExp,51423,51423)
	MisResultAction(AddMoney,1282,1282)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 261 )	
	TriggerAction( 1, AddNextFlag, 863, 10, 10 )
	RegCurTrigger( 8631 )


-----------------------------------
	DefineMission( 864, "Last Ingredient", 864 )
	
	MisBeginTalk( "<t>You can't give up now! It's already too late! I will kill you if you dare! However, if you help me, I will share the secret of immortality with you.<n><t>I need the last type of ingredient now! Get me 10 <ySkeleton Bone Fragments> from <rCursed Corpse>!")
	MisBeginCondition(LvCheck, ">", 53 )
	MisBeginCondition(HasRecord, 862)
	MisBeginCondition(NoMission, 864)
	MisBeginCondition(NoRecord, 864)
	MisBeginAction(AddMission, 864)
	MisBeginAction(AddTrigger, 8641, TE_GETITEM, 4886, 10 )
	MisCancelAction(ClearMission, 864)
	
	MisNeed(MIS_NEED_DESP,"Bring 10 <ySkeleton Bone Fragments> for <bFreya> at <j(651, 1585)>.")
	MisNeed(MIS_NEED_ITEM, 4886, 10, 10, 10)
	
	MisHelpTalk("I need 10 <ySkeleton Bone Fragments>!")
	MisResultTalk("Nobody can stop me now! I will reach immortality soon!")
	MisResultCondition(NoRecord, 864)
	MisResultCondition(HasMission, 864)
	MisResultCondition(HasItem, 4886, 10)
	MisResultAction(TakeItem, 4886, 10)
	MisResultAction(ClearMission, 864)
	MisResultAction(SetRecord, 864)
	MisResultAction(AddExp,56496,56496)	
	MisResultAction(AddMoney,1306,1306)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4886 )	
	TriggerAction( 1, AddNextFlag, 864, 10, 10 )
	RegCurTrigger( 8641 )

-----------------------------------
	DefineMission( 865, "Hidden Shadow", 865 )
	
	MisBeginTalk( "<t>I have been investigating the case about those mummies and have found out that the <rCursed Corpses> have been manipulating them all along.<n><t>Please put a stop to this evil and defeat those <rCursed Corpses>!")
	MisBeginCondition(LvCheck, ">", 53 )
	MisBeginCondition(HasRecord, 855)
	MisBeginCondition(NoMission, 865)
	MisBeginCondition(NoRecord, 865)
	MisBeginAction(AddMission, 865)
	MisBeginAction(AddTrigger, 8651, TE_KILL, 52, 10 )
	MisCancelAction(ClearMission, 865)

	MisNeed(MIS_NEED_DESP,"Kill 10 <rCursed Corpse> and report back to <bGuard Captain Mallack> at <j(706, 1465)>.")
	MisNeed(MIS_NEED_KILL, 52, 10, 10, 10)

	MisHelpTalk("Can you handle them? I am worried about the safety of our city.")
	MisResultTalk("May light shine upon us now that those <rCursed Corpses> are destroyed!")
	MisResultCondition(NoRecord, 865)
	MisResultCondition(HasMission, 865)
	MisResultCondition(HasFlag, 865, 19)
	MisResultAction(ClearMission, 865)
	MisResultAction(SetRecord, 865)
	MisResultAction(AddExp,56496,56496)	
	MisResultAction(AddMoney,1306,1306)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 52 )	
	TriggerAction( 1, AddNextFlag, 865, 10, 10 )
	RegCurTrigger( 8651 )

-----------------------------------
	DefineMission( 866, "Last Enemy", 866 )
	
	MisBeginTalk( "<t>The war expedition has been a success. Now the enemies have gathered near the western shores. However, we are unable to break through their defense.<n><t>Maybe you can give it a try by killing 10 <rSkeletal Warrior Leaders>.")
	MisBeginCondition(LvCheck, ">", 53 )
	MisBeginCondition(HasRecord, 860)
	MisBeginCondition(NoMission, 866)
	MisBeginCondition(NoRecord, 866)
	MisBeginAction(AddMission, 866)
	MisBeginAction(AddTrigger, 8661, TE_KILL, 565, 10 )
	MisBeginAction(AddTrigger, 8662, TE_GETITEM, 4879, 1 )
	MisCancelAction(ClearMission, 866)

	MisNeed(MIS_NEED_DESP,"Kill 10 <rSkeletal Warrior Leader> and bring 1 <yWarrior Leader Token> to <bColonel Maxi> at <j(653, 1505)> to prove your bravery")
	MisNeed(MIS_NEED_KILL, 565, 10, 10, 10)
	MisNeed(MIS_NEED_ITEM, 4879, 1, 20, 1)

	MisHelpTalk("It is understandable that you are afraid. Those <rSkeletal Warrior Leaders> are merciless.")
	MisResultTalk("The last of the enemies have been destroyed by you! You are our war hero!")
	MisResultCondition(NoRecord, 866)
	MisResultCondition(HasMission, 866)
	MisResultCondition(HasFlag, 866, 19)
	MisResultCondition(HasItem, 4879, 1)
	MisResultAction(TakeItem, 4879, 1)
	MisResultAction(ClearMission, 866)
	MisResultAction(SetRecord, 866)
	MisResultAction(AddExp,56496,56496)
	MisResultAction(AddMoney,1306,1306)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 565 )	
	TriggerAction( 1, AddNextFlag, 866, 10, 10 )
	RegCurTrigger( 8661 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4879 )	
	TriggerAction( 1, AddNextFlag, 866, 20, 1 )
	RegCurTrigger( 8662 )

-----------------------------------
	DefineMission( 867, "Phantom Blood Test", 867 )
	
	MisBeginTalk( "<t>Forsake the light and dwell into the darkness of your soul! Nobody possesses the power that you have within. Embrace darkness and release the power now! Defeat 15 <rBloodthirsty Hunters> to prove that you deserve the power!")
	MisBeginCondition(LvCheck, ">", 54 )
	--MisBeginCondition(HasRecord, 860)
	MisBeginCondition(NoMission, 867)
	MisBeginCondition(NoRecord, 867)
	MisBeginAction(AddMission, 867)
	MisBeginAction(AddTrigger, 8671, TE_KILL, 666, 15 )
	MisCancelAction(ClearMission, 867)

	MisNeed(MIS_NEED_DESP,"Kill 15 <rBloodthirsty Hunters> to pass the test of <bCindy> at <j(740, 1451)>.")
	MisNeed(MIS_NEED_KILL, 666, 15, 10, 15 )

	MisHelpTalk("Is there still light in your heart? Its not too late to give up.")
	MisResultTalk("You will walk in darkness and remain invincible from now on!")
	MisResultCondition(NoRecord, 867)
	MisResultCondition(HasMission, 867)
	MisResultCondition(HasFlag, 867, 24)
	MisResultAction(ClearMission, 867)
	MisResultAction(SetRecord, 867)
	MisResultAction(AddExp,62032,62032)
	MisResultAction(AddMoney,1331,1331)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 666 )	
	TriggerAction( 1, AddNextFlag, 867, 10, 15 )
	RegCurTrigger( 8671 )

-----------------------------------
	DefineMission( 868, "Dark Bow", 868 )
	
	MisBeginTalk( "<t><yBloodied Hammer>, <yFallen Helmet> and others are only rubbish. If you ask me, the only treasure will be a <yDark Bow>.<n><t>If you can help me obtain 5 <yDark Bows>, I will reward you greatly!")
	MisBeginCondition(LvCheck, ">", 54 )
	MisBeginCondition(HasRecord, 861)
	MisBeginCondition(NoMission, 868)
	MisBeginCondition(NoRecord, 868)
	MisBeginAction(AddMission, 868)
	MisBeginAction(AddTrigger, 8681, TE_GETITEM, 4922, 5 )
	MisCancelAction(ClearMission, 868)
	
	MisNeed(MIS_NEED_DESP,"<bWesley> at <j(741, 1563)> requires 5 <yDark Bows>.")
	MisNeed(MIS_NEED_ITEM, 4922, 5, 10, 5)
	
	MisHelpTalk("I ask for nothing else but <yDark Bow>. Please get me 5")
	MisResultTalk("This mysterious darkness is a form of beauty")
	MisResultCondition(NoRecord, 868)
	MisResultCondition(HasMission, 868)
	MisResultCondition(HasItem, 4922, 5)
	MisResultAction(TakeItem, 4922, 5)
	MisResultAction(ClearMission, 868)
	MisResultAction(SetRecord, 868)
	MisResultAction(AddExp,62032,62032)	
	MisResultAction(AddMoney,1331,1331)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4922 )	
	TriggerAction( 1, AddNextFlag, 868, 10, 5 )
	RegCurTrigger( 8681 )


-----------------------------------
	DefineMission( 869, "Repel Snowman", 869 )
	
	MisBeginTalk( "<t>There was a recent attack by the snowman. Although there were similar incidents in the past, this attack was well organised for some unknown reason. Will you help us defend against these snowman?<n><t>Of course, we will let you handle weaker ones first. The <rcumbersome snowman> appears at <j(2471, 502)>. So, are you willing to take up the job?")
	MisBeginCondition(LvCheck, ">", 35 )
	--MisBeginCondition(HasRecord, 860)
	MisBeginCondition(NoMission, 869)
	MisBeginCondition(NoRecord, 869)
	MisBeginAction(AddMission, 869)
	MisBeginAction(AddTrigger, 8691, TE_KILL, 516, 15 )
	MisCancelAction(ClearMission, 869)

	MisNeed(MIS_NEED_DESP,"Kill 15 <rcumbersome snowman> and report back to <bFardey> at <j(2680, 657)>.")
	MisNeed(MIS_NEED_KILL, 516, 15, 10, 15 )

	MisHelpTalk("Why are you still here? The <rCumbersome Snowmen> are already invading our haven!")
	MisResultTalk("I can sense your courage from the way you repel those <rSnowmen>. Well done!")
	MisResultCondition(NoRecord, 869)
	MisResultCondition(HasMission, 869)
	MisResultCondition(HasFlag, 869, 24)
	MisResultAction(ClearMission, 869)
	MisResultAction(SetRecord, 869)
	MisResultAction(AddExp,9170,9170)	
	MisResultAction(AddMoney,939,939)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 516 )	
	TriggerAction( 1, AddNextFlag, 869, 10, 15 )
	RegCurTrigger( 8691 )

-----------------------------------
	DefineMission( 870, "Werewolves Invasion", 870 )
	
	MisBeginTalk( "<t>It seems to be season of the werewolves again. Children will not be safe with them around. Please help us get ird of these <rWerewolf Warriors>!<n><t>They can be found near <j(2580, 553)>.")
	MisBeginCondition(LvCheck, ">", 36 )
	--MisBeginCondition(HasRecord, 860)
	MisBeginCondition(NoMission, 870)
	MisBeginCondition(NoRecord, 870)
	MisBeginAction(AddMission, 870)
	MisBeginAction(AddTrigger, 8701, TE_KILL, 271, 15 )
	MisCancelAction(ClearMission, 870)

	MisNeed(MIS_NEED_DESP,"Help <bNeila> at <j(2675, 631)> to kill 15 <rWerewolf Warriors>.")
	MisNeed(MIS_NEED_KILL, 271, 15, 10, 15 )

	MisHelpTalk("Those werewolves are west of this Haven. Hurry up!")
	MisResultTalk("Thank you for keeping our children safe!")
	MisResultCondition(NoRecord, 870)
	MisResultCondition(HasMission, 870)
	MisResultCondition(HasFlag, 870, 24)
	MisResultAction(ClearMission, 870)
	MisResultAction(SetRecord, 870)
	MisResultAction(AddExp,10238,10238)
	MisResultAction(AddMoney,955,955)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 271 )	
	TriggerAction( 1, AddNextFlag, 870, 10, 15 )
	RegCurTrigger( 8701 )

-----------------------------------
	DefineMission( 871, "Rubbish Collection", 871 )
	
	MisBeginTalk( "<t>Don't belittle this pile of junk for they are worth quite a bit of money. I will give you a reward if you bring me 5 <yRusty Broadswords>, <yBroken Hammers> and <ySlipshod Wooden Sticks> each.<n><t>You can get them from the <rWerewolves> at <j(2580, 553)>, the <rCumbersome Yeti> at <j(2811, 565)> and the <rHorrific Snowman> at <j(2471, 502)>.")
	MisBeginCondition(LvCheck, ">", 36 )
	--MisBeginCondition(HasRecord, 861)
	MisBeginCondition(NoMission, 871)
	MisBeginCondition(NoRecord, 871)
	MisBeginAction(AddMission, 871)
	MisBeginAction(AddTrigger, 8711, TE_GETITEM, 4836, 5 )
	MisBeginAction(AddTrigger, 8712, TE_GETITEM, 4907, 5 )
	MisBeginAction(AddTrigger, 8713, TE_GETITEM, 4838, 5 )
	MisCancelAction(ClearMission, 871)
	
	MisNeed(MIS_NEED_DESP,"Help <bMekkilon> at <j(2662, 648)> to collect 5 <yRusty Broadswords>, 5 <yBroken Hammers> and 5 <ySlipshod Wooden Sticks>.")
	MisNeed(MIS_NEED_ITEM, 4836, 5, 10, 5)
	MisNeed(MIS_NEED_ITEM, 4907, 5, 10, 5)
	MisNeed(MIS_NEED_ITEM, 4838, 5, 10, 5)
	
	MisHelpTalk("If you cannot find those stuff I wanted, I will be very disappointed.")
	MisResultTalk("Haha! Now I can use these to cheat <bWesley>!")
	MisResultCondition(NoRecord, 871)
	MisResultCondition(HasMission, 871)
	MisResultCondition(HasItem, 4836, 5)
	MisResultCondition(HasItem, 4907, 5)
	MisResultCondition(HasItem, 4838, 5)
	MisResultAction(TakeItem, 4836, 5)
	MisResultAction(TakeItem, 4907, 5)
	MisResultAction(TakeItem, 4838, 5)
	MisResultAction(ClearMission, 871)
	MisResultAction(SetRecord, 871)
	MisResultAction(AddExp,10238,10238)	
	MisResultAction(AddMoney,955,955)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4836 )	
	TriggerAction( 1, AddNextFlag, 871, 10, 5 )
	RegCurTrigger( 8711 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4907 )	
	TriggerAction( 1, AddNextFlag, 871, 20, 5 )
	RegCurTrigger( 8712 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4838 )	
	TriggerAction( 1, AddNextFlag, 871, 30, 5 )
	RegCurTrigger( 8713 )


-----------------------------------
	DefineMission( 872, "Upgrade Battle", 872 )
	
	MisBeginTalk( "<t>The war has escalated to another level! Now we are facing attacks from ferocious <rSnowmen>!<n><t>Would you accept the task to kill 15 <rSnowmen>?<n><t>They are camping at <j(2587, 455)>.")
	MisBeginCondition(LvCheck, ">", 37 )
	MisBeginCondition(HasRecord, 869)
	MisBeginCondition(NoMission, 872)
	MisBeginCondition(NoRecord, 872)
	MisBeginAction(AddMission, 872)
	MisBeginAction(AddTrigger, 8721, TE_KILL, 194, 15 )
	MisCancelAction(ClearMission, 872)

	MisNeed(MIS_NEED_DESP,"<bFardey> at <j(2680, 657)> requires you to kill 15 <rSnowmen>.")
	MisNeed(MIS_NEED_KILL, 194, 15, 10, 15 )

	MisHelpTalk("War is merciless. You have to kill your enemies before they kill you.")
	MisResultTalk("Thank you for not letting us down!")
	MisResultCondition(NoRecord, 872)
	MisResultCondition(HasMission, 872)
	MisResultCondition(HasFlag, 872, 24)
	MisResultAction(ClearMission, 872)
	MisResultAction(SetRecord, 872)
	MisResultAction(AddExp,11413,11413)	
	MisResultAction(AddMoney,972,972)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 194 )	
	TriggerAction( 1, AddNextFlag, 872, 10, 15 )
	RegCurTrigger( 8721 )

-----------------------------------
	DefineMission( 873, "Giant Broadsword", 873 )
	
	MisBeginTalk( "<t>Remember me? Now I need 5 <yGiant Broadswords>. Get them for me and I shall reward you.<n><t>They can be found on the <rSnowmen> at <j(2587, 455)>.")
	MisBeginCondition(LvCheck, ">", 38 )
	MisBeginCondition(HasRecord, 871)
	MisBeginCondition(NoMission, 873)
	MisBeginCondition(NoRecord, 873)
	MisBeginAction(AddMission, 873)
	MisBeginAction(AddTrigger, 8731, TE_GETITEM, 4861, 5 )
	MisCancelAction(ClearMission, 873)
	
	MisNeed(MIS_NEED_DESP,"Collect 5 <yGiant Broadswords> for <bMekkilon> at <j(2662, 648)>.")
	MisNeed(MIS_NEED_ITEM, 4861, 5, 10, 5)
	
	MisHelpTalk("Bring me 5 <yGiant Broadswords> or you can forget about coming back.")
	MisResultTalk("These <yGiant Broadswords> will make good money. Haha!")
	MisResultCondition(NoRecord, 873)
	MisResultCondition(HasMission, 873)
	MisResultCondition(HasItem, 4861, 5)
	MisResultAction(TakeItem, 4861, 5)
	MisResultAction(ClearMission, 873)
	MisResultAction(SetRecord, 873)
	MisResultAction(AddExp,11413,11413)
	MisResultAction(AddMoney,972,972)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4861 )	
	TriggerAction( 1, AddNextFlag, 873, 10, 5 )
	RegCurTrigger(8731)

-----------------------------------
	DefineMission( 874, "Repel Yeti", 874 )
	
	MisBeginTalk( "<t>You might have defeated those <rSnowmen>. However, these <rCumbersome Yetis> are far more formidable compare to them.<n><t>We are training new recruits to kill these <rCumbersome Yetis>. Show off your skill by killing 12 <rCumbersome Yetis> at <j(2811, 565)>.")
	MisBeginCondition(LvCheck, ">", 38 )
	MisBeginCondition(HasRecord, 872)
	MisBeginCondition(NoMission, 874)
	MisBeginCondition(NoRecord, 874)
	MisBeginAction(AddMission, 874)
	MisBeginAction(AddTrigger, 8741, TE_KILL, 517, 12 )
	MisCancelAction(ClearMission, 874)

	MisNeed(MIS_NEED_DESP,"Kill 12 <rCumbersome Yeti> and report back to <bFardey> at <j(2680, 657)>.")
	MisNeed(MIS_NEED_KILL, 517, 12, 10, 12 )

	MisHelpTalk("<rCumbersome Yetis> are more agile than those <rSnowmen>. Beware!")
	MisResultTalk("There is nothing to be proud of. More challenges await you!")
	MisResultCondition(NoRecord, 874)
	MisResultCondition(HasMission, 874)
	MisResultCondition(HasFlag, 874, 21)
	MisResultAction(ClearMission, 874)
	MisResultAction(SetRecord, 874)
	MisResultAction(AddExp,12706,12706)
	MisResultAction(AddMoney,990,990)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 517 )	
	TriggerAction( 1, AddNextFlag, 874, 10, 12 )
	RegCurTrigger( 8741 )

-----------------------------------
	DefineMission( 875, "Snail Hunting", 875 )
	
	MisBeginTalk( "<t>The supplies we have here come from hunting. However, it is not as easy as you think.<n><t>Do you want to join us? Hunt down 20 <rSteel-Shell Snails> at <j(2733, 651)>.")
	MisBeginCondition(LvCheck, ">", 38 )
	--MisBeginCondition(HasRecord, 872)
	MisBeginCondition(NoMission, 875)
	MisBeginCondition(NoRecord, 875)
	MisBeginAction(AddMission, 875)
	MisBeginAction(AddTrigger, 8751, TE_KILL, 501, 20 )
	MisBeginAction(AddTrigger, 8752, TE_GETITEM, 4821, 6 )
	MisCancelAction(ClearMission, 875)

	MisNeed(MIS_NEED_DESP,"Kill 20 <rSteel-Shell Snails> and bring back 6 of their <ySteel Feelers >to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 501, 20, 10, 20 )
	MisNeed(MIS_NEED_ITEM, 4821, 6, 40, 6 )

	MisHelpTalk("Its only some snail. Bring their <ySteel Feelers >to me.")
	MisResultTalk("Good! This show that you have the potential to be a hunter.")
	MisResultCondition(NoRecord, 875)
	MisResultCondition(HasMission, 875)
	MisResultCondition(HasFlag, 875, 29)
	MisResultCondition(HasItem, 4821, 6)
	MisResultAction(TakeItem, 4821, 6)
	MisResultAction(ClearMission, 875)
	MisResultAction(SetRecord, 875)
	MisResultAction(AddExp,12706,12706)
	MisResultAction(AddMoney,990,990)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 501 )	
	TriggerAction( 1, AddNextFlag, 875, 10, 20 )
	RegCurTrigger( 8751 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4821 )	
	TriggerAction( 1, AddNextFlag, 875, 40, 6 )
	RegCurTrigger( 8752 )

-----------------------------------
	DefineMission( 876, "New Invader", 876 )
	
	MisBeginTalk( "<t>Just after the <rWerewolf Warriors> attacks died down, the <rUndead Archers> started a new wave of attack! Is this the curse of <pIcespire Haven>?<n><t>Help us again! Please defeat those invader at <j(2746, 451)>!")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(HasRecord, 870)
	MisBeginCondition(NoMission, 876)
	MisBeginCondition(NoRecord, 876)
	MisBeginAction(AddMission, 876)
	MisBeginAction(AddTrigger, 8761, TE_KILL, 270, 12 )
	MisCancelAction(ClearMission, 876)

	MisNeed(MIS_NEED_DESP,"Defend the Haven by killing 12 <rUndead Archers> and report back to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 270, 12, 10, 12 )

	MisHelpTalk("Beware of those <rUndead Archers>. They are very accurate!")
	MisResultTalk("Those band of <rUndead Archers> escaping are comical. Thanks for your help.")
	MisResultCondition(NoRecord, 876)
	MisResultCondition(HasMission, 876)
	MisResultCondition(HasFlag, 876, 21)
	MisResultAction(ClearMission, 876)
	MisResultAction(SetRecord, 876)
	MisResultAction(AddExp,14128,14128)	
	MisResultAction(AddMoney,1008,1008)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 270 )	
	TriggerAction( 1, AddNextFlag, 876, 10, 12 )
	RegCurTrigger( 8761 )

-----------------------------------
	DefineMission( 877, "Skeleton Accessory", 877 )
	
	MisBeginTalk( "<t>The arrows of <rUndead Archers> at <j(2746, 451)> seems to be decorated with some beautiful bones.<n><t>Can you get me 10 <yDeath Arrows> for collection?")
	MisBeginCondition(LvCheck, ">", 39 )
	--MisBeginCondition(HasRecord, 871)
	MisBeginCondition(NoMission, 877)
	MisBeginCondition(NoRecord, 877)
	MisBeginAction(AddMission, 877)
	MisBeginAction(AddTrigger, 8771, TE_GETITEM, 4911, 10 )
	MisCancelAction(ClearMission, 877)
	
	MisNeed(MIS_NEED_DESP,"Collect 10 <yDeath Arrows> for <bSasha> at <j(2678, 631)>.")
	MisNeed(MIS_NEED_ITEM, 4911, 10, 10, 10)
	
	MisHelpTalk("I need those <yDeath Arrows> to decorate my table.")
	MisResultTalk("This fits so beautifully! Thanks!")
	MisResultCondition(NoRecord, 877)
	MisResultCondition(HasMission, 877)
	MisResultCondition(HasItem, 4911, 10)
	MisResultAction(TakeItem, 4911, 10)
	MisResultAction(ClearMission, 877)
	MisResultAction(SetRecord, 877)
	MisResultAction(AddExp,14128,14128)	
	MisResultAction(AddMoney,1008,1008)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4911 )	
	TriggerAction( 1, AddNextFlag, 877, 10, 10 )
	RegCurTrigger(8771)

-----------------------------------
	DefineMission( 878, "Deep Venture", 878 )
	
	MisBeginTalk( "<t>The training of the recruits are over. We are now going to real battle. We will venture deeper to destroy the <rSnow Yetis>.<n><t>Are you with us? You will have to kill <rSnow Yetis> at <j(2855, 451)>.")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(HasRecord, 874)
	MisBeginCondition(NoMission, 878)
	MisBeginCondition(NoRecord, 878)
	MisBeginAction(AddMission, 878)
	MisBeginAction(AddTrigger, 8781, TE_KILL, 195, 12 )
	MisCancelAction(ClearMission, 878)

	MisNeed(MIS_NEED_DESP,"Kill 12 <rSnow Yetis> and report back <bFardey> at <j(2680, 657)> for a reward.")
	MisNeed(MIS_NEED_KILL, 195, 12, 10, 12 )

	MisHelpTalk("Set out if you are ready.")
	MisResultTalk("You are daring to venture so deeply.")
	MisResultCondition(NoRecord, 878)
	MisResultCondition(HasMission, 878)
	MisResultCondition(HasFlag, 878, 21)
	MisResultAction(ClearMission, 878)
	MisResultAction(SetRecord, 878)
	MisResultAction(AddExp,14128,14128)
	MisResultAction(AddMoney,1008,1008)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 195 )	
	TriggerAction( 1, AddNextFlag, 878, 10, 12 )
	RegCurTrigger( 8781 )

-----------------------------------
	DefineMission( 879, "Giant Wooden Stick", 879 )
	
	MisBeginTalk( "<t>I heard that you are going to attack those <rSnow Yetis>. Can you bring 10 <yGiant Wooden Sticks> back for me? They usually appear at <j(2855, 451)>.")
	MisBeginCondition(LvCheck, ">", 39 )
	MisBeginCondition(HasRecord, 873)
	MisBeginCondition(NoMission, 879)
	MisBeginCondition(NoRecord, 879)
	MisBeginAction(AddMission, 879)
	MisBeginAction(AddTrigger, 8791, TE_GETITEM, 4862, 10 )
	MisCancelAction(ClearMission, 879)
	
	MisNeed(MIS_NEED_DESP,"Help <bMekkilon> at <j(2662, 648)> to collect 10 <yGiant Wooden Sticks>.")
	MisNeed(MIS_NEED_ITEM, 4862, 10, 10, 10)
	
	MisHelpTalk("Have you gotten hold of any <yGiant Wooden Sticks>?")
	MisResultTalk("I can foresee shining coins in my pocket soon!")
	MisResultCondition(NoRecord, 879)
	MisResultCondition(HasMission, 879)
	MisResultCondition(HasItem, 4862, 10)
	MisResultAction(TakeItem, 4862, 10)
	MisResultAction(ClearMission, 879)
	MisResultAction(SetRecord, 879)
	MisResultAction(AddExp,14128,14128)
	MisResultAction(AddMoney,1008,1008)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4862 )	
	TriggerAction( 1, AddNextFlag, 879, 10, 10 )
	RegCurTrigger(8791)

-----------------------------------
	DefineMission( 880, "Enemy's Rear", 880 )
	
	MisBeginTalk( "<t>You have been of great help to us all! We don't wish to trouble you anymore. However, we do not have the strength to fight anymore and <rElite Skeletal Archers> are camping nearby waiting to attack us. Can you kill those <rElite Skeletal Archers> for us again?")
	MisBeginCondition(LvCheck, ">", 41 )
	MisBeginCondition(HasRecord, 876)
	MisBeginCondition(NoMission, 880)
	MisBeginCondition(NoRecord, 880)
	MisBeginAction(AddMission, 880)
	MisBeginAction(AddTrigger, 8801, TE_KILL, 502, 12 )
	MisCancelAction(ClearMission, 880)

	MisNeed(MIS_NEED_DESP,"<bNeila> at <j(2675, 631)> requires you to kill 12 <rElite Skeletal Archers>.")
	MisNeed(MIS_NEED_KILL, 502, 12, 10, 12 )

	MisHelpTalk("Have you got any <yRuptured Ribs>?")
	MisResultTalk("These remind me of the good old days. Ah...")
	MisResultCondition(NoRecord, 880)
	MisResultCondition(HasMission, 880)
	MisResultCondition(HasFlag, 880, 21)
	MisResultAction(ClearMission, 880)
	MisResultAction(SetRecord, 880)
	MisResultAction(AddExp,17409,17409)
	MisResultAction(AddMoney,1045,1045)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 502 )	
	TriggerAction( 1, AddNextFlag, 880, 10, 12 )
	RegCurTrigger( 8801 )

-----------------------------------
	DefineMission( 881, "Glory of the Past", 881 )
	
	MisBeginTalk( "<t>Have you seen the <rElite Skeletal Archers>?<n><t>In the past, they used to be no match for me. But now, I am just a fragile old man. Can you do me a favor and get me some of their <yRuptured Ribs> as a memento?")
	MisBeginCondition(LvCheck, ">", 41 )
	--MisBeginCondition(HasRecord, 873)
	MisBeginCondition(NoMission, 881)
	MisBeginCondition(NoRecord, 881)
	MisBeginAction(AddMission, 881)
	MisBeginAction(AddTrigger, 8811, TE_GETITEM, 4822, 8 )
	MisCancelAction(ClearMission, 881)
	
	MisNeed(MIS_NEED_DESP,"Bring back 8 <yRuptured Ribs> to <bKevin Wolf> at <j(2688, 628)> to fulfill his wish")
	MisNeed(MIS_NEED_ITEM, 4822, 8, 10, 8)
	
	MisHelpTalk("Have you got any <yRuptured Ribs>?")
	MisResultTalk("These remind me of the good old days. Ah...")
	MisResultCondition(NoRecord, 881)
	MisResultCondition(HasMission, 881)
	MisResultCondition(HasItem, 4822, 8)
	MisResultAction(TakeItem, 4822, 8)
	MisResultAction(ClearMission, 881)
	MisResultAction(SetRecord, 881)
	MisResultAction(AddExp,17409,17409)
	MisResultAction(AddMoney,1045,1045)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4822 )	
	TriggerAction( 1, AddNextFlag, 881, 10, 8 )
	RegCurTrigger(8811)

----------------------------------
	DefineMission( 882, "Hunt for Infant Dragon", 882 )
	
	MisBeginTalk( "<t>You have done well on the last trip. This time we are going to hunt <rInfant Icy Dragons>. You have to be very careful if not you will become the hunted instead. Kill 10 <rInfant Icy Dragons> and get 1 <yHeart of Ice Crystal Fragment>.")
	MisBeginCondition(LvCheck, ">", 41 )
	MisBeginCondition(HasRecord, 875)
	MisBeginCondition(NoMission, 882)
	MisBeginCondition(NoRecord, 882)
	MisBeginAction(AddMission, 882)
	MisBeginAction(AddTrigger, 8821, TE_KILL, 530, 10 )
	MisBeginAction(AddTrigger, 8822, TE_GETITEM, 4850, 1 )
	MisCancelAction(ClearMission, 882)

	MisNeed(MIS_NEED_DESP,"Hunt 10 <rInfant Icy Dragons> and bring 1 <yHeart of Ice Crystal Fragment> back to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 530, 10, 10, 10 )
	MisNeed(MIS_NEED_ITEM, 4850, 1, 20, 1 )

	MisHelpTalk("How is the hunting going?")
	MisResultTalk("This is a beautiful fragment! I guess this is what <bSasha> wanted all along")
	MisResultCondition(NoRecord, 882)
	MisResultCondition(HasMission, 882)
	MisResultCondition(HasFlag, 882, 19)
	MisResultCondition(HasItem, 4850, 1)
	MisResultAction(TakeItem, 4850, 1)
	MisResultAction(ClearMission, 882)
	MisResultAction(SetRecord, 882)
	MisResultAction(AddExp,17409,17409)
	MisResultAction(AddMoney,1045,1045)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 530 )	
	TriggerAction( 1, AddNextFlag, 882, 10, 10 )
	RegCurTrigger( 8821 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4850 )	
	TriggerAction( 1, AddNextFlag, 882, 20, 1 )
	RegCurTrigger( 8822 )

-----------------------------------
	DefineMission( 883, "Puzzle of the Crystalline", 883 )
	
	MisBeginTalk( "<t>I heard that you participated in the hunt for <rInfant Icy Dragons>. I believed that you have seen the <yHeart of Ice Crystal Fragment>.<n><t>If its possible, can you collect 5 <yHeart of Ice Crystal Fragments> for me?")
	MisBeginCondition(LvCheck, ">", 41 )
	MisBeginCondition(HasRecord, 882)
	MisBeginCondition(NoMission, 883)
	MisBeginCondition(NoRecord, 883)
	MisBeginAction(AddMission, 883)
	MisBeginAction(AddTrigger, 8831, TE_GETITEM, 4850, 5 )
	MisCancelAction(ClearMission, 883)
	
	MisNeed(MIS_NEED_DESP,"Bring 5 <yHeart of Ice Crystal Fragment> to <bSasha> at <j(2678, 631)>.")
	MisNeed(MIS_NEED_ITEM, 4850, 5, 10, 5)
	
	MisHelpTalk("Have you get me what you have promised?")
	MisResultTalk("The <yHeart of Ice Crystal Fragments> look so pure and beautiful")
	MisResultCondition(NoRecord, 883)
	MisResultCondition(HasMission, 883)
	MisResultCondition(HasItem, 4850, 5)
	MisResultAction(TakeItem, 4850, 5)
	MisResultAction(ClearMission, 883)
	MisResultAction(SetRecord, 883)
	MisResultAction(AddExp,17409,17409)	
	MisResultAction(AddMoney,1045,1045)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4850 )	
	TriggerAction( 1, AddNextFlag, 883, 10, 5 )
	RegCurTrigger(8831)


-----------------------------------
	DefineMission( 884, "Crystal Heart", 884 )
	
	MisBeginTalk( "<t>As stated in the legends, combiningg the <yHeart of Ice Crystal Fragment> with a <yPure Crystal> yields a <yCrystalline Heart>. The <yCrystalline Heart> is a symbol of eternal purity.<n><t>Can you get 5 <yPure Crystals> from the <rWater Fairies>?")
	MisBeginCondition(LvCheck, ">", 43 )
	MisBeginCondition(HasRecord, 883)
	MisBeginCondition(NoMission, 884)
	MisBeginCondition(NoRecord, 884)
	MisBeginAction(AddMission, 884)
	MisBeginAction(AddTrigger, 8841, TE_GETITEM, 4895, 5 )
	MisCancelAction(ClearMission, 884)
	
	MisNeed(MIS_NEED_DESP,"Bring <bSasha> at <j(2678, 631)> 5 <yPure Crystals>.")
	MisNeed(MIS_NEED_ITEM, 4895, 5, 10, 5)
	
	MisHelpTalk("You promised to get me <yPure Crystals>. Do not fall back on your promise please.")
	MisResultTalk("This way when combine the pieces together, I can get the <yCrystalline Heart> that I had always dreamed for.")
	MisResultCondition(NoRecord, 884)
	MisResultCondition(HasMission, 884)
	MisResultCondition(HasItem, 4895, 5)
	MisResultAction(TakeItem, 4895, 5)
	MisResultAction(ClearMission, 884)
	MisResultAction(SetRecord, 884)
	MisResultAction(AddExp,21361,21361)	
	MisResultAction(AddMoney,1084,1084)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4895 )	
	TriggerAction( 1, AddNextFlag, 884, 10, 5 )
	RegCurTrigger(8841)

-----------------------------------
	DefineMission( 885, "Hunt for Polar Bear", 885 )
	
	MisBeginTalk( "<t>Listen to me! In the past, many have perished while hunting ferocious <rGreat Polar Bears>. Those who are change their mind may withdraw now! If not, go forth and hunt these beast down!")
	MisBeginCondition(LvCheck, ">", 43 )
	MisBeginCondition(HasRecord, 882)
	MisBeginCondition(NoMission, 885)
	MisBeginCondition(NoRecord, 885)
	MisBeginAction(AddMission, 885)
	MisBeginAction(AddTrigger, 8851, TE_KILL, 504, 8 )
	MisBeginAction(AddTrigger, 8852, TE_GETITEM, 4824, 3 )
	MisCancelAction(ClearMission, 885)

	MisNeed(MIS_NEED_DESP,"Kill 8 <rGreat Polar Bears> and bring back 3 <yRazor Bear Tooths> to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 504, 8, 10, 8 )
	MisNeed(MIS_NEED_ITEM, 4824, 3, 20, 3 )

	MisHelpTalk("Have you seen the <rGreat Polar Bears>? They are only fierce on the outside.")
	MisResultTalk("Look at this, it's a <yRazor Bear Tooth>! Many have been slain by it.")
	MisResultCondition(NoRecord, 885)
	MisResultCondition(HasMission, 885)
	MisResultCondition(HasFlag, 885, 17)
	MisResultCondition(HasItem, 4824, 3)
	MisResultAction(TakeItem, 4824, 3)
	MisResultAction(ClearMission, 885)
	MisResultAction(SetRecord, 885)
	MisResultAction(AddExp,21361,21361)	
	MisResultAction(AddMoney,1084,1084)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 504 )	
	TriggerAction( 1, AddNextFlag, 885, 10, 8 )
	RegCurTrigger( 8851 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4824 )	
	TriggerAction( 1, AddNextFlag, 885, 20, 3 )
	RegCurTrigger( 8852 )

-----------------------------------
	DefineMission( 886, "Snowman Warlord", 886 )
	
	MisBeginTalk( "<t>Although the battle with <rSnowmen> is still fresh in our mind. This time, we will have to deal with <rHorrific Snowmen>.<n><t>Let us set forth and end this once and for all! Kill 16 <rHorrific Snowmans>!")
	MisBeginCondition(LvCheck, ">", 44 )
	MisBeginCondition(HasRecord, 872)
	MisBeginCondition(NoMission, 886)
	MisBeginCondition(NoRecord, 886)
	MisBeginAction(AddMission, 886)
	MisBeginAction(AddTrigger, 8861, TE_KILL, 194, 16 )
	MisCancelAction(ClearMission, 886)

	MisNeed(MIS_NEED_DESP,"Help <bFardey> at <j(2680, 657)> to defeat 16 <rSnowmen>.")
	MisNeed(MIS_NEED_KILL, 194, 16, 10, 16 )

	MisHelpTalk(" Do not show mercy to the <rHorrific Snowmen>!")
	MisResultTalk("You have done what others did not have the courage to achieve. Brave indeed!")
	MisResultCondition(NoRecord, 886)
	MisResultCondition(HasMission, 886)
	MisResultCondition(HasFlag, 886, 25)
	MisResultAction(ClearMission, 886)
	MisResultAction(SetRecord, 886)
	MisResultAction(AddExp,23628,23628)	
	MisResultAction(AddMoney,1104,1104)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 194 )	
	TriggerAction( 1, AddNextFlag, 886, 10, 16 )
	RegCurTrigger( 8861 )

-----------------------------------
	DefineMission( 887, "Fearsome Sword of Giant", 887 )
	
	MisBeginTalk( "<t>Hoho! Its you again!<n><ts>ince you have fought with those <rHorrific Snowmen>, can you get me 5 <yFearsome Sword of Giant>?")
	MisBeginCondition(LvCheck, ">", 43 )
	MisBeginCondition(HasRecord, 883)
	MisBeginCondition(NoMission, 887)
	MisBeginCondition(NoRecord, 887)
	MisBeginAction(AddMission, 887)
	MisBeginAction(AddTrigger, 8871, TE_GETITEM, 4910, 5 )
	MisCancelAction(ClearMission, 887)
	
	MisNeed(MIS_NEED_DESP,"Collect 5 <yFearsome Swords of Giant> for <bMekkilon> at <j(2662, 648)>.")
	MisNeed(MIS_NEED_ITEM, 4910, 5, 10, 5)
	
	MisHelpTalk("Bring me 5 <yFearsome Sword of Giant> and I will pay you.")
	MisResultTalk("So... This is the legendary <yFearsome Sword of Giant> they used")
	MisResultCondition(NoRecord, 887)
	MisResultCondition(HasMission, 887)
	MisResultCondition(HasItem, 4910, 5)
	MisResultAction(TakeItem, 4910, 5)
	MisResultAction(ClearMission, 887)
	MisResultAction(SetRecord, 887)
	MisResultAction(AddExp,23628,23628)	
	MisResultAction(AddMoney,1104,1104)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4910 )	
	TriggerAction( 1, AddNextFlag, 887, 10, 5 )
	RegCurTrigger(8871)

-----------------------------------
	DefineMission( 888, "Werewolf Archer", 888 )
	
	MisBeginTalk( "<t>There is a cluster of <rWerewolf Archers> gathering near the outside of our village. Whatever their motives are, it can't be good.<n><t>Please bring along your weapon and teach them a lesson!")
	MisBeginCondition(LvCheck, ">", 45 )
	MisBeginCondition(HasRecord, 880)
	MisBeginCondition(NoMission, 888)
	MisBeginCondition(NoRecord, 888)
	MisBeginAction(AddMission, 888)
	MisBeginAction(AddTrigger, 8881, TE_KILL, 272, 20 )
	MisBeginAction(AddTrigger, 8882, TE_GETITEM, 4916, 5 )
	MisCancelAction(ClearMission, 888)

	MisNeed(MIS_NEED_DESP,"Kill 20 <rWerewolf Archers> and bring back 5 <yBroken Werewolf Bows> to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 272, 20, 10, 20 )
	MisNeed(MIS_NEED_ITEM, 4916, 5, 40, 5 )

	MisHelpTalk("Have you dealt with them yet?")
	MisResultTalk("You have done us another favor! Thank you!")
	MisResultCondition(NoRecord, 888)
	MisResultCondition(HasMission, 888)
	MisResultCondition(HasFlag, 888, 29)
	MisResultCondition(HasItem, 4916, 5)
	MisResultAction(TakeItem, 4916, 5)
	MisResultAction(ClearMission, 888)
	MisResultAction(SetRecord, 888)
	MisResultAction(AddExp,26112,26112)
	MisResultAction(AddMoney,1125,1125)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 272 )	
	TriggerAction( 1, AddNextFlag, 888, 10, 20 )
	RegCurTrigger( 8881 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4916 )	
	TriggerAction( 1, AddNextFlag, 888, 40, 5 )
	RegCurTrigger( 8882 )

-----------------------------------
	DefineMission( 889, "Last Yeti", 889 )
	
	MisBeginTalk( "<t>We are now facing the greatest enemy of all time, the <rHorrific Yetis>. Nobody has survived an attack from it before.<n><t>Will you be the first? Bring your weapon and go forth!")
	MisBeginCondition(LvCheck, ">", 45 )
	MisBeginCondition(HasRecord, 878)
	MisBeginCondition(NoMission, 889)
	MisBeginCondition(NoRecord, 889)
	MisBeginAction(AddMission, 889)
	MisBeginAction(AddTrigger, 8891, TE_KILL, 545, 15 )
	MisCancelAction(ClearMission, 889)

	MisNeed(MIS_NEED_DESP,"Kill 15 <rHorrific Yetis> and return to <bFardey> at <j(2680, 657)> for a reward.")
	MisNeed(MIS_NEED_KILL, 545, 15, 10, 15 )

	MisHelpTalk("Please give up while there is still time!")
	MisResultTalk("When the last of the <rHorrific Yetis> fall, nothing in this world will be able to stop you.")
	MisResultCondition(NoRecord, 889)
	MisResultCondition(HasMission, 889)
	MisResultCondition(HasFlag, 889, 24)
	MisResultAction(ClearMission, 889)
	MisResultAction(SetRecord, 889)
	MisResultAction(AddExp,26112,26112)
	MisResultAction(AddMoney,1125,1125)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 545 )	
	TriggerAction( 1, AddNextFlag, 889, 10, 15 )
	RegCurTrigger( 8891 )

-----------------------------------
	DefineMission( 890, "Fearsome Staff of Giant", 890 )
	
	MisBeginTalk( "<t>I'm surprised that you are still alive. Have you brought back 5 <yFearsome Staff of Giant>? I will buy them from you.")
	MisBeginCondition(LvCheck, ">", 45 )
	MisBeginCondition(HasRecord, 887)
	MisBeginCondition(NoMission, 890)
	MisBeginCondition(NoRecord, 890)
	MisBeginAction(AddMission, 890)
	MisBeginAction(AddTrigger, 8901, TE_GETITEM, 4912, 5 )
	MisCancelAction(ClearMission, 890)
	
	MisNeed(MIS_NEED_DESP,"Bring 5 <yFearsome Staffs of Giant> to <bMekkilon> at <j(2662, 648)>.")
	MisNeed(MIS_NEED_ITEM, 4912, 5, 10, 5)
	
	MisHelpTalk("I can't wait! Please hurry!")
	MisResultTalk("The best weapons of the Yetis")
	MisResultCondition(NoRecord, 890)
	MisResultCondition(HasMission, 890)
	MisResultCondition(HasItem, 4912, 5)
	MisResultAction(TakeItem, 4912, 5)
	MisResultAction(ClearMission, 890)
	MisResultAction(SetRecord, 890)
	MisResultAction(AddExp,26112,26112)
	MisResultAction(AddMoney,1125,1125)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4912 )	
	TriggerAction( 1, AddNextFlag, 890, 10, 5 )
	RegCurTrigger(8901)

-----------------------------------
	DefineMission( 891, "For Honour", 891 )
	
	MisBeginTalk( "<ts>ome people go to war for peace, others for honor. To prove yourself worthy, defeat those <rElite Skeletal Warriors>.<n><t>For honor and glory!")
	MisBeginCondition(LvCheck, ">", 46 )
	MisBeginCondition(HasRecord, 888)
	MisBeginCondition(NoMission, 891)
	MisBeginCondition(NoRecord, 891)
	MisBeginAction(AddMission, 891)
	MisBeginAction(AddTrigger, 8911, TE_KILL, 506, 10 )
	MisBeginAction(AddTrigger, 8912, TE_GETITEM, 4826, 1 )
	MisCancelAction(ClearMission, 891)

	MisNeed(MIS_NEED_DESP,"Kill 10 <rElite Skeletal Warriors> and bring back the <yMark of Warrior Honor> to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 506, 10, 10, 10 )
	MisNeed(MIS_NEED_ITEM, 4826, 1, 20, 1 )

	MisHelpTalk("Have you gotten hold of the <yMark of Warrior Honor>?")
	MisResultTalk("Take this, honorable warrior and you will be remembered forever!")
	MisResultCondition(NoRecord, 891)
	MisResultCondition(HasMission, 891)
	MisResultCondition(HasFlag, 891, 19)
	MisResultCondition(HasItem, 4826, 1)
	MisResultAction(TakeItem, 4826, 1)
	MisResultAction(ClearMission, 891)
	MisResultAction(SetRecord, 891)
	MisResultAction(AddExp,28832,28832)
	MisResultAction(AddMoney,1146,1146)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 506 )	
	TriggerAction( 1, AddNextFlag, 891, 10, 10 )
	RegCurTrigger( 8911 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4826 )	
	TriggerAction( 1, AddNextFlag, 891, 20, 1 )
	RegCurTrigger( 8912 )


-----------------------------------
	DefineMission( 892, "Hunt for Lizardman", 892 )
	
	MisBeginTalk( "<t>Our hunt should have ended with the <rGreat Polar Bears>. However, a group of <rLizardmen> have been causing havoc and robbing any traveler they see.<n><t>Put a stop to this! There will be a great reward for them!")
	MisBeginCondition(LvCheck, ">", 49 )
	MisBeginCondition(HasRecord, 885)
	MisBeginCondition(NoMission, 892)
	MisBeginCondition(NoRecord, 892)
	MisBeginAction(AddMission, 892)
	MisBeginAction(AddTrigger, 8921, TE_KILL, 196, 15 )
	MisCancelAction(ClearMission, 892)

	MisNeed(MIS_NEED_DESP,"Hunt 15 <rLizardmen> and return to <bNeila> at <j(2675, 631)>.")
	MisNeed(MIS_NEED_KILL, 196, 15, 10, 15 )

	MisHelpTalk("Remember, its 15 <rLizardmen>.")
	MisResultTalk("At last somebody managed to deal with those pesky <rLizardmen>.")
	MisResultCondition(NoRecord, 892)
	MisResultCondition(HasMission, 892)
	MisResultCondition(HasFlag, 892, 24)
	MisResultAction(ClearMission, 892)
	MisResultAction(SetRecord, 892)
	MisResultAction(AddExp,38628,38628)
	MisResultAction(AddMoney,1212,1212)


	InitTrigger()
	TriggerCondition( 1, IsMonster, 196 )	
	TriggerAction( 1, AddNextFlag, 892, 10, 15 )
	RegCurTrigger( 8921 )

-----------------------------------
	DefineMission( 893, "Lizard Man Axe", 893 )
	
	MisBeginTalk( "<t>I am low in supply for <yLizardman Axes>. Can you bring me 5 <yLizardman Axes>?<n><t>I promise to pay you well.")
	MisBeginCondition(LvCheck, ">", 49 )
	MisBeginCondition(HasRecord, 890)
	MisBeginCondition(NoMission, 893)
	MisBeginCondition(NoRecord, 893)
	MisBeginAction(AddMission, 893)
	MisBeginAction(AddTrigger, 8931, TE_GETITEM, 4920, 5 )
	MisCancelAction(ClearMission, 893)
	
	MisNeed(MIS_NEED_DESP,"Collect 5 <yLizardman Axes> for <bMekkilon> at <j(2662, 648)>.")
	MisNeed(MIS_NEED_ITEM, 4920, 5, 10, 5)
	
	MisHelpTalk("Where are the 5 <yLizardman Axes> you promised?")
	MisResultTalk("Let me examine these axe before I pay you.")
	MisResultCondition(NoRecord, 893)
	MisResultCondition(HasMission, 893)
	MisResultCondition(HasItem, 4920, 5)
	MisResultAction(TakeItem, 4920, 5)
	MisResultAction(ClearMission, 893)
	MisResultAction(SetRecord, 893)
	MisResultAction(AddExp,38628,38628)
	MisResultAction(AddMoney,1212,1212)


	InitTrigger()
	TriggerCondition( 1, IsItem, 4920 )	
	TriggerAction( 1, AddNextFlag, 893, 10, 5 )
	RegCurTrigger(8931)


	DefineMission(894, "Counter Probe", 894)
	MisBeginTalk("<t> Hey you! Come over please! You seems to be new around here.<n><t>If you have nothing to do, you might as well help us out by destroying those cannon towers of the <rDeathsouls>! I am getting sleepless nights to devise a way to counter those towers.<n><t>There are great risk involved so think twice before carrying out the task.")
	
	MisBeginCondition(NoRecord, 894)
	MisBeginCondition(NoMission, 894)
	MisBeginCondition(HasPirateGuild)

	MisBeginAction(AddMission, 894)
	MisBeginAction(AddTrigger, 8941, TE_KILL, 801, 4)

	MisCancelAction(ClearMission, 894)

	MisNeed(MIS_NEED_KILL, 801, 4, 10, 4)
	

	MisHelpTalk("<t>What are you still doing here? Hiding like a coward? I despise your lot!")
	MisResultTalk("<t> It seems that I was wrong about you. You have great potential.<n><t>I have told <bJack Arrow> about you and he is quite interested in meeting you. Why don't you pay him a visit?")

	MisResultCondition(HasMission , 894)
	MisResultCondition(NoRecord, 894)
	MisResultCondition(HasFlag, 894, 13)
	MisResultCondition(HasPirateGuild)

	MisResultAction(ClearMission, 894)
	MisResultAction(SetRecord, 894)

	MisResultAction(AddExp, 80000,80000)
	MisResultAction(AddMoney, 40000,40000)

	InitTrigger()
	TriggerCondition(1, IsMonster, 801)
	TriggerAction(1, AddNextFlag, 894, 10, 4)
	RegCurTrigger(8941)
		

-- >> 

	DefineMission(895, "Peerless Pirate", 895)
	MisBeginTalk("<t> I have heard from the <bBlacksmith> that you have destroyed the <rDeathsoul>'s sentry tower.<n><t>I can imagine how furious <rBaborosa> is right now haha!<n><t>However, their might is still not to be underestimated. Can you help us get rid of 30 <rDeathsoul Soldiers> and 15 <rDeathsoul Officers>?<n><t>This will cripple their army greatly.")
	
	MisBeginCondition(HasRecord, 894)
	MisBeginCondition(NoRecord, 895)
	MisBeginCondition(NoMission, 895)
	MisBeginCondition(HasPirateGuild)

	MisBeginAction(AddMission, 895)
	MisBeginAction(AddTrigger, 8951, TE_KILL, 808, 30)
	MisBeginAction(AddTrigger, 8952, TE_KILL, 817, 15)

	MisCancelAction(ClearMission, 895)

	MisNeed(MIS_NEED_KILL, 808, 30, 30, 30)
	MisNeed(MIS_NEED_KILL, 817, 15, 70, 15)
	

	MisHelpTalk("<t>Really hope that those <rDeathsoul> army disappear from the face of the world.")
	MisResultTalk("<t>Dear god! You have brought hope back to us! Now is not the time for us to relax.<n><t>The day of the final battle draws near. You should rest and prepare yourself for the next challenge.")

	MisResultCondition(HasMission ,895)
	MisResultCondition(NoRecord , 895)
	MisResultCondition(HasFlag, 895, 59)
	MisResultCondition(HasFlag, 895, 84)
	MisResultCondition(HasPirateGuild)

	MisResultAction(ClearMission, 895)
	MisResultAction(SetRecord, 895)

	MisResultAction(AddExp, 200000,200000)
	MisResultAction(AddMoney, 150000,150000)

	InitTrigger()
	TriggerCondition(1, IsMonster, 808)
	TriggerAction(1, AddNextFlag, 895, 30, 30)
	RegCurTrigger(8951)
		
	InitTrigger()
	TriggerCondition(1, IsMonster, 817)
	TriggerAction(1, AddNextFlag, 895, 70, 15)
	RegCurTrigger(8952)
	
	
--  >> 

	DefineMission(896, "Commander's Head", 896)
	MisBeginTalk("<t>Have you prepared? The time for battle draws near! I want you to kill <rDeathsoul Commander> and bring me his head!<n><t>I know you have the capability so please accept this task! Of course, I will reward you well.")
	
	MisBeginCondition(HasRecord, 895)
	MisBeginCondition(NoRecord, 896)
	MisBeginCondition(NoMission, 896)
	MisBeginCondition(HasPirateGuild)

	MisBeginAction(AddMission, 896)
	MisBeginAction(AddTrigger, 8961, TE_KILL, 807, 1)
	MisBeginAction(AddTrigger, 8962, TE_GETITEM, 2387, 1)

	MisCancelAction(ClearMission, 896)

	MisNeed(MIS_NEED_KILL, 807, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 2387, 1, 20, 1)
	

	MisHelpTalk("<t>Wait for daybreak if you dare! The sun will reduce you to dust!")
	MisResultTalk("<t>Haha! I knew you can do it! Damn that commander haha!")

	MisResultCondition(HasMission, 896)
	MisResultCondition(NoRecord, 896)
	MisResultCondition(HasFlag, 896, 10)
	MisResultCondition(HasItem, 2387, 1)
	MisResultBagNeed(1)
	MisResultCondition(HasPirateGuild)

	MisResultAction(TakeItem, 2387, 1)
	MisResultAction(ClearMission, 896)
	MisResultAction(SetRecord, 896)

	MisResultAction(AddExp, 500000,500000)
	MisResultAction(AddMoney, 1000000,1000000)

	InitTrigger()
	TriggerCondition(1, IsMonster, 807)
	TriggerAction(1, AddNextFlag, 896, 10, 1)
	RegCurTrigger(8961)
		
	InitTrigger()
	TriggerCondition(1, IsItem, 2387)
	TriggerAction(1, AddNextFlag, 896, 20, 1)
	RegCurTrigger(8962)




----------------------------------------
--                                    --
--                            --
--                                    --
----------------------------------------

--  >> 
	DefineMission(897, "Pirate's Food", 897)

	MisBeginTalk("<t><rDeathsouls> of the <pSkeletar Isle> are getting arrogant. It is rumored that they got their energy from <yBeer> and <yBBQ Meat>.<n><t>I command you to bring back 5 <yJugs of Beer of Pirate> and 5 <ySlices of BBQ Meat of Pirate>. I want to see if there is anything magical in those food!")
	MisBeginCondition(NoRecord, 897)
	MisBeginCondition(NoMission,897)
	MisBeginCondition(HasNavyGuild)

	MisBeginAction(AddMission, 897)
	MisBeginAction(AddTrigger, 8971,TE_GETITEM, 2413, 5)
	MisBeginAction(AddTrigger, 8972,TE_GETITEM, 2414, 5)
	
	MisCancelAction(ClearMission, 897)

	MisNeed(MIS_NEED_ITEM, 2413, 5, 10, 5)
	MisNeed(MIS_NEED_ITEM, 2414, 5, 20, 5)

	MisHelpTalk("<t>Don't tell me <rDeathsouls> need food too?")

	MisResultTalk("Well done, let me have a look. Hmm...This is weird...What are they doing with normal <yBeer> and <yBBQ Meat>?<n><ts>eems like the rumor cannot be trusted. Take your reward with you, thanks!")
	
	MisResultCondition(HasMission, 897)
	MisResultCondition(NoRecord, 897)
	MisResultCondition(HasItem, 2413, 5)
	MisResultCondition(HasItem, 2414, 5)
	MisResultCondition(HasNavyGuild)

	MisResultAction(TakeItem , 2413, 5)
	MisResultAction(TakeItem , 2414, 5)
	MisResultAction(ClearMission, 897)
	MisResultAction(SetRecord, 897)

	MisResultAction(AddExp ,200000, 200000)
	MisResultAction(AddMoney ,100000, 100000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2413)
	TriggerAction(1,AddNextFlag, 897, 10, 5)
	RegCurTrigger(8971)

	InitTrigger()
	TriggerCondition(1, IsItem, 2414)
	TriggerAction(1,AddNextFlag, 897, 20, 5)
	RegCurTrigger(8972)
	
--  >>  
	DefineMission(898, "Pirate's Annihilation", 898)

	MisBeginTalk("<t>Hey, soldier! I have a new task for you!<n><t>I need you to collect 15 pieces of <yPirate's Bone>. Don't question me about it, this is top secret! Just carry out your orders!")
	MisBeginCondition(HasRecord, 897)
	MisBeginCondition(NoRecord, 898)
	MisBeginCondition(NoMission,898)
	MisBeginCondition(HasNavyGuild)

	MisBeginAction(AddMission, 898)
	MisBeginAction(AddTrigger, 8981, TE_GETITEM, 2419, 15)
	
	MisCancelAction(ClearMission, 898)

	MisNeed(MIS_NEED_ITEM, 2419, 15, 30, 15)

	MisHelpTalk("<t>You have not collected 30 <yPirate's Bones>? Why are you here then?")

	MisResultTalk("<t>Yes, these are the bones we needed. Time to take some action against them.<n><t>For the time being, please rest well and prepare for the next battle.")
	
	MisResultCondition(HasMission, 898)
	MisResultCondition(NoRecord, 898)
	MisResultCondition(HasItem, 2419, 15)
	MisResultCondition(HasNavyGuild)

	MisResultAction(TakeItem , 2419, 15)
	MisResultAction(ClearMission, 898)
	MisResultAction(SetRecord, 898)

	MisResultAction(AddExp ,250000, 250000)
	MisResultAction(AddMoney ,150000, 150000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2419)
	TriggerAction(1,AddNextFlag, 898, 30, 15)
	RegCurTrigger(8981)


--  >> 
	DefineMission(899, "Cursed Black Jewel", 899)

	MisBeginTalk("<t>Are you prepared? Listen in details regarding the next task!<n><t>We have found out that those <rDeathsouls> come under direct command from the \"Black Jewel\". It should be destroyed as soon as possible.<n><t>At the same time, bring back their <yCaptain's Token>. This way they will have no proper leadership and will fall into disarray.<n><t>Remember, it is not as simple as it seems, take extra precaution!")
	MisBeginCondition(HasRecord, 898)
	MisBeginCondition(NoRecord, 899)
	MisBeginCondition(NoMission,899)
	MisBeginCondition(HasNavyGuild)

	MisBeginAction(AddMission, 899)
	MisBeginAction(AddTrigger, 8991,TE_KILL, 815, 1)
	MisBeginAction(AddTrigger, 8992,TE_GETITEM, 2429, 1 )
	
	MisCancelAction(ClearMission, 899)

	MisNeed(MIS_NEED_KILL, 815, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 2429, 1, 20, 1)

	MisHelpTalk("<t>The army is amassing now. Once we have sunk the <rBlack Jewel>, we will begin our main attack! Hehe!")

	MisResultTalk("<t>Woah, you managed to complete this task all by yourself? You are truly a great help to us!")
	
	MisResultCondition(HasMission, 899)
	MisResultCondition(NoRecord, 899)
	MisResultCondition(HasFlag, 899, 10)
	MisResultCondition(HasItem, 2429, 1)
	MisResultCondition(HasNavyGuild)

	MisResultAction(TakeItem , 2429, 1)
	MisResultAction(ClearMission, 899)
	MisResultAction(SetRecord, 899)

	MisResultAction(AddExp ,500000, 500000)
	MisResultAction(AddMoney ,1000000, 1000000)

	InitTrigger()
	TriggerCondition(1, IsMonster, 815, 1)
	TriggerAction(1,AddNextFlag, 899, 10,1)
	RegCurTrigger(8991)

	InitTrigger()
	TriggerCondition(1, IsItem, 2429, 1)
	TriggerAction(1,AddNextFlag, 899, 20,1)
	RegCurTrigger(8992)


end
AreaMission001()
