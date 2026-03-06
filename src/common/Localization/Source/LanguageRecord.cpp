#include <filesystem>
#include "enclib.h"

#include "LanguageRecord.h"
#include <StringsUtil.h>

__byte ENC_KEY[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

CLanguageRecord::CLanguageRecord() = default;

CLanguageRecord::CLanguageRecord(const std::string& szTxtFile) {
	//if (!LoadFromBinFile(szBinFile)) {
	LoadFromTxtFile(szTxtFile);
	//}
}

CLanguageRecord::~CLanguageRecord() = default;

const char* CLanguageRecord::GetString(int nID) const {
	const auto it = _stringById.find(nID);
	if (it != _stringById.cend()) {
		return it->second.c_str();
	}

	return "";
}

const char* CLanguageRecord::GetString(std::string_view nID) const {
	if (!_stringByName.empty()) {
		const auto it = _stringByName.find(std::string(nID));
		if (it != _stringByName.cend()) {
			return it->second.c_str();
		}
	}

	return nID.data();
}

bool CLanguageRecord::LoadFromBinFile(const std::string& szBinFile) {
	_stringById.clear();

	std::ifstream fs;
	fs.open(szBinFile);

	if (!fs.is_open())
		return false;

	char szBuffer[1024]{};
	char szNewBuf[1024]{};
	SetEncKey(ENC_KEY);

	while (fs.getline(szBuffer, std::size(szBuffer))) {
		Decrypt(reinterpret_cast<__byte*>(szNewBuf), 1024, reinterpret_cast<__byte*>(szBuffer), strlen(szBuffer));
		AddLineStrByNumber(szNewBuf);
	}

	return true;
}

bool CLanguageRecord::LoadFromTxtFile(const std::string& szTxtFile) {
	_stringById.clear();

	std::ifstream fs{};
	fs.open(szTxtFile);

	if (!fs.is_open())
		return false;

	std::string line{};
	while (std::getline(fs, line)) {
		AddLineStrByNumber(line);
	}

	return true;
}

bool CLanguageRecord::LoadFromTxtStringFile(const std::string& szTxtFile) {
	_stringByName.clear();

	std::ifstream fs{};
	fs.open(szTxtFile);

	if (!fs.is_open())
		return false;

	std::string line{};
	while (std::getline(fs, line)) {
		AddLineStrByString(line);
	}

	return true;
}

bool CLanguageRecord::MadeBinFile(const std::string& szBinFile, const std::string& szTxtFile) {
	if (!std::filesystem::exists(szBinFile)) {
		return false;
	}

	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open(szTxtFile);
	ofs.open(szBinFile);

	if (!ifs.is_open() || !ofs.is_open())
		return false;

	SetEncKey(ENC_KEY);
	char szBuffer[1024]{};
	char szBinBuf[1024]{};
	while (ifs.getline(szBuffer, std::size(szBuffer))) {
		Encrypt(reinterpret_cast<__byte*>(szBinBuf), 1024, reinterpret_cast<__byte*>(szBuffer), strlen(szBuffer) + 1);

		ofs.write(szBinBuf, strlen(szBinBuf));
		ofs.write("\n", 1);
	}

	ofs.flush();
	return false;
}

std::size_t CLanguageRecord::GetRecordCount() const {
	return _stringById.size();
}

std::size_t CLanguageRecord::GetRecordStringCount() const {
	return _stringByName.size();
}

void CLanguageRecord::AddLineStrByNumber(std::string line) {
	ReplaceStringInPlace(line, "\\n", "\n");
	ReplaceStringInPlace(line, "\\t", "\t");

	const auto nPos = line.find('\t');
	if (nPos == std::string::npos) {
		return;
	}

	const auto strID = line.substr(0, nPos);
	const auto strLine = line.substr(nPos + 1, line.size() - nPos - 1);

	if (2 < strID.size() && strID[0] == '[' && strID[strID.size() - 1] == ']' &&
		2 < strLine.size() && strLine[0] == '\"' && strLine[strLine.size() - 1] == '\"') {
		const auto nID = atol(strID.substr(1, strID.size() - 2).c_str());
		_stringById[nID] = strLine.substr(1, strLine.size() - 2);
	}
}

void CLanguageRecord::AddLineStrByString(std::string line) {
	ReplaceStringInPlace(line, "\n", "\n");
	ReplaceStringInPlace(line, "\t", "\t");

	const auto nPos = line.find("\t");
	if (nPos <= 0) {
		return;
	}

	const auto strID = line.substr(0, nPos);
	const auto strLine = line.substr(nPos + 1, line.size() - nPos - 1);
	_stringByName[strID] = strLine;
}

const char* ConvertResString(std::string_view key) {
	if (!key.empty() && '#' == *key.begin() && '#' == *key.rbegin()) {
		const std::string_view temp = key.substr(1, key.size() - 2);
		return RES_STRING(temp);
	}

	return key.data();
}
