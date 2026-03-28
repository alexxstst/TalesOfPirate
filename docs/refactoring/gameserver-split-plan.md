# Разделение C++ GameServer на проекты

**Дата:** 2026-03-29
**Статус:** Запланировано
**Выбранный вариант:** 3 проекта (GameServerScript.lib + GameServerCore.lib + GameServer.exe)

## Контекст

GameServer — самый большой C++ проект (~81 300 строк, 58 .cpp + 65 .h). Монолитная структура затрудняет:
- Миграцию Lua-привязок (CaLua → LuaBridge3, ~370 регистраций)
- Параллельную сборку
- Изоляцию изменений при рефакторинге
- Будущую миграцию EXE → DLL для .NET хоста

## Текущая структура

### Статистика по подсистемам

| Подсистема | Файлы (.cpp) | Строк кода | Ключевые файлы |
|---|---|---|---|
| Character (ядро персонажа) | 9 | ~24 000 | Character.cpp (7730), CharacterCmd (3687), CharacterPrl (2575) |
| Script/Lua | 6+4h | ~17 000 | CharScript (6520), lua_gamectrl.h (2100), NpcScript (1174), Expand.h (5973) |
| Database | 2 | ~6 130 | GameDB (6079), TradeLogDB (51) |
| Maps/World | 7 | ~4 300 | SubMap (2097), MapRes (774), EyeshotCell (171) |
| Combat | 1 | ~2 530 | FightAble (2528) |
| Trade/Stall/Forge | 3 | ~4 940 | CharTrade (3708), CharStall (1029), CharForge (200) |
| Mission/Quests | 1 | ~2 660 | Mission (2664) |
| Network/Framework | 4 | ~4 760 | GameAppNet (1930), GameApp (1550), GameServerApp (787), gmsvr (519) |
| Entity base | 5 | ~2 500 | Entity (784), EntityAlloc (197), EntitySpawn (375), MoveAble (1063) |
| Guild | 1 | 225 | Guild.cpp |
| Auction | 2 | 294 | Auction (170), AuctionItem (124) |
| NPC/Eudemon | 2 | 711 | NPC (546), WorldEudemon (165) |
| Прочее | 15 | ~5 200 | Player (1203), StackWalker (1217), Action (331), и т.д. |

### Топ-5 самых включаемых заголовков

1. `GameAppNet.h` — **35 файлов** (сетевые макросы SENDTOCLIENT, PEEKPACKET и т.д.)
2. `Character.h` — **33 файла** (центральная сущность)
3. `GameApp.h` — **30 файлов** (класс-оркестратор CGameApp)
4. `SubMap.h` — **29 файлов** (пространственное управление)
5. `lua_gamectrl.h` — **23 файла** (скриптовый мост, 2100 строк inline-функций)

### Хорошо изолированные подсистемы

- Auction (294 строк) — минимальные связи
- TradeLogDB (51 строк) — полностью изолирован
- Weather (84 строки) — независим
- StackWalker (1217 строк) — утилита, 0 зависимостей от GameServer
- FindPath (151 строк) — чистая утилита

### Сильно связанные ядра

- `Character.*` (7 файлов + CharBoat/Forge/Stall/Trade) — ~24 000 строк, зависит от всего
- `GameApp` + `GameAppNet` — каркас, ссылается на все подсистемы
- `lua_gamectrl.h` — включается в 23 файла, содержит inline-функции с зависимостями на CCharacter, CGameApp, SubMap

---

## Рассмотренные варианты

### Вариант 1: 2 проекта (отклонён — мало пользы)

GameServerCore.lib (~73 000 строк) + GameServer.exe (~8 000 строк). Core остаётся монолитным.

### Вариант 2: 3 проекта (выбран)

Выделение скриптовой системы — прямая подготовка к миграции LuaBridge3.

### Вариант 3: 4 проекта (отклонён — слишком сложно)

Дополнительное разделение Entity/World потребует месяцы рефакторинга из-за круговых зависимостей Character ↔ SubMap ↔ GameDB.

---

## План реализации (Вариант 2)

### Целевая структура

```
GameServer.exe          (6 .cpp, ~8 000 строк)  — точка входа + каркас
  ├── GameServerCore.lib  (42 .cpp, ~56 000 строк) — игровая логика
  └── GameServerScript.lib (10 .cpp, ~17 000 строк) — Lua-система
```

**Зависимости:** `GameServer.exe → GameServerScript.lib → GameServerCore.lib`

### Распределение файлов

#### GameServerScript.lib — Lua-система

```
CharScript.cpp/h        (6 520 строк)  — 236 регистраций Lua-функций для CCharacter
NpcScript.cpp/h         (1 174 строк)  — 47 регистраций для NPC
EntityScript.cpp/h      (142 строк)    — 3 регистрации для Entity
lua_gamectrl.cpp/h      (2 624 строк)  — ~80 утилит управления из Lua
lua_gamectrl2.h         (349 строк)    — расширения lua_gamectrl
Parser.cpp/h            (227 строк)    — Lua-интерпретатор
Script.cpp/h            (171 строк)    — менеджер скриптов
Expand.h                (5 973 строк)  — макросы расширения для Lua
Mission.cpp/h           (2 869 строк)  — квестовая система (тесно связана со скриптами)
MapTrigger.cpp/h        (47 строк)     — триггеры карт (скриптовые)
```

#### GameServerCore.lib — Игровая логика

**Entity (базовые классы):**
```
Entity.cpp/h, Attachable.cpp/h, AttachManage.cpp/h
MoveAble.cpp/h, FightAble.cpp/h, EntityAlloc.cpp/h, EntitySpawn.cpp/h
Event.h, HarmRec.cpp/h, SkillState.h, SkillTemp.h
Action.cpp/h, StateCell.cpp/h
```

**Персонаж:**
```
Character.cpp/h         (8 718 строк)
CharacterAI.cpp         (14 строк)
CharacterAIState.cpp    (69 строк)
CharacterCmd.cpp        (3 687 строк)
CharacterPacket.cpp     (942 строк)
CharacterPrl.cpp        (2 575 строк)
CharacterRun.cpp        (306 строк)
CharacterSuperCmd.cpp   (1 345 строк)
```

**Игрок и NPC:**
```
Player.cpp/h            (1 591 строк)
NPC.cpp/h               (754 строк)
WorldEudemon.cpp/h      (217 строк)
EventEntity.cpp/h       (372 строк)
```

**Игровые системы:**
```
CharBoat.cpp/h          (1 591 строк) — корабли
CharForge.cpp/h         (243 строк)   — ковка
CharStall.cpp/h         (1 092 строк) — лавки
CharTrade.cpp/h         (3 965 строк) — торговля
Guild.cpp/h             (295 строк)   — гильдии
Auction.cpp/h, AuctionItem.cpp/h (348 строк) — аукцион
Item.cpp/h              (161 строк)   — предметы
```

**Карты:**
```
SubMap.cpp/h            (2 463 строк)
MapRes.cpp/h            (1 088 строк)
MapEntry.cpp/h          (343 строк)
EyeshotCell.cpp/h       (393 строк)
Birthplace.cpp/h        (111 строк)
FindPath.cpp/h          (156 строк)
```

**База данных:**
```
GameDB.cpp/h            (7 415 строк)
TradeLogDB.cpp/h        (119 строк)
```

**Ресурсы:**
```
PicSet.cpp/h, Picture.cpp/h (175 строк)
RoleData.h              (368 строк)
```

**Утилиты:**
```
xString.cpp/h, Identity.h, Range.h, MsgQueue.h, Timer.h, Usage.h
```

#### GameServer.exe — Каркас

```
GameSMain.cpp           (285 строк)   — точка входа (main)
GameApp.cpp/h           (2 341 строк) — CGameApp (оркестратор)
GameAppNet.cpp/h        (2 036 строк) — сетевая обработка
GameServerApp.cpp/h     (1 157 строк) — GameServerApp (legacy network layer)
gmsvr.cpp/h             (732 строк)   — gmsvr (IOCP network layer)
gtplayer.h              (72 строк)    — структура игрока для сети
Config.cpp/h            (697 строк)   — конфигурация сервера
Weather.cpp/h           (182 строк)   — погода
EventHandler.cpp/h      (36 строк)    — обработка событий
EventCalculate.cpp      (181 строк)   — расчёт событий
SystemDialog.cpp/h      (556 строк)   — системные диалоги
StackWalker.cpp/h       (1 406 строк) — стек вызовов (отладка)
CallStack.h             (49 строк)    — отладка
stdafx.cpp/h            (58 строк)    — PCH
```

---

### Этапы выполнения

#### Этап 1: Рефакторинг lua_gamectrl.h (2-3 дня)

**Проблема:** `lua_gamectrl.h` (2 100 строк) содержит inline-функции, которые включаются в 23 файла. Многие из них используют `CCharacter*`, `CGameApp*`, `SubMap*` — создавая тяжёлые неявные зависимости.

**Решение:**
1. Разделить `lua_gamectrl.h` на:
   - `lua_gamectrl_decl.h` — только объявления функций (без зависимостей на Character/GameApp)
   - `lua_gamectrl.cpp` — реализации (остаётся в Script.lib)
2. Файлы Core.lib будут включать только `lua_gamectrl_decl.h`
3. Убрать inline из функций, которые действительно не нужны inline

#### Этап 2: Создание GameServerCore.lib (1 день)

1. Создать `sources/Server/GameServer/Proj/GameServerCore.vcxproj`
2. Тип: Static Library (.lib), Win32
3. Перенести файлы Entity/Character/Maps/DB/Guild/Auction/Trade
4. Настроить include paths (те же, что у текущего проекта)
5. Создать отдельный `stdafx_core.h` без Lua-зависимостей

#### Этап 3: Создание GameServerScript.lib (1 день)

1. Создать `sources/Server/GameServer/Proj/GameServerScript.vcxproj`
2. Тип: Static Library (.lib), Win32
3. Перенести файлы Script/CharScript/NpcScript/EntityScript/lua_gamectrl/Parser/Mission
4. Добавить зависимость на GameServerCore.lib
5. Создать отдельный `stdafx_script.h` с Lua-зависимостями

#### Этап 4: Обновление GameServer.exe (2-3 дня)

1. Обновить `GameServer.vcxproj`:
   - Удалить перенесённые файлы
   - Добавить зависимости: GameServerCore.lib, GameServerScript.lib
   - Обновить Library Directories
2. Обновить `GameServer.sln` — добавить 2 новых проекта, настроить Build Order
3. Обновить `stdafx.h` — оставить только заголовки каркаса

#### Этап 5: Исправление include path и зависимостей (2-3 дня)

**Ожидаемые проблемы:**

1. **Круговая зависимость Character ↔ lua_gamectrl:**
   - `Character.cpp` вызывает `lua_gamectrl` функции (OnScriptTimer и т.д.)
   - `CharScript.cpp` вызывает методы `CCharacter`
   - **Решение:** Character.cpp вызывает скрипты через указатели функций, зарегистрированные при инициализации

2. **GameApp.h включает Script.h и Parser.h:**
   - **Решение:** Использовать forward declarations, убрать `#include "Script.h"` и `#include "Parser.h"` из GameApp.h

3. **Expand.h используется в нескольких файлах Core:**
   - **Решение:** Expand.h остаётся в Script.lib, Core-файлы не должны его включать напрямую. Вынести нужные определения в общий заголовок.

#### Этап 6: Тестирование и отладка (2-3 дня)

1. Собрать все 3 проекта в правильном порядке: Core → Script → EXE
2. Проверить линковку (нет ли unresolved externals)
3. Проверить что сервер запускается и обрабатывает подключения
4. Проверить Lua-скрипты (NPC-диалоги, квесты, боевые скрипты)
5. Проверить сохранение/загрузку из БД

---

## Мостовые файлы (требуют особого внимания)

| Файл | Строк | Проблема | Решение |
|---|---|---|---|
| `lua_gamectrl.h` | 2 100 | 23 потребителя, inline-функции с тяжёлыми зависимостями | Разделить на decl.h + .cpp |
| `CharScript.cpp` | 6 520 | Мост Script↔Character (236 Lua-регистраций) | Остаётся в Script.lib, зависит от Core.lib |
| `GameApp.h` | 791 | Включает Script.h, Parser.h | Forward declarations |
| `Character.cpp` | 7 730 | Вызывает lua_gamectrl функции | Указатели функций / callback-интерфейс |
| `FightAble.cpp` | 2 528 | Использует lua_gamectrl для damage-формул | Forward declarations + вызов через указатель |

---

## Оценка трудоёмкости

| Этап | Срок |
|---|---|
| Рефакторинг lua_gamectrl.h | 2-3 дня |
| Создание GameServerCore.lib | 1 день |
| Создание GameServerScript.lib | 1 день |
| Обновление GameServer.exe | 2-3 дня |
| Исправление include path | 2-3 дня |
| Тестирование и отладка | 2-3 дня |
| **Итого** | **~1.5-2 недели** |

## Связанные планы

- [client-split-plan.md](client-split-plan.md) — аналогичное разделение клиента (ClientUI.lib + Game.exe)
- [lua-migration-plan.md](lua-migration-plan.md) — миграция CaLua → LuaBridge3 (выиграет от выделения Script.lib)
