//add by ALLEN 2007-10-19
#pragma once

#include "DBCCommon.h"
#include "AuctionItem.h"

_DBC_USING

class CAuctionSystem
{
public:
	CAuctionSystem();
	~CAuctionSystem();

	BOOL StartAuction(short sItemID, const std::string& strName, short sCount, uInt nBasePrice, uInt nMinBid);
	BOOL EndAuction(short sItemID);
	void ListAuction(CCharacter* pCha, CCharacter* pNpc);
	void NotifyAuction( CCharacter* pCha, CCharacter* pNpc );
	BOOL BidUp(CCharacter *pCha, short sItemID, uInt price);

private:
	std::map<short, CAuctionItem *> m_mapItemList;

};

extern CAuctionSystem g_AuctionSystem;
