-- Avaliable functions:
-- GetChaName(player)
-- GetGmLv(player)
-- EstopPlayer(player, time in minutes)
-- SystemNotice(player, message)
-- GetGuildName(guild)
--/mute function
--unmute + name
--/mutelist show allpeople in muted list 
print("Loading...Group server Chat Function")

function WorldChat(player, message)
	-- Print message in GroupServer console
	--print("[World] " .. GetChaName(player) .. ": " .. message)
	-- Print message in system chat
	--SystemNotice(player, "Your name: " .. GetChaName(player))
	--SystemNotice(player, "Your GM Lvl: " .. GetGmLv(player))
	--SystemNotice(player, "You wrote in World Chat: " .. message)
	if string.len(message) > 100 then
		SystemNotice(player,"<Chat> Message too long!")
		return 0
	end	
	-- Write Message to log file
	LG("WorldChat", "[World] " .. GetChaName(player) .. ": " .. message)
	--[[
	-- Let's mute player for bad words
	local ban_time = 2 -- 5 minutes
	if (string.find(message, "fuck") ~= nil) then
		SystemNotice(player, "muted for " .. ban_time .. " minutes for usage of profanity")
		EstopPlayer(player, ban_time)
		-- Nobody will see message in World Chat
		return 0
	end
	]]
	return 1
end

-- Trade Chat
function TradeChat(player, message)
	-- Print message in GroupServer console
	--print("[Trade] " .. GetChaName(player) .. ": " .. message)
	-- Print message in system chat
	--SystemNotice(player, "Your name: " .. GetChaName(player))
	--SystemNotice(player, "Your GM Lvl: " .. GetGmLv(player))
	--SystemNotice(player, "You wrote in Trade Chat: " .. message)
	if string.len(message) > 100 then
		SystemNotice(player,"<Chat> Message too long!")
		return 0
	end	
	-- Write Message to log file
	LG("TradeChat", "[Trade] " .. GetChaName(player) .. ": " .. message)
	return 1
end

-- Guild chat
function GuildChat(player, guild, message)
	-- Print message in GroupServer console
	--print("[Guild](".. GetGuildName(guild) .. ") " .. GetChaName(player) .. ": " .. message)
	-- Print message in system chat
	--SystemNotice(player, "Your name: " .. GetChaName(player))
	--SystemNotice(player, "Your guild: " .. GetGuildName(guild))
	--SystemNotice(player, "Your GM Lvl: " .. GetGmLv(player))
	--SystemNotice(player, "You wrote in Guild Chat: " .. message)
	if string.len(message) > 100 then
		SystemNotice(player,"<Chat> Message too long!")
		return 0
	end	
	-- Write Message to log file
	LG("GuildChat", "[Guild](".. GetGuildName(guild) .. ")" .. GetChaName(player) .. ": " .. message)
	return 1
end

-- Private chat
function PrivateChat(player1, player2, message)
	-- Print message in GroupServer console
--	print("[Private] (".. GetChaName(player1) ..") ->  (".. GetChaName(player2) .."): " .. message)
	-- Print message in system chat
	--SystemNotice(player1, "You wrote to player (".. GetChaName(player2) ..") a message: " .. message)
	--SystemNotice(player2, "Player (".. GetChaName(player1) ..")  wrote to you a message: " .. message)
	-- Write Message to log file
	LG("PrivateChat", "[Private] (".. GetChaName(player1) ..") ->  (".. GetChaName(player2) .."): " .. message)
	----limit the private chat ---
	if string.len(message) > 100 then
		SystemNotice(player1,"<Chat> Message too long!")
		return 0
	end	
	-------------- GM Mute Function just pm a Player with "mute" to mute him for 10 minutes---
	local ban_time = 10
	if GetGmLv(player1) == 99 then 
	if (string.find(message, "mute") ~= nil) then
		SystemNotice(player1, "muted for " .. ban_time .. " minutes for usage of profanity")
		SystemNotice(player2,"["..GetChaName(player1).."] muted you for " .. ban_time .. " minutes for usage of profanity")
		EstopPlayer(player2, ban_time)
		-- Nobody will see message in World Chat
		LG("Player Mute","GM[["..GetChaName(player1).." ]Muted ["..GetChaName(player1).."] for ["..ban_time.."] minutes for usage of profanity")
		return 0
	end
	end
	--------
	
	return 1
end
