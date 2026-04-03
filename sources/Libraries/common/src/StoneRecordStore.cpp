#include <StoneRecordStore.h>
#include <sstream>

GameRecordset<CStoneInfo>::RecordEntry StoneRecordStore::ReadRecord(SqliteStatement& stmt) {
	CStoneInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	record.nItemID = stmt.GetInt(col++);

	// equip_pos — "p0,p1,p2"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < STONE_EQUIP_MAX && std::getline(ss, token, ','); ++i) {
			if (!token.empty())
				record.nEquipPos[i] = std::stoi(token);
		}
	}

	record.nType = stmt.GetInt(col++);

	{
		auto hf = stmt.GetText(col++);
		strncpy(record.szHintFunc, hf.data(), sizeof(record.szHintFunc) - 1);
		record.szHintFunc[sizeof(record.szHintFunc) - 1] = '\0';
	}

	record.nItemRgb = static_cast<DWORD>(stmt.GetInt(col++));

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void StoneRecordStore::Insert(SqliteDatabase& db, const CStoneInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO stone_info "
			"(id,data_name,item_id,equip_pos,type,hint_func,item_rgb) "
			"VALUES (?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, r.nItemID);

		std::string equipPos;
		for (int i = 0; i < STONE_EQUIP_MAX; ++i) {
			if (i > 0) equipPos += ',';
			equipPos += std::to_string(r.nEquipPos[i]);
		}
		stmt.Bind(p++, equipPos);

		stmt.Bind(p++, r.nType);
		stmt.Bind(p++, std::string_view(r.szHintFunc));
		stmt.Bind(p++, static_cast<int>(r.nItemRgb));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "StoneRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CStoneInfo* GetStoneInfo(int nTypeID, const std::source_location& loc) {
	return StoneRecordStore::Instance()->Get(nTypeID, loc);
}
