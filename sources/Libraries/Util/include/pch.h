
// pch.h
//  created by claude fan at 2004-8-31
//  for precompiled-header

#if !defined(LOGUTIL_PRECOMPILED_HEADER_FILE)
#define LOGUTIL_PRECOMPILED_HEADER_FILE

// 
#define LINE_COMMENT /##/
#define LC LINE_COMMENT
#define $ LINE_COMMENT

// C
#include <stdio.h>
#include <time.h>

// C++
#include <list>
#include <map>
#include <list>
#include <string>

// 
#if defined(WIN32)
	// Win32
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4786)

	#define NOMINMAX
	#include <windows.h>
	#include <io.h>
	#include <direct.h>

#elif defined(LINUX)
	// Linux
    #include <unistd.h>
    #include <string.h>
    
    #include <sys/io.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <sys/stat.h>
    #include <sys/dir.h>
    
    #define VOID    void
    #define BYTE    unsigned char
    #define LPBYTE  unsigned char*
    #define BOOL    bool
    #define DWORD   unsigned long
    #define LPCSTR const char*
    #define TRUE true
    #define FALSE false
    #define HANDLE void*

	#define _LOG
	#define _LOG_CONSOLE
#elif defined(FREEBSD)
	// FreeBSD

#endif


#endif
