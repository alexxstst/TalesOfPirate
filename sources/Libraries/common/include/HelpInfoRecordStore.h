#pragma once

#include "GameRecordset.h"
#include "HelpInfoRecord.h"

// Хранилище таблицы подсказок на базе SQLite.
class HelpInfoRecordStore : public GameRecordset<CHelpInfo> {
public:
	static HelpInfoRecordStore* Instance() {
		static HelpInfoRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "help_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS help_info (
			id          INTEGER PRIMARY KEY,
			data_name   TEXT,
			help_detail TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM help_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CHelpInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

const char* GetHelpInfo(const char* pszName, const std::source_location& loc = std::source_location::current());
