#pragma once

#include "GameRecordset.h"
#include "SceneObjRecord.h"

// Хранилище таблицы объектов сцены на базе SQLite.
class SceneObjRecordStore : public GameRecordset<CSceneObjInfo> {
public:
	static SceneObjRecordStore* Instance() {
		static SceneObjRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "scene_objects";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS scene_objects (
			id                 INTEGER PRIMARY KEY,
			data_name          TEXT,
			name               TEXT,
			type               INTEGER,
			point_color        TEXT,
			env_color          TEXT,
			range              INTEGER,
			attenuation        REAL,
			anim_ctrl_id       INTEGER,
			attach_effect_id   INTEGER,
			enable_env_light   INTEGER,
			enable_point_light INTEGER,
			style              INTEGER,
			flag               INTEGER,
			size_flag          INTEGER,
			env_sound          TEXT,
			env_sound_dis      INTEGER,
			shade_flag         INTEGER,
			is_really_big      INTEGER,
			fade_obj_num       INTEGER,
			fade_obj_seq       TEXT,
			fade_coefficient   REAL
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM scene_objects ORDER BY id";

	bool Load(SqliteDatabase& db, int(*getTexId)(const char*) = nullptr) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		if (!GameRecordset::Load(db, SELECT_ALL_SQL)) return false;

		if (getTexId) {
			ForEach([&](CSceneObjInfo& info) {
				char szPhoto[72];
				sprintf(szPhoto, "texture/photo/sceneobj/%s.bmp", info.szName);
				info.nPhotoTexID = getTexId(szPhoto);
			});
		}
		return true;
	}

	static void Insert(SqliteDatabase& db, const CSceneObjInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CSceneObjInfo* GetSceneObjInfo(int nTypeID) {
	return SceneObjRecordStore::Instance()->Get(nTypeID);
}
