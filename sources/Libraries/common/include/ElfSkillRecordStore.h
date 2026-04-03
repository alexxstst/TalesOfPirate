#pragma once

#include "GameRecordset.h"
#include "ElfSkillRecord.h"

// Хранилище таблицы навыков эльфов на базе SQLite.
class ElfSkillRecordStore : public GameRecordset<CElfSkillInfo> {
public:
	static ElfSkillRecordStore* Instance() {
		static ElfSkillRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "elf_skill_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS elf_skill_info (
			id          INTEGER PRIMARY KEY,
			data_name   TEXT,
			skill_index INTEGER,
			type_id     INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM elf_skill_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CElfSkillInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CElfSkillInfo* GetElfSkillInfo(int nIndex, int nTypeID, const std::source_location& loc = std::source_location::current());
