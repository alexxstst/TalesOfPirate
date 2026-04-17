#include "ItemLitStore.h"

GameRecordset<ItemLitRecord>::RecordEntry ItemLitStore::ReadRecord(SqliteStatement& stmt) {
	ItemLitRecord r{};
	int col = 0;
	r.ItemId         = stmt.GetInt(col++);
	r.LitId          = stmt.GetInt(col++);
	r.ItemDescriptor = std::string(stmt.GetText(col++));
	r.ItemFile       = std::string(stmt.GetText(col++));
	r.TexFile        = std::string(stmt.GetText(col++));
	r.AnimType       = stmt.GetInt(col++);
	r.TranspType     = stmt.GetInt(col++);
	r.Opacity        = static_cast<float>(stmt.GetDouble(col++));

	// Ключ — пара (ItemId, LitId), базовые индексы GameRecordset не используем.
	return {0, std::string{}, std::move(r)};
}

bool ItemLitStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void ItemLitStore::Insert(SqliteDatabase& db, const ItemLitRecord& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO item_lit "
			"(item_id, lit_id, item_descriptor, item_file, tex_file, "
			" anim_type, transp_type, opacity) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		stmt.Bind(1, r.ItemId);
		stmt.Bind(2, r.LitId);
		stmt.Bind(3, std::string_view(r.ItemDescriptor));
		stmt.Bind(4, std::string_view(r.ItemFile));
		stmt.Bind(5, std::string_view(r.TexFile));
		stmt.Bind(6, r.AnimType);
		stmt.Bind(7, r.TranspType);
		stmt.Bind(8, static_cast<double>(r.Opacity));
		stmt.Step();
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "ItemLitStore::Insert(item_id={}, lit_id={}) failed: {}",
					 r.ItemId, r.LitId, e.what());
	}
}

std::vector<const ItemLitRecord*> ItemLitStore::GetByItemId(int item_id) const {
	std::vector<const ItemLitRecord*> result;
	ForEach([&](const ItemLitRecord& r) {
		if (r.ItemId == item_id) {
			result.push_back(&r);
		}
	});
	return result;
}

const ItemLitRecord* ItemLitStore::Find(int item_id, int lit_id) const {
	const ItemLitRecord* result = nullptr;
	ForEach([&](const ItemLitRecord& r) {
		if (result) {
			return;
		}
		if (r.ItemId == item_id && r.LitId == lit_id) {
			result = &r;
		}
	});
	return result;
}
