print("-- [Loading] Eudemon Script")

function Eudemon()
    print("--------------------------------------------------")
    print("[End] ** Loading LUA Files **")
    print("--------------------------------------------------")
    print("--------------------------------------------------")
    print("[Start] ** Loading Map Files **")
    print("--------------------------------------------------")
end

function EudemonMission()
    InitTrigger()

    RegTrigger(8889, 1)

    DefineWorldMission(1, "Welcome", 1)

    MisBeginTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")
    MisHelpTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")

    MisBeginAction(AddMission, 1)
    MisBeginAction(SetFlag, 1, 1)

    MisNeed(MIS_NEED_DESP, "Look for <rNewbie Guide - Senna> at (2223, 2785) to understand more about <rClasses> and <rAttributes>")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(2, "Welcome", 2)

    MisBeginTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")
    MisHelpTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")

    MisBeginAction(AddMission, 2)
    MisBeginAction(SetFlag, 2, 1)

    MisNeed(MIS_NEED_DESP, "Find <rNewbie Guide - Resline> at (877, 3573) to learn about <rClass> and <rAttributes>")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(3, "Welcome", 3)

    MisBeginTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")
    MisHelpTalk("<t>Greetings! Welcome to Pirate King Online.<n><t>A world filled with mystical creatures, treasures and of course, danger and adventure. Feel free to roam about the town area.<n><t>Throughout the course of your travel, you may also wish to enlist into the Navy Militia or join the Pirates and hunt for fame and treasure!<n><t>Your first task of the game is to locate Newbie Guide Senna. She has been stationed in Argent City at (2223, 2785) to help out people such as yourself to get familiarized with their surroundings.<n><t>Enjoy your stay and all the best on your journey!")

    MisBeginAction(AddMission, 3)
    MisBeginAction(SetFlag, 3, 1)

    MisNeed(MIS_NEED_DESP, "Look for <rNewbie Guide - Angela> at (1315, 507) to understand more about <rClasses> and <rAttributes>")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(4, "Lure Behemoth", 4)

    MisBeginTalk("<t>Proceed to <pAbandoned Mine 2> and find the lair of the Behemoth. When you reached the lair, use the <ySpecial Strawberry Biscuit> and see what happens.<n><t>Once you manage to lure the beast out, please kill him within the hour.")
    MisHelpTalk("<t>Proceed to <pAbandoned Mine 2> and find the lair of the Behemoth. When you reached the lair, use the <ySpecial Strawberry Biscuit> and see what happens.<n><t>Once you manage to lure the beast out, please kill him within the hour.")
    MisBeginAction(AddMission, 4)
    MisBeginAction(SystemNotice, "Obtained Quest: 'Lure Behemoth'")

    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Use the Strawberry biscuit to lure the Behemoth")
    MisNeed(MIS_NEED_KILL, 99, 1, 10, 1)

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(5, "For Whom It May Concern", 5)

    MisBeginTalk("<t>Discovered that the letter addressee is actually <bSupermun>!<n><t>Report this to <bMerrix>.")
    MisHelpTalk("<t>Discovered that the letter addressee is actually <bSupermun>!<n><t>Report this to <bMerrix>.")
    MisBeginAction(AddMission, 5)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Look for Merrix at (1118, 3611)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(6, "Wrong Question", 6)

    MisBeginTalk("<t>Looks like Chairman Ronnie does not want to disclose more. Perhaps talking to <bBlurry> would be better.")
    MisHelpTalk("<t>Looks like Chairman Ronnie does not want to disclose more. Perhaps talking to <bBlurry> would be better.")
    MisBeginAction(AddMission, 6)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Report back to Blurry at (2272, 2700)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(7, "Return to Icicle", 7)

    MisBeginTalk("<t>Now is time to return to Icicle Castle and have a chat with Lee about the location of 'Nine Haven'.")
    MisHelpTalk("<t>Now is time to return to Icicle Castle and have a chat with Lee about the location of 'Nine Haven'.")
    MisBeginAction(AddMission, 7)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Return to Luke in Icicle City at (1320, 585).")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(8, "Blood of Pirate", 8)

    MisBeginTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, ' Blood of a Pirate, proved your loyalty and I will show you the way'.<n>What is the Blood of a Pirate, I pondered. How to prove my loyalty? Maybe <bJack> at (1672, 3777) will know something.")
    MisHelpTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, ' Blood of a Pirate, proved your loyalty and I will show you the way'.<n>What is the Blood of a Pirate, I pondered. How to prove my loyalty? Maybe <bJack> at (1672, 3777) will know something.")
    MisBeginAction(AddMission, 8)
    MisBeginAction(TakeItem, 4220, 1)
    MisBeginAction(GiveItem, 4221, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Find Jack for the answer to the question in your heart")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(9, "Soul of Navy", 9)

    MisBeginTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, 'Soul of Navy, proved your determination and I will show you the way'.<n><t>What is the Soul of Navy, I pondered. How to prove my determination? Maybe <bJack> at (1672, 3777) will know something.")
    MisHelpTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, 'Soul of Navy, proved your determination and I will show you the way'.<n><t>What is the Soul of Navy, I pondered. How to prove my determination? Maybe <bJack> at (1672, 3777) will know something.")
    MisBeginAction(AddMission, 9)
    MisBeginAction(TakeItem, 4220, 1)
    MisBeginAction(GiveItem, 4221, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Find Jack for the answer to the question in your heart")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(10, "Nameless One", 10)

    MisBeginTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, 'Nameless One, proved your bravery and I will show you the way'.<n><t>Who is the Nameless One, I pondered. How to prove my bravery? Maybe <bJack> at (1672, 3777) will know something.")
    MisHelpTalk("<t>When I used my both hands to touch the black object, a mysterious voice echoed into me, 'Nameless One, proved your bravery and I will show you the way'.<n><t>Who is the Nameless One, I pondered. How to prove my bravery? Maybe <bJack> at (1672, 3777) will know something.")
    MisBeginAction(AddMission, 10)
    MisBeginAction(TakeItem, 4220, 1)
    MisBeginAction(GiveItem, 4221, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Look for Jack at (1672,3777)t to answer your question")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(11, "Blood of Pirate", 11)

    MisBeginTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his henchmen? HmmI better run fast. Time to meet up with <bIcicle Royal - Mas>.")
    MisHelpTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his henchmen? HmmI better run fast. Time to meet up with <bIcicle Royal - Mas>.")
    MisBeginAction(AddMission, 11)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Talk to <bIcicle Royal - Mas> at (1346, 451) regarding the Blood of Pirate")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(12, "Soul of Navy", 12)

    MisBeginTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his henchmen? HmmI better run fast. Time to meet up with <bGeneral - William>.")
    MisHelpTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his henchmen? HmmI better run fast. Time to meet up with <bGeneral - William>.")
    MisBeginAction(AddMission, 12)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Go to (2277, 2831) and ask <bGeneral - William> about Soul of Navy")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(13, "Nameless One", 13)

    MisBeginTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his underlings? HmmI better run fast. Time to meet up with <bArgent Secretary - Salvier>.")
    MisHelpTalk("<t>I feel that Jack knows the solution but he would not tell me. Is it because I killed too many of his underlings? HmmI better run fast. Time to meet up with <bArgent Secretary - Salvier>.")
    MisBeginAction(AddMission, 13)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Ask <bArgent Secretary - Salvier> at (2219, 2749) about the Nameless One")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(14, "Miracle Light", 14)

    MisBeginTalk("<t>When I reached this part of the ocean, me and my ship were worn out. If I cannot find anything worthy, I guess I will not have the courage to go back. At this moment, the Heart of Sailor gives out a bright glow. I feel rejuvenated and my ship is restored fully. Surprise by this turn of event, I feel a urge to tell <bArgent Secretary - Salvier> about this matter. He should be able to explain to me what has happened.")
    MisHelpTalk("<t>When I reached this part of the ocean, me and my ship were worn out. If I cannot find anything worthy, I guess I will not have the courage to go back. At this moment, the Heart of Sailor gives out a bright glow. I feel rejuvenated and my ship is restored fully. Surprise by this turn of event, I feel a urge to tell <bArgent Secretary - Salvier> about this matter. He should be able to explain to me what has happened.")
    MisBeginAction(AddMission, 14)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Relate your misfortune to <bArgent Secretary - Salvier> at (2219, 2749)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(15, "Thundoria Bank", 15)

    MisBeginTalk("<t>I examined the keys in my hand closely. It looks exquisite and beautiful. Although it is worn out, it still bare the logo of Thundoria bank. Looks like I have to head towards Thundoria")
    MisHelpTalk("<t>I examined the keys in my hand closely. It looks exquisite and beautiful. Although it is worn out, it still bare the logo of Thundoria bank. Looks like I have to head towards Thundoria")
    MisBeginAction(AddMission, 15)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Bring the <rAncient Key> and look for<bBanker - Macurdo> in Thundoria Bank at (743, 1534)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(16, "Will of Andrew", 16)

    MisBeginTalk("<t>' I am a little bird that longed for the freedom of the skies' Please do not look at me this way, I will get shy.<n><t> What is this? Am I dreaming? Is this a genuine copy of Andrew's Will? I need <bJack's> verification to prove this.")
    MisHelpTalk("<t>' I am a little bird that longed for the freedom of the skies' Please do not look at me this way, I will get shy.<n><t> What is this? Am I dreaming? Is this a genuine copy of Andrew's Will? I need <bJack's> verification to prove this.")
    MisBeginAction(AddMission, 16)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Look for <bJack> at (1672, 3777) to verify the Will")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(17, "Love Entanglement", 17)

    MisBeginTalk("<t>Seems like the vengeance between Little Daniel and Captain Jack will have to depend on this pumpkin headHope that it will bring back some sweet memories for Little Daniel.")
    MisHelpTalk("<t>Seems like the vengeance between Little Daniel and Captain Jack will have to depend on this pumpkin headHope that it will bring back some sweet memories for Little Daniel.")
    MisBeginAction(AddMission, 17)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Bring the <rPumpkin with Craved Wording> to <bLittle Daniel> at (2193, 2730)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(18, "Language of the Pirates", 18)

    MisBeginTalk("<t>When I gentled sprinkled the Invisible Ink Negator upon Andrew's will, something astonishing happened. The words on it gradually faded away to reveal sentences distinguishingly written in a mysterious pirate language, 'The sound of victory leads you to the path of death. Towards the horizon of the skeletons you must go. When blood flows once again through withered bones, find you shall, the legendary treasure.' <n><t>What does this mean? I suppose Jack would know better.")
    MisHelpTalk("<t>When I gentled sprinkled the Invisible Ink Negator upon Andrew's will, something astonishing happened. The words on it gradually faded away to reveal sentences distinguishingly written in a mysterious pirate language, 'The sound of victory leads you to the path of death. Towards the horizon of the skeletons you must go. When blood flows once again through withered bones, find you shall, the legendary treasure.' <n><t>What does this mean? I suppose Jack would know better.")
    MisBeginAction(AddMission, 18)
    MisBeginAction(TakeItem, 4228, 1)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Look for <bCaptain Jack> at (1672, 3777) regarding the scribbling")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(19, "Mermaid's Treasure", 19)

    MisBeginTalk("<t>(This must be it! The <rMermaid's Carcass>! However there doesn't seem to be anything about treasure here. I guess Jack must have made a mistake. Wait...What's this? Seems like some kind of handwriting, <p'(1843, 1717)'>. It must be the location of the treasure!<n><t>But which sea!? I guess I'll have to go by trial and error. According to Jack, if I drip my own fresh blood onto the <rMermaid's Carcass> will reveal the correct position. Sounds like a good idea but it's going to hurt!)")
    MisHelpTalk("<t>(This must be it! The <rMermaid's Carcass>! However there doesn't seem to be anything about treasure here. I guess Jack must have made a mistake. Wait...What's this? Seems like some kind of handwriting, <p'(1843, 1717)'>. It must be the location of the treasure!<n><t>But which sea!? I guess I'll have to go by trial and error. According to Jack, if I drip my own fresh blood onto the <rMermaid's Carcass> will reveal the correct position. Sounds like a good idea but it's going to hurt!)")
    MisBeginAction(AddMission, 19)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Go to (1843, 1717) and use <rMermaid Carcass> to see if anything happens.")

    MisResultCondition(AlwaysFailure)
    MisResultAction(AddExp, 100000, 100000)
    MisResultAction(AddMoney, 12000, 12000)
    MisResultAction(AddExpAndType, 2, 30000, 30000)
    MisResultAction(GiveItem, 3462, 20, 4)

    DefineWorldMission(20, "Mystic Diary", 20)

    MisBeginTalk("<t>When my blood spilled on the mermaid's carcass, I felt had such a terrible pain that I fainted.<n><t>When I awoke, the carcass was gone, on my hands instead was an old diary. If I did not see wrong, the name Andrew was written on top using demonic language.<n><t>Rubbing my eyes, I carefully flip open the diary and as expected, inside contains the journals of Andrew's seafaring days - all written in demonic language. However, at some pages around the middle, in another language I have never seen before, records Andrew's experiences in a mysterious small town.<n><t>Let me recall, if I remembered correctly, <bOldman - Blurry> at (2272, 2700) told me that if I were to encounter problems regarding weird languages, he can be approached at midnight. As to why only in the midst of the night, it doesnt really matter that much now, so go look for him.")
    MisHelpTalk("<t>When my blood spilled on the mermaid's carcass, I felt had such a terrible pain that I fainted.<n><t>When I awoke, the carcass was gone, on my hands instead was an old diary. If I did not see wrong, the name Andrew was written on top using demonic language.<n><t>Rubbing my eyes, I carefully flip open the diary and as expected, inside contains the journals of Andrew's seafaring days - all written in demonic language. However, at some pages around the middle, in another language I have never seen before, records Andrew's experiences in a mysterious small town.<n><t>Let me recall, if I remembered correctly, <bOldman - Blurry> at (2272, 2700) told me that if I were to encounter problems regarding weird languages, he can be approached at midnight. As to why only in the midst of the night, it doesnt really matter that much now, so go look for him.")
    MisBeginAction(AddMission, 20)
    MisBeginAction(SetRecord, 19)
    MisBeginAction(ClearMission, 19)
    MisBeginAction(TakeItem, 4230, 1)
    MisBeginAction(GiveItem, 4231, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Bring the diary to (2272, 2700) and look for <b Blurry>")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(21, "Mystery Town", 21)

    MisBeginTalk("<t>'I have been in this mystical little town for one week already and I am still unable to confirm if I am dreaming. This town filled my eyes with mystical happenings that I was unable to think, its forever spring in this uniquely mystical little town, you can hardly sense any worries here, with laughter filling every corner of the town. <n><t>I wish I could stay in this town forever, enjoying the life here. However, it seems like the detestable Navy does not think so, when dawn approaches, a warship appears on the horizon. I should leave now, otherwise this little town will suffer the same as my hometown<n><t>If one should see this diary I hope they could share this memory with me and remember this town as 'Spring Town' '<n><t>(Hmmthis seems to be a jokeI better look for Holy Priestess to confirm this)")
    MisHelpTalk("<t>'I have been in this mystical little town for one week already and I am still unable to confirm if I am dreaming. This town filled my eyes with mystical happenings that I was unable to think, its forever spring in this uniquely mystical little town, you can hardly sense any worries here, with laughter filling every corner of the town. <n><t>I wish I could stay in this town forever, enjoying the life here. However, it seems like the detestable Navy does not think so, when dawn approaches, a warship appears on the horizon. I should leave now, otherwise this little town will suffer the same as my hometown<n><t>If one should see this diary I hope they could share this memory with me and remember this town as 'Spring Town' '<n><t>(Hmmthis seems to be a jokeI better look for Holy Priestess to confirm this)")
    MisBeginAction(AddMission, 21)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Find <bHoly Priestess - Ada> (862, 3303) for Spring town coordinate problem.")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(22, "Wheel of Fate", 22)

    MisBeginTalk("<t>I have been traveling on these dark waters for 7 days, without the Water Wheel's navigation, I don't think I would have been able to make it out of this darkness. Whenever I can't see anything, I'll decide to find a nice cozy corner to take some rest, but before that out of habit, I usually take a look at the water wheel again. <n><t>Suddenly, through the crashing sound of the waves, I heard a faint whisper calling for help. Being a sailor, naturally it is hard for me to hear it and not act. I immediately changed the direction of ship towards the whisper. <n><t>Is it a trap? Or it is an unexpected incident? Whatever it is...Wait for me! I'll be right there!")
    MisHelpTalk("<t>I have been traveling on these dark waters for 7 days, without the Water Wheel's navigation, I don't think I would have been able to make it out of this darkness. Whenever I can't see anything, I'll decide to find a nice cozy corner to take some rest, but before that out of habit, I usually take a look at the water wheel again. <n><t>Suddenly, through the crashing sound of the waves, I heard a faint whisper calling for help. Being a sailor, naturally it is hard for me to hear it and not act. I immediately changed the direction of ship towards the whisper. <n><t>Is it a trap? Or it is an unexpected incident? Whatever it is...Wait for me! I'll be right there!")
    MisBeginAction(AddMission, 22)
    MisBeginAction(TakeItem, 4237, 1)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Go to <pAscaron (1497, 1707)> and investigate if there is anything there")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(24, "Tear of Dragon", 24)

    MisBeginTalk("<t>If I'm not wrong, I saw the Icy Dragon shed a tear. The tear became extremely cold and froze rapidly. As a result of the crystallization, it became a gorgeous gem. What could be the secret to make an Icy Dragon shed its tears?<n><t>Seems like there are many strange thing in this town.")
    MisHelpTalk("<t>If I'm not wrong, I saw the Icy Dragon shed a tear. The tear became extremely cold and froze rapidly. As a result of the crystallization, it became a gorgeous gem. What could be the secret to make an Icy Dragon shed its tears?<n><t>Seems like there are many strange thing in this town.")
    MisBeginAction(AddMission, 24)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Bring the Tear of Dragon to the village and ask about")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(25, "Secret of Dragon's Tear", 25)

    MisBeginTalk("<t>Was the legend really just a hoax? Or did Haysh use the wrong method? Was it a curse? We'll never know unless someone goes down there and asks Haysh himself. <n><t>Suddenly, the Tear of Dragon shimmers and a weak ray of light suddenly seemed to point towards the direction of Shaitan City. <n><t>Could it be a clue? Maybe a friend of mine, <bHoly Priestess - Ada> might be able to shed some light on this mystery.")
    MisHelpTalk("<t>Was the legend really just a hoax? Or did Haysh use the wrong method? Was it a curse? We'll never know unless someone goes down there and asks Haysh himself. <n><t>Suddenly, the Tear of Dragon shimmers and a weak ray of light suddenly seemed to point towards the direction of Shaitan City. <n><t>Could it be a clue? Maybe a friend of mine, <bHoly Priestess - Ada> might be able to shed some light on this mystery.")
    MisBeginAction(AddMission, 25)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Look for <bHoly Priestess - Ada> and ask about the Tear of Dragon")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(26, "Hassli's Deposit", 26)

    MisBeginTalk("<t> Dear friend, thank you for freeing me. All this while I was imprisoned in the body of an Icy Dragon with no control over what I was doing. Helpless, I saw through my own eyes how I mercilessly massacred all those poor people who came across my path. Was it really wrong to even try to make use of the Tear of Dragon to revive my dead sister? I don't know if god really exists in this world, but what I do know is that this world holds many mysteries and secrets, such as the Tear of Dragon. <n><t> Alas, my time here is short. I do not have much but I hope that the money I have in Argent City bank will be of good use to you. Lastly, I leave you with this last bit of advice. All that you see is not what it seems. Your own eyes are often the cause of deception.")
    MisHelpTalk("<t> Dear friend, thank you for freeing me. All this while I was imprisoned in the body of an Icy Dragon with no control over what I was doing. Helpless, I saw through my own eyes how I mercilessly massacred all those poor people who came across my path. Was it really wrong to even try to make use of the Tear of Dragon to revive my dead sister? I don't know if god really exists in this world, but what I do know is that this world holds many mysteries and secrets, such as the Tear of Dragon. <n><t> Alas, my time here is short. I do not have much but I hope that the money I have in Argent City bank will be of good use to you. Lastly, I leave you with this last bit of advice. All that you see is not what it seems. Your own eyes are often the cause of deception.")
    MisBeginAction(AddMission, 26)
    MisBeginAction(TakeItem, 4240, 1)
    MisBeginAction(SetRecord, 370)
    MisBeginAction(ClearMission, 370)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Go to <pThundoria Bank> and withdraw the deposit")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(27, "Love Letter of Zhou", 27)

    MisBeginTalk("<t>Although it is illegal to open letters belonging to others, I was still very curious about what this dreadful man has written. Hence, I opened the letter carefully ensuring that it could be sealed to its original look. Immediately, I regretted after reading the first sentence, it's a love letter, and a well-written love letter.<n><t> I quickly and cautiously sealed the letter up, my heart was pumping heavily as if I am running 1000 meters. I wonder if Wang Mo would notice it")
    MisHelpTalk("<t>Although it is illegal to open letters belonging to others, I was still very curious about what this dreadful man has written. Hence, I opened the letter carefully ensuring that it could be sealed to its original look. Immediately, I regretted after reading the first sentence, it's a love letter, and a well-written love letter.<n><t> I quickly and cautiously sealed the letter up, my heart was pumping heavily as if I am running 1000 meters. I wonder if Wang Mo would notice it")
    MisBeginAction(AddMission, 27)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Send the <rPink Letter> to <b Wang Mo> at (3290, 2512)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(28, "Sea Exploration", 28)

    MisBeginTalk("<t>If I'm not mistaken, this must be Yuri's boat. It's no wonder that it's still out afloat at sea. If I drank as much as them, I would never ever be sober again. Looks like Long Er need not worry. I'll be returning back to submit my report.")
    MisHelpTalk("<t>If I'm not mistaken, this must be Yuri's boat. It's no wonder that it's still out afloat at sea. If I drank as much as them, I would never ever be sober again. Looks like Long Er need not worry. I'll be returning back to submit my report.")
    MisBeginAction(AddMission, 28)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Report back to <bTrader - Yuri> at (3195, 2506)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(29, "Tragedy", 29)

    MisBeginTalk("<t>During my exploration, I came across a <rLetter in a Bottle>. It is made out of animal hide. The weird scribblings on it make out to be that of someone's will. Why don't you take this to Holy Priestess, Ada, and see if she can decipher it?")
    MisHelpTalk("Bring this <rLetter in a Bottle> to <bHoly Priestess - Ada>")
    MisBeginAction(AddMission, 29)
    MisBeginAction(SetRecord, 381)
    MisBeginAction(ClearMission, 381)
    MisBeginAction(TakeItem, 4253, 1, 4)
    MisBeginAction(GiveItem, 4233, 1, 4)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")
    MisNeed(MIS_NEED_DESP, "Bring the <rLetter in a Bottle> to <bHigh Priestess - Ada> at (862, 3303)")

    MisResultCondition(AlwaysFailure)

    DefineWorldMission(30, "Language Barrier", 30)

    MisBeginTalk("<t>Hello, this is Omni-Relevation Prescription speaking. Don't ask me why I can speak.<n><t>Don't burn me! You win, I shall tell you how to make the Potion of Omni-Relevation. You will need: a Fascia Fish Bone, a Thick Fish Bone, 30 Rotten Fish Bones and 10 Large Shiny Sea Jelly Skins. That's all, once you have gathered all the materials, bring me to <bMaster Kerra> and he'll help complete the potion.")
    MisHelpTalk("<t>Hello, this is Omni-Relevation Prescription speaking. Don't ask me why I can speak.<n><t>Don't burn me! You win, I shall tell you how to make the Potion of Omni-Relevation. You will need: a Fascia Fish Bone, a Thick Fish Bone, 30 Rotten Fish Bones and 10 Large Shiny Sea Jelly Skins. That's all, once you have gathered all the materials, bring me to <bMaster Kerra> and he'll help complete the potion.")
    MisBeginAction(AddMission, 30)
    MisBeginAction(TakeItem, 4256, 1)
    MisBeginAction(AddTrigger, 301, TE_GETITEM, 4938, 30)
    MisBeginAction(AddTrigger, 302, TE_GETITEM, 4957, 30)
    MisBeginAction(AddTrigger, 303, TE_GETITEM, 4976, 30)
    MisBeginAction(AddTrigger, 304, TE_GETITEM, 4974, 10)
    MisCancelAction(SystemNotice, "This quest cannot be abandoned")

    MisNeed(MIS_NEED_DESP, "Follow the intruction of the Prescription and bring the ingredient to <bMaster Kerra> at (2664, 654)")
    MisNeed(MIS_NEED_ITEM, 4938, 30, 1, 30)
    MisNeed(MIS_NEED_ITEM, 4957, 30, 31, 30)
    MisNeed(MIS_NEED_ITEM, 4976, 30, 61, 30)
    MisNeed(MIS_NEED_ITEM, 4974, 10, 91, 10)

    MisResultCondition(AlwaysFailure)

    InitTrigger()
    TriggerCondition(1, IsItem, 4938)
    TriggerAction(1, AddNextFlag, 30, 1, 30)
    RegCurTrigger(301)
    InitTrigger()
    TriggerCondition(1, IsItem, 4957)
    TriggerAction(1, AddNextFlag, 30, 31, 30)
    RegCurTrigger(302)
    InitTrigger()
    TriggerCondition(1, IsItem, 4976)
    TriggerAction(1, AddNextFlag, 30, 61, 30)
    RegCurTrigger(303)
    InitTrigger()
    TriggerCondition(1, IsItem, 4974)
    TriggerAction(1, AddNextFlag, 30, 91, 10)
    RegCurTrigger(304)
end
EudemonMission()
