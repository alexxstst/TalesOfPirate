print("-- [Loading] Mounts Category")

local MainTabName = 'Mounts'

IGS.Category[MainTabName] = IGS.Category[MainTabName] or {
	[MainTabName] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][MainTabName].Packs[1]	= AddMallPack("Cuddly Lamb Mount", "A companion to get along with your journey.", 29, 1, {9501}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[2]	= AddMallPack("Witch Broomstick Mount", "A companion to get along with your journey.", 29, 1, {9502}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[3]	= AddMallPack("Pink Ostrich Mount", "A companion to get along with your journey.", 29, 1, {9503}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[4]	= AddMallPack("Squirrel Mount", "A companion to get along with your journey.", 29, 1, {9515}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[5]	= AddMallPack("Tortoise Mount", "A companion to get along with your journey.", 29, 1, {9507}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[6]	= AddMallPack("Elephant King Mount", "A companion to get along with your journey.", 29, 1, {9513}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[7]	= AddMallPack("Baby Rockets Mount", "A companion to get along with your journey.", 39, 1, {9514}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[8]	= AddMallPack("Little Wolf Mount", "A companion to get along with your journey.", 39, 1, {9511}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[9]	= AddMallPack("Pink Deer Mount", "A companion to get along with your journey.", 39, 1, {9510}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[10] = AddMallPack("Rat King Mount", "A companion to get along with your journey.", 39, 1, {9509}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[11] = AddMallPack("T-Rex Mount", "A companion to get along with your journey.", 39, 1, {9508}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[12] = AddMallPack("Little Cat Mount", "A companion to get along with your journey.", 39, 1, {9505}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[13] = AddMallPack("Lycan Mount", "A companion to get along with your journey.", 39, 1, {9504}, 1, -1)
IGS.Category[MainTabName][MainTabName].Packs[14] = AddMallPack("Baby Black Dragon Mount", "A companion to get along with your journey.", 39, 1, {9500}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
