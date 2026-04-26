/* * * * * * * * * * * * * * * * * * * *

    
    Jampe
    2006/3/27

 * * * * * * * * * * * * * * * * * * * */


#if !defined __THREAD_H__
#define __THREAD_H__

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>


class CThreadBase {
public:
	CThreadBase();
	virtual ~CThreadBase();

	virtual bool Begin(int flag = 0);
	virtual bool Resume();
	virtual bool Suspend();
	virtual bool Terminate();
	virtual unsigned int Run() = 0;

public:
	HANDLE m_thread;
	unsigned int m_threadid;
};


#endif
