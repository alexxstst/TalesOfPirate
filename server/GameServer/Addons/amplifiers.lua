Amplifier = {}
Amplifier[3844] = {State = {STATE_SBJYGZ}, StateLevel = 1, Type = "experience", Bonus = 2, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[3094] = {State = {STATE_SBJYGZ}, StateLevel = 1, Type = "experience", Bonus = 2, StateTime = 1800, Level = {Min = nil, Max = nil}}
Amplifier[3095] = {State = {STATE_SBJYGZ}, StateLevel = 2, Type = "experience", Bonus = 2.5, StateTime = 1800, Level = {Min = 60, Max = nil}}
Amplifier[3880] = {State = {STATE_SBJYGZ}, StateLevel = 2, Type = "experience", Bonus = 2.5, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[898]  = {State = {STATE_SBJYGZ}, StateLevel = 1, Type = "experience", Bonus = 2, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[1128] = {State = {STATE_SBJYGZ}, StateLevel = 1, Type = "experience", Bonus = 2, StateTime = 900, Level = {Min = nil, Max = 40}}
Amplifier[3039] = {State = {STATE_SBJYGZ}, StateLevel = 3, Type = "experience", Bonus = 5, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[993]  = {State = {STATE_SBJYGZ}, StateLevel = 4, Type = "experience", Bonus = 10, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[48]   = {State = {STATE_SBJYGZ}, StateLevel = 3, Type = "experience", Bonus = 5, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[56]   = {State = {STATE_SBJYGZ}, StateLevel = 4, Type = "experience", Bonus = 10, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[998]  = {State = {STATE_SBJYGZ}, StateLevel = 4, Type = "experience", Bonus = 10, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[3845] = {State = {STATE_SBBLGZ}, StateLevel = 1, Type = "drop", Bonus = 2, StateTime = 900, Level = {Min = nil, Max = nil}}
Amplifier[3881] = {State = {STATE_SBBLGZ}, StateLevel = 2, Type = "drop", Bonus = 2.5, StateTime = 900, Level = {Min = 40, Max = nil}}
Amplifier[3882] = {State = {STATE_SBBLGZ}, StateLevel = 3, Type = "drop", Bonus = 3.25, StateTime = 900, Level = {Min = 60, Max = nil}}
Amplifier[3096] = {State = {STATE_SBBLGZ}, StateLevel = 1, Type = "drop", Bonus = 2, StateTime = 1800, Level = {Min = nil, Max = nil}}
Amplifier[3097] = {State = {STATE_SBBLGZ}, StateLevel = 3, Type = "drop", Bonus = 3.25, StateTime = 1800, Level = {Min = 60, Max = nil}}
Amplifier[1006] = {State = {STATE_SBBLGZ}, StateLevel = 1, Type = "drop", Bonus = 2, StateTime = 1800, Level = {Min = nil, Max = nil}}
Amplifier[578]  = {State = {STATE_JLJSGZ}, StateLevel = 1, Type = "fairy growth", Bonus = 2, StateTime = 900, Level = {Min = nil, Max = nil}}

function AmplifierItem(role, Item)
	local ItemID = GetItemID(Item)
	local Level = GetChaAttr(role, ATTR_LV)
	if Amplifier[ItemID] ~= nil then
		if Amplifier[ItemID].Level.Min ~= nil then
			if Level < Amplifier[ItemID].Level.Min then
				SystemNotice(role, ""..GetItemName(ItemID).." is for Level "..Amplifier[ItemID].Level.Min.." and above!") 
				UseItemFailed(role) 
				return
			end
		end
		if Amplifier[ItemID].Level.Max ~= nil then
			if Level > Amplifier[ItemID].Level.Max then
				SystemNotice(role, ""..GetItemName(ItemID).." is for Level "..Amplifier[ItemID].Level.Max.." and below!")
				UseItemFailed(role) 
				return
			end
		end
		
		local Boat = 0
		Boat = GetCtrlBoat(role)
		for i,v in pairs(Amplifier[ItemID].State) do
			if Boat == nil then
				if GetChaStateLv(role, v) >= Amplifier[ItemID].StateLevel then
					SystemNotice(role, "You are already using a fruit with the same or superior effect than "..GetItemName(ItemID)..".")
					UseItemFailed(role)
					return
				end
				AddState(role, role, v, Amplifier[ItemID].StateLevel, Amplifier[ItemID].StateTime)
				SystemNotice(role, "Feels the effect of "..GetItemName(ItemID)..". Granted "..(Amplifier[ItemID].Bonus).." times of "..Amplifier[ItemID].Type.." bonus for duration "..Amplifier[ItemID].StateTime.." secs.")
			else
				if GetChaStateLv(Boat, v) >= Amplifier[ItemID].StateLevel then
					SystemNotice(Boat, "You are already using a fruit with the same or superior effect than "..GetItemName(ItemID)..".")
					UseItemFailed(Boat)
					return
				end
				AddState(Boat, Boat, v, Amplifier[ItemID].StateLevel, Amplifier[ItemID].StateTime)
				SystemNotice(Boat, "Feels the effect of "..GetItemName(ItemID)..". Granted "..(Amplifier[ItemID].Bonus).." times of "..Amplifier[ItemID].Type.." bonus for duration "..Amplifier[ItemID].StateTime.." secs.")
			end
		end
	else
		SystemNotice(role, "This amplifier is not registered!")
		UseItemFailed(role)
		return
	end
end
