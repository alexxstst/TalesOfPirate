print("-- [Loading] Tickets Category")

local MainTabName = 'Tickets'
local SubTabName1 = 'Common'
local SubTabName2 = 'Special'

IGS.Category[MainTabName] = IGS.Category[MainTabName] or {
	[MainTabName] = {Packs = {}, Pointer = nil},
	[SubTabName1] = {Packs = {}, Pointer = nil},
	[SubTabName2] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][SubTabName1].Packs[1]	= AddMallPack("Pass to Lone Tower", "Used for teleportation purposes.", 10, 1, {{ID = 3054, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[2]	= AddMallPack("Pass to Abandon Mine 1", "Used for teleportation purposes.", 10, 1, {{ID = 3073, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[3]	= AddMallPack("Pass to Andes Forest Haven", "Used for teleportation purposes.", 10, 1, {{ID = 3051, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[4]	= AddMallPack("Pass to Thundoria Castle", "Used for teleportation purposes.", 10, 1, {{ID = 3048, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[5]	= AddMallPack("Pass to Thundoria Harbor", "Used for teleportation purposes.", 10, 1, {{ID = 3049, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[6]	= AddMallPack("Pass to Oasis Haven", "Used for teleportation purposes.", 10, 1, {{ID = 3052, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[7]	= AddMallPack("Pass to Icespire Haven", "Used for teleportation purposes.", 10, 1, {{ID = 3053, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[8]	= AddMallPack("Pass to Sacred Snow Mountain", "Used for teleportation purposes.", 8, 1, {{ID = 3050, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[9]	= AddMallPack("Pass to Spring Town", "Used for teleportation purposes.", 10, 1, {{ID = 0332, Qty = 12}}, 1, -1)

IGS.Category[MainTabName][SubTabName2].Packs[1] = AddMallPack("Pass to Autumn Island", "Used for teleportation purposes.", 5, 1, {{ID = 0583, Qty = 5}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[2] = AddMallPack("Pass to Summer Island", "Used for teleportation purposes.", 5, 1, {{ID = 0563, Qty = 5}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[3] = AddMallPack("Pass to Abandon 4", "Used for teleportation purposes.", 5, 1, {{ID = 2844, Qty = 5}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[4] = AddMallPack("Caribbean Tour Ticket", "Used for teleportation purposes.", 5, 1, {{ID = 2445, Qty = 5}}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
IGS.Category[MainTabName][SubTabName1].Pointer = AddMallTab(SubTabName1, IGS.Category[MainTabName][SubTabName1].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName2].Pointer = AddMallTab(SubTabName2, IGS.Category[MainTabName][SubTabName2].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
