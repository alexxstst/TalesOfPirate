#include <EventSoundRecordStore.h>

GameRecordset<CEventSoundInfo>::RecordEntry EventSoundRecordStore::ReadRecord(SqliteStatement& stmt) {
	CEventSoundInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.nSoundID = stmt.GetInt(col++);

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void EventSoundRecordStore::Insert(SqliteDatabase& db, const CEventSoundInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO event_sounds (id, name, sound_id) VALUES (?, ?, ?)");
		stmt.Bind(1, r.Id);
		stmt.Bind(2, std::string_view(r.DataName));
		stmt.Bind(3, r.nSoundID);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "EventSoundRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CEventSoundInfo* GetEventSoundInfo(int nTypeID, const std::source_location& loc) {
	return EventSoundRecordStore::Instance()->Get(nTypeID, loc);
}
