function config_entry(entry)
    SetMapEntryEntiID(entry,193,1)
end

function after_create_entry(entry) 
    local copy_mgr		= GetMapEntryCopyObj(entry, 0)
    local EntryName		= "Chaos Argent"
    SetMapEntryEventName( entry, EntryName )
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    Notice("Announcement: Portal to Chaos Argent has opened at ["..posx..","..posy.."] Argent City!")
end

function after_destroy_entry_garner2(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry) 
    Notice("Announcement: The portal to Chaos Argent has vanished!") 
end

function after_player_login_garner2(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
	ChaNotice(player_name, "Announcement: Portal to Chaos Argent has opened at ["..posx..","..posy.."] Argent City!")
end

function check_can_enter_garner2(role, copy_mgr)
	if Lv(role) < 40 then
		SystemNotice(role, "To enter Chaos Argent, players need to be above level 40")
		return 0
	end
	if GetChaItem2(role, 2, 3849) == nil then
		SystemNotice(role, "You do not have a Medal of Valor to enter Chaos Argent. Please obtain it from Arena Administrator.")
		return 0
	end
	if IsInTeam(role) == 1 then
		SystemNotice(role, "You are not allowed to enter with a party.")
		return 0
	end
	local ActName,PlayerID = GetActName(role), GetCharID(role)
	if (ChaosArgentAlt[ActName] == nil) then
		ChaosArgentAlt[ActName] = {}
		ChaosArgentAlt[ActName].PID = GetCharID(role)
		ChaosArgentAlt[ActName].Num = 1
	end
	if (ChaosArgentAlt[ActName].PID ~= PlayerID) then
		if (ChaosArgentAlt[ActName].Num >= ChaosArgentAltLimit) then
			SystemNotice(role, "Unable to enter Chaos Argent! There\'s "..ChaosArgentAltLimit.." character inside already.")
			return 0
		end
	end
	if (BeforeEnterZone(role) == 0) then
		local PID = GetCharID(role)
		local Dir = GetResPath(string.format("../PlayerData/RelogPenalty/%d.dat", GetPlayerID(GetChaPlayer(role))))
		local Table = DataFile:Init(Dir, Table):Load()
		local TimeLeft = Table[PID].Time - os.time()
		PopupNotice(role, "You\'ve received a penalty for reloging.\nUnable to enter Chaos Argent for "..TimeLeft.." second(s)!")
		return 0
	end
	return 1
end

function begin_enter_garner2(role, copy_mgr)
	SystemNotice(role,"Entering [Chaos Argent]")
	MoveCity(role, "Chaos Argent")
	Notice(GetChaDefaultName(role).." has entered [Chaos Argent]")
end
