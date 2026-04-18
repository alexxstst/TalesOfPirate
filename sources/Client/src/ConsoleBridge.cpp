#include "stdafx.h"
#include "ConsoleBridge.h"
#include "FontManager.h"
#include "GameApp.h"
#include "GameConfig.h"
#include "GlobalVar.h"
#include "LoginScene.h"
#include "Scene.h"
#include "lua_platform.h"
#include "IniFile.h"

// Legacy C++ callback — fallback для команд ещё не мигрированных в Lua.
extern const char* ConsoleCallback(const char* pszCmd);

namespace {

// ---- C-функции, экспортируемые в Lua (без namespace, имена с префиксом) ----

void Console_Print(const std::string& text) {
    if (!ConsoleBridge::Get().IsMainThread("console.print")) {
        return;
    }
    if (g_pGameApp && g_pGameApp->GetConsole()) {
        g_pGameApp->GetConsole()->AddText(text);
    }
}

void Console_PrintErr(const std::string& text) {
    if (!ConsoleBridge::Get().IsMainThread("console.print_err")) {
        return;
    }
    if (g_pGameApp && g_pGameApp->GetConsole()) {
        g_pGameApp->GetConsole()->AddText("[!] " + text);
    }
    ToLogService("lua", LogLevel::Error, "{}", text);
}

void Console_Clear() {
    if (!ConsoleBridge::Get().IsMainThread("console.clear")) {
        return;
    }
    if (g_pGameApp && g_pGameApp->GetConsole()) {
        g_pGameApp->GetConsole()->Clear();
    }
}

// Задать размеры консоли: ширина как % от окна, число строк (= высота в
// строках FontSlot::Console + padding). Вызывается из console_bootstrap.lua.
void Console_SetSize(int widthPercent, int lines) {
    if (!g_pGameApp || !g_pGameApp->GetConsole()) {
        return;
    }
    if (widthPercent < 10)  widthPercent = 10;
    if (widthPercent > 100) widthPercent = 100;
    if (lines < 3)          lines = 3;
    if (lines > 200)        lines = 200;

    constexpr int kLineStep = 22;
    constexpr int kPadding  = 24;
    const int scrW = g_Render.GetScrWidth();
    const int w = scrW > 0 ? (scrW * widthPercent / 100) : 1024;
    const int h = lines * kLineStep + kPadding;
    g_pGameApp->GetConsole()->Resize(w, h);
}

// Задать ARGB-цвет backdrop (как целое hex, например 0xE0101018).
void Console_SetBackdrop(std::uint32_t argb) {
    if (g_pGameApp && g_pGameApp->GetConsole()) {
        g_pGameApp->GetConsole()->SetBackdropColor(argb);
    }
}

// Задать кодовую страницу для DrawText слота Console (MultiByteToWideChar).
// По умолчанию FontRender использует CP_ACP; Lua-строки нативно в UTF-8 (65001).
// Вызывается из console_bootstrap.lua после UI_CreateFont("Console", ...).
void Console_SetFontEncoding(int codepage) {
    if (auto* pFont = FontManager::Instance().Get(FontSlot::Console)) {
        pFont->SetCodepage(static_cast<UINT>(codepage));
    }
}

// ---- LuaBridge-адаптеры для dbc::IniSection / dbc::IniFile ----------------
// Нужны потому что нативные методы принимают std::string_view, а LuaBridge
// передаёт std::string из Lua. Лямбды конвертируют корректно и избегают
// зависимости от std::string_view в интерфейсе Lua.

std::string IniSection_GetString(const dbc::IniSection* self,
                                  const std::string& key,
                                  const std::string& def) {
    return self ? self->GetString(key, def) : def;
}

int64_t IniSection_GetInt64(const dbc::IniSection* self,
                             const std::string& key,
                             int64_t def) {
    return self ? self->GetInt64(key, def) : def;
}

std::string IniSection_GetName(const dbc::IniSection* self) {
    return self ? self->GetName() : std::string{};
}

int IniSection_ItemCount(const dbc::IniSection* self) {
    return self ? self->ItemCount() : 0;
}

dbc::IniSection* IniFile_Section(dbc::IniFile* self, const std::string& name) {
    return self ? &(*self)[name] : nullptr;
}

int IniFile_SectCount(const dbc::IniFile* self) {
    return self ? self->SectCount() : 0;
}

}  // namespace

// ---- ConsoleBridge singleton ---------------------------------------------

ConsoleBridge& ConsoleBridge::Get() {
    static ConsoleBridge s_instance;
    return s_instance;
}

void ConsoleBridge::RegisterMainThread() {
    _mainThreadId = std::this_thread::get_id();
    _mainThreadRegistered = true;
}

bool ConsoleBridge::IsMainThread(const char* where) const {
    if (!_mainThreadRegistered) {
        return true;  // До регистрации проверку не форсируем (старт клиента)
    }
    if (std::this_thread::get_id() == _mainThreadId) {
        return true;
    }
    ToLogService("lua", LogLevel::Error,
                 "console API called from non-main thread: {}", where ? where : "?");
    return false;
}

void ConsoleBridge::_RegisterLuaNamespace() {
    if (!g_LuaState) {
        return;
    }
    // Регистрируем типы dbc::IniSection и dbc::IniFile с обёрнутыми методами
    // (std::string-signature для удобной работы из Lua) + глобалы-функции
    // консоли. Не используем LuaBridge-namespace "console" — init.lua создаёт
    // свою таблицу `console` как реестр команд; namespace создавал бы
    // конфликтующую метатаблицу и приводил к AV на lua_getfield.
    luabridge::getGlobalNamespace(g_LuaState)
        .beginClass<dbc::IniSection>("IniSection")
            .addFunction("GetString",  IniSection_GetString)
            .addFunction("GetInt64",   IniSection_GetInt64)
            .addFunction("GetName",    IniSection_GetName)
            .addFunction("ItemCount",  IniSection_ItemCount)
        .endClass()
        .beginClass<dbc::IniFile>("IniFile")
            .addFunction("Section",    IniFile_Section)
            .addFunction("SectCount",  IniFile_SectCount)
        .endClass()
        .addFunction("console_print",         Console_Print)
        .addFunction("console_print_err",     Console_PrintErr)
        .addFunction("console_clear",         Console_Clear)
        .addFunction("console_set_size",           Console_SetSize)
        .addFunction("console_set_backdrop",       Console_SetBackdrop)
        .addFunction("console_set_font_encoding",  Console_SetFontEncoding);

    // Глобал `SystemIni` в Lua ссылается на g_SystemIni.
    luabridge::setGlobal(g_LuaState, &g_SystemIni, "SystemIni");
}

bool ConsoleBridge::_LoadConsoleScripts() {
    if (!g_LuaState) {
        return false;
    }
    int rc1 = luaL_dofile(g_LuaState, "scripts/lua/console/helpers.lua");
    if (rc1 != 0) {
        const char* err = lua_tostring(g_LuaState, -1);
        ToLogService("lua", LogLevel::Error, "console/helpers.lua error: {}", err ? err : "?");
        lua_pop(g_LuaState, 1);
        return false;
    }
    int rc2 = luaL_dofile(g_LuaState, "scripts/lua/console/init.lua");
    if (rc2 != 0) {
        const char* err = lua_tostring(g_LuaState, -1);
        ToLogService("lua", LogLevel::Error, "console/init.lua error: {}", err ? err : "?");
        lua_pop(g_LuaState, 1);
        return false;
    }
    ToLogService("lua", "console scripts loaded");
    return true;
}

std::string ConsoleBridge::Dispatch(std::string_view cmd) {
    if (!IsMainThread("ConsoleBridge::Dispatch")) {
        return {};
    }

    // Контракт console.dispatch(line):
    //   string → команда обработана в Lua (output, может быть пустым)
    //   nil    → команда неизвестна Lua → fallback в legacy ConsoleCallback
    if (g_LuaState) {
        try {
            luabridge::LuaRef consoleTbl = luabridge::getGlobal(g_LuaState, "console");
            if (consoleTbl.isTable()) {
                luabridge::LuaRef dispatchFn = consoleTbl["dispatch"];
                if (dispatchFn.isFunction()) {
                    luabridge::LuaResult result = dispatchFn(std::string(cmd));
                    if (!result.wasOk()) {
                        ToLogService("lua", LogLevel::Error,
                                     "console.dispatch error: {}", result.errorMessage());
                        return "[lua error] " + result.errorMessage();
                    }
                    if (result.size() > 0) {
                        luabridge::LuaRef first = result[0];
                        if (first.isString()) {
                            return first.unsafe_cast<std::string>();
                        }
                        // nil / не-строка → unhandled, идём в fallback.
                    }
                }
            }
        }
        catch (std::exception& e) {
            ToLogService("lua", LogLevel::Error, "console.dispatch exception: {}", e.what());
            return std::string{"[lua error] "} + e.what();
        }
    }

    // Fallback: legacy C++ ConsoleCallback. Возвращаемое значение НЕ читаем —
    // legacy возвращает c_str() временных std::string (use-after-free). Все
    // полезные сообщения legacy выводит напрямую через AddText().
    if (!cmd.empty()) {
        const std::string buf(cmd);  // null-terminated для C-функции legacy
        ConsoleCallback(buf.c_str());
    }
    return {};
}

bool ConsoleBridge::CanOpen() {
    if (!g_Config.m_bConsoleEnabled) {
        return false;
    }
    if (g_Config.m_bConsoleRequireSuperKey) {
        // IsPower() сейчас отражает legacy-флаг [Log].console. После логина
        // здесь будет проверяться настоящий атрибут PowerUser/GM.
        if (!g_Config.IsPower()) {
            return false;
        }
    }
    // Сценарный гейт: запрет на CLoginScene (там вводятся пароли и backtick
    // мешает), разрешено на SelectChaScene / CreateChaScene / WorldScene.
    // До создания первой сцены — считаем, что первое окно ещё не отрисовано.
    CGameScene* scene = g_pGameApp ? g_pGameApp->GetCurScene() : nullptr;
    if (!scene) {
        return false;
    }
    if (dynamic_cast<CLoginScene*>(scene) != nullptr) {
        return false;
    }
    return true;
}

void ConsoleBridge::InitLuaAPI() {
    RegisterMainThread();
    _RegisterLuaNamespace();
    _LoadConsoleScripts();
}

bool ConsoleBridge::ReloadLua() {
    return _LoadConsoleScripts();
}
