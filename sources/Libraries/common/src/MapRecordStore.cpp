#include <MapRecordStore.h>
#include <sstream>
#include <format>

GameRecordset<CMapInfo>::RecordEntry MapRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMapInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
	}

	record.nInitX = stmt.GetInt(col++);
	record.nInitY = stmt.GetInt(col++);

	// light_dir — "x,y,z"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 3; i++) {
			if (std::getline(ss, token, ','))
				record.fLightDir[i] = std::stof(token);
		}
	}

	// light_color — "r,g,b"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 3; i++) {
			record.btLightColor[i] = 0;
			if (std::getline(ss, token, ','))
				record.btLightColor[i] = static_cast<BYTE>(std::stoul(token));
		}
	}

	record.IsShowSwitch = stmt.GetInt(col++) != 0;

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void MapRecordStore::Insert(SqliteDatabase& db, const CMapInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO maps (id,data_name,name,init_x,init_y,light_dir,light_color,show_switch) "
			"VALUES (?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, r.nInitX);
		stmt.Bind(p++, r.nInitY);
		stmt.Bind(p++, std::format("{},{},{}", r.fLightDir[0], r.fLightDir[1], r.fLightDir[2]));
		stmt.Bind(p++, std::format("{},{},{}", r.btLightColor[0], r.btLightColor[1], r.btLightColor[2]));
		stmt.Bind(p++, r.IsShowSwitch ? 1 : 0);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MapRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
