#include <SqliteDatabase.h>
#include <sqlite3.h>
#include <logutil.h>
#include <format>

// ============================================================================
// SqliteException helper
// ============================================================================

static std::string FormatError(sqlite3* db, int rc, const char* context) {
	if (context) {
		if (db) {
			return std::format("SQLite error [{}] in {}: {}", rc, context, sqlite3_errmsg(db));
		}
		return std::format("SQLite error [{}] in {}", rc, context);
	}
	if (db) {
		return std::format("SQLite error [{}]: {}", rc, sqlite3_errmsg(db));
	}
	return std::format("SQLite error [{}]", rc);
}

// ============================================================================
// SqliteDatabase
// ============================================================================

SqliteDatabase::SqliteDatabase(std::string_view path) {
	std::string pathStr(path);
	int rc = sqlite3_open(pathStr.c_str(), &_db);
	if (rc != SQLITE_OK) {
		std::string msg = FormatError(_db, rc, "open");
		if (_db) {
			sqlite3_close(_db);
			_db = nullptr;
		}
		throw SqliteException(rc, msg);
	}

	// WAL-режим для лучшей производительности при параллельном чтении
	Execute("PRAGMA journal_mode=WAL");
	Execute("PRAGMA synchronous=NORMAL");

	ToLogService("common", "SQLite database opened: {}", pathStr);
}

SqliteDatabase::~SqliteDatabase() {
	if (_db) {
		sqlite3_close(_db);
		_db = nullptr;
	}
}

SqliteDatabase::SqliteDatabase(SqliteDatabase&& other) noexcept
	: _db(other._db) {
	other._db = nullptr;
}

SqliteDatabase& SqliteDatabase::operator=(SqliteDatabase&& other) noexcept {
	if (this != &other) {
		if (_db) sqlite3_close(_db);
		_db = other._db;
		other._db = nullptr;
	}
	return *this;
}

void SqliteDatabase::Execute(std::string_view sql) {
	std::string sqlStr(sql);
	char* errMsg = nullptr;
	int rc = sqlite3_exec(_db, sqlStr.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		std::string msg = errMsg ? errMsg : "unknown error";
		sqlite3_free(errMsg);
		throw SqliteException(rc, std::format("SQLite exec error [{}]: {}", rc, msg));
	}
}

SqliteStatement SqliteDatabase::Prepare(std::string_view sql) {
	sqlite3_stmt* stmt = nullptr;
	int rc = sqlite3_prepare_v2(_db, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		ThrowIfError(rc, "prepare");
	}
	return SqliteStatement(stmt, _db);
}

SqliteTransaction SqliteDatabase::BeginTransaction() {
	return SqliteTransaction(_db);
}

int SqliteDatabase::GetChanges() const {
	return sqlite3_changes(_db);
}

int64_t SqliteDatabase::GetLastInsertRowId() const {
	return sqlite3_last_insert_rowid(_db);
}

void SqliteDatabase::ThrowIfError(int rc, const char* context) const {
	if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE) {
		throw SqliteException(rc, FormatError(_db, rc, context));
	}
}

// ============================================================================
// SqliteStatement
// ============================================================================

SqliteStatement::SqliteStatement(sqlite3_stmt* stmt, sqlite3* db)
	: _stmt(stmt), _db(db) {}

SqliteStatement::~SqliteStatement() {
	if (_stmt) {
		sqlite3_finalize(_stmt);
		_stmt = nullptr;
	}
}

SqliteStatement::SqliteStatement(SqliteStatement&& other) noexcept
	: _stmt(other._stmt), _db(other._db) {
	other._stmt = nullptr;
	other._db = nullptr;
}

SqliteStatement& SqliteStatement::operator=(SqliteStatement&& other) noexcept {
	if (this != &other) {
		if (_stmt) sqlite3_finalize(_stmt);
		_stmt = other._stmt;
		_db = other._db;
		other._stmt = nullptr;
		other._db = nullptr;
	}
	return *this;
}

void SqliteStatement::Bind(int index, int value) {
	int rc = sqlite3_bind_int(_stmt, index, value);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind int"));
	}
}

void SqliteStatement::Bind(int index, int64_t value) {
	int rc = sqlite3_bind_int64(_stmt, index, value);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind int64"));
	}
}

void SqliteStatement::Bind(int index, double value) {
	int rc = sqlite3_bind_double(_stmt, index, value);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind double"));
	}
}

void SqliteStatement::Bind(int index, std::string_view value) {
	int rc = sqlite3_bind_text(_stmt, index, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind text"));
	}
}

void SqliteStatement::Bind(int index, std::span<const uint8_t> blob) {
	int rc = sqlite3_bind_blob(_stmt, index, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind blob"));
	}
}

void SqliteStatement::BindNull(int index) {
	int rc = sqlite3_bind_null(_stmt, index);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "bind null"));
	}
}

bool SqliteStatement::Step() {
	int rc = sqlite3_step(_stmt);
	if (rc == SQLITE_ROW) return true;
	if (rc == SQLITE_DONE) return false;
	throw SqliteException(rc, FormatError(_db, rc, "step"));
}

void SqliteStatement::Reset() {
	int rc = sqlite3_reset(_stmt);
	if (rc != SQLITE_OK) {
		throw SqliteException(rc, FormatError(_db, rc, "reset"));
	}
	sqlite3_clear_bindings(_stmt);
}

int SqliteStatement::GetInt(int column) const {
	return sqlite3_column_int(_stmt, column);
}

int64_t SqliteStatement::GetInt64(int column) const {
	return sqlite3_column_int64(_stmt, column);
}

double SqliteStatement::GetDouble(int column) const {
	return sqlite3_column_double(_stmt, column);
}

std::string_view SqliteStatement::GetText(int column) const {
	const char* text = reinterpret_cast<const char*>(sqlite3_column_text(_stmt, column));
	if (!text) return {};
	int len = sqlite3_column_bytes(_stmt, column);
	return {text, static_cast<size_t>(len)};
}

std::span<const uint8_t> SqliteStatement::GetBlob(int column) const {
	const uint8_t* data = static_cast<const uint8_t*>(sqlite3_column_blob(_stmt, column));
	if (!data) return {};
	int len = sqlite3_column_bytes(_stmt, column);
	return {data, static_cast<size_t>(len)};
}

bool SqliteStatement::IsNull(int column) const {
	return sqlite3_column_type(_stmt, column) == SQLITE_NULL;
}

int SqliteStatement::GetColumnCount() const {
	return sqlite3_column_count(_stmt);
}

const char* SqliteStatement::GetColumnName(int column) const {
	return sqlite3_column_name(_stmt, column);
}

// ============================================================================
// SqliteTransaction
// ============================================================================

SqliteTransaction::SqliteTransaction(sqlite3* db) : _db(db) {
	char* errMsg = nullptr;
	int rc = sqlite3_exec(_db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		std::string msg = errMsg ? errMsg : "unknown error";
		sqlite3_free(errMsg);
		throw SqliteException(rc, std::format("SQLite begin transaction error [{}]: {}", rc, msg));
	}
}

SqliteTransaction::~SqliteTransaction() {
	if (_db && !_committed) {
		sqlite3_exec(_db, "ROLLBACK", nullptr, nullptr, nullptr);
	}
}

SqliteTransaction::SqliteTransaction(SqliteTransaction&& other) noexcept
	: _db(other._db), _committed(other._committed) {
	other._db = nullptr;
}

SqliteTransaction& SqliteTransaction::operator=(SqliteTransaction&& other) noexcept {
	if (this != &other) {
		if (_db && !_committed) {
			sqlite3_exec(_db, "ROLLBACK", nullptr, nullptr, nullptr);
		}
		_db = other._db;
		_committed = other._committed;
		other._db = nullptr;
	}
	return *this;
}

void SqliteTransaction::Commit() {
	if (!_db || _committed) return;
	char* errMsg = nullptr;
	int rc = sqlite3_exec(_db, "COMMIT", nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		std::string msg = errMsg ? errMsg : "unknown error";
		sqlite3_free(errMsg);
		throw SqliteException(rc, std::format("SQLite commit error [{}]: {}", rc, msg));
	}
	_committed = true;
}

void SqliteTransaction::Rollback() {
	if (!_db || _committed) return;
	sqlite3_exec(_db, "ROLLBACK", nullptr, nullptr, nullptr);
	_committed = true; // предотвращаем повторный rollback в деструкторе
}
