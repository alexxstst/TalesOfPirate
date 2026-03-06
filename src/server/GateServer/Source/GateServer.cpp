#include <iostream>
#include <signal.h>
#include "timer.h"
#include "gateserver.h"

using namespace std;

dbc::uLong	NetBuffer[]		= {100, 10, 0};
bool	g_logautobak	= true;
dbc::LogStream g_gateerr("ErrServer");
dbc::LogStream g_gatelog("GateServer");
dbc::LogStream g_chkattack("AttackMonitor");
dbc::LogStream g_gateconnect("Connect");
//LogStream g_gatepacket("PacketProc");

dbc::InterLockedLong		g_exit	=0;
dbc::InterLockedLong		g_ref	=0;

dbc::TimerMgr			g_timermgr;
//=========Timer==============
extern "C"{WINBASEAPI HWND APIENTRY GetConsoleWindow(VOID);}
class DisableCloseButton: public dbc::Timer
{
public:
	DisableCloseButton(dbc::uLong interval):dbc::Timer(interval),m_hMenu(0)
	{
		HWND hWnd	= ::GetConsoleWindow();
		m_hMenu		= GetSystemMenu(hWnd, FALSE);
	}
private:
	~DisableCloseButton()
	{
	}
	void Process()
	{
		dbc::RefArmor l(g_ref);
		if (!g_exit && m_hMenu)
		{
			EnableMenuItem(m_hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		}
	}
	HMENU m_hMenu;
};
class DelayLogout: public dbc::Timer, public dbc::RunBiDirectChain<ClientConnection>
{
public:
	DelayLogout(dbc::uLong interval):dbc::Timer(interval){}
	void AddPlayer(ClientConnection* ply)
	{
		ply->_BeginRun(this);
	}
	void DelPlayer(ClientConnection* ply)
	{
		ply->_EndRun();
	}
private:
	void Process()
	{
		ClientConnection* l_ply{};
		dbc::RunChainGetArmor<ClientConnection> l_lock(*this);
		while(l_ply	=GetNextItem())
		{
			
		}
		l_lock.unlock();
	}
};

void __cdecl ctrlc_dispatch(int sig)
{
	if (sig == SIGINT)
	{
		g_exit	=1;
		signal(SIGINT, ctrlc_dispatch);
	}
}

//---------------------------------------------------------------------------
// class GateServer
//---------------------------------------------------------------------------
GateServer::GateServer(char const* fname)
:client_heap(1,2000),m_tch(1,200),gm_conn(NULL),gp_conn(NULL),cli_conn(NULL)
,m_clcomm(NULL),m_gpcomm(NULL),m_gmcomm(NULL),m_clproc(NULL)
{
	dbc::TcpCommApp::WSAStartup();
	srand((unsigned int)time(NULL)); // 

	m_tch.Init();
	client_heap.Init();

	m_clproc = dbc::ThreadPool::CreatePool(24, 32, 4096);
	m_clcomm = dbc::ThreadPool::CreatePool(6, 12, 4096, THREAD_PRIORITY_ABOVE_NORMAL);
	m_gpproc = dbc::ThreadPool::CreatePool(4, 8, 1024, THREAD_PRIORITY_ABOVE_NORMAL);
	m_gpcomm = dbc::ThreadPool::CreatePool(12, 24, 2048, THREAD_PRIORITY_ABOVE_NORMAL);
	m_gmcomm = dbc::ThreadPool::CreatePool(4, 4, 2048, THREAD_PRIORITY_ABOVE_NORMAL);

	try{
		gm_conn = new ToGameServer(fname, 0, m_gmcomm);
		gp_conn = new ToGroupServer(fname, m_gpproc, m_gpcomm);
		cli_conn = new ToClient(fname, m_clproc, m_clcomm);
		m_gpproc->AddTask(new ConnectGroupServer(gp_conn));
		m_clproc->AddTask(&g_timermgr);
		g_timermgr.AddTimer(new DisableCloseButton(200));
		signal(SIGINT, ctrlc_dispatch);
	}catch (...)
	{
		if(gp_conn)
		{
			delete gp_conn;
			gp_conn = 0;
		}
		if(gm_conn)
		{
			delete gm_conn;
			gm_conn = NULL;
		}
		if(cli_conn)
		{
			delete cli_conn;
			cli_conn = NULL;
		}
		m_gmcomm->DestroyPool();
		m_gpcomm->DestroyPool();
		m_clcomm->DestroyPool();
		m_clproc->DestroyPool();
		dbc::TcpCommApp::WSACleanup();
		throw;
	}
}

GateServer::~GateServer()
{
	g_exit	=1;
	while(g_ref)
	{
		Sleep(1);
	}
	delete cli_conn;
	delete gp_conn;
	delete gm_conn;
	m_gmcomm->DestroyPool();
	m_gpcomm->DestroyPool(); 
	m_clcomm->DestroyPool();
	m_clproc->DestroyPool();
	dbc::TcpCommApp::WSACleanup();
}

void GateServer::RunLoop()
{
	dbc::BandwidthStat	l_band;
	dbc::LLong	recvpkps_max=0,recvbandps_max=0,sendpkps_max=0,sendbandps_max=0;

	dbc::dstring l_str;
	l_str.SetSize(256);
	while(!g_exit)
	{
		//std::cout<<"(exitCtrl+C):\n";
		std::cout<< RES_STRING(GS_GATESERVER_CPP_00001); //Modify by lark.li 20070130
		std::cin.getline(l_str.GetBuffer(),256);

		if(l_str =="exit" || g_exit)
		{
			//std::cout<<"..."<<std::endl;
			std::cout<< RES_STRING(GS_GATESERVER_CPP_00002)<<std::endl;
			break;
		}else	if(l_str =="getinfo")
		{
			std::cout<<"getinfo..."<<std::endl;
			
			l_band	=cli_conn->GetBandwidthStat();
			std::cout<<"getinfo: GetBandwidthStat..."<<std::endl;

			//std::cout<<""<<cli_conn->GetSockTotal()<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00003)<<cli_conn->GetSockTotal()<<std::endl;
			//std::cout<<"[]{pkt/s:"<<l_band.m_sendpktps<<"}{pkt:"<<l_band.m_sendpkts<<"}{KB/s:"<<l_band.m_sendbyteps/1024<<"}{KB:"<<l_band.m_sendbytes/1024<<"}"<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00004)<<l_band.m_sendpktps<<"}{pkt:"<<l_band.m_sendpkts<<"}{KB/s:"<<l_band.m_sendbyteps/1024<<"}{KB:"<<l_band.m_sendbytes/1024<<"}"<<std::endl;
			//std::cout<<"[]{pkt/s:"<<l_band.m_recvpktps<<"}{pkt:"<<l_band.m_recvpkts<<"}{KB/s:"<<l_band.m_recvbyteps/1024<<"}{KB:"<<l_band.m_recvbytes/1024<<"}"<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00005)<<l_band.m_recvpktps<<"}{pkt:"<<l_band.m_recvpkts<<"}{KB/s:"<<l_band.m_recvbyteps/1024<<"}{KB:"<<l_band.m_recvbytes/1024<<"}"<<std::endl;

			if(l_band.m_sendpktps	>sendpkps_max)			sendpkps_max	=l_band.m_sendpktps;
			if(l_band.m_sendbyteps/1024 >sendbandps_max)	sendbandps_max	=l_band.m_sendbyteps/1024;
			if(l_band.m_recvpktps >recvpkps_max)			recvpkps_max	=l_band.m_recvpktps;
			if(l_band.m_recvbyteps/1024 >recvbandps_max)	recvbandps_max	=l_band.m_recvbyteps/1024;
			//std::cout<<"[Max]{pkt/s:"<<sendpkps_max<<"}{KB/s:"<<sendbandps_max<<"}"<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00006)<<sendpkps_max<<"}{KB/s:"<<sendbandps_max<<"}"<<std::endl;
			//std::cout<<"[Max]{pkt/s:"<<recvpkps_max<<"}{KB/s:"<<recvbandps_max<<"}"<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00007)<<recvpkps_max<<"}{KB/s:"<<recvbandps_max<<"}"<<std::endl;
		}else	if(l_str	=="clmax")
		{
			recvpkps_max=recvbandps_max=sendpkps_max=sendbandps_max=0;
		}else	if(l_str	=="getmaxcon")
		{
			//std::cout<<""<<g_gtsvr->cli_conn->GetMaxCon()<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00008)<<g_gtsvr->cli_conn->GetMaxCon()<<std::endl;
		}else	if(!strncmp(l_str.c_str(),"setmaxcon",9))
		{
			dbc::uShort l_maxcon	=atoi(l_str.c_str() +9);
			if(l_maxcon >1500)
			{
				//std::cout<<"1500,1500"<<std::endl;
				std::cout<<RES_STRING(GS_GATESERVER_CPP_00009)<<std::endl;
				l_maxcon	=1500;
			}else
			{
				//std::cout<<":"<<l_maxcon<<std::endl;
				std::cout<<RES_STRING(GS_GATESERVER_CPP_00010)<<l_maxcon<<std::endl;
			}
			g_gtsvr->cli_conn->SetMaxCon(l_maxcon);
		}else	if(l_str	=="logbak")
		{
			dbc::LogStream::Backup();
		}else	if(l_str	=="getqueparm")
		{
			std::cout<<"ToClient Process Queue:"<<m_clproc->GetTaskCount()<<"\tToClint Comm Queue:"<<m_clcomm->GetTaskCount()<<std::endl;
			std::cout<<"ToGroup Comm Queue:"<<m_gpcomm->GetTaskCount()<<"\tToGame Comm Queue:"<<m_gmcomm->GetTaskCount()<<std::endl;
		}else	if(!strncmp(l_str.c_str(),"setshowrange",12))
		{
			const char* pstring = l_str.c_str();
			pstring += 12;
			int min = atoi(pstring);
			pstring = strchr( pstring, ',' );
			if( !pstring )
			{
				//std::cout<<"setshowrange 1,2" <<std::endl;
				std::cout<<RES_STRING(GS_GATESERVER_CPP_00011) <<std::endl;
			}
			else
			{
				pstring++;
				int max = atoi( pstring );
				std::cout<<"SetShowRnage:["<< min << "-" << max << "]" <<std::endl;
				g_app->SetShowRange( min, max );
			}
		}
		else	if(l_str	=="getshowrange")
		{
			std::cout<<"ShowRnage:["<< g_app->GetShowMin() << "-" << g_app->GetShowMax() << "]" <<std::endl;
		}
		else if( l_str == "reconnect" )
		{
			if( g_gtsvr->gp_conn ) 
			{
				g_gtsvr->gp_conn->Disconnect( g_gtsvr->gp_conn->get_datasock(), -9 );
				std::cout<<"reconnect success!" <<std::endl;
			}
			else
			{
				std::cout<<"reconnect failed! null pointer!" <<std::endl;
			}
		}
		else if( l_str == "calltotal" )
		{
			std::cout<<"clinet::calltotal:["<< g_gtsvr->cli_conn->GetCallTotal() <<std::endl;
			std::cout<<"group::calltotal:["<< g_gtsvr->gp_conn->GetCallTotal() <<std::endl;
		}
		else
		{
			//std::cout<<""<<std::endl;
			std::cout<<RES_STRING(GS_GATESERVER_CPP_00012)<<std::endl;
		}
	}
}

//---------------------------------------------------------------------------
// class Player
//---------------------------------------------------------------------------
bool ClientConnection::InitReference(dbc::DataSocket* datasock)
{
	auto const lock = std::lock_guard{g_gtsvr->_mtxother};//
	if(datasock && !datasock->GetPointer())
	{
		datasock->SetPointer(this);
		m_datasock = datasock;
		return true;
	}
	else
	{
		if( datasock )
		{
			try
			{
				//printf( "InitReference warning: %s", datasock->GetPeerIP() );
				printf( RES_STRING(GS_GATESERVER_CPP_00013), datasock->GetPeerIP() );
				auto l_ply = static_cast<ClientConnection*>(datasock->GetPointer());
				if( l_ply )
				{
					l_ply->m_datasock = NULL;
					datasock->SetPointer( NULL );
				}
			}
			catch(...)
			{
				//printf( "InitReference warning: %sexception", datasock->GetPeerIP() );
				printf( RES_STRING(GS_GATESERVER_CPP_00014), datasock->GetPeerIP() );
			}
		}
		return false;
	}
	return false;
}

void ClientConnection::Initially()
{
	m_worldid = m_actid = m_dbid = gm_addr = gp_addr = 0;
	m_status = ClientConnection::Status::Invalid;
	m_password[0] = 0;
	m_datasock = NULL;
	game = NULL;
	
	enc = false;
	m_pingtime	=0;
	m_lestoptick = GetTickCount();
	m_estop = false;
	m_sGarnerWiner = 0;

	handshakeDone = false;

	rng.Reseed();
}

void ClientConnection::Finally()
{
	m_worldid = m_actid = m_dbid = gm_addr = gp_addr = 0;
	m_status = ClientConnection::Status::Invalid;
	game = NULL;
	enc = false;
	if (m_datasock != NULL)
	{
		m_datasock->SetPointer(NULL);
		m_datasock = NULL;
	}

	srvPrivateKey = {};
	handshakeDone = false;

	m_chamap.SetSize(0);
}



// Add by lark.li 20081119 begin
bool ClientConnection::BeginRun()
{
	return	RunBiDirectItem<ClientConnection>::_BeginRun(&(g_gtsvr->m_plylst))?true:false;
}
bool ClientConnection::EndRun()
{
	return RunBiDirectItem<ClientConnection>::_EndRun()?true:false;
}

void ClientConnection::SendSysInfo(std::string_view message) const
{
	auto wpk = g_gtsvr->cli_conn->GetWPacket();
	wpk.WriteCmd(CMD_MC_SYSINFO);
	wpk.WriteString(message);
	g_gtsvr->cli_conn->SendData(m_datasock, wpk);
}

// End

void ClientConnection::SendPacketToClient(dbc::WPacket pkt) {
	g_gtsvr->cli_conn->SendData(m_datasock, pkt);
}

void ClientConnection::SendPacketToGroupServer(dbc::WPacket pkt){
	if (gp_addr && gm_addr){
		dbc::WPacket	l_wpk = dbc::WPacket(pkt).Duplicate();
		l_wpk.WriteLong(ToAddress(this));
		l_wpk.WriteLong(gp_addr);
		g_gtsvr->gp_conn->SendData(g_gtsvr->gp_conn->get_datasock(), l_wpk);
	}
}

void ClientConnection::SendPacketToGameServer(dbc::WPacket pkt){
	if (gp_addr && gm_addr && game) {
		dbc::WPacket	l_wpk = dbc::WPacket(pkt).Duplicate();
		l_wpk.WriteLong(ToAddress(this));
		l_wpk.WriteLong(gm_addr);
		g_gtsvr->gm_conn->SendData(game->m_datasock, l_wpk);
	}
}

bool ClientConnection::EncryptAES(char* ciphertext, dbc::uLong ciphertext_len, dbc::cChar* plaintext, dbc::uLong& plainsize) {

	try
	{
		CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE); // IV size = 16 bytes
		rng.GenerateBlock(iv.data(), CryptoPP::AES::BLOCKSIZE); // Generate random initialization vector

		string output;
		string cipher;
		CryptoPP::GCM<CryptoPP::AES>::Encryption e;

		e.SetKeyWithIV(cliPrivateKey, cliPrivateKey.size(), iv.data(), CryptoPP::AES::BLOCKSIZE);			// Set key and IV pair
		CryptoPP::StringSource ss((const CryptoPP::byte*)plaintext, plainsize, true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(output), false, 12));
		// Encrypt from byte pointer, ciphersize bytes long, towards output.
		// plaintext might contain null-terminated values, so we got to use the ciphersize provided by operator<< definition.
		CryptoPP::StringSource ss2(output, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(cipher), false));
		// Encode the output using Base64.

		if (ciphertext_len < (cipher.size() + CryptoPP::AES::BLOCKSIZE + 1))
		{
			// cipher wont fit in buffer
			return false;
		}

		std::memcpy(ciphertext, cipher.c_str(), cipher.size());	// This does not copy the null-terminated char.
		// Copy the Base64 output to the ciphertext pointer.
		plainsize = cipher.size();
		// Update cipher size

		//Data from text gets encrypted using key provided, then Base64Encoded. The result is char array, copied to ciphertext pointer.
		//The text length provided also gets updated. New length is the ciphertext length, not including the null-terminated char.
		std::memcpy(ciphertext + plainsize + 1, iv.data(), CryptoPP::AES::BLOCKSIZE);
		// Copy initialization vector to ciphertext char array. Do not overwrite the last char.
		plainsize += CryptoPP::AES::BLOCKSIZE + 1;
		return true;
	}
	catch (CryptoPP::BufferedTransformation::NoChannelSupport& e)
	{
		// The tag must go in to the default channel:
		//  "unknown: this object doesn't support multiple channels"
		cerr << "Caught NoChannelSupport..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		return false;
	}
	catch (CryptoPP::AuthenticatedSymmetricCipher::BadState& e)
	{
		// Pushing PDATA before ADATA results in:
		//  "GMC/AES: Update was called before State_IVSet"
		cerr << "Caught BadState..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		return false;
	}
	catch (CryptoPP::InvalidArgument& e)
	{
		cerr << "Caught InvalidArgument..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		return false;
	}
	catch (...)
	{
		return false;
	}

	return false;
}


bool ClientConnection::DecryptAES(dbc::cChar* ciphertext, char* plaintext, dbc::uLong& ciphersize)
{
	try
	{
		if (ciphersize < (CryptoPP::AES::BLOCKSIZE + 1))
		{
			//NOTE(Ogge): Its impossible that it contains a IV key.
			return false;
		}

		// Get IV key and update length
		ciphersize -= (CryptoPP::AES::BLOCKSIZE + 1);
		CryptoPP::SecByteBlock iv((CryptoPP::byte*)ciphertext + ciphersize + 1, CryptoPP::AES::BLOCKSIZE);

		CryptoPP::GCM<CryptoPP::AES>::Decryption d;
		std::string base64decoded;
		std::string plain;

		d.SetKeyWithIV(cliPrivateKey.data(), cliPrivateKey.size(), iv.data(), CryptoPP::AES::BLOCKSIZE);
		CryptoPP::StringSource ss((CryptoPP::byte*)ciphertext, ciphersize, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(base64decoded)));
		CryptoPP::AuthenticatedDecryptionFilter df(d, new CryptoPP::StringSink(plain), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12);
		CryptoPP::StringSource ss2(base64decoded, true, new CryptoPP::Redirector(df));
		std::memcpy(plaintext, plain.c_str(), plain.size());
		ciphersize = plain.size();
		if (!df.GetLastResult()) return false;

		return true;
	}
	catch (CryptoPP::InvalidArgument& e)
	{
		cerr << "Caught InvalidArgument..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
	catch (CryptoPP::AuthenticatedSymmetricCipher::BadState& e)
	{
		// Pushing PDATA before ADATA results in:
		//  "GMC/AES: Update was called before State_IVSet"
		cerr << "Caught BadState..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
	catch (CryptoPP::HashVerificationFilter::HashVerificationFailed& e)
	{
		cerr << "Caught HashVerificationFailed..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}

	//NOTE(Ogge): I guess only way to reach bottom,
	// is if 'return true' in try block is never reached.
	// A exception must have been handled to get here.
	return false;
}


//---------------------------------------------------------------------------
// class GateServerApp
//---------------------------------------------------------------------------
int	GateServerApp::_nShowMin = 0;	
int	GateServerApp::_nShowMax = 0;	
GateServerApp* g_app = NULL;

GateServerApp::GateServerApp()
: _pUdpManage(NULL)
{
	g_app = this;
}

void GateServerApp::ServiceStart()
{
	// 
	try
	{
		const char* file_cfg = "GateServer.cfg";
		g_gtsvr = new GateServer( file_cfg );

		dbc::IniFile inf(file_cfg);
		_nShowMin = std::stoi(inf["ShowRange"]["ShowMin"]);
		_nShowMax = std::stoi(inf["ShowRange"]["ShowMax"]);
		if(std::stoi(inf["ShowRange"]["IsUse"])!=0 )
		{
		}
	}
	catch (dbc::excp& e)
	{
		cout << e.what() << endl;
		Sleep(10 * 1000);
		exit(-1);
	}
	catch (...)
	{
		//cout << "GateServer !" << endl;
		cout << RES_STRING(GS_GATESERVER_CPP_00016) << endl;
		Sleep(10 * 1000);
		exit(-2);
	}

	// 
	//cout << "GateServer !" << endl;
	cout << RES_STRING(GS_GATESERVER_CPP_00017) << endl;
}
void GateServerApp::ServiceStop()
{
	if( _pUdpManage ) 
	{
		delete _pUdpManage;
		_pUdpManage = NULL;
	}

	// 
	delete g_gtsvr;
	g_app = NULL;

	//cout << "GateServer !" << endl;
	cout << RES_STRING(GS_GATESERVER_CPP_00018) << endl;
	Sleep(2000);
}

void GateServerApp::_NotifySocketNumEvent( CUdpManage* pManage, CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len )
{
	static char szBuf[255] = { 0 };
	if( len==1 && pData[0]=='#' ) 
	{
		static DWORD dwTime = 0;
		static DWORD dwLastTime = 0;
		static DWORD dwCount = 0;

		// ,Jerry
		dwTime = ::GetTickCount();
		if( dwTime>dwLastTime )
		{
			dwCount = g_gtsvr->cli_conn->GetSockTotal();
			dwLastTime = dwTime + 60000;
		}

		sprintf( szBuf, "%d,%d,%d", dwCount, _nShowMin, _nShowMax );
	}
}

//  GateServer 
GateServer* g_gtsvr;
bool volatile g_appexit = false;

GameServer::~GameServer()
{
	if (m_datasock)
	{
		m_datasock->SetPointer(nullptr);
		m_datasock = nullptr;
	}
}

// Remain should be > 0, to give server a chance to send packet
void ToClient::TC_DISCONNECT(dbc::DataSocket* datasock, int reason, int remain)
{
	if (!datasock)
	{
		return;
	}

	Disconnect(datasock, remain, reason);

	auto wpk = GetWPacket();
	wpk.WriteCmd(CMD_TC_DISCONNECT);
	wpk.WriteLong(reason);
	g_gtsvr->cli_conn->SendData(datasock, wpk);
}

ClientConnection::ClientConnection(dbc::uLong Size) : dbc::PreAllocStru(Size)
{
}

ClientConnection::~ClientConnection()
{
	if (m_datasock) {
		m_datasock->SetPointer(nullptr);
		m_datasock = nullptr;
	}
}
