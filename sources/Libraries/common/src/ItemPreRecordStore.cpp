#include <ItemPreRecordStore.h>

GameRecordset<CItemPreInfo>::RecordEntry ItemPreRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemPreInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void ItemPreRecordStore::Insert(SqliteDatabase& db, const CItemPreInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO item_prefixes (id, name) VALUES (?, ?)");
		stmt.Bind(1, r.nID);
		stmt.Bind(2, std::string_view(r.szDataName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemPreRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CItemPreInfo* GetItemPreInfo(int nTypeID, const std::source_location& loc) {
	return ItemPreRecordStore::Instance()->Get(nTypeID, loc);
}
