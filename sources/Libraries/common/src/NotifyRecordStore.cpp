#include <NotifyRecordStore.h>

GameRecordset<CNotifyInfo>::RecordEntry NotifyRecordStore::ReadRecord(SqliteStatement& stmt) {
	CNotifyInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.chType = static_cast<char>(stmt.GetInt(col++));

	{
		auto info = stmt.GetText(col++);
		strncpy(record.szInfo, info.data(), sizeof(record.szInfo) - 1);
		record.szInfo[sizeof(record.szInfo) - 1] = '\0';
	}

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void NotifyRecordStore::Insert(SqliteDatabase& db, const CNotifyInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO notifies (id, name, type, info) VALUES (?, ?, ?, ?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, static_cast<int>(r.chType));
		stmt.Bind(p++, std::string_view(r.szInfo));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "NotifyRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CNotifyInfo* GetNotifyInfo(int nTypeID, const std::source_location& loc) {
	return NotifyRecordStore::Instance()->Get(nTypeID, loc);
}
