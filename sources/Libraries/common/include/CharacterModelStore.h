#pragma once

#include "GameRecordset.h"
#include "CharacterModelRecord.h"

// Хранилище моделей персонажей (таблица character_models).
// Одна запись = (character_type, bone, skin1..skin5).
// При первом Load() таблица создаётся, и если пуста — автоматически импортируется
// из INI-файла `effect/model.txt` (секции `[N]`, ключи `bone` и `skin1..5`).
class CharacterModelStore : public GameRecordset<CCharacterModelInfo>
{
public:
	static CharacterModelStore* Instance() {
		static CharacterModelStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "character_models";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS character_models (
			character_type INTEGER PRIMARY KEY,
			bone  TEXT NOT NULL DEFAULT '',
			skin1 TEXT NOT NULL DEFAULT '',
			skin2 TEXT NOT NULL DEFAULT '',
			skin3 TEXT NOT NULL DEFAULT '',
			skin4 TEXT NOT NULL DEFAULT '',
			skin5 TEXT NOT NULL DEFAULT ''
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT character_type, bone, skin1, skin2, skin3, skin4, skin5 "
		"FROM character_models ORDER BY character_type";

	// Путь по умолчанию к INI-файлу (используется при авто-импорте в пустую таблицу).
	static constexpr const char* DEFAULT_TXT_PATH = "effect/model.txt";

	// Загрузить записи из SQLite. При пустой таблице автоматически импортирует .txt-файл.
	bool Load(SqliteDatabase& db);

	// Вставить одну запись (INSERT OR REPLACE).
	static void Insert(SqliteDatabase& db, const CCharacterModelInfo& record);

	// Импортировать из INI-файла в SQLite (INSERT OR REPLACE в транзакции).
	// Возвращает число импортированных секций (0 если ошибка открытия).
	static int ImportFromTxtFile(SqliteDatabase& db, const char* path);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
