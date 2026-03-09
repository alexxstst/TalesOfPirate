#include "windows.h"
#include "big5.h"


CBig5::CBig5()
{
}

CBig5::~CBig5()
{
	m_mapSentencesTable.clear();
}

CBig5 *CBig5::GetInstance()
{
	static CBig5 pThis;
	return &pThis;
}

const char* CBig5::DoBig5(const char *lpszStr, const char *lpszFileName, int nLineNum)
{
	if (lpszStr== nullptr || lpszStr[0]==0) return lpszStr;
	mapSentencesType::const_iterator iter=m_mapSentencesTable.find(lpszStr);
	if (iter!=m_mapSentencesTable.end())
	{
		return iter->second.c_str();
	}
	else
	{
		char *lpBuf=ChangeCode(lpszStr);
		if (!lpBuf) return "?";
		m_mapSentencesTable.insert(std::pair<std::string,std::string>(lpszStr, lpBuf));
		delete lpBuf;
		iter=m_mapSentencesTable.find(lpszStr);
		return iter->second.c_str();
	}
	return "?";
}

const char* CBig5::DoBig5_S(const char *lpszOrgStr, const char *lpszNowStr, const char *lpszFileName, int nLineNum)
{
	if (lpszOrgStr== nullptr || lpszOrgStr[0]==0) return lpszNowStr;
	mapSentencesType::const_iterator iter=m_mapSentencesTable.find(lpszOrgStr);
	if (iter!=m_mapSentencesTable.end())
	{
		return iter->second.c_str();
	}
	return lpszNowStr;
}

char* CBig5::ChangeCode(const char *lpszOrgStr)
{
	int nOrgStrLen=(int)strlen(lpszOrgStr);
	DWORD dwLCID=MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nSize1=LCMapString(dwLCID, LCMAP_TRADITIONAL_CHINESE, lpszOrgStr, nOrgStrLen, nullptr, 0);
	if (nSize1==0) return nullptr;

	char *lpBuf1=new char[nSize1*2+2];
	if (LCMapString(dwLCID, LCMAP_TRADITIONAL_CHINESE, lpszOrgStr, nOrgStrLen, lpBuf1, nSize1+2)!=nSize1)
	{
		delete lpBuf1;
		return nullptr;
	}
    int nSize2=MultiByteToWideChar(936, 0, lpBuf1, nSize1, nullptr, 0);
	if (nSize2==0) return nullptr;

	wchar_t *lpBuf2=new wchar_t[nSize2+2];
	if (MultiByteToWideChar(936, 0, lpBuf1, nSize1, lpBuf2, nSize2+2)!=nSize2)
	{
		delete lpBuf1;
		delete lpBuf2;
		return nullptr;
	}

	BOOL bValue = false;
	ZeroMemory(lpBuf1, nSize1*2+2);
	if (WideCharToMultiByte(950, 0, lpBuf2, nSize2, lpBuf1, nSize1*2+2, "?", &bValue)==0)
	{
		delete lpBuf1;
		delete lpBuf2;
		return nullptr;
	}

	delete lpBuf2;
	return lpBuf1;
}
