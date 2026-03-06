#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <string>
std::string StringLimit(const std::string& str,size_t len);
bool GetNameFormString(const std::string &str, std::string &name);
std::string CutFaceText(std::string &text,size_t cutLimitlen);
//void ReplaceText(string &text,const string strRpl);
//void FilterText(string &text,vector<char*> *p_strFilterTxt);
void ChangeParseSymbol(std::string &text,int nMaxCount);
int StringNewLine( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );

int StringNewLineChs( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );
int StringNewLineEng( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );


// StringSplitNumszBufnCountcSplit
const char* StringSplitNum( long nNumber, int nCount=3, char cSplit=',' );
const char* StringSplitNum( const char* bigIntBuf, int nCount=3, char cSplit=',' );

