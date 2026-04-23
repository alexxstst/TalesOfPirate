# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Language

All documentation, comments, and communication — in Russian. Technical terms and code identifiers stay in original form.

## Project Overview

Tales of Pirate — MMORPG with two codebases:
- **C++23** (VS 2022, `/std:c++latest`, Win32): client (DirectX 9 engine MindPower3D), 4 servers (Account/Gate/Group/Game)
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
- Сторонние библиотеки (`LuaJIT`, `LuaBridge`, `SDL3`, `FreeType`, `Crypto++`, `ICUHelper`, `fontstash`, `discord-rpc`, `InfoNet`) — их заголовки и реализации остаются как есть, namespace не навязываем.
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
```bash
msbuild sources/TalesOfPirates.sln /p:Configuration=Release /p:Platform=Win32
msbuild sources/TalesOfPirates.sln /p:Configuration=Debug /p:Platform=Win32
```

Individual C++ servers have batch scripts: `sources/Server/{GameServer,GateServer,GroupServer,AccountServer}/*.bat`

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
- All C++ projects use PCH via `stdafx.h` with `/FI` forced include

### C++ Servers (legacy)
- `sources/Server/{GameServer,GateServer,GroupServer,AccountServer}/`
- GameServer is the largest (~80 source files): combat, quests, trading, guilds, NPCs
- DB: ODBC via `cfl_db` library (`LIBDBC`). GameServer uses hardcoded connection string; Account/Group use DSN from config files

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

## Key Libraries (C++)
- `Common` — shared utilities (all projects depend on this)
- `LuaJIT` (`lua51.lib`) — Lua 5.1 JIT compiler, used by client and GameServer
- `LuaBridge` (header-only) — C++17 Lua binding, auto-marshaling for typed C++ functions
- `EncLib` — encryption utilities
- `Cryptopp` — Crypto++ library (AES-GCM for UI textures, BLAKE2s for password hashing)
- `InfoNet` — network transport layer
- `AudioSDL` — SDL-based audio (client only)
- `ICUHelper` — Unicode support
- `LIBDBC` — database connectivity (servers only)

## Important Caveats

- **RC files**: Edit tool corrupts GBK encoding in `.rc` files — use `sed -i` for resource file edits
- **PCH**: All 5 main C++ projects use `stdafx.h` with forced include (`/FI`). Don't add `#include "stdafx.h"` manually to source files
- **Platform**: Win32 only for C++ (no x64 configuration). .NET targets `net10.0`
- **Client launch**: `Game.exe pKcfT0PcaX` (password argument required)
- **F# compilation order**: File order matters in .fsproj — new files must be added in dependency order
- **Лицензии сторонних компонентов**: при добавлении новой библиотеки, шрифта или иного стороннего ресурса в клиент — обязательно положить текст лицензии в `Client/licenses/` (шрифты — в `Client/licenses/fonts/`) и добавить запись в `Client/licenses/README.md`. Аналогично — при удалении компонента убрать соответствующий файл и строку из README.

<!-- rtk-instructions v2 -->
# RTK (Rust Token Killer) - Token-Optimized Commands

## Golden Rule

**Always prefix commands with `rtk`**. If RTK has a dedicated filter, it uses it. If not, it passes through unchanged. This means RTK is always safe to use.

**Important**: Even in command chains with `&&`, use `rtk`:
```bash
# ❌ Wrong
git add . && git commit -m "msg" && git push

# ✅ Correct
rtk git add . && rtk git commit -m "msg" && rtk git push
```

## RTK Commands by Workflow

### Build & Compile (80-90% savings)
```bash
rtk cargo build         # Cargo build output
rtk cargo check         # Cargo check output
rtk cargo clippy        # Clippy warnings grouped by file (80%)
rtk tsc                 # TypeScript errors grouped by file/code (83%)
rtk lint                # ESLint/Biome violations grouped (84%)
rtk prettier --check    # Files needing format only (70%)
rtk next build          # Next.js build with route metrics (87%)
```

### Test (90-99% savings)
```bash
rtk cargo test          # Cargo test failures only (90%)
rtk vitest run          # Vitest failures only (99.5%)
rtk playwright test     # Playwright failures only (94%)
rtk test <cmd>          # Generic test wrapper - failures only
```

### Git (59-80% savings)
```bash
rtk git status          # Compact status
rtk git log             # Compact log (works with all git flags)
rtk git diff            # Compact diff (80%)
rtk git show            # Compact show (80%)
rtk git add             # Ultra-compact confirmations (59%)
rtk git commit          # Ultra-compact confirmations (59%)
rtk git push            # Ultra-compact confirmations
rtk git pull            # Ultra-compact confirmations
rtk git branch          # Compact branch list
rtk git fetch           # Compact fetch
rtk git stash           # Compact stash
rtk git worktree        # Compact worktree
```

Note: Git passthrough works for ALL subcommands, even those not explicitly listed.

### GitHub (26-87% savings)
```bash
rtk gh pr view <num>    # Compact PR view (87%)
rtk gh pr checks        # Compact PR checks (79%)
rtk gh run list         # Compact workflow runs (82%)
rtk gh issue list       # Compact issue list (80%)
rtk gh api              # Compact API responses (26%)
```

### JavaScript/TypeScript Tooling (70-90% savings)
```bash
rtk pnpm list           # Compact dependency tree (70%)
rtk pnpm outdated       # Compact outdated packages (80%)
rtk pnpm install        # Compact install output (90%)
rtk npm run <script>    # Compact npm script output
rtk npx <cmd>           # Compact npx command output
rtk prisma              # Prisma without ASCII art (88%)
```

### Files & Search (60-75% savings)
```bash
rtk ls <path>           # Tree format, compact (65%)
rtk read <file>         # Code reading with filtering (60%)
rtk grep <pattern>      # Search grouped by file (75%)
rtk find <pattern>      # Find grouped by directory (70%)
```

### Analysis & Debug (70-90% savings)
```bash
rtk err <cmd>           # Filter errors only from any command
rtk log <file>          # Deduplicated logs with counts
rtk json <file>         # JSON structure without values
rtk deps                # Dependency overview
rtk env                 # Environment variables compact
rtk summary <cmd>       # Smart summary of command output
rtk diff                # Ultra-compact diffs
```

### Infrastructure (85% savings)
```bash
rtk docker ps           # Compact container list
rtk docker images       # Compact image list
rtk docker logs <c>     # Deduplicated logs
rtk kubectl get         # Compact resource list
rtk kubectl logs        # Deduplicated pod logs
```

### Network (65-70% savings)
```bash
rtk curl <url>          # Compact HTTP responses (70%)
rtk wget <url>          # Compact download output (65%)
```

### Meta Commands
```bash
rtk gain                # View token savings statistics
rtk gain --history      # View command history with savings
rtk discover            # Analyze Claude Code sessions for missed RTK usage
rtk proxy <cmd>         # Run command without filtering (for debugging)
rtk init                # Add RTK instructions to CLAUDE.md
rtk init --global       # Add RTK to ~/.claude/CLAUDE.md
```

## Token Savings Overview

| Category | Commands | Typical Savings |
|----------|----------|-----------------|
| Tests | vitest, playwright, cargo test | 90-99% |
| Build | next, tsc, lint, prettier | 70-87% |
| Git | status, log, diff, add, commit | 59-80% |
| GitHub | gh pr, gh run, gh issue | 26-87% |
| Package Managers | pnpm, npm, npx | 70-90% |
| Files | ls, read, grep, find | 60-75% |
| Infrastructure | docker, kubectl | 85% |
| Network | curl, wget | 65-70% |

Overall average: **60-90% token reduction** on common development operations.
<!-- /rtk-instructions -->