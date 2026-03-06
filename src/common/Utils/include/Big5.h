#pragma once

#include <string>
#include <map>

class CBig5
{
public:
	~CBig5();
	static CBig5 *GetInstance();
	const char* DoBig5(const char *lpszStr, const char *lpszFileName, int nLineNum);
	const char* DoBig5_S(const char *lpszOrgStr, const char *lpszNowStr, const char *lpszFileName, int nLineNum);

protected:
	CBig5(void);
	char* ChangeCode(const char *lpszOrgStr);

private:
	typedef std::map<std::string, std::string>	mapSentencesType;
	mapSentencesType							m_mapSentencesTable;
};


#define TO_BIG5(s)		CBig5::GetInstance()->DoBig5(#s, __FILE__, __LINE__)
#define TO_BIG5_S(s)	CBig5::GetInstance()->DoBig5_S(#s, L_##s, __FILE__, __LINE__)
