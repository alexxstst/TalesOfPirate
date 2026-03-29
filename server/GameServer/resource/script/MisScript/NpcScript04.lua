print("-- [Loading] NPC Script [04]")

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk1()


	Talk( 1, "Wu Xin: \"Emerald Blade on my left, Galiant Sword on my right, Dragon Crown on my headNothing can stop me!\"" )
	Text( 1, "Trade", BuyPage )
	Text( 1, "Repair", OpenRepair)
	Text( 1, "Regarding Forging", JumpPage, 2)
	Text( 1, "Regarding Combining", JumpPage, 3)

	Talk( 2, "Wu Xin: Forging? Only the blacksmith of Shaitan City knows the secret." )

	Talk( 3, "Wu Xin: You wish to combine something? From what I understand, you need a combination scroll and it will list the required items. Collect the items needed and talk to Grocer - Amos in Shaitan City. He is the only person who knows how to combine." )
	
	Talk( 4, "Blacksmith - Wu Xin: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 4 )
	Start( GetMultiTrigger(), 2 )


	InitTrade()
	Weapon(	0023	)
	Weapon(	1399	)
	Weapon(	0021	)
	Weapon(	1378	)
	Weapon(	1387	)
	Weapon(	0084	)
	Weapon(	1426	)
	Weapon(	1461	)
	Weapon(	0108	)
	Weapon(	4302	)
	Weapon(	1438	)
	Weapon(	1441	)
	Weapon(	1473	)
	Weapon(	1476	)
	Weapon(	0045	)
	Weapon(	1414	)


	AddNpcMission ( 393 )
        AddNpcMission ( 567 )

	--------------------
	AddNpcMission	(5817)
	AddNpcMission	(5818)
end


------------------------------------------------------------
--  )
------------------------------------------------------------

function b_talk2()
	Talk(1, "Long Er: \"Welcome! We have all type of crusine for you to choose from. Anything to your liking?\"")
	--Text(1, "Trade", BuyPage)

	InitTrigger()
	TriggerCondition(1, HasRecord, 330)
	TriggerAction(1, JumpPage, 1)
	TriggerCondition(2, NoRecord, 330)
	TriggerAction(2, JumpPage, 2)
	Start(GetMultiTrigger(), 2)
	
	Talk(2, "Long Er: \"^$&#&^%\"" )

   	InitTrade()
	Other(1078) -- Steam Bun
	Other(1079) -- Bun
	Other(1080) -- Biscuit
	Other(1082) -- Fried Dough
	Other(1083) -- Spring Roll

	AddNpcMission(394)
	AddNpcMission(536)
	AddNpcMission(537)
	AddNpcMission(538)
    AddNpcMission(569)
	AddNpcMission(5819)
	AddNpcMission(5820)
end

------------------------------------------------------------
--  ()
------------------------------------------------------------

function b_talk3()


	Talk( 1, "Luna: \"HmmSomeone seems be have the same name as me.I might sue that person for copyrights infringement!\"" )
	Text( 1, "Trade", BuyPage )
	Talk( 2, "Luna: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )



	InitTrade()
	Other(	19500	)
	Other(	19501	)
	Other(	19502	)
	Other(	19503	)

	AddNpcMission ( 395 )
	AddNpcMission ( 517 )
	AddNpcMission ( 519 )
	AddNpcMission ( 525 )
	AddNpcMission ( 526 )
	--AddNpcMission ( 535 )
        AddNpcMission ( 564 )

	--------------------
	AddNpcMission	(5821)
	AddNpcMission	(5822)


end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk4()


	Talk( 1, "Yuri: \"Have you heard of Fountain of Life before?Devil Beast? What! You know nothing? How about Mahjong? Oh manWhat sort of adventurer are you?\"" )
	--Text( 1, "Trade", BuyPage )
	InitTrigger()
	TriggerCondition( 1, HasBoatInBerth, 13 )
	TriggerAction( 1, TradeBerthList, 13 )
	TriggerFailure( 1, JumpPage, 2 )
	--Text( 1, "Cargo Trade", MultiTrigger, GetMultiTrigger(), 1 )
	--Text( 1, "Regarding High Lv Commerce Permit",JumpPage, 3)
	
	Talk( 2, "Yuri: Sorry! Your ship is not docked in Spring Harbor. Unable to trade." )
	
	Talk( 3, "Hi! If you want to get rich, then you will need a \"Commerce Permit!\" With it you can have more products for commerce trade and also reduce your capital to maximize your profits! Currently, players can only obtain Commerce Permit from Shaitan Trading Post.")
	Text( 3, "Regarding Tax Rate",JumpPage, 7)
	Text( 3, "Low Lv Commerce Permit",JumpPage, 4)
	Text( 3, "Mid Lv Commerce Permit",JumpPage, 5)
	Text( 3, "High Lv Commerce Permit",JumpPage, 6)
	

	Talk( 4, "Low Lv Commerce Permit is easy to obtain. When you reaches Lv 20, go to the trading post in Shaitan and complete a simple task to have one.")
	Talk( 5, "When you reaches Lv 40 and reduces your tax rate to 20%, you can go to Shaitan Trading Post and obtain a Mid Lv Commerce Permit.")
	Talk( 6, "When you have reached level 60 and possess a Commerce Permit with 10% tax, you can activate the quest for High Lv Commerce.")
	Talk( 7, "Check your Commerce Permit to see the current Tax Rate imposed on your product when you sell it to any trader. Complete some quest at Shaitan Trading Post to reduce the Tax Rate.")
	Talk( 8, "Yuri: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 8 )
	Start( GetMultiTrigger(), 2 )


	InitGoods()
	SaleGoodsData(	0	,	4573	,	900	,	279	,	62	)
	SaleGoodsData(	0	,	4574	,	800	,	342	,	76	)
	SaleGoodsData(	1	,	4575	,	700	,	391	,	87	)
	SaleGoodsData(	1	,	4576	,	480	,	432	,	96	)
	SaleGoodsData(	2	,	4577	,	300	,	495	,	110	)
	SaleGoodsData(	2	,	4578	,	240	,	522	,	116	)
	SaleGoodsData(	3	,	4579	,	60	,	589	,	131	)
	SaleGoodsData(	3	,	4580	,	40	,	648	,	144	)

	BuyGoodsData(0,	4581	,	-1	,	542	,	121	)
	BuyGoodsData(0,	4583	,	-1	,	724	,	161	)
	BuyGoodsData(0,	4585	,	-1	,	846	,	188	)
	BuyGoodsData(0,	4587	,	-1	,	846	,	564	)
	BuyGoodsData(0,	4593	,	-1	,	1000	,	223	)
	BuyGoodsData(0,	4595	,	-1	,	938	,	625	)
	BuyGoodsData(0,	4597	,	-1	,	881	,	195	)
	BuyGoodsData(0,	4598	,	-1	,	878	,	195	)
	BuyGoodsData(0,	4600	,	-1	,	866	,	577	)
	BuyGoodsData(0,	4552	,	-1	,	642	,	143	)
	BuyGoodsData(0,	4553	,	-1	,	789	,	175	)
	BuyGoodsData(0,	4554	,	-1	,	934	,	207	)
	BuyGoodsData(0,	4556	,	-1	,	870	,	193	)
	BuyGoodsData(0,	4557	,	-1	,	970	,	215	)
	BuyGoodsData(0,	4560	,	-1	,	1056	,	235	)
	BuyGoodsData(0,	4563	,	-1	,	909	,	202	)
	BuyGoodsData(0,	4566	,	-1	,	785	,	175	)
	BuyGoodsData(0,	4569	,	-1	,	1014	,	225	)
	BuyGoodsData(0,	4572	,	-1	,	924	,	205	)
	BuyGoodsData(0,	4547	,	-1	,	100	,	0	)
	BuyGoodsData(0,	4549	,	-1	,	500	,	0	)
	BuyGoodsData(0,	4548	,	-1	,	300	,	0	)
	BuyGoodsData(0,	4551	,	-1	,	20	,	0	)

	InitTrigger()
	TriggerAction( 1, AddNpcTrigger, 51, TE_GAMETIME, TT_CYCLETIME, 30, 0 )
	SetNpcTrigger( GetTrigger( 1 ) )
	SetNpcActive()

	AddNpcMission ( 396 )
	AddNpcMission ( 530 )
	AddNpcMission ( 545 )
	AddNpcMission ( 546 )
	AddNpcMission ( 547 )
        AddNpcMission ( 572 )

	--------------------
	AddNpcMission	(5825)
	AddNpcMission	(5826)


end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk5()
	--
	local ReSelectTalk = "I need to reconsider"
	local ReSelectPage = 1

	--
	local CancelSelectTalk = "Forget itI will stay here"
	local CancelSelectPage = 7

	--
	local CurMapName1 = "garner"
	local CurMapName2 = "magicsea"
	local CurMapName5 = "darkblue"

	--
	local GoTo01X = 2231
	local GoTo01Y = 2788
	local GoTo01M = CurMapName1

	--
	local GoTo02X = 890
	local GoTo02Y = 3575
	local GoTo02M = CurMapName2

	--
	local GoTo04X = 1318
	local GoTo04Y = 510
	local GoTo04M = CurMapName5

	Talk( 1, "Chun Li: \"I am proud that I did not leave anyone in the sky \"" )
	Text( 1, "Go to Argent City!", JumpPage, 2 )
	Text( 1, "Go to Shaitan City!", JumpPage, 3 )
	Text( 1, "Go to Icicle City!", JumpPage, 4 )
	Text( 1, "Record Spawn point", JumpPage, 5 )
	
	InitTrigger()
	TriggerCondition( 1, LvCheck, "<", 11 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerCondition( 2, HasMoney, 500 )
	TriggerAction( 2, TakeMoney, 500 )
	TriggerAction( 2, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 2, JumpPage, 6 )
	Talk( 2, "Teleports to Argent City? No problem! Please pay 500G. Free for players Lv 10 and below!" )
	Text( 2, "Teleport",MultiTrigger, GetMultiTrigger(), 2 ) 
	Text( 2, CancelSelectTalk, JumpPage , CancelSelectPage )


	InitTrigger()
	TriggerCondition( 1, LvCheck, "<", 11 )
	TriggerAction( 1, GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerCondition( 2, HasMoney, 500 )
	TriggerAction( 2, TakeMoney, 500 )
	TriggerAction( 2,  GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerFailure( 2, JumpPage, 6 )
	Talk( 3, "I just love Shaitan City! Please pay 500G! Players Lv 10 and below free of charge!" )
	Text( 3, "Teleport",MultiTrigger, GetMultiTrigger(), 2)
	Text( 3, CancelSelectTalk, JumpPage , CancelSelectPage )


	InitTrigger()
	TriggerCondition( 1, LvCheck, "<", 11 )
	TriggerAction( 1, GoTo, GoTo04X, GoTo04Y, GoTo04M )
	TriggerCondition( 2, HasMoney, 500 )
	TriggerAction( 2, TakeMoney, 500 )
	TriggerAction( 2, GoTo, GoTo04X, GoTo04Y, GoTo04M )
	TriggerFailure( 2, JumpPage, 6 )
	Talk( 4, "Teleport to Icicle City? No problem! Please pay 500G! Players Lv 10 and below free!" )
	Text( 4, "Teleport",MultiTrigger, GetMultiTrigger(), 2)
	Text( 4, CancelSelectTalk, JumpPage , CancelSelectPage )

	Talk( 5, "Chun Li: Want to record in Spring Town? Is that correct?" )
	Text( 5, "Yes. Please record.", SetSpawnPos, "Spring Town" )
	Text( 5, "No, thank you",CloseTalk )

	Talk( 6, "Sorry! You do not have enough gold to teleport." )

	Talk( 7, "Remember to look for me if you need teleportation services", CloseTalk )
	Talk( 8, "Chun Li: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 8 )
	Start( GetMultiTrigger(), 2 )

	AddNpcMission ( 397 )
	 AddNpcMission ( 559 )
	MisListPage(8)

	------------- -------
	AddNpcMission	(5827)
	AddNpcMission	(5828)	

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk6()
	Talk(1, "Hocus Pocus: Hello there! I can see that you are blessed by the deities and fortune is smiling upon you. Since this is your lucky day, I will sell you a Pirate Manual for 500G. How about it?")
	Text(1, "Career and Fortune Lots", BuyPage)
	Text(1, "Bargain of Death", JumpPage, 2)

	InitTrade()
	Other(3086) -- Career Lot
	Other(3087) -- Fortune Lot
	
	Talk(5, "Hocus Pocus: '^$&#&^%%'")

	Talk(2, "Hocus Pocus: Darkness is the mask of Death... What do you want to redeem?")
	Text(2, "Silhouette of Death", AbaddonExchange, 2934)
	Text(2, "Curse Series", JumpPage, 3)
	Text(2, "Soul Series", JumpPage, 4)

	Talk(3, "Hocus Pocus: A kind child will not go around cursing others...")
	Text(3, "Curse of Hardin", AbaddonExchange, 2935)
	Text(3, "Curse of Darkness", AbaddonExchange, 2936)
	Text(3, "Curse of Abaddon", AbaddonExchange, 2937)
	Text(3, "Curse of Asura", AbaddonExchange, 2938)
	Text(3, "Curse of Abyss", AbaddonExchange, 2939)
	Text(3, "Curse of Styx", AbaddonExchange, 2940)
	Text(3, "- Back", JumpPage, 2)

	Talk(4, "Hocus Pocus: I heard human souls weigh 0.1g. I wonder how heavy does the soul of Death weighs.")
	Text(4, "Soul of Hardin", AbaddonExchange, 2562)
	Text(4, "Soul of Darkness", AbaddonExchange, 2563)
	Text(4, "Soul of Abaddon", AbaddonExchange, 2564)
	Text(4, "Soul of Asura", AbaddonExchange, 2565)
	Text(4, "Soul of Abyss", AbaddonExchange, 2566)
	Text(4, "Soul of Styx", AbaddonExchange, 2567)
	Text(4, "Soul of Black Dragon", AbaddonExchange, 2404)
	Text(4, "- Back", JumpPage, 2)

	InitTrigger()
	TriggerCondition(1, HasRecord, 330)
	TriggerAction(1, JumpPage, 1)
	TriggerCondition(2, NoRecord, 330)
	TriggerAction(2, JumpPage, 5)
	Start(GetMultiTrigger(), 2)
	
	AddNpcMission(520)
    AddNpcMission(900)
	AddNpcMission(595)
	AddNpcMission(599)
	AddNpcMission(5000)
	AddNpcMission(5001)
	AddNpcMission(5002)
	AddNpcMission(5003)
	AddNpcMission(5065)
	AddNpcMission(5066)
	AddNpcMission(5829)
	AddNpcMission(5830)	
end
------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk7()


	Talk( 1, "Sang Di: \"Hey guys! Come to receive the guest!\"" )
   	--Text( 1, "Trade", BuyPage )
	
	Talk( 2, "Sang Di: \"^$&#&^%\"" )

	InitTrade()
	Other(	1084	)
	Other(	1085	)
	Other(	1087	)
	Other(	1088	)
	Other(	1089	)
	Other(	1090	) 


	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )


	AddNpcMission ( 392 )
	AddNpcMission ( 514 )
	AddNpcMission ( 516 )
	AddNpcMission ( 527 )
	AddNpcMission ( 528 )
	AddNpcMission ( 529 )
	AddNpcMission ( 548 )
	AddNpcMission ( 550 )
	AddNpcMission ( 551 )
       AddNpcMission ( 558 )
       AddNpcMission ( 571 )
------------
	AddNpcMission(	592	)
	AddNpcMission(	5004	)
	AddNpcMission(	5005	)
	AddNpcMission(	5006	)
	AddNpcMission(	5007	)
	AddNpcMission(	5008	)
	AddNpcMission(	5043	)
	AddNpcMission(	5054	)
	AddNpcMission(	5055	)

	------------- -------
	AddNpcMission	(5831)
	AddNpcMission	(5832)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk8()


	Talk( 1, "Shuang: \"Sailing? Salvage? Repair? Refuel?\"" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 13 )
	TriggerAction( 1, LuanchBerthList, 13, 3426, 2575, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 13 )
	TriggerAction( 2, RepairBerthList, 13 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 13 )
	TriggerAction( 2, SupplyBerthList, 13 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 13 )
	TriggerAction( 1, SalvageBerthList, 13 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Shuang: \"^$&#&^%\"" )


	AddNpcMission ( 398 )
	AddNpcMission ( 565 )

	------------- -------
	AddNpcMission	(5833)
	AddNpcMission	(5834)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk9()
	Talk( 1, "Wang Mo: \"Youlook familiar? Do you have anything that requires storage?\"" )
	InitTrigger()
	TriggerCondition(1, HasMoney, 200)
	TriggerAction(1, TakeMoney, 200)
	TriggerAction(1, OpenBank)
	TriggerFailure(1, JumpPage, 2)
	Text(1, "Vault (200G Fee)", MultiTrigger, GetMultiTrigger(), 1)
	Talk(2, "Sorry, you do not have sufficient gold to pay for vault usage")
	Talk( 2, "Sorry! You do not have enough gold to pay the fee to access the bank." )
	Talk( 3, "Wang Mo: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 3 )
	Start( GetMultiTrigger(), 2 )



	AddNpcMission ( 399 )
	AddNpcMission ( 510 )
	AddNpcMission ( 511 )
	AddNpcMission ( 512 )
	AddNpcMission ( 513 )
	AddNpcMission ( 543 )
	AddNpcMission ( 544 )
        AddNpcMission ( 570 )

	------------- -------
	AddNpcMission	(5839)
	AddNpcMission	(5840)
end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk10()


	Talk( 1, "Zhao: \"So hungryWhat time is my dinner arriving?\"" )
	Talk( 2, "Zhao: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )

	------------- -------
	AddNpcMission	(5835)
	AddNpcMission	(5836)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk11()


	Talk( 1, "Zhou: \"1 plus 1equals 2? Not 2? 2? Actually I am only talking to myself. You don't have to be so worked up. Equals 2? Not equals 2? \"" )
	Talk( 2, "Zhou: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )

	AddNpcMission ( 531 )
	AddNpcMission ( 532 )
	AddNpcMission ( 533 )
	AddNpcMission ( 534 )

	------------- -------
	AddNpcMission	(5837)
	AddNpcMission	(5838)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk12()


	Talk( 1, "Cloud: \"Have you seen Misty? She is my girlfriend.\"" )
	Talk( 2, "Cloud: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )
         
	 AddNpcMission ( 533 )
	 AddNpcMission ( 563 )

	------------- -------
	AddNpcMission	(5841)
	AddNpcMission	(5842)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk13()


	Talk( 1, "Misty: \"Have you seen Cloud? He is my boyfriend\"" )
	Talk( 2, "Misty: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )

	------------- -------
	AddNpcMission	(5843)
	AddNpcMission	(5844)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk14()


	Talk( 1, "Bill: \"Trouble? No trouble? I am earning millions each second! Do not disturb me!\"" )
	Talk( 2, "Bill: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )

        AddNpcMission ( 568 )

	------------- -------
	AddNpcMission	(5847)
	AddNpcMission	(5848)


end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk15()


	Talk( 1, "Wang Rong: \"I does not know any martial art. Don't bully me\"" )
	Talk( 2, "Wang Rong: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )

	------------- -------
	AddNpcMission	(5845)
	AddNpcMission	(5846)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk16()


	Talk( 1, "Yakamoto: \"Seriouslycan't you see that we are robbers? \"" )
	Talk( 2, "Yakamoto: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )
--------------------------
	AddNpcMission	(5641)
	AddNpcMission	(5642)

	------------- -------
	AddNpcMission	(5849)
	AddNpcMission	(5850)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk17()


	Talk( 1, "Hairstylist: \"Braid\"" )
        Text( 1, "I want to change my hairstyle", JumpPage, 2)
	InitTrigger()
	TriggerCondition( 1, HasItem, 1807, 1 )
	TriggerAction( 1, OpenHair )
	TriggerFailure( 1, JumpPage, 3 )
	Talk( 2, "Hairstylist: Want a change of hairstyle? Let me have a look at your Hairstyling Coupon.")
	Text( 2, "Yes I'm sure", MultiTrigger, GetMultiTrigger(), 1)
	Text( 2, "Forget it, let me think it over.", CloseTalk )

	Talk( 3, "Hairstylist: You don't seem to possess Hairstyling Voucher. I cannot style your hair.")
	Talk( 4, "Hairstylist: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 4 )
	Start( GetMultiTrigger(), 2 )

	AddNpcMission	(6003)
	AddNpcMission	(6004)
	AddNpcMission	(6005)
	AddNpcMission	(6006)
	AddNpcMission	(6007)
	AddNpcMission	(6008)
	AddNpcMission	(6009)


	--------------------
	AddNpcMission	(5851)
	AddNpcMission	(5852)

end

------------------------------------------------------------
--  
------------------------------------------------------------

function b_talk18()


	Talk( 1, "Hami: \"\"" )
	Talk( 2, "Hami: \"^$&#&^%\"" )



	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 2 )
	Start( GetMultiTrigger(), 2 )
	
	AddNpcMission ( 515 )
	AddNpcMission ( 518 )
	AddNpcMission ( 524 )
end

------------------------------------------------------------
-- to
------------------------------------------------------------

function b_talk21()
	local ReSelectTalk = "I need to reconsider."
	local ReSelectPage = 1
	local CancelSelectTalk = "Forget it, I will stay here"
	local CancelSelectPage = 5
	local CurMapName = "heilong"
	local GoTo01X = 767
	local GoTo01Y = 602
	local GoTo01M = CurMapName

	Talk( 1, "Guardian Statue: \"I am the Goddess that sealed Death within.\"" )
	Text( 1, "Go to Black Dragon Lair 2!", JumpPage, 2 )

	InitTrigger()
	TriggerCondition( 1, HasItem, 179,1 )
	TriggerCondition( 1, HasItem, 3084,1 )
	TriggerCondition( 1, HasItem, 3085,1 )
	TriggerAction( 1, TakeItem, 179,1 )
	TriggerAction( 1, TakeItem, 3084,1 )
	TriggerAction( 1, TakeItem, 3085,1 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Teleport to Black Dragon Lair 2 requires 1<ySigil of Anubis>, 1 <yMask of Mummy King> and 1 <yTimeless Machine>!" )
	Text( 2, "Teleport",MultiTrigger, GetMultiTrigger(), 2 ) 
	Text( 2, CancelSelectTalk, JumpPage , CancelSelectPage )
	
	Talk( 3, "Guardian Statue: \"^$&#&^%\"" )

	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 3 )
	Start( GetMultiTrigger(), 3 )

	Talk( 4, "Sorry, you have not collect all 3 items!" )

	Talk( 5, "Remember to look for me again if you need to teleport", CloseTalk )
end

------------------------------------------------------------
-- to
------------------------------------------------------------

function b_talk22()
	local ReSelectTalk = "I need to reconsider."
	local ReSelectPage = 1
	local CancelSelectTalk = "Forget it, I will stay here"
	local CancelSelectPage = 4
	local CurMapName = "heilong"
	local GoTo01X = 847
	local GoTo01Y = 247
	local GoTo01M = CurMapName

	Talk( 1, "Guardian Statue: \"I am the Goddess that sealed Death within.\"" )
	Text( 1, "Go to Black Dragon Lair 1!", JumpPage, 2 )

	InitTrigger()
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	Talk( 2, "Do you wish to go out?" )
	Text( 2, "Teleport",MultiTrigger, GetMultiTrigger(), 2 ) 
	Text( 2, CancelSelectTalk, JumpPage , CancelSelectPage )
	
	Talk( 3, "Guardian Statue: \"^$&#&^%\"" )

	InitTrigger()
	TriggerCondition( 1, HasRecord, 330 )
	TriggerAction( 1, JumpPage, 1 )
	TriggerCondition( 2, NoRecord, 330 )
	TriggerAction( 2, JumpPage, 3 )
	Start( GetMultiTrigger(), 3 )

	Talk( 4, "Remember to look for me again if you need to teleport", CloseTalk )
end

------------------------------------------------------------
-- to
------------------------------------------------------------

function b_talk23()
	local CurMapName = "heilong"
	local GoTo01X = 772
	local GoTo01Y = 722
	local GoTo01M = CurMapName

	InitTrigger()
	TriggerCondition( 1, HasItem, 182,1 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 1, "Guardian Statue: \"I am the Goddess that sealed Death within.\"" )
	Text( 1, "Go to Black Dragon Lair 3!", MultiTrigger, GetMultiTrigger(), 1 )

	Talk( 4, "Statue is being sealed. You need a <Dragon Key> to access it" )
end

------------------------------------------------------------
--
------------------------------------------------------------

function b_talk24()
	InitTrigger()
	TriggerCondition( 1, HasItem, 182, 1 )
	TriggerAction( 1, TakeItem, 182, 1 )
	TriggerAction( 1, AddMoney, 10000000 )
	TriggerAction(1, GiveItem, 266, 1, 4)
	TriggerFailure( 1, JumpPage, 2 )
	Talk( 1, "This is a mysterious chest. It seems to be locked!" )
	Text( 1, "Use the Dragon Key", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 2, "Failed to open. It seems that some sort of key is needed")
end

------------------------------------------------------------
--  -----  
------------------------------------------------------------

function l_talk01()
	Talk(1, "Jack Arrow and I have been here for almost a month but failed to find the way out. And I don't know where is my loved one now! Arrgh! I hate this hellish place!")

	AddNpcMission(894)
	AddNpcMission(487)
	AddNpcMission(504)
	AddNpcMission(508)
	AddNpcMission(509)
	AddNpcMission(573)
	AddNpcMission(581)
	AddNpcMission(582)
	AddNpcMission(583)

end

------------------------------------------------------------
--  -----  
------------------------------------------------------------

function l_talk02()
	Talk(1, "Rumored that Barborosa got his monstrous power from the curse. This place is no longer safe, please be careful and these monsters aren't vegetarian.")

	AddNpcMission(895)
	AddNpcMission(896)
	AddNpcMission(482)
	AddNpcMission(485)
	AddNpcMission(486)
	AddNpcMission(574)
	AddNpcMission(575)
	AddNpcMission(584)
	AddNpcMission(585)
	AddNpcMission(586)

	------------
	
	AddNpcMission	(5676)
end

------------------------------------------------------------
--  -----  
------------------------------------------------------------

function l_talk03()
	Talk(1, "Damn Jack! If he did not steal our war galley, I would not be stuck in this hellhole")

	AddNpcMission(897)
	AddNpcMission(898)
	AddNpcMission(899)
	AddNpcMission(476)
	AddNpcMission(479)
	AddNpcMission(480)
	AddNpcMission(481)
	AddNpcMission(576)
	AddNpcMission(577)
	AddNpcMission(578)
	AddNpcMission(589)
	AddNpcMission(590)

end

------------------------------------------------------------
--  -----  
------------------------------------------------------------

function l_talk04()
	Talk(1, "Where am I? How did I ended up here? Mark! SobSob")
	Text( 1, "Trade", BuyPage )

	InitTrade()
	Other(3141)
	Other(4602)
	Other(4603)
	Other(4604)

	AddNpcMission(505)
	AddNpcMission(506)
	AddNpcMission(507)
	AddNpcMission(579)
	AddNpcMission(580)
	AddNpcMission(587)
	AddNpcMission(588)
	AddNpcMission(591)

	---------------
	AddNpcMission	(5573)
	AddNpcMission	(5574)

	--------------------------
	AddNpcMission	(5661)
	AddNpcMission	(5662)

	-----------------
	AddNpcMission( 6171 )
	AddNpcMission( 6172 )
end

------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk06()

	Talk( 1, "Penny: Dock? Set sail? Salvage? Repair? Refuel?" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 650,999, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Shuang: \"^$&#&^%\"" )




end

------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk05()

	Talk( 1, "Catchy: Dock? Set sail? Salvage? Repair? Refuel?" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 625,925, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Catchy: ..................^$&#&^%.................." )




end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk07()
Talk( 1, "Seaport Commander: Dock? Set sail? Salvage? Repair? Refuel?" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 620,628, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Seaport Commander: ..................^$&#&^%.................." )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk08()
Talk( 1, "Mysterious Sailor: Dock? Set sail? Salvage? Repair? Refuel?" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 380,161, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Mysterious Sailor: ..................^$&#&^%.................." )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk09()
Talk( 1, "Seaport Commander: Dock? Set sail? Salvage? Repair? Refuel?" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 1070,212, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "Sorry, you do not have a ship at the harbor or it was sunken. Please salvage it before setting sail again!" )
	Talk( 4, "Sorry! I only repair ships docked in this harbor. Please pay 1000G." )
	Talk( 5, "Sorry, you need to dock your ship in our harbor in order to refuel. Please pay 200G" )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Seaport Commander: ^$&#&^%" )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk10()
Talk( 1, "Zabu: Hi! I am the Freights NPC here. I can help you load the products up your ship if it is docked in this harbor" )
	InitTrigger()
	TriggerCondition( 1, HasBoatInBerth, 12)
	TriggerAction( 1, PackBagList, 12, RES_WOOD, 1 )
	TriggerFailure( 1, JumpPage, 2 )
	Text( 1, "Load Wood", MultiTrigger, GetMultiTrigger(), 1 )

	InitTrigger()
	TriggerCondition( 1, HasBoatInBerth, 12)
	TriggerAction( 1, PackBagList, 12, RES_MINE, 3 )
	TriggerFailure( 1, JumpPage, 2 )
	Text( 1, "Load Crystal", MultiTrigger, GetMultiTrigger(), 1 )

	Talk( 2, "Zabu:  Hi! You do not have any ship docked at this harbor" )

end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk11()
	--
	local CurMapName = "jialebi" 

	--
	local GoTo01X = 190
	local GoTo01Y = 815
	local GoTo01M = CurMapName


	InitTrigger()
	TriggerCondition( 1, HasItem, 2438,20 )
	TriggerCondition( 1, HasItem, 2419,15 )
	TriggerCondition( 1, HasItem, 2386,15 )
	TriggerAction( 1, TakeItem, 2438,20 )
	TriggerAction( 1, TakeItem, 2419,15 )
	TriggerAction( 1, TakeItem, 2386,15 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 1, "Abaddon Teleporter: Want to go to Abaddon? Be prepared. Bring me 20 <Gold Coins>, 15 <Pirate's Bones> and 15 <Souls of Undead> and I will send you down." )
	Text( 1, "Go to Abaddon 1", MultiTrigger, GetMultiTrigger(), 1 )

	--Talk( 3, "Guardian Statue: \"^$&#&^%\"" )

--InitTrigger()
--	TriggerCondition( 1, HasRecord, 330 )
--	TriggerAction( 1, JumpPage, 1 )
--	TriggerCondition( 2, NoRecord, 330 )
--	TriggerAction( 2, JumpPage, 3 )
--	Start( GetMultiTrigger(), 3 )

	Talk( 4, "Come back again after you have the required items" )

--	Talk( 4, "The statue emits a energy shield to keep me away. Looks like I am not strong enough to break it yet. I must think of other ways." )

---------------
	AddNpcMission	(5593)
	AddNpcMission	(5594)
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk12()
--
	local CurMapName = "jialebi" 


	--
	local GoTo01X = 1604
	local GoTo01Y = 906
	local GoTo01M = CurMapName

	Talk( 1, "Heaven Teleporter: I am a messenger of the Goddess. You have to go through me to go to Heaven. On a side note, Goddess will award you according to your Honor value. A higher Honor value will earn you greater rewards." )
	Text( 1, "I have been through hell", JumpPage, 2 )
	Text( 1, "I have obtained Goddess's Favor", JumpPage, 3 )
	Text( 1, "I heard that you have some other good items too", JumpPage, 7 )

	InitTrigger()
	TriggerCondition( 1, HasItem, 2488,1 )
	TriggerCondition( 1, HasItem, 2489,1 )
	TriggerCondition( 1, HasItem, 2490,1 )
	TriggerCondition( 1, HasItem, 2436,1 )
	TriggerAction( 1, TakeItem, 2488,1 )
	TriggerAction( 1, TakeItem, 2489,1 )
	TriggerAction( 1, TakeItem, 2490,1 )
	TriggerAction( 1, TakeItem, 2436,1 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Heaven Teleporter: Want to go to Heaven? Then you would have to collect for me 1 Bone of Mummy, 1 Soul of Corpse Soldier, 1 Heart of Pharoah and 1 Holy Bible." )
	Text( 2, "I want to go to Heaven now", MultiTrigger, GetMultiTrigger(), 1 )


	InitTrigger()
	TriggerCondition( 1, HasItem, 0938,1 )
	TriggerAction( 1, TakeItem, 0938,1 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 5 )
	Talk( 3, "Heaven Teleporter: You have the Goddess's Favor? I will send you in now." )
	Text( 3, "I want to go to Heaven now", MultiTrigger, GetMultiTrigger(), 1 )
	Text( 3, "Regarding Goddess's Favor", JumpPage, 6 )
	


	Talk( 4, "Come back again after you have the required items" )
	Talk( 5, "Goddess's Favor is not obtainable by everyone" )
	Talk( 6, "Goddess's Favor is left by the Goddess to help low level players to enter Heaven. Only players below Lv 60 can obtain it." )

	InitTrigger()
	TriggerCondition( 1, HasItem, 3457, 5 )
	TriggerCondition( 1, HasItem, 2386, 10 )
	TriggerAction( 1, TakeItem, 3457, 5 )
	TriggerAction( 1, TakeItem, 2386, 10 )
	TriggerAction( 1, GiveItem, 0271, 1, 4 )
	TriggerFailure( 1, JumpPage, 8 )
	Talk( 7, "Heaven Teleporter: Angelic Dice is a treasure that is left by the Goddess. You will need to give me 10 Soul of Undead and 5 Kal Runestones in exchange" )
	Text( 7, "Obtained Angelic Dice", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 8, "Heaven Teleporter: Come back again after you have the required items" )
---------------
	AddNpcMission	(5595)
	AddNpcMission	(5596)

	AddNpcMission ( 6056 )
	AddNpcMission ( 6057 )
	AddNpcMission ( 6058 )
	AddNpcMission ( 6059 )
	AddNpcMission ( 6060 )
------------------------------------04
	AddNpcMission	(5726)
	AddNpcMission	(5727)

end
------------------------------------------------------------
--  -----  A
------------------------------------------------------------
function mmm_talk13()
	Talk( 1, "Bewildered Researcher A: Hi, I have been lost for a long time. I got some good stuff with me, do you wish to do an exchange?" )
	Text( 1, "Redeem Secrets set item", JumpPage, 2)

	Talk( 2, "Bewildered Researcher A: These good items are also known as Tsunami set!" )
	Text( 2, "Redeem Armor of Secrets (Lance Crusader)", JumpPage, 3)
	Text( 2, "Redeem Gloves of Secrets (Lance Crusader)", JumpPage, 4)
	Text( 2, "Redeem Boots of Secrets (Lance Crusader)", JumpPage, 5)
	Text( 2, "Redeem Drakan (Lance Crusader)", JumpPage, 6)

	Talk(3,"To redeem Armor of Secrets requires 1 Guise Veilstone, 30 Rainbow Fruit, 20 Animal Skin, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2467, 1 )
	TriggerCondition( 1, HasItem, 3121, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2467, 1 )
	TriggerAction( 1, TakeItem, 3121, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0396, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 3, "Obtained Armor of Secrets", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Gloves of Secrets requires 1 Avow Blotstone, 30 Rainbow Fruit, 20 Hard Shell, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2473, 1 )
	TriggerCondition( 1, HasItem, 3121, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2473, 1 )
	TriggerAction( 1, TakeItem, 3121, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0588, 1 , 4)
	TriggerFailure( 1, JumpPage, 7)
	Text( 4, "Obtained Gloves of Secrets ", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Boots of Secrets requires 1 Mirage Shadestone, 30 Rainbow Fruit, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2479, 1 )
	TriggerCondition( 1, HasItem, 3121, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2479, 1 )
	TriggerAction( 1, TakeItem, 3121, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0748, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 5, "Obtained Boots of Secrets", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Drakan requires 1 Demonic Orestone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2463, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2463, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0114, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 6, "Obtained Drakan", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"Sorry, you do not have the required items to redeem")
end
------------------------------------------------------------
--  -----  B
------------------------------------------------------------
function mmm_talk14()
	Talk( 1, "Bewildered Researcher B: Hi, I have been lost for a long time. I got some good stuff with me, do you wish to do an exchange?" )
	Text( 1, "Redeem Olympus set", JumpPage, 2)

	Talk( 2, "Bewildered Researcher B: These good items are also known as Olympus set!" )
	Text( 2, "Redeem Armor of Olympus (Carsise Champion)", JumpPage, 3)
	Text( 2, "Redeem Gauntlets of Olympus (Carsise Champion)", JumpPage, 4)
	Text( 2, "Redeem Greaves of Olympus (Carsise Champion)", JumpPage, 5)
	Text( 2, "Redeem Colossus (Carsise Champion)", JumpPage, 6)

	Talk(3,"To redeem Armor of Olympus requires 1 Myth Flamestone, 30 Magical Branches, 20 Animal Skins, 10 Splendor Cloths and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2468, 1 )
	TriggerCondition( 1, HasItem, 1593, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2468, 1 )
	TriggerAction( 1, TakeItem, 1593, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0398, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 3, "Obtained Armor of Olympus", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Gauntlets of Olympus requires 1 Rainbow Soothstone, 30 Magical Branch, 20 Hard Shell, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2478, 1 )
	TriggerCondition( 1, HasItem, 1593, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2478, 1 )
	TriggerAction( 1, TakeItem, 1593, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0604, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 4, "Obtained Gauntlets of Olympus", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Greaves of Olympus requires 1 Spirit Orestone, 30 Magical Branch, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2484, 1 )
	TriggerCondition( 1, HasItem, 1593, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2484, 1 )
	TriggerAction( 1, TakeItem, 1593, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0830, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 5, "Obtained Greaves of Olympus", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Colossus requires 1 Thaw Finestone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2464, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2464, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0116, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 6, "Obtained Colossus", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"Sorry, you do not have the required items to redeem")
end
------------------------------------------------------------
--  -----  C
------------------------------------------------------------
function mmm_talk15()
	Talk( 1, "Bewildered Researcher C: Hi, I have been lost for a long time. I got some good stuff with me, do you wish to do an exchange?" )
	Text( 1, "Redeem Dragon Lord set", JumpPage, 2)
	Text( 1, "Redeem Tsunami set", JumpPage, 9)

	Talk( 2, "Bewildered Researcher C: These good items are also known as Dragon Lord set!" )
	Text( 2, "Redeem Dragon Lord Costume (Ami Voyager)", JumpPage, 3)
	Text( 2, "Redeem Dragon Lord Muffs (Ami Voyager)", JumpPage, 4)
	Text( 2, "Redeem Dragon Lord Shoes (Ami Voyager)", JumpPage, 5)
	Text( 2, "Redeem Dragon Lord Cap (Ami Voyager)", JumpPage, 6)
	Text( 2, "Redeem Riven Soul (Ami Voyager)", JumpPage, 7)

	Talk(3,"To redeem Dragon Lord Costume requires 1 Beastie Finestone, 30 Rainbow Glass, 20 Animal Skin, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2472, 1 )
	TriggerCondition( 1, HasItem, 1778, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2472, 1 )
	TriggerAction( 1, TakeItem, 1778, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0413, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 3, "Obtained Dragon Lord Costume", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Dragon Lord Muffs requires 1 Beastie Toothstone, 30 Rainbow Glass, 20 Hard Shell, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2477, 1 )
	TriggerCondition( 1, HasItem, 1778, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2477, 1 )
	TriggerAction( 1, TakeItem, 1778, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0602, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 4, "Obtained Dragon Lord Muffs", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Dragon Lord Shoes requires 1 Cadavar Orestone, 30 Rainbow Glass, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2483, 1 )
	TriggerCondition( 1, HasItem, 1778, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2483, 1 )
	TriggerAction( 1, TakeItem, 1778, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0824, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 5, "Obtained Dragon Lord Shoes ", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Dragon Lord Cap requires 1 Beastie Borestone, 30 Rainbow Glass, 20 Life Stones, 10 Meteorites and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2487, 1 )
	TriggerCondition( 1, HasItem, 1778, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 3391, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2487, 1 )
	TriggerAction( 1, TakeItem, 1778, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 3391, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 2223, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 6, "Obtained Dragon Lord Cap ", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"To redeem Riven Soul requires 1 Tear Soothstone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2466, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2466, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0151, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 7, "Obtained Riven Soul", MultiTrigger, GetMultiTrigger(), 1)

	Talk(8,"Sorry, you do not have the required items to redeem")

	Talk( 9, "Bewildered Researcher C: These good items are also known as Tsunami set!" )
	Text( 9, "Redeem Tsunami Robe (Lance, Phyllis Voyager)", JumpPage, 10)
	Text( 9, "Redeem Tsunami Gloves (Lance, Phyllis Voyager)", JumpPage, 11)
	Text( 9, "Redeem Tsunami Shoes (Lance, Phyllis Voyager)", JumpPage, 12)
	Text( 9, "Redeem Riven Soul (Lance, Phyllis Voyager)", JumpPage, 7)

	Talk(10,"To redeem Tsunami Robe requires 1 Beastie Finestone, 30 Agate Crystal, 20 Animal Skin, 10 Earthen Element Fragment and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2472, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2472, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0411, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 10, "Obtained Tsunami Robe", MultiTrigger, GetMultiTrigger(), 1)

	Talk(11,"To redeem Tsunami Gloves requires 1 Zest Jadestone, 30 Quartz Crystals, 20 Extender, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2474, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2474, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0600, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 11, "Obtained Tsunami Gloves", MultiTrigger, GetMultiTrigger(), 1)

	Talk(12,"To redeem Tsunami Shoes requires 1 Mirage Shadestone, 30 Agate Crystal, 20 Life Stone, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2479, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2479, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0760, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 12, "Obtained Tsunami Shoes", MultiTrigger, GetMultiTrigger(), 1)
end

------------------------------------------------------------
--  -----  D
------------------------------------------------------------
function mmm_talk26()
	Talk( 1, "Bewildered Researcher D: Hi, I have been lost for a long time. I got some good stuff with me, do you wish to do an exchange?" )
	Text( 1, "Redeem Fish Fairy set", JumpPage, 2)
	Text( 1, "Redeem Faerie set", JumpPage, 9)

	Talk( 2, "Bewildered Researcher D: These good items are also known as Fish Fairy set!" )
	Text( 2, "Redeem Fish Fairy Costume (Ami Cleric)", JumpPage, 3)
	Text( 2, "Redeem Fish Fairy Muffs (Ami Cleric)", JumpPage, 4)
	Text( 2, "Redeem Fish Fairy Shoes (Ami Cleric)", JumpPage, 5)
	Text( 2, "Redeem Fish Fairy Cap (Ami Cleric)", JumpPage, 6)
	Text( 2, "Redeem Revered Staff (Ami Cleric)", JumpPage, 7)

	Talk(3,"To redeem Fish Fairy Costume requires 1 Searing Debstone, 30 Life Stone Fragment, 20 Animal Skin, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2471, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2471, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0408, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 3, "Obtained Fish Fairy Costume", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Fish Fairy Muffs requires 1 Dark Soothstone, 30 Life Stone Fragment, 20 Hard Shell, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2476, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2476, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0598, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 4, "Obtained Fish Fairy Muffs", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Fish Fairy Shoes requires 1 Pearl Soothstone, 30 Life Stone Fragment, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2482, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2482, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0758, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 5, "Obtained Fish Fairy Shoes", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Fish Fairy Cap requires 1 Oceanic Gutstone, 30 Life Stone Fragment, 20 Life Stone, 10 Meteorite and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2486, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 3391, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2486, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 3391, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 2221, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 6, "Obtained Fish Fairy Cap", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"To redeem Revered Staff requires 1 Goddess Mudstone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2461, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2461, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0110, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 7, "Obtained Revered Staff", MultiTrigger, GetMultiTrigger(), 1)

	Talk(8,"Sorry, you do not have the required items to redeem")

	Talk( 9, "Bewildered Researcher D: These good items are also known as Faerie set!" )
	Text( 9, "Redeem Faerie Robe (Phyllis Cleric)", JumpPage, 10)
	Text( 9, "Redeem Faerie Gloves (Phyllis Cleric) ", JumpPage, 11)
	Text( 9, "Redeem Faerie Shoes (Phyllis Cleric)", JumpPage, 12)
	Text( 9, "Redeem Revered Staff (Phyllis Cleric)", JumpPage, 7)

	Talk(10,"To redeem Faerie Robe requires 1 Myth Flamestone, 30 Life Stone Fragment, 20 Life Stone, 10 Pearl and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2468, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2468, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0406, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 10, "Obtained Faerie Robe", MultiTrigger, GetMultiTrigger(), 1)

	Talk(11,"To redeem Faerie Gloves requires 1 Dark Soothstone, 30 Agate Crystal, 20 Animal Skin, 10 Splendor Cloth and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2476, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2476, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0596, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 11, "Obtained Faerie Gloves", MultiTrigger, GetMultiTrigger(), 1)

	Talk(12,"To redeem Faerie Shoes requires 1 Goddess Tearstone, 30 Life Stone Fragment, 20 Life Stone, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2480, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2480, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0756, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 12, "Obtained Faerie Shoes", MultiTrigger, GetMultiTrigger(), 1)
end

------------------------------------------------------------
--  -----  E
------------------------------------------------------------
function mmm_talk27()
	Talk( 1, "Bewildered Researcher E: Hi, I have been lost for a long time. I got some good stuff with me. Do you wish to do an exchange?" )
	Text( 1, "Redeem Panda set", JumpPage, 2)
	Text( 1, "Redeem Sage set", JumpPage, 9)

	Talk( 2, "Bewildered Researcher E: These good items are also known as Panda set!" )
	Text( 2, "Redeem Mystic Panda Costume (Ami Seal Master)", JumpPage, 3)
	Text( 2, "Redeem Mystic Panda Gloves (Ami Seal Master)", JumpPage, 4)
	Text( 2, "Redeem Mystic Panda Shoes (Ami Seal Master)", JumpPage, 5)
	Text( 2, "Redeem Mystic Panda Cap (Ami Seal Master)", JumpPage, 6)
	Text( 2, "Redeem Crimson Rod (Ami Seal Master)", JumpPage, 7)

	Talk(3,"To redeem Mystic Panda Costume requires 1 Dim Jadestone, 30 Quartz Crystal, 20 Animal Skin, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2470, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2470, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0404, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 3, "Obtained Mystic Panda Costume", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Mystic Panda Gloves requires 1 Worldstone, 30 Quartz Crystal, 20 Hard Shell, 10 Earthen Element Fragment and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2475, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2475, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0594, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 4, "Obtained Mystic Panda Gloves", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Mystic Panda Shoes requires 1 Crystal Zionstone, 30 Quartz Crystal, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2481, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2481, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0754, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 5, "Obtained Mystic Panda Shoes", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Mystic Panda Cap requires 1 Sky Shadestone, 30 Quartz Crystals, 20 Life Stones, 10 Meteorites and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2485, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1631, 20 )
	TriggerCondition( 1, HasItem, 3391, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2485, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1631, 20 )
	TriggerAction( 1, TakeItem, 3391, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 2219, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 6, "Obtained Mystic Panda Cap", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"To redeem Crimson Rod requires 1 Abrade Orestone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2462, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2462, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0112, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 7, "Obtained Crimson Rod", MultiTrigger, GetMultiTrigger(), 1)

	Talk(8,"Sorry, you do not have the required items to redeem")

	Talk( 9, "Bewildered Researcher E: These good items are also known as Sage set!" )
	Text( 9, "Redeem Robe of the Sage (Phyllis Seal Master)", JumpPage, 10)
	Text( 9, "Redeem Gloves of the Sage (Phyllis Seal Master)", JumpPage, 11)
	Text( 9, "Redeem Boots of the Sage (Phyllis Seal Master)", JumpPage, 12)
	Text( 9, "Redeem Crimson Rod (Phyllis Seal Master)", JumpPage, 7)

	Talk(10,"To redeem Robe of the Sage requires 1 Dim Jadestone, 30 Quartz Crystal, 20 Hard Shell, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2470, 1 )
	TriggerCondition( 1, HasItem, 1699, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2470, 1 )
	TriggerAction( 1, TakeItem, 1699, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0402, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 10, "Obtained Robe of the Sage", MultiTrigger, GetMultiTrigger(), 1)

	Talk(11,"To redeem Gloves of the Sage requires 1 Rainbow Soothstone, 30 Life Stone Fragment, 20 Extender, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2478, 1 )
	TriggerCondition( 1, HasItem, 1630, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2478, 1 )
	TriggerAction( 1, TakeItem, 1630, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0592, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 11, "Obtained Gloves of the Sage", MultiTrigger, GetMultiTrigger(), 1)

	Talk(12,"To redeem Boots of the Sage requires 1 Pearl Soothstone, 30 Agate Crystal, 20 Animal Skin, 10 Magical Bone and 50000")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2482, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2482, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0752, 1 , 4)
	TriggerFailure( 1, JumpPage, 8 )
	Text( 12, "Obtained Boots of the Sage", MultiTrigger, GetMultiTrigger(), 1)
end

------------------------------------------------------------
--  -----  F
------------------------------------------------------------
function mmm_talk28()
	Talk( 1, "Bewildered Researcher F: Hi, I have been lost for a long time. I got some good stuff with me, do you wish to do an exchange?" )
	Text( 1, "Redeem Apollo set", JumpPage, 2)

	Talk( 2, "Bewildered Researcher F: These good items are also known as Apollo set!" )
	Text( 2, "Redeem Vest of Apollo (Lance, Phyllis Sharpshooter)", JumpPage, 3)
	Text( 2, "Redeem Gloves of Apollo (Lance, Phyllis Sharpshooter) ", JumpPage, 4)
	Text( 2, "Redeem Boots of Apollo (Lance, Phyllis Sharpshooter)", JumpPage, 5)
	Text( 2, "Redeem Twilight (Lance, Phyllis Sharpshooter)", JumpPage, 6)

	Talk(3,"To redeem Vest of Apollo requires 1 Hervo Bloodstone, 30 Clarity Crystal, 20 Animal Skin, 10 Splendor Cloth and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2469, 1 )
	TriggerCondition( 1, HasItem, 3364, 30 )
	TriggerCondition( 1, HasItem, 1608, 20 )
	TriggerCondition( 1, HasItem, 3385, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2469, 1 )
	TriggerAction( 1, TakeItem, 3364, 30 )
	TriggerAction( 1, TakeItem, 1608, 20 )
	TriggerAction( 1, TakeItem, 3385, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0400, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 3, "Obtained Vest of Apollo", MultiTrigger, GetMultiTrigger(), 1)

	Talk(4,"To redeem Gloves of Apollo requires 1 Zest Jadestone, 30 Clarity Crystals, 20 Hard Shells, 10 Earthen Element Fragments and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2474, 1 )
	TriggerCondition( 1, HasItem, 3364, 30 )
	TriggerCondition( 1, HasItem, 1614, 20 )
	TriggerCondition( 1, HasItem, 1658, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2474, 1 )
	TriggerAction( 1, TakeItem, 3364, 30 )
	TriggerAction( 1, TakeItem, 1614, 20 )
	TriggerAction( 1, TakeItem, 1658, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0590, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 4, "Obtained Gloves of Apollo", MultiTrigger, GetMultiTrigger(), 1)

	Talk(5,"To redeem Boots of Apollo requires 1 Goddess Tearstone, 30 Clarity Crystal, 20 Special Gas, 10 Pearl and 50000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2480, 1 )
	TriggerCondition( 1, HasItem, 3364, 30 )
	TriggerCondition( 1, HasItem, 1657, 20 )
	TriggerCondition( 1, HasItem, 3361, 10 )
	TriggerCondition( 1, HasMoney, 50000 )
	TriggerAction( 1, TakeItem, 2480, 1 )
	TriggerAction( 1, TakeItem, 3364, 30 )
	TriggerAction( 1, TakeItem, 1657, 20 )
	TriggerAction( 1, TakeItem, 3361, 10 )
	TriggerAction( 1, TakeMoney, 50000 )

	TriggerAction( 1, GiveItem, 0750, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 5, "Obtained Boots of Apollo", MultiTrigger, GetMultiTrigger(), 1)

	Talk(6,"To redeem Twilight requires 1 Oceanic Soulstone, 30 Agate Crystal, 20 Extender, 10 Magical Bone and 100000G")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2465, 1 )
	TriggerCondition( 1, HasItem, 3380, 30 )
	TriggerCondition( 1, HasItem, 1652, 20 )
	TriggerCondition( 1, HasItem, 1626, 10 )
	TriggerCondition( 1, HasMoney, 100000 )
	TriggerAction( 1, TakeItem, 2465, 1 )
	TriggerAction( 1, TakeItem, 3380, 30 )
	TriggerAction( 1, TakeItem, 1652, 20 )
	TriggerAction( 1, TakeItem, 1626, 10 )
	TriggerAction( 1, TakeMoney, 100000 )

	TriggerAction( 1, GiveItem, 0118, 1 , 4)
	TriggerFailure( 1, JumpPage, 7 )
	Text( 6, "Obtained Twilight", MultiTrigger, GetMultiTrigger(), 1)

	Talk(7,"Sorry, you do not have the required items to redeem")
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk16()
	--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 1250
	local GoTo01Y = 1010
	local GoTo01M = CurMapName

	Talk( 1, "Want to find out what is underneath? Pay me 5000G and I will send you in!" )
	Text( 1, "Vile merchant! Never mind...I will pay", JumpPage, 2 )
	Text( 1, "Forget it! I do not have enough money to go", JumpPage, 3 )

	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Below leads to the Underground Dock of the Deathsouls. Are you brave enough to enter?" )
	Text( 2, "Yes! Go to Underground Dock!", MultiTrigger,GetMultiTrigger(), 1)

	Talk( 3, "Go away beggar!" )
	Talk( 4, "You do not have enough gold", CloseTalk)
end
------------------------------------------------------------
--  -----  A
------------------------------------------------------------
function mmm_talk17()
	--
	local CurMapName = "jialebi"

	--A
	local GoTo01X = 968
	local GoTo01Y = 828
	local GoTo01M = CurMapName

	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Vile merchant! Never mind...I will pay", JumpPage, 2 )
	Text( 1, "Forget it! I do not have enough money to go", JumpPage, 3 )
	
	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )

	Talk( 2, "The Research Room of the Deathsouls is on the inside. Do you dare to enter?" )
	Text( 2, "Go to Research Room A!", MultiTrigger,GetMultiTrigger(), 1)

	Talk( 3, "Go away beggar!" )
	Talk( 4, "You do not have enough gold", CloseTalk)
end
------------------------------------------------------------
--  -----  B2386
------------------------------------------------------------
function mmm_talk18()
	--
	local CurMapName = "jialebi"


	--B
	local GoTo01X = 968
	local GoTo01Y = 906
	local GoTo01M = CurMapName

	Talk( 1, "Do you wish to know what is inside? Bring me 20 Soul of Undead and I will let you in!" )
	Text( 1, "Bring me in", JumpPage, 2 )
	Text( 1, "Forget it! I do not want to go", JumpPage, 3 )

	InitTrigger()
	TriggerCondition( 1, HasItem, 2386, 20 )
	TriggerAction( 1, TakeItem, 2386, 20 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )

	Talk( 2, "Research Room B consists of a terrifying Deathsoul Commander. Do you still wish to enter?" )
	Text( 2, "Go to Research Room B!", MultiTrigger, GetMultiTrigger(), 1 )

	Talk( 3, "Go away beggar!" )
	Talk( 4, "You have not collect 20 Souls of Undead", CloseTalk)
end
------------------------------------------------------------
--  -----  C
------------------------------------------------------------
function mmm_talk19()
	--
	local CurMapName = "jialebi"


	--C
	local GoTo01X = 968
	local GoTo01Y = 993
	local GoTo01M = CurMapName

	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Vile merchant! Never mind...I will pay", JumpPage, 2 )
	Text( 1, "Forget it! I do not have enough money to go", JumpPage, 3 )

	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )

	Talk( 2, "Research Room C is on the other side" )
	Text( 2, "Yes! Go to Research Room C!", MultiTrigger, GetMultiTrigger(), 1 )

	Talk( 3, "Go away beggar!" )
	Talk( 4, "You do not have enough gold", CloseTalk)
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk20()
	--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 1076
	local GoTo01Y = 869
	local GoTo01M = CurMapName

	Talk( 1, "Want to know what is inside?" )
	Text( 1, "What is underneath?", JumpPage, 2 )
	Text( 1, "I do not wish to know", JumpPage, 3 )

	InitTrigger()
	TriggerCondition(1,HasItem , 2408,1)
	TriggerCondition(1,HasItem , 2409,1)
	TriggerCondition(1,HasItem , 2410,1)
	TriggerCondition(1,HasItem , 2411,1)
	TriggerCondition(1,HasItem , 2412,1)

	TriggerAction(1,TakeItem,2408,1)
	TriggerAction(1,TakeItem,2409,1)
	TriggerAction(1,TakeItem,2410,1)
	TriggerAction(1,TakeItem,2411,1)
	TriggerAction(1,TakeItem,2412,1)

	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "On the inside will be the Research Room of the Deathsouls. Many dangers lie in it and if you wish to enter, collect the full set of Passwords: AB, BC, CD, DE, EF." )
	Text( 2, "Go to Research Room!", MultiTrigger, GetMultiTrigger(), 1 )

	Talk( 3, "......Bye!" )
	Talk( 4, "You have not finish collecting all the Passwords", CloseTalk)
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk22()
	--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 79
	local GoTo01Y = 823
	local GoTo01M = CurMapName


	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Vile merchant! Never mind...I will pay", JumpPage, 2 )
	Text( 1, "Forget it! I do not have enough money to go", JumpPage, 3 )
	Text( 1, "I heard that you have some good items here", JumpPage, 5 )


	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )

	Talk( 2, "Go to Abaddon 2?" )
	Text( 2, "Yes! Go to Abaddon 2!", MultiTrigger, GetMultiTrigger(), 1 )
	
	Talk( 3, "Go away beggar!" )
	Talk( 4, "You do not have enough gold" ,CloseTalk)

	InitTrigger()
	TriggerCondition( 1, HasItem, 3457, 1 )
	TriggerAction( 1, TakeItem, 3457, 1 )
	TriggerAction( 1, GiveItem, 0267, 1, 4 )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 5, "Abaddon 1 Warden: Hi, I got some good stuff with me but don't expect me to give it for free. Bring me 1 Kal Runestone and I might reconsider" )
	Text( 5, "Obtain Power of Flame", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 6, "Abaddon 1 Warden: Come back again after you have the required items" )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk23()
	--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 199
	local GoTo01Y = 824
	local GoTo01M = CurMapName

	--
	local GoTo02X = 70
	local GoTo02Y = 952
	local GoTo02M = CurMapName


	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Go to Abaddon 1", JumpPage, 2 )
	Text( 1, "Go to Abaddon 3", JumpPage, 3 )
	Text( 1, "I heard that you have some good items here", JumpPage, 5 )

	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Do you wish to go to Abaddon 1?" )
	Text( 2, "Go to Abaddon 1", MultiTrigger, GetMultiTrigger(), 1 )

	InitTrigger()
	TriggerCondition(2,HasMoney,5000)
	TriggerAction(2,TakeMoney,5000)
	TriggerAction( 2, GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerFailure( 2, JumpPage, 4 )
	Talk( 3, "Go to Abaddon 3?" )
	Text( 3, "Yes! Go to Abaddon 3!", MultiTrigger, GetMultiTrigger(),2 )
	
	Talk( 4, "You do not have enough gold" ,CloseTalk)

	InitTrigger()
	TriggerCondition( 1, HasItem, 3457, 1 )
	TriggerAction( 1, TakeItem, 3457, 1 )
	TriggerAction( 1, GiveItem, 0268, 1, 4 )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 5, "Abaddon 2 Warden: Hi, I got some good stuff with me but don't expect me to give it for free. Bring me 1 Kal Runestone and I might reconsider" )
	Text( 5, "Obtained Power of Wind", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 6, "Abaddon 2 Warden: Come back again after you have the required items" )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk24()

	--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 79
	local GoTo01Y = 823
	local GoTo01M = CurMapName

	--
	local GoTo02X = 209
	local GoTo02Y = 955
	local GoTo02M = CurMapName


	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Go to Abaddon 2", JumpPage, 2 )
	Text( 1, "Go to Abaddon 4", JumpPage, 3 )
	Text( 1, "I heard that you have some good items here", JumpPage, 5 )

	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Go to Abaddon 2?" )
	Text( 2, "Yes! Go to Abaddon 2!", MultiTrigger, GetMultiTrigger(), 1 )

	InitTrigger()
	TriggerCondition(2,HasMoney,5000)
	TriggerAction(2,TakeMoney,5000)
	TriggerAction( 2, GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerFailure( 2, JumpPage, 4 )
	Talk( 3, "Do you wish to go to Abaddon 4?" )
	Text( 3, "Go to Abaddon 4", MultiTrigger, GetMultiTrigger(), 2 )
	
	Talk( 4, "You do not have enough gold" ,CloseTalk)

	InitTrigger()
	TriggerCondition( 1, HasItem, 3457, 1 )
	TriggerAction( 1, TakeItem, 3457, 1 )
	TriggerAction( 1, GiveItem, 0269, 1, 4 )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 5, "Abaddon 3 Warden: Hi, I got some good stuff with me but don't expect me to give it for free. Bring me 1 Kal Runestone and I might reconsider" )
	Text( 5, "Power of Thunder", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 6, "Abaddon 3 Warden: Come back again after you have the required items" )
end
------------------------------------------------------------
--  -----  
------------------------------------------------------------
function mmm_talk25()

		--
	local CurMapName = "jialebi"


	--
	local GoTo01X = 70
	local GoTo01Y = 952
	local GoTo01M = CurMapName

	--
	local GoTo02X = 690
	local GoTo02Y = 1043
	local GoTo02M = CurMapName


	Talk( 1, "Want to find out what is inside? Pay me 5000G and I will send you in!" )
	Text( 1, "Go back to Abaddon 3", JumpPage, 2 )
	Text( 1, "I want to leave this hellish place", JumpPage, 3 )
	Text( 1, "I heard that you have some good items here", JumpPage, 5 )

	InitTrigger()
	TriggerCondition(1,HasMoney,5000)
	TriggerAction(1,TakeMoney,5000)
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 4 )
	Talk( 2, "Go to Abaddon 3?" )
	Text( 2, "Yes! Go to Abaddon 3!", MultiTrigger, GetMultiTrigger(), 1 )

	InitTrigger()
	TriggerCondition(2,HasMoney,5000)
	TriggerAction(2,TakeMoney,5000)
	TriggerAction( 2, GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerFailure( 2, JumpPage, 4 )
	Talk( 3, "Do you wish to go out?" )
	Text( 3, "Yes! Let me out!", MultiTrigger, GetMultiTrigger(), 2 )
	
	Talk( 4, "You do not have enough gold" ,CloseTalk)

	InitTrigger()
	TriggerCondition( 1, HasItem, 3457, 1 )
	TriggerAction( 1, TakeItem, 3457, 1 )
	TriggerAction( 1, GiveItem, 0270, 1, 4 )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 5, "Abaddon 4 Warden: Hi, I got some good stuff with me but don't expect me to give it for free. Bring me 1 Kal Runestone and I might reconsider" )
	Text( 5, "Power of Frost", MultiTrigger, GetMultiTrigger(), 1)

	Talk( 6, "Abaddon 4 Warden: Come back again after you have the required items" )
end


------------------------------------------------------------
--  
------------------------------------------------------------

function mmm_talk29()
	--
	local ReSelectTalk = "I need to reconsider"
	local ReSelectPage = 1

	--
	local CancelSelectTalk = "Forget itI will stay here"
	local CancelSelectPage = 7

	--
	local CurMapName1 = "garner"
	local CurMapName2 = "magicsea"
	local CurMapName5 = "darkblue"

	--
	local GoTo01X = 2231
	local GoTo01Y = 2788
	local GoTo01M = CurMapName1

	--
	local GoTo02X = 890
	local GoTo02Y = 3575
	local GoTo02M = CurMapName2

	--
	local GoTo04X = 1318
	local GoTo04Y = 510
	local GoTo04M = CurMapName5

	Talk( 1, "Teleporter: Do you wish to leave the beautiful Treasure Gulf?" )
	Text( 1, "Go to Argent City!", JumpPage, 2 )
	Text( 1, "Go to Shaitan City!", JumpPage, 3 )
	Text( 1, "Go to Icicle City!", JumpPage, 4 )
	Text( 1, "Record Spawn point", JumpPage, 5 )
	
	InitTrigger()
	TriggerCondition( 1, HasMoney, 500 )
	TriggerAction( 1, TakeMoney, 500 )
	TriggerAction( 1, GoTo, GoTo01X, GoTo01Y, GoTo01M )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 2, "Teleports to Argent City? No problem! Please pay 500G." )
	Text( 2, "Teleport",MultiTrigger, GetMultiTrigger(), 2 ) 
	Text( 2, CancelSelectTalk, JumpPage , CancelSelectPage )


	InitTrigger()
	TriggerCondition( 1, HasMoney, 500 )
	TriggerAction( 1, TakeMoney, 500 )
	TriggerAction( 1,  GoTo, GoTo02X, GoTo02Y, GoTo02M )
	TriggerFailure( 2, JumpPage, 6 )
	Talk( 3, "I love Shaitan City! Please pay 500G" )
	Text( 3, "Teleport",MultiTrigger, GetMultiTrigger(), 2)
	Text( 3, CancelSelectTalk, JumpPage , CancelSelectPage )


	InitTrigger()
	TriggerCondition( 1, HasMoney, 500 )
	TriggerAction( 1, TakeMoney, 500 )
	TriggerAction( 1, GoTo, GoTo04X, GoTo04Y, GoTo04M )
	TriggerFailure( 1, JumpPage, 6 )
	Talk( 4, "Teleports to Icicle City? No problem! Please pay 500G." )
	Text( 4, "Teleport",MultiTrigger, GetMultiTrigger(), 2)
	Text( 4, CancelSelectTalk, JumpPage , CancelSelectPage )

	Talk( 6, "Sorry! You do not have enough gold to teleport." )

	Talk( 7, "Remember to look for me if you need teleportation services", CloseTalk )
	Talk( 8, "..................^$&#&^%.................." )



	InitTrigger()
	TriggerAction( 1, JumpPage, 1 )
	TriggerAction( 2, JumpPage, 8 )
	Start( GetMultiTrigger(), 2 )

	

end

function jjsj_talk1 ()

	InitTrigger()
	TriggerCondition(1, LvCheck, "<", 40)
	TriggerAction(1, ReAll)
	TriggerCondition(2, LvCheck, ">", 39)
	TriggerCondition(2, HasMoney, 2000)
	TriggerAction(2, TakeMoney, 2000)
	TriggerAction(2, ReAll)
	TriggerFailure(2, JumpPage, 2)

	Talk(1, "Lana: Hello! My name is Lana, I am the nurse of PKmap. Please come here when you need. It will charge you 2000 Gold, if your level is higher than 40. It is free if your level is below 40!")
	Text(1, "Greater Heal", MultiTrigger,GetMultiTrigger(),2) 

	Talk(2, "LanaSorry, Greater Heal will cost you 2000 Gold, it seems you dont have enough Gold")	

end 
function mmm_talk30()
	Talk(1, "Goddess: It must be tough on you making your way here. You can start great challenges or even get some rewards for your effort.")
	Text(1, "Special Request", JumpPage, 2)
	Text(1, "Rebirth Information", JumpPage, 4)
	Text(1, "Teleport", JumpPage, 7)
	
	Talk(2, "Goddess: I have some mystical precious stones with me at the moment. The effect remarkable if put to the correct use. I hope that you have some use for them. My messenger has discovered them during her time on the land. You need to be at least level 60 and have 5 Kal Runestones to exchange.")
	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2530, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Hephaestus", MultiTrigger, GetMultiTrigger(), 1)	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2533, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Hermes", MultiTrigger, GetMultiTrigger(), 1)	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2536, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Apollo", MultiTrigger, GetMultiTrigger(), 1)	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2539, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Poseidon", MultiTrigger, GetMultiTrigger(), 1)	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2542, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Hestia", MultiTrigger, GetMultiTrigger(), 1)	InitTrigger()
	TriggerCondition(1, HasItem, 3457, 5)
	TriggerCondition(1, LvCheck, ">", 59)
	TriggerAction(1, TakeItem, 3457, 5)
	TriggerAction(1, GiveItem, 2545, 1, 4)
	TriggerFailure(1, JumpPage, 3)
	Text(2, "Framestone: Athena", MultiTrigger, GetMultiTrigger(), 1)
	
	Talk(3, "Goddess: It seems you do not have enough Kal Runestones or you are not level 60 yet to exchange for framestones. Come back later.")
	
	Talk(4, "Goddess: Rebirth is a special process where you can reincarnate once more, this will reset your class, stat and skills so you can make more choices.")
	Text(4, "Phoenix Rebirth", JumpPage, 5)
	--Text(4, "Athene Rebirth", JumpPage, 6)
	
	Talk(5, "Goddess: This Rebirth is called Phoenix Rebirth, also called the First Rebirth.")
	Text(5, "Rebirth: Champion", PlayerRebirth, 1, 8)
	Text(5, "Rebirth: Crusader", PlayerRebirth, 1, 9)
	Text(5, "Rebirth: Sharpshooter", PlayerRebirth, 1, 12)
	Text(5, "Rebirth: Cleric", PlayerRebirth, 1, 13)
	Text(5, "Rebirth: Seal Master", PlayerRebirth, 1, 14)
	Text(5, "Rebirth: Voyager", PlayerRebirth, 1, 16)
	
	--Talk(6, "Goddess: This Rebirth is called Phoenix Rebirth, also called the First Rebirth.")
	--Text(6, "Rebirth: Champion", PlayerRebirth, 2, 8)
	--Text(6, "Rebirth: Crusader", PlayerRebirth, 2, 9)
	--Text(6, "Rebirth: Sharpshooter", PlayerRebirth, 2, 12)
	--Text(6, "Rebirth: Cleric", PlayerRebirth, 2, 13)
	--Text(6, "Rebirth: Seal Master", PlayerRebirth, 2, 14)
	--Text(6, "Rebirth: Voyager", PlayerRebirth, 2, 16)
	
	Talk(7, "Goddess: You wish to leave? Please choose a destination.")
	Text(7, "Teleport: Argent City", Teleport, "Argent City")
	Text(7, "Teleport: Shaitan City", Teleport, "Shaitan City")
	Text(7, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	
	AddNpcMission(6000)
	-- AddNpcMission(6277)
	-- AddNpcMission(6278)
	-- AddNpcMission(6366)
	-- AddNpcMission(6367)
	-- AddNpcMission(6380)
	-- AddNpcMission(6381)
end

function dydz_talk()


	Talk( 1, "Abaddon Vagabond: \"If you treasured your life, leave now! I have seen too much adventurers like you!\"" )
	InitTrigger()
	TriggerCondition( 1, HasAllBoatInBerth, 12 )
	TriggerAction( 1, LuanchBerthList, 12, 40,74, 180 )
	TriggerFailure( 1, JumpPage, 3 )
	Text( 1, "Set sail", MultiTrigger, GetMultiTrigger(), 1 )
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, RepairBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, RepairBerthList, 12 )
	TriggerFailure( 2, JumpPage, 4 )
	Text( 1, "Repair Ship", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasLuanchOut )
	TriggerAction( 1, SupplyBoat )
	TriggerCondition( 2, HasBoatInBerth, 12 )
	TriggerAction( 2, SupplyBerthList, 12 )
	TriggerFailure( 2, JumpPage, 5 )
	Text( 1, "Refuel", MultiTrigger, GetMultiTrigger(), 2 ) 
	InitTrigger()
	TriggerCondition( 1, HasDeadBoatInBerth, 12 )
	TriggerAction( 1, SalvageBerthList, 12 )
	TriggerFailure( 1, JumpPage, 6 )
	Text( 1, "Salvage Ship", MultiTrigger, GetMultiTrigger(), 1 ) 
	
	Talk( 3, "What are you thinking? You don't have a boat that you can use anymore. Now scram if you value your life!" )
	Talk( 4, "Don't think that your boat will be repaired for free here. Dock your boat and prepare a fee of 1000G." )
	Talk( 5, "Don't think you can refuel your ship here for free. Dock your ship and prepare a fee of 200G." )
	Talk( 6, "Sorry! We only salvage ships docked in our harbor. You have to pay a fee of 1000G." )
	Talk( 7, "Abaddon Vagabond: \"^$&#&^%\"" )

end


function dysr_talk ()

	Talk( 1, "Abaddon Merchant: On command of my lord, Death. I am here to provide help to you brave warriors, and to witness your deaths. Haha!" )
	Text( 1, "Trade", BuyPage)
	Text( 1, "Repair", OpenRepair)

	InitTrade()
	Other(	1847	)
	Other(	1848	)
	Other(	1849	)
	Other(	3143	)
	Other(	637	)
	Other(	819	)
	Other(	869	)
	Other(	874	)


end 


function zsts_talk()
	Talk( 1, "Rebirth Angel: Only the best and elite Navy or Pirates will have a chance to get rebirth!")
	Text( 1, "Information about Rebirth", JumpPage, 2)
	--Text( 1, "- Rebirth", JumpPage, 6)

	Talk( 2, "Rebirth Angel: What do you need to know?")
	Text( 2, "Classes after rebirth", JumpPage, 3)
	Text( 2, "Regarding stats and skill points after rebirth", JumpPage, 4)
	Text( 2, "Regarding conditions of rebirth", JumpPage, 5)
	
	Talk( 3, "Players can reselect their class after rebirth: Lance can become Crusader, Voyager or Sharpshooter. Carsise can become Champion. Phyllis can become Cleric, Seal Master, Voyager or Sharpshooter. Ami can become Cleric, Seal Master or Voyager")
	Text( 3, "Thank you, I understand", CloseTalk)

	Talk( 4, "All attributes will be resetted upon rebirth and all additional stat points gained from Career Lot or Birthday Cake will remained for reallocation. Also, all skill will be resetted and skill points will be returned for reallocation.")
	Text( 4, "Thank you, I understand", CloseTalk)

	Talk( 5, "To be rebirth, you will need to complete quest Phoenix Rebirth which can be activated at the Goddess. Look for me after you obtained the Rebirth Stone or you can come directly to me if you have the Rebirth Card.")
	Text( 5, "Thank you, I understand", CloseTalk)

	--Talk( 6, "Goddess: What would you like to be after rebirth" )
	--Text( 6, "Crusader to uphold justice and peace",  GetChaName1_born, 1)
	--Text( 6, "Champion to protect companion",  GetChaName2_born, 1)
	--Text( 6, "Voyager to sail the seas",  GetChaName3_born, 1)
	--Text( 6, "Sharpshooter to vanquish all enemies",  GetChaName4_born, 1)
	--Text( 6, "Cleric to heal the sick", GetChaName5_born, 1)
	--Text( 6, "Seal Master to do anything", GetChaName6_born, 1)

	--AddNpcMission ( 6071 )

end
function k_talk002()


	Talk( 1, ":,,!,,." )
	Text( 1, "", JumpPage, 2)

	Talk( 2, ":,,,!")	
	InitTrigger()
	TriggerCondition( 1, HasItem, 1097, 1)             ------------
	TriggerCondition(1, HasLeaveBagGrid, 1)
	TriggerCondition(1, KitbagLock, 0 )
	TriggerAction(1, TakeItem, 1097, 1)
	TriggerAction(1, GiveItem, 1104, 1, 4)              ----------
	TriggerFailure(1, JumpPage, 3)
	Text(2,"Confirm to redeem", MultiTrigger, GetMultiTrigger(), 1) 	

	Talk( 3, ",!!!... ...,,!")

	Text( 1, "10", JumpPage, 4)

	Talk( 4, ":,,,!")
	InitTrigger()
	TriggerCondition( 1, HasItem, 2989, 1)             ------------
	TriggerCondition(1, HasLeaveBagGrid, 1)
	TriggerCondition(1, KitbagLock, 0 )
	TriggerAction(1, TakeItem, 2989, 1)
	TriggerAction(1, GiveItem, 0992, 10, 4)              ----------10
	TriggerFailure(1, JumpPage, 5)
	Text(4,"Confirm to redeem", MultiTrigger, GetMultiTrigger(), 1) 	

	Talk( 5, ",... ...?!")

end
