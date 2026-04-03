#pragma once

#include "GameRecordset.h"
#include "AreaRecord.h"

// Хранилище таблицы зон (areas) на базе SQLite.
class AreaRecordStore : public GameRecordset<CAreaInfo> {
public:
	static AreaRecordStore* Instance() {
		static AreaRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "areas";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS areas (
			id          INTEGER PRIMARY KEY,
			name        TEXT,
			color       INTEGER,
			music       INTEGER,
			env_color   INTEGER,
			light_color INTEGER,
			light_dir   TEXT,
			type        INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM areas ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старого CAreaSet
	static void Insert(SqliteDatabase& db, const CAreaInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CAreaInfo* GetAreaInfo(int nAreaID, const std::source_location& loc = std::source_location::current());
