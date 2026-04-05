#pragma once

// Хранилище данных анимаций персонажей (CharacterAction) на базе SQLite.
// Составной ключ (character_type, action_id) — не наследует GameRecordset<T>.
// Хранит «плоские» данные без зависимости от Engine/Client.

#include <SqliteDatabase.h>
#include <vector>
#include <string>
#include <logutil.h>

// Запись одного действия (без зависимости от Engine-типов)
struct PoseDataRecord {
	int characterType = 0;
	int actionId = 0;
	int startFrame = 0;
	int endFrame = 0;
	int keyframeCount = 0;
	int keyframes[8] = {};
};

class PoseDataStore {
public:
	static PoseDataStore* Instance() {
		static PoseDataStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "pose_data";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS pose_data (
			character_type INTEGER NOT NULL,
			action_id      INTEGER NOT NULL,
			start_frame    INTEGER NOT NULL,
			end_frame      INTEGER NOT NULL,
			keyframes      TEXT NOT NULL DEFAULT '',
			PRIMARY KEY (character_type, action_id)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT character_type, action_id, start_frame, end_frame, keyframes "
		"FROM pose_data ORDER BY character_type, action_id";

	// Загрузить данные из SQLite в память
	bool Load(SqliteDatabase& db);

	// Количество загруженных записей
	int GetCount() const { return static_cast<int>(_records.size()); }

	// Максимальный character_type
	int GetMaxCharType() const { return _maxCharType; }

	// Получить все записи для данного character_type
	// Возвращает span: указатель + количество. nullptr если тип не найден.
	const PoseDataRecord* GetActions(int charType, int* outCount) const;

	// Получить конкретное действие
	const PoseDataRecord* GetAction(int charType, int actionId) const;

	// Вставить одну запись в SQLite (для миграции)
	static void Insert(SqliteDatabase& db, const PoseDataRecord& record);

	// Очистить таблицу (для миграции)
	static void Clear(SqliteDatabase& db);

	// Создать таблицу если не существует
	static void EnsureCreated(SqliteDatabase& db);

private:
	// _charTypes[charType - 1] → {offset, count} в _records
	struct CharTypeIndex {
		int offset = 0;
		int count = 0;
		int maxActionId = 0;
	};

	std::vector<PoseDataRecord> _records;
	std::vector<CharTypeIndex> _charTypes;
	int _maxCharType = 0;
};
