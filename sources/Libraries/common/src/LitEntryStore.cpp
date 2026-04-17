#include "LitEntryStore.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace {
	std::string JoinCsv(const std::vector<std::string>& parts) {
		std::string s;
		for (size_t i = 0; i < parts.size(); ++i) {
			if (i > 0) s += ',';
			s += parts[i];
		}
		return s;
	}

	std::vector<std::string> SplitCsv(std::string_view text) {
		std::vector<std::string> out;
		size_t start = 0;
		while (start <= text.size()) {
			size_t end = text.find(',', start);
			if (end == std::string_view::npos) end = text.size();
			if (end > start) {
				out.emplace_back(text.substr(start, end - start));
			}
			if (end == text.size()) break;
			start = end + 1;
		}
		return out;
	}

	bool IEquals(std::string_view a, std::string_view b) {
		if (a.size() != b.size()) return false;
		return std::equal(a.begin(), a.end(), b.begin(),
						  [](char x, char y) {
							  return std::tolower(static_cast<unsigned char>(x)) ==
									 std::tolower(static_cast<unsigned char>(y));
						  });
	}

	void BindRecord(SqliteStatement& stmt, const LitEntryRecord& r, const std::string& texturesCsv) {
		stmt.Bind(1, r._obj_type);
		stmt.Bind(2, std::string_view(r._file));
		stmt.Bind(3, r._anim_type);
		stmt.Bind(4, std::string_view(r._mask));
		stmt.Bind(5, r._sub_id);
		stmt.Bind(6, r._color_op);
		stmt.Bind(7, std::string_view(texturesCsv));
	}
} // namespace

GameRecordset<LitEntryRecord>::RecordEntry LitEntryStore::ReadRecord(SqliteStatement& stmt) {
	LitEntryRecord r{};
	int col = 0;
	r._obj_type  = stmt.GetInt(col++);
	r._file      = std::string(stmt.GetText(col++));
	r._anim_type = stmt.GetInt(col++);
	r._mask      = std::string(stmt.GetText(col++));
	r._sub_id    = stmt.GetInt(col++);
	r._color_op  = stmt.GetInt(col++);
	r._textures  = SplitCsv(stmt.GetText(col++));

	// Поиск через Find(obj_type, file) / ForEach — базовые int/name индексы
	// GameRecordset не задействуем (составной ключ, int-id бессмыслен).
	return {0, std::string{}, std::move(r)};
}

bool LitEntryStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void LitEntryStore::Insert(SqliteDatabase& db, const LitEntryRecord& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO lit_entries "
			"(obj_type, file, anim_type, mask, sub_id, color_op, textures) "
			"VALUES (?, ?, ?, ?, ?, ?, ?)");
		BindRecord(stmt, r, JoinCsv(r._textures));
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "LitEntryStore::Insert(obj_type={}, file='{}') failed: {}",
					 r._obj_type, r._file, e.what());
	}
}

const LitEntryRecord* LitEntryStore::Find(int obj_type, std::string_view file) const {
	const LitEntryRecord* result = nullptr;
	ForEach([&](const LitEntryRecord& r) {
		if (result) {
			return;
		}
		if (r._obj_type == obj_type && IEquals(r._file, file)) {
			result = &r;
		}
	});
	return result;
}
