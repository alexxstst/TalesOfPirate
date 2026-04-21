#include <SwitchMapRecordStore.h>

#include <algorithm>
#include <charconv>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

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

	template <size_t N>
	void CopyFixed(char (&dst)[N], std::string_view src) {
		size_t n = (std::min)(src.size(), N - 1);
		if (n > 0) {
			std::memcpy(dst, src.data(), n);
		}
		dst[n] = '\0';
	}
} // namespace

bool SwitchMapRecordStore::Load(const char* txtPath) {
	_records.clear();
	_idIndex.clear();

	if (txtPath == nullptr || txtPath[0] == '\0') {
		ToLogService("errors", LogLevel::Error, "SwitchMapRecordStore::Load: пустой путь");
		return false;
	}

	std::ifstream in(txtPath);
	if (!in.is_open()) {
		ToLogService("errors", LogLevel::Error,
					 "SwitchMapRecordStore::Load: не удалось открыть '{}'", txtPath);
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

		// Колонки legacy <map>SwhMap.txt:
		// [0] id
		// [1] EntityID (в старом парсере идёт через DataName)
		// [2] EventType → lEventID
		// [3] "x,y"  EntityPos
		// [4] Angle
		// [5] TargetMap name
		// [6] "x,y"  TargetPos
		// [7,8] ActivateCondition, Name — игнорируются
		CSwitchMapRecord r{};
		r.Id = Str2Int(fields[0]);
		r.lID = r.Id;
		r.lEntityID = n > 1 ? Str2Int(fields[1]) : 0;
		r.lEventID  = n > 2 ? Str2Int(fields[2]) : 0;

		auto [x, y] = n > 3 ? ParseXY(fields[3]) : std::pair<int, int>{0, 0};
		r.SEntityPos.x = x;
		r.SEntityPos.y = y;

		r.sAngle = n > 4 ? static_cast<short>(Str2Int(fields[4])) : -1;

		if (n > 5) {
			Util_TrimTabString(fields[5]);
			CopyFixed(r.szTarMapName, std::string_view{fields[5]});
		}

		auto [tx2, ty2] = n > 6 ? ParseXY(fields[6]) : std::pair<int, int>{0, 0};
		r.STarPos.x = tx2;
		r.STarPos.y = ty2;

		_records.push_back(std::move(r));
	}

	_idIndex.reserve(_records.size());
	for (auto& r : _records) {
		_idIndex[static_cast<int>(r.Id)] = &r;
	}

	ToLogService("common", "SwitchMapRecordStore: загружено {} записей из '{}'",
				 _records.size(), txtPath);
	return true;
}

CSwitchMapRecord* SwitchMapRecordStore::Get(int id) {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}

const CSwitchMapRecord* SwitchMapRecordStore::Get(int id) const {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}
