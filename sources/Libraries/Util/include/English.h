#pragma once

class CEnglish
{
public:
	CEnglish(void);
	static CEnglish *GetInstance();
	const char* DoEnglish(const char *lpszStr, const char *lpszFileName, int nLineNum);
	const char* DoEnglish_S(const char *lpszOrgStr, const char *lpszNowStr, const char *lpszFileName, int nLineNum);

protected:
	~CEnglish(void);

private:
};

#define TO_ENGLISH(s)		CEnglish::GetInstance()->DoEnglish(#s, __FILE__, __LINE__)
#define TO_ENGLISH_S(s)		CEnglish::GetInstance()->DoEnglish_S(#s, L_##s, __FILE__, __LINE__)
