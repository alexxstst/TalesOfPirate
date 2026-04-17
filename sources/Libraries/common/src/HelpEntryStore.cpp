#include <HelpEntryStore.h>

#include <random>
#include <string>
#include <string_view>

namespace {
	void InsertRow(SqliteDatabase& db, std::string_view category, int ord, std::string_view content) {
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO help_entries (category, ord, content) VALUES (?, ?, ?)");
		stmt.Bind(1, category);
		stmt.Bind(2, ord);
		stmt.Bind(3, content);
		stmt.Step();
	}
} // namespace

GameRecordset<HelpEntryRecord>::RecordEntry HelpEntryStore::ReadRecord(SqliteStatement& stmt) {
	HelpEntryRecord record{};
	int col = 0;

	record._category = std::string(stmt.GetText(col++));
	record._ord      = stmt.GetInt(col++);
	record._content  = std::string(stmt.GetText(col++));

	// В базовых индексах GameRecordset эту таблицу не адресуем: ключ — пара (category, ord),
	// а запросы идут по категории через ForEach. Поэтому отдаём id=0 и пустое name.
	return {0, std::string{}, std::move(record)};
}

bool HelpEntryStore::Load(SqliteDatabase& db) {
	EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
	return GameRecordset::Load(db, SELECT_ALL_SQL);
}

void HelpEntryStore::Insert(SqliteDatabase& db, const HelpEntryRecord& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		InsertRow(db, r._category, r._ord, r._content);
	}
	catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
					 "HelpEntryStore::Insert(category='{}', ord={}) failed: {}",
					 r._category, r._ord, e.what());
	}
}

std::string_view HelpEntryStore::GetPage(std::string_view category) const {
	std::string_view result;
	ForEach([&](const HelpEntryRecord& r) {
		if (result.empty() && r._category == category) {
			result = r._content;
		}
	});
	return result;
}

std::vector<const HelpEntryRecord*> HelpEntryStore::GetCategory(std::string_view category) const {
	std::vector<const HelpEntryRecord*> result;
	ForEach([&](const HelpEntryRecord& r) {
		if (r._category == category) {
			result.push_back(&r);
		}
	});
	return result;
}

std::string_view HelpEntryStore::GetRandomTip() const {
	auto tips = GetCategory(CATEGORY_TIP);
	if (tips.empty()) return {};

	static thread_local std::mt19937 rng{std::random_device{}()};
	std::uniform_int_distribution<size_t> dist(0, tips.size() - 1);
	return tips[dist(rng)]->_content;
}
