print("-- [Loading] Equipment Category")

local MainTabName = 'Equipment'

IGS.Category[MainTabName] =  {
	[MainTabName] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][MainTabName].Packs[1] = AddMallPack("Honorary Crown of Kings", "", 40, 1, {{ID = 7001, Qty = 1}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[2] = AddMallPack("Arthurs King Faith", "", 45, 1, {{ID = 7002, Qty = 1}}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
