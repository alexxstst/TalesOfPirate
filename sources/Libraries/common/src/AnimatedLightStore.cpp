#include <AnimatedLightStore.h>

#include <algorithm>

GameRecordset<CAnimatedLightInfo>::RecordEntry AnimatedLightStore::ReadRecord(SqliteStatement& stmt) {
	CAnimatedLightInfo record{};
	int col = 0;

	record._lightNo = static_cast<short>(stmt.GetInt(col++));
	record._keyNo = static_cast<short>(stmt.GetInt(col++));
	record._frameId = stmt.GetInt(col++);
	record._type = stmt.GetInt(col++);
	record._r = stmt.GetInt(col++);
	record._g = stmt.GetInt(col++);
	record._b = stmt.GetInt(col++);
	record._range = static_cast<float>(stmt.GetDouble(col++));
	record._attenuation0 = static_cast<float>(stmt.GetDouble(col++));
	record._attenuation1 = static_cast<float>(stmt.GetDouble(col++));
	record._attenuation2 = static_cast<float>(stmt.GetDouble(col++));

	record.bExist = TRUE;

	// Синтетический ID: light_no * 1000 + key_no.
	int id = static_cast<int>(record._lightNo) * 1000 + static_cast<int>(record._keyNo);
	record.nID = id;

	_maxLightNo = (std::max)(record._lightNo, _maxLightNo);

	return {id, std::string{}, std::move(record)};
}

bool AnimatedLightStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	_maxLightNo = -1;
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void AnimatedLightStore::Insert(SqliteDatabase& db, const CAnimatedLightInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO animated_lights "
			"(light_no, key_no, frame_id, type, r, g, b, "
			"range_, attenuation0, attenuation1, attenuation2) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		stmt.Bind(1, r._lightNo);
		stmt.Bind(2, r._keyNo);
		stmt.Bind(3, r._frameId);
		stmt.Bind(4, r._type);
		stmt.Bind(5, r._r);
		stmt.Bind(6, r._g);
		stmt.Bind(7, r._b);
		stmt.Bind(8, r._range);
		stmt.Bind(9, r._attenuation0);
		stmt.Bind(10, r._attenuation1);
		stmt.Bind(11, r._attenuation2);
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "AnimatedLightStore::Insert(light={}, key={}) failed: {}",
					 r._lightNo, r._keyNo, e.what());
	}
}
