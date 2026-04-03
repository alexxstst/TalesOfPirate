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

	record._id       = stmt.GetInt(col++);
	record._dataName = std::string(stmt.GetText(col++));
	record._name     = std::string(stmt.GetText(col++));

	record._type = stmt.GetInt(col++);

	// point_color — "r,g,b"
	{
		auto text = stmt.GetText(col++);
		ParseByteArray(text, record._pointColor, 3);
	}

	// env_color — "r,g,b"
	{
		auto text = stmt.GetText(col++);
		ParseByteArray(text, record._envColor, 3);
	}

	record._range        = stmt.GetInt(col++);
	record._attenuation  = static_cast<float>(stmt.GetDouble(col++));
	record._animCtrlId   = stmt.GetInt(col++);
	record._attachEffectId = stmt.GetInt(col++);
	record._enableEnvLight   = stmt.GetInt(col++);
	record._enablePointLight = stmt.GetInt(col++);
	record._style        = stmt.GetInt(col++);
	record._flag         = stmt.GetInt(col++);
	record._sizeFlag     = stmt.GetInt(col++);

	record._envSound = std::string(stmt.GetText(col++));

	record._envSoundDis  = stmt.GetInt(col++);
	record._shadeFlag    = stmt.GetInt(col++);
	record._isReallyBig  = stmt.GetInt(col++);
	record._fadeObjNum   = stmt.GetInt(col++);

	// fade_obj_seq — "s0,s1,..."
	{
		auto text = stmt.GetText(col++);
		ParseIntArray(text, record._fadeObjSeq, 16);
	}

	record._fadeCoefficient = static_cast<float>(stmt.GetDouble(col++));

	return {record._id, record._dataName, std::move(record)};
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
		stmt.Bind(p++, r._id);
		stmt.Bind(p++, std::string_view(r._dataName));
		stmt.Bind(p++, r._name);
		stmt.Bind(p++, r._type);
		stmt.Bind(p++, SerializeByteArray(r._pointColor, 3));
		stmt.Bind(p++, SerializeByteArray(r._envColor, 3));
		stmt.Bind(p++, r._range);
		stmt.Bind(p++, static_cast<double>(r._attenuation));
		stmt.Bind(p++, r._animCtrlId);
		stmt.Bind(p++, r._attachEffectId);
		stmt.Bind(p++, static_cast<int>(r._enableEnvLight));
		stmt.Bind(p++, static_cast<int>(r._enablePointLight));
		stmt.Bind(p++, r._style);
		stmt.Bind(p++, r._flag);
		stmt.Bind(p++, r._sizeFlag);
		stmt.Bind(p++, std::string_view(r._envSound));
		stmt.Bind(p++, r._envSoundDis);
		stmt.Bind(p++, static_cast<int>(r._shadeFlag));
		stmt.Bind(p++, static_cast<int>(r._isReallyBig));
		stmt.Bind(p++, r._fadeObjNum);
		stmt.Bind(p++, SerializeIntArray(r._fadeObjSeq, r._fadeObjNum));
		stmt.Bind(p++, static_cast<double>(r._fadeCoefficient));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "SceneObjRecordStore::Insert(id={}) failed: {}", r._id, e.what());
	}
}

CSceneObjInfo* GetSceneObjInfo(int nTypeID, const std::source_location& loc) {
	return SceneObjRecordStore::Instance()->Get(nTypeID, loc);
}
