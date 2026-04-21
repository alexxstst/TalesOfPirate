#pragma once

// Хранилище регионов спауна монстров одной карты. Источник — legacy-файл <map>ChaSpn.txt.
// Один экземпляр на карту, владельцем выступает CChaSpawn.
//
//   MonRefRecordStore store;
//   store.Load("./resource/garner/garnerChaSpn.txt");
//   CMonRefRecord* r = store.Get(1);

#include "MonRefRecord.h"
#include <unordered_map>
#include <vector>

class MonRefRecordStore {
public:
	MonRefRecordStore() = default;
	MonRefRecordStore(const MonRefRecordStore&) = delete;
	MonRefRecordStore& operator=(const MonRefRecordStore&) = delete;

	bool Load(const char* txtPath);

	CMonRefRecord* Get(int id);
	const CMonRefRecord* Get(int id) const;

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
	std::vector<CMonRefRecord> _records;
	std::unordered_map<int, CMonRefRecord*> _idIndex;
};
