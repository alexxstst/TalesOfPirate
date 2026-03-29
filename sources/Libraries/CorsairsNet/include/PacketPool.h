#pragma once

// PacketPool      .
//  F# RangedPool.      64  65536 .
//   recv thread  game thread.

#include <cstdint>
#include <atomic>
#include <mutex>
#include <vector>

namespace net {

class PacketPool {
public:
    //   (singleton)
    static PacketPool& Shared();

    PacketPool();
    ~PacketPool();

    PacketPool(const PacketPool&) = delete;
    PacketPool& operator=(const PacketPool&) = delete;

    //   >= size .    .
    //      .
    uint8_t* Allocate(int size);

    //    . allocatedSize  ,   Allocate.
    void Free(uint8_t* buf, int allocatedSize);

    //      (   Free)
    int BucketSize(int size) const;

    //     std::cout
    void PrintStats() const;

private:
    static constexpr int BUCKET_COUNT = 11;
    static constexpr int BUCKET_SIZES[BUCKET_COUNT] = {
        64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536
    };

    struct Bucket {
        std::mutex mtx;
        std::vector<uint8_t*> freeList;
        int bufSize;
        std::atomic<int> inUseCount{0};
    };

    Bucket _buckets[BUCKET_COUNT];
    int FindBucketIndex(int size) const;
};

} // namespace net
