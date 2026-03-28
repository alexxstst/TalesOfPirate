

#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>



unsigned int __stdcall ThreadFunc(void* param)
{
    return ((CThreadBase*)param)->Run();
}


CThreadBase::CThreadBase()
{
}


CThreadBase::~CThreadBase()
{
}


bool CThreadBase::Begin(int flag)
{
    // ������ʱ���ȹ���
    m_thread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &m_threadid);

	ToLogService("common", "{}:{}", m_threadid, "CThreadBase");

    if(m_thread)
    {
        // ָ�� CPU ���к��ٿ�ʼ����
        SetThreadAffinityMask(m_thread, 1);
        ResumeThread(m_thread);
    }

    return NULL != m_thread;
}


bool CThreadBase::Resume()
{
    return -1 != ResumeThread((HANDLE)m_thread);
}


bool CThreadBase::Suspend()
{
    return -1 != SuspendThread((HANDLE)m_thread);
}


bool CThreadBase::Terminate()
{
    return TRUE == TerminateThread((HANDLE)m_thread, 0);
}

