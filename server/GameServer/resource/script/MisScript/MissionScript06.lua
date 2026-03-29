print("-- [Loading] Mission Script [06].")

--------------------------------------------------


--                 Copy Task


--------------------------------------------
function DuplicateMission001()
-----------------------------------
	DefineMission( 500, "Drunkyard Secrets", 500 )
	
	MisBeginTalk( "I have not taste any wine for a long time...Young fellow, wine is like oxygen to me! I cannot live without it! <n><t>Can you buy a bottle of good wine for me from the bar in Argent? I will tell you a secret if you do it.")
	MisBeginCondition(LvCheck, ">", 29 )
	MisBeginCondition(NoMission, 500)
	MisBeginCondition(NoRecord, 500)
	MisBeginAction(AddMission, 500)
	MisBeginAction(AddTrigger, 5001, TE_GETITEM, 3916, 1 )
	MisCancelAction(ClearMission, 500)

	MisNeed(MIS_NEED_DESP,"Buy 1 Coconut Wine for Drunkyard in Argent Bar at <j(2222, 2889)>.")
	MisNeed(MIS_NEED_ITEM, 3916, 1, 10, 1)
		
	MisHelpTalk("What? That girl is not willing to sell wine to you? Or you have not visited her? Go now! Don't make me angry. I will forget what you want to find out.")
	MisResultTalk("Hmm This is indeed a good wine! It's been a long time since I have tasted such quality! Do you want some? Ah ZZzzzZZZzzz.")
	MisResultCondition(NoRecord, 500)
	MisResultCondition(HasMission, 500)
	MisResultCondition(HasItem, 3916, 1)
	MisResultAction(TakeItem, 3916, 1)
	MisResultAction(ClearMission, 500)
	MisResultAction(SetRecord, 500)
	MisResultAction(AddExp, 5000, 5000)
	--MisResultAction(AddMoney,270,270)

	InitTrigger()
	TriggerCondition( 1, IsItem, 3916 )	
	TriggerAction( 1, AddNextFlag, 500, 10, 1 )
	RegCurTrigger( 5001 )

-----------------------------------
	DefineMission( 501, "Another Cup Please!", 501 )
	
	MisBeginTalk( "<t>Oh my...The last drop of my wine is gone! But I am still thirsty for more...<n><t>Young adventurer, get me the famous \"Drunkern Dream\" from Argent bar. Faster!<n><t>Some Sashimi too will be nice...")
	MisBeginCondition(NoMission, 501)
	MisBeginCondition(NoRecord, 501)
	MisBeginCondition(HasRecord, 500)
	MisBeginAction(AddMission, 501)
	MisBeginAction(AddTrigger, 5011, TE_GETITEM, 3926, 1 )
	MisBeginAction(AddTrigger, 5012, TE_GETITEM, 1478, 20 )
	MisCancelAction(ClearMission, 501)

	MisNeed(MIS_NEED_DESP,"Argent Drunkyard at <j(2222, 2889)> requires a bottle of Drunken Dreams and 20 Sashimi.")
	MisNeed(MIS_NEED_ITEM, 3926, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 1478, 20, 20, 20)
		
	MisHelpTalk("zZZZzzzZZZI want more wine!")
	MisResultTalk(" AhGood wine! I will tell you the secret now.<n><t>When I was young around your age, I gathered a group of enthusiastic adventurers like you and me. We went on a sea expedition once and salvage an ancient looking compass. We sail towards the direction it was pointing and suddenly, a hugh whirlpool appears in front of our ship and suck us into the portal.<n><t>Through the portal is a small island with a forsaken city in the middle of it. Piles of treasures litters the street of the city!<n><t>However, none of us are able to leave with any treasures. Anyone who tried to take any treasure from the city was killed by undead souls and skeletons that appeared out of nowhere! It is so scary! Only me and Little Daniel escaped that calamity.<n><t>You want me to bring you there? NO! I will never set foot on that cursed place ever again! Look for Little Daniel, he knows the way to get to the Forsaken City. Leave me with my wine...zZzz...")
	MisResultCondition(NoRecord, 501)
	MisResultCondition(HasMission, 501)
	MisResultCondition(HasItem, 3926, 1)
	MisResultCondition(HasItem, 1478, 20)
	MisResultAction(TakeItem, 3926, 1)
	MisResultAction(TakeItem, 1478, 20)
	MisResultAction(ClearMission, 501)
	MisResultAction(SetRecord, 501)
	MisResultAction(AddExp, 10000, 10000)
	--MisResultAction(AddMoney,270,270)

	InitTrigger()
	TriggerCondition( 1, IsItem, 3926 )	
	TriggerAction( 1, AddNextFlag, 501, 10, 1 )
	RegCurTrigger( 5011 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 1478 )	
	TriggerAction( 1, AddNextFlag, 501, 20, 20 )
	RegCurTrigger( 5012 )

-----------------------------------
	DefineMission( 502, "Drunken Dreams", 502 )
	
	MisBeginTalk( "<t>Oh...Drunken Dreams? It must be that drunkyard who told you about it.<n><t>It requires special brewing ingredients that consist of Stramonium Flower, Rainbow Fruit and Strange Fruit. Get me these and I will brew one for you. However, it needed to be contained in a Snowy Trumpet Shell to make it tasty. Get me Snowy Trumpet Shell too.<n><t>And also a fee of 2000G for my effort.")
	MisBeginCondition(NoMission, 502)
	MisBeginCondition(NoRecord, 502)
	MisBeginCondition(HasRecord, 500)
	MisBeginCondition(HasMission, 501)
	MisBeginAction(AddMission, 502)
	MisBeginAction(AddTrigger, 5021, TE_GETITEM, 4377, 1 )
	MisBeginAction(AddTrigger, 5022, TE_GETITEM, 3121, 5 )
	MisBeginAction(AddTrigger, 5023, TE_GETITEM, 3131, 5 )
	MisBeginAction(AddTrigger, 5024, TE_GETITEM, 4352, 20 )
	MisCancelAction(ClearMission, 502)

	MisNeed(MIS_NEED_ITEM, 4377, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 3121, 5, 15, 5)
	MisNeed(MIS_NEED_ITEM, 3131, 5, 20, 5)
	MisNeed(MIS_NEED_ITEM, 4352, 20, 30, 20)

	MisPrize(MIS_PRIZE_ITEM, 3926, 1, 4)
	MisPrizeSelAll()
		
	MisHelpTalk("Brewing of \"Drunken Dreams\"All ingredients must be prepared.")
	MisResultTalk("Yes! These are the stuff. Looks like you are really determine. Take this wine that the Drunkyard wanted.")
	MisResultCondition(NoRecord, 502)
	MisResultCondition(HasMission, 502)
	MisResultCondition(HasItem, 4377, 1)
	MisResultCondition(HasItem, 3121, 5)
	MisResultCondition(HasItem, 3131, 5)
	MisResultCondition(HasItem, 4352, 20)
	MisResultCondition(HasMoney, 2000 )
	MisResultAction(TakeMoney, 2000 )
	MisResultAction(TakeItem, 4377, 1)
	MisResultAction(TakeItem, 3121, 5)
	MisResultAction(TakeItem, 3131, 5)
	MisResultAction(TakeItem, 4352, 20)
	MisResultAction(ClearMission, 502)
	MisResultAction(SetRecord, 502)
	MisResultAction(AddExp, 20000, 20000)
	--MisResultAction(AddMoney,270,270)

	InitTrigger()
	TriggerCondition( 1, IsItem, 4377 )	
	TriggerAction( 1, AddNextFlag, 502, 10, 1 )
	RegCurTrigger( 5021 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3121 )	
	TriggerAction( 1, AddNextFlag, 502, 15, 5 )
	RegCurTrigger( 5022 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3131 )	
	TriggerAction( 1, AddNextFlag, 502, 20, 5 )
	RegCurTrigger( 5023 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 4352 )	
	TriggerAction( 1, AddNextFlag, 502, 20, 20 )
	RegCurTrigger( 5024 )

-----------------------------------
	DefineMission( 503, "Skeleton of Sorrow Warrior", 503 )
	
	MisBeginTalk( "<t>Since the drunkyard send you here, I will help you. I have been there before. You will need an Ancient Generator to enter. Bring me 10 Robot Core and I will make it for you.<n><t>By the way, can you collect some bones for my research while you are there.")
	MisBeginCondition(NoMission, 503)
	MisBeginCondition(NoRecord, 503)
	MisBeginCondition(HasRecord, 501)
	MisBeginAction(AddMission, 503)
	MisBeginAction(AddTrigger, 5031, TE_GETITEM, 3434, 10 )
	MisBeginAction(AddTrigger, 5032, TE_GETITEM, 3435, 10 )
	MisBeginAction(AddTrigger, 5033, TE_GETITEM, 3436, 10 )
	MisBeginAction(AddTrigger, 5034, TE_GETITEM, 3437, 10 )
	MisCancelAction(ClearMission, 503)

	--MisNeed(MIS_NEED_DESP,"Found 10 Sorrow Warrior Carcass in Forsaken City. Give it to Little Daniel in Argent City at (2193, 2730).")
	MisNeed(MIS_NEED_ITEM, 3434, 10, 10, 10)
	MisNeed(MIS_NEED_ITEM, 3435, 10, 20, 10)
	MisNeed(MIS_NEED_ITEM, 3436, 10, 30, 10)
	MisNeed(MIS_NEED_ITEM, 3437, 10, 40, 10)
		
	MisHelpTalk("HmmThis is a dangerous and meaningful quest. Maybe you should consider to take it up.")
	MisResultTalk("So these are the carcass of those undead.<n><t>I can feel them calling out to me! I wonder what is this mysterious force behind this. I will need to do an indepth research.<n><t>Maybe I might discover some secret!")
	MisResultCondition(NoRecord, 503)
	MisResultCondition(HasMission, 503)
	MisResultCondition(HasItem, 3434, 10)
	MisResultCondition(HasItem, 3435, 10)
	MisResultCondition(HasItem, 3436, 10)
	MisResultCondition(HasItem, 3437, 10)
	MisResultAction(TakeItem, 3434, 10)
	MisResultAction(TakeItem, 3435, 10)
	MisResultAction(TakeItem, 3436, 10)
	MisResultAction(TakeItem, 3437, 10)
	MisResultAction(ClearMission, 503)
	MisResultAction(SetRecord, 503)
	MisResultAction(AddExp, 80000, 80000)
	--MisResultAction(AddMoney,270,270)

	InitTrigger()
	TriggerCondition( 1, IsItem, 3434 )	
	TriggerAction( 1, AddNextFlag, 503, 10, 10 )
	RegCurTrigger( 5031 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3435 )	
	TriggerAction( 1, AddNextFlag, 503, 20, 10 )
	RegCurTrigger( 5032 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3436 )	
	TriggerAction( 1, AddNextFlag, 503, 30, 10 )
	RegCurTrigger( 5033 )
	InitTrigger()
	TriggerCondition( 1, IsItem, 3437 )	
	TriggerAction( 1, AddNextFlag, 503, 40, 10 )
	RegCurTrigger( 5034 )

---------------------------------------------------------
--                                   				   --
--        Pirates of the Caribbean Main Quest          --
--                                   				   --
---------------------------------------------------------

---------------------------------------------------------
--                                   				   --
--              Main Mission            			   --
--                                   				   --
---------------------------------------------------------

-- Blacksmith >> Blacksmith's Worries
	DefineMission(504,"Blacksmith's Worries",504)
	
	MisBeginTalk("<t>My beloved was kidnapped! I will go save her myself! Damn those Deathsouls! I will rip you to pieces!<n><t>Sorry, forgive me for my outburst. If you seen Elizabeth, tell her do not be afraid for I will save her.<n><t>Even if it cost me my life!")

	MisBeginCondition(NoRecord, 504)
	MisBeginCondition(NoMission, 504)

	MisBeginAction(AddMission, 504)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>I will go to your rescure immediately! Wait for me!")
	MisNeed(MIS_NEED_DESP,"Relay Blacksmith's message to Elizabeth.")
	
	MisResultCondition(AlwaysFailure)

-- Blacksmith >> Blacksmith's Worries
	DefineMission(505,"Blacksmith's Worries",504,COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Did Mark sent you to me?<n><t>I know...I love him too...Always do....")
	MisResultCondition(HasMission, 504)
	MisBeginCondition(NoRecord, 504)
	
	MisResultAction(AddExp, 10000, 10000)
	MisResultAction(AddMoney, 25000, 25000)
	MisResultAction(ClearMission, 504)
	MisResultAction(SetRecord, 504)


-- Blacksmith >> Elizabeth's Love Token
	DefineMission(506,"Elizabeth's Love Keepsake",505)

	MisBeginTalk("<t>I would like you to pass this necklace to him...Oh no! Where is my necklace?<n><t>Oh dear! I think I have dropped it on the Skeletar Pirate Ship! It is an important keepsake between me and Mark!<n><t>Can you retrieve it back for me please.")

	MisBeginCondition(HasRecord, 504)
	MisBeginCondition(NoMission, 505)
	MisBeginCondition(NoRecord, 505)

	MisBeginAction(AddMission, 505)
	MisBeginAction(AddTrigger, 5051, TE_GETITEM, 2415, 1)
	
	MisNeed(MIS_NEED_ITEM, 2415, 1, 10, 1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>Have you found the necklace.")

	MisResultTalk("<t>Yes, this is the one. Thank you for getting it back for me.")

	MisResultCondition(HasMission, 505)
	MisResultCondition(HasItem, 2415, 1)
	MisResultCondition(NoRecord,505)

	MisResultAction(AddExp, 250000, 250000)
	MisResultAction(AddMoney, 150000, 150000)
	MisResultAction(ClearMission, 505)
	MisResultAction(TakeItem, 2415,1)
	MisResultAction(SetRecord, 505)

	InitTrigger()
	TriggerCondition(1,IsItem, 2415)
	TriggerAction(1, AddNextFlag, 505, 10, 1)
	RegCurTrigger(5051)


-- Blacksmith >> Love Necklace
	DefineMission(507,"Necklace of Love",506)
	
	MisBeginTalk("<t>It is done. I have written my message within this necklace<n><t>Can you help me pass this necklace to Mark? May the Gods protect you!")

	MisBeginCondition(HasRecord, 505)
	MisBeginCondition(NoRecord, 506)
	MisBeginCondition(NoMission, 506)
	MisBeginBagNeed(1)

	MisBeginAction(AddMission, 506)
	MisBeginAction(GiveItem, 2415,1,4)

	MisHelpTalk("<t>May the Goddess bless you.")
	MisNeed(MIS_NEED_DESP,"Help Elizabeth pass the Necklace of Love to Blacksmith Mark.")

	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisResultCondition(AlwaysFailure)

-- Blacksmith >> Love Necklace
	DefineMission(508,"Necklace of Love",506, COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>You found Elizabeth? Let me go save her now,<n><t>Wait, where is our necklace? Let me check.")
	MisResultCondition(HasMission, 506)
	MisResultCondition(NoRecord,506)
	MisResultCondition(HasItem, 2415, 1)

	MisResultAction(AddExp, 10000, 10000)
	MisResultAction(AddMoney, 25000, 25000)

	MisResultAction(TakeItem, 2415, 1)
	MisResultAction(ClearMission, 506)
	MisResultAction(SetRecord, 506)

-- Blacksmith >> Blacksmith's Promise
	DefineMission(509,"Blacksmith's Promise",507)
	
	MisBeginTalk("<t>To save my goddess, we will need a very unique weapon. Are you able to help me with that?<n><t>This weapon only drops from Deathsoul Officer which I believe you are able to deal with easily.")

	MisBeginCondition(HasRecord, 506)
	MisBeginCondition(NoRecord, 507)
	MisBeginCondition(NoMission, 507)

	MisBeginAction(AddMission, 507)
	MisBeginAction(AddTrigger, 5071, TE_GETITEM, 2384, 1)

	MisNeed(MIS_NEED_ITEM, 2384, 1, 10, 1)

	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
	MisHelpTalk("<t>...It will be ready soon. Hmm? You haven't found the unique weapon.")

	MisResultTalk("<t>This should be the weapon that Elizabeth is talking about. Its really special from the way I look at it.")

	MisResultCondition(HasMission, 507)
	MisResultCondition(HasItem, 2384, 1)
	MisResultCondition(NoRecord, 507)

	MisResultAction(TakeItem, 2384, 1)
	MisResultAction(AddExp, 200000, 200000)
	MisResultAction(AddMoney, 100000, 100000)

	MisResultAction(ClearMission, 507)
	MisResultAction(SetRecord, 507)

	InitTrigger()
	TriggerCondition(1, IsItem, 2384)
	TriggerAction(1, AddNextFlag, 507, 10, 1)
	RegCurTrigger(5071)

-- Blacksmith >> Special Weapon
	DefineMission(573,"Unique Weapon",508)
	
	MisBeginTalk("<t>This weapon seems to lack something, the feeling is not there. It feels strange to wield.<n><t>Sigh...Can you bring to Jack Arrow and let him take a look.")
	
	MisBeginBagNeed(1)
	MisBeginCondition(HasRecord, 507)
	MisBeginCondition(NoRecord, 508)
	MisBeginCondition(NoMission, 508)
	MisBeginBagNeed(1)

	MisBeginAction(AddMission, 508)
	MisBeginAction(GiveItem, 2384, 1,4)

	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>What did Jack say? You have not go over.")
	MisNeed(MIS_NEED_DESP,"Bring the unique weapon from the Blacksmith to Jack Arrow.")

	MisResultCondition(AlwaysFailure)


-- Jack Sparrow >> Special Weapon
	DefineMission(574,"Unique Weapon", 508,COMPLETE_SHOW)
	
	MisBeginCondition(AlwaysFailure)
	MisResultTalk("<t>Let me have a look! This weapon has a dark curse on it.<n><t>Only a man of evil can wield it...Mark is just too kind...")
	MisResultCondition(HasMission, 508)
	MisResultCondition(NoRecord, 508)
	MisResultCondition(HasItem, 2384, 1)

	MisResultAction(AddMoney, 25000, 25000)
	MisResultAction(TakeItem, 2384, 1)
	MisResultAction(ClearMission, 508)
	MisResultAction(SetRecord, 508)


-- Jack Sparrow >> Pirates and the Navy
	DefineMission(575,"Pirate Vs Navy",509)
	
	MisBeginTalk("<t>I requires you to make a trip to the Navy. Tell Wellington about the current situation.<n><t>I believed that he will make a suitable judgement.")

	MisBeginCondition(HasRecord, 508)
	MisBeginCondition(NoRecord, 509)
	MisBeginCondition(NoMission, 509)

	MisBeginAction(AddMission, 509)
	
	MisHelpTalk("<t>You have not make a move? Time does not wait!")
	MisNeed(MIS_NEED_DESP,"Help Jack Arrow send a message to Wellington.")
	
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisResultCondition(AlwaysFailure)

-- Norrington >> Pirates and Navy
	DefineMission(576, "Pirate Vs Navy", 509,COMPLETE_SHOW)
	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Jack is not dead yet? What! He dares to request my aid? How daring of him!<n><t>Hmm...Deathsoul Officer's Captain Token?<n><t>Seems like we are on the same side after all. I will consider Jack's suggestion.")

	MisResultCondition(HasMission, 509)
	MisResultCondition(NoRecord, 509)

	MisResultAction(AddMoney, 25000, 25000)
	MisResultAction(ClearMission, 509)
	MisResultAction(SetRecord, 509)

-- Norrington >> The General's Confession
	DefineMission(577,"General's Confession of Love",510)
	
	MisBeginTalk("<t>Damn those Deathsoul! My Proposal Ring is stolen by them!<n><t>Why took my ring of all things?<n><t>Damn! I spent quite a fortune to make it. Please get it back for me as soon as possible. It seems that they are escaping by the Spirit Ship.")

	MisBeginCondition(HasRecord, 509)
	MisBeginCondition(NoRecord, 510)
	MisBeginCondition(NoMission, 510)
	
	MisBeginAction(AddMission, 510)
	MisBeginAction(AddTrigger, 5101, TE_GETITEM, 2416, 1)
	MisNeed(MIS_NEED_ITEM, 2416, 1, 10, 1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>If you do not retrieve my ring, I will not help Jack Arrow.")

	MisResultTalk("<t>Thanks God! I can propose to Elizabeth now since I have the ring!")

	MisResultCondition(HasMission, 510)
	MisResultCondition(NoRecord, 510)
	MisResultCondition(HasItem, 2416, 1)
	
	MisResultAction(TakeItem, 2416, 1)
	MisResultAction(AddExp, 30000, 30000)
	MisResultAction(AddMoney, 35000, 35000)
	MisResultAction(ClearMission, 510)
	MisResultAction(SetRecord, 510)
	
	InitTrigger()
	TriggerCondition(1, IsItem, 2416)
	TriggerAction(1, AddNextFlag, 510, 10, 1)
	RegCurTrigger(5101)

-- Norrington >> The General's Confession
	DefineMission(578,"General's Confession of Love",511)
	
	MisBeginTalk("<t>How do I say this...I like a girl...I want to propose to her...<n><t>But I am shy...Can you help me pass this ring to her...Observe her reaction please...<n><t>I will be grateful to you....If she accepts my love...Oh right..... Her name is Elizabeth.")

	MisBeginCondition(HasRecord, 510)
	MisBeginCondition(NoRecord, 511)
	MisBeginCondition(NoMission, 511)
	MisBeginBagNeed(1)

	MisBeginAction(AddMission, 511)
	MisBeginAction(GiveItem, 2416, 1,4)
	MisHelpTalk("<t>Elizabeth...Elizabeth...")
	MisNeed(MIS_NEED_DESP,"<t>Help Wellington by giving the Proposal Ring to Elizabeth and observe her reaction.")

	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisResultCondition(AlwaysFailure)

-- Elizabeth >> The General's Confession
	DefineMission(579, "General's Confession of Love", 511,COMPLETE_SHOW)
	
	MisBeginCondition(AlwaysFailure)
	
	MisResultTalk("<t>General Wellington...Its not worth it.")
	MisResultCondition(HasMission, 511)
	MisResultCondition(NoRecord, 511)
	MisResultCondition(HasItem, 2416, 1)

	MisResultAction(AddMoney, 35000, 35000)
	MisResultAction(TakeItem, 2416, 1)
	MisResultAction(ClearMission, 511)
	MisResultAction(SetRecord, 511)

-- Elizabeth >> Elizabeth's Prayer
	DefineMission(580,"Elizabeth's Prayer",512)

	MisBeginTalk("<t>I don't feel good.<n><t>It seems that something has happened to Mark. I am quite worry, can you go over and check on him?<n><t>I will pray for his safety!")

	MisBeginCondition(HasRecord, 511)
	MisBeginCondition(NoRecord, 512)
	MisBeginCondition(NoMission, 512)
	
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")
	MisBeginAction(AddMission, 512)
	MisHelpTalk("<t>Oh Almighty God, please bless Mark...")
	MisNeed(MIS_NEED_DESP,"Help out the Blacksmith on behalf of Elizabeth.")

	MisResultCondition(AlwaysFailure)


-- Blacksmith >> Elizabeth's Prayer
	DefineMission(581, "Elizabeth's Prayer", 512,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Did Elizabeth sent you?<n><t>How is she getting on? I encounter some minor problem here but tell her not to worry.")

	MisResultCondition(HasMission, 512)
	MisResultCondition(NoRecord, 512)
	
	MisResultAction(AddMoney, 35000, 35000)
	MisResultAction(ClearMission, 512)
	MisResultAction(SetRecord, 512)

-- Blacksmith >> Curse
	DefineMission(582,"The Curse",513)

	MisBeginTalk("<t>The treasures we collected have been stolen from us by the dead spirits on the Black Jewel and they also set an evil curse on it.<n><t>I need you to find and return these cursed treasures to me, then I will proceed to lift the curse from the treasures.<n><t>How about it, it shouldn't be a problem.<n><t>Go forth and kill those from the Black Jewel. I know this is a extremely difficult mission, I will reward you handsomely once it is completed.")
	
	MisBeginCondition(HasRecord, 512)
	MisBeginCondition(NoRecord, 513)
	MisBeginCondition(NoMission, 513)
	
	MisBeginAction(AddMission, 513)
	MisBeginAction(AddTrigger, 5131,TE_GETITEM, 2417, 1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisNeed(MIS_NEED_ITEM, 2417, 1, 10, 1)

	MisHelpTalk("<t>You have not brought back the treasures.")

	MisResultTalk("<t> Let me have a look...This is a powerful curse indeed. Let me try to undo it.<n><t>!^($......%*#oa2......1&s?~*#^%!...... (Blacksmith starts to chant in some weird language...Scary!) Good! The curse is undone!")

	MisResultCondition(HasMission, 513)
	MisResultCondition(NoRecord, 513)
	MisResultCondition(HasItem, 2417, 1)
	
	MisResultAction(TakeItem, 2417, 1)
	MisResultAction(AddExp, 500000, 500000)
	MisResultAction(AddMoney, 800000, 800000)
	MisResultAction(ClearMission, 513)
	MisResultAction(SetRecord, 513)

	InitTrigger()
	TriggerCondition(1, IsItem, 2417)
	TriggerAction(1, AddNextFlag, 513, 10, 1)
	RegCurTrigger(5131)

-- Blacksmith >> Curse

	DefineMission(583,"The Curse",514)
	
	MisBeginTalk("<t>We have undone the curse of coins that those Deathsoul pirates have been relying on.<n><t>Without their treasures and source of power, they will not be able to resist our army!<n><t>It is time to make a decisive attack on them! However, please inform Jack Arrow about our plans first.")

	MisBeginCondition(HasRecord, 513)
	MisBeginCondition(NoRecord, 514)
	MisBeginCondition(NoMission, 514)

	MisBeginAction(AddMission, 514)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>You have not make a move? The enemies are at their weakest now! Go!")
	MisNeed(MIS_NEED_DESP,"Tell Jack Arrow oh behalf of the Blacksmith that the curse has been broken.")

	MisResultCondition(AlwaysFailure)

-- Jack Sparrow >> Curse
	DefineMission(584, "The Curse", 514,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>I've got it. We will begin attack soon. Are you with us.")
	MisResultCondition(HasMission, 514)
	MisResultCondition(NoRecord, 514)

	MisResultAction(AddMoney, 30000, 30000)
	MisResultAction(ClearMission, 514)
	MisResultAction(SetRecord, 514)

--  >> 
	DefineMission(585,"The Real Captain",515)
	
	MisBeginTalk("<t>Battle draws near! Wait for me Baborosa! Let us settle this once and for all!<n><t>My friend, if you can get back the Captain's Token from Baborosa, I will reward you greatly!<n><t>Let him know who is the real captain of the Black Jewel!")
	MisBeginCondition(HasRecord, 514)
	MisBeginCondition(NoRecord, 515)
	MisBeginCondition(NoMission,515)
	
	MisBeginAction(AddMission, 515)
	MisBeginAction(AddTrigger, 5151, TE_KILL, 805, 1)
	MisBeginAction(AddTrigger, 5152, TE_GETITEM, 2428, 1)

	MisNeed(MIS_NEED_KILL, 805, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 2428, 1,20, 1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisHelpTalk("<t>Damn you Baborosa!")

	MisResultTalk("<t>This is great! I am the captain of the Black Jewel once again!")

	MisResultCondition(HasMission, 515)
	MisResultCondition(HasFlag, 515, 10)
	MisResultCondition(HasItem, 2428,1)
	MisResultCondition(NoRecord, 515)

	MisResultAction(TakeItem, 2428, 1)
	MisResultAction(ClearMission, 515)
	MisResultAction(SetRecord, 515)

	MisResultAction(AddExp, 500000,500000)
	MisResultAction(AddMoney, 800000,800000)

	InitTrigger()
	TriggerCondition(1,IsMonster, 805)
	TriggerAction(1,AddNextFlag, 515, 10, 1)
	RegCurTrigger(5151)

	InitTrigger()
	TriggerCondition(1, IsItem, 2428)
	TriggerAction(1, AddNextFlag , 515, 20, 1)
	RegCurTrigger(5152)


-- Jack Sparrow >> Congrats
	DefineMission(586,"The Gift",516)
	
	MisBeginTalk("<t> Everything has finally ended. I heard that Mark and Elizabeth has gotten married at last.<n><t>I have not sent them my gift yet. Can you do it on my behalf? Why don't I sent it myself?<n><t>...Have you heard of an NPC who will do his own task.")

	MisBeginCondition(HasRecord, 515)
	MisBeginCondition(NoRecord, 516)
	MisBeginCondition(NoMission, 516)
	MisBeginBagNeed(1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisBeginAction(AddMission, 516)
	MisBeginAction(GiveItem, 2433, 1,4)
	MisHelpTalk("<t> Hmm...Love is in the air...Hehe!")
	MisNeed(MIS_NEED_DESP,"Send a gift on behalf of Jack to congratulate Elizabeth on her marriage to the Blacksmith.")
	
	MisResultCondition(AlwaysFailure)

-- Jack Sparrow >> Congrats
	DefineMission(587,"The Gift",516,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Wow...A gift from Jack! Let's open it!<n><t>What!!! Only a skeletar emblem...What a miser!")
	MisResultCondition(HasMission, 516)
	MisResultCondition(NoRecord, 516)
	MisResultCondition(HasItem, 2433, 1)
	
	MisResultAction(TakeItem, 2433, 1)
	MisResultAction(AddMoney, 30000, 30000)
	MisResultAction(ClearMission, 516)
	MisResultAction(SetRecord, 516)

-- Elizabeth >> I have a heart
	DefineMission(588,"Undeserving Love",517)
	
	MisBeginTalk("<t>Help me once again please...<n><t>Return this ring to General Wellington and I hope he can understand my feeling.")

	MisBeginCondition(HasRecord, 516)
	MisBeginCondition(NoRecord, 517)
	MisBeginCondition(NoMission, 517)
	MisBeginBagNeed(1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisBeginAction(AddMission, 517)
	MisBeginAction(GiveItem, 2416, 1,4)
	MisHelpTalk("<t>Where to go for my honeymoon...? Hmm..")
	MisNeed(MIS_NEED_DESP,"Help Elizabeth return the ring to Wellington.")
	
	MisResultCondition(AlwaysFailure)

-- Norrington >> I have a heart
	DefineMission(589,"Undeserving Love",517,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>There will be no turning back now...<n><t>Why make a promise if it cannot be kept...<n><t>Sorrow is the only thing left...")
	MisResultCondition(HasMission, 517)
	MisResultCondition(NoRecord, 517)
	MisResultCondition(HasItem, 2416, 1)
	
	MisResultAction(TakeItem, 2416, 1)
	MisResultAction(AddMoney, 30000, 30000)
	MisResultAction(ClearMission, 517)
	MisResultAction(SetRecord, 517)

-- Norrington >> Norrington's Blessing
	DefineMission(590,"Wellington's Blessing",518)
	
	MisBeginTalk("<t>I have thought it through. I should sent them my blessing instead since they truly love each other.<n><t>Please hand this gift of mine to them.")

	MisBeginCondition(HasRecord, 517)
	MisBeginCondition(NoRecord, 518)
	MisBeginCondition(NoMission, 518)
	MisBeginBagNeed(1)
	MisCancelAction(SystemNotice, "This quest cannot be abandoned!")

	MisBeginAction(AddMission, 518)
	MisBeginAction(GiveItem, 2435, 1,4)
	MisHelpTalk("<t>Elizabeth! I hope that fate will bring us together in our next life.")
	MisNeed(MIS_NEED_DESP,"Send the gift of Wellington to Elizabeth.")
	
	MisResultCondition(AlwaysFailure)

-- Elizabeth >> Norrington's Blessing
	DefineMission(591,"Wellington's Blessing",518,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>General Turner...Thanks for understanding how I feel.<n><t>Thank you...")
	MisResultCondition(HasMission, 518)
	MisResultCondition(NoRecord, 518)
	MisResultCondition(HasItem, 2435, 1)
	
	MisResultAction(TakeItem, 2435, 1)
	MisResultAction(AddMoney,300000, 300000)
	MisResultAction(ClearMission, 518)
	MisResultAction(SetRecord, 518)
	MisResultAction(GiveItem, 15054, 1, 4, 0)
	
end
DuplicateMission001()
