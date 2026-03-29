print("-- [Loading] Forging Category")

local MainTabName = 'Forging'

IGS.Category[MainTabName] = {
	[MainTabName] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][MainTabName].Packs[1] = AddMallPack("Equipment Stabilizer", "Stabilize equipment during forging.", 6, 1, {0890}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[2] = AddMallPack("Equipment Catalyst", "Catalyst for equipment forging.", 6, 1, {0891}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[3] = AddMallPack("Strengthening Scroll", "Recipe for Strengthening process.", 4, 1, {0455}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[4] = AddMallPack("Strengthening Crystal", "Catalyst for Strengthening process.", 4, 1, {0456}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[5] = AddMallPack("Blue Forging Fruit", "Increases forging success rate for 1 minute", 6, 1, {3883}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[6] = AddMallPack("Red Combining Fruit", "Increase composition success rate for 1 minute", 6, 1, {3884}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[7] = AddMallPack("1st Slot Plier", "Allow gem to be extracted from weapons or equipments", 6, 1, {1020}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[8] = AddMallPack("2nd Slot Plier", "Allow gem to be extracted from weapons or equipments.", 8, 1, {6870}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[9] = AddMallPack("3rd Slot Plier", "Allow gem to be extracted from weapons or equipments.", 10, 1, {6871}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
