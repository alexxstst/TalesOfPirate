#pragma once

#include <deque>
using std::deque;

class CSLock
{
public:
    CSLock()
    {
        InitializeCriticalSection(&m_cs);
    }

    ~CSLock()
    {
        DeleteCriticalSection(&m_cs);
    }


    void lock()
    {
        EnterCriticalSection(&m_cs);
    }


    void unlock()
    {
        LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION m_cs;
};

template <class T>
class MsgQueue
{
public:
    MsgQueue()
    {
    }
    
    virtual ~MsgQueue()
    {
    }
    
    virtual T Pop()
    {
        T ty = Top();
        m_lock.lock();
        m_queue.pop_front();
        m_lock.unlock();
        return ty;
    }
    virtual T Top()
    {
        m_lock.lock();
        T ty = (T)m_queue.front();
        m_lock.unlock();
        return ty;
    }
    virtual void Push(T val)
    {
        m_lock.lock();
        m_queue.push_back(val);
        m_lock.unlock();
    }
    virtual int Size()
    {
        return (int)m_queue.size();
    }
    virtual void Clear()
    {
        m_lock.lock();
        for(auto it = m_queue.begin(); it != m_queue.end(); it++)
        {
            if(*it != NULL)
				delete(*it);
        }
        m_queue.clear();
        m_lock.unlock();
    }
    bool IsEmpty()
    {
        return m_queue.empty();
    }

protected:
    deque<T>    m_queue;
    CSLock      m_lock;
};



