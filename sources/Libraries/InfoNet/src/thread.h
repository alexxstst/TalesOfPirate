
/* * * * * * * * * * * * * * * * * * * *

    
    Jampe
    2006/3/27

 * * * * * * * * * * * * * * * * * * * */


#if !defined __THREAD_H__
#define __THREAD_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    virtual bool Begin(int flag = 0);  //  
    virtual bool Resume();  //  
    virtual bool Suspend(); //  
    virtual bool Terminate();   //  
    virtual bool SetPriority(int priority);     //  
    virtual int GetPriority();  //  
    virtual int Wait(int time = INFINITE);  //  
    virtual unsigned Run() = 0;

public:
    HANDLE          m_thread;
    unsigned        m_threadid;
};


#endif
