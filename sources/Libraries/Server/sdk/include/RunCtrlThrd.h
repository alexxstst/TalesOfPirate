/*
Created By DBZHANG IN 8.25.2004
For Control Thread
Usage:
	reference by class MovObj;
*/
#pragma once
#include "DBCCommon.h"
#include "Task.h"
#include "RunBiDirectChain.h"
#include "Timer.h"

#include <mutex>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//============================================================================================
template<class T>
class RunCtrlObj	:public	RunBiDirectItem< RunCtrlObj<T> >
{//	enum{CtrlThrdMax=100,RunObjMax=500,TimePrecision=400};
	template<class T>friend class RunCtrlThrd;
protected:
	RunCtrlObj()
	{
	}
	virtual ~RunCtrlObj() = default;
	Task	* _BeginRun(RunCtrlMgr<T> *mgr)
	{
		RunCtrlThrd<T>	*l_thrd	=0;
		bool	l_bret	=false;
		m_mtxrun.lock();
		try{
			l_bret =mgr->Add(this,l_thrd);
		}catch(...)
		{}
		m_mtxrun.unlock();
		return	l_thrd;
	}
	void lock()		{m_mtxrun.lock();}
	void unlock()	{m_mtxrun.unlock();}
private:
	virtual void	_Run()=0;					//override

	std::recursive_mutex			m_mtxrun;
};
//============================================================================================
template<class T>
class RunCtrlThrd	:private RunBiDirectChain< RunCtrlObj<T> >	,public	Task
{
	template<class T>friend class RunCtrlObj;
	template<class T>friend class RunCtrlMgr;
private:
	RunCtrlThrd(RunCtrlMgr<T> * mgr,short thrdpos):m_mgr(mgr),m_thrdpos(thrdpos),m_timer(false)//QueuePerformanceX
	{
	}
	virtual	~RunCtrlThrd() = default;
	Task	*	Lastly()			//mgr
	{
		return 0;
	}
	long Process()
	{
		RunCtrlObj<T>	*l_obj;
		while(!GetExitFlag()){try{
			m_timer.Begin();
			RunChainGetArmor< RunCtrlObj<T> > l(*this);
			while(l_obj	=this->GetLastItem()){
				try{
					l_obj->_Run();
				}catch(...){}
			}
			l.unlock();

			uLong	l_tick	=m_timer.End();
			AddUsage(l_tick);
			if(T::TimePrecision>long(l_tick))::Sleep(T::TimePrecision-long(l_tick));	//CPU
		}catch(...){}}
		return 0;
	}
public:
	//for performance monitor:
	void AddUsage(uLong	usage)const
	{
		m_mtxcpu.lock();
		try{
			m_cpusage +=usage;
		}catch(...){}
		m_mtxcpu.unlock();
	}
	uLong GetUsage()const
	{
		uLong l_retval;
		m_mtxcpu.lock();
		try{
			l_retval	=m_cpusage;
			m_cpusage	=0;
		}catch(...){}
		m_mtxcpu.unlock();
		return l_retval;
	}
	bool	IsRuning()const
	{
		return (m_timer.End()>(T::TimePrecision*3/2))?false:true;
	}
	long GetTotal()const
	{
		return RunBiDirectChain< RunCtrlObj<T> >::GetTotal();
	}
	//end for performance monitor.
private:
	cShort			volatile			m_thrdpos;
	RunCtrlMgr<T>	*volatile	const	m_mgr;
	struct
	{
		mutable std::recursive_mutex m_mtxcpu;
		long volatile m_count;
		mutable uLong volatile m_cpusage;
		mutable Timekeeper m_timer;
	};
};
//============================================================================================
template<class T>
class RunCtrlMgr
{
	template<class T>friend class RunCtrlObj;
public:
	RunCtrlMgr():m_CtrlThrdNum(0)
	{
		for(short i=0;i<T::CtrlThrdMax;i++)
		{
			m_CtrlThrd[i]=0;
		}
	}
	~RunCtrlMgr()
	{
		auto const lock = std::lock_guard{m_CtrlThrdMut};
		try{
			for(short i=0;i<m_CtrlThrdNum;i++){
				delete m_CtrlThrd[i];
				m_CtrlThrd[i]	=0;
			}
			m_CtrlThrdNum	=0;
		}catch(...){}
	}

//for performance monitor:
	short GetThrdNum()
	{
		return m_CtrlThrdNum;
	}
	const RunCtrlThrd<T>* const* GetCtrlThrd()
	{
		return m_CtrlThrd;
	}
//end for performance monitor.
private:
	bool Add(RunCtrlObj<T> * runobj ,RunCtrlThrd<T>	* &pThrd)
	{
		bool	l_bret	=false;
		pThrd	=0;
		if(runobj)
		{
			m_CtrlThrdMut.lock();
			try{
				long	l_min	=T::RunObjMax;
				short	l_pos	=0;
				short	i		=0;
				for(i=0;(i<m_CtrlThrdNum)&&m_CtrlThrd[i];i++)
				{
					long l_total	=m_CtrlThrd[i]->GetTotal();
					if( l_total<l_min)
					{
						l_min	=l_total;
						l_pos	=i;
					}
				}
				if(l_min==T::RunObjMax){
					if(i==T::CtrlThrdMax){
						m_CtrlThrdMut.unlock();
						return false;
					}
					pThrd	=new RunCtrlThrd<T>(this,i);
					if(l_bret=static_cast<RunBiDirectItem< RunCtrlObj<T> >*>(runobj)->_BeginRun(pThrd)?true:false)
					{
						m_CtrlThrd[i]	=pThrd;
						m_CtrlThrdNum	++;
					}else
					{
						delete pThrd;
						pThrd	=0;
					}
				}else
				{
					l_bret	=static_cast<RunBiDirectItem< RunCtrlObj<T> >*>(runobj)->_BeginRun(m_CtrlThrd[l_pos])?true:false;
				}
			}catch(...){}
			m_CtrlThrdMut.unlock();
		}
		return	l_bret;
	}

private:
	std::recursive_mutex						m_CtrlThrdMut;
	short			volatile	m_CtrlThrdNum;
	RunCtrlThrd<T>	*volatile	m_CtrlThrd[T::CtrlThrdMax];
};
//============================================================================================

#pragma pack(pop)
_DBC_END
