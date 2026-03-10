# TalesOfPirate — Контекст проекта

## Общее
- MMORPG клиент на C++
- Ветка разработки: `develop`
- Рабочий путь: `D:\Projects\MMORPG\TalesOfPirate\src`

## Язык
- Все комментарии, документация и ответы — **на русском языке**
- Идентификаторы кода остаются в оригинальном виде

## Архитектура анимаций атаки
- `EActionNumber` — логический ID позы (`client/Game/Include/CharacterAction.h`)
- `_BigPose2SmallPose()` — конвертирует ID в индекс движка с учётом оружия
- `CWaitAttackState::_UseSkill()` — центр запуска анимации атаки (`client/Game/Source/STAttack.cpp`)
- Путь событий: `MPEngine → CCharacter::ActionKeyFrame() → CActor → CActionState::ActionFrame()`
- `IsGlitched` — флаг 60fps-режима, влияет на velocity и blend_flag

## Ключевые файлы клиента
| Файл | Назначение |
|------|-----------|
| `client/Game/Include/CharacterAction.h` | Перечисление поз/анимаций |
| `client/Game/Include/STAttack.h` | Заголовок системы атаки |
| `client/Game/Source/STAttack.cpp` | Логика запуска анимаций атаки |
| `client/Game/Source/Character.cpp` | Основная логика персонажа |
| `client/Game/Source/CharacterModel.cpp` | Рендер и анимации модели |
| `client/Game/Source/NetIF.cpp` | Сетевой интерфейс клиента |
| `client/Game/Source/NetProtocol.cpp` | Сетевой протокол |
| `server/GameServer/Source/FightAble.cpp` | Боевая система сервера |

## Известные баги
Подробности: `memory/bugs.md`

### BUG-1 (критический) — STAttack.cpp
Лишняя `;` после `if` в `CWaitAttackState::_UseSkill()`:
```cpp
if( _pSelf->GetPoseVelocity() < per) ; // BUG
```
Эффект: анимация атаки в 60fps играет в ~6 раз быстрее.

### BUG-2 — конфликт IsGlitched
`Character::PlayPose` и `CharacterModel::PlayPose` записывают противоположные значения в `IsGlitched`.
Эффект: velocity doubling никогда не срабатывает.

## Инструкции для Claude
- В конце каждой значимой сессии сохранять итоги в `memory/bugs.md` или `memory/decisions.md`
- Перед началом работы читать этот файл и файлы из `memory/`
- Не изменять стиль кода без явной просьбы
