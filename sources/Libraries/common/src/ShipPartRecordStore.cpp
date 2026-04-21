#include <ShipPartRecordStore.h>
#include <sstream>

// ============================================================================
// Утилиты сериализации USHORT-массивов
// ============================================================================

static std::string SerializeUShortArray(const USHORT* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(arr[i]);
	}
	return result;
}

static void ParseUShortArray(std::string_view text, USHORT* out, int maxLen) {
	std::fill(out, out + maxLen, USHORT{0});
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<USHORT>(std::stoul(token));
	}
}

// ============================================================================
// ReadRecord — конструирует xShipPartInfo из строки SQLite-запроса
// ============================================================================

GameRecordset<xShipPartInfo>::RecordEntry ShipPartRecordStore::ReadRecord(SqliteStatement& stmt) {
	xShipPartInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	// name
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), BOAT_MAXSIZE_NAME - 1);
		record.szName[BOAT_MAXSIZE_NAME - 1] = '\0';
		record.DataName = name;
	}

	record.dwModel = static_cast<DWORD>(stmt.GetInt(col++));

	// motors
	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sMotor, BOAT_MAXNUM_MOTOR);
	}

	record.dwPrice = static_cast<DWORD>(stmt.GetInt(col++));

	// атрибуты
	record.sEndure      = static_cast<USHORT>(stmt.GetInt(col++));
	record.sResume      = static_cast<USHORT>(stmt.GetInt(col++));
	record.sDefence     = static_cast<USHORT>(stmt.GetInt(col++));
	record.sResist      = static_cast<USHORT>(stmt.GetInt(col++));
	record.sMinAttack   = static_cast<USHORT>(stmt.GetInt(col++));
	record.sMaxAttack   = static_cast<USHORT>(stmt.GetInt(col++));
	record.sDistance     = static_cast<USHORT>(stmt.GetInt(col++));
	record.sTime        = static_cast<USHORT>(stmt.GetInt(col++));
	record.sScope       = static_cast<USHORT>(stmt.GetInt(col++));
	record.sCapacity    = static_cast<USHORT>(stmt.GetInt(col++));
	record.sSupply      = static_cast<USHORT>(stmt.GetInt(col++));
	record.sConsume     = static_cast<USHORT>(stmt.GetInt(col++));
	record.sCannonSpeed = static_cast<USHORT>(stmt.GetInt(col++));
	record.sSpeed       = static_cast<USHORT>(stmt.GetInt(col++));

	// desp
	{
		auto text = stmt.GetText(col++);
		strncpy(record.szDesp, text.data(), BOAT_MAXSIZE_DESP - 1);
		record.szDesp[BOAT_MAXSIZE_DESP - 1] = '\0';
	}

	record.sParam = static_cast<USHORT>(stmt.GetInt(col++));

	std::string name(record.szName);
	return {record.Id, std::move(name), std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции из xShipPartSet
// ============================================================================

void ShipPartRecordStore::Insert(SqliteDatabase& db, const xShipPartInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO ship_parts "
			"(id,name,model,motors,price,"
			"endure,resume,defence,resist,min_attack,max_attack,"
			"distance,time,scope,capacity,supply,consume,cannon_speed,speed,"
			"desp,param) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, static_cast<int>(r.dwModel));
		stmt.Bind(p++, SerializeUShortArray(r.sMotor, BOAT_MAXNUM_MOTOR));
		stmt.Bind(p++, static_cast<int>(r.dwPrice));
		stmt.Bind(p++, static_cast<int>(r.sEndure));
		stmt.Bind(p++, static_cast<int>(r.sResume));
		stmt.Bind(p++, static_cast<int>(r.sDefence));
		stmt.Bind(p++, static_cast<int>(r.sResist));
		stmt.Bind(p++, static_cast<int>(r.sMinAttack));
		stmt.Bind(p++, static_cast<int>(r.sMaxAttack));
		stmt.Bind(p++, static_cast<int>(r.sDistance));
		stmt.Bind(p++, static_cast<int>(r.sTime));
		stmt.Bind(p++, static_cast<int>(r.sScope));
		stmt.Bind(p++, static_cast<int>(r.sCapacity));
		stmt.Bind(p++, static_cast<int>(r.sSupply));
		stmt.Bind(p++, static_cast<int>(r.sConsume));
		stmt.Bind(p++, static_cast<int>(r.sCannonSpeed));
		stmt.Bind(p++, static_cast<int>(r.sSpeed));
		stmt.Bind(p++, std::string_view(r.szDesp));
		stmt.Bind(p++, static_cast<int>(r.sParam));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ShipPartRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

xShipPartInfo* GetShipPartInfo(int nTypeID, const std::source_location& loc) {
	return ShipPartRecordStore::Instance()->Get(nTypeID, loc);
}
