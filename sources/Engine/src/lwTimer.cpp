//
#include "stdafx.h"
#include "lwTimer.h"

LW_BEGIN
	// lwTimer
	LW_STD_IMPLEMENTATION(lwTimer)

	LONGLONG lwTimer::__tick_freq = 0;

	lwTimer::lwTimer() {
		_timer_tickcount = 0;
		_timer_seq_size = TIMER_DEFAULT_SIZE;
		_timer_seq = LW_NEW(lwTimerInfo[_timer_seq_size]);
		memset(_timer_seq, 0, sizeof(lwTimerInfo) * _timer_seq_size);

		if (__tick_freq == 0) {
			LARGE_INTEGER timefreq;
			QueryPerformanceFrequency(&timefreq);
			__tick_freq = timefreq.QuadPart;
		}
	}

	lwTimer::~lwTimer() {
		LW_DELETE_A(_timer_seq);
	}

	LW_RESULT lwTimer::ReallocTimerSeq(DWORD size) {
		lwTimerInfo* new_seq = LW_NEW(lwTimerInfo[size]);
		memcpy(new_seq, _timer_seq, sizeof(lwTimerInfo) * (size > _timer_seq_size ? _timer_seq_size : size));
		LW_DELETE_A(_timer_seq);
		_timer_seq = new_seq;
		_timer_seq_size = size;

		return LW_RET_OK;
	}

	LW_RESULT lwTimer::SetTimerInterval(DWORD id, DWORD interval) {
		LW_RESULT ret = LW_RET_FAILED;

		if (id >= _timer_seq_size)
			goto __ret;
		{
			lwTimerInfo* info = &_timer_seq[id];

			if (info->proc == 0)
				goto __ret;

			info->interval = interval;
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimer::SetTimer(DWORD id, lwTimerProc proc, DWORD interval, DWORD hit_type, DWORD hit_add_cnt) {
		LW_RESULT ret = LW_RET_FAILED;

		if (proc == 0 || id >= _timer_seq_size)
			goto __ret;

		{
			lwTimerInfo* info = &_timer_seq[id];

			if (info->proc)
				goto __ret;

			info->proc = proc;
			info->interval = interval;
			info->hit_type = hit_type;
			info->hit_add_cnt = hit_add_cnt;
			info->pause = 0;

			info->last_time = _QueryTick();
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimer::ClearTimer(DWORD id) {
		LW_RESULT ret = LW_RET_FAILED;

		if (id == LW_INVALID_INDEX) {
			memset(_timer_seq, 0, sizeof(lwTimerInfo) * _timer_seq_size);
			_timer_seq_size = 0;
		}
		else {
			if (id >= _timer_seq_size)
				goto __ret;

			lwTimerInfo* info = &_timer_seq[id];

			if (info->proc == 0)
				goto __ret;

			info->proc = 0;
			info->interval = 0;
			info->last_time = 0;
			info->pause = 0;
			info->hit_type = 0;
			info->hit_add_cnt = 0;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimer::ResetTimer(DWORD id) {
		LW_RESULT ret = LW_RET_FAILED;

		if (id == LW_INVALID_INDEX) {
			for (DWORD i = 0; i < _timer_seq_size; i++) {
				if (_timer_seq[i].proc) {
					_timer_seq[i].last_time = _QueryTick();
				}
			}
		}
		else {
			if (id >= _timer_seq_size)
				goto __ret;

			lwTimerInfo* info = &_timer_seq[id];

			if (info->proc == 0)
				goto __ret;

			info->last_time = _QueryTick();
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimer::OnTimer() {
		_timer_tickcount = _QueryTick();

		lwTimerInfo* info;

		for (DWORD i = 0; i < _timer_seq_size; i++) {
			info = &_timer_seq[i];

			if (info->proc == 0 || info->pause == 1)
				continue;

			DWORD t = _timer_tickcount - info->last_time;

			if (t >= info->interval) {
				(*info->proc)(_timer_tickcount);

				if (info->hit_type == TIMER_HIT_FILTER) {
					info->last_time = _timer_tickcount;
				}
				else {
					info->last_time += info->interval;


					for (DWORD i = 0; (_timer_tickcount - info->last_time >= info->interval) && (i < info->hit_add_cnt);
						 i++) {
						(*info->proc)(_timer_tickcount);
						info->last_time += info->interval;
					}
				}
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwTimer::Pause(DWORD id, DWORD flag) {
		LW_RESULT ret = LW_RET_FAILED;

		if (id == LW_INVALID_INDEX) {
			for (DWORD i = 0; i < _timer_seq_size; i++) {
				if (_timer_seq[id].proc) {
					_timer_seq[id].pause = flag;
				}
			}
		}
		else {
			if (id >= _timer_seq_size)
				goto __ret;

			if (_timer_seq[id].proc == 0)
				goto __ret;

			_timer_seq[id].pause = flag;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	DWORD lwTimer::GetTickCount() {
		return _QueryTick();
	}

	// lwTimerThread
	DWORD WINAPI lwTimerThreadInfo::__thread_proc(void* param) {
		lwTimerThreadInfo* info = (lwTimerThreadInfo*)param;

		while (info->thread_state == lwTimerThreadInfo::STATE_RUN) {
			if (info->pause)
				goto __sleep_continue;
			{
				LARGE_INTEGER t;
				QueryPerformanceCounter(&t);

				DWORD cur_t = (DWORD)(t.QuadPart * 1000 / lwTimerThread::__tick_freq);

				if ((cur_t - info->last_time) >= info->interval) {
					info->last_time = cur_t;
					(*info->proc)(cur_t);
				}
			}
		__sleep_continue:
			::Sleep(1);
		}

		info->thread_state = lwTimerThreadInfo::STATE_INVALID;

		return 0;
	}

	LW_RESULT lwTimerThreadInfo::Init(lwTimerProc p, DWORD i) {
		LW_RESULT ret = LW_RET_FAILED;

		thread_handle = ::CreateThread(NULL, 0, __thread_proc, (void*)this, 0, &thread_id);
		if (thread_handle == 0)
			goto __ret;

		proc = p;
		interval = i;
		pause = 0;
		last_time = 0;
		thread_state = STATE_RUN;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimerThreadInfo::Term(DWORD delay) {
		thread_state = STATE_EXIT;

		const DWORD __term_lmt = 10;

		DWORD term_cntt = 0;
		while (thread_state != STATE_INVALID) {
			::Sleep(delay);

			if (++term_cntt == __term_lmt) {
				::TerminateThread(thread_handle, 0xffffffff);
			}
		}

		thread_handle = 0;
		thread_id = 0;
		thread_state = STATE_INVALID;

		return LW_RET_OK;
	}

	// lwTimerThread
	LW_STD_IMPLEMENTATION(lwTimerThread)

	lwTimerThread::lwTimerThread()
		: _timer_seq(0), _timer_seq_size(0) {
	}

	lwTimerThread::~lwTimerThread() {
		ClearTimer(LW_INVALID_INDEX, 1);
	}

	LW_RESULT lwTimerThread::AllocateTimerSeq(DWORD size) {
		_QueryFreq();

		lwTimerThreadInfo** new_seq = LW_NEW(lwTimerThreadInfo*[size]);
		memset(new_seq, 0, sizeof(lwTimerThreadInfo*) * size);
		memcpy(new_seq, _timer_seq, sizeof(lwTimerThreadInfo*) * (size > _timer_seq_size ? _timer_seq_size : size));
		LW_DELETE_A(_timer_seq);
		_timer_seq = new_seq;
		_timer_seq_size = size;

		return LW_RET_OK;
	}

	LW_RESULT lwTimerThread::SetTimer(DWORD id, lwTimerProc proc, DWORD interval) {
		LW_RESULT ret = LW_RET_FAILED;

		if ((id >= _timer_seq_size) || _timer_seq[id])
			goto __ret;
		{
			lwTimerThreadInfo* tti = LW_NEW(lwTimerThreadInfo);

			if (LW_RESULT r = tti->Init(proc, interval); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] tti->Init failed: id={}, interval={}, ret={}",
							 __FUNCTION__, id, interval, static_cast<long long>(r));
				goto __ret;
			}

			_timer_seq[id] = tti;
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimerThread::SetTimerInterval(DWORD id, DWORD interval) {
		LW_RESULT ret = LW_RET_FAILED;

		if ((id >= _timer_seq_size) || (_timer_seq[id] == 0))
			goto __ret;

		_timer_seq[id]->interval = interval;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwTimerThread::Pause(DWORD id, DWORD flag) {
		for (DWORD i = 0; i < _timer_seq_size; i++) {
			if (_timer_seq[i] == 0)
				continue;

			if (id == LW_INVALID_INDEX || id == i) {
				_timer_seq[i]->pause = flag;
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwTimerThread::ClearTimer(DWORD id, DWORD delay) {
		LW_RESULT ret = LW_RET_FAILED;

		for (DWORD i = 0; i < _timer_seq_size; i++) {
			if (_timer_seq[i] == 0)
				continue;

			if (id == LW_INVALID_INDEX || id == i) {
				if (LW_RESULT r = _timer_seq[i]->Term(delay); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _timer_seq[i]->Term failed: i={}, delay={}, ret={}",
								 __FUNCTION__, i, delay, static_cast<long long>(r));
					goto __ret;
				}

				LW_DELETE(_timer_seq[i]);
				_timer_seq[i] = 0;
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	// lwTimerPeriod
	LW_STD_IMPLEMENTATION(lwTimerPeriod)

LW_END
