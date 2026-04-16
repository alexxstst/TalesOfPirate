#pragma once

#include "GameRecordset.h"
#include "CharacterActionRecord.h"

// Хранилище действий персонажей (таблица character_actions).
// Одна запись = (character_type, action_no, start_frame, end_frame, key_frames CSV).
class CharacterActionStore : public GameRecordset<CCharacterActionInfo>
{
public:
	static CharacterActionStore* Instance() {
		static CharacterActionStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "character_actions";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS character_actions (
			character_type INTEGER NOT NULL,
			action_no      INTEGER NOT NULL,
			start_frame    INTEGER NOT NULL,
			end_frame      INTEGER NOT NULL,
			key_frames     TEXT,
			PRIMARY KEY (character_type, action_no)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT character_type, action_no, start_frame, end_frame, key_frames "
		"FROM character_actions ORDER BY character_type, action_no";

	// Загрузить записи из SQLite.
	bool Load(SqliteDatabase& db);

	// Вставить одну запись (INSERT OR REPLACE).
	static void Insert(SqliteDatabase& db, const CCharacterActionInfo& record);

	// Максимальный character_type среди загруженных записей.
	short GetMaxCharacterType() const { return _maxCharacterType; }

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	short _maxCharacterType{0};
};
