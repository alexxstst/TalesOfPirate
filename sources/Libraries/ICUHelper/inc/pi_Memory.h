#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "psapi.h"

#include <iostream>
#include <fstream>

class pi_Memory
{
public:
	pi_Memory(const char* fileName = "memorymonitor.log");
	~pi_Memory(void);

public:
	bool startMonitor(int minuteInterval, DWORD processID = ::GetCurrentProcessId());
	bool stopMonitor();
	bool wait();

private:
	static DWORD WINAPI ThreadProc( LPVOID lpParam );
	
private:
	int m_minuteInterval;
	bool m_bStop;

	std::fstream m_outputFile;
	DWORD m_processID;
	DWORD m_dwThreadId;
	HANDLE m_hThread; 
	std::string m_szfileName;
};
