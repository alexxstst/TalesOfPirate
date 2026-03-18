#include "StdAfx.h"
#pragma warning(disable: 4018)
#include "NetIF.h"
#include "GameApp.h"
#include "GameAppMsg.h"
#include "Character.h"
#pragma warning(default: 4018)
#include "Algo.h"
#include "procirculate.h"
#include "GameConfig.h"
#include "ProCirculate.h"
//=============附加包含头文件BEGIN=============
#include "PacketCmd.h"
#include "NetChat.h"
#include "NetGuild.h"
#include "uiequipform.h"
#include "uiglobalvar.h"
#include <fstream>
#include <iostream>
#include "CmdNames.h"
//=============附加包含头文件END===============

// Консоль для логирования пакетов
static bool g_consoleInitialized = false;

static void EnsureConsole() {
	if (g_consoleInitialized) return;
	g_consoleInitialized = true;
	if (AllocConsole()) {
		FILE* fp = nullptr;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		SetConsoleTitleA("TalesOfPirate — Packet Log");
		// Увеличить буфер консоли
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD size = {160, 9999};
		SetConsoleScreenBufferSize(hOut, size);
	}
}

bool g_logautobak = false;

//End
NetIF* g_NetIF;
CLogName g_LogName;

extern short g_sClientVer;

#include "uidoublepwdform.h"
extern CDoublePwdMgr g_stUIDoublePwd;
using namespace std;

//-------------------
// Packet消息处理函数
//-------------------

BOOL NetIF::HandlePacketMessage(LPRPACKET pk) {
	if (!static_cast<bool>(pk)) return FALSE;
	unsigned short sCmdType = pk.GetCmd();

	BOOL bRet = FALSE;
	switch (sCmdType) {
	case CMD_MC_LOGIN: return SC_Login(pk);
	case CMD_MC_ENTERMAP: return SC_EnterMap(pk);
	case CMD_MC_BGNPLAY: return SC_BeginPlay(pk);
	case CMD_MC_ENDPLAY: return SC_EndPlay(pk);
	case CMD_MC_NEWCHA: return SC_NewCha(pk);
	case CMD_MC_DELCHA: return SC_DelCha(pk); //===
	case CMD_MC_CREATE_PASSWORD2: return SC_CreatePassword2(pk);
	case CMD_MC_UPDATE_PASSWORD2: return SC_UpdatePassword2(pk);
	case CMD_MC_CHABEGINSEE: return SC_ChaBeginSee(pk);
	case CMD_MC_CHAENDSEE: return SC_ChaEndSee(pk);
	case CMD_MC_ADD_ITEM_CHA: return SC_AddItemCha(pk);
	case CMD_MC_DEL_ITEM_CHA: return SC_DelItemCha(pk);
	case CMD_MC_NOTIACTION: return SC_CharacterAction(pk);
	case CMD_MC_SAY: return SC_Say(pk);
	case CMD_MC_SYSINFO: return SC_SysInfo(pk);

	case CMD_PC_GUILDNOTICE: return SC_GuildInfo(pk);

	case CMD_MC_POPUP_NOTICE: return SC_PopupNotice(pk);
	case CMD_MC_ITEMBEGINSEE: return SC_ItemCreate(pk);
	case CMD_MC_ITEMENDSEE: return SC_ItemDestroy(pk);
	case CMD_MC_TALKPAGE: return SC_TalkInfo(pk);
	case CMD_MC_FUNCPAGE: return SC_FuncInfo(pk);
	case CMD_MC_CLOSETALK: return SC_CloseTalk(pk);
	case CMD_MC_HELPINFO: return SC_HelpInfo(pk);
	case CMD_MC_TRADEPAGE: return SC_TradeInfo(pk);
	case CMD_MC_BLACKMARKET_TRADEUPDATE: return SC_TradeUpdate(pk);
	case CMD_MC_TRADE_DATA: return SC_TradeData(pk);
	case CMD_MC_TRADE_ALLDATA: return SC_TradeAllData(pk);
	case CMD_MC_TRADERESULT: return SC_TradeResult(pk);
	case CMD_MC_CHARTRADE: return SC_CharTradeInfo(pk);
	case CMD_MC_SYNATTR: return SC_SynAttribute(pk);
	case CMD_MC_SYNSKILLBAG: return SC_SynSkillBag(pk);
	case CMD_MC_SYNDEFAULTSKILL: return SC_SynDefaultSkill(pk);
	case CMD_MC_SYNASKILLSTATE: return SC_SynSkillState(pk);
	case CMD_MC_MISSION: return SC_MissionInfo(pk);
	case CMD_MC_MISPAGE: return SC_MisPage(pk);
	case CMD_MC_MISLOG: return SC_MisLog(pk);
	case CMD_MC_MISLOGINFO: return SC_MisLogInfo(pk);
	case CMD_MC_MISLOG_CLEAR: return SC_MisLogClear(pk);
	case CMD_MC_MISLOG_ADD: return SC_MisLogAdd(pk);
	case CMD_MC_MISLOG_CHANGE: return SC_MisLogState(pk);
	case CMD_MC_NPCSTATECHG: return SC_NpcStateChange(pk);

	case CMD_PC_ERRMSG:
	case CMD_MC_MAPCRASH: return SC_MapCrash(pk);

	case CMD_MC_TRIGGER_ACTION: return SC_TriggerAction(pk);
	case CMD_MC_BEGIN_ITEM_FORGE: return SC_Forge(pk);

	case CMD_MC_BEGIN_ITEM_UNITE: return SC_Unite(pk);
	case CMD_MC_BEGIN_ITEM_MILLING: return SC_Milling(pk);
	case CMD_MC_BEGIN_ITEM_FUSION: return SC_Fusion(pk);
	case CMD_MC_BEGIN_ITEM_UPGRADE: return SC_Upgrade(pk);
	case CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS: return SC_EidolonMetempsychosis(pk);
	case CMD_MC_BEGIN_ITEM_EIDOLON_FUSION: return SC_Eidolon_Fusion(pk);
	case CMD_MC_BEGIN_ITEM_PURIFY: return SC_Purify(pk);
	case CMD_MC_BEGIN_ITEM_ENERGY: return SC_Energy(pk);
	case CMD_MC_BEGIN_GET_STONE: return SC_GetStone(pk);
	case CMD_MC_BEGIN_TIGER: return SC_Tiger(pk);
	case CMD_MC_BEGIN_ITEM_FIX: return SC_Fix(pk);
	case CMD_MC_BEGIN_GM_SEND: return SC_GMSend(pk);
	case CMD_MC_BEGIN_GM_RECV: return SC_GMRecv(pk);

	case CMD_MC_CREATEBOAT: return SC_CreateBoat(pk);
	case CMD_MC_UPDATEBOAT: return SC_UpdateBoat(pk);
	case CMD_MC_UPDATEBOAT_PART: return SC_UpdateBoatPart(pk);
	case CMD_MC_BERTH_LIST: return SC_BoatList(pk);
	case CMD_MC_BOATINFO: return SC_BoatInfo(pk);
	//case CMD_MC_BOAT_BAGLIST: return SC_BoatBagList(pk);
	case CMD_MC_BICKER_NOTICE: return SC_BickerNotice(pk);
	case CMD_MC_COLOUR_NOTICE: return SC_ColourNotice(pk);
	case CMD_MC_STALL_ALLDATA: return SC_StallInfo(pk);
	//case CMD_MC_STALL_UPDATE: return ;
	case CMD_MC_STALL_DELGOODS: return SC_StallDelGoods(pk);
	case CMD_MC_STALL_CLOSE: return SC_StallClose(pk);
	case CMD_MC_STALL_START: return SC_StallSuccess(pk);
	case CMD_MC_STARTEXIT: return SC_StartExit(pk);
	case CMD_MC_CANCELEXIT: return SC_CancelExit(pk);
	case CMD_MC_RANK: return SC_ShowRanking(pk);
	case CMD_MC_STALLSEARCH: return SC_ShowStallSearch(pk);
	case CMD_MC_UPDATEGUILDBANKGOLD: return SC_UpdateGuildGold(pk);

	case CMD_MC_UPDATEIMP: g_stUIEquip.UpdateIMP(pk.ReadInt64());
		return true;

	case CMD_MC_REQUESTPIN: {
		CCursor::I()->SetCursor(CCursor::stNormal);
		g_stUIDoublePwd.SetType(CDoublePwdMgr::MC_REQUEST);
		g_stUIDoublePwd.ShowDoublePwdForm();
		return true;
	}


	case CMD_PC_SAY2YOU: return PC_Say2You(pk);
	case CMD_PC_SAY2TEM: return PC_Say2Team(pk);
	case CMD_PC_SAY2GUD: return PC_Say2Gud(pk);
	case CMD_PC_SAY2ALL: {
		return PC_Say2All(pk);
	}
	case CMD_PC_SAY2TRADE: return PC_SAY2TRADE(pk);
	case CMD_PC_GM1SAY: return PC_GM1SAY(pk);
	case CMD_PC_GM1SAY1: return PC_GM1SAY1(pk); //add by sunny.sun20080804

	case CMD_PC_SESS_CREATE: return PC_SESS_CREATE(pk);
	case CMD_PC_SESS_ADD: return PC_SESS_ADD(pk);
	case CMD_PC_SESS_LEAVE: return PC_SESS_LEAVE(pk);
	case CMD_PC_SESS_SAY: return PC_SESS_SAY(pk);

	case CMD_PC_FRND_INVITE: return PC_FRND_INVITE(pk);
	case CMD_PC_FRND_CANCEL: return PC_FRND_CANCEL(pk);
	case CMD_PC_FRND_REFRESH: return PC_FRND_REFRESH(pk);
	case CMD_PC_FRND_REFRESH_INFO: return PC_FRND_REFRESH_INFO(pk);

	case CMD_PC_CHANGE_PERSONINFO: return PC_CHANGE_PERSONINFO(pk);
	case CMD_PC_REGISTER: return PC_REGISTER(pk);
	case CMD_PC_GM_INFO: return PC_GM_INFO(pk);

	case CMD_MC_GUILD_GETNAME: return MC_GUILD_GETNAME(pk);
	case CMD_MC_LISTGUILD: return MC_LISTGUILD(pk);
	case CMD_MC_GUILD_TRYFORCFM: return MC_GUILD_TRYFORCFM(pk);
	case CMD_MC_GUILD_LISTTRYPLAYER: return MC_GUILD_LISTTRYPLAYER(pk);
	case CMD_PC_GUILD: return PC_GUILD(pk);
	case CMD_PC_GUILD_PERM: return PC_GUILD_PERM(pk);
	case CMD_MC_GUILD_MOTTO: return MC_GUILD_MOTTO(pk);
	case CMD_MC_GUILD_LEAVE: return MC_GUILD_LEAVE(pk);
	case CMD_MC_GUILD_KICK: return MC_GUILD_KICK(pk);
	case CMD_MC_GUILD_INFO: return MC_GUILD_INFO(pk);
	case CMD_MC_GUILD_LISTCHALL: return MC_GUILD_LISTCHALL(pk);
	case CMD_PC_TEAM_INVITE: return PC_TEAM_INVITE(pk);
	case CMD_PC_TEAM_CANCEL: return PC_TEAM_CANCEL(pk);
	case CMD_PC_TEAM_REFRESH: return PC_TEAM_REFRESH(pk);
	case CMD_MC_TEAM: return SC_SynTeam(pk);
	case CMD_MC_TLEADER_ID: return SC_SynTLeaderID(pk);

	case CMD_MC_FAILEDACTION: return SC_FailedAction(pk);
	case CMD_MC_MESSAGE: return SC_Message(pk);
	case CMD_MC_ASTATEBEGINSEE: return SC_AStateBeginSee(pk);
	case CMD_MC_ASTATEENDSEE: return SC_AStateEndSee(pk);
	case CMD_MC_CHA_EMOTION: return SC_Cha_Emotion(pk);

	case CMD_MC_QUERY_CHA: return SC_QueryCha(pk);
	case CMD_MM_QUERY_CHAITEM: return SC_QueryChaItem(pk);
	case CMD_MC_QUERY_CHAPING: return SC_QueryChaPing(pk);
	case CMD_MC_QUERY_RELIVE: return SC_QueryRelive(pk);
	case CMD_MC_SEND_SERVER_PUBLIC_KEY: return SC_SendPublicKey(pk);
	case CMD_MC_SEND_HANDSHAKE: return SC_SendHandshake(pk);
	case CMD_MC_PREMOVE_TIME: return SC_PreMoveTime(pk);
	case CMD_MC_MAP_MASK: return SC_MapMask(pk);

	case CMD_MC_UPDATEHAIR_RES: return SC_UpdateHairRes(pk);
	case CMD_MC_OPENHAIR: return SC_OpenHairCut(pk);
	case CMD_MC_STALL_NAME: return SC_SynStallName(pk);
	case CMD_MC_EVENT_INFO: return SC_SynEventInfo(pk);
	case CMD_MC_SIDE_INFO: return SC_SynSideInfo(pk);

	case CMD_MC_TEAM_FIGHT_ASK: return SC_TeamFightAsk(pk);
	case CMD_MC_ITEM_REPAIR_ASK: return SC_ItemRepairAsk(pk);
	case CMD_MC_BEGIN_ITEM_REPAIR: return SC_BeginItemRepair(pk);
	case CMD_MC_APPEND_LOOK: return SC_SynAppendLook(pk);
	case CMD_MC_ITEM_FORGE_ASK: return SC_ItemForgeAsk(pk);
	case CMD_MC_ITEM_FORGE_ASR: return SC_ItemForgeAnswer(pk);

	case CMD_MC_ITEM_USE_SUC: return SC_ItemUseSuc(pk);
	case CMD_MC_KITBAG_CAPACITY: return SC_KitbagCapacity(pk);
	case CMD_MC_ESPE_ITEM: return SC_EspeItem(pk);

	case CMD_MC_KITBAG_CHECK_ASR: return SC_KitbagCheckAnswer(pk);

	case CMD_MC_STORE_OPEN_ASR: return SC_StoreOpenAnswer(pk);
	case CMD_MC_STORE_LIST_ASR: return SC_StoreListAnswer(pk);
	case CMD_MC_STORE_BUY_ASR: return SC_StoreBuyAnswer(pk);
	case CMD_MC_STORE_CHANGE_ASR: return SC_StoreChangeAnswer(pk);
	case CMD_MC_STORE_QUERY: return SC_StoreHistory(pk);
	case CMD_MC_STORE_VIP: return SC_StoreVIP(pk);

	case CMD_MC_BLACKMARKET_EXCHANGEDATA: return SC_BlackMarketExchangeData(pk);
	case CMD_MC_BLACKMARKET_EXCHANGE_ASR: return SC_BlackMarketExchangeAsr(pk);
	case CMD_MC_BLACKMARKET_EXCHANGEUPDATE: return SC_BlackMarketExchangeUpdate(pk);
	case CMD_MC_EXCHANGEDATA: return SC_ExchangeData(pk);

	case CMD_MC_TIGER_ITEM_ID: return SC_TigerItemID(pk);

	case CMD_PC_GARNER2_ORDER: return PC_PKSilver(pk);

	case CMD_MC_LIFESKILL_BGING: return SC_LifeSkillShow(pk);
	case CMD_MC_LIFESKILL_ASK: return SC_LifeSkill(pk);
	case CMD_MC_LIFESKILL_ASR: return SC_LifeSkillAsr(pk);
	case CMD_CM_ITEM_LOCK_ASR: return SC_DropLockAsr(pk);
	case CMD_MC_ITEM_UNLOCK_ASR: return SC_UnlockItemAsr(pk);
	case CMD_MC_VOLUNTER_LIST: return SC_VolunteerList(pk);
	case CMD_MC_VOLUNTER_STATE: return SC_VolunteerState(pk);
	case CMD_MC_VOLUNTER_OPEN: return SC_VolunteerOpen(pk);
	case CMD_MC_VOLUNTER_ASK: return SC_VolunteerAsk(pk);

	case CMD_MC_KITBAGTEMP_SYNC: return SC_SyncKitbagTemp(pk);

	case CMD_MC_MASTER_ASK: return SC_MasterAsk(pk);
	case CMD_PC_MASTER_REFRESH: return PC_MasterRefresh(pk);
	case CMD_PC_MASTER_CANCEL: return PC_MasterCancel(pk);
	case CMD_PC_MASTER_REFRESH_INFO: return PC_MasterRefreshInfo(pk);
	case CMD_PC_PRENTICE_REFRESH_INFO: return PC_PrenticeRefreshInfo(pk);
	case CMD_MC_PRENTICE_ASK: return SC_PrenticeAsk(pk);
	case CMD_MC_TIGER_STOP: return SC_SyncTigerString(pk);

	case CMD_MC_CHAPLAYEFFECT: return SC_ChaPlayEffect(pk);

	case CMD_MC_SAY2CAMP: return SC_Say2Camp(pk);

	case CMD_MC_GM_MAIL: return SC_GMMail(pk);

	case CMD_MC_CHEAT_CHECK: return SC_CheatCheck(pk);
	case CMD_MC_LISTAUCTION: return SC_ListAuction(pk);
	case CMD_MC_RecDailyBuffInfo: return SC_DailyBuffInfo(pk);

	case CMD_MC_REQUEST_DROP_RATE: return SC_RequestDropRate(pk);
	case CMD_MC_REQUEST_EXP_RATE: return SC_RequestExpRate(pk);
	case CMD_TC_DISCONNECT: return SC_Disconnect(pk);
	case CMD_PC_REFRESH_SELECT: return SC_RefreshSelectScreen(pk);

	// Пинг — обрабатываем здесь (ранее обрабатывались в OnProcessData)
	case CMD_PC_PING: return PC_Ping(pk);
	case CMD_MC_PING: return SC_Ping(pk);
	case CMD_MC_CHECK_PING: return SC_CheckPing(pk);
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// class CLogName
//---------------------------------------------------------------------------
CLogName::CLogName() {
	Init();
}

void CLogName::Init() {
	memset(_dwWorldArray, 0, sizeof(_dwWorldArray));
	memset(_szLogName, 0, sizeof(_szLogName));
	memset(_szNoFind, 0, sizeof(_szNoFind));
}

const char* CLogName::SetLogName(DWORD dwWorlID, const char* szName) {
	if (!g_pGameApp->GetLGConfig()->bEnableAll)
		return "nolog";

	for (int i = 0; i < LOG_MAX; i++) {
		if (_dwWorldArray[i] == 0) {
			_dwWorldArray[i] = dwWorlID;
			sprintf(_szLogName[i], "%s+%u", szName, dwWorlID);
			return _szLogName[i];
		}
	}

	sprintf(_szNoFind, "nofind(s)-%u", dwWorlID);
	return _szNoFind;
}

const char* CLogName::GetLogName(DWORD dwWorlID) {
	if (!g_pGameApp->GetLGConfig()->bEnableAll)
		return "nolog";

	for (int i = 0; i < LOG_MAX; i++)
		if (_dwWorldArray[i] == dwWorlID)
			return _szLogName[i];

	sprintf(_szNoFind, "nofind(g)-%u", dwWorlID);
	return _szNoFind;
}

const char* CLogName::GetMainLogName() {
	if (!g_pGameApp->GetLGConfig()->bEnableAll)
		return "nolog";

	CCharacter* pMain = CGameScene::GetMainCha();
	if (pMain) {
		return GetLogName(pMain->getAttachID());
	}

	sprintf(_szNoFind, "nofind(m)");
	return _szNoFind;
}

bool CLogName::IsMainCha(DWORD dwWorlID) {
	CCharacter* pMain = CGameScene::GetMainCha();
	if (pMain) {
		return pMain->getAttachID() == dwWorlID;
	}

	return false;
}

inline int lua_HandleNetMessage(lua_State* L) {
	BOOL bValid = (lua_gettop(L) == 1 && lua_islightuserdata(L, 1));
	if (!bValid) {
		return 0;
	}

	net::RPacket* pPacket = (net::RPacket*)lua_touserdata(L, 1);
	if (pPacket) {
		g_NetIF->HandlePacketMessage(*pPacket);
	}
	return 0;
}

//---------------------------------------------------------------------------
// class NetIF
//---------------------------------------------------------------------------
NetIF::NetIF()
	: m_framedelay(40)
	  , m_maxdelay(0), m_curdelay(0), m_mindelay(0), m_pingid(0)
	  , m_connect(this)
	  , m_ulCurStatistic(0), m_ulPacketCount(0)
	  , _enc(false), _comm_enc(0) {
	net::InitWinSock();

	// Настройка TcpClient
	_client.SetHandler(this);
	_client.SetCrypto(this);

	// Инициализация шифрования
	memset(_key, 0, sizeof _key);
	_key_len = 0;
	g_rLvm = init_lua();
	g_sLvm = init_lua();
	load_luc(g_rLvm, "scripts/lua/apple.luc");
	load_luc(g_sLvm, "scripts/lua/apple.luc");
	load_luc(g_rLvm, "scripts/lua/pear.luc");
	load_luc(g_sLvm, "scripts/lua/pear.luc");

	handshakeDone = false;
	hRsaPubKey = NULL;
	hAesAlg = NULL;
	hAesKey = NULL;
	memset(cliAesKey, 0, sizeof(cliAesKey));
	memset(m_ulDelayTime, 0, sizeof(unsigned long) * 4);

	m_pCProCir = new CProCirculateCC(this);
}

NetIF::~NetIF() {
	_enc = false;
	memset(_key, 0, sizeof _key);
	exit_lua(g_sLvm);
	exit_lua(g_rLvm);

	CleanupCrypto();

	_client.Disconnect(0);

	if (m_pCProCir) {
		delete m_pCProCir;
		m_pCProCir = NULL;
	}

	net::CleanupWinSock();
}

void NetIF::CleanupCrypto() {
	if (hAesKey) {
		BCryptDestroyKey(hAesKey);
		hAesKey = NULL;
	}
	if (hAesAlg) {
		BCryptCloseAlgorithmProvider(hAesAlg, 0);
		hAesAlg = NULL;
	}
	if (hRsaPubKey) {
		BCryptDestroyKey(hRsaPubKey);
		hRsaPubKey = NULL;
	}
	SecureZeroMemory(cliAesKey, sizeof(cliAesKey));
	handshakeDone = false;
}

bool NetIF::InitAesKey() {
	NTSTATUS status;

	status = BCryptOpenAlgorithmProvider(&hAesAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptOpenAlgorithmProvider failed: 0x%08X\n", status);
		return false;
	}

	status = BCryptSetProperty(hAesAlg, BCRYPT_CHAINING_MODE,
							   (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptSetProperty GCM failed: 0x%08X\n", status);
		return false;
	}

	status = BCryptGenerateSymmetricKey(hAesAlg, &hAesKey, NULL, 0, cliAesKey, 32, 0);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptGenerateSymmetricKey failed: 0x%08X\n", status);
		return false;
	}

	return true;
}

// ── ITcpClientHandler ────────────────────────────────────────

void NetIF::OnPacket(net::RPacket& packet) {
	unsigned short sCmdType = packet.GetCmd();

	if (sCmdType != CMD_MC_PING) {
		LG("FromGateServer", "%d\n", sCmdType);
		// [IN] логи временно отключены для отладки SESS-счётчика
		//EnsureConsole();
		//printf("[IN ] CMD=%5u %-40s SESS=%10u SIZE=%d\n",
		//	   sCmdType, GetCmdName(sCmdType), packet.GetSess(), packet.GetPacketSize());
	}

	HandlePacketMessage(packet);
}

void NetIF::OnDisconnected(int reason) {
	LG("connect", "\tOnDisconnected, Reason:%d\n", reason);

	if (g_pGameApp) {
		CleanupCrypto();
		m_ulPacketCount = 0;
		m_connect.OnDisconnect();
		g_pGameApp->SendMessage(APP_NET_DISCONNECT, reason);

		if (m_pCProCir) {
			delete m_pCProCir;
		}
		m_pCProCir = new CProCirculateCC(this);
	}
}

// ── ICryptoProvider ──────────────────────────────────────────

bool NetIF::IsActive() const {
	return _comm_enc > 0 && handshakeDone;
}

bool NetIF::Encrypt(uint8_t* ciphertext, int ciphertext_len,
					const uint8_t* plaintext, int& len) {
	unsigned long ulen = static_cast<unsigned long>(len);
	bool ok = EncryptAES(
		reinterpret_cast<char*>(ciphertext),
		static_cast<unsigned long>(ciphertext_len),
		reinterpret_cast<const char*>(plaintext),
		ulen);
	len = static_cast<int>(ulen);
	return ok;
}

bool NetIF::Decrypt(uint8_t* data, int& len) {
	unsigned long ulen = static_cast<unsigned long>(len);
	bool ok = DecryptAES(reinterpret_cast<char*>(data), ulen);
	len = static_cast<int>(ulen);
	return ok;
}

// ── SwitchNet ────────────────────────────────────────────────

void NetIF::SwitchNet(bool isConnected) {
	if (m_pCProCir) delete m_pCProCir;
	if (isConnected) {
		m_pCProCir = new CProCirculateCS(this);
	}
	else {
		m_pCProCir = new CProCirculateCC(this);
	}
}

std::string NetIF::GetDisconnectErrText(int reason) const {
	return [&]()-> std::string {
		switch (reason) {
		case -33: return "Offline mode has been successfully established, you may now close the client";
		default: return g_oLangRec.GetString(138);
		}
	}();
}

unsigned long NetIF::GetAveragePing() {
	unsigned long ulAverage = 0, ulCount = 0;

	m_mutmov.lock();

	for (int i = 0; i < 4; i++) {
		if (m_ulDelayTime[i] <= 0)
			break;
		ulAverage += m_ulDelayTime[i];
		ulCount++;
	}

	m_mutmov.unlock();

	if (ulCount > 0)
		ulAverage = (ulAverage + ulCount - 1) / ulCount;
	if (ulAverage <= 0)
		ulAverage = 1;

	return ulAverage;
}

//------------- Packet отправка    Client -> Server ----------------------------
void NetIF::SendPacketMessage(LPWPACKET pk) {
	BOOL bUseFakeServer = FALSE;
	if (bUseFakeServer) {
		return;
	}

	if (!IsConnected()) return;

	if (!_client.IsConnected()) {
		LG("error", "msgClientSocket Is NULL, Can't Send Socket Message!\n");
		return;
	}

	// WPE: записываем инкрементальный счётчик пакетов в поле SESS (байты 2-5)
	pk.WriteSess(m_ulPacketCount++);

	{
		unsigned short cmd = pk.GetCmd();
		if (cmd != CMD_CM_PING) {
			EnsureConsole();
			printf("[OUT] CMD=%5u %-40s SESS=%10u SIZE=%d\n",
				   cmd, GetCmdName(cmd), pk.GetSess(), pk.GetPacketSize());
		}
	}

	if (!_client.Send(pk)) {
		LG("net_error", "msgSendData Error!\n");
	}
}


// AES-256-GCM шифрование (BCrypt). Wire format: [nonce(12)][tag(16)][ciphertext]
bool NetIF::EncryptAES(char* ciphertext, uLong ciphertext_len, cChar* plaintext, unsigned long& ciphersize) {
	const ULONG NONCE_SIZE = 12;
	const ULONG TAG_SIZE = 16;
	const ULONG overhead = NONCE_SIZE + TAG_SIZE;
	ULONG plaintextLen = ciphersize;

	if (ciphertext_len < overhead + plaintextLen) {
		LG("enc", "EncryptAES: buffer too small (%u < %u)\n", ciphertext_len, overhead + plaintextLen);
		return false;
	}

	BYTE nonce[NONCE_SIZE];
	NTSTATUS status = BCryptGenRandom(NULL, nonce, NONCE_SIZE, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptGenRandom failed: 0x%08X\n", status);
		return false;
	}

	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
	authInfo.pbNonce = nonce;
	authInfo.cbNonce = NONCE_SIZE;

	BYTE tag[TAG_SIZE];
	authInfo.pbTag = tag;
	authInfo.cbTag = TAG_SIZE;

	ULONG resultLen = 0;
	char* ctOut = ciphertext + overhead;
	if (plaintext != ciphertext) {
		std::memcpy(ctOut, plaintext, plaintextLen);
	}
	else {
		std::memmove(ctOut, plaintext, plaintextLen);
	}

	status = BCryptEncrypt(
		hAesKey,
		(PUCHAR)ctOut, plaintextLen,
		&authInfo,
		NULL, 0,
		(PUCHAR)ctOut, plaintextLen,
		&resultLen,
		0);

	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptEncrypt failed: 0x%08X\n", status);
		return false;
	}

	std::memcpy(ciphertext, nonce, NONCE_SIZE);
	std::memcpy(ciphertext + NONCE_SIZE, tag, TAG_SIZE);
	ciphersize = overhead + resultLen;

	return true;
}

// AES-256-GCM дешифрование (BCrypt). Wire format: [nonce(12)][tag(16)][ciphertext]
bool NetIF::DecryptAES(char* ciphertext, unsigned long& len) {
	const ULONG NONCE_SIZE = 12;
	const ULONG TAG_SIZE = 16;
	const ULONG overhead = NONCE_SIZE + TAG_SIZE;

	if (len < overhead) {
		LG("dec", "DecryptAES: data too short (%u < %u)\n", len, overhead);
		return false;
	}

	ULONG ctLen = len - overhead;

	BYTE nonce[NONCE_SIZE];
	BYTE tag[TAG_SIZE];
	std::memcpy(nonce, ciphertext, NONCE_SIZE);
	std::memcpy(tag, ciphertext + NONCE_SIZE, TAG_SIZE);

	PUCHAR ctData = (PUCHAR)ciphertext + overhead;

	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
	authInfo.pbNonce = nonce;
	authInfo.cbNonce = NONCE_SIZE;
	authInfo.pbTag = tag;
	authInfo.cbTag = TAG_SIZE;

	ULONG resultLen = 0;
	NTSTATUS status = BCryptDecrypt(
		hAesKey,
		ctData, ctLen,
		&authInfo,
		NULL, 0,
		(PUCHAR)ciphertext, ctLen,
		&resultLen,
		0);

	if (!BCRYPT_SUCCESS(status)) {
		LG("dec", "BCryptDecrypt failed: 0x%08X\n", status);
		return false;
	}

	len = resultLen;
	return true;
}
