#include "StdAfx.h"
#include "UIGlobalVar.h"
#include "uitextparse.h"
#include "uiEditor.h"
#include "UIFormMgr.h"
#include "uiheadsay.h"
#include "UITeam.h"
#include "uitreeview.h"
#include "UIChat.h"
#include "uiboxform.h"
#include "uitradeform.h"
#include "uiequipform.h"
#include "uiminimapform.h"
#include "uisystemform.h"
#include "uimissionform.h"
#include "UIMisLogForm.h"
#include "uiforgeform.h"
#include "uinpctradeform.h"
#include "uistartform.h"
#include "uistateform.h"
#include "uiCozeform.h"
#include "uifastcommand.h"
#include "gameapp.h"
#include "scene.h"
#include "uigoodsgrid.h"
#include "uinpctalkform.h"
#include "worldscene.h"
#include "uiguildlist.h"
#include "UIGuildMgr.h"
#include "uiguildapply.h"
#include "uiboatform.h"
#include "uibourseform.h"
#include "UIBankForm.h"
#include "UIGuildBankForm.h"
#include "UIBoothForm.h"
#include "UIHaircutForm.h"
#include "UIPKDialog.h"
#include "UIMakeEquipForm.h"
#include "UIGuildChallengeForm.h"
#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"
#include "uiCozeform.h"
#include "UISpiritForm.h"
#include "UIPurifyForm.h"
#include "UIPKSilverForm.h"
#include "UIFindTeamForm.h"
#include "UIComposeForm.h"
#include "UIBreakForm.h"
#include "UIFoundForm.h"
#include "UICookingForm.h"
#include "UIMailForm.h"
#include "UINumAnswer.h"
#include "UIChurchChallenge.h"

using namespace std;

namespace GUI {
	CTextParse g_TextParse;
}

using namespace GUI;

bool UIMainInit(CFormMgr* pSender) {
	if (!CHeadSay::Init()) {
		ToLogService("common", "CHeadSay::Init failed");
		return false;
	}

	return CUIInterface::All_Init();
}

bool UIClear() {
	if (!CHeadSay::Clear()) {
		ToLogService("common", "CHeadSay::Clear failed");
		return false;
	}

	CUIInterface::All_End();
	return true;
}

//---------------------------------------------------------------------------
// calss CUIInterface
//---------------------------------------------------------------------------
CUIInterface::allmgr CUIInterface::all(0);

CEditor g_stUIEditor;
CChat g_stUIChat;
CBoxMgr g_stUIBox;
CTradeMgr g_stUITrade;
CEquipMgr g_stUIEquip;
CMiniMapMgr g_stUIMap;
CSystemMgr g_stUISystem;
CMissionMgr g_stUIMission;
CMisLogForm g_stUIMisLog;
CForgeMgr g_stUIForge;

CNpcTradeMgr g_stUINpcTrade;
CStartMgr g_stUIStart;
CStateMgr g_stUIState;
CNpcTalkMgr g_stUINpcTalk;
CUIGuildList g_stUIGuildList;
CUIGuildMgr g_stUIGuildMgr;
CUIGuildApply g_stUIGuildApply;
CBoatMgr g_stUIBoat;
CBourseMgr g_stUIBourse;
CBankMgr g_stUIBank;
CGuildBankMgr g_stUIGuildBank;
CBoothMgr g_stUIBooth;
CHaircutMgr g_stUIHaircut;
CPkDialog g_stUIPKDialog;
CMakeEquipMgr g_stUIMakeEquip;
CGuildChallengeMgr g_stGuildChallenge;
CDoublePwdMgr g_stUIDoublePwd;
CStoreMgr g_stUIStore;
CSpiritMgr g_stUISpirit;
CPurifyMgr g_stUIPurify;
CBlackTradeMgr g_stUIBlackTrade;
CPKSilverMgr g_stUIPKSilver;
CFindTeamMgr g_stUIFindTeam;
CComposeMgr g_stUICompose;
CBreakMgr g_stUIBreak;
CFoundMgr g_stUIFound;
CCookingMgr g_stUICooking;
CMailMgr g_stUIMail;
CNumAnswerMgr g_stUINumAnswer;
CChurchChallengeMgr g_stChurchChallenge;

CCozeForm g_stUICozeForm;
CChannelSwitchForm g_stUIChannelSwitch;

bool CUIInterface::_IsAllInit = false;

CUIInterface::CUIInterface() {
	all.push_back(this);
}

bool CUIInterface::All_Init() {
	int size = all.size();

	for (allmgr::iterator it = all.begin(); it != all.end(); it++) {
		CUIInterface* pUI = dynamic_cast<CUIInterface*>(*it);
		if (!pUI->Init()) {
			const type_info& info = typeid(**it);
			ToLogService("common", "msg[{}] Init failed", info.name());
			return false;
		}
	}

	CFormMgr& mgr = CFormMgr::s_Mgr;
	mgr.AddKeyCharEvent(_evtESCKey);
	_IsAllInit = true;
	return true;
}

void CUIInterface::All_End() {
	if (!_IsAllInit) return;

	for (allmgr::iterator it = all.begin(); it != all.end(); it++) {
		(*it)->End();
	}
}

void CUIInterface::All_FrameMove(DWORD dwTime) {
	for (allmgr::iterator it = all.begin(); it != all.end(); it++) {
		(*it)->FrameMove(dwTime);
	}
}

void CUIInterface::All_LoadingCall() {
	for (allmgr::iterator it = all.begin(); it != all.end(); it++) {
		(*it)->LoadingCall();
	}
}

void CUIInterface::All_SwitchMap() {
	for (allmgr::iterator it = all.begin(); it != all.end(); it++) {
		(*it)->SwitchMap();
	}
}

void CUIInterface::MainChaMove() {
	for (allmgr::iterator it = all.begin(); it != all.end(); it++)
		(*it)->CloseForm();
}

CForm* CUIInterface::_FindForm(const char* frmName) {
	CForm* form = CFormMgr::s_Mgr.Find(frmName);
	if (!form) {
		g_logManager.InternalLog(LogLevel::Debug, "ui", SafeVFormat(GetLanguageString(464), frmName));
	}
	return form;
}

void CUIInterface::_evtDragToGoodsEvent(CGuiData* pSender, CCommandObj* pItem, int nGridID, bool& isAccept) {
	isAccept = false;

	if (!CGameScene::GetMainCha()) return;

	if (!CGameApp::GetCurScene()) return;

	CGoodsGrid* pSelf = dynamic_cast<CGoodsGrid*>(pSender);
	if (!pSelf) return;

	COneCommand* pCom = dynamic_cast<COneCommand*>(CDrag::GetParent());
	if (pCom) {
		int iIndex = -1;
		if (g_stUIEquip.IsEquipCom(pCom) && pSelf == g_stUIEquip.GetGoodsGrid()) {
			// 
			g_stUIEquip.UnfixToGrid(pCom->GetCommand(), nGridID, pCom->nTag);
			return;
		}

		/*  */
		// 
		iIndex = g_stUIForge.GetForgeComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != iIndex) {
			g_stUIForge.DragToEquipGrid(iIndex);
			return;
		}

		/*  */
		// 
		if (pSelf == g_stUIEquip.GetGoodsGrid() && g_stUIMakeEquip.IsAllCommand(pCom)) {
			if (g_stUIMakeEquip.IsRouleauCommand(pCom))
				g_stUIMakeEquip.DragRouleauToEquipGrid();
			else {
				int iIndex = g_stUIMakeEquip.GetItemComIndex(pCom);
				if (iIndex != -1)
					g_stUIMakeEquip.DragItemToEquipGrid(iIndex);
			}
			return;
		}

		// 
		// 
		int nPurifyIndex = g_stUIPurify.GetItemComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != nPurifyIndex) {
			g_stUIPurify.DragItemToEquipGrid(nPurifyIndex);
		}

		//  
		iIndex = g_stUICompose.GetComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != iIndex) {
			if (1 == iIndex) {
				g_stUICompose.ClearCommand();
			}
			else {
				g_stUICompose.DragToEquipGrid(iIndex);
			}
			return;
		}

		//  
		iIndex = g_stUIFound.GetComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != iIndex) {
			if (1 == iIndex) {
				g_stUIFound.ClearCommand();
			}
			else {
				g_stUIFound.DragToEquipGrid(iIndex);
			}
			return;
		}

		//  
		iIndex = g_stUICooking.GetComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != iIndex) {
			if (1 == iIndex) {
				g_stUICooking.ClearCommand();
			}
			else {
				g_stUICooking.DragToEquipGrid(iIndex);
			}
			return;
		}

		//  
		iIndex = g_stUIBreak.GetComIndex(pCom);
		if (pSelf == g_stUIEquip.GetGoodsGrid() && -1 != iIndex) {
			g_stUIBreak.DragToEquipGrid(iIndex);
			return;
		}

		return;
	}

	CGoodsGrid* pDrag = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (!pDrag) return;

	// NPC
	if (pSelf == g_stUIEquip.GetGoodsGrid()
		&& g_stUINpcTrade.IsNpcGoods(pDrag)) {
		g_stUINpcTrade.LocalBuyFromNpc(pDrag, pSelf, nGridID, pItem);
		return;
	}

	// NPC
	if (pDrag == g_stUIEquip.GetGoodsGrid()
		&& g_stUINpcTrade.IsNpcGoods(pSelf)) {
		g_stUINpcTrade.LocalSaleToNpc(pSelf, pDrag, nGridID, pItem);
		return;
	}

	//  Michael Chen (2005-05-27)
	if (pDrag == g_stUIBourse.GetBuyGoodsGrid() &&
		pSelf == g_stUIBourse.GetShipRoomGoodsGrid()) {
		g_stUIBourse.BuyItem(*pSelf, *pItem, nGridID);
		return;
	}

	// :
	if (pDrag == g_stUITrade.GetRequestGrid() &&
		pSelf == g_stUITrade.GetPlayertradeSaleGrid()) {
		g_stUITrade.LocalSaleItem(pSelf, pDrag, nGridID, pItem);
		return;
	}

	// :
	if (pSelf == g_stUITrade.GetRequestGrid()
		&& pDrag == g_stUITrade.GetPlayertradeSaleGrid()) {
		g_stUITrade.LocalCancelItem(pDrag, pSelf, nGridID, pItem);
		return;
	}

	// 
	if (pSelf == g_stUIEquip.GetGoodsGrid()
		&& pDrag == g_stUIBank.GetBankGoodsGrid()) {
		g_stUIBank.PopFromBank(*pDrag, *pSelf, nGridID, *pItem);
	}

	// 
	if (pSelf == g_stUIBank.GetBankGoodsGrid()
		&& pDrag == g_stUIEquip.GetGoodsGrid()) {
		g_stUIBank.PushToBank(*pDrag, *pSelf, nGridID, *pItem);
	}

	if (pSelf == g_stUIEquip.GetGoodsGrid()
		&& pDrag == g_stUIGuildBank.GetBankGoodsGrid()) {
		g_stUIGuildBank.PopFromBank(*pDrag, *pSelf, nGridID, *pItem);
	}

	// 
	if (pSelf == g_stUIGuildBank.GetBankGoodsGrid()
		&& pDrag == g_stUIEquip.GetGoodsGrid()) {
		g_stUIGuildBank.PushToBank(*pDrag, *pSelf, nGridID, *pItem);
	}

	/*  */
	// 
	if (pSelf == g_stUIEquip.GetGoodsGrid()
		&& pDrag == g_stUIBooth.GetBoothItemsGrid()) {
		g_stUIBooth.PopFromBooth(*pDrag, *pSelf, nGridID, *pItem);
	}

	// 
	if (pSelf == g_stUIBooth.GetBoothItemsGrid()
		&& pDrag == g_stUIEquip.GetGoodsGrid()) {
		g_stUIBooth.PushToBooth(*pDrag, *pSelf, nGridID, *pItem);
	}

	// 
	if (pSelf == g_stUIEquip.GetGoodsGrid() &&
		pDrag == g_stUIStore.GetTempKitbagGrid()) {
		g_stUIStore.PopFromTempKitbag(*pDrag, *pSelf, nGridID, *pItem);
	}

	// 
	if (pSelf == g_stUIBlackTrade.GetBuyGoodsGrid() &&
		pDrag == g_stUIBlackTrade.GetSaleGoodsGrid()) {
		g_stUIBlackTrade.SailToBuy(*pDrag, *pSelf, nGridID, *pItem);
	}
}

bool CUIInterface::_evtESCKey(char& key) {
	if (key == VK_ESCAPE) {
		if (g_pGameApp->IsCtrlPress()) return false; // Ctrl+[

		if (!dynamic_cast<CWorldScene*>(CGameApp::GetCurScene())) return false;

		g_stUIStart.CloseForm();

		if (g_stUITrade.IsTrading()) return false;

		CForm* frm = CFormMgr::s_Mgr.FindESCForm();
		if (frm) {
			GuiFormEscCloseEvent evtEscClose = frm->evtEscClose;
			if (evtEscClose) {
				evtEscClose(frm);
			}
			else {
				frm->Close();
			}
			return true;
		}

		if (CFormMgr::s_Mgr.ModalFormNum() == 0 && CFormMgr::s_Mgr.GetEnableHotKey()) {
			frm = g_stUISystem.GetSystemForm();
			if (frm) {
				frm->Show();
				return true;
			}
		}
	}
	return false;
}

bool CUIInterface::Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
	string strFormat = strInfo;
	if (strFormat.substr(0, 3) != "msg")
		strFormat = "msg" + strFormat;

	{
		char _buf[512];
		snprintf(_buf, sizeof(_buf), strFormat.c_str(), strFormName, strCompentName);
		g_logManager.InternalLog(LogLevel::Debug, "common", _buf);
	}
	return false;
}
