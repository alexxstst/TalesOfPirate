addon				= {}
addon.rootDir		= GetResPath('../addons/')
addon.files			=
{
	'gameserver.lua',
	'packet.lua',
	'utils.lua',
	'chathandler.lua',
	'itempre.lua',
	'npcexploit.lua',
	'amplifiers.lua',
	'make.lua',
	'guild.lua',
	'relogpenalty.lua',
	--'equips.lua',
}

print("--------------------------------------------------")
print("[**] Addon Files [**]")
addon.unpack = function()
	for i = 1, table.getn(addon.files) do
		dofile(addon.rootDir..addon.files[i])
		print("-- [Loading] "..addon.files[i])
	end
end

addon.unpack()
