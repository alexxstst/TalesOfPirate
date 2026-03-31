#include <MusicRecordStore.h>

GameRecordset<CMusicInfo>::RecordEntry MusicRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMusicInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	record.nType = stmt.GetInt(col++);

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void MusicRecordStore::Insert(SqliteDatabase& db, const CMusicInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO music (id, name, type) VALUES (?, ?, ?)");
		stmt.Bind(1, r.nID);
		stmt.Bind(2, std::string_view(r.szDataName));
		stmt.Bind(3, r.nType);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MusicRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
