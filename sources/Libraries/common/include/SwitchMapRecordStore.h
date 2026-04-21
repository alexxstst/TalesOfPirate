#pragma once

// Хранилище точек телепорта между картами для одной карты. Источник — legacy-файл <map>SwhMap.txt.
// Один экземпляр на карту, владельцем выступает CMapSwitchEntitySpawn.
//
// ВАЖНО: адреса записей стабильны после Load (std::vector не перевыделяется) —
// CEvent::pTableRec хранит сырой указатель на запись и переживает весь lifecycle карты.

#include "SwitchMapRecord.h"
#include <unordered_map>
#include <vector>

class SwitchMapRecordStore {
public:
	SwitchMapRecordStore() = default;
	SwitchMapRecordStore(const SwitchMapRecordStore&) = delete;
	SwitchMapRecordStore& operator=(const SwitchMapRecordStore&) = delete;

	bool Load(const char* txtPath);

	CSwitchMapRecord* Get(int id);
	const CSwitchMapRecord* Get(int id) const;

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
	std::vector<CSwitchMapRecord> _records;
	std::unordered_map<int, CSwitchMapRecord*> _idIndex;
};
