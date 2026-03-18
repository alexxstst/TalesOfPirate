
#include "Timer.h"

_DBC_USING

uLong Timekeeper::dwFrequency = 1;

Timer::Timer(uLong interval) : m_interval(interval ? interval : 1), m_starttick(0), m_lasttick(0)
{
}

bool Timer::operator()()
{
	uLong l_tick = GetTickCount();
	if ((l_tick - m_starttick) / m_interval > (m_lasttick - m_starttick) / m_interval)
	{
		m_lasttick = l_tick;
		return true;
	}
	return false;
}

TimerMgr::TimerMgr() : m_starttick(GetTickCount()), m_timercount(0)
{
}

TimerMgr::~TimerMgr()
{
	Stop();
}

bool TimerMgr::AddTimer(Timer* timer)
{
	if (!timer) return false;
	timer->m_starttick = m_starttick;
	timer->m_lasttick  = m_starttick;

	auto const lock = std::lock_guard{m_mtxtime};
	if (m_timercount >= 100) return false;
	m_timer[m_timercount] = timer;
	m_timercount++;
	return true;
}

void TimerMgr::Start()
{
	m_exitFlag = false;
	m_thread = std::thread(&TimerMgr::Run, this);
}

void TimerMgr::Stop()
{
	m_exitFlag = true;
	if (m_thread.joinable())
		m_thread.join();

	auto const lock = std::lock_guard{m_mtxtime};
	for (uLong i = 0; i < m_timercount; i++)
	{
		m_timer[i]->Free();
	}
	m_timercount = 0;
}

void TimerMgr::Run()
{
	while (!m_exitFlag)
	{
		{
			auto const lock = std::lock_guard{m_mtxtime};
			for (uLong i = 0; i < m_timercount; i++)
			{
				if ((*m_timer[i])())
				{
					m_timer[i]->Process();
				}
			}
		}
		Sleep(40);
	}
}
