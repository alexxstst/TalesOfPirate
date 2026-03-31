#pragma once
#include "GameRecordset.h"
#include "NPCDataRecord.h"

class MonsterListRecordStore : public GameRecordset<NPCData> {
public:
	static MonsterListRecordStore* Instance() { static MonsterListRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "monster_list";
	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS monster_list (
			id       INTEGER PRIMARY KEY,
			name     TEXT,
			area     TEXT,
			x_pos    INTEGER,
			y_pos    INTEGER,
			map_name TEXT
		)
	)";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM monster_list ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const NPCData& r);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
