--------------------------------------------------------------------------
--									--
--									--
--mission.lua Created by Robin 2004.12.27.	--
--									--
--									--
--------------------------------------------------------------------------
print( "loading mission.lua" )

-- Lua boolean value definitions
LUA_FALSE					= 0
LUA_TRUE					= 1

MIS_TREENODE_INVALID	= 0	-- Invalid quest
MIS_TREENODE_NOMAL		= 1	-- Normal quest
MIS_TREENODE_HISTORY	= 2	-- History quest
MIS_TREENODE_GUILD		= 3	-- Guild quest
		
MisData = {}

function AddMisData( id, name, tp )
	if id == nil or name == nil or tp == nil then
		print( "AddMisData: Function parameter error! Id = , Name = , tp = ", id, name, tp )
		return
	end
	
	if MisData[id] ~= nil then
		print( "AddMisData: Original quest ID overlayed when setting new quest date! Original data id = , name = , tp = , new data id = , name = , tp = ", id, MisData[id].name, MisData[id].tp, id, name, tp )
	end
	
	MisData[id] = {}
	MisData[id].name = name
	MisData[id].tp = tp
	
end

function GetMisData( id )
	if id == nil then
		print( "GetMisData: Obtained quest details, parameter error! id = nil" )
		return LUA_FALSE
	end
	
	if MisData[id] == nil or MisData[id].name == nil or MisData[id].tp == nil then
		print( "GetMisData: Obtain quest notice failed! Have not found target quest data notice. ID = "..id )
		return LUA_FALSE
	end
	
	return LUA_TRUE, MisData[id].tp, MisData[id].name
end
