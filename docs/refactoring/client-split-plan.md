# Разделение C++ Client (Game.exe): ClientUI + Game.exe

**Дата:** 2026-03-29
**Статус:** Запланировано

## Контекст

Проект Client (`sources/Client/proj/kop.vcxproj`) — монолитный Win32 EXE с ~420 файлами (210 .cpp + 210 .h). Цель: улучшить архитектуру, разделив UI-фреймворк в отдельную статическую библиотеку.

### Текущие подсистемы

| Подсистема | Файлов | Описание |
|---|---|---|
| **UI framework** | ~40 | Базовые компоненты: UIForm, UICompent, UIFormMgr, UIEdit, UICheckBox, UIGrid и т.д. |
| **UI формы** | ~75 | Игровые окна: UIEquipForm, UIChat, UIMailForm, UIGuildMgr и т.д. |
| **Сцены** | ~17 | Scene, WorldScene, LoginScene, SelectChaScene, CreateChaScene |
| **Персонаж/Актор** | ~17 | Character, CharacterModel, CharacterAction, Actor, ChaState |
| **Стейт-машина** | ~7 | STAttack, STMove, STPose, STSeat, STNpcTalk, STReadyDie |
| **Сеть** | ~11 | NetProtocol, NetIF, PacketCmd_SC/CS, Connection, NetChat, NetGuild |
| **Скрипты** | ~11 | Script, AppScript, lua_*.h, LuaInterface |
| **Эффекты/Камера** | ~14 | EffectObj, CameraCtrl, RenderStateMgr, ArcTrack |
| **Аудио** | ~9 | AudioThread, DSoundManager, BassMusicInterface |
| **Данные/Утилиты** | ~20 | GlobalVar, GameConfig, GuildData, *Set.h |
| **GameApp (ядро)** | ~8 | Main, GameApp*.cpp, GameLoading, GameMovie |

### Мостовые файлы (основной источник связности)

- **PacketCmd_SC.cpp** — 16 UI-инклудов + 5 game — обрабатывает серверные пакеты и обновляет UI-формы
- **NetProtocol.cpp** — ~30 UI-инклудов + game — протокол с прямыми вызовами UI
- **WorldScene.cpp** — 8 UI-инклудов + game — основная сцена обращается к формам
- **Character.cpp** — 8 UI-инклудов — персонаж обновляет UI напрямую
- **stdafx.h** — включает всё: UI, Scene, Character, Network, GameApp

---

## План: 2 проекта — ClientUI (Static Library) + Game.exe

### Проект 1: `ClientUI` (Static Library, ~40 файлов)

Чистый UI-фреймворк — базовые компоненты без зависимости на игровую логику.

**Файлы для переноса:**

Базовые классы:
- UIForm.cpp / .h
- UICompent.cpp / .h
- UIFormMgr.cpp / .h
- UIGuiData.cpp / .h
- UIGlobalVar.cpp / .h
- UICommandCompent.cpp / .h
- UIRender.cpp / .h

Контролы ввода:
- UIEdit.cpp / .h
- UIEditData.cpp / .h
- UIEditKey.cpp / .h
- UIEditStrategy.cpp / .h
- UICheckBox.cpp / .h
- UICheckBoxItem.cpp / .h
- UICombo.cpp / .h
- UILabel.cpp / .h
- UITextButton.cpp / .h
- UIMenu.cpp / .h
- UITitle.cpp / .h
- UIDragTitle.cpp / .h

Списки и гриды:
- UIGrid.cpp / .h
- UIGoodsGrid.cpp / .h
- UIList.cpp / .h
- UIListView.cpp / .h
- UITreeView.cpp / .h
- UIPicList.cpp / .h
- UIPage.cpp / .h
- UIScroll.cpp / .h

Графические компоненты:
- UIPicture.cpp / .h
- UIImage.cpp / .h
- UIGraph.cpp / .h
- UIProgressBar.cpp / .h
- UIRichEdit.cpp / .h
- UITextParse.cpp / .h
- UIMemo.cpp / .h

Утилиты:
- UIFont.cpp / .h
- UIImeInput.cpp / .h
- UICursor.cpp / .h
- UI3DCompent.cpp / .h
- UIStep.h
- UIEditor.cpp / .h

**Зависимости ClientUI:**
- Engine (MindPower3D) — для рендеринга GUI
- Common — для базовых типов данных (ChaAttr и т.д. если нужны для гридов)
- Windows SDK, STL

### Проект 2: `Game.exe` (Application, ~170+ файлов)

Всё остальное: игровые формы, логика, сеть, скрипты, GameApp.

**Зависимости Game.exe:**
- ClientUI (static lib)
- Engine (MindPower3D)
- Common, CorsairsNet, Status, CaLua, EncLib, ICUHelper, AudioSDL
- Все внешние библиотеки (DirectX, SDL, Lua, Crypto, Discord и т.д.)

---

## Порядок выполнения

### Этап 1: Подготовка

1. Создать `sources/Client/proj/ClientUI.vcxproj` как StaticLibrary (Win32)
   - Toolset: v145 (VS 2022)
   - C++ Standard: C++17 (`/std:c++latest`)
   - Exception handling: `/EHa`
   - RTTI: `/GR`
   - Output: `ClientUI_D.lib` / `ClientUI.lib`

2. Создать `sources/Client/src/ui/stdafx.h` для ClientUI — минимальный PCH:
   ```cpp
   #pragma once
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <stdlib.h>
   #include <malloc.h>
   #include <memory.h>
   #include <tchar.h>
   #include <string>
   #include <list>
   #include <map>
   #include <vector>
   #include <queue>
   #include <set>
   #include <algorithm>
   #include <functional>

   namespace GUI {};
   using namespace GUI;

   #include "logutil.h"
   #include "Language.h"
   #include "util.h"
   #include "GlobalInc.h"
   #include "MindPower.h"
   #include "LanguageRecord.h"
   extern CLanguageRecord g_oLangRec;

   #include "GameConfig.h"
   ```

3. Настроить include paths для ClientUI:
   - `sources/Client/src/` (основной)
   - `sources/Engine/include/`
   - `sources/Libraries/common/include/`
   - `sources/Libraries/Util/include/`

### Этап 2: Перенос файлов

4. Добавить ~40 UI-framework файлов в ClientUI.vcxproj
   - Физически файлы остаются в `sources/Client/src/` (не перемещаем)
   - Только меняем принадлежность в vcxproj

5. Убрать эти же файлы из kop.vcxproj

### Этап 3: Разрешение зависимостей

6. Проверить каждый UI-framework файл на зависимости от игровой логики:
   - Если UIGoodsGrid зависит от ItemRecord — либо вынести зависимость в интерфейс, либо оставить файл в Game.exe
   - UIGlobalVar может зависеть от GameApp — требуется анализ

7. Возможно, переместить некоторые файлы обратно в Game.exe, если зависимости слишком глубоки

### Этап 4: Обновление Game.exe

8. Добавить ClientUI как зависимость в kop.vcxproj (Project Reference или Additional Dependencies)
9. Обновить stdafx.h Game.exe — оставить как есть, добавить include path к ClientUI headers
10. Добавить `ClientUI_D.lib` / `ClientUI.lib` в линковку Game.exe

### Этап 5: Сборка и тестирование

11. Собрать ClientUI отдельно: `msbuild ClientUI.vcxproj /p:Configuration=Debug /p:Platform=Win32`
12. Собрать весь солюшен: `msbuild sources/TalesOfPirates.sln /p:Configuration=Debug /p:Platform=Win32`
13. Запустить: `Game.exe pKcfT0PcaX`
14. Проверить базовую функциональность: логин, открытие окон, чат

---

## Потенциальные проблемы

1. **UIGlobalVar** — вероятно зависит от GameApp и игровых типов. Может потребоваться оставить в Game.exe или рефакторить.
2. **UIGoodsGrid** — работает с ItemRecord/ItemAttr, может зависеть от Common/game types.
3. **UIScript** — интеграция с Lua, зависит от CaLua. Оставить в Game.exe.
4. **PCH конфликты** — разные stdafx.h в разных проектах требуют аккуратной настройки include paths.

---

## Следующий шаг (будущий)

После успешного выделения ClientUI можно рассмотреть выделение GameCore (Character, Scene, Actor, States, Effects) в третью библиотеку. Это потребует серьёзного рефакторинга мостовых файлов (PacketCmd_SC, NetProtocol, WorldScene, Character) через систему событий/callbacks.
