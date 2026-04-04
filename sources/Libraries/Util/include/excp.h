#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include "DBCCommon.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//------------------------------------------------------------------------------------------------------------------
//common exception define
class excp :public std::runtime_error			//
{
public:
	excp(const char* desc) : std::runtime_error(desc) {}
};
//------------------------------------------------------------------------------------------------------------------
class excpMem:public excp				//
{
public:
	excpMem(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpArr:public excp				//
{
public:
	excpArr(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpSync:public excp				//
{
public:
	excpSync(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpThrd:public excp				//
{
public:
	excpThrd(cChar * desc):excp(desc){};
};
class excpSock:public excp				//
{
public:
	excpSock(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpCOM:public excp				//COM
{
public:
	excpCOM(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpDB:public excp				//
{
public:
	excpDB(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpIniF:public excp				//
{
public:
	excpIniF(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpFile:public excp				//
{
public:
	excpFile(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpXML:public excp				//
{
public:
	excpXML(cChar * desc):excp(desc){};
};

#pragma pack(pop)
_DBC_END

//===================================================================================================================================

//From: https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define THROW_EXCP(EXCP, DESC) throw EXCP((std::string("File:") + __FILE__ + " Line:" + LINE_STRING + " desc:" + DESC).c_str());
