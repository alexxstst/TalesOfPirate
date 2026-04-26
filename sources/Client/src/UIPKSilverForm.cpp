#include "stdafx.h"
#include "UIPKSilverForm.h"
#include "uiformmgr.h"
#include "UIGoodsGrid.h"
#include "uiEquipForm.h"
#include "uiItemCommand.h"
#include "uinpctalkform.h"
#include "NetProtocol.h"
#include "packetcmd.h"
#include "GameApp.h"

namespace GUI {
	CPKSilverMgr::CPKSilverMgr() {
	}


	CPKSilverMgr::~CPKSilverMgr() {
	}


	void CPKSilverMgr::ShowPKSilverForm(bool bShow/* = true*/) {
		if (bShow) {
			frmPKSilver->Show();
		}
		else {
			frmPKSilver->Hide();
		}
	}


	bool CPKSilverMgr::AddFormAttribute(int idx, const std::string& szName, long sLevel, const std::string& szJob,
										long lPkval) {
		if ((idx < 0) || (idx >= MAX_PKSILVER_PLAYER)) {
			return false;
		}

		if ((!labName[idx]) || (!labLevel[idx]) || (!labJob[idx]) || (!labDate[idx])) {
			return false;
		}

		if (sLevel < 0) {
			labName[idx]->SetCaption("");
			labLevel[idx]->SetCaption("");
			labJob[idx]->SetCaption("");
			labDate[idx]->SetCaption("");
			return true;
		}

		labName[idx]->SetCaption(szName.c_str());
		labLevel[idx]->SetCaption(std::format("{}", sLevel).c_str());
		labJob[idx]->SetCaption(szJob.c_str());
		labDate[idx]->SetCaption(std::format("{}", lPkval).c_str());
		return true;
	}


	bool CPKSilverMgr::Init() {
		CFormMgr& mgr = CFormMgr::s_Mgr;

		frmPKSilver = mgr.Find("frmPKSilver");
		if (!frmPKSilver) {
			ToLogService("common", "frmPKSilver not found.");
			return false;
		}

		for (int i = 0; i < MAX_PKSILVER_PLAYER; i++) {
			std::string szTitle = std::format("labName_{}", i);
			labName[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle.c_str()));
			if (!labName[i]) {
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

			szTitle = std::format("labLevel_{}", i);
			labLevel[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle.c_str()));
			if (!labLevel[i]) {
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

			szTitle = std::format("labJob_{}", i);
			labJob[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle.c_str()));
			if (!labJob[i]) {
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

			szTitle = std::format("labData_{}", i);
			labDate[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle.c_str()));
			if (!labDate[i]) {
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}
		}
		//frmPKSilver->evtEntrustMouseEvent = _evtPKSilverSortBtn;
		return true;
	}


	void CPKSilverMgr::CloseForm() {
	}

	// void CPKSilverMgr::_evtPKSilverSortBtn(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	// {
	//     string szName = pSender->GetName();
	//     if(szName == "btnSort")
	//     {
	//         //  
	//         CGoodsGrid* pGrid  = g_stUIEquip.GetGoodsGrid();
	//CItemCommand* pCmd = NULL;
	//         CItemRecord* pItemRecord = NULL;

	//int nCount = pGrid->GetMaxNum();
	//for(int i = 0; i < nCount; ++i)
	//{
	//	pCmd = dynamic_cast<CItemCommand*>(pGrid->GetItem(i));
	//	if(pCmd)
	//	{
	//		pItemRecord = pCmd->GetItemInfo();

	//		if(pItemRecord && pItemRecord->lID == 1123)
	//		{
	//                     CS_PKSilverSort(g_stUINpcTalk.GetNpcId(), i);
	//			return;
	//		}
	//	}
	//}
	//         g_pGameApp->MsgBox(GetLanguageString(849));
	//     }
	// }
};
