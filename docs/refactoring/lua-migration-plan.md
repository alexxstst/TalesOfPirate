# Миграция Lua: CaLua → LuaBridge3

**Дата:** 2026-03-29
**Статус:** Запланировано

## Контекст

Текущая система скриптов использует CaLua — кастомную обёртку над Lua 5.0 (2003-2004), которая мертва и не поддерживается. Клиент использует Lua 5.0, сервер — Lua 5.1 (LuaJIT). Цель: унифицировать версию Lua и заменить CaLua на современную библиотеку привязок.

### Почему LuaBridge3

| Критерий | LuaBridge3 |
|---|---|
| C++ стандарт | C++17 |
| Зависимости | Нет (header-only) |
| Lua версии | 5.1–5.4, LuaJIT, Luau |
| Лицензия | MIT |
| Активность | Поддерживается (2025+) |
| Вес компиляции | Лёгкий (легче sol2) |
| Исключения | Работает и без (`-fno-exceptions`) |

GitHub: https://github.com/kunitoki/LuaBridge3

---

## Текущее состояние

### Масштаб привязок

| Где | Файл | Регистраций |
|---|---|---|
| Клиент | UIScript.cpp | 126 |
| Клиент | lua_platform.cpp | 41 |
| Клиент | SceneScript.cpp | 11 |
| Клиент | AppScript.cpp | 2 |
| Клиент | ChaScript.cpp | 4 |
| **Клиент итого** | | **~145** |
| Сервер | CharScript.cpp | 236 |
| Сервер | NpcScript.cpp | 47 |
| Сервер | lua_gamectrl.h/cpp | ~80 |
| Сервер | EntityScript.cpp | 3 |
| **Сервер итого** | | **~370** |
| **Всего** | **12 файлов** | **~510** |

### Версии Lua

- Клиент: Lua 5.0 (через CaLua, библиотеки lua50.lib + lualib.lib)
- Сервер: Lua 5.1+ (LuaJIT, библиотека lua51.lib)

### Механизм .bin скриптов

Клиентские Lua-скрипты хранятся в обфусцированном формате:
- `.clu` — исходный Lua-код (для разработки)
- `.bin` — обфусцированный (каждый байт + 23, шифр Цезаря)
- Компиляция: `Game.exe pKcfT0PcaX clu_bin` или `compiler_clu.bat`
- Загрузка: `CLU_LoadScript(file, 0)` → вычитание 23 → `luaL_loadbuffer()` → `lua_pcall()`

Механизм **не зависит от CaLua** — это ~10 строк кода, легко сохранить при миграции.

**Важно:** все `.clu`/`.bin` файлы, использующие `CLU_Call("FuncName", ...)`, нужно переписать под прямые вызовы Lua-функций.

### Ключевые файлы CaLua

- `sources/Libraries/CaLua/include/CaLua.h` — публичный API
- `sources/Libraries/CaLua/CaLua/CaLua_main.c` — реализация (init, load, register, call)
- `sources/Libraries/CaLua/CaLua/CaLua_guts.h` — внутренние структуры (VMSlate, StructForm, FuncForm)

### CaLua API (что нужно заменить)

- `CLU_Init()` — инициализация
- `CLU_LoadState(state)` — загрузка состояния Lua
- `CLU_LoadScript(filename, isObfuscate)` — загрузка/дешифрация скриптов
- `CLU_RegisterFunction(name, ret, args, convention, func)` — регистрация C-функций
- `CLU_RegisterStructure(name, desc)` — маршаллинг структур
- `CLU_CallScriptFunction()` — вызов Lua из C++

---

## Как будет выглядеть миграция

### Пример: регистрация функции

**Было (CaLua / raw Lua API):**
```cpp
lua_register(L, "appPlaySound", lua_appPlaySound);

int lua_appPlaySound(lua_State* L) {
    int nSoundNo = (int)lua_tonumber(L, 1);
    g_pGameApp->PlaySound(nSoundNo);
    return 0;
}
```

**Стало (LuaBridge3):**
```cpp
luabridge::getGlobalNamespace(L)
    .addFunction("appPlaySound", [](int soundNo) {
        g_pGameApp->PlaySound(soundNo);
    });
```

### Пример: регистрация класса

**Было (raw Lua API, сервер):**
```cpp
lua_register(L, "ChaGetLevel", lua_ChaGetLevel);
lua_register(L, "ChaAddExp", lua_ChaAddExp);
// ... 236 строк в CharScript.cpp
```

**Стало (LuaBridge3):**
```cpp
luabridge::getGlobalNamespace(L)
    .beginClass<CCharacter>("Character")
        .addFunction("GetLevel", &CCharacter::GetLevel)
        .addFunction("AddExp", &CCharacter::AddExp)
    .endClass();
```

### Пример: скрипты

**Было (.clu):**
```lua
CLU_Call("CameraRangeXY", C_NORMAL, 50, 35)
```

**Стало (.clu):**
```lua
CameraRangeXY(C_NORMAL, 50, 35)
```

---

## Порядок выполнения

### Этап 1: Унификация версии Lua (1-2 недели)

1. Выбрать целевую версию: **Lua 5.4** (или LuaJIT, если нужна производительность)
2. Собрать Lua 5.4 как статическую библиотеку (Win32)
3. Обновить клиент: заменить lua50.lib + lualib.lib на lua54.lib
4. Исправить несовместимости API Lua 5.0 → 5.4:
   - `lua_dostring()` → `luaL_dostring()`
   - `lua_open()` → `luaL_newstate()`
   - `luaopen_*()` → `luaL_openlibs()`
   - Числовая система: все числа double в 5.0, integer/float разделение в 5.4
5. Проверить, что все `.clu`/`.bin` скрипты работают на новой версии

### Этап 2: Подключение LuaBridge3 (1-2 дня)

1. Добавить LuaBridge3 headers в `sources/Libraries/LuaBridge3/`
2. Добавить include path в проекты клиента и сервера
3. Проверить компиляцию с C++17

### Этап 3: Миграция клиента (1.5-2 недели)

Файлы по приоритету:

1. **Script.cpp** — заменить `CLU_Init()` / `CLU_LoadState()` на стандартную инициализацию Lua + LuaBridge3. Сохранить функцию загрузки `.bin` (дешифрация -23 + `luaL_loadbuffer`)
2. **lua_platform.cpp** (41 регистрация) — переписать REGFN макросы на `luabridge::getGlobalNamespace`
3. **AppScript.cpp** (2 регистрации) — тривиально
4. **ChaScript.cpp клиент** (4 регистрации) — тривиально
5. **SceneScript.cpp** (11 регистраций) — просто
6. **UIScript.cpp** (126 регистраций) — самый объёмный файл клиента
7. Обновить все `.clu` файлы: заменить `CLU_Call("FuncName", ...)` на прямые вызовы `FuncName(...)`
8. Перекомпилировать `.bin` файлы через обновлённый `clu_bin`

### Этап 4: Миграция сервера (3-4 недели)

Файлы по приоритету:

1. **Script.cpp сервер** — инициализация (уже использует `luaL_newstate`, проще)
2. **lua_gamectrl.h/cpp** (~80 функций) — утилиты управления
3. **NpcScript.cpp** (47 регистраций) — пакеты и диалоги NPC
4. **EntityScript.cpp** (3 регистрации) — тривиально
5. **CharScript.cpp** (236 регистраций) — **самый большой файл**, основная работа
   - Тяжёлое использование `lua_lightuserdata` для `CCharacter*`, `Packet*`
   - Нужна аккуратная миграция на типобезопасные обёртки LuaBridge3

### Этап 5: Удаление CaLua и тестирование (1-2 недели)

1. Удалить `sources/Libraries/CaLua/` из солюшена
2. Удалить lua50.lib, lualib.lib
3. Полное тестирование:
   - Клиент: логин, создание персонажа, все UI-формы, камера, миссии
   - Сервер: скрипты NPC, квесты, боевые функции, управление персонажами
4. Проверить все `.bin` скрипты

---

## Потенциальные проблемы

1. **CLU_RegisterStructure()** — маршаллинг структур C++↔Lua. В LuaBridge3 заменяется на `beginClass<T>()`, но нужно проверить все структуры
2. **lua_lightuserdata** — сервер массово передаёт сырые указатели (`CCharacter*`, `Packet*`). LuaBridge3 поддерживает userdata с метатаблицами — безопаснее, но нужна миграция
3. **Lua 5.0 → 5.4 несовместимости** — изменения в string library, math library, метатаблицах
4. **Серверные .lua файлы** — 20+ файлов в `server/GameServer/Addons/`, могут использовать Lua 5.0/5.1 специфику

## Оценка трудоёмкости

| Этап | Срок |
|---|---|
| Унификация Lua версии | 1-2 недели |
| Подключение LuaBridge3 | 1-2 дня |
| Миграция клиента (145 биндингов) | 1.5-2 недели |
| Миграция сервера (370 биндингов) | 3-4 недели |
| Удаление CaLua + тестирование | 1-2 недели |
| **Итого** | **7-10 недель** |
