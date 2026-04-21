#include "stdafx.h"
#include "GamePool.h"

#include "Entity.h"
#include "Player.h"
#include "Item.h"
#include "Character.h"
#include "NPC.h"
#include "EventEntity.h"
#include "logutil.h"

#include <vector>

namespace
{
    // Тэги в старших 8 битах handle. Значения совпадают с прежней схемой
    // defENTI_ALLOC_TYPE_* — игровой код местами сверяет lID & 0xff000000
    // с этими константами напрямую.
    constexpr uint32_t kTagPlayer    = 0x01;
    constexpr uint32_t kTagCharacter = 0x02;
    constexpr uint32_t kTagItem      = 0x03;
    constexpr uint32_t kTagTalkNpc   = 0x04;
    constexpr uint32_t kTagBerth     = 0x07;
    constexpr uint32_t kTagResource  = 0x08;

    constexpr uint32_t kSerialMask = 0x00FFFFFFu;

    // Максимум попыток подобрать свободный серийник при коллизии.
    constexpr int kMaxHandleProbes = 32;
}

GamePool& GamePool::Instance()
{
    static GamePool instance;
    return instance;
}

long GamePool::NewEntityHandle(uint32_t tag)
{
    auto& counter = _nextSerial[tag & 0xFFu];
    for (int probe = 0; probe < kMaxHandleProbes; ++probe)
    {
        uint32_t serial = (counter.fetch_add(1, std::memory_order_relaxed) + 1u) & kSerialMask;
        if (serial == 0u)
        {
            ToLogService("errors", LogLevel::Warning,
                "GamePool: handle serial wrap for tag {:#x}", tag);
            continue;
        }
        long handle = static_cast<long>((tag << 24) | serial);

        std::shared_lock lock(_indexMutex);
        if (!_entityByHandle.contains(handle))
        {
            return handle;
        }
    }

    ToLogService("errors", LogLevel::Error,
        "GamePool: exhausted handle probes for tag {:#x}", tag);
    return 0;
}

long GamePool::NewPlayerHandle(uint32_t tag)
{
    auto& counter = _nextSerial[tag & 0xFFu];
    for (int probe = 0; probe < kMaxHandleProbes; ++probe)
    {
        uint32_t serial = (counter.fetch_add(1, std::memory_order_relaxed) + 1u) & kSerialMask;
        if (serial == 0u)
        {
            ToLogService("errors", LogLevel::Warning,
                "GamePool: player handle serial wrap for tag {:#x}", tag);
            continue;
        }
        long handle = static_cast<long>((tag << 24) | serial);

        std::shared_lock lock(_indexMutex);
        if (!_playerByHandle.contains(handle))
        {
            return handle;
        }
    }

    ToLogService("errors", LogLevel::Error,
        "GamePool: exhausted player handle probes for tag {:#x}", tag);
    return 0;
}

CPlayer* GamePool::AcquirePlayer()
{
    CPlayer* p = _playerPool.Get();
    long handle = NewPlayerHandle(kTagPlayer);
    p->SetHandle(handle);

    {
        std::unique_lock lock(_indexMutex);
        _playerByHandle.emplace(handle, p);
        _alivePlayerPtrs.insert(p);
    }
    p->Initially();
    return p;
}

CCharacter* GamePool::AcquireCharacter()
{
    CCharacter* p = _chaPool.Get();
    long handle = NewEntityHandle(kTagCharacter);
    p->SetHandle(handle);

    {
        std::unique_lock lock(_indexMutex);
        _entityByHandle.emplace(handle, static_cast<Entity*>(p));
        _aliveEntityPtrs.insert(static_cast<const Entity*>(p));
    }
    p->Initially();
    return p;
}

CItem* GamePool::AcquireItem()
{
    CItem* p = _itemPool.Get();
    long handle = NewEntityHandle(kTagItem);
    p->SetHandle(handle);

    {
        std::unique_lock lock(_indexMutex);
        _entityByHandle.emplace(handle, static_cast<Entity*>(p));
        _aliveEntityPtrs.insert(static_cast<const Entity*>(p));
    }
    p->Initially();
    return p;
}

mission::CTalkNpc* GamePool::AcquireTalkNpc()
{
    mission::CTalkNpc* p = _tnpcPool.Get();
    long handle = NewEntityHandle(kTagTalkNpc);
    p->SetHandle(handle);

    {
        std::unique_lock lock(_indexMutex);
        _entityByHandle.emplace(handle, static_cast<Entity*>(p));
        _aliveEntityPtrs.insert(static_cast<const Entity*>(p));
    }
    p->Initially();
    return p;
}

mission::CEventEntity* GamePool::AcquireEventEntity(BYTE byType)
{
    mission::CEventEntity* p = nullptr;
    uint32_t tag = 0;

    switch (byType)
    {
    case mission::RESOURCE_ENTITY:
        p = _resourcePool.Get();
        tag = kTagResource;
        break;
    case mission::BERTH_ENTITY:
        p = _berthPool.Get();
        tag = kTagBerth;
        break;
    default:
        ToLogService("common", LogLevel::Error,
            "GamePool::AcquireEventEntity — unsupported type {}", byType);
        return nullptr;
    }

    long handle = NewEntityHandle(tag);
    p->SetHandle(handle);

    {
        std::unique_lock lock(_indexMutex);
        _entityByHandle.emplace(handle, static_cast<Entity*>(p));
        _aliveEntityPtrs.insert(static_cast<const Entity*>(p));
    }
    p->Initially();
    return p;
}

void GamePool::ReleasePlayer(CPlayer* player)
{
    if (!player)
    {
        return;
    }
    long handle = player->GetHandle();

    {
        std::unique_lock lock(_indexMutex);
        if (_alivePlayerPtrs.erase(player) == 0)
        {
            ToLogService("errors", LogLevel::Error,
                "GamePool::ReleasePlayer — unknown player pointer {:p}",
                static_cast<const void*>(player));
            return;
        }
        _playerByHandle.erase(handle);
    }
    _playerPool.Release(player);
}

void GamePool::ReleaseEntity(Entity* entity)
{
    if (!entity)
    {
        return;
    }
    long handle = entity->GetHandle();

    {
        std::unique_lock lock(_indexMutex);
        if (_aliveEntityPtrs.erase(entity) == 0)
        {
            ToLogService("errors", LogLevel::Error,
                "GamePool::ReleaseEntity — unknown entity pointer {:p} (handle {:#x})",
                static_cast<const void*>(entity), handle);
            return;
        }
        _entityByHandle.erase(handle);
    }

    uint32_t tag = static_cast<uint32_t>((handle >> 24) & 0xFFu);
    switch (tag)
    {
    case kTagCharacter:
        _chaPool.Release(static_cast<CCharacter*>(entity));
        break;
    case kTagItem:
        _itemPool.Release(static_cast<CItem*>(entity));
        break;
    case kTagTalkNpc:
        _tnpcPool.Release(static_cast<mission::CTalkNpc*>(entity));
        break;
    case kTagBerth:
        _berthPool.Release(static_cast<mission::CBerthEntity*>(entity));
        break;
    case kTagResource:
        _resourcePool.Release(static_cast<mission::CResourceEntity*>(entity));
        break;
    default:
        ToLogService("errors", LogLevel::Error,
            "GamePool::ReleaseEntity — unknown handle tag {:#x}", tag);
        break;
    }
}

void GamePool::ReleaseEntityByHandle(long handle)
{
    Entity* entity = FindEntity(handle);
    if (entity)
    {
        ReleaseEntity(entity);
    }
}

CPlayer* GamePool::FindPlayer(long handle) const
{
    std::shared_lock lock(_indexMutex);
    auto it = _playerByHandle.find(handle);
    return it == _playerByHandle.end() ? nullptr : it->second;
}

Entity* GamePool::FindEntity(long handle) const
{
    std::shared_lock lock(_indexMutex);
    auto it = _entityByHandle.find(handle);
    return it == _entityByHandle.end() ? nullptr : it->second;
}

bool GamePool::IsValidPlayerPtr(const CPlayer* player) const
{
    if (!player)
    {
        return false;
    }
    std::shared_lock lock(_indexMutex);
    return _alivePlayerPtrs.contains(player);
}

bool GamePool::IsValidEntityPtr(const Entity* entity) const
{
    if (!entity)
    {
        return false;
    }
    std::shared_lock lock(_indexMutex);
    return _aliveEntityPtrs.contains(entity);
}

void GamePool::ForEachPlayer(const std::function<void(CPlayer*)>& fn)
{
    std::vector<CPlayer*> snapshot;
    {
        std::shared_lock lock(_indexMutex);
        snapshot.reserve(_playerByHandle.size());
        for (auto& kv : _playerByHandle)
        {
            snapshot.push_back(kv.second);
        }
    }
    for (CPlayer* p : snapshot)
    {
        fn(p);
    }
}

void GamePool::ForEachCharacter(const std::function<void(CCharacter*)>& fn)
{
    std::vector<CCharacter*> snapshot;
    {
        std::shared_lock lock(_indexMutex);
        snapshot.reserve(_chaPool.GetUsedCount());
        for (auto& kv : _entityByHandle)
        {
            uint32_t tag = static_cast<uint32_t>((kv.first >> 24) & 0xFFu);
            if (tag == kTagCharacter)
            {
                snapshot.push_back(static_cast<CCharacter*>(kv.second));
            }
        }
    }
    for (CCharacter* p : snapshot)
    {
        fn(p);
    }
}

void GamePool::ForEachItem(const std::function<void(CItem*)>& fn)
{
    std::vector<CItem*> snapshot;
    {
        std::shared_lock lock(_indexMutex);
        snapshot.reserve(_itemPool.GetUsedCount());
        for (auto& kv : _entityByHandle)
        {
            uint32_t tag = static_cast<uint32_t>((kv.first >> 24) & 0xFFu);
            if (tag == kTagItem)
            {
                snapshot.push_back(static_cast<CItem*>(kv.second));
            }
        }
    }
    for (CItem* p : snapshot)
    {
        fn(p);
    }
}

void GamePool::ForEachTalkNpc(const std::function<void(mission::CTalkNpc*)>& fn)
{
    std::vector<mission::CTalkNpc*> snapshot;
    {
        std::shared_lock lock(_indexMutex);
        snapshot.reserve(_tnpcPool.GetUsedCount());
        for (auto& kv : _entityByHandle)
        {
            uint32_t tag = static_cast<uint32_t>((kv.first >> 24) & 0xFFu);
            if (tag == kTagTalkNpc)
            {
                snapshot.push_back(static_cast<mission::CTalkNpc*>(kv.second));
            }
        }
    }
    for (mission::CTalkNpc* p : snapshot)
    {
        fn(p);
    }
}

void GamePool::DumpLeaks(const std::string& logger)
{
    _playerPool.DumpLeaks(logger);
    _chaPool.DumpLeaks(logger);
    _itemPool.DumpLeaks(logger);
    _tnpcPool.DumpLeaks(logger);
    _berthPool.DumpLeaks(logger);
    _resourcePool.DumpLeaks(logger);
}
