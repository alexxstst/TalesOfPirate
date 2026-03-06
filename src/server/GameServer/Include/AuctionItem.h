//add by ALLEN 2007-10-19
#pragma once

#include "DBCCommon.h"
#include "Character.h"
#include "Player.h"
#include <string>

_DBC_USING
using namespace std;

class CAuctionItem
{
public:
	CAuctionItem(short sItemID, const string& strName, short sCount, uInt nBasePrice, uInt nMinBid);
	~CAuctionItem();

	uInt GetBasePrice() { return m_nBasePrice; }
	void SetBasePrice(uInt price) { m_nBasePrice = price; }

	uInt GetMinBid() { return m_nMinBid; }
	void SetMinbid(uInt price) { m_nMinBid = price; }

	uInt GetCurPrice() { return m_nCurPrice; }
	void SetCurPrice(uInt price) { m_nCurPrice = price; }

	DWORD GetCurChaID() { return m_dwCurChaID; }
	void SetCurChaID(DWORD id) { m_dwCurChaID = id; }

	short GetItemID() { return m_sItemID; }
	void SetItemID(short id) { m_sItemID = id; }

	short GetItemCount() { return m_sCount; }
	void SetItemCount(short sCount) { m_sCount = sCount; }

	string GetCurChaName() { return m_strCurChaName; }
	void SetCurChaName(string strName) { m_strCurChaName = strName; }

	string GetName() { return m_strName; }
	void SetName(string strName) { m_strName = strName; }

	BOOL BidUp(CCharacter *pCha, uInt price);

private:
	uInt m_nBasePrice;
	uInt m_nMinBid;
	uInt m_nCurPrice;
	DWORD m_dwCurChaID;
	string m_strCurChaName;
	string m_strName;

	short m_sItemID;
	short m_sCount;

};
