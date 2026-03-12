#include "PacketPool.h"
#include <algorithm>
#include <cassert>

namespace net {

PacketPool& PacketPool::Shared() {
    static PacketPool instance;
    return instance;
}

PacketPool::PacketPool() {
    for (int i = 0; i < BUCKET_COUNT; ++i) {
        _buckets[i].bufSize = BUCKET_SIZES[i];
    }
}

PacketPool::~PacketPool() {
    for (int i = 0; i < BUCKET_COUNT; ++i) {
        for (auto* buf : _buckets[i].freeList) {
            delete[] buf;
        }
        _buckets[i].freeList.clear();
    }
}

int PacketPool::FindBucketIndex(int size) const {
    for (int i = 0; i < BUCKET_COUNT; ++i) {
        if (BUCKET_SIZES[i] >= size) return i;
    }
    return -1; // Больше максимального бакета
}

int PacketPool::BucketSize(int size) const {
    int idx = FindBucketIndex(size);
    return idx >= 0 ? BUCKET_SIZES[idx] : size;
}

uint8_t* PacketPool::Allocate(int size) {
    assert(size > 0);

    int idx = FindBucketIndex(size);

    if (idx >= 0) {
        auto& bucket = _buckets[idx];
        std::lock_guard<std::mutex> lock(bucket.mtx);

        if (!bucket.freeList.empty()) {
            uint8_t* buf = bucket.freeList.back();
            bucket.freeList.pop_back();
            return buf;
        }

        return new uint8_t[bucket.bufSize];
    }

    // Больше максимального бакета — аллокация напрямую
    return new uint8_t[size];
}

void PacketPool::Free(uint8_t* buf, int allocatedSize) {
    if (!buf) return;

    int idx = FindBucketIndex(allocatedSize);

    if (idx >= 0 && BUCKET_SIZES[idx] == allocatedSize) {
        auto& bucket = _buckets[idx];
        std::lock_guard<std::mutex> lock(bucket.mtx);
        bucket.freeList.push_back(buf);
        return;
    }

    // Нестандартный размер — просто удаляем
    delete[] buf;
}

} // namespace net
