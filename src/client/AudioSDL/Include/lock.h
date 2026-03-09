#pragma once


/* * * * * * * * * * * * * * * * * * * *

    
    2006/3/27
    Jampe

 * * * * * * * * * * * * * * * * * * * */


#if !defined __LOCK_H__
#define __LOCK_H__


#define  WIN32_LEAN_AND_MEAN
#include <windows.h>


class Lock {
public:
	Lock();
	~Lock();
	void lock();
	void unlock();

protected:
	CRITICAL_SECTION m_cs;
};


class AutoLockObj {
public:
	inline AutoLockObj(Lock* lock) {
		m_lock = lock;
		m_lock->lock();
	}

	inline ~AutoLockObj() {
		m_lock->unlock();
	}

private:
	Lock* m_lock;
};


#define AutoLock(l)         AutoLockObj __lock_obj(l)


#endif
