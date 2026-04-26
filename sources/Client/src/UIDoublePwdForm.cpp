#include "StdAfx.h"
#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"
#include "UIEquipForm.h"

#include "GameApp.h"
#include "PacketCmd.h"
#include "UIItemCommand.h"// ning.yan 2008-11-11

using namespace std;

namespace GUI {
	CDoublePwdMgr::CDoublePwdMgr() {
	}


	// 
	bool CDoublePwdMgr::Init() {
		CFormMgr& mgr = CFormMgr::s_Mgr;

		// 
		frmDoublePwdCreate = mgr.Find("frmDoublePwdCreate");
		if (!frmDoublePwdCreate) return false;

		edtDoublePwdCreate = dynamic_cast<CEdit*>(frmDoublePwdCreate->Find("edtDoublePwdCreate"));
		if (!edtDoublePwdCreate) return false;

		edtDoublePwdCreateRetry = dynamic_cast<CEdit*>(frmDoublePwdCreate->Find("edtDoublePwdCreateRetry"));
		if (!edtDoublePwdCreateRetry) return false;

		frmDoublePwdCreate->evtEntrustMouseEvent = _evtCreateFromMouseEvent;
		frmDoublePwdCreate->evtClose = _evtFormClose;
		edtDoublePwdCreate->evtActive = _evtEditFocus;
		edtDoublePwdCreateRetry->evtActive = _evtEditFocus;

		edtDoublePwdCreate->SetIsPassWord(true);
		edtDoublePwdCreateRetry->SetIsPassWord(true);


		// 
		frmDoublePwdAlter = mgr.Find("frmDoublePwdAlter");
		if (!frmDoublePwdAlter) return false;

		edtDoublePwdAlterOld = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterOld"));
		if (!edtDoublePwdAlterOld) return false;

		edtDoublePwdAlterNew = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterNew"));
		if (!edtDoublePwdAlterNew) return false;

		edtDoublePwdAlterNewRetry = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterNewRetry"));
		if (!edtDoublePwdAlterNewRetry) return false;

		frmDoublePwdAlter->evtEntrustMouseEvent = _evtAlterFromMouseEvent;
		frmDoublePwdAlter->evtClose = _evtFormClose;
		edtDoublePwdAlterOld->evtActive = _evtEditFocus;
		edtDoublePwdAlterNew->evtActive = _evtEditFocus;
		edtDoublePwdAlterNewRetry->evtActive = _evtEditFocus;

		edtDoublePwdAlterOld->SetIsPassWord(true);
		edtDoublePwdAlterNew->SetIsPassWord(true);
		edtDoublePwdAlterNewRetry->SetIsPassWord(true);


		// 
		frmDoublePwd = mgr.Find("frmDoublePwd");
		if (!frmDoublePwd) return false;

		edtDoublePwd = dynamic_cast<CEdit*>(frmDoublePwd->Find("edtDoublePwd"));
		if (!edtDoublePwd) return false;

		frmDoublePwd->evtEntrustMouseEvent = _evtDoublePwdFromMouseEvent;
		frmDoublePwd->evtClose = _evtFormClose;
		edtDoublePwd->evtActive = _evtEditFocus;
		edtDoublePwd->SetIsPassWord(true);


		// 
		frmDoublePwdInput = mgr.Find("frmDoublePwdInput");
		if (!frmDoublePwdInput) return false;

		frmDoublePwdInput->evtEntrustMouseEvent = _evtInputFromMouseEvent;
		frmDoublePwdInput->evtClose = _evtFormClose;


		// 
		frmDoublePwdInfo = mgr.Find("frmDoublePwdInfo");
		if (!frmDoublePwdInfo) return false;

		return true;
	}


	// 
	void CDoublePwdMgr::CloseForm() {
		CloseAllForm();
	}


	// 
	bool CDoublePwdMgr::IsPwdValid(const char* szStr) {
		if (!szStr)
			return false;

		for (int i = 0; szStr[i]; ++i) {
			if ('0' > szStr[i] || szStr[i] > '9')
				return false;
		}

		return true;
	}


	// 
	void CDoublePwdMgr::ShowCreateForm() {
		CloseAllForm();

		if (frmDoublePwdCreate && !frmDoublePwdCreate->GetIsShow()) {
			edtDoublePwdCreate->SetCaption("");
			edtDoublePwdCreateRetry->SetCaption("");

			frmDoublePwdCreate->Show();
		}

		if (frmDoublePwdInfo) {
			frmDoublePwdInfo->SetPos(frmDoublePwdCreate->GetLeft(), frmDoublePwdCreate->GetBottom());
			frmDoublePwdInfo->Refresh();

			frmDoublePwdInfo->SetIsShow(true);
		}

		frmDoublePwdInput->SetPos(frmDoublePwdCreate->GetRight(), frmDoublePwdCreate->GetTop());
		frmDoublePwdInput->Refresh();
		ShowDoublePwdKeyboardForm();
	}


	// 
	void CDoublePwdMgr::ShowAlterForm() {
		CloseAllForm();

		if (frmDoublePwdAlter && !frmDoublePwdAlter->GetIsShow()) {
			edtDoublePwdAlterOld->SetCaption("");
			edtDoublePwdAlterNew->SetCaption("");
			edtDoublePwdAlterNewRetry->SetCaption("");

			frmDoublePwdAlter->Show();
		}

		frmDoublePwdInput->SetPos(frmDoublePwdAlter->GetRight(), frmDoublePwdAlter->GetTop());
		frmDoublePwdInput->Refresh();
		ShowDoublePwdKeyboardForm();
	}


	// 
	void CDoublePwdMgr::ShowDoublePwdForm() {
		CloseAllForm();

		if (frmDoublePwd && !frmDoublePwd->GetIsShow()) {
			edtFocusEditBox = edtDoublePwd;

			edtDoublePwd->SetCaption("");
			frmDoublePwd->Show();
		}

		frmDoublePwdInput->SetPos(frmDoublePwd->GetRight(), frmDoublePwd->GetTop());
		frmDoublePwdInput->Refresh();
		ShowDoublePwdKeyboardForm();
	}


	// 
	void CDoublePwdMgr::CloseAllForm() {
		// 
		if (frmDoublePwdCreate && frmDoublePwdCreate->GetIsShow()) {
			frmDoublePwdCreate->Close();
		}

		// 
		if (frmDoublePwdAlter && frmDoublePwdAlter->GetIsShow()) {
			frmDoublePwdAlter->Close();
		}

		// 
		if (frmDoublePwd && frmDoublePwd->GetIsShow()) {
			frmDoublePwd->Close();
		}

		// 
		if (frmDoublePwdInput && frmDoublePwdInput->GetIsShow()) {
			frmDoublePwdInput->Close();
		}

		// 
		if (frmDoublePwdInfo && frmDoublePwdInfo->GetIsShow()) {
			frmDoublePwdInfo->Close();
		}
	}


	// 
	void CDoublePwdMgr::ShowDoublePwdKeyboardForm() {
		if (!frmDoublePwdInput)
			return;

		RandomInputButton();
		frmDoublePwdInput->Refresh();
		frmDoublePwdInput->Show();
	}


	// 
	void CDoublePwdMgr::SendDeleteCharactor() {
		CSelectChaScene* pScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());
		pScene->SendDelChaToServer(g_stUIDoublePwd.edtDoublePwd->GetCaption());
		CGameApp::Waiting();
	}


	// 
	void CDoublePwdMgr::SendPackageUnlock() {
		CS_UnlockKitbag(g_stUIDoublePwd.edtDoublePwd->GetCaption());
	}

	//  add by ning.yan 2008-11-11 begin
	void CDoublePwdMgr::SendItemUnlock() // id
	{
		CS_UnlockItem(g_stUIDoublePwd.edtDoublePwd->GetCaption(), lockGridID);
		g_stUIDoublePwd.CloseAllForm();
	}

	void CDoublePwdMgr::SendUnlockSelectedItems() {
		auto grid = g_stUIEquip.GetGoodsGrid();
		if (!grid) {
			return;
		}

		for (auto i = 0, n = grid->GetMaxNum(); i < n; ++i) {
			if (!grid->IsItemSelected(i)) {
				continue;
			}

			auto item = static_cast<CItemCommand*>(grid->GetItem(i));
			if (!item || !item->GetIsValid()) {
				continue;
			}

			if (!item->IsLocked()) {
				continue;
			}

			CS_UnlockItem(g_stUIDoublePwd.edtDoublePwd->GetCaption(), i);
		}

		grid->ResetItemSelections();
	}

	// end

	// 
	void CDoublePwdMgr::SendPackageStoreOpen() {
		if (!g_stUIStore.ResetLastOperate()) {
			CloseAllForm();
			return;
		}

		g_stUIStore.ShowStoreLoad();
		CS_StoreOpenAsk(g_stUIDoublePwd.edtDoublePwd->GetCaption());
		CloseAllForm();
	}

	void CDoublePwdMgr::SendGameRequest() // id
	{
		CS_SendGameRequest(g_stUIDoublePwd.edtDoublePwd->GetCaption());

		g_stUIDoublePwd.CloseAllForm();
	}


	// 
	void CDoublePwdMgr::RandomInputButton() {
		CTextButton* btnNum[10] = {0};

		char szName[32] = {0};
		for (int i = 0; i < 10; ++i) {
			sprintf(szName, "btnNum%d", i);
			btnNum[i] = dynamic_cast<CTextButton*>(frmDoublePwdInput->Find(szName));

			if (!btnNum[i])
				return;
		}

		const int nRandomCount = 10;
		srand(g_pGameApp->GetCurTick());

		int nOldX, nOldY, nNewX, nNewY, nNum1, nNum2;
		for (int i = 0; i < nRandomCount;) {
			nNum1 = rand() % 10;
			nNum2 = rand() % 10;

			if (nNum1 == nNum2)
				continue;

			nOldX = btnNum[nNum1]->GetLeft();
			nOldY = btnNum[nNum1]->GetTop();
			nNewX = btnNum[nNum2]->GetLeft();
			nNewY = btnNum[nNum2]->GetTop();

			btnNum[nNum1]->SetPos(nNewX, nNewY);
			btnNum[nNum2]->SetPos(nOldX, nOldY);

			++i;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////

	// 
	void CDoublePwdMgr::_evtCreateFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		string strName = pSender->GetName();

		if (strName == "btnYes") {
			int nPwdLen = (int)strlen(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption());

			if (!IsPwdValid(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption())) {
				// 
				g_pGameApp->MsgBox("%s", GetLanguageString(797).c_str()); //" 0 ~ 9 "

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			if (6 > nPwdLen || 12 < nPwdLen) {
				//  6 ~ 12 
				g_pGameApp->MsgBox("%s", GetLanguageString(798).c_str()); //" 6 ~ 12"

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			if (0 != strcmp(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption(),
							g_stUIDoublePwd.edtDoublePwdCreateRetry->GetCaption())) {
				// 
				g_pGameApp->MsgBox("%s", GetLanguageString(799).c_str()); //""

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			// 
			CS_CreatePassword2(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption());
			CCursor::I()->SetCursor(CCursor::stWait);
		}
		else {
			//
			CS_Logout();
			CS_Disconnect(DS_DISCONN);
			g_pGameApp->LoadScriptScene(enumLoginScene);
		}
	}


	// 
	void CDoublePwdMgr::_evtAlterFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		string strName = pSender->GetName();
		if (strName.size() <= 0) return;

		if (strName == "btnYes") {
			if (!IsPwdValid(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption())) {
				// 
				g_pGameApp->MsgBox("%s", GetLanguageString(797).c_str()); //" 0 ~ 9 "

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			int nPwdLen = (int)strlen(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption());
			if (6 > nPwdLen || 12 < nPwdLen) {
				//  6 ~ 12 
				g_pGameApp->MsgBox("%s", GetLanguageString(798).c_str()); //" 6 ~ 12"

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			if (0 != strcmp(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption(),
							g_stUIDoublePwd.edtDoublePwdAlterNewRetry->GetCaption())) {
				// 
				g_pGameApp->MsgBox("%s", GetLanguageString(799).c_str()); //""

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			// 
			CS_UpdatePassword2(g_stUIDoublePwd.edtDoublePwdAlterOld->GetCaption(),
							   g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption());
		}
		else {
			g_stUIDoublePwd.CloseAllForm();
		}
	}


	// 
	void CDoublePwdMgr::_evtDoublePwdFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		string strName = pSender->GetName();
		if (strName.size() <= 0) return;

		if (strName == "btnYes") {
			switch (g_stUIDoublePwd.m_nType) {
			case DELETE_CHARACTOR:
				g_stUIDoublePwd.SendDeleteCharactor();
				break;

			case PACKAGE_UNLOCK:
				g_stUIDoublePwd.SendPackageUnlock();
				break;

			case STORE_OPEN_ASK:
				g_stUIDoublePwd.SendPackageStoreOpen();
				break;

			// add by ning.yan 2008-11-11  begin
			case ITEM_UNLOCK:
				g_stUIDoublePwd.SendItemUnlock();
				break; // end

			case MULTI_ITEM_UNLOCK:
				g_stUIDoublePwd.SendUnlockSelectedItems();

			case MC_REQUEST:
				g_stUIDoublePwd.SendGameRequest();
				break; // end
			}
		}
		else {
			g_stUIDoublePwd.RandomInputButton();
			g_stUIDoublePwd.CloseAllForm();
		}
	}


	// 
	void CDoublePwdMgr::_evtInputFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		if (!g_stUIDoublePwd.edtFocusEditBox)
			return;

		string strName = pSender->GetName();
		if (strName.empty()) return;

		char cNumber = strName[strName.size() - 1];
		string strPwd = g_stUIDoublePwd.edtFocusEditBox->GetCaption();

		if (strName == "btnClear") {
			// 
			g_stUIDoublePwd.edtFocusEditBox->SetCaption("");
		}
		else if ('0' <= cNumber && cNumber <= '9' && strPwd.size() < 12) {
			// 
			strPwd += cNumber;
			g_stUIDoublePwd.edtFocusEditBox->SetCaption(strPwd.c_str());
		}
	}


	// 
	void CDoublePwdMgr::_evtEditFocus(CGuiData* pSender) {
		CEdit* edtTemp = dynamic_cast<CEdit*>(pSender);
		if (edtTemp) {
			g_stUIDoublePwd.SetFocusEditBox(edtTemp);
		}
	}


	// 
	void CDoublePwdMgr::_evtFormClose(CForm* pForm, bool& IsClose) {
		//g_stUIDoublePwd.CloseAllForm();
	}
}
