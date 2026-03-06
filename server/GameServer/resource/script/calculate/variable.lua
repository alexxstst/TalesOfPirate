print("-- [Loading] Variable")
-------------------------------------------------------------------------------------------------------------------------
--------------------------------------------- ** Enable / Disable System ** ---------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Sys.Level.Active = true								-- Enable/disable levelling system.
Server.Sys.Bank.Active = true								-- Are players able to access their bank inventory?
Server.Sys.Apparel.Active = false							-- Are players able to upgrade their items?
Server.Sys.Fairy.Active = true								-- Enable/disable everything related to fairies.
Server.Sys.Socket.Active = true								-- Enable/disable socket system.
Server.Sys.Rates.Global.Active = true						-- Enable/disable global rates.
Server.Sys.Rates.Hour.Active = false						-- Enable/disable experience hour rates.
Server.Sys.Rates.Day.Active = true							-- Enable/disable experience day rates.
Server.Sys.Rates.Map.Active = true							-- Enable/disable experience map rates.
Server.Sys.Rates.Level.Active = false						-- Enable/disable experience level rates.
Server.Sys.Rates.Class.Active = false						-- Enable/disable experience class rates.
Server.Sys.Ticket.Active = true								-- Enable/disable ticket system.
Server.Sys.BlackDragonAltar.Active = true					-- Enable/disable "Black Dragon Altar" system.
Server.Sys.MasterAccount = false							-- Enable/disable to check if master account is legit (Kick and Ban the account if NOT)
-------------------------------------------------------------------------------------------------------------------------
------------------------------------------ ** Master Accounts / Validation ** -------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
AuthorizedGM = {}
AuthorizedGM['admin'] = 1									-- Username for master account
AuthorizedGM['admin4'] = 1	
-------------------------------------------------------------------------------------------------------------------------
----------------------------------------------- ** Modify Global Rates ** -----------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Level.Limit = 80										-- Maximum player level, must also edit "character_lvup.txt" in order to take effect.
Server.Rates.Modifier = false								-- Activate server experience rate modifier
Server.Rates.Global.EXP = 1									-- Rate at which a lone player grows.
Server.Rates.Global.TeamEXP = 1								-- Rate at which an entire player team grows.
Server.Rates.Global.DROP = 1								-- Rate at which items are dropped.
Server.Rates.Global.ShipEXP = 1								-- Rate at which a player's boat gains experience.
Server.Rates.Global.FairyEXP = 1							-- Rate at which a player's fairy grows.
Server.Rates.Global.MissionGold = 1							-- The rate that the player receives gold from missions.
Server.Rates.Global.Resource = 1							-- Rate of gathering resources.
-------------------------------------------------------------------------------------------------------------------------
------------------------------------------- ** Modify Experience Rate(Map) ** -------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Rates.Day[0] = 1										-- Rate for "Sunday".
Server.Rates.Day[1] = 1										-- Rate for "Monday".
Server.Rates.Day[2] = 1										-- Rate for "Tuesday".
Server.Rates.Day[3] = 1										-- Rate for "Wednesday".
Server.Rates.Day[4] = 1										-- Rate for "Thursday".
Server.Rates.Day[5] = 1										-- Rate for "Friday".
Server.Rates.Day[6] = 1										-- Rate for "Saturday".

Server.Rates.Map["garner"] = 1								-- Rate for "Ascaron"(Argent City, etc).
Server.Rates.Map["magicsea"] = 1							-- Rate for "Magical Ocean"(Shaitan City, etc).
Server.Rates.Map["darkblue"] = 1							-- Rate for "Deep Blue"(Icicle Castle, etc).
Server.Rates.Map["abandonedcity"] = 0						-- Rate for "Forsaken City 1".
Server.Rates.Map["abandonedcity2"] = 0						-- Rate for "Forsaken City 2".
Server.Rates.Map["abandonedcity3"] = 0						-- Rate for "Forsaken City 3.
Server.Rates.Map["darkswamp"] = 0							-- Rate for "Dark Swamp"(1/2/3).
Server.Rates.Map["garner2"] = 0								-- Rate for "Chaos Argent".
Server.Rates.Map["puzzleworld"] = 1							-- Rate for "Demonic World 1".
Server.Rates.Map["puzzleworld2"] = 1						-- Rate for "Demonic World 2".
-------------------------------------------------------------------------------------------------------------------------
---------------------------------------------- ** Enable / Disable Maps ** ----------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Teleport.Haven = true
Server.Teleport.City = true
Server.Sys.Maps['Dream Island'].Active = true				-- If disabled, everything related to it will not function.
Server.Sys.Maps['Chaos Argent'].Active = true				-- If disabled, everything related to it will not function.
Server.Sys.Maps['Forsaken City'].Active = true				-- If disabled, everything related to it will not function.
Server.Sys.Maps['Dark Swamp'].Active = true					-- If disabled, everything related to it will not function.
Server.Sys.Maps['Demonic World'].Active = true				-- If disabled, everything related to it will not function.
Server.Sys.Maps['PKmap'].Active = true						-- If disabled, everything related to it will not function.
Server.Sys.Maps['secretgarden'].Active = false				-- If disabled, everything related to it will not function.
-------------------------------------------------------------------------------------------------------------------------
-------------------------------------- ** Modify Apparel / Forge / Gem Variable ** --------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Equipment.Upgrade.Limit = 10							-- Maximum apparel upgrade level(10 = 100%, additional 1 = 2% increase).
Server.Equipment.Upgrade.Cost = 10000						-- Cost to for an apparel upgrade.
Server.Socket.Limit = 3										-- Maximum sockets allowed in equipment.
Server.Socket.Cost = 50000									-- Cost for making a socket in an equipment.
-------------------------------------------------------------------------------------------------------------------------
------------------------------------------ ** Modify Cloak System Variables ** ------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Cloak.Item.ID = 9604									-- This is "Admiral Cloak" ID.
Server.Cloak.Item.Upgrade = 9605							-- This is "Admiral Cloak Upgrade Device" ID.
Server.Cloak.Level = 5										-- This is the maximum level "Admiral Cloak" can reach.
Server.Cloak.StatPerLevel = 1								-- Number of stats added per level.
Server.Cloak.Rate[1] = 1.0
Server.Cloak.Rate[2] = 0.9
Server.Cloak.Rate[3] = 0.8
Server.Cloak.Rate[4] = 0.7
Server.Cloak.Rate[5] = 0.6
Server.Cloak.Rate[6] = 0.5
Server.Cloak.Rate[7] = 0.4
Server.Cloak.Rate[8] = 0.3
Server.Cloak.Rate[9] = 0.2
Server.Cloak.Rate[10] = 0.1
-------------------------------------------------------------------------------------------------------------------------
------------------------------------------ ** Modify Fairy System Variables ** ------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Fairy.Marriage['Cost'] = 200							-- Cost to marry two fairies:(MaxLv - Lv_Fairy1) *(MaxLv - Lv_Fairy2) * Cost
Server.Fairy.Level.Normal = 42								-- Maximum level a fairy can reach by using normal/great fruits.
Server.Fairy.Level.Improved = 62							-- Maximum level a fairy can reach by using improved fruits.
Server.Fairy.Level.Minimum = 0								-- Minimum level a fairy needs to be for player to receive fairy coinds.
Server.Fairy.Level.Maximum = 42								-- Overall maximum level a fairy can reach.
Server.Fairy.Possession.Stamina = 100						-- Amount of stamina a fairy needs in order to activate possesion.
Server.Fairy.Effect.Strength = 132							-- Skill effect ID for fairy possession.
Server.Fairy.Effect.Constitution = 168						-- Skill effect ID for fairy possession.
Server.Fairy.Effect.Spirit = 169							-- Skill effect ID for fairy possession.
Server.Fairy.Effect.Accuracy = 170							-- Skill effect ID for fairy possession.
Server.Fairy.Effect.Agility = 171                           -- Skill effect ID for fairy possession.
Server.Fairy.Effect.Evil = 172                              -- Skill effect ID for fairy possession.
Server.Fairy.Effect.Luck = 173                              -- Skill effect ID for fairy possession.
Server.Fairy.Effect.MordoJR = 174                           -- Skill effect ID for fairy possession.
Server.Fairy.Effect.AngelaJR = 0                            -- Skill effect ID for fairy possession.
Server.Fairy.Fruits.Normal = 1								-- Amount of level(s) a fairy will gain when using a normal fruit.
Server.Fairy.Fruits.Great = 2								-- Amount of level(s) a fairy will gain when using a great fruit.
Server.Fairy.Fruits.Improved = 1							-- Amount of level(s) a fairy will gain when using a improved fruit.
Server.Fairy.Ration[227] = 50								-- Amount of stamina given by a fairy ration, must place ration ID and amount.
Server.Fairy.Ration[2312] = 50								-- Amount of stamina given by a auto ration, must place ration ID and amount.
Server.Fairy.Ration[3152] = 5								-- Amount of stamina given by pet food, must place ration ID and amount.
Server.Fairy.Ration[6841] = 100								-- Amount of stamina given by great fairy ration, must place ration ID and amount.
Server.Fairy.Ration[6842] = 100								-- Amount of stamina given by great auto ration, must place ration ID and amount.
Server.Fairy.AutoFeed.Stamina = 0.5							-- Minimum stamina percentage in order for auto feed to work.
Server.Fairy.AutoFeed.Normal = 2312							-- Item ID for normal auto ration.
Server.Fairy.AutoFeed.Great = 6842							-- Item ID for great auto ration.
Server.Fairy.Fruit[0222] = {LV = 1, ATTR = ITEMATTR_VAL_STR}	-- Normal Leveling Fruits
Server.Fairy.Fruit[0223] = {LV = 1, ATTR = ITEMATTR_VAL_AGI}	-- Normal Leveling Fruits
Server.Fairy.Fruit[0224] = {LV = 1, ATTR = ITEMATTR_VAL_DEX}	-- Normal Leveling Fruits
Server.Fairy.Fruit[0225] = {LV = 1, ATTR = ITEMATTR_VAL_CON}	-- Normal Leveling Fruits
Server.Fairy.Fruit[0226] = {LV = 1, ATTR = ITEMATTR_VAL_STA}	-- Normal Leveling Fruits
Server.Fairy.Fruit[0276] = {LV = 2, ATTR = ITEMATTR_VAL_STR}	-- Great Leveling Fruits
Server.Fairy.Fruit[0277] = {LV = 2, ATTR = ITEMATTR_VAL_AGI}	-- Great Leveling Fruits
Server.Fairy.Fruit[0278] = {LV = 2, ATTR = ITEMATTR_VAL_DEX}	-- Great Leveling Fruits
Server.Fairy.Fruit[0279] = {LV = 2, ATTR = ITEMATTR_VAL_CON}	-- Great Leveling Fruits
Server.Fairy.Fruit[0280] = {LV = 2, ATTR = ITEMATTR_VAL_STA}	-- Great Leveling Fruits
Server.Fairy.Fruit[7003] = {LV = 1, ATTR = ITEMATTR_VAL_STR}	-- Improved Leveling Fruits
Server.Fairy.Fruit[7004] = {LV = 1, ATTR = ITEMATTR_VAL_AGI}	-- Improved Leveling Fruits
Server.Fairy.Fruit[7005] = {LV = 1, ATTR = ITEMATTR_VAL_DEX}	-- Improved Leveling Fruits
Server.Fairy.Fruit[7006] = {LV = 1, ATTR = ITEMATTR_VAL_CON}	-- Improved Leveling Fruits
Server.Fairy.Fruit[7007] = {LV = 1, ATTR = ITEMATTR_VAL_STA}	-- Improved Leveling Fruits
Server.Fairy.ID['Fairy of Luck'] = 231						-- Item ID for fairy: Fairy of Luck
Server.Fairy.ID['Fairy of Evil'] = 237						-- Item ID for fairy: Fairy of Evil
Server.Fairy.ID['Fairy of Strength'] = 232					-- Item ID for fairy: Fairy of Strength
Server.Fairy.ID['Fairy of Constitution'] = 233				-- Item ID for fairy: Fairy of Constitution
Server.Fairy.ID['Fairy of Spirit'] = 234					-- Item ID for fairy: Fairy of Spirit
Server.Fairy.ID['Fairy of Accuracy'] = 235					-- Item ID for fairy: Fairy of Accuracy
Server.Fairy.ID['Fairy of Agility'] = 236					-- Item ID for fairy: Fairy of Agility
Server.Fairy.ID['Mordo'] = 680								-- Item ID for fairy: Mordo
Server.Fairy.ID['Mordo JR'] = 681							-- Item ID for fairy: Mordo JR
Server.Fairy.ID['Angela'] = 7125							-- Item ID for fairy: Angela
Server.Fairy.ID['Angela JR'] = 7126							-- Item ID for fairy: Angela JR
-------------------------------------------------------------------------------------------------------------------------
--------------------------------------- ** Modify Extra Fairy System Variables ** ---------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.Fairy.Marriage[1] = {ID = 3918, Fairy = 0571, Item1 = 4530, Item2 = 3434}	-- Luck
Server.Fairy.Marriage[2] = {ID = 3920, Fairy = 0573, Item1 = 1196, Item2 = 3436}	-- Strength
Server.Fairy.Marriage[3] = {ID = 3919, Fairy = 0572, Item1 = 4531, Item2 = 3435}	-- Constitution
Server.Fairy.Marriage[4] = {ID = 3921, Fairy = 0574, Item1 = 4533, Item2 = 3437}	-- Spirit
Server.Fairy.Marriage[5] = {ID = 3922, Fairy = 0575, Item1 = 4537, Item2 = 3444}	-- Accuracy
Server.Fairy.Marriage[6] = {ID = 3924, Fairy = 0576, Item1 = 4540, Item2 = 3443}	-- Agility
Server.Fairy.Marriage[7] = {ID = 3925, Fairy = 0577, Item1 = 1253, Item2 = 3442}	-- Evil
Server.Fairy.Egg[422] = {ID = 183, Marriage = 0}	-- Egg of Life
Server.Fairy.Egg[423] = {ID = 184, Marriage = 0}	-- Egg of Darkness
Server.Fairy.Egg[424] = {ID = 185, Marriage = 0}	-- Egg of Virtue
Server.Fairy.Egg[425] = {ID = 186, Marriage = 0}	-- Egg of Kudos
Server.Fairy.Egg[426] = {ID = 187, Marriage = 0}	-- Egg of Faith
Server.Fairy.Egg[427] = {ID = 188, Marriage = 0}	-- Egg of Valor
Server.Fairy.Egg[428] = {ID = 189, Marriage = 0}	-- Egg of Hope
Server.Fairy.Egg[429] = {ID = 190, Marriage = 0}	-- Egg of Woe
Server.Fairy.Egg[679] = {ID = 680, Marriage = 0}	-- Egg of Mordo
Server.Fairy.Egg[430] = {ID = 191, Marriage = 0}	-- Egg of Love
Server.Fairy.Egg[431] = {ID = 199, Marriage = 0}	-- Egg of Heart
Server.Fairy.Egg[571] = {ID = 231, Marriage = 1}	-- Egg of Luck
Server.Fairy.Egg[572] = {ID = 233, Marriage = 1}	-- Egg of Constitution
Server.Fairy.Egg[573] = {ID = 232, Marriage = 1}	-- Egg of Strength
Server.Fairy.Egg[574] = {ID = 234, Marriage = 1}	-- Egg of Spirit
Server.Fairy.Egg[575] = {ID = 235, Marriage = 1}	-- Egg of Accuracy
Server.Fairy.Egg[576] = {ID = 236, Marriage = 1}	-- Egg of Agility
Server.Fairy.Egg[577] = {ID = 237, Marriage = 1}	-- Egg of Evil
Server.Fairy.Egg[910] = {ID = 681, Marriage = 1}	-- Egg of Mordo Junior
Server.Fairy.Egg[7127] = {ID = 7125, Marriage = 0}	-- Egg of Angela
Server.Fairy.Egg[7131] = {ID = 7126, Marriage = 1}	-- Egg of Angela Junior
---------------------------------------------------------------------------------------------------------------------------
------------------------------------------ ** Fairy Skill Learning Books Vars ** -----------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Fairy.Skill[0243] = {Skill = 01, Level = 1} -- Novice Protection
Server.Fairy.Skill[0244] = {Skill = 01, Level = 2} -- Standard Protection
Server.Fairy.Skill[0245] = {Skill = 01, Level = 3} -- Expert Protection
Server.Fairy.Skill[0246] = {Skill = 02, Level = 1} -- Novice Berserk
Server.Fairy.Skill[0247] = {Skill = 02, Level = 2} -- Standard Berserk
Server.Fairy.Skill[0248] = {Skill = 02, Level = 3} -- Expert Berserk
Server.Fairy.Skill[0249] = {Skill = 03, Level = 1} -- Novice Magic
Server.Fairy.Skill[0250] = {Skill = 03, Level = 2} -- Standard Magic
Server.Fairy.Skill[0251] = {Skill = 03, Level = 3} -- Expert Magic
Server.Fairy.Skill[0252] = {Skill = 04, Level = 1} -- Novice Recover
Server.Fairy.Skill[0253] = {Skill = 04, Level = 2} -- Standard Recover
Server.Fairy.Skill[0254] = {Skill = 04, Level = 3} -- Expert Recover
Server.Fairy.Skill[0259] = {Skill = 05, Level = 1} -- Novice Meditation
Server.Fairy.Skill[0260] = {Skill = 05, Level = 2} -- Standard Meditation
Server.Fairy.Skill[0261] = {Skill = 05, Level = 3} -- Expert Meditation
Server.Fairy.Skill[1055] = {Skill = 13, Level = 1} -- Novice Pet Manufacturing
Server.Fairy.Skill[1056] = {Skill = 13, Level = 2} -- Standard Pet Manufacturing
Server.Fairy.Skill[1057] = {Skill = 13, Level = 3} -- Expert Pet Manufacturing
Server.Fairy.Skill[1058] = {Skill = 14, Level = 1} -- Novice Pet Crafting
Server.Fairy.Skill[1059] = {Skill = 14, Level = 2} -- Standard Pet Crafting
Server.Fairy.Skill[1060] = {Skill = 14, Level = 3} -- Expert Pet Crafting
Server.Fairy.Skill[1061] = {Skill = 15, Level = 1} -- Novice Pet Analyze
Server.Fairy.Skill[1062] = {Skill = 15, Level = 2} -- Standard Pet Analyze
Server.Fairy.Skill[1063] = {Skill = 15, Level = 3} -- Expert Pet Analyze
Server.Fairy.Skill[1064] = {Skill = 16, Level = 1} -- Novice Pet Cooking
Server.Fairy.Skill[1065] = {Skill = 16, Level = 2} -- Standard Pet Cooking
Server.Fairy.Skill[1066] = {Skill = 16, Level = 3} -- Expert Pet Cooking
---------------------------------------------------------------------------------------------------------------------------
--------------------------------------------- ** Fairy Fun Skill Books Vars ** --------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Fairy.Funny.Skill[6456] = {Skill = 319, Level = 1} -- Pet Spellbook-Dumb
Server.Fairy.Funny.Skill[6457] = {Skill = 314, Level = 1} -- Pet Spellbook-Bra
Server.Fairy.Funny.Skill[6458] = {Skill = 315, Level = 1} -- Pet Spellbook-Briefs
Server.Fairy.Funny.Skill[6459] = {Skill = 317, Level = 1} -- Pet Spellbook-Snooty
Server.Fairy.Funny.Skill[6460] = {Skill = 318, Level = 1} -- Pet Spellbook-Trickster
Server.Fairy.Funny.Skill[6461] = {Skill = 316, Level = 1} -- Pet Spellbook-Stupid
Server.Fairy.Funny.Skill[6462] = {Skill = 312, Level = 1} -- Pet Spellbook-Defecate
Server.Fairy.Funny.Skill[6463] = {Skill = 315, Level = 1} -- Pet Spellbook-Coin Shower
---------------------------------------------------------------------------------------------------------------------------
------------------------------------ ** Fairy Possession Skill Learning Books Vars ** -------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Fairy.Possession.Skill[239] = {Skill = 280, Level = 01} -- Novice Fairy Possession	
Server.Fairy.Possession.Skill[608] = {Skill = 280, Level = 02} -- Standard Fairy Possession
Server.Fairy.Possession.Skill[609] = {Skill = 280, Level = 03} -- Expert Fairy Possession	
Server.Fairy.Possession.Skill[610] = {Skill = 311, Level = 01} -- Novice Self Destruct
Server.Fairy.Possession.Skill[611] = {Skill = 311, Level = 02} -- Standard Self Destruct
Server.Fairy.Possession.Skill[612] = {Skill = 311, Level = 03} -- Expert Self Destruct
---------------------------------------------------------------------------------------------------------------------------
------------------------------------------ ** Player Skill Learning Books Vars ** -----------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Player.Skill[3160] = {Skill = 062, MinLevel = nil, MaxLevel = nil} -- Sword Mastery
Server.Player.Skill[3161] = {Skill = 063, MinLevel = nil, MaxLevel = nil} -- Will of Steel
Server.Player.Skill[3297] = {Skill = 242, MinLevel = nil, MaxLevel = nil} -- Taunt
Server.Player.Skill[3298] = {Skill = 243, MinLevel = nil, MaxLevel = nil} -- Roar
Server.Player.Skill[3293] = {Skill = 127, MinLevel = nil, MaxLevel = nil} -- Tiger Roar
Server.Player.Skill[3162] = {Skill = 064, MinLevel = nil, MaxLevel = nil} -- Strengthen
Server.Player.Skill[3163] = {Skill = 065, MinLevel = nil, MaxLevel = nil} -- Deftness
Server.Player.Skill[3164] = {Skill = 066, MinLevel = nil, MaxLevel = nil} -- Concentration
Server.Player.Skill[3165] = {Skill = 081, MinLevel = nil, MaxLevel = nil} -- Illusion Slash
Server.Player.Skill[3166] = {Skill = 082, MinLevel = nil, MaxLevel = nil} -- Mighty Strike
Server.Player.Skill[3167] = {Skill = 107, MinLevel = nil, MaxLevel = nil} -- Howl
Server.Player.Skill[3168] = {Skill = 067, MinLevel = nil, MaxLevel = nil} -- Greatsword Mastery
Server.Player.Skill[3170] = {Skill = 084, MinLevel = nil, MaxLevel = nil} -- Berserk
Server.Player.Skill[3172] = {Skill = 068, MinLevel = nil, MaxLevel = nil} -- Blood Bull
Server.Player.Skill[3173] = {Skill = 083, MinLevel = nil, MaxLevel = nil} -- Primal Rage
Server.Player.Skill[3174] = {Skill = 109, MinLevel = nil, MaxLevel = nil} -- Dual Sword Mastery
Server.Player.Skill[3175] = {Skill = 069, MinLevel = nil, MaxLevel = nil} -- Evasion
Server.Player.Skill[3176] = {Skill = 070, MinLevel = nil, MaxLevel = nil} -- Blood Frenzy
Server.Player.Skill[3177] = {Skill = 123, MinLevel = nil, MaxLevel = nil} -- Stealth
Server.Player.Skill[3179] = {Skill = 086, MinLevel = nil, MaxLevel = nil} -- Shadow Slash
Server.Player.Skill[3180] = {Skill = 087, MinLevel = nil, MaxLevel = nil} -- Poison Dart
Server.Player.Skill[3187] = {Skill = 074, MinLevel = nil, MaxLevel = nil} -- Range Mastery
Server.Player.Skill[3188] = {Skill = 075, MinLevel = nil, MaxLevel = nil} -- Windwalk
Server.Player.Skill[3190] = {Skill = 090, MinLevel = nil, MaxLevel = nil} -- Dual Shot
Server.Player.Skill[3192] = {Skill = 078, MinLevel = nil, MaxLevel = nil} -- Firegun Mastery
Server.Player.Skill[3193] = {Skill = 112, MinLevel = nil, MaxLevel = nil} -- Meteor Shower
Server.Player.Skill[3197] = {Skill = 093, MinLevel = nil, MaxLevel = nil} -- Frozen Arrow
Server.Player.Skill[3198] = {Skill = 113, MinLevel = nil, MaxLevel = nil} -- Magma Bullet
Server.Player.Skill[3202] = {Skill = 094, MinLevel = nil, MaxLevel = nil} -- Cripple
Server.Player.Skill[3203] = {Skill = 095, MinLevel = nil, MaxLevel = nil} -- Enfeeble
Server.Player.Skill[3204] = {Skill = 096, MinLevel = nil, MaxLevel = nil} -- Headshot
Server.Player.Skill[3205] = {Skill = 079, MinLevel = nil, MaxLevel = nil} -- Vigor
Server.Player.Skill[3206] = {Skill = 097, MinLevel = nil, MaxLevel = nil} -- Heal
Server.Player.Skill[3207] = {Skill = 098, MinLevel = nil, MaxLevel = nil} -- Recover
Server.Player.Skill[3208] = {Skill = 099, MinLevel = nil, MaxLevel = nil} -- Spiritual Bolt
Server.Player.Skill[3209] = {Skill = 116, MinLevel = nil, MaxLevel = nil} -- True Sight
Server.Player.Skill[3210] = {Skill = 100, MinLevel = nil, MaxLevel = nil} -- Spiritual Fire
Server.Player.Skill[3211] = {Skill = 101, MinLevel = nil, MaxLevel = nil} -- Tempest Boost
Server.Player.Skill[3212] = {Skill = 080, MinLevel = nil, MaxLevel = nil} -- Divine Grace
Server.Player.Skill[3215] = {Skill = 124, MinLevel = nil, MaxLevel = nil} -- Revival
Server.Player.Skill[3216] = {Skill = 102, MinLevel = nil, MaxLevel = nil} -- Tornado Swirl
Server.Player.Skill[3217] = {Skill = 103, MinLevel = nil, MaxLevel = nil} -- Angelic Shield
Server.Player.Skill[3218] = {Skill = 104, MinLevel = nil, MaxLevel = nil} -- Seal of Elder
Server.Player.Skill[3219] = {Skill = 105, MinLevel = nil, MaxLevel = nil} -- Shadow Insignia
Server.Player.Skill[3220] = {Skill = 119, MinLevel = nil, MaxLevel = nil} -- Cursed Fire
Server.Player.Skill[3222] = {Skill = 121, MinLevel = nil, MaxLevel = nil} -- Abyss Mire
Server.Player.Skill[3223] = {Skill = 106, MinLevel = nil, MaxLevel = nil} -- Energy Shield
Server.Player.Skill[3224] = {Skill = 122, MinLevel = nil, MaxLevel = nil} -- Healing Spring
Server.Player.Skill[3227] = {Skill = 210, MinLevel = nil, MaxLevel = nil} -- Diligence
Server.Player.Skill[3228] = {Skill = 211, MinLevel = nil, MaxLevel = nil} -- Current
Server.Player.Skill[3229] = {Skill = 212, MinLevel = nil, MaxLevel = nil} -- Conch Armor
Server.Player.Skill[3230] = {Skill = 213, MinLevel = nil, MaxLevel = nil} -- Tornado
Server.Player.Skill[3231] = {Skill = 214, MinLevel = nil, MaxLevel = nil} -- Lightning Bolt
Server.Player.Skill[3232] = {Skill = 215, MinLevel = nil, MaxLevel = nil} -- Algae Entanglement
Server.Player.Skill[3233] = {Skill = 216, MinLevel = nil, MaxLevel = nil} -- Conch Ray
Server.Player.Skill[3234] = {Skill = 217, MinLevel = nil, MaxLevel = nil} -- Tail Wind
Server.Player.Skill[3235] = {Skill = 218, MinLevel = nil, MaxLevel = nil} -- Whirlpool
Server.Player.Skill[3236] = {Skill = 219, MinLevel = nil, MaxLevel = nil} -- Fog
Server.Player.Skill[3237] = {Skill = 220, MinLevel = nil, MaxLevel = nil} -- Lightning Curtain
Server.Player.Skill[3238] = {Skill = 222, MinLevel = nil, MaxLevel = nil} -- Break Armor
Server.Player.Skill[3239] = {Skill = 223, MinLevel = nil, MaxLevel = nil} -- Rousing
Server.Player.Skill[3241] = {Skill = 224, MinLevel = nil, MaxLevel = nil} -- Venom Arrow
Server.Player.Skill[3242] = {Skill = 225, MinLevel = nil, MaxLevel = nil} -- Harden
Server.Player.Skill[3300] = {Skill = 256, MinLevel = nil, MaxLevel = nil} -- Intense Magic
Server.Player.Skill[3301] = {Skill = 255, MinLevel = nil, MaxLevel = nil} -- Crystalline
Server.Player.Skill[2956] = {Skill = 453, MinLevel = nil, MaxLevel = nil} -- Ethereal Slash
Server.Player.Skill[2957] = {Skill = 455, MinLevel = nil, MaxLevel = nil} -- Super consciousness
Server.Player.Skill[2958] = {Skill = 457, MinLevel = nil, MaxLevel = nil} -- Beast Legion Smash
Server.Player.Skill[2959] = {Skill = 458, MinLevel = nil, MaxLevel = nil} -- Red Thunder Cannon
Server.Player.Skill[2960] = {Skill = 454, MinLevel = nil, MaxLevel = nil} -- Devil Curse
Server.Player.Skill[2961] = {Skill = 456, MinLevel = nil, MaxLevel = nil} -- Holy Judgement
Server.Player.Skill[6050] = {Skill = 475, MinLevel = 1, MaxLevel = 10}    -- Newbie Strike
---------------------------------------------------------------------------------------------------------------------------
-------------------------------------------- ** Life Skill Learning Books Vars ** -----------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Life.Skill[2679] = {Skill = 338, AddLevel = 1, Level = 0, Prerequisite = {Skill = 200, Level = 1}, Give = 1068}	-- Lv1 Manufacturing Guide	
Server.Life.Skill[2680] = {Skill = 338, AddLevel = 2, Level = 1, Prerequisite = {Skill = 200, Level = 2}, Give = nil}	-- Lv2 Manufacturing Guide	
Server.Life.Skill[2681] = {Skill = 338, AddLevel = 3, Level = 2, Prerequisite = {Skill = 200, Level = 3}, Give = nil}	-- Lv3 Manufacturing Guide	
Server.Life.Skill[2682] = {Skill = 338, AddLevel = 4, Level = 3, Prerequisite = {Skill = 200, Level = 4}, Give = nil}	-- Lv4 Manufacturing Guide	
Server.Life.Skill[2683] = {Skill = 338, AddLevel = 5, Level = 4, Prerequisite = {Skill = 200, Level = 5}, Give = nil}	-- Lv5 Manufacturing Guide	
Server.Life.Skill[2684] = {Skill = 338, AddLevel = 6, Level = 5, Prerequisite = {Skill = 200, Level = 6}, Give = nil}	-- Lv6 Manufacturing Guide	
Server.Life.Skill[2685] = {Skill = 338, AddLevel = 7, Level = 6, Prerequisite = {Skill = 200, Level = 7}, Give = nil}	-- Lv7 Manufacturing Guide	
Server.Life.Skill[2686] = {Skill = 338, AddLevel = 8, Level = 7, Prerequisite = {Skill = 200, Level = 8}, Give = nil}	-- Lv8 Manufacturing Guide	
Server.Life.Skill[2687] = {Skill = 338, AddLevel = 9, Level = 8, Prerequisite = {Skill = 200, Level = 9}, Give = nil}	-- Lv9 Manufacturing Guide	
Server.Life.Skill[2688] = {Skill = 338, AddLevel = 10, Level = 9, Prerequisite = {Skill = 200, Level = 10}, Give = nil}	-- Lv10 Manufacturing Guide
Server.Life.Skill[2689] = {Skill = 339, AddLevel = 1, Level = 0, Prerequisite = {Skill = 231, Level = 1}, Give = 1067}	-- Lv1 Cooking Guide	
Server.Life.Skill[2690] = {Skill = 339, AddLevel = 2, Level = 1, Prerequisite = {Skill = 231, Level = 2}, Give = nil}	-- Lv2 Cooking Guide	
Server.Life.Skill[2691] = {Skill = 339, AddLevel = 3, Level = 2, Prerequisite = {Skill = 231, Level = 3}, Give = nil}	-- Lv3 Cooking Guide	
Server.Life.Skill[2692] = {Skill = 339, AddLevel = 4, Level = 3, Prerequisite = {Skill = 231, Level = 4}, Give = nil}	-- Lv4 Cooking Guide	
Server.Life.Skill[2693] = {Skill = 339, AddLevel = 5, Level = 4, Prerequisite = {Skill = 231, Level = 5}, Give = nil}	-- Lv5 Cooking Guide	
Server.Life.Skill[2694] = {Skill = 339, AddLevel = 6, Level = 5, Prerequisite = {Skill = 231, Level = 6}, Give = nil}	-- Lv6 Cooking Guide	
Server.Life.Skill[2695] = {Skill = 339, AddLevel = 7, Level = 6, Prerequisite = {Skill = 231, Level = 7}, Give = nil}	-- Lv7 Cooking Guide	
Server.Life.Skill[2696] = {Skill = 339, AddLevel = 8, Level = 7, Prerequisite = {Skill = 231, Level = 8}, Give = nil}	-- Lv8 Cooking Guide	
Server.Life.Skill[2697] = {Skill = 339, AddLevel = 9, Level = 8, Prerequisite = {Skill = 231, Level = 9}, Give = nil}	-- Lv9 Cooking Guide	
Server.Life.Skill[2698] = {Skill = 339, AddLevel = 10, Level = 9, Prerequisite = {Skill = 231, Level = 10}, Give = nil}	-- Lv10 Cooking Guide
Server.Life.Skill[2699] = {Skill = 340, AddLevel = 1, Level = 0, Prerequisite = {Skill = 201, Level = 1}, Give = 1069}	-- Lv1 Crafting Guide	
Server.Life.Skill[2700] = {Skill = 340, AddLevel = 2, Level = 1, Prerequisite = {Skill = 201, Level = 2}, Give = nil}	-- Lv2 Crafting Guide	
Server.Life.Skill[2701] = {Skill = 340, AddLevel = 3, Level = 2, Prerequisite = {Skill = 201, Level = 3}, Give = nil}	-- Lv3 Crafting Guide	
Server.Life.Skill[2702] = {Skill = 340, AddLevel = 4, Level = 3, Prerequisite = {Skill = 201, Level = 4}, Give = nil}	-- Lv4 Crafting Guide	
Server.Life.Skill[2703] = {Skill = 340, AddLevel = 5, Level = 4, Prerequisite = {Skill = 201, Level = 5}, Give = nil}	-- Lv5 Crafting Guide	
Server.Life.Skill[2704] = {Skill = 340, AddLevel = 6, Level = 5, Prerequisite = {Skill = 201, Level = 6}, Give = nil}	-- Lv6 Crafting Guide	
Server.Life.Skill[2705] = {Skill = 340, AddLevel = 7, Level = 6, Prerequisite = {Skill = 201, Level = 7}, Give = nil}	-- Lv7 Crafting Guide	
Server.Life.Skill[2706] = {Skill = 340, AddLevel = 8, Level = 7, Prerequisite = {Skill = 201, Level = 8}, Give = nil}	-- Lv8 Crafting Guide	
Server.Life.Skill[2707] = {Skill = 340, AddLevel = 9, Level = 8, Prerequisite = {Skill = 201, Level = 9}, Give = nil}	-- Lv9 Crafting Guide	
Server.Life.Skill[2708] = {Skill = 340, AddLevel = 10, Level = 9, Prerequisite = {Skill = 201, Level = 10}, Give = nil}	-- Lv10 Crafting Guide
Server.Life.Skill[2709] = {Skill = 341, AddLevel = 1, Level = 0, Prerequisite = {Skill = 232, Level = 1}, Give = 1070}	-- Lv1 Analyze Guide	
Server.Life.Skill[2710] = {Skill = 341, AddLevel = 2, Level = 1, Prerequisite = {Skill = 232, Level = 2}, Give = nil}	-- Lv2 Analyze Guide	
Server.Life.Skill[2711] = {Skill = 341, AddLevel = 3, Level = 2, Prerequisite = {Skill = 232, Level = 3}, Give = nil}	-- Lv3 Analyze Guide	
Server.Life.Skill[2712] = {Skill = 341, AddLevel = 4, Level = 3, Prerequisite = {Skill = 232, Level = 4}, Give = nil}	-- Lv4 Analyze Guide	
Server.Life.Skill[2713] = {Skill = 341, AddLevel = 5, Level = 4, Prerequisite = {Skill = 232, Level = 5}, Give = nil}	-- Lv5 Analyze Guide	
Server.Life.Skill[2714] = {Skill = 341, AddLevel = 6, Level = 5, Prerequisite = {Skill = 232, Level = 6}, Give = nil}	-- Lv6 Analyze Guide	
Server.Life.Skill[2715] = {Skill = 341, AddLevel = 7, Level = 6, Prerequisite = {Skill = 232, Level = 7}, Give = nil}	-- Lv7 Analyze Guide	
Server.Life.Skill[2716] = {Skill = 341, AddLevel = 8, Level = 7, Prerequisite = {Skill = 232, Level = 8}, Give = nil}	-- Lv8 Analyze Guide	
Server.Life.Skill[2717] = {Skill = 341, AddLevel = 9, Level = 8, Prerequisite = {Skill = 232, Level = 9}, Give = nil}	-- Lv9 Analyze Guide	
Server.Life.Skill[2718] = {Skill = 341, AddLevel = 10, Level = 9, Prerequisite = {Skill = 232, Level = 10}, Give = nil}	-- Lv10 Analyze Guide
Server.Life.Skill[3225] = {Skill = 201, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Mining
Server.Life.Skill[3226] = {Skill = 200, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Woddcutting
Server.Life.Skill[3294] = {Skill = 231, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Fishing
Server.Life.Skill[3295] = {Skill = 232, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Salvage
Server.Life.Skill[3296] = {Skill = 241, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Set Stall
Server.Life.Skill[3299] = {Skill = 254, AddLevel = 1, Level = 0, Prerequisite = {Skill = nil, Level = nil}, Give = nil}	-- Repair
---------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------- ** Item Potions Vars ** -----------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Restore.Life[0263] = {Amount = {HP = 1000, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}        -- Dumpling
Server.Restore.Life[1847] = {Amount = {HP = 20, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 15}         -- Apple
Server.Restore.Life[1848] = {Amount = {HP = 80, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Bread
Server.Restore.Life[1849] = {Amount = {HP = 180, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Cake
Server.Restore.Life[3460] = {Amount = {HP = 180, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Cake
Server.Restore.Life[3116] = {Amount = {HP = 15, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Elven Fruit
Server.Restore.Life[3117] = {Amount = {HP = 20, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Red Date
Server.Restore.Life[3118] = {Amount = {HP = 35, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Mushroom
Server.Restore.Life[3119] = {Amount = {HP = 40, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Stramonium Fruit
Server.Restore.Life[3120] = {Amount = {HP = 50, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Ice Fruit
Server.Restore.Life[3121] = {Amount = {HP = 108, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Rainbow Fruit
Server.Restore.Life[3122] = {Amount = {HP = 250, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Elven Fruit Juice
Server.Restore.Life[3123] = {Amount = {HP = 300, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Red Date Tea
Server.Restore.Life[3124] = {Amount = {HP = 350, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Mushroom Soup
Server.Restore.Life[3125] = {Amount = {HP = 400, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Stramonium Fruit Juice
Server.Restore.Life[3126] = {Amount = {HP = 450, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Ice Cream
Server.Restore.Life[3127] = {Amount = {HP = 500, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Rainbow Fruit Juice
Server.Restore.Life[3128] = {Amount = {HP = 550, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Fruity Mix
Server.Restore.Life[3461] = {Amount = {HP = 550, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Fruity Mix
Server.Restore.Life[3129] = {Amount = {HP = 0, SP = 4}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}           -- Medicated Grass
Server.Restore.Life[3130] = {Amount = {HP = 0, SP = 8}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}           -- Fancy Petal
Server.Restore.Life[3131] = {Amount = {HP = 0, SP = 18}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Strange Fruit
Server.Restore.Life[3132] = {Amount = {HP = 0, SP = 22}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Snowy Grass Bud
Server.Restore.Life[3133] = {Amount = {HP = 0, SP = 33}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Liquorice Potion
Server.Restore.Life[3134] = {Amount = {HP = 0, SP = 38}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Energetic Tea
Server.Restore.Life[3135] = {Amount = {HP = 0, SP = 41}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Special Ointment
Server.Restore.Life[3136] = {Amount = {HP = 0, SP = 59}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Snowy Soft Bud
Server.Restore.Life[3137] = {Amount = {HP = 0, SP = 100}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Tiamari Fruit
Server.Restore.Life[3138] = {Amount = {HP = 0, SP = 111}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Mystery Fruit
Server.Restore.Life[3139] = {Amount = {HP = 0, SP = 141}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Agrypnotic
Server.Restore.Life[3140] = {Amount = {HP = 0, SP = 201}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Magical Potion
Server.Restore.Life[3848] = {Amount = {HP = 800, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Consitution Recovery Vial
Server.Restore.Life[3098] = {Amount = {HP = 1000, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}        -- Constitution Recovery Flask
Server.Restore.Life[3899] = {Amount = {HP = 30, SP = 10}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Christmas Lollipop
Server.Restore.Life[3910] = {Amount = {HP = 100, SP = 100}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}       -- Super Candy Stick
Server.Restore.Life[5626] = {Amount = {HP = 1500, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}        -- Super HP Potion
Server.Restore.Life[5627] = {Amount = {HP = 0, SP = 250}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Sweet Nectar
Server.Restore.Life[6203] = {Amount = {HP = 235, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Burger
Server.Restore.Life[6204] = {Amount = {HP = 280, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- Roasted Suckling Pig
Server.Restore.Life[6589] = {Amount = {HP = 20, SP = 0}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}          -- Normal Dumpling
Server.Restore.Life[3099] = {Amount = {HP = 0, SP = 150}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- SP Holy Water
Server.Restore.Life[6408] = {Amount = {HP = 0, SP = 150}, Percentage = {HP = 0, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}         -- SP Holy Water
Server.Restore.Life[3909] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0.3, SP = 0}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 8}    	   -- Gyoza
Server.Restore.Life[6590] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0.1, SP = 0.1}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}       -- Fresh Meat Dumpling
Server.Restore.Life[6591] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0.15, SP = 0.15}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}     -- Seafood Dumpling
Server.Restore.Life[6592] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0.25, SP = 0.25}, State = nil, StateLevel = nil, StateTime = nil, Cooldown = 1}     -- Mix Dumpling
Server.Restore.Life[6407] = {Amount = {HP = 200, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 1, StateTime = 900, Cooldown = 1}           -- Steam Bun
Server.Restore.Life[1078] = {Amount = {HP = 200, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 1, StateTime = 900, Cooldown = 1}           -- Steam Bun
Server.Restore.Life[1079] = {Amount = {HP = 400, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 2, StateTime = 600, Cooldown = 1}           -- Bun
Server.Restore.Life[1080] = {Amount = {HP = 600, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 3, StateTime = 420, Cooldown = 1}           -- Biscuit
Server.Restore.Life[1082] = {Amount = {HP = 800, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 4, StateTime = 300, Cooldown = 1}           -- Fried Dough
Server.Restore.Life[1083] = {Amount = {HP = 1000, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 166, StateLevel = 5, StateTime = 120, Cooldown = 1}          -- Spring Roll
Server.Restore.Life[1084] = {Amount = {HP = 0, SP = 100}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 1, StateTime = 900, Cooldown = 1}           -- Maiden Wine
Server.Restore.Life[1085] = {Amount = {HP = 0, SP = 200}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 2, StateTime = 600, Cooldown = 1}           -- Scholar Wine
Server.Restore.Life[1088] = {Amount = {HP = 0, SP = 300}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 3, StateTime = 420, Cooldown = 1}           -- Dukan Wine
Server.Restore.Life[1087] = {Amount = {HP = 0, SP = 400}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 4, StateTime = 300, Cooldown = 1}           -- Mao Wine
Server.Restore.Life[1089] = {Amount = {HP = 0, SP = 500}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 5, StateTime = 180, Cooldown = 1}           -- Ginseng Wine
Server.Restore.Life[1090] = {Amount = {HP = 0, SP = 600}, Percentage = {HP = 0, SP = 0}, State = 167, StateLevel = 6, StateTime = 120, Cooldown = 1}           -- Tiger Bone Tonic
Server.Restore.Life[4019] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 1, StateTime = 900, Cooldown = 1}             -- Codfish Steamboat
Server.Restore.Life[4020] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 2, StateTime = 600, Cooldown = 1}             -- Sturgeon Fish with Bamboo
Server.Restore.Life[4021] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 3, StateTime = 600, Cooldown = 1}             -- Savory Bubble Fish
Server.Restore.Life[4022] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 4, StateTime = 480, Cooldown = 1}             -- Sturgeon Soup
Server.Restore.Life[4023] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 5, StateTime = 480, Cooldown = 1}             -- Fried Oyster Soup
Server.Restore.Life[4024] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 6, StateTime = 360, Cooldown = 1}             -- Prawn Dumpling
Server.Restore.Life[4025] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 7, StateTime = 300, Cooldown = 1}             -- Tigerfish Bone Crisp
Server.Restore.Life[4026] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 8, StateTime = 180, Cooldown = 1}             -- Ratfish Rice
Server.Restore.Life[4027] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 9, StateTime = 180, Cooldown = 1}             -- China Clay
Server.Restore.Life[4028] = {Amount = {HP = 0, SP = 0}, Percentage = {HP = 0, SP = 0}, State = 165, StateLevel = 10, StateTime = 60, Cooldown = 1}         	   -- BBQ Shark Fin
---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------- ** Item Stat Reset Vials Vars ** -------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
Server.Restore.Attr[899] = {Name = "Strength", Amount = -1, Attribute = ATTR_BSTR}
Server.Restore.Attr[900] = {Name = "Consitution", Amount = -1, Attribute = ATTR_BCON}
Server.Restore.Attr[901] = {Name = "Agility", Amount = -1, Attribute = ATTR_BAGI}
Server.Restore.Attr[902] = {Name = "Accuracy", Amount = -1, Attribute = ATTR_BDEX}
Server.Restore.Attr[903] = {Name = "Spirit", Amount = -1, Attribute = ATTR_BSTA}
Server.Restore.Attr[3109] = {Name = "Strength", Amount = -1, Attribute = ATTR_BSTR}
Server.Restore.Attr[3110] = {Name = "Consitution", Amount = -1, Attribute = ATTR_BCON}
Server.Restore.Attr[3111] = {Name = "Agility", Amount = -1, Attribute = ATTR_BAGI}
Server.Restore.Attr[3112] = {Name = "Accuracy", Amount = -1, Attribute = ATTR_BDEX}
Server.Restore.Attr[3113] = {Name = "Spirit", Amount = -1, Attribute = ATTR_BSTA}
-------------------------------------------------------------------------------------------------------------------------
--------------------------------------- ** Modify Black Dragon Altar Variables ** ---------------------------------------
-------------------------------------------------------------------------------------------------------------------------
Server.AltarBD.MaxCurse = 8									-- Max curse an altar can have in order to open it.
Server.AltarBD.PowerPercentage = 0.5						-- Percentage of power failing and places a curse.
Server.AltarBD.Dice[1] = {Min = 1, Max = 4, Percentage = 20}
Server.AltarBD.Dice[2] = {Min = 5, Max = 8, Percentage = 40}
Server.AltarBD.Dice[3] = {Min = 9, Max = 12, Percentage = 60}
Server.AltarBD.Dice[4] = {Min = 13, Max = 16, Percentage = 80}
Server.AltarBD.Dice[5] = {Min = 17, Max = 100, Percentage = 100}
-------------------------------------------------------------------------------------------------------------------------
---------------------------------------------- ** Modify Gems Variables ** ----------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
GemVar = {}
GemVar[0] = {ID = 0, Level = 0, Type = 0, Effect = 0, Attribute = 0, Equip = 0}
GemVar[1] = {ID = 878, Level = 6, Type = 1, Effect = 4, Attribute = ITEMATTR_VAL_MNATK, Equip = {1,0}}
GemVar[2] = {ID = 879, Level = 6, Type = 1, Effect = 6, Attribute = ITEMATTR_VAL_MNATK, Equip = {2,3,4,7,9,0}}
GemVar[3] = {ID = 880, Level = 6, Type = 1, Effect = 10, Attribute = ITEMATTR_VAL_MNATK, Equip = {3,4,0}}
GemVar[4] = {ID = 881, Level = 6, Type = 1, Effect = 5, Attribute = ITEMATTR_VAL_HIT, Equip = {1,2,3,4,7,9,23,0}}
GemVar[5] = {ID = 882, Level = 6, Type = 2, Effect = 5, Attribute = ITEMATTR_VAL_DEF, Equip = {11,22,27,0}}
GemVar[6] = {ID = 883, Level = 6, Type = 2, Effect = 100, Attribute = ITEMATTR_VAL_MXHP, Equip = {11,22,27,0}}
GemVar[7] = {ID = 884, Level = 6, Type = 2, Effect = 2, Attribute = ITEMATTR_VAL_FLEE, Equip = {1,2,3,4,7,9,24,0}}
GemVar[8] = {ID = 887, Level = 6, Type = 4, Effect = 1, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,24,0}}
GemVar[9] = {ID = 860, Level = 3, Type = 4, Effect = 5, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[10] = {ID = 861, Level = 3, Type = 4, Effect = 5, Attribute = ITEMATTR_VAL_DEX, Equip = {23,0}}
GemVar[11] = {ID = 862, Level = 3, Type = 4, Effect = 5, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[12] = {ID = 863, Level = 3, Type = 4, Effect = 5, Attribute = ITEMATTR_VAL_STR, Equip = {1,2,3,4,7,9,0}}
GemVar[13] = {ID = 864, Level = 2, Type = 1, Effect = 50, Attribute = ITEMATTR_VAL_MNATK, Equip = {1,2,3,4,7,9,0}}
GemVar[14] = {ID = 865, Level = 2, Type = 2, Effect = 3, Attribute = ITEMATTR_VAL_PDEF, Equip = {11,22,27,0}}
GemVar[15] = {ID = 866, Level = 2, Type = 2, Effect = 500, Attribute = ITEMATTR_VAL_MXHP, Equip = {23,24,0}}
GemVar[16] = {ID = 1012, Level = 3, Type = 4, Effect = 5, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,0}}
GemVar[17] = {ID = 6832, Level = 1, Type = 2, Effect = 10, Attribute = ITEMATTR_VAL_DEF, Equip = {20,0}}
GemVar[18] = {ID = 6833, Level = 1, Type = 1, Effect = 200, Attribute = ITEMATTR_VAL_MXHP, Equip = {20,0}}
GemVar[19] = {ID = 6834, Level = 1, Type = 2, Effect = 200, Attribute = ITEMATTR_VAL_MXSP, Equip = {20,0}}
GemVar[20] = {ID = 6836, Level = 1, Type = 1, Effect = 10, Attribute = ITEMATTR_VAL_CRT, Equip = {20,0}}
GemVar[21] = {ID = 6837, Level = 1, Type = 1, Effect = 10, Attribute = ITEMATTR_VAL_HIT, Equip = {23,0}}
GemVar[22] = {ID = 6838, Level = 1, Type = 2, Effect = 10, Attribute = ITEMATTR_VAL_FLEE, Equip = {24,0}}
GemVar[23] = {ID = 6839, Level = 1, Type = 2, Effect = 15, Attribute = ITEMATTR_VAL_DEF, Equip = {22, 27, 0}}
GemVar[24] = {ID = 6840, Level = 1, Type = 4, Effect = 300, Attribute = ITEMATTR_VAL_MXHP, Equip = {22,11,27,0}}
GemVar[25] = {ID = 5845, Level = 1, Type = 1, Effect = 8, Attribute = ITEMATTR_VAL_STR, Equip = {1,2,3,4,7,9,0}}
GemVar[26] = {ID = 5846, Level = 1, Type = 2, Effect = 8, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[27] = {ID = 5847, Level = 1, Type = 4, Effect = 8, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,22,27,0}}
GemVar[28] = {ID = 5848, Level = 1, Type = 1, Effect = 8, Attribute = ITEMATTR_VAL_DEX, Equip = {1,2,3,4,7,9,23,0}}
GemVar[29] = {ID = 5849, Level = 1, Type = 4, Effect = 8, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[30] = {ID = 5878, Level = 1, Type = 1, Effect = 1, Attribute = ITEMATTR_VAL_PDEF, Equip = {1,2,3,4,7,9,0}}
GemVar[31] = {ID = 5879, Level = 1, Type = 2, Effect = 1, Attribute = ITEMATTR_VAL_MXHP, Equip = {11,22,27,0}}
GemVar[32] = {ID = 5880, Level = 1, Type = 2, Effect = 1, Attribute = ITEMATTR_VAL_STA, Equip = {23,24,0}}
GemVar[33] = {ID = 6718, Level = 1, Type = 2, Effect = 1, Attribute = ITEMATTR_VAL_CON, Equip = {22,27,0}}
GemVar[34] = {ID = 6817, Level = 4, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[35] = {ID = 6818, Level = 4, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[36] = {ID = 6819, Level = 4, Type = 4, Effect = 4, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[37] = {ID = 6820, Level = 4, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_DEX, Equip = {3,4,23,0}}
GemVar[38] = {ID = 6821, Level = 4, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_DEX, Equip = {23,0}}
GemVar[39] = {ID = 6822, Level = 4, Type = 4, Effect = 4, Attribute = ITEMATTR_VAL_DEX, Equip = {23,0}}
GemVar[40] = {ID = 6823, Level = 4, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[41] = {ID = 6824, Level = 4, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[42] = {ID = 6825, Level = 4, Type = 4, Effect = 4, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[43] = {ID = 6826, Level = 4, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_STR, Equip = {1,2,3,4,7,9,0}}
GemVar[44] = {ID = 6827, Level = 4, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_STR, Equip = {1,2,3,4,7,9,0}}
GemVar[45] = {ID = 6828, Level = 4, Type = 4, Effect = 4, Attribute = ITEMATTR_VAL_STR, Equip ={1,2,3,4,7,9,0}}
GemVar[46] = {ID = 6829, Level = 4, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,0}}
GemVar[47] = {ID = 6830, Level = 4, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,0}}
GemVar[48] = {ID = 6831, Level = 4, Type = 4, Effect = 4, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,0}}
GemVar[49] = {ID = 7108, Level = 1, Type = 1, Effect = 6, Attribute = ITEMATTR_VAL_STR, Equip = {1,2,3,4,7,9,0}}
GemVar[50] = {ID = 7109, Level = 1, Type = 4, Effect = 6, Attribute = ITEMATTR_VAL_STA, Equip = {1,2,3,4,7,9,0}}
GemVar[51] = {ID = 7110, Level = 1, Type = 1, Effect = 6, Attribute = ITEMATTR_VAL_DEX, Equip = {23,0}}
GemVar[52] = {ID = 7111, Level = 1, Type = 2, Effect = 6, Attribute = ITEMATTR_VAL_CON, Equip = {11,22,27,0}}
GemVar[53] = {ID = 7112, Level = 1, Type = 4, Effect = 6, Attribute = ITEMATTR_VAL_AGI, Equip = {24,0}}
GemVar[54] = {ID = 7288, Level = 1, Type = 1, Effect = 2, Attribute = ITEMATTR_VAL_STR, Equip = {23,0}}
GemVar[55] = {ID = 7289, Level = 1, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_STA, Equip = {23,0}}
GemVar[56] = {ID = 7290, Level = 1, Type = 1, Effect = 2, Attribute = ITEMATTR_VAL_DEX, Equip = {23,0}}
GemVar[57] = {ID = 7291, Level = 1, Type = 2, Effect = 2, Attribute = ITEMATTR_VAL_AGI, Equip = {23,0}}
GemVar[58] = {ID = 7292, Level = 1, Type = 4, Effect = 2, Attribute = ITEMATTR_VAL_CON, Equip = {23,0}}
GemVar[59] = {ID = 7558, Level = 1, Type = 2, Effect = 20, Attribute = ITEMATTR_VAL_DEF, Equip = {83, 0}}
GemVar[60] = {ID = 7559, Level = 1, Type = 2, Effect = 300, Attribute = ITEMATTR_VAL_MXHP, Equip = {82,0}}
GemVar[61] = {ID = 7560, Level = 1, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_STA, Equip = {81,0}}
GemVar[62] = {ID = 7561, Level = 1, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_DEX, Equip = {81,0}}
GemVar[63] = {ID = 7562, Level = 1, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_AGI, Equip = {81,0}}
GemVar[64] = {ID = 9175, Level = 1, Type = 4, Effect = 3, Attribute = ITEMATTR_VAL_DEX, Equip = {3,4,0}}
GemVar[65] = {ID = 9017, Level = 1, Type = 1, Effect = 20, Attribute = ITEMATTR_VAL_CRT, Equip = {1,2,3,4,5,7,9,0}}

-- Christmast Quest
XmasMonsterNum1 = 0
XmasMonsterNum2 = 0
XmasMonsterNum3 = 0
XmasMonsterNum4 = 0
XmasMonsterNum5 = 0

BBBB = {}			-- Following array of monsters
TestGuildLv = 0		-- Guild War
ReadyToFight = 0	-- Guild War
ATTR_RADIX = 1000	-- Properties Coefficient Ratio
JINISI_TIME = 2100	-- Genesis Quest

-- Server Rates
EXP_RAID = Server.Rates.Global.EXP
MF_RAID = Server.Rates.Global.DROP
SetGlobalRates(MF_RAID,EXP_RAID)
TEAM_EXP_RAID = Server.Rates.Global.TeamEXP
ELF_EXP_GET_RAID = Server.Rates.Global.FairyEXP
SHIP_EXP_RAID = Server.Rates.Global.ShipEXP
Resource_RAID_ADJUST = Server.Rates.Global.Resource

ChangeItemList = {}
ChangeItemList[1] = {2608, 100, 2682, 1}
ChangeItemList[2] = {2609, 100, 2683, 1}
ChangeItemList[3] = {2609, 1000, 2684, 1}
ChangeItemList[4] = {2608, 100, 2692, 1}
ChangeItemList[5] = {2609, 100, 2693, 1}
ChangeItemList[6] = {2609, 1000, 2694, 1}
ChangeItemList[7] = {2608, 100, 2702, 1}
ChangeItemList[8] = {2609, 100, 2703, 1}
ChangeItemList[9] = {2609, 1000, 2704, 1}
ChangeItemList[10] = {2608, 100, 2712, 1}
ChangeItemList[11] = {2609, 100, 2713, 1}
ChangeItemList[12] = {2609, 1000, 2714, 1}
ChangeItemList[13] = {3989, 99, 3999, 20}
ChangeItemList[14] = {3990, 99, 4000, 20}
ChangeItemList[15] = {3991, 99, 4001, 20}
ChangeItemList[16] = {3992, 99, 4002, 20}
ChangeItemList[17] = {3993, 99, 4003, 20}
ChangeItemList[18] = {3994, 99, 4004, 20}
ChangeItemList[19] = {3995, 99, 4005, 20}
ChangeItemList[20] = {3996, 99, 4006, 20}
ChangeItemList[21] = {3997, 99, 4007, 20}
ChangeItemList[22] = {3998, 99, 4008, 20}
ChangeItemList[23] = {4029, 99, 4039, 20}
ChangeItemList[24] = {4030, 99, 4040, 20}
ChangeItemList[25] = {4031, 99, 4041, 20}
ChangeItemList[26] = {4032, 99, 4042, 20}
ChangeItemList[27] = {4033, 99, 4043, 20}
ChangeItemList[28] = {4034, 99, 4044, 20}
ChangeItemList[29] = {4035, 99, 4045, 20}
ChangeItemList[30] = {4036, 99, 4046, 20}
ChangeItemList[31] = {4037, 99, 4047, 20}
ChangeItemList[32] = {4038, 99, 4048, 20}
ChangeItemList[33] = {1670, 99, 1671, 20}
ChangeItemList[34] = {1668, 99, 3368, 20}
ChangeItemList[35] = {1667, 99, 3360, 20}
ChangeItemList[36] = {1642, 99, 1643, 20}
ChangeItemList[37] = {4825, 99, 1638, 20}
ChangeItemList[38] = {1633, 99, 1641, 20}
ChangeItemList[39] = {1782, 99, 1769, 20}
ChangeItemList[40] = {2815, 99, 1775, 20}
ChangeItemList[41] = {1674, 99, 1767, 20}
ChangeItemList[42] = {4832, 99, 2901, 20}
ChangeItemList[43] = {855, 60, 2617, 1}
ChangeItemList[44] = {2588, 3, 2619, 1}
ChangeItemList[45] = {2588, 20, 2622, 1}
ChangeItemList[46] = {2589, 5, 2624, 1}
ChangeItemList[47] = {2588, 3, 2640, 1}
ChangeItemList[48] = {855, 60, 2641, 1}
ChangeItemList[49] = {2588, 20, 2642, 1}
ChangeItemList[50] = {2588, 20, 2643, 1}
ChangeItemList[51] = {2588, 50, 2644, 1}
ChangeItemList[52] = {2588, 50, 2649, 1}
ChangeItemList[53] = {855, 100, 1055, 1}
ChangeItemList[54] = {2588, 100, 1056, 1}
ChangeItemList[55] = {855, 100, 1058, 1}
ChangeItemList[56] = {2588, 100, 1059, 1}
ChangeItemList[57] = {855, 100, 1061, 1}
ChangeItemList[58] = {2588, 100, 1062, 1}
ChangeItemList[59] = {855, 100, 1064, 1}
ChangeItemList[60] = {2588, 100, 1065, 1}
ChangeItemList[61] = {855, 10, 2680, 1}
ChangeItemList[62] = {855, 50, 2681, 1}
ChangeItemList[63] = {855, 300, 2682, 1}
ChangeItemList[64] = {2588, 10, 2683, 1}
ChangeItemList[65] = {2588, 50, 2684, 1}
ChangeItemList[66] = {2588, 300, 2685, 1}
ChangeItemList[67] = {855, 10, 2690, 1}
ChangeItemList[68] = {855, 50, 2691, 1}
ChangeItemList[69] = {855, 300, 2692, 1}
ChangeItemList[70] = {2588, 10, 2693, 1}
ChangeItemList[71] = {2588, 50, 2694, 1}
ChangeItemList[72] = {2588, 300, 2695, 1}
ChangeItemList[73] = {855, 10, 2700, 1}
ChangeItemList[74] = {855, 50, 2701, 1}
ChangeItemList[75] = {855, 300, 2702, 1}
ChangeItemList[76] = {2588, 10, 2703, 1}
ChangeItemList[77] = {2588, 50, 2704, 1}
ChangeItemList[78] = {2588, 300, 2705, 1}
ChangeItemList[79] = {855, 10, 2710, 1}
ChangeItemList[80] = {855, 50, 2711, 1}
ChangeItemList[81] = {855, 300, 2712, 1}
ChangeItemList[82] = {2588, 10, 2713, 1}
ChangeItemList[83] = {2588, 50, 2714, 1}
ChangeItemList[84] = {2588, 300, 2715, 1}
ChangeItemList[85] = {4215, 5, 3655, 1}
ChangeItemList[86] = {4215, 20, 3656, 1}
ChangeItemList[87] = {4215, 50, 3657, 1}
ChangeItemList[88] = {4215, 5, 3658, 1}
ChangeItemList[89] = {4215, 20, 3659, 1}
ChangeItemList[90] = {4215, 30, 3660, 1}
ChangeItemList[91] = {4215, 100, 5781, 1}
ChangeItemList[92] = {3272, 100, 5782, 1}
ChangeItemList[93] = {4215, 100, 5784, 1}
ChangeItemList[94] = {3272, 100, 5785, 1}
ChangeItemList[95] = {4215, 100, 5787, 1}
ChangeItemList[96] = {3272, 100, 5788, 1}
ChangeItemList[97] = {4215, 100, 5790, 1}
ChangeItemList[98] = {3272, 100, 5791, 1}
ChangeItemList[99] = {4215, 10, 3915, 1}
ChangeItemList[100] = {4215, 50, 3916, 1}
ChangeItemList[101] = {4215, 300, 3917, 1}
ChangeItemList[102] = {3272, 10, 3918, 1}
ChangeItemList[103] = {3272, 50, 3919, 1}
ChangeItemList[104] = {3272, 300, 3920, 1}
ChangeItemList[105] = {4215, 10, 3925, 1}
ChangeItemList[106] = {4215, 50, 3926, 1}
ChangeItemList[107] = {4215, 300, 3927, 1}
ChangeItemList[108] = {3272, 10, 3928, 1}
ChangeItemList[109] = {3272, 50, 3929, 1}
ChangeItemList[110] = {3272, 300, 3930, 1}
ChangeItemList[111] = {4215, 10, 3935, 1}
ChangeItemList[112] = {4215, 50, 3936, 1}
ChangeItemList[113] = {4215, 300, 3937, 1}
ChangeItemList[114] = {3272, 10, 3938, 1}
ChangeItemList[115] = {3272, 50, 3939, 1}
ChangeItemList[116] = {3272, 300, 3940, 1}
ChangeItemList[117] = {4215, 10, 3935, 1}
ChangeItemList[118] = {4215, 50, 3936, 1}
ChangeItemList[119] = {4215, 300, 3937, 1}
ChangeItemList[120] = {3272, 10, 3938, 1}
ChangeItemList[121] = {3272, 50, 3939, 1}
ChangeItemList[122] = {3272, 300, 3940, 1}
ChangeItemList[123] = {1028, 2, 0766, 1}
ChangeItemList[124] = {1028, 2, 0769, 1}
ChangeItemList[125] = {1028, 2, 0773, 1}
ChangeItemList[126] = {1028, 2, 0776, 1}
ChangeItemList[127] = {1028, 2, 0780, 1}
ChangeItemList[128] = {1028, 2, 0784, 1}
ChangeItemList[129] = {1028, 2, 0788, 1}
ChangeItemList[130] = {1028, 2, 0792, 1}
ChangeItemList[131] = {1028, 2, 0795, 1}
ChangeItemList[132] = {1028, 2, 0798, 1}
ChangeItemList[133] = {1028, 2, 0802, 1}
ChangeItemList[134] = {1028, 2, 0806, 1}
ChangeItemList[135] = {3457, 1, 0765, 1}
ChangeItemList[136] = {3457, 1, 0768, 1}
ChangeItemList[137] = {3457, 1, 0772, 1}
ChangeItemList[138] = {3457, 1, 0775, 1}
ChangeItemList[139] = {3457, 1, 0779, 1}
ChangeItemList[140] = {3457, 1, 0783, 1}
ChangeItemList[141] = {3457, 1, 0787, 1}
ChangeItemList[142] = {3457, 1, 0791, 1}
ChangeItemList[143] = {3457, 1, 0794, 1}
ChangeItemList[144] = {3457, 1, 0797, 1}
ChangeItemList[145] = {3457, 1, 0801, 1}
ChangeItemList[146] = {3457, 1, 0805, 1}
ChangeItemList[147] = {3457, 1, 0807, 1}
ChangeItemList[148] = {3457, 1, 0808, 1}
ChangeItemList[149] = {3457, 1, 0809, 1}
ChangeItemList[150] = {3457, 1, 0810, 1}
ChangeItemList[151] = {3457, 1, 0811, 1}
ChangeItemList[152] = {3457, 1, 0812, 1}
ChangeItemList[153] = {3457, 1, 0813, 1}
ChangeItemList[154] = {3457, 1, 0814, 1}
ChangeItemList[155] = {3457, 1, 0815, 1}
ChangeItemList[156] = {3457, 1, 0877, 1}
ChangeItemList[157] = {855, 1000, 764, 1}
ChangeItemList[158] = {855, 1000, 767, 1}
ChangeItemList[159] = {855, 1000, 771, 1}
ChangeItemList[160] = {855, 1000, 774, 1}
ChangeItemList[161] = {855, 1000, 778, 1}
ChangeItemList[162] = {855, 1000, 782, 1}
ChangeItemList[163] = {855, 1000, 786, 1}
ChangeItemList[164] = {855, 1000, 790, 1}
ChangeItemList[165] = {855, 1000, 793, 1}
ChangeItemList[166] = {855, 1000, 796, 1}
ChangeItemList[167] = {855, 1000, 800, 1}
ChangeItemList[168] = {855, 1000, 804, 1}
ChangeItemList[169] = {855, 1000, 764, 1}
ChangeItemList[170] = {855, 1000, 767, 1}
ChangeItemList[171] = {855, 1000, 771, 1}
ChangeItemList[172] = {855, 1000, 774, 1}
ChangeItemList[173] = {855, 1000, 778, 1}
ChangeItemList[174] = {855, 1000, 782, 1}
ChangeItemList[175] = {855, 1000, 786, 1}
ChangeItemList[176] = {855, 1000, 790, 1}
ChangeItemList[177] = {855, 1000, 793, 1}
ChangeItemList[178] = {855, 1000, 796, 1}
ChangeItemList[179] = {855, 1000, 800, 1}
ChangeItemList[180] = {855, 1000, 804, 1}
ChangeItemList[181] = {855, 1000, 764, 1}
ChangeItemList[182] = {855, 1000, 767, 1}
ChangeItemList[183] = {855, 1000, 771, 1}
ChangeItemList[184] = {855, 1000, 774, 1}
ChangeItemList[185] = {855, 1000, 778, 1}
ChangeItemList[186] = {855, 1000, 782, 1}
ChangeItemList[187] = {855, 1000, 786, 1}
ChangeItemList[188] = {855, 1000, 790, 1}
ChangeItemList[189] = {855, 1000, 793, 1}
ChangeItemList[190] = {855, 1000, 796, 1}
ChangeItemList[191] = {855, 1000, 800, 1}
ChangeItemList[192] = {855, 1000, 804, 1}
ChangeItemList[193] = {855, 600, 763, 1}
ChangeItemList[194] = {855, 600, 770, 1}
ChangeItemList[195] = {855, 600, 777, 1}
ChangeItemList[196] = {855, 600, 781, 1}
ChangeItemList[197] = {855, 600, 785, 1}
ChangeItemList[198] = {855, 600, 789, 1}
ChangeItemList[199] = {855, 600, 799, 1}
ChangeItemList[200] = {855, 600, 803, 1}
ChangeItemList[201] = {855, 600, 763, 1}
ChangeItemList[202] = {855, 600, 770, 1}
ChangeItemList[203] = {855, 600, 777, 1}
ChangeItemList[204] = {855, 600, 781, 1}
ChangeItemList[205] = {855, 600, 785, 1}
ChangeItemList[206] = {855, 600, 789, 1}
ChangeItemList[207] = {855, 600, 799, 1}
ChangeItemList[208] = {855, 600, 803, 1}
ChangeItemList[209] = {855, 600, 763, 1}
ChangeItemList[210] = {855, 600, 770, 1}
ChangeItemList[211] = {855, 600, 777, 1}
ChangeItemList[212] = {855, 600, 781, 1}
ChangeItemList[213] = {855, 600, 785, 1}
ChangeItemList[214] = {855, 600, 789, 1}
ChangeItemList[215] = {855, 600, 799, 1}
ChangeItemList[216] = {855, 600, 803, 1}
ChangeItemList[217] = {855, 600, 763, 1}
ChangeItemList[218] = {855, 600, 770, 1}
ChangeItemList[219] = {855, 600, 777, 1}
ChangeItemList[220] = {855, 600, 781, 1}
ChangeItemList[221] = {855, 600, 785, 1}
ChangeItemList[222] = {855, 600, 789, 1}
ChangeItemList[223] = {855, 600, 799, 1}
ChangeItemList[224] = {855, 600, 803, 1}
ChangeItemList[225] = {855, 600, 763, 1}
ChangeItemList[226] = {855, 600, 770, 1}
ChangeItemList[227] = {855, 600, 777, 1}
ChangeItemList[228] = {855, 600, 781, 1}
ChangeItemList[229] = {855, 600, 785, 1}
ChangeItemList[230] = {855, 600, 789, 1}
ChangeItemList[231] = {855, 600, 799, 1}
ChangeItemList[232] = {855, 600, 803, 1}
ChangeItemList[233] = {855, 600, 763, 1}
ChangeItemList[234] = {855, 600, 770, 1}
ChangeItemList[235] = {855, 600, 777, 1}
ChangeItemList[236] = {855, 600, 781, 1}
ChangeItemList[237] = {855, 600, 785, 1}
ChangeItemList[238] = {855, 600, 789, 1}
ChangeItemList[239] = {855, 600, 799, 1}
ChangeItemList[240] = {855, 600, 803, 1}
ChangeItemList[241] = {855, 600, 763, 1}
ChangeItemList[242] = {855, 600, 770, 1}
ChangeItemList[243] = {855, 600, 777, 1}
ChangeItemList[244] = {855, 600, 781, 1}
ChangeItemList[245] = {855, 600, 785, 1}
ChangeItemList[246] = {855, 600, 789, 1}
ChangeItemList[247] = {855, 600, 799, 1}
ChangeItemList[248] = {855, 600, 803, 1}

-- Types of Items that can be converted into apparels
Apparel_CanConvert_Num = 13
Apparel_CanConvert_ID = {}
Apparel_CanConvert_ID[1] = 5291		-- Sword
Apparel_CanConvert_ID[2] = 5292		-- Greatsword
Apparel_CanConvert_ID[3] = 5296		-- Bows
Apparel_CanConvert_ID[4] = 5293		-- Guns
Apparel_CanConvert_ID[7] = 5295		-- Daggers
Apparel_CanConvert_ID[9] = 5294		-- Staff
Apparel_CanConvert_ID[11] = 5297	-- Shields
Apparel_CanConvert_ID[20] = 5287	-- Hats
Apparel_CanConvert_ID[22] = 5288	-- Armor
Apparel_CanConvert_ID[23] = 5289	-- Gloves
Apparel_CanConvert_ID[24] = 5290	-- Shoes
Apparel_CanConvert_ID[27] = 5288	-- Tattoo

-- Types of Items that can be repaired
Item_CanRepair_Num = 17
Item_CanRepair_ID = {}
Item_CanRepair_ID[0] = 1 	-- Sword
Item_CanRepair_ID[1] = 2 	-- Greatsword
Item_CanRepair_ID[2] = 3 	-- Bows
Item_CanRepair_ID[3] = 4 	-- Guns
Item_CanRepair_ID[4] = 7 	-- Daggers
Item_CanRepair_ID[5] = 11 	-- Shields
Item_CanRepair_ID[6] = 20 	-- Helm
Item_CanRepair_ID[7] = 22 	-- Armor
Item_CanRepair_ID[8] = 23 	-- Gloves
Item_CanRepair_ID[9] = 24 	-- Shoes
Item_CanRepair_ID[10] = 27 	-- Tattoo
Item_CanRepair_ID[11] = 9 	-- Staff
Item_CanRepair_ID[12] = 25 	-- Necklace
Item_CanRepair_ID[13] = 26 	-- Ring
Item_CanRepair_ID[14] = 81 	-- Bracelet
Item_CanRepair_ID[15] = 82 	-- Belt
Item_CanRepair_ID[16] = 83 	-- Handguard
Item_CanRepair_ID[17] = 88 	-- Cloak

-- Forgiable Item Types
Item_CanJinglian_Num = 17
Item_CanJinglian_ID = {}
Item_CanJinglian_ID[0] = 1 --Sword
Item_CanJinglian_ID[1] = 2 --Greatsword
Item_CanJinglian_ID[2] = 3 --Bows
Item_CanJinglian_ID[3] = 4 --Guns
Item_CanJinglian_ID[4] = 7 --Daggers
Item_CanJinglian_ID[5] = 11 --Shields
Item_CanJinglian_ID[6] = 20 --Hats
Item_CanJinglian_ID[7] = 22 --Clothes
Item_CanJinglian_ID[8] = 23 --Gloves
Item_CanJinglian_ID[9] = 24 --Shoes
Item_CanJinglian_ID[10] = 27 --Tattoo
Item_CanJinglian_ID[11] = 9 --Staff
Item_CanJinglian_ID[12] = 25 --Necklace
Item_CanJinglian_ID[13] = 26 --Ring
Item_CanJinglian_ID[14] = 81 --Bracelet
Item_CanJinglian_ID[15] = 82 --Belt
Item_CanJinglian_ID[16] = 83 --Handguard
Item_CanJinglian_ID[17] = 88 --Cloak

StoneTpye_ID		= {}	StoneItemType	 	= {}				StoneEffType		= {}	StoneEff		= {}	StoneAttrType		= {}
StoneTpye_ID_Num	= 71	StoneItemType_Num	= 71				StoneEffType_Num	= 71	StoneEff_Num 	= 71	StoneAttrType_Num	= 71
StoneTpye_ID[0]	= 0			StoneItemType[0] = {0}					StoneEffType[0] = 0			StoneEff[0]	= 0			StoneAttrType[0] = 0                    -- Nothing
StoneTpye_ID[1]	= 0878		StoneItemType[1] = {1}					StoneEffType[1] = 1			StoneEff[1]	= 10		StoneAttrType[1] = ITEMATTR_VAL_MNATK   -- Fiery Gem
StoneTpye_ID[2]	= 0879		StoneItemType[2] = {2,3,7,9}			StoneEffType[2] = 1			StoneEff[2]	= 10		StoneAttrType[2] = ITEMATTR_VAL_MNATK   -- Furious Gem
StoneTpye_ID[3]	= 0880		StoneItemType[3] = {4}					StoneEffType[3] = 1			StoneEff[3]	= 10		StoneAttrType[3] = ITEMATTR_VAL_MNATK   -- Explosive Gem
StoneTpye_ID[4]	= 0881		StoneItemType[4] = {1,2,3,4,7,9,23}		StoneEffType[4] = 1			StoneEff[4]	= 5			StoneAttrType[4] = ITEMATTR_VAL_HIT	    -- Lustrious Gem
StoneTpye_ID[5]	= 0882		StoneItemType[5] = {11,22,27}			StoneEffType[5] = 2			StoneEff[5]	= 15		StoneAttrType[5] = ITEMATTR_VAL_DEF	    -- Glowing Gem
StoneTpye_ID[6]	= 0883		StoneItemType[6] = {11,22,27}			StoneEffType[6] = 2			StoneEff[6]	= 100		StoneAttrType[6] = ITEMATTR_VAL_MXHP    -- Shining Gem
StoneTpye_ID[7]	= 0884		StoneItemType[7] = {1,2,3,4,7,9,24}		StoneEffType[7] = 2			StoneEff[7]	= 5			StoneAttrType[7] = ITEMATTR_VAL_FLEE    -- Shadow Gem
StoneTpye_ID[8]	= 0887		StoneItemType[8] = {1,2,3,4,7,9,24}		StoneEffType[8] = 4			StoneEff[8]	= 2			StoneAttrType[8] = ITEMATTR_VAL_STA	    -- Spirit Gem
StoneTpye_ID[9]	= 0860		StoneItemType[9] = {24}       			StoneEffType[9] = 4			StoneEff[9]	= 5			StoneAttrType[9] = ITEMATTR_VAL_AGI	    -- Gem of the Wind
StoneTpye_ID[10] = 0861		StoneItemType[10] = {23}       			StoneEffType[10] = 4		StoneEff[10] = 5		StoneAttrType[10] = ITEMATTR_VAL_DEX    -- Gem of Striking
StoneTpye_ID[11] = 0862		StoneItemType[11] = {11,22,27} 			StoneEffType[11] = 4		StoneEff[11] = 5		StoneAttrType[11] = ITEMATTR_VAL_CON    -- Gem of Colossus
StoneTpye_ID[12] = 0863		StoneItemType[12] = {1,2,3,4,7,9}		StoneEffType[12] = 4		StoneEff[12] = 5		StoneAttrType[12] = ITEMATTR_VAL_STR    -- Gem of Rage
StoneTpye_ID[13] = 0864		StoneItemType[13] = {1,2,3,4,7,9}		StoneEffType[13] = 1		StoneEff[13] = 50		StoneAttrType[13] = ITEMATTR_VAL_MNATK	-- Eye of Black Dragon
StoneTpye_ID[14] = 0865		StoneItemType[14] = {11,22,27}			StoneEffType[14] = 2		StoneEff[14] = 3		StoneAttrType[14] = ITEMATTR_VAL_PDEF	-- Soul of Black Dragon
StoneTpye_ID[15] = 0866		StoneItemType[15] = {23,24}				StoneEffType[15] = 2		StoneEff[15] = 500		StoneAttrType[15] = ITEMATTR_VAL_MXHP	-- Heart of Black Dragon
StoneTpye_ID[16] = 1012		StoneItemType[16] = {1,2,3,4,7,9}		StoneEffType[16] = 4		StoneEff[16] = 5		StoneAttrType[16] = ITEMATTR_VAL_STA	-- Gem of Soul
StoneTpye_ID[17] = 5750		StoneItemType[17] = {20}				StoneEffType[17] = 2		StoneEff[17] = 10		StoneAttrType[17] = ITEMATTR_VAL_DEF	-- Yellow Jade
StoneTpye_ID[18] = 5751		StoneItemType[18] = {20}				StoneEffType[18] = 2		StoneEff[18] = 200		StoneAttrType[18] = ITEMATTR_VAL_MXHP	-- Red Jade
StoneTpye_ID[19] = 5752		StoneItemType[19] = {20}				StoneEffType[19] = 2		StoneEff[19] = 200		StoneAttrType[19] = ITEMATTR_VAL_MXSP	-- Green Jade
StoneTpye_ID[20] = 5771		StoneItemType[20] = {20}				StoneEffType[20] = 1		StoneEff[20] = 10		StoneAttrType[20] = ITEMATTR_VAL_CRT	-- Chaitans Aura
StoneTpye_ID[21] = 5772		StoneItemType[21] = {23}				StoneEffType[21] = 1		StoneEff[21] = 10		StoneAttrType[21] = ITEMATTR_VAL_HIT	-- Locks Hit
StoneTpye_ID[22] = 5773		StoneItemType[22] = {24}				StoneEffType[22] = 2		StoneEff[22] = 10		StoneAttrType[22] = ITEMATTR_VAL_FLEE	-- Bings Dodging
StoneTpye_ID[23] = 5774		StoneItemType[23] = {22,27}				StoneEffType[23] = 2		StoneEff[23] = 15		StoneAttrType[23] = ITEMATTR_VAL_DEF	-- Fengs Defence
StoneTpye_ID[24] = 5775		StoneItemType[24] = {11,22,27}			StoneEffType[24] = 4		StoneEff[24] = 300		StoneAttrType[24] = ITEMATTR_VAL_MXHP	-- Sharks Strengthening
StoneTpye_ID[25] = 5845		StoneItemType[25] = {1,2,3,4,7,9}		StoneEffType[25] = 4		StoneEff[25] = 8		StoneAttrType[25] = ITEMATTR_VAL_STR	-- Azreals Glare
StoneTpye_ID[26] = 5846		StoneItemType[26] = {22,27} 			StoneEffType[26] = 4		StoneEff[26] = 8		StoneAttrType[26] = ITEMATTR_VAL_CON	-- Undead Azreal
StoneTpye_ID[27] = 5847		StoneItemType[27] = {1,2,3,4,7,9,22}	StoneEffType[27] = 4		StoneEff[27] = 8		StoneAttrType[27] = ITEMATTR_VAL_STA	-- Azreals Light
StoneTpye_ID[28] = 5848		StoneItemType[28] = {1,2,3,4,7,9,23}	StoneEffType[28] = 4		StoneEff[28] = 8		StoneAttrType[28] = ITEMATTR_VAL_DEX	-- Azreals Aggregation
StoneTpye_ID[29] = 5849		StoneItemType[29] = {24} 				StoneEffType[29] = 4		StoneEff[29] = 8		StoneAttrType[29] = ITEMATTR_VAL_AGI	-- Azreals Dance
StoneTpye_ID[30] = 5878		StoneItemType[30] = {0}       			StoneEffType[30] = 0		StoneEff[30] = 0		StoneAttrType[30] = 0					-- Apollo's spirit
StoneTpye_ID[31] = 5879		StoneItemType[31] = {0}       			StoneEffType[31] = 0		StoneEff[31] = 0		StoneAttrType[31] = 0					-- Cupid's spirit
StoneTpye_ID[32] = 5880		StoneItemType[32] = {0}       			StoneEffType[32] = 0		StoneEff[32] = 0		StoneAttrType[32] = 0					-- Athene's spirit
StoneTpye_ID[33] = 6718		StoneItemType[33] = {0}       			StoneEffType[33] = 0		StoneEff[33] = 0		StoneAttrType[33] = 0					-- Hestia's spirit
StoneTpye_ID[34] = 6817		StoneItemType[34] = {24,26}       		StoneEffType[34] = 4		StoneEff[30] = 2		StoneAttrType[34] = ITEMATTR_VAL_AGI	-- Broken Gem of The Wind
StoneTpye_ID[35] = 6818		StoneItemType[35] = {24}       			StoneEffType[35] = 4		StoneEff[31] = 3		StoneAttrType[35] = ITEMATTR_VAL_AGI	-- Cracked Gem of The Wind
StoneTpye_ID[36] = 6819		StoneItemType[36] = {24}				StoneEffType[36] = 4		StoneEff[32] = 4		StoneAttrType[36] = ITEMATTR_VAL_AGI	-- Chipped Gem of The Wind
StoneTpye_ID[37] = 6820		StoneItemType[37] = {3,4,23,26}       	StoneEffType[37] = 4		StoneEff[33] = 2		StoneAttrType[37] = ITEMATTR_VAL_DEX	-- Broken Gem of Striking
StoneTpye_ID[38] = 6821		StoneItemType[38] = {23}       			StoneEffType[38] = 4		StoneEff[34] = 3		StoneAttrType[38] = ITEMATTR_VAL_DEX	-- Cracked Gem of Striking
StoneTpye_ID[39] = 6822		StoneItemType[39] = {23}       			StoneEffType[39] = 4		StoneEff[35] = 4		StoneAttrType[39] = ITEMATTR_VAL_DEX	-- Chipped Gem of Striking
StoneTpye_ID[40] = 6823		StoneItemType[40] = {11,22,26,27} 		StoneEffType[40] = 4		StoneEff[36] = 2		StoneAttrType[40] = ITEMATTR_VAL_CON	-- Broken Gem of Colossus
StoneTpye_ID[41] = 6824		StoneItemType[41] = {11,22,27} 			StoneEffType[41] = 4		StoneEff[37] = 3		StoneAttrType[41] = ITEMATTR_VAL_CON	-- Cracked Gem of Colossus
StoneTpye_ID[42] = 6825		StoneItemType[42] = {11,22,27} 			StoneEffType[42] = 4		StoneEff[38] = 4		StoneAttrType[42] = ITEMATTR_VAL_CON	-- Chipped Gem of Colossus
StoneTpye_ID[43] = 6826		StoneItemType[43] = {1,2,3,4,7,9,26}	StoneEffType[43] = 4		StoneEff[39] = 2		StoneAttrType[43] = ITEMATTR_VAL_STR    -- Broken Gem of Rage
StoneTpye_ID[44] = 6827		StoneItemType[44] = {1,2,3,4,7,9}		StoneEffType[44] = 4		StoneEff[40] = 3		StoneAttrType[44] = ITEMATTR_VAL_STR    -- Cracked Gem of Rage
StoneTpye_ID[45] = 6828		StoneItemType[45] = {1,2,3,4,7,9}		StoneEffType[45] = 4		StoneEff[41] = 4		StoneAttrType[45] = ITEMATTR_VAL_STR    -- Chipped Gem of Rage
StoneTpye_ID[46] = 6829		StoneItemType[46] = {1,2,3,4,7,9,26}	StoneEffType[46] = 4		StoneEff[42] = 2		StoneAttrType[46] = ITEMATTR_VAL_STA	-- Broken Gem of Soul
StoneTpye_ID[47] = 6830		StoneItemType[47] = {1,2,3,4,7,9}		StoneEffType[47] = 4		StoneEff[43] = 3		StoneAttrType[47] = ITEMATTR_VAL_STA	-- Cracked Gem of Soul
StoneTpye_ID[48] = 6831		StoneItemType[48] = {1,2,3,4,7,9}		StoneEffType[48] = 4		StoneEff[44] = 4		StoneAttrType[48] = ITEMATTR_VAL_STA    -- Chipped Gem of soul
StoneTpye_ID[49] = 7108		StoneItemType[49] = {1,2,3,4,7,9}		StoneEffType[49] = 4		StoneEff[49] = 6		StoneAttrType[49] = ITEMATTR_VAL_STR    -- Great Gem of Rage
StoneTpye_ID[50] = 7109		StoneItemType[50] = {1,2,3,4,7,9}		StoneEffType[50] = 4		StoneEff[50] = 6		StoneAttrType[50] = ITEMATTR_VAL_STA    -- Great Gem of Soul
StoneTpye_ID[51] = 7110		StoneItemType[51] = {23}				StoneEffType[51] = 4		StoneEff[51] = 6		StoneAttrType[51] = ITEMATTR_VAL_DEX    -- Great Gem of Striking
StoneTpye_ID[52] = 7111		StoneItemType[52] = {11,22,27}			StoneEffType[52] = 4		StoneEff[52] = 6		StoneAttrType[52] = ITEMATTR_VAL_CON    -- Great Gem of Colossus
StoneTpye_ID[53] = 7112		StoneItemType[53] = {24}				StoneEffType[53] = 4		StoneEff[53] = 6		StoneAttrType[53] = ITEMATTR_VAL_AGI    -- Great Gem of Wind
StoneTpye_ID[54] = 7288		StoneItemType[54] = {23}				StoneEffType[54] = 4		StoneEff[54] = 2		StoneAttrType[54] = ITEMATTR_VAL_STR    -- Golden Rock
StoneTpye_ID[55] = 7289		StoneItemType[55] = {23}				StoneEffType[55] = 4		StoneEff[55] = 2		StoneAttrType[55] = ITEMATTR_VAL_STA    -- Lumber Rock
StoneTpye_ID[56] = 7290		StoneItemType[56] = {23}				StoneEffType[56] = 4		StoneEff[56] = 2		StoneAttrType[56] = ITEMATTR_VAL_DEX    -- Water Rock
StoneTpye_ID[57] = 7291		StoneItemType[57] = {23}				StoneEffType[57] = 4		StoneEff[57] = 2		StoneAttrType[57] = ITEMATTR_VAL_AGI    -- Fire Rock
StoneTpye_ID[58] = 7292		StoneItemType[58] = {23}				StoneEffType[58] = 4		StoneEff[58] = 2		StoneAttrType[58] = ITEMATTR_VAL_CON	-- Earth Rock
StoneTpye_ID[59] = 7558		StoneItemType[59] = {83}				StoneEffType[59] = 2		StoneEff[59] = 20		StoneAttrType[59] = ITEMATTR_VAL_DEF	-- Anchor of Satan
StoneTpye_ID[60] = 7559		StoneItemType[60] = {82}				StoneEffType[60] = 2		StoneEff[60] = 300		StoneAttrType[60] = ITEMATTR_VAL_MXHP	-- Might of Satan
StoneTpye_ID[61] = 7560		StoneItemType[61] = {81}				StoneEffType[61] = 4		StoneEff[61] = 3		StoneAttrType[61] = ITEMATTR_VAL_STA	-- Summoning of Satan
StoneTpye_ID[62] = 7561		StoneItemType[62] = {81}				StoneEffType[62] = 4		StoneEff[62] = 3		StoneAttrType[62] = ITEMATTR_VAL_DEX	-- Vengeance of Satan
StoneTpye_ID[63] = 7562		StoneItemType[63] = {81}				StoneEffType[63] = 4		StoneEff[63] = 3		StoneAttrType[63] = ITEMATTR_VAL_AGI	-- Stormsurge of Satan
StoneTpye_ID[64] = 9175		StoneItemType[64] = {3,4}				StoneEffType[64] = 4		StoneEff[64] = 3		StoneAttrType[64] = ITEMATTR_VAL_DEX	-- Lock's Power
StoneTpye_ID[65] = 9017		StoneItemType[65] = {1,2,3,4,7,9}		StoneEffType[65] = 1		StoneEff[65] = 20		StoneAttrType[65] = ITEMATTR_VAL_CRT	-- Fang of Demonic Dragon
StoneTpye_ID[66] = 9208		StoneItemType[66] = {88}				StoneEffType[66] = 1		StoneEff[66] = 5		StoneAttrType[66] = 0	    			-- Riven Soul Rune
StoneTpye_ID[67] = 9209		StoneItemType[67] = {88}				StoneEffType[67] = 1		StoneEff[67] = 50		StoneAttrType[67] = 0	    			-- Piercing Rune
StoneTpye_ID[68] = 9210		StoneItemType[68] = {88}				StoneEffType[68] = 1		StoneEff[68] = 10		StoneAttrType[68] = 0	    			-- Illusory Rune
StoneTpye_ID[69] = 9206		StoneItemType[69] = {88}				StoneEffType[69] = 4		StoneEff[69] = 5		StoneAttrType[69] = 0	    			-- Prayer Rune
StoneTpye_ID[70] = 9207		StoneItemType[70] = {88}				StoneEffType[70] = 4		StoneEff[70] = 5		StoneAttrType[70] = 0	    			-- Favor Rune
StoneTpye_ID[71] = 9211		StoneItemType[71] = {88}				StoneEffType[71] = 1		StoneEff[71] = 10		StoneAttrType[71] = 0	    			-- Curse Rune

-------------------------------------------------------------------
JNSTime_Flag_Num=13
JNSTime_Flag={}
JNSTime_Flag[1]				=0
JNSTime_Flag[2]				=31
JNSTime_Flag[3]				=59
JNSTime_Flag[4]				=90
JNSTime_Flag[5]				=120
JNSTime_Flag[6]				=151
JNSTime_Flag[7]				=181
JNSTime_Flag[8]				=212
JNSTime_Flag[9]				=243
JNSTime_Flag[10]			=273
JNSTime_Flag[11]			=304
JNSTime_Flag[12]			=334
JNSTime_Flag[13]			=365

DEXP_Num = Server.Level.Limit
DEXP = {}
DEXP[1] = 0
DEXP[2] = 5
DEXP[3] = 15
DEXP[4] = 35
DEXP[5] = 101
DEXP[6] = 250
DEXP[7] = 500
DEXP[8] = 1000
DEXP[9] = 1974
DEXP[10] = 3208
DEXP[11] = 4986
DEXP[12] = 7468
DEXP[13] = 10844
DEXP[14] = 15338
DEXP[15] = 21210
DEXP[16] = 28766
DEXP[17] = 38356
DEXP[18] = 50382
DEXP[19] = 65306
DEXP[20] = 83656
DEXP[21] = 106032
DEXP[22] = 133112
DEXP[23] = 165668
DEXP[24] = 204564
DEXP[25] = 250780
DEXP[26] = 305412
DEXP[27] = 369692
DEXP[28] = 444998
DEXP[29] = 532870
DEXP[30] = 635026
DEXP[31] = 753378
DEXP[32] = 890062
DEXP[33] = 1047438
DEXP[34] = 1228138
DEXP[35] = 1435074
DEXP[36] = 1671470
DEXP[37] = 1940892
DEXP[38] = 2247288
DEXP[39] = 2595010
DEXP[40] = 2988860
DEXP[41] = 3434132
DEXP[42] = 3936658
DEXP[43] = 4502856
DEXP[44] = 5139778
DEXP[45] = 5855180
DEXP[46] = 6657576
DEXP[47] = 7556310
DEXP[48] = 8561630
DEXP[49] = 9684764
DEXP[50] = 10938016
DEXP[51] = 12334856
DEXP[52] = 13890020
DEXP[53] = 15619622
DEXP[54] = 17541282
DEXP[55] = 19674240
DEXP[56] = 22039516
DEXP[57] = 24660044
DEXP[58] = 27560852
DEXP[59] = 30769230
DEXP[60] = 37746418
DEXP[61] = 45876427
DEXP[62] = 59571153
DEXP[63] = 75703638
DEXP[64] = 94615279
DEXP[65] = 116688304
DEXP[66] = 155291059
DEXP[67] = 186418013
DEXP[68] = 238159614
DEXP[69] = 298622278
DEXP[70] = 368975850
DEXP[71] = 450525549
DEXP[72] = 568409779
DEXP[73] = 679324744
DEXP[74] = 806544569
DEXP[75] = 952091724
DEXP[76] = 1188099236
DEXP[77] = 1480429211
DEXP[78] = 1776125584
DEXP[79] = 2091634902
DEXP[80] = 2425349810
DEXP[81] = 2440895086
DEXP[82] = 2458896515
DEXP[83] = 2479742169
DEXP[84] = 2503881436
DEXP[85] = 2531834707
DEXP[86] = 2564204594
DEXP[87] = 2601688923
DEXP[88] = 2645095775
DEXP[89] = 2695360909
DEXP[90] = 2753567934
DEXP[91] = 2820971668
DEXP[92] = 2899025191
DEXP[93] = 2989411170
DEXP[94] = 3094078133
DEXP[95] = 3215282476
DEXP[96] = 3355637105
DEXP[97] = 3518167765
DEXP[98] = 3706378269
DEXP[99] = 3924326032
DEXP[100] = 4176709541

do
    for i = 1, #DEXP, 1 do
        if i > Server.Level.Limit then
			DEXP[i] = nil
		end
    end
end

--------------------
STAR_ATTR_Num = 47
STAR_ATTR={}
STAR_ATTR[1]				= "Strength modulus bonus"
STAR_ATTR[2]				= "Agility modulus bonus"
STAR_ATTR[3]				= "Accuracy modulus bonus"
STAR_ATTR[4]				= "Stamina modulus bonus"
STAR_ATTR[5]				= "Spirit modulus bonus"
STAR_ATTR[6]				= "Luck modulus bonus"
STAR_ATTR[7]				= "Hit rate modulus bonus"
STAR_ATTR[8]				= "Attack range modulus bonus"
STAR_ATTR[9]				= "Min attack modulus bonus"
STAR_ATTR[10]				= "Max Attack modulus bonus"
STAR_ATTR[11]				= "Defense modulus bonus"
STAR_ATTR[12]				= "Max HP modulus bonus"
STAR_ATTR[13]				= "Max SP modulus bonus"
STAR_ATTR[14]				= "Dodge modulus bonus"
STAR_ATTR[15]				= "Hit Rate modulus bonus"
STAR_ATTR[16]				= "Berserk Rate modulus bonus"
STAR_ATTR[17]				= "Treasure drop rate modulus bonus"
STAR_ATTR[18]				= "HP Recovery Rate modulus bonus"
STAR_ATTR[19]				= "SP Recovery rate modulus bonus"
STAR_ATTR[20]				= "Movement speed modulus bonus"
STAR_ATTR[21]				= "Resource gathering rate modulus bonus"
STAR_ATTR[22]				= "Physical resist modulus bonus"
STAR_ATTR[23]				= "None"
STAR_ATTR[24]				= "None"
STAR_ATTR[25]				= "None"
STAR_ATTR[26]				= "Strength constant bonus"
STAR_ATTR[27]				= "Agility constant bonus"
STAR_ATTR[28]				= "Accuracy constant bonus"
STAR_ATTR[29]				= "Stamina constant bonus"
STAR_ATTR[30]				= "Spirit constant bonus"
STAR_ATTR[31]				= "Luck constant bonus"
STAR_ATTR[32]				= "Hit Rate constant bonus"
STAR_ATTR[33]				= "Attack range constant bonus"
STAR_ATTR[34]				= "Min Attack constant bonus"
STAR_ATTR[35]				= "Max Attack constant bonus"
STAR_ATTR[36]				= "Defense constant bonus"
STAR_ATTR[37]				= "Max HP constant bonus"
STAR_ATTR[38]				= "Max SP constant bonus"
STAR_ATTR[39]				= "Dodge constant bonus"
STAR_ATTR[40]				= "Hit Rate constant bonus"
STAR_ATTR[41]				= "Berserk rate constant bonus"
STAR_ATTR[42]				= "Treasure drop rate constant bonus"
STAR_ATTR[43]				= "HP Recovery Rate constant bonus"
STAR_ATTR[44]				= "SP Recovery Rate constant bonus"
STAR_ATTR[45]				= "Movement speed constant bonus"
STAR_ATTR[46]				= "Resource gathering rate constant bonus"
STAR_ATTR[47]				= "Physical Resist constant bonus"

N1=5
N2=5
N3=5
N4=5
N5=5

SI=183

SN=1

SA1=5
SA2=5
SA3=5
SA4=5
SA5=5

SE=15

RYZ_Rongyu_Min = -300
RYZ_Rongyu_Max = 30000

UnNormalMonster_Num = 13
UnNormalMonster_ID = {}
UnNormalMonster_ID[0] =	1
UnNormalMonster_ID[1] =	2
UnNormalMonster_ID[2] =	3
UnNormalMonster_ID[3] =	4
UnNormalMonster_ID[4] =	728
UnNormalMonster_ID[5] =	729
UnNormalMonster_ID[6] =	730
UnNormalMonster_ID[7] =	731
UnNormalMonster_ID[8] =	739
UnNormalMonster_ID[9] =	740
UnNormalMonster_ID[10] = 742
UnNormalMonster_ID[11] = 743
UnNormalMonster_ID[12] = 744
UnNormalMonster_ID[13] = 745

--PK-----------
PK_Win_CountNum		=	60

----------------------------------------------------------------------------------------------------
--PK
PK_BagItemDelCheckNum = 6
PK_BagItemDelCheck_ID = { }
PK_BagItemDelCheck_ID [0] = 1854
PK_BagItemDelCheck_ID [1] = 1855
PK_BagItemDelCheck_ID [2] = 1856
PK_BagItemDelCheck_ID [3] = 1857
PK_BagItemDelCheck_ID [4] = 1858
PK_BagItemDelCheck_ID [5] = 1859
PK_BagItemDelCheck_ID [6] = 1860
--PK_BagItemDelCheck_ID [7] = 4661



-------------
SZ_Win_CountNum		=	60
SZ_Win_CountNum2	=	60
GUILDNOTICE = 6
GUILDCLOSESHOW = {}
GUILDCLOSESHOW[1] = 900
GUILDCLOSESHOW[2] = 600
GUILDCLOSESHOW[3] = 300
GUILDCLOSESHOW[4] = 180
GUILDCLOSESHOW[5] = 120
GUILDCLOSESHOW[6] = 60
GUILDWARCLOSETIME = 10800

GUILDNOTICE2 = 6
GUILDCLOSESHOW2 = {}
GUILDCLOSESHOW2[1] = 900
GUILDCLOSESHOW2[2] = 600
GUILDCLOSESHOW2[3] = 300
GUILDCLOSESHOW2[4] = 180
GUILDCLOSESHOW2[5] = 120
GUILDCLOSESHOW2[6] = 60
GUILDWARCLOSETIME2 = 10800




--
SZ_BagItemDelCheckNum = 4
SZ_BagItemDelCheck_ID = { }
SZ_BagItemDelCheck_ID [0] = 4661
SZ_BagItemDelCheck_ID [1] = 2964
SZ_BagItemDelCheck_ID [2] = 3001
SZ_BagItemDelCheck_ID [3] = 2381 ---




----
SS_BagItemDelCheckNum = 2
SS_BagItemDelCheck_ID = { }
SS_BagItemDelCheck_ID [1] = 1855		--
SS_BagItemDelCheck_ID [2] = 1856		--

CRY = {}
CRY[5]=0
CRY[6]=0
CRY[7]=0
CRY[8]=0
CRY[9]=0
CRY[10]=0
CRY[11]=0
CRY[12]=0
CRY[13]=0
CRY[14]=0
CRY[15]=0
CRY[16]=0
CRY[17]=0
CRY[18]=0
CRY[19]=0

AZRAEL = {}
AZRAEL[5]=0
AZRAEL[6]=0
AZRAEL[7]=0
AZRAEL[8]=0
AZRAEL[9]=0
AZRAEL[10]=0
AZRAEL[11]=0
AZRAEL[12]=0
AZRAEL[13]=0
AZRAEL[14]=0
AZRAEL[15]=0
AZRAEL[16]=0
AZRAEL[17]=0
AZRAEL[18]=0
AZRAEL[19]=0

SUMMON = {}
SUMMON[1]=0
SUMMON[2]=0
SUMMON[3]=0
SUMMON[4]=0
SUMMON[5]=0

HELLCLOSETIME = 300
MAXNOTICE = 17
NOTICETIME = {}
NOTICETIME[1] = 300
NOTICETIME[2] = 240
NOTICETIME[3] = 180
NOTICETIME[4] = 120
NOTICETIME[5] = 60
NOTICETIME[6] = 30
NOTICETIME[7] = 15
NOTICETIME[8] = 10
NOTICETIME[9] = 9
NOTICETIME[10] = 8
NOTICETIME[11] = 7
NOTICETIME[12] = 6
NOTICETIME[13] = 5
NOTICETIME[14] = 4
NOTICETIME[15] = 3
NOTICETIME[16] = 2
NOTICETIME[17] = 1

--


--
NPC_SALE				=	0	--npc
MONSTER_BAOLIAO		=	1	--
PLAYER_HECHENG		=	2	--
QUEST_AWARD_1		=	3	--1
QUEST_AWARD_2		=	4	--2
QUEST_AWARD_3		=	5	--3
QUEST_AWARD_4		=	6	--4
QUEST_AWARD_5		=	7	--5
QUEST_AWARD_6		=	8	--6
QUEST_AWARD_7		=	9	--7
QUEST_AWARD_8		=	10	--8
PLAYER_XSBOX			=	11	--

PLAYER_CCFSBOXA		=	12
PLAYER_CCFSBOXB		=	13
PLAYER_CCFSBOXC		=	14
PLAYER_CCFSBOXD		=	15
PLAYER_CCFSBOXE		=	16
PLAYER_CCFSBOXF 		=	17
PLAYER_CCFSBOXG		=	18
PLAYER_CCFSBOXH		=	19
PLAYER_CCFSBOXI		=	20
PLAYER_ZSITEM			=	22	--
PLAYER_HSSR			=	23	--
PLAYER_HSSRA			=	24	--A

QUEST_AWARD_GODBOX	=	94	-- --------------kokora
QUEST_AWARD_SCBOX	=	95	--
QUEST_AWARD_SDJ		=	96	--
QUEST_AWARD_RYZ		=	97	--
QUEST_AWARD_WZX		=	98	--
QUEST_AWARD_RAND		=	99	--




--

ITEMSERIES_DRAGON		= 1							--
ITEMSERIES_TAITAN = 2							--
ITEMSERIES_HUNTER = 3							--
ITEMSERIES_DELIVER = 4							--
ITEMSERIES_HOLY = 5							--

--

Itemattr_Baoliao = {}





--

Item_Baoliao = { }									-- --
Item_Baoliao [0]		=		0						--
Item_Baoliao [1]		=		0						--
Item_Baoliao [2]		=		0						--
Item_Baoliao [3]		=		0						--
Item_Baoliao [4]		=		0						--
Item_Baoliao [5]		=		1						--...
Item_Baoliao [6]		=		5						--...
Item_Baoliao [7]		=		10						--...
Item_Baoliao [8]		=		40						--...
Item_Baoliao [9]		=		80						--...

Item_Attr_0 = { }
Item_Attr_0 [0]		=		0						--5
Item_Attr_0 [1]		=		0						--4
Item_Attr_0 [2]		=		1						--3
Item_Attr_0 [3]		=		4						--2
Item_Attr_0 [4]		=		50						--1


Item_Mission_1 = { }
Item_Mission_1 [0]		=		0						--
Item_Mission_1 [1]		=		0						--
Item_Mission_1 [2]		=		0						--
Item_Mission_1 [3]		=		0						--
Item_Mission_1 [4]		=		0						--
Item_Mission_1 [5]		=		0						--...
Item_Mission_1 [6]		=		0						--...
Item_Mission_1 [7]		=		1						--...
Item_Mission_1 [8]		=		10						--...
Item_Mission_1 [9]		=		50						--...

Item_Attr_1 = { }
Item_Attr_1 [0]		=		0						--5
Item_Attr_1 [1]		=		0						--4
Item_Attr_1 [2]		=		0						--3
Item_Attr_1 [3]		=		0						--2
Item_Attr_1 [4]		=		0						--1





Item_Mission_2 = { }
Item_Mission_2 [0]		=		0						--
Item_Mission_2 [1]		=		0						--
Item_Mission_2 [2]		=		0						--
Item_Mission_2 [3]		=		0						--
Item_Mission_2 [4]		=		0						--
Item_Mission_2 [5]		=		0						--...
Item_Mission_2 [6]		=		1						--...
Item_Mission_2 [7]		=		5						--...
Item_Mission_2 [8]		=		20						--...
Item_Mission_2 [9]		=		80						--...

Item_Attr_2 = { }
Item_Attr_2 [0]		=		0						--5
Item_Attr_2 [1]		=		0						--4
Item_Attr_2 [2]		=		0						--3
Item_Attr_2 [3]		=		10						--2
Item_Attr_2 [4]		=		30						--1



Item_Mission_3 = { }
Item_Mission_3 [0]		=		0						--
Item_Mission_3 [1]		=		0						--
Item_Mission_3 [2]		=		0						--
Item_Mission_3 [3]		=		0						--
Item_Mission_3 [4]		=		0						--
Item_Mission_3 [5]		=		0						--...
Item_Mission_3 [6]		=		1						--...
Item_Mission_3 [7]		=		5						--...
Item_Mission_3 [8]		=		50						--...
Item_Mission_3 [9]		=		100						--...

Item_Attr_3 = { }
Item_Attr_3 [0]		=		0						--5
Item_Attr_3 [1]		=		0						--4
Item_Attr_3 [2]		=		0						--3
Item_Attr_3 [3]		=		10						--2
Item_Attr_3 [4]		=		60						--1





Item_Mission_4 = { }
Item_Mission_4 [0]		=		0						--
Item_Mission_4 [1]		=		0						--
Item_Mission_4 [2]		=		0						--
Item_Mission_4 [3]		=		0						--
Item_Mission_4 [4]		=		0						--
Item_Mission_4 [5]		=		1						--...
Item_Mission_4 [6]		=		5						--...
Item_Mission_4 [7]		=		15						--...
Item_Mission_4 [8]		=		90						--...
Item_Mission_4 [9]		=		100						--...

Item_Attr_4 = { }
Item_Attr_4 [0]		=		0						--5
Item_Attr_4 [1]		=		0						--4
Item_Attr_4 [2]		=		1						--3
Item_Attr_4 [3]		=		20						--2
Item_Attr_4 [4]		=		100						--1


Item_Mission_5 = { }
Item_Mission_5 [0]		=		0						--
Item_Mission_5 [1]		=		0						--
Item_Mission_5 [2]		=		0						--
Item_Mission_5 [3]		=		0						--
Item_Mission_5 [4]		=		0						--
Item_Mission_5 [5]		=		1						--...
Item_Mission_5 [6]		=		15						--...
Item_Mission_5 [7]		=		100						--...
Item_Mission_5 [8]		=		100						--...
Item_Mission_5 [9]		=		100						--...

Item_Attr_5 = { }
Item_Attr_5 [0]		=		0						--5
Item_Attr_5 [1]		=		0						--4
Item_Attr_5 [2]		=		1						--3
Item_Attr_5 [3]		=		5						--2
Item_Attr_5 [4]		=		100						--1

Item_Mission_94 = { }					 -----------------kokora
Item_Mission_94 [0]		=		0						--
Item_Mission_94 [1]		=		0						--
Item_Mission_94 [2]		=		0						--
Item_Mission_94 [3]		=		0						--
Item_Mission_94 [4]		=		0						--
Item_Mission_94 [5]		=		0						--...
Item_Mission_94 [6]		=		1						--...
Item_Mission_94 [7]		=		100						--...
Item_Mission_94 [8]		=		100						--...
Item_Mission_94 [9]		=		100						--...

Item_Attr_94 = { }
Item_Attr_94 [0]		=		0						--5
Item_Attr_94 [1]		=		0						--4
Item_Attr_94 [2]		=		1						--3
Item_Attr_94 [3]		=		20						--2
Item_Attr_94 [4]		=		90						--1

Item_Mission_95 = { }
Item_Mission_95 [0]		=		0						--
Item_Mission_95 [1]		=		0						--
Item_Mission_95 [2]		=		0						--
Item_Mission_95 [3]		=		0						--
Item_Mission_95 [4]		=		0						--
Item_Mission_95 [5]		=		0						--...
Item_Mission_95 [6]		=		0						--...
Item_Mission_95 [7]		=		100						--...
Item_Mission_95 [8]		=		100						--...
Item_Mission_95 [9]		=		100						--...

Item_Attr_95 = { }
Item_Attr_95 [0]		=		0						--5
Item_Attr_95 [1]		=		0						--4
Item_Attr_95 [2]		=		1						--3
Item_Attr_95 [3]		=		4						--2
Item_Attr_95 [4]		=		50						--1

Item_Mission_96 = { }
Item_Mission_96 [0]		=		0						--
Item_Mission_96 [1]		=		0						--
Item_Mission_96 [2]		=		0						--
Item_Mission_96 [3]		=		0						--
Item_Mission_96 [4]		=		0						--
Item_Mission_96 [5]		=		10						--...
Item_Mission_96 [6]		=		20						--...
Item_Mission_96 [7]		=		50						--...
Item_Mission_96 [8]		=		90						--...
Item_Mission_96 [9]		=		100						--...

Item_Attr_96 = { }
Item_Attr_96 [0]		=		0						--5
Item_Attr_96 [1]		=		0						--4
Item_Attr_96 [2]		=		2						--3
Item_Attr_96 [3]		=		20						--2
Item_Attr_96 [4]		=		100						--1


Item_Mission_97 = { }
Item_Mission_97 [0]		=		0						--
Item_Mission_97 [1]		=		0						--
Item_Mission_97 [2]		=		0						--
Item_Mission_97 [3]		=		0						--
Item_Mission_97 [4]		=		0						--
Item_Mission_97 [5]		=		0						--...
Item_Mission_97 [6]		=		0						--...
Item_Mission_97 [7]		=		0						--...
Item_Mission_97 [8]		=		0						--...
Item_Mission_97 [9]		=		100						--...

Item_Attr_97 = { }
Item_Attr_97 [0]		=		100						--5
Item_Attr_97 [1]		=		100						--4
Item_Attr_97 [2]		=		100						--3
Item_Attr_97 [3]		=		100						--2
Item_Attr_97 [4]		=		100						--1


Item_Mission_98 = { }
Item_Mission_98 [0]		=		0						--
Item_Mission_98 [1]		=		0						--
Item_Mission_98 [2]		=		0						--
Item_Mission_98 [3]		=		0						--
Item_Mission_98 [4]		=		0						--
Item_Mission_98 [5]		=		10						--...
Item_Mission_98 [6]		=		20						--...
Item_Mission_98 [7]		=		50						--...
Item_Mission_98 [8]		=		90						--...
Item_Mission_98 [9]		=		100						--...

Item_Attr_98 = { }
Item_Attr_98 [0]		=		0						--5
Item_Attr_98 [1]		=		0						--4
Item_Attr_98 [2]		=		2						--3
Item_Attr_98 [3]		=		20						--2
Item_Attr_98 [4]		=		100						--1



Item_Mission_99 = { }
Item_Mission_99 [0]		=		0						--
Item_Mission_99 [1]		=		0						--
Item_Mission_99 [2]		=		0						--
Item_Mission_99 [3]		=		0						--
Item_Mission_99 [4]		=		0						--
Item_Mission_99 [5]		=		1						--...
Item_Mission_99 [6]		=		5						--...
Item_Mission_99 [7]		=		20						--...
Item_Mission_99 [8]		=		50						--...
Item_Mission_99 [9]		=		100						--...

Item_Attr_99 = { }
Item_Attr_99 [0]		=		0						--5
Item_Attr_99 [1]		=		0						--4
Item_Attr_99 [2]		=		2						--3
Item_Attr_99 [3]		=		20						--2
Item_Attr_99 [4]		=		100						--1

Item_Mission_11 = { }
Item_Mission_11 [0]		=		0						--
Item_Mission_11 [1]		=		0						--
Item_Mission_11 [2]		=		0						--
Item_Mission_11 [3]		=		0						--
Item_Mission_11 [4]		=		0						--
Item_Mission_11 [5]		=		0						--...
Item_Mission_11 [6]		=		0						--...
Item_Mission_11 [7]		=		100						--...
Item_Mission_11 [8]		=		100						--...
Item_Mission_11 [9]		=		100						--...

Item_Attr_11 = { }
Item_Attr_11 [0]		=		0						--5
Item_Attr_11 [1]		=		0						--4
Item_Attr_11 [2]		=		1						--3
Item_Attr_11 [3]		=		4						--2
Item_Attr_11 [4]		=		50						--1



Item_Mission_12 = { }
Item_Mission_12 [0]		=		0						--
Item_Mission_12 [1]		=		0						--
Item_Mission_12 [2]		=		0						--
Item_Mission_12 [3]		=		0						--
Item_Mission_12 [4]		=		0						--
Item_Mission_12 [5]		=		0						--...
Item_Mission_12 [6]		=		0						--...
Item_Mission_12 [7]		=		0						--...
Item_Mission_12 [8]		=		0						--...
Item_Mission_12 [9]		=		100						--...

Item_Attr_12 = { }
Item_Attr_12 [0]		=		0						--5
Item_Attr_12 [1]		=		0						--4
Item_Attr_12 [2]		=		0						--3
Item_Attr_12 [3]		=		0						--2
Item_Attr_12 [4]		=		0						--1




Item_Mission_13 = { }
Item_Mission_13 [0]		=		0						--
Item_Mission_13 [1]		=		0						--
Item_Mission_13 [2]		=		0						--
Item_Mission_13 [3]		=		0						--
Item_Mission_13 [4]		=		0						--
Item_Mission_13 [5]		=		0						--...
Item_Mission_13 [6]		=		0						--...
Item_Mission_13 [7]		=		0						--...
Item_Mission_13 [8]		=		100						--...
Item_Mission_13 [9]		=		100						--...

Item_Attr_13 = { }
Item_Attr_13 [0]		=		0						--5
Item_Attr_13 [1]		=		0						--4
Item_Attr_13 [2]		=		0						--3
Item_Attr_13 [3]		=		0						--2
Item_Attr_13 [4]		=		0						--1


Item_Mission_14 = { }
Item_Mission_14 [0]		=		0						--
Item_Mission_14 [1]		=		0						--
Item_Mission_14 [2]		=		0						--
Item_Mission_14 [3]		=		0						--
Item_Mission_14 [4]		=		0						--
Item_Mission_14 [5]		=		0						--...
Item_Mission_14 [6]		=		0						--...
Item_Mission_14 [7]		=		100						--...
Item_Mission_14 [8]		=		100						--...
Item_Mission_14 [9]		=		100						--...

Item_Attr_14 = { }
Item_Attr_14 [0]		=		0						--5
Item_Attr_14 [1]		=		0						--4
Item_Attr_14 [2]		=		0							--3
Item_Attr_14 [3]		=		0							--2
Item_Attr_14 [4]		=		0						--1


Item_Mission_15 = { }
Item_Mission_15 [0]		=		0						--
Item_Mission_15 [1]		=		0						--
Item_Mission_15 [2]		=		0						--
Item_Mission_15 [3]		=		0						--
Item_Mission_15 [4]		=		0						--
Item_Mission_15 [5]		=		0						--...
Item_Mission_15 [6]		=		100						--...
Item_Mission_15 [7]		=		100						--...
Item_Mission_15 [8]		=		100						--...
Item_Mission_15 [9]		=		100						--...

Item_Attr_15 = { }
Item_Attr_15 [0]		=		0						--5
Item_Attr_15 [1]		=		0						--4
Item_Attr_15 [2]		=		0						--3
Item_Attr_15 [3]		=		0						--2
Item_Attr_15 [4]		=		0						--1


Item_Mission_16 = { }
Item_Mission_16 [0]		=		0						--
Item_Mission_16 [1]		=		0						--
Item_Mission_16 [2]		=		0						--
Item_Mission_16 [3]		=		0						--
Item_Mission_16 [4]		=		0						--
Item_Mission_16 [5]		=		100						--...
Item_Mission_16 [6]		=		100						--...
Item_Mission_16 [7]		=		100						--...
Item_Mission_16 [8]		=		100						--...
Item_Mission_16 [9]		=		100						--...

Item_Attr_16 = { }
Item_Attr_16 [0]		=		0					--5
Item_Attr_16 [1]		=		0					--4
Item_Attr_16 [2]		=		0					--3
Item_Attr_16 [3]		=		0					--2
Item_Attr_16 [4]		=		0						--1

Item_Mission_17 = { }
Item_Mission_17 [0]		=		0						--
Item_Mission_17 [1]		=		0						--
Item_Mission_17 [2]		=		0						--
Item_Mission_17 [3]		=		0						--
Item_Mission_17 [4]		=		100						--
Item_Mission_17 [5]		=		100						--...
Item_Mission_17 [6]		=		100						--...
Item_Mission_17 [7]		=		100						--...
Item_Mission_17 [8]		=		100						--...
Item_Mission_17 [9]		=		100						--...

Item_Attr_17 = { }
Item_Attr_17 [0]		=		0					--5
Item_Attr_17 [1]		=		0					--4
Item_Attr_17 [2]		=		0					--3
Item_Attr_17 [3]		=		0					--2
Item_Attr_17 [4]		=		0						--1


Item_Mission_18 = { }
Item_Mission_18 [0]		=		0						--
Item_Mission_18 [1]		=		0						--
Item_Mission_18 [2]		=		0						--
Item_Mission_18 [3]		=		100						--
Item_Mission_18 [4]		=		100						--
Item_Mission_18 [5]		=		100						--...
Item_Mission_18 [6]		=		100						--...
Item_Mission_18 [7]		=		100						--...
Item_Mission_18 [8]		=		100						--...
Item_Mission_18 [9]		=		100						--...

Item_Attr_18 = { }
Item_Attr_18 [0]		=		0					--5
Item_Attr_18 [1]		=		0					--4
Item_Attr_18 [2]		=		0					--3
Item_Attr_18 [3]		=		0					--2
Item_Attr_18 [4]		=		0						--1

Item_Mission_19 = { }
Item_Mission_19 [0]		=		0						--
Item_Mission_19 [1]		=		0						--
Item_Mission_19 [2]		=		100						--
Item_Mission_19 [3]		=		100						--
Item_Mission_19 [4]		=		100						--
Item_Mission_19 [5]		=		100						--...
Item_Mission_19 [6]		=		100						--...
Item_Mission_19 [7]		=		100						--...
Item_Mission_19 [8]		=		100						--...
Item_Mission_19 [9]		=		100						--...

Item_Attr_19 = { }
Item_Attr_19 [0]		=		0					--5
Item_Attr_19 [1]		=		0					--4
Item_Attr_19 [2]		=		0					--3
Item_Attr_19 [3]		=		0					--2
Item_Attr_19 [4]		=		0						--1


Item_Mission_20 = { }
Item_Mission_20 [0]		=		0						--
Item_Mission_20 [1]		=		100						--
Item_Mission_20 [2]		=		100						--
Item_Mission_20 [3]		=		100						--
Item_Mission_20 [4]		=		100						--
Item_Mission_20 [5]		=		100						--...
Item_Mission_20 [6]		=		100						--...
Item_Mission_20 [7]		=		100						--...
Item_Mission_20 [8]		=		100						--...
Item_Mission_20 [9]		=		100						--...

Item_Attr_20 = { }
Item_Attr_20 [0]		=		0						--5
Item_Attr_20 [1]		=		0						--4
Item_Attr_20 [2]		=		0						--3
Item_Attr_20 [3]		=		0						--2
Item_Attr_20 [4]		=		0						--1

Item_Mission_22 = { }
Item_Mission_22 [0]		=		0						--
Item_Mission_22 [1]		=		0						--
Item_Mission_22 [2]		=		0						--
Item_Mission_22 [3]		=		0						--
Item_Mission_22 [4]		=		100						--
Item_Mission_22 [5]		=		100						--...
Item_Mission_22 [6]		=		100						--...
Item_Mission_22 [7]		=		100						--...
Item_Mission_22 [8]		=		100						--...
Item_Mission_22 [9]		=		100						--...

Item_Attr_22 = { }
Item_Attr_22 [0]		=		0							--5
Item_Attr_22 [1]		=		0							--4
Item_Attr_22 [2]		=		1							--3
Item_Attr_22 [3]		=		4							--2
Item_Attr_22 [4]		=		50							--1

Item_Mission_23 = { }
Item_Mission_23 [0]		=		0							--
Item_Mission_23 [1]		=		0							--
Item_Mission_23 [2]		=		0							--
Item_Mission_23 [3]		=		0							--
Item_Mission_23 [4]		=		3							--
Item_Mission_23 [5]		=		6							--...
Item_Mission_23 [6]		=		25							--...
Item_Mission_23 [7]		=		50							--...
Item_Mission_23 [8]		=		70							--...
Item_Mission_23 [9]		=		100							--...

Item_Attr_23 = { }
Item_Attr_23 [0]		=		0							--5
Item_Attr_23 [1]		=		0							--4
Item_Attr_23 [2]		=		1							--3
Item_Attr_23 [3]		=		5							--2
Item_Attr_23 [4]		=		60							--1

Item_Mission_24 = { }
Item_Mission_24 [0]		=		0							--
Item_Mission_24 [1]		=		0							--
Item_Mission_24 [2]		=		0							--
Item_Mission_24 [3]		=		0							--
Item_Mission_24 [4]		=		0							--
Item_Mission_24 [5]		=		1							--...
Item_Mission_24 [6]		=		5							--...
Item_Mission_24 [7]		=		20							--...
Item_Mission_24 [8]		=		65							--...
Item_Mission_24 [9]		=		99							--...

Item_Attr_24 = { }
Item_Attr_24 [0]		=		0							--5
Item_Attr_24 [1]		=		0							--4
Item_Attr_24 [2]		=		1							--3
Item_Attr_24 [3]		=		4							--2
Item_Attr_24 [4]		=		50							--1

--

Item_HoleNum_Monster = { }
Item_HoleNum_Monster [0]		=		75						--0
Item_HoleNum_Monster [1]		=		99						--1
Item_HoleNum_Monster [2]		=		100				--2
Item_HoleNum_Monster [3]		=		100						--3

Item_HoleNum_Hecheng = { }
Item_HoleNum_Hecheng [0]		=		25						--0
Item_HoleNum_Hecheng [1]		=		75						--1
Item_HoleNum_Hecheng [2]		=		100						--2
Item_HoleNum_Hecheng [3]		=		100						--3

Item_HoleNum_Mission_1 = { }
Item_HoleNum_Mission_1 [0]		=		25						--0
Item_HoleNum_Mission_1 [1]		=		75						--1
Item_HoleNum_Mission_1 [2]		=		100						--2
Item_HoleNum_Mission_1 [3]		=		100						--3




--
--
sk_jbjg = { }
sk_jbjg [1]				=		625
sk_jbjg [2]				=		3439
sk_jbjg [3]				=		12209
sk_jbjg [4]				=		29679
sk_jbjg [5]				=		58849
sk_jbjg [6]				=		102719
sk_jbjg [7]				=		164289
sk_jbjg [8]				=		246559
sk_jbjg [9]				=		352529
sk_jbjg [10]			=		485199

--
sk_hpsl = { }
sk_hpsl [1]				=		671
sk_hpsl [2]				=		4641
sk_hpsl [3]				=		14911
sk_hpsl [4]				=		34481
sk_hpsl [5]				=		66351
sk_hpsl [6]				=		113521
sk_hpsl [7]				=		178991
sk_hpsl [8]				=		265761
sk_hpsl [9]				=		376831
sk_hpsl [10]			=		515201

--
sk_ctqh = { }
sk_ctqh [1]			=		1105
sk_ctqh [2]			=		6095
sk_ctqh [3]			=		17985
sk_ctqh [4]			=		39775
sk_ctqh [5]			=		74465
sk_ctqh [6]			=		125055
sk_ctqh [7]			=		194545
sk_ctqh [8]			=		285935
sk_ctqh [9]			=		402225
sk_ctqh [10]			=		546415

--
sk_cfs = { }
sk_cfs [1]				=		1695
sk_cfs [2]				=		7825
sk_cfs [3]				=		21455
sk_cfs [4]				=		45585
sk_cfs [5]				=		83215
sk_cfs [6]				=		137345
sk_cfs [7]				=		210975
sk_cfs [8]				=		307105
sk_cfs [9]				=		428735
sk_cfs [10]				=		578865


--
sk_bjkr = { }
sk_bjkr [1]				=		2465
sk_bjkr [2]				=		9855
sk_bjkr [3]				=		25345
sk_bjkr [4]				=		51935
sk_bjkr [5]				=		92625
sk_bjkr [6]				=		150415
sk_bjkr [7]				=		228305
sk_bjkr [8]				=		329295
sk_bjkr [9]				=		456385
sk_bjkr [10]			=		612575

--
Guild_ItemMax = 1
Guild_item = {}
Guild_count = {}
Guild_fame	= 0
Guild_Gold	= 100000
Guild_item[1] =	1780	Guild_count[1]	= 1
Guild_item[2] =	-1		Guild_count[2]	=	-1
Guild_item[3] =	-1		Guild_count[3]	=	-1
Guild_item[4] =	-1		Guild_count[4]	=	-1
Guild_item[5] =	-1		Guild_count[5]	=	-1

----
JOINGUILD_NAVY_FAME	=	0			--
JOINGUILD_PIRATE_FAME	=	0			--

enumItemTypeSword 		= 1	-- sword
enumItemTypeGlave 		= 2	-- greatsword
enumItemTypeBow 		= 3	-- bow
enumItemTypeHarquebus 	= 4	-- gun
enumItemTypeFalchion 	= 5	-- blade
enumItemTypeMitten 		= 6	-- boxing glove
enumItemTypeStylet 		= 7	-- dagger
enumItemTypeMoneybag 	= 8 -- gold pouch
enumItemTypeCosh 		= 9	-- staff
enumItemTypeSinker 		= 10 -- hammer
enumItemTypeShield 		= 11 -- shield
enumItemTypeArrow 		= 12 -- arrow
enumItemTypeAmmo 		= 13 -- ammunition
enumItemTypeHeadpiece	= 19 -- pickaxe
enumItemTypeHair 		= 20 -- helm
enumItemTypeFace 		= 21 -- face
enumItemTypeClothing 	= 22 -- armor
enumItemTypeGlove 		= 23 -- glove
enumItemTypeBoot 		= 24 -- shoes
enumItemTypeConch 		= 29 -- coral
enumItemTypeMedicine 	= 31 -- recovery type
enumItemTypeOvum 		= 32 -- addition type
enumItemTypeScroll 		= 36 -- scrolls
enumItemTypeGeneral 	= 41 -- monster drop
enumItemTypeMission 	= 42 -- quest items
enumItemTypeBoat 		= 43 -- ships
enumItemTypeWing 		= 44 -- wings
enumItemTypeTrade 		= 45
enumItemTypeBravery 	= 46 -- medal of valor
enumItemTypeHull 		= 51 -- prow
enumItemTypeEmbolon 	= 52 -- hull
enumItemTypeEngine 		= 53 -- mobility
enumItemTypeArtillery 	= 54 -- ship cannon
enumItemTypeAirscrew 	= 55 -- propeller
enumItemTypeBoatSign 	= 56 -- ship symbol
enumItemTypePetFodder 	= 57 -- fairy ration
enumItemTypePetSock 	= 58 -- fairy fruit
enumItemTypePet 		= 59 -- fairy


SK_DPSL				= 073	-- Shield Mastery
SK_LZJ				= 090	-- Dual Shot
SK_LXJY				= 112	-- Meteor Shower
SK_LH				= 107	-- Howl
SK_SSD				= 114	-- Dispersion Bullet
SK_CTD				= 115	-- Penetrating Bullet
SK_DZY				= 117	-- Greater Heal
SK_HX				= 127	-- Tiger Roar
SK_RSD				= 113	-- Magma Bullet
SK_JJSL				= 067	-- Greatsword Mastery
SK_KB				= 084	-- Berserk
SK_FSZ				= 109	-- Dual Sword Mastery
SK_XZFY				= 104	-- Seal of Elder
SK_YMSL				= 108	-- Barbaric Crush
SK_CLXZ				= 076	-- Ranger
SK_FZLZ				= 101	-- Tempest Boost
SK_GJSL				= 074	-- Range Mastery
SK_HFWQ				= 122	-- Healing Spring
SK_JSFB				= 102	-- Tornado Swirl
SK_JFZ				= 064	-- Strengthen
SK_QXYJ				= 065	-- Deftness
SK_SHTZ				= 071	-- Beast Strength
SK_TSHD				= 103	-- Angelic Shield
SK_TSQY				= 045	-- Angel Blessing
SK_TJ				= 094	-- Cripple
SK_XZY				= 097	-- Heal
SK_XLZH				= 100	-- Spiritual Fire
SK_ZZZH				= 119	-- Cursed Fire
SK_SMYB				= 073	-- Shield Mastery
SK_YS				= 123	-- Stealth
SK_MB				= 088	-- Numb
SK_DB				= 087	-- Poison Dart
SK_ZJFT				= 110	-- Shield of Thorns
SK_HXDJ				= 111	-- Parry
SK_HXQJ				= 092	-- Astro Strike
SK_HQSL				= 078	-- Firegun Mastery
SK_BT				= 096	-- Headshot
SK_SY				= 080	-- Divine Grace
SK_ZF				= 091	-- Hunter Strike
SK_HFS				= 098	-- Recover
SK_DHFS				= 118	-- Greater Recover
SK_SYZY				= 116	-- True Sight
SK_AYZZ				= 105	-- Shadow Insignia
SK_JSSL				= 062	-- Sword Mastery
SK_GTYZ				= 063	-- Will of Steel
SK_QHTZ				= 064	-- Strengthen
SK_LQHB				= 065	-- Deftness
SK_JDZZ				= 066	-- Concentration
SK_HYZ				= 081	-- Illusion Slash
SK_ZJ				= 082	-- Mighty Strike
SK_MNRX				= 068	-- Blood Bull
SK_SWZQ				= 083	-- Primal Rage
SK_HYS				= 069	-- Evasion
SK_PXKG				= 070	-- Blood Frenzy
SK_GWZ				= 086	-- Shadow Slash
SK_TZHF				= 072	-- Recuperate
SK_JFB				= 075	-- Windwalk
SK_YY				= 089	-- Eagle's Eye
SK_LDC				= 091	-- Hunter Strike
SK_JSJC				= 079	-- Vigor
SK_XLCZ				= 099	-- Spiritual Bolt
SK_BDJ				= 093	-- Frozen Arrow
SK_LRWZ				= 077	-- Hunter Disguise
SK_SJ				= 095	-- Enfeeble
SK_SDBZ				= 120	-- Counterguard
SK_SYNZ				= 121	-- Abyss Mire
SK_XLPZ				= 106	-- Energy Shield
SK_FH				= 124	-- Revival
SK_TTCB				= 125	-- Totem's Worship
SK_DYYJ				= 126	-- Gunpowder Research
SK_JR				= 210	-- Diligence
SK_SL				= 211	-- Current
SK_BKZJ				= 212	-- Conch Armor
SK_JF				= 213	-- Tornado
SK_LJ				= 214	-- Lightning Bolt
SK_HZCR				= 215	-- Algae Entanglement
SK_BKCJ				= 216	-- Conch Ray
SK_SF				= 217	-- Tail Wind
SK_XW				= 218	-- Whirlpool
SK_MW				= 219	-- Fog
SK_LM				= 220	-- Lightning Curtain
SK_PJ				= 222	-- Break Armor
SK_FNQ				= 223	-- Rousing
SK_DJ				= 224	-- Venom Arrow
SK_SHPF				= 225	-- Harden
SK_HPSL				= 226	-- Cannon Mastery
SK_JBJG				= 227	-- Toughen Wood
SK_CFS				= 228	-- Sail Mastery
SK_CTQH				= 229	-- Reinforce Ship
SK_BJCR				= 230	-- Oil Tank Upgrade
SK_BY				= 231	-- Fishing
SK_DL				= 232	-- Salvage
SK_SWCX				= 234	-- Death Shriek
SK_XN				= 235	-- Blood Fury
SK_NT				= 236	-- Lair
SK_DIZ				= 237	-- Earthquake
SK_XIK				= 238	-- Colossus Smash
SK_BIW				= 239	-- Kiss of Frost
SK_Fer				= 240	-- Tempest Blade
SK_BAT				= 241	-- Set Stall
SK_CHF				= 242	-- Taunt
SK_PAX				= 243	-- Roar
SK_FUZ				= 244	-- Replicate
SK_HZTX				= 245	-- Alga Ambush
SK_SMDJ				= 246	-- Jellyfish Electric Bolt
SK_WZXF				= 247	-- Squid Whirlstrike
SK_SYZM				= 248	-- Shark Attack
SK_KDZB				= 249	-- Polliwog Self Explode
SK_SHJNY			= 250	-- Primal Skill 1
SK_SHJNE			= 251	-- Primal Rage 2
SK_BOMB  			= 252	-- Water Mine Explosion
SK_BLYZ				= 255	-- Crystalline Blessing
SK_MLCH				= 256	-- Intense Magic
SK_JSDD				= 257	-- Corpse Attack
SK_JSMF				= 258	-- Corpse Range Attack
SK_HDSMF			= 259	-- Fox Taoist Sorcery
SK_HYMF				= 260	-- Fox Spirit Sorcery
SK_HYMH				= 261	-- Fascinate
SK_HXMF				= 262	-- Fox Immortal Sorcery
SK_HXFWMF			= 263	-- Fox Immortal Area Spell
SK_TZJSMagic		= 264	-- Ritual
SK_QZMF				= 265	-- Sachem's Witchcraft
SK_XZSB				= 266	-- Whirling Hook
SK_QX				= 267	-- Dog Howl
SK_SD				= 268	-- Corpse Venom
SK_BLGJ				= 269	-- Icy Dragon Strike
SK_JXJBFW			= 270	-- Cyborg Blockade
SK_CRXSF			= 271	-- Crab Binding
SK_SXZZZ			= 272	-- Curse of Kelpie
SK_XBLBD			= 273	-- Frost Breath
SK_BHSD				= 274	-- Dance of Icy Dragon
SK_HLKJ				= 275	-- Black Dragon Terror
SK_HLLM				= 276	-- Black Dragon Roar
SK_BlackLY			= 277	-- Black Dragon Flight
SK_BlackLX			= 278	-- Black Dragon Breath
SK_BlackHeal		= 279	-- Resurrect
SK_JLFT				= 280	-- Fairy body
SK_JLZB				= 311	-- Self Destruct
SK_JLTX1			= 312	-- Skill - Defecate
SK_JLTX2			= 313	-- Skill - Undergarment
SK_JLTX3			= 314	-- Skill - Garment
SK_JLTX4			= 315	-- Skill - Coin Shower
SK_JLTX5			= 316	-- Skill - Fool
SK_JLTX6			= 317	-- Skill - Snooty
SK_JLTX7			= 318	-- Skill - Trickster
SK_JLTX8			= 319	-- Skill - Dumb
SK_KS				= 200	-- Woodcutting
SK_WK				= 201	-- Mining
SK_PKQX				= 254	-- Repair
SK_ZHIZAO			= 338	-- Manufacturing
SK_PENGREN			= 339	-- Cooking
SK_ZHUZAO			= 340	-- Crafting
SK_FENJIE			= 341	-- Analyze
SK_WYZ				= 453	-- Ethereal Slash
SK_CYN				= 454	-- Super consciousness
SK_BSJ				= 455	-- Beast Legion Smash
SK_HLP				= 456	-- Red Thunder Cannon
SK_EMZZ				= 457	-- Devil Curse
SK_SSSP				= 458	-- Holy Judgement
SK_ZSSL				= 459	-- Rebirth Mystic Power
SK_DS				= 461	-- Read
SK_QLZX				= 467	-- Love line
SK_FNZ 				= 494	-- Fake Singlehood Ring
SK_HW 				= 495	-- Treasure Hunt
SK_NLD	 			= 496	-- Shock to the Heart
SK_MLCJ				= 498	-- Tribulation of Faith BOSS1
ITEM_RELIFE			= 3143
FCARDID 			= 7298

STATE_RS			= 1  	-- Burn
STATE_HFWQ			= 2  	-- Healing Spring
STATE_ZZZH			= 3  	-- Cursed Fire
STATE_ZD			= 4  	-- Poisoned
STATE_SDBZ			= 5  	-- Counterguard
STATE_SYZY			= 6  	-- True Sight
STATE_SYNZ			= 7  	-- Abyss Mire
STATE_FREE1			= 8  	-- FREE EFFECT ID
STATE_LQ			= 9  	-- Thunderstorm
STATE_WQ			= 10 	-- Fog
STATE_FQ			= 11 	-- Tornado
STATE_XW			= 12 	-- Whirlpool
STATE_MW			= 13 	-- Fog
STATE_LM			= 14 	-- Lightning Curtain
STATE_CHF			= 15 	-- Taunt
STATE_BOMB			= 16 	-- Water Mine Self Destruct
STATE_PKMNYS		= 17 	-- Bull Potion
STATE_HAIR			= 18 	-- Hairstyling
STATE_PKZDYS		= 19 	-- Battle Potion
STATE_PKKBYS		= 20 	-- Berserk Potion
STATE_FREE2			= 21 	-- FREE EFFECT ID
STATE_HX			= 22 	-- Tiger Roar
STATE_FREE3			= 23 	-- FREE EFFECT ID
STATE_KB			= 24 	-- Berserk
STATE_SD			= 25 	-- Sudden Death 2
STATE_JNJZ			= 26 	-- Skill Forbidden
STATE_ZMYJ			= 27 	-- Fatal Strike
STATE_FREE4			= 28 	-- FREE EFFECT ID
STATE_FZLZ			= 29 	-- Tempest Boost
STATE_FREE5			= 30 	-- FREE EFFECT ID
STATE_FREE6			= 31 	-- FREE EFFECT ID
STATE_FREE7			= 32 	-- FREE EFFECT ID
STATE_JSFB			= 33 	-- Tornado Swirl
STATE_FREE8			= 34 	-- FREE EFFECT ID
STATE_FREE9			= 35 	-- FREE EFFECT ID
STATE_FREE10		= 36 	-- FREE EFFECT ID
STATE_TSHD			= 37 	-- Angelic Shield
STATE_TSQY			= 38 	-- Angel Blessing
STATE_TJ			= 39 	-- Cripple
STATE_XLZH			= 40 	-- Spiritual Fire
STATE_PKJSYS		= 41 	-- Energy Potion
STATE_PKSFYS		= 42 	-- Harden Potion
STATE_YS			= 43 	-- Conceal
STATE_PKJZYS		= 44 	-- Accurate Potion
STATE_XY			= 45 	-- Blackout
STATE_MB			= 46 	-- Numb
STATE_PKWD			= 47 	-- Invincible Potion
STATE_SBJYGZ		= 48 	-- Double Experience Rate
STATE_SBBLGZ		= 49 	-- Double Drop Rate
STATE_MLCH			= 50 	-- Intense Magic
STATE_BSHD			= 51 	-- Frost Shield
STATE_FREE11		= 52 	-- FREE EFFECT ID
STATE_FREE12		= 53 	-- FREE EFFECT ID
STATE_JLHY			= 54 	-- Refined Gunpowder
STATE_RDGJ			= 55 	-- Attack Weakness
STATE_FREE13		= 56 	-- FREE EFFECT ID
STATE_QY			= 57 	-- Angel Blessing
STATE_ZF			= 58 	-- Benediction
STATE_LYZY			= 59 	-- Inferno Wings
STATE_SHZG			= 60 	-- Holy Beam
STATE_SZWZ			= 61 	-- Holy Heraldry
STATE_XYZG			= 62 	-- Ray of Luck
STATE_PKDYK			= 63 	-- Ammunition Warehouse is empty
STATE_PKLC			= 64 	-- Granary empty
STATE_GJJZ			= 65 	-- Attack forbidden
STATE_CLCY			= 66 	-- Traversing
STATE_HITD			= 67 	-- Hit Rate Decrease
STATE_SJ			= 68 	-- Enfeeble
STATE_FREE14		= 69 	-- FREE EFFECT ID
STATE_GTYZ			= 70 	-- Will of Steel
STATE_FREE15		= 71 	-- FREE EFFECT ID
STATE_FREE16		= 72 	-- FREE EFFECT ID
STATE_FREE17		= 73 	-- FREE EFFECT ID
STATE_FREE18		= 74 	-- FREE EFFECT ID
STATE_FREE19		= 75 	-- FREE EFFECT ID
STATE_FREE20		= 76 	-- FREE EFFECT ID
STATE_FREE21		= 77 	-- FREE EFFECT ID
STATE_FREE22		= 78 	-- FREE EFFECT ID
STATE_YY			= 79 	-- Eagle's Eye
STATE_FREE23		= 80 	-- FREE EFFECT ID
STATE_BDJ			= 81 	-- Frozen Arrow
STATE_FREE24		= 82 	-- FREE EFFECT ID
STATE_MFD			= 83 	-- Magical Shield
STATE_FREE25		= 84 	-- FREE EFFECT ID
STATE_JY			= 85 	-- Trade Status
STATE_JF			= 86 	-- Tornado
STATE_HZCR			= 87 	-- Algae Entanglement
STATE_SF			= 88 	-- Tail Wind
STATE_PJ			= 89 	-- Break Armor
STATE_FNQ			= 90 	-- Rousing
STATE_DJ			= 91 	-- Dart
STATE_SHPF			= 92 	-- Harden
STATE_XN			= 93 	-- Blood Fury
STATE_NT			= 94 	-- Lair
STATE_DIZ			= 95 	-- Earthquake
STATE_BIW			= 96 	-- Kiss of Frost
STATE_MCK			= 97 	-- Wood Bundle
STATE_SWCX			= 98 	-- Death Shriek
STATE_BAT			= 99 	-- Set Stall
STATE_REPA			= 100	-- Repair
STATE_FORG			= 101	-- Forging
STATE_YSLLQH		= 102	-- Potion strength modify
STATE_YSMJQH		= 103	-- Potion agility modify
STATE_YSLQQH		= 104	-- Potion accuracy modify
STATE_YSTZQH		= 105	-- Potion constitution modify
STATE_YSJSQH		= 106	-- Potion spirit modify
STATE_JLGLJB		= 107	-- Forging success rate increased
STATE_HCGLJB		= 108	-- Increases combination success rates
STATE_DENGLONG		= 109	-- Lantern
STATE_GAMEMASTER	= 220	-- Game Master
STATE_YPCXHFSM		= 111	-- Potions to recover HP
STATE_CFZJiu1		= 112	-- Wine 1
STATE_CFZJiu2		= 113	-- Wine 2
STATE_JSDD			= 114	-- Corpse Poison
STATE_HYMH			= 115	-- Fascinate
STATE_HLKJ			= 116	-- Black Dragon Terror
STATE_HLLM			= 117	-- Black Dragon Roar
STATE_CRXSF			= 118	-- Crab Binding
STATE_MarchElf		= 119	-- Fairy March
STATE_YSMspd		= 120	-- Potion speed modify
STATE_YSBoatMspd	= 121	-- Potion ship speed modify
STATE_YSBoatDEF		= 122	-- Potion ship defense modify
STATE_TTISW			= 123	-- Spring Town Status
STATE_PKSBYS		= 124	-- Ether Clover
STATE_BlackHX		= 125	-- Black Dragon Roar
STATE_BDLB			= 126	-- Black Dragon Lightning Bolt
STATE_ZDSBJYGZ		= 127	-- Team experience bonus
STATE_KUANGZ		= 128	-- Battle Array
STATE_QUANS			= 129	-- Full Body Armor
STATE_QINGZ			= 130	-- Weightless Potion
STATE_JLDS			= 131	-- Pet Poison Bite
STATE_JLFT1			= 132	-- Fairy Possession A
STATE_CJBBT			= 133	-- Super Lollipop
STATE_JRQKL			= 134	-- Huge Chocolate
STATE_WLRSD			= 135	-- Deathsoul MaGMa Bullet
STATE_WLJS			= 136	-- Deathsoul Acceleration
STATE_WLXW			= 137	-- Deathsoul Whirlpool
STATE_KLCS			= 138	-- Encumbered Skeleton
STATE_KLHD			= 139	-- Skeletar Shielding
STATE_WLCX			= 140	-- Deathsoul Taunt
STATE_ZZZX			= 141	-- Moonlight Recovery
STATE_WLDB			= 142	-- Deathsoul Poison Dart
STATE_WLJY			= 143	-- Deathsoul Roquet
STATE_WHIR			= 144	-- Whirlpool
STATE_CLOUD			= 145	-- Cloud Attack
STATE_WLNH			= 146	-- Roar of Deathsoul
STATE_JLJSGZ		= 147	-- Double growth rate for Fairy
STATE_JLTX1			= 148	-- Skill - Defecate
STATE_JLTX2			= 149	-- Skill - Undergarment
STATE_JLTX3			= 150	-- Skill - Garment
STATE_JLTX4			= 151	-- Skill - Coin Shower
STATE_JLTX5			= 152	-- Skill - Fool
STATE_JLTX6			= 153	-- Skill - Snooty
STATE_JLTX7			= 154	-- Skill - Dumb
STATE_JLTX8			= 155	-- Skill - Dumb
STATE_CZZX			= 156	-- Heart of Innocence
STATE_KALA			= 157	-- Kara's Victory
STATE_5MBS			= 158	-- Sure-death
STATE_ShanGD		= 159	-- Flash Bomb
STATE_FuShe			= 160	-- Radiation
STATE_PSQ			= 161	-- Ship Atomizer
STATE_PRD			= 162	-- Impaler
STATE_CZRSD			= 163	-- Ship Flamer
STATE_BOATSPD		= 164	-- Boat Accelerate
STATE_XUEYU			= 165	-- Max HP Physical Resist Type
STATE_MANTOU		= 166	-- Bun attack type
STATE_NVER			= 167	-- Maiden Wine Spirit Type
STATE_JLFT2			= 168	-- Fairy Possession B
STATE_JLFT3			= 169	-- Fairy Possession C
STATE_JLFT4			= 170	-- Fairy Possession D
STATE_JLFT5			= 171	-- Fairy Possession E
STATE_JLFT6			= 172	-- Fairy Possession F
STATE_JLFT7			= 173	-- Fairy Possession G
STATE_JLFT8			= 174	-- Fairy Possession H
STATE_MOWQ			= 175	-- Devil Curse B
STATE_FSZQ 			= 176	-- Carrion Ball
STATE_ZYZZ 			= 177	-- Noise Polluter
STATE_DZFS 			= 178	-- Earthquake Generator
STATE_LD   			= 179	-- Chain Bullet
STATE_HYFS 			= 180	-- Mirage Generator
STATE_CZQX 			= 181	-- Stealth Ship
STATE_LEIDA			= 182	-- Radar
STATE_FSD  			= 183	-- Carrion Bullet
STATE_Slrs 			= 184	-- Water Mine burning
STATE_Myrs 			= 185	-- Burning Lamb
STATE_LST  			= 186	-- Drag Tower
STATE_HFZQ 			= 187	-- Healing Spring
STATE_EMYY 			= 188	-- Devil Curse A
STATE_YNZL 			= 189	-- Super consciousness
STATE_FREE26		= 190	-- FREE EFFECT ID
STATE_BDH  			= 191	-- Frozen ring
STATE_SOL			= 192	-- Spirit of Light
STATE_DBS			= 193	-- Dizzy Back Spirit
STATE_DHZ			= 194	-- Egg Yolk Dumpling
STATE_DSZ			= 195	-- Bean Paste Dumpling
STATE_APPLE			= 196	-- Double Study experience
STATE_ILOVEDAD		= 197	-- I love my father
STATE_HPHMHF		= 198	-- Recover HP slowly
STATE_SPHMHF		= 199	-- Recover SP slowly
STATE_BBRING1		= 200	-- 85BB Cow Ring
STATE_BBRING2		= 201	-- 85BB Dual Ring
STATE_BBRING3		= 202	-- 85BB Hunt Ring
STATE_BBRING4		= 203	-- 85BB Sail Ring
STATE_BBRING5		= 204	-- 85BB Seal Ring
STATE_BBRING6		= 205	-- 85BB Sacred Ring
STATE_LANTERN		= 206	-- Navigating Lantern
STATE_RAPIDDRUG		= 207	-- Swiftness Potion
STATE_WARSIT		= 208	-- War Fanatic Injection
STATE_DARKDRESS		= 209	-- Dragon's Armor
STATE_FREE30		= 210	-- FREE EFFECT ID
STATE_FREE31		= 211	-- FREE EFFECT ID
STATE_FREE32		= 212	-- FREE EFFECT ID
STATE_FREE33		= 213	-- FREE EFFECT ID
STATE_FREE34		= 214	-- FREE EFFECT ID
STATE_SHIPDRIVE		= 215	-- Ship's Driving Sail
STATE_SHIPRECOVER	= 216	-- Ship's Defensive deck
STATE_AMPPF			= 217	-- Party Fruit EXP
STATE_XZDLL			= 218	-- Spirit Power
STATE_XTMFS			= 219	-- Chiatan's Magic Book
STATE_MWMXJ			= 220	-- Super Moye Sword
STATE_TELF			= 221	-- Thunder
STATE_LELF			= 222	-- Light
STATE_DELF			= 223	-- Darkness
STATE_XLTX			= 224	-- Necklace Effect
STATE_FREE27		= 225	-- FREE EFFECT ID
STATE_FREE28		= 226	-- FREE EFFECT ID
STATE_JLBYS			= 227	-- 100% Forging successful rate
STATE_HCBYS			= 228	-- 100% CombineSuccess rate
STATE_zhongshen		= 229	-- Lv 95 BB Set
STATE_LEIPI			= 230	-- Thunder Strike
STATE_XIANRENJIAO 	= 231	-- Immortal Feet
STATE_CUSI 			= 232	-- Sudden Death
STATE_GANMAO		= 233	-- Barbecue
STATE_ZBMAXHP		= 234	-- Augury: Max HP increased by shield
STATE_ZBHP			= 235	-- Augury: Max HP
STATE_ZBSP			= 236	-- Augury: Max SP
STATE_ZBMOVE		= 237	-- Augury: Max Movement Speed
STATE_FREE29		= 238	-- FREE EFFECT ID
STATE_QB			= 239	-- Information Bank
STATE_XZK1			= 240	-- Aquarius
STATE_XZK2			= 241	-- Pisces
STATE_XZK3			= 242	-- Aries
STATE_XZK4			= 243	-- Taurus
STATE_XZK5			= 244	-- Gemini
STATE_XZK6			= 245	-- Cancer
STATE_XZK7			= 246	-- Leo
STATE_XZK8			= 247	-- Virgo
STATE_XZK9			= 248	-- Libra
STATE_XZK10			= 249	-- Scorpio
STATE_XZK11			= 250	-- Sagittarius
STATE_XZK12			= 251	-- Capricorn
STATE_NSTX			= 252	-- Goddess
STATE_HAIDAOQI		= 253	-- Pirate Banner

dmg = 0					--
sus = 1					--
hpdmg = 0					--hp
dmgsa = 1					--
dis = 0					--
dis_eff = 0					--
sklv = 0					--

ItemAttr_Rad	=	 { }
ItemAttr_Rad	[	0	]	=	0	--	-1	
ItemAttr_Rad	[	1	]	=	10	--		+str
ItemAttr_Rad	[	2	]	=	10	--		+dex
ItemAttr_Rad	[	3	]	=	10	--		+con
ItemAttr_Rad	[	4	]	=	5	--		+agi
ItemAttr_Rad	[	5	]	=	10	--		+sta
ItemAttr_Rad	[	6	]	=	0	--	-1	0
ItemAttr_Rad	[	7	]	=	0	--	-1	0
ItemAttr_Rad	[	8	]	=	0	--	-1	0
ItemAttr_Rad	[	9	]	=	0	--	-1	0
ItemAttr_Rad	[	10	]	=	0	--	-1	0
ItemAttr_Rad	[	11	]	=	10	--		+str +dex
ItemAttr_Rad	[	12	]	=	10	--		+str +con
ItemAttr_Rad	[	13	]	=	5	--		+str +agi
ItemAttr_Rad	[	14	]	=	10	--		+str +sta
ItemAttr_Rad	[	15	]	=	10	--		+dex +con
ItemAttr_Rad	[	16	]	=	5	--		+dex +agi
ItemAttr_Rad	[	17	]	=	10	--		+dex +sta
ItemAttr_Rad	[	18	]	=	5	--		+con +agi
ItemAttr_Rad	[	19	]	=	10	--		+con +sta
ItemAttr_Rad	[	20	]	=	5	--		+agi +sta
ItemAttr_Rad	[	21	]	=	0	--	-1	0
ItemAttr_Rad	[	22	]	=	0	--	-1	0
ItemAttr_Rad	[	23	]	=	0	--	-1	0
ItemAttr_Rad	[	24	]	=	0	--	-1	0
ItemAttr_Rad	[	25	]	=	0	--	-1	0
ItemAttr_Rad	[	26	]	=	0	--	-1	0
ItemAttr_Rad	[	27	]	=	0	--	-1	0
ItemAttr_Rad	[	28	]	=	0	--	-1	0
ItemAttr_Rad	[	29	]	=	0	--	-1	0
ItemAttr_Rad	[	30	]	=	0	--	-1	0
ItemAttr_Rad	[	31	]	=	0	--	-1	0
ItemAttr_Rad	[	32	]	=	0	--	-1	0
ItemAttr_Rad	[	33	]	=	0	--	-1	0
ItemAttr_Rad	[	34	]	=	0	--	-1	0
ItemAttr_Rad	[	35	]	=	0	--	-1	0
ItemAttr_Rad	[	36	]	=	0	--	-1	0
ItemAttr_Rad	[	37	]	=	0	--	-1	0
ItemAttr_Rad	[	38	]	=	0	--	-1	0
ItemAttr_Rad	[	39	]	=	0	--	-1	0
ItemAttr_Rad	[	40	]	=	0	--	-1	0
ItemAttr_Rad	[	41	]	=	0	--	-1	0
ItemAttr_Rad	[	42	]	=	0	--	-1	0
ItemAttr_Rad	[	43	]	=	0	--	-1	0
ItemAttr_Rad	[	44	]	=	0	--	-1	0
ItemAttr_Rad	[	45	]	=	0	--	-1	0
ItemAttr_Rad	[	46	]	=	0	--	-1	0
ItemAttr_Rad	[	47	]	=	0	--	-1	0
ItemAttr_Rad	[	48	]	=	0	--	-1	0
ItemAttr_Rad	[	49	]	=	0	--	-1	0
ItemAttr_Rad	[	50	]	=	10	--		+str +dex +con
ItemAttr_Rad	[	51	]	=	5	--		+str +dex +agi
ItemAttr_Rad	[	52	]	=	10	--		+str +dex +sta
ItemAttr_Rad	[	53	]	=	5	--		+str +con +agi
ItemAttr_Rad	[	54	]	=	10	--		+str +con +sta
ItemAttr_Rad	[	55	]	=	5	--		+str +agi +sta
ItemAttr_Rad	[	56	]	=	5	--		+dex +con +agi
ItemAttr_Rad	[	57	]	=	10	--		+dex +con +sta
ItemAttr_Rad	[	58	]	=	5	--		+dex +agi +sta
ItemAttr_Rad	[	59	]	=	5	--		+con +agi +sta
ItemAttr_Rad	[	60	]	=	0	--	-1	0
ItemAttr_Rad	[	61	]	=	0	--	-1	0
ItemAttr_Rad	[	62	]	=	0	--	-1	0
ItemAttr_Rad	[	63	]	=	0	--	-1	0
ItemAttr_Rad	[	64	]	=	0	--	-1	0
ItemAttr_Rad	[	65	]	=	0	--	-1	0
ItemAttr_Rad	[	66	]	=	0	--	-1	0
ItemAttr_Rad	[	67	]	=	0	--	-1	0
ItemAttr_Rad	[	68	]	=	0	--	-1	0
ItemAttr_Rad	[	69	]	=	0	--	-1	0
ItemAttr_Rad	[	70	]	=	0	--	-1	0
ItemAttr_Rad	[	71	]	=	0	--	-1	0
ItemAttr_Rad	[	72	]	=	0	--	-1	0
ItemAttr_Rad	[	73	]	=	0	--	-1	0
ItemAttr_Rad	[	74	]	=	0	--	-1	0
ItemAttr_Rad	[	75	]	=	0	--	-1	0
ItemAttr_Rad	[	76	]	=	0	--	-1	0
ItemAttr_Rad	[	77	]	=	0	--	-1	0
ItemAttr_Rad	[	78	]	=	0	--	-1	0
ItemAttr_Rad	[	79	]	=	0	--	-1	0
ItemAttr_Rad	[	80	]	=	0	--	-1	0
ItemAttr_Rad	[	81	]	=	0	--	-1	0
ItemAttr_Rad	[	82	]	=	0	--	-1	0
ItemAttr_Rad	[	83	]	=	0	--	-1	0
ItemAttr_Rad	[	84	]	=	0	--	-1	0
ItemAttr_Rad	[	85	]	=	0	--	-1	0
ItemAttr_Rad	[	86	]	=	0	--	-1	0
ItemAttr_Rad	[	87	]	=	0	--	-1	0
ItemAttr_Rad	[	88	]	=	0	--	-1	0
ItemAttr_Rad	[	89	]	=	0	--	-1	0
ItemAttr_Rad	[	90	]	=	10	--		 +str +dex +con +agi
ItemAttr_Rad	[	91	]	=	10	--		 +str +dex +con +sta
ItemAttr_Rad	[	92	]	=	10	--		 +str +dex +agi +sta
ItemAttr_Rad	[	93	]	=	10	--		 +str +con +agi +sta
ItemAttr_Rad	[	94	]	=	10	--		 +dex +con +agi +sta
ItemAttr_Rad	[	95	]	=	0	--	-1	0
ItemAttr_Rad	[	96	]	=	0	--	-1	0
ItemAttr_Rad	[	97	]	=	0	--	-1	0
ItemAttr_Rad	[	98	]	=	0	--	-1	0
ItemAttr_Rad	[	99	]	=	0	--	-1	0

Upgrade = {}
Upgrade.Money = 100000	-- Amount multipled by item level for total cost.
-- // Original items from script. // --
Upgrade[1 ] = {ID = 825, Catalyst = 2403, Result = 2549}
Upgrade[2 ] = {ID = 826, Catalyst = 2403, Result = 2550}
Upgrade[3 ] = {ID = 827, Catalyst = 2403, Result = 2551}
Upgrade[4 ] = {ID = 828, Catalyst = 2403, Result = 2552}
Upgrade[5 ] = {ID = 845, Catalyst = 2404, Result = 2367}
Upgrade[6 ] = {ID = 846, Catalyst = 2404, Result = 2368}
Upgrade[7 ] = {ID = 847, Catalyst = 2404, Result = 2369}
Upgrade[8 ] = {ID = 848, Catalyst = 2404, Result = 2370}
Upgrade[9 ] = {ID = 1884, Catalyst = 13749, Result = 763}
Upgrade[10] = {ID = 1891, Catalyst = 13749, Result = 770}
Upgrade[11] = {ID = 1898, Catalyst = 13749, Result = 777}
Upgrade[12] = {ID = 1902, Catalyst = 13749, Result = 781}
Upgrade[13] = {ID = 1910, Catalyst = 13749, Result = 789}
Upgrade[14] = {ID = 1906, Catalyst = 13749, Result = 785}
Upgrade[15] = {ID = 1924, Catalyst = 13749, Result = 803}
Upgrade[16] = {ID = 1920, Catalyst = 13749, Result = 799}
Upgrade[17] = {ID = 1899, Catalyst = 13750, Result = 778}
Upgrade[18] = {ID = 1903, Catalyst = 13750, Result = 782}
Upgrade[19] = {ID = 6092, Catalyst = 13750, Result = 6089}
Upgrade[20] = {ID = 1888, Catalyst = 13750, Result = 767}
Upgrade[21] = {ID = 1895, Catalyst = 13750, Result = 774}
Upgrade[22] = {ID = 1885, Catalyst = 13750, Result = 764}
Upgrade[23] = {ID = 1892, Catalyst = 13750, Result = 771}
Upgrade[24] = {ID = 1911, Catalyst = 13750, Result = 790}
Upgrade[25] = {ID = 1907, Catalyst = 13750, Result = 786}
Upgrade[26] = {ID = 1917, Catalyst = 13750, Result = 796}
Upgrade[27] = {ID = 1914, Catalyst = 13750, Result = 793}
Upgrade[28] = {ID = 1925, Catalyst = 13750, Result = 804}
Upgrade[29] = {ID = 1921, Catalyst = 13750, Result = 800}
Upgrade[30] = {ID = 1900, Catalyst = 13751, Result = 779}
Upgrade[31] = {ID = 1990, Catalyst = 13751, Result = 809}
Upgrade[32] = {ID = 1991, Catalyst = 13751, Result = 810}
Upgrade[33] = {ID = 1904, Catalyst = 13751, Result = 783}
Upgrade[34] = {ID = 6093, Catalyst = 13751, Result = 6090}
Upgrade[35] = {ID = 1889, Catalyst = 13751, Result = 768}
Upgrade[36] = {ID = 1989, Catalyst = 13751, Result = 808}
Upgrade[37] = {ID = 1988, Catalyst = 13751, Result = 807}
Upgrade[38] = {ID = 1896, Catalyst = 13751, Result = 775}
Upgrade[39] = {ID = 1886, Catalyst = 13751, Result = 765}
Upgrade[40] = {ID = 1893, Catalyst = 13751, Result = 772}
Upgrade[41] = {ID = 1912, Catalyst = 13751, Result = 791}
Upgrade[42] = {ID = 1993, Catalyst = 13751, Result = 812}
Upgrade[43] = {ID = 1995, Catalyst = 13751, Result = 814}
Upgrade[44] = {ID = 1908, Catalyst = 13751, Result = 787}
Upgrade[45] = {ID = 1918, Catalyst = 13751, Result = 797}
Upgrade[46] = {ID = 1992, Catalyst = 13751, Result = 811}
Upgrade[47] = {ID = 1994, Catalyst = 13751, Result = 813}
Upgrade[48] = {ID = 1915, Catalyst = 13751, Result = 794}
Upgrade[49] = {ID = 1926, Catalyst = 13751, Result = 805}
Upgrade[50] = {ID = 1996, Catalyst = 13751, Result = 815}
Upgrade[51] = {ID = 1997, Catalyst = 13751, Result = 877}
Upgrade[52] = {ID = 1922, Catalyst = 13751, Result = 801}
Upgrade[53] = {ID = 1905, Catalyst = 13752, Result = 784}
Upgrade[54] = {ID = 6094, Catalyst = 13752, Result = 6091}
Upgrade[55] = {ID = 1901, Catalyst = 13752, Result = 780}
Upgrade[56] = {ID = 1890, Catalyst = 13752, Result = 769}
Upgrade[57] = {ID = 1897, Catalyst = 13752, Result = 776}
Upgrade[58] = {ID = 1887, Catalyst = 13752, Result = 766}
Upgrade[59] = {ID = 1894, Catalyst = 13752, Result = 773}
Upgrade[60] = {ID = 1913, Catalyst = 13752, Result = 792}
Upgrade[61] = {ID = 1909, Catalyst = 13752, Result = 788}
Upgrade[62] = {ID = 1919, Catalyst = 13752, Result = 798}
Upgrade[63] = {ID = 1916, Catalyst = 13752, Result = 795}
Upgrade[64] = {ID = 1927, Catalyst = 13752, Result = 806}
Upgrade[65] = {ID = 1923, Catalyst = 13752, Result = 802}
Upgrade[66] = {ID = 2530, Catalyst = 2562, Result = 2817}
Upgrade[67] = {ID = 2531, Catalyst = 2562, Result = 2818}
Upgrade[68] = {ID = 2532, Catalyst = 2562, Result = 2819}
Upgrade[69] = {ID = 2533, Catalyst = 2563, Result = 2820}
Upgrade[70] = {ID = 2534, Catalyst = 2563, Result = 2821}
Upgrade[71] = {ID = 2535, Catalyst = 2563, Result = 2822}
Upgrade[72] = {ID = 2536, Catalyst = 2564, Result = 2823}
Upgrade[73] = {ID = 2537, Catalyst = 2564, Result = 2824}
Upgrade[74] = {ID = 2538, Catalyst = 2564, Result = 2825}
Upgrade[75] = {ID = 2539, Catalyst = 2565, Result = 2826}
Upgrade[76] = {ID = 2540, Catalyst = 2565, Result = 2827}
Upgrade[77] = {ID = 2541, Catalyst = 2565, Result = 2828}
Upgrade[78] = {ID = 2542, Catalyst = 2566, Result = 2829}
Upgrade[79] = {ID = 2543, Catalyst = 2566, Result = 2830}
Upgrade[80] = {ID = 2544, Catalyst = 2566, Result = 2831}
Upgrade[81] = {ID = 2545, Catalyst = 2567, Result = 2832}
Upgrade[82] = {ID = 2546, Catalyst = 2567, Result = 2833}
Upgrade[83] = {ID = 2547, Catalyst = 2567, Result = 2834}


MentorSys = {}
-- MentorSys[9] = {Gold = nil, Reputation = {Disciple = nil, Mentor = 5}, Items = nil, Message = nil}
MentorSys[2 ] = {Reputation = {Disciple = 0, Mentor = 1}, Message = nil}
MentorSys[3 ] = {Reputation = {Disciple = 0, Mentor = 2}, Message = nil}
MentorSys[4 ] = {Reputation = {Disciple = 0, Mentor = 3}, Message = nil}
MentorSys[5 ] = {Reputation = {Disciple = 0, Mentor = 4}, Message = nil}
MentorSys[6 ] = {Reputation = {Disciple = 0, Mentor = 5}, Message = nil}
MentorSys[7 ] = {Reputation = {Disciple = 0, Mentor = 6}, Message = nil}
MentorSys[8 ] = {Reputation = {Disciple = 0, Mentor = 7}, Message = nil}
MentorSys[9 ] = {Reputation = {Disciple = 0, Mentor = 8}, Message = nil}
MentorSys[10] = {Reputation = {Disciple = 0, Mentor = 9}, Message = nil}
MentorSys[11] = {Reputation = {Disciple = 0, Mentor = 10}, Message = nil}
MentorSys[12] = {Reputation = {Disciple = 0, Mentor = 12}, Message = nil}
MentorSys[13] = {Reputation = {Disciple = 0, Mentor = 14}, Message = nil}
MentorSys[14] = {Reputation = {Disciple = 0, Mentor = 16}, Message = nil}
MentorSys[15] = {Reputation = {Disciple = 0, Mentor = 18}, Message = nil}
MentorSys[16] = {Reputation = {Disciple = 0, Mentor = 20}, Message = nil}
MentorSys[17] = {Reputation = {Disciple = 0, Mentor = 22}, Message = nil}
MentorSys[18] = {Reputation = {Disciple = 0, Mentor = 24}, Message = nil}
MentorSys[19] = {Reputation = {Disciple = 0, Mentor = 26}, Message = nil}
MentorSys[20] = {Reputation = {Disciple = 0, Mentor = 28}, Message = nil}
MentorSys[21] = {Reputation = {Disciple = 0, Mentor = 30}, Message = nil}
MentorSys[22] = {Reputation = {Disciple = 0, Mentor = 32}, Message = nil}
MentorSys[23] = {Reputation = {Disciple = 0, Mentor = 34}, Message = nil}
MentorSys[24] = {Reputation = {Disciple = 0, Mentor = 36}, Message = nil}
MentorSys[25] = {Reputation = {Disciple = 0, Mentor = 38}, Message = nil}
MentorSys[26] = {Reputation = {Disciple = 0, Mentor = 40}, Message = nil}
MentorSys[27] = {Reputation = {Disciple = 0, Mentor = 42}, Message = nil}
MentorSys[28] = {Reputation = {Disciple = 0, Mentor = 44}, Message = nil}
MentorSys[29] = {Reputation = {Disciple = 0, Mentor = 46}, Message = nil}
MentorSys[30] = {Reputation = {Disciple = 0, Mentor = 48}, Message = nil}
MentorSys[31] = {Reputation = {Disciple = 0, Mentor = 50}, Message = nil}
MentorSys[32] = {Reputation = {Disciple = 0, Mentor = 52}, Message = nil}
MentorSys[33] = {Reputation = {Disciple = 0, Mentor = 54}, Message = nil}
MentorSys[34] = {Reputation = {Disciple = 0, Mentor = 56}, Message = nil}
MentorSys[35] = {Reputation = {Disciple = 0, Mentor = 58}, Message = nil}
MentorSys[36] = {Reputation = {Disciple = 0, Mentor = 60}, Message = nil}
MentorSys[37] = {Reputation = {Disciple = 0, Mentor = 62}, Message = nil}
MentorSys[38] = {Reputation = {Disciple = 0, Mentor = 64}, Message = nil}
MentorSys[39] = {Reputation = {Disciple = 0, Mentor = 66}, Message = nil}
MentorSys[40] = {Reputation = {Disciple = 0, Mentor = 68}, Message = nil}
MentorSys[41] = {Reputation = {Disciple = 0, Mentor = 500}, Message = "Congratulations, you are now able to Mentorship!"}
MentorSys[42] = {Reputation = {Disciple = 0, Mentor = 75}, Message = nil}
MentorSys[43] = {Reputation = {Disciple = 0, Mentor = 80}, Message = nil}
MentorSys[44] = {Reputation = {Disciple = 0, Mentor = 86}, Message = nil}
MentorSys[45] = {Reputation = {Disciple = 0, Mentor = 93}, Message = nil}
MentorSys[46] = {Reputation = {Disciple = 0, Mentor = 101}, Message = nil}
MentorSys[47] = {Reputation = {Disciple = 0, Mentor = 110}, Message = nil}
MentorSys[48] = {Reputation = {Disciple = 0, Mentor = 120}, Message = nil}
MentorSys[49] = {Reputation = {Disciple = 0, Mentor = 131}, Message = nil}
MentorSys[50] = {Reputation = {Disciple = 0, Mentor = 143}, Message = nil}
MentorSys[51] = {Reputation = {Disciple = 0, Mentor = 156}, Message = nil}
MentorSys[52] = {Reputation = {Disciple = 0, Mentor = 170}, Message = nil}
MentorSys[53] = {Reputation = {Disciple = 0, Mentor = 185}, Message = nil}
MentorSys[54] = {Reputation = {Disciple = 0, Mentor = 201}, Message = nil}
MentorSys[55] = {Reputation = {Disciple = 0, Mentor = 218}, Message = nil}
MentorSys[56] = {Reputation = {Disciple = 0, Mentor = 236}, Message = nil}
MentorSys[57] = {Reputation = {Disciple = 0, Mentor = 255}, Message = nil}
MentorSys[58] = {Reputation = {Disciple = 0, Mentor = 275}, Message = nil}
MentorSys[59] = {Reputation = {Disciple = 0, Mentor = 296}, Message = nil}
MentorSys[60] = {Reputation = {Disciple = 0, Mentor = 318}, Message = nil}
MentorSys[61] = {Reputation = {Disciple = 0, Mentor = 341}, Message = nil}
MentorSys[62] = {Reputation = {Disciple = 0, Mentor = 365}, Message = nil}
MentorSys[63] = {Reputation = {Disciple = 0, Mentor = 390}, Message = nil}
MentorSys[64] = {Reputation = {Disciple = 0, Mentor = 416}, Message = nil}
MentorSys[65] = {Reputation = {Disciple = 0, Mentor = 443}, Message = nil}
MentorSys[66] = {Reputation = {Disciple = 0, Mentor = 471}, Message = nil}
MentorSys[67] = {Reputation = {Disciple = 0, Mentor = 500}, Message = nil}
MentorSys[68] = {Reputation = {Disciple = 0, Mentor = 530}, Message = nil}
MentorSys[69] = {Reputation = {Disciple = 0, Mentor = 561}, Message = nil}
MentorSys[70] = {Reputation = {Disciple = 0, Mentor = 593}, Message = nil}
MentorSys[71] = {Reputation = {Disciple = 0, Mentor = 626}, Message = nil}
MentorSys[72] = {Reputation = {Disciple = 0, Mentor = 660}, Message = nil}
MentorSys[73] = {Reputation = {Disciple = 0, Mentor = 695}, Message = nil}
MentorSys[74] = {Reputation = {Disciple = 0, Mentor = 731}, Message = nil}
MentorSys[75] = {Reputation = {Disciple = 0, Mentor = 768}, Message = nil}
MentorSys[76] = {Reputation = {Disciple = 0, Mentor = 806}, Message = nil}
MentorSys[77] = {Reputation = {Disciple = 0, Mentor = 845}, Message = nil}
MentorSys[78] = {Reputation = {Disciple = 0, Mentor = 885}, Message = nil}
MentorSys[79] = {Reputation = {Disciple = 0, Mentor = 926}, Message = nil}
MentorSys[80] = {Reputation = {Disciple = 0, Mentor = 968}, Message = nil}
MentorSys[81] = {Reputation = {Disciple = 0, Mentor = 1011}, Message = nil}
MentorSys[82] = {Reputation = {Disciple = 0, Mentor = 1055}, Message = nil}
MentorSys[83] = {Reputation = {Disciple = 0, Mentor = 1100}, Message = nil}
MentorSys[84] = {Reputation = {Disciple = 0, Mentor = 1146}, Message = nil}
MentorSys[85] = {Reputation = {Disciple = 0, Mentor = 1193}, Message = nil}
MentorSys[86] = {Reputation = {Disciple = 0, Mentor = 1241}, Message = nil}
MentorSys[87] = {Reputation = {Disciple = 0, Mentor = 1290}, Message = nil}
MentorSys[88] = {Reputation = {Disciple = 0, Mentor = 1340}, Message = nil}
MentorSys[89] = {Reputation = {Disciple = 0, Mentor = 1391}, Message = nil}
MentorSys[90] = {Reputation = {Disciple = 0, Mentor = 1443}, Message = nil}
MentorSys[91] = {Reputation = {Disciple = 0, Mentor = 1496}, Message = nil}
MentorSys[92] = {Reputation = {Disciple = 0, Mentor = 1550}, Message = nil}
MentorSys[93] = {Reputation = {Disciple = 0, Mentor = 1605}, Message = nil}
MentorSys[94] = {Reputation = {Disciple = 0, Mentor = 1661}, Message = nil}
MentorSys[95] = {Reputation = {Disciple = 0, Mentor = 1718}, Message = nil}
MentorSys[96] = {Reputation = {Disciple = 0, Mentor = 1776}, Message = nil}
MentorSys[97] = {Reputation = {Disciple = 0, Mentor = 1835}, Message = nil}
MentorSys[98] = {Reputation = {Disciple = 0, Mentor = 1895}, Message = nil}
MentorSys[99] = {Reputation = {Disciple = 0, Mentor = 1956}, Message = nil}
MentorSys[100] = {Reputation = {Disciple = 0, Mentor = 1956}, Message = nil}

TicketSys = {Enabled = true, Ticket = {}, Prohibited = {}}
TicketSys.Prohibited["garner2"] = {Name = "Chaos Argent", Active = true}
TicketSys.Prohibited["prisonisland"] = {Name = "Prison Island", Active = true}
TicketSys.Ticket[3141] = {Name = "", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[4602] = {Name = "Argent City", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[4603] = {Name = "Shaitan City", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[4604] = {Name = "Icicle Castle", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3828] = {Name = "Thundoria Castle", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3048] = {Name = "Thundoria Castle", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3831] = {Name = "Andes Forest Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3051] = {Name = "Andes Forest Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3847] = {Name = "Abandon Mine Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3073] = {Name = "Abandon Mine Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3832] = {Name = "Oasis Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3052] = {Name = "Oasis Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3833] = {Name = "Icespire Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3053] = {Name = "Icespire Haven", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3829] = {Name = "Thundoria Harbor", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3049] = {Name = "Thundoria Harbor", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3830] = {Name = "Sacred Snow Mountain", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3050] = {Name = "Sacred Snow Mountain", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3834] = {Name = "Lone Tower Entrance", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3054] = {Name = "Lone Tower Entrance", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3835] = {Name = "Barren Cavern Entrance", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3055] = {Name = "Barren Cavern Entrance", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3836] = {Name = "Abandoned Mine 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3056] = {Name = "Abandoned Mine 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3837] = {Name = "Silver Mine 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3057] = {Name = "Silver Mine 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3838] = {Name = "Silver Mine 3", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3058] = {Name = "Silver Mine 3", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3847] = {Name = "Abandon Mine 1", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3073] = {Name = "Abandon Mine 1", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3059] = {Name = "Lone Tower 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3839] = {Name = "Lone Tower 2", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3840] = {Name = "Lone Tower 3", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3060] = {Name = "Lone Tower 3", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3841] = {Name = "Lone Tower 4", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3070] = {Name = "Lone Tower 4", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3842] = {Name = "Lone Tower 5", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3071] = {Name = "Lone Tower 5", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3843] = {Name = "Lone Tower 6", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3072] = {Name = "Lone Tower 6", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[0332] = {Name = "Spring Town", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[3076] = {Name = "Spring Town", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[0583] = {Name = "Autumn Island", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[2445] = {Name = "Caribbean", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[2844] = {Name = "Abaddon 4", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[2447] = {Name = "Skeletar Isle", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[0563] = {Name = "Summer Island", Level = {Min = nil, Max = nil}}
TicketSys.Ticket[2491] = {Name = "Naval Base", Level = {Min = nil, Max = nil}}

UnsJewelVar = {}
UnsJewelVar[1] = {Active = true, Name = "Undead Sealed", JewelID = 13749, Gold = 200000, Items = {{3431, 5}, {3428, 5}, {3425, 10}}}
UnsJewelVar[2] = {Active = true, Name = "Earth Sealed", JewelID = 13750, Gold = 400000, Items = {{3432, 5}, {3426, 10}, {3429, 5}}}
UnsJewelVar[3] = {Active = true, Name = "Fire Sealed", JewelID = 13751, Gold = 600000, Items = {{3433, 5}, {3427, 10}, {3430, 10}}}
UnsJewelVar[4] = {Active = true, Name = "Ice Seal", JewelID = 13752, Gold = 800000, Items = {{3457, 3}, {3456, 10}, {3455, 10}}}
UnsJewelVar[5] = {Active = false, Name = "Snow Seal", JewelID = 13754, Gold = 1200000, Items = {}}
UnsJewelVar[6] = {Active = false, Name = "Holy Seal", JewelID = 13755, Gold = 1400000, Items = {}}
UnsJewelVar[7] = {Active = false, Name = "Water Seal", JewelID = 13756, Gold = 1600000, Items = {}}
UnsJewelVar[8] = {Active = false, Name = "Dusty Seal", JewelID = 13757, Gold = 1800000, Items = {}}
UnsJewelVar[9] = {Active = false, Name = "Darkness Seal", JewelID = 13758, Gold = 2000000, Items = {}}
UnsJewelVar[10] = {Active = false, Name = "Magic Seal", JewelID = 13759, Gold = 2200000, Items = {}}
UnsJewelVar[11] = {Active = false, Name = "Light Seal", JewelID = 13760, Gold = 2400000, Items = {}}
UnsJewelVar[12] = {Active = false, Name = "Thunder Seal", JewelID = 13761, Gold = 2600000, Items = {}}
UnsJewelVar[13] = {Active = false, Name = "Demonic Seal", JewelID = 13762, Gold = 2800000, Items = {}}
UnsJewelVar[14] = {Active = false, Name = "Ghost Seal", JewelID = 13763, Gold = 3000000, Items = {}}
UnsJewelVar[15] = {Active = false, Name = "Wind Seal", JewelID = 13764, Gold = 3200000, Items = {}}
UnsJewelVar[16] = {Active = false, Name = "Shadow Seal", JewelID = 13765, Gold = 3400000, Items = {}}
UnsJewelVar[17] = {Active = false, Name = "Frost Seal", JewelID = 13766, Gold = 3600000, Items = {}}
UnsJewelVar[18] = {Active = false, Name = "Hollow Seal", JewelID = 13767, Gold = 3800000, Items = {}}
UnsJewelVar[19] = {Active = false, Name = "Flame Seal", JewelID = 13768, Gold = 4000000, Items = {}}
UnsJewelVar[20] = {Active = false, Name = "Forest Seal", JewelID = 13769, Gold = 4200000, Items = {}}
UnsJewelVar[21] = {Active = false, Name = "Undine Seal", JewelID = 13770, Gold = 4400000, Items = {}}
UnsJewelVar[22] = {Active = false, Name = "Energy Seal", JewelID = 13771, Gold = 4600000, Items = {}}
UnsJewelVar[23] = {Active = false, Name = "Astral Seal", JewelID = 13772, Gold = 4800000, Items = {}}
UnsJewelVar[24] = {Active = false, Name = "Time Seal", JewelID = 13773, Gold = 5000000, Items = {}}
UnsJewelVar[25] = {Active = false, Name = "Sylph Seal", JewelID = 13774, Gold = 5200000, Items = {}}
UnsJewelVar[26] = {Active = false, Name = "Divine Seal", JewelID = 13775, Gold = 5400000, Items = {}}

JackpotM = {}
JackpotM.Bet = 855
JackpotM.UserGold = 1000000000
JackpotM.Items = {276, 277, 278, 279, 280, 3844, 3845, 849, 222, 223, 224, 225, 226, 885, 863, 862, 861, 860, 1012, 3094, 3096, 578}

Resources = {}
Resources.MaxLevel = 10
Resources.EXP = 1500
Resources.GoldItems = {207,208}
Resources.Uncommon = {}
Resources.Uncommon.Tree = {778}
Resources.Uncommon.Fish = {}
Resources.Uncommon.Boat = {}
Resources.Uncommon.Rock = {777}
-----------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------- ** Manufacturing, Crafting, & Cooking Vars ** ---------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
Manufacturing = {}
Manufacturing[1  ] = {Active = 1,	ItemID = 1067,	ItemLv = 10	,	Material1 = 4418,	Material2 = 3999,	Material3 = 1677,	Rad = 5,	GoodItem = 0} -- Crystal Cauldron
Manufacturing[2  ] = {Active = 1,	ItemID = 1068,	ItemLv = 10	,	Material1 = 4418,	Material2 = 3999,	Material3 = 1677,	Rad = 5,	GoodItem = 0} -- Black Hole Crystal
Manufacturing[3  ] = {Active = 1,	ItemID = 1069,	ItemLv = 10	,	Material1 = 1708,	Material2 = 3999,	Material3 = 1677,	Rad = 5,	GoodItem = 0} -- Anti Matter Crystal
Manufacturing[4  ] = {Active = 1,	ItemID = 1070,	ItemLv = 10	,	Material1 = 1708,	Material2 = 3999,	Material3 = 1677,	Rad = 5,	GoodItem = 0} -- Particle Crystal
Manufacturing[5  ] = {Active = 1,	ItemID = 1135,	ItemLv = 10	,	Material1 = 4488,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[6  ] = {Active = 1,	ItemID = 2372,	ItemLv = 10	,	Material1 = 1649,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv1
Manufacturing[7  ] = {Active = 1,	ItemID = 1137,	ItemLv = 10	,	Material1 = 4340,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Radiation Lv1
Manufacturing[8  ] = {Active = 1,	ItemID = 1146,	ItemLv = 10	,	Material1 = 4340,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv1
Manufacturing[9  ] = {Active = 1,	ItemID = 1145,	ItemLv = 10	,	Material1 = 4488,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv1
Manufacturing[10 ] = {Active = 1,	ItemID = 1136,	ItemLv = 10	,	Material1 = 1649,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv1
Manufacturing[11 ] = {Active = 1,	ItemID = 1135,	ItemLv = 10	,	Material1 = 4488,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[12 ] = {Active = 1,	ItemID = 1135,	ItemLv = 10	,	Material1 = 4475,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[13 ] = {Active = 1,	ItemID = 1138,	ItemLv = 10	,	Material1 = 1649,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[14 ] = {Active = 1,	ItemID = 1138,	ItemLv = 10	,	Material1 = 4475,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[15 ] = {Active = 1,	ItemID = 1150,	ItemLv = 10	,	Material1 = 1696,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv1
Manufacturing[16 ] = {Active = 1,	ItemID = 1152,	ItemLv = 10	,	Material1 = 4490,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv1
Manufacturing[17 ] = {Active = 1,	ItemID = 2673,	ItemLv = 10	,	Material1 = 4388,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv1
Manufacturing[18 ] = {Active = 1,	ItemID = 1139,	ItemLv = 10	,	Material1 = 4336,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv1
Manufacturing[19 ] = {Active = 1,	ItemID = 1139,	ItemLv = 10	,	Material1 = 4344,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv1
Manufacturing[20 ] = {Active = 1,	ItemID = 2677,	ItemLv = 10	,	Material1 = 4423,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv1
Manufacturing[21 ] = {Active = 1,	ItemID = 1140,	ItemLv = 10	,	Material1 = 4420,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv1
Manufacturing[22 ] = {Active = 1,	ItemID = 1141,	ItemLv = 10	,	Material1 = 4337,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv1
Manufacturing[23 ] = {Active = 1,	ItemID = 1142,	ItemLv = 10	,	Material1 = 4339,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv1
Manufacturing[24 ] = {Active = 1,	ItemID = 1142,	ItemLv = 10	,	Material1 = 4342,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv1
Manufacturing[25 ] = {Active = 1,	ItemID = 1143,	ItemLv = 10	,	Material1 = 4423,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv1
Manufacturing[26 ] = {Active = 1,	ItemID = 1151,	ItemLv = 10	,	Material1 = 4335,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Food Generation Lv1
Manufacturing[27 ] = {Active = 1,	ItemID = 2678,	ItemLv = 10	,	Material1 = 4490,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Water Mine Lv1
Manufacturing[28 ] = {Active = 1,	ItemID = 1138,	ItemLv = 10	,	Material1 = 4427,	Material2 = 3999,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[29 ] = {Active = 1,	ItemID = 2236,	ItemLv = 20	,	Material1 = 1747,	Material2 = 4000,	Material3 = 2640,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[30 ] = {Active = 1,	ItemID = 2236,	ItemLv = 20	,	Material1 = 4370,	Material2 = 4000,	Material3 = 2619,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[31 ] = {Active = 1,	ItemID = 2236,	ItemLv = 20	,	Material1 = 3929,	Material2 = 4000,	Material3 = 2617,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[32 ] = {Active = 1,	ItemID = 2236,	ItemLv = 20	,	Material1 = 4444,	Material2 = 4000,	Material3 = 2641,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[33 ] = {Active = 1,	ItemID = 1135,	ItemLv = 20	,	Material1 = 1627,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[34 ] = {Active = 1,	ItemID = 2372,	ItemLv = 20	,	Material1 = 1629,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv1
Manufacturing[35 ] = {Active = 1,	ItemID = 1137,	ItemLv = 20	,	Material1 = 4441,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Radiation Lv1
Manufacturing[36 ] = {Active = 1,	ItemID = 1146,	ItemLv = 20	,	Material1 = 4441,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv1
Manufacturing[37 ] = {Active = 1,	ItemID = 1145,	ItemLv = 20	,	Material1 = 1627,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv1
Manufacturing[38 ] = {Active = 1,	ItemID = 1136,	ItemLv = 20	,	Material1 = 1629,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv1
Manufacturing[39 ] = {Active = 1,	ItemID = 1135,	ItemLv = 20	,	Material1 = 1627,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[40 ] = {Active = 1,	ItemID = 1135,	ItemLv = 20	,	Material1 = 1838,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Grenade Lv1
Manufacturing[41 ] = {Active = 1,	ItemID = 1138,	ItemLv = 20	,	Material1 = 1629,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[42 ] = {Active = 1,	ItemID = 1138,	ItemLv = 20	,	Material1 = 1838,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[43 ] = {Active = 1,	ItemID = 1150,	ItemLv = 20	,	Material1 = 4346,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv1
Manufacturing[44 ] = {Active = 1,	ItemID = 1152,	ItemLv = 20	,	Material1 = 4383,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv1
Manufacturing[45 ] = {Active = 1,	ItemID = 2673,	ItemLv = 20	,	Material1 = 4356,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv1
Manufacturing[46 ] = {Active = 1,	ItemID = 1139,	ItemLv = 20	,	Material1 = 4379,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv1
Manufacturing[47 ] = {Active = 1,	ItemID = 1139,	ItemLv = 20	,	Material1 = 4377,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv1
Manufacturing[48 ] = {Active = 1,	ItemID = 2677,	ItemLv = 20	,	Material1 = 4465,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv1
Manufacturing[49 ] = {Active = 1,	ItemID = 1140,	ItemLv = 20	,	Material1 = 4432,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv1
Manufacturing[50 ] = {Active = 1,	ItemID = 1141,	ItemLv = 20	,	Material1 = 4376,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv1
Manufacturing[51 ] = {Active = 1,	ItemID = 1142,	ItemLv = 20	,	Material1 = 4373,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv1
Manufacturing[52 ] = {Active = 1,	ItemID = 1142,	ItemLv = 20	,	Material1 = 4480,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv1
Manufacturing[53 ] = {Active = 1,	ItemID = 1143,	ItemLv = 20	,	Material1 = 4493,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv1
Manufacturing[54 ] = {Active = 1,	ItemID = 1151,	ItemLv = 20	,	Material1 = 4430,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Food Generation Lv1
Manufacturing[55 ] = {Active = 1,	ItemID = 2678,	ItemLv = 20	,	Material1 = 4480,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Water Mine Lv1
Manufacturing[56 ] = {Active = 1,	ItemID = 1138,	ItemLv = 20	,	Material1 = 4371,	Material2 = 4000,	Material3 = 3116,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv1
Manufacturing[57 ] = {Active = 1,	ItemID = 2236,	ItemLv = 30	,	Material1 = 1739,	Material2 = 4001,	Material3 = 2642,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[58 ] = {Active = 1,	ItemID = 2236,	ItemLv = 30	,	Material1 = 4504,	Material2 = 4001,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[59 ] = {Active = 1,	ItemID = 2236,	ItemLv = 30	,	Material1 = 4498,	Material2 = 4001,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[60 ] = {Active = 1,	ItemID = 2236,	ItemLv = 30	,	Material1 = 4454,	Material2 = 4001,	Material3 = 2643,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[61 ] = {Active = 1,	ItemID = 2721,	ItemLv = 30	,	Material1 = 4931,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Radiation Lv2
Manufacturing[62 ] = {Active = 1,	ItemID = 2373,	ItemLv = 30	,	Material1 = 4951,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv2
Manufacturing[63 ] = {Active = 1,	ItemID = 2721,	ItemLv = 30	,	Material1 = 4951,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Radiation Lv2
Manufacturing[64 ] = {Active = 1,	ItemID = 2730,	ItemLv = 30	,	Material1 = 4963,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv2
Manufacturing[65 ] = {Active = 1,	ItemID = 2729,	ItemLv = 30	,	Material1 = 1652,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv2
Manufacturing[66 ] = {Active = 1,	ItemID = 2720,	ItemLv = 30	,	Material1 = 4931,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv2
Manufacturing[67 ] = {Active = 1,	ItemID = 2719,	ItemLv = 30	,	Material1 = 4924,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Grenade Lv2
Manufacturing[68 ] = {Active = 1,	ItemID = 2719,	ItemLv = 30	,	Material1 = 4925,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Grenade Lv2
Manufacturing[69 ] = {Active = 1,	ItemID = 2722,	ItemLv = 30	,	Material1 = 4945,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv2
Manufacturing[70 ] = {Active = 1,	ItemID = 2724,	ItemLv = 30	,	Material1 = 4924,	Material2 = 4001,	Material3 = 2605,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv2
Manufacturing[71 ] = {Active = 1,	ItemID = 2734,	ItemLv = 30	,	Material1 = 1624,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv2
Manufacturing[72 ] = {Active = 1,	ItemID = 2736,	ItemLv = 30	,	Material1 = 4537,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv2
Manufacturing[73 ] = {Active = 1,	ItemID = 2737,	ItemLv = 30	,	Material1 = 1624,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv2
Manufacturing[74 ] = {Active = 1,	ItemID = 2723,	ItemLv = 30	,	Material1 = 4930,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv2
Manufacturing[75 ] = {Active = 1,	ItemID = 2741,	ItemLv = 30	,	Material1 = 4950,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv2
Manufacturing[76 ] = {Active = 1,	ItemID = 2741,	ItemLv = 30	,	Material1 = 4534,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv2
Manufacturing[77 ] = {Active = 1,	ItemID = 2724,	ItemLv = 30	,	Material1 = 4950,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv2
Manufacturing[78 ] = {Active = 1,	ItemID = 2725,	ItemLv = 30	,	Material1 = 4534,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv2
Manufacturing[79 ] = {Active = 1,	ItemID = 2726,	ItemLv = 30	,	Material1 = 1713,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv2
Manufacturing[80 ] = {Active = 1,	ItemID = 2727,	ItemLv = 30	,	Material1 = 4930,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv2
Manufacturing[81 ] = {Active = 1,	ItemID = 2727,	ItemLv = 30	,	Material1 = 4521,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv2
Manufacturing[82 ] = {Active = 1,	ItemID = 2735,	ItemLv = 30	,	Material1 = 1688,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Food Generation Lv2
Manufacturing[83 ] = {Active = 1,	ItemID = 2742,	ItemLv = 30	,	Material1 = 1713,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Water Mine Lv2
Manufacturing[84 ] = {Active = 1,	ItemID = 2722,	ItemLv = 30	,	Material1 = 1688,	Material2 = 4001,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv2
Manufacturing[85 ] = {Active = 1,	ItemID = 2236,	ItemLv = 40	,	Material1 = 4523,	Material2 = 4002,	Material3 = 2642,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[86 ] = {Active = 1,	ItemID = 2236,	ItemLv = 40	,	Material1 = 1210,	Material2 = 4002,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[87 ] = {Active = 1,	ItemID = 2236,	ItemLv = 40	,	Material1 = 4526,	Material2 = 4002,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[88 ] = {Active = 1,	ItemID = 2236,	ItemLv = 40	,	Material1 = 1301,	Material2 = 4002,	Material3 = 2643,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[89 ] = {Active = 1,	ItemID = 2721,	ItemLv = 40	,	Material1 = 1309,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Radiation Lv2
Manufacturing[90 ] = {Active = 1,	ItemID = 2373,	ItemLv = 40	,	Material1 = 1327,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv2
Manufacturing[91 ] = {Active = 1,	ItemID = 2721,	ItemLv = 40	,	Material1 = 1289,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Radiation Lv2
Manufacturing[92 ] = {Active = 1,	ItemID = 2730,	ItemLv = 40	,	Material1 = 4988,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv2
Manufacturing[93 ] = {Active = 1,	ItemID = 2729,	ItemLv = 40	,	Material1 = 1327,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv2
Manufacturing[94 ] = {Active = 1,	ItemID = 2720,	ItemLv = 40	,	Material1 = 4988,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv2
Manufacturing[95 ] = {Active = 1,	ItemID = 2719,	ItemLv = 40	,	Material1 = 4969,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Grenade Lv2
Manufacturing[96 ] = {Active = 1,	ItemID = 2719,	ItemLv = 40	,	Material1 = 1309,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Grenade Lv2
Manufacturing[97 ] = {Active = 1,	ItemID = 2722,	ItemLv = 40	,	Material1 = 4969,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv2
Manufacturing[98 ] = {Active = 1,	ItemID = 2724,	ItemLv = 40	,	Material1 = 1289,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv2
Manufacturing[99 ] = {Active = 1,	ItemID = 2734,	ItemLv = 40	,	Material1 = 4968,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv2
Manufacturing[100] = {Active = 1,	ItemID = 2736,	ItemLv = 40	,	Material1 = 1176,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv2
Manufacturing[101] = {Active = 1,	ItemID = 2737,	ItemLv = 40	,	Material1 = 4542,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv2
Manufacturing[102] = {Active = 1,	ItemID = 2723,	ItemLv = 40	,	Material1 = 4987,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv2
Manufacturing[103] = {Active = 1,	ItemID = 2741,	ItemLv = 40	,	Material1 = 4908,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv2
Manufacturing[104] = {Active = 1,	ItemID = 2741,	ItemLv = 40	,	Material1 = 4524,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv2
Manufacturing[105] = {Active = 1,	ItemID = 2724,	ItemLv = 40	,	Material1 = 1616,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv2
Manufacturing[106] = {Active = 1,	ItemID = 2725,	ItemLv = 40	,	Material1 = 4908,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv2
Manufacturing[107] = {Active = 1,	ItemID = 2726,	ItemLv = 40	,	Material1 = 4524,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv2
Manufacturing[108] = {Active = 1,	ItemID = 2727,	ItemLv = 40	,	Material1 = 1199,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv2
Manufacturing[109] = {Active = 1,	ItemID = 2727,	ItemLv = 40	,	Material1 = 1613,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv2
Manufacturing[110] = {Active = 1,	ItemID = 2735,	ItemLv = 40	,	Material1 = 4539,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Food Generation Lv2
Manufacturing[111] = {Active = 1,	ItemID = 2742,	ItemLv = 40	,	Material1 = 1613,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Water Mine Lv2
Manufacturing[112] = {Active = 1,	ItemID = 2722,	ItemLv = 40	,	Material1 = 4746,	Material2 = 4002,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv2
Manufacturing[113] = {Active = 1,	ItemID = 2236,	ItemLv = 50	,	Material1 = 1211,	Material2 = 4003,	Material3 = 2642,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[114] = {Active = 1,	ItemID = 2236,	ItemLv = 50	,	Material1 = 4947,	Material2 = 4003,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[115] = {Active = 1,	ItemID = 2236,	ItemLv = 50	,	Material1 = 1302,	Material2 = 4003,	Material3 = 2622,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[116] = {Active = 1,	ItemID = 2236,	ItemLv = 50	,	Material1 = 4879,	Material2 = 4003,	Material3 = 2643,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[117] = {Active = 1,	ItemID = 2750,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv3
Manufacturing[118] = {Active = 1,	ItemID = 2375,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv3
Manufacturing[119] = {Active = 1,	ItemID = 2745,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Radiation Lv3
Manufacturing[120] = {Active = 1,	ItemID = 2754,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv3
Manufacturing[121] = {Active = 1,	ItemID = 2753,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv3
Manufacturing[122] = {Active = 1,	ItemID = 2744,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv3
Manufacturing[123] = {Active = 1,	ItemID = 2743,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Grenade Lv3
Manufacturing[124] = {Active = 1,	ItemID = 2743,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Grenade Lv3
Manufacturing[125] = {Active = 1,	ItemID = 2746,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv3
Manufacturing[126] = {Active = 1,	ItemID = 2758,	ItemLv = 50	,	Material1 = 1346,	Material2 = 4003,	Material3 = 2668,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv3
Manufacturing[127] = {Active = 1,	ItemID = 2758,	ItemLv = 50	,	Material1 = 1710,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv3
Manufacturing[128] = {Active = 1,	ItemID = 2760,	ItemLv = 50	,	Material1 = 4883,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv3
Manufacturing[129] = {Active = 1,	ItemID = 2761,	ItemLv = 50	,	Material1 = 1182,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv3
Manufacturing[130] = {Active = 1,	ItemID = 2747,	ItemLv = 50	,	Material1 = 1334,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv3
Manufacturing[131] = {Active = 1,	ItemID = 2765,	ItemLv = 50	,	Material1 = 1179,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv3
Manufacturing[132] = {Active = 1,	ItemID = 2765,	ItemLv = 50	,	Material1 = 1361,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv3
Manufacturing[133] = {Active = 1,	ItemID = 2748,	ItemLv = 50	,	Material1 = 4979,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv3
Manufacturing[134] = {Active = 1,	ItemID = 2749,	ItemLv = 50	,	Material1 = 1237,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv3
Manufacturing[135] = {Active = 1,	ItemID = 2750,	ItemLv = 50	,	Material1 = 1353,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv3
Manufacturing[136] = {Active = 1,	ItemID = 2751,	ItemLv = 50	,	Material1 = 167	,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv3
Manufacturing[137] = {Active = 1,	ItemID = 2751,	ItemLv = 50	,	Material1 = 4998,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv3
Manufacturing[138] = {Active = 1,	ItemID = 2759,	ItemLv = 50	,	Material1 = 1219,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Food Generation Lv3
Manufacturing[139] = {Active = 1,	ItemID = 2766,	ItemLv = 50	,	Material1 = 1183,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Water Mine Lv3
Manufacturing[140] = {Active = 1,	ItemID = 2746,	ItemLv = 50	,	Material1 = 1364,	Material2 = 4003,	Material3 = 2608,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv3
Manufacturing[141] = {Active = 1,	ItemID = 2236,	ItemLv = 60	,	Material1 = 3386,	Material2 = 4004,	Material3 = 2644,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[142] = {Active = 1,	ItemID = 2236,	ItemLv = 60	,	Material1 = 1790,	Material2 = 4004,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[143] = {Active = 1,	ItemID = 2236,	ItemLv = 60	,	Material1 = 1788,	Material2 = 4004,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[144] = {Active = 1,	ItemID = 2236,	ItemLv = 60	,	Material1 = 1791,	Material2 = 4004,	Material3 = 2649,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[145] = {Active = 1,	ItemID = 2750,	ItemLv = 60	,	Material1 = 1294,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv3
Manufacturing[146] = {Active = 1,	ItemID = 2375,	ItemLv = 60	,	Material1 = 1313,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv3
Manufacturing[147] = {Active = 1,	ItemID = 2745,	ItemLv = 60	,	Material1 = 1332,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Radiation Lv3
Manufacturing[148] = {Active = 1,	ItemID = 2754,	ItemLv = 60	,	Material1 = 1332,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv3
Manufacturing[149] = {Active = 1,	ItemID = 2753,	ItemLv = 60	,	Material1 = 1294,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv3
Manufacturing[150] = {Active = 1,	ItemID = 2744,	ItemLv = 60	,	Material1 = 1313,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv3
Manufacturing[151] = {Active = 1,	ItemID = 2743,	ItemLv = 60	,	Material1 = 1294,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Grenade Lv3
Manufacturing[152] = {Active = 1,	ItemID = 2743,	ItemLv = 60	,	Material1 = 1351,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Grenade Lv3
Manufacturing[153] = {Active = 1,	ItemID = 2746,	ItemLv = 60	,	Material1 = 1313,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv3
Manufacturing[154] = {Active = 1,	ItemID = 2758,	ItemLv = 60	,	Material1 = 1351,	Material2 = 4004,	Material3 = 2606,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv3
Manufacturing[155] = {Active = 1,	ItemID = 2758,	ItemLv = 60	,	Material1 = 1684,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv3
Manufacturing[156] = {Active = 1,	ItemID = 2760,	ItemLv = 60	,	Material1 = 1350,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv3
Manufacturing[157] = {Active = 1,	ItemID = 2761,	ItemLv = 60	,	Material1 = 1684,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv3
Manufacturing[158] = {Active = 1,	ItemID = 2747,	ItemLv = 60	,	Material1 = 1331,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv3
Manufacturing[159] = {Active = 1,	ItemID = 2765,	ItemLv = 60	,	Material1 = 1350,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv3
Manufacturing[160] = {Active = 1,	ItemID = 2765,	ItemLv = 60	,	Material1 = 1684,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv3
Manufacturing[161] = {Active = 1,	ItemID = 2748,	ItemLv = 60	,	Material1 = 1350,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv3
Manufacturing[162] = {Active = 1,	ItemID = 2749,	ItemLv = 60	,	Material1 = 1684,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv3
Manufacturing[163] = {Active = 1,	ItemID = 2750,	ItemLv = 60	,	Material1 = 1221,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv3
Manufacturing[164] = {Active = 1,	ItemID = 2751,	ItemLv = 60	,	Material1 = 1331,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv3
Manufacturing[165] = {Active = 1,	ItemID = 2751,	ItemLv = 60	,	Material1 = 1331,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv3
Manufacturing[166] = {Active = 1,	ItemID = 2759,	ItemLv = 60	,	Material1 = 1221,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Food Generation Lv3
Manufacturing[167] = {Active = 1,	ItemID = 2766,	ItemLv = 60	,	Material1 = 1221,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Water Mine Lv3
Manufacturing[168] = {Active = 1,	ItemID = 2746,	ItemLv = 60	,	Material1 = 1221,	Material2 = 4004,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv3
Manufacturing[169] = {Active = 1,	ItemID = 2236,	ItemLv = 70	,	Material1 = 1702,	Material2 = 4005,	Material3 = 2644,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[170] = {Active = 1,	ItemID = 2236,	ItemLv = 70	,	Material1 = 1702,	Material2 = 4005,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[171] = {Active = 1,	ItemID = 2236,	ItemLv = 70	,	Material1 = 1702,	Material2 = 4005,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[172] = {Active = 1,	ItemID = 2236,	ItemLv = 70	,	Material1 = 1702,	Material2 = 4005,	Material3 = 2649,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[173] = {Active = 1,	ItemID = 2769,	ItemLv = 70	,	Material1 = 4716,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Radiation Lv4
Manufacturing[174] = {Active = 1,	ItemID = 2379,	ItemLv = 70	,	Material1 = 3067,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv4
Manufacturing[175] = {Active = 1,	ItemID = 2769,	ItemLv = 70	,	Material1 = 1264,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Radiation Lv4
Manufacturing[176] = {Active = 1,	ItemID = 2778,	ItemLv = 70	,	Material1 = 1261,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv4
Manufacturing[177] = {Active = 1,	ItemID = 2777,	ItemLv = 70	,	Material1 = 1269,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv4
Manufacturing[178] = {Active = 1,	ItemID = 2768,	ItemLv = 70	,	Material1 = 1264,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv4
Manufacturing[179] = {Active = 1,	ItemID = 2767,	ItemLv = 70	,	Material1 = 1261,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Grenade Lv4
Manufacturing[180] = {Active = 1,	ItemID = 2767,	ItemLv = 70	,	Material1 = 1269,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Grenade Lv4
Manufacturing[181] = {Active = 1,	ItemID = 2770,	ItemLv = 70	,	Material1 = 4716,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv4
Manufacturing[182] = {Active = 1,	ItemID = 2782,	ItemLv = 70	,	Material1 = 3067,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv4
Manufacturing[183] = {Active = 1,	ItemID = 2782,	ItemLv = 70	,	Material1 = 4716,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv4
Manufacturing[184] = {Active = 1,	ItemID = 2784,	ItemLv = 70	,	Material1 = 3067,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv4
Manufacturing[185] = {Active = 1,	ItemID = 2785,	ItemLv = 70	,	Material1 = 1264,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv4
Manufacturing[186] = {Active = 1,	ItemID = 2771,	ItemLv = 70	,	Material1 = 1264,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv4
Manufacturing[187] = {Active = 1,	ItemID = 2789,	ItemLv = 70	,	Material1 = 4891,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv4
Manufacturing[188] = {Active = 1,	ItemID = 2789,	ItemLv = 70	,	Material1 = 4716,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv4
Manufacturing[189] = {Active = 1,	ItemID = 2772,	ItemLv = 70	,	Material1 = 1261,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv4
Manufacturing[190] = {Active = 1,	ItemID = 2773,	ItemLv = 70	,	Material1 = 1269,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv4
Manufacturing[191] = {Active = 1,	ItemID = 2774,	ItemLv = 70	,	Material1 = 4891,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv4
Manufacturing[192] = {Active = 1,	ItemID = 2775,	ItemLv = 70	,	Material1 = 1269,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv4
Manufacturing[193] = {Active = 1,	ItemID = 2775,	ItemLv = 70	,	Material1 = 4716,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv4
Manufacturing[194] = {Active = 1,	ItemID = 2783,	ItemLv = 70	,	Material1 = 3067,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Food Generation Lv4
Manufacturing[195] = {Active = 1,	ItemID = 2790,	ItemLv = 70	,	Material1 = 3067,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Water Mine Lv4
Manufacturing[196] = {Active = 1,	ItemID = 2770,	ItemLv = 70	,	Material1 = 1261,	Material2 = 4005,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv4
Manufacturing[197] = {Active = 1,	ItemID = 2236,	ItemLv = 80	,	Material1 = 1175,	Material2 = 4006,	Material3 = 2644,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[198] = {Active = 1,	ItemID = 2236,	ItemLv = 80	,	Material1 = 1357,	Material2 = 4006,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[199] = {Active = 1,	ItemID = 2236,	ItemLv = 80	,	Material1 = 1266,	Material2 = 4006,	Material3 = 2624,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[200] = {Active = 1,	ItemID = 2236,	ItemLv = 80	,	Material1 = 2490,	Material2 = 4006,	Material3 = 2649,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[201] = {Active = 1,	ItemID = 2769,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Radiation Lv4
Manufacturing[202] = {Active = 1,	ItemID = 2379,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv4
Manufacturing[203] = {Active = 1,	ItemID = 2769,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Radiation Lv4
Manufacturing[204] = {Active = 1,	ItemID = 2778,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv4
Manufacturing[205] = {Active = 1,	ItemID = 2777,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv4
Manufacturing[206] = {Active = 1,	ItemID = 2768,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv4
Manufacturing[207] = {Active = 1,	ItemID = 2767,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Grenade Lv4
Manufacturing[208] = {Active = 1,	ItemID = 2767,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Grenade Lv4
Manufacturing[209] = {Active = 1,	ItemID = 2770,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv4
Manufacturing[210] = {Active = 1,	ItemID = 2782,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2667,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv4
Manufacturing[211] = {Active = 1,	ItemID = 2782,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv4
Manufacturing[212] = {Active = 1,	ItemID = 2784,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv4
Manufacturing[213] = {Active = 1,	ItemID = 2785,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv4
Manufacturing[214] = {Active = 1,	ItemID = 2771,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv4
Manufacturing[215] = {Active = 1,	ItemID = 2789,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv4
Manufacturing[216] = {Active = 1,	ItemID = 2789,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv4
Manufacturing[217] = {Active = 1,	ItemID = 2772,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv4
Manufacturing[218] = {Active = 1,	ItemID = 2773,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv4
Manufacturing[219] = {Active = 1,	ItemID = 2774,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv4
Manufacturing[220] = {Active = 1,	ItemID = 2775,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv4
Manufacturing[221] = {Active = 1,	ItemID = 2775,	ItemLv = 80	,	Material1 = 1716,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv4
Manufacturing[222] = {Active = 1,	ItemID = 2783,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Food Generation Lv4
Manufacturing[223] = {Active = 1,	ItemID = 2790,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Water Mine Lv4
Manufacturing[224] = {Active = 1,	ItemID = 2770,	ItemLv = 80	,	Material1 = 1492,	Material2 = 4006,	Material3 = 2609,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv4
Manufacturing[225] = {Active = 1,	ItemID = 2236,	ItemLv = 90	,	Material1 = 3065,	Material2 = 4007,	Material3 = 2589,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[226] = {Active = 1,	ItemID = 2236,	ItemLv = 90	,	Material1 = 3065,	Material2 = 4007,	Material3 = 2589,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[227] = {Active = 1,	ItemID = 2236,	ItemLv = 90	,	Material1 = 3065,	Material2 = 4007,	Material3 = 2589,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[228] = {Active = 1,	ItemID = 2236,	ItemLv = 90	,	Material1 = 3065,	Material2 = 4007,	Material3 = 2589,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[229] = {Active = 1,	ItemID = 2793,	ItemLv = 90	,	Material1 = 4037,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Radiation Lv5
Manufacturing[230] = {Active = 1,	ItemID = 2380,	ItemLv = 90	,	Material1 = 3463,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv5
Manufacturing[231] = {Active = 1,	ItemID = 2793,	ItemLv = 90	,	Material1 = 4037,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Radiation Lv5
Manufacturing[232] = {Active = 1,	ItemID = 2802,	ItemLv = 90	,	Material1 = 3463,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv5
Manufacturing[233] = {Active = 1,	ItemID = 2801,	ItemLv = 90	,	Material1 = 4037,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv5
Manufacturing[234] = {Active = 1,	ItemID = 2792,	ItemLv = 90	,	Material1 = 3463,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv5
Manufacturing[235] = {Active = 1,	ItemID = 2791,	ItemLv = 90	,	Material1 = 4037,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Grenade Lv5
Manufacturing[236] = {Active = 1,	ItemID = 2791,	ItemLv = 90	,	Material1 = 3463,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Grenade Lv5
Manufacturing[237] = {Active = 1,	ItemID = 2794,	ItemLv = 90	,	Material1 = 4037,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv5
Manufacturing[238] = {Active = 1,	ItemID = 2806,	ItemLv = 90	,	Material1 = 3463,	Material2 = 4007,	Material3 = 2607,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv5
Manufacturing[239] = {Active = 1,	ItemID = 2806,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv5
Manufacturing[240] = {Active = 1,	ItemID = 2808,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv5
Manufacturing[241] = {Active = 1,	ItemID = 2809,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv5
Manufacturing[242] = {Active = 1,	ItemID = 2795,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv5
Manufacturing[243] = {Active = 1,	ItemID = 2796,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv5
Manufacturing[244] = {Active = 1,	ItemID = 2813,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv5
Manufacturing[245] = {Active = 1,	ItemID = 2796,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv5
Manufacturing[246] = {Active = 1,	ItemID = 2797,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv5
Manufacturing[247] = {Active = 1,	ItemID = 2798,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv5
Manufacturing[248] = {Active = 1,	ItemID = 2799,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv5
Manufacturing[249] = {Active = 1,	ItemID = 2799,	ItemLv = 90	,	Material1 = 1711,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv5
Manufacturing[250] = {Active = 1,	ItemID = 2807,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Food Generation Lv5
Manufacturing[251] = {Active = 1,	ItemID = 2814,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Water Mine Lv5
Manufacturing[252] = {Active = 1,	ItemID = 2794,	ItemLv = 90	,	Material1 = 1758,	Material2 = 4007,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv5
Manufacturing[253] = {Active = 1,	ItemID = 2236,	ItemLv = 100,	Material1 = 1797,	Material2 = 4008,	Material3 = 2665,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[254] = {Active = 1,	ItemID = 2236,	ItemLv = 100,	Material1 = 1797,	Material2 = 4008,	Material3 = 2665,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[255] = {Active = 1,	ItemID = 2236,	ItemLv = 100,	Material1 = 1797,	Material2 = 4008,	Material3 = 2665,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[256] = {Active = 1,	ItemID = 2236,	ItemLv = 100,	Material1 = 1797,	Material2 = 4008,	Material3 = 2665,	Rad = 5,	GoodItem = 0} -- Repair Tool
Manufacturing[257] = {Active = 1,	ItemID = 2793,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Radiation Lv5
Manufacturing[258] = {Active = 1,	ItemID = 2380,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Exploding Lamb Lv5
Manufacturing[259] = {Active = 1,	ItemID = 2793,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Radiation Lv5
Manufacturing[260] = {Active = 1,	ItemID = 2802,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Carrion Ball Lv5
Manufacturing[261] = {Active = 1,	ItemID = 2801,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Sand Bag Lv5
Manufacturing[262] = {Active = 1,	ItemID = 2792,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Flash Bomb Lv5
Manufacturing[263] = {Active = 1,	ItemID = 2791,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Grenade Lv5
Manufacturing[264] = {Active = 1,	ItemID = 2791,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Grenade Lv5
Manufacturing[265] = {Active = 1,	ItemID = 2794,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv5
Manufacturing[266] = {Active = 1,	ItemID = 2806,	ItemLv = 100,	Material1 = 4038,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv5
Manufacturing[267] = {Active = 1,	ItemID = 2806,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Hull Repair Lv5
Manufacturing[268] = {Active = 1,	ItemID = 2808,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Carrion Bullet Lv5
Manufacturing[269] = {Active = 1,	ItemID = 2809,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Mirage Generator Lv5
Manufacturing[270] = {Active = 1,	ItemID = 2795,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Accelerator Lv5
Manufacturing[271] = {Active = 1,	ItemID = 2796,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv5
Manufacturing[272] = {Active = 1,	ItemID = 2813,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Chain Bullet Lv5
Manufacturing[273] = {Active = 1,	ItemID = 2796,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Atomizer Lv5
Manufacturing[274] = {Active = 1,	ItemID = 2797,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Penetrator Lv5
Manufacturing[275] = {Active = 1,	ItemID = 2798,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Impaler Lv5
Manufacturing[276] = {Active = 1,	ItemID = 2799,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv5
Manufacturing[277] = {Active = 1,	ItemID = 2799,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Ship Flamer Lv5
Manufacturing[278] = {Active = 1,	ItemID = 2807,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Food Generation Lv5
Manufacturing[279] = {Active = 1,	ItemID = 2814,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Water Mine Lv5
Manufacturing[280] = {Active = 1,	ItemID = 2794,	ItemLv = 100,	Material1 = 1626,	Material2 = 4008,	Material3 = 2610,	Rad = 1,	GoodItem = 0} -- Soul Detector Lv5

Crafting = {}
Crafting[1  ] = {Active = 1,	ItemID = 635	,	ItemLv = 10	,	Material1 = 4427	,	Material2 = 4415	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Lv 1 Strike Coral
Crafting[2  ] = {Active = 1,	ItemID = 817	,	ItemLv = 10	,	Material1 = 4342	,	Material2 = 4029	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Lv 1 Wind Coral
Crafting[3  ] = {Active = 1,	ItemID = 867	,	ItemLv = 10	,	Material1 = 4344	,	Material2 = 3368	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Lv 1 Thunder Coral
Crafting[4  ] = {Active = 1,	ItemID = 872	,	ItemLv = 10	,	Material1 = 4339	,	Material2 = 4415	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Lv 1 Fog Coral
Crafting[5  ] = {Active = 1,	ItemID = 1388	,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 4415	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Short Blade
Crafting[6  ] = {Active = 1,	ItemID = 1		,	ItemLv = 10	,	Material1 = 1611	,	Material2 = 4029	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Short Sword
Crafting[7  ] = {Active = 1,	ItemID = 25		,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 3368	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Short Bow
Crafting[8  ] = {Active = 1,	ItemID = 1400	,	ItemLv = 10	,	Material1 = 1611	,	Material2 = 4415	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Mini Bow
Crafting[9  ] = {Active = 1,	ItemID = 73		,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 4029	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Dagger
Crafting[10 ] = {Active = 1,	ItemID = 1415	,	ItemLv = 10	,	Material1 = 1611	,	Material2 = 3368	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Small Dagger
Crafting[11 ] = {Active = 1,	ItemID = 1443	,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 4415	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Thesis Dagger
Crafting[12 ] = {Active = 1,	ItemID = 97		,	ItemLv = 10	,	Material1 = 1611	,	Material2 = 4029	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Mystic Branch
Crafting[13 ] = {Active = 1,	ItemID = 1427	,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 3368	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Blessed Staff
Crafting[14 ] = {Active = 1,	ItemID = 1462	,	ItemLv = 10	,	Material1 = 1611	,	Material2 = 4415	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Arcane Branch
Crafting[15 ] = {Active = 1,	ItemID = 121	,	ItemLv = 10	,	Material1 = 1583	,	Material2 = 4029	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Husk Shield
Crafting[16 ] = {Active = 1,	ItemID = 2202	,	ItemLv = 10	,	Material1 = 1671	,	Material2 = 1604	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Mousey Cap
Crafting[17 ] = {Active = 1,	ItemID = 2205	,	ItemLv = 10	,	Material1 = 3368	,	Material2 = 1678	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Goaty Cap
Crafting[18 ] = {Active = 1,	ItemID = 335	,	ItemLv = 10	,	Material1 = 3387	,	Material2 = 4341	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Cloth Shirt
Crafting[19 ] = {Active = 1,	ItemID = 305	,	ItemLv = 10	,	Material1 = 1670	,	Material2 = 4422	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Coarse Vest
Crafting[20 ] = {Active = 1,	ItemID = 380	,	ItemLv = 10	,	Material1 = 4039	,	Material2 = 1604	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Mousey Costume
Crafting[21 ] = {Active = 1,	ItemID = 290	,	ItemLv = 10	,	Material1 = 1640	,	Material2 = 4422	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Husk Armor
Crafting[22 ] = {Active = 1,	ItemID = 365	,	ItemLv = 10	,	Material1 = 3387	,	Material2 = 1605	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Medic Robe
Crafting[23 ] = {Active = 1,	ItemID = 383	,	ItemLv = 10	,	Material1 = 1706	,	Material2 = 1678	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Goaty Costume
Crafting[24 ] = {Active = 1,	ItemID = 511	,	ItemLv = 10	,	Material1 = 4029	,	Material2 = 4422	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Cloth Gloves
Crafting[25 ] = {Active = 1,	ItemID = 481	,	ItemLv = 10	,	Material1 = 3368	,	Material2 = 4413	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Coarse Gloves
Crafting[26 ] = {Active = 1,	ItemID = 556	,	ItemLv = 10	,	Material1 = 3360	,	Material2 = 1605	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Mousey Muffs
Crafting[27 ] = {Active = 1,	ItemID = 466	,	ItemLv = 10	,	Material1 = 3387	,	Material2 = 4422	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Husk Gloves
Crafting[28 ] = {Active = 1,	ItemID = 541	,	ItemLv = 10	,	Material1 = 3368	,	Material2 = 1707	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Medic Gloves
Crafting[29 ] = {Active = 1,	ItemID = 559	,	ItemLv = 10	,	Material1 = 4029	,	Material2 = 1678	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Goaty Muffs
Crafting[30 ] = {Active = 1,	ItemID = 687	,	ItemLv = 10	,	Material1 = 1671	,	Material2 = 4405	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Cloth Boots
Crafting[31 ] = {Active = 1,	ItemID = 657	,	ItemLv = 10	,	Material1 = 1671	,	Material2 = 1695	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Coarse Boots
Crafting[32 ] = {Active = 1,	ItemID = 732	,	ItemLv = 10	,	Material1 = 1671	,	Material2 = 4317	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Mousey Shoes
Crafting[33 ] = {Active = 1,	ItemID = 642	,	ItemLv = 10	,	Material1 = 4029	,	Material2 = 4486	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Husk Boots
Crafting[34 ] = {Active = 1,	ItemID = 717	,	ItemLv = 10	,	Material1 = 4029	,	Material2 = 4407	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Medic Boots
Crafting[35 ] = {Active = 1,	ItemID = 735	,	ItemLv = 10	,	Material1 = 4029	,	Material2 = 1679	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Goaty Shoes
Crafting[36 ] = {Active = 1,	ItemID = 4666	,	ItemLv = 15	,	Material1 = 1696	,	Material2 = 4415	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Necklace of Vitality
Crafting[37 ] = {Active = 1,	ItemID = 4667	,	ItemLv = 15	,	Material1 = 4335	,	Material2 = 4029	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Moonlight Necklace
Crafting[38 ] = {Active = 1,	ItemID = 4668	,	ItemLv = 15	,	Material1 = 4336	,	Material2 = 3368	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Emerald Amulet
Crafting[39 ] = {Active = 1,	ItemID = 4669	,	ItemLv = 15	,	Material1 = 4420	,	Material2 = 4415	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Ebony Wreath
Crafting[40 ] = {Active = 1,	ItemID = 4611	,	ItemLv = 15	,	Material1 = 4337	,	Material2 = 4029	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Gold Ring
Crafting[41 ] = {Active = 1,	ItemID = 4612	,	ItemLv = 15	,	Material1 = 4339	,	Material2 = 3368	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Brass Ring
Crafting[42 ] = {Active = 1,	ItemID = 4613	,	ItemLv = 15	,	Material1 = 4423	,	Material2 = 4415	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Bright Silver Ring
Crafting[43 ] = {Active = 1,	ItemID = 4614	,	ItemLv = 15	,	Material1 = 4490	,	Material2 = 4029	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Fine Steel Ring
Crafting[44 ] = {Active = 1,	ItemID = 4615	,	ItemLv = 15	,	Material1 = 4388	,	Material2 = 3368	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Ruby Encrusted Ring
Crafting[45 ] = {Active = 1,	ItemID = 10		,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 3368	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Short Metal Sword
Crafting[46 ] = {Active = 1,	ItemID = 1395	,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 4415	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Small Steel Sword
Crafting[47 ] = {Active = 1,	ItemID = 13		,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 4029	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Two Handed Sword
Crafting[48 ] = {Active = 1,	ItemID = 1370	,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 3368	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Two-Edged Sword
Crafting[49 ] = {Active = 1,	ItemID = 1379	,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 4415	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Heavy Sword
Crafting[50 ] = {Active = 1,	ItemID = 32		,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 4029	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Hunter Bow
Crafting[51 ] = {Active = 1,	ItemID = 1403	,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 3368	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Shikar Bow
Crafting[52 ] = {Active = 1,	ItemID = 80		,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 4415	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Sharp Dagger
Crafting[53 ] = {Active = 1,	ItemID = 1422	,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 4029	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Piercing Dagger
Crafting[54 ] = {Active = 1,	ItemID = 1450	,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 3368	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Sharp Blade
Crafting[55 ] = {Active = 1,	ItemID = 104	,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 4415	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Wooden Stave
Crafting[56 ] = {Active = 1,	ItemID = 1434	,	ItemLv = 15	,	Material1 = 1708	,	Material2 = 4029	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Fine Stave
Crafting[57 ] = {Active = 1,	ItemID = 1469	,	ItemLv = 15	,	Material1 = 4418	,	Material2 = 3368	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Grained Stave
Crafting[58 ] = {Active = 1,	ItemID = 2188	,	ItemLv = 15	,	Material1 = 4029	,	Material2 = 4341	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Rooroo Cap
Crafting[59 ] = {Active = 1,	ItemID = 2196	,	ItemLv = 15	,	Material1 = 4351	,	Material2 = 1605	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Sheepy Cap
Crafting[60 ] = {Active = 1,	ItemID = 311	,	ItemLv = 15	,	Material1 = 1784	,	Material2 = 1707	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Safari Vest
Crafting[61 ] = {Active = 1,	ItemID = 351	,	ItemLv = 15	,	Material1 = 1668	,	Material2 = 4479	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Rooroo Costume
Crafting[62 ] = {Active = 1,	ItemID = 336	,	ItemLv = 15	,	Material1 = 4040	,	Material2 = 1608	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Tough Vest
Crafting[63 ] = {Active = 1,	ItemID = 372	,	ItemLv = 15	,	Material1 = 1668	,	Material2 = 1604	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Attendant Robe
Crafting[64 ] = {Active = 1,	ItemID = 359	,	ItemLv = 15	,	Material1 = 1634	,	Material2 = 1608	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Sheepy Costume
Crafting[65 ] = {Active = 1,	ItemID = 296	,	ItemLv = 15	,	Material1 = 1706	,	Material2 = 1707	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Soft Leather Armor
Crafting[66 ] = {Active = 1,	ItemID = 489	,	ItemLv = 15	,	Material1 = 4030	,	Material2 = 1845	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Hunter Gloves
Crafting[67 ] = {Active = 1,	ItemID = 527	,	ItemLv = 15	,	Material1 = 3368	,	Material2 = 4422	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Rooroo Muffs
Crafting[68 ] = {Active = 1,	ItemID = 512	,	ItemLv = 15	,	Material1 = 4351	,	Material2 = 4479	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Tough Gloves
Crafting[69 ] = {Active = 1,	ItemID = 548	,	ItemLv = 15	,	Material1 = 4030	,	Material2 = 4470	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Attendant Gloves
Crafting[70 ] = {Active = 1,	ItemID = 535	,	ItemLv = 15	,	Material1 = 3368	,	Material2 = 1605	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Sheepy Muffs
Crafting[71 ] = {Active = 1,	ItemID = 472	,	ItemLv = 15	,	Material1 = 4030	,	Material2 = 4458	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Soft Leather Gloves
Crafting[72 ] = {Active = 1,	ItemID = 663	,	ItemLv = 15	,	Material1 = 4349	,	Material2 = 4333	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Safari Boots
Crafting[73 ] = {Active = 1,	ItemID = 703	,	ItemLv = 15	,	Material1 = 3368	,	Material2 = 4341	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Rooroo Shoes
Crafting[74 ] = {Active = 1,	ItemID = 688	,	ItemLv = 15	,	Material1 = 4349	,	Material2 = 4334	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Tough Boots
Crafting[75 ] = {Active = 1,	ItemID = 724	,	ItemLv = 15	,	Material1 = 4351	,	Material2 = 4416	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Attendant Boots
Crafting[76 ] = {Active = 1,	ItemID = 711	,	ItemLv = 15	,	Material1 = 4351	,	Material2 = 4489	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Sheepy Shoes
Crafting[77 ] = {Active = 1,	ItemID = 648	,	ItemLv = 15	,	Material1 = 4351	,	Material2 = 4341	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Soft Leather Boots
Crafting[78 ] = {Active = 1,	ItemID = 4671	,	ItemLv = 20	,	Material1 = 1682	,	Material2 = 4351	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Vaudeville Necklace
Crafting[79 ] = {Active = 1,	ItemID = 4672	,	ItemLv = 20	,	Material1 = 4346	,	Material2 = 4349	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Baptismal Necklace
Crafting[80 ] = {Active = 1,	ItemID = 4673	,	ItemLv = 20	,	Material1 = 4430	,	Material2 = 4030	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Officer Necklace
Crafting[81 ] = {Active = 1,	ItemID = 4674	,	ItemLv = 20	,	Material1 = 4379	,	Material2 = 3360	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Hunter Charm
Crafting[82 ] = {Active = 1,	ItemID = 4675	,	ItemLv = 20	,	Material1 = 4432	,	Material2 = 4351	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Mercenary Necklace
Crafting[83 ] = {Active = 1,	ItemID = 4616	,	ItemLv = 20	,	Material1 = 4356	,	Material2 = 4349	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Animal Tusk Ring
Crafting[84 ] = {Active = 1,	ItemID = 4617	,	ItemLv = 20	,	Material1 = 4371	,	Material2 = 4030	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Antler Ring
Crafting[85 ] = {Active = 1,	ItemID = 4618	,	ItemLv = 20	,	Material1 = 4480	,	Material2 = 3360	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Squid Ring
Crafting[86 ] = {Active = 1,	ItemID = 4619	,	ItemLv = 20	,	Material1 = 4377	,	Material2 = 4351	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Hunter Ring
Crafting[87 ] = {Active = 1,	ItemID = 4620	,	ItemLv = 20	,	Material1 = 4465	,	Material2 = 4349	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Mystic Flower Ring
Crafting[88 ] = {Active = 1,	ItemID = 636	,	ItemLv = 20	,	Material1 = 4376	,	Material2 = 3360	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Lv 2 Strike Coral
Crafting[89 ] = {Active = 1,	ItemID = 818	,	ItemLv = 20	,	Material1 = 4373	,	Material2 = 4351	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Lv 2 Wind Coral
Crafting[90 ] = {Active = 1,	ItemID = 868	,	ItemLv = 20	,	Material1 = 4493	,	Material2 = 4349	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Lv 2 Thunder Coral
Crafting[91 ] = {Active = 1,	ItemID = 873	,	ItemLv = 20	,	Material1 = 4383	,	Material2 = 4030	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Lv 2 Fog Coral
Crafting[92 ] = {Active = 1,	ItemID = 2		,	ItemLv = 20	,	Material1 = 1747	,	Material2 = 4351	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Long Sword
Crafting[93 ] = {Active = 1,	ItemID = 1389	,	ItemLv = 20	,	Material1 = 4370	,	Material2 = 4030	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Light Sword
Crafting[94 ] = {Active = 1,	ItemID = 26		,	ItemLv = 20	,	Material1 = 1698	,	Material2 = 3360	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Long Bow
Crafting[95 ] = {Active = 1,	ItemID = 1401	,	ItemLv = 20	,	Material1 = 4434	,	Material2 = 4349	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Common Bow
Crafting[96 ] = {Active = 1,	ItemID = 74		,	ItemLv = 20	,	Material1 = 1753	,	Material2 = 4349	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Kris
Crafting[97 ] = {Active = 1,	ItemID = 1416	,	ItemLv = 20	,	Material1 = 3933	,	Material2 = 3360	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Impact Spike
Crafting[98 ] = {Active = 1,	ItemID = 1444	,	ItemLv = 20	,	Material1 = 4437	,	Material2 = 4349	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Bandit Spike
Crafting[99 ] = {Active = 1,	ItemID = 98		,	ItemLv = 20	,	Material1 = 4354	,	Material2 = 3360	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Wooden Stick
Crafting[100] = {Active = 1,	ItemID = 1428	,	ItemLv = 20	,	Material1 = 4348	,	Material2 = 4349	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Battle Staff
Crafting[101] = {Active = 1,	ItemID = 1463	,	ItemLv = 20	,	Material1 = 4481	,	Material2 = 3360	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Thick Stave
Crafting[102] = {Active = 1,	ItemID = 122	,	ItemLv = 20	,	Material1 = 4358	,	Material2 = 4349	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Steel Shield
Crafting[103] = {Active = 1,	ItemID = 2208	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 4479	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Racoon Cap
Crafting[104] = {Active = 1,	ItemID = 2203	,	ItemLv = 20	,	Material1 = 3360	,	Material2 = 1608	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Kitty Cap
Crafting[105] = {Active = 1,	ItemID = 386	,	ItemLv = 20	,	Material1 = 1784	,	Material2 = 4479	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Racoon Costume
Crafting[106] = {Active = 1,	ItemID = 338	,	ItemLv = 20	,	Material1 = 4040	,	Material2 = 4458	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Adventure Vest
Crafting[107] = {Active = 1,	ItemID = 306	,	ItemLv = 20	,	Material1 = 1634	,	Material2 = 3384	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Canvas Vest
Crafting[108] = {Active = 1,	ItemID = 291	,	ItemLv = 20	,	Material1 = 4040	,	Material2 = 1845	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Thick Armor
Crafting[109] = {Active = 1,	ItemID = 381	,	ItemLv = 20	,	Material1 = 1784	,	Material2 = 1608	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Kitty Costume
Crafting[110] = {Active = 1,	ItemID = 366	,	ItemLv = 20	,	Material1 = 1634	,	Material2 = 4458	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Foster Robe
Crafting[111] = {Active = 1,	ItemID = 562	,	ItemLv = 20	,	Material1 = 4351	,	Material2 = 4470	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Racoon Muffs
Crafting[112] = {Active = 1,	ItemID = 514	,	ItemLv = 20	,	Material1 = 4349	,	Material2 = 4458	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Adventure Gloves
Crafting[113] = {Active = 1,	ItemID = 482	,	ItemLv = 20	,	Material1 = 1673	,	Material2 = 4470	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Canvas Gloves
Crafting[114] = {Active = 1,	ItemID = 467	,	ItemLv = 20	,	Material1 = 3360	,	Material2 = 4458	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Thick Gloves
Crafting[115] = {Active = 1,	ItemID = 557	,	ItemLv = 20	,	Material1 = 4349	,	Material2 = 1845	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Kitty Muffs
Crafting[116] = {Active = 1,	ItemID = 542	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 1608	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Foster Gloves
Crafting[117] = {Active = 1,	ItemID = 738	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 4364	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Racoon Shoes
Crafting[118] = {Active = 1,	ItemID = 690	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 1721	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Adventure Boots
Crafting[119] = {Active = 1,	ItemID = 658	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 3384	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Canvas Boots
Crafting[120] = {Active = 1,	ItemID = 643	,	ItemLv = 20	,	Material1 = 1671	,	Material2 = 3384	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Thick Boots
Crafting[121] = {Active = 1,	ItemID = 733	,	ItemLv = 20	,	Material1 = 4030	,	Material2 = 3932	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Kitty Shoes
Crafting[122] = {Active = 1,	ItemID = 718	,	ItemLv = 20	,	Material1 = 4349	,	Material2 = 1721	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Foster Boots
Crafting[123] = {Active = 1,	ItemID = 4676	,	ItemLv = 25	,	Material1 = 4376	,	Material2 = 4349	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Necklace of the Roaring Wind
Crafting[124] = {Active = 1,	ItemID = 4677	,	ItemLv = 25	,	Material1 = 4373	,	Material2 = 4030	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Necklace of Dusk
Crafting[125] = {Active = 1,	ItemID = 4678	,	ItemLv = 25	,	Material1 = 4493	,	Material2 = 3360	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Jade Necklace
Crafting[126] = {Active = 1,	ItemID = 4679	,	ItemLv = 25	,	Material1 = 4383	,	Material2 = 4351	,	Material3 =	2611,	Rad = 1,	GoodItem = 0} -- Necklace of Exorcism
Crafting[127] = {Active = 1,	ItemID = 4621	,	ItemLv = 25	,	Material1 = 1682	,	Material2 = 4030	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Assault Ring
Crafting[128] = {Active = 1,	ItemID = 4622	,	ItemLv = 25	,	Material1 = 4346	,	Material2 = 3360	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Reinforced Ring
Crafting[129] = {Active = 1,	ItemID = 4623	,	ItemLv = 25	,	Material1 = 4430	,	Material2 = 4351	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Ring of Force
Crafting[130] = {Active = 1,	ItemID = 4624	,	ItemLv = 25	,	Material1 = 4379	,	Material2 = 4349	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Ring of Aim
Crafting[131] = {Active = 1,	ItemID = 4625	,	ItemLv = 25	,	Material1 = 4432	,	Material2 = 4030	,	Material3 =	2614,	Rad = 1,	GoodItem = 0} -- Ring of Ecstasy
Crafting[132] = {Active = 1,	ItemID = 11		,	ItemLv = 25	,	Material1 = 3929	,	Material2 = 4349	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Steel Sword
Crafting[133] = {Active = 1,	ItemID = 1396	,	ItemLv = 25	,	Material1 = 4391	,	Material2 = 3360	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Common Steel Sword
Crafting[134] = {Active = 1,	ItemID = 14		,	ItemLv = 25	,	Material1 = 4431	,	Material2 = 4351	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Warrior Sword
Crafting[135] = {Active = 1,	ItemID = 1371	,	ItemLv = 25	,	Material1 = 4492	,	Material2 = 4349	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Fearsome Sword
Crafting[136] = {Active = 1,	ItemID = 1380	,	ItemLv = 25	,	Material1 = 1661	,	Material2 = 4030	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Vehemence Sword
Crafting[137] = {Active = 1,	ItemID = 33		,	ItemLv = 25	,	Material1 = 4392	,	Material2 = 4351	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Battle Bow
Crafting[138] = {Active = 1,	ItemID = 1404	,	ItemLv = 25	,	Material1 = 4438	,	Material2 = 4030	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Soldier Bow
Crafting[139] = {Active = 1,	ItemID = 37		,	ItemLv = 25	,	Material1 = 4460	,	Material2 = 3360	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Firegun
Crafting[140] = {Active = 1,	ItemID = 1406	,	ItemLv = 25	,	Material1 = 4436	,	Material2 = 4351	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Old Firegun
Crafting[141] = {Active = 1,	ItemID = 81		,	ItemLv = 25	,	Material1 = 1841	,	Material2 = 4030	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Swift Kris
Crafting[142] = {Active = 1,	ItemID = 1423	,	ItemLv = 25	,	Material1 = 4350	,	Material2 = 4351	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Guerrilla Dagger
Crafting[143] = {Active = 1,	ItemID = 1451	,	ItemLv = 25	,	Material1 = 4445	,	Material2 = 4030	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Light Dagger
Crafting[144] = {Active = 1,	ItemID = 105	,	ItemLv = 25	,	Material1 = 4440	,	Material2 = 4351	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Hand Stave
Crafting[145] = {Active = 1,	ItemID = 1435	,	ItemLv = 25	,	Material1 = 4394	,	Material2 = 4030	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Scholar Stave
Crafting[146] = {Active = 1,	ItemID = 1470	,	ItemLv = 25	,	Material1 = 3793	,	Material2 = 4351	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Enhanced Stave
Crafting[147] = {Active = 1,	ItemID = 2189	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 1721	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Playful Racoon Cap
Crafting[148] = {Active = 1,	ItemID = 2197	,	ItemLv = 25	,	Material1 = 4031	,	Material2 = 4470	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Meowy Cap
Crafting[149] = {Active = 1,	ItemID = 313	,	ItemLv = 25	,	Material1 = 3391	,	Material2 = 4458	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Hunter Vest
Crafting[150] = {Active = 1,	ItemID = 352	,	ItemLv = 25	,	Material1 = 1667	,	Material2 = 1608	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Playful Racoon Costume
Crafting[151] = {Active = 1,	ItemID = 337	,	ItemLv = 25	,	Material1 = 1662	,	Material2 = 4512	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Explorer Vest
Crafting[152] = {Active = 1,	ItemID = 360	,	ItemLv = 25	,	Material1 = 3388	,	Material2 = 4929	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Meowy Costume
Crafting[153] = {Active = 1,	ItemID = 297	,	ItemLv = 25	,	Material1 = 3380	,	Material2 = 4470	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Heavy Leather Armor
Crafting[154] = {Active = 1,	ItemID = 373	,	ItemLv = 25	,	Material1 = 1639	,	Material2 = 4949	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Scholar Robe
Crafting[155] = {Active = 1,	ItemID = 487	,	ItemLv = 25	,	Material1 = 4349	,	Material2 = 1608	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Safari Gloves
Crafting[156] = {Active = 1,	ItemID = 528	,	ItemLv = 25	,	Material1 = 4351	,	Material2 = 4364	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Playful Racoon Muffs
Crafting[157] = {Active = 1,	ItemID = 513	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 4512	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Explorer Gloves
Crafting[158] = {Active = 1,	ItemID = 536	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 4949	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Meowy Muffs
Crafting[159] = {Active = 1,	ItemID = 473	,	ItemLv = 25	,	Material1 = 4031	,	Material2 = 4949	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Heavy Leather Gloves
Crafting[160] = {Active = 1,	ItemID = 549	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 4929	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Scholar Gloves
Crafting[161] = {Active = 1,	ItemID = 665	,	ItemLv = 25	,	Material1 = 3368	,	Material2 = 4359	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Hunter Boots
Crafting[162] = {Active = 1,	ItemID = 704	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 3932	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Playful Racoon Shoes
Crafting[163] = {Active = 1,	ItemID = 689	,	ItemLv = 25	,	Material1 = 1253	,	Material2 = 4347	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Explorer Boots
Crafting[164] = {Active = 1,	ItemID = 712	,	ItemLv = 25	,	Material1 = 4029	,	Material2 = 3932	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Meowy Shoes
Crafting[165] = {Active = 1,	ItemID = 649	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 4359	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Heavy Leather Boots
Crafting[166] = {Active = 1,	ItemID = 725	,	ItemLv = 25	,	Material1 = 3360	,	Material2 = 4347	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Scholar Boots
Crafting[167] = {Active = 1,	ItemID = 4681	,	ItemLv = 30	,	Material1 = 1624	,	Material2 = 1630	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Soul Necklace
Crafting[168] = {Active = 1,	ItemID = 4682	,	ItemLv = 30	,	Material1 = 1688	,	Material2 = 1781	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Light of the Holy Priest
Crafting[169] = {Active = 1,	ItemID = 4683	,	ItemLv = 30	,	Material1 = 4930	,	Material2 = 1751	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Emperor Necklace
Crafting[170] = {Active = 1,	ItemID = 4684	,	ItemLv = 30	,	Material1 = 4950	,	Material2 = 1645	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Heavenly Necklace
Crafting[171] = {Active = 1,	ItemID = 4685	,	ItemLv = 30	,	Material1 = 4534	,	Material2 = 1636	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Hero Necklace
Crafting[172] = {Active = 1,	ItemID = 4626	,	ItemLv = 30	,	Material1 = 1688	,	Material2 = 1751	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Tooth of Ferocity
Crafting[173] = {Active = 1,	ItemID = 4627	,	ItemLv = 30	,	Material1 = 4930	,	Material2 = 1645	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Huge Bear Footprint
Crafting[174] = {Active = 1,	ItemID = 4628	,	ItemLv = 30	,	Material1 = 4950	,	Material2 = 1636	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Feathers of Paradise Bird
Crafting[175] = {Active = 1,	ItemID = 4629	,	ItemLv = 30	,	Material1 = 4534	,	Material2 = 4041	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Hoof of Nimble Deer
Crafting[176] = {Active = 1,	ItemID = 4630	,	ItemLv = 30	,	Material1 = 1713	,	Material2 = 1643	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Mystic Stone
Crafting[177] = {Active = 1,	ItemID = 637	,	ItemLv = 30	,	Material1 = 4950	,	Material2 = 4041	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 3 Strike Coral
Crafting[178] = {Active = 1,	ItemID = 819	,	ItemLv = 30	,	Material1 = 4534	,	Material2 = 1643	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 3 Wind Coral
Crafting[179] = {Active = 1,	ItemID = 869	,	ItemLv = 30	,	Material1 = 1713	,	Material2 = 1630	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 3 Thunder Coral
Crafting[180] = {Active = 1,	ItemID = 874	,	ItemLv = 30	,	Material1 = 4521	,	Material2 = 1781	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 3 Fog Coral
Crafting[181] = {Active = 1,	ItemID = 3		,	ItemLv = 30	,	Material1 = 1739	,	Material2 = 1630	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Fencing Sword
Crafting[182] = {Active = 1,	ItemID = 1390	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1751	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Fine Blade
Crafting[183] = {Active = 1,	ItemID = 3798	,	ItemLv = 30	,	Material1 = 1739	,	Material2 = 1636	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Blade of Crimson Flame
Crafting[184] = {Active = 1,	ItemID = 27		,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1643	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Tribal Bow
Crafting[185] = {Active = 1,	ItemID = 1402	,	ItemLv = 30	,	Material1 = 4498	,	Material2 = 1751	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Tribal Short Bow
Crafting[186] = {Active = 1,	ItemID = 3805	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1630	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Firegun of Clarion Mist
Crafting[187] = {Active = 1,	ItemID = 75		,	ItemLv = 30	,	Material1 = 4454	,	Material2 = 1781	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Trident
Crafting[188] = {Active = 1,	ItemID = 1417	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1636	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Dagger of Ripple
Crafting[189] = {Active = 1,	ItemID = 1445	,	ItemLv = 30	,	Material1 = 1739	,	Material2 = 1643	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Dagger of Haste
Crafting[190] = {Active = 1,	ItemID = 3816	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1781	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Blade of the Mist
Crafting[191] = {Active = 1,	ItemID = 99		,	ItemLv = 30	,	Material1 = 4454	,	Material2 = 1751	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Magical Staff
Crafting[192] = {Active = 1,	ItemID = 1429	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 4041	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Staff
Crafting[193] = {Active = 1,	ItemID = 1464	,	ItemLv = 30	,	Material1 = 1739	,	Material2 = 1630	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Ceremonial Staff
Crafting[194] = {Active = 1,	ItemID = 3809	,	ItemLv = 30	,	Material1 = 4504	,	Material2 = 1751	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Emerald Vine Wand
Crafting[195] = {Active = 1,	ItemID = 123	,	ItemLv = 30	,	Material1 = 4454	,	Material2 = 1645	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Tower Shield
Crafting[196] = {Active = 1,	ItemID = 2187	,	ItemLv = 30	,	Material1 = 1216	,	Material2 = 4519	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Crabby Cap
Crafting[197] = {Active = 1,	ItemID = 2211	,	ItemLv = 30	,	Material1 = 1630	,	Material2 = 4472	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Night Owl Cap
Crafting[198] = {Active = 1,	ItemID = 307	,	ItemLv = 30	,	Material1 = 3380	,	Material2 = 1287	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Exquisite Vest
Crafting[199] = {Active = 1,	ItemID = 340	,	ItemLv = 30	,	Material1 = 1639	,	Material2 = 1287	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Oarsman Vest
Crafting[200] = {Active = 1,	ItemID = 1976	,	ItemLv = 30	,	Material1 = 1662	,	Material2 = 4986	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Robe of the Mist
Crafting[201] = {Active = 1,	ItemID = 1943	,	ItemLv = 30	,	Material1 = 1639	,	Material2 = 4986	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Vest of Clarion Mist
Crafting[202] = {Active = 1,	ItemID = 350	,	ItemLv = 30	,	Material1 = 3388	,	Material2 = 4986	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Crabby Costume
Crafting[203] = {Active = 1,	ItemID = 1928	,	ItemLv = 30	,	Material1 = 1662	,	Material2 = 1287	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Battle Armor of Crimson Flame
Crafting[204] = {Active = 1,	ItemID = 368	,	ItemLv = 30	,	Material1 = 3388	,	Material2 = 1287	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Nurse Robe
Crafting[205] = {Active = 1,	ItemID = 1955	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 1287	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emerald Vine Robe
Crafting[206] = {Active = 1,	ItemID = 389	,	ItemLv = 30	,	Material1 = 1630	,	Material2 = 1287	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Night Owl Costume
Crafting[207] = {Active = 1,	ItemID = 293	,	ItemLv = 30	,	Material1 = 3380	,	Material2 = 4986	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Rhino Hide Armor
Crafting[208] = {Active = 1,	ItemID = 483	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4380	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Exquisite Gloves
Crafting[209] = {Active = 1,	ItemID = 516	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4949	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Oarsman Gloves
Crafting[210] = {Active = 1,	ItemID = 1980	,	ItemLv = 30	,	Material1 = 1630	,	Material2 = 4967	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Gloves of the Mist
Crafting[211] = {Active = 1,	ItemID = 1947	,	ItemLv = 30	,	Material1 = 1216	,	Material2 = 1606	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Gloves of Clarion Mist
Crafting[212] = {Active = 1,	ItemID = 526	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4986	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Crabby Gloves
Crafting[213] = {Active = 1,	ItemID = 1935	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4516	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Gauntlets of Crimson Flame
Crafting[214] = {Active = 1,	ItemID = 544	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 1307	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Nurse Gloves
Crafting[215] = {Active = 1,	ItemID = 1962	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4517	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emerald Vine Gloves
Crafting[216] = {Active = 1,	ItemID = 565	,	ItemLv = 30	,	Material1 = 4467	,	Material2 = 4472	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Night Owl Muffs
Crafting[217] = {Active = 1,	ItemID = 469	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 1607	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Rhino Hide Gloves
Crafting[218] = {Active = 1,	ItemID = 659	,	ItemLv = 30	,	Material1 = 4031	,	Material2 = 4517	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Exquisite Boots
Crafting[219] = {Active = 1,	ItemID = 692	,	ItemLv = 30	,	Material1 = 4031	,	Material2 = 1307	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Oarsman Boots
Crafting[220] = {Active = 1,	ItemID = 1984	,	ItemLv = 30	,	Material1 = 1196	,	Material2 = 4519	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Boots of the Mist
Crafting[221] = {Active = 1,	ItemID = 1951	,	ItemLv = 30	,	Material1 = 4041	,	Material2 = 4472	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Boots of Clarion Mist
Crafting[222] = {Active = 1,	ItemID = 702	,	ItemLv = 30	,	Material1 = 1216	,	Material2 = 4986	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Crabby Shoes
Crafting[223] = {Active = 1,	ItemID = 1939	,	ItemLv = 30	,	Material1 = 4031	,	Material2 = 4380	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Greaves of Crimson Flame
Crafting[224] = {Active = 1,	ItemID = 720	,	ItemLv = 30	,	Material1 = 4031	,	Material2 = 4516	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Nurse Boots
Crafting[225] = {Active = 1,	ItemID = 1969	,	ItemLv = 30	,	Material1 = 1196	,	Material2 = 1606	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emerald Vine Boots
Crafting[226] = {Active = 1,	ItemID = 741	,	ItemLv = 30	,	Material1 = 1216	,	Material2 = 4967	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Night Owl Shoes
Crafting[227] = {Active = 1,	ItemID = 645	,	ItemLv = 30	,	Material1 = 4031	,	Material2 = 1607	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Rhino Hide Boots
Crafting[228] = {Active = 1,	ItemID = 4686	,	ItemLv = 35	,	Material1 = 1713	,	Material2 = 4041	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Elegant Necklace
Crafting[229] = {Active = 1,	ItemID = 4687	,	ItemLv = 35	,	Material1 = 4521	,	Material2 = 1643	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Wood Necklace
Crafting[230] = {Active = 1,	ItemID = 4688	,	ItemLv = 35	,	Material1 = 4537	,	Material2 = 1630	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Outlander Necklace
Crafting[231] = {Active = 1,	ItemID = 4689	,	ItemLv = 35	,	Material1 = 1624	,	Material2 = 1781	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Academic Charm
Crafting[232] = {Active = 1,	ItemID = 4631	,	ItemLv = 35	,	Material1 = 4521	,	Material2 = 1630	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Barbaric Ring
Crafting[233] = {Active = 1,	ItemID = 4632	,	ItemLv = 35	,	Material1 = 4537	,	Material2 = 1781	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Cavalier Ring
Crafting[234] = {Active = 1,	ItemID = 4633	,	ItemLv = 35	,	Material1 = 1624	,	Material2 = 1751	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Cavalry Ring
Crafting[235] = {Active = 1,	ItemID = 4634	,	ItemLv = 35	,	Material1 = 1688	,	Material2 = 1645	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Shooter's Ring
Crafting[236] = {Active = 1,	ItemID = 4635	,	ItemLv = 35	,	Material1 = 4930	,	Material2 = 1636	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Sacrificial Ring
Crafting[237] = {Active = 1,	ItemID = 12		,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1781	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Striking Sword
Crafting[238] = {Active = 1,	ItemID = 1397	,	ItemLv = 35	,	Material1 = 4454	,	Material2 = 1645	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Assassin Sword
Crafting[239] = {Active = 1,	ItemID = 770	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 4041	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Sword of Grief
Crafting[240] = {Active = 1,	ItemID = 28		,	ItemLv = 35	,	Material1 = 4454	,	Material2 = 1630	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Meteor Bow
Crafting[241] = {Active = 1,	ItemID = 34		,	ItemLv = 35	,	Material1 = 1739	,	Material2 = 1781	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Marksman Bow
Crafting[242] = {Active = 1,	ItemID = 1405	,	ItemLv = 35	,	Material1 = 4504	,	Material2 = 1645	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Marksman Bow
Crafting[243] = {Active = 1,	ItemID = 38		,	ItemLv = 35	,	Material1 = 4454	,	Material2 = 1636	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Fire-Rifle
Crafting[244] = {Active = 1,	ItemID = 781	,	ItemLv = 35	,	Material1 = 1739	,	Material2 = 4041	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Touch of Death
Crafting[245] = {Active = 1,	ItemID = 1407	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1643	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Enhanced Firegun
Crafting[246] = {Active = 1,	ItemID = 82		,	ItemLv = 35	,	Material1 = 1739	,	Material2 = 1751	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pointed Kris
Crafting[247] = {Active = 1,	ItemID = 799	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1645	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Tooth of Specter
Crafting[248] = {Active = 1,	ItemID = 1424	,	ItemLv = 35	,	Material1 = 4454	,	Material2 = 4041	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Sword of Ripple
Crafting[249] = {Active = 1,	ItemID = 1452	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1630	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Refined Dagger
Crafting[250] = {Active = 1,	ItemID = 106	,	ItemLv = 35	,	Material1 = 1739	,	Material2 = 1645	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Blessing Stave
Crafting[251] = {Active = 1,	ItemID = 785	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1636	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of the Avenger
Crafting[252] = {Active = 1,	ItemID = 1436	,	ItemLv = 35	,	Material1 = 4454	,	Material2 = 1643	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Vehemence Stave
Crafting[253] = {Active = 1,	ItemID = 1471	,	ItemLv = 35	,	Material1 = 4498	,	Material2 = 1781	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Beautiful Stave
Crafting[254] = {Active = 1,	ItemID = 124	,	ItemLv = 35	,	Material1 = 1739	,	Material2 = 1636	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Feather Shield
Crafting[255] = {Active = 1,	ItemID = 2191	,	ItemLv = 35	,	Material1 = 1751	,	Material2 = 4967	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Big Crab Cap
Crafting[256] = {Active = 1,	ItemID = 2198	,	ItemLv = 35	,	Material1 = 1643	,	Material2 = 1307	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Owl Cap
Crafting[257] = {Active = 1,	ItemID = 339	,	ItemLv = 35	,	Material1 = 1636	,	Material2 = 4820	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Helmsman Vest
Crafting[258] = {Active = 1,	ItemID = 803	,	ItemLv = 35	,	Material1 = 1771	,	Material2 = 1607	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Mantle of the Naga
Crafting[259] = {Active = 1,	ItemID = 354	,	ItemLv = 35	,	Material1 = 1643	,	Material2 = 4820	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Big Crab Costume
Crafting[260] = {Active = 1,	ItemID = 314	,	ItemLv = 35	,	Material1 = 1751	,	Material2 = 4820	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Slick Vest
Crafting[261] = {Active = 1,	ItemID = 777	,	ItemLv = 35	,	Material1 = 1642	,	Material2 = 1607	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Robe of Death
Crafting[262] = {Active = 1,	ItemID = 374	,	ItemLv = 35	,	Material1 = 1630	,	Material2 = 4820	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emergency Robe
Crafting[263] = {Active = 1,	ItemID = 789	,	ItemLv = 35	,	Material1 = 1672	,	Material2 = 1607	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Robe of the Venom Witch
Crafting[264] = {Active = 1,	ItemID = 361	,	ItemLv = 35	,	Material1 = 1631	,	Material2 = 4820	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Owl Costume
Crafting[265] = {Active = 1,	ItemID = 298	,	ItemLv = 35	,	Material1 = 1645	,	Material2 = 4820	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Strong Leather Armor
Crafting[266] = {Active = 1,	ItemID = 763	,	ItemLv = 35	,	Material1 = 4042	,	Material2 = 1607	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Armor of Revenant
Crafting[267] = {Active = 1,	ItemID = 515	,	ItemLv = 35	,	Material1 = 1781	,	Material2 = 4805	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Helmsman Gloves
Crafting[268] = {Active = 1,	ItemID = 530	,	ItemLv = 35	,	Material1 = 1645	,	Material2 = 4519	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Big Crab Muffs
Crafting[269] = {Active = 1,	ItemID = 490	,	ItemLv = 35	,	Material1 = 1751	,	Material2 = 4986	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Slick Gloves
Crafting[270] = {Active = 1,	ItemID = 550	,	ItemLv = 35	,	Material1 = 1643	,	Material2 = 4805	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emergency Gloves
Crafting[271] = {Active = 1,	ItemID = 537	,	ItemLv = 35	,	Material1 = 1781	,	Material2 = 4517	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Owl Muffs
Crafting[272] = {Active = 1,	ItemID = 474	,	ItemLv = 35	,	Material1 = 1645	,	Material2 = 4516	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Strong Leather Gloves
Crafting[273] = {Active = 1,	ItemID = 691	,	ItemLv = 35	,	Material1 = 4467	,	Material2 = 1606	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Helmsman Boots
Crafting[274] = {Active = 1,	ItemID = 706	,	ItemLv = 35	,	Material1 = 4363	,	Material2 = 4519	,	Material3 =	2596,	Rad = 1,	GoodItem = 0} -- Big Crab Shoes
Crafting[275] = {Active = 1,	ItemID = 666	,	ItemLv = 35	,	Material1 = 4536	,	Material2 = 4986	,	Material3 =	2593,	Rad = 1,	GoodItem = 0} -- Slick Boots
Crafting[276] = {Active = 1,	ItemID = 726	,	ItemLv = 35	,	Material1 = 1631	,	Material2 = 4967	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Emergency Boots
Crafting[277] = {Active = 1,	ItemID = 713	,	ItemLv = 35	,	Material1 = 4536	,	Material2 = 1609	,	Material3 =	2599,	Rad = 1,	GoodItem = 0} -- Owl Shoes
Crafting[278] = {Active = 1,	ItemID = 650	,	ItemLv = 35	,	Material1 = 1631	,	Material2 = 1607	,	Material3 =	2590,	Rad = 1,	GoodItem = 0} -- Strong Leather Boots
Crafting[279] = {Active = 1,	ItemID = 739	,	ItemLv = 40	,	Material1 = 4968	,	Material2 = 3927	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Onslaught
Crafting[280] = {Active = 1,	ItemID = 4691	,	ItemLv = 40	,	Material1 = 4539	,	Material2 = 1666	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Ashen Gem
Crafting[281] = {Active = 1,	ItemID = 4692	,	ItemLv = 40	,	Material1 = 4987	,	Material2 = 1699	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Mark of the Dragon
Crafting[282] = {Active = 1,	ItemID = 4693	,	ItemLv = 40	,	Material1 = 1616	,	Material2 = 3364	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Hope of Life
Crafting[283] = {Active = 1,	ItemID = 4694	,	ItemLv = 40	,	Material1 = 4908	,	Material2 = 4541	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Symbolic Necklace
Crafting[284] = {Active = 1,	ItemID = 1121	,	ItemLv = 40	,	Material1 = 1634	,	Material2 = 1287	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Pumpkin Mask
Crafting[285] = {Active = 1,	ItemID = 1122	,	ItemLv = 40	,	Material1 = 1673	,	Material2 = 4479	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Deathsoul Mask
Crafting[286] = {Active = 1,	ItemID = 1120	,	ItemLv = 40	,	Material1 = 1706	,	Material2 = 4470	,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Snowdoll Mask
Crafting[287] = {Active = 1,	ItemID = 4695	,	ItemLv = 40	,	Material1 = 4524	,	Material2 = 1288	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Red Nit Gem
Crafting[288] = {Active = 1,	ItemID = 4636	,	ItemLv = 40	,	Material1 = 1199	,	Material2 = 3927	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Crusader Ring
Crafting[289] = {Active = 1,	ItemID = 4637	,	ItemLv = 40	,	Material1 = 4968	,	Material2 = 1666	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Counterattack Ring
Crafting[290] = {Active = 1,	ItemID = 4638	,	ItemLv = 40	,	Material1 = 4539	,	Material2 = 1699	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Guerrilla Warfare Ring
Crafting[291] = {Active = 1,	ItemID = 4639	,	ItemLv = 40	,	Material1 = 4987	,	Material2 = 3364	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Sniper Ring
Crafting[292] = {Active = 1,	ItemID = 4640	,	ItemLv = 40	,	Material1 = 1616	,	Material2 = 4541	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Advancement
Crafting[293] = {Active = 1,	ItemID = 638	,	ItemLv = 40	,	Material1 = 4746	,	Material2 = 3927	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 4 Strike Coral
Crafting[294] = {Active = 1,	ItemID = 820	,	ItemLv = 40	,	Material1 = 1199	,	Material2 = 1666	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 4 Wind Coral
Crafting[295] = {Active = 1,	ItemID = 870	,	ItemLv = 40	,	Material1 = 4968	,	Material2 = 1699	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 4 Thunder Coral
Crafting[296] = {Active = 1,	ItemID = 875	,	ItemLv = 40	,	Material1 = 4539	,	Material2 = 3364	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 4 Fog Coral
Crafting[297] = {Active = 1,	ItemID = 885	,	ItemLv = 40	,	Material1 = 4717	,	Material2 = 1234	,	Material3 =	2602,	Rad = 1,	GoodItem = 0} -- Refining Gem
Crafting[298] = {Active = 1,	ItemID = 4		,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 3927	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Serpentine Sword
Crafting[299] = {Active = 1,	ItemID = 1391	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 1699	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Emerald Blade
Crafting[300] = {Active = 1,	ItemID = 3799	,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 4541	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Sword of the Tempest
Crafting[301] = {Active = 1,	ItemID = 15		,	ItemLv = 40	,	Material1 = 4526	,	Material2 = 1288	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Criss Sword
Crafting[302] = {Active = 1,	ItemID = 1372	,	ItemLv = 40	,	Material1 = 4946	,	Material2 = 1326	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Righteous Sword
Crafting[303] = {Active = 1,	ItemID = 1381	,	ItemLv = 40	,	Material1 = 4526	,	Material2 = 1638	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Protector Sword
Crafting[304] = {Active = 1,	ItemID = 3802	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 1666	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Sword of Glowing Flame
Crafting[305] = {Active = 1,	ItemID = 39		,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 3364	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Exquisite Pistol
Crafting[306] = {Active = 1,	ItemID = 1408	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 1288	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Pocket Pistol
Crafting[307] = {Active = 1,	ItemID = 3806	,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 1326	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Flaming Pistol
Crafting[308] = {Active = 1,	ItemID = 76		,	ItemLv = 40	,	Material1 = 4526	,	Material2 = 4032	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Moon Kris
Crafting[309] = {Active = 1,	ItemID = 1418	,	ItemLv = 40	,	Material1 = 4946	,	Material2 = 1666	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Comet Spike
Crafting[310] = {Active = 1,	ItemID = 1446	,	ItemLv = 40	,	Material1 = 4526	,	Material2 = 3364	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Spike of Feint
Crafting[311] = {Active = 1,	ItemID = 3817	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 1288	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Blade of the Tempest
Crafting[312] = {Active = 1,	ItemID = 100	,	ItemLv = 40	,	Material1 = 4946	,	Material2 = 1308	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Grace Wand
Crafting[313] = {Active = 1,	ItemID = 101	,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 1326	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Beastly Wand
Crafting[314] = {Active = 1,	ItemID = 1430	,	ItemLv = 40	,	Material1 = 4946	,	Material2 = 1638	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Wand of Holiness
Crafting[315] = {Active = 1,	ItemID = 1431	,	ItemLv = 40	,	Material1 = 4523	,	Material2 = 3927	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Feral Wand
Crafting[316] = {Active = 1,	ItemID = 1465	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 3364	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Brilliance Wand
Crafting[317] = {Active = 1,	ItemID = 1466	,	ItemLv = 40	,	Material1 = 4946	,	Material2 = 4541	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Wand of Vigor
Crafting[318] = {Active = 1,	ItemID = 3810	,	ItemLv = 40	,	Material1 = 1210	,	Material2 = 1326	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Seal of Frozen Crescent
Crafting[319] = {Active = 1,	ItemID = 3813	,	ItemLv = 40	,	Material1 = 1301	,	Material2 = 4032	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Glory Sigil
Crafting[320] = {Active = 1,	ItemID = 2214	,	ItemLv = 40	,	Material1 = 1638	,	Material2 = 1619	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Bunny Baby Cap
Crafting[321] = {Active = 1,	ItemID = 2212	,	ItemLv = 40	,	Material1 = 3927	,	Material2 = 2396	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Kangaroo Cap
Crafting[322] = {Active = 1,	ItemID = 2190	,	ItemLv = 40	,	Material1 = 1666	,	Material2 = 4474	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Duckling Cap
Crafting[323] = {Active = 1,	ItemID = 1977	,	ItemLv = 40	,	Material1 = 3927	,	Material2 = 4933	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Coat of the Tempest
Crafting[324] = {Active = 1,	ItemID = 310	,	ItemLv = 40	,	Material1 = 4455	,	Material2 = 4933	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Feather Vest
Crafting[325] = {Active = 1,	ItemID = 300	,	ItemLv = 40	,	Material1 = 1636	,	Material2 = 4933	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Chain Mail
Crafting[326] = {Active = 1,	ItemID = 1929	,	ItemLv = 40	,	Material1 = 1666	,	Material2 = 4933	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Battle Armor of the Tempest
Crafting[327] = {Active = 1,	ItemID = 1944	,	ItemLv = 40	,	Material1 = 4032	,	Material2 = 4933	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Flaming Coat
Crafting[328] = {Active = 1,	ItemID = 392	,	ItemLv = 40	,	Material1 = 3442	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Bunny Baby Costume
Crafting[329] = {Active = 1,	ItemID = 390	,	ItemLv = 40	,	Material1 = 4368	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Kangaroo Costume
Crafting[330] = {Active = 1,	ItemID = 1956	,	ItemLv = 40	,	Material1 = 3364	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Coat of Frozen Crescent
Crafting[331] = {Active = 1,	ItemID = 341	,	ItemLv = 40	,	Material1 = 1699	,	Material2 = 4933	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Deckman Vest
Crafting[332] = {Active = 1,	ItemID = 367	,	ItemLv = 40	,	Material1 = 1751	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Travel Robe
Crafting[333] = {Active = 1,	ItemID = 295	,	ItemLv = 40	,	Material1 = 4541	,	Material2 = 4933	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Breast Plate
Crafting[334] = {Active = 1,	ItemID = 353	,	ItemLv = 40	,	Material1 = 1234	,	Material2 = 4933	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Duckling Costume
Crafting[335] = {Active = 1,	ItemID = 370	,	ItemLv = 40	,	Material1 = 1645	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Holy Robe
Crafting[336] = {Active = 1,	ItemID = 1959	,	ItemLv = 40	,	Material1 = 1253	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Glory Coat
Crafting[337] = {Active = 1,	ItemID = 1981	,	ItemLv = 40	,	Material1 = 1666	,	Material2 = 2396	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Gloves of the Tempest
Crafting[338] = {Active = 1,	ItemID = 486	,	ItemLv = 40	,	Material1 = 4541	,	Material2 = 1325	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Feather Gloves
Crafting[339] = {Active = 1,	ItemID = 476	,	ItemLv = 40	,	Material1 = 4541	,	Material2 = 4474	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Chain Gauntlets
Crafting[340] = {Active = 1,	ItemID = 568	,	ItemLv = 40	,	Material1 = 4032	,	Material2 = 4805	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Bunny Baby Muffs
Crafting[341] = {Active = 1,	ItemID = 1936	,	ItemLv = 40	,	Material1 = 1699	,	Material2 = 1325	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Gauntlets of the Tempest
Crafting[342] = {Active = 1,	ItemID = 1948	,	ItemLv = 40	,	Material1 = 4032	,	Material2 = 4474	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Flaming Gloves
Crafting[343] = {Active = 1,	ItemID = 566	,	ItemLv = 40	,	Material1 = 3364	,	Material2 = 1344	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Kangaroo Muffs
Crafting[344] = {Active = 1,	ItemID = 1963	,	ItemLv = 40	,	Material1 = 3364	,	Material2 = 1609	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of Frozen Crescent
Crafting[345] = {Active = 1,	ItemID = 543	,	ItemLv = 40	,	Material1 = 1666	,	Material2 = 4522	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Travel Gloves
Crafting[346] = {Active = 1,	ItemID = 517	,	ItemLv = 40	,	Material1 = 1699	,	Material2 = 1607	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Deckman Gloves
Crafting[347] = {Active = 1,	ItemID = 471	,	ItemLv = 40	,	Material1 = 1638	,	Material2 = 4805	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Gauntlets
Crafting[348] = {Active = 1,	ItemID = 546	,	ItemLv = 40	,	Material1 = 3927	,	Material2 = 1619	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Holy Gloves
Crafting[349] = {Active = 1,	ItemID = 529	,	ItemLv = 40	,	Material1 = 3927	,	Material2 = 4820	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Duckling Muffs
Crafting[350] = {Active = 1,	ItemID = 1966	,	ItemLv = 40	,	Material1 = 4541	,	Material2 = 4522	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Glory Gloves
Crafting[351] = {Active = 1,	ItemID = 1985	,	ItemLv = 40	,	Material1 = 1234	,	Material2 = 1344	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Boots of the Tempest
Crafting[352] = {Active = 1,	ItemID = 662	,	ItemLv = 40	,	Material1 = 1669	,	Material2 = 4805	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Feather Boots
Crafting[353] = {Active = 1,	ItemID = 744	,	ItemLv = 40	,	Material1 = 3442	,	Material2 = 4820	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Bunny Baby Shoes
Crafting[354] = {Active = 1,	ItemID = 1940	,	ItemLv = 40	,	Material1 = 1234	,	Material2 = 1325	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Greaves of the Tempest
Crafting[355] = {Active = 1,	ItemID = 1952	,	ItemLv = 40	,	Material1 = 1669	,	Material2 = 4820	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Flaming Boots
Crafting[356] = {Active = 1,	ItemID = 742	,	ItemLv = 40	,	Material1 = 3442	,	Material2 = 1619	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Kangaroo Shoes
Crafting[357] = {Active = 1,	ItemID = 1970	,	ItemLv = 40	,	Material1 = 4368	,	Material2 = 4522	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of Frozen Crescent
Crafting[358] = {Active = 1,	ItemID = 719	,	ItemLv = 40	,	Material1 = 3442	,	Material2 = 4805	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Travel Boots
Crafting[359] = {Active = 1,	ItemID = 652	,	ItemLv = 40	,	Material1 = 1253	,	Material2 = 1325	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Chain Greaves
Crafting[360] = {Active = 1,	ItemID = 693	,	ItemLv = 40	,	Material1 = 1253	,	Material2 = 1619	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Deckman Boots
Crafting[361] = {Active = 1,	ItemID = 647	,	ItemLv = 40	,	Material1 = 4368	,	Material2 = 2396	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Greaves
Crafting[362] = {Active = 1,	ItemID = 722	,	ItemLv = 40	,	Material1 = 1253	,	Material2 = 1344	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Holy Boots
Crafting[363] = {Active = 1,	ItemID = 705	,	ItemLv = 40	,	Material1 = 4455	,	Material2 = 2396	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Duckling Shoes
Crafting[364] = {Active = 1,	ItemID = 1973	,	ItemLv = 40	,	Material1 = 4455	,	Material2 = 4820	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Glory Boots
Crafting[365] = {Active = 1,	ItemID = 1932	,	ItemLv = 40	,	Material1 = 4850	,	Material2 = 4933	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Battle Armor of Nature
Crafting[366] = {Active = 1,	ItemID = 29		,	ItemLv = 44	,	Material1 = 4946	,	Material2 = 1699	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Recca Bow
Crafting[367] = {Active = 1,	ItemID = 4696	,	ItemLv = 45	,	Material1 = 1613	,	Material2 = 1308	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Necklace of Shooting Star
Crafting[368] = {Active = 1,	ItemID = 4697	,	ItemLv = 45	,	Material1 = 1176	,	Material2 = 1326	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Necklace of Speed
Crafting[369] = {Active = 1,	ItemID = 4698	,	ItemLv = 45	,	Material1 = 4542	,	Material2 = 4032	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Storm Necklace
Crafting[370] = {Active = 1,	ItemID = 4699	,	ItemLv = 45	,	Material1 = 4746	,	Material2 = 1638	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Charm of Encounter
Crafting[371] = {Active = 1,	ItemID = 4641	,	ItemLv = 45	,	Material1 = 4908	,	Material2 = 1288	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Eye of the Tiger
Crafting[372] = {Active = 1,	ItemID = 4642	,	ItemLv = 45	,	Material1 = 4524	,	Material2 = 1308	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of the Yeti
Crafting[373] = {Active = 1,	ItemID = 4643	,	ItemLv = 45	,	Material1 = 1613	,	Material2 = 1326	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of the Hawk
Crafting[374] = {Active = 1,	ItemID = 4644	,	ItemLv = 45	,	Material1 = 1176	,	Material2 = 4032	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Paw of Cheetah
Crafting[375] = {Active = 1,	ItemID = 4645	,	ItemLv = 45	,	Material1 = 4542	,	Material2 = 1638	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Wild Breeze
Crafting[376] = {Active = 1,	ItemID = 22		,	ItemLv = 45	,	Material1 = 4526	,	Material2 = 1666	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Crescent Sword
Crafting[377] = {Active = 1,	ItemID = 774	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 3369	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Blade of Incantation
Crafting[378] = {Active = 1,	ItemID = 1398	,	ItemLv = 45	,	Material1 = 4946	,	Material2 = 3364	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Tribal Sword
Crafting[379] = {Active = 1,	ItemID = 20		,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 1308	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Rebel Sword
Crafting[380] = {Active = 1,	ItemID = 771	,	ItemLv = 45	,	Material1 = 1301	,	Material2 = 4042	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Greatsword of Incantation
Crafting[381] = {Active = 1,	ItemID = 1377	,	ItemLv = 45	,	Material1 = 4523	,	Material2 = 4032	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Common Rebel Sword
Crafting[382] = {Active = 1,	ItemID = 1386	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 3927	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Enhanced Rebel Sword
Crafting[383] = {Active = 1,	ItemID = 44		,	ItemLv = 45	,	Material1 = 4526	,	Material2 = 4541	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Token Pistol
Crafting[384] = {Active = 1,	ItemID = 782	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 4825	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Kiss of the Cursed
Crafting[385] = {Active = 1,	ItemID = 1413	,	ItemLv = 45	,	Material1 = 4946	,	Material2 = 1308	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Steel Pistol
Crafting[386] = {Active = 1,	ItemID = 83		,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 1638	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Crystalline Kris
Crafting[387] = {Active = 1,	ItemID = 800	,	ItemLv = 45	,	Material1 = 1301	,	Material2 = 3927	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Tooth of the Cursed
Crafting[388] = {Active = 1,	ItemID = 1425	,	ItemLv = 45	,	Material1 = 4523	,	Material2 = 1699	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Quartz Dagger
Crafting[389] = {Active = 1,	ItemID = 1460	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 4541	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Gemmed Dagger
Crafting[390] = {Active = 1,	ItemID = 107	,	ItemLv = 45	,	Material1 = 4526	,	Material2 = 4032	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Battle Stave
Crafting[391] = {Active = 1,	ItemID = 786	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 3369	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Incantation
Crafting[392] = {Active = 1,	ItemID = 793	,	ItemLv = 45	,	Material1 = 1301	,	Material2 = 4042	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Abraxas
Crafting[393] = {Active = 1,	ItemID = 1437	,	ItemLv = 45	,	Material1 = 4526	,	Material2 = 1666	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Restraining Staff
Crafting[394] = {Active = 1,	ItemID = 1440	,	ItemLv = 45	,	Material1 = 1210	,	Material2 = 1699	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Elegant Staff
Crafting[395] = {Active = 1,	ItemID = 1472	,	ItemLv = 45	,	Material1 = 4523	,	Material2 = 1288	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Endeavor
Crafting[396] = {Active = 1,	ItemID = 1475	,	ItemLv = 45	,	Material1 = 4526	,	Material2 = 1308	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Spiritual Staff
Crafting[397] = {Active = 1,	ItemID = 4301	,	ItemLv = 45	,	Material1 = 4946	,	Material2 = 1638	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Sagacious
Crafting[398] = {Active = 1,	ItemID = 125	,	ItemLv = 45	,	Material1 = 4523	,	Material2 = 1638	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Crevice Shield
Crafting[399] = {Active = 1,	ItemID = 2199	,	ItemLv = 45	,	Material1 = 1638	,	Material2 = 1325	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Hopperoo Cap
Crafting[400] = {Active = 1,	ItemID = 2210	,	ItemLv = 45	,	Material1 = 1308	,	Material2 = 4953	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Happy Bunny Cap
Crafting[401] = {Active = 1,	ItemID = 2193	,	ItemLv = 45	,	Material1 = 1288	,	Material2 = 4972	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Ducky Cap
Crafting[402] = {Active = 1,	ItemID = 315	,	ItemLv = 45	,	Material1 = 1666	,	Material2 = 4971	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Peacock Vest
Crafting[403] = {Active = 1,	ItemID = 790	,	ItemLv = 45	,	Material1 = 3425	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Coat of Invocation
Crafting[404] = {Active = 1,	ItemID = 362	,	ItemLv = 45	,	Material1 = 3442	,	Material2 = 4971	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Hopperoo Costume
Crafting[405] = {Active = 1,	ItemID = 388	,	ItemLv = 45	,	Material1 = 4368	,	Material2 = 4971	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Happy Bunny Costume
Crafting[406] = {Active = 1,	ItemID = 778	,	ItemLv = 45	,	Material1 = 3431	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Corset of Incantation
Crafting[407] = {Active = 1,	ItemID = 375	,	ItemLv = 45	,	Material1 = 1308	,	Material2 = 4820	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Passage Robe
Crafting[408] = {Active = 1,	ItemID = 764	,	ItemLv = 45	,	Material1 = 3429	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Tattoo of the Cursed Warrior
Crafting[409] = {Active = 1,	ItemID = 301	,	ItemLv = 45	,	Material1 = 1638	,	Material2 = 4991	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Strong Platemail
Crafting[410] = {Active = 1,	ItemID = 302	,	ItemLv = 45	,	Material1 = 3927	,	Material2 = 4805	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Light Platemail
Crafting[411] = {Active = 1,	ItemID = 796	,	ItemLv = 45	,	Material1 = 3426	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Robe of Abraxas
Crafting[412] = {Active = 1,	ItemID = 378	,	ItemLv = 45	,	Material1 = 4455	,	Material2 = 4971	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Piety Robe
Crafting[413] = {Active = 1,	ItemID = 342	,	ItemLv = 45	,	Material1 = 4541	,	Material2 = 4971	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Mastman Vest
Crafting[414] = {Active = 1,	ItemID = 356	,	ItemLv = 45	,	Material1 = 1234	,	Material2 = 4971	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Ducky Costume
Crafting[415] = {Active = 1,	ItemID = 804	,	ItemLv = 45	,	Material1 = 3428	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Mantle of the Cursed Flame
Crafting[416] = {Active = 1,	ItemID = 767	,	ItemLv = 45	,	Material1 = 3427	,	Material2 = 1310	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Platemail of the Cursed Soul
Crafting[417] = {Active = 1,	ItemID = 538	,	ItemLv = 45	,	Material1 = 1288	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Hopperoo Muffs
Crafting[418] = {Active = 1,	ItemID = 491	,	ItemLv = 45	,	Material1 = 1308	,	Material2 = 178		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Peacock Gloves
Crafting[419] = {Active = 1,	ItemID = 564	,	ItemLv = 45	,	Material1 = 1326	,	Material2 = 4934	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Happy Bunny Muffs
Crafting[420] = {Active = 1,	ItemID = 554	,	ItemLv = 45	,	Material1 = 1638	,	Material2 = 175		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Piety Gloves
Crafting[421] = {Active = 1,	ItemID = 518	,	ItemLv = 45	,	Material1 = 1326	,	Material2 = 4954	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Mastman Gloves
Crafting[422] = {Active = 1,	ItemID = 551	,	ItemLv = 45	,	Material1 = 1308	,	Material2 = 4934	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Passage Gloves
Crafting[423] = {Active = 1,	ItemID = 477	,	ItemLv = 45	,	Material1 = 1288	,	Material2 = 178		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Strong Gauntlets
Crafting[424] = {Active = 1,	ItemID = 478	,	ItemLv = 45	,	Material1 = 1288	,	Material2 = 176		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Light Gauntlets
Crafting[425] = {Active = 1,	ItemID = 532	,	ItemLv = 45	,	Material1 = 1638	,	Material2 = 4953	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Ducky Muffs
Crafting[426] = {Active = 1,	ItemID = 714	,	ItemLv = 45	,	Material1 = 1669	,	Material2 = 1325	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Hopperoo Shoes
Crafting[427] = {Active = 1,	ItemID = 667	,	ItemLv = 45	,	Material1 = 1234	,	Material2 = 4934	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Peacock Boots
Crafting[428] = {Active = 1,	ItemID = 740	,	ItemLv = 45	,	Material1 = 3442	,	Material2 = 2396	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Happy Bunny Shoes
Crafting[429] = {Active = 1,	ItemID = 730	,	ItemLv = 45	,	Material1 = 1638	,	Material2 = 4933	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Piety Boots
Crafting[430] = {Active = 1,	ItemID = 727	,	ItemLv = 45	,	Material1 = 4455	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Passage Boots
Crafting[431] = {Active = 1,	ItemID = 654	,	ItemLv = 45	,	Material1 = 4455	,	Material2 = 4953	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Light Greaves
Crafting[432] = {Active = 1,	ItemID = 694	,	ItemLv = 45	,	Material1 = 1253	,	Material2 = 4971	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Mastman Boots
Crafting[433] = {Active = 1,	ItemID = 708	,	ItemLv = 45	,	Material1 = 4850	,	Material2 = 176		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Ducky Shoes
Crafting[434] = {Active = 1,	ItemID = 653	,	ItemLv = 45	,	Material1 = 4368	,	Material2 = 4954	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Strong Greaves
Crafting[435] = {Active = 1,	ItemID = 4701	,	ItemLv = 50	,	Material1 = 1710	,	Material2 = 1637	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Burning Vitality
Crafting[436] = {Active = 1,	ItemID = 4702	,	ItemLv = 50	,	Material1 = 1219	,	Material2 = 1345	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Warm Wind of Spring
Crafting[437] = {Active = 1,	ItemID = 4703	,	ItemLv = 50	,	Material1 = 1334	,	Material2 = 3390	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Autumn Night Glimmer
Crafting[438] = {Active = 1,	ItemID = 4704	,	ItemLv = 50	,	Material1 = 4979	,	Material2 = 1201	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Wintery Blizzard
Crafting[439] = {Active = 1,	ItemID = 4705	,	ItemLv = 50	,	Material1 = 1237	,	Material2 = 1635	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Force of Four Seasons
Crafting[440] = {Active = 1,	ItemID = 4646	,	ItemLv = 50	,	Material1 = 1364	,	Material2 = 1637	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Pharaoh
Crafting[441] = {Active = 1,	ItemID = 4647	,	ItemLv = 50	,	Material1 = 167		,	Material2 = 1345	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Resistance
Crafting[442] = {Active = 1,	ItemID = 4648	,	ItemLv = 50	,	Material1 = 1179	,	Material2 = 3390	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Bandit Ring
Crafting[443] = {Active = 1,	ItemID = 4649	,	ItemLv = 50	,	Material1 = 1361	,	Material2 = 1201	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Bewitching Ring
Crafting[444] = {Active = 1,	ItemID = 4650	,	ItemLv = 50	,	Material1 = 1183	,	Material2 = 1635	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Believer's Ring
Crafting[445] = {Active = 1,	ItemID = 639	,	ItemLv = 50	,	Material1 = 1219	,	Material2 = 1345	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 5 Strike Coral
Crafting[446] = {Active = 1,	ItemID = 821	,	ItemLv = 50	,	Material1 = 1334	,	Material2 = 3390	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 5 Wind Coral
Crafting[447] = {Active = 1,	ItemID = 871	,	ItemLv = 50	,	Material1 = 4979	,	Material2 = 1201	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Lv 5 Thunder Coral
Crafting[448] = {Active = 1,	ItemID = 876	,	ItemLv = 50	,	Material1 = 1237	,	Material2 = 1635	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Lv 5 Fog Coral
Crafting[449] = {Active = 1,	ItemID = 881	,	ItemLv = 50	,	Material1 = 1237	,	Material2 = 1637	,	Material3 =	2603,	Rad = 1,	GoodItem = 0} -- Lustrious Gem
Crafting[450] = {Active = 1,	ItemID = 882	,	ItemLv = 50	,	Material1 = 1353	,	Material2 = 1345	,	Material3 =	2603,	Rad = 1,	GoodItem = 0} -- Glowing Gem
Crafting[451] = {Active = 1,	ItemID = 883	,	ItemLv = 50	,	Material1 = 4998	,	Material2 = 3390	,	Material3 =	2603,	Rad = 1,	GoodItem = 0} -- Shining Gem
Crafting[452] = {Active = 1,	ItemID = 884	,	ItemLv = 50	,	Material1 = 4883	,	Material2 = 1201	,	Material3 =	2603,	Rad = 1,	GoodItem = 0} -- Shadow Gem
Crafting[453] = {Active = 1,	ItemID = 887	,	ItemLv = 50	,	Material1 = 1179	,	Material2 = 1635	,	Material3 =	2603,	Rad = 1,	GoodItem = 0} -- Spirit Gem
Crafting[454] = {Active = 1,	ItemID = 5		,	ItemLv = 50	,	Material1 = 1211	,	Material2 = 1637	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Dazzling Sword
Crafting[455] = {Active = 1,	ItemID = 1392	,	ItemLv = 50	,	Material1 = 4794	,	Material2 = 3390	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Steel Saber
Crafting[456] = {Active = 1,	ItemID = 3800	,	ItemLv = 50	,	Material1 = 1185	,	Material2 = 1635	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Spike of Sistine
Crafting[457] = {Active = 1,	ItemID = 16		,	ItemLv = 50	,	Material1 = 1276	,	Material2 = 1239	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Paladin Sword
Crafting[458] = {Active = 1,	ItemID = 1373	,	ItemLv = 50	,	Material1 = 1281	,	Material2 = 4033	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Cavalier Saber
Crafting[459] = {Active = 1,	ItemID = 1382	,	ItemLv = 50	,	Material1 = 1191	,	Material2 = 1637	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Invader Sword
Crafting[460] = {Active = 1,	ItemID = 3803	,	ItemLv = 50	,	Material1 = 4927	,	Material2 = 3390	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Roar of Gallon
Crafting[461] = {Active = 1,	ItemID = 40		,	ItemLv = 50	,	Material1 = 1275	,	Material2 = 1635	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Exquisite Rifle
Crafting[462] = {Active = 1,	ItemID = 1409	,	ItemLv = 50	,	Material1 = 4964	,	Material2 = 1647	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Battle Rifle
Crafting[463] = {Active = 1,	ItemID = 3807	,	ItemLv = 50	,	Material1 = 1302	,	Material2 = 1641	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Rifle of Darwin
Crafting[464] = {Active = 1,	ItemID = 77		,	ItemLv = 50	,	Material1 = 4947	,	Material2 = 1637	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Hyena Dagger
Crafting[465] = {Active = 1,	ItemID = 1419	,	ItemLv = 50	,	Material1 = 1185	,	Material2 = 1201	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Blade of Torment
Crafting[466] = {Active = 1,	ItemID = 1447	,	ItemLv = 50	,	Material1 = 1367	,	Material2 = 1239	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pirate Dagger
Crafting[467] = {Active = 1,	ItemID = 3818	,	ItemLv = 50	,	Material1 = 1281	,	Material2 = 4033	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Spike of Forlorn
Crafting[468] = {Active = 1,	ItemID = 102	,	ItemLv = 50	,	Material1 = 4926	,	Material2 = 1641	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Thundorian Staff
Crafting[469] = {Active = 1,	ItemID = 103	,	ItemLv = 50	,	Material1 = 1191	,	Material2 = 1647	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Life
Crafting[470] = {Active = 1,	ItemID = 1432	,	ItemLv = 50	,	Material1 = 1275	,	Material2 = 1647	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Thunderbolt
Crafting[471] = {Active = 1,	ItemID = 1433	,	ItemLv = 50	,	Material1 = 1366	,	Material2 = 4033	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Hearten Wand
Crafting[472] = {Active = 1,	ItemID = 1467	,	ItemLv = 50	,	Material1 = 1211	,	Material2 = 1345	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Protector Staff
Crafting[473] = {Active = 1,	ItemID = 1468	,	ItemLv = 50	,	Material1 = 1302	,	Material2 = 3390	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of High Priest
Crafting[474] = {Active = 1,	ItemID = 3811	,	ItemLv = 50	,	Material1 = 4898	,	Material2 = 1239	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Udine
Crafting[475] = {Active = 1,	ItemID = 3814	,	ItemLv = 50	,	Material1 = 1185	,	Material2 = 1647	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Hope
Crafting[476] = {Active = 1,	ItemID = 2207	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 4990	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Otter Cap
Crafting[477] = {Active = 1,	ItemID = 2204	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 175		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Cap
Crafting[478] = {Active = 1,	ItemID = 2192	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 1290	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Lobster Cap
Crafting[479] = {Active = 1,	ItemID = 312	,	ItemLv = 50	,	Material1 = 4541	,	Material2 = 176		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Emerald Vest
Crafting[480] = {Active = 1,	ItemID = 1945	,	ItemLv = 50	,	Material1 = 1234	,	Material2 = 176		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Mantle of Darwin
Crafting[481] = {Active = 1,	ItemID = 385	,	ItemLv = 50	,	Material1 = 4541	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Otter Costume
Crafting[482] = {Active = 1,	ItemID = 382	,	ItemLv = 50	,	Material1 = 3927	,	Material2 = 4971	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Costume
Crafting[483] = {Active = 1,	ItemID = 369	,	ItemLv = 50	,	Material1 = 1638	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Garcon Robe
Crafting[484] = {Active = 1,	ItemID = 1957	,	ItemLv = 50	,	Material1 = 1638	,	Material2 = 4954	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Coat of Udine
Crafting[485] = {Active = 1,	ItemID = 345	,	ItemLv = 50	,	Material1 = 1253	,	Material2 = 176		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Wind Vest
Crafting[486] = {Active = 1,	ItemID = 1978	,	ItemLv = 50	,	Material1 = 1638	,	Material2 = 176		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Robe of Forlorn
Crafting[487] = {Active = 1,	ItemID = 299	,	ItemLv = 50	,	Material1 = 1234	,	Material2 = 178		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Mithril Platemail
Crafting[488] = {Active = 1,	ItemID = 355	,	ItemLv = 50	,	Material1 = 1253	,	Material2 = 178		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Lobster Costume
Crafting[489] = {Active = 1,	ItemID = 1960	,	ItemLv = 50	,	Material1 = 1234	,	Material2 = 4954	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Coat of Hope
Crafting[490] = {Active = 1,	ItemID = 1930	,	ItemLv = 50	,	Material1 = 4541	,	Material2 = 4954	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Battle Armor of Sistine
Crafting[491] = {Active = 1,	ItemID = 371	,	ItemLv = 50	,	Material1 = 1253	,	Material2 = 4954	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Follower Robe
Crafting[492] = {Active = 1,	ItemID = 488	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 160		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Emerald Gloves
Crafting[493] = {Active = 1,	ItemID = 1949	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 161		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Gloves of Darwin
Crafting[494] = {Active = 1,	ItemID = 561	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 4780	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Otter Muffs
Crafting[495] = {Active = 1,	ItemID = 545	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 160		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Garcon Gloves
Crafting[496] = {Active = 1,	ItemID = 558	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 1328	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Muffs
Crafting[497] = {Active = 1,	ItemID = 1964	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 4780	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of Udine
Crafting[498] = {Active = 1,	ItemID = 521	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 1290	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Wind Gloves
Crafting[499] = {Active = 1,	ItemID = 1982	,	ItemLv = 50	,	Material1 = 1637	,	Material2 = 177		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Gloves of Forlorn
Crafting[500] = {Active = 1,	ItemID = 475	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 4884	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Mithril Gauntlets
Crafting[501] = {Active = 1,	ItemID = 531	,	ItemLv = 50	,	Material1 = 1638	,	Material2 = 1290	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Lobster Muffs
Crafting[502] = {Active = 1,	ItemID = 1967	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 175		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of Hope
Crafting[503] = {Active = 1,	ItemID = 547	,	ItemLv = 50	,	Material1 = 4033	,	Material2 = 177		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Follower Gloves
Crafting[504] = {Active = 1,	ItemID = 1937	,	ItemLv = 50	,	Material1 = 1345	,	Material2 = 161		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Gauntlets of Sistine
Crafting[505] = {Active = 1,	ItemID = 664	,	ItemLv = 50	,	Material1 = 4895	,	Material2 = 1290	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Emerald Boots
Crafting[506] = {Active = 1,	ItemID = 737	,	ItemLv = 50	,	Material1 = 4895	,	Material2 = 4779	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Otter Shoes
Crafting[507] = {Active = 1,	ItemID = 721	,	ItemLv = 50	,	Material1 = 4895	,	Material2 = 175		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Garcon Boots
Crafting[508] = {Active = 1,	ItemID = 1953	,	ItemLv = 50	,	Material1 = 4721	,	Material2 = 1729	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Boots of Darwin
Crafting[509] = {Active = 1,	ItemID = 734	,	ItemLv = 50	,	Material1 = 4791	,	Material2 = 4972	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Shoes
Crafting[510] = {Active = 1,	ItemID = 697	,	ItemLv = 50	,	Material1 = 4721	,	Material2 = 4857	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Wind Boots
Crafting[511] = {Active = 1,	ItemID = 1986	,	ItemLv = 50	,	Material1 = 4791	,	Material2 = 1328	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Boots of Forlorn
Crafting[512] = {Active = 1,	ItemID = 1971	,	ItemLv = 50	,	Material1 = 4721	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of Udine
Crafting[513] = {Active = 1,	ItemID = 707	,	ItemLv = 50	,	Material1 = 4791	,	Material2 = 161		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Lobster Shoes
Crafting[514] = {Active = 1,	ItemID = 1974	,	ItemLv = 50	,	Material1 = 4791	,	Material2 = 4971	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of Hope
Crafting[515] = {Active = 1,	ItemID = 651	,	ItemLv = 50	,	Material1 = 4721	,	Material2 = 160		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Mithril Greaves
Crafting[516] = {Active = 1,	ItemID = 1941	,	ItemLv = 50	,	Material1 = 4721	,	Material2 = 177		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Greaves of Sistine
Crafting[517] = {Active = 1,	ItemID = 723	,	ItemLv = 50	,	Material1 = 4895	,	Material2 = 176		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Follower Boots
Crafting[518] = {Active = 1,	ItemID = 1933	,	ItemLv = 50	,	Material1 = 1326	,	Material2 = 4990	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Tattoo of Gallon
Crafting[519] = {Active = 1,	ItemID = 229	,	ItemLv = 50	,	Material1 = 1326	,	Material2 = 1310	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Savage Bull Tattoo
Crafting[520] = {Active = 1,	ItemID = 30		,	ItemLv = 53	,	Material1 = 1184	,	Material2 = 1201	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Bow of Dawn
Crafting[521] = {Active = 1,	ItemID = 4706	,	ItemLv = 55	,	Material1 = 1353	,	Material2 = 1239	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Spirit Spark
Crafting[522] = {Active = 1,	ItemID = 4707	,	ItemLv = 55	,	Material1 = 4998	,	Material2 = 1647	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Milky Way
Crafting[523] = {Active = 1,	ItemID = 4708	,	ItemLv = 55	,	Material1 = 4883	,	Material2 = 4033	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Shooting Star
Crafting[524] = {Active = 1,	ItemID = 4709	,	ItemLv = 55	,	Material1 = 1182	,	Material2 = 1641	,	Material3 =	2612,	Rad = 1,	GoodItem = 0} -- Blessed Rainbow
Crafting[525] = {Active = 1,	ItemID = 4651	,	ItemLv = 55	,	Material1 = 1365	,	Material2 = 1239	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Suppression
Crafting[526] = {Active = 1,	ItemID = 4652	,	ItemLv = 55	,	Material1 = 1202	,	Material2 = 1647	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Trust
Crafting[527] = {Active = 1,	ItemID = 4653	,	ItemLv = 55	,	Material1 = 1293	,	Material2 = 4033	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Vanishing Ring
Crafting[528] = {Active = 1,	ItemID = 4654	,	ItemLv = 55	,	Material1 = 4938	,	Material2 = 1641	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Ring of Binding
Crafting[529] = {Active = 1,	ItemID = 4655	,	ItemLv = 55	,	Material1 = 1710	,	Material2 = 1637	,	Material3 =	2615,	Rad = 1,	GoodItem = 0} -- Mermaid Tears
Crafting[530] = {Active = 1,	ItemID = 23		,	ItemLv = 55	,	Material1 = 1302	,	Material2 = 1345	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Delusion Sword
Crafting[531] = {Active = 1,	ItemID = 775	,	ItemLv = 55	,	Material1 = 4947	,	Material2 = 3371	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Dance of Evanescence
Crafting[532] = {Active = 1,	ItemID = 1399	,	ItemLv = 55	,	Material1 = 4898	,	Material2 = 1201	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Amber Sword
Crafting[533] = {Active = 1,	ItemID = 21		,	ItemLv = 55	,	Material1 = 1367	,	Material2 = 1647	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Charging Sword
Crafting[534] = {Active = 1,	ItemID = 772	,	ItemLv = 55	,	Material1 = 1190	,	Material2 = 3389	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Roar of Evanescence
Crafting[535] = {Active = 1,	ItemID = 1378	,	ItemLv = 55	,	Material1 = 4926	,	Material2 = 1641	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Fiery Sword of Darkan
Crafting[536] = {Active = 1,	ItemID = 1387	,	ItemLv = 55	,	Material1 = 1282	,	Material2 = 1345	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Rustle Sword
Crafting[537] = {Active = 1,	ItemID = 45		,	ItemLv = 55	,	Material1 = 1366	,	Material2 = 1239	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Gattling Firegun
Crafting[538] = {Active = 1,	ItemID = 783	,	ItemLv = 55	,	Material1 = 4879	,	Material2 = 3361	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Bellow of Evanescence
Crafting[539] = {Active = 1,	ItemID = 2984	,	ItemLv = 55	,	Material1 = 4879	,	Material2 = 3361	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Bow of Evanescence
Crafting[540] = {Active = 1,	ItemID = 1414	,	ItemLv = 55	,	Material1 = 1211	,	Material2 = 4033	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Spectar Firegun
Crafting[541] = {Active = 1,	ItemID = 84		,	ItemLv = 55	,	Material1 = 4794	,	Material2 = 1345	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Vampiric Kris
Crafting[542] = {Active = 1,	ItemID = 801	,	ItemLv = 55	,	Material1 = 4898	,	Material2 = 3390	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Tooth of Evanescence
Crafting[543] = {Active = 1,	ItemID = 1426	,	ItemLv = 55	,	Material1 = 1276	,	Material2 = 1635	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Nightmare Dagger
Crafting[544] = {Active = 1,	ItemID = 1461	,	ItemLv = 55	,	Material1 = 1190	,	Material2 = 1647	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Cursed Dagger
Crafting[545] = {Active = 1,	ItemID = 108	,	ItemLv = 55	,	Material1 = 1282	,	Material2 = 4033	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lotus Staff
Crafting[546] = {Active = 1,	ItemID = 787	,	ItemLv = 55	,	Material1 = 4927	,	Material2 = 4043	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Evanescence
Crafting[547] = {Active = 1,	ItemID = 794	,	ItemLv = 55	,	Material1 = 1184	,	Material2 = 1633	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Mirage
Crafting[548] = {Active = 1,	ItemID = 1438	,	ItemLv = 55	,	Material1 = 4879	,	Material2 = 1641	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Fury Staff
Crafting[549] = {Active = 1,	ItemID = 1441	,	ItemLv = 55	,	Material1 = 4964	,	Material2 = 1637	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Gaia
Crafting[550] = {Active = 1,	ItemID = 1473	,	ItemLv = 55	,	Material1 = 4947	,	Material2 = 1201	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Flame
Crafting[551] = {Active = 1,	ItemID = 1476	,	ItemLv = 55	,	Material1 = 4794	,	Material2 = 1635	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Staff of Binding
Crafting[552] = {Active = 1,	ItemID = 4302	,	ItemLv = 55	,	Material1 = 1276	,	Material2 = 4033	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Guardian of Nature
Crafting[553] = {Active = 1,	ItemID = 126	,	ItemLv = 55	,	Material1 = 1367	,	Material2 = 1641	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Blessed Shield
Crafting[554] = {Active = 1,	ItemID = 2200	,	ItemLv = 55	,	Material1 = 1641	,	Material2 = 4459	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Clever Otter Cap
Crafting[555] = {Active = 1,	ItemID = 2194	,	ItemLv = 55	,	Material1 = 1637	,	Material2 = 4884	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Prawn Cap
Crafting[556] = {Active = 1,	ItemID = 2213	,	ItemLv = 55	,	Material1 = 1635	,	Material2 = 4936	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Cap
Crafting[557] = {Active = 1,	ItemID = 316	,	ItemLv = 55	,	Material1 = 1637	,	Material2 = 1310	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Ringdove Vest
Crafting[558] = {Active = 1,	ItemID = 363	,	ItemLv = 55	,	Material1 = 1326	,	Material2 = 3385	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Clever Otter Costume
Crafting[559] = {Active = 1,	ItemID = 797	,	ItemLv = 55	,	Material1 = 3378	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Robe of Malediction
Crafting[560] = {Active = 1,	ItemID = 779	,	ItemLv = 55	,	Material1 = 3361	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Coat of Evanescence
Crafting[561] = {Active = 1,	ItemID = 357	,	ItemLv = 55	,	Material1 = 4033	,	Material2 = 1310	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Prawn Costume
Crafting[562] = {Active = 1,	ItemID = 303	,	ItemLv = 55	,	Material1 = 1345	,	Material2 = 1310	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Silver Platemail
Crafting[563] = {Active = 1,	ItemID = 768	,	ItemLv = 55	,	Material1 = 4044	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Armor of Evanescence
Crafting[564] = {Active = 1,	ItemID = 391	,	ItemLv = 55	,	Material1 = 1637	,	Material2 = 3385	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Costume
Crafting[565] = {Active = 1,	ItemID = 805	,	ItemLv = 55	,	Material1 = 3389	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Cloak of Evanescence
Crafting[566] = {Active = 1,	ItemID = 343	,	ItemLv = 55	,	Material1 = 4033	,	Material2 = 3385	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Hurricane Vest
Crafting[567] = {Active = 1,	ItemID = 791	,	ItemLv = 55	,	Material1 = 1633	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Robe of the Arcane
Crafting[568] = {Active = 1,	ItemID = 379	,	ItemLv = 55	,	Material1 = 1345	,	Material2 = 3385	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Blessed Robe
Crafting[569] = {Active = 1,	ItemID = 376	,	ItemLv = 55	,	Material1 = 3390	,	Material2 = 3385	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Healer Robe
Crafting[570] = {Active = 1,	ItemID = 492	,	ItemLv = 55	,	Material1 = 1345	,	Material2 = 4780	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Ringdove Gloves
Crafting[571] = {Active = 1,	ItemID = 539	,	ItemLv = 55	,	Material1 = 1201	,	Material2 = 4731	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Clever Otter Muffs
Crafting[572] = {Active = 1,	ItemID = 811	,	ItemLv = 55	,	Material1 = 1647	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Gloves of Malediction
Crafting[573] = {Active = 1,	ItemID = 809	,	ItemLv = 55	,	Material1 = 1635	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Gloves of Evanescence
Crafting[574] = {Active = 1,	ItemID = 533	,	ItemLv = 55	,	Material1 = 1637	,	Material2 = 4990	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Prawn Muffs
Crafting[575] = {Active = 1,	ItemID = 479	,	ItemLv = 55	,	Material1 = 3390	,	Material2 = 4991	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Silver Gauntlets
Crafting[576] = {Active = 1,	ItemID = 807	,	ItemLv = 55	,	Material1 = 1239	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Gauntlets of Evanescence
Crafting[577] = {Active = 1,	ItemID = 567	,	ItemLv = 55	,	Material1 = 1239	,	Material2 = 4956	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Muffs
Crafting[578] = {Active = 1,	ItemID = 815	,	ItemLv = 55	,	Material1 = 1641	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Heavy Gloves of Evanescence
Crafting[579] = {Active = 1,	ItemID = 519	,	ItemLv = 55	,	Material1 = 1201	,	Material2 = 4990	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Hurricane Gloves
Crafting[580] = {Active = 1,	ItemID = 552	,	ItemLv = 55	,	Material1 = 1239	,	Material2 = 4782	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Healer Gloves
Crafting[581] = {Active = 1,	ItemID = 812	,	ItemLv = 55	,	Material1 = 1201	,	Material2 = 3385	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Gloves of the Arcane
Crafting[582] = {Active = 1,	ItemID = 555	,	ItemLv = 55	,	Material1 = 1635	,	Material2 = 4991	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Blessed Gloves
Crafting[583] = {Active = 1,	ItemID = 668	,	ItemLv = 55	,	Material1 = 4895	,	Material2 = 4780	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Ringdove Boots
Crafting[584] = {Active = 1,	ItemID = 715	,	ItemLv = 55	,	Material1 = 4895	,	Material2 = 4991	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Clever Otter Shoes
Crafting[585] = {Active = 1,	ItemID = 813	,	ItemLv = 55	,	Material1 = 4034	,	Material2 = 4977	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Boots of Malediction
Crafting[586] = {Active = 1,	ItemID = 810	,	ItemLv = 55	,	Material1 = 4034	,	Material2 = 4939	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Shoes of Evanescence
Crafting[587] = {Active = 1,	ItemID = 709	,	ItemLv = 55	,	Material1 = 4721	,	Material2 = 4884	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Prawn Shoes
Crafting[588] = {Active = 1,	ItemID = 655	,	ItemLv = 55	,	Material1 = 4791	,	Material2 = 4990	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Silver Greaves
Crafting[589] = {Active = 1,	ItemID = 808	,	ItemLv = 55	,	Material1 = 4034	,	Material2 = 4958	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Greaves of Evanescence
Crafting[590] = {Active = 1,	ItemID = 743	,	ItemLv = 55	,	Material1 = 4721	,	Material2 = 4459	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Shoes
Crafting[591] = {Active = 1,	ItemID = 877	,	ItemLv = 55	,	Material1 = 4034	,	Material2 = 4896	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Boots of Evanescence
Crafting[592] = {Active = 1,	ItemID = 695	,	ItemLv = 55	,	Material1 = 4895	,	Material2 = 4731	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Hurricane Boots
Crafting[593] = {Active = 1,	ItemID = 728	,	ItemLv = 55	,	Material1 = 4850	,	Material2 = 4956	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Healer Boots
Crafting[594] = {Active = 1,	ItemID = 814	,	ItemLv = 55	,	Material1 = 4034	,	Material2 = 1697	,	Material3 =	2667,	Rad = 1,	GoodItem = 0} -- Boots of the of the Arcane
Crafting[595] = {Active = 1,	ItemID = 731	,	ItemLv = 55	,	Material1 = 4791	,	Material2 = 4936	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Blessed Boots
Crafting[596] = {Active = 1,	ItemID = 765	,	ItemLv = 55	,	Material1 = 1482	,	Material2 = 3386	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Tattoo of Evanescence
Crafting[597] = {Active = 1,	ItemID = 228	,	ItemLv = 55	,	Material1 = 3821	,	Material2 = 1310	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Raging Bull Tattoo
Crafting[598] = {Active = 1,	ItemID = 495	,	ItemLv = 60	,	Material1 = 1684	,	Material2 = 3366	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Soul Generator
Crafting[599] = {Active = 1,	ItemID = 497	,	ItemLv = 60	,	Material1 = 1221	,	Material2 = 3365	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Blessed Davao
Crafting[600] = {Active = 1,	ItemID = 4711	,	ItemLv = 60	,	Material1 = 1331	,	Material2 = 4044	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Grace of Heaven
Crafting[601] = {Active = 1,	ItemID = 4712	,	ItemLv = 60	,	Material1 = 1350	,	Material2 = 1769	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Dragon's Breath
Crafting[602] = {Active = 1,	ItemID = 4713	,	ItemLv = 60	,	Material1 = 1684	,	Material2 = 3366	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Heaven's Seal
Crafting[603] = {Active = 1,	ItemID = 4714	,	ItemLv = 60	,	Material1 = 1221	,	Material2 = 3365	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Angelic Protection
Crafting[604] = {Active = 1,	ItemID = 4715	,	ItemLv = 60	,	Material1 = 1331	,	Material2 = 4044	,	Material3 =	2613,	Rad = 1,	GoodItem = 0} -- Light of Terra
Crafting[605] = {Active = 1,	ItemID = 4656	,	ItemLv = 60	,	Material1 = 1350	,	Material2 = 1769	,	Material3 =	2616,	Rad = 1,	GoodItem = 0} -- Flame of Fury
Crafting[606] = {Active = 1,	ItemID = 4657	,	ItemLv = 60	,	Material1 = 1684	,	Material2 = 3366	,	Material3 =	2616,	Rad = 1,	GoodItem = 0} -- Stable Cliff
Crafting[607] = {Active = 1,	ItemID = 4658	,	ItemLv = 60	,	Material1 = 1221	,	Material2 = 3365	,	Material3 =	2616,	Rad = 1,	GoodItem = 0} -- Wind of the Gentle Soul
Crafting[608] = {Active = 1,	ItemID = 4659	,	ItemLv = 60	,	Material1 = 1331	,	Material2 = 4044	,	Material3 =	2616,	Rad = 1,	GoodItem = 0} -- Entwined Rattan
Crafting[609] = {Active = 1,	ItemID = 4660	,	ItemLv = 60	,	Material1 = 1350	,	Material2 = 1769	,	Material3 =	2616,	Rad = 1,	GoodItem = 0} -- Water of Miracle
Crafting[610] = {Active = 1,	ItemID = 878	,	ItemLv = 60	,	Material1 = 1684	,	Material2 = 3366	,	Material3 =	2604,	Rad = 1,	GoodItem = 0} -- Fiery Gem
Crafting[611] = {Active = 1,	ItemID = 879	,	ItemLv = 60	,	Material1 = 1221	,	Material2 = 3365	,	Material3 =	2604,	Rad = 1,	GoodItem = 0} -- Furious Gem
Crafting[612] = {Active = 1,	ItemID = 880	,	ItemLv = 60	,	Material1 = 1331	,	Material2 = 4044	,	Material3 =	2604,	Rad = 1,	GoodItem = 0} -- Explosive Gem
Crafting[613] = {Active = 1,	ItemID = 6		,	ItemLv = 60	,	Material1 = 3386	,	Material2 = 3366	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Wyrm Sword
Crafting[614] = {Active = 1,	ItemID = 1393	,	ItemLv = 60	,	Material1 = 1790	,	Material2 = 3365	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Hymn Sword of Darkan
Crafting[615] = {Active = 1,	ItemID = 3801	,	ItemLv = 60	,	Material1 = 1791	,	Material2 = 4044	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sword of the Kylin
Crafting[616] = {Active = 1,	ItemID = 17		,	ItemLv = 60	,	Material1 = 4608	,	Material2 = 3365	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Bone Sword
Crafting[617] = {Active = 1,	ItemID = 1374	,	ItemLv = 60	,	Material1 = 1788	,	Material2 = 4044	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Grey Sword of Darkan
Crafting[618] = {Active = 1,	ItemID = 1383	,	ItemLv = 60	,	Material1 = 1790	,	Material2 = 1769	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Soul Sword of Darkan
Crafting[619] = {Active = 1,	ItemID = 3804	,	ItemLv = 60	,	Material1 = 1791	,	Material2 = 3366	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Greatsword of the Tortoise
Crafting[620] = {Active = 1,	ItemID = 41		,	ItemLv = 60	,	Material1 = 1788	,	Material2 = 3365	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Laser Gun
Crafting[621] = {Active = 1,	ItemID = 1410	,	ItemLv = 60	,	Material1 = 1791	,	Material2 = 4044	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Holy Judgment
Crafting[622] = {Active = 1,	ItemID = 3808	,	ItemLv = 60	,	Material1 = 1793	,	Material2 = 1769	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Rifle of the White Tiger
Crafting[623] = {Active = 1,	ItemID = 78		,	ItemLv = 60	,	Material1 = 3386	,	Material2 = 4044	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Dagger of Hydra
Crafting[624] = {Active = 1,	ItemID = 1420	,	ItemLv = 60	,	Material1 = 1790	,	Material2 = 3366	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Venom Soul Spike
Crafting[625] = {Active = 1,	ItemID = 1448	,	ItemLv = 60	,	Material1 = 1791	,	Material2 = 3365	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Sovereign Kris
Crafting[626] = {Active = 1,	ItemID = 3819	,	ItemLv = 60	,	Material1 = 1793	,	Material2 = 4044	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Blade of the Green Dragon
Crafting[627] = {Active = 1,	ItemID = 1439	,	ItemLv = 60	,	Material1 = 1791	,	Material2 = 4044	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Wrath
Crafting[628] = {Active = 1,	ItemID = 1442	,	ItemLv = 60	,	Material1 = 1793	,	Material2 = 1769	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Tears of Angel
Crafting[629] = {Active = 1,	ItemID = 1474	,	ItemLv = 60	,	Material1 = 4606	,	Material2 = 3366	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Sun
Crafting[630] = {Active = 1,	ItemID = 1477	,	ItemLv = 60	,	Material1 = 4608	,	Material2 = 3365	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Eye of Sorrow
Crafting[631] = {Active = 1,	ItemID = 3812	,	ItemLv = 60	,	Material1 = 3386	,	Material2 = 4044	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of the Peacock
Crafting[632] = {Active = 1,	ItemID = 3815	,	ItemLv = 60	,	Material1 = 1788	,	Material2 = 1769	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of the Phoenix
Crafting[633] = {Active = 1,	ItemID = 4300	,	ItemLv = 60	,	Material1 = 1788	,	Material2 = 4044	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Amercement
Crafting[634] = {Active = 1,	ItemID = 4303	,	ItemLv = 60	,	Material1 = 1790	,	Material2 = 1769	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Holy Guidance
Crafting[635] = {Active = 1,	ItemID = 2195	,	ItemLv = 60	,	Material1 = 4034	,	Material2 = 1347	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pincer Cap
Crafting[636] = {Active = 1,	ItemID = 2201	,	ItemLv = 60	,	Material1 = 1483	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Otter Cap
Crafting[637] = {Active = 1,	ItemID = 2215	,	ItemLv = 60	,	Material1 = 1482	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Cap
Crafting[638] = {Active = 1,	ItemID = 1946	,	ItemLv = 60	,	Material1 = 4044	,	Material2 = 4977	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Coat of the White Tiger
Crafting[639] = {Active = 1,	ItemID = 377	,	ItemLv = 60	,	Material1 = 4046	,	Material2 = 1603	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Protector Robe
Crafting[640] = {Active = 1,	ItemID = 358	,	ItemLv = 60	,	Material1 = 1767	,	Material2 = 1609	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pincer Costume
Crafting[641] = {Active = 1,	ItemID = 317	,	ItemLv = 60	,	Material1 = 1635	,	Material2 = 4977	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Raptor Vest
Crafting[642] = {Active = 1,	ItemID = 304	,	ItemLv = 60	,	Material1 = 1632	,	Material2 = 4977	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Ceremonial Platemail
Crafting[643] = {Active = 1,	ItemID = 1958	,	ItemLv = 60	,	Material1 = 2815	,	Material2 = 1603	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Coat of the Peacock
Crafting[644] = {Active = 1,	ItemID = 1961	,	ItemLv = 60	,	Material1 = 1201	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Coat of the Phoenix
Crafting[645] = {Active = 1,	ItemID = 1979	,	ItemLv = 60	,	Material1 = 1647	,	Material2 = 4977	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Coat of the Green Dragon
Crafting[646] = {Active = 1,	ItemID = 344	,	ItemLv = 60	,	Material1 = 1258	,	Material2 = 4977	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Whirlpool Vest
Crafting[647] = {Active = 1,	ItemID = 364	,	ItemLv = 60	,	Material1 = 1775	,	Material2 = 1604	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Otter Costume
Crafting[648] = {Active = 1,	ItemID = 1931	,	ItemLv = 60	,	Material1 = 1239	,	Material2 = 4977	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Battle Armor of the Kylin
Crafting[649] = {Active = 1,	ItemID = 394	,	ItemLv = 60	,	Material1 = 3455	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Heavenly Vest
Crafting[650] = {Active = 1,	ItemID = 393	,	ItemLv = 60	,	Material1 = 3367	,	Material2 = 1605	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Costume
Crafting[651] = {Active = 1,	ItemID = 1950	,	ItemLv = 60	,	Material1 = 1483	,	Material2 = 1347	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Gloves of the White Tiger
Crafting[652] = {Active = 1,	ItemID = 553	,	ItemLv = 60	,	Material1 = 1494	,	Material2 = 1347	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Protector Gloves
Crafting[653] = {Active = 1,	ItemID = 493	,	ItemLv = 60	,	Material1 = 1632	,	Material2 = 1347	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Raptor Gloves
Crafting[654] = {Active = 1,	ItemID = 534	,	ItemLv = 60	,	Material1 = 4044	,	Material2 = 1347	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pincer Muffs
Crafting[655] = {Active = 1,	ItemID = 480	,	ItemLv = 60	,	Material1 = 1482	,	Material2 = 1347	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Ceremonial Gauntlets
Crafting[656] = {Active = 1,	ItemID = 1965	,	ItemLv = 60	,	Material1 = 1647	,	Material2 = 1347	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of the Peacock
Crafting[657] = {Active = 1,	ItemID = 1968	,	ItemLv = 60	,	Material1 = 3455	,	Material2 = 1347	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of the Phoenix
Crafting[658] = {Active = 1,	ItemID = 1983	,	ItemLv = 60	,	Material1 = 1201	,	Material2 = 1347	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Gloves of the Green Dragon
Crafting[659] = {Active = 1,	ItemID = 520	,	ItemLv = 60	,	Material1 = 1239	,	Material2 = 1347	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Whirlpool Gloves
Crafting[660] = {Active = 1,	ItemID = 1938	,	ItemLv = 60	,	Material1 = 1258	,	Material2 = 1347	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Gauntlets of the Kylin
Crafting[661] = {Active = 1,	ItemID = 540	,	ItemLv = 60	,	Material1 = 1494	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Otter Muffs
Crafting[662] = {Active = 1,	ItemID = 570	,	ItemLv = 60	,	Material1 = 1635	,	Material2 = 1347	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Heavenly Gloves
Crafting[663] = {Active = 1,	ItemID = 569	,	ItemLv = 60	,	Material1 = 4728	,	Material2 = 4977	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Muffs
Crafting[664] = {Active = 1,	ItemID = 729	,	ItemLv = 60	,	Material1 = 1483	,	Material2 = 4724	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Protector Boots
Crafting[665] = {Active = 1,	ItemID = 1954	,	ItemLv = 60	,	Material1 = 1647	,	Material2 = 4724	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Boots of the White Tiger
Crafting[666] = {Active = 1,	ItemID = 669	,	ItemLv = 60	,	Material1 = 1647	,	Material2 = 1730	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Raptor Boots
Crafting[667] = {Active = 1,	ItemID = 710	,	ItemLv = 60	,	Material1 = 1482	,	Material2 = 4724	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Pincer Shoes
Crafting[668] = {Active = 1,	ItemID = 656	,	ItemLv = 60	,	Material1 = 1494	,	Material2 = 4724	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Ceremonial Greaves
Crafting[669] = {Active = 1,	ItemID = 1972	,	ItemLv = 60	,	Material1 = 1494	,	Material2 = 4993	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of the Peacock
Crafting[670] = {Active = 1,	ItemID = 1987	,	ItemLv = 60	,	Material1 = 1205	,	Material2 = 1730	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Boots of the Green Dragon
Crafting[671] = {Active = 1,	ItemID = 1975	,	ItemLv = 60	,	Material1 = 4034	,	Material2 = 1730	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of the Phoenix
Crafting[672] = {Active = 1,	ItemID = 696	,	ItemLv = 60	,	Material1 = 1483	,	Material2 = 4993	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Whirlpool Boots
Crafting[673] = {Active = 1,	ItemID = 1942	,	ItemLv = 60	,	Material1 = 1483	,	Material2 = 1730	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Greaves of the Kylin
Crafting[674] = {Active = 1,	ItemID = 716	,	ItemLv = 60	,	Material1 = 1482	,	Material2 = 4974	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Otter Shoes
Crafting[675] = {Active = 1,	ItemID = 746	,	ItemLv = 60	,	Material1 = 1295	,	Material2 = 4993	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Heavenly Shoes
Crafting[676] = {Active = 1,	ItemID = 745	,	ItemLv = 60	,	Material1 = 4034	,	Material2 = 4974	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Shoes
Crafting[677] = {Active = 1,	ItemID = 230	,	ItemLv = 60	,	Material1 = 3379	,	Material2 = 1606	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Primal Tattoo
Crafting[678] = {Active = 1,	ItemID = 1934	,	ItemLv = 60	,	Material1 = 4036	,	Material2 = 1603	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Tattoo of the Tortoise
Crafting[679] = {Active = 1,	ItemID = 31		,	ItemLv = 62	,	Material1 = 3386	,	Material2 = 3366	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Bow of Dusk
Crafting[680] = {Active = 1,	ItemID = 776	,	ItemLv = 65	,	Material1 = 1788	,	Material2 = 3455	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Blade of Enigma
Crafting[681] = {Active = 1,	ItemID = 4212	,	ItemLv = 65	,	Material1 = 1793	,	Material2 = 1769	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Ember Scar
Crafting[682] = {Active = 1,	ItemID = 4213	,	ItemLv = 65	,	Material1 = 4606	,	Material2 = 3366	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Swift Lightning
Crafting[683] = {Active = 1,	ItemID = 773	,	ItemLv = 65	,	Material1 = 3386	,	Material2 = 3456	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Judgment of Enigma
Crafting[684] = {Active = 1,	ItemID = 4209	,	ItemLv = 65	,	Material1 = 1793	,	Material2 = 3365	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Roar of Turbulence
Crafting[685] = {Active = 1,	ItemID = 4210	,	ItemLv = 65	,	Material1 = 4606	,	Material2 = 4044	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Soul Beast
Crafting[686] = {Active = 1,	ItemID = 4211	,	ItemLv = 65	,	Material1 = 4608	,	Material2 = 1769	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Comet Magenta
Crafting[687] = {Active = 1,	ItemID = 784	,	ItemLv = 65	,	Material1 = 1790	,	Material2 = 1782	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Rifle of Enigma
Crafting[688] = {Active = 1,	ItemID = 3464	,	ItemLv = 65	,	Material1 = 1790	,	Material2 = 1782	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Bow of Enigma
Crafting[689] = {Active = 1,	ItemID = 4214	,	ItemLv = 65	,	Material1 = 4606	,	Material2 = 3366	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Rattlesnake
Crafting[690] = {Active = 1,	ItemID = 4215	,	ItemLv = 65	,	Material1 = 4608	,	Material2 = 3365	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Serpentine Gun of Resonance
Crafting[691] = {Active = 1,	ItemID = 802	,	ItemLv = 65	,	Material1 = 1788	,	Material2 = 1769	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Kris of the Sphinx
Crafting[692] = {Active = 1,	ItemID = 4216	,	ItemLv = 65	,	Material1 = 4606	,	Material2 = 1769	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Tooth of Eclipse
Crafting[693] = {Active = 1,	ItemID = 4217	,	ItemLv = 65	,	Material1 = 4608	,	Material2 = 3366	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Thunder Tooth
Crafting[694] = {Active = 1,	ItemID = 4218	,	ItemLv = 65	,	Material1 = 3386	,	Material2 = 3365	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Essence Dagger
Crafting[695] = {Active = 1,	ItemID = 788	,	ItemLv = 65	,	Material1 = 1788	,	Material2 = 3455	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Enigma
Crafting[696] = {Active = 1,	ItemID = 795	,	ItemLv = 65	,	Material1 = 1790	,	Material2 = 3456	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of the Sphinx
Crafting[697] = {Active = 1,	ItemID = 4197	,	ItemLv = 65	,	Material1 = 1790	,	Material2 = 3366	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Flame of the Arctic
Crafting[698] = {Active = 1,	ItemID = 4199	,	ItemLv = 65	,	Material1 = 1791	,	Material2 = 3365	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Searing Frost
Crafting[699] = {Active = 1,	ItemID = 4201	,	ItemLv = 65	,	Material1 = 1793	,	Material2 = 4044	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Claw of Demonic Dragon
Crafting[700] = {Active = 1,	ItemID = 4203	,	ItemLv = 65	,	Material1 = 4606	,	Material2 = 1769	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- God of Flame
Crafting[701] = {Active = 1,	ItemID = 4205	,	ItemLv = 65	,	Material1 = 4608	,	Material2 = 3366	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- God of Fire
Crafting[702] = {Active = 1,	ItemID = 4207	,	ItemLv = 65	,	Material1 = 3386	,	Material2 = 3365	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- God of Sight
Crafting[703] = {Active = 1,	ItemID = 127	,	ItemLv = 65	,	Material1 = 1791	,	Material2 = 3366	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Wyrm Shield
Crafting[704] = {Active = 1,	ItemID = 4161	,	ItemLv = 65	,	Material1 = 3366	,	Material2 = 1310	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Robe of Vengence
Crafting[705] = {Active = 1,	ItemID = 4153	,	ItemLv = 65	,	Material1 = 1769	,	Material2 = 1310	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Nautical Coat
Crafting[706] = {Active = 1,	ItemID = 798	,	ItemLv = 65	,	Material1 = 1295	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Robe of the Sphinx
Crafting[707] = {Active = 1,	ItemID = 780	,	ItemLv = 65	,	Material1 = 1632	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Mantle of Enigma
Crafting[708] = {Active = 1,	ItemID = 4151	,	ItemLv = 65	,	Material1 = 1632	,	Material2 = 178		,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Robe of Perception
Crafting[709] = {Active = 1,	ItemID = 806	,	ItemLv = 65	,	Material1 = 3365	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Mantle of the Sphinx
Crafting[710] = {Active = 1,	ItemID = 4162	,	ItemLv = 65	,	Material1 = 1769	,	Material2 = 179		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Otter Costume
Crafting[711] = {Active = 1,	ItemID = 4154	,	ItemLv = 65	,	Material1 = 3365	,	Material2 = 178		,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Arcane Lobster Costume
Crafting[712] = {Active = 1,	ItemID = 792	,	ItemLv = 65	,	Material1 = 1258	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Robe of Enigma
Crafting[713] = {Active = 1,	ItemID = 769	,	ItemLv = 65	,	Material1 = 1769	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Armor of Enigma
Crafting[714] = {Active = 1,	ItemID = 4149	,	ItemLv = 65	,	Material1 = 3366	,	Material2 = 178		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Capricious Battle Armor
Crafting[715] = {Active = 1,	ItemID = 4158	,	ItemLv = 65	,	Material1 = 4044	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Bunny Costume
Crafting[716] = {Active = 1,	ItemID = 4157	,	ItemLv = 65	,	Material1 = 3455	,	Material2 = 178		,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Robe of Melody
Crafting[717] = {Active = 1,	ItemID = 4177	,	ItemLv = 65	,	Material1 = 3362	,	Material2 = 1610	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of Vengence
Crafting[718] = {Active = 1,	ItemID = 4169	,	ItemLv = 65	,	Material1 = 1780	,	Material2 = 1603	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Nautical Gloves
Crafting[719] = {Active = 1,	ItemID = 4178	,	ItemLv = 65	,	Material1 = 1632	,	Material2 = 1310	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Otter Muffs
Crafting[720] = {Active = 1,	ItemID = 4167	,	ItemLv = 65	,	Material1 = 1295	,	Material2 = 1310	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Gloves of Perception
Crafting[721] = {Active = 1,	ItemID = 4170	,	ItemLv = 65	,	Material1 = 1258	,	Material2 = 1310	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Arcane Lobster Muffs
Crafting[722] = {Active = 1,	ItemID = 4174	,	ItemLv = 65	,	Material1 = 1268	,	Material2 = 1310	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Bunny Muffs
Crafting[723] = {Active = 1,	ItemID = 4165	,	ItemLv = 65	,	Material1 = 4034	,	Material2 = 1310	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Capricious Battle Gauntlets
Crafting[724] = {Active = 1,	ItemID = 4173	,	ItemLv = 65	,	Material1 = 1205	,	Material2 = 1310	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Gloves of Melody
Crafting[725] = {Active = 1,	ItemID = 4193	,	ItemLv = 65	,	Material1 = 1647	,	Material2 = 4974	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of Vengence
Crafting[726] = {Active = 1,	ItemID = 4185	,	ItemLv = 65	,	Material1 = 1314	,	Material2 = 4974	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Nautical Boots
Crafting[727] = {Active = 1,	ItemID = 4194	,	ItemLv = 65	,	Material1 = 1258	,	Material2 = 4756	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Otter Shoes
Crafting[728] = {Active = 1,	ItemID = 4183	,	ItemLv = 65	,	Material1 = 1785	,	Material2 = 4996	,	Material3 =	2594,	Rad = 1,	GoodItem = 0} -- Boots of Perception
Crafting[729] = {Active = 1,	ItemID = 4186	,	ItemLv = 65	,	Material1 = 1268	,	Material2 = 4756	,	Material3 =	2597,	Rad = 1,	GoodItem = 0} -- Arcane Lobster Shoes
Crafting[730] = {Active = 1,	ItemID = 4190	,	ItemLv = 65	,	Material1 = 4728	,	Material2 = 4756	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Arcane Bunny Shoes
Crafting[731] = {Active = 1,	ItemID = 4181	,	ItemLv = 65	,	Material1 = 1632	,	Material2 = 4756	,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Capricious Battle Greaves
Crafting[732] = {Active = 1,	ItemID = 4189	,	ItemLv = 65	,	Material1 = 1263	,	Material2 = 4993	,	Material3 =	2600,	Rad = 1,	GoodItem = 0} -- Boots of Melody
Crafting[733] = {Active = 1,	ItemID = 4147	,	ItemLv = 65	,	Material1 = 1268	,	Material2 = 178		,	Material3 =	2591,	Rad = 1,	GoodItem = 0} -- Spirit Beast Tattoo
Crafting[734] = {Active = 1,	ItemID = 766	,	ItemLv = 65	,	Material1 = 3366	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Tattoo of Enigma
Crafting[735] = {Active = 1,	ItemID = 860	,	ItemLv = 70	,	Material1 = 1716	,	Material2 = 4045	,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Gem of the Wind
Crafting[736] = {Active = 1,	ItemID = 861	,	ItemLv = 70	,	Material1 = 1492	,	Material2 = 2815	,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Gem of Striking
Crafting[737] = {Active = 1,	ItemID = 862	,	ItemLv = 70	,	Material1 = 1716	,	Material2 = 4045	,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Gem of Colossus
Crafting[738] = {Active = 1,	ItemID = 863	,	ItemLv = 70	,	Material1 = 1492	,	Material2 = 2815	,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Gem of Rage
Crafting[739] = {Active = 1,	ItemID = 1012	,	ItemLv = 70	,	Material1 = 1716	,	Material2 = 4045	,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Gem of Soul
Crafting[740] = {Active = 1,	ItemID = 7		,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sacro Sword
Crafting[741] = {Active = 1,	ItemID = 113	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Draco
Crafting[742] = {Active = 1,	ItemID = 1394	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sword of Dawn
Crafting[743] = {Active = 1,	ItemID = 18		,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Thunder Blade
Crafting[744] = {Active = 1,	ItemID = 115	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Crag
Crafting[745] = {Active = 1,	ItemID = 1375	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Primal Axe of Rage
Crafting[746] = {Active = 1,	ItemID = 1384	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Winds of Death
Crafting[747] = {Active = 1,	ItemID = 117	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Rainbow
Crafting[748] = {Active = 1,	ItemID = 42		,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Venom Gun
Crafting[749] = {Active = 1,	ItemID = 119	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Meteor Pearl
Crafting[750] = {Active = 1,	ItemID = 1411	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Kiss of the Serpent
Crafting[751] = {Active = 1,	ItemID = 79		,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Trident of Poseidon
Crafting[752] = {Active = 1,	ItemID = 150	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Visceral
Crafting[753] = {Active = 1,	ItemID = 1421	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Tooth of Fury
Crafting[754] = {Active = 1,	ItemID = 1449	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Clarity Dagger
Crafting[755] = {Active = 1,	ItemID = 109	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Wonders
Crafting[756] = {Active = 1,	ItemID = 111	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Demon Bane Rod
Crafting[757] = {Active = 1,	ItemID = 4198	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Soul Spring
Crafting[758] = {Active = 1,	ItemID = 4200	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Consonance of Souls
Crafting[759] = {Active = 1,	ItemID = 4202	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Eye of Sacred Dragon
Crafting[760] = {Active = 1,	ItemID = 4204	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gold of Tears
Crafting[761] = {Active = 1,	ItemID = 4206	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- God of Rebuke
Crafting[762] = {Active = 1,	ItemID = 4208	,	ItemLv = 70	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- God of Wrath
Crafting[763] = {Active = 1,	ItemID = 2548	,	ItemLv = 70	,	Material1 = 3371	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Artemis Crownstone
Crafting[764] = {Active = 1,	ItemID = 2531	,	ItemLv = 70	,	Material1 = 1315	,	Material2 = 1347	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Hephaestus Clawstone
Crafting[765] = {Active = 1,	ItemID = 2532	,	ItemLv = 70	,	Material1 = 1297	,	Material2 = 1712	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Hephaestus Pawstone
Crafting[766] = {Active = 1,	ItemID = 2530	,	ItemLv = 70	,	Material1 = 1333	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Hephaestus Framestone
Crafting[767] = {Active = 1,	ItemID = 2530	,	ItemLv = 70	,	Material1 = 3820	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hephaestus Framestone
Crafting[768] = {Active = 1,	ItemID = 7		,	ItemLv = 70	,	Material1 = 3820	,	Material2 = 1734	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Sacro Sword
Crafting[769] = {Active = 1,	ItemID = 2536	,	ItemLv = 70	,	Material1 = 4035	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Apollo Framestone
Crafting[770] = {Active = 1,	ItemID = 111	,	ItemLv = 70	,	Material1 = 3444	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Demon Bane Rod
Crafting[771] = {Active = 1,	ItemID = 2539	,	ItemLv = 70	,	Material1 = 3432	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Poseidon Framestone
Crafting[772] = {Active = 1,	ItemID = 4163	,	ItemLv = 70	,	Material1 = 3371	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Robe of Amercement
Crafting[773] = {Active = 1,	ItemID = 2533	,	ItemLv = 70	,	Material1 = 4045	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hermes Framestone
Crafting[774] = {Active = 1,	ItemID = 4156	,	ItemLv = 70	,	Material1 = 2901	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Pique Lobster Costume
Crafting[775] = {Active = 1,	ItemID = 18		,	ItemLv = 70	,	Material1 = 4043	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Thunder Blade
Crafting[776] = {Active = 1,	ItemID = 4150	,	ItemLv = 70	,	Material1 = 4825	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sagacious Battle Armor
Crafting[777] = {Active = 1,	ItemID = 109	,	ItemLv = 70	,	Material1 = 1643	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Staff of Wonders
Crafting[778] = {Active = 1,	ItemID = 113	,	ItemLv = 70	,	Material1 = 3369	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Draco
Crafting[779] = {Active = 1,	ItemID = 4160	,	ItemLv = 70	,	Material1 = 1780	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Bunny Costume
Crafting[780] = {Active = 1,	ItemID = 4159	,	ItemLv = 70	,	Material1 = 4035	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Robe of Spirit
Crafting[781] = {Active = 1,	ItemID = 397	,	ItemLv = 70	,	Material1 = 1352	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Armor of Hercules
Crafting[782] = {Active = 1,	ItemID = 4164	,	ItemLv = 70	,	Material1 = 3821	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Otter Costume
Crafting[783] = {Active = 1,	ItemID = 79		,	ItemLv = 70	,	Material1 = 1355	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Trident of Poseidon
Crafting[784] = {Active = 1,	ItemID = 2542	,	ItemLv = 70	,	Material1 = 4043	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hestia Framestone
Crafting[785] = {Active = 1,	ItemID = 42		,	ItemLv = 70	,	Material1 = 3362	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Venom Gun
Crafting[786] = {Active = 1,	ItemID = 4155	,	ItemLv = 70	,	Material1 = 1363	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Robe of Stride
Crafting[787] = {Active = 1,	ItemID = 4152	,	ItemLv = 70	,	Material1 = 4045	,	Material2 = 1734	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Robe of Gallant
Crafting[788] = {Active = 1,	ItemID = 2545	,	ItemLv = 70	,	Material1 = 3444	,	Material2 = 1621	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Athena Framestone
Crafting[789] = {Active = 1,	ItemID = 4160	,	ItemLv = 70	,	Material1 = 3365	,	Material2 = 1347	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Pique Bunny Costume
Crafting[790] = {Active = 1,	ItemID = 117	,	ItemLv = 70	,	Material1 = 1297	,	Material2 = 1347	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Rainbow
Crafting[791] = {Active = 1,	ItemID = 2537	,	ItemLv = 70	,	Material1 = 3430	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Apollo Clawstone
Crafting[792] = {Active = 1,	ItemID = 2540	,	ItemLv = 70	,	Material1 = 3456	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Poseidon Clawstone
Crafting[793] = {Active = 1,	ItemID = 4179	,	ItemLv = 70	,	Material1 = 1271	,	Material2 = 1347	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gloves of Amercement
Crafting[794] = {Active = 1,	ItemID = 2531	,	ItemLv = 70	,	Material1 = 3427	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hephaestus Clawstone
Crafting[795] = {Active = 1,	ItemID = 4172	,	ItemLv = 70	,	Material1 = 3365	,	Material2 = 1712	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Pique Lobster Muffs
Crafting[796] = {Active = 1,	ItemID = 2534	,	ItemLv = 70	,	Material1 = 3433	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hermes Clawstone
Crafting[797] = {Active = 1,	ItemID = 119	,	ItemLv = 70	,	Material1 = 1263	,	Material2 = 1347	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Meteor Pearl
Crafting[798] = {Active = 1,	ItemID = 603	,	ItemLv = 70	,	Material1 = 1271	,	Material2 = 1712	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Gauntlets of Hercules
Crafting[799] = {Active = 1,	ItemID = 4166	,	ItemLv = 70	,	Material1 = 3428	,	Material2 = 1712	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sagacious Battle Gauntlets
Crafting[800] = {Active = 1,	ItemID = 4159	,	ItemLv = 70	,	Material1 = 1333	,	Material2 = 1347	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Robe of Spirit
Crafting[801] = {Active = 1,	ItemID = 4176	,	ItemLv = 70	,	Material1 = 1315	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Bunny Muffs
Crafting[802] = {Active = 1,	ItemID = 4163	,	ItemLv = 70	,	Material1 = 3425	,	Material2 = 1712	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Robe of Amercement
Crafting[803] = {Active = 1,	ItemID = 4168	,	ItemLv = 70	,	Material1 = 3431	,	Material2 = 1712	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Gloves of Gallant
Crafting[804] = {Active = 1,	ItemID = 4171	,	ItemLv = 70	,	Material1 = 3429	,	Material2 = 1621	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Gloves of Stride
Crafting[805] = {Active = 1,	ItemID = 4175	,	ItemLv = 70	,	Material1 = 3426	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gloves of Spirit
Crafting[806] = {Active = 1,	ItemID = 115	,	ItemLv = 70	,	Material1 = 3455	,	Material2 = 1712	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Crag
Crafting[807] = {Active = 1,	ItemID = 4156	,	ItemLv = 70	,	Material1 = 3427	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Lobster Costume
Crafting[808] = {Active = 1,	ItemID = 4180	,	ItemLv = 70	,	Material1 = 1644	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Otter Muffs
Crafting[809] = {Active = 1,	ItemID = 2543	,	ItemLv = 70	,	Material1 = 3425	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hestia Clawstone
Crafting[810] = {Active = 1,	ItemID = 150	,	ItemLv = 70	,	Material1 = 1263	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Visceral
Crafting[811] = {Active = 1,	ItemID = 2546	,	ItemLv = 70	,	Material1 = 3432	,	Material2 = 1734	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Athena Clawstone
Crafting[812] = {Active = 1,	ItemID = 861	,	ItemLv = 70	,	Material1 = 1333	,	Material2 = 4977	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Gem of Striking
Crafting[813] = {Active = 1,	ItemID = 1384	,	ItemLv = 70	,	Material1 = 1315	,	Material2 = 4977	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Winds of Death
Crafting[814] = {Active = 1,	ItemID = 2538	,	ItemLv = 70	,	Material1 = 1782	,	Material2 = 4958	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Apollo Pawstone
Crafting[815] = {Active = 1,	ItemID = 4195	,	ItemLv = 70	,	Material1 = 1297	,	Material2 = 4977	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Boots of Amercement
Crafting[816] = {Active = 1,	ItemID = 2532	,	ItemLv = 70	,	Material1 = 3430	,	Material2 = 1697	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hephaestus Pawstone
Crafting[817] = {Active = 1,	ItemID = 2541	,	ItemLv = 70	,	Material1 = 3456	,	Material2 = 4939	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Poseidon Pawstone
Crafting[818] = {Active = 1,	ItemID = 4188	,	ItemLv = 70	,	Material1 = 1333	,	Material2 = 4974	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Pique Lobster Shoes
Crafting[819] = {Active = 1,	ItemID = 862	,	ItemLv = 70	,	Material1 = 1271	,	Material2 = 4977	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gem of Colossus
Crafting[820] = {Active = 1,	ItemID = 2535	,	ItemLv = 70	,	Material1 = 3371	,	Material2 = 4977	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hermes Pawstone
Crafting[821] = {Active = 1,	ItemID = 1375	,	ItemLv = 70	,	Material1 = 1644	,	Material2 = 4977	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Primal Axe of Rage
Crafting[822] = {Active = 1,	ItemID = 4192	,	ItemLv = 70	,	Material1 = 1644	,	Material2 = 4974	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Bunny Shoes
Crafting[823] = {Active = 1,	ItemID = 829	,	ItemLv = 70	,	Material1 = 1271	,	Material2 = 4958	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Greaves of Hercules
Crafting[824] = {Active = 1,	ItemID = 4184	,	ItemLv = 70	,	Material1 = 1335	,	Material2 = 1730	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Boots of Gallant
Crafting[825] = {Active = 1,	ItemID = 1394	,	ItemLv = 70	,	Material1 = 1315	,	Material2 = 4958	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Sword of Dawn
Crafting[826] = {Active = 1,	ItemID = 4187	,	ItemLv = 70	,	Material1 = 1783	,	Material2 = 1730	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Boots of Stride
Crafting[827] = {Active = 1,	ItemID = 4191	,	ItemLv = 70	,	Material1 = 1644	,	Material2 = 4958	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Boots of Spirit
Crafting[828] = {Active = 1,	ItemID = 4182	,	ItemLv = 70	,	Material1 = 1333	,	Material2 = 4958	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Sagacious Battle Greaves
Crafting[829] = {Active = 1,	ItemID = 829	,	ItemLv = 70	,	Material1 = 1783	,	Material2 = 1712	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Greaves of Hercules
Crafting[830] = {Active = 1,	ItemID = 1012	,	ItemLv = 70	,	Material1 = 1335	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gem of Soul
Crafting[831] = {Active = 1,	ItemID = 4196	,	ItemLv = 70	,	Material1 = 1315	,	Material2 = 4974	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Pique Otter Shoes
Crafting[832] = {Active = 1,	ItemID = 2544	,	ItemLv = 70	,	Material1 = 3821	,	Material2 = 4936	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Hestia Pawstone
Crafting[833] = {Active = 1,	ItemID = 863	,	ItemLv = 70	,	Material1 = 1297	,	Material2 = 4958	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gem of Rage
Crafting[834] = {Active = 1,	ItemID = 2547	,	ItemLv = 70	,	Material1 = 3433	,	Material2 = 4956	,	Material3 =	2666,	Rad = 1,	GoodItem = 0} -- Athena Pawstone
Crafting[835] = {Active = 1,	ItemID = 4148	,	ItemLv = 70	,	Material1 = 1360	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Arcane Beast Tattoo
Crafting[836] = {Active = 1,	ItemID = 397	,	ItemLv = 70	,	Material1 = 1782	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Armor of Hercules
Crafting[837] = {Active = 1,	ItemID = 114	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Drakan
Crafting[838] = {Active = 1,	ItemID = 116	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Colossus
Crafting[839] = {Active = 1,	ItemID = 118	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Twilight
Crafting[840] = {Active = 1,	ItemID = 120	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 4035	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Blitz Thunderbolt
Crafting[841] = {Active = 1,	ItemID = 151	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Riven Soul
Crafting[842] = {Active = 1,	ItemID = 110	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 3362	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Revered Staff
Crafting[843] = {Active = 1,	ItemID = 112	,	ItemLv = 75	,	Material1 = 1702	,	Material2 = 1775	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Crimson Rod
Crafting[844] = {Active = 1,	ItemID = 2223	,	ItemLv = 75	,	Material1 = 1767	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Dragon Lord Cap
Crafting[845] = {Active = 1,	ItemID = 2219	,	ItemLv = 75	,	Material1 = 4037	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Mystic Panda Cap
Crafting[846] = {Active = 1,	ItemID = 2221	,	ItemLv = 75	,	Material1 = 1775	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Fish Fairy Cap
Crafting[847] = {Active = 1,	ItemID = 400	,	ItemLv = 75	,	Material1 = 1314	,	Material2 = 1734	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Vest of Apollo
Crafting[848] = {Active = 1,	ItemID = 406	,	ItemLv = 75	,	Material1 = 3367	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Faerie Robe
Crafting[849] = {Active = 1,	ItemID = 411	,	ItemLv = 75	,	Material1 = 4036	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Tsunami Robe
Crafting[850] = {Active = 1,	ItemID = 413	,	ItemLv = 75	,	Material1 = 4046	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Dragon Lord Costume
Crafting[851] = {Active = 1,	ItemID = 404	,	ItemLv = 75	,	Material1 = 1263	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Mystic Panda Costume
Crafting[852] = {Active = 1,	ItemID = 402	,	ItemLv = 75	,	Material1 = 3365	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Robe of the Sage
Crafting[853] = {Active = 1,	ItemID = 408	,	ItemLv = 75	,	Material1 = 1295	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Fish Fairy Costume
Crafting[854] = {Active = 1,	ItemID = 396	,	ItemLv = 75	,	Material1 = 1785	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Armor of Secrets
Crafting[855] = {Active = 1,	ItemID = 590	,	ItemLv = 75	,	Material1 = 1642	,	Material2 = 1734	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Gloves of Apollo
Crafting[856] = {Active = 1,	ItemID = 600	,	ItemLv = 75	,	Material1 = 1633	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Tsunami Gloves
Crafting[857] = {Active = 1,	ItemID = 596	,	ItemLv = 75	,	Material1 = 3378	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Faerie Gloves
Crafting[858] = {Active = 1,	ItemID = 602	,	ItemLv = 75	,	Material1 = 1201	,	Material2 = 1734	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Dragon Lord Muffs
Crafting[859] = {Active = 1,	ItemID = 588	,	ItemLv = 75	,	Material1 = 1783	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Gloves of Secrets
Crafting[860] = {Active = 1,	ItemID = 594	,	ItemLv = 75	,	Material1 = 4047	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Mystic Panda Gloves
Crafting[861] = {Active = 1,	ItemID = 604	,	ItemLv = 75	,	Material1 = 1674	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Gauntlets of Olympus
Crafting[862] = {Active = 1,	ItemID = 592	,	ItemLv = 75	,	Material1 = 1335	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Gloves of the Sage
Crafting[863] = {Active = 1,	ItemID = 598	,	ItemLv = 75	,	Material1 = 2815	,	Material2 = 1734	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Fish Fairy Muffs
Crafting[864] = {Active = 1,	ItemID = 760	,	ItemLv = 75	,	Material1 = 1352	,	Material2 = 4756	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Tsunami Shoes
Crafting[865] = {Active = 1,	ItemID = 756	,	ItemLv = 75	,	Material1 = 1363	,	Material2 = 4756	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Faerie Shoes
Crafting[866] = {Active = 1,	ItemID = 824	,	ItemLv = 75	,	Material1 = 1360	,	Material2 = 4756	,	Material3 =	2598,	Rad = 1,	GoodItem = 0} -- Dragon Lord Shoes
Crafting[867] = {Active = 1,	ItemID = 750	,	ItemLv = 75	,	Material1 = 1363	,	Material2 = 1712	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Boots of Apollo
Crafting[868] = {Active = 1,	ItemID = 748	,	ItemLv = 75	,	Material1 = 1360	,	Material2 = 4993	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Boots of Secrets
Crafting[869] = {Active = 1,	ItemID = 830	,	ItemLv = 75	,	Material1 = 1355	,	Material2 = 4756	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Greaves of Olympus
Crafting[870] = {Active = 1,	ItemID = 754	,	ItemLv = 75	,	Material1 = 1355	,	Material2 = 1712	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Mystic Panda Shoes
Crafting[871] = {Active = 1,	ItemID = 752	,	ItemLv = 75	,	Material1 = 1352	,	Material2 = 4993	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Boots of the Sage
Crafting[872] = {Active = 1,	ItemID = 758	,	ItemLv = 75	,	Material1 = 3367	,	Material2 = 4993	,	Material3 =	2601,	Rad = 1,	GoodItem = 0} -- Fish Fairy Shoes
Crafting[873] = {Active = 1,	ItemID = 398	,	ItemLv = 75	,	Material1 = 3379	,	Material2 = 1734	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Armor of Olympus
Crafting[874] = {Active = 1,	ItemID = 2368	,	ItemLv = 75	,	Material1 = 4038	,	Material2 = 1712	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Rightful Black Dragon Claw
Crafting[875] = {Active = 1,	ItemID = 1098	,	ItemLv = 75	,	Material1 = 4047	,	Material2 = 1703	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Greater Health Regenerator
Crafting[876] = {Active = 1,	ItemID = 1111	,	ItemLv = 75	,	Material1 = 4037	,	Material2 = 1703	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Lost Ring
Crafting[877] = {Active = 1,	ItemID = 1104	,	ItemLv = 75	,	Material1 = 1674	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Bermuda Chest
Crafting[878] = {Active = 1,	ItemID = 1114	,	ItemLv = 75	,	Material1 = 4038	,	Material2 = 1734	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Light Ring
Crafting[879] = {Active = 1,	ItemID = 1107	,	ItemLv = 75	,	Material1 = 2901	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Desert Ring
Crafting[880] = {Active = 1,	ItemID = 1101	,	ItemLv = 75	,	Material1 = 4832	,	Material2 = 3385	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Heart of Demonic Dragon
Crafting[881] = {Active = 1,	ItemID = 1108	,	ItemLv = 75	,	Material1 = 4048	,	Material2 = 1734	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Tranquil Ring
Crafting[882] = {Active = 1,	ItemID = 2369	,	ItemLv = 75	,	Material1 = 4832	,	Material2 = 1734	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Rightful Black Dragon Wings
Crafting[883] = {Active = 1,	ItemID = 2553	,	ItemLv = 75	,	Material1 = 4048	,	Material2 = 1703	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Ami SiSi  Hat
Crafting[884] = {Active = 1,	ItemID = 2370	,	ItemLv = 75	,	Material1 = 4047	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Rightful Head of Black Dragon
Crafting[885] = {Active = 1,	ItemID = 1102	,	ItemLv = 75	,	Material1 = 1674	,	Material2 = 1703	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Fried Dough
Crafting[886] = {Active = 1,	ItemID = 1109	,	ItemLv = 75	,	Material1 = 4037	,	Material2 = 4756	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Ring of Nature
Crafting[887] = {Active = 1,	ItemID = 2367	,	ItemLv = 75	,	Material1 = 4038	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Corporeal Black Dragon Torso
Crafting[888] = {Active = 1,	ItemID = 2371	,	ItemLv = 75	,	Material1 = 1360	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Aladdin Parcel
Crafting[889] = {Active = 1,	ItemID = 1103	,	ItemLv = 75	,	Material1 = 1352	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- The Sorrow Summoning Scroll
Crafting[890] = {Active = 1,	ItemID = 1110	,	ItemLv = 75	,	Material1 = 4037	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Snow Whisper Ring
Crafting[891] = {Active = 1,	ItemID = 1100	,	ItemLv = 75	,	Material1 = 1780	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Greater Mana Regenerator
Crafting[892] = {Active = 1,	ItemID = 1106	,	ItemLv = 75	,	Material1 = 4037	,	Material2 = 1734	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Yahoo hundred Treasure Chest
Crafting[893] = {Active = 1,	ItemID = 1113	,	ItemLv = 75	,	Material1 = 1363	,	Material2 = 1621	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Raptor Ring
Crafting[894] = {Active = 1,	ItemID = 1105	,	ItemLv = 75	,	Material1 = 2901	,	Material2 = 3385	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Loneliness Summoning Scroll
Crafting[895] = {Active = 1,	ItemID = 1099	,	ItemLv = 75	,	Material1 = 4832	,	Material2 = 1734	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Mana Regenerator
Crafting[896] = {Active = 1,	ItemID = 1112	,	ItemLv = 75	,	Material1 = 4038	,	Material2 = 1703	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Bloodythirsty Ring
Crafting[897] = {Active = 1,	ItemID = 864	,	ItemLv = 80	,	Material1 = 1626	,	Material2 = 4048	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Eye of Black Dragon
Crafting[898] = {Active = 1,	ItemID = 865	,	ItemLv = 80	,	Material1 = 1626	,	Material2 = 4048	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Soul of Black Dragon
Crafting[899] = {Active = 1,	ItemID = 866	,	ItemLv = 80	,	Material1 = 1626	,	Material2 = 4048	,	Material3 =	2665,	Rad = 1,	GoodItem = 0} -- Heart of Black Dragon
Crafting[900] = {Active = 1,	ItemID = 19		,	ItemLv = 80	,	Material1 = 2490	,	Material2 = 3379	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Onyx Sword
Crafting[901] = {Active = 1,	ItemID = 1376	,	ItemLv = 80	,	Material1 = 2490	,	Material2 = 4036	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Vampiric Bat King
Crafting[902] = {Active = 1,	ItemID = 1385	,	ItemLv = 80	,	Material1 = 2490	,	Material2 = 1767	,	Material3 =	2592,	Rad = 1,	GoodItem = 0} -- Wings of Death
Crafting[903] = {Active = 1,	ItemID = 43		,	ItemLv = 80	,	Material1 = 2490	,	Material2 = 3379	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Firegun of Sol
Crafting[904] = {Active = 1,	ItemID = 1412	,	ItemLv = 80	,	Material1 = 2490	,	Material2 = 4036	,	Material3 =	2595,	Rad = 1,	GoodItem = 0} -- Lightning Taser

Cooking =  {}
Cooking[1 ] = {Active = 1,	ItemID = 1848,	ItemLv = 10,	Material1 =	1576,	Material2 =	4411,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Bread
Cooking[2 ] = {Active = 1,	ItemID = 3133,	ItemLv = 10,	Material1 =	1576,	Material2 =	1704,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Liquorice Potion
Cooking[3 ] = {Active = 1,	ItemID = 4019,	ItemLv = 10,	Material1 =	4421,	Material2 =	4009,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Codfish Steamboat
Cooking[4 ] = {Active = 1,	ItemID = 1849,	ItemLv = 10,	Material1 =	4049,	Material2 =	1680,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Cake
Cooking[5 ] = {Active = 1,	ItemID = 3122,	ItemLv = 10,	Material1 =	4049,	Material2 =	1722,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Elven Fruit Juice
Cooking[6 ] = {Active = 1,	ItemID = 3134,	ItemLv = 10,	Material1 =	1686,	Material2 =	4419,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Energetic Tea
Cooking[7 ] = {Active = 1,	ItemID = 3135,	ItemLv = 10,	Material1 =	4477,	Material2 =	4390,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Special Ointment
Cooking[8 ] = {Active = 1,	ItemID = 4020,	ItemLv = 20,	Material1 =	4466,	Material2 =	4010,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Sturgeon Fish with Bamboo
Cooking[9 ] = {Active = 1,	ItemID = 3123,	ItemLv = 20,	Material1 =	4476,	Material2 =	4010,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Red Date Tea
Cooking[10] = {Active = 1,	ItemID = 3124,	ItemLv = 20,	Material1 =	1303,	Material2 =	4442,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Mushroom Soup
Cooking[11] = {Active = 1,	ItemID = 3136,	ItemLv = 20,	Material1 =	4352,	Material2 =	4393,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Snowy Soft Bud
Cooking[12] = {Active = 1,	ItemID = 3137,	ItemLv = 20,	Material1 =	4050,	Material2 =	4355,	Material3 =	3116,	Rad = 1,	GoodItem = 0} -- Tiamari Fruit
Cooking[13] = {Active = 1,	ItemID = 4021,	ItemLv = 30,	Material1 =	1578,	Material2 =	4011,	Material3 =	2617,	Rad = 1,	GoodItem = 0} -- Savory Bubble Fish
Cooking[14] = {Active = 1,	ItemID = 3125,	ItemLv = 30,	Material1 =	1578,	Material2 =	4357,	Material3 =	2617,	Rad = 1,	GoodItem = 0} -- Stramonium Fruit Juice
Cooking[15] = {Active = 1,	ItemID = 3126,	ItemLv = 30,	Material1 =	1578,	Material2 =	4452,	Material3 =	2617,	Rad = 1,	GoodItem = 0} -- Ice Cream
Cooking[16] = {Active = 1,	ItemID = 3138,	ItemLv = 30,	Material1 =	1578,	Material2 =	4387,	Material3 =	2617,	Rad = 1,	GoodItem = 0} -- Mystery Fruit
Cooking[17] = {Active = 1,	ItemID = 3139,	ItemLv = 30,	Material1 =	1578,	Material2 =	4462,	Material3 =	2617,	Rad = 1,	GoodItem = 0} -- Agrypnotic
Cooking[18] = {Active = 1,	ItemID = 4022,	ItemLv = 40,	Material1 =	4804,	Material2 =	4012,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Sturgeon Soup
Cooking[19] = {Active = 1,	ItemID = 1078,	ItemLv = 40,	Material1 =	4915,	Material2 =	4809,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Steam Bun
Cooking[20] = {Active = 1,	ItemID = 1079,	ItemLv = 40,	Material1 =	4060,	Material2 =	4540,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Bun
Cooking[21] = {Active = 1,	ItemID = 1084,	ItemLv = 40,	Material1 =	4804,	Material2 =	4720,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Maiden Wine
Cooking[22] = {Active = 1,	ItemID = 1085,	ItemLv = 40,	Material1 =	4915,	Material2 =	1267,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Scholar Wine
Cooking[23] = {Active = 1,	ItemID = 3127,	ItemLv = 40,	Material1 =	4060,	Material2 =	4809,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Rainbow Fruit Juice
Cooking[24] = {Active = 1,	ItemID = 3128,	ItemLv = 40,	Material1 =	4804,	Material2 =	4720,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Fruity Mix
Cooking[25] = {Active = 1,	ItemID = 3099,	ItemLv = 40,	Material1 =	4915,	Material2 =	4809,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- SP Holy Water
Cooking[26] = {Active = 1,	ItemID = 3140,	ItemLv = 40,	Material1 =	4060,	Material2 =	4720,	Material3 =	2588,	Rad = 1,	GoodItem = 0} -- Magical Potion
Cooking[27] = {Active = 1,	ItemID = 4023,	ItemLv = 50,	Material1 =	4055,	Material2 =	4013,	Material3 =	2619,	Rad = 1,	GoodItem = 0} -- Fried Oyster Soup
Cooking[28] = {Active = 1,	ItemID = 1080,	ItemLv = 50,	Material1 =	4730,	Material2 =	4955,	Material3 =	2619,	Rad = 1,	GoodItem = 0} -- Biscuit
Cooking[29] = {Active = 1,	ItemID = 1082,	ItemLv = 50,	Material1 =	4730,	Material2 =	1324,	Material3 =	2619,	Rad = 1,	GoodItem = 0} -- Fried Dough
Cooking[30] = {Active = 1,	ItemID = 1088,	ItemLv = 50,	Material1 =	4730,	Material2 =	1291,	Material3 =	2619,	Rad = 1,	GoodItem = 0} -- Dukan Wine
Cooking[31] = {Active = 1,	ItemID = 1087,	ItemLv = 50,	Material1 =	4730,	Material2 =	1358,	Material3 =	2619,	Rad = 1,	GoodItem = 0} -- Mao Wine
Cooking[32] = {Active = 1,	ItemID = 1860,	ItemLv = 60,	Material1 =	4061,	Material2 =	1329,	Material3 =	2589,	Rad = 1,	GoodItem = 0} -- Blessed Potion
Cooking[33] = {Active = 1,	ItemID = 4024,	ItemLv = 60,	Material1 =	4061,	Material2 =	4014,	Material3 =	2622,	Rad = 1,	GoodItem = 0} -- Prawn Dumpling
Cooking[34] = {Active = 1,	ItemID = 1083,	ItemLv = 60,	Material1 =	4864,	Material2 =	1735,	Material3 =	2622,	Rad = 1,	GoodItem = 0} -- Spring Roll
Cooking[35] = {Active = 1,	ItemID = 1089,	ItemLv = 60,	Material1 =	4831,	Material2 =	1359,	Material3 =	2622,	Rad = 1,	GoodItem = 0} -- Ginseng Wine
Cooking[36] = {Active = 1,	ItemID = 1090,	ItemLv = 60,	Material1 =	4727,	Material2 =	4792,	Material3 =	2622,	Rad = 1,	GoodItem = 0} -- Tiger Bone Tonic
Cooking[37] = {Active = 1,	ItemID = 4025,	ItemLv = 70,	Material1 =	2225,	Material2 =	4015,	Material3 =	2624,	Rad = 1,	GoodItem = 0} -- Tigerfish Bone Crisp
Cooking[38] = {Active = 1,	ItemID = 4026,	ItemLv = 80,	Material1 =	4057,	Material2 =	4016,	Material3 =	2624,	Rad = 1,	GoodItem = 0} -- Ratfish Rice
Cooking[39] = {Active = 1,	ItemID = 4027,	ItemLv = 90,	Material1 =	1768,	Material2 =	4017,	Material3 =	2624,	Rad = 1,	GoodItem = 0} -- China Clay
Cooking[40] = {Active = 1,	ItemID = 4028,	ItemLv = 100,	Material1 =	1768,	Material2 =	4018,	Material3 =	2624,	Rad = 1,	GoodItem = 0} -- BBQ Shark Fin

Analyze = Analyze or {}
Analyze.Catalyst = {}

-- [2625] Stone Catalyst  
Analyze.Catalyst[2625] = {} 
Analyze.Catalyst[2625][0] = {3387,1671,1670,4415,4029,4039,1640,1706,3363,3368,1668,1784} 
Analyze.Catalyst[2625][20] = {4030,4040,1634,4351,4349,1673,3391,3360,1667,1662,3388,1639,1786,3380}   
Analyze.Catalyst[2625][30] = {1771,4031,4041,3425,1196,3428,1216,1630,1781,4467,1631,1643,1642,1751,4363,1645,3426,1636,4536,3431,1669,1672}   
Analyze.Catalyst[2625][40] = {3442,3927,1666,1699,4368,4455,3429,3364,4032,4042,4541,1234,1253,3369,1638,4825,1288,1308,1326,3378,4850,4791,4895}   
Analyze.Catalyst[2625][50] = {3432,3444,3371,1637,3427,3820,4033,4043,4721,1345,3390,3433,3821,1641,1633,3389,3430,3361,1201,1635,1239,1647}   
Analyze.Catalyst[2625][60] = {1483,1494,1482,4034,444,3455,1632,3456,1258,3366,1769,1782,1268,1205,1295,1314,1263,3365,4728,1297,1785}    
Analyze.Catalyst[2625][70] = {1271,4035,4045,1315,1333,1644,1335,1783,1775,2815,1363,1355,1352,1360,3362}  
Analyze.Catalyst[2625][80] = {3367,1780,3379,4036,4046,1767,1674,4037,4047,2901,4832,4038,4048}

-- [2630] Food Catalyst 
Analyze.Catalyst[2630] = {}  
Analyze.Catalyst[2630][0] = {1690,1622,4320,1587,4484,1676,1704,1746,4404,4325,4329,4411,1595,1680,4417,4009,1722,4419,1689,4425,4426,4491,4390,4428} 
Analyze.Catalyst[2630][20] = {4457,4494,4010,4463,4433,4442,4393,4528,4355,4464,4529,4530,1193,1213,1284,1304}    
Analyze.Catalyst[2630][30] = {1188,1279,4357,4461,4011,1194,1214,1285,1305,1195,1286,4532,1215,1306,4382,4533,1231,1322,4473,4518,1250,1341,4535,1189,1280,4520,1209,1300,4366,4452,1218,1227,1318,4387,4462}      
Analyze.Catalyst[2630][40] = {1232,1323,4012,1251,1342,1233,1324,4540,1252,1343,4525,1267,1358,1197,1217,4809,1198,1235,4786,4890,165,1236,4720,4935,4955}     
Analyze.Catalyst[2630][50] = {4013,1243,4973,1262,1270,1273,1254,1255,1200,1274,1291,1220,1292,1311,4937,1330,4975,1735}     
Analyze.Catalyst[2630][60] = {3062,4014,1203,1222,1241,4792,4793,1238,1329,4735,1256,1257,1348,4992,1349,3064,4994,1177,1260,1359,1296,4759,4941,1354,4999,1180}     
Analyze.Catalyst[2630][70] = {1362,3069,4015,4785,4016}
Analyze.Catalyst[2630][80] = {4017,4018}

-- [2634] Special Catalyst  
Analyze.Catalyst[2634] = {} 
Analyze.Catalyst[2634][0] = {1573,4399,1620,1777,1839,4402,1840,4485,1779,1654,4332,1719,1778,4418,1752,3381,4503,1708} 
Analyze.Catalyst[2634][20] = {1658,1747,3929,4370,4391,4431,4492,1661,1698,4392,4434,1773,4438,4460,1759,4436,1749,1753,1841,3933,4350,4437,4445,1749,4501,3382,4354,4440,1728,4348,4435,4394,4481,4496,4502,3793,4358,4444}    
Analyze.Catalyst[2634][30] = {4478,1739,1757,3790,3794,4498,4504,4515,4531,4372,4424,1657,1754,4932,4360,4361,4447,4510,4952,1653,4448,4499,4505,1655,4362,4450,4471,3438,3439,3935,4367,4454}      
Analyze.Catalyst[2634][40] = {3440,3441,4369,4456,4523,3791,3795,4732,4836,4803,4907,4806,4861,4734,4838,4970,4989,3446,3453,3792,3796,4526,4807,4808,4862,4911,4810,4913,4914,4718,153 ,3370,1210,1301,4719,4823,4946,1211}     
Analyze.Catalyst[2634][50] = {1302,3447,3454,4757,4910,4916,4947,4758,4794,4898,4912,1185,1276,1367,4722,4795,4796,4813,4826,4899,4900,4917,4729,4833,1190,1281,4834,4926,1191,1282,1480,1481,1740,3448,3827,4814,4815,4918,4919,4920,4927,3822,4733,4781,4797,4798,4837,4885,4901,4902,1184,1275,1366,4754,4799,4817,4903,4921,1228,1319,3823,4775,4879,4964}     
Analyze.Catalyst[2634][60] = {1229,1320,1493,1504,1505,1742,1763,3386,4819,4922,4965,3824,1186,1277,1368,1788,4839,1790,4736,4840,1484,1495,1791,3825,3826,4841,1793,4606,4762,4866,4608,4776,4800,4880,4904,4940,4788,4789,4801,4802,4892,4893,4905,4906,4959,1206,1247,1338,4738,4739,4740,4741,4842,4843,4844,4845,4942,4983}     
Analyze.Catalyst[2634][70] = {1248,1339,1486,1487,1488,1489,1497,1498,1499,1500,3085,4984,1207,1298,4743,4744,4777,4846,4847,4848,4881,4943,4960,1208,1299,4745,4778,4849,4882,4944,4978,1187,1244,1278,1369,4725,4748,4749,4829,4852,4853,4980,1225,1226,1316,1317,4747,4851,4865,4961,4962,1245,1246,1336,1337,1490,1501,2489,4750,4763,4854,4867,4981,4982,1181,1479,1660,4764,4770,4868,4874,4765,4766,4772,4869,4870,4876,4997,1178,1491,1502,4871,1174,1265,1356,1702,4787}
Analyze.Catalyst[2634][80] = {1175,1266,1357,2490,3084,4784,4888,179,3065,1797}

-- [2635] Bone Catalyst  
Analyze.Catalyst[2635] = {} 
Analyze.Catalyst[2635][0] = {1583,4319,4507,4400,1610,1623,4327,4328,4408,1843,4410,1614,4330,4409,1584,4414,4331,4412,4487,1611,1720,1617,1696,4335,4336,4420,4337,4339,4423,4490,4388,4497,1709,4427,4342,1618,4344} 
Analyze.Catalyst[2635][20] = {1682,4346,4430,1677,4500,4379,4432,4376,4373,4493,4383,4356,4371,4480,4439,4377,4513,4465}    
Analyze.Catalyst[2635][30] = {1842,4374,4443,1624,1688,4446,4509,4930,4950,3434,4451,3435,4469,3436,3437,1717,4386,4534,4449,4381,4468,4384,4385,3449,4395,4365,4453,1713,4521,4537,4324,3450}      
Analyze.Catalyst[2635][40] = {1612,3451,4968,4539,1715,4353,4987,1616,3452,4908,4524,4909,4717,4821,1613,1176,4542,4822,4746,4824,1199}     
Analyze.Catalyst[2635][50] = {4812,1710,1219,1683,4753,1334,4979,1237,1353,4998,4883,1182,1364,167,1179,1361,4835,1183,1365,4858,4886,1202,1293,4938}     
Analyze.Catalyst[2635][60] = {4818,1693,1312,4957,1684,1221,1240,4976,1331,4897,4828,4783,4887,1485,4737,1259,4995,1774,1350,4860,1204,1223}     
Analyze.Catalyst[2635][70] = {3063,4716,2419,2488,4742,1224,1242,4761,4889,3067,1272,1264,1261,4767,1269,4891}
Analyze.Catalyst[2635][80] = {1716,1492,1625,1711,1758,1626}

-- [2636] Plant Catalyst
Analyze.Catalyst[2636] = {} 
Analyze.Catalyst[2636][0] = {1576,1575,1597,1691,4314,4315,4316,4396,4397,4398,4506,1577,1574,1579,1600,1692,3372,4322,4323,4401,4403,4483,4508,1725,4406,4049,4338,4421,1601,1685,1686,1846,4343,4477,4345,4429} 
Analyze.Catalyst[2636][20] = {4050,4466,4389,4476,1212,1303,4511,4527,4948,1230,1321,4352,4966,1192,1283,4928,1249,1340,4378,4985}    
Analyze.Catalyst[2636][30] = {1593,1578,4060,4804,4915}      
Analyze.Catalyst[2636][40] = {}     
Analyze.Catalyst[2636][50] = {1593,1578,4060,4804,4915,4055,4730}     
Analyze.Catalyst[2636][60] = {1593,1578,4060,4804,4915,4055,4730,4061,4790,4894,4864,4727,4831}     
Analyze.Catalyst[2636][70] = {1593,1578,4060,4804,4915,4055,4730,4061,4790,4894,4864,4727,4831,2225,4057,1768}
Analyze.Catalyst[2636][80] = {}

-- [2637] Fur Catalyst  
Analyze.Catalyst[2637] = {}
Analyze.Catalyst[2637][0] = {4317,1695,4405,4486,4407,1678,1679,4413,4333,4334,4416,3383,1603,1604,1707,1605,4422,4489,4341} 
Analyze.Catalyst[2637][20] = {1845,3384,4347,4479,4470,4458,1608,4512,4929,4949}    
Analyze.Catalyst[2637][30] = {4359,3932,1721,4364,4516,1287,1307,4517,1606,4380,4967,4519,4986,1607,4472,1609,4474,4522}      
Analyze.Catalyst[2637][40] = {1619,4820,4805,1325,1344,2396,4933,4953,4934,4971,176,178,4954,4972,1290}     
Analyze.Catalyst[2637][50] = {1310,1729,1328,4857,3385,160,161,175,177,4779,4780,4884,4990,4991,4459,4731,4936,4956,4782}     
Analyze.Catalyst[2637][60] = {4459,4731,4936,4956,4782,1697,4939,4896,4958,4977,4724,4974,1347,4993,1730,4756,4996}     
Analyze.Catalyst[2637][70] = {4459,4731,4936,4956,4782,1697,4939,4896,4958,4977,4724,4974,1347,4993,1730,4756,4996,1712,1734,1621,1703}
Analyze.Catalyst[2637][80] = {}

-- [2638] Liquid Catalyst  
Analyze.Catalyst[2638] = {} 
Analyze.Catalyst[2638][0] = {1585,4318,1648,1705,1650,1681,1844,4488,1649,4340,4475} 
Analyze.Catalyst[2638][20] = {1628,1651,1627,1629,1726,4441,1838}    
Analyze.Catalyst[2638][30] = {4514,4924,4931,4951,4925,1652,4482,4945,4963}      
Analyze.Catalyst[2638][40] = {4969,4988,1289,1309,1327}     
Analyze.Catalyst[2638][50] = {1346,1294,1313,1332,1351}     
Analyze.Catalyst[2638][60] = {}     
Analyze.Catalyst[2638][70] = {}
Analyze.Catalyst[2638][80] = {}

-- [3905] Dark Wishing Stone
BaoXiang_ADBOX = {}
BaoXiang_ADBOX[1  ] = {Active = 1,	ItemID = 3909,	Quantity = 1,	Quality = 5,	Rad = 600,	GoodItem = 0} -- Gyoza
BaoXiang_ADBOX[2  ] = {Active = 1,	ItemID = 3345,	Quantity = 1,	Quality = 5,	Rad = 300,	GoodItem = 0} -- Firecracker A
BaoXiang_ADBOX[3  ] = {Active = 1,	ItemID = 3346,	Quantity = 1,	Quality = 5,	Rad = 300,	GoodItem = 0} -- Firecracker B
BaoXiang_ADBOX[4  ] = {Active = 1,	ItemID = 3347,	Quantity = 1,	Quality = 5,	Rad = 300,	GoodItem = 0} -- Firecracker C
BaoXiang_ADBOX[5  ] = {Active = 1,	ItemID = 0002,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Long Sword
BaoXiang_ADBOX[6  ] = {Active = 1,	ItemID = 0003,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Fencing Sword
BaoXiang_ADBOX[7  ] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Serpentine Sword
BaoXiang_ADBOX[8  ] = {Active = 1,	ItemID = 0010,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Short Metal Sword
BaoXiang_ADBOX[9  ] = {Active = 1,	ItemID = 0011,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Steel Sword
BaoXiang_ADBOX[10 ] = {Active = 1,	ItemID = 0012,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Striking Sword
BaoXiang_ADBOX[11 ] = {Active = 1,	ItemID = 0013,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Two Handed Sword
BaoXiang_ADBOX[12 ] = {Active = 1,	ItemID = 0014,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Warrior Sword
BaoXiang_ADBOX[13 ] = {Active = 1,	ItemID = 0015,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Criss Sword
BaoXiang_ADBOX[14 ] = {Active = 1,	ItemID = 0025,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Short Bow
BaoXiang_ADBOX[15 ] = {Active = 1,	ItemID = 0026,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Long Bow
BaoXiang_ADBOX[16 ] = {Active = 1,	ItemID = 0027,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Tribal Bow
BaoXiang_ADBOX[17 ] = {Active = 1,	ItemID = 0032,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Hunter Bow
BaoXiang_ADBOX[18 ] = {Active = 1,	ItemID = 0033,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Battle Bow
BaoXiang_ADBOX[19 ] = {Active = 1,	ItemID = 0034,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Marksman Bow
BaoXiang_ADBOX[20 ] = {Active = 1,	ItemID = 0037,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Firegun
BaoXiang_ADBOX[21 ] = {Active = 1,	ItemID = 0038,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Fire-Rifle
BaoXiang_ADBOX[22 ] = {Active = 1,	ItemID = 0039,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_ADBOX[23 ] = {Active = 1,	ItemID = 0074,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Kris
BaoXiang_ADBOX[24 ] = {Active = 1,	ItemID = 0075,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Trident
BaoXiang_ADBOX[25 ] = {Active = 1,	ItemID = 0076,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Moon Kris
BaoXiang_ADBOX[26 ] = {Active = 1,	ItemID = 0081,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Swift Kris
BaoXiang_ADBOX[27 ] = {Active = 1,	ItemID = 0082,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Pointed Kris
BaoXiang_ADBOX[28 ] = {Active = 1,	ItemID = 0098,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Wooden Stick
BaoXiang_ADBOX[29 ] = {Active = 1,	ItemID = 0099,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Magical Staff
BaoXiang_ADBOX[30 ] = {Active = 1,	ItemID = 0100,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Grace Wand
BaoXiang_ADBOX[31 ] = {Active = 1,	ItemID = 0101,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Beastly Wand
BaoXiang_ADBOX[32 ] = {Active = 1,	ItemID = 0104,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Wooden Stave
BaoXiang_ADBOX[33 ] = {Active = 1,	ItemID = 0105,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Hand Stave
BaoXiang_ADBOX[34 ] = {Active = 1,	ItemID = 0106,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Blessing Stave
BaoXiang_ADBOX[35 ] = {Active = 1,	ItemID = 0122,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Steel Shield
BaoXiang_ADBOX[36 ] = {Active = 1,	ItemID = 0123,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Tower Shield
BaoXiang_ADBOX[37 ] = {Active = 1,	ItemID = 0124,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Feather Shield
BaoXiang_ADBOX[38 ] = {Active = 1,	ItemID = 0291,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Thick Armor
BaoXiang_ADBOX[39 ] = {Active = 1,	ItemID = 0293,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Rhino Hide Armor
BaoXiang_ADBOX[40 ] = {Active = 1,	ItemID = 0295,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Breast Plate
BaoXiang_ADBOX[41 ] = {Active = 1,	ItemID = 0297,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Heavy Leather Armor
BaoXiang_ADBOX[42 ] = {Active = 1,	ItemID = 0298,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Strong Leather Armor
BaoXiang_ADBOX[43 ] = {Active = 1,	ItemID = 0300,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Chain Mail
BaoXiang_ADBOX[44 ] = {Active = 1,	ItemID = 0306,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Canvas Vest
BaoXiang_ADBOX[45 ] = {Active = 1,	ItemID = 0307,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Exquisite Vest
BaoXiang_ADBOX[46 ] = {Active = 1,	ItemID = 0311,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Safari Vest
BaoXiang_ADBOX[47 ] = {Active = 1,	ItemID = 0313,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Hunter Vest
BaoXiang_ADBOX[48 ] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Slick Vest
BaoXiang_ADBOX[49 ] = {Active = 1,	ItemID = 0336,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Tough Vest
BaoXiang_ADBOX[50 ] = {Active = 1,	ItemID = 0337,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Explorer Vest
BaoXiang_ADBOX[51 ] = {Active = 1,	ItemID = 0338,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Adventure Vest
BaoXiang_ADBOX[52 ] = {Active = 1,	ItemID = 0339,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Helmsman Vest
BaoXiang_ADBOX[53 ] = {Active = 1,	ItemID = 0340,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Oarsman Vest
BaoXiang_ADBOX[54 ] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Deckman Vest
BaoXiang_ADBOX[55 ] = {Active = 1,	ItemID = 0352,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Playful Racoon Costume
BaoXiang_ADBOX[56 ] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Duckling Costume
BaoXiang_ADBOX[57 ] = {Active = 1,	ItemID = 0354,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Big Crab Costume
BaoXiang_ADBOX[58 ] = {Active = 1,	ItemID = 0350,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Crabby Costume
BaoXiang_ADBOX[59 ] = {Active = 1,	ItemID = 0360,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Meowy Costume
BaoXiang_ADBOX[60 ] = {Active = 1,	ItemID = 0361,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Owl Costume
BaoXiang_ADBOX[61 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_ADBOX[62 ] = {Active = 1,	ItemID = 0467,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Thick Gloves
BaoXiang_ADBOX[63 ] = {Active = 1,	ItemID = 0469,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Rhino Hide Gloves
BaoXiang_ADBOX[64 ] = {Active = 1,	ItemID = 0471,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Gauntlets
BaoXiang_ADBOX[65 ] = {Active = 1,	ItemID = 0473,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Heavy Leather Gloves
BaoXiang_ADBOX[66 ] = {Active = 1,	ItemID = 0474,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Strong Leather Gloves
BaoXiang_ADBOX[67 ] = {Active = 1,	ItemID = 0476,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Chain Gauntlets
BaoXiang_ADBOX[68 ] = {Active = 1,	ItemID = 0482,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Canvas Gloves
BaoXiang_ADBOX[69 ] = {Active = 1,	ItemID = 0483,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Exquisite Gloves
BaoXiang_ADBOX[70 ] = {Active = 1,	ItemID = 0486,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Feather Gloves
BaoXiang_ADBOX[71 ] = {Active = 1,	ItemID = 0487,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Safari Gloves
BaoXiang_ADBOX[72 ] = {Active = 1,	ItemID = 0490,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Slick Gloves
BaoXiang_ADBOX[73 ] = {Active = 1,	ItemID = 0513,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Explorer Gloves
BaoXiang_ADBOX[74 ] = {Active = 1,	ItemID = 0514,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Adventure Gloves
BaoXiang_ADBOX[75 ] = {Active = 1,	ItemID = 0515,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Helmsman Gloves
BaoXiang_ADBOX[76 ] = {Active = 1,	ItemID = 0516,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Oarsman Gloves
BaoXiang_ADBOX[77 ] = {Active = 1,	ItemID = 0517,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Deckman Gloves
BaoXiang_ADBOX[78 ] = {Active = 1,	ItemID = 0536,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Meowy Muffs
BaoXiang_ADBOX[79 ] = {Active = 1,	ItemID = 0537,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Owl Muffs
BaoXiang_ADBOX[80 ] = {Active = 1,	ItemID = 0542,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Foster Gloves
BaoXiang_ADBOX[81 ] = {Active = 1,	ItemID = 0543,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Travel Gloves
BaoXiang_ADBOX[82 ] = {Active = 1,	ItemID = 0544,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Nurse Gloves
BaoXiang_ADBOX[83 ] = {Active = 1,	ItemID = 0546,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Holy Gloves
BaoXiang_ADBOX[84 ] = {Active = 1,	ItemID = 0549,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Scholar Gloves
BaoXiang_ADBOX[85 ] = {Active = 1,	ItemID = 0550,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Emergency Gloves
BaoXiang_ADBOX[86 ] = {Active = 1,	ItemID = 0557,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Kitty Muffs
BaoXiang_ADBOX[87 ] = {Active = 1,	ItemID = 0562,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Racoon Muffs
BaoXiang_ADBOX[88 ] = {Active = 1,	ItemID = 0565,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Night Owl Muffs
BaoXiang_ADBOX[89 ] = {Active = 1,	ItemID = 0566,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Kangaroo Muffs
BaoXiang_ADBOX[90 ] = {Active = 1,	ItemID = 0568,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Bunny Baby Muffs
BaoXiang_ADBOX[91 ] = {Active = 1,	ItemID = 0649,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Heavy Leather Boots
BaoXiang_ADBOX[92 ] = {Active = 1,	ItemID = 0650,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Strong Leather Boots
BaoXiang_ADBOX[93 ] = {Active = 1,	ItemID = 0652,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Chain Greaves
BaoXiang_ADBOX[94 ] = {Active = 1,	ItemID = 0658,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Canvas Boots
BaoXiang_ADBOX[95 ] = {Active = 1,	ItemID = 0659,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Exquisite Boots
BaoXiang_ADBOX[96 ] = {Active = 1,	ItemID = 0662,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Feather Boots
BaoXiang_ADBOX[97 ] = {Active = 1,	ItemID = 0665,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Hunter Boots
BaoXiang_ADBOX[98 ] = {Active = 1,	ItemID = 0666,	Quantity = 1,	Quality = 5,	Rad = 8  ,	GoodItem = 0} -- Slick Boots
BaoXiang_ADBOX[99 ] = {Active = 1,	ItemID = 0689,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Explorer Boots
BaoXiang_ADBOX[100] = {Active = 1,	ItemID = 0690,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Adventure Boots
BaoXiang_ADBOX[101] = {Active = 1,	ItemID = 0691,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Helmsman Boots
BaoXiang_ADBOX[102] = {Active = 1,	ItemID = 0692,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Oarsman Boots
BaoXiang_ADBOX[103] = {Active = 1,	ItemID = 0693,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Deckman Boots
BaoXiang_ADBOX[104] = {Active = 1,	ItemID = 0702,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Crabby Shoes
BaoXiang_ADBOX[105] = {Active = 1,	ItemID = 0704,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Playful Racoon Shoes
BaoXiang_ADBOX[106] = {Active = 1,	ItemID = 0705,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Duckling Shoes
BaoXiang_ADBOX[107] = {Active = 1,	ItemID = 0706,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Big Crab Shoes
BaoXiang_ADBOX[108] = {Active = 1,	ItemID = 0712,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Meowy Shoes
BaoXiang_ADBOX[109] = {Active = 1,	ItemID = 0713,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Owl Shoes
BaoXiang_ADBOX[110] = {Active = 1,	ItemID = 0718,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Foster Boots
BaoXiang_ADBOX[111] = {Active = 1,	ItemID = 0719,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Travel Boots
BaoXiang_ADBOX[112] = {Active = 1,	ItemID = 0720,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Nurse Boots
BaoXiang_ADBOX[113] = {Active = 1,	ItemID = 0722,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Holy Boots
BaoXiang_ADBOX[114] = {Active = 1,	ItemID = 0725,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Scholar Boots
BaoXiang_ADBOX[115] = {Active = 1,	ItemID = 0726,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Emergency Boots
BaoXiang_ADBOX[116] = {Active = 1,	ItemID = 0733,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Kitty Shoes
BaoXiang_ADBOX[117] = {Active = 1,	ItemID = 0738,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Racoon Shoes
BaoXiang_ADBOX[118] = {Active = 1,	ItemID = 0741,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Night Owl Shoes
BaoXiang_ADBOX[119] = {Active = 1,	ItemID = 0742,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Kangaroo Shoes
BaoXiang_ADBOX[120] = {Active = 1,	ItemID = 0744,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Bunny Baby Shoes
BaoXiang_ADBOX[121] = {Active = 1,	ItemID = 0763,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Armor of Revenant
BaoXiang_ADBOX[122] = {Active = 1,	ItemID = 0770,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Sword of Grief
BaoXiang_ADBOX[123] = {Active = 1,	ItemID = 0777,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Robe of Death
BaoXiang_ADBOX[124] = {Active = 1,	ItemID = 0781,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Touch of Death
BaoXiang_ADBOX[125] = {Active = 1,	ItemID = 0785,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Staff of the Avenger
BaoXiang_ADBOX[126] = {Active = 1,	ItemID = 0789,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Robe of the Venom Witch
BaoXiang_ADBOX[127] = {Active = 1,	ItemID = 0799,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Tooth of Specter
BaoXiang_ADBOX[128] = {Active = 1,	ItemID = 0803,	Quantity = 1,	Quality = 5,	Rad = 7  ,	GoodItem = 0} -- Mantle of the Naga
BaoXiang_ADBOX[129] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Mystery Fruit
BaoXiang_ADBOX[130] = {Active = 1,	ItemID = 3139,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Agrypnotic
BaoXiang_ADBOX[131] = {Active = 1,	ItemID = 3136,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Snowy Soft Bud
BaoXiang_ADBOX[132] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Magical Potion
BaoXiang_ADBOX[133] = {Active = 1,	ItemID = 3123,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Red Date Tea
BaoXiang_ADBOX[134] = {Active = 1,	ItemID = 3125,	Quantity = 1,	Quality = 5,	Rad = 737,	GoodItem = 0} -- Stramonium Fruit Juice
BaoXiang_ADBOX[135] = {Active = 1,	ItemID = 3122,	Quantity = 1,	Quality = 5,	Rad = 736,	GoodItem = 0} -- Elven Fruit Juice
BaoXiang_ADBOX[136] = {Active = 1,	ItemID = 3126,	Quantity = 1,	Quality = 5,	Rad = 736,	GoodItem = 0} -- Ice Cream
BaoXiang_ADBOX[137] = {Active = 1,	ItemID = 3127,	Quantity = 1,	Quality = 5,	Rad = 736,	GoodItem = 0} -- Rainbow Fruit Juice
BaoXiang_ADBOX[138] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 736,	GoodItem = 0} -- Mystery Fruit
BaoXiang_ADBOX[139] = {Active = 1,	ItemID = 3135,	Quantity = 1,	Quality = 5,	Rad = 736,	GoodItem = 0} -- Special Ointment

BaoXiang_ADBOX_Rad = 0
for k,v in pairs(BaoXiang_ADBOX) do
	if BaoXiang_ADBOX[k].Active == 1 then
		BaoXiang_ADBOX_Rad = BaoXiang_ADBOX[k].Rad + BaoXiang_ADBOX_Rad
	end
end

-- [3906] Sparkling Wishing Stone
BaoXiang_SGBOX = {}
BaoXiang_SGBOX[1 ] = {Active = 1,	ItemID = 3909,	Quantity = 1,	Quality = 5,	Rad = 60000,	GoodItem = 0} -- Gyoza
BaoXiang_SGBOX[2 ] = {Active = 1,	ItemID = 3345,	Quantity = 1,	Quality = 5,	Rad = 10000,	GoodItem = 0} -- Firecracker A
BaoXiang_SGBOX[3 ] = {Active = 1,	ItemID = 3346,	Quantity = 1,	Quality = 5,	Rad = 10000,	GoodItem = 0} -- Firecracker B
BaoXiang_SGBOX[4 ] = {Active = 1,	ItemID = 3347,	Quantity = 1,	Quality = 5,	Rad = 10000,	GoodItem = 0} -- Firecracker C
BaoXiang_SGBOX[5 ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Wyrm Sword
BaoXiang_SGBOX[6 ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Bone Sword
BaoXiang_SGBOX[7 ] = {Active = 1,	ItemID = 0041,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Laser Gun
BaoXiang_SGBOX[8 ] = {Active = 1,	ItemID = 0078,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_SGBOX[9 ] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Primal Tattoo
BaoXiang_SGBOX[10] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_SGBOX[11] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Raptor Vest
BaoXiang_SGBOX[12] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_SGBOX[13] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Pincer Costume
BaoXiang_SGBOX[14] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_SGBOX[15] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Protector Robe
BaoXiang_SGBOX[16] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_SGBOX[17] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Heavenly Vest
BaoXiang_SGBOX[18] = {Active = 1,	ItemID = 0480,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Ceremonial Gauntlets
BaoXiang_SGBOX[19] = {Active = 1,	ItemID = 0493,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Raptor Gloves
BaoXiang_SGBOX[20] = {Active = 1,	ItemID = 0520,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Whirlpool Gloves
BaoXiang_SGBOX[21] = {Active = 1,	ItemID = 0534,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Pincer Muffs
BaoXiang_SGBOX[22] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Otter Muffs
BaoXiang_SGBOX[23] = {Active = 1,	ItemID = 0553,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Protector Gloves
BaoXiang_SGBOX[24] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Bunny Muffs
BaoXiang_SGBOX[25] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Heavenly Gloves
BaoXiang_SGBOX[26] = {Active = 1,	ItemID = 0656,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Ceremonial Greaves
BaoXiang_SGBOX[27] = {Active = 1,	ItemID = 0669,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Raptor Boots
BaoXiang_SGBOX[28] = {Active = 1,	ItemID = 0696,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Whirlpool Boots
BaoXiang_SGBOX[29] = {Active = 1,	ItemID = 0710,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Pincer Shoes
BaoXiang_SGBOX[30] = {Active = 1,	ItemID = 0716,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Otter Shoes
BaoXiang_SGBOX[31] = {Active = 1,	ItemID = 0729,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Protector Boots
BaoXiang_SGBOX[32] = {Active = 1,	ItemID = 0745,	Quantity = 1,	Quality = 5,	Rad = 333  ,	GoodItem = 0} -- Lucky Bunny Shoes
BaoXiang_SGBOX[33] = {Active = 1,	ItemID = 0746,	Quantity = 1,	Quality = 5,	Rad = 334  ,	GoodItem = 0} -- Heavenly Shoes
BaoXiang_SGBOX[34] = {Active = 1,	ItemID = 3828,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Thundoria Castle
BaoXiang_SGBOX[35] = {Active = 1,	ItemID = 3829,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Thundoria Harbor
BaoXiang_SGBOX[36] = {Active = 1,	ItemID = 3830,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Sacred Snow Mountain
BaoXiang_SGBOX[37] = {Active = 1,	ItemID = 3831,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Andes Forest Haven
BaoXiang_SGBOX[38] = {Active = 1,	ItemID = 3832,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket Oasis Haven
BaoXiang_SGBOX[39] = {Active = 1,	ItemID = 3833,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Icespire Haven
BaoXiang_SGBOX[40] = {Active = 1,	ItemID = 3834,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower
BaoXiang_SGBOX[41] = {Active = 1,	ItemID = 3835,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Barren Cavern
BaoXiang_SGBOX[42] = {Active = 1,	ItemID = 3836,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Abandoned Mine 2
BaoXiang_SGBOX[43] = {Active = 1,	ItemID = 3837,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Silver Mine 2
BaoXiang_SGBOX[44] = {Active = 1,	ItemID = 3838,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Silver Mine 3
BaoXiang_SGBOX[45] = {Active = 1,	ItemID = 3839,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower 2
BaoXiang_SGBOX[46] = {Active = 1,	ItemID = 3840,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower 3
BaoXiang_SGBOX[47] = {Active = 1,	ItemID = 3841,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower 4
BaoXiang_SGBOX[48] = {Active = 1,	ItemID = 3842,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower 5
BaoXiang_SGBOX[49] = {Active = 1,	ItemID = 3843,	Quantity = 1,	Quality = 5,	Rad = 625  ,	GoodItem = 0} -- Ticket to Lone Tower 6
BaoXiang_SGBOX[50] = {Active = 1,	ItemID = 0879,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Furious Gem
BaoXiang_SGBOX[51] = {Active = 1,	ItemID = 0880,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Explosive Gem
BaoXiang_SGBOX[52] = {Active = 1,	ItemID = 0881,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Lustrious Gem
BaoXiang_SGBOX[53] = {Active = 1,	ItemID = 0882,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Glowing Gem
BaoXiang_SGBOX[54] = {Active = 1,	ItemID = 0883,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Shining Gem
BaoXiang_SGBOX[55] = {Active = 1,	ItemID = 0884,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Shadow Gem
BaoXiang_SGBOX[56] = {Active = 1,	ItemID = 0885,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Refining Gem
BaoXiang_SGBOX[57] = {Active = 1,	ItemID = 0887,	Quantity = 1,	Quality = 5,	Rad = 2500 ,	GoodItem = 1} -- Spirit Gem
BaoXiang_SGBOX[58] = {Active = 1,	ItemID = 0007,	Quantity = 1,	Quality = 5,	Rad = 2    ,	GoodItem = 1} -- Sacro Sword
BaoXiang_SGBOX[59] = {Active = 1,	ItemID = 0042,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Venom Gun
BaoXiang_SGBOX[60] = {Active = 1,	ItemID = 0018,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Thunder Blade
BaoXiang_SGBOX[61] = {Active = 1,	ItemID = 1375,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Primal Axe of Rage
BaoXiang_SGBOX[62] = {Active = 1,	ItemID = 1384,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Winds of Death
BaoXiang_SGBOX[63] = {Active = 1,	ItemID = 1394,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Sword of Dawn
BaoXiang_SGBOX[64] = {Active = 1,	ItemID = 1411,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Kiss of the Serpent
BaoXiang_SGBOX[65] = {Active = 1,	ItemID = 1421,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Tooth of Fury
BaoXiang_SGBOX[66] = {Active = 1,	ItemID = 4198,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Soul Spring
BaoXiang_SGBOX[67] = {Active = 1,	ItemID = 4200,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Consonance of Souls
BaoXiang_SGBOX[68] = {Active = 1,	ItemID = 4202,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Eye of Sacred Dragon
BaoXiang_SGBOX[69] = {Active = 1,	ItemID = 4204,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- Gold of Tears
BaoXiang_SGBOX[70] = {Active = 1,	ItemID = 4206,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- God of Rebuke
BaoXiang_SGBOX[71] = {Active = 1,	ItemID = 4208,	Quantity = 1,	Quality = 5,	Rad = 1    ,	GoodItem = 1} -- God of Wrath
BaoXiang_SGBOX[72] = {Active = 1,	ItemID = 3139,	Quantity = 1,	Quality = 5,	Rad = 12712,	GoodItem = 0} -- Agrypnotic
BaoXiang_SGBOX[73] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 12713,	GoodItem = 0} -- Magical Potion
BaoXiang_SGBOX[74] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 12711,	GoodItem = 0} -- Mystery Fruit
BaoXiang_SGBOX[75] = {Active = 1,	ItemID = 3127,	Quantity = 1,	Quality = 5,	Rad = 12711,	GoodItem = 0} -- Rainbow Fruit Juice
BaoXiang_SGBOX[76] = {Active = 1,	ItemID = 3123,	Quantity = 1,	Quality = 5,	Rad = 12711,	GoodItem = 0} -- Red Date Tea
BaoXiang_SGBOX[77] = {Active = 1,	ItemID = 3128,	Quantity = 1,	Quality = 5,	Rad = 12711,	GoodItem = 0} -- Fruity Mix
BaoXiang_SGBOX[78] = {Active = 1,	ItemID = 3125,	Quantity = 1,	Quality = 5,	Rad = 12711,	GoodItem = 0} -- Stramonium Fruit Juice

BaoXiang_SGBOX_Rad = 0
for k,v in pairs(BaoXiang_SGBOX) do
	if BaoXiang_SGBOX[k].Active == 1 then
		BaoXiang_SGBOX_Rad = BaoXiang_SGBOX[k].Rad + BaoXiang_SGBOX_Rad
	end
end

-- [1094] Lucky Gift Parcel
BoxXiang_YiYuanBOX = {}
BoxXiang_YiYuanBOX[1 ] = {Active = 1,	ItemID = 3098,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Constitution Recovery Flask
BoxXiang_YiYuanBOX[2 ] = {Active = 1,	ItemID = 3054,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Pass to Lone Tower
BoxXiang_YiYuanBOX[3 ] = {Active = 1,	ItemID = 3049,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Pass to Thundoria Harbor
BoxXiang_YiYuanBOX[4 ] = {Active = 1,	ItemID = 3048,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Pass to Thundoria Castle
BoxXiang_YiYuanBOX[5 ] = {Active = 1,	ItemID = 3051,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Pass to Andes Forest Haven
BoxXiang_YiYuanBOX[6 ] = {Active = 1,	ItemID = 3076,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Pass to Spring Town
BoxXiang_YiYuanBOX[7 ] = {Active = 1,	ItemID = 3106,	Quantity = 4,	Quality = 5,	Rad = 150 , GoodItem = 0} -- Battleship Armor
BoxXiang_YiYuanBOX[8 ] = {Active = 1,	ItemID = 3050,	Quantity = 4,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Pass to Sacred Snow Mountain
BoxXiang_YiYuanBOX[9 ] = {Active = 1,	ItemID = 0227,	Quantity = 4,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Fairy Ration
BoxXiang_YiYuanBOX[10] = {Active = 1,	ItemID = 3096,	Quantity = 4,	Quality = 5,	Rad = 180 , GoodItem = 1} -- Amplifier of Luck
BoxXiang_YiYuanBOX[11] = {Active = 0,	ItemID = 3300,	Quantity = 4,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Intense Magic
BoxXiang_YiYuanBOX[12] = {Active = 0,	ItemID = 3301,	Quantity = 4,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Crystalline Blessing
BoxXiang_YiYuanBOX[13] = {Active = 1,	ItemID = 3114,	Quantity = 2,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Sea Weed Killer
BoxXiang_YiYuanBOX[14] = {Active = 1,	ItemID = 3094,	Quantity = 2,	Quality = 5,	Rad = 180 , GoodItem = 1} -- Amplifier of Strive
BoxXiang_YiYuanBOX[15] = {Active = 1,	ItemID = 3909,	Quantity = 2,	Quality = 5,	Rad = 180 , GoodItem = 0} -- Gyoza
BoxXiang_YiYuanBOX[16] = {Active = 1,	ItemID = 0222,	Quantity = 2,	Quality = 5,	Rad = 180 , GoodItem = 1} -- Snow Dragon Fruit
BoxXiang_YiYuanBOX[17] = {Active = 1,	ItemID = 0225,	Quantity = 2,	Quality = 5,	Rad = 180 , GoodItem = 1} -- Argent Mango
BoxXiang_YiYuanBOX[18] = {Active = 1,	ItemID = 3336,	Quantity = 1,	Quality = 5,	Rad = 180 , GoodItem = 1} -- Mystic Clover
BoxXiang_YiYuanBOX[19] = {Active = 1,	ItemID = 3885,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- Refining Gem Voucher
BoxXiang_YiYuanBOX[20] = {Active = 1,	ItemID = 0430,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 0} -- Egg of Love
BoxXiang_YiYuanBOX[21] = {Active = 1,	ItemID = 3886,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- Gem Voucher
BoxXiang_YiYuanBOX[22] = {Active = 1,	ItemID = 3113,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- Vial of Spirit Reset
BoxXiang_YiYuanBOX[23] = {Active = 1,	ItemID = 3111,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- Vial of Agility Reset
BoxXiang_YiYuanBOX[24] = {Active = 1,	ItemID = 3093,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- 48 Slot Inventory Enlarger
BoxXiang_YiYuanBOX[25] = {Active = 1,	ItemID = 3090,	Quantity = 1,	Quality = 5,	Rad = 50  , GoodItem = 1} -- 36 Slot Inventory Enlarger
BoxXiang_YiYuanBOX[26] = {Active = 1,	ItemID = 0860,	Quantity = 1,	Quality = 5,	Rad = 10  , GoodItem = 1} -- Gem of the Wind
BoxXiang_YiYuanBOX[27] = {Active = 1,	ItemID = 0861,	Quantity = 1,	Quality = 5,	Rad = 10  , GoodItem = 1} -- Gem of Striking
BoxXiang_YiYuanBOX[28] = {Active = 1,	ItemID = 0862,	Quantity = 1,	Quality = 5,	Rad = 10  , GoodItem = 1} -- Gem of Colossus
BoxXiang_YiYuanBOX[29] = {Active = 1,	ItemID = 0863,	Quantity = 1,	Quality = 5,	Rad = 10  , GoodItem = 1} -- Gem of Rage
BoxXiang_YiYuanBOX[30] = {Active = 1,	ItemID = 0179,	Quantity = 1,	Quality = 5,	Rad = 1   , GoodItem = 0} -- Timeless Machine
BoxXiang_YiYuanBOX[31] = {Active = 1,	ItemID = 3084,	Quantity = 1,	Quality = 5,	Rad = 1   , GoodItem = 0} -- Sigil of Anubis
BoxXiang_YiYuanBOX[32] = {Active = 1,	ItemID = 3085,	Quantity = 1,	Quality = 5,	Rad = 1   , GoodItem = 0} -- Mask of Mummy King

BoxXiang_YiYuanBOX_Rad = 0
for k,v in pairs(BoxXiang_YiYuanBOX) do
	if BoxXiang_YiYuanBOX[k].Active == 1 then
		BoxXiang_YiYuanBOX_Rad = BoxXiang_YiYuanBOX[k].Rad + BoxXiang_YiYuanBOX_Rad
	end
end

-- [1815] Beautiful Chest
BaoXiang_HLBX = {}
BaoXiang_HLBX[1  ] = {Active = 0,	ItemID = 0004,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Serpentine Sword
BaoXiang_HLBX[2  ] = {Active = 0,	ItemID = 0005,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Dazzling Sword
BaoXiang_HLBX[3  ] = {Active = 0,	ItemID = 0006,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Wyrm Sword
BaoXiang_HLBX[4  ] = {Active = 0,	ItemID = 0015,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Criss Sword
BaoXiang_HLBX[5  ] = {Active = 0,	ItemID = 0016,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Paladin Sword
BaoXiang_HLBX[6  ] = {Active = 0,	ItemID = 0017,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Bone Sword
BaoXiang_HLBX[7  ] = {Active = 0,	ItemID = 0039,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_HLBX[8  ] = {Active = 0,	ItemID = 0040,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Exquisite Rifle
BaoXiang_HLBX[9  ] = {Active = 0,	ItemID = 0041,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Laser Gun
BaoXiang_HLBX[10 ] = {Active = 0,	ItemID = 0076,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Moon Kris
BaoXiang_HLBX[11 ] = {Active = 0,	ItemID = 0077,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Hyena Dagger
BaoXiang_HLBX[12 ] = {Active = 0,	ItemID = 0078,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_HLBX[13 ] = {Active = 0,	ItemID = 0100,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Grace Wand
BaoXiang_HLBX[14 ] = {Active = 0,	ItemID = 0103,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Staff of Life
BaoXiang_HLBX[15 ] = {Active = 0,	ItemID = 4303,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Holy Guidance
BaoXiang_HLBX[16 ] = {Active = 0,	ItemID = 0101,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Beastly Wand
BaoXiang_HLBX[17 ] = {Active = 0,	ItemID = 0102,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Thundorian Staff
BaoXiang_HLBX[18 ] = {Active = 0,	ItemID = 4300,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Staff of Amercement
BaoXiang_HLBX[19 ] = {Active = 0,	ItemID = 3122,	Quantity = 20,	Quality = 7,	Rad = 15,	GoodItem = 0} -- Elven Fruit Juice
BaoXiang_HLBX[20 ] = {Active = 0,	ItemID = 3123,	Quantity = 15,	Quality = 7,	Rad = 14,	GoodItem = 0} -- Red Date Tea
BaoXiang_HLBX[21 ] = {Active = 0,	ItemID = 3124,	Quantity = 10,	Quality = 7,	Rad = 13,	GoodItem = 0} -- Mushroom Soup
BaoXiang_HLBX[22 ] = {Active = 0,	ItemID = 3125,	Quantity = 9,	Quality = 7,	Rad = 12,	GoodItem = 0} -- Stramonium Fruit Juice
BaoXiang_HLBX[23 ] = {Active = 0,	ItemID = 3126,	Quantity = 8,	Quality = 7,	Rad = 11,	GoodItem = 0} -- Ice Cream
BaoXiang_HLBX[24 ] = {Active = 0,	ItemID = 3127,	Quantity = 7,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Rainbow Fruit Juice
BaoXiang_HLBX[25 ] = {Active = 0,	ItemID = 3128,	Quantity = 6,	Quality = 7,	Rad = 9	,	GoodItem = 0} -- Fruity Mix
BaoXiang_HLBX[26 ] = {Active = 0,	ItemID = 3133,	Quantity = 10,	Quality = 7,	Rad = 8	,	GoodItem = 0} -- Liquorice Potion
BaoXiang_HLBX[27 ] = {Active = 0,	ItemID = 3134,	Quantity = 9,	Quality = 7,	Rad = 7	,	GoodItem = 0} -- Energetic Tea
BaoXiang_HLBX[28 ] = {Active = 0,	ItemID = 3135,	Quantity = 8,	Quality = 7,	Rad = 6	,	GoodItem = 0} -- Special Ointment
BaoXiang_HLBX[29 ] = {Active = 0,	ItemID = 3136,	Quantity = 7,	Quality = 7,	Rad = 5	,	GoodItem = 0} -- Snowy Soft Bud
BaoXiang_HLBX[30 ] = {Active = 0,	ItemID = 3137,	Quantity = 6,	Quality = 7,	Rad = 4	,	GoodItem = 0} -- Tiamari Fruit
BaoXiang_HLBX[31 ] = {Active = 0,	ItemID = 3138,	Quantity = 5,	Quality = 7,	Rad = 3	,	GoodItem = 0} -- Mystery Fruit
BaoXiang_HLBX[32 ] = {Active = 0,	ItemID = 3139,	Quantity = 4,	Quality = 7,	Rad = 2	,	GoodItem = 0} -- Agrypnotic
BaoXiang_HLBX[33 ] = {Active = 0,	ItemID = 3140,	Quantity = 3,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Magical Potion
BaoXiang_HLBX[34 ] = {Active = 0,	ItemID = 0293,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Rhino Hide Armor
BaoXiang_HLBX[35 ] = {Active = 0,	ItemID = 0295,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Breast Plate
BaoXiang_HLBX[36 ] = {Active = 0,	ItemID = 0299,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Mithril Platemail
BaoXiang_HLBX[37 ] = {Active = 0,	ItemID = 0300,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Chain Mail
BaoXiang_HLBX[38 ] = {Active = 0,	ItemID = 0301,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Strong Platemail
BaoXiang_HLBX[39 ] = {Active = 0,	ItemID = 0302,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Light Platemail
BaoXiang_HLBX[40 ] = {Active = 0,	ItemID = 0307,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Exquisite Vest
BaoXiang_HLBX[41 ] = {Active = 0,	ItemID = 0310,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Feather Vest
BaoXiang_HLBX[42 ] = {Active = 0,	ItemID = 0312,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Emerald Vest
BaoXiang_HLBX[43 ] = {Active = 0,	ItemID = 0314,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Slick Vest
BaoXiang_HLBX[44 ] = {Active = 0,	ItemID = 0315,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Peacock Vest
BaoXiang_HLBX[45 ] = {Active = 0,	ItemID = 0316,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Ringdove Vest
BaoXiang_HLBX[46 ] = {Active = 0,	ItemID = 0339,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Helmsman Vest
BaoXiang_HLBX[47 ] = {Active = 0,	ItemID = 0341,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Deckman Vest
BaoXiang_HLBX[48 ] = {Active = 0,	ItemID = 0342,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Mastman Vest
BaoXiang_HLBX[49 ] = {Active = 0,	ItemID = 0343,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Hurricane Vest
BaoXiang_HLBX[50 ] = {Active = 0,	ItemID = 0344,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_HLBX[51 ] = {Active = 0,	ItemID = 0345,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Wind Vest
BaoXiang_HLBX[52 ] = {Active = 0,	ItemID = 0350,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Crabby Costume
BaoXiang_HLBX[53 ] = {Active = 0,	ItemID = 0353,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Duckling Costume
BaoXiang_HLBX[54 ] = {Active = 0,	ItemID = 0354,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Big Crab Costume
BaoXiang_HLBX[55 ] = {Active = 0,	ItemID = 0355,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lobster Costume
BaoXiang_HLBX[56 ] = {Active = 0,	ItemID = 0356,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Ducky Costume
BaoXiang_HLBX[57 ] = {Active = 0,	ItemID = 0357,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Prawn Costume
BaoXiang_HLBX[58 ] = {Active = 0,	ItemID = 0358,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Pincer Costume
BaoXiang_HLBX[59 ] = {Active = 0,	ItemID = 0361,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Owl Costume
BaoXiang_HLBX[60 ] = {Active = 0,	ItemID = 0362,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_HLBX[61 ] = {Active = 0,	ItemID = 0363,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_HLBX[62 ] = {Active = 0,	ItemID = 0364,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_HLBX[63 ] = {Active = 0,	ItemID = 0367,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Travel Robe
BaoXiang_HLBX[64 ] = {Active = 0,	ItemID = 0368,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Nurse Robe
BaoXiang_HLBX[65 ] = {Active = 0,	ItemID = 0369,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Garcon Robe
BaoXiang_HLBX[66 ] = {Active = 0,	ItemID = 0370,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Holy Robe
BaoXiang_HLBX[67 ] = {Active = 0,	ItemID = 0371,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Follower Robe
BaoXiang_HLBX[68 ] = {Active = 0,	ItemID = 0374,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Emergency Robe
BaoXiang_HLBX[69 ] = {Active = 0,	ItemID = 0375,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Passage Robe
BaoXiang_HLBX[70 ] = {Active = 0,	ItemID = 0376,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Healer Robe
BaoXiang_HLBX[71 ] = {Active = 0,	ItemID = 0377,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Protector Robe
BaoXiang_HLBX[72 ] = {Active = 0,	ItemID = 0378,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Piety Robe
BaoXiang_HLBX[73 ] = {Active = 0,	ItemID = 0379,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Blessed Robe
BaoXiang_HLBX[74 ] = {Active = 0,	ItemID = 0382,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_HLBX[75 ] = {Active = 0,	ItemID = 0820,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 4 Wind Coral
BaoXiang_HLBX[76 ] = {Active = 0,	ItemID = 0821,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 5 Wind Coral
BaoXiang_HLBX[77 ] = {Active = 0,	ItemID = 0870,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 4 Thunder Coral
BaoXiang_HLBX[78 ] = {Active = 0,	ItemID = 0871,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 5 Thunder Coral
BaoXiang_HLBX[79 ] = {Active = 0,	ItemID = 0875,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 4 Fog Coral
BaoXiang_HLBX[80 ] = {Active = 0,	ItemID = 0876,	Quantity = 1,	Quality = 7,	Rad = 1	,	GoodItem = 0} -- Lv 5 Fog Coral
BaoXiang_HLBX[81 ] = {Active = 1,	ItemID = 1787,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Red Dye
BaoXiang_HLBX[82 ] = {Active = 1,	ItemID = 1788,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Orange Dye
BaoXiang_HLBX[83 ] = {Active = 1,	ItemID = 1789,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Yellow Dye
BaoXiang_HLBX[84 ] = {Active = 1,	ItemID = 1790,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Green Dye
BaoXiang_HLBX[85 ] = {Active = 1,	ItemID = 1791,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Cyan Dye
BaoXiang_HLBX[86 ] = {Active = 1,	ItemID = 1792,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Blue Dye
BaoXiang_HLBX[87 ] = {Active = 1,	ItemID = 1793,	Quantity = 2,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Purple Dye
BaoXiang_HLBX[88 ] = {Active = 1,	ItemID = 1797,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Red Colorant
BaoXiang_HLBX[89 ] = {Active = 1,	ItemID = 1798,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Orange Colorant
BaoXiang_HLBX[90 ] = {Active = 1,	ItemID = 1799,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Yellow Colorant
BaoXiang_HLBX[91 ] = {Active = 1,	ItemID = 1800,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Green Colorant
BaoXiang_HLBX[92 ] = {Active = 1,	ItemID = 1801,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Cyan Colorant
BaoXiang_HLBX[93 ] = {Active = 1,	ItemID = 1802,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Blue Colorant
BaoXiang_HLBX[94 ] = {Active = 1,	ItemID = 1803,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Purple Colorant
BaoXiang_HLBX[95 ] = {Active = 1,	ItemID = 1804,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Scissor
BaoXiang_HLBX[96 ] = {Active = 1,	ItemID = 1805,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Comb
BaoXiang_HLBX[97 ] = {Active = 1,	ItemID = 1806,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Hair Gel
BaoXiang_HLBX[98 ] = {Active = 1,	ItemID = 1807,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Hairstyling Voucher
BaoXiang_HLBX[99 ] = {Active = 1,	ItemID = 1808,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Lance Hairstyle Book
BaoXiang_HLBX[100] = {Active = 1,	ItemID = 1809,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Carsise Hairstyle Book
BaoXiang_HLBX[101] = {Active = 1,	ItemID = 1810,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Phyllis Hairstyle Book
BaoXiang_HLBX[102] = {Active = 1,	ItemID = 1811,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Ami Hairstyle Book
BaoXiang_HLBX[103] = {Active = 1,	ItemID = 4606,	Quantity = 2,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Black Dye
BaoXiang_HLBX[104] = {Active = 1,	ItemID = 4607,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Black Colorant
BaoXiang_HLBX[105] = {Active = 1,	ItemID = 4608,	Quantity = 2,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Brown Dye
BaoXiang_HLBX[106] = {Active = 1,	ItemID = 4609,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Brown Colorant
BaoXiang_HLBX[107] = {Active = 1,	ItemID = 4610,	Quantity = 1,	Quality = 7,	Rad = 10,	GoodItem = 0} -- Decolorant
BaoXiang_HLBX[108] = {Active = 1,	ItemID = 0453,	Quantity = 1,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Fusion Scroll
BaoXiang_HLBX[109] = {Active = 1,	ItemID = 0455,	Quantity = 1,	Quality = 7,	Rad = 20,	GoodItem = 0} -- Strengthening Scroll

BaoXiang_HLBX_Rad = 0
for k,v in pairs(BaoXiang_HLBX) do
	if BaoXiang_HLBX[k].Active == 1 then
		BaoXiang_HLBX_Rad = BaoXiang_HLBX[k].Rad + BaoXiang_HLBX_Rad
	end
end

-- [1814] Mystic Chest
BaoXiang_SMBX = {}
BaoXiang_SMBX[1  ] = {Active = 1,	ItemID = 4636,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Crusader Ring
BaoXiang_SMBX[2  ] = {Active = 1,	ItemID = 4637,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Counterattack Ring
BaoXiang_SMBX[3  ] = {Active = 1,	ItemID = 4638,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Guerrilla Warfare Ring
BaoXiang_SMBX[4  ] = {Active = 1,	ItemID = 4639,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Sniper Ring
BaoXiang_SMBX[5  ] = {Active = 1,	ItemID = 4640,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Advancement
BaoXiang_SMBX[6  ] = {Active = 1,	ItemID = 4691,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ashen Gem
BaoXiang_SMBX[7  ] = {Active = 1,	ItemID = 4692,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mark of the Dragon
BaoXiang_SMBX[8  ] = {Active = 1,	ItemID = 4693,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hope of Life
BaoXiang_SMBX[9  ] = {Active = 1,	ItemID = 4694,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Symbolic Necklace
BaoXiang_SMBX[10 ] = {Active = 1,	ItemID = 4695,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Red Nit Gem
BaoXiang_SMBX[11 ] = {Active = 1,	ItemID = 0125,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Crevice Shield
BaoXiang_SMBX[12 ] = {Active = 1,	ItemID = 0301,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Strong Platemail
BaoXiang_SMBX[13 ] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Light Platemail
BaoXiang_SMBX[14 ] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Peacock Vest
BaoXiang_SMBX[15 ] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mastman Vest
BaoXiang_SMBX[16 ] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ducky Costume
BaoXiang_SMBX[17 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_SMBX[18 ] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Passage Robe
BaoXiang_SMBX[19 ] = {Active = 1,	ItemID = 0378,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Piety Robe
BaoXiang_SMBX[20 ] = {Active = 1,	ItemID = 0388,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Happy Bunny Costume
BaoXiang_SMBX[21 ] = {Active = 1,	ItemID = 0477,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Strong Gauntlets
BaoXiang_SMBX[22 ] = {Active = 1,	ItemID = 0478,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Light Gauntlets
BaoXiang_SMBX[23 ] = {Active = 1,	ItemID = 0491,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Peacock Gloves
BaoXiang_SMBX[24 ] = {Active = 1,	ItemID = 0518,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mastman Gloves
BaoXiang_SMBX[25 ] = {Active = 1,	ItemID = 0532,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ducky Muffs
BaoXiang_SMBX[26 ] = {Active = 1,	ItemID = 0538,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hopperoo Muffs
BaoXiang_SMBX[27 ] = {Active = 1,	ItemID = 0551,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Passage Gloves
BaoXiang_SMBX[28 ] = {Active = 1,	ItemID = 0554,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Piety Gloves
BaoXiang_SMBX[29 ] = {Active = 1,	ItemID = 0564,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Happy Bunny Muffs
BaoXiang_SMBX[30 ] = {Active = 1,	ItemID = 0653,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Strong Greaves
BaoXiang_SMBX[31 ] = {Active = 1,	ItemID = 0654,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Light Greaves
BaoXiang_SMBX[32 ] = {Active = 1,	ItemID = 0667,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Peacock Boots
BaoXiang_SMBX[33 ] = {Active = 1,	ItemID = 0694,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mastman Boots
BaoXiang_SMBX[34 ] = {Active = 1,	ItemID = 0708,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ducky Shoes
BaoXiang_SMBX[35 ] = {Active = 1,	ItemID = 0714,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hopperoo Shoes
BaoXiang_SMBX[36 ] = {Active = 1,	ItemID = 0727,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Passage Boots
BaoXiang_SMBX[37 ] = {Active = 1,	ItemID = 0730,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Piety Boots
BaoXiang_SMBX[38 ] = {Active = 1,	ItemID = 0740,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Happy Bunny Shoes
BaoXiang_SMBX[39 ] = {Active = 1,	ItemID = 2193,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ducky Cap
BaoXiang_SMBX[40 ] = {Active = 1,	ItemID = 2199,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hopperoo Cap
BaoXiang_SMBX[41 ] = {Active = 1,	ItemID = 2210,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Happy Bunny Cap
BaoXiang_SMBX[42 ] = {Active = 1,	ItemID = 4301,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Staff of Sagacious
BaoXiang_SMBX[43 ] = {Active = 1,	ItemID = 4641,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Eye of the Tiger
BaoXiang_SMBX[44 ] = {Active = 1,	ItemID = 4642,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of the Yeti
BaoXiang_SMBX[45 ] = {Active = 1,	ItemID = 4643,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of the Hawk
BaoXiang_SMBX[46 ] = {Active = 1,	ItemID = 4644,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Paw of Cheetah
BaoXiang_SMBX[47 ] = {Active = 1,	ItemID = 4645,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Wild Breeze
BaoXiang_SMBX[48 ] = {Active = 1,	ItemID = 4696,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Necklace of Shooting Star
BaoXiang_SMBX[49 ] = {Active = 1,	ItemID = 4697,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Necklace of Speed
BaoXiang_SMBX[50 ] = {Active = 1,	ItemID = 4698,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Storm Necklace
BaoXiang_SMBX[51 ] = {Active = 1,	ItemID = 4699,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Charm of Encounter
BaoXiang_SMBX[52 ] = {Active = 1,	ItemID = 0229,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Savage Bull Tattoo
BaoXiang_SMBX[53 ] = {Active = 1,	ItemID = 0299,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mithril Platemail
BaoXiang_SMBX[54 ] = {Active = 1,	ItemID = 0312,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Emerald Vest
BaoXiang_SMBX[55 ] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Wind Vest
BaoXiang_SMBX[56 ] = {Active = 1,	ItemID = 0355,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lobster Costume
BaoXiang_SMBX[57 ] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Garcon Robe
BaoXiang_SMBX[58 ] = {Active = 1,	ItemID = 0371,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Follower Robe
BaoXiang_SMBX[59 ] = {Active = 1,	ItemID = 0382,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_SMBX[60 ] = {Active = 1,	ItemID = 0385,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Otter Costume
BaoXiang_SMBX[61 ] = {Active = 1,	ItemID = 0475,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mithril Gauntlets
BaoXiang_SMBX[62 ] = {Active = 1,	ItemID = 0488,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Emerald Gloves
BaoXiang_SMBX[63 ] = {Active = 1,	ItemID = 0521,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Wind Gloves
BaoXiang_SMBX[64 ] = {Active = 1,	ItemID = 0531,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lobster Muffs
BaoXiang_SMBX[65 ] = {Active = 1,	ItemID = 0545,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Garcon Gloves
BaoXiang_SMBX[66 ] = {Active = 1,	ItemID = 0547,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Follower Gloves
BaoXiang_SMBX[67 ] = {Active = 1,	ItemID = 0558,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Muffs
BaoXiang_SMBX[68 ] = {Active = 1,	ItemID = 0561,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Otter Muffs
BaoXiang_SMBX[69 ] = {Active = 1,	ItemID = 0639,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lv 5 Strike Coral
BaoXiang_SMBX[70 ] = {Active = 1,	ItemID = 0651,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mithril Greaves
BaoXiang_SMBX[71 ] = {Active = 1,	ItemID = 0664,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Emerald Boots
BaoXiang_SMBX[72 ] = {Active = 1,	ItemID = 0697,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Wind Boots
BaoXiang_SMBX[73 ] = {Active = 1,	ItemID = 0707,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lobster Shoes
BaoXiang_SMBX[74 ] = {Active = 1,	ItemID = 0721,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Garcon Boots
BaoXiang_SMBX[75 ] = {Active = 1,	ItemID = 0723,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Follower Boots
BaoXiang_SMBX[76 ] = {Active = 1,	ItemID = 0734,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Shoes
BaoXiang_SMBX[77 ] = {Active = 1,	ItemID = 0737,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Otter Shoes
BaoXiang_SMBX[78 ] = {Active = 1,	ItemID = 0821,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lv 5 Wind Coral
BaoXiang_SMBX[79 ] = {Active = 1,	ItemID = 0871,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lv 5 Thunder Coral
BaoXiang_SMBX[80 ] = {Active = 1,	ItemID = 0876,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lv 5 Fog Coral
BaoXiang_SMBX[81 ] = {Active = 1,	ItemID = 2192,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lobster Cap
BaoXiang_SMBX[82 ] = {Active = 1,	ItemID = 2204,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Loopy Bunny Cap
BaoXiang_SMBX[83 ] = {Active = 1,	ItemID = 2207,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Otter Cap
BaoXiang_SMBX[84 ] = {Active = 1,	ItemID = 4646,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Pharaoh
BaoXiang_SMBX[85 ] = {Active = 1,	ItemID = 4647,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Resistance
BaoXiang_SMBX[86 ] = {Active = 1,	ItemID = 4648,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Bandit Ring
BaoXiang_SMBX[87 ] = {Active = 1,	ItemID = 4649,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Bewitching Ring
BaoXiang_SMBX[88 ] = {Active = 1,	ItemID = 4650,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Believer's Ring
BaoXiang_SMBX[89 ] = {Active = 1,	ItemID = 4701,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Burning Vitality
BaoXiang_SMBX[90 ] = {Active = 1,	ItemID = 4702,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Warm Wind of Spring
BaoXiang_SMBX[91 ] = {Active = 1,	ItemID = 4703,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Autumn Night Glimmer
BaoXiang_SMBX[92 ] = {Active = 1,	ItemID = 4704,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Wintery Blizzard
BaoXiang_SMBX[93 ] = {Active = 1,	ItemID = 4705,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Force of Four Seasons
BaoXiang_SMBX[94 ] = {Active = 1,	ItemID = 0021,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Charging Sword
BaoXiang_SMBX[95 ] = {Active = 1,	ItemID = 0023,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Delusion Sword
BaoXiang_SMBX[96 ] = {Active = 1,	ItemID = 0045,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Gattling Firegun
BaoXiang_SMBX[97 ] = {Active = 1,	ItemID = 0084,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Vampiric Kris
BaoXiang_SMBX[98 ] = {Active = 1,	ItemID = 0108,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lotus Staff
BaoXiang_SMBX[99 ] = {Active = 1,	ItemID = 0126,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Blessed Shield
BaoXiang_SMBX[100] = {Active = 1,	ItemID = 0228,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Raging Bull Tattoo
BaoXiang_SMBX[101] = {Active = 1,	ItemID = 0303,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Silver Platemail
BaoXiang_SMBX[102] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ringdove Vest
BaoXiang_SMBX[103] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hurricane Vest
BaoXiang_SMBX[104] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Prawn Costume
BaoXiang_SMBX[105] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_SMBX[106] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Healer Robe
BaoXiang_SMBX[107] = {Active = 1,	ItemID = 0379,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Blessed Robe
BaoXiang_SMBX[108] = {Active = 1,	ItemID = 0391,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Costume
BaoXiang_SMBX[109] = {Active = 1,	ItemID = 0479,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Silver Gauntlets
BaoXiang_SMBX[110] = {Active = 1,	ItemID = 0492,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ringdove Gloves
BaoXiang_SMBX[111] = {Active = 1,	ItemID = 0519,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hurricane Gloves
BaoXiang_SMBX[112] = {Active = 1,	ItemID = 0533,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Prawn Muffs
BaoXiang_SMBX[113] = {Active = 1,	ItemID = 0539,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Clever Otter Muffs
BaoXiang_SMBX[114] = {Active = 1,	ItemID = 0552,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Healer Gloves
BaoXiang_SMBX[115] = {Active = 1,	ItemID = 0555,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Blessed Gloves
BaoXiang_SMBX[116] = {Active = 1,	ItemID = 0567,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Muffs
BaoXiang_SMBX[117] = {Active = 1,	ItemID = 0655,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Silver Greaves
BaoXiang_SMBX[118] = {Active = 1,	ItemID = 0668,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ringdove Boots
BaoXiang_SMBX[119] = {Active = 1,	ItemID = 0695,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Hurricane Boots
BaoXiang_SMBX[120] = {Active = 1,	ItemID = 0709,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Prawn Shoes
BaoXiang_SMBX[121] = {Active = 1,	ItemID = 0715,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Clever Otter Shoes
BaoXiang_SMBX[122] = {Active = 1,	ItemID = 0728,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Healer Boots
BaoXiang_SMBX[123] = {Active = 1,	ItemID = 0731,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Blessed Boots
BaoXiang_SMBX[124] = {Active = 1,	ItemID = 0743,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Shoes
BaoXiang_SMBX[125] = {Active = 1,	ItemID = 2194,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Prawn Cap
BaoXiang_SMBX[126] = {Active = 1,	ItemID = 2200,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Clever Otter Cap
BaoXiang_SMBX[127] = {Active = 1,	ItemID = 2213,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Joyful Bunny Cap
BaoXiang_SMBX[128] = {Active = 1,	ItemID = 4302,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Guardian of Nature
BaoXiang_SMBX[129] = {Active = 1,	ItemID = 4651,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Suppression
BaoXiang_SMBX[130] = {Active = 1,	ItemID = 4652,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Trust
BaoXiang_SMBX[131] = {Active = 1,	ItemID = 4653,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Vanishing Ring
BaoXiang_SMBX[132] = {Active = 1,	ItemID = 4654,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ring of Binding
BaoXiang_SMBX[133] = {Active = 1,	ItemID = 4655,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Mermaid Tears
BaoXiang_SMBX[134] = {Active = 1,	ItemID = 4706,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Spirit Spark
BaoXiang_SMBX[135] = {Active = 1,	ItemID = 4707,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Milky Way
BaoXiang_SMBX[136] = {Active = 1,	ItemID = 4708,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Shooting Star
BaoXiang_SMBX[137] = {Active = 1,	ItemID = 4709,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Blessed Rainbow
BaoXiang_SMBX[138] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Primal Tattoo
BaoXiang_SMBX[139] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_SMBX[140] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Raptor Vest
BaoXiang_SMBX[141] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_SMBX[142] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Pincer Costume
BaoXiang_SMBX[143] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_SMBX[144] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Protector Robe
BaoXiang_SMBX[145] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_SMBX[146] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Heavenly Vest
BaoXiang_SMBX[147] = {Active = 1,	ItemID = 0480,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ceremonial Gauntlets
BaoXiang_SMBX[148] = {Active = 1,	ItemID = 0493,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Raptor Gloves
BaoXiang_SMBX[149] = {Active = 1,	ItemID = 0520,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Whirlpool Gloves
BaoXiang_SMBX[150] = {Active = 1,	ItemID = 0534,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Pincer Muffs
BaoXiang_SMBX[151] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Otter Muffs
BaoXiang_SMBX[152] = {Active = 1,	ItemID = 0553,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Protector Gloves
BaoXiang_SMBX[153] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Muffs
BaoXiang_SMBX[154] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Heavenly Gloves
BaoXiang_SMBX[155] = {Active = 1,	ItemID = 0656,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Ceremonial Greaves
BaoXiang_SMBX[156] = {Active = 1,	ItemID = 0669,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Raptor Boots
BaoXiang_SMBX[157] = {Active = 1,	ItemID = 0696,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Whirlpool Boots
BaoXiang_SMBX[158] = {Active = 1,	ItemID = 0710,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Pincer Shoes
BaoXiang_SMBX[159] = {Active = 1,	ItemID = 0716,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Otter Shoes
BaoXiang_SMBX[160] = {Active = 1,	ItemID = 0729,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Protector Boots
BaoXiang_SMBX[161] = {Active = 1,	ItemID = 0745,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Shoes
BaoXiang_SMBX[162] = {Active = 1,	ItemID = 0746,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Heavenly Shoes
BaoXiang_SMBX[163] = {Active = 1,	ItemID = 1477,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Eye of Sorrow
BaoXiang_SMBX[164] = {Active = 1,	ItemID = 2195,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Pincer Cap
BaoXiang_SMBX[165] = {Active = 1,	ItemID = 2201,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Otter Cap
BaoXiang_SMBX[166] = {Active = 1,	ItemID = 2215,	Quantity = 1,	Quality = 7,	Rad = 1,	GoodItem = 0} -- Lucky Bunny Cap

BaoXiang_SMBX_Rad = 0
for k,v in pairs(BaoXiang_SMBX) do
	if BaoXiang_SMBX[k].Active == 1 then
		BaoXiang_SMBX_Rad = BaoXiang_SMBX[k].Rad + BaoXiang_SMBX_Rad
	end
end

-- [1852] Whammy Chest, [2442] Sunken Cupboard, [2443] Sunken Cupboard
BaoXiang_SYBOX = {}
BaoXiang_SYBOX[1 ] = {Active = 0,	ItemID = 0183, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Life
BaoXiang_SYBOX[2 ] = {Active = 0,	ItemID = 0184, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Darkness
BaoXiang_SYBOX[3 ] = {Active = 0,	ItemID = 0185, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Virtue
BaoXiang_SYBOX[4 ] = {Active = 0,	ItemID = 0186, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Kudos
BaoXiang_SYBOX[5 ] = {Active = 0,	ItemID = 0187, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Faith
BaoXiang_SYBOX[6 ] = {Active = 0,	ItemID = 0188, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Valor
BaoXiang_SYBOX[7 ] = {Active = 0,	ItemID = 0189, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Hope
BaoXiang_SYBOX[8 ] = {Active = 0,	ItemID = 0190, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Woe
BaoXiang_SYBOX[9 ] = {Active = 0,	ItemID = 0191, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Love
BaoXiang_SYBOX[10] = {Active = 0,	ItemID = 0199, Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fairy of Heart
BaoXiang_SYBOX[11] = {Active = 1,	ItemID = 3336, Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Mystic Clover
BaoXiang_SYBOX[12] = {Active = 1,	ItemID = 3885, Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Refining Gem Voucher
BaoXiang_SYBOX[13] = {Active = 1,	ItemID = 3339, Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Acceleration Potion
BaoXiang_SYBOX[14] = {Active = 1,	ItemID = 3340, Quantity = 1,	Quality = 5,	Rad = 150,	GoodItem = 0} -- Triangle Sail
BaoXiang_SYBOX[15] = {Active = 1,	ItemID = 3342, Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Lantern

BaoXiang_SYBOX_Rad = 0
for k,v in pairs(BaoXiang_SYBOX) do
	if BaoXiang_SYBOX[k].Active == 1 then
		BaoXiang_SYBOX_Rad = BaoXiang_SYBOX[k].Rad + BaoXiang_SYBOX_Rad
	end
end

-- [1851] Daily Supply (Reward for Daily Quest)
BaoXiang_WZX = {}
BaoXiang_WZX[1  ] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Serpentine Sword
BaoXiang_WZX[2  ] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Dazzling Sword
BaoXiang_WZX[3  ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Wyrm Sword
BaoXiang_WZX[4  ] = {Active = 1,	ItemID = 0015,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Criss Sword
BaoXiang_WZX[5  ] = {Active = 1,	ItemID = 0016,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Paladin Sword
BaoXiang_WZX[6  ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Bone Sword
BaoXiang_WZX[7  ] = {Active = 1,	ItemID = 0039,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_WZX[8  ] = {Active = 1,	ItemID = 0040,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Rifle
BaoXiang_WZX[9  ] = {Active = 1,	ItemID = 0041,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Laser Gun
BaoXiang_WZX[10 ] = {Active = 1,	ItemID = 0076,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Moon Kris
BaoXiang_WZX[11 ] = {Active = 1,	ItemID = 0077,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Hyena Dagger
BaoXiang_WZX[12 ] = {Active = 1,	ItemID = 0078,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_WZX[13 ] = {Active = 1,	ItemID = 0100,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Grace Wand
BaoXiang_WZX[14 ] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Staff of Life
BaoXiang_WZX[15 ] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Holy Guidance
BaoXiang_WZX[16 ] = {Active = 1,	ItemID = 0101,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Beastly Wand
BaoXiang_WZX[17 ] = {Active = 1,	ItemID = 0102,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Thundorian Staff
BaoXiang_WZX[18 ] = {Active = 1,	ItemID = 4300,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Staff of Amercement
BaoXiang_WZX[19 ] = {Active = 1,	ItemID = 3122,	Quantity = 20,	Quality = 98,	Rad = 25 ,	GoodItem = 0} -- Elven Fruit Juice
BaoXiang_WZX[20 ] = {Active = 1,	ItemID = 3123,	Quantity = 15,	Quality = 98,	Rad = 25 ,	GoodItem = 0} -- Red Date Tea
BaoXiang_WZX[21 ] = {Active = 1,	ItemID = 3124,	Quantity = 10,	Quality = 98,	Rad = 25 ,	GoodItem = 0} -- Mushroom Soup
BaoXiang_WZX[22 ] = {Active = 1,	ItemID = 3125,	Quantity = 9,	Quality = 98,	Rad = 25 ,	GoodItem = 0} -- Stramonium Fruit Juice
BaoXiang_WZX[23 ] = {Active = 1,	ItemID = 3126,	Quantity = 8,	Quality = 98,	Rad = 25 ,	GoodItem = 0} -- Ice Cream
BaoXiang_WZX[24 ] = {Active = 1,	ItemID = 3127,	Quantity = 7,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Rainbow Fruit Juice
BaoXiang_WZX[25 ] = {Active = 1,	ItemID = 3128,	Quantity = 6,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Fruity Mix
BaoXiang_WZX[26 ] = {Active = 1,	ItemID = 3133,	Quantity = 10,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Liquorice Potion
BaoXiang_WZX[27 ] = {Active = 1,	ItemID = 3134,	Quantity = 9,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Energetic Tea
BaoXiang_WZX[28 ] = {Active = 1,	ItemID = 3135,	Quantity = 8,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Special Ointment
BaoXiang_WZX[29 ] = {Active = 1,	ItemID = 3136,	Quantity = 7,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Snowy Soft Bud
BaoXiang_WZX[30 ] = {Active = 1,	ItemID = 3137,	Quantity = 6,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Tiamari Fruit
BaoXiang_WZX[31 ] = {Active = 1,	ItemID = 3138,	Quantity = 5,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Mystery Fruit
BaoXiang_WZX[32 ] = {Active = 1,	ItemID = 3139,	Quantity = 4,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Agrypnotic
BaoXiang_WZX[33 ] = {Active = 1,	ItemID = 3140,	Quantity = 3,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Magical Potion
BaoXiang_WZX[34 ] = {Active = 1,	ItemID = 0293,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Rhino Hide Armor
BaoXiang_WZX[35 ] = {Active = 1,	ItemID = 0295,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Breast Plate
BaoXiang_WZX[36 ] = {Active = 1,	ItemID = 0299,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Mithril Platemail
BaoXiang_WZX[37 ] = {Active = 1,	ItemID = 0300,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Chain Mail
BaoXiang_WZX[38 ] = {Active = 1,	ItemID = 0301,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Strong Platemail
BaoXiang_WZX[39 ] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Light Platemail
BaoXiang_WZX[40 ] = {Active = 1,	ItemID = 0307,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Vest
BaoXiang_WZX[41 ] = {Active = 1,	ItemID = 0310,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Feather Vest
BaoXiang_WZX[42 ] = {Active = 1,	ItemID = 0312,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Emerald Vest
BaoXiang_WZX[43 ] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Slick Vest
BaoXiang_WZX[44 ] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Peacock Vest
BaoXiang_WZX[45 ] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_WZX[46 ] = {Active = 1,	ItemID = 0339,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Helmsman Vest
BaoXiang_WZX[47 ] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Deckman Vest
BaoXiang_WZX[48 ] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Mastman Vest
BaoXiang_WZX[49 ] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_WZX[50 ] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_WZX[51 ] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Wind Vest
BaoXiang_WZX[52 ] = {Active = 1,	ItemID = 0350,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Crabby Costume
BaoXiang_WZX[53 ] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Duckling Costume
BaoXiang_WZX[54 ] = {Active = 1,	ItemID = 0354,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Big Crab Costume
BaoXiang_WZX[55 ] = {Active = 1,	ItemID = 0355,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lobster Costume
BaoXiang_WZX[56 ] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ducky Costume
BaoXiang_WZX[57 ] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_WZX[58 ] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_WZX[59 ] = {Active = 1,	ItemID = 0361,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Owl Costume
BaoXiang_WZX[60 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_WZX[61 ] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_WZX[62 ] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_WZX[63 ] = {Active = 1,	ItemID = 0367,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Travel Robe
BaoXiang_WZX[64 ] = {Active = 1,	ItemID = 0368,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Nurse Robe
BaoXiang_WZX[65 ] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Garcon Robe
BaoXiang_WZX[66 ] = {Active = 1,	ItemID = 0370,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Holy Robe
BaoXiang_WZX[67 ] = {Active = 1,	ItemID = 0371,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Follower Robe
BaoXiang_WZX[68 ] = {Active = 1,	ItemID = 0374,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Emergency Robe
BaoXiang_WZX[69 ] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Passage Robe
BaoXiang_WZX[70 ] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Healer Robe
BaoXiang_WZX[71 ] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Protector Robe
BaoXiang_WZX[72 ] = {Active = 1,	ItemID = 0378,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Piety Robe
BaoXiang_WZX[73 ] = {Active = 1,	ItemID = 0379,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Blessed Robe
BaoXiang_WZX[74 ] = {Active = 1,	ItemID = 0382,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_WZX[75 ] = {Active = 1,	ItemID = 0820,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Wind Coral
BaoXiang_WZX[76 ] = {Active = 1,	ItemID = 0821,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Wind Coral
BaoXiang_WZX[77 ] = {Active = 1,	ItemID = 0870,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Thunder Coral
BaoXiang_WZX[78 ] = {Active = 1,	ItemID = 0871,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Thunder Coral
BaoXiang_WZX[79 ] = {Active = 1,	ItemID = 0875,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Fog Coral
BaoXiang_WZX[80 ] = {Active = 1,	ItemID = 0876,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Fog Coral
BaoXiang_WZX[81 ] = {Active = 1,	ItemID = 1787,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Red Dye
BaoXiang_WZX[82 ] = {Active = 1,	ItemID = 1788,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Orange Dye
BaoXiang_WZX[83 ] = {Active = 1,	ItemID = 1789,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Yellow Dye
BaoXiang_WZX[84 ] = {Active = 1,	ItemID = 1790,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Green Dye
BaoXiang_WZX[85 ] = {Active = 1,	ItemID = 1791,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Cyan Dye
BaoXiang_WZX[86 ] = {Active = 1,	ItemID = 1792,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Blue Dye
BaoXiang_WZX[87 ] = {Active = 1,	ItemID = 1793,	Quantity = 2,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Purple Dye
BaoXiang_WZX[88 ] = {Active = 1,	ItemID = 1797,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Red Colorant
BaoXiang_WZX[89 ] = {Active = 1,	ItemID = 1798,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Orange Colorant
BaoXiang_WZX[90 ] = {Active = 1,	ItemID = 1799,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Yellow Colorant
BaoXiang_WZX[91 ] = {Active = 1,	ItemID = 1800,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Green Colorant
BaoXiang_WZX[92 ] = {Active = 1,	ItemID = 1801,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Cyan Colorant
BaoXiang_WZX[93 ] = {Active = 1,	ItemID = 1802,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Blue Colorant
BaoXiang_WZX[94 ] = {Active = 1,	ItemID = 1803,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Purple Colorant
BaoXiang_WZX[95 ] = {Active = 1,	ItemID = 1804,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Scissor
BaoXiang_WZX[96 ] = {Active = 1,	ItemID = 1805,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Comb
BaoXiang_WZX[97 ] = {Active = 1,	ItemID = 1806,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Hair Gel
BaoXiang_WZX[98 ] = {Active = 1,	ItemID = 1807,	Quantity = 1,	Quality = 98,	Rad = 300,	GoodItem = 0} -- Hairstyling Voucher
BaoXiang_WZX[99 ] = {Active = 1,	ItemID = 1808,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Lance Hairstyle Book
BaoXiang_WZX[100] = {Active = 1,	ItemID = 1809,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Carsise Hairstyle Book
BaoXiang_WZX[101] = {Active = 1,	ItemID = 1810,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Phyllis Hairstyle Book
BaoXiang_WZX[102] = {Active = 1,	ItemID = 1811,	Quantity = 1,	Quality = 98,	Rad = 15 ,	GoodItem = 0} -- Ami Hairstyle Book
BaoXiang_WZX[103] = {Active = 1,	ItemID = 4606,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Black Dye
BaoXiang_WZX[104] = {Active = 1,	ItemID = 4607,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Black Colorant
BaoXiang_WZX[105] = {Active = 1,	ItemID = 4608,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Brown Dye
BaoXiang_WZX[106] = {Active = 1,	ItemID = 4609,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Brown Colorant
BaoXiang_WZX[107] = {Active = 1,	ItemID = 4610,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Decolorant
BaoXiang_WZX[108] = {Active = 1,	ItemID = 4636,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Crusader Ring
BaoXiang_WZX[109] = {Active = 1,	ItemID = 4637,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Counterattack Ring
BaoXiang_WZX[110] = {Active = 1,	ItemID = 4638,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Guerrilla Warfare Ring
BaoXiang_WZX[111] = {Active = 1,	ItemID = 4639,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Sniper Ring
BaoXiang_WZX[112] = {Active = 1,	ItemID = 4640,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Advancement
BaoXiang_WZX[113] = {Active = 1,	ItemID = 4691,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ashen Gem
BaoXiang_WZX[114] = {Active = 1,	ItemID = 4692,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Mark of the Dragon
BaoXiang_WZX[115] = {Active = 1,	ItemID = 4693,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Hope of Life
BaoXiang_WZX[116] = {Active = 1,	ItemID = 4694,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Symbolic Necklace
BaoXiang_WZX[117] = {Active = 1,	ItemID = 4695,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Red Nit Gem
BaoXiang_WZX[118] = {Active = 1,	ItemID = 4641,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Eye of the Tiger
BaoXiang_WZX[119] = {Active = 1,	ItemID = 4642,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of the Yeti
BaoXiang_WZX[120] = {Active = 1,	ItemID = 4643,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of the Hawk
BaoXiang_WZX[121] = {Active = 1,	ItemID = 4644,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Paw of Cheetah
BaoXiang_WZX[122] = {Active = 1,	ItemID = 4645,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Wild Breeze
BaoXiang_WZX[123] = {Active = 1,	ItemID = 4696,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Necklace of Shooting Star
BaoXiang_WZX[124] = {Active = 1,	ItemID = 4697,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Necklace of Speed
BaoXiang_WZX[125] = {Active = 1,	ItemID = 4698,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Storm Necklace
BaoXiang_WZX[126] = {Active = 1,	ItemID = 4699,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Charm of Encounter
BaoXiang_WZX[127] = {Active = 1,	ItemID = 4646,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Pharaoh
BaoXiang_WZX[128] = {Active = 1,	ItemID = 4647,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Resistance
BaoXiang_WZX[129] = {Active = 1,	ItemID = 4648,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Bandit Ring
BaoXiang_WZX[130] = {Active = 1,	ItemID = 4649,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Bewitching Ring
BaoXiang_WZX[131] = {Active = 1,	ItemID = 4650,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Believer's Ring
BaoXiang_WZX[132] = {Active = 1,	ItemID = 4701,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Burning Vitality
BaoXiang_WZX[133] = {Active = 1,	ItemID = 4702,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Warm Wind of Spring
BaoXiang_WZX[134] = {Active = 1,	ItemID = 4703,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Autumn Night Glimmer
BaoXiang_WZX[135] = {Active = 1,	ItemID = 4704,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Wintery Blizzard
BaoXiang_WZX[136] = {Active = 1,	ItemID = 4705,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Force of Four Seasons
BaoXiang_WZX[137] = {Active = 1,	ItemID = 4651,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Suppression
BaoXiang_WZX[138] = {Active = 1,	ItemID = 4652,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Trust
BaoXiang_WZX[139] = {Active = 1,	ItemID = 4653,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Vanishing Ring
BaoXiang_WZX[140] = {Active = 1,	ItemID = 4654,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Ring of Binding
BaoXiang_WZX[141] = {Active = 1,	ItemID = 4655,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Mermaid Tears
BaoXiang_WZX[142] = {Active = 1,	ItemID = 4706,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Spirit Spark
BaoXiang_WZX[143] = {Active = 1,	ItemID = 4707,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Milky Way
BaoXiang_WZX[144] = {Active = 1,	ItemID = 4708,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Shooting Star
BaoXiang_WZX[145] = {Active = 1,	ItemID = 4709,	Quantity = 1,	Quality = 98,	Rad = 1	 ,	GoodItem = 0} -- Blessed Rainbow
BaoXiang_WZX[146] = {Active = 1,	ItemID = 4543,	Quantity = 20,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Wood
BaoXiang_WZX[147] = {Active = 1,	ItemID = 4544,	Quantity = 10,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Energy Ore
BaoXiang_WZX[148] = {Active = 1,	ItemID = 4545,	Quantity = 15,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Iron Ore
BaoXiang_WZX[149] = {Active = 1,	ItemID = 4546,	Quantity = 10,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Crystal Ore
BaoXiang_WZX[150] = {Active = 1,	ItemID = 1478,	Quantity = 20,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Sashimi
BaoXiang_WZX[151] = {Active = 1,	ItemID = 0227,	Quantity = 1,	Quality = 98,	Rad = 60 ,	GoodItem = 0} -- Fairy Ration
BaoXiang_WZX[152] = {Active = 1,	ItemID = 0453,	Quantity = 1,	Quality = 98,	Rad = 60 ,	GoodItem = 0} -- Fusion Scroll
BaoXiang_WZX[153] = {Active = 1,	ItemID = 0455,	Quantity = 1,	Quality = 98,	Rad = 60 ,	GoodItem = 0} -- Strengthening Scroll
BaoXiang_WZX[154] = {Active = 1,	ItemID = 0222,	Quantity = 1,	Quality = 98,	Rad = 50 ,	GoodItem = 0} -- Snow Dragon Fruit
BaoXiang_WZX[155] = {Active = 1,	ItemID = 0223,	Quantity = 1,	Quality = 98,	Rad = 50 ,	GoodItem = 0} -- Icespire Plum
BaoXiang_WZX[156] = {Active = 1,	ItemID = 0224,	Quantity = 1,	Quality = 98,	Rad = 50 ,	GoodItem = 0} -- Zephyr Fish Floss
BaoXiang_WZX[157] = {Active = 1,	ItemID = 0225,	Quantity = 1,	Quality = 98,	Rad = 50 ,	GoodItem = 0} -- Argent Mango
BaoXiang_WZX[158] = {Active = 1,	ItemID = 0226,	Quantity = 1,	Quality = 98,	Rad = 50 ,	GoodItem = 0} -- Shaitan Biscuit
BaoXiang_WZX[159] = {Active = 1,	ItemID = 0276,	Quantity = 1,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Great Snow Dragon Fruit
BaoXiang_WZX[160] = {Active = 1,	ItemID = 0277,	Quantity = 1,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Great Icespire Plum
BaoXiang_WZX[161] = {Active = 1,	ItemID = 0278,	Quantity = 1,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Great Zephyr Fish Floss
BaoXiang_WZX[162] = {Active = 1,	ItemID = 0279,	Quantity = 1,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Great Argent Mango
BaoXiang_WZX[163] = {Active = 1,	ItemID = 0280,	Quantity = 1,	Quality = 98,	Rad = 20 ,	GoodItem = 0} -- Great Shaitan Biscuit
BaoXiang_WZX[164] = {Active = 1,	ItemID = 0890,	Quantity = 1,	Quality = 98,	Rad = 30 ,	GoodItem = 0} -- Equipment Stabilizer
BaoXiang_WZX[165] = {Active = 1,	ItemID = 0891,	Quantity = 1,	Quality = 98,	Rad = 30 ,	GoodItem = 0} -- Equipment Catalyst

BaoXiang_WZX_Rad = 0
for k,v in pairs(BaoXiang_WZX) do
	if BaoXiang_WZX[k].Active == 1 then
		BaoXiang_WZX_Rad = BaoXiang_WZX[k].Rad + BaoXiang_WZX_Rad
	end
end

-- [3400] Skeletar Chest of Swordsman
BaoXiang_KLJS = {}
BaoXiang_KLJS[1] = {Active = 1,	ItemID = 1928,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Battle Armor of Crimson Flame
BaoXiang_KLJS[2] = {Active = 1,	ItemID = 1935,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gauntlets of Crimson Flame
BaoXiang_KLJS[3] = {Active = 1,	ItemID = 1939,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greaves of Crimson Flame
BaoXiang_KLJS[4] = {Active = 1,	ItemID = 3798,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Blade of Crimson Flame

-- [3401] Skeletar Chest of Hunter
BaoXiang_KLLR = {}
BaoXiang_KLLR[1] = {Active = 1,	ItemID = 1943,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Vest of Clarion Mist
BaoXiang_KLLR[2] = {Active = 1,	ItemID = 1947,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Clarion Mist
BaoXiang_KLLR[3] = {Active = 1,	ItemID = 1951,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Clarion Mist
BaoXiang_KLLR[4] = {Active = 1,	ItemID = 3805,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Firegun of Clarion Mist

-- [3402] Skeletar Chest of Herbalist
BaoXiang_KLYS = {}
BaoXiang_KLYS[1] = {Active = 1,	ItemID = 1955,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Emerald Vine Robe
BaoXiang_KLYS[2] = {Active = 1,	ItemID = 1962,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Emerald Vine Gloves
BaoXiang_KLYS[3] = {Active = 1,	ItemID = 1969,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Emerald Vine Boots
BaoXiang_KLYS[4] = {Active = 1,	ItemID = 3809,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Emerald Vine Wand

-- [3403] Skeletar Chest of Explorer
BaoXiang_KLMX = {}
BaoXiang_KLMX[1] = {Active = 1,	ItemID = 1976,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Robe of the Mist
BaoXiang_KLMX[2] = {Active = 1,	ItemID = 1980,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of the Mist
BaoXiang_KLMX[3] = {Active = 1,	ItemID = 1984,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of the Mist
BaoXiang_KLMX[4] = {Active = 1,	ItemID = 3816,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Blade of the Mist

-- [3404] Incantation Chest of Crusader
BaoXiang_ZSSJ = {}
BaoXiang_ZSSJ[1] = {Active = 1,	ItemID = 1929,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Battle Armor of the Tempest
BaoXiang_ZSSJ[2] = {Active = 1,	ItemID = 1936,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gauntlets of the Tempest
BaoXiang_ZSSJ[3] = {Active = 1,	ItemID = 1940,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greaves of the Tempest
BaoXiang_ZSSJ[4] = {Active = 1,	ItemID = 3799,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Sword of the Tempest

-- [3405] Incantation Chest of Champion
BaoXiang_ZSJS ={}
BaoXiang_ZSJS[1] = {Active = 1,	ItemID = 3802,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Sword of Glowing Flame
BaoXiang_ZSJS[2] = {Active = 1,	ItemID = 1932,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Battle Armor of Nature
BaoXiang_ZSJS[3] = {Active = 1,	ItemID = 4283,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Cursed Warrior Boots
BaoXiang_ZSJS[4] = {Active = 1,	ItemID = 4286,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Cursed Warrior Gloves

-- [3406] Incantation Chest of Sharpshooter
BaoXiang_ZSJJ = {}
BaoXiang_ZSJJ[1] = {Active = 1,	ItemID = 1944,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Flaming Coat
BaoXiang_ZSJJ[2] = {Active = 1,	ItemID = 1948,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Flaming Gloves
BaoXiang_ZSJJ[3] = {Active = 1,	ItemID = 1952,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Flaming Boots
BaoXiang_ZSJJ[4] = {Active = 1,	ItemID = 3806,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Flaming Pistol

-- [3407] Incantation Chest of Cleric
BaoXiang_ZSSZ = {}
BaoXiang_ZSSZ[1] = {Active = 1,	ItemID = 1959,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Glory Coat
BaoXiang_ZSSZ[2] = {Active = 1,	ItemID = 1966,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Glory Gloves
BaoXiang_ZSSZ[3] = {Active = 1,	ItemID = 1973,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Glory Boots
BaoXiang_ZSSZ[4] = {Active = 1,	ItemID = 3813,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Glory Sigil

-- [3408] Incantation Chest of Seal Master
BaoXiang_ZSFY = {}
BaoXiang_ZSFY[1] = {Active = 1,	ItemID = 3810,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Seal of Frozen Crescent
BaoXiang_ZSFY[2] = {Active = 1,	ItemID = 1956,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of Frozen Crescent
BaoXiang_ZSFY[3] = {Active = 1,	ItemID = 1963,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Frozen Crescent
BaoXiang_ZSFY[4] = {Active = 1,	ItemID = 1970,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Frozen Crescent

-- [3409] Incantation Chest of Voyager
BaoXiang_ZSHH = {}
BaoXiang_ZSHH[1] = {Active = 1,	ItemID = 1977,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of the Tempest
BaoXiang_ZSHH[2] = {Active = 1,	ItemID = 1981,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of the Tempest
BaoXiang_ZSHH[3] = {Active = 1,	ItemID = 1985,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of the Tempest
BaoXiang_ZSHH[4] = {Active = 1,	ItemID = 3817,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Blade of the Tempest

-----------------------
--Lv50 Boss Equipment--
-----------------------
-- [3410] Evanescence Chest of Crusader
BaoXiang_HLSJ = {}
BaoXiang_HLSJ[1] = {Active = 1,	ItemID = 1930,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Battle Armor of Sistine
BaoXiang_HLSJ[2] = {Active = 1,	ItemID = 1937,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gauntlets of Sistine
BaoXiang_HLSJ[3] = {Active = 1,	ItemID = 1941,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greaves of Sistine
BaoXiang_HLSJ[4] = {Active = 1,	ItemID = 3800,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Spike of Sistine

-- [3411] Evanescence Chest of Champion
BaoXiang_HLJS = {}
BaoXiang_HLJS[1] = {Active = 1,	ItemID = 1933,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Tattoo of Gallon
BaoXiang_HLJS[2] = {Active = 1,	ItemID = 3803,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Roar of Gallon
BaoXiang_HLJS[3] = {Active = 1,	ItemID = 4284,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Warrior Boots of Evanescence
BaoXiang_HLJS[4] = {Active = 1,	ItemID = 4287,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Warrior Gloves of Evanescence

-- [3412] Evanescence Chest of Sharpshooter
BaoXiang_HLJJ = {}
BaoXiang_HLJJ[1] = {Active = 1,	ItemID = 1945,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Mantle of Darwin
BaoXiang_HLJJ[2] = {Active = 1,	ItemID = 1949,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Darwin
BaoXiang_HLJJ[3] = {Active = 1,	ItemID = 1953,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Darwin
BaoXiang_HLJJ[4] = {Active = 1,	ItemID = 3807,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Rifle of Darwin

-- [3413] Evanescence Chest of Cleric
BaoXiang_HLSZ = {}
BaoXiang_HLSZ[1] = {Active = 1,	ItemID = 1960,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of Hope
BaoXiang_HLSZ[2] = {Active = 1,	ItemID = 1967,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Hope
BaoXiang_HLSZ[3] = {Active = 1,	ItemID = 1974,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Hope
BaoXiang_HLSZ[4] = {Active = 1,	ItemID = 3814,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Staff of Hope

-- [3414] Evanescence Chest of Seal Master
BaoXiang_HLFY = {}
BaoXiang_HLFY[1] = {Active = 1,	ItemID = 1957,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of Udine
BaoXiang_HLFY[2] = {Active = 1,	ItemID = 1964,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Udine
BaoXiang_HLFY[3] = {Active = 1,	ItemID = 1971,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Udine
BaoXiang_HLFY[4] = {Active = 1,	ItemID = 3811,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Staff of Udine

-- [3415] Evanescence Chest of Voyager
BaoXiang_HLHH = {}
BaoXiang_HLHH[1] = {Active = 1,	ItemID = 1978,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Robe of Forlorn
BaoXiang_HLHH[2] = {Active = 1,	ItemID = 1982,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of Forlorn
BaoXiang_HLHH[3] = {Active = 1,	ItemID = 1986,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of Forlorn
BaoXiang_HLHH[4] = {Active = 1,	ItemID = 3818,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Spike of Forlorn

-- [3416] Enigma Chest of Crusader
BaoXiang_MSJ = {}
BaoXiang_MSJ[1] = {Active = 1,	ItemID = 1931,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Battle Armor of the Kylin
BaoXiang_MSJ[2] = {Active = 1,	ItemID = 1938,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gauntlets of the Kylin
BaoXiang_MSJ[3] = {Active = 1,	ItemID = 1942,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greaves of the Kylin
BaoXiang_MSJ[4] = {Active = 1,	ItemID = 3801,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Sword of the Kylin

-- [3417] Enigma Chest of Champion
BaoXiang_MJS = {}
BaoXiang_MJS[1] = {Active = 1,	ItemID = 1934,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Tattoo of the Tortoise
BaoXiang_MJS[2] = {Active = 1,	ItemID = 3804,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greatsword of the Tortoise
BaoXiang_MJS[3] = {Active = 1,	ItemID = 4285,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Greaves of Enigma
BaoXiang_MJS[4] = {Active = 1,	ItemID = 4288,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gauntlets of Enigma

-- [3418] Enigma Chest of Sharpshooter
BaoXiang_MJJ = {}
BaoXiang_MJJ[1] = {Active = 1,	ItemID = 1946,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of the White Tiger
BaoXiang_MJJ[2] = {Active = 1,	ItemID = 1950,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of the White Tiger
BaoXiang_MJJ[3] = {Active = 1,	ItemID = 1954,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of the White Tiger
BaoXiang_MJJ[4] = {Active = 1,	ItemID = 3808,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Rifle of the White Tiger

-- [3419] Enigma Chest of Cleric
BaoXiang_MSZ = {}
BaoXiang_MSZ[1] = {Active = 1,	ItemID = 1961,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of the Phoenix
BaoXiang_MSZ[2] = {Active = 1,	ItemID = 1968,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of the Phoenix
BaoXiang_MSZ[3] = {Active = 1,	ItemID = 1975,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of the Phoenix
BaoXiang_MSZ[4] = {Active = 1,	ItemID = 3815,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Staff of the Phoenix

-- [3420] Enigma Chest of Seal Master
BaoXiang_MFY = {}
BaoXiang_MFY[1] = {Active = 1,	ItemID = 1958,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Coat of the Peacock
BaoXiang_MFY[2] = {Active = 1,	ItemID = 1965,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gloves of the Peacock
BaoXiang_MFY[3] = {Active = 1,	ItemID = 1972,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Boots of the Peacock
BaoXiang_MFY[4] = {Active = 1,	ItemID = 3812,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Staff of the Peacock

-- [3421] Enigma Chest of Voyager
BaoXiang_MHH = {}
BaoXiang_MHH[1] = {Active = 1,	ItemID = 1979,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} --Coat of the Green Dragon
BaoXiang_MHH[2] = {Active = 1,	ItemID = 1983,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} --Gloves of the Green Dragon
BaoXiang_MHH[3] = {Active = 1,	ItemID = 1987,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} --Boots of the Green Dragon
BaoXiang_MHH[4] = {Active = 1,	ItemID = 3819,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} --Blade of the Green Dragon

-- [3423] Chest of Chest of Forsaken
BaoXiang_ZZBX = {}
BaoXiang_ZZBX[1 ] = {Active = 1,	ItemID = 1884,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Armor of Revenant
BaoXiang_ZZBX[2 ] = {Active = 1,	ItemID = 1891,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Sword of Grief
BaoXiang_ZZBX[3 ] = {Active = 1,	ItemID = 1898,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Robe of Death
BaoXiang_ZZBX[4 ] = {Active = 1,	ItemID = 1902,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Touch of Death
BaoXiang_ZZBX[5 ] = {Active = 1,	ItemID = 1906,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Staff of the Avenger
BaoXiang_ZZBX[6 ] = {Active = 1,	ItemID = 1910,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Robe of the Venom Witch
BaoXiang_ZZBX[7 ] = {Active = 1,	ItemID = 1920,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Tooth of Specter
BaoXiang_ZZBX[8 ] = {Active = 1,	ItemID = 1924,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Undead Sealed Mantle of the Naga

-- [3424] Chest of Dark Swamp
BaoXiang_MFBX = {}
BaoXiang_MFBX[1 ] = {Active = 1,	ItemID = 1885,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Tattoo of the Cursed Warrior
BaoXiang_MFBX[2 ] = {Active = 1,	ItemID = 1888,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Platemail of the Cursed Soul
BaoXiang_MFBX[3 ] = {Active = 1,	ItemID = 1892,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Greatsword of Incantation
BaoXiang_MFBX[4 ] = {Active = 1,	ItemID = 1895,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Blade of Incantation
BaoXiang_MFBX[5 ] = {Active = 1,	ItemID = 1899,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Corset of Incantation
BaoXiang_MFBX[6 ] = {Active = 1,	ItemID = 1903,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Kiss of the Cursed
BaoXiang_MFBX[7 ] = {Active = 1,	ItemID = 1907,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Staff of Incantation
BaoXiang_MFBX[8 ] = {Active = 1,	ItemID = 1911,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Coat of Invocation
BaoXiang_MFBX[9 ] = {Active = 1,	ItemID = 1914,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Staff of Abraxas
BaoXiang_MFBX[10] = {Active = 1,	ItemID = 1917,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Robe of Abraxas
BaoXiang_MFBX[11] = {Active = 1,	ItemID = 1921,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Tooth of the Cursed
BaoXiang_MFBX[12] = {Active = 1,	ItemID = 1925,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Mantle of the Cursed Flame
BaoXiang_MFBX[13] = {Active = 1,	ItemID = 6092,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Earth Sealed Eclipse of Woe

-- [3424] Chest of Demonic World
BaoXiang_MZBX = {}
BaoXiang_MZBX[1 ] = {Active = 1,	ItemID = 1886,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Tattoo of Evanescence
BaoXiang_MZBX[2 ] = {Active = 1,	ItemID = 1889,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Armor of Evanescence
BaoXiang_MZBX[3 ] = {Active = 1,	ItemID = 1893,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Roar of Evanescence
BaoXiang_MZBX[4 ] = {Active = 1,	ItemID = 1896,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Dance of Evanescence
BaoXiang_MZBX[5 ] = {Active = 1,	ItemID = 1900,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Coat of Evanescence
BaoXiang_MZBX[6 ] = {Active = 1,	ItemID = 1904,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Bellow of Evanescence
BaoXiang_MZBX[7 ] = {Active = 1,	ItemID = 1908,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Staff of Evanescence
BaoXiang_MZBX[8 ] = {Active = 1,	ItemID = 1912,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Robe of the Arcane
BaoXiang_MZBX[9 ] = {Active = 1,	ItemID = 1915,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Staff of Mirage
BaoXiang_MZBX[10] = {Active = 1,	ItemID = 1918,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Robe of Malediction
BaoXiang_MZBX[11] = {Active = 1,	ItemID = 1922,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Tooth of Evanescence
BaoXiang_MZBX[12] = {Active = 1,	ItemID = 1926,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Cloak of Evanescence
BaoXiang_MZBX[13] = {Active = 1,	ItemID = 1988,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Gauntlets of Evanescence
BaoXiang_MZBX[14] = {Active = 1,	ItemID = 1989,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Greaves of Evanescence
BaoXiang_MZBX[15] = {Active = 1,	ItemID = 1990,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Gloves of Evanescence
BaoXiang_MZBX[16] = {Active = 1,	ItemID = 1991,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Shoes of Evanescence
BaoXiang_MZBX[17] = {Active = 1,	ItemID = 1992,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Gloves of Malediction
BaoXiang_MZBX[18] = {Active = 1,	ItemID = 1993,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Gloves of the Arcane
BaoXiang_MZBX[19] = {Active = 1,	ItemID = 1994,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Boots of Malediction
BaoXiang_MZBX[20] = {Active = 1,	ItemID = 1995,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Boots of the of the Arcane
BaoXiang_MZBX[21] = {Active = 1,	ItemID = 1996,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Heavy Gloves of Evanescence
BaoXiang_MZBX[22] = {Active = 1,	ItemID = 1997,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Boots of Evanescence
BaoXiang_MZBX[23] = {Active = 1,	ItemID = 6093,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Fire Sealed Piercer of Evanescence

-- [3458] Chest of Enigma
BaoXiang_MZBX2 = {}
BaoXiang_MZBX2[1 ] = {Active = 1,	ItemID = 1887,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Tattoo of Enigma
BaoXiang_MZBX2[2 ] = {Active = 1,	ItemID = 1890,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Armor of Enigma
BaoXiang_MZBX2[3 ] = {Active = 1,	ItemID = 1894,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Judgment of Enigma
BaoXiang_MZBX2[4 ] = {Active = 1,	ItemID = 1897,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Blade of Enigma
BaoXiang_MZBX2[5 ] = {Active = 1,	ItemID = 1901,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Mantle of Enigma
BaoXiang_MZBX2[6 ] = {Active = 1,	ItemID = 1905,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Rifle of Enigma
BaoXiang_MZBX2[7 ] = {Active = 1,	ItemID = 1909,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Staff of Enigma
BaoXiang_MZBX2[8 ] = {Active = 1,	ItemID = 1913,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Robe of Enigma
BaoXiang_MZBX2[9 ] = {Active = 1,	ItemID = 1916,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Staff of the Sphinx
BaoXiang_MZBX2[10] = {Active = 1,	ItemID = 1919,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Robe of the Sphinx
BaoXiang_MZBX2[11] = {Active = 1,	ItemID = 1923,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Kris of the Sphinx
BaoXiang_MZBX2[12] = {Active = 1,	ItemID = 1927,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Mantle of the Sphinx
BaoXiang_MZBX2[13] = {Active = 1,	ItemID = 6094,	Quantity = 1,	Quality = 4,	Rad = 1,	GoodItem = 1} -- Ice Sealed Bow of Enigma

-- [3895] Christmas Sock
BaoXiang_SDWZBOX = {}
BaoXiang_SDWZBOX[1 ] = {Active = 1,	ItemID = 0863,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gem of Rage
BaoXiang_SDWZBOX[2 ] = {Active = 1,	ItemID = 0862,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gem of Colossus
BaoXiang_SDWZBOX[3 ] = {Active = 1,	ItemID = 0861,	Quantity = 1,	Quality = 5,	Rad = 2,	GoodItem = 1} -- Gem of Striking
BaoXiang_SDWZBOX[4 ] = {Active = 1,	ItemID = 0860,	Quantity = 1,	Quality = 5,	Rad = 2,	GoodItem = 1} -- Gem of the Wind
BaoXiang_SDWZBOX[5 ] = {Active = 1,	ItemID = 1012,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Gem of Soul
BaoXiang_SDWZBOX[6 ] = {Active = 1,	ItemID = 0899,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Book of Strength Reset
BaoXiang_SDWZBOX[7 ] = {Active = 1,	ItemID = 0900,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Book of Consitution Reset
BaoXiang_SDWZBOX[8 ] = {Active = 1,	ItemID = 0901,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Book of Agility Reset
BaoXiang_SDWZBOX[9 ] = {Active = 1,	ItemID = 0902,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Book of Accuracy Reset
BaoXiang_SDWZBOX[10] = {Active = 1,	ItemID = 0903,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Book of Spirit Reset
BaoXiang_SDWZBOX[11] = {Active = 1,	ItemID = 0885,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 1} -- Refining Gem
BaoXiang_SDWZBOX[12] = {Active = 1,	ItemID = 0878,	Quantity = 1,	Quality = 5,	Rad = 7,	GoodItem = 1} -- Fiery Gem
BaoXiang_SDWZBOX[13] = {Active = 1,	ItemID = 0879,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Furious Gem
BaoXiang_SDWZBOX[14] = {Active = 1,	ItemID = 0880,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Explosive Gem
BaoXiang_SDWZBOX[15] = {Active = 1,	ItemID = 0881,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Lustrious Gem
BaoXiang_SDWZBOX[16] = {Active = 1,	ItemID = 0882,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Glowing Gem
BaoXiang_SDWZBOX[17] = {Active = 1,	ItemID = 0883,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Shining Gem
BaoXiang_SDWZBOX[18] = {Active = 1,	ItemID = 0884,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Shadow Gem
BaoXiang_SDWZBOX[19] = {Active = 1,	ItemID = 0887,	Quantity = 1,	Quality = 5,	Rad = 8,	GoodItem = 1} -- Spirit Gem

-- [3898] A gift box
BaoXiang_SDLHBOX = {}
BaoXiang_SDLHBOX[1  ] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Serpentine Sword
BaoXiang_SDLHBOX[2  ] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Dazzling Sword
BaoXiang_SDLHBOX[3  ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Wyrm Sword
BaoXiang_SDLHBOX[4  ] = {Active = 1,	ItemID = 0015,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Criss Sword
BaoXiang_SDLHBOX[5  ] = {Active = 1,	ItemID = 0016,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Paladin Sword
BaoXiang_SDLHBOX[6  ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Bone Sword
BaoXiang_SDLHBOX[7  ] = {Active = 1,	ItemID = 0039,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_SDLHBOX[8  ] = {Active = 1,	ItemID = 0040,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Rifle
BaoXiang_SDLHBOX[9  ] = {Active = 1,	ItemID = 0041,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Laser Gun
BaoXiang_SDLHBOX[10 ] = {Active = 1,	ItemID = 0076,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Moon Kris
BaoXiang_SDLHBOX[11 ] = {Active = 1,	ItemID = 0077,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Hyena Dagger
BaoXiang_SDLHBOX[12 ] = {Active = 1,	ItemID = 0078,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_SDLHBOX[13 ] = {Active = 1,	ItemID = 0100,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Grace Wand
BaoXiang_SDLHBOX[14 ] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Staff of Life
BaoXiang_SDLHBOX[15 ] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Holy Guidance
BaoXiang_SDLHBOX[16 ] = {Active = 1,	ItemID = 0101,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Beastly Wand
BaoXiang_SDLHBOX[17 ] = {Active = 1,	ItemID = 0102,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Thundorian Staff
BaoXiang_SDLHBOX[18 ] = {Active = 1,	ItemID = 4300,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Staff of Amercement
BaoXiang_SDLHBOX[19 ] = {Active = 1,	ItemID = 3122,	Quantity = 10,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Elven Fruit Juice
BaoXiang_SDLHBOX[20 ] = {Active = 1,	ItemID = 3123,	Quantity = 8,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Red Date Tea
BaoXiang_SDLHBOX[21 ] = {Active = 1,	ItemID = 3124,	Quantity = 5,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Mushroom Soup
BaoXiang_SDLHBOX[22 ] = {Active = 1,	ItemID = 3125,	Quantity = 4,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Stramonium Fruit Juice
BaoXiang_SDLHBOX[23 ] = {Active = 1,	ItemID = 3126,	Quantity = 4,	Quality = 5,	Rad = 25 ,	GoodItem = 0} -- Ice Cream
BaoXiang_SDLHBOX[24 ] = {Active = 1,	ItemID = 3127,	Quantity = 3,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Rainbow Fruit Juice
BaoXiang_SDLHBOX[25 ] = {Active = 1,	ItemID = 3128,	Quantity = 3,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Fruity Mix
BaoXiang_SDLHBOX[26 ] = {Active = 1,	ItemID = 3133,	Quantity = 5,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Liquorice Potion
BaoXiang_SDLHBOX[27 ] = {Active = 1,	ItemID = 3134,	Quantity = 4,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Energetic Tea
BaoXiang_SDLHBOX[28 ] = {Active = 1,	ItemID = 3135,	Quantity = 4,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Special Ointment
BaoXiang_SDLHBOX[29 ] = {Active = 1,	ItemID = 3136,	Quantity = 3,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Snowy Soft Bud
BaoXiang_SDLHBOX[30 ] = {Active = 1,	ItemID = 3137,	Quantity = 3,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Tiamari Fruit
BaoXiang_SDLHBOX[31 ] = {Active = 1,	ItemID = 3138,	Quantity = 2,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Mystery Fruit
BaoXiang_SDLHBOX[32 ] = {Active = 1,	ItemID = 3139,	Quantity = 2,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Agrypnotic
BaoXiang_SDLHBOX[33 ] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Magical Potion
BaoXiang_SDLHBOX[34 ] = {Active = 1,	ItemID = 0293,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Rhino Hide Armor
BaoXiang_SDLHBOX[35 ] = {Active = 1,	ItemID = 0295,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Breast Plate
BaoXiang_SDLHBOX[36 ] = {Active = 1,	ItemID = 0299,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Mithril Platemail
BaoXiang_SDLHBOX[37 ] = {Active = 1,	ItemID = 0300,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Chain Mail
BaoXiang_SDLHBOX[38 ] = {Active = 1,	ItemID = 0301,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Strong Platemail
BaoXiang_SDLHBOX[39 ] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Light Platemail
BaoXiang_SDLHBOX[40 ] = {Active = 1,	ItemID = 0307,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Exquisite Vest
BaoXiang_SDLHBOX[41 ] = {Active = 1,	ItemID = 0310,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Feather Vest
BaoXiang_SDLHBOX[42 ] = {Active = 1,	ItemID = 0312,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Emerald Vest
BaoXiang_SDLHBOX[43 ] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Slick Vest
BaoXiang_SDLHBOX[44 ] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Peacock Vest
BaoXiang_SDLHBOX[45 ] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_SDLHBOX[46 ] = {Active = 1,	ItemID = 0339,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Helmsman Vest
BaoXiang_SDLHBOX[47 ] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Deckman Vest
BaoXiang_SDLHBOX[48 ] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Mastman Vest
BaoXiang_SDLHBOX[49 ] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_SDLHBOX[50 ] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_SDLHBOX[51 ] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Wind Vest
BaoXiang_SDLHBOX[52 ] = {Active = 1,	ItemID = 0350,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Crabby Costume
BaoXiang_SDLHBOX[53 ] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Duckling Costume
BaoXiang_SDLHBOX[54 ] = {Active = 1,	ItemID = 0354,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Big Crab Costume
BaoXiang_SDLHBOX[55 ] = {Active = 1,	ItemID = 0355,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lobster Costume
BaoXiang_SDLHBOX[56 ] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ducky Costume
BaoXiang_SDLHBOX[57 ] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_SDLHBOX[58 ] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_SDLHBOX[59 ] = {Active = 1,	ItemID = 0361,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Owl Costume
BaoXiang_SDLHBOX[60 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_SDLHBOX[61 ] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_SDLHBOX[62 ] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_SDLHBOX[63 ] = {Active = 1,	ItemID = 0367,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Travel Robe
BaoXiang_SDLHBOX[64 ] = {Active = 1,	ItemID = 0368,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Nurse Robe
BaoXiang_SDLHBOX[65 ] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Garcon Robe
BaoXiang_SDLHBOX[66 ] = {Active = 1,	ItemID = 0370,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Holy Robe
BaoXiang_SDLHBOX[67 ] = {Active = 1,	ItemID = 0371,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Follower Robe
BaoXiang_SDLHBOX[68 ] = {Active = 1,	ItemID = 0374,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Emergency Robe
BaoXiang_SDLHBOX[69 ] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Passage Robe
BaoXiang_SDLHBOX[70 ] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Healer Robe
BaoXiang_SDLHBOX[71 ] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Protector Robe
BaoXiang_SDLHBOX[72 ] = {Active = 1,	ItemID = 0378,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Piety Robe
BaoXiang_SDLHBOX[73 ] = {Active = 1,	ItemID = 0379,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Blessed Robe
BaoXiang_SDLHBOX[74 ] = {Active = 1,	ItemID = 0382,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_SDLHBOX[75 ] = {Active = 1,	ItemID = 0820,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Wind Coral
BaoXiang_SDLHBOX[76 ] = {Active = 1,	ItemID = 0821,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Wind Coral
BaoXiang_SDLHBOX[77 ] = {Active = 1,	ItemID = 0870,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Thunder Coral
BaoXiang_SDLHBOX[78 ] = {Active = 1,	ItemID = 0871,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Thunder Coral
BaoXiang_SDLHBOX[79 ] = {Active = 1,	ItemID = 0875,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 4 Fog Coral
BaoXiang_SDLHBOX[80 ] = {Active = 1,	ItemID = 0876,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Lv 5 Fog Coral
BaoXiang_SDLHBOX[81 ] = {Active = 1,	ItemID = 1787,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Red Dye
BaoXiang_SDLHBOX[82 ] = {Active = 1,	ItemID = 1788,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Orange Dye
BaoXiang_SDLHBOX[83 ] = {Active = 1,	ItemID = 1789,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Yellow Dye
BaoXiang_SDLHBOX[84 ] = {Active = 1,	ItemID = 1790,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Green Dye
BaoXiang_SDLHBOX[85 ] = {Active = 1,	ItemID = 1791,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Cyan Dye
BaoXiang_SDLHBOX[86 ] = {Active = 1,	ItemID = 1792,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Blue Dye
BaoXiang_SDLHBOX[87 ] = {Active = 1,	ItemID = 1793,	Quantity = 2,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Purple Dye
BaoXiang_SDLHBOX[88 ] = {Active = 1,	ItemID = 1797,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Red Colorant
BaoXiang_SDLHBOX[89 ] = {Active = 1,	ItemID = 1798,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Orange Colorant
BaoXiang_SDLHBOX[90 ] = {Active = 1,	ItemID = 1799,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Yellow Colorant
BaoXiang_SDLHBOX[91 ] = {Active = 1,	ItemID = 1800,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Green Colorant
BaoXiang_SDLHBOX[92 ] = {Active = 1,	ItemID = 1801,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Cyan Colorant
BaoXiang_SDLHBOX[93 ] = {Active = 1,	ItemID = 1802,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Blue Colorant
BaoXiang_SDLHBOX[94 ] = {Active = 1,	ItemID = 1803,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Purple Colorant
BaoXiang_SDLHBOX[95 ] = {Active = 1,	ItemID = 1804,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Scissor
BaoXiang_SDLHBOX[96 ] = {Active = 1,	ItemID = 1805,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Comb
BaoXiang_SDLHBOX[97 ] = {Active = 1,	ItemID = 1806,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Hair Gel
BaoXiang_SDLHBOX[98 ] = {Active = 1,	ItemID = 1807,	Quantity = 1,	Quality = 5,	Rad = 300,	GoodItem = 0} -- Hairstyling Voucher
BaoXiang_SDLHBOX[99 ] = {Active = 1,	ItemID = 1808,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Lance Hairstyle Book
BaoXiang_SDLHBOX[100] = {Active = 1,	ItemID = 1809,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Carsise Hairstyle Book
BaoXiang_SDLHBOX[101] = {Active = 1,	ItemID = 1810,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Phyllis Hairstyle Book
BaoXiang_SDLHBOX[102] = {Active = 1,	ItemID = 1811,	Quantity = 1,	Quality = 5,	Rad = 15 ,	GoodItem = 0} -- Ami Hairstyle Book
BaoXiang_SDLHBOX[103] = {Active = 1,	ItemID = 4606,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Black Dye
BaoXiang_SDLHBOX[104] = {Active = 1,	ItemID = 4607,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Black Colorant
BaoXiang_SDLHBOX[105] = {Active = 1,	ItemID = 4608,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Brown Dye
BaoXiang_SDLHBOX[106] = {Active = 1,	ItemID = 4609,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Brown Colorant
BaoXiang_SDLHBOX[107] = {Active = 1,	ItemID = 4610,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Decolorant
BaoXiang_SDLHBOX[108] = {Active = 1,	ItemID = 4636,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Crusader Ring
BaoXiang_SDLHBOX[109] = {Active = 1,	ItemID = 4637,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Counterattack Ring
BaoXiang_SDLHBOX[110] = {Active = 1,	ItemID = 4638,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Guerrilla Warfare Ring
BaoXiang_SDLHBOX[111] = {Active = 1,	ItemID = 4639,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Sniper Ring
BaoXiang_SDLHBOX[112] = {Active = 1,	ItemID = 4640,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Advancement
BaoXiang_SDLHBOX[113] = {Active = 1,	ItemID = 4691,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ashen Gem
BaoXiang_SDLHBOX[114] = {Active = 1,	ItemID = 4692,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Mark of the Dragon
BaoXiang_SDLHBOX[115] = {Active = 1,	ItemID = 4693,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Hope of Life
BaoXiang_SDLHBOX[116] = {Active = 1,	ItemID = 4694,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Symbolic Necklace
BaoXiang_SDLHBOX[117] = {Active = 1,	ItemID = 4695,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Red Nit Gem
BaoXiang_SDLHBOX[118] = {Active = 1,	ItemID = 4641,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Eye of the Tiger
BaoXiang_SDLHBOX[119] = {Active = 1,	ItemID = 4642,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of the Yeti
BaoXiang_SDLHBOX[120] = {Active = 1,	ItemID = 4643,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of the Hawk
BaoXiang_SDLHBOX[121] = {Active = 1,	ItemID = 4644,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Paw of Cheetah
BaoXiang_SDLHBOX[122] = {Active = 1,	ItemID = 4645,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Wild Breeze
BaoXiang_SDLHBOX[123] = {Active = 1,	ItemID = 4696,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Necklace of Shooting Star
BaoXiang_SDLHBOX[124] = {Active = 1,	ItemID = 4697,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Necklace of Speed
BaoXiang_SDLHBOX[125] = {Active = 1,	ItemID = 4698,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Storm Necklace
BaoXiang_SDLHBOX[126] = {Active = 1,	ItemID = 4699,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Charm of Encounter
BaoXiang_SDLHBOX[127] = {Active = 1,	ItemID = 4646,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Pharaoh
BaoXiang_SDLHBOX[128] = {Active = 1,	ItemID = 4647,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Resistance
BaoXiang_SDLHBOX[129] = {Active = 1,	ItemID = 4648,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Bandit Ring
BaoXiang_SDLHBOX[130] = {Active = 1,	ItemID = 4649,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Bewitching Ring
BaoXiang_SDLHBOX[131] = {Active = 1,	ItemID = 4650,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Believer's Ring
BaoXiang_SDLHBOX[132] = {Active = 1,	ItemID = 4701,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Burning Vitality
BaoXiang_SDLHBOX[133] = {Active = 1,	ItemID = 4702,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Warm Wind of Spring
BaoXiang_SDLHBOX[134] = {Active = 1,	ItemID = 4703,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Autumn Night Glimmer
BaoXiang_SDLHBOX[135] = {Active = 1,	ItemID = 4704,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Wintery Blizzard
BaoXiang_SDLHBOX[136] = {Active = 1,	ItemID = 4705,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Force of Four Seasons
BaoXiang_SDLHBOX[137] = {Active = 1,	ItemID = 4651,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Suppression
BaoXiang_SDLHBOX[138] = {Active = 1,	ItemID = 4652,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Trust
BaoXiang_SDLHBOX[139] = {Active = 1,	ItemID = 4653,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Vanishing Ring
BaoXiang_SDLHBOX[140] = {Active = 1,	ItemID = 4654,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Ring of Binding
BaoXiang_SDLHBOX[141] = {Active = 1,	ItemID = 4655,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Mermaid Tears
BaoXiang_SDLHBOX[142] = {Active = 1,	ItemID = 4706,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Spirit Spark
BaoXiang_SDLHBOX[143] = {Active = 1,	ItemID = 4707,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Milky Way
BaoXiang_SDLHBOX[144] = {Active = 1,	ItemID = 4708,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Shooting Star
BaoXiang_SDLHBOX[145] = {Active = 1,	ItemID = 4709,	Quantity = 1,	Quality = 5,	Rad = 1	 ,	GoodItem = 0} -- Blessed Rainbow
BaoXiang_SDLHBOX[146] = {Active = 1,	ItemID = 4543,	Quantity = 10,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Wood
BaoXiang_SDLHBOX[147] = {Active = 1,	ItemID = 4544,	Quantity = 5,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Energy Ore
BaoXiang_SDLHBOX[148] = {Active = 1,	ItemID = 4545,	Quantity = 8,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Iron Ore
BaoXiang_SDLHBOX[149] = {Active = 1,	ItemID = 4546,	Quantity = 5,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Crystal Ore
BaoXiang_SDLHBOX[150] = {Active = 1,	ItemID = 1478,	Quantity = 10,	Quality = 5,	Rad = 20 ,	GoodItem = 0} -- Sashimi

-- [3901] Lucky Packet
BaoXiang_HYBOX = {}
BaoXiang_HYBOX[1 ] = {Active = 0,	ItemID = 3850,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card A
BaoXiang_HYBOX[2 ] = {Active = 1,	ItemID = 3851,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card B
BaoXiang_HYBOX[3 ] = {Active = 1,	ItemID = 3852,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card C
BaoXiang_HYBOX[4 ] = {Active = 1,	ItemID = 3853,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card D
BaoXiang_HYBOX[5 ] = {Active = 0,	ItemID = 3854,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card E
BaoXiang_HYBOX[6 ] = {Active = 1,	ItemID = 3855,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card F
BaoXiang_HYBOX[7 ] = {Active = 1,	ItemID = 3856,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card G
BaoXiang_HYBOX[8 ] = {Active = 1,	ItemID = 3857,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card H
BaoXiang_HYBOX[9 ] = {Active = 0,	ItemID = 3858,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card I
BaoXiang_HYBOX[10] = {Active = 1,	ItemID = 3859,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card J
BaoXiang_HYBOX[11] = {Active = 1,	ItemID = 3860,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card K
BaoXiang_HYBOX[12] = {Active = 1,	ItemID = 3861,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card L
BaoXiang_HYBOX[13] = {Active = 1,	ItemID = 3862,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card M
BaoXiang_HYBOX[14] = {Active = 1,	ItemID = 3863,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card N
BaoXiang_HYBOX[15] = {Active = 0,	ItemID = 3864,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card O
BaoXiang_HYBOX[16] = {Active = 1,	ItemID = 3865,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card P
BaoXiang_HYBOX[17] = {Active = 1,	ItemID = 3866,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card Q
BaoXiang_HYBOX[18] = {Active = 1,	ItemID = 3867,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card R
BaoXiang_HYBOX[19] = {Active = 1,	ItemID = 3868,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card S
BaoXiang_HYBOX[20] = {Active = 1,	ItemID = 3869,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card T
BaoXiang_HYBOX[21] = {Active = 0,	ItemID = 3870,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card U
BaoXiang_HYBOX[22] = {Active = 1,	ItemID = 3871,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card V
BaoXiang_HYBOX[23] = {Active = 1,	ItemID = 3872,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card W
BaoXiang_HYBOX[24] = {Active = 1,	ItemID = 3873,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card X
BaoXiang_HYBOX[25] = {Active = 1,	ItemID = 3874,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card Y
BaoXiang_HYBOX[26] = {Active = 1,	ItemID = 3875,	Quantity = 1,	Quality = 5,	Rad = 1,	GoodItem = 1} -- Card Z

-- [3902] Fortune Packet
BaoXiang_FGBOX = {}
BaoXiang_FGBOX[1 ] = {Active = 1,	ItemID = 3828,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Thundoria Castle
BaoXiang_FGBOX[2 ] = {Active = 1,	ItemID = 3829,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Thundoria Harbor
BaoXiang_FGBOX[3 ] = {Active = 1,	ItemID = 3830,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Sacred Snow Mountain
BaoXiang_FGBOX[4 ] = {Active = 1,	ItemID = 3831,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Andes Forest Haven
BaoXiang_FGBOX[5 ] = {Active = 1,	ItemID = 3832,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket Oasis Haven
BaoXiang_FGBOX[6 ] = {Active = 1,	ItemID = 3833,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Icespire Haven
BaoXiang_FGBOX[7 ] = {Active = 1,	ItemID = 3834,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Lone Tower
BaoXiang_FGBOX[8 ] = {Active = 1,	ItemID = 3835,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Barren Cavern
BaoXiang_FGBOX[9 ] = {Active = 1,	ItemID = 3836,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Abandoned Mine 2
BaoXiang_FGBOX[10] = {Active = 1,	ItemID = 3837,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Silver Mine 2
BaoXiang_FGBOX[11] = {Active = 1,	ItemID = 3838,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Silver Mine 3
BaoXiang_FGBOX[12] = {Active = 1,	ItemID = 3839,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Lone Tower 2
BaoXiang_FGBOX[13] = {Active = 1,	ItemID = 3840,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Lone Tower 3
BaoXiang_FGBOX[14] = {Active = 1,	ItemID = 3841,	Quantity = 1,	Quality = 5,	Rad = 32 ,	GoodItem = 0} -- Ticket to Lone Tower 4
BaoXiang_FGBOX[15] = {Active = 1,	ItemID = 3842,	Quantity = 1,	Quality = 5,	Rad = 36 ,	GoodItem = 0} -- Ticket to Lone Tower 5
BaoXiang_FGBOX[16] = {Active = 1,	ItemID = 3843,	Quantity = 1,	Quality = 5,	Rad = 36 ,	GoodItem = 0} -- Ticket to Lone Tower 6
BaoXiang_FGBOX[17] = {Active = 1,	ItemID = 1787,	Quantity = 1,	Quality = 5,	Rad = 42 ,	GoodItem = 0} -- Red Dye
BaoXiang_FGBOX[18] = {Active = 1,	ItemID = 1788,	Quantity = 1,	Quality = 5,	Rad = 44 ,	GoodItem = 0} -- Orange Dye
BaoXiang_FGBOX[19] = {Active = 1,	ItemID = 1789,	Quantity = 1,	Quality = 5,	Rad = 42 ,	GoodItem = 0} -- Yellow Dye
BaoXiang_FGBOX[20] = {Active = 1,	ItemID = 1790,	Quantity = 1,	Quality = 5,	Rad = 42 ,	GoodItem = 0} -- Green Dye
BaoXiang_FGBOX[21] = {Active = 1,	ItemID = 1791,	Quantity = 1,	Quality = 5,	Rad = 44 ,	GoodItem = 0} -- Cyan Dye
BaoXiang_FGBOX[22] = {Active = 1,	ItemID = 1792,	Quantity = 1,	Quality = 5,	Rad = 42 ,	GoodItem = 0} -- Blue Dye
BaoXiang_FGBOX[23] = {Active = 1,	ItemID = 1793,	Quantity = 1,	Quality = 5,	Rad = 44 ,	GoodItem = 0} -- Purple Dye
BaoXiang_FGBOX[24] = {Active = 1,	ItemID = 0893,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 0} -- Potion of Monkey
BaoXiang_FGBOX[25] = {Active = 1,	ItemID = 0894,	Quantity = 1,	Quality = 5,	Rad = 4  ,	GoodItem = 0} -- Potion of Eagle
BaoXiang_FGBOX[26] = {Active = 1,	ItemID = 0895,	Quantity = 1,	Quality = 5,	Rad = 4  ,	GoodItem = 0} -- Potion of Bull
BaoXiang_FGBOX[27] = {Active = 1,	ItemID = 0896,	Quantity = 1,	Quality = 5,	Rad = 4  ,	GoodItem = 0} -- Potion of Soul
BaoXiang_FGBOX[28] = {Active = 1,	ItemID = 0897,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 0} -- Potion of Lion
BaoXiang_FGBOX[29] = {Active = 1,	ItemID = 0898,	Quantity = 1,	Quality = 5,	Rad = 4  ,	GoodItem = 0} -- Mystical Fruit
BaoXiang_FGBOX[30] = {Active = 1,	ItemID = 0878,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Fiery Gem
BaoXiang_FGBOX[31] = {Active = 1,	ItemID = 3844,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 0} -- Heaven's Berry
BaoXiang_FGBOX[32] = {Active = 1,	ItemID = 3845,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 0} -- Charmed Berry
BaoXiang_FGBOX[33] = {Active = 1,	ItemID = 3131,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Strange Fruit
BaoXiang_FGBOX[34] = {Active = 1,	ItemID = 3132,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Snowy Grass Bud
BaoXiang_FGBOX[35] = {Active = 1,	ItemID = 3133,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Liquorice Potion
BaoXiang_FGBOX[36] = {Active = 1,	ItemID = 3121,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Rainbow Fruit
BaoXiang_FGBOX[37] = {Active = 1,	ItemID = 3130,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Fancy Petal
BaoXiang_FGBOX[38] = {Active = 1,	ItemID = 3119,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Stramonium Fruit
BaoXiang_FGBOX[39] = {Active = 1,	ItemID = 3122,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Elven Fruit Juice
BaoXiang_FGBOX[40] = {Active = 1,	ItemID = 3141,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Old Ticket
BaoXiang_FGBOX[41] = {Active = 1,	ItemID = 3129,	Quantity = 1,	Quality = 5,	Rad = 174,	GoodItem = 0} -- Medicated Grass

--3903	Prosperous Packet
BaoXiang_HYUNBOX = {}
BaoXiang_HYUNBOX[1 ] = {Active = 0,	ItemID = 3850,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card A
BaoXiang_HYUNBOX[2 ] = {Active = 1,	ItemID = 3851,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card B
BaoXiang_HYUNBOX[3 ] = {Active = 1,	ItemID = 3852,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card C
BaoXiang_HYUNBOX[4 ] = {Active = 1,	ItemID = 3853,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card D
BaoXiang_HYUNBOX[5 ] = {Active = 0,	ItemID = 3854,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card E
BaoXiang_HYUNBOX[6 ] = {Active = 1,	ItemID = 3855,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card F
BaoXiang_HYUNBOX[7 ] = {Active = 1,	ItemID = 3856,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card G
BaoXiang_HYUNBOX[8 ] = {Active = 1,	ItemID = 3857,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card H
BaoXiang_HYUNBOX[9 ] = {Active = 0,	ItemID = 3858,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card I
BaoXiang_HYUNBOX[10] = {Active = 1,	ItemID = 3859,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card J
BaoXiang_HYUNBOX[11] = {Active = 1,	ItemID = 3860,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card K
BaoXiang_HYUNBOX[12] = {Active = 1,	ItemID = 3861,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card L
BaoXiang_HYUNBOX[13] = {Active = 1,	ItemID = 3862,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card M
BaoXiang_HYUNBOX[14] = {Active = 1,	ItemID = 3863,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card N
BaoXiang_HYUNBOX[15] = {Active = 0,	ItemID = 3864,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card O
BaoXiang_HYUNBOX[16] = {Active = 1,	ItemID = 3865,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card P
BaoXiang_HYUNBOX[17] = {Active = 1,	ItemID = 3866,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card Q
BaoXiang_HYUNBOX[18] = {Active = 1,	ItemID = 3867,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card R
BaoXiang_HYUNBOX[19] = {Active = 1,	ItemID = 3868,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card S
BaoXiang_HYUNBOX[20] = {Active = 1,	ItemID = 3869,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card T
BaoXiang_HYUNBOX[21] = {Active = 0,	ItemID = 3870,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card U
BaoXiang_HYUNBOX[22] = {Active = 1,	ItemID = 3871,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card V
BaoXiang_HYUNBOX[23] = {Active = 1,	ItemID = 3872,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card W
BaoXiang_HYUNBOX[24] = {Active = 1,	ItemID = 3873,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card X
BaoXiang_HYUNBOX[25] = {Active = 1,	ItemID = 3874,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card Y
BaoXiang_HYUNBOX[26] = {Active = 1,	ItemID = 3875,	Quantity = 1,	Quality = 5,	Rad = 200,	GoodItem = 0} -- Card Z
BaoXiang_HYUNBOX[27] = {Active = 1,	ItemID = 3828,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Thundoria Castle
BaoXiang_HYUNBOX[28] = {Active = 1,	ItemID = 3829,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Thundoria Harbor
BaoXiang_HYUNBOX[29] = {Active = 1,	ItemID = 3830,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Sacred Snow Mountain
BaoXiang_HYUNBOX[30] = {Active = 1,	ItemID = 3831,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Andes Forest Haven
BaoXiang_HYUNBOX[31] = {Active = 1,	ItemID = 3832,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket Oasis Haven
BaoXiang_HYUNBOX[32] = {Active = 1,	ItemID = 3833,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Icespire Haven
BaoXiang_HYUNBOX[33] = {Active = 1,	ItemID = 3834,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower
BaoXiang_HYUNBOX[34] = {Active = 1,	ItemID = 3835,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Barren Cavern
BaoXiang_HYUNBOX[35] = {Active = 1,	ItemID = 3836,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Abandoned Mine 2
BaoXiang_HYUNBOX[36] = {Active = 1,	ItemID = 3837,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Silver Mine 2
BaoXiang_HYUNBOX[37] = {Active = 1,	ItemID = 3838,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Silver Mine 3
BaoXiang_HYUNBOX[38] = {Active = 1,	ItemID = 3839,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower 2
BaoXiang_HYUNBOX[39] = {Active = 1,	ItemID = 3840,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower 3
BaoXiang_HYUNBOX[40] = {Active = 1,	ItemID = 3841,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower 4
BaoXiang_HYUNBOX[41] = {Active = 1,	ItemID = 3842,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower 5
BaoXiang_HYUNBOX[42] = {Active = 1,	ItemID = 3843,	Quantity = 1,	Quality = 5,	Rad = 175 ,	GoodItem = 0} -- Ticket to Lone Tower 6
BaoXiang_HYUNBOX[43] = {Active = 1,	ItemID = 3844,	Quantity = 1,	Quality = 5,	Rad = 1000,	GoodItem = 1} -- Heaven's Berry
BaoXiang_HYUNBOX[44] = {Active = 1,	ItemID = 3845,	Quantity = 1,	Quality = 5,	Rad = 1000,	GoodItem = 1} -- Charmed Berry
BaoXiang_HYUNBOX[45] = {Active = 1,	ItemID = 0899,	Quantity = 1,	Quality = 5,	Rad = 6   ,	GoodItem = 1} -- Book of Strength Reset
BaoXiang_HYUNBOX[46] = {Active = 1,	ItemID = 0900,	Quantity = 1,	Quality = 5,	Rad = 6   ,	GoodItem = 1} -- Book of Consitution Reset
BaoXiang_HYUNBOX[47] = {Active = 1,	ItemID = 0901,	Quantity = 1,	Quality = 5,	Rad = 6   ,	GoodItem = 1} -- Book of Agility Reset
BaoXiang_HYUNBOX[48] = {Active = 1,	ItemID = 0902,	Quantity = 1,	Quality = 5,	Rad = 6   ,	GoodItem = 1} -- Book of Accuracy Reset
BaoXiang_HYUNBOX[49] = {Active = 1,	ItemID = 0903,	Quantity = 1,	Quality = 5,	Rad = 6   ,	GoodItem = 1} -- Book of Spirit Reset
BaoXiang_HYUNBOX[50] = {Active = 1,	ItemID = 3131,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Strange Fruit
BaoXiang_HYUNBOX[51] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Magical Potion
BaoXiang_HYUNBOX[52] = {Active = 1,	ItemID = 3133,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Liquorice Potion
BaoXiang_HYUNBOX[53] = {Active = 1,	ItemID = 3139,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Agrypnotic
BaoXiang_HYUNBOX[54] = {Active = 1,	ItemID = 3135,	Quantity = 1,	Quality = 5,	Rad = 23  ,	GoodItem = 0} -- Special Ointment
BaoXiang_HYUNBOX[55] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Mystery Fruit
BaoXiang_HYUNBOX[56] = {Active = 1,	ItemID = 3136,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Snowy Soft Bud
BaoXiang_HYUNBOX[57] = {Active = 1,	ItemID = 3137,	Quantity = 1,	Quality = 5,	Rad = 21  ,	GoodItem = 0} -- Tiamari Fruit

-- [262] Fairy Box
BaoXiang_JingLingBOX = {}
BaoXiang_JingLingBOX[1 ] = {Active = 1,	ItemID = 0183,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Life
BaoXiang_JingLingBOX[2 ] = {Active = 1,	ItemID = 0189,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Hope
BaoXiang_JingLingBOX[3 ] = {Active = 1,	ItemID = 0185,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Virtue
BaoXiang_JingLingBOX[4 ] = {Active = 1,	ItemID = 0186,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Kudos
BaoXiang_JingLingBOX[5 ] = {Active = 1,	ItemID = 0187,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Faith
BaoXiang_JingLingBOX[6 ] = {Active = 1,	ItemID = 0188,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Valor
BaoXiang_JingLingBOX[7 ] = {Active = 1,	ItemID = 0184,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Darkness
BaoXiang_JingLingBOX[8 ] = {Active = 1,	ItemID = 0190,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Woe
BaoXiang_JingLingBOX[9 ] = {Active = 1,	ItemID = 0191,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Love
BaoXiang_JingLingBOX[10] = {Active = 1,	ItemID = 0199,	Quantity = 1,	Quality = 4,	Rad = 100,	GoodItem = 1} -- Fairy of Heart
BaoXiang_JingLingBOX[11] = {Active = 1,	ItemID = 0680,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 1} -- Mordo

-- [1095] 99 Parcel
BoxXiang_BaoZhaBOX = {}
BoxXiang_BaoZhaBOX[1 ] = {Active = 1,	ItemID = 3077,	Quantity = 10,	Quality = 5,	Rad = 566,	GoodItem = 0} -- Honey Chocolate
BoxXiang_BaoZhaBOX[2 ] = {Active = 1,	ItemID = 3094,	Quantity = 2,	Quality = 5,	Rad = 566,	GoodItem = 0} -- Amplifier of Strive
BoxXiang_BaoZhaBOX[3 ] = {Active = 1,	ItemID = 3096,	Quantity = 4,	Quality = 5,	Rad = 566,	GoodItem = 0} -- Amplifier of Luck
BoxXiang_BaoZhaBOX[4 ] = {Active = 1,	ItemID = 3107,	Quantity = 3,	Quality = 5,	Rad = 566,	GoodItem = 0} -- lantern festival hurricane lamp
BoxXiang_BaoZhaBOX[5 ] = {Active = 1,	ItemID = 4272,	Quantity = 8,	Quality = 5,	Rad = 566,	GoodItem = 0} -- Demon Emote
BoxXiang_BaoZhaBOX[6 ] = {Active = 1,	ItemID = 4273,	Quantity = 8,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Skeleton Emote
BoxXiang_BaoZhaBOX[7 ] = {Active = 1,	ItemID = 4270,	Quantity = 8,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Rainbow Emote
BoxXiang_BaoZhaBOX[8 ] = {Active = 1,	ItemID = 4271,	Quantity = 8,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Angel Emote
BoxXiang_BaoZhaBOX[9 ] = {Active = 1,	ItemID = 0227,	Quantity = 4,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Fairy Ration
BoxXiang_BaoZhaBOX[10] = {Active = 1,	ItemID = 0905,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Dragon Wings
BoxXiang_BaoZhaBOX[11] = {Active = 1,	ItemID = 3047,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Voodoo Puppet
BoxXiang_BaoZhaBOX[12] = {Active = 1,	ItemID = 0932,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Carsise Trendy Hairstyle Book
BoxXiang_BaoZhaBOX[13] = {Active = 1,	ItemID = 0931,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Lance Trendy Hairstyle Book
BoxXiang_BaoZhaBOX[14] = {Active = 1,	ItemID = 0933,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Phyllis Trendy Hairstyle Book
BoxXiang_BaoZhaBOX[15] = {Active = 1,	ItemID = 0934,	Quantity = 1,	Quality = 5,	Rad = 567,	GoodItem = 0} -- Ami Trendy Hairstyle Book
BoxXiang_BaoZhaBOX[16] = {Active = 1,	ItemID = 3111,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Vial of Agility Reset
BoxXiang_BaoZhaBOX[17] = {Active = 1,	ItemID = 3110,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Vial of Consitution Reset
BoxXiang_BaoZhaBOX[18] = {Active = 1,	ItemID = 3112,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Vial of Accuracy Reset
BoxXiang_BaoZhaBOX[19] = {Active = 1,	ItemID = 3886,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Gem Voucher
BoxXiang_BaoZhaBOX[20] = {Active = 1,	ItemID = 3093,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- 48 Slot Inventory Enlarger
BoxXiang_BaoZhaBOX[21] = {Active = 1,	ItemID = 3090,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- 36 Slot Inventory Enlarger
BoxXiang_BaoZhaBOX[22] = {Active = 1,	ItemID = 0430,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Egg of Love
BoxXiang_BaoZhaBOX[23] = {Active = 1,	ItemID = 0179,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Timeless Machine
BoxXiang_BaoZhaBOX[24] = {Active = 1,	ItemID = 3084,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Sigil of Anubis
BoxXiang_BaoZhaBOX[25] = {Active = 1,	ItemID = 3085,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Mask of Mummy King
BoxXiang_BaoZhaBOX[26] = {Active = 1,	ItemID = 0244,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Standard Protection
BoxXiang_BaoZhaBOX[27] = {Active = 1,	ItemID = 0250,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Standard Magic
BoxXiang_BaoZhaBOX[28] = {Active = 1,	ItemID = 0253,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Standard Recover
BoxXiang_BaoZhaBOX[29] = {Active = 1,	ItemID = 0260,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 1} -- Standard Meditation
BoxXiang_BaoZhaBOX[30] = {Active = 1,	ItemID = 0860,	Quantity = 1,	Quality = 5,	Rad = 22 ,	GoodItem = 1} -- Gem of the Wind
BoxXiang_BaoZhaBOX[31] = {Active = 1,	ItemID = 0861,	Quantity = 1,	Quality = 5,	Rad = 23 ,	GoodItem = 1} -- Gem of Striking
BoxXiang_BaoZhaBOX[32] = {Active = 1,	ItemID = 0862,	Quantity = 1,	Quality = 5,	Rad = 22 ,	GoodItem = 1} -- Gem of Colossus
BoxXiang_BaoZhaBOX[33] = {Active = 1,	ItemID = 3458,	Quantity = 1,	Quality = 5,	Rad = 22 ,	GoodItem = 1} -- Chest of Enigma
BoxXiang_BaoZhaBOX[34] = {Active = 1,	ItemID = 0247,	Quantity = 1,	Quality = 5,	Rad = 11 ,	GoodItem = 1} -- Standard Berserk
BoxXiang_BaoZhaBOX[35] = {Active = 1,	ItemID = 0271,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 1} -- Angelic Dice

-- [1096] Anniversary Chest
BoxXiang_ZhousSuiBOX = {}
BoxXiang_ZhousSuiBOX[1 ] = {Active = 1,	ItemID = 0853,	Quantity = 1,	Quality = 5,	Rad = 2,	GoodItem = 1} -- Happy Holiday Magazine
BoxXiang_ZhousSuiBOX[2 ] = {Active = 1,	ItemID = 3047,	Quantity = 5,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Voodoo Puppet
BoxXiang_ZhousSuiBOX[3 ] = {Active = 0,	ItemID = 3112,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Vial of Accuracy Reset
BoxXiang_ZhousSuiBOX[4 ] = {Active = 0,	ItemID = 3110,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Vial of Consitution Reset
BoxXiang_ZhousSuiBOX[5 ] = {Active = 0,	ItemID = 3111,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Vial of Agility Reset
BoxXiang_ZhousSuiBOX[6 ] = {Active = 0,	ItemID = 3113,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Vial of Spirit Reset
BoxXiang_ZhousSuiBOX[7 ] = {Active = 0,	ItemID = 3109,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Vial of Strength Reset
BoxXiang_ZhousSuiBOX[8 ] = {Active = 1,	ItemID = 3092,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- 44 Slot Inventory Enlarger
BoxXiang_ZhousSuiBOX[9 ] = {Active = 1,	ItemID = 3089,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- 32 Slot Inventory Enlarger
BoxXiang_ZhousSuiBOX[10] = {Active = 1,	ItemID = 0430,	Quantity = 1,	Quality = 5,	Rad = 5,	GoodItem = 0} -- Egg of Love
BoxXiang_ZhousSuiBOX[11] = {Active = 1,	ItemID = 4273,	Quantity = 20,	Quality = 5,	Rad = 9,	GoodItem = 0} -- Skeleton Emote
BoxXiang_ZhousSuiBOX[12] = {Active = 1,	ItemID = 4271,	Quantity = 20,	Quality = 5,	Rad = 9,	GoodItem = 0} -- Angel Emote
BoxXiang_ZhousSuiBOX[13] = {Active = 1,	ItemID = 3096,	Quantity = 10,	Quality = 5,	Rad = 9,	GoodItem = 0} -- Amplifier of Luck
BoxXiang_ZhousSuiBOX[14] = {Active = 1,	ItemID = 0227,	Quantity = 10,	Quality = 5,	Rad = 9,	GoodItem = 0} -- Fairy Ration
BoxXiang_ZhousSuiBOX[15] = {Active = 1,	ItemID = 0937,	Quantity = 1,	Quality = 5,	Rad = 9,	GoodItem = 0} -- Angelic Wings
BoxXiang_ZhousSuiBOX[16] = {Active = 1,	ItemID = 3094,	Quantity = 10,	Quality = 5,	Rad = 8,	GoodItem = 0} -- Amplifier of Strive

-- [1119] Paradise Pouch
BaoXiang_98BOX = {}
BaoXiang_98BOX[1 ] = {Active = 1,	ItemID = 2440,	Quantity = 30,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Lock of Mystic
BaoXiang_98BOX[2 ] = {Active = 1,	ItemID = 3098,	Quantity = 6,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Constitution Recovery Flask
BaoXiang_98BOX[3 ] = {Active = 1,	ItemID = 3096,	Quantity = 3,	Quality = 5,	Rad = 50 ,	GoodItem = 0} -- Amplifier of Luck
BaoXiang_98BOX[4 ] = {Active = 1,	ItemID = 0227,	Quantity = 3,	Quality = 5,	Rad = 50 ,	GoodItem = 0} -- Fairy Ration
BaoXiang_98BOX[5 ] = {Active = 1,	ItemID = 3105,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Skating Potion
BaoXiang_98BOX[6 ] = {Active = 1,	ItemID = 3107,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- lantern festival hurricane lamp
BaoXiang_98BOX[7 ] = {Active = 1,	ItemID = 0563,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Pass to Summer
BaoXiang_98BOX[8 ] = {Active = 1,	ItemID = 0583,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Pass to Autumn
BaoXiang_98BOX[9 ] = {Active = 1,	ItemID = 2445,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Caribbean Tour Ticket
BaoXiang_98BOX[10] = {Active = 1,	ItemID = 0455,	Quantity = 2,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Strengthening Scroll
BaoXiang_98BOX[11] = {Active = 1,	ItemID = 3094,	Quantity = 2,	Quality = 5,	Rad = 80 ,	GoodItem = 0} -- Amplifier of Strive
BaoXiang_98BOX[12] = {Active = 1,	ItemID = 0849,	Quantity = 1,	Quality = 5,	Rad = 70 ,	GoodItem = 0} -- Party EXP Fruit
BaoXiang_98BOX[13] = {Active = 1,	ItemID = 3074,	Quantity = 1,	Quality = 5,	Rad = 20 ,	GoodItem = 1} -- Refining Catalyst
BaoXiang_98BOX[14] = {Active = 1,	ItemID = 3075,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 1} -- Composition Catalyst
BaoXiang_98BOX[15] = {Active = 1,	ItemID = 0937,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 1} -- Angelic Wings
BaoXiang_98BOX[16] = {Active = 1,	ItemID = 3885,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 1} -- Refining Gem Voucher
BaoXiang_98BOX[17] = {Active = 1,	ItemID = 0680,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 1} -- Mordo
BaoXiang_98BOX[18] = {Active = 1,	ItemID = 0938,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 1} -- Goddess's Favor
BaoXiang_98BOX[19] = {Active = 1,	ItemID = 2488,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 1} -- Bone of Mummy
BaoXiang_98BOX[20] = {Active = 1,	ItemID = 2489,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 1} -- Soul of Corpse Soldier
BaoXiang_98BOX[21] = {Active = 1,	ItemID = 2490,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 1} -- Heart of Pharaoh
BaoXiang_98BOX[22] = {Active = 1,	ItemID = 2436,	Quantity = 1,	Quality = 5,	Rad = 2  ,	GoodItem = 1} -- Holy Bible

-- [1103] Lv20 Unique Ring Voucher
BaoXiang_BenteBOX = {}
BaoXiang_BenteBOX[1] = {Active = 1,	ItemID = 281,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Final Perseverance
BaoXiang_BenteBOX[2] = {Active = 1,	ItemID = 282,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Bandit Scoop
BaoXiang_BenteBOX[3] = {Active = 1,	ItemID = 283,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Hunter's Strength
BaoXiang_BenteBOX[4] = {Active = 1,	ItemID = 284,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Followers Loop
BaoXiang_BenteBOX[5] = {Active = 1,	ItemID = 285,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Spirit Loop

-- [1104] Lv30 Unique Ring Voucher
BaoXiang_TrentaBOX = {}
BaoXiang_TrentaBOX[1] = {Active = 1,	ItemID = 286,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Ancient Tree Tuber
BaoXiang_TrentaBOX[2] = {Active = 1,	ItemID = 287,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- May Breeze
BaoXiang_TrentaBOX[3] = {Active = 1,	ItemID = 288,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Fairy's Adulation
BaoXiang_TrentaBOX[4] = {Active = 1,	ItemID = 524,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Meditating Monk
BaoXiang_TrentaBOX[5] = {Active = 1,	ItemID = 321,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Saint Loop

-- [1105] Lv40 Unique Ring Voucher
BaoXiang_QuarentaBOX = {}
BaoXiang_QuarentaBOX[1] = {Active = 1,	ItemID = 324,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Seal of Strength
BaoXiang_QuarentaBOX[2] = {Active = 1,	ItemID = 327,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Spectral Wolf Button
BaoXiang_QuarentaBOX[3] = {Active = 1,	ItemID = 328,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Ring of Dragon Wing
BaoXiang_QuarentaBOX[4] = {Active = 1,	ItemID = 329,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Unicorn's Blessing
BaoXiang_QuarentaBOX[5] = {Active = 1,	ItemID = 330,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Angel's Grief

-- [1106] Lv50 Unique Ring Voucher
BaoXiang_LimaBOX = {}
BaoXiang_LimaBOX[1] = {Active = 1,	ItemID = 334,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Hammer of Thunder
BaoXiang_LimaBOX[2] = {Active = 1,	ItemID = 346,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Eye of Lightning
BaoXiang_LimaBOX[3] = {Active = 1,	ItemID = 347,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Wings of Light
BaoXiang_LimaBOX[4] = {Active = 1,	ItemID = 348,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Morning Bell
BaoXiang_LimaBOX[5] = {Active = 1,	ItemID = 349,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Nature's Whisper

-- [1107] Lv60 Unique Ring Voucher
BaoXiang_AnimBOX = {}
BaoXiang_AnimBOX[1] = {Active = 1,	ItemID = 387,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_AnimBOX[2] = {Active = 1,	ItemID = 414,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic
BaoXiang_AnimBOX[3] = {Active = 1,	ItemID = 415,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Ray's Fury
BaoXiang_AnimBOX[4] = {Active = 1,	ItemID = 416,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Daniel's Regret
BaoXiang_AnimBOX[5] = {Active = 1,	ItemID = 417,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Consecration of Priestess

-- [1108] Lv20 Unique Necklace Voucher
BaoXiang_Quintas2BOX = {}
BaoXiang_Quintas2BOX[1] = {Active = 1,	ItemID = 418,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_Quintas2BOX[2] = {Active = 1,	ItemID = 419,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic

-- [1109] Lv30 Unique Necklace Voucher
BaoXiang_Quintas3BOX = {}
BaoXiang_Quintas3BOX[1] = {Active = 1,	ItemID = 420,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_Quintas3BOX[2] = {Active = 1,	ItemID = 421,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic

-- [1110] Lv40 Unique Necklace Voucher
BaoXiang_Quintas4BOX = {}
BaoXiang_Quintas4BOX[1] = {Active = 1,	ItemID = 739,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_Quintas4BOX[2] = {Active = 1,	ItemID = 461,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic

-- [1111] Lv50 Unique Necklace Voucher
BaoXiang_Quintas5BOX = {}
BaoXiang_Quintas5BOX[1] = {Active = 1,	ItemID = 462,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_Quintas5BOX[2] = {Active = 1,	ItemID = 463,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic

-- [1112] Lv60 Unique Necklace Voucher
BaoXiang_Quintas6BOX = {}
BaoXiang_Quintas6BOX[1] = {Active = 1,	ItemID = 495,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Peter's Call
BaoXiang_Quintas6BOX[2] = {Active = 1,	ItemID = 497,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0} -- Kiss of Nic

-- [0582] Unique Coral Voucher
BaoXiang_DeCoralBOX = {}
BaoXiang_DeCoralBOX[1] = {Active = 1,	ItemID = 0504,	Quantity = 1,	Quality = 2,	Rad = 50,	GoodItem = 0} -- Standard Thunder Coral
BaoXiang_DeCoralBOX[2] = {Active = 1,	ItemID = 0505,	Quantity = 1,	Quality = 2,	Rad = 50,	GoodItem = 0} -- Expert Thunder Coral
BaoXiang_DeCoralBOX[3] = {Active = 1,	ItemID = 0506,	Quantity = 1,	Quality = 2,	Rad = 50,	GoodItem = 0} -- Thunder Coral of Thor
BaoXiang_DeCoralBOX[4] = {Active = 1,	ItemID = 0510,	Quantity = 1,	Quality = 2,	Rad = 25,	GoodItem = 0} -- Standard Wind Coral
BaoXiang_DeCoralBOX[5] = {Active = 1,	ItemID = 0522,	Quantity = 1,	Quality = 2,	Rad = 25,	GoodItem = 0} -- Expert Wind Coral
BaoXiang_DeCoralBOX[6] = {Active = 1,	ItemID = 0523,	Quantity = 1,	Quality = 2,	Rad = 25,	GoodItem = 0} -- Wind Coral of Fujin
BaoXiang_DeCoralBOX[7] = {Active = 1,	ItemID = 1100,	Quantity = 1,	Quality = 2,	Rad = 5,	GoodItem = 0} -- Standard Gale Coral
BaoXiang_DeCoralBOX[8] = {Active = 1,	ItemID = 1101,	Quantity = 1,	Quality = 2,	Rad = 5,	GoodItem = 0} -- Expert Gale Coral	
BaoXiang_DeCoralBOX[9] = {Active = 1,	ItemID = 1102,	Quantity = 1,	Quality = 2,	Rad = 5,	GoodItem = 0} -- Gale Coral of Susanoo

-- [9600] Mystery Box
BaoXiang_MysteryBOX = {}
BaoXiang_MysteryBOX[1 ] = {Active = 1,	ItemID = 07002,	Quantity = 1,	Quality = 4,	Rad = 5,	GoodItem = 0} -- Arthur's Faithe
BaoXiang_MysteryBOX[2 ] = {Active = 1,	ItemID = 13871,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Cloak Upgrade Devicer
BaoXiang_MysteryBOX[3 ] = {Active = 1,	ItemID = 20496,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Fallen Guardian Wingsntain
BaoXiang_MysteryBOX[4 ] = {Active = 1,	ItemID = 08131,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Valentine's Day Effect 1ven
BaoXiang_MysteryBOX[5 ] = {Active = 1,	ItemID = 08138,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Valentine's Day Effect 2
BaoXiang_MysteryBOX[6 ] = {Active = 1,	ItemID = 09601,	Quantity = 1,	Quality = 4,	Rad = 35,	GoodItem = 0} -- Lv65 Ring Voucher
BaoXiang_MysteryBOX[7 ] = {Active = 1,	ItemID = 04019,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Codfish Steamboat
BaoXiang_MysteryBOX[8 ] = {Active = 1,	ItemID = 04021,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Savory Bubble Fish
BaoXiang_MysteryBOX[9 ] = {Active = 1,	ItemID = 04022,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Sturgeon Soup2
BaoXiang_MysteryBOX[10] = {Active = 1,	ItemID = 04023,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Fried Oyster Soup
BaoXiang_MysteryBOX[11] = {Active = 1,	ItemID = 04024,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Prawn Dumpling
BaoXiang_MysteryBOX[12] = {Active = 1,	ItemID = 04025,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Tigerfish Bone Crisp
BaoXiang_MysteryBOX[13] = {Active = 1,	ItemID = 04026,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Ratfish Rice
BaoXiang_MysteryBOX[14] = {Active = 1,	ItemID = 04027,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- China Clay	n1688
BaoXiang_MysteryBOX[15] = {Active = 1,	ItemID = 04028,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- BBQ Shark Fin
BaoXiang_MysteryBOX[16] = {Active = 1,	ItemID = 06407,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Steam Bun
BaoXiang_MysteryBOX[17] = {Active = 1,	ItemID = 01079,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Bun
BaoXiang_MysteryBOX[18] = {Active = 1,	ItemID = 01080,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Biscuit
BaoXiang_MysteryBOX[19] = {Active = 1,	ItemID = 01082,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Fried Dough
BaoXiang_MysteryBOX[20] = {Active = 1,	ItemID = 01083,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Spring Roll
BaoXiang_MysteryBOX[21] = {Active = 1,	ItemID = 01084,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Maiden Wine
BaoXiang_MysteryBOX[22] = {Active = 1,	ItemID = 01085,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Scholar Wine
BaoXiang_MysteryBOX[23] = {Active = 1,	ItemID = 01087,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Mao Wine
BaoXiang_MysteryBOX[24] = {Active = 1,	ItemID = 01088,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Dukan Wine
BaoXiang_MysteryBOX[25] = {Active = 1,	ItemID = 01089,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Ginseng Wine
BaoXiang_MysteryBOX[26] = {Active = 1,	ItemID = 01090,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Tiger Bone Tonic
BaoXiang_MysteryBOX[27] = {Active = 1,	ItemID = 03339,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Acceleration Potion
BaoXiang_MysteryBOX[28] = {Active = 1,	ItemID = 03342,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Lantern
BaoXiang_MysteryBOX[29] = {Active = 1,	ItemID = 00852,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Full Body Armor
BaoXiang_MysteryBOX[30] = {Active = 1,	ItemID = 03098,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- Constitution Recovery Flask
BaoXiang_MysteryBOX[31] = {Active = 1,	ItemID = 03099,	Quantity = 2,	Quality = 4,	Rad = 80,	GoodItem = 0} -- SP Holy Water
BaoXiang_MysteryBOX[32] = {Active = 1,	ItemID = 01135,	Quantity = 1,	Quality = 4,	Rad = 60,	GoodItem = 0} -- Grenade Lv1
BaoXiang_MysteryBOX[33] = {Active = 1,	ItemID = 01136,	Quantity = 1,	Quality = 4,	Rad = 60,	GoodItem = 0} -- Flash Bomb Lv1
BaoXiang_MysteryBOX[34] = {Active = 1,	ItemID = 01137,	Quantity = 1,	Quality = 4,	Rad = 60,	GoodItem = 0} -- Radiation Lv1
BaoXiang_MysteryBOX[35] = {Active = 1,	ItemID = 985,	Quantity = 1,	Quality = 4,	Rad = 15,	GoodItem = 0} -- Lv50 Magenta Chest
BaoXiang_MysteryBOX[36] = {Active = 1,	ItemID = 986,	Quantity = 1,	Quality = 4,	Rad = 15,	GoodItem = 0} -- Lv60 Magenta Chest
BaoXiang_MysteryBOX[37] = {Active = 1,	ItemID = 276,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Great Fruits
BaoXiang_MysteryBOX[38] = {Active = 1,	ItemID = 277,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Great Fruits
BaoXiang_MysteryBOX[39] = {Active = 1,	ItemID = 278,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Great Fruits
BaoXiang_MysteryBOX[40] = {Active = 1,	ItemID = 279,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Great Fruits
BaoXiang_MysteryBOX[41] = {Active = 1,	ItemID = 280,	Quantity = 1,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Great Fruits
BaoXiang_MysteryBOX[42] = {Active = 1,	ItemID = 890,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Socket Set
BaoXiang_MysteryBOX[43] = {Active = 1,	ItemID = 891,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Socket Set
BaoXiang_MysteryBOX[44] = {Active = 1,	ItemID = 455,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Apparel Upgrade
BaoXiang_MysteryBOX[45] = {Active = 1,	ItemID = 456,	Quantity = 1,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Apparel Upgrade
BaoXiang_MysteryBOX[46] = {Active = 1,	ItemID = 227,	Quantity = 10,	Quality = 4,	Rad = 30,	GoodItem = 0} -- Rations
BaoXiang_MysteryBOX[47] = {Active = 1,	ItemID = 5644,	Quantity = 10,	Quality = 4,	Rad = 20,	GoodItem = 0} -- Rations

-- [88]	Black Market Equipment
BaoXiang_jsmzcqa = {}
BaoXiang_jsmzcqa[1 ] = {Active = 1,	ItemID = 1902,	Quantity = 1,	Quality = 24,	Rad = 100  ,	GoodItem = 0} -- Undead Sealed Touch of Death
BaoXiang_jsmzcqa[2 ] = {Active = 1,	ItemID = 1903,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Kiss of the Cursed
BaoXiang_jsmzcqa[3 ] = {Active = 1,	ItemID = 1904,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Bellow of Evanescence
BaoXiang_jsmzcqa[4 ] = {Active = 1,	ItemID = 1905,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Rifle of Enigma
BaoXiang_jsmzcqa[5 ] = {Active = 1,	ItemID = 0120,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Blitz Thunderbolt
BaoXiang_jsmzcqa[6 ] = {Active = 1,	ItemID = 0042,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Venom Gun
BaoXiang_jsmzcqa[7 ] = {Active = 1,	ItemID = 0041,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Laser Gun
BaoXiang_jsmzcqa[8 ] = {Active = 1,	ItemID = 1410,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Holy Judgment
BaoXiang_jsmzcqa[9 ] = {Active = 1,	ItemID = 3808,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Rifle of the White Tiger
BaoXiang_jsmzcqa[10] = {Active = 1,	ItemID = 4214,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Rattlesnake
BaoXiang_jsmzcqa[11] = {Active = 1,	ItemID = 4215,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Serpentine Gun of Resonance
BaoXiang_jsmzcqa[12] = {Active = 1,	ItemID = 0040,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Exquisite Rifle
BaoXiang_jsmzcqa[13] = {Active = 1,	ItemID = 0045,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Gattling Firegun
BaoXiang_jsmzcqa[14] = {Active = 1,	ItemID = 1409,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Battle Rifle
BaoXiang_jsmzcqa[15] = {Active = 1,	ItemID = 1414,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Spectar Firegun
BaoXiang_jsmzcqa[16] = {Active = 1,	ItemID = 0039,	Quantity = 1,	Quality = 24,	Rad = 17000,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_jsmzcqa[17] = {Active = 1,	ItemID = 0044,	Quantity = 1,	Quality = 24,	Rad = 17000,	GoodItem = 0} -- Token Pistol
BaoXiang_jsmzcqa[18] = {Active = 1,	ItemID = 1408,	Quantity = 1,	Quality = 24,	Rad = 17000,	GoodItem = 0} -- Pocket Pistol
BaoXiang_jsmzcqa[19] = {Active = 1,	ItemID = 1413,	Quantity = 1,	Quality = 24,	Rad = 17000,	GoodItem = 0} -- Steel Pistol
BaoXiang_jsmzcqa[20] = {Active = 1,	ItemID = 3806,	Quantity = 1,	Quality = 24,	Rad = 17000,	GoodItem = 0} -- Flaming Pistol

-- [89]	Black Market Equipment
BaoXiang_jsmzcqb = {}
BaoXiang_jsmzcqb[1 ] = {Active = 1,	ItemID = 1902,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Undead Sealed Touch of Death
BaoXiang_jsmzcqb[2 ] = {Active = 1,	ItemID = 1903,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Kiss of the Cursed
BaoXiang_jsmzcqb[3 ] = {Active = 1,	ItemID = 1904,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Bellow of Evanescence
BaoXiang_jsmzcqb[4 ] = {Active = 1,	ItemID = 1905,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Rifle of Enigma
BaoXiang_jsmzcqb[5 ] = {Active = 1,	ItemID = 0120,	Quantity = 1,	Quality = 23,	Rad = 40   ,	GoodItem = 0} -- Blitz Thunderbolt
BaoXiang_jsmzcqb[6 ] = {Active = 1,	ItemID = 0042,	Quantity = 1,	Quality = 23,	Rad = 40   ,	GoodItem = 0} -- Venom Gun
BaoXiang_jsmzcqb[7 ] = {Active = 1,	ItemID = 0041,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Laser Gun
BaoXiang_jsmzcqb[8 ] = {Active = 1,	ItemID = 1410,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Holy Judgment
BaoXiang_jsmzcqb[9 ] = {Active = 1,	ItemID = 3808,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Rifle of the White Tiger
BaoXiang_jsmzcqb[10] = {Active = 1,	ItemID = 4214,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Rattlesnake
BaoXiang_jsmzcqb[11] = {Active = 1,	ItemID = 4215,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Serpentine Gun of Resonance
BaoXiang_jsmzcqb[12] = {Active = 1,	ItemID = 0040,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Exquisite Rifle
BaoXiang_jsmzcqb[13] = {Active = 1,	ItemID = 0045,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Gattling Firegun
BaoXiang_jsmzcqb[14] = {Active = 1,	ItemID = 1409,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Battle Rifle
BaoXiang_jsmzcqb[15] = {Active = 1,	ItemID = 1414,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Spectar Firegun
BaoXiang_jsmzcqb[16] = {Active = 1,	ItemID = 0039,	Quantity = 1,	Quality = 23,	Rad = 17500,	GoodItem = 0} -- Exquisite Pistol
BaoXiang_jsmzcqb[17] = {Active = 1,	ItemID = 0044,	Quantity = 1,	Quality = 23,	Rad = 17500,	GoodItem = 0} -- Token Pistol
BaoXiang_jsmzcqb[18] = {Active = 1,	ItemID = 1408,	Quantity = 1,	Quality = 23,	Rad = 17500,	GoodItem = 0} -- Pocket Pistol
BaoXiang_jsmzcqb[19] = {Active = 1,	ItemID = 1413,	Quantity = 1,	Quality = 23,	Rad = 17500,	GoodItem = 0} -- Steel Pistol
BaoXiang_jsmzcqb[20] = {Active = 1,	ItemID = 3806,	Quantity = 1,	Quality = 23,	Rad = 17500,	GoodItem = 0} -- Flaming Pistol

-- [3302] Black Market Equipment
BaoXiang_jsyla = {}
BaoXiang_jsyla[1 ] = {Active = 1,	ItemID = 0114,	Quantity = 1,	Quality = 24,	Rad = 1   ,		GoodItem = 0} -- Drakan
BaoXiang_jsyla[2 ] = {Active = 1,	ItemID = 3302,	Quantity = 1,	Quality = 24,	Rad = 30  ,		GoodItem = 0} -- Black Market Equipment
BaoXiang_jsyla[3 ] = {Active = 1,	ItemID = 0007,	Quantity = 1,	Quality = 24,	Rad = 30  ,		GoodItem = 0} -- Sacro Sword
BaoXiang_jsyla[4 ] = {Active = 1,	ItemID = 1394,	Quantity = 1,	Quality = 24,	Rad = 30  ,		GoodItem = 0} -- Sword of Dawn
BaoXiang_jsyla[5 ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Wyrm Sword
BaoXiang_jsyla[6 ] = {Active = 1,	ItemID = 1393,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Hymn Sword of Darkan
BaoXiang_jsyla[7 ] = {Active = 1,	ItemID = 3801,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Sword of the Kylin
BaoXiang_jsyla[8 ] = {Active = 1,	ItemID = 4212,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Ember Scar
BaoXiang_jsyla[9 ] = {Active = 1,	ItemID = 4213,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Swift Lightning
BaoXiang_jsyla[10] = {Active = 1,	ItemID = 0003,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Fencing Sword
BaoXiang_jsyla[11] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Serpentine Sword
BaoXiang_jsyla[12] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Dazzling Sword
BaoXiang_jsyla[13] = {Active = 1,	ItemID = 1390,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Fine Blade
BaoXiang_jsyla[14] = {Active = 1,	ItemID = 1391,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Emerald Blade
BaoXiang_jsyla[15] = {Active = 1,	ItemID = 1392,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Steel Saber
BaoXiang_jsyla[16] = {Active = 1,	ItemID = 1397,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Assassin Sword
BaoXiang_jsyla[17] = {Active = 1,	ItemID = 1398,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Tribal Sword
BaoXiang_jsyla[18] = {Active = 1,	ItemID = 1399,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Amber Sword
BaoXiang_jsyla[19] = {Active = 1,	ItemID = 0022,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Crescent Sword

-- [3303] Black Market Equipment
BaoXiang_jsylb = {}
BaoXiang_jsylb[1 ] = {Active = 1,	ItemID = 0114,	Quantity = 1,	Quality = 23,	Rad = 5   ,		GoodItem = 0} -- Drakan
BaoXiang_jsylb[2 ] = {Active = 1,	ItemID = 3303,	Quantity = 1,	Quality = 23,	Rad = 15  ,		GoodItem = 0} -- Black Market Equipment
BaoXiang_jsylb[3 ] = {Active = 1,	ItemID = 0007,	Quantity = 1,	Quality = 23,	Rad = 15  ,		GoodItem = 0} -- Sacro Sword
BaoXiang_jsylb[4 ] = {Active = 1,	ItemID = 1394,	Quantity = 1,	Quality = 23,	Rad = 15  ,		GoodItem = 0} -- Sword of Dawn
BaoXiang_jsylb[5 ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 23,	Rad = 150 ,		GoodItem = 0} -- Wyrm Sword
BaoXiang_jsylb[6 ] = {Active = 1,	ItemID = 1393,	Quantity = 1,	Quality = 23,	Rad = 150 ,		GoodItem = 0} -- Hymn Sword of Darkan
BaoXiang_jsylb[7 ] = {Active = 1,	ItemID = 3801,	Quantity = 1,	Quality = 23,	Rad = 150 ,		GoodItem = 0} -- Sword of the Kylin
BaoXiang_jsylb[8 ] = {Active = 1,	ItemID = 4212,	Quantity = 1,	Quality = 23,	Rad = 150 ,		GoodItem = 0} -- Ember Scar
BaoXiang_jsylb[9 ] = {Active = 1,	ItemID = 4213,	Quantity = 1,	Quality = 23,	Rad = 150 ,		GoodItem = 0} -- Swift Lightning
BaoXiang_jsylb[10] = {Active = 1,	ItemID = 0003,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Fencing Sword
BaoXiang_jsylb[11] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Serpentine Sword
BaoXiang_jsylb[12] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Dazzling Sword
BaoXiang_jsylb[13] = {Active = 1,	ItemID = 1390,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Fine Blade
BaoXiang_jsylb[14] = {Active = 1,	ItemID = 1391,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Emerald Blade
BaoXiang_jsylb[15] = {Active = 1,	ItemID = 1392,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Steel Saber
BaoXiang_jsylb[16] = {Active = 1,	ItemID = 1397,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Assassin Sword
BaoXiang_jsylb[17] = {Active = 1,	ItemID = 1398,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Tribal Sword
BaoXiang_jsylb[18] = {Active = 1,	ItemID = 1399,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Amber Sword
BaoXiang_jsylb[19] = {Active = 1,	ItemID = 0022,	Quantity = 1,	Quality = 23,	Rad = 9200,		GoodItem = 0} -- Crescent Sword

-- [3304] Black Market Equipment
BaoXiang_jsmzlra = {}
BaoXiang_jsmzlra[1 ] = {Active = 1,	ItemID = 1895,	Quantity = 1,	Quality = 24,	Rad = 30  ,		GoodItem = 0} -- Earth Sealed Blade of Incantation
BaoXiang_jsmzlra[2 ] = {Active = 1,	ItemID = 1896,	Quantity = 1,	Quality = 24,	Rad = 20  ,		GoodItem = 0} -- Fire Sealed Dance of Evanescence
BaoXiang_jsmzlra[3 ] = {Active = 1,	ItemID = 1897,	Quantity = 1,	Quality = 24,	Rad = 1   ,		GoodItem = 0} -- Ice Sealed Blade of Enigma
BaoXiang_jsmzlra[4 ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 24,	Rad = 200 ,		GoodItem = 0} -- Wyrm Sword
BaoXiang_jsmzlra[5 ] = {Active = 1,	ItemID = 1393,	Quantity = 1,	Quality = 24,	Rad = 200 ,		GoodItem = 0} -- Hymn Sword of Darkan
BaoXiang_jsmzlra[6 ] = {Active = 1,	ItemID = 3801,	Quantity = 1,	Quality = 24,	Rad = 200 ,		GoodItem = 0} -- Sword of the Kylin
BaoXiang_jsmzlra[7 ] = {Active = 1,	ItemID = 4212,	Quantity = 1,	Quality = 24,	Rad = 200 ,		GoodItem = 0} -- Ember Scar
BaoXiang_jsmzlra[8 ] = {Active = 1,	ItemID = 4213,	Quantity = 1,	Quality = 24,	Rad = 200 ,		GoodItem = 0} -- Swift Lightning
BaoXiang_jsmzlra[9 ] = {Active = 1,	ItemID = 0003,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Fencing Sword
BaoXiang_jsmzlra[10] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Serpentine Sword
BaoXiang_jsmzlra[11] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Dazzling Sword
BaoXiang_jsmzlra[12] = {Active = 1,	ItemID = 1390,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Fine Blade
BaoXiang_jsmzlra[13] = {Active = 1,	ItemID = 1391,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Emerald Blade
BaoXiang_jsmzlra[14] = {Active = 1,	ItemID = 1392,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Steel Saber
BaoXiang_jsmzlra[15] = {Active = 1,	ItemID = 1397,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Assassin Sword
BaoXiang_jsmzlra[16] = {Active = 1,	ItemID = 1398,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Tribal Sword
BaoXiang_jsmzlra[17] = {Active = 1,	ItemID = 1399,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Amber Sword
BaoXiang_jsmzlra[18] = {Active = 1,	ItemID = 0022,	Quantity = 1,	Quality = 24,	Rad = 9000,		GoodItem = 0} -- Crescent Sword

-- [3305] Black Market Equipment
BaoXiang_jsmzlrb = {}
BaoXiang_jsmzlrb[1 ] = {Active = 1,	ItemID = 1895,	Quantity = 1,	Quality = 23,	Rad = 30  ,		GoodItem = 0} -- Earth Sealed Blade of Incantation
BaoXiang_jsmzlrb[2 ] = {Active = 1,	ItemID = 1896,	Quantity = 1,	Quality = 23,	Rad = 10  ,		GoodItem = 0} -- Fire Sealed Dance of Evanescence
BaoXiang_jsmzlrb[3 ] = {Active = 1,	ItemID = 1897,	Quantity = 1,	Quality = 23,	Rad = 5   ,		GoodItem = 0} -- Ice Sealed Blade of Enigma
BaoXiang_jsmzlrb[4 ] = {Active = 1,	ItemID = 0006,	Quantity = 1,	Quality = 23,	Rad = 175 ,		GoodItem = 0} -- Wyrm Sword
BaoXiang_jsmzlrb[5 ] = {Active = 1,	ItemID = 1393,	Quantity = 1,	Quality = 23,	Rad = 175 ,		GoodItem = 0} -- Hymn Sword of Darkan
BaoXiang_jsmzlrb[6 ] = {Active = 1,	ItemID = 3801,	Quantity = 1,	Quality = 23,	Rad = 175 ,		GoodItem = 0} -- Sword of the Kylin
BaoXiang_jsmzlrb[7 ] = {Active = 1,	ItemID = 4212,	Quantity = 1,	Quality = 23,	Rad = 175 ,		GoodItem = 0} -- Ember Scar
BaoXiang_jsmzlrb[8 ] = {Active = 1,	ItemID = 4213,	Quantity = 1,	Quality = 23,	Rad = 175 ,		GoodItem = 0} -- Swift Lightning
BaoXiang_jsmzlrb[9 ] = {Active = 1,	ItemID = 0003,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Fencing Sword
BaoXiang_jsmzlrb[10] = {Active = 1,	ItemID = 0004,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Serpentine Sword
BaoXiang_jsmzlrb[11] = {Active = 1,	ItemID = 0005,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Dazzling Sword
BaoXiang_jsmzlrb[12] = {Active = 1,	ItemID = 1390,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Fine Blade
BaoXiang_jsmzlrb[13] = {Active = 1,	ItemID = 1391,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Emerald Blade
BaoXiang_jsmzlrb[14] = {Active = 1,	ItemID = 1392,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Steel Saber
BaoXiang_jsmzlrb[15] = {Active = 1,	ItemID = 1397,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Assassin Sword
BaoXiang_jsmzlrb[16] = {Active = 1,	ItemID = 1398,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Tribal Sword
BaoXiang_jsmzlrb[17] = {Active = 1,	ItemID = 1399,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Amber Sword
BaoXiang_jsmzlrb[18] = {Active = 1,	ItemID = 0022,	Quantity = 1,	Quality = 23,	Rad = 9100,		GoodItem = 0} -- Crescent Sword

-- [3306] Black Market Equipment
BaoXiang_jsjqa = {}
BaoXiang_jsjqa[1 ] = {Active = 1,	ItemID = 0116,	Quantity = 1,	Quality = 24,	Rad = 1   ,		GoodItem = 0} -- Colossus
BaoXiang_jsjqa[2 ] = {Active = 1,	ItemID = 3306,	Quantity = 1,	Quality = 24,	Rad = 23  ,		GoodItem = 0} -- Black Market Equipment
BaoXiang_jsjqa[3 ] = {Active = 1,	ItemID = 1375,	Quantity = 1,	Quality = 24,	Rad = 23  ,		GoodItem = 0} -- Primal Axe of Rage
BaoXiang_jsjqa[4 ] = {Active = 1,	ItemID = 1384,	Quantity = 1,	Quality = 24,	Rad = 23  ,		GoodItem = 0} -- Winds of Death
BaoXiang_jsjqa[5 ] = {Active = 1,	ItemID = 0018,	Quantity = 1,	Quality = 24,	Rad = 23  ,		GoodItem = 0} -- Thunder Blade
BaoXiang_jsjqa[6 ] = {Active = 1,	ItemID = 1374,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Grey Sword of Darkan
BaoXiang_jsjqa[7 ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Bone Sword
BaoXiang_jsjqa[8 ] = {Active = 1,	ItemID = 1383,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Soul Sword of Darkan
BaoXiang_jsjqa[9 ] = {Active = 1,	ItemID = 0021,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Charging Sword
BaoXiang_jsjqa[10] = {Active = 1,	ItemID = 1378,	Quantity = 1,	Quality = 24,	Rad = 180 ,		GoodItem = 0} -- Fiery Sword of Darkan
BaoXiang_jsjqa[11] = {Active = 1,	ItemID = 0020,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Rebel Sword
BaoXiang_jsjqa[12] = {Active = 1,	ItemID = 1372,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Righteous Sword
BaoXiang_jsjqa[13] = {Active = 1,	ItemID = 1373,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Cavalier Saber
BaoXiang_jsjqa[14] = {Active = 1,	ItemID = 1377,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Common Rebel Sword
BaoXiang_jsjqa[15] = {Active = 1,	ItemID = 1381,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Protector Sword
BaoXiang_jsjqa[16] = {Active = 1,	ItemID = 1382,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Invader Sword
BaoXiang_jsjqa[17] = {Active = 1,	ItemID = 1386,	Quantity = 1,	Quality = 24,	Rad = 1300,		GoodItem = 0} -- Enhanced Rebel Sword

-- [3307] Black Market Equipment
BaoXiang_jsjqb = {}
BaoXiang_jsjqb[1 ] = {Active = 1,	ItemID = 0116,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Colossus
BaoXiang_jsjqb[2 ] = {Active = 1,	ItemID = 3307,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jsjqb[3 ] = {Active = 1,	ItemID = 1375,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Primal Axe of Rage
BaoXiang_jsjqb[4 ] = {Active = 1,	ItemID = 1384,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Winds of Death
BaoXiang_jsjqb[5 ] = {Active = 1,	ItemID = 0018,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Thunder Blade
BaoXiang_jsjqb[6 ] = {Active = 1,	ItemID = 1374,	Quantity = 1,	Quality = 23,	Rad = 155  ,	GoodItem = 0} -- Grey Sword of Darkan
BaoXiang_jsjqb[7 ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 23,	Rad = 155  ,	GoodItem = 0} -- Bone Sword
BaoXiang_jsjqb[8 ] = {Active = 1,	ItemID = 1383,	Quantity = 1,	Quality = 23,	Rad = 155  ,	GoodItem = 0} -- Soul Sword of Darkan
BaoXiang_jsjqb[9 ] = {Active = 1,	ItemID = 0021,	Quantity = 1,	Quality = 23,	Rad = 155  ,	GoodItem = 0} -- Charging Sword
BaoXiang_jsjqb[10] = {Active = 1,	ItemID = 1378,	Quantity = 1,	Quality = 23,	Rad = 155  ,	GoodItem = 0} -- Fiery Sword of Darkan
BaoXiang_jsjqb[11] = {Active = 1,	ItemID = 0020,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Rebel Sword
BaoXiang_jsjqb[12] = {Active = 1,	ItemID = 1372,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Righteous Sword
BaoXiang_jsjqb[13] = {Active = 1,	ItemID = 1373,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Cavalier Saber
BaoXiang_jsjqb[14] = {Active = 1,	ItemID = 1377,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Common Rebel Sword
BaoXiang_jsjqb[15] = {Active = 1,	ItemID = 1381,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Protector Sword
BaoXiang_jsjqb[16] = {Active = 1,	ItemID = 1382,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Invader Sword
BaoXiang_jsjqb[17] = {Active = 1,	ItemID = 1386,	Quantity = 1,	Quality = 23,	Rad = 14000,	GoodItem = 0} -- Enhanced Rebel Sword

-- [3308] Black Market Equipment, [3309] Black Market Equipment
BaoXiang_jsmzcja = {}
BaoXiang_jsmzcja[1 ] = {Active = 1,	ItemID = 1892,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Greatsword of Incantation
BaoXiang_jsmzcja[2 ] = {Active = 1,	ItemID = 1893,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Roar of Evanescence
BaoXiang_jsmzcja[3 ] = {Active = 1,	ItemID = 1894,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Judgment of Enigma
BaoXiang_jsmzcja[4 ] = {Active = 1,	ItemID = 3308,	Quantity = 1,	Quality = 24,	Rad = 23   ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jsmzcja[5 ] = {Active = 1,	ItemID = 1375,	Quantity = 1,	Quality = 24,	Rad = 23   ,	GoodItem = 0} -- Primal Axe of Rage
BaoXiang_jsmzcja[6 ] = {Active = 1,	ItemID = 1384,	Quantity = 1,	Quality = 24,	Rad = 23   ,	GoodItem = 0} -- Winds of Death
BaoXiang_jsmzcja[7 ] = {Active = 1,	ItemID = 0018,	Quantity = 1,	Quality = 24,	Rad = 23   ,	GoodItem = 0} -- Thunder Blade
BaoXiang_jsmzcja[8 ] = {Active = 1,	ItemID = 1374,	Quantity = 1,	Quality = 24,	Rad = 180  ,	GoodItem = 0} -- Grey Sword of Darkan
BaoXiang_jsmzcja[9 ] = {Active = 1,	ItemID = 0017,	Quantity = 1,	Quality = 24,	Rad = 180  ,	GoodItem = 0} -- Bone Sword
BaoXiang_jsmzcja[10] = {Active = 1,	ItemID = 1383,	Quantity = 1,	Quality = 24,	Rad = 180  ,	GoodItem = 0} -- Soul Sword of Darkan
BaoXiang_jsmzcja[11] = {Active = 1,	ItemID = 0021,	Quantity = 1,	Quality = 24,	Rad = 180  ,	GoodItem = 0} -- Charging Sword
BaoXiang_jsmzcja[12] = {Active = 1,	ItemID = 1378,	Quantity = 1,	Quality = 24,	Rad = 180  ,	GoodItem = 0} -- Fiery Sword of Darkan
BaoXiang_jsmzcja[13] = {Active = 1,	ItemID = 0020,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Rebel Sword
BaoXiang_jsmzcja[14] = {Active = 1,	ItemID = 1372,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Righteous Sword
BaoXiang_jsmzcja[15] = {Active = 1,	ItemID = 1373,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Cavalier Saber
BaoXiang_jsmzcja[16] = {Active = 1,	ItemID = 1377,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Common Rebel Sword
BaoXiang_jsmzcja[17] = {Active = 1,	ItemID = 1381,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Protector Sword
BaoXiang_jsmzcja[18] = {Active = 1,	ItemID = 1382,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Invader Sword
BaoXiang_jsmzcja[19] = {Active = 1,	ItemID = 1386,	Quantity = 1,	Quality = 24,	Rad = 13000,	GoodItem = 0} -- Enhanced Rebel Sword

-- [3310] Black Market Equipment
BaoXiang_jssjkja = {}
BaoXiang_jssjkja[1 ] = {Active = 1,	ItemID = 1884,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Armor of Revenant
BaoXiang_jssjkja[2 ] = {Active = 1,	ItemID = 1888,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Platemail of the Cursed Soul
BaoXiang_jssjkja[3 ] = {Active = 1,	ItemID = 1889,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Armor of Evanescence
BaoXiang_jssjkja[4 ] = {Active = 1,	ItemID = 1890,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Armor of Enigma
BaoXiang_jssjkja[5 ] = {Active = 1,	ItemID = 0396,	Quantity = 1,	Quality = 24,	Rad = 2000 ,	GoodItem = 0} -- Armor of Secrets
BaoXiang_jssjkja[6 ] = {Active = 1,	ItemID = 4150,	Quantity = 1,	Quality = 24,	Rad = 2000 ,	GoodItem = 0} -- Sagacious Battle Armor
BaoXiang_jssjkja[7 ] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_jssjkja[8 ] = {Active = 1,	ItemID = 1931,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Battle Armor of the Kylin
BaoXiang_jssjkja[9 ] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 24,	Rad = 22400,	GoodItem = 0} -- Light Platemail
BaoXiang_jssjkja[10] = {Active = 1,	ItemID = 0303,	Quantity = 1,	Quality = 24,	Rad = 22400,	GoodItem = 0} -- Silver Platemail
BaoXiang_jssjkja[11] = {Active = 1,	ItemID = 1929,	Quantity = 1,	Quality = 24,	Rad = 22400,	GoodItem = 0} -- Battle Armor of the Tempest
BaoXiang_jssjkja[12] = {Active = 1,	ItemID = 1930,	Quantity = 1,	Quality = 24,	Rad = 22400,	GoodItem = 0} -- Battle Armor of Sistine

-- [3311] Black Market Equipment
BaoXiang_jssjkjb = {}
BaoXiang_jssjkjb[1 ] = {Active = 1,	ItemID = 1884,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Armor of Revenant
BaoXiang_jssjkjb[2 ] = {Active = 1,	ItemID = 1888,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Platemail of the Cursed Soul
BaoXiang_jssjkjb[3 ] = {Active = 1,	ItemID = 1889,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Armor of Evanescence
BaoXiang_jssjkjb[4 ] = {Active = 1,	ItemID = 1890,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Armor of Enigma
BaoXiang_jssjkjb[5 ] = {Active = 1,	ItemID = 0396,	Quantity = 1,	Quality = 23,	Rad = 1000 ,	GoodItem = 0} -- Armor of Secrets
BaoXiang_jssjkjb[6 ] = {Active = 1,	ItemID = 4150,	Quantity = 1,	Quality = 23,	Rad = 1000 ,	GoodItem = 0} -- Sagacious Battle Armor
BaoXiang_jssjkjb[7 ] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_jssjkjb[8 ] = {Active = 1,	ItemID = 1931,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Battle Armor of the Kylin
BaoXiang_jssjkjb[9 ] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 23,	Rad = 23000,	GoodItem = 0} -- Light Platemail
BaoXiang_jssjkjb[10] = {Active = 1,	ItemID = 0303,	Quantity = 1,	Quality = 23,	Rad = 23000,	GoodItem = 0} -- Silver Platemail
BaoXiang_jssjkjb[11] = {Active = 1,	ItemID = 1929,	Quantity = 1,	Quality = 23,	Rad = 23000,	GoodItem = 0} -- Battle Armor of the Tempest
BaoXiang_jssjkjb[12] = {Active = 1,	ItemID = 1930,	Quantity = 1,	Quality = 23,	Rad = 23000,	GoodItem = 0} -- Battle Armor of Sistine

-- [3312] Black Market Equipment
BaoXiang_jszjkja = {}
BaoXiang_jszjkja[1 ] = {Active = 1,	ItemID = 1898,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of Death
BaoXiang_jszjkja[2 ] = {Active = 1,	ItemID = 1899,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Corset of Incantation
BaoXiang_jszjkja[3 ] = {Active = 1,	ItemID = 1900,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Coat of Evanescence
BaoXiang_jszjkja[4 ] = {Active = 1,	ItemID = 1901,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Mantle of Enigma
BaoXiang_jszjkja[5 ] = {Active = 1,	ItemID = 3312,	Quantity = 1,	Quality = 24,	Rad = 1330 ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jszjkja[6 ] = {Active = 1,	ItemID = 0400,	Quantity = 1,	Quality = 24,	Rad = 1330 ,	GoodItem = 0} -- Vest of Apollo
BaoXiang_jszjkja[7 ] = {Active = 1,	ItemID = 4152,	Quantity = 1,	Quality = 24,	Rad = 1330 ,	GoodItem = 0} -- Robe of Gallant
BaoXiang_jszjkja[8 ] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_jszjkja[9 ] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jszjkja[10] = {Active = 1,	ItemID = 4151,	Quantity = 1,	Quality = 24,	Rad = 5000 ,	GoodItem = 0} -- Robe of Perception
BaoXiang_jszjkja[11] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 24,	Rad = 5000 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jszjkja[12] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 24,	Rad = 40000,	GoodItem = 0} -- Slick Vest
BaoXiang_jszjkja[13] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 24,	Rad = 40000,	GoodItem = 0} -- Peacock Vest

-- [3313] Black Market Equipment
BaoXiang_jszjkjb = {}
BaoXiang_jszjkjb[1 ] = {Active = 1,	ItemID = 1898,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of Death
BaoXiang_jszjkjb[2 ] = {Active = 1,	ItemID = 1899,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Corset of Incantation
BaoXiang_jszjkjb[3 ] = {Active = 1,	ItemID = 1900,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Coat of Evanescence
BaoXiang_jszjkjb[4 ] = {Active = 1,	ItemID = 1901,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Mantle of Enigma
BaoXiang_jszjkjb[5 ] = {Active = 1,	ItemID = 3313,	Quantity = 1,	Quality = 23,	Rad = 700  ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jszjkjb[6 ] = {Active = 1,	ItemID = 0400,	Quantity = 1,	Quality = 23,	Rad = 700  ,	GoodItem = 0} -- Vest of Apollo
BaoXiang_jszjkjb[7 ] = {Active = 1,	ItemID = 4152,	Quantity = 1,	Quality = 23,	Rad = 700  ,	GoodItem = 0} -- Robe of Gallant
BaoXiang_jszjkjb[8 ] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_jszjkjb[9 ] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jszjkjb[10] = {Active = 1,	ItemID = 4151,	Quantity = 1,	Quality = 23,	Rad = 5000 ,	GoodItem = 0} -- Robe of Perception
BaoXiang_jszjkjb[11] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 23,	Rad = 5000 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jszjkjb[12] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 23,	Rad = 42000,	GoodItem = 0} -- Slick Vest
BaoXiang_jszjkjb[13] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 23,	Rad = 42000,	GoodItem = 0} -- Peacock Vest

-- [3314] Black Market Equipment
BaoXiang_jsszkja = {}
BaoXiang_jsszkja[1 ] = {Active = 1, 	ItemID = 1910,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of the Venom Witch
BaoXiang_jsszkja[2 ] = {Active = 1, 	ItemID = 1911,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Coat of Invocation
BaoXiang_jsszkja[3 ] = {Active = 1, 	ItemID = 1912,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Robe of the Arcane
BaoXiang_jsszkja[4 ] = {Active = 1, 	ItemID = 1913,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Robe of Enigma
BaoXiang_jsszkja[5 ] = {Active = 1, 	ItemID = 0406,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Faerie Robe
BaoXiang_jsszkja[6 ] = {Active = 1, 	ItemID = 4158,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Arcane Bunny Costume
BaoXiang_jsszkja[7 ] = {Active = 1, 	ItemID = 4159,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of Spirit
BaoXiang_jsszkja[8 ] = {Active = 1, 	ItemID = 4160,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Pique Bunny Costume
BaoXiang_jsszkja[9 ] = {Active = 1, 	ItemID = 4157,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of Melody
BaoXiang_jsszkja[10] = {Active = 1, 	ItemID = 0391,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Joyful Bunny Costume
BaoXiang_jsszkja[11] = {Active = 1, 	ItemID = 0392,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jsszkja[12] = {Active = 1, 	ItemID = 0393,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_jsszkja[13] = {Active = 1, 	ItemID = 0394,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Heavenly Vest
BaoXiang_jsszkja[14] = {Active = 1, 	ItemID = 1960,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Coat of Hope
BaoXiang_jsszkja[15] = {Active = 1, 	ItemID = 1961,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Coat of the Phoenix
BaoXiang_jsszkja[16] = {Active = 1, 	ItemID = 1959,	Quantity = 1,	Quality = 24,	Rad = 20000,	GoodItem = 0} -- Glory Coat
BaoXiang_jsszkja[17] = {Active = 1, 	ItemID = 0382,	Quantity = 1,	Quality = 24,	Rad = 20000,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_jsszkja[18] = {Active = 1, 	ItemID = 0388,	Quantity = 1,	Quality = 24,	Rad = 20000,	GoodItem = 0} -- Happy Bunny Costume
BaoXiang_jsszkja[19] = {Active = 1, 	ItemID = 0392,	Quantity = 1,	Quality = 24,	Rad = 20000,	GoodItem = 0} -- Bunny Baby Costume

-- [3315] Black Market Equipment
BaoXiang_jsszkjb = {}
BaoXiang_jsszkjb[1 ] = {Active = 1,	ItemID = 1910,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of the Venom Witch
BaoXiang_jsszkjb[2 ] = {Active = 1,	ItemID = 1911,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Coat of Invocation
BaoXiang_jsszkjb[3 ] = {Active = 1,	ItemID = 1912,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Robe of the Arcane
BaoXiang_jsszkjb[4 ] = {Active = 1,	ItemID = 1913,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Robe of Enigma
BaoXiang_jsszkjb[5 ] = {Active = 1,	ItemID = 0406,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Faerie Robe
BaoXiang_jsszkjb[6 ] = {Active = 1,	ItemID = 4158,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Arcane Bunny Costume
BaoXiang_jsszkjb[7 ] = {Active = 1,	ItemID = 4159,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of Spirit
BaoXiang_jsszkjb[8 ] = {Active = 1,	ItemID = 4160,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Pique Bunny Costume
BaoXiang_jsszkjb[9 ] = {Active = 1,	ItemID = 4157,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of Melody
BaoXiang_jsszkjb[10] = {Active = 1,	ItemID = 0391,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Joyful Bunny Costume
BaoXiang_jsszkjb[11] = {Active = 1,	ItemID = 0392,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jsszkjb[12] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_jsszkjb[13] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Heavenly Vest
BaoXiang_jsszkjb[14] = {Active = 1,	ItemID = 1960,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Coat of Hope
BaoXiang_jsszkjb[15] = {Active = 1,	ItemID = 1961,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Coat of the Phoenix
BaoXiang_jsszkjb[16] = {Active = 1,	ItemID = 1959,	Quantity = 1,	Quality = 23,	Rad = 21110,	GoodItem = 0} -- Glory Coat
BaoXiang_jsszkjb[17] = {Active = 1,	ItemID = 0382,	Quantity = 1,	Quality = 23,	Rad = 21110,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_jsszkjb[18] = {Active = 1,	ItemID = 0388,	Quantity = 1,	Quality = 23,	Rad = 21110,	GoodItem = 0} -- Happy Bunny Costume
BaoXiang_jsszkjb[19] = {Active = 1,	ItemID = 0392,	Quantity = 1,	Quality = 23,	Rad = 21110,	GoodItem = 0} -- Bunny Baby Costume

-- [3316] Black Market Equipment
BaoXiang_jsfykja = {}
BaoXiang_jsfykja[1 ] = {Active = 1,	ItemID = 1910,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of the Venom Witch
BaoXiang_jsfykja[2 ] = {Active = 1,	ItemID = 1917,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Robe of Abraxas
BaoXiang_jsfykja[3 ] = {Active = 1,	ItemID = 1918,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Robe of Malediction
BaoXiang_jsfykja[4 ] = {Active = 1,	ItemID = 1919,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Robe of the Sphinx
BaoXiang_jsfykja[5 ] = {Active = 1,	ItemID = 0402,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of the Sage
BaoXiang_jsfykja[6 ] = {Active = 1,	ItemID = 0404,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Mystic Panda Costume
BaoXiang_jsfykja[7 ] = {Active = 1,	ItemID = 4161,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of Vengence
BaoXiang_jsfykja[8 ] = {Active = 1,	ItemID = 4162,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Arcane Otter Costume
BaoXiang_jsfykja[9 ] = {Active = 1,	ItemID = 4163,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of Amercement
BaoXiang_jsfykja[10] = {Active = 1,	ItemID = 4164,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Pique Otter Costume
BaoXiang_jsfykja[11] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_jsfykja[12] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_jsfykja[13] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Healer Robe
BaoXiang_jsfykja[14] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Protector Robe
BaoXiang_jsfykja[15] = {Active = 1,	ItemID = 1957,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Coat of Udine
BaoXiang_jsfykja[16] = {Active = 1,	ItemID = 1958,	Quantity = 1,	Quality = 24,	Rad = 2500 ,	GoodItem = 0} -- Coat of the Peacock
BaoXiang_jsfykja[17] = {Active = 1,	ItemID = 1956,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Coat of Frozen Crescent
BaoXiang_jsfykja[18] = {Active = 1,	ItemID = 0385,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Otter Costume
BaoXiang_jsfykja[19] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Passage Robe
BaoXiang_jsfykja[20] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Garcon Robe
BaoXiang_jsfykja[21] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Hopperoo Costume

-- [3317] Black Market Equipment
BaoXiang_jsfykjb = {}
BaoXiang_jsfykjb[1 ] = {Active = 1,	ItemID = 1910,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Robe of the Venom Witch
BaoXiang_jsfykjb[2 ] = {Active = 1,	ItemID = 1917,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Robe of Abraxas
BaoXiang_jsfykjb[3 ] = {Active = 1,	ItemID = 1918,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Robe of Malediction
BaoXiang_jsfykjb[4 ] = {Active = 1,	ItemID = 1919,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Robe of the Sphinx
BaoXiang_jsfykjb[5 ] = {Active = 1,	ItemID = 0402,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of the Sage
BaoXiang_jsfykjb[6 ] = {Active = 1,	ItemID = 0404,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Mystic Panda Costume
BaoXiang_jsfykjb[7 ] = {Active = 1,	ItemID = 4161,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of Vengence
BaoXiang_jsfykjb[8 ] = {Active = 1,	ItemID = 4162,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Arcane Otter Costume
BaoXiang_jsfykjb[9 ] = {Active = 1,	ItemID = 4163,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of Amercement
BaoXiang_jsfykjb[10] = {Active = 1,	ItemID = 4164,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Pique Otter Costume
BaoXiang_jsfykjb[11] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_jsfykjb[12] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_jsfykjb[13] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Healer Robe
BaoXiang_jsfykjb[14] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Protector Robe
BaoXiang_jsfykjb[15] = {Active = 1,	ItemID = 1957,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Coat of Udine
BaoXiang_jsfykjb[16] = {Active = 1,	ItemID = 1958,	Quantity = 1,	Quality = 23,	Rad = 2000 ,	GoodItem = 0} -- Coat of the Peacock
BaoXiang_jsfykjb[17] = {Active = 1,	ItemID = 1956,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Coat of Frozen Crescent
BaoXiang_jsfykjb[18] = {Active = 1,	ItemID = 0385,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Otter Costume
BaoXiang_jsfykjb[19] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Passage Robe
BaoXiang_jsfykjb[20] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Garcon Robe
BaoXiang_jsfykjb[21] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Hopperoo Costume

-- [3318] Black Market Equipment
BaoXiang_jshhkja = {}
BaoXiang_jshhkja[1 ] = {Active = 1,	ItemID = 1924,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Mantle of the Naga
BaoXiang_jshhkja[2 ] = {Active = 1,	ItemID = 1925,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Mantle of the Cursed Flame
BaoXiang_jshhkja[3 ] = {Active = 1,	ItemID = 1926,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Cloak of Evanescence
BaoXiang_jshhkja[4 ] = {Active = 1,	ItemID = 1927,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Mantle of the Sphinx
BaoXiang_jshhkja[5 ] = {Active = 1,	ItemID = 0411,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Tsunami Robe
BaoXiang_jshhkja[6 ] = {Active = 1,	ItemID = 0413,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Dragon Lord Costume
BaoXiang_jshhkja[7 ] = {Active = 1,	ItemID = 4153,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Nautical Coat
BaoXiang_jshhkja[8 ] = {Active = 1,	ItemID = 4154,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Arcane Lobster Costume
BaoXiang_jshhkja[9 ] = {Active = 1,	ItemID = 4155,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Robe of Stride
BaoXiang_jshhkja[10] = {Active = 1,	ItemID = 4156,	Quantity = 1,	Quality = 24,	Rad = 700  ,	GoodItem = 0} -- Pique Lobster Costume
BaoXiang_jshhkja[11] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 24,	Rad = 3000 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_jshhkja[12] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 24,	Rad = 3000 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_jshhkja[13] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 24,	Rad = 3000 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_jshhkja[14] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 24,	Rad = 3000 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_jshhkja[15] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 24,	Rad = 3000 ,	GoodItem = 0} -- Wind Vest
BaoXiang_jshhkja[16] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Deckman Vest
BaoXiang_jshhkja[17] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Mastman Vest
BaoXiang_jshhkja[18] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Duckling Costume
BaoXiang_jshhkja[19] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Ducky Costume
BaoXiang_jshhkja[20] = {Active = 1,	ItemID = 1977,	Quantity = 1,	Quality = 24,	Rad = 16000,	GoodItem = 0} -- Coat of the Tempest

-- [3319] Black Market Equipment
BaoXiang_jshhkjb = {}
BaoXiang_jshhkjb[1 ] = {Active = 1,	ItemID = 1924,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Mantle of the Naga
BaoXiang_jshhkjb[2 ] = {Active = 1,	ItemID = 1925,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Mantle of the Cursed Flame
BaoXiang_jshhkjb[3 ] = {Active = 1,	ItemID = 1926,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Cloak of Evanescence
BaoXiang_jshhkjb[4 ] = {Active = 1,	ItemID = 1927,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Mantle of the Sphinx
BaoXiang_jshhkjb[5 ] = {Active = 1,	ItemID = 0411,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Tsunami Robe
BaoXiang_jshhkjb[6 ] = {Active = 1,	ItemID = 0413,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Dragon Lord Costume
BaoXiang_jshhkjb[7 ] = {Active = 1,	ItemID = 4153,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Nautical Coat
BaoXiang_jshhkjb[8 ] = {Active = 1,	ItemID = 4154,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Arcane Lobster Costume
BaoXiang_jshhkjb[9 ] = {Active = 1,	ItemID = 4155,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Robe of Stride
BaoXiang_jshhkjb[10] = {Active = 1,	ItemID = 4156,	Quantity = 1,	Quality = 23,	Rad = 400  ,	GoodItem = 0} -- Pique Lobster Costume
BaoXiang_jshhkjb[11] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_jshhkjb[12] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_jshhkjb[13] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_jshhkjb[14] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_jshhkjb[15] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 23,	Rad = 2500 ,	GoodItem = 0} -- Wind Vest
BaoXiang_jshhkjb[16] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Deckman Vest
BaoXiang_jshhkjb[17] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Mastman Vest
BaoXiang_jshhkjb[18] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Duckling Costume
BaoXiang_jshhkjb[19] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Ducky Costume
BaoXiang_jshhkjb[20] = {Active = 1,	ItemID = 1977,	Quantity = 1,	Quality = 23,	Rad = 17000,	GoodItem = 0} -- Coat of the Tempest

-- [3320] Black Market Equipment
BaoXiang_jsjjkja = {}
BaoXiang_jsjjkja[1] = {Active = 1,	ItemID = 1885,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Earth Sealed Tattoo of the Cursed Warrior
BaoXiang_jsjjkja[2] = {Active = 1,	ItemID = 1886,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Tattoo of Evanescence
BaoXiang_jsjjkja[3] = {Active = 1,	ItemID = 1887,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Ice Sealed Tattoo of Enigma
BaoXiang_jsjjkja[4] = {Active = 1,	ItemID = 0398,	Quantity = 1,	Quality = 24,	Rad = 1660 ,	GoodItem = 0} -- Armor of Olympus
BaoXiang_jsjjkja[5] = {Active = 1,	ItemID = 4148,	Quantity = 1,	Quality = 24,	Rad = 1660 ,	GoodItem = 0} -- Arcane Beast Tattoo
BaoXiang_jsjjkja[6] = {Active = 1,	ItemID = 4147,	Quantity = 1,	Quality = 24,	Rad = 1660 ,	GoodItem = 0} -- Spirit Beast Tattoo
BaoXiang_jsjjkja[7] = {Active = 1,	ItemID = 0228,	Quantity = 1,	Quality = 24,	Rad = 32000,	GoodItem = 0} -- Raging Bull Tattoo
BaoXiang_jsjjkja[8] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 24,	Rad = 32000,	GoodItem = 0} -- Primal Tattoo
BaoXiang_jsjjkja[9] = {Active = 1,	ItemID = 0229,	Quantity = 1,	Quality = 24,	Rad = 32000,	GoodItem = 0} -- Savage Bull Tattoo

-- [3321] Black Market Equipment
BaoXiang_jsjjkjb = {}
BaoXiang_jsjjkjb[1] = {Active = 1,	ItemID = 1885,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Earth Sealed Tattoo of the Cursed Warrior
BaoXiang_jsjjkjb[2] = {Active = 1,	ItemID = 1886,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Tattoo of Evanescence
BaoXiang_jsjjkjb[3] = {Active = 1,	ItemID = 1887,	Quantity = 1,	Quality = 23,	Rad = 5    ,	GoodItem = 0} -- Ice Sealed Tattoo of Enigma
BaoXiang_jsjjkjb[4] = {Active = 1,	ItemID = 0398,	Quantity = 1,	Quality = 23,	Rad = 900  ,	GoodItem = 0} -- Armor of Olympus
BaoXiang_jsjjkjb[5] = {Active = 1,	ItemID = 4148,	Quantity = 1,	Quality = 23,	Rad = 900  ,	GoodItem = 0} -- Arcane Beast Tattoo
BaoXiang_jsjjkjb[6] = {Active = 1,	ItemID = 4147,	Quantity = 1,	Quality = 23,	Rad = 900  ,	GoodItem = 0} -- Spirit Beast Tattoo
BaoXiang_jsjjkjb[7] = {Active = 1,	ItemID = 0228,	Quantity = 1,	Quality = 23,	Rad = 32500,	GoodItem = 0} -- Raging Bull Tattoo
BaoXiang_jsjjkjb[8] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 23,	Rad = 32500,	GoodItem = 0} -- Primal Tattoo
BaoXiang_jsjjkjb[9] = {Active = 1,	ItemID = 0229,	Quantity = 1,	Quality = 23,	Rad = 32500,	GoodItem = 0} -- Savage Bull Tattoo

--[3322] Black Market Equipment
BaoXiang_jshlza = {}
BaoXiang_jshlza[1 ] = {Active = 0,	ItemID = 0845,	Quantity = 1,	Quality = 24,	Rad = 1   ,	GoodItem = 0} -- Black Dragon Torso
BaoXiang_jshlza[2 ] = {Active = 1,	ItemID = 0398,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Armor of Olympus
BaoXiang_jshlza[3 ] = {Active = 1,	ItemID = 4148,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Arcane Beast Tattoo
BaoXiang_jshlza[4 ] = {Active = 1,	ItemID = 4147,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Spirit Beast Tattoo
BaoXiang_jshlza[5 ] = {Active = 1,	ItemID = 0411,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Tsunami Robe
BaoXiang_jshlza[6 ] = {Active = 1,	ItemID = 0413,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Dragon Lord Costume
BaoXiang_jshlza[7 ] = {Active = 1,	ItemID = 4153,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Nautical Coat
BaoXiang_jshlza[8 ] = {Active = 1,	ItemID = 4154,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Arcane Lobster Costume
BaoXiang_jshlza[9 ] = {Active = 1,	ItemID = 4155,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Stride
BaoXiang_jshlza[10] = {Active = 1,	ItemID = 4156,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Pique Lobster Costume
BaoXiang_jshlza[11] = {Active = 1,	ItemID = 0402,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of the Sage
BaoXiang_jshlza[12] = {Active = 1,	ItemID = 0404,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Mystic Panda Costume
BaoXiang_jshlza[13] = {Active = 1,	ItemID = 4161,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Vengence
BaoXiang_jshlza[14] = {Active = 1,	ItemID = 4162,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Arcane Otter Costume
BaoXiang_jshlza[15] = {Active = 1,	ItemID = 4163,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Amercement
BaoXiang_jshlza[16] = {Active = 1,	ItemID = 4164,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Pique Otter Costume
BaoXiang_jshlza[17] = {Active = 1,	ItemID = 0406,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Faerie Robe
BaoXiang_jshlza[18] = {Active = 1,	ItemID = 4158,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Arcane Bunny Costume
BaoXiang_jshlza[19] = {Active = 1,	ItemID = 4159,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Spirit
BaoXiang_jshlza[20] = {Active = 1,	ItemID = 4160,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Pique Bunny Costume
BaoXiang_jshlza[21] = {Active = 1,	ItemID = 4157,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Melody
BaoXiang_jshlza[22] = {Active = 1,	ItemID = 3322,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jshlza[23] = {Active = 1,	ItemID = 0400,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Vest of Apollo
BaoXiang_jshlza[24] = {Active = 1,	ItemID = 4152,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Robe of Gallant
BaoXiang_jshlza[25] = {Active = 1,	ItemID = 0396,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Armor of Secrets
BaoXiang_jshlza[26] = {Active = 1,	ItemID = 3322,	Quantity = 1,	Quality = 24,	Rad = 160 ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jshlza[27] = {Active = 1,	ItemID = 0228,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Raging Bull Tattoo
BaoXiang_jshlza[28] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Primal Tattoo
BaoXiang_jshlza[29] = {Active = 1,	ItemID = 0765,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Tattoo of Evanescence
BaoXiang_jshlza[30] = {Active = 1,	ItemID = 0357,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_jshlza[31] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_jshlza[32] = {Active = 1,	ItemID = 0343,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_jshlza[33] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_jshlza[34] = {Active = 1,	ItemID = 0345,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Wind Vest
BaoXiang_jshlza[35] = {Active = 1,	ItemID = 0363,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_jshlza[36] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_jshlza[37] = {Active = 1,	ItemID = 0376,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Healer Robe
BaoXiang_jshlza[38] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Protector Robe
BaoXiang_jshlza[39] = {Active = 1,	ItemID = 1957,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Coat of Udine
BaoXiang_jshlza[40] = {Active = 1,	ItemID = 1958,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Coat of the Peacock
BaoXiang_jshlza[41] = {Active = 1,	ItemID = 0391,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Joyful Bunny Costume
BaoXiang_jshlza[42] = {Active = 1,	ItemID = 0392,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jshlza[43] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_jshlza[44] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Heavenly Vest
BaoXiang_jshlza[45] = {Active = 1,	ItemID = 1960,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Coat of Hope
BaoXiang_jshlza[46] = {Active = 1,	ItemID = 1961,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Coat of the Phoenix
BaoXiang_jshlza[47] = {Active = 1,	ItemID = 0316,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_jshlza[48] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jshlza[49] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_jshlza[50] = {Active = 1,	ItemID = 1931,	Quantity = 1,	Quality = 24,	Rad = 500 ,	GoodItem = 0} -- Battle Armor of the Kylin
BaoXiang_jshlza[51] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Deckman Vest
BaoXiang_jshlza[52] = {Active = 1,	ItemID = 0342,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Mastman Vest
BaoXiang_jshlza[53] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Duckling Costume
BaoXiang_jshlza[54] = {Active = 1,	ItemID = 0356,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Ducky Costume
BaoXiang_jshlza[55] = {Active = 1,	ItemID = 1977,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Coat of the Tempest
BaoXiang_jshlza[56] = {Active = 1,	ItemID = 1956,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Coat of Frozen Crescent
BaoXiang_jshlza[57] = {Active = 1,	ItemID = 0385,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Otter Costume
BaoXiang_jshlza[58] = {Active = 1,	ItemID = 0375,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Passage Robe
BaoXiang_jshlza[59] = {Active = 1,	ItemID = 0369,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Garcon Robe
BaoXiang_jshlza[60] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_jshlza[61] = {Active = 1,	ItemID = 1959,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Glory Coat
BaoXiang_jshlza[62] = {Active = 1,	ItemID = 0382,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_jshlza[63] = {Active = 1,	ItemID = 0388,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Happy Bunny Costume
BaoXiang_jshlza[64] = {Active = 1,	ItemID = 0392,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jshlza[65] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Slick Vest
BaoXiang_jshlza[66] = {Active = 1,	ItemID = 0315,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Peacock Vest
BaoXiang_jshlza[67] = {Active = 1,	ItemID = 0302,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Light Platemail
BaoXiang_jshlza[68] = {Active = 1,	ItemID = 0303,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Silver Platemail
BaoXiang_jshlza[69] = {Active = 1,	ItemID = 1929,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Battle Armor of the Tempest
BaoXiang_jshlza[70] = {Active = 1,	ItemID = 1930,	Quantity = 1,	Quality = 24,	Rad = 4200,	GoodItem = 0} -- Battle Armor of Sistine

-- [3323] Black Market Equipment
BaoXiang_jshlzb = {}
BaoXiang_jshlzb[1 ] = {Active = 0,	ItemID = 0845,	Quantity = 1, 	Quality = 23,	Rad = 1	  ,	GoodItem = 0} -- Black Dragon Torso
BaoXiang_jshlzb[2 ] = {Active = 1,	ItemID = 0398,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Armor of Olympus
BaoXiang_jshlzb[3 ] = {Active = 1,	ItemID = 4148,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Arcane Beast Tattoo
BaoXiang_jshlzb[4 ] = {Active = 1,	ItemID = 4147,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Spirit Beast Tattoo
BaoXiang_jshlzb[5 ] = {Active = 1,	ItemID = 0411,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Tsunami Robe
BaoXiang_jshlzb[6 ] = {Active = 1,	ItemID = 0413,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Dragon Lord Costume
BaoXiang_jshlzb[7 ] = {Active = 1,	ItemID = 4153,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Nautical Coat
BaoXiang_jshlzb[8 ] = {Active = 1,	ItemID = 4154,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Arcane Lobster Costume
BaoXiang_jshlzb[9 ] = {Active = 1,	ItemID = 4155,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Stride
BaoXiang_jshlzb[10] = {Active = 1,	ItemID = 4156,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Pique Lobster Costume
BaoXiang_jshlzb[11] = {Active = 1,	ItemID = 0402,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of the Sage
BaoXiang_jshlzb[12] = {Active = 1,	ItemID = 0404,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Mystic Panda Costume
BaoXiang_jshlzb[13] = {Active = 1,	ItemID = 4161,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Vengence
BaoXiang_jshlzb[14] = {Active = 1,	ItemID = 4162,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Arcane Otter Costume
BaoXiang_jshlzb[15] = {Active = 1,	ItemID = 4163,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Amercement
BaoXiang_jshlzb[16] = {Active = 1,	ItemID = 4164,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Pique Otter Costume
BaoXiang_jshlzb[17] = {Active = 1,	ItemID = 0406,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Faerie Robe
BaoXiang_jshlzb[18] = {Active = 1,	ItemID = 4158,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Arcane Bunny Costume
BaoXiang_jshlzb[19] = {Active = 1,	ItemID = 4159,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Spirit
BaoXiang_jshlzb[20] = {Active = 1,	ItemID = 4160,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Pique Bunny Costume
BaoXiang_jshlzb[21] = {Active = 1,	ItemID = 4157,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Melody
BaoXiang_jshlzb[22] = {Active = 1,	ItemID = 3323,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jshlzb[23] = {Active = 1,	ItemID = 0400,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Vest of Apollo
BaoXiang_jshlzb[24] = {Active = 1,	ItemID = 4152,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Robe of Gallant
BaoXiang_jshlzb[25] = {Active = 1,	ItemID = 0396,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Armor of Secrets
BaoXiang_jshlzb[26] = {Active = 1,	ItemID = 3323,	Quantity = 1, 	Quality = 23,	Rad = 100 ,	GoodItem = 0} -- Black Market Equipment
BaoXiang_jshlzb[27] = {Active = 1,	ItemID = 0228,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Raging Bull Tattoo
BaoXiang_jshlzb[28] = {Active = 1,	ItemID = 0230,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Primal Tattoo
BaoXiang_jshlzb[29] = {Active = 1,	ItemID = 0765,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Tattoo of Evanescence
BaoXiang_jshlzb[30] = {Active = 1,	ItemID = 0357,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Prawn Costume
BaoXiang_jshlzb[31] = {Active = 1,	ItemID = 0358,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Pincer Costume
BaoXiang_jshlzb[32] = {Active = 1,	ItemID = 0343,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Hurricane Vest
BaoXiang_jshlzb[33] = {Active = 1,	ItemID = 0344,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Whirlpool Vest
BaoXiang_jshlzb[34] = {Active = 1,	ItemID = 0345,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Wind Vest
BaoXiang_jshlzb[35] = {Active = 1,	ItemID = 0363,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Clever Otter Costume
BaoXiang_jshlzb[36] = {Active = 1,	ItemID = 0364,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Lucky Otter Costume
BaoXiang_jshlzb[37] = {Active = 1,	ItemID = 0376,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Healer Robe
BaoXiang_jshlzb[38] = {Active = 1,	ItemID = 0377,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Protector Robe
BaoXiang_jshlzb[39] = {Active = 1,	ItemID = 1957,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Coat of Udine
BaoXiang_jshlzb[40] = {Active = 1,	ItemID = 1958,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Coat of the Peacock
BaoXiang_jshlzb[41] = {Active = 1,	ItemID = 0391,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Joyful Bunny Costume
BaoXiang_jshlzb[42] = {Active = 1,	ItemID = 0392,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jshlzb[43] = {Active = 1,	ItemID = 0393,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Lucky Bunny Costume
BaoXiang_jshlzb[44] = {Active = 1,	ItemID = 0394,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Heavenly Vest
BaoXiang_jshlzb[45] = {Active = 1,	ItemID = 1960,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Coat of Hope
BaoXiang_jshlzb[46] = {Active = 1,	ItemID = 1961,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Coat of the Phoenix
BaoXiang_jshlzb[47] = {Active = 1,	ItemID = 0316,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Ringdove Vest
BaoXiang_jshlzb[48] = {Active = 1,	ItemID = 0317,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Raptor Vest
BaoXiang_jshlzb[49] = {Active = 1,	ItemID = 0304,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Ceremonial Platemail
BaoXiang_jshlzb[50] = {Active = 1,	ItemID = 1931,	Quantity = 1, 	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Battle Armor of the Kylin
BaoXiang_jshlzb[51] = {Active = 1,	ItemID = 0341,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Deckman Vest
BaoXiang_jshlzb[52] = {Active = 1,	ItemID = 0342,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Mastman Vest
BaoXiang_jshlzb[53] = {Active = 1,	ItemID = 0353,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Duckling Costume
BaoXiang_jshlzb[54] = {Active = 1,	ItemID = 0356,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Ducky Costume
BaoXiang_jshlzb[55] = {Active = 1,	ItemID = 1977,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Coat of the Tempest
BaoXiang_jshlzb[56] = {Active = 1,	ItemID = 1956,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Coat of Frozen Crescent
BaoXiang_jshlzb[57] = {Active = 1,	ItemID = 0385,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Otter Costume
BaoXiang_jshlzb[58] = {Active = 1,	ItemID = 0375,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Passage Robe
BaoXiang_jshlzb[59] = {Active = 1,	ItemID = 0369,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Garcon Robe
BaoXiang_jshlzb[60] = {Active = 1,	ItemID = 0362,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Hopperoo Costume
BaoXiang_jshlzb[61] = {Active = 1,	ItemID = 1959,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Glory Coat
BaoXiang_jshlzb[62] = {Active = 1,	ItemID = 0382,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Loopy Bunny Costume
BaoXiang_jshlzb[63] = {Active = 1,	ItemID = 0388,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Happy Bunny Costume
BaoXiang_jshlzb[64] = {Active = 1,	ItemID = 0392,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Bunny Baby Costume
BaoXiang_jshlzb[65] = {Active = 1,	ItemID = 0314,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Slick Vest
BaoXiang_jshlzb[66] = {Active = 1,	ItemID = 0315,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Peacock Vest
BaoXiang_jshlzb[67] = {Active = 1,	ItemID = 0302,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Light Platemail
BaoXiang_jshlzb[68] = {Active = 1,	ItemID = 0303,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Silver Platemail
BaoXiang_jshlzb[69] = {Active = 1,	ItemID = 1929,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Battle Armor of the Tempest
BaoXiang_jshlzb[70] = {Active = 1,	ItemID = 1930,	Quantity = 1, 	Quality = 23,	Rad = 4400,	GoodItem = 0} -- Battle Armor of Sistine

-- [3324] Black Market Equipment
BaoXiang_jshlta = {}
BaoXiang_jshlta[1] = {Active = 1,	ItemID = 0848,	Quantity = 1,	Quality = 24,	Rad = 1    ,	GoodItem = 0} -- Black Dragon Helm
BaoXiang_jshlta[2] = {Active = 1,	ItemID = 2215,	Quantity = 1,	Quality = 24,	Rad = 1000 ,	GoodItem = 0} -- Lucky Bunny Cap
BaoXiang_jshlta[3] = {Active = 1,	ItemID = 2201,	Quantity = 1,	Quality = 24,	Rad = 1000 ,	GoodItem = 0} -- Lucky Otter Cap
BaoXiang_jshlta[4] = {Active = 1,	ItemID = 2200,	Quantity = 1,	Quality = 24,	Rad = 4000 ,	GoodItem = 0} -- Clever Otter Cap
BaoXiang_jshlta[5] = {Active = 1,	ItemID = 2213,	Quantity = 1,	Quality = 24,	Rad = 4000 ,	GoodItem = 0} -- Joyful Bunny Cap
BaoXiang_jshlta[6] = {Active = 1,	ItemID = 2207,	Quantity = 1,	Quality = 24,	Rad = 22500,	GoodItem = 0} -- Otter Cap
BaoXiang_jshlta[7] = {Active = 1,	ItemID = 2210,	Quantity = 1,	Quality = 24,	Rad = 22500,	GoodItem = 0} -- Happy Bunny Cap
BaoXiang_jshlta[8] = {Active = 1,	ItemID = 2214,	Quantity = 1,	Quality = 24,	Rad = 22500,	GoodItem = 0} -- Bunny Baby Cap
BaoXiang_jshlta[9] = {Active = 1,	ItemID = 2212,	Quantity = 1,	Quality = 24,	Rad = 22500,	GoodItem = 0} -- Kangaroo Cap

-- [3325] Black Market Equipment
BaoXiang_jshlsa = {}
BaoXiang_jshlsa[1 ] = {Active = 0,	ItemID = 0846,	Quantity = 1,	Quality = 24,	Rad = 1   ,	GoodItem = 0} -- Black Dragon Claw
BaoXiang_jshlsa[2 ] = {Active = 1,	ItemID = 1988,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Gauntlets of Evanescence
BaoXiang_jshlsa[3 ] = {Active = 1,	ItemID = 1990,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Gloves of Evanescence
BaoXiang_jshlsa[4 ] = {Active = 1,	ItemID = 1992,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Gloves of Malediction
BaoXiang_jshlsa[5 ] = {Active = 1,	ItemID = 1993,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Gloves of the Arcane
BaoXiang_jshlsa[6 ] = {Active = 1,	ItemID = 1996,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Heavy Gloves of Evanescence
BaoXiang_jshlsa[7 ] = {Active = 1,	ItemID = 0588,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Gloves of Secrets
BaoXiang_jshlsa[8 ] = {Active = 1,	ItemID = 0590,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Gloves of Apollo
BaoXiang_jshlsa[9 ] = {Active = 1,	ItemID = 0592,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Gloves of the Sage
BaoXiang_jshlsa[10] = {Active = 1,	ItemID = 0594,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Mystic Panda Gloves
BaoXiang_jshlsa[11] = {Active = 1,	ItemID = 0596,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Faerie Gloves
BaoXiang_jshlsa[12] = {Active = 1,	ItemID = 0598,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Fish Fairy Muffs
BaoXiang_jshlsa[13] = {Active = 1,	ItemID = 0600,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Tsunami Gloves
BaoXiang_jshlsa[14] = {Active = 1,	ItemID = 1938,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Gauntlets of the Kylin
BaoXiang_jshlsa[15] = {Active = 1,	ItemID = 1950,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Gloves of the White Tiger
BaoXiang_jshlsa[16] = {Active = 1,	ItemID = 1965,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Gloves of the Peacock
BaoXiang_jshlsa[17] = {Active = 1,	ItemID = 1968,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Gloves of the Phoenix
BaoXiang_jshlsa[18] = {Active = 1,	ItemID = 1983,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Gloves of the Green Dragon
BaoXiang_jshlsa[19] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Lucky Otter Muffs
BaoXiang_jshlsa[20] = {Active = 1,	ItemID = 0539,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Clever Otter Muffs
BaoXiang_jshlsa[21] = {Active = 1,	ItemID = 0555,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Blessed Gloves
BaoXiang_jshlsa[22] = {Active = 1,	ItemID = 0567,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Joyful Bunny Muffs
BaoXiang_jshlsa[23] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Lucky Bunny Muffs
BaoXiang_jshlsa[24] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 24,	Rad = 750 ,	GoodItem = 0} -- Heavenly Gloves
BaoXiang_jshlsa[25] = {Active = 1,	ItemID = 0545,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Garcon Gloves
BaoXiang_jshlsa[26] = {Active = 1,	ItemID = 0547,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Follower Gloves
BaoXiang_jshlsa[27] = {Active = 1,	ItemID = 0551,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Passage Gloves
BaoXiang_jshlsa[28] = {Active = 1,	ItemID = 0554,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Piety Gloves
BaoXiang_jshlsa[29] = {Active = 1,	ItemID = 0558,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Loopy Bunny Muffs
BaoXiang_jshlsa[30] = {Active = 1,	ItemID = 0561,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Otter Muffs
BaoXiang_jshlsa[31] = {Active = 1,	ItemID = 0564,	Quantity = 1,	Quality = 24,	Rad = 3400,	GoodItem = 0} -- Happy Bunny Muffs
BaoXiang_jshlsa[32] = {Active = 1,	ItemID = 0471,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Gauntlets
BaoXiang_jshlsa[33] = {Active = 1,	ItemID = 0476,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Chain Gauntlets
BaoXiang_jshlsa[34] = {Active = 1,	ItemID = 0486,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Feather Gloves
BaoXiang_jshlsa[35] = {Active = 1,	ItemID = 0517,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Deckman Gloves
BaoXiang_jshlsa[36] = {Active = 1,	ItemID = 0529,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Duckling Muffs
BaoXiang_jshlsa[37] = {Active = 1,	ItemID = 0543,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Travel Gloves
BaoXiang_jshlsa[38] = {Active = 1,	ItemID = 0566,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Kangaroo Muffs
BaoXiang_jshlsa[39] = {Active = 1,	ItemID = 1936,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Gauntlets of the Tempest
BaoXiang_jshlsa[40] = {Active = 1,	ItemID = 1948,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Flaming Gloves
BaoXiang_jshlsa[41] = {Active = 1,	ItemID = 1963,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Gloves of Frozen Crescent
BaoXiang_jshlsa[42] = {Active = 1,	ItemID = 1966,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Glory Gloves
BaoXiang_jshlsa[43] = {Active = 1,	ItemID = 1981,	Quantity = 1,	Quality = 24,	Rad = 5400,	GoodItem = 0} -- Gloves of the Tempest

-- [3326] Black Market Equipment
BaoXiang_jshlsb = {}
BaoXiang_jshlsb[1 ] = {Active = 0,	ItemID = 0846,	Quantity = 1,	Quality = 23,	Rad = 1	  ,	GoodItem = 0} -- Black Dragon Claw
BaoXiang_jshlsb[2 ] = {Active = 1,	ItemID = 1988,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fire Sealed Gauntlets of Evanescence
BaoXiang_jshlsb[3 ] = {Active = 1,	ItemID = 1990,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fire Sealed Gloves of Evanescence
BaoXiang_jshlsb[4 ] = {Active = 1,	ItemID = 1992,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fire Sealed Gloves of Malediction
BaoXiang_jshlsb[5 ] = {Active = 1,	ItemID = 1993,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fire Sealed Gloves of the Arcane
BaoXiang_jshlsb[6 ] = {Active = 1,	ItemID = 1996,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fire Sealed Heavy Gloves of Evanescence
BaoXiang_jshlsb[7 ] = {Active = 1,	ItemID = 0588,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Gloves of Secrets
BaoXiang_jshlsb[8 ] = {Active = 1,	ItemID = 0590,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Gloves of Apollo
BaoXiang_jshlsb[9 ] = {Active = 1,	ItemID = 0592,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Gloves of the Sage
BaoXiang_jshlsb[10] = {Active = 1,	ItemID = 0594,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Mystic Panda Gloves
BaoXiang_jshlsb[11] = {Active = 1,	ItemID = 0596,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Faerie Gloves
BaoXiang_jshlsb[12] = {Active = 1,	ItemID = 0598,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Fish Fairy Muffs
BaoXiang_jshlsb[13] = {Active = 1,	ItemID = 0600,	Quantity = 1,	Quality = 23,	Rad = 400 ,	GoodItem = 0} -- Tsunami Gloves
BaoXiang_jshlsb[14] = {Active = 1,	ItemID = 1938,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Gauntlets of the Kylin
BaoXiang_jshlsb[15] = {Active = 1,	ItemID = 1950,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Gloves of the White Tiger
BaoXiang_jshlsb[16] = {Active = 1,	ItemID = 1965,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Gloves of the Peacock
BaoXiang_jshlsb[17] = {Active = 1,	ItemID = 1968,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Gloves of the Phoenix
BaoXiang_jshlsb[18] = {Active = 1,	ItemID = 1983,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Gloves of the Green Dragon
BaoXiang_jshlsb[19] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Lucky Otter Muffs
BaoXiang_jshlsb[20] = {Active = 1,	ItemID = 0539,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Clever Otter Muffs
BaoXiang_jshlsb[21] = {Active = 1,	ItemID = 0555,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Blessed Gloves
BaoXiang_jshlsb[22] = {Active = 1,	ItemID = 0567,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Joyful Bunny Muffs
BaoXiang_jshlsb[23] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Lucky Bunny Muffs
BaoXiang_jshlsb[24] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 23,	Rad = 700 ,	GoodItem = 0} -- Heavenly Gloves
BaoXiang_jshlsb[25] = {Active = 1,	ItemID = 0545,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Garcon Gloves
BaoXiang_jshlsb[26] = {Active = 1,	ItemID = 0547,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Follower Gloves
BaoXiang_jshlsb[27] = {Active = 1,	ItemID = 0551,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Passage Gloves
BaoXiang_jshlsb[28] = {Active = 1,	ItemID = 0554,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Piety Gloves
BaoXiang_jshlsb[29] = {Active = 1,	ItemID = 0558,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Loopy Bunny Muffs
BaoXiang_jshlsb[30] = {Active = 1,	ItemID = 0561,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Otter Muffs
BaoXiang_jshlsb[31] = {Active = 1,	ItemID = 0564,	Quantity = 1,	Quality = 23,	Rad = 3000,	GoodItem = 0} -- Happy Bunny Muffs
BaoXiang_jshlsb[32] = {Active = 1,	ItemID = 0471,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Gauntlets
BaoXiang_jshlsb[33] = {Active = 1,	ItemID = 0476,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Chain Gauntlets
BaoXiang_jshlsb[34] = {Active = 1,	ItemID = 0486,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Feather Gloves
BaoXiang_jshlsb[35] = {Active = 1,	ItemID = 0517,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Deckman Gloves
BaoXiang_jshlsb[36] = {Active = 1,	ItemID = 0529,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Duckling Muffs
BaoXiang_jshlsb[37] = {Active = 1,	ItemID = 0543,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Travel Gloves
BaoXiang_jshlsb[38] = {Active = 1,	ItemID = 0566,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Kangaroo Muffs
BaoXiang_jshlsb[39] = {Active = 1,	ItemID = 1936,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Gauntlets of the Tempest
BaoXiang_jshlsb[40] = {Active = 1,	ItemID = 1948,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Flaming Gloves
BaoXiang_jshlsb[41] = {Active = 1,	ItemID = 1963,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Gloves of Frozen Crescent
BaoXiang_jshlsb[42] = {Active = 1,	ItemID = 1966,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Glory Gloves
BaoXiang_jshlsb[43] = {Active = 1,	ItemID = 1981,	Quantity = 1,	Quality = 23,	Rad = 6000,	GoodItem = 0} -- Gloves of the Tempest

-- [3327] Black Market Equipment
BaoXiang_jshlya = {}
BaoXiang_jshlya[1 ] = {Active = 0,	ItemID = 0847,	Quantity = 1,	Quality = 24,	Rad = 1	  ,	GoodItem = 0} -- Black Dragon Wing
BaoXiang_jshlya[2 ] = {Active = 1,	ItemID = 1989,	Quantity = 1,	Quality = 24,	Rad = 120 ,	GoodItem = 0} -- Fire Sealed Greaves of Evanescence
BaoXiang_jshlya[3 ] = {Active = 1,	ItemID = 1991,	Quantity = 1,	Quality = 24,	Rad = 120 ,	GoodItem = 0} -- Fire Sealed Shoes of Evanescence
BaoXiang_jshlya[4 ] = {Active = 1,	ItemID = 1994,	Quantity = 1,	Quality = 24,	Rad = 120 ,	GoodItem = 0} -- Fire Sealed Boots of Malediction
BaoXiang_jshlya[5 ] = {Active = 1,	ItemID = 1995,	Quantity = 1,	Quality = 24,	Rad = 120 ,	GoodItem = 0} -- Fire Sealed Boots of the of the Arcane
BaoXiang_jshlya[6 ] = {Active = 1,	ItemID = 1997,	Quantity = 1,	Quality = 24,	Rad = 120 ,	GoodItem = 0} -- Fire Sealed Boots of Evanescence
BaoXiang_jshlya[7 ] = {Active = 1,	ItemID = 0748,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Boots of Secrets
BaoXiang_jshlya[8 ] = {Active = 1,	ItemID = 0750,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Boots of Apollo
BaoXiang_jshlya[9 ] = {Active = 1,	ItemID = 0752,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Boots of the Sage
BaoXiang_jshlya[10] = {Active = 1,	ItemID = 0754,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Mystic Panda Shoes
BaoXiang_jshlya[11] = {Active = 1,	ItemID = 0756,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Faerie Shoes
BaoXiang_jshlya[12] = {Active = 1,	ItemID = 0758,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Fish Fairy Shoes
BaoXiang_jshlya[13] = {Active = 1,	ItemID = 0760,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Tsunami Shoes
BaoXiang_jshlya[14] = {Active = 1,	ItemID = 0830,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Greaves of Olympus
BaoXiang_jshlya[15] = {Active = 1,	ItemID = 0669,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Raptor Boots
BaoXiang_jshlya[16] = {Active = 1,	ItemID = 0696,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Whirlpool Boots
BaoXiang_jshlya[17] = {Active = 1,	ItemID = 0710,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Pincer Shoes
BaoXiang_jshlya[18] = {Active = 1,	ItemID = 0716,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Lucky Otter Shoes
BaoXiang_jshlya[19] = {Active = 1,	ItemID = 0729,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Protector Boots
BaoXiang_jshlya[20] = {Active = 1,	ItemID = 0746,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Heavenly Shoes
BaoXiang_jshlya[21] = {Active = 1,	ItemID = 1942,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Greaves of the Kylin
BaoXiang_jshlya[22] = {Active = 1,	ItemID = 1954,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Boots of the White Tiger
BaoXiang_jshlya[23] = {Active = 1,	ItemID = 1972,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Boots of the Peacock
BaoXiang_jshlya[24] = {Active = 1,	ItemID = 1975,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Boots of the Phoenix
BaoXiang_jshlya[25] = {Active = 1,	ItemID = 1987,	Quantity = 1,	Quality = 24,	Rad = 450 ,	GoodItem = 0} -- Boots of the Green Dragon
BaoXiang_jshlya[26] = {Active = 1,	ItemID = 0651,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Mithril Greaves
BaoXiang_jshlya[27] = {Active = 1,	ItemID = 0655,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Silver Greaves
BaoXiang_jshlya[28] = {Active = 1,	ItemID = 0668,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Ringdove Boots
BaoXiang_jshlya[29] = {Active = 1,	ItemID = 0695,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Hurricane Boots
BaoXiang_jshlya[30] = {Active = 1,	ItemID = 0707,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Lobster Shoes
BaoXiang_jshlya[31] = {Active = 1,	ItemID = 0709,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Prawn Shoes
BaoXiang_jshlya[32] = {Active = 1,	ItemID = 0715,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Clever Otter Shoes
BaoXiang_jshlya[33] = {Active = 1,	ItemID = 0728,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Healer Boots
BaoXiang_jshlya[34] = {Active = 1,	ItemID = 0731,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Blessed Boots
BaoXiang_jshlya[35] = {Active = 1,	ItemID = 0734,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Loopy Bunny Shoes
BaoXiang_jshlya[36] = {Active = 1,	ItemID = 0737,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Otter Shoes
BaoXiang_jshlya[37] = {Active = 1,	ItemID = 0743,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Joyful Bunny Shoes
BaoXiang_jshlya[38] = {Active = 1,	ItemID = 1941,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Greaves of Sistine
BaoXiang_jshlya[39] = {Active = 1,	ItemID = 1953,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Boots of Darwin
BaoXiang_jshlya[40] = {Active = 1,	ItemID = 1971,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Boots of Udine
BaoXiang_jshlya[41] = {Active = 1,	ItemID = 1974,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Boots of Hope
BaoXiang_jshlya[42] = {Active = 1,	ItemID = 1986,	Quantity = 1,	Quality = 24,	Rad = 900 ,	GoodItem = 0} -- Boots of Forlorn
BaoXiang_jshlya[43] = {Active = 1,	ItemID = 0652,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Chain Greaves
BaoXiang_jshlya[44] = {Active = 1,	ItemID = 0653,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Strong Greaves
BaoXiang_jshlya[45] = {Active = 1,	ItemID = 0654,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Light Greaves
BaoXiang_jshlya[46] = {Active = 1,	ItemID = 0662,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Feather Boots
BaoXiang_jshlya[47] = {Active = 1,	ItemID = 0667,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Peacock Boots
BaoXiang_jshlya[48] = {Active = 1,	ItemID = 0693,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Deckman Boots
BaoXiang_jshlya[49] = {Active = 1,	ItemID = 0694,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Mastman Boots
BaoXiang_jshlya[50] = {Active = 1,	ItemID = 0705,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Duckling Shoes
BaoXiang_jshlya[51] = {Active = 1,	ItemID = 0708,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Ducky Shoes
BaoXiang_jshlya[52] = {Active = 1,	ItemID = 0714,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Hopperoo Shoes
BaoXiang_jshlya[53] = {Active = 1,	ItemID = 0719,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Travel Boots
BaoXiang_jshlya[54] = {Active = 1,	ItemID = 0720,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Nurse Boots
BaoXiang_jshlya[55] = {Active = 1,	ItemID = 0722,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Holy Boots
BaoXiang_jshlya[56] = {Active = 1,	ItemID = 0727,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Passage Boots
BaoXiang_jshlya[57] = {Active = 1,	ItemID = 0730,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Piety Boots
BaoXiang_jshlya[58] = {Active = 1,	ItemID = 0740,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Happy Bunny Shoes
BaoXiang_jshlya[59] = {Active = 1,	ItemID = 0742,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Kangaroo Shoes
BaoXiang_jshlya[60] = {Active = 1,	ItemID = 0744,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Bunny Baby Shoes
BaoXiang_jshlya[61] = {Active = 1,	ItemID = 1940,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Greaves of the Tempest
BaoXiang_jshlya[62] = {Active = 1,	ItemID = 1952,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Flaming Boots
BaoXiang_jshlya[63] = {Active = 1,	ItemID = 1970,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Boots of Frozen Crescent
BaoXiang_jshlya[64] = {Active = 1,	ItemID = 1973,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Glory Boots
BaoXiang_jshlya[65] = {Active = 1,	ItemID = 1985,	Quantity = 1,	Quality = 24,	Rad = 3500,	GoodItem = 0} -- Boots of the Tempest

-- [3328] Black Market Equipment
BaoXiang_jshlyb = {}
BaoXiang_jshlyb[1 ] = {Active = 0, 	ItemID = 0847,	Quantity = 1,	Quality = 23,	Rad = 1	  ,	GoodItem = 0} -- Black Dragon Wing
BaoXiang_jshlyb[2 ] = {Active = 1, 	ItemID = 1989,	Quantity = 1,	Quality = 23,	Rad = 80  ,	GoodItem = 0} -- Fire Sealed Greaves of Evanescence
BaoXiang_jshlyb[3 ] = {Active = 1, 	ItemID = 1991,	Quantity = 1,	Quality = 23,	Rad = 80  ,	GoodItem = 0} -- Fire Sealed Shoes of Evanescence
BaoXiang_jshlyb[4 ] = {Active = 1, 	ItemID = 1994,	Quantity = 1,	Quality = 23,	Rad = 80  ,	GoodItem = 0} -- Fire Sealed Boots of Malediction
BaoXiang_jshlyb[5 ] = {Active = 1, 	ItemID = 1995,	Quantity = 1,	Quality = 23,	Rad = 80  ,	GoodItem = 0} -- Fire Sealed Boots of the of the Arcane
BaoXiang_jshlyb[6 ] = {Active = 1, 	ItemID = 1997,	Quantity = 1,	Quality = 23,	Rad = 80  ,	GoodItem = 0} -- Fire Sealed Boots of Evanescence
BaoXiang_jshlyb[7 ] = {Active = 1, 	ItemID = 0748,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Boots of Secrets
BaoXiang_jshlyb[8 ] = {Active = 1, 	ItemID = 0750,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Boots of Apollo
BaoXiang_jshlyb[9 ] = {Active = 1, 	ItemID = 0752,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Boots of the Sage
BaoXiang_jshlyb[10] = {Active = 1, 	ItemID = 0754,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Mystic Panda Shoes
BaoXiang_jshlyb[11] = {Active = 1, 	ItemID = 0756,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Faerie Shoes
BaoXiang_jshlyb[12] = {Active = 1, 	ItemID = 0758,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Fish Fairy Shoes
BaoXiang_jshlyb[13] = {Active = 1, 	ItemID = 0760,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Tsunami Shoes
BaoXiang_jshlyb[14] = {Active = 1, 	ItemID = 0830,	Quantity = 1,	Quality = 23,	Rad = 60  ,	GoodItem = 0} -- Greaves of Olympus
BaoXiang_jshlyb[15] = {Active = 1, 	ItemID = 0669,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Raptor Boots
BaoXiang_jshlyb[16] = {Active = 1, 	ItemID = 0696,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Whirlpool Boots
BaoXiang_jshlyb[17] = {Active = 1, 	ItemID = 0710,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Pincer Shoes
BaoXiang_jshlyb[18] = {Active = 1, 	ItemID = 0716,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Lucky Otter Shoes
BaoXiang_jshlyb[19] = {Active = 1, 	ItemID = 0729,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Protector Boots
BaoXiang_jshlyb[20] = {Active = 1, 	ItemID = 0746,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Heavenly Shoes
BaoXiang_jshlyb[21] = {Active = 1, 	ItemID = 1942,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Greaves of the Kylin
BaoXiang_jshlyb[22] = {Active = 1, 	ItemID = 1954,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Boots of the White Tiger
BaoXiang_jshlyb[23] = {Active = 1, 	ItemID = 1972,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Boots of the Peacock
BaoXiang_jshlyb[24] = {Active = 1, 	ItemID = 1975,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Boots of the Phoenix
BaoXiang_jshlyb[25] = {Active = 1, 	ItemID = 1987,	Quantity = 1,	Quality = 23,	Rad = 300 ,	GoodItem = 0} -- Boots of the Green Dragon
BaoXiang_jshlyb[26] = {Active = 1, 	ItemID = 0651,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Mithril Greaves
BaoXiang_jshlyb[27] = {Active = 1, 	ItemID = 0655,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Silver Greaves
BaoXiang_jshlyb[28] = {Active = 1, 	ItemID = 0668,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Ringdove Boots
BaoXiang_jshlyb[29] = {Active = 1, 	ItemID = 0695,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Hurricane Boots
BaoXiang_jshlyb[30] = {Active = 1, 	ItemID = 0707,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Lobster Shoes
BaoXiang_jshlyb[31] = {Active = 1, 	ItemID = 0709,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Prawn Shoes
BaoXiang_jshlyb[32] = {Active = 1, 	ItemID = 0715,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Clever Otter Shoes
BaoXiang_jshlyb[33] = {Active = 1, 	ItemID = 0728,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Healer Boots
BaoXiang_jshlyb[34] = {Active = 1, 	ItemID = 0731,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Blessed Boots
BaoXiang_jshlyb[35] = {Active = 1, 	ItemID = 0734,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Loopy Bunny Shoes
BaoXiang_jshlyb[36] = {Active = 1, 	ItemID = 0737,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Otter Shoes
BaoXiang_jshlyb[37] = {Active = 1, 	ItemID = 0743,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Joyful Bunny Shoes
BaoXiang_jshlyb[38] = {Active = 1, 	ItemID = 1941,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Greaves of Sistine
BaoXiang_jshlyb[39] = {Active = 1, 	ItemID = 1953,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Boots of Darwin
BaoXiang_jshlyb[40] = {Active = 1, 	ItemID = 1971,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Boots of Udine
BaoXiang_jshlyb[41] = {Active = 1, 	ItemID = 1974,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Boots of Hope
BaoXiang_jshlyb[42] = {Active = 1, 	ItemID = 1986,	Quantity = 1,	Quality = 23,	Rad = 900 ,	GoodItem = 0} -- Boots of Forlorn
BaoXiang_jshlyb[43] = {Active = 1, 	ItemID = 0652,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Chain Greaves
BaoXiang_jshlyb[44] = {Active = 1, 	ItemID = 0653,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Strong Greaves
BaoXiang_jshlyb[45] = {Active = 1, 	ItemID = 0654,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Light Greaves
BaoXiang_jshlyb[46] = {Active = 1, 	ItemID = 0662,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Feather Boots
BaoXiang_jshlyb[47] = {Active = 1, 	ItemID = 0667,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Peacock Boots
BaoXiang_jshlyb[48] = {Active = 1, 	ItemID = 0693,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Deckman Boots
BaoXiang_jshlyb[49] = {Active = 1, 	ItemID = 0694,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Mastman Boots
BaoXiang_jshlyb[50] = {Active = 1, 	ItemID = 0705,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Duckling Shoes
BaoXiang_jshlyb[51] = {Active = 1, 	ItemID = 0708,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Ducky Shoes
BaoXiang_jshlyb[52] = {Active = 1, 	ItemID = 0714,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Hopperoo Shoes
BaoXiang_jshlyb[53] = {Active = 1, 	ItemID = 0719,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Travel Boots
BaoXiang_jshlyb[54] = {Active = 1, 	ItemID = 0720,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Nurse Boots
BaoXiang_jshlyb[55] = {Active = 1, 	ItemID = 0722,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Holy Boots
BaoXiang_jshlyb[56] = {Active = 1, 	ItemID = 0727,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Passage Boots
BaoXiang_jshlyb[57] = {Active = 1, 	ItemID = 0730,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Piety Boots
BaoXiang_jshlyb[58] = {Active = 1, 	ItemID = 0740,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Happy Bunny Shoes
BaoXiang_jshlyb[59] = {Active = 1, 	ItemID = 0742,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Kangaroo Shoes
BaoXiang_jshlyb[60] = {Active = 1, 	ItemID = 0744,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Bunny Baby Shoes
BaoXiang_jshlyb[61] = {Active = 1, 	ItemID = 1940,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Greaves of the Tempest
BaoXiang_jshlyb[62] = {Active = 1, 	ItemID = 1952,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Flaming Boots
BaoXiang_jshlyb[63] = {Active = 1, 	ItemID = 1970,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Boots of Frozen Crescent
BaoXiang_jshlyb[64] = {Active = 1, 	ItemID = 1973,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Glory Boots
BaoXiang_jshlyb[65] = {Active = 1, 	ItemID = 1985,	Quantity = 1,	Quality = 23,	Rad = 3700,	GoodItem = 0} -- Boots of the Tempest

-- [3329] Black Market Equipment
BaoXiang_jsmhzca = {}
BaoXiang_jsmhzca[1 ] = {Active = 1,	ItemID = 1920,	Quantity = 1,	Quality = 24,	Rad = 10   ,	GoodItem = 0} -- Undead Sealed Tooth of Specter
BaoXiang_jsmhzca[2 ] = {Active = 1,	ItemID = 1921,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Tooth of the Cursed
BaoXiang_jsmhzca[3 ] = {Active = 1,	ItemID = 1922,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Tooth of Evanescence
BaoXiang_jsmhzca[4 ] = {Active = 1,	ItemID = 1923,	Quantity = 1,	Quality = 24,	Rad = 1	   ,	GoodItem = 0} -- Ice Sealed Kris of the Sphinx
BaoXiang_jsmhzca[5 ] = {Active = 1,	ItemID = 0151,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Riven Soul
BaoXiang_jsmhzca[6 ] = {Active = 1,	ItemID = 0079,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Trident of Poseidon
BaoXiang_jsmhzca[7 ] = {Active = 1,	ItemID = 0078,	Quantity = 1,	Quality = 24,	Rad = 100  ,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_jsmhzca[8 ] = {Active = 1,	ItemID = 1448,	Quantity = 1,	Quality = 24,	Rad = 100  ,	GoodItem = 0} -- Sovereign Kris
BaoXiang_jsmhzca[9 ] = {Active = 1,	ItemID = 0077,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Hyena Dagger
BaoXiang_jsmhzca[10] = {Active = 1,	ItemID = 0084,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Vampiric Kris
BaoXiang_jsmhzca[11] = {Active = 1,	ItemID = 1419,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Blade of Torment
BaoXiang_jsmhzca[12] = {Active = 1,	ItemID = 1426,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Nightmare Dagger
BaoXiang_jsmhzca[13] = {Active = 1,	ItemID = 1447,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Pirate Dagger
BaoXiang_jsmhzca[14] = {Active = 1,	ItemID = 1461,	Quantity = 1,	Quality = 24,	Rad = 110  ,	GoodItem = 0} -- Cursed Dagger
BaoXiang_jsmhzca[15] = {Active = 1,	ItemID = 0076,	Quantity = 1,	Quality = 24,	Rad = 18000,	GoodItem = 0} -- Moon Kris
BaoXiang_jsmhzca[16] = {Active = 1,	ItemID = 1418,	Quantity = 1,	Quality = 24,	Rad = 18000,	GoodItem = 0} -- Comet Spike
BaoXiang_jsmhzca[17] = {Active = 1,	ItemID = 1425,	Quantity = 1,	Quality = 24,	Rad = 18000,	GoodItem = 0} -- Quartz Dagger
BaoXiang_jsmhzca[18] = {Active = 1,	ItemID = 1446,	Quantity = 1,	Quality = 24,	Rad = 18000,	GoodItem = 0} -- Spike of Feint
BaoXiang_jsmhzca[19] = {Active = 1,	ItemID = 1460,	Quantity = 1,	Quality = 24,	Rad = 18000,	GoodItem = 0} -- Gemmed Dagger

-- [3330] Black Market Equipment
BaoXiang_jsmhzcb = {}
BaoXiang_jsmhzcb[1 ] = {Active = 1,	ItemID = 1920,	Quantity = 1,	Quality = 23,	Rad = 20   ,	GoodItem = 0} -- Undead Sealed Tooth of Specter
BaoXiang_jsmhzcb[2 ] = {Active = 1,	ItemID = 1921,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Tooth of the Cursed
BaoXiang_jsmhzcb[3 ] = {Active = 1,	ItemID = 1922,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Tooth of Evanescence
BaoXiang_jsmhzcb[4 ] = {Active = 1,	ItemID = 1923,	Quantity = 1,	Quality = 23,	Rad = 5	   ,	GoodItem = 0} -- Ice Sealed Kris of the Sphinx
BaoXiang_jsmhzcb[5 ] = {Active = 1,	ItemID = 0151,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Riven Soul
BaoXiang_jsmhzcb[6 ] = {Active = 1,	ItemID = 0079,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Trident of Poseidon
BaoXiang_jsmhzcb[7 ] = {Active = 1,	ItemID = 0078,	Quantity = 1,	Quality = 23,	Rad = 70   ,	GoodItem = 0} -- Dagger of Hydra
BaoXiang_jsmhzcb[8 ] = {Active = 1,	ItemID = 1448,	Quantity = 1,	Quality = 23,	Rad = 70   ,	GoodItem = 0} -- Sovereign Kris
BaoXiang_jsmhzcb[9 ] = {Active = 1,	ItemID = 0077,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Hyena Dagger
BaoXiang_jsmhzcb[10] = {Active = 1,	ItemID = 0084,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Vampiric Kris
BaoXiang_jsmhzcb[11] = {Active = 1,	ItemID = 1419,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Blade of Torment
BaoXiang_jsmhzcb[12] = {Active = 1,	ItemID = 1426,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Nightmare Dagger
BaoXiang_jsmhzcb[13] = {Active = 1,	ItemID = 1447,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Pirate Dagger
BaoXiang_jsmhzcb[14] = {Active = 1,	ItemID = 1461,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Cursed Dagger
BaoXiang_jsmhzcb[15] = {Active = 1,	ItemID = 0076,	Quantity = 1,	Quality = 23,	Rad = 18500,	GoodItem = 0} -- Moon Kris
BaoXiang_jsmhzcb[16] = {Active = 1,	ItemID = 1418,	Quantity = 1,	Quality = 23,	Rad = 18500,	GoodItem = 0} -- Comet Spike
BaoXiang_jsmhzcb[17] = {Active = 1,	ItemID = 1425,	Quantity = 1,	Quality = 23,	Rad = 18500,	GoodItem = 0} -- Quartz Dagger
BaoXiang_jsmhzcb[18] = {Active = 1,	ItemID = 1446,	Quantity = 1,	Quality = 23,	Rad = 18500,	GoodItem = 0} -- Spike of Feint
BaoXiang_jsmhzcb[19] = {Active = 1,	ItemID = 1460,	Quantity = 1,	Quality = 23,	Rad = 18500,	GoodItem = 0} -- Gemmed Dagger

-- [3331] Black Market Equipment
BaoXiang_jsmzfza = {}
BaoXiang_jsmzfza[1 ] = {Active = 1,	ItemID = 1906,	Quantity = 1,	Quality = 24,	Rad = 10  ,	GoodItem = 0} -- Undead Sealed Staff of the Avenger
BaoXiang_jsmzfza[2 ] = {Active = 1,	ItemID = 1914,	Quantity = 1,	Quality = 24,	Rad = 30  ,	GoodItem = 0} -- Earth Sealed Staff of Abraxas
BaoXiang_jsmzfza[3 ] = {Active = 1,	ItemID = 1915,	Quantity = 1,	Quality = 24,	Rad = 20  ,	GoodItem = 0} -- Fire Sealed Staff of Mirage
BaoXiang_jsmzfza[4 ] = {Active = 1,	ItemID = 1916,	Quantity = 1,	Quality = 24,	Rad = 1	  ,	GoodItem = 0} -- Ice Sealed Staff of the Sphinx
BaoXiang_jsmzfza[5 ] = {Active = 1,	ItemID = 0112,	Quantity = 1,	Quality = 24,	Rad = 50  ,	GoodItem = 0} -- Crimson Rod
BaoXiang_jsmzfza[6 ] = {Active = 1,	ItemID = 1439,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Staff of Wrath
BaoXiang_jsmzfza[7 ] = {Active = 1,	ItemID = 1474,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Staff of Sun
BaoXiang_jsmzfza[8 ] = {Active = 1,	ItemID = 3812,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Staff of the Peacock
BaoXiang_jsmzfza[9 ] = {Active = 1,	ItemID = 4300,	Quantity = 1,	Quality = 24,	Rad = 100 ,	GoodItem = 0} -- Staff of Amercement
BaoXiang_jsmzfza[10] = {Active = 1,	ItemID = 1432,	Quantity = 1,	Quality = 24,	Rad = 200 ,	GoodItem = 0} -- Staff of Thunderbolt
BaoXiang_jsmzfza[11] = {Active = 1,	ItemID = 1438,	Quantity = 1,	Quality = 24,	Rad = 200 ,	GoodItem = 0} -- Fury Staff
BaoXiang_jsmzfza[12] = {Active = 1,	ItemID = 1467,	Quantity = 1,	Quality = 24,	Rad = 200 ,	GoodItem = 0} -- Protector Staff
BaoXiang_jsmzfza[13] = {Active = 1,	ItemID = 1473,	Quantity = 1,	Quality = 24,	Rad = 200 ,	GoodItem = 0} -- Staff of Flame
BaoXiang_jsmzfza[14] = {Active = 1,	ItemID = 3811,	Quantity = 1,	Quality = 24,	Rad = 200 ,	GoodItem = 0} -- Staff of Udine
BaoXiang_jsmzfza[15] = {Active = 1,	ItemID = 1431,	Quantity = 1,	Quality = 24,	Rad = 2100,	GoodItem = 0} -- Feral Wand
BaoXiang_jsmzfza[16] = {Active = 1,	ItemID = 1437,	Quantity = 1,	Quality = 24,	Rad = 2100,	GoodItem = 0} -- Restraining Staff
BaoXiang_jsmzfza[17] = {Active = 1,	ItemID = 1466,	Quantity = 1,	Quality = 24,	Rad = 2100,	GoodItem = 0} -- Wand of Vigor
BaoXiang_jsmzfza[18] = {Active = 1,	ItemID = 1472,	Quantity = 1,	Quality = 24,	Rad = 2100,	GoodItem = 0} -- Staff of Endeavor

-- [3332] Black Market Equipment
BaoXiang_jsmzfzb = {}
BaoXiang_jsmzfzb[1 ] = {Active = 1,	ItemID = 1906,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Undead Sealed Staff of the Avenger
BaoXiang_jsmzfzb[2 ] = {Active = 1,	ItemID = 1914,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Staff of Abraxas
BaoXiang_jsmzfzb[3 ] = {Active = 1,	ItemID = 1915,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Staff of Mirage
BaoXiang_jsmzfzb[4 ] = {Active = 1,	ItemID = 1916,	Quantity = 1,	Quality = 23,	Rad = 5	   ,	GoodItem = 0} -- Ice Sealed Staff of the Sphinx
BaoXiang_jsmzfzb[5 ] = {Active = 1,	ItemID = 0112,	Quantity = 1,	Quality = 23,	Rad = 40   ,	GoodItem = 0} -- Crimson Rod
BaoXiang_jsmzfzb[6 ] = {Active = 1,	ItemID = 1439,	Quantity = 1,	Quality = 23,	Rad = 80   ,	GoodItem = 0} -- Staff of Wrath
BaoXiang_jsmzfzb[7 ] = {Active = 1,	ItemID = 1474,	Quantity = 1,	Quality = 23,	Rad = 80   ,	GoodItem = 0} -- Staff of Sun
BaoXiang_jsmzfzb[8 ] = {Active = 1,	ItemID = 3812,	Quantity = 1,	Quality = 23,	Rad = 80   ,	GoodItem = 0} -- Staff of the Peacock
BaoXiang_jsmzfzb[9 ] = {Active = 1,	ItemID = 4300,	Quantity = 1,	Quality = 23,	Rad = 80   ,	GoodItem = 0} -- Staff of Amercement
BaoXiang_jsmzfzb[10] = {Active = 1,	ItemID = 1432,	Quantity = 1,	Quality = 23,	Rad = 200  ,	GoodItem = 0} -- Staff of Thunderbolt
BaoXiang_jsmzfzb[11] = {Active = 1,	ItemID = 1438,	Quantity = 1,	Quality = 23,	Rad = 200  ,	GoodItem = 0} -- Fury Staff
BaoXiang_jsmzfzb[12] = {Active = 1,	ItemID = 1467,	Quantity = 1,	Quality = 23,	Rad = 200  ,	GoodItem = 0} -- Protector Staff
BaoXiang_jsmzfzb[13] = {Active = 1,	ItemID = 1473,	Quantity = 1,	Quality = 23,	Rad = 200  ,	GoodItem = 0} -- Staff of Flame
BaoXiang_jsmzfzb[14] = {Active = 1,	ItemID = 3811,	Quantity = 1,	Quality = 23,	Rad = 200  ,	GoodItem = 0} -- Staff of Udine
BaoXiang_jsmzfzb[15] = {Active = 1,	ItemID = 1431,	Quantity = 1,	Quality = 23,	Rad = 21500,	GoodItem = 0} -- Feral Wand
BaoXiang_jsmzfzb[16] = {Active = 1,	ItemID = 1437,	Quantity = 1,	Quality = 23,	Rad = 21500,	GoodItem = 0} -- Restraining Staff
BaoXiang_jsmzfzb[17] = {Active = 1,	ItemID = 1466,	Quantity = 1,	Quality = 23,	Rad = 21500,	GoodItem = 0} -- Wand of Vigor
BaoXiang_jsmzfzb[18] = {Active = 1,	ItemID = 1472,	Quantity = 1,	Quality = 23,	Rad = 21500,	GoodItem = 0} -- Staff of Endeavor

-- [3333] Black Market Equipment
BaoXiang_jsmfzza = {}
BaoXiang_jsmfzza[1 ] = {Active = 1,	ItemID = 1906,	Quantity = 1,	Quality = 24,	Rad = 100  ,	GoodItem = 0} -- Undead Sealed Staff of the Avenger
BaoXiang_jsmfzza[2 ] = {Active = 1,	ItemID = 1907,	Quantity = 1,	Quality = 24,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Staff of Incantation
BaoXiang_jsmfzza[3 ] = {Active = 1,	ItemID = 1908,	Quantity = 1,	Quality = 24,	Rad = 20   ,	GoodItem = 0} -- Fire Sealed Staff of Evanescence
BaoXiang_jsmfzza[4 ] = {Active = 1,	ItemID = 1909,	Quantity = 1,	Quality = 24,	Rad = 1	   ,	GoodItem = 0} -- Ice Sealed Staff of Enigma
BaoXiang_jsmfzza[5 ] = {Active = 1,	ItemID = 0110,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Revered Staff
BaoXiang_jsmfzza[6 ] = {Active = 1,	ItemID = 4198,	Quantity = 1,	Quality = 24,	Rad = 50   ,	GoodItem = 0} -- Soul Spring
BaoXiang_jsmfzza[7 ] = {Active = 1,	ItemID = 1442,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Tears of Angel
BaoXiang_jsmfzza[8 ] = {Active = 1,	ItemID = 1477,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Eye of Sorrow
BaoXiang_jsmfzza[9 ] = {Active = 1,	ItemID = 3815,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Staff of the Phoenix
BaoXiang_jsmfzza[10] = {Active = 1,	ItemID = 4197,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Flame of the Arctic
BaoXiang_jsmfzza[11] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Holy Guidance
BaoXiang_jsmfzza[12] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 24,	Rad = 60   ,	GoodItem = 0} -- Holy Guidance
BaoXiang_jsmfzza[13] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of Life
BaoXiang_jsmfzza[14] = {Active = 1,	ItemID = 1433,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Hearten Wand
BaoXiang_jsmfzza[15] = {Active = 1,	ItemID = 1441,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of Gaia
BaoXiang_jsmfzza[16] = {Active = 1,	ItemID = 1468,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of High Priest
BaoXiang_jsmfzza[17] = {Active = 1,	ItemID = 1476,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of Binding
BaoXiang_jsmfzza[18] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of Life
BaoXiang_jsmfzza[19] = {Active = 1,	ItemID = 3814,	Quantity = 1,	Quality = 24,	Rad = 130  ,	GoodItem = 0} -- Staff of Hope
BaoXiang_jsmfzza[20] = {Active = 1,	ItemID = 1430,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Wand of Holiness
BaoXiang_jsmfzza[21] = {Active = 1,	ItemID = 1437,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Restraining Staff
BaoXiang_jsmfzza[22] = {Active = 1,	ItemID = 1465,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Brilliance Wand
BaoXiang_jsmfzza[23] = {Active = 1,	ItemID = 1475,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Spiritual Staff
BaoXiang_jsmfzza[24] = {Active = 1,	ItemID = 3813,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Glory Sigil
BaoXiang_jsmfzza[25] = {Active = 1,	ItemID = 4301,	Quantity = 1,	Quality = 24,	Rad = 14500,	GoodItem = 0} -- Staff of Sagacious

-- [3334] Black Market Equipment
BaoXiang_jsmfzzb = {}
BaoXiang_jsmfzzb[1 ] = {Active = 1,	ItemID = 1906,	Quantity = 1,	Quality = 23,	Rad = 100  ,	GoodItem = 0} -- Undead Sealed Staff of the Avenger
BaoXiang_jsmfzzb[2 ] = {Active = 1,	ItemID = 1907,	Quantity = 1,	Quality = 23,	Rad = 30   ,	GoodItem = 0} -- Earth Sealed Staff of Incantation
BaoXiang_jsmfzzb[3 ] = {Active = 1,	ItemID = 1908,	Quantity = 1,	Quality = 23,	Rad = 10   ,	GoodItem = 0} -- Fire Sealed Staff of Evanescence
BaoXiang_jsmfzzb[4 ] = {Active = 1,	ItemID = 1909,	Quantity = 1,	Quality = 23,	Rad = 5	   ,	GoodItem = 0} -- Ice Sealed Staff of Enigma
BaoXiang_jsmfzzb[5 ] = {Active = 1,	ItemID = 0110,	Quantity = 1,	Quality = 23,	Rad = 40   ,	GoodItem = 0} -- Revered Staff
BaoXiang_jsmfzzb[6 ] = {Active = 1,	ItemID = 4198,	Quantity = 1,	Quality = 23,	Rad = 40   ,	GoodItem = 0} -- Soul Spring
BaoXiang_jsmfzzb[7 ] = {Active = 1,	ItemID = 1442,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Tears of Angel
BaoXiang_jsmfzzb[8 ] = {Active = 1,	ItemID = 1477,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Eye of Sorrow
BaoXiang_jsmfzzb[9 ] = {Active = 1,	ItemID = 3815,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Staff of the Phoenix
BaoXiang_jsmfzzb[10] = {Active = 1,	ItemID = 4197,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Flame of the Arctic
BaoXiang_jsmfzzb[11] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Holy Guidance
BaoXiang_jsmfzzb[12] = {Active = 1,	ItemID = 4303,	Quantity = 1,	Quality = 23,	Rad = 50   ,	GoodItem = 0} -- Holy Guidance
BaoXiang_jsmfzzb[13] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of Life
BaoXiang_jsmfzzb[14] = {Active = 1,	ItemID = 1433,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Hearten Wand
BaoXiang_jsmfzzb[15] = {Active = 1,	ItemID = 1441,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of Gaia
BaoXiang_jsmfzzb[16] = {Active = 1,	ItemID = 1468,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of High Priest
BaoXiang_jsmfzzb[17] = {Active = 1,	ItemID = 1476,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of Binding
BaoXiang_jsmfzzb[18] = {Active = 1,	ItemID = 0103,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of Life
BaoXiang_jsmfzzb[19] = {Active = 1,	ItemID = 3814,	Quantity = 1,	Quality = 23,	Rad = 130  ,	GoodItem = 0} -- Staff of Hope
BaoXiang_jsmfzzb[20] = {Active = 1,	ItemID = 1430,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Wand of Holiness
BaoXiang_jsmfzzb[21] = {Active = 1,	ItemID = 1437,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Restraining Staff
BaoXiang_jsmfzzb[22] = {Active = 1,	ItemID = 1465,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Brilliance Wand
BaoXiang_jsmfzzb[23] = {Active = 1,	ItemID = 1475,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Spiritual Staff
BaoXiang_jsmfzzb[24] = {Active = 1,	ItemID = 3813,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Glory Sigil
BaoXiang_jsmfzzb[25] = {Active = 1,	ItemID = 4301,	Quantity = 1,	Quality = 23,	Rad = 14500,	GoodItem = 0} -- Staff of Sagacious

-- [1093] Ancient Pirate Treasure Map
BaoXiang_CBTBOX = {}
BaoXiang_CBTBOX[1  ] = {Active = 1,	ItemID = 0396,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Armor of Secrets
BaoXiang_CBTBOX[2  ] = {Active = 1,	ItemID = 0398,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Armor of Olympus
BaoXiang_CBTBOX[3  ] = {Active = 1,	ItemID = 0400,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Vest of Apollo
BaoXiang_CBTBOX[4  ] = {Active = 1,	ItemID = 0402,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Robe of the Sage
BaoXiang_CBTBOX[5  ] = {Active = 1,	ItemID = 0404,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Mystic Panda Costume
BaoXiang_CBTBOX[6  ] = {Active = 1,	ItemID = 0406,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Faerie Robe
BaoXiang_CBTBOX[7  ] = {Active = 1,	ItemID = 0408,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Fish Fairy Costume
BaoXiang_CBTBOX[8  ] = {Active = 1,	ItemID = 0411,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Tsunami Robe
BaoXiang_CBTBOX[9  ] = {Active = 1,	ItemID = 0291,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Thick Armor
BaoXiang_CBTBOX[10 ] = {Active = 1,	ItemID = 0293,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Rhino Hide Armor
BaoXiang_CBTBOX[11 ] = {Active = 1,	ItemID = 0295,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Breast Plate
BaoXiang_CBTBOX[12 ] = {Active = 1,	ItemID = 0297,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavy Leather Armor
BaoXiang_CBTBOX[13 ] = {Active = 1,	ItemID = 0298,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Strong Leather Armor
BaoXiang_CBTBOX[14 ] = {Active = 1,	ItemID = 0300,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Chain Mail
BaoXiang_CBTBOX[15 ] = {Active = 1,	ItemID = 0306,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Canvas Vest
BaoXiang_CBTBOX[16 ] = {Active = 1,	ItemID = 0307,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Exquisite Vest
BaoXiang_CBTBOX[17 ] = {Active = 1,	ItemID = 0311,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Safari Vest
BaoXiang_CBTBOX[18 ] = {Active = 1,	ItemID = 0313,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Hunter Vest
BaoXiang_CBTBOX[19 ] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Slick Vest
BaoXiang_CBTBOX[20 ] = {Active = 1,	ItemID = 0336,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Tough Vest
BaoXiang_CBTBOX[21 ] = {Active = 1,	ItemID = 0337,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Explorer Vest
BaoXiang_CBTBOX[22 ] = {Active = 1,	ItemID = 0338,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Adventure Vest
BaoXiang_CBTBOX[23 ] = {Active = 1,	ItemID = 0339,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Helmsman Vest
BaoXiang_CBTBOX[24 ] = {Active = 1,	ItemID = 0340,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Oarsman Vest
BaoXiang_CBTBOX[25 ] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Deckman Vest
BaoXiang_CBTBOX[26 ] = {Active = 1,	ItemID = 0352,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Playful Racoon Costume
BaoXiang_CBTBOX[27 ] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Duckling Costume
BaoXiang_CBTBOX[28 ] = {Active = 1,	ItemID = 0354,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Big Crab Costume
BaoXiang_CBTBOX[29 ] = {Active = 1,	ItemID = 0350,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Crabby Costume
BaoXiang_CBTBOX[30 ] = {Active = 1,	ItemID = 0360,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Meowy Costume
BaoXiang_CBTBOX[31 ] = {Active = 1,	ItemID = 0361,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Owl Costume
BaoXiang_CBTBOX[32 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Hopperoo Costume
BaoXiang_CBTBOX[33 ] = {Active = 1,	ItemID = 0467,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Thick Gloves
BaoXiang_CBTBOX[34 ] = {Active = 1,	ItemID = 0469,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Rhino Hide Gloves
BaoXiang_CBTBOX[35 ] = {Active = 1,	ItemID = 0471,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gauntlets
BaoXiang_CBTBOX[36 ] = {Active = 1,	ItemID = 0473,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavy Leather Gloves
BaoXiang_CBTBOX[37 ] = {Active = 1,	ItemID = 0474,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Strong Leather Gloves
BaoXiang_CBTBOX[38 ] = {Active = 1,	ItemID = 0476,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Chain Gauntlets
BaoXiang_CBTBOX[39 ] = {Active = 1,	ItemID = 0482,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Canvas Gloves
BaoXiang_CBTBOX[40 ] = {Active = 1,	ItemID = 0483,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Exquisite Gloves
BaoXiang_CBTBOX[41 ] = {Active = 1,	ItemID = 0486,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Feather Gloves
BaoXiang_CBTBOX[42 ] = {Active = 1,	ItemID = 0487,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Safari Gloves
BaoXiang_CBTBOX[43 ] = {Active = 1,	ItemID = 0490,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Slick Gloves
BaoXiang_CBTBOX[44 ] = {Active = 1,	ItemID = 0513,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Explorer Gloves
BaoXiang_CBTBOX[45 ] = {Active = 1,	ItemID = 0514,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Adventure Gloves
BaoXiang_CBTBOX[46 ] = {Active = 1,	ItemID = 0515,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Helmsman Gloves
BaoXiang_CBTBOX[47 ] = {Active = 1,	ItemID = 0516,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Oarsman Gloves
BaoXiang_CBTBOX[48 ] = {Active = 1,	ItemID = 0517,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Deckman Gloves
BaoXiang_CBTBOX[49 ] = {Active = 1,	ItemID = 0536,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Meowy Muffs
BaoXiang_CBTBOX[50 ] = {Active = 1,	ItemID = 0537,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Owl Muffs
BaoXiang_CBTBOX[51 ] = {Active = 1,	ItemID = 0542,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Foster Gloves
BaoXiang_CBTBOX[52 ] = {Active = 1,	ItemID = 0543,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Travel Gloves
BaoXiang_CBTBOX[53 ] = {Active = 1,	ItemID = 0544,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Nurse Gloves
BaoXiang_CBTBOX[54 ] = {Active = 1,	ItemID = 0546,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Holy Gloves
BaoXiang_CBTBOX[55 ] = {Active = 1,	ItemID = 0549,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Scholar Gloves
BaoXiang_CBTBOX[56 ] = {Active = 1,	ItemID = 0550,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Emergency Gloves
BaoXiang_CBTBOX[57 ] = {Active = 1,	ItemID = 0557,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Kitty Muffs
BaoXiang_CBTBOX[58 ] = {Active = 1,	ItemID = 0562,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Racoon Muffs
BaoXiang_CBTBOX[59 ] = {Active = 1,	ItemID = 0565,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Night Owl Muffs
BaoXiang_CBTBOX[60 ] = {Active = 1,	ItemID = 0566,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Kangaroo Muffs
BaoXiang_CBTBOX[61 ] = {Active = 1,	ItemID = 0568,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Bunny Baby Muffs
BaoXiang_CBTBOX[62 ] = {Active = 1,	ItemID = 0649,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavy Leather Boots
BaoXiang_CBTBOX[63 ] = {Active = 1,	ItemID = 0650,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Strong Leather Boots
BaoXiang_CBTBOX[64 ] = {Active = 1,	ItemID = 0652,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Chain Greaves
BaoXiang_CBTBOX[65 ] = {Active = 1,	ItemID = 0658,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Canvas Boots
BaoXiang_CBTBOX[66 ] = {Active = 1,	ItemID = 0659,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Exquisite Boots
BaoXiang_CBTBOX[67 ] = {Active = 1,	ItemID = 0662,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Feather Boots
BaoXiang_CBTBOX[68 ] = {Active = 1,	ItemID = 0665,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Hunter Boots
BaoXiang_CBTBOX[69 ] = {Active = 1,	ItemID = 0666,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Slick Boots
BaoXiang_CBTBOX[70 ] = {Active = 1,	ItemID = 0689,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Explorer Boots
BaoXiang_CBTBOX[71 ] = {Active = 1,	ItemID = 0690,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Adventure Boots
BaoXiang_CBTBOX[72 ] = {Active = 1,	ItemID = 0691,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Helmsman Boots
BaoXiang_CBTBOX[73 ] = {Active = 1,	ItemID = 0692,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Oarsman Boots
BaoXiang_CBTBOX[74 ] = {Active = 1,	ItemID = 0693,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Deckman Boots
BaoXiang_CBTBOX[75 ] = {Active = 1,	ItemID = 0702,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Crabby Shoes
BaoXiang_CBTBOX[76 ] = {Active = 1,	ItemID = 0704,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Playful Racoon Shoes
BaoXiang_CBTBOX[77 ] = {Active = 1,	ItemID = 0705,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Duckling Shoes
BaoXiang_CBTBOX[78 ] = {Active = 1,	ItemID = 0706,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Big Crab Shoes
BaoXiang_CBTBOX[79 ] = {Active = 1,	ItemID = 0712,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Meowy Shoes
BaoXiang_CBTBOX[80 ] = {Active = 1,	ItemID = 0713,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Owl Shoes
BaoXiang_CBTBOX[81 ] = {Active = 1,	ItemID = 0718,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Foster Boots
BaoXiang_CBTBOX[82 ] = {Active = 1,	ItemID = 0719,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Travel Boots
BaoXiang_CBTBOX[83 ] = {Active = 1,	ItemID = 0720,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Nurse Boots
BaoXiang_CBTBOX[84 ] = {Active = 1,	ItemID = 0722,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Holy Boots
BaoXiang_CBTBOX[85 ] = {Active = 1,	ItemID = 0725,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Scholar Boots
BaoXiang_CBTBOX[86 ] = {Active = 1,	ItemID = 0726,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Emergency Boots
BaoXiang_CBTBOX[87 ] = {Active = 1,	ItemID = 0733,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Kitty Shoes
BaoXiang_CBTBOX[88 ] = {Active = 1,	ItemID = 0738,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Racoon Shoes
BaoXiang_CBTBOX[89 ] = {Active = 1,	ItemID = 0741,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Night Owl Shoes
BaoXiang_CBTBOX[90 ] = {Active = 1,	ItemID = 0742,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Kangaroo Shoes
BaoXiang_CBTBOX[91 ] = {Active = 1,	ItemID = 0744,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Bunny Baby Shoes
BaoXiang_CBTBOX[92 ] = {Active = 1,	ItemID = 0763,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Armor of Revenant
BaoXiang_CBTBOX[93 ] = {Active = 1,	ItemID = 0770,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Sword of Grief
BaoXiang_CBTBOX[94 ] = {Active = 1,	ItemID = 0777,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Robe of Death
BaoXiang_CBTBOX[95 ] = {Active = 1,	ItemID = 0781,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Touch of Death
BaoXiang_CBTBOX[96 ] = {Active = 1,	ItemID = 0785,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Staff of the Avenger
BaoXiang_CBTBOX[97 ] = {Active = 1,	ItemID = 0789,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Robe of the Venom Witch
BaoXiang_CBTBOX[98 ] = {Active = 1,	ItemID = 0799,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Tooth of Specter
BaoXiang_CBTBOX[99 ] = {Active = 1,	ItemID = 0803,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Mantle of the Naga
BaoXiang_CBTBOX[100] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Primal Tattoo
BaoXiang_CBTBOX[101] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Ceremonial Platemail
BaoXiang_CBTBOX[102] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Raptor Vest
BaoXiang_CBTBOX[103] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Whirlpool Vest
BaoXiang_CBTBOX[104] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Pincer Costume
BaoXiang_CBTBOX[105] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Otter Costume
BaoXiang_CBTBOX[106] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Protector Robe
BaoXiang_CBTBOX[107] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Bunny Costume
BaoXiang_CBTBOX[108] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavenly Vest
BaoXiang_CBTBOX[109] = {Active = 1,	ItemID = 0480,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Ceremonial Gauntlets
BaoXiang_CBTBOX[110] = {Active = 1,	ItemID = 0493,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Raptor Gloves
BaoXiang_CBTBOX[111] = {Active = 1,	ItemID = 0520,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Whirlpool Gloves
BaoXiang_CBTBOX[112] = {Active = 1,	ItemID = 0534,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Pincer Muffs
BaoXiang_CBTBOX[113] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Otter Muffs
BaoXiang_CBTBOX[114] = {Active = 1,	ItemID = 0553,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Protector Gloves
BaoXiang_CBTBOX[115] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Bunny Muffs
BaoXiang_CBTBOX[116] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavenly Gloves
BaoXiang_CBTBOX[117] = {Active = 1,	ItemID = 0656,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Ceremonial Greaves
BaoXiang_CBTBOX[118] = {Active = 1,	ItemID = 0669,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Raptor Boots
BaoXiang_CBTBOX[119] = {Active = 1,	ItemID = 0696,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Whirlpool Boots
BaoXiang_CBTBOX[120] = {Active = 1,	ItemID = 0710,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Pincer Shoes
BaoXiang_CBTBOX[121] = {Active = 1,	ItemID = 0716,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Otter Shoes
BaoXiang_CBTBOX[122] = {Active = 1,	ItemID = 0729,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Protector Boots
BaoXiang_CBTBOX[123] = {Active = 1,	ItemID = 0745,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lucky Bunny Shoes
BaoXiang_CBTBOX[124] = {Active = 1,	ItemID = 0746,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Heavenly Shoes
BaoXiang_CBTBOX[125] = {Active = 1,	ItemID = 3425,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Nal Runestone
BaoXiang_CBTBOX[126] = {Active = 1,	ItemID = 3426,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Sol Runestone
BaoXiang_CBTBOX[127] = {Active = 1,	ItemID = 3427,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Ja Runestone
BaoXiang_CBTBOX[128] = {Active = 1,	ItemID = 3428,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- El Runestone
BaoXiang_CBTBOX[129] = {Active = 1,	ItemID = 3429,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Cam Runestone
BaoXiang_CBTBOX[130] = {Active = 1,	ItemID = 3430,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Tef Runestone
BaoXiang_CBTBOX[131] = {Active = 1,	ItemID = 3431,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Yal Runestone
BaoXiang_CBTBOX[132] = {Active = 1,	ItemID = 3432,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Lum Runestone
BaoXiang_CBTBOX[133] = {Active = 1,	ItemID = 3433,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Fel Runestone
BaoXiang_CBTBOX[134] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Mystery Fruit
BaoXiang_CBTBOX[135] = {Active = 1,	ItemID = 3139,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Agrypnotic
BaoXiang_CBTBOX[136] = {Active = 1,	ItemID = 3136,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Snowy Soft Bud
BaoXiang_CBTBOX[137] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Magical Potion
BaoXiang_CBTBOX[138] = {Active = 1,	ItemID = 3123,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Red Date Tea
BaoXiang_CBTBOX[139] = {Active = 1,	ItemID = 3125,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Stramonium Fruit Juice
BaoXiang_CBTBOX[140] = {Active = 1,	ItemID = 3122,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Elven Fruit Juice
BaoXiang_CBTBOX[141] = {Active = 1,	ItemID = 3126,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Ice Cream
BaoXiang_CBTBOX[142] = {Active = 1,	ItemID = 3127,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Rainbow Fruit Juice
BaoXiang_CBTBOX[143] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Mystery Fruit
BaoXiang_CBTBOX[144] = {Active = 1,	ItemID = 3135,	Quantity = 1,	Quality = 5,	Rad = 15,	GoodItem = 0}	-- Special Ointment
BaoXiang_CBTBOX[145] = {Active = 1,	ItemID = 3828,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Thundoria Castle
BaoXiang_CBTBOX[146] = {Active = 1,	ItemID = 3829,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Thundoria Harbor
BaoXiang_CBTBOX[147] = {Active = 1,	ItemID = 3830,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Sacred Snow Mountain
BaoXiang_CBTBOX[148] = {Active = 1,	ItemID = 3831,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Andes Forest Haven
BaoXiang_CBTBOX[149] = {Active = 1,	ItemID = 3832,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket Oasis Haven
BaoXiang_CBTBOX[150] = {Active = 1,	ItemID = 3833,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Icespire Haven
BaoXiang_CBTBOX[151] = {Active = 1,	ItemID = 3834,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower
BaoXiang_CBTBOX[152] = {Active = 1,	ItemID = 3835,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Barren Cavern
BaoXiang_CBTBOX[153] = {Active = 1,	ItemID = 3836,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Abandoned Mine 2
BaoXiang_CBTBOX[154] = {Active = 1,	ItemID = 3837,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Silver Mine 2
BaoXiang_CBTBOX[155] = {Active = 1,	ItemID = 3838,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Silver Mine 3
BaoXiang_CBTBOX[156] = {Active = 1,	ItemID = 3839,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower 2
BaoXiang_CBTBOX[157] = {Active = 1,	ItemID = 3840,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower 3
BaoXiang_CBTBOX[158] = {Active = 1,	ItemID = 3841,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower 4
BaoXiang_CBTBOX[159] = {Active = 1,	ItemID = 3842,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower 5
BaoXiang_CBTBOX[160] = {Active = 1,	ItemID = 3843,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Ticket to Lone Tower 6
BaoXiang_CBTBOX[161] = {Active = 1,	ItemID = 0899,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Book of Strength Reset
BaoXiang_CBTBOX[162] = {Active = 1,	ItemID = 0900,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Book of Consitution Reset
BaoXiang_CBTBOX[163] = {Active = 1,	ItemID = 0901,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Book of Agility Reset
BaoXiang_CBTBOX[164] = {Active = 1,	ItemID = 0902,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Book of Accuracy Reset
BaoXiang_CBTBOX[165] = {Active = 1,	ItemID = 0903,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Book of Spirit Reset
BaoXiang_CBTBOX[166] = {Active = 1,	ItemID = 3846,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Voodoo Doll
BaoXiang_CBTBOX[167] = {Active = 1,	ItemID = 3462,	Quantity = 1,	Quality = 5,	Rad = 70,	GoodItem = 0}	-- Magical Clover
BaoXiang_CBTBOX[168] = {Active = 1,	ItemID = 0860,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gem of the Wind
BaoXiang_CBTBOX[169] = {Active = 1,	ItemID = 0861,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gem of Striking
BaoXiang_CBTBOX[170] = {Active = 1,	ItemID = 0862,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gem of Colossus
BaoXiang_CBTBOX[171] = {Active = 1,	ItemID = 0863,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gem of Rage
BaoXiang_CBTBOX[172] = {Active = 1,	ItemID = 1012,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Gem of Soul
BaoXiang_CBTBOX[173] = {Active = 1,	ItemID = 3463,	Quantity = 1,	Quality = 5,	Rad = 70,	GoodItem = 0}	-- Icy Crystal
BaoXiang_CBTBOX[174] = {Active = 1,	ItemID = 3844,	Quantity = 1,	Quality = 5,	Rad = 31,	GoodItem = 0}	-- Heaven's Berry
BaoXiang_CBTBOX[175] = {Active = 1,	ItemID = 3845,	Quantity = 1,	Quality = 5,	Rad = 31,	GoodItem = 0}	-- Charmed Berry
BaoXiang_CBTBOX[176] = {Active = 1,	ItemID = 0413,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Dragon Lord Costume
BaoXiang_CBTBOX[177] = {Active = 1,	ItemID = 0588,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Gloves of Secrets
BaoXiang_CBTBOX[178] = {Active = 1,	ItemID = 0590,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Gloves of Apollo
BaoXiang_CBTBOX[179] = {Active = 1,	ItemID = 0592,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Gloves of the Sage
BaoXiang_CBTBOX[180] = {Active = 1,	ItemID = 0594,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Mystic Panda Gloves
BaoXiang_CBTBOX[181] = {Active = 1,	ItemID = 0596,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Faerie Gloves
BaoXiang_CBTBOX[182] = {Active = 1,	ItemID = 0598,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Fish Fairy Muffs
BaoXiang_CBTBOX[183] = {Active = 1,	ItemID = 0600,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Tsunami Gloves
BaoXiang_CBTBOX[184] = {Active = 1,	ItemID = 0602,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Dragon Lord Muffs
BaoXiang_CBTBOX[185] = {Active = 1,	ItemID = 0604,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Gauntlets of Olympus
BaoXiang_CBTBOX[186] = {Active = 1,	ItemID = 0748,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Boots of Secrets
BaoXiang_CBTBOX[187] = {Active = 1,	ItemID = 0750,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Boots of Apollo
BaoXiang_CBTBOX[188] = {Active = 1,	ItemID = 0752,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Boots of the Sage
BaoXiang_CBTBOX[189] = {Active = 1,	ItemID = 0754,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Mystic Panda Shoes
BaoXiang_CBTBOX[190] = {Active = 1,	ItemID = 0756,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Faerie Shoes
BaoXiang_CBTBOX[191] = {Active = 1,	ItemID = 0758,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Fish Fairy Shoes
BaoXiang_CBTBOX[192] = {Active = 1,	ItemID = 0760,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Tsunami Shoes
BaoXiang_CBTBOX[193] = {Active = 1,	ItemID = 0824,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Dragon Lord Shoes
BaoXiang_CBTBOX[194] = {Active = 1,	ItemID = 0830,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Greaves of Olympus
BaoXiang_CBTBOX[195] = {Active = 1,	ItemID = 2219,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Mystic Panda Cap
BaoXiang_CBTBOX[196] = {Active = 1,	ItemID = 2221,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Fish Fairy Cap
BaoXiang_CBTBOX[197] = {Active = 1,	ItemID = 2223,	Quantity = 1,	Quality = 5,	Rad = 1 ,	GoodItem = 0}	-- Dragon Lord Cap
BaoXiang_CBTBOX[198] = {Active = 1,	ItemID = 0878,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Fiery Gem
BaoXiang_CBTBOX[199] = {Active = 1,	ItemID = 0879,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Furious Gem
BaoXiang_CBTBOX[200] = {Active = 1,	ItemID = 0880,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Explosive Gem
BaoXiang_CBTBOX[201] = {Active = 1,	ItemID = 0881,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Lustrious Gem
BaoXiang_CBTBOX[202] = {Active = 1,	ItemID = 0882,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Glowing Gem
BaoXiang_CBTBOX[203] = {Active = 1,	ItemID = 0883,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Shining Gem
BaoXiang_CBTBOX[204] = {Active = 1,	ItemID = 0884,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Shadow Gem
BaoXiang_CBTBOX[205] = {Active = 1,	ItemID = 0885,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Refining Gem
BaoXiang_CBTBOX[206] = {Active = 1,	ItemID = 0887,	Quantity = 1,	Quality = 5,	Rad = 8 ,	GoodItem = 0}	-- Spirit Gem
BaoXiang_CBTBOX[207] = {Active = 1,	ItemID = 0893,	Quantity = 1,	Quality = 5,	Rad = 20,	GoodItem = 0}	-- Potion of Monkey
BaoXiang_CBTBOX[208] = {Active = 1,	ItemID = 0894,	Quantity = 1,	Quality = 5,	Rad = 20,	GoodItem = 0}	-- Potion of Eagle
BaoXiang_CBTBOX[209] = {Active = 1,	ItemID = 0895,	Quantity = 1,	Quality = 5,	Rad = 20,	GoodItem = 0}	-- Potion of Bull
BaoXiang_CBTBOX[210] = {Active = 1,	ItemID = 0896,	Quantity = 1,	Quality = 5,	Rad = 20,	GoodItem = 0}	-- Potion of Soul
BaoXiang_CBTBOX[211] = {Active = 1,	ItemID = 0897,	Quantity = 1,	Quality = 5,	Rad = 20,	GoodItem = 0}	-- Potion of Lion
BaoXiang_CBTBOX[212] = {Active = 1,	ItemID = 0243,	Quantity = 1,	Quality = 5,	Rad = 10,	GoodItem = 0}	-- Novice Protection
BaoXiang_CBTBOX[213] = {Active = 1,	ItemID = 0244,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Standard Protection
BaoXiang_CBTBOX[214] = {Active = 1,	ItemID = 0246,	Quantity = 1,	Quality = 5,	Rad = 10,	GoodItem = 0}	-- Novice Berserk
BaoXiang_CBTBOX[215] = {Active = 1,	ItemID = 0247,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Standard Berserk
BaoXiang_CBTBOX[216] = {Active = 1,	ItemID = 0249,	Quantity = 1,	Quality = 5,	Rad = 10,	GoodItem = 0}	-- Novice Magic
BaoXiang_CBTBOX[217] = {Active = 1,	ItemID = 0250,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Standard Magic
BaoXiang_CBTBOX[218] = {Active = 1,	ItemID = 0252,	Quantity = 1,	Quality = 5,	Rad = 10,	GoodItem = 0}	-- Novice Recover
BaoXiang_CBTBOX[219] = {Active = 1,	ItemID = 0253,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Standard Recover
BaoXiang_CBTBOX[220] = {Active = 1,	ItemID = 0259,	Quantity = 1,	Quality = 5,	Rad = 10,	GoodItem = 0}	-- Novice Meditation
BaoXiang_CBTBOX[221] = {Active = 1,	ItemID = 0260,	Quantity = 1,	Quality = 5,	Rad = 5 ,	GoodItem = 0}	-- Standard Meditation
BaoXiang_CBTBOX[222] = {Active = 1,	ItemID = 0453,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Fusion Scroll
BaoXiang_CBTBOX[223] = {Active = 1,	ItemID = 0455,	Quantity = 1,	Quality = 5,	Rad = 25,	GoodItem = 0}	-- Strengthening Scroll

-- [0682] Caribbean Treasure Map
BaoXiang_JLBCBTBOX = {}
BaoXiang_JLBCBTBOX[1  ] = {Active = 1,	ItemID = 0291,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Thick Armor
BaoXiang_JLBCBTBOX[2  ] = {Active = 1,	ItemID = 0293,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Rhino Hide Armor
BaoXiang_JLBCBTBOX[3  ] = {Active = 1,	ItemID = 0295,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Breast Plate
BaoXiang_JLBCBTBOX[4  ] = {Active = 1,	ItemID = 0297,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Heavy Leather Armor
BaoXiang_JLBCBTBOX[5  ] = {Active = 1,	ItemID = 0298,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Strong Leather Armor
BaoXiang_JLBCBTBOX[6  ] = {Active = 1,	ItemID = 0300,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Chain Mail
BaoXiang_JLBCBTBOX[7  ] = {Active = 1,	ItemID = 0306,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Canvas Vest
BaoXiang_JLBCBTBOX[8  ] = {Active = 1,	ItemID = 0307,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Exquisite Vest
BaoXiang_JLBCBTBOX[9  ] = {Active = 1,	ItemID = 0311,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Safari Vest
BaoXiang_JLBCBTBOX[10 ] = {Active = 1,	ItemID = 0313,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Hunter Vest
BaoXiang_JLBCBTBOX[11 ] = {Active = 1,	ItemID = 0314,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Slick Vest
BaoXiang_JLBCBTBOX[12 ] = {Active = 1,	ItemID = 0336,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Tough Vest
BaoXiang_JLBCBTBOX[13 ] = {Active = 1,	ItemID = 0337,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Explorer Vest
BaoXiang_JLBCBTBOX[14 ] = {Active = 1,	ItemID = 0338,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Adventure Vest
BaoXiang_JLBCBTBOX[15 ] = {Active = 1,	ItemID = 0339,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Helmsman Vest
BaoXiang_JLBCBTBOX[16 ] = {Active = 1,	ItemID = 0340,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Oarsman Vest
BaoXiang_JLBCBTBOX[17 ] = {Active = 1,	ItemID = 0341,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Deckman Vest
BaoXiang_JLBCBTBOX[18 ] = {Active = 1,	ItemID = 0352,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Playful Racoon Costume
BaoXiang_JLBCBTBOX[19 ] = {Active = 1,	ItemID = 0353,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Duckling Costume
BaoXiang_JLBCBTBOX[20 ] = {Active = 1,	ItemID = 0354,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Big Crab Costume
BaoXiang_JLBCBTBOX[21 ] = {Active = 1,	ItemID = 0350,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Crabby Costume
BaoXiang_JLBCBTBOX[22 ] = {Active = 1,	ItemID = 0360,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Meowy Costume
BaoXiang_JLBCBTBOX[23 ] = {Active = 1,	ItemID = 0361,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Owl Costume
BaoXiang_JLBCBTBOX[24 ] = {Active = 1,	ItemID = 0362,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Hopperoo Costume
BaoXiang_JLBCBTBOX[25 ] = {Active = 1,	ItemID = 0467,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Thick Gloves
BaoXiang_JLBCBTBOX[26 ] = {Active = 1,	ItemID = 0469,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Rhino Hide Gloves
BaoXiang_JLBCBTBOX[27 ] = {Active = 1,	ItemID = 0471,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Gauntlets
BaoXiang_JLBCBTBOX[28 ] = {Active = 1,	ItemID = 0473,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Heavy Leather Gloves
BaoXiang_JLBCBTBOX[29 ] = {Active = 1,	ItemID = 0474,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Strong Leather Gloves
BaoXiang_JLBCBTBOX[30 ] = {Active = 1,	ItemID = 0476,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Chain Gauntlets
BaoXiang_JLBCBTBOX[31 ] = {Active = 1,	ItemID = 0482,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Canvas Gloves
BaoXiang_JLBCBTBOX[32 ] = {Active = 1,	ItemID = 0483,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Exquisite Gloves
BaoXiang_JLBCBTBOX[33 ] = {Active = 1,	ItemID = 0486,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Feather Gloves
BaoXiang_JLBCBTBOX[34 ] = {Active = 1,	ItemID = 0487,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Safari Gloves
BaoXiang_JLBCBTBOX[35 ] = {Active = 1,	ItemID = 0490,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Slick Gloves
BaoXiang_JLBCBTBOX[36 ] = {Active = 1,	ItemID = 0513,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Explorer Gloves
BaoXiang_JLBCBTBOX[37 ] = {Active = 1,	ItemID = 0514,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Adventure Gloves
BaoXiang_JLBCBTBOX[38 ] = {Active = 1,	ItemID = 0515,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Helmsman Gloves
BaoXiang_JLBCBTBOX[39 ] = {Active = 1,	ItemID = 0516,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Oarsman Gloves
BaoXiang_JLBCBTBOX[40 ] = {Active = 1,	ItemID = 0517,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Deckman Gloves
BaoXiang_JLBCBTBOX[41 ] = {Active = 1,	ItemID = 0536,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Meowy Muffs
BaoXiang_JLBCBTBOX[42 ] = {Active = 1,	ItemID = 0537,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Owl Muffs
BaoXiang_JLBCBTBOX[43 ] = {Active = 1,	ItemID = 0542,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Foster Gloves
BaoXiang_JLBCBTBOX[44 ] = {Active = 1,	ItemID = 0543,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Travel Gloves
BaoXiang_JLBCBTBOX[45 ] = {Active = 1,	ItemID = 0544,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Nurse Gloves
BaoXiang_JLBCBTBOX[46 ] = {Active = 1,	ItemID = 0546,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Holy Gloves
BaoXiang_JLBCBTBOX[47 ] = {Active = 1,	ItemID = 0549,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Scholar Gloves
BaoXiang_JLBCBTBOX[48 ] = {Active = 1,	ItemID = 0550,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Emergency Gloves
BaoXiang_JLBCBTBOX[49 ] = {Active = 1,	ItemID = 0557,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Kitty Muffs
BaoXiang_JLBCBTBOX[50 ] = {Active = 1,	ItemID = 0562,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Racoon Muffs
BaoXiang_JLBCBTBOX[51 ] = {Active = 1,	ItemID = 0565,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Night Owl Muffs
BaoXiang_JLBCBTBOX[52 ] = {Active = 1,	ItemID = 0566,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Kangaroo Muffs
BaoXiang_JLBCBTBOX[53 ] = {Active = 1,	ItemID = 0568,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Bunny Baby Muffs
BaoXiang_JLBCBTBOX[54 ] = {Active = 1,	ItemID = 0649,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Heavy Leather Boots
BaoXiang_JLBCBTBOX[55 ] = {Active = 1,	ItemID = 0650,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Strong Leather Boots
BaoXiang_JLBCBTBOX[56 ] = {Active = 1,	ItemID = 0652,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Chain Greaves
BaoXiang_JLBCBTBOX[57 ] = {Active = 1,	ItemID = 0658,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Canvas Boots
BaoXiang_JLBCBTBOX[58 ] = {Active = 1,	ItemID = 0659,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Exquisite Boots
BaoXiang_JLBCBTBOX[59 ] = {Active = 1,	ItemID = 0662,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Feather Boots
BaoXiang_JLBCBTBOX[60 ] = {Active = 1,	ItemID = 0665,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Hunter Boots
BaoXiang_JLBCBTBOX[61 ] = {Active = 1,	ItemID = 0666,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Slick Boots
BaoXiang_JLBCBTBOX[62 ] = {Active = 1,	ItemID = 0689,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Explorer Boots
BaoXiang_JLBCBTBOX[63 ] = {Active = 1,	ItemID = 0690,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Adventure Boots
BaoXiang_JLBCBTBOX[64 ] = {Active = 1,	ItemID = 0691,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Helmsman Boots
BaoXiang_JLBCBTBOX[65 ] = {Active = 1,	ItemID = 0692,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Oarsman Boots
BaoXiang_JLBCBTBOX[66 ] = {Active = 1,	ItemID = 0693,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Deckman Boots
BaoXiang_JLBCBTBOX[67 ] = {Active = 1,	ItemID = 0702,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Crabby Shoes
BaoXiang_JLBCBTBOX[68 ] = {Active = 1,	ItemID = 0704,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Playful Racoon Shoes
BaoXiang_JLBCBTBOX[69 ] = {Active = 1,	ItemID = 0705,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Duckling Shoes
BaoXiang_JLBCBTBOX[70 ] = {Active = 1,	ItemID = 0706,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Big Crab Shoes
BaoXiang_JLBCBTBOX[71 ] = {Active = 1,	ItemID = 0712,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Meowy Shoes
BaoXiang_JLBCBTBOX[72 ] = {Active = 1,	ItemID = 0713,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Owl Shoes
BaoXiang_JLBCBTBOX[73 ] = {Active = 1,	ItemID = 0718,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Foster Boots
BaoXiang_JLBCBTBOX[74 ] = {Active = 1,	ItemID = 0719,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Travel Boots
BaoXiang_JLBCBTBOX[75 ] = {Active = 1,	ItemID = 0720,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Nurse Boots
BaoXiang_JLBCBTBOX[76 ] = {Active = 1,	ItemID = 0722,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Holy Boots
BaoXiang_JLBCBTBOX[77 ] = {Active = 1,	ItemID = 0725,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Scholar Boots
BaoXiang_JLBCBTBOX[78 ] = {Active = 1,	ItemID = 0726,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Emergency Boots
BaoXiang_JLBCBTBOX[79 ] = {Active = 1,	ItemID = 0733,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Kitty Shoes
BaoXiang_JLBCBTBOX[80 ] = {Active = 1,	ItemID = 0738,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Racoon Shoes
BaoXiang_JLBCBTBOX[81 ] = {Active = 1,	ItemID = 0741,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Night Owl Shoes
BaoXiang_JLBCBTBOX[82 ] = {Active = 1,	ItemID = 0742,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Kangaroo Shoes
BaoXiang_JLBCBTBOX[83 ] = {Active = 1,	ItemID = 0744,	Quantity = 1,	Quality = 5,	Rad = 50 ,	GoodItem = 0}	-- Bunny Baby Shoes
BaoXiang_JLBCBTBOX[84 ] = {Active = 1,	ItemID = 0763,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Armor of Revenant
BaoXiang_JLBCBTBOX[85 ] = {Active = 1,	ItemID = 0770,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Sword of Grief
BaoXiang_JLBCBTBOX[86 ] = {Active = 1,	ItemID = 0777,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Robe of Death
BaoXiang_JLBCBTBOX[87 ] = {Active = 1,	ItemID = 0781,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Touch of Death
BaoXiang_JLBCBTBOX[88 ] = {Active = 1,	ItemID = 0785,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Staff of the Avenger
BaoXiang_JLBCBTBOX[89 ] = {Active = 1,	ItemID = 0789,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Robe of the Venom Witch
BaoXiang_JLBCBTBOX[90 ] = {Active = 1,	ItemID = 0799,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Tooth of Specter
BaoXiang_JLBCBTBOX[91 ] = {Active = 1,	ItemID = 0803,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Mantle of the Naga
BaoXiang_JLBCBTBOX[92 ] = {Active = 1,	ItemID = 0230,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Primal Tattoo
BaoXiang_JLBCBTBOX[93 ] = {Active = 1,	ItemID = 0304,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Ceremonial Platemail
BaoXiang_JLBCBTBOX[94 ] = {Active = 1,	ItemID = 0317,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Raptor Vest
BaoXiang_JLBCBTBOX[95 ] = {Active = 1,	ItemID = 0344,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Whirlpool Vest
BaoXiang_JLBCBTBOX[96 ] = {Active = 1,	ItemID = 0358,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Pincer Costume
BaoXiang_JLBCBTBOX[97 ] = {Active = 1,	ItemID = 0364,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Otter Costume
BaoXiang_JLBCBTBOX[98 ] = {Active = 1,	ItemID = 0377,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Protector Robe
BaoXiang_JLBCBTBOX[99 ] = {Active = 1,	ItemID = 0393,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Bunny Costume
BaoXiang_JLBCBTBOX[100] = {Active = 1,	ItemID = 0394,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Heavenly Vest
BaoXiang_JLBCBTBOX[101] = {Active = 1,	ItemID = 0480,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Ceremonial Gauntlets
BaoXiang_JLBCBTBOX[102] = {Active = 1,	ItemID = 0493,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Raptor Gloves
BaoXiang_JLBCBTBOX[103] = {Active = 1,	ItemID = 0520,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Whirlpool Gloves
BaoXiang_JLBCBTBOX[104] = {Active = 1,	ItemID = 0534,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Pincer Muffs
BaoXiang_JLBCBTBOX[105] = {Active = 1,	ItemID = 0540,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Otter Muffs
BaoXiang_JLBCBTBOX[106] = {Active = 1,	ItemID = 0553,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Protector Gloves
BaoXiang_JLBCBTBOX[107] = {Active = 1,	ItemID = 0569,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Bunny Muffs
BaoXiang_JLBCBTBOX[108] = {Active = 1,	ItemID = 0570,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Heavenly Gloves
BaoXiang_JLBCBTBOX[109] = {Active = 1,	ItemID = 0656,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Ceremonial Greaves
BaoXiang_JLBCBTBOX[110] = {Active = 1,	ItemID = 0669,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Raptor Boots
BaoXiang_JLBCBTBOX[111] = {Active = 1,	ItemID = 0696,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Whirlpool Boots
BaoXiang_JLBCBTBOX[112] = {Active = 1,	ItemID = 0710,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Pincer Shoes
BaoXiang_JLBCBTBOX[113] = {Active = 1,	ItemID = 0716,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Otter Shoes
BaoXiang_JLBCBTBOX[114] = {Active = 1,	ItemID = 0729,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Protector Boots
BaoXiang_JLBCBTBOX[115] = {Active = 1,	ItemID = 0745,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Lucky Bunny Shoes
BaoXiang_JLBCBTBOX[116] = {Active = 1,	ItemID = 0746,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Heavenly Shoes
BaoXiang_JLBCBTBOX[117] = {Active = 1,	ItemID = 3425,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Nal Runestone
BaoXiang_JLBCBTBOX[118] = {Active = 1,	ItemID = 3426,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Sol Runestone
BaoXiang_JLBCBTBOX[119] = {Active = 1,	ItemID = 3427,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Ja Runestone
BaoXiang_JLBCBTBOX[120] = {Active = 1,	ItemID = 3428,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- El Runestone
BaoXiang_JLBCBTBOX[121] = {Active = 1,	ItemID = 3429,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Cam Runestone
BaoXiang_JLBCBTBOX[122] = {Active = 1,	ItemID = 3430,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Tef Runestone
BaoXiang_JLBCBTBOX[123] = {Active = 1,	ItemID = 3431,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Yal Runestone
BaoXiang_JLBCBTBOX[124] = {Active = 1,	ItemID = 3432,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Lum Runestone
BaoXiang_JLBCBTBOX[125] = {Active = 1,	ItemID = 3433,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Fel Runestone
BaoXiang_JLBCBTBOX[126] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Mystery Fruit
BaoXiang_JLBCBTBOX[127] = {Active = 1,	ItemID = 3139,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Agrypnotic
BaoXiang_JLBCBTBOX[128] = {Active = 1,	ItemID = 3136,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Snowy Soft Bud
BaoXiang_JLBCBTBOX[129] = {Active = 1,	ItemID = 3140,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Magical Potion
BaoXiang_JLBCBTBOX[130] = {Active = 1,	ItemID = 3123,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Red Date Tea
BaoXiang_JLBCBTBOX[131] = {Active = 1,	ItemID = 3125,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Stramonium Fruit Juice
BaoXiang_JLBCBTBOX[132] = {Active = 1,	ItemID = 3122,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Elven Fruit Juice
BaoXiang_JLBCBTBOX[133] = {Active = 1,	ItemID = 3126,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Ice Cream
BaoXiang_JLBCBTBOX[134] = {Active = 1,	ItemID = 3127,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Rainbow Fruit Juice
BaoXiang_JLBCBTBOX[135] = {Active = 1,	ItemID = 3138,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Mystery Fruit
BaoXiang_JLBCBTBOX[136] = {Active = 1,	ItemID = 3135,	Quantity = 1,	Quality = 5,	Rad = 30 ,	GoodItem = 0}	-- Special Ointment
BaoXiang_JLBCBTBOX[137] = {Active = 1,	ItemID = 3828,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Thundoria Castle
BaoXiang_JLBCBTBOX[138] = {Active = 1,	ItemID = 3829,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Thundoria Harbor
BaoXiang_JLBCBTBOX[139] = {Active = 1,	ItemID = 3830,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Sacred Snow Mountain
BaoXiang_JLBCBTBOX[140] = {Active = 1,	ItemID = 3831,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Andes Forest Haven
BaoXiang_JLBCBTBOX[141] = {Active = 1,	ItemID = 3832,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket Oasis Haven
BaoXiang_JLBCBTBOX[142] = {Active = 1,	ItemID = 3833,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Icespire Haven
BaoXiang_JLBCBTBOX[143] = {Active = 1,	ItemID = 3834,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower
BaoXiang_JLBCBTBOX[144] = {Active = 1,	ItemID = 3835,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Barren Cavern
BaoXiang_JLBCBTBOX[145] = {Active = 1,	ItemID = 3836,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Abandoned Mine 2
BaoXiang_JLBCBTBOX[146] = {Active = 1,	ItemID = 3837,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Silver Mine 2
BaoXiang_JLBCBTBOX[147] = {Active = 1,	ItemID = 3838,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Silver Mine 3
BaoXiang_JLBCBTBOX[148] = {Active = 1,	ItemID = 3839,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower 2
BaoXiang_JLBCBTBOX[149] = {Active = 1,	ItemID = 3840,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower 3
BaoXiang_JLBCBTBOX[150] = {Active = 1,	ItemID = 3841,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower 4
BaoXiang_JLBCBTBOX[151] = {Active = 1,	ItemID = 3842,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower 5
BaoXiang_JLBCBTBOX[152] = {Active = 1,	ItemID = 3843,	Quantity = 1,	Quality = 5,	Rad = 25 ,	GoodItem = 0}	-- Ticket to Lone Tower 6
BaoXiang_JLBCBTBOX[153] = {Active = 1,	ItemID = 0899,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Book of Strength Reset
BaoXiang_JLBCBTBOX[154] = {Active = 1,	ItemID = 0900,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Book of Consitution Reset
BaoXiang_JLBCBTBOX[155] = {Active = 1,	ItemID = 0901,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Book of Agility Reset
BaoXiang_JLBCBTBOX[156] = {Active = 1,	ItemID = 0902,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Book of Accuracy Reset
BaoXiang_JLBCBTBOX[157] = {Active = 1,	ItemID = 0903,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Book of Spirit Reset
BaoXiang_JLBCBTBOX[158] = {Active = 1,	ItemID = 3846,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Voodoo Doll
BaoXiang_JLBCBTBOX[159] = {Active = 1,	ItemID = 3462,	Quantity = 1,	Quality = 5,	Rad = 20 ,	GoodItem = 0}	-- Magical Clover
BaoXiang_JLBCBTBOX[160] = {Active = 1,	ItemID = 0860,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Gem of the Wind
BaoXiang_JLBCBTBOX[161] = {Active = 1,	ItemID = 0861,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Gem of Striking
BaoXiang_JLBCBTBOX[162] = {Active = 1,	ItemID = 0862,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Gem of Colossus
BaoXiang_JLBCBTBOX[163] = {Active = 1,	ItemID = 0863,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Gem of Rage
BaoXiang_JLBCBTBOX[164] = {Active = 1,	ItemID = 3463,	Quantity = 1,	Quality = 5,	Rad = 20 ,	GoodItem = 0}	-- Icy Crystal
BaoXiang_JLBCBTBOX[165] = {Active = 1,	ItemID = 3844,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 0}	-- Heaven's Berry
BaoXiang_JLBCBTBOX[166] = {Active = 1,	ItemID = 3845,	Quantity = 1,	Quality = 5,	Rad = 10 ,	GoodItem = 0}	-- Charmed Berry
BaoXiang_JLBCBTBOX[167] = {Active = 1,	ItemID = 0878,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Fiery Gem
BaoXiang_JLBCBTBOX[168] = {Active = 1,	ItemID = 0879,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Furious Gem
BaoXiang_JLBCBTBOX[169] = {Active = 1,	ItemID = 0880,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Explosive Gem
BaoXiang_JLBCBTBOX[170] = {Active = 1,	ItemID = 0881,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Lustrious Gem
BaoXiang_JLBCBTBOX[171] = {Active = 1,	ItemID = 0882,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Glowing Gem
BaoXiang_JLBCBTBOX[172] = {Active = 1,	ItemID = 0883,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Shining Gem
BaoXiang_JLBCBTBOX[173] = {Active = 1,	ItemID = 0884,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Shadow Gem
BaoXiang_JLBCBTBOX[174] = {Active = 1,	ItemID = 0885,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Refining Gem
BaoXiang_JLBCBTBOX[175] = {Active = 1,	ItemID = 0887,	Quantity = 1,	Quality = 5,	Rad = 1  ,	GoodItem = 0}	-- Spirit Gem
BaoXiang_JLBCBTBOX[176] = {Active = 1,	ItemID = 0893,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 0}	-- Potion of Monkey
BaoXiang_JLBCBTBOX[177] = {Active = 1,	ItemID = 0894,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 0}	-- Potion of Eagle
BaoXiang_JLBCBTBOX[178] = {Active = 1,	ItemID = 0895,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 0}	-- Potion of Bull
BaoXiang_JLBCBTBOX[179] = {Active = 1,	ItemID = 0896,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 0}	-- Potion of Soul
BaoXiang_JLBCBTBOX[180] = {Active = 1,	ItemID = 0897,	Quantity = 1,	Quality = 5,	Rad = 5  ,	GoodItem = 0}	-- Potion of Lion
BaoXiang_JLBCBTBOX[181] = {Active = 1,	ItemID = 0453,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0}	-- Fusion Scroll
BaoXiang_JLBCBTBOX[182] = {Active = 1,	ItemID = 0455,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0}	-- Strengthening Scroll
BaoXiang_JLBCBTBOX[183] = {Active = 1,	ItemID = 0938,	Quantity = 1,	Quality = 5,	Rad = 100,	GoodItem = 0}	-- Goddess's Favor

-- [1872] Fairy Coin Chest
BaoXiang_CHESTCOIN = {}
BaoXiang_CHESTCOIN[1 ] = {Active = 1,	ItemID = 3048,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Thundoria Castle
BaoXiang_CHESTCOIN[2 ] = {Active = 1,	ItemID = 3049,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Thundoria Harbor
BaoXiang_CHESTCOIN[3 ] = {Active = 1,	ItemID = 3050,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Sacred Snow Mountain
BaoXiang_CHESTCOIN[4 ] = {Active = 1,	ItemID = 3051,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Andes Forest Haven
BaoXiang_CHESTCOIN[5 ] = {Active = 1,	ItemID = 3054,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower
BaoXiang_CHESTCOIN[6 ] = {Active = 1,	ItemID = 3059,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower 2
BaoXiang_CHESTCOIN[7 ] = {Active = 1,	ItemID = 3060,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower 3
BaoXiang_CHESTCOIN[8 ] = {Active = 1,	ItemID = 3070,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower 4
BaoXiang_CHESTCOIN[9 ] = {Active = 1,	ItemID = 3071,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower 5
BaoXiang_CHESTCOIN[10] = {Active = 1,	ItemID = 3072,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Lone Tower 6
BaoXiang_CHESTCOIN[11] = {Active = 1,	ItemID = 3057,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Silver Mine 2
BaoXiang_CHESTCOIN[12] = {Active = 1,	ItemID = 3058,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Silver Mine 3
BaoXiang_CHESTCOIN[13] = {Active = 1,	ItemID = 3073,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Abandon Mine 1
BaoXiang_CHESTCOIN[14] = {Active = 1,	ItemID = 3056,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Pass to Abandon Mine 2
BaoXiang_CHESTCOIN[15] = {Active = 1,	ItemID = 3460,	Quantity = 5,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Cake
BaoXiang_CHESTCOIN[16] = {Active = 1,	ItemID = 3134,	Quantity = 5,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Energetic Tea
BaoXiang_CHESTCOIN[17] = {Active = 0,	ItemID = 3848,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- Constitution Recovery Flask
BaoXiang_CHESTCOIN[18] = {Active = 1,	ItemID = 3099,	Quantity = 3,	Quality = 4,	Rad = 50 ,	GoodItem = 0}	-- SP Holy Water
BaoXiang_CHESTCOIN[19] = {Active = 1,	ItemID = 0878,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Fiery Gem
BaoXiang_CHESTCOIN[20] = {Active = 1,	ItemID = 0879,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Furious Gem
BaoXiang_CHESTCOIN[21] = {Active = 1,	ItemID = 0880,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Explosive Gem
BaoXiang_CHESTCOIN[22] = {Active = 1,	ItemID = 0881,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Lustrious Gem
BaoXiang_CHESTCOIN[23] = {Active = 1,	ItemID = 0882,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Glowing Gem
BaoXiang_CHESTCOIN[24] = {Active = 1,	ItemID = 0883,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Shining Gem
BaoXiang_CHESTCOIN[25] = {Active = 1,	ItemID = 0884,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Shadow Gem
BaoXiang_CHESTCOIN[26] = {Active = 1,	ItemID = 0885,	Quantity = 1,	Quality = 4,	Rad = 3 ,	GoodItem = 0}	-- Refining Gem
BaoXiang_CHESTCOIN[27] = {Active = 1,	ItemID = 0887,	Quantity = 1,	Quality = 4,	Rad = 9 ,	GoodItem = 0}	-- Spirit Gem
