#include <ElfSkillRecordStore.h>

GameRecordset<CElfSkillInfo>::RecordEntry ElfSkillRecordStore::ReadRecord(SqliteStatement& stmt) {
	CElfSkillInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	record.nIndex  = stmt.GetInt(col++);
	record.nTypeID = stmt.GetInt(col++);

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void ElfSkillRecordStore::Insert(SqliteDatabase& db, const CElfSkillInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO elf_skill_info "
			"(id,data_name,skill_index,type_id) "
			"VALUES (?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, r.nIndex);
		stmt.Bind(p++, r.nTypeID);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ElfSkillRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CElfSkillInfo* GetElfSkillInfo(int nIndex, int nTypeID, const std::source_location& /*loc*/) {
	CElfSkillInfo* result = nullptr;
	ElfSkillRecordStore::Instance()->ForEach([&](CElfSkillInfo& info) {
		if (info.nIndex == nIndex && info.nTypeID == nTypeID)
			result = &info;
	});
	return result;
}
