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

void CGameApp::ProcessNetMsg(int nMsgType, GateServer* pGate, RPACKET pkt)
{
		switch (nMsgType)
		{
		case NETMSG_GATE_CONNECTED: // ??????Gate
		{
			ToLogService("network", "Exec OnGateConnected()");
			OnGateConnected(pGate, pkt);
			break;}

		case NETMSG_GATE_DISCONNECT: // ??Gate???????
		{
			OnGateDisconnect(pGate, pkt);
			break;}

		case NETMSG_PACKET: // ????????
		{
			ProcessPacket(pGate, pkt);
			break;}

		}
}

void CGameApp::ProcessInfoMsg(pNetMessage msg, short sType, InfoServer* pInfo)
{
		switch (sType)
		{
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

void CGameApp::OnInfoConnected(InfoServer* pInfo)
{
		//??�InfoServer
		pInfo->Login();
}

void CGameApp::OnInfoDisconnected(InfoServer* pInfo)
{
		// ????????
		g_StoreSystem.InValid();
	pInfo->InValid();
}

void CGameApp::ProcessMsg(pNetMessage msg, InfoServer* pInfo)
{
		if (msg)
		{
			switch (msg->msgHead.msgID)
			{
			case INFO_LOGIN:		// ??�InfoServer
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					ToLogService("store", "InfoServer Login Success!");

					pInfo->SetValid();
					//g_StoreSystem.SetValid();

					//??InfoServer?????????????????
					g_StoreSystem.GetItemList();
					g_StoreSystem.GetAfficheList();
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					ToLogService("store", "InfoServer Login Failed!");
					pInfo->InValid();
				}
				else
				{
					//ToLogService("store", "??�InfoServer???????????!");
					ToLogService("store", "enter InfoServer message data error!");
				}
			}
			break;

			case INFO_REQUEST_ACCOUNT:	// ?????????
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????????????!", lOrderID);
					ToLogService("store", "[{}]succeed to obtain account information!", lOrderID);

					RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));
					g_StoreSystem.AcceptRoleInfo(lOrderID, ChaInfo);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????????????!", lOrderID);
					ToLogService("store", "[{}]obtain account information failed!", lOrderID);

					g_StoreSystem.CancelRoleInfo(lOrderID);
				}
				else
				{
					//ToLogService("store", "?????????????????!");
					ToLogService("store", "account information message data error");
				}
			}
			break;

			case INFO_REQUEST_STORE:	// ?????????
			{
				//ToLogService("store", "?????????!");
				ToLogService("store", "get store list!");
				if (msg->msgHead.subID == INFO_SUCCESS)		// ??????????
				{
					//???????
					short lComNum = LOWORD(msg->msgHead.msgExtend);
					//???????
					short lClassNum = HIWORD(msg->msgHead.msgExtend);
					//???�??????
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
					//???�??????
					g_StoreSystem.SetItemClass((ClassInfo*)(msg->msgBody), lClassNum);
					//??????????
					g_StoreSystem.SetItemList((StoreStruct*)((char*)msg->msgBody + lClassNum * sizeof(ClassInfo)), lComNum);
				}
				else
				{
					//ToLogService("store", "?????????????????!");
					ToLogService("store", "store list message data error!");
				}
			}
			break;

			case INFO_REQUEST_AFFICHE:		// ??????????
			{
				//ToLogService("store", "??�??????!");
				ToLogService("store", "get offiche information!");
				if (msg->msgHead.subID == INFO_SUCCESS) // ???????????
				{
					//???????
					long lAfficheNum = msg->msgHead.msgExtend;
					//???�??????
					g_StoreSystem.SetAfficheList((AfficheInfo*)msg->msgBody, lAfficheNum);
				}
				else if (msg->msgHead.subID == INFO_FAILED) // ???????????
				{
					//???????
					long lAfficheNum = msg->msgHead.msgExtend;
					//???�??????
					g_StoreSystem.SetAfficheList((AfficheInfo*)msg->msgBody, lAfficheNum);
				}
				else
				{
					//ToLogService("store", "??????????????????!");
					ToLogService("store", "offiche information message data error!");
				}
			}
			break;

			case INFO_STORE_BUY:		// ???????
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
				else
				{
					//ToLogService("store", "????????????????????????!");
					ToLogService("store", "confirm information that buy item message data error!");
				}
			}
			break;

			case INFO_REGISTER_VIP:
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????VIP???!", lOrderID);
					ToLogService("store", "[{}] buy VIP succeed !", lOrderID);

					RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));

					g_StoreSystem.AcceptVIP(lOrderID, ChaInfo, msg->msgHead.msgExtend);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????VIP???!", lOrderID);
					ToLogService("store", "[{}] buy VIP failed !", lOrderID);

					g_StoreSystem.CancelVIP(lOrderID);
				}
				else
				{
					//ToLogService("store", "????VIP?????????????????!");
					ToLogService("store", "buy VIP confirm information message data error !");
				}
			}
			break;

			case INFO_EXCHANGE_MONEY:		// ???????
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]?????????!", lOrderID);
					ToLogService("store", "[{}]change token succeed !", lOrderID);

					RoleInfo* ChaInfo = (RoleInfo*)((char*)msg->msgBody + sizeof(long long));
					g_StoreSystem.AcceptChange(lOrderID, ChaInfo);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]??????????!", lOrderID);
					ToLogService("store", "[{}]change token failed!", lOrderID);

					g_StoreSystem.CancelChange(lOrderID);
				}
				else
				{
					//ToLogService("store", "????????????????????????!");
					ToLogService("store", "change token confirm information message data error !");
				}
			}
			break;

			case INFO_REQUEST_HISTORY:		// ????????�
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????????�???!", lOrderID);
					ToLogService("store", "[{}]succeed to query trade note!", lOrderID);

					HistoryInfo* pRecord = (HistoryInfo*)((char*)msg->msgBody + sizeof(long long));
					g_StoreSystem.AcceptRecord(lOrderID, pRecord);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????????�???!", lOrderID);
					ToLogService("store", "[{}]query trade note failed!", lOrderID);

					g_StoreSystem.CancelRecord(lOrderID);
				}
				else
				{
					//ToLogService("store", "?????�????????????????????!");
					ToLogService("store", "trade note query resoibsuib nessage data error!");
				}
			}
			break;

			case INFO_SND_GM_MAIL:
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????GM??????!", lOrderID);
					ToLogService("store", "[{}]send GM mail success!", lOrderID);

					long lMailID = msg->msgHead.msgExtend;
					g_StoreSystem.AcceptGMSend(lOrderID, lMailID);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????GM??????!", lOrderID);
					ToLogService("store", "[{}]send GM mail failed!", lOrderID);

					g_StoreSystem.CancelGMSend(lOrderID);
				}
				else
				{
					//ToLogService("store", "????GM??????????????!");
					ToLogService("store", "send GM mail message data error!");
				}
			}
			break;

			case INFO_RCV_GM_MAIL:
			{
				if (msg->msgHead.subID == INFO_SUCCESS)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????GM??????!", lOrderID);
					ToLogService("store", "[{}]receive GM mail success!", lOrderID);

					MailInfo* pMi = (MailInfo*)((char*)msg->msgBody + sizeof(long long));
					g_StoreSystem.AcceptGMRecv(lOrderID, pMi);
				}
				else if (msg->msgHead.subID == INFO_FAILED)
				{
					long long lOrderID = *(long long*)msg->msgBody;
					//ToLogService("store", "[{}]????GM??????!", lOrderID);
					ToLogService("store", "[{}]reciveGMmail failed!", lOrderID);

					g_StoreSystem.CancelGMRecv(lOrderID);
				}
				else
				{
					//ToLogService("store", "????GM??????????????!");
					ToLogService("store", "receive GM mail message data error!");
				}
			}
			break;

			case INFO_EXCEPTION_SERVICE:	//???????
			{
				//ToLogService("store", "InfoServer???????!");
				ToLogService("store", "InfoServer refuse serve!");
				g_StoreSystem.InValid();
				pInfo->InValid();
			}
			break;

			default:
			{
				//ToLogService("store", "??�???????????!");
				ToLogService("store", "get unknown information type!");
			}
			break;

			}

			FreeNetMessage(msg);
		}
}

// ??Gate??????????????
void CGameApp::OnGateConnected(GateServer* pGate, RPACKET pkt)
{
	// ??GateServer???GameServer
	WPACKET	wpk = GETWPACKET();
	WRITE_CMD(wpk, CMD_MT_LOGIN);
	WRITE_STRING(wpk, GETGMSVRNAME());
	WRITE_STRING(wpk, g_pGameApp->m_strMapNameList.c_str());

	ToLogService("network", "[{}]", g_pGameApp->m_strMapNameList.c_str());

	pGate->SendData(wpk);
}

// ??Gate???????????????
void CGameApp::OnGateDisconnect(GateServer* pGate, RPACKET pkt)
{
		bool ret = VALIDRPACKET(pkt);
	if (!ret) return; // ?????packet


	//NOTE(Ogge): Weird that we first cast to GatePlayer(base class) and then in while loop to CPlayer(derived class)
	auto tmp = ToPointer<GatePlayer>(READ_LONG(pkt));

	while (tmp != NULL)
	{
		if (static_cast<CPlayer*>(tmp)->IsValid())
		{
			GoOutGame((CPlayer*)tmp, true);
			tmp->OnLogoff();
		}

		tmp = tmp->Next;
	}

	pGate->Invalid();
}

// ?????????????
void CGameApp::ProcessPacket(GateServer* pGate, RPACKET pkt)
{
		CPlayer* l_player = nullptr;
	uShort cmd = READ_CMD(pkt);

	switch (cmd)
	{
	case CMD_TM_LOGIN_ACK:
	{
		short	sErrCode;
		if (sErrCode = READ_SHORT(pkt))
		{
			/*ToLogService("network", "??� GateServer: {}:{}???[{}], ?????[{}]",
				pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(sErrCode),
				g_pGameApp->m_strMapNameList.c_str());*/
			ToLogService("network", "enter GateServer: {}:{} failed [{}], register map[{}]",
				pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(sErrCode),
				g_pGameApp->m_strMapNameList.c_str());
			DISCONNECT(pGate);
		}
		else
		{
			pGate->GetName() = READ_STRING(pkt);
			if (!strcmp(pGate->GetName().c_str(), ""))
			{
				/*ToLogService("network", "??� GateServer: [{}:{}]??? ??�???�???????????????????????",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());*/
				ToLogService("network", "entry GateServer: [{}:{}]success but do not get his name??so disconnection and entry again",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());

				DISCONNECT(pGate);
			}
			else
			{
				/*ToLogService("network", "??� GateServer: {} [{}:{}]??? [MapName:{}]",
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
	//		int reason = READ_LONG(pkt);

	//		ToLogService("network", "Gate {} Ip {} {} deleted\r", pGate->GetName(), pGate->GetIP(), reason);

	//		pGate->Invalid();

	//		break;
	//	}
	//	//End

	case CMD_TM_ENTERMAP:
	{
		net::msg::TmEnterMapMessage enterMsg;
		net::msg::deserialize(pkt, enterMsg);

		if (enterMsg.password.empty())
			break;
		if (enterMsg.mapName.empty())
			break;

		ToLogService("map", "start entry map atorID = {} enter--------------------------", enterMsg.dbCharId);

		l_player = CreateGamePlayer(enterMsg.password.c_str(), enterMsg.dbCharId, enterMsg.worldId,
			enterMsg.mapName.c_str(), enterMsg.loginFlag == 0 ? 0 : 1);
		if (!l_player)
		{
			WPACKET pkret = GETWPACKET();
			WRITE_CMD(pkret, CMD_MC_ENTERMAP);
			WRITE_SHORT(pkret, ERR_MC_ENTER_ERROR);
			WRITE_LONG(pkret, enterMsg.dbCharId);
			WRITE_LONG(pkret, enterMsg.gateAddr);
			WRITE_SHORT(pkret, 1);
			pGate->SendData(pkret);
			ToLogService("map", LogLevel::Error, "when create new palyer ID = {} assign memory failed", enterMsg.dbCharId);
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
		if (pCCha->Cmd_EnterMap(enterMsg.mapName.c_str(), enterMsg.mapCopyNo, enterMsg.posX, enterMsg.posY, enterMsg.loginFlag))
		{
			l_player->MisEnterMap();
			if (enterMsg.loginFlag == 0)
			{
				NoticePlayerLogin(l_player);
			}
		}

		ToLogService("map", "end up entry map  [{}]================", pCCha->GetLogName());
		break;
	}
	case CMD_TM_GOOUTMAP:
	{
		l_player = ToPointer<CPlayer>(READ_LONG_R(pkt));
		DWORD	l_gateaddr = READ_LONG_R(pkt);

		if (!l_player)
			break;
		try
		{
			if (l_player->GetGateAddr() != l_gateaddr)
			{
				//ToLogService("errors", LogLevel::Error, "?????ID: {}, ????????????:{:x}, gate:{:x},cmd={}, ?????({}).", l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr,cmd, l_player->IsValidFlag());
				ToLogService("errors", LogLevel::Error, "DB ID: {}, address not matching??local :{:x}, gate:{:x},cmd={}, validity({}).", l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr, cmd, l_player->IsValidFlag());
				break;
			}
		}
		catch (...)
		{
			//ToLogService("errors", LogLevel::Error, "===========================??Gate?????????????{},cmd ={}", l_player, cmd);
			ToLogService("errors", LogLevel::Error, "===========================from Gate player's address error {},cmd ={}", static_cast<void*>(l_player), static_cast<int>(cmd));
			break;
		}
		if (!l_player->IsValid())
		{
			//LG("enter_map", "???????????\n");
			ToLogService("map", "this palyer already impotence");
			break;
		}
		if (l_player->GetMainCha()->GetPlayer() != l_player)
		{
			//ToLogService("errors", LogLevel::Error, "????player????????????{}??Gate???[????{}, ????{}]????cmd={}", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
			ToLogService("errors", LogLevel::Error, "two player not matching, character name: {}, Gate address [local {}, guest {}], cmd={}", l_player->GetMainCha()->GetLogName(), static_cast<void*>(l_player->GetMainCha()->GetPlayer()), static_cast<void*>(l_player), static_cast<int>(cmd));
		}
		ToLogService("map", "start leave map--------");

		char chOffLine = READ_CHAR(pkt); // ???????(0)
		ToLogService("map", "Delete Player [{}]", l_player->GetMainCha()->GetLogName());

		char	szLogName[512];
		strncpy(szLogName, l_player->GetMainCha()->GetLogName(), 512 - 1);
		//LG("OutMap", "%s????????\n", szLogName);

		GoOutGame(l_player, !chOffLine, false);

		//LG("enter_map", "?????????========\n\n");
		ToLogService("map", "end and leave the map========");

		//LG("OutMap", "%s?????\n", szLogName);

		break;
	}
	case CMD_PM_SAY2ALL:
	{
		uLong ulChaID = pkt.ReadInt64();
		std::string szContent = pkt.ReadString();
		long lChatMoney = pkt.ReadInt64();

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney))
			{
				//pCha->SystemNotice("??????????,?????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00007));

				WPACKET l_wpk = GETWPACKET();
				WRITE_CMD(l_wpk, CMD_MP_SAY2ALL);
				WRITE_CHAR(l_wpk, 0);
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("?????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00006), lChatMoney);

			WPACKET l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk, CMD_MP_SAY2ALL);
			WRITE_CHAR(l_wpk, 1);
			WRITE_STRING(l_wpk, pCha->GetName());
			WRITE_STRING(l_wpk, szContent);
			pCha->ReflectINFof(pCha, l_wpk);
		}
	}
	break;
	case CMD_PM_SAY2TRADE:
	{
		uLong ulChaID = pkt.ReadInt64();
		std::string szContent = pkt.ReadString();
		long lChatMoney = pkt.ReadInt64();

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney))
			{
				//pCha->SystemNotice("??????????,????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00005));

				WPACKET l_wpk = GETWPACKET();
				WRITE_CMD(l_wpk, CMD_MP_SAY2TRADE);
				WRITE_CHAR(l_wpk, 0);
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00004), lChatMoney);

			WPACKET l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk, CMD_MP_SAY2TRADE);
			WRITE_CHAR(l_wpk, 1);
			WRITE_STRING(l_wpk, pCha->GetName());
			WRITE_STRING(l_wpk, szContent);
			pCha->ReflectINFof(pCha, l_wpk);
		}
	}
	break;
	case CMD_PM_TEAM: // GroupServer??????????
	{
		ProcessTeamMsg(pGate, pkt);
		break;
	}
	case CMD_PM_GUILDINFO:	// GroupServer???????????
	{
		ProcessGuildMsg(pGate, pkt);
		break;
	}
	case CMD_PM_GUILD_CHALLMONEY:
	{
		ProcessGuildChallMoney(pGate, pkt);
	}
	break;
	case CMD_PM_GUILD_CHALL_PRIZEMONEY:
	{
		//ProcessGuildChallPrizeMoney( pGate, pkt );

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MM_GUILD_CHALL_PRIZEMONEY);
		WRITE_LONG(WtPk, 0);
		WRITE_LONG(WtPk, READ_LONG(pkt));
		WRITE_LONG(WtPk, READ_LONG(pkt));
		WRITE_SHORT(WtPk, 0);
		WRITE_LONG(WtPk, 0);
		WRITE_LONG(WtPk, 0);
		pGate->SendData(WtPk);
	}
	break;
	case CMD_TM_MAPENTRY:
	{
		ProcessDynMapEntry(pGate, pkt);
		break;
	}
	/*case CMD_TM_KICKCHA:
		{
			long lChaDbID = READ_LONG(pkt);
			CPlayer* pCOldPly = FindPlayerByDBChaID(lChaDbID);
			if (!pCOldPly || !pCOldPly->IsValid())
				break;

			pCOldPly->GetCtrlCha()->BreakAction();
			pCOldPly->MisLogout();
			pCOldPly->MisGooutMap();
			ReleaseGamePlayer( pCOldPly );
			break;
		}*/
	case CMD_TM_MAPENTRY_NOMAP:
	{
		break;
	}
	case CMD_PM_GARNER2_UPDATE:
	{
		ProcessGarner2Update(pkt);
		break;
	}
	case CMD_PM_EXPSCALE:
	{
		//  ??????
		uLong ulChaID = pkt.ReadInt64();
		uLong ulTime = pkt.ReadInt64();

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);

		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();

			if (pCha->IsScaleFlag())
			{
				break;
			}

			switch (ulTime)
			{
			case 1:
			{
				if (pCha->m_noticeState != 1)
				{
					pCha->m_noticeState = 1;
					//pCha->BickerNotice("???????????????? %d ????", ulTime);
					pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00001), ulTime);
					pCha->SetExpScale(100);
				}

			}	break;
			case 2:
			{
				if (pCha->m_noticeState != 2)
				{
					pCha->m_noticeState = 2;
					//pCha->BickerNotice("???????????????? %d ????", ulTime);
					pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00001), ulTime);
					pCha->SetExpScale(100);
				}
			}  break;
			case 3:
			{
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
				if (pCha->m_retry3 == 1)
				{
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
					//pCha->BickerNotice("???????????????? 3 ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????");
					//pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00002));
					//pCha->SetExpScale(50);
				}
				pCha->m_retry3++;
				//	yyy	change	end!
			}  break;
			case 4:
			{
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
				if (pCha->m_retry3 == 1)
				{
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
				}

				pCha->m_retry3++;

				//	yyy	change	end!
			}  break;
			case 5:
			{
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
				if (pCha->m_retry3 == 1)
				{
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
					//pCha->BickerNotice("????????????????????????????????????????????????????????????????????????????????????????????????????? 5 ????????????????");
					//pCha->BickerNotice(RES_STRING(GM_GAMEAPPNET_CPP_00008));
					//pCha->SetExpScale(0);
				}
				pCha->m_retry3++;
				// pCha->m_retry5++;
			}  break;
			case 6:
			{
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
				if (pCha->m_retry3 == 1)
				{
					KICKPLAYER(pCha->GetPlayer(), 5);
					g_pGameApp->GoOutGame(pCha->GetPlayer(), true);
				}

				pCha->m_retry3++;
			}  break;
			}
		}
	}  break;
	default:
		if (cmd / 500 == CMD_MM_BASE / 500)
		{
			ProcessInterGameMsg(cmd, pGate, pkt);
		}
		else
		{
			l_player = ToPointer<CPlayer>(READ_LONG_R(pkt));
			if (cmd / 500 == CMD_PM_BASE / 500 && !l_player)
			{
				ProcessGroupBroadcast(cmd, pGate, pkt);
			}
			else
			{
				if (!l_player)
					break;
				try
				{
					DWORD	l_gateaddr = READ_LONG_R(pkt);
					if (l_player->GetGateAddr() != l_gateaddr)
					{
						/*ToLogService("errors", LogLevel::Error, "?????ID:{}, ????????????:{}, gate:{},cmd={}, ?????({})", l_player->GetDBChaId(), l_player->GetGateAddr(),
							l_gateaddr,cmd, l_player->IsValidFlag() );*/
						ToLogService("errors", LogLevel::Error, "DB ID:{}, address not matching??local :{}, gate:{},cmd={}, validity ({})", l_player->GetDBChaId(), l_player->GetGateAddr(),
							l_gateaddr, cmd, l_player->IsValidFlag());
						break;
					}
				}
				catch (...)
				{
					//ToLogService("errors", LogLevel::Error, "===========================??Gate?????????????{},cmd ={}", l_player, cmd);
					ToLogService("errors", LogLevel::Error, "===========================Player address error that come from Gate {},cmd ={}", static_cast<void*>(l_player), static_cast<int>(cmd));
					break;
				}
				if (!l_player->IsValid())
					break;
				if (l_player->GetMainCha()->GetPlayer() != l_player)
				{
					//ToLogService("errors", LogLevel::Error, "????player????????????{}??Gate???[????{}, ????{}]????cmd={}", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
					ToLogService("errors", LogLevel::Error, "two player not matching, character name: {}, Gate address [local {}, guest {}], cmd={}", l_player->GetMainCha()->GetLogName(), static_cast<void*>(l_player->GetMainCha()->GetPlayer()), static_cast<void*>(l_player), static_cast<int>(cmd));
				}

				CCharacter* pCCha = l_player->GetCtrlCha();
				if (!pCCha)
					break;
				if (g_pGameApp->IsValidEntity(pCCha->GetID(), pCCha->GetHandle()))
				{
					g_pNoticeChar = pCCha;

					g_ulCurID = pCCha->GetID();
					g_lCurHandle = pCCha->GetHandle();

					pCCha->ProcessPacket(cmd, pkt);

					g_ulCurID = defINVALID_CHA_ID;
					g_lCurHandle = defINVALID_CHA_HANDLE;

					g_pNoticeChar = NULL;
				}
				else
				{
					//ToLogService("errors", LogLevel::Error, "???CMD_CM_BASE?????[{}]?, ??????pCCha???", cmd);
					ToLogService("errors", LogLevel::Error, "when receive CMD_CM_BASE message[{}], find character pCCha is null", cmd);
				}
				break;
			}
		}
	}
}

// ??????????????
void CGameApp::ProcessGuildChallMoney(GateServer* pGate, RPACKET pkt)
{
		DWORD dwChaDBID = READ_LONG(pkt);
	DWORD dwMoney = READ_LONG(pkt);
	std::string pszGuild1 = READ_STRING(pkt);
	std::string pszGuild2 = READ_STRING(pkt);

	//	2007-8-4	yangyinyu	change	begin!	//	?????????????????????????????????
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetMainCha();
		//pCha->AddMoney( "??", dwMoney );
		pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
		/*pCha->SystemNotice( "??????????%s?????????%s????????????????(%u)????????????\n", pszGuild1, pszGuild2, dwMoney );
		ToLogService("common", "??{}??????????{}?????????{}????????????????({})????????????", pCha->GetGuildName(), pszGuild1, pszGuild2, dwMoney);*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00009), pszGuild1.c_str(), pszGuild2.c_str(), dwMoney);
		ToLogService("common", "bidder and consortia [{}] battle was consortia [{}] replace, your consortia gold ({}) had back to you", pCha->GetGuildName(), pszGuild1.c_str(), pszGuild2.c_str(), dwMoney);
	}
	else
	{
		//LG( "?????????", "?????????????????????DBID[%u],???[%u].\n", dwChaDBID, dwMoney );
		ToLogService("common", "not find deacon information finger??cannot back gold DBID[{}],how much money[{}].", dwChaDBID, dwMoney);
	}
}

void CGameApp::ProcessGuildChallPrizeMoney(GateServer* pGate, RPACKET pkt)
{
		DWORD dwChaDBID = READ_LONG(pkt);
	DWORD dwMoney = READ_LONG(pkt);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->AddMoney("??", dwMoney);
		/*pCha->SystemNotice( "????????????%s????????????????????�?????%u????", pCha->GetGuildName(), dwMoney );
		ToLogService("common", "????????????{}????????????????????�?????{}????", pCha->GetGuildName(), dwMoney);*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
		ToLogService("common", "congratulate you have leading the consortia??{}??get win in consortia battle??gain bounty??{}????", pCha->GetGuildName(), dwMoney);
	}
	else
	{
		//LG( "?????????", "??????????????????????DBID[%u],???[%u]", dwChaDBID, dwMoney );
		ToLogService("common", "cannot find deacon information finger??cannot hortation DBID[{}],how much money[{}]", dwChaDBID, dwMoney);
	}
}

// ???????????
void CGameApp::ProcessGuildMsg(GateServer* pGate, RPACKET pkt)
{
		DWORD dwChaDBID = READ_LONG(pkt);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetCtrlCha();
		DWORD dwGuildID = READ_LONG(pkt);
		DWORD dwLeaderID = READ_LONG(pkt);
		std::string pszGuildName = READ_STRING(pkt);
		std::string pszGuildMotto = READ_STRING(pkt);
		pCha->SetGuildName(pszGuildName.c_str());
		pCha->SetGuildMotto(pszGuildMotto.c_str());
		pCha->SyncGuildInfo();
	}
}

// ?????????????
void CGameApp::ProcessTeamMsg(GateServer* pGate, RPACKET pkt)
{
		//ToLogService("common", "?????????????");

		char cTeamMsgType = READ_CHAR(pkt);

	switch (cTeamMsgType)
	{
	case TEAM_MSG_ADD: {	/*ToLogService("common", "?????? [?�???] ???");*/ break; }
	case TEAM_MSG_LEAVE: {	/*ToLogService("common", "?????? [??????] ???");*/ break; }
	case TEAM_MSG_UPDATE: {	/*ToLogService("common", "?????? [??????] ???");*/ break; }
	default:
		//ToLogService("common", "????????Team??? [{}]", cTeamMsgType);
		return;
	}

	char cMemberCnt = READ_CHAR(pkt);
	//ToLogService("common", "????????????[{}]", cMemberCnt); // ?????????? < 2????GroupServer????????????????.

	uplayer Team[MAX_TEAM_MEMBER];
	CPlayer* PlayerList[MAX_TEAM_MEMBER];
	bool	CanSeenO[MAX_TEAM_MEMBER][2];
	bool	CanSeenN[MAX_TEAM_MEMBER][2];

	// ????????????????????Player
	for (char i = 0; i < cMemberCnt; i++)
	{
		std::string pszGateName = READ_STRING(pkt);
		DWORD dwGateAddr = READ_LONG(pkt);
		DWORD dwChaDBID = READ_LONG(pkt);
		Team[i].Init(pszGateName.c_str(), dwGateAddr, dwChaDBID);
		if (!Team[i].pGate)
		{
			ToLogService("common", "GameServer can't find matched Gate??{}, addr = 0x{:X}, chaid = {}.", pszGateName.c_str(), dwGateAddr, dwChaDBID);
			ToLogService("common", "\tGameServer all Gate:");
			BEGINGETGATE();
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE())
			{
				ToLogService("common", "\t{}", pGateServer->GetName().c_str());
			}
		}

		PlayerList[i] = GetPlayerByDBID(dwChaDBID);

		//ToLogService("common", "???: {}, {} {} ????Gate [{}]", PlayerList[i]!=NULL ? PlayerList[i]->GetCtrlCha()->GetLogName():"(????????Server!)", dwChaDBID, dwGateAddr, pszGateName);
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
		if (pLeave)
		{
			// Remove party fruit when leaving team
			if (pLeave->IsTeamLeader())
			{
				for (auto i = 0; i < cMemberCnt - 1; ++i)
				{
					if (PlayerList[i] && PlayerList[i]->GetMainCha())
					{
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
	else if (cTeamMsgType == TEAM_MSG_ADD)
	{
		try {
			[&]()
			{
				CPlayer* newPly = PlayerList[cMemberCnt - 1];
				if (!newPly)
				{
					return;
				}

				CCharacter* newCha = newPly->GetMainCha();
				if (!newCha)
				{
					return;
				}

				CPlayer* leaderPly = GetPlayerByDBID(Team[0].m_dwDBChaId);
				if (!leaderPly)
				{
					return;
				}

				CCharacter* leaderCha = leaderPly->GetMainCha();
				if (!leaderCha)
				{
					return;
				}

				CSkillState& leader_states = leaderCha->m_CSkillState;

				//if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // ״̬��ʱ���
				if (leader_states.HasState(217))
				{
					const auto& state = leader_states.GetSStateByID(217);
					const auto use_duration = state->lOnTick * 1000;
					const auto remaining = (use_duration - (GetTickCount() - state->ulStartTick)) / 1000;
					newCha->AddSkillState(g_uchFightID, newCha->GetID(), newCha->GetHandle(), enumSKILL_TYPE_SELF,
						enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, 217, state->GetStateLv(), remaining, enumSSTATE_ADD, true);
					return;
				}

				if (leader_states.HasState(218))
				{
					const auto& state = leader_states.GetSStateByID(218);
					const auto use_duration = state->lOnTick * 1000;
					const auto remaining = (use_duration - (GetTickCount() - state->ulStartTick)) / 1000;
					newCha->AddSkillState(g_uchFightID, newCha->GetID(), newCha->GetHandle(), enumSKILL_TYPE_SELF,
						enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, 218, state->GetStateLv(), remaining, enumSSTATE_ADD, true);
					return;
				}
			}();
		}
		catch (...) {
			ToLogService("errors", LogLevel::Error, "Exception handling: newPly invalid, cMemberCnt={}", cMemberCnt);
		}
	}
	
	// ?????????????, ???cMember????1, ?????????????
	for (int i = 0; i < nLeftMember; i++)
	{
		if (PlayerList[i] == NULL) continue;

		PlayerList[i]->ClearTeamMember();
		for (int j = 0; j < nLeftMember; j++)
		{
			if (i == j) continue;
			PlayerList[i]->AddTeamMember(&Team[j]);
		}
		if (nLeftMember != 1)
		{
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

	for (char i = 0; i < cMemberCnt; i++)
	{
		if (i < 5)
		{
			if (PlayerList[i])
			{
				CCharacter* pCtrlCha = PlayerList[i]->GetCtrlCha();
				if (pCtrlCha)
				{
					SubMap* pSubMap = pCtrlCha->GetSubMap();
					if (pSubMap && pSubMap->GetMapRes())
					{
						if (!(pSubMap->GetMapRes()->CanTeam()))
						{
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
void CGameApp::CheckSeeWithTeamChange(bool CanSeen[][2], CPlayer** pCPlayerList, char chMemberCnt)
{
		if (chMemberCnt <= 1)
			return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter* pCProcCha = pCProcPly->GetCtrlCha(), * pCCurCha;
	for (char i = 0; i < chMemberCnt - 1; i++)
	{
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		if (pCProcCha->IsInEyeshot(pCCurCha))
		{
			pCProcCha->CanSeen(pCCurCha) ? CanSeen[i][0] = true : CanSeen[i][0] = false;
			pCCurCha->CanSeen(pCProcCha) ? CanSeen[i][1] = true : CanSeen[i][1] = false;
		}
	}
}

// ??????????????????????????????
void CGameApp::RefreshTeamEyeshot(bool CanSeenOld[][2], bool CanSeenNew[][2], CPlayer** pCPlayerList, char chMemberCnt, char chRefType)
{
		if (chMemberCnt <= 1)
			return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter* pCProcCha = pCProcPly->GetCtrlCha(), * pCCurCha;
	for (char i = 0; i < chMemberCnt - 1; i++)
	{
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		if (pCProcCha->IsInEyeshot(pCCurCha))
		{
			if (chRefType == TEAM_MSG_ADD)
			{
				if (!CanSeenOld[i][0] && CanSeenNew[i][0])
					pCCurCha->BeginSee(pCProcCha);
				if (!CanSeenOld[i][1] && CanSeenNew[i][1])
					pCProcCha->BeginSee(pCCurCha);
			}
			else if (chRefType == TEAM_MSG_LEAVE)
			{
				if (CanSeenOld[i][0] && !CanSeenNew[i][0])
					pCCurCha->EndSee(pCProcCha);
				if (CanSeenOld[i][1] && !CanSeenNew[i][1])
					pCProcCha->EndSee(pCCurCha);
			}
		}
	}
}

// ???????????????????
void CGameApp::RefreshTeamEyeshot(CPlayer** pCPlayerList, char chMemberCnt, char chRefType)
{
		if (chMemberCnt <= 1)
			return;

	CPlayer* pCProcPly = pCPlayerList[chMemberCnt - 1];
	if (!pCProcPly)
		return;

	CPlayer* pCCurPly;
	CCharacter* pCProcCha = pCProcPly->GetCtrlCha(), * pCCurCha;
	bool	bCurChaHide;
	bool	bProcChaHide = pCProcCha->IsHide();
	for (char i = 0; i < chMemberCnt - 1; i++)
	{
		pCCurPly = pCPlayerList[i];
		if (!pCCurPly)
			continue;
		pCCurCha = pCCurPly->GetCtrlCha();
		bCurChaHide = pCCurCha->IsHide();
		if (bProcChaHide || bCurChaHide) // ????????
		{
			if (pCProcCha->IsInEyeshot(pCCurCha))
			{
				if (chRefType == TEAM_MSG_ADD)
				{
					if (bProcChaHide)
						pCCurCha->BeginSee(pCProcCha);
					if (bCurChaHide)
						pCProcCha->BeginSee(pCCurCha);
				}
				else if (chRefType == TEAM_MSG_LEAVE)
				{
					if (bProcChaHide)
						pCCurCha->EndSee(pCProcCha);
					if (bCurChaHide)
						pCProcCha->EndSee(pCCurCha);
				}
			}
		}
	}
}

BOOL CGameApp::AddVolunteer(CCharacter* pCha)
{
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

BOOL CGameApp::DelVolunteer(CCharacter* pCha)
{
		if (!pCha->IsVolunteer())
			return false;
	pCha->SetVolunteer(false);

	vector<SVolunteer>::iterator it;
	for (it = m_vecVolunteerList.begin(); it != m_vecVolunteerList.end(); it++)
	{
		if (!strcmp((*it).szName, pCha->GetName()))
		{
			m_vecVolunteerList.erase(it);
			return true;
		}
	}

	return false;
}

int CGameApp::GetVolNum()
{
		return (int)m_vecVolunteerList.size();
}

SVolunteer* CGameApp::GetVolInfo(int nIndex)
{
		if (nIndex < 0 || nIndex >= (int)m_vecVolunteerList.size())
			return NULL;

	return &m_vecVolunteerList[nIndex];
}

SVolunteer* CGameApp::FindVolunteer(const char* szName)
{
		vector<SVolunteer>::iterator it;
	for (it = m_vecVolunteerList.begin(); it != m_vecVolunteerList.end(); it++)
	{
		if (!strcmp((*it).szName, szName))
		{
			return (SVolunteer*)&(*it);
		}
	}
	return NULL;
}

void CGameApp::ProcessInterGameMsg(unsigned short usCmd, GateServer* pGate, RPACKET pkt)
{
	long	lSrcID = READ_LONG(pkt);
	short	sNum = READ_SHORT_R(pkt);
	long	lGatePlayerAddr = READ_LONG_R(pkt);
	long	lGatePlayerID = READ_LONG_R(pkt);

	switch (usCmd)
	{
	case CMD_MM_UPDATEGUILDBANK: {
		int guildID = READ_LONG(pkt);
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
				if (pCha->GetGuildID() == guildID) {//&& pCPlayer->GetBankType() == 2){	
					pCPlayer->SynGuildBank(&bag, 0);
				}
			}
		}
		break;
	}
	case CMD_MM_UPDATEGUILDBANKGOLD: {
		int guildID = READ_LONG(pkt);
		BEGINGETGATE();
		CPlayer* pCPlayer;
		CCharacter* pCha = 0;
		GateServer* pGateServer;

		unsigned long long gold = game_db.GetGuildBankGold(guildID);

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_UPDATEGUILDBANKGOLD);
		WRITE_STRING(WtPk, to_string(gold).c_str());

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
	case CMD_MM_GUILD_MOTTO:
	{
		uLong	l_gldid = lSrcID;
		std::string pszMotto = READ_STRING(pkt);
		{//??????FindPlayerChaByID
			BEGINGETGATE();
			CPlayer* pCPlayer;
			CCharacter* pCha = 0;
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE())
			{
				if (!BEGINGETPLAYER(pGateServer))
					continue;
				int nCount = 0;
				while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer))
				{
					if (++nCount > GETPLAYERCOUNT(pGateServer))
					{
						//LG("???????????", "??????:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						ToLogService("errors", LogLevel::Error, "player number:{}, {}", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
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
	case CMD_MM_GUILD_DISBAND:
	{
		uLong	l_gldid = lSrcID;
		{//??????FindPlayerChaByID
			BEGINGETGATE();
			CPlayer* pCPlayer;
			CCharacter* pCha = 0;
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE())
			{
				if (!BEGINGETPLAYER(pGateServer))
					continue;
				int nCount = 0;
				while (pCPlayer = (CPlayer*)GETNEXTPLAYER(pGateServer))
				{
					if (++nCount > GETPLAYERCOUNT(pGateServer))
					{
						//LG("???????????", "??????:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						ToLogService("errors", LogLevel::Error, "player number:{}, {}", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
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
	case CMD_MM_GUILD_KICK:
	{
		uLong	l_chaid = lSrcID;
		CCharacter* pCha = FindMainPlayerChaByID(l_chaid);
		if (pCha)
		{
			pCha->SetGuildName("");
			std::string l_gldname = READ_STRING(pkt);
			pCha->guildPermission = 0;
			pCha->SetGuildID(0);			//???�???ID
			pCha->SetGuildState(0);
			pCha->SetGuildName("");
			pCha->SetGuildMotto("");
			// pCha->SystemNotice("?????????????????[%s].",l_gldname);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00014), l_gldname.c_str());
			pCha->SyncGuildInfo();
		}
	}
	break;
	case CMD_MM_GUILD_APPROVE:
	{
		uLong	l_chaid = lSrcID;
		CCharacter* pCha = FindMainPlayerChaByID(l_chaid);
		if (pCha)
		{
			pCha->SetGuildID(READ_LONG(pkt));				//???�???ID
			pCha->SetGuildState(0);	//???�???Type
			std::string l_gldname = READ_STRING(pkt);
			pCha->SetGuildName(l_gldname.c_str());
			std::string l_gldmotto = READ_STRING(pkt);
			pCha->SetGuildMotto(l_gldmotto.c_str());
			//  pCha->SystemNotice("????????????????[%s].",l_gldname);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00015), l_gldname.c_str());
			pCha->SyncGuildInfo();
		}
	}
	break;
	case CMD_MM_GUILD_REJECT:
	{
		uLong l_chaid = lSrcID;
		CCharacter* pCha = FindMainPlayerChaByID(l_chaid);
		if (pCha)
		{
			pCha->SetGuildID(0);
			pCha->SetGuildState(0);
			pCha->SetGuildName("");

			//pCha->SystemNotice("??????[%s]????????????.",READ_STRING(pkt));
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00016), READ_STRING(pkt).c_str());
		}
	}
	break;
	case	CMD_MM_QUERY_CHAPING:
	{
		std::string cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha)
			break;

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_PING);
		WRITE_LONG(WtPk, GetTickCount());
		WRITE_LONG(WtPk, ToAddress(pGate));
		WRITE_LONG(WtPk, lSrcID);
		WRITE_LONG(WtPk, lGatePlayerID);
		WRITE_LONG(WtPk, lGatePlayerAddr);
		WRITE_SHORT(WtPk, 1);
		pCCha->ReflectINFof(pCCha, WtPk);

		break;
	}
	case	CMD_MM_QUERY_CHA:
	{
		std::string cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha || !pCCha->GetSubMap())
			break;

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_QUERY_CHA);
		WRITE_LONG(WtPk, lSrcID);
		WRITE_STRING(WtPk, pCCha->GetName());
		WRITE_STRING(WtPk, pCCha->GetSubMap()->GetName());
		WRITE_LONG(WtPk, pCCha->GetPos().x);
		WRITE_LONG(WtPk, pCCha->GetPos().y);
		WRITE_LONG(WtPk, pCCha->GetID());
		WRITE_LONG(WtPk, lGatePlayerID);
		WRITE_LONG(WtPk, lGatePlayerAddr);
		WRITE_SHORT(WtPk, 1);
		pGate->SendData(WtPk);

		break;
	}
	case	CMD_MM_QUERY_CHAITEM:
	{
		std::string cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha)
			break;
		pCCha->m_CKitbag.SetChangeFlag();

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_QUERY_CHA);
		WRITE_LONG(WtPk, lSrcID);
		pCCha->WriteKitbag(pCCha->m_CKitbag, WtPk, enumSYN_KITBAG_INIT);
		WRITE_LONG(WtPk, lGatePlayerID);
		WRITE_LONG(WtPk, lGatePlayerAddr);
		WRITE_SHORT(WtPk, 1);
		pGate->SendData(WtPk);

		break;
	}
	case	CMD_MM_CALL_CHA:
	{
		std::string cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha || !pCCha->GetSubMap())
			break;
		bool	bTarIsBoat = READ_CHAR(pkt) ? true : false;
		if (bTarIsBoat != pCCha->IsBoat()) // ???????????
			break;
		std::string cszMapName = READ_STRING(pkt);
		long	lPosX = READ_LONG(pkt);
		long	lPosY = READ_LONG(pkt);
		long	lCopyNO = READ_LONG(pkt);
		pCCha->SwitchMap(pCCha->GetSubMap(), cszMapName.c_str(), lPosX, lPosY, true, enumSWITCHMAP_CARRY, lCopyNO);

		break;
	}
	case	CMD_MM_GOTO_CHA:
	{
		std::string cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha || !pCCha->GetSubMap())
			break;
		switch (READ_CHAR(pkt))
		{
		case	1: // ????????????
		{
			std::string cszSrcName = READ_STRING(pkt);
			WPACKET WtPk = GETWPACKET();
			WRITE_CMD(WtPk, CMD_MM_GOTO_CHA);
			WRITE_LONG(WtPk, lSrcID);
			WRITE_STRING(WtPk, cszSrcName);
			WRITE_CHAR(WtPk, 2);
			if (pCCha->IsBoat())
				WRITE_CHAR(WtPk, 1);
			else
				WRITE_CHAR(WtPk, 0);
			WRITE_STRING(WtPk, pCCha->GetSubMap()->GetName());
			WRITE_LONG(WtPk, pCCha->GetPos().x);
			WRITE_LONG(WtPk, pCCha->GetPos().y);
			WRITE_LONG(WtPk, pCCha->GetSubMap()->GetCopyNO());
			WRITE_LONG(WtPk, lGatePlayerID);
			WRITE_LONG(WtPk, lGatePlayerAddr);
			WRITE_SHORT(WtPk, 1);
			pGate->SendData(WtPk);

			break;
		}
		case	2: // ???????????????????????
		{
			bool	bTarIsBoat = READ_CHAR(pkt) ? true : false;
			if (bTarIsBoat != pCCha->IsBoat()) // ???????????
				break;
			std::string cszMapName = READ_STRING(pkt);
			long	lPosX = READ_LONG(pkt);
			long	lPosY = READ_LONG(pkt);
			long	lCopyNO = READ_LONG(pkt);
			pCCha->SwitchMap(pCCha->GetSubMap(), cszMapName.c_str(), lPosX, lPosY, true, enumSWITCHMAP_CARRY, lCopyNO);

			break;
		}
		}

		break;
	}
	case	CMD_MM_KICK_CHA:
	{
		std::string cszChaName = READ_STRING(pkt);
		long	lTime = READ_LONG(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
		if (!pCCha || !pCCha->GetSubMap())
			break;

		KICKPLAYER(pCCha->GetPlayer(), lTime);
		g_pGameApp->GoOutGame(pCCha->GetPlayer(), true);

		break;
	}
	case	CMD_MM_NOTICE:
	{
		LocalNotice(READ_STRING(pkt).c_str());

		break;
	}
	case	CMD_MM_CHA_NOTICE:
	{
		std::string cszNotiCont = READ_STRING(pkt);
		std::string cszChaName = READ_STRING(pkt);

		if (cszChaName.empty())
			LocalNotice(cszNotiCont.c_str());
		else
		{
			CCharacter* pCCha = FindPlayerChaByName(cszChaName.c_str());
			if (!pCCha)
				break;

			WPACKET wpk = GETWPACKET();
			WRITE_CMD(wpk, CMD_MC_SYSINFO);
			WRITE_STRING(wpk, cszNotiCont);
			pCCha->ReflectINFof(pCCha, wpk);
		}

		break;
	}
    case    CMD_MM_DO_STRING:
    {
        const auto str = READ_STRING(pkt);

        //LG("DO_STRING", "%s\n", str);
        luaL_dostring(g_pLuaState, str.c_str());
        break;
    }
	case	CMD_MM_LOGIN:
	{
		g_pGameApp->AfterPlayerLogin(READ_STRING(pkt).c_str());

		break;
	}
	case	CMD_MM_GUILD_CHALL_PRIZEMONEY:
	{
		DWORD dwChaDBID = READ_LONG(pkt);
		DWORD dwMoney = READ_LONG(pkt);
		CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			/*pCha->AddMoney( "??", dwMoney );
			pCha->SystemNotice( "????????????%s????????????????????�?????%u????", pCha->GetGuildName(), dwMoney );
			ToLogService("common", "????????????{}??ID??{}????????????????????�?????{}????", pCha->GetGuildName(), pCha->GetGuildID(), dwMoney);*/
			pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
			ToLogService("common", "congratulate you leading consortia [{}] ID [{}] get win in consortia battle, gain bounty [{}]", pCha->GetGuildName(),
				pCha->GetGuildID(), dwMoney);
		}
		//else
		//{
		//	LG( "?????????", "??????????????????????DBID[%u],???[%u]\n", dwChaDBID, dwMoney );
		//}

		break;
	}
	case	CMD_MM_ADDCREDIT:
	{
		DWORD dwChaDBID = READ_LONG(pkt);
		long lCredit = READ_LONG(pkt);
		CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			pCha->SetCredit((long)pCha->GetCredit() + lCredit);
			pCha->SynAttr(enumATTRSYN_TASK);
		}
		break;
	}
	case	CMD_MM_STORE_BUY:
	{
		DWORD dwChaDBID = READ_LONG(pkt);
		long lComID = READ_LONG(pkt);
		//long lMoBean = READ_LONG(pkt);
		long lRplMoney = READ_LONG(pkt);
		CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			g_StoreSystem.Accept(pCha, lComID);
			//pCha->GetPlayer()->SetMoBean(lMoBean);
			pCha->GetPlayer()->SetRplMoney(lRplMoney);
		}
		break;
	}
	case CMD_MM_ADDMONEY:
	{
		DWORD dwChaID = READ_LONG(pkt);
		DWORD dwMoney = READ_LONG(pkt);

		CCharacter* pCha = NULL;
		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(dwChaID);
		if (pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}
		if (pCha)
		{
			pCha->AddMoney("??", dwMoney);
		}

		break;
	}
	case CMD_MM_AUCTION:
		//add by ALLEN 2007-10-19
	{

		DWORD dwChaID = READ_LONG(pkt);

		CCharacter* pCha = NULL;
		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(dwChaID);
		if (pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}
		if (pCha)
		{
			g_CParser.DoString("AuctionEnd", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END);
		}

		break;
	}
	}
}
void CGameApp::ProcessGroupBroadcast(unsigned short usCmd, GateServer* pGate, RPACKET pkt)
{


}
void CGameApp::ProcessGarner2Update(RPACKET pkt)//CMD_PM_GARNER2_UPDATE
{
		long chaid[6];
	CPlayer* pplay;
	//	CCharacter * pcha;
	chaid[0] = READ_LONG_R(pkt);
	chaid[1] = READ_LONG(pkt);
	chaid[2] = READ_LONG(pkt);
	chaid[3] = READ_LONG(pkt);
	chaid[4] = READ_LONG(pkt);
	chaid[5] = READ_LONG(pkt);
	if (0 != chaid[0])
	{
		pplay = FindPlayerByDBChaID(chaid[0]);
		if (pplay)
		{
			pplay->SetGarnerWiner(0);
		}
	}

	for (int i = 1;i < 6 && chaid[i];i++)
	{
		pplay = FindPlayerByDBChaID(chaid[0]);
		if (pplay)
		{
			pplay->SetGarnerWiner(i);
		}
	}
}
void CGameApp::ProcessDynMapEntry(GateServer* pGate, RPACKET pkt)
{
		std::string szTarMapN = READ_STRING(pkt);
	std::string szSrcMapN = READ_STRING(pkt);

	switch (READ_CHAR(pkt))
	{
	case	enumMAPENTRY_CREATE:
	{
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes)
		{
			break;
		}
		pCMap = pCMapRes->GetCopy();
		Long	lPosX = READ_LONG(pkt);
		Long	lPosY = READ_LONG(pkt);
		Short	sMapCopyNum = READ_SHORT(pkt);
		Short	sCopyPlyNum = READ_SHORT(pkt);
		CDynMapEntryCell	CEntryCell;
		CEntryCell.SetMapName(szTarMapN.c_str());
		CEntryCell.SetTMapName(szSrcMapN.c_str());
		CEntryCell.SetEntiPos(lPosX, lPosY);
		CDynMapEntryCell* pCEntry = g_CDMapEntry.Add(&CEntryCell);
		if (pCEntry)
		{
			pCEntry->SetCopyNum(sMapCopyNum);
			pCEntry->SetCopyPlyNum(sCopyPlyNum);
			string	strScript;
			std::string cszSctLine;
			Short	sLineNum = READ_SHORT_R(pkt);
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s[%u, %u]????????? %d\n", szSrcMapN, szTarMapN, lPosX, lPosY, sLineNum);
				ToLogService("common", "receive request to create entry??position {} --> {}[{}, {}]??script line {}", szSrcMapN.c_str(), szTarMapN.c_str(), lPosX, lPosY, sLineNum);
			while (--sLineNum >= 0)
			{
				cszSctLine = READ_STRING(pkt);
				strScript += cszSctLine;
				strScript += " ";
			}
			luaL_dostring(g_pLuaState, strScript.c_str());
			g_CParser.DoString("config_entry", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry, DOSTRING_PARAM_END);

			if (pCEntry->GetEntiID() > 0)
			{
				SItemGrid	SItemCont;
				SItemCont.sID = (Short)pCEntry->GetEntiID();
				SItemCont.sNum = 1;
				SItemCont.SetDBParam(-1, 0);
				SItemCont.chForgeLv = 0;
				SItemCont.SetInstAttrInvalid();
				CItem* pCItem = pCMap->ItemSpawn(&SItemCont, lPosX, lPosY, enumITEM_APPE_NATURAL, 0, g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(), -1, -1,
					pCEntry->GetEvent());
				if (pCItem)
				{
					pCItem->SetOnTick(0);
					pCEntry->SetEnti(pCItem);
				}
				else
				{
					if (g_cchLogMapEntry)
						//LG("??????????", "?????????????? %s --> %s[%u, %u]?????? %u ???????\n", szSrcMapN, szTarMapN, lPosX, lPosY, SItemCont.sID);
						ToLogService("common", "create entry failed??position {} --> {}[{}, {}]??item {} create failed", szSrcMapN.c_str(), szTarMapN.c_str(), lPosX, lPosY, SItemCont.sID);
					g_CDMapEntry.Del(pCEntry);
					break;
				}
			}
			// ??????????????
			WPACKET	wpk = GETWPACKET();
			WRITE_CMD(wpk, CMD_MT_MAPENTRY);
			WRITE_STRING(wpk, szSrcMapN);
			WRITE_STRING(wpk, szTarMapN);
			WRITE_CHAR(wpk, enumMAPENTRY_RETURN);
			WRITE_CHAR(wpk, enumMAPENTRYO_CREATE_SUC);

			BEGINGETGATE();
			GateServer* pGateServer = NULL;
			while (pGateServer = GETNEXTGATE())
			{
				pGateServer->SendData(wpk);
				break;
			}
			if (g_cchLogMapEntry)
				//LG("??????????", "?????????????? %s --> %s[%u, %u] \n", szSrcMapN, szTarMapN, lPosX, lPosY);
				ToLogService("common", "create entry success??position {} --> {}[{}, {}] ", szSrcMapN.c_str(), szTarMapN.c_str(), lPosX, lPosY);

			g_CParser.DoString("after_create_entry", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry, DOSTRING_PARAM_END);
		}
	}
	break;
	case	enumMAPENTRY_SUBPLAYER:
	{
		Short	sCopyNO = READ_SHORT(pkt);
		Short	sSubNum = READ_SHORT(pkt);

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (pCEntry)
		{
			CMapEntryCopyCell* pCCopyInfo = pCEntry->GetCopy(sCopyNO);
			if (pCCopyInfo)
				pCCopyInfo->AddCurPlyNum(-1 * sSubNum);
		}
	}
	break;
	case	enumMAPENTRY_SUBCOPY:
	{
		Short	sCopyNO = READ_SHORT(pkt);

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (pCEntry)
		{
			pCEntry->ReleaseCopy(sCopyNO);
		}
		// ????????????????
		WPACKET	wpk = GETWPACKET();
		WRITE_CMD(wpk, CMD_MT_MAPENTRY);
		WRITE_STRING(wpk, szSrcMapN);
		WRITE_STRING(wpk, szTarMapN);
		WRITE_CHAR(wpk, enumMAPENTRY_RETURN);
		WRITE_CHAR(wpk, enumMAPENTRYO_COPY_CLOSE_SUC);
		WRITE_SHORT(wpk, sCopyNO);

		BEGINGETGATE();
		GateServer* pGateServer = NULL;
		while (pGateServer = GETNEXTGATE())
		{
			pGateServer->SendData(wpk);
			break;
		}
	}
	break;
	case	enumMAPENTRY_DESTROY:
	{
		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN.c_str());
		if (g_cchLogMapEntry)
			//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
			ToLogService("common", "receive request to destroy entry??position {} --> {}", szSrcMapN.c_str(), szTarMapN.c_str());
		if (pCEntry)
		{
			string	strScript = "after_destroy_entry_";
			strScript += szSrcMapN;
			g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry, DOSTRING_PARAM_END);
			g_CDMapEntry.Del(pCEntry);

			// ?????????????
			WPACKET	wpk = GETWPACKET();
			WRITE_CMD(wpk, CMD_MT_MAPENTRY);
			WRITE_STRING(wpk, szSrcMapN);
			WRITE_STRING(wpk, szTarMapN);
			WRITE_CHAR(wpk, enumMAPENTRY_RETURN);
			WRITE_CHAR(wpk, enumMAPENTRYO_DESTROY_SUC);

			BEGINGETGATE();
			GateServer* pGateServer = NULL;
			while (pGateServer = GETNEXTGATE())
			{
				pGateServer->SendData(wpk);
				break;
			}
			if (g_cchLogMapEntry)
				//LG("??????????", "?????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "destroy entry success??position {} --> {}", szSrcMapN.c_str(), szTarMapN.c_str());
		}
	}
	break;
	case	enumMAPENTRY_COPYPARAM:
	{
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes)
			break;
		pCMap = pCMapRes->GetCopy(READ_SHORT(pkt));
		if (!pCMap)
			break;
		for (dbc::Char i = 0; i < defMAPCOPY_INFO_PARAM_NUM; i++)
			pCMap->SetInfoParam(i, READ_LONG(pkt));
	}
	break;
	case	enumMAPENTRY_COPYRUN:
	{
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN.c_str());
		if (!pCMapRes)
			break;
		pCMap = pCMapRes->GetCopy(READ_SHORT(pkt));
		if (!pCMap)
			break;

		Char	chType = READ_CHAR(pkt);
		Long	lVal = READ_LONG(pkt);
		pCMapRes->SetCopyStartCondition(chType, lVal);
	}
	break;
	case	enumMAPENTRY_RETURN:
	{
		CMapRes* pCMap = FindMapByName(szTarMapN.c_str(), true);
		if (!pCMap)
			break;
		switch (READ_CHAR(pkt))
		{
		case	enumMAPENTRYO_CREATE_SUC:
		{
			pCMap->CheckEntryState(enumMAPENTRY_STATE_OPEN);
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "receive entry create success ??position {} --> {}", szSrcMapN.c_str(), szTarMapN.c_str());
		}
		break;
		case	enumMAPENTRYO_DESTROY_SUC:
		{
			if (g_cchLogMapEntry)
				//LG("??????????", "???????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				ToLogService("common", "receive entry destroy success??position {} --> {}", szSrcMapN.c_str(), szTarMapN.c_str());
			pCMap->CheckEntryState(enumMAPENTRY_STATE_CLOSE_SUC);
		}
		break;
		case	enumMAPENTRYO_COPY_CLOSE_SUC:
		{
			pCMap->CopyClose(READ_SHORT(pkt));
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