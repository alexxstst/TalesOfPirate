
#include "DataSocket.h"
#include "Sender.h"
#include "Packet.h"
#include "Comm.h"
#include <fstream>
#include <logutil.h>
_DBC_USING

extern PreAllocHeap<rbuf>	__bufheap;
PreAllocHeapPtr<SNDPacket> Sender::m_SNDPackets(0, 30);
//==============================================================================
class SendAll :public PreAllocTask {
public:
	SendAll(uLong size) :PreAllocTask(size) {}
	long		Process();
	Task* Lastly() { return PreAllocTask::Lastly(); }
	inline void Init(DataSocket* datasock);

	void Finally() override 
	{
		m_datasock = nullptr;
	}

private:
	DataSocket* m_datasock{ nullptr };
};
//--------------------------------------------------
inline void SendAll::Init(DataSocket* datasock)
{
	m_datasock = datasock;
}
long SendAll::Process()
{
	try {
		m_datasock->GetTcpApp()->OnSendAll(m_datasock);
	}
	catch (...) {}
	return 0;
}
static PreAllocHeapPtr<SendAll> s_SendAlls(0, 30);
//================================================================================
Sender::Sender(DataSocket* datasock)
	:m_datasock(datasock), __tca(0), m_p(0)
	, m_send(0), m_head(0), m_tail(0)
{
}
void Sender::Initially()
{
	m_p = 0;
}
void Sender::Finally()
{
	auto const l_lockSend = std::lock_guard{m_mtxsend};
	auto const l_lockArra = std::lock_guard{m_mtxarra};
	if (m_send)
	{
		m_send->Free();
	}
	while (m_head)
	{
		m_send = m_head;
		m_head = m_head->m_next;
		m_send->Free();
	}
	m_send = 0;
	m_tail = 0;
	m_semSndC.Destroy();
}
void Sender::Init()
{
	__tca = m_datasock->GetTcpApp();
	m_semSndC.Create(__tca->__maxsndque, __tca->__maxsndque, 0);
}
//================================================================================
void SNDPacket::Init(DataSocket* datasock, uLong size)
{
	rptr<rbuf>::operator =(__bufheap.Get(size));
	m_haspace = true;
	m_datasock = datasock;
}
void SNDPacket::Init(DataSocket* datasock, rbuf* buf)
{
	rptr<rbuf>::operator =(buf);
	m_haspace = true;
	m_datasock = datasock;
}
//--------------------------------------------------
inline SNDPacket& SNDPacket::operator<<(WPacket& wpk)
{
	// This function inserts a WPacket into a SNDPacket, which is encrypted and ready to be sent via Sender::Process().
	uLong l_len = wpk.GetPktLen() - wpk.m_cpos;
	// Reminder: m_cpos is the current offset from the beginning of rbuf. Example: it is equal to packet length when 1 packet is inserted.
	// Initially, m_cpos is equal to 0, so l_len in this case is simply the packet length based on data/head/cmdsize
	if (wpk.m_wpos < SIGN32 - em_cmdsize)
	{
		// I have no clue what this should be doing.
		l_len += (l_len + 9) / 10;
	}
	if (HasSpace() < l_len)
	{
		// HasSpace returns rbuf size minus m_cpos (Reminder: m_cpos defines what is currently already written to SNDPacket).
		m_haspace = false;
	}
	else if (l_len)
	{
		// We have space. l_len is now only the size of wpk data.
		l_len = wpk.GetDataLen();
		if (wpk.m_wpos < SIGN32 - em_cmdsize)
		{
			// Encrypt the packet here.
			try {
				bool bEnc = m_datasock->GetTcpApp()->OnEncrypt(m_datasock, GetBufAddr() + m_cpos + wpk.Head(), HasSpace() , wpk.GetDataAddr(), l_len);
				if (!bEnc)
				{
					m_haspace = false;
					return *this;
				}

				// GetBufAddr -> gets the start of rbuf.
				// m_cpos -> gets the "offset" from the rbuf.
				// wpk.Head() -> we want to move the encrypted data right after the header ends.
			}
			catch (...) {}
			wpk.m_wpos = l_len - em_cmdsize;
			// Update the wpk data size with the new size after encryption.. 
			wpk.WritePktLen();
			// Write new (post-encryption) packet length to the wpk header.
			MemCpy(GetBufAddr() + m_cpos, wpk.GetPktAddr(), wpk.Head());
			// Copy the header information from wpk to the head location of the current packet inside rbuf.
		}
		else
		{
			// I don't think this is ever used.
			wpk.WritePktLen();
			MemCpy(GetBufAddr() + m_cpos, wpk.GetPktAddr(), wpk.GetPktLen());
		}
		m_cpos += wpk.GetPktLen();
		// Update the offset from rbuf. It needs to be as large as the size of the packet that was copied there.
		wpk.m_cpos += wpk.GetPktLen();
		// Update the size of wpk
		if (wpk.m_cpos == wpk.GetPktLen())
		{
			// This is a weird check, should always work.
			wpk = 0;
			// After this is set to 0, break out of the while-loop inside Sender	&Sender::operator<<(WPacket &wpk).
			m_pkts++;
			// Increment the number of WPacket inside this SNDPacket.
		}
	}
	return *this;
}
//--------------------------------------------------
Sender& Sender::operator<<(WPacket& wpk)
{
	if (!bool(wpk))return *this;
	//wpk.WritePktLen();

	auto const l_lockSend = std::lock_guard{m_mtxsend};
	m_mtxcomb.lock();
	while (bool(wpk) && m_datasock->m_delflag == 0)
	{
		// Repeat while packet pointer exists and datasock is valid
		// Note: wpk is set to 0 after it is successfully inserted into the SNDPacket.
		// That means this while loop most likely only runs once. I can't see an occasion where wpk.m_cpos !=wpk.GetPktLen(), maybe
		// on encryption failure?
		if (m_tail && m_tail->HasSpace())
		{
			// Check if the m_tail SNDPacket pointer exists and has space. If it does, insert the WPacket into that SNDPacket.
			*m_tail << wpk;
			if (m_head->m_cpos > uLong(0.618 * m_head->SendSize()) && !m_send && !m_datasock->m_sendflag && !m_datasock->m_delflag)
			{
				// Check several things before proceding:
				// The first SNDPacket (m_head) offset must be bigger than 61.8% of rbuf. 
				// m_send (the current SNDPacket) must be null
				// m_sendflag and m_delflag must be null
				if (m_datasock->m_sendflag++ == 0)
				{
					// If m_sendflag is 0 after incrementing, add this task (Sender) to the ThreadPool
					m_datasock->GetTcpApp()->GetCommunicator()->AddTask(this);
				}
				else
				{
					// Decrement sendflag
					m_datasock->m_sendflag--;
				}
			}
		}
		else
		{
			// m_tail does not exist or doesnt have space. Let's check.
			m_mtxcomb.unlock();
			try {
				uLong	l_timelock = WAIT_TIMEOUT;
				bool l_flag = true;
				if ((l_timelock = m_semSndC.trylock()) != WAIT_OBJECT_0)
				{
					do {
						try { 
							l_flag = m_datasock->GetTcpApp()->OnSendBlock(m_datasock);
						}
						catch (...) {}
					} while (!m_datasock->m_delflag && !GetExitFlag() && l_flag && (l_timelock = m_semSndC.timelock(100)) != WAIT_OBJECT_0);
					// l_flag is always returning false (OnSendBlock is not defined), so this block does absolutely nothing except locking the Sema.
				}

				if (!l_flag)
				{
					// This code is always executed atm.
					m_datasock->GetTcpApp()->Disconnect(m_datasock, 0, -7);
				}

				if (l_timelock != WAIT_OBJECT_0)
				{
					wpk = 0;
				}
				else
				{
					SNDPacket* l_snd = m_SNDPackets.Get(); // No argument, so size = 0
					// Get a new SNDPacket pointer from the heap.
					uLong		l_len = wpk.GetPktLen() + (wpk.GetPktLen() + 9) / 10;
					// Again, why?
					l_snd->Init(m_datasock, std::max<>(m_datasock->GetSendBuf(), l_len));
					// Initialize this SNDPacket based on datasock and size. 
					// NOTE: All binaries atm are using a buffersize of 64 KB, l_len will hardly be bigger than that.
					*l_snd << wpk;
					// Insert the packet to this newly created SNDPacket.
					auto const l_lockArra = std::lock_guard{m_mtxarra};
					if (m_tail)
					{
						//Check if tail exists. If it does, then link it to this new SNDPacket pointer.
						m_tail->m_next = l_snd;
						m_tail = l_snd;
					}
					else
					{
						// tail does not exist, so this l_snd will be both the tail and head.
						m_head = m_tail = l_snd;
						// The code below is identical to the one where m_tail already existed.
						if (m_head->m_cpos > uLong(0.618 * m_head->SendSize()) && !m_send && !m_datasock->m_sendflag && !m_datasock->m_delflag)
						{

							if (m_datasock->m_sendflag++ == 0)
							{
								m_datasock->GetTcpApp()->GetCommunicator()->AddTask(this);
							}
							else
							{
								m_datasock->m_sendflag--;
							}
						}
					}
				}
			}
			catch (...)
			{
				ToLogService("network", "Sender::operator<<(WPacket &wpk) exception,PeerIP:{}", m_datasock->GetPeerIP());
			}
			m_mtxcomb.lock();
		}//if(m_tail && m_tail->HasSpace())
	}//while(bool(wpk) && m_datasock->m_delflag ==0)
	m_mtxcomb.unlock();
	return *this;
}
//--------------------------------------------------
bool Sender::PopSNDPacket(int instancy)
{
	// This function rearranges m_head/m_tail/m_send
	bool l_bret = false;
	auto const l_lockArra = std::lock_guard{m_mtxarra};
	if (!m_send)
	{
		// m_send (the current one) is not found.

		if (!m_head)
		{
			// There is no head either. Packet queue has no valid packets left.
			m_send = 0;
			// Is this useless? Seems so.
		}else if(m_head !=m_tail)
		{
			// The starting packet is different than the ending packet (we got more than 1).
			m_send = m_head;
			// Our new current packet will be the head.
			m_head = m_head->m_next;
			// The head is now the next packet in line.
		}else// if(m_head ==m_tail)
		{
			// The packet in head is equal to the packet in tail (only 1 packet in the queue).
			auto const l_lockComb = std::lock_guard{m_mtxcomb};
			if (!instancy || m_head->m_cpos > uLong(0.618 * m_head->SendSize()))
			{
				// Check if the rbuf offset is larger than 61.8% of the rbuf total size.
				m_send = m_head;
				// Our new current packet will be the head.
				m_head = m_tail = 0;
				// Our head is not the next packet in line, because there are no more packets in line.
			}
		}
		l_bret = m_send ? true : false;
		// Did we manage to get a new current packet?
	}else
	{
		l_bret = false;
	}
	return l_bret;
}
//--------------------------------------------------
long Sender::Process()
{
	// This is called by the ThreadPool.
	try {
		long	l_retval;
		int		l_sendlen = 0;
		while (!m_datasock->m_delflag && !GetExitFlag() && HasData())
		{
			// Check if datasock is valid, check for the exit flag and check to see if either m_head or m_send pointers exist.
			if (!m_send)
			{
				// Huh, m_send does not exist. Get a new one, and start over.
				if (!PopSNDPacket(0))
				{
					break;
				}
			}

			// (1) send returns actual number of sent bytes - could be less than requested in len parameter.
			// (2) m_p combined with m_send->GetBufAddr() points to the beginning of the data to-be sent.
			l_sendlen = send(m_datasock->m_socket, m_send->GetBufAddr() + m_p, m_send->m_cpos - m_p, 0);
			if (l_sendlen >= 0)
			{
				// Great, we sent atleast one byte.
				m_datasock->m_sendtime = __tca->GetCurrentTick();
				m_p += l_sendlen;
				// m_p is now the number of bytes sent successfully by send()
				l_retval = 0;
			}
			else if (l_sendlen == SOCKET_ERROR)
			{
				// Generic Error
				l_retval = WSAGetLastError();
			}
			else
			{
				// Unknown error.
				l_retval = -2;
			}
			if (l_retval && (l_retval != WSAEWOULDBLOCK))
			{
				// Error handling.
				m_datasock->GetTcpApp()->Disconnect(m_datasock, 0, l_retval);
				break;
			}
			else if (m_p == m_send->m_cpos)
			{
				// Nice, the amount sent is now just the size of our packet offset. This means we finished :)
				m_datasock->m_sbts += m_p;
				m_datasock->m_spks += m_send->m_pkts;
				// Increment the number of bytes sent, and increment the number of packets sent based on how many...
				// ... WPackets were inserted into SNDPacket.
				m_send->Free();
				m_send = 0;
				m_p = 0;
				m_semSndC.unlock();
				// Free the memory allocated by rbuf, set the current m_send to null, unlock sema.

				if (!PopSNDPacket(1))
				{
				// Okay, we didn't manage to get a new valid m_send.
				SendAll	*l_task	=s_SendAlls.Get();
				l_task->Init(m_datasock);
				if(m_datasock->GetTcpApp()->GetProcessor())
				{
					m_datasock->GetTcpApp()->GetProcessor()->AddTask(l_task);
				}else
				{
					try{l_task->Process();}catch(...){}
					try{l_task->Lastly();}catch(...){}
				}
				// The code above does nothing atm, because SendAll::Process() is not defined. 
					break;
					// Break out of this god-forsaken loop. There is no valid packets left to send.
				}
			}
			else
			{
				break;
			}

		}//while(!m_datasock->m_delflag && !GetExitFlag() && HasData())
	}
	catch (...)
	{
		ToLogService("network", "Sender::Process() exception,PeerIP:{} this={:p} m_datasock={:p} m_send={:p} m_head={:p} m_tail={:p} m_p={} m_datasock->m_sendflag={}",
			m_datasock->GetPeerIP(), (void*)this, (void*)m_datasock, (void*)m_send, (void*)m_head, (void*)m_tail, (uLong)m_p, (LONG)m_datasock->m_sendflag);
	}

	m_datasock->m_sendflag--;
	return 0;
}
