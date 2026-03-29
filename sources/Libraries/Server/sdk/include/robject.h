//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#include "DBCCommon.h"
#include "excp.h"

#include <mutex>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

template<class T, bool is_empty_>
struct ConditionallyEmpty;

template<class T>
struct ConditionallyEmpty<T, true>
{
	using Type = T;
	static constexpr auto is_empty = true;
};

template<class T>
struct ConditionallyEmpty<T, false>
{
	using Type = T;
	static constexpr auto is_empty = false;

	Type value;

	constexpr Type const& operator*() const
	{
		return value;
	}
	constexpr Type& operator*()
	{
		return value;
	}
};

template<class T, bool is_empty_>
struct ConditionalLockGuard;

template<class T>
struct ConditionalLockGuard<T, true>
{
	constexpr ConditionalLockGuard(ConditionallyEmpty<T, true> const&) {}
};

template<class T>
struct ConditionalLockGuard<T, false>
{
	ConditionalLockGuard(ConditionallyEmpty<T, false>& mutex) : lock{*mutex}
	{}

	std::lock_guard<T> lock;
};

template<class T, bool is_empty_>
ConditionalLockGuard(ConditionallyEmpty<T, is_empty_>) -> ConditionalLockGuard<T, is_empty_>;

//=================================================================
//common ancestry class define
template<bool sync =false>
class robject								//
{
protected:
	robject()
	{
		InitRCount(1);
	}
	virtual ~robject() = default;

	virtual void Free(){delete this;};	//,.

public:
	uLong adopt()								//[],1
	{
		_lock();
		try{
			_m_RefCount++;
			uLong l_count =_m_RefCount;
			_unlock();
			return l_count;
		}catch(...){}
		_unlock();
		return 0;
	}
	uLong discard()							//[],1,0Free
	{
		_lock();
		try{
			if(!_m_RefCount)throw 0;
			_m_RefCount--;
			uLong l_count =_m_RefCount;
			_unlock();
			if (!l_count)
			{
				Free();
			}
			return l_count;
		}catch(...){}
		_unlock();
		return 0;
	};
	uLong GetRCount(){return _m_RefCount;};
protected:
	void InitRCount(uLong iniref){ _m_RefCount =iniref;}
private:
	uLong	volatile	_m_RefCount{};
	std::recursive_mutex _m_SyncMutex;
	void	_lock()	{if(sync){_m_SyncMutex.lock();}}
	void	_unlock(){if(sync){_m_SyncMutex.unlock();}}

};
//------------------------------------------------------------------------------------------------------------------
template<class T, bool sync = false>
class rptr
{
private:
	T* volatile	m_objectPointer{};
	ConditionallyEmpty<std::recursive_mutex, !sync> mutable m_mutex;

public:
	rptr() = default;
	rptr(rptr const& ptr) : m_objectPointer{ptr.m_objectPointer}
	{
		if (m_objectPointer)
		{
			m_objectPointer->adopt();
		}
	}
	rptr(T* const objptr) : m_objectPointer{objptr}
	{
		if (m_objectPointer)
		{
			m_objectPointer->adopt();
		}
	}
	~rptr()
	{
		auto const lock = ConditionalLockGuard{m_mutex};
		if (m_objectPointer)
		{
			m_objectPointer->discard();
		}
	}

	void lock() 
	{
		if constexpr (sync)
		{
			(*m_mutex).lock();
		}
	}
	void unlock() 
	{ 
		if constexpr (sync)
		{
			(*m_mutex).unlock();
		}
	}

	rptr &operator=(rptr const& ptr){
		auto const lock = ConditionalLockGuard{m_mutex};
		if (m_objectPointer)
		{
			m_objectPointer->discard();
		}
		m_objectPointer = ptr.m_objectPointer;
		if (m_objectPointer)
		{
			m_objectPointer->adopt();
		}
		return *this;
	}
	rptr &operator=(T* const objptr){
		auto const lock = ConditionalLockGuard{m_mutex};
		if (m_objectPointer)
		{
			m_objectPointer->discard();
		}
		m_objectPointer = objptr;
		if (m_objectPointer)
		{
			m_objectPointer->adopt();
		}
		return *this;
	}
	operator bool() const 
	{ 
		return m_objectPointer != nullptr;
	}
	T& operator* () const 
	{ 
		return *m_objectPointer;
	}
	T* operator->() const 
	{ 
		return m_objectPointer;
	}
	operator T* () const 
	{
		return m_objectPointer;
	}
};

#pragma pack(pop)
_DBC_END
#pragma once
