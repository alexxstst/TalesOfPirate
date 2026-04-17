#pragma once

// FontManager — резолв и регистрация UI-шрифта по конфигу из system.ini.
//
// system.ini, секция [Fonts]:
//   FontDir    = ./font        — корневая папка шрифтов (подпапки на семейство)
//   SystemFont = PTSans        — ключ семейства (см. таблицу в FontManager.cpp)
//
// Если запрошенный ключ не найден в таблице или файлы на диске отсутствуют —
// fallback на системный "Arial". Зарегистрированные файлы подгружаются
// через AddFontResourceEx с FR_PRIVATE — видны только этому процессу.
//
// Использование:
//   FontManager::Instance().Init(g_SystemIni);                   // один раз при старте
//   FontManager::Instance().PushToLua(g_LuaState);               // до loadLua font.lua
//   const std::string& f = FontManager::Instance().GetResolvedFamily();

#include <string>
#include <vector>

namespace dbc { class IniFile; }
struct lua_State;

class FontManager {
public:
	static FontManager& Instance();

	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

	// Прочитать [Fonts] из ini, зарегистрировать TTF-файлы семейства
	// через AddFontResourceExA(FR_PRIVATE). Безопасно вызывать повторно.
	void Init(dbc::IniFile& ini);

	// Имя семейства для GDI (PT Sans / Open Sans / ... / Arial — при fallback).
	const std::string& GetResolvedFamily() const { return _resolvedFamily; }

	// Экспортировать глобал g_SystemFont в Lua. Вызывать ДО загрузки font.lua.
	void PushToLua(lua_State* L) const;

	~FontManager();

private:
	FontManager() = default;

	std::string              _resolvedFamily{"Arial"};
	std::vector<std::string> _registeredPaths{};
};
