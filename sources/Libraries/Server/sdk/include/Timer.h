// Timer.h    (standalone,    ThreadPool)
#pragma once

#include "DBCCommon.h"

#include <mutex>
#include <thread>
#include <atomic>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//   (GetTickCount  QueryPerformanceCounter)
class Timekeeper
{
public:
	Timekeeper(bool mode = false) : m_mode(mode), m_ulTimeCount(0)
	{
		if (m_mode)
		{
			if (dwFrequency == 1)
			{
				HANDLE    l_hThrd   = GetCurrentThread();
				DWORD_PTR l_oldmask = SetThreadAffinityMask(l_hThrd, 1);
				LARGE_INTEGER l_freq;
				QueryPerformanceFrequency(&l_freq);
				SetThreadAffinityMask(l_hThrd, l_oldmask);
				dwFrequency = uLong(l_freq.QuadPart / (1000 * 1000));
			}
			m_liStartTime.QuadPart = 0;
		}
		else
		{
			m_ulStartTime = 0;
		}
	}

	void Begin()
	{
		if (m_mode)
		{
			HANDLE    l_hThrd   = GetCurrentThread();
			DWORD_PTR l_oldmask = SetThreadAffinityMask(l_hThrd, 1);
			QueryPerformanceCounter(&m_liStartTime);
			SetThreadAffinityMask(l_hThrd, l_oldmask);
		}
		else
		{
			m_ulStartTime = GetTickCount();
		}
	}

	uLong End()
	{
		if (m_mode)
		{
			LARGE_INTEGER liEndTime;
			HANDLE    l_hThrd   = GetCurrentThread();
			DWORD_PTR l_oldmask = SetThreadAffinityMask(l_hThrd, 1);
			QueryPerformanceCounter(&liEndTime);
			SetThreadAffinityMask(l_hThrd, l_oldmask);
			m_ulTimeCount = uLong((liEndTime.QuadPart - m_liStartTime.QuadPart) / dwFrequency);
		}
		else
		{
			m_ulTimeCount = GetTickCount() - m_ulStartTime;
		}
		return m_ulTimeCount;
	}

	uLong GetTimeCount()
	{
		return m_ulTimeCount;
	}

private:
	static uLong  dwFrequency;
	bool const    m_mode;
	LARGE_INTEGER m_liStartTime;
	uLong         m_ulStartTime;
	uLong         m_ulTimeCount;
};

//       Process()
class Timer
{
	friend class TimerMgr;
protected:
	Timer(uLong interval);
	virtual ~Timer() {}
private:
	bool operator()();
	virtual void Process() {}
	virtual void Free() { delete this; }

	uLong m_starttick;
	uLong m_lasttick;
	uLong m_interval;
};

//         
class TimerMgr
{
	friend class Timer;
public:
	TimerMgr();
	~TimerMgr();

	bool AddTimer(Timer* timer);
	void Start();
	void Stop();

private:
	void Run();

	std::recursive_mutex m_mtxtime;
	Timer* volatile      m_timer[100]{};
	uLong volatile       m_timercount;
	uLong                m_starttick;

	std::thread          m_thread;
	std::atomic<bool>    m_exitFlag{false};
};

#pragma pack(pop)
_DBC_END
