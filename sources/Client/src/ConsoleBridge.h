// ConsoleBridge — мост между игровой консолью (ConsoleProcessor) и Lua.
//
// Синглтон: клиент однопоточный, инстанс создаётся лениво через Meyers' static
// при первом обращении. Все состояние (_dispatchResult и т.п.) — члены класса,
// чтобы избежать namespace-scope глобалов со строками (thread_local std::string
// падает с AV в Debug CRT на этапе TLS-инициализации).
//
// Thread-safety:
//   * Консоль — артефакт UI-потока (WndProc → HandleChar → Dispatch).
//     RegisterMainThread() сохраняет id главного потока при инициализации.
//     Dispatch/Console_Print/Clear ассертят принадлежность главному потоку.
//     Любая попытка залогировать в консоль из сетевого/аудио/loading-потока
//     приведёт к записи в log/lua.Error вместо вывода в консоль (без падения).
//
// Роль:
//   * Dispatch()   — единая точка диспатчеризации команд. Пытается выполнить
//                    команду через Lua (console.dispatch), при неудаче передаёт
//                    в legacy C++ ConsoleCallback (GameAppMsg.cpp) для обратной
//                    совместимости в период поэтапной миграции.
//   * CanOpen()    — проверка разрешения на открытие консоли:
//                    – секция [Console] в system.ini (enabled/requireSuperKey);
//                    – активная сцена ≠ CLoginScene (не открывать на логине).
//   * InitLuaAPI() — регистрирует в Lua пространство имён `console.*`
//                    (print/print_err/clear) и загружает каркас
//                    `scripts/lua/console/*.lua`. Тут же запоминает главный
//                    поток (вызов гарантированно идёт из CScriptMgr::Init →
//                    главный поток).
//   * ReloadLua()  — hot-reload Lua-скриптов консоли без перезапуска клиента.
//
// C-совместимые thunk-функции в namespace ConsoleBridgeAPI оставлены для
// передачи в ConsoleProcessor::SetCmdHandler / SetCanOpenCheck.
#pragma once

#include <string>
#include <string_view>
#include <thread>

class ConsoleBridge {
public:
    static ConsoleBridge& Get();

    std::string Dispatch(std::string_view cmd);
    bool        CanOpen();
    void        InitLuaAPI();
    bool        ReloadLua();

    // Запомнить главный поток (вызывается из InitLuaAPI).
    void RegisterMainThread();
    // Проверка принадлежности вызывающего потока главному; при несоответствии
    // пишет лог-ошибку и возвращает false.
    bool IsMainThread(const char* where) const;

private:
    ConsoleBridge() = default;
    ConsoleBridge(const ConsoleBridge&) = delete;
    ConsoleBridge& operator=(const ConsoleBridge&) = delete;

    void _RegisterLuaNamespace();
    bool _LoadConsoleScripts();

    std::thread::id _mainThreadId{};
    bool _mainThreadRegistered = false;
};
