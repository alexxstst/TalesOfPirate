#include <ServerRecordStore.h>

GameRecordset<CServerGroupInfo>::RecordEntry ServerRecordStore::ReadRecord(SqliteStatement& stmt) {
	CServerGroupInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
	}

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szRegion, text.data(), sizeof(record.szRegion) - 1);
	}

	// gate_ips — "ip0;ip1;ip2;ip3;ip4"
	{
		std::string text(stmt.GetText(col++));
		int i = 0;
		size_t start = 0;
		while (start < text.size() && i < MAX_GROUP_GATE) {
			auto end = text.find(';', start);
			if (end == std::string::npos) end = text.size();
			auto ip = text.substr(start, end - start);
			strncpy(record.szGateIP[i], ip.c_str(), 15);
			record.szGateIP[i][15] = '\0';
			i++;
			start = end + 1;
		}
	}

	record.cValidGateCnt = static_cast<char>(stmt.GetInt(col++));

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void ServerRecordStore::Insert(SqliteDatabase& db, const CServerGroupInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

		std::string ips;
		for (int i = 0; i < r.cValidGateCnt; i++) {
			if (i > 0) ips += ';';
			ips += r.szGateIP[i];
		}

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO servers (id,name,region,gate_ips,valid_gate_cnt) VALUES (?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.szRegion));
		stmt.Bind(p++, ips);
		stmt.Bind(p++, static_cast<int>(r.cValidGateCnt));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ServerRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
