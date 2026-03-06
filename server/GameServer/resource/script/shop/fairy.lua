print("-- [Loading] Fairy Category")

local MainTabName = 'Fairy'
local SubTabName1 = 'Eggs'
local SubTabName2 = 'Rations'
local SubTabName3 = 'Fruits'
local SubTabName4 = 'Marriage'
local SubTabName5 = 'Skills'

IGS.Category[MainTabName] = {
	[MainTabName] = {Packs = {}, Pointer = nil},
	[SubTabName1] = {Packs = {}, Pointer = nil},
	[SubTabName2] = {Packs = {}, Pointer = nil},
	[SubTabName3] = {Packs = {}, Pointer = nil},
	[SubTabName4] = {Packs = {}, Pointer = nil},
	[SubTabName5] = {Packs = {}, Pointer = nil}
}

-- AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
-- Fairy Eggs
IGS.Category[MainTabName][SubTabName1].Packs[1] = AddMallPack("Egg of Life", "Double click to obtain Fairy of Life.", 6, 1, {0422}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[2] = AddMallPack("Egg of Darkness", "Double click to obtain Fairy of Darkness.", 6, 1, {0423}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[3] = AddMallPack("Egg of Virtue", "Double click to obtain Fairy of Virtue.", 6, 1, {0424}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[4] = AddMallPack("Egg of Kudos", "Double click to obtain Fairy of Kudos.", 6, 1, {0425}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[5] = AddMallPack("Egg of Faith", "Double click to obtain Fairy of Faith.", 6, 1, {0426}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[6] = AddMallPack("Egg of Valor", "Double click to obtain Fairy of Valor.", 6, 1, {0427}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[7] = AddMallPack("Egg of Hope", "Double click to obtain Fairy of Hope.", 6, 1, {0428}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[8] = AddMallPack("Egg of Woe", "Double click to obtain Fairy of Woe.", 6, 1, {0429}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[9] = AddMallPack("Egg of Love", "Double click to obtain Fairy of Love.", 6, 1, {0430}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[10] = AddMallPack("Egg of Heart", "Double click to obtain Fairy of Heart.", 6, 1, {0431}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[11] = AddMallPack("Egg of Mordo", "Double click to obtain Mordo.", 10, 1, {0679}, 1, -1)
--	Fairy Rations
IGS.Category[MainTabName][SubTabName2].Packs[1] = AddMallPack("Fairy Ration", "Recovers the stamina of a pet.", 8, 1, {{ID = 0227, Qty = 99}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[2] = AddMallPack("Auto Ration", "Automatically recovers the stamina of a pet.", 10, 1, {{ID = 2312, Qty = 99}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[3] = AddMallPack("Great Fairy Ration", "Recovers the stamina of a pet.", 16, 1, {{ID = 6841, Qty = 99}}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[4] = AddMallPack("Great Auto Ration", "Automatically recovers the stamina of a pet.", 18, 1, {{ID = 6842, Qty = 99}}, 1, -1)
--	Normal/Great Fairy Fruits.
IGS.Category[MainTabName][SubTabName3].Packs[1] = AddMallPack("Snow Dragon Fruit", "Snow Dragon Fruit can increases the Strength of pet by 1.", 4, 1, {0222}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[2] = AddMallPack("Icespire Plum", "Icespire Plum can increases the Agility of pet by 1.", 4, 1, {0223}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[3] = AddMallPack("Zephyr Fish Floss", "Zephyr Fish Floss can increases the Accuracy of pet by 1.", 4, 1, {0224}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[4] = AddMallPack("Argent Mango", "Argent Mango can increases the Constitution of pet by 1.", 4, 1, {0225}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[5] = AddMallPack("Shaitan Biscuit", "Shaitan Biscuit can increases the Spirit of pet by 1.", 4, 1, {0226}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[6] = AddMallPack("Great Snow Dragon Fruit", "Great Snow Dragon Fruit can increases the Strength of pet by 2.", 7, 1, {0276}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[7] = AddMallPack("Great Icespire Plum", "Icespire Plum can increases the Agility of pet by 2.", 7, 1, {0277}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[8] = AddMallPack("Great Zephyr Fish Floss", "Zephyr Fish Floss can increases the Accuracy of pet by 2.", 7, 1, {0278}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[9] = AddMallPack("Great Argent Mango", "Argent Mango can increases the Constitution of pet by 2.", 7, 1, {0279}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[10] = AddMallPack("Great Shaitan Biscuit", "Shaitan Biscuit can increases the Spirit of pet by 2.", 7, 1, {0280}, 1, -1)
--	Marriage Fruits.
IGS.Category[MainTabName][SubTabName4].Packs[1] = AddMallPack("Demonic Fruit of Acidity", "Use during pet marriage to conceive a Fairy of Luck.", 19, 1, {3918}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[2] = AddMallPack("Demonic Fruit of Courage", "Use during pet marriage to conceive a Fairy of Constitution.", 19, 1, {3919}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[3] = AddMallPack("Demonic Fruit of Strength", "Use during pet marriage to conceive a Fairy of Strength.", 19, 1, {3920}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[4] = AddMallPack("Demonic Fruit of Intellect", "Use during pet marriage to conceive a Fairy of Spirit.", 19, 1, {3921}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[5] = AddMallPack("Demonic Fruit of Energy", "Use during pet marriage to conceive a Fairy of Accuracy.", 19, 1, {3922}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[6] = AddMallPack("Demonic Fruit of Aberrant", "Use during pet marriage to conceive a Fairy of Agility.", 19, 1, {3924}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[7] = AddMallPack("Demonic Fruit of Mystery", "Use during pet marriage to conceive a Fairy of Evil.", 19, 1, {3925}, 1, -1)
--	Novice Fairy Skills.
IGS.Category[MainTabName][SubTabName5].Packs[1] = AddMallPack("Novice Protection", "Pet Skill", 6, 1, {0243}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[2] = AddMallPack("Novice Berserk", "Pet Skill", 6, 1, {0246}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[3] = AddMallPack("Novice Magic", "Pet Skill", 6, 1, {0249}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[4] = AddMallPack("Novice Recover", "Pet Skill", 6, 1, {0252}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[5] = AddMallPack("Novice Meditation", "Pet Skill", 6, 1, {0259}, 1, -1)
--	Standard Fairy Skills.
IGS.Category[MainTabName][SubTabName5].Packs[6 ] = AddMallPack("Standard Protection", "Pet Skill", 6, 1, {0244}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[7 ] = AddMallPack("Standard Berserk", "Pet Skill", 6, 1, {0247}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[8 ] = AddMallPack("Standard Magic", "Pet Skill", 6, 1, {0250}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[9 ] = AddMallPack("Standard Recover", "Pet Skill", 6, 1, {0253}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[10] = AddMallPack("Standard Meditation", "Pet Skill", 6, 1, {0260}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
IGS.Category[MainTabName][SubTabName1].Pointer = AddMallTab(SubTabName1, IGS.Category[MainTabName][SubTabName1].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName2].Pointer = AddMallTab(SubTabName2, IGS.Category[MainTabName][SubTabName2].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName3].Pointer = AddMallTab(SubTabName3, IGS.Category[MainTabName][SubTabName3].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName4].Pointer = AddMallTab(SubTabName4, IGS.Category[MainTabName][SubTabName4].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName5].Pointer = AddMallTab(SubTabName5, IGS.Category[MainTabName][SubTabName5].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
