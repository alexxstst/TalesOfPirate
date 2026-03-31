#pragma once
#include "GameRecordset.h"
#include "TerrainRecord.h"
#include <cstdio>

class TerrainRecordStore : public GameRecordset<MPTerrainInfo> {
public:
	static TerrainRecordStore* Instance() {
		static TerrainRecordStore i{};
		return &i;
	}

	static constexpr const char* TABLE_NAME = "terrains";
	static constexpr const char* CREATE_TABLE_SQL =
		R"(CREATE TABLE IF NOT EXISTS terrains (id INTEGER PRIMARY KEY, name TEXT, type INTEGER, attr INTEGER))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM terrains ORDER BY id";

	bool Load(SqliteDatabase& db, int (*getTexId)(const char*)) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		if (!GameRecordset::Load(db, SELECT_ALL_SQL)) return false;

		// Вычислить nTextureID для каждой записи
		ForEach([&](MPTerrainInfo& info) {
			info.nTextureID = getTexId(info.szDataName);
		});

		// Инициализация текстур water/alpha
		char szName[64];
		for (int i = 0; i < MAX_WATER_LOOP; i++) {
			sprintf(szName, "texture/terrain/water/ocean_h.%02d.bmp", i + 1);
			_nWaterBumpTexture[i] = getTexId(szName);
		}
		for (int i = 1; i < 15; i++) {
			sprintf(szName, "texture/terrain/alpha/%d.tga", i);
			_nAlphaTexture[i] = getTexId(szName);
		}
		m_nAlphaTextureI = getTexId("texture/terrain/alpha/total.tga");
		return true;
	}

	static void Insert(SqliteDatabase& db, const MPTerrainInfo& r);

	int m_nAlphaTextureI{0};

	int GetWaterBumpTextureID(int nFrame) const {
		return _nWaterBumpTexture[nFrame];
	}

	int GetAlphaTextureID(int nAlphaNo) const {
		return _nAlphaTexture[nAlphaNo];
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	int _nWaterBumpTexture[MAX_WATER_LOOP]{};
	int _nAlphaTexture[40]{};
};

inline MPTerrainInfo* GetTerrainInfo(int nID) {
	return TerrainRecordStore::Instance()->Get(nID);
}

inline int GetTerrainTextureID(int nID) {
	auto* pInfo = TerrainRecordStore::Instance()->Get(nID);
	return pInfo ? pInfo->nTextureID : 0;
}

inline int GetWaterBumpTextureID(int nFrame) {
	return TerrainRecordStore::Instance()->GetWaterBumpTextureID(nFrame);
}

inline int GetTerrainAlphaTextureID(int nAlphaNo) {
	return TerrainRecordStore::Instance()->GetAlphaTextureID(nAlphaNo);
}

inline int GetTerrainAlphaTextureID_I() {
	return TerrainRecordStore::Instance()->m_nAlphaTextureI;
}
