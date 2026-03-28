//=============================================================================
// FileName: LogFile.cpp
// Creater: ZhangXuedong
// Date: 2004.11.19
// Comment: CLogFile class
//=============================================================================

#include "LogFile.h"
#include "util.h"

CLogFile	*g_pCLogObj = 0;

void CLogFile::Log(const char* format, ...)
{
	if (!m_bLogEnable)
		return;

	char buf[1000] = {0};
	int len;

	va_list args;
	va_start(args, format);
	len = vsprintf(buf, format, args);	
	va_end(args);

	if (len > sizeof buf) throw std::logic_error("format len > 1000\n");
	g_logManager.InternalLog(LogLevel::Debug, m_szLogName, buf);
}
