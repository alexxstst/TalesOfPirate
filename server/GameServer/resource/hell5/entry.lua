function config_entry(entry)
    SetMapEntryEntiID(entry, 2492,1)
end 
function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0)
    SetMapEntryEventName(entry, "Abaddon Eternal")
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    Notice("Announcement: In the depths of Abaddon 18 ["..posx..","..posy.."] is accumulating a form of energy and energy of evil keeps welling out from it!")
end
function after_destroy_entry_hell5(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
end
function after_player_login_hell5(entry, player_name)
end
function check_can_enter_hell5(Player, copy_mgr)
	if Abaddon.Tick ~= 18 then
		SystemNotice(Player, "The power of darkness is sealing the gate. It will be impossible for you to pass.")
		return 0
	end
	return 1
end
function begin_enter_hell5(Player, copy_mgr) 
	SystemNotice(Player, "A strong force pulls you towards the endless darkness. When you open your eyes, you see before you a familiar world.")
	MoveCity(Player, "Eternal Abaddon")
end
