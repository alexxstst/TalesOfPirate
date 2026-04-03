#pragma once

#include "GameRecordset.h"
#include "MountRecord.h"

// Хранилище таблицы маунтов на базе SQLite.
class MountRecordStore : public GameRecordset<CMountInfo> {
public:
	static MountRecordStore* Instance() {
		static MountRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "mount_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS mount_info (
			id        INTEGER PRIMARY KEY,
			data_name TEXT,
			mount_id  INTEGER,
			bone_id   INTEGER,
			heights   TEXT,
			offset_x  INTEGER,
			offset_y  INTEGER,
			pose_ids  TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM mount_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CMountInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CMountInfo* GetMountInfo(int nMountID, const std::source_location& loc = std::source_location::current());
CMountInfo* GetMountInfo(const char* pszItemName, const std::source_location& loc = std::source_location::current());
