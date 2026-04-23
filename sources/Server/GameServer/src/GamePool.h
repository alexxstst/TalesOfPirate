// GamePool — современная замена CEntityAlloc / CPlayerAlloc.
//
// Сингтон с TrackedPool<T> на каждый тип сущности и словарями для O(1)-поиска
// по стабильному handle. Handle генерируется монотонным серийником на тип
// (старшие 8 бит — тэг типа, младшие 24 бита — серийник, при переполнении —
// повторная попытка пока слот занят). Указатели отслеживаются отдельными
// unordered_set для быстрой type-erased проверки IsValid*Ptr — помощь в поимке
// garbled pointers при x86→x64 миграции.

#ifndef GAMEPOOL_H
#define GAMEPOOL_H

#include "TrackedPool.h"

// Транзитивные заголовки — раньше приезжали через EntityAlloc.h, и заметная
// часть игрового кода опиралась на это через #include "GameApp.h".
#include "NPC.h"
#include "Item.h"
#include "EventEntity.h"
#include "Player.h"

#include <array>
#include <atomic>
#include <cstdint>
#include <functional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Entity;
class CPlayer;
class CCharacter;
class CItem;

namespace mission
{
    class CTalkNpc;
    class CEventEntity;
    class CBerthEntity;
    class CResourceEntity;
}

class GamePool
{
public:
    static GamePool& Instance();

    GamePool(const GamePool&) = delete;
    GamePool& operator=(const GamePool&) = delete;

    // Acquire — заменяет CEntityAlloc::GetNewXxx / CPlayerAlloc::GetNewPly.
    // Возвращённый объект уже получил стабильный handle и зарегистрирован в словарях.
    CPlayer*                                AcquirePlayer();
    CCharacter*                             AcquireCharacter();
    CItem*                                  AcquireItem();
    mission::CTalkNpc*                      AcquireTalkNpc();
    mission::CEventEntity*                  AcquireEventEntity(BYTE byType);

    // Release — возврат в пул. Диспатч по тэгу handle.
    void    ReleasePlayer(CPlayer* player);
    void    ReleaseEntity(Entity* entity);
    void    ReleaseEntityByHandle(long handle);

    // Поиск по handle — O(1), под shared-локом.
    CPlayer*    FindPlayer(long handle) const;
    Entity*     FindEntity(long handle) const;

    // Type-erased проверка по указателю без разыменования —
    // O(1) lookup в множестве живых указателей.
    bool    IsValidPlayerPtr(const CPlayer* player) const;
    bool    IsValidEntityPtr(const Entity* entity) const;

    // Полиморфные перегрузки — позволяют ассертам не различать тип:
    // `IsValidPtr(any_entity_or_player_ptr)`. Перегрузка разрешается
    // компилятором: Entity* и CPlayer* — разные несвязанные иерархии,
    // поэтому никаких неоднозначностей.
    bool    IsValidPtr(const Entity* entity) const   { return IsValidEntityPtr(entity); }
    bool    IsValidPtr(const CPlayer* player) const  { return IsValidPlayerPtr(player); }

    // Итерация (snapshot): копируем указатели под локом, вызываем лямбду
    // уже без лока — внутри можно свободно вызывать Acquire/Release.
    void    ForEachPlayer(const std::function<void(CPlayer*)>& fn);
    void    ForEachCharacter(const std::function<void(CCharacter*)>& fn);
    void    ForEachItem(const std::function<void(CItem*)>& fn);
    void    ForEachTalkNpc(const std::function<void(mission::CTalkNpc*)>& fn);

    // Мониторинг (количество живых объектов по типу).
    size_t  GetPlayerCount() const      { return _playerPool.GetUsedCount(); }
    size_t  GetCharacterCount() const   { return _chaPool.GetUsedCount(); }
    size_t  GetItemCount() const        { return _itemPool.GetUsedCount(); }
    size_t  GetTalkNpcCount() const     { return _tnpcPool.GetUsedCount(); }

    // Вывести все живые объекты в лог (для диагностики утечек при shutdown).
    void    DumpLeaks(const std::string& logger);

private:
    GamePool() = default;
    ~GamePool() = default;

    // Генерация стабильного handle: (tag:8) | (serial:24).
    // При исчерпании 24-битного серийника — циклический сброс + поиск
    // свободного значения по _entityByHandle / _playerByHandle.
    long    NewEntityHandle(uint32_t tag);
    long    NewPlayerHandle(uint32_t tag);

    TrackedPool<CPlayer>                        _playerPool{"Player"};
    TrackedPool<CCharacter>                     _chaPool{"Character"};
    TrackedPool<CItem>                          _itemPool{"Item"};
    TrackedPool<mission::CTalkNpc>              _tnpcPool{"TalkNpc"};
    TrackedPool<mission::CBerthEntity>          _berthPool{"Berth"};
    TrackedPool<mission::CResourceEntity>       _resourcePool{"Resource"};

    mutable std::shared_mutex                   _indexMutex;
    std::unordered_map<long, Entity*>           _entityByHandle;
    std::unordered_map<long, CPlayer*>          _playerByHandle;
    std::unordered_set<const Entity*>           _aliveEntityPtrs;
    std::unordered_set<const CPlayer*>          _alivePlayerPtrs;

    std::array<std::atomic<uint32_t>, 256>      _nextSerial{};
};

// --- Диагностические ассерты (только DEBUG) --------------------------------
// Вызываются во входах методов, принимающих Entity*/CPlayer*. Ловят
// висячие/только что освобождённые указатели, когда объект всё ещё лежит в
// чьём-то eyeshot-списке или прочей агрегации. В RELEASE — no-op, нулевая
// стоимость. Реализация через IsValidPtr (O(1), под shared-локом).
//
// Полиморфно: один макрос на оба типа, диспатч через перегрузку
// GamePool::IsValidPtr. Старые алиасы (DBG_ASSERT_ENTITY/DBG_ASSERT_PLAYER)
// оставлены — местами читаемее показать намерение.
//
// Пропускаем nullptr: вызывающий код часто ветвится на nullptr сам, и
// отстрел на null тут только спамил бы лог.
#if defined(_DEBUG)
#  define DBG_ASSERT_VALID(p) \
    do { \
        const auto* _dbg_p = (p); \
        if (_dbg_p && !GamePool::Instance().IsValidPtr(_dbg_p)) { \
            ToLogService("errors", LogLevel::Error, \
                "DBG_ASSERT_VALID failed: stale ptr {} at {}:{}", \
                static_cast<const void*>(_dbg_p), __FILE__, __LINE__); \
            assert(!"DBG_ASSERT_VALID: dangling Entity*/CPlayer*"); \
        } \
    } while (0)
#else
#  define DBG_ASSERT_VALID(p) ((void)0)
#endif

// Алиасы для совместимости/читаемости — оба раскрываются в одно и то же.
#define DBG_ASSERT_ENTITY(obj) DBG_ASSERT_VALID(obj)
#define DBG_ASSERT_PLAYER(p)   DBG_ASSERT_VALID(p)

#endif // GAMEPOOL_H
