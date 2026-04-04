#pragma once

// Хранилище языковых строк на базе SQLite.
//
// Единая таблица для двух типов строк:
//   - По числовому ID (из StringSet.txt): GetString(42)
//   - По строковому ключу (из .res ресурсов): GetKeyString("GM_WEATHER_CPP_00001")
//
// Использование:
//   LanguageRecordStore::Instance()->Load(db, "en_US", LanguageTarget::Client);
//   const std::string& text = LanguageRecordStore::Instance()->GetString(42);
//   const std::string& res  = LanguageRecordStore::Instance()->GetKeyString("MY_KEY");

#include "GameRecordset.h"
#include <string>
#include <fstream>
#include <functional>

// Контекст использования языковых строк
enum class LanguageTarget : int {
	Client = 0,
	Server = 1
};

// Запись таблицы языковых строк
struct LanguageStringRecord {
	int _id{0};
	std::string _key{};         // Строковый ключ (для ресурсов из .res). Пустой для StringSet.
	std::string _text{};
	std::string _language{};
	LanguageTarget _target{LanguageTarget::Client};
};

// Хранилище языковых строк на базе SQLite.
class LanguageRecordStore : public GameRecordset<LanguageStringRecord> {
public:
	static LanguageRecordStore* Instance() {
		static LanguageRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "language_strings";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS language_strings (
			id       INTEGER NOT NULL DEFAULT 0,
			key      TEXT    NOT NULL DEFAULT '',
			language TEXT    NOT NULL DEFAULT 'en_US',
			target   INTEGER NOT NULL DEFAULT 0,
			text     TEXT    NOT NULL DEFAULT '',
			PRIMARY KEY (id, key, language, target)
		)
	)";

	// Миграция: добавить колонку key если таблица уже существует без неё
	static void MigrateSchema(SqliteDatabase& db);

	// Загрузить строки для конкретного языка и контекста
	bool Load(SqliteDatabase& db, std::string_view language, LanguageTarget target);

	// Получить строку по числовому ID. Пустая строка если не найдена.
	const std::string& GetString(int id, const std::source_location& loc = std::source_location::current());

	// Получить строку по строковому ключу. Пустая строка если не найдена.
	const std::string& GetKeyString(std::string_view key, const std::source_location& loc = std::source_location::current());

	// Вставить/обновить запись
	static void Insert(SqliteDatabase& db, const LanguageStringRecord& record);

	// Импортировать строки из StringSet.txt в SQLite.
	static int ImportFromTextFile(SqliteDatabase& db, const std::string& filePath,
								  std::string_view language, LanguageTarget target);

	// Импортировать ресурсы из CResourceBundleManage в SQLite.
	// Принимает callback-итератор: fn(const char* key, const char* value).
	static int ImportFromResourceBundle(SqliteDatabase& db, std::string_view language,
										LanguageTarget target,
										const std::function<void(std::function<void(const char*, const char*)>)>& forEach);

	// Гарантировать создание таблицы
	static void EnsureTable(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		MigrateSchema(db);
	}

	const std::string& GetLanguage() const {
		return _language;
	}

	LanguageTarget GetTarget() const {
		return _target;
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	std::string _language{"english"};
	LanguageTarget _target{LanguageTarget::Client};

	// Обработка escape-последовательностей (\n, \t) в строке
	static void ProcessEscapes(std::string& text);
};

// Глобальная функция-обёртка для совместимости с GetString()
const std::string&  GetLanguageString(int id, const std::source_location& loc = std::source_location::current());
