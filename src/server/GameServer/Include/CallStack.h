#pragma once

#ifndef __CALLSTACK_H__
#define __CALLSTACK_H__

#include "StackWalker.h"

// Simple implementation of an additional output to the console:
class MyStackWalker : public StackWalker
{
public:
	MyStackWalker() : StackWalker() {}
	MyStackWalker(DWORD dwProcessId, HANDLE hProcess) : StackWalker(dwProcessId, hProcess) {}
	virtual void OnOutput(LPCSTR szText) 
	{
		//	
		char sName[24];

		sprintf( sName, "c:\\exception%d.txt", GetCurrentThreadId() );

		//
		FILE* fp;

		fp = fopen( sName, "a+" );

		if( !fp )
		{
			fp = fopen( sName, "w+" );
		}

		if( fp )
		{
			fprintf( fp,szText );
			fclose( fp );
		}

		//
		StackWalker::OnOutput(szText);
	}
};

// Exception handling and stack-walking example:
inline LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	MyStackWalker sw;
	sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif //__CALLSTACK_H__
