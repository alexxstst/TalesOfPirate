#pragma once

#ifndef NETIF_H
#define NETIF_H

#include "CommRPC.h"
#include "PacketQueue.h"
#include "Connection.h"

class CProCirculate;

_DBC_USING

//  (->)
typedef RPacket&	LPRPACKET;
typedef WPacket&	LPWPACKET;

struct lua_State;

extern inline int lua_HandleNetMessage( lua_State* L );

// Log
class CLogName
{
public:
	CLogName();
	void	Init();

	const char*	SetLogName( DWORD dwWorlID, const char* szName );	// Log
	const char*	GetLogName( DWORD dwWorlID );		// IDlog
	const char*	GetMainLogName();					// log

	bool	IsMainCha( DWORD dwWorlID );

private:
	enum 
	{
		LOG_NAME = 256,
		LOG_MAX = 1000,
	};

	DWORD	_dwWorldArray[LOG_MAX];
	char	_szLogName[LOG_MAX][LOG_NAME];
	char	_szNoFind[LOG_NAME];
};

class NetIF : public TcpClientApp, public RPCMGR,public PKQueue{
public:
	// Packet Server -> Client 
	BOOL HandlePacketMessage(dbc::DataSocket *datasock,LPRPACKET pk);
	// Packet     Client -> Server 
	void SendPacketMessage(LPWPACKET pk);
	dbc::RPacket SyncSendPacketMessage(LPWPACKET pk,unsigned long timeout =30*1000);

//===============================================================================================
	NetIF(dbc::ThreadPool *comm =0);
	virtual ~NetIF();
	virtual void	OnProcessData(dbc::DataSocket *datasock,dbc::RPacket& recvbuf);
	virtual bool	OnConnect(dbc::DataSocket *datasock);					//:true-,false-
	virtual void	OnDisconnect(dbc::DataSocket *datasock,int reason);		//reason:0--1-Socket-3--5-
	std::string GetDisconnectErrText(int reason)const;

	bool IsConnected(){return m_connect.IsConnected();}
	int	 GetConnStat(){return m_connect.GetConnStat();}
	virtual void ProcessData(dbc::DataSocket *datasock,dbc::RPacket& recvbuf);

	unsigned long	 GetAveragePing();
	CProCirculate	*GetProCir(void) {return m_pCProCir;}
	void SwitchNet(bool isConnected);

	Connection		m_connect;
	struct
	{
		dbc::uLong	m_pingid;
		dbc::uLong	m_maxdelay,m_curdelay,m_mindelay;
		DWORD dwLatencyTime[20];

		// pingclient,server  xuedong 2004.09.01
		dbc::uLong	m_ulCurStatistic;
		dbc::uLong	m_ulDelayTime[4];
		// end
	};
	unsigned long   m_ulPacketCount; // xuedong 2004.09.10
	long			m_framedelay; // 

	CProCirculate	*m_pCProCir;
	std::recursive_mutex		m_mutmov;
	char			m_accounts[100];
	char			m_passwd[100];
	bool			handshakeDone;



	bool _enc; // 
	int _comm_enc; // 
	char _key[12];
	int _key_len;
	virtual bool OnEncrypt(dbc::DataSocket* datasock, char* ciphertext, uLong ciphertext_len, const char* text, unsigned long& len);
	virtual bool OnDecrypt(dbc::DataSocket *datasock,char *ciphertext,unsigned long& len);
	lua_State* g_rLvm;
	lua_State* g_sLvm;
};


extern NetIF	*g_NetIF;
extern CLogName	 g_LogName;

#endif
