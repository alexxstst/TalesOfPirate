#include "StdAfx.h"
#include "Tools.h"
#include <strstream>

using namespace std;

string ConvertNumToChinese(int num) {
	if (num == 0) return GetLanguageString(421);

	char szNum[255] = {0};
	itoa(num, szNum, 10);

	char szBuf[255] = {0};
	std::ostrstream str(szBuf, sizeof(szBuf));

	const char* pszPos = szNum;
	if (*pszPos == '-') {
		str << GetLanguageString(422).c_str();
		pszPos++;
	}

	static char szChinese[10][16];
	// = { GetLanguageString(421), GetLanguageString(423), GetLanguageString(424), GetLanguageString(425), GetLanguageString(426), GetLanguageString(427), GetLanguageString(428), GetLanguageString(429), GetLanguageString(430), GetLanguageString(431) };
	strncpy_s(szChinese[0], sizeof(szChinese[0]), GetLanguageString(421).c_str(), _TRUNCATE);
	strncpy_s(szChinese[1], sizeof(szChinese[1]), GetLanguageString(423).c_str(), _TRUNCATE);
	strncpy_s(szChinese[2], sizeof(szChinese[2]), GetLanguageString(424).c_str(), _TRUNCATE);
	strncpy_s(szChinese[3], sizeof(szChinese[3]), GetLanguageString(425).c_str(), _TRUNCATE);
	strncpy_s(szChinese[4], sizeof(szChinese[4]), GetLanguageString(426).c_str(), _TRUNCATE);
	strncpy_s(szChinese[5], sizeof(szChinese[5]), GetLanguageString(427).c_str(), _TRUNCATE);
	strncpy_s(szChinese[6], sizeof(szChinese[6]), GetLanguageString(428).c_str(), _TRUNCATE);
	strncpy_s(szChinese[7], sizeof(szChinese[7]), GetLanguageString(429).c_str(), _TRUNCATE);
	strncpy_s(szChinese[8], sizeof(szChinese[8]), GetLanguageString(430).c_str(), _TRUNCATE);
	strncpy_s(szChinese[9], sizeof(szChinese[9]), GetLanguageString(431).c_str(), _TRUNCATE);

	static char szHigh[8][16];
	// = { GetLanguageString(432), GetLanguageString(433), GetLanguageString(434), GetLanguageString(435), GetLanguageString(432), GetLanguageString(433), GetLanguageString(434), GetLanguageString(436) };
	strncpy_s(szHigh[0], sizeof(szHigh[0]), GetLanguageString(432).c_str(), _TRUNCATE);
	strncpy_s(szHigh[1], sizeof(szHigh[1]), GetLanguageString(433).c_str(), _TRUNCATE);
	strncpy_s(szHigh[2], sizeof(szHigh[2]), GetLanguageString(434).c_str(), _TRUNCATE);
	strncpy_s(szHigh[3], sizeof(szHigh[3]), GetLanguageString(435).c_str(), _TRUNCATE);
	strncpy_s(szHigh[4], sizeof(szHigh[4]), GetLanguageString(432).c_str(), _TRUNCATE);
	strncpy_s(szHigh[5], sizeof(szHigh[5]), GetLanguageString(433).c_str(), _TRUNCATE);
	strncpy_s(szHigh[6], sizeof(szHigh[6]), GetLanguageString(434).c_str(), _TRUNCATE);
	strncpy_s(szHigh[7], sizeof(szHigh[7]), GetLanguageString(436).c_str(), _TRUNCATE);

	char nChar = 0;
	int nZeroNum = 0;
	bool IsBigMark = false; // 
	int nHigh = 0;
	int nLen = 0;
	while (*pszPos) {
		nChar = *pszPos++;
		nLen = (int)strlen(pszPos);

		// ,
		if (nChar == '0') {
			nZeroNum++;
			if (IsBigMark && (nLen == 4 || nLen == 8)) {
				str << szHigh[nLen - 1];
				IsBigMark = false;
			}
			continue;
		}
		else {
			IsBigMark = true;
			if (nZeroNum > 0) // ,
			{
				nZeroNum = 0;
				str << GetLanguageString(421).c_str();
			}
			str << szChinese[nChar - '0'];
			if (nLen > 0) {
				if (nLen < 9) {
					nHigh = nLen - 1;
				}
				else {
					nLen = nLen % 9;
					nHigh = nLen;
				}
				str << szHigh[nHigh];

				// 
				if (nHigh == 3 || nHigh == 7) {
					IsBigMark = false;
				}
			}
		}
	}
	str << ends;

	// ,
	string rv = str.str();
	if (rv.length() >= 4 && rv.substr(0, 4) == GetLanguageString(437))
		return rv.substr(2, rv.length());

	return rv;
}
