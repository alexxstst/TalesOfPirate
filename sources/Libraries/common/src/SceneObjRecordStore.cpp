#include <SceneObjRecordStore.h>
#include <sstream>

static std::string SerializeByteArray(const BYTE* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(arr[i]);
	}
	return result;
}

static void ParseByteArray(std::string_view text, BYTE* out, int maxLen) {
	std::fill(out, out + maxLen, BYTE{0});
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<BYTE>(std::stoul(token));
	}
}

static std::string SerializeIntArray(const int* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(arr[i]);
	}
	return result;
}

static void ParseIntArray(std::string_view text, int* out, int maxLen) {
	std::fill(out, out + maxLen, 0);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = std::stoi(token);
	}
}

GameRecordset<CSceneObjInfo>::RecordEntry SceneObjRecordStore::ReadRecord(SqliteStatement& stmt) {
	CSceneObjInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	// data_name
	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}
	// name
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
	}

	record.nType = stmt.GetInt(col++);

	// point_color — "r,g,b"
	{
		auto text = stmt.GetText(col++);
		ParseByteArray(text, record.btPointColor, 3);
	}

	// env_color — "r,g,b"
	{
		auto text = stmt.GetText(col++);
		ParseByteArray(text, record.btEnvColor, 3);
	}

	record.nRange        = stmt.GetInt(col++);
	record.Attenuation1  = static_cast<float>(stmt.GetDouble(col++));
	record.nAnimCtrlID   = stmt.GetInt(col++);
	record.nAttachEffectID = stmt.GetInt(col++);
	record.bEnableEnvLight   = stmt.GetInt(col++);
	record.bEnablePointLight = stmt.GetInt(col++);
	record.nStyle        = stmt.GetInt(col++);
	record.nFlag         = stmt.GetInt(col++);
	record.nSizeFlag     = stmt.GetInt(col++);

	// env_sound
	{
		auto text = stmt.GetText(col++);
		strncpy(record.szEnvSound, text.data(), sizeof(record.szEnvSound) - 1);
		record.szEnvSound[sizeof(record.szEnvSound) - 1] = '\0';
	}

	record.nEnvSoundDis  = stmt.GetInt(col++);
	record.bShadeFlag    = stmt.GetInt(col++);
	record.bIsReallyBig  = stmt.GetInt(col++);
	record.nFadeObjNum   = stmt.GetInt(col++);

	// fade_obj_seq — "s0,s1,..."
	{
		auto text = stmt.GetText(col++);
		ParseIntArray(text, record.nFadeObjSeq, 16);
	}

	record.fFadeCoefficent = static_cast<float>(stmt.GetDouble(col++));

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void SceneObjRecordStore::Insert(SqliteDatabase& db, const CSceneObjInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO scene_objects "
			"(id,data_name,name,type,point_color,env_color,range,attenuation,anim_ctrl_id,"
			"attach_effect_id,enable_env_light,enable_point_light,style,flag,size_flag,"
			"env_sound,env_sound_dis,shade_flag,is_really_big,fade_obj_num,fade_obj_seq,fade_coefficient) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, r.nType);
		stmt.Bind(p++, SerializeByteArray(r.btPointColor, 3));
		stmt.Bind(p++, SerializeByteArray(r.btEnvColor, 3));
		stmt.Bind(p++, r.nRange);
		stmt.Bind(p++, static_cast<double>(r.Attenuation1));
		stmt.Bind(p++, r.nAnimCtrlID);
		stmt.Bind(p++, r.nAttachEffectID);
		stmt.Bind(p++, static_cast<int>(r.bEnableEnvLight));
		stmt.Bind(p++, static_cast<int>(r.bEnablePointLight));
		stmt.Bind(p++, r.nStyle);
		stmt.Bind(p++, r.nFlag);
		stmt.Bind(p++, r.nSizeFlag);
		stmt.Bind(p++, std::string_view(r.szEnvSound));
		stmt.Bind(p++, r.nEnvSoundDis);
		stmt.Bind(p++, static_cast<int>(r.bShadeFlag));
		stmt.Bind(p++, static_cast<int>(r.bIsReallyBig));
		stmt.Bind(p++, r.nFadeObjNum);
		stmt.Bind(p++, SerializeIntArray(r.nFadeObjSeq, r.nFadeObjNum));
		stmt.Bind(p++, static_cast<double>(r.fFadeCoefficent));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "SceneObjRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}
