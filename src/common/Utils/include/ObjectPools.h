#pragma once

#include <list>
#include <deque>
#include <vector>
#include <mutex>
#include <cassert>
#include <set>

namespace TalesOfPirate::Utils::Pools {

    template <typename T>
    class ObjectPool {
        std::list<void *> _bufferPtr{};
        std::deque<T *> _pool{};
#ifdef _DEBUG
	    std::set<T*> _free{};
		std::set<T*> _used{};
	    std::recursive_mutex _lock{};
#else
	    std::mutex _lock{};
#endif

        void Reallocate() {
            auto ptr = malloc(1000 * sizeof(T));
            _bufferPtr.emplace_back(ptr);
            for (auto i = 0; i < 1000; ++i) {
				const auto objPtr = new ((char *)ptr + sizeof(T) * i) T;
                _pool.push_back(objPtr);
#ifdef _DEBUG
	            _free.emplace(objPtr);
#endif
            }
        }

    public:
        ObjectPool() {
            Reallocate();
        };

        ~ObjectPool() {
            for (auto &p : _bufferPtr) {
                free(p);
            }
        }

#ifdef _DEBUG
        std::set<T*>& GetFreeOperations() { return _free; };
        std::set<T*>& GetUsedOperations() { return _used; };
#endif

        T *Allocate() {
            std::lock_guard guard(_lock);

            if (_pool.empty())
                Reallocate();

            auto objPtr = _pool.front();
#ifdef _DEBUG
	        assert (!_used.contains(objPtr));
	        assert (_free.contains(objPtr));
#endif
            _pool.pop_front();
            objPtr->Allocate();

#ifdef _DEBUG
	        _used.emplace(objPtr);
	        _free.erase(objPtr);
#endif
            return objPtr;
        }

        bool Free(T *objPtr) {
            std::lock_guard guard(_lock);
            assert(objPtr != nullptr);

#ifdef _DEBUG
	        assert (_used.contains(objPtr));
	        assert (!_free.contains(objPtr));
	        _free.emplace(objPtr);
	        _used.erase(objPtr);
#endif
            objPtr->Free();
            _pool.push_back(objPtr);
            return true;
        }

        [[nodiscard]] std::uint32_t GetUsed() {
#ifdef _DEBUG
	        std::lock_guard guard(_lock);
	        assert (_used.size() == (_bufferPtr.size() * 1000 - GetFree()));
#endif
	        return _bufferPtr.size() * 1000 - GetFree();
        }

        [[nodiscard]] std::uint32_t GetFree() {
#ifdef _DEBUG
	        std::lock_guard guard(_lock);
	        assert (_free.size() == _pool.size());
#endif

			return _pool.size(); }
    };



    class IoBuffer {
    public:
        std::uint32_t size{};
        std::uint32_t used{};
        [[nodiscard]] char *GetData() const { return (char *)this + sizeof(IoBuffer); };
    };

    class BufferPool {
        std::list<void*> _bufferPtr{};
        std::vector<std::deque<IoBuffer*>> _buffers{};
        std::mutex _mutex{};
        std::uint32_t _totalCounter{};
#ifdef _DEBUG
		std::set<IoBuffer*> _free{};
		std::set<IoBuffer*> _used{};
#endif

        static std::uint32_t GetIndex(std::uint16_t size);
        static std::uint32_t GetSize(std::uint16_t index);
        void Reallocate(std::uint32_t idx);
    public:
        BufferPool();
        ~BufferPool();

        IoBuffer* Allocate(std::uint16_t size);
        bool Free(IoBuffer* buffer);
        [[nodiscard]] std::uint32_t GetUsed() const;
        [[nodiscard]] std::uint32_t GetFree() const;
    };

}