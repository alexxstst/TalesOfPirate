print("-- [Loading] Mission Script [04]")

----------------------------------------------------------
--														--
--														--
--					Main Mission	 					--
--														--
--														--
----------------------------------------------------------
	--Main Quest Begins
function CenterMission001()

----------------------------wild beast forest
	DefineMission( 400, "Wandering Creatures", 400 )
	
	MisBeginTalk( "<t>The deep forest is a dangerous place to be in with ferocious bears on the prowl.<n><t>However, my nephew, <bBerry>, lives in the <pAbandoned Mine Haven> located at the edge of the forest. Could you visit him on my behalf and see if he's doing alright, brave adventurer?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 400)
	MisBeginCondition(NoMission, 400)
	MisBeginAction(AddMission, 400)
	MisCancelAction(ClearMission, 400)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bBerry> at <j(1893, 2812)>")
	
	MisHelpTalk("Berry is always playing around in the precinct of the Abandoned Mine Haven. I hope he is fine.")
	MisResultCondition(AlwaysFailure )

-----------------------------------wild beast forest
	DefineMission( 401, "Wandering Creatures", 400, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I am fine. The bear cubs are not as dangerous as what my uncle thinks. In fact, I love playing with them.")
	MisResultCondition(NoRecord, 400)
	MisResultCondition(HasMission, 400)
	MisResultAction(ClearMission, 400 )
	MisResultAction(SetRecord, 400 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------late desert business trip
	DefineMission( 402, "Late Arrival", 401 )
	
	MisBeginTalk( "<t>Hi! You've come at the right time! I am on close terms with the merchants from the desert. They travel annually to our city to do trading. However, there has been no sign of them and they are very very late.<n><t>Could you look for <bMarcus> in <pOasis Haven> and find out what's keeping them?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 401)
	MisBeginCondition(NoMission, 401)
	MisBeginAction(AddMission, 401)
	MisCancelAction(ClearMission, 401)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bMarcus> at <j(789, 3112)>.")
	
	MisHelpTalk("Hey! Why are you still here? Have you found Marcus?")
	MisResultCondition(AlwaysFailure )

-----------------------------------late desert business trip
	DefineMission( 403, "Late Arrival", 401, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Oh, the recent, sandstorms are preventing us from traveling, so we have to postpone the date. Don't worry we'll definitely come by Argent City. It's good business!")
	MisResultCondition(NoRecord, 401)
	MisResultCondition(HasMission, 401)
	MisResultAction(ClearMission, 401 )
	MisResultAction(SetRecord, 401 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------Icefield Adventure
	DefineMission( 404, "Tundra Exploration", 402 )
	
	MisBeginTalk( "<t>Hi my friend, you have progressed rapidly. These little animals are no longer your match.<n><t>I heard <pIcicle Haven> is looking to recruit new adventurers. Why don't you give it a try? I'll write you a recommendation letter to <bNana>.")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 402)
	MisBeginCondition(NoMission, 402)
	MisBeginAction(AddMission, 402)
	MisCancelAction(ClearMission, 402)
		
	MisNeed(MIS_NEED_DESP,"Look for <bNana> at <j(798, 369)> for greater challenges.")
	
	MisHelpTalk("Please look for Nana as soon as possible. She would to meet you.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Icefield Adventure
	DefineMission( 405, "Tundra Exploration", 402, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I have faith in your abilities since you <bPeter> recommended you.<n><t>You should thank him for his recommendation.")
	MisResultCondition(NoRecord, 402)
	MisResultCondition(HasMission, 402)
	MisResultAction(ClearMission, 402 )
	MisResultAction(SetRecord, 402 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------towards the desert
	DefineMission( 406, "To the Desert!", 403 )
	
	MisBeginTalk( "<t>Gracious me! I have not seen someone as new as you progress so quickly.<n><t>Sadly, these monster at the outskirts of Icicle city are no longer a threat to you. If you are interested, I can recommend you to <bKavosky> at <pOasis Haven> he's in need of a helping hand.")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 403)
	MisBeginCondition(NoMission, 403)
	MisBeginAction(AddMission, 403)
	MisCancelAction(ClearMission, 403)
		
	MisNeed(MIS_NEED_DESP,"Look for <bKavosky> at <j(779, 3098)>.")
	
	MisHelpTalk("Kavosky is very friendly. There's no need to feel shy about approaching him.")
	MisResultCondition(AlwaysFailure )

-----------------------------------towards the desert
	DefineMission( 407, "To the Desert!", 403, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I need your assistance. Please stay around and help out. Thanks!")
	MisResultCondition(NoRecord, 403)
	MisResultCondition(HasMission, 403)
	MisResultAction(ClearMission, 403 )
	MisResultAction(SetRecord, 403 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------Invitation from Silver
	DefineMission( 408, "Argent's Invitation", 404 )
	
	MisBeginTalk( "<t>Listen to me, my friend. Icicle city is a very cold place to be in and is not good for your health if you aren't used to it.<n><t>People from <pAbandoned Mine Haven> has been requesting help from me lately.<n><t>If you don't mind, could you travel there and look for <bKentaro>?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 404)
	MisBeginCondition(NoMission, 404)
	MisBeginAction(AddMission, 404)
	MisCancelAction(ClearMission, 404)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bKentaro> at <j(1894, 2798)>.")
	
	MisHelpTalk("You have not go and see him? Kentaro is waiting for your help!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Invitation from Silver
	DefineMission( 409, "Argent's Invitation", 404, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I have faith in your abilities since <bPeter> introduced you.<n><t>You should be grateful for his recommendation.")
	MisResultCondition(NoRecord, 404)
	MisResultCondition(HasMission, 404)
	MisResultAction(ClearMission, 404 )
	MisResultAction(SetRecord, 404 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------tumultuous ice field
	DefineMission( 410, "Stir of the Tundra", 405 )
	
	MisBeginTalk( "<t>Hey! You've arrived at the right time.<n><t><bJernis> is in trouble at <pIcicle Haven> and is in need of your help.<n><t>Since it there's nothing much here for you to do, why don't you make a trip there?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 405)
	MisBeginCondition(NoMission, 405)
	MisBeginAction(AddMission, 405)
	MisCancelAction(ClearMission, 405)
		
	MisNeed(MIS_NEED_DESP,"Look for <bJernis> at <j(803, 352)>.")
	
	MisHelpTalk("Why are you still hanging around? Seek out Jernis at Icicle Haven.")
	MisResultCondition(AlwaysFailure )

-----------------------------------tumultuous ice field
	DefineMission( 411, "Stir of the Tundra", 405, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Since you are recommended by Odessis, I have faith in your abilities.<n><t>Stick around and I will make the necessary arrangements.")
	MisResultCondition(NoRecord, 405)
	MisResultCondition(HasMission, 405 )
	MisResultAction(ClearMission, 405 )
	MisResultAction(SetRecord, 405 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------blocked material
	DefineMission( 412, "Trade Embargo", 406 )
	
	MisBeginTalk( "<t>Hi, I need your help. I am in charge of delivering these supplies to <pOasis Haven>. However, wolves has been constantly attacking travelers lately. Could you send a message to <bMarcus> at Oasis Haven and see if he can think of some way for me to reach him safely?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 406)
	MisBeginCondition(NoMission, 406)
	MisBeginAction(AddMission, 406)
	MisCancelAction(ClearMission, 406)
		
	MisNeed(MIS_NEED_DESP,"Look for <bMarcus> at <j(789, 3112)>")
	
	MisHelpTalk("Can you hurry please? These supplies are urgently required at Oasis Haven.")
	MisResultCondition(AlwaysFailure )

-----------------------------------blocked material
	DefineMission( 413, "Trade Embargo", 406, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>So grateful to you! Without these supplies, we won't last till next month.")
	MisResultCondition(NoRecord, 406)
	MisResultCondition(HasMission, 406 )
	MisResultAction(ClearMission, 406 )
	MisResultAction(SetRecord, 406 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------Brave recruitment
	DefineMission( 414, "Warrior Recruitment", 407 )
	
	MisBeginTalk( "<t>Listen to me, my friend. Shaitan city is a dusty place to be in and is not good for your health.<n><t>The <pAbandoned Mine Haven> has been requesting help from me lately.<n><t>If you don't mind, could you travel there and look for <bKentaro>?")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 407)
	MisBeginCondition(NoMission, 407)
	MisBeginAction(AddMission, 407)
	MisCancelAction(ClearMission, 407)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bKentaro> at <j(1894, 2798)>.")
	
	MisHelpTalk("You have not go and see him? <bKentaro> is waiting for your help!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Brave recruitment
	DefineMission( 415, "Warrior Recruitment", 407, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Oh, so <bResline> asked you to come. Actually, I don't really trust her judgment.<n><t>But since you are here...Oh well...")
	MisResultCondition(NoRecord, 407)
	MisResultCondition(HasMission, 407 )
	MisResultAction(ClearMission, 407 )
	MisResultAction(SetRecord, 407 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------The Goddess of the Ice City
	DefineMission( 416, "Asha Followers", 408 )
	
	MisBeginTalk( "<t>You've arrived just in time!<n><t>My good friend, <bGranny Dong>, moved to <pIcicle Haven> 10 years ago to spread the knowledge of medicine. However, the chill there is taking a toll on her health.<n><t>I need the help of a courageous adventurer to make the journey to send my blessings to her.")
	MisBeginCondition( LvCheck, ">", 9 )
	MisBeginCondition(NoRecord, 408)
	MisBeginCondition(NoMission, 408)
	MisBeginAction(AddMission, 408)
	MisCancelAction(ClearMission, 408)
		
	MisNeed(MIS_NEED_DESP,"Talk to <Granny Dong> at <j(795, 363)>")
	
	MisHelpTalk("I worried that <bGranny Dong> won't last much longer.<n><t>Please hurry!")
	MisResultCondition(AlwaysFailure )

-----------------------------------The Goddess of the Ice City
	DefineMission( 417, "Asha Followers", 408, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Thank you for conveying the blessings of the High Priest. I am feeling better already!")
	MisResultCondition(NoRecord, 408)
	MisResultCondition(HasMission, 408 )
	MisResultAction(ClearMission, 408 )
	MisResultAction(SetRecord, 408 )
	MisResultAction(AddExp, 100, 100)
	MisResultAction(AddMoney, 300, 300)

----------------------------Jungle Adventure
	DefineMission( 418, "Jungle Exploration", 409 )
	
	MisBeginTalk( "<t>Hi, I can see that you have a passion for exploration.<n><t>However, this is not the right place for you. Seek out <pDurian> at <pRockery Haven>. He has some challenging tasks for you.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 409)
	MisBeginCondition(NoMission, 409)
	MisBeginAction(AddMission, 409)
	MisCancelAction(ClearMission, 409)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bDurian> at <j(1535, 3071)>.")
	
	MisHelpTalk("Have you not make a move?<n><t>Hurry, time do not wait for anyone!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Jungle Adventure
	DefineMission( 419, "Jungle Exploration", 409, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Although you are recommended by <bWenona>, if you are not suitable for the job, I will entrust any tasks to you.")
	MisResultCondition(NoRecord, 409)
	MisResultCondition(HasMission, 409 )
	MisResultAction(ClearMission, 409 )
	MisResultAction(SetRecord, 409 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)

----------------------------travel east
	DefineMission( 420, "Journey to the East", 410 )
	
	MisBeginTalk( "<t>Hey, I know you.<n><t>Your reputation has spread far and wide in this desert. This Haven is too small for a person of your stature.<n><t>How about going to <pBabul Haven> and putting your ability to good use? <bSolaru> is currently in need of a person with your talents.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 410)
	MisBeginCondition(NoMission, 410)
	MisBeginAction(AddMission, 410)
	MisCancelAction(ClearMission, 410)
		
	MisNeed(MIS_NEED_DESP,"Look for <bSolaru> at <j(1202, 3179)>.")
	
	MisHelpTalk("Do not linger around here for too long. Go to Babul Haven!")
	MisResultCondition(AlwaysFailure )

-----------------------------------travel east
	DefineMission( 421, "Journey to the East", 410, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Oh, I've heard a lot about you. <bMarcus> has a flair for scouting talents.<n><t>Please stay around here and I'll inform you if I need help with any tasks at hand.")
	MisResultCondition(NoRecord, 410)
	MisResultCondition(HasMission, 410 )
	MisResultAction(ClearMission, 410 )
	MisResultAction(SetRecord, 410 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)


----------------------------looking for bodyguards
	DefineMission( 422, "In Search of a Bodyguard", 411 )
	
	MisBeginTalk( "<t>Hi, you sure looked bored. Is it because you are unable to find a worthy opponent here? If that is the case, let me introduce you to a businessman named <bUlkar> at <pAtlantis Haven>.<n><t>His trade route lies between dangerous forests and seems like a good job for the likes of you!")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 411)
	MisBeginCondition(NoMission, 411)
	MisBeginAction(AddMission, 411)
	MisCancelAction(ClearMission, 411)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bUlkar> at <j(1059, 661)>.")
	
	MisHelpTalk("Why don't you go and seek out Ulkar at Atlantis Haven?")
	MisResultCondition(AlwaysFailure )

-----------------------------------looking for bodyguards
	DefineMission( 423, "In Search of a Bodyguard", 411, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Since you are recommended by <bFey Fey>, I will make arrangements for you to aid us in the escort.")
	MisResultCondition(NoRecord, 411)
	MisResultCondition(HasMission, 411 )
	MisResultAction(ClearMission, 411 )
	MisResultAction(SetRecord, 411 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)


----------------------------forest adventure
	DefineMission( 424, "Forest Exploration", 412 )
	
	MisBeginTalk( "<t>Hey, I can see that you are a person who likes to take risks.<n><t>Too bad you are at a wrong place, there is nothing of excitement here.<n><t>How about I recommend you a place called <pRockery Haven>. I have a friend named <bAlexandis>, he'll definitely give you some exciting errands.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 412)
	MisBeginCondition(NoMission, 412)
	MisBeginAction(AddMission, 412)
	MisCancelAction(ClearMission, 412)
		
	MisNeed(MIS_NEED_DESP,"Look for <bAlexandis> at <j(1526, 3089)>.")
	
	MisHelpTalk("Get a move on! <bAlexandis> is waiting at Rockery Haven.")
	MisResultCondition(AlwaysFailure )

-----------------------------------forest adventure
	DefineMission( 425, "Forest Exploration", 412, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t><bKentaro> rarely praises anybody.<n><t>Since he trusts your ability, I will find a challenging task for you.")
	MisResultCondition(NoRecord, 412)
	MisResultCondition(HasMission, 412 )
	MisResultAction(ClearMission, 412 )
	MisResultAction(SetRecord, 412 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)


----------------------------Dangerous
	DefineMission( 426, "Dangerous Times", 413 )
	
	MisBeginTalk( "<t>Hi, I was just about to look for you. My friend, <bSolaru>, was badly injured by sand bandits a few days ago while he was out looking for treasure. He is now recovering at <pBabul Haven>.<n><t>The trip there is too dangerous for me to make. Could you regards to him on my behalf?")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 413)
	MisBeginCondition(NoMission, 413)
	MisBeginAction(AddMission, 413)
	MisCancelAction(ClearMission, 413)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bSolaru> at <j(1202, 3179)>.")
	
	MisHelpTalk("Have you met <bSolaru>? I am very worried!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Dangerous
	DefineMission( 427, "Dangerous Times", 413, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I'm fine. Its just that I cannot move as fast as I usually can just yet. Don't worry, I'll probably be well again in a few days.<n><t>Thank you for your concern!")
	MisResultCondition(NoRecord, 413)
	MisResultCondition(HasMission, 413 )
	MisResultAction(ClearMission, 413 )
	MisResultAction(SetRecord, 413 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)


----------------------------Support Arambis
	DefineMission( 428, "Reinforcement for Atlantis", 414 )
	
	MisBeginTalk( "<t>Hello. I believe the monsters here are not longer your match. You have been a great defender of our small haven all this while.<n><t>However, one of my friends in <pAtlantis Haven> is in greater need of your abilities. <n><t>Can you look for <bZurbi> see how you can help him out?")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 414)
	MisBeginCondition(NoMission, 414)
	MisBeginAction(AddMission, 414)
	MisCancelAction(ClearMission, 414)
		
	MisNeed(MIS_NEED_DESP,"Look for <bZurbi> at <j(1037, 671)>.")
	
	MisHelpTalk("Atlantis Haven requires your help.<n><t>Don't stay around here, go there now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Support Arambis
	DefineMission( 429, "Reinforcement for Atlantis", 414, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>This is great! We're need of some help.<n><t>Please stay around!")
	MisResultCondition(NoRecord, 414)
	MisResultCondition(HasMission, 414 )
	MisResultAction(ClearMission, 414 )
	MisResultAction(SetRecord, 414 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)

----------------------------Eye-opener
	DefineMission( 430, "Awakening", 415 )
	
	MisBeginTalk( "<t>Youngster, if you stay here, it will only be a waste of your time.<n><t>Why don't you go and explore the world while you are still young? My student, <bAlexandis>, is currently residing in <pRockery Haven>.<n><t>If you tell him that I sent you, he will surely welcome you.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 415)
	MisBeginCondition(NoMission, 415)
	MisBeginAction(AddMission, 415)
	MisCancelAction(ClearMission, 415)
		
	MisNeed(MIS_NEED_DESP,"Look for <bAlexandis> at <j(1526, 3089)>.")
	
	MisHelpTalk("<bAlexandis> is waiting for you at Rockery Haven. Please go now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Eye-opener
	DefineMission( 431, "Awakening", 415, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Oh you were sent here by my <bWeird Teacher>? Please make yourself at home.")
	MisResultCondition(NoRecord, 415)
	MisResultCondition(HasMission, 415 )
	MisResultAction(ClearMission, 415 )
	MisResultAction(SetRecord, 415 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)

----------------------------desert adventure
	DefineMission( 432, "Desert Exploration", 416 )
	
	MisBeginTalk( "<t>Hi, I can see that you have a passion for exploration. However, you have come to the wrong place. Seek out <bMomo> at <tBabul Haven>.<n><t>She has some challenging task for you.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 416)
	MisBeginCondition(NoMission, 416)
	MisBeginAction(AddMission, 416)
	MisCancelAction(ClearMission, 416)
		
	MisNeed(MIS_NEED_DESP,"Talk <bMomo> at <j(1209, 3196)>.")
	
	MisHelpTalk("What are you waiting for? Go to Babul Haven and look for <bMomo>!")
	MisResultCondition(AlwaysFailure )

-----------------------------------desert adventure
	DefineMission( 433, "Desert Exploration", 416, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Even though you are recommended by <bNana>, if you are not capable, I will have to turn you down.<bNana>.")
	MisResultCondition(NoRecord, 416)
	MisResultCondition(HasMission, 416 )
	MisResultAction(ClearMission, 416 )
	MisResultAction(SetRecord, 416 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)

----------------------------to the south
	DefineMission( 434, "Journey to the South", 417 )
	
	MisBeginTalk( "<t>Youngster, if you stay here, it will only be a waste of your time. Why don't you go and explore the world while you are still young? My student, <bAlexandis>, is currently residing in <pRockery Haven>.<n><t>If you tell him that I sent you, he will surely welcome you.")
	MisBeginCondition( LvCheck, ">", 19 )
	MisBeginCondition(NoRecord, 417)
	MisBeginCondition(NoMission, 417)
	MisBeginAction(AddMission, 417)
	MisCancelAction(ClearMission, 417)
		
	MisNeed(MIS_NEED_DESP,"Look for <bGasardis> at <j(1028, 649)>.")
	
	MisHelpTalk("<bGasardis> is at Atlantis Haven. Go now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------to the south
	DefineMission( 435, "Journey to the South", 417, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Sigh, that strange old man still likes to order me around...<n><t>Oh well, forget it and since you are already here, you might as well stay around.<n><t>I'm still lacking in manpower here anyway.")
	MisResultCondition(NoRecord, 417)
	MisResultCondition(HasMission, 417 )
	MisResultAction(ClearMission, 417 )
	MisResultAction(SetRecord, 417 )
	MisResultAction(AddExp, 500, 500)
	MisResultAction(AddMoney, 550, 550)

----------------------------dream ocean
	DefineMission( 436, "Dream Sea", 418 )
	
	MisBeginTalk( "<t>Oh dear, don't tell me that you have yet to explore the ocean!?<n><t>Let me introduce you to <pArgent City> <bMaritime Assistant - Alena>. She can teach you all you need to know.")
	MisBeginCondition( LvCheck, ">", 24 )
	MisBeginCondition(NoRecord, 418)
	MisBeginCondition(NoRecord, 419)
	MisBeginCondition(NoRecord, 420)
	MisBeginCondition(NoMission, 418)
	MisBeginCondition(NoMission, 419)
	MisBeginCondition(NoMission, 420)
	MisBeginAction(AddMission, 418)
	MisCancelAction(ClearMission, 418)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bAlena> at <j(2247, 2858)>.")
	
	MisHelpTalk("Alena is at Argent Harbor. Please go to her now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------dream ocean
	DefineMission( 437, "Dream Sea", 418, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Don't worry. With my help you will be out at sea in no time!!")
	MisResultCondition(NoRecord, 418)
	MisResultCondition(HasMission, 418 )
	MisResultAction(ClearMission, 418 )
	MisResultAction(SetRecord, 418 )
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney, 700, 700)

----------------------------Naval office report
	DefineMission( 438, "Navy Registration Now Open!", 419 )
	
	MisBeginTalk( "<t>Aye my friend. Have you not been to sea before? I guess its about time you did.<n><t>Let me introduce you to <pArgent City>'s <bMaritime Assistant - Alena>. She can teach you all you need to know about sailing.")
	MisBeginCondition( LvCheck, ">", 24 )
	MisBeginCondition(NoRecord, 418)
	MisBeginCondition(NoRecord, 419)
	MisBeginCondition(NoRecord, 420)
	MisBeginCondition(NoMission, 418)
	MisBeginCondition(NoMission, 419)
	MisBeginCondition(NoMission, 420)
	MisBeginAction(AddMission, 419)
	MisCancelAction(ClearMission, 419)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bAlena> at <j(2247, 2858)>.")
	
	MisHelpTalk("<bAlena> is at Argent Harbor. Please go to her now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Naval office report
	DefineMission( 439, "Navy Registration Now Open!", 419, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Don't worry. With my help you will be out at sea in no time!!")
	MisResultCondition(NoRecord, 419)
	MisResultCondition(HasMission, 419 )
	MisResultAction(ClearMission, 419 )
	MisResultAction(SetRecord, 419 )
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney, 700, 700)

----------------------------Navy Office
	DefineMission( 440, "The Navy Office", 420 )
	
	MisBeginTalk( "<t>Oh dear, don't tell me that you have yet to explore the ocean!?<n><t>Let me introduce you to <pArgent City> <bMaritime Assistant - Alena>. She can teach you all you need to know.")
	MisBeginCondition( LvCheck, ">", 24 )
	MisBeginCondition(NoRecord, 418)
	MisBeginCondition(NoRecord, 419)
	MisBeginCondition(NoRecord, 420)
	MisBeginCondition(NoMission, 418)
	MisBeginCondition(NoMission, 419)
	MisBeginCondition(NoMission, 420)
	MisBeginAction(AddMission, 420)
	MisCancelAction(ClearMission, 420)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bAlena> at <j(2247, 2858)>.")
	
	MisHelpTalk("Alena is at Argent Harbor. Please go to her now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Navy Office
	DefineMission( 441, "The Navy Office", 420, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Don't worry. With my help you will be out at sea in no time!!")
	MisResultCondition(NoRecord, 420)
	MisResultCondition(HasMission, 420 )
	MisResultAction(ClearMission, 420 )
	MisResultAction(SetRecord, 420 )
	MisResultAction(AddExp, 1000, 1000)
	MisResultAction(AddMoney, 700, 700)

----------------------------No. 1 businessman in the world
	DefineMission( 442, "World's Finest Merchant", 421 )
	
	MisBeginTalk( "<t>Finally you have arrived! I am on good terms with <bChiroro>, the Trader in <pArgent City>. Recently, he wishes to take in some students to pass on his knowledge of sea commerce.<n><t>If you are interested in learning more about sea commerce, go and see <bChiroro>.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 421)
	MisBeginCondition(NoRecord, 422)
	MisBeginCondition(NoRecord, 423)
	MisBeginCondition(NoMission, 421)
	MisBeginCondition(NoMission, 422)
	MisBeginCondition(NoMission, 423)
	MisBeginAction(AddMission, 421)
	MisCancelAction(ClearMission, 421)
		
	MisNeed(MIS_NEED_DESP,"Talk to <b<bTrader Chiroro>> at <j(2331, 2821)>.")
	
	MisHelpTalk("Oh dear! Why are you still here? Aren't you eager to learn? <b<bTrader Chiroro>> is in Argent city.")
	MisResultCondition(AlwaysFailure )

-----------------------------------No. 1 businessman in the world
	DefineMission( 443, "World's Finest Merchant", 421, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>You want to be the best trader?<n><t>Good! I expected that much from you.")
	MisResultCondition(NoRecord, 421)
	MisResultCondition(HasMission, 421 )
	MisResultAction(ClearMission, 421 )
	MisResultAction(SetRecord, 421 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------silver exchange
	DefineMission( 444, "Argent Trade Center", 422 )
	
	MisBeginTalk( "<t>Finally you have arrived! I am on good terms with <bChiroro>, the Trader of <pArgent City>.<n><t>Recently, he wishes to take in some students to pass on his knowledge of sea commerce.<n><t>If you are interested in learning more about sea commerce, go and see <bChiroro>.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 421)
	MisBeginCondition(NoRecord, 422)
	MisBeginCondition(NoRecord, 423)
	MisBeginCondition(NoMission, 421)
	MisBeginCondition(NoMission, 422)
	MisBeginCondition(NoMission, 423)
	MisBeginAction(AddMission, 422)
	MisCancelAction(ClearMission, 422)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bTrader Chiroro> at <j(2331, 2821)>.")
	
	MisHelpTalk("Oh dear! Why are you still here? Aren't you eager to learn? <bTrader Chiroro> is in Argent city.")
	MisResultCondition(AlwaysFailure )

-----------------------------------silver exchange
	DefineMission( 445, "Argent Trade Center", 422, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>You want to be the best trader?<n><t>Good! I expected that much from you.")
	MisResultCondition(NoRecord, 422)
	MisResultCondition(HasMission, 422 )
	MisResultAction(ClearMission, 422 )
	MisResultAction(SetRecord, 422 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Overseas transactions
	DefineMission( 446, "Sea Commerce", 423 )
	
	MisBeginTalk( "<t>Finally you have arrived! I am on good terms with <bChiroro>, the Trader of <pArgent City>.<n><t>Recently, he wishes to take in some students to pass on his knowledge of sea commerce.<n><t>If you are interested in learning more about sea commerce, go and see <bChiroro>.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 421)
	MisBeginCondition(NoRecord, 422)
	MisBeginCondition(NoRecord, 423)
	MisBeginCondition(NoMission, 421)
	MisBeginCondition(NoMission, 422)
	MisBeginCondition(NoMission, 423)
	MisBeginAction(AddMission, 423)
	MisCancelAction(ClearMission, 423)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bTrader Chiroro> at <j(2331, 2821)>.")
	
	MisHelpTalk("Oh dear! Why are you still here? Aren't you eager to learn? <bTrader Chiroro> is in Argent city.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Overseas transactions
	DefineMission( 447, "Sea Commerce", 423, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>You want to be the best trader?<n><t>Good! I expected that much from you.")
	MisResultCondition(NoRecord, 423)
	MisResultCondition(HasMission, 423 )
	MisResultAction(ClearMission, 423 )
	MisResultAction(SetRecord, 423 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------attacked swordsman
	DefineMission( 448, "Ambushed Swordsman", 424)
	
	MisBeginTalk( "<t>Oh dear!<n><t>Even someone as skilled as <bKentaro> got hurt by a monster inside the <pAbandoned Mine Haven>.<n><t>I am quite worried about the situation down there. Can you go to <pAbandoned Mine> and take a look?")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 424)
	MisBeginCondition(NoMission, 424)
	MisBeginAction(AddMission, 424)
	MisCancelAction(ClearMission, 424)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bKentaro> at <j(1894, 2798)>.")
	
	MisHelpTalk("Since you made a promise, please go now.")
	MisResultCondition(AlwaysFailure )

-----------------------------------attacked swordsman
	DefineMission( 449, "Ambushed Swordsman", 424, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I enraged those moles by accident while in the mine and got injuired.<n><t>Now my wound has recovered.")
	MisResultCondition(NoRecord, 424)
	MisResultCondition(HasMission, 424 )
	MisResultAction(ClearMission, 424 )
	MisResultAction(SetRecord, 424 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)


----------------------------Travel through Ryan Forest
	DefineMission( 450, "Traversing", 425)
	
	MisBeginTalk( "<t>Dangerous creatures lie in wait on the pass through the <pAndes Forest> to <pValhalla Haven>. Many people do not dare to venture there.<n><t>However, my son is doing research at the area. Can you take pity on a frail old woman like me and pass a message to my son, <bWaala>?<n><t>I wish that he will come and visit me.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 425)
	MisBeginCondition(NoMission, 425)
	MisBeginAction(AddMission, 425)
	MisCancelAction(ClearMission, 425)
		
	MisNeed(MIS_NEED_DESP,"Send a message to <bWaala> at <j(1136, 2778)>.")
	
	MisHelpTalk("I am not trying to rush you but I hope that <bWaala> is able to get my message as soon as possible.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Travel through Ryan Forest
	DefineMission( 451, "Traversing", 425, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Did my mother say that? Oh my, looks like I have been too engrossed in my work.")
	MisResultCondition(NoRecord, 425)
	MisResultCondition(HasMission, 425 )
	MisResultAction(ClearMission, 425 )
	MisResultAction(SetRecord, 425 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)


----------------------------maze unsolved
	DefineMission( 452, "The Labyrinth Mystery", 426)
	
	MisBeginTalk( "<t><pIcicle Castle> used to be very majestic. However, the rocky structures have start to corrode over the years.<n><t>Monsters have also started to dwell beneath the castle and recently, there are cases of missing people. <b<bMas>>, the Royal of <pIcicle Castle> need my son, Waala, to investigate this matter. However, my son is busy with his research.<n><t>Can you tell <b<bMas>> to look for someone else?")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 426)
	MisBeginCondition(NoMission, 426)
	MisBeginAction(AddMission, 426)
	MisCancelAction(ClearMission, 426)
		
	MisNeed(MIS_NEED_DESP,"Send a message to <bMas> at <j(1346, 451)>.")
	
	MisHelpTalk("Icicle Royal, <bMas> is still waiting for my reply. Please do not let him wait for too long.")
	MisResultCondition(AlwaysFailure )

-----------------------------------maze unsolved
	DefineMission( 453, "The Labyrinth Mystery", 426, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>OhSince Waala cannot make it, I will find other people.<n><t>Thank you for sending the letter over.")
	MisResultCondition(NoRecord, 426)
	MisResultCondition(HasMission, 426 )
	MisResultAction(ClearMission, 426 )
	MisResultAction(SetRecord, 426 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Abandoned mine exploration
	DefineMission( 454, "Mine Exploration", 427)
	
	MisBeginTalk( "<t>Due to centuries of mining, the Abandoned Mine near Argent City is like an underground labyrinth.<n><t>Lots of creatures has start to grow in there after so many years.<n><t>Perhaps you might find some challenging monsters in there. Go and see <bGreg> at the <pAbandoned Mine Haven>, she can tell you more about the place.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 427)
	MisBeginCondition(NoMission, 427)
	MisBeginAction(AddMission, 427)
	MisCancelAction(ClearMission, 427)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bGreg> at <j(1883, 2805)>.")
	
	MisHelpTalk("I believe you will like to explore the maze beneath the Abandoned Mine. Hurry up and go!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Abandoned mine exploration
	DefineMission( 455, "Mine Exploration", 427, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I have faith in your abilities since <bPeter> introduced you.<n><t>You should be grateful for his recommendation.")
	MisResultCondition(NoRecord, 427)
	MisResultCondition(HasMission, 427 )
	MisResultAction(ClearMission, 427 )
	MisResultAction(SetRecord, 427 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Crisis in Wasafar
	DefineMission( 456, "Waala's Crisis", 428)
	
	MisBeginTalk( "<t>What took you so long?!<n><t>My dear <bWaala> is doing research at <pValhalla Haven>. However, he has lost his supplies and medicine along the way.<n><t>Can you help me deliver these items and check on him?")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 428)
	MisBeginCondition(NoMission, 428)
	MisBeginAction(AddMission, 428)
	MisCancelAction(ClearMission, 428)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bWaala> at <j(1136, 2778)>.")
	
	MisHelpTalk("Please hurry! <bWaala> needs these items in order to survive.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Crisis in Wasafar
	DefineMission( 457, "Waala's Crisis", 428, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Water...Water...Ah...I feel better now.<n><t>Thank you! I'll be fine after a short rest.")
	MisResultCondition(NoRecord, 428)
	MisResultCondition(HasMission, 428 )
	MisResultAction(ClearMission, 428 )
	MisResultAction(SetRecord, 428 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Mysterious Discovery
	DefineMission( 458, "Mysterious Discovery", 429)
	
	MisBeginTalk( "<t>Hey, I found a map of Icicle castle by accident while researching ancient tomes.<n><t>The chairman of Icicle, <bReyno> will be very interested to learn about this. Can you pass a message to him?")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 429)
	MisBeginCondition(NoMission, 429)
	MisBeginAction(AddMission, 429)
	MisCancelAction(ClearMission, 429)
		
	MisNeed(MIS_NEED_DESP,"Send a message to <bIcicle Chairman Reyno> at <j(1294, 498)>.")
	
	MisHelpTalk("Please take these to <bIcicle Chairman Reyno>. This discovery is important!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Mysterious Discovery
	DefineMission( 459, "Mysterious Discovery", 429, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>This is an amazing discovery! Thank you for informing me!")
	MisResultCondition(NoRecord, 429)
	MisResultCondition(HasMission, 429 )
	MisResultAction(ClearMission, 429 )
	MisResultAction(SetRecord, 429 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Abandoned Mine Labyrinth
	DefineMission( 460, "Mine Labyrinth", 430)
	
	MisBeginTalk( "<t>Due to centuries of mining, the Abandoned Mine near Argent City is like an underground labyrinth. Lots of creatures has start to grow in there after so many years.<n><t>Perhaps you might find some challenging monsters in there.<n><t>Go and see Greg at the Abandoned Mine Haven, she can tell you more about the place.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 430)
	MisBeginCondition(NoMission, 430)
	MisBeginAction(AddMission, 430)
	MisCancelAction(ClearMission, 430)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bGreg> at <j(1883, 2805)>.")
	
	MisHelpTalk("I believe you will like to explore the maze beneath the Abandoned Mine. Hurry up and go!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Abandoned Mine Labyrinth
	DefineMission( 461, "Mine Labyrinth", 430, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Even though <bDurian> recommended you, if you are not suitable for the job, I will entrust any tasks to you.")
	MisResultCondition(NoRecord, 430)
	MisResultCondition(HasMission, 430 )
	MisResultAction(ClearMission, 430 )
	MisResultAction(SetRecord, 430 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Looking for talent
	DefineMission( 462, "Search for Talent", 431)
	
	MisBeginTalk( "<t>Hi. My friend, <bGuiderius> is looking for a reliable person to help him out at <pValhalla Haven>. Since he has entrusted me with this request, I am not going to disappoint him.<n><t>Therefore, I have decided to recommend you. There will be a lot of challenging monsters. Are you ready?")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 431)
	MisBeginCondition(NoMission, 431)
	MisBeginAction(AddMission, 431)
	MisCancelAction(ClearMission, 431)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bGuiderius> at <j(1123, 2766)>.")
	
	MisHelpTalk("<bGuiderius> is waiting for you, please hurry!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Looking for talent
	DefineMission( 463, "Search for Talent", 431, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>I have faith in your abilities since Ulkar recommended you.<n><t>I will assign you some important tasks should anything arise.")
	MisResultCondition(NoRecord, 431)
	MisResultCondition(HasMission, 431 )
	MisResultAction(ClearMission, 431 )
	MisResultAction(SetRecord, 431 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------maze exploration
	DefineMission( 464, "Labyrinth Exploration", 432)
	
	MisBeginTalk( "<t>My friend, I believed that the monsters here are no longer your match. Why don't you make a trip to <pIcicle City>?<n><t>The Chairman of Icicle city, <bReyno>, is currently looking for fighters to deal with the monster threat that's plaguing the city.<n><t>If you are willing, I can write you a recommendation letter.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 432)
	MisBeginCondition(NoMission, 432)
	MisBeginAction(AddMission, 432)
	MisCancelAction(ClearMission, 432)
		
	MisNeed(MIS_NEED_DESP,"Talk to <bIcicle Chairman Reyno> at <j(1294, 498)>.")
	
	MisHelpTalk("I think you are strong enough to deal with the monsters at <pLower Icicle City>! Go forth and make a name for yourself!")
	MisResultCondition(AlwaysFailure )

-----------------------------------maze exploration
	DefineMission( 465, "Labyrinth Exploration", 432, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Even though you are recommended by <bZurbi>, if you are not capable, I will have no choice but to turn you away.")
	MisResultCondition(NoRecord, 432)
	MisResultCondition(HasMission, 432 )
	MisResultAction(ClearMission, 432 )
	MisResultAction(SetRecord, 432 )
	MisResultAction(AddExp, 2000, 2000)
	MisResultAction(AddMoney, 850, 850)

----------------------------Jedi is born
	DefineMission( 466, "Til We Meet", 433)
	
	MisBeginTalk( "<t>Praise to the gods! My friend, <bHoho> managed to escape death after a deep cut from the katana of a <rSkeleton Warrior>. <n><t>I wanted to visit him but the horrific monsters at <pSkeleton Haven> are preventing me.<n><t>Please do me a favor and visit <bHoho> on my behalf.")
	MisBeginCondition( LvCheck, ">", 29 )
	MisBeginCondition(NoRecord, 433)
	MisBeginCondition(NoMission, 433)
	MisBeginAction(AddMission, 433)
	MisCancelAction(ClearMission, 433)
		
	MisNeed(MIS_NEED_DESP,"Send a message to <bHoho> at <j(2142, 556)>.")
	
	MisHelpTalk("Please be beware of the skeletons around Skeleton Haven!")
	MisResultCondition(AlwaysFailure )

-----------------------------------Jedi is born
	DefineMission( 467, "Til We Meet", 433, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Thanks a lot! Most of my friends were terrified to visit me after I moved here.")
	MisResultCondition(NoRecord, 433)
	MisResultCondition(HasMission, 433 )
	MisResultAction(ClearMission, 433 )
	MisResultAction(SetRecord, 433 )
	MisResultAction(AddExp, 3500, 3500)
	MisResultAction(AddMoney, 1000, 1000)

----------------------------Eastern Mentor
	DefineMission( 468, "Oriental Mentor", 434)
	
	MisBeginTalk( "<t>Dear adventurer! I have been looking for my teacher, <b<bMas>ter Kerra>, for many years around Skeleton Haven but to no avail. Recently, I heard that she has moved to <pIcespire Haven>.<n><t>If you are going there, can you tell her that I have already acquired the \"Heart of Will\"?")
	MisBeginCondition( LvCheck, ">", 39 )
	MisBeginCondition(NoRecord, 434)
	MisBeginCondition(NoMission, 434)
	MisBeginAction(AddMission, 434)
	MisCancelAction(ClearMission, 434)
		
	MisNeed(MIS_NEED_DESP,"Look for <bMaster Kerra> at <j(2664, 654)>.")
	
	MisHelpTalk("Go towards east from here and you will find Icespire Haven. It may be dangerous along the way but I believed that your determination will see you through.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Eastern Mentor
	DefineMission( 469, "Oriental Mentor", 434, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Thank you for visiting me. Looks like my beloved student has finally graduated!")
	MisResultCondition(NoRecord, 434)
	MisResultCondition(HasMission, 434 )
	MisResultAction(ClearMission, 434 )
	MisResultAction(SetRecord, 434 )
	MisResultAction(AddExp, 6000, 6000)	
	MisResultAction(AddMoney, 1000, 1000)

----------------------------doctor diversion
	DefineMission( 470, "Occupation Change", 435)
	
	MisBeginTalk( "<t>Oh dear! Can you help me out? My sister, <bDoctor - Chivo>, used to live in Shaitan city. Recently, she moved to <pChaldea Haven> but that place is so isolated, it does not even have a water source!<n><t>Can you visit her on my behalf?")
	MisBeginCondition( LvCheck, ">", 44 )
	MisBeginCondition(NoRecord, 435)
	MisBeginCondition(NoMission, 435)
	MisBeginAction(AddMission, 435)
	MisCancelAction(ClearMission, 435)
		
	MisNeed(MIS_NEED_DESP,"Visit <bDocter Chivo> at <j(630, 2091)>.")
	
	MisHelpTalk("Chaldea Haven is just south of Thundoria Castle.")
	MisResultCondition(AlwaysFailure )

-----------------------------------doctor diversion
	DefineMission( 471, "Occupation Change", 435, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Haha! My brother sure likes to poke his nose into everything. Actually, I enjoy living here in this place.")
	MisResultCondition(NoRecord, 435)
	MisResultCondition(HasMission, 435 )
	MisResultAction(ClearMission, 435 )
	MisResultAction(SetRecord, 435 )
	MisResultAction(AddExp, 10000, 10000)	
	MisResultAction(AddMoney, 1100, 1100)

----------------------------Thunder Navy
	DefineMission( 472, "Thundoria Navy", 436)
	
	MisBeginTalk( "<t>I heard that <pThundoria Castle> is recruiting adventurers. There should be lots of tasks there for you to do.<n><t>I can recommend you to Guard Captain <bMallack> if you are interested.")
	MisBeginCondition( LvCheck, ">", 49 )
	MisBeginCondition(NoRecord, 436)
	MisBeginCondition(NoMission, 436)
	MisBeginAction(AddMission, 436)
	MisCancelAction(ClearMission, 436)
		
	MisNeed(MIS_NEED_DESP,"Meet up with <bMallack> in Thundoria Castle at <j(706, 1465)>.")
	
	MisHelpTalk("Trust me. Go to Thundoria Castle right now and you might become an officer there.")
	MisResultCondition(AlwaysFailure )

-----------------------------------Thunder Navy
	DefineMission( 473, "Thundoria Navy", 436, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Hi, do you wish to stay here temporarily?<n><t>No problem! Just bear in mind not to break any rules.")
	MisResultCondition(NoRecord, 436)
	MisResultCondition(HasMission, 436 )
	MisResultAction(ClearMission, 436 )
	MisResultAction(SetRecord, 436 )
	MisResultAction(AddExp, 16000, 16000)
	MisResultAction(AddMoney, 1200, 1200)


----------------------------deep in the forest
	DefineMission( 474, "Deep Deep Forest", 437)
	
	MisBeginTalk( "<t>Deep forest of Ascaron, are many tribesmen. One of my friend, Old Cuwen, stays at the nearby <pSolace Haven>. He send letters to me often, asking me to visit him.<n><t>However, I cannot leave due to my busy work schedule. Can you pay him a visit on my behalf?")
	MisBeginCondition( LvCheck, ">", 54 )
	MisBeginCondition(NoRecord, 437)
	MisBeginCondition(NoMission, 437)
	MisBeginAction(AddMission, 437)
	MisCancelAction(ClearMission, 437)
		
	MisNeed(MIS_NEED_DESP,"Visit <bOld Cuwen> at <j(529, 2458)>.")
	
	MisHelpTalk("Thank you my friend. Please visit <pSolace Haven> when you are free.")
	MisResultCondition(AlwaysFailure )

-----------------------------------deep in the forest
	DefineMission( 475, "Deep Deep Forest", 437, COMPLETE_SHOW )
	
	MisBeginCondition(AlwaysFailure )
		
	MisResultTalk("<t>Thank you for visiting. I cannot believe that Sonny still remember me after such a long time.")
	MisResultCondition(NoRecord, 437)
	MisResultCondition(HasMission, 437 )
	MisResultAction(ClearMission, 437 )
	MisResultAction(SetRecord, 437 )
	MisResultAction(AddExp, 25000, 25000)
	MisResultAction(AddMoney, 1300, 1300)

------------------------------------------------------------
--                           				              --
--        Pirates of the Carribean Common Quests          --
--                           				              --
------------------------------------------------------------

-- General Warrington >> Commander's Grief

	DefineMission(476, "Commander's Complaint", 438)

	MisBeginTalk("<t>The Navy Commander of Thundoria has just sent an order for me to return there.<n><t>However, with the current situation, I am unable to make the trip myself. Can you do it on my behalf?<n><t>Yes, go back and check with the old fellow and report back to me.")
	MisBeginCondition(NoRecord, 438)
	MisBeginCondition(NoMission,438)
	MisBeginCondition(HasNavyGuild)
	MisBeginAction(AddMission, 438)
	MisCancelAction(ClearMission, 438)
	MisHelpTalk("<t>You have not make a move? Please hurry!")

	MisNeed(MIS_NEED_DESP,"Report to Thundoria Castle's <bNavy Commander> on behalf of Wellington.")

	MisResultCondition(AlwaysFailure)

-- Thunderfort Naval Commander >> Commander's Grief
	DefineMission(477,"Commander's Complaint",438,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Oh, so Wellington cannot come himself. Never mind.")

	MisResultCondition(HasMission, 438)
	MisResultCondition(NoRecord, 438)
	MisResultCondition(HasNavyGuild)
	MisResultAction(ClearMission, 438)
	MisResultAction(SetRecord, 438)

-- Thunderfort Naval Commander >> Commander's Secret
	DefineMission(478, "Secret of the Commander", 439)

	MisBeginTalk("<t>Since General Wellington cannot make it here himself, please deliver this letter to him.<n><t>Remember to keep it secure at all time as the letter is of utmost importance!")
	MisBeginCondition(HasRecord, 438)
	MisBeginCondition(NoRecord, 439)
	MisBeginCondition(NoMission,439)
	MisBeginCondition(HasNavyGuild)
	MisBeginBagNeed(1)
	MisBeginAction(AddMission, 439)
	MisBeginAction(GiveItem, 2430, 1, 4)
	MisCancelAction(ClearMission, 439)
	MisHelpTalk("<t>You have not make a move? Please hurry!")
	MisNeed(MIS_NEED_DESP,"Send the letter of Thundoria Castle's <bNavy Commander> to Wellington safely.")

	MisResultCondition(AlwaysFailure)

-- General Warrington >> Commander's Secret

	DefineMission(479,"Secret of the Commander",439,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>So this is the order passed down from the commander? Looks like we have to think of something.<n><t>Soldier, have a rest first. We will be busy once again.")

	MisResultCondition(HasMission, 439)
	MisResultCondition(NoRecord, 439)
	MisResultCondition(HasItem, 2430,1 )
	MisResultCondition(HasNavyGuild)
	MisResultAction(ClearMission, 439)
	MisResultAction(TakeItem, 2430,1 )
	MisResultAction(SetRecord, 439)
	MisResultAction(AddExp, 10000, 10000)
	MisResultAction(AddMoney, 25000, 25000)

-- General Warrington >> Naval Weapons

	DefineMission(480, "Navy's Weapon", 440)

	MisBeginTalk("<t>Are you ready? Listen carefully then.<n><t>I command you on behalf of the commander, sneak into the Armory of the enemy base and kill some Deathsouls. Bring back 5 Soul of Undead as proof.")

	MisBeginCondition(HasRecord, 439)
	MisBeginCondition(NoMission, 440)
	MisBeginCondition(NoRecord, 440)
	MisBeginCondition(HasNavyGuild)
	MisBeginAction(AddMission, 440)
	MisBeginAction(AddTrigger, 4401, TE_GETITEM, 2386, 5)
	
	MisNeed(MIS_NEED_ITEM, 2386, 5, 10, 5)

	MisCancelAction(ClearMission, 440)
	
	MisHelpTalk("<t>Have you collected the 5 Soul of Undead? Victory is near!")
	MisResultTalk("<t>Not too bad, this is all that is required. Maybe you should join us haha! Just a comment, don't worry.")

	MisResultCondition(HasMission, 440)
	MisResultCondition(NoRecord, 440)
	MisResultCondition(HasItem, 2386, 5)
	MisResultCondition(HasNavyGuild)
	MisResultAction(TakeItem, 2386, 5)
	MisResultAction(ClearMission, 440)
	MisResultAction(SetRecord, 440)
	MisResultAction(AddExp, 250000, 250000)
	MisResultAction(AddMoney, 150000, 150000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2386)
	TriggerAction(1, AddNextFlag, 440, 10, 5)
	RegCurTrigger(4401)

-- Admiral Warrington >> Naval Cheats
	
	DefineMission(481,"Navy Manual",441)

	MisBeginTalk("<t> After recent operations, the military might of the Deathsouls have been reduced greatly.<n><t>At the same time, we have also devised some strategies to counter them and recorded them in the Tome. However, it was stolen recently by a bunch of spy from the army of Deathsouls. This is a great insult to us!<n><t>I want you to sneak into their Naval Base and retrieve all 8 pages of the Tome itself. Get me a Superglue from the Deathsoul Commander himself as well.<n><t>Do not come back if you don't have all of the items!")

	MisBeginCondition(HasRecord, 440)
	MisBeginCondition(NoRecord, 441)
	MisBeginCondition(NoMission, 441)
	MisBeginCondition(HasNavyGuild)
	MisBeginAction(AddMission, 441)
	MisBeginAction(AddTrigger, 4411, TE_GETITEM, 2388, 1)
	MisBeginAction(AddTrigger, 4412, TE_GETITEM, 2389, 1)
	MisBeginAction(AddTrigger, 4413, TE_GETITEM, 2390, 1)
	MisBeginAction(AddTrigger, 4414, TE_GETITEM, 2391, 1)
	MisBeginAction(AddTrigger, 4415, TE_GETITEM, 2392, 1)
	MisBeginAction(AddTrigger, 4416, TE_GETITEM, 2393, 1)
	MisBeginAction(AddTrigger, 4417, TE_GETITEM, 2394, 1)
	MisBeginAction(AddTrigger, 4418, TE_GETITEM, 2395, 1)
	MisBeginAction(AddTrigger, 4419, TE_GETITEM, 2441, 1)

	MisNeed(MIS_NEED_ITEM, 2388, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 2389, 1, 20, 1)
	MisNeed(MIS_NEED_ITEM, 2390, 1, 30, 1)
	MisNeed(MIS_NEED_ITEM, 2391, 1, 40, 1)
	MisNeed(MIS_NEED_ITEM, 2392, 1, 50, 1)
	MisNeed(MIS_NEED_ITEM, 2393, 1, 60, 1)
	MisNeed(MIS_NEED_ITEM, 2394, 1, 70, 1)
	MisNeed(MIS_NEED_ITEM, 2395, 1, 80, 1)
	MisNeed(MIS_NEED_ITEM, 2441, 1, 90, 1)

	MisCancelAction(ClearMission, 441)

	MisHelpTalk("<t>We must not allow those Deathsoul to learn of our strategy from the Tome itself!")
	
	MisResultTalk("<t>This...This is it...You are really our hero!<n><t>This is unbelievable! I can't simply express my joys through words, I need to cool down...Oh right...These are your rewards, take it.")

	MisResultCondition(HasMission, 441)
	MisResultCondition(NoRecord, 441)
	MisResultCondition(HasItem, 2388, 1)
	MisResultCondition(HasItem, 2389, 1)
	MisResultCondition(HasItem, 2390, 1)
	MisResultCondition(HasItem, 2391, 1)
	MisResultCondition(HasItem, 2392, 1)
	MisResultCondition(HasItem, 2393, 1)
	MisResultCondition(HasItem, 2394, 1)
	MisResultCondition(HasItem, 2395, 1)
	MisResultCondition(HasItem, 2441, 1)
	MisResultBagNeed(1)
	MisResultCondition(HasNavyGuild)

	MisResultAction(TakeItem, 2388, 1)
	MisResultAction(TakeItem, 2389, 1)
	MisResultAction(TakeItem, 2390, 1)
	MisResultAction(TakeItem, 2391, 1)
	MisResultAction(TakeItem, 2392, 1)
	MisResultAction(TakeItem, 2393, 1)
	MisResultAction(TakeItem, 2394, 1)
	MisResultAction(TakeItem, 2395, 1)
	MisResultAction(TakeItem, 2441, 1)
	MisResultAction(ClearMission, 441)
	MisResultAction(SetRecord, 441)
	MisResultAction(AddExp, 500000, 500000)
	MisResultAction(AddMoney, 500000, 500000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2388)
	TriggerAction(1, AddNextFlag, 441, 10, 1)
	RegCurTrigger(4411)

	InitTrigger()
	TriggerCondition(1, IsItem, 2389)
	TriggerAction(1, AddNextFlag, 441, 20, 1)
	RegCurTrigger(4412)

	InitTrigger()
	TriggerCondition(1, IsItem, 2390)
	TriggerAction(1, AddNextFlag, 441, 30, 1)
	RegCurTrigger(4413)

	InitTrigger()
	TriggerCondition(1, IsItem, 2391)
	TriggerAction(1, AddNextFlag, 441, 40, 1)
	RegCurTrigger(4414)

	InitTrigger()
	TriggerCondition(1, IsItem, 2392)
	TriggerAction(1, AddNextFlag, 441, 50, 1)
	RegCurTrigger(4415)

	InitTrigger()
	TriggerCondition(1, IsItem, 2393)
	TriggerAction(1, AddNextFlag, 441, 60, 1)
	RegCurTrigger(4416)

	InitTrigger()
	TriggerCondition(1, IsItem, 2394)
	TriggerAction(1, AddNextFlag, 441, 70, 1)
	RegCurTrigger(4417)

	InitTrigger()
	TriggerCondition(1, IsItem, 2395)
	TriggerAction(1, AddNextFlag, 441, 80, 1)
	RegCurTrigger(4418)

	InitTrigger()
	TriggerCondition(1, IsItem, 2441)
	TriggerAction(1, AddNextFlag, 441, 90, 1)
	RegCurTrigger(4419)



-- Jack Sparrow >> CLICK FOR HELP
	DefineMission(482, "SOS", 442)

	MisBeginTalk("<t>I have to admit that the military might of the Deathsouls far exceeds our expectation, so I have decided to call on my old foe, Captain Jack Arrow for help.<n><t>He is residing in some part of the Magical Ocean at the moment. Can you make a trip there on my behalf? Chances of success are slim but give it a try.")
	MisBeginCondition(NoRecord, 442)
	MisBeginCondition(NoMission,442)
	MisBeginCondition(HasPirateGuild)
	MisBeginAction(AddMission, 442)
	MisCancelAction(ClearMission, 442)
	MisHelpTalk("<t>You have not make a move? Please hurry!")

	MisNeed(MIS_NEED_DESP,"Send a help request to Captain Jack in Magical Ocean.")

	MisResultCondition(AlwaysFailure)

-- Captain Jack >> Help

	DefineMission(483,"SOS",442,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>What! He dare to request aid from me? What a joke.<n><t>Go back and tell him: If he needs my aid, win me in a duel then!")

	MisResultCondition(HasMission, 442)
	MisResultCondition(NoRecord, 442)
	MisBeginCondition(HasPirateGuild)
	MisResultAction(ClearMission, 442)
	MisResultAction(SetRecord, 442)

--  >> 
	DefineMission(484, "Jack Vs Jack", 443)

	MisBeginTalk("<t>This is the challenge letter I have written. Bring it to Jack Arrow.<n><t>Tell him that I will be waiting for him at the pavilion on the midnight of the full month of the eighth month. ")
	MisBeginCondition(HasRecord, 442)
	MisBeginCondition(NoRecord, 443)
	MisBeginCondition(NoMission,443)
	MisBeginCondition(HasPirateGuild)
	MisBeginBagNeed(1)

	MisBeginAction(AddMission, 443)
	
	MisCancelAction(ClearMission, 443)
	MisBeginAction(GiveItem, 2431, 1, 4)
	MisHelpTalk("<t>You have not make a move? Stop dreaming!")

	MisNeed(MIS_NEED_DESP,"Send Captain Jack's Challenge Letter to Jack Arrow.")

	MisResultCondition(AlwaysFailure)

-- Jack Sparrow >> Jack Sees Jack Again

	DefineMission(485,"Jack Vs Jack",443,COMPLETE_SHOW)

	MisBeginCondition(AlwaysFailure)

	MisResultTalk("<t>Sigh...Looks like he will not provide us with any assistance. We will think of a way ourselves.<n><t>It has been tough on you. Have a rest first.")

	MisResultCondition(HasItem,2431,1)
	MisResultCondition(HasMission, 443)
	MisResultCondition(NoRecord, 443)
	MisResultCondition(HasPirateGuild)
	MisResultAction(ClearMission, 443)
	MisResultAction(TakeItem,2431,1)
	MisResultAction(SetRecord, 443)
	MisResultAction(AddExp, 10000, 10000)
	MisResultAction(AddMoney, 50000, 50000)

-- Jack Sparrow >> Caribbean Treasures

	DefineMission(486, "Treasure of the Caribbean", 444)

	MisBeginTalk("<t>Skeletar Isle has many Soul Gathering Coffins and the Heart of Davy Jones remains hidden in one of them.<n><t>We can make use of it if it can be found. Will you help me to retrieve the Heart of Davy Jones?<n><t>It will give us enough power to repel the invasion of the Deathsoul army.")

	MisBeginCondition(HasRecord, 443)
	MisBeginCondition(NoRecord, 444)
	MisBeginCondition(NoMission, 444)
	MisBeginCondition(HasPirateGuild)

	MisBeginAction(AddMission, 444)
	MisBeginAction(AddTrigger, 4441, TE_GETITEM, 2444, 1)

	MisNeed(MIS_NEED_ITEM, 2444, 1, 10, 1)
	MisCancelAction(ClearMission, 444)

	MisHelpTalk("<t>You have not finished collecting? Please hurry!")

	MisResultTalk("<t>Good, now I will have enough funds to build my own pirate crew.")

	MisResultCondition(NoRecord, 444)
	MisResultCondition(HasMission, 444)
	MisResultCondition(HasPirateGuild)
	MisResultCondition(HasItem,2444,1)

	MisResultAction(TakeItem, 2444, 1)
	MisResultAction(ClearMission, 444)
	MisResultAction(SetRecord, 444)
	MisResultAction(AddExp, 250000, 250000)
	MisResultAction(AddMoney, 150000, 150000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2444)
	TriggerAction(1, AddNextFlag, 444, 10, 1)
	RegCurTrigger(4441)

--Blacksmith >> Holy Ark
	
	DefineMission(487, "The Congregation", 445)

	MisBeginTalk("<t>I have heard of your courageous deed. But do you know why those Deathsoul army are so feriocious in their attack?<n><t>Let me tell you, it is because they have been collecting some sort of cursed coins. I do not know the actual reason but it can't be something good.<n><t>If you are interested, collect 40 Coins and 1 Bloodied Coin from them. I might be able to break the curse.")

	MisBeginCondition(HasRecord, 444)
	MisBeginCondition(NoRecord, 445)
	MisBeginCondition(NoMission, 445)
	MisBeginCondition(HasPirateGuild)

	MisBeginAction(AddMission, 445)
	MisBeginAction(AddTrigger, 4451, TE_GETITEM, 2427, 1)
	MisBeginAction(AddTrigger, 4452, TE_GETITEM, 2438, 40)
	

	MisNeed(MIS_NEED_ITEM, 2427, 1, 10, 1)
	MisNeed(MIS_NEED_ITEM, 2438, 40, 20, 40)
	

	MisCancelAction(ClearMission, 445)
	MisHelpTalk("<t>How is it? How many have you collected?")

	MisResultTalk("<t> Yes! Rumor has it that the chat goes this way: !^($......%*#oa2......1&s?~*#^%!...... (Blacksmith starts to chant in some weird language). Ok! Its done now!<n><t>These gold coins is no longer cursed. Those Deathsoul will take extra care during any attacks now.")

	MisResultCondition(NoRecord, 445)
	MisResultCondition(HasMission, 445)
	MisResultCondition(HasItem, 2438, 40)
	MisResultCondition(HasItem, 2427, 1)
	MisResultCondition(HasPirateGuild)

	MisResultAction(TakeItem, 2438, 40)
	MisResultAction(TakeItem, 2427, 1)
	MisResultAction(ClearMission ,445)
	MisResultAction(SetRecord, 445)
	MisResultAction(AddExp, 500000,500000)
	MisResultAction(AddMoney, 800000, 800000)

	InitTrigger()
	TriggerCondition(1, IsItem, 2427)
	TriggerAction(1, AddNextFlag, 445, 10, 1)
	RegCurTrigger(4451)

	InitTrigger()
	TriggerCondition(1, IsItem, 2438)
	TriggerAction(1, AddNextFlag, 445, 20, 40)
	RegCurTrigger(4452)

end
CenterMission001()
