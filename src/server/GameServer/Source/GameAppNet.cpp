#include "Stdafx.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "MapEntry.h"
#include "CharTrade.h"
#include "GameDB.h"

using namespace std;

extern std::string g_strLogName;

void CGameApp::ProcessNetMsg(int nMsgType, GateServer* pGate, RPACKET pkt)
{
	T_B
		switch (nMsgType)
		{
		case NETMSG_GATE_CONNECTED: // ??????Gate
		{
			LG("Connect", "Exec OnGateConnected()\n");
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
	T_E
}


// ??Gate??????????????
void CGameApp::OnGateConnected(GateServer* pGate, RPACKET pkt)
{
	T_B
	// ??GateServer???GameServer
	WPACKET	wpk = GETWPACKET();
	WRITE_CMD(wpk, CMD_MT_LOGIN);
	WRITE_STRING(wpk, GETGMSVRNAME());
	WRITE_STRING(wpk, g_pGameApp->m_strMapNameList.c_str());

	LG("Connect", "[%s]\n", g_pGameApp->m_strMapNameList.c_str());

	pGate->SendData(wpk);
	T_E
}

// ??Gate???????????????
void CGameApp::OnGateDisconnect(GateServer* pGate, RPACKET pkt)
{
	T_B
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
	T_E
}

// ?????????????
void CGameApp::ProcessPacket(GateServer* pGate, RPACKET pkt)
{
	T_B
		CPlayer* l_player = nullptr;
	uShort cmd = READ_CMD(pkt);

	switch (cmd)
	{
	case CMD_TM_LOGIN_ACK:
	{
		short	sErrCode;
		if (sErrCode = READ_SHORT(pkt))
		{
			/*LG("GameLogin", "?? GateServer: %s:%d???[%s], ?????[%s]\n",
				pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(sErrCode),
				g_pGameApp->m_strMapNameList.c_str());*/
			LG("GameLogin", "enter GateServer: %s:%d failed [%s], register map[%s]\n",
				pGate->GetIP().c_str(), pGate->GetPort(), g_GameGateConnError(sErrCode),
				g_pGameApp->m_strMapNameList.c_str());
			DISCONNECT(pGate->GetDataSock());
		}
		else
		{
			pGate->GetName() = READ_STRING(pkt);
			if (!strcmp(pGate->GetName().c_str(), ""))
			{
				/*LG("GameLogin", "?? GateServer: [%s:%d]??? ????????????????????????????\n",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());*/
				LG("GameLogin", "entry GateServer: [%s:%d]success but do not get his name??so disconnection and entry again\n",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());

				DISCONNECT(pGate->GetDataSock());
			}
			else
			{
				/*LG("GameLogin", "?? GateServer: %s [%s:%d]??? [MapName:%s]\n",
					pGate->GetName().c_str(), pGate->GetIP().c_str(), pGate->GetPort(),
					g_pGameApp->m_strMapNameList.c_str());*/
				LG("GameLogin", "entry GateServer: %s [%s:%d]success [MapName:%s]\n",
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

	//		LG("GameLogin", "Gate %s Ip %s %d deleted\r\n", pGate->GetName(), pGate->GetIP(), reason);

	//		pGate->Invalid();

	//		break;
	//	}
	//	//End

	case CMD_TM_ENTERMAP:
	{
		uLong l_actid = READ_LONG(pkt);
		const char* pszPassword = READ_STRING(pkt);
		if (pszPassword == NULL)
			break;
		uLong l_dbid = READ_LONG(pkt);
		uLong l_worldid = READ_LONG(pkt);
		cChar* l_map = READ_STRING(pkt);
		if (l_map == NULL)
			break;
		Long lMapCpyNO = READ_LONG(pkt);
		uLong l_x = READ_LONG(pkt);
		uLong l_y = READ_LONG(pkt);
		char chLogin = READ_CHAR(pkt);
		short swiner = READ_SHORT_R(pkt);
		uLong l_gtaddr = READ_LONG_R(pkt);

		LG("enter_map", "start entry map atorID = %d enter--------------------------\n", l_dbid);

		l_player = CreateGamePlayer(pszPassword, l_dbid, l_worldid, l_map, chLogin == 0 ? 0 : 1);
		if (!l_player)
		{
			WPACKET pkret = GETWPACKET();
			WRITE_CMD(pkret, CMD_MC_ENTERMAP);
			WRITE_SHORT(pkret, ERR_MC_ENTER_ERROR);
			WRITE_LONG(pkret, l_dbid);
			WRITE_LONG(pkret, l_gtaddr);
			WRITE_SHORT(pkret, 1);
			pGate->SendData(pkret);
			LG("enter_map", "when create new palyer ID = %u assign memory failed \n", l_dbid);
			return;
		}
		l_player->SetActLoginID(l_actid);
		l_player->SetGarnerWiner(swiner);
		l_player->GetLifeSkillinfo() = "";
		l_player->SetInLifeSkill(false);

		if (!chLogin)
			l_player->MisLogin();

		ADDPLAYER(l_player, pGate, l_gtaddr);
		l_player->OnLogin();

		CCharacter* pCCha = l_player->GetMainCha();
		if (pCCha->Cmd_EnterMap(l_map, lMapCpyNO, l_x, l_y, chLogin))
		{
			l_player->MisEnterMap();
			if (chLogin == 0)
			{
				NoticePlayerLogin(l_player);
			}
		}

		LG("enter_map", "end up entry map  [%s]================\n\n", pCCha->GetLogName());
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
				//LG("error", "?????ID: %u, ????????????:%x, gate:%x,cmd=%d, ?????(%d).\n", l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr,cmd, l_player->IsValidFlag());
				LG("error", "DB ID: %u, address not matching??local :%x, gate:%x,cmd=%d, validity(%d).\n", l_player->GetDBChaId(), l_player->GetGateAddr(), l_gateaddr, cmd, l_player->IsValidFlag());
				break;
			}
		}
		catch (...)
		{
			//LG("error", "===========================??Gate?????????????%p,cmd =%d\n", l_player, cmd);
			LG("error", "===========================from Gate plyaer's address error %p,cmd =%d\n", l_player, cmd);
			break;
		}
		if (!l_player->IsValid())
		{
			//LG("enter_map", "???????????\n");
			LG("enter_map", "this palyer already impotence\n");
			break;
		}
		if (l_player->GetMainCha()->GetPlayer() != l_player)
		{
			//LG("error", "????player????????????%s??Gate???[????%p, ????%p]????cmd=%u\n", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
			LG("error", "two player not matching??character name??%s??Gate address [local %p, guest %p]????cmd=%u\n", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
		}
		LG("enter_map", "start leave map--------\n");

		char chOffLine = READ_CHAR(pkt); // ???????(0)
		LG("enter_map", "Delete Player [%s]\n", l_player->GetMainCha()->GetLogName());

		char	szLogName[512];
		strncpy(szLogName, l_player->GetMainCha()->GetLogName(), 512 - 1);
		//LG("OutMap", "%s????????\n", szLogName);

		GoOutGame(l_player, !chOffLine, false);

		//LG("enter_map", "?????????========\n\n");
		LG("enter_map", "end and leave the map========\n\n");

		//LG("OutMap", "%s?????\n", szLogName);

		break;
	}
	case CMD_PM_SAY2ALL:
	{
		uLong ulChaID = pkt.ReadLong();
		cChar* szContent = pkt.ReadString();
		long lChatMoney = pkt.ReadLong();

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney))
			{
				//pCha->SystemNotice("??????????,?????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00007));

				WPacket l_wpk = GETWPACKET();
				WRITE_CMD(l_wpk, CMD_MP_SAY2ALL);
				WRITE_CHAR(l_wpk, 0);
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("?????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00006), lChatMoney);

			WPacket l_wpk = GETWPACKET();
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
		uLong ulChaID = pkt.ReadLong();
		cChar* szContent = pkt.ReadString();
		long lChatMoney = pkt.ReadLong();

		CPlayer* pPlayer = g_pGameApp->GetPlayerByDBID(ulChaID);
		if (pPlayer)
		{
			CCharacter* pCha = pPlayer->GetMainCha();
			if (!pCha->HasMoney(lChatMoney))
			{
				//pCha->SystemNotice("??????????,????????????????!");
				pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00005));

				WPacket l_wpk = GETWPACKET();
				WRITE_CMD(l_wpk, CMD_MP_SAY2TRADE);
				WRITE_CHAR(l_wpk, 0);
				pCha->ReflectINFof(pCha, l_wpk);

				break;
			}
			pCha->setAttr(ATTR_GD, (pCha->getAttr(ATTR_GD) - lChatMoney));
			pCha->SynAttr(enumATTRSYN_TASK);
			//pCha->SystemNotice("????????????????,??????%ld?????!", lChatMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00004), lChatMoney);

			WPacket l_wpk = GETWPACKET();
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
		uLong ulChaID = pkt.ReadLong();
		uLong ulTime = pkt.ReadLong();

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
						/*LG("error", "?????ID:%u, ????????????:%u, gate:%u,cmd=%d, ?????(%d)\n", l_player->GetDBChaId(), l_player->GetGateAddr(),
							l_gateaddr,cmd, l_player->IsValidFlag() );*/
						LG("error", "DB ID:%u, address not matching??local :%u, gate:%u,cmd=%d, validity (%d)\n", l_player->GetDBChaId(), l_player->GetGateAddr(),
							l_gateaddr, cmd, l_player->IsValidFlag());
						break;
					}
				}
				catch (...)
				{
					//LG("error", "===========================??Gate?????????????%p,cmd =%d\n", l_player, cmd);
					LG("error", "===========================Player address error that come from Gate %p,cmd =%d\n", l_player, cmd);
					break;
				}
				if (!l_player->IsValid())
					break;
				if (l_player->GetMainCha()->GetPlayer() != l_player)
				{
					//LG("error", "????player????????????%s??Gate???[????%p, ????%p]????cmd=%u\n", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
					LG("error", "two player not matching??character name??%s??Gate address [local %p, guest %p]????cmd=%u\n", l_player->GetMainCha()->GetLogName(), l_player->GetMainCha()->GetPlayer(), l_player, cmd);
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
					//LG("error", "???CMD_CM_BASE?????[%d]?, ??????pCCha???\n", cmd);
					LG("error", "when receive CMD_CM_BASE message[%d], find character pCCha is null\n", cmd);
				}
				break;
			}
		}
	}
	T_E
}

// ??????????????
void CGameApp::ProcessGuildChallMoney(GateServer* pGate, RPACKET pkt)
{
	T_B
		DWORD dwChaDBID = READ_LONG(pkt);
	DWORD dwMoney = READ_LONG(pkt);
	const char* pszGuild1 = READ_STRING(pkt);
	const char* pszGuild2 = READ_STRING(pkt);

	//	2007-8-4	yangyinyu	change	begin!	//	?????????????????????????????????
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetMainCha();
		//pCha->AddMoney( "??", dwMoney );
		pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
		/*pCha->SystemNotice( "??????????%s?????????%s????????????????(%u)????????????\n", pszGuild1, pszGuild2, dwMoney );
		LG( "?????????", "??%s??????????%s?????????%s????????????????(%u)????????????\n", pCha->GetGuildName(), pszGuild1, pszGuild2, dwMoney );*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00009), pszGuild1, pszGuild2, dwMoney);
		LG("challenge consortia result", "??%s??bidder and consortia??%s??battle was consortia??%s??replace??your consortia gold (%u)had back to you??\n", pCha->GetGuildName(), pszGuild1, pszGuild2, dwMoney);
	}
	else
	{
		//LG( "?????????", "?????????????????????DBID[%u],???[%u].\n", dwChaDBID, dwMoney );
		LG("challenge consortia result", "not find deacon information finger??cannot back gold DBID[%u],how much money[%u].\n", dwChaDBID, dwMoney);
	}
	T_E
}

void CGameApp::ProcessGuildChallPrizeMoney(GateServer* pGate, RPACKET pkt)
{
	T_B
		DWORD dwChaDBID = READ_LONG(pkt);
	DWORD dwMoney = READ_LONG(pkt);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetMainCha();
		pCha->AddMoney("??", dwMoney);
		/*pCha->SystemNotice( "????????????%s?????????????????????????%u????", pCha->GetGuildName(), dwMoney );
		LG( "?????????", "????????????%s?????????????????????????%u????", pCha->GetGuildName(), dwMoney );*/
		pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
		LG("challenge consortia result", "congratulate you have leading the consortia??%s??get win in consortia battle??gain bounty??%u????", pCha->GetGuildName(), dwMoney);
	}
	else
	{
		//LG( "?????????", "??????????????????????DBID[%u],???[%u]", dwChaDBID, dwMoney );
		LG("challenge consortia result", "cannot find deacon information finger??cannot hortation DBID[%u],how much money[%u]", dwChaDBID, dwMoney);
	}
	T_E
}

// ???????????
void CGameApp::ProcessGuildMsg(GateServer* pGate, RPACKET pkt)
{
	T_B
		DWORD dwChaDBID = READ_LONG(pkt);
	CPlayer* pPlayer = GetPlayerByDBID(dwChaDBID);
	if (pPlayer)
	{
		CCharacter* pCha = pPlayer->GetCtrlCha();
		DWORD dwGuildID = READ_LONG(pkt);
		DWORD dwLeaderID = READ_LONG(pkt);
		const char* pszGuildName = READ_STRING(pkt);
		const char* pszGuildMotto = READ_STRING(pkt);
		pCha->SetGuildName(pszGuildName);
		pCha->SetGuildMotto(pszGuildMotto);
		pCha->SyncGuildInfo();
	}
	T_E
}

// ?????????????
void CGameApp::ProcessTeamMsg(GateServer* pGate, RPACKET pkt)
{
	T_B
		//LG("team", "?????????????\n");

		char cTeamMsgType = READ_CHAR(pkt);

	switch (cTeamMsgType)
	{
	case TEAM_MSG_ADD: {	/*LG("team", "?????? [????] ???\n");*/ break; }
	case TEAM_MSG_LEAVE: {	/*LG("team", "?????? [??????] ???\n");*/ break; }
	case TEAM_MSG_UPDATE: {	/*LG("team", "?????? [??????] ???\n");*/ break; }
	default:
		//LG("team", "????????Team??? [%d]\n", cTeamMsgType);
		return;
	}

	char cMemberCnt = READ_CHAR(pkt);
	//LG("team", "????????????[%d]\n", cMemberCnt); // ?????????? < 2????GroupServer????????????????.

	uplayer Team[MAX_TEAM_MEMBER];
	CPlayer* PlayerList[MAX_TEAM_MEMBER];
	bool	CanSeenO[MAX_TEAM_MEMBER][2];
	bool	CanSeenN[MAX_TEAM_MEMBER][2];

	// ????????????????????Player
	for (char i = 0; i < cMemberCnt; i++)
	{
		const char* pszGateName = READ_STRING(pkt);
		DWORD dwGateAddr = READ_LONG(pkt);
		DWORD dwChaDBID = READ_LONG(pkt);
		Team[i].Init(pszGateName, dwGateAddr, dwChaDBID);
		if (!Team[i].pGate)
		{
			LG("team", "GameServer can't find matched Gate??%s, addr = 0x%X, chaid = %d.\n", pszGateName, dwGateAddr, dwChaDBID);
			LG("team", "\tGameServer all Gate:\n");
			BEGINGETGATE();
			GateServer* pGateServer;
			while (pGateServer = GETNEXTGATE())
			{
				LG("team", "\t%s\n", pGateServer->GetName().c_str());
			}
		}

		PlayerList[i] = GetPlayerByDBID(dwChaDBID);

		//LG("team", "???: %s, %d %d ????Gate [%s]\n", PlayerList[i]!=NULL ? PlayerList[i]->GetCtrlCha()->GetLogName():"(????????Server!)", dwChaDBID, dwGateAddr, pszGateName);
	}

	//RefreshTeamEyeshot(PlayerList, cMemberCnt, cTeamMsgType);
	CheckSeeWithTeamChange(CanSeenO, PlayerList, cMemberCnt);
	//if(PlayerList[0]==NULL)
	//{
	//	LG("team", "????????game server????\n");
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

				//if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // 
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
			printf("\nException handling: newPly invalid\ncMemberCnt=%d", cMemberCnt);
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

	//if(nLeftMember==1) LG("team", "nLeftMember==1, ??????!\n");

	//LG("team", "??????????????\n\n");
	T_E
}

// ????????????
void CGameApp::CheckSeeWithTeamChange(bool CanSeen[][2], CPlayer** pCPlayerList, char chMemberCnt)
{
	T_B
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
	T_E
}

// ??????????????????????????????
void CGameApp::RefreshTeamEyeshot(bool CanSeenOld[][2], bool CanSeenNew[][2], CPlayer** pCPlayerList, char chMemberCnt, char chRefType)
{
	T_B
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
	T_E
}

// ???????????????????
void CGameApp::RefreshTeamEyeshot(CPlayer** pCPlayerList, char chMemberCnt, char chRefType)
{
	T_B
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
	T_E
}

BOOL CGameApp::AddVolunteer(CCharacter* pCha)
{
	T_B
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
	T_E
}

BOOL CGameApp::DelVolunteer(CCharacter* pCha)
{
	T_B
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
	T_E
}

int CGameApp::GetVolNum()
{
	T_B
		return (int)m_vecVolunteerList.size();
	T_E
}

SVolunteer* CGameApp::GetVolInfo(int nIndex)
{
	T_B
		if (nIndex < 0 || nIndex >= (int)m_vecVolunteerList.size())
			return NULL;

	return &m_vecVolunteerList[nIndex];
	T_E
}

SVolunteer* CGameApp::FindVolunteer(const char* szName)
{
	T_B
		vector<SVolunteer>::iterator it;
	for (it = m_vecVolunteerList.begin(); it != m_vecVolunteerList.end(); it++)
	{
		if (!strcmp((*it).szName, szName))
		{
			return (SVolunteer*)&(*it);
		}
	}
	return NULL;
	T_E
}

void CGameApp::ProcessInterGameMsg(unsigned short usCmd, GateServer* pGate, RPACKET pkt)
{
	T_B
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
		cChar* pszMotto = READ_STRING(pkt);
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
						LG("player list error", "player number:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
						break;
					}
					pCha = pCPlayer->GetMainCha();
					if (!pCha)
						continue;
					if (pCha->GetGuildID() == l_gldid) // ??????
					{
						pCha->SetGuildMotto(pszMotto);
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
						LG("player list error", "player number:%u, %s\n", GETPLAYERCOUNT(pGateServer), "ProcessInterGameMsg::CMD_MM_GUILD_DISBAND");
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
			cChar* l_gldname = READ_STRING(pkt);
			pCha->guildPermission = 0;
			pCha->SetGuildID(0);			//??????ID
			pCha->SetGuildState(0);
			pCha->SetGuildName("");
			pCha->SetGuildMotto("");
			// pCha->SystemNotice("?????????????????[%s].",l_gldname);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00014), l_gldname);
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
			pCha->SetGuildID(READ_LONG(pkt));				//??????ID
			pCha->SetGuildState(0);	//??????Type
			cChar* l_gldname = READ_STRING(pkt);
			pCha->SetGuildName(l_gldname);
			cChar* l_gldmotto = READ_STRING(pkt);
			pCha->SetGuildMotto(l_gldmotto);
			//  pCha->SystemNotice("????????????????[%s].",l_gldname);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00015), l_gldname);
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
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00016), READ_STRING(pkt));
		}
	}
	break;
	case	CMD_MM_QUERY_CHAPING:
	{
		const char* cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
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
		const char* cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
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
		const char* cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
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
		const char* cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
		if (!pCCha || !pCCha->GetSubMap())
			break;
		bool	bTarIsBoat = READ_CHAR(pkt) ? true : false;
		if (bTarIsBoat != pCCha->IsBoat()) // ???????????
			break;
		const char* cszMapName = READ_STRING(pkt);
		long	lPosX = READ_LONG(pkt);
		long	lPosY = READ_LONG(pkt);
		long	lCopyNO = READ_LONG(pkt);
		pCCha->SwitchMap(pCCha->GetSubMap(), cszMapName, lPosX, lPosY, true, enumSWITCHMAP_CARRY, lCopyNO);

		break;
	}
	case	CMD_MM_GOTO_CHA:
	{
		const char* cszChaName = READ_STRING(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
		if (!pCCha || !pCCha->GetSubMap())
			break;
		switch (READ_CHAR(pkt))
		{
		case	1: // ????????????
		{
			const char* cszSrcName = READ_STRING(pkt);
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
			const char* cszMapName = READ_STRING(pkt);
			long	lPosX = READ_LONG(pkt);
			long	lPosY = READ_LONG(pkt);
			long	lCopyNO = READ_LONG(pkt);
			pCCha->SwitchMap(pCCha->GetSubMap(), cszMapName, lPosX, lPosY, true, enumSWITCHMAP_CARRY, lCopyNO);

			break;
		}
		}

		break;
	}
	case	CMD_MM_KICK_CHA:
	{
		const char* cszChaName = READ_STRING(pkt);
		long	lTime = READ_LONG(pkt);
		CCharacter* pCCha = FindPlayerChaByName(cszChaName);
		if (!pCCha || !pCCha->GetSubMap())
			break;

		KICKPLAYER(pCCha->GetPlayer(), lTime);
		g_pGameApp->GoOutGame(pCCha->GetPlayer(), true);

		break;
	}
	case	CMD_MM_NOTICE:
	{
		LocalNotice(READ_STRING(pkt));

		break;
	}
	case	CMD_MM_CHA_NOTICE:
	{
		const char* cszNotiCont = READ_STRING(pkt);
		const char* cszChaName = READ_STRING(pkt);

		if (!strcmp(cszChaName, ""))
			LocalNotice(cszNotiCont);
		else
		{
			CCharacter* pCCha = FindPlayerChaByName(cszChaName);
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
        luaL_dostring(g_pLuaState, str);
        break;
    }
	case	CMD_MM_LOGIN:
	{
		g_pGameApp->AfterPlayerLogin(READ_STRING(pkt));

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
			pCha->SystemNotice( "????????????%s?????????????????????????%u????", pCha->GetGuildName(), dwMoney );
			LG( "?????????", "????????????%s??ID??%u?????????????????????????%u????\n", pCha->GetGuildName(),
				pCha->GetGuildID(), dwMoney );*/
			pCha->AddMoney(RES_STRING(GM_GAMEAPPNET_CPP_00017), dwMoney);
			pCha->SystemNotice(RES_STRING(GM_GAMEAPPNET_CPP_00010), pCha->GetGuildName(), dwMoney);
			LG("challenge consortia result", "congratulate you leading consortia??%s??ID??%u??get win in consortia battle??gain bounty??%u????\n", pCha->GetGuildName(),
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
	T_E
}
void CGameApp::ProcessGroupBroadcast(unsigned short usCmd, GateServer* pGate, RPACKET pkt)
{
	T_B


		T_E
}
void CGameApp::ProcessGarner2Update(RPACKET pkt)//CMD_PM_GARNER2_UPDATE
{
	T_B
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
	T_E
}
void CGameApp::ProcessDynMapEntry(GateServer* pGate, RPACKET pkt)
{
	T_B
		cChar* szTarMapN = READ_STRING(pkt);
	cChar* szSrcMapN = READ_STRING(pkt);

	switch (READ_CHAR(pkt))
	{
	case	enumMAPENTRY_CREATE:
	{
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN);
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
		CEntryCell.SetMapName(szTarMapN);
		CEntryCell.SetTMapName(szSrcMapN);
		CEntryCell.SetEntiPos(lPosX, lPosY);
		CDynMapEntryCell* pCEntry = g_CDMapEntry.Add(&CEntryCell);
		if (pCEntry)
		{
			pCEntry->SetCopyNum(sMapCopyNum);
			pCEntry->SetCopyPlyNum(sCopyPlyNum);
			string	strScript;
			cChar* cszSctLine;
			Short	sLineNum = READ_SHORT_R(pkt);
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s[%u, %u]????????? %d\n", szSrcMapN, szTarMapN, lPosX, lPosY, sLineNum);
				LG("map entry flow", "receive request to create entry??position %s --> %s[%u, %u]??script line %d\n", szSrcMapN, szTarMapN, lPosX, lPosY, sLineNum);
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
						LG("map entry flow", "create entry failed??position %s --> %s[%u, %u]??item %u create failed\n", szSrcMapN, szTarMapN, lPosX, lPosY, SItemCont.sID);
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
				LG("map entry flow", "create entry success??position %s --> %s[%u, %u] \n", szSrcMapN, szTarMapN, lPosX, lPosY);

			g_CParser.DoString("after_create_entry", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCEntry, DOSTRING_PARAM_END);
		}
	}
	break;
	case	enumMAPENTRY_SUBPLAYER:
	{
		Short	sCopyNO = READ_SHORT(pkt);
		Short	sSubNum = READ_SHORT(pkt);

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN);
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

		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN);
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
		CDynMapEntryCell* pCEntry = g_CDMapEntry.GetEntry(szSrcMapN);
		if (g_cchLogMapEntry)
			//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
			LG("map entry flow", "receive request to destroy entry??position %s --> %s\n", szSrcMapN, szTarMapN);
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
				LG("map entry flow", "destroy entry success??position %s --> %s\n", szSrcMapN, szTarMapN);
		}
	}
	break;
	case	enumMAPENTRY_COPYPARAM:
	{
		CMapRes* pCMapRes;
		SubMap* pCMap;
		pCMapRes = FindMapByName(szTarMapN);
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
		pCMapRes = FindMapByName(szTarMapN);
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
		CMapRes* pCMap = FindMapByName(szTarMapN, true);
		if (!pCMap)
			break;
		switch (READ_CHAR(pkt))
		{
		case	enumMAPENTRYO_CREATE_SUC:
		{
			pCMap->CheckEntryState(enumMAPENTRY_STATE_OPEN);
			if (g_cchLogMapEntry)
				//LG("??????????", "????????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				LG("map entry flow", "receive entry create success ??position %s --> %s\n", szSrcMapN, szTarMapN);
		}
		break;
		case	enumMAPENTRYO_DESTROY_SUC:
		{
			if (g_cchLogMapEntry)
				//LG("??????????", "???????????????? %s --> %s\n", szSrcMapN, szTarMapN);
				LG("map entry flow", "receive entry destroy success??position %s --> %s\n", szSrcMapN, szTarMapN);
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
	T_E
}
