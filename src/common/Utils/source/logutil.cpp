//
// logutil.cpp
//
//  created by claude fan at 2004-8-9
//

#include "pch.h"
#include "log.h"
#include "log2.h"


#if 0
// printf
int myprintf(char const* fmt, ...)
    {
    char buf[8192];

    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    LG2("printf", buf);
    return 1;}

// fprintf
int myfprintf(FILE* fp, char const* fmt, ...)
    {
    char buf[8192];

    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    if (fp == stdout) {LG2("printf", buf);}
    else if (fp == stderr) {LG2("err", buf);}
    else LG2("other", buf);
    return 1;}

// LOG
static HWND LG_hwnd = NULL;
void LG(char const* type, char const* format, ...)
    {
	// LOG
	CLogMgr* logMgr = CLogMgr::Instance();
	if (logMgr == NULL || !logMgr->IsEnable()) return;
	CLog* log = logMgr->Add(type);
	if (log == NULL || !log->IsEnable()) return;

	char buf[8192] = {0};
	int len;
	va_list args;
	va_start(args, format);
	len = vsprintf(buf, format, args);
	va_end(args);
	if (len >= sizeof buf)
        throw std::logic_error("buf overflow, length > 8K\n");

    // 
    SYSTEMTIME st; char tim[100];
    ::GetLocalTime(&st);
    sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,
            st.wMinute, st.wSecond);


	// total buffer
	char buffer[8192];
	


    //////////////////////////////////////////////////////////////////////////
	//
	// LOG
#if 0
    string ctx;
    if (strncmp(buf, "msg", 3) != 0)
        {
        ctx = "["; ctx += tim; ctx += "]"; ctx += buf;
        fLG(type, ctx.c_str());}
    else fLG(type, buf);
#endif

	if (strncmp(buf, "msg", 3) != 0)
		{
		len = sprintf(buffer, "[%s]%s", tim, buf);
		if (len >= sizeof buffer)
			throw std::logic_error("buffer overflow, length >= 2K\n");
		fLG(type, buffer);}
	else {
		fLG(type, buf);}


    //////////////////////////////////////////////////////////////////////////
    //
    // LogviewerLOG
    if (LG_hwnd == NULL)
        {LG_hwnd = FindWindow(NULL, "kIng Of pIrAtEs lOgvIEwEr");}

    if (LG_hwnd == NULL) return;

#if 0
    ctx.clear();
    ctx += "LOG!"; ctx += type;
    ctx += "!"; ctx += tim;
    ctx += "!"; ctx += buf;
    ctx += "!";
#endif

	buffer[0] = '\0';
	len = sprintf(buffer, "LOG!%s!%s!%s!", type, tim, buf);
	if (len >= sizeof buffer) throw std::logic_error("buffer overflow, length >= 2K\n");

    COPYDATASTRUCT cds;
    cds.dwData = sizeof cds;
//    cds.lpData = (PVOID)ctx.c_str();
//    cds.cbData = (DWORD)ctx.length() + 1;
    cds.lpData = (PVOID)buffer;
    cds.cbData = (DWORD)len + 1;
    if (FALSE == SendMessage(LG_hwnd, WM_COPYDATA,
                             reinterpret_cast<WPARAM>(LG_hwnd),
                             reinterpret_cast<LPARAM>(&cds)))
        {LG_hwnd = NULL;}
    }
#endif

#if 1
// LOG
static HWND GPL_hwnd = NULL;
void GPL(char const* type, int x, int y, char const* format, ...)
    {

#if 0
    //////////////////////////////////////////////////////////////////////////
    //
    // LogviewerGPL
    if (GPL_hwnd == NULL)
        {GPL_hwnd = FindWindow(NULL, "kIng Of pIrAtEs lOgvIEwEr");}

    if (GPL_hwnd == NULL) return;

    // LOG
    char buf[1024] = {0};
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    char xy[20]; sprintf(xy, "xy=%d", x + y * GPL_CONSTANT);

    string ctx;
    ctx += "GPL!"; ctx += type;
    ctx += "!"; ctx += xy;
    ctx += "!"; ctx += buf;
    ctx += "!";

    COPYDATASTRUCT cds;
    cds.dwData = sizeof cds;
    cds.lpData = (PVOID)ctx.c_str();
    cds.cbData = (DWORD)ctx.length() + 1;
    if (FALSE == SendMessage(GPL_hwnd, WM_COPYDATA,
                             reinterpret_cast<WPARAM>(GPL_hwnd),
                             reinterpret_cast<LPARAM>(&cds)))
        {GPL_hwnd = NULL;}
#endif
    }
#endif


#if 0

// LG
void EnableAllLG(bool enable /* = true */)
{
    CLogMgr::Instance()->Enable(enable);
}

// LG
void EnableLG(char const* type, bool enable /* = true */)
{
    CLog* pLog = NULL;
    if (pLog = CLogMgr::Instance()->Add(type), pLog != NULL)
	{
	    pLog->Enable(enable);
	}	
}
#endif



// new interface
void LG_EnableAll(bool bEnable /* = true */)
{
    CLog2Mgr::Inst()->Enable(bEnable);
}
void LG_EnableMsgBox(bool bEnable /* = true */)
{
    CLog2Mgr::Inst()->EnableMsgBox(bEnable);
}
void LG_SetDir(char const* szDir)
{
    CLog2Mgr* pLogMgr = CLog2Mgr::Inst();
    
    pLogMgr->SetDirectory(szDir);
    pLogMgr->LogMgrReopen();
}
void LG_GetDir(std::string& strDir)
{
    CLog2Mgr* pLogMgr = CLog2Mgr::Inst();

	pLogMgr->GetDirectory(strDir);
}
void LG_CloseAll()
{
    CLog2Mgr::Inst()->CloseAll();
}
void LG_SetEraseMode(bool bEraseMode /* = true */)
{
    CLog2Mgr* pLogMgr = CLog2Mgr::Inst();

    pLogMgr->bEraseMode = bEraseMode;
}
void LG2(char const* type, char const* format, ...)
{
    // LOG
    CLog2Mgr* pLogMgr = CLog2Mgr::Inst();
    if (pLogMgr == NULL || !pLogMgr->IsEnable()) return;

    CLog2* pLog = pLogMgr->Add(type);
    if (pLog == NULL || !pLog->IsEnable()) return;

    char buf[8192] = {0};
    int len;
    va_list args;
    va_start(args, format);
    len = vsprintf(buf, format, args);
    va_end(args);
    if (len >= sizeof buf)
    {
        pLogMgr->Log("stack buffer overflow when LG [%s]\n", type);
        return;
    }

    pLog->Log(buf);
}

void LG3(char const* type, char const* format, ...)
{
	// LOG
	CLog2Mgr* pLogMgr = CLog2Mgr::Inst();
	if (pLogMgr == NULL || !pLogMgr->IsEnable()) return;

	CLog2* pLog = pLogMgr->Add(type);
	if (pLog == NULL || !pLog->IsEnable()) return;

	char buf[8192] = {0};
	int len;
	va_list args;
	va_start(args, format);
	len = vsprintf(buf, format, args);
	va_end(args);
	if (len >= sizeof buf)
	{
		pLogMgr->Log("stack buffer overflow when LG [%s]\n", type);
		return;
	}

	pLog->Log3(buf);
}
