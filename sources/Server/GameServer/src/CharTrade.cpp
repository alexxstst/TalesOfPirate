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

	// ���ײ���
	BOOL CTradeSystem::Request( BYTE byType, CCharacter& character, DWORD dwAcceptID )
	{
		if(character.GetPlyMainCha()->IsStoreEnable())
		{
			//character.SystemNotice("�޷�����!");
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
			//character.SystemNotice( "���ڰ�̯�������Խ���" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
		if(character.IsReadBook())
		{
			//character.SystemNotice("���ڶ��飬�����Խ���");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00004));
			return FALSE;
		}

		if( character.m_CKitbag.IsLock() || !character.GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "�����ѱ������������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

		if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsLock() )
		{
			//character.SystemNotice( "�����ѱ������������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
            //character.SystemNotice( "�����ѱ����������������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00006) );
			return FALSE;
        }

		CCharacter* pMain = &character;
		CCharacter* pChar = pMain->GetSubMap()->FindCharacter( dwAcceptID, pMain->GetShape().centre );
		if( pChar == NULL || !pChar->IsPlayerCha() ) 
		{
			//pMain->SystemNotice( "����������Ѿ��뿪!" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00007) );
			return FALSE;
		}

        if(pChar->GetPlayer()->GetBankNpc())
        {
            //pMain->SystemNotice( "�Է�����ʹ�����У����Ժ����ԣ�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00008)  );
            return FALSE;
        }

		if(pChar->GetPlyMainCha()->IsStoreEnable())
		{
			//character.SystemNotice("�޷�����!");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			return FALSE;
		}

		if( !pMain->GetPlyMainCha() || !pChar->GetPlyMainCha() )
		{
			/*pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
			pChar->SystemNotice( "���׽�ɫ�����ڣ�" );*/
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00010) );
		}

		if(pMain->GetPlyMainCha()->GetLevel() < 6)
		{
			//pMain->SystemNotice("���ĵȼ�����,�޷�����!");
			pMain->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00011));
			return FALSE;
		}

		if( pChar->GetBoat() )
		{
			//character.SystemNotice( "��ɫ%s�����촬�������Խ���", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00012), pChar->GetName() );
			return FALSE;
		}

		if( pChar->GetStallData() )
		{
			//character.SystemNotice( "��ɫ%s���ڰ�̯�������Խ���", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00013), pChar->GetName() );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
		if( pChar->IsReadBook() )
		{
			//character.SystemNotice( "��ɫ%s���ڶ��飬�����Խ���", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00014), pChar->GetName() );
			return FALSE;
		}

		if( pChar->m_CKitbag.IsLock() || !pChar->GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "��ɫ%s�����ѱ������������Խ��ף�", pChar->GetName() );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00015), pChar->GetName() );
			return FALSE;
		}

        if( pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
           // character.SystemNotice( "��ɫ%s�����ѱ����������������Խ��ף�", pChar->GetName() );
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
				/*pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
				pChar->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );*/
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		if( pMain->GetPlayer()->IsLuanchOut() || pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "���Ͻ�ֹ���ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00018) );
			return FALSE;
		}
		/*else if( pMain->GetPlayer()->IsLuanchOut() && !pChar->GetPlayer()->IsLuanchOut() )
		{
			pMain->SystemNotice( "���Ѿ����������ڲ����������������ף�" );
			return FALSE;
		}*/
		else if( pMain->GetPlayer()->IsInForge() )
		{
			//pMain->SystemNotice( "�����ڲ����������������˽��ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00019) );
			return FALSE;
		}

		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 )
		{
			//pMain->SystemNotice( "%s���ڽ����У�", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00020), pChar->GetName() );
			return FALSE;
		}

		CTradeData* pTradeData2 = pMain->GetTradeData();
		if( pTradeData2 )
		{			
			return FALSE;
		}

		// ���ͽ�������
		WPACKET packet = GETWPACKET();
        WRITE_CMD(packet, CMD_MC_CHARTRADE);
        WRITE_SHORT(packet, CMD_MC_CHARTRADE_REQUEST);
		WRITE_CHAR(packet, byType);
		WRITE_LONG(packet, character.GetID());

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
			//character.SystemNotice( "���ڽ��촬ֻ�������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00002) );
			return FALSE;
		}

		if( character.GetStallData() )
		{
			//character.SystemNotice( "���ڰ�̯�������Խ���" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
				if( character.IsReadBook() )
		{
			//character.SystemNotice("���ڶ��飬�����Խ���");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00004));
			return FALSE;
		}

		if( character.m_CKitbag.IsLock() || !character.GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//character.SystemNotice( "�����ѱ������������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

		if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsLock() )
		{
			//character.SystemNotice( "�����ѱ������������Խ��ף�" );
			character.SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( character.GetPlyMainCha() && character.GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
           // character.SystemNotice( "�����ѱ����������������Խ��ף�" );
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
			//pMain->SystemNotice( "������������Լ����ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00021) );
			return FALSE;
		}

		CCharacter* pChar = pMain->GetSubMap()->FindCharacter( dwRequestID, pMain->GetShape().centre );
		if( pChar == NULL ) 
		{
			//pMain->SystemNotice( "����֪ͨ�ý�ɫ�Ѿ��뿪!" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00022) );
			return FALSE;
		}
		if (!pChar->IsLiveing()){
			pChar->SystemNotice("Dead pirates are unable to trade.");
			return FALSE;
		}

        if(character.GetPlyMainCha()->GetPlayer()->GetBankNpc())
        {
           // character.SystemNotice("������ʹ�����У������Խ���");
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00023));
           // pChar->SystemNotice( "�Է�����ʹ�����У����Ժ����ԣ�" );
           pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00008) );
			return FALSE;
        }

		if(character.GetPlyMainCha()->IsStoreEnable() || pChar->GetPlyMainCha()->IsStoreEnable())
		{
			/*character.SystemNotice("�޷�����!");
			pChar->SystemNotice("�޷�����!");*/
			character.SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			pChar->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00001));
			return FALSE;
		}

		if( !pChar->IsLiveing() )
		{
			//pMain->SystemNotice( "�����׷����������ɽ��ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00025) );
			return FALSE;
		}

		if( !pMain->IsLiveing() )
		{
			//pMain->SystemNotice( "�����������ɽ��ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00026) );
			return FALSE;
		}

		if( pChar->GetBoat() )
		{
			//pChar->SystemNotice( "���ڽ��촬ֻ�������Խ��ף�" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00002) );
			return FALSE;
		}

		if( pChar->GetStallData() )
		{
			//pChar->SystemNotice( "���ڰ�̯�������Խ��ף�" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00003) );
			return FALSE;
		}

		//add by ALLEN 2007-10-16
				if( pChar->IsReadBook() )
		{
			//pChar->SystemNotice( "���ڶ��飬�����Խ��ף�" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00004) );
			return FALSE;
		}

		if( pChar->m_CKitbag.IsLock() || !pChar->GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//pChar->SystemNotice( "�����ѱ������������Խ��ף�" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00005) );
			return FALSE;
		}

        if( pChar->GetPlyMainCha()->m_CKitbag.IsPwdLocked() )
        {
            //pChar->SystemNotice( "�����ѱ����������������Խ��ף�" );
			pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00006) );
			return FALSE;
        }

        if(pChar->GetPlayer()->GetBankNpc())
        {
           // pChar->SystemNotice("���д�ʱ�����������ף�");
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
				/*pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
				pChar->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );*/
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				pChar->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		if( !pMain->GetPlayer()->IsLuanchOut() && pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "�Է��Ѿ������������ڲ����Խ��������������ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00029) );
			return FALSE;
		}
		else if( pMain->GetPlayer()->IsLuanchOut() && !pChar->GetPlayer()->IsLuanchOut() )
		{
			//pMain->SystemNotice( "���Ѿ����������ڲ����Խ��������������ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00024) );
			return FALSE;
		}
		else if( pMain->GetPlayer()->IsInForge() )
		{
			//pMain->SystemNotice( "�����ڲ������������������˽��ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00030) );
			return FALSE;
		}

		//if( !IsTradeDist( *pMain, *pChar, ROLE_MAXSIZE_TRADEDIST - 400 ) )
		//{
		//	// ������ɫ���׾��룬���ͽ�ɫ���뿪��Ϣ��
		//	return FALSE;
		//}

		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 )
		{
			//pMain->SystemNotice( "%s���ڽ����У�", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00020), pChar->GetName() );
			return FALSE;
		}

		CTradeData* pTradeData2 = pMain->GetTradeData();
		if( pTradeData2 )
		{
			// �Լ��������������ҽ��н�����
			return FALSE;
		}

		// �������Դ�ɽ����������ͷ�
		CTradeData* pData = g_pGameApp->m_TradeDataHeap.Get();
		if( pData == NULL ) 
		{
			//pMain->SystemNotice( "���佻�����ݻ���ʧ�ܣ�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00031) );
			return FALSE;
		}
		pData->Clear();
		pData->pRequest = pChar;
		pData->pAccept  = pMain;
		pData->dwTradeTime = GetTickCount();
		pData->bTradeStart = ROLE_TRADE_START;

		//// ������֤�ص�
		//pData->sxPos = (USHORT)pMain->GetShape().centre.x;
		//pData->syPos = (USHORT)pMain->GetShape().centre.y;

		// ���ý��׽�����Ϣ����
		pMain->SetTradeData( pData );
		pChar->SetTradeData( pData );
		
		// �������׽�ɫ״̬
		pMain->TradeAction( TRUE );
		pChar->TradeAction( TRUE );
		CKitbag& ReqBag = pData->pRequest->m_CKitbag;
		CKitbag& AcpBag = pData->pAccept->m_CKitbag;
		ReqBag.Lock();
		AcpBag.Lock();

		// ���ͽ�ɫ����ҳ����
		WPACKET packet = GETWPACKET();
        WRITE_CMD(packet, CMD_MC_CHARTRADE);
        WRITE_SHORT(packet, CMD_MC_CHARTRADE_PAGE);
		WRITE_CHAR(packet, byType);
        WRITE_LONG(packet, pMain->GetID());
        WRITE_LONG(packet, pChar->GetID());
		pChar->ReflectINFof( pMain, packet );
		pMain->ReflectINFof( pMain, packet );
		return TRUE;
	}

	BOOL CTradeSystem::Cancel( BYTE byType, CCharacter& character, DWORD dwCharID )
	{
		CCharacter* pMain = &character;
		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
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
				//pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
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
			// Заменено printf → логирование
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
			//pMain->SystemNotice( "�ͻ�������Ľ��׶�����Ϣ����ID = 0x%x", dwCharID );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00034), dwCharID );
			return FALSE;
		}
		
		CTradeData* pTradeData1 = pChar->GetTradeData();
		if( pTradeData1 == NULL || pTradeData2 != pTradeData1 )
		{
			//pMain->SystemNotice( "����:���%sδ������н��ף�", pChar->GetName() );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00009), pChar->GetName() );
			return FALSE;
		}
		
		// ���������λ����״̬
		pTradeData1->pAccept->m_CKitbag.UnLock();
		pTradeData1->pRequest->m_CKitbag.UnLock();

		ResetItemState( *pTradeData1->pAccept, *pTradeData1 );
		ResetItemState( *pTradeData1->pRequest, *pTradeData1 );
		
		pTradeData1->pAccept->SetTradeData( NULL );
		pTradeData1->pRequest->SetTradeData( NULL );

		// ȡ����ɫ����
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_CHARTRADE );
		WRITE_SHORT(packet, CMD_MC_CHARTRADE_CANCEL );
		WRITE_LONG(packet, pMain->GetID() );

		pTradeData1->pAccept->ReflectINFof( pMain, packet );
		pTradeData1->pRequest->ReflectINFof( pMain, packet );

		// ȡ����ɫ����״̬
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
			//pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
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
				//pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00017) );
				return FALSE;
			}
		}

		CTradeData* pTradeData = pMain->GetTradeData();
		if( !pTradeData )
		{
			// �ý�ɫ����������!
			return FALSE;
		}

		if( pTradeData->pRequest == pMain )
		{
			// ȡ����ɫ����
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_CANCEL );
			WRITE_LONG(packet, pMain->GetID() );
			pTradeData->pAccept->ReflectINFof( pMain, packet );
			pTradeData->pAccept->SetTradeData( NULL );

			// ���������λ����״̬
			pTradeData->pAccept->m_CKitbag.UnLock();
			pTradeData->pAccept->TradeAction( FALSE );
			ResetItemState( *pTradeData->pAccept, *pTradeData );
		}
		else if( pTradeData->pAccept == pMain )
		{
			// ȡ����ɫ����
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_CANCEL );
			WRITE_LONG(packet, pMain->GetID() );
			pTradeData->pRequest->ReflectINFof( pMain, packet );
			pTradeData->pRequest->SetTradeData( NULL );
			
			// ���������λ����״̬
			pTradeData->pRequest->m_CKitbag.UnLock();
			pTradeData->pRequest->TradeAction( FALSE );
			ResetItemState( *pTradeData->pRequest, *pTradeData );
		}
		else
		{
			//LG( "Trade", "ɾ����ɫʱ������佻����Ϣ���ִ���(��ƥ��Ľ�ɫָ��)��"  );
			ToLogService("common", "when delete character��it find error while clear trade information,the error is:(unsuited charcter pointer)��"  );
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

		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_CHARTRADE);
		WRITE_SHORT(packet, CMD_MC_CHARTRADE_MONEY);
		WRITE_LONG(packet, pMain->GetID());
		WRITE_LONG(packet, pItemData->dwIMP);
		WRITE_CHAR(packet, 1);
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

		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_CHARTRADE );
		WRITE_SHORT(packet, CMD_MC_CHARTRADE_MONEY );
		WRITE_LONG(packet, pMain->GetID() );
		WRITE_LONG(packet, pItemData->dwMoney );
		WRITE_CHAR(packet, 0);
		pTradeData->pAccept->ReflectINFof( pMain, packet );
		pTradeData->pRequest->ReflectINFof( pMain, packet );
		return TRUE;
	}

	// ���û���ȡ����Ʒ��������
	BOOL CTradeSystem::AddItem( BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount )
	{
		CCharacter* pMain = &character;
		if( pMain->GetPlayer() == NULL )
		{		
			return FALSE;
		}

		if( !pMain->GetPlyMainCha() )
		{
			//pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
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
				//pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
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
			//pMain->SystemNotice( "������Ϣ���󣬲���������Ʒ���Լ�ID��ͬ�Ľ��ײ�����" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00041) );
			return FALSE;
		}		
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			//pMain->SystemNotice( "���׶�����Ϣ����" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00036) );
			return FALSE;
		}

		CCharacter* pChar = NULL;
		TRADE_DATA* pItemData = NULL;
		// ��֤�Ƿ����������Ʒ��ȡ����Ʒ����
		if( pMain == pTradeData->pRequest )
		{
			// �ж��Ƿ���Բ�����Ʒ
			if( pTradeData->bReqTrade == 1 )
			{
				
				//pMain->SystemNotice( "�ý�ɫ������Ʒ��֤��ť�Ѿ����£�������������Ʒ�϶�������" );
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
				//pMain->SystemNotice( "�ý�ɫ������Ʒ��֤��ť�Ѿ����£�������������Ʒ�϶�������" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00042) );
				return FALSE;
			}
			pItemData = &pTradeData->AcpTradeData;
			pChar = pTradeData->pRequest;
		}
		else
		{
			//pMain->SystemNotice( "�ý�ɫδ�ڽ��ף�" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00038) );
			return FALSE;
		}
		
		// ���ý�������Ʒ�����ҷ���֪ͨ��Ϣ���ͻ���
		if( byOpType == TRADE_DRAGTO_ITEM )
		{
			if( byIndex >= ROLE_MAXNUM_TRADEDATA )
			{
				//pMain->SystemNotice( "δ֪�Ľ�����λ������Ϣ��" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00043));
				return FALSE;
			}
			int nCapacity = pMain->m_CKitbag.GetCapacity();
			if( byItemIndex >= nCapacity )
			{
				//pMain->SystemNotice( "δ֪�ĵ�����λ������Ϣ��" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00044) );
				return FALSE;
			}
			if( pItemData->ItemArray[byIndex].sItemID == 0 )
			{
				//pMain->SystemNotice( "�ý�ɫ�϶��Ľ�����λ��Ʒ�����ڣ�" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00045) );
				return FALSE;
			}
			if( Bag.GetNum( pItemData->ItemArray[byIndex].byIndex ) > 0 && 
				Bag.GetID( pItemData->ItemArray[byIndex].byIndex ) != pItemData->ItemArray[byIndex].sItemID )
			{
				//pMain->SystemNotice( "ϵͳ��Ʒ�����׼�¼������֪ͨ������Ա��лл��ID1= %d, ID2 = %d", 
				//	Bag.GetID( pItemData->ItemArray[byIndex].byIndex ), pItemData->ItemArray[byIndex].sItemID );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00046), 
					Bag.GetID( pItemData->ItemArray[byIndex].byIndex ), pItemData->ItemArray[byIndex].sItemID );
				return FALSE;
			}

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_ITEM );
			WRITE_LONG(packet, pMain->GetID() );
			WRITE_CHAR(packet, TRADE_DRAGTO_ITEM );
			WRITE_CHAR(packet, pItemData->ItemArray[byIndex].byIndex );
			WRITE_CHAR(packet, byIndex );
			WRITE_CHAR(packet, byCount );

			// ��������������Ʒ�״̬
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
				//pMain->SystemNotice( "δ֪�Ľ�����λ������Ϣ��" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00043) );
				return FALSE;
			}
			int nCapacity = pMain->m_CKitbag.GetCapacity();
			if( byItemIndex >= nCapacity )
			{
				//pMain->SystemNotice( "δ֪�ĵ�����λ������Ϣ��" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00044) );
				return FALSE;
			}

			if( !Bag.HasItem( byItemIndex ) || !Bag.IsEnable( byItemIndex ) )
			{
				//pMain->SystemNotice( "����Ʒ��λ�ѱ���ֹ�϶���" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00047) );
				return FALSE;
			}
			if( pItemData->ItemArray[byIndex].sItemID != 0 )
			{
				//pMain->SystemNotice( "�ý�����Ʒ��λ�Ѵ�����Ʒ������ѡλ�ðڷţ�" );
				pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00048) );
				return FALSE;
			}


			CItemRecord* pItem = (CItemRecord*)GetItemRecordInfo( Bag.GetID( byItemIndex ) );
			if( pItem == NULL )
			{
				//pMain->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ��ID = %d", Bag.GetID( byItemIndex ) );
				pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), Bag.GetID( byItemIndex ) );
				return FALSE;
			}

			if( !pItem->chIsTrade || !Bag.GetGridContByID(byItemIndex)->GetInstAttr(ITEMATTR_TRADABLE))
			{
				//pMain->SystemNotice( "��Ʒ��%s�����ɽ��ף�", pItem->szName );
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
			//	pMain->SystemNotice( "������ߡ�%s�������Խ��ף�", pItem->szName );
			//	return FALSE;
			//}
			//else 
			if( pItem->sType == enumItemTypeBoat )
			{
				if( pMain->GetPlayer()->IsLuanchOut() )
				{
					if( Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) == pMain->GetPlayer()->GetLuanchID() )
					{
						//pMain->SystemNotice( "������ʹ�øô�ֻ�������Խ��׸ô�ֻ����֤����" );
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
								//pMain->SystemNotice( "�Է��Ѿ�ӵ�����㹻�����Ĵ�ֻ����������ӵ���´�ֻ��" );
								pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00051) );
								return FALSE;
							}
						}
					}
				}
				else
				{
					//pMain->SystemNotice( "�Է��Ѿ�ӵ�����㹻�����Ĵ�ֻ����������ӵ���´�ֻ��" );
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00051) );
					return FALSE;
				}

				CCharacter* pBoat = pMain->GetPlayer()->GetBoat( (DWORD)Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
				if( !pBoat )
				{
					/*pMain->SystemNotice( "�ô����ݴ��󣬲��ɽ��ף�ID[0x%X]", 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "�ô����ݴ��󣬲��ɽ��ף�ID[0x{:X}]", Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ));*/
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00052), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "The data error of this boat��cannot trade��ID[0x{:X}]", 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					return FALSE;
				}
				if( !game_db.SaveBoat( *pBoat, enumSAVE_TYPE_OFFLINE ) )
				{
					/*pMain->SystemNotice( "AddItem:���洬ֻ����ʧ�ܣ���ֻ��%s��ID[0x%X]��", pBoat->GetName(), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "AddItem:���洬ֻ����ʧ�ܣ���ֻ��{}��ID[0x{:X}]��", pBoat->GetName(), Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ));*/
					pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00053), pBoat->GetName(), 
						Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
					ToLogService("trade", LogLevel::Error, "AddItem:it failed to save boat data��boat��{}��ID[0x{:X}]��", pBoat->GetName(), 
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

			// ��ֹ��Ʒ��λ�״̬
			Bag.Disable( byItemIndex );

			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_ITEM );
			WRITE_LONG(packet, pMain->GetID() );
			WRITE_CHAR(packet, TRADE_DRAGTO_TRADE );
			WRITE_SHORT(packet, pItemData->ItemArray[byIndex].sItemID );
			WRITE_CHAR(packet, pItemData->ItemArray[byIndex].byIndex );
			WRITE_CHAR(packet, byIndex );
			WRITE_CHAR(packet, byCount );
			WRITE_SHORT(packet, pItem->sType );

			if( pItem->sType == enumItemTypeBoat )
			{
				CCharacter* pBoat = pMain->GetPlayer()->GetBoat( (DWORD)Bag.GetDBParam( enumITEMDBP_INST_ID, byItemIndex ) );
				if( pBoat )
				{
					WRITE_CHAR( packet, 1 );
					WRITE_STRING( packet, pBoat->GetName() );
					WRITE_SHORT( packet, (USHORT)pBoat->getAttr( ATTR_BOAT_SHIP ) );
					WRITE_SHORT( packet, (USHORT)pBoat->getAttr( ATTR_LV ) );

					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_CEXP ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_HP ) );

					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BMXHP ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_SP ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BMXSP ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BMNATK ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BMXATK ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BDEF ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BMSPD ) );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BASPD ) );
					WRITE_CHAR( packet, (BYTE)pBoat->m_CKitbag.GetUseGridNum() );
					WRITE_CHAR( packet, (BYTE)pBoat->m_CKitbag.GetCapacity() );
					WRITE_LONG( packet, (long)pBoat->getAttr( ATTR_BOAT_PRICE ) );
				}
				else
				{
					WRITE_CHAR( packet, 0 );
				}
			}
			else
			{
				// �õ��ߵ�ʵ������
				SItemGrid* pGridCont = Bag.GetGridContByID( byItemIndex );
				if( !pGridCont )
				{
					//pMain->SystemNotice( "ָ������Ʒ��λ��Ʒʵ����ϢΪ�գ�ID[%d]", byItemIndex );
					pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00057), byItemIndex );
					return FALSE;
				}

				WRITE_SHORT( packet, pGridCont->sEndure[0] );
				WRITE_SHORT( packet, pGridCont->sEndure[1] );
				WRITE_SHORT( packet, pGridCont->sEnergy[0] );
				WRITE_SHORT( packet, pGridCont->sEnergy[1] );
				WRITE_CHAR( packet, pGridCont->chForgeLv );
				WRITE_CHAR( packet, pGridCont->IsValid() ? 1 : 0 );
				WRITE_CHAR(packet, pGridCont->bItemTradable);
				WRITE_LONG(packet, pGridCont->expiration);

				WRITE_LONG(packet, pGridCont->GetDBParam(enumITEMDBP_FORGE));
				WRITE_LONG(packet, pGridCont->GetDBParam(enumITEMDBP_INST_ID));
				if( pGridCont->IsInstAttrValid() ) // ����ʵ������
				{
					WRITE_CHAR( packet, 1 );
					for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
					{
						WRITE_SHORT(packet, pGridCont->sInstAttr[j][0]);
						WRITE_SHORT(packet, pGridCont->sInstAttr[j][1]);
					}
				}
				else
				{
					WRITE_CHAR( packet, 0 ); // ������ʵ������
				}
			}

			pTradeData->pRequest->ReflectINFof( pMain, packet );
			pTradeData->pAccept->ReflectINFof( pMain, packet );
		}
		else
		{
			//pMain->SystemNotice( "δ֪����Ʒ�϶�����ָ�" );
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
			//pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
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
				//pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
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
			//pMain->SystemNotice( "������Ϣ���󣬲���ȡ�����Լ�ID��ͬ�Ľ��ײ�����" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00033) );
			return FALSE;
		}		
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			//pMain->SystemNotice( "���׶�����Ϣ����" );
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00036) );
			return FALSE;
		}

		// ����ȷ����Ʒ��Ϣ״̬
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
			/*pMain->SystemNotice( "���׶�����Ϣ�ڲ�����" );
			ToLogService("common", "���׶�����Ϣ�ڲ�����");*/
			pMain->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00056) );
			ToLogService("trade", LogLevel::Error, "information of trade object  inside error" );
			return FALSE;
		}
	
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_CHARTRADE );
		WRITE_SHORT(packet, CMD_MC_CHARTRADE_VALIDATEDATA );
		WRITE_LONG(packet, pMain->GetID() );

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
			//pMain->SystemNotice( "���׽�ɫ�����ڣ�" );
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
				//pMain->SystemNotice( "���׽�ɫ���Ͳ�ƥ�䣡" );
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
			// Заменено printf → логирование
			g_logManager.InternalLog(LogLevel::Debug, "store", RES_STRING(GM_CHARTRADE_CPP_00033));
			return FALSE;
		}
		else if( pTradeData->pRequest->GetID() != dwCharID && pTradeData->pAccept->GetID() != dwCharID )
		{
			// Заменено printf → логирование
			g_logManager.InternalLog(LogLevel::Debug, "store", RES_STRING(GM_CHARTRADE_CPP_00036));
			return FALSE;
		}

		// ���ý���״̬��������Ƿ�˫����������
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


			// �ٴ�У�齻�׽�Ǯ������Ϣ
			if( pTradeData->ReqTradeData.dwMoney > dwReqMoney )
			{
				/*pAccept->SystemNotice( "��ɫ��%s�����׽�Ǯ���ݲ���ȷ�������Լ������ף�", pRequest->GetName() );
				pRequest->SystemNotice( "��ɫ��%s�����׽�Ǯ���ݲ���ȷ�������Լ������ף�", pRequest->GetName() );*/
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				return FALSE;
			}

			if( pTradeData->AcpTradeData.dwMoney > dwAcpMoney )
			{
				/*pAccept->SystemNotice( "��ɫ��%s�����׽�Ǯ���ݲ���ȷ�������Լ������ף�", pAccept->GetName() );
				pRequest->SystemNotice( "��ɫ��%s�����׽�Ǯ���ݲ���ȷ�������Լ������ף�", pAccept->GetName() );*/
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00058), pRequest->GetName() );
				return FALSE;
			}

			// ���������λ����״̬
			ReqBag.UnLock();
			AcpBag.UnLock();
			ResetItemState( *pAccept, *pTradeData );
			ResetItemState( *pRequest, *pTradeData );

			// ���ݽ���˫�������ͽ�Ǯ������Ϣ
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

			// ��ɽ�����Ϣ����
			int nAcpCapacity = pAccept->m_CKitbag.GetCapacity();
			int nReqCapacity = pRequest->m_CKitbag.GetCapacity();
			SItemGrid AcpGrid[ROLE_MAXNUM_TRADEDATA];
			SItemGrid ReqGrid[ROLE_MAXNUM_TRADEDATA];

			// ������߽����Ƿ���Խ���
			char szTemp[128] = "";
			char szTrade[2046] = "";
			sprintf( szTrade, RES_STRING(GM_CHARTRADE_CPP_00059), pAccept->GetName() );

			//�ж�˫������
			BOOL bBagSucc = true;
			if(!pTradeData->pAccept->HasLeaveBagGrid(pTradeData->ReqTradeData.byItemCount))
			{
				/*pTradeData->pRequest->SystemNotice("�Է������ռ䲻��,����ʧ��!");
				pTradeData->pAccept->SystemNotice("�����ռ䲻��,����ʧ��!");*/
				pTradeData->pRequest->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00060));
				pTradeData->pAccept->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00061));
				bBagSucc = false;
			}
			else if(!pTradeData->pRequest->HasLeaveBagGrid(pTradeData->AcpTradeData.byItemCount))
			{
				/*pTradeData->pAccept->SystemNotice("�Է������ռ䲻��,����ʧ��!");
				pTradeData->pRequest->SystemNotice("�����ռ䲻��,����ʧ��!");*/
				pTradeData->pAccept->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00060));
				pTradeData->pRequest->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00061));
				bBagSucc = false;	
			}
			if(!bBagSucc)
			{
				pAccept->SetTradeData( NULL );
				pRequest->SetTradeData( NULL );
				pTradeData->Free();

				// ȡ����ɫ����״̬
				pTradeData->pAccept->TradeAction( FALSE );
				pTradeData->pRequest->TradeAction( FALSE );

				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_CHARTRADE );
				WRITE_SHORT(packet, CMD_MC_CHARTRADE_RESULT );
				WRITE_CHAR(packet, TRADE_FAILER );

				pTradeData->pAccept->ReflectINFof( pMain, packet );
				pTradeData->pRequest->ReflectINFof( pMain, packet );
				return FALSE;
			}

			// ���߽��ײ���
			for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
			{
				// 
				if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
				{
					CItemRecord* pItem = GetItemRecordInfo( pTradeData->AcpTradeData.ItemArray[i].sItemID );
					if( pItem == NULL )
					{
						/*pMain->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ��ID = %d", pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("common", "��ƷID�����޷��ҵ�����Ʒ��Ϣ��ID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��it cannot find this res information��ID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID );
						return FALSE;
					}
					else
					{
						AcpGrid[i].sNum = pTradeData->AcpTradeData.ItemArray[i].byCount;
						if( pAccept->KbPopItem( true, false, AcpGrid  + i, pTradeData->AcpTradeData.ItemArray[i].byIndex ) != enumKBACT_SUCCESS )
						{
							/*pAccept->SystemNotice( "�ӽ��׽����ߡ�%s��������ȡ��Ʒ��%d����Ʒʧ�ܣ�ID = %d", 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( "�ӽ��׽����ߡ�%s��������ȡ��Ʒ��%d����Ʒʧ�ܣ�ID = %d", 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "�ӽ��������ߡ�{}��������ȡ��Ʒ��{}����Ʒʧ�ܣ�ID = {}", pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00062), 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00062), 
								pAccept->GetName(), pTradeData->AcpTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "it failed to get trade res ��d%�� from trade asker ��d%����ID = {}", 
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
								/*pAccept->SystemNotice( "ɾ��%sӵ�еĴ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "ɾ��%sӵ�еĴ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "ɾ��{}�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00065), 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00065), 
									pAccept->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete captain confirm boat that {} have ��DBID[0x{:X}]", 
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
						/*pMain->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ��ID = %d", pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("common", "��ƷID�����޷��ҵ�����Ʒ��Ϣ��ID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
						pMain->SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00041), pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��it cannot find this res information��ID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID );
						return FALSE;
					}
					else
					{
						ReqGrid[i].sNum = pTradeData->ReqTradeData.ItemArray[i].byCount;
						if( pRequest->KbPopItem( true, false, ReqGrid + i, pTradeData->ReqTradeData.ItemArray[i].byIndex ) != enumKBACT_SUCCESS )
						{
							/*pAccept->SystemNotice( "�ӽ��������ߡ�%s��������ȡ��Ʒ��%d����Ʒʧ�ܣ�ID = %d", 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( "�ӽ��������ߡ�%s��������ȡ��Ʒ��%d����Ʒʧ�ܣ�ID = %d", 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "�ӽ��������ߡ�{}��������ȡ��Ʒ��{}����Ʒʧ�ܣ�ID = {}", pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00067), 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00067), 
								pRequest->GetName(), pTradeData->ReqTradeData.ItemArray[i].sItemID );
							ToLogService("trade", LogLevel::Error, "it failed get res from trade asker ��{}����ID = {}",
								pRequest->GetName(), static_cast<int>(pTradeData->ReqTradeData.ItemArray[i].sItemID) );
							return FALSE;
						}

						if( pItem->sType == enumItemTypeBoat )
						{
							CCharacter* pBoat = pRequest->GetPlayer()->GetBoat( (DWORD)ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							if( pBoat )
							{
								/*sprintf( szTemp, "%d�Ҵ�ֻ��%s��ID[0x%X]��", ReqGrid[i].sNum, pBoat->GetName(),
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );*/
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00063), ReqGrid[i].sNum, pBoat->GetName(),
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}
							else
							{
								/*sprintf( szTemp, "%d�Ҵ�ֻ��δ֪��ֻ����ID[0x%X]��", ReqGrid[i].sNum, 
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );*/
								sprintf( szTemp, RES_STRING(GM_CHARTRADE_CPP_00063), ReqGrid[i].sNum, 
									ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								strcat( szTrade, szTemp );
							}

							if( !pRequest->BoatClear( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "ɾ��{}�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} have��DBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
						else
						{
							sprintf( szTemp, "%d��%s��", ReqGrid[i].sNum, pItem->szName );
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
						/*pRequest->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��it cannot find res information��it cannot give you this res��ID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						continue;
					}

					// �������߸��Ķ�������������					
					USHORT sCount = AcpGrid[i].sNum;
					Short sPushPos = defKITBAG_DEFPUSH_POS;
					Short sPushRet = pRequest->KbPushItem( true, false, AcpGrid + i, sPushPos );

					if( sPushRet == enumKBACT_ERROR_FULL ) // ��������������������
					{
						// �����Ʒ�����¼�
						USHORT sNum = sCount - AcpGrid[i].sNum;

						CCharacter	*pCCtrlCha = pRequest->GetPlyCtrlCha(), *pCMainCha = pRequest->GetPlyMainCha();
						Long	lPosX, lPosY;
						pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
						if( pCCtrlCha->GetSubMap()->ItemSpawn( AcpGrid + i, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle() ) == NULL )
						{
							/*pAccept->SystemNotice( "����ʱ��%s����װ���µ���Ʒ��%s���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							pRequest->SystemNotice( "����ʱ��%s����װ���µ���Ʒ��%s���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],����ʱ��{}����װ���µ���Ʒ��{}���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[{}], Num[{}]", sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],when trading,{} bag is full,��{}��failed to put on floor��trade res failed��ID[{}], Num[{}]", 
								sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
						}
					}
					else if( sPushRet != enumKBACT_SUCCESS )
					{						
						/*pAccept->SystemNotice( "����ʱ����Ʒ��%s������%s����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( "����ʱ����Ʒ��%s������%s����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],����ʱ����Ʒ��{}������{}����ʧ�ܣ�������Ʒ��ʧ��ID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), AcpGrid[i].sID, ReqGrid[i].sNum);*/
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pRequest->GetName(), 
							AcpGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],it failed to put res in {} bag when trading res ��{}����trade res failed��ID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), 
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
							/*pAccept->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "���Ӹ�{}����֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failed��DBID[0x{:X}]", 
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
						/*pRequest->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = {}", pTradeData->ReqTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��cannot find this res information��this res cannot give you��ID = {}", 
							pTradeData->ReqTradeData.ItemArray[i].sItemID );
						continue;
					}

					// �������߸��Ķ������������
					USHORT sCount = ReqGrid[i].sNum;
					Short sPushPos = defKITBAG_DEFPUSH_POS;
					Short sPushRet = pAccept->KbPushItem( true, false, ReqGrid + i, sPushPos );
					
					if( sPushRet == enumKBACT_ERROR_FULL ) // ��������������������
					{
						// �����Ʒ�����¼�
						USHORT sNum = sCount - ReqGrid[i].sNum;

						CCharacter	*pCCtrlCha = pAccept->GetPlyCtrlCha(), *pCMainCha = pAccept->GetPlyMainCha();
						Long	lPosX, lPosY;
						pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
						if( pCCtrlCha->GetSubMap()->ItemSpawn( ReqGrid + i, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle() ) == NULL )
						{
							/*pAccept->SystemNotice( "����ʱ��%s����װ���µ���Ʒ��%s���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							pRequest->SystemNotice( "����ʱ��%s����װ���µ���Ʒ��%s���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],����ʱ��{}����װ���µ���Ʒ��{}���ŵ�����ʧ�ܣ�������Ʒ��ʧ��ID[{}], Num[{}]", sPushRet, pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum);*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00070), 
								pAccept->GetName(), pItem->szName, ReqGrid[i].sID, ReqGrid[i].sNum );
							ToLogService("trade", LogLevel::Error, "Error code[{}],when trading,{} bag is full,��{}��failed to put on floor��trade res failed��ID[{}], Num[{}]", 
								sPushRet, pRequest->GetName(), pItem->szName, AcpGrid[i].sID, AcpGrid[i].sNum );
						}
					}
					else if( sPushRet != enumKBACT_SUCCESS )
					{						
						/*pAccept->SystemNotice( "����ʱ����Ʒ��%s������%s����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( "����ʱ����Ʒ��%s������%s����ʧ�ܣ�������Ʒ��ʧ��ID[%d], Num[%d]", pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],����ʱ����Ʒ��{}������{}����ʧ�ܣ�������Ʒ��ʧ��ID[{}], Num[{}]", sPushRet, pItem->szName, pAccept->GetName(), ReqGrid[i].sID, ReqGrid[i].sNum);*/
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00071), pItem->szName, pAccept->GetName(), 
							ReqGrid[i].sID, ReqGrid[i].sNum );
						ToLogService("trade", LogLevel::Error, "Error code[{}],it failed to put res in {} bag when trading res ��{}����trade res failed��ID[{}], Num[{}]", sPushRet, pItem->szName, pRequest->GetName(), 
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
							/*pAccept->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "���Ӹ�{}����֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failed��DBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
						}
					}
				}
			}

			// ��Ǯ
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

			// ���ݿ�洢
			game_db.BeginTran();
			if( !pRequest->SaveAssets() || !pAccept->SaveAssets() )
			{
				game_db.RollBack();

				// �������ݿ�洢ʧ�ܣ����ݻָ�
				ReqBag = ReqBagData;
				AcpBag = AcpBagData;
				pRequest->setAttr( ATTR_GD, dwReqMoney );
				pAccept->setAttr( ATTR_GD, dwAcpMoney );

				// �ָ���ֻ������Ϣ
				for( int i = 0; i < ROLE_MAXNUM_TRADEDATA; i++ )
				{
					if( pTradeData->AcpTradeData.ItemArray[i].sItemID != 0 )
					{
						CItemRecord* pItem = GetItemRecordInfo( pTradeData->AcpTradeData.ItemArray[i].sItemID );
						if( pItem == NULL )
						{
							/*pRequest->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��it cannot find res information��it cannot give you this res��ID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
							continue;
						}

						// �������߸��Ķ�������������					
						if( pItem->sType == enumItemTypeBoat )
						{
							if( !pRequest->BoatClear( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								
								/*pAccept->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "ɾ��{}�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} have��DBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}

							if( !pAccept->BoatAdd( AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "���Ӹ�{}����֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failed��DBID[0x{:X}]", 
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
							/*pRequest->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = %d", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "��ƷID�����޷��ҵ�����Ʒ��Ϣ�����ܸ��������Ʒ��ID = {}", pTradeData->AcpTradeData.ItemArray[i].sItemID);*/
						pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00069), 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
						ToLogService("trade", LogLevel::Error, "res ID error��it cannot find res information��it cannot give you this res��ID = {}", 
							pTradeData->AcpTradeData.ItemArray[i].sItemID );
							continue;
						}

						// �������߸��Ķ������������
						if( pItem->sType == enumItemTypeBoat )
						{
							if( !pAccept->BoatClear( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								
								/*pAccept->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( "ɾ��%s�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "ɾ��{}�Ĵ���֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
								pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00068), 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
								ToLogService("trade", LogLevel::Error, "it failed to delete boat that captain confirm of {} have��DBID[0x{:X}]", 
									pRequest->GetName(), ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}

							if( !pRequest->BoatAdd( ReqGrid[i].GetDBParam( enumITEMDBP_INST_ID ) ) )
							{
								/*pAccept->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( "���Ӹ�%s����֤��ӵ�еĴ�ֻʧ�ܣ�ID[0x%X]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "���Ӹ�{}����֤��ӵ�еĴ�ֻʧ�ܣ�DBID[0x{:X}]", pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ));*/
							pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00072), 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							ToLogService("trade", LogLevel::Error, "add to {}captain confirm it hold boat failed��DBID[0x{:X}]", 
								pRequest->GetName(), AcpGrid[i].GetDBParam( enumITEMDBP_INST_ID ) );
							}
						}
					}
				}

				// ֪ͨ�ͻ��˲��Ҽ�¼��־
				/*pRequest->SystemNotice( "����ʧ�ܣ����ݴ洢����" );
				pAccept->SystemNotice( "����ʧ�ܣ����ݴ洢����" );
				ToLogService("trade", LogLevel::Error, "�������ݴ洢���ݿ�ʧ�ܣ��������ݻָ���ɣ����ף����󷽡�{}��ID[0x{:X}]�����ܷ���{}��ID[0x{:X}]��", pRequest->GetName(), pRequest->GetPlayer()->GetDBChaId(), pAccept->GetName(), pAccept->GetPlayer()->GetDBChaId());*/
				pRequest->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00074) );
				pAccept->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00074) );
				ToLogService("trade", LogLevel::Error, "the trade data failed to memory in DB��trade data resume complete��trade��request one��{}��ID[0x{:X}]��accept one��{}��ID[0x{:X}]��",
					pRequest->GetName(), pRequest->GetPlayer()->GetDBChaId(), pAccept->GetName(), pAccept->GetPlayer()->GetDBChaId() );

				// ȡ����ɫ����״̬
				pAccept->TradeAction( FALSE );
				pRequest->TradeAction( FALSE );

				// ��ɫ���׳ɹ�
				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_CHARTRADE );
				WRITE_SHORT(packet, CMD_MC_CHARTRADE_RESULT );
				WRITE_CHAR(packet, TRADE_FAILER );

				pAccept->ReflectINFof( pMain, packet );
				pRequest->ReflectINFof( pMain, packet );

				return FALSE;
			}
			else
			{
				// �������ݴ洢�ɹ�
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

			// ������Ʒ�ɹ����ȡ����
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

			// ֪ͨ���׽�Ǯ������Ϣ
			char szNotice[255];

			if( pTradeData->ReqTradeData.dwMoney )
			{
				//pAccept->SystemNotice( "���(%s)���õ���%d��Ǯ��", pRequest->GetName(), pTradeData->ReqTradeData.dwMoney );
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

			

			// ͬ����Ǯ���ݺͱ�������
			pAccept->SynAttr( enumATTRSYN_TRADE );
			pAccept->SyncBoatAttr(enumATTRSYN_TRADE);
			pRequest->SynAttr( enumATTRSYN_TRADE );	
			pRequest->SyncBoatAttr(enumATTRSYN_TRADE);

			pRequest->SynKitbagNew( enumSYN_KITBAG_TRADE );
			pAccept->SynKitbagNew( enumSYN_KITBAG_TRADE );

			if (pTradeData->AcpTradeData.dwIMP > 0 || pTradeData->ReqTradeData.dwIMP > 0){
				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_UPDATEIMP);
				WRITE_LONG(packet, pAccept->GetIMP());
				pAccept->ReflectINFof(pMain, packet);

				WPACKET packet2 = GETWPACKET();
				WRITE_CMD(packet2, CMD_MC_UPDATEIMP);
				WRITE_LONG(packet2, pRequest->GetIMP());
				pRequest->ReflectINFof(pMain, packet2);
			}

			// ȡ����ɫ����״̬
			pAccept->TradeAction( FALSE );
			pRequest->TradeAction( FALSE );

			// ��ɫ���׳ɹ�
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_RESULT );
			WRITE_CHAR(packet, TRADE_SUCCESS );
			
			pAccept->ReflectINFof( pMain, packet );
			pRequest->ReflectINFof( pMain, packet );
		}
		else
		{
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_CHARTRADE );
			WRITE_SHORT(packet, CMD_MC_CHARTRADE_VALIDATE );
			WRITE_LONG(packet, pMain->GetID() );
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
			//pCha->SystemNotice("������һ��������δ������!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00077));

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

			return false;
		}

		SItemData *pComData = GetItemData(lComID);
		if(!pComData || pComData->store_head.comNumber == 0)
		{
			//pCha->SystemNotice("����Ʒ�ѳ���!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00078));

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

			return false;
		}

		cChar *szClsName = GetClassName(pComData->store_head.comClass);
		if(szClsName)
		{
			//if(!strcmp(szClsName, "�׽��Ա"))
			if(!strcmp(szClsName, RES_STRING(GM_CHARTRADE_CPP_00079)))
			{
				// Modify by lark.li 20080919 begin
				//if(pCha->GetPlayer()->GetVipType() == 0 || )
				if(pCha->GetPlayer()->GetVipType() == 0 || pCha->m_SChaPart.sTypeID == 1 || pCha->m_SChaPart.sTypeID == 2  )
				// End
				{
					//pCha->SystemNotice("ֻ�а׽��Ա�����������Ʒ!");
					pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00080));

					WPACKET WtPk	= GETWPACKET();
					WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
					WRITE_CHAR(WtPk, 0);
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
				//pCha->SystemNotice( "Request: �������Ʒ�������ͣ�ID = %d", pItem->itemID );
				pCha->SystemNotice( RES_STRING(GM_CHARTRADE_CPP_00081), pItem->itemID );

				WPACKET WtPk	= GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
				WRITE_CHAR(WtPk, 0);
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
			//pCha->PopupNotice("������ʱ�����ռ䲻��!");
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}] repeat!", pNm->msgHead.msgOrder);

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("�̳ǲ���ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			pCha->SetStoreBuy(true);
			PushOrder(pCha, pNm->msgHead.msgOrder, lComID, 1);
			//LG("Store_record", "��ɫ[%s][ID:%ld]��������Ʒ[ID:%ld]!\n", pChaInfo->chaName, pChaInfo->chaID, lComID);
			ToLogService("store", "character [{}][ID:{}] order merchandise [ID:{}]!", pChaInfo->chaName, pChaInfo->chaID, lComID);
		}
		else
		{
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("�̳ǽ���ʧ��!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00084));

			//LG("Store_msg", "Request: InfoServer����ʧ��!\n");
		
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

			//LG("Store_record", "��ɫ[%s][ID:%ld]��������Ʒ[ID:%ld], �ӵ��߲����ɹ�!\n", pCha->GetName(), pCha->GetPlayer()->GetDBChaId(), lComID);
			{ char _buf[256]; sprintf(_buf, RES_STRING(GM_CHARTRADE_CPP_00085), pCha->GetName(), pCha->GetPlayer()->GetDBChaId(), lComID); g_logManager.InternalLog(LogLevel::Debug, "store", _buf); }

			if(pComData->store_head.comNumber > 0)
			{
				pComData->store_head.comNumber--;
			}

			//ɾ����Ʒ
			if(pComData->store_head.comNumber <= 0 && pComData->store_head.comNumber != -1)
			{
				DelItemData(lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Accept2: �Ҳ�����Ʒ[ID:%ld]!\n", lComID);
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

			//LG("Store_record", "��ɫ[%s][ID:%ld]��������Ʒ[ID:%ld], �ѿۿ�!\n", ChaInfo->chaName, lChaID, lComID);
			ToLogService("store", "character[{}][ID:{}] has buy res[ID:{}], has buckle money!", ChaInfo->chaName, lChaID, lComID);
			SItemData *pCData = GetItemData(lComID);
			if(pCData->store_head.comNumber > 0)
			{
				pCData->store_head.comNumber--;
			}

			if(!pCha)
			{
				//LG("Store_msg", "��ɫ[%s][ID:%ld]���뿪!\n", ChaInfo->chaName, lChaID);
				ToLogService("store", "character[{}][ID:{}] has leava!", ChaInfo->chaName, lChaID);
			}

			//�ӵ���
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
					//LG("Store_msg", "�Ҳ�����Ʒ[ID:%ld]!\n", lComID);
					ToLogService("store", "cannot finde merchandise[ID:{}]!", lComID);
				}
			}
			else
			{
				//LG("Store_msg", "��ɫ[%s][ID:%ld]���ڱ���ͼ!\n", ChaInfo->chaName, lChaID);
				ToLogService("store", "character[{}][ID:{}] don't in this map!", ChaInfo->chaName, lChaID);

				BOOL bOnline;
				if(!game_db.IsChaOnline(lChaID, bOnline))
				{
					//LG("Store_msg", "��ȡ��ɫ����״̬ʧ�ܡ�\n");
					ToLogService("store", "it failed to get character online state��");
				}
				else
				{
					if(!bOnline)
					{
						//LG("Store_msg", "��ɫ�����ߡ�\n");
						ToLogService("store", "character didn't online��");

						if(!game_db.SaveStoreItemID(lChaID, lComID))
						{
							//LG("Store_msg", "�������߽�ɫ��Ʒ��Ϣʧ�ܡ�\n");
							ToLogService("store", "it failed to memory merchandise information who did not online character��");
						}
					}
					else
					{
						//LG("Store_msg", "��ɫ[%s][ID:%ld]��������ͼ!\n", ChaInfo->chaName, lChaID);
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
									WPACKET WtPk	=GETWPACKET();
									WRITE_CMD(WtPk, CMD_MM_STORE_BUY);
									WRITE_LONG(WtPk, pChaOut->GetID());
									WRITE_LONG(WtPk, lChaID);
									WRITE_LONG(WtPk, lComID);
									//WRITE_LONG(WtPk, ChaInfo->moBean);
									WRITE_LONG(WtPk, ChaInfo->rplMoney);
									pChaOut->ReflectINFof(pChaOut, WtPk);//ͨ��

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
			//��¼
			if(bSucc)
			{
				ToLogService("store", "��ɫ[{}][ID:{}]��������Ʒ[ID:{}], �ӵ��߲����ɹ�!", pCha->GetName(), lChaID, lComID);

				//֪ͨ��ҽ��׳ɹ�
				WPACKET WtPk	= GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
				WRITE_CHAR(WtPk, 1);
				WRITE_LONG(WtPk, ChaInfo->rplMoney);

				pCha->ReflectINFof(pCha, WtPk);
			}
			else
			{
			}

			//ɾ����Ʒ
			if(pCData->store_head.comNumber <= 0 && pCData->store_head.comNumber != -1)
			{
				DelItemData(lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Accept:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			//֪ͨ��ҽ���ʧ��
			if(pCha)
			{
				pCha->SetStoreBuy(false);

				WPACKET WtPk	= GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_STORE_BUY_ASR);
				WRITE_CHAR(WtPk, 0);
				pCha->ReflectINFof(pCha, WtPk);

				//pCha->SystemNotice("�̳ǽ���ʧ��!");
				pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00084));
				//LG("Store_data", "[%s]�������[ComID:%ld]ʧ��!\n", pCha->GetName(), OrderInfo.lComID);
				ToLogService("store", "[{}]failed to buy prop [ComID:{}]!", pCha->GetName(), OrderInfo.lComID);
			}
		}
		else
		{
			//LG("Store_msg", "Cancel:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
						//pCha->SystemNotice("�̳ǲ�����ʱ,�ѱ�ȡ��!");
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
		//LG("Store_msg", "�����̳��б�!\n");
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
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_LIST_ASR);
			short sItemNum = 0;
			short sPageNum = static_cast<short>((m_ItemList[lClsID].size() % sNum == 0) ? (m_ItemList[lClsID].size() / sNum) : (m_ItemList[lClsID].size() / sNum + 1));
			WRITE_SHORT(WtPk, sPageNum);
			WRITE_SHORT(WtPk, sPage);
			if(sPage > sPageNum)
			{
				sItemNum = 0;
				//LG("Store_msg", "��������ҳ�泬���˷�Χ!\n");
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
			WRITE_SHORT(WtPk, sItemNum);
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

				WRITE_LONG(WtPk, (*it).store_head.comID);
				WRITE_SEQ(WtPk, (*it).store_head.comName, uShort(strlen((*it).store_head.comName) + 1));
				WRITE_LONG(WtPk, (*it).store_head.comPrice);
				WRITE_SEQ(WtPk, (*it).store_head.comRemark, USHORT(strlen((*it).store_head.comRemark) + 1));
				WRITE_CHAR(WtPk, (*it).store_head.isHot);
				WRITE_LONG(WtPk, static_cast<long>((*it).store_head.comTime));
				WRITE_LONG(WtPk, static_cast<long>((*it).store_head.comNumber));
				WRITE_LONG(WtPk, l_time);

				WRITE_SHORT(WtPk, (*it).store_head.itemNum);
				int j;
				for(j = 0; j < (*it).store_head.itemNum; j++)
				{
					WRITE_SHORT(WtPk, (*it).pItemArray[j].itemID);
					WRITE_SHORT(WtPk, (*it).pItemArray[j].itemNum);
					WRITE_SHORT(WtPk, (*it).pItemArray[j].itemFlute);
					int k;
					for(k = 0; k < 5; k++)
					{
						WRITE_SHORT(WtPk, (*it).pItemArray[j].itemAttrID[k]);
						WRITE_SHORT(WtPk, (*it).pItemArray[j].itemAttrVal[k]);
					}
				}

				it++;
			}
			pCha->ReflectINFof(pCha, WtPk);
		}
		else
		{
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_LIST_ASR);
			WRITE_SHORT(WtPk, 0);
			WRITE_SHORT(WtPk, sPage);
			WRITE_SHORT(WtPk, 0);
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_VIP);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("�̳ǲ���ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_VIP);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

		//	LG("Store_msg", "RequestVIP: InfoServer����ʧ��!\n");
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
			//LG("Store_msg", "AcceptVIP:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_VIP);
			WRITE_CHAR(WtPk, 1);
			WRITE_SHORT(WtPk, HIWORD(dwVipParam));
			WRITE_SHORT(WtPk, LOWORD(dwVipParam));
			WRITE_LONG(WtPk, ChaInfo->rplMoney);
			WRITE_LONG(WtPk, ChaInfo->moBean);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->PopupNotice("����׽��Ա�ɹ�!");
			pCha->PopupNotice(RES_STRING(GM_CHARTRADE_CPP_00086));
			//LG("Store_data", "[%s]����VIP�ɹ�!\n", pCha->GetName());
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
				WPACKET WtPk	= GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_STORE_VIP);
				WRITE_CHAR(WtPk, 0);
				pCha->ReflectINFof(pCha, WtPk);
				//pCha->PopupNotice("����׽��Աʧ��!");
				pCha->PopupNotice(RES_STRING(GM_CHARTRADE_CPP_00087));
				//LG("Store_data", "[%s]����VIPʧ��!\n", pCha->GetName());
				ToLogService("store", "[{}]perchase VIP failed!", pCha->GetName());
			}
		}
		else
		{
			//LG("Store_msg", "CancelVIP:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}] repeat!", pNm->msgHead.msgOrder);

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_CHANGE_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("�̳ǲ���ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_CHANGE_ASR);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_msg", "RequestChange: InfoServer����ʧ��!\n");
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
			//LG("Store_msg", "AcceptChange:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_CHANGE_ASR);
			WRITE_CHAR(WtPk, 1);
			WRITE_LONG(WtPk, ChaInfo->moBean);
			WRITE_LONG(WtPk, ChaInfo->rplMoney);
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]�һ����ҳɹ�!\n", pCha->GetName());
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
			//֪ͨ��Ҷһ�����ʧ��
			CCharacter *pCha = NULL;
			CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(OrderInfo.ChaID);
			if(pPlayer)
			{
				pCha = pPlayer->GetMainCha();
			}

			if(pCha)
			{
				pCha->ResetStoreTime();
				WPACKET WtPk	= GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_STORE_CHANGE_ASR);
				WRITE_CHAR(WtPk, 0);
				pCha->ReflectINFof(pCha, WtPk);
				//LG("Store_data", "[%s]�һ�����ʧ��!\n", pCha->GetName());
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice("�̳ǲ���ʧ��, �������ظ�!");
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

			//LG("Store_msg", "RequestRoleInfo: InfoServer����ʧ��!\n");
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
			//LG("Store_msg", "AcceptRoleInfo:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			//LG("Store_data", "[%s]��ȡ�ʻ���Ϣ�ɹ�!\n", pCha->GetName());
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
			//LG("Store_msg", "CancelRoleInfo:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			/*pCha->SystemNotice("�޷��鵽�����˺�,���̳�ʧ��!");
			ToLogService("common", "[{}]��ȡ�ʻ���Ϣʧ��!", pCha->GetName());*/
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);

			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_QUERY);
			WRITE_CHAR(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//pCha->SystemNotice("�̳ǲ���ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00083));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, lNum);
		}
		else
		{
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_QUERY);
			WRITE_LONG(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_msg", "RequestRecord: InfoServer����ʧ��!\n");
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
			//LG("Store_msg", "AcceptRecord:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_QUERY);
			WRITE_LONG(WtPk, lNum);
			int i;
			for(i = 0; i < lNum; i++)
			{
				WRITE_SEQ(WtPk, ctime(&pRecord->tradeTime), USHORT(strlen(ctime(&pRecord->tradeTime)) + 1));
				WRITE_LONG(WtPk, pRecord->comID);
				WRITE_SEQ(WtPk, pRecord->comName, USHORT(strlen(pRecord->comName) + 1));
				WRITE_LONG(WtPk, pRecord->tradeMoney);
				pRecord++;
			}
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]��ѯ���׼�¼�ɹ�!\n", pCha->GetName());
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
			//LG("Store_msg", "CancelRecord:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_STORE_QUERY);
			WRITE_LONG(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);
			//LG("Store_data", "[%s]��ѯ���׼�¼ʧ��!\n", pCha->GetName());
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number[ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice("�ʼ�����ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00089));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			/*pCha->SystemNotice("GM�ʼ�����ʧ�ܣ�������Ѿ����͹�һ���ʼ�����ȴ�GM�ظ�֮���ٴη���!");
			ToLogService("common", "RequestGMSend: InfoServer����ʧ��!");*/
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
			//LG("Store_msg", "AcceptGMSend:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			/*pCha->SystemNotice("GM�ʼ����ͳɹ�, [����ID: %ld]!", lMailID);
			ToLogService("common", "[{}]����GM�ʼ��ɹ�!", pCha->GetName());*/
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
			//LG("Store_msg", "CancelGMSend:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			/*pCha->SystemNotice("GM�ʼ�����ʧ�ܣ�������Ѿ����͹�һ���ʼ�����ȴ�GM�ظ�֮���ٴη���!");
			ToLogService("common", "[{}]����GM�ʼ�ʧ��!", pCha->GetName());*/
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
			//LG("Store_msg", "������[ID:%I64i]�ظ�!\n", pNm->msgHead.msgOrder);
			ToLogService("store", "order form number [ID:{}]repeat!", pNm->msgHead.msgOrder);
			//pCha->SystemNotice("�ʼ�����ʧ��, �������ظ�!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00089));

			return false;
		}
		if(IsValid() && g_gmsvr->GetInfoServer()->SendData(pNm))
		{
			PushOrder(pCha, pNm->msgHead.msgOrder, 0, 0);
		}
		else
		{
			//pCha->SystemNotice("GM�ʼ�����ʧ��!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00092));
			//LG("Store_msg", "RequestGMRecv: InfoServer����ʧ��!\n");
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
			//LG("Store_msg", "AcceptGMRecv:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_GM_MAIL);
			WRITE_STRING(WtPk, pMi->title);
			WRITE_STRING(WtPk, pMi->description);
			WRITE_LONG(WtPk, static_cast<long>(pMi->sendTime));
			pCha->ReflectINFof(pCha, WtPk);
			/*pCha->SystemNotice("GM�ʼ��ظ�: [����ID: %ld]!", pMi->id);
			ToLogService("common", "[{}]����GM�ʼ��ɹ�!", pCha->GetName());*/
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
			//LG("Store_msg", "CancelGMRecv:�Ҳ�������[ID:%I64i]!\n", lOrderID);
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
			WPACKET WtPk	= GETWPACKET();
			WRITE_CMD(WtPk, CMD_MC_GM_MAIL);
			//WRITE_STRING(WtPk, "GMû���ʼ�����!");
			WRITE_STRING(WtPk, "GM do not have mail send to you!");
			WRITE_STRING(WtPk, "");
			WRITE_LONG(WtPk, 0);
			pCha->ReflectINFof(pCha, WtPk);

			//LG("Store_data", "[%s]����GM�ʼ�ʧ��!\n", pCha->GetName());
		
			ToLogService("store", "[{}]receive GM mail failed!", pCha->GetName());
		}

		return true;
	}

	BOOL CStoreSystem::GetAfficheList()
	{
		//LG("Store_msg", "���󹫸��б�!\n");
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
			//LG("Store_msg", "�����̳ǵ����б�!\n");
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

				//������Ʒ�ڵ�
				SItemData ItemNode;
				memcpy(&ItemNode.store_head, pStore, sizeof(StoreInfo));
				if(lItemNum > 0)
				{
					ItemNode.pItemArray = new ItemInfo[lItemNum];
					memcpy(ItemNode.pItemArray, pItem, lItemNum * sizeof(ItemInfo));
				}
				else
					ItemNode.pItemArray = NULL;

				//������Ʒ�б�
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
			//LG("Store_info", "�̳���Ʒ:\n");
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
			//LG("Store_error", "CStoreSystem::SetItemList() δ֪�쳣!\n");
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
		//LG("Store_msg", "�����̳ǵ��߷���!\n");
		ToLogService("store", "set store item sort!");
		ClearItemClass();
		while(lNum-- > 0)
		{
			m_ItemClass.push_back(*pClassList);
			pClassList++;
		}

		//for test
		//LG("Store_info", "�̳Ƿ���:\n");
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
		//LG("Store_msg", "�����̳ǹ����б�!\n");
		ToLogService("store", "set stroe affiche list!");
		ClearAfficheList();
		while(lNum > 0)
		{
			m_AfficheList.push_back(*pAfficheList);
			lNum--;
			pAfficheList++;
		}

		//for test
		//LG("Store_info", "�̳ǹ���:\n");
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
		WPACKET WtPk	= GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_STORE_OPEN_ASR);
		WRITE_CHAR(WtPk, bValid);	// �̳��Ƿ���

		if(bValid == 1)
		{
			WRITE_LONG(WtPk, vip);
			WRITE_LONG(WtPk, pCha->GetPlayer()->GetMoBean());	// Ħ��
			WRITE_LONG(WtPk, pCha->GetPlayer()->GetRplMoney());	// Ԫ��

			WRITE_LONG(WtPk, lAfficheNum);	// ��������
			for(i = 0; i < lAfficheNum; i++)
			{
				WRITE_LONG(WtPk, m_AfficheList[i].affiID);
				WRITE_SEQ(WtPk, m_AfficheList[i].affiTitle, uShort(strlen(m_AfficheList[i].affiTitle) + 1));
				WRITE_SEQ(WtPk, m_AfficheList[i].comID, uShort(strlen(m_AfficheList[i].comID) + 1));
			}
			WRITE_LONG(WtPk, lClsNum);	// ��������
			for(i = 0; i < lClsNum; i++)
			{
				WRITE_SHORT(WtPk, m_ItemClass[i].clsID);
				WRITE_SEQ(WtPk, m_ItemClass[i].clsName, uShort(strlen(m_ItemClass[i].clsName) + 1));
				WRITE_SHORT(WtPk, m_ItemClass[i].parentID);
			}
		}		
		pCha->ReflectINFof(pCha, WtPk);

		if(bValid != 1)
		{
			//pCha->SystemNotice("�����̳ǻ�δ����,���ڴ��̳���ҳ!");
			pCha->SystemNotice(RES_STRING(GM_CHARTRADE_CPP_00094));
		}

		return true;
	}

}