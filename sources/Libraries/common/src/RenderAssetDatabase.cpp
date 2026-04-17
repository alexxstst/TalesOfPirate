#include "RenderAssetDatabase.h"

#include <format>
#include <logutil.h>
#include <stdexcept>

namespace {
	constexpr const char* CREATE_SQLAR_SQL = R"(
		CREATE TABLE IF NOT EXISTS sqlar (
			name  TEXT PRIMARY KEY,
			mode  INT  NOT NULL DEFAULT 0,
			mtime INT  NOT NULL DEFAULT 0,
			sz    INT  NOT NULL,
			data  BLOB
		)
	)";

	bool TableExists(SqliteDatabase& db, std::string_view tableName) {
		auto stmt = db.Prepare("SELECT 1 FROM sqlite_master WHERE type='table' AND name=?");
		stmt.Bind(1, tableName);
		return stmt.Step();
	}
} // namespace

void RenderAssetDatabase::Open(std::string_view path) {
	_db.reset();
	_db = std::make_unique<SqliteDatabase>(path);
	EnsureSchema();
	ToLogService("common", "RenderAssetDatabase: {}", std::string(path));
}

SqliteDatabase& RenderAssetDatabase::GetDb() {
	if (!_db) {
		throw std::runtime_error("RenderAssetDatabase не инициализирован. Вызовите Open() при старте.");
	}
	return *_db;
}

void RenderAssetDatabase::EnsureSchema() {
	auto& db = *_db;

	// page_size и auto_vacuum работают только до создания первой таблицы —
	// ставим их если БД только что создалась (таблицы sqlar ещё нет).
	const bool isFresh = !TableExists(db, "sqlar");
	if (isFresh) {
		db.Execute("PRAGMA page_size = 8192");
		db.Execute("PRAGMA auto_vacuum = INCREMENTAL");
	}

	// journal_mode безопасно переставлять в любой момент.
	db.Execute("PRAGMA journal_mode = WAL");

	db.Execute(CREATE_SQLAR_SQL);
}

std::vector<uint8_t> RenderAssetDatabase::ReadBlob(std::string_view name) {
	auto& db = GetDb();
	auto stmt = db.Prepare("SELECT sz, data FROM sqlar WHERE name = ?");
	stmt.Bind(1, name);
	if (!stmt.Step()) {
		return {};
	}

	const int64_t sz = stmt.GetInt64(0);
	const auto data = stmt.GetBlob(1);

	if (sz < 0 || sz != static_cast<int64_t>(data.size())) {
		throw SqliteException(0, std::format(
			"RenderAssetDatabase::ReadBlob('{}'): unsupported SQLAR entry "
			"(sz={}, data.size()={}). Only raw (sz==size) entries supported.",
			name, sz, data.size()));
	}

	return std::vector<uint8_t>(data.begin(), data.end());
}

void RenderAssetDatabase::WriteBlob(std::string_view name, std::span<const uint8_t> data) {
	auto& db = GetDb();
	auto stmt = db.Prepare(
		"INSERT OR REPLACE INTO sqlar (name, mode, mtime, sz, data) "
		"VALUES (?, 0, 0, ?, ?)");
	stmt.Bind(1, name);
	stmt.Bind(2, static_cast<int64_t>(data.size()));
	stmt.Bind(3, data);
	stmt.Step();
}

bool RenderAssetDatabase::HasBlob(std::string_view name) {
	auto& db = GetDb();
	auto stmt = db.Prepare("SELECT 1 FROM sqlar WHERE name = ?");
	stmt.Bind(1, name);
	return stmt.Step();
}
