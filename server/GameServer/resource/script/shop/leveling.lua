print("-- [Loading] Leveling Category")

local MainTabName = 'Leveling'

IGS.Category[MainTabName] =  {
	[MainTabName] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][MainTabName].Packs[1] = AddMallPack("Heaven's Berry", "Grants an extra 2.00X exp rate for a short period of time.", 3, 1, {{ID = 3844, Qty = 3}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[2] = AddMallPack("Charmed Berry", "Grants an extra 2.00X drop rate for a short period of time.", 3, 1, {{ID = 3845, Qty = 3}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[3] = AddMallPack("Party EXP Fruit", "Grants an extra 2.00X experience for party members for a period of time, must be used by leader.", 4, 1, {{ID = 0849, Qty = 4}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[4] = AddMallPack("Fruit of Growth", "Grants an extra 2.00X increased fairy growth for a short period of time.", 4, 1, {{ID = 0578, Qty = 4}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[5] = AddMallPack("Amplifier of Strive", "Grants an extra 2X exp rate for a short period of time.", 5, 1, {{ID = 3094, Qty = 3}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[6] = AddMallPack("Amplifier of Luck", "Grants an extra 2X drop rate for a short period of time.", 5, 1, {{ID = 3096, Qty = 3}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[7] = AddMallPack("Hi-Amplifier of Strive", "Grants an extra 2X drop rate for a short period of time.", 8, 1, {{ID = 3095, Qty = 3}}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[8] = AddMallPack("Hi-Amplifier of Luck", "Grants an extra 2X drop rate for a short period of time.", 8, 1, {{ID = 3097, Qty = 3}}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
