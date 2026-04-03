#pragma once

#include "GameRecordset.h"
#include "ForgeRecord.h"

// Хранилище таблицы улучшения предметов (forge) на базе SQLite.
class ForgeRecordStore : public GameRecordset<CForgeRecord> {
public:
	static ForgeRecordStore* Instance() {
		static ForgeRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "forge";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS forge (
			id      INTEGER PRIMARY KEY,
			failure INTEGER,
			rate    INTEGER,
			money   INTEGER,
			items   TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM forge ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старого CForgeRecordSet
	static void Insert(SqliteDatabase& db, const CForgeRecord& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CForgeRecord* GetForgeRecordInfo(int nIndex, const std::source_location& loc = std::source_location::current());
