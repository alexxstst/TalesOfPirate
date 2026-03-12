#pragma once

// PacketPool — потокобезопасный пул буферов для пакетов.
// Аналог F# RangedPool. Бакеты по степеням двойки от 64 до 65536 байт.
// Используется из recv thread и game thread.

#include <cstdint>
#include <mutex>
#include <vector>

namespace net {

class PacketPool {
public:
    // Глобальный пул (singleton)
    static PacketPool& Shared();

    PacketPool();
    ~PacketPool();

    PacketPool(const PacketPool&) = delete;
    PacketPool& operator=(const PacketPool&) = delete;

    // Выделить буфер >= size байт. Возвращает указатель на буфер.
    // Реальный размер аллокации — ближайший бакет.
    uint8_t* Allocate(int size);

    // Вернуть буфер в пул. allocatedSize — размер, который вернул Allocate.
    void Free(uint8_t* buf, int allocatedSize);

    // Размер бакета для данного размера (для передачи в Free)
    int BucketSize(int size) const;

private:
    static constexpr int BUCKET_COUNT = 11;
    static constexpr int BUCKET_SIZES[BUCKET_COUNT] = {
        64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536
    };

    struct Bucket {
        std::mutex mtx;
        std::vector<uint8_t*> freeList;
        int bufSize;
    };

    Bucket _buckets[BUCKET_COUNT];
    int FindBucketIndex(int size) const;
};

} // namespace net
