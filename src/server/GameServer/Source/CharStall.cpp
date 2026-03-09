// CharStall.cpp Created by knight-gongjian 2005.8.29.
//---------------------------------------------------------
#include "stdafx.h"
#include "CharStall.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "lua_gamectrl.h"
#include "GameDB.h"
#include <fstream>
#include <iostream>

using namespace std;
//---------------------------------------------------------
mission::CStallSystem g_StallSystem;

namespace mission
{
	#define MAX_STALL_MONEY 1000000000 // 1

	CStallData::CStallData(dbc::uLong lSize)
		: PreAllocStru(1)
	{
		Clear();
	}

	CStallData::~CStallData()
	{
	}

	void CStallData::Clear()
	{
		m_byNum = 0;
		memset( &m_Goods, 0, sizeof(STALL_GOODS)*ROLE_MAXNUM_STALL_GOODS );
	}

	// CStallSystem implemented

	CStallSystem::CStallSystem()
	{
	}

	CStallSystem::~CStallSystem()
	{
	}

	void CStallSystem::StartStall( CCharacter& staller, RPACKET& packet )
	{
		if (!staller.IsLiveing()){
			return;
		}
		if( staller.GetPlayer()->IsLuanchOut() )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00001) );
			return;
		}

		if( staller.GetTradeData() )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00001) );
			return;
		}

        if(staller.HasTradeAction())
        {
            staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00002) );
			return;
        }

		if( staller.GetBoat() )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00003));
			return;
		}

		if( staller.m_CKitbag.IsLock() || !staller.GetActControl(enumACTCONTROL_ITEM_OPT) )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00004));
			return;
		}

        if( staller.m_CKitbag.IsPwdLocked() )
        {
            //staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00005));
			return;
        }

		//add by ALLEN 2007-10-16
		if( staller.IsReadBook() )
        {
           // staller.SystemNotice( "" );
			 staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00006) );
			return;
        }

		BYTE byStallNum = staller.GetStallNum();
		if( byStallNum == 0 || byStallNum > ROLE_MAXNUM_STALL_GOODS )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00007) );
			return;
		}

		char szLog[2046] = "";
		char szTemp[128] = "";
		const char* pszName = packet.ReadString();
		if( pszName == NULL )
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00008) );
			return;
		}

		CStallData* pData = g_pGameApp->m_StallDataHeap.Get();
		if (!pData)
		{
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00009) );
			return;
		}

		// NPC add by ryan wang 2006 03 23----------------
		SubMap *pMap = staller.GetSubMap(); 

		if(!pMap)
		{
			//staller.SystemNotice( "" );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00010) );
			return;
		}
		else
		{
			if(!(pMap->GetMapRes()->CanStall()))
			{
				//staller.SystemNotice( "" );
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00011) );
				return;

			}
			if (g_Config.m_bBlindChaos && staller.IsPKSilver())
			{
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00011) );
				return;
			}
		}
	
		int r = 150; // 3
		int x = staller.GetPos().x;
		int y = staller.GetPos().y;
		unsigned long	ulMinDist2 = r * r;
		CCharacter	*pCTempCha = NULL;
		Long	lRangeB[] = {x, y, 0};
		Long	lRangeE[] = {enumRANGE_TYPE_CIRCLE, r};
		pMap->BeginSearchInRange(lRangeB, lRangeE);


		 /*
		while (pCTempCha = pMap->GetNextCharacterInRange())
		{
			if(pCTempCha->IsNpc())
			{
				//staller.SystemNotice( "" );
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00012) );
				return;
			}
			if(pCTempCha->IsPlayerCha())
			{
				if (pCTempCha->GetStallData())
				{
					if (pCTempCha->m_CSkillBag.GetSkillContByID(241)->chLv == 3) {
						staller.SystemNotice("You are stalling too close to a Stall Level 3!");
						return;
					}
					else {
						break;
					}
				}
			}
		}
		
		x = staller.GetPos().x;
		y = staller.GetPos().y;
		pMap = NULL;
		pMap = staller.GetSubMap();
		lRangeE[0] = enumRANGE_TYPE_CIRCLE;
		lRangeE[1] = 200;
		lRangeB[0] = x;
		lRangeB[1] = y;
		lRangeB[2] = 0;
		pCTempCha = NULL;
		*/
		while (pCTempCha = pMap->GetNextCharacterInRange())
		{
			if (pCTempCha->IsNpc())
			{
				//staller.SystemNotice( "" );
				staller.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00012));
				return;
			}
			if (pCTempCha->IsPlayerCha())
			{
				if (pCTempCha->GetStallData())
				{
					staller.SystemNotice("You are stalling too close to other player's stall!");
					return;
				}
			}
		}






		//-------------------------------------------------------------
		
		strncpy( pData->m_szName, pszName, ROLE_MAXNUM_STALL_NUM );
		//sprintf( szLog, "%s", staller.GetName() );
		sprintf( szLog, RES_STRING(GM_CHARSTALL_CPP_00013), staller.GetName() );
		pData->m_byNum = packet.ReadChar();
		if( pData->m_byNum == 0 || pData->m_byNum > byStallNum )
		{
			pData->Free();
			//staller.SystemNotice( "%s[%d]", staller.GetName(), pData->m_byNum );
			staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00014), staller.GetName(), pData->m_byNum );
			//LG( "stall_error", "%s[%d]", staller.GetName(), pData->m_byNum );
			ToLogService( "stall_error", "start to stallcharacter{}submit goods data over range[{}]", staller.GetName(), pData->m_byNum );
			return;
		}

		//sprintf( szTemp, "%d", pData->m_byNum );
		sprintf( szTemp, RES_STRING(GM_CHARSTALL_CPP_00015), pData->m_byNum );
		strcat( szLog, szTemp );

		for( BYTE i = 0;i < pData->m_byNum; i++ )
		{

			int grid = packet.ReadChar();
			int gold = packet.ReadLong();
			int count = packet.ReadChar();
			int index = packet.ReadChar();

			//validation for item stall.
			if (gold > 2000000000){
				int price = gold - 2000000000;
				int num = price / 100000;
				int ID = price - (num * 100000);

				CItemRecord* pInfo = GetItemRecordInfo(ID);

				if (!pInfo || pInfo->sType == 43 || !pInfo->chIsTrade){
					//if is invalid, just dont show item rather thanclosing the stall.
					continue;
				}
				else if(pInfo->nPileMax < num){
					num = pInfo->nPileMax;
				}
			}

			pData->m_Goods[i].byGrid  = grid;
			pData->m_Goods[i].dwMoney = gold;
			pData->m_Goods[i].byCount = count;
			pData->m_Goods[i].byIndex = index;

			if( pData->m_Goods[i].byGrid >= byStallNum )
			{
				pData->Free();
				//staller.SystemNotice( "%sGRID[%d]", staller.GetName(), pData->m_Goods[i].byGrid );
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00016), staller.GetName(), pData->m_Goods[i].byGrid );
				//LG( "stall_error", "%sGRID[%d]", staller.GetName(), pData->m_Goods[i].byGrid );
				ToLogService( "stall_error", "start to stallcharacter{}submit goods data index over rangeGRID[{}]", staller.GetName(), pData->m_Goods[i].byGrid );
				return;
			}

			__int64 n64Temp = (__int64)(pData->m_Goods[i].dwMoney) * pData->m_Goods[i].byCount;
			if (n64Temp > MAX_STALL_MONEY && pData->m_Goods[i].dwMoney <= 2000000000)
			{
				pData->Free();
				//staller.SystemNotice( "" );
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00017) );
				return;
			}

			// 
			if( staller.m_CKitbag.HasItem( pData->m_Goods[i].byIndex ) )
			{
				pData->m_Goods[i].sItemID = staller.m_CKitbag.GetID( pData->m_Goods[i].byIndex );
			}
			else
			{
				/*staller.SystemNotice( "%sID[%d]", staller.GetName(), pData->m_Goods[i].byIndex );
				LG( "stall_error", "%sID[%d]", staller.GetName(), pData->m_Goods[i].byIndex );
				pData->Free();*/
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00018), staller.GetName(), pData->m_Goods[i].byIndex );
				ToLogService( "stall_error", "start to stallcharacter{}submit data inexistence of stall goodsID[{}]", staller.GetName(), pData->m_Goods[i].byIndex );
				pData->Free();
				return;
			}
			
			pData->m_Goods[i].sItemID = staller.m_CKitbag.GetID( pData->m_Goods[i].byIndex );
			CItemRecord* pItem = (CItemRecord*)GetItemRecordInfo( pData->m_Goods[i].sItemID );
			if( pItem == NULL )
			{
				pData->Free();
				/*staller.SystemNotice( "IDID = %d", pData->m_Goods[i].sItemID );
				LG( "stall_error", "IDID = %d", pData->m_Goods[i].sItemID );*/
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00019), pData->m_Goods[i].sItemID );
				ToLogService( "stall_error", "start to stallres ID errorcannot find this res informationID = {}", pData->m_Goods[i].sItemID );
				return;
			}
			::SItemGrid*	grid2	=	staller.m_CKitbag.GetGridContByID(	pData->m_Goods[i].byIndex	);

			if(	grid2	&&	grid2->dwDBID	)
			{
				pData->Free();
				staller.SystemNotice(	"Item %s is bind, cannot be sold!",	pItem->szName	);
				return;
			}
			if( !pItem->chIsTrade || !staller.m_CKitbag.GetGridContByID(pData->m_Goods[i].byIndex)->GetInstAttr(ITEMATTR_TRADABLE))
			{
				pData->Free();
				/*staller.SystemNotice( "%s", pItem->szName );
				LG( "stall_error", "%s", pItem->szName );*/
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00020), pItem->szName );
				ToLogService( "stall_error", "start to stallres{}cannot trade", pItem->szName );
				return;
			}

			if( pData->m_Goods[i].byCount == 0 )
			{
				pData->m_Goods[i].byCount = 1;
			}

			if( staller.m_CKitbag.GetNum( pData->m_Goods[i].byIndex ) < pData->m_Goods[i].byCount )
			{
				/*staller.SystemNotice( "%sID[%d]", staller.GetName(), pData->m_Goods[i].byIndex );
				LG( "stall_error", "%sID[%d]", staller.GetName(), pData->m_Goods[i].byIndex );
				pData->Free();*/
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00021), staller.GetName(), pData->m_Goods[i].byIndex );
				ToLogService( "stall_error", "start to stallcharacter{}submit res of staller number errorID[{}]", staller.GetName(), pData->m_Goods[i].byIndex );
				pData->Free();
				return;
			}

			// 
			for( BYTE j = 0; j < i + 1; j++ )
			{
				if( i == j ) continue;
				if( pData->m_Goods[j].byGrid == pData->m_Goods[i].byGrid || 
					pData->m_Goods[j].byIndex == pData->m_Goods[i].byIndex )
				{
					pData->Free();
					/*staller.SystemNotice( "%sID[%d]", staller.GetName(), pData->m_Goods[i].byGrid );
					LG( "stall_error", "%sID[%d]", staller.GetName(), pData->m_Goods[i].byGrid );*/
					staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00022), staller.GetName(), pData->m_Goods[i].byGrid );
					ToLogService( "stall_error", "start to stallcharacter{}repeat submit res of staller data indexID[{}]", staller.GetName(), pData->m_Goods[i].byGrid );
					return;
				}
			}

			if( pItem->sType == enumItemTypeBoat )
			{
				DWORD dwBoatID = staller.m_CKitbag.GetDBParam( enumITEMDBP_INST_ID, pData->m_Goods[i].byIndex );
				CCharacter* pBoat = staller.GetPlayer()->GetBoat( dwBoatID );
				if( !pBoat )
				{
					/*staller.SystemNotice( "ID[0x%X]", dwBoatID );
					LG( "stall_error", "ID[0x%X]", dwBoatID );*/
staller.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00023), dwBoatID);
ToLogService("stall_error", "start to stallit cannot find the information of the boat that captain to confirm in this tradeID[0x{:X}]", dwBoatID);
pData->Free();
return;
				}
				else
				{
					//sprintf( szTemp, "%d%sID[0x%X]", pData->m_Goods[i].byCount, pBoat->GetName(), pData->m_Goods[i].byCount, dwBoatID ); 
					sprintf(szTemp, RES_STRING(GM_CHARSTALL_CPP_00024), pData->m_Goods[i].byCount, pBoat->GetName(), pData->m_Goods[i].byCount, dwBoatID);
				}
			}
			else
			{
				sprintf(szTemp, RES_STRING(GM_CHARSTALL_CPP_00025), pData->m_Goods[i].byCount, pItem->szName, pData->m_Goods[i].dwMoney);
			}
			strcat(szLog, szTemp);
		}

		TL(CHA_VENDOR, staller.GetName(), "", szLog);
		staller.SetStallData(pData);
		staller.StallAction();
		staller.SetStallName(pData->m_szName);
		staller.SynStallName();
		staller.m_CKitbag.Lock();
		//staller.SystemNotice( "" );
		staller.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00026));

		WPACKET wpk = GETWPACKET();
		WRITE_CMD(wpk, CMD_MC_STALL_START);
		WRITE_LONG(wpk, staller.GetID());
		staller.ReflectINFof(&staller, wpk);
	}

	void CStallSystem::CloseStall(CCharacter& staller)
	{
		if (!staller.GetStallData()) return;

		staller.StallAction(false);
		staller.SetStallName("");
		staller.m_CKitbag.UnLock();
		staller.GetStallData()->Free();
		staller.SetStallData(NULL);
		//staller.SystemNotice( "" );
		staller.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00027));

		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_STALL_CLOSE );
		WRITE_LONG(packet, staller.GetID() );
		staller.NotiChgToEyeshot( packet );
	}

	void CStallSystem::OpenStall(CCharacter& character, RPACKET& packet)
	{
		if (character.GetBoat())
		{
			//character.SystemNotice( "" );
			character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00028));
			return;
		}

		if (character.GetTradeData())
		{
			//character.SystemNotice( "" );
			character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00029));
			return;
		}

		if (character.GetPlayer()->IsLuanchOut())
		{
			//character.SystemNotice( "" );
			character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00030));
			return;
		}

		DWORD dwCharID = packet.ReadLong();
		CCharacter* pStaller = character.GetSubMap()->FindCharacter(dwCharID, character.GetShape().centre);
		if (!pStaller)
		{
			return;
		}

		if (pStaller->GetStallData())
		{
			SyncData(character, *pStaller);
		}
		else
		{
			//character.SystemNotice( "%s", pStaller->GetName() );
			character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00031), pStaller->GetName());
		}
	}
	
	void CStallSystem::SearchItem(CCharacter& ply, int itemID){
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_STALLSEARCH);

		int resultCount = 0;
		CCharacter * pCha;
		SubMap* map = ply.GetSubMap();
		map->BeginGetPlyCha();
		while ((pCha = map->GetNextPlyCha())){
			if (resultCount == 255){
				break;
			}
			
			if (pCha->IsPlayerCha() && pCha->IsLiveing()){
				CStallData* pData = pCha->GetStallData();
				if (pData){
					for (BYTE i = 0; i < pData->m_byNum; ++i){
						if (pData->m_Goods[i].sItemID == itemID){
							resultCount += 1;
							WRITE_STRING(WtPk, pCha->GetName());
							WRITE_STRING(WtPk, pCha->GetStallName());
							char buf[16];
							sprintf(buf, "%d,%d", (int)pCha->GetPos().x/100, (int)pCha->GetPos().y/100);
							WRITE_STRING(WtPk, buf);
							WRITE_LONG(WtPk, pData->m_Goods[i].byCount);
							WRITE_LONG(WtPk, pData->m_Goods[i].dwMoney);
							
							if (resultCount == 255){
								break;
							}
						}
					}
				}
				
			}
		}
		WRITE_SHORT(WtPk, resultCount);
		ply.ReflectINFof(&ply, WtPk);
	}

	void CStallSystem::BuyGoods( CCharacter& character, RPACKET& packet )
	{
		if( character.m_CKitbag.IsPwdLocked())
		{
			//character.SystemNotice( "," );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00032) );
			return;
		}

		//add by ALLEN 2007-10-16
		if( character.IsReadBook())
		{
			//character.SystemNotice( "," );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00033) );
			return;
		}

		if( character.GetBoat() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00028) );
			return;
		}

		if( character.GetTradeData() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00029) );
			return;
		}

		if( character.GetPlayer()->IsLuanchOut() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00030) );
			return;
		}

		if( character.m_CKitbag.IsFull() )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00034) );
			return;
		}

		DWORD dwCharID = packet.ReadLong();
		CCharacter* pStaller = character.GetSubMap()->FindCharacter( dwCharID, character.GetShape().centre );
		if( !pStaller || !pStaller->GetStallData() )
		{
			return;
		}

		BYTE byGrid = packet.ReadChar();
		BYTE byCount = packet.ReadChar();
		if( byCount == 0 )
		{
			return;
		}

		CStallData* pData = pStaller->GetStallData();
		BYTE byIndex = -1;
		for( BYTE i = 0; i < pData->m_byNum; ++i )
		{
			if( pData->m_Goods[i].byGrid == byGrid )
			{
				byIndex = i;
				break;
			}
		}

		if( byIndex == BYTE(-1) )
		{
			//character.SystemNotice( "" );
			character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00035));
			return;
		}

		if( byCount > pData->m_Goods[byIndex].byCount )
		{
			byCount = pData->m_Goods[byIndex].byCount;
		}

		if (pData->m_Goods[byIndex].dwMoney > 2000000000){
			int price = pData->m_Goods[byIndex].dwMoney-2000000000;
			int quantity = price / 100000;
			int itemID = price - (quantity * 100000);
			//consider changing this to check a given slot?
			CItemRecord* pInfo = GetItemRecordInfo(itemID);
			if (pInfo){
				if (!pInfo->chIsTrade || pInfo->sType == 43){
					character.SystemNotice("Invalid Item.");
					return;
				}
				//check if char has item.
				SItemGrid	*pSItem = 0;
				int slot = packet.ReadChar();
				pSItem = character.GetItem2(2, slot);
				if (!pSItem->GetInstAttr(ITEMATTR_TRADABLE)) {
					character.SystemNotice("Item is untradable!");
					return;
				}

				CKitbag& Bag = pStaller->m_CKitbag;
				Bag.UnLock();
				bool traded = false;
				for (int i = 0; i < byCount; i++){
					if (pSItem && pSItem->sNum >= quantity && pSItem->sID == itemID){
						//check if seller has inv space
						//check if char has inv space
						if (character.m_CKitbag.GetCapacity() - character.m_CKitbag.GetUseGridNum() < 1){
							character.SystemNotice("Your inventory is full.");
							Bag.Lock();
							return;
						}
						else if (Bag.GetCapacity() - Bag.GetUseGridNum() < 1){
							character.SystemNotice("The sellers inventory is full.");
							Bag.Lock();
							return;
						}
						else{
							Short sPushPos = defKITBAG_DEFPUSH_POS;
							//give seller item
							SItemGrid Grid;
							Grid.sNum = quantity;
							Grid.sID = itemID;

							Bag.UnLock();
							if (character.KbPopItem(true, false, &Grid, slot) != enumKBACT_SUCCESS){
								Bag.Lock();
								return;
							}
							Short sPushRet = pStaller->KbPushItem(true, false, &Grid, sPushPos);
							//give char item
							SItemGrid Grid2;
							Grid2.sNum = 1;
							Grid2.sID = pData->m_Goods[byIndex].sItemID;

							Bag.UnLock();
							if (pStaller->KbPopItem(true, false, &Grid2, pData->m_Goods[byIndex].byIndex) != enumKBACT_SUCCESS){
								Bag.Lock();
								return;
							}
							Short sPushRet2 = character.KbPushItem(true, false, &Grid2, sPushPos);
							pData->m_Goods[byIndex].byCount -= 1;

							character.SynAttr(enumATTRSYN_TRADE);
							character.SyncBoatAttr(enumATTRSYN_TRADE);
							character.SynKitbagNew(enumSYN_KITBAG_TRADE);
							pStaller->SynAttr(enumATTRSYN_TRADE);
							pStaller->SyncBoatAttr(enumATTRSYN_TRADE);
							pStaller->SynKitbagNew(enumSYN_KITBAG_TRADE);
							DelGoods(*pStaller, pData->m_Goods[byIndex].byGrid, 1);
							pStaller->RefreshNeedItem(Grid2.sID);
							character.RefreshNeedItem(Grid2.sID);

							traded = true;

						}
					}
					else{
						if (!traded){
							character.SystemNotice("Please drag over the item you are using to trade.");
						}
						Bag.Lock();
						return;
					}
				}
				Bag.Lock();
			}
			
		}
		else{


			__int64 n64Temp = (__int64)(pData->m_Goods[byIndex].dwMoney) * byCount;
			if (n64Temp > MAX_STALL_MONEY)
			{
				//character.SystemNotice( "" );
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00036));
				return;
			}

			if (pData->m_Goods[byIndex].dwMoney * byCount > (DWORD)character.getAttr(ATTR_GD))
			{
				//character.SystemNotice( "" );
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00037));
				return;
			}

			CKitbag& Bag = pStaller->m_CKitbag;
			if (!Bag.HasItem(pData->m_Goods[byIndex].byIndex))
			{
				/*character.SystemNotice( "ID[%d]", pData->m_Goods[byIndex].byIndex );
				LG( "stall_error", "ID[%d]", pData->m_Goods[byIndex].byIndex );*/
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00038), pData->m_Goods[byIndex].byIndex);
				ToLogService("stall_error", "errorthe res is inexistent that you want to buyID[{}]", pData->m_Goods[byIndex].byIndex);
				return;
			}

			if (Bag.GetNum(pData->m_Goods[byIndex].byIndex) < byCount)
			{
				/*character.SystemNotice( "ID[%d]", pData->m_Goods[byIndex].byIndex );
				LG( "stall_error", "ID[%d]", pData->m_Goods[byIndex].byIndex );*/
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00039), pData->m_Goods[byIndex].byIndex);
				ToLogService("stall_error", "inside errorthe res number error that you want to buyID[{}]", pData->m_Goods[byIndex].byIndex);
				return;
			}

			if (Bag.GetID(pData->m_Goods[byIndex].byIndex) != pData->m_Goods[byIndex].sItemID)
			{
				/*character.SystemNotice( "IDIDID0[%d], ID1[%d]",
					Bag.GetID( pData->m_Goods[byIndex].byIndex ), pData->m_Goods[byIndex].sItemID );
					LG( "stall_error", "IDIDID0[%d], ID1[%d]",
					Bag.GetID( pData->m_Goods[byIndex].byIndex ), pData->m_Goods[byIndex].sItemID );*/
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00040),
					Bag.GetID(pData->m_Goods[byIndex].byIndex), pData->m_Goods[byIndex].sItemID);
				ToLogService("stall_error", "inside errorthe res ID in backpack differ with stall information IDID0[{}], ID1[{}]",
					Bag.GetID(pData->m_Goods[byIndex].byIndex), pData->m_Goods[byIndex].sItemID);
				return;
			}

			CItemRecord* pItem = (CItemRecord*)GetItemRecordInfo(pData->m_Goods[byIndex].sItemID);
			if (pItem == NULL)
			{
				//character.SystemNotice( "IDID = %d", pData->m_Goods[byIndex].sItemID );
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00041), pData->m_Goods[byIndex].sItemID);
				return;
			}

			if (pItem->sType == enumItemTypeBoat && character.GetPlayer()->GetNumBoat() >= MAX_CHAR_BOAT)
			{
				//character.SystemNotice( "" );
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00042));
				return;
			}

			SItemGrid Grid;
			Grid.sNum = byCount;
			Grid.sID = pData->m_Goods[byIndex].sItemID;
			Short sPushPos = defKITBAG_DEFPUSH_POS;
			if (character.m_CKitbag.CanPush(&Grid, sPushPos) != enumKBACT_SUCCESS)
			{
				//character.SystemNotice( "!" );
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00043));
				return;
			}
			Grid.sNum = byCount;

			pStaller->m_CChaAttr.ResetChangeFlag();
			pStaller->SetBoatAttrChangeFlag(false);
			character.m_CChaAttr.ResetChangeFlag();
			character.SetBoatAttrChangeFlag(false);

			Bag.UnLock();
			pStaller->m_CKitbag.SetChangeFlag(false);
			character.m_CKitbag.SetChangeFlag(false);

			if (pStaller->KbPopItem(true, false, &Grid, pData->m_Goods[byIndex].byIndex) != enumKBACT_SUCCESS)
			{
				Bag.Lock();
				/*character.SystemNotice( "%sID[%d]", pStaller->GetName(), pData->m_Goods[byIndex].byIndex );
				LG( "stall_error", "%sID[%d]", pStaller->GetName(), pData->m_Goods[byIndex].byIndex );*/
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00044), pStaller->GetName(), pData->m_Goods[byIndex].byIndex);
				ToLogService("stall_error", "goods of stall trade fail that get from charcters{}bagID[{}]", pStaller->GetName(), pData->m_Goods[byIndex].byIndex);
				return;
			}

			Short sPushRet = character.KbPushItem(true, false, &Grid, sPushPos);
			if (sPushRet != enumKBACT_SUCCESS)
			{
				Bag.Lock();
				/*character.SystemNotice( "" );
				LG( "stall_error", "ID[%d]", pData->m_Goods[byIndex].sItemID );*/
				character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00045));
				ToLogService("stall_error", "inside error:the res that you bought failed to put in bagID[{}]", pData->m_Goods[byIndex].sItemID);
				return;
			}

			// 
			pData->m_Goods[byIndex].byCount -= byCount;

			char szLog[128] = "";
			char szNotice[255] = "";

			if (pItem->sType == enumItemTypeBoat)
			{
				CCharacter* pBoat = pStaller->GetPlayer()->GetBoat((DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
				if (!pBoat)
				{
					/*pStaller->SystemNotice( "ID[0x%X]", (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					character.SystemNotice( "ID[0x%X]", (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					LG( "stall_error", "ID[0x%X]", (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					sprintf( szLog, "%sID[0x%X]", character.GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) ); */
					pStaller->SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00046), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00046), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					ToLogService("stall_error", "Stall:it cannot find boat information that captain confirm in tradeID[0x{:X}]", (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					sprintf(szLog, RES_STRING(GM_CHARSTALL_CPP_00047), character.GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
				}
				else
				{
					//sprintf( szLog, "%sID[0x%X]", pBoat->GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) ); 
					sprintf(szLog, RES_STRING(GM_CHARSTALL_CPP_00048), pBoat->GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
				}

				if (!game_db.SaveBoat(*pBoat, enumSAVE_TYPE_OFFLINE))
				{
					Bag.Lock();

					/*pStaller->SystemNotice( "BuyGoods:%sID[0x%X]", pBoat->GetName(),
						(DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
						LG( "stall_error", "BuyGoods:%sID[0x%X]", pBoat->GetName(),
						(DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );*/
					pStaller->SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00049), pBoat->GetName(),
						(DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					ToLogService("stall_error", "BuyGoods:boat data save failedboat{}ID[0x{:X}]", pBoat->GetName(),
						(DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					return;
				}

				if (!pStaller->BoatClear((DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID)))
				{
					/*pStaller->SystemNotice( "%sID[0x%X]", pStaller->GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					character.SystemNotice( "%sID[0x%X]", pStaller->GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					LG( "stall_error", "%sID[0x%X]", pStaller->GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );*/
					pStaller->SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00050), pStaller->GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00050), pStaller->GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					ToLogService("stall_error", "stalldelete boat failed that charcter{}haveID[0x{:X}]", pStaller->GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
				}
			}
			else
			{
				//sprintf( szLog, ":%s", pItem->szName );
				sprintf(szLog, RES_STRING(GM_CHARSTALL_CPP_00051), pItem->szName);
			}
			Bag.Lock();

			character.setAttr(ATTR_GD, character.getAttr(ATTR_GD) - pData->m_Goods[byIndex].dwMoney * byCount);
			character.SynAttr(enumATTRSYN_TRADE);
			character.SyncBoatAttr(enumATTRSYN_TRADE);
			pStaller->setAttr(ATTR_GD, pStaller->getAttr(ATTR_GD) + (pData->m_Goods[byIndex].dwMoney * byCount));
			/*pStaller->SystemNotice( "%s%d%s%d%d%d", character.GetName(), byCount, pItem->szName,
				byCount * pData->m_Goods[byIndex].dwMoney, pData->m_Goods[byIndex].dwMoney, pStaller->getAttr( ATTR_GD ) );*/

			pStaller->SystemNotice(
				"\"%s\" bought %d item \"%s\" for %d gold. Unit price: %d, Total amount: %d",
				character.GetName(), byCount, pItem->szName, pData->m_Goods[byIndex].dwMoney* byCount,
				pData->m_Goods[byIndex].dwMoney, (std::uint32_t)pStaller->getAttr(ATTR_GD));

			pStaller->SynAttr(enumATTRSYN_TRADE);
			pStaller->SyncBoatAttr(enumATTRSYN_TRADE);
			pStaller->SynKitbagNew(enumSYN_KITBAG_TRADE);
			DelGoods(*pStaller, pData->m_Goods[byIndex].byGrid, byCount);

			// 
			pStaller->RefreshNeedItem(Grid.sID);
			character.RefreshNeedItem(Grid.sID);

			if (pItem->sType == enumItemTypeBoat)
			{
				if (!character.BoatAdd((DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID)))
				{
					/*pStaller->SystemNotice( "%sID[0xX]", character.GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					character.SystemNotice( "%sID[0xX]", character.GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );
					LG( "stall_error", "%sID[0xX]", character.GetName(), (DWORD)Grid.GetDBParam( enumITEMDBP_INST_ID ) );*/
					pStaller->SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00053), character.GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					character.SystemNotice(RES_STRING(GM_CHARSTALL_CPP_00053), character.GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
					ToLogService("stall_error", "stalladd boat failed that charcter{}boughtID[0xX]", character.GetName(), (DWORD)Grid.GetDBParam(enumITEMDBP_INST_ID));
				}
			}

			character.SystemNotice("You have bought from \"%s\" an item \"%s\" in quantity %d, spent %d gold! Unit price: %d, Left balance: %d.",
				pStaller->GetName(), pItem->szName, byCount, pData->m_Goods[byIndex].dwMoney* byCount,
				pData->m_Goods[byIndex].dwMoney, (std::uint32_t)character.getAttr(ATTR_GD));


			character.SynKitbagNew(enumSYN_KITBAG_TRADE);

			char szTemp[1024] = "";
			sprintf(szTemp, RES_STRING(GM_CHARSTALL_CPP_00055),
				szLog, pData->m_Goods[byIndex].dwMoney, byCount, pData->m_Goods[byIndex].dwMoney * byCount,
				pStaller->getAttr(ATTR_GD), character.getAttr(ATTR_GD));
			TL(CHA_VENDOR, pStaller->GetName(), character.GetName(), szTemp);
		}
		// Update the stall items information
		if( pData->m_Goods[byIndex].byCount == 0 )
		{
			STALL_GOODS Goods[ROLE_MAXNUM_STALL_GOODS];
			memcpy( Goods, pData->m_Goods, sizeof(STALL_GOODS)*ROLE_MAXNUM_STALL_GOODS );
			memcpy( pData->m_Goods + byIndex, Goods + byIndex + 1, sizeof(STALL_GOODS)*(ROLE_MAXNUM_STALL_GOODS - byIndex - 1) );
			pData->m_byNum--;
		}

		// check if disconnect flag is true
		if (g_Config.m_bDiscStall)
		{
			if (pData->m_byNum == 0)
			{
				// check if player is in offline mode state
				Long pStallerId = pStaller->GetID();
				CPlayer* pStallPly = pStaller->GetPlayer();
				if (!pStallPly || !pStallPly->IsValid()) {
					return;
				}

				if (pStaller->IsOfflineMode()) {
					pStallPly->GetCtrlCha()->BreakAction();
					pStallPly->MisLogout();
					pStallPly->MisGooutMap();
					g_pGameApp->ReleaseGamePlayer(pStallPly);
				}
			}
		}
	}

	void CStallSystem::DelGoods( CCharacter& staller, BYTE byGrid, BYTE byCount )
	{
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_STALL_DELGOODS );
		WRITE_LONG(packet, staller.GetID() );
		WRITE_CHAR(packet, byGrid);
		WRITE_CHAR(packet, byCount);
		staller.NotiChgToEyeshot( packet );
	}

	void CStallSystem::SyncData( CCharacter& character, CCharacter& staller )
	{
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_STALL_ALLDATA );
		WRITE_LONG(packet, staller.GetID() );

		mission::CStallData* pData = staller.GetStallData();
		if( pData == NULL ) return;		
		CKitbag& Bag = staller.m_CKitbag;

		WRITE_CHAR(packet, pData->m_byNum );
		WRITE_STRING(packet, pData->m_szName );

		for( BYTE i = 0; i < pData->m_byNum; ++i )
		{
			WRITE_CHAR(packet, pData->m_Goods[i].byGrid );
			WRITE_SHORT(packet, pData->m_Goods[i].sItemID );
			WRITE_CHAR(packet, pData->m_Goods[i].byCount );
			WRITE_LONG(packet, pData->m_Goods[i].dwMoney );

			CItemRecord* pItem = (CItemRecord*)GetItemRecordInfo( Bag.GetID( pData->m_Goods[i].byIndex ) );
			if( pItem == NULL )
			{
				/*staller.SystemNotice( "IDID = %d, Index = %d", 
					Bag.GetID( pData->m_Goods[i].byIndex ), pData->m_Goods[i].byIndex );
				character.SystemNotice( "IDID = %d, Index = %d", 
					Bag.GetID( pData->m_Goods[i].byIndex ), pData->m_Goods[i].byIndex );*/
				staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00056), 
					Bag.GetID( pData->m_Goods[i].byIndex ), pData->m_Goods[i].byIndex );
				character.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00056), 
					Bag.GetID( pData->m_Goods[i].byIndex ), pData->m_Goods[i].byIndex );
				return;
			}

			WRITE_SHORT(packet, pItem->sType );

			if( pItem->sType == enumItemTypeBoat )
			{
				CCharacter* pBoat = staller.GetPlayer()->GetBoat( (DWORD)Bag.GetDBParam( enumITEMDBP_INST_ID, pData->m_Goods[i].byIndex ) );
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
				// 
				SItemGrid* pGridCont = Bag.GetGridContByID( pData->m_Goods[i].byIndex );
				if( !pGridCont )
				{
					//staller.SystemNotice( "ID[%d]", pData->m_Goods[i].byIndex );
					staller.SystemNotice( RES_STRING(GM_CHARSTALL_CPP_00057), pData->m_Goods[i].byIndex );
					return;
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
				if( pGridCont->IsInstAttrValid() ) // 
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
					WRITE_CHAR( packet, 0 ); // 
				}
			}
		}
		character.ReflectINFof( &staller, packet );
	}
}
