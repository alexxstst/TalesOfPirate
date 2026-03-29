

#include "log.h"
#include <sys/timeb.h>
#include <time.h>
#include <direct.h>


#define LOG_FOLDER          "LOG"


LogMgr  g_lm;

Log::Log()
    :   m_file(0)
{
}


Log::~Log()
{
}

void Log::Open(const char* file)
{
__BEGIN_TRY
    m_lock.lock();
    if(m_file)
    {
        m_lock.unlock();
        return;
    }
    m_file = fopen(file, "at+");
    if(!m_file)
    {
        m_file = fopen(file, "wt");
    }
    m_lock.unlock();
__END_TRY
}


void Log::Close()
{
__BEGIN_TRY
    m_lock.lock();
    if(!m_file)
    {
        m_lock.unlock();
        return;
    }
    fclose(m_file);
    m_lock.unlock();
__END_TRY
}


void Log::WriteLog(const char* buf)
{
__BEGIN_TRY
    m_lock.lock();
    _timeb  tmb;
    tm*     ttm;
    _ftime(&tmb);
    ttm = localtime(&tmb.time);

    char tmfuf[64] = {0};
    sprintf(tmfuf, "%04i-%02i-%02i %02i:%02i:%02i.%03i ", 
        ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday, ttm->tm_hour, ttm->tm_min, ttm->tm_sec, tmb.millitm);

    fprintf(m_file, "%s %s", tmfuf, buf);
    fflush(m_file);
    m_lock.unlock();
__END_TRY
}



LogMgr::LogMgr()
{
    mkdir(LOG_FOLDER);                  //  log
    getcwd(m_path, 260);   //  
}


LogMgr::~LogMgr()
{
}


Log* LogMgr::GetFileHandle(const char* filename)
{
    Log* plog = 0;

__BEGIN_TRY
    m_lock.lock();
    LogList::iterator it = m_list.find(filename);
    if(it != m_list.end())
    {
        plog = (*it).second;
    }
    else
    {
        plog = new Log;
        char logpath[MAX_PATH] = {0};
        sprintf(logpath, "%s\\%s\\%s.log", m_path, LOG_FOLDER, filename);
        plog->Open(logpath);
        m_list[filename] = plog;
    }
    m_lock.unlock();
__END_TRY
    return plog;
}


void LogMgr::CloseLogFile(const char* filename)
{
    Log* plog = 0;

__BEGIN_TRY
    m_lock.lock();
    LogList::iterator it = m_list.find(filename);
    if(it != m_list.end())
    {
        plog = (*it).second;
        plog->Close();
        m_list.erase(it);
        SAFE_DELETE(plog);
    }
    m_lock.unlock();
__END_TRY
}


void LogMgr::CloseAllLogFile()
{
__BEGIN_TRY
    m_lock.lock();
    LogList::iterator it;
    for(it = m_list.begin(); it != m_list.end(); it++)
    {
        (*it).second->Close();
    }
    m_list.clear();
    m_lock.unlock();
__END_TRY
}


void LG(const char* filename, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LGFMT(filename, format, args);
    va_end(args);
}


void LGFMT(const char* filename, const char* format, va_list param)
{
    Log* plog = 0;
    plog = g_lm.GetFileHandle(filename);
    if(plog)
    {
        char buf[8192] = {0};
        _vsnprintf(buf, 8192, format, param);
        plog->WriteLog(buf);
    }
}


void LGPRT(const char* filename, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LGFMT(filename, format, args);
    vprintf(format, args);
    va_end(args);
}


void CloseAllLog()
{
    g_lm.CloseAllLogFile();
}
