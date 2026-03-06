//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#include "Comm.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

class RPCInfo;
class TFNCall;

//==============================================================================
//RPC
class	RPCMGR
{
	friend class TcpCommApp;
	friend class OnServeCall;
	friend class OnAsynReturn;
	friend class Receiver;
	friend DataSocket *	TcpClientApp::Connect(cChar *hostname,uShort port,SOCKET *sock_out);
public:
	enum{__ses_size	=sizeof(uLong)};

	long			AsynCall(DataSocket *datasock,WPacket in_parameter,void *pointer);//pointer0one-way
	RPacket			SyncCall(DataSocket *datasock,WPacket in_parameter,uLong ulMilliseconds=30*1000);
protected:
	RPCMGR(TcpCommApp *tca);
private:
	virtual	WPacket	OnServeCall(DataSocket *datasock,RPacket &in_parameter){return 0;}
	virtual	void	OnAsynReturn(DataSocket *datasock,RPacket &in_parameter,void *pointer){};//pointerpointer
	bool			ProcessSESS(DataSocket *datasock,RPacket &recvbuf);
	long			SendSESS(DataSocket *datasock,WPacket &sendbuf);

	TcpCommApp	*	const __tca;
};

#pragma pack(pop)
_DBC_END

