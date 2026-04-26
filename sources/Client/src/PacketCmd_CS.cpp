#include "StdAfx.h"
#include "PacketCmd.h"

#include "GameApp.h"
#include "Character.h"
#include "actor.h"
#include "procirculate.h"
#include "UIBoothForm.h"
#include "UIStoreForm.h"
#include "CommandMessages.h"
#include "CryptoUtils.h"
//------------------------
// C->S : 
//------------------------
bool CS_Connect(cChar* hostname, uint16_t port, uint32_t timeout) {
	g_logManager.InternalLog(LogLevel::Debug, "connections", SafeVFormat(GetLanguageString(294), hostname));
	if (g_NetIF->m_pCProCir) {
		delete g_NetIF->m_pCProCir;
	}
	g_NetIF->m_pCProCir = new CProCirculateCS(g_NetIF);

	discordInit();
	updateDiscordPresence("Connecting...", "");


	return g_NetIF->m_pCProCir->Connect(hostname, port, timeout);
}

//------------------------
// C->S : 
//------------------------
void CS_Disconnect(int reason) {
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->Disconnect(reason);
}

void CS_SendPrivateKey() {
	g_NetIF->m_pCProCir->SendPrivateKey();
}


//------------------------
// C->S : 
//------------------------
void CS_Login(const char* accounts, const char* password, const char* passport) {
	g_NetIF->m_pCProCir->Login(accounts, password, passport);
}

//------------------------
// C->S : 
//------------------------
void CS_Logout() {
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->Logout();
	return;
}

void CS_OfflineMode() {
	auto pk = net::msg::serializeCmOfflineModeCmd();
	g_NetIF->SendPacketMessage(pk);
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
}

void CS_CancelExit() {
	//    
	auto pk = net::msg::serializeCmCancelExitCmd();
	g_NetIF->SendPacketMessage(pk);
}

//------------------------
// C->S : 
//------------------------
void CS_BeginPlay(char cha_index) {
	g_NetIF->m_pCProCir->BeginPlay(cha_index);
}

//------------------------
// C->S : 
//------------------------
void CS_EndPlay() {
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->EndPlay();
}

//------------------------
// C->S : 
//------------------------

void CS_NewCha(const char* chaname, const char* birth, int type, int hair, int face) {
	g_NetIF->m_pCProCir->NewCha(chaname, birth, type, hair, face);
}

//------------------------
// C->S : 
//------------------------
void CS_DelCha(uint8_t cha_index, const char szPassword2[]) {
	g_NetIF->m_pCProCir->DelCha(cha_index, szPassword2);
}

//------------------------
// C->S : 
//------------------------
void CS_Say(const char* content) {
	g_NetIF->m_pCProCir->Say(content);
}

void CS_GuildBankOper(stNetBank* pNetBank) {
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}

	auto pk = net::msg::serialize(net::msg::CmGuildBankOperMessage{
		0, pNetBank->chSrcType, pNetBank->sSrcID, pNetBank->sSrcNum, pNetBank->chTarType, pNetBank->sTarID
	});
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankTakeGold(int gold) {
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	auto pk = net::msg::serialize(net::msg::CmGuildBankGoldMessage{1, 0, gold});
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankGiveGold(int gold) {
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	auto pk = net::msg::serialize(net::msg::CmGuildBankGoldMessage{1, 1, gold});
	g_NetIF->SendPacketMessage(pk);
}

void CS_ChangePass(const char* pass, const char* pin) {
	//TODO(Ogge): Might need to hash it using blake2s?
	auto pk = net::msg::serialize(net::msg::CmChangePassMessage{pass, pin});
	g_NetIF->SendPacketMessage(pk);
}

void CS_Register(const char* user, const char* pass, const char* email) {
	/*
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REGISTER);
	pk.WriteString(user);
	
	pk.WriteString(HashPassword(pass));
	pk.WriteString(email);
	g_NetIF->SendPacketMessage(pk);
	*/
}

void CS_StallSearch(int ItemID) {
	auto pk = net::msg::serialize(net::msg::CmStallSearchMessage{ItemID});
	g_NetIF->SendPacketMessage(pk);
}

//   
void CS_CreatePassword2(const char szPassword[]) {
	auto pk = net::msg::serialize(net::msg::CmCreatePassword2Message{szPassword});
	g_NetIF->SendPacketMessage(pk);
}

//   
void CS_UpdatePassword2(const char szOld[], const char szPassword[]) {
	auto pk = net::msg::serialize(net::msg::CmUpdatePassword2Message{szOld, szPassword});
	g_NetIF->SendPacketMessage(pk);
}

//
void CS_LockKitbag() {
	auto pk = net::msg::serializeCmKitbagLockCmd();
	g_NetIF->SendPacketMessage(pk);
}

//
void CS_UnlockKitbag(const char szPassword[]) {
	auto pk = net::msg::serialize(net::msg::CmKitbagUnlockMessage{szPassword});
	g_NetIF->SendPacketMessage(pk);
}

//
void CS_KitbagCheck() {
	auto pk = net::msg::serializeCmKitbagCheckCmd();
	g_NetIF->SendPacketMessage(pk);
}

// 
void CS_AutoKitbagLock(bool bAutoLock) {
	auto pk = net::msg::serialize(net::msg::CmAutoKitbagLockMessage{bAutoLock ? 1 : 0});
	g_NetIF->SendPacketMessage(pk);
}

// C->S : 
void CS_BeginAction(CCharacter* pCha, DWORD type, void* param, CActionState* pState) {
	g_NetIF->m_pCProCir->BeginAction(pCha, type, param, pState);
}

// C->S : 
void CS_EndAction(CActionState* pState) {
	g_NetIF->m_pCProCir->EndAction(pState);
}

// C->S : 
void CS_DieReturn(char chReliveType) {
	auto pk = net::msg::serialize(net::msg::CmDieReturnMessage{chReliveType});
	g_NetIF->SendPacketMessage(pk);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("###Send(DieReturn):\tTick:[{}]", GetTickCount()));
	//
}

//----------------------------
//C->S	: Ping
//----------------------------
void CS_SendPing() {
	return;
	//WPacket pk	= g_NetIF->GetWPacket();
	//pk.WriteCmd(CMD_CM_PING);

	//g_NetIF->dwLatencyTime[g_NetIF->m_pingid] = GetTickCount();
	//pk.WriteInt64(g_NetIF->m_pingid);
	//++(g_NetIF->m_pingid);
	//if(g_NetIF->m_pingid>=20) g_NetIF->m_pingid = 0;

	//g_NetIF->SendPacketMessage(pk);
}

void CS_MapMask(const char* szMapName) {
	auto pk = net::msg::serialize(net::msg::CmMapMaskMessage{szMapName});
	g_NetIF->SendPacketMessage(pk);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("###Send(MapMask):\tTick:[{}]", GetTickCount()));
	//
}

void CS_RequestTalk(DWORD dwNpcID, BYTE byCmd) {
	auto packet = net::msg::serialize(net::msg::CmRequestTalkMessage{(int64_t)dwNpcID, (int64_t)byCmd});
	g_NetIF->SendPacketMessage(packet);
}

void CS_SelFunction(DWORD dwNpcID, BYTE byPageID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmSelFunctionMessage{
		(int64_t)dwNpcID, (int64_t)byPageID, (int64_t)byIndex
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_Sale(DWORD dwNpcID, BYTE byIndex, BYTE byCount) {
	auto packet = net::msg::serialize(net::msg::CmSaleMessage{(int64_t)dwNpcID, (int64_t)byIndex, (int64_t)byCount});
	g_NetIF->SendPacketMessage(packet);
}

void CS_Buy(DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount) {
	auto packet = net::msg::serialize(net::msg::CmBuyMessage{
		(int64_t)dwNpcID, (int64_t)byItemType, (int64_t)byIndex1, (int64_t)byIndex2, (int64_t)byCount
	});
	g_NetIF->SendPacketMessage(packet);
}

//   
void CS_SelectTradeBoat(DWORD dwNpcID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmSelectTradeBoatMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage(packet);
}

//   NPC ( )
void CS_SaleGoods(DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount) {
	auto packet = net::msg::serialize(net::msg::CmSaleGoodsMessage{
		(int64_t)dwNpcID, (int64_t)dwBoatID, (int64_t)byIndex, (int64_t)byCount
	});
	g_NetIF->SendPacketMessage(packet);
}

//   NPC ( )
void CS_BuyGoods(DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount) {
	auto packet = net::msg::serialize(net::msg::CmBuyGoodsMessage{
		(int64_t)dwNpcID, (int64_t)dwBoatID, (int64_t)byItemType, (int64_t)byIndex1, (int64_t)byIndex2, (int64_t)byCount
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_BlackMarketExchangeReq(DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum,
							   BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmBlackMarketExchangeReqMessage{
		(int64_t)dwNpcID, (int64_t)sTimeNum, (int64_t)sSrcID, (int64_t)sSrcNum, (int64_t)sTarID, (int64_t)sTarNum,
		(int64_t)byIndex
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_RequestTrade(BYTE byType, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmRequestTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_AcceptTrade(BYTE byType, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmAcceptTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_AddItem(BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount) {
	auto packet = net::msg::serialize(net::msg::CmAddItemMessage{
		byType, dwCharID, byOpType, byIndex, byItemIndex, byCount
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_AddMoney(BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney) {
	auto packet = net::msg::serialize(net::msg::CmAddMoneyMessage{byType, dwCharID, byOpType, 0, dwMoney});
	g_NetIF->SendPacketMessage(packet);
}

void CS_AddIMP(BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney) {
	auto packet = net::msg::serialize(net::msg::CmAddMoneyMessage{byType, dwCharID, byOpType, 1, dwMoney});
	g_NetIF->SendPacketMessage(packet);
}

void CS_CancelTrade(BYTE byType, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmCancelTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_ValidateTradeData(BYTE byType, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmValidateTradeDataMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_ValidateTrade(BYTE byType, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmValidateTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_MissionPage(DWORD dwNpcID, BYTE byCmd, BYTE bySelItem, BYTE byParam) {
	auto packet = net::msg::serialize(net::msg::CmMissionPageMessage{
		(int64_t)dwNpcID, (int64_t)byCmd, (int64_t)bySelItem, (int64_t)byParam
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_SelMission(DWORD dwNpcID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmSelMissionMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage(packet);
}

//    NPC
void CS_MissionTalk(DWORD dwNpcID, BYTE byCmd) {
	auto packet = net::msg::serialize(net::msg::CmMissionTalkMessage{(int64_t)dwNpcID, (int64_t)byCmd});
	g_NetIF->SendPacketMessage(packet);
}

//     NPC
void CS_SelMissionFunc(DWORD dwNpcID, BYTE byPageID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmSelMissionFuncMessage{
		(int64_t)dwNpcID, (int64_t)byPageID, (int64_t)byIndex
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_MisLog() {
	auto packet = net::msg::serializeCmMisLogCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_MisLogInfo(WORD wID) {
	if (wID == -1)
		return;
	auto packet = net::msg::serialize(net::msg::CmMisLogInfoMessage{wID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_MisClear(WORD wID) {
	if (wID == -1)
		return;
	auto packet = net::msg::serialize(net::msg::CmMisLogClearMessage{wID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_ForgeItem(BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmForgeItemMessage{byIndex});
	g_NetIF->SendPacketMessage(packet);
}

void CS_CreateBoat(const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment) {
	auto packet = net::msg::serialize(net::msg::CmCreateBoatMessage{szBoat, szHeader, szEngine, szCannon, szEquipment});
	g_NetIF->SendPacketMessage(packet);
}

void CS_SelectBoatList(DWORD dwNpcID, BYTE byType, BYTE byIndex) {
	if (byType == mission::BERTH_TRADE_LIST) {
		CS_SelectTradeBoat(dwNpcID, byIndex);
	}
	else {
		auto packet = net::msg::serialize(net::msg::CmSelectBoatListMessage{
			(int64_t)dwNpcID, (int64_t)byType, (int64_t)byIndex
		});
		g_NetIF->SendPacketMessage(packet);
	}
}

void CS_SelectBoat(DWORD dwNpcID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmBoatLaunchMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage(packet);
}

void CS_SelectBoatBag(DWORD dwNpcID, BYTE byIndex) {
	auto packet = net::msg::serialize(net::msg::CmBoatBagSelMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage(packet);
}

void CS_UpdateBoat(char szHeader, char szEngine, char szCannon, char szEquipment) {
	auto packet = net::msg::serialize(net::msg::CmUpdateBoatMessage{szHeader, szEngine, szCannon, szEquipment});
	g_NetIF->SendPacketMessage(packet);
}

void CS_CancelBoat() {
	auto packet = net::msg::serializeCmBoatCancelCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_GetBoatInfo() {
	auto packet = net::msg::serializeCmBoatGetInfoCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_EntityEvent(DWORD dwEntityID) {
	auto packet = net::msg::serialize(net::msg::CmEntityEventMessage{(int64_t)dwEntityID});
	g_NetIF->SendPacketMessage(packet);

	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("###Send(Event-Entyty):\tTick:[{}]", GetTickCount()));
}

//    ( )
void CS_StallInfo(const char szName[], mission::NET_STALL_ALLDATA& Data) {
	net::msg::CmStallInfoMessage msg;
	msg.name = szName;
	msg.num = Data.byNum;
	msg.items.resize(Data.byNum);
	for (BYTE i = 0; i < Data.byNum; ++i) {
		msg.items[i] = {Data.Info[i].byGrid, Data.Info[i].dwMoney, Data.Info[i].byCount, Data.Info[i].byIndex};
	}
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallOpen(DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmStallOpenMessage{(int64_t)dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallBuy(DWORD dwCharID, BYTE byIndex, BYTE byCount, int gridID) {
	auto packet = net::msg::serialize(net::msg::CmStallBuyMessage{
		(int64_t)dwCharID, (int64_t)byIndex, (int64_t)byCount, (int64_t)gridID
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallClose() {
	auto packet = net::msg::serializeCmStallCloseCmd();
	g_NetIF->SendPacketMessage(packet);
}

//add by jilinlee 2007/4/20/////////////////////
void CS_ReadBookStart() {
	auto packet = net::msg::serializeCmReadBookStartCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_ReadBookClose() {
	auto packet = net::msg::serializeCmReadBookCloseCmd();
	g_NetIF->SendPacketMessage(packet);
}

///////////////////////////////////////////////

void CS_UpdateHair(stNetUpdateHair& stData) {
	auto packet = net::msg::serialize(net::msg::CmUpdateHairMessage{
		stData.sScriptID, stData.sGridLoc[0], stData.sGridLoc[1], stData.sGridLoc[2], stData.sGridLoc[3]
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_TeamFightAsk(unsigned long ulWorldID, long lHandle, char chType) {
	auto packet = net::msg::serialize(net::msg::CmTeamFightAskMessage{chType, (int64_t)ulWorldID, lHandle});
	g_NetIF->SendPacketMessage(packet);
}

void CS_TeamFightAnswer(bool bAccess) {
	auto packet = net::msg::serialize(net::msg::CmTeamFightAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage(packet);
}

// lRepairmanID,lRepairmanHandle 
// chPosType12
// chPosID
void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID) {
	auto packet = net::msg::serialize(net::msg::CmItemRepairAskMessage{
		lRepairmanID, lRepairmanHandle, chPosType, chPosID
	});
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemRepairAnswer(bool bAccess) {
	auto packet = net::msg::serialize(net::msg::CmItemRepairAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage(packet);
}

//  :   ( )
void CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk* pSForge) {
	net::msg::CmItemForgeGroupAskMessage msg;
	msg.sure = bSure ? 1 : 0;
	if (bSure) {
		msg.type = pSForge->chType;
		for (int i = 0; i < defMAX_FORGE_GROUP_NUM; i++) {
			msg.groups[i].cells.resize(pSForge->SGroup[i].sCellNum);
			for (short j = 0; j < pSForge->SGroup[i].sCellNum; j++) {
				msg.groups[i].cells[j] = {pSForge->SGroup[i].pCell->sPosID, pSForge->SGroup[i].pCell->sNum};
			}
		}
	}
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemForgeAnswer(bool bAccess) {
	auto packet = net::msg::serialize(net::msg::CmItemForgeAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage(packet);
}

// Add by lark.li 20080514 begin
void CS_ItemLotteryAnswer(bool bAccess) {
	auto packet = net::msg::serialize(net::msg::CmItemLotteryAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage(packet);
}

//  :   ( )
void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk* pSLottery) {
	net::msg::CmItemLotteryGroupAskMessage msg;
	msg.sure = bSure ? 1 : 0;
	if (bSure) {
		for (int i = 0; i < defMAX_LOTTERY_GROUP_NUM; i++) {
			msg.groups[i].cells.resize(pSLottery->SGroup[i].sCellNum);
			for (short j = 0; j < pSLottery->SGroup[i].sCellNum; j++) {
				msg.groups[i].cells[j] = {pSLottery->SGroup[i].pCell->sPosID, pSLottery->SGroup[i].pCell->sNum};
			}
		}
	}
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

// End
//   ()
void CS_ItemAmphitheaterAsk(bool bSure, int ReID) {
	auto packet = net::msg::serialize(net::msg::CmItemAmphitheaterAskMessage{bSure ? 1 : 0, (int64_t)ReID});
	g_NetIF->SendPacketMessage(packet);
}

//  :   (  )
void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount) {
	net::msg::CmItemForgePosAskMessage msg;
	msg.sure = bSure ? 1 : 0;
	if (bSure) {
		msg.type = (int64_t)(char)(nType);
		msg.posCount = (int64_t)nPosCount;
		for (int i = 0; i < nPosCount && i < 6; ++i)
			msg.positions[i] = (int64_t)arPacketPos[i];
	}
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}


// 
void CS_StoreOpenAsk(const char szPassword[]) {
	auto packet = net::msg::serialize(net::msg::CmStoreOpenAskMessage{szPassword});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreClose() {
	auto packet = net::msg::serializeCmStoreCloseCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreListAsk(long lClsID, short sPage, short sNum) {
	auto packet = net::msg::serialize(net::msg::CmStoreListAskMessage{lClsID, sPage, sNum});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreBuyAsk(long lComID) {
	auto packet = net::msg::serialize(net::msg::CmStoreBuyAskMessage{lComID});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreChangeAsk(long lNum) {
	auto packet = net::msg::serialize(net::msg::CmStoreChangeAskMessage{lNum});
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreQuery(long lNum) {
	auto packet = net::msg::serialize(net::msg::CmStoreQueryMessage{lNum});
	g_NetIF->SendPacketMessage(packet);
}

//void CS_StoreVIP(short sVipID, short sMonth)
//{
//	WPacket packet = g_NetIF->GetWPacket();
//	packet.WriteCmd( CMD_CM_STORE_VIP );
//	packet.WriteInt64(sVipID);
//	packet.WriteInt64(sMonth);
//	g_NetIF->SendPacketMessage( packet );
//}

void CS_ReportWG(const char szInfo[]) {
	auto packet = net::msg::serialize(net::msg::CmReportWgMessage{szInfo});
	g_NetIF->SendPacketMessage(packet);
}

void CS_TigerStart(DWORD dwNpcID, short sSel1, short sSel2, short sSel3) {
	auto packet = net::msg::serialize(net::msg::CmTigerStartMessage{(int64_t)dwNpcID, sSel1, sSel2, sSel3});
	g_NetIF->SendPacketMessage(packet);
}

void CS_TigerStop(DWORD dwNpcID, short sNum) {
	auto packet = net::msg::serialize(net::msg::CmTigerStopMessage{(int64_t)dwNpcID, sNum});
	g_NetIF->SendPacketMessage(packet);
}

void CS_RequestDailyBuffInfo() {
	auto packet = net::msg::serializeCmDailyBuffRequestCmd();
	g_NetIF->SendPacketMessage(packet);
}


void CS_VolunteerList(short sPage, short sNum) {
	auto packet = net::msg::serialize(net::msg::CmVolunteerListMessage{sPage, sNum});
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerAdd() {
	auto packet = net::msg::serializeCmVolunteerAddCmd();
	g_NetIF->SendPacketMessage(packet);
}


void CS_VolunteerDel() {
	auto packet = net::msg::serializeCmVolunteerDelCmd();
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerSel(const char* szName) {
	auto packet = net::msg::serialize(net::msg::CmVolunteerSelMessage{szName});
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerOpen(short sNum) {
	auto packet = net::msg::serialize(net::msg::CmVolunteerOpenMessage{sNum});
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerAsr(BOOL bRet, const char* szName) {
	auto packet = net::msg::serialize(net::msg::CmVolunteerAsrMessage{bRet ? 1 : 0, szName});
	g_NetIF->SendPacketMessage(packet);
}

void CS_SyncKitbagTemp() {
	auto packet = net::msg::serializeCmKitbagTempSyncCmd();
	g_NetIF->SendPacketMessage(packet);
}

// Add by lark.li 20080707 begin
void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID) {
	auto packet = net::msg::serialize(net::msg::CmCaptainConfirmAsrMessage{sRet, (int64_t)dwTeamID});
	g_NetIF->SendPacketMessage(packet);
}

// End

//  
void CS_MasterInvite(const char* szName, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmMasterInviteMessage{szName, (int64_t)dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

//    
void CS_MasterAsr(short sRet, const char* szName, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmMasterAsrMessage{(int64_t)sRet, szName, (int64_t)dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

//  
void CS_PrenticeInvite(const char* szName, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmPrenticeInviteMessage{szName, (int64_t)dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

//    
void CS_PrenticeAsr(short sRet, const char* szName, DWORD dwCharID) {
	auto packet = net::msg::serialize(net::msg::CmPrenticeAsrMessage{(int64_t)sRet, szName, (int64_t)dwCharID});
	g_NetIF->SendPacketMessage(packet);
}

//  
void CS_MasterDel(const char* szName, uLong ulChaID) {
	auto packet = net::msg::serialize(net::msg::CmMasterDelMessage{szName, (int64_t)ulChaID});
	g_NetIF->SendPacketMessage(packet);
}

//  
void CS_PrenticeDel(const char* szName, uLong ulChaID) {
	auto packet = net::msg::serialize(net::msg::CmPrenticeDelMessage{szName, (int64_t)ulChaID});
	g_NetIF->SendPacketMessage(packet);
}

//     
void CP_Master_Refresh_Info(unsigned long chaid) {
	auto pk = net::msg::serialize(net::msg::CmCpMasterRefreshInfoMessage{(int64_t)chaid});
	g_NetIF->SendPacketMessage(pk);
}

//     
void CP_Prentice_Refresh_Info(unsigned long chaid) {
	auto pk = net::msg::serialize(net::msg::CmCpPrenticeRefreshInfoMessage{(int64_t)chaid});
	g_NetIF->SendPacketMessage(pk);
}

//    
void CS_Say2Camp(const char* szContent) {
	auto pk = net::msg::serialize(net::msg::CmSay2CampMessage{szContent});
	g_NetIF->SendPacketMessage(pk);
}

//  GM-
void CS_GMSend(DWORD dwNPCID, const char* szTitle, const char* szContent) {
	auto pk = net::msg::serialize(net::msg::CmGmSendMessage{(int64_t)dwNPCID, szTitle, szContent});
	g_NetIF->SendPacketMessage(pk);
}

//  GM-
void CS_GMRecv(DWORD dwNPCID) {
	auto pk = net::msg::serialize(net::msg::CmGmRecvMessage{(int64_t)dwNPCID});
	g_NetIF->SendPacketMessage(pk);
}

//void CS_PKCtrl(bool bCanPK)
//{
//	WPacket l_wpk	=g_NetIF->GetWPacket();
//	l_wpk.WriteCmd(CMD_CM_PK_CTRL);
//	l_wpk.WriteInt64(bCanPK? 1 : 0);
//	g_NetIF->SendPacketMessage(l_wpk);
//}

//    
void CS_CheatCheck(cChar* answer) {
	auto pk = net::msg::serialize(net::msg::CmCheatCheckMessage{answer});
	g_NetIF->SendPacketMessage(pk);
}

//  
//void CS_PKSilverSort(DWORD dwNPCID, short sItemPos)
//{
//    WPacket packet = g_NetIF->GetWPacket();
//    packet.WriteCmd(CMD_CM_GARNER2_REORDER);
//    packet.WriteInt64(dwNPCID);
//    packet.WriteInt64(sItemPos);
//    g_NetIF->SendPacketMessage(packet);
//}


//   
void CS_LifeSkill(long type, DWORD dwNPCID) {
	auto packet = net::msg::serialize(net::msg::CmLifeSkillMessage{(int64_t)type, (int64_t)dwNPCID});
	g_NetIF->SendPacketMessage(packet);
}


//  :  (Compose)
void CS_Compose(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */) {
	net::msg::CmLifeSkillCraftMessage msg;
	msg.isAnswer = asr;
	msg.skillType = 0;
	msg.npcId = (int64_t)dwNPCID;
	msg.positions.resize(iCount);
	for (int i = 0; i < iCount; i++)
		msg.positions[i] = (int64_t)(short)iPos[i];
	msg.hasExtra = false;
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

//  :  (Break)
void CS_Break(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */) {
	net::msg::CmLifeSkillCraftMessage msg;
	msg.isAnswer = asr;
	msg.skillType = 1;
	msg.npcId = (int64_t)dwNPCID;
	msg.positions.resize(iCount);
	for (int i = 0; i < iCount; i++)
		msg.positions[i] = (int64_t)(short)iPos[i];
	msg.hasExtra = false;
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

//  :  (Found)
void CS_Found(DWORD dwNPCID, int* iPos, int iCount, short big, bool asr /* = false */) {
	net::msg::CmLifeSkillCraftMessage msg;
	msg.isAnswer = asr;
	msg.skillType = 2;
	msg.npcId = (int64_t)dwNPCID;
	msg.positions.resize(iCount);
	for (int i = 0; i < iCount; i++)
		msg.positions[i] = (int64_t)(short)iPos[i];
	msg.hasExtra = true;
	msg.extraParam = (int64_t)big;
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

//  :  (Cooking)
void CS_Cooking(DWORD dwNPCID, int* iPos, int iCount, short percent, bool asr /* = false */) {
	net::msg::CmLifeSkillCraftMessage msg;
	msg.isAnswer = asr;
	msg.skillType = 3;
	msg.npcId = (int64_t)dwNPCID;
	msg.positions.resize(iCount);
	for (int i = 0; i < iCount; i++)
		msg.positions[i] = (int64_t)(short)iPos[i];
	msg.hasExtra = true;
	msg.extraParam = (int64_t)percent;
	auto packet = net::msg::serialize(msg);
	g_NetIF->SendPacketMessage(packet);
}

//    
void CS_UnlockCharacter() {
	auto packet = net::msg::serialize(net::msg::CmUnlockCharacterMessage{0});
	g_NetIF->SendPacketMessage(packet);
}

//   
void CS_AutionBidup(DWORD dwNPCID, short sItemID, uLong price) {
	auto packet = net::msg::serialize(net::msg::CmBidUpMessage{(int64_t)dwNPCID, (int64_t)sItemID, (int64_t)price});
	g_NetIF->SendPacketMessage(packet);
}

//   
void CS_AntiIndulgence_Close() {
	auto packet = net::msg::serializeCmAntiIndulgenceCmd();
	g_NetIF->SendPacketMessage(packet);
}

//    
void CS_DropLock(int slot) {
	auto pk = net::msg::serialize(net::msg::CmItemLockAskMessage{(int64_t)slot});
	g_NetIF->SendPacketMessage(pk);
}

//    
void CS_UnlockItem(const char szPassword[], int slot) {
	auto pk = net::msg::serialize(net::msg::CmItemUnlockAskMessage{szPassword, (int64_t)slot});
	g_NetIF->SendPacketMessage(pk);
}

//  PIN-
void CS_SendGameRequest(const char szPassword[]) {
	auto pk = net::msg::serialize(net::msg::CmGameRequestPinMessage{szPassword});
	g_NetIF->SendPacketMessage(pk);
}


//    
void CS_SetGuildPerms(DWORD ID, uLong Perms) {
	auto pk = net::msg::serialize(net::msg::CmGuildPermMessage{(int64_t)ID, (int64_t)Perms});
	g_NetIF->SendPacketMessage(pk);
}

//    
void CS_RequestDropRate() {
	auto pk = net::msg::serializeCmRequestDropRateCmd();
	g_NetIF->SendPacketMessage(pk);
}

//    
void CS_RequestExpRate() {
	auto pk = net::msg::serializeCmRequestExpRateCmd();
	g_NetIF->SendPacketMessage(pk);
}
