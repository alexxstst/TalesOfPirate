function config_entry(entry) 
    SetMapEntryEntiID(entry, 2492,1)
end 
function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0)
    SetMapEntryEventName(entry, "Abaddon 5")
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: In the depths of Abaddon ["..posx..","..posy.."], cries of the undead have constantly been heard, resulting in the people of Caribbean Island being afraid. Is there any warrior whose willing to investigate?")
	Abaddon_Init()
end
function after_destroy_entry_hell(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
end
function after_player_login_hell(entry, player_name)
end
function check_can_enter_hell(Player, copy_mgr)
end
function begin_enter_hell(Player, copy_mgr) 
	SystemNotice(Player, "An unknown gravity pulls you towards the endless darkness. A darker Abaddon awaits you.")
	MoveCity(Player, "Abaddon 5 - 8")
end 
