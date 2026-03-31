#pragma once

#include "GameRecordset.h"
#include "ShipSet.h"

// Хранилище таблицы корабельных деталей (ship_parts) на базе SQLite.
class ShipPartRecordStore : public GameRecordset<xShipPartInfo> {
public:
	static ShipPartRecordStore* Instance() {
		static ShipPartRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "ship_parts";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS ship_parts (
			id           INTEGER PRIMARY KEY,
			name         TEXT,
			model        INTEGER,
			motors       TEXT,
			price        INTEGER,
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
		"SELECT * FROM ship_parts ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старого xShipPartSet
	static void Insert(SqliteDatabase& db, const xShipPartInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline xShipPartInfo* GetShipPartInfo(int nTypeID) {
	return ShipPartRecordStore::Instance()->Get(nTypeID);
}
