#pragma once

#include "ThreadPool.h"

#include <mutex>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
//====================================================
class Timekeeper
{
public:
	Timekeeper(bool mode =false):m_mode(mode),m_ulTimeCount(0)	//mode=falseGetTickCount()mode=trueQueryPerformance[X]
	{
		if(m_mode)
		{
			if(dwFrequency==1)
			{
				HANDLE		l_hThrd		=GetCurrentThread();
				DWORD_PTR	l_oldmask	=SetThreadAffinityMask(l_hThrd,1);
				LARGE_INTEGER l_freq; QueryPerformanceFrequency(&l_freq); // current frequency
				SetThreadAffinityMask(l_hThrd,l_oldmask);

				dwFrequency = uLong(l_freq.QuadPart/(1000*1000));
			}
			m_liStartTime.QuadPart	=0;
		}
		else
		{
			m_ulStartTime			=0;
		}
	}


	void  Begin()
	{
		if(m_mode)
		{
			HANDLE		l_hThrd		=GetCurrentThread();
			DWORD_PTR	l_oldmask	=SetThreadAffinityMask(l_hThrd,1);
			QueryPerformanceCounter(&m_liStartTime);
			SetThreadAffinityMask(l_hThrd,l_oldmask);
		}
		else
		{
			m_ulStartTime	=GetTickCount();
		}
	}

	uLong End()
	{
		LARGE_INTEGER   liEndTime;
		if(m_mode)
		{
			HANDLE		l_hThrd		=GetCurrentThread();
			DWORD_PTR	l_oldmask	=SetThreadAffinityMask(l_hThrd,1);
			QueryPerformanceCounter(&liEndTime);	
			SetThreadAffinityMask(l_hThrd,l_oldmask);
			m_ulTimeCount = uLong((liEndTime.QuadPart - m_liStartTime.QuadPart)/dwFrequency);
		}
		else
		{
			m_ulTimeCount	=GetTickCount()-m_ulStartTime;
		}
		return m_ulTimeCount;
	}

	uLong GetTimeCount()
	{
		return m_ulTimeCount;
	}

private:
	static uLong dwFrequency;

	bool	const	m_mode;
	LARGE_INTEGER   m_liStartTime;
	uLong			m_ulStartTime;

	uLong           m_ulTimeCount;
};
//====================================================
class Timer
{
	friend class TimerMgr;
protected:
	Timer(uLong interval);
	virtual ~Timer(){}
private:
	bool operator()();
	virtual void Process(){}
	virtual void Free(){delete this;}

	uLong		m_starttick;
	uLong		m_lasttick;

	uLong		m_interval;
};
class TimerMgr :public Task
{
	friend class Timer;
public:
	TimerMgr();
	virtual ~TimerMgr();
	bool AddTimer(Timer *timer);
private:
	long Process();
	void Free();
	std::recursive_mutex					m_mtxtime;
	Timer* volatile	m_timer[100]{};
	uLong		volatile	m_timercount;
	uLong					m_starttick;
};


#pragma pack(pop)
_DBC_END
