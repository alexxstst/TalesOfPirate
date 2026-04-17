#pragma once

// Хранилище клиентских статичных подсказок (help_entries).
//
// Категории в таблице:
//   - 'intro' (intro-окно, показывается при первом входе в мир), ord=0
//   - 'store' (FAQ в окне магазина), ord=0
//   - 'tip'   (случайный совет на экране загрузки), ord=0..N
//
// Использование:
//   HelpEntryStore::Instance()->Load(db);
//   auto intro = HelpEntryStore::Instance()->GetPage(HelpEntryStore::CATEGORY_INTRO);
//   auto tip   = HelpEntryStore::Instance()->GetRandomTip();

#include "GameRecordset.h"
#include "HelpEntryRecord.h"
#include <string_view>
#include <vector>

class HelpEntryStore : public GameRecordset<HelpEntryRecord> {
public:
	static HelpEntryStore* Instance() {
		static HelpEntryStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "help_entries";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS help_entries (
			category TEXT    NOT NULL,
			ord      INTEGER NOT NULL,
			content  TEXT    NOT NULL,
			PRIMARY KEY (category, ord)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT category, ord, content FROM help_entries ORDER BY category, ord";

	// Константы категорий. В одном месте, чтобы не сеять строки по коду.
	static constexpr const char* CATEGORY_INTRO = "intro";
	static constexpr const char* CATEGORY_STORE = "store";
	static constexpr const char* CATEGORY_TIP   = "tip";

	// Загрузить записи из таблицы help_entries.
	bool Load(SqliteDatabase& db);

	// Вставить/обновить запись.
	static void Insert(SqliteDatabase& db, const HelpEntryRecord& record);

	// Получить содержимое единственной записи в категории (intro/store). Пустое — если нет.
	std::string_view GetPage(std::string_view category) const;

	// Получить все записи заданной категории в порядке ord.
	std::vector<const HelpEntryRecord*> GetCategory(std::string_view category) const;

	// Случайная подсказка из категории 'tip'. Пустая если нет.
	std::string_view GetRandomTip() const;

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
