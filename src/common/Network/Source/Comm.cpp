//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#define	FD_SETSIZE      1024

#include "CommRPC.h"
#include "ThreadPool.h"
#include "Sender.h"
#include "Receiver.h"
#include "AcceptConnect.h"

#include <iostream>
#include <string>
#include <algorithm>
using namespace std::string_literals;

#ifdef	FD_SET	//FD_SET
#undef	FD_SET
#endif			//FD_SET
#define FD_SET(fd, set) \
	((fd_set FAR *)(set))->fd_array[((fd_set FAR *)(set))->fd_count] = (fd); \
	((fd_set FAR *)(set))->fd_count++;

_DBC_USING
InterLockedLong				__bufinit	=0;
PreAllocHeap<rbuf>			__bufheap(1024);
PreAllocHeapPtr<DataSocket>	__sockheap(1,100);
extern	uLong	NetBuffer[];

bool g_bCommAppDebug = false;
void SetCommAppDebug( bool bDebug )
{
	g_bCommAppDebug = bDebug;
}
//:
//static uLong	NetBuffer[]	={400,300,200,100,50,40,30,0};

//=====DispatchThread==============================================================================
class DispatchThread:public Task
{
public:
	DispatchThread(TcpCommApp *tcpapp):m_tcpapp(tcpapp){};
private:
	virtual long Process()
	{
		long l_stat =0;
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
		m_tcpapp->TaskDispatcher(this);
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
		return 0;
	};

	TcpCommApp*	m_tcpapp;
};
//================================================================================
int TcpCommApp::WSAStartup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = ::WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return -1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 )
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return -1; 
	}
	return 0;
}
void TcpCommApp::WSACleanup()
{
	::WSACleanup();
}
//================================================================================
TcpCommApp::TcpCommApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode)
		:__maxsndque(10),m_keepalive(10*1000),__delay(0)
		,__len_offset(0),__len_size(2),__pkt_maxlen(4*1024)
		,__rpc(rpc),m_selexit(0),__mode(mode)
		,__communicator1(communicator)
		,__communicator(communicator?communicator:ThreadPool::CreatePool(3,3,128,THREAD_PRIORITY_ABOVE_NORMAL))
		,__processor1(processor),__processor(processor)
		,__atnotconn(0),__conntotal(0),__deltotal(0)
{
	m_band.m_sendbytes	=m_band.m_sendpkts	=m_band.m_recvbytes=m_band.m_recvpkts		=0;
};
TcpCommApp::~TcpCommApp()
{
	if(!__communicator1)
	{
		if(__communicator)
		{
			__communicator->m_taskexitflag	=TRUE;
			__communicator->DestroyPool();
		}
	}
	if(!__processor1)
	{
		if(__processor)
		{
			__processor->m_taskexitflag	=TRUE;
			__processor->DestroyPool();
		}
	}
};
void	TcpCommApp::ShutDown(uLong ulMilliseconds)
{
	__atnotconn	=1;
	while(__conntotal)
	{
		Sleep(1);
	}
	RunChainGetArmor<DataSocket> l(__socklist);
	for(DataSocket	*l_datasock	=__socklist.GetNextItem();l_datasock;l_datasock	=__socklist.GetNextItem())
	{
		::shutdown(l_datasock->m_socket,SD_SEND);
		Disconnect(l_datasock,0,DS_SHUTDOWN);
	}
	l.unlock();

	uLong	l_starttime	=GetTickCount();
	while(GetTickCount()-l_starttime <ulMilliseconds && GetSockTotal())
	{
		Sleep(1);
	}
	while(__deltotal)
	{
		Sleep(1);
	}
	m_selexit	=1;
	l_starttime	=GetTickCount();
	while(m_selexit &&GetTickCount()-l_starttime <1000)
	{
		Sleep(1);
	}
}
//--------------------------------------------------------------------
void TcpCommApp::SetPKParse(uLong len_offset,uChar len_size,uLong pkt_maxlen,long maxsndque)
{
	*const_cast<long *>(&__maxsndque)	=max(maxsndque,10);
	*const_cast<uLong *>(&__len_offset)	=len_offset;
	*const_cast<uChar *>(&__len_size)	=len_size;
	*const_cast<uLong *>(&__pkt_maxlen)	=pkt_maxlen;
	if(((__len_size !=2)&&(__len_size !=4)&&(__len_size !=1)))
	{
		THROW_EXCP(excp,"packet size is wrong!");
	}
}
bool TcpCommApp::AddSocket(DataSocket *datasock)
{
	if(datasock)
	{
		datasock->m_sendtime	=datasock->m_recvtime	=m_TickCount;
		return datasock->_BeginRun(&__socklist)?true:false;
	}else
	{
		return false;
	}
}
inline bool TcpCommApp::DelSocket(DataSocket *datasock)
{
	return datasock?datasock->_EndRun()?true:false:false;
}
BandwidthStat	TcpCommApp::GetBandwidthStat()const
{
	printf( "TcpCommApp::GetBandwidthStat: lock.\n" );
	BandwidthStat	l_band;
	RunChainGetArmor<DataSocket> l(__socklist);
	l_band	=m_band;
	l.unlock();
	printf( "TcpCommApp::GetBandwidthStat: unlock.\n" );
	return l_band;
}
uLong TcpCommApp::GetPktHead()const
{
	return __len_offset + __len_size + (__rpc?(RPCMGR::__ses_size):0);
}
WPacket TcpCommApp::GetWPacket()const
{
	WPacket l_ret =this;
	l_ret =__bufheap.Get(1024);
	return l_ret;
}
void TcpCommApp::BeginWork(uLong keepalive_seconds,uLong delay)
{
	keepalive_seconds = std::max<>(keepalive_seconds, 10UL);

	m_keepalive = keepalive_seconds * 1000; // milliseconds

	*const_cast<uLong*>(&__delay)		=delay;
	m_TickCount	=GetTickCount();
	if(++__bufinit ==1)
	{
		__bufheap.Init(NetBuffer);
	}
	Task *task	=new DispatchThread(this);
	__communicator->AddTask(task);
}

void TcpCommApp::DisconnectAll()
{
	RunChainGetArmor<DataSocket> l(__socklist);
	DataSocket *l_datasock;
	while(l_datasock =__socklist.GetNextItem())
	{
		Disconnect(l_datasock);
	}
	l.unlock();
}

//--------------------------------------------------------------------
void TcpCommApp::Disconnect(DataSocket *datasock,uLong remain,int reason)
{
	if(datasock)
	{
		datasock->m_delreason	=reason;
		datasock->m_delremain	=remain;
		datasock->m_deltime		=datasock->__tca->m_TickCount;
	}
};

bool dbc::TcpCommApp::OnEncrypt(DataSocket* datasock, char* ciphertext, uLong ciphertext_len, cChar* text, uLong& len)
{
	if (ciphertext == text)
	{
		return true;
	}

	MemCpy(ciphertext, text, len); 

	return true;
}

//--------------------------------------------------------------------
long TcpCommApp::SendData(DataSocket *datasock,WPacket sendbuf)
{
	if(__rpc)
	{
		return __rpc->SendSESS(datasock,sendbuf);
	}else
	{
		return _SendData(datasock,sendbuf);
	}
}
long TcpCommApp::_SendData(DataSocket *datasock,WPacket &sendbuf)
{
	if(!datasock||datasock->m_delflag ||!bool(sendbuf))
	{
		return -1;
	}
	*datasock->m_sender << sendbuf;
	return 0;
};
//--------------------------------------------------------------------
_DBC_BEGIN
struct selparm
{
	fd_set			m_readfds{}, m_writefds{}, m_errfds{};
	bool			m_readflag{}, m_writeflag{}, m_errflag{};
	int				m_selretval{};
	uLong			m_tick{};
};
_DBC_END


void TcpCommApp::BeforeSel(selparm &p)
{
	if( g_bCommAppDebug )
	{
		printf( "CommApp: BeforeSel start!\n" );
	}

	uLong	l_tick	=GetTickCount();
	if(l_tick >uLong(m_TickCount))
	{
		m_TickCount	=l_tick;
	}else if(l_tick <uLong(m_TickCount))
	{
		l_tick		=++m_TickCount;
		std::cout<<std::endl<<"system GetTickCount function wrong: later < before."<<std::endl;
	}
	uLong	l_sendbytes,l_recvbytes,l_sendpkts,l_recvpkts;
	uLong	l_intertime;
	bool l_flag	=((l_tick -p.m_tick) >=1000);
	RunChainGetArmor<DataSocket> l(__socklist);
	if(l_flag)
	{
		l_sendbytes =l_recvbytes =l_sendpkts =l_recvpkts	=0;
		l_intertime		=l_tick -p.m_tick;
		p.m_tick		=l_tick;
		m_band.m_tick	=l_tick;
	}
	DataSocket *l_datasock;
	while(l_datasock =__socklist.GetNextItem())
	{
		if(l_flag)
		{
			uLong	l_ulong	=l_datasock->m_sbts.SetZero();
			if(l_ulong)
			{
				l_sendbytes				+=l_ulong;
				l_datasock->m_sendbytes	+=l_ulong;
				if(l_ulong <(0xFFFFFFFF/1000))
				{
					l_datasock->m_sendbyteps =l_ulong*1000/l_intertime;
				}else
				{
					l_datasock->m_sendbyteps =(l_ulong/l_intertime)*1000;
				}
			}else
			{
				l_datasock->m_sendbyteps =0;
			}
			l_ulong			=l_datasock->m_spks.SetZero();
			if(l_ulong)
			{
				l_sendpkts				+=l_ulong;
				l_datasock->m_sendpkts	+=l_ulong;
				if(l_ulong <(0xFFFFFFFF/1000))
				{
					l_datasock->m_sendpktps	 =l_ulong*1000/l_intertime;
				}else
				{
					l_datasock->m_sendpktps	 =(l_ulong/l_intertime)*1000;
				}
			}else
			{
				l_datasock->m_sendpktps	 =0;
			}
			l_ulong			=l_datasock->m_rbts.SetZero();
			if(l_ulong)
			{
				l_recvbytes				+=l_ulong;
				l_datasock->m_recvbytes	+=l_ulong;
				if(l_ulong <(0xFFFFFFFF/1000))
				{
					l_datasock->m_recvbyteps =l_ulong*1000/l_intertime;
				}else
				{
					l_datasock->m_recvbyteps =(l_ulong/l_intertime)*1000;
				}
			}else
			{
				l_datasock->m_recvbyteps =0;
			}
			l_ulong			=l_datasock->m_rpks.SetZero();
			if(l_ulong)
			{
				l_recvpkts				+=l_ulong;
				l_datasock->m_recvpkts	+=l_ulong;
				if(l_ulong <(0xFFFFFFFF/1000))
				{
					l_datasock->m_recvpktps	 =l_ulong*1000/l_intertime;
				}else
				{
					l_datasock->m_recvpktps	 =(l_ulong/l_intertime)*1000;
				}
			}else
			{
				l_datasock->m_recvpktps	 =0;
			}
		}
		if(m_keepalive)
		{
			if (auto delta = l_tick - l_datasock->m_recvtime; delta > m_keepalive)
			{
				Disconnect(l_datasock, 0, -9);
			}
			else if (l_tick - 2000 > uLong(l_datasock->m_sendtime))
			{
				WPacket	l_wpk = GetWPacket();
				l_wpk.m_wpos = __len_offset + __len_size - l_wpk.GetPktLen();
				_SendData(l_datasock, l_wpk);
				l_datasock->m_sendtime = l_tick;
			}
		}
		if(l_datasock->m_deltime)
		{
			if(l_datasock->m_isProcess)	l_datasock->m_isProcess	=0;
			if((l_tick-l_datasock->m_deltime) >=uLong(l_datasock->m_delremain))
			{
				if (!l_datasock->m_delflag)
				{
					l_datasock->m_delflag = 1;
				}
				if(l_datasock->m_recvflag++ ==0)
				{
					if( l_datasock->m_procflag <=0 )
					{
						if(l_datasock->m_sendflag++ ==0)
						{
							DelSocket(l_datasock);

							++__deltotal;
							DelConnect	*l_del	=DelConnect::m_delHeap.Get();
							l_del->Init(l_datasock,l_datasock->m_delreason);
							(__processor?__processor:__communicator)->AddTask(l_del);
						}
						else
						{
							l_datasock->m_sendflag--;l_datasock->m_recvflag--;
						}
					}
					else
					{
						l_datasock->m_recvflag--;
					}
				}else
				{
					l_datasock->m_recvflag--;
				}
				continue;
			}
		}

		FD_SET(l_datasock->m_socket,&(p.m_errfds));
		p.m_errflag	=true;

		if(!l_datasock->m_recvflag)
		{
			FD_SET(l_datasock->m_socket,&(p.m_readfds));
			p.m_readflag	=true;
		}
		if(!l_datasock->m_sendflag&&l_datasock->m_sender->HasData())
		{
			FD_SET(l_datasock->m_socket,&(p.m_writefds));
			p.m_writeflag	=true;
		}
	}//while(l_datasock =__socklist.GetNextItem())
	if(l_flag)
	{
		m_band.m_sendbytes		+=l_sendbytes;
		m_band.m_sendpkts		+=l_sendpkts;
		m_band.m_recvbytes		+=l_recvbytes;
		m_band.m_recvpkts		+=l_recvpkts;
		if(l_sendbytes <(0xFFFFFFFF/1000))
		{
			m_band.m_sendbyteps =l_sendbytes*1000/l_intertime;
		}else
		{
			m_band.m_sendbyteps =(l_sendbytes/l_intertime)*1000;
		}
		if(l_sendpkts <(0xFFFFFFFF/1000))
		{
			m_band.m_sendpktps =l_sendpkts*1000/l_intertime;
		}else
		{
			m_band.m_sendpktps =(l_sendpkts/l_intertime)*1000;
		}
		if(l_recvbytes <(0xFFFFFFFF/1000))
		{
			m_band.m_recvbyteps =l_recvbytes*1000/l_intertime;
		}else
		{
			m_band.m_recvbyteps =(l_recvbytes/l_intertime)*1000;
		}
		if(l_recvpkts <(0xFFFFFFFF/1000))
		{
			m_band.m_recvpktps =l_recvpkts*1000/l_intertime;
		}else
		{
			m_band.m_recvpktps =(l_recvpkts/l_intertime)*1000;
		}
	}
	l.unlock();

	if( g_bCommAppDebug )
	{
		printf( "CommApp: BeforeSel end!\n" );
	}
}
void TcpCommApp::AfterSel(selparm &p)
{
	if( g_bCommAppDebug )
	{
		printf( "CommApp: AfterSel start!\n" );
	}

	RunChainGetArmor<DataSocket> l(__socklist);
	DataSocket *l_datasock;
	while((l_datasock	=__socklist.GetNextItem())&&p.m_selretval)
	{
		if(l_datasock->m_delflag)continue;
		if(FD_ISSET(l_datasock->m_socket,&(p.m_errfds)))
		{
			Disconnect(l_datasock,0,-1);
			p.m_selretval--;
		}else
		{
			if(!l_datasock->m_recvflag&&FD_ISSET(l_datasock->m_socket,&(p.m_readfds)))
			{
				if(l_datasock->m_recvflag++ ==0)
				{
					__communicator->AddTask(l_datasock->m_receiver.get());
				}else
				{
					l_datasock->m_recvflag--;
				}
				p.m_selretval--;
			}
			if(!l_datasock->m_sendflag&&l_datasock->m_sender->HasData()&&FD_ISSET(l_datasock->m_socket,&(p.m_writefds)))
			{
				if(l_datasock->m_sendflag++	==0)
				{
					__communicator->AddTask(l_datasock->m_sender.get());
				}else
				{
					l_datasock->m_sendflag--;
				}
				p.m_selretval--;
			}
		}
	}
	l.unlock();

	if( g_bCommAppDebug )
	{
		printf( "CommApp: AfterSel end!\n" );
	}
}
//==============TcpClientApp==================================================================
TcpClientApp::TcpClientApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode)
		:TcpCommApp(rpc,processor,communicator,mode)
{
}
void TcpClientApp::TaskDispatcher(Task *l_dt)
{
	timeval		l_timeout;
	selparm		l_p;

	while(!l_dt->GetExitFlag() && !m_selexit)
	{
		FD_ZERO(&(l_p.m_readfds));
		FD_ZERO(&(l_p.m_writefds));
		FD_ZERO(&(l_p.m_errfds));
		l_p.m_readflag	=l_p.m_writeflag	=l_p.m_errflag	=false;
		BeforeSel(l_p);
		if(!l_p.m_readflag&&!l_p.m_writeflag&&!l_p.m_errflag)
		{
			Sleep(2);
			continue;
		}
		l_timeout.tv_sec	=0;
		l_timeout.tv_usec	=2000;
		l_p.m_selretval		=select(0,l_p.m_readflag?&(l_p.m_readfds):0,l_p.m_writeflag?&(l_p.m_writefds):0,l_p.m_errflag?&(l_p.m_errfds):0,&l_timeout);
		switch(l_p.m_selretval)
		{
		case 0:
			{
				break;
			}
		case SOCKET_ERROR:
			{
				int l_wsaerr	=WSAGetLastError();
				if((l_wsaerr==WSAENETDOWN)||(l_wsaerr==WSANOTINITIALISED))
				{
					return;
				}
				break;
			}
		default:
			{
				AfterSel(l_p);
			}
		}
		if(__delay)Sleep(__delay);
	}
	m_selexit	=0;
};
//--------------------------------------------------------------------
DataSocket *	TcpClientApp::Connect(cChar *hostname,uShort port,SOCKET *sock_out)
{
	if(!__atnotconn)
	{
		RefArmor	l_conn(__conntotal);
		sockaddr_in	l_peersaddr;
		HOSTENT	*	l_hostent;

		MemSet((char*)&l_peersaddr,0,sizeof(sockaddr_in));
		l_peersaddr.sin_family					=AF_INET;
		l_peersaddr.sin_port					=htons(port);
		if((l_peersaddr.sin_addr.S_un.S_addr	=inet_addr(hostname))==INADDR_NONE)
		{
			l_hostent	=gethostbyname(hostname);
			if (!l_hostent) { return nullptr; }
			l_peersaddr.sin_addr.S_un.S_addr	=*(uLong*)(l_hostent->h_addr);
		}
		SOCKET	l_sock	=socket(PF_INET,SOCK_STREAM,0);
		if(sock_out)*sock_out	=l_sock;

		if(connect(l_sock,(sockaddr*)&l_peersaddr,sizeof(sockaddr))==SOCKET_ERROR)
		{
			closesocket(l_sock);
			return 0;
		}

		uLong l_noblock =1;
		if (ioctlsocket(l_sock, FIONBIO, &l_noblock)) { closesocket(l_sock); return nullptr; }
		BOOL l_enable =1;
		//if(setsockopt(l_sock,SOL_SOCKET,SO_OOBINLINE,(cChar*)&l_enable,sizeof(l_enable))){return 0;}
		if (setsockopt(l_sock, SOL_SOCKET, SO_KEEPALIVE, (cChar*)&l_enable, sizeof(l_enable))) { closesocket(l_sock); return nullptr; }

		DataSocket *l_datasock	=__sockheap.Get();
		l_datasock->Init(l_sock,inet_ntoa(l_peersaddr.sin_addr),port,this,false);

		bool	l_b	=false;
		try{
			l_b	=OnConnect(l_datasock);
		}catch(...){}
		if(l_b)
		{
			AddSocket(l_datasock);
			return l_datasock;
		}else
		{
			l_datasock->Free();
			return nullptr;
		}
	}else
	{
		return nullptr;
	}
};
//==============TcpServerApp==================================================================
TcpServerApp::TcpServerApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode)
		:__socket(INVALID_SOCKET),__port(1973),__acceptflag(false)
		,TcpClientApp(rpc,processor,communicator,mode)
{
	in_addr l_ina;
	l_ina.S_un.S_addr	=INADDR_ANY;
	strcpy(__localaddr,inet_ntoa(l_ina));
}
TcpServerApp::~TcpServerApp()
{
	if(__socket!=INVALID_SOCKET)
	{
		closesocket(__socket);
	}
};
//--------------------------------------------------------------------
void TcpServerApp::ShutDown(uLong ulMilliseconds)
{
	CloseListenSocket();
	TcpClientApp::ShutDown(ulMilliseconds);
}
//--------------------------------------------------------------------
int TcpServerApp::OpenListenSocket(uShort port,cChar *cp)
{
	__port	=port;
	if(cp && (strlen(cp) < 16))
	{
		strcpy(__localaddr,cp);
	}else
	{
		in_addr l_ina;
		l_ina.S_un.S_addr	=INADDR_ANY;
		strcpy(__localaddr,inet_ntoa(l_ina));
	}

	if(__socket !=INVALID_SOCKET){return -1;}
	__socket	=socket(PF_INET,SOCK_STREAM,0);
	if(__socket ==INVALID_SOCKET){return -1;}

	sockaddr_in my_addr;
	MemSet((char*)&my_addr,0,sizeof(my_addr));

	uLong l_inaddr	=__localaddr[0]?inet_addr(__localaddr):INADDR_ANY;
	if(l_inaddr ==INADDR_NONE){return -1;}

	my_addr.sin_family				=AF_INET;
	my_addr.sin_addr.S_un.S_addr	=l_inaddr;
	my_addr.sin_port					=htons(__port);

	if(bind(__socket,(sockaddr FAR*)&my_addr,sizeof(my_addr))){return -2;}
	if(listen(__socket,5)){return -3;}

	uLong l_noblock =1;
	if(ioctlsocket(__socket,FIONBIO,&l_noblock)){return -4;}
	__acceptflag	=false;

	return 0;
};
//--------------------------------------------------------------------
void TcpServerApp::CloseListenSocket()
{
	SOCKET	l_sock	=__socket;
	if(l_sock!=INVALID_SOCKET)
	{
		__socket	=INVALID_SOCKET;
		closesocket(l_sock);
	}
	__acceptflag	=true;
};
//--------------------------------------------------------------------
void TcpServerApp::TaskDispatcher(Task *l_dt)
{
	AcceptConnect	*l_newconn;
	timeval			l_timeout;
	selparm			l_p;

	while(!l_dt->GetExitFlag() && !m_selexit)
	{
		FD_ZERO(&(l_p.m_readfds));
		FD_ZERO(&(l_p.m_writefds));
		FD_ZERO(&(l_p.m_errfds));
		l_p.m_readflag	=l_p.m_writeflag	=l_p.m_errflag	=false;
		if(__socket!=INVALID_SOCKET)
		{
			FD_SET(__socket,&(l_p.m_errfds));
			l_p.m_errflag	=true;
			if(!__acceptflag && !__atnotconn)
			{
				FD_SET(__socket,&(l_p.m_readfds));
				l_p.m_readflag	=true;
			}
		}
		BeforeSel(l_p);
		if(!l_p.m_readflag&&!l_p.m_writeflag&&!l_p.m_errflag)
		{
			Sleep(2);
			continue;
		}
		l_timeout.tv_sec	=0;
		l_timeout.tv_usec	=2000;
		l_p.m_selretval			=select(0,l_p.m_readflag?&(l_p.m_readfds):0,l_p.m_writeflag?&(l_p.m_writefds):0,l_p.m_errflag?&(l_p.m_errfds):0,&l_timeout);
		switch(l_p.m_selretval)
		{
		case 0:
			{
				break;
			}
		case SOCKET_ERROR :
			{
				int l_wsaerr	=WSAGetLastError();
				if((l_wsaerr==WSAENETDOWN)||(l_wsaerr==WSANOTINITIALISED))
				{
					return;
				}
				break;
			}
		default:
			{
				if(FD_ISSET(__socket,&(l_p.m_errfds)))
				{
					CloseListenSocket();
					OpenListenSocket(__port,__localaddr);
					l_p.m_selretval--;
				}else if(!__acceptflag&&FD_ISSET(__socket,&(l_p.m_readfds)))
				{
					++__conntotal;
					if(!__atnotconn)
					{
						l_newconn	=AcceptConnect::m_acceptheap.Get();
						l_newconn->Init(this);
						__acceptflag	=true;
						__communicator->AddTask(l_newconn);
					}else
					{
						--__conntotal;
					}
					l_p.m_selretval--;
				}
				AfterSel(l_p);
			}
		}
		if(__delay)Sleep(__delay);
	}
	m_selexit	=0;
}
std::string TcpCommApp::GetDisconnectErrText(int reason) const {
	// WSA error codes
	// https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2
	switch (reason) {
	case 0:
		return "The local program exits normally.";
	case -1:
		return "Socket error.";
	case -3:
		return "The network was shut down normally by the other party.";
	case -5:
		return "The packet length exceeds the limit.";
	case -7:
		return "Data blocking on the socket is too severe.";
	case -9:
		return "KeepAlive failed.";
	case DS_DISCONN:
		return "The program uses the default anti-link.";
	case DS_SHUTDOWN:
		return "The program shut down due to the ShutDown network subsystem.";
	case WSAECONNRESET:
		return "The remote host forcibly closed an existing connection.";
	case WSAENETDOWN:
		return "The socket operation encountered a dead network.";
	case WSAENETUNREACH:
		return "A socket operation was attempted against an unreachable network.";
	case WSAENETRESET:
		return "While the operation was in progress, the connection was interrupted due to a failure detected by the keep-alive operation.";
	case WSAECONNABORTED:
		return "The software in your host dropped an established connection.";
	case WSAENOBUFS:
		return "The operation on the socket cannot be performed because the system buffer space is insufficient or the queue is full.";
	case WSAEISCONN:
		return "A connection request was made on an already connected socket.";
	case WSAENOTCONN:
		return "Since the socket was not connected and (when sending a datagram socket using a sendto call) no address was provided, a request to send or receive data was not accepted.";
	case WSAESHUTDOWN:
		return "Due to a previous close call, the socket has been closed in that direction, and requests to send or receive data were not accepted.";
	case WSAETOOMANYREFS:
		return "Too many references to a kernel object.";
	case WSAETIMEDOUT:
		return "The connection attempt failed because the connecting party did not reply correctly after a while or the connected host did not respond.";
	case WSAECONNREFUSED:
		return "Unable to connect because the target machine actively refused.";
	case WSAEHOSTDOWN:
		return "The socket operation failed because the target host is broken.";
	case WSAEHOSTUNREACH:
		return "The socket operation attempted an unreachable host.";
	case WSAEPROCLIM:
		return "A Windows socket operation may have a limit on the number of applications that can be used simultaneously.";
	case WSAEADDRINUSE:
		return "Usually each socket address (protocol / network address / port) is allowed to be used only once.";
	case WSAEAFNOSUPPORT:
		return "An address that is incompatible with the requested protocol was used.";
	default:
		return "Unknown Disconnect code:"s + std::to_string(reason);
	}
}
