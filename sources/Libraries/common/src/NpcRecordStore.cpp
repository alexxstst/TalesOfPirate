#include <NpcRecordStore.h>

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

	// Разбор строки "x,y" в пару int. Пустая или некорректная — {0,0}.
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

	// Копирует строку в фиксированный char-буфер с принудительным '\0'.
	template <size_t N>
	void CopyFixed(char (&dst)[N], std::string_view src) {
		size_t n = (std::min)(src.size(), N - 1);
		if (n > 0) {
			std::memcpy(dst, src.data(), n);
		}
		dst[n] = '\0';
	}
} // namespace

bool NpcRecordStore::Load(const char* txtPath) {
	_records.clear();
	_idIndex.clear();

	if (txtPath == nullptr || txtPath[0] == '\0') {
		ToLogService("errors", LogLevel::Error, "NpcRecordStore::Load: пустой путь");
		return false;
	}

	std::ifstream in(txtPath);
	if (!in.is_open()) {
		ToLogService("errors", LogLevel::Error,
					 "NpcRecordStore::Load: не удалось открыть '{}'", txtPath);
		return false;
	}

	char szLine[MAX_LINE_SIZE];
	auto fields = std::make_unique<std::string[]>(MAX_FIELDS + 1);

	while (!in.eof()) {
		in.getline(szLine, MAX_LINE_SIZE);
		std::string strLine = szLine;

		// Срезать комментарий '//'
		size_t pos = strLine.find("//");
		if (pos != std::string::npos) {
			strLine.resize(pos);
		}

		int n = Util_ResolveTextLine(strLine.c_str(), fields.get(), MAX_FIELDS + 1, '\t');
		if (n < 2) {
			continue;
		}

		// Разметка колонок legacy <map>NPC.txt:
		// [0]  id
		// [1]  data_name
		// [2]  sNpcType
		// [3]  sCharID
		// [4]  byShowType
		// [5]  "x,y"  pos0
		// [6]  "x,y"  pos1
		// [7]  sDir
		// [8]  sParam1
		// [9]  sParam2
		// [10] szNpc   (npc_link)
		// [11] szMsgProc
		// [12] szMisProc
		CNpcRecord r{};
		r.Id = Str2Int(fields[0]);
		Util_TrimTabString(fields[1]);

		r.DataName = fields[1];
		CopyFixed(r.szName, std::string_view{fields[1]});

		r.sNpcType   = n >  2 ? static_cast<USHORT>(Str2Int(fields[2])) : 0;
		r.sCharID    = n >  3 ? static_cast<USHORT>(Str2Int(fields[3])) : 0;
		r.byShowType = n >  4 ? static_cast<BYTE>(Str2Int(fields[4])) : 0;

		auto [x0, y0] = n > 5 ? ParseXY(fields[5]) : std::pair<int, int>{0, 0};
		auto [x1, y1] = n > 6 ? ParseXY(fields[6]) : std::pair<int, int>{0, 0};
		r.dwxPos0 = static_cast<DWORD>(x0);
		r.dwyPos0 = static_cast<DWORD>(y0);
		r.dwxPos1 = static_cast<DWORD>(x1);
		r.dwyPos1 = static_cast<DWORD>(y1);

		r.sDir    = n >  7 ? static_cast<USHORT>(Str2Int(fields[7])) : 0;
		r.sParam1 = n >  8 ? static_cast<USHORT>(Str2Int(fields[8])) : 0;
		r.sParam2 = n >  9 ? static_cast<USHORT>(Str2Int(fields[9])) : 0;

		if (n > 10) {
			CopyFixed(r.szNpc, std::string_view{fields[10]});
		}
		if (n > 11) {
			CopyFixed(r.szMsgProc, std::string_view{fields[11]});
		}
		if (n > 12) {
			CopyFixed(r.szMisProc, std::string_view{fields[12]});
		}

		_records.push_back(std::move(r));
	}

	// Индекс строится после полной загрузки — вектор больше не перевыделяется,
	// указатели в _idIndex стабильны до следующего Load().
	_idIndex.reserve(_records.size());
	for (auto& r : _records) {
		_idIndex[r.Id] = &r;
	}

	ToLogService("common", "NpcRecordStore: загружено {} записей из '{}'",
				 _records.size(), txtPath);
	return true;
}

CNpcRecord* NpcRecordStore::Get(int id) {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}

const CNpcRecord* NpcRecordStore::Get(int id) const {
	auto it = _idIndex.find(id);
	return it != _idIndex.end() ? it->second : nullptr;
}
