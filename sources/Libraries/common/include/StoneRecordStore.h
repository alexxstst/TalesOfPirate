#pragma once

#include "GameRecordset.h"
#include "StoneRecord.h"

// Хранилище таблицы камней на базе SQLite.
class StoneRecordStore : public GameRecordset<CStoneInfo> {
public:
	static StoneRecordStore* Instance() {
		static StoneRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "stone_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS stone_info (
			id        INTEGER PRIMARY KEY,
			data_name TEXT,
			item_id   INTEGER,
			equip_pos TEXT,
			type      INTEGER,
			hint_func TEXT,
			item_rgb  INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM stone_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CStoneInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CStoneInfo* GetStoneInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
