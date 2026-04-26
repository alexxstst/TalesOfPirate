#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"
#include "uiboxform.h"

namespace GUI {
	// NPC
	class CNpcTradeMgr : public CUIInterface {
	public:
		friend class CEquipMgr;
		void ShowTradePage(const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID);
		void SaleToNpc(BYTE byIndex, BYTE byCount, USHORT sItemID, DWORD dwMoney);
		void BuyFromNpc(BYTE byIndex, BYTE byCount, USHORT sItemID, DWORD dwMoney);

		// NPC
		bool IsNpcGoods(CGoodsGrid* pGrid);

		// NPC
		void LocalBuyFromNpc(CGoodsGrid* pNpcGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem);

		// NPC
		void LocalSaleToNpc(CGoodsGrid* pNpcGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem);

		void SellSelectedItems(CGoodsGrid* grid);
		bool GetIsShow() {
			return frmNPCtrade->GetIsShow();
		}

	protected:
		bool Init();
		void End();
		void CloseForm();

		static void _BuyTradeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _SaleTradeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 
		static void _BuyEquipYesNoTradeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _SaleEquipYesNoTradeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		void _NpcItemRefresh(CItemCommand* pItem);

		CGoodsGrid* GetNPCtradeWeaponGrid() {
			return grdNPCtradeWeapon;
		}

		CGoodsGrid* GetNPCtradeEquipGrid() {
			return grdNPCtradeEquip;
		}

		CGoodsGrid* GetNPCtradeOtherGrid() {
			return grdNPCtradeOther;
		}

	private:
		DWORD _dwNpcID;
		bool _IsShow;

	private:
		CForm* frmNPCtrade;

		CGoodsGrid* grdNPCtradeWeapon; //NPC
		CGoodsGrid* grdNPCtradeEquip;
		CGoodsGrid* grdNPCtradeOther;

	private:
		struct stBuy {
			stBuy() : pBox(NULL), dwNpcID(0), nIndex(0), nDragIndex(0), nBuyGrid(0) {
			}

			stTradeBox* pBox;
			DWORD dwNpcID;
			int nIndex;
			int nDragIndex;
			int nBuyGrid;
		};

		stBuy _sBuy;

		struct stSale {
			stSale() : pBox(NULL), dwNpcID(0), nIndex(0) {
			}

			stTradeBox* pBox;
			DWORD dwNpcID;
			int nIndex;
		};

		stSale _sSale;
	};

	inline bool CNpcTradeMgr::IsNpcGoods(CGoodsGrid* pGrid) {
		return pGrid == grdNPCtradeWeapon || pGrid == grdNPCtradeEquip || pGrid == grdNPCtradeOther;
	}
}
