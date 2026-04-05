//add by ALLEN 2007-10-19
#include "stdafx.h"
#include "AuctionItem.h"
#include "GameDB.h"
#include "GameApp.h"

CAuctionItem::CAuctionItem(short sItemID, const string& strName, short sCount, uInt nBasePrice, uInt nMinBid)
{
	m_sItemID = sItemID;
	m_sCount = sCount;
	m_nBasePrice = nBasePrice;
	m_nMinBid = nMinBid;
	m_dwCurChaID = 0;
	m_nCurPrice = 0;
	m_strName = strName;
}

CAuctionItem::~CAuctionItem()
{
}

BOOL CAuctionItem::BidUp(CCharacter *pCha, uInt price)
{
	if(pCha->m_CKitbag.IsPwdLocked())
	{
		//pCha->SystemNotice(",!");
		pCha->SystemNotice(RES_STRING(GM_AUCTIONITEM_CPP_00001));
		return false;
	}

	if(pCha->IsReadBook())
	{
		//pCha->SystemNotice(",!");
		pCha->SystemNotice(RES_STRING(GM_AUCTIONITEM_CPP_00002));
		return false;
	}

	if(GetCurPrice() == 0)
	{
		if(!pCha->HasMoney(GetBasePrice()))
		{
			//pCha->SystemNotice("!");
			pCha->SystemNotice(RES_STRING(GM_AUCTIONITEM_CPP_00003));
			return false;
		}

		SetCurPrice(GetBasePrice());
		SetCurChaID(pCha->GetPlayer()->GetDBChaId());
		SetCurChaName(pCha->GetName());
		//if(pCha->TakeMoney("", GetCurPrice()))
		if(pCha->TakeMoney(RES_STRING(GM_AUCTIONITEM_CPP_00004), GetCurPrice()))
		//LG("Auction", " %s  %ld !\n", GetCurChaName().c_str(),GetCurPrice());
		ToLogService("trade", "character {} bid {} success!", GetCurChaName().c_str(),GetCurPrice());
		return true;
	}

	if((price < GetCurPrice()) || (price - GetCurPrice() < GetMinBid()))
	{
		//pCha->SystemNotice("!");
		pCha->SystemNotice(RES_STRING(GM_AUCTIONITEM_CPP_00005));
		return false;
	}

	if(!pCha->HasMoney(price))
	{
		//pCha->SystemNotice("!");
		pCha->SystemNotice(RES_STRING(GM_AUCTIONITEM_CPP_00003));
		return false;
	}

	DWORD dwPreChaID = GetCurChaID();
	uInt nPrePrice = GetCurPrice();
	string strPreChaName = GetCurChaName();

	SetCurPrice(price);
	SetCurChaID(pCha->GetPlayer()->GetDBChaId());
	SetCurChaName(pCha->GetName());
	//if(pCha->TakeMoney("", GetCurPrice()))
	if(pCha->TakeMoney( RES_STRING(GM_AUCTIONITEM_CPP_00004), GetCurPrice()))
		//LG("Auction", " %s  %ld !\n", GetCurChaName().c_str(),GetCurPrice());
		ToLogService("trade", "character {} bid {} success!", GetCurChaName().c_str(),GetCurPrice());

	//
	bool bOnline = false;
	if(!game_db.IsChaOnline(dwPreChaID, bOnline))
	{
		//LG("Auction", " %s !\n", strPreChaName.c_str());
		ToLogService("trade", "get character {} online state failed!", strPreChaName.c_str());
	}
	else
	{
		if(!bOnline)
		{
			if(!game_db.AddMoney(dwPreChaID, nPrePrice))
			{
				//LG("Auction", " %s  %ld !\n", strPreChaName.c_str(), nPrePrice);
				ToLogService("trade", "character {} back money {} failed!", strPreChaName.c_str(), nPrePrice);
			}
		}
		else
		{
			CCharacter *pCha_ = NULL;
			CPlayer *pPlayer = g_pGameApp->GetPlayerByDBID(dwPreChaID);
			if(pPlayer)
			{
				pCha_ = pPlayer->GetMainCha();
			}
			if(pCha_)
			{
				//pCha_->AddMoney("", nPrePrice);
				pCha_->AddMoney(RES_STRING(GM_AUCTIONITEM_CPP_00004), nPrePrice);
			}
			else
			{
				//  :      - 
				auto WtPk = net::msg::serialize(net::msg::MmAddMoneyMessage{
					static_cast<int64_t>(pCha->GetID()), static_cast<int64_t>(dwPreChaID), static_cast<int64_t>(nPrePrice)
				});
				pCha->ReflectINFof(pCha, WtPk);
			}
		}
	}

	return true;
}
