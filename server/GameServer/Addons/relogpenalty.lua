Relog = {}
Relog.Penalty = (2*60)
Relog.Maps = {}
Relog.Maps[1] = "garner2"

function relogerPunishment(Player)
	local ChaMap = GetChaMapName(Player)
	local PID = GetCharID(Player)
	local Dir = GetResPath(string.format("../PlayerData/RelogPenalty/%d.dat", GetPlayerID(GetChaPlayer(Player))))
	local Table = DataFile:Init(Dir, Table):Load()
	for k = 1, #Relog.Maps, 1 do
		if ChaMap == Relog.Maps[k] then
			Table[PID] = {Time = os.time() + Relog.Penalty}
			DataFile:Init(Dir, Table):Save()
		end
	end
end

function PlayerCanEnterPortal(Player)
	local PID = GetCharID(Player)
	local Dir = GetResPath(string.format("../PlayerData/RelogPenalty/%d.dat", GetPlayerID(GetChaPlayer(Player))))
	local Table = DataFile:Init(Dir, Table):Load()
	if (Table[PID] ~= nil) then
		if (os.time() > Table[PID].Time) then
			return 1
		else
			return 0
		end
	end
	return 1
end

function BeforeLeaveZone(Player)
	if (IsPlayer(Player) == 1) then
		hp = Hp(Player)
		if (hp > 0) then
			relogerPunishment(Player)
		end
	end
end

function BeforeEnterZone(Player)
	if (PlayerCanEnterPortal(Player) == 0) then
		return 0
	end
	return 1
end
