#include "stdafx.h"
#include "script.h"
#include <iostream>
#include "GameConfig.h"
#include "GameApp.h"

#include "lua_platform.h"
#include "UISystemForm.h"

using namespace std;

#define DEFAULT_SCRIPT_NUM		 1024

DWORD CScript::_dwCount = DEFAULT_SCRIPT_NUM;
DWORD CScript::_dwFreeCount = DEFAULT_SCRIPT_NUM;
DWORD CScript::_dwLastFree = 0;

CScript** CScript::_AllObj = NULL;

lua_State* g_LuaState = nullptr;

//---------------------------------------------------------------------------
// Lua panic handler
//---------------------------------------------------------------------------
int LuaPanicHandler(lua_State* L) {
	const char* msg = lua_tostring(L, -1);
	if (!msg) msg = "unknown error";
	ToLogService("lua", LogLevel::Error, "Lua PANIC: {}", msg);
	return 0;
}

//---------------------------------------------------------------------------
// Load plain .lua/.clu script
//---------------------------------------------------------------------------
int LoadLuaScript(lua_State* L, const std::string& filename) {
	int status = luaL_dofile(L, filename.c_str());
	if (status == 0) {
		ToLogService("lua", "LoadLuaScript '{}': OK", filename);
	}
	return status;
}

//---------------------------------------------------------------------------
// CScript
//---------------------------------------------------------------------------
bool CScript::Init() {
	_dwCount = DEFAULT_SCRIPT_NUM;
	_dwFreeCount = DEFAULT_SCRIPT_NUM;
	_dwLastFree = 0;

	_AllObj = new CScript*[_dwCount];
	memset(CScript::_AllObj, 0, _dwCount * sizeof(CScript*));
	return true;
}

bool CScript::Clear() {
	delete [] _AllObj;

	_AllObj = NULL;
	_dwCount = 0;
	_dwFreeCount = 0;
	_dwLastFree = 0;
	return true;
}

CScript::CScript() {
	if (_dwFreeCount <= 0) {
		_dwLastFree = _dwCount + 1;
		_dwCount += DEFAULT_SCRIPT_NUM;
		_dwFreeCount = DEFAULT_SCRIPT_NUM;

		CScript** tmp = _AllObj;

		_AllObj = new CScript*[_dwCount];
		memset(_AllObj, 0, _dwCount * sizeof(CScript*));
		memcpy(_AllObj, tmp, (_dwCount - DEFAULT_SCRIPT_NUM) * sizeof(CScript*));
		delete [] tmp;
	}

	if (!_AllObj[_dwLastFree]) {
		_AllObj[_dwLastFree] = this;
		_dwScriptID = _dwLastFree;

		--_dwFreeCount;
		++_dwLastFree;
		if (_dwLastFree >= _dwCount)
			_dwLastFree = 0;
		return;
	}

	for (DWORD i = _dwLastFree + 1; i < _dwCount; ++i) {
		if (!_AllObj[i]) {
			_AllObj[i] = this;
			_dwScriptID = i;

			--_dwFreeCount;
			return;
		}
	}

	for (int i = _dwLastFree - 1; i >= 0; --i) {
		if (!_AllObj[i]) {
			_AllObj[i] = this;
			_dwScriptID = i;

			--_dwFreeCount;
			return;
		}
	}

	ToLogService("errors", LogLevel::Error, "msgCScript::CScript Error, dwCount: {}, dwFreeCount: {}, dwLastFree: {}",
				 _dwCount, _dwFreeCount, _dwLastFree);
}

CScript::~CScript() {
	if (_dwScriptID > _dwCount)
		ToLogService("errors", LogLevel::Error,
					 "msgCScript::~CScript Error, dwCount: {}, dwFreeCount: {}, dwLastFree: {}", _dwCount, _dwFreeCount,
					 _dwLastFree);

	_AllObj[_dwScriptID] = NULL;

	_dwLastFree = _dwScriptID;
	++_dwFreeCount;
}

//---------------------------------------------------------------------------
// CScriptMgr
//---------------------------------------------------------------------------
lua_State* _pLuaState = NULL;

CScriptMgr::CScriptMgr() {
}

CScriptMgr::~CScriptMgr() {
	Clear();
}


bool CScriptMgr::Init() {
	if (!CScript::Init()) return false;


	// Register functions via LuaBridge
	extern void MPInitLua_Scene(lua_State* L);
	extern void MPInitLua_Gui(lua_State* L);
	extern void MPInitLua_Cha(lua_State* L);
	extern void MPInitLua_App(lua_State* L);

	MPInitLua_Scene(g_LuaState);
	MPInitLua_Gui(g_LuaState);
	MPInitLua_App(g_LuaState);
	MPInitLua_Cha(g_LuaState);

	// Load Lua scripts (debug.lua first — defines utility functions)
	LoadLuaScript(g_LuaState, "scripts/lua/debug.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/scene.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/scene/face.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/CameraConf.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/CharacterConf.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/mission/mission.lua");
	LoadLuaScript(g_LuaState, "scripts/lua/mission/missioninfo.lua");

	// lua_platform uses the same VM
	_pLuaState = g_LuaState;

	return true;
}

bool CScriptMgr::LoadScript() {
	_pLuaState = g_LuaState;
	return LoadLuaScript(_pLuaState, "scripts/lua/table/scripts.lua") == 0;
}

bool CScriptMgr::Clear() {
	if (!CScript::Clear()) return false;

	return true;
}

bool CScriptMgr::DoFile(const char* szLuaFile) {
	ToLogService("lua", "DoFile({})", szLuaFile);
	return luaL_dofile(_pLuaState, szLuaFile) != 0;
}

bool CScriptMgr::DoString(const char* szLuaString) {
	ToLogService("lua", "DoString({})", szLuaString);
	FILE* fp = fopen("luaexec.txt", "wt");
	if (fp == NULL) return false;
	fwrite(szLuaString, strlen(szLuaString), 1, fp);
	fclose(fp);
	return luaL_dofile(_pLuaState, "luaexec.tmp") != 0;
}

bool CScriptMgr::DoString(const char* szFunc, const char* szFormat, ...) {
	const double value = 1081000000.0;
	double dd = value / 1000.0 * 1000.0;
	if (dd != value) {
		_control87(_CW_DEFAULT, 0xfffff);
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(380), szFunc, szFormat));
	}

	int narg, nres;

	va_list vl;
	va_start(vl, szFormat);
	lua_getglobal(_pLuaState, szFunc);
	if (!lua_isfunction(_pLuaState, -1)) {
		lua_settop(_pLuaState, 0);
		ToLogService("common", "Func is Error, Func:{}, Fromat:{}", szFunc, szFormat);
		return false;
	}

	narg = 0;
	while (*szFormat) {
		switch (*szFormat++) {
		case 'f':
			lua_pushnumber(_pLuaState, va_arg(vl, double));
			break;
		case 'd':
			lua_pushnumber(_pLuaState, va_arg(vl, int));
			break;
		case 'u':
			lua_pushnumber(_pLuaState, va_arg(vl, unsigned int));
			break;
		case 's':
			lua_pushstring(_pLuaState, va_arg(vl, char*));
			break;
		case '-':
			goto endwhile;
		default:
			lua_settop(_pLuaState, 0);
			ToLogService("common", "Param Error, Func:{}, Fromat:{}", szFunc, szFormat);
			return false;
		}
		narg++;
		luaL_checkstack(_pLuaState, 1, "too many arguments");
	}

endwhile:

	nres = (int)strlen(szFormat);
	if (lua_pcall(_pLuaState, narg, nres, 0) != 0) {
		lua_settop(_pLuaState, 0);
		ToLogService("common", "Func call is error, Func:{}, Fromat:{}", szFunc, szFormat);
		return false;
	}

	nres = -nres;
	while (*szFormat) {
		switch (*szFormat++) {
		case 'f':
			if (!lua_isnumber(_pLuaState, nres)) {
				lua_settop(_pLuaState, 0);
				ToLogService("common", "return value(f) is error, Func:{}, Fromat:{}", szFunc, szFormat);
				return false;
			}

			*va_arg(vl, double*) = (double)lua_tonumber(_pLuaState, nres);
			break;
		case 'd':
			if (!lua_isnumber(_pLuaState, nres)) {
				lua_settop(_pLuaState, 0);
				ToLogService("common", "return value(d) is error, Func:{}, Fromat:{}", szFunc, szFormat);
				return false;
			}

			*va_arg(vl, int*) = (int)lua_tonumber(_pLuaState, nres);
			break;
		case 'u':
			if (!lua_isnumber(_pLuaState, nres)) {
				lua_settop(_pLuaState, 0);
				ToLogService("common", "return value(u) is error, Func:{}, Fromat:{}", szFunc, szFormat);
				return false;
			}

			*va_arg(vl, unsigned int*) = (unsigned int)lua_tonumber(_pLuaState, nres);
			break;
		case 's':
			if (!lua_isstring(_pLuaState, nres)) {
				lua_settop(_pLuaState, 0);
				ToLogService("common", "return value(s) is error, Func:{}, Fromat:{}", szFunc, szFormat);
				return false;
			}

			*va_arg(vl, string*) = lua_tostring(_pLuaState, nres);
			break;
		default:
			lua_settop(_pLuaState, 0);
			ToLogService("common", "return value(?) is error, Func:{}, Fromat:{}", szFunc, szFormat);
			return false;
		}
		nres++;
	}
	va_end(vl);
	lua_settop(_pLuaState, 0);
	return true;
}

string CScriptMgr::GetStoneHint(const char* szHintFun, int Lv) {
	const double value = 1081000000.0;
	double dd = value / 1000.0 * 1000.0;
	if (dd != value) {
		_control87(_CW_DEFAULT, 0xfffff);
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(381), szHintFun, Lv));
	}

	lua_getglobal(_pLuaState, szHintFun);
	if (!lua_isfunction(_pLuaState, -1)) {
		lua_pop(_pLuaState, 1);
		return GetLanguageString(382);
	}

	int nParamNum = 0;
	lua_pushnumber(_pLuaState, Lv);
	nParamNum = 1;
	int nState = lua_pcall(_pLuaState, nParamNum, LUA_MULTRET, 0);
	if (nState != 0) {
		ToLogService("common", "DoString {}", szHintFun);
		lua_pop(_pLuaState, 2);
		return "lua_pcall error";
	}

	string hint;
	int nRetNum = 1;
	if (!lua_isstring(_pLuaState, -1)) {
		g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(383));
	}
	else {
		hint = lua_tostring(_pLuaState, -1);
	}
	lua_pop(_pLuaState, nRetNum);
	return hint;
}
