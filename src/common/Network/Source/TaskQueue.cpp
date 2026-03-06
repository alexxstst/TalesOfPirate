//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================

#include "ThreadPool.h"
#include "Thread.h"

_DBC_USING
#ifdef USING_IOCP

PreAllocHeapPtr<TaskWait>	Task::__freetaskwait(0,20);
//==============================================================================
bool  Task::GetExitFlag()
{
	return __ThisThread?__ThisThread->m_pool->m_taskexitflag!=0:false;
}
//==============================================================================
HANDLE Task::GetHandle()
{
	return __ThisThread?__ThisThread->GetHandle():0;
}
//==============================================================================
DWORD Task::GetThreadID()
{
	return __ThisThread?__ThisThread->GetThreadID():0;
}
//==============================================================================
TaskQue::~TaskQue()
{
	m_mtxQue.lock();
	try
	{
		Task *l_task;
		while(m_head)
		{
			l_task	=m_head;
			m_head	=m_head ->__nexttask;

			l_task->__nexttask	=0;
			while(l_task =l_task->Lastly()){}
		}
		m_tail =0;
	}catch(...){}
	m_mtxQue.unlock();
};




























































#else
PreAllocHeapPtr<TaskWait>	Task::__freetaskwait(0,20);
//==============================================================================
bool  Task::GetExitFlag()
{
	return __ThisThread?__ThisThread->m_pool->m_taskexitflag!=0:false;
}
//==============================================================================
HANDLE Task::GetHandle()
{
	return __ThisThread?__ThisThread->GetHandle():0;
}
//==============================================================================
DWORD Task::GetThreadID()
{
	return __ThisThread?__ThisThread->GetThreadID():0;
}
//==============================================================================
long Task::WaitMe()
{
	TaskWait	*l_taskwait = {};
	bool		l_canwait	=false;
	long		l_retval	=NOTWAITSTATE;	//
	__mtxtaskwait.lock();
	try
	{
		if(__canwait)
		{
			l_canwait			=__canwait;
			l_taskwait			=__freetaskwait.Get();
			l_taskwait->next	=__taskwait;
			__taskwait			=l_taskwait;
		}
	}catch(...)
	{
		__mtxtaskwait.unlock();
		throw;
	}
	__mtxtaskwait.unlock();

	if(l_canwait)
	{
		l_taskwait->m_semWait.lock();
		l_retval	=l_taskwait->m_retval;
		l_taskwait->Free();
	}
	return l_retval;
}
//==============================================================================
TaskQue::~TaskQue()
{
	m_mtxQue.lock();
	try
	{
		Task *l_task;
		while(m_head)
		{
			l_task	=m_head;
			m_head	=m_head ->__nexttask;

			l_task->__nexttask	=0;
			while(l_task =l_task->Lastly()){}
		}
		m_tail =0;
	}catch(...){}
	m_mtxQue.unlock();
};
//==============================================================================
void TaskQue::AddTask(Task *req)
{
	if(!req)return;

	req->__mtxtaskwait.lock();
	try
	{
		req->__taskwait			=0;
		req->__canwait			=true;
	}catch(...){}
	req->__mtxtaskwait.unlock();

	m_semQueGet.lock();
	m_mtxQue.lock();
	try
	{
		if(!req->__TaskQue)
		{
			req->__TaskQue			=this;
			req->__nexttask			=0;
			if(m_tail)
			{
				m_tail->__nexttask	=req;
				m_tail				=req;
			}else
			{
				m_head				=req;
				m_tail				=req;
			}
			m_taskcount++;
			m_semQueAdd.unlock();
		}
	}catch(...){}
	m_mtxQue.unlock();
};
//==============================================================================
Task *TaskQue::GetTask(uLong l_howidle)
{
	Task * l_retval=0;

	if(m_semQueAdd.timelock(l_howidle) ==WAIT_OBJECT_0)
	{
		m_mtxQue.lock();
		try
		{
			if(m_head	==	m_tail)
			{
				l_retval	=m_head;
				m_head		=m_tail	=0;
			}else
			{
				l_retval	=m_head;
				m_head	=m_head->__nexttask;
			}
			if(l_retval)
			{
				l_retval->__TaskQue	=0;
				m_taskcount--;
				m_semQueGet.unlock();
			}
		}catch(...){}
		m_mtxQue.unlock();
	}
	return l_retval;
};
#endif
