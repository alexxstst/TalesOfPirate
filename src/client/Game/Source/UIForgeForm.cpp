#include "StdAfx.h"
#include "uiforgeform.h"
#include "uiformmgr.h"
#include "uiform.h"
#include "uilabel.h"
#include "uimemo.h"
#include "uitextbutton.h"
#include "scene.h"
#include "uiitemcommand.h"
#include "uifastcommand.h"
#include "forgerecord.h"
#include "gameapp.h"
#include "uigoodsgrid.h"
#include "uiequipform.h"
#include "packetcmd.h"
#include "character.h"
#include "UIBoxform.h"
#include "packetCmd.h"
#include "StoneSet.h"
#include "GameApp.h"
#include "UIProgressBar.h"
#include "WorldScene.h"
#include "UIList.h"
#include "StringLib.h"



using namespace GUI;

using namespace std;

static int         g_nForgeIndex  =-1;

//---------------------------------------------------------------------------
// class CForgeMgr
//---------------------------------------------------------------------------
bool CForgeMgr::Init()
{
	CFormMgr &mgr = CFormMgr::s_Mgr;
	//npc
	frmNPCforge  = mgr.Find("frmNPCforge" );
	if ( !frmNPCforge )
	{	
		LG("gui", g_oLangRec.GetString(560));
		return false;
	}
	
	frmNPCforge->evtEntrustMouseEvent = _MainMouseEvent;
	frmNPCforge->evtClose = _OnClose;
	
	labForgeGold = dynamic_cast<CLabelEx*> (frmNPCforge->Find("labForgeGold")) ;
	if( !labForgeGold ) 
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "labForgeGold");	
	labForgeGold->SetCaption("");

	// 
	char szBuf[32];
	for (int i(0); i<ITEM_NUM; i++)
	{
		sprintf(szBuf, "cmdForgeItem%d", i);
		cmdForgeItem[i] = dynamic_cast<COneCommand*>(frmNPCforge->Find(szBuf));
		if (!cmdForgeItem[i]) 
			return Error(g_oLangRec.GetString(561),
						 frmNPCforge->GetName(), 
						 szBuf);
	}
	cmdForgeItem[EQUIP]->evtBeforeAccept = _DragEvtEquip;
	cmdForgeItem[GEN_STONE]->evtBeforeAccept = _DragEvtGenStone;
	cmdForgeItem[FORGE_STONE]->evtBeforeAccept = _DragEvtForgStone;
	if( cmdForgeItem[EQUIP]->GetDrag() )
	{
		cmdForgeItem[EQUIP]->GetDrag()->evtMouseDragBegin = _DragBeforeEvt;
	}
	if( cmdForgeItem[GEN_STONE]->GetDrag() )
	{
		cmdForgeItem[GEN_STONE]->GetDrag()->evtMouseDragBegin = _DragBeforeEvt;
	}
	if( cmdForgeItem[FORGE_STONE]->GetDrag() )
	{
		cmdForgeItem[FORGE_STONE]->GetDrag()->evtMouseDragBegin = _DragBeforeEvt;
	}

	proNPCforge = dynamic_cast<CProgressBar*>(frmNPCforge->Find("proNPCforge"));
	if( !proNPCforge ) 
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "proNPCforge");
	proNPCforge->evtTimeArrive = _ProTimeArriveEvt;

	btnForgeYes = dynamic_cast<CTextButton*>(frmNPCforge->Find("btnForgeYes"));
	if( !btnForgeYes ) 
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "btnForgeYes");

	btnMillingYes = dynamic_cast<CTextButton*>(frmNPCforge->Find("btnMillingYes"));
	if( !btnMillingYes ) 
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "btnMillingYes");
	btnMillingYes->SetIsShow(false);

	lstForgeItemState = dynamic_cast<CList*>(frmNPCforge->Find("lstForgeItemState"));
	if (!lstForgeItemState)
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "lstForgeItemState");

	imgMillingTitle = dynamic_cast<CImage*>(frmNPCforge->Find("imgMillingTitle"));
	if (!imgMillingTitle)
		return Error(g_oLangRec.GetString(561), frmNPCforge->GetName(), "imgMillingTitle");

	btnYes = btnForgeYes;

	return true;
}

//---------------------------------------------------------------------------
void CForgeMgr::CloseForm()
{
}

//---------------------------------------------------------------------------
void CForgeMgr::SwitchMap()
{
	Clear();
}

//---------------------------------------------------------------------------
void CForgeMgr::ShowForge(bool bShow, bool isMilling)
{
	Clear();

	m_isMilling = isMilling;

	if (bShow)
	{
		frmNPCforge->SetPos(150, 150);
		frmNPCforge->Reset();
		frmNPCforge->Refresh();
		frmNPCforge->Show();

		//
		int x = frmNPCforge->GetX() + frmNPCforge->GetWidth();
		int y = frmNPCforge->GetY();
		g_stUIEquip.GetItemForm()->SetPos(x, y);
		g_stUIEquip.GetItemForm()->Refresh();

		if (!(m_isOldEquipFormShow = g_stUIEquip.GetItemForm()->GetIsShow()))
		{
			g_stUIEquip.GetItemForm()->Show();
		}

		//
		btnMillingYes->SetIsShow(m_isMilling);
		btnForgeYes->SetIsShow(!m_isMilling);
		btnYes = m_isMilling ? btnMillingYes : btnForgeYes;

		if (m_isMilling)
		{
			cmdForgeItem[EQUIP]->SetHint(g_oLangRec.GetString(562));
			cmdForgeItem[GEN_STONE]->SetHint(g_oLangRec.GetString(563));
			cmdForgeItem[FORGE_STONE]->SetHint(g_oLangRec.GetString(564));
			imgMillingTitle->SetIsShow(true);
		}
		else
		{
			cmdForgeItem[EQUIP]->SetHint(g_oLangRec.GetString(565));
			cmdForgeItem[GEN_STONE]->SetHint(g_oLangRec.GetString(566));
			cmdForgeItem[FORGE_STONE]->SetHint(g_oLangRec.GetString(567));
			imgMillingTitle->SetIsShow(false);
		}
	}
	else
	{
		frmNPCforge->Close();
		g_stUIEquip.GetItemForm()->SetIsShow(m_isOldEquipFormShow);
	}

	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::ShowConfirmDialog(long lMoney)
{
	char szBuf[255] = { 0 };
	sprintf( szBuf, g_oLangRec.GetString(568), lMoney );
	g_stUIBox.ShowSelectBox( _evtConfirmEvent, szBuf, true );

}

//---------------------------------------------------------------------------
int  CForgeMgr::GetForgeComIndex(COneCommand* oneCommand)
{
	for (int i(0); i<ITEM_NUM; i++)
		if (cmdForgeItem[i] == oneCommand)
			return i;
	return -1;
}

void CForgeMgr::DragToEquipGrid(int iIndex)
{
	PopItem(iIndex);
}

//---------------------------------------------------------------------------
void CForgeMgr::Clear()
{
	ClearUI();

	ClearItem();
}

//---------------------------------------------------------------------------
void CForgeMgr::ClearUI()
{
	labForgeGold->SetCaption("");
	proNPCforge->SetPosition(0);
	btnForgeYes->SetIsEnabled(false);
	btnMillingYes->SetIsEnabled(false);

}

//---------------------------------------------------------------------------
void CForgeMgr::ClearItem()
{
	for (int i(0); i<ITEM_NUM; ++i)
	{
		PopItem(i);
	}
}

//---------------------------------------------------------------------------
bool CForgeMgr::SendForgeProtocol()
{
	stNetItemForgeAsk kNetItemForgeAsk;
	
	kNetItemForgeAsk.chType = m_isMilling? MILLING_TYPE : FORGE_TYPE;	// 

	for (int i(0); i<ITEM_NUM; ++i)
	{
		kNetItemForgeAsk.SGroup[i].sCellNum = 1;		// 1
		kNetItemForgeAsk.SGroup[i].pCell = new SForgeCell::SCell[1];
		kNetItemForgeAsk.SGroup[i].pCell[0].sNum = 1;
		kNetItemForgeAsk.SGroup[i].pCell[0].sPosID = m_iForgeItemPos[i];
	}
	CS_ItemForgeAsk(true, &kNetItemForgeAsk);

	return true;

}

//---------------------------------------------------------------------------
void CForgeMgr::ForgeSuccess(long lChaID)
{
	if( !CGameApp::GetCurScene() ) return;

	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( lChaID );
	if( !pCha ) return;

	pCha->SelfEffect(FORGE_SUCCESS_EFF_ID);

	if( pCha->IsMainCha() )
		this->ShowForge(false);
}

//---------------------------------------------------------------------------
void CForgeMgr::ForgeFailed(long lChaID)
{
	if( !CGameApp::GetCurScene() ) return;

	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( lChaID );
	if( !pCha ) return;

	pCha->SelfEffect(FORGE_FAILED_EFF_ID);

	if( pCha->IsMainCha() )
		this->ShowForge(false);
}

void CForgeMgr::ForgeOther(long lChaID)
{
	this->ShowForge(false);
}

bool CForgeMgr::IsRunning()		
{ 
	return proNPCforge->IsRuning();	
}




//---------------------------------------------------------------------------
// Callback Function
//---------------------------------------------------------------------------
void CForgeMgr::_MainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if( name=="btnClose"  || name == "btnForgeNo" )  
	{ ///
		// 
		//OnClose
		//if (g_stUIForge.proNPCforge->IsRuning())
		//{
		//	g_stUIForge.proNPCforge->Start(0);
		//	CS_ItemForgeAnswer( false );
		//}
		//else
		//{
		//	g_stUIForge.ShowForge(false);
		//}
		return;
	}
	else if( name == g_stUIForge.btnYes->GetName() ) 
	{
		g_stUIForge.btnYes->SetIsEnabled(false);

		g_stUIForge.SendForgeProtocol();
	}
		
	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::_DragEvtEquip(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
{
	if (g_stUIForge.IsRunning())
		return;

	if (!g_stUIForge.IsValidDragSource())
		return;

	CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand) return;
	if( !(pItemCommand->GetIsValid())) return;


	if (g_stUIForge.IsEquip(*pItemCommand))
	{
		g_stUIForge.PushItem(EQUIP, *pItemCommand);
		g_stUIForge.SetForgeUI();
	}
	else
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(569));
	}
	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::_DragEvtGenStone(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
{
	if (g_stUIForge.IsRunning())
		return;

	if (!g_stUIForge.IsValidDragSource())
		return;

	CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand ) return;
	if( !(pItemCommand->GetIsValid())) return;

	if (g_stUIForge.m_isMilling)
	{ //
		if (g_stUIForge.IsMillingReinforce(*pItemCommand))
		{
			g_stUIForge.PushItem(GEN_STONE, *pItemCommand);
			g_stUIForge.SetForgeUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(570));
		}
	}
	else
	{ //
		if (g_stUIForge.IsGenStone(*pItemCommand))
		{
			g_stUIForge.PushItem(GEN_STONE, *pItemCommand);
			g_stUIForge.SetForgeUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(571));
		}
	}
	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::_DragEvtForgStone(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
{
	if (g_stUIForge.IsRunning())
		return;

	if (!g_stUIForge.IsValidDragSource())
		return;

	CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand) return;
	if (!(pItemCommand->GetIsValid())) return;

	if (g_stUIForge.m_isMilling)
	{ //
		if (g_stUIForge.IsMillingKatalyst(*pItemCommand))
		{
			g_stUIForge.PushItem(FORGE_STONE, *pItemCommand);
			g_stUIForge.SetForgeUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(572));
		}
	}
	else
	{ //
		if (g_stUIForge.IsForgStone(*pItemCommand))
		{
			g_stUIForge.PushItem(FORGE_STONE, *pItemCommand);
			g_stUIForge.SetForgeUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(573));
		}
	}
	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::_evtConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if (g_stUIForge.m_isConfirm = nMsgType==CForm::mrYes)
		g_stUIForge.proNPCforge->Start(FORGE_PRO_TIME);
	else
		CS_ItemForgeAnswer( false );

}

//---------------------------------------------------------------------------
void CForgeMgr::_OnClose(CForm* pForm, bool& IsClose)
{
	if (g_stUIForge.proNPCforge->IsRuning())
	{
		g_stUIForge.proNPCforge->Start(0);
		CS_ItemForgeAnswer( false );
	}
	g_stUIForge.Clear();

	CS_ItemForgeAsk(false);

}

void CForgeMgr::_ProTimeArriveEvt(CGuiData *pSender)
{
	CS_ItemForgeAnswer( g_stUIForge.m_isConfirm );
}

void CForgeMgr::_DragBeforeEvt(CGuiData *pSender, int x, int y, DWORD key)
{
	if ( g_stUIForge.IsRunning() )
		if( pSender->GetDrag() ) 
			pSender->GetDrag()->Reset();
}


//---------------------------------------------------------------------------
// Help Function
//---------------------------------------------------------------------------
bool CForgeMgr::IsEquip(CItemCommand& rItem)
{
	CItemRecord* pItemRecord = rItem.GetItemInfo();

	if (pItemRecord) 
	{
		short sType = pItemRecord->sType;
		//	Close by alfred.shi 20080912 
		if (sType < EQUIP_TYPE && sType != 12 && sType != 13 && sType != 17 && sType != 18 && sType != 19 /*&& sType != 20*/ && sType != 21 || sType == 88)
			return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool CForgeMgr::IsGenStone(CItemCommand& rItem)
{
	CItemRecord* pItemRecord = rItem.GetItemInfo();

	if (pItemRecord) 
		return pItemRecord->sType == GEN_STONE_TYPE;

		return false;
}

//---------------------------------------------------------------------------
bool CForgeMgr::IsForgStone(CItemCommand& rItem)
{
	CItemRecord* pItemRecord = rItem.GetItemInfo();

	if (pItemRecord) 
		return pItemRecord->sType == FORGE_STONE_TYPE;

		return false;
}
//---------------------------------------------------------------------------
bool CForgeMgr::IsMillingReinforce(CItemCommand& rItem)
{
	CItemRecord* pItemRecord = rItem.GetItemInfo();

	if (pItemRecord) 
		return pItemRecord->sType == MILLING_Reinforce_TYPE;

	return false;

}
//---------------------------------------------------------------------------
bool CForgeMgr::IsMillingKatalyst(CItemCommand& rItem)
{
	CItemRecord* pItemRecord = rItem.GetItemInfo();

	if (pItemRecord) 
		return pItemRecord->sType == MILLING_Katalyst_TYPE;

	return false;

}
//---------------------------------------------------------------------------


bool CForgeMgr::IsValidDragSource()
{
	CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if( pGood == g_stUIEquip.GetGoodsGrid() ) return true;

	return false;
}


//---------------------------------------------------------------------------
void CForgeMgr::PushItem(int iIndex, CItemCommand& rItem)
{
	// CmdItem
	CItemCommand* pItemCommand =  
		dynamic_cast<CItemCommand*>(cmdForgeItem[iIndex]->GetCommand());
	if (pItemCommand)
	{
		PopItem(iIndex);
	}

	// Item
	m_iForgeItemPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
	// Item
	rItem.SetIsValid(false);

	// ItemCmdnewPopItem()
	CItemCommand* pItemCmd = new CItemCommand(rItem);
	pItemCmd->SetIsValid(true);
	cmdForgeItem[iIndex]->AddCommand(pItemCmd);

	return;
}

//---------------------------------------------------------------------------
void CForgeMgr::PopItem(int iIndex)
{
	// CmdItemItemPushItem()new
	CItemCommand* pItemCommand =  
		dynamic_cast<CItemCommand*>(cmdForgeItem[iIndex]->GetCommand());
	if (pItemCommand)
		cmdForgeItem[iIndex]->DelCommand();	// delete Item

	// Item
	CCommandObj* pItem = 
				g_stUIEquip.GetGoodsGrid()->GetItem(m_iForgeItemPos[iIndex]);
	if (pItem)
	{
		pItem->SetIsValid(true);
	}

	// Item
	m_iForgeItemPos[iIndex] = NO_USE;

	this->SetForgeUI();

	return;
}



//---------------------------------------------------------------------------
void CForgeMgr::SetForgeUI()
{

	/*
	      +5
	      3             
	               
	              
	    +18    +79    +99
	      
	     +2      +1 
	*/

	char szBuf[64];
	if (cmdForgeItem[EQUIP]->GetCommand())
	{
		// 
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdForgeItem[EQUIP]->GetCommand());
		if (!pItemCommand)
			return;
		CItemRecord* pItemRecord = pItemCommand->GetItemInfo();
		if (!pItemRecord)
			return;

		SItemForge rItemForgeInfo = pItemCommand->GetForgeInfo();

		string sEquipState("");
		// 
		CItemRow* pItem = lstForgeItemState->GetItems()->GetItem(0);
		if (pItem)
		{
			sEquipState = pItemRecord->szName;
			sEquipState += "  +";
			sEquipState += _itoa(rItemForgeInfo.nLevel,szBuf, 10);
			pItem->GetBegin()->SetString(sEquipState.c_str());
		}

		// 
		pItem = lstForgeItemState->GetItems()->GetItem(1);
		if (pItem)
		{
			sEquipState = g_oLangRec.GetString(835);
			if (rItemForgeInfo.nHoleNum < 1)
				sEquipState += "--  --\n";
			else
			{
				if (rItemForgeInfo.nStoneNum < 1)
				{
					sEquipState += g_oLangRec.GetString(836);
				}
				else
				{
					sEquipState += _itoa(rItemForgeInfo.nStoneLevel[0], szBuf, 10);
					sEquipState += g_oLangRec.GetString(837);
					sEquipState += rItemForgeInfo.pStoneInfo[0]->szDataName;
					sEquipState += "\n";
				}
			}
			pItem->GetBegin()->SetString(sEquipState.c_str());
		}

		// 
		pItem = lstForgeItemState->GetItems()->GetItem(2);
		if (pItem)
		{
			sEquipState = g_oLangRec.GetString(838);
			if (rItemForgeInfo.nHoleNum < 2)
				sEquipState += "--  --\n";
			else
			{
				if (rItemForgeInfo.nStoneNum < 2)
				{
					sEquipState += g_oLangRec.GetString(836);
				}
				else
				{
					sEquipState += _itoa(rItemForgeInfo.nStoneLevel[1], szBuf, 10);
					sEquipState += g_oLangRec.GetString(837);
					sEquipState += rItemForgeInfo.pStoneInfo[1]->szDataName;
					sEquipState += "\n";
				}
			}
			pItem->GetBegin()->SetString(sEquipState.c_str());
		}

		// 
		pItem = lstForgeItemState->GetItems()->GetItem(3);
		if (pItem)
		{
			sEquipState = g_oLangRec.GetString(839);
			if (rItemForgeInfo.nHoleNum < 3)
				sEquipState += "--  --\n";
			else
			{
				if (rItemForgeInfo.nStoneNum < 3)
				{
					sEquipState += g_oLangRec.GetString(836);
				}
				else
				{
					sEquipState += _itoa(rItemForgeInfo.nStoneLevel[2], szBuf, 10);
					sEquipState += g_oLangRec.GetString(837);
					sEquipState += rItemForgeInfo.pStoneInfo[2]->szDataName;
					sEquipState += "\n";
				}
			}
			pItem->GetBegin()->SetString(sEquipState.c_str());
		}

		// 
		pItem = lstForgeItemState->GetItems()->GetItem(4);
		if (pItem)
		{
			sEquipState = g_oLangRec.GetString(840);
			for (int i(0); i<rItemForgeInfo.nStoneNum; ++i)
			{
				sEquipState += rItemForgeInfo.szStoneHint[i];	
				sEquipState += "  ";
			}
			if (sEquipState == g_oLangRec.GetString(840))
				sEquipState += "--";

			pItem->GetBegin()->SetString(sEquipState.c_str());
		}


		// 
		pItem = lstForgeItemState->GetItems()->GetItem(5);
		if (pItem)
		{
			string sEquipState = g_oLangRec.GetString(841);

			pItem->GetBegin()->SetString(sEquipState.c_str());
		}
	}
	else
	{
		CItemRow* pItem;
		for (int i(0); i<6; ++i)
		{
			pItem = lstForgeItemState->GetItems()->GetItem(i);
			if (pItem)
				pItem->GetBegin()->SetString("");
		}

	}

	//		     +2      +1 
	if (!m_isMilling)
	{
		if (cmdForgeItem[GEN_STONE]->GetCommand())
		{
			CItemCommand* pItemCommand =  
				dynamic_cast<CItemCommand*>(cmdForgeItem[GEN_STONE]->GetCommand());
			if (!pItemCommand)
				return;

			CItemRow* pItem;
			pItem = lstForgeItemState->GetItems()->GetItem(6);
			if (pItem)
			{
				string sEquipState = g_oLangRec.GetString(842) + pItemCommand->GetStoneHint(1);
				pItem->GetBegin()->SetString(sEquipState.c_str());
			}
		}
		else
		{
			CItemRow* pItem;
			pItem = lstForgeItemState->GetItems()->GetItem(6);
			if (pItem)
			{
				pItem->GetBegin()->SetString("");
			}
		}



	}
		// UI

	if (cmdForgeItem[EQUIP]->GetCommand() 
		&& cmdForgeItem[GEN_STONE]->GetCommand() 
		&& cmdForgeItem[FORGE_STONE]->GetCommand())
	{
		labForgeGold->SetCaption(StringSplitNum(m_isMilling ? CalMillingMoney() : CalForgeMoney()));
		assert(btnYes);
		btnYes->SetIsEnabled(true);
	}
	else
	{
		labForgeGold->SetCaption("");
		assert(btnYes);
		btnYes->SetIsEnabled(false);
	}


}

//---------------------------------------------------------------------------
long CForgeMgr::CalForgeMoney()
{
	CItemCommand* pItemCommand =  
				dynamic_cast<CItemCommand*>(cmdForgeItem[EQUIP]->GetCommand());

	SItemForge rItemForgeInfo = pItemCommand->GetForgeInfo();
	if (rItemForgeInfo.IsForge)
	{
		return FORGE_PER_LEVEL_MONEY * (rItemForgeInfo.nLevel + 1);
		//	add by alfred.shi 20080804 begin
		//if(rItemForgeInfo.nLevel < 1)
		//	return ((long)( FORGE_PER_LEVEL_MONEY/10));
		//else
		//	return ((long)((rItemForgeInfo.nLevel + 1)) * FORGE_PER_LEVEL_MONEY);
		//	end
		//return ((long)((rItemForgeInfo.nLevel + 1)) * FORGE_PER_LEVEL_MONEY);
	}
	return 0;

}
//---------------------------------------------------------------------------
long CForgeMgr::CalMillingMoney()
{
	CItemCommand* pItemCommand =  
		dynamic_cast<CItemCommand*>(cmdForgeItem[EQUIP]->GetCommand());

	SItemForge rItemForgeInfo = pItemCommand->GetForgeInfo();
	return ((long)((rItemForgeInfo.nHoleNum + 1)) * MILLING_PER_LEVEL_MONEY);
}


