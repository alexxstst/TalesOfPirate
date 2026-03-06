#pragma once


#ifndef LOG2_H_
#define LOG2_H_

#include <string>
#include <map>

#ifdef WIN32
#include <windows.h>
#endif

class CThrdLock
{
public:
    CThrdLock();
    ~CThrdLock();

    void Lock();
    void Unlock();

private:
#ifdef WIN32
    CRITICAL_SECTION _cs;
#endif
};

#define LOGBUF_SIZE (8 * 1024)

class CLog2
{
public:
    CLog2();
    CLog2(char const* szName, char const* szExt = "log");
    ~CLog2();

    bool Open(char const* szName, char const* szExt = "log");
    void Close();
    bool ReOpen();

    void Enable(bool bEnable = true);
    bool IsEnable() const;
    void Log(char const* szFormat, ...);
	void Log3(char const* szFormat, ...);

	void SetMaxSize( DWORD dwSize );

protected:
    void _Init();
    bool _Open();
	bool _OpenX();

private:
    bool _bEnable;    

    std::string _strName;
    std::string _strExt;
	long		_dwMaxSize;
	int			_nType;

    FILE* _fp;
    CThrdLock _fpLock;
};
inline void CLog2::Enable(bool bEnable /* = true */)
{
    _bEnable = bEnable;
}
inline bool CLog2::IsEnable() const
{
    return _bEnable;
}
inline bool CLog2::ReOpen()
{
    Close();
    return _Open();
}

class CLog2Mgr
{
private:
    CLog2Mgr();
    ~CLog2Mgr() {}
    CLog2Mgr(CLog2Mgr const&) {}
    CLog2Mgr& operator=(CLog2Mgr const&) {}    

public:
    static CLog2Mgr* Inst();

    static void Enable(bool bEnable = true);
    static bool IsEnable();
    static void EnableMsgBox(bool bEnable = true);
    static bool IsEnableMsgBox();
    static std::string* pstrLogDir;
    static bool bEraseMode;

#ifdef WIN32
    static void SetWnd(HWND hWnd);
    static HWND GetWnd();
#endif

    CLog2* Add(char const* szName, char const* szExt = "log");
    bool Remove(char const* szName);
    void RemoveAll();
    void CloseAll();

    void SetDirectory(char const* szDir);
    void GetDirectory(std::string& strDir);

    void Log(char const* szFormat, ...);
    bool LogMgrReopen();

#if 0
    CLogMgr();

    char    m_szDir[255];


    CLog*   Get(const char *pszName);
    void    ClearAll();

    void        Log(FILE *fp, const char *pszLog, bool bMsg, bool bConsole); 
    void        SetCallBack(LOG_PROC pfn);
    void        RunCallBack(const char *pszType, const char *pszContent);
#endif

private:
    static CLog2Mgr* _pSelf;
    static bool _bEnable;
    static bool _bEnableMsgBox;
#ifdef WIN32
    static HWND _hWnd;
#endif

    std::map<std::string, CLog2*> _LogMap;
    CThrdLock _MapLock;

    CLog2 _LogMgr;
};
inline void CLog2Mgr::Enable(bool bEnable /* = true */)
{
    _bEnable = bEnable;
}
inline bool CLog2Mgr::IsEnable()
{
    return _bEnable;
}
inline void CLog2Mgr::EnableMsgBox(bool bEnable /* = true */)
{
    _bEnableMsgBox = bEnable;
}
inline bool CLog2Mgr::IsEnableMsgBox() 
{
    return _bEnableMsgBox;
}
inline bool CLog2Mgr::LogMgrReopen()
{
    return _LogMgr.ReOpen();
}

#ifdef WIN32
inline void CLog2Mgr::SetWnd(HWND hWnd)
{
    _hWnd = hWnd;
}
inline HWND CLog2Mgr::GetWnd()
{
    return _hWnd;
}
#endif

#endif // LOG2_H_
