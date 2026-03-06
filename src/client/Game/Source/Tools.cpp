#include "StdAfx.h"
#include "Tools.h"
#include <strstream> 

using namespace std;

string ConvertNumToChinese( int num )
{
	if( num==0 ) return g_oLangRec.GetString(421);

	char szNum[255] = { 0 };
	_itoa( num, szNum, 10 );

	char szBuf[255] = { 0 };
    std::ostrstream str( szBuf, sizeof(szBuf) );

	const char* pszPos = szNum;
	if( *pszPos=='-' )
	{
		str << g_oLangRec.GetString(422);
		pszPos++;
	}

	static char szChinese[10][16];// = { g_oLangRec.GetString(421), g_oLangRec.GetString(423), g_oLangRec.GetString(424), g_oLangRec.GetString(425), g_oLangRec.GetString(426), g_oLangRec.GetString(427), g_oLangRec.GetString(428), g_oLangRec.GetString(429), g_oLangRec.GetString(430), g_oLangRec.GetString(431) };
	strcpy(szChinese[0], g_oLangRec.GetString(421));
	strcpy(szChinese[1], g_oLangRec.GetString(423));
	strcpy(szChinese[2], g_oLangRec.GetString(424));
	strcpy(szChinese[3], g_oLangRec.GetString(425));
	strcpy(szChinese[4], g_oLangRec.GetString(426));
	strcpy(szChinese[5], g_oLangRec.GetString(427));
	strcpy(szChinese[6], g_oLangRec.GetString(428));
	strcpy(szChinese[7], g_oLangRec.GetString(429));
	strcpy(szChinese[8], g_oLangRec.GetString(430));
	strcpy(szChinese[9], g_oLangRec.GetString(431));

	static char szHigh[8][16];// = { g_oLangRec.GetString(432), g_oLangRec.GetString(433), g_oLangRec.GetString(434), g_oLangRec.GetString(435), g_oLangRec.GetString(432), g_oLangRec.GetString(433), g_oLangRec.GetString(434), g_oLangRec.GetString(436) };
	strcpy(szHigh[0], g_oLangRec.GetString(432));
	strcpy(szHigh[1], g_oLangRec.GetString(433));
	strcpy(szHigh[2], g_oLangRec.GetString(434));
	strcpy(szHigh[3], g_oLangRec.GetString(435));
	strcpy(szHigh[4], g_oLangRec.GetString(432));
	strcpy(szHigh[5], g_oLangRec.GetString(433));
	strcpy(szHigh[6], g_oLangRec.GetString(434));
	strcpy(szHigh[7], g_oLangRec.GetString(436));
	
	char nChar = 0;
	int nZeroNum = 0;				
	bool IsBigMark = false;		// 
	int nHigh = 0;
	int nLen = 0;
	while( *pszPos )
	{
		nChar = *pszPos++;
		nLen = (int)strlen(pszPos);

		// ,
		if( nChar=='0' )
		{
			nZeroNum++;
			if( IsBigMark && (nLen==4 || nLen==8) )
			{
				str << szHigh[ nLen-1 ];
				IsBigMark = false;
			}
			continue;
		}
		else
		{
			IsBigMark = true;
			if( nZeroNum>0 )    // ,
			{
				nZeroNum = 0;
				str << g_oLangRec.GetString(421);
			}
			str << szChinese[ nChar - '0' ];
			if( nLen > 0 )
			{
				if( nLen < 9 )
				{
					nHigh = nLen-1;
				}
				else
				{
					nLen = nLen % 9;
					nHigh = nLen;
				}
				str << szHigh[nHigh];

				// 
				if( nHigh==3 || nHigh==7 )
				{
					IsBigMark = false;
				}
			}
		}
	}
    str << ends;

	// ,
	string rv = str.str();
	if( rv.length()>=4 && rv.substr( 0, 4 ) == g_oLangRec.GetString(437) )
		return rv.substr( 2, rv.length() );

	return rv;
}
