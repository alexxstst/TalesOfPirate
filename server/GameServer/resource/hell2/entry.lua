function config_entry(entry) 
 SetMapEntryEntiID(entry, 2492, 1)
end 
function after_create_entry(entry) 
 local copy_mgr = GetMapEntryCopyObj(entry, 0)
 SetMapEntryEventName(entry, "Abaddon 9")
 map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
 Notice("Announcement: In the depths of Abaddon 8["..posx..","..posy.."] opens the portal that leads to Abaddon 9!")
end
function after_destroy_entry_hell2(entry)
 map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
end
function after_player_login_hell2(entry, player_name)
end
function check_can_enter_hell2(Player, copy_mgr)
	if Abaddon.Tick ~= 8 then
		SystemNotice(Player, "The power of darkness is sealing the gate. It will be impossible for you to pass.")
		return 0
	end
	return 1
end
function begin_enter_hell2(Player, copy_mgr) 
	SystemNotice(Player, "An unknown gravity pulls you towards the endless darkness. A darker Abaddon awaits you.")
	MoveCity(Player, "Abaddon 9")
end 
