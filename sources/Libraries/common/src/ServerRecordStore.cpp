#include <ServerRecordStore.h>
#include <sstream>
#include <cstdlib>

// --- ReadRecord / Insert ---

GameRecordset<CServerGroupInfo>::RecordEntry ServerRecordStore::ReadRecord(SqliteStatement& stmt) {
	CServerGroupInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.region = std::string(stmt.GetText(col++));

	// gate_ips — "ip0;ip1;ip2;..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		while (std::getline(ss, token, ';')) {
			if (!token.empty() && token != "0")
				record.gateIPs.push_back(std::move(token));
		}
	}

	col++; // valid_gate_cnt — вычисляется из gateIPs.size()

	return {record.Id, std::string(record.DataName), std::move(record)};
}

void ServerRecordStore::Insert(SqliteDatabase& db, const CServerGroupInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

		std::string ips;
		for (size_t i = 0; i < r.gateIPs.size(); i++) {
			if (i > 0) ips += ';';
			ips += r.gateIPs[i];
		}

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO servers (id,name,region,gate_ips,valid_gate_cnt) VALUES (?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, r.region);
		stmt.Bind(p++, ips);
		stmt.Bind(p++, static_cast<int>(r.gateIPs.size()));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ServerRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

// --- Функции доступа ---

static const std::string s_empty;
static const std::vector<int> s_emptyVec;

// Получить n-й элемент map по индексу
static auto GetRegionEntry(int nRegionNo) {
	auto& groups = ServerRecordStore::Instance()->m_regionGroups;
	if (nRegionNo < 0 || nRegionNo >= static_cast<int>(groups.size()))
		return groups.end();
	return std::next(groups.begin(), nRegionNo);
}

CServerGroupInfo* GetServerGroupInfo(int nGroupID, const std::source_location& loc) {
	return ServerRecordStore::Instance()->Get(nGroupID, loc);
}

CServerGroupInfo* GetServerGroupInfo(const std::string& groupName, const std::source_location& loc) {
	return ServerRecordStore::Instance()->Get(std::string_view(groupName), loc);
}

int GetCurServerGroupCnt(int nRegionNo) {
	auto it = GetRegionEntry(nRegionNo);
	if (it == ServerRecordStore::Instance()->m_regionGroups.end()) return 0;
	return static_cast<int>(it->second.size());
}

const std::string& GetCurServerGroupName(int nRegionNo, int nGroupNo) {
	auto it = GetRegionEntry(nRegionNo);
	if (it == ServerRecordStore::Instance()->m_regionGroups.end()) return s_empty;
	if (nGroupNo < 0 || nGroupNo >= static_cast<int>(it->second.size())) return s_empty;
	auto* info = GetServerGroupInfo(it->second[nGroupNo]);
	if (!info) return s_empty;
	static thread_local std::string result;
	result = info->DataName;
	return result;
}

int GetRegionCnt() {
	return static_cast<int>(ServerRecordStore::Instance()->m_regionGroups.size());
}

const std::string& GetCurRegionName(int nRegionNo) {
	auto it = GetRegionEntry(nRegionNo);
	if (it == ServerRecordStore::Instance()->m_regionGroups.end()) return s_empty;
	return it->first;
}

const std::string& SelectGroupIP(int nRegionNo, int nGroupNo) {
	auto it = GetRegionEntry(nRegionNo);
	if (it == ServerRecordStore::Instance()->m_regionGroups.end()) return s_empty;
	if (nGroupNo < 0 || nGroupNo >= static_cast<int>(it->second.size())) return s_empty;

	auto* pGroup = GetServerGroupInfo(it->second[nGroupNo]);
	if (!pGroup || pGroup->gateIPs.empty()) return s_empty;

	srand(GetTickCount());
	int nGateNo = rand() % static_cast<int>(pGroup->gateIPs.size());
	return pGroup->gateIPs[nGateNo];
}
