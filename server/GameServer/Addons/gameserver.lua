--[[
-- OnBankItem
-- @author: kong@pkodev.net
-- @param Player - player who is banking
-- @param Item - userdata Item type of targeted Item
-- @return return 1 to allow banking, return 0 to prevent banking
--]]

function OnBankItem(Player, Item)
    local ItemType = GetItemType(Item)
    if ItemType == 43 or ItemType == 45 or ItemType == 46 or GetItemID(Item) == 0682 then
        return 0
    end
    return 1
end

CUSTOM_RETURN_POINTS = 
{
	['puzzleworld'] = {Spawn = 'Sacred Snow Mountain', Heal = true},
	['puzzleworld2'] = {Spawn = 'Sacred Snow Mountain', Heal = true},
	['garner2'] = {Spawn = 'Chaos Argent Portal', Heal = false},
}

function MapRespawnOnDeath(Player, MapName)
	if CUSTOM_RETURN_POINTS[MapName] ~= nil then
		if CUSTOM_RETURN_POINTS[MapName].Heal then
			ReAll(Player)
		end
		return CUSTOM_RETURN_POINTS[MapName].Spawn
	end
	return ""
end

function ClearOnlineChars(cha_id)
	local role = GetRoleByID(cha_id)
	if (role ~= nil and type(role) == "userdata") then
		RemoveOfflineMode(role)
	end
end
