#pragma once

#include "GameRecordset.h"
#include "MusicRecord.h"

// Хранилище таблицы музыки/звуков на базе SQLite.
class MusicRecordStore : public GameRecordset<CMusicInfo> {
public:
	static MusicRecordStore* Instance() {
		static MusicRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "music";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS music (
			id   INTEGER PRIMARY KEY,
			name TEXT,
			type INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM music ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CMusicInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CMusicInfo* GetMusicInfo(int nTypeID) {
	return MusicRecordStore::Instance()->Get(nTypeID);
}
