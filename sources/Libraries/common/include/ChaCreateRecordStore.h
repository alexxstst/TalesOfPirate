#pragma once

#include "GameRecordset.h"
#include "ChaCreateRecord.h"

class ChaCreateRecordStore : public GameRecordset<CChaCreateInfo> {
public:
	static ChaCreateRecordStore* Instance() {
		static ChaCreateRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "cha_create";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS cha_create (
			id          INTEGER PRIMARY KEY,
			name        TEXT,
			bone        INTEGER,
			faces       TEXT,
			hairs       TEXT,
			bodies      TEXT,
			hands       TEXT,
			feet        TEXT,
			profession  INTEGER,
			description TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM cha_create ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CChaCreateInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CChaCreateInfo* GetChaCreateInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
