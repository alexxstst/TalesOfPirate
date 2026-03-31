#pragma once

#include "GameRecordset.h"
#include "HairRecord.h"

// Хранилище таблицы причёсок на базе SQLite.
class HairRecordStore : public GameRecordset<CHairRecord> {
public:
	static HairRecordStore* Instance() {
		static HairRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "hair";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS hair (
			id             INTEGER PRIMARY KEY,
			color          TEXT,
			need_items     TEXT,
			money          INTEGER,
			item_id        INTEGER,
			fail_item_ids  TEXT,
			is_cha_use     TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM hair ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	static void ParseDwordArray(std::string_view text, DWORD* out, int maxLen);
	static void ParseBoolArray(std::string_view text, bool* out, int maxLen);
};
