#pragma once

// RenderAssetDatabase — клиентская SQLite БД для 3D-ассетов (render.sqlite).
//
// Содержит:
//   - таблицу sqlar (SQLAR-совместимый blob-контейнер: модели, текстуры, шейдеры)
//   - таблицы метаданных конкретных доменов (добавляются по мере миграции)
//
// Сервер эту БД не открывает. Клиент инициализирует один раз в Main.cpp
// по пути из system.ini (секция [Assets], ключ RenderAssetPack).
//
// Использование:
//   RenderAssetDatabase::Instance()->Open("./render.sqlite");
//   auto& db  = RenderAssetDatabase::Instance()->GetDb();
//   auto data = RenderAssetDatabase::Instance()->ReadBlob("models/pirate.lgo");

#include <SqliteDatabase.h>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class RenderAssetDatabase {
public:
	static RenderAssetDatabase* Instance() {
		static RenderAssetDatabase instance{};
		return &instance;
	}

	RenderAssetDatabase(const RenderAssetDatabase&) = delete;
	RenderAssetDatabase& operator=(const RenderAssetDatabase&) = delete;

	// Открыть/создать БД. При первом открытии пустого файла ставит PRAGMA
	// (page_size=8192, auto_vacuum=INCREMENTAL, journal_mode=WAL) и создаёт sqlar.
	// Можно вызывать повторно (reload).
	void Open(std::string_view path);

	// Получить ссылку на открытую БД.
	SqliteDatabase& GetDb();

	// Проверить, открыта ли база.
	bool IsOpen() const { return _db != nullptr; }

	// Прочитать raw-блоб по имени. Пустой vector — если name не найден.
	//
	// Стандарт SQLAR: sz==data.size() -> raw; sz>data.size() -> zlib-deflate;
	// sz==0 -> directory; sz==-1 -> symlink. Сейчас поддерживаем только raw.
	// Встретив иное — бросаем SqliteException.
	std::vector<uint8_t> ReadBlob(std::string_view name);

	// Записать raw-блоб (INSERT OR REPLACE по name). mode=0, mtime=0, sz=data.size().
	void WriteBlob(std::string_view name, std::span<const uint8_t> data);

	// Есть ли блоб с таким именем.
	bool HasBlob(std::string_view name);

private:
	RenderAssetDatabase() = default;

	void EnsureSchema();

	std::unique_ptr<SqliteDatabase> _db;
};
