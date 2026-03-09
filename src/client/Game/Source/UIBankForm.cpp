#include "StdAfx.h"

#include "UIBankForm.h"
#include "uiform.h"
#include "uilabel.h"
#include "uiformmgr.h"
#include "uigoodsgrid.h"
#include "NetProtocol.h"
#include "uiboxform.h"
#include "uiEquipForm.h"
#include "UIGoodsGrid.h"
#include "uiItemCommand.h"
#include "uiform.h"
#include "uiBoatForm.h"
#include "packetcmd.h"
#include "Character.h"
#include "GameApp.h"

namespace GUI
{
	//=======================================================================
	//	CBankMgr 's Members
	//=======================================================================

	bool CBankMgr::Init()  //
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		frmBank = mgr.Find("frmNPCstorage");// NPC 
		if ( !frmBank)
		{
			ToLogService("gui", "{}", g_oLangRec.GetString(438));
			return false;
		}
		frmBank->evtClose = _evtOnClose; 

		grdBank = dynamic_cast<CGoodsGrid*>(frmBank->Find("grdNPCstorage"));
		if (!grdBank) 
			return Error(g_oLangRec.GetString(439),
						 frmBank->GetName(), "grdNPCstorage");
		grdBank->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;//   CUIInterface::_evtDragToGoodsEvent
		grdBank->evtSwapItem = _evtBankToBank;
		labCharName = dynamic_cast<CLabel*>(frmBank->Find("labOwnerName"));
		if (!grdBank) 
			return Error(g_oLangRec.GetString(439),
						 frmBank->GetName(), "labOwnerName");

		return true;
	}

	void CBankMgr::_evtOnClose( CForm* pForm, bool& IsClose )  // 
	{
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_CLOSE_BANK, NULL); 

		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_BANK, true);		// 
	}


	//-------------------------------------------------------------------------
	void CBankMgr::ShowBank() // 
	{
		// 

		if (!g_stUIBoat.GetHuman()) //  
			return;

		char szBuf[32];
		sprintf(szBuf, "%s%s", g_stUIBoat.GetHuman()->getName(), g_oLangRec.GetString(440));//
		labCharName->SetCaption(szBuf);//

		frmBank->Show();

		// 
		if (!g_stUIEquip.GetItemForm()->GetIsShow())
		{
			int nLeft, nTop;
			nLeft = frmBank->GetX2();
			nTop = frmBank->GetY();

			g_stUIEquip.GetItemForm()->SetPos(nLeft, nTop); //
			g_stUIEquip.GetItemForm()->Refresh(); //		
			g_stUIEquip.GetItemForm()->Show();  //
		}

		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_BANK, false);		// 
	}

	//-------------------------------------------------------------------------
	bool CBankMgr::PushToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
#define EQUIP_TYPE 0
#define BANK_TYPE 1

		// 
		m_kNetBank.chSrcType = EQUIP_TYPE;
		m_kNetBank.sSrcID = rkDrag.GetDragIndex();
		//m_kNetBank.sSrcNum = ; 
		m_kNetBank.chTarType = BANK_TYPE;
		m_kNetBank.sTarID = nGridID;

		// 
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;
		CItemRecord* pkItemRecord = pkItemCmd->GetItemInfo();
		if (!pkItemRecord)	return false;

		//if(pkItemRecord->sType == 59 && m_kNetBank.sSrcID == 1)
		//{
		//	g_pGameApp->MsgBox("\n");
		//	return false;	// 
		//}

		// if(pkItemRecord->lID == 2520 || pkItemRecord->lID == 2521)
		if( pkItemRecord->lID == 2520 || pkItemRecord->lID == 2521 || pkItemRecord->lID == 6341 || pkItemRecord->lID == 6343
		 || pkItemRecord->lID == 6347 || pkItemRecord->lID == 6359 || pkItemRecord->lID == 6370 || pkItemRecord->lID == 6371
		 || pkItemRecord->lID == 6373 || pkItemRecord->lID >= 6376 && pkItemRecord->lID <= 6378
		 || pkItemRecord->lID >= 6383 && pkItemRecord->lID <= 6385 )// modify by ning.yan 20080820 
		{
			//g_pGameApp->MsgBox(g_oLangRec.GetString(958));	// ""
			g_pGameApp->MsgBox(g_oLangRec.GetString(958));	// ""
			return false;
		}
		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{	/**/
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox(_MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(441), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{	/**/
			g_stUIBank.m_kNetBank.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
			return true;
			//char buf[256] = { 0 };
			//sprintf(buf, "\n[%s]?", pkItemCmd->GetName());
			//g_stUIBox.ShowSelectBox(_MoveAItemEvent, buf, true);
			//return true;
		}
	}

	//-------------------------------------------------------------------------
	bool CBankMgr::PopFromBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		// 
		m_kNetBank.chSrcType = BANK_TYPE ;
		m_kNetBank.sSrcID = rkDrag.GetDragIndex();
		//m_kNetBank.sSrcNum = ; 
		m_kNetBank.chTarType = EQUIP_TYPE;
		m_kNetBank.sTarID = nGridID;


		// 
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;

		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{	/**/
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox( _MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(442), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{	/**/
			g_stUIBank.m_kNetBank.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
			return true;

			//char buf[256] = { 0 };
			//sprintf(buf, "\n[%s]?", pkItemCmd->GetName());
			//g_stUIBox.ShowSelectBox(_MoveAItemEvent, buf, true);
			//return true;
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey) // 
	{
		if(nMsgType != CForm::mrYes)  // 
			return;


		int num =  g_stUIBank.m_pkNumberBox->GetNumber();//  
		if ( num > 0 )
		{
			g_stUIBank.m_kNetBank.sSrcNum = num;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_MoveAItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey) // 
	{
		if(nMsgType != CForm::mrYes) 
			return;

		g_stUIBank.m_kNetBank.sSrcNum = 1;
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));//
	}
	
	//-------------------------------------------------------------------------
	void CBankMgr::CloseForm()  //  
	{
		if (frmBank->GetIsShow())
		{
			frmBank->Close();
			g_stUIEquip.GetItemForm()->Close();
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_evtBankToBank(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap) // 
	{
		isSwap = false;
		if( !g_stUIBoat.GetHuman() ) return;

		g_stUIBank.m_kNetBank.chSrcType = BANK_TYPE ;
		g_stUIBank.m_kNetBank.sSrcID = nSecond;
		g_stUIBank.m_kNetBank.sSrcNum = 0; 
		g_stUIBank.m_kNetBank.chTarType = BANK_TYPE;
		g_stUIBank.m_kNetBank.sTarID = nFirst;

		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
	}


}	// end of namespace GUI
