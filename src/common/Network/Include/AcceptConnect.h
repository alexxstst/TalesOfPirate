#pragma once

#ifndef ACCEPTCONNECT_H
#define ACCEPTCONNECT_H

#include "DBCCommon.h"
#include "ThreadPool.h"

_DBC_BEGIN

class TcpCommApp;
class TcpServerApp;
class DataSocket;
//=====OnConnect==============================================================================
class OnConnect:public PreAllocTask{
public:
	OnConnect(uLong size):PreAllocTask(size){}
	inline void Init(DataSocket *datasock);
	virtual long Process();
	virtual Task*Lastly();
private:
	DataSocket* m_datasock{};
	TcpCommApp* __tca{};
};

//=====AcceptConnect==============================================================================
class AcceptConnect:public PreAllocTask{
public:
	AcceptConnect(uLong size):PreAllocTask(size)
	{}
	void Init(TcpServerApp * tsa){__tsa	=tsa;};
	static PreAllocHeapPtr<AcceptConnect>	m_acceptheap;
private:
	virtual long Process();
	virtual Task*Lastly();

	static PreAllocHeapPtr<OnConnect>		m_connectheap;
	TcpServerApp* volatile __tsa{};
};
//=====DelConnect==============================================================================
class DelConnect:public PreAllocTask
{
public:
	DelConnect(uLong size):PreAllocTask(size){}
	void Init(DataSocket *datasock,int reason)
	{
		m_datasock	=datasock;
		m_reason	=reason;
		__tca	=m_datasock->GetTcpApp();
	};

	virtual long Process();
	virtual Task*Lastly();

	static PreAllocHeapPtr<DelConnect>	m_delHeap;

	int				m_reason{};
	DataSocket* m_datasock{};
	TcpCommApp* __tca{};
};


_DBC_END

#endif

