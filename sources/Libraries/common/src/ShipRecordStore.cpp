#include <ShipRecordStore.h>
#include <sstream>
#include <format>

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
// ReadRecord — конструирует xShipInfo из строки SQLite-запроса
// ============================================================================

GameRecordset<xShipInfo>::RecordEntry ShipRecordStore::ReadRecord(SqliteStatement& stmt) {
	xShipInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	// name
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), BOAT_MAXSIZE_NAME - 1);
		record.szName[BOAT_MAXSIZE_NAME - 1] = '\0';
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}

	record.sItemID    = static_cast<USHORT>(stmt.GetInt(col++));
	record.sCharID    = static_cast<USHORT>(stmt.GetInt(col++));
	record.sPosID     = static_cast<USHORT>(stmt.GetInt(col++));
	record.byIsUpdate = static_cast<BYTE>(stmt.GetInt(col++));
	record.sBody      = static_cast<USHORT>(stmt.GetInt(col++));

	// массивы деталей
	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sEngine, BOAT_MAXNUM_PARTITEM);
		record.sNumEngine = 0;
		for (int i = 0; i < BOAT_MAXNUM_PARTITEM && record.sEngine[i]; i++) record.sNumEngine++;
	}
	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sHeader, BOAT_MAXNUM_PARTITEM);
		record.sNumHeader = 0;
		for (int i = 0; i < BOAT_MAXNUM_PARTITEM && record.sHeader[i]; i++) record.sNumHeader++;
	}
	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sCannon, BOAT_MAXNUM_PARTITEM);
		record.sNumCannon = 0;
		for (int i = 0; i < BOAT_MAXNUM_PARTITEM && record.sCannon[i]; i++) record.sNumCannon++;
	}
	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sEquipment, BOAT_MAXNUM_PARTITEM);
		record.sNumEquipment = 0;
		for (int i = 0; i < BOAT_MAXNUM_PARTITEM && record.sEquipment[i]; i++) record.sNumEquipment++;
	}

	record.sLvLimit = static_cast<USHORT>(stmt.GetInt(col++));

	{
		auto text = stmt.GetText(col++);
		ParseUShortArray(text, record.sPfLimit, BOAT_MAXNUM_PARTITEM);
		record.sNumPfLimit = 0;
		for (int i = 0; i < BOAT_MAXNUM_PARTITEM && record.sPfLimit[i]; i++) record.sNumPfLimit++;
	}

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
	return {record.nID, std::move(name), std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции из xShipSet
// ============================================================================

void ShipRecordStore::Insert(SqliteDatabase& db, const xShipInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO ships "
			"(id,name,item_id,char_id,pos_id,is_update,body,"
			"engines,headers,cannons,equipment,lv_limit,pf_limits,"
			"endure,resume,defence,resist,min_attack,max_attack,"
			"distance,time,scope,capacity,supply,consume,cannon_speed,speed,"
			"desp,param) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, static_cast<int>(r.sItemID));
		stmt.Bind(p++, static_cast<int>(r.sCharID));
		stmt.Bind(p++, static_cast<int>(r.sPosID));
		stmt.Bind(p++, static_cast<int>(r.byIsUpdate));
		stmt.Bind(p++, static_cast<int>(r.sBody));
		stmt.Bind(p++, SerializeUShortArray(r.sEngine, r.sNumEngine));
		stmt.Bind(p++, SerializeUShortArray(r.sHeader, r.sNumHeader));
		stmt.Bind(p++, SerializeUShortArray(r.sCannon, r.sNumCannon));
		stmt.Bind(p++, SerializeUShortArray(r.sEquipment, r.sNumEquipment));
		stmt.Bind(p++, static_cast<int>(r.sLvLimit));
		stmt.Bind(p++, SerializeUShortArray(r.sPfLimit, r.sNumPfLimit));
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
		ToLogService("errors", LogLevel::Error, "ShipRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

xShipInfo* GetShipInfo(int nTypeID, const std::source_location& loc) {
	return ShipRecordStore::Instance()->Get(nTypeID, loc);
}
