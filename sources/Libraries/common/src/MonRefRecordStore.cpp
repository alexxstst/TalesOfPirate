#include <MonRefRecordStore.h>

#include <algorithm>
#include <charconv>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <logutil.h>
#include <util2.h>

namespace {
	constexpr int MAX_LINE_SIZE = 2048;
	constexpr int MAX_FIELDS    = 64;

	std::pair<int, int> ParseXY(std::string_view text) {
		if (text.empty()) {
			return {0, 0};
		}
		size_t comma = text.find(',');
		if (comma == std::string_view::npos) {
			return {0, 0};
		}
		int x = 0, y = 0;
		std::from_chars(text.data(), text.data() + comma, x);
		std::from_chars(text.data() + comma + 1, text.data() + text.size(), y);
		return {x, y};
	}

	// Разбор CSV-списка int-значений. Пустые токены пропускаются.
	// Результат обрезается до limit значений, остаток игнорируется.
	std::vector<int> ParseIntCsv(std::string_view text, size_t limit) {
		std::vector<int> out;
		out.reserve(limit);
		size_t start = 0;
		while (start <= text.size() && out.size() < limit) {
			size_t end = text.find(',', start);
			if (end == std::string_view::npos) {
				end = text.size();
			}
			auto token = text.substr(start, end - start);
			if (!token.empty()) {
				int value = 0;
				std::from_chars(token.data(), token.data() + token.size(), value);
				out.push_back(value);
			}
			if (end == text.size()) {
				break;
			}
			start = end + 1;
		}
		return out;
	}
} // namespace

bool MonRefRecordStore::Load(const char* txtPath) {
	_records.clear();
	_idIndex.clear();

	if (txtPath == nullptr || txtPath[0] == '\0') {
		ToLogService("errors", LogLevel::Error, "MonRefRecordStore::Load: пустой путь");
		return false;
	}

	std::ifstream in(txtPath);
	if (!in.is_open()) {
		ToLogService("errors", LogLevel::Error,
					 "MonRefRecordStore::Load: не удалось открыть '{}'", txtPath);
		return false;
	}

	char szLine[MAX_LINE_SIZE];
	auto fields = std::make_unique<std::string[]>(MAX_FIELDS + 1);

	while (!in.eof()) {
		in.getline(szLine, MAX_LINE_SIZE);
		std::string strLine = szLine;

		size_t pos = strLine.find("//");
		if (pos != std::string::npos) {
			strLine.resize(pos);
		}

		int n = Util_ResolveTextLine(strLine.c_str(), fields.get(), MAX_FIELDS + 1, '\t');
		if (n < 2) {
			continue;
		}

		// Колонки legacy <map>ChaSpn.txt:
		// [0] id
		// [1] "x,y"  startpos
		// [2] "x,y"  endpos
		// [3] direction (sAngle; -1 = случайный)
		// [4] monsterID CSV
		// [5] count CSV
		// [6] probability CSV
		// [7] refreshTime CSV
		// [8,9] commotion/note — игнорируются
		CMonRefRecord r{};
		r.Id = Str2Int(fields[0]);
		r.lID = r.Id;

		auto [x0, y0] = ParseXY(fields[1]);
		auto [x1, y1] = n > 2 ? ParseXY(fields[2]) : std::pair<int, int>{0, 0};
		r.SRegion[0].x = x0;
		r.SRegion[0].y = y0;
		r.SRegion[1].x = x1;
		r.SRegion[1].y = y1;

		r.sAngle = n > 3 ? static_cast<short>(Str2Int(fields[3])) : -1;

		auto ids   = n > 4 ? ParseIntCsv(fields[4], defMAX_REGION_MONSTER_TYPE) : std::vector<int>{};
		auto cnts  = n > 5 ? ParseIntCsv(fields[5], defMAX_REGION_MONSTER_TYPE) : std::vector<int>{};
		auto probs = n > 6 ? ParseIntCsv(fields[6], defMAX_REGION_MONSTER_TYPE) : std::vector<int>{};
		auto tms   = n > 7 ? ParseIntCsv(fields[7], defMAX_REGION_MONSTER_TYPE) : std::vector<int>{};

		// Неиспользуемые слоты — нули (не 0xFFFFFFFF как в legacy-парсере).
		// CChaSpawn::Load итерирует по всем 15 слотам и останавливается на count==0.
		std::memset(r.lMonster, 0, sizeof(r.lMonster));
		for (size_t i = 0; i < ids.size();   i++) r.lMonster[i][0] = ids[i];
		for (size_t i = 0; i < cnts.size();  i++) r.lMonster[i][1] = cnts[i];
		for (size_t i = 0; i < probs.size(); i++) r.lMonster[i][2] = probs[i];
		for (size_t i = 0; i < tms.size();   i++) r.lMonster[i][3] = tms[i];

		_records.push_back(std::move(r));
	}

	_idIndex.reserve(_records.size());
	for (auto& r : _records) {
		_idIndex[static_cast<int>(r.Id)] = &r;
	}

	ToLogService("common", "MonRefRecordStore: загружено {} записей из '{}'",
				 _records.size(), txtPath);
	return true;
}

CMonRefRecord* MonRefRecordStore::Get(int id) {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}

const CMonRefRecord* MonRefRecordStore::Get(int id) const {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}
