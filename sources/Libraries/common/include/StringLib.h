#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>
#include <string_view>
std::string StringLimit(const std::string& str,size_t len);
bool GetNameFormString(const std::string &str, std::string &name);
std::string CutFaceText(std::string &text,size_t cutLimitlen);
//void ReplaceText(string &text,const string strRpl);
//void FilterText(string &text,vector<char*> *p_strFilterTxt);
void ChangeParseSymbol(std::string &text,int nMaxCount);
int StringNewLine( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );

int StringNewLineChs( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );
int StringNewLineEng( char* pOutBuf, unsigned int nWidth, const char* pInBuf, unsigned int nInLen );


// Разбить строку по разделителю. Пробелы вокруг токенов обрезаются.
std::vector<std::string> SplitString(std::string_view str, char delimiter = ',');

// Разбить строку по разделителю в вектор int.
std::vector<int> SplitStringInt(std::string_view str, char delimiter = ',');

// StringSplitNumszBufnCountcSplit
const char* StringSplitNum( long nNumber, int nCount=3, char cSplit=',' );
const char* StringSplitNum( const char* bigIntBuf, int nCount=3, char cSplit=',' );

