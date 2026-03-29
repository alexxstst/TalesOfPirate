function explode(separator, str)
    local pos, arr = 0, {}
    for st, sp in function() return string.find(str, separator, pos, true) end do
        table.insert(arr, string.sub(str, pos, st-1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(str, pos))
    return arr
end

cmd = {} local cmd = cmd
cmd.symbol = '/'
cmd.list = {}

function HandleChat(role, message)
    if string.find(message, cmd.symbol) == 1 then
        local t = string.lower(string.sub(message, 2))
        if t == '' then
            BickerNotice(role, 'Invalid command!')
            return 0
        end
        local param = {n = 0}
        local r = string.find(t, ' ')
        if r == nil then     
			t = string.sub(message, 2)
        else                 
			t = string.sub(message, 2, r)
			local arg = string.sub(message, r + 2)
			param = explode(',', arg)
			param.n = #param
        end
        if cmd.list[t] ~= nil then
            local NocLock =    KitbagLock(role, 0)
            if NocLock ~= LUA_FALSE then
                if not cmd.canexecute(role, t, param) then
                    return 0
                end
                cmd.list[t].func(role, param)
                return 0
            else
                BickerNotice(role, 'Unlock inventory before inputting a command!');
                return 0;
            end
        else
            BickerNotice(role, 'Entered an invalid command!')
            return 0
        end
        return 0
    end
    return 1
end

cmd.checkparam = function(role, t, name)
    for i = 1, #cmd.list[name].param do
        local func;
        if cmd.list[name].param[i] == 'number' then
            func = tonumber
        elseif cmd.list[name].param[i] == 'string' then
            func = tostring
        end
        t[i] = func(t[i])
        if type(t[i]) ~= cmd.list[name].param[i] then
            BickerNotice(role, 'Parameter '..i..' must be a '..cmd.list[name].param[i]..'!')
            return false
        end
    end
    return true
end

cmd.canexecute = function(role, name, param)
    if GetGmLv(role) < cmd.list[name].gm then
        BickerNotice(role, 'This command requires GM Lv'..cmd.list[name].gm..' authorization!')
        return false
    end
    if param.n ~= #cmd.list[name].param then
        BickerNotice(role, 'This command requires '..#cmd.list[name].param..' paramenters!')
        return false
    end
    if not cmd.checkparam(role, param, name) then
        return false
    end
    return true
end

cmd.list['make'] = {
	gm 		= 99,
	param 	= {'number','number','number','number'},
	func 	= function(role, param) GiveItem(role, 0, param[1], param[2], 4, param[3], param[4]) end
} cmd.list['give'] = cmd.list['make']

cmd.list['shout'] = {
	gm 		= 99,
	param 	= {'string'},
	func 	= function(role, param) GMNotice(param[1]) end
}

cmd.list['gmlv'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) BickerNotice(role, 'Account: '..GetActName(role)..' GM Level '..GetGmLv(role)) end
}

cmd.list['online'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) SystemNotice(role, 'Players Online: '..GetOnlineCount()) end
} cmd.list['onlinecount'] = cmd.list['online']


cmd.list['date'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) BickerNotice(role, 'Server Date: '..os.date('%b')..'. '..os.date('%d')..', 20'..os.date('%y')) end
}

cmd.list['time'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) BickerNotice(role, 'Server Time: '..os.date('%H')..':'..os.date('%M')..':'..os.date('%S')) end
}

cmd.list['setgm'] = {
	gm 		= 99,
	param 	= {"string","number"},
	func 	= function(role, param) SetGM(role, param[1], param[2]) end
}

cmd.list['change'] = {
	gm 		= 99,
	param 	= {"number"},
	func 	= function(role, param) TransformCha(role, param[1]) end
}

cmd.list['bank'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenBank(role, role) end
}

cmd.list['forge'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenForge(role, role) end
}

cmd.list['socket'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenMilling(role, role) end
} cmd.list['milling'] = cmd.list['socket']

cmd.list['fusion'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenFusion(role, role) end
}

cmd.list['strengthening'] = {
	gm = 99,
	param = {},
	func = function(role, param) OpenUpgrade(role, role) end
}

cmd.list['upgrade'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenItemTiChun(role, role) end
}

cmd.list['recharge'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenItemEnergy(role, role) end
}

cmd.list['extract'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenGetStone(role, role) end
}

cmd.list['combine'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenUnite(role, role) end
}

cmd.list['getcrystals'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) GiveIMP(role, 9999) end
}

cmd.list['givecrystals'] = {
	gm 		= 99,
	param 	= {"string","number"},
	func 	= function(role, param) GiveCrystals(role, param[1], param[2]) end
}

cmd.list['takecrystals'] = {
	gm 		= 99,
	param 	= {"string","number"},
	func 	= function(role, param) TakeCrystals(role, param[1], param[2]) end
}

cmd.list['setstock'] = {
	gm 		= 99,
	param 	= {"number","number"},
	func 	= function(role, param) limitchange(role,param[1], param[2]) end
}

cmd.list['fairy'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenEidolonMetempsychosis(role, role) end
} cmd.list['elf'] = cmd.list['fairy']

cmd.list['tiger'] = {
	gm 		= 99,
	param 	= {},
	func 	= function(role, param) OpenTiger(role, role) end
} cmd.list['lottery'] = cmd.list['tiger']

cmd.list['buff'] = {
	gm 		= 29,
	param 	= {},
	func 	= function(role, param) FullBuffs(role) end
}

cmd.list['clear'] = {
	gm 		= 99,
	param 	= {"number"},
	func 	= function(role, param) ClearSlots(role, param[1]) end
}

cmd.list["senditem"] = {
	gm 		= 99,
	param 	= {'string','number','number','number'},
	func	= function(role, param) SendItem(role, param[1], param[2], param[3], param[4])
end
}

cmd.list["gmforge"] = {
	gm 		= 99,
	param 	= {'number','number','number','number','number','number','number','number','number'},
	func	= function(role, param) MakeForgedItems(role, param[1],  param[2], param[3], param[4], param[5], param[6], param[7], param[8], param[9])
end
}

cmd.list["ban"] = {
	gm 		= 99,
	param 	= {'string'},
	func	= function(role, param) BanActRole(param[1])
end
}

cmd.list["unban"] = {
	gm 		= 99,
	param 	= {'string'},
	func	= function(role, param) UnbanAct(param[1])
end
}

cmd.list["banplayer"] = {
	gm 		= 99,
	param 	= {'string'},
	func	= function(role, param) BanByName(role, param[1])
end
}

cmd.list["summon"] = {
	gm 		= 99,
	param 	= {'number','number'},
	func	= function(role, param) SummonMonster(role, param[1],param[2])
end
}

cmd.list['kick'] = {
	gm 		= 99,
	param 	= {"string"},
	func 	= function(role, param) KickPlayer(param[1]) end
}

cmd.list['close'] = {
	gm 		= 99,
	param 	= {"string"},
	func 	= function(role, param) ClosePlayerClient(param[1]) end
}

cmd.list["eff"] = {
	gm 		= 99,
	param 	= {'number'},
	func	= function(role, param) PlayEffect(role, param[1])
end
}

local a = ''
for i,v in pairs(cmd.list) do
	local arg, argnum = '', #v.param
	for i = 1, argnum do
		if i ~= argnum then 	
			arg = arg..'<'..v.param[i]..'>, '
		else 					
			arg = arg..'<'..v.param[i]..'>' 
		end
	end
	a = a..'/'..i..' '..arg..'_'..
	'    GM lv required: '..v.gm..'_'..
	'    Arguments: '..#v.param..'_'
end
