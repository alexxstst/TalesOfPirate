//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#ifndef	USING_TAO		//Win32Platform SDK
#include <winsock2.h>		//WinSock2.2
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include "DataSocket.h"
#include "Packet.h"

#define DS_SHUTDOWN (0xFFFF)
#define DS_DISCONN  (0xFFFE)

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

class RPCMGR;
class Task;
class ThreadPool;
struct selparm;
struct BandwidthStat
{
	 uLong		m_tick;
	 uLong		m_sendbyteps,m_recvbyteps,m_sendpktps,m_recvpktps;
	 LLong		m_sendbytes,m_recvbytes;
	 LLong		m_sendpkts,m_recvpkts;
};

//=========================TcpCommApp=================================

extern void SetCommAppDebug( bool bDebug );

class TcpCommApp
{
	friend	class	TcpClientApp;
	friend	class	TcpServerApp;
	friend	class	RPCMGR;
	friend	class	AcceptConnect;
	friend	class	DelConnect;
	friend	class	OnConnect;
	friend	class	OnServeCall;
public:
	static	int		WSAStartup();
	static	void	WSACleanup();
//
	virtual	bool	OnConnect(DataSocket *datasock){return true;}	//:true-,false-
	virtual	void	OnConnected(DataSocket *datasock){}			//:true-,false-
	virtual	void	OnDisconnect(DataSocket *datasock,int reason){}//reason:0-;-1-Socket;-3-;-5-;-7-;-9-KeepAlive
	virtual	void	OnProcessData(DataSocket *datasock,RPacket &recvbuf)=0;
	virtual	void	OnSendAll(DataSocket *datasock){}
	virtual	bool	OnSendBlock(DataSocket *datasock){return false;}//:true-,false-
	virtual bool	OnEncrypt(DataSocket* datasock, char* ciphertext, uLong ciphertext_len, cChar* text, uLong& len);			//
	virtual bool	OnDecrypt(DataSocket* datasock, char* ciphertext, uLong& len) { return false; }	//
	virtual void	TaskDispatcher(Task *)=0;

	long			SendData(DataSocket *datasock,WPacket sendbuf);
	void			Disconnect(DataSocket *datasock,uLong remain =0,int reason =DS_DISCONN);

	BandwidthStat	GetBandwidthStat()const;
	uLong			GetSockTotal()const			{return __socklist.GetTotal();}
	ThreadPool	*	GetProcessor()const			{return __processor;}
	ThreadPool	*	GetCommunicator()const		{return __communicator;}
	uLong			GetPktHead()const;
	WPacket			GetWPacket()const;
	uLong			GetCurrentTick()const		{return m_TickCount;}
	uLong			GetRecvTime(DataSocket *datasock)const{if(datasock){return datasock->m_recvtime;}else{return uLong(long(-1));}}
	std::string			GetDisconnectErrText(int reason) const;

//const
	cLong					__maxsndque;
	cuLong					__len_offset,__pkt_maxlen;
	cuChar					__len_size{};
	bool			const	__mode;
	RPCMGR		*	const	__rpc;
protected:
	void			SetPKParse(uLong len_offset,uChar len_size,uLong pkt_maxlen,long maxsndque);
	void			BeginWork(uLong keepalive_seconds=10,uLong delay =0);

	virtual void	ShutDown(uLong ulMilliseconds);

	void			DisconnectAll();
private:
	TcpCommApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode);
	virtual			~TcpCommApp();

	void			BeforeSel(selparm &p);
	void			AfterSel(selparm &p);
	bool			AddSocket(DataSocket *datasock);
	bool			DelSocket(DataSocket *datasock);
	mutable RunBiDirectChain<DataSocket> __socklist;
	BandwidthStat		m_band;
	InterLockedLong		m_selexit;

	ThreadPool			*const __communicator,*const __communicator1,*const __processor,*const __processor1;
	InterLockedLong		m_TickCount;
	uLong			const	__delay;
	uLong				m_keepalive;

	InterLockedLong		__atnotconn,__conntotal,__deltotal;
	long				_SendData(DataSocket *datasock,WPacket &sendbuf);
};

//==============================================================================
class TcpClientApp:public TcpCommApp
{
	friend class TcpServerApp;
public:
	TcpClientApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode =true);//mode =false 
	~TcpClientApp(){};
	DataSocket *	Connect(cChar *hostname,uShort port,SOCKET *sock_out =0);
private:
	virtual void	TaskDispatcher(Task	*);
};
//=====TcpServerApp==============================================================================
class TcpServerApp:public TcpClientApp
{
public:
	TcpServerApp(RPCMGR *rpc,ThreadPool *processor,ThreadPool *communicator,bool mode =true);//mode =false 
	~TcpServerApp();
	int				OpenListenSocket(uShort port,cChar *cp);	//01
	void			CloseListenSocket();
	const SOCKET	GetListenSocket()const	{return __socket;}
	cChar	*		GetListenIP()const		{return __localaddr;}
	uShort			GetListenPort()const	{return __port;}
	void			ZeroAcceptFlag()		{__acceptflag	=false;}
protected:
	virtual	void	ShutDown(uLong ulMilliseconds);
private:
	virtual	void	TaskDispatcher(Task	*);

	bool	volatile	__acceptflag;
	SOCKET	volatile	__socket;
	uShort				__port;							//()
	char				__localaddr[16];				//(IP)
};

#pragma pack(pop)
_DBC_END
#pragma warning( disable :4355)
