#include <ChaRecordStore.h>
#include <sstream>

// ============================================================================
// Вспомогательные функции парсинга
// ============================================================================

void ChaRecordStore::ParseShortArray(std::string_view text, short* out, int maxLen, short defaultVal) {
	std::fill(out, out + maxLen, defaultVal);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<short>(std::atoi(token.c_str()));
	}
}

void ChaRecordStore::ParseLongArray(std::string_view text, long* out, int maxLen, long defaultVal) {
	std::fill(out, out + maxLen, defaultVal);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = std::atol(token.c_str());
	}
}

void ChaRecordStore::ParseCharArray(std::string_view text, char* out, int maxLen) {
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

void ChaRecordStore::ParseIntArray(std::string_view text, int* out, int maxLen) {
	std::fill(out, out + maxLen, 0);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = std::atoi(token.c_str());
	}
}

void ChaRecordStore::ParseFloatArray(std::string_view text, float* out, int maxLen) {
	std::fill(out, out + maxLen, 0.0f);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<float>(std::atof(token.c_str()));
	}
}

// ============================================================================
// ReadRecord — конструирует CChaRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CChaRecord>::RecordEntry ChaRecordStore::ReadRecord(SqliteStatement& stmt) {
	CChaRecord record{};
	int col = 0;

	// id, name
	record.lID = stmt.GetInt(col++);
	record.Id = static_cast<int>(record.lID);
	record.szName = stmt.GetText(col++);
	record.DataName = record.szName;

	// icon_name
	record.szIconName = stmt.GetText(col++);

	// modal_type, ctrl_type
	record.chModalType = static_cast<char>(stmt.GetInt(col++));
	record.chCtrlType  = static_cast<char>(stmt.GetInt(col++));

	// model, suit_id, suit_num
	record.sModel   = static_cast<short>(stmt.GetInt(col++));
	record.sSuitID  = static_cast<short>(stmt.GetInt(col++));
	record.sSuitNum = static_cast<short>(stmt.GetInt(col++));

	// skin_info[8] — TEXT
	memset(record.sSkinInfo, cchChaRecordKeyValue, sizeof(record.sSkinInfo));
	ParseShortArray(stmt.GetText(col++), record.sSkinInfo, defCHA_SKIN_NUM, static_cast<short>(cchChaRecordKeyValue));

	// feff_id[4] — TEXT
	memset(record.sFeffID, 0, sizeof(record.sFeffID));
	ParseShortArray(stmt.GetText(col++), record.sFeffID, 4);

	// eeff_id
	record.sEeffID = static_cast<short>(stmt.GetInt(col++));

	// effect_action_id[3] — TEXT
	memset(record.sEffectActionID, 0, sizeof(record.sEffectActionID));
	ParseShortArray(stmt.GetText(col++), record.sEffectActionID, 3);

	// shadow, action_id, diaphaneity
	record.sShadow       = static_cast<short>(stmt.GetInt(col++));
	record.sActionID     = static_cast<short>(stmt.GetInt(col++));
	record.chDiaphaneity = static_cast<char>(stmt.GetInt(col++));

	// footfall, whoop, dirge
	record.sFootfall = static_cast<short>(stmt.GetInt(col++));
	record.sWhoop    = static_cast<short>(stmt.GetInt(col++));
	record.sDirge    = static_cast<short>(stmt.GetInt(col++));

	// control_able, territory, sea_height
	record.chControlAble = static_cast<char>(stmt.GetInt(col++));
	record.chTerritory   = static_cast<char>(stmt.GetInt(col++));
	record.sSeaHeight    = static_cast<short>(stmt.GetInt(col++));

	// item_type[20] — TEXT
	memset(record.sItemType, cchChaRecordKeyValue, sizeof(record.sItemType));
	ParseShortArray(stmt.GetText(col++), record.sItemType, defCHA_ITEM_KIND_NUM, static_cast<short>(cchChaRecordKeyValue));

	// lengh, width, height, radii
	record.fLengh = static_cast<float>(stmt.GetDouble(col++));
	record.fWidth = static_cast<float>(stmt.GetDouble(col++));
	record.fHeight = static_cast<float>(stmt.GetDouble(col++));
	record.sRadii = static_cast<short>(stmt.GetInt(col++));

	// birth_behave[3] — TEXT
	memset(record.nBirthBehave, 0, sizeof(record.nBirthBehave));
	ParseCharArray(stmt.GetText(col++), record.nBirthBehave, defCHA_BIRTH_EFFECT_NUM);

	// died_behave[3] — TEXT
	memset(record.nDiedBehave, 0, sizeof(record.nDiedBehave));
	ParseCharArray(stmt.GetText(col++), record.nDiedBehave, defCHA_DIE_EFFECT_NUM);

	// born_eff, die_eff, dormancy, die_action
	record.sBornEff    = static_cast<short>(stmt.GetInt(col++));
	record.sDieEff     = static_cast<short>(stmt.GetInt(col++));
	record.sDormancy   = static_cast<short>(stmt.GetInt(col++));
	record.chDieAction = static_cast<char>(stmt.GetInt(col++));

	// hp_effect[3] — TEXT
	memset(record._nHPEffect, 0, sizeof(record._nHPEffect));
	ParseIntArray(stmt.GetText(col++), record._nHPEffect, defCHA_HP_EFFECT_NUM);

	// is_face, is_cyclone
	record._IsFace    = stmt.GetInt(col++) != 0;
	record._IsCyclone = stmt.GetInt(col++) != 0;

	// script, weapon
	record.lScript = stmt.GetInt(col++);
	record.lWeapon = stmt.GetInt(col++);

	// skill_ids[11] — TEXT, skill_lvs[11] — TEXT
	memset(record.lSkill, cchChaRecordKeyValue, sizeof(record.lSkill));
	{
		long ids[defCHA_INIT_SKILL_NUM]{};
		ParseLongArray(stmt.GetText(col++), ids, defCHA_INIT_SKILL_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_SKILL_NUM; i++)
			record.lSkill[i][0] = ids[i];

		long lvs[defCHA_INIT_SKILL_NUM]{};
		ParseLongArray(stmt.GetText(col++), lvs, defCHA_INIT_SKILL_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_SKILL_NUM; i++)
			record.lSkill[i][1] = lvs[i];
	}

	// item_ids[15] — TEXT, item_counts[15] — TEXT
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++) {
		record.lItem[i][0] = cchChaRecordKeyValue;
		record.lItem[i][1] = cchChaRecordKeyValue;
	}
	{
		long ids[defCHA_INIT_ITEM_NUM]{};
		ParseLongArray(stmt.GetText(col++), ids, defCHA_INIT_ITEM_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
			record.lItem[i][0] = ids[i];

		long counts[defCHA_INIT_ITEM_NUM]{};
		ParseLongArray(stmt.GetText(col++), counts, defCHA_INIT_ITEM_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
			record.lItem[i][1] = counts[i];
	}

	// max_show_item, all_show, prefix
	record.lMaxShowItem = stmt.GetInt(col++);
	record.fAllShow     = static_cast<float>(stmt.GetDouble(col++));
	record.lPrefix      = stmt.GetInt(col++);

	// task_item_ids[15] — TEXT, task_item_counts[15] — TEXT
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++) {
		record.lTaskItem[i][0] = cchChaRecordKeyValue;
		record.lTaskItem[i][1] = cchChaRecordKeyValue;
	}
	{
		long ids[defCHA_INIT_ITEM_NUM]{};
		ParseLongArray(stmt.GetText(col++), ids, defCHA_INIT_ITEM_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
			record.lTaskItem[i][0] = ids[i];

		long counts[defCHA_INIT_ITEM_NUM]{};
		ParseLongArray(stmt.GetText(col++), counts, defCHA_INIT_ITEM_NUM, static_cast<long>(cchChaRecordKeyValue));
		for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
			record.lTaskItem[i][1] = counts[i];
	}

	// ai_no, can_turn, vision, noise, get_exp, light
	record.lAiNo    = stmt.GetInt(col++);
	record.chCanTurn = static_cast<char>(stmt.GetInt(col++));
	record.lVision  = stmt.GetInt(col++);
	record.lNoise   = stmt.GetInt(col++);
	record.lGetEXP  = stmt.GetInt(col++);
	record.bLight   = stmt.GetInt(col++) != 0;

	// mobexp, lv, mx_hp, hp, mx_sp, sp
	record.lMobexp = stmt.GetInt(col++);
	record.lLv     = stmt.GetInt(col++);
	record.lMxHp   = stmt.GetInt(col++);
	record.lHp     = stmt.GetInt(col++);
	record.lMxSp   = stmt.GetInt(col++);
	record.lSp     = stmt.GetInt(col++);

	// mn_atk, mx_atk, p_def, def, hit, flee
	record.lMnAtk = stmt.GetInt(col++);
	record.lMxAtk = stmt.GetInt(col++);
	record.lPDef  = stmt.GetInt(col++);
	record.lDef   = stmt.GetInt(col++);
	record.lHit   = stmt.GetInt(col++);
	record.lFlee  = stmt.GetInt(col++);

	// crt, mf, h_rec, s_rec, a_spd, a_dis, c_dis, m_spd, col
	record.lCrt  = stmt.GetInt(col++);
	record.lMf   = stmt.GetInt(col++);
	record.lHRec = stmt.GetInt(col++);
	record.lSRec = stmt.GetInt(col++);
	record.lASpd = stmt.GetInt(col++);
	record.lADis = stmt.GetInt(col++);
	record.lCDis = stmt.GetInt(col++);
	record.lMSpd = stmt.GetInt(col++);
	record.lCol  = stmt.GetInt(col++);

	// str, agi, dex, con, sta, luk, l_hand_val
	record.lStr      = stmt.GetInt(col++);
	record.lAgi      = stmt.GetInt(col++);
	record.lDex      = stmt.GetInt(col++);
	record.lCon      = stmt.GetInt(col++);
	record.lSta      = stmt.GetInt(col++);
	record.lLuk      = stmt.GetInt(col++);
	record.lLHandVal = stmt.GetInt(col++);

	// guild, title, job
	record.szGuild = stmt.GetText(col++);
	record.szTitle = stmt.GetText(col++);
	record.szJob = stmt.GetText(col++);

	// c_exp, n_exp, fame, ap, tp, gd, spri, stor
	record.lCExp = stmt.GetInt(col++);
	record.lNExp = stmt.GetInt(col++);
	record.lFame = stmt.GetInt(col++);
	record.lAp   = stmt.GetInt(col++);
	record.lTp   = stmt.GetInt(col++);
	record.lGd   = stmt.GetInt(col++);
	record.lSpri = stmt.GetInt(col++);
	record.lStor = stmt.GetInt(col++);

	// mx_sail, sail, stasa, scsm
	record.lMxSail = stmt.GetInt(col++);
	record.lSail   = stmt.GetInt(col++);
	record.lStasa  = stmt.GetInt(col++);
	record.lScsm   = stmt.GetInt(col++);

	// t_str .. t_scsm (21 бонусных характеристик)
	record.lTStr  = stmt.GetInt(col++);
	record.lTAgi  = stmt.GetInt(col++);
	record.lTDex  = stmt.GetInt(col++);
	record.lTCon  = stmt.GetInt(col++);
	record.lTSta  = stmt.GetInt(col++);
	record.lTLuk  = stmt.GetInt(col++);
	record.lTMxHp = stmt.GetInt(col++);
	record.lTMxSp = stmt.GetInt(col++);
	record.lTAtk  = stmt.GetInt(col++);
	record.lTDef  = stmt.GetInt(col++);
	record.lTHit  = stmt.GetInt(col++);
	record.lTFlee = stmt.GetInt(col++);
	record.lTMf   = stmt.GetInt(col++);
	record.lTCrt  = stmt.GetInt(col++);
	record.lTHRec = stmt.GetInt(col++);
	record.lTSRec = stmt.GetInt(col++);
	record.lTASpd = stmt.GetInt(col++);
	record.lTADis = stmt.GetInt(col++);
	record.lTSpd  = stmt.GetInt(col++);
	record.lTSpri = stmt.GetInt(col++);
	record.lTScsm = stmt.GetInt(col++);

	// scaling[3] — TEXT
	memset(record.scaling, 0, sizeof(record.scaling));
	ParseFloatArray(stmt.GetText(col++), record.scaling, 3);

	record.RefreshPrivateData();

	return {record.Id, record.szName, std::move(record)};
}
