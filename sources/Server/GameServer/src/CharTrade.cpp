// CharTrade.cpp Created by knight-gongjian 2004.12.7.
//---------------------------------------------------------
#include "stdafx.h"
#include "CharTrade.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "Player.h"
#include "GameDB.h"
#include "lua_gamectrl.h"
#include "CommandMessages.h"
//---------------------------------------------------------
using namespace std;

mission::CTradeSystem g_TradeSystem;

mission::CStoreSystem g_StoreSystem;

namespace mission
{
	//----------------------------------------------------
	// CTradeData implemented

	CTradeData::CTradeData(dbc::uLong lSize)
	: PreAllocStru(1)
	{

	}

	CTradeData::~CTradeData()
	{

	}

	//----------------------------------------------------
	// CTradeSystem implemented

	CTradeSystem::CTradeSystem()
	{

	}

	CTradeSystem::~CTradeSystem()
	{

	}

	// 
	BOOL CTradeSystem::Request( BYTE byType, CCharacter& character, DWORD dwAcceptID )
	{
		if(character.GetPlyMainCha()->IsStoreEnable())
		{
			//character.SystemNotice("!");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			return FALSE;
		}

		if( character.GetBoat() )
		{
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00002) );
			return FALSE;
		}

		if( character.GetStallData() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
		if(character.IsReadBook())
		{
			//character.SystemNotice("");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00004));
			return FALSE;
		}

		if( character.m_CKitbag.IsLock() || !character.GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

		if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsLock() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
            //character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00006) );
			return FALSE;
        }

		CCharacter* pMain = &character;
		CCharacter* pChar = pMain->GetSubMap()->FindCharacter( dwAcceptID, pMain->GetShape().centre );
		if( pChar == NULL || !pChar->IsPlayerCha() ) 
		{
			//pMain->SystemNotice( "!" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00007) );
			return FALSE;
		}

        if(pChar->GetPlayer()->GetBankNpc())
        {
            //pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00008)  );
            return FALSE;
        }

		if(pChar->GetPlyMainCha()->IsStoreEnable())
		{
			//character.SystemNotice("!");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			return FALSE;
		}

		if( !pMain->GetPlyMainCha() || !pChar->GetPlyMainCha() )
		{
			/*pMain->SystemNotice( "" );
			pChar->SystemNotice( "" );*/
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if(pMain->GetPlyMainCha()->GetLevel() < 6)
		{
			//pMain->SystemNotice(",!");
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00011));
			return FALSE;
		}

		if( pChar->GetBoat() )
		{
			//character.SystemNotice( "%s", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00012), pChar->GetName() );
			return FALSE;
		}

		if( pChar->GetStallData() )
		{
			//character.SystemNotice( "%s", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00013), pChar->GetName() );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
		if( pChar->IsReadBook() )
		{
			//character.SystemNotice( "%s", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00014), pChar->GetName() );
			return FALSE;
		}

		if( pChar->m_CKitbag.IsLock() || !pChar->GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "%s", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00015), pChar->GetName() );
			return FALSE;
		}

        if( pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
           // character.SystemNotice( "%s", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00016), pChar->GetName() );
			return FALSE;
        }
		
		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
			pChar = pChar->GetPlyMainCha();
		}
		else
		{
			if( pChar == pChar->GetPlyMainCha() || pMain == pMain->GetPlyMainCha() )
			{
				/*pMain->SystemNotice( "" );
				pChar->SystemNotice( "" );*/
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		if( pMain->GetPlayer()->IsLuanchOut() || pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00018) );
			return FALSE;
		}
		/*else if( pMain->GetPlayer()->IsLuanchOut() && !pChar->GetPlayer()->IsLuanchOut() )
		{
			pMain->SystemNotice( "" );
			return FALSE;
		}*/
		else if( pMain->GetPlayer()->IsInForge() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00019) );
			return FALSE;
		}

		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 )
		{
			//pMain->SystemNotice( "%s", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00020), pChar->GetName() );
			return FALSE;
		}

		CTradeData* pTradeData2 = pMain->GetTradeData();
		if( pTradeData2 )
		{			
			return FALSE;
		}

		//  :    
		auto packet = net::msg::serialize(net::msg::McCharTradeRequestMessage{
			CMD_MC_CHARTRADE_REQUEST, static_cast<int64_t>(byType), character.GetID()
		});
		pChar->ReflectINFof( pChar, packet );
		return TRUE;
	}

	BOOL CTradeSystem::IsTradeDist( CCharacter& Char1, CCharacter& Char2, DWORD dwDist )
	{
		DWORD dwxDist = (Char1.GetShape().centre.x - Char2.GetShape().centre.x) * 
			(Char1.GetShape().centre.x - Char2.GetShape().centre.x);
		DWORD dwyDist = (Char1.GetShape().centre.y - Char2.GetShape().centre.y) * 
			(Char1.GetShape().centre.y - Char2.GetShape().centre.y);
		return ( dwxDist + dwyDist < dwDist * 100 );
	}

	BOOL CTradeSystem::Accept( BYTE byType, CCharacter& character, DWORD dwRequestID )
	{
		if( character.GetBoat() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00002) );
			return FALSE;
		}

		if( character.GetStallData() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
				if( character.IsReadBook() )
		{
			//character.SystemNotice("");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00004));
			return FALSE;
		}

		if( character.m_CKitbag.IsLock() || !character.GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

		if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsLock() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
           // character.SystemNotice( "" );
			 character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00006) );
			return FALSE;
        }

		if (!character.IsLiveing()){
			character.SystemNotice("Dead pirates are unable to trade.");
			return FALSE;
		}
		CCharacter* pMain = &character;
		if( pMain->GetID() == dwRequestID )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00021) );
			return FALSE;
		}

		CCharacter* pChar = pMain->GetSubMap()->FindCharacter( dwRequestID, pMain->GetShape().centre );
		if( pChar == NULL ) 
		{
			//pMain->SystemNotice( "!" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00022) );
			return FALSE;
		}
		if (!pChar->IsLiveing()){
			pChar->SystemNotice("Dead pirates are unable to trade.");
			return FALSE;
		}

        if(character.GetPlyMainCha()->GetPlayer()->GetBankNpc())
        {
           // character.SystemNotice("");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00023));
           // pChar->SystemNotice( "" );
           pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00008) );
			return FALSE;
        }

		if(character.GetPlyMainCha()->IsStoreEnable() || pChar->GetPlyMainCha()->IsStoreEnable())
		{
			/*character.SystemNotice("!");
			pChar->SystemNotice("!");*/
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			pChar->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			return FALSE;
		}

		if( !pChar->IsLiveing() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00025) );
			return FALSE;
		}

		if( !pMain->IsLiveing() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00026) );
			return FALSE;
		}

		if( pChar->GetBoat() )
		{
			//pChar->SystemNotice( "" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00002) );
			return FALSE;
		}

		if( pChar->GetStallData() )
		{
			//pChar->SystemNotice( "" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
				if( pChar->IsReadBook() )
		{
			//pChar->SystemNotice( "" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00004) );
			return FALSE;
		}

		if( pChar->m_CKitbag.IsLock() || !pChar->GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//pChar->SystemNotice( "" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
            //pChar->SystemNotice( "" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00006) );
			return FALSE;
        }

        if(pChar->GetPlayer()->GetBankNpc())
        {
           // pChar->SystemNotice("");
			 pChar->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00027));
            return FALSE;
        }

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
			pChar = pChar->GetPlyMainCha();
		}
		else
		{
			if( pChar == pChar->GetPlyMainCha() || pMain == pMain->GetPlyMainCha() )
			{
				/*pMain->SystemNotice( "" );
				pChar->SystemNotice( "" );*/
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		if( !pMain->GetPlayer()->IsLuanchOut() && pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00029) );
			return FALSE;
		}
		else if( pMain->GetPlayer()->IsLuanchOut() && !pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00024) );
			return FALSE;
		}
		else if( pMain->GetPlayer()->IsInForge() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00030) );
			return FALSE;
		}

		//if( !IsTradeDist( *pMain, *pChar, ROLE_MAXSIZE_TRADEDIST - 400 ) )
		//{
		//	// 
		//	return FALSE;
		//}

		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 )
		{
			//pMain->SystemNotice( "%s", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00020), pChar->GetName() );
			return FALSE;
		}

		CTradeData* pTradeData2 = pMain->GetTradeData();
		if( pTradeData2 )
		{
			// 
			return FALSE;
		}

		// 
		CTradeData* pData = g_pGameApp->m_TradeDataHeap.Get();
		if( pData == NULL ) 
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00031) );
			return FALSE;
		}
		pData->Clear();
		pData->pRequest = pChar;
		pData->pAccept  = pMain;
		pData->dwTradeTime = GetTickCount();
		pData->bTradeStart = ROLE_TRADE_START;

		//// 
		//pData->sxPos = (USHORT)pMain->GetShape().centre.x;
		//pData->syPos = (USHORT)pMain->GetShape().centre.y;

		// 
		pMain->SetTradeData( pData );
		pChar->SetTradeData( pData );
		
		// 
		pMain->TradeAction( TRUE );
		pChar->TradeAction( TRUE );
		CKitbag& ReqBag = pData->pRequest->m_CKitbag;
		CKitbag& AcpBag = pData->pAccept->m_CKitbag;
		ReqBag.Lock();
		AcpBag.Lock();

		//  :   
		auto packet = net::msg::serialize(net::msg::McCharTradePageMessage{CMD_MC_CHARTRADE_PAGE, (int64_t)byType, (int64_t)pMain->GetID(), (int64_t)pChar->GetID()});
		pChar->ReflectINFof( pMain, packet );
		pMain->ReflectINFof( pMain, packet );
		return TRUE;
	}

	BOOL CTradeSystem::Cancel( BYTE byType, CCharacter& character, DWORD dwCharID )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}
		else
		{
			if( pMain == pMain->GetPlyMainCha() )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CTradeData* pTradeData2 = pMain->GetTradeData();
		if( !pTradeData2 )
		{
			char szData[128];
			sprintf( szData, RES_STRING(GM_CHARTRADE_CPP_00032), pMain->GetName() );
			g_logManager.InternalLog(LogLevel::Error, "trade", szData );
			return FALSE;
		}

		CCharacter* pChar;
		if( pMain->GetID() == dwCharID )
		{
			//  printf  
			g_logManager.InternalLog(LogLevel::Debug, "store", RES_STRING(GM_CHARTRADE_CPP_00033));
			return FALSE;
		}
		else if( pTradeData2->pRequest->GetID() == dwCharID )
		{			
			pChar = pTradeData2->pRequest;
		}
		else if( pTradeData2->pAccept->GetID() == dwCharID )
		{
			pChar = pTradeData2->pAccept;
		}
		else
		{
			//pMain->SystemNotice( "ID = 0x%x", dwCharID );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00034), dwCharID );
			return FALSE;
		}
		
		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 == NULL || pTradeData2 != pTradeData1 )
		{
			//pMain->SystemNotice( ":%s", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00009), pChar->GetName() );
			return FALSE;
		}
		
		// 
		pTradeData1->pAccept->m_CKitbag.UnLock();
		pTradeData1->pRequest->m_CKitbag.UnLock();

		ResetItemState( *pTradeData1->pAccept, *pTradeData1 );
		ResetItemState( *pTradeData1->pRequest, *pTradeData1 );
		
		pTradeData1->pAccept->SetTradeData( NULL );
		pTradeData1->pRequest->SetTradeData( NULL );

		//  :  
		auto packet = net::msg::serialize(net::msg::McCharTradeCancelMessage{
			CMD_MC_CHARTRADE_CANCEL, pMain->GetID()
		});
		pTradeData1->pAccept->ReflectINFof( pMain, packet );
		pTradeData1->pRequest->ReflectINFof( pMain, packet );

		//    
		pTradeData1->pAccept->TradeAction( FALSE );
		pTradeData1->pRequest->TradeAction( FALSE );

		pTradeData1->Free();

		return TRUE;
	}

	BOOL CTradeSystem::Clear( BYTE byType, CCharacter& character )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}
		else
		{
			if( pMain == pMain->GetPlyMainCha() )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			// !
			return FALSE;
		}

		if( pTradeData->pRequest == pMain )
		{
			//  :   ( )
			auto packet = net::msg::serialize(net::msg::McCharTradeCancelMessage{
				CMD_MC_CHARTRADE_CANCEL, pMain->GetID()
			});
			pTradeData->pAccept->ReflectINFof( pMain, packet );
			pTradeData->pAccept->SetTradeData( NULL );

			//   
			pTradeData->pAccept->m_CKitbag.UnLock();
			pTradeData->pAccept->TradeAction( FALSE );
			ResetItemState( *pTradeData->pAccept, *pTradeData );
		}
		else if( pTradeData->pAccept == pMain )
		{
			//  :   ( )
			auto packet = net::msg::serialize(net::msg::McCharTradeCancelMessage{
				CMD_MC_CHARTRADE_CANCEL, pMain->GetID()
			});
			pTradeData->pRequest->ReflectINFof( pMain, packet );
			pTradeData->pRequest->SetTradeData( NULL );
			
			// 
			pTradeData->pRequest->m_CKitbag.UnLock();
			pTradeData->pRequest->TradeAction( FALSE );
			ResetItemState( *pTradeData->pRequest, *pTradeData );
		}
		else
		{
			//LG( "Trade", "()"  );
			ToLogService("common", "when delete characterit find error while clear trade information,the error is:(unsuited charcter pointer)"  );
			return FALSE;
		}

		pTradeData->Free();
		return TRUE;
	}

	BOOL CTradeSystem::AddIMP(BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, DWORD dwMoney)
	{
			CCharacter* pMain = &character;
		if (!pMain->GetPlyMainCha()){
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00010));
		}

		if (byType == mission::TRADE_CHAR)
		{
			pMain = pMain->GetPlyMainCha();
		}
		else{
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00028), byType);
			return FALSE;
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if (!pTradeData)
		{
			char szData[128];
			sprintf(szData, RES_STRING(GM_CHARTRADE_CPP_00035), pMain->GetName());
			g_logManager.InternalLog(LogLevel::Error, "trade", szData);
			return FALSE;
		}

		if (pMain->GetID() == dwCharID)
		{
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00033));
			return FALSE;
		}
		else if (pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID)
		{
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00036));
			return FALSE;
		}

		TRADE_DATA* pItemData = NULL;
		if (pMain == pTradeData->pRequest){
			if (pTradeData->bReqTrade == 1){
				pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00037));
				return FALSE;
			}
			pItemData = &pTradeData->ReqTradeData;
		}
		else if (pMain == pTradeData->pAccept){
			if (pTradeData->bAcpTrade == 1){
				pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00037));
				return FALSE;
			}
			pItemData = &pTradeData->AcpTradeData;
		}
		else{
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00038));
			return FALSE;
		}

		if (byOpType == TRADE_DRAGMONEY_ITEM){
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00039));
			return FALSE;
		}
		else if (byOpType == TRADE_DRAGMONEY_TRADE){
			DWORD dwCharIMP = pMain->GetIMP();
			pItemData->dwIMP = dwMoney;
			if (pItemData->dwIMP > 2000000){
				pItemData->dwIMP = 2000000;
			}
			if (pItemData->dwIMP > dwCharIMP){
				pItemData->dwIMP = dwCharIMP;
			}
		}
		else{
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00039));
			return FALSE;
		}

		//  :  IMP  
		auto packet = net::msg::serialize(net::msg::McCharTradeMoneyMessage{
			CMD_MC_CHARTRADE_MONEY, pMain->GetID(),
			static_cast<int64_t>(pItemData->dwIMP), 1
		});
		pTradeData->pAccept->ReflectINFof(pMain, packet);
		pTradeData->pRequest->ReflectINFof(pMain, packet);
		return TRUE;
	}

	BOOL CTradeSystem::AddMoney( BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, DWORD dwMoney )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() ){
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}else{
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00028), byType );
			return FALSE;
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			char szData[128];
			sprintf( szData, RES_STRING(GM_CHARTRADE_CPP_00035), pMain->GetName() );
			g_logManager.InternalLog(LogLevel::Error, "trade", szData );
			return FALSE;
		}

		if( pMain->GetID() == dwCharID )
		{
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00033) );
			return FALSE;
		}		
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00036) );
			return FALSE;
		}

		TRADE_DATA* pItemData = NULL;
		if( pMain == pTradeData->pRequest ){
			if( pTradeData->bReqTrade == 1 ){
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00037) );
				return FALSE;
			}
			pItemData = &pTradeData->ReqTradeData;
		}
		else if( pMain == pTradeData->pAccept ){
			if( pTradeData->bAcpTrade == 1 ){
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00037) );
				return FALSE;
			}
			pItemData = &pTradeData->AcpTradeData;
		}else{
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00038) );
			return FALSE;
		}

		if( byOpType == TRADE_DRAGMONEY_ITEM ){
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00039) );
			return FALSE;
		}
		else if( byOpType == TRADE_DRAGMONEY_TRADE ){
			DWORD dwCharMoney = (long)pMain->m_CChaAttr.GetAttr( ATTR_GD );
			pItemData->dwMoney = dwMoney;
			if( pItemData->dwMoney > dwCharMoney )
			{
				pItemData->dwMoney = dwCharMoney;
			}
		}else{
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00039) );
			return FALSE;
		}

		//  :    
		auto packet = net::msg::serialize(net::msg::McCharTradeMoneyMessage{
			CMD_MC_CHARTRADE_MONEY, pMain->GetID(),
			static_cast<int64_t>(pItemData->dwMoney), 0
		});
		pTradeData->pAccept->ReflectINFof( pMain, packet );
		pTradeData->pRequest->ReflectINFof( pMain, packet );
		return TRUE;
	}

	//     
	BOOL CTradeSystem::AddItem( BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount )
	{
		CCharacter* pMain = &character;
		if( pMain->GetPlayer() == NULL )
		{		
			return FALSE;
		}

		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}
		else
		{
			if( pMain == pMain->GetPlyMainCha() )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CKitbag& Bag = pMain->m_CKitbag;
		SItemGrid* pGridCont = Bag.GetGridContByID( byItemIndex );

		
		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			char szData[128];
			sprintf( szData, RES_STRING(GM_CHARTRADE_CPP_00040), pMain->GetName() );
			g_logManager.InternalLog(LogLevel::Error, "trade", szData );
			return FALSE;
		}

		if( pMain->GetID() == dwCharID )
		{
			//pMain->SystemNotice( "ID" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00041) );
			return FALSE;
		}		
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00036) );
			return FALSE;
		}

		CCharacter* pChar = NULL;
		TRADE_DATA* pItemData = NULL;
		// 
		if( pMain == pTradeData->pRequest )
		{
			// 
			if( pTradeData->bReqTrade == 1 )
			{
				
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00042) );
				return FALSE;
			}
			pItemData = &pTradeData->ReqTradeData;
			pChar = pTradeData->pAccept;
		}
		else if( pMain == pTradeData->pAccept )
		{
			if( pTradeData->bAcpTrade == 1 )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00042) );
				return FALSE;
			}
			pItemData = &pTradeData->AcpTradeData;
			pChar = pTradeData->pRequest;
		}
		else
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00038) );
			return FALSE;
		}
		
		// 
		if( byOpType == TRADE_DRAGTO_ITEM )
		{
			if( byIndex >= ROLE_MAXNUM_TRADEDATA )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00043));
				return FALSE;
			}
			int nCapacity = pMain->m_CKitbag.GetCapacity();
			if( byItemIndex >= nCapacity )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00044) );
				return FALSE;
			}
			if( pItemData->ItemArray[byIndex].sItemID == 0 )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00045) );
				return FALSE;
			}
			if( Bag.GetNum( pItemData->ItemArray[byIndex].byIndex ) > 0 && 
				Bag.GetID( pItemData->ItemArray[byIndex].byIndex ) != pItemData->ItemArray[byIndex].sItemID )
			{
				//pMain->SystemNotice( "ID1= %d, ID2 = %d", 
				//	Bag.GetID( pItemData->ItemArray[byIndex].byIndex ), pItemData->ItemArray[byIndex].sItemID );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00046), 
					Bag.GetID( pItemData->ItemArray[byIndex].byIndex ), pItemData->ItemArray[byIndex].sItemID );
				return FALSE;
			}

			auto packet = net::msg::serialize(net::msg::McCharTradeItemMessage{
				pMain->GetID(), TRADE_DRAGTO_ITEM,
				net::msg::McCharTradeItemRemoveData{
					static_cast<int64_t>(pItemData->ItemArray[byIndex].byIndex),
					static_cast<int64_t>(byIndex), static_cast<int64_t>(byCount)
				}
			});



			// 
			Bag.Enable( pItemData->ItemArray[byIndex].byIndex );
			pItemData->ItemArray[byIndex].sItemID = 0;
			pItemData->ItemArray[byIndex].byCount = 0;
			pItemData->ItemArray[byIndex].byType = 0;
			pItemData->ItemArray[byIndex].byIndex = 0;
			pItemData->byItemCount--;

						pTradeData->pRequest->ReflectINFof( pMain, packet );
			pTradeData->pAccept->ReflectINFof( pMain, packet );
		}
		else if( byOpType == TRADE_DRAGTO_TRADE )
		{
			if( byIndex >= ROLE_MAXNUM_TRADEDATA )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00043) );
				return FALSE;
			}
			int nCapacity = pMain->m_CKitbag.GetCapacity();
			if( byItemIndex >= nCapacity )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00044) );
				return FALSE;
			}

			if( !Bag.HasItem( byItemIndex ) || !Bag.IsEnable( byItemIndex ) )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00047) );
				return FALSE;
			}
			if( pItemData->ItemArray[byIndex].sItemID != 0 )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00048) );
				return FALSE;
			}


			CItemRecord* pItem = (CItemRecord*)GetItemRecordInfo( Bag.GetID( byItemIndex ) );
			if( pItem == NULL )
			{
				//pMain->SystemNotice( "IDID = %d", Bag.GetID( byItemIndex ) );
				pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), Bag.GetID( byItemIndex ) );
				return FALSE;
			}

			if( !pItem->chIsTrade || !Bag.GetGridContByID(byItemIndex)->GetInstAttr(ITEMATTR_TRADABLE))
			{
				//pMain->SystemNotice( "%s", pItem->szName );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00049), pItem->szName );
				return FALSE;
			}

			if (pGridCont->dwDBID)
			{
				pMain->SystemNotice("Item is bind, cannot be traded!");
				return	FALSE;
			};

			//if( pItem->sType == enumItemTypeMission )
			//{
			//	pMain->SystemNotice( "%s", pItem->szName );
			//	return FALSE;
			//}
			//else 
			if( pItem->sType == enumItemTypeBoat )
			{
				if( pMain->GetPlayer()->IsLuanchOut() )
				{
					if( Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) == pMain->GetPlayer()->GetLuanchID() )
					{
						//pMain->SystemNotice( "" );
						pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00050) );
						return FALSE;
					}
				}

				if( !pChar->GetPlayer()->IsBoatFull() )
				{
					USHORT sID  = Bag.GetID( byItemIndex );
					USHORT sNum = pChar->GetPlayer()->GetNumBoat();

					for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
					{
						if( sID == pItemData->ItemArray[i].sItemID )
						{
							sNum++;
							if( sNum >= MAX_CHAR_BOAT )
							{
								//pMain->SystemNotice( "" );
								pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00051) );
								return FALSE;
							}
						}
					}
				}
				else
				{
					//pMain->SystemNotice( "" );
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00051) );
					return FALSE;
				}

				CCharacter* pBoat = pMain->GetPlayer()->GetBoat( (DWORD)Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
				if( !pBoat )
				{
					/*pMain->SystemNotice( "ID[0x%X]", 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "ID[0x{:X}]", Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ));*/
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00052), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "The data error of this boatcannot tradeID[0x{:X}]", 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					return FALSE;
				}
				if( !game_db.SaveBoat( *pBoat, enumSAVE_TYPE_OFFLINE ) )
				{
					/*pMain->SystemNotice( "AddItem:%sID[0x%X]", pBoat->GetName(), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "AddItem:{}ID[0x{:X}]", pBoat->GetName(), Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ));*/
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00053), pBoat->GetName(), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "AddItem:it failed to save boat databoat{}ID[0x{:X}]", pBoat->GetName(), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					return FALSE;
				}
			}

			if( byCount == 0 )
			{
				byCount = 1;
			}

			if( byCount > ROLE_MAXNUM_ITEMTRADE )
			{
				byCount = ROLE_MAXNUM_ITEMTRADE;
			}

			if( byCount > Bag.GetNum( byItemIndex ) )
			{
				byCount = (BYTE)Bag.GetNum( byItemIndex );
			}

			pItemData->ItemArray[byIndex].sItemID = Bag.GetID( byItemIndex );
			pItemData->ItemArray[byIndex].byCount = byCount;
			pItemData->ItemArray[byIndex].byIndex = byItemIndex;
			pItemData->byItemCount++;

			// 
			Bag.Disable( byItemIndex );

			net::msg::McCharTradeItemAddData addData;
			addData.itemId = pItemData->ItemArray[byIndex].sItemID;
			addData.bagIndex = pItemData->ItemArray[byIndex].byIndex;
			addData.tradeIndex = byIndex;
			addData.count = byCount;
			addData.itemType = pItem->sType;
			if( pItem->sType == enumItemTypeBoat )
			{
				net::msg::TradeBoatData boat;
				CCharacter* pBoat = pMain->GetPlayer()->GetBoat( (DWORD)Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
				if( pBoat )
				{
					boat.hasBoat = true;
					boat.name = pBoat->GetName();
					boat.ship = (USHORT)pBoat->getAttr( ATTR_BOAT_SHIP );
					boat.lv = (USHORT)pBoat->getAttr( ATTR_LV );
					boat.cexp = (long)pBoat->getAttr( ATTR_CEXP );
					boat.hp = (long)pBoat->getAttr( ATTR_HP );
					boat.mxhp = (long)pBoat->getAttr( ATTR_BMXHP );
					boat.sp = (long)pBoat->getAttr( ATTR_SP );
					boat.mxsp = (long)pBoat->getAttr( ATTR_BMXSP );
					boat.mnatk = (long)pBoat->getAttr( ATTR_BMNATK );
					boat.mxatk = (long)pBoat->getAttr( ATTR_BMXATK );
					boat.def = (long)pBoat->getAttr( ATTR_BDEF );
					boat.mspd = (long)pBoat->getAttr( ATTR_BMSPD );
					boat.aspd = (long)pBoat->getAttr( ATTR_BASPD );
					boat.useGridNum = (BYTE)pBoat->m_CKitbag.GetUseGridNum();
					boat.capacity = (BYTE)pBoat->m_CKitbag.GetCapacity();
					boat.price = (long)pBoat->getAttr( ATTR_BOAT_PRICE );
				}
				addData.equipData = std::move(boat);
			}
			else
			{
				net::msg::TradeItemData item;
				SItemGrid* pGridCont = Bag.GetGridContByID( byItemIndex );
				if( !pGridCont )
				{
					pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00057), byItemIndex );
					return FALSE;
				}
				item.endure0 = pGridCont->sEndure[0];
				item.endure1 = pGridCont->sEndure[1];
				item.energy0 = pGridCont->sEnergy[0];
				item.energy1 = pGridCont->sEnergy[1];
				item.forgeLv = pGridCont->chForgeLv;
				item.valid = pGridCont->IsValid() ? 1 : 0;
				item.tradable = pGridCont->bItemTradable;
				item.expiration = pGridCont->expiration;
				item.forgeParam = pGridCont->GetDBParam(enumITEMDBP_FORGE);
				item.instId = pGridCont->GetDBParam(enumITEMDBP_INST_ID);
				if( pGridCont->IsInstAttrValid() )
				{
					item.hasInstAttr = true;
					for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
					{
						item.instAttr[j][0] = pGridCont->sInstAttr[j][0];
						item.instAttr[j][1] = pGridCont->sInstAttr[j][1];
					}
				}
				addData.equipData = std::move(item);
			}

			net::msg::McCharTradeItemMessage msg;
			msg.mainChaId = pMain->GetID();
			msg.opType = TRADE_DRAGTO_TRADE;
			msg.data = std::move(addData);
			auto packet = net::msg::serialize(msg);
			pTradeData->pRequest->ReflectINFof( pMain, packet );
			pTradeData->pAccept->ReflectINFof( pMain, packet );
		}
		else
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00054) );
			return FALSE;
		}

		return TRUE;
	}

	BOOL CTradeSystem::ValidateItemData( BYTE byType, CCharacter& character, DWORD dwCharID )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}
		else
		{
			if( pMain == pMain->GetPlyMainCha() )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			char szData[128];
			sprintf( szData, RES_STRING(GM_CHARTRADE_CPP_00055), pMain->GetName() );
			g_logManager.InternalLog(LogLevel::Error, "trade", szData );
			return FALSE;
		}


		if (!pTradeData->pRequest->IsLiveing() || !pTradeData->pAccept->IsLiveing()){
			pTradeData->pAccept->SystemNotice("Dead pirates are unable to trade.");
			pTradeData->pRequest->SystemNotice("Dead pirates are unable to trade.");
			return FALSE;
		}

		if( pMain->GetID() == dwCharID )
		{
			//pMain->SystemNotice( "ID" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00033) );
			return FALSE;
		}		
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00036) );
			return FALSE;
		}

		// 
		if( pMain == pTradeData->pRequest )
		{
			pTradeData->bReqTrade = 1;
		}
		else if( pMain == pTradeData->pAccept )
		{
			pTradeData->bAcpTrade = 1;
		}
		else
		{
			/*pMain->SystemNotice( "" );
			ToLogService("common", "");*/
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00056) );
			ToLogService("trade", LogLevel::Error, "information of trade object  inside error" );
			return FALSE;
		}
	
		//  :   
		auto packet = net::msg::serialize(net::msg::McCharTradeValidateDataMessage{
			CMD_MC_CHARTRADE_VALIDATEDATA, pMain->GetID()
		});
		if( pMain == pTradeData->pRequest )
		{
			pTradeData->pAccept->ReflectINFof( pMain, packet );
		}
		else
		{
			pTradeData->pRequest->ReflectINFof( pMain, packet );
		}	
		return TRUE;
	}

	BOOL CTradeSystem::ValidateTrade( BYTE byType, CCharacter& character, DWORD dwCharID )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if( byType == mission::TRADE_CHAR )
		{
			pMain = pMain->GetPlyMainCha();
		}
		else
		{
			if( pMain == pMain->GetPlyMainCha() )
			{
				//pMain->SystemNotice( "" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			char szData[128];
			sprintf( szData, RES_STRING(GM_CHARTRADE_CPP_00057), pMain->GetName() );
			g_logManager.InternalLog(LogLevel::Error, "trade", szData );
			return FALSE;
		}

		if (!pTradeData->pRequest->IsLiveing() || !pTradeData->pAccept->IsLiveing()){
			pTradeData->pAccept->SystemNotice("Dead pirates are unable to trade.");
			pTradeData->pRequest->SystemNotice("Dead pirates are unable to trade.");
			return FALSE;
		}

		if( pMain->GetID() == dwCharID )
		{
			//  printf  
			g_logManager.InternalLog(LogLevel::Debug, "store", RES_STRING(GM_CHARTRADE_CPP_00033));
			return FALSE;
		}
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			//  printf  
			g_logManager.InternalLog(LogLevel::Debug, "store", RES_STRING(GM_CHARTRADE_CPP_00036));
			return FALSE;
		}

		// 
		if( pMain == pTradeData->pRequest )
		{
			if( pTradeData->bReqTrade != 1 || pTradeData->bAcpTrade != 1 )
			{
				return FALSE;				
			}
			pTradeData->bReqOk = 1;
		}
		else if( pMain == pTradeData->pAccept )
		{
			if( pTradeData->bReqTrade != 1 || pTradeData->bAcpTrade != 1 )
			{
				return FALSE;
			}
			pTradeData->bAcpOk = 1;
		}

		if( pTradeData->bAcpTrade == 1 && pTradeData->bReqTrade == 1 && 
			pTradeData->bAcpOk == 1 && pTradeData->bReqOk == 1 )
		{
			CCharacter* pRequest = pTradeData->pRequest;
			CCharacter* pAccept  = pTradeData->pAccept;
			CKitbag& ReqBag = pRequest->m_CKitbag;
			CKitbag& AcpBag = pAccept->m_CKitbag;
			DWORD dwReqMoney = (long)pRequest->getAttr( ATTR_GD );
			DWORD dwAcpMoney = (long)pAccept->getAttr( ATTR_GD );

			int dwReqIMP = pRequest->GetIMP();
			int dwAcpIMP = pAccept->GetIMP();

			if (pTradeData->ReqTradeData.dwIMP > dwReqIMP)
			{
				pAccept->SystemNotice("Character (%s] IMP in trading mode is incorrect, trading cannot be continued!", pRequest->GetName());
				pRequest->SystemNotice("Character (%s] IMP in trading mode is incorrect, trading cannot be continued!", pRequest->GetName());

				return FALSE;
			}

			if (pTradeData->AcpTradeData.dwIMP > dwAcpIMP)
			{
				pAccept->SystemNotice("Character (%s] IMP in trading mode is incorrect, trading cannot be continued!", pAccept->GetName());
				pRequest->SystemNotice("Character (%s] IMP in trading mode is incorrect, trading cannot be continued!", pAccept->GetName());
				return FALSE;
			}

			if (dwAcpIMP + pTradeData->ReqTradeData.dwIMP > 2000000){
				pAccept->SystemNotice("Character (%s] IMP would exceed 2b, trading cannot be continued!", pAccept->GetName());
				pRequest->SystemNotice("Character (%s] IMP would exceed 2b, trading cannot be continued!", pAccept->GetName());
				return FALSE;
			}

			if (dwReqIMP + pTradeData->AcpTradeData.dwIMP > 2000000){
				pAccept->SystemNotice("Character (%s] IMP would exceed 2b, trading cannot be continued!", pRequest->GetName());
				pRequest->SystemNotice("Character (%s] IMP would exceed 2b, trading cannot be continued!", pRequest->GetName());
				return FALSE;
			}


			// 
			if( pTradeData->ReqTradeData.dwMoney > dwReqMoney )
			{
				/*pAccept->SystemNotice( "%s", pRequest->GetName() );
				pRequest->SystemNotice( "%s", pRequest->GetName() );*/
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				return FALSE;
			}

			if( pTradeData->AcpTradeData.dwMoney > dwAcpMoney )
			{
				/*pAccept->SystemNotice( "%s", pAccept->GetName() );
				pRequest->SystemNotice( "%s", pAccept->GetName() );*/
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				return FALSE;
			}

			// 
			ReqBag.UnLock();
			AcpBag.UnLock();
			ResetItemState( *pAccept, *pTradeData );
			ResetItemState( *pRequest, *pTradeData );

			// 
			CKitbag ReqBagData, AcpBagData;
			ReqBagData = ReqBag;
			AcpBagData = AcpBag;	

			// 
			ReqBag.SetChangeFlag(false);
			AcpBag.SetChangeFlag(false);
			pRequest->m_CChaAttr.ResetChangeFlag();
			pRequest->SetBoatAttrChangeFlag(false);
			pAccept->m_CChaAttr.ResetChangeFlag();
			pAccept->SetBoatAttrChangeFlag(false);

			// 
			int nAcpCapacity = pAccept->m_CKitbag.GetCapacity();
			int nReqCapacity = pRequest->m_CKitbag.GetCapacity();
			SItemGrid AcpGrid[ROLE_MAXNUM_TRADEDATA];
			SItemGrid ReqGrid[ROLE_MAXNUM_TRADEDATA];

			// 
			char szTemp[128] = "";
			char szTrade[2046] = "";
			sprintf( szTrade, RES_STRING(GM_CHARTRADE_CPP_00059), pAccept->GetName() );

			//
			BOOL bBagSucc = true;
			if(!pTradeData->pAccept->HasLeaveBagGrid(pTradeData->ReqTradeData.byItemCount))
			{
				/*pTradeData->pRequest->SystemNotice(",!");
				pTradeData->pAccept->SystemNotice(",!");*/
				pTradeData->pRequest->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00060));
				pTradeData->pAccept->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00061));
				bBagSucc = false;
			}
			else if(!pTradeData->pRequest->HasLeaveBagGrid(pTradeData->AcpTradeData.byItemCount))
			{
				/*pTradeData->pAccept->SystemNotice(",!");
				pTradeData->pRequest->SystemNotice(",!");*/
				pTradeData->pAccept->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00060));
				pTradeData->pRequest->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00061));
				bBagSucc = false;	
			}
			if(!bBagSucc)
			{
				pAccept->SetTradeData( NULL );
				pRequest->SetTradeData( NULL );
				pTradeData->Free();

				// 
				pTradeData->pAccept->TradeAction( FALSE );
				pTradeData->pRequest->TradeAction( FALSE );

				//  :   (   )
				auto packet = net::msg::serialize(net::msg::McCharTradeResultMessage{
					CMD_MC_CHARTRADE_RESULT, TRADE_FAILER
				});
				pTradeData->pAccept->ReflectINFof( pMain, packet );
				pTradeData->pRequest->ReflectINFof( pMain, packet );
				return FALSE;
			}

			//   
			for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
			{
				// 
				if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
				{
					CItemRecord* pItem = GetItemRecordInfo( pTradeData->AcpTradeData.ItemArray[i].sItemID );
					if( pItem == NULL )
					{
						/*pMain->SystemNotice( "IDID = %d", pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("common", "IDID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorit cannot find this res informationID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID );
						return FALSE;
					}
					else
					{
						AcpGrid[i].sNum = pTradeData->AcpTradeData.ItemArray[i].byCount;
						if( pAccept->KbPopItem( true, false, AcpGrid  + i, pTradeData->AcpTradeData.ItemArray[i].byIndex ) != enumKBACT_SUCCESS )
						{
							/*pAccept->SystemNotice( "%s%dID = %d", 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( "%s%dID = %d", 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "{}{}ID = {}", pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00062), 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00062), 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "it failed to get trade res d% from trade asker d%ID = {}", 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							return FALSE;
						}

						if( pItem->sType == enumItemTypeBoat )
						{
							CCharacter* pBoat = pAccept->GetPlayer()->GetBoat( (DWORD)AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							if( pBoat )
							{
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00063), AcpGrid[i].sNum, pBoat->GetName(),
									AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}
							else
							{
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00064), AcpGrid[i].sNum, 
									AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}

							if( !pAccept->BoatClear( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "%sID[0x%X]", 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "%sID[0x%X]", 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00065), 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00065), 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete captain confirm boat that {} have DBID[0x{:X}]", 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
						else
						{
							sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00096), AcpGrid[i].sNum, pItem->szName );
							strcat( szTrade, szTemp );
						}
					}
				}
			}

			
			sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00066), pRequest->GetName() );
			strcat( szTrade, szTemp );
			for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
			{
				if( pTradeData->ReqTradeData.ItemArray[i].sItemID != 0 )
				{
					CItemRecord* pItem = GetItemRecordInfo( pTradeData->ReqTradeData.ItemArray[i].sItemID );
					if( pItem == NULL )
					{
						/*pMain->SystemNotice( "IDID = %d", pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("common", "IDID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
						pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorit cannot find this res informationID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID );
						return FALSE;
					}
					else
					{
						ReqGrid[i].sNum = pTradeData->ReqTradeData.ItemArray[i].byCount;
						if( pRequest->KbPopItem( true, false, ReqGrid + i, pTradeData->ReqTradeData.ItemArray[i].byIndex ) != enumKBACT_SUCCESS )
						{
							/*pAccept->SystemNotice( "%s%dID = %d", 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( "%s%dID = %d", 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "{}{}ID = {}", pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00067), 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00067), 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "it failed get res from trade asker {}ID = {}",
								pRequest->GetName(), static_cast<int>(pTradeData->ReqTradeData.ItemArray[i].sItemID) );
							return FALSE;
						}

						if( pItem->sType == enumItemTypeBoat )
						{
							CCharacter* pBoat = pRequest->GetPlayer()->GetBoat( (DWORD)ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							if( pBoat )
							{
								/*sprintf( szTemp, "%d%sID[0x%X]", ReqGrid[i].sNum, pBoat->GetName(),
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );*/
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00063), ReqGrid[i].sNum, pBoat->GetName(),
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}
							else
							{
								/*sprintf( szTemp, "%dID[0x%X]", ReqGrid[i].sNum, 
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );*/
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00063), ReqGrid[i].sNum, 
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}

							if( !pRequest->BoatClear( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} haveDBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
						else
						{
							sprintf( szTemp, "%d%s", ReqGrid[i].sNum, pItem->szName );
							strcat( szTrade, szTemp );
						}
					}
				}
			}
			strcat( szTrade, "}" );

			for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
			{
				if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
				{
					CItemRecord* pItem = GetItemRecordInfo( pTradeData->AcpTradeData.ItemArray[i].sItemID );
					if( pItem == NULL )
					{
						/*pRequest->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "IDID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorit cannot find res informationit cannot give you this resID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						continue;
					}

					// 					
					USHORT sCount = AcpGrid[i].sNum;
					Short sPushPos = defKITBAG_DEFPUSH_POS;
					Short sPushRet = pRequest->KbPushItem( true, false, AcpGrid + i, sPushPos );

					if( sPushRet == enumKBACT_ERROR_FULL ) // 
					{
						// 
						USHORT sNum = sCount - AcpGrid[i].sNum;

						CCharacter	*pCCtrlCha = pRequest->GetPlyCtrlCha(), *pCMainCha = pRequest->GetPlyMainCha();
						Long	lPosX, lPosY;
						pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
						if( pCCtrlCha->GetSubMap()->ItemSpawn( AcpGrid + i, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle() ) == NULL )
						{
							/*pAccept->SystemNotice( "%s%sID[%d], Num[%d]", 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							pRequest->SystemNotice( "%s%sID[%d], Num[%d]", 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],{}{}ID[{}], Num[{}]", sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],when trading,{} bag is full,{}failed to put on floortrade res failedID[{}], Num[{}]", 
								sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
						}
					}
					else if( sPushRet != enumKBACT_SUCCESS )
					{						
						/*pAccept->SystemNotice( "%s%sID[%d], Num[%d]", pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( "%s%sID[%d], Num[%d]", pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],{}{}ID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), AcpGrid[i].sID, ReqGrid[i].sNum);*/
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],it failed to put res in {} bag when trading res {}trade res failedID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
					}
					else
					{
						AcpGrid[i].sNum = 0;
					}

					if( sPushRet != enumKBACT_ERROR_FULL && pItem->sType == enumItemTypeBoat )
					{
						if( !pRequest->BoatAdd( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
						{
							/*pAccept->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failedDBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
						}
					}
				}

				// 
				if( pTradeData->ReqTradeData.ItemArray[i].sItemID != 0 )
				{
					CItemRecord* pItem = GetItemRecordInfo( pTradeData->ReqTradeData.ItemArray[i].sItemID );
					if( pItem == NULL )
					{
						/*pRequest->SystemNotice( "IDID = %d", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "IDID = %d", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "IDID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorcannot find this res informationthis res cannot give youID = {}", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						continue;
					}

					// 
					USHORT sCount = ReqGrid[i].sNum;
					Short sPushPos = defKITBAG_DEFPUSH_POS;
					Short sPushRet = pAccept->KbPushItem( true, false, ReqGrid + i, sPushPos );
					
					if( sPushRet == enumKBACT_ERROR_FULL ) // 
					{
						// 
						USHORT sNum = sCount - ReqGrid[i].sNum;

						CCharacter	*pCCtrlCha = pAccept->GetPlyCtrlCha(), *pCMainCha = pAccept->GetPlyMainCha();
						Long	lPosX, lPosY;
						pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
						if( pCCtrlCha->GetSubMap()->ItemSpawn( ReqGrid + i, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle() ) == NULL )
						{
							/*pAccept->SystemNotice( "%s%sID[%d], Num[%d]", 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							pRequest->SystemNotice( "%s%sID[%d], Num[%d]", 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],{}{}ID[{}], Num[{}]", sPushRet, pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],when trading,{} bag is full,{}failed to put on floortrade res failedID[{}], Num[{}]", 
								sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
						}
					}
					else if( sPushRet != enumKBACT_SUCCESS )
					{						
						/*pAccept->SystemNotice( "%s%sID[%d], Num[%d]", pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( "%s%sID[%d], Num[%d]", pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],{}{}ID[{}], Num[{}]", sPushRet, pItem->szName, pAccept->GetName(), ReqGrid[i].sID, ReqGrid[i].sNum);*/
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],it failed to put res in {} bag when trading res {}trade res failedID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
					}
					else 
					{
						ReqGrid[i].sNum = 0;
					}

					if( sPushRet != enumKBACT_ERROR_FULL && pItem->sType == enumItemTypeBoat )
					{
						if( !pAccept->BoatAdd( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
						{
							/*pAccept->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failedDBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
						}
					}
				}
			}

			// 
			if( pTradeData->ReqTradeData.dwMoney > 0 )
			{				
				pRequest->setAttr( ATTR_GD, pRequest->getAttr( ATTR_GD ) - pTradeData->ReqTradeData.dwMoney );
				pAccept->setAttr( ATTR_GD, pAccept->getAttr( ATTR_GD ) + pTradeData->ReqTradeData.dwMoney );				
			}

			if( pTradeData->AcpTradeData.dwMoney > 0 )
			{
				pAccept->setAttr( ATTR_GD, pAccept->getAttr( ATTR_GD ) - pTradeData->AcpTradeData.dwMoney );
				pRequest->setAttr( ATTR_GD, pRequest->getAttr( ATTR_GD ) + pTradeData->AcpTradeData.dwMoney );				
			}

			//IMP
			if (pTradeData->ReqTradeData.dwIMP > 0)
			{
				pRequest->SetIMP(pRequest->GetIMP() - pTradeData->ReqTradeData.dwIMP);
				pAccept->SetIMP(pAccept->GetIMP() + pTradeData->ReqTradeData.dwIMP);
			}

			if (pTradeData->AcpTradeData.dwIMP > 0)
			{
				pAccept->SetIMP(pAccept->GetIMP() - pTradeData->AcpTradeData.dwIMP);
				pRequest->SetIMP(pRequest->GetIMP() + pTradeData->AcpTradeData.dwIMP);
			}

			sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00073), pTradeData->AcpTradeData.dwMoney, 
				pTradeData->ReqTradeData.dwMoney );
			strcat( szTrade, szTemp );

			pAccept->SetTradeData( NULL );
			pRequest->SetTradeData( NULL );
			pTradeData->Free();	

			// 
			game_db.BeginTran();
			if( !pRequest->SaveAssets() || !pAccept->SaveAssets() )
			{
				game_db.RollBack();

				// 
				ReqBag = ReqBagData;
				AcpBag = AcpBagData;
				pRequest->setAttr( ATTR_GD, dwReqMoney );
				pAccept->setAttr( ATTR_GD, dwAcpMoney );

				// 
				for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
				{
					if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
					{
						CItemRecord* pItem = GetItemRecordInfo( pTradeData->AcpTradeData.ItemArray[i].sItemID );
						if( pItem == NULL )
						{
							/*pRequest->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "IDID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorit cannot find res informationit cannot give you this resID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
							continue;
						}

						// 					
						if( pItem->sType == enumItemTypeBoat )
						{
							if( !pRequest->BoatClear( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								
								/*pAccept->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} haveDBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}

							if( !pAccept->BoatAdd( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failedDBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
					}

					// 
					if( pTradeData->ReqTradeData.ItemArray[i].sItemID != 0 )
					{
						CItemRecord* pItem = GetItemRecordInfo( pTradeData->ReqTradeData.ItemArray[i].sItemID );
						if( pItem == NULL )
						{
							/*pRequest->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "IDID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "IDID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID errorit cannot find res informationit cannot give you this resID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
							continue;
						}

						// 
						if( pItem->sType == enumItemTypeBoat )
						{
							if( !pAccept->BoatClear( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								
								/*pAccept->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "%sID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} haveDBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}

							if( !pRequest->BoatAdd( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "%sID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "{}DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failedDBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
					}
				}

				// 
				/*pRequest->SystemNotice( "" );
				pAccept->SystemNotice( "" );
				ToLogService("trade", LogLevel::Error, "{}ID[0x{:X}]{}ID[0x{:X}]", pRequest->GetName(), pRequest->GetPlayer()->GetDBChaId(), pAccept->GetName(), pAccept->GetPlayer()->GetDBChaId());*/
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00074) );
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00074) );
				ToLogService("trade", LogLevel::Error, "the trade data failed to memory in DBtrade data resume completetraderequest one{}ID[0x{:X}]accept one{}ID[0x{:X}]",
					pRequest->GetName(), pRequest->GetPlayer()->GetDBChaId(), pAccept->GetName(), pAccept->GetPlayer()->GetDBChaId() );

				// 
				pAccept->TradeAction( FALSE );
				pRequest->TradeAction( FALSE );

				//  :   (   )
				auto packet = net::msg::serialize(net::msg::McCharTradeResultMessage{
					CMD_MC_CHARTRADE_RESULT, TRADE_FAILER
				});
				pAccept->ReflectINFof( pMain, packet );
				pRequest->ReflectINFof( pMain, packet );

				return FALSE;
			}
			else
			{
				// 
				game_db.CommitTran();
				if( pRequest->IsBoat() )
				{
					char szBoat1[64] = "";
					char szBoat2[64] = "";
					sprintf( szBoat1, RES_STRING(GM_CHARTRADE_CPP_00075), pAccept->GetPlyMainCha()->GetName(), pAccept->GetName() );
					sprintf( szBoat2, RES_STRING(GM_CHARTRADE_CPP_00075), pRequest->GetPlyMainCha()->GetName(), pRequest->GetName() );
					ToLogService("trade", "[CHA_CHA] {} -> {} : {}", szBoat1, szBoat2, szTrade);
				}
				else
				{
					ToLogService("trade", "[CHA_CHA] {} -> {} : {}", pAccept->GetName(), pRequest->GetName(), szTrade);
				}

				pRequest->LogAssets(enumLASSETS_TRADE);
				pAccept->LogAssets(enumLASSETS_TRADE);
			}

			// 
			for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
			{
				if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
				{
					pAccept->RefreshNeedItem( pTradeData->AcpTradeData.ItemArray[i].sItemID );
					BYTE byNum = pTradeData->AcpTradeData.ItemArray[i].byCount - AcpGrid[i].sNum;
					if( byNum )
					{
						pRequest->AfterPeekItem( pTradeData->AcpTradeData.ItemArray[i].sItemID, byNum );
					}
				}

				if( pTradeData->ReqTradeData.ItemArray[i].sItemID != 0 )
				{
					pRequest->RefreshNeedItem( pTradeData->ReqTradeData.ItemArray[i].sItemID );
					BYTE byNum = pTradeData->ReqTradeData.ItemArray[i].byCount - ReqGrid[i].sNum;
					if( byNum )
					{
						pAccept->AfterPeekItem( pTradeData->ReqTradeData.ItemArray[i].sItemID, byNum );
					}
				}
			}

			// 
			char szNotice[255];

			if( pTradeData->ReqTradeData.dwMoney )
			{
				//pAccept->SystemNotice( "(%s)%d", pRequest->GetName(), pTradeData->ReqTradeData.dwMoney );
				CFormatParameter param(2);
				param.setString(0, pRequest->GetName());
				param.setLong(1, pTradeData->ReqTradeData.dwMoney);

				RES_FORMAT_STRING(GM_CHARTRADE_CPP_00076, param, szNotice);

				pAccept->SystemNotice( szNotice );
			}

			if (pTradeData->AcpTradeData.dwMoney)
			{
				CFormatParameter param(2);
				param.setString(0, pAccept->GetName());
				param.setLong(1, pTradeData->AcpTradeData.dwMoney);

				RES_FORMAT_STRING(GM_CHARTRADE_CPP_00076, param, szNotice);

				pRequest->SystemNotice(szNotice);
			}

			if (pTradeData->AcpTradeData.dwIMP)
			{
				sprintf(szNotice, "You have received [%d] IMPs from (%s).", pTradeData->AcpTradeData.dwIMP, pAccept->GetName());
				pRequest->SystemNotice(szNotice);

			}

			if (pTradeData->ReqTradeData.dwIMP)
			{
				sprintf(szNotice, "You have received [%d] IMPs from (%s).", pTradeData->ReqTradeData.dwIMP, pRequest->GetName());
				pAccept->SystemNotice(szNotice);
			}

			

			// 
			pAccept->SynAttr( enumATTRSYN_TRADE );
			pAccept->SyncBoatAttr(enumATTRSYN_TRADE);
			pRequest->SynAttr( enumATTRSYN_TRADE );	
			pRequest->SyncBoatAttr(enumATTRSYN_TRADE);

			pRequest->SynKitbagNew( enumSYN_KITBAG_TRADE );
			pAccept->SynKitbagNew( enumSYN_KITBAG_TRADE );

			if (pTradeData->AcpTradeData.dwIMP > 0 || pTradeData->ReqTradeData.dwIMP > 0){
				//  :  IMP  
				auto packet = net::msg::serialize(net::msg::McUpdateImpMessage{static_cast<int64_t>(pAccept->GetIMP())});
				pAccept->ReflectINFof(pMain, packet);

				auto packet2 = net::msg::serialize(net::msg::McUpdateImpMessage{static_cast<int64_t>(pRequest->GetIMP())});
				pRequest->ReflectINFof(pMain, packet2);
			}

			// 
			pAccept->TradeAction( FALSE );
			pRequest->TradeAction( FALSE );

			//  :   ()
			auto packet = net::msg::serialize(net::msg::McCharTradeResultMessage{
				CMD_MC_CHARTRADE_RESULT, TRADE_SUCCESS
			});
			pAccept->ReflectINFof( pMain, packet );
			pRequest->ReflectINFof( pMain, packet );
		}
		else
		{
			//  :    ( )
			auto packet = net::msg::serialize(net::msg::McCharTradeValidateDataMessage{
				CMD_MC_CHARTRADE_VALIDATE, pMain->GetID()
			});
			if( pMain == pTradeData->pRequest )
			{
				pTradeData->pAccept->ReflectINFof( pMain, packet );
			}
			else
			{
				pTradeData->pRequest->ReflectINFof( pMain, packet );
			}
		}

		return TRUE;
	}

	void CTradeSystem::ResetItemState( CCharacter& character, CTradeData& TradeData )
	{
		int nCapacity = character.m_CKitbag.GetCapacity();
		CKitbag& Bag = character.m_CKitbag;
		TRADE_DATA* pItemData;
		if( &character == TradeData.pAccept )
		{
			pItemData = &TradeData.AcpTradeData;
		}
		else
		{
			pItemData = &TradeData.ReqTradeData;
		}
		
		for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
		{
			if( pItemData->ItemArray[i].byIndex < nCapacity )
			{
				Bag.Enable( pItemData->ItemArray[i].byIndex );
			}				
		}
	}

	CStoreSystem::CStoreSystem() : m_bValid(false)
	{
		
	}

    CStoreSystem::~CStoreSystem()
	{

	}

	BOOL CStoreSystem::PushOrder(CCharacter *pCha, long long lOrderID, long lComID, long lNum)
	{
		SOrderData OrderInfo;
		OrderInfo.lOrderID = lOrderID;
		OrderInfo.ChaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(OrderInfo.ChaName, pCha->GetName());
		OrderInfo.lComID = lComID;
		OrderInfo.lNum = lNum;
		OrderInfo.lRecDBID = pCha->GetKitbagTmpRecDBID();
		OrderInfo.dwTickCount = GetTickCount();
		
		m_OrderList.push_back(OrderInfo);
		ToLogService("store", "PushOrder:[OrderID:{}][ChaID:{}][ChaName:{}][ComID:{}][Num:{}][RecDBID:{}][TickCount:{}]", OrderInfo.lOrderID, OrderInfo.ChaID, OrderInfo.ChaName, OrderInfo.lComID, OrderInfo.lNum, OrderInfo.lRecDBID, OrderInfo.dwTickCount);
		return true;
	}

	SOrderData CStoreSystem::PopOrder(long long lOrderID)
	{
		SOrderData OrderInfo;
		BOOL bFound = false;

		vector<SOrderData>::iterator vec_it;
		for(vec_it = m_OrderList.begin(); vec_it != m_OrderList.end(); vec_it++)
		{
			if((*vec_it).lOrderID == lOrderID)
			{
				OrderInfo.ChaID = (*vec_it).ChaID;
				strcpy(OrderInfo.ChaName, (*vec_it).ChaName);
				OrderInfo.lComID = (*vec_it).lComID;
				OrderInfo.lNum = (*vec_it).lNum;
				OrderInfo.lOrderID = (*vec_it).lOrderID;
				OrderInfo.lRecDBID = (*vec_it).lRecDBID;
				OrderInfo.dwTickCount = (*vec_it).dwTickCount;
				m_OrderList.erase(vec_it);
				bFound = TRUE;
				ToLogService("store", "PopOrder:[OrderID:{}][ChaID:{}][ChaName:{}][ComID:{}][Num:{}][RecDBID:{}][TickCount:{}]", OrderInfo.lOrderID, OrderInfo.ChaID, OrderInfo.ChaName, OrderInfo.lComID, OrderInfo.lNum, OrderInfo.lRecDBID, OrderInfo.dwTickCount);
				break;
			}
		}
		if(!bFound)
		{
			OrderInfo.ChaID = 0;
			OrderInfo.lComID = 0;
			OrderInfo.lNum = 0;
			OrderInfo.lOrderID = 0;
			OrderInfo.lRecDBID = 0;
		}
		return OrderInfo;
	}

	BOOL CStoreSystem::HasOrder(long long lOrderID)
	{
		vector<SOrderData>::iterator vec_it;
		for(vec_it = m_OrderList.begin(); vec_it != m_OrderList.end(); vec_it++)
		{
			if((*vec_it).lOrderID == lOrderID)
			{
				return true;
			}			
		}
		return false;
	}

	long CStoreSystem::GetClassId(long lComID)
	{
		map<long,long>::iterator it = m_ItemSearchList.find(lComID);
		if(it != m_ItemSearchList.end())
		{
			return m_ItemSearchList[lComID];
		}
		else
			return 0;
	}

	cChar *CStoreSystem::GetClassName(long lClsID)
	{
		vector<ClassInfo>::iterator vec_it;
		for(vec_it = m_ItemClass.begin(); vec_it != m_ItemClass.end(); vec_it++)
		{
			if((*vec_it).clsID == lClsID)
				return (*vec_it).clsName;
		}
		return NULL;
	}

	SItemData *CStoreSystem::GetItemData(long lComID)
	{
		long lClsID = GetClassId(lComID);
		if(lClsID != 0)
		{
			vector<SItemData>::iterator it;
			for(it = m_ItemList[lClsID].begin(); it != m_ItemList[lClsID].end(); it++)
			{
				if((*it).store_head.comID == lComID)
					return &(*it);
			}
		}
		return NULL;
	}

	BOOL CStoreSystem::DelItemData(long lComID)
	{
		long lClsID = 0;

		map<long,long>::iterator cls_it = m_ItemSearchList.find(lComID);
		if(cls_it != m_ItemSearchList.end())
		{
			lClsID = m_ItemSearchList[lComID];
			m_ItemSearchList.erase(cls_it);
		}

		if(lClsID != 0)
		{
			vector<SItemData>::iterator it;
			for(it = m_ItemList[lClsID].begin(); it != m_ItemList[lClsID].end(); it++)
			{
				if((*it).store_head.comID == lComID)
				{
					m_ItemList[lClsID].erase(it);
					break;
				}
			}
		}

		return TRUE;
	}

    BOOL CStoreSystem::Request( CCharacter *pCha, long lComID )
    {
		if(pCha->IsStoreBuy())
		{
			//pCha->SystemNotice("!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00077));

			//  :    
			auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
			pCha->ReflectINFof(pCha, WtPk);

			return false;
		}

		SItemData *pComData = GetItemData(lComID);
		if(!pComData || pComData->store_head.comNumber == 0)
		{
			//pCha->SystemNotice("!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00078));

			//  :    
			auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
			pCha->ReflectINFof(pCha, WtPk);

			return false;
		}

		cChar *szClsName = GetClassName(pComData->store_head.comClass);
		if(szClsName)
		{
			//if(!strcmp(szClsName, ""))
			if(!strcmp(szClsName, RES_STRING(GM_CHARTRADE_CPP_00079)))
			{
				// Modify by lark.li 20080919 begin
				//if(pCha->GetPlayer()->GetVipType() == 0 || )
				if(pCha->GetPlayer()->GetVipType() == 0 || pCha->m_SChaPart.sTypeID == 1 || pCha->m_SChaPart.sTypeID == 2  )
				// End
				{
					//pCha->SystemNotice("!");
					pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00080));

					//  :    
					auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
					pCha->ReflectINFof(pCha, WtPk);

					return false;
				}
			}
		}

		short sGridNum = 0;
		ItemInfo *pItem = pComData->pItemArray;
		for(int i = 0; i < pComData->store_head.itemNum; i++)
		{
			CItemRecord* pItemRec = GetItemRecordInfo( pItem->itemID );
			if( pItemRec == NULL )
			{
				//pCha->SystemNotice( "Request: ID = %d", pItem->itemID );
				pCha->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00081), pItem->itemID );

				//  :    
				auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
				pCha->ReflectINFof(pCha, WtPk);

				return false;
			}
			if(pItemRec->GetIsPile())
			{
				sGridNum += 1;
			}
			else
			{
				sGridNum += pItem->itemNum;
			}

			pItem++;
		}

		if (!pCha->HasLeaveBagTempGrid(sGridNum))
		{
			//pCha->PopupNotice("!");
			pCha->PopupNotice(RES_STRING(GM_CHARTRADE_CPP_00082));
			return false;
		}
		
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

        pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());
		pChaInfo->moBean = pCha->GetPlayer()->GetMoBean();
		pChaInfo->rplMoney = pCha->GetPlayer()->GetRplMoney();
		pChaInfo->vip = pCha->GetPlayer()->GetVipType();

		BuildNetMessage(pNm, INFO_STORE_BUY, 0, lComID, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}] repeat!", pNm->msgHead.msgOrder);

			//  :    
			auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			pCha->SetStoreBuy(true);
			PushOrder(pCha, pNm->msgHead.msgOrder, lComID, 1);
			//LG("Store_record", "[%s][ID:%ld][ID:%ld]!\n", pChaInfo->chaName, pChaInfo->chaID, lComID);
			ToLogService("store", "character [{}][ID:{}] order merchandise [ID:{}]!", pChaInfo->chaName, pChaInfo->chaID, lComID);
		}
		else
		{
			//  :    
			auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00084));

			//LG("Store_msg", "Request: InfoServer!\n");

		ToLogService("store", "Request: InfoServer send failed!");}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

        return true;
    }

	BOOL CStoreSystem::Accept( CCharacter *pCha, long lComID )
	{
		pCha->SetStoreBuy(false);
		SItemData *pComData = GetItemData(lComID);
		if(pComData)
		{
			long lNum = pComData->store_head.itemNum;
			ItemInfo *pItem = pComData->pItemArray;

			while(lNum-- > 0)
			{
				pCha->AddItem2KitbagTemp((short)pItem->itemID, (short)pItem->itemNum, pItem);
				pItem++;
			}

			//LG("Store_record", "[%s][ID:%ld][ID:%ld], !\n", pCha->GetName(), pCha->GetPlayer()->GetDBChaId(), lComID);
			{ char _buf[256]; sprintf(_buf, RES_STRING(GM_CHARTRADE_CPP_00085), pCha->GetName(), pCha->GetPlayer()->GetDBChaId(), lComID); g_logManager.InternalLog(LogLevel::Debug, "store", _buf); }

			if(pComData->store_head.comNumber > 0)
			{
				pComData->store_head.comNumber--;
			}

			//
			if(pComData->store_head.comNumber <= 0 && pComData->store_head.comNumber != -1)
			{
				DelItemData(lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Accept2: [ID:%ld]!\n", lComID);
			ToLogService("store", "Accept2: cannot find merchandise [ID:{}]!", lComID);
			return false;
		}
		return true;
	}

    BOOL CStoreSystem::Accept( long long lOrderID, RoleInfo *ChaInfo )
    {
		extern CGameApp *g_pGameApp;

		BOOL bSucc = false;
		SOrderData OrderInfo = PopOrder(lOrderID);
		if(OrderInfo.lOrderID != 0)
		{
			long lChaID = OrderInfo.ChaID;
			long lComID = OrderInfo.lComID;
			CCharacter *pCha = NULL;
			CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(lChaID);
			if(pPlayer)
			{
				pCha = pPlayer->GetMainCha();
			}

			//LG("Store_record", "[%s][ID:%ld][ID:%ld], !\n", ChaInfo->chaName, lChaID, lComID);
			ToLogService("store", "character[{}][ID:{}] has buy res[ID:{}], has buckle money!", ChaInfo->chaName, lChaID, lComID);
			SItemData *pCData = GetItemData(lComID);
			if(pCData->store_head.comNumber > 0)
			{
				pCData->store_head.comNumber--;
			}

			if(!pCha)
			{
				//LG("Store_msg", "[%s][ID:%ld]!\n", ChaInfo->chaName, lChaID);
				ToLogService("store", "character[{}][ID:{}] has leava!", ChaInfo->chaName, lChaID);
			}

			//
			if(pCha)
			{
				pCha->SetStoreBuy(false);
				SItemData *pComData = GetItemData(lComID);
				if(pComData)
				{
					long lNum = pComData->store_head.itemNum;
					ItemInfo *pItem = pComData->pItemArray;

					while(lNum-- > 0)
					{
						pCha->AddItem2KitbagTemp((short)pItem->itemID, (short)pItem->itemNum, pItem);
						pItem++;

					}
					bSucc = true;

					pCha->GetPlayer()->SetMoBean(ChaInfo->moBean);
					pCha->GetPlayer()->SetRplMoney(ChaInfo->rplMoney);
					pCha->GetPlayer()->SetVipType(ChaInfo->vip);
				}
				else
				{
					//LG("Store_msg", "[ID:%ld]!\n", lComID);
					ToLogService("store", "cannot finde merchandise[ID:{}]!", lComID);
				}
			}
			else
			{
				//LG("Store_msg", "[%s][ID:%ld]!\n", ChaInfo->chaName, lChaID);
				ToLogService("store", "character[{}][ID:{}] don't in this map!", ChaInfo->chaName, lChaID);

				BOOL bOnline;
				if(!game_db.IsChaOnline(lChaID, bOnline))
				{
					//LG("Store_msg", "\n");
					ToLogService("store", "it failed to get character online state");
				}
				else
				{
					if(!bOnline)
					{
						//LG("Store_msg", "\n");
						ToLogService("store", "character didn't online");

						if(!game_db.SaveStoreItemID(lChaID, lComID))
						{
							//LG("Store_msg", "\n");
							ToLogService("store", "it failed to memory merchandise information who did not online character");
						}
					}
					else
					{
						//LG("Store_msg", "[%s][ID:%ld]!\n", ChaInfo->chaName, lChaID);
						ToLogService("store", "character[{}][ID:{}]is in other map!", ChaInfo->chaName, lChaID);

						BEGINGETGATE();
						CPlayer	*pCPlayer;
						CCharacter	*pChaOut = 0;
						GateServer	*pGateServer;
						while (pGateServer = GETNEXTGATE())
						{
							bool bFound = false;

							if (!BEGINGETPLAYER(pGateServer))
								continue;
							while (pCPlayer = (CPlayer *)GETNEXTPLAYER(pGateServer))
							{
								pChaOut = pCPlayer->GetMainCha();
								if(pChaOut)
								{
									//  :    (-)
									auto WtPk = net::msg::serialize(net::msg::MmStoreBuyRelayMessage{(int64_t)pChaOut->GetID(), (int64_t)lChaID, (int64_t)lComID, (int64_t)ChaInfo->rplMoney});
									pChaOut->ReflectINFof(pChaOut, WtPk);//

									bFound = true;
									break;
								}
							}

							if(bFound)
							{
								break;
							}
						}		
					}
				}
			}
			//
			if(bSucc)
			{
				ToLogService("store", "[{}][ID:{}][ID:{}], !", pCha->GetName(), lChaID, lComID);

				//
				//  :    
				auto WtPk = net::msg::serializeMcStoreBuySucc(ChaInfo->rplMoney);
				pCha->ReflectINFof(pCha, WtPk);
			}
			else
			{
			}

			//
			if(pCData->store_head.comNumber <= 0 && pCData->store_head.comNumber != -1)
			{
				DelItemData(lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Accept:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "Accept:not find order form[ID:{}]!", lOrderID);
		}
        return true;
    }

    BOOL CStoreSystem::Cancel( long long lOrderID )
    {
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);
		
		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(OrderInfo.lOrderID != 0)
		{
			//
			if(pCha)
			{
				pCha->SetStoreBuy(false);

				//  :    
				auto WtPk = net::msg::serializeMcStoreBuyFailCmd();
				pCha->ReflectINFof(pCha, WtPk);

				//pCha->SystemNotice("!");
				pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00084));
				//LG("Store_data", "[%s][ComID:%ld]!\n", pCha->GetName(), OrderInfo.lComID);
				ToLogService("store", "[{}]failed to buy prop [ComID:{}]!", pCha->GetName(), OrderInfo.lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Cancel:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "Cancel:not find order form[ID:{}]!", lOrderID);
		}
        return true;
    }

    void CStoreSystem::Run( DWORD dwCurTime, DWORD dwIntervalTime, DWORD dwOrderTime )
    {
		try
		{
			static DWORD dwLastTime = 0;
			if(dwCurTime - dwLastTime < dwIntervalTime)
			{
				return;
			}
			else
			{
				dwLastTime = dwCurTime;
			}

			vector<SOrderData>::iterator vec_it;
			for(vec_it = m_OrderList.begin(); vec_it != m_OrderList.end(); vec_it++)
			{
				if(dwCurTime - (*vec_it).dwTickCount > dwOrderTime)
				{
					DWORD dwChaID = (*vec_it).ChaID;
					ToLogService("store", "timeout:[OrderID:{}][ChaID:{}][ChaName:{}][ComID:{}][Num:{}][RecDBID:{}][TickCount:{}]", (*vec_it).lOrderID, (*vec_it).ChaID, (*vec_it).ChaName, (*vec_it).lComID, (*vec_it).lNum, (*vec_it).lRecDBID, (*vec_it).dwTickCount);
					m_OrderList.erase(vec_it);

					CCharacter *pCha = NULL;
					CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(dwChaID);
					if(pPlayer)
					{
						pCha = pPlayer->GetMainCha();
					}
					if(pCha)
					{
						//pCha->SystemNotice(",!");
						pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00095));
					}

					break;
				}
			}
		}
		catch(...)
		{
		}
    }

	BOOL CStoreSystem::GetItemList()
	{
		//LG("Store_msg", "!\n");
		ToLogService("store", "ask for store list!");
		pNetMessage pNm = new NetMessage();
		BuildNetMessage(pNm, INFO_REQUEST_STORE, 0, 0, 0, NULL, 0);
		g_gmsvr->GetInfoServer()->SendData(pNm);
		FreeNetMessage(pNm);
		return true;
	}

	BOOL CStoreSystem::RequestItemList(CCharacter *pCha, long lClsID, short sPage, short sNum)
	{
		map< long, vector<SItemData> >::iterator map_it = m_ItemList.find(lClsID);
		if(map_it != m_ItemList.end())
		{
			//  :   
			short sItemNum = 0;
			short sPageNum = static_cast<short>((m_ItemList[lClsID].size() % sNum == 0) ? (m_ItemList[lClsID].size() / sNum) : (m_ItemList[lClsID].size() / sNum + 1));
			if(sPage > sPageNum)
			{
				sItemNum = 0;
				//LG("Store_msg", "!\n");
				ToLogService("store", "player open-eared page layout over range!");
			}
			else if(sPage == sPageNum)
			{
				sItemNum = static_cast<short>(m_ItemList[lClsID].size() - (sPage - 1) * sNum);
			}
			else
			{
				sItemNum = sNum;
			}

			net::msg::McStoreListAnswerMessage msg;
			msg.pageTotal = sPageNum;
			msg.pageCurrent = sPage;
			vector<SItemData>::iterator it = m_ItemList[lClsID].begin();
			int i;
			for(i = 0; i < (sPage - 1) * sNum; i++)
			{
				it++;
			}
			for(i = 0; i < sItemNum; i++)
			{
				long l_time = (long)((*it).store_head.comExpire);
				if(l_time <= 0)
				{
					l_time = -1;
				}
				else
				{
					l_time -= (long)time(0);
					l_time /= 3600;
					if(l_time < 1)
					{
						l_time = 1;
					}
				}

				net::msg::StoreProductEntry product;
				product.comId = (*it).store_head.comID;
				product.comName = (*it).store_head.comName;
				product.price = (*it).store_head.comPrice;
				product.remark = (*it).store_head.comRemark;
				product.isHot = (*it).store_head.isHot != 0;
				product.time = static_cast<long>((*it).store_head.comTime);
				product.quantity = static_cast<long>((*it).store_head.comNumber);
				product.expire = l_time;

				int j;
				for(j = 0; j < (*it).store_head.itemNum; j++)
				{
					net::msg::StoreVariantEntry variant;
					variant.itemId = (*it).pItemArray[j].itemID;
					variant.itemNum = (*it).pItemArray[j].itemNum;
					variant.flute = (*it).pItemArray[j].itemFlute;
					int k;
					for(k = 0; k < 5; k++)
					{
						variant.attrs[k].attrId = (*it).pItemArray[j].itemAttrID[k];
						variant.attrs[k].attrVal = (*it).pItemArray[j].itemAttrVal[k];
					}
					product.variants.push_back(variant);
				}
				msg.products.push_back(product);
				it++;
			}
			auto WtPk = net::msg::serialize(msg);
			pCha->ReflectINFof(pCha, WtPk);
		}
		else
		{
			//  :   
			net::msg::McStoreListAnswerMessage emptyMsg;
			emptyMsg.pageTotal = 0;
			emptyMsg.pageCurrent = sPage;
			auto WtPk = net::msg::serialize(emptyMsg);
			pCha->ReflectINFof(pCha, WtPk);
		}
		
		return true;
	}

	BOOL CStoreSystem::RequestVIP(CCharacter *pCha, short sVipID, short sMonth)
	{
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

		pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());
		pChaInfo->moBean = pCha->GetPlayer()->GetMoBean();
		pChaInfo->rplMoney = pCha->GetPlayer()->GetRplMoney();
		pChaInfo->vip = pCha->GetPlayer()->GetVipType();

		DWORD dwVipParam = ((sVipID << 16) & 0xffff0000) | (sMonth & 0x0000ffff);

		BuildNetMessage(pNm, INFO_REGISTER_VIP, 0, dwVipParam, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);

			//  :  VIP
			auto WtPk = net::msg::serializeMcStoreVipFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			//  :  VIP
			auto WtPk = net::msg::serializeMcStoreVipFailCmd();
			pCha->ReflectINFof(pCha, WtPk);

		//	LG("Store_msg", "RequestVIP: InfoServer!\n");
			ToLogService("store", "RequestVIP: InfoServer send failed!");
		}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptVIP(long long lOrderID, RoleInfo *ChaInfo, DWORD dwVipParam)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptVIP:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptVIP: cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			pCha->ResetStoreTime();
			pCha->GetPlayer()->SetMoBean(ChaInfo->moBean);
			pCha->GetPlayer()->SetRplMoney(ChaInfo->rplMoney);
			pCha->GetPlayer()->SetVipType(ChaInfo->vip);

			//  :  VIP
			auto WtPk = net::msg::serialize(net::msg::McStoreVipMessage{1, (int64_t)HIWORD(dwVipParam), (int64_t)LOWORD(dwVipParam), (int64_t)ChaInfo->rplMoney, (int64_t)ChaInfo->moBean});
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->PopupNotice("!");
			pCha->PopupNotice(RES_STRING(GM_CHARTRADE_CPP_00086));
			//LG("Store_data", "[%s]VIP!\n", pCha->GetName());
			ToLogService("store", "[{}] purchase VIP succeed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::CancelVIP(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);
		if(OrderInfo.lOrderID != 0)
		{
			CCharacter *pCha = NULL;
			CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
			if(pPlayer)
			{
				pCha = pPlayer->GetMainCha();
			}

			if(pCha)
			{
				pCha->ResetStoreTime();
				//  :  VIP
				auto WtPk = net::msg::serializeMcStoreVipFailCmd();
				pCha->ReflectINFof(pCha, WtPk);
				//pCha->PopupNotice("!");
				pCha->PopupNotice(RES_STRING(GM_CHARTRADE_CPP_00087));
				//LG("Store_data", "[%s]VIP!\n", pCha->GetName());
				ToLogService("store", "[{}]perchase VIP failed!", pCha->GetName());
			}
		}
		else
		{
			//LG("Store_msg", "CancelVIP:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "CancelVIP:cannot find order form[ID:{}]!", lOrderID);
		}
		return true;
	}

	BOOL CStoreSystem::RequestChange(CCharacter *pCha, long lNum)
	{
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

		pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());
		pChaInfo->moBean = pCha->GetPlayer()->GetMoBean();
		pChaInfo->rplMoney = pCha->GetPlayer()->GetRplMoney();
		pChaInfo->vip = pCha->GetPlayer()->GetVipType();

		BuildNetMessage(pNm, INFO_EXCHANGE_MONEY, 0, lNum, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}] repeat!", pNm->msgHead.msgOrder);

			//  :  
			auto WtPk = net::msg::serializeMcStoreChangeFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			//  :  
			auto WtPk = net::msg::serializeMcStoreChangeFailCmd();
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_msg", "RequestChange: InfoServer!\n");
			ToLogService("store", "RequestChange: InfoServer send failed!");
		}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptChange(long long lOrderID, RoleInfo *ChaInfo)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptChange:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptChange:cannot find order form [ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			pCha->ResetStoreTime();
			pCha->GetPlayer()->SetMoBean(ChaInfo->moBean);
			pCha->GetPlayer()->SetRplMoney(ChaInfo->rplMoney);
			pCha->GetPlayer()->SetVipType(ChaInfo->vip);

			//  :  
			auto WtPk = net::msg::serialize(net::msg::McStoreChangeAnswerMessage{1, (int64_t)ChaInfo->moBean, (int64_t)ChaInfo->rplMoney});
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]!\n", pCha->GetName());
			ToLogService("store", "[{}]change token succeed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::CancelChange(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);
		if(OrderInfo.lOrderID != 0)
		{
			//
			CCharacter *pCha = NULL;
			CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
			if(pPlayer)
			{
				pCha = pPlayer->GetMainCha();
			}

			if(pCha)
			{
				pCha->ResetStoreTime();
				//  :  
				auto WtPk = net::msg::serializeMcStoreChangeFailCmd();
				pCha->ReflectINFof(pCha, WtPk);
				//LG("Store_data", "[%s]!\n", pCha->GetName());
				ToLogService("store", "[{}]change token failed!", pCha->GetName());
			}
		}
		else
		{
			ToLogService("store", "CancelChange:cannot find order form[ID:{}]!", lOrderID);
		}
		return true;
	}

	BOOL CStoreSystem::RequestRoleInfo(CCharacter *pCha)
	{
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

		pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());

		BuildNetMessage(pNm, INFO_REQUEST_ACCOUNT, 0, 0, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			BOOL bValid = IsValid();
			if(bValid)
			{
				InValid();
			}

			pCha->GetPlayer()->SetMoBean(0);
			pCha->GetPlayer()->SetRplMoney(0);
			pCha->GetPlayer()->SetVipType(0);
			g_StoreSystem.Open(pCha, 0);

			if(bValid)
			{
				SetValid();
			}

			//LG("Store_msg", "RequestRoleInfo: InfoServer!\n");
			ToLogService("store", "RequestRoleInfo: InfoServer send failed!");
		}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptRoleInfo(long long lOrderID, RoleInfo *ChaInfo)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptRoleInfo:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptRoleInfo:cannot find order form [ID:{}]!", lOrderID);
			return false;
		}

		long lChaID = ChaInfo->chaID;
		long lMoBean = ChaInfo->moBean;
		long lRplMoney = ChaInfo->rplMoney;
		long lVip = ChaInfo->vip;
		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(lChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			pCha->ResetStoreTime();
			pCha->GetPlayer()->SetMoBean(lMoBean);
			pCha->GetPlayer()->SetRplMoney(lRplMoney);
			pCha->GetPlayer()->SetVipType(lVip);

			g_StoreSystem.Open(pCha, lVip);
			//LG("Store_data", "[%s]!\n", pCha->GetName());
			ToLogService("store", "[{}]get account information succeed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::CancelRoleInfo(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);
		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "CancelRoleInfo:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "CancelRoleInfo:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		long lChaID = OrderInfo.ChaID;
		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(lChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			pCha->GetPlayer()->SetMoBean(0);
			pCha->GetPlayer()->SetRplMoney(0);
			pCha->GetPlayer()->SetVipType(0);
			pCha->ResetStoreTime();
			//g_StoreSystem.Open(pCha, 0);
			/*pCha->SystemNotice(",!");
			ToLogService("common", "[{}]!", pCha->GetName());*/
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00088));
			ToLogService("store", "[{}]get account information failed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::RequestRecord(CCharacter *pCha, long lNum)
	{
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

		pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());
		pChaInfo->moBean = pCha->GetPlayer()->GetMoBean();
		pChaInfo->rplMoney = pCha->GetPlayer()->GetRplMoney();
		pChaInfo->vip = pCha->GetPlayer()->GetVipType();

		BuildNetMessage(pNm, INFO_REQUEST_HISTORY, 0, lNum, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);

			//  :   
			auto WtPk = net::msg::serializeMcStoreQueryFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, lNum);
		}
		else
		{
			//  :   
			auto WtPk = net::msg::serializeMcStoreQueryFailCmd();
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_msg", "RequestRecord: InfoServer!\n");
			ToLogService("store", "RequestRecord: InfoServer send failed!");
		}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptRecord(long long lOrderID, HistoryInfo *pRecord)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);
		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptRecord:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptRecord:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		long lNum = OrderInfo.lNum;

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}
		
		if(pCha)
		{
			//  :   
			net::msg::McStoreHistoryMessage histMsg;
			for(int i = 0; i < lNum; i++)
			{
				net::msg::StoreHistoryEntry entry;
				entry.time = ctime(&pRecord->tradeTime);
				entry.itemId = pRecord->comID;
				entry.name = pRecord->comName;
				entry.money = pRecord->tradeMoney;
				histMsg.records.push_back(entry);
				pRecord++;
			}
			auto WtPk = net::msg::serialize(histMsg);
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]!\n", pCha->GetName());
			ToLogService("store", "[{}]query trade note succeed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::CancelRecord(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "CancelRecord:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "CancelRecord:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}
		
		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			//  :   
			auto WtPk = net::msg::serializeMcStoreQueryFailCmd();
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]!\n", pCha->GetName());
			ToLogService("store", "[{}]query trade note failed!", pCha->GetName());
		}
		return true;
	}

	BOOL CStoreSystem::RequestGMSend(CCharacter *pCha, cChar *szTitle, cChar *szContent)
	{
		pNetMessage pNm = new NetMessage();
		pMailInfo pMi = new MailInfo();

		strcpy(pMi->title, szTitle);
		strcpy(pMi->description, szContent);
		pMi->actID = pCha->GetPlayer()->GetActLoginID();
		pMi->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pMi->actName, pCha->GetPlayer()->GetActName());
		strcpy(pMi->chaName, pCha->GetName());
		pMi->sendTime = time(0);

		BuildNetMessage(pNm, INFO_SND_GM_MAIL, 0, 0, 0, (unsigned char*)pMi, sizeof(MailInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pMi);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00089));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			/*pCha->SystemNotice("GMGM!");
			ToLogService("common", "RequestGMSend: InfoServer!");*/
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00090));
			ToLogService("store", "RequestGMSend: InfoServer send failed!");
		}

		SAFE_DELETE(pMi);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptGMSend(long long lOrderID, long lMailID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptGMSend:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptGMSend:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			/*pCha->SystemNotice("GM, [ID: %ld]!", lMailID);
			ToLogService("common", "[{}]GM!", pCha->GetName());*/
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00091), lMailID);
			ToLogService("store", "[{}]send GM mail succeed !", pCha->GetName());
		}

		return true;
	}

	BOOL CStoreSystem::CancelGMSend(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "CancelGMSend:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "CancelGMSend:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			/*pCha->SystemNotice("GMGM!");
			ToLogService("common", "[{}]GM!", pCha->GetName());*/
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00090));
			ToLogService("store", "[{}]send GM mail failed!", pCha->GetName());
		}

		return true;
	}

	BOOL CStoreSystem::RequestGMRecv(CCharacter *pCha)
	{
		pNetMessage pNm = new NetMessage();
		RoleInfo *pChaInfo = new RoleInfo();

		pChaInfo->actID = pCha->GetPlayer()->GetActLoginID();
		strcpy(pChaInfo->actName, pCha->GetPlayer()->GetActName());
		pChaInfo->chaID = pCha->GetPlayer()->GetDBChaId();
		strcpy(pChaInfo->chaName, pCha->GetName());
		pChaInfo->moBean = pCha->GetPlayer()->GetMoBean();
		pChaInfo->rplMoney = pCha->GetPlayer()->GetRplMoney();
		pChaInfo->vip = pCha->GetPlayer()->GetVipType();

		BuildNetMessage(pNm, INFO_RCV_GM_MAIL, 0, 0, 0, (unsigned char*)pChaInfo, sizeof(RoleInfo));
		if(HasOrder(pNm->msgHead.msgOrder))
		{
			SAFE_DELETE(pChaInfo);
			FreeNetMessage(pNm);
			//LG("Store_msg", "[ID:%I64i]!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice(", !");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00089));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			//pCha->SystemNotice("GM!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00092));
			//LG("Store_msg", "RequestGMRecv: InfoServer!\n");
			ToLogService("store", "RequestGMRecv: InfoServersend failed!");
		}

		SAFE_DELETE(pChaInfo);
		FreeNetMessage(pNm);

		return true;
	}

	BOOL CStoreSystem::AcceptGMRecv(long long lOrderID, MailInfo *pMi)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "AcceptGMRecv:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "AcceptGMRecv:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			//  : GM- ()
			auto WtPk = net::msg::serialize(net::msg::McGmMailMessage{
				pMi->title, pMi->description, static_cast<int64_t>(pMi->sendTime)
			});
			pCha->ReflectINFof(pCha, WtPk);
			/*pCha->SystemNotice("GM: [ID: %ld]!", pMi->id);
			ToLogService("common", "[{}]GM!", pCha->GetName());*/
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00093), pMi->id);
			ToLogService("store", "[{}] receive GM mail succeed!", pCha->GetName());
		}

		return true;
	}

	BOOL CStoreSystem::CancelGMRecv(long long lOrderID)
	{
		extern CGameApp *g_pGameApp;

		SOrderData OrderInfo = PopOrder(lOrderID);

		if(OrderInfo.lOrderID == 0)
		{
			//LG("Store_msg", "CancelGMRecv:[ID:%I64i]!\n", lOrderID);
			ToLogService("store", "CancelGMRecv:cannot find order form[ID:{}]!", lOrderID);
			return false;
		}

		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
		if(pPlayer)
		{
			pCha = pPlayer->GetMainCha();
		}

		if(pCha)
		{
			//  : GM- ( )
			auto WtPk = net::msg::serialize(net::msg::McGmMailMessage{
				"GM do not have mail send to you!", "", 0
			});
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_data", "[%s]GM!\n", pCha->GetName());
		
			ToLogService("store", "[{}]receive GM mail failed!", pCha->GetName());
		}

		return true;
	}

	BOOL CStoreSystem::GetAfficheList()
	{
		//LG("Store_msg", "!\n");
		ToLogService("store", "ask for affiche list!");
		pNetMessage pNm = new NetMessage();
		BuildNetMessage(pNm, INFO_REQUEST_AFFICHE, 0, 0, 0, NULL, 0);
		g_gmsvr->GetInfoServer()->SendData(pNm);
		FreeNetMessage(pNm);
		return true;
	}

	BOOL CStoreSystem::SetItemList(void *pItemList, long lNum)
	{
		try
		{
			//LG("Store_msg", "!\n");
			ToLogService("store", "set store item list!");

			ClearItemList();

			int i;
			StoreInfo *pStore = (StoreInfo *)pItemList;
			ItemInfo *pItem = (ItemInfo *)(pStore + 1);
			for(i = 0; i < lNum; i++)
			{
				long lComID = pStore->comID;
				long lClsID = pStore->comClass;
				long lItemNum = pStore->itemNum;
				time_t lComTime = pStore->comTime;

				//
				SItemData ItemNode;
				memcpy(&ItemNode.store_head, pStore, sizeof(StoreInfo));
				if(lItemNum > 0)
				{
					ItemNode.pItemArray = new ItemInfo[lItemNum];
					memcpy(ItemNode.pItemArray, pItem, lItemNum * sizeof(ItemInfo));
				}
				else
					ItemNode.pItemArray = NULL;

				//
				map< long, vector<SItemData> >::iterator map_it = m_ItemList.find(lClsID);
				if(map_it != m_ItemList.end())
				{
					(*map_it).second.push_back(ItemNode);
				}
				else
				{
					vector<SItemData> vecItem;
					vecItem.push_back(ItemNode);
					pair< long, vector<SItemData> > MapNode(lClsID, vecItem);
					m_ItemList.insert(MapNode);
				}

				pair<long,long> SearchNode(lComID, lClsID);
				m_ItemSearchList.insert(SearchNode);

				pStore = (StoreInfo *)(pItem + lItemNum);
				pItem = (ItemInfo *)(pStore + 1);
			}

			//for test
			//LG("Store_info", ":\n");
			ToLogService("common", "store merchandise:");
			vector<ClassInfo>::iterator cls_it = m_ItemClass.begin();
			{
				while(cls_it != m_ItemClass.end())
				{
					short sClsID = (*cls_it).clsID;
					map< long, vector<SItemData> >::iterator itemList_it = m_ItemList.find(sClsID);
					if(itemList_it != m_ItemList.end())
					{
						vector<SItemData>::iterator item_it = m_ItemList[sClsID].begin();
						while(item_it != m_ItemList[sClsID].end())
						{
							ToLogService("common", "\t[comID:{}]\t[comName:{}]\t[comClass:{}]\t[comPrice:{}]\t[itemNum:{}]", (*item_it).store_head.comID, (*item_it).store_head.comName, (*item_it).store_head.comClass, (*item_it).store_head.comPrice, (*item_it).store_head.itemNum);
							ItemInfo *pItemIt = (*item_it).pItemArray;
							int i;
							for(i = 0; i < (*item_it).store_head.itemNum; i++)
							{
								ToLogService("common", "\t\t[itemID:{}]\t[itemNum:{}]", pItemIt->itemID, pItemIt->itemNum);
								pItemIt++;
							}
							item_it++;
						}
					}
					cls_it++;
				}
			}
			ToLogService("common", "");
		}
		catch (excp& e)
		{
			ToLogService("errors", LogLevel::Error, "CStoreSystem::SetItemList() {}!", e.what());
		}
		catch(...)
		{
			//LG("Store_error", "CStoreSystem::SetItemList() !\n");
			ToLogService("errors", LogLevel::Error, "CStoreSystem::SetItemList() unknown abnormity!");
		}

		return true;
	}

	BOOL CStoreSystem::ClearItemList()
	{
		m_ItemList.clear();
		m_ItemSearchList.clear();
		return true;
	}

	BOOL CStoreSystem::SetItemClass(ClassInfo *pClassList, long lNum)
	{
		//LG("Store_msg", "!\n");
		ToLogService("store", "set store item sort!");
		ClearItemClass();
		while(lNum-- > 0)
		{
			m_ItemClass.push_back(*pClassList);
			pClassList++;
		}

		//for test
		//LG("Store_info", ":\n");
		ToLogService("common", "store sort:");
		vector<ClassInfo>::iterator it = m_ItemClass.begin();
		while(it != m_ItemClass.end())
		{
			ToLogService("common", "\t[clsID:{}]\t[clsName:{}]\t[parentID:{}]", (*it).clsID, (*it).clsName, (*it).parentID);
			it++;
		}
		ToLogService("common", "");

		return true;
	}

	BOOL CStoreSystem::ClearItemClass()
	{
		m_ItemClass.clear();
		return true;
	}

	BOOL CStoreSystem::SetAfficheList(AfficheInfo *pAfficheList, long lNum)
	{
		//LG("Store_msg", "!\n");
		ToLogService("store", "set stroe affiche list!");
		ClearAfficheList();
		while(lNum > 0)
		{
			m_AfficheList.push_back(*pAfficheList);
			lNum--;
			pAfficheList++;
		}

		//for test
		//LG("Store_info", ":\n");
		ToLogService("common", "store affiche:");
		vector<AfficheInfo>::iterator it = m_AfficheList.begin();
		while(it != m_AfficheList.end())
		{
			ToLogService("common", "\t[affiID:{}]\t[affiTitle:{}]\t[comID:{}]", (*it).affiID, (*it).affiTitle, (*it).comID);
			it++;
		}
		ToLogService("common", "");

		return true;
	}

	BOOL CStoreSystem::ClearAfficheList()
	{
		m_AfficheList.clear();
		return true;
	}

	BOOL CStoreSystem::Open(CCharacter *pCha, long vip)
	{
		char bValid;
		long lAfficheNum;
		long lClsNum;
		if(!IsValid())
		{
			bValid = 0;
			lAfficheNum = 0;
			lClsNum = 0;
		}
		else
		{
			bValid = 1;
			lAfficheNum = (long)m_AfficheList.size();
			lClsNum = (long)m_ItemClass.size();
		}

		int i;
		if(bValid == 1)
		{
			pCha->ForgeAction();
			pCha->SetStoreEnable(true);
		}
		//  :    
		net::msg::McStoreOpenAnswerMessage openMsg;
		openMsg.isValid = (bValid == 1);

		if(bValid == 1)
		{
			openMsg.vip = vip;
			openMsg.moBean = pCha->GetPlayer()->GetMoBean();
			openMsg.replMoney = pCha->GetPlayer()->GetRplMoney();

			for(i = 0; i < lAfficheNum; i++)
			{
				net::msg::StoreAfficheEntry affiche;
				affiche.afficheId = m_AfficheList[i].affiID;
				affiche.title = m_AfficheList[i].affiTitle;
				affiche.comId = m_AfficheList[i].comID;
				openMsg.affiches.push_back(affiche);
			}
			for(i = 0; i < lClsNum; i++)
			{
				net::msg::StoreClassEntry cls;
				cls.classId = m_ItemClass[i].clsID;
				cls.className = m_ItemClass[i].clsName;
				cls.parentId = m_ItemClass[i].parentID;
				openMsg.classes.push_back(cls);
			}
		}
		auto WtPk = net::msg::serialize(openMsg);
		pCha->ReflectINFof(pCha, WtPk);

		if(bValid != 1)
		{
			//pCha->SystemNotice(",!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00094));
		}

		return true;
	}

}
