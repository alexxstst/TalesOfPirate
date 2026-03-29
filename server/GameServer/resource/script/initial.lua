print("--------------------------------------------------")
print("[Start] ** Loading LUA Files **")
print("--------------------------------------------------")
_G["0"] = function () end
_G["event_cha_lifetime"] = function(e) end
_G["TigerStart"] = function(e) end
do
    dofile(GetResPath("script\\hook.lua"))
    dofile(GetResPath("script\\datafile.lua"))
    dofile(GetResPath("script\\serialize.lua"))

    split = function(str, delim, maxNb)
        if string.find(str, delim) == nil then
            return {str}
        end
        if maxNb == nil or maxNb < 1 then
            maxNb = 0
        end
        local result = {}
        local pat = "(.-)" .. delim .. "()"
        local nb = 0
        local lastPos
        for part, pos in string.gmatch(str, pat) do
            nb = nb + 1
            result[nb] = part
            lastPos = pos
            if nb == maxNb then
                break
            end
        end

        if nb ~= maxNb then
            result[nb + 1] = string.sub(str, lastPos)
        end
        return result
    end
	
    turn_number = function(na)
        local num = tostring(na)
        local line, a = string.gsub(num, "%.", ",")
        local position = string.find(line, "[,]*//")
        if (position ~= 1) then
            local data = split(line, ",")
            local d1 = tonumber(data[1])
            local d2 = tonumber(data[2])
            if d2 == nil then
                return na
            end
            if (d2 / 100) < 0.5 then
                return d1
            elseif (d2 / 100) >= 0.5 then
                return (d1 + 1)
            end
        end
    end
	
    GetPlayerKey = function(role)
        local hex, name = "", GetChaDefaultName(role)
        while string.len(name) > 0 do
            local hb = string.format("%X", string.byte(name, 1, 1))
            if string.len(hb) < 2 then
                hb = "0" .. hb
            end
            hex = hex .. hb
            name = string.sub(name, 2)
        end
        return hex
    end

    round = function(n, mult)
        mult = mult or 1
        return math.floor((n + mult / 2) / mult) * mult
    end
	
    AdjustTextSpace = function(Text, Spaces, End)
        local Count = math.floor((Spaces - string.len(Text)) * 0.5)
        local Message = ""
        for C = 1, Count, 1 do
            Message = Message .. " "
        end
        Message = Message .. Text
        Count = math.floor(Spaces - string.len(Message))
        for C = 1, Count, 1 do
            Message = Message .. " "
        end
        if End ~= nil then
            Message = Message .. End
        end
        return Message
    end
	
    ExtractData = function(na)
        local num = tostring(na)
        local line, a = string.gsub(num, ",", ",")
        local position = string.find(line, "[,]*//")
        if (position ~= 1) then
            local data = split(line, ",")
            local d1 = tonumber(data[1])
            local d2 = tonumber(data[2])
            if d1 ~= nil then
                return d1
            end
        end
    end
	
    Escape = function(str)
        local newstring = ""
        for i = 1, string.len(str) do
            local c = string.sub(str, i, i)
            if (c == "'") then
                newstring = newstring .. "'"
            end
            newstring = newstring .. c
        end
        return newstring
    end
	
    FileToArray = function(file)
        if not FileExist(file) then
            local f = io.open(file, "a")
            f:close()
        end
        local f = io.open(file, "r")
        local Array = {}
        for line in f:lines() do
            table.insert(Array, line)
        end
        return Array
    end
	
    trim = function(s)
        return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
    end
	
    explode = function(separator, str)
        local pos, i, arr = 0, 0, {}
        for st, sp in function()
            return string.find(str, separator, pos, true)
        end do
            table.insert(arr, i, trim(string.sub(str, pos, st - 1)))
            pos = sp + 1
            i = i + 1
        end
        table.insert(arr, i, trim(string.sub(str, pos)))
        return arr
    end
	
    paste = function(file, str)
        local index = filetoarray(file)
        local r = 0
        for i = 1, #index do
            if index[i] ~= tostring(str) then
                r = r + 1
            end
        end
        if r == #index then
            local f = io.open(file, "a")
            f:write("\n")
            f:write(tostring(str))
            f:close()
        end
    end
	
    CreateDir = function(Path)
        os.execute('MKDIR "'..GetResPath(Path)..'"')
    end
	
	DirectoryExist = function(Path)
		if type(Path) ~= "string" then return false end
		local response = os.execute("cd "..Path)
		return response == 0
	end
	
	LogFile = function(Path, File, Text)
		if DirectoryExist(Path) == false then
			os.execute('MKDIR "'..Path..'"')
		end
		local F = io.open(File, 'a')
		F:write("["..os.date().."]\t"..Text.."\n")
		F:close()
	end
	
    GetNowDay = function()
        local now_day = os.date("%d")
        local Now_DayNum = tonumber(now_day)
        return Now_DayNum
    end
	
    GetTime = function(Seconds)
        if tonumber(Seconds) == 0 then
            return "00:00:00"
        else
            Hours = string.format("%02.f", math.floor(tonumber(Seconds) / 3600))
            Mins = string.format("%02.f", math.floor(tonumber(Seconds) / 60 - (Hours * 60)))
            Secs = string.format("%02.f", math.floor(tonumber(Seconds) - Hours * 3600 - Mins * 60))
            return Hours .. " Hour(s) and " .. Mins .. " minute(s)"
        end
    end
	
    GetServerTime = function()
        local Hour = tonumber(os.date("%H"))
        local Minute = tonumber(os.date("%M"))
        local Second = tonumber(os.date("%S"))
        return Hour, Minute, Second
    end
	
    PopupNotice = function(role, text)
		if string.len(text) > 150 then
			PopupNotice(role,"<PopupNotice> Message too long!")
			return
		end	
        local packet = GetPacket()
        WriteCmd(packet, 503)
        WriteString(packet, text)
        SendPacket(role, packet)
    end
	
    Say = function(character, text)
        local map_copy = GetChaMapCopy(character)
        local ply_num = GetMapCopyPlayerNum(map_copy)
        local ps = {}
        local i = 1

        BeginGetMapCopyPlayerCha(map_copy)
        for i = 1, ply_num, 1 do
            ps[i] = GetMapCopyNextPlayerCha(map_copy)
        end

        for i = 1, ply_num, 1 do
            if (ps[i] ~= 0 and ps[i] ~= nil) then
                ChaSay(ps[i], character, text)
            end
        end
    end
	
    ChaSay = function(role, target, text)
        local pkt = GetPacket()
        local tid = GetCharID(target)
        WriteCmd(pkt, 501)
        WriteDword(pkt, tid)
        WriteString(pkt, text)
        SendPacket(role, pkt)
    end
	
    AddComma = function(amount)
        local formatted = amount
        while true do
            formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", "%1,%2")
            if (k == 0) then
                break
            end
        end
        return formatted .. "G"
    end
end
