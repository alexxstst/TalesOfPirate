#include "StdAfx.h"
#include "PacketCmd.h"

#include "GameApp.h"
#include "Character.h"
#include "actor.h"
#include "procirculate.h"
#include "UIBoothForm.h"
#include "UIStoreForm.h"
#include "CommandMessages.h"
// Типы uChar, uShort, uLong, cChar определены в NetIF.h
// Crypto++ — BLAKE2s для хеширования пароля
#include "blake2.h"
#include "hex.h"
#include "filters.h"
//------------------------
// Э��C->S : ����
//------------------------
bool CS_Connect(cChar *hostname, uint16_t port, uint32_t timeout)
{
	
	{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(294), hostname); g_logManager.InternalLog(LogLevel::Debug, "connections", _buf); }
    if( g_NetIF->m_pCProCir )
    {
        delete g_NetIF->m_pCProCir;
    }
    g_NetIF->m_pCProCir = new CProCirculateCS( g_NetIF );

	discordInit();
	updateDiscordPresence("Connecting...", "");



	return g_NetIF->m_pCProCir->Connect(hostname,port,timeout);
}

//------------------------
// Э��C->S : �Ͽ�����
//------------------------
void CS_Disconnect(int reason)
{
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->Disconnect(reason);
}

void CS_SendPrivateKey()
{
	g_NetIF->m_pCProCir->SendPrivateKey();
}



//------------------------
// Э��C->S : ��½
//------------------------
void CS_Login(const char *accounts,const char *password, const char* passport)
{
	g_NetIF->m_pCProCir->Login(accounts,password,passport);
}

//------------------------
// Э��C->S : ���͵ǳ�����
//------------------------
void CS_Logout()
{
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->Logout();
	return;
}

void CS_OfflineMode()
{
	auto pk = net::msg::serializeCmOfflineModeCmd();
	g_NetIF->SendPacketMessage(pk);
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
}

void CS_CancelExit()
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_CANCELEXIT);	//�����ж�
	g_NetIF->SendPacketMessage(pk);
}

//------------------------
// Э��C->S : ����ѡ���ɫ����
//------------------------
void CS_BeginPlay(char cha_index)
{
	g_NetIF->m_pCProCir->BeginPlay(cha_index);
}

//------------------------
// Э��C->S : ��������ѡ���ɫ����
//------------------------
void CS_EndPlay()
{
	//fix stall bugs with offline stalls @mothannakh
	g_stUIBooth.PullBoothSuccess();
	g_NetIF->m_pCProCir->EndPlay();
}

//------------------------
// Э��C->S : �����½���ɫ����
//------------------------

void CS_NewCha(const char *chaname,const char *birth,int type, int hair, int face)
{
	g_NetIF->m_pCProCir->NewCha(chaname,birth,type,hair,face);
}

//------------------------
// Э��C->S : ����ɾ����ɫ����
//------------------------
void CS_DelCha(uint8_t cha_index, const char szPassword2[])
{
	g_NetIF->m_pCProCir->DelCha(cha_index, szPassword2);
}

//------------------------
// Э��C->S : ����������Ϣ
//------------------------
void CS_Say(const char *content){
	g_NetIF->m_pCProCir->Say(content);
}

void CS_GuildBankOper(stNetBank * pNetBank){
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}

	auto pk = net::msg::serialize(net::msg::CmGuildBankOperMessage{0, pNetBank->chSrcType, pNetBank->sSrcID, pNetBank->sSrcNum, pNetBank->chTarType, pNetBank->sTarID});
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankTakeGold(int gold){
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	auto pk = net::msg::serialize(net::msg::CmGuildBankGoldMessage{1, 0, gold});
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankGiveGold(int gold){
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	auto pk = net::msg::serialize(net::msg::CmGuildBankGoldMessage{1, 1, gold});
	g_NetIF->SendPacketMessage(pk);
}

void CS_ChangePass(const char *pass,const char *pin)
{
//TODO(Ogge): Might need to hash it using blake2s?
	auto pk = net::msg::serialize(net::msg::CmChangePassMessage{pass, pin});
	g_NetIF->SendPacketMessage(pk);
}

void CS_Register(const char *user,const char *pass,const char *email)
{
	/*
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REGISTER);
	pk.WriteString(user);
	
	//char szBuf[33]={ 0 };
	//md5string(pass, szBuf);
	std::string digest;
	std::string hexencoded;
	CryptoPP::BLAKE2s d;
	CryptoPP::StringSource(pass, true, new CryptoPP::HashFilter(d, new CryptoPP::StringSink(digest)));
	CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexencoded)));
	pk.WriteString(hexencoded.c_str());
	pk.WriteString(email);
	g_NetIF->SendPacketMessage(pk);
	*/
}

void CS_StallSearch(int ItemID)
{
	auto pk = net::msg::serialize(net::msg::CmStallSearchMessage{ItemID});
	g_NetIF->SendPacketMessage(pk);
}

// ������������
void CS_CreatePassword2( const char szPassword[] )
{
	WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_CREATE_PASSWORD2);	//�����ж�
	pk.WriteString(szPassword);

	g_NetIF->SendPacketMessage(pk);
}

void CS_UpdatePassword2( const char szOld[], const char szPassword[] )
{
	WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_UPDATE_PASSWORD2);	//�����ж�
	pk.WriteString(szOld);
	pk.WriteString(szPassword);

	g_NetIF->SendPacketMessage(pk);
}

//��������
void CS_LockKitbag()
{
	auto pk = net::msg::serializeCmKitbagLockCmd();
	g_NetIF->SendPacketMessage(pk);
}

//��������
void CS_UnlockKitbag( const char szPassword[] )
{
	auto pk = net::msg::serialize(net::msg::CmKitbagUnlockMessage{szPassword});
	g_NetIF->SendPacketMessage(pk);
}

//��鱳������״̬
void CS_KitbagCheck()
{
	auto pk = net::msg::serializeCmKitbagCheckCmd();
	g_NetIF->SendPacketMessage(pk);
}

// �����Զ�����״̬
void CS_AutoKitbagLock(bool bAutoLock)
{
	auto pk = net::msg::serialize(net::msg::CmAutoKitbagLockMessage{bAutoLock ? 1 : 0});
	g_NetIF->SendPacketMessage(pk);
}

// Э��C->S : �����ж���Ϣ
void CS_BeginAction(  CCharacter* pCha, DWORD type, void* param, CActionState* pState )
{
	g_NetIF->m_pCProCir->BeginAction( pCha, type, param, pState );
}

// Э��C->S : ����ֹͣ�ж���Ϣ
void CS_EndAction( CActionState* pState )
{
	g_NetIF->m_pCProCir->EndAction(pState);
}

// Э��C->S : ����ֹͣ�ж���Ϣ
void CS_DieReturn(char chReliveType)
{
	auto pk = net::msg::serialize(net::msg::CmDieReturnMessage{chReliveType});
	g_NetIF->SendPacketMessage(pk);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("###Send(DieReturn):\tTick:[{}]", GetTickCount()));
	//
}

//----------------------------
//Э��C->S	: Ping��Ϸ������
//----------------------------
void CS_SendPing()
{
	return;
	//WPacket pk	= g_NetIF->GetWPacket();
	//pk.WriteCmd(CMD_CM_PING);

	//g_NetIF->dwLatencyTime[g_NetIF->m_pingid] = GetTickCount();
	//pk.WriteInt64(g_NetIF->m_pingid);
	//++(g_NetIF->m_pingid);
	//if(g_NetIF->m_pingid>=20) g_NetIF->m_pingid = 0;

	//g_NetIF->SendPacketMessage(pk);
}

void CS_MapMask(const char *szMapName)
{
	auto pk = net::msg::serialize(net::msg::CmMapMaskMessage{szMapName});
	g_NetIF->SendPacketMessage(pk);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("###Send(MapMask):\tTick:[{}]", GetTickCount()));
	//
}

void CS_RequestTalk( DWORD dwNpcID, BYTE byCmd )
{
	auto packet = net::msg::serialize(net::msg::CmRequestTalkMessage{(int64_t)dwNpcID, (int64_t)byCmd});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SelFunction( DWORD dwNpcID, BYTE byPageID, BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmSelFunctionMessage{(int64_t)dwNpcID, (int64_t)byPageID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_Sale( DWORD dwNpcID, BYTE byIndex, BYTE byCount )
{
	auto packet = net::msg::serialize(net::msg::CmSaleMessage{(int64_t)dwNpcID, (int64_t)byIndex, (int64_t)byCount});
	g_NetIF->SendPacketMessage( packet );
}

void CS_Buy(  DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount )
{
	auto packet = net::msg::serialize(net::msg::CmBuyMessage{(int64_t)dwNpcID, (int64_t)byItemType, (int64_t)byIndex1, (int64_t)byIndex2, (int64_t)byCount});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SelectTradeBoat( DWORD dwNpcID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteInt64( dwNpcID );
	packet.WriteInt64( CMD_CM_TRADEITEM );
	packet.WriteInt64( ROLE_TRADE_SELECT_BOAT );
	packet.WriteInt64( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SaleGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteInt64( dwNpcID );
	packet.WriteInt64( CMD_CM_TRADEITEM );
	packet.WriteInt64( ROLE_TRADE_SALE_GOODS );
	packet.WriteInt64( dwBoatID );
	packet.WriteInt64( byIndex );
	packet.WriteInt64( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_BuyGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteInt64( dwNpcID );
	packet.WriteInt64( CMD_CM_TRADEITEM );
	packet.WriteInt64( ROLE_TRADE_BUY_GOODS );
	packet.WriteInt64( dwBoatID );
	packet.WriteInt64( byItemType );
	packet.WriteInt64( byIndex1 );
	packet.WriteInt64( byIndex2 );
	packet.WriteInt64( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_BlackMarketExchangeReq( DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum, BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmBlackMarketExchangeReqMessage{(int64_t)dwNpcID, (int64_t)sTimeNum, (int64_t)sSrcID, (int64_t)sSrcNum, (int64_t)sTarID, (int64_t)sTarNum, (int64_t)byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_RequestTrade( BYTE byType, DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmRequestTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_AcceptTrade( BYTE byType, DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmAcceptTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_AddItem( BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount )
{
	auto packet = net::msg::serialize(net::msg::CmAddItemMessage{byType, dwCharID, byOpType, byIndex, byItemIndex, byCount});
	g_NetIF->SendPacketMessage( packet );
}

void CS_AddMoney( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney )
{
	auto packet = net::msg::serialize(net::msg::CmAddMoneyMessage{byType, dwCharID, byOpType, 0, dwMoney});
	g_NetIF->SendPacketMessage( packet );
}

void CS_AddIMP( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney )
{
	auto packet = net::msg::serialize(net::msg::CmAddMoneyMessage{byType, dwCharID, byOpType, 1, dwMoney});
	g_NetIF->SendPacketMessage( packet );
}

void CS_CancelTrade( BYTE byType, DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmCancelTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ValidateTradeData( BYTE byType, DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmValidateTradeDataMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ValidateTrade( BYTE byType, DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmValidateTradeMessage{byType, dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_MissionPage( DWORD dwNpcID, BYTE byCmd, BYTE bySelItem, BYTE byParam )
{
	auto packet = net::msg::serialize(net::msg::CmMissionPageMessage{(int64_t)dwNpcID, (int64_t)byCmd, (int64_t)bySelItem, (int64_t)byParam});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SelMission( DWORD dwNpcID, BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmSelMissionMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_MissionTalk( DWORD dwNpcID, BYTE byCmd )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteInt64( dwNpcID );
	packet.WriteInt64( CMD_CM_MISSION );
	packet.WriteInt64( ROLE_MIS_TALK );
	packet.WriteInt64( CMD_CM_TALKPAGE );
	packet.WriteInt64( byCmd );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelMissionFunc( DWORD dwNpcID, BYTE byPageID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteInt64( dwNpcID );
	packet.WriteInt64( CMD_CM_MISSION );
	packet.WriteInt64( ROLE_MIS_TALK );
	packet.WriteInt64( CMD_CM_FUNCITEM );
	packet.WriteInt64( byPageID );
	packet.WriteInt64( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_MisLog()
{
	auto packet = net::msg::serializeCmMisLogCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_MisLogInfo( WORD wID )
{
	if( wID == -1 )
		return;
	auto packet = net::msg::serialize(net::msg::CmMisLogInfoMessage{wID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_MisClear( WORD wID )
{
	if( wID == -1 )
		return;
	auto packet = net::msg::serialize(net::msg::CmMisLogClearMessage{wID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ForgeItem( BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmForgeItemMessage{byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_CreateBoat( const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment )
{
	auto packet = net::msg::serialize(net::msg::CmCreateBoatMessage{szBoat, szHeader, szEngine, szCannon, szEquipment});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SelectBoatList( DWORD dwNpcID, BYTE byType, BYTE byIndex )
{

	if( byType == mission::BERTH_TRADE_LIST )
	{
		CS_SelectTradeBoat( dwNpcID, byIndex );
	}
	else
	{
		auto packet = net::msg::serialize(net::msg::CmSelectBoatListMessage{(int64_t)dwNpcID, (int64_t)byType, (int64_t)byIndex});
		g_NetIF->SendPacketMessage( packet );
	}

}

void CS_SelectBoat( DWORD dwNpcID, BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmBoatLaunchMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SelectBoatBag( DWORD dwNpcID, BYTE byIndex )
{
	auto packet = net::msg::serialize(net::msg::CmBoatBagSelMessage{(int64_t)dwNpcID, (int64_t)byIndex});
	g_NetIF->SendPacketMessage( packet );
}

void CS_UpdateBoat( char szHeader, char szEngine, char szCannon, char szEquipment )
{
	auto packet = net::msg::serialize(net::msg::CmUpdateBoatMessage{szHeader, szEngine, szCannon, szEquipment});
	g_NetIF->SendPacketMessage( packet );
}

void CS_CancelBoat()
{
	auto packet = net::msg::serializeCmBoatCancelCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_GetBoatInfo()
{
	auto packet = net::msg::serializeCmBoatGetInfoCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_EntityEvent( DWORD dwEntityID )
{
	auto packet = net::msg::serialize(net::msg::CmEntityEventMessage{(int64_t)dwEntityID});
	g_NetIF->SendPacketMessage( packet );

	const char* szLogName = g_LogName.GetMainLogName();
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("###Send(Event-Entyty):\tTick:[{}]", GetTickCount()));

}

void CS_StallInfo( const char szName[], mission::NET_STALL_ALLDATA& Data )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STALL_ALLDATA );
	packet.WriteString( szName );
	packet.WriteInt64( Data.byNum );
	for( BYTE i = 0; i < Data.byNum; ++i )
	{
		packet.WriteInt64( Data.Info[i].byGrid );
		packet.WriteInt64( Data.Info[i].dwMoney );
		packet.WriteInt64( Data.Info[i].byCount );
		packet.WriteInt64( Data.Info[i].byIndex );
	}
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallOpen( DWORD dwCharID )
{
	auto packet = net::msg::serialize(net::msg::CmStallOpenMessage{(int64_t)dwCharID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallBuy( DWORD dwCharID, BYTE byIndex, BYTE byCount ,int gridID)
{
	auto packet = net::msg::serialize(net::msg::CmStallBuyMessage{(int64_t)dwCharID, (int64_t)byIndex, (int64_t)byCount, (int64_t)gridID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallClose()
{
	auto packet = net::msg::serializeCmStallCloseCmd();
	g_NetIF->SendPacketMessage( packet );
}

//add by jilinlee 2007/4/20/////////////////////
void CS_ReadBookStart()
{
	auto packet = net::msg::serializeCmReadBookStartCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_ReadBookClose()
{
	auto packet = net::msg::serializeCmReadBookCloseCmd();
	g_NetIF->SendPacketMessage( packet );
}
///////////////////////////////////////////////

void CS_UpdateHair( stNetUpdateHair& stData )
{
	auto packet = net::msg::serialize(net::msg::CmUpdateHairMessage{stData.sScriptID, stData.sGridLoc[0], stData.sGridLoc[1], stData.sGridLoc[2], stData.sGridLoc[3]});
	g_NetIF->SendPacketMessage( packet );
}

void CS_TeamFightAsk( unsigned long ulWorldID, long lHandle, char chType )
{
	auto packet = net::msg::serialize(net::msg::CmTeamFightAskMessage{chType, (int64_t)ulWorldID, lHandle});
	g_NetIF->SendPacketMessage( packet );
}

void CS_TeamFightAnswer(bool bAccess)
{
	auto packet = net::msg::serialize(net::msg::CmTeamFightAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage( packet );
}

// lRepairmanID,lRepairmanHandle ����Ա
// chPosType��1��װ������2��������
// chPosID����Ӧλ�õı��
void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID)
{
	auto packet = net::msg::serialize(net::msg::CmItemRepairAskMessage{lRepairmanID, lRepairmanHandle, chPosType, chPosID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemRepairAnswer(bool bAccess)
{
	auto packet = net::msg::serialize(net::msg::CmItemRepairAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk *pSForge)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_ASK);
	if (!bSure)
	{
		packet.WriteInt64(0);
	}
	else
	{
		packet.WriteInt64(1);
		packet.WriteInt64(pSForge->chType);
		for (int i = 0; i < defMAX_FORGE_GROUP_NUM; i++)
		{
			packet.WriteInt64(pSForge->SGroup[i].sCellNum);
			for (short j = 0; j < pSForge->SGroup[i].sCellNum; j++)
			{
				packet.WriteInt64(pSForge->SGroup[i].pCell->sPosID);
				packet.WriteInt64(pSForge->SGroup[i].pCell->sNum);
			}
		}
	}

	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAnswer(bool bAccess)
{
	auto packet = net::msg::serialize(net::msg::CmItemForgeAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage( packet );
}

// Add by lark.li 20080514 begin
void CS_ItemLotteryAnswer(bool bAccess)
{
	auto packet = net::msg::serialize(net::msg::CmItemLotteryAnswerMessage{bAccess ? 1 : 0});
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk *pSLottery)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_LOTTERY_ASK);
	if (!bSure)
	{
		packet.WriteInt64(0);
	}
	else
	{
		packet.WriteInt64(1);

		for (int i = 0; i < defMAX_LOTTERY_GROUP_NUM; i++)
		{
			packet.WriteInt64(pSLottery->SGroup[i].sCellNum);
			for (short j = 0; j < pSLottery->SGroup[i].sCellNum; j++)
			{
				packet.WriteInt64(pSLottery->SGroup[i].pCell->sPosID);
				packet.WriteInt64(pSLottery->SGroup[i].pCell->sNum);
			}
		}
	}

	g_NetIF->SendPacketMessage( packet );
}
// End
//Add by sunny.sun 20080726
void CS_ItemAmphitheaterAsk(bool bSure,int ReID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_AMPHITHEATER_ASK);
	if(!bSure)
	{
		packet.WriteInt64(0);
	}
	else
	{
		packet.WriteInt64(1);
		packet.WriteInt64( ReID );
	}
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_FORGE_ASK );

	if(! bSure)
	{
		packet.WriteInt64(0);
	}
	else
	{
		packet.WriteInt64(1);
		packet.WriteInt64((char)(nType)); // ����������

		for(int i = 0; i < defMAX_FORGE_GROUP_NUM; ++i)
		{
			if(i < nPosCount)
			{
				packet.WriteInt64((short) 1);		// cellnum
				packet.WriteInt64(arPacketPos[i]);	// ����λ��
				packet.WriteInt64((short) 1);		// ����
			}
			else
			{
				packet.WriteInt64((short) 0);		// cellnum
				packet.WriteInt64((short) 0);	// ����λ��
				packet.WriteInt64((short) 0);		// ����
			}
		}
	}

	g_NetIF->SendPacketMessage( packet );
}


// ���̳�
void CS_StoreOpenAsk(const char szPassword[])
{
	auto packet = net::msg::serialize(net::msg::CmStoreOpenAskMessage{szPassword});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreClose()
{
	auto packet = net::msg::serializeCmStoreCloseCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreListAsk(long lClsID, short sPage, short sNum)
{
	auto packet = net::msg::serialize(net::msg::CmStoreListAskMessage{lClsID, sPage, sNum});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreBuyAsk(long lComID)
{
	auto packet = net::msg::serialize(net::msg::CmStoreBuyAskMessage{lComID});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreChangeAsk(long lNum)
{
	auto packet = net::msg::serialize(net::msg::CmStoreChangeAskMessage{lNum});
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreQuery(long lNum)
{
	auto packet = net::msg::serialize(net::msg::CmStoreQueryMessage{lNum});
	g_NetIF->SendPacketMessage( packet );
}

//void CS_StoreVIP(short sVipID, short sMonth)
//{
//	WPacket packet = g_NetIF->GetWPacket();
//	packet.WriteCmd( CMD_CM_STORE_VIP );
//	packet.WriteInt64(sVipID);
//	packet.WriteInt64(sMonth);
//	g_NetIF->SendPacketMessage( packet );
//}

void CS_ReportWG( const char szInfo[] )
{
	auto packet = net::msg::serialize(net::msg::CmReportWgMessage{szInfo});
	g_NetIF->SendPacketMessage( packet );
}

void CS_TigerStart( DWORD dwNpcID, short sSel1, short sSel2, short sSel3 )
{
	auto packet = net::msg::serialize(net::msg::CmTigerStartMessage{(int64_t)dwNpcID, sSel1, sSel2, sSel3});
	g_NetIF->SendPacketMessage( packet );
}

void CS_TigerStop( DWORD dwNpcID, short sNum )
{
	auto packet = net::msg::serialize(net::msg::CmTigerStopMessage{(int64_t)dwNpcID, sNum});
	g_NetIF->SendPacketMessage( packet );
}

void CS_RequestDailyBuffInfo()
{
	auto packet = net::msg::serializeCmDailyBuffRequestCmd();
	g_NetIF->SendPacketMessage(packet);
}


void CS_VolunteerList(short sPage, short sNum)
{
	auto packet = net::msg::serialize(net::msg::CmVolunteerListMessage{sPage, sNum});
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerAdd()
{
	auto packet = net::msg::serializeCmVolunteerAddCmd();
	g_NetIF->SendPacketMessage( packet );
}



void CS_VolunteerDel()
{
	auto packet = net::msg::serializeCmVolunteerDelCmd();
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerSel(const char *szName)
{
	auto packet = net::msg::serialize(net::msg::CmVolunteerSelMessage{szName});
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerOpen(short sNum)
{
	auto packet = net::msg::serialize(net::msg::CmVolunteerOpenMessage{sNum});
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerAsr(BOOL bRet, const char *szName)
{
	auto packet = net::msg::serialize(net::msg::CmVolunteerAsrMessage{bRet ? 1 : 0, szName});
	g_NetIF->SendPacketMessage( packet );
}

void CS_SyncKitbagTemp()
{
	auto packet = net::msg::serializeCmKitbagTempSyncCmd();
	g_NetIF->SendPacketMessage( packet );
}

// Add by lark.li 20080707 begin
void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID)
{
	auto packet = net::msg::serialize(net::msg::CmCaptainConfirmAsrMessage{sRet, (int64_t)dwTeamID});
	g_NetIF->SendPacketMessage( packet );
}
// End

void CS_MasterInvite(const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_INVITE );
	packet.WriteString(szName);
	packet.WriteInt64(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_MasterAsr(short sRet, const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_ASR );
	packet.WriteInt64(sRet);
	packet.WriteString(szName);
	packet.WriteInt64(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeInvite(const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_INVITE );
	packet.WriteString(szName);
	packet.WriteInt64(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeAsr(short sRet, const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_ASR );
	packet.WriteInt64(sRet);
	packet.WriteString(szName);
	packet.WriteInt64(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_MasterDel(const char *szName, uLong ulChaID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_DEL );
	packet.WriteString(szName);
	packet.WriteInt64(ulChaID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeDel(const char *szName, uLong ulChaID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_DEL );
	packet.WriteString(szName);
	packet.WriteInt64(ulChaID);
	g_NetIF->SendPacketMessage( packet );
}

void CP_Master_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_MASTER_REFRESH_INFO);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Prentice_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PRENTICE_REFRESH_INFO);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_Say2Camp(const char *szContent)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_SAY2CAMP);
	l_wpk.WriteString(szContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_GMSend(DWORD dwNPCID, const char *szTitle, const char *szContent)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GM_SEND);
	l_wpk.WriteInt64(dwNPCID);
	l_wpk.WriteString(szTitle);
	l_wpk.WriteString(szContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_GMRecv(DWORD dwNPCID)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GM_RECV);
	l_wpk.WriteInt64(dwNPCID);
	g_NetIF->SendPacketMessage(l_wpk);
}

//void CS_PKCtrl(bool bCanPK)
//{
//	WPacket l_wpk	=g_NetIF->GetWPacket();
//	l_wpk.WriteCmd(CMD_CM_PK_CTRL);
//	l_wpk.WriteInt64(bCanPK? 1 : 0);
//	g_NetIF->SendPacketMessage(l_wpk);
//}

void CS_CheatCheck(cChar *answer)
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_CHEAT_CHECK);
	l_wpk.WriteString(answer);
	g_NetIF->SendPacketMessage(l_wpk);
}

//  ���������ǣ�ˢ������
//void CS_PKSilverSort(DWORD dwNPCID, short sItemPos)
//{
//    WPacket packet = g_NetIF->GetWPacket();
//    packet.WriteCmd(CMD_CM_GARNER2_REORDER);
//    packet.WriteInt64(dwNPCID);
//    packet.WriteInt64(sItemPos);
//    g_NetIF->SendPacketMessage(packet);
//}


void CS_LifeSkill(long type, DWORD dwNPCID)
{
    WPacket packet = g_NetIF->GetWPacket();
    packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    packet.WriteInt64(type);
    packet.WriteInt64(dwNPCID);
    g_NetIF->SendPacketMessage(packet);
}


void CS_Compose(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */)
{
    WPacket packet = g_NetIF->GetWPacket();
    if(asr)
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    }
    else
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
    }
    packet.WriteInt64(0);    //  ��֧
    packet.WriteInt64(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteInt64((short)iPos[i]);
    }
    g_NetIF->SendPacketMessage(packet);
}

void CS_Break(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */)
{
    WPacket packet = g_NetIF->GetWPacket();
    if(asr)
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    }
    else
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
    }
    packet.WriteInt64(1);    //  ��֧
    packet.WriteInt64(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteInt64((short)iPos[i]);
    }
    g_NetIF->SendPacketMessage(packet);
}

void CS_Found(DWORD dwNPCID, int* iPos, int iCount, short big, bool asr /* = false */)
{
    WPacket packet = g_NetIF->GetWPacket();
    if(asr)
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    }
    else
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
    }
    packet.WriteInt64(2);    //  ��֧
    packet.WriteInt64(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteInt64((short)iPos[i]);
    }
    packet.WriteInt64(big);
    g_NetIF->SendPacketMessage(packet);
}

void CS_Cooking(DWORD dwNPCID, int* iPos, int iCount, short percent, bool asr /* = false */)
{
    WPacket packet = g_NetIF->GetWPacket();
    if(asr)
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    }
    else
    {
        packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
    }
    packet.WriteInt64(3);    //  ��֧
    packet.WriteInt64(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteInt64((short)iPos[i]);
    }
    packet.WriteInt64(percent);
    g_NetIF->SendPacketMessage(packet);
}

void CS_UnlockCharacter()
{
    WPacket packet = g_NetIF->GetWPacket();
    packet.WriteCmd(CMD_CM_ITEM_FORGE_CANACTION);
    packet.WriteInt64(0);
    g_NetIF->SendPacketMessage(packet);
}
//add by ALLEN 2007-10-19
void CS_AutionBidup(DWORD dwNPCID, short sItemID, uLong price)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BIDUP);
	packet.WriteInt64(dwNPCID);
	packet.WriteInt64(sItemID);
	packet.WriteInt64(price);
	g_NetIF->SendPacketMessage(packet);
}

void CS_AntiIndulgence_Close()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ANTIINDULGENCE);
    g_NetIF->SendPacketMessage(packet);
}

void	CS_DropLock(int slot){
	WPacket	pk	=	g_NetIF->GetWPacket();
	pk.WriteCmd(	CMD_CM_ITEM_LOCK_ASK	);
	pk.WriteInt64(	slot	);
	g_NetIF->SendPacketMessage(	pk	);
}

void CS_UnlockItem( const char szPassword[], int slot)	{
    WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_ITEM_UNLOCK_ASK);
    pk.WriteString(szPassword);
	pk.WriteInt64(	slot	);
	g_NetIF->SendPacketMessage(pk);
}

void CS_SendGameRequest( const char szPassword[])	{
    WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_GAME_REQUEST_PIN);
	pk.WriteString(szPassword);
	g_NetIF->SendPacketMessage(pk);
}


void CS_SetGuildPerms( DWORD ID, uLong Perms ){
    WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_GUILD_PERM);
    pk.WriteInt64(ID);
    pk.WriteInt64(Perms);
	g_NetIF->SendPacketMessage(pk);
}

void CS_RequestDropRate() {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REQUEST_DROP_RATE);
	g_NetIF->SendPacketMessage(pk);
}

void CS_RequestExpRate() {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REQUEST_EXP_RATE);
	g_NetIF->SendPacketMessage(pk);
}









