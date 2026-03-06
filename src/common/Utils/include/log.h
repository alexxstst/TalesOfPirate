#pragma once


#ifndef LOG_H_
#define LOG_H_

#include "pch.h"
#include "util2.h"


void   fLG(const char *pszName, const char *pszFormat, ...);

#define GPL_CONSTANT 2000

// LG
extern "C" void SetLGDir(char const* dir);

// LG
void GetLGDir(std::string& dir);

// LGMessageBox
extern "C" void EnableMsgBox(bool enable = true);

// LG
extern "C" void EnableAllLG(bool enable = true);

// LG
extern "C" void EnableLG(char const* type, bool enable = true);

// LOG
//void LG(char const* type, char const* format, ...);

// LOG
extern "C" void GPL(char const* type, int x, int y, char const* format, ...);

extern "C" int myprintf(char const* fmt, ...);

extern "C" int myfprintf(FILE* fp, char const* fmt, ...);

// new interface
void LG2(char const* type, char const* format, ...);
void LG3(char const* type, char const* format, ...);
void LG_EnableAll(bool bEnable = true);
void LG_EnableMsgBox(bool bEnable = true);
void LG_SetDir(char const* szDir);
void LG_GetDir(std::string& strDir);
void LG_CloseAll();
void LG_SetEraseMode(bool bEraseMode = true);
#define LG LG2
#define GameServerLG LG3

typedef void (CALLBACK* LOG_PROC)(const char *pszType, const char *pszContent);



#define LOG_BUF_SIZE 8192

class   CLog
{
private:
    bool        _bEnable;
    bool        _bConsole;
    bool        _bFile;
    FILE*       _fp;
    void        _Init();
    bool        _bValid;
public:
    char        m_szName[255];
    
    CLog();
    CLog(const char *pszName, const char *pszExt = "log");
    bool    Open(const char *pszName, const char *pszExt = "log");
    void    Enable(bool bEnable);
    bool    IsEnable();
    bool    IsConsole();
    bool    IsValid();
    FILE*   GetFile();

    void    L(const char *pszFormat, ...);
    ~CLog();
};

inline void CLog::Enable(bool bEnable)
{
    _bEnable = bEnable;
}

inline bool CLog::IsEnable()
{
    return _bEnable;
}

inline bool CLog::IsConsole()
{
    return _bConsole;
}

inline bool CLog::IsValid()
{
    return _bValid;
}

inline FILE* CLog::GetFile()
{
    return _fp;
}

class CLogMgr : public GAME::Singleton<CLogMgr>
{

public:

    CLogMgr();

    char    m_szDir[255];
    
    void    SetDirectory(const char *pszDir);
    void    GetDirectory(std::string& dir);

    void    EnableMsgBox(bool enable /* = true */);
    CLog*   Add(const char *pszName, const char *pszExt = "log");
    CLog*   Get(const char *pszName);
    bool    Remove(const char *pszName);
    void    ClearAll();
    void    Enable(bool bEnable);
    bool    IsEnable();

    void        Log(FILE *fp, const char *pszLog, bool bMsg, bool bConsole); 
    void        SetMessageWnd(HWND hWnd) { _hWnd = hWnd; }
    void        SetCallBack(LOG_PROC pfn);
    void        RunCallBack(const char *pszType, const char *pszContent);

private:
    
    std::map<std::string, CLog*>  _LogIdx;
    bool                _bEnable;
    bool                _bEnableMsgBox;
    HWND                _hWnd;
    LOG_PROC            _pfnProc;
};

inline void CLogMgr::Enable(bool bEnable)
{
    _bEnable = bEnable;
}

inline bool CLogMgr::IsEnable()
{
    return _bEnable;
}

inline void CLogMgr::SetCallBack(LOG_PROC pfn)
{
    _pfnProc = pfn;
}

inline void CLogMgr::RunCallBack(const char *pszType, const char *pszContent)
{
    if(_pfnProc)
    {
        _pfnProc(pszType, pszContent);
    }
}


#endif

