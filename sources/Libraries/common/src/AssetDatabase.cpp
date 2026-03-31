#include "AssetDatabase.h"
#include <logutil.h>
#include <stdexcept>

void AssetDatabase::Open(std::string_view path) {
	_db.reset();
	_db = std::make_unique<SqliteDatabase>(path);
	ToLogService("common", "AssetDatabase: {}", std::string(path));
}

SqliteDatabase& AssetDatabase::GetDb() {
	if (!_db) {
		throw std::runtime_error("AssetDatabase не инициализирован. Вызовите Open() при старте.");
	}
	return *_db;
}
