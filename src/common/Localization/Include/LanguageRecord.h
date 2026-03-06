#pragma once

#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include "StringNames.h"


class CLanguageRecord {
	std::unordered_map<int, std::string> _stringById{};
	std::unordered_map<std::string, std::string> _stringByName{};

	void AddLineStrByNumber(std::string line);
	void AddLineStrByString(std::string szLine);

public:
	CLanguageRecord(void);
	CLanguageRecord(const std::string& szTxtFile);
	~CLanguageRecord(void);
	const char* GetString(int nID) const;
	const char* GetString(std::string_view nID) const;

	bool LoadFromBinFile(const std::string& szBinFile);
	bool LoadFromTxtFile(const std::string& szTxtFile);
	bool LoadFromTxtStringFile(const std::string& szTxtFile);
	std::size_t GetRecordCount() const;
	std::size_t GetRecordStringCount() const;
	static bool MadeBinFile(const std::string& szBinFile, const std::string& szTxtFile);
};

extern CLanguageRecord& CLanguageRecordInstance();

#define RES_STRING(x) (CLanguageRecordInstance().GetString(x))
const char* ConvertResString(std::string_view key);
