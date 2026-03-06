//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#ifndef USING_TAO			//Win32Platform SDK
#include <winsock2.h>	
#include <windows.h>
	//WinSock2.2
#else
#include "TAOSpecial.h"
#endif

#include "DBCCommon.h"
#include "Packet.h"
#include "PreAlloc.h"
#include "RunBiDirectChain.h"
//#include "RPCInfo.h"
_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
//==============================DataSocket===================================
//
class RPCInfo;
class Sender;
class Receiver;

class DataSocket :public PreAllocStru, public RunBiDirectItem<DataSocket>
{
	friend			class TcpCommApp;
	friend			class TcpClientApp;
	friend			class TcpServerApp;
	friend			class RPCMGR;
	friend			class Sender;
	friend			class Receiver;
	friend			class OnProcessData;
public:
	uLong					GetRecvBuf()const		{return m_recvbuf;}
	uLong					GetSendBuf()const		{return m_sendbuf;}
	TcpCommApp			*	GetTcpApp()const		{return __tca;}
	cChar				*	GetLocalIP()const		{return m_localip;}
	uShort					GetLocalPort()const		{return m_localport;}
	cChar				*	GetPeerIP()const		{return m_peerip;}
	uShort					GetPeerPort()const		{return m_peerport;}
	int						GetDisconnectReason()const{return m_delreason;}
	bool					IsServer()const			{return m_isServer;}

	WPacket					GetWPacket() const;
	long					SendData(WPacket sendbuf);
	void				*	GetPointer()const;
	bool					SetPointer(void *appinfo);

	int						SetSendBuf(uLong bytes);
	int						SetRecvBuf(uLong bytes);

	DataSocket(uLong size);
	void Init(SOCKET socket,cChar *peerip,uShort peerport,TcpCommApp *tca,bool IsServer);
	void Free(){PreAllocStru::Free();}
	LLong	volatile		m_sendbytes,m_recvbytes;
	LLong	volatile		m_sendpkts,m_recvpkts;
	uLong	volatile		m_sendbyteps,m_recvbyteps;
	uLong	volatile		m_sendpktps,m_recvpktps;
public:
	virtual ~DataSocket();
	void Initially();
	void Finally();
	RPCInfo* GetRPCInfo()const { return m_rpcinfo.get(); }

	InterLockedLong			m_sbts,m_rbts;
	InterLockedLong			m_spks,m_rpks;

	InterLockedLong			m_sendflag,m_recvflag,m_procflag,m_isProcess;
	InterLockedLong			m_sendtime{}, m_recvtime{};
	InterLockedLong			m_deltime{}, m_delremain{}, m_delflag;
	int		volatile		m_delreason;

	TcpCommApp* __tca{ nullptr };
	std::unique_ptr<Sender> m_sender;
	std::unique_ptr<Receiver> m_receiver;
	volatile uLong			m_sendbuf, m_recvbuf;

	bool			const	m_isServer{ false };
	SOCKET			const	m_socket;
	char					m_localip[16],m_peerip[16];
	uShort					m_localport,m_peerport;

	std::unique_ptr<RPCInfo> m_rpcinfo;
	
	void* volatile	m_appinfo{};

	volatile uLong		m_cmdNum;					// 	
	volatile uLong		m_lashTick;					// 
	volatile uLong		m_waringNum;				// 	

	short				m_gsCheck;

public:
	DWORD					m_pktn{};
	char					m_pktn2{};

};

#pragma pack(pop)
_DBC_END
