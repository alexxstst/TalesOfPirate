// lwSlotMap.h
// Slot-map фиксированной ёмкости с refcount и generation-проверкой ABA.
// Пришёл на смену lwObjectPool. Основные отличия:
//   - handle = (generation:12 | slot:20), DWORD. Старый handle был просто
//     slot index, из-за чего после Unregister тот же id мог вернуться к
//   - freelist интрузивный, FIFO (через head/tail) — generation растёт
//     равномерно по слотам, минимизируя раннее переиспользование.
//   - AddRef реально учитывает ref_cnt (в lwObjectPool он молча игнорировался).
//   - GetRef/GetObj валидируют handle (generation + refcount), битый id
//     возвращает 0/ERR, а не OOB-чтение.
//   - Итерация только через ForEach(fn) — fn получает актуальный handle.
// LW_INVALID_INDEX (0xFFFFFFFF) зарезервирован как невалидный handle:
// у всех живых handle generation ∈ [1..0xFFF], slot < Capacity, поэтому
// LW_INVALID_INDEX никогда не совпадёт с живым.

#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"

LW_BEGIN

enum {
    ERR_LWISLOTMAP_POOL_FULL      = -2,
    ERR_LWISLOTMAP_INVALID_HANDLE = -3,
};

template <typename Tp, DWORD Capacity>
class lwSlotMap {
    static_assert(Capacity > 0, "lwSlotMap capacity must be > 0");
    static_assert(Capacity <= (1u << 20), "lwSlotMap capacity exceeds 20-bit slot index");

    static constexpr DWORD SLOT_BITS = 20;
    static constexpr DWORD SLOT_MASK = (1u << SLOT_BITS) - 1; // 0x000FFFFF
    static constexpr DWORD GEN_MASK  = 0x00000FFFu;           // 12 бит
    static constexpr DWORD FREE_MARK = 0xFFFFFFFFu;

    struct Slot {
        Tp       Obj{};
        uint32_t NextFree   = FREE_MARK;
        uint16_t Generation = 0;
        uint16_t RefCount   = 0;
    };

public:
    // Совместимость со старым API (используется в lwSysCharacter.cpp и т.п.).
    static constexpr DWORD POOL_SIZE = Capacity;

    lwSlotMap()
        : _free_head(0)
        , _free_tail(Capacity - 1)
        , _obj_num(0)
    {
        for (DWORD i = 0; i < Capacity; ++i) {
            _slots[i].NextFree = (i + 1 < Capacity) ? (i + 1) : FREE_MARK;
        }
    }

    ~lwSlotMap() = default;

    lwSlotMap(const lwSlotMap&) = delete;
    lwSlotMap& operator=(const lwSlotMap&) = delete;

    LW_RESULT Register(DWORD* ret_handle, Tp obj) {
        if (_obj_num == Capacity) {
            return ERR_LWISLOTMAP_POOL_FULL;
        }

        const DWORD slot = _free_head;
        Slot& s = _slots[slot];
        _free_head = s.NextFree;
        if (_free_head == FREE_MARK) {
            _free_tail = FREE_MARK;
        }

        s.Obj = obj;
        s.NextFree = FREE_MARK;

        uint16_t gen = static_cast<uint16_t>((s.Generation + 1) & GEN_MASK);
        if (gen == 0) {
            gen = 1;
        }
        s.Generation = gen;
        s.RefCount = 1;
        ++_obj_num;

        *ret_handle = _Pack(slot, gen);
        return LW_RET_OK;
    }

    // Возвращает:
    //   LW_RET_OK_1 — последний reference, объект удалён из пула (записан в ret_obj).
    //   LW_RET_OK   — decrement, объект ещё жив.
    //   ERR_*       — handle битый.
    LW_RESULT Unregister(Tp* ret_obj, DWORD handle) {
        DWORD slot;
        Slot* s = _Decode(handle, slot);
        if (!s) {
            return ERR_LWISLOTMAP_INVALID_HANDLE;
        }

        --s->RefCount;
        if (s->RefCount == 0) {
            if (ret_obj) {
                *ret_obj = s->Obj;
            }
            s->Obj = Tp{};
            s->NextFree = FREE_MARK;
            if (_free_tail == FREE_MARK) {
                _free_head = slot;
                _free_tail = slot;
            } else {
                _slots[_free_tail].NextFree = slot;
                _free_tail = slot;
            }
            --_obj_num;
            return LW_RET_OK_1;
        }
        return LW_RET_OK;
    }

    LW_RESULT AddRef(DWORD handle, DWORD ref_cnt = 1) {
        DWORD slot;
        Slot* s = _Decode(handle, slot);
        if (!s) {
            return ERR_LWISLOTMAP_INVALID_HANDLE;
        }
        const uint32_t next = static_cast<uint32_t>(s->RefCount) + ref_cnt;
        s->RefCount = (next > 0xFFFFu) ? static_cast<uint16_t>(0xFFFFu)
                                       : static_cast<uint16_t>(next);
        return LW_RET_OK;
    }

    LW_RESULT GetObj(Tp* ret_obj, DWORD handle) {
        DWORD slot;
        Slot* s = _Decode(handle, slot);
        if (!s) {
            return ERR_LWISLOTMAP_INVALID_HANDLE;
        }
        *ret_obj = s->Obj;
        return LW_RET_OK;
    }

    DWORD GetRef(DWORD handle) {
        DWORD slot;
        Slot* s = _Decode(handle, slot);
        if (!s) {
            return 0;
        }
        return s->RefCount;
    }

    DWORD GetObjNum() const { return _obj_num; }

    void Clear() {
        for (DWORD i = 0; i < Capacity; ++i) {
            _slots[i].Obj = Tp{};
            _slots[i].Generation = 0;
            _slots[i].RefCount = 0;
            _slots[i].NextFree = (i + 1 < Capacity) ? (i + 1) : FREE_MARK;
        }
        _free_head = 0;
        _free_tail = Capacity - 1;
        _obj_num = 0;
    }

    // Итерация по живым слотам. fn(DWORD handle, Tp obj).
    // Если fn возвращает bool, значение false прерывает обход (as range-break).
    template <typename Fn>
    void ForEach(Fn&& fn) {
        DWORD left = _obj_num;
        for (DWORD i = 0; i < Capacity && left > 0; ++i) {
            Slot& s = _slots[i];
            if (s.RefCount == 0) {
                continue;
            }
            --left;
            const DWORD handle = _Pack(i, s.Generation);
            if constexpr (std::is_same_v<std::invoke_result_t<Fn, DWORD, Tp>, bool>) {
                if (!fn(handle, s.Obj)) {
                    return;
                }
            } else {
                fn(handle, s.Obj);
            }
        }
    }

private:
    static constexpr DWORD _Pack(DWORD slot, DWORD gen) {
        return ((gen & GEN_MASK) << SLOT_BITS) | (slot & SLOT_MASK);
    }

    Slot* _Decode(DWORD handle, DWORD& slot) {
        if (handle == LW_INVALID_INDEX) {
            return nullptr;
        }
        slot = handle & SLOT_MASK;
        if (slot >= Capacity) {
            return nullptr;
        }
        const DWORD gen = (handle >> SLOT_BITS) & GEN_MASK;
        Slot* s = &_slots[slot];
        if (s->RefCount == 0 || s->Generation != gen) {
            return nullptr;
        }
        return s;
    }

    Slot  _slots[Capacity]{};
    DWORD _free_head;
    DWORD _free_tail;
    DWORD _obj_num;
};

using lwSlotMapVoidPtr1024  = lwSlotMap<void*,  1024>;
using lwSlotMapVoidPtr2048  = lwSlotMap<void*,  2048>;
using lwSlotMapVoidPtr4096  = lwSlotMap<void*,  4096>;
using lwSlotMapVoidPtr10240 = lwSlotMap<void*, 10240>;
using lwSlotMapVoidPtr40960 = lwSlotMap<void*, 40960>;

LW_END
