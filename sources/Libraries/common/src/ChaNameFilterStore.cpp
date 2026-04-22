#include <ChaNameFilterStore.h>

#include <logutil.h>

GameRecordset<ChaNameFilterRecord>::RecordEntry ChaNameFilterStore::ReadRecord(SqliteStatement& stmt) {
	ChaNameFilterRecord record{};
	int col = 0;

	record._id      = stmt.GetInt(col++);
	record._pattern = std::string(stmt.GetText(col++));

	// В базовых индексах GameRecordset адресуемся по id.
	return {record._id, std::string{}, std::move(record)};
}

bool ChaNameFilterStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void ChaNameFilterStore::Insert(SqliteDatabase& db, std::string_view pattern) {
	if (pattern.empty()) {
		return;
	}
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR IGNORE INTO cha_name_filters (pattern) VALUES (?)");
		stmt.Bind(1, pattern);
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "ChaNameFilterStore::Insert(pattern='{}') failed: {}",
					 pattern, e.what());
	}
}
