//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwSyncObj.h"
#include "lwInterfaceExt.h"
#include <process.h>
#include <queue>
//////////////
//
LW_BEGIN


enum lwThreadTaskStateType
{
    THREADTASKSTATE_INVALID =   0,
    THREADTASKSTATE_WAITING =   1,
    THREADTASKSTATE_RUNNING =   2,
};

struct lwThreadTaskInfo
{
    lwThreadProc proc;
    void* param;
};

struct lwThreadInfo
{
    uintptr_t handle;
    unsigned int id;
    DWORD state;
};

class lwThreadPool : public lwIThreadPool
{
    LW_STD_DECLARATION();

    typedef std::deque<lwThreadTaskInfo> lwQueueTaskInfo;
    typedef lwQueueTaskInfo::iterator lwQueueTaskInfo_It;

private:
    lwQueueTaskInfo _queue_task;
    lwThreadTaskInfo* _task_seq;
    lwThreadInfo* _thread_seq;
    DWORD _thread_seq_size;
    DWORD _thread_num;
    int _priority;
    
    DWORD _task_seq_size;
    DWORD _exit_flag;
    HANDLE _event_signal;
    CRITICAL_SECTION _cs_lock;
    lwEvent _event_pool;

static unsigned int __stdcall __thread_proc(void* param);

private:
    unsigned int _ThreadProc();
    inline void _Lock() { ::EnterCriticalSection(&_cs_lock); }
    inline void _Unlock() { ::LeaveCriticalSection(&_cs_lock); }
    inline BOOL _SetEvent() { return ::SetEvent(_event_signal); }
    inline BOOL _ResetEvent() { return ::ResetEvent(_event_signal); }
    LW_RESULT _FindTask(lwQueueTaskInfo_It* it, lwThreadProc proc, void* param);
public:
    lwThreadPool();
    ~lwThreadPool();
 
    LW_RESULT Create(DWORD thread_seq_size, DWORD task_seq_size, DWORD suspend_flag);
    LW_RESULT Destroy();
    LW_RESULT RegisterTask(lwThreadProc proc, void* param);
    LW_RESULT RemoveTask(lwThreadProc proc, void* param);
    LW_RESULT FindTask(lwThreadProc proc, void* param);
    LW_RESULT SuspendThread();
    LW_RESULT ResumeThread();
    LW_RESULT SetPriority(int priority);
    LW_RESULT SetPoolEvent(BOOL lock_flag);
    int GetPriority() const { return _priority; }
    DWORD GetCurrentWaitingTaskNum() const { return (DWORD)_queue_task.size(); }
    DWORD GetCurrentRunningTaskNum() const;
    DWORD GetCurrentIdleThreadNum() const;
    DWORD GetThreadId(DWORD id) { return _thread_seq[id].id; }

};

LW_END
