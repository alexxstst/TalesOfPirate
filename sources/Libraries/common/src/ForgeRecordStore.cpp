#include <ForgeRecordStore.h>
#include <sstream>

// ============================================================================
// Сериализация массива ForgeItem в строку "sItem0,byNum0,sItem1,byNum1,..."
// ============================================================================

static std::string SerializeForgeItems(const CForgeRecord& record) {
	std::string result;
	for (int i = 0; i < FORGE_MAXNUM_ITEM; i++) {
		if (i > 0) result += ',';
		result += std::to_string(record.ForgeItem[i].sItem);
		result += ',';
		result += std::to_string(record.ForgeItem[i].byNum);
	}
	return result;
}

// ============================================================================
// ReadRecord — конструирует CForgeRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CForgeRecord>::RecordEntry ForgeRecordStore::ReadRecord(SqliteStatement& stmt) {
	CForgeRecord record{};
	int col = 0;

	// id = byLevel
	record.nID     = stmt.GetInt(col++);
	record.bExist  = TRUE;
	record.byLevel = static_cast<BYTE>(record.nID);

	// failure
	record.byFailure = static_cast<BYTE>(stmt.GetInt(col++));

	// rate (с ограничением до 100)
	record.byRate = static_cast<BYTE>(stmt.GetInt(col++));
	if (record.byRate > 100)
		record.byRate = 100;

	// money
	record.dwMoney = static_cast<DWORD>(stmt.GetInt(col++));

	// items — "sItem0,byNum0,sItem1,byNum1,..."
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < FORGE_MAXNUM_ITEM; i++) {
			record.ForgeItem[i] = {};
			if (std::getline(ss, token, ','))
				record.ForgeItem[i].sItem = static_cast<USHORT>(std::stoul(token));
			if (std::getline(ss, token, ','))
				record.ForgeItem[i].byNum = static_cast<BYTE>(std::stoul(token));
		}
	}

	// szDataName для совместимости со старым хранилищем
	sprintf(record.szDataName, "%d", record.byFailure);

	return {record.nID, {}, std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции из CForgeRecordSet
// ============================================================================

void ForgeRecordStore::Insert(SqliteDatabase& db, const CForgeRecord& record) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO forge (id, failure, rate, money, items) VALUES (?, ?, ?, ?, ?)");
		stmt.Bind(1, static_cast<int>(record.byLevel));
		stmt.Bind(2, static_cast<int>(record.byFailure));
		stmt.Bind(3, static_cast<int>(record.byRate));
		stmt.Bind(4, static_cast<int>(record.dwMoney));
		stmt.Bind(5, SerializeForgeItems(record));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ForgeRecordStore::Insert(id={}) failed: {}", record.byLevel, e.what());
	}
}

CForgeRecord* GetForgeRecordInfo(int nIndex, const std::source_location& loc) {
	return ForgeRecordStore::Instance()->Get(nIndex, loc);
}
