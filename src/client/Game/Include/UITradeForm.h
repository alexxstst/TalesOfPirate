#pragma once
#include "UIGlobalVar.h"
#include "gamecommon.h"

struct NET_CHARTRADE_BOATDATA;
namespace GUI
{
// 
class CTradeMgr : public CUIInterface
{
public:
	void	ShowCharTradeRequest( BYTE byType, DWORD dwRequestID );				// 
	void	ShowCharTrade( BYTE byType, DWORD dwAcceptID,DWORD dwRequestID );	// 
	void	CancelCharTrade(  DWORD dwCharID );						// 
	void	ShowCharTradeMoney(DWORD dwCharID, DWORD dwMoney);   // 
	void	ShowCharTradeIMP(DWORD dwCharID, DWORD dwMoney);   // 

	void	ValidateTradeData( DWORD dwCharID );
	void	ValidateTrade(  DWORD dwCharID );
	void	ShowTradeSuccess(); // 
	void	ShowTradeFailed();  // 

	void    CloseAllForm();		// 

	// 
	void	DragTradeToItem(DWORD dwCharID,BYTE byIndex,BYTE byItemIndex );

	//sItem:  byIndex :  byCount
	//byItemIndex: 
	void	DragItemToTrade(DWORD dwCharID,USHORT sItemID,BYTE byIndex,BYTE byCount,BYTE byItemIndex, SItemGrid* pGrid, const NET_CHARTRADE_BOATDATA* const pBoat );

	CGoodsGrid*     GetPlayertradeSaleGrid()    { return  grdSale;		} 
	CGoodsGrid*		GetRequestGrid()			{ return  grdRequest;	} //

	void			LocalSaleItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );  // 
	void			LocalCancelItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );// 

	// 
	bool			IsTrading();

	CForm*			GetForm()	{ return frmPlayertrade; } //

protected:
	bool Init();
    void End();
    void FrameMove(DWORD dwTime);

	void Clear();

static void _evtSelectYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
    static void _evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow);     // 

	static void _evtIMPFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtGoldFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _MainMousePlayerTradeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtSelfRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// 
	static void _evtOtherRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// 

private:
	CGoodsGrid* GetPlayertradeBuyGrid()     { return  grdBuy;	}
	static void _evtLocalSaleEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

CForm*		frmPlayertrade;
	CForm*		frmRequest;				//()
	CGoodsGrid* grdRequest;
	CGoodsGrid*	grdBuy;
	CGoodsGrid*	grdSale;
	CLabelEx*	labOtherGold;
	CLabelEx*	labSelfGold;

	CLabelEx*	labOtherIMP;
	CLabelEx*	labSelfIMP;
	
	CTextButton*	btnTrade;
	CTextButton*	btnYes;
	CTextButton*	btnGold;
	CTextButton*	btnIMP;

	CCheckBox*		chkTrade;
	CCheckBox*		chkYes;

	DWORD			m_dwAcceptID;         // 
	DWORD			m_dwRequestID;        // 
	BYTE			m_bTradeType;		  // ,RoleCommand.h TRADE_CHAR_TYPE

	DWORD			m_dwMainID;

struct stSale
	{
		stSale() : dwSaleID(0), nGridID(0), nDragID(0) {}

		DWORD   dwSaleID;
		int		nGridID;
		int		nDragID;
	};
	stSale		_sSale;

};

}

