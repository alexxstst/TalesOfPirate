#pragma once

#include "GameRecordset.h"
#include "ResourceRecord.h"

// Хранилище таблицы ресурсов на базе SQLite.
class ResourceRecordStore : public GameRecordset<CResourceInfo> {
public:
	static ResourceRecordStore* Instance() {
		static ResourceRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "resource_info";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS resource_info (
			id        INTEGER PRIMARY KEY,
			data_name TEXT,
			type      INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM resource_info ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static void Insert(SqliteDatabase& db, const CResourceInfo& record);

	// Перегрузка Insert для MPResourceInfo (Engine) — копирует базовые поля
	template <typename T>
	static void InsertFrom(SqliteDatabase& db, const T& src) {
		CResourceInfo r{};
		r.nID = src.nID;
		r.bExist = src.bExist;
		strncpy(r.szDataName, src.szDataName, sizeof(r.szDataName) - 1);
		r.szDataName[sizeof(r.szDataName) - 1] = '\0';
		r.m_iType = src.m_iType;
		Insert(db, r);
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};

CResourceInfo* GetResourceInfo(int nID, const std::source_location& loc = std::source_location::current());
CResourceInfo* GetResourceInfo(const char* pszName, const std::source_location& loc = std::source_location::current());
