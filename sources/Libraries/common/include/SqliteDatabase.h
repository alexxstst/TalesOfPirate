#pragma once

// SqliteDatabase — RAII-обёртка над SQLite3 для загрузки игровых таблиц.
// Использование:
//   SqliteDatabase db("tables.db");
//   db.Execute("CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY, name TEXT)");
//
//   // Запись
//   auto stmt = db.Prepare("INSERT INTO items VALUES (?, ?)");
//   stmt.Bind(1, 42);
//   stmt.Bind(2, "Sword");
//   stmt.Step();
//
//   // Чтение
//   auto query = db.Prepare("SELECT id, name FROM items WHERE id > ?");
//   query.Bind(1, 10);
//   while (query.Step()) {
//       int id = query.GetInt(0);
//       std::string_view name = query.GetText(1);
//   }
//
//   // Транзакции
//   {
//       auto tx = db.BeginTransaction();
//       db.Execute("INSERT INTO ...");
//       tx.Commit();
//   } // автоматический ROLLBACK при исключении

#include <string>
#include <string_view>
#include <stdexcept>
#include <memory>
#include <functional>
#include <span>

struct sqlite3;
struct sqlite3_stmt;

// Исключение при ошибках SQLite
class SqliteException : public std::runtime_error {
public:
	SqliteException(int errorCode, const std::string& message)
		: std::runtime_error(message), _errorCode(errorCode) {}

	int GetErrorCode() const { return _errorCode; }

private:
	int _errorCode;
};

// RAII-обёртка над sqlite3_stmt
class SqliteStatement {
public:
	SqliteStatement() = default;
	~SqliteStatement();

	SqliteStatement(SqliteStatement&& other) noexcept;
	SqliteStatement& operator=(SqliteStatement&& other) noexcept;

	SqliteStatement(const SqliteStatement&) = delete;
	SqliteStatement& operator=(const SqliteStatement&) = delete;

	// Привязка параметров по индексу (1-based)
	void Bind(int index, int value);
	void Bind(int index, int64_t value);
	void Bind(int index, double value);
	void Bind(int index, std::string_view value);
	void Bind(int index, std::span<const uint8_t> blob);
	void BindNull(int index);

	// Выполнить шаг. Возвращает true если есть строка данных (SQLITE_ROW)
	bool Step();

	// Сброс для повторного выполнения
	void Reset();

	// Получение значений столбцов (0-based)
	int GetInt(int column) const;
	int64_t GetInt64(int column) const;
	double GetDouble(int column) const;
	std::string_view GetText(int column) const;
	std::span<const uint8_t> GetBlob(int column) const;
	bool IsNull(int column) const;

	// Количество столбцов в результате
	int GetColumnCount() const;

	// Имя столбца
	const char* GetColumnName(int column) const;

	bool IsValid() const { return _stmt != nullptr; }

private:
	friend class SqliteDatabase;
	SqliteStatement(sqlite3_stmt* stmt, sqlite3* db);

	sqlite3_stmt* _stmt = nullptr;
	sqlite3* _db = nullptr;
};

// RAII-обёртка над транзакцией
class SqliteTransaction {
public:
	~SqliteTransaction();

	SqliteTransaction(SqliteTransaction&& other) noexcept;
	SqliteTransaction& operator=(SqliteTransaction&& other) noexcept;

	SqliteTransaction(const SqliteTransaction&) = delete;
	SqliteTransaction& operator=(const SqliteTransaction&) = delete;

	void Commit();
	void Rollback();

private:
	friend class SqliteDatabase;
	explicit SqliteTransaction(sqlite3* db);

	sqlite3* _db = nullptr;
	bool _committed = false;
};

// Основной класс — RAII-обёртка над sqlite3
class SqliteDatabase {
public:
	// Открыть/создать базу. Пустая строка или ":memory:" — в памяти
	explicit SqliteDatabase(std::string_view path = ":memory:");
	~SqliteDatabase();

	SqliteDatabase(SqliteDatabase&& other) noexcept;
	SqliteDatabase& operator=(SqliteDatabase&& other) noexcept;

	SqliteDatabase(const SqliteDatabase&) = delete;
	SqliteDatabase& operator=(const SqliteDatabase&) = delete;

	// Выполнить SQL без результата
	void Execute(std::string_view sql);

	// Подготовить запрос
	SqliteStatement Prepare(std::string_view sql);

	// Транзакция (RAII — ROLLBACK в деструкторе если не Commit)
	SqliteTransaction BeginTransaction();

	// Количество изменённых строк последним INSERT/UPDATE/DELETE
	int GetChanges() const;

	// ID последней вставленной строки
	int64_t GetLastInsertRowId() const;

	// Прямой доступ к sqlite3* (для расширенного использования)
	sqlite3* GetHandle() const { return _db; }

	bool IsOpen() const { return _db != nullptr; }

private:
	void ThrowIfError(int rc, const char* context = nullptr) const;

	sqlite3* _db = nullptr;
};
