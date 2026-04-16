#include <CharacterActionStore.h>

#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>
#include <vector>

namespace {
	// Парсит CSV-строку из short-чисел в вектор. Пустая строка -> пустой вектор.
	std::vector<short> ParseKeyFramesCsv(std::string_view text) {
		std::vector<short> result;
		size_t start = 0;
		while (start <= text.size()) {
			size_t end = text.find(',', start);
			if (end == std::string_view::npos) end = text.size();
			auto token = text.substr(start, end - start);
			if (!token.empty()) {
				int value = 0;
				auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);
				if (ec == std::errc{}) {
					result.push_back(static_cast<short>(value));
				}
			}
			if (end == text.size()) break;
			start = end + 1;
		}
		return result;
	}
} // namespace

GameRecordset<CCharacterActionInfo>::RecordEntry CharacterActionStore::ReadRecord(SqliteStatement& stmt) {
	CCharacterActionInfo record{};
	int col = 0;

	record._characterType = static_cast<short>(stmt.GetInt(col++));
	record._actionNo = static_cast<short>(stmt.GetInt(col++));
	record._startFrame = static_cast<short>(stmt.GetInt(col++));
	record._endFrame = static_cast<short>(stmt.GetInt(col++));
	record._keyFrames = ParseKeyFramesCsv(stmt.GetText(col++));

	record.bExist = TRUE;

	// Синтетический ID: уникален при action_no < 10000 (гарантировано коротким типом).
	int id = static_cast<int>(record._characterType) * 10000 + static_cast<int>(record._actionNo);
	record.nID = id;

	_maxCharacterType = (std::max)(record._characterType, _maxCharacterType);

	return {id, std::string{}, std::move(record)};
}

bool CharacterActionStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	_maxCharacterType = 0;
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void CharacterActionStore::Insert(SqliteDatabase& db, const CCharacterActionInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

		std::string ids;
		for (size_t i = 0; i < r._keyFrames.size(); i++) {
			if (i > 0) ids += ',';
			ids += std::to_string(r._keyFrames[i]);
		}

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO character_actions "
			"(character_type, action_no, start_frame, end_frame, key_frames) "
			"VALUES (?, ?, ?, ?, ?)");
		stmt.Bind(1, static_cast<int>(r._characterType));
		stmt.Bind(2, static_cast<int>(r._actionNo));
		stmt.Bind(3, static_cast<int>(r._startFrame));
		stmt.Bind(4, static_cast<int>(r._endFrame));
		stmt.Bind(5, std::string_view{ids});
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "CharacterActionStore::Insert(type={}, action={}) failed: {}",
					 r._characterType, r._actionNo, e.what());
	}
}

