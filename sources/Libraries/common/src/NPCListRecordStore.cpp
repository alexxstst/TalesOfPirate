#include <NPCListRecordStore.h>

GameRecordset<NPCData>::RecordEntry NPCListRecordStore::ReadRecord(SqliteStatement& stmt) {
	NPCData record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), NPC_MAXSIZE_NAME - 1);
		record.DataName = name;
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

	return {record.Id, std::string(record.szName), std::move(record)};
}

void NPCListRecordStore::Insert(SqliteDatabase& db, const NPCData& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO npc_list (id,name,area,x_pos,y_pos,map_name) VALUES (?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, std::string_view(r.szArea));
		stmt.Bind(p++, static_cast<int>(r.dwxPos0));
		stmt.Bind(p++, static_cast<int>(r.dwyPos0));
		stmt.Bind(p++, std::string_view(r.szMapName));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "NPCListRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}
