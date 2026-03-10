
#include "gateserver.h"

using namespace dbc;
using namespace std;

dbc::cuShort g_version =103;

long ConnectGroupServer::Process()
{

	_tgps->m_calltotal++;

	DataSocket* datasock = NULL;
	while (!GetExitFlag() && !_tgps->m_atexit)
	{        
		if (_tgps->_connected)
		{
			if(_tgps->IsSync() && g_gtsvr)
			{
				RunChainGetArmor<ClientConnection> l(g_gtsvr->m_plylst);
				WPacket pk = _tgps->GetWPacket();
				pk.WriteCmd(CMD_TP_SYNC_PLYLST);

				int ply_cnt = g_gtsvr->m_plylst.GetTotal();		//
				pk.WriteLong(ply_cnt);
				pk.WriteString(_tgps->_myself.c_str());			//gateserver
				auto ply_array = std::make_unique<ClientConnection* []>(ply_cnt);

				int i = 0;
				for(auto l_ply =g_gtsvr->m_plylst.GetNextItem();l_ply;l_ply =g_gtsvr->m_plylst.GetNextItem())
				{
					pk.WriteLong(ToAddress(l_ply)); // GateServer
					pk.WriteLong(l_ply->m_loginID);
					pk.WriteLong(l_ply->m_actid);

					ply_array[i++] = l_ply;
				}

				RPacket retpk = _tgps->SyncCall(_tgps->get_datasock(), pk);
				int err = retpk.ReadShort();

				if (!retpk.HasData() || err == ERR_PT_LOGFAIL)
				{
					Sleep(5000);
					_tgps->Disconnect(_tgps->get_datasock());
					// 
				}
				else
				{
					int num = retpk.ReadShort();

					if(num != ply_cnt)
					{
						Sleep(5000);
						_tgps->Disconnect(_tgps->get_datasock());
						// 
					}
					else
					{
						//NOTE(Ogge): What is this? Investigate
						for(int i =0;i<num;i++)
						{
							if(retpk.ReadShort() == 1)
							{
								uLong test = retpk.ReadLong();;
								ply_array[i]->gp_addr = test;
							}
						}
					}

					_tgps->SetSync(false);
				}
			}

			// 
			Sleep(1000);
		}else
		{
			// !
			ToLogService("Connect", "{}", RES_STRING(GS_TOGROUPSERVER_CPP_00001));
			datasock = _tgps->Connect(_tgps->_gs.ip.c_str(), _tgps->_gs.port);
			if (datasock == NULL)
			{
				ToLogService("Connect", "{}", RES_STRING(GS_TOGROUPSERVER_CPP_00002));
				Sleep(5000);
				continue;
			}else
			{
				//  GroupServer
				WPacket pk = _tgps->GetWPacket();
				pk.WriteCmd(CMD_TP_LOGIN);
				pk.WriteShort(g_version);
				pk.WriteString(_tgps->_myself.c_str());

				RPacket retpk = _tgps->SyncCall(datasock, pk);
				int err = retpk.ReadShort();
				if (!retpk.HasData() || err == ERR_PT_LOGFAIL)
				{
					ToLogService("Connect", "{}", RES_STRING(GS_TOGROUPSERVER_CPP_00003));
					datasock = NULL;
					Sleep(5000);
					_tgps->Disconnect(datasock);
				}else
				{
					ToLogService("Connect", "{}", RES_STRING(GS_TOGROUPSERVER_CPP_00004));
					_tgps->_gs.datasock = datasock;
					_tgps->_connected = true;

					_tgps->SetSync();

					datasock = NULL;
				}
			}
		}
	}



	return 0;
}
Task	*ConnectGroupServer::Lastly()
{
	--(_tgps->m_calltotal);
	return Task::Lastly();
}

ToGroupServer::ToGroupServer(char const* fname, ThreadPool* proc, ThreadPool* comm)
: TcpClientApp(this, proc, comm), RPCMGR(this), _gs(), _connected(false),m_atexit(0),m_calltotal(0),
_myself()
{
	IniFile inf(fname);
	IniSection& is = inf["GroupServer"];
	_myself = inf["Main"]["Name"];
	_gs.ip = is["IP"];
	_gs.port = std::stoi(is["Port"]);

	//  PING 

	SetPKParse(0, 2, 64 * 1024, 400);
	BeginWork(std::stoi(is["EnablePing"]));

	//++m_calltotal;
	//proc->AddTask(new ConnectGroupServer(this));
}

ToGroupServer::~ToGroupServer()
{
	m_atexit	=1;
	while(m_calltotal)
	{
		Sleep(1);
	}
	ShutDown(12 * 1000);
}

bool ToGroupServer::OnConnect(DataSocket* datasock) //:true-,false-
{
	datasock->SetRecvBuf(64 * 1024);
	datasock->SetSendBuf(64 * 1024);
	ToLogService("Connect", "connect GroupServer: {},Socket num:{}", datasock->GetPeerIP(), GetSockTotal()+1);
	return true;
}

void ToGroupServer::OnDisconnect(DataSocket* datasock, int reason) //reason:0--3--1-Socket;-5-
{ //  ConnnectGroupServer 
	ToLogService("Connect", "disconnection with GroupServer,Socket num: {},reason ={}, reconnecting...", GetSockTotal(), GetDisconnectErrText(reason).c_str());

	if (!g_appexit) {_connected = false;}
}

WPacket ToGroupServer::OnServeCall(DataSocket* datasock, RPacket &in_para)
{
	uShort l_cmd = in_para.ReadCmd();
	ToLogService("ToGroupServer", "OnServeCall-->l_cmd = {}", l_cmd);

	WPacket retpk = GetWPacket();

	switch (l_cmd)
	{
	case CMD_PT_KICKPLAYINGPLAYER:
	{
		auto player = ToPointer<ClientConnection>(in_para.ReadLong());
		if (!player)
		{
			retpk.WriteShort(ERR_PT_INERR);
			return retpk;
		}

		auto wpk = GetWPacket();
		wpk.WriteCmd(CMD_TM_KICKCHA);
		wpk.WriteLong(player->m_dbid); // this should be cha_id in db
		constexpr auto timeout = 10'000;
		if (player->game)
		{
			auto rpk = SyncCall(player->game->m_datasock, wpk, timeout);
			if (!rpk.HasData())
			{
				retpk.WriteShort(ERR_PT_INERR);
				return retpk;
			}
		}

		g_gtsvr->cli_conn->Disconnect(player->m_datasock, 0, -21);

		retpk.WriteShort(ERR_SUCCESS);
		return retpk;
	}break;

	default:
	{
	}break;
	}

	return retpk;
}

void ToGroupServer::OnProcessData(DataSocket* datasock, RPacket &recvbuf)
{
	uShort	l_cmd	=recvbuf.ReadCmd();
	ToLogService("ToGroupServer", "OnProcessData-->l_cmd = {}", l_cmd);
	try
	{
		switch(l_cmd)
		{
		//case CMD_PM_GARNER2_UPDATE:
		//	{
		//		//CMD_PM_GARNER2_UPDATE
		//	}
		//	break;
		case CMD_MM_DO_STRING:{
			for (GameServer *l_game = g_gtsvr->gm_conn->_game_list; l_game; l_game = l_game->next){
				g_gtsvr->gm_conn->SendData(l_game->m_datasock, recvbuf);
			}
			break;
		}
		case CMD_PM_TEAM:
			{
				for(GameServer *l_game =g_gtsvr->gm_conn->_game_list;l_game;l_game =l_game->next)
				{
					g_gtsvr->gm_conn->SendData(l_game->m_datasock,recvbuf);
				}
#if 0
				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				recvbuf.ReadChar();
				char	l_count	=recvbuf.ReadChar();

				for(int i=0;i<l_count;i++)
				{
					if(_myself !=recvbuf.ReadString())
					{
						recvbuf.ReadLong();
						recvbuf.ReadLong();
					}else
					{
						Player * l_ply =reinterpret_cast<Player *>(MakePointer(recvbuf.ReadLong()));
						if(recvbuf.ReadLong() ==l_ply->m_worldid)
						{
							WPacket l_wpk1	=l_wpk;
							l_wpk1.WriteLong(MakeULong(l_ply));
							l_wpk1.WriteLong(l_ply->gm_addr);
							l_ply->game->m_datasock->SendData(l_wpk1);
						}
					}
				}
#endif
				break;
			}
		case CMD_AP_KICKUSER:
		case CMD_PT_KICKUSER:
			{
				uShort	l_aimnum	=recvbuf.ReverseReadShort();
				auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
				if(l_ply && l_ply->gp_addr ==recvbuf.ReverseReadLong())
				{
					ToLogService("GateServer", "GroupServer kill person,l_ply->m_dbid ={}", l_ply->m_dbid);
					g_gtsvr->cli_conn->Disconnect(l_ply->m_datasock,0,-21);
				}else if(l_ply)
				{
					ToLogService("GateServer", "GroupServer kick person, but can't kick person,l_ply->m_dbid ={}", l_ply->m_dbid);
				}
				break;
			}
		case CMD_PT_DEL_ESTOPUSER:
			{
				uShort	l_aimnum	=recvbuf.ReverseReadShort();
				auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
				if(l_ply && l_ply->gp_addr ==recvbuf.ReverseReadLong())
				{
					ToLogService("GateServer", "GroupServer del estop user,operator success,l_ply->m_dbid ={}", l_ply->m_dbid);
					l_ply->m_estop = false;
				}
				else if(l_ply)
				{
					ToLogService("GateServer", "GroupServer del estop user, but can't operator success,l_ply->m_dbid ={}", l_ply->m_dbid);
				}
			}
			break;
		case CMD_PT_ESTOPUSER:
			{
				uShort	l_aimnum	=recvbuf.ReverseReadShort();
				auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
				if(l_ply && l_ply->gp_addr ==recvbuf.ReverseReadLong())
				{
					ToLogService("GateServer", "GroupServer del estop user,operator success,l_ply->m_dbid ={}", l_ply->m_dbid);
					l_ply->m_estop = true;
				}
				else if(l_ply)
				{
					ToLogService("GateServer", "GroupServer del estop user, but can't operator success,l_ply->m_dbid ={}", l_ply->m_dbid);
				}
			}
			break;
		case CMD_MC_SYSINFO:
			l_cmd	=CMD_PC_BASE;
		default:	//
			{
				if(l_cmd/500 == CMD_PC_BASE/500)
				{
					RPacket	l_rpk		=recvbuf;
					uShort	l_aimnum	=l_rpk.ReverseReadShort();
					recvbuf.DiscardLast(sizeof(uLong)*2*l_aimnum + sizeof(uShort));
					ClientConnection* l_ply{};
					for(uShort i=0;i<l_aimnum;i++)
					{

						l_ply = ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
						if (!l_ply)
						{
							continue;
						}

						if(l_ply->gp_addr ==l_rpk.ReverseReadLong())
						{
							l_ply->SendPacketToClient(recvbuf);
						}else
						{
							//TODO(Ogge): Okay? What exactly are we doing here?
							l_ply = nullptr;
						}
					}
					if(l_cmd == CMD_PC_CHANGE_PERSONINFO && l_ply)
					{
						WPacket	l_wpk	=recvbuf;
						l_wpk.WriteCmd(CMD_TM_CHANGE_PERSONINFO);
						l_wpk.WriteLong(ToAddress(l_ply));
						l_wpk.WriteLong(l_ply->gm_addr);	//GameServer
						g_gtsvr->gm_conn->SendData(l_ply->game->m_datasock ,l_wpk);
						break;
					}
					if(l_cmd == CMD_PC_PING && l_ply)
					{
						l_ply->m_pingtime	=GetTickCount();
						break;
					}
				}else if(l_cmd/500 == CMD_PM_BASE/500)
				{
					RPacket	l_rpk		=recvbuf;
					uShort	l_aimnum	=l_rpk.ReverseReadShort();
					recvbuf.DiscardLast(sizeof(uLong)*2*l_aimnum + sizeof(uShort));
					if(!l_aimnum)
					{
						WPacket	l_wpk	=WPacket(recvbuf).Duplicate();
						l_wpk.WriteLong(0);
						for(GameServer *l_game =g_gtsvr->gm_conn->_game_list;l_game;l_game =l_game ->next)
						{
							g_gtsvr->gm_conn->SendData(l_game->m_datasock,l_wpk);
						}
					}else
					{
						WPacket l_wpk,l_wpk0 =WPacket(recvbuf).Duplicate();
						for(uShort i=0;i<l_aimnum;i++)
						{
							auto l_ply = ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
							if (!l_ply)
							{
								continue;
							}

							if(l_ply->gp_addr ==l_rpk.ReverseReadLong() && l_ply->game)
							{
								l_wpk =l_wpk0;
								l_wpk.WriteLong(ToAddress(l_ply));
								l_wpk.WriteLong(l_ply->gm_addr);
								g_gtsvr->gm_conn->SendData(l_ply->game->m_datasock ,l_wpk);
							}
						}
					}
				}
				break;
			}
		}
	}
	catch(...)
	{
		ToLogService("ToGroupServerError", "l_cmd = {}", l_cmd);
	}
}

//  GroupServer 
RPacket ToGroupServer::get_playerlist()
{
	WPacket pk = GetWPacket();
	pk.WriteCmd(CMD_TP_REQPLYLST);

	return SyncCall(_gs.datasock, pk);
}
