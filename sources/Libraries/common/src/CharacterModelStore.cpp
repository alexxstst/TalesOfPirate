#include <CharacterModelStore.h>

#include <charconv>
#include <fstream>
#include <string>
#include <string_view>

namespace {
	void TrimRight(std::string& s) {
		while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) {
			s.pop_back();
		}
	}

	void TrimLeft(std::string& s) {
		size_t p = 0;
		while (p < s.size() && (s[p] == ' ' || s[p] == '\t')) {
			++p;
		}
		if (p > 0) {
			s.erase(0, p);
		}
	}
} // namespace

GameRecordset<CCharacterModelInfo>::RecordEntry CharacterModelStore::ReadRecord(SqliteStatement& stmt) {
	CCharacterModelInfo record{};
	int col = 0;

	record._characterType = static_cast<short>(stmt.GetInt(col++));
	record._bone = stmt.GetText(col++);
	for (int i = 0; i < 5; i++) {
		record._skins[i] = stmt.GetText(col++);
	}

	record.bExist = TRUE;
	record.nID = static_cast<int>(record._characterType);

	return {record.nID, std::string{}, std::move(record)};
}

bool CharacterModelStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

	int rowCount = 0;
	auto stmt = db.Prepare("SELECT COUNT(*) FROM character_models");
	if (stmt.Step()) {
		rowCount = stmt.GetInt(0);
	}

	if (rowCount == 0) {
		ToLogService("common",
					 "CharacterModelStore: таблица пуста, импорт из '{}'", DEFAULT_TXT_PATH);
		ImportFromTxtFile(db, DEFAULT_TXT_PATH);
	}

	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void CharacterModelStore::Insert(SqliteDatabase& db, const CCharacterModelInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO character_models "
			"(character_type, bone, skin1, skin2, skin3, skin4, skin5) "
			"VALUES (?, ?, ?, ?, ?, ?, ?)");
		stmt.Bind(1, static_cast<int>(r._characterType));
		stmt.Bind(2, std::string_view{r._bone});
		for (int i = 0; i < 5; i++) {
			stmt.Bind(3 + i, std::string_view{r._skins[i]});
		}
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "CharacterModelStore::Insert(type={}) failed: {}",
					 r._characterType, e.what());
	}
}

int CharacterModelStore::ImportFromTxtFile(SqliteDatabase& db, const char* path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		ToLogService("errors", LogLevel::Error,
					 "CharacterModelStore::ImportFromTxtFile: не удалось открыть '{}'", path);
		return 0;
	}

	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

	int imported = 0;
	try {
		auto tx = db.BeginTransaction();
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO character_models "
			"(character_type, bone, skin1, skin2, skin3, skin4, skin5) "
			"VALUES (?, ?, ?, ?, ?, ?, ?)");

		int currentType = 0;
		std::string bone;
		std::array<std::string, 5> skins;
		bool haveSection = false;

		auto flush = [&]() {
			if (!haveSection) {
				return;
			}
			stmt.Reset();
			stmt.Bind(1, currentType);
			stmt.Bind(2, std::string_view{bone});
			for (int i = 0; i < 5; i++) {
				stmt.Bind(3 + i, std::string_view{skins[i]});
			}
			stmt.Step();
			++imported;
			bone.clear();
			for (auto& s : skins) {
				s.clear();
			}
		};

		std::string line;
		while (std::getline(file, line)) {
			TrimRight(line);
			TrimLeft(line);
			if (line.empty()) {
				continue;
			}

			if (line.front() == '[' && line.back() == ']') {
				flush();
				int t = 0;
				auto view = std::string_view(line).substr(1, line.size() - 2);
				std::from_chars(view.data(), view.data() + view.size(), t);
				if (t < 1) {
					haveSection = false;
					continue;
				}
				currentType = t;
				haveSection = true;
				continue;
			}

			if (!haveSection) {
				continue;
			}

			auto eq = line.find('=');
			if (eq == std::string::npos) {
				continue;
			}

			std::string key = line.substr(0, eq);
			std::string value = line.substr(eq + 1);
			TrimRight(key);
			TrimLeft(value);

			if (key == "bone") {
				bone = std::move(value);
			}
			else if (key.size() == 5 && key.starts_with("skin")) {
				char d = key[4];
				if (d >= '1' && d <= '5') {
					skins[d - '1'] = std::move(value);
				}
			}
		}
		flush();

		tx.Commit();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "CharacterModelStore::ImportFromTxtFile('{}') failed: {}", path, e.what());
		return imported;
	}

	ToLogService("common",
				 "CharacterModelStore: импорт из '{}' — {} секций", path, imported);
	return imported;
}
