#pragma once

#include "GameRecordset.h"
#include "ServerRecord.h"

class ServerRecordStore : public GameRecordset<CServerGroupInfo> {
public:
	static ServerRecordStore* Instance() {
		static ServerRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "servers";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS servers (
			id             INTEGER PRIMARY KEY,
			name           TEXT,
			region         TEXT,
			gate_ips       TEXT,
			valid_gate_cnt INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM servers ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CServerGroupInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
