#include <PoseRecordStore.h>
#include <sstream>

GameRecordset<CPoseInfo>::RecordEntry PoseRecordStore::ReadRecord(SqliteStatement& stmt) {
	CPoseInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
	}

	// pose_ids — "p0,p1,p2,p3,p4,p5,p6"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 7; i++) {
			record.sRealPoseID[i] = 0;
			if (std::getline(ss, token, ','))
				record.sRealPoseID[i] = static_cast<short>(std::stoi(token));
		}
	}

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void PoseRecordStore::Insert(SqliteDatabase& db, const CPoseInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		std::string ids;
		for (int i = 0; i < 7; i++) {
			if (i > 0) ids += ',';
			ids += std::to_string(r.sRealPoseID[i]);
		}
		auto stmt = db.Prepare("INSERT OR REPLACE INTO poses (id, name, pose_ids) VALUES (?, ?, ?)");
		stmt.Bind(1, r.nID);
		stmt.Bind(2, std::string_view(r.szDataName));
		stmt.Bind(3, ids);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "PoseRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
