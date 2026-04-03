#pragma once

#include "GameRecordset.h"
#include "EffectRecord.h"

// Хранилище таблицы эффектов (магия) на базе SQLite.
class EffectRecordStore : public GameRecordset<CMagicInfo> {
public:
	static EffectRecordStore* Instance() {
		static EffectRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "effects";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS effects (
			id         INTEGER PRIMARY KEY,
			data_name  TEXT,
			name       TEXT,
			photo_name TEXT,
			eff_type   INTEGER,
			obj_type   INTEGER,
			dummies    TEXT,
			dummy2     INTEGER,
			height_off INTEGER,
			play_time  REAL,
			light_id   INTEGER,
			base_size  REAL
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM effects ORDER BY id";

	bool Load(SqliteDatabase& db, int(*getTexId)(const char*) = nullptr) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		if (!GameRecordset::Load(db, SELECT_ALL_SQL)) return false;

		if (getTexId) {
			ForEach([&](CMagicInfo& info) {
				char szPhoto[72];
				sprintf(szPhoto, "texture/photo/%s.bmp", info.szPhotoName);
				info.nPhotoTexID = getTexId(szPhoto);
			});
		}
		return true;
	}

	static void Insert(SqliteDatabase& db, const CMagicInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CMagicInfo* GetMagicInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
