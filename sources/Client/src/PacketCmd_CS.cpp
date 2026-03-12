#include "StdAfx.h"
#include "PacketCmd.h"

#include "GameApp.h"
#include "Character.h"
#include "actor.h"
#include "procirculate.h"
#include "UIBoothForm.h"
#include "UIStoreForm.h"
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
	
	LG( "connect", g_oLangRec.GetString(294), hostname );
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
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_OFFLINE_MODE);
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

	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteUInt8(0);
	pk.WriteUInt8(pNetBank->chSrcType);
	pk.WriteUInt16(pNetBank->sSrcID);
	pk.WriteUInt16(pNetBank->sSrcNum);
	pk.WriteUInt8(pNetBank->chTarType);
	pk.WriteUInt16(pNetBank->sTarID);
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankTakeGold(int gold){
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteUInt8(1);
	pk.WriteUInt8(0);
	pk.WriteUInt32(gold);
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankGiveGold(int gold){
	if (g_pGameApp->GetCurScene()->GetMainCha()->IsBoat()) {
		g_pGameApp->MsgBox("Not available at sea");
		return;
	}
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteUInt8(1);
	pk.WriteUInt8(1);
	pk.WriteUInt32(gold);
	g_NetIF->SendPacketMessage(pk);
}

void CS_ChangePass(const char *pass,const char *pin)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_CHANGEPASS);	
//TODO(Ogge): Might need to hash it using blake2s?
	pk.WriteString(pass);	
	pk.WriteString(pin);
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
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_STALLSEARCH);	//�����ж�
	pk.WriteUInt32(ItemID);
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
    WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_LOCK);
	g_NetIF->SendPacketMessage(pk);
}

//��������
void CS_UnlockKitbag( const char szPassword[] )
{
    WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_UNLOCK);
    pk.WriteString(szPassword);
	g_NetIF->SendPacketMessage(pk);
}

//��鱳������״̬
void CS_KitbagCheck()
{
    WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_CHECK);
	g_NetIF->SendPacketMessage(pk);
}

// �����Զ�����״̬
void CS_AutoKitbagLock(bool bAutoLock)
{
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_AUTOLOCK);
	pk.WriteUInt8(bAutoLock ? 1 : 0);
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
	WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_DIE_RETURN);	//�����ж�
	pk.WriteUInt8(chReliveType);
	g_NetIF->SendPacketMessage(pk);

	// log
	LG(g_LogName.GetMainLogName(), "###Send(DieReturn):\tTick:[%d]\n", GetTickCount());
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
	//pk.WriteUInt32(g_NetIF->m_pingid);
	//++(g_NetIF->m_pingid);
	//if(g_NetIF->m_pingid>=20) g_NetIF->m_pingid = 0;

	//g_NetIF->SendPacketMessage(pk);
}

void CS_MapMask(const char *szMapName)
{
	WPacket pk	=g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_MAP_MASK);	//�����ж�
	pk.WriteString(szMapName);
	g_NetIF->SendPacketMessage(pk);

	// log
	LG(g_LogName.GetMainLogName(), "###Send(MapMask):\tTick:[%d]\n", GetTickCount());
	//
}

void CS_RequestTalk( DWORD dwNpcID, BYTE byCmd )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TALKPAGE );
	packet.WriteUInt8( byCmd );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelFunction( DWORD dwNpcID, BYTE byPageID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_FUNCITEM );
	packet.WriteUInt8( byPageID );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_Sale( DWORD dwNpcID, BYTE byIndex, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TRADEITEM );
	packet.WriteUInt8( ROLE_TRADE_SALE );
	packet.WriteUInt8( byIndex );
	packet.WriteUInt8( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_Buy(  DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TRADEITEM );
	packet.WriteUInt8( ROLE_TRADE_BUY );
	packet.WriteUInt8( byItemType );
	packet.WriteUInt8( byIndex1 );
	packet.WriteUInt8( byIndex2 );
	packet.WriteUInt8( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelectTradeBoat( DWORD dwNpcID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TRADEITEM );
	packet.WriteUInt8( ROLE_TRADE_SELECT_BOAT );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SaleGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TRADEITEM );
	packet.WriteUInt8( ROLE_TRADE_SALE_GOODS );
	packet.WriteUInt32( dwBoatID );
	packet.WriteUInt8( byIndex );
	packet.WriteUInt8( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_BuyGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_TRADEITEM );
	packet.WriteUInt8( ROLE_TRADE_BUY_GOODS );
	packet.WriteUInt32( dwBoatID );
	packet.WriteUInt8( byItemType );
	packet.WriteUInt8( byIndex1 );
	packet.WriteUInt8( byIndex2 );
	packet.WriteUInt8( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_BlackMarketExchangeReq( DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTRADE );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_BLACKMARKET_EXCHANGE_REQ );
	packet.WriteUInt16( sTimeNum );
	packet.WriteUInt16( sSrcID );
	packet.WriteUInt16( sSrcNum );
	packet.WriteUInt16( sTarID );
	packet.WriteUInt16( sTarNum );
	packet.WriteUInt16( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_RequestTrade( BYTE byType, DWORD dwCharID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_REQUEST );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );

	g_NetIF->SendPacketMessage( packet );
}

void CS_AcceptTrade( BYTE byType, DWORD dwCharID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_ACCEPT );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );

	g_NetIF->SendPacketMessage( packet );
}

void CS_AddItem( BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount )
{
    WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_ITEM );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );
	packet.WriteUInt8( byOpType );
	packet.WriteUInt8( byIndex );
	packet.WriteUInt8( byItemIndex );
	packet.WriteUInt8( byCount );

	g_NetIF->SendPacketMessage( packet );
}

void CS_AddMoney( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_MONEY );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );
	packet.WriteUInt8( byOpType );
	packet.WriteUInt8(0);
	packet.WriteUInt32( dwMoney );

	g_NetIF->SendPacketMessage( packet );
}

void CS_AddIMP( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_MONEY );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );
	packet.WriteUInt8( byOpType );
	packet.WriteUInt8(1);
	packet.WriteUInt32( dwMoney );

	g_NetIF->SendPacketMessage( packet );
}

void CS_CancelTrade( BYTE byType, DWORD dwCharID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_CANCEL );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );

	g_NetIF->SendPacketMessage( packet );
}

void CS_ValidateTradeData( BYTE byType, DWORD dwCharID )
{
    WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_VALIDATEDATA );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );

	g_NetIF->SendPacketMessage( packet );
}

void CS_ValidateTrade( BYTE byType, DWORD dwCharID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CHARTRADE_VALIDATE );
	packet.WriteUInt8( byType );
	packet.WriteUInt32( dwCharID );

	g_NetIF->SendPacketMessage( packet );
}

void CS_MissionPage( DWORD dwNpcID, BYTE byCmd, BYTE bySelItem, BYTE byParam )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_MISSION );
	packet.WriteUInt8( byCmd );
	packet.WriteUInt8( bySelItem );
	packet.WriteUInt8( byParam );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelMission( DWORD dwNpcID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_MISSION );
	packet.WriteUInt8( ROLE_MIS_SEL );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_MissionTalk( DWORD dwNpcID, BYTE byCmd )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_MISSION );
	packet.WriteUInt8( ROLE_MIS_TALK );
	packet.WriteUInt16( CMD_CM_TALKPAGE );
	packet.WriteUInt8( byCmd );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelMissionFunc( DWORD dwNpcID, BYTE byPageID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_REQUESTTALK );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( CMD_CM_MISSION );
	packet.WriteUInt8( ROLE_MIS_TALK );
	packet.WriteUInt16( CMD_CM_FUNCITEM );
	packet.WriteUInt8( byPageID );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_MisLog()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MISLOG );

	g_NetIF->SendPacketMessage( packet );	
}

void CS_MisLogInfo( WORD wID )
{
	if( wID == -1 )
		return;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MISLOGINFO );
	packet.WriteUInt16( wID );
	g_NetIF->SendPacketMessage( packet );
}

void CS_MisClear( WORD wID )
{
	if( wID == -1 )
		return;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MISLOG_CLEAR );
	packet.WriteUInt16( wID );
	g_NetIF->SendPacketMessage( packet );
}

void CS_ForgeItem( BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_FORGE );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_CreateBoat( const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CREATE_BOAT );
	packet.WriteString( szBoat );
	packet.WriteUInt8( szHeader );
	packet.WriteUInt8( szEngine );
	packet.WriteUInt8( szCannon );
	packet.WriteUInt8( szEquipment );

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
		WPacket packet = g_NetIF->GetWPacket();
		packet.WriteCmd( CMD_CM_BOAT_SELECT );
		packet.WriteUInt32( dwNpcID );
		packet.WriteUInt8( byType );
		packet.WriteUInt8( byIndex );
		g_NetIF->SendPacketMessage( packet );
	}
	
}

void CS_SelectBoat( DWORD dwNpcID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_BOAT_LUANCH );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_SelectBoatBag( DWORD dwNpcID, BYTE byIndex )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_BOAT_BAGSEL );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt8( byIndex );

	g_NetIF->SendPacketMessage( packet );
}

void CS_UpdateBoat( char szHeader, char szEngine, char szCannon, char szEquipment )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_UPDATEBOAT_PART );
	packet.WriteUInt8( szHeader );
	packet.WriteUInt8( szEngine );
	packet.WriteUInt8( szCannon );
	packet.WriteUInt8( szEquipment );

	g_NetIF->SendPacketMessage( packet );
}

void CS_CancelBoat()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_BOAT_CANCEL );

	g_NetIF->SendPacketMessage( packet );
}

void CS_GetBoatInfo()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_BOAT_GETINFO );

	g_NetIF->SendPacketMessage( packet );	
}

void CS_EntityEvent( DWORD dwEntityID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ENTITY_EVENT );
	packet.WriteUInt32( dwEntityID );

	g_NetIF->SendPacketMessage( packet );

	const char* szLogName = g_LogName.GetMainLogName();
	LG(szLogName, "###Send(Event-Entyty):\tTick:[%d]\n", GetTickCount());
	LG(szLogName, "\n");
}

void CS_StallInfo( const char szName[], mission::NET_STALL_ALLDATA& Data )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STALL_ALLDATA );
	packet.WriteString( szName );
	packet.WriteUInt8( Data.byNum );
	for( BYTE i = 0; i < Data.byNum; ++i )
	{
		packet.WriteUInt8( Data.Info[i].byGrid );
		packet.WriteUInt32( Data.Info[i].dwMoney );
		packet.WriteUInt8( Data.Info[i].byCount );
		packet.WriteUInt8( Data.Info[i].byIndex );
	}
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallOpen( DWORD dwCharID )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STALL_OPEN );
	packet.WriteUInt32( dwCharID );
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallBuy( DWORD dwCharID, BYTE byIndex, BYTE byCount ,int gridID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STALL_BUY );
	packet.WriteUInt32( dwCharID );
	packet.WriteUInt8( byIndex );
	packet.WriteUInt8( byCount );
	packet.WriteUInt8( gridID );
	g_NetIF->SendPacketMessage( packet );
}

void CS_StallClose()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STALL_CLOSE );
	g_NetIF->SendPacketMessage( packet );
}

//add by jilinlee 2007/4/20/////////////////////
void CS_ReadBookStart()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_READBOOK_START );
	g_NetIF->SendPacketMessage( packet );
}

void CS_ReadBookClose()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_READBOOK_CLOSE );
	g_NetIF->SendPacketMessage( packet );
}
///////////////////////////////////////////////

void CS_UpdateHair( stNetUpdateHair& stData )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_UPDATEHAIR );
	packet.WriteUInt16( stData.sScriptID );
	for(short i = 0; i < 4; i++)
	{
		packet.WriteUInt16( stData.sGridLoc[i] );
	}
	g_NetIF->SendPacketMessage( packet );
}

void CS_TeamFightAsk( unsigned long ulWorldID, long lHandle, char chType )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_TEAM_FIGHT_ASK );
	packet.WriteUInt8(chType);
	packet.WriteUInt32( ulWorldID );
	packet.WriteUInt32( lHandle );
	g_NetIF->SendPacketMessage( packet );
}

void CS_TeamFightAnswer(bool bAccess)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_TEAM_FIGHT_ASR );
	packet.WriteUInt8(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage( packet );
}

// lRepairmanID,lRepairmanHandle ����Ա
// chPosType��1��װ������2��������
// chPosID����Ӧλ�õı��
void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_REPAIR_ASK );
	packet.WriteUInt32( lRepairmanID );
	packet.WriteUInt32( lRepairmanHandle );
	packet.WriteUInt8(chPosType);
	packet.WriteUInt8(chPosID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemRepairAnswer(bool bAccess)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_REPAIR_ASR );
	packet.WriteUInt8(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk *pSForge)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_ASK);
	if (!bSure)
	{
		packet.WriteUInt8(0);
	}
	else
	{
		packet.WriteUInt8(1);
		packet.WriteUInt8(pSForge->chType);
		for (int i = 0; i < defMAX_FORGE_GROUP_NUM; i++)
		{
			packet.WriteUInt16(pSForge->SGroup[i].sCellNum);
			for (short j = 0; j < pSForge->SGroup[i].sCellNum; j++)
			{
				packet.WriteUInt16(pSForge->SGroup[i].pCell->sPosID);
				packet.WriteUInt16(pSForge->SGroup[i].pCell->sNum);
			}
		}
	}

	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAnswer(bool bAccess)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_FORGE_ASR );
	packet.WriteUInt8(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage( packet );
}

// Add by lark.li 20080514 begin
void CS_ItemLotteryAnswer(bool bAccess)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_LOTTERY_ASR );
	packet.WriteUInt8(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk *pSLottery)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_LOTTERY_ASK);
	if (!bSure)
	{
		packet.WriteUInt8(0);
	}
	else
	{
		packet.WriteUInt8(1);

		for (int i = 0; i < defMAX_LOTTERY_GROUP_NUM; i++)
		{
			packet.WriteUInt16(pSLottery->SGroup[i].sCellNum);
			for (short j = 0; j < pSLottery->SGroup[i].sCellNum; j++)
			{
				packet.WriteUInt16(pSLottery->SGroup[i].pCell->sPosID);
				packet.WriteUInt16(pSLottery->SGroup[i].pCell->sNum);
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
		packet.WriteUInt8(0);
	}
	else
	{
		packet.WriteUInt8(1);
		packet.WriteUInt16( ReID );
	}
	g_NetIF->SendPacketMessage( packet );
}

void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_ITEM_FORGE_ASK );

	if(! bSure)
	{
		packet.WriteUInt8(0);
	}
	else
	{
		packet.WriteUInt8(1);
		packet.WriteUInt8((char)(nType)); // ����������

		for(int i = 0; i < defMAX_FORGE_GROUP_NUM; ++i)
		{
			if(i < nPosCount)
			{
				packet.WriteUInt16((short) 1);		// cellnum
				packet.WriteUInt16(arPacketPos[i]);	// ����λ��
				packet.WriteUInt16((short) 1);		// ����
			}
			else
			{
				packet.WriteUInt16((short) 0);		// cellnum
				packet.WriteUInt16((short) 0);	// ����λ��
				packet.WriteUInt16((short) 0);		// ����
			}
		}
	}

	g_NetIF->SendPacketMessage( packet );
}


// ���̳�
void CS_StoreOpenAsk(const char szPassword[])
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_OPEN_ASK );
	packet.WriteString(szPassword);
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreClose()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_CLOSE );
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreListAsk(long lClsID, short sPage, short sNum)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_LIST_ASK );
	packet.WriteUInt32(lClsID);
	packet.WriteUInt16(sPage);
	packet.WriteUInt16(sNum);
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreBuyAsk(long lComID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_BUY_ASK );
	packet.WriteUInt32(lComID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreChangeAsk(long lNum)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_CHANGE_ASK );
	packet.WriteUInt32(lNum);
	g_NetIF->SendPacketMessage( packet );
}

void CS_StoreQuery(long lNum)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_STORE_QUERY );
	packet.WriteUInt32(lNum);
	g_NetIF->SendPacketMessage( packet );
}

//void CS_StoreVIP(short sVipID, short sMonth)
//{
//	WPacket packet = g_NetIF->GetWPacket();
//	packet.WriteCmd( CMD_CM_STORE_VIP );
//	packet.WriteUInt16(sVipID);
//	packet.WriteUInt16(sMonth);
//	g_NetIF->SendPacketMessage( packet );
//}

void CS_ReportWG( const char szInfo[] )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CP_REPORT_WG );
	packet.WriteString( szInfo );
	g_NetIF->SendPacketMessage( packet );
}

void CS_TigerStart( DWORD dwNpcID, short sSel1, short sSel2, short sSel3 )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_TIGER_START );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16(sSel1);
	packet.WriteUInt16(sSel2);
	packet.WriteUInt16(sSel3);
	g_NetIF->SendPacketMessage( packet );
}

void CS_TigerStop( DWORD dwNpcID, short sNum )
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_TIGER_STOP );
	packet.WriteUInt32( dwNpcID );
	packet.WriteUInt16( sNum );
	g_NetIF->SendPacketMessage( packet );
}

void CS_RequestDailyBuffInfo()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_DailyBuffRequest);
	g_NetIF->SendPacketMessage(packet);
}


void CS_VolunteerList(short sPage, short sNum)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_LIST );
	packet.WriteUInt16( sPage );
	packet.WriteUInt16( sNum );
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerAdd()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_ADD );
	g_NetIF->SendPacketMessage( packet );
}



void CS_VolunteerDel()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_DEL );
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerSel(const char *szName)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_SEL );
	packet.WriteString(szName);
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerOpen(short sNum)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_OPEN );
	packet.WriteUInt16( sNum );
	g_NetIF->SendPacketMessage( packet );
}

void CS_VolunteerAsr(BOOL bRet, const char *szName)
{
	short sRet = bRet ? 1 : 0;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_VOLUNTER_ASR );
	packet.WriteUInt16( sRet );
	packet.WriteString( szName );
	g_NetIF->SendPacketMessage( packet );
}

void CS_SyncKitbagTemp()
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_KITBAGTEMP_SYNC );
	g_NetIF->SendPacketMessage( packet );
}

// Add by lark.li 20080707 begin
void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_CAPTAIN_CONFIRM_ASR );
	packet.WriteUInt16(sRet);
	packet.WriteUInt32(dwTeamID);
	g_NetIF->SendPacketMessage( packet );
}
// End

void CS_MasterInvite(const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_INVITE );
	packet.WriteString(szName);
	packet.WriteUInt32(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_MasterAsr(short sRet, const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_ASR );
	packet.WriteUInt16(sRet);
	packet.WriteString(szName);
	packet.WriteUInt32(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeInvite(const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_INVITE );
	packet.WriteString(szName);
	packet.WriteUInt32(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeAsr(short sRet, const char *szName, DWORD dwCharID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_ASR );
	packet.WriteUInt16(sRet);
	packet.WriteString(szName);
	packet.WriteUInt32(dwCharID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_MasterDel(const char *szName, uLong ulChaID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_MASTER_DEL );
	packet.WriteString(szName);
	packet.WriteUInt32(ulChaID);
	g_NetIF->SendPacketMessage( packet );
}

void CS_PrenticeDel(const char *szName, uLong ulChaID)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd( CMD_CM_PRENTICE_DEL );
	packet.WriteString(szName);
	packet.WriteUInt32(ulChaID);
	g_NetIF->SendPacketMessage( packet );
}

void CP_Master_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_MASTER_REFRESH_INFO);
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Prentice_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PRENTICE_REFRESH_INFO);
	l_wpk.WriteUInt32(chaid);
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
	l_wpk.WriteUInt32(dwNPCID);
	l_wpk.WriteString(szTitle);
	l_wpk.WriteString(szContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_GMRecv(DWORD dwNPCID)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GM_RECV);
	l_wpk.WriteUInt32(dwNPCID);
	g_NetIF->SendPacketMessage(l_wpk);
}

//void CS_PKCtrl(bool bCanPK)
//{
//	WPacket l_wpk	=g_NetIF->GetWPacket();
//	l_wpk.WriteCmd(CMD_CM_PK_CTRL);
//	l_wpk.WriteUInt8(bCanPK? 1 : 0);
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
//    packet.WriteUInt32(dwNPCID);
//    packet.WriteUInt16(sItemPos);
//    g_NetIF->SendPacketMessage(packet);
//}


void CS_LifeSkill(long type, DWORD dwNPCID)
{
    WPacket packet = g_NetIF->GetWPacket();
    packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
    packet.WriteUInt32(type);
    packet.WriteUInt32(dwNPCID);
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
    packet.WriteUInt32(0);    //  ��֧
    packet.WriteUInt32(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteUInt16((short)iPos[i]);
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
    packet.WriteUInt32(1);    //  ��֧
    packet.WriteUInt32(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteUInt16((short)iPos[i]);
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
    packet.WriteUInt32(2);    //  ��֧
    packet.WriteUInt32(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteUInt16((short)iPos[i]);
    }
    packet.WriteUInt16(big);
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
    packet.WriteUInt32(3);    //  ��֧
    packet.WriteUInt32(dwNPCID);
    for(int i = 0; i < iCount; i++)
    {
        packet.WriteUInt16((short)iPos[i]);
    }
    packet.WriteUInt16(percent);
    g_NetIF->SendPacketMessage(packet);
}

void CS_UnlockCharacter()
{
    WPacket packet = g_NetIF->GetWPacket();
    packet.WriteCmd(CMD_CM_ITEM_FORGE_CANACTION);
    packet.WriteUInt16(0);
    g_NetIF->SendPacketMessage(packet);
}
//add by ALLEN 2007-10-19
void CS_AutionBidup(DWORD dwNPCID, short sItemID, uLong price)
{
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BIDUP);
	packet.WriteUInt32(dwNPCID);
	packet.WriteUInt16(sItemID);
	packet.WriteUInt32(price);
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
	pk.WriteUInt8(	slot	);
	g_NetIF->SendPacketMessage(	pk	);
}

void CS_UnlockItem( const char szPassword[], int slot)	{
    WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_ITEM_UNLOCK_ASK);
    pk.WriteString(szPassword);
	pk.WriteUInt8(	slot	);
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
    pk.WriteUInt32(ID);
    pk.WriteUInt32(Perms);
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









