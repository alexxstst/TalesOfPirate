#include <EventSoundRecordStore.h>

GameRecordset<CEventSoundInfo>::RecordEntry EventSoundRecordStore::ReadRecord(SqliteStatement& stmt) {
	CEventSoundInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	record.nSoundID = stmt.GetInt(col++);

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void EventSoundRecordStore::Insert(SqliteDatabase& db, const CEventSoundInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO event_sounds (id, name, sound_id) VALUES (?, ?, ?)");
		stmt.Bind(1, r.nID);
		stmt.Bind(2, std::string_view(r.szDataName));
		stmt.Bind(3, r.nSoundID);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "EventSoundRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CEventSoundInfo* GetEventSoundInfo(int nTypeID, const std::source_location& loc) {
	return EventSoundRecordStore::Instance()->Get(nTypeID, loc);
}
