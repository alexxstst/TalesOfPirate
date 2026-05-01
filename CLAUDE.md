# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Language

All documentation, comments, and communication — in Russian. Technical terms and code identifiers stay in original form.

## Project Overview

Tales of Pirate — MMORPG with two codebases:
- **C++23** (VS 2026 Preview, toolset v145, `/std:c++latest`, **x64-only** since 2026-04-22): client (DirectX 9 engine MindPower3D) + GameServer. Account/Gate/Group servers were migrated out of C++ and now live only in .NET.
- **Modern .NET 10** (F# + C#): replacement server infrastructure (Corsairs.*), Blazor admin panel

**C++ standard: C++23.** Prefer modern C++ constructs: `std::string`/`std::string_view` over `char*`, `std::format` over `sprintf`, `std::filesystem` over Win32 file API, structured bindings, `auto`, range-based for, `std::optional`, `std::span`, smart pointers. Avoid raw `new`/`delete` where possible.

**Именование полей:**
- **Поля класса (class)** — всегда начинаются с `_`, далее `camelCase`: `_id`, `_name`, `_currentTrack`, `_isPlaying`. Это инкапсулированное состояние с приватным/protected доступом. Никакой венгерской нотации (`nID`, `szName`, `m_nType`).
- **Открытые поля структуры (struct)** — в `PascalCase`, без `_`. struct в нашем коде — это data bag / DTO / POD, поля публичные по умолчанию и читаются как данные, а не состояние объекта. Правильно: `struct Point { int X; int Y; };`, `struct AudioInfo { AudioType Type; std::uint32_t Code; };`. Неправильно: `struct Point { int x; int y; }` или `struct Point { int _x; int _y; }`.
- Приватные поля внутри `struct` (если они есть) — так же как у класса: `_camelCase`. Но если появляются приватные поля — скорее всего это уже класс, а не структура.

**Имена методов:** методы класса/интерфейса и свободные функции — в `PascalCase`. Это распространяется и на виртуальные методы, и на перегрузки, и на static helper'ы в namespace.
- Правильно: `Init()`, `Instance()`, `GetResourceId()`, `IsValid()`, `SetVolume(float)`.
- Неправильно: `init()`, `get_instance()`, `get_resID()`, `is_valid()`, `set_volume(int)`.
- Аббревиатуры из 2+ букв — первая заглавная, остальные строчные (`GetResId`, а не `GetResID`; `ParseUrl`, а не `ParseURL`; `HttpClient`, а не `HTTPClient`). Так же, как в .NET-части `Corsairs.*`.
- Локальные переменные, параметры функций — `camelCase` (`const int maxValue = ...`); поля — `_camelCase` (см. правило выше).
- Исключения: сигнатуры, диктуемые сторонним API (WinAPI-колбэки с `WndProc`/`LRESULT CALLBACK`; STL customization-point'ы вроде `begin()`/`end()`/`size()`/`swap()` для совместимости с range-based for и `std::swap`). Только там — остаётся snake_case/CamelCase как требует API.

**Singleton:** у всех классов-одиночек в проекте стандартизированный аксессор — `static T& Instance()` (не `GetInstance()`, не `get_instance()`, не `Get()`). Реализация — Meyers' singleton через `static T instance;` внутри функции; конструктор `private`, copy/assign — `delete`. Пример — `AssetDatabase::Instance()`, `AudioSDL::Instance()`. При рефакторинге legacy-singleton'ов (`_singleton<T>::get_instance()`, `SomeClass::getInstance()`) — приводить к `Instance()`.

**Namespace'ы:** новые классы и код при рефакторинге обязательно оборачивать в namespace с корнем `Corsairs::` — согласовано с .NET-частью проекта (`Corsairs.*`). Схема: `Corsairs::<Раздел>[::<Подраздел>]`.
- `Corsairs::Client::*` — клиент (`sources/Client/`, `sources/Libraries/AudioSDL/`, и т.п.): `Corsairs::Client::Audio`, `Corsairs::Client::Net`, `Corsairs::Client::Ui`, `Corsairs::Client::Lua`.
- `Corsairs::Engine::*` — движок MindPower3D (`sources/Engine/`): `Corsairs::Engine::Render`, `Corsairs::Engine::Dx10`, `Corsairs::Engine::Animation`, `Corsairs::Engine::Font`, `Corsairs::Engine::Particle`.
- `Corsairs::Server::<Name>` — серверы (`sources/Server/<Name>Server/`): `Corsairs::Server::Game`, `Corsairs::Server::Gate`, `Corsairs::Server::Group`, `Corsairs::Server::Account`.
- `Corsairs::Common::*`, `Corsairs::Util::*` — общие библиотеки (`sources/Libraries/Common/`, `sources/Libraries/Util/`).

Пример: `sources/Libraries/AudioSDL/` → `Corsairs::Client::Audio`.

Стиль (C++23):
- Nested-namespace: `namespace Corsairs::Client::Audio { ... }`.
- Закрывающую скобку комментировать: `} // namespace Corsairs::Client::Audio`.
- В `.h` и соответствующем `.cpp` — один и тот же namespace, оба файла обёрнуты.
- **Никаких `using namespace ...` в глобальной области** `.h`-файлов. В `.cpp` — допустимо, но лучше квалификация по месту или локальный `using` внутри функции.

Исключения (не оборачивать):
- Сторонние библиотеки (`LuaJIT`, `LuaBridge`, `SDL3`, `SDL3_mixer`, `FreeType`, `Crypto++`, `fontstash`, `discord-rpc`, `stb_image`, `sqlite3`) — их заголовки и реализации остаются как есть, namespace не навязываем.
- Чистые C API (WinAPI, DirectX, ODBC, OpenSSL) — без обёртки.
- Legacy-код, не затронутый текущим рефакторингом, — не переносить массово в namespace; только при касании соседнего кода, согласованно с правилом про поля `_` и фиксированные типы.

**Защита заголовков:** в новых `.h` и при рефакторинге старых — использовать `#pragma once` в самом верху файла. C-style include guards (`#ifndef __FOO_H__ / #define __FOO_H__ / #endif`) — не применять.
- `#pragma once` поддерживается всеми актуальными компиляторами проекта (MSVC, clang, gcc), короче, и нельзя сломать опечаткой в символе guard'а или коллизией имён при копипасте.
- Исключение — сторонние заголовки (их не трогаем в принципе).

**Рефакторинг C-строк:** при рефакторинге и исправлении ошибок заменять C-функции на C++ аналоги:
- `strcpy`/`strncpy` → `std::string` присваивание или `.assign()`
- `strcmp`/`strncmp` → `==`, `!=`, `.compare()` или `std::string_view`
- `sprintf`/`snprintf` → `std::format`
- `strlen` → `.size()` / `.length()`
- `strcat` → `+=` или `std::string::append()`
- `atoi`/`atof` → `std::stoi`/`std::stof` или `std::from_chars`

**Целочисленные типы:** при рефакторинге и в новом коде — фиксированной ширины из `<cstdint>` вместо платформно-зависимых и Windows-typedef'ов.
- `int`, `long`, `short`, `unsigned int`, `unsigned long` → `std::int32_t` / `std::uint32_t` / `std::int64_t` / `std::uint64_t` / `std::int16_t` / `std::uint16_t` / `std::int8_t` / `std::uint8_t`
- `DWORD`, `ULONG`, `UINT` → `std::uint32_t`
- `LONG`, `INT` → `std::int32_t`
- `WORD`, `USHORT` → `std::uint16_t`
- `BYTE`, `UCHAR` → `std::uint8_t`
- `QWORD`, `ULONGLONG` → `std::uint64_t`
- `LONGLONG` → `std::int64_t`
- `size_t` → `std::size_t`, `ptrdiff_t` → `std::ptrdiff_t`, `intptr_t`/`uintptr_t` → `std::intptr_t`/`std::uintptr_t`
- Для handles и typed IDs предпочитать явные `std::uint32_t`/`std::uint64_t` вместо `DWORD`/`ULONGLONG` — скрытая ширина `DWORD` уже один раз привела к багу усечения указателя на x64 (см. `x64-playerptr-fix`).

Исключения (оставлять как есть):
- `bool`, `char` в `char*`/`std::string`, `wchar_t`, `float`, `double`;
- счётчики `for`-циклов без сохранения;
- **параметры и возвращаемые значения WinAPI** — передавать и принимать то, что требует API (`WPARAM`/`LPARAM`/`HRESULT`/`DWORD` в колбэках), но собственные поля класса и внутренние переменные — фиксированной ширины;
- legacy-места, не затронутые текущим рефакторингом, — не менять массово, только при касании соседнего кода.

**Перечисления:** в новом коде и при рефакторинге — **только `enum class`**, без plain `enum`. Обязательно с фиксированным underlying-типом из `<cstdint>` (`std::uint32_t` / `std::int32_t` / и т.п.).
- Правильно: `enum class TextureLoadMode : std::uint32_t { LOAD_TEXTURE_DDS = 0, LOAD_TEXTURE_USER_IMAGE = 1 };`
- Неправильно: `enum TextureLoadMode { ... };` (нет scope, неявная конверсия в int, underlying-тип неопределён).
- Plain enum допустим только в legacy-коде, который мы не трогаем (например, соседние `lwTexInfoTypeEnum`, `lwResStateEnum` и т.п. — переписывать массово не нужно). При расширении такого enum — на месте конвертировать в `enum class`, если затрагиваем большую часть колсайтов.
- Имена значений — `SCREAMING_CASE` с осмысленным префиксом (`LOAD_TEXTURE_*`, `TEX_TYPE_*`). Сам тип — `PascalCase` без `lw`/`Enum`-суффиксов: `TextureLoadMode`, не `lwTextureLoadModeEnum`.
- Колсайт обязан квалифицировать значение: `TextureLoadMode::LOAD_TEXTURE_DDS`. Никаких `using enum ...` в глобальной области заголовка.

**Обработка исключений:** `try { ... } catch (...) { ... }` — **только по явному согласованию с пользователем**. Самостоятельно оборачивать вызов в `try/catch` не нужно; пусть исключение пробрасывается и процесс падает. Подавление исключений прячет битые данные/ресурсы и приводит к тихим регрессиям. Исключение — если catch реально нужен для отката ресурса или перевода одного типа ошибки в другой, и это согласовано.

**Замер времени и таймауты:** в новом коде и при рефакторинге — `std::chrono` вместо `GetTickCount`/`timeGetTime`/`QueryPerformanceCounter` напрямую и вместо `DWORD`-таймстампов.
- Моменты времени — `std::chrono::steady_clock::time_point` (`steady_clock::now()`); для wall-clock — `system_clock`.
- Длительности — `std::chrono::milliseconds` / `std::chrono::seconds` / `std::chrono::microseconds`. Никаких `DWORD timeoutMs = 400;`.
- Сравнение/арифметика через chrono-операторы: `(now - start) > 400ms`, `start + 1s` и т.п.
- Sleep — `std::this_thread::sleep_for(...)` вместо `Sleep(N)`.
- Sentinel «не задано» — `time_point{}` (default-constructed = epoch); проверка `if (tp == steady_clock::time_point{})`.
- Исключение: callbacks WinAPI и legacy-API, требующие `DWORD` напрямую, — там оставляем как есть.

**Тайминги бизнес-логики — по времени, а не по кадрам.** Любой long-press, blink, cooldown, debounce, animation timing, ttl и т.п. должен считаться от `std::chrono`-таймстампов, не от инкремента счётчика каждый кадр. Frame-counter таймауты на 30 FPS превращаются в 2.4× более быстрые на 144 FPS — поведение ломается с переключением FPS. Если встречаешь паттерн `++counter; if (counter > N)` для timeout/blink — это **bug**, переписывай на `(now - start) > Nms`. Исключение — счётчики, которые семантически про **кадры**, а не про **время** (например, индекс анимационного кадра в спрайте), — там FPS-нормализация делается через `SteadyFrameSync::GetAnimMultiplier()`.

**Форматирование:** Однострочные `if` без фигурных скобок запрещены. Всегда использовать скобки и переносы:
```cpp
// Неправильно:
if (result.empty()) return false;

// Правильно:
if (result.empty()) {
    return false;
}
```

**Цепочки `.SetParam()`:** Каждый параметр на отдельной строке:
```cpp
// Правильно:
_db.CreateCommand("UPDATE character SET level = ? WHERE atorID = ?")
    .SetParam(1, level)
    .SetParam(2, chaId)
    .ExecuteNonQuery();
```

Main solution: `sources/TalesOfPirates.sln` — contains both C++ and .NET projects.

## Build Commands

### C++ (entire solution)

Main solution: `sources/talesofpirates.sln`. Platform is **x64-only** (no Win32 configuration). Default to Debug; build Release only when explicitly asked.

```bash
# MSBuild lives in VS 2026 Preview:
#   /c/Program\ Files/Microsoft\ Visual\ Studio/18/<edition>/MSBuild/Current/Bin/MSBuild.exe
msbuild sources/talesofpirates.sln /p:Configuration=Debug   /p:Platform=x64
msbuild sources/talesofpirates.sln /p:Configuration=Release /p:Platform=x64
```

Individual project `.vcxproj` files:
- Client: `sources/Client/Game.vcxproj`
- Engine: `sources/Engine/MindPower3D.vcxproj`
- GameServer: `sources/Server/GameServer/GameServer.vcxproj`

### .NET projects
```bash
# Build all .NET projects
dotnet build sources/Dotnet --configuration Release

# Build specific server
dotnet build sources/Dotnet/Servers/Account/Corsairs.AccountServer
dotnet build sources/Dotnet/Servers/Gate/Corsairs.GateServer
dotnet build sources/Dotnet/Servers/Group/Corsairs.GroupServer
dotnet build sources/Dotnet/Admin/Corsairs.Admin.Web
```

### Run .NET servers
```bash
dotnet run --project sources/Dotnet/Servers/Account/Corsairs.AccountServer  # TCP:9958, gRPC:15000
dotnet run --project sources/Dotnet/Servers/Gate/Corsairs.GateServer        # TCP:1973, gRPC:15001
dotnet run --project sources/Dotnet/Servers/Group/Corsairs.GroupServer      # Gate TCP:9957, Game TCP:9956, gRPC:15002
dotnet run --project sources/Dotnet/Admin/Corsairs.Admin.Web                # HTTP:5100
```

### Tests
```bash
# All .NET tests
dotnet test sources/Dotnet

# Specific test project
dotnet test sources/Dotnet/Shared/Corsairs.Platform.Network.Tests
dotnet test sources/Dotnet/Servers/Gate/Test/Corsairs.GateServer.Tests
```

Test framework: xUnit. No C++ test projects.

## Architecture

### C++ Client
- Entry: `sources/Client/src/Main.cpp` → `GameApp` class
- Game loop: `GameAppInit.cpp`, `GameAppFrameMove.cpp`, `GameAppRender.cpp`
- Engine: `sources/Engine/` (MindPower3D — DirectX 9 renderer, animation, particles, GUI)
- Network: `NetProtocol.cpp`, `PacketCmd*.cpp`, `Connection.cpp`
- State machine: `STAttack.cpp`, `STMove.cpp` (attack/movement states)
- Character system: `Character.cpp` → `CharacterModel.cpp` → `CharacterAction.cpp`
- Input: `Corsairs::Engine::Input::InputSystem` поверх `WM_KEY*` / `WM_MOUSE*` / `WM_CHAR` (DirectInput 8 снят 2026-04-24)
- Textures: `TextureLoader` + `TextureCache` (stb_image для BMP/TGA/PNG, ручной `DdsLoader` для DDS)
- Entity pooling: `GamePool` + `lwSlotMap<T, Capacity>` со стабильными handle'ами `(gen:12|slot:20)` / `(tag:8|serial:24)` — заменили raw-указатели после x86→x64
- PCH: все C++ проекты используют `stdafx.h` с `/FI` (ForcedIncludeFiles). **Не добавляйте `#include "stdafx.h"` вручную в `.cpp`.**

### C++ Server (GameServer — единственный C++ сервер)
- `sources/Server/GameServer/` — combat, quests, trading, guilds, NPCs (~130 source files)
- DB: ODBC, hardcoded connection string `DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;DATABASE=gamedb;Trusted_Connection=Yes;` в `GameDB.cpp` / `TradeLogDB.cpp`
- Кодировка: MSSQL и сервер переведены на UTF-8 (миграция закрыта 2026-04-19, утилиты — `Libraries/Util/EncodingUtil.h`)
- Static data: читает общий `server/GameServer/gamedata.sqlite` через `AssetDatabase::Instance()` (см. ниже)

### .NET Server Infrastructure (Corsairs.*)
- **Platform.Network** (F#): SAEA IOCP networking, WPacket/RPacket binary protocol, AES/RSA encryption, TcpListener/Connector
- **Platform.Protocol** (F#): ICommandHandler interface, CommandRouter, PacketBuilder — command dispatch
- **Platform.Shared** (F#): Generic ServerHost, Serilog logging, endpoint config
- **Platform.Database** (C#): EF Core Code-First with multi-provider support (PostgreSQL, MySQL, MSSQL, SQLite)
- **Platform.Grpc.Contracts** (C#): Protobuf service definitions for inter-server and admin communication

### .NET Server Pattern
Each server follows the same architecture:
- `Program.fs` — entry point with `ServerHost` setup
- `Handlers/` — command handlers implementing `ICommandHandler`
- `Services/` — business logic and hosted services
- `Grpc/` — gRPC admin service implementation
- GateServer uses two system types: `DirectSystemCommand` (event-based, for GameServerSystem) and `ChannelSystemCommand` (poll-based, for ClientSystem)

### Admin.Web
- Blazor SSR with DaisyUI/Tailwind CSS
- Pages: Dashboard, Accounts, Online Players, Guilds, Chat Broadcast, Server Topology
- Communicates with servers via gRPC (ports 15000-15002)

### Attack Chain (key flow for combat debugging)
```
Client: mouse → ActAttackCha → CAttackState → SwitchState → Cmd_BeginSkill → server
Server: Cmd_BeginMove+Cmd_BeginSkill → CAction::Add → DesireMoveBegin+DesireFightBegin
Server → Client: CMD_MC_NOTIACTION (skill rep) or CMD_MC_FAILEDACTION
Client: NetActorSkillRep → state->ServerEnd(sState) / NetFailedAction → FailedAction
```

## Static Data Store

**`server/GameServer/gamedata.sqlite`** — общая БД статических данных (клиент + сервер, dev-окружение). Точка доступа: `AssetDatabase::Instance()`. Сюда постепенно переезжают все `.txt` / `.bin` из `server/GameServer/resource/` (миграция Lua→stores закрыта 2026-04-22; 53 корневых таблицы перенесены в `old.table/`). Для новых «таблиц» — создавать store (пример: `NpcRecordStore`, `ChaNameFilterStore`) с чтением через `AssetDatabase::Instance().GetDb()`. При миграции легаси-таблицы — **всегда в 2 этапа**: (1) стор + подключение к легаси-парсеру для переноса данных; (2) удаление легаси-пути после фактического импорта.

Клиентский `render.sqlite` (SQLAR + метаданные 3D/текстур) — в проработке, план в `memory/render-assets-plan.md`.

## Key Libraries (C++)

Все живут в `sources/Libraries/`:

- `common` — shared utilities (все проекты зависят; Windows SDK, PCH, базовые типы)
- `Util` — общие утилиты (`EncodingUtil.h` UTF-8, и т.п.)
- `CorsairsNet` — сетевой транспорт клиента и GameServer (заменил legacy `InfoNet`)
- `LuaJIT` (`lua51.lib`) — Lua 5.1 JIT, клиент + GameServer
- `LuaBridge` (header-only) — C++ Lua binding, auto-marshaling
- `Cryptopp` — Crypto++ (AES-GCM для UI-текстур, BLAKE2s для хешей паролей)
- `AudioSDL` — аудио-обёртка клиента поверх **SDL3 3.4.4 + SDL3_mixer 3.2.0** (миграция 2026-04-23, track-модель API). OGG Vorbis — через встроенный stb_vorbis, `libogg` не нужен.
- `SDL3` — заголовки/импортные либы SDL3
- `FreeType` + `fontstash` — multi-page glyph-атлас для UI-шрифтов (DX9-backend; план в `memory/font-render-freetype-plan.md`)
- `sqlite3` — SQLite (для `AssetDatabase`, future `render.sqlite`)
- `Discord` — discord-rpc DLL
- `DirectX` — заголовки/либы DirectX 9. **Не включать `sources/Libraries/DirectX/include` на x64** — даёт C2733 из-за конфликта `winnt.h` со свежим Windows SDK (см. `memory/x64_migration_plan.md`)

## Important Caveats

- **Platform**: C++ сборки — **x64 only** (Win32-конфиги удалены из солюшена 2026-04-22). .NET таргетит `net10.0`.
- **RC files**: Edit tool ломает GBK-кодировку в `.rc` файлах — актуальный из C++ проектов `GameServer.rc`. Для правок ресурсников использовать `sed -i`.
- **PCH**: все три C++ проекта (`kop`, `MindPower3D`, `GameServer`) используют `stdafx.h` через `ForcedIncludeFiles` (`/FI`). **Не добавляйте `#include "stdafx.h"` в `.cpp` вручную.**
- **Client launch**: `Game.exe pKcfT0PcaX` (password argument required). Артефакты: `Client/system/Game.exe` (Debug) и `sources/Client/bin/system/Game.exe` (Release). SDL3 DLL (`SDL3.dll`, `SDL3_mixer.dll`) должны лежать рядом.
- **F# compilation order**: порядок файлов в `.fsproj` значим — новые файлы добавлять в порядке зависимостей.
- **sqlcmd из git-bash**: вызывать через `bash -c "unset SQLCMDUSER SQLCMDPASSWORD; ..."` + `cygpath -w` для `-i`; SSPI — флагом `-E`. См. `memory/feedback_mssql_sqlcmd.md`.
- **Лицензии сторонних компонентов**: при добавлении новой библиотеки/шрифта/ресурса в клиент — положить текст лицензии в `Client/licenses/` (шрифты — в `Client/licenses/fonts/`), добавить запись в `Client/licenses/README.md`. При удалении — убрать файл и строку из README. Тексты лицензий **только скачивать** (curl/cp), не генерировать.

<!-- rtk-instructions v2 -->
# RTK (Rust Token Killer) — Token-Optimized Commands

**Golden rule**: префиксуй `rtk` к командам, у которых есть фильтр. Если фильтра нет — `rtk` просто проксирует, так что вызов остаётся безопасным. Префиксовать нужно **каждое** звено в цепочках `&&`:

```bash
# ❌
git add . && git commit -m "msg" && git push
# ✅
rtk git add . && rtk git commit -m "msg" && rtk git push
```

Стек этого проекта — C++ (MSBuild) + .NET (F#/C#) + SQLite + Lua. Значимые RTK-фильтры для него:

### Git (59–80% savings)
```bash
rtk git status | log | diff | show | add | commit | push | pull | branch | fetch | stash | worktree
```
Passthrough работает для **всех** git-subcommand'ов, даже не перечисленных.

### GitHub (26–87% savings)
```bash
rtk gh pr view <num>    # Compact PR view
rtk gh pr checks
rtk gh run list
rtk gh issue list
rtk gh api
```

### Files & Search (60–75% savings)
```bash
rtk ls <path>      # Tree format, compact
rtk read <file>
rtk grep <pattern>
rtk find <pattern>
```

### Analysis & Debug (70–90% savings)
```bash
rtk err <cmd>          # Только ошибки из stdout/stderr
rtk log <file>         # Дедуп логов с счётчиками
rtk json <file>        # Структура JSON без значений
rtk summary <cmd>      # Умный summary вывода
rtk diff               # Ultra-compact diff
```

### Network (65–70% savings)
```bash
rtk curl <url>
rtk wget <url>
```

### Meta
```bash
rtk gain [--history]   # Статистика экономии токенов
rtk discover           # Поиск упущенных мест применения RTK в сессии
rtk proxy <cmd>        # Прогнать без фильтра (для дебага)
```

**Неприменимо к этому проекту** (и поэтому не используется): `rtk cargo`, `rtk tsc`, `rtk lint`, `rtk prettier`, `rtk next`, `rtk vitest`, `rtk playwright`, `rtk pnpm`, `rtk npm`, `rtk npx`, `rtk prisma`, `rtk docker`, `rtk kubectl`. Для сборки здесь — MSBuild (C++) и `dotnet build`/`dotnet test` (F#/C#); у них RTK-фильтров нет, пишем команды напрямую.
<!-- /rtk-instructions -->