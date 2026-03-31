#pragma once
#include "GameRecordset.h"
#include "ChatIconRecord.h"

class ChatIconRecordStore : public GameRecordset<CChatIconInfo> {
public:
	static ChatIconRecordStore* Instance() { static ChatIconRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "chat_icons";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS chat_icons (id INTEGER PRIMARY KEY, name TEXT, small_x INTEGER, small_y INTEGER, small_off TEXT, small_off_x INTEGER, small_off_y INTEGER, big TEXT, big_x INTEGER, big_y INTEGER, hint TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM chat_icons ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const CChatIconInfo& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

inline CChatIconInfo* GetChatIconInfo(int nIconID) { return ChatIconRecordStore::Instance()->Get(nIconID); }
