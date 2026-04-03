#include <EffectRecordStore.h>
#include <sstream>

GameRecordset<CMagicInfo>::RecordEntry EffectRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMagicInfo record{};
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

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szPhotoName, text.data(), sizeof(record.szPhotoName) - 1);
		record.szPhotoName[sizeof(record.szPhotoName) - 1] = '\0';
	}

	record.nEffType = stmt.GetInt(col++);
	record.nObjType = stmt.GetInt(col++);

	// dummies — "d0,d1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		record.nDummyNum = 0;
		for (int i = 0; i < 8; i++) {
			record.nDummy[i] = -1;
			if (std::getline(ss, token, ',')) {
				record.nDummy[i] = std::stoi(token);
				record.nDummyNum++;
			}
		}
	}

	record.nDummy2    = stmt.GetInt(col++);
	record.nHeightOff = stmt.GetInt(col++);
	record.fPlayTime  = static_cast<float>(stmt.GetDouble(col++));
	record.LightID    = stmt.GetInt(col++);
	record.fBaseSize  = static_cast<float>(stmt.GetDouble(col++));

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void EffectRecordStore::Insert(SqliteDatabase& db, const CMagicInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO effects "
			"(id,data_name,name,photo_name,eff_type,obj_type,dummies,dummy2,height_off,play_time,light_id,base_size) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, std::string_view(r.szPhotoName));
		stmt.Bind(p++, r.nEffType);
		stmt.Bind(p++, r.nObjType);

		std::string dummies;
		for (int i = 0; i < r.nDummyNum; i++) {
			if (i > 0) dummies += ',';
			dummies += std::to_string(r.nDummy[i]);
		}
		stmt.Bind(p++, dummies);

		stmt.Bind(p++, r.nDummy2);
		stmt.Bind(p++, r.nHeightOff);
		stmt.Bind(p++, static_cast<double>(r.fPlayTime));
		stmt.Bind(p++, r.LightID);
		stmt.Bind(p++, static_cast<double>(r.fBaseSize));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "EffectRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CMagicInfo* GetMagicInfo(int nTypeID, const std::source_location& loc) {
	return EffectRecordStore::Instance()->Get(nTypeID, loc);
}
