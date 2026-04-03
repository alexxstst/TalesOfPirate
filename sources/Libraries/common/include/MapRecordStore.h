#pragma once

#include "GameRecordset.h"
#include "MapRecord.h"

class MapRecordStore : public GameRecordset<CMapInfo> {
public:
	static MapRecordStore* Instance() {
		static MapRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "maps";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS maps (
			id          INTEGER PRIMARY KEY,
			data_name   TEXT,
			name        TEXT,
			init_x      INTEGER,
			init_y      INTEGER,
			light_dir   TEXT,
			light_color TEXT,
			show_switch INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM maps ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CMapInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CMapInfo* GetMapInfo(int nMapID, const std::source_location& loc = std::source_location::current());
CMapInfo* GetMapInfo(const char* pszMapName, const std::source_location& loc = std::source_location::current());
