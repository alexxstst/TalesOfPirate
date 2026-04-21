#include <ItemRecordStore.h>
#include <sstream>

// ============================================================================
// Вспомогательные функции парсинга
// ============================================================================

void ItemRecordStore::ParsePair(std::string_view text, short& out0, short& out1) {
	out0 = 0;
	out1 = 0;
	if (text.empty() || text == "0") return;

	auto pos = text.find(',');
	if (pos == std::string_view::npos) {
		out0 = static_cast<short>(std::atoi(std::string(text).c_str()));
		return;
	}
	out0 = static_cast<short>(std::atoi(std::string(text.substr(0, pos)).c_str()));
	out1 = static_cast<short>(std::atoi(std::string(text.substr(pos + 1)).c_str()));
}

void ItemRecordStore::ParseCharArray(std::string_view text, std::int8_t* out, int maxLen, std::int8_t defaultVal) {
	std::fill(out, out + maxLen, defaultVal);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<std::int8_t>(std::atoi(token.c_str()));
	}
}

void ItemRecordStore::ParseShortArray(std::string_view text, short* out, int maxLen) {
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

// ============================================================================
// ReadRecord — конструирует CItemRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CItemRecord>::RecordEntry ItemRecordStore::ReadRecord(SqliteStatement& stmt) {
	CItemRecord record{};
	int col = 0;

	// id, name, icon
	record.lID   = stmt.GetInt(col++);
	record.Id   = static_cast<int>(record.lID);
	record.szName = stmt.GetText(col++);
	record.DataName = record.szName;
	record.szICON = stmt.GetText(col++);

	// module_0..4
	for (int i = 0; i < defITEM_MODULE_NUM; i++)
		record.chModule[i] = stmt.GetText(col++);

	// scalar fields
	record.sShipFlag     = static_cast<short>(stmt.GetInt(col++));
	record.sShipType     = static_cast<short>(stmt.GetInt(col++));
	record.sType         = static_cast<short>(stmt.GetInt(col++));
	record.chForgeLv     = static_cast<char>(stmt.GetInt(col++));
	record.chForgeSteady = static_cast<char>(stmt.GetInt(col++));
	record.chExclusiveID = static_cast<char>(stmt.GetInt(col++));
	record.chIsTrade     = static_cast<char>(stmt.GetInt(col++));
	record.chIsPick      = static_cast<char>(stmt.GetInt(col++));
	record.chIsThrow     = static_cast<char>(stmt.GetInt(col++));
	record.chIsDel       = static_cast<char>(stmt.GetInt(col++));
	record.lPrice        = stmt.GetInt(col++);

	// byte arrays
	ParseCharArray(stmt.GetText(col++), record.chBody.data(), defITEM_BODY, static_cast<std::int8_t>(0xFE));
	record.sNeedLv = static_cast<short>(stmt.GetInt(col++));
	ParseCharArray(stmt.GetText(col++), record.szWork.data(), MAX_JOB_TYPE, static_cast<std::int8_t>(0xFE));

	record.nPileMax  = stmt.GetInt(col++);
	record.chInstance = static_cast<char>(stmt.GetInt(col++));

	ParseCharArray(stmt.GetText(col++), record.szAbleLink.data(), enumEQUIP_NUM, static_cast<std::int8_t>(0xFE));
	ParseCharArray(stmt.GetText(col++), record.szNeedLink.data(), enumEQUIP_NUM, static_cast<std::int8_t>(0xFE));
	record.chPickTo = static_cast<char>(stmt.GetInt(col++));

	// 21 коэффициентов
	record.sStrCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sAgiCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sDexCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sConCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sStaCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sLukCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sASpdCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sADisCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sMnAtkCoef = static_cast<short>(stmt.GetInt(col++));
	record.sMxAtkCoef = static_cast<short>(stmt.GetInt(col++));
	record.sDefCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sMxHpCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sMxSpCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sFleeCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sHitCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sCrtCoef   = static_cast<short>(stmt.GetInt(col++));
	record.sMfCoef    = static_cast<short>(stmt.GetInt(col++));
	record.sHRecCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sSRecCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sMSpdCoef  = static_cast<short>(stmt.GetInt(col++));
	record.sColCoef   = static_cast<short>(stmt.GetInt(col++));

	// 21 пар значений
	ParsePair(stmt.GetText(col++), record.sStrValu[0],   record.sStrValu[1]);
	ParsePair(stmt.GetText(col++), record.sAgiValu[0],   record.sAgiValu[1]);
	ParsePair(stmt.GetText(col++), record.sDexValu[0],   record.sDexValu[1]);
	ParsePair(stmt.GetText(col++), record.sConValu[0],   record.sConValu[1]);
	ParsePair(stmt.GetText(col++), record.sStaValu[0],   record.sStaValu[1]);
	ParsePair(stmt.GetText(col++), record.sLukValu[0],   record.sLukValu[1]);
	ParsePair(stmt.GetText(col++), record.sASpdValu[0],  record.sASpdValu[1]);
	ParsePair(stmt.GetText(col++), record.sADisValu[0],  record.sADisValu[1]);
	ParsePair(stmt.GetText(col++), record.sMnAtkValu[0], record.sMnAtkValu[1]);
	ParsePair(stmt.GetText(col++), record.sMxAtkValu[0], record.sMxAtkValu[1]);
	ParsePair(stmt.GetText(col++), record.sDefValu[0],   record.sDefValu[1]);
	ParsePair(stmt.GetText(col++), record.sMxHpValu[0],  record.sMxHpValu[1]);
	ParsePair(stmt.GetText(col++), record.sMxSpValu[0],  record.sMxSpValu[1]);
	ParsePair(stmt.GetText(col++), record.sFleeValu[0],  record.sFleeValu[1]);
	ParsePair(stmt.GetText(col++), record.sHitValu[0],   record.sHitValu[1]);
	ParsePair(stmt.GetText(col++), record.sCrtValu[0],   record.sCrtValu[1]);
	ParsePair(stmt.GetText(col++), record.sMfValu[0],    record.sMfValu[1]);
	ParsePair(stmt.GetText(col++), record.sHRecValu[0],  record.sHRecValu[1]);
	ParsePair(stmt.GetText(col++), record.sSRecValu[0],  record.sSRecValu[1]);
	ParsePair(stmt.GetText(col++), record.sMSpdValu[0],  record.sMSpdValu[1]);
	ParsePair(stmt.GetText(col++), record.sColValu[0],   record.sColValu[1]);

	// p_def, l_hand_valu, endure, energy, hole
	ParsePair(stmt.GetText(col++), record.sPDef[0], record.sPDef[1]);
	record.sLHandValu = static_cast<short>(stmt.GetInt(col++));
	ParsePair(stmt.GetText(col++), record.sEndure[0], record.sEndure[1]);
	ParsePair(stmt.GetText(col++), record.sEnergy[0], record.sEnergy[1]);
	record.sHole = static_cast<short>(stmt.GetInt(col++));

	// attr_effect, drap
	record.szAttrEffect = stmt.GetText(col++);
	record.sDrap = static_cast<short>(stmt.GetInt(col++));

	// effects[8][2]
	{
		short ids[defITEM_BIND_EFFECT_NUM]{};
		ParseShortArray(stmt.GetText(col++), ids, defITEM_BIND_EFFECT_NUM);
		for (int e = 0; e < defITEM_BIND_EFFECT_NUM; e++)
			record.sEffect[e][0] = ids[e];

		short dummies[defITEM_BIND_EFFECT_NUM]{};
		ParseShortArray(stmt.GetText(col++), dummies, defITEM_BIND_EFFECT_NUM);
		for (int e = 0; e < defITEM_BIND_EFFECT_NUM; e++)
			record.sEffect[e][1] = dummies[e];

		record.sEffNum = 0;
		for (int e = 0; e < defITEM_BIND_EFFECT_NUM; e++) {
			if (ids[e] != 0) record.sEffNum = static_cast<short>(e + 1);
		}
		if (defITEM_BIND_EFFECT_NUM == 1 && ids[0] == 0)
			record.sEffNum = 0;
	}

	// item_effect, area_effect, use_item_effect
	ParsePair(stmt.GetText(col++), record.sItemEffect[0],    record.sItemEffect[1]);
	ParsePair(stmt.GetText(col++), record.sAreaEffect[0],    record.sAreaEffect[1]);
	ParsePair(stmt.GetText(col++), record.sUseItemEffect[0], record.sUseItemEffect[1]);

	// descriptor, cooldown
	auto desc = stmt.GetText(col++);
	record.szDescriptor = (desc == "0") ? "" : std::string(desc);

	record.nCooldown = static_cast<float>(stmt.GetDouble(col++));
	if (!record.szAttrEffect.empty() && record.nCooldown == 0)
		record.nCooldown = 1.0f;

	record.RefreshData();

	return {static_cast<int>(record.lID), record.szName, std::move(record)};
}
