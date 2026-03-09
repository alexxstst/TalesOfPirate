#include <format>
#include <iostream>
#include "ObjectPools.h"

namespace TalesOfPirate::Utils::Pools {

    BufferPool::BufferPool() {
        Reallocate(GetIndex(512 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 2 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 4 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 8 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 16 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 32 - sizeof(IoBuffer)));
        Reallocate(GetIndex(1024 * 64 - sizeof(IoBuffer)));
    }

    BufferPool::~BufferPool() {
        for(auto &ptr: _bufferPtr) {
            free(ptr);
        }
    }

    IoBuffer *BufferPool::Allocate(const std::uint16_t size) {
        std::lock_guard guard(_mutex);
        const auto idx = GetIndex(size);
        if (_buffers[idx].empty())
            Reallocate(idx);

        auto obj = _buffers[idx].front();
        _buffers[idx].pop_front();

#ifdef _DEBUG
	    assert(_free.contains(obj));
	    assert(!_used.contains(obj));
	    _free.erase(obj);
		_used.emplace(obj);
#endif

	    return obj;
    }

    bool BufferPool::Free(IoBuffer *buffer) {
        std::lock_guard guard(_mutex);
        const auto idx = GetIndex(buffer->size);

#ifdef _DEBUG
		if (_free.contains(buffer)) {
			std::cout << "Buffer is free!!!" << std::endl;
		}

	    if (!_used.contains(buffer)) {
		    std::cout << "Buffer is not used!!!" << std::endl;
	    }

	    assert(!_free.contains(buffer));
	    assert(_used.contains(buffer));
	    _used.erase(buffer);
	    _free.emplace(buffer);
#endif

        _buffers[idx].push_back(buffer);
        return true;
    }

    std::uint32_t BufferPool::GetUsed() const {
        return _totalCounter - GetFree();
    }

    std::uint32_t BufferPool::GetFree() const {
        std::size_t counter = 0;
        for(auto &dq:_buffers)
            counter += dq.size();

        return counter;
    }

    void BufferPool::Reallocate(std::uint32_t idx) {
        while (_buffers.size() <= idx)
            _buffers.emplace_back();

        const auto blockSize = BufferPool::GetSize(idx);
        auto ptr = (char*)malloc(blockSize * 1000);
        memset(ptr, 0, blockSize * 1000 );

        _bufferPtr.push_back(ptr);
        _totalCounter += 1000;
        for (auto i = 0; i < 1000; ++i ) {
            auto ioBuffer = (IoBuffer*)(ptr + i * blockSize);
            ioBuffer->size = blockSize - sizeof (IoBuffer);
            ioBuffer->used = 0;

            _buffers[idx].push_back(ioBuffer);
#ifdef _DEBUG
			_free.emplace(ioBuffer);
#endif
        }
    }

    std::uint32_t BufferPool::GetIndex(std::uint16_t size) {
        if (size <= 512 - sizeof(IoBuffer))
            return 0;
        else if (size <= 1024 - sizeof(IoBuffer))
            return 1;
        else if (size <= 1024 * 2 - sizeof(IoBuffer))
            return 2;
        else if (size <= 1024 * 4 - sizeof(IoBuffer))
            return 3;
        else if (size <= 1024 * 8 - sizeof(IoBuffer))
            return 4;
        else if (size <= 1024 * 16 - sizeof(IoBuffer))
            return 5;
        else if (size <= 1024 * 32 - sizeof(IoBuffer))
            return 6;
        return 7;
    }
    std::uint32_t BufferPool::GetSize(std::uint16_t index) {
        switch (index) {
        case 0: return 512;
        case 1: return 1024;
        case 2: return 1024 * 2;
        case 3: return 1024 * 4;
        case 4: return 1024 * 8;
        case 5: return 1024 * 16;
        case 6: return 1024 * 32;
        case 7: return 1024 * 64;
        default:
            throw std::runtime_error(std::format("Undefined index {}. Index must be [0..8].", index));
        }
    }

}