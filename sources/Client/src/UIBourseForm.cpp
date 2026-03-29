#include "StdAfx.h"
#include "uibourseform.h"
#include "uiform.h"
#include "uiedit.h"
#include "uilabel.h"
#include "tools.h"
#include "uiformmgr.h"
#include "character.h"
#include "uigoodsgrid.h"
#include "UIBoxForm.h"
#include "UITradeForm.h"
#include "UIFastCommand.h"
#include "itemrecord.h"
#include "NetProtocol.h"
#include "UIItemCommand.h"
#include "UITemplete.h"
#include "UIBoatForm.h"
#include "characterrecord.h"
#include "gameapp.h"
#include "PacketCmd.h"
#include "shipset.h"
#include "UIMemo.h"
#include "worldscene.h"
#include "ShipFactory.h"
#include "UICozeForm.h"
#include "UIGoodsGrid.h"
#include "UIEquipForm.h"
#include "StringLib.h"
using namespace std;
using namespace GUI;

//---------------------------------------------------------------------------
// class CBourseMgr
//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// static 
//////////////////////////////////////////////////////////////////////////
const BYTE CBourseMgr::ITEM_TYPE = 0;

const float CBourseMgr::SALE_RATE = 0.5;	//50%

const int CBourseMgr::BUY_PAGE_INDEX = 0;		

const int CBourseMgr::SALE_PAGE_INDEX = 1;

const BYTE NO_TRADE_LEVEL = 0;
const BYTE ERR_TRADE_LEVEL = 100;



//////////////////////////////////////////////////////////////////////////
// CBourseMgr
//////////////////////////////////////////////////////////////////////////
CBourseMgr::CBourseMgr()
: m_iItemSelIndex(-1), grdShipRoom(NULL), frmBoatRoom(NULL)
{
}

//---------------------------------------------------------------------------
bool CBourseMgr::Init()
{
	CFormMgr &mgr = CFormMgr::s_Mgr;

	/**/
	{	
		frmSeaTrade = mgr.Find("frmSeaTrade", enumMainForm);
		if ( !frmSeaTrade)
		{
			g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(456));
			return false;
		}
		frmSeaTrade->evtEntrustMouseEvent = _MainMouseSeaTradeEvent ;

		//
		grdItemBuy = 
			dynamic_cast<CGoodsGrid*>(frmSeaTrade->Find("grdItemSale"));
		if (!grdItemBuy) 
			return Error(g_oLangRec.GetString(446),
			frmSeaTrade->GetName(), "grdItemSale");
		grdItemBuy->SetShowStyle(CGoodsGrid::enumSale);
		grdItemBuy->SetIsHint(true);
		grdItemBuy->evtBeforeAccept = __gui_event_sale_drag_before;

		grdItemSale = 
			dynamic_cast<CGoodsGrid*>(frmSeaTrade->Find("grdItemBuy"));
		if (!grdItemSale) 
			return Error(g_oLangRec.GetString(446), 
				frmSeaTrade->GetName(), "grdItemBuy");
		grdItemSale->SetShowStyle(CGoodsGrid::enumSale);
		grdItemSale->SetIsHint(true);
		grdItemSale->evtBeforeAccept = __gui_event_sale_drag_before;
	}
	return true;
}

//---------------------------------------------------------------------------
void CBourseMgr::End()
{

}

void CBourseMgr::CloseForm()
{
	if (frmSeaTrade->GetIsShow())
	{
		frmSeaTrade->SetIsShow(false);
	}
}

//---------------------------------------------------------------------------
void CBourseMgr::ShowNPCSelectShip(BYTE byNumBoat, const BOAT_BERTH_DATA& Data, 
								   BYTE byType)
{
	((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_launch_list->Update(byNumBoat, &Data);
}

//---------------------------------------------------------------------------
void CBourseMgr::ShowBourse(const NET_TRADEINFO& TradeInfo, BYTE byCmd, 
							DWORD dwNpcID, DWORD dwBoatID)
{
	m_dwNpcID = dwNpcID;
	m_dwBoatID = dwBoatID;

	if ( frmSeaTrade )   //
	{
		ClearItemList(m_BuyList);
		ClearItemList(m_SaleList);
		frmSeaTrade->ClearChild();
	}

	// 
	CBoat* pBoat = g_stUIBoat.FindBoat( dwBoatID );
	if (!pBoat) return; 
	CForm* pBoatRoom = pBoat->GetForm();
	if (!pBoatRoom) return;

	//,
	CForm* pForm = dynamic_cast<CForm*>(pBoatRoom->GetParent());
	xShipFactory* pkShip = ((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
	if (pkShip && pkShip->sbf.wnd->GetIsShow() && pForm == pkShip->sbf.wnd)
	{
		pkShip->sbf.wnd->SetIsShow(false);
	}

	frmBoatRoom = pBoatRoom;
	frmBoatRoom->SetParent(frmSeaTrade);
	frmBoatRoom->SetPos(0, 216);
	frmBoatRoom->Refresh();
	frmBoatRoom->SetIsShow(true);
	grdShipRoom = pBoat->GetGoodsGrid();
	if (!grdShipRoom) return;

	// 
	int i(0);
	ItemInfo_T* pkItemInfo(NULL);
	for (; i<TradeInfo.TradePage[BUY_PAGE_INDEX].byCount; i++ )
	{
		pkItemInfo = new ItemInfo_T();
		pkItemInfo->sId = TradeInfo.TradePage[BUY_PAGE_INDEX].sItemID[i];
		pkItemInfo->dwPrice = TradeInfo.TradePage[BUY_PAGE_INDEX].dwPrice[i];
		pkItemInfo->wNum = TradeInfo.TradePage[BUY_PAGE_INDEX].sCount[i];
		pkItemInfo->byLevel = TradeInfo.TradePage[BUY_PAGE_INDEX].byLevel[i];
		m_BuyList.push_back(pkItemInfo);
	}

	if (TradeInfo.TradePage[0].byCount == 0) 
	{
		m_iItemSelIndex = -1;
	} 
	else 
	{
		m_iItemSelIndex = 0;
	}//end of if 

	//
	for (i=0; i<TradeInfo.TradePage[SALE_PAGE_INDEX].byCount; i++)
	{
		pkItemInfo = new ItemInfo_T();
		pkItemInfo->sId = TradeInfo.TradePage[SALE_PAGE_INDEX].sItemID[i];
		pkItemInfo->dwPrice = TradeInfo.TradePage[SALE_PAGE_INDEX].dwPrice[i];
		pkItemInfo->wNum = TradeInfo.TradePage[SALE_PAGE_INDEX].sCount[i];
		pkItemInfo->byLevel = TradeInfo.TradePage[SALE_PAGE_INDEX].byLevel[i];
		m_SaleList.push_back(pkItemInfo);
	} //end of for

	SetItems();

	frmSeaTrade->Show();
}

void CBourseMgr::UpdateOneGood(BYTE byPage, BYTE byIndex, USHORT sItemID, USHORT sCount, DWORD dwPrice)
{	
	if (BUY_PAGE_INDEX == byPage)
	{
		m_BuyList[byIndex]->sId = sItemID;
		m_BuyList[byIndex]->dwPrice = dwPrice;
		m_BuyList[byIndex]->wNum = sCount;
	}
	else if (SALE_PAGE_INDEX == byPage)
	{
		m_SaleList[byIndex]->sId = sItemID;
		m_SaleList[byIndex]->dwPrice = dwPrice;
		m_SaleList[byIndex]->wNum = sCount;
	}

	SetItems();
}


//---------------------------------------------------------------------------
void CBourseMgr::ClearItemList(ItemList& itemList)
{
	const ItemListIter end = itemList.end();
	ItemListIter iter = itemList.begin();

	for (; iter != end; ++iter)
	{
		//delete *iter;
		SAFE_DELETE(*iter); // UI
	} //end of for 

	itemList.clear();
}

//---------------------------------------------------------------------------
bool CBourseMgr::IsTradeCmd(COneCommand* pkCommand)
{
	//if (pkCommand == cmdItem1)
	//	return true;
	//if (pkCommand == cmdItem2)
	//	return true;
	//if (pkCommand == cmdItem3) 
	//	return true;

	return false;
}

//---------------------------------------------------------------------------
void CBourseMgr::ChangeItem(eDirectType enumDirect/*= LEFT*/)
{
	if (m_iItemSelIndex < 0 || m_iItemSelIndex > (int)m_BuyList.size() - 1)
		return;

	//
	m_iItemSelIndex += ((int)(enumDirect));

	// 
	m_iItemSelIndex = (int)
		((m_iItemSelIndex + m_BuyList.size()) % m_BuyList.size());

	SetItems();
}

//---------------------------------------------------------------------------
bool CBourseMgr::ShowBoat(unsigned int iBoatIndex /*= 0*/)
{
	if (iBoatIndex > MAX_BOAT_NUM)
	{
		return false;
	}

	//
	//grdTradeRoom->Clear();

	CBoat* pkBoat = g_stUIBoat.GetBoat(iBoatIndex);
	if (!pkBoat || !pkBoat->GetIsValid())
	{
		m_dwBoatID = -1;
		return false;
	}

	m_dwBoatID = pkBoat->GetCha()->getAttachID();		//ID
	

	CGoodsGrid* pkGoodGrid = pkBoat->GetGoodsGrid();
	//grdTradeRoom->SetContent(pkGoodGrid->GetRow(), pkGoodGrid->GetCol());
	//grdTradeRoom->Refresh();
	CCommandObj *pkCmdObj, *pkNewCmdObj;
	int num = pkGoodGrid->GetMaxNum();
	for (int i(0); i<num; i++)
	{
		pkCmdObj = pkGoodGrid->GetItem(i);
		if( pkCmdObj )
		{
			pkNewCmdObj = dynamic_cast<CCommandObj*>( pkCmdObj->Clone() );
			//grdTradeRoom->SetItem(i, pkNewCmdObj);
		}
	} //end of for 

	return true;
}

//---------------------------------------------------------------------------
DWORD CBourseMgr::GetSalePriceById(USHORT sId)
{
	const ItemListIter end = m_SaleList.end();
	ItemListIter iter = m_SaleList.begin();

	for (; iter != end; ++iter)
	{
		if ((*iter)->sId == sId)
			break;
	} //end of for

	if (iter != end) 
	{
		return (*iter)->dwPrice;
	} //end of if 

	return 0;
}

//---------------------------------------------------------------------------
void CBourseMgr::BuyGoods(CItemCommand& rkBuy, int nFreeCnt)
{
	if (nFreeCnt == 0)
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(457));
		return;
	}

	int iNum = int(rkBuy.GetData().sNum);
	if (iNum == 0) 
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(458));
		return;
	} //end of if 

	int nBuyCount = 1;
	int nMax = -1;

	//int iPrice = pkBuy->GetData()

	if( rkBuy.GetPrice()>0 && CGameScene::GetMainCha() )
	{	/*  */
		nMax = CGameScene::GetMainCha()->getGameAttr()->get(ATTR_GD) /  rkBuy.GetPrice();

		if( nMax==0 )
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(459) );
			return;
		}
	}	

	int count = nFreeCnt * rkBuy.GetItemInfo()->nPileMax;
	nMax = nMax < count ? nMax : count;
	nMax = nMax < iNum ? nMax : iNum;

#if (GOOD_DISTINGUISH_PILE == 1)
	if( rkBuy.GetIsPile() )
	{	// 
		m_pkTradeBox = g_stUIBox.ShowTradeBox(
			_BuyGoodsEvent, (float)rkBuy.GetPrice(), nMax, rkBuy.GetItemInfo()->szName );
	}
	else
	{	// 
		char buf[256] = { 0 };
		sprintf(buf, g_oLangRec.GetString(460), rkBuy.GetName());
		g_stUIBox.ShowSelectBox(_BuyAGoodEvent, buf, true);
	}
#else
	m_pkTradeBox = g_stUIBox.ShowTradeBox(
		_BuyGoodsEvent, (float)rkBuy.GetPrice(), nMax, rkBuy.GetItemInfo()->szName );
#endif



	return;
}

//---------------------------------------------------------------------------
bool CBourseMgr::SaleGoods(CItemCommand& rkSaleCmd, int iGridIndex)
{
	g_stUIBourse.m_byGoodIndex = (BYTE)iGridIndex;


	DWORD dwPrice = this->GetSalePriceById(rkSaleCmd.GetData().sID);
	int iSalePrice(0);
	if (dwPrice == 0)
		iSalePrice = int(rkSaleCmd.GetPrice()*SALE_RATE);
	else
		iSalePrice = int(dwPrice);

	if( rkSaleCmd.GetIsPile() )
	{	/*  */
		m_pkTradeBox = 
			g_stUIBox.ShowTradeBox( _SaleGoodsEvent, 
									(float)iSalePrice, 
									rkSaleCmd.GetTotalNum(),
									rkSaleCmd.GetItemInfo()->szName );
		if (m_pkTradeBox->GetTradeNum() < rkSaleCmd.GetTotalNum())
			return false;
		else
			return true;
	}
	else/*  */
	{
		char buf[256] = { 0 };
		sprintf(buf, g_oLangRec.GetString(461), 
				StringSplitNum( iSalePrice ),
				rkSaleCmd.GetName());
		g_stUIBox.ShowSelectBox(_SaleAGoodEvent, buf, true);

		return true;
	}
}

//---------------------------------------------------------------------------
void CBourseMgr::Refresh()
{
	//cmdItem1->Refresh();
	//cmdItem2->Refresh();
	//cmdItem3->Refresh();

	//grdTradeRoom->Refresh();
}

//---------------------------------------------------------------------------
int CBourseMgr::GetGoodsIndex(CItemRecord* pkGoodRecord)
{
	int i(0);
	for (; i < (int)(m_BuyList.size()); i++)
	{
		if (m_BuyList[i]->sId == (USHORT)pkGoodRecord->lID)
			break;
	} //end of for

	if (i == m_BuyList.size())
		return -1;

	return i;

}

//---------------------------------------------------------------------------
CGoodsGrid* CBourseMgr::GetShipRoomGoodsGrid()
{
	return grdShipRoom;
}


//---------------------------------------------------------------------------
CGoodsGrid* CBourseMgr::GetBuyGoodsGrid() 
{ 
	return grdItemBuy; 
}

//---------------------------------------------------------------------------
CGoodsGrid* CBourseMgr::GetSaleGoodsGrid() 
{ 
	return grdItemSale; 
}

//---------------------------------------------------------------------------
void CBourseMgr::SetItems()
{
	grdItemSale->Clear();
	grdItemBuy->Clear();

	CItemRecord*  pInfo(NULL);
	CItemCommand* pItem(NULL);

	BYTE byTradeLevel = GetTradeLevel();
	if (byTradeLevel == ERR_TRADE_LEVEL)
	{
		ToLogService("common", "No GoodGrid.");
	}

	//
	int iIndex(0);
	for (; iIndex < (int)(m_BuyList.size()); ++iIndex)
	{
		pInfo = GetItemRecordInfo(m_BuyList[iIndex]->sId);
		if (!pInfo) return;
		pItem = new CItemCommand(pInfo);
		pItem->SetPrice( m_BuyList[iIndex]->dwPrice );
		//pItem->GetData().sID = m_BuyList[iIndex]->sId;
		pItem->GetData().sNum = m_BuyList[iIndex]->wNum;
		if (m_BuyList[iIndex]->byLevel > byTradeLevel)
		{
			pItem->SetIsValid(false);
		}
		if (!grdItemBuy->SetItem(iIndex, pItem))
		{
			g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(462));
		}
	} //end of for 

	//
	iIndex = 0;
	for (iIndex=0; iIndex<(int)(m_SaleList.size()); ++iIndex)
	{
		pInfo = GetItemRecordInfo(m_SaleList[iIndex]->sId);
		if (!pInfo) return;
		pItem = new CItemCommand(pInfo);
		pItem->SetPrice( m_SaleList[iIndex]->dwPrice );
		//pItem->GetData().sID = m_SaleList[iIndex]->sId;
		pItem->GetData().sNum = m_SaleList[iIndex]->wNum;
		if (!grdItemSale->SetItem(iIndex, pItem))
		{
			g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(462));
		}
	}
}

//---------------------------------------------------------------------------
BYTE CBourseMgr::GetTradeLevel()
{
	CGoodsGrid *pGoodGrid = g_stUIEquip.GetGoodsGrid();
	if (!pGoodGrid) return ERR_TRADE_LEVEL;

	CItemCommand *pItem(0);
	const int iGoodMaxNum = pGoodGrid->GetMaxNum();
	for (int i(0); i<iGoodMaxNum; i++)
	{
		pItem = dynamic_cast<CItemCommand*>(pGoodGrid->GetItem(i));
		if (pItem && 45 == pItem->GetItemInfo()->sType)
		{
			
			return BYTE(pItem->GetData().sEnergy[0]);
		}
	}

	return NO_TRADE_LEVEL;
}


//~  =================================================================
void CBourseMgr::_MainMouseSeaTradeEvent(CCompent *pSender, int nMsgType, 
										 int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if( name=="btnNo"  || name == "btnClose" )  
	{	
		//,
		return;
	}
	else if ( name == "btnYes")
	{
		//
		return ;			
	}
}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_left_rotate(CGuiData *sender, 
										 int x, int y, DWORD key )
{
	g_stUIBourse.ChangeItem(LEFT);
}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_right_rotate(CGuiData *sender,
										  int x, int y, DWORD key )
{
	g_stUIBourse.ChangeItem(RIGHT);
}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_left_continue_rotate(CGuiData *sender )
{
	g_stUIBourse.ChangeItem(LEFT);
}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_right_continue_rotate(CGuiData *sender )
{
	g_stUIBourse.ChangeItem(RIGHT);
}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_sale_drag_before(CGuiData *pSender, CCommandObj* pItem, 
										 int nGridID, bool& isAccept)
{
	isAccept = false;

	CGoodsGrid* pSelf = dynamic_cast<CGoodsGrid*>(pSender);
	if( !pSelf ) return;

	CGoodsGrid* pDrag = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (!pDrag )	return;

	CItemCommand* pkSaleCmd = dynamic_cast<CItemCommand*>(pItem);
	if (!pkSaleCmd)	return;

	if (pDrag == g_stUIBourse.GetShipRoomGoodsGrid())
	{
		g_stUIBourse.SaleGoods(*pkSaleCmd, pDrag->GetDragIndex());
	}

}

//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_drag_before(CGuiData *pSender, CCommandObj* pItem, 
										 int nGridID, bool& isAccept)
{
	//isAccept = false;

	//CGoodsGrid* pSelf = dynamic_cast<CGoodsGrid*>(pSender);
	//if( !pSelf ) return;

	//COneCommand* pDrag = dynamic_cast<COneCommand*>(CDrag::GetParent());
	//if (!pDrag)	return;

	//CItemCommand* pkBuyCmd = dynamic_cast<CItemCommand*>(pItem);
	//if (!pkBuyCmd)	return;
	//CItemRecord* pkBuyRecord = pkBuyCmd->GetItemInfo();
	//if (!pkBuyRecord)	return;

	//int nBuyGrid = nGridID;
	//int nBuyCount = 1;
	//if ( pkBuyCmd && pkBuyCmd->GetItemInfo()->GetIsPile() )
	//{	/**/
	//	CItemRecord* pRecord = pkBuyCmd->GetItemInfo();
	//	CItemCommand* pInfo = 0;
	//	int count = pSelf->GetMaxNum();
	//	for( int i(0); i<count; i++ )
	//	{
	//		pInfo = dynamic_cast<CItemCommand*>( pSelf->GetItem(i) );
	//		if( pInfo && pInfo->GetItemInfo()==pRecord )
	//		{
	//			nBuyGrid = i;
	//			break;
	//		}
	//	}  		   
	//}



	//if( pSelf == g_stUIBourse.GetGoodsGrid() )
	//{
	//	int iIndex = g_stUIBourse.GetGoodsIndex(pkBuyRecord);
	//	if (iIndex<0)
	//		return;

	//	g_stUIBourse.m_byGoodIndex = BYTE(iIndex);
	//	g_stUIBourse.m_byDragIndex = nGridID;
	//	g_stUIBourse.BuyGoods(pkBuyCmd);
	//	return;
	//}
}

//---------------------------------------------------------------------------
void CBourseMgr::BuyItem(CGoodsGrid& rkToGoodsGrid, CCommandObj& rkItem, 
						 int nGridID)
{
	CItemCommand* pkBuyCmd = dynamic_cast<CItemCommand*>(&rkItem);
	if (!pkBuyCmd)	return;
	if ( !(pkBuyCmd->GetIsValid()) )
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(463));
		return;
	}
	CItemRecord* pkBuyRecord = pkBuyCmd->GetItemInfo();
	if (!pkBuyRecord)	return;

	int nBuyGrid = nGridID;
	int nBuyCount = 1;

	if ( pkBuyCmd && pkBuyCmd->GetItemInfo()->GetIsPile() )
	{	/**/
		CItemRecord* pRecord = pkBuyCmd->GetItemInfo();
		CItemCommand* pInfo = 0;
		int count = rkToGoodsGrid.GetMaxNum();
		for( int i(0); i<count; i++ )
		{
			pInfo = dynamic_cast<CItemCommand*>( rkToGoodsGrid.GetItem(i) );
			if( pInfo && pInfo->GetItemInfo()==pRecord )
			{
				nBuyGrid = i;
				break;
			}
		}  		   
	}

	int iIndex = GetGoodsIndex(pkBuyRecord);
	if (iIndex<0)
		return;

	int max = rkToGoodsGrid.GetMaxNum();
	const int MAX_SIZE = 100;
	int pkBuf[MAX_SIZE] = { 0 };
	int nFreeCnt(0);
	rkToGoodsGrid.GetFreeIndex(pkBuf, nFreeCnt, MAX_SIZE);

	m_byGoodIndex = BYTE(iIndex);
	m_byDragIndex = nGridID;
	BuyGoods(*pkBuyCmd, nFreeCnt);
	return;
}


//---------------------------------------------------------------------------
void CBourseMgr::__gui_event_drag_before_com(CGuiData *pSender, CCommandObj* pItem, 
											 bool& isAccept)
{
	isAccept = false;

	COneCommand* pSelf = dynamic_cast<COneCommand*>(pSender);
	if( !pSelf ) return;

	CGoodsGrid* pDrag = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (!pDrag)	return;

	CItemCommand* pkSaleCmd = dynamic_cast<CItemCommand*>(pItem);
	if (!pkSaleCmd)	return;

	g_stUIBourse.SaleGoods(*pkSaleCmd, pDrag->GetDragIndex());
}

//---------------------------------------------------------------------------
void CBourseMgr::_BuyGoodsEvent(CCompent *pSender, int nMsgType, 
								 int x, int y, DWORD dwKey)
{
	if(nMsgType != CForm::mrYes) 
		return;

	CS_BuyGoods(g_stUIBourse.m_dwNpcID, 
				g_stUIBourse.m_dwBoatID, 
				ITEM_TYPE, 				
				g_stUIBourse.m_byGoodIndex, 
				g_stUIBourse.m_byDragIndex, 
				g_stUIBourse.m_pkTradeBox->GetTradeNum());
}

//---------------------------------------------------------------------------
void CBourseMgr::_SaleGoodsEvent(CCompent *pSender, int nMsgType, 
								 int x, int y, DWORD dwKey)
{
	if(nMsgType != CForm::mrYes) 
		return;

	CS_SaleGoods(g_stUIBourse.m_dwNpcID, 
				 g_stUIBourse.m_dwBoatID, 
				 g_stUIBourse.m_byGoodIndex, 
				 g_stUIBourse.m_pkTradeBox->GetTradeNum());
}

//---------------------------------------------------------------------------
void CBourseMgr::_BuyAGoodEvent(CCompent *pSender, int nMsgType, 
								int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) 
		return;

	CS_BuyGoods(g_stUIBourse.m_dwNpcID, 
				g_stUIBourse.m_dwBoatID, 
				ITEM_TYPE, 				
				g_stUIBourse.m_byGoodIndex, 
				g_stUIBourse.m_byDragIndex, 
				1);
}

//---------------------------------------------------------------------------
void CBourseMgr::_SaleAGoodEvent(CCompent *pSender, int nMsgType, 
								 int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) 
		return;

	CS_SaleGoods(g_stUIBourse.m_dwNpcID, 
				 g_stUIBourse.m_dwBoatID, 
				 g_stUIBourse.m_byGoodIndex, 
				 1);
}




////////////////////////////////////////////////////////////////////////////////
//
//  
//

bool CBlackTradeMgr::Init()
{// mothannakh exchanger hover//
	CFormMgr &mgr = CFormMgr::s_Mgr;
	frmBlackTrade = mgr.Find("frmBlackTrade");
	if(! frmBlackTrade)
	{
		ToLogService("common", "frmBlackTrade not Found.");
		return false;
	}

	grdItemSale = dynamic_cast<CGoodsGrid*>(frmBlackTrade->Find("grdItemSale"));
	if(! grdItemSale)
	{
		ToLogService("common", "frmBlackTrade:grdItemSale not Found.");
		return false;
	}

	grdItemBuy = dynamic_cast<CGoodsGrid*>(frmBlackTrade->Find("grdItemBuy"));
	if(! grdItemBuy)
	{
		ToLogService("common", "frmBlackTrade:grdItemBuy not Found.");
		return false;
	}

	grdItemSale->SetShowStyle(CGoodsGrid::enumOwnDef);
	grdItemSale->SetIsHint(true);

	grdItemBuy->SetShowStyle(CGoodsGrid::enumSmall);
	grdItemBuy->SetIsHint(true);

	grdItemBuy->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;
	frmBlackTrade->evtClose = _evtCloseForm;

	SetDefaultSaleGrid();

	return true;
}


void CBlackTradeMgr::CloseForm()
{
	if(frmBlackTrade)
	{
		frmBlackTrade->SetIsShow(false);
	}
}


void CBlackTradeMgr::SetNpcID(DWORD dwNpcID)
{
	//int nCount    = (int)m_vecBlackTrade.size();
	//int nColCount = grdItemSale->GetCol();
	//int nRowCount = nCount / nColCount + (nColCount % 3) == 0 ? 0 : 1;
	//grdItemSale->SetContent(nRowCount, nColCount);
	//mothannakh test end
	m_dwNpcID = dwNpcID;
	grdItemSale->Refresh();
}

bool CBlackTradeMgr::SailToBuy(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
{
	m_nDragIndex = rkDrag.GetDragIndex();

	CItemCommand* pItem = dynamic_cast<CItemCommand*>(rkDrag.GetItem(m_nDragIndex));
	if(! pItem)
		return false;

	CBoxMgr::ShowSelectBox( _TradeExchangeEvent, g_oLangRec.GetString(832), true ); // 

	//if(! pItem->GetIsValid())
	//{
	//	g_pGameApp->MsgBox(g_oLangRec.GetString(834)); // 
	//}
	//else
	//{
	//	CBoxMgr::ShowSelectBox( _TradeExchangeEvent, g_oLangRec.GetString(832)); // 
	//}

	return true;
}


void CBlackTradeMgr::ShowBlackTradeForm(bool b)
{
	if(b)
	{
		RefreshSaleGrid();
	}
	frmBlackTrade->SetIsShow(b);
}


void CBlackTradeMgr::RefreshSaleGrid()
{
	CItemCommand* pItem = NULL;

	int nCount = (int) m_vecBlackTrade.size();
	for(int i = 0; i < nCount; ++i)
	{
		pItem = dynamic_cast<CItemCommand*>(grdItemSale->GetItem(m_vecBlackTrade[i].sIndex));
		if(pItem)
		{
			if(m_vecBlackTrade[i].sSrcNum > g_stUIEquip.GetItemCount(m_vecBlackTrade[i].sSrcID))
			{
				// 
				pItem->SetIsValid(false);
			}
			else
			{
				pItem->SetIsValid(true);
			}
		}
	}
}

void CBlackTradeMgr::SetItem(stBlackTrade* pBlackTrade)
{
	CItemRecord*  pInfo(NULL);
	CItemCommand* pItem(NULL);

	pInfo = GetItemRecordInfo(pBlackTrade->sTarID);
	if (!pInfo) return;
	pItem = new CItemCommand(pInfo);
	pItem->GetData().sNum = pBlackTrade->sTarNum;		 // 

	pInfo = GetItemRecordInfo(pBlackTrade->sSrcID);
	if(pInfo)
	{
		char szBuffer[128] = {0};
		sprintf(szBuffer, "%d x %s", pBlackTrade->sSrcNum, pInfo->szName);
		pItem->SetOwnDefText(szBuffer);
	}

	grdItemSale->SetItem(pBlackTrade->sIndex, pItem);
	m_vecBlackTrade.push_back(*pBlackTrade);
}


void CBlackTradeMgr::ExchangeAnswerProc(bool bSuccess, stBlackTrade* pBlackTrade)
{
	if(bSuccess)
	{
		//  grid 
		int nCurNum = g_stUIBlackTrade.grdItemBuy->GetCurNum();
		if( nCurNum < g_stUIBlackTrade.grdItemBuy->GetMaxNum())
		{
			CItemRecord*  pInfo(NULL);
			CItemCommand* pItem(NULL);

			pInfo = GetItemRecordInfo(pBlackTrade->sTarID);
			if (!pInfo) return;
			pItem = new CItemCommand(pInfo);
			pItem->GetData().sNum = pBlackTrade->sTarNum;		 // 
			pInfo = GetItemRecordInfo(pBlackTrade->sSrcID);
			if(pInfo)
			{
				char szBuffer[128] = {0};
				sprintf(szBuffer, g_oLangRec.GetString(843), pBlackTrade->sSrcNum, pInfo->szName);	// x
				pItem->SetOwnDefText(szBuffer);
			}
			grdItemBuy->SetItem(nCurNum, pItem);
		}

		//
		// 
		//
		RefreshSaleGrid();
		CCozeForm::GetInstance()->OnSystemMsg(g_oLangRec.GetString(833)); // 
	}
	else
	{
		CCozeForm::GetInstance()->OnSystemMsg(g_oLangRec.GetString(834)); // 
	}
}


// 
void CBlackTradeMgr::_TradeExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if(nMsgType != CForm::mrYes)
	{
		return;
	}

	int nCount = (int) g_stUIBlackTrade.m_vecBlackTrade.size();
	for(int i = 0; i < nCount; ++i)
	{
		if(g_stUIBlackTrade.m_vecBlackTrade[i].sIndex == g_stUIBlackTrade.m_nDragIndex)
		{
			// 
			CS_BlackMarketExchangeReq(  g_stUIBlackTrade.GetNpcID(), 
										g_stUIBlackTrade.m_vecBlackTrade[i].sSrcID, 
										g_stUIBlackTrade.m_vecBlackTrade[i].sSrcNum, 
										g_stUIBlackTrade.m_vecBlackTrade[i].sTarID, 
										g_stUIBlackTrade.m_vecBlackTrade[i].sTarNum, 
										g_stUIBlackTrade.m_vecBlackTrade[i].sTimeNum, 
										g_stUIBlackTrade.m_nDragIndex  );

			return;
		}
	}
}


// 
void CBlackTradeMgr::_evtCloseForm(CForm* pForm, bool& IsClose)
{
	for(int i = 0; i < g_stUIBlackTrade.grdItemSale->GetMaxNum(); ++i)
	{
		g_stUIBlackTrade.grdItemSale->DelItem(i);
	}

	for(int i = 0; i < g_stUIBlackTrade.grdItemBuy->GetMaxNum(); ++i)
	{
		g_stUIBlackTrade.grdItemBuy->DelItem(i);
	}

	g_stUIBlackTrade.ClearItemData();
	g_stUIBlackTrade.SetDefaultSaleGrid();
}

