#pragma once
#include "GameRecordset.h"
#include "ItemRefineEffectRecord.h"

class ItemRefineEffectRecordStore : public GameRecordset<CItemRefineEffectInfo> {
public:
	static ItemRefineEffectRecordStore* Instance() { static ItemRefineEffectRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "item_refine_effects";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS item_refine_effects (id INTEGER PRIMARY KEY, name TEXT, light_id INTEGER, effect_ids TEXT, dummies TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM item_refine_effects ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const CItemRefineEffectInfo& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CItemRefineEffectInfo* GetItemRefineEffectInfo(int nRefineID) { return ItemRefineEffectRecordStore::Instance()->Get(nRefineID); }
