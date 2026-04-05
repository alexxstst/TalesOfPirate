#pragma once

// TrackedPool<T> — типизированный пул объектов на базе std::pmr с диагностикой.
//
// Замена устаревшего PreAllocHeap. Каждый живой объект отслеживается,
// что позволяет находить утечки (зависшие trade/stall сессии и т.д.).
//
// Использование:
//   TrackedPool<CTradeData> pool{"TradeData"};
//   auto* p = pool.Get();          // аллокация + конструирование
//   pool.Release(p);               // деструкция + возврат в пул
//   pool.GetUsedCount();           // количество живых объектов
//   pool.DumpLeaks("trade");       // вывод в лог всех живых объектов

#include <memory_resource>
#include <unordered_set>
#include <mutex>
#include <string>
#include <functional>
#include <chrono>
#include <source_location>
#include "logutil.h"

template <typename T>
class TrackedPool {
public:
	explicit TrackedPool(std::string name)
		: _name(std::move(name)), _alloc(&_pool) {}

	~TrackedPool() {
		if (!_alive.empty()) {
			ToLogService("errors", LogLevel::Warning,
				"TrackedPool<{}> destroyed with {} leaked objects", _name, _alive.size());
		}
		_alive.clear();
	}

	// Запрет копирования и перемещения
	TrackedPool(const TrackedPool&) = delete;
	TrackedPool& operator=(const TrackedPool&) = delete;

	// Аллокация + конструирование (perfect forwarding аргументов в конструктор T)
	T* Get(const std::source_location& loc = std::source_location::current()) {
		T* ptr = nullptr;
		try {
			ptr = _alloc.allocate(1);
			std::construct_at(ptr);
		}
		catch (const std::exception& e) {
			auto msg = std::format(
				"TrackedPool<{}>::Get - exception during {}: {} (at {}:{})",
				_name, ptr ? "construct" : "allocate", e.what(),
				loc.file_name(), loc.line());
			ToLogService("errors", LogLevel::Error, "{}", msg);
			if (ptr) {
				_alloc.deallocate(ptr, 1);
			}
			throw;
		}

		std::scoped_lock lock(_diagMutex);
		_alive.insert(ptr);
		return ptr;
	}

	// Деструкция + возврат в пул
	void Release(T* ptr) {
		if (!ptr) return;

		{
			std::scoped_lock lock(_diagMutex);
			auto it = _alive.find(ptr);
			if (it == _alive.end()) {
				ToLogService("errors", LogLevel::Error,
					"TrackedPool<{}>::Release — double-free or unknown pointer {:p}", _name, static_cast<void*>(ptr));
				return;
			}
			_alive.erase(it);
		}

		ptr->~T();
		_alloc.deallocate(ptr, 1);
	}

	// Количество живых (аллоцированных и не освобождённых) объектов
	[[nodiscard]] size_t GetUsedCount() const {
		std::scoped_lock lock(_diagMutex);
		return _alive.size();
	}

	// Итерация по всем живым объектам (под мьютексом)
	void ForEachAlive(const std::function<void(T*)>& fn) {
		std::scoped_lock lock(_diagMutex);
		for (auto* ptr : _alive) {
			fn(ptr);
		}
	}

	// Вывести в лог все живые объекты как потенциальные утечки
	void DumpLeaks(const std::string& logger) {
		std::scoped_lock lock(_diagMutex);
		if (_alive.empty()) return;

		ToLogService(logger, LogLevel::Warning,
			"TrackedPool<{}> — {} живых объектов (потенциальные утечки):", _name, _alive.size());

		int i = 0;
		for (auto* ptr : _alive) {
			ToLogService(logger, LogLevel::Warning,
				"  [{}] {:p}", i++, static_cast<void*>(ptr));
		}
	}

	// Имя пула (для логов)
	[[nodiscard]] const std::string& GetName() const { return _name; }

private:
	std::pmr::synchronized_pool_resource _pool;
	std::pmr::polymorphic_allocator<T> _alloc;

	mutable std::mutex _diagMutex;
	std::unordered_set<T*> _alive;
	std::string _name;
};
