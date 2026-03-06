//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once
#include "ThreadPool.h"
#include "Comm.h"
#define SESSFLAG (0x80000000)

_DBC_BEGIN
//==============================================================================
class TGenSessID
{
	friend class RPCMGR;
	friend class RPCInfo;

	uLong	GetSessid(){
		uLong l_retval;
		m_mtxSessid.lock();
		try{
			l_retval	=m_nextsessid ++;
			m_nextsessid=(m_nextsessid >(SESSFLAG-2))?1:m_nextsessid;//,0-,1-
		}catch(...){}
		m_mtxSessid.unlock();
		return l_retval;
	};

	std::recursive_mutex				m_mtxSessid;
	uLong	volatile	m_nextsessid{1};
};
//==============================================================================
class TFNCall:public PreAllocStru
{
	friend class RPCMGR;
	friend class RPCInfo;
public:
	TFNCall(uLong size):PreAllocStru(size),m_retbuf(0),m_pointer(0),m_nextfnc(0),m_sessid(0)
	{
		m_semBlock.Create(0,1,0);
	}
	~TFNCall()
	{
		delete m_nextfnc;
	}
private:
	void Initially(){
		TimeBlock(0);	//
	}
	void Finally(){
		m_retbuf	=0;
		m_pointer	=0;
		m_nextfnc	=0; 

		m_sessid	=0;
	}
	DWORD TimeBlock(DWORD time)
	{
		return (m_semBlock.timelock(time)==WAIT_OBJECT_0)?0:-1;
	}
	int SendSignal(){return m_semBlock.unlock();}

	uLong		volatile	m_sessid;			//sessid
	uLong		volatile	m_oldsessid{};		//sessid
	bool		volatile	m_isSync{};

	RPacket					m_retbuf;			//
	void		*volatile	m_pointer;			//

	Sema					m_semBlock;
	std::recursive_mutex					m_mtx;

	TFNCall		*volatile	m_nextfnc;
};
//==============================================================================
class RPCInfo
{
public:
	friend class DataSocket;
	friend class RPCMGR;
	RPCInfo():m_fncall(0)
	{
	}
	~RPCInfo()
	{
		TFNCall	*l_fncall;
		while(l_fncall =m_fncall)
		{
			l_fncall->m_mtx.lock();
			try{
				if(l_fncall->m_sessid && DelFnCall(l_fncall))
				{
					if(l_fncall->m_isSync)
					{
						l_fncall->m_sessid	=0;
						l_fncall->SendSignal();
					}
					else
					{
						l_fncall->Free();
					}
				}
			}catch(...){}
			l_fncall->m_mtx.unlock();
		}
	}
/**/

private:
	uLong			AddFnCall(TFNCall	*fncall);
	bool			DelFnCall(TFNCall	*fncall);

	TGenSessID					m_gensessid;

	std::recursive_mutex						m_mtxfncall;		//
	TFNCall			*volatile	m_fncall;			//
};
//================================================================================
class OnServeCall:public PreAllocTask
{
public:
	OnServeCall(uLong size):PreAllocTask(size){};
	long			Process();
	Task		*	Lastly(){return	PreAllocTask::Lastly();}
	inline void		Init(DataSocket * datasock,uLong sessid,RPacket &rpk);

	virtual void Finally() override
	{
		__tca = nullptr;
		__rpc = nullptr;
		m_datasock = nullptr;
		m_sessid = 0;
		m_rpk = nullptr;
	}

private:
	TcpCommApp* volatile	__tca{};
	RPCMGR* volatile	__rpc{};
	DataSocket* volatile	m_datasock{};
	uLong					m_sessid{};
	RPacket					m_rpk{};
};
//================================================================================
class OnAsynReturn:public PreAllocTask
{
public:
	OnAsynReturn(uLong size) :PreAllocTask(size) {};
	long			Process();
	Task		*	Lastly(){return	PreAllocTask::Lastly();}
	inline void		Init(DataSocket * datasock,RPacket &rpk,void *pointer);

	virtual void Finally() override
	{
		__tca = nullptr;
		__rpc = nullptr;
		m_datasock = nullptr;
		m_rpk = nullptr;
		m_pointer = nullptr;
	}
private:
	TcpCommApp* volatile	__tca{};
	RPCMGR* volatile	__rpc{};
	DataSocket* volatile	m_datasock{};
	RPacket					m_rpk{ nullptr };
	void* volatile	m_pointer{};
};
_DBC_END	//RPCINFO_H

