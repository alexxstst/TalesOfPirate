#pragma once
#include "GameRecordset.h"
#include "GroupParamRecord.h"

class GroupParamRecordStore : public GameRecordset<Group_Param> {
public:
	static GroupParamRecordStore* Instance() { static GroupParamRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "group_params";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS group_params (id INTEGER PRIMARY KEY, name TEXT, type_ids TEXT, nums TEXT, render_idx INTEGER))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM group_params ORDER BY id";
	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		if (!GameRecordset::Load(db, SELECT_ALL_SQL)) return false;
		ForEach([](Group_Param& p) {
			p.nTotalNum = 0;
			for (int i = 0; i < 8; i++) p.nTotalNum += p.nNum[i];
		});
		return true;
	}
	static void Insert(SqliteDatabase& db, const Group_Param& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
