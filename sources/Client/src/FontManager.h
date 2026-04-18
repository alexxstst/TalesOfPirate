#pragma once

// FontManager — единый владелец всех CMPFont-инстансов клиента.
//
// Архитектура:
//   - TTF-файлы регистрируются через AddFontResourceExA(FR_PRIVATE)
//     методами InstallFontFile / InstallFontsFromDir (вызываются из Lua).
//   - CMPFont создаются через CreateFont(name, family, ...). Каждому шрифту
//     соответствует уникальное имя-ключ + int-handle (индекс в _fonts).
//   - C++ enum FontSlot маппится на заранее определённые имена ("TipText",
//     "MidAnnounce", "BottomShadow"). Get(FontSlot) — типобезопасный доступ.
//     Если слот не создан из Lua — авто-создание с дефолтами + error в лог.
//   - system.ini секция [Fonts]: FontDir (корневая папка) + SystemFont
//     (IniKey семейства либо GDI-имя). Resolved family пушится в Lua как
//     глобал g_SystemFont — его использует font_bootstrap.lua при CreateFont.
//
// Использование:
//   FontManager::Instance().Init(g_SystemIni);
//   FontManager::Instance().PushToLua(g_LuaState);
//   LoadLuaScript(L, "scripts/lua/font_bootstrap.lua");
//   ...
//   FontManager::Instance().Get(FontSlot::TipText)->DrawText(...);

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class CMPFont;
namespace dbc { class IniFile; }
struct lua_State;

enum class FontSlot {
	TipText = 0,
	MidAnnounce,
	BottomShadow,
	Console,
	_Count
};

class FontManager {
public:
	static FontManager& Instance();

	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

	// Прочитать [Fonts] из ini. TTF-файлы регистрируются из Lua — здесь
	// только резолвится семейство (IniKey → GDI family) и ставится fallback.
	void Init(dbc::IniFile& ini);

	// Экспортировать g_SystemFont в Lua. Вызывать ДО font_bootstrap.lua.
	void PushToLua(lua_State* L) const;

	const std::string& GetResolvedFamily() const { return _resolvedFamily; }

	// Регистрация TTF через AddFontResourceExA(FR_PRIVATE).
	bool InstallFontFile(const std::filesystem::path& ttf);
	int  InstallFontsFromDir(const std::filesystem::path& dir);

	// Создание именованного шрифта. Дубль имени → замена + warning.
	// Возвращает int-handle (индекс в _fonts). -1 при ошибке.
	int CreateFont(std::string name, const std::string& family,
				   int size800, int size1024, int level, unsigned long flags);

	// Доступ. Get(FontSlot) при отсутствии auto-create с дефолтами + error в лог.
	// Get(int)/Get(string) возвращают nullptr + error в лог.
	CMPFont* Get(FontSlot slot);
	CMPFont* Get(int handle);
	CMPFont* Get(const std::string& name);

	int FindHandle(const std::string& name) const; // -1 если нет

	// Диагностический дамп: превью всех шрифтов (белый фон, чёрный текст)
	// в <dir>/font_dump_<FontName>.bmp.
	void DumpAllSlots(const std::filesystem::path& dir);

	// Дамп D3D-атласов всех зарегистрированных шрифтов.
	// <dir>/atlas_<FontName>.bmp — шахматный фон + альфа из атласа.
	void DumpAllAtlases(const std::filesystem::path& dir);

	// Уничтожить все CMPFont (AddFontResourceEx-регистрации остаются).
	void ClearFonts();

	// Установить кодовую страницу DrawText для всех зарегистрированных шрифтов.
	// Используется как миграционный костыль: UI-текст пока ANSI (CP_ACP/CP1251),
	// Console-слот перенастраивается отдельно на CP_UTF8 в console_bootstrap.
	void SetAllCodepage(unsigned int codepage);

	~FontManager();

	// Каноническое имя enum-слота (для Lua-bootstrap и внутренних вызовов).
	static std::string_view SlotName(FontSlot slot);

private:
	FontManager() = default;

	// Выбор размера по текущему разрешению окна (нарастание: ≤ 800 — size800).
	static int _CurrentSize(int size800, int size1024);

	std::vector<std::unique_ptr<CMPFont>>  _fonts;
	std::vector<std::string>                _fontNames;  // parallel to _fonts
	std::unordered_map<std::string, int>    _byName;

	std::vector<std::string>                _registeredPaths;
	std::string                             _resolvedFamily{"Arial"};
};
