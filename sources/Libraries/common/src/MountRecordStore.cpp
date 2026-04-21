#include <MountRecordStore.h>
#include <sstream>

GameRecordset<CMountInfo>::RecordEntry MountRecordStore::ReadRecord(SqliteStatement& stmt) {
	CMountInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	record.DataName = stmt.GetText(col++);

	record.mountID = static_cast<short>(stmt.GetInt(col++));
	record.boneID  = static_cast<short>(stmt.GetInt(col++));

	// heights — "h0,h1,h2,h3"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 4 && std::getline(ss, token, ','); ++i) {
			if (!token.empty())
				record.height[i] = static_cast<short>(std::stoi(token));
		}
	}

	record.offsetX = static_cast<short>(stmt.GetInt(col++));
	record.offsetY = static_cast<short>(stmt.GetInt(col++));

	// pose_ids — "p0,p1,p2,p3"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 4 && std::getline(ss, token, ','); ++i) {
			if (!token.empty())
				record.poseID[i] = static_cast<short>(std::stoi(token));
		}
	}

	return {record.Id, std::string(record.DataName), std::move(record)};
}

void MountRecordStore::Insert(SqliteDatabase& db, const CMountInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO mount_info "
			"(id,data_name,mount_id,bone_id,heights,offset_x,offset_y,pose_ids) "
			"VALUES (?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.DataName));
		stmt.Bind(p++, static_cast<int>(r.mountID));
		stmt.Bind(p++, static_cast<int>(r.boneID));

		std::string heights;
		for (int i = 0; i < 4; ++i) {
			if (i > 0) heights += ',';
			heights += std::to_string(r.height[i]);
		}
		stmt.Bind(p++, heights);

		stmt.Bind(p++, static_cast<int>(r.offsetX));
		stmt.Bind(p++, static_cast<int>(r.offsetY));

		std::string poseIds;
		for (int i = 0; i < 4; ++i) {
			if (i > 0) poseIds += ',';
			poseIds += std::to_string(r.poseID[i]);
		}
		stmt.Bind(p++, poseIds);

		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "MountRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CMountInfo* GetMountInfo(int nMountID, const std::source_location& loc) {
	return MountRecordStore::Instance()->Get(nMountID, loc);
}

CMountInfo* GetMountInfo(const char* pszItemName, const std::source_location& loc) {
	return MountRecordStore::Instance()->Get(std::string_view(pszItemName), loc);
}
