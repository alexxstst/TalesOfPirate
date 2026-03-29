print("-- [Loading] NPC Script [02]")

function TeleportCity()
	Talk(1, "Jovial: Hi! I am the Teleporter! How may I help you?")
	Text(1, "Teleport: Shaitan City", Teleport, "Shaitan City")
	Text(1, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	Text(1, "Teleport: Thundoria Castle", Teleport, "Thundoria Castle")
	if Server.Sys.Maps['Dream Island'].Active == true then
		Text(1, "Teleport: Dream Island", Teleport, "Dream Island")
	end
	if Server.Teleport.Haven == true then
		Text(1, "Teleport: Haven", JumpPage, 2)
	end
	Text(1, "Record Spawn", JumpPage, 8)
	Text(1, "Nothing, just looking around.", CloseTalk)
	
	Talk(2, "Jovial: Hi! I am the Teleporter! How may I help you?")
	Text(2, "Teleport: Abandon Mine Haven", Teleport, "Abandon Mine Haven")
	Text(2, "Teleport: Rockery Haven", Teleport, "Rockery Haven")
	Text(2, "Teleport: Andes Forest Haven", Teleport, "Andes Forest Haven")
	Text(2, "Teleport: Valhalla Haven", Teleport, "Valhalla Haven")
	Text(2, "Teleport: Solace Haven", Teleport, "Solace Haven")
	Text(2, "Teleport: Chaldea Haven", Teleport, "Chaldea Haven")
	Text(2, "<- Back", JumpPage, 1)
	Text(2, "Nothing, just looking around.", CloseTalk)
	
	Talk(8, "Jovial: Are you sure you want to record here?")
	Text(8, "Record Spawn", SetSpawnPos, "Argent City")
	Text(8, "<- Back", JumpPage, 1)

	Talk(3, "Sarah: Hi! I am the Teleporter! How may I help you?")
	Text(3, "Teleport: Argent City", Teleport, "Argent City")
	Text(3, "Teleport: Shaitan City", Teleport, "Shaitan City")
	Text(3, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	if Server.Sys.Maps['Dream Island'].Active == true then
		Text(3, "Teleport: Dream Island", Teleport, "Dream Island")
	end
	Text(3, "Record Spawn", JumpPage, 9)
	Text(3, "Nothing, just looking around.", CloseTalk)

	Talk(9, "Sarah: Are you sure you want to record spawn here?")
	Text(9, "Record Spawn", SetSpawnPos, "Thundoria Castle")
	Text(9, "<- Back", JumpPage, 3)
	
	Talk(4, "May: Hi! I am the Teleporter! How may I help you?")
	Text(4, "Teleport: Argent City", Teleport, "Argent City")
	Text(4, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	Text(4, "Teleport: Thundoria Castle", Teleport, "Thundoria Castle")
	if Server.Sys.Maps['Dream Island'].Active == true then
		Text(4, "Teleport: Dream Island", Teleport, "Dream Island")
	end
	if Server.Teleport.Haven == true then
		Text(4, "Teleport: Haven", JumpPage, 5)
	end
	Text(4, "Record Spawn", JumpPage, 10)
	Text(4, "Nothing, just looking around.", CloseTalk)
	
	Talk(5, "May: Hi! I am the Teleporter! How may I help you?")
	Text(5, "Teleport: Oasis Haven", Teleport, "Oasis Haven")
	Text(5, "Teleport: Babul Haven", Teleport, "Babul Haven")
	Text(5, "<- Back", JumpPage, 4)
	Text(5, "Nothing, just looking around.", CloseTalk)
	
	Talk(10, "May: Are you sure you want to record spawn here?")
	Text(10, "Record Spawn", SetSpawnPos,"Shaitan City")
	Text(10, "<- Back", JumpPage, 4)
	
	Talk(6, "Helen: Hi! I am the Teleporter! How may I help you?")
	Text(6, "Teleport: Argent City", Teleport, "Argent City")
	Text(6, "Teleport: Shaitan City", Teleport, "Shaitan City")
	Text(6, "Teleport: Thundoria Castle", Teleport, "Thundoria Castle")
	if Server.Sys.Maps['Dream Island'].Active == true then
		Text(6, "Teleport: Dream Island", Teleport, "Dream Island")
	end
	if Server.Teleport.Haven == true then
		Text(6, "Teleport: Haven", JumpPage, 7)
	end
	Text(6, "Record Spawn", JumpPage, 11)
	Text(6, "Nothing, just looking around.", CloseTalk)
	
	Talk(7, "Helen: Hi! I am the Teleporter! How may I help you?")
	Text(7, "Teleport: Icicle Haven", Teleport, "Icicle Haven")
	Text(7, "Teleport: Atlantis Haven", Teleport, "Atlantis Haven")
	Text(7, "Teleport: Skeleton Haven", Teleport, "Skeleton Haven")
	Text(7, "Teleport: Icespire Haven", Teleport, "Icespire Haven")
	Text(7, "<- Back", JumpPage, 6)
	Text(7, "Nothing, just looking around.", CloseTalk)

	Talk(11, "May: Are you sure you want to record spawn here?")
	Text(11, "Record Spawn", SetSpawnPos, "Icicle Castle")
	Text(11, "<- Back", JumpPage, 6)
	
	InitTrigger()
	TriggerCondition(1, IsMapNpc, "garner", 0)
	TriggerAction(1, JumpPage, 1)
	TriggerCondition(2, IsMapNpc, "garner", 43)
	TriggerAction(2, JumpPage, 3)
	TriggerCondition(3, IsMapNpc, "magicsea", 0)
	TriggerAction(3, JumpPage, 4)
	TriggerCondition(4, IsMapNpc, "darkblue", 12)
	TriggerAction(4, JumpPage, 6)
	Start(GetMultiTrigger(), 4)
end
function TeleportHaven()
	Talk(1, "Meiya: Hi! I am the Teleporter! How may I help you?")
	Text(1, "Teleport: Rockery Haven", Teleport, "Rockery Haven")
	if Server.Teleport.City == true then
		Text(1, "Teleport: Argent City", Teleport, "Argent City")
	end
	Text(1, "Record Spawn", SetSpawnPos, "Abandon Mine Haven")
	Text(1, "Nothing, just looking around.", CloseTalk)

	Talk(2, "Felicia: Hi! I am the Teleporter! How may I help you?")
	Text(2, "Teleport: Abandon Mine Haven", Teleport, "Abandon Mine Haven")
	Text(2, "Teleport: Andes Forest Haven", Teleport, "Andes Forest Haven")
	Text(2, "Record Spawn", SetSpawnPos, "Rockery Haven")
	Text(2, "Nothing, just looking around.", CloseTalk)
	
	Talk(3, "Wendy: Hi! I am the Teleporter! How may I help you?")
	Text(3, "Teleport: Rockery Haven", Teleport, "Rockery Haven")
	Text(3, "Teleport: Valhalla Haven", Teleport, "Valhalla Haven")
	Text(3, "Record Spawn", SetSpawnPos, "Andes Forest Haven")
	Text(3, "Nothing, just looking around.", CloseTalk)
	
	Talk(4, "Meila: Hi! I am the Teleporter! How may I help you?")
	Text(4, "Teleport: Valhalla Haven", Teleport, "Valhalla Haven")
	Text(4, "Teleport: Chaldea Haven", Teleport, "Chaldea Haven")
	Text(4, "Record Spawn", SetSpawnPos, "Solace Haven")
	Text(4, "Nothing, just looking around.", CloseTalk)
	
	Talk(5, "Elizabeth: Hi! I am the Teleporter! How may I help you?")
	Text(5, "Teleport: Andes Forest Haven", Teleport, "Andes Forest Haven")
	Text(5, "Teleport: Solace Haven", Teleport, "Solace Haven")
	Text(5, "Record Spawn", SetSpawnPos, "Valhalla Haven")
	Text(5, "Nothing, just looking around.", CloseTalk)
	
	Talk(6, "Mabel: Hi! I am the Teleporter! How may I help you?")
	Text(6, "Teleport: Solace Haven", Teleport, "Solace Haven")
	if Server.Teleport.City == true then
		Text(6, "Teleport: Argent City", Teleport, "Argent City")
	end
	Text(6, "Record Spawn", SetSpawnPos, "Chaldea Haven")
	Text(6, "Nothing, just looking around.", CloseTalk)
	
	Talk(7, "Aina: Hi! I am the Teleporter! How may I help you?")
	Text(7, "Teleport: Babul Haven", Teleport, "Babul Haven")
	if Server.Teleport.City == true then
		Text(7, "Teleport: Shaitan City", Teleport, "Shaitan City")
	end
	Text(7, "Record Spawn", SetSpawnPos, "Oasis Haven")
	Text(7, "Nothing, just looking around.", CloseTalk)
	
	Talk(8, "Berlin: Hi! I am the Teleporter! How may I help you?")
	Text(8, "Teleport: Oasis Haven", Teleport, "Oasis Haven")
	if Server.Teleport.City == true then
		Text(8, "Teleport: Shaitan City", Teleport, "Shaitan City")
	end
	Text(8, "Record Spawn", SetSpawnPos, "Babul Haven")
	Text(8, "Nothing, just looking around.", CloseTalk)
	
	Talk(9, "Lily: Hi! I am the Teleporter! How may I help you?")
	Text(9, "Teleport: Atlantis Haven", Teleport, "Atlantis Haven")
	if Server.Teleport.City == true then
		Text(9, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	end
	Text(9, "Record Spawn", SetSpawnPos, "Icicle Haven")
	Text(9, "Nothing, just looking around.", CloseTalk)
	
	Talk(10, "Sofia: Hi! I am the Teleporter! How may I help you?")
	Text(10, "Teleport: Icicle Haven", Teleport, "Icicle Haven")
	Text(10, "Teleport: Skeleton Haven", Teleport, "Skeleton Haven")
	if Server.Teleport.City == true then
		Text(10, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	end
	Text(10, "Record Spawn", SetSpawnPos, "Atlantis Haven")
	Text(10, "Nothing, just looking around.", CloseTalk)
	
	Talk(11, "Mina: Hi! I am the Teleporter! How may I help you?")
	Text(11, "Teleport: Atlantis Haven", Teleport, "Atlantis Haven")
	Text(11, "Teleport: Icespire Haven", Teleport, "Icespire Haven")
	if Server.Teleport.City == true then
		Text(11, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	end
	Text(11, "Record Spawn", SetSpawnPos, "Skeleton Haven")
	Text(11, "Nothing, just looking around.", CloseTalk)
	
	Talk(12, "Artemis: Hi! I am the Teleporter! How may I help you?")
	Text(12, "Teleport: Skeleton Haven", Teleport, "Skeleton Haven")
	if Server.Teleport.City == true then
		Text(12, "Teleport: Icicle Castle", Teleport, "Icicle Castle")
	end
	Text(12, "Record Spawn", SetSpawnPos, "Icespire Haven")
	Text(12, "Nothing, just looking around.", CloseTalk)
	
	InitTrigger()
	TriggerCondition(1, IsMapNpc, "garner", 95)
	TriggerAction(1, JumpPage, 1)
	TriggerCondition(2, IsMapNpc, "garner", 93)
	TriggerAction(2, JumpPage, 2)
	TriggerCondition(3, IsMapNpc, "garner", 55)
	TriggerAction(3, JumpPage, 3)
	TriggerCondition(4, IsMapNpc, "garner", 96)
	TriggerAction(4, JumpPage, 4)
	TriggerCondition(5, IsMapNpc, "garner", 94)
	TriggerAction(5, JumpPage, 5)
	TriggerCondition(6, IsMapNpc, "garner", 54)
	TriggerAction(6, JumpPage, 6)
	TriggerCondition(7, IsMapNpc, "magicsea", 49)
	TriggerAction(7, JumpPage, 7)
	TriggerCondition(8, IsMapNpc, "magicsea", 45)
	TriggerAction(8, JumpPage, 8)
	TriggerCondition(9, IsMapNpc, "darkblue", 54)
	TriggerAction(9, JumpPage, 9)
	TriggerCondition(10, IsMapNpc, "darkblue", 40)
	TriggerAction(10, JumpPage, 10)
	TriggerCondition(11, IsMapNpc, "darkblue", 44)
	TriggerAction(11, JumpPage, 11)
	TriggerCondition(12, IsMapNpc, "darkblue", 49)
	TriggerAction(12, JumpPage, 12)
	Start(GetMultiTrigger(), 12)
end
function TeleportIsland()
	Talk(1, "Silvius: Hi, I am the Island Teleporter! How can I help you?")
	Text(1, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(1, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(1, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(1, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(1, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(1, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(1, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	if Server.Sys.Maps['PKmap'].Active == true then
		Text(1, "Teleport: PK Arena", Teleport, "PK Arena")
	end
	
	Talk(2, "Andrea: Hi, I am the Island Teleporter! How can I help you?")
	Text(2, "Teleport: Argent City", Teleport, "Argent City")
	Text(2, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(2, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(2, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(2, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(2, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(2, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(2, "Record Spawn point", SetSpawnPos, "Zephyr Isle")

	Talk(3, "Arsene: Hi, I am the Island Teleporter! How can I help you?")
	Text(3, "Teleport: Argent City", Teleport, "Argent City")
	Text(3, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(3, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(3, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(3, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(3, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(3, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(3, "Record Spawn point", SetSpawnPos, "Glacier Isle")
	
	Talk(4, "Shayala: Hi, I am the Island Teleporter! How can I help you?")
	Text(4, "Teleport: Argent City", Teleport, "Argent City")
	Text(4, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(4, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(4, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(4, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(4, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(4, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(4, "Record Spawn point", SetSpawnPos, "Outlaw Isle")
	
	Talk(5, "Julie: Hi, I am the Island Teleporter! How can I help you?")
	Text(5, "Teleport: Argent City", Teleport, "Argent City")
	Text(5, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(5, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(5, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(5, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(5, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(5, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(5, "Record Spawn point", SetSpawnPos, "Isle of Chill")
	
	Talk(6, "Winnie: Hi, I am the Island Teleporter! How can I help you?")
	Text(6, "Teleport: Argent City", Teleport, "Argent City")
	Text(6, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(6, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(6, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(6, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(6, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(6, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(6, "Record Spawn point", SetSpawnPos, "Canary Isle")
	
	Talk(7, "Wanda: Hi, I am the Island Teleporter! How can I help you?")
	Text(7, "Teleport: Argent City", Teleport, "Argent City")
	Text(7, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(7, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(7, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(7, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(7, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(7, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	Text(7, "Record Spawn point", SetSpawnPos, "Cupid Isle")

	Talk(8, "Juliet: Hi, I am the Island Teleporter! How can I help you?")
	Text(8, "Teleport: Argent City", Teleport, "Argent City")
	Text(8, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(8, "Teleport: Argent City", Teleport, "Argent City")
	Text(8, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(8, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(8, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(8, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(8, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(8, "Record Spawn point", SetSpawnPos, "Isle of Fortune")
	
	Talk(9, "Niecy: Hi, I am the Island Teleporter! How can I help you?")
	Text(9, "Teleport: Argent City", Teleport, "Argent City")
	Text(9, "Teleport: Zephyr Isle", Teleport, "Zephyr Isle")
	Text(9, "Teleport: Glacier Isle", Teleport, "Glacier Isle")
	Text(9, "Teleport: Outlaw Isle", Teleport, "Outlaw Isle")
	Text(9, "Teleport: Isle of Chill", Teleport, "Isle of Chill")
	Text(9, "Teleport: Canary Isle", Teleport, "Canary Isle")
	Text(9, "Teleport: Cupid Isle", Teleport, "Cupid Isle")
	Text(9, "Teleport: Isle of Fortune", Teleport, "Isle of Fortune")
	
	InitTrigger()
	TriggerCondition(1, IsMapNpc, "garner", 122)
	TriggerAction(1, JumpPage, 1)
	TriggerCondition(2, IsMapNpc, "garner", 123)
	TriggerAction(2, JumpPage, 2)
	TriggerCondition(3, IsMapNpc, "garner", 124)
	TriggerAction(3, JumpPage, 3)
	TriggerCondition(4, IsMapNpc, "garner", 125)
	TriggerAction(4, JumpPage, 4)
	TriggerCondition(5, IsMapNpc, "magicsea", 68)
	TriggerAction(5, JumpPage, 5)
	TriggerCondition(6, IsMapNpc, "magicsea", 69)
	TriggerAction(6, JumpPage, 6)
	TriggerCondition(7, IsMapNpc, "magicsea", 70)
	TriggerAction(7, JumpPage, 7)
	TriggerCondition(8, IsMapNpc, "darkblue", 63)
	TriggerAction(8, JumpPage, 8)
	TriggerCondition(9, IsMapNpc, "PKmap", 1)
	TriggerAction(9, JumpPage, 9)
    Start(GetMultiTrigger(), 9)
end
