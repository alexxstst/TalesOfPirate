#include <GroupParamRecordStore.h>
#include <sstream>

GameRecordset<Group_Param>::RecordEntry GroupParamRecordStore::ReadRecord(SqliteStatement& stmt) {
	Group_Param record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	// name → szName и szDataName
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	// type_ids — "id0,id1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		record.nTypeNum = 0;
		for (int i = 0; i < 8; i++) {
			record.nTypeID[i] = -1;
			if (std::getline(ss, token, ',')) {
				record.nTypeID[i] = std::stoi(token);
				if (record.nTypeID[i] >= 0)
					record.nTypeNum++;
			}
		}
	}

	// nums — "n0,n1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		record.nTotalNum = 0;
		for (int i = 0; i < 8; i++) {
			record.nNum[i] = 0;
			if (std::getline(ss, token, ',')) {
				record.nNum[i] = std::stoi(token);
				record.nTotalNum += record.nNum[i];
			}
		}
	}

	record.nRenderIdx = stmt.GetInt(col++);

	// nTotalNum — runtime-computed, уже посчитан выше

	std::string name(record.szName);
	return {record.nID, std::move(name), std::move(record)};
}

void GroupParamRecordStore::Insert(SqliteDatabase& db, const Group_Param& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO group_params "
			"(id,name,type_ids,nums,render_idx) "
			"VALUES (?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szName));

		// type_ids
		{
			std::string type_ids;
			for (int i = 0; i < 8; i++) {
				if (i > 0) type_ids += ',';
				type_ids += std::to_string(r.nTypeID[i]);
			}
			stmt.Bind(p++, type_ids);
		}

		// nums
		{
			std::string nums;
			for (int i = 0; i < 8; i++) {
				if (i > 0) nums += ',';
				nums += std::to_string(r.nNum[i]);
			}
			stmt.Bind(p++, nums);
		}

		stmt.Bind(p++, r.nRenderIdx);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "GroupParamRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
