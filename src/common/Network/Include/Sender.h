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
#include "Packet.h"

#include <mutex>

_DBC_BEGIN
class DataSocket;
class SNDPacket :public PreAllocStru, private rptr<rbuf> {
	friend class Sender;
public:
	SNDPacket(uLong size) :PreAllocStru(size), m_haspace(false) {}
	void Initially() { m_pkts = 0; m_cpos = 0; m_next = 0; }
	void Finally() { rptr<rbuf>::operator =(0); m_haspace = false; }
	void Init(DataSocket* datasock, uLong size);
	void Init(DataSocket* datasock, rbuf* buf);

	inline SNDPacket& operator<<(WPacket& wpk);

	operator bool()const { return rptr<rbuf>::operator bool(); }
	uLong		SendSize()const { return	(*this) ? (*this)->Size() : 0; }
	uLong		HasSpace()const { return (m_haspace && bool(*this) && (*this)->Size() > m_cpos) ? (*this)->Size() - m_cpos : 0; }
	char* GetBufAddr()const { return (*this)->getbuf(); }
private:
	uLong		volatile	m_pkts{};
	uLong		volatile	m_cpos{};
	DataSocket* volatile	m_datasock{};
	SNDPacket* volatile	m_next{};
	bool		volatile	m_haspace{ false };
};

//===================================================================================
class Sender :public Task {
public:
	Sender(DataSocket* datasock);
	void Initially();
	void Finally();
	void Init();
	Sender& operator<<(WPacket& wpk);
	bool			PopSNDPacket(int instancy);
	bool			HasData() { return m_send || m_head; }
private://override
	long			Process();
	Task		*	Lastly(){return 0;}
private://data
	static PreAllocHeapPtr<SNDPacket> m_SNDPackets;
	Sema			m_semSndC;
	std::recursive_mutex			m_mtxarra;
	std::recursive_mutex			m_mtxcomb;
	std::recursive_mutex			m_mtxsend;
	SNDPacket* volatile	m_send;
	SNDPacket* volatile	m_head;
	SNDPacket* volatile	m_tail;

	uLong			volatile	m_p;
	DataSocket* volatile	m_datasock;
	TcpCommApp* volatile	__tca;
};
//================================================================================

_DBC_END
