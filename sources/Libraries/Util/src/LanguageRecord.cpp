
#include "LanguageRecord.h"
#include <io.h>

__byte ENC_KEY[] = {0x42,0x34,0x19,0x56,0x10,0x14,0x29,0x72};


CLanguageRecord::CLanguageRecord(void)
{
}

CLanguageRecord::CLanguageRecord(const char* szBinFile, const char* szTxtFile)
{
	if(! LoadFromBinFile(szBinFile))
	{
		LoadFromTxtFile(szTxtFile);
	}
}


CLanguageRecord::~CLanguageRecord(void)
{
}


// 
const char* CLanguageRecord::GetString(int nID)
{
	std::map<int, std::string>::iterator it = m_mapString.find(nID);

	if(it != m_mapString.end())
	{
		// 
		return it->second.c_str();
	}

	// 
	return "";
}


//  BIN BIN -> TXT
bool CLanguageRecord::LoadFromBinFile(const char* szBinFile)
{
	m_mapString.clear();

	std::ifstream fs;
	fs.open(szBinFile);

	if(! fs.is_open())
		return false;

	char szBuffer[1024];
	char szNewBuf[1024];

	SetEncKey(ENC_KEY);

	while(fs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		// 
		Decrypt((__byte*)szNewBuf, 1024, (__byte*)szBuffer, strlen(szBuffer));

		Add(szNewBuf);
	}

	return true;
}


//  TXT 
//
//	[0] <TAB> ""
//
bool CLanguageRecord::LoadFromTxtFile(const char* szTxtFile)
{
	m_mapString.clear();

	std::ifstream fs;
	fs.open(szTxtFile);

	if(! fs.is_open())
		return false;

	char szBuffer[1024];
	while(fs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		Add(szBuffer);
	}

	return true;
}


// TXT -> BIN
bool CLanguageRecord::MadeBinFile(const char* szBinFile, const char* szTxtFile)
{
	if(-1 != access(szBinFile, 0))	// BIN 
		return false;

	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open(szTxtFile);
	ofs.open(szBinFile);

	if(! ifs.is_open() || ! ofs.is_open())
		return false;

	SetEncKey(ENC_KEY);
	char szBuffer[1024];
	char szBinBuf[1024];
	while(ifs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		Encrypt((__byte*)szBinBuf, 1024, (__byte*)szBuffer, strlen(szBuffer) + 1);

		ofs.write(szBinBuf, strlen(szBinBuf));
		ofs.write("\n", 1);
	}

	ofs.flush();

	return false;
}


// 
int CLanguageRecord::GetRecordCount(void)
{
	return (int)(m_mapString.size());
}


// 
void CLanguageRecord::Add(char* szLine)
{
	// 
	ReplaceString(szLine, "\\n", "\n");
	ReplaceString(szLine, "\\t", "\t");

	size_t nPos;
	int nID;
	std::string strLine;
	std::string strID;

	strLine = szLine;
	if(7 >= strLine.size() || (nPos = strLine.find("\t")) == std::string::npos)
	{
		return;
	}

	strID   = strLine.substr(0, nPos);
	strLine = strLine.substr(nPos + 1, strLine.size() - nPos - 1);

	if(2 < strID.size() && strID[0] == '[' && strID[(int)strID.size() - 1] == ']' &&
		2 < strLine.size() && strLine[0] == '\"' && strLine[(int)strLine.size() - 1] == '\"')
	{
		nID = atoi(strID.substr(1, (int)strID.size() - 2).c_str());
		m_mapString[nID] = strLine.substr(1, (int)strLine.size() - 2);
	}
}


// 
int CLanguageRecord::ReplaceString(char* _str, const char* _old, const char* _new)
{
    int __pos = 0;
	__pos = Find(_str, _old); // find the occurance of _old in _str
    if(!__pos)
    {
        return 0;
    }

    char* __ptr = 0;
    int __oldlen = (int)strlen(_old);
    int __newlen = (int)strlen(_new);
    int __other = 0;

    __ptr = _str + (__pos - 1); // pointer to beginning of string to be replaced
    __other = (int)strlen(__ptr + __oldlen); // pointer past the string to be replaced

	auto __tmp = std::make_unique<char[]>(__other + 1); // allocated and array big enough to hold the data past the string to be replaced

    strcpy(__tmp.get(), __ptr + __oldlen); // save the data past the string to be replaced
    strcpy(__ptr, _new); // overwrite the new string over the old one

    __ptr += __newlen; // increment the pointer to point to the character past the newly replaced string

	strcpy(__ptr, __tmp.get()); // append the old end 

	__tmp.reset(nullptr);

    return ReplaceString(__ptr, _old, _new) + 1;
}


// 
int CLanguageRecord::Find(const char* _str, const char* _find)
{
    char* __ptr = 0;

    __ptr = (char*)strstr(_str, _find);
    if(__ptr)
    {
        return (int)(__ptr - _str + 1);
    }
    return 0;
}

