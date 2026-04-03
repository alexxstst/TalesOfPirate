#pragma once

#include "GameRecordset.h"
#include "ShadeRecord.h"

// Хранилище таблицы шейдовых эффектов на базе SQLite.
class ShadeRecordStore : public GameRecordset<CShadeInfo> {
public:
	static ShadeRecordStore* Instance() {
		static ShadeRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "shades";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS shades (
			id             INTEGER PRIMARY KEY,
			data_name      TEXT,
			name           TEXT,
			size           REAL,
			ani            INTEGER,
			row            INTEGER,
			col            INTEGER,
			use_alpha_test INTEGER,
			alpha_type     INTEGER,
			color_r        INTEGER,
			color_g        INTEGER,
			color_b        INTEGER,
			color_a        INTEGER,
			type           INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM shades ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CShadeInfo& record);

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CShadeInfo* GetShadeInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
