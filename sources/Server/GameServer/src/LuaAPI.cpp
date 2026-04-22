#include "stdafx.h"
#include "LuaAPI.h"

LuaAPI g_luaAPI;

// ============================================================================
// Ранее inline-методы из LuaAPI.h, вынесены в .cpp 2026-04-22.
// Шаблонные Call/CallR/CallMulti остаются в хидере — обязательное требование
// для инстанцирования на стороне вызова.
// ============================================================================

LuaCallSite::LuaCallSite(const char* fn_, std::source_location loc_)
	: fn(fn_), loc(loc_) {}

LuaCallSite::LuaCallSite(const std::string& fn_, std::source_location loc_)
	: fn(fn_), loc(loc_) {}

LuaCallSite::LuaCallSite(std::string_view fn_, std::source_location loc_)
	: fn(fn_), loc(loc_) {}

void       LuaAPI::Init(lua_State* L)  { m_L = L; }
lua_State* LuaAPI::State() const       { return m_L; }

bool LuaAPI::HasFunction(std::string_view fn) const {
	lua_getglobal(m_L, fn.data());
	bool isFunc = lua_isfunction(m_L, -1);
	lua_pop(m_L, 1);
	return isFunc;
}
