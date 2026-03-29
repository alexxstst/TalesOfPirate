#include "Stdafx.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "CommandMessages.h"
#include "SubMap.h"
#include "MapEntry.h"
#include "CharTrade.h"
#include "GameDB.h"

using namespace std;

extern std::string g_strLogName;

void CGameApp::ProcessNetMsg(int nMsgType, GateServer* pGate, net::RPacket& pkt) {
	switch (nMsgType) {
	case NETMSG_GATE_CONNECTED: // ??????Gate
	{
		ToLogService("network", "Exec OnGateConnected()");
		OnGateConnected(pGate, pkt);
		break;
	}

	case NETMSG_GATE_DISCONNECT: // ??Gate???????
	{
		OnGateDisconnect(pGate, pkt);
		break;
	}

	case NETMSG_PACKET: // ????????
	{
		ProcessPacket(pGate, pkt);
		break;
	}
	}
}

void CGameApp::ProcessInfoMsg(pNetMessage msg, short sType, InfoServer* pInfo) {
	switch (sType) {
	case InfoServer::CMD_FM_CONNECTED:
		OnInfoConnected(pInfo);
		break;

	case InfoServer::CMD_FM_DISCONNECTED:
		OnInfoDisconnected(pInfo);
		break;

	case InfoServer::CMD_FM_MSG:
		ProcessMsg(msg, pInfo);
		break;

	default:
		break;
	}
}

void CGameApp::OnInfoConnected(InfoServer* pInfo) {
	//??InfoServer
	pInfo->Login();
}

void CGameApp::OnInfoDisconnected(InfoServer* pInfo) {
	// ????????
	g_StoreSystem.InValid();
	pInfo->InValid();
}

void CGameApp::ProcessMsg(pNetMessage msg, InfoServer* pInfo) {
	if (msg) {
		switch (msg->msgHead.msgID) {
		case INFO_LOGIN: // ??InfoServer
		{
			if (msg->msgHead.subID == INFO_SUCCESS) {
				ToLogService("store", "InfoServer Login Success!");

				pInfo->SetValid();
				//g_StoreSystem.SetValid();

				//??InfoServer?????????????????
				g_StoreSystem.GetItemList();
				g_StoreSystem.GetAfficheList();
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				ToLogService("store", "InfoServer Login Failed!");
				pInfo->InValid();
			}
			else {
				//ToLogService("store", "??InfoServer???????????!");
				ToLogService("store", "enter InfoServer message data error!");
			}
		}
		break;

		case INFO_REQUEST_ACCOUNT: // ?????????
		{
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????????????!", lOrderID);
				ToLogService("store", "[{}]succeed to obtain account information!", lOrderID);

				RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));
				g_StoreSystem.AcceptRoleInfo(lOrderID, ChaInfo);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????????????!", lOrderID);
				ToLogService("store", "[{}]obtain account information failed!", lOrderID);

				g_StoreSystem.CancelRoleInfo(lOrderID);
			}
			else {
				//ToLogService("store", "?????????????????!");
				ToLogService("store", "account information message data error");
			}
		}
		break;

		case INFO_REQUEST_STORE: // ?????????
		{
			//ToLogService("store", "?????????!");
			ToLogService("store", "get store list!");
			if (msg->msgHead.subID == INFO_SUCCESS) // ??????????
			{
				//???????
				short lComNum = LOWORD(msg->msgHead.msgExtend);
				//???????
				short lClassNum = HIWORD(msg->msgHead.msgExtend);
				//?????????
				g_StoreSystem.SetItemClass((ClassInfo*)(msg->msgBody), lClassNum);
				//??????????
				g_StoreSystem.SetItemList((StoreStruct*)((char*)msg->msgBody + lClassNum * sizeof(ClassInfo)), lComNum);

				g_StoreSystem.SetValid();
			}
			else if (msg->msgHead.subID == INFO_FAILED) // ??????????
			{
				//???????
				short lComNum = LOWORD(msg->msgHead.msgExtend);
				//???????
				short lClassNum = HIWORD(msg->msgHead.msgExtend);
				//?????????
				g_StoreSystem.SetItemClass((ClassInfo*)(msg->msgBody), lClassNum);
				//??????????
				g_StoreSystem.SetItemList((StoreStruct*)((char*)msg->msgBody + lClassNum * sizeof(ClassInfo)), lComNum);
			}
			else {
				//ToLogService("store", "?????????????????!");
				ToLogService("store", "store list message data error!");
			}
		}
		break;

		case INFO_REQUEST_AFFICHE: // ??????????
		{
			//ToLogService("store", "????????!");
			ToLogService("store", "get offiche information!");
			if (msg->msgHead.subID == INFO_SUCCESS) // ???????????
			{
				//???????
				long lAfficheNum = msg->msgHead.msgExtend;
				//?????????
				g_StoreSystem.SetAfficheList((AfficheInfo*)msg->msgBody, lAfficheNum);
			}
			else if (msg->msgHead.subID == INFO_FAILED) // ???????????
			{
				//???????
				long lAfficheNum = msg->msgHead.msgExtend;
				//?????????
				g_StoreSystem.SetAfficheList((AfficheInfo*)msg->msgBody, lAfficheNum);
			}
			else {
				//ToLogService("store", "??????????????????!");
				ToLogService("store", "offiche information message data error!");
			}
		}
		break;

		case INFO_STORE_BUY: // ???????
		{
			if (msg->msgHead.subID == INFO_SUCCESS) // ??????
			{
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]?????????!", lOrderID);
				ToLogService("store", "[{}]succeed to buy item!", lOrderID);

				RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));
				g_StoreSystem.Accept(lOrderID, ChaInfo);
			}
			else if (msg->msgHead.subID == INFO_FAILED) // ???????
			{
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]??????????!", lOrderID);
				ToLogService("store", "[{}]buy item failed!", lOrderID);

				g_StoreSystem.Cancel(lOrderID);
			}
			else {
				//ToLogService("store", "????????????????????????!");
				ToLogService("store", "confirm information that buy item message data error!");
			}
		}
		break;

		case INFO_REGISTER_VIP: {
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????VIP???!", lOrderID);
				ToLogService("store", "[{}] buy VIP succeed !", lOrderID);

				RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));

				g_StoreSystem.AcceptVIP(lOrderID, ChaInfo, msg->msgHead.msgExtend);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????VIP???!", lOrderID);
				ToLogService("store", "[{}] buy VIP failed !", lOrderID);

				g_StoreSystem.CancelVIP(lOrderID);
			}
			else {
				//ToLogService("store", "????VIP?????????????????!");
				ToLogService("store", "buy VIP confirm information message data error !");
			}
		}
		break;

		case INFO_EXCHANGE_MONEY: // ???????
		{
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]?????????!", lOrderID);
				ToLogService("store", "[{}]change token succeed !", lOrderID);

				RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));
				g_StoreSystem.AcceptChange(lOrderID, ChaInfo);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]??????????!", lOrderID);
				ToLogService("store", "[{}]change token failed!", lOrderID);

				g_StoreSystem.CancelChange(lOrderID);
			}
			else {
				//ToLogService("store", "????????????????????????!");
				ToLogService("store", "change token confirm information message data error !");
			}
		}
		break;

		case INFO_REQUEST_HISTORY: // ????????
		{
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]???????????!", lOrderID);
				ToLogService("store", "[{}]succeed to query trade note!", lOrderID);

				HistoryInfo* pRecord = (HistoryInfo*)((char*)msg->msgBody + sizeof(long long));
				g_StoreSystem.AcceptRecord(lOrderID, pRecord);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]???????????!", lOrderID);
				ToLogService("store", "[{}]query trade note failed!", lOrderID);

				g_StoreSystem.CancelRecord(lOrderID);
			}
			else {
				//ToLogService("store", "?????????????????????????!");
				ToLogService("store", "trade note query resoibsuib nessage data error!");
			}
		}
		break;

		case INFO_SND_GM_MAIL: {
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????GM??????!", lOrderID);
				ToLogService("store", "[{}]send GM mail success!", lOrderID);

				long lMailID = msg->msgHead.msgExtend;
				g_StoreSystem.AcceptGMSend(lOrderID, lMailID);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????GM??????!", lOrderID);
				ToLogService("store", "[{}]send GM mail failed!", lOrderID);

				g_StoreSystem.CancelGMSend(lOrderID);
			}
			else {
				//ToLogService("store", "????GM??????????????!");
				ToLogService("store", "send GM mail message data error!");
			}
		}
		break;

		case INFO_RCV_GM_MAIL: {
			if (msg->msgHead.subID == INFO_SUCCESS) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????GM??????!", lOrderID);
				ToLogService("store", "[{}]receive GM mail success!", lOrderID);

				MailInfo* pMi = (MailInfo*)((char*)msg->msgBody + sizeof(long long));
				g_StoreSystem.AcceptGMRecv(lOrderID, pMi);
			}
			else if (msg->msgHead.subID == INFO_FAILED) {
				long long lOrderID = *(long long*)msg->msgBody;
				//ToLogService("store", "[{}]????GM??????!", lOrderID);
				ToLogService("store", "[{}]reciveGMmail failed!", lOrderID);

				g_StoreSystem.CancelGMRecv(lOrderID);
			}
			else {
				//ToLogService("store", "????GM??????????????!");
				ToLogService("store", "receive GM mail message data error!");
			}
		}
		break;

		case INFO_EXCEPTION_SERVICE: //???????
		{
			//ToLogService("store", "InfoServer???????!");
			ToLogService("store", "InfoServer refuse serve!");
			g_StoreSystem.InValid();
			pInfo->InValid();
		}
		break;

		default: {
			//ToLogService("store", "?????????????!");
			ToLogService("store", "get unknown information type!");
		}
		break;
		}

		FreeNetMessage(msg);
	}
}

// ??Gate??????????????
void CGameApp::OnGateConnected(GateServer* pGate, net::RPacket& pkt) {
	// ??GateServer???GameServer
	//  :  GameServer  GateServer
	auto wpk = net::msg::serialize(net::msg::MtLoginMessage{GETGMSVRNAME(), g_pGameApp->m_strMapNameList.c_str()});

	ToLogService("network", "[{}]", g_pGameApp->m_strMapNameList.c_str());

	pGate->SendData(wpk);
}

// ??Gate???????????????
void CGameApp::OnGateDisconnect(GateServer* pGate, net::RPacket& pkt) {
	bool ret = static_cast<bool>(pkt);
	if (!ret) return; // ?????packet


	//NOTE(Ogge): Weird that we first cast to GatePlayer(base class) and then in while loop to CPlayer(derived class)
	auto tmp = ToPointer<GatePlayer>(pkt.ReadInt64());

	while (tmp != NULL) {
		if (static_cast<CPlayer*>(tmp)->IsValid()) {
			GoOutGame((CPlayer*)tmp, true);
			tmp->OnLogoff();
		}

		tmp = tmp->Next;
	}

	pGate->Invalid();
}

// ?????????????
void CGameApp::ProcessPacket(GateServer* pGate, net::RPacket& pkt) {
	CPlayer* l_player = nullptr;
	uShort cmd = pkt.ReadCmd();

	switch (cmd) {
	case CMD_TM_LOGIN_ACK: {
		net::msg::TmLoginAckMessage loginAck;
		net::msg::deserialize(pkt, loginAck);
		if (loginAck.errCode) {
			/*ToLogService("network", "?? GateServer: {}:{}???[{}], ?????[{}]",
				pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(loginAck.errCode),
				g_pGameApp->m_strMapNameList.c_str());*/
			ToLogService("network", "enter GateServer: {}:{} failed [{}], register map[{}]",
						 pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(loginAck.errCode),
						 g_pGameApp->m_strMapNameList.c_str());
			DISCONNECT(pGate);
		}
		else {
			pGate->GetName() = loginAck.gateName;
			if (!strcmp(pGate->GetName().c_str(), "")) {
				/*ToLogService("network", "?? GateServer: [{}:{}]??? ????????????????????????????",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());*/
				ToLogService(
					"network",
					"entry GateServer: [{}:{}]success but do not get his name??so disconnection and entry again",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());

				DISCONNECT(pGate);
			}
			else {
				/*ToLogService("network", "?? GateServer: {} [{}:{}]??? [MapName:{}]",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());*/
				ToLogService("network", "entry GateServer: {} [{}:{}]success [MapName:{}]",
							 pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
							 g_pGameApp->m_strMapNameList.c_str());
			}
		}

		break;
	}
	//	// Add by lark.li 20080921 begin
	//case CMD_TM_DELETEMAP:
	//	{
	//		int reason = pkt.ReadInt64();

	//		ToLogService("network", "Gate {} Ip {} {} deleted\r", pGate->GetName(), pGate->GetIP(), reason);

	//		pGate->Invalid();

	//		break;
	//	}
	//	//End

	case CMD_TM_ENTERMAP: {
		net::msg::TmEnterMapMessage enterMsg;
		net::msg::deserialize(pkt, enterMsg);

		if (enterMsg.password.empty())
			break;
		if (enterMsg.mapName.empty())
			break;

		ToLogService("map", "start entry map atorID = {} enter--------------------------", enterMsg.dbCharId);

		l_player = CreateGamePlayer(enterMsg.password.c_str(), enterMsg.dbCharId, enterMsg.worldId,
									enterMsg.mapName.c_str(), enterMsg.loginFlag == 0 ? 0 : 1);
		if (!l_player) {
			//  :       trailer 
			auto pkret = net::msg::serializeMcEnterMapError(ERR_MC_ENTER_ERROR, enterMsg.dbCharId, enterMsg.gateAddr);
			pGate->SendData(pkret);
			ToLogService("map", LogLevel::Error, "when create new palyer ID = {} assign memory failed",
						 enterMsg.dbCharId);
			return;
		}
		l_player->SetActLoginID(enterMsg.actId);
		l_player->SetGarnerWiner(enterMsg.winer);
		l_player->GetLifeSkillinfo() = "";
		l_player->SetInLifeSkill(false);

		if (!enterMsg.loginFlag)
			l_player->MisLogin();

		ADDPLAYER(l_player, pGate, enterMsg.gateAddr);
		l_player->OnLogin();

		CCharacter* pCCha = l_player->GetMainCha();
		if (pCCha->Cmd_EnterMap(enterMsg.mapName.c_str(), enterMsg.mapCopyNo, enterMsg.posX, enterMsg.posY,
								enterMsg.loginFlag)) {
			l_player->MisEnterMap();
			if (enterMsg.loginFlag == 0) {
				NoticePlayerLogin(l_player);
			}
		}

		ToLogService("map", "end up entry map  [{}]================", pCCha->GetLogName());
		break;
	}
	case CMD_TM_GOOUTMAP: {
		net::msg::TmGoOutMapMessage goOutMsg;
		net::msg::deserialize(pkt, goOutMsg);
		l_player = ToPointer<CPlayer>(goOutMsg.playerPtr);
		DWORD l_gateaddr = static_cast<DWORD>(goOutMsg.gateAddr);

		if (!l_player)
			break;
		try {
			if (l_player->GetGateAddr() != l_gateaddr) {
				//ToLogService("errors", LogLevel::Error, "?????ID: {}, ????????????:{:x}, gate:{:x},cmd={}, ?????({}).", l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr,cmd, l_player->IsValidFlag());
				ToLogService("errors", LogLevel::Error,
							 "DB ID: {}, address not matching??local :{:x}, gate:{:x},cmd={}, validity({}).",
							 l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr, cmd, l_player->IsValidFlag());
				break;
			}
		}
		catch (...) {
			//ToLogService("errors", LogLevel::Error, "===========================??Gate?????????????{},cmd ={}", l_player, cmd);
			ToLogService("errors", LogLevel::Error,
						 "===========================from Gate player's address error {},cmd ={}",
						 static_cast<void*>(l_player), static_cast<int>(cmd));
			break;
		}
		if (!l_player->IsValid()) {
			//LG("enter_map", "???????????\n");
			ToLogService("map", "this palyer already impotence");
			break;
		}
		if (l_player->GetMainCha()->GetPlayer() != l_player) {
			//ToLogService("errors", LogLevel::Error, "????player????????????{}??Gate???[????{}, ????{}]????cmd={}", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
			ToLogService("errors", LogLevel::Error,
						 "two player not matching, character name: {}, Gate address [local {}, guest {}], cmd={}",
						 l_player->GetMainCha()->GetLogName(), static_cast<void*>(l_player->GetMainCha()->GetPlayer()),
						 static_cast<void*>(l_player), static_cast<int>(cmd));
		}
		ToLogService("map", "start leave map--------");

		char chOffLine = static_cast<char>(goOutMsg.offlineFlag);
		ToLogService("map", "Delete Player [{}]", l_player->GetMainCha()->GetLogName());

		char szLogName[512];
		strncpy(szLogName, l_player->GetMainCha()->GetLogName(), 512 - 1);
		//LG("OutMap", "%s????????\n", szLogName);

		GoOutGame(l_player, !chOffLine, false);

		//LG("enter_map", "?????????========\n\n");
		ToLogService("map", "end and leave the map========");

		//LG("OutMap", "%s?????\n", szLogName);

		break;
	}
	case CMD_PM_SAY2ALL: {
		net::msg::PmSay2AllMessage say2AllMsg;
		net::msg::deserialize(pkt, say2AllMsg);
		uLong ulChaID = static_cast<uLong>(say2AllMsg.chaId);
		std::string szContent = say2AllMsg.content;
		long lChatMoney = static_cast<long>(say2AllMsg.money);

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer) {
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney)) {
				//pCha->SystemNotice("??????????,?????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00007));

				//  :   (  )
				auto l_wpk = net::msg::serialize(net::msg::GmSay2AllMessage{0, {}, {}});
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("?????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00006), lChatMoney);

			//  :   (CMD_MP_SAY2ALL)
			auto l_wpk = net::msg::serialize(net::msg::GmSay2AllMessage{1, pCha->GetName(), szContent});
			pCha->ReflectINFof(pCha, l_wpk);
		}
	}
	break;
	case CMD_PM_SAY2TRADE: {
		net::msg::PmSay2TradeMessage say2TradeMsg;
		net::msg::deserialize(pkt, say2TradeMsg);
		uLong ulChaID = static_cast<uLong>(say2TradeMsg.chaId);
		std::string szContent = say2TradeMsg.content;
		long lChatMoney = static_cast<long>(say2TradeMsg.money);

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer) {
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney)) {
				//pCha->SystemNotice("??????????,????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00005));

				//  :    (  )
				auto l_wpk = net::msg::serialize(net::msg::GmSay2TradeMessage{0, {}, {}});
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00004), lChatMoney);

			//  :    (CMD_MP_SAY2TRADE)
			auto l_wpk = net::msg::serialize(net::msg::GmSay2TradeMessage{1, pCha->GetName(), szContent});
			pCha->ReflectINFof(pCha, l_wpk);
		}
	}
	break;
	case CMD_PM_TEAM: // GroupServer  
	{
		net::msg::PmTeamMessage teamMsg;
		net::msg::deserialize(pkt, teamMsg);
		ProcessTeamMsg(pGate, teamMsg);
		break;
	}
	case CMD_PM_GUILDINFO: // GroupServer???????????
	{
		net::msg::PmGuildInfoMessage guildInfoMsg;
		net::msg::deserialize(pkt, guildInfoMsg);
		ProcessGuildMsg(pGate, guildInfoMsg);
		break;
	}
	case CMD_PM_GUILD_CHALLMONEY: {
		net::msg::PmGuildChallMoneyMessage challMoneyMsg;
		net::msg::deserialize(pkt, challMoneyMsg);
		ProcessGuildChallMoney(pGate, challMoneyMsg);
	}
	break;
	case CMD_PM_GUILD_CHALL_PRIZEMONEY: {
		//ProcessGuildChallPrizeMoney( pGate, pkt );

		//  :    
		net::msg::PmGuildChallPrizeMoneyMessage prizeMsg;
		net::msg::deserialize(pkt, prizeMsg);
		int64_t _guildId = prizeMsg.leaderId;
		int64_t _money = prizeMsg.money;
		auto WtPk = net::msg::serialize(net::msg::GmGuildChallPrizeMoneyBroadcast{0, _guildId, _money, 0, 0, 0});
		pGate->SendData(WtPk);
	}
	break;
	case CMD_TM_MAPENTRY: {
		net::msg::MapEntryMessage mapEntryMsg;
		net::msg::deserializeMapEntry(pkt, mapEntryMsg);
		ProcessDynMapEntry(pGate, mapEntryMsg);
		break;
	}
	/*case CMD_TM_KICKCHA:
		{
			long lChaDbID = pkt.ReadInt64();
			CPlayer* pCOldPly = FindPlayerByDBChaID(lChaDbID);
			if (!pCOldPly || !pCOldPly->IsValid())
				break;

			pCOldPly->GetCtrlCha()->BreakAction();
			pCOldPly->MisLogout();
			pCOldPly->MisGooutMap();
			ReleaseGamePlayer( pCOldPly );
			break;
		}*/
	case CMD_TM_MAPENTRY_NOMAP: {
		break;
	}
	case CMD_PM_GARNER2_UPDATE: {
		net::msg::PmGarner2UpdateLegacyMessage garnerMsg;
		net::msg::deserialize(pkt, garnerMsg);
		ProcessGarner2Update(garnerMsg);
		break;
	}
	case CMD_PM_EXPSCALE: {
		//  ??????
		net::msg::PmExpScaleMessage expMsg;
		net::msg::deserialize(pkt, expMsg);
		uLong ulChaID = static_cast<uLong>(expMsg.chaId);
		uLong ulTime = static_cast<uLong>(expMsg.time);

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);

		if (pPlayer) {
			CCharacter* pCha = pPlayer->GetMainCha();

			if (pCha->IsScaleFlag()) {
				break;
			}

			switch (ulTime) {
			case 1: {
				if (pCha->m_noticeState != 1) {
					pCha->m_noticeState = 1;
					//pCha->BickerNotice("???????????????? %d ????", ulTime);
					pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00001), ulTime);
					pCha->SetExpScale(100);
				}
			}
			break;
			case 2: {
				if (pCha->m_noticeState != 2) {
					pCha->m_noticeState = 2;
					//pCha->BickerNotice("???????????????? %d ????", ulTime);
					pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00001), ulTime);
					pCha->SetExpScale(100);
				}
			}
			break;
			case 3: {
				/*	yyy	2008-3-27 change	begin!
				if(!(pCha->m_retry3 % 30))
				{
					pCha->BickerNotice("???????????????? 3 ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????");
					pCha->SetExpScale(50);
				}

				pCha->m_retry3++;
				*/
				if (pCha->m_retry3 == 0)
					//pCha->PopupNotice("????????????????60??????????...");
					pCha->PopupNotice(RES_STRING(GM_GAMEAPPNET_CPP_00002));
				if (pCha->m_retry3 == 1) {
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
					//pCha->BickerNotice("???????????????? 3 ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????");
					//pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00002));
					//pCha->SetExpScale(50);
				}
				pCha->m_retry3++;
				//	yyy	change	end!
			}
			break;
			case 4: {
				/*	yyy	2008-3-27 change	begin!
				if(!(pCha->m_retry4 % 30))
				{
					//pCha->BickerNotice("?????????????????????????????????????????????????????????????????????????????????????????????");
					pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00003));
					pCha->SetExpScale(50);
				}
				pCha->m_retry4++;
				*/
				if (pCha->m_retry3 == 0)
					//pCha->PopupNotice("????????????????60??????????...");
					pCha->PopupNotice(RES_STRING(GM_GAMEAPPNET_CPP_00003));
				if (pCha->m_retry3 == 1) {
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
				}

				pCha->m_retry3++;

				//	yyy	change	end!
			}
			break;
			case 5: {
				/*	yyy	2008-3-27 change	begin!
				if(!(pCha->m_retry5 % 15))
				{
					pCha->BickerNotice("????????????????????????????????????????????????????????????????????????????????????????????????????? 5 ????????????????");
					pCha->SetExpScale(0);

				}

				pCha->m_retry5++;
				*/
				if (pCha->m_retry3 == 0)
					//pCha->PopupNotice("????????????????60??????????...");
					pCha->PopupNotice(RES_STRING(GM_GAMEAPPNET_CPP_00008));
				if (pCha->m_retry3 == 1) {
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
					//pCha->BickerNotice("????????????????????????????????????????????????????????????????????????????????????????????????????? 5 ????????????????");
					//pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00008));
					//pCha->SetExpScale(0);
				}
				pCha->m_retry3++;
				// pCha->m_retry5++;
			}
			break;
			case 6: {
				/*	yyy	2008-3-27 change	begin!
				if(!(pCha->m_retry6 % 15))
				{
					pCha->BickerNotice("????????????????????????????????????????????????????????????????????????????????????????????????????? 5 ????????????????");
					pCha->SetExpScale(0);
				}

				pCha->m_retry6++;
				*/
				if (pCha->m_retry3 == 0)
					//pCha->PopupNotice("????????????????60??????????...");
					pCha->PopupNotice(RES_STRING(GM_GAMEAPPNET_CPP_00008));
				if (pCha->m_retry3 == 1) {
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
				}

				pCha->m_retry3++;
			}
			break;
			}
		}
	}
	break;
	default:
		if (cmd / 500 == CMD_MM_BASE / 500) {
			ProcessInterGameMsg(cmd, pGate, pkt);
		}
		else {
			l_player = ToPointer<CPlayer>(pkt.ReverseReadInt64());
			if (cmd / 500 == CMD_PM_BASE / 500 && !l_player) {
				ProcessGroupBroadcast(cmd, pGate, pkt);
			}
			else {
				if (!l_player)
					break;
				try {
					DWORD l_gateaddr = pkt.ReverseReadInt64();
					if (l_player->GetGateAddr() != l_gateaddr) {
						/*ToLogService("errors", LogLevel::Error, "?????ID:{}, ????????????:{}, gate:{},cmd={}, ?????({})", l_player->GetDBChaId(), l_player->GetGateAddr(),
							l_gateaddr,cmd, l_player->IsValidFlag() );*/
						ToLogService("errors", LogLevel::Error,
									 "DB ID:{}, address not matching??local :{}, gate:{},cmd={}, validity ({})",
									 l_player->GetDBChaId(), l_player->GetGateAddr(),
									 l_gateaddr, cmd, l_player->IsValidFlag());
						break;
					}
				}
				catch (...) {
					//ToLogService("errors", LogLevel::Error, "===========================??Gate?????????????{},cmd ={}", l_player, cmd);
					ToLogService("errors", LogLevel::Error,
								 "===========================Player address error that come from Gate {},cmd ={}",
								 static_cast<void*>(l_player), static_cast<int>(cmd));
					break;
				}
				if (!l_player->IsValid())
					break;
				if (l_player->GetMainCha()->GetPlayer() != l_player) {
					//ToLogService("errors", LogLevel::Error, "????player????????????{}??Gate???[????{}, ????{}]????cmd={}", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
					ToLogService("errors", LogLevel::Error,
								 "two player not matching, character name: {}, Gate address [local {}, guest {}], cmd={}",
								 l_player->GetMainCha()->GetLogName(),
								 static_cast<void*>(l_player->GetMainCha()->GetPlayer()), static_cast<void*>(l_player),
								 static_cast<int>(cmd));
				}

				CCharacter* pCCha = l_player->GetCtrlCha();
				if (!pCCha)
					break;
				if (g_pGameApp->IsValidEntity(pCCha->GetID(), pCCha->GetHandle())) {
					g_pNoticeChar = pCCha;

					g_ulCurID = pCCha->GetID();
					g_lCurHandle = pCCha->GetHandle();

					pCCha->ProcessPacket(cmd, pkt);

					g_ulCurID = defINVALID_CHA_ID;
					g_lCurHandle = defINVALID_CHA_HANDLE;

					g_pNoticeChar = NULL;
				}
				else {
					//ToLogService("errors", LogLevel::Error, "???CMD_CM_BASE?????[{}]?, ??????pCCha???", cmd);
					ToLogService("errors", LogLevel::Error,
								 "when receive CMD_CM_BASE message[{}], find character pCCha is null", cmd);
				}
				break;
			}
		}
	}
}

// ??????????????
void CGameApp::ProcessGuildChallMoney(GateServer* pGate, const net::msg::PmGuildChallMoneyMessage& msg) {
	DWORD dwChaDBID = static_cast<DWORD>(msg.leaderId);
	DWORD dwMoney = static_cast<DWORD>(msg.money);

	//	2007-8-4	yangyinyu	change	begin!	//	?????????????????????????????????
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		//pCha->AddMoney( "??", dwMoney );
		pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
		/*pCha->SystemNotice( "??????????%s?????????%s????????????????(%u)????????????\n", pszGuild1, pszGuild2, dwMoney );
		ToLogService("common", "??{}??????????{}?????????{}????????????????({})????????????", pCha->GetGuildName(), pszGuild1, pszGuild2, dwMoney);*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00009), msg.guildName1.c_str(), msg.guildName2.c_str(),
						   dwMoney);
		ToLogService(
			"common",
			"bidder and consortia [{}] battle was consortia [{}] replace, your consortia gold ({}) had back to you",
			pCha->GetGuildName(), msg.guildName1.c_str(), msg.guildName2.c_str(), dwMoney);
	}
	else {
		//LG( "?????????", "?????????????????????DBID[%u],???[%u].\n", dwChaDBID, dwMoney );
		ToLogService("common", "not find deacon information finger??cannot back gold DBID[{}],how much money[{}].",
					 dwChaDBID, dwMoney);
	}
}

void CGameApp::ProcessGuildChallPrizeMoney(GateServer* pGate, const net::msg::PmGuildChallPrizeMoneyMessage& msg) {
	DWORD dwChaDBID = static_cast<DWORD>(msg.leaderId);
	DWORD dwMoney = static_cast<DWORD>(msg.money);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->AddMoney("??", dwMoney);
		/*pCha->SystemNotice( "????????????%s?????????????????????????%u????", pCha->GetGuildName(), dwMoney );
		ToLogService("common", "????????????{}?????????????????????????{}????", pCha->GetGuildName(), dwMoney);*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
		ToLogService(
			"common",
			"congratulate you have leading the consortia??{}??get win in consortia battle??gain bounty??{}????",
			pCha->GetGuildName(), dwMoney);
	}
	else {
		//LG( "?????????", "??????????????????????DBID[%u],???[%u]", dwChaDBID, dwMoney );
		ToLogService("common", "cannot find deacon information finger??cannot hortation DBID[{}],how much money[{}]",
					 dwChaDBID, dwMoney);
	}
}

// ???????????
void CGameApp::ProcessGuildMsg(GateServer* pGate, const net::msg::PmGuildInfoMessage& msg) {
	DWORD dwChaDBID = static_cast<DWORD>(msg.chaId);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetCtrlCha();
		pCha->SetGuildName(msg.guildName.c_str());
		pCha->SetGuildMotto(msg.guildMotto.c_str());
		pCha->SyncGuildInfo();
	}
}

// ?????????????
void CGameApp::ProcessTeamMsg(GateServer* pGate, const net::msg::PmTeamMessage& teamMsg) {
	char cTeamMsgType = static_cast<char>(teamMsg.msg);

	switch (cTeamMsgType) {
	case TEAM_MSG_ADD: break;
	case TEAM_MSG_LEAVE: break;
	case TEAM_MSG_UPDATE: break;
	default:
		return;
	}

	char cMemberCnt = static_cast<char>(teamMsg.count);

	uplayer Team[MAX_TEAM_MEMBER];
	CPlayer* PlayerList[MAX_TEAM_MEMBER];
	bool CanSeenO[MAX_TEAM_MEMBER][2];
	bool CanSeenN[MAX_TEAM_MEMBER][2];

	for (char i = 0; i < cMemberCnt; i++) {
		const auto& member = teamMsg.members[i];
		Team[i].Init(member.gateName.c_str(), static_cast<DWORD>(member.gtAddr), static_cast<DWORD>(member.chaId));
		if (!Team[i].pGate) {
			ToLogService("common", "GameServer can't find matched Gate: {}, addr = 0x{:X}, chaid = {}.",
						 member.gateName.c_str(), member.gtAddr, member.chaId);
			ToLogService("common", "\tGameServer all Gate:");
			BEGINGETGATE();
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE()) {
				ToLogService("common", "\t{}", pGateServer->GetName().c_str());
			}
		}

		PlayerList[i] = GetPlayerByDBID(static_cast<DWORD>(member.chaId));
	}

	//RefreshTeamEyeshot(PlayerList, cMemberCnt, cTeamMsgType);
	CheckSeeWithTeamChange(CanSeenO, PlayerList, cMemberCnt);
	//if(PlayerList[0]==NULL)
	//{
	//	ToLogService("common", "????????game server????");
	//}

	int nLeftMember = cMemberCnt;
	if (cTeamMsgType == TEAM_MSG_LEAVE) // ???????????
	{
		nLeftMember -= 1;
		CPlayer* pLeave = PlayerList[cMemberCnt - 1];
		if (pLeave) {
			// Remove party fruit when leaving team
			if (pLeave->IsTeamLeader()) {
				for (auto i = 0; i < cMemberCnt - 1; ++i) {
					if (PlayerList[i] && PlayerList[i]->GetMainCha()) {
						PlayerList[i]->GetMainCha()->DelSkillState(217, true);
						PlayerList[i]->GetMainCha()->DelSkillState(218, true);
					}
				}
			}
			pLeave->GetMainCha()->DelSkillState(217, true);
			pLeave->GetMainCha()->DelSkillState(218, true);

			pLeave->LeaveTeam();
		}
	}
	else if (cTeamMsgType == TEAM_MSG_ADD) {
		try {
			[&]() {
				CPlayer* newPly = PlayerList[cMemberCnt - 1];
				if (!newPly) {
					return;
				}

				CCharacter* newCha = newPly->GetMainCha();
				if (!newCha) {
					return;
				}

				CPlayer* leaderPly = GetPlayerByDBID(Team[0].m_dwDBChaId);
				if (!leaderPly) {
					return;
				}

				CCharacter* leaderCha = leaderPly->GetMainCha();
				if (!leaderCha) {
					return;
				}

				CSkillState& leader_states = leaderCha->m_CSkillState;

				//if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // 
				if (leader_states.HasState(217)) {
					const auto& state = leader_states.GetSStateByID(217);
					const auto use_duration = state->lOnTick * 1000;
					const auto remaining = (use_duration - (GetTickCount() - state->ulStartTick)) / 1000;
					newCha->AddSkillState(g_uchFightID, newCha->GetID(), newCha->GetHandle(), enumSKILL_TYPE_SELF,
										  enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, 217, state->GetStateLv(),
										  remaining, enumSSTATE_ADD, true);
					return;
				}

				if (leader_states.HasState(218)) {
					const auto& state = leader_states.GetSStateByID(218);
					const auto use_duration = state->lOnTick * 1000;
					const auto remaining = (use_duration - (GetTickCount() - state->ulStartTick)) / 1000;
					newCha->AddSkillState(g_uchFightID, newCha->GetID(), newCha->GetHandle(), enumSKILL_TYPE_SELF,
										  enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, 218, state->GetStateLv(),
										  remaining, enumSSTATE_ADD, true);
					return;
				}
			}();
		}
		catch (...) {
			ToLogService("errors", LogLevel::Error, "Exception handling: newPly invalid, cMemberCnt={}", cMemberCnt);
		}
	}

	// ?????????????, ???cMember????1, ?????????????
	for (int i = 0; i < nLeftMember; i++) {
		if (PlayerList[i] == NULL) continue;

		PlayerList[i]->ClearTeamMember();
		for (int j = 0; j < nLeftMember; j++) {
			if (i == j) continue;
			PlayerList[i]->AddTeamMember(&Team[j]);
		}
		if (nLeftMember != 1) {
			PlayerList[i]->setTeamLeaderID(Team[0].m_dwDBChaId);
			PlayerList[i]->NoticeTeamLeaderID();
		}
		else {
			// Remove party fruit to last person in party
			PlayerList[0]->GetMainCha()->DelSkillState(217, true);
			PlayerList[0]->GetMainCha()->DelSkillState(218, true);
		}
	}

	CheckSeeWithTeamChange(CanSeenN, PlayerList, cMemberCnt);
	RefreshTeamEyeshot(CanSeenO, CanSeenN, PlayerList, cMemberCnt, cTeamMsgType);

	//add by jilinlee 2007/07/11

	for (char i = 0; i < cMemberCnt; i++) {
		if (i < 5) {
			if (PlayerList[i]) {
				CCharacter* pCtrlCha = PlayerList[i]->GetCtrlCha();
				if (pCtrlCha) {
					SubMap* pSubMap = pCtrlCha->GetSubMap();
					if (pSubMap && pSubMap->GetMapRes()) {
						if (!(pSubMap->GetMapRes()->CanTeam())) {
							// pCtrlCha ->SystemNotice("?????????????????????????");
							pCtrlCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00011));
							pCtrlCha->MoveCity("garner");
						}
					}
				}
			}
		}
	}

	//if(nLeftMember==1) ToLogService("common", "nLeftMember==1, ??????!");

	//ToLogService("common", "??????????????\n");
}

// ????????????
void CGameApp::CheckSeeWithTeamChange(bool CanSeen[][2], CPlayer** pCPlayerList, char chMemberCnt) {
	if (chMemberCnt <= 1)
		return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter *pCProcCha = pCProcPly->GetCtrlCha(), *pCCurCha;
	for (char i = 0; i < chMemberCnt - 1; i++) {
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		if (pCProcCha->IsInEyeshot(pCCurCha)) {
			pCProcCha->CanSeen(pCCurCha) ? CanSeen[i][0] = true : CanSeen[i][0] = false;
			pCCurCha->CanSeen(pCProcCha) ? CanSeen[i][1] = true : CanSeen[i][1] = false;
		}
	}
}

// ??????????????????????????????
void CGameApp::RefreshTeamEyeshot(bool CanSeenOld[][2], bool CanSeenNew[][2], CPlayer** pCPlayerList, char chMemberCnt,
								  char chRefType) {
	if (chMemberCnt <= 1)
		return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter *pCProcCha = pCProcPly->GetCtrlCha(), *pCCurCha;
	for (char i = 0; i < chMemberCnt - 1; i++) {
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		if (pCProcCha->IsInEyeshot(pCCurCha)) {
			if (chRefType == TEAM_MSG_ADD) {
				if (!CanSeenOld[i][0] && CanSeenNew[i][0])
					pCCurCha->BeginSee(pCProcCha);
				if (!CanSeenOld[i][1] && CanSeenNew[i][1])
					pCProcCha->BeginSee(pCCurCha);
			}
			else if (chRefType == TEAM_MSG_LEAVE) {
				if (CanSeenOld[i][0] && !CanSeenNew[i][0])
					pCCurCha->EndSee(pCProcCha);
				if (CanSeenOld[i][1] && !CanSeenNew[i][1])
					pCProcCha->EndSee(pCCurCha);
			}
		}
	}
}

// ???????????????????
void CGameApp::RefreshTeamEyeshot(CPlayer** pCPlayerList, char chMemberCnt, char chRefType) {
	if (chMemberCnt <= 1)
		return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter *pCProcCha = pCProcPly->GetCtrlCha(), *pCCurCha;
	bool bCurChaHide;
	bool bProcChaHide = pCProcCha->IsHide();
	for (char i = 0; i < chMemberCnt - 1; i++) {
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		bCurChaHide = pCCurCha->IsHide();
		if (bProcChaHide || bCurChaHide) // ????????
		{
			if (pCProcCha->IsInEyeshot(pCCurCha)) {
				if (chRefType == TEAM_MSG_ADD) {
					if (bProcChaHide)
						pCCurCha->BeginSee(pCProcCha);
					if (bCurChaHide)
						pCProcCha->BeginSee(pCCurCha);
				}
				else if (chRefType == TEAM_MSG_LEAVE) {
					if (bProcChaHide)
						pCCurCha->EndSee(pCProcCha);
					if (bCurChaHide)
						pCProcCha->EndSee(pCCurCha);
				}
			}
		}
	}
}

BOOL CGameApp::AddVolunteer(CCharacter* pCha) {
	if (pCha->IsVolunteer())
		return false;
	pCha->SetVolunteer(true);

	SVolunteer volNode;
	volNode.lJob = (long)pCha->getAttr(ATTR_JOB);
	volNode.lLevel = pCha->GetLevel();
	volNode.ulID = pCha->GetID();
	strncpy_s(volNode.szMapName, sizeof(volNode.szMapName), pCha->GetPlyCtrlCha()->GetSubMap()->GetName(), _TRUNCATE);
	strcpy(volNode.szName, pCha->GetName());

	m_vecVolunteerList.push_back(volNode);

	return true;
}

BOOL CGameApp::DelVolunteer(CCharacter* pCha) {
	if (!pCha->IsVolunteer())
		return false;
	pCha->SetVolunteer(false);

	vector<SVolunteer>::iterator it;
	for (it = m_vecVolunteerList.begin(); it != m_vecVolunteerList.end(); it++) {
		if (!strcmp((*it).szName, pCha->GetName())) {
			m_vecVolunteerList.erase(it);
			return true;
		}
	}

	return false;
}

int CGameApp::GetVolNum() {
	return (int)m_vecVolunteerList.size();
}

SVolunteer* CGameApp::GetVolInfo(int nIndex) {
	if (nIndex < 0 || nIndex >= (int)m_vecVolunteerList.size())
		return NULL;

	return &m_vecVolunteerList[nIndex];
}

SVolunteer* CGameApp::FindVolunteer(const char* szName) {
	vector<SVolunteer>::iterator it;
	for (it = m_vecVolunteerList.begin(); it != m_vecVolunteerList.end(); it++) {
		if (!strcmp((*it).szName, szName)) {
			return (SVolunteer*)&(*it);
		}
	}
	return NULL;
}

void CGameApp::ProcessInterGameMsg(unsigned short usCmd, GateServer* pGate, net::RPacket& pkt) {
	long lSrcID = pkt.ReadInt64();
	short sNum = pkt.ReverseReadInt64();
	long lGatePlayerAddr = pkt.ReverseReadInt64();
	long lGatePlayerID = pkt.ReverseReadInt64();

	switch (usCmd) {
	case CMD_MM_UPDATEGUILDBANK: {
		net::msg::MmUpdateGuildBankMessage m;
		net::msg::deserialize(pkt, m);
		int guildID = static_cast<int>(m.guildId);
		BEGINGETGATE();
		CPlayer* pCPlayer;
		CCharacter* pCha = 0;
		GateServer* pGateServer;

		CKitbag bag;
		if (!game_db.GetGuildBank(guildID, &bag)) {
			return;
		}

		while (pGateServer = GETNEXTGATE()) {
			if (!BEGINGETPLAYER(pGateServer))
				continue;
			int nCount = 0;
			while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer)) {
				pCha = pCPlayer->GetMainCha();
				if (!pCha)
					continue;
				if (pCha->GetGuildID() == guildID) {
					//&& pCPlayer->GetBankType() == 2){
					pCPlayer->SynGuildBank(&bag, 0);
				}
			}
		}
		break;
	}
	case CMD_MM_UPDATEGUILDBANKGOLD: {
		net::msg::MmUpdateGuildBankGoldMessage m;
		net::msg::deserialize(pkt, m);
		int guildID = static_cast<int>(m.guildId);
		BEGINGETGATE();
		CPlayer* pCPlayer;
		CCharacter* pCha = 0;
		GateServer* pGateServer;

		unsigned long long gold = game_db.GetGuildBankGold(guildID);

		//  :    
		auto WtPk = net::msg::serialize(net::msg::McUpdateGuildGoldMessage{to_string(gold).c_str()});

		while (pGateServer = GETNEXTGATE()) {
			if (!BEGINGETPLAYER(pGateServer))
				continue;
			int nCount = 0;
			while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer)) {
				pCha = pCPlayer->GetMainCha();
				if (!pCha)
					continue;
				pCPlayer->GetGuildGold();
				//int canSeeBank = (pCha->guildPermission & emGldPermViewBank);
				//if (pCha->GetGuildID() == guildID && canSeeBank == emGldPermViewBank){
				//	pCha->ReflectINFof(pCha, WtPk);
				//}
			}
		}
		break;
	}
	case CMD_MM_GUILD_MOTTO: {
		net::msg::MmGuildMottoMessage mottoMsg;
		mottoMsg.guildId = lSrcID;
		mottoMsg.motto = pkt.ReadString();
		uLong l_gldid = static_cast<uLong>(mottoMsg.guildId);
		const std::string& pszMotto = mottoMsg.motto;
		{
			//??????FindPlayerChaByID
			BEGINGETGATE();
			CPlayer* pCPlayer;
			CCharacter* pCha = 0;
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE()) {
				if (!BEGINGETPLAYER(pGateServer))
					continue;
				int nCount = 0;
				while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer)) {
					if (++nCount > GETPLAYERCOUNT(pGateServer)) {
						//LG("???????????", "??????:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						ToLogService("errors", LogLevel::Error, "player number:{}, {}", GETPLAYERCOUNT(pGateServer),
									 "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						break;
					}
					pCha = pCPlayer->GetMainCha();
					if (!pCha)
						continue;
					if (pCha->GetGuildID() == l_gldid) // ??????
					{
						pCha->SetGuildMotto(pszMotto.c_str());
						pCha->SyncGuildInfo();
						//pCha->SystemNotice("????????????????");
						//pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00012));
					}
				}
			}
		}
	}
	break;
	case CMD_MM_GUILD_DISBAND: {
		uLong l_gldid = lSrcID;
		{
			//??????FindPlayerChaByID
			BEGINGETGATE();
			CPlayer* pCPlayer;
			CCharacter* pCha = 0;
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE()) {
				if (!BEGINGETPLAYER(pGateServer))
					continue;
				int nCount = 0;
				while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer)) {
					if (++nCount > GETPLAYERCOUNT(pGateServer)) {
						//LG("???????????", "??????:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						ToLogService("errors", LogLevel::Error, "player number:{}, {}", GETPLAYERCOUNT(pGateServer),
									 "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						break;
					}
					pCha = pCPlayer->GetMainCha();
					if (!pCha)
						continue;
					if (pCha->GetGuildID() == l_gldid) // ??????
					{
						pCha->m_CChaAttr.ResetChangeFlag();
						pCha->guildPermission = 0;
						pCha->SetGuildID(0);
						pCha->SetGuildState(0);
						pCha->SynAttr(enumATTRSYN_TRADE);

						pCha->SetGuildName("");
						pCha->SetGuildMotto("");
						pCha->SyncGuildInfo();
						//pCha->SystemNotice("??????????????");
						pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00013));
					}
				}
			}
		}
	}
	break;
	case CMD_MM_GUILD_KICK: {
		net::msg::MmGuildKickMessage kickMsg;
		kickMsg.srcId = lSrcID;
		kickMsg.guildName = pkt.ReadString();
		uLong l_chaid = static_cast<uLong>(kickMsg.srcId);
		CCharacter* pCha = FindMainPlayerChaByID(l_chaid);
		if (pCha) {
			pCha->SetGuildName("");
			const std::string& l_gldname = kickMsg.guildName;
			pCha->guildPermission = 0;
			pCha->SetGuildID(0); //??????ID
			pCha->SetGuildState(0);
			pCha->SetGuildName("");
			pCha->SetGuildMotto("");
			// pCha->SystemNotice("?????????????????[%s].",l_gldname);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00014), l_gldname.c_str());
			pCha->SyncGuildInfo();
		}
	}
	break;
	case CMD_MM_GUILD_APPROVE: {
		net::msg::MmGuildApproveMessage msg;
		msg.srcId = lSrcID;
		net::msg::deserializeBody(pkt, msg);
		Handle_GuildApprove(msg);
		break;
	}
	case CMD_MM_GUILD_REJECT: {
		net::msg::MmGuildRejectMessage rejectMsg;
		rejectMsg.srcId = lSrcID;
		rejectMsg.guildName = pkt.ReadString();
		uLong l_chaid = static_cast<uLong>(rejectMsg.srcId);
		CCharacter* pCha = FindMainPlayerChaByID(l_chaid);
		if (pCha) {
			pCha->SetGuildID(0);
			pCha->SetGuildState(0);
			pCha->SetGuildName("");

			//pCha->SystemNotice("??????[%s]????????????.",pkt.ReadString());
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00016), rejectMsg.guildName.c_str());
		}
	}
	break;
	case CMD_MM_QUERY_CHAPING: {
		net::msg::MmQueryChaPingMessage pingMsg;
		pingMsg.srcId = lSrcID;
		pingMsg.chaName = pkt.ReadString();
		const std::string& cszChaName = pingMsg.chaName;
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha)
			break;

		//  :  +  trailer
		auto WtPk = net::msg::serialize(net::msg::McPingMessage{
			(int64_t)GetTickCount(), ToAddress(pGate), lSrcID, lGatePlayerID, lGatePlayerAddr
		});
		WtPk.WriteInt64(1);
		pCCha->ReflectINFof(pCCha, WtPk);

		break;
	}
	case CMD_MM_QUERY_CHA: {
		net::msg::MmQueryChaMessage queryMsg;
		queryMsg.srcId = lSrcID;
		queryMsg.chaName = pkt.ReadString();
		const std::string& cszChaName = queryMsg.chaName;
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha || !pCCha->GetSubMap())
			break;

		//  :      +  trailer
		auto WtPk = net::msg::serialize(net::msg::McQueryChaMessage{
			lSrcID, pCCha->GetName(), pCCha->GetSubMap()->GetName(), (int64_t)pCCha->GetPos().x,
			(int64_t)pCCha->GetPos().y, pCCha->GetID()
		});
		WtPk.WriteInt64(lGatePlayerID);
		WtPk.WriteInt64(lGatePlayerAddr);
		WtPk.WriteInt64(1);
		pGate->SendData(WtPk);

		break;
	}
	case CMD_MM_QUERY_CHAITEM: {
		net::msg::MmQueryChaItemMessage queryItemMsg;
		queryItemMsg.srcId = lSrcID;
		queryItemMsg.chaName = pkt.ReadString();
		const std::string& cszChaName = queryItemMsg.chaName;
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha)
			break;
		pCCha->m_CKitbag.SetChangeFlag();

		//  :      (-)
		{
			auto kitbag = pCCha->BuildKitbagInfo(pCCha->m_CKitbag, enumSYN_KITBAG_INIT);
			auto WtPk = net::msg::serialize(net::msg::McQueryChaKitbagMessage{
				lSrcID, kitbag, lGatePlayerID, lGatePlayerAddr, 1
			});
			pGate->SendData(WtPk);
		}

		break;
	}
	case CMD_MM_CALL_CHA: {
		net::msg::MmCallChaMessage msg;
		msg.srcId = lSrcID;
		net::msg::deserializeBody(pkt, msg);
		Handle_CallCha(msg);
		break;
	}
	case CMD_MM_GOTO_CHA: {
		net::msg::MmGotoChaMessage msg;
		msg.srcId = lSrcID;
		net::msg::deserializeBody(pkt, msg);
		Handle_GotoCha(pGate, lGatePlayerID, lGatePlayerAddr, msg);
		break;
	}
	case CMD_MM_KICK_CHA: {
		net::msg::MmKickChaMessage msg;
		msg.srcId = lSrcID;
		net::msg::deserializeBody(pkt, msg);
		Handle_KickCha(msg);
		break;
	}
	case CMD_MM_NOTICE: {
		net::msg::MmNoticeMessage noticeMsg;
		noticeMsg.content = pkt.ReadString();
		LocalNotice(noticeMsg.content.c_str());

		break;
	}
	case CMD_MM_CHA_NOTICE: {
		net::msg::MmChaNoticeMessage chaNoticeMsg;
		chaNoticeMsg.content = pkt.ReadString();
		chaNoticeMsg.chaName = pkt.ReadString();
		const std::string& cszNotiCont = chaNoticeMsg.content;
		const std::string& cszChaName = chaNoticeMsg.chaName;

		if (cszChaName.empty())
			LocalNotice(cszNotiCont.c_str());
		else {
			CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
			if (!pCCha)
				break;

			//  :  
			auto wpk = net::msg::serialize(net::msg::McSysInfoMessage{cszNotiCont});
			pCCha->ReflectINFof(pCCha, wpk);
		}

		break;
	}
	case CMD_MM_DO_STRING: {
		net::msg::MmDoStringMessage doStrMsg;
		doStrMsg.luaCode = pkt.ReadString();
		luaL_dostring(g_pLuaState, doStrMsg.luaCode.c_str());
		break;
	}
	case CMD_MM_LOGIN: {
		net::msg::MmLoginMessage loginMsg;
		loginMsg.chaName = pkt.ReadString();
		g_pGameApp->AfterPlayerLogin(loginMsg.chaName.c_str());

		break;
	}
	case CMD_MM_GUILD_CHALL_PRIZEMONEY: {
		net::msg::MmGuildChallPrizeMoneyMessage msg;
		net::msg::deserialize(pkt, msg);
		Handle_GuildChallPrizeMoney(msg);
		break;
	}
	case CMD_MM_ADDCREDIT: {
		net::msg::MmAddCreditMessage msg;
		net::msg::deserialize(pkt, msg);
		Handle_AddCredit(msg);
		break;
	}
	case CMD_MM_STORE_BUY: {
		net::msg::MmStoreBuyMessage msg;
		net::msg::deserialize(pkt, msg);
		Handle_StoreBuy(msg);
		break;
	}
	case CMD_MM_ADDMONEY: {
		net::msg::MmAddMoneyMessage msg;
		net::msg::deserialize(pkt, msg);
		Handle_AddMoney(msg);
		break;
	}
	case CMD_MM_AUCTION: {
		net::msg::MmAuctionMessage msg;
		net::msg::deserialize(pkt, msg);

		CCharacter* pCha = NULL;
		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(msg.charDbId);
		if (pPlayer) {
			pCha = pPlayer->GetMainCha();
		}
		if (pCha) {
			g_CParser.DoString("AuctionEnd", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha,
							   DOSTRING_PARAM_END);
		}

		break;
	}
	}
}

void CGameApp::ProcessGroupBroadcast(unsigned short usCmd, GateServer* pGate, net::RPacket& pkt) {
}

void CGameApp::Handle_StoreBuy(const net::msg::MmStoreBuyMessage& msg) {
	CPlayer* pPlayer = GetPlayerByDBID(static_cast<DWORD>(msg.charDbId));
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		g_StoreSystem.Accept(pCha, static_cast<long>(msg.commodityId));
		pCha->GetPlayer()->SetRplMoney(static_cast<long>(msg.money));
	}
}

void CGameApp::Handle_AddMoney(const net::msg::MmAddMoneyMessage& msg) {
	CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(static_cast<DWORD>(msg.charDbId));
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->AddMoney("??", static_cast<DWORD>(msg.money));
	}
}

void CGameApp::Handle_AddCredit(const net::msg::MmAddCreditMessage& msg) {
	CPlayer* pPlayer = GetPlayerByDBID(static_cast<DWORD>(msg.charDbId));
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->SetCredit((long)pCha->GetCredit() + static_cast<long>(msg.amount));
		pCha->SynAttr(enumATTRSYN_TASK);
	}
}

void CGameApp::Handle_GuildChallPrizeMoney(const net::msg::MmGuildChallPrizeMoneyMessage& msg) {
	DWORD dwChaDBID = static_cast<DWORD>(msg.charDbId);
	DWORD dwMoney = static_cast<DWORD>(msg.money);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer) {
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
		ToLogService(
			"common",
			"congratulate you leading consortia [{}] ID [{}] get win in consortia battle, gain bounty [{}]",
			pCha->GetGuildName(),
			pCha->GetGuildID(), dwMoney);
	}
}

void CGameApp::Handle_KickCha(const net::msg::MmKickChaMessage& msg) {
	CCharacter* pCCha = FindPlayerChaByName(msg.targetName.c_str());
	if (!pCCha || !pCCha->GetSubMap())
		return;
	KICKPLAYER(pCCha->GetPlayer(), static_cast<long>(msg.kickDuration));
	g_pGameApp->GoOutGame(pCCha->GetPlayer(), true);
}

void CGameApp::Handle_GotoCha(GateServer* pGate, long lGatePlayerID, long lGatePlayerAddr, const net::msg::MmGotoChaMessage& msg) {
	CCharacter* pCCha = FindPlayerChaByName(msg.targetName.c_str());
	if (!pCCha || !pCCha->GetSubMap())
		return;
	switch (msg.mode) {
	case 1: {
		auto WtPk = net::msg::serialize(net::msg::MmGotoChaMessage{
			msg.srcId, msg.srcName, 2, {}, pCCha->IsBoat() ? 1LL : 0LL, pCCha->GetSubMap()->GetName(),
			(int64_t)pCCha->GetPos().x, (int64_t)pCCha->GetPos().y, (int64_t)pCCha->GetSubMap()->GetCopyNO()
		});
		WtPk.WriteInt64(lGatePlayerID);
		WtPk.WriteInt64(lGatePlayerAddr);
		WtPk.WriteInt64(1);
		pGate->SendData(WtPk);
		break;
	}
	case 2: {
		if ((msg.isBoat != 0) != pCCha->IsBoat())
			break;
		pCCha->SwitchMap(pCCha->GetSubMap(), msg.mapName.c_str(),
			static_cast<Long>(msg.posX), static_cast<Long>(msg.posY), true, enumSWITCHMAP_CARRY, static_cast<Long>(msg.copyNo));
		break;
	}
	}
}

void CGameApp::Handle_CallCha(const net::msg::MmCallChaMessage& msg) {
	CCharacter* pCCha = FindPlayerChaByName(msg.targetName.c_str());
	if (!pCCha || !pCCha->GetSubMap())
		return;
	if ((msg.isBoat != 0) != pCCha->IsBoat())
		return;
	pCCha->SwitchMap(pCCha->GetSubMap(), msg.mapName.c_str(),
		static_cast<Long>(msg.posX), static_cast<Long>(msg.posY), true, enumSWITCHMAP_CARRY, static_cast<Long>(msg.copyNo));
}

void CGameApp::Handle_GuildApprove(const net::msg::MmGuildApproveMessage& msg) {
	CCharacter* pCha = FindMainPlayerChaByID(static_cast<uLong>(msg.srcId));
	if (pCha) {
		pCha->SetGuildID(static_cast<int>(msg.guildId));
		pCha->SetGuildState(0);
		pCha->SetGuildName(msg.guildName.c_str());
		pCha->SetGuildMotto(msg.guildMotto.c_str());
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00015), msg.guildName.c_str());
		pCha->SyncGuildInfo();
	}
}

void CGameApp::ProcessGarner2Update(const net::msg::PmGarner2UpdateLegacyMessage& msg)
{
	long chaid[6];
	CPlayer* pplay;
	for (int i = 0; i < 6; ++i) chaid[i] = static_cast<long>(msg.chaIds[i]);
	if (0 != chaid[0]) {
		pplay = FindPlayerByDBChaID(chaid[0]);
		if (pplay) {
			pplay->SetGarnerWiner(0);
		}
	}

	for (int i = 1; i < 6 && chaid[i]; i++) {
		pplay = FindPlayerByDBChaID(chaid[0]);
		if (pplay) {
			pplay->SetGarnerWiner(i);
		}
	}
}

void CGameApp::ProcessDynMapEntry(GateServer* pGate, const net::msg::MapEntryMessage& mapMsg) {
	//  : 1-    = srcMapName (    szTarMapN),
	// 2-  = targetMapName (    szSrcMapN).    .
	const auto& szTarMapN = mapMsg.srcMapName;
	const auto& szSrcMapN = mapMsg.targetMapName;

	switch (mapMsg.subType) {
	case enumMAPENTRY_CREATE: {
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes) {
			break;
		}
		pCMap = pCMapRes->GetCopy();
		Long lPosX = static_cast<Long>(mapMsg.posX);
		Long lPosY = static_cast<Long>(mapMsg.posY);
		Short sMapCopyNum = static_cast<Short>(mapMsg.mapCopyNum);
		Short sCopyPlyNum = static_cast<Short>(mapMsg.copyPlayerNum);
		CDynMapEntryCell CEntryCell;
		CEntryCell.SetMapName(szTarMapN.c_str());
		CEntryCell.SetTMapName(szSrcMapN.c_str());
		CEntryCell.SetEntiPos(lPosX, lPosY);
		CDynMapEntryCell* pCEntry = g_CDMapEntry.Add(&CEntryCell);
		if (pCEntry) {
			pCEntry->SetCopyNum(sMapCopyNum);
			pCEntry->SetCopyPlyNum(sCopyPlyNum);
			string strScript;
			Short sLineNum = static_cast<Short>(mapMsg.luaScriptLines.size());
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s[%u, %u]????????? %d\n", szSrcMapN, szTarMapN, lPosX, lPosY, sLineNum);
				ToLogService("common", "receive request to create entry??position {} --> {}[{}, {}]??script line {}",
							 szSrcMapN.c_str(), szTarMapN.c_str(), lPosX, lPosY, sLineNum);
			for (const auto& line : mapMsg.luaScriptLines) {
				strScript += line;
				strScript += " ";
			}
			luaL_dostring(g_pLuaState, strScript.c_str());
			g_CParser.DoString("config_entry", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry,
							   DOSTRING_PARAM_END);

			if (pCEntry->GetEntiID() > 0) {
				SItemGrid SItemCont;
				SItemCont.sID = (Short)pCEntry->GetEntiID();
				SItemCont.sNum = 1;
				SItemCont.SetDBParam(-1, 0);
				SItemCont.chForgeLv = 0;
				SItemCont.SetInstAttrInvalid();
				CItem* pCItem = pCMap->ItemSpawn(&SItemCont, lPosX, lPosY, enumITEM_APPE_NATURAL, 0,
												 g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(), -1, -1,
												 pCEntry->GetEvent());
				if (pCItem) {
					pCItem->SetOnTick(0);
					pCEntry->SetEnti(pCItem);
				}
				else {
					if (g_cchLogMapEntry)
						//LG("??????????", "?????????????? %s --> %s[%u, %u]?????? %u ???????\n", szSrcMapN, szTarMapN, lPosX, lPosY, SItemCont.sID);
						ToLogService("common", "create entry failed??position {} --> {}[{}, {}]??item {} create failed",
									 szSrcMapN.c_str(), szTarMapN.c_str(), lPosX, lPosY, SItemCont.sID);
					g_CDMapEntry.Del(pCEntry);
					break;
				}
			}
			//  :     
			auto wpk = net::msg::serialize(net::msg::GmMapEntryResultMessage{
				szSrcMapN, szTarMapN, (int64_t)enumMAPENTRY_RETURN, (int64_t)enumMAPENTRYO_CREATE_SUC
			});

			BEGINGETGATE();
			GateServer* pGateServer = NULL;
			while (pGateServer = GETNEXTGATE()) {
				pGateServer->SendData(wpk);
				break;
			}
			if (g_cchLogMapEntry)
				//LG("??????????", "?????????????? %s --> %s[%u, %u] \n", szSrcMapN, szTarMapN, lPosX, lPosY);
				ToLogService("common", "create entry success??position {} --> {}[{}, {}] ", szSrcMapN.c_str(),
							 szTarMapN.c_str(), lPosX, lPosY);

			g_CParser.DoString("after_create_entry", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1,
							   pCEntry, DOSTRING_PARAM_END);
		}
	}
	break;
	case enumMAPENTRY_SUBPLAYER: {
		Short sCopyNO = static_cast<Short>(mapMsg.copyNo);
		Short sSubNum = static_cast<Short>(mapMsg.numPlayers);

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (pCEntry) {
			CMapEntryCopyCell* pCCopyInfo = pCEntry->GetCopy(sCopyNO);
			if (pCCopyInfo)
				pCCopyInfo->AddCurPlyNum(-1 * sSubNum);
		}
	}
	break;
	case enumMAPENTRY_SUBCOPY: {
		Short sCopyNO = static_cast<Short>(mapMsg.copyNo);

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (pCEntry) {
			pCEntry->ReleaseCopy(sCopyNO);
		}
		//  :     
		net::msg::MapEntryMessage meMsg;
		meMsg.srcMapName = szSrcMapN;
		meMsg.targetMapName = szTarMapN;
		meMsg.subType = net::msg::MAPENTRY_RETURN;
		meMsg.resultCode = enumMAPENTRYO_COPY_CLOSE_SUC;
		meMsg.returnCopyNo = sCopyNO;
		auto wpk = net::msg::serialize(meMsg, CMD_MT_MAPENTRY);

		BEGINGETGATE();
		GateServer* pGateServer = NULL;
		while (pGateServer = GETNEXTGATE()) {
			pGateServer->SendData(wpk);
			break;
		}
	}
	break;
	case enumMAPENTRY_DESTROY: {
		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (g_cchLogMapEntry)
			//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
			ToLogService("common", "receive request to destroy entry??position {} --> {}", szSrcMapN.c_str(),
						 szTarMapN.c_str());
		if (pCEntry) {
			string strScript = "after_destroy_entry_";
			strScript += szSrcMapN;
			g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry,
							   DOSTRING_PARAM_END);
			g_CDMapEntry.Del(pCEntry);

			//  :     
			auto wpk = net::msg::serialize(net::msg::GmMapEntryResultMessage{
				szSrcMapN, szTarMapN, (int64_t)enumMAPENTRY_RETURN, (int64_t)enumMAPENTRYO_DESTROY_SUC
			});

			BEGINGETGATE();
			GateServer* pGateServer = NULL;
			while (pGateServer = GETNEXTGATE()) {
				pGateServer->SendData(wpk);
				break;
			}
			if (g_cchLogMapEntry)
				//LG("??????????", "?????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "destroy entry success??position {} --> {}", szSrcMapN.c_str(),
							 szTarMapN.c_str());
		}
	}
	break;
	case enumMAPENTRY_COPYPARAM: {
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes)
			break;
		pCMap = pCMapRes->GetCopy(static_cast<Short>(mapMsg.copyId));
		if (!pCMap)
			break;
		for (dbc::Char i = 0; i < defMAPCOPY_INFO_PARAM_NUM; i++)
			pCMap->SetInfoParam(i, static_cast<Long>(mapMsg.params[i]));
	}
	break;
	case enumMAPENTRY_COPYRUN: {
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes)
			break;
		pCMap = pCMapRes->GetCopy(static_cast<Short>(mapMsg.copyId));
		if (!pCMap)
			break;

		Char chType = static_cast<Char>(mapMsg.condType);
		Long lVal = static_cast<Long>(mapMsg.condValue);
		pCMapRes->SetCopyStartCondition(chType, lVal);
	}
	break;
	case enumMAPENTRY_RETURN: {
		CMapRes* pCMap = FindMapByName(szTarMapN.c_str(), true);
		if (!pCMap)
			break;
		switch (mapMsg.resultCode) {
		case enumMAPENTRYO_CREATE_SUC: {
			pCMap->CheckEntryState(enumMAPENTRY_STATE_OPEN);
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "receive entry create success ??position {} --> {}", szSrcMapN.c_str(),
							 szTarMapN.c_str());
		}
		break;
		case enumMAPENTRYO_DESTROY_SUC: {
			if (g_cchLogMapEntry)
				//LG("??????????", "???????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "receive entry destroy success??position {} --> {}", szSrcMapN.c_str(),
							 szTarMapN.c_str());
			pCMap->CheckEntryState(enumMAPENTRY_STATE_CLOSE_SUC);
		}
		break;
		case enumMAPENTRYO_COPY_CLOSE_SUC: {
			pCMap->CopyClose(static_cast<Short>(mapMsg.returnCopyNo));
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	};
}
