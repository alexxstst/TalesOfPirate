function config_entry(entry) 
    SetMapEntryEntiID(entry, 193,1) 
end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) 
	SetMapEntryEventName(entry, "Forsaken City 2")
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
end

function after_destroy_entry_abandonedcity2(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
end

function after_player_login_abandonedcity2(entry, player_name)

end

function check_can_enter_abandonedcity2(role, copy_mgr)
	for A = 1, 4, 1 do
		if CheckMonsterDead(BossFC[A]) == 0 then
			SystemNotice(role, "You cannot proceed until you kill all mini bosses on this floor.")
			return 0
		end
	end
end

function begin_enter_abandonedcity2(role, copy_mgr) 
	SystemNotice(role,"Entering [Forsaken City 2]") 
	MoveCity(role, "Forsaken City 2")
end
