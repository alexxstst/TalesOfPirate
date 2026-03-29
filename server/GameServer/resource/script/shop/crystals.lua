print("-- [Loading] Crystals Category")

local MainTabName = 'Crystals'

IGS.Category[MainTabName] = {
	[MainTabName] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][MainTabName].Packs[1] = AddMallPack("10 Crystals", "Double click to get 10 In-Game Mall Points.", 10, 1, {19692}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[2] = AddMallPack("25 Crystals", "Double click to get 25 In-Game Mall Points.", 25, 1, {19693}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[3] = AddMallPack("40 Crystals", "Double click to get 40 In-Game Mall Points.", 40, 1, {19694}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[4] = AddMallPack("55 Crystals", "Double click to get 55 In-Game Mall Points.", 55, 1, {19695}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[5] = AddMallPack("100 Crystals", "Double click to get 100 In-Game Mall Points.", 100, 1, {19696}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
