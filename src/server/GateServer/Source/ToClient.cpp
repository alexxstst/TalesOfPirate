
#include "gateserver.h"
using namespace dbc;
using namespace std;

ToClient::ToClient(char const* fname, ThreadPool* proc, ThreadPool* comm)
	: TcpServerApp(this, proc, comm, false), RPCMGR(this), m_maxcon(500), m_atexit(0), m_calltotal(0), m_lastConnect(0)
{
	IniFile inf(fname);
	m_maxcon			= std::stoi(inf["ToClient"]["MaxConnection"]); //Modify by lark.li

	m_version		= std::stoi(inf["Main"]["Version"]);
	IniSection& is = inf["ToClient"];
	const std::string ip = is["IP"]; uShort port = std::stoi(is["Port"]);
	_comm_enc = std::stoi(is["CommEncrypt"]);
	printf("Current client version is %d\n", m_version);
	// DDOS Protection begins
	if (g_ddos)
	{
		m_checkSpan = std::stoi(is["CheckSpan"]);
		m_checkWaring = std::stoi(is["CheckWaring"]);
		m_checkError = std::stoi(is["CheckError"]);

		if(m_checkSpan < 1)
			m_checkSpan = 1;

		if(m_checkWaring < (5 * m_checkSpan))
			m_checkWaring = (5 * m_checkSpan);

		if(m_checkError < (7 * m_checkSpan))
			m_checkError = (7 * m_checkSpan);

		// DDOS Protection end
	}
	SetPKParse(0, 2, 16 * 1024, 100); BeginWork(std::stoi(is["EnablePing"]),1);
	if (OpenListenSocket(port, ip.c_str()) != 0)
		THROW_EXCP(excp, "ToClient listen failed\n");
	
}

ToClient::~ToClient()
{
	m_atexit	=1;
	while(m_calltotal)
	{
		Sleep(1);
	}
	ShutDown(12 * 1000);
}


void ToClient::SetCheckSpan(uShort checkSpan)
{
	if(checkSpan > 1)
	{
		if(m_checkError < (7 * checkSpan))
			m_checkError = (7 * checkSpan);

		if(m_checkWaring < (5 * checkSpan))
			m_checkWaring = (5 * checkSpan);

		m_checkSpan = checkSpan;
	}
}

void ToClient::SetCheckWaring(uShort checkWaring)
{
	if(checkWaring > (5 * m_checkSpan) && checkWaring < m_checkError)
	{
		m_checkWaring = checkWaring;
	}
}

void ToClient::SetCheckError(uShort checkError)
{
	if(checkError > (7 * m_checkSpan))
		m_checkError = checkError;
}

bool ToClient::DoCommand(DataSocket* datasock, cChar *cmdline)
{
	/*
		if(!strncmp(cmdline,"getbandwidth",sizeof("getbandwidth")))
		{
			BandwidthStat	l_band	=GetBandwidthStat();

			char buffer[255];
			sprintf(buffer,RES_STRING(GS_TOCLIENT_CPP_00002),GetSockTotal(),l_band.m_sendpktps,l_band.m_sendpkts,l_band.m_sendbyteps/1024,l_band.m_sendbytes/1024,
						l_band.m_recvpktps, l_band.m_recvpkts, l_band.m_recvbyteps/1024, l_band.m_recvbytes/1024);

			WPacket l_wpk	=GetWPacket();
			l_wpk.WriteCmd(CMD_MC_SYSINFO);
			//l_wpk.WriteString(l_output.c_str());
			l_wpk.WriteString(buffer);
			SendData(datasock,l_wpk);
			return true;
		}
	*/

	return false;
}
bool ToClient::OnConnect(DataSocket* datasock)
{
	if (!g_gtsvr->gp_conn->IsReady()) {
		ToLogService("GateServer", "client: {}	come, groupserver is't ready, force disconnect...", datasock->GetPeerIP());
		Disconnect(datasock, 0, DS_DISCONN);
		return false;
	}

	// DDOS Protection ToClient:OnConnect
	// Won't work with alternating IPs!!
	// Might work with distributed DoS if it is not set up correctly.
	// In the future, rework LIBDBC to remove datasocket entries if the same IP is found in more than X entries.
	// Since OnConnected/AddPK are only called if OnConnect returns true (Comm.cpp), returning false here will be enough to avoid resource consumption on RSA.
	if (g_ddos)
	{

		if (std::find(blacklistedIP.begin(), blacklistedIP.end(), datasock->GetPeerIP()) != blacklistedIP.end()) {
			// Check for blacklisted IPs, then disconnect them. Not the ideal way, should get rid of them in PKQueue.
			this->Disconnect(datasock, 0, -31);
			return false;
		}
		uLong l_tick = GetCurrentTick();
		if (l_tick - m_lastConnect < 1000 && !strcmp(datasock->GetPeerIP(), (const char*) (m_lastSockIP)))
		{
			// If the last connection attempt on Gateserver happened less than 1000ms ago and the IP is the same, increment the counter.
			m_cmdNum++;

			
			auto DisconnectDDOSer = [&] {
				printf("[%s] DoS suspected... ", datasock->GetPeerIP());
				C_PRINT("disconnected!\n");
				this->Disconnect(datasock, 0, -31);
				ToLogService("GateServer", "client: {}is suspect of DoS'ing, disconnected", datasock->GetPeerIP());
				blacklistedIP.push_back(datasock->GetPeerIP());
				
			};
			// If the counter is above 5, disconnect and add to the blacklist.
			if (m_cmdNum > 5 ||
				datasock->m_recvbyteps > 1024)
			{
				DisconnectDDOSer();
				return false;

			}
			// If the counter is above 2, just add a warning.
			else if (m_cmdNum > 2)
			{
				m_Warning++;
				// If it has more than 3 warnings, disconnect and add to the blacklist
				if (m_Warning++ > 3)
				{
					DisconnectDDOSer();
					return false;
				}
			}
		}
		else
		{
			// Reset warnings
			if (m_cmdNum < 5)
			{
				m_Warning = 0;
			}
			// Reset counters, update the last connection time and the last PeerIP
			m_cmdNum = 0;
			m_lastConnect = l_tick;
			m_lastSockIP = datasock->GetPeerIP();
		}
	
	}
	if(GetSockTotal()<=m_maxcon)
	{
		
		datasock->SetRecvBuf(64 * 1024); datasock->SetSendBuf(64 * 1024);
		ToLogService("GateServer", "client: {}	come...Socket num: {}", datasock->GetPeerIP(), GetSockTotal() + 1);
		return true;
	}
	else
	{
		
		ToLogService("GateServer", "client: {}	come, greater than {} player, force disconnect...", datasock->GetPeerIP(), m_maxcon);
		return false;
	}
}
void ToClient::OnConnected(DataSocket* datasock)
{
	ClientConnection* l_ply = g_gtsvr->client_heap.Get();
	if (!l_ply) {
		printf("error: poor mem %s!\n", datasock->GetPeerIP());
		Disconnect(datasock);
		return;
	}

	if(!l_ply->InitReference(datasock))
	{
		printf( "warning: forbid %s repeat connect !", datasock->GetPeerIP() );
		l_ply->Free();
		Disconnect(datasock);
		return;
	}

	if (g_ddos)
	{
		datasock->m_pktn = 0;
		datasock->m_pktn2 = 'H';
	}
	if (g_rsaaes)	// RSA-AES Network Encryption by Mdr - CO 2021
	{
		try
		{
			////if (datasock->IsServer()) return;
			l_ply->srvPrivateKey.GenerateRandomWithKeySize(l_ply->rng, 3072);
			CryptoPP::RSA::PublicKey srvPublicKey(l_ply->srvPrivateKey);

			l_ply->handshakeDone = false;
			if (!l_ply->srvPrivateKey.Validate(l_ply->rng, 2) || !srvPublicKey.Validate(l_ply->rng, 2))
			{
				printf("The generated key is invalid!\n");
				Disconnect(datasock, 65, -27);
				return;
			}

			//https://stackoverflow.com/questions/32401929/sending-publickey-within-packet-payload

			std::string publickey;
			std::string base64encoded_public;
			CryptoPP::StringSink sink(publickey);
			srvPublicKey.Save(sink);
			CryptoPP::StringSource encoded(publickey, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64encoded_public)));
			WPacket l_wpk = GetWPacket();
			l_wpk.WriteCmd(CMD_MC_SEND_SERVER_PUBLIC_KEY);
			l_wpk.WriteShort(publickey.size());
			l_wpk.WriteSequence(reinterpret_cast<const char*>(publickey.data()), publickey.size()); // Send the public key as a const char sequence																								
			SendData(datasock, l_wpk);
			C_PRINT("[%s]", datasock->GetPeerIP());
			printf("Public RSA sent\n");
		}
		catch (std::exception const& e)
		{
			ToLogService("ToClientOnConnected", "{}", e.what());
			Disconnect(datasock);
		}
		return;
	}
}
void ToClient::OnDisconnect(DataSocket* datasock, int reason)
{
	ToLogService("GateServer", "client: {} gone...Socket num: {} ,reason={}", datasock->GetPeerIP(), GetSockTotal(), GetDisconnectErrText(reason).c_str());

	RPacket l_rpk = 0;
	CM_LOGOUT(datasock,l_rpk);
}

std::string	ToClient::GetDisconnectErrText(int reason) const
{
	switch(reason)
	{
		case -21:return RES_STRING(GS_TOCLIENT_CPP_00011);
		case -23:return RES_STRING(GS_TOCLIENT_CPP_00012);
		case -24:return	RES_STRING(GS_TOCLIENT_CPP_00013);
		case -25:return RES_STRING(GS_TOCLIENT_CPP_00014);
		case -27:return RES_STRING(GS_TOCLIENT_CPP_00015);
		case -29:return RES_STRING(GS_TOCLIENT_CPP_00016);
		case -31:return RES_STRING(GS_TOCLIENT_CPP_00017);
		case -32:return RES_STRING(GS_TOCLIENT_CPP_00019);
		case -33:return RES_STRING(GS_TOCLIENT_CPP_00020);
		default:return TcpServerApp::GetDisconnectErrText(reason);
	}
}

WPacket ToClient::OnServeCall(DataSocket* datasock, RPacket &in_para)
{
	uShort	l_cmd		=in_para.ReadCmd();

	switch (l_cmd)
	{
	case CMD_CM_LOGOUT:
		{
			CM_LOGOUT(datasock,in_para);
			Disconnect(datasock,65,-27);
			return	0;
		}
	default:
		{
			break;
		}
	}
	return 0;
}


void ToClient::OnProcessData(DataSocket* datasock, RPacket &recvbuf)
{
	uShort l_cmd;
	DWORD counter{ 0 };
	char counter2{ 0 };
	try
	{
		// WPE PROTECTION
		if (g_wpe)
		{
			l_cmd = recvbuf.ReadCmd();
			counter = recvbuf.ReadLong(); // counter from client (4 bytes)
			if (counter != datasock->m_pktn)
			{
				C_PRINT("[%s]", datasock->GetPeerIP());
				printf("WPE detected! Packet counter: %d, expected: %d, CMD:%d", counter, datasock->m_pktn, l_cmd);
				C_PRINT("- Player disconnected!\n");
				Disconnect(datasock, 100, 0xFFFFFFE);
				return;
			}
			datasock->m_pktn++;

			if (recvbuf.GetDataLen() < 4)
			{
				ToLogService("DATA_LEN", "Packet sent from {} smaller than 4", datasock->m_peerip);
				return;
			}

			if (recvbuf.GetPktLen() < 4)
			{
				ToLogService("PKT_LEN", "Packet sent from {} smaller than 4", datasock->m_peerip);
				return;
			}
	
			char* buf = (char*)recvbuf.GetBuffer();
			memmove( buf + (recvbuf.GetOffset() + 2) + recvbuf.GetHead(),
					buf + (recvbuf.GetHead() + 6) + recvbuf.GetOffset(),
					recvbuf.GetDataLen()-4);
			recvbuf.SetPktLen(recvbuf.GetPktLen() - 4);
			recvbuf.SetPktPos(0);
			recvbuf.SetPktRevPos(0);
		}
		// reads the command
		l_cmd = recvbuf.ReadCmd();
		C_PRINT("[%s]", datasock->GetPeerIP());
		printf("Packet CMD ID: %d, counter:%d \n", l_cmd , datasock->m_pktn);
		C_PRINT("[%s]", datasock->GetPeerIP());
		printf("Packet data size: %d bytes\n",recvbuf.GetDataLen());
		C_PRINT("[%s]", datasock->GetPeerIP());
		printf("Packet total size: %d bytes\n",recvbuf.GetPktLen());


		// DDOS Protection
		if (g_ddos && (l_cmd != CMD_CM_BEGINACTION && l_cmd != CMD_CM_ENDACTION))
		{
			//Player *l_ply = (Player*)datasock->GetPointer();
			//if (l_ply)
			//{
				uLong l_tick = GetCurrentTick();
				if (l_tick - datasock->m_lashTick < 1000 * m_checkSpan)
				{
					++datasock->m_cmdNum;

					auto DisconnectDDOSer = [&] {
						printf("[%s] ddos suspected... ", datasock->GetPeerIP());
						dbc::WPacket l_wpk = GetWPacket();
						l_wpk.WriteCmd(CMD_MC_LOGIN);
						l_wpk.WriteShort(ERR_MC_NETEXCP);
						SendData(datasock, l_wpk);

						C_PRINT("disconnected!\n");
						this->Disconnect(datasock, 100, -31);
					};

					if (datasock->m_cmdNum > m_checkError ||
						datasock->m_recvbyteps > 1024)
					{
						DisconnectDDOSer();
					}
					else if (datasock->m_cmdNum > m_checkWaring)
					{
						++datasock->m_waringNum;
						if (datasock->m_waringNum > 2)
						{
							DisconnectDDOSer();
						}
					}
				}
				else
				{
					if (datasock->m_cmdNum < m_checkWaring)
					{
						datasock->m_waringNum = 0;
					}

					datasock->m_cmdNum = 0;
					datasock->m_lashTick = l_tick;
				}
			//}
		}

		ToLogService("ToClientOnProcessData", "cmd: {}", l_cmd);

		switch (l_cmd)
		{

		case CMD_CM_SEND_PRIVATE_KEY:
		{

			auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
			if (!l_ply) return;

			CryptoPP::RSAES_OAEP_SHA_Decryptor d(l_ply->srvPrivateKey);

			// Let's read the cipher (base64encoded and encrypted client private key)
			std::string cipher_private = recvbuf.ReadString();

			// Base64Decode and Decrypt client private key using server private key.
			std::string base64decoded_private;
			std::string clientPrivateKey;

			try 
			{
				CryptoPP::StringSource a(cipher_private, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(base64decoded_private)));
				CryptoPP::StringSource b(base64decoded_private, true, new CryptoPP::PK_DecryptorFilter(l_ply->rng, d, new CryptoPP::StringSink(clientPrivateKey)));

				// Copy AES key to SecureByteBlock
				l_ply->cliPrivateKey.Assign((CryptoPP::byte*)clientPrivateKey.data(), CryptoPP::AES::MIN_KEYLENGTH);
			}
			catch (std::exception const& e)
			{
				ToLogService("SEND_PRIVATE_KEY", "{}", e.what());
				Disconnect(datasock);
				return;
			}

			// Clear the key
			l_ply->srvPrivateKey = {};

#ifdef _DEBUG
			cout << "Key exchange process completed for " << datasock->GetPeerIP() << endl;
#endif
			l_ply->handshakeDone = true;
			break;
		}


		case CMD_CM_LOGIN:		// /,.
		case CMD_CM_LOGOUT:		//
		case CMD_CM_BGNPLAY:	//GroupServerGameServer.
		case CMD_CM_ENDPLAY:
		case CMD_CM_NEWCHA:
		case CMD_CM_DELCHA:
		case CMD_CM_CREATE_PASSWORD2:
		case CMD_CM_UPDATE_PASSWORD2:
		case CMD_CM_REGISTER:
		case CMD_CP_CHANGEPASS:
		
			{
				++m_calltotal;
				if(m_atexit)
				{
					--m_calltotal;
					return;
				}
				TransmitCall * l_tc	=g_gtsvr->m_tch.Get();
				l_tc->Init(datasock,recvbuf);
				GetProcessor()->AddTask(l_tc);
			}
			break;
		case CMD_CP_PING:
		{
			auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
			if (l_ply && l_ply->gp_addr && l_ply->gm_addr)
			{
				WPacket l_wpk = GetWPacket();
				l_wpk.WriteCmd(CMD_CP_PING);
				l_wpk.WriteLong(GetTickCount() - l_ply->m_pingtime);
				l_ply->m_pingtime = 0;

				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);
				g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), l_wpk);
			}
		}
		break;
		case CMD_CM_SAY:
			{
			auto player = static_cast<ClientConnection*>(datasock->GetPointer());
			if (!player)
			{
				// The packet can't be coming from an actual player,
				// lets not process it further
				ToLogService("ErrServer", "CMD_CM_SAY: invalid player.");
				break;
			}

			cChar* l_str = recvbuf.ReadString();
			if (!l_str)
			{
				break;
			}
			if (*l_str == '&' && DoCommand(datasock, l_str + 1))
			{
				break;
			}
			if (strstr(l_str, "#21"))
			{
				break;
			}
		
			if (player->m_estop)
			{
				if (GetTickCount() - player->m_lestoptick >= 1000 * 60 * 2)
				{
					WPacket l_wpk = GetWPacket();
					l_wpk.WriteCmd(CMD_TP_ESTOPUSER_CHECK);
					l_wpk.WriteLong(player->m_actid);

					g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), l_wpk);
				}
				WPacket l_wpk = GetWPacket();
				l_wpk.WriteCmd(CMD_MC_SYSINFO);
				//l_wpk.WriteString("");
				l_wpk.WriteString(RES_STRING(GS_TOCLIENT_CPP_00018));
				g_gtsvr->gp_conn->SendData(player->m_datasock, l_wpk);
				break;
			}

			player->SendPacketToGameServer(recvbuf);
		}
		break;
		case CMD_CM_OFFLINE_MODE:
		{
			auto const lock = std::lock_guard{g_gtsvr->_mtxother};

			auto player = static_cast<ClientConnection*>(datasock->GetPointer());
			if (!player)
			{
				break;
			}

			if (!player->game)
			{
				break;
			}

			auto const lock_stat = std::lock_guard{player->m_mtxstat};
			if (player->m_status != ClientConnection::Status::Playing)
			{
				break;
			}

			auto wpk = GetWPacket();
			wpk.WriteCmd(CMD_TM_OFFLINE_MODE);
			wpk.WriteLong(ToAddress(player));
			wpk.WriteLong(player->gm_addr);


			auto rpk = SyncCall(player->game->m_datasock, wpk);
			if (!rpk.HasData())
			{
				// Don't even acknowledge the client request
				break;
			}

			switch (const auto return_code = static_cast<ReturnCode::OfflineMode>(rpk.ReadChar()))
			{
			case ReturnCode::OfflineMode::Success:
			{
				try {
					/* wpk = g_gtsvr->gp_conn->get_datasock()->GetWPacket();
					wpk.WriteCmd(CMD_TP_USER_LOGOUT);
					wpk.WriteLong(ToAddress(player));
					wpk.WriteLong(player->gp_addr);
					player->gp_addr = 0;
					g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), wpk, 30);
					*/
					player->EndRun();
					TC_DISCONNECT(player->m_datasock, -33);
				}
				catch (...)
				{
					ToLogService("GateServer", "Error offline mode!");
				}

				// Free in the end, or we invalidate data we need in player
				player->m_datasock = nullptr;
				datasock->SetPointer(nullptr);
				player->Free();
			}
			break;
			case ReturnCode::OfflineMode::Disabled:
			{
				player->SendSysInfo("Offline mode is disabled in this map.");
			}
			break;
			default:
			{
				player->SendSysInfo("Something went wrong trying to use offline mode.");
				ToLogService("ErrServer", "Offline stall failed for a player, return code: {}", static_cast<int>(return_code));
			}
			break;
			}
		}
		break;

		default:		//GroupServerGameServer
			if(l_cmd/500 == CMD_CM_BASE/500)
			{//GameServer
				auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
				if (l_ply)
				{
					l_ply->SendPacketToGameServer(recvbuf);
				}
			}
			else if(l_cmd/500 == CMD_CP_BASE/500)
			{//GroupServer
				auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
				if(l_ply)
				{
					if (l_cmd == CMD_CP_SAY2TRADE ||
						l_cmd == CMD_CP_SAY2ALL ||
						l_cmd == CMD_CP_SAY2YOU ||
						l_cmd == CMD_CP_SAY2GUD ||
						l_cmd == CMD_CP_SESS_SAY)
					{
						IniFile inf("GateServer.cfg");
						if (std::stoi(inf["Chaos"]["IsActive"]))
						{
							const char* chamap = l_ply->GetMapName();
							if (strcmp(chamap, inf["Chaos"]["Map"].c_str()) == 0)
							{
								WPacket	b_wpk	=datasock->GetWPacket();
								b_wpk.WriteCmd(CMD_MC_SYSINFO);
								const char* msg = "Unable to chat in this map!";
								b_wpk.WriteSequence(msg, uShort(strlen(msg))+1);
								g_gtsvr->cli_conn->SendData(l_ply->m_datasock, b_wpk);
								return;
							}
						}
					}
					l_ply->SendPacketToGroupServer(recvbuf);
				}
			}
			break;
		}
		static uLong l_last	=GetTickCount();
		if(datasock->m_recvbyteps>1024*5)
		{
			uLong	l_tick		=GetCurrentTick();
			if(l_tick -l_last >1000*30)
			{
				l_last	=l_tick;
				std::cout<<"["<<datasock->GetPeerIP()<<"] sending big packet (>5K/s) attack server,please deal with!\n";
				ToLogService("AttackMonitor", "[{}] sending big packet (>5K/s) attack server,please deal with!", datasock->GetPeerIP());
			}
		}
	}
	catch (const CryptoPP::BERDecodeErr& ex)
	{
		cerr << ex.what() << endl;
	}
	catch (const CryptoPP::CryptoMaterial::InvalidMaterial& ex)
	{
		cerr << ex.what() << endl;
	}
	catch(...)
	{
		ToLogService("ToClientError", "l_cmd = {}", l_cmd);
	}
	ToLogService("ToClient", "<--l_cmd = {}", l_cmd);
	return;
}
long TransmitCall::Process()
{
	if(!g_gtsvr->gp_conn->IsReady())
	{
		g_gtsvr->cli_conn->Disconnect(m_datasock,50,-27);
		ToLogService("GateServer", "IsReady = false");
		return 0;
	}

	uShort l_cmd = m_recvbuf.ReadCmd();

	//l_line<<newln<<"st:"<<l_cmd;

	try
	{
		switch(l_cmd)
		{
		case CMD_CM_LOGIN:		// /,.
			g_gtsvr->cli_conn->CM_LOGIN(m_datasock, m_recvbuf);
			break;
		case CMD_CM_LOGOUT:		//
			g_gtsvr->cli_conn->CM_LOGOUT(m_datasock,m_recvbuf);
			g_gtsvr->cli_conn->Disconnect(m_datasock,50,-27);
			break;
		case CMD_CM_BGNPLAY:	//GroupServerGameServer.
			g_gtsvr->cli_conn->CM_BGNPLAY(m_datasock, m_recvbuf);
			break;
		case CMD_CM_ENDPLAY:
			g_gtsvr->cli_conn->CM_ENDPLAY(m_datasock, m_recvbuf);
			break;
		case CMD_CM_NEWCHA:
			g_gtsvr->cli_conn->CM_NEWCHA(m_datasock, m_recvbuf);
			break;
		case CMD_CM_DELCHA:
			g_gtsvr->cli_conn->CM_DELCHA(m_datasock, m_recvbuf);
			break;
		case CMD_CM_CREATE_PASSWORD2:
			g_gtsvr->cli_conn->CM_CREATE_PASSWORD2(m_datasock, m_recvbuf);
			break;
		case CMD_CM_UPDATE_PASSWORD2:
			g_gtsvr->cli_conn->CM_UPDATE_PASSWORD2(m_datasock, m_recvbuf);
			break;
		case CMD_CM_REGISTER:
			g_gtsvr->cli_conn->CM_REGISTER(m_datasock, m_recvbuf);
			break;
		case CMD_CP_CHANGEPASS:
			g_gtsvr->cli_conn->CP_CHANGEPASS(m_datasock, m_recvbuf);
			break;
		}
	}
	catch (std::exception& e) {
		ToLogService("GateServer", "exception: {}", e.what());
		ToLogService("GateServer", "IsReady = false exception:{}", l_cmd);
	}
	catch(...)
	{
		try
		{
		g_gtsvr->cli_conn->Disconnect(m_datasock,50,-27);
		}
		catch(...)
		{

		}
		ToLogService("GateServer", "IsReady = false exception:{}", l_cmd);
	}

	--(g_gtsvr->cli_conn->m_calltotal);
	//l_line<<newln<<"st:"<<l_cmd;
	return 0;
}

void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
{
	try {
		auto l_plyCheck = static_cast<ClientConnection*>(datasock->GetPointer());
		if (l_plyCheck && l_plyCheck->m_actid > 0) {
			WPacket l_wpk = GetWPacket();
			l_wpk.WriteCmd(CMD_MC_MAPCRASH);
			l_wpk.WriteString("Login Error - You are already logged in.");
			SendData(datasock, l_wpk);
			return;
		}


		uLong	l_ulMilliseconds = 30 * 1000;
		uLong	l_tick = GetTickCount() - recvbuf.GetTickCount();
		bool	bCheat = false;	//
		if (l_ulMilliseconds > l_tick)
		{
			l_ulMilliseconds = l_ulMilliseconds - l_tick;

			if (m_version != recvbuf.ReverseReadShort())
			{
				WPacket l_wpk = GetWPacket();
				l_wpk.WriteCmd(CMD_MC_LOGIN);
				l_wpk.WriteShort(ERR_MC_VER_ERROR); //
				SendData(datasock, l_wpk);			// 
				ToLogService("GateServer", "client: {}	login error: client and server can't match!", datasock->GetPeerIP());
				Disconnect(datasock, 100, -31);
				return;
			}

			//
			if (recvbuf.ReverseReadShort() != 911)
			{
				bCheat = true;
			}
			else
			{
				recvbuf.DiscardLast(static_cast<uLong>(sizeof(uShort)));
			}

			recvbuf.DiscardLast(static_cast<uLong>(sizeof(uShort)));

			ToGroupServer* l_gps = g_gtsvr->gp_conn;
			auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
			if (!l_ply)//
			{
				return;
			}
			WPacket l_wpk = WPacket(recvbuf).Duplicate();

			l_wpk.WriteCmd(CMD_TP_USER_LOGIN);

			//l_wpk.WriteString(l_ply->m_chapstr);
			l_wpk.WriteLong(inet_addr(datasock->GetPeerIP()));
			l_wpk.WriteLong(ToAddress(l_ply)); // GateServer

			if (bCheat)
			{
				l_wpk.WriteShort(0);
			}
			else
			{
				l_wpk.WriteShort(911);
			}

			RPacket l_rpk = l_gps->SyncCall(l_gps->get_datasock(), l_wpk, l_ulMilliseconds);
			uShort l_errno = 0;
			if (l_rpk.HasData() == 0)
			{
				l_wpk = GetWPacket();
				l_wpk.WriteCmd(CMD_MC_LOGIN);
				l_wpk.WriteShort(ERR_MC_NETEXCP); //
				SendData(datasock, l_wpk); // 
				ToLogService("GateServer", "client: {}	login error: GroupServer is disresponse!", datasock->GetPeerIP());
				Disconnect(datasock, 100, -31);
				return;
			}

			if (l_errno = l_rpk.ReadShort())
			{
				l_wpk = l_rpk;
				l_wpk.WriteCmd(CMD_MC_LOGIN);
				l_wpk.WriteShort(l_errno);
				SendData(datasock, l_wpk);
				ToLogService("GateServer", "client: {}	login error, error:{}", datasock->GetPeerIP(), l_errno);
				Disconnect(datasock, 100, -31);
				return;
			}

			l_ply->m_status = ClientConnection::Status::CharacterSelection;

			l_ply->gp_addr = l_rpk.ReverseReadLong();	//GroupServer
			l_ply->m_loginID = l_rpk.ReverseReadLong();   //  Account DB id
			l_ply->m_actid = l_rpk.ReverseReadLong();
			BYTE byPassword = l_rpk.ReverseReadChar();
			//l_ply->comm_key_len =l_rpk.ReverseReadShort();
			//memcpy(l_ply->comm_textkey,l_rpk.GetDataAddr() +l_rpk.GetDataLen() -15 -l_ply->comm_key_len ,l_ply->comm_key_len);
			l_rpk.DiscardLast(sizeof(uLong) * 3 + 1);

			l_wpk = WPacket(l_rpk).Duplicate();
			l_wpk.WriteCmd(CMD_MC_LOGIN);
			l_wpk.WriteChar(byPassword);
			//l_wpk.WriteLong( 0x3214 );
			SendData(datasock, l_wpk);
			ToLogService("GateServer", "client: {}	login ok.", datasock->GetPeerIP());

			// 
		}
		else
		{
			WPacket l_wpk = GetWPacket();
			l_wpk.WriteCmd(CMD_MC_LOGIN);
			l_wpk.WriteShort(ERR_MC_NETEXCP); //
			SendData(datasock, l_wpk); // 
			ToLogService("GateServer", "client: {}	login error: packet time out!", datasock->GetPeerIP());
			Disconnect(datasock, 100, -31);
		}
	}
	catch (std::exception &e)
	{
		ToLogService("GateServer", "exception: {}", e.what());
	}
}



WPacket ToClient::CM_LOGOUT(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	l_ulMilliseconds =(l_ulMilliseconds>l_tick)?l_ulMilliseconds -l_tick:1;

	WPacket	l_retpk	=0;

	g_gtsvr->_mtxother.lock();

	auto* const l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
	if (l_ply)
	{
		l_ply->m_datasock = NULL;
	}

	datasock->SetPointer(0);
	g_gtsvr->_mtxother.unlock();

	if(l_ply)
	{
		l_ply->EndRun();

		auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
		try
		{
			if(l_ply->m_status == ClientConnection::Status::Invalid)			//
			{
				WPacket	l_wpk	=datasock->GetWPacket();
				l_retpk.WriteShort(ERR_MC_NOTLOGIN);
			}else
			{
				WPacket l_wpk	=GetWPacket();

				l_wpk.WriteCmd(CMD_TP_DISC);
				l_wpk.WriteLong(l_ply->m_actid);
				l_wpk.WriteLong(inet_addr(datasock->GetPeerIP()));
				l_wpk.WriteString(GetDisconnectErrText(datasock->GetDisconnectReason()?datasock->GetDisconnectReason():-27).c_str());
				g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(),l_wpk);

				GameServer	*l_game	=l_ply->game;
				if((l_ply->m_status	== ClientConnection::Status::Playing) && l_ply->gm_addr && l_game && l_game->m_datasock)
				{
					ToLogService("GateServer", "client: {}:{}	GoOut map,Gate to [{}]send GoOutMap command,dbid:{},Gate address:{:X},Game address:{:X}", datasock->GetPeerIP(), datasock->GetPeerPort(), l_game->m_datasock->GetPeerIP(), l_ply->m_dbid, ToAddress(l_ply), (LONG)l_ply->gm_addr);

					WPacket	l_wpk	=l_game->m_datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_TM_GOOUTMAP);
					l_wpk.WriteChar(0);

					l_wpk.WriteLong(ToAddress(l_ply));
					l_wpk.WriteLong(l_ply->gm_addr);		//GameServer

					l_ply->game		=0;						//GameServer
					l_ply->gm_addr	=0;						//GameServer

					SendData(l_game->m_datasock,l_wpk);
				}
				l_wpk = g_gtsvr->gp_conn->get_datasock()->GetWPacket();
				l_wpk.WriteCmd(CMD_TP_USER_LOGOUT);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);
				l_ply->gp_addr	=0;
				l_retpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk,l_ulMilliseconds);
			}
		}
		catch(...)
		{
			ToLogService("GateServer", "Error exit!");
		}

		l_ply->Free();
	}
	return l_retpk;
}

void ToClient::CM_BGNPLAY(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds = 30 * 1000;
	uLong	l_tick = GetTickCount() - recvbuf.GetTickCount();
	if (l_tick >= l_ulMilliseconds)
	{
		WPacket l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock, l_wpk);
		return;
	}

	l_ulMilliseconds = l_ulMilliseconds - l_tick;

	auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
	if (!l_ply)
	{
		return;
	}

	auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
	if (l_ply->m_status != ClientConnection::Status::CharacterSelection || !l_ply->gp_addr)
	{
		WPacket	l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		l_wpk.WriteShort(ERR_MC_NOTSELCHA);
		SendData(datasock, l_wpk);
		return;
	}

	//
	WPacket	l_wpk = WPacket(recvbuf).Duplicate();
	l_wpk.WriteCmd(CMD_TP_BGNPLAY);
	l_wpk.WriteLong(ToAddress(l_ply));
	l_wpk.WriteLong(l_ply->gp_addr);
	RPacket	l_rpk = g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), l_wpk, l_ulMilliseconds);
	if (!l_rpk.HasData())	//
	{
		l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock, l_wpk);
		return;
	}
	if (const auto l_errno = l_rpk.ReadShort())	//
	{
		l_wpk = l_rpk;
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		SendData(datasock, l_wpk);
		if (l_errno == ERR_PT_KICKUSER)
		{
			Disconnect(datasock, 100, -25);
		}
		return;
	}
	
	memset(l_ply->m_password, 0, sizeof(l_ply->m_password));
	strncpy(l_ply->m_password, l_rpk.ReadString(), ROLE_MAXSIZE_PASSWORD2);

	l_ply->m_dbid = l_rpk.ReadLong();
	l_ply->m_worldid = l_rpk.ReadLong();
	cChar* l_map = l_rpk.ReadString();
	l_ply->m_sGarnerWiner = l_rpk.ReadShort();
	GameServer* l_game = g_gtsvr->gm_conn->find(l_map);
	if (!l_game)
	{
		l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		l_wpk.WriteShort(ERR_MC_NOTARRIVE);
		SendData(datasock, l_wpk);
		return;
	}
	if (l_game->m_plynum > 15000)
	{
		l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_BGNPLAY);
		l_wpk.WriteShort(ERR_MC_TOOMANYPLY);
		SendData(datasock, l_wpk);
		return;
	}

	l_ply->m_status = ClientConnection::Status::Playing;
	l_game->m_plynum++;

	ToLogService("GateServer", "client: {}:{}	BeginPlay entry map,Gate to[{}]send EnterMap command,dbid:{},Gate address:{:X}", datasock->GetPeerIP(), datasock->GetPeerPort(), l_game->m_datasock->GetPeerIP(), l_ply->m_dbid, ToAddress(l_ply));

	l_game->EnterMap(l_ply, l_ply->m_loginID, l_ply->m_dbid, l_ply->m_worldid, l_map, -1, 0, 0, 0, l_ply->m_sGarnerWiner);
}

void ToClient::CM_ENDPLAY(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	l_ulMilliseconds =(l_ulMilliseconds>l_tick)?l_ulMilliseconds -l_tick:1;

	auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
	if(l_ply)
	{
		auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
		if(l_ply->m_status != ClientConnection::Status::Playing ||!l_ply->gm_addr)
		{
			WPacket	l_wpk	=datasock->GetWPacket();
			l_wpk.WriteCmd(CMD_MC_ENDPLAY);
			l_wpk.WriteShort(ERR_MC_NOTPLAY);
			SendData(datasock,l_wpk);
			Disconnect(datasock,100,-25);
		}else
		{
			GameServer	*l_game	=l_ply->game;
			if(l_game && l_game->m_datasock)
			{				
				l_ply->m_status		= ClientConnection::Status::CharacterSelection;;						//
				l_game->m_plynum	--;
				
				ToLogService("GateServer", "client: {}:{}	GoOut map,Gate to[{}] send GoOutMap command,dbid:{},Gate address:{:X},Game address:{:X}", datasock->GetPeerIP(), datasock->GetPeerPort(), l_game->m_datasock->GetPeerIP(), l_ply->m_dbid, ToAddress(l_ply), (LONG)l_ply->gm_addr);

				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				l_wpk.WriteCmd(CMD_TM_GOOUTMAP);
				l_wpk.WriteChar(0);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gm_addr);			//GameServer
				l_ply->game		=0;							//GameServer
				l_ply->gm_addr	=0;							//GameServer
				g_gtsvr->gm_conn->SendData(l_game->m_datasock,l_wpk);
				
				l_wpk	=WPacket(recvbuf).Duplicate(); // seems to be the problem
				l_wpk.WriteCmd(CMD_TP_ENDPLAY);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);
				l_wpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk,l_ulMilliseconds);
				if(!l_wpk.HasData())
				{
					l_wpk	=datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_ENDPLAY);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					SendData(datasock,l_wpk);
					Disconnect(datasock,100,-25);
				}else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_ENDPLAY);
					SendData(datasock,l_wpk);
					l_ply->m_dbid	=0;
					l_ply->m_worldid=0;
					if(RPacket(l_wpk).ReadShort() ==ERR_PT_KICKUSER)
					{
						Disconnect(datasock,100,-25);
					}
				}				
			}
		}
	}
}

void ToClient::CP_CHANGEPASS(DataSocket* datasock, RPacket& recvbuf){
	auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
	if (l_ply){
		auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
		WPacket l_wpk = WPacket(recvbuf).Duplicate();
		l_wpk.WriteCmd(CMD_TP_CHANGEPASS);
		l_wpk.WriteLong(ToAddress(l_ply));
		l_wpk.WriteLong(l_ply->gp_addr);
		l_wpk = g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), l_wpk);
		if (l_wpk.HasData()){
			SendData(datasock, l_wpk);
		}
	}
}

void ToClient::CM_REGISTER(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds = 30 * 1000;
	uLong	l_tick = GetTickCount() - recvbuf.GetTickCount();
	//if (l_ulMilliseconds>l_tick)
	if (true)
	{
		l_ulMilliseconds = l_ulMilliseconds - l_tick;

		auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if (l_ply)
		{
			auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
			WPacket l_wpk = WPacket(recvbuf).Duplicate();
			l_wpk.WriteCmd(CMD_TP_REGISTER);
			l_wpk.WriteLong(ToAddress(l_ply));
			l_wpk.WriteLong(l_ply->gp_addr);
			l_wpk = g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(), l_wpk, l_ulMilliseconds);
			if (l_wpk.HasData())
			{
				l_wpk.WriteCmd(CMD_PC_REGISTER);
				SendData(datasock, l_wpk);
			}
		}
	}
	else
	{
		WPacket	l_wpk = datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_NEWCHA);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock, l_wpk);
		Disconnect(datasock, 100, -25);
	}
	Disconnect(datasock, 100, -25);
}

void ToClient::CM_NEWCHA(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	if(l_ulMilliseconds>l_tick)
	{
		l_ulMilliseconds =l_ulMilliseconds -l_tick;

		auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if(l_ply)
		{
			auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
			if(l_ply->m_status != ClientConnection::Status::CharacterSelection || !l_ply->gp_addr)
			{
				WPacket	l_wpk	=datasock->GetWPacket();
				l_wpk.WriteCmd(CMD_MC_NEWCHA);
				l_wpk.WriteShort(ERR_MC_NOTSELCHA);
				SendData(datasock,l_wpk);
			}else
			{
				//GroupServer
				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				l_wpk.WriteCmd(CMD_TP_NEWCHA);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);	//
				l_wpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk,l_ulMilliseconds);
				if(!l_wpk.HasData())
				{
					l_wpk	=datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_NEWCHA);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					SendData(datasock,l_wpk);
					Disconnect(datasock,100,-25);
				}else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_NEWCHA);
					SendData(datasock,l_wpk);
					if(RPacket(l_wpk).ReadShort() ==ERR_PT_KICKUSER)
					{
						Disconnect(datasock,100,-25);
					}
				}
			}
		}
	}else
	{
		WPacket	l_wpk	=datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_NEWCHA);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock,l_wpk);
		Disconnect(datasock,100,-25);
	}
}
void ToClient::CM_DELCHA(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	if(l_ulMilliseconds>l_tick)
	{
		l_ulMilliseconds =l_ulMilliseconds -l_tick;

		auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if(l_ply)
		{
			auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
			if(l_ply->m_status != ClientConnection::Status::CharacterSelection || !l_ply->gp_addr)
			{
				WPacket	l_wpk	=datasock->GetWPacket();
				l_wpk.WriteCmd(CMD_MC_DELCHA);
				l_wpk.WriteShort(ERR_MC_NOTSELCHA);
				SendData(datasock,l_wpk);
			}else
			{
				
				//GroupServer
				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				l_wpk.WriteCmd(CMD_TP_DELCHA);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);	//
				l_wpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk);
				if(!l_wpk.HasData())
				{
					l_wpk	=datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_DELCHA);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					SendData(datasock,l_wpk);
					Disconnect(datasock,100,-25);
				}else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_DELCHA);
					SendData(datasock,l_wpk);
					if(RPacket(l_wpk).ReadShort() ==ERR_PT_KICKUSER)
					{
						Disconnect(datasock,100,-25);
					}
				}
			}
		}
	}else
	{
		WPacket	l_wpk	=datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_DELCHA);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock,l_wpk);
		Disconnect(datasock,100,-25);
	}
}

void ToClient::CM_CREATE_PASSWORD2(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	if(l_ulMilliseconds>l_tick)
	{
		l_ulMilliseconds =l_ulMilliseconds -l_tick;

		auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if(l_ply)
		{
			auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
			if (l_ply->m_status != ClientConnection::Status::CharacterSelection || !l_ply->gp_addr)
			{
				WPacket	l_wpk	=datasock->GetWPacket();
				l_wpk.WriteCmd(CMD_MC_DELCHA);
				l_wpk.WriteShort(ERR_MC_NOTSELCHA);
				SendData(datasock,l_wpk);
			}else
			{
				//GroupServer
				WPacket l_log(recvbuf);
				const char * pszPW = recvbuf.ReadString();
				//if( pszPW )
				//{
				//	LogLine l_line(g_gateerr);
				//	l_line<<newln<<""<<pszPW<<"ID"<<l_ply->m_actid<<endln;
				//}
				//else
				//{
				//	LogLine l_line(g_gateerr);
				//	l_line<<newln<<""<<""<<"ID"<<l_ply->m_actid<<endln;
				//}
				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				l_wpk.WriteCmd(CMD_TP_CREATE_PASSWORD2);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);	//
				l_wpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk);
				if(!l_wpk.HasData())
				{
					l_wpk	=datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_CREATE_PASSWORD2);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					SendData(datasock,l_wpk);
					Disconnect(datasock,100,-25);
				}else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_CREATE_PASSWORD2);
					SendData(datasock,l_wpk);
					if(RPacket(l_wpk).ReadShort() ==ERR_PT_KICKUSER)
					{
						Disconnect(datasock,100,-25);
					}
				}
			}
		}
	}else
	{
		WPacket	l_wpk	=datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_CREATE_PASSWORD2);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock,l_wpk);
		Disconnect(datasock,100,-25);
	}
}

void ToClient::CM_UPDATE_PASSWORD2(DataSocket* datasock, RPacket& recvbuf)
{
	uLong	l_ulMilliseconds	=30*1000;
	uLong	l_tick	=GetTickCount()	-recvbuf.GetTickCount();
	if(l_ulMilliseconds>l_tick)
	{
		l_ulMilliseconds =l_ulMilliseconds -l_tick;

		auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if(l_ply)
		{
			auto const l_lockStat = std::lock_guard{l_ply->m_mtxstat};
			if(l_ply->m_status != ClientConnection::Status::CharacterSelection || !l_ply->gp_addr)
			{
				WPacket	l_wpk	=datasock->GetWPacket();
				l_wpk.WriteCmd(CMD_MC_DELCHA);
				l_wpk.WriteShort(ERR_MC_NOTSELCHA);
				SendData(datasock,l_wpk);
			}else
			{
				//GroupServer
				WPacket l_wpk	=WPacket(recvbuf).Duplicate();
				l_wpk.WriteCmd(CMD_TP_UPDATE_PASSWORD2);
				l_wpk.WriteLong(ToAddress(l_ply));
				l_wpk.WriteLong(l_ply->gp_addr);	//
				l_wpk	=g_gtsvr->gp_conn->SyncCall(g_gtsvr->gp_conn->get_datasock(),l_wpk);
				if(!l_wpk.HasData())
				{
					l_wpk	=datasock->GetWPacket();
					l_wpk.WriteCmd(CMD_MC_UPDATE_PASSWORD2);
					l_wpk.WriteShort(ERR_MC_NETEXCP);
					SendData(datasock,l_wpk);
					Disconnect(datasock,100,-25);
				}else
				{
					//Client
					l_wpk.WriteCmd(CMD_MC_UPDATE_PASSWORD2);
					SendData(datasock,l_wpk);
					if(RPacket(l_wpk).ReadShort() ==ERR_PT_KICKUSER)
					{
						Disconnect(datasock,100,-25);
					}
				}
			}
		}
	}else
	{
		WPacket	l_wpk	=datasock->GetWPacket();
		l_wpk.WriteCmd(CMD_MC_UPDATE_PASSWORD2);
		l_wpk.WriteShort(ERR_MC_NETEXCP);
		SendData(datasock,l_wpk);
		Disconnect(datasock,100,-25);
	}
}

bool ToClient::OnEncrypt(DataSocket* datasock, char* ciphertext, uLong ciphertext_len, const char* text, uLong& len)
{
	//TcpCommApp::OnEncrypt(datasock,ciphertext, text, len);
	auto PlayerHandShakeDone = [&]() -> ClientConnection*
	{
		auto ply = static_cast<ClientConnection*>(datasock->GetPointer());
		return (ply && ply->handshakeDone) ? ply : nullptr;
	};

	ClientConnection* ply{};
	if (_comm_enc > 0 && (ply = PlayerHandShakeDone()))
	{
		return ply->EncryptAES(ciphertext, ciphertext_len, text, len);

	}

	if (ciphertext == text)
	{
		return true;
	}

	std::memcpy(ciphertext, text, len);
	return true;
}

bool ToClient::OnDecrypt(DataSocket *datasock,char *ciphertext,uLong& len)
{
	//TcpCommApp::OnDecrypt(datasock, ciphertext, len);

	if (_comm_enc > 0)
	{ 
		auto ply = static_cast<ClientConnection*>(datasock->GetPointer());
		if (ply&&ply->handshakeDone) 
		{
			return ply->DecryptAES(ciphertext, ciphertext, len);
		}
	}
	return false;
}

void ToClient::post_mapcrash_msg(ClientConnection* ply)
{
	if (ply->m_datasock == NULL) return;
	WPacket pk = ply->m_datasock->GetWPacket();
	pk.WriteCmd(CMD_MC_MAPCRASH);
	//pk.WriteString("...");
	pk.WriteString(RES_STRING(GS_TOCLIENT_CPP_00031));
	SendData(ply->m_datasock, pk);
}

