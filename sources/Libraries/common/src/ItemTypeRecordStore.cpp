#include <ItemTypeRecordStore.h>

GameRecordset<CItemTypeInfo>::RecordEntry ItemTypeRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemTypeInfo record{};
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

void ItemTypeRecordStore::Insert(SqliteDatabase& db, const CItemTypeInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO item_types (id, name) VALUES (?, ?)");
		stmt.Bind(1, r.nID);
		stmt.Bind(2, std::string_view(r.szDataName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemTypeRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
