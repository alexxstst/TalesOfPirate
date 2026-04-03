#include <MonsterInfoRecordStore.h>
#include <sstream>

GameRecordset<CMonsterInfo>::RecordEntry MonsterInfoRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMonsterInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
		strncpy(record.szName, dn.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
	}

	record.ptStart.x = stmt.GetInt(col++);
	record.ptStart.y = stmt.GetInt(col++);
	record.ptEnd.x   = stmt.GetInt(col++);
	record.ptEnd.y   = stmt.GetInt(col++);

	// monster_list — "id0,id1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < MONSTER_LIST_MAX && std::getline(ss, token, ','); ++i) {
			if (!token.empty())
				record.nMonsterList[i] = std::stoi(token);
		}
	}

	{
		auto area = stmt.GetText(col++);
		strncpy(record.szArea, area.data(), sizeof(record.szArea) - 1);
		record.szArea[sizeof(record.szArea) - 1] = '\0';
	}

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void MonsterInfoRecordStore::Insert(SqliteDatabase& db, const CMonsterInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO monster_info "
			"(id,data_name,start_x,start_y,end_x,end_y,monster_list,area) "
			"VALUES (?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, static_cast<int>(r.ptStart.x));
		stmt.Bind(p++, static_cast<int>(r.ptStart.y));
		stmt.Bind(p++, static_cast<int>(r.ptEnd.x));
		stmt.Bind(p++, static_cast<int>(r.ptEnd.y));

		std::string monsters;
		for (int i = 0; i < MONSTER_LIST_MAX; ++i) {
			if (i > 0) monsters += ',';
			monsters += std::to_string(r.nMonsterList[i]);
		}
		stmt.Bind(p++, monsters);

		stmt.Bind(p++, std::string_view(r.szArea));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MonsterInfoRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CMonsterInfo* GetMonsterInfo(int nMapID, const std::source_location& loc) {
	return MonsterInfoRecordStore::Instance()->Get(nMapID, loc);
}

CMonsterInfo* GetMonsterInfo(const char* pszMapName, const std::source_location& loc) {
	return MonsterInfoRecordStore::Instance()->Get(std::string_view(pszMapName), loc);
}
