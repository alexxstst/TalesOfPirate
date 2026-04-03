#pragma once

#include "GameRecordset.h"
#include "ServerRecord.h"
#include <string>
#include <vector>
#include <map>

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
		if (!GameRecordset::Load(db, SELECT_ALL_SQL))
			return false;

		m_regionGroups.clear();
		ForEach([this](const CServerGroupInfo& info) {
			m_regionGroups[info.region].push_back(info.nID);
		});

		return true;
	}

	// region → список group ID
	std::map<std::string, std::vector<int>> m_regionGroups{};

	static void Insert(SqliteDatabase& db, const CServerGroupInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

// Доступ к данным серверов
CServerGroupInfo*      GetServerGroupInfo(int nGroupID, const std::source_location& loc = std::source_location::current());
CServerGroupInfo*      GetServerGroupInfo(const std::string& groupName, const std::source_location& loc = std::source_location::current());
int                    GetCurServerGroupCnt(int nRegionNo);
const std::string&     GetCurServerGroupName(int nRegionNo, int nGroupNo);
int                    GetRegionCnt();
const std::string&     GetCurRegionName(int nRegionNo);
const std::string&     SelectGroupIP(int nRegionNo, int nGroupNo);
