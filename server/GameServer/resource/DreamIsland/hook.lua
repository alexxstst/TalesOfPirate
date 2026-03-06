print("-- [Loading] DreamIslandHook.lua")

function dream_PKM(dead, atk)
	local MobID = GetChaTypeID(dead)
	if MobID == 1281 then
		local Note = "<Dream Island Invasion>: The wicked robber "..GetChaDefaultName(dead).." has been captured by "..GetChaDefaultName(atk)..". Thank you everyone for the help!"
		GMNotice(Note)
	end
end

function dream_MKP(dead, atk)
	local ChaName = GetChaDefaultName(dead)
	local Voice = {}
	Voice[1] = "[Thief - Wang Xiao Hu]: Haha! At least there is somebody worthy of my challenge! Only Death awaits you! Let the game begins!"
	Voice[2] = "[Thief - Wang Xiao Hu]: Foolish humans, Death has granted us the power of immortality!"
	Voice[3] = "[Thief - Wang Xiao Hu]: "..ChaName.." I will let you behold the real Power of Death!"
	Voice[4] = "[Thief - Wang Xiao Hu]: This is your last evasion "..ChaName.." and also your place of burial!"
	local Random = math.random(1, #Voice)
	Notice(Voice[Random])
end

function dream_PKP(dead , atk)
	local DeadName = GetChaDefaultName(dead)
	local KillerName = GetChaDefaultName(atk)
	Notice("Info: ["..DeadName.."] been buried by ["..KillerName.."] somewhere in Dream Island!")
end

----------------------------
--Hooking for Dream Island--
----------------------------
do
	dream_pkm_hook = GetExp_PKM
	GetExp_PKM = function(dead, atk)
		dream_pkm_hook(dead, atk)
		if (GetChaMapName(atk) == "DreamIsland") then
			dream_PKM(dead, atk)
		end
	end
	
	dream_mkp_hook = GetExp_MKP
	GetExp_MKP = function (dead , atk)
		dream_mkp_hook(dead, atk)
		if (GetChaMapName(atk) == "DreamIsland") then
			dream_MKP(dead, atk)
		end
	end	

	dream_pkp_hook = GetExp_PKP
	GetExp_PKP = function (dead , atk)
		dream_pkp_hook(dead, atk)
		if (GetChaMapName(atk) == "DreamIsland") then
			dream_PKP(dead , atk)
		end
	end
end
