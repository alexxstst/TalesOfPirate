#pragma once

#include "GameRecordset.h"
#include "EventSoundRecord.h"

// Хранилище таблицы звуков событий на базе SQLite.
class EventSoundRecordStore : public GameRecordset<CEventSoundInfo> {
public:
	static EventSoundRecordStore* Instance() {
		static EventSoundRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "event_sounds";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS event_sounds (
			id       INTEGER PRIMARY KEY,
			name     TEXT,
			sound_id INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM event_sounds ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CEventSoundInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CEventSoundInfo* GetEventSoundInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
