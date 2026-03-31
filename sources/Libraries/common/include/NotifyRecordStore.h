#pragma once
#include "GameRecordset.h"
#include "NotifyRecord.h"

class NotifyRecordStore : public GameRecordset<CNotifyInfo> {
public:
	static NotifyRecordStore* Instance() { static NotifyRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "notifies";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS notifies (id INTEGER PRIMARY KEY, name TEXT, type INTEGER, info TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM notifies ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const CNotifyInfo& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CNotifyInfo* GetNotifyInfo(int nTypeID) { return NotifyRecordStore::Instance()->Get(nTypeID); }
