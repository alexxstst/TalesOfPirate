#include <SkillStateRecordStore.h>
#include <cstring>
#include <sstream>

// ============================================================================
// Вспомогательные функции парсинга
// ============================================================================

static void ParseCharArray(std::string_view text, char* out, int maxLen) {
	std::memset(out, 0, maxLen);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<char>(std::atoi(token.c_str()));
	}
}

static void ParseIconArray(std::string_view text, char szIcon[][10], int maxSlots) {
	for (int i = 0; i < maxSlots; i++)
		szIcon[i][0] = '\0';
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxSlots) {
		std::strncpy(szIcon[i], token.c_str(), 9);
		szIcon[i][9] = '\0';
		i++;
	}
}

// ============================================================================
// ReadRecord — конструирует CSkillStateRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CSkillStateRecord>::RecordEntry SkillStateRecordStore::ReadRecord(SqliteStatement& stmt) {
	CSkillStateRecord record{};
	int col = 0;

	// id
	record.Id    = stmt.GetInt(col++);

	// ch_id, name
	record.chID = static_cast<char>(stmt.GetInt(col++));
	record.szName = stmt.GetText(col++);

	// frequency
	record.sFrequency = static_cast<short>(stmt.GetInt(col++));

	// скрипты
	record.szOnTransfer = stmt.GetText(col++);
	record.szAddState = stmt.GetText(col++);
	record.szSubState = stmt.GetText(col++);

	// add_type
	record.chAddType = static_cast<char>(stmt.GetInt(col++));

	// boolean-флаги
	record.bCanCancel    = stmt.GetInt(col++) != 0;
	record.bCanMove      = stmt.GetInt(col++) != 0;
	record.bCanMSkill    = stmt.GetInt(col++) != 0;
	record.bCanGSkill    = stmt.GetInt(col++) != 0;
	record.bCanTrade     = stmt.GetInt(col++) != 0;
	record.bCanItem      = stmt.GetInt(col++) != 0;
	record.bCanUnbeatable = stmt.GetInt(col++) != 0;
	record.bCanItemmed   = stmt.GetInt(col++) != 0;
	record.bCanSkilled   = stmt.GetInt(col++) != 0;
	record.bNoHide       = stmt.GetInt(col++) != 0;
	record.bNoShow       = stmt.GetInt(col++) != 0;
	record.bOptItem      = stmt.GetInt(col++) != 0;
	record.bTalkToNPC    = stmt.GetInt(col++) != 0;

	// free_state_id, screen
	record.bFreeStateID = static_cast<char>(stmt.GetInt(col++));
	record.chScreen     = static_cast<char>(stmt.GetInt(col++));

	// act_behave — TEXT с запятыми → char[3]
	ParseCharArray(stmt.GetText(col++), record.nActBehave, defSKILLSTATE_ACT_NUM);

	// charge_link, area_effect
	record.sChargeLink = static_cast<short>(stmt.GetInt(col++));
	record.sAreaEffect = static_cast<short>(stmt.GetInt(col++));

	// is_show_center, is_dizzy
	record.IsShowCenter = stmt.GetInt(col++) != 0;
	record.IsDizzy      = stmt.GetInt(col++) != 0;

	// effect, dummy1, bit_effect, dummy2, icon_id
	record.sEffect    = static_cast<short>(stmt.GetInt(col++));
	record.sDummy1    = static_cast<short>(stmt.GetInt(col++));
	record.sBitEffect = static_cast<short>(stmt.GetInt(col++));
	record.sDummy2    = static_cast<short>(stmt.GetInt(col++));
	record.sIcon      = static_cast<short>(stmt.GetInt(col++));

	// icons — TEXT с запятыми → char[17][10]
	ParseIconArray(stmt.GetText(col++), record.szIcon, defSKILLSTATE_NAME_LEN);

	// descriptor
	record.szDesc = stmt.GetText(col++);

	// colour
	record.lColour = stmt.GetInt(col++);

	// Пересчитать приватные данные (_nActNum)
	record.RefreshPrivateData();

	return {record.Id, {}, std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции
// ============================================================================

static std::string JoinChars(const char* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(static_cast<int>(arr[i]));
	}
	return result;
}

static std::string JoinIcons(const char szIcon[][10], int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (szIcon[i][0] == '\0') break;
		if (i > 0) result += ',';
		result += szIcon[i];
	}
	return result;
}

