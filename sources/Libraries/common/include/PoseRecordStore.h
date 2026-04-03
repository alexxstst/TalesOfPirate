#pragma once

#include "GameRecordset.h"
#include "PoseRecord.h"

class PoseRecordStore : public GameRecordset<CPoseInfo> {
public:
	static PoseRecordStore* Instance() {
		static PoseRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "poses";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS poses (
			id       INTEGER PRIMARY KEY,
			name     TEXT,
			pose_ids TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM poses ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CPoseInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CPoseInfo* GetPoseInfo(short sPoseID, const std::source_location& loc = std::source_location::current());
short GetRealPoseID(short sPoseID, short sPoseType);
