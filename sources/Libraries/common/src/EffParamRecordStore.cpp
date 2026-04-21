#include <EffParamRecordStore.h>
#include <sstream>

GameRecordset<EFF_Param>::RecordEntry EffParamRecordStore::ReadRecord(SqliteStatement& stmt) {
	EFF_Param record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	// name → szName и DataName
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
		record.DataName = name;
	}

	// models — "model0,model1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		record.nModelNum = 0;
		for (int i = 0; i < 8; i++) {
			record.strModel[i][0] = '\0';
			if (std::getline(ss, token, ',')) {
				if (!token.empty()) {
					strncpy(record.strModel[i], token.c_str(), sizeof(record.strModel[i]) - 1);
					record.strModel[i][sizeof(record.strModel[i]) - 1] = '\0';
					record.nModelNum++;
				}
			}
		}
	}

	record.nVel = stmt.GetInt(col++);

	// parts — "part0,part1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		record.nParNum = 0;
		for (int i = 0; i < 8; i++) {
			record.strPart[i][0] = '\0';
			if (std::getline(ss, token, ',')) {
				if (!token.empty()) {
					strncpy(record.strPart[i], token.c_str(), sizeof(record.strPart[i]) - 1);
					record.strPart[i][sizeof(record.strPart[i]) - 1] = '\0';
					record.nParNum++;
				}
			}
		}
	}

	// dummies — "d0,d1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 8; i++) {
			record.nDummy[i] = -1;
			if (std::getline(ss, token, ','))
				record.nDummy[i] = std::stoi(token);
		}
	}

	record.nRenderIdx = stmt.GetInt(col++);
	record.nLightID   = stmt.GetInt(col++);

	{
		auto text = stmt.GetText(col++);
		strncpy(record.strResult, text.data(), sizeof(record.strResult) - 1);
		record.strResult[sizeof(record.strResult) - 1] = '\0';
	}

	std::string name(record.szName);
	return {record.Id, std::move(name), std::move(record)};
}

void EffParamRecordStore::Insert(SqliteDatabase& db, const EFF_Param& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO eff_params "
			"(id,name,models,vel,parts,dummies,render_idx,light_id,result) "
			"VALUES (?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.szName));

		// models
		{
			std::string models;
			for (int i = 0; i < r.nModelNum; i++) {
				if (i > 0) models += ',';
				models += r.strModel[i];
			}
			stmt.Bind(p++, models);
		}

		stmt.Bind(p++, r.nVel);

		// parts
		{
			std::string parts;
			for (int i = 0; i < r.nParNum; i++) {
				if (i > 0) parts += ',';
				parts += r.strPart[i];
			}
			stmt.Bind(p++, parts);
		}

		// dummies
		{
			std::string dummies;
			for (int i = 0; i < 8; i++) {
				if (i > 0) dummies += ',';
				dummies += std::to_string(r.nDummy[i]);
			}
			stmt.Bind(p++, dummies);
		}

		stmt.Bind(p++, r.nRenderIdx);
		stmt.Bind(p++, r.nLightID);
		stmt.Bind(p++, std::string_view(r.strResult));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "EffParamRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}
