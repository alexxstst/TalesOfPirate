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
		// Заменено printf → логирование через snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_AUCTION_CPP_00001), sItemID, strName.c_str()); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
		//LG( "Auction_error", "StartAuction:�����ظ���ID(%d), name = %s!", sItemID, strName.c_str() );
		ToLogService("trade", LogLevel::Error, "StartAuction:exist repeat ID({}), name = {}!", sItemID, strName.c_str() );
		return false;
	}

	CAuctionItem *pAucItem = new CAuctionItem(sItemID, strName, sCount, nBasePrice, nMinBid);
	if(pAucItem == NULL)
	{
		// Заменено printf → логирование через snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_AUCTION_CPP_00002), sItemID, strName.c_str()); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
		//LG( "Auction_error", "StartAuction:�ڴ���䲻��,ID(%d), name = %s!", sItemID, strName.c_str() );
		ToLogService("trade", LogLevel::Error, "StartAuction:memory allot not enough,ID({}), name = {}!", sItemID, strName.c_str() );
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
		// Заменено printf → логирование через snprintf + InternalLog
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), RES_STRING(GM_AUCTION_CPP_00003), sItemID); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
		//LG( "Auction_error", "EndAuction:�����ڵ�ID(%d)!", sItemID );
		ToLogService("trade", LogLevel::Error, "EndAuction:inexistent ID({})!", sItemID );
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
				//LG("Auction", "EndAuction: ��� %s ������, ����id = %d, count = %d!\n", pAucItem->GetCurChaName().c_str(), sItemID, pAucItem->GetItemCount());
				ToLogService("trade", "EndAuction: player {} not online, item id = {}, count = {}!", pAucItem->GetCurChaName().c_str(), sItemID, pAucItem->GetItemCount());
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
							auto WtPk = net::msg::serialize(net::msg::MmAuctionMessage{pChaOut->GetID(), pAucItem->GetCurChaID()});
							pChaOut->ReflectINFof(pChaOut, WtPk);

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
		//LG("Auction", "EndAuction: ���Ľ���, û�����Ͷ��!\n");
		ToLogService("trade", "EndAuction: contest finish, no player to bid!");
	}
	SAFE_DELETE(pAucItem);
	m_mapItemList.erase(it);

	return true;
}

void CAuctionSystem::ListAuction(CCharacter* pCha, CCharacter* pNpc)
{
	// Типизированная сериализация: список аукционных предметов (клиенту)
	net::msg::McListAuctionMessage auctionMsg;
	for (auto it = m_mapItemList.begin(); it != m_mapItemList.end(); ++it)
	{
		CAuctionItem *pAucItem = it->second;
		auctionMsg.items.push_back({
			static_cast<int64_t>(pAucItem->GetItemID()), pAucItem->GetName(), pAucItem->GetCurChaName(),
			static_cast<int64_t>(pAucItem->GetItemCount()), static_cast<int64_t>(pAucItem->GetBasePrice()),
			static_cast<int64_t>(pAucItem->GetMinBid()), static_cast<int64_t>(pAucItem->GetCurPrice())
		});
	}
	auto l_wpk = net::msg::serialize(auctionMsg);
	pCha->ReflectINFof( pCha, l_wpk);
}

void CAuctionSystem::NotifyAuction( CCharacter* pCha, CCharacter* pNpc )
{
	// Типизированная сериализация: список аукционных предметов (окружению)
	net::msg::McListAuctionMessage auctionMsg;
	for (auto it = m_mapItemList.begin(); it != m_mapItemList.end(); ++it)
	{
		CAuctionItem *pAucItem = it->second;
		auctionMsg.items.push_back({
			static_cast<int64_t>(pAucItem->GetItemID()), pAucItem->GetName(), pAucItem->GetCurChaName(),
			static_cast<int64_t>(pAucItem->GetItemCount()), static_cast<int64_t>(pAucItem->GetBasePrice()),
			static_cast<int64_t>(pAucItem->GetMinBid()), static_cast<int64_t>(pAucItem->GetCurPrice())
		});
	}
	auto l_wpk = net::msg::serialize(auctionMsg);
	pNpc->NotiChgToEyeshot(l_wpk, false);
}

BOOL CAuctionSystem::BidUp(CCharacter *pCha, short sItemID, uInt price)
{
	map<short, CAuctionItem *>::iterator it = m_mapItemList.find(sItemID);
	if(it == m_mapItemList.end())
	{
		//pCha->SystemNotice("�����ѽ���!");
		pCha->SystemNotice(RES_STRING(GM_AUCTION_CPP_00004));
		return false;
	}

	CAuctionItem *pAucItem = m_mapItemList[sItemID];
	return pAucItem->BidUp(pCha, price);
}