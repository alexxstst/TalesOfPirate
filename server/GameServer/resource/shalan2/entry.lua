--after_destroy_entry_testpk
--255

function config_entry(entry) 
    SetMapEntryEntiID(entry, 193,1) --characterinfo.txt

end 

function after_create_entry(entry) 
    local copy_mgr = GetMapEntryCopyObj(entry, 0) --

    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    Notice("Announcement: In Magical Ocean region, players has discovered ["..posx..","..posy.."] emerges a portal that leads to [Shaitan Mirage]. All players beware.") --

end

function after_destroy_entry_shalan2(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: According to reports, portal to [Shaitan Mirage] has disappeared. Check announcement for more details. Enjoy!") 

end

function after_player_login_shalan2(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) --
    ChaNotice(player_name, "Announcement: In Magical Ocean region, players has discovered ["..posx..","..posy.."] emerges a portal that leads to [Shaitan Mirage]. All players beware.") --

end









--
--01
function check_can_enter_shalan2( role, copy_mgr )
	local i = IsChaStall(role)
	if i == LUA_TRUE then
		SystemNotice(role, "Cannot teleport while setting stall")
		return 0    
	end
	if Lv(role) < 70 then
		SystemNotice(role, "70")
		return 0    
	end
	if Lv(role) > 89 then
		SystemNotice(role, "Characters need to be below Lv 90 to enter Shaitan Mirage")
		return 0    
	end
	
	local Num
	Num = CheckBagItem(role,2326)
	if Num < 1 then
		SystemNotice(role, "Without Reality Mask, ")	
		return 0
	end

	local Credit_Shalan2 = GetCredit(role)
	if Credit_Shalan2 < 10 then
		SystemNotice(role, "You do not have sufficient Reputation points. Unable to enter Shaitan Mirage")
		return 0
	else
		DelCredit(role,10)
		return 1
	end
end


function begin_enter_shalan2(role, copy_mgr)

	local Cha = TurnToCha(role)	
	local Dbag = 0
	Dbag = DelBagItem(Cha, 2326, 1)
	
	if Dbag == 1 then
		SystemNotice(role,"Entering [Shaitan Mirage]") 
		MoveCity(role, "Shaitan Mirage")

	else
	
		SystemNotice(role, "Collection of Reality Mask failed. Unable to enter Shaitan Mirage")
	end
end
