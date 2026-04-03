#include <ChaCreateRecordStore.h>
#include <sstream>

static std::string SerializeDwordArray(const DWORD* arr, DWORD count) {
	std::string result;
	for (DWORD i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(arr[i]);
	}
	return result;
}

static DWORD ParseDwordArray(std::string_view text, DWORD* out, int maxLen) {
	std::fill(out, out + maxLen, DWORD{0});
	if (text.empty() || text == "0") return 0;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	DWORD i = 0;
	while (std::getline(ss, token, ',') && i < static_cast<DWORD>(maxLen)) {
		out[i++] = static_cast<DWORD>(std::stoul(token));
	}
	return i;
}

GameRecordset<CChaCreateInfo>::RecordEntry ChaCreateRecordStore::ReadRecord(SqliteStatement& stmt) {
	CChaCreateInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto name = stmt.GetText(col++);
		strncpy(record.szDataName, name.data(), sizeof(record.szDataName) - 1);
	}

	record.bone = static_cast<DWORD>(stmt.GetInt(col++));

	record.face_num = ParseDwordArray(stmt.GetText(col++), record.face, 64);
	record.hair_num = ParseDwordArray(stmt.GetText(col++), record.hair, 64);
	record.body_num = ParseDwordArray(stmt.GetText(col++), record.body, 64);
	record.hand_num = ParseDwordArray(stmt.GetText(col++), record.hand, 64);
	record.foot_num = ParseDwordArray(stmt.GetText(col++), record.foot, 64);

	record.profession = static_cast<DWORD>(stmt.GetInt(col++));

	{
		auto text = stmt.GetText(col++);
		strncpy(record.description, text.data(), sizeof(record.description) - 1);
	}

	std::string name(record.szDataName);
	return {record.nID, std::move(name), std::move(record)};
}

void ChaCreateRecordStore::Insert(SqliteDatabase& db, const CChaCreateInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO cha_create "
			"(id,name,bone,faces,hairs,bodies,hands,feet,profession,description) "
			"VALUES (?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, static_cast<int>(r.bone));
		stmt.Bind(p++, SerializeDwordArray(r.face, r.face_num));
		stmt.Bind(p++, SerializeDwordArray(r.hair, r.hair_num));
		stmt.Bind(p++, SerializeDwordArray(r.body, r.body_num));
		stmt.Bind(p++, SerializeDwordArray(r.hand, r.hand_num));
		stmt.Bind(p++, SerializeDwordArray(r.foot, r.foot_num));
		stmt.Bind(p++, static_cast<int>(r.profession));
		stmt.Bind(p++, std::string_view(r.description));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ChaCreateRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CChaCreateInfo* GetChaCreateInfo(int nTypeID, const std::source_location& loc) {
	return ChaCreateRecordStore::Instance()->Get(nTypeID, loc);
}
