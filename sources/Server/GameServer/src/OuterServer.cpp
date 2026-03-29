#include <iostream>
#include <time.h>
#include "DataSocket.h"
#include "GameCommon.h"
#include "OuterServer.h"
#include "Character.h"
#include "Player.h"
#include "GameServerApp.h"
#include "util.h"
#include "Task.h"
#include "Config.h"
#include "GameApp.h"

_DBC_USING
//OuterServer * g_gmout	=0;

OuterServer::OuterServer(ThreadPool *proc,ThreadPool *comm)
:TcpServerApp(this,proc,comm),RPCMGR(this)
,m_count(0)
{
	SetPKParse(0,2,4*1024,400);
	BeginWork(g_Config.m_lSocketAlive);
	m_mutdisc.Create(false);
}

OuterServer::~OuterServer()
{
	ShutDown(12*1000);
}

bool	OuterServer::OnConnect(DataSocket *datasock)					//:true-,false-
{
	datasock->SetRecvBuf(32*1024); 
	datasock->SetSendBuf(32*1024);
	ToLogService("network", "GateServer Connected! IP = [{}] port = {}", datasock->GetPeerIP(), datasock->GetPeerPort());

    //  GateServer
    //  :    GateServer
    auto wpkt = net::msg::serializeGmGateConnectCmd();
    AddPK(datasock, wpkt);

	return true;
}

void	OuterServer::OnDisconnect(DataSocket *datasock,int reason)		//reason:0--3--1-Socket;-5-
{
	ToLogService("network", "GateServer Disconnect! IP = [{}] port = {}, reason = [{}]", datasock->GetPeerIP(), datasock->GetPeerPort(), reason);

    GateServer* gt = (GateServer *)datasock->GetPointer();
    if (gt == NULL) return;

	if (gt->IsValid())
	{
		// Gate
		//  :    GateServer
		auto WtPk = net::msg::serializeGmGateReleaseCmd();
		AddPK(datasock, WtPk);

        // GateServer
        gt->Invalid();      
    }
}

void OuterServer::OnProcessData(DataSocket *datasock,RPacket recvbuf)
{
	AddPK(datasock,recvbuf);
}


WPacket	OuterServer::ServeCall(DataSocket *datasock,RPacket in_para)
{
    m_count++;
    WPacket l_retpk = GetWPacket();
    return l_retpk;
}

void OuterServer::ProcessData(DataSocket* datasock, RPacket pk)
{
    if (datasock == NULL) return;
    GateServer* gt = (GateServer *)datasock->GetPointer();
    if (gt == NULL) return;

    int msg_id;
    uShort cmd = pk.ReadCmd();
    switch (cmd)
        {
        case CMD_MM_GATE_CONNECT:
            msg_id = NETMSG_GATE_CONNECTED;
            break;

        case CMD_MM_GATE_RELEASE:
            msg_id = NETMSG_GATE_DISCONNECT;
            break;

        default:
            msg_id = NETMSG_PACKET;
            break;
        }

    g_pGameApp->ProcessNetMsg(msg_id, gt, pk);
}



char	g_szConnectLog[256] = "Connect";


long ToGateServer::Process()
{
	DWORD	dwTick, dwCurTick;
	DWORD	dwInterval = 5000;
	DWORD	dwConnectTick = 0;
	
	dwTick = dwCurTick = GetTickCount();
	dwTick -= dwConnectTick; // GateServer

	while (!GetExitFlag())
    {
		dwCurTick = GetTickCount();

		static DWORD dwLastRunTick = 0;
		if(dwCurTick - dwLastRunTick < dwInterval) 
   		{
			Sleep(10);
            continue;
		}
		
		dwLastRunTick = dwCurTick;

		//  GateServer 
		if (dwCurTick - dwTick >= dwConnectTick)
	    {
			dwTick = dwCurTick;
			for (int i = 0; i < g_pGameApp->m_gatenum; i++)
				if (!g_pGameApp->m_gate[i].IsValid())
					g_gmsvr->LoginGate(&g_pGameApp->m_gate[i]);
		}
	}

	return 0;
}
