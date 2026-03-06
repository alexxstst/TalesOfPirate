#include "English.h"

CEnglish::CEnglish(void)
{
}

CEnglish::~CEnglish(void)
{
}

CEnglish *CEnglish::GetInstance()
{
	static CEnglish pThis;
	return &pThis;
}

const char* CEnglish::DoEnglish(const char *lpszStr, const char *lpszFileName, int nLineNum)
{
#if LANGUAGE_DEBUG==1
#endif
	return "??";
}

const char* CEnglish::DoEnglish_S(const char *lpszOrgStr, const char *lpszNowStr, const char *lpszFileName, int nLineNum)
{
#if LANGUAGE_DEBUG==1
#endif
	return "??";
}
