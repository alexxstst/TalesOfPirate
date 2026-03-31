#pragma once
#include "GameRecordset.h"
#include "EffParamRecord.h"

class EffParamRecordStore : public GameRecordset<EFF_Param> {
public:
	static EffParamRecordStore* Instance() { static EffParamRecordStore i{}; return &i; }
	static constexpr const char* TABLE_NAME = "eff_params";
	static constexpr const char* CREATE_TABLE_SQL = R"(CREATE TABLE IF NOT EXISTS eff_params (id INTEGER PRIMARY KEY, name TEXT, models TEXT, vel INTEGER, parts TEXT, dummies TEXT, render_idx INTEGER, light_id INTEGER, result TEXT))";
	static constexpr const char* SELECT_ALL_SQL = "SELECT * FROM eff_params ORDER BY id";
	bool Load(SqliteDatabase& db) { EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL); return GameRecordset::Load(db, SELECT_ALL_SQL); }
	static void Insert(SqliteDatabase& db, const EFF_Param& r);
protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
