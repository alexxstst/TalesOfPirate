--after_destroy_entry_testpk
--255

function config_entry(entry) 
    SetMapEntryEntiID(entry, 193,1) --characterinfo.txt

end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) --

    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    Notice("Announcement: According to reports, near Ascaron at ["..posx..","..posy.."] emerges a portal to [Thundoria Mirage]. All players please take note!") --

end

function after_destroy_entry_leiting2(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: According to reports, the portal to [Thundoria Mirage] has vanished. Enjoy!") 

end

function after_player_login_leiting2(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    ChaNotice(player_name, "Announcement: According to reports, near Ascaron at ["..posx..","..posy.."] emerges a portal to [Thundoria Mirage]. All players please take note!") --

end









--
--01
function check_can_enter_leiting2( role, copy_mgr )
	local i = IsChaStall(role)
	if i == LUA_TRUE then
		SystemNotice(role, "Cannot teleport while setting stall")
		return 0    
	end
	if Lv(role) < 70 then
		SystemNotice(role, " Characters need to be above Lv 70 to enter Icicle Mirage")
		return 0    
	end
	if Lv(role) > 89 then
		SystemNotice(role, "Characters need to be below Lv 90 to enter Thundoria Mirage")
		return 0    
	end
	
	local Num
	Num = CheckBagItem(role,2326)
	if Num < 1 then
		SystemNotice(role, "Without the Reality Mask, there is no way to get to Thundoria Mirage")	
		return 0
	end

	local Credit_Leiting2 = GetCredit(role)
	if Credit_Leiting2 < 10 then
		SystemNotice(role, "You do not have sufficient Reputation points. Unable to enter Thundoria Mirage")
		return 0
	else
		DelCredit(role,10)
		return 1
	end
end


function begin_enter_leiting2(role, copy_mgr)

	local Cha = TurnToCha(role)	
	local Dbag = 0
	Dbag = DelBagItem(Cha, 2326, 1)
	
	if Dbag == 1 then
		SystemNotice(role,"Entering [Thundoria Mirage]") 
		MoveCity(role, "Thundoria Mirage")

	else
	
		SystemNotice(role, "Collection of Reality Mask failed. Unable to enter Thundoria Mirage")
	end
end
