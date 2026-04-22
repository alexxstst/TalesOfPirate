#pragma once

// Хранилище запрещённых подстрок имён персонажей (cha_name_filters).
//
// Использование:
//   ChaNameFilterStore::Instance()->Load(db);
//   if (store->GetCount() == 0) {
//       // legacy-fallback: прочитать ChaNameFilter.txt и залить через Insert()
//   }
//   store->ForEach([](const ChaNameFilterRecord& r) {
//       CTextFilter::Add(CTextFilter::NAME_TABLE, r._pattern);
//   });

#include "GameRecordset.h"
#include "ChaNameFilterRecord.h"
#include <string_view>

class ChaNameFilterStore : public GameRecordset<ChaNameFilterRecord> {
public:
	static ChaNameFilterStore* Instance() {
		static ChaNameFilterStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "cha_name_filters";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS cha_name_filters (
			id      INTEGER PRIMARY KEY AUTOINCREMENT,
			pattern TEXT NOT NULL UNIQUE
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT id, pattern FROM cha_name_filters ORDER BY id";

	bool Load(SqliteDatabase& db);

	// Вставить одну запись (INSERT OR IGNORE — дубликаты игнорируются).
	static void Insert(SqliteDatabase& db, std::string_view pattern);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
