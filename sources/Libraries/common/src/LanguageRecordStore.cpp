#include <LanguageRecordStore.h>

bool LanguageRecordStore::Load(SqliteDatabase& db, std::string_view language, LanguageTarget target) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	MigrateSchema(db);
	_language = language;
	_target = target;

	auto query = std::format(
		"SELECT id, language, target, text FROM language_strings "
		"WHERE language='{}' AND target={} ORDER BY id",
		language, static_cast<int>(target));

	return GameRecordset::Load(db, query);
}

GameRecordset<LanguageStringRecord>::RecordEntry LanguageRecordStore::ReadRecord(SqliteStatement& stmt) {
	LanguageStringRecord record{};
	int col = 0;

	record._id = stmt.GetInt(col++);
	record._language = std::string(stmt.GetText(col++));
	record._target = static_cast<LanguageTarget>(stmt.GetInt(col++));
	record._text = std::string(stmt.GetText(col++));

	return {record._id, {}, std::move(record)};
}

const std::string& LanguageRecordStore::GetString(int id, const std::source_location& loc) {
	auto* record = Get(id, loc);
	if (!record) return "";
	return record->_text;
}

void LanguageRecordStore::Insert(SqliteDatabase& db, const LanguageStringRecord& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO language_strings (id, language, target, text) VALUES (?, ?, ?, ?)");
		stmt.Bind(1, r._id);
		stmt.Bind(2, std::string_view(r._language));
		stmt.Bind(3, static_cast<int>(r._target));
		stmt.Bind(4, std::string_view(r._text));
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "LanguageRecordStore::Insert(id={}) failed: {}", r._id, e.what());
	}
}

void LanguageRecordStore::ProcessEscapes(std::string& text) {
	std::string result;
	result.reserve(text.size());

	for (size_t i = 0; i < text.size(); ++i) {
		if (text[i] == '\\' && i + 1 < text.size()) {
			switch (text[i + 1]) {
			case 'n': result += '\n';
				++i;
				break;
			case 't': result += '\t';
				++i;
				break;
			default: result += text[i];
				break;
			}
		}
		else {
			result += text[i];
		}
	}

	text = std::move(result);
}

int LanguageRecordStore::ImportFromTextFile(SqliteDatabase& db, const std::string& filePath,
											std::string_view language, LanguageTarget target) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		ToLogService("errors", LogLevel::Error,
					 "LanguageRecordStore::ImportFromTextFile: не удалось открыть '{}'", filePath);
		return 0;
	}

	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);

	auto tx = db.BeginTransaction();
	int count = 0;
	std::string line;

	while (std::getline(file, line)) {
		if (line.empty()) continue;

		// Формат: [ID]<TAB>"text"
		auto bracketStart = line.find('[');
		auto bracketEnd = line.find(']');
		if (bracketStart == std::string::npos || bracketEnd == std::string::npos || bracketEnd <= bracketStart)
			continue;

		int id = 0;
		try {
			id = std::stoi(line.substr(bracketStart + 1, bracketEnd - bracketStart - 1));
		}
		catch (...) {
			continue;
		}

		// Ищем текст в кавычках после TAB
		auto quoteStart = line.find('"', bracketEnd);
		auto quoteEnd = line.rfind('"');
		if (quoteStart == std::string::npos || quoteEnd == std::string::npos || quoteStart == quoteEnd)
			continue;

		std::string text = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
		ProcessEscapes(text);

		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO language_strings (id, language, target, text) VALUES (?, ?, ?, ?)");
		stmt.Bind(1, id);
		stmt.Bind(2, language);
		stmt.Bind(3, static_cast<int>(target));
		stmt.Bind(4, std::string_view(text));
		stmt.Step();
		++count;
	}

	tx.Commit();

	ToLogService("common", "LanguageRecordStore: импортировано {} строк из '{}' (lang={}, target={})",
				 count, filePath, language, static_cast<int>(target));
	return count;
}

void LanguageRecordStore::MigrateSchema(SqliteDatabase& db) {
	try {
		// Проверяем наличие колонки key через pragma table_info
		auto stmt = db.Prepare("PRAGMA table_info(language_strings)");
		bool hasKey = false;
		while (stmt.Step()) {
			auto colName = stmt.GetText(1);
			if (colName == "key") {
				hasKey = true;
				break;
			}
		}
		if (!hasKey) {
			db.Execute("ALTER TABLE language_strings ADD COLUMN key TEXT NOT NULL DEFAULT ''");
		}
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "LanguageRecordStore::MigrateSchema failed: {}", e.what());
	}
}

int LanguageRecordStore::ImportFromResourceBundle(SqliteDatabase& db, std::string_view language,
												  LanguageTarget target,
												  const std::function<void(std::function<void(const char*, const char*)>)>& forEach) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	MigrateSchema(db);

	auto tx = db.BeginTransaction();
	int count = 0;

	// Стартовый id = MAX(id) + 1 (или 1 если таблица пуста)
	int nextId = 1;
	{
		auto maxStmt = db.Prepare("SELECT COALESCE(MAX(id), 0) FROM language_strings");
		if (maxStmt.Step()) {
			nextId = maxStmt.GetInt(0) + 1;
		}
	}

	forEach([&](const char* key, const char* value) {
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO language_strings (id, key, language, target, text) VALUES (?, ?, ?, ?, ?)");
		stmt.Bind(1, nextId++);
		stmt.Bind(2, std::string_view(key));
		stmt.Bind(3, language);
		stmt.Bind(4, static_cast<int>(target));
		stmt.Bind(5, std::string_view(value));
		stmt.Step();
		++count;
	});

	tx.Commit();

	ToLogService("common", "LanguageRecordStore: импортировано {} ресурсов (lang={}, target={})",
				 count, language, static_cast<int>(target));
	return count;
}

const std::string& LanguageRecordStore::GetKeyString(std::string_view key, const std::source_location& loc) {
	static const std::string empty;
	const std::string* result = &empty;
	ForEach([&](const LanguageStringRecord& record) {
		if (record._key == key) {
			result = &record._text;
		}
	});
	return *result;
}

const std::string& GetLanguageString(int id, const std::source_location& loc) {
	return LanguageRecordStore::Instance()->GetString(id, loc);
}
