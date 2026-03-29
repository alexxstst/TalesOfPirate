
/* * * * * * * * * * * * * * * * * * * *

    
    2006/3/27
    Jampe

 * * * * * * * * * * * * * * * * * * * */


#if !defined __LOCK_H__
#define __LOCK_H__


#define  WIN32_LEAN_AND_MEAN
#include <windows.h>


class Lock
{
public:
    Lock();
    virtual ~Lock();
    virtual void lock();
    virtual void unlock();

protected:
    CRITICAL_SECTION        m_cs;
};



#endif
