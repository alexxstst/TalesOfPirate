#include "stdafx.h"
#include "SubMap.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "CharTrade.h"
#include "NPC.h"
#include "LuaAPI.h"
#include "WorldEudemon.h"
#include "Player.h"
#include "LevelRecord.h"
#include "CharForge.h"
#include "HairRecord.h"
#include "gamedb.h"

#include "Birthplace.h"
#include "CharBoat.h"
#include "Guild.h"
#include "CharStall.h"

#include "Auction.h"
#include "CommandMessages.h"

_DBC_USING

extern std::string g_strLogName;

//----------------------------------------------------------
//                    
//----------------------------------------------------------
void CCharacter::ProcessPacket(unsigned short usCmd, net::RPacket& pk) {
	switch (usCmd) {
	case CMD_CM_RANK: {
		DWORD COOLDOWN = GetTickCount();
		if (ShowRankColD > COOLDOWN) {
			BickerNotice("Please Calm Down Don't Spam! ");
			return;
		}
		game_db.ShowExpRank(GetPlyMainCha(), 50);
		break;
	}
	case CMD_CM_STALLSEARCH: {
		net::msg::CmStallSearchMessage msg;
		net::msg::deserialize(pk, msg);
		g_StallSystem.SearchItem(*this, static_cast<Long>(msg.itemId));
		break;
	}
	case CMD_PM_GUILDBANK: {
		net::msg::PmGuildBankMessage gbMsg;
		net::msg::deserialize(pk, gbMsg);
		Handle_GuildBankCmd(gbMsg);
		break;
	}

	case CMD_PM_PUSHTOGUILDBANK: {
		std::string strItem = pk.ReadString();
		Handle_PushToGuildBank(strItem);
		break;
	}

	case CMD_CM_PING: {
		net::msg::CmPingResponseMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_Ping(msg);
		break;
	}
	case CMD_CM_CHECK_PING: {
		DWORD dwPing = GetTickCount() - m_dwPingSendTick;
		m_dwPing = dwPing;
		SendPreMoveTime();
		break;
	}
	case CMD_CM_CANCELEXIT: {
		CancelExit();
	}
	break;
	case CMD_CM_BEGINACTION: {
		net::msg::CmBeginActionMessage actionMsg;
		net::msg::deserialize(pk, actionMsg);
		uLong ulWorldID = static_cast<uLong>(actionMsg.worldId);

		if (GetPlayer()) {
			if (GetPlayer()->GetCtrlCha() && ulWorldID == GetPlayer()->GetCtrlCha()->GetID())
				GetPlayer()->GetCtrlCha()->BeginAction(actionMsg);
			else if (GetPlayer()->GetMainCha() && ulWorldID == GetPlayer()->GetMainCha()->GetID())
				GetPlayer()->GetMainCha()->BeginAction(actionMsg);
		}
		break;
	}
	case CMD_CM_ENDACTION: {
		EndAction();

		break;
	}
	case CMD_CM_DIE_RETURN: {
		net::msg::CmDieReturnMessage msg;
		net::msg::deserialize(pk, msg);
		m_chSelRelive = static_cast<Char>(msg.reliveType);
		GetPlyMainCha()->ResetChaRelive();
		if (m_chSelRelive == enumEPLAYER_RELIVE_NORIGIN)
			SetRelive(enumEPLAYER_RELIVE_ORIGIN, 0);
		break;
	}
	case CMD_CM_SAY: {
		net::msg::CmSayMessage sayMsg;
		net::msg::deserialize(pk, sayMsg);
		Handle_Say(sayMsg);
		break;
	}
	case CMD_CM_REQUESTTALK:
	case CMD_CM_REQUESTTRADE: {
		if (GetTradeData() || GetBoat() || GetStallData() || !GetActControl(enumACTCONTROL_TALKTO_NPC) || m_CKitbag.
			IsLock() || !GetActControl(enumACTCONTROL_ITEM_OPT)) {
			return;
		}

		uLong ulID = pk.ReadInt64();
		Handle_RequestTalk(ulID, pk);
	}
	break;
	//daily buff request to open ui
	case CMD_CM_DailyBuffRequest: {
		Handle_DailyBuffRequest();
		break;
	}
	case CMD_CM_MISLOG: {
		MisLog();
	}
	break;
	case CMD_CM_MISLOGINFO: {
		net::msg::CmMisLogInfoMessage msg;
		net::msg::deserialize(pk, msg);
		MisLogInfo(static_cast<WORD>(msg.id));
	}
	break;
	case CMD_CM_MISLOG_CLEAR: {
		net::msg::CmMisLogClearMessage msg;
		net::msg::deserialize(pk, msg);
		MisLogClear(static_cast<WORD>(msg.id));
	}
	break;
	case CMD_CM_FORGE: {
		net::msg::CmForgeItemMessage msg;
		net::msg::deserialize(pk, msg);
		g_ForgeSystem.ForgeItem(*this, static_cast<BYTE>(msg.index));
	}
	break;
	case CMD_CM_CHARTRADE_REQUEST: {
		net::msg::CmRequestTradeMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.Request(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId));
	}
	break;
	case CMD_CM_CHARTRADE_ACCEPT: {
		net::msg::CmAcceptTradeMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.Accept(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId));
	}
	break;
	case CMD_CM_CHARTRADE_REJECT: {
	}
	break;
	case CMD_CM_CHARTRADE_CANCEL: {
		net::msg::CmCancelTradeMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.Cancel(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId));
	}
	break;
	case CMD_CM_CHARTRADE_ITEM: {
		net::msg::CmAddItemMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.AddItem(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId),
							  static_cast<BYTE>(msg.opType), static_cast<BYTE>(msg.index),
							  static_cast<BYTE>(msg.itemIndex), static_cast<BYTE>(msg.count));
	}
	break;
	case CMD_CM_CHARTRADE_MONEY: {
		net::msg::CmAddMoneyMessage msg;
		net::msg::deserialize(pk, msg);
		BYTE byType = static_cast<BYTE>(msg.type);
		DWORD dwCharID = static_cast<DWORD>(msg.charId);
		BYTE byOpType = static_cast<BYTE>(msg.opType);
		BYTE currency = static_cast<BYTE>(msg.isImp);
		DWORD dwMondy = static_cast<DWORD>(msg.money);

		if (currency == 0) {
			//gold
			g_TradeSystem.AddMoney(byType, *this, dwCharID, byOpType, dwMondy);
		}
		else if (currency == 1) {
			//IMPS
			g_TradeSystem.AddIMP(byType, *this, dwCharID, byOpType, dwMondy);
		}
	}
	break;
	case CMD_CM_CHARTRADE_VALIDATEDATA: {
		net::msg::CmValidateTradeDataMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.ValidateItemData(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId));
	}
	break;
	case CMD_CM_CHARTRADE_VALIDATE: {
		net::msg::CmValidateTradeMessage msg;
		net::msg::deserialize(pk, msg);
		g_TradeSystem.ValidateTrade(static_cast<BYTE>(msg.type), *this, static_cast<DWORD>(msg.charId));
	}
	break;
	case CMD_CM_CREATE_BOAT: {
		net::msg::CmCreateBoatMessage boatMsg;
		net::msg::deserialize(pk, boatMsg);
		g_CharBoat.MakeBoat(*this, boatMsg);
	}
	break;
	case CMD_CM_UPDATEBOAT_PART: {
		net::msg::CmUpdateBoatMessage boatMsg;
		net::msg::deserialize(pk, boatMsg);
		g_CharBoat.Update(*this, boatMsg);
	}
	break;
	case CMD_CM_BOAT_GETINFO: {
		if (GetPlayer()->IsLuanchOut()) {
			g_CharBoat.GetBoatInfo(*this, GetPlayer()->GetLuanchID());
		}
		else {
			//SystemNotice( "" );
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00003));
		}
	}
	break;
	case CMD_CM_BOAT_CANCEL: {
		g_CharBoat.Cancel(*this);
	}
	break;
	case CMD_CM_BOAT_LUANCH: {
		net::msg::CmBoatLaunchMessage msg;
		net::msg::deserialize(pk, msg);
		DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
		CCharacter* pCha = m_submap->FindCharacter(dwNpcID, GetShape().centre);
		if (pCha == NULL) {
			break;
		}
		else if (GetPlayer()->GetBankNpc()) {
			break;
		}
		else if (auto ret = g_luaAPI.CallR<int>("IsSailNpc", static_cast<CCharacter*>(this), pCha)) {
			if (!ret.value()) {
				break;
			}
		}

		BoatSelLuanch(static_cast<BYTE>(msg.index));
	}
	break;
	case CMD_CM_BOAT_SELECT: {
		net::msg::CmSelectBoatListMessage msg;
		net::msg::deserialize(pk, msg);
		DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
		CCharacter* pCha = m_submap->FindCharacter(dwNpcID, GetShape().centre);
		if (pCha == NULL) {
			break;
		}
		if (auto ret = g_luaAPI.CallR<int>("IsSailBoatNpc", static_cast<CCharacter*>(this), pCha)) {
			if (!ret.value()) {
				break;
			}
		}
		BoatSelected(static_cast<BYTE>(msg.type), static_cast<BYTE>(msg.index));
	}
	break;
	case CMD_CM_BOAT_BAGSEL: {
		net::msg::CmBoatBagSelMessage msg;
		net::msg::deserialize(pk, msg);
		DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
		if (dwNpcID) {
			CCharacter* pCha = m_submap->FindCharacter(dwNpcID, GetShape().centre);
			if (pCha == NULL)
				break;
		}

		BoatPackBag(static_cast<BYTE>(msg.index));
	}
	break;
	case CMD_CM_ENTITY_EVENT: {
		DWORD dwEntityID = pk.ReadInt64();
		CCharacter* pCha = m_submap->FindCharacter(dwEntityID, GetShape().centre);
		if (pCha == NULL) break;
		mission::CEventEntity* pEntity = pCha->IsEvent();
		if (pEntity) {
			pEntity->MsgProc(*this, pk);
			break;
		}
	}
	break;
	case CMD_CM_STALL_ALLDATA: {
		net::msg::CmStallInfoMessage stallMsg;
		net::msg::deserialize(pk, stallMsg);
		g_StallSystem.StartStall(*this, stallMsg);
	}
	break;
	case CMD_CM_STALL_OPEN: {
		net::msg::CmStallOpenMessage stallMsg;
		net::msg::deserialize(pk, stallMsg);
		g_StallSystem.OpenStall(*this, stallMsg);
	}
	break;
	case CMD_CM_STALL_BUY: {
		net::msg::CmStallBuyMessage stallMsg;
		net::msg::deserialize(pk, stallMsg);
		g_StallSystem.BuyGoods(*this, stallMsg);
	}
	break;
	case CMD_CM_STALL_CLOSE: {
		g_StallSystem.CloseStall(*this);
	}
	break;
	case CMD_CM_READBOOK_START: {
		CCharacter* pMainCha = GetPlyMainCha();
		if (!IsBoat()) {
			pMainCha->SetReadBookState(true);
			pMainCha->ForgeAction(true);
			pMainCha->m_CKitbag.Lock();
		}
		else
		//pMainCha->SystemNotice("");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00004));
	}
	break;
	case CMD_CM_READBOOK_CLOSE: {
		CCharacter* pMainCha = GetPlyMainCha();
		if (!IsBoat()) {
			pMainCha->SetReadBookState(false);
			pMainCha->ForgeAction(false);
			pMainCha->m_CKitbag.UnLock();
		}
		else
		//pMainCha->SystemNotice("");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00005));
	}
	break;
	case CMD_CM_SYNATTR:
	{
		net::msg::CmSynAttrMessage synMsg;
		net::msg::deserialize(pk, synMsg);
		GetPlayer()->GetMainCha()->Cmd_ReassignAttr(synMsg);
	}
	break;
	case CMD_CM_SKILLUPGRADE: {
		net::msg::CmSkillUpgradeMessage msg;
		net::msg::deserialize(pk, msg);
		Short sSkillID = static_cast<Short>(msg.skillId);
		Char chAddGrade = static_cast<Char>(msg.addGrade);

		// kong@pkodev.net 09.22.2017
		chAddGrade = 1;

		char chSkillLv = 0;
		CCharacter* pMainCha = GetPlyMainCha();
		SSkillGrid* pSkill = pMainCha->m_CSkillBag.GetSkillContByID(sSkillID);
		if (pSkill)
			chSkillLv = pSkill->chLv;

		if (chSkillLv <= 0) {
			SystemNotice("Unable to upgrade skill without learning!");
			break;
		}

		GetPlayer()->GetMainCha()->LearnSkill(sSkillID, chAddGrade, false);
	}
	break;
	case CMD_CM_REFRESH_DATA: {
		net::msg::CmRefreshDataMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_RefreshData(msg);
	}
	break;
	case CMD_TM_CHANGE_PERSONINFO: {
		net::msg::TmChangePersonInfoMessage msg;
		net::msg::deserialize(pk, msg);
		SetMotto(msg.motto.c_str());
		SetIcon(static_cast<int>(msg.icon));
	}
	break;
	case CMD_CM_GUILD_PERM: {
		net::msg::CmGuildPermMessage msg;
		net::msg::deserialize(pk, msg);
		int targetID = static_cast<int>(msg.id);
		unsigned long permission = static_cast<unsigned long>(msg.perms);
		int guild_id = GetPlyMainCha()->GetGuildID();
		if (guild_id == 0 || !emGldPermMgr & GetPlyMainCha()->guildPermission || game_db.GetGuildLeaderID(guild_id) ==
			targetID) {
			GetPlyMainCha()->SystemNotice("You do not have permission to do this.");
			return;
		}

		//update in DB
		if (!game_db.SetGuildPermission(targetID, permission, guild_id)) {
			GetPlyMainCha()->SystemNotice("Player not found");
			return;
		}

		//update in game
		CPlayer* targetPly = g_pGameApp->GetPlayerByDBID(targetID);
		if (targetPly) {
			targetPly->GetMainCha()->guildPermission = permission;
		}

		//update for group (sends to players)
		auto wpk = net::msg::serialize(net::msg::MpGuildPermMessage{
			static_cast<int64_t>(targetID), static_cast<int64_t>(permission)
		});
		ReflectINFof(this, wpk);
	}
	case CMD_CM_GUILD_PUTNAME: {
		net::msg::CmGuildPutNameMessage msg;
		net::msg::deserialize(pk, msg);
		bool l_confirm = msg.confirm ? true : false;
		if (!msg.guildName.empty() && Guild::IsValidGuildName(msg.guildName.c_str(), uShort(msg.guildName.length())) &&
			!msg.passwd.empty() && !strchr(msg.passwd.c_str(), '\'')) {
			Guild::cmd_CreateGuild(GetPlyMainCha(), l_confirm, msg.guildName.c_str(), msg.passwd.c_str());
		}
		else {
			//GetPlyMainCha()->SystemNotice("");
			GetPlyMainCha()->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00006));
		}
	}
	break;
	case CMD_CM_GUILD_TRYFOR: {
		net::msg::CmGuildTryForMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildTryFor(GetPlyMainCha(), msg.guildId);
	}
	break;
	case CMD_CM_GUILD_TRYFORCFM: {
		net::msg::CmGuildTryForCfmMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildTryForComfirm(GetPlyMainCha(), msg.confirm);
	}
	break;
	case CMD_CM_GUILD_LISTTRYPLAYER: {
		Guild::cmd_GuildListTryPlayer(GetPlyMainCha());
	}
	break;
	case CMD_CM_GUILD_APPROVE: {
		net::msg::CmGuildApproveMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildApprove(GetPlyMainCha(), msg.chaId);
	}
	break;
	case CMD_CM_GUILD_REJECT: {
		net::msg::CmGuildRejectMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildReject(GetPlyMainCha(), msg.chaId);
	}
	break;
	case CMD_CM_GUILD_KICK: {
		net::msg::CmGuildKickMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildKick(GetPlyMainCha(), msg.chaId);
	}
	break;
	case CMD_CM_GUILD_LEAVE: {
		if (!(GetPlyCtrlCha()->GetSubMap()->GetMapRes()->CanGuild())) {
			//GetPlyMainCha()->SystemNotice("!");
			GetPlyMainCha()->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00007));
			break;
		}

		Guild::cmd_GuildLeave(GetPlyMainCha());
	}
	break;
	case CMD_CM_GUILD_DISBAND: {
		net::msg::CmGuildDisbandMessage msg;
		net::msg::deserialize(pk, msg);
		int canDisband = (GetPlyMainCha()->guildPermission & emGldPermDisband);
		if (canDisband == emGldPermDisband) {
			if (!msg.passwd.empty() && !strchr(msg.passwd.c_str(), '\'')) {
				Guild::cmd_GuildDisband(GetPlyMainCha(), msg.passwd.c_str());
			}
		}
		break;
	}
	case CMD_CM_GUILD_MOTTO: {
		net::msg::CmGuildMottoMessage msg;
		net::msg::deserialize(pk, msg);
		if (!msg.motto.empty() && msg.motto.length() < 50 &&
			IsValidName(msg.motto.c_str(), uShort(msg.motto.length()))) {
			int canMotto = (GetPlyMainCha()->guildPermission & emGldPermMotto);
			if (canMotto == emGldPermMotto && !strchr(msg.motto.c_str(), '\'')) {
				// Probably not enough
				Guild::cmd_GuildMotto(GetPlyMainCha(), msg.motto.c_str());
			}
		}
	}
	break;
	case CMD_PM_GUILD_DISBAND: {
		Guild::cmd_PMDisband(GetPlyMainCha());
	}
	break;
	case CMD_CM_GUILD_CHALLENGE: {
		net::msg::CmGuildChallMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildChallenge(GetPlyMainCha(), static_cast<BYTE>(msg.level), static_cast<DWORD>(msg.money));
	}
	break;
	case CMD_CM_GUILD_LEIZHU: {
		net::msg::CmGuildLeizhuMessage msg;
		net::msg::deserialize(pk, msg);
		Guild::cmd_GuildLeizhu(GetPlyMainCha(), static_cast<BYTE>(msg.level), static_cast<DWORD>(msg.money));
	}
	break;
	case CMD_CM_MAP_MASK: {
		Handle_MapMask();
	}
	break;

	case CMD_CM_UPDATEHAIR: // 
	{
		if (!GetSubMap()) break;
		Cmd_ChangeHair(pk);
	}
	break;
	case CMD_CM_TEAM_FIGHT_ASK: // 
	{
		net::msg::CmTeamFightAskMessage msg;
		net::msg::deserialize(pk, msg);
		Cmd_FightAsk(static_cast<Char>(msg.type), static_cast<Long>(msg.worldId), static_cast<Long>(msg.handle));
	}
	break;
	case CMD_CM_TEAM_FIGHT_ASR: // 
	{
		net::msg::CmTeamFightAnswerMessage msg;
		net::msg::deserialize(pk, msg);
		Cmd_FightAnswer(msg.accept != 0 ? true : false);
	}
	break;
	case CMD_CM_ITEM_REPAIR_ASK: {
		net::msg::CmItemRepairAskMessage msg;
		net::msg::deserialize(pk, msg);

		Cmd_ItemRepairAsk(static_cast<Char>(msg.posType), static_cast<Char>(msg.posId));
	}
	break;
	case CMD_CM_ITEM_REPAIR_ASR: {
		net::msg::CmItemRepairAnswerMessage msg;
		net::msg::deserialize(pk, msg);
		Cmd_ItemRepairAnswer(msg.accept != 0 ? true : false);
	}
	break;
	case CMD_CM_ITEM_FORGE_CANACTION: {
		net::msg::CmItemForgeCanActionMessage msg;
		net::msg::deserialize(pk, msg);
		ForgeAction(msg.canAction != 0);
		break;
	}
	case CMD_CM_ITEM_FORGE_ASK: {
		net::msg::CmItemForgeGroupAskMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_ItemForgeAsk(msg);
	}
	break;
	// Add by lark.li 20080515 begin
	case CMD_CM_ITEM_LOTTERY_ASK: {
		net::msg::CmItemLotteryGroupAskMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_ItemLotteryAsk(msg);
	}
	break;
	// End
	case CMD_CM_ITEM_FORGE_ASR: {
		net::msg::CmItemForgeAnswerMessage msg;
		net::msg::deserialize(pk, msg);
		Cmd_ItemForgeAnswer(msg.accept != 0 ? true : false);
	}
	break;
	case CMD_CM_KITBAG_LOCK: {
		GetPlyMainCha()->Cmd_LockKitbag();
	}
	break;
	case CMD_CM_LIFESKILL_ASK: {
		net::msg::CmLifeSkillCraftMessage msg;
		if (net::msg::deserializeLifeSkillAsk(pk, msg))
			Handle_LifeSkillAsk(msg);
		break;
	}
	case CMD_CM_LIFESKILL_ASR: {
		net::msg::CmLifeSkillCraftMessage msg;
		if (net::msg::deserializeLifeSkillAsr(pk, msg))
			Handle_LifeSkillAsr(msg);
	}
	break;
	case CMD_CM_KITBAG_UNLOCK: {
		net::msg::CmKitbagUnlockMessage msg;
		net::msg::deserialize(pk, msg);
		GetPlyMainCha()->Cmd_UnlockKitbag(msg.password.c_str());
	}
	break;
	case CMD_CM_KITBAG_CHECK: {
		GetPlyMainCha()->Cmd_CheckKitbagState();
	}
	break;
	case CMD_CM_KITBAG_AUTOLOCK: {
		net::msg::CmAutoKitbagLockMessage msg;
		net::msg::deserialize(pk, msg);
		GetPlyMainCha()->Cmd_SetKitbagAutoLock(static_cast<char>(msg.autoLock));
	}
	break;
	case CMD_CM_STORE_OPEN_ASK: {
		net::msg::CmStoreOpenAskMessage storeMsg;
		net::msg::deserialize(pk, storeMsg);
		const std::string& szPwd = storeMsg.password;
		CCharacter* pMainCha = GetPlyMainCha();
		if (pMainCha->IsReadBook()) {
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00008));
			break;
		}

		if (pMainCha->IsStoreEnable()) {
			break;
		}

		if (!pMainCha->CheckStoreTime(1000)) {
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00009));
			break;
		}
		else {
			pMainCha->ResetStoreTime();
		}

		CPlayer* pCply = pMainCha->GetPlayer();
		cChar* szPwd2 = pCply->GetPassword();

		if ((szPwd2[0] == 0) || (!strcmp(szPwd.c_str(), szPwd2)) || g_Config.m_bInstantIGS) {
			pMainCha->SetStoreEnable(true);
		}
		else {
			pMainCha->PopupNotice(RES_STRING(GM_CHARACTERPRL_CPP_00010));
			break;
		}
	}
	case CMD_CM_STORE_LIST_ASK:
	case CMD_CM_STORE_BUY_ASK:
	case CMD_CM_STORE_CHANGE_ASK:
	case CMD_CM_STORE_QUERY:
	case CMD_CM_STORE_CLOSE:
	case CMD_CM_STORE_VIP: {
		Handle_StoreCommand(usCmd, pk);
		break;
	}

	case CMD_CM_TIGER_START: {
		net::msg::CmTigerStartMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_TigerStart(msg);
	}
	break;
	case CMD_CM_TIGER_STOP: {
		net::msg::CmTigerStopMessage msg;
		net::msg::deserialize(pk, msg);
		Handle_TigerStop(msg);
	}
	break;
	case CMD_CM_VOLUNTER_OPEN: {
		net::msg::CmVolunteerOpenMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_VolunteerOpen(cmMsg);
	}
	break;
	case CMD_CM_VOLUNTER_LIST: {
		net::msg::CmVolunteerListMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_VolunteerList(cmMsg);
	}
	break;
	case CMD_CM_VOLUNTER_ADD: {
		CCharacter* pMainCha = GetPlyMainCha();
		pMainCha->Cmd_AddVolunteer();
		pMainCha->SynVolunteerState(pMainCha->IsVolunteer());
	}
	break;
	case CMD_CM_VOLUNTER_DEL: {
		CCharacter* pMainCha = GetPlyMainCha();
		pMainCha->Cmd_DelVolunteer();
		pMainCha->SynVolunteerState(pMainCha->IsVolunteer());
	}
	break;
	case CMD_CM_VOLUNTER_SEL: {
		net::msg::CmVolunteerSelMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_VolunteerSel(cmMsg);
	}
	break;
	case CMD_CM_VOLUNTER_ASR: {
		net::msg::CmVolunteerAsrMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_VolunteerAsr(cmMsg);
	}
	break;
	case CMD_CM_KITBAGTEMP_SYNC: {
		Handle_KitbagTempSync();
	}
	break;
	case CMD_CM_ITEM_LOCK_ASK: {
		net::msg::CmItemLockAskMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_ItemLockAsk(cmMsg);
	}
	break;
	case CMD_CM_GAME_REQUEST_PIN: {
		net::msg::CmGameRequestPinMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_GameRequestPin(cmMsg);
		break;
	}
	case CMD_CM_ITEM_UNLOCK_ASK: {
		net::msg::CmItemUnlockAskMessage unlockMsg;
		net::msg::deserialize(pk, unlockMsg);
		ItemUnlockRequest(unlockMsg);
	}
	break;
	case CMD_CM_MASTER_INVITE: {
		net::msg::CmMasterInviteMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_MasterInvite(cmMsg);
	}
	break;
	case CMD_CM_MASTER_ASR: {
		net::msg::CmMasterAsrMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_MasterAsr(cmMsg);
	}
	break;
	case CMD_CM_MASTER_DEL: {
		net::msg::CmMasterDelMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_MasterDel(cmMsg);
	}
	break;
	case CMD_CM_PRENTICE_DEL: {
		net::msg::CmPrenticeDelMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		Handle_PrenticeDel(cmMsg);
	}
	break;
	case CMD_CM_PRENTICE_INVITE: {
		CCharacter* pMainCha = GetPlyMainCha();
		net::msg::CmPrenticeInviteMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		std::string szName = cmMsg.name;
		DWORD dwCharID = static_cast<DWORD>(cmMsg.chaId);

		if (IsBoat()) {
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00023));
			break;
		}

		CCharacter* pTarCha = pMainCha->GetSubMap()->FindCharacter(dwCharID, pMainCha->GetShape().centre);
		if (!pTarCha) {
			//pMainCha->SystemNotice("%s !", szName);
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
			break;
		}

		if (pMainCha->GetLevel() < 41) {
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
			break;
		}

		if (pTarCha->GetLevel() > 40) {
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
			break;
		}

		if (pTarCha->IsInvited()) {
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00022));
			break;
		}
		if (!pTarCha->GetPlayer()->CanReceiveRequests()) {
			pMainCha->SystemNotice("%s is currently offline. Unable to send request!", pMainCha->GetName());
			break;
		}
		pTarCha->SetInvited(true);

		//  :  
		auto packet = net::msg::serialize(net::msg::McPrenticeAskMessage{pMainCha->GetName(), pMainCha->GetID()});
		pTarCha->ReflectINFof(pTarCha, packet);
	}
	break;
	case CMD_CM_PRENTICE_ASR: {
		CCharacter* pMainCha = GetPlyMainCha();
		net::msg::CmPrenticeAsrMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		short sRet = static_cast<short>(cmMsg.agree);
		std::string szName = cmMsg.name;
		DWORD dwCharID = static_cast<DWORD>(cmMsg.chaId);

		pMainCha->SetInvited(false);

		if (IsBoat()) {
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00019));
			break;
		}

		CCharacter* pSrcCha = pMainCha->GetSubMap()->FindCharacter(dwCharID, pMainCha->GetShape().centre);
		if (!pSrcCha) {
			//pMainCha->SystemNotice("%s !", szName);
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
			break;
		}

		if (pSrcCha->GetLevel() < 41) {
			//pSrcCha->SystemNotice("!");
			pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
			break;
		}

		if (pMainCha->GetLevel() > 40) {
			//pSrcCha->SystemNotice("!");
			pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
			break;
		}

		if (sRet == 0) {
			//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
			pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00030), pMainCha->GetName());
			break;
		}

		//  :   - (  )
		auto l_wpk = net::msg::serialize(net::msg::MpMasterCreateMessage{
			pMainCha->GetName(), pMainCha->GetPlayer()->GetDBChaId(),
			pSrcCha->GetName(), pSrcCha->GetPlayer()->GetDBChaId()
		});
		pMainCha->ReflectINFof(pMainCha, l_wpk);
	}
	break;
	case CMD_CM_SAY2CAMP: {
		CCharacter* pMainCha = GetPlyMainCha();
		net::msg::CmSay2CampMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		std::string szContent = cmMsg.content;
		CCharacter* pCha = NULL;
		SubMap* pSubMap = GetPlyCtrlCha()->GetSubMap();

		BOOL bHasGuild = pMainCha->HasGuild();
		if (!bHasGuild) {
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00031));
			break;
		}

		SystemNotice("Channel disabled; used for communicating with guild members within sacred war map.");
		break;
		/*
		if(pSubMap->GetMapRes()->CanGuildWar())
		{
			char cGuildType = pMainCha->GetGuildType();

			pSubMap->BeginGetPlyCha();
			while(pCha = pSubMap->GetNextPlyCha())
			{
				if(pCha->HasGuild() && pCha->GetGuildType() == cGuildType)
				{
					auto l_wpk = net::msg::serialize(net::msg::McSay2CampMessage{pMainCha->GetName(), szContent});
					pCha->ReflectINFof(pCha, l_wpk);
				}
			}
		}
		else
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00032));
		}
		*/
		//if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
		//{
		//	SystemNotice("Unable to chat in this map!");
		//	break;
		//}
	}
	break;
	case CMD_CM_GM_SEND:
	case CMD_CM_GM_RECV:
		// GM mail   InfoServer (Trade Server) --
		break;
	case CMD_CM_PK_CTRL: {
		CCharacter* pMainCha = GetPlyMainCha();

		net::msg::CmPkCtrlMessage cmMsg;
		net::msg::deserialize(pk, cmMsg);
		if (cmMsg.ctrl)
			Cmd_SetInPK();
		else
			Cmd_SetInPK(false);
		SynPKCtrl();
	}
	break;
	case CMD_CM_CHEAT_CHECK: {
		//
		/*CCharacter *pMainCha = GetPlyMainCha();

		cChar *answer = pk.ReadString();
		pMainCha->CheatCheck(answer);*/
	}
	break;
	case CMD_CM_BIDUP:
		//add by ALLEN 2007-10-19
	{
		//
		CCharacter* pMainCha = GetPlyMainCha();
		if (auto yornRet = g_luaAPI.CallR<int>("YORN", pMainCha)) {
			if (yornRet.value()) {
				net::msg::CmBidUpMessage cmMsg;
				net::msg::deserialize(pk, cmMsg);
				DWORD dwNpcID = static_cast<DWORD>(cmMsg.npcId);
				CCharacter* pNpc = m_submap->FindCharacter(dwNpcID, GetShape().centre);
				if (pNpc == NULL) {
					//SystemNotice( "NPCID%d", dwNpcID );
					SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00034), dwNpcID);
					break;
				}
				g_AuctionSystem.BidUp(pMainCha, static_cast<short>(cmMsg.itemId), static_cast<uInt>(cmMsg.price));
				g_AuctionSystem.NotifyAuction(this, pNpc);
			}
		}
	}
	break;
	case CMD_CM_ANTIINDULGENCE: {
		GetPlyMainCha()->SetScaleFlag();
	}
	break;
	case CMD_CM_REQUEST_DROP_RATE: {
		CCharacter* pCha = GetPlyCtrlCha();
		if (pCha) {
			//  :     
			auto pk = net::msg::serialize(net::msg::McRequestDropRateMessage{pCha->GetDropRate()});
			pCha->ReflectINFof(pCha, pk);
		}
		break;
	}
	case CMD_CM_REQUEST_EXP_RATE: {
		CCharacter* pCha = GetPlyMainCha();
		if (pCha) {
			//  :     
			auto pk = net::msg::serialize(net::msg::McRequestExpRateMessage{pCha->GetExpRate()});
			pCha->ReflectINFof(pCha, pk);
		}
		break;
	}
	default:
		break;
	}
}

//    (  ProcessPacket) 

void CCharacter::Handle_GuildBankCmd(const net::msg::PmGuildBankMessage& msg) {
	Char bankType = static_cast<Char>(msg.bankType);

	if (const DWORD COOLDOWN = GetTickCount(); GetPlyMainCha()->GuildBankCD > COOLDOWN) {
		BickerNotice("Please calm down Don't spam! %ds left!", (GetPlyMainCha()->GuildBankCD - COOLDOWN) / 1000);
	}
	else if (!IsLiveing()) {
		SystemNotice("Dead pirates are unable to trade.");
	}
	else if (GetPlyCtrlCha()->IsBoat()) {
		SystemNotice("Must be on land to use the guild bank.");
	}
	else if (!IsInArea(2)) {
		SystemNotice("Must be in safe zone to use the guild bank.");
	}
	else {
		const int cdtime = 3000;
		GetPlyMainCha()->GuildBankCD = COOLDOWN + cdtime;

		if (bankType == 0 && std::holds_alternative<net::msg::PmGuildBankOperData>(msg.data)) {
			auto& operData = std::get<net::msg::PmGuildBankOperData>(msg.data);
			Char chSrcType = static_cast<Char>(operData.srcType);
			Short sSrcGrid = static_cast<Short>(operData.srcGrid);
			Short sSrcNum = static_cast<Short>(operData.srcNum);
			Char chTarType = static_cast<Char>(operData.tarType);
			Short sTarGrid = static_cast<Short>(operData.tarGrid);
			int guildID = GetGuildID();
			std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);

			if (chTarType != chSrcType) {
				CTableGuild::BankLog l;
				CKitbag bag;

				l.time = time(0);
				l.quantity = sSrcNum;
				l.userID = GetPlyMainCha()->m_ID;

				if (chTarType == 0) {
					game_db.GetGuildBank(guildID, &bag);
					l.type = 2;
				}
				else if (chTarType == 1) {
					bag = GetPlyMainCha()->m_CKitbag;
					l.type = 3;
				}
				l.parameter = bag.GetID(sSrcGrid);
				logs.push_back(l);
			}
			Short sRet = Cmd_GuildBankOper(chSrcType, sSrcGrid, sSrcNum, chTarType, sTarGrid);
			if (sRet != enumITEMOPT_SUCCESS || !game_db.SetGuildLog(logs, guildID)) {
				ItemOprateFailed(sRet);
			}
		}
		else if (bankType == 1 && std::holds_alternative<net::msg::PmGuildBankGoldData>(msg.data)) {
			[&]() {
				auto& goldData = std::get<net::msg::PmGuildBankGoldData>(msg.data);
				Char action = static_cast<Char>(goldData.direction);

				int guildID = GetGuildID();
				int gold = static_cast<int>(goldData.gold);
				int currentgold = getAttr(ATTR_GD);
				unsigned long long guildGold = game_db.GetGuildBankGold(guildID);

				unsigned long long maxGuildGold = 9223372036854775807LL;
				int maxCharGold = 2000000000;
				std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);

				int canTake = (emGldPermTakeBank & guildPermission);
				int canGive = (emGldPermDepoBank & guildPermission);

				CTableGuild::BankLog l;

				if (action == 0 && canTake == emGldPermTakeBank) {
					l.type = 0;
					if (gold + currentgold > maxCharGold) {
						gold = maxCharGold - currentgold;
					}
					if (gold > guildGold) {
						gold = guildGold;
					}
					if (gold < 1) {
						return;
					}
				}
				else if (action == 1 && canGive == emGldPermDepoBank) {
					l.type = 1;
					if (gold > currentgold) {
						gold = currentgold;
					}
					if (gold + guildGold > maxGuildGold) {
						gold = maxGuildGold - guildGold;
					}
					if (gold < 1) {
						return;
					}
					gold = 0 - gold;
				}
				else {
					return;
				}

				if (game_db.UpdateGuildBankGold(guildID, -gold)) {
					l.time = time(0);
					l.parameter = gold > 0 ? gold : -gold;
					l.quantity = 0;
					l.userID = GetPlyMainCha()->m_ID;

					logs.push_back(l);
					if (game_db.SetGuildLog(logs, guildID)) {
						setAttr(ATTR_GD, currentgold + gold);
						SynAttr(enumATTRSYN_TRADE);
						SyncBoatAttr(enumATTRSYN_TRADE);

						auto WtPk = net::msg::serialize(net::msg::MmUpdateGuildBankGoldMessage{
							static_cast<int64_t>(GetPlyMainCha()->GetGuildID())
						});
						ReflectINFof(this, WtPk);
					}
				}
			}();
		}
	}

	auto WtPk = net::msg::serialize(net::msg::MpGuildBankAckMessage{
		static_cast<int64_t>(GetGuildID())
	});
	ReflectINFof(this, WtPk);
}

void CCharacter::Handle_PushToGuildBank(const std::string& strItem) {
	int guildID = GetGuildID();
	if (guildID == 0) {
		return;
	}
	CKitbag pCSrcBag;
	game_db.GetGuildBank(guildID, &pCSrcBag);
	pCSrcBag.SetChangeFlag(false);

	SItemGrid SPopItem;
	String2Item(strItem.c_str(), &SPopItem);

	short sSrcGridID = defKITBAG_DEFPUSH_POS;
	if (pCSrcBag.Push(&SPopItem, sSrcGridID) == enumKBACT_ERROR_FULL) {
		//drop item next to player?
	}
	else {
		GetPlayer()->SynGuildBank(&pCSrcBag, enumSYN_KITBAG_BANK);
		GetPlayer()->SetBankSaveFlag(0);
		game_db.UpdateGuildBank(guildID, &pCSrcBag);
	}
	//let group know we have finished, so the next guild bank packet can be processed.
	auto WtPk = net::msg::serialize(net::msg::MpGuildBankAckMessage{
		static_cast<int64_t>(guildID)
	});
	ReflectINFof(this, WtPk);
}

void CCharacter::Handle_Ping(const net::msg::CmPingResponseMessage& msg) {
	uLong ulPing = GetTickCount() - static_cast<uLong>(msg.v1);
	Long lGateSvr = static_cast<Long>(msg.v2);
	Long lSrcID = static_cast<Long>(msg.v3);
	Long lGatePlayerID = static_cast<Long>(msg.v4);
	Long lGatePlayerAddr = static_cast<Long>(msg.v5);

	BEGINGETGATE();
	GateServer* pNoGate;
	GateServer* pGate = 0;
	while (pNoGate = GETNEXTGATE()) {
		if (ToAddress(pNoGate) == lGateSvr) {
			pGate = pNoGate;
			break;
		}
	}
	if (!pGate)
		return;

	auto WtPk = net::msg::serialize(net::msg::McQueryChaPingRouteMessage{
		lSrcID, std::string(GetName()), std::string(GetSubMap()->GetName()),
		static_cast<int64_t>(ulPing), lGatePlayerID, lGatePlayerAddr, 1
	});
	pGate->SendData(WtPk);
}

void CCharacter::Handle_Say(const net::msg::CmSayMessage& sayMsg) {
	DWORD dwNowTick = GetTickCount();
	if (dwNowTick - _dwLastSayTick < (DWORD)g_Config.m_lSayInterval) {
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00001));
		return;
	}
	_dwLastSayTick = dwNowTick;

	if (!GetSubMap()) {
		ToLogService("errors", LogLevel::Error, "when character{} is dialog, the map is null", GetLogName());
		return;
	}
	if (sayMsg.content.empty())
		return;
	else if (sayMsg.content[0] == '&') {
		Char chGMLv = GetPlayer()->GetGMLev();
		if (chGMLv == 0 || chGMLv > 150)
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00002));
		else
			DoCommand(sayMsg.content.c_str() + 1, static_cast<uLong>(sayMsg.content.size()));
	}
	else if (sayMsg.content.size() > 2 && sayMsg.content[0] == '$' && sayMsg.content[1] == '$') {
		DoCommand_CheckStatus(sayMsg.content.c_str() + 3, static_cast<uLong>(sayMsg.content.size() - 2));
	}
	else {
		auto chatRet = g_luaAPI.CallR<int>("HandleChat", static_cast<CCharacter*>(this), sayMsg.content.c_str());
		if (!chatRet.value_or(0))
			return;
		if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver()) {
			SystemNotice("Unable to chat in this map!");
			return;
		}

		auto wpk = net::msg::serialize(net::msg::McSayMessage{
			m_ID, sayMsg.content, static_cast<int64_t>(chatColour)
		});
		NotiChgToEyeshot(wpk);
	}
}

void CCharacter::Handle_RequestTalk(uLong npcId, net::RPacket& pk) {
	if (npcId == mission::g_WorldEudemon.GetID()) {
		mission::g_WorldEudemon.MsgProc(*this, pk);
		return;
	}
	CCharacter* pCha = m_submap->FindCharacter(npcId, GetShape().centre);
	if (pCha == NULL) return;
	mission::CNpc* pNpc = pCha->IsNpc();
	if (pNpc) {
		pNpc->MsgProc(*this, pk);
		return;
	}
	else {
		lua_getglobal(g_pLuaState, "extNpcNpcProc");
		if (!lua_isfunction(g_pLuaState, -1)) {
			lua_pop(g_pLuaState, 1);
			return;
		}

		luabridge::push(g_pLuaState, static_cast<CCharacter*>(this));
		luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCha));
		luabridge::push(g_pLuaState, &pk);

		int nStatus = lua_pcall(g_pLuaState, 3, 0, 0);
		lua_settop(g_pLuaState, 0);
	}
}

void CCharacter::Handle_DailyBuffRequest() {
	CCharacter* pCMainCha = GetPlyMainCha();
	CPlayer* pCPly = GetPlayer();
	if (pCMainCha->m_CKitbag.IsLock() || pCMainCha->m_CKitbag.IsPwdLocked() || pCPly->GetStallData() || pCPly->
		GetMainCha()->GetTradeData()) {
		SystemNotice("Bag is currently locked.");
		return;
	}
	if (!IsLiveing()) {
		SystemNotice("Ahoy there, matey! Looks like you took a tumble. Up and at 'em!");
		return;
	}
	if (pCPly->GetCtrlCha()->IsBoat()) {
		SystemNotice("Can't Use While Sailing .");
		return;
	}

	g_luaAPI.Call("DailyBuffRequest", static_cast<CCharacter*>(this));
}

void CCharacter::Handle_RefreshData(const net::msg::CmRefreshDataMessage& msg) {
	Long lWorldID = static_cast<Long>(msg.worldId);
	Long lHandle = static_cast<Long>(msg.handle);
	Entity* pCEnt = g_pGameApp->IsLiveingEntity(lWorldID, lHandle);
	if (pCEnt) {
		CCharacter* pCCha = pCEnt->IsCharacter();
		if (pCCha && pCCha->GetPlayer() == GetPlayer()) {
			pCCha->SynAttr(enumATTRSYN_ITEM_EQUIP);
		}
	}
}

void CCharacter::Handle_MapMask() {
	if (!GetSubMap())
		return;
	//const char	*szMapName = pk.ReadString();
	const char* szMapName = GetSubMap()->GetName();

	long lDataLen;
	BYTE* pData = GetPlayer()->GetMapMask(lDataLen);
	net::msg::McMapMaskMessage msg;
	msg.worldId = m_ID;
	if (!pData)
		msg.hasData = false;
	else {
		msg.hasData = true;
		msg.data.assign((char*)pData, (char*)pData + lDataLen);
	}
	auto wpk = net::msg::serialize(msg);
	ReflectINFof(this, wpk);
}

void CCharacter::Handle_ItemForgeAsk(const net::msg::CmItemForgeGroupAskMessage& msg) {
	if (!msg.sure) {
		ForgeAction(false);
		return;
	}
	SForgeItem SFgeItem;
	for (int i = 0; i < defMAX_ITEM_FORGE_GROUP; i++) {
		SFgeItem.SGroup[i].sGridNum = static_cast<short>(msg.groups[i].cells.size());
		if (SFgeItem.SGroup[i].sGridNum < 0 || SFgeItem.SGroup[i].sGridNum > defMAX_KBITEM_NUM_PER_TYPE) {
			ForgeAction(false);
			return;
		}
		for (short j = 0; j < SFgeItem.SGroup[i].sGridNum; j++) {
			SFgeItem.SGroup[i].SGrid[j].sGridID = static_cast<short>(msg.groups[i].cells[j].posId);
			SFgeItem.SGroup[i].SGrid[j].sItemNum = static_cast<short>(msg.groups[i].cells[j].num);
		}
	}
	Cmd_ItemForgeAsk(static_cast<Char>(msg.type), &SFgeItem);
}

void CCharacter::Handle_ItemLotteryAsk(const net::msg::CmItemLotteryGroupAskMessage& msg) {
	if (!msg.sure) {
		ForgeAction(false);
		return;
	}

	SLotteryItem SLtrItem;
	for (int i = 0; i < defMAX_ITEM_LOTTERY_GROUP; i++) {
		SLtrItem.SGroup[i].sGridNum = static_cast<short>(msg.groups[i].cells.size());
		if (SLtrItem.SGroup[i].sGridNum < 0 || SLtrItem.SGroup[i].sGridNum > defMAX_KBITEM_NUM_PER_TYPE) {
			return;
		}
		for (short j = 0; j < SLtrItem.SGroup[i].sGridNum; j++) {
			SLtrItem.SGroup[i].SGrid[j].sGridID = static_cast<short>(msg.groups[i].cells[j].posId);
			SLtrItem.SGroup[i].SGrid[j].sItemNum = static_cast<short>(msg.groups[i].cells[j].num);
		}
	}
	Cmd_ItemLotteryAsk(&SLtrItem);
}

void CCharacter::Handle_LifeSkillAsk(const net::msg::CmLifeSkillCraftMessage& craftMsg) {
	long type = static_cast<long>(craftMsg.skillType);
	int posCount = net::msg::kLifeSkillNeedItemNum[type];

	SLifeSkillItem LifeSkillItem;
	LifeSkillItem.sbagCount = static_cast<short>(posCount);
	for (int i = 0; i < posCount; i++) {
		LifeSkillItem.sGridID[i] = static_cast<short>(craftMsg.positions[i]);
	}
	switch (type) {
	case 0: {
		LifeSkillItem.sReturn = atoi(GetPlayer()->GetLifeSkillinfo().c_str());
		break;
	}
	case 1: {
		string strVer[2];
		Util_ResolveTextLine(GetPlayer()->GetLifeSkillinfo().c_str(), strVer, 2, ',');
		if (atoi(strVer[0].c_str()) > atoi(strVer[1].c_str()))
			LifeSkillItem.sReturn = 1;
		else
			LifeSkillItem.sReturn = 0;
		break;
	}
	case 2: {
		short sret = static_cast<short>(craftMsg.extraParam);
		string strVer[3];
		Util_ResolveTextLine(GetPlayer()->GetLifeSkillinfo().c_str(), strVer, 3, ',');
		int count = atoi(strVer[0].c_str()) + atoi(strVer[1].c_str()) + atoi(strVer[2].c_str());
		count -= 9;
		if (count > 0)
			count = 1;
		else
			count = 0;
		if (count == sret)
			LifeSkillItem.sReturn = 1;
		else
			LifeSkillItem.sReturn = 0;
		break;
	}
	case 3: {
		LifeSkillItem.sReturn = static_cast<short>(craftMsg.extraParam);
		break;
	}
	}
	Cmd_LifeSkillItemAsk(type, &LifeSkillItem);
}

void CCharacter::Handle_LifeSkillAsr(const net::msg::CmLifeSkillCraftMessage& craftAsrMsg) {
	long type = static_cast<long>(craftAsrMsg.skillType);
	int posCount = net::msg::kLifeSkillNeedItemNum[type];

	SLifeSkillItem LifeSkillItem;
	LifeSkillItem.sbagCount = static_cast<short>(posCount);
	for (int i = 0; i < posCount; i++) {
		LifeSkillItem.sGridID[i] = static_cast<short>(craftAsrMsg.positions[i]);
	}

	switch (type) {
	case 0: {
		LifeSkillItem.sReturn = 1;
	}
	case 1: {
		LifeSkillItem.sReturn = 0;
	}
	case 2: {
		LifeSkillItem.sReturn = static_cast<short>(craftAsrMsg.extraParam);
		break;
	}
	case 3: {
		LifeSkillItem.sReturn = static_cast<short>(craftAsrMsg.extraParam);
		break;
	}
	}

	Cmd_LifeSkillItemAsR(type, &LifeSkillItem);
}

void CCharacter::Handle_StoreCommand(uShort usCmd, net::RPacket& pk) {
	CCharacter* pMainCha = GetPlyMainCha();
	if (!pMainCha->IsStoreEnable()) {
		return;
	}
	lua_getglobal(g_pLuaState, "operateIGS");
	if (!lua_isfunction(g_pLuaState, -1)) {
		lua_pop(g_pLuaState, 1);
		return;
	}

	luabridge::push(g_pLuaState, static_cast<CCharacter*>(this));
	luabridge::push(g_pLuaState, &pk);
	int nStatus = lua_pcall(g_pLuaState, 2, 0, 0);
	lua_settop(g_pLuaState, 0);

	if (usCmd == CMD_CM_STORE_CLOSE) {
		CCharacter* pMainCha = GetPlyMainCha();
		pMainCha->SetStoreEnable(false);
		pMainCha->ForgeAction(false);
	}
}

void CCharacter::Handle_TigerStart(const net::msg::CmTigerStartMessage& msg) {
	m_sTigerSel[0] = (msg.sel1 > 0) ? 1 : 0;
	m_sTigerSel[1] = (msg.sel2 > 0) ? 1 : 0;
	m_sTigerSel[2] = (msg.sel3 > 0) ? 1 : 0;

	CCharacter* pCha = m_submap->FindCharacter(static_cast<DWORD>(msg.npcId), GetShape().centre);
	if (pCha == NULL)
		return;

	CCharacter* pMainCha = GetPlyMainCha();
	pMainCha->DoTigerScript("TigerStart");
}

void CCharacter::Handle_TigerStop(const net::msg::CmTigerStopMessage& msg) {
	CCharacter* pCha = m_submap->FindCharacter(static_cast<DWORD>(msg.npcId), GetShape().centre);
	if (pCha == NULL)
		return;

	CCharacter* pMainCha = GetPlyMainCha();
	short sNum = static_cast<short>(msg.num);

	if (sNum < 1 || sNum > 3) {
		pMainCha->ForgeAction(false);
		memset(m_sTigerItemID, 0, sizeof(m_sTigerItemID));
		memset(m_sTigerSel, 0, sizeof(m_sTigerSel));
		return;
	}

	short sIndex = 3 * (sNum - 1);
	bool bSucc = true;
	//  :   
	int64_t ids[3];
	for (int i = 0; i < 3; i++) {
		if (pMainCha->m_sTigerItemID[sIndex] <= 0) {
			bSucc = false;
		}
		ids[i] = pMainCha->m_sTigerItemID[sIndex++];
	}
	auto wpk = net::msg::serialize(net::msg::McTigerItemIdMessage{
		static_cast<int64_t>(sNum), ids[0], ids[1], ids[2]
	});
	ReflectINFof(this, wpk);

	if (bSucc) {
		if (sNum == 3) {
			pMainCha->DoTigerScript("TigerStop");
			memset(m_sTigerItemID, 0, sizeof(m_sTigerItemID));
			memset(m_sTigerSel, 0, sizeof(m_sTigerSel));
		}
	}
}

void CCharacter::Handle_VolunteerOpen(const net::msg::CmVolunteerOpenMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	short sNum = static_cast<short>(cmMsg.num);

	int nVolNum = g_pGameApp->GetVolNum();
	int nStart = 0;
	short sRetNum = (nVolNum - nStart < sNum) ? (nVolNum - nStart) : sNum;
	if (sRetNum < 0)
		sRetNum = 0;
	short sPageNum = (nVolNum % sNum == 0) ? (nVolNum / sNum) : (nVolNum / sNum + 1);

	char chState = (pMainCha->IsVolunteer() ? 1 : 0);
	//  :   
	net::msg::McVolunteerOpenMessage openMsg;
	openMsg.state = static_cast<int64_t>(chState);
	openMsg.pageTotal = static_cast<int64_t>(sPageNum);
	for (int i = 0; i < sRetNum; i++) {
		SVolunteer* pVolunteer = g_pGameApp->GetVolInfo(nStart + i);
		openMsg.volunteers.push_back({
			pVolunteer->szName, pVolunteer->lLevel, pVolunteer->lJob, pVolunteer->szMapName
		});
	}
	auto packet = net::msg::serialize(openMsg);
	ReflectINFof(this, packet);
}

void CCharacter::Handle_VolunteerList(const net::msg::CmVolunteerListMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	short sPage = static_cast<short>(cmMsg.page);
	short sNum = static_cast<short>(cmMsg.num);

	int nVolNum = g_pGameApp->GetVolNum();
	int nStart = (sPage - 1) * sNum;
	short sRetNum = (nVolNum - nStart < sNum) ? (nVolNum - nStart) : sNum;
	if (sRetNum < 0)
		sRetNum = 0;
	short sPageNum = (nVolNum % sNum == 0) ? (nVolNum / sNum) : (nVolNum / sNum + 1);

	//  :  
	net::msg::McVolunteerListMessage listMsg;
	listMsg.pageTotal = static_cast<int64_t>(sPageNum);
	listMsg.page = static_cast<int64_t>(sPage);
	for (int i = 0; i < sRetNum; i++) {
		SVolunteer* pVolunteer = g_pGameApp->GetVolInfo(nStart + i);
		listMsg.volunteers.push_back({
			pVolunteer->szName, pVolunteer->lLevel, pVolunteer->lJob, pVolunteer->szMapName
		});
	}
	auto packet = net::msg::serialize(listMsg);
	ReflectINFof(this, packet);
}

void CCharacter::Handle_VolunteerSel(const net::msg::CmVolunteerSelMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	if (pMainCha->GetLevel() < 8) {
		pMainCha->PopupNotice("Only players lv8 and above can request party!");
		return;
	}

	std::string szName = cmMsg.name;
	CCharacter* pTarCha = FindVolunteer(szName.c_str());
	if (!pTarCha) {
		//pMainCha->SystemNotice("%s !", szName);
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
		return;
	}

	if (pTarCha == pMainCha) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00013));
		return;
	}

	if (strcmp(pTarCha->GetPlyCtrlCha()->GetSubMap()->GetName(), GetPlyCtrlCha()->GetSubMap()->GetName())) {
		//pMainCha->SystemNotice(", !");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00014));
		return;
	}

	if (!(GetPlyCtrlCha()->GetSubMap()->GetMapRes()->CanTeam())) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00015));
		return;
	}

	//pMainCha->SystemNotice(",!");
	pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00016));

	//  :  
	auto packet = net::msg::serialize(net::msg::McVolunteerAskMessage{pMainCha->GetName()});
	pTarCha->ReflectINFof(pTarCha, packet);
}

void CCharacter::Handle_VolunteerAsr(const net::msg::CmVolunteerAsrMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	short sRet = static_cast<short>(cmMsg.ret);
	std::string szName = cmMsg.name;
	CCharacter* pSrcCha = g_pGameApp->FindChaByName(szName.c_str());
	if (!pSrcCha) {
		//pMainCha->SystemNotice("%s !", szName);
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
		return;
	}

	if (sRet == 0) {
		//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
		pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00018), pMainCha->GetName());
		return;
	}

	auto l_wpk = net::msg::serialize(net::msg::MpTeamCreateMessage{
		std::string(pSrcCha->GetName()), std::string(pMainCha->GetName())
	});
	pMainCha->ReflectINFof(pMainCha, l_wpk);
}

void CCharacter::Handle_KitbagTempSync() {
	CCharacter* pMainCha = GetPlyMainCha();

	if (!pMainCha->m_pCKitbagTmp) {
		return;
	}

	net::msg::McKitbagTempSyncMessage msg;
	msg.kitbag = pMainCha->BuildKitbagInfo(*(pMainCha->m_pCKitbagTmp), enumSYN_KITBAG_INIT);
	auto pkret = net::msg::serialize(msg);
	pMainCha->ReflectINFof(pMainCha, pkret);
}

void CCharacter::Handle_ItemLockAsk(const net::msg::CmItemLockAskMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	CPlayer* pCPly = GetPlayer();

	if (pMainCha) {
		if (pMainCha->m_CKitbag.IsLock() || pMainCha->m_CKitbag.IsPwdLocked() || pCPly->GetStallData() || pCPly->
			GetMainCha()->GetTradeData()) {
			SystemNotice("Bag is currently locked.");
			return;
		}

		dbc::Char chPosType = static_cast<dbc::Char>(cmMsg.slot);
		SItemGrid* item = pMainCha->m_CKitbag.GetGridContByID(chPosType);
		if (item) {
			CItemRecord* pCItemRec = GetItemRecordInfo(item->sID);
			if (pCItemRec) {
				CPlayer* pPlayer = pMainCha->GetPlayer();
				if (pPlayer) {
					item->dwDBID = 1;
					this->m_CKitbag.SetChangeFlag();
					this->SynKitbagNew(enumSYN_KITBAG_SWITCH);
					auto rpk = net::msg::serialize(net::msg::McItemLockAsrMessage{1});
					this->ReflectINFof(pMainCha, rpk);
					return;
				};
			};
		};
	};
	auto rpk = net::msg::serialize(net::msg::McItemLockAsrMessage{0});
	pMainCha->ReflectINFof(pMainCha, rpk);
}

void CCharacter::Handle_GameRequestPin(const net::msg::CmGameRequestPinMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	if (!pMainCha)
		return;

	if (requestType == NULL)
		return;

	if (!IsReqPosEqualRealPos()) {
		requestType = NULL;
		return;
	}

	std::string szPwd = cmMsg.password;
	if (szPwd.empty())
		return;

	CPlayer* pCply = pMainCha->GetPlayer();
	cChar* szPwd2 = pCply->GetPassword();
	if ((szPwd2[0] == 0) || (!strcmp(szPwd.c_str(), szPwd2))) {
		auto pinRet = g_luaAPI.CallR<int>("HandlePinRequest", static_cast<CCharacter*>(this), (int)requestType);
		if (!pinRet.value_or(0))
			return;
	}
	else {
		pMainCha->PopupNotice(RES_STRING(GM_CHARACTERPRL_CPP_00010));
	}
}

void CCharacter::Handle_MasterInvite(const net::msg::CmMasterInviteMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	std::string szName = cmMsg.name;
	DWORD dwCharID = static_cast<DWORD>(cmMsg.chaId);

	if (IsBoat()) {
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00019));
		return;
	}

	CCharacter* pTarCha = pMainCha->GetSubMap()->FindCharacter(dwCharID, pMainCha->GetShape().centre);
	if (!pTarCha) {
		//pMainCha->SystemNotice("%s !", szName);
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
		return;
	}

	if (pTarCha->GetLevel() < 41) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
		return;
	}

	if (pMainCha->GetLevel() > 40) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
		return;
	}

	if (pMainCha->GetMasterDBID() != 0) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00021));
		return;
	}

	if (pTarCha->IsInvited()) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00022));
		return;
	}
	if (!pTarCha->GetPlayer()->CanReceiveRequests()) {
		pMainCha->SystemNotice("%s is currently offline. Unable to send request!", pMainCha->GetName());
		return;
	}

	pTarCha->SetInvited(true);

	//  :  
	auto packet = net::msg::serialize(net::msg::McMasterAskMessage{pMainCha->GetName(), pMainCha->GetID()});
	pTarCha->ReflectINFof(pTarCha, packet);
}

void CCharacter::Handle_MasterAsr(const net::msg::CmMasterAsrMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	short sRet = static_cast<short>(cmMsg.agree);
	std::string szName = cmMsg.name;
	DWORD dwCharID = static_cast<DWORD>(cmMsg.chaId);

	pMainCha->SetInvited(false);

	if (IsBoat()) {
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00023));
		return;
	}

	CCharacter* pSrcCha = pMainCha->GetSubMap()->FindCharacter(dwCharID, pMainCha->GetShape().centre);
	if (!pSrcCha) {
		//pMainCha->SystemNotice("%s !", szName);
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName.c_str());
		return;
	}

	if (pMainCha->GetLevel() < 41) {
		//pSrcCha->SystemNotice("!");
		pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
		return;
	}

	if (pSrcCha->GetLevel() > 40) {
		//pSrcCha->SystemNotice("!");
		pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
		return;
	}

	if (sRet == 0) {
		//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
		pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00026), pMainCha->GetName());
		return;
	}

	//  :   - (  )
	auto l_wpk = net::msg::serialize(net::msg::MpMasterCreateMessage{
		pSrcCha->GetName(), pSrcCha->GetPlayer()->GetDBChaId(),
		pMainCha->GetName(), pMainCha->GetPlayer()->GetDBChaId()
	});
	pMainCha->ReflectINFof(pMainCha, l_wpk);
}

void CCharacter::Handle_MasterDel(const net::msg::CmMasterDelMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	std::string szName = cmMsg.name;
	uLong ulChaID = static_cast<uLong>(cmMsg.chaId);

	if (pMainCha->GetLevel() > 40) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00027));
		return;
	}

	long lDelMoney = 0; //* pMainCha->GetLevel();
	if (!pMainCha->HasMoney(lDelMoney)) {
		//pMainCha->SystemNotice("!");
		pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00028));
		return;
	}
	//pMainCha->TakeMoney("", lDelMoney);
	//pMainCha->TakeMoney(RES_STRING(GM_CHARSCRIPT_CPP_00001), lDelMoney);
	pMainCha->SystemNotice("Your Mentor Deleted Successfully ");

	//  :  
	auto l_wpk = net::msg::serialize(net::msg::MpMasterDelMessage{
		pMainCha->GetName(), pMainCha->GetPlayer()->GetDBChaId(),
		std::string(szName), static_cast<int64_t>(ulChaID)
	});
	pMainCha->ReflectINFof(pMainCha, l_wpk);
}

void CCharacter::Handle_PrenticeDel(const net::msg::CmPrenticeDelMessage& cmMsg) {
	CCharacter* pMainCha = GetPlyMainCha();
	std::string szName = cmMsg.name;
	uLong ulChaID = static_cast<uLong>(cmMsg.chaId);

	//long lDelMoney = 10000 * pMainCha->GetLevel();
	//if(!pMainCha->HasMoney(lDelMoney))
	//{
	//	pMainCha->SystemNotice("!");
	//	break;
	//}
	//pMainCha->TakeMoney("", lDelMoney);
	long lCredit = (long)pMainCha->GetCredit(); //- 5 * pMainCha->GetLevel();
	if (lCredit < 0) {
		lCredit = 0;
	}
	pMainCha->SetCredit(lCredit);
	pMainCha->SynAttr(enumATTRSYN_TASK);
	//pMainCha->SystemNotice("!");
	pMainCha->SystemNotice("Your Disciple Deleted Successfully ");

	//  :  
	auto l_wpk = net::msg::serialize(net::msg::MpMasterDelMessage{
		std::string(szName), static_cast<int64_t>(ulChaID),
		pMainCha->GetName(), pMainCha->GetPlayer()->GetDBChaId()
	});
	pMainCha->ReflectINFof(pMainCha, l_wpk);
}

void CCharacter::BeginAction(const net::msg::CmBeginActionMessage& msg) {
	const long clPing = 300;

	if (!IsLiveing()) {
		return;
	}
	if (GetPlayer()->GetCtrlCha() == this && !GetSubMap()) {
		return;
	}

	uLong ulPacketId = static_cast<uLong>(msg.packetId);
	Char chActionType = static_cast<Char>(msg.actionType);


	m_ulPacketID = ulPacketId;
	switch (chActionType) {
	case enumACTION_MOVE: {
		if (!GetSubMap()) {
			return;
		}

		if (m_CAction.GetCurActionNo() >= 0) // 
		{
			FailedActionNoti(enumACTION_MOVE, enumFACTION_EXISTACT);
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
			break;
		}

		if (m_sPoseState == enumPoseSeat) {
			FailedActionNoti(enumACTION_MOVE, enumFACTION_EXISTACT);
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
			break;
		}
		ResetPosState();

		const auto& moveData = std::get<net::msg::CmActionMoveInputData>(msg.data);
		cChar* pData = moveData.pathData.data();
		uShort ulTurnNum = static_cast<uShort>(moveData.pathData.size());
		Point Path[defMOVE_INFLEXION_NUM];
		Char chPointNum;
		if (!pData) {
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00036));
			break;
		}
		if ((chPointNum = Char(ulTurnNum / sizeof(Point))) > defMOVE_INFLEXION_NUM) {
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
			//SystemNotice("%d%d\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00037), ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
			break;
		}
		memcpy(Path, pData, chPointNum * sizeof(Point));

		Cmd_BeginMove((Short)m_dwPing, Path, chPointNum);
	}
	break;
	case enumACTION_SKILL: {
		if (GetPlyMainCha()->m_CKitbag.IsLock()) {
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00038));
			FailedActionNoti(enumACTION_SKILL, enumFACTION_ACTFORBID);
			break;
		}

		if (!GetSubMap()) {
			return;
		}

		if (GetPlayer()->GetBankNpc()) {
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00039));
			FailedActionNoti(enumACTION_SKILL, enumFACTION_ACTFORBID);
			break;
		}

		if (m_CAction.GetCurActionNo() >= 0) // 
		{
			FailedActionNoti(enumACTION_SKILL, enumFACTION_EXISTACT);
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
			break;
		}

		if (m_sPoseState == enumPoseSeat) {
			FailedActionNoti(enumACTION_SKILL, enumFACTION_EXISTACT);
			//SystemNotice("\n");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
			break;
		}
		ResetPosState();

		const auto& skillData = std::get<net::msg::CmActionSkillInputData>(msg.data);
		char chMove = static_cast<char>(skillData.chMove);
		if (chMove == 2) // 
		{
			// 
			Point Path[defMOVE_INFLEXION_NUM];
			Char chPointNum;
			cChar* pData = skillData.pathData.data();
			uShort ulTurnNum = static_cast<uShort>(skillData.pathData.size());
			if (!pData) {
				FailedActionNoti(enumACTION_SKILL, enumFACTION_MOVEPATH);
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00036));
				break;
			}

			if ((chPointNum = Char(ulTurnNum / sizeof(Point))) > defMOVE_INFLEXION_NUM) {
				FailedActionNoti(enumACTION_SKILL, enumFACTION_MOVEPATH);
				//SystemNotice("%d%d\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00037), ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				break;
			}
			memcpy(Path, pData, chPointNum * sizeof(Point));
			// 
			dbc::uLong ulSkillID = static_cast<dbc::uLong>(skillData.skillId);
			Long lTarInfo1 = static_cast<Long>(skillData.tarInfo1);
			Long lTarInfo2 = static_cast<Long>(skillData.tarInfo2);

			CSkillRecord* pRec = GetSkillRecordInfo(ulSkillID);
			if (!pRec) {
				//LG( "", "%s1: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
				ToLogService("common", "character{}1skill inexistenceskill number: {}[PacketID: {}]", GetName(),
							 ulSkillID, ulPacketId);
				FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);
				//LG( "", "%s2: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
				ToLogService("common", "character{}2skill inexistenceskill number: {}[PacketID: {}]", GetName(),
							 ulSkillID, ulPacketId);
				//SystemNotice(": %d\n", ulSkillID);
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00040), ulSkillID);
				break;
			}
			Cmd_BeginSkill((Short)m_dwPing, Path, chPointNum, pRec, 1, lTarInfo1, lTarInfo2);
		}
		else {
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00041));
			break;
		}
	}
	break;
	case enumACTION_STOP_STATE: {
		if (!GetSubMap()) {
			return;
		}

		const auto& d = std::get<net::msg::CmActionStopStateData>(msg.data);
		Short sStateID = static_cast<Short>(d.stateId);

		CSkillStateRecord* pSSkillState = GetCSkillStateRecordInfo((uChar)sStateID);
		if (!pSSkillState)
			break;
		if (!pSSkillState->bCanCancel)
			break;
		DelSkillState((uChar)sStateID);
	}
	break;
	case enumACTION_LEAN: // 
	{
		if (!GetSubMap()) {
			return;
		}

		m_sPoseState = enumPoseLean;
		m_SSeat.chIsSeat = 0;

		m_SLean.ulPacketID = ulPacketId;
		const auto& d = std::get<net::msg::CmActionLeanData>(msg.data);
		m_SLean.lPose = static_cast<Long>(d.pose);
		m_SLean.lAngle = static_cast<Long>(d.angle);
		m_SLean.lPosX = static_cast<Long>(d.posX);
		m_SLean.lPosY = static_cast<Long>(d.posY);
		m_SLean.lHeight = static_cast<Long>(d.height);
		m_SLean.chState = 0;

		//  : /  std::variant
		{
			net::msg::McCharacterActionMessage actionMsg;
			actionMsg.worldId = m_ID;
			actionMsg.packetId = m_SLean.ulPacketID;
			actionMsg.actionType = net::msg::ActionType::LEAN;
			actionMsg.data = net::msg::ActionLeanData{
				m_SLean.chState, m_SLean.lPose, m_SLean.lAngle,
				m_SLean.lPosX, m_SLean.lPosY, m_SLean.lHeight
			};
			auto WtPk = net::msg::serialize(actionMsg);
			NotiChgToEyeshot(WtPk);
		}
		//

		// log
		//
	}
	break;
	///item picks
	case enumACTION_ITEM_PICK: // 
	{
		const auto& d = std::get<net::msg::CmActionItemPickData>(msg.data);
		Long lWorldID = static_cast<Long>(d.worldId);
		Long lHandle = static_cast<Long>(d.handle);

		Short sRet = Cmd_PickupItem(lWorldID, lHandle);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_THROW: // 
	{
		const auto& d = std::get<net::msg::CmActionItemThrowData>(msg.data);
		Short sGridID = static_cast<Short>(d.gridId);
		Short sNum = static_cast<Short>(d.num);
		Long lPosX = static_cast<Long>(d.posX);
		Long lPosY = static_cast<Long>(d.posY);

		Short sRet = Cmd_ThrowItem(0, sGridID, &sNum, lPosX, lPosY);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_USE: // 
	{
		const auto& d = std::get<net::msg::CmActionItemUseData>(msg.data);
		Short sFromGridID = static_cast<Short>(d.fromGridId);
		Short sToGridID = static_cast<Short>(d.toGridId);

		Short sRet = Cmd_UseItem(0, sFromGridID, 0, sToGridID);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_UNFIX: // 
	{
		m_CChaAttr.ResetChangeFlag();

		Char chDir;
		Long lParam1, lParam2;

		const auto& d = std::get<net::msg::CmActionItemUnfixData>(msg.data);
		Char chLinkID = static_cast<Char>(d.linkId);
		Short sGridID = static_cast<Short>(d.gridId);
		if (sGridID == -2) // 
		{
			chDir = 0;
			lParam1 = static_cast<Long>(d.param1);
			lParam2 = static_cast<Long>(d.param2);
		}
		else if (sGridID == -1) // 
		{
			chDir = 1;
			lParam1 = 0;
			lParam2 = -1;
		}
		else if (sGridID >= 0) // 
		{
			chDir = 1;
			lParam1 = 0;
			lParam2 = sGridID;
		}


		Short sUnfixNum = 0;
		Short sRet = Cmd_UnfixItem(chLinkID, &sUnfixNum, chDir, lParam1, lParam2);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_POS: // 
	{
		const auto& d = std::get<net::msg::CmActionItemPosData>(msg.data);
		Short sSrcGrid = static_cast<Short>(d.srcGrid);
		Short sSrcNum = static_cast<Short>(d.srcNum);
		Short sTarGrid = static_cast<Short>(d.tarGrid);

		Short sRet = Cmd_ItemSwitchPos(0, sSrcGrid, sSrcNum, sTarGrid);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_KITBAGTMP_DRAG: //
	{
		const auto& d = std::get<net::msg::CmActionItemPosData>(msg.data);
		Short sSrcGrid = static_cast<Short>(d.srcGrid);
		Short sSrcNum = static_cast<Short>(d.srcNum);
		Short sTarGrid = static_cast<Short>(d.tarGrid);

		Short sRet = Cmd_DragItem(sSrcGrid, sSrcNum, sTarGrid);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_DELETE: // 
	{
		const auto& d = std::get<net::msg::CmActionItemDeleteData>(msg.data);
		Short sFromGridID = static_cast<Short>(d.fromGridId);

		Short sOptNum = 0;
		Short sRet = Cmd_DelItem(0, sFromGridID, &sOptNum);
		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_ITEM_INFO: // 
	{
		const auto& d = std::get<net::msg::CmActionViewItemData>(msg.data);
		ViewItemInfo(d);
	}
	break;
	case enumACTION_BANK: {
		const auto& d = std::get<net::msg::CmActionBankData>(msg.data);
		Char chSrcType = static_cast<Char>(d.srcType);
		Short sSrcGrid = static_cast<Short>(d.srcGrid);
		Short sSrcNum = static_cast<Short>(d.srcNum);
		Char chTarType = static_cast<Char>(d.tarType);
		Short sTarGrid = static_cast<Short>(d.tarGrid);
		Short sRet;

		sRet = Cmd_BankOper(chSrcType, sSrcGrid, sSrcNum, chTarType, sTarGrid);

		if (sRet != enumITEMOPT_SUCCESS)
			ItemOprateFailed(sRet);
	}
	break;
	case enumACTION_CLOSE_BANK: {
		GetPlayer()->CloseBank();
	}
	break;
	case enumACTION_REQUESTGUILDBANK: {
		if (GetGuildID() == 0) {
			return;
		}
		GetPlayer()->OpenGuildBank();
		GetPlayer()->GetGuildGold();
		break;
	}
	case enumACTION_UPDATEGUILDLOGS: {
		int guildID = GetGuildID();
		if (guildID == 0) {
			return;
		}

		std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);
		net::msg::McUpdateGuildLogsMessage logsMsg;
		logsMsg.worldId = m_ID;
		logsMsg.packetId = ulPacketId;
		logsMsg.totalSize = logs.size();
		for (int i = 1; i <= 13; i++) {
			if (i > (int)logs.size()) {
				logsMsg.terminated = true;
				break;
			}
			net::msg::GuildBankLogEntry e;
			e.type = logs.at(logs.size() - i).type;
			e.time = logs.at(logs.size() - i).time;
			e.parameter = logs.at(logs.size() - i).parameter;
			e.quantity = logs.at(logs.size() - i).quantity;
			e.userId = logs.at(logs.size() - i).userID;
			logsMsg.logs.push_back(e);
		}
		auto WtPk = net::msg::serialize(logsMsg);

		ReflectINFof(this, WtPk);
		break;
	}
	case enumACTION_REQUESTGUILDLOGS: {
		int guildID = GetGuildID();
		if (guildID == 0) {
			return;
		}
		std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);

		const auto& reqData = std::get<net::msg::CmActionRequestGuildLogsData>(msg.data);
		uShort curSize = static_cast<uShort>(reqData.curSize);

		net::msg::McRequestGuildLogsMessage respMsg;
		respMsg.worldId = m_ID;
		respMsg.packetId = ulPacketId;
		for (int i = 1; i <= 13; i++) {
			if ((int)(curSize + i) > (int)logs.size()) {
				respMsg.terminated = true;
				break;
			}
			net::msg::GuildBankLogEntry e;
			e.type = logs.at(logs.size() - curSize - i).type;
			e.time = logs.at(logs.size() - curSize - i).time;
			e.parameter = logs.at(logs.size() - curSize - i).parameter;
			e.quantity = logs.at(logs.size() - curSize - i).quantity;
			e.userId = logs.at(logs.size() - curSize - i).userID;
			respMsg.logs.push_back(e);
		}
		auto WtPk = net::msg::serialize(respMsg);

		ReflectINFof(this, WtPk);
		break;
	}
	case enumACTION_SHORTCUT: {
		const auto& d = std::get<net::msg::CmActionShortcutData>(msg.data);
		char chIndex = static_cast<char>(d.index);
		char chType = static_cast<char>(d.type);
		short sGrid = static_cast<short>(d.grid);

		if (chIndex < 0 || chIndex >= SHORT_CUT_NUM)
			break;
		m_CShortcut.chType[chIndex] = chType;
		m_CShortcut.byGridID[chIndex] = sGrid;
	}
	break;
	case enumACTION_LOOK: {
		//m_SChaPart.sTypeID = pk.ReadInt64();
		//for (int i = 0; i < enumEQUIP_NUM; i++)
		//	m_SChaPart.SLink[i].sID = pk.ReadInt64();

		//// 
		//net::WPacket WtPk	=g_gmsvr->GetWPacket();
		//WtPk.WriteCmd(CMD_MC_NOTIACTION);	//
		//WtPk.WriteInt64(m_ID);
		//WtPk.WriteInt64(ulPacketId);
		//WtPk.WriteInt64(enumACTION_LOOK);
		//WtPk.WriteInt64(m_SChaPart.sTypeID);
		//for (int i = 0; i < enumEQUIP_NUM; i++)
		//	WtPk.WriteInt64(m_SChaPart.sLink[i]);
		//NotiChgToEyeshot(WtPk);//
	}
	break;
	case enumACTION_TEMP: {
		const auto& d = std::get<net::msg::CmActionTempData>(msg.data);
		m_STempChaPart.sItemID = static_cast<short>(d.itemId);
		m_STempChaPart.sPartID = static_cast<short>(d.partId);

		//  :     std::variant
		{
			net::msg::McCharacterActionMessage actionMsg;
			actionMsg.worldId = m_ID;
			actionMsg.packetId = ulPacketId;
			actionMsg.actionType = net::msg::ActionType::TEMP;
			actionMsg.data = net::msg::ActionTempData{
				static_cast<int64_t>(m_STempChaPart.sItemID),
				static_cast<int64_t>(m_STempChaPart.sPartID)
			};
			auto WtPk = net::msg::serialize(actionMsg);
			NotiChgToEyeshot(WtPk);
		}
	}
	break;
	case enumACTION_EVENT: {
		const auto& d = std::get<net::msg::CmActionEventData>(msg.data);
		Long lID = static_cast<Long>(d.worldId);
		Long lHandle = static_cast<Long>(d.handle);
		Entity* pCObj = g_pGameApp->IsLiveingEntity(lID, lHandle);
		if (!pCObj) {
			break;
		}
		uShort usEventID = static_cast<uShort>(d.eventId);
		ExecuteEvent(pCObj, usEventID);
	}
	break;
	case enumACTION_FACE: {
		const auto& d = std::get<net::msg::CmActionFaceData>(msg.data);
		Short sAngle = static_cast<Short>(d.angle);
		Short sPose = static_cast<Short>(d.pose);

		//  :    std::variant
		{
			net::msg::McCharacterActionMessage actionMsg;
			actionMsg.worldId = m_ID;
			actionMsg.packetId = ulPacketId;
			actionMsg.actionType = net::msg::ActionType::FACE;
			actionMsg.data = net::msg::ActionFaceData{static_cast<int64_t>(sAngle), static_cast<int64_t>(sPose)};
			auto WtPk = net::msg::serialize(actionMsg);
			NotiChgToEyeshot(WtPk);
		}
	}
	break;
	case enumACTION_SKILL_POSE: {
		if (!GetSubMap()) {
			return;
		}

		if (IsBoat())
			break;
		if (GetMoveState() == enumMSTATE_ON || GetFightState() == enumFSTATE_ON || !GetActControl(enumACTCONTROL_MOVE))
			break;

		const auto& d = std::get<net::msg::CmActionFaceData>(msg.data);
		Short sAngle = static_cast<Short>(d.angle);
		Short sPose = static_cast<Short>(d.pose);

		//  :    std::variant
		net::msg::McCharacterActionMessage actionMsg;
		actionMsg.worldId = m_ID;
		actionMsg.packetId = ulPacketId;
		actionMsg.actionType = net::msg::ActionType::SKILL_POSE;
		actionMsg.data = net::msg::ActionFaceData{static_cast<int64_t>(sAngle), static_cast<int64_t>(sPose)};
		auto WtPk = net::msg::serialize(actionMsg);
		NotiChgToEyeshot(WtPk);

		bool bToSeat = g_IsSeatPose(sPose);
		if ((bToSeat && m_SSeat.chIsSeat) || (!bToSeat && !m_SSeat.chIsSeat))
			break;

		// 
		dbc::uLong ulSkillID = 202;
		CSkillRecord* pCSkill = GetSkillRecordInfo(ulSkillID);
		if (!pCSkill) {
			break;
		}

		if (bToSeat) // 
		{
			m_SSeat.chIsSeat = 1;
			m_SSeat.sAngle = sAngle;
			m_SSeat.sPose = sPose;
			g_luaAPI.Call(pCSkill->szActive, static_cast<CCharacter*>(this), 1);
		}
		else // 
		{
			m_SSeat.chIsSeat = 0;
			g_luaAPI.Call(pCSkill->szInactive, static_cast<CCharacter*>(this), 1);
		}
		if (bToSeat)
			m_sPoseState = enumPoseSeat;
		else
			m_sPoseState = enumPoseStand;
	}
	break;
	case enumACTION_PK_CTRL: {
		const auto& d = std::get<net::msg::CmActionPkCtrlData>(msg.data);
		if (d.ctrl)
			Cmd_SetInPK();
		else
			Cmd_SetInPK(false);
		SynPKCtrl();
	}
	break;
	default:
		break;
	}
}


//  : 
void CCharacter::Cmd_ChangeHair(net::RPacket& pk) {
	char szRes[128];

	net::msg::CmUpdateHairMessage hairMsg;
	net::msg::deserialize(pk, hairMsg);
	short sScriptID = static_cast<short>(hairMsg.scriptId);
	int64_t gridLocs[4] = {hairMsg.gridLoc0, hairMsg.gridLoc1, hairMsg.gridLoc2, hairMsg.gridLoc3};

	TradeAction(false);
	HairAction(false);

	if (sScriptID == 0) {
		return;
	}

	if (m_CKitbag.IsPwdLocked()) {
		sprintf(szRes, RES_STRING(GM_CHARACTERPRL_CPP_00042));
		Prl_ChangeHairResult(0, szRes);
		return;
	}

	CHairRecord* pHair = GetHairRecordInfo(sScriptID);
	if (!pHair) {
		sprintf(szRes, RES_STRING(GM_CHARACTERPRL_CPP_00043), sScriptID);
		Prl_ChangeHairResult(0, szRes);
		return;
	}

	short sValidCnt = 0;
	short sValidGrid[defHAIR_MAX_ITEM][3];
	int gridIdx = 0;

	for (short i = 0; i < defHAIR_MAX_ITEM; i++) {
		short sNeedItemID = (short)(pHair->dwNeedItem[i][0]);
		if (sNeedItemID > 0) {
			BOOL bOK = TRUE;
			short sGridLoc = (gridIdx < 4) ? static_cast<short>(gridLocs[gridIdx++]) : -1;
			if (sGridLoc == -1) bOK = FALSE;

			if (bOK) {
				// 
				short sNowItemID = m_CKitbag.GetID(sGridLoc);
				if (sNowItemID != sNeedItemID) {
					bOK = FALSE;
				}
			}

			if (!bOK) {
				sprintf(szRes, RES_STRING(GM_CHARACTERPRL_CPP_00044));
				Prl_ChangeHairResult(0, szRes);
				return;
			}
			sValidGrid[sValidCnt][0] = sGridLoc;
			sValidGrid[sValidCnt][1] = sNeedItemID;
			sValidGrid[sValidCnt][2] = (short)(pHair->dwNeedItem[i][1]); // 
			sValidCnt++;
		}
	}


	// , 
	m_CKitbag.SetChangeFlag(false);
	/*if(!TakeMoney("", pHair->dwMoney))
	{
		SystemNotice(", !");
		return;
	}*/
	if (!TakeMoney(RES_STRING(GM_CHARACTERPRL_CPP_00045), pHair->dwMoney)) {
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00046));
		return;
	}

	SItemGrid item;
	for (short i = 0; i < sValidCnt; i++) {
		item.sID = sValidGrid[i][1];
		item.sNum = sValidGrid[i][2];

		short sRet = KbPopItem(true, false, &item, sValidGrid[i][0]);
		if (sRet != enumKBACT_SUCCESS) {
			//SystemNotice(", !");

			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00047));
			return;
		}
	}

	// 
	SynKitbagNew(enumSYN_KITBAG_FROM_NPC);

	// , 

	SetLookChangeFlag(true);
	// 10%
	if (rand() % 100 < 10 && pHair->GetFailItemNum() > 0) {
		int nRandFail = rand() % pHair->GetFailItemNum();
		short sFailHair = (short)(pHair->dwFailItemID[nRandFail]);
		m_SChaPart.sHairID = sFailHair;
		//SystemNotice(", !");
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00048));
		Prl_ChangeHairResult(sScriptID, "fail", true);
	}
	else {
		// , 
		m_SChaPart.sHairID = (short)(pHair->dwItemID); // 
		Prl_ChangeHairResult(sScriptID, "ok", true);
	}

	// 
	if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
		SynLook(LOOK_SELF, true); // sync to self (changing hair)
	else
		SynLook();
}

// 
// 1 : ID, 0
// 2 : 
void CCharacter::Prl_ChangeHairResult(int nScriptID, const char* szReason, BOOL bNoticeAll) {
	//  :   
	auto wpk = net::msg::serialize(net::msg::McUpdateHairResMessage{
		GetID(), static_cast<int64_t>(nScriptID), szReason
	});
	if (bNoticeAll) {
		NotiChgToEyeshot(wpk);
	}
	else {
		ReflectINFof(this, wpk);
	}
}

// 
void CCharacter::Prl_OpenHair() {
	HairAction(true);

	//  :  UI 
	auto wpk = net::msg::serializeMcOpenHairCutCmd();
	ReflectINFof(this, wpk);
}
