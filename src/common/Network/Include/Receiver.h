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
#include "ThreadPool.h"
#include "Comm.h"

_DBC_BEGIN

//==============================================================================
class OnProcessData:public PreAllocTask
{
public:
	OnProcessData(uLong size) :PreAllocTask(size) {};
	long			Process();
	Task		*	Lastly(){return	PreAllocTask::Lastly();}
	inline void		Init(DataSocket * datasock,RPacket &rpk);

	virtual void Finally()
	{
		m_rpk = nullptr;
	}

private:
	RPacket					m_rpk{ nullptr };
	TcpCommApp* volatile	__tca{};
	DataSocket* volatile	m_datasock{};
};
//==============================================================================
class Receiver:public Task
{
public:
	Receiver(DataSocket * datasock);
	void Initially();
	void Finally();
	void Init();
private:	//override
	long					Process();
	Task		*			Lastly(){return 0;}
private:	//data
	uLong					HasSpace() const;
	static PreAllocHeapPtr<OnProcessData> m_HeapProcData;

	uLong			volatile		m_p{};
	RPacket							m_rpk;

	cuLong			volatile		_len_inc;
	DataSocket	*	const volatile	m_datasock;
	TcpCommApp* volatile	__tca;
};
//================================================================================


_DBC_END
