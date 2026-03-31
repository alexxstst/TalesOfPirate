#include <JobEquipRecordStore.h>
#include <sstream>

// ============================================================================
// Сериализация массива sItemID в строку "id0,id1,id2,..."
// ============================================================================

static std::string SerializeItems(const CJobEquipRecord& record) {
	std::string result;
	for (int i = 0; i < defJOB_INIT_EQUIP_MAX; i++) {
		if (i > 0) result += ',';
		result += std::to_string(record.sItemID[i]);
	}
	return result;
}

// ============================================================================
// ReadRecord — конструирует CJobEquipRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CJobEquipRecord>::RecordEntry JobEquipRecordStore::ReadRecord(SqliteStatement& stmt) {
	CJobEquipRecord record{};
	int col = 0;

	// id
	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;
	record.chID   = static_cast<char>(record.nID);

	// job
	record.chJob = static_cast<char>(stmt.GetInt(col++));

	// items — "id0,id1,id2,id3,id4,id5"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < defJOB_INIT_EQUIP_MAX; i++) {
			record.sItemID[i] = 0;
			if (std::getline(ss, token, ','))
				record.sItemID[i] = static_cast<short>(std::stoi(token));
		}
	}

	return {record.nID, {}, std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции из CJobEquipRecordSet
// ============================================================================

void JobEquipRecordStore::Insert(SqliteDatabase& db, const CJobEquipRecord& record) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO job_equip (id, job, items) VALUES (?, ?, ?)");
		stmt.Bind(1, static_cast<int>(record.chID));
		stmt.Bind(2, static_cast<int>(record.chJob));
		stmt.Bind(3, SerializeItems(record));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "JobEquipRecordStore::Insert(id={}) failed: {}", record.chID, e.what());
	}
}
