#pragma once

// Хранилище lit-эффектов. Живёт в render.sqlite (не gamedata.sqlite).
//
// Использование:
//   auto& renderDb = RenderAssetDatabase::Instance()->GetDb();
//   LitEntryStore::Instance()->Load(renderDb);
//   auto* rec = LitEntryStore::Instance()->Find(0, "0000060002.lgo");

#include "GameRecordset.h"
#include "LitEntryRecord.h"
#include <string_view>

class LitEntryStore : public GameRecordset<LitEntryRecord> {
public:
	static LitEntryStore* Instance() {
		static LitEntryStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "lit_entries";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS lit_entries (
			obj_type   INT  NOT NULL,
			file       TEXT NOT NULL,
			anim_type  INT  NOT NULL,
			mask       TEXT NOT NULL DEFAULT '',
			sub_id     INT  NOT NULL DEFAULT 0,
			color_op   INT  NOT NULL DEFAULT 0,
			textures   TEXT NOT NULL DEFAULT '',
			PRIMARY KEY (obj_type, file)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT obj_type, file, anim_type, mask, sub_id, color_op, textures "
		"FROM lit_entries ORDER BY obj_type, file";

	// Загрузить записи из таблицы lit_entries.
	bool Load(SqliteDatabase& db);

	// Вставить/обновить запись.
	static void Insert(SqliteDatabase& db, const LitEntryRecord& record);

	// Найти по (obj_type, file). Сравнение по file — case-insensitive (как _tcsicmp).
	// nullptr если нет.
	const LitEntryRecord* Find(int obj_type, std::string_view file) const;

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
