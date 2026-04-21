#include <ItemPreRecordStore.h>

GameRecordset<CItemPreInfo>::RecordEntry ItemPreRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemPreInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void ItemPreRecordStore::Insert(SqliteDatabase& db, const CItemPreInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO item_prefixes (id, name) VALUES (?, ?)");
		stmt.Bind(1, r.Id);
		stmt.Bind(2, std::string_view(r.DataName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemPreRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CItemPreInfo* GetItemPreInfo(int nTypeID, const std::source_location& loc) {
	return ItemPreRecordStore::Instance()->Get(nTypeID, loc);
}
