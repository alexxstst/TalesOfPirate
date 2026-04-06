#pragma once

// Database.h — современная ODBC-обёртка в стиле ADO.NET
//
// Классы:
//   OdbcDatabase    — соединение (RAII)
//   OdbcCommand     — подготовленный запрос с параметрами (позиционные ? и именованные @name)
//   OdbcReader      — forward-only итератор по результатам SELECT
//   OdbcTransaction — RAII транзакция (auto-rollback)
//   OdbcTable<T>    — типизированная таблица с лямбда-маппером
//   OdbcException   — исключение с SQL state и native error

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include <functional>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <cctype>

// ============================================================================
// OdbcException
// ============================================================================

class OdbcException : public std::runtime_error {
public:
	OdbcException(std::string sqlState, int nativeError, std::string message);

	[[nodiscard]] const std::string& GetSqlState() const { return _sqlState; }
	[[nodiscard]] int GetNativeError() const { return _nativeError; }

private:
	std::string _sqlState;
	int _nativeError;
};

// Forward declarations
class OdbcCommand;
class OdbcReader;
class OdbcTransaction;

// ============================================================================
// OdbcDatabase
// ============================================================================

class OdbcDatabase {
public:
	OdbcDatabase() = default;
	~OdbcDatabase();

	OdbcDatabase(const OdbcDatabase&) = delete;
	OdbcDatabase& operator=(const OdbcDatabase&) = delete;

	void Open(std::string_view connectionString);
	void Close();
	[[nodiscard]] bool IsOpen() const { return _hdbc != SQL_NULL_HDBC; }

	// Проверить соединение и переподключиться при необходимости
	void EnsureConnected();

	// Максимальное количество попыток реконнекта
	void SetMaxReconnectAttempts(int attempts) { _maxReconnectAttempts = attempts; }

	OdbcCommand CreateCommand(std::string_view sql = {});
	OdbcTransaction BeginTransaction();

	[[nodiscard]] SQLHDBC GetHandle() const { return _hdbc; }

	// Утилита для проверки SQLRETURN и выброса OdbcException с диагностикой
	static void ThrowIfError(SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret, std::string_view context = {});

private:
	SQLHENV _henv{SQL_NULL_HENV};
	SQLHDBC _hdbc{SQL_NULL_HDBC};
	std::string _connectionString;
	int _maxReconnectAttempts{3};

	friend class OdbcCommand;
	friend class OdbcTransaction;
};

// ============================================================================
// OdbcReader
// ============================================================================

class OdbcReader {
public:
	~OdbcReader();

	OdbcReader(OdbcReader&& other) noexcept;
	OdbcReader& operator=(OdbcReader&& other) noexcept;
	OdbcReader(const OdbcReader&) = delete;
	OdbcReader& operator=(const OdbcReader&) = delete;

	[[nodiscard]] bool Read();

	// Доступ по индексу (0-based)
	[[nodiscard]] std::string GetString(int col) const;
	[[nodiscard]] int GetInt(int col) const;
	[[nodiscard]] int64_t GetInt64(int col) const;
	[[nodiscard]] double GetDouble(int col) const;
	[[nodiscard]] std::vector<uint8_t> GetBinary(int col) const;
	[[nodiscard]] bool IsNull(int col) const;

	// Доступ по имени колонки (case-insensitive)
	[[nodiscard]] std::string GetString(std::string_view columnName) const;
	[[nodiscard]] int GetInt(std::string_view columnName) const;
	[[nodiscard]] int64_t GetInt64(std::string_view columnName) const;
	[[nodiscard]] double GetDouble(std::string_view columnName) const;
	[[nodiscard]] std::vector<uint8_t> GetBinary(std::string_view columnName) const;
	[[nodiscard]] bool IsNull(std::string_view columnName) const;

	// Метаданные колонок
	[[nodiscard]] int GetColumnCount() const { return static_cast<int>(_columnNames.size()); }
	[[nodiscard]] int GetColumnIndex(std::string_view name) const;
	[[nodiscard]] const std::string& GetColumnName(int col) const;
	[[nodiscard]] const std::unordered_map<std::string, int>& GetColumnMap() const { return _columnMap; }

	void Close();

private:
	explicit OdbcReader(SQLHSTMT hstmt, std::string sql = {});
	void BuildColumnMap();
	[[nodiscard]] int ResolveColumn(std::string_view name) const;

	SQLHSTMT _hstmt{SQL_NULL_HSTMT};
	bool _closed{false};
	std::string _sql;
	std::vector<std::string> _columnNames;
	std::unordered_map<std::string, int> _columnMap;

	friend class OdbcCommand;
};

// ============================================================================
// OdbcCommand
// ============================================================================

class OdbcCommand {
public:
	OdbcCommand(OdbcDatabase& db, std::string_view sql = {});
	~OdbcCommand();

	OdbcCommand(OdbcCommand&& other) noexcept;
	OdbcCommand& operator=(OdbcCommand&& other) noexcept;
	OdbcCommand(const OdbcCommand&) = delete;
	OdbcCommand& operator=(const OdbcCommand&) = delete;

	void SetCommandText(std::string_view sql);

	// Параметры по индексу (1-based)
	OdbcCommand& SetParam(int index, int value);
	OdbcCommand& SetParam(int index, long value) { return SetParam(index, static_cast<int>(value)); }
	OdbcCommand& SetParam(int index, unsigned int value) { return SetParam(index, static_cast<int>(value)); }
	OdbcCommand& SetParam(int index, unsigned long value) { return SetParam(index, static_cast<int>(value)); }
	OdbcCommand& SetParam(int index, int64_t value);
	OdbcCommand& SetParam(int index, std::string_view value);
	OdbcCommand& SetParam(int index, double value);
	OdbcCommand& SetParam(int index, std::nullptr_t);
	OdbcCommand& SetParam(int index, const void* data, size_t len);
	OdbcCommand& SetTimestampParam(int index, std::string_view value);

	// Параметры по имени (@name)
	OdbcCommand& SetParam(std::string_view name, int value);
	OdbcCommand& SetParam(std::string_view name, int64_t value);
	OdbcCommand& SetParam(std::string_view name, std::string_view value);
	OdbcCommand& SetParam(std::string_view name, double value);
	OdbcCommand& SetParam(std::string_view name, std::nullptr_t);

	void ClearParameters();

	// Выполнение
	int ExecuteNonQuery();
	OdbcReader ExecuteReader();
	std::string ExecuteScalar();

	void SetTimeout(unsigned short seconds) { _timeout = seconds; }

	// Строка-обёртка для datetime/timestamp параметров (биндится как SQL_TYPE_TIMESTAMP)
	struct TimestampString { std::string value; };

	// Хранение значения параметра (public для доступа из вспомогательных функций)
	struct ParamValue {
		std::variant<
			int,
			int64_t,
			std::string,
			double,
			std::vector<char>,   // binary
			std::monostate,      // NULL
			TimestampString      // datetime/timestamp
		> value;
	};

private:
	OdbcDatabase& _db;
	SQLHSTMT _hstmt{SQL_NULL_HSTMT};
	std::string _sql;
	std::string _processedSql;
	unsigned short _timeout{30};

	std::vector<ParamValue> _params;
	std::unordered_map<std::string, int> _namedParams;  // @name → index (1-based)

	// SQLLEN буферы для SQLBindParameter (должны жить до SQLExecute)
	std::vector<SQLLEN> _indicators;

	void ParseNamedParameters();
	void AllocateStatement();
	void BindAndExecute();

	int ResolveNamedParam(std::string_view name);
};

// ============================================================================
// OdbcTransaction
// ============================================================================

class OdbcTransaction {
public:
	~OdbcTransaction();

	OdbcTransaction(OdbcTransaction&& other) noexcept;
	OdbcTransaction& operator=(OdbcTransaction&&) = delete;
	OdbcTransaction(const OdbcTransaction&) = delete;
	OdbcTransaction& operator=(const OdbcTransaction&) = delete;

	void Commit();
	void Rollback();

private:
	explicit OdbcTransaction(SQLHDBC hdbc);

	SQLHDBC _hdbc;
	bool _finished{false};

	friend class OdbcDatabase;
};

// ============================================================================
// Column<T> — декларативное описание колонки таблицы
// ============================================================================

enum ColumnFlags {
	None = 0,
	PrimaryKey = 1,
	Nullable = 2,    // Пустая строка биндится как SQL NULL
	Timestamp = 4,   // Строка биндится как SQL_TYPE_TIMESTAMP (для datetime-колонок)
};

template <typename T>
struct Column {
	std::string name;
	int flags = None;
	std::function<void(OdbcCommand&, int, const T&)> bind;
	std::function<void(OdbcReader&, T&)> read;
};

template <typename T, typename V>
Column<T> MakeColumn(std::string name, V T::* member, int flags = None) {
	return {
		.name = name,
		.flags = flags,
		.bind = [member, flags](OdbcCommand& cmd, int idx, const T& row) {
			if constexpr (std::is_same_v<V, std::vector<uint8_t>>) {
				const auto& v = row.*member;
				cmd.SetParam(idx, v.data(), v.size());
			} else if constexpr (std::is_same_v<V, std::string>) {
				const auto& s = row.*member;
				if ((flags & Nullable) && s.empty()) {
					cmd.SetParam(idx, nullptr);
				} else if (flags & Timestamp) {
					cmd.SetTimestampParam(idx, s);
				} else {
					cmd.SetParam(idx, s);
				}
			} else {
				cmd.SetParam(idx, row.*member);
			}
		},
		.read = [name, member](OdbcReader& r, T& row) {
			if constexpr (std::is_same_v<V, int>) {
				row.*member = r.GetInt(name);
			} else if constexpr (std::is_same_v<V, int64_t>) {
				row.*member = r.GetInt64(name);
			} else if constexpr (std::is_same_v<V, std::string>) {
				row.*member = r.GetString(name);
			} else if constexpr (std::is_same_v<V, double>) {
				row.*member = r.GetDouble(name);
			} else if constexpr (std::is_same_v<V, std::vector<uint8_t>>) {
				row.*member = r.GetBinary(name);
			}
		},
	};
}

// ============================================================================
// OdbcTable<T> — типизированная таблица с Column DSL
// ============================================================================

template <typename T>
class OdbcTable {
public:
	OdbcTable(OdbcDatabase& db, std::string_view tableName,
			  std::initializer_list<Column<T>> columns)
		: _db(db), _tableName(tableName),
		  _columns(columns.begin(), columns.end())
	{
		BuildSql();
	}

	// Найти одну запись по условию с параметрами
	template <typename... Args>
	std::optional<T> FindOne(std::string_view where, Args&&... args) {
		auto sql = "SELECT * FROM " + _tableName + " WHERE " + std::string(where);
		auto cmd = _db.CreateCommand(sql);
		BindParams(cmd, 1, std::forward<Args>(args)...);
		auto reader = cmd.ExecuteReader();
		if (reader.Read()) {
			return MapRow(reader);
		}
		return std::nullopt;
	}

	// Найти все записи по условию с параметрами
	template <typename... Args>
	std::vector<T> FindAll(std::string_view where, Args&&... args) {
		auto sql = "SELECT * FROM " + _tableName + " WHERE " + std::string(where);
		auto cmd = _db.CreateCommand(sql);
		BindParams(cmd, 1, std::forward<Args>(args)...);
		auto reader = cmd.ExecuteReader();
		std::vector<T> result;
		while (reader.Read()) {
			result.push_back(MapRow(reader));
		}
		return result;
	}

	// Все записи
	std::vector<T> FindAll() {
		auto sql = "SELECT * FROM " + _tableName;
		auto cmd = _db.CreateCommand(sql);
		auto reader = cmd.ExecuteReader();
		std::vector<T> result;
		while (reader.Read()) {
			result.push_back(MapRow(reader));
		}
		return result;
	}

	// Обновить запись по PK
	int Update(const T& row) {
		if (_updateSql.empty()) {
			throw OdbcException("", 0, "Update not configured for table " + _tableName);
		}
		auto cmd = _db.CreateCommand(_updateSql);
		int idx = 1;
		for (const auto& col : _columns) {
			if (!(col.flags & PrimaryKey)) {
				col.bind(cmd, idx++, row);
			}
		}
		for (const auto& col : _columns) {
			if (col.flags & PrimaryKey) {
				col.bind(cmd, idx++, row);
			}
		}
		return cmd.ExecuteNonQuery();
	}

	// Выполнить произвольный SQL с параметрами
	template <typename... Args>
	int Execute(std::string_view sql, Args&&... args) {
		auto cmd = _db.CreateCommand(sql);
		BindParams(cmd, 1, std::forward<Args>(args)...);
		return cmd.ExecuteNonQuery();
	}

	[[nodiscard]] const std::string& GetTableName() const { return _tableName; }

private:
	OdbcDatabase& _db;
	std::string _tableName;
	std::vector<Column<T>> _columns;
	std::string _updateSql;

	T MapRow(OdbcReader& reader) {
		T row{};
		for (const auto& col : _columns) {
			col.read(reader, row);
		}
		return row;
	}

	void BuildSql() {
		std::string setCols, whereCols;
		for (const auto& col : _columns) {
			if (col.flags & PrimaryKey) {
				if (!whereCols.empty()) {
					whereCols += " AND ";
				}
				whereCols += col.name + " = ?";
			} else {
				if (!setCols.empty()) {
					setCols += ", ";
				}
				setCols += col.name + " = ?";
			}
		}
		if (!setCols.empty() && !whereCols.empty()) {
			_updateSql = "UPDATE " + _tableName + " SET " + setCols + " WHERE " + whereCols;
		}
	}

	static void BindParams(OdbcCommand&, int) {}

	template <typename First, typename... Rest>
	static void BindParams(OdbcCommand& cmd, int index, First&& first, Rest&&... rest) {
		cmd.SetParam(index, std::forward<First>(first));
		BindParams(cmd, index + 1, std::forward<Rest>(rest)...);
	}
};
