#include "TestFramework.h"
#include "Database.h"

// ============================================================================
// Тестовая row-структура
// ============================================================================

struct TestRow {
	int id = 0;
	std::string name;
	int value = 0;
	int64_t bigValue = 0;
	double score = 0.0;
};

// Тестовая таблица с Column DSL
class TestTable : public OdbcTable<TestRow> {
public:
	explicit TestTable(OdbcDatabase& db)
		: OdbcTable(db, "test_odbc_table", {
			MakeColumn("id",        &TestRow::id, PrimaryKey),
			MakeColumn("name",      &TestRow::name),
			MakeColumn("value",     &TestRow::value),
			MakeColumn("big_value", &TestRow::bigValue),
			MakeColumn("score",     &TestRow::score),
		}) {}
};

// ============================================================================
// Хелпер: подготовка тестовой таблицы
// ============================================================================

static OdbcDatabase& GetTestDb() {
	static OdbcDatabase db;
	static bool initialized = false;
	if (!initialized) {
		// Используем переменную окружения или дефолтный connection string
		const char* connStr = std::getenv("TEST_DB_CONNSTR");
		if (!connStr) {
			connStr = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;DATABASE=gamedb;Trusted_Connection=Yes;";
		}
		db.Open(connStr);
		initialized = true;
	}
	return db;
}

static void EnsureTestTable(OdbcDatabase& db) {
	try {
		db.CreateCommand(
			"IF OBJECT_ID('test_odbc_table', 'U') IS NOT NULL DROP TABLE test_odbc_table"
		).ExecuteNonQuery();
	}
	catch (...) {}

	db.CreateCommand(
		"CREATE TABLE test_odbc_table ("
		"  id INT NOT NULL PRIMARY KEY,"
		"  name VARCHAR(100) NOT NULL,"
		"  value INT NOT NULL,"
		"  big_value BIGINT NOT NULL,"
		"  score FLOAT NOT NULL"
		")"
	).ExecuteNonQuery();
}

static void InsertTestRow(OdbcDatabase& db, int id, const char* name, int value, int64_t bigValue, double score) {
	db.CreateCommand("INSERT INTO test_odbc_table (id, name, value, big_value, score) VALUES (?, ?, ?, ?, ?)")
		.SetParam(1, id)
		.SetParam(2, std::string_view(name))
		.SetParam(3, value)
		.SetParam(4, bigValue)
		.SetParam(5, score)
		.ExecuteNonQuery();
}

// ============================================================================
// Тесты
// ============================================================================

TEST(OdbcTable, FindOne_ReturnsRow) {
	auto& db = GetTestDb();
	EnsureTestTable(db);
	InsertTestRow(db, 1, "Alice", 42, 9999999999LL, 3.14);

	TestTable table(db);
	auto row = table.FindOne("id = ?", 1);

	ASSERT_TRUE(row.has_value());
	ASSERT_EQ(1, row->id);
	ASSERT_EQ(std::string("Alice"), row->name);
	ASSERT_EQ(42, row->value);
	ASSERT_EQ(9999999999LL, row->bigValue);
	ASSERT_FLOAT_EQ(3.14, row->score);
}

TEST(OdbcTable, FindOne_ReturnsNullopt_WhenNotFound) {
	auto& db = GetTestDb();
	EnsureTestTable(db);

	TestTable table(db);
	auto row = table.FindOne("id = ?", 999);

	ASSERT_FALSE(row.has_value());
}

TEST(OdbcTable, FindAll_ReturnsMultipleRows) {
	auto& db = GetTestDb();
	EnsureTestTable(db);
	InsertTestRow(db, 1, "Alice", 10, 100, 1.0);
	InsertTestRow(db, 2, "Bob", 20, 200, 2.0);
	InsertTestRow(db, 3, "Charlie", 30, 300, 3.0);

	TestTable table(db);
	auto rows = table.FindAll("value >= ?", 20);

	ASSERT_EQ(2, static_cast<int>(rows.size()));
}

TEST(OdbcTable, FindAll_NoWhere_ReturnsAll) {
	auto& db = GetTestDb();
	EnsureTestTable(db);
	InsertTestRow(db, 1, "Alice", 10, 100, 1.0);
	InsertTestRow(db, 2, "Bob", 20, 200, 2.0);

	TestTable table(db);
	auto rows = table.FindAll();

	ASSERT_EQ(2, static_cast<int>(rows.size()));
}

TEST(OdbcTable, Update_ModifiesRow) {
	auto& db = GetTestDb();
	EnsureTestTable(db);
	InsertTestRow(db, 1, "Alice", 42, 100, 1.0);

	TestTable table(db);

	// Читаем
	auto row = table.FindOne("id = ?", 1);
	ASSERT_TRUE(row.has_value());
	ASSERT_EQ(42, row->value);

	// Обновляем
	row->name = "Alice Updated";
	row->value = 99;
	row->bigValue = 5555555555LL;
	row->score = 9.99;
	int affected = table.Update(*row);
	ASSERT_EQ(1, affected);

	// Перечитываем
	auto updated = table.FindOne("id = ?", 1);
	ASSERT_TRUE(updated.has_value());
	ASSERT_EQ(std::string("Alice Updated"), updated->name);
	ASSERT_EQ(99, updated->value);
	ASSERT_EQ(5555555555LL, updated->bigValue);
	ASSERT_FLOAT_EQ(9.99, updated->score);
}

TEST(OdbcTable, Update_ReturnsZero_WhenRowNotExists) {
	auto& db = GetTestDb();
	EnsureTestTable(db);

	TestTable table(db);
	TestRow row{.id = 999, .name = "Ghost", .value = 0, .bigValue = 0, .score = 0.0};
	int affected = table.Update(row);
	ASSERT_EQ(0, affected);
}

TEST(OdbcTable, Execute_Arbitrary_SQL) {
	auto& db = GetTestDb();
	EnsureTestTable(db);
	InsertTestRow(db, 1, "Alice", 42, 100, 1.0);

	TestTable table(db);
	int affected = table.Execute(
		"UPDATE test_odbc_table SET value = ? WHERE id = ?", 77, 1);
	ASSERT_EQ(1, affected);

	auto row = table.FindOne("id = ?", 1);
	ASSERT_EQ(77, row->value);
}
