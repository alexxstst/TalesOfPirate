#include <ElfSkillRecordStore.h>

GameRecordset<CElfSkillInfo>::RecordEntry ElfSkillRecordStore::ReadRecord(SqliteStatement& stmt) {
	CElfSkillInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.nIndex  = stmt.GetInt(col++);
	record.nTypeID = stmt.GetInt(col++);

	return {record.Id, std::string(record.DataName), std::move(record)};
}

void ElfSkillRecordStore::Insert(SqliteDatabase& db, const CElfSkillInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO elf_skill_info "
			"(id,data_name,skill_index,type_id) "
			"VALUES (?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, r.nIndex);
		stmt.Bind(p++, r.nTypeID);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ElfSkillRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
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
