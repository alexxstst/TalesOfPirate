#pragma once

#include "GameRecordset.h"
#include "LevelRecord.h"

// Хранилище таблицы опыта персонажей на базе SQLite.
class LevelRecordStore : public GameRecordset<CLevelRecord> {
public:
	static LevelRecordStore* Instance() {
		static LevelRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "levels";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS levels (
			id    INTEGER PRIMARY KEY,
			level INTEGER NOT NULL,
			exp   INTEGER NOT NULL
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM levels ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старой коллекции
	static void Insert(SqliteDatabase& db, int id, int level, uint64_t exp) {
		try {
			EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
			auto stmt = db.Prepare("INSERT OR REPLACE INTO levels (id, level, exp) VALUES (?, ?, ?)");
			stmt.Bind(1, id);
			stmt.Bind(2, level);
			stmt.Bind(3, static_cast<int64_t>(exp));
			stmt.Step();
		} catch (const std::exception& e) {
			ToLogService("errors", LogLevel::Error, "LevelRecordStore::Insert(id={}) failed: {}", id, e.what());
		}
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CLevelRecord* GetLevelRecordInfo(int nTypeID) {
	return LevelRecordStore::Instance()->Get(nTypeID);
}
