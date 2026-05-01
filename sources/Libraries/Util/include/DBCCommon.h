//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// The last modification of this program is in 2003.7
//=================================================================
//include file;
#pragma once

#ifndef	_WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#ifndef	_WIN32_WINDOWS
#define _WIN32_WINDOWS _WIN32_WINNT_WIN7
#endif

#ifndef USING_TAO							//Win32Platform SDK
#include <winsock2.h>						//WinSock2.2
#define NOMINMAX
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include <atomic>
#include <cstdint>


//----------------------------------------------------------------
#define _DBC_BEGIN	namespace dbc {
#define _DBC_END		};
#define _DBC_USING	using namespace dbc;

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

	//=================================================================
#define	nil 0
	//typedef type define
	typedef char Char; //8(=1)/
	typedef wchar_t wChar;
	typedef short Short; //2
	typedef long Long; //4
	typedef int Int; //(32uLong)
	typedef const char cChar;
	typedef const wchar_t cwChar;
	typedef const short cShort;
	typedef const long cLong;
	typedef const int cInt;
	typedef unsigned char uChar; //8(=1)/
	typedef unsigned short uShort; //2
	typedef unsigned long uLong; //4
	typedef unsigned int uInt; //(32uLong)
	typedef const unsigned char cuChar;
	typedef const unsigned short cuShort;
	typedef const unsigned long cuLong;
	typedef const unsigned int cuInt;
	typedef wchar_t WChar; //2
	typedef __int64 LLong; //8
#define SIGN32	uLong(0x80000000)
#define SIGN16	uShort(0x8000)
#define SIGN8	uChar(0x80)


	//=================================================================
	template <typename T>
	inline T* ToPointer(std::uintptr_t address) {
		return reinterpret_cast<T*>(address);
	}

	inline std::uintptr_t ToAddress(void* pointer) {
		return reinterpret_cast<std::uintptr_t>(pointer);
	}

	//=================================================================
	//TODO(Ogge): Remove MemCpy and MemSet
	inline char* MemCpy(char* dest, cChar* src, uLong size) {
		return (char*)memcpy(dest, src, size);
	}

	inline char* MemSet(char* dest, int val, uLong size) {
		return (char*)memset(dest, val, size);
	}


#pragma pack(pop)
_DBC_END
