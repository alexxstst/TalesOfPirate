
#include "DataSocket.h"
#include "Sender.h"
#include "Receiver.h"
#include "Comm.h"
#include "RPCInfo.h"
#include <string>
#include <memory>

_DBC_USING

DataSocket::DataSocket(uLong size)
	:PreAllocStru(size), m_socket(INVALID_SOCKET),
	m_sender(std::make_unique<Sender>(this)),
	m_receiver(std::make_unique<Receiver>(this))
{
	Initially();
}

DataSocket::~DataSocket() {};

void DataSocket::Initially()
{
	RunBiDirectItem<DataSocket>::Initially();

	m_rpcinfo = nullptr;
	m_appinfo = 0;
	m_sbts = m_spks = m_rbts = m_rpks = 0;
	m_sendbytes = m_recvbytes = m_sendpkts = m_recvpkts = 0;
	m_sendbyteps = m_recvbyteps = m_sendpktps = m_recvpktps = 0;

	m_sendflag = m_recvflag = m_procflag = 0;
	m_deltime = m_delflag = m_delremain = 0;

	m_recvtime = 0;
	m_sendtime = 0;

	m_delreason = 0;
	m_isProcess = 1;

	m_pktn = 0;
	m_pktn2 = 0;

	m_sender->Initially();
	m_receiver->Initially();
}
void DataSocket::Finally()
{
	m_receiver->Finally();
	m_sender->Finally();

	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		*const_cast<SOCKET*>(&m_socket) = INVALID_SOCKET;
	}

	m_rpcinfo.reset(nullptr);

	RunBiDirectItem<DataSocket>::Finally();
	m_appinfo = nullptr;
}

void DataSocket::Init(SOCKET socket, cChar* peerip, uShort peerport, TcpCommApp* tca, bool IsServer)
{
	*const_cast<SOCKET*>(&m_socket) = socket;
	*const_cast<bool*>(&m_isServer) = IsServer;
	__tca = tca;
	//m_rpcinfo	=tca->__rpc?(m_rpcinfo =new RPCInfo)?m_rpcinfo:new RPCInfo:0;
	if (tca->__rpc)
	{
		m_rpcinfo = std::make_unique<RPCInfo>();
		if (!m_rpcinfo)
		{
			printf("rpc failed!\n");
		}
	}

	m_peerport = peerport;
	strcpy(m_peerip, peerip);

	int	l_len = sizeof(int);
	int	l_buflen = 0;
	getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&l_buflen, &l_len);
	m_sendbuf = uLong(l_buflen);
	getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&l_buflen, &l_len);
	m_recvbuf = uLong(l_buflen);

	sockaddr_in	l_sa{};
	l_len = sizeof(l_sa);
	MemSet((char*)&l_sa, 0, l_len);
	getsockname(m_socket, (sockaddr*)&l_sa, &l_len);
	strcpy(m_localip, inet_ntoa(l_sa.sin_addr));
	m_localport = ntohs(l_sa.sin_port);

	m_receiver->Init();
	m_sender->Init();

	m_cmdNum = 0;
	m_waringNum = 0;
	m_lashTick = 0;
};

WPacket	DataSocket::GetWPacket() const
{
	return __tca->GetWPacket();
}
long DataSocket::SendData(WPacket sendbuf)
{
	return __tca ? __tca->SendData(this, sendbuf) : -100;
}

void* DataSocket::GetPointer()const
{
	return m_appinfo;
}
bool	DataSocket::SetPointer(void* appinfo)
{
	bool	l_retval = false;
	if (!m_appinfo || !appinfo)
	{
		m_appinfo = appinfo;
		l_retval = true;
	}
	return l_retval;
}

extern PreAllocHeap<rbuf>	__bufheap;
int	DataSocket::SetSendBuf(uLong bytes)
{
	bytes = ((bytes + __bufheap.GetUnitSize() - 1) / __bufheap.GetUnitSize()) * __bufheap.GetUnitSize();
	m_sendbuf = max(bytes, 4 * 1024);//>=4K
	int	l_sendbuf = m_sendbuf;
	return setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&l_sendbuf, sizeof(int));
}
int	DataSocket::SetRecvBuf(uLong bytes)
{
	bytes = ((bytes + __bufheap.GetUnitSize() - 1) / __bufheap.GetUnitSize()) * __bufheap.GetUnitSize();
	m_recvbuf = max(bytes, 4 * 1024);//>=4K
	int	l_recvbuf = m_recvbuf;
	return setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&l_recvbuf, sizeof(int));
}
