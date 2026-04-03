#include <HelpInfoRecordStore.h>

GameRecordset<CHelpInfo>::RecordEntry HelpInfoRecordStore::ReadRecord(SqliteStatement& stmt) {
	CHelpInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	{
		auto detail = stmt.GetText(col++);
		strncpy(record.m_strHelpDetail, detail.data(), sizeof(record.m_strHelpDetail) - 1);
		record.m_strHelpDetail[sizeof(record.m_strHelpDetail) - 1] = '\0';
	}

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void HelpInfoRecordStore::Insert(SqliteDatabase& db, const CHelpInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO help_info "
			"(id,data_name,help_detail) VALUES (?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.m_strHelpDetail));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "HelpInfoRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

const char* GetHelpInfo(const char* pszName, const std::source_location& loc) {
	auto* info = HelpInfoRecordStore::Instance()->Get(std::string_view(pszName), loc);
	return info ? info->m_strHelpDetail : nullptr;
}
