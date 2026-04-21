#include <ItemTypeRecordStore.h>

GameRecordset<CItemTypeInfo>::RecordEntry ItemTypeRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemTypeInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void ItemTypeRecordStore::Insert(SqliteDatabase& db, const CItemTypeInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO item_types (id, name) VALUES (?, ?)");
		stmt.Bind(1, r.Id);
		stmt.Bind(2, std::string_view(r.DataName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemTypeRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CItemTypeInfo* GetItemTypeInfo(int nTypeID, const std::source_location& loc) {
	return ItemTypeRecordStore::Instance()->Get(nTypeID, loc);
}
