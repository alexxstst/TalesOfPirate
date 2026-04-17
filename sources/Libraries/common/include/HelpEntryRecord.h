#pragma once

#include <string>

// Запись таблицы help_entries (клиентские статичные подсказки).
// Категория разделяет домены: intro (HelpInfo.tx), store (StoreHelp.tx), tip (tips.tx).
struct HelpEntryRecord {
	std::string _category{};
	int         _ord{0};
	std::string _content{};
};
