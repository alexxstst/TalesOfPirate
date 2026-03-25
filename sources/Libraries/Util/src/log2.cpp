
#include "log2.h"

#include <iostream>

#include "util2.h"
#include <stdexcept>


CLog2::CLog2() : _fp(NULL), _fpLock(), _strName(""), _strExt("")
{
    _bEnable = true;
	_nType = 0;
	SetMaxSize(100 * 1024 * 1024);
}
CLog2::CLog2(char const* szName, char const* szExt /* = "log" */)
: _fp(NULL), _fpLock(), _strName(szName), _strExt(szExt)
{
    _bEnable = true;
	_nType = 0;
	SetMaxSize(100 * 1024 * 1024);

    if (!_Open()) throw std::logic_error("Open file failed\n");
}
CLog2::~CLog2()
{
    Close();
	_nType = 0;
}
bool CLog2::Open(char const* szName, char const* szExt)
{
    if (_fp != NULL) return false;

    _strName = szName;
    _strExt = szExt;
    return _Open();
}
void CLog2::SetMaxSize( DWORD dwSize )
{
	_dwMaxSize = dwSize;
}
bool CLog2::_Open()
{
    if (_fp != NULL) return false;

    std::string str = *CLog2Mgr::pstrLogDir;
    str += '/';
    str += _strName;
    if (_strExt.length() > 0)
    {
        str += ".";
        str += _strExt;
    }    

    _fpLock.Lock();
    do 
    {
        if (_fp != NULL)
            break;

        try
        {
            if (CLog2Mgr::bEraseMode)
            {
                _fp = fopen(str.c_str(), "wt");
            }
            else
            {
                _fp = fopen(str.c_str(), "at+");

                if (_fp == NULL)
                {
                    _fp = fopen(str.c_str(), "wt");
                }
            }
        }
        catch (...)
        {
        }
    }
    while (0);
    _fpLock.Unlock();

    return (_fp == NULL) ? false : true;
}

bool CLog2::_OpenX()
{
	if (_fp != NULL && _nType != 0) return false;

	std::string str = *CLog2Mgr::pstrLogDir;
	str += '/';
	str += _strName;
	if (_strExt.length() > 0)
	{
		str += ".";
		str += _strExt;
	}    

	_fpLock.Lock();
	do 
	{
		if (_fp != NULL)
		{
			Close();
		}

		try
		{
			_fp = fopen(str.c_str(), "rt+");
			if (_fp == NULL)
			{
				_fp = fopen(str.c_str(), "wt");
			}
			_nType = 1;
		}
		catch (...)
		{
		}
	}
	while (0);
	_fpLock.Unlock();

	return (_fp == NULL) ? false : true;
}

void CLog2::Close()
{
    if (_fp != NULL)
    {
        _fpLock.Lock();
        try
        {
            fclose(_fp);
            _fp = NULL;
			_nType = 0;
        }
        catch (...)
        {
        }
        _fpLock.Unlock();
    }
}
void CLog2::Log(char const* szFormat, ...)
{
    _Open();

    bool bMsgBox = false;
    if ((strlen(szFormat) > 3) && (strncmp(szFormat, "msg", 3) == 0))
    {
        bMsgBox = true;
    }

    char szBuf[LOGBUF_SIZE] = {0};
    va_list list;
    va_start(list, szFormat);
    if (bMsgBox)
    {
        vsprintf(szBuf, szFormat + 3, list);
    }
    else
    {
        vsprintf(szBuf, szFormat, list);
    }
    va_end(list);

    char buf[LOGBUF_SIZE] = {0};
    SYSTEMTIME st;
    char tim[100] = {0};
    GetLocalTime(&st);
    sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,
            st.wMinute, st.wSecond);

    if (bMsgBox)
    {
        sprintf(buf, "[%s][MSG]%s", tim, szBuf);
    }
    else
    {
        sprintf(buf, "[%s]%s", tim, szBuf);
    }

    // Log
    _fpLock.Lock();
    try
    {
    	std::cout << buf;
        fprintf(_fp, "%s", buf);
        fflush(_fp);
    }
    catch (...)
    {
    }
    _fpLock.Unlock();

    // MsgBox
#ifdef WIN32
    if (bMsgBox && CLog2Mgr::IsEnableMsgBox())
    {
        MessageBox(CLog2Mgr::GetWnd(), szBuf, "LOG", 0);
    }
#endif
}

void CLog2::Log3(char const* szFormat, ...)
{
	_OpenX();

	bool bMsgBox = false;
	if ((strlen(szFormat) > 3) && (strncmp(szFormat, "msg", 3) == 0))
	{
		bMsgBox = true;
	}

	char szBuf[LOGBUF_SIZE] = {0};
	va_list list;
	va_start(list, szFormat);
	if (bMsgBox)
	{
		vsprintf(szBuf, szFormat + 3, list);
	}
	else
	{
		vsprintf(szBuf, szFormat, list);
	}
	va_end(list);

	char buf[LOGBUF_SIZE] = {0};
	SYSTEMTIME st;
	char tim[100] = {0};
	GetLocalTime(&st);
	sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,
		st.wMinute, st.wSecond);

	if (bMsgBox)
	{
		sprintf(buf, "[%s][MSG]%s", tim, szBuf);
	}
	else
	{
		sprintf(buf, "[%s]%s", tim, szBuf);
	}

	// Log
	_fpLock.Lock();
	try
	{
		if( ftell(_fp) >= _dwMaxSize )
		{
			fseek(_fp, 0, SEEK_SET);
		}

		fprintf(_fp, "%s", buf);
		fflush(_fp);
	}
	catch (...)
	{
	}
	_fpLock.Unlock();

	// MsgBox
#ifdef WIN32
	if (bMsgBox && CLog2Mgr::IsEnableMsgBox())
	{
		MessageBox(CLog2Mgr::GetWnd(), szBuf, "LOG", 0);
	}
#endif
}



//////////////////////////////////////////////////////////////////////////
//
//  CLog2Mgr class
//
//
CLog2Mgr* CLog2Mgr::_pSelf = NULL;
bool CLog2Mgr::_bEnable;
bool CLog2Mgr::_bEnableMsgBox;
std::string* CLog2Mgr::pstrLogDir = NULL;
bool CLog2Mgr::bEraseMode = false;
#ifdef WIN32
HWND CLog2Mgr::_hWnd;
#endif

CLog2Mgr::CLog2Mgr() : _MapLock(), _LogMap()
{
    _setmaxstdio(2048);

    _bEnable = true;
    _bEnableMsgBox = true;
    pstrLogDir = new std::string("log");

#ifdef WIN32
    _hWnd = NULL;
#endif

    SetDirectory(pstrLogDir->c_str());
    _LogMgr.Open("LGMgr");
}
CLog2Mgr* CLog2Mgr::Inst()
{
    static bool bInit = false;

    if (!bInit)
    {
        static CThrdLock Lock;
        Lock.Lock();
        if (!bInit)
        {
            try
            {
                CLog2Mgr::_pSelf = new CLog2Mgr();
                bInit = true;
            }
            catch (std::bad_alloc&)
            {
#ifdef WIN32
                OutputDebugString("Failed to new CLog2Mgr\n");
#endif
            }
            catch (std::logic_error& le)
            {
#ifdef WIN32
                OutputDebugString(le.what());
#endif
                delete CLog2Mgr::_pSelf;
                CLog2Mgr::_pSelf = NULL;
            }
            catch (...)
            {
#ifdef WIN32
                OutputDebugString("Unknown exception raised from CLog2Mgr::Inst()\n");
#endif
                delete CLog2Mgr::_pSelf;
                CLog2Mgr::_pSelf = NULL;
            }
        }
        Lock.Unlock();
    }

    return CLog2Mgr::_pSelf;
}
CLog2* CLog2Mgr::Add(char const* szName, char const* szExt /* = "log" */)
{
    CLog2* pLog = NULL;
    _MapLock.Lock();
    try
    {
        std::map<std::string, CLog2*>::iterator it = _LogMap.find(szName);
        if (it != _LogMap.end())
        {
            pLog = (*it).second;
        }
        else
        {
            try
            {
                pLog = new CLog2(szName, szExt);
            }
            catch (std::bad_alloc&)
            {
                _LogMgr.Log("Failed to new CLog2([%s.%s])\n", szName, szExt);
            }
            catch (std::logic_error& le)
            {
                _LogMgr.Log(le.what());
                delete pLog;
                pLog = NULL;
            }
            catch (...)
            {
                _LogMgr.Log("Unknown exception raised from new [%s.%s]\n", szName, szExt);
                delete pLog;
                pLog = NULL;
            }

            if (pLog != NULL)
            {
                _LogMap[szName] = pLog;
                _LogMgr.Log("Add LG [%s]\n", szName);
            }
        }
    }
    catch (...)
    {
    }
    _MapLock.Unlock();

    return pLog;
}
bool CLog2Mgr::Remove(char const* szName)
{
    _LogMgr.Log("Remove LG [%s]\n", szName);

    bool ret;
    _MapLock.Lock();
    try
    {
        std::map<std::string, CLog2*>::iterator it = _LogMap.find(szName);
        if (it != _LogMap.end())
        {
            CLog2* pLog = (*it).second;
            _LogMap.erase(it);
            delete pLog;
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    catch (...)
    {
        _LogMgr.Log("Unknown excepton raised from CLog2Mgr::Remove([%s])\n", szName);
        ret = false;
    }
    _MapLock.Unlock();

    return ret;
}
void CLog2Mgr::RemoveAll()
{
    _LogMgr.Log("Remove All LG\n");

    CLog2* pLog = NULL;
    _MapLock.Lock();
    try
    {
        std::map<std::string, CLog2*>::iterator it = _LogMap.begin();
        while (it != _LogMap.end())
        {
            pLog = (*it).second;
            _LogMap.erase(it);
            delete pLog;

            ++ it;
        }
    }
    catch (...)
    {
        _LogMgr.Log("Unknown excepton raised from CLog2Mgr::RemoveAll()\n");
    }
    _MapLock.Unlock();
}
void CLog2Mgr::CloseAll()
{
    _LogMgr.Log("Close All LG\n");

    CLog2* pLog = NULL;
    _MapLock.Lock();
    try
    {
        DWORD dwBegin = ::GetTickCount();
        DWORD dwClose = 0;

        std::map<std::string, CLog2*>::iterator it = _LogMap.begin();
        while (it != _LogMap.end())
        {
            pLog = (*it).second;

            DWORD dwBegin = ::GetTickCount();
            pLog->Close();
            DWORD dwEnd = ::GetTickCount();
            dwClose += dwEnd - dwBegin;

            ++ it;
        }

        DWORD dwEnd = ::GetTickCount();
        _LogMgr.Log("CloseAll consume %ds\n", (dwEnd - dwBegin) / 1000);
        _LogMgr.Log("Close consume %ds\n", dwClose / 1000);
    }
    catch (...)
    {
        _LogMgr.Log("Unknown excepton raised from CLog2Mgr::CloseAll()\n");
    }
    _MapLock.Unlock();
}
void CLog2Mgr::SetDirectory(char const* szDir)
{
    *pstrLogDir = szDir;
    Util_MakeDir(szDir);
}
void CLog2Mgr::GetDirectory(std::string& strDir)
{
    strDir = *pstrLogDir;
}
void CLog2Mgr::Log(char const* szFormat, ...)
{
    char szBuf[LOGBUF_SIZE] = {0};
    
    va_list list;
    va_start(list, szFormat);
    vsprintf(szBuf, szFormat, list);
    va_end(list);

    _LogMgr.Log(szBuf);
}


//////////////////////////////////////////////////////////////////////////
//
//  CThrdLock class
//
//
CThrdLock::CThrdLock()
{
#ifdef WIN32
    InitializeCriticalSection(&_cs);
#endif
}
CThrdLock::~CThrdLock()
{
#ifdef WIN32
    DeleteCriticalSection(&_cs);
#endif
}
void CThrdLock::Lock()
{
#ifdef WIN32
    EnterCriticalSection(&_cs);
#endif
}
void CThrdLock::Unlock()
{
#ifdef WIN32
    LeaveCriticalSection(&_cs);
#endif
}
