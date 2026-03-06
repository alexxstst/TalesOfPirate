
#include "Comm.h"
#include "AcceptConnect.h"
#include "DataSocket.h"

_DBC_USING

extern PreAllocHeapPtr<DataSocket>	__sockheap;
PreAllocHeapPtr<AcceptConnect>	AcceptConnect::m_acceptheap(0,5);
PreAllocHeapPtr<OnConnect>		AcceptConnect::m_connectheap(0,5);
PreAllocHeapPtr<DelConnect>		DelConnect::m_delHeap(1,50);
//================================================================================
inline void OnConnect::Init(DataSocket *datasock)
{
	m_datasock	=datasock;
	__tca			=m_datasock->GetTcpApp();
}

long OnConnect::Process(){
	bool l_b	=false;
	try
	{
		l_b	=__tca->OnConnect(m_datasock);
	}catch(...){}
	if(l_b)
	{
		__tca->AddSocket(m_datasock);
		__tca->OnConnected(m_datasock);
	}else
	{
		m_datasock->Free();
	}
	return 0;
}
Task *OnConnect::Lastly()
{
	--(__tca->__conntotal);
	return PreAllocTask::Lastly();
}

//================================================================================
long AcceptConnect::Process()
{
	SOCKET		l_sock;
	sockaddr_in	l_peersaddr;
	int			l_addrlen	=sizeof sockaddr_in;
	l_sock	=accept(__tsa->GetListenSocket(),(sockaddr *)&l_peersaddr,&l_addrlen);
	__tsa->ZeroAcceptFlag();
	if(l_sock==INVALID_SOCKET){return -1;}
	uLong l_noblock =1;
	if(ioctlsocket(l_sock,FIONBIO,&l_noblock)){return 0;}
	BOOL l_enable =1;
	if(setsockopt(l_sock,SOL_SOCKET,SO_KEEPALIVE,(cChar*)&l_enable,sizeof(l_enable))){return 0;}
	DataSocket*	l_datasock	=__sockheap.Get();
	l_datasock->Init(l_sock,inet_ntoa(l_peersaddr.sin_addr),ntohs(l_peersaddr.sin_port),__tsa,true);

	OnConnect *l_onconn;
	l_onconn	=m_connectheap.Get();
	l_onconn->Init(l_datasock);
	if(__tsa->GetProcessor())
	{
		++(__tsa->__conntotal);
		if(!__tsa->__atnotconn)
		{
			__tsa->GetProcessor()->AddTask(l_onconn);
		}else
		{
			--(__tsa->__conntotal);
		}
	}else
	{
		try
		{
			l_onconn->Process();
		}catch(...){}
		try
		{
			l_onconn->Free();
		}catch(...){}
		}
	return 0;
};
Task *AcceptConnect::Lastly()
{
	--(__tsa->__conntotal);
	return PreAllocTask::Lastly();
}
//================================================================================
long DelConnect::Process()
{
	try
	{
		__tca->OnDisconnect(m_datasock,m_reason);
	}catch(...){}

	m_datasock->Free();
	return 0;
}

Task *DelConnect::Lastly()
{
	--(__tca->__deltotal);
	return PreAllocTask::Lastly();
}
