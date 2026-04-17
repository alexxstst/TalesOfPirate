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
