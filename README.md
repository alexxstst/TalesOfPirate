
# TalesOfPirateDX9

[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](LICENSE)

## About This Repository

This repository contains the source code for **Tales of Pirate**, upgraded from DirectX 8 to DirectX 9. Over the past few years, we collaborated with several freelancers to modernize the engine and server infrastructure.

In response to certain groups within the community profiting from leaked versions of this code, we decided to make the source public for the benefit of all developers. Please note that we do not offer support for future bugs or technical issues.

While the core DX9 upgrade is functional, it originated from a separate development branch; you may encounter minor inconsistencies. This remains an excellent starting point for new projects. We are not responsible for any legacy bugs or exploits present in the original code, as these were inherited from previous owners.

## Architecture

The project is a hybrid **C++23 / .NET 10** codebase:

| Component | Language | Description |
|-----------|----------|-------------|
| Client | C++23 (Win32) | DirectX 9 engine (MindPower3D), game UI, rendering |
| Engine | C++23 (Win32) | MindPower3D — DX9 renderer, animation, particles, GUI (static lib) |
| GameServer | C++23 (Win32) | Combat, quests, trading, guilds, NPCs (~130 source files) |
| AccountServer | F# (.NET 10) | Authentication (Argon2id), account management |
| GateServer | F# (.NET 10) | Session management, RSA/AES encryption, proxy routing |
| GroupServer | F# (.NET 10) | Player registry, guilds, friends, chat |
| Admin.Web | C# Blazor (.NET 10) | Web admin panel (DaisyUI/Tailwind CSS) |

**Main solution:** `sources/TalesOfPirates.sln` — 27 projects (15 C++ / 12 .NET).

## Tech Stack

### C++ (Client + GameServer)

- **C++23** — VS 2026, `/std:c++latest`, Win32
- **DirectX 9** — upgraded from original DX8 foundation
- **MindPower3D** — 3D engine compiled as **static library** (.lib)
- **Lightweight ORM** — type-safe database layer for GameServer, fully eliminates SQL injections
- **Command serialization/deserialization** — structured command protocol with automatic marshaling
- **MessagePack** — binary serialization protocol replacing legacy raw binary format
- **Modern logging system** — reworked from legacy to structured, leveled logging
- **LuaJIT** (Lua 5.1 JIT) + **LuaBridge** — scripting for client and GameServer
- **Crypto++** — AES-GCM for UI textures, BLAKE2s for password hashing
- **RSA/AES** connection encryption (client/server)
- **SDL** audio (client)
- **SQLite** — game data tables stored in SQLite databases, replacing legacy binary/txt formats

### .NET 10 (Server Infrastructure)

- **Platform.Network** (F#) — async SAEA/IOCP networking, WPacket/RPacket binary protocol, AES/RSA encryption
- **Platform.Protocol** (F#) — ICommandHandler interface, CommandRouter, command dispatch
- **Platform.Database** (C#) — EF Core Code-First, multi-provider support (PostgreSQL, MySQL, MSSQL, SQLite)
- **Platform.Grpc.Contracts** (C#) — Protobuf service definitions for inter-server and admin communication
- **Platform.Shared** (F#) — Generic ServerHost, Serilog logging, endpoint configuration
- **xUnit** — test framework (72+ tests for Network layer, Gate server tests)

### Admin Panel (Blazor SSR)

- Dashboard, accounts, online players, guilds, chat broadcast, server topology
- Communicates with servers via **gRPC** (ports 15000-15002)
- DaisyUI / Tailwind CSS

## Server Ports

| Server | TCP | gRPC |
|--------|-----|------|
| AccountServer | 9958 | 15000 |
| GateServer | 1973 | 15001 |
| GroupServer | 9957 (Gate), 9956 (Game) | 15002 |
| Admin.Web | — | HTTP 5100 |

## Build

### C++ (entire solution)
```bash
msbuild sources/TalesOfPirates.sln /p:Configuration=Release /p:Platform=Win32
```

### .NET projects
```bash
dotnet build sources/Dotnet --configuration Release
```

### Run .NET servers
```bash
dotnet run --project sources/Dotnet/Servers/Account/Corsairs.AccountServer
dotnet run --project sources/Dotnet/Servers/Gate/Corsairs.GateServer
dotnet run --project sources/Dotnet/Servers/Group/Corsairs.GroupServer
dotnet run --project sources/Dotnet/Admin/Corsairs.Admin.Web
```

### Tests
```bash
dotnet test sources/Dotnet
```

### Client launch
```
Game.exe pKcfT0PcaX
```

## Gameplay Systems

- Offline Stalls and Offline Mode
- Full Mount System
- Guild Bank with transaction logs
- AuthorizedGM system
- Reworked Jackpot system
- In-game Shop (Classic style, 9 items per page)

## UI & Features

- Multi-resolution support
- 60 FPS toggle option
- HP/SP bars as values or percentages
- World Map with Zoom In/Out
- Drop Info with integrated Drop Filter
- Right-click inventory menu options
- Friend/Enemy combat modes
- Visibility toggles for Effects, Apps, and Mounts

## Scripting

- **LuaJIT** (Lua 5.1 JIT compiler) with **LuaBridge** (C++17 auto-marshaling)
- Restructured Lua folders for better organization
- Balanced Vanilla Lua setup
- Simplified file structure with intuitive variable and function names
- Modular design for easy code migration

## Security

- RSA/AES connection encryption (Client ↔ Server)
- Encrypted `.clu` files
- Core packets updated to use smart pointers
- Anti-WPE and Anti-DDoS protection
- Anti-Dupe system
- Argon2id password hashing (.NET servers)
- AES-GCM for UI texture encryption
- SQL injection protection via type-safe ORM (GameServer)

## Credits & Copyright

**Ownership & Contributions:** All rights to the engine upgrades and source modifications belong to **[SatisfyTeam](https://satisfy.live/)**, **Mothannkh**, and the private benefactors who funded the project's development but chose to remain anonymous.

**Terms of Use:** This source is provided to the community for development purposes. While we encourage innovation, we ask that users respect the work put in by the team and the contributors who made this progress possible.

**VS config file:** **[Here](https://drive.google.com/file/d/1GR8GdSNe-UocUNpkvChoHBaqZJaTcuuS/view)**
