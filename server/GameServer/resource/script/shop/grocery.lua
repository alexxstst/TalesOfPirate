print("-- [Loading] Miscellaneous Category")

local MainTabName = 'Miscellaneous'
local SubTabName1 = 'Appearance'
local SubTabName2 = 'Voucher'
local SubTabName3 = 'Materials'
local SubTabName4 = 'Miscellaneous'
local SubTabName5 = 'Wings'
local SubTabName6 = 'Effect'

IGS.Category[MainTabName] = {
	[MainTabName] = {Packs = {}, Pointer = nil},
	[SubTabName1] = {Packs = {}, Pointer = nil},
	[SubTabName2] = {Packs = {}, Pointer = nil},
	[SubTabName3] = {Packs = {}, Pointer = nil},
	[SubTabName4] = {Packs = {}, Pointer = nil},
	[SubTabName5] = {Packs = {}, Pointer = nil},
	[SubTabName6] = {Packs = {}, Pointer = nil}
}

--	AddMallPack('Pack Title', 'Description', Price, 'Hot', Items ({ID, ID, ID} or {{ID =, Qty = , Qly = }, {ID =, Qty = , Qly = }), Ignored?, StockQty)
IGS.Category[MainTabName][SubTabName1].Packs[1] = AddMallPack("Lance Trendy Hairstyle Book", "Lance Trendy Book, depicting latest and fashionable hairstyles", 6, 1, {0931}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[2] = AddMallPack("Carsise Trendy Hairstyle Book", "Carsise Trendy Book, depicting latest and fashionable hairstyles", 6, 1, {0932}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[3] = AddMallPack("Phyllis Trendy Hairstyle Book", "Phyllis Trendy Book, depicting latest and fashionable hairstyles", 6, 1, {0933}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[4] = AddMallPack("Ami Trendy Hairstyle Book", "Ami Trendy Book, depicting latest and fashionable hairstyles", 6, 1, {0934}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[5] = AddMallPack("Magical Feather", "Said to be used on a wing to give it an aerial state.", 8, 1, {9601}, 1, -1)
IGS.Category[MainTabName][SubTabName1].Packs[6] = AddMallPack("Apparel Converter", "Use on an equip to turn it into an apparel.", 8, 1, {9602}, 1, -1)

IGS.Category[MainTabName][SubTabName2].Packs[1] = AddMallPack("Lv40 Unique Ring Voucher", "Double click to obtain random Lv40 ring", 3, 1, {19706}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[2] = AddMallPack("Lv50 Unique Ring Voucher", "Double click to obtain random Lv50 ring", 5, 1, {19707}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[3] = AddMallPack("Lv60 Unique Ring Voucher", "Double click to obtain random Lv60 ring", 7, 1, {19708}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[4] = AddMallPack("Lv40 Unique Necklace Voucher", "Double click to obtain random Lv40 necklace", 3, 1, {19711}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[5] = AddMallPack("Lv50 Unique Necklace Voucher", "Double click to obtain random Lv50 necklace", 5, 1, {19712}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[6] = AddMallPack("Lv60 Unique Necklace Voucher", "Double click to obtain random Lv60 necklace", 7, 1, {19713}, 1, -1)
IGS.Category[MainTabName][SubTabName2].Packs[7] = AddMallPack("Unique Coral Voucher", "Double click to obtain unique corals", 5, 1, {0582}, 1, -1)

IGS.Category[MainTabName][SubTabName3].Packs[1] = AddMallPack("Stone Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2625, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[2] = AddMallPack("Food Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2630, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[3] = AddMallPack("Special Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2634, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[4] = AddMallPack("Bone Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2635, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[5] = AddMallPack("Plant Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2636, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[6] = AddMallPack("Fur Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2637, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[7] = AddMallPack("Liquid Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2638, Qty = 12}}, 1, -1)
IGS.Category[MainTabName][SubTabName3].Packs[8] = AddMallPack("Wood Catalyst", "For use during Analyzation to analyze bone related items. Obtainable from Item Mall", 10, 1, {{ID = 2639, Qty = 12}}, 1, -1)

IGS.Category[MainTabName][SubTabName4].Packs[1] = AddMallPack("28 Inventory Slot", "Increases inventory slots to 28. Can be used only if you have 24 inventory slots.", 6, 1, {3088}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[2] = AddMallPack("32 Inventory Slot", "Increases inventory slots to 32. Can be used only if you have 28 inventory slots.", 6, 1, {3089}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[3] = AddMallPack("36 Inventory Slot", "Increases inventory slots to 36. Can be used only if you have 32 inventory slots.", 6, 1, {3090}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[4] = AddMallPack("40 Inventory Slot", "Increases inventory slots to 40. Can be used only if you have 36 inventory slots.", 6, 1, {3091}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[5] = AddMallPack("44 Inventory Slot", "Increases inventory slots to 44. Can be used only if you have 40 inventory slots.", 6, 1, {3092}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[6] = AddMallPack("48 Inventory Slot", "Increases inventory slots to 48. Can be used only if you have 44 inventory slots.", 6, 1, {3093}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[7] = AddMallPack("Book of Skill Reset", "Double click to reset all skills.", 18, 1, {996}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[8] = AddMallPack("Book of Stat Reset", "Double click to reset all stat points.", 18, 1, {997}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[9] = AddMallPack("Summon Scroll - Aries", "Double click to summon Aries Guardian.", 6, 1, {2962}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[9] = AddMallPack("Store Permit", "A requirement to upgrade your stall level.", 16, 1, {19674}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[10] = AddMallPack("Change Name Card", "Give this to GM and you can request a name change.", 25, 1, {19703}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[11] = AddMallPack("Intense Magic", "Increases magic power. Requires 1 Magical Clover per use", 15, 1, {3300}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[12] = AddMallPack("Crystalline Blessing", "Make self immune to melee and skill attack. However no other action can be taken. For Herbalist only", 15, 1, {3301}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[10] = AddMallPack("Super Rechargeable Battery", "Obtainable through Item Mall. Recharge energy of coral. High energy.", 2, 1, {1024}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[14] = AddMallPack("Magical Clover", "Increases magic power when item is properly used", 16, 1, {{ID = 3462, Qty = 99}}, 1, -1)
--IGS.Category[MainTabName][SubTabName4].Packs[15] = AddMallPack("Icy Crystal", "A crystal that is icy and chilling to touch", 16, 1, {{ID = 3463, Qty = 99}}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[11] = AddMallPack("Gold Pickaxe", "Mining tool for all class that produce double yield. Required level 10 to use.", 7, 1, {0208}, 1, -1)
IGS.Category[MainTabName][SubTabName4].Packs[12] = AddMallPack("Gold Axe", "Mining tool for all class that produce double yield. Required level 10 to use.", 7, 1, {0207}, 1, -1)

IGS.Category[MainTabName][SubTabName5].Packs[1] = AddMallPack("Ebony Dragon Wings", "Wing Apparel", 22, 1, {2506}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[2] = AddMallPack("Rainbow Wings", "Wing Apparel", 22, 1, {2507}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[3] = AddMallPack("Vampiric Wings", "Wing Apparel", 22, 1, {2508}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[4] = AddMallPack("Dragon Wings", "Wing Apparel", 22, 1, {2509}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[5] = AddMallPack("Devil Wings", "Wing Apparel", 22, 1, {2510}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[6] = AddMallPack("Elven Wings", "Wing Apparel", 22, 1, {2511}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[7] = AddMallPack("Butterfly Wings", "Wing Apparel", 22, 1, {2512}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[8] = AddMallPack("Angelic Wings", "Wing Apparel", 22, 1, {2513}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[9] = AddMallPack("Dreamer Wings", "Wing Apparel", 22, 1, {2514}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[10] = AddMallPack("Nature Wings", "Wing Apparel", 22, 1, {2515}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[11] = AddMallPack("Guardian Wings", "Wing Apparel", 22, 1, {2516}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[12] = AddMallPack("Undead Wings", "Wing Apparel", 22, 1, {2517}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[13] = AddMallPack("Fairy Wings", "Wing Apparel", 22, 1, {2518}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[14] = AddMallPack("Crystal Wings", "Wing Apparel", 22, 1, {2519}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[15] = AddMallPack("Tribal Wings", "Wing Apparel", 22, 1, {2520}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[16] = AddMallPack("Sunset Wings", "Wing Apparel", 22, 1, {2521}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[17] = AddMallPack("Night Sky Wings", "Wing Apparel", 22, 1, {2522}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[18] = AddMallPack("Fallen Guardian Wings", "Wing Apparel", 22, 1, {2523}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[19] = AddMallPack("Fallen Angel Wings", "Wing Apparel", 22, 1, {2524}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[20] = AddMallPack("Fallen Dragon Wings", "Wing Apparel", 22, 1, {2525}, 1, -1)
IGS.Category[MainTabName][SubTabName5].Packs[21] = AddMallPack("Thunder Wings", "Wing Apparel", 22, 1, {2526}, 1, -1)

IGS.Category[MainTabName][SubTabName6].Packs[1] = AddMallPack("Aura of Hardin", "Put to glow app slot to obtain unique effect.", 36, 1, {8110}, 1, -1)
IGS.Category[MainTabName][SubTabName6].Packs[2] = AddMallPack("Aura of Darkness", "Put to glow app slot to obtain unique effect.", 36, 1, {8111}, 1, -1)
IGS.Category[MainTabName][SubTabName6].Packs[3] = AddMallPack("Aura of Abaddon", "Put to glow app slot to obtain unique effect.", 36, 1, {8112}, 1, -1)
IGS.Category[MainTabName][SubTabName6].Packs[4] = AddMallPack("Aura of Asura", "Put to glow app slot to obtain unique effect.", 36, 1, {8109}, 1, -1)
IGS.Category[MainTabName][SubTabName6].Packs[5] = AddMallPack("Aura of Abyss", "Put to glow app slot to obtain unique effect.", 36, 1, {8113}, 1, -1)
IGS.Category[MainTabName][SubTabName6].Packs[6] = AddMallPack("Aura of Styx", "Put to glow app slot to obtain unique effect.", 36, 1, {8108}, 1, -1)

IGS.Category[MainTabName][MainTabName].Pointer = AddMallTab(MainTabName, IGS.Category[MainTabName][MainTabName].Packs)
IGS.Category[MainTabName][SubTabName1].Pointer = AddMallTab(SubTabName1, IGS.Category[MainTabName][SubTabName1].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName2].Pointer = AddMallTab(SubTabName2, IGS.Category[MainTabName][SubTabName2].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName3].Pointer = AddMallTab(SubTabName3, IGS.Category[MainTabName][SubTabName3].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName4].Pointer = AddMallTab(SubTabName4, IGS.Category[MainTabName][SubTabName4].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName5].Pointer = AddMallTab(SubTabName5, IGS.Category[MainTabName][SubTabName5].Packs, IGS.Category[MainTabName][MainTabName].Pointer)
IGS.Category[MainTabName][SubTabName6].Pointer = AddMallTab(SubTabName6, IGS.Category[MainTabName][SubTabName6].Packs, IGS.Category[MainTabName][MainTabName].Pointer)

