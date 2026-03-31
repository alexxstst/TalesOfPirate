#include <ItemRefineRecordStore.h>
#include <sstream>

GameRecordset<CItemRefineInfo>::RecordEntry ItemRefineRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemRefineInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	// values — "v0,v1,...,v13"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < ITEM_REFINE_NUM; i++) {
			record.Value[i] = 0;
			if (std::getline(ss, token, ','))
				record.Value[i] = static_cast<short>(std::stoi(token));
		}
	}

	// cha_effect_scale — "f0,f1,f2,f3"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 4; i++) {
			record.fChaEffectScale[i] = 0.0f;
			if (std::getline(ss, token, ','))
				record.fChaEffectScale[i] = std::stof(token);
		}
	}

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void ItemRefineRecordStore::Insert(SqliteDatabase& db, const CItemRefineInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO item_refine "
			"(id,name,refine_values,cha_effect_scale) "
			"VALUES (?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));

		// values
		{
			std::string values;
			for (int i = 0; i < ITEM_REFINE_NUM; i++) {
				if (i > 0) values += ',';
				values += std::to_string(r.Value[i]);
			}
			stmt.Bind(p++, values);
		}

		// cha_effect_scale
		{
			std::string scales;
			for (int i = 0; i < 4; i++) {
				if (i > 0) scales += ',';
				scales += std::to_string(r.fChaEffectScale[i]);
			}
			stmt.Bind(p++, scales);
		}

		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemRefineRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
