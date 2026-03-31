#pragma once
#include "GameRecordset.h"
#include "ItemRefineRecord.h"

class ItemRefineRecordStore : public GameRecordset<CItemRefineInfo> {
public:
	static ItemRefineRecordStore* Instance() { static ItemRefineRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "item_refine";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS item_refine (id INTEGER PRIMARY KEY, name TEXT, refine_values TEXT, cha_effect_scale TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM item_refine ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const CItemRefineInfo& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CItemRefineInfo* GetItemRefineInfo(int nRefineID) { return ItemRefineRecordStore::Instance()->Get(nRefineID); }
