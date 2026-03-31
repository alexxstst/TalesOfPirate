#pragma once

#include "GameRecordset.h"
#include "ShipSet.h"

// Хранилище таблицы кораблей (ships) на базе SQLite.
class ShipRecordStore : public GameRecordset<xShipInfo> {
public:
	static ShipRecordStore* Instance() {
		static ShipRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "ships";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS ships (
			id           INTEGER PRIMARY KEY,
			name         TEXT,
			item_id      INTEGER,
			char_id      INTEGER,
			pos_id       INTEGER,
			is_update    INTEGER,
			body         INTEGER,
			engines      TEXT,
			headers      TEXT,
			cannons      TEXT,
			equipment    TEXT,
			lv_limit     INTEGER,
			pf_limits    TEXT,
			endure       INTEGER,
			resume       INTEGER,
			defence      INTEGER,
			resist       INTEGER,
			min_attack   INTEGER,
			max_attack   INTEGER,
			distance     INTEGER,
			time         INTEGER,
			scope        INTEGER,
			capacity     INTEGER,
			supply       INTEGER,
			consume      INTEGER,
			cannon_speed INTEGER,
			speed        INTEGER,
			desp         TEXT,
			param        INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM ships ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старого xShipSet
	static void Insert(SqliteDatabase& db, const xShipInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline xShipInfo* GetShipInfo(int nTypeID) {
	return ShipRecordStore::Instance()->Get(nTypeID);
}
