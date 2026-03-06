#pragma once

//=============================================================================
// FileName: LogFile.h
// Creater: ZhangXuedong
// Date: 2004.11.19
// Comment: CLogFile class
//=============================================================================

#ifndef LOGFILE_H
#define LOGFILE_H

#include <string.h>

#define defLOG_NAME_LEN	64 // 3 + 1 + 32 + 1 + 10

class CLogFile
{
public:
	void SetEnable(bool bEnable = true) {m_bLogEnable = bEnable;}
	bool GetEnable() {return m_bLogEnable;}
	void SetLogName(const char *szName) {strncpy(m_szLogName, szName, sizeof(char) * (defLOG_NAME_LEN -1)); m_szLogName[defLOG_NAME_LEN -1] = '\0';}
	char *GetLogName() {return m_szLogName;}
	void Log(const char* format, ...);

protected:

private:
	char	m_szLogName[defLOG_NAME_LEN];
	bool	m_bLogEnable;

};

extern CLogFile	*g_pCLogObj;

#endif // LOGFILE_H
