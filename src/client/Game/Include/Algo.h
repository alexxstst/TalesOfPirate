#pragma once

#include <lua.hpp>
#include <LuaBridge.h>

lua_State* init_lua();
void exit_lua(lua_State* L);
int load_luc(lua_State* L, char const* fname);
