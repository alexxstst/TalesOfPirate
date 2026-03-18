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

bool	OuterServer::OnConnect(DataSocket *datasock)					//返回值:true-允许连接,false-不允许连接
{
	datasock->SetRecvBuf(32*1024); 
	datasock->SetSendBuf(32*1024);
	LG(g_szConnectLog, "GateServer Cconnected! IP = [%s] port = %d\n",  datasock->GetPeerIP() , datasock->GetPeerPort());

    // 通知应用层，连上一个 GateServer
    WPacket wpkt = g_gmsvr->GetWPacket();
    wpkt.WriteCmd(CMD_MM_GATE_CONNECT);
    wpkt.WriteChar(0);
    AddPK(datasock, wpkt);

	return true;
}

void	OuterServer::OnDisconnect(DataSocket *datasock,int reason)		//reason值:0-本地程序正常退出；-3-网络被对方关闭；-1-Socket错误;-5-包长度超过限制。
{
	LG(g_szConnectLog, "GateServer Disconnect! IP = [%s] port = %d, reason = [%d]\n",  datasock->GetPeerIP() , datasock->GetPeerPort(), reason);

    GateServer* gt = (GateServer *)datasock->GetPointer();
    if (gt == NULL) return;

	if (gt->IsValid())
	{
		// 通知逻辑层与该Gate的连接断开
		WPacket WtPk=g_gmsvr->GetWPacket();
		WtPk.WriteCmd(CMD_MM_GATE_RELEASE);
		WtPk.WriteChar(0);
		AddPK(datasock, WtPk);

        // 清理GateServer
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
	dwTick -= dwConnectTick; // 保证线程运行后立即执行连接GateServer的操作

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

		// 对未连接的 GateServer 进行连接
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
