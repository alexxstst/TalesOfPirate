//---------------------------------------------------------
// EntityScript.cpp Created by knight-gong in 2005.5.12.
#include "stdafx.h"
#include "EntityScript.h"
#include "GameAppNet.h"
#include "Character.h"
#include "lua_gamectrl.h"

//---------------------------------------------------------
_DBC_USING
using namespace mission;

std::tuple<int, SubMap*> GetCurSubmap()
{
	if (!g_pScriptMap)
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", RES_STRING(GM_ENTITYSCRIPT_CPP_00001));
		return {LUA_FALSE, nullptr};
	}

	return {LUA_TRUE, g_pScriptMap};
}

std::tuple<int, mission::CEventEntity*> CreateEventEntity(int byType, SubMap* pMap, const std::string& name, int sID, int sInfoID, int dwxPos, int dwyPos, int sDir)
{
	mission::CEventEntity* pEntity = g_pGameApp->CreateEntity((BYTE)byType);
	if (!pMap || !pEntity)
	{
		return {LUA_FALSE, nullptr};
	}

	BOOL bRet = pEntity->Create(*pMap, name.c_str(), (USHORT)sID, (USHORT)sInfoID, (DWORD)dwxPos, (DWORD)dwyPos, (USHORT)sDir);
	return {bRet ? LUA_TRUE : LUA_FALSE, pEntity};
}

// SetEntityData has variable args depending on entity type — kept as lua_CFunction
int SetEntityData_raw(lua_State* L)
{
	BOOL bValid = lua_gettop(L) >= 1;
	if (!bValid)
	{
		E_LUAPARAM;
		return 0;
	}

	BOOL bRet = FALSE;
	mission::CEventEntity* pEntity = (mission::CEventEntity*)lua_touserdata(L, 1);
	switch (pEntity->GetType())
	{
	case BASE_ENTITY:
		break;

	case RESOURCE_ENTITY:
		{
			bValid = lua_gettop(L) >= 4;
			if (!bValid)
			{
				E_LUAPARAM;
				return 0;
			}
			USHORT sItemID = (USHORT)lua_tonumber(L, 2);
			USHORT sCount = (USHORT)lua_tonumber(L, 3);
			USHORT sTime = (USHORT)lua_tonumber(L, 4);
			bRet = ((mission::CResourceEntity*)pEntity)->SetData(sItemID, sCount, sTime);
		}
		break;

	case TRANSIT_ENTITY:
		break;

	case BERTH_ENTITY:
		{
			bValid = lua_gettop(L) >= 5;
			if (!bValid)
			{
				E_LUAPARAM;
				return 0;
			}
			USHORT sBerthID = (USHORT)lua_tonumber(L, 2);
			USHORT sxPos = (USHORT)lua_tonumber(L, 3);
			USHORT syPos = (USHORT)lua_tonumber(L, 4);
			USHORT sDir = (USHORT)lua_tonumber(L, 5);
			bRet = ((mission::CBerthEntity*)pEntity)->SetData(sBerthID, sxPos, syPos, sDir);
		}
		break;

	default:
		E_LUAPARAM;
		return 0;
	}

	lua_pushnumber(L, bRet ? LUA_TRUE : LUA_FALSE);
	return 1;
}

BOOL RegisterEntityScript()
{
	lua_State* L = g_pLuaState;

	luabridge::getGlobalNamespace(L)
		LUABRIDGE_REGISTER_FUNC(GetCurSubmap)
		LUABRIDGE_REGISTER_FUNC(CreateEventEntity);

	// Variable args — kept as lua_CFunction
	lua_register(L, "SetEntityData", SetEntityData_raw);

	return TRUE;
}
