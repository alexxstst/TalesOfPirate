#include <MonsterListRecordStore.h>

GameRecordset<NPCData>::RecordEntry MonsterListRecordStore::ReadRecord(SqliteStatement& stmt) {
	NPCData record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), NPC_MAXSIZE_NAME - 1);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
	}
	{
		auto text = stmt.GetText(col++);
		strncpy(record.szArea, text.data(), NPC_MAXSIZE_NAME - 1);
	}

	record.dwxPos0 = static_cast<DWORD>(stmt.GetInt(col++));
	record.dwyPos0 = static_cast<DWORD>(stmt.GetInt(col++));

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szMapName, text.data(), NPC_MAXSIZE_NAME - 1);
	}

	return {record.nID, std::string(record.szName), std::move(record)};
}

void MonsterListRecordStore::Insert(SqliteDatabase& db, const NPCData& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO monster_list (id,name,area,x_pos,y_pos,map_name) VALUES (?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, std::string_view(r.szArea));
		stmt.Bind(p++, static_cast<int>(r.dwxPos0));
		stmt.Bind(p++, static_cast<int>(r.dwyPos0));
		stmt.Bind(p++, std::string_view(r.szMapName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MonsterListRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
