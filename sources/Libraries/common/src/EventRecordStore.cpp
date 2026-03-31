#include <EventRecordStore.h>

GameRecordset<CEventRecord>::RecordEntry EventRecordStore::ReadRecord(SqliteStatement& stmt) {
	CEventRecord record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;
	record.lID    = record.nID;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), defEVENT_NAME_LEN - 1);
		record.szName[defEVENT_NAME_LEN - 1] = '\0';
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
	}

	record.sEventType    = static_cast<short>(stmt.GetInt(col++));
	record.sArouseType   = static_cast<short>(stmt.GetInt(col++));
	record.sArouseRadius = static_cast<short>(stmt.GetInt(col++));
	record.sEffect       = static_cast<short>(stmt.GetInt(col++));
	record.sMusic        = static_cast<short>(stmt.GetInt(col++));
	record.sBornEffect   = static_cast<short>(stmt.GetInt(col++));
	record.sCursor       = static_cast<short>(stmt.GetInt(col++));
	record.chMainChaType = static_cast<char>(stmt.GetInt(col++));

	std::string name(record.szName);
	return {record.nID, std::move(name), std::move(record)};
}

void EventRecordStore::Insert(SqliteDatabase& db, const CEventRecord& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO events "
			"(id,name,event_type,arouse_type,arouse_radius,effect,music,born_effect,cursor,main_cha_type) "
			"VALUES (?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, static_cast<int>(r.sEventType));
		stmt.Bind(p++, static_cast<int>(r.sArouseType));
		stmt.Bind(p++, static_cast<int>(r.sArouseRadius));
		stmt.Bind(p++, static_cast<int>(r.sEffect));
		stmt.Bind(p++, static_cast<int>(r.sMusic));
		stmt.Bind(p++, static_cast<int>(r.sBornEffect));
		stmt.Bind(p++, static_cast<int>(r.sCursor));
		stmt.Bind(p++, static_cast<int>(r.chMainChaType));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "EventRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
