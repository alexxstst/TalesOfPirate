function config_entry(entry) 
	SetMapEntryEntiID(entry, 2492,4)
end 

function after_create_entry(entry) 
    local CopyMgr = GetMapEntryCopyObj(entry, 0) 
	SetMapEntryEventName(entry, "Forsaken City") 
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: The portal of Forsaken City has appeared on Shaitan City ["..posx..","..posy.."].")
end

function after_destroy_entry_abandonedcity(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: The portal of Forsaken City has already disappeared. Please always check the announcement for the next portal appearance.") 
end

function after_player_login_abandonedcity(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    ChaNotice(player_name, "Announcement: The portal of Forsaken City has appeared on Shaitan City ["..posx..","..posy.."].") 
end

function check_can_enter_abandonedcity(Player, CopyMgr)
	if Lv(Player)>= 30 and Lv(Player) <= 45 then
		return 1
	else
		SystemNotice(Player, "Characters need to be level 30 - 45 to enter Forsaken City.")
		return 0    
	end
end

function begin_enter_abandonedcity(Player, CopyMgr) 
	SystemNotice(Player, "Entering [Forsaken City]")
	MoveCity(Player, "Forsaken City")
	Notice("Beware! "..GetChaDefaultName(Player).." has entered Forsaken City!")
end
