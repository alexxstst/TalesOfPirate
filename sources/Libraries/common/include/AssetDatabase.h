#pragma once

#include <SqliteDatabase.h>
#include <memory>
#include <string_view>

// AssetDatabase — глобальный синглтон для доступа к SQLite базе игровых данных.
// Инициализируется один раз при старте приложения с указанием пути к файлу БД.
//
//   AssetDatabase::Instance()->Open("path/to/gamedata.sqlite");
//   auto& db = AssetDatabase::Instance()->GetDb();

class AssetDatabase {
public:
	static AssetDatabase* Instance() {
		static AssetDatabase instance{};
		return &instance;
	}

	AssetDatabase(const AssetDatabase&) = delete;
	AssetDatabase& operator=(const AssetDatabase&) = delete;

	// Открыть базу данных по указанному пути. Можно вызывать повторно (для reload).
	void Open(std::string_view path);

	// Получить ссылку на открытую базу данных.
	SqliteDatabase& GetDb();

	// Проверить, открыта ли база.
	bool IsOpen() const { return _db != nullptr; }

private:
	AssetDatabase() = default;
	std::unique_ptr<SqliteDatabase> _db;
};
