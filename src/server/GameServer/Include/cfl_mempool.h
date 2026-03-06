#pragma once


#ifndef _CFL_MEMPOOL_H_
#define _CFL_MEMPOOL_H_

#include <vector>
#include <list>
#include "cfl_lock.h"

struct cfl_mpelem;
template <class T>
class cfl_mp
    {
    friend struct cfl_mpelem;

public:

    cfl_mp(int max_cnt) : _eplist(), _lock()
        {
        _eplist.clear();
        _free.clear();

        _cnt = max(1, max_cnt);
        _ep = new T[_cnt];

        for (int i = 0; i < _cnt; ++ i)
            {
            _ep[i]._pool = this;
            _free.push_back(&_ep[i]);}

        _eplist.push_back(_ep);}

    ~cfl_mp()
        {
        _free.clear();

		for (auto it = _eplist.begin(); it != _eplist.end(); ++it)
		{
			_ep = (*it);
			delete[] _ep;
		}
        
        _eplist.clear();
        _cnt = 0;}

    T* get()
        {
        T* tmp;

        _lock.lock();
        try {
            tmp = (_free.empty()) ? NULL : _free.front();
            if (tmp != NULL)
                _free.pop_front();
            else {
                try {
                    _ep = new T[_cnt];
                    for (int i = 0; i < _cnt; ++ i)
                        {
                        _ep[i]._pool = this;
                        _free.push_back(&_ep[i]);}

                    _eplist.push_back(_ep);

                    tmp = (_free.empty()) ? NULL : _free.front();
                    if (tmp != NULL) _free.pop_front();}
                catch (std::bad_alloc) {tmp = NULL;}
                catch (...) {tmp = NULL;}
                }
            }
        catch (...) {}
        _lock.unlock();

        if (tmp != NULL)
            {
            try {tmp->on_get();}
            catch (...) {}
            }

        return tmp;}
    
protected:

    void free(T* elem)
        {
        if (elem == NULL) return;

        try {elem->on_ret();}
        catch (...) {}

        _lock.lock();
        try {_free.push_back(elem);}
        catch (...) {}
        _lock.unlock();}

private:

    T* _ep;
    int _cnt;
    std::vector<T *> _eplist;

    std::list<T *> _free;
    cfl_spinlock _lock;};


struct cfl_mpelem
    {
    template <class T> friend class cfl_mp;

protected:

    cfl_mpelem() : _pool(NULL) {}
    virtual ~cfl_mpelem() {}

    virtual void on_get() {}
    virtual void on_ret() {}

public:

    void free() {((cfl_mp<cfl_mpelem> *)_pool)->free(this);}

protected:

    void* _pool;};

#endif // _CFL_MEMPOOL_H_
