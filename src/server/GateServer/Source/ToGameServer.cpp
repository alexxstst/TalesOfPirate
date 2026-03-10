
#include "gateserver.h"
using namespace std;
using namespace dbc;

ToGameServer::ToGameServer(char const* fname, ThreadPool* proc, ThreadPool* comm)
: TcpServerApp(this, proc, comm), RPCMGR(this), _mut_game(),
_game_heap(1, 20), _game_list(NULL), _map_game()
{
	_game_num = 0;

	// 
	IniFile inf(fname);
	IniSection& is = inf["ToGameServer"];
	const std::string ip = is["IP"]; uShort port = std::stoi(is["Port"]);

	//  PING 

	SetPKParse(0, 2, 64 * 1024, 400); BeginWork(std::stoi(is["EnablePing"]));
	if (OpenListenSocket(port, ip.c_str()) != 0)
	{
		THROW_EXCP(excp, "ToGameServer listen failed");
	}
}

ToGameServer::~ToGameServer() {ShutDown(12 * 1000);}

void ToGameServer::_add_game(GameServer* game)
{
	game->next = _game_list;
	_game_list = game;
	++ _game_num;
}

bool ToGameServer::_exist_game(char const* game)
{
	GameServer* curr = _game_list;
	bool exist = false;

	while (curr)
	{
		if (curr->gamename == game) {exist = true; break;}
		curr = curr->next;
	}

	return exist;
}

void ToGameServer::_del_game(GameServer* game)
{
	GameServer* curr = _game_list;
	GameServer* prev = 0;
	while (curr)
	{
		if (curr == game) break;
		prev = curr; curr = curr->next;
	}

	if (curr)
	{
		if (prev)
			prev->next = curr->next;
		else
			_game_list = curr->next;
		-- _game_num;
	}
}

bool ToGameServer::OnConnect(DataSocket* datasock) // :true-,false-
{
	datasock->SetPointer(0);
	datasock->SetRecvBuf(64 * 1024);
	datasock->SetSendBuf(64 * 1024);
	ToLogService("GateServer", "GameServer= [{}] come,Socket num= {}", datasock->GetPeerIP(), GetSockTotal()+1);
	return true;
}

void ToGameServer::OnDisconnect(DataSocket* datasock, int reason) // reason:0--3--1-Socket;-5-
{
	ToLogService("GateServer", "GameServer= [{}] gone,Socket num= {},reason= {}", datasock->GetPeerIP(), GetSockTotal()+1, GetDisconnectErrText(reason).c_str());

	if (reason == DS_SHUTDOWN || reason == DS_DISCONN)
	{
		return;
	}

	auto l_game = static_cast<GameServer*>(datasock->GetPointer());
	if (!l_game)
	{
		return;
	}

	bool already_delete = false;
	//  GameServer
	_mut_game.lock();
	try
	{
		//NOTE(Ogge): Is it checking twice because of multithreading? 
		// Once before locking, then as safety after locking?
		l_game = static_cast<GameServer*>(datasock->GetPointer());
		if (l_game)
		{
			ToLogService("GateServer", " delete [{}]", l_game->gamename.c_str());
			_del_game(l_game);
			for (int i = 0; i < l_game->mapcnt; ++i)
			{
				ToLogService("GateServer", "delete map [{}]", l_game->maplist[i].c_str());
				_map_game.erase(l_game->maplist[i]);
			}
			l_game->mapcnt = 0;
			l_game->Free();
			datasock->SetPointer(nullptr);
		}
		else
		{
			already_delete = true;
		}
	}
	catch (...)
	{
		ToLogService("GateServer", "Exception raised from OnDisconnect{{delete GameServer from list}}");
	}
	_mut_game.unlock();

	if (already_delete) return;

	// GateServerGameServer
	RPacket retpk = g_gtsvr->gp_conn->get_playerlist();
	uShort ply_cnt = retpk.ReverseReadShort(); // GateServer

	ClientConnection* ply_addr{}; uLong db_id{};
	auto ply_array = std::make_unique<ClientConnection* []>(ply_cnt);
	uShort	l_notcount	=0;
	for (uShort i = 0; i < ply_cnt; ++ i)
	{
		ply_addr = ToPointer<ClientConnection>(retpk.ReadLong());
		db_id = (uLong)retpk.ReadLong();
		if(l_game ==ply_addr->game)
		{
			ply_array[i -l_notcount] = ply_addr;
		}else
		{
			l_notcount	++;
			continue;
		}

		try
		{ // 
			uLong tmp_id = ply_addr->m_dbid;
			if (tmp_id != db_id) // 
				continue;
		}catch (...)		// 
		{
			continue;
		}

		try
		{
			g_gtsvr->cli_conn->post_mapcrash_msg(ply_addr); // 
		}catch (...)
		{
			continue;
		}

		continue;
	}

	ply_cnt	-=l_notcount;
	ToLogService("GateServer", "because GameServer trouble, notice {} user offline", ply_cnt);
	for (int i = 0; i < ply_cnt; ++i)
	{
		try			//
		{
			ToLogService("GateServer", "because GameServer trouble, disconnect [{}] ", ply_array[i]->m_datasock->GetPeerIP());
			g_gtsvr->cli_conn->Disconnect(ply_array[i]->m_datasock,100,-29);
		}catch (...)
		{
		}
	}
}

WPacket ToGameServer::OnServeCall(DataSocket* datasock, RPacket &in_para)
{
	/*
	GameServer* l_game = (GameServer *)(datasock->GetPointer());
	WPacket l_retpk = GetWPacket();
	uShort l_cmd = in_para.ReadCmd();

	return l_retpk;
	*/

	return NULL;
}

void ToGameServer::OnProcessData(DataSocket* datasock, RPacket &recvbuf)
{
	GameServer* l_game = (GameServer *)(datasock->GetPointer());

	uShort l_cmd = recvbuf.ReadCmd();

	printf("Incoming from GameServer Packet CMD ID: %d\n", l_cmd);
	printf("Packet data size: %d bytes\n", recvbuf.GetDataLen());
	printf("Packet total size: %d bytes\n",recvbuf.GetPktLen());

	try
	{
		switch (l_cmd)
		{
		case CMD_MP_GM1SAY: {
			g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), WPacket(recvbuf));
			break;
		}
		case CMD_MT_LOGIN:
			MT_LOGIN(datasock, recvbuf);        
			break;
		case CMD_MT_SWITCHMAP:
			{
			MT_SWITCHMAP(datasock, recvbuf);
				break;
			}
		case CMD_MC_ENTERMAP:
			{
			MC_ENTERMAP(datasock, recvbuf);
				break;
			}
		case CMD_MC_STARTEXIT:
		{
			MC_STARTEXIT(datasock, recvbuf);
			break;
		}
		case CMD_MC_CANCELEXIT:
		{
			MC_CANCELEXIT(datasock, recvbuf);
			break;
		}
		case CMD_MT_PALYEREXIT:
		{
			MT_PALYEREXIT(datasock, recvbuf);
			break;
		}
		case CMD_MT_KICKUSER:
			{
			MT_KICKUSER(datasock, recvbuf);
				break;
			}
		case CMD_MT_MAPENTRY:
			{
			MT_MAPENTRY(datasock, recvbuf);
			}
			break;
		default:		// 
			{
				if(l_cmd/500 == CMD_MC_BASE/500)
				{
					RPacket	l_rpk		=recvbuf;
					uShort	l_aimnum	=l_rpk.ReverseReadShort();
					recvbuf.DiscardLast(sizeof(uLong)*2*l_aimnum + sizeof(uShort));
					for(uShort i=0;i<l_aimnum;i++)
					{
						auto l_ply = ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
						if (!l_ply)
						{
							continue;
						}
						if (l_ply->m_dbid == l_rpk.ReverseReadLong())
						{
							l_ply->SendPacketToClient(recvbuf);
						}
					}
				}else if(l_cmd/500 == CMD_MP_BASE/500)
				{
					RPacket l_rpk		=recvbuf;
					uShort	l_aimnum	=l_rpk.ReverseReadShort();
					recvbuf.DiscardLast(sizeof(uLong)*2*l_aimnum + sizeof(uShort));
					if( l_aimnum > 0 )
					{
						WPacket l_wpk,l_wpk0 =WPacket(recvbuf).Duplicate();
						for(uShort i=0;i<l_aimnum;i++)
						{
							auto l_ply	=ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
							if (!l_ply)
							{
								continue;
							}
							if(l_ply->m_dbid ==l_rpk.ReverseReadLong())
							{
								l_wpk			=l_wpk0;
								l_wpk.WriteLong(ToAddress(l_ply));
								l_wpk.WriteLong(l_ply->gp_addr);
								g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(),l_wpk);
							}
						}
					}
					else
					{
						WPacket l_wpk =WPacket(recvbuf).Duplicate();
						g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(),l_wpk);
					}
				}else if(l_cmd/500 ==CMD_MM_BASE/500)
				{
					for(GameServer *l_game =_game_list;l_game;l_game =l_game ->next)
					{
						g_gtsvr->gm_conn->SendData(l_game->m_datasock,recvbuf);
					}
				}
				break;
			}
		}
	}
	catch(...)
	{
		ToLogService("ToGameServerError", "l_cmd = {}", l_cmd);
	}
}

void ToGameServer::MT_LOGIN(DataSocket* datasock, RPacket& rpk)
{
	cChar* gms_name = rpk.ReadString();
	cChar* map_list = rpk.ReadString();
	GameServer* gms = _game_heap.Get();
	WPacket retpk = GetWPacket();
	bool valid = true;
	int i;

	retpk.WriteCmd(CMD_TM_LOGIN_ACK);
	int cnt = Util_ResolveTextLine(map_list, gms->maplist, MAX_MAP, ';', 0);
	ToLogService("GateServer", "recieve GameServer [{}] map string [{}] total [{}]", gms_name, map_list, cnt);
	if (cnt <= 0)
	{ // MAP
		ToLogService("GateServer", "map string [{}] has syntax mistake, please use ';'compart", map_list);
		retpk.WriteShort(ERR_TM_MAPERR);
		datasock->SetPointer(NULL);
		gms->Free();
	}else
	{
		gms->gamename = gms_name;
		gms->mapcnt = cnt;

		_mut_game.lock();
		try
		{
			do
			{ //  GameServer 
				if (_exist_game(gms_name))
				{
					ToLogService("GateServer", "the same name GameServer exsit: {}", gms_name);
					retpk.WriteShort(ERR_TM_OVERNAME);
					datasock->SetPointer(NULL);
					valid = false; break;
				}

				// 
				for (i = 0; i < cnt; ++ i)
				{
					if (find(gms->maplist[i].c_str()) != NULL)
					{
						ToLogService("GateServer", "the same name MAP exsit: {}", gms->maplist[i].c_str());
						retpk.WriteShort(ERR_TM_OVERMAP);
						datasock->SetPointer(NULL);
						valid = false;
						break;
					}
				}
			} while (false);

			if (valid)
			{ //  GameServer 
				_add_game(gms); // 
				ToLogService("GateServer", "add GameServer [{}] ok", gms_name);
				for (i = 0; i < cnt; ++ i) //  map 
				{
					ToLogService("GateServer", "add [{}]  [{}] map ok", gms_name, gms->maplist[i].c_str());
					_map_game[gms->maplist[i]] = gms;
				}

				datasock->SetPointer(gms);
				gms->m_datasock	= datasock;
				retpk.WriteShort(ERR_SUCCESS);
				retpk.WriteString(g_gtsvr->gp_conn->_myself.c_str());
			}
			else
			{ //  GateServer
				gms->Free();
			}
		}
		catch (...)
		{
			ToLogService("GateServer", "Exception raised from MT_LOGIN{{add map}}");
		}
		_mut_game.unlock();

		//if (!valid) Disconnect(datasock);
	}

	SendData(datasock, retpk);
}

void ToGameServer::MT_SWITCHMAP(DataSocket* datasock, RPacket& recvbuf)
{
	RPacket	l_rpk = recvbuf;
	uShort	l_aimnum = l_rpk.ReverseReadShort();		//l_aimnum1

	auto l_ply = ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
	if (l_ply->m_dbid != l_rpk.ReverseReadLong())					//chaid
	{
		return;
	}
	uChar	l_return = l_rpk.ReverseReadChar();
	recvbuf.DiscardLast(sizeof(uChar) + sizeof(uLong) * 2 * l_aimnum + sizeof(uShort));

	cChar* l_srcmap = l_rpk.ReadString();
	Long	lSrcMapCopyNO = l_rpk.ReadLong();
	//...
	uLong	l_srcx = l_rpk.ReadLong();	//
	uLong	l_srcy = l_rpk.ReadLong();	//

	cChar* l_map = l_rpk.ReadString();
	Long	lMapCopyNO = l_rpk.ReadLong();
	//...
	uLong	l_x = l_rpk.ReadLong();	//
	uLong	l_y = l_rpk.ReadLong();	//
	GameServer* l_game = g_gtsvr->gm_conn->find(l_map);

	if (l_game)
	{
		l_ply->game->m_plynum--;
		l_ply->game = nullptr;
		l_ply->gm_addr = 0;
		ToLogService("GateServer", "clinet: {}:{}	Switch to map,to Gate[{}]send EnterMap command,dbid:{},Gate address:{:X}", l_ply->m_datasock->GetPeerIP(), l_ply->m_datasock->GetPeerPort(), l_game->m_datasock->GetPeerIP(), l_ply->m_dbid, ToAddress(l_ply));
		l_game->EnterMap(l_ply, l_ply->m_loginID, l_ply->m_dbid, l_ply->m_worldid, l_map, lMapCopyNO, l_x, l_y, 1, l_ply->m_sGarnerWiner);	
		l_game->m_plynum++;
	}
	else if (!l_return)
	{
		WPacket	l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_SYSINFO);
		l_wpk.WriteString((std::string("[") + l_map + std::string("] can't reach, pealse retry later!")).c_str());
		l_ply->m_datasock->SendData(l_wpk);

		ToLogService("GateServer", "clinet: {}:{}	Switch back map,to Gate[{}]send EnterMap command,dbid:{},Gate address:{:X}", l_ply->m_datasock->GetPeerIP(), l_ply->m_datasock->GetPeerPort(), l_ply->game->m_datasock->GetPeerIP(), l_ply->m_dbid, ToAddress(l_ply));
		l_ply->game->EnterMap(l_ply, l_ply->m_loginID, l_ply->m_dbid, l_ply->m_worldid, l_srcmap, lSrcMapCopyNO, l_srcx, l_srcy, 1, l_ply->m_sGarnerWiner);
	}
	else
	{
		g_gtsvr->cli_conn->Disconnect(l_ply->m_datasock, 0, -24);
	}
}

void ToGameServer::MT_MAPENTRY(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	WPacket l_wpk0 = WPacket(recvbuf).Duplicate();
	WPacket l_wpk = l_wpk0;

	RPacket	l_rpk = recvbuf;
	cChar* l_map = l_rpk.ReadString();
	GameServer* l_game = g_gtsvr->gm_conn->find(l_map);
	if (l_game)
	{
		l_wpk.WriteCmd(CMD_TM_MAPENTRY);
		g_gtsvr->gm_conn->SendData(l_game->m_datasock, l_wpk);
	}
	else
	{
		l_wpk.WriteCmd(CMD_TM_MAPENTRY_NOMAP);
		g_gtsvr->gm_conn->SendData(datasock, l_wpk);
	}
}

void ToGameServer::MC_ENTERMAP(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	RPacket	l_rpk = recvbuf;
	const auto l_aimnum = l_rpk.ReverseReadShort();					//l_aimnum1

	auto game = static_cast<GameServer*>(datasock->GetPointer());
	auto l_ply = ToPointer<ClientConnection>(l_rpk.ReverseReadLong());
	if (!l_ply)
	{
		ToLogService("Error", "l_ply nullptr in " __FUNCTION__ );
		return;
	}

	const auto	l_dbid = l_rpk.ReverseReadLong();
	if (l_ply->m_dbid != l_dbid)					//chaid
	{
		ToLogService("GateServer", "recieve from [{}] EnterMap command ,can't match DBID:locale [{}],far[{}],Gate address:{:X}", datasock->GetPeerIP(), l_ply->m_dbid, l_dbid, ToAddress(l_ply));
		return;
	}

	const auto l_retcode = l_rpk.ReadShort();

	if (l_retcode != ERR_SUCCESS)
	{
		l_ply->m_status = ClientConnection::Status::CharacterSelection;
		--game->m_plynum;
		ToLogService("GateServer", "clinet: {}:{}	Gate recieve from [{}]failed EnterMap command ,Error:{}", l_ply->m_datasock->GetPeerIP(), l_ply->m_datasock->GetPeerPort(), datasock->GetPeerIP(), l_retcode);

		recvbuf.DiscardLast(sizeof(uShort) + sizeof(uLong) * 2 * l_aimnum);
		//g_gtsvr->cli_conn->SendData(l_ply->m_datasock,recvbuf);
		g_gtsvr->cli_conn->Disconnect(l_ply->m_datasock, 10, -33);
		return;
	}

	l_ply->game = static_cast<GameServer*>(datasock->GetPointer());
	l_ply->gm_addr = l_rpk.ReverseReadLong();
	game->m_plynum = l_rpk.ReverseReadLong();
	const auto l_isSwitch = l_rpk.ReverseReadChar();

	ToLogService("GateServer", "clinet: {}:{}	recieve Gate  from [{}]success EnterMap command,Game address:{:X},Gate address:{:X}", l_ply->m_datasock->GetPeerIP(), l_ply->m_datasock->GetPeerPort(), datasock->GetPeerIP(), (LONG)l_ply->gm_addr, ToAddress(l_ply));

	recvbuf.DiscardLast(sizeof(uShort) + sizeof(uLong) * 2 * l_aimnum + sizeof(uLong) * 2 + sizeof(uChar));

	l_ply->SendPacketToClient(recvbuf);

	{
		WPacket l_wpk = GetWPacket();
		l_wpk.WriteCmd(CMD_MP_ENTERMAP);
		l_wpk.WriteChar(l_isSwitch);
		l_wpk.WriteLong(ToAddress(l_ply));
		l_wpk.WriteLong(l_ply->gp_addr);
		g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), l_wpk);
	}
}

void ToGameServer::MC_STARTEXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	RPacket	l_rpk = recvbuf;
	uShort	l_aimnum = l_rpk.ReverseReadShort();					//l_aimnum1
	auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
	if (l_ply)
	{
		g_gtsvr->cli_conn->SendData(l_ply->m_datasock, recvbuf);
	}
}

void ToGameServer::MC_CANCELEXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	RPacket	l_rpk = recvbuf;
	uShort	l_aimnum = l_rpk.ReverseReadShort();					//l_aimnum1
	auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
	if (l_ply)
	{
		auto const l_lockStat = std::lock_guard{ l_ply->m_mtxstat };
		if (!l_ply->m_datasock->IsFree() && l_ply->m_exit != ClientConnection::Status::None)
		{
			l_ply->m_exit = ClientConnection::Status::Invalid;
			g_gtsvr->cli_conn->SendData(l_ply->m_datasock, recvbuf);
		}
	}
}

void ToGameServer::MT_PALYEREXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	uShort	l_aimnum = recvbuf.ReverseReadShort();
	for (uShort i = 0; i < l_aimnum; i++)
	{
		auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
		if (l_ply && l_ply->m_dbid == recvbuf.ReverseReadLong())
		{
			auto const l_lockStat = std::lock_guard{ l_ply->m_mtxstat };
			uLong	l_ulMilliseconds = 30 * 1000;
			if (l_ply->m_status == ClientConnection::Status::Playing && l_ply->m_exit == ClientConnection::Status::CharacterSelection) // 
			{
				WPacket l_wpk = g_gtsvr->gp_conn->GetWPacket();
				l_wpk.WriteCmd(CMD_TP_ENDPLAY);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);
				l_wpk = g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), l_wpk, l_ulMilliseconds);
				if (!l_wpk.HasData())
				{
					l_wpk = datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_ENDPLAY);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					//SendData(datasock,l_wpk);
					g_gtsvr->cli_conn->SendData(l_ply->m_datasock, l_wpk);
				}
				else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_ENDPLAY);
					//SendData(datasock,l_wpk);
					g_gtsvr->cli_conn->SendData(l_ply->m_datasock, l_wpk);
					l_ply->m_status = ClientConnection::Status::CharacterSelection;
					l_ply->m_exit = ClientConnection::Status::Invalid;
					l_ply->m_dbid = 0;
					l_ply->m_worldid = 0;
					if (RPacket(l_wpk).ReadShort() == ERR_PT_KICKUSER)
					{
						Disconnect(l_ply->m_datasock, 100, -25);
					}
				}
			}
			else if (l_ply->m_status == ClientConnection::Status::Playing && l_ply->m_exit == ClientConnection::Status::Playing) // 
			{
				/*
				WPacket l_wpk = g_gtsvr->gp_conn->GetWPacket();
				l_wpk.WriteCmd( CMD_TP_PLAYEREXIT );
				l_wpk.WriteLong( l_ply->m_dbid );
				g_gtsvr->gp_conn->SendData( g_gtsvr->gp_conn->get_datasock(),l_wpk);
				*/
				WPacket l_wpk = g_gtsvr->gp_conn->get_datasock()->GetWPacket();
				l_wpk.WriteCmd(CMD_TP_DISC);
				l_wpk.WriteLong(l_ply->m_actid);
				l_wpk.WriteLong(inet_addr(datasock->GetPeerIP()));
				l_wpk.WriteString(GetDisconnectErrText(datasock->GetDisconnectReason() ? datasock->GetDisconnectReason() : -27).c_str());
				g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), l_wpk);

				l_wpk = g_gtsvr->gp_conn->get_datasock()->GetWPacket();
				l_wpk.WriteCmd(CMD_TP_USER_LOGOUT);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);
				l_ply->gp_addr = 0;
				RPacket l_retpk = g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), l_wpk, l_ulMilliseconds);
				g_gtsvr->cli_conn->Disconnect(l_ply->m_datasock, 0, -23);	//23GameServer
				l_ply->Free();
			}
		}
	}
}

void ToGameServer::MT_KICKUSER(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
{
	uShort l_aimnum = recvbuf.ReverseReadShort();
	auto l_ply = ToPointer<ClientConnection>(recvbuf.ReverseReadLong());
	uLong b = recvbuf.ReverseReadLong();
	if (l_ply && l_ply->m_dbid == b)
	{
		g_gtsvr->cli_conn->Disconnect(l_ply->m_datasock, 0, -23);
	}
}

GameServer* ToGameServer::find(cChar* mapname)
{
	map<string, GameServer*>::iterator it = _map_game.find(mapname);
	if (it == _map_game.end())
	{
		ToLogService("GateServer", "not found [{}] map!!!", mapname);
		return NULL;
	}else
		return (*it).second;
}

void GameServer::Initially()
{
	m_plynum = 0;
	gamename = "";
	ip = "";
	port = 0;
	m_datasock = nullptr;
	next = nullptr;
	mapcnt = 0;
}

void GameServer::Finally()
{
	m_plynum = 0;
	gamename = "";
	ip = "";
	port = 0;
	m_datasock = nullptr;
	next = nullptr;
	mapcnt = 0;
}

void GameServer::EnterMap(ClientConnection *ply,uLong actid, uLong dbid,uLong worldid,cChar *map, Long lMapCpyNO,uLong x,uLong y,char entertype,short swiner)
{
	WPacket l_wpk	=m_datasock->GetWPacket();
	l_wpk.WriteCmd(CMD_TM_ENTERMAP);
    l_wpk.WriteLong(actid);
	l_wpk.WriteString(ply->m_password);
	l_wpk.WriteLong(dbid);
	l_wpk.WriteLong(worldid);
	l_wpk.WriteString(map);
	l_wpk.WriteLong(lMapCpyNO);
	l_wpk.WriteLong(x);
	l_wpk.WriteLong(y);
	l_wpk.WriteChar(entertype);
	l_wpk.WriteLong(ToAddress(ply));		//
	l_wpk.WriteShort(swiner);
	g_gtsvr->gm_conn->SendData(m_datasock,l_wpk);
	ply->SetMapName(map); // Chaos Blind
}
