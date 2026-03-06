#pragma once

#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_


//#include "BillThread.h"

#include "tlsindex.h"
#include "databasectrl.h"
//Add by lark.li 20080307


//#define _RELOGIN_MODE_					//

extern std::string g_strCfgFile;

extern dbc::TLSIndex	g_TlsIndex;
extern CDataBaseCtrl	g_MainDBHandle;
extern HWND				g_hMainWnd;
extern DWORD			g_MainThreadID;
//extern BillThread g_BillThread;

#define WM_USER_LOG		WM_USER+100
#define WM_USER_LOG_MAP	WM_USER+101


struct sUserLog
{
	bool bLogin;		//true:login  false:logout
	int nUserID;
	std::string strUserName;
	std::string strPassport;
	std::string strLoginIP;
};


extern HANDLE hConsole;

#define C_PRINT(s, ...) \
	SetConsoleTextAttribute(hConsole, 14); \
	printf(s, __VA_ARGS__); \
	SetConsoleTextAttribute(hConsole, 10);

#define C_TITLE(s) \
	char szPID[32]; \
	_snprintf_s(szPID,sizeof(szPID),_TRUNCATE, "%d", GetCurrentProcessId()); \
	std::string strConsoleT; \
	strConsoleT += "[PID:"; \
	strConsoleT += szPID; \
	strConsoleT += "]"; \
	strConsoleT += s; \
	SetConsoleTitle(strConsoleT.c_str());


#endif
