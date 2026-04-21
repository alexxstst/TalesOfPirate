#pragma once

// Хранилище NPC-записей одной карты. Источник — legacy-файл <map>NPC.txt.
// Один экземпляр на карту, владельцем выступает CNpcSpawn.
//
//   NpcRecordStore store;
//   store.Load("./resource/garner/garnerNPC.txt");
//   CNpcRecord* r = store.Get(1001);

#include "NpcRecord.h"
#include <unordered_map>
#include <vector>

class NpcRecordStore {
public:
	NpcRecordStore() = default;
	NpcRecordStore(const NpcRecordStore&) = delete;
	NpcRecordStore& operator=(const NpcRecordStore&) = delete;

	// Загрузить записи из <map>NPC.txt. Любые ранее загруженные данные очищаются.
	// Возвращает true, если файл удалось прочитать (пустой файл — тоже true).
	bool Load(const char* txtPath);

	// Получить запись по ID; nullptr если не найдена.
	CNpcRecord* Get(int id);
	const CNpcRecord* Get(int id) const;

	int GetCount() const {
		return static_cast<int>(_records.size());
	}

	template <typename Fn>
	void ForEach(Fn&& fn) {
		for (auto& r : _records) {
			fn(r);
		}
	}

	template <typename Fn>
	void ForEach(Fn&& fn) const {
		for (const auto& r : _records) {
			fn(r);
		}
	}

private:
	std::vector<CNpcRecord> _records;
	std::unordered_map<int, CNpcRecord*> _idIndex;
};
