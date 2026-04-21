#include <AreaRecordStore.h>
#include <sstream>
#include <format>

// ============================================================================
// ReadRecord — конструирует CAreaInfo из строки SQLite-запроса
// ============================================================================

GameRecordset<CAreaInfo>::RecordEntry AreaRecordStore::ReadRecord(SqliteStatement& stmt) {
	CAreaInfo record{};
	int col = 0;

	// id
	record.Id    = stmt.GetInt(col++);

	// name → DataName
	record.DataName = stmt.GetText(col++);

	// color (RGB)
	record.dwColor = static_cast<DWORD>(stmt.GetInt(col++));

	// music
	record.nMusic = stmt.GetInt(col++);

	// env_color (ARGB)
	record.dwEnvColor = static_cast<DWORD>(stmt.GetInt(col++));

	// light_color (ARGB)
	record.dwLightColor = static_cast<DWORD>(stmt.GetInt(col++));

	// light_dir — "x,y,z"
	{
		std::string text(stmt.GetText(col++));
		std::istringstream ss(text);
		std::string token;
		for (int i = 0; i < 3; i++) {
			record.fLightDir[i] = 0.0f;
			if (std::getline(ss, token, ','))
				record.fLightDir[i] = std::stof(token);
		}
	}

	// type
	record.chType = static_cast<char>(stmt.GetInt(col++));

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции из CAreaSet
// ============================================================================

void AreaRecordStore::Insert(SqliteDatabase& db, const CAreaInfo& record) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO areas (id, name, color, music, env_color, light_color, light_dir, type) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		stmt.Bind(1, record.Id);
		stmt.Bind(2, std::string_view(record.DataName));
		stmt.Bind(3, static_cast<int>(record.dwColor));
		stmt.Bind(4, record.nMusic);
		stmt.Bind(5, static_cast<int>(record.dwEnvColor));
		stmt.Bind(6, static_cast<int>(record.dwLightColor));
		stmt.Bind(7, std::format("{},{},{}", record.fLightDir[0], record.fLightDir[1], record.fLightDir[2]));
		stmt.Bind(8, static_cast<int>(record.chType));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "AreaRecordStore::Insert(id={}) failed: {}", record.Id, e.what());
	}
}

CAreaInfo* GetAreaInfo(int nAreaID, const std::source_location& loc) {
	return AreaRecordStore::Instance()->Get(nAreaID, loc);
}
