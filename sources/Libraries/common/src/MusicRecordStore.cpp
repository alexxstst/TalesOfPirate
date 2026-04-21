#include <MusicRecordStore.h>

GameRecordset<CMusicInfo>::RecordEntry MusicRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMusicInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.nType = stmt.GetInt(col++);

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void MusicRecordStore::Insert(SqliteDatabase& db, const CMusicInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO music (id, name, type) VALUES (?, ?, ?)");
		stmt.Bind(1, r.Id);
		stmt.Bind(2, std::string_view(r.DataName));
		stmt.Bind(3, r.nType);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MusicRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CMusicInfo* GetMusicInfo(int nTypeID, const std::source_location& loc) {
	return MusicRecordStore::Instance()->Get(nTypeID, loc);
}
