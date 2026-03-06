--after_destroy_entry_testpk
--255

function config_entry(entry) 
    SetMapEntryEntiID(entry, 193,1) --characterinfo.txt

end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) --
    local EntryName = "Arena Island"
    SetMapEntryEventName( entry, EntryName )

    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    Notice("Announcement: Map for Mass player kill has been opened!")  --

end

function after_destroy_entry_PKmap(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: According to reports, portal to [Arena Island] has vanished. Check announcement for more details. Enjoy!") 

end

function after_player_login_PKmap(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    ChaNotice(player_name, "Announcement: According to reports, Lower Icicle Castle at ["..posx..","..posy.."] appears a portal to Arena Island. Please check it out.")

end

function begin_enter_PKmap(role, copy_mgr) 

		SystemNotice(role,"Entering [Isle of PK]") 
		MoveCity(role, "PKmap")

end

