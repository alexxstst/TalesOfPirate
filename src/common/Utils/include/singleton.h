#pragma once

/**
 *  @file singleton.h
 *
 *  singleton.h, v 1.0 2004/9/21 13:14:05 shanghai China
 *
 *  @author claude Fan <fanyf2002@hotmail.com>
 *
 */

#if !defined _SINGLETON_H_
#define _SINGLETON_H_

#include <memory>
#include <iostream>
// nil-mutex :
//  nothing to do just return
class cfl_nil_mutex
    {
public:
    cfl_nil_mutex() {}
    virtual ~cfl_nil_mutex() {}

public:
    virtual bool lock();
    virtual bool unlock();

protected:
private:
    };

inline bool cfl_nil_mutex::lock() {return true;}
inline bool cfl_nil_mutex::unlock() {return true;}

// multi-thread mutex :
//  only one thread is allowed to access data or resource
//  at a time in a process
class cfl_mt_mutex : public cfl_nil_mutex
    {
public:
    cfl_mt_mutex();
    ~cfl_mt_mutex();

public:
    bool lock();
    bool unlock();

protected:
#if defined(WIN32)
    HANDLE _mtx;
#elif defined(FREEBSD)
    pthread_mutex_t _mtx;
#endif
    };

inline cfl_mt_mutex::cfl_mt_mutex()
    {
#if defined(WIN32)
    _mtx = CreateEvent(0, TRUE, TRUE, 0);
#elif defined(FREEBSD)
    pthread_mutex_init(&_mtx, NULL);
#endif
    }

inline cfl_mt_mutex::~cfl_mt_mutex()
    {
#if defined(WIN32)
    CloseHandle(_mtx);
#elif defined(FREEBSD)
    pthread_mutex_destroy(&_mtx);
#endif
    }

inline bool cfl_mt_mutex::lock()
    {
#if defined(WIN32)
    WaitForSingleObject(_mtx, INFINITE); return true;
#elif defined(FREEBSD)
    return (pthread_mutex_lock(&_mtx) == 0) ? true : false;
#endif
    }

inline bool cfl_mt_mutex::unlock()
    {
#if defined(WIN32)
    SetEvent(_mtx); return true;
#elif defined(FREEBSD)
    return (pthread_mutex_unlock(&_mtx) == 0) ? true : false;
#endif
    }


// original cfl singleton
template<class T, class CFL_LOCK = cfl_nil_mutex>
class cfl_singleton
{
protected:
	cfl_singleton() {}
	~cfl_singleton() {}
	cfl_singleton(cfl_singleton<T, CFL_LOCK> const&) {}
	cfl_singleton<T, CFL_LOCK>& operator =(cfl_singleton<T, CFL_LOCK> const&) {}
	static T* _instance;
public:
	static T* instance()
	{
		if (_instance == 0)
		{
			static CFL_LOCK locker;
			locker.lock();
			if (_instance == 0) _instance = new T;
			locker.unlock();
		}
		return _instance;
	}

	static void dump() { std::cout << "instance = " << std::hex << _instance << std::endl; }
	static void cleanup() { if (_instance) { delete _instance; _instance = 0; } }

};

template<class T, class CFL_LOCK>
T* cfl_singleton<T, CFL_LOCK> ::_instance = 0;

// cfl singleton with auto_ptr
template<class T, class CFL_LOCK = cfl_nil_mutex>
class cfl_singleton_ap
    {
protected:
    cfl_singleton_ap() {}
    ~cfl_singleton_ap() {}
    cfl_singleton_ap(cfl_singleton_ap<T, CFL_LOCK> const&) {}
    cfl_singleton_ap<T, CFL_LOCK>&  operator =(cfl_singleton_ap<T, CFL_LOCK> const&) {}

public:
    static T* instance()
        {
        if (!_instance)
            {
            static CFL_LOCK locker;
            locker.lock();
            if (!_instance)
            {
                _instance = std::make_unique<T>();

            }
            locker.unlock();}
        return _instance.get();}
    static void dump() {std::cout << "instance = " << std::hex << _instance << std::endl;}
protected:
	inline static std::unique_ptr<T> _instance = nullptr;
    };

// singleton
// singleton template
#if 1
template <class T>
class CSingleton
    {
protected:
    CSingleton() {}
virtual ~CSingleton() {}

private:
    CSingleton(const CSingleton<T>&) {}
    CSingleton<T>& operator =(const CSingleton<T>&) {}

public:
    static T* Instance();

private:
    inline static std::unique_ptr<T> m_instance = nullptr;
    };


template <class T>
inline T* CSingleton<T>::Instance()
    {
    if (!m_instance )
        {
        m_instance = std::make_unique<T>();
        }
    return m_instance.get();
    }
#endif

#endif // _SINGLETON_H_

