#pragma once

// GameRecordset<T> — типизированное хранилище игровых записей с загрузкой из SQLite.
//
// Использование:
//   class ItemStore : public GameRecordset<CItemRecord> {
//       CItemRecord ReadRecord(SqliteStatement& stmt) override { ... }
//   };
//
//   ItemStore store;
//   store.Load(db, "SELECT * FROM items");
//   CItemRecord* pItem = store.Get(1001);

#include <SqliteDatabase.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <chrono>
#include <logutil.h>

template <typename T>
class GameRecordset {
public:
	GameRecordset() = default;
	virtual ~GameRecordset() = default;

	GameRecordset(const GameRecordset&) = delete;
	GameRecordset& operator=(const GameRecordset&) = delete;

	// Получить запись по ID. nullptr если не найдена
	T* Get(int id) {
		auto it = _idIndex.find(id);
		if (it == _idIndex.end()) return nullptr;
		return &_records[it->second];
	}

	const T* Get(int id) const {
		auto it = _idIndex.find(id);
		if (it == _idIndex.end()) return nullptr;
		return &_records[it->second];
	}

	// Получить запись по имени. nullptr если не найдена
	T* Get(std::string_view name) {
		auto it = _nameIndex.find(std::string(name));
		if (it == _nameIndex.end()) return nullptr;
		return &_records[it->second];
	}

	const T* Get(std::string_view name) const {
		auto it = _nameIndex.find(std::string(name));
		if (it == _nameIndex.end()) return nullptr;
		return &_records[it->second];
	}

	// Количество загруженных записей
	int GetCount() const {
		return static_cast<int>(_records.size());
	}

	// Максимальный ID среди загруженных записей (аналог CRawDataSet::GetLastID)
	int GetMaxId() const {
		return _maxId;
	}

	// Загрузить записи из SQLite-запроса
	bool Load(SqliteDatabase& db, std::string_view selectQuery) {
		try {
			auto start = std::chrono::high_resolution_clock::now();

			auto stmt = db.Prepare(selectQuery);
			_records.clear();
			_idIndex.clear();
			_nameIndex.clear();

			_maxId = 0;
			while (stmt.Step()) {
				auto [id, name, record] = ReadRecord(stmt);
				size_t idx = _records.size();
				_records.push_back(std::move(record));
				_idIndex[id] = idx;
				if (id > _maxId) _maxId = id;
				if (!name.empty()) {
					_nameIndex[name] = idx;
				}
			}

			auto elapsed = std::chrono::high_resolution_clock::now() - start;
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

			ToLogService("common", "GameRecordset<{}> loaded {} records in {} ms", typeid(T).name(), _records.size(), ms);
			return true;
		}
		catch (const SqliteException& e) {
			ToLogService("errors", LogLevel::Error, "GameRecordset<{}>::Load failed: {}", typeid(T).name(), e.what());
			return false;
		}
	}

	// Обход всех записей
	template <typename Fn>
	void ForEach(Fn&& fn) {
		for (auto& record : _records) {
			fn(record);
		}
	}

	template <typename Fn>
	void ForEach(Fn&& fn) const {
		for (const auto& record : _records) {
			fn(record);
		}
	}

protected:
	// Подкласс реализует: прочитать одну запись из текущей строки SQL-запроса.
	// Возвращает {id, name, record}.
	struct RecordEntry {
		int id;
		std::string name;
		T record;
	};

	virtual RecordEntry ReadRecord(SqliteStatement& stmt) = 0;

	// Проверить, существует ли таблица tableName. Если нет — создать по createSql.
	static void EnsureCreated(SqliteDatabase& db, std::string_view tableName, std::string_view createSql) {
		try {
			auto stmt = db.Prepare("SELECT 1 FROM sqlite_master WHERE type='table' AND name=?");
			stmt.Bind(1, tableName);
			if (!stmt.Step()) {
				db.Execute(createSql);
				ToLogService("common", "GameRecordset: created table '{}'", tableName);
			}
		} catch (const SqliteException& e) {
			auto msg = std::format("GameRecordset<{}>::EnsureCreated('{}') failed: {}\nSQL: {}",
				typeid(T).name(), tableName, e.what(), createSql);
			ToLogService("errors", LogLevel::Error, "{}", msg);
		}
	}

private:
	std::vector<T> _records;
	std::unordered_map<int, size_t> _idIndex;
	std::unordered_map<std::string, size_t> _nameIndex;
	int _maxId = 0;
};
