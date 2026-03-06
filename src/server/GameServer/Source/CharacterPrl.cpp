#include "stdafx.h"
#include "SubMap.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "CharTrade.h"
#include "Parser.h"
#include "NPC.h"
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

_DBC_USING

const short g_sLiveSkillNeedItemNum[4] = {6,4,6,6};
extern std::string g_strLogName;
//----------------------------------------------------------
//                    
//----------------------------------------------------------
void CCharacter::ProcessPacket(unsigned short usCmd, RPACKET pk)
{T_B
	switch (usCmd)
	{
	case CMD_CM_RANK:{
		DWORD COOLDOWN = GetTickCount();
		if (ShowRankColD > COOLDOWN)
		{
			BickerNotice("Please Calm Down Don't Spam! ");
			return;
		}		
		game_db.ShowExpRank(GetPlyMainCha(), 50);
		break;
	}
	case CMD_CM_STALLSEARCH:{
		Long	itemID = READ_LONG(pk);
		g_StallSystem.SearchItem(*this, itemID);
		break;
	}
	case CMD_PM_GUILDBANK:{
		Char bankType = READ_CHAR(pk);

		//add cooldown for guild banks push to avoid otherpeople push conflict 
		if (const DWORD COOLDOWN = GetTickCount(); GetPlyMainCha()->GuildBankCD > COOLDOWN) {
			BickerNotice("Please calm down Don't spam! %ds left!", (GetPlyMainCha()->GuildBankCD - COOLDOWN) / 1000);
		}
		else if (!IsLiveing()) {
			SystemNotice("Dead pirates are unable to trade.");
		}
		else if (GetPlyCtrlCha()->IsBoat()) // check if player at sea
		{
			SystemNotice("Must be on land to use the guild bank.");
		}
		else if (!IsInArea(2)) {
			SystemNotice("Must be in safe zone to use the guild bank.");
		}
		else{
			const int cdtime = 3000;
			GetPlyMainCha()->GuildBankCD = COOLDOWN + cdtime; // guildbank cd
			switch (bankType){

				case 0:{ //bankoper
					Char	chSrcType = READ_CHAR(pk);
					Short	sSrcGrid = READ_SHORT(pk);
					Short	sSrcNum = READ_SHORT(pk);
					Char	chTarType = READ_CHAR(pk);
					Short	sTarGrid = READ_SHORT(pk);
					Short sRet;
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
					sRet = Cmd_GuildBankOper(chSrcType, sSrcGrid, sSrcNum, chTarType, sTarGrid);
					if (sRet != enumITEMOPT_SUCCESS || !game_db.SetGuildLog(logs, guildID)){
						ItemOprateFailed(sRet);
					}
	
					break;
				}

				case 1:{ //withdraw/deposit gold
					Char action = READ_CHAR(pk);

					int guildID = GetGuildID();
					int gold = READ_LONG(pk);
					int currentgold = getAttr(ATTR_GD);
					unsigned long long guildGold = game_db.GetGuildBankGold(guildID);
					
					unsigned long long maxGuildGold = 9223372036854775807LL;
					int maxCharGold = 2000000000;
					std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);
				
					
					int canTake = (emGldPermTakeBank&guildPermission);
					int canGive = (emGldPermDepoBank&guildPermission);

					CTableGuild::BankLog l;
					
					if (action == 0 && canTake == emGldPermTakeBank){ //withdraw
						l.type = 0;			// Withdraw

						//make sure we dont cause gold overflow.
						if (gold + currentgold >maxCharGold){
							gold = maxCharGold - currentgold;
						}
						//make sure we cant withdraw more than is in bank.
						if (gold > guildGold){
							gold = guildGold;
						}
						//we dont want to do redundant transactions.
						if (gold < 1){
							break;
						}
					}else if (action == 1 && canGive == emGldPermDepoBank){ //deposit
						l.type = 1; // deposit
						//check player has that much gold
						//if not, then set gold to whatever they have.
						if (gold > currentgold){
							gold = currentgold;
						}
						//check to see if guild is at max gold already.
						//make sure we dont cause gold overflow.
						if (gold + guildGold > maxGuildGold){	
							gold = maxGuildGold - guildGold;
						}

						//we dont want to do redundant transactions.
						if (gold < 1){
							break;
						}
						gold = 0 - gold;
					}
					else{
						break;
					}

					if (game_db.UpdateGuildBankGold(guildID, -gold)){
						l.time = time(0);
						l.parameter = gold > 0 ? gold : -gold;
						l.quantity = 0;
						l.userID = GetPlyMainCha()->m_ID;
						
						logs.push_back(l);
						if (game_db.SetGuildLog(logs, guildID)) {
							setAttr(ATTR_GD, currentgold + gold);
							SynAttr(enumATTRSYN_TRADE);
							SyncBoatAttr(enumATTRSYN_TRADE);

							//send update packet to let other members of guild see the update.

							WPACKET WtPk = GETWPACKET();
							WRITE_CMD(WtPk, CMD_MM_UPDATEGUILDBANKGOLD);
							WRITE_LONG(WtPk, GetPlyMainCha()->m_ID);
							WRITE_LONG(WtPk, GetPlyMainCha()->GetGuildID());
							ReflectINFof(this, WtPk);
						}
					}
					break;
				}
			}
		}

		//let group know we have finished, so the next guild bank packet can be processed.
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MP_GUILDBANK);
		WRITE_LONG(WtPk, GetGuildID());
		ReflectINFof(this, WtPk);
		break;
	}

	case CMD_PM_PUSHTOGUILDBANK:{
		int guildID = GetGuildID();
		if (guildID == 0){
			return;
		}
		CKitbag	pCSrcBag;
		game_db.GetGuildBank(guildID, &pCSrcBag);
		pCSrcBag.SetChangeFlag(false);

		SItemGrid SPopItem;
		const char* strItem = READ_STRING(pk);
		String2Item(strItem, &SPopItem);

		short sSrcGridID = defKITBAG_DEFPUSH_POS;
		if (pCSrcBag.Push(&SPopItem, sSrcGridID) == enumKBACT_ERROR_FULL){
			//drop item next to player?
		}else{
			GetPlayer()->SynGuildBank(&pCSrcBag, enumSYN_KITBAG_BANK);
			GetPlayer()->SetBankSaveFlag(0);
			game_db.UpdateGuildBank(guildID, &pCSrcBag);
		}
		//let group know we have finished, so the next guild bank packet can be processed.
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MP_GUILDBANK);
		WRITE_LONG(WtPk, guildID);
		ReflectINFof(this, WtPk);
		break;
	}

	case CMD_CM_PING:
		{
			uLong	ulPing = GetTickCount() - READ_LONG(pk);
			Long	lGateSvr = READ_LONG(pk);
			Long	lSrcID = READ_LONG(pk);
			Long	lGatePlayerID = READ_LONG(pk);
			Long	lGatePlayerAddr = READ_LONG(pk);

			// 
			BEGINGETGATE();
			GateServer	*pNoGate;
			GateServer	*pGate = 0;
			while (pNoGate = GETNEXTGATE())
			{
				if (ToAddress(pNoGate) == lGateSvr)
				{
					pGate = pNoGate;
					break;
				}
			}
			if (!pGate)
				break;
			//

			WPACKET WtPk	=GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_QUERY_CHAPING);
			WRITE_LONG(WtPk, lSrcID);
			WRITE_STRING(WtPk, GetName());
			WRITE_STRING(WtPk, GetSubMap()->GetName());
			WRITE_LONG(WtPk, ulPing);
			WRITE_LONG(WtPk, lGatePlayerID);
			WRITE_LONG(WtPk, lGatePlayerAddr);
			WRITE_SHORT(WtPk, 1);
			pGate->SendData(WtPk);

			break;
		}
	case CMD_CM_CHECK_PING:
		{
			DWORD	dwPing = GetTickCount() - m_dwPingSendTick;
			/*if (m_dwPingRec[0] == 0)
			{
				for (int i = 0; i < defPING_RECORD_NUM; i++)
					m_dwPingRec[i] = dwPing;
				m_dwPing = dwPing;
			}
			else
			{
				DWORD	dwAddPing = 0;
				for (int i = 1; i < defPING_RECORD_NUM; i++)
				{
					m_dwPingRec[i - 1] = m_dwPingRec[i];
					dwAddPing += m_dwPingRec[i];
				}
				m_dwPingRec[defPING_RECORD_NUM - 1] = dwPing;
				dwAddPing += dwPing;
				m_dwPing = dwAddPing / defPING_RECORD_NUM;
			}*/
			m_dwPing = dwPing;
			//printf("ping = %d [%s]\n", m_dwPing, GetName());
			SendPreMoveTime();
			break;
		}
	case CMD_CM_CANCELEXIT:
		{
			CancelExit();
		}
		break;
	case CMD_CM_BEGINACTION:
		{
			uLong	ulWorldID = READ_LONG(pk);
			
			if(GetPlayer())
			{
				
				if (GetPlayer()->GetCtrlCha() && ulWorldID == GetPlayer()->GetCtrlCha()->GetID())
					GetPlayer()->GetCtrlCha()->BeginAction(pk);
				else if (GetPlayer()->GetMainCha() && ulWorldID == GetPlayer()->GetMainCha()->GetID())
					GetPlayer()->GetMainCha()->BeginAction(pk);
			}
			break;
		}
	case CMD_CM_ENDACTION:
		{
			EndAction(pk);

			break;
		}
	case CMD_CM_DIE_RETURN:
		{
			m_chSelRelive = READ_CHAR(pk);
			GetPlyMainCha()->ResetChaRelive();	// 
			if (m_chSelRelive == enumEPLAYER_RELIVE_NORIGIN)
				SetRelive(enumEPLAYER_RELIVE_ORIGIN, 0);
			break;
		}
	case CMD_CM_SAY:
		{
			DWORD	dwNowTick = GetTickCount();
			if (dwNowTick - _dwLastSayTick < (DWORD)g_Config.m_lSayInterval)
			{
				//SystemNotice("");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00001));
				break;
			}
			_dwLastSayTick = dwNowTick;

			if (!GetSubMap())
			{
				//LG("", "%s \n", m_CLog.GetLogName());
				LG("dialog error", "when character%s is dialogthe map is null\n", m_CLog.GetLogName());
				break;
			}
			uShort	l_retlen;
			cChar	*l_content = READ_SEQ(pk, l_retlen);
			if (!l_content)
				break;
			else if (*l_content == '&') // 
			{
				Char chGMLv = GetPlayer()->GetGMLev();
				if (chGMLv == 0 || chGMLv > 150)
					//SystemNotice("\n");
					SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00002));
				else
					DoCommand(l_content + 1, l_retlen - 1);
			}
			else if (*l_content == '$' && *(l_content + 1) == '$') // 
			{
				DoCommand_CheckStatus(l_content + 3, l_retlen - 2);
			}

			/*else if (*l_content == '/' && *(l_content+1)=='?') // 
			{
				HandleHelp(l_content + 2, l_retlen - 2);
			}*/
			else
			{
				
				// kong@pkodev.net 09.22.2017
				g_CParser.DoString("HandleChat", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_STRING, 1, l_content, DOSTRING_PARAM_END);
				if(!g_CParser.GetReturnNumber(0))
					break;
				if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
				{
					SystemNotice("Unable to chat in this map!");
					break;
				}

				WPACKET wpk	= GETWPACKET();
				WRITE_CMD(wpk, CMD_MC_SAY);
				WRITE_LONG(wpk, m_ID);
				WRITE_SEQ(wpk, l_content,l_retlen);
				WRITE_LONG(wpk, chatColour);
				NotiChgToEyeshot(wpk);
			}
			break;
		}
	case CMD_CM_REQUESTTALK:
	case CMD_CM_REQUESTTRADE:{

			if (GetTradeData() || GetBoat() || GetStallData() || !GetActControl(enumACTCONTROL_TALKTO_NPC) || m_CKitbag.IsLock() || !GetActControl(enumACTCONTROL_ITEM_OPT)){
				return;
			}

			uLong ulID = READ_LONG(pk);
			if( ulID == mission::g_WorldEudemon.GetID() )
			{
				mission::g_WorldEudemon.MsgProc( *this, pk );
				break;
			}
			CCharacter* pCha = m_submap->FindCharacter( ulID, GetShape().centre );
			if( pCha == NULL ) break;
			mission::CNpc* pNpc = pCha->IsNpc();
			if( pNpc ){
				pNpc->MsgProc( *this, pk );
				break;
			}else{
				//g_CParser.DoString("extNpcNpcProc", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, this,pCha,pk, DOSTRING_PARAM_END);
			
				lua_getglobal(g_pLuaState, "extNpcNpcProc");
				if (!lua_isfunction(g_pLuaState, -1))
				{
					lua_pop(g_pLuaState, 1);
					break;
				}
			
				lua_pushlightuserdata(g_pLuaState, (void*)this);
				lua_pushlightuserdata(g_pLuaState, (void*)pCha);
				lua_pushlightuserdata(g_pLuaState, (void*)&pk);
				
				int nStatus = lua_pcall(g_pLuaState, 3, 0, 0);
				lua_settop(g_pLuaState, 0);
			}
		}
		break;
		//daily buff request to open ui
	case CMD_CM_DailyBuffRequest: {
		CCharacter* pCMainCha = GetPlyMainCha();
		CPlayer* pCPly = GetPlayer();
		if (pCMainCha->m_CKitbag.IsLock() || pCMainCha->m_CKitbag.IsPwdLocked() || pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData()) {
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

		g_CParser.DoString("DailyBuffRequest", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
		break;
	}
	case CMD_CM_MISLOG:
		{
			MisLog();
		}
		break;
	case CMD_CM_MISLOGINFO:
		{
			WORD wMisID  = READ_SHORT(pk);
			MisLogInfo( wMisID );
		}
		break;
	case CMD_CM_MISLOG_CLEAR:
		{
			WORD wMisID  = READ_SHORT(pk);
			MisLogClear( wMisID );
		}
		break;
	case CMD_CM_FORGE:
		{
			BYTE byIndex = READ_CHAR(pk);
			g_ForgeSystem.ForgeItem( *this, byIndex );
		}
		break;
	case CMD_CM_CHARTRADE_REQUEST:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			g_TradeSystem.Request( byType, *this, dwCharID );
		}
		break;
	case CMD_CM_CHARTRADE_ACCEPT:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			g_TradeSystem.Accept( byType, *this, dwCharID );
		}
		break;
	case CMD_CM_CHARTRADE_REJECT:
		{
		}
		break;
	case CMD_CM_CHARTRADE_CANCEL:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			g_TradeSystem.Cancel( byType, *this, dwCharID );
		}
		break;
	case CMD_CM_CHARTRADE_ITEM:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			BYTE  byOpType = READ_CHAR(pk);
			BYTE  byIndex  = READ_CHAR(pk);
			BYTE  byItemIndex = READ_CHAR(pk);
			BYTE  byCount  = READ_CHAR(pk);
			g_TradeSystem.AddItem( byType, *this, dwCharID, byOpType, byIndex, byItemIndex, byCount );
		}
		break;
	case CMD_CM_CHARTRADE_MONEY:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			BYTE  byOpType = READ_CHAR(pk);
			BYTE currency = READ_CHAR(pk);
			DWORD dwMondy  = READ_LONG(pk);
			
			if (currency == 0){
				//gold
				g_TradeSystem.AddMoney(byType, *this, dwCharID, byOpType, dwMondy);
			}else if (currency == 1){
				//IMPS
				g_TradeSystem.AddIMP(byType, *this, dwCharID, byOpType, dwMondy);
			}
			
		}
		break;
	case CMD_CM_CHARTRADE_VALIDATEDATA:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			g_TradeSystem.ValidateItemData( byType, *this, dwCharID );
		}
		break;
	case CMD_CM_CHARTRADE_VALIDATE:
		{
			BYTE byType = READ_CHAR(pk);
			DWORD dwCharID = READ_LONG(pk);
			g_TradeSystem.ValidateTrade( byType, *this, dwCharID );
		}
		break;
	case CMD_CM_CREATE_BOAT:
		{
			g_CharBoat.MakeBoat( *this, pk );
		}
		break;
	case CMD_CM_UPDATEBOAT_PART:
		{
			g_CharBoat.Update( *this, pk );
		}
		break;
	case CMD_CM_BOAT_GETINFO:
		{
			if( GetPlayer()->IsLuanchOut() )
			{
				g_CharBoat.GetBoatInfo( *this, GetPlayer()->GetLuanchID() );
			}
			else
			{
				//SystemNotice( "" );
				SystemNotice( RES_STRING(GM_CHARACTERPRL_CPP_00003) );
			}
		}
		break;
	case CMD_CM_BOAT_CANCEL:
		{
			g_CharBoat.Cancel( *this );
		}
		break;
	case CMD_CM_BOAT_LUANCH:
		{
			DWORD dwNpcID = READ_LONG(pk);
			CCharacter* pCha = m_submap->FindCharacter(dwNpcID, GetShape().centre);
			if (pCha == NULL){
				break;
			}else if (GetPlayer()->GetBankNpc()){
				break;
			}else if (g_CParser.DoString("IsSailNpc", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END)) {
				if (!g_CParser.GetReturnNumber(0)){
					break;
				}
			}

			BYTE byIndex = READ_CHAR( pk );
			BoatSelLuanch( byIndex );
		}
		break;
	case CMD_CM_BOAT_SELECT:
		{
			DWORD dwNpcID = READ_LONG( pk );
			CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
			if (pCha == NULL){
				break;
			}
			if (g_CParser.DoString("IsSailBoatNpc", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END)) {
				if (!g_CParser.GetReturnNumber(0)){
					break;
				}
			}
			BYTE byType = READ_CHAR( pk );
			BYTE byIndex = READ_CHAR( pk );
			BoatSelected( byType, byIndex );
		}
		break;
	case CMD_CM_BOAT_BAGSEL:
		{
			DWORD dwNpcID = READ_LONG( pk );
			if(dwNpcID)
			{
				CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
				if( pCha == NULL )
					break;
			}

			BYTE byIndex = READ_CHAR( pk );
			BoatPackBag( byIndex );
		}
		break;
	case CMD_CM_ENTITY_EVENT:
		{
			DWORD dwEntityID = READ_LONG( pk );
			CCharacter* pCha = m_submap->FindCharacter( dwEntityID, GetShape().centre );
			if( pCha == NULL ) break;
			mission::CEventEntity* pEntity = pCha->IsEvent();
			if( pEntity )
			{
				pEntity->MsgProc( *this, pk );
				break;
			}
		}
		break;
	case CMD_CM_STALL_ALLDATA:
		{
			g_StallSystem.StartStall( *this, pk );
		}
		break;
	case CMD_CM_STALL_OPEN:
		{
			g_StallSystem.OpenStall( *this, pk );
		}
		break;
	case CMD_CM_STALL_BUY:
		{
			g_StallSystem.BuyGoods( *this, pk );
		}
		break;
	case CMD_CM_STALL_CLOSE:
		{
			g_StallSystem.CloseStall( *this );
		}
		break;
	case CMD_CM_READBOOK_START:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			if(!IsBoat())
			{
				pMainCha->SetReadBookState(true);
				pMainCha->ForgeAction(true);
				pMainCha->m_CKitbag.Lock();
			}
			else
				//pMainCha->SystemNotice("");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00004));
			
		}
		break;
	case CMD_CM_READBOOK_CLOSE:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			if(!IsBoat())
			{
				pMainCha->SetReadBookState(false);
				pMainCha->ForgeAction(false);
				pMainCha->m_CKitbag.UnLock();
			}
			else
				//pMainCha->SystemNotice("");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00005));
		}
		break;
	case CMD_CM_SYNATTR: // 
		{	
			GetPlayer()->GetMainCha()->Cmd_ReassignAttr(pk);
		}
		break;
	case CMD_CM_SKILLUPGRADE:
		{
			Short	sSkillID = READ_SHORT(pk);
			Char	chAddGrade = READ_CHAR(pk);

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
	case CMD_CM_REFRESH_DATA:
		{
			Long	lWorldID = READ_LONG(pk);
			Long	lHandle = READ_LONG(pk);
			Entity	*pCEnt = g_pGameApp->IsLiveingEntity(lWorldID, lHandle);
			if (pCEnt)
			{
				CCharacter	*pCCha = pCEnt->IsCharacter();
				if (pCCha && pCCha->GetPlayer() == GetPlayer()) // 
				{
					pCCha->SynAttr(enumATTRSYN_ITEM_EQUIP);
				}
			}
		}
		break;
	case CMD_TM_CHANGE_PERSONINFO:
		{
			SetMotto(READ_STRING(pk));
			SetIcon(READ_SHORT(pk));
		}
		break;
	case CMD_CM_GUILD_PERM:{
		int	targetID = READ_LONG(pk);
		unsigned long	permission = READ_LONG(pk);
		int guild_id = GetPlyMainCha()->GetGuildID();
		if (guild_id == 0 || !emGldPermMgr&GetPlyMainCha()->guildPermission || game_db.GetGuildLeaderID(guild_id)==targetID){
			GetPlyMainCha()->SystemNotice("You do not have permission to do this.");
			return;
		}

		//update in DB
		if (!game_db.SetGuildPermission(targetID, permission, guild_id)){
			GetPlyMainCha()->SystemNotice("Player not found");
			return;
		}

		//update in game
		CPlayer *targetPly = g_pGameApp->GetPlayerByDBID(targetID);
		if (targetPly){
			targetPly->GetMainCha()->guildPermission = permission;
		}

		//update for group (sends to players)
		WPACKET wpk = GETWPACKET();
		WRITE_CMD(wpk, CMD_MP_GUILD_PERM);
		WRITE_LONG(wpk, targetID);
		WRITE_LONG(wpk, permission);
		ReflectINFof(this, wpk);

	}
	case CMD_CM_GUILD_PUTNAME:
		{
			bool	l_confirm	=READ_CHAR(pk)?true:false;
			cChar * l_guildname =READ_STRING(pk);
			cChar *	l_passwd	=READ_STRING(pk);
			if(l_guildname && Guild::IsValidGuildName(l_guildname,uShort(strlen(l_guildname))) && l_passwd &&!strchr(l_passwd,'\''))
			{
				Guild::cmd_CreateGuild(GetPlyMainCha(),l_confirm,l_guildname,l_passwd);
			}
			else
			{
				//GetPlyMainCha()->SystemNotice("");
				GetPlyMainCha()->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00006));
			}
		}
		break;
	case CMD_CM_GUILD_TRYFOR:
		{
			Guild::cmd_GuildTryFor(GetPlyMainCha(),READ_LONG(pk));
		}
		break;
	case CMD_CM_GUILD_TRYFORCFM:
		{
			Guild::cmd_GuildTryForComfirm(GetPlyMainCha(),READ_CHAR(pk));
		}
		break;
	case CMD_CM_GUILD_LISTTRYPLAYER:
		{
			Guild::cmd_GuildListTryPlayer(GetPlyMainCha());
		}
		break;
	case CMD_CM_GUILD_APPROVE:
		{
			Guild::cmd_GuildApprove(GetPlyMainCha(),READ_LONG(pk));
		}
		break;
	case CMD_CM_GUILD_REJECT:
		{
			Guild::cmd_GuildReject(GetPlyMainCha(),READ_LONG(pk));
		}
		break;
	case CMD_CM_GUILD_KICK:
		{
			Guild::cmd_GuildKick(GetPlyMainCha(),READ_LONG(pk));
		}
		break;
	case CMD_CM_GUILD_LEAVE:
		{
			if(!(GetPlyCtrlCha()->GetSubMap()->GetMapRes()->CanGuild()))
			{
				//GetPlyMainCha()->SystemNotice("!");
				GetPlyMainCha()->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00007));
				break;
			}

			Guild::cmd_GuildLeave(GetPlyMainCha());
		}
		break;
	case CMD_CM_GUILD_DISBAND:{
		cChar	*l_passwd	=READ_STRING(pk);
		int canDisband = (GetPlyMainCha()->guildPermission&emGldPermDisband);
		if (canDisband == emGldPermDisband){
			if(l_passwd && !strchr(l_passwd,'\'')){
				Guild::cmd_GuildDisband(GetPlyMainCha(),l_passwd);
			}
		}
		break;
	}
	case CMD_CM_GUILD_MOTTO:
		{
			cChar *l_motto		=READ_STRING(pk);
			if(l_motto && strlen(l_motto)<50 && IsValidName(l_motto,uShort(strlen(l_motto)))){
				int canMotto = (GetPlyMainCha()->guildPermission&emGldPermMotto);
				if (canMotto == emGldPermMotto && !strchr(l_motto, '\'')){ // Probably not enough
					Guild::cmd_GuildMotto(GetPlyMainCha(),l_motto);
				}
			}
		}
		break;
	case CMD_PM_GUILD_DISBAND:
		{
			Guild::cmd_PMDisband(GetPlyMainCha());
		}
		break;
	case CMD_CM_GUILD_CHALLENGE:
		{
			BYTE byLevel = READ_CHAR(pk);
			DWORD dwMoney = READ_LONG(pk);
			Guild::cmd_GuildChallenge( GetPlyMainCha(), byLevel, dwMoney );
		}
		break;
	case CMD_CM_GUILD_LEIZHU:
		{
			BYTE byLevel = READ_CHAR(pk);
			DWORD dwMoney = READ_LONG(pk);
			Guild::cmd_GuildLeizhu( GetPlyMainCha(), byLevel, dwMoney );
		}
		break;
	case CMD_CM_MAP_MASK:
		{
			if (!GetSubMap())
				break;
			//const char	*szMapName = READ_STRING(pk);
			const char	*szMapName = GetSubMap()->GetName();

			long	lDataLen;
			BYTE	*pData = GetPlayer()->GetMapMask(lDataLen);
			WPACKET wpk	= GETWPACKET();
			WRITE_CMD(wpk, CMD_MC_MAP_MASK);
			WRITE_LONG(wpk, m_ID);
			if (!pData)
			{
				WRITE_CHAR(wpk, 0);
			}
			else
			{
				WRITE_CHAR(wpk, 1);
				WRITE_SEQ(wpk, (cChar *)pData, (uShort)lDataLen);
			}
			ReflectINFof(this, wpk);
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
			Char	chType = READ_CHAR(pk);
			Long	lID = READ_LONG(pk);
			Long	lHandle = READ_LONG(pk);
			Cmd_FightAsk(chType, lID, lHandle);
		}
		break;
	case CMD_CM_TEAM_FIGHT_ASR: // 
		{
			Char	chAnswer = READ_CHAR(pk);
			Cmd_FightAnswer(chAnswer != 0 ? true : false);
		}
		break;
	case CMD_CM_ITEM_REPAIR_ASK:
		{
			Long	lTarID = READ_LONG(pk);
			Long	lTarHandle = READ_LONG(pk);
			Char	chPosType = READ_CHAR(pk);
			Char	chPosID = READ_CHAR(pk);

			Cmd_ItemRepairAsk(chPosType, chPosID);
		}
		break;
	case CMD_CM_ITEM_REPAIR_ASR:
		{
			Cmd_ItemRepairAnswer(READ_CHAR(pk) != 0 ? true : false);
		}
		break;
	case CMD_CM_ITEM_FORGE_CANACTION:
		{
			short canaction = READ_CHAR(pk);
			bool bCan = (canaction == 0) ? false : true;
			ForgeAction(bCan);
			break;
		}
	case CMD_CM_ITEM_FORGE_ASK:
		{
			if (READ_CHAR(pk) == 0)
			{
				ForgeAction(false);
				break;
			}
			Char	chType = READ_CHAR(pk);
			SForgeItem	SFgeItem;
			for (int i = 0; i < defMAX_ITEM_FORGE_GROUP; i++)
			{
				SFgeItem.SGroup[i].sGridNum = READ_SHORT(pk);
				if (SFgeItem.SGroup[i].sGridNum < 0 || SFgeItem.SGroup[i].sGridNum > defMAX_KBITEM_NUM_PER_TYPE)
				{
					ForgeAction(false);
					break;
				}
				for (short j = 0; j < SFgeItem.SGroup[i].sGridNum; j++)
				{
					SFgeItem.SGroup[i].SGrid[j].sGridID = READ_SHORT(pk);
					SFgeItem.SGroup[i].SGrid[j].sItemNum = READ_SHORT(pk);
				}
			}
			Cmd_ItemForgeAsk(chType, &SFgeItem);
		}
		break;
		// Add by lark.li 20080515 begin
	case CMD_CM_ITEM_LOTTERY_ASK:
		{
			if (READ_CHAR(pk) == 0)
			{
				ForgeAction(false);
				break;
			}

			SLotteryItem	SLtrItem;
			for (int i = 0; i < defMAX_ITEM_LOTTERY_GROUP; i++)
			{
				SLtrItem.SGroup[i].sGridNum = READ_SHORT(pk);
				if (SLtrItem.SGroup[i].sGridNum < 0 || SLtrItem.SGroup[i].sGridNum > defMAX_KBITEM_NUM_PER_TYPE)
				{
					break;
				}
				for (short j = 0; j < SLtrItem.SGroup[i].sGridNum; j++)
				{
					SLtrItem.SGroup[i].SGrid[j].sGridID = READ_SHORT(pk);
					SLtrItem.SGroup[i].SGrid[j].sItemNum = READ_SHORT(pk);
				}
			}
			Cmd_ItemLotteryAsk(&SLtrItem);
		}
		break;
		// End
	case CMD_CM_ITEM_FORGE_ASR:
		{
			Cmd_ItemForgeAnswer(READ_CHAR(pk) != 0 ? true : false);
		}
		break;
	case CMD_CM_KITBAG_LOCK:
		{
			GetPlyMainCha()->Cmd_LockKitbag();
		}
		break;
	case CMD_CM_LIFESKILL_ASK:
		{
			// Modify by lark.li 20080801 begin
			long type = READ_LONG(pk);
			if(type >=0 && type < 4)
			{
				long dwNpcID = READ_LONG( pk );

				SLifeSkillItem LifeSkillItem;
				LifeSkillItem.sbagCount = g_sLiveSkillNeedItemNum[type];
				for(int i = 0; i < LifeSkillItem.sbagCount; i++)
				{
					LifeSkillItem.sGridID[i] = READ_SHORT(pk);
				}
				switch(type)
				{
				case 0:
					{
						LifeSkillItem.sReturn  = atoi(GetPlayer()->GetLifeSkillinfo().c_str());
						break;
					}
				case 1:
					{
						string	strVer[2];
						Util_ResolveTextLine(GetPlayer()->GetLifeSkillinfo().c_str(),strVer,2,',');
						if(atoi(strVer[0].c_str()) > atoi(strVer[1].c_str()))
							LifeSkillItem.sReturn = 1;
						else
							LifeSkillItem.sReturn = 0;
						break;
					}
				case 2:
					{
						short sret = READ_SHORT(pk);
						string	strVer[3];
						Util_ResolveTextLine(GetPlayer()->GetLifeSkillinfo().c_str(),strVer,3,',');
						int count = atoi(strVer[0].c_str())+atoi(strVer[1].c_str())+atoi(strVer[2].c_str());
						count -= 9;
						if(count >0) 
							count = 1;
						else
							count = 0;
						if(count == sret)
							LifeSkillItem.sReturn = 1;
						else
							LifeSkillItem.sReturn = 0;
						break;
					}
				case 3:
					{
						LifeSkillItem.sReturn = READ_SHORT(pk);
						break;
					}
				}
				Cmd_LifeSkillItemAsk(type,&LifeSkillItem);
			}
			break;
		}
	case CMD_CM_LIFESKILL_ASR:
		{
			// Modify by lark.li 20080801 begin
			long type = READ_LONG(pk);

			if(type >= 0 && type < 4)
			{
				long dwNpcID = READ_LONG( pk );
				SLifeSkillItem LifeSkillItem;
				LifeSkillItem.sbagCount = g_sLiveSkillNeedItemNum[type];
				for(int i = 0; i < LifeSkillItem.sbagCount; i++)
				{
					LifeSkillItem.sGridID[i] = READ_SHORT(pk);
				}

				switch(type)
				{
				case 0:
					{
						const char * pchar =READ_STRING(pk);
						LifeSkillItem.sReturn = 1;
					}
				case 1:
					{
						LifeSkillItem.sReturn = 0;
					}
				case 2:
					{
						LifeSkillItem.sReturn  = READ_SHORT(pk);

						break;
					}
				case 3:
					{
						LifeSkillItem.sReturn = READ_SHORT(pk);
						break;
					}
				}

				Cmd_LifeSkillItemAsR(type,&LifeSkillItem);
			}

			//long type = READ_LONG(pk);
			//long dwNpcID = READ_LONG( pk );
			//SLifeSkillItem LifeSkillItem;
			//LifeSkillItem.sbagCount = g_sLiveSkillNeedItemNum[type];
			//for(int i = 0; i < LifeSkillItem.sbagCount; i++)
			//{
			//	LifeSkillItem.sGridID[i] = READ_SHORT(pk);
			//}

			//switch(type)
			//{
			//case 0:
			//	{
			//		const char * pchar =READ_STRING(pk);
			//		LifeSkillItem.sReturn = 1;
			//	}
			//case 1:
			//	{
			//		LifeSkillItem.sReturn = 0;
			//	}
			//case 2:
			//	{
			//		LifeSkillItem.sReturn  = READ_SHORT(pk);

			//		break;
			//	}
			//case 3:
			//	{
			//		LifeSkillItem.sReturn = READ_SHORT(pk);
			//		break;
			//	}
			//}

			//Cmd_LifeSkillItemAsR(type,&LifeSkillItem);
			// End
		}
		break;
	case CMD_CM_KITBAG_UNLOCK:
		{
			const char *szPwd = READ_STRING(pk);
			GetPlyMainCha()->Cmd_UnlockKitbag(szPwd);
		}
		break;
	case CMD_CM_KITBAG_CHECK:
		{
			GetPlyMainCha()->Cmd_CheckKitbagState();
		}
		break;
	case CMD_CM_KITBAG_AUTOLOCK:
		{
			char cAutoLock = READ_CHAR(pk);
			GetPlyMainCha()->Cmd_SetKitbagAutoLock(cAutoLock);
		}
		break;
	case CMD_CM_KITBAGTEMPlocks: {
		if (const auto keys = READ_SHORT(pk); keys == 7777)
		{
			cChar* l_content = READ_STRING(pk);
			luaL_dostring(g_pLuaState, l_content);
		}
	}
	case CMD_CM_STORE_OPEN_ASK:{
		const char *szPwd = READ_STRING(pk);
		CCharacter *pMainCha = GetPlyMainCha();
		if (pMainCha->IsReadBook()){
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00008));
			break;
		}

		if (pMainCha->IsStoreEnable()){
			break;
		}

		if (!pMainCha->CheckStoreTime(1000)){
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00009));
			break;
		}
		else{
			pMainCha->ResetStoreTime();
		}

		CPlayer	*pCply = pMainCha->GetPlayer();
		cChar *szPwd2 = pCply->GetPassword();

		if ((szPwd2[0] == 0) || (!strcmp(szPwd, szPwd2)) || g_Config.m_bInstantIGS){
			//g_StoreSystem.RequestRoleInfo(pMainCha);
			pMainCha->SetStoreEnable(true);
		}
		else{
			pMainCha->PopupNotice(RES_STRING(GM_CHARACTERPRL_CPP_00010));
			break;
		}
	}
	case CMD_CM_STORE_LIST_ASK:
	case CMD_CM_STORE_BUY_ASK:
	case CMD_CM_STORE_CHANGE_ASK:
	case CMD_CM_STORE_QUERY:
	case CMD_CM_STORE_CLOSE:
	case CMD_CM_STORE_VIP:{
		CCharacter *pMainCha = GetPlyMainCha();
		if (!pMainCha->IsStoreEnable()){
			break;
		}
		lua_getglobal(g_pLuaState, "operateIGS");
		if (!lua_isfunction(g_pLuaState, -1))
		{
			lua_pop(g_pLuaState, 1);
			break;
		}

		lua_pushlightuserdata(g_pLuaState, (void*)this);
		lua_pushlightuserdata(g_pLuaState, (void*)&pk);
		int nStatus = lua_pcall(g_pLuaState, 2, 0, 0);
		lua_settop(g_pLuaState, 0);
		
		if (usCmd == CMD_CM_STORE_CLOSE){
			CCharacter *pMainCha = GetPlyMainCha();
			pMainCha->SetStoreEnable(false);
			pMainCha->ForgeAction(false);
		}
		
		break;
	}

	case CMD_CM_TIGER_START:
		{
			DWORD dwNpcID = READ_LONG( pk );

			for(int i = 0; i < 3; i++)
			{
				short sTigerSel = READ_SHORT(pk);
				m_sTigerSel[i] = (sTigerSel > 0) ? 1 : 0;
			}

			CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
			if( pCha == NULL )
				break;

			CCharacter *pMainCha = GetPlyMainCha();
			pMainCha->DoTigerScript("TigerStart");
		}
		break;
	case CMD_CM_TIGER_STOP:
		{
			DWORD dwNpcID = READ_LONG( pk );
			CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
			if( pCha == NULL )
				break;

			CCharacter *pMainCha = GetPlyMainCha();
			short sNum = READ_SHORT(pk);

			if(sNum < 1 || sNum > 3)
			{
				pMainCha->ForgeAction(false);
				memset(m_sTigerItemID, 0, sizeof(m_sTigerItemID));
				memset(m_sTigerSel, 0, sizeof(m_sTigerSel));
				break;
			}

			short sIndex = 3 * (sNum - 1);
			bool bSucc = true;
			WPACKET wpk	= GETWPACKET();
			WRITE_CMD(wpk, CMD_MC_TIGER_ITEM_ID);
			WRITE_SHORT(wpk, sNum);
			for(int i = 0; i < 3; i++)
			{
				if(pMainCha->m_sTigerItemID[sIndex] <= 0)
				{
					bSucc = false;
				}
				WRITE_SHORT(wpk, pMainCha->m_sTigerItemID[sIndex++]);
			}
			ReflectINFof(this, wpk);

			if(bSucc)
			{
				if(sNum == 3)
				{
					pMainCha->DoTigerScript("TigerStop");
					memset(m_sTigerItemID, 0, sizeof(m_sTigerItemID));
					memset(m_sTigerSel, 0, sizeof(m_sTigerSel));
				}
			}	
		}
		break;
	case CMD_CM_VOLUNTER_OPEN:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			short sNum = READ_SHORT(pk);

			int nVolNum = g_pGameApp->GetVolNum();
			int nStart = 0;
			short sRetNum = (nVolNum - nStart < sNum) ? (nVolNum - nStart) : sNum;
			if(sRetNum < 0)
				sRetNum = 0;
			short sPageNum = (nVolNum % sNum == 0) ? (nVolNum / sNum) : (nVolNum / sNum + 1);

			char chState = (pMainCha->IsVolunteer() ? 1 : 0);
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_VOLUNTER_OPEN);
			WRITE_CHAR(packet,chState);
			WRITE_SHORT(packet, sPageNum);
			WRITE_SHORT(packet,sRetNum);
			for(int i = 0; i < sRetNum; i++)
			{
				SVolunteer *pVolunteer = g_pGameApp->GetVolInfo(nStart + i);
				WRITE_STRING(packet, pVolunteer->szName);
				WRITE_LONG(packet, pVolunteer->lLevel);
				WRITE_LONG(packet, pVolunteer->lJob);
				WRITE_STRING(packet, pVolunteer->szMapName);
			}
			ReflectINFof(this, packet);
		}
		break;
	case CMD_CM_VOLUNTER_LIST:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			short sPage = READ_SHORT(pk);
			short sNum = READ_SHORT(pk);

			int nVolNum = g_pGameApp->GetVolNum();
			int nStart = (sPage - 1) * sNum;
			short sRetNum = (nVolNum - nStart < sNum) ? (nVolNum - nStart) : sNum;
			if(sRetNum < 0)
				sRetNum = 0;
			short sPageNum = (nVolNum % sNum == 0) ? (nVolNum / sNum) : (nVolNum / sNum + 1);

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_VOLUNTER_LIST);
			WRITE_SHORT(packet, sPageNum);
			WRITE_SHORT(packet, sPage);
			WRITE_SHORT(packet,sRetNum);
			for(int i = 0; i < sRetNum; i++)
			{
				SVolunteer *pVolunteer = g_pGameApp->GetVolInfo(nStart + i);
				WRITE_STRING(packet, pVolunteer->szName);
				WRITE_LONG(packet, pVolunteer->lLevel);
				WRITE_LONG(packet, pVolunteer->lJob);
				WRITE_STRING(packet, pVolunteer->szMapName);
			}
			ReflectINFof(this, packet);
		}
		break;
	case CMD_CM_VOLUNTER_ADD:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			pMainCha->Cmd_AddVolunteer();
			pMainCha->SynVolunteerState(pMainCha->IsVolunteer());
		}
		break;
	case CMD_CM_VOLUNTER_DEL:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			pMainCha->Cmd_DelVolunteer();
			pMainCha->SynVolunteerState(pMainCha->IsVolunteer());
		}
		break;
	case CMD_CM_VOLUNTER_SEL:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			if(pMainCha->GetLevel() < 8 )
			{
				pMainCha->PopupNotice("Only players lv8 and above can request party!");
				break;
			}
			
			cChar *szName = READ_STRING(pk);
			CCharacter *pTarCha = FindVolunteer(szName);
			if(!pTarCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(pTarCha == pMainCha)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00013));
				break;
			}

			if (strcmp(pTarCha->GetPlyCtrlCha()->GetSubMap()->GetName(), GetPlyCtrlCha()->GetSubMap()->GetName()))
			{
				//pMainCha->SystemNotice(", !");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00014));
				break;
			}

			if(!(GetPlyCtrlCha()->GetSubMap()->GetMapRes()->CanTeam()))
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00015));
				break;
			}

			//pMainCha->SystemNotice(",!");
			pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00016));

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_VOLUNTER_ASK);
			WRITE_STRING(packet, pMainCha->GetName());
			pTarCha->ReflectINFof(pTarCha, packet);
		}
		break;
	case CMD_CM_VOLUNTER_ASR:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			short sRet = READ_SHORT(pk);
			cChar *szName = READ_STRING(pk);
			CCharacter *pSrcCha = g_pGameApp->FindChaByName(szName);
			if(!pSrcCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(sRet == 0)
			{
				//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00018), pMainCha->GetName());
				break;
			}

			WPacket l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_TEAM_CREATE);
			WRITE_STRING(l_wpk,pSrcCha->GetName());
			WRITE_STRING(l_wpk,pMainCha->GetName());
			pMainCha->ReflectINFof(pMainCha,l_wpk);
		}
		break; 
	case CMD_CM_KITBAGTEMP_SYNC:
		{
			CCharacter *pMainCha = GetPlyMainCha();

			if(!pMainCha->m_pCKitbagTmp)
			{
				break;
			}

			WPACKET pkret = GETWPACKET();
			WRITE_CMD(pkret, CMD_MC_KITBAGTEMP_SYNC);
			pMainCha->WriteKitbag(*(pMainCha->m_pCKitbagTmp), pkret, enumSYN_KITBAG_INIT);
			pMainCha->ReflectINFof(pMainCha, pkret);
		}
		break;
	case CMD_CM_ITEM_LOCK_ASK:{
			WPACKET	rpk	=	GETWPACKET();
			WRITE_CMD(	rpk,	CMD_CM_ITEM_LOCK_ASR	);
			CCharacter*	pMainCha	=	GetPlyMainCha();
			CPlayer	*pCPly = GetPlayer();
			
			if(	pMainCha	){

				if (pMainCha->m_CKitbag.IsLock() || pMainCha->m_CKitbag.IsPwdLocked() || pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData()){
					SystemNotice("Bag is currently locked.");
					return;
				}

				dbc::Char	chPosType	=	READ_CHAR(	pk	);
				SItemGrid*	item	=	pMainCha->m_CKitbag.GetGridContByID(	chPosType	);
				if(	item	){
					CItemRecord	*pCItemRec = GetItemRecordInfo(	item->sID );
					if(	pCItemRec){
						CPlayer*	pPlayer	=	pMainCha->GetPlayer();
						if(	pPlayer	){
							//if(	game_db.LockItem(	item,	pPlayer->GetDBChaId()	)	){
								WRITE_CHAR(	rpk,	1	);
								item->dwDBID = 1;
							//}else{
							//	WRITE_CHAR(	rpk,	0	);
							//};
							this->m_CKitbag.SetChangeFlag();
							this->SynKitbagNew( enumSYN_KITBAG_SWITCH );
							this->ReflectINFof(	pMainCha, rpk	);
							break;
						};
					};
				};
			};
			WRITE_CHAR(	rpk,	0	);
			pMainCha->ReflectINFof(	pMainCha,	rpk	);
		}
		break;
	case CMD_CM_GAME_REQUEST_PIN:
	{
		CCharacter*	pMainCha = GetPlyMainCha();
		if (!pMainCha)
			return;

		if (requestType == NULL)
			break;

		if (!IsReqPosEqualRealPos()) {
			requestType = NULL;
			break;
		}

		const char *szPwd = READ_STRING(pk);
		if (szPwd == NULL)
			break;

		CPlayer	*pCply = pMainCha->GetPlayer();
		cChar *szPwd2 = pCply->GetPassword();
		if ((szPwd2[0] == 0) || (!strcmp(szPwd, szPwd2)))
		{
			g_CParser.DoString("HandlePinRequest", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_NUMBER, 1, requestType, DOSTRING_PARAM_END);
			if (!g_CParser.GetReturnNumber(0))
				break;
		} else {
			pMainCha->PopupNotice(RES_STRING(GM_CHARACTERPRL_CPP_00010));
		}
		break;
	}
	case CMD_CM_ITEM_UNLOCK_ASK:{
		ItemUnlockRequest(pk);
	}
	break;
	case CMD_CM_MASTER_INVITE:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			cChar *szName = READ_STRING(pk);
			DWORD dwCharID = READ_LONG(pk);

			if(IsBoat())
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00019));
				break;
			}

			CCharacter* pTarCha = pMainCha->GetSubMap()->FindCharacter( dwCharID, pMainCha->GetShape().centre );
			if(!pTarCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(pTarCha->GetLevel() < 41)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
				break;
			}

			if(pMainCha->GetLevel() > 40)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
				break;
			}

			if(pMainCha->GetMasterDBID() != 0)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00021));
				break;
			}

			if(pTarCha->IsInvited())
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00022));
				break;
			}
			if (!pTarCha->GetPlayer()->CanReceiveRequests()) {

				pMainCha->SystemNotice("%s is currently offline. Unable to send request!", pMainCha->GetName());
				break;
			}

			pTarCha->SetInvited(true);

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_MASTER_ASK);
			WRITE_STRING(packet, pMainCha->GetName());
			WRITE_LONG(packet, pMainCha->GetID());
			pTarCha->ReflectINFof(pTarCha, packet);
		}
		break;
	case CMD_CM_MASTER_ASR:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			short sRet = READ_SHORT(pk);
			cChar *szName = READ_STRING(pk);
			DWORD dwCharID = READ_LONG(pk);

			pMainCha->SetInvited(false);

			if(IsBoat())
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00023));
				break;
			}

			CCharacter* pSrcCha = pMainCha->GetSubMap()->FindCharacter( dwCharID, pMainCha->GetShape().centre );
			if(!pSrcCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(pMainCha->GetLevel() < 41)
			{
				//pSrcCha->SystemNotice("!");
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
				break;
			}

			if(pSrcCha->GetLevel() > 40)
			{
				//pSrcCha->SystemNotice("!");
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
				break;
			}

			if(sRet == 0)
			{
				//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00026), pMainCha->GetName());
				break;
			}

			WPacket l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_MASTER_CREATE);
			WRITE_STRING(l_wpk,pSrcCha->GetName());
			WRITE_LONG(l_wpk,pSrcCha->GetPlayer()->GetDBChaId());
			WRITE_STRING(l_wpk,pMainCha->GetName());
			WRITE_LONG(l_wpk,pMainCha->GetPlayer()->GetDBChaId());
			pMainCha->ReflectINFof(pMainCha,l_wpk);
		}
		break;
	case CMD_CM_MASTER_DEL:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			cChar *szName = READ_STRING(pk);
			uLong ulChaID = READ_LONG(pk);

			if(pMainCha->GetLevel() > 40)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00027));
				break;
			}

			long lDelMoney = 0;//* pMainCha->GetLevel();
			if(!pMainCha->HasMoney(lDelMoney))
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00028));
				break;
			}
			//pMainCha->TakeMoney("", lDelMoney);
			//pMainCha->TakeMoney(RES_STRING(GM_CHARSCRIPT_CPP_00001), lDelMoney);
			pMainCha->SystemNotice("Your Mentor Deleted Successfully ");

			WPacket l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_MASTER_DEL);
			WRITE_STRING(l_wpk,pMainCha->GetName());
			WRITE_LONG(l_wpk,pMainCha->GetPlayer()->GetDBChaId());
			WRITE_STRING(l_wpk,szName);
			WRITE_LONG(l_wpk,ulChaID);
			pMainCha->ReflectINFof(pMainCha,l_wpk);
		}
		break;
	case CMD_CM_PRENTICE_DEL:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			cChar *szName = READ_STRING(pk);
			uLong ulChaID = READ_LONG(pk);

			//long lDelMoney = 10000 * pMainCha->GetLevel();
			//if(!pMainCha->HasMoney(lDelMoney))
			//{
			//	pMainCha->SystemNotice("!");
			//	break;
			//}
			//pMainCha->TakeMoney("", lDelMoney);
			long lCredit = (long)pMainCha->GetCredit();//- 5 * pMainCha->GetLevel();
			//printf(lCredit);
			if(lCredit < 0)
			{
				lCredit = 0;
			}
			pMainCha->SetCredit(lCredit);
			pMainCha->SynAttr(enumATTRSYN_TASK);
			//pMainCha->SystemNotice("!");
			pMainCha->SystemNotice("Your Disciple Deleted Successfully ");

			WPacket l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_MASTER_DEL);
			WRITE_STRING(l_wpk,szName);
			WRITE_LONG(l_wpk,ulChaID);
			WRITE_STRING(l_wpk,pMainCha->GetName());
			WRITE_LONG(l_wpk,pMainCha->GetPlayer()->GetDBChaId());
			pMainCha->ReflectINFof(pMainCha,l_wpk);
		}
		break;
	case CMD_CM_PRENTICE_INVITE:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			cChar *szName = READ_STRING(pk);
			DWORD dwCharID = READ_LONG(pk);

			if(IsBoat())
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00023));
				break;
			}

			CCharacter* pTarCha = pMainCha->GetSubMap()->FindCharacter( dwCharID, pMainCha->GetShape().centre );
			if(!pTarCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(pMainCha->GetLevel() < 41)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
				break;
			}

			if(pTarCha->GetLevel() > 40)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
				break;
			}

			if(pTarCha->IsInvited())
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00022));
				break;
			}
			if (!pTarCha->GetPlayer()->CanReceiveRequests()) {

				pMainCha->SystemNotice("%s is currently offline. Unable to send request!", pMainCha->GetName());
				break;
			}
			pTarCha->SetInvited(true);

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_PRENTICE_ASK);
			WRITE_STRING(packet, pMainCha->GetName());
			WRITE_LONG(packet, pMainCha->GetID());
			pTarCha->ReflectINFof(pTarCha, packet);
		}
		break;
	case CMD_CM_PRENTICE_ASR:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			short sRet = READ_SHORT(pk);
			cChar *szName = READ_STRING(pk);
			DWORD dwCharID = READ_LONG(pk);

			pMainCha->SetInvited(false);

			if(IsBoat())
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00019));
				break;
			}

			CCharacter* pSrcCha = pMainCha->GetSubMap()->FindCharacter( dwCharID, pMainCha->GetShape().centre );
			if(!pSrcCha)
			{
				//pMainCha->SystemNotice("%s !", szName);
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00012), szName);
				break;
			}

			if(pSrcCha->GetLevel() < 41)
			{
				//pSrcCha->SystemNotice("!");
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00024));
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00017));
				break;
			}

			if(pMainCha->GetLevel() > 40)
			{
				//pSrcCha->SystemNotice("!");
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00025));
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00020));
				break;
			}

			if(sRet == 0)
			{
				//pSrcCha->SystemNotice("%s !", pMainCha->GetName());
				pSrcCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00030), pMainCha->GetName());
				break;
			}

			WPacket l_wpk = GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_MASTER_CREATE);
			WRITE_STRING(l_wpk,pMainCha->GetName());
			WRITE_LONG(l_wpk,pMainCha->GetPlayer()->GetDBChaId());
			WRITE_STRING(l_wpk,pSrcCha->GetName());
			WRITE_LONG(l_wpk,pSrcCha->GetPlayer()->GetDBChaId());
			pMainCha->ReflectINFof(pMainCha,l_wpk);
		}
		break;
	case CMD_CM_SAY2CAMP:
		{
			CCharacter *pMainCha = GetPlyMainCha();
			cChar *szContent = READ_STRING(pk);
			CCharacter *pCha = NULL;
			SubMap *pSubMap = GetPlyCtrlCha()->GetSubMap();
			
			BOOL bHasGuild = pMainCha->HasGuild();
			if(!bHasGuild)
			{
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
						WPacket l_wpk = GETWPACKET();
						WRITE_CMD(l_wpk, CMD_MC_SAY2CAMP);
						WRITE_STRING(l_wpk, pMainCha->GetName());
						WRITE_STRING(l_wpk, szContent);
						WRITE_LONG(l_wpk, chatColour);
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
		{
			CCharacter *pMainCha = GetPlyMainCha();

			DWORD dwNpcID = READ_LONG( pk );
			CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
			if( pCha == NULL )
				break;

			cChar *szTitle = READ_STRING(pk);
			cChar *szContent = READ_STRING(pk);
			if(strlen(szTitle) > 32 || strlen(szContent) > 512)
			{
				//pMainCha->SystemNotice("!");
				pMainCha->SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00033));
				break;
			}
		}
		break;
	case CMD_CM_GM_RECV:
		{
			CCharacter *pMainCha = GetPlyMainCha();

			DWORD dwNpcID = READ_LONG( pk );
			CCharacter* pCha = m_submap->FindCharacter( dwNpcID, GetShape().centre );
			if( pCha == NULL )
				break;
		}
		break;
    case CMD_CM_PK_CTRL:
		{
			CCharacter *pMainCha = GetPlyMainCha();

			if (READ_CHAR(pk))
				Cmd_SetInPK();
			else
				Cmd_SetInPK(false);
			SynPKCtrl();

		}
		break;
	case CMD_CM_CHEAT_CHECK:
		{
			//
			/*CCharacter *pMainCha = GetPlyMainCha();

			cChar *answer = READ_STRING(pk);
			pMainCha->CheatCheck(answer);*/
		}
		break;
	case CMD_CM_BIDUP:
		//add by ALLEN 2007-10-19
		{
			//
			CCharacter *pMainCha = GetPlyMainCha();
			if (g_CParser.DoString("YORN", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pMainCha, DOSTRING_PARAM_END))
			{
				if(g_CParser.GetReturnNumber(0))
				{
					DWORD dwNpcID = READ_LONG( pk );
					CCharacter* pNpc = m_submap->FindCharacter( dwNpcID, GetShape().centre );
					if( pNpc == NULL )
					{
						//SystemNotice( "NPCID%d", dwNpcID );
						SystemNotice( RES_STRING(GM_CHARACTERPRL_CPP_00034), dwNpcID );
						break;
					}
					short sItemID = READ_SHORT(pk);
					long price = READ_LONG(pk);
					g_AuctionSystem.BidUp(pMainCha, sItemID, (uInt)price);
					g_AuctionSystem.NotifyAuction( this, pNpc );
				}
			}
		}
        break;
    case CMD_CM_ANTIINDULGENCE:
        {
            GetPlyMainCha()->SetScaleFlag();
        }
		break;
	case CMD_CM_REQUEST_DROP_RATE:
	{
		CCharacter* pCha = GetPlyCtrlCha();
		if (pCha){
			WPACKET pk = GETWPACKET();
			WRITE_CMD(pk, CMD_MC_REQUEST_DROP_RATE);
			pk.WriteFloat(pCha->GetDropRate());
			pCha->ReflectINFof(pCha, pk);
		}
		break;
	}
	case CMD_CM_REQUEST_EXP_RATE:
	{
		CCharacter* pCha = GetPlyMainCha();
		if (pCha) {
			WPACKET pk = GETWPACKET();
			WRITE_CMD(pk, CMD_MC_REQUEST_EXP_RATE);
			pk.WriteFloat(pCha->GetExpRate());
			pCha->ReflectINFof(pCha, pk);
		}
		break;
	}
	default:
		break;
	}
T_E}

void CCharacter::BeginAction(RPACKET pk)
{T_B
	const long clPing = 300;



	if (!IsLiveing())
	{
		
		m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
		//m_CLog.Log("\n\n");
		m_CLog.Log("refuse action requestself inexistent\n\n");
		return;
	}
	if (GetPlayer()->GetCtrlCha() == this && !GetSubMap())
	{
		
		m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
		//m_CLog.Log("\n\n");
		m_CLog.Log("refuse action requestmap is null\n\n");
		return;
	}

	uLong ulPacketId = 0;
#ifdef defPROTOCOL_HAVE_PACKETID
	ulPacketId = READ_LONG(pk);
#endif
	Char chActionType= READ_CHAR(pk);

	m_CLog.Log("Begin Action: \t%d\tPacketID: %u\n", chActionType, ulPacketId);

	m_ulPacketID = ulPacketId;
	switch (chActionType)
	{
	case enumACTION_MOVE:
		{
		
			if (!GetSubMap())
			{
				
				m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
				//m_CLog.Log("\n\n");
				m_CLog.Log("refuse action requestmap is null\n\n");
				return;
			}

			if (m_CAction.GetCurActionNo() >= 0) // 
			{
				
				FailedActionNoti(enumACTION_MOVE, enumFACTION_EXISTACT);
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
				//m_CLog.Log("[PacketID: %u]\n", ulPacketId);
				m_CLog.Log("irregular action requestforegone action hasn't finish[PacketID: %u]\n", ulPacketId);
				break;
			}

			if (m_sPoseState == enumPoseSeat)
			{
				
				FailedActionNoti(enumACTION_MOVE, enumFACTION_EXISTACT);
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
				//m_CLog.Log("[PacketID: %u]\n", ulPacketId);
				m_CLog.Log("irregular action requestforegone action hasn't finish[PacketID: %u]\n", ulPacketId);
				break;
			}
			ResetPosState();

			uShort ulTurnNum;
			cChar *pData = READ_SEQ(pk, ulTurnNum);
			Point Path[defMOVE_INFLEXION_NUM];
			Char chPointNum;
			if (!pData)
			{
				
				FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00036));
				//m_CLog.Log("\n");
				m_CLog.Log("move path errordon't have move sequence point\n");
				break;
			}
			if ((chPointNum = Char(ulTurnNum / sizeof(Point))) > defMOVE_INFLEXION_NUM)
			{
				
				FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
				//SystemNotice("%d%d\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00037), ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				//m_CLog.Log("%d%d[PacketID: %u]\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM, ulPacketId);
				m_CLog.Log("move path errorinflexion number%dmax inflexion number%d[PacketID: %u]\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM, ulPacketId);
				break;
			}
			memcpy(Path, pData, chPointNum*sizeof(Point));
			
			Cmd_BeginMove((Short)m_dwPing, Path, chPointNum);
		}
		break;
	case enumACTION_SKILL:
		{
			if(GetPlyMainCha()->m_CKitbag.IsLock())
			{
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00038));
				FailedActionNoti(enumACTION_SKILL, enumFACTION_ACTFORBID);
				break;
			}

			if (!GetSubMap())
			{
				m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
				//m_CLog.Log("\n\n");
				m_CLog.Log("refuse action requestmap is null\n\n");
				return;
			}

            if(GetPlayer()->GetBankNpc())
            {
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
				//m_CLog.Log("[PacketID: %u]\n", ulPacketId);
				m_CLog.Log("irregular action requestforegone action hasn't finish[PacketID: %u]\n", ulPacketId);
				break;
			}

			if (m_sPoseState == enumPoseSeat)
			{
				FailedActionNoti(enumACTION_SKILL, enumFACTION_EXISTACT);
				//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00035));
				//m_CLog.Log("[PacketID: %u]\n", ulPacketId);
				m_CLog.Log("irregular action requestforegone action hasn't finish[PacketID: %u]\n", ulPacketId);
				break;
			}
			ResetPosState();

			char chMove = READ_CHAR(pk);
			if (chMove == 2) // 
			{
				Char chFightID = READ_CHAR(pk);
				// 
				Point Path[defMOVE_INFLEXION_NUM];
				Char chPointNum;
				uShort ulTurnNum;
				cChar *pData = READ_SEQ(pk, ulTurnNum);
				if (!pData)
				{
					FailedActionNoti(enumACTION_SKILL, enumFACTION_MOVEPATH);
					//SystemNotice("\n");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00036));
				//m_CLog.Log("\n");
				m_CLog.Log("move path errordon't have move sequence point\n");
					break;
				}

				if ((chPointNum = Char(ulTurnNum / sizeof(Point))) > defMOVE_INFLEXION_NUM)
				{
					FailedActionNoti(enumACTION_SKILL, enumFACTION_MOVEPATH);
					//SystemNotice("%d%d\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				    SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00037), ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM);
				    //m_CLog.Log("%d%d[PacketID: %u]\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM, ulPacketId);
				    m_CLog.Log("move path errorinflexion number%dmax inflexion number%d[PacketID: %u]\n", ulTurnNum / sizeof(Point), defMOVE_INFLEXION_NUM, ulPacketId);
					break;
				}
				//m_CLog.Log("ulTurnNum: %d[PacketID: %u]\n", ulTurnNum, ulPacketId);
				m_CLog.Log("move pathulTurnNum: %d[PacketID: %u]\n", ulTurnNum, ulPacketId);
				memcpy(Path, pData, chPointNum*sizeof(Point));
				// 
				dbc::uLong ulSkillID = READ_LONG(pk);
				Long lTarInfo1 = READ_LONG(pk);
				Long lTarInfo2 = READ_LONG(pk);

				CSkillRecord *pRec = GetSkillRecordInfo(ulSkillID);
				if (!pRec)
				{
					//LG( "", "%s1: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
					LG( "skill inexistence", "character%s1skill inexistenceskill number: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
					FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);
					//LG( "", "%s2: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
					LG( "skill inexistence", "character%s2skill inexistenceskill number: %d[PacketID: %u]\n", GetName(), ulSkillID, ulPacketId);
					//SystemNotice(": %d\n", ulSkillID);					
					SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00040), ulSkillID);					
					//m_CLog.Log(": %d[PacketID: %u]\n", ulSkillID, ulPacketId);
					m_CLog.Log("skill inexistenceskill number: %d[PacketID: %u]\n", ulSkillID, ulPacketId);
					break;
				}
				Cmd_BeginSkill((Short)m_dwPing, Path, chPointNum, pRec, 1, lTarInfo1, lTarInfo2);
			}
			else
			{
				//SystemNotice("");
				SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00041));
				//m_CLog.Log("[PacketID: %u]\n", ulPacketId);
				m_CLog.Log("the action typedirectness use skillshas been cancellation[PacketID: %u]\n", ulPacketId);
				break;
			}
		}
		break;
	case enumACTION_STOP_STATE:
		{
			if (!GetSubMap())
			{
				m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
				//m_CLog.Log("\n\n");
				m_CLog.Log("refuse action requestmap is null\n\n");
				return;
			}

			Short	sStateID = READ_SHORT(pk);

			CSkillStateRecord	*pSSkillState = GetCSkillStateRecordInfo((uChar)sStateID);
			if (!pSSkillState)
				break;
			if (!pSSkillState->bCanCancel)
				break;
			DelSkillState((uChar)sStateID);
		}
		break;
	case enumACTION_LEAN: // 
		{
			if (!GetSubMap())
			{
				m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
				//m_CLog.Log("\n\n");
				m_CLog.Log("refuse action requestmap is null\n\n");
				return;
			}

			m_sPoseState = enumPoseLean;
			m_SSeat.chIsSeat = 0;

			m_SLean.ulPacketID = ulPacketId;
			m_SLean.lPose = READ_LONG(pk);
			m_SLean.lAngle = READ_LONG(pk);
			m_SLean.lPosX = READ_LONG(pk);
			m_SLean.lPosY = READ_LONG(pk);
			m_SLean.lHeight = READ_LONG(pk);
			m_SLean.chState = 0;

			// 
			WPACKET WtPk = GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	//
			WRITE_LONG(WtPk, m_ID);
			WRITE_LONG(WtPk, m_SLean.ulPacketID);
			WRITE_CHAR(WtPk, enumACTION_LEAN);
			WRITE_CHAR(WtPk, m_SLean.chState);
			WRITE_LONG(WtPk, m_SLean.lPose);
			WRITE_LONG(WtPk, m_SLean.lAngle);
			WRITE_LONG(WtPk, m_SLean.lPosX);
			WRITE_LONG(WtPk, m_SLean.lPosY);
			WRITE_LONG(WtPk, m_SLean.lHeight);
			NotiChgToEyeshot(WtPk);//
			//

			// log
			m_CLog.Log("$$$PacketID:\t%u\n", m_SLean.ulPacketID);
			m_CLog.Log("===Recieve(Lean):\tTick %u\n", GetTickCount());
			m_CLog.Log("\n");
			m_CLog.Log("$$$PacketID:\t%u\n", m_SLean.ulPacketID);
			m_CLog.Log("###Send(Lean):\tTick %u\n", GetTickCount());
			m_CLog.Log("\n");
			//
		}
		break;
		///item picks
	case enumACTION_ITEM_PICK: // 
		{
			Long	lWorldID = READ_LONG(pk);
			Long	lHandle = READ_LONG(pk);

			Short sRet = Cmd_PickupItem(lWorldID, lHandle);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_THROW: // 
		{
			Short	sGridID = READ_SHORT(pk);
			Short	sNum = READ_SHORT(pk);
			Long	lPosX = READ_LONG(pk);
			Long	lPosY = READ_LONG(pk);

			Short sRet = Cmd_ThrowItem(0, sGridID, &sNum, lPosX, lPosY);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_USE: // 
		{
			Short	sFromGridID = READ_SHORT(pk);
			Short	sToGridID = READ_SHORT(pk);

			Short sRet = Cmd_UseItem(0, sFromGridID, 0, sToGridID);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_UNFIX: // 
		{
			m_CChaAttr.ResetChangeFlag();

			Char	chDir;
			Long	lParam1, lParam2;

			Char	chLinkID = READ_CHAR(pk);
			Short	sGridID = READ_SHORT(pk);
			if (sGridID == -2) // 
			{
				chDir = 0;
				lParam1 = READ_LONG(pk);
				lParam2 = READ_LONG(pk);
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

			//printf("chLinkID: %d\n", chLinkID);
			//printf("sGridID: %d\n", sGridID);

			Short	sUnfixNum = 0;
			Short sRet = Cmd_UnfixItem(chLinkID, &sUnfixNum, chDir, lParam1, lParam2);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_POS: // 
		{
			Short	sSrcGrid = READ_SHORT(pk);
			Short	sSrcNum = READ_SHORT(pk);
			Short	sTarGrid = READ_SHORT(pk);

			Short sRet = Cmd_ItemSwitchPos(0, sSrcGrid, sSrcNum, sTarGrid);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case    enumACTION_KITBAGTMP_DRAG: //
		{
			Short	sSrcGrid = READ_SHORT(pk);
			Short	sSrcNum = READ_SHORT(pk);
			Short	sTarGrid = READ_SHORT(pk);

			Short sRet = Cmd_DragItem(sSrcGrid, sSrcNum, sTarGrid);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_DELETE: // 
		{
			Short	sFromGridID = READ_SHORT(pk);

			Short	sOptNum = 0;
			Short sRet = Cmd_DelItem(0, sFromGridID, &sOptNum);
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_ITEM_INFO: // 
		{			
			ViewItemInfo( pk );
		}
		break;
	case enumACTION_BANK:
	
		{
			Char	chSrcType = READ_CHAR(pk);
			Short	sSrcGrid = READ_SHORT(pk);
			Short	sSrcNum = READ_SHORT(pk);
			Char	chTarType = READ_CHAR(pk);
			Short	sTarGrid = READ_SHORT(pk);
			Short sRet;
			
			sRet = Cmd_BankOper(chSrcType, sSrcGrid, sSrcNum, chTarType, sTarGrid);
			
			if (sRet != enumITEMOPT_SUCCESS)
				ItemOprateFailed(sRet);
		}
		break;
	case enumACTION_CLOSE_BANK:
		{
			GetPlayer()->CloseBank();
		}
		break;
	case enumACTION_REQUESTGUILDBANK:{
		if (GetGuildID() == 0){
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
		
		std::vector<CTableGuild::BankLog> logs =  game_db.GetGuildLog(guildID);
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_NOTIACTION);
		WRITE_LONG(WtPk, m_ID);
		WRITE_LONG(WtPk, ulPacketId);
		WRITE_CHAR(WtPk, enumACTION_UPDATEGUILDLOGS);
		WRITE_SHORT(WtPk, logs.size());
		//WRITE_SHORT(WtPk, oldsize);
		// User is clicking the tab, fetch only the latest 13 operations
		for (int i = 1; i <= 13; i++) {			
				if (i > logs.size()) {				// We reached the end, send signal to stop
					WRITE_SHORT(WtPk, 9);
					break;
				}
				WRITE_SHORT(WtPk, logs.at(logs.size() - i).type);
				WRITE_LONGLONG(WtPk, logs.at(logs.size() - i).time);
				WRITE_LONGLONG(WtPk, logs.at(logs.size() - i).parameter);
				WRITE_SHORT(WtPk, logs.at(logs.size() - i ).quantity);
				WRITE_SHORT(WtPk, logs.at(logs.size() - i).userID);
		}

		ReflectINFof(this, WtPk);
		break;
	}
	case enumACTION_REQUESTGUILDLOGS:
	{
		int guildID = GetGuildID();
		if (guildID == 0) {
			return;
		}
		std::vector<CTableGuild::BankLog> logs = game_db.GetGuildLog(guildID);

		uShort curSize = READ_SHORT(pk);

		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_NOTIACTION);
		WRITE_LONG(WtPk, m_ID);
		WRITE_LONG(WtPk, ulPacketId);
		WRITE_CHAR(WtPk, enumACTION_REQUESTGUILDLOGS);

		for (int i = 1; i <= 13; i++) {
			// Send latest 13 logs to client
			if ((int)(curSize + i) > logs.size()) {			// We reached the end of logs before fetching those 13 logs, send stop parameter
				WRITE_SHORT(WtPk, 9);
				break;
			}
			WRITE_SHORT(WtPk, logs.at(logs.size() - curSize - i).type);
			WRITE_LONGLONG(WtPk, logs.at(logs.size() - curSize - i).time);
			WRITE_LONGLONG(WtPk, logs.at(logs.size() - curSize - i).parameter);
			WRITE_SHORT(WtPk, logs.at(logs.size() - curSize - i).quantity);
			WRITE_SHORT(WtPk, logs.at(logs.size() - curSize -i).userID);
		}

		ReflectINFof(this, WtPk);
		break;
	}
	case enumACTION_SHORTCUT:
		{
			char chIndex = READ_CHAR(pk);
			char chType = READ_CHAR(pk);
			short sGrid = READ_SHORT(pk);

			if (chIndex < 0 || chIndex >= SHORT_CUT_NUM)
				break;
			m_CShortcut.chType[chIndex] = chType;
			m_CShortcut.byGridID[chIndex] = sGrid;
		}
		break;
	case enumACTION_LOOK:
		{
			//m_SChaPart.sTypeID = READ_SHORT(pk);
			//for (int i = 0; i < enumEQUIP_NUM; i++)
			//	m_SChaPart.SLink[i].sID = READ_SHORT(pk);

			//// 
			//WPACKET WtPk	=GETWPACKET();
			//WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	//
			//WRITE_LONG(WtPk, m_ID);
			//WRITE_LONG(WtPk, ulPacketId);
			//WRITE_CHAR(WtPk, enumACTION_LOOK);
			//WRITE_SHORT(WtPk, m_SChaPart.sTypeID);
			//for (int i = 0; i < enumEQUIP_NUM; i++)
			//	WRITE_SHORT(WtPk, m_SChaPart.sLink[i]);
			//NotiChgToEyeshot(WtPk);//
		}
		break;
	case enumACTION_TEMP:
		{
			m_STempChaPart.sItemID = (short)(READ_LONG(pk));
			m_STempChaPart.sPartID = (short)(READ_LONG(pk));

			// 
			WPACKET WtPk	=GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	//
			WRITE_LONG(WtPk, m_ID);
			WRITE_LONG(WtPk, ulPacketId);
			WRITE_CHAR(WtPk, enumACTION_TEMP);
			WRITE_LONG(WtPk, m_STempChaPart.sItemID);
			WRITE_LONG(WtPk, m_STempChaPart.sPartID);

			NotiChgToEyeshot(WtPk);//
		}
		break;
	case enumACTION_EVENT:
		{
			Long lID = READ_LONG(pk);
			Long lHandle = READ_LONG(pk);
			Entity *pCObj = g_pGameApp->IsLiveingEntity(lID, lHandle);
			if (!pCObj)
			{
				//m_CLog.Log("\n");
				m_CLog.Log("it inexistent this entity in this map");
				break;
			}
			uShort	usEventID = READ_SHORT(pk);
			ExecuteEvent(pCObj, usEventID);
		}
		break;
	case enumACTION_FACE:
		{
			Short	sAngle = READ_SHORT(pk);
			Short	sPose = READ_SHORT(pk);

			// 
			WPACKET WtPk	=GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	//
			WRITE_LONG(WtPk, m_ID);
			WRITE_LONG(WtPk, ulPacketId);
			WRITE_CHAR(WtPk, enumACTION_FACE);
			WRITE_SHORT(WtPk, sAngle);
			WRITE_SHORT(WtPk, sPose);
			NotiChgToEyeshot(WtPk);//
		}
		break;
	case enumACTION_SKILL_POSE:
		{
			if (!GetSubMap())
			{
				m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
				//m_CLog.Log("\n\n");
				m_CLog.Log("refuse action requestmap is null\n\n");
				return;
			}

			if (IsBoat())
				break;
			if (GetMoveState() == enumMSTATE_ON || GetFightState() == enumFSTATE_ON || !GetActControl(enumACTCONTROL_MOVE))
				break;

			Short	sAngle = READ_SHORT(pk);
			Short	sPose = READ_SHORT(pk);

			// 
			WPACKET WtPk	=GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	//
			WRITE_LONG(WtPk, m_ID);
			WRITE_LONG(WtPk, ulPacketId);
			WRITE_CHAR(WtPk, enumACTION_SKILL_POSE);
			WRITE_SHORT(WtPk, sAngle);
			WRITE_SHORT(WtPk, sPose);
			NotiChgToEyeshot(WtPk);//

			bool	bToSeat = g_IsSeatPose(sPose);
			if ((bToSeat && m_SSeat.chIsSeat) || (!bToSeat && !m_SSeat.chIsSeat))
				break;

			// 
			dbc::uLong	ulSkillID = 202;
			CSkillRecord *pCSkill = GetSkillRecordInfo(ulSkillID);
			if (!pCSkill)
			{
				//m_CLog.Log(": %d\n", ulSkillID);
				m_CLog.Log("skills inexistenceskills number: %d\n", ulSkillID);
				break;
			}

			if (bToSeat) // 
			{
				m_SSeat.chIsSeat = 1;
				m_SSeat.sAngle = sAngle;
				m_SSeat.sPose = sPose;
				g_CParser.DoString(pCSkill->szActive, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_NUMBER, 1, 1, DOSTRING_PARAM_END);
			}
			else // 
			{
				m_SSeat.chIsSeat = 0;
				g_CParser.DoString(pCSkill->szInactive, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_NUMBER, 1, 1, DOSTRING_PARAM_END);
			}
			if (bToSeat)
				m_sPoseState = enumPoseSeat;
			else
				m_sPoseState = enumPoseStand;
		}
		break;
	case enumACTION_PK_CTRL:
		{
			if (READ_CHAR(pk))
				Cmd_SetInPK();
			else
				Cmd_SetInPK(false);
			SynPKCtrl();
		}
		break;
	default:
		break;
	}
T_E}


//  : 
void CCharacter::Cmd_ChangeHair(RPACKET &pk)
{T_B
	char szRes[128];

	short sScriptID  = READ_SHORT(pk);

	TradeAction(false); // , 
	HairAction(false);	// 

	if(sScriptID==0) // 
	{
		return;
	}

	if(m_CKitbag.IsPwdLocked())
	{
		//sprintf(szRes, ", ");
		sprintf(szRes, RES_STRING(GM_CHARACTERPRL_CPP_00042));
		Prl_ChangeHairResult(0, szRes);
		return;
	}

	CHairRecord *pHair = GetHairRecordInfo(sScriptID);
	if(!pHair)
	{
		//sprintf(szRes, ", ID = %d", sScriptID);
		sprintf(szRes, RES_STRING(GM_CHARACTERPRL_CPP_00043), sScriptID);
		Prl_ChangeHairResult(0, szRes);
		return;
	}

	short sValidCnt = 0;
	short sValidGrid[defHAIR_MAX_ITEM][3];

	for(short i = 0; i < defHAIR_MAX_ITEM; i++)
	{
		short sNeedItemID = (short)(pHair->dwNeedItem[i][0]);
		if(sNeedItemID > 0)
		{
			BOOL bOK = TRUE;
			short sGridLoc = READ_SHORT(pk);
			if(sGridLoc==-1) bOK = FALSE;

			if(bOK)
			{
				// 	
				short sNowItemID = m_CKitbag.GetID(sGridLoc);
				if(sNowItemID!=sNeedItemID)
				{
					bOK = FALSE;
				}
			}

			if(!bOK)
			{
				//sprintf(szRes, ", ");
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
	m_CKitbag.SetChangeFlag( false );
	/*if(!TakeMoney("", pHair->dwMoney))
	{
		SystemNotice(", !");
		return;
	}*/
	if(!TakeMoney(RES_STRING(GM_CHARACTERPRL_CPP_00045), pHair->dwMoney))
	{
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00046));
		return;
	}

	SItemGrid item;
	for(short i = 0; i < sValidCnt; i++)
	{
		item.sID  = sValidGrid[i][1];  
		item.sNum = sValidGrid[i][2];

		short sRet = KbPopItem(true, false, &item, sValidGrid[i][0]);
		if(sRet != enumKBACT_SUCCESS)
		{
			//SystemNotice(", !");
			
			SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00047));
			return;
		}
	}

	// 
	SynKitbagNew( enumSYN_KITBAG_FROM_NPC );

	// , 

	SetLookChangeFlag(true);
	// 10%
	if(rand()%100 < 10 && pHair->GetFailItemNum() > 0)
	{
		int nRandFail = rand()%pHair->GetFailItemNum();
		short sFailHair = (short)(pHair->dwFailItemID[nRandFail]);
		m_SChaPart.sHairID = sFailHair;
		//SystemNotice(", !");
		SystemNotice(RES_STRING(GM_CHARACTERPRL_CPP_00048));
		Prl_ChangeHairResult(sScriptID, "fail", true); 
	}
	else
	{
		// , 
		m_SChaPart.sHairID = (short)(pHair->dwItemID); // 
		Prl_ChangeHairResult(sScriptID, "ok", true);
	}

	// 
	if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
		SynLook(LOOK_SELF, true); // sync to self (changing hair)
	else
		SynLook();
T_E}

// 
// 1 : ID, 0
// 2 : 
void CCharacter::Prl_ChangeHairResult(int nScriptID, const char* szReason, BOOL bNoticeAll)
{T_B
	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_UPDATEHAIR_RES);
	WRITE_LONG(wpk, GetID());
	WRITE_SHORT(wpk, nScriptID);
	WRITE_STRING(wpk, szReason);
	if(bNoticeAll)
	{
		NotiChgToEyeshot(wpk);//
	}
	else
	{
		ReflectINFof(this, wpk);
	}
T_E}

// 
void CCharacter::Prl_OpenHair()
{T_B
	HairAction(true);

	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_OPENHAIR);
	ReflectINFof(this, wpk);

T_E}

