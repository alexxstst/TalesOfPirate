--[[
	Lua TradeServer by Billy.
	* Limited quantity by Mothana.
	* Individual item quantity by Angelix.
	* Fixed version for Corsairs files by N1nja
	
	Todo:
	* LuaAll is not properly working, should update all gameservers.
]]--
print("--------------------------------------------------")
print("[**] In-Game Shop Files [**]")

IGS = IGS or {}
IGS.Category = IGS.Category or {}
IGS.Texts = {}
IGS.LogTexts = {}
IGS.LogPaths = {}
IGS.Users = IGS.Users or {}
IGS.Packs = {}
IGS.Tabs = {}
IGS.Stock = IGS.Stock or {}

CMD_CM_STORE_OPEN_ASK = 41
CMD_CM_STORE_LIST_ASK = 42
CMD_CM_STORE_BUY_ASK = 43
CMD_CM_STORE_CHANGE_ASK = 44
CMD_CM_STORE_QUERY = 45
CMD_CM_STORE_VIP = 46
CMD_CM_STORE_AFFICHE = 47
CMD_CM_STORE_CLOSE = 48
CMD_MC_STORE_OPEN_ASR = 561
CMD_MC_STORE_LIST_ASR = 562
CMD_MC_STORE_BUY_ASR = 563
CMD_MC_STORE_CHANGE_ASR = 564
CMD_MC_STORE_QUERY = 565
CMD_MC_STORE_VIP = 566
CMD_MC_UPDATEIMP = 611
PACK_PER_PAGE = 9

function AddMallPack(title, description, price, hot, items, qty, stock)
	local index = #IGS.Packs + 1
	IGS.Packs[index] = {
		Title = title,
		Description = description,
		Price = price,
		Hot = hot,
		Items = items,
		Quantity = qty,
		Stock = stock,
	}
	return index
end

function AddMallTab(Title, Packs, Parent)
	Packs = Packs or {}
	local index = #IGS.Tabs+1
	IGS.Tabs[index] = {
		Title = Title,
		Packs = Packs,
		Parent = Parent or 0,
	}
	for i,v in pairs(Packs) do
		IGS.Packs[v].Enabled = true
	end
	return index
end

function AddPackToTab(tab, item)
	IGS.Tabs[tab].Packs[#IGS.Tabs[tab].Packs + 1] = IGS.Packs[item]
	IGS.Packs[item].Enabled = true
end

function operateIGS(Player, Packet)
	local cmd = ReadCmd(Packet)
	if cmd == CMD_CM_STORE_OPEN_ASK then
		openIGS(Player)
	elseif cmd == CMD_CM_STORE_BUY_ASK then
		local ID = ReadDword(Packet)
		IGS.BuyPackage(Player, ID)
	elseif cmd == CMD_CM_STORE_LIST_ASK then
		local lClsID = ReadDword(Packet)
		local sPage = ReadWord(Packet)
		local sNum = ReadWord(Packet)
		openIGSTab(Player,lClsID,sPage)
	elseif cmd == CMD_CM_STORE_CLOSE then
		IGS.Users[Player] = nil
	end
end

-- Set limited stock inside the IGS
function limitchange(Player,ID,limit)
	if	ID == nil or limit == nil then
		PopupNotice(Player,"Packet ID or Limit value is null!")
		return
	end
	local cmd = string.format([[IGS.Stock[%d].Stocks = %d]],ID,limit)
	Lua_All(cmd,Player)
end

function WritePackage(Packet, ID)
	local Package = IGS.Packs[ID]
	local qty = Package.Quantity
	local stock = Package.Stock
	-- Create stock tables
	if stock == nil then
		-- Store Stocks IDs and Qty of it
		stock = -1
	else
		-- Create table for each pack
		if IGS.Stock[ID] == nil then
			IGS.Stock[ID] = { Stocks = Package.Stock }
		end
	end
	-- End of stock tables
	WriteDword(Packet,ID) 							-- ComID
	WriteString(Packet, Package.Title) 				-- Package Name
	WriteDword(Packet, Package.Price) 				-- Price
	WriteString(Packet, Package.Description) 		-- Description
	WriteByte(Packet,  Package.Hot) 				-- Hot 0:1
	WriteDword(Packet, 0x80000000) 					-- Time (??)
	WriteDword(Packet, IGS.Stock[ID].Stocks) 		-- Stock Quantity
	WriteDword(Packet, 0x80000000) 					-- Time Remaining
	WriteWord(Packet, #Package.Items)				-- Number of items in a package
	for _, Item in pairs(Package.Items) do
		if type(Item) == 'table' then
			WriteItem(Packet, Item.ID, Item.Qty, Item.Attr)
		else
			WriteItem(Packet, Item)
		end
	end
end

function WriteItem(Packet, ItemID, Quantity, Attributes)
	WriteWord(Packet, ItemID)
	WriteWord(Packet, Quantity or 1)
	WriteWord(Packet, 0)
	for i = 1, 5, 1 do
		if Attributes and Attributes[i] then
			WriteWord(Packet, Attributes[i].ID)
			WriteWord(Packet, Attributes[i].Num)
		else
			WriteWord(Packet, 0)
			WriteWord(Packet, 0)
		end
	end
end

function openIGSTab(Player, Tab, Page)
	Tab = Tab or 1
	Page = Page or 1
	if not IGS.Tabs[Tab] or not Player then
		return
	end
	
	IGS.Users[Player] = {Tab, Page}
	local Packet = GetPacket()
	WriteCmd(Packet, CMD_MC_STORE_LIST_ASR)
	
	local TotalPackages = #IGS.Tabs[Tab].Packs
	local maxPage = math.ceil(TotalPackages / PACK_PER_PAGE)
	
	-- Max page (calc this)
	WriteWord(Packet, maxPage)
	
	-- Current page
	WriteWord(Packet, Page)
	
	-- Add <= if want to fill empty slots
	if Page < maxPage or TotalPackages == PACK_PER_PAGE then
		-- Number of item packages
		WriteWord(Packet,PACK_PER_PAGE)
	else
		-- Number of item packages
		WriteWord(Packet, (TotalPackages % PACK_PER_PAGE))
	end
	
	for i = 1, PACK_PER_PAGE, 1 do
		local index = i + (Page - 1) * PACK_PER_PAGE
		local packID = IGS.Tabs[Tab].Packs[index]
		if packID then
			WritePackage(Packet, packID)
		end
	end
	
	SendPacket(Player, Packet)
end

function openIGS(Player)
	if not Player then
		return
	end
	IGS.Users[Player] = true
	local Packet = GetPacket()
	WriteCmd(Packet, CMD_MC_STORE_OPEN_ASR)
	WriteByte(Packet, 1)
	WriteDword(Packet, 0)
	WriteDword(Packet, 0)
	WriteDword(Packet, GetIMP(Player))
	WriteDword(Packet, 0)
	WriteDword(Packet, #IGS.Tabs)
	for i,v in ipairs(IGS.Tabs) do
		WriteWord(Packet, i)
		WriteString(Packet, v.Title)
		WriteWord(Packet, v.Parent or 0)
	end
	SendPacket(Player,Packet)
end

function UpdateIGS()
	for i,v in pairs(IGS.Users) do
		if v then
			if type(v) == "table" then
				openIGSTab(i, v[1], v[2])
			else
				openIGS(i)
			end
		end
	end
end
UpdateIGS()

function IGS.BuyPackage(Player, ID)
	if IGS.Packs[ID] and IGS.Packs[ID].Enabled then
		local Package = IGS.Packs[ID]
		local Packet = GetPacket()
		WriteCmd(Packet, CMD_MC_STORE_BUY_ASR)
		
		-- Initiate cooldown table for player
		if IGS.Cooldown == nil then
			IGS.Cooldown = {}
			IGS.Cooldown.Attempt = 0
		end
		
		-- Saved player's data using os.time()
		if IGS.Cooldown[Player] == nil then
			IGS.Cooldown[Player] = {}
			IGS.Cooldown[Player] = os.time()
		end

		-- If player reaches 3rd attempt, then apply the delay to avoid them from spamming
		local Delay = 30
		if IGS.Cooldown.Attempt == 3 then
			IGS.Cooldown[Player] = os.time() + Delay
			IGS.Cooldown.Attempt = 0
		end
			
		-- Check if player have a cooldown restriction
		local Cooldown = IGS.Cooldown[Player] - os.time()
		if Cooldown > 0 then
			PopupNotice(Player, "You are being restricted for "..Cooldown.."sec(s) due to illegal behavior")
			return
		end
			
		-- Check if player inventory is lock
		if KitbagLock(Player, 0) == LUA_FALSE then
			PopupNotice(Player, "Inventory is locked. Please unlock before proceeding on your puchase")
			return
		end

		-- Check if player has enough crystals to buy package, if not, then exit transaction.
		if not HasIMP(Player, Package.Price) then
			IGS.Cooldown.Attempt = IGS.Cooldown.Attempt + 1
			WriteByte(Packet, 0)
			SendPacket(Player, Packet)
			return
		end
			
		-- Check if player inventory is lock
		if KitbagLock(Player, 0) == LUA_FALSE then
			PopupNotice(Player, "Inventory is locked. Please unlock before proceeding on your puchase")
			return
		end
		
		-- Check if player has enough slots in their temporary bag, if not, then exit transaction.
		if GetChaFreeTempBagGridNum(Player) < Package.Quantity then
			PopupNotice(Player, "You cannot purchase the package "..Package.Title.." due to not enough space, you need "..Package.Quantity.." free temporary bag slots.")
			return
		end

		-- Check stock by Mothannakh
		if IGS.Stock[ID].Stocks ~= -1 then
			if	IGS.Stock[ID].Stocks >= 1  then
				IGS.Stock[ID].Stocks = IGS.Stock[ID].Stocks - 1
				local cmd2 = string.format([[IGS.Stock[%d].Stocks = %d]], ID, IGS.Stock[ID].Stocks)
				LuaAll(cmd2)
				-- Purchase successful, update IGS throughout all GameServers.
				LuaAll('UpdateIGS()')
			else
				-- Item is out of stock, notify player that transaction has failed due to no stock.
				PopupNotice(Player, "This package is out of stock.")
				return
			end
		end
		
		-- Notify players that someone purchase from mall
		ScrollNotice("Player "..GetChaDefaultName(Player).." has purchased "..Package.Title.."", 1, 0XFFF6E58D)
		
		-- Take away package price in crystal(s) from player.
		TakeIMP(Player, Package.Price)
		
		-- Give the purchased item inside player's temporary bag
		for _, Item in ipairs(Package.Items) do
			if type(Item) == 'table' then
				GiveItemX(Player, 0, Item.ID, Item.Qty, Item.Qly or 0)
			else
				GiveItemX(Player, 0, Item, 1, 4)
			end
		end
		
		-- Record purchase log using Logging System
		local Path = GetResPath("../PlayerData/ShopPurchase/")
		local File = Path..GetChaDefaultName(TurnToCha(Player))..".txt"
		LogFile(Path, File, string.format("Bought [%s] for [%d], [%d] remaining.", Package.Title, Package.Price, GetIMP(Player)))
		
		WriteByte(Packet, 1)
		WriteDword(Packet, GetIMP(Player))
		SendPacket(Player, Packet)
	end
end

function GiveIMP(Player, Amount)	
	if type(Player) == "string" then
		Player = GetPlayerByName(Player)
	end
	SetIMP(Player, GetIMP(Player) + Amount)
end

function TakeIMP(Player, Amount)
	if type(Player) == "string" then
		Player = GetPlayerByName(Player)
	end
	local Crystals = GetIMP(Player)
	Crystals = Crystals - Amount
	SetIMP(Player, Crystals)
end

function HasIMP(Player, Amount)
	if type(Player) == "string" then
		Player = GetPlayerByName(Player)
	end
	return (GetIMP(Player) >= Amount)
end

function UpdateIMP(Player)
	local Packet = GetPacket()
	WriteCmd(Packet, CMD_MC_UPDATEIMP)
	WriteDword(Packet, GetIMP(Player))
	SendPacket(Player, Packet)
end

-- Load item list table
dofile(GetResPath("script/shop/fairy.lua"))
dofile(GetResPath("script/shop/leveling.lua"))
--dofile(GetResPath("script/shop/equipment.lua"))
dofile(GetResPath("script/shop/forging.lua"))
dofile(GetResPath("script/shop/tickets.lua"))
dofile(GetResPath("script/shop/apparels.lua"))
dofile(GetResPath("script/shop/grocery.lua"))
dofile(GetResPath("script/shop/mounts.lua"))
dofile(GetResPath("script/shop/crystals.lua"))
