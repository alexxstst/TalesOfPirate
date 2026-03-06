function config_entry(entry) 
    SetMapEntryEntiID(entry, 2492,4)
end 

function after_create_entry(entry) 
    local CopyMgr = GetMapEntryCopyObj(entry, 0)
	SetMapEntryEventName(entry, "Dark Swamp") 
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
	Notice("Announcement: The portal of Dark Swamp has appeared on Shaitan City ["..posx..","..posy.."].")
end

function after_destroy_entry_darkswamp(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: The portal of Dark Swamp has already disappeared. Please always check the announcement for the next portal appearance.") 
end

function after_player_login_darkswamp(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    ChaNotice(player_name, "Announcement: The portal of Dark Swamp has appeared on Shaitan City ["..posx..","..posy.."].") 
end

function check_can_enter_darkswamp( Player, CopyMgr )
    local Cha = TurnToCha(Player)
	if Lv(Cha) >= 40 and Lv(Cha) <= 55 then
		return 1
	else
		SystemNotice(Player, "Characters need to be level 40 - 55 to enter Dark Swamp")
		return 0
	end
end

function begin_enter_darkswamp(Player, CopyMgr) 
	SystemNotice(Player, "Entering Dark Swamp") 
	MoveCity(Player, "Dark Swamp")
	Notice("Beware! "..GetChaDefaultName(Player).." has entered Dark Swamp!")
end 
