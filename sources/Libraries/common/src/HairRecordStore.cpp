#include <HairRecordStore.h>
#include <sstream>

// ============================================================================
// Вспомогательные функции парсинга
// ============================================================================

void HairRecordStore::ParseDwordArray(std::string_view text, DWORD* out, int maxLen) {
	std::fill(out, out + maxLen, DWORD{0});
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = static_cast<DWORD>(std::stoul(token));
	}
}

void HairRecordStore::ParseBoolArray(std::string_view text, bool* out, int maxLen) {
	std::fill(out, out + maxLen, false);
	if (text.empty() || text == "0") return;

	std::string s(text);
	std::istringstream ss(s);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ',') && i < maxLen) {
		out[i++] = (std::atoi(token.c_str()) != 0);
	}
}

// ============================================================================
// ReadRecord — конструирует CHairRecord из строки SQLite-запроса
// ============================================================================

GameRecordset<CHairRecord>::RecordEntry HairRecordStore::ReadRecord(SqliteStatement& stmt) {
	CHairRecord record{};
	int col = 0;

	// id
	record.Id    = stmt.GetInt(col++);

	// color
	record.szColor = stmt.GetText(col++);

	// need_items — dwNeedItem[4][2], хранится как "id0,cnt0,id1,cnt1,id2,cnt2,id3,cnt3"
	{
		DWORD flat[defHAIR_MAX_ITEM * 2]{};
		ParseDwordArray(stmt.GetText(col++), flat, defHAIR_MAX_ITEM * 2);
		for (int i = 0; i < defHAIR_MAX_ITEM; i++) {
			record.dwNeedItem[i][0] = flat[i * 2];
			record.dwNeedItem[i][1] = flat[i * 2 + 1];
		}
	}

	// money
	record.dwMoney = static_cast<DWORD>(stmt.GetInt(col++));

	// item_id
	record.dwItemID = static_cast<DWORD>(stmt.GetInt(col++));

	// fail_item_ids — dwFailItemID[3]
	ParseDwordArray(stmt.GetText(col++), record.dwFailItemID, defHAIR_MAX_FAIL_ITEM);

	// is_cha_use — bool[4]
	ParseBoolArray(stmt.GetText(col++), record.IsChaUse, 4);

	record.RefreshPrivateData();

	return {record.Id, {}, std::move(record)};
}

// ============================================================================
// Insert — временный метод миграции
// ============================================================================

static std::string JoinDwords(const DWORD* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += std::to_string(arr[i]);
	}
	return result;
}

static std::string JoinBools(const bool* arr, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		if (i > 0) result += ',';
		result += arr[i] ? '1' : '0';
	}
	return result;
}

