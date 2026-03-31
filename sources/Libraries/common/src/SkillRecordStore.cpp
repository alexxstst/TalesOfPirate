#include <SkillRecordStore.h>
#include <sstream>

// ============================================================================
// Вспомогательные функции десериализации
// ============================================================================

void SkillRecordStore::ParseShortArray(std::string_view text, short* out, int maxLen) {
	std::fill(out, out + maxLen, short{0});
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<short>(std::atoi(token.c_str()));
	}
}

void SkillRecordStore::ParseCharArray(std::string_view text, char* out, int maxLen) {
	std::fill(out, out + maxLen, char{0});
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<char>(std::atoi(token.c_str()));
	}
}

void SkillRecordStore::ParseCharPairs(std::string_view text, char* out, int rows, int cols, char sentinel) {
	// Заполнить sentinel-значениями
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			out[i * cols + j] = sentinel;

	if (text.empty()) return;

	std::string s(text);
	std::istringstream ss(s);
	std::string pair;
	int row = 0;
	while (std::getline(ss, pair, ';') && row < rows) {
		std::istringstream ps(pair);
		std::string val;
		int col = 0;
		while (std::getline(ps, val, ',') && col < cols) {
			out[row * cols + col] = static_cast<char>(std::atoi(val.c_str()));
			col++;
		}
		row++;
	}
}

void SkillRecordStore::ParseShortPairs(std::string_view text, short* out, int rows, int cols, short sentinel) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			out[i * cols + j] = sentinel;

	if (text.empty()) return;

	std::string s(text);
	std::istringstream ss(s);
	std::string pair;
	int row = 0;
	while (std::getline(ss, pair, ';') && row < rows) {
		std::istringstream ps(pair);
		std::string val;
		int col = 0;
		while (std::getline(ps, val, ',') && col < cols) {
			out[row * cols + col] = static_cast<short>(std::atoi(val.c_str()));
			col++;
		}
		row++;
	}
}

void SkillRecordStore::ParseShortTriples(std::string_view text, short* out, int rows, int cols, short sentinel) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			out[i * cols + j] = sentinel;

	if (text.empty()) return;

	std::string s(text);
	std::istringstream ss(s);
	std::string triple;
	int row = 0;
	while (std::getline(ss, triple, ';') && row < rows) {
		std::istringstream ps(triple);
		std::string val;
		int col = 0;
		while (std::getline(ps, val, ',') && col < cols) {
			out[row * cols + col] = static_cast<short>(std::atoi(val.c_str()));
			col++;
		}
		row++;
	}
}

// ============================================================================
// ReadRecord — конструирует CSkillRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CSkillRecord>::RecordEntry SkillRecordStore::ReadRecord(SqliteStatement& stmt) {
	CSkillRecord record{};
	int col = 0;

	// id, name
	record.sID  = static_cast<short>(stmt.GetInt(col++));
	record.nID  = static_cast<int>(record.sID);
	record.bExist = TRUE;

	record.szName = stmt.GetText(col++);
	strncpy(record.szDataName, record.szName.c_str(), sizeof(record.szDataName) - 1);
	record.szDataName[sizeof(record.szDataName) - 1] = '\0';

	// fight_type
	record.chFightType = static_cast<char>(stmt.GetInt(col++));

	// job_select -> chJobSelect[9][2]
	ParseCharPairs(stmt.GetText(col++), &record.chJobSelect[0][0],
		defSKILL_JOB_SELECT_NUM, 2, cchSkillRecordKeyValue);

	// item_need_1, item_need_2, item_need_3 -> sItemNeed[3][8][2]
	for (int i = 0; i < 3; i++) {
		ParseShortPairs(stmt.GetText(col++), &record.sItemNeed[i][0][0],
			defSKILL_ITEM_NEED_NUM, 2, static_cast<short>(cchSkillRecordKeyValue));
	}

	// conch_need -> sConchNeed[8][3]
	ParseShortTriples(stmt.GetText(col++), &record.sConchNeed[0][0],
		defSKILL_ITEM_NEED_NUM, 3, static_cast<short>(cchSkillRecordKeyValue));

	// Скалярные поля
	record.chPhase      = static_cast<char>(stmt.GetInt(col++));
	record.chType       = static_cast<char>(stmt.GetInt(col++));
	record.chHelpful    = static_cast<char>(stmt.GetInt(col++));
	record.sLevelDemand = static_cast<short>(stmt.GetInt(col++));

	// premiss_skill -> sPremissSkill[3][2]
	ParseShortPairs(stmt.GetText(col++), &record.sPremissSkill[0][0],
		defSKILL_PRE_SKILL_NUM, 2, static_cast<short>(cchSkillRecordKeyValue));

	record.chPointExpend = static_cast<char>(stmt.GetInt(col++));
	record.chSrcType     = static_cast<char>(stmt.GetInt(col++));
	record.chTarType     = static_cast<char>(stmt.GetInt(col++));
	record.sApplyDistance = static_cast<short>(stmt.GetInt(col++));
	record.chApplyTarget = static_cast<char>(stmt.GetInt(col++));
	record.chApplyType   = static_cast<char>(stmt.GetInt(col++));
	record.sAngle        = static_cast<short>(stmt.GetInt(col++));
	record.sRadii        = static_cast<short>(stmt.GetInt(col++));
	record.chRange       = static_cast<char>(stmt.GetInt(col++));

	// Строковые скрипты
	record.szPrepare    = stmt.GetText(col++);
	record.szRangeState = stmt.GetText(col++);
	record.szUseSP      = stmt.GetText(col++);
	record.szUseEndure  = stmt.GetText(col++);
	record.szUseEnergy  = stmt.GetText(col++);
	record.szSetRange   = stmt.GetText(col++);
	record.szUse        = stmt.GetText(col++);
	record.szEffect     = stmt.GetText(col++);
	record.szActive     = stmt.GetText(col++);
	record.szInactive   = stmt.GetText(col++);

	record.nStateID = stmt.GetInt(col++);

	// Скалярные поля (после пропущенных в оригинале столбцов)
	record.sSplashPara   = static_cast<short>(stmt.GetInt(col++));
	record.sTargetEffect = static_cast<short>(stmt.GetInt(col++));
	record.sSplashEffect = static_cast<short>(stmt.GetInt(col++));
	record.sVariation    = static_cast<short>(stmt.GetInt(col++));
	record.sSummon       = static_cast<short>(stmt.GetInt(col++));

	record.szFireSpeed = stmt.GetText(col++);

	record.sActionHarm      = static_cast<short>(stmt.GetInt(col++));
	record.chActionPlayType = static_cast<char>(stmt.GetInt(col++));

	// action_pose -> sActionPose[10]
	ParseShortArray(stmt.GetText(col++), record.sActionPose, defSKILL_POSE_NUM);

	record.sActionKeyFrme = static_cast<short>(stmt.GetInt(col++));
	record.sWhop          = static_cast<short>(stmt.GetInt(col++));

	// action_dummy_link[3], action_effect[3], action_effect_type[3]
	ParseShortArray(stmt.GetText(col++), record.sActionDummyLink,  defSKILL_ACTION_EFFECT);
	ParseShortArray(stmt.GetText(col++), record.sActionEffect,     defSKILL_ACTION_EFFECT);
	ParseShortArray(stmt.GetText(col++), record.sActionEffectType, defSKILL_ACTION_EFFECT);

	record.sItemDummyLink = static_cast<short>(stmt.GetInt(col++));

	// item_effect_1[2], item_effect_2[2]
	ParseShortArray(stmt.GetText(col++), record.sItemEffect1, defSKILL_ITEM_EFFECT);
	ParseShortArray(stmt.GetText(col++), record.sItemEffect2, defSKILL_ITEM_EFFECT);

	record.sSkyEffectActionKeyFrame  = static_cast<short>(stmt.GetInt(col++));
	record.sSkyEffectActionDummyLink = static_cast<short>(stmt.GetInt(col++));
	record.sSkyEffectItemDummyLink   = static_cast<short>(stmt.GetInt(col++));
	record.sSkyEffect                = static_cast<short>(stmt.GetInt(col++));
	record.sSkySpd                   = static_cast<short>(stmt.GetInt(col++));
	record.sWhoped                   = static_cast<short>(stmt.GetInt(col++));

	record.sTargetDummyLink   = static_cast<short>(stmt.GetInt(col++));
	record.sTargetEffectID    = static_cast<short>(stmt.GetInt(col++));
	record.chTargetEffectTime = static_cast<char>(stmt.GetInt(col++));
	record.sAgroundEffectID   = static_cast<short>(stmt.GetInt(col++));
	record.sWaterEffectID     = static_cast<short>(stmt.GetInt(col++));

	// icon
	record.szICON = stmt.GetText(col++);

	record.chPlayTime = static_cast<char>(stmt.GetInt(col++));

	// operate -> chOperate[3]
	ParseCharArray(stmt.GetText(col++), record.chOperate, defSKILL_OPERATE_NUM);

	// Хинты
	record.szDescribeHint = stmt.GetText(col++);
	record.szEffectHint   = stmt.GetText(col++);
	record.szExpendHint   = stmt.GetText(col++);

	// Обновить приватные данные (eSelectCha, nPoseNum, ...)
	record.RefreshPrivateData();

	return {record.nID, record.szName, std::move(record)};
}
