//
#include "stdafx.h"
#include "lwThreadPool.h"

using namespace std;

LW_BEGIN


// lwThreadPool
LW_STD_IMPLEMENTATION(lwThreadPool)

unsigned int __stdcall lwThreadPool::__thread_proc(void* param)
{
    lwThreadPool* thread_pool = (lwThreadPool*)param;

    return thread_pool->_ThreadProc();

}

unsigned int lwThreadPool::_ThreadProc()
{
    DWORD this_thread = ::GetCurrentThreadId();
    lwThreadInfo* thread_info = 0;

    for(DWORD i = 0; i < _thread_seq_size; i++)
    {
        thread_info = &_thread_seq[i];
        if((unsigned int)this_thread == thread_info->id)
            break;
    }
    assert(thread_info);

    thread_info->state = THREADTASKSTATE_WAITING;

    while(1)
    {
        ::WaitForSingleObjectEx(_event_pool._handle, INFINITE, FALSE);

        int ret = ::WaitForSingleObjectEx(_event_signal, INFINITE, FALSE);
        assert(ret == WAIT_OBJECT_0);
        
        // check exit flag
        if(_exit_flag == 1)
        {

            thread_info->handle = 0;
            thread_info->id = 0;
            thread_info->state = THREADTASKSTATE_INVALID;

            _Lock();

            _thread_num -= 1;

            _Unlock();

            goto __ret;
        }

        size_t task_size = 0;
        lwThreadTaskInfo task_info;

        _Lock();

        task_size = _queue_task.size();
        if(task_size > 0)
        {
            task_info = _queue_task.front();
            _queue_task.pop_front();
        }

        _Unlock();

        // run registered task
        if(task_size == 0)
        {
            _ResetEvent();
        }
        else
        {
            thread_info->state = THREADTASKSTATE_RUNNING;

            (*task_info.proc)(task_info.param);

            thread_info->state = THREADTASKSTATE_WAITING;
        }
    }

__ret:
    return 0L;
}

lwThreadPool::lwThreadPool()
{
    _thread_seq = 0;
    _thread_seq_size = 0;
    _thread_num = 0;
    _task_seq_size = 0;
    _exit_flag = 0;
    _event_signal = 0;
    _priority = 0;
}
lwThreadPool::~lwThreadPool()
{
    Destroy();
}

LW_RESULT lwThreadPool::Create(DWORD thread_seq_size, DWORD task_seq_size, DWORD suspend_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_thread_num > 0)
        goto __ret;

    if(thread_seq_size == 0 || task_seq_size == 0)
        goto __ret;

    _thread_seq = LW_NEW(lwThreadInfo[thread_seq_size]);
    memset(_thread_seq, 0, sizeof(lwThreadInfo) * thread_seq_size);
    _thread_seq_size = thread_seq_size;

    _queue_task.clear();
    _task_seq_size = task_seq_size;

    _event_signal = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    if(_event_signal == NULL)
        goto __ret;

    if(LW_RESULT r = _event_pool.Create(NULL, TRUE, TRUE, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _event_pool.Create failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ::InitializeCriticalSection(&_cs_lock);

    _thread_num = 0;
    for(DWORD i = 0; i < thread_seq_size; i++)
    {
        _thread_seq[i].handle = ::_beginthreadex(NULL, 0, __thread_proc, (void*)this, suspend_flag, &_thread_seq[i].id);
        if(_thread_seq[i].handle == 0)
        {
            goto __ret;
        }

        _thread_num++;
    }

    _priority = THREAD_PRIORITY_NORMAL;

    ret = LW_RET_OK;
__ret:
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] Create failed (pre-Destroy cleanup): thread_seq_size={}, task_seq_size={}, suspend_flag={}, ret={}",
                     __FUNCTION__, thread_seq_size, task_seq_size, suspend_flag, static_cast<long long>(ret));
        Destroy();
    }

    return ret;
}
LW_RESULT lwThreadPool::Destroy()
{
    if(_thread_num > 0)
    {
        _exit_flag = 1;
        _SetEvent();

        while(_thread_num > 0)
        {
            ::Sleep(1);
        }

        _exit_flag = 0;
    }

    if(_event_signal)
    {
        ::CloseHandle(_event_signal);
        _event_signal = 0;
    }

    _event_pool.Destroy();

    ::DeleteCriticalSection(&_cs_lock);

    if(_thread_seq)
    {
        LW_DELETE_A(_thread_seq);
        _thread_seq = 0;
        _thread_seq_size = 0;
    }

    if(_task_seq_size)
    {
        _queue_task.clear();
        _task_seq_size = 0;
    }

    _priority = 0;

    return LW_RET_OK;
}
LW_RESULT lwThreadPool::RegisterTask(lwThreadProc proc, void* param)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(proc == 0)
        goto __ret;

    _Lock();

    if(_queue_task.size() == _task_seq_size)
    {
        _Unlock();
        goto __ret;
    }

    lwThreadTaskInfo info;
    info.proc = proc;
    info.param = param;

    _queue_task.push_back(info);

    _Unlock();

    if(_SetEvent() == 0)
        goto __ret;

    ret = LW_RET_OK;    

__ret:
    return ret;
}

LW_RESULT lwThreadPool::RemoveTask(lwThreadProc proc, void* param)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwQueueTaskInfo_It it;

    if(LW_RESULT r = _FindTask(&it, proc, param); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _FindTask failed: proc={}, param={}, ret={}",
                     __FUNCTION__, reinterpret_cast<const void*>(proc), param, static_cast<long long>(r));
        goto __ret;
    }

    _queue_task.erase(it);

    ret = LW_RET_OK;    

__ret:
    return ret;
}

LW_RESULT lwThreadPool::FindTask(lwThreadProc proc, void* param)
{
    return _FindTask(NULL, proc, param);
}

LW_RESULT lwThreadPool::_FindTask(lwQueueTaskInfo_It* it, lwThreadProc proc, void* param)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwQueueTaskInfo_It it_b;
    lwQueueTaskInfo_It it_e;

    if(_queue_task.size() == 0)
        goto __ret;

    _Lock();

    it_b = _queue_task.begin();
    it_e = _queue_task.end();

    for(; it_b != it_e; ++it_b)
    {
        if(((*it_b).proc == proc) && ((*it_b).param == param))
        {
            break;
        }
    }

    _Unlock();

    if(it_b == it_e)
        goto __ret;


    if(it)
    {
        *it = it_b;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwThreadPool::SuspendThread()
{
    return -1L;
}
LW_RESULT lwThreadPool::ResumeThread()
{
    return -1L;
}
LW_RESULT lwThreadPool::SetPriority(int priority)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_priority == priority)
        goto __ret_ok;

    for(DWORD i = 0; i < _thread_num; i++)
    {
        if(::SetThreadPriority((HANDLE)_thread_seq[i].handle, priority) == FALSE)
            goto __ret;
    }

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwThreadPool::SetPoolEvent(BOOL lock_flag)
{
    BOOL b;

    if(lock_flag)
    {
        b = _event_pool.ResetEvent();
    }
    else
    {
        b = _event_pool.SetEvent();
    }

    return b ? LW_RET_OK : LW_RET_FAILED;
}

DWORD lwThreadPool::GetCurrentRunningTaskNum() const
{
    DWORD n = 0;

    for(DWORD i = 0; i < _thread_num; i++)
    {
        if(_thread_seq[i].state == THREADTASKSTATE_RUNNING)
            n++;
    }

    return n;
}

DWORD lwThreadPool::GetCurrentIdleThreadNum() const
{
    return _thread_num - GetCurrentRunningTaskNum();
}

LW_END
