//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"

LW_BEGIN
	class lwEvent {
	public:
		HANDLE _handle;
		BOOL _state;

	public:
		lwEvent()
			: _handle(0), _state(0) {
		}

		~lwEvent() {
			Destroy();
		}

		LW_RESULT Create(LPSECURITY_ATTRIBUTES attributes, BOOL manual_flag, BOOL init_state, LPCTSTR name) {
			_handle = ::CreateEvent(attributes, manual_flag, init_state, name);
			if (_handle == NULL)
				return LW_RET_FAILED;

			_state = init_state;
			return LW_RET_OK;
		}

		LW_RESULT Destroy() {
			if (_handle) {
				if (::CloseHandle(_handle) == 0)
					return LW_RET_FAILED;
				_handle = 0;
				_state = 0;
			}
			return LW_RET_OK;
		}

		BOOL SetEvent() {
			_state = TRUE;
			return ::SetEvent(_handle);
		}

		BOOL ResetEvent() {
			_state = FALSE;
			return ::ResetEvent(_handle);
		}

		BOOL GetState() const {
			return _state;
		}
	};

	class lwSemaphore {
	public:
		HANDLE _handle;
		LONG _max_count;

	public:
		lwSemaphore()
			: _handle(0), _max_count(0) {
		}

		~lwSemaphore() {
			Destroy();
		}

		LW_RESULT Create(LPSECURITY_ATTRIBUTES attributes, LONG init_count, LONG max_count, LPCTSTR name) {
			_handle = ::CreateSemaphore(attributes, init_count, max_count, name);
			if (_handle == NULL)
				return LW_RET_FAILED;

			_max_count = max_count;
			return LW_RET_OK;
		}

		LW_RESULT Destroy() {
			if (_handle) {
				if (::CloseHandle(_handle) == 0)
					return LW_RET_FAILED;

				_handle = 0;
				_max_count = 0;
			}
			return LW_RET_OK;
		}

		BOOL ReleaseSemaphore(LONG release_count, LPLONG prev_count) {
			return ::ReleaseSemaphore(_handle, release_count, prev_count);
		}

		BOOL ReduceSemaphore(LONG count) {
			if (count > _max_count)
				return FALSE;

			while (count > 0) {
				::WaitForSingleObject(_handle, 0);
				count--;
			}
			return TRUE;
		}
	};

	class lwCriticalSection {
	public:
		CRITICAL_SECTION _handle;
		DWORD _state;

	public:
		lwCriticalSection()
			: _state(LW_INVALID_INDEX) {
		}

		~lwCriticalSection() {
			Destroy();
		}

		LW_RESULT Create() {
			::InitializeCriticalSection(&_handle);
			_state = 0;
			return LW_RET_OK;
		}

		LW_RESULT Destroy() {
			if (_state != LW_INVALID_INDEX) {
				::DeleteCriticalSection(&_handle);
				_state = LW_INVALID_INDEX;
			}
			return LW_RET_OK;
		}

		void Lock() {
			::EnterCriticalSection(&_handle);
			_state = 1;
		}

		void Unlock() {
			::LeaveCriticalSection(&_handle);
			_state = 0;
		}

		DWORD GetState() const {
			return _state;
		}
	};


LW_END
