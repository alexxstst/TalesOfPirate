//add by ALLEN 2007-10-19
#include "stdafx.h"
#include "Auction.h"
#include "GameApp.h"
#include "GameDB.h"

CAuctionSystem g_AuctionSystem;

CAuctionSystem::CAuctionSystem()
{
}

CAuctionSystem::~CAuctionSystem()
{
	map<short, CAuctionItem *>::iterator it = m_mapItemList.begin();
	for(; it != m_mapItemList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mapItemList.clear();
}

BOOL CAuctionSystem::StartAuction(short sItemID, const string& strName, short sCount, uInt nBasePrice, uInt nMinBid)
{
	map<short, CAuctionItem *>::iterator it = m_mapItemList.find(sItemID);
	if(it != m_mapItemList.end())
	{
		//printf( "StartAuction:存在重复的ID(%d), name = %s!", sItemID, strName.c_str() );
		printf( RES_STRING(GM_AUCTION_CPP_00001), sItemID, strName.c_str() );
		//LG( "Auction_error", "StartAuction:存在重复的ID(%d), name = %s!", sItemID, strName.c_str() );
		LG( "Auction_error", "StartAuction:exist repeat ID(%d), name = %s!", sItemID, strName.c_str() );
		return false;
	}

	CAuctionItem *pAucItem = new CAuctionItem(sItemID, strName, sCount, nBasePrice, nMinBid);
	if(pAucItem == NULL)
	{
		//printf( "StartAuction:内存分配不足,ID(%d), name = %s!", sItemID, strName.c_str() );
		printf( RES_STRING(GM_AUCTION_CPP_00002), sItemID, strName.c_str() );
		//LG( "Auction_error", "StartAuction:内存分配不足,ID(%d), name = %s!", sItemID, strName.c_str() );
		LG( "Auction_error", "StartAuction:memory allot not enough,ID(%d), name = %s!", sItemID, strName.c_str() );
		return false;
	}

	m_mapItemList[sItemID] = pAucItem;
	return true;
}

BOOL CAuctionSystem::EndAuction(short sItemID)
{
	map<short, CAuctionItem *>::iterator it = m_mapItemList.find(sItemID);
	if(it == m_mapItemList.end())
	{
		//printf( "EndAuction:不存在的ID(%d)!", sItemID );
		printf( RES_STRING(GM_AUCTION_CPP_00003), sItemID );
		//LG( "Auction_error", "EndAuction:不存在的ID(%d)!", sItemID );
		LG( "Auction_error", "EndAuction:inexistent ID(%d)!", sItemID );
		return false;
	}

	CAuctionItem *pAucItem = m_mapItemList[sItemID];
	if(pAucItem->GetCurChaID() > 0)
	{
		BOOL bOnline = false;
		CCharacter *pCha = NULL;
		CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(pAucItem->GetCurChaID());
		if(pPlayer)
			pCha = pPlayer->GetMainCha();
		if(pCha)
		{
			g_CParser.DoString("AuctionEnd", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END);
		}
		else
		{
			game_db.IsChaOnline(pAucItem->GetCurChaID(), bOnline);
			if(!bOnline)
			{
				//LG("Auction", "EndAuction: 玩家 %s 不在线, 道具id = %d, count = %d!\n", pAucItem->GetCurChaName().c_str(), sItemID, pAucItem->GetItemCount());
				LG("Auction", "EndAuction: player %s not online, item id = %d, count = %d!\n", pAucItem->GetCurChaName().c_str(), sItemID, pAucItem->GetItemCount());
			}
			else
			{
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
							WRITE_CMD(WtPk, CMD_MM_AUCTION);
							WRITE_LONG(WtPk, pChaOut->GetID());
							WRITE_LONG(WtPk, pAucItem->GetCurChaID());
							pChaOut->ReflectINFof(pChaOut, WtPk);//通告

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
	else
	{
		//LG("Auction", "EndAuction: 竞拍结束, 没有玩家投标!\n");
		LG("Auction", "EndAuction: contest finish, no player to bid!\n");
	}
	SAFE_DELETE(pAucItem);
	m_mapItemList.erase(it);

	return true;
}

void CAuctionSystem::ListAuction(CCharacter* pCha, CCharacter* pNpc)
{
	short sNum = 0;

	WPACKET l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MC_LISTAUCTION);

	map<short, CAuctionItem *>::iterator it = m_mapItemList.begin();
	for(; it != m_mapItemList.end(); it++)
	{
		sNum++;
		CAuctionItem *pAucItem = (CAuctionItem *)(it->second);
		WRITE_SHORT(l_wpk, pAucItem->GetItemID());
		WRITE_STRING(l_wpk, pAucItem->GetName().c_str());
		WRITE_STRING(l_wpk, pAucItem->GetCurChaName().c_str());
		WRITE_SHORT(l_wpk, pAucItem->GetItemCount());
		WRITE_LONG(l_wpk, pAucItem->GetBasePrice());
		WRITE_LONG(l_wpk, pAucItem->GetMinBid());
		WRITE_LONG(l_wpk, pAucItem->GetCurPrice());
	}

	WRITE_SHORT(l_wpk, sNum);
	pCha->ReflectINFof( pCha, l_wpk);
}

void CAuctionSystem::NotifyAuction( CCharacter* pCha, CCharacter* pNpc )
{
	short sNum = 0;

	WPACKET l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MC_LISTAUCTION);

	map<short, CAuctionItem *>::iterator it = m_mapItemList.begin();
	for(; it != m_mapItemList.end(); it++)
	{
		sNum++;
		CAuctionItem *pAucItem = (CAuctionItem *)(it->second);
		WRITE_SHORT(l_wpk, pAucItem->GetItemID());
		WRITE_STRING(l_wpk, pAucItem->GetName().c_str());
		WRITE_STRING(l_wpk, pAucItem->GetCurChaName().c_str());
		WRITE_SHORT(l_wpk, pAucItem->GetItemCount());
		WRITE_LONG(l_wpk, pAucItem->GetBasePrice());
		WRITE_LONG(l_wpk, pAucItem->GetMinBid());
		WRITE_LONG(l_wpk, pAucItem->GetCurPrice());
	}

	WRITE_SHORT(l_wpk, sNum);
	pNpc->NotiChgToEyeshot(l_wpk, false);
}

BOOL CAuctionSystem::BidUp(CCharacter *pCha, short sItemID, uInt price)
{
	map<short, CAuctionItem *>::iterator it = m_mapItemList.find(sItemID);
	if(it == m_mapItemList.end())
	{
		//pCha->SystemNotice("竞拍已结束!");
		pCha->SystemNotice(RES_STRING(GM_AUCTION_CPP_00004));
		return false;
	}

	CAuctionItem *pAucItem = m_mapItemList[sItemID];
	return pAucItem->BidUp(pCha, price);
}