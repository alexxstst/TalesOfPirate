#include <ResourceRecordStore.h>

GameRecordset<CResourceInfo>::RecordEntry ResourceRecordStore::ReadRecord(SqliteStatement& stmt) {
	CResourceInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.m_iType = stmt.GetInt(col++);

	return {record.Id, std::string(record.DataName), std::move(record)};
}

void ResourceRecordStore::Insert(SqliteDatabase& db, const CResourceInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO resource_info "
			"(id,data_name,type) VALUES (?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, r.m_iType);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ResourceRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CResourceInfo* GetResourceInfo(int nID, const std::source_location& loc) {
	return ResourceRecordStore::Instance()->Get(nID, loc);
}

CResourceInfo* GetResourceInfo(const char* pszName, const std::source_location& loc) {
	return ResourceRecordStore::Instance()->Get(std::string_view(pszName), loc);
}
