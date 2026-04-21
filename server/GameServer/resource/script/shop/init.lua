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

--      C++-   action-table:
--   action.cmd   (STORE_OPEN_ASK, STORE_BUY_ASK, STORE_LIST_ASK, STORE_CLOSE)
--   action.id    (STORE_BUY_ASK)
--   action.clsId, page, num (STORE_LIST_ASK)
function operateIGS(Player, action)
	local cmd = action.cmd
	if cmd == CMD_CM_STORE_OPEN_ASK then
		openIGS(Player)
	elseif cmd == CMD_CM_STORE_BUY_ASK then
		IGS.BuyPackage(Player, action.id)
	elseif cmd == CMD_CM_STORE_LIST_ASK then
		openIGSTab(Player, action.clsId, action.page)
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

--    Package       .
local function BuildProduct(ID)
	local Package = IGS.Packs[ID]
	local stock = Package.Stock
	if stock == nil then
		stock = -1
	else
		if IGS.Stock[ID] == nil then
			IGS.Stock[ID] = { Stocks = Package.Stock }
		end
	end

	local items = {}
	for idx, Item in ipairs(Package.Items) do
		if type(Item) == 'table' then
			local attrs = {}
			if Item.Attr then
				for i = 1, 5, 1 do
					if Item.Attr[i] then
						attrs[i] = { id = Item.Attr[i].ID, val = Item.Attr[i].Num }
					end
				end
			end
			items[idx] = { id = Item.ID, num = Item.Qty or 1, flute = 0, attrs = attrs }
		else
			items[idx] = { id = Item, num = 1, flute = 0 }
		end
	end

	return {
		comId    = ID,
		title    = Package.Title,
		price    = Package.Price,
		remark   = Package.Description,
		hot      = Package.Hot,
		time     = 0x80000000,
		quantity = (IGS.Stock[ID] and IGS.Stock[ID].Stocks) or stock,
		expire   = 0x80000000,
		items    = items,
	}
end

function openIGSTab(Player, Tab, Page)
	Tab = Tab or 1
	Page = Page or 1
	if not IGS.Tabs[Tab] or not Player then
		return
	end

	IGS.Users[Player] = {Tab, Page}

	local TotalPackages = #IGS.Tabs[Tab].Packs
	local maxPage = math.ceil(TotalPackages / PACK_PER_PAGE)

	local products = {}
	for i = 1, PACK_PER_PAGE, 1 do
		local index = i + (Page - 1) * PACK_PER_PAGE
		local packID = IGS.Tabs[Tab].Packs[index]
		if packID then
			products[#products + 1] = BuildProduct(packID)
		end
	end

	ShowStoreList(Player, maxPage, Page, products)
end

function openIGS(Player)
	if not Player then
		return
	end
	IGS.Users[Player] = true
	local tabs = {}
	for i, v in ipairs(IGS.Tabs) do
		tabs[i] = { id = i, title = v.Title, parent = v.Parent or 0 }
	end
	ShowStoreOpen(Player, 0, 0, GetIMP(Player), tabs)
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
			ShowStoreBuyResult(Player, false, GetIMP(Player))
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
		
		ShowStoreBuyResult(Player, true, GetIMP(Player))
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
	ShowUpdateImp(Player, GetIMP(Player))
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
