#pragma once

// Хранилище пресетов свечения предметов по уровню заточки. Живёт в render.sqlite.
//
// Использование:
//   auto& renderDb = RenderAssetDatabase::Instance()->GetDb();
//   ItemLitStore::Instance()->Load(renderDb);
//   auto* rec = ItemLitStore::Instance()->Find(nLightID, level);

#include "GameRecordset.h"
#include "ItemLitRecord.h"
#include <string_view>
#include <vector>

class ItemLitStore : public GameRecordset<ItemLitRecord> {
public:
	static ItemLitStore* Instance() {
		static ItemLitStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "item_lit";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS item_lit (
			item_id         INT  NOT NULL,
			lit_id          INT  NOT NULL,
			item_descriptor TEXT NOT NULL DEFAULT '',
			item_file       TEXT NOT NULL DEFAULT '',
			tex_file        TEXT NOT NULL DEFAULT '',
			anim_type       INT  NOT NULL DEFAULT 0,
			transp_type     INT  NOT NULL DEFAULT 0,
			opacity         REAL NOT NULL DEFAULT 0,
			PRIMARY KEY (item_id, lit_id)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT item_id, lit_id, item_descriptor, item_file, tex_file, "
		"anim_type, transp_type, opacity FROM item_lit ORDER BY item_id, lit_id";

	// Загрузить записи из таблицы item_lit.
	bool Load(SqliteDatabase& db);

	// Вставить/обновить запись (INSERT OR REPLACE).
	static void Insert(SqliteDatabase& db, const ItemLitRecord& record);

	// Гарантировать создание таблицы без полной загрузки (для bulk-импорта в транзакции).
	static void EnsureTable(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	}

	// Все уровни свечения для заданного item_id, в порядке lit_id.
	std::vector<const ItemLitRecord*> GetByItemId(int item_id) const;

	// Конкретный уровень. nullptr если нет.
	const ItemLitRecord* Find(int item_id, int lit_id) const;

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
