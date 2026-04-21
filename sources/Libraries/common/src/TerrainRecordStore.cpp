#include <TerrainRecordStore.h>

GameRecordset<MPTerrainInfo>::RecordEntry TerrainRecordStore::ReadRecord(SqliteStatement& stmt) {
	MPTerrainInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.btType = static_cast<BYTE>(stmt.GetInt(col++));
	record.btAttr = static_cast<BYTE>(stmt.GetInt(col++));

	// nTextureID — runtime-only, не хранится в SQLite

	std::string name(record.DataName);
	return {.id = record.Id, .name = std::move(name), .record = record};
}

void TerrainRecordStore::Insert(SqliteDatabase& db, const MPTerrainInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare("INSERT OR REPLACE INTO terrains (id, name, type, attr) VALUES (?, ?, ?, ?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, static_cast<int>(r.btType));
		stmt.Bind(p++, static_cast<int>(r.btAttr));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "TerrainRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

MPTerrainInfo* GetTerrainInfo(int nID, const std::source_location& loc) {
	return TerrainRecordStore::Instance()->Get(nID, loc);
}

int GetTerrainTextureID(int nID, const std::source_location& loc) {
	auto* pInfo = TerrainRecordStore::Instance()->Get(nID, loc);
	return pInfo ? pInfo->nTextureID : 0;
}

int GetWaterBumpTextureID(int nFrame) {
	return TerrainRecordStore::Instance()->GetWaterBumpTextureID(nFrame);
}

int GetTerrainAlphaTextureID(int nAlphaNo) {
	return TerrainRecordStore::Instance()->GetAlphaTextureID(nAlphaNo);
}

int GetTerrainAlphaTextureID_I() {
	return TerrainRecordStore::Instance()->m_nAlphaTextureI;
}
