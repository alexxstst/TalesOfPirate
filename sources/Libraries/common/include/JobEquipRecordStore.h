#pragma once

#include "GameRecordset.h"
#include "JobInitEquip.h"

// Хранилище таблицы начального снаряжения по классам на базе SQLite.
class JobEquipRecordStore : public GameRecordset<CJobEquipRecord> {
public:
	static JobEquipRecordStore* Instance() {
		static JobEquipRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "job_equip";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS job_equip (
			id    INTEGER PRIMARY KEY,
			job   INTEGER,
			items TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM job_equip ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	// Временный метод для миграции данных из старого CJobEquipRecordSet
	static void Insert(SqliteDatabase& db, const CJobEquipRecord& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CJobEquipRecord* GetJobEquipRecordInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
