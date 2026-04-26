#pragma once
#include <vector>
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// Global Lua VM (LuaJIT)
extern lua_State* g_LuaState;

// Include LuaBridge only where needed (heavy header  avoid in widely-included files)
// Usage: #include <LuaBridge.h> in files that register functions
#define LUABRIDGE_REGISTER_FUNC(fn) .addFunction(#fn, fn)

// Register a raw lua_CFunction (int(lua_State*)) directly
#define LUA_REGISTER_CFUNC(L, fn) lua_register(L, #fn, fn)

enum eScriptReturn {
	R_OK = 1,
	R_FAIL = -1,
};

class CScript {
public:
	CScript();
	virtual ~CScript();

	DWORD GetScriptID() {
		return _dwScriptID;
	}

public:
	static CScript* GetScriptObj(DWORD id) {
		if (id < _dwCount) return _AllObj[id];
		return NULL;
	}

	static bool Init();
	static bool Clear();

private:
	DWORD _dwScriptID;

private:
	static CScript** _AllObj;
	static DWORD _dwCount;
	static DWORD _dwFreeCount;
	static DWORD _dwLastFree;
};

// Log Lua error from stack and pop it
inline void LuaPrintError(lua_State* L) {
	const char* err = lua_tostring(L, -1);
	ToLogService("lua", LogLevel::Error, "Lua error: {}", err ? err : "unknown");
	lua_pop(L, 1);
}

// luaL_dofile with built-in error logging
#undef luaL_dofile
#define luaL_dofile(L, fn) \
	((luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0 ? (LuaPrintError(L), 1) : 0)

// luaL_dostring with built-in error logging
#undef luaL_dostring
#define luaL_dostring(L, s) \
	((luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0 ? (LuaPrintError(L), 1) : 0)

// Load a .lua script with error logging
int LoadLuaScript(lua_State* L, const std::string& filename);

class CScriptMgr {
public:
	CScriptMgr();
	~CScriptMgr();

	bool Init();
	bool LoadScript();
	bool Clear();
	bool DoFile(const char* szLuaFile);
	bool DoString(const char* szLuaString);

	std::string GetStoneHint(const char* szHintFun, int Lv);

	bool DoString(const char* szFunc, const char* szFormat, ...);
};
