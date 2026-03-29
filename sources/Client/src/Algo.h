
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

lua_State* init_lua();
void exit_lua(lua_State* L);
int load_luc(lua_State* L, char const* fname);
