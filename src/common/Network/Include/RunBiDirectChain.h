/*
Created By DBZHANG IN 8.25.2004
For RunTime Thread
*/

#pragma once
#include "DBCCommon.h"

#include <mutex>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

template<class T>
class RunBiDirectChain;

//============================================================================================
template<class T>
class RunBiDirectItem
{
	template<class T>friend class RunBiDirectChain;
	template<class T>friend class RunCtrlMgr;
protected:
	RunBiDirectItem():m_chain(0),m_last(0),m_next(0){}
	virtual ~RunBiDirectItem()	{_EndRun();}
	virtual void Initially()	{m_chain	=0;m_last	=0;m_next	=0;}
	virtual void Finally()		{_EndRun();}
	virtual	void Free()			{delete this;}//deleteoverride

	long	_BeginRun(RunBiDirectChain<T> *chain)
	{
		return	chain ? chain->AddItem(static_cast<T*>(this)) : 0;
	}
	long	_EndRun()
	{
		RunBiDirectChain<T>	*l_chain =m_chain;
		return l_chain?l_chain->DelItem(static_cast<T*>(this)):0;
	}
	RunBiDirectChain<T> *	GetChain(){return m_chain;}
private:
	virtual void	_OnBeginRun() {};
	virtual void	_OnEndRun() {};
	RunBiDirectChain<T>* volatile	m_chain{  };
	T* volatile	m_last{  };
	T* volatile	m_next{  };
};
//============================================================================================
template<class T>
class RunBiDirectChain
{
	template<class T>friend class RunBiDirectItem;
	template<class T>friend class RunChainGetArmor;
public:
	RunBiDirectChain() : m_total(0), m_head(0), m_tail(0), m_getp(0)
	{
	}
	virtual	~RunBiDirectChain()
	{
		auto const l_lockGet = std::lock_guard{m_mtxget};
		auto const l_lockChain = std::lock_guard{m_mtxchain};
		for(T*l_item =m_head;l_item;l_item =m_head)
		{
			DelItem(l_item);
			static_cast<RunBiDirectItem<T>*>(l_item)->Free();
		}
	}
	virtual void Initially()	{m_total =0;m_head =0;m_tail =0;m_getp =0;}
	virtual void Finally()
	{
		auto const l_lockGet = std::lock_guard{m_mtxget};
		auto const l_lockChain = std::lock_guard{m_mtxchain};
		for(T*l_item =m_head;l_item;l_item =m_head)
		{
			DelItem(l_item);
			static_cast<RunBiDirectItem<T>*>(l_item)->Free();
		}
		m_tail	=0;
	}

	long GetTotal()const
	{
		long l_retval	=0;
		auto const l_lockChain = std::lock_guard{m_mtxchain};

		l_retval	=m_total;

		return l_retval;
	}
	T* GetNextItem()const
	{
		auto const l_lockChain = std::lock_guard{m_mtxchain};
		if(m_getp<1)return 0;
		T	*l_t =0;
		m_getstack[m_getp-1].m_bidir	=true;
		if(m_getstack[m_getp-1].m_get)
		{
			l_t	=m_getstack[m_getp-1].m_get	=static_cast<RunBiDirectItem<T>*>(m_getstack[m_getp-1].m_get)->m_next;
		}
		else
		{
			l_t	=m_getstack[m_getp-1].m_get	=m_head;
		}
		return l_t;
	}
	T* GetLastItem()const
	{
		auto const l_lockChain = std::lock_guard{ m_mtxchain };
		if (m_getp < 1)return 0;
		T* l_t = 0;
		m_getstack[m_getp - 1].m_bidir = false;
		if (m_getstack[m_getp - 1].m_get)
		{
			l_t = m_getstack[m_getp - 1].m_get = static_cast<RunBiDirectItem<T>*>(m_getstack[m_getp - 1].m_get)->m_last;
		}
		else
		{
			l_t = m_getstack[m_getp - 1].m_get = m_tail;
		}
		//l_lockChain.unlock();
		return l_t;
	}
	T* GetFirstItem()const { auto const l = std::lock_guard{ m_mtxchain }; return m_head; }
private:
	long AddItem(T *item)
	{
		long l_lret	=0;

		auto const l_lockChain = std::lock_guard{m_mtxchain};
		if(item &&!static_cast<RunBiDirectItem<T>*>(item)->m_chain)
		{
			try{
				static_cast<RunBiDirectItem<T>*>(item)->_OnBeginRun();
			}catch(...)
			{
			}
			if(!m_head)
			{
				m_head	=m_tail	=item;
				static_cast<RunBiDirectItem<T>*>(m_head)->m_last
					=static_cast<RunBiDirectItem<T>*>(m_head)->m_next
					=0;
			}else
			{
				static_cast<RunBiDirectItem<T>*>(m_tail)	->m_next	=item;
				static_cast<RunBiDirectItem<T>*>(item)		->m_last	=m_tail;
				m_tail			=item;
				static_cast<RunBiDirectItem<T>*>(m_tail)	->m_next	=0;
			}
			static_cast<RunBiDirectItem<T>*>(item)->m_chain	=this;
			++m_total;
			l_lret	=	m_total;
		}
		return l_lret;
	}
	long DelItem(T *item)
	{
		long l_lret	=0;

		auto const l_lockChain = std::lock_guard{m_mtxchain};
		if(item &&(static_cast<RunBiDirectItem<T>*>(item)->m_chain	==this))
		{
			l_lret	=	m_total;
			--m_total;
			static_cast<RunBiDirectItem<T>*>(item)->m_chain	=0;
			if(static_cast<RunBiDirectItem<T>*>(item)->m_last)
			{
				static_cast<RunBiDirectItem<T>*>(static_cast<RunBiDirectItem<T>*>(item)->m_last)->m_next
					=static_cast<RunBiDirectItem<T>*>(item)->m_next;
			}
			if(static_cast<RunBiDirectItem<T>*>(item)->m_next)
			{
				static_cast<RunBiDirectItem<T>*>(static_cast<RunBiDirectItem<T>*>(item)->m_next)->m_last
					=static_cast<RunBiDirectItem<T>*>(item)->m_last;
			}
			if(m_head	==item)m_head	=static_cast<RunBiDirectItem<T>*>(item)->m_next;
			if(m_tail	==item)m_tail	=static_cast<RunBiDirectItem<T>*>(item)->m_last;
			for(uLong i=0;i<m_getp;i++)
			{
				if(m_getstack[i].m_get	==item)
				{
					m_getstack[i].m_get	=m_getstack[i].m_bidir?static_cast<RunBiDirectItem<T>*>(item)->m_last:static_cast<RunBiDirectItem<T>*>(item)->m_next;
				}
			}
			static_cast<RunBiDirectItem<T>*>(item)->m_last =static_cast<RunBiDirectItem<T>*>(item)->m_next =0;
			try{
				static_cast<RunBiDirectItem<T>*>(item)->_OnEndRun();
			}catch(...)
			{
			}
		}
		return l_lret;
	}

	mutable std::recursive_mutex m_mtxchain; 
	std::recursive_mutex m_mtxget;
	long		 volatile		m_total;
	T			*volatile		m_head;
	T			*volatile		m_tail;
	uLong	mutable	 volatile	m_getp;
	struct
	{
		mutable bool volatile m_bidir{ false };
		mutable T* volatile m_get{ nullptr };
	} m_getstack[20];
};
template<class T>
class RunChainGetArmor
{
public:
	RunChainGetArmor(RunBiDirectChain<T> &chain):m_chain(chain),m_locknum(0){lock();}
	~RunChainGetArmor()
	{
		while(m_locknum)
		{
			unlock();
		}
	}
	void lock()
	{
		m_chain.m_mtxget.lock();
		if(!m_locknum)
		{
			auto const l_lockChain = std::lock_guard{m_chain.m_mtxchain};
			m_chain.m_getstack[m_chain.m_getp].m_get	=0;
			++m_chain.m_getp;
		}
		++m_locknum;
	}
	void unlock()
	{
		if(!m_locknum)return;
		--m_locknum;
		if(!m_locknum)
		{
			auto const l_lockChain = std::lock_guard{m_chain.m_mtxchain};
			--m_chain.m_getp;
		}
		m_chain.m_mtxget.unlock();
	}
private:
	uLong m_locknum;
	RunBiDirectChain<T>& m_chain;
};

#pragma pack(pop)
_DBC_END

