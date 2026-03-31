#pragma once

#include "GameRecordset.h"
#include "EventRecord.h"

class EventRecordStore : public GameRecordset<CEventRecord> {
public:
	static EventRecordStore* Instance() {
		static EventRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "events";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS events (
			id            INTEGER PRIMARY KEY,
			name          TEXT,
			event_type    INTEGER,
			arouse_type   INTEGER,
			arouse_radius INTEGER,
			effect        INTEGER,
			music         INTEGER,
			born_effect   INTEGER,
			cursor        INTEGER,
			main_cha_type INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM events ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CEventRecord& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
