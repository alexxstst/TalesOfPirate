#include "StdAfx.h"
#include "uitradeform.h"
#include "gameapp.h"
#include "scene.h"
#include "character.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "packetcmd.h"
#include "uiequipform.h"
#include "uiboxform.h"
#include "uigoodsgrid.h"
#include "uiequipform.h"
#include "uicheckbox.h"
#include "uicommand.h"
#include "uiitemcommand.h"
#include "uiboatform.h"
#include "rolecommon.h"
#include "ShipFactory.h"
#include "rolecommon.h"
#include "StringLib.h"
using namespace GUI;
using namespace std;
//---------------------------------------------------------------------------
// class CTradeMgr
//---------------------------------------------------------------------------
bool CTradeMgr::Init()
{
	m_bTradeType = 0;
	m_dwAcceptID = 0;         // 
	m_dwRequestID = 0;        // 
	m_dwMainID = 0;

	frmRequest = NULL;				
	grdRequest = NULL;

	CFormMgr &mgr = CFormMgr::s_Mgr;

    //
	frmPlayertrade =  mgr.Find("frmPlayertrade");
	if ( !frmPlayertrade)
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(456));
		return false;
	}
	frmPlayertrade->evtEntrustMouseEvent = _MainMousePlayerTradeEvent ;

	// 
	grdSale = dynamic_cast<CGoodsGrid*>(frmPlayertrade->Find("grdSale"));
	if( !grdSale )		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "grdSale" );
	GetPlayertradeSaleGrid()->evtBeforeAccept = _evtDragToGoodsEvent;
	GetPlayertradeSaleGrid()->evtRMouseEvent = _evtSelfRMouseGridEvent;

	grdBuy = dynamic_cast<CGoodsGrid*>(frmPlayertrade->Find("grdBuy"));
	if( !grdBuy )		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "grdBuy" );
    GetPlayertradeBuyGrid()->evtBeforeAccept = _evtDragToGoodsEvent;
	GetPlayertradeBuyGrid()->evtRMouseEvent = _evtOtherRMouseGridEvent;

	//
	labOtherGold =  dynamic_cast<CLabelEx*>(frmPlayertrade->Find("labOtherGold") ); 
	if ( !labOtherGold)		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "labOtherGold" );
	
	labSelfGold =  dynamic_cast<CLabelEx*>(frmPlayertrade->Find("labSelfGold") ); 
	if ( !labSelfGold)		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "labSelfGold" );

	
	labOtherIMP =  dynamic_cast<CLabelEx*>(frmPlayertrade->Find("labOtherIMP") ); 
	if ( !labOtherIMP)		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "labOtherIMP" );
	
	labSelfIMP =  dynamic_cast<CLabelEx*>(frmPlayertrade->Find("labSelfIMP") ); 
	if ( !labSelfIMP)		return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "labSelfIMP" );
	
	
	
	chkYes = ( CCheckBox *)frmPlayertrade->Find( "chkYes" );     
	if ( !chkYes)	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "chkYes" );    

	chkTrade = ( CCheckBox *)frmPlayertrade->Find( "chkTrade" );  
	if ( !chkTrade )	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "chkTrade" );     

	btnYes =  ( CTextButton *)frmPlayertrade->Find( "btnYes" ); 
	if ( !btnYes )	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "btnYes" );         

	btnTrade =  ( CTextButton *)frmPlayertrade->Find( "btnTrade" ); 
	if ( !btnTrade )	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "btnTrade" );             

	btnGold =  ( CTextButton *)frmPlayertrade->Find( "btnGold" ); 
	if ( !btnGold )	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "btnGold" );  

	btnIMP =  ( CTextButton *)frmPlayertrade->Find( "btnIMP" ); 
	if ( !btnIMP )	return Error( g_oLangRec.GetString(45), frmPlayertrade->GetName(), "btnIMP" );    
	
	return true;
}

void CTradeMgr::End()
{
}

void CTradeMgr::FrameMove(DWORD dwTime)
{
    if(IsTrading())
    {
        CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
        if(pWorldScene && pWorldScene->GetShipMgr())
        {
	        pWorldScene->GetShipMgr()->CloseForm();
        }
    }
}

void CTradeMgr::ShowCharTradeRequest( BYTE byType, DWORD dwRequestID )
{
	CGameScene *pScene = CGameApp::GetCurScene();
	if(!pScene) return;

	CCharacter * pCha = pScene->SearchByID( dwRequestID );  //
	if( !pCha)	
	{
		if( !pCha ) return;
	}
	
	char szBuf[80] = { 0 };
	if( byType==mission::TRADE_CHAR )
		sprintf( szBuf, g_oLangRec.GetString(779), pCha->getHumanName() );
	else
		sprintf( szBuf, g_oLangRec.GetString(780), pCha->getHumanName() );


	// add by Philip.Wu  2006-06-11  
	//                               
	g_stUIBox.CloseAllBox();

	stSelectBox* pBox = g_stUIBox.ShowSelectBox( _evtSelectYesNoEvent, szBuf );
	pBox->dwTag = dwRequestID;
	pBox->dwParam = byType;
	static int PosY = 100;
	PosY+=10;
	if( PosY>150 ) PosY=100;
	pBox->frmDialog->SetPos( 10, PosY );
	pBox->frmDialog->Refresh();
}

void CTradeMgr::_evtSelectYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	pSender->GetForm()->Close();
	string name = pSender->GetName();
	if( name!="btnYes" ) return;

	stSelectBox* pBox = (stSelectBox*)pSender->GetForm()->GetPointer();
	CS_AcceptTrade( (BYTE)pBox->dwParam, pBox->dwTag );
}

void CTradeMgr::_MainMousePlayerTradeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if( name=="btnNo"  || name == "btnClose" )  //,
	{	
		if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwAcceptID )
			::CS_CancelTrade( g_stUITrade.m_bTradeType, g_stUITrade.m_dwRequestID );
		else
			::CS_CancelTrade( g_stUITrade.m_bTradeType, g_stUITrade.m_dwAcceptID );
		g_stUITrade.btnTrade->SetIsEnabled(false);
		g_stUITrade.btnYes->SetIsEnabled(false);
		g_stUITrade.btnGold->SetIsEnabled(false);
		g_stUITrade.btnIMP->SetIsEnabled(false);
		return;
	}
	else if ( name == "btnYes")
	{
		if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwAcceptID )
			::CS_ValidateTradeData( g_stUITrade.m_bTradeType, g_stUITrade.m_dwRequestID );
		else
			::CS_ValidateTradeData( g_stUITrade.m_bTradeType, g_stUITrade.m_dwAcceptID );

		g_stUITrade.btnYes->SetIsEnabled(false);
		g_stUITrade.btnGold->SetIsEnabled(false);
		g_stUITrade.btnIMP->SetIsEnabled(false);
		if( g_stUITrade.chkYes->GetIsChecked() )
			g_stUITrade.btnTrade->SetIsEnabled(true);
		return ;
	}
	else if ( name == "btnTrade" )
	{
		if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwAcceptID )
			::CS_ValidateTrade( g_stUITrade.m_bTradeType, g_stUITrade.m_dwRequestID );
		else
			::CS_ValidateTrade( g_stUITrade.m_bTradeType, g_stUITrade.m_dwAcceptID );
		g_stUITrade.btnTrade->SetIsEnabled(false);
		return;
	}
	else if ( name=="btnGold")
	{			
		g_stUIBox.ShowNumberBox( _evtGoldFormEvent, g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_GD), g_oLangRec.GetString(781), false );
	}
	else if ( name=="btnIMP")
	{			
		g_stUIBox.ShowNumberBox( _evtIMPFormEvent, g_stUIEquip.GetIMP(), "Please Input IMP", false );
	}
}

void CTradeMgr::_evtIMPFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	DWORD dwCharID = 0;
	if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwRequestID )
		dwCharID = g_stUITrade.m_dwAcceptID;	
	else
		dwCharID = g_stUITrade.m_dwRequestID;

	CS_AddIMP( g_stUITrade.m_bTradeType, dwCharID, mission::TRADE_DRAGMONEY_TRADE, pBox->GetNumber() );
}

void CTradeMgr::_evtGoldFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	DWORD dwCharID = 0;
	if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwRequestID )
		dwCharID = g_stUITrade.m_dwAcceptID;	
	else
		dwCharID = g_stUITrade.m_dwRequestID;

	CS_AddMoney( g_stUITrade.m_bTradeType, dwCharID, mission::TRADE_DRAGMONEY_TRADE, pBox->GetNumber() );
}

void CTradeMgr::ShowCharTrade( BYTE byType, DWORD dwAcceptID, DWORD dwRequestID )
{
	if( !frmPlayertrade ) return;			//

	CGameScene *pScene = g_pGameApp->GetCurScene();
	if(!pScene) return;

	m_bTradeType = byType;
	m_dwRequestID = dwRequestID;
	m_dwAcceptID = dwAcceptID;
	CCharacter* pRequestCha(NULL);
	CCharacter* pAcceptCha(NULL);

	if (mission::TRADE_CHAR == byType)		//
	{
		frmRequest = g_stUIEquip.GetItemForm();
		grdRequest = g_stUIEquip.GetGoodsGrid();  //Grid

		pRequestCha = pScene->SearchByHumanID( dwRequestID );
		pAcceptCha =  pScene->SearchByHumanID( dwAcceptID );
	} 
	else if (mission::TRADE_BOAT == byType)	//,,ID
	{		
		CCharacter* pMain = CGameScene::GetMainCha();
		if( !pMain ) return;

		if( !pMain->IsBoat() ) return;

		pRequestCha = pScene->SearchByID( dwRequestID );
		pAcceptCha =  pScene->SearchByID( dwAcceptID );

		CCharacter* pMainShip = NULL;
		if( pRequestCha->GetMainType() )
			pMainShip = pRequestCha;
		else
			pMainShip = pAcceptCha;

		xShipFactory* pkShip = ((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
		pkShip->SetState(xShipFactory::STATE_TRADE_FREEDOM);
		CBoat* pBoat = g_stUIBoat.FindBoat( pMainShip->getAttachID() );
		if( !pBoat )
		{
			return;			
		}
		// ,,
		CForm* pBoatRoom = pBoat->GetForm();
		if (!pBoatRoom) return;
		CForm* pForm = dynamic_cast<CForm*>(pBoatRoom->GetParent());
		if (pkShip && pkShip->sbf.wnd->GetIsShow() && pForm == pkShip->sbf.wnd)
		{
			pkShip->sbf.wnd->SetIsShow(false);
		}

		pkShip->SetBoatID(pMainShip->getAttachID());

		DWORD dwBuf[8];
		if (!CCharacter::ConvertPartTo8DWORD(pMainShip->GetPart(), dwBuf))
			return;
		pkShip->UpdateBoatFreedomTrade(pMainShip->getName().c_str(), dwBuf, sizeof(dwBuf));

		grdRequest = pBoat->GetGoodsGrid();
		frmRequest = pkShip->sbf.wnd;
		// frmRequest = pBoat->GetForm();
	}	//end of if

	if( !frmRequest )	return;
	if(!pRequestCha || !pAcceptCha) return;

	if( pRequestCha->GetMainType() )
		m_dwMainID = dwRequestID;
	else
		m_dwMainID = dwAcceptID;

	//,
	frmRequest->SetParent( NULL );
	frmRequest->SetPos( 0, 100 );
	frmRequest->Refresh();	
	frmRequest->Show();

	frmPlayertrade->SetPos( frmRequest->GetWidth(),100 );
	frmPlayertrade->Refresh();
	frmPlayertrade->Show();

	if( grdBuy ) grdBuy->Clear();
	if( grdSale )	grdSale->Clear();
    if( labOtherGold ) labOtherGold->SetCaption("0");
	if( labSelfGold )	labSelfGold->SetCaption("0");
	if( labOtherIMP )	labOtherIMP->SetCaption("0");
	if( labSelfIMP )	labSelfIMP->SetCaption("0");

	chkYes->SetIsChecked( false );
	chkTrade->SetIsChecked( false );

	btnYes->SetIsEnabled( true );
	btnGold->SetIsEnabled( m_bTradeType==mission::TRADE_CHAR );
	btnIMP->SetIsEnabled( m_bTradeType==mission::TRADE_CHAR );
	btnTrade->SetIsEnabled( false );	

	string strPlayTradeLabName("");			//labPlayertradeNameCaption
	strPlayTradeLabName = pRequestCha->getHumanName();
	strPlayTradeLabName +=  "/";
	strPlayTradeLabName += pAcceptCha->getHumanName();

	// 
	labOtherGold->SetIsShow( false );
	GetPlayertradeBuyGrid()->SetIsShow( false );

	CLabelEx  *labPlayertradeName = dynamic_cast<CLabelEx*>(frmPlayertrade->Find("labPlayertradeName") ); 
	if ( labPlayertradeName )
	{	
		labPlayertradeName->SetCaption( strPlayTradeLabName.c_str() );			
	}	
}

void CTradeMgr::DragTradeToItem( DWORD dwCharID, BYTE byIndex, BYTE byItemIndex )
{
	CGoodsGrid* tradeGrd = NULL;
	if ( g_stUITrade.m_dwMainID==dwCharID )
	{
		tradeGrd = g_stUITrade.GetPlayertradeSaleGrid();
	
		CCommandObj* pObj  = grdRequest->GetItem( byItemIndex );
		//ISSUE - IF HOVERING ITEM IT DOESNT SET TO VALID.
		if(pObj) pObj->SetIsValid(true);
	}
	else
	{
		tradeGrd = g_stUITrade.GetPlayertradeBuyGrid();
	}

	if ( tradeGrd ) tradeGrd->DelItem( byIndex );	
}

void  CTradeMgr::DragItemToTrade(DWORD dwCharID,USHORT sItemID,BYTE byIndex,BYTE byCount,BYTE byItemIndex, SItemGrid* pGrid, const NET_CHARTRADE_BOATDATA* const pBoat )
{
	ToLogService("common", "DragItemToTrade( CharID:{}, ItemID:{}, Index:{}, Count:{}, ItemIndex:{} ) - MainID:{}", dwCharID, sItemID, byIndex, byCount, byItemIndex, g_stUITrade.m_dwMainID);

	CGoodsGrid* tradeGrd;
	if ( m_dwMainID==dwCharID )
	{
		tradeGrd = g_stUITrade.GetPlayertradeSaleGrid();

		//
		CCommandObj* pSelf = grdRequest->GetItem( byItemIndex );
		if (pSelf)
		{
			pSelf->SetIsValid(false);
		}
	}
	else
	{
		tradeGrd = g_stUITrade.GetPlayertradeBuyGrid();
	}	

	CItemRecord *pItem = GetItemRecordInfo( sItemID );
	if( !pItem )
	{
		ToLogService("network", "GetItemRecordInfo return NULL");
		return;
	}
	
	CCommandObj* pObj  = tradeGrd->GetItem( byIndex );	
	if ( pObj )
	{
		ToLogService("common", "msgTrade Error");
	}
	else
	{
		CItemCommand* pObj = new CItemCommand( pItem );
		if( pGrid ) pObj->SetData( *pGrid );
		pObj->SetBoatHint( pBoat );
		if( !tradeGrd->SetItem( byIndex, pObj ) )
		{
			ToLogService("network", "SetItem return false");
			return;
		}
	}
}

void CTradeMgr::ShowCharTradeMoney(DWORD dwCharID, DWORD dwMoney)
{
	if( !frmPlayertrade )	return;
	if ( !labOtherGold )	return;
	if ( !labSelfGold )		return;

	char  szBuf[32] = { 0 };
	int  nCurMoney = dwMoney ;
	sprintf ( szBuf, "%s", StringSplitNum( nCurMoney ) );
	if ( dwCharID==m_dwMainID )
	{
		labSelfGold->SetCaption ( szBuf );
	}
	else
	{
		labOtherGold->SetCaption( szBuf );
	}
}


void CTradeMgr::ShowCharTradeIMP(DWORD dwCharID, DWORD dwMoney)
{
	if( !frmPlayertrade )	return;
	if ( !labOtherIMP )	return;
	if ( !labSelfIMP )		return;

	char  szBuf[32] = { 0 };
	int  nCurMoney = dwMoney ;
	sprintf ( szBuf, "%s", StringSplitNum( nCurMoney ) );
	if ( dwCharID==m_dwMainID )
	{
		labSelfIMP->SetCaption ( szBuf );
	}
	else
	{
		labOtherIMP->SetCaption( szBuf );
	}
}

void CTradeMgr::_evtLocalSaleEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stSale* p = (stSale*)pBox->pointer;
	//commented out to fix issue with stacked items, replaced with if statement.
	//CS_AddItem( g_stUITrade.m_bTradeType, p->dwSaleID, mission::TRADE_DRAGTO_TRADE, p->nGridID, p->nDragID, pBox->GetNumber() );

	if ( g_stUITrade.m_dwMainID==g_stUITrade.m_dwAcceptID )
		CS_AddItem( g_stUITrade.m_bTradeType, g_stUITrade.m_dwRequestID, mission::TRADE_DRAGTO_TRADE, p->nGridID, p->nDragID, pBox->GetNumber()  );
	else 
		CS_AddItem( g_stUITrade.m_bTradeType, g_stUITrade.m_dwAcceptID, mission::TRADE_DRAGTO_TRADE, p->nGridID, p->nDragID, pBox->GetNumber() );

	
	
}

void CTradeMgr::LocalSaleItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem )
{
	if( !pItem->GetIsValid() ) return;

	if( CItemCommand* pBoat = dynamic_cast<CItemCommand*>(pItem) )
	{
		if( pBoat && pBoat->GetItemInfo()->sType==43 )
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(782) );
		}
	}

	if( pItem->GetIsPile() && pItem->GetTotalNum()>1 )
	{
		stNumBox* pBox = g_stUIBox.ShowNumberBox( _evtLocalSaleEvent, pItem->GetTotalNum() );
		if( pBox )
		{
			pBox->pointer = (void*)&_sSale;
			//commented out to fix crash on drag stacked item, moved into _evtLocalSaleEvent
			//_sSale.dwSaleID = g_stUITrade.m_dwMainID==m_dwAcceptID ? m_dwRequestID : m_dwAcceptID;
			_sSale.nGridID = nGridID;
			_sSale.nDragID = pSelfGrid->GetDragIndex();
			return;
		}
	}

	if ( m_dwMainID==m_dwAcceptID )
		CS_AddItem( g_stUITrade.m_bTradeType, m_dwRequestID, mission::TRADE_DRAGTO_TRADE, nGridID, pSelfGrid->GetDragIndex(), pItem->GetTotalNum() );
	else 
		CS_AddItem( g_stUITrade.m_bTradeType, m_dwAcceptID, mission::TRADE_DRAGTO_TRADE, nGridID, pSelfGrid->GetDragIndex(), pItem->GetTotalNum() );
}

void CTradeMgr::LocalCancelItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem )
{
	if( !pItem->GetIsValid() ) return;	
	if ( m_dwMainID==m_dwAcceptID )
		CS_AddItem( g_stUITrade.m_bTradeType, m_dwRequestID, mission::TRADE_DRAGTO_ITEM, pSaleGrid->GetDragIndex(), nGridID, 0 );
	else
		CS_AddItem( g_stUITrade.m_bTradeType, m_dwAcceptID, mission::TRADE_DRAGTO_ITEM, pSaleGrid->GetDragIndex(), nGridID, 0 );		
}

void  CTradeMgr::ValidateTradeData( DWORD dwCharID )  // 
{
	if ( dwCharID==m_dwMainID ) 
	{
		btnYes->SetIsEnabled( false );
		btnGold->SetIsEnabled( false );
		btnIMP->SetIsEnabled( false );
	}
	else
	{
		labOtherGold->SetIsShow( true );
		GetPlayertradeBuyGrid()->SetIsShow( true );

		chkYes->SetIsChecked( true );
		if( !btnYes->GetIsEnabled() )
			btnTrade->SetIsEnabled(true);
	}
}

void  CTradeMgr::ValidateTrade( DWORD dwCharID )
{	
	if ( dwCharID==m_dwMainID )
	{
		btnYes->SetIsEnabled( false );
		btnGold->SetIsEnabled( false );
		btnIMP->SetIsEnabled( false );
	}
	else
	{
		chkTrade->SetIsChecked( true );
	}
}

void  CTradeMgr::ShowTradeSuccess()
{
	int nFree = g_stUIEquip.GetGoodsGrid()->GetMaxNum() - g_stUIEquip.GetGoodsGrid()->GetCurNum();
	if( nFree < g_stUITrade.grdBuy->GetCurNum() )
	{
		g_pGameApp->ShowMidText(g_oLangRec.GetString(783));
		g_pGameApp->SysInfo(g_oLangRec.GetString(783));
	}

	g_pGameApp->SysInfo(g_oLangRec.GetString(784));
	Clear();
}

void CTradeMgr::ShowTradeFailed()
{
	g_pGameApp->SysInfo(g_oLangRec.GetString(785));
	Clear();
}

void CTradeMgr::Clear()
{
	CCommandObj* pItem(NULL);
	for( int i = 0; i <grdRequest->GetMaxNum(); i++ )
	{
		pItem = grdRequest->GetItem( i);
		if (pItem && !pItem->GetIsValid())
		{
			pItem->SetIsValid(true);
		}
	}

	//
	if ( frmRequest ) frmRequest->Close();

	//
	if ( frmPlayertrade) frmPlayertrade->Close();

	//
	frmRequest = NULL;
	grdRequest = NULL;

	m_bTradeType = 0;
	m_dwAcceptID  = 0;
	m_dwRequestID = 0;	

	if( CDrag::GetDrag() )
	{
		CDrag::GetDrag()->Reset();
	}

    // 
	CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
	if(pWorldScene && pWorldScene->GetShipMgr())
	{
		pWorldScene->GetShipMgr()->CloseForm();
	}
}

void CTradeMgr::CancelCharTrade(  DWORD dwCharID )
{	
	if ( dwCharID==m_dwMainID )
		g_pGameApp->SysInfo(g_oLangRec.GetString(786));
	else
		g_pGameApp->SysInfo(g_oLangRec.GetString(787));

	Clear();
}

bool CTradeMgr::IsTrading()
{
	if( frmPlayertrade ) return frmPlayertrade->GetIsShow();
	return false;
}

void CTradeMgr::_evtSelfRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{

	if( !g_stUIBoat.GetHuman() ) return;

	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand || pItemCommand->GetItemInfo()->sType!=43 ) return;

	stNetItemInfo info;
	info.chType = mission::VIEW_CHARTRADE_SELF;
	info.sGridID = nGridID;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_INFO, &info );	
}

void CTradeMgr::_evtOtherRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{

	if( !g_stUIBoat.GetHuman() ) return;

	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand || pItemCommand->GetItemInfo()->sType!=43 ) return;

	stNetItemInfo info;
	info.chType = mission::VIEW_CHARTRADE_OTHER;
	info.sGridID = nGridID;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_INFO, &info );	
}


// add by Philip.Wu  2006-07-04  
void CTradeMgr::CloseAllForm()
{
	// 
	if(IsTrading())
	{
		::CS_CancelTrade( g_stUITrade.m_bTradeType, g_stUITrade.m_dwRequestID );
	}

	// 
	if(frmPlayertrade && frmPlayertrade->GetIsShow())
	{
		frmPlayertrade->SetIsShow(false);
	}

	if(frmRequest && frmRequest->GetIsShow())
	{
		frmRequest->SetIsShow(false);
	}
}

