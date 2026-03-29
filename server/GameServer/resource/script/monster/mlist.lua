function PM(monster)
    local filename = "script/monster/"..monster..".lua"
    dofile(GetResPath(filename))
end

local m = SetCurMap("garner")
if m==1 then
    local Now_Day = os.date("%d")
	local Now_Month = os.date("%m")
	local Now_Time = os.date("%H")
	local Now_Year = os.date("%y")
	local NowTimeNum = tonumber(Now_Time)
	local NowDayNum = tonumber(Now_Day)
	local NowMonthNum = tonumber(Now_Month)
	local NowYearNum = tonumber(Now_Year)
	local CheckDateNum = NowYearNum * 1000000 + NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    PM("garner01")
    PM("garner02")
    PM("garner03")
    PM("garner04")
    PM("garner05")
    PM("garner06")
    PM("garner07")
    PM("garner08")
	
	--if CheckDateNum >= 8122200  then
	--	if CheckDateNum <= 9010923 then
	--		PM("garner09")
	--		PM("garner10")
	--		PM("garner11")
	--	end
	--end
end


m = SetCurMap("magicsea")
if m==1 then
    local Now_Day = os.date("%d")
	local Now_Month = os.date("%m")
	local Now_Time = os.date("%H")
	local Now_Year = os.date("%y")
	local NowTimeNum = tonumber(Now_Time)
	local NowDayNum = tonumber(Now_Day)
	local NowMonthNum = tonumber(Now_Month)
	local NowYearNum = tonumber(Now_Year)
	local CheckDateNum = NowYearNum * 1000000 + NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
	PM("magicsea01")
    PM("magicsea02")
    PM("magicsea03")
    PM("magicsea04")
    PM("magicsea05")
    PM("magicsea06")
    PM("magicsea07")
    PM("magicsea08")
    PM("magicsea09")
    PM("magicsea10")
    PM("magicsea11")
    PM("magicsea12")
    PM("magicsea13")
    PM("magicsea14")
    PM("magicsea15")

	--if CheckDateNum >= 8122200  then
	--	if CheckDateNum <= 9010923 then
	--		PM("magicsea16")
	--		PM("magicsea17")
	--		PM("magicsea18")
	--	end
	--end
end

m = SetCurMap("darkblue")
if m==1 then
    local Now_Day = os.date("%d")
	local Now_Month = os.date("%m")
	local Now_Time = os.date("%H")
	local Now_Year = os.date("%y")
	local NowTimeNum = tonumber(Now_Time)
	local NowDayNum = tonumber(Now_Day)
	local NowMonthNum = tonumber(Now_Month)
	local NowYearNum = tonumber(Now_Year)
	local CheckDateNum = NowYearNum * 1000000 + NowMonthNum * 10000 + NowDayNum * 100 + NowTimeNum
    PM("darkblue01")
    PM("darkblue02")
    PM("darkblue03")
    PM("darkblue04")
    PM("darkblue05")
    PM("darkblue06")
    PM("darkblue07")
    PM("darkblue08")

	--if CheckDateNum >= 8122200  then
	--	if CheckDateNum <= 9010923 then
	--		PM("darkblue09")
	--		PM("darkblue10")
	--		PM("darkblue11")
	--	end
	--end

end

m = SetCurMap("lonetower")
if m==1 then
    PM("lonetower01")
end

m = SetCurMap("secretgarden")
if m==1 then
    PM("secretgarden01")
	PM("secretgarden02")
	PM("secretgarden03")
end

m = SetCurMap("eastgoaf")
if m==1 then
    PM("eastgoaf01")
    PM("eastgoaf02")
end

m = SetCurMap("heilong")
if m==1 then
    PM("heilong1")
    PM("heilong2")
end

m = SetCurMap("guildwar")
if m==1 then
    PM("guildwar01")
	PM("guildwar02")
end

m = SetCurMap("guildwar2")
if m==1 then
    PM("guildwar03")
	PM("guildwar04")
end

m = SetCurMap("07xmas2")
if m==1 then
    PM("07xmas2")
end

m = SetCurMap("sdBoss")
if m==1 then
    PM("sdBoss01")
end
