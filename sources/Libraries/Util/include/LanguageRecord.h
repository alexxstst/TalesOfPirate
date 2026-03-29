
//
//	
//
//	add by Philip.Wu  2006-07-18
//

#pragma once

#include <fstream>
#include <string>
#include <map>

#include "enclib.h"


class CLanguageRecord
{
public:
	CLanguageRecord(void);
	CLanguageRecord(const char* szBinFile, const char* szTxtFile);

	~CLanguageRecord(void);

	// 
	const char* GetString(int nID);

	// 
	bool LoadFromBinFile(const char* szBinFile);

	// 
	bool LoadFromTxtFile(const char* szTxtFile);

	// 
	int GetRecordCount(void);

	// 
	bool MadeBinFile(const char* szBinFile, const char* szTxtFile);

private:

	//  map
	std::map<int, std::string> m_mapString;

	// 
	void Add(char* szLine);

	// 
	int ReplaceString(char* _str, const char* _old, const char* _new);

	// 
	int Find(const char* _str, const char* _find);
};

