#ifndef LUAAPI_H
#define LUAAPI_H

#include "lua.hpp"
#include <LuaBridge.h>
#include <optional>
#include <string>
#include <string_view>
#include <source_location>

// Implicit capture of call site via source_location default in constructor.
// Usage: g_luaAPI.Call("func", args...) — "func" converts to LuaCallSite,
// capturing file/line of the caller automatically.
struct LuaCallSite {
	std::string fn;
	std::source_location loc;

	LuaCallSite(const char* fn, std::source_location loc = std::source_location::current())
		: fn(fn), loc(loc) {
	}

	LuaCallSite(const std::string& fn, std::source_location loc = std::source_location::current())
		: fn(fn), loc(loc) {
	}

	LuaCallSite(std::string_view fn, std::source_location loc = std::source_location::current())
		: fn(fn), loc(loc) {
	}
};

class LuaAPI {
public:
	void Init(lua_State* L) {
		m_L = L;
	}

	lua_State* State() const {
		return m_L;
	}

	bool HasFunction(std::string_view fn) const {
		lua_getglobal(m_L, fn.data());
		bool isFunc = lua_isfunction(m_L, -1);
		lua_pop(m_L, 1);
		return isFunc;
	}

	// Call Lua function, no return value. Returns true on success.
	template <typename... Args>
	bool Call(LuaCallSite site, Args&&... args) const {
		luabridge::LuaRef func = luabridge::getGlobal(m_L, site.fn.c_str());
		if (!func.isFunction()) {
			ToLogService("lua", LogLevel::Error, "Call: function '{}' not found [{}:{}]",
						 site.fn, site.loc.file_name(), site.loc.line());
			return false;
		}
		luabridge::LuaResult result = func(std::forward<Args>(args)...);
		if (result.hasFailed()) {
			ToLogService("lua", LogLevel::Error, "Call '{}' failed: {} [{}:{}]",
						 site.fn, result.errorMessage(), site.loc.file_name(), site.loc.line());
			return false;
		}
		return true;
	}

	// Call Lua function, single return value.
	template <typename R, typename... Args>
	std::optional<R> CallR(LuaCallSite site, Args&&... args) const {
		luabridge::LuaRef func = luabridge::getGlobal(m_L, site.fn.c_str());
		if (!func.isFunction()) {
			ToLogService("lua", LogLevel::Error, "CallR: function '{}' not found [{}:{}]",
						 site.fn, site.loc.file_name(), site.loc.line());
			return std::nullopt;
		}
		luabridge::LuaResult result = func(std::forward<Args>(args)...);
		if (result.hasFailed()) {
			ToLogService("lua", LogLevel::Error, "CallR '{}' failed: {} [{}:{}]",
						 site.fn, result.errorMessage(), site.loc.file_name(), site.loc.line());
			return std::nullopt;
		}
		if (result.size() == 0)
			return std::nullopt;
		auto val = result[0].cast<R>();
		if (!val)
			return std::nullopt;
		return val.value();
	}

	// Call Lua function, multiple return values.
	// On failure (not found or runtime error) returns a failed LuaResult (hasFailed() == true).
	template <typename... Args>
	luabridge::LuaResult CallMulti(LuaCallSite site, Args&&... args) const {
		luabridge::LuaRef func = luabridge::getGlobal(m_L, site.fn.c_str());
		if (!func.isFunction()) {
			ToLogService("lua", LogLevel::Error, "CallMulti: function '{}' not found [{}:{}]",
						 site.fn, site.loc.file_name(), site.loc.line());
			return func(std::forward<Args>(args)...);
		}
		luabridge::LuaResult result = func(std::forward<Args>(args)...);
		if (result.hasFailed()) {
			ToLogService("lua", LogLevel::Error, "CallMulti '{}' failed: {} [{}:{}]",
						 site.fn, result.errorMessage(), site.loc.file_name(), site.loc.line());
		}
		return result;
	}

private:
	lua_State* m_L = nullptr;
};

extern LuaAPI g_luaAPI;

#endif // LUAAPI_H
