#pragma once
#include "GameRecordset.h"
#include "ItemPreRecord.h"

class ItemPreRecordStore : public GameRecordset<CItemPreInfo> {
public:
	static ItemPreRecordStore* Instance() { static ItemPreRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "item_prefixes";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS item_prefixes (id INTEGER PRIMARY KEY, name TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM item_prefixes ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const CItemPreInfo& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CItemPreInfo* GetItemPreInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
