function config_entry(entry) 
    SetMapEntryEntiID(entry, 193,1)
end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry,0 ) 
	SetMapEntryEventName(entry, "Forsaken City 3")
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
end

function after_destroy_entry_abandonedcity3(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
end

function after_player_login_abandonedcity3(entry, player_name)

end

function begin_enter_abandonedcity3(role, copy_mgr) 
    SystemNotice(role,"Entering [Forsaken City 3]") 
    MoveCity(role, "Forsaken City 3")
end 
