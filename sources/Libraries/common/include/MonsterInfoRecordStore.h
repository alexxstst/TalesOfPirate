#pragma once

#include "GameRecordset.h"
#include "MonsterInfoRecord.h"

// Хранилище таблицы зон монстров на базе SQLite.
class MonsterInfoRecordStore : public GameRecordset<CMonsterInfo> {
public:
	static MonsterInfoRecordStore* Instance() {
		static MonsterInfoRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "monster_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS monster_info (
			id           INTEGER PRIMARY KEY,
			data_name    TEXT,
			start_x      INTEGER,
			start_y      INTEGER,
			end_x        INTEGER,
			end_y        INTEGER,
			monster_list TEXT,
			area         TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM monster_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CMonsterInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CMonsterInfo* GetMonsterInfo(int nMapID, const std::source_location& loc = std::source_location::current());
CMonsterInfo* GetMonsterInfo(const char* pszMapName, const std::source_location& loc = std::source_location::current());
