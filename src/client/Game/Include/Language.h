//#pragma once
//
//#define GB2312		1
//#define BIG5		2
//#define ENGLISH		3
//#define EUC			4
//#define SHIFT_JIS	5
//
//
//#define LANGUAGE_CODE	GB2312
//
//
//
//
//#ifdef LANGUAGE_CODE
//	#if LANGUAGE_CODE==GB2312
//		#include "GB2312.h"
//		#define DO_TEXT(s)		#s
//		#define DO_TEXT_S(s)	#s
//	#elif LANGUAGE_CODE==BIG5
//		#include "Big5.h"
//		#define DO_TEXT(s)		TO_BIG5(s)
//		#define DO_TEXT_S(s)	TO_BIG5_S(s)
//	#elif LANGUAGE_CODE==ENGLISH
//		#include "English.h"
//		#define DO_TEXT(s)		TO_ENGLISH(s)
//		#define DO_TEXT_S(s)	TO_ENGLISH_S(s)
//	#elif LANGUAGE_CODE==EUC
//	#elif LANGUAGE_CODE==SHIFT_JIS
//	#else
//		#error "unknow LANGUAGE_CODE define, choice a language!"
//	#endif
//	
//	#define LS(s)	DO_TEXT(s)
//	#define LS_S(s) DO_TEXT_S(s)
//
//#else
//	#error "undefine LANGUAGE_CODE , choice a language!"
//#endif
