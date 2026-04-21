#include <ItemRefineEffectRecordStore.h>
#include <sstream>

GameRecordset<CItemRefineEffectInfo>::RecordEntry ItemRefineEffectRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemRefineEffectInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.nLightID = stmt.GetInt(col++);

	// effect_ids — row-major "e00,e01,e02,e03,e10,e11,..." (4x4)
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int cha = 0; cha < REFINE_EFFECT_CHA_NUM; cha++) {
			int count = 0;
			for (int eff = 0; eff < REFINE_EFFECT_NUM; eff++) {
				record.sEffectID[cha][eff] = 0;
				if (std::getline(ss, token, ',')) {
					record.sEffectID[cha][eff] = static_cast<short>(std::stoi(token));
					if (record.sEffectID[cha][eff] != 0)
						count++;
				}
			}
			record._sEffectNum[cha] = count;
		}
	}

	// dummies — "d0,d1,d2,d3"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < REFINE_EFFECT_NUM; i++) {
			record.chDummy[i] = 0;
			if (std::getline(ss, token, ','))
				record.chDummy[i] = static_cast<char>(std::stoi(token));
		}
	}

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void ItemRefineEffectRecordStore::Insert(SqliteDatabase& db, const CItemRefineEffectInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO item_refine_effects "
			"(id,name,light_id,effect_ids,dummies) "
			"VALUES (?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, r.nLightID);

		// effect_ids — row-major
		{
			std::string effect_ids;
			for (int cha = 0; cha < REFINE_EFFECT_CHA_NUM; cha++) {
				for (int eff = 0; eff < REFINE_EFFECT_NUM; eff++) {
					if (!effect_ids.empty()) effect_ids += ',';
					effect_ids += std::to_string(r.sEffectID[cha][eff]);
				}
			}
			stmt.Bind(p++, effect_ids);
		}

		// dummies
		{
			std::string dummies;
			for (int i = 0; i < REFINE_EFFECT_NUM; i++) {
				if (i > 0) dummies += ',';
				dummies += std::to_string(static_cast<int>(r.chDummy[i]));
			}
			stmt.Bind(p++, dummies);
		}

		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ItemRefineEffectRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CItemRefineEffectInfo* GetItemRefineEffectInfo(int nRefineID, const std::source_location& loc) {
	return ItemRefineEffectRecordStore::Instance()->Get(nRefineID, loc);
}
