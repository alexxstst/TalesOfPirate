//------------------------------------------------------------------------
//	2005.3.25	Arcol	create this file
//	2005.4.7	Arcol	add function : bool GetNameFormString(const string &str,string &name)
//						add function : string CutFaceText(string &text,size_t cutLimitlen)
//	2005.4.18	Arcol	add function : bool GetNameFormString(const string &str,string &name)
//						add function : string CutFaceText(string &text,size_t cutLimitlen)
//	2005.4.19	Arcol	add function : void ChangeParseSymbol(string &text,int nMaxCount)
//	2005.4.28	Arcol	remove all filter system function and create a filter manager class in the common lib
//------------------------------------------------------------------------

#include "StringLib.h"
#include <mbstring.h>

#include "LanguageRecord.h"
extern CLanguageRecord g_oLangRec;

using namespace std;

//TODO(Ogge): There is is a flaw here
// new allocation may fail 
string StringLimit(const string& str,size_t len)
{
	if (len<=3||len>=str.length()) return str;
	char *buf=new char[len+5];
	memcpy(buf,str.c_str(),len);
	buf[len]=0;
	buf[len-2]=buf[len-1]='.';
	if (_ismbslead((unsigned char*)buf,(unsigned char*)(buf+len-3)))
	{
		buf[len-3]='.';
	}
	string ret=buf;
	if(buf) delete[] buf;

	return ret;
}

//------------------------------------------------------------------------
//	16
//------------------------------------------------------------------------
bool GetNameFormString(const string &str,string &name)
{
	size_t n=str.find(":");
	if ( n>24 ) return false;
	name=str.substr(0,n);
	while ( n && name[--n]==0x20) name.erase(n);
	if (name.find("<From ")==0 && name.length()>7)
	{
		name=name.substr(6,name.length()-7);
	}
	else if (name.find("<To ")==0 && name.length()>5)
	{
		name=name.substr(4,name.length()-5);
	}
	if (name.length()<=16) return true;
	return false;
}

//------------------------------------------------------------------------
//	
//------------------------------------------------------------------------
string CutFaceText(string &text,size_t cutLimitlen)
{
	string retStr=text;
	if ( cutLimitlen<=0 || text.length()<=cutLimitlen )
	{
		text.clear();
		return retStr;
	}

	if(0 == _stricmp(g_oLangRec.GetString(0), "English"))
	{
		// 
		string temp = text.substr(0,cutLimitlen);
		size_t nPos = temp.find_last_of(" ");
		if(nPos != string::npos && nPos > 8)
		{
			retStr = text.substr(0,nPos);
			cutLimitlen = nPos;
		}
		else
		{
			retStr=text.substr(0,cutLimitlen);
		}
	}
	else
	{
		// 
		retStr=text.substr(0,cutLimitlen);
	}

	if (_ismbslead((unsigned char*)text.c_str(),(unsigned char*)&text[cutLimitlen-1]))
	{
		retStr=text.substr(0,cutLimitlen-1);
	}
	if ((*--retStr.end())=='#')
	{
		retStr=retStr.substr(0,retStr.length()-1);
	}
	else if ((*(retStr.end()-2))=='#')
	{
		retStr=retStr.substr(0,retStr.length()-2);
	}
	text=text.substr(retStr.length(),text.length()-retStr.length());
	return retStr;
}

////------------------------------------------------------------------------
////	*
////------------------------------------------------------------------------
//void ReplaceText(string &text,const string strRpl)
//{
//	_W64 nPos=text.find(strRpl);
//	while (nPos>=0)
//	{
//		text.replace(nPos,strRpl.length(),strRpl.length(),'*');
//		nPos=text.find(strRpl,nPos+strRpl.length());
//	}
//}
//
////------------------------------------------------------------------------
////	
////------------------------------------------------------------------------
//void FilterText(string &text,vector<char*> *p_strFilterTxt)
//{
//	vector <char*>::iterator Iter;
//	for (Iter=p_strFilterTxt->begin();Iter!=p_strFilterTxt->end();Iter++)
//	{
//		ReplaceText(text,(*Iter));
//	}
//}

//------------------------------------------------------------------------
//	,0,:
//	#00 -> </00>
//	#01 -> </01>
//	#02 -> </02>
//	...nMaxCount-1
//------------------------------------------------------------------------
void ChangeParseSymbol(string &text,int nMaxCount)
{
	for (int i=0;i<nMaxCount;i++)
	{
		char szSrc[100];
		char szRpl[100];
		sprintf(szSrc,"#%.2d",i);
		sprintf(szRpl,"</%.2d>",i);
		auto nPos=text.find(szSrc);
		while (nPos != std::string::npos)
		{
			text.replace(nPos,strlen(szSrc),szRpl);
			nPos=text.find(szSrc,nPos+strlen(szRpl));
		}
	}
}


// InBufnWidthOutBuf
int StringNewLine( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen )
{
	if(0 == _stricmp(g_oLangRec.GetString(0), "english"))
	{
		// 
		return StringNewLineEng(pOutBuf, nWidth, pInBuf, nInLen);
	}
	else
	{
		// 
		return StringNewLineChs(pOutBuf, nWidth, pInBuf, nInLen);
	}
}


int StringNewLineChs( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen )
{	
	if( nWidth==0 ) return 0;

	unsigned int nOutPos = 0;
	unsigned int nInPos = 0;
	for(;;)
	{
		memcpy( pOutBuf + nOutPos, pInBuf + nInPos, nWidth );
		nInPos += nWidth;
		nOutPos += nWidth;
		if( _ismbslead( (unsigned char*)pInBuf, (unsigned char*)(pInBuf+nInPos) )==0 )
		{
			pOutBuf[ nOutPos++ ] = pInBuf[ nInPos++ ];
		}
		if( nInPos >= nInLen ) 
		{
			pOutBuf[nOutPos] = '\0';
			return nOutPos;
		}

		pOutBuf[ nOutPos++ ] = '\n';
	}
}


int StringNewLineEng( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen )
{	
	if( nWidth==0 ) return 0;

	unsigned int nOutPos   = 0;
	unsigned int nInPos    = 0;
	unsigned int nSpacePos = -1;
	unsigned int nLinePos  = 0;

	for(;;)
	{
		if(pInBuf[nInPos] == '\0')
		{
			pOutBuf[nOutPos] = '\0';
			break;
		}
		if(pInBuf[nInPos] == ' ')
		{
			nSpacePos = nLinePos;
		}

		if(nLinePos >= nWidth)
		{
			//if(pInBuf[nInPos + 1] == ' ')
			//{
			//	++nLinePos;
			//	nSpacePos = nLinePos;
			//}

			nOutPos -= nLinePos - nSpacePos;
			nInPos  -= nLinePos - nSpacePos;

			pOutBuf[nOutPos] = '\n';

			nLinePos = 0;
		}
		else
		{
			pOutBuf[nOutPos] = pInBuf[nInPos];
		}

		++nInPos;
		++nOutPos;
		++nLinePos;
	}

	return nOutPos;
}

const char* StringSplitNum( const char* bigIntBuf, int nCount, char cSplit )
{
	static char szBuf[256] = { 0 };
	static char szTmp[256] = { 0 };

	sprintf( szBuf, "%s", bigIntBuf );
	strcpy( szTmp, szBuf );

	int nLen = (int)strlen( szBuf );
	int nLoop = ( nLen - 1 ) / nCount;
	if( nLoop < 0 ) return szBuf;

	if( nLoop + nLen > sizeof(szBuf) ) return szBuf;

	szBuf[ nLen + nLoop ] = '\0';

	int nStart = 0;
	int nTarget = 0;
	for( int i=0; i<nLoop; i++ )
	{
		nStart = nLen - nCount * (i + 1);
		nTarget = nStart + nLoop - i;
		memcpy( &szBuf[nTarget], &szTmp[nStart], nCount );
		szBuf[nTarget-1] = cSplit;
	}
	return szBuf;
}

const char* StringSplitNum( long nNumber, int nCount, char cSplit )
{
	static char szBuf[256] = { 0 };
	static char szTmp[256] = { 0 };

	sprintf( szBuf, "%d", nNumber );
	strcpy( szTmp, szBuf );

	int nLen = (int)strlen( szBuf );
	int nLoop = ( nLen - 1 ) / nCount;
	if( nLoop < 0 ) return szBuf;

	if( nLoop + nLen > sizeof(szBuf) ) return szBuf;

	szBuf[ nLen + nLoop ] = '\0';

	int nStart = 0;
	int nTarget = 0;
	for( int i=0; i<nLoop; i++ )
	{
		nStart = nLen - nCount * (i + 1);
		nTarget = nStart + nLoop - i;
		memcpy( &szBuf[nTarget], &szTmp[nStart], nCount );
		szBuf[nTarget-1] = cSplit;
	}
	return szBuf;
}
