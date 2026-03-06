#pragma once

#ifndef UI_BOOTH_FORM_H
#define UI_BOOTH_FORM_H

#include "uiform.h"
#include "uiGlobalVar.h"

#include <vector>

struct NET_CHARTRADE_BOATDATA;

struct SItemGrid;


namespace GUI
{
	struct stNumBox;
	struct stTradeBox;
	struct stSelectBox;

	class CBoothMgr : public CUIInterface // 
	{
	public:
		CBoothMgr();
		~CBoothMgr();

		// 
		void ShowSetupBoothForm(int iLevel);
		void SearchAllStalls();
		// 
		void ShowTradeBoothForm(DWORD dwOwnerId, const char *szBoothName, int nItemNum);

		// 
		bool PushToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		// 
		bool PopFromBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		// 
		bool BoothToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);

		// 
		void AddTradeBoothItem(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney);
		void AddTradeBoothBoat(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data);
		void AddTradeBoothGood(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, SItemGrid& rSItemGrid);
		void RemoveTradeBoothItem( DWORD dwCharID, int iGrid, int iCount); // 

		void SetupBoothSuccess(); 
		void PullBoothSuccess()const;   

		// Getters And Setters
		CGoodsGrid* GetBoothItemsGrid() { return grdBoothItem; }
		DWORD GetOwnerId() const { return m_dwOwnerId; }
		void  SetOwnerId(DWORD dwOwnerId) { m_dwOwnerId = dwOwnerId; }
		//
		bool IsOpen() { return frmBooth->GetIsShow(); }
		//
		bool IsSetupedBooth() const { return m_bSetupedBooth; }
		void SetSetupedBooth(bool bSetupedBooth) { m_bSetupedBooth = bSetupedBooth; }

		//
		void CloseBoothByOther(DWORD dwOtherId)	{ if (dwOtherId == m_dwOwnerId)	CloseBoothUI();	}

	protected:
		virtual bool Init();
		virtual void End();
		virtual void CloseForm();

	private:

		struct SBoothItem;

		// 
		bool PushToBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 
		bool PopFromBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 
		bool PushToBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 
		bool PopFromBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		//
		int GetItemNumByLevel(int iLevel);
		//
		void ClearBoothItems();
		//
		void AddBoothItem(SBoothItem* pBoothItem);
		//
		void RemoveBoothItemByNum(SBoothItem* pBoothItem, int iNum);
		//
		void OpenBoothUI();
		//
		void CloseBoothUI();

	private:
		static void _MainMouseBoothEvent(CCompent *pSender, int nMsgType, 
										 int x, int y, DWORD dwKey);
		static void _MainBoothOnCloseEvent(CForm* pForm, bool& IsClose);

		static void _InquireSetupPushItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemCurrencyType(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeQuantity(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _SearchStallID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireSetupPushItemPriceEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireSetupPopItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireTradeItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		static void _BuyGoodsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _BuyAGoodEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		//
		CForm			* frmBooth;
		CLabel			* lblOwnerName;
		CEdit			* edtBoothName;
		CGoodsGrid		* grdBoothItem;
		CTextButton		* btnSetupBooth;
		CTextButton		* btnPullStakes;


		typedef std::vector<SBoothItem*> BoothItemContainer;
		typedef BoothItemContainer::iterator BoothItemConIter;

		SBoothItem		* m_pkCurrSetupBooth;

		BoothItemContainer m_kBoothItems;
		DWORD			m_dwOwnerId;
		int				m_iBoothItemMaxNum;
		bool			m_isOldEquipFormShow;
		bool			m_bSetupedBooth;

		stNumBox*		m_NumBox;
		stTradeBox*		m_TradeBox;
		stSelectBox*	m_SelectBox;

	};	// end of class CBoothMgr

	//add by ALLEN 2007-10-16
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CReadBookMgr
	{
	public:
		static bool IsCanReadBook(CCharacter* pCha);
		static bool ShowReadBookForm();

	private:
		static void _evtSelectBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtMsgBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static CCharacter* _pCha;
	};
}	// end of namespace GUI

#endif	// UI_BOOTH_FORM_H
