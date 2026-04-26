#include "StdAfx.h"
#include "UIPKDialog.h"
#include "NetProtocol.h"
#include "UIFormMgr.h"
#include "UILabel.h"
#include "UIForm.h"
#include "PacketCmd.h"
#include "UIListView.h"
#include "UIList.h"
#include "UIItem.h"
#include "GameApp.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool CPkDialog::Init() {
	CFormMgr& mgr = CFormMgr::s_Mgr;

	{
		// pk 
		frmTeamPkStart = mgr.Find("frmTeamPK");
		if (!frmTeamPkStart) {
			g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(744).c_str());
			return false;
		}
		frmTeamPkStart->evtEntrustMouseEvent = _MainMousePkStartEvent;
		frmTeamPkStart->SetIsEscClose(false);

		for (int i(0); i < TEAM_NUM; i++) {
			const std::string szBuf = std::format("lstTeam{}", i);
			lvStartTeams[i] = dynamic_cast<CListView*>(frmTeamPkStart->Find(szBuf.c_str()));
			if (!lvStartTeams[i])
				return Error(GetLanguageString(616).c_str(),
							 frmTeamPkStart->GetName(),
							 szBuf.c_str());

			CItemRow* pRow(0);
			CItem* pItem(0);
			lvStartTeams[i]->GetList()->GetItems()->Clear();
			for (int j(0); j < MEM_NUM; j++) {
				pRow = lvStartTeams[i]->GetList()->GetItems()->NewItem();
				for (int k(0); k < S_ITEM_NUM; k++) {
					pItem = new CItem();
					pItem->SetColor(COLOR_BLACK);
					pRow->SetIndex(k, pItem);
				}
			}
		}
	}

	{
		// pk 
		//frmTeamPkEnd = mgr.Find("frmfrmTeamPkStart");
		//if ( !frmTeamPkEnd)
		//{
		//	LG("gui", "npc.clufrmfrmTeamPk");
		//	return false;
		//}
		//frmTeamPkEnd->evtEntrustMouseEvent = _MainMousePkEndEvent;

		//char szBuf[32];
		//for (int i(0); i<TEAM_NUM; i++)
		//{
		//	lvEndTeams[i] = dynamic_cast<CListView*>(frmTeamPkEnd->Find(szBuf));
		//	if (!lvEndTeams[i]) 
		//		return Error("npc.clu<%s><%s>",
		//					 frmTeamPkEnd->GetName(), 
		//					 szBuf);
		//}
	}

	return true;
}

//-----------------------------------------------------------------------------
void CPkDialog::SetStartDialogContent(const stNetTeamFightAsk& sNetTeamFightAsk) {
	ClearStartDialog();

	int iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	int iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);

	for (int i(0); i < iLeftNum; i++) {
		auto* row = lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i);
		row->GetIndex(0)->SetString(sNetTeamFightAsk.Info[i].szName.c_str());
		row->GetIndex(1)->SetString(sNetTeamFightAsk.Info[i].szJob.c_str());
		row->GetIndex(2)->SetString(std::format("{}", static_cast<int>(sNetTeamFightAsk.Info[i].chLv)).c_str());
		row->GetIndex(3)->SetString(std::format("{}", static_cast<int>(sNetTeamFightAsk.Info[i].usVictoryNum)).c_str());
		row->GetIndex(4)->SetString(std::format("{}", static_cast<int>(sNetTeamFightAsk.Info[i].usFightNum)).c_str());
	}

	iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);
	iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	for (int i(0); i < iRightNum; i++) {
		auto* row = lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i);
		const auto& info = sNetTeamFightAsk.Info[i + iLeftNum];
		row->GetIndex(0)->SetString(info.szName.c_str());
		row->GetIndex(1)->SetString(info.szJob.c_str());
		row->GetIndex(2)->SetString(std::format("{}", static_cast<int>(info.chLv)).c_str());
		row->GetIndex(3)->SetString(std::format("{}", static_cast<int>(info.usVictoryNum)).c_str());
		row->GetIndex(4)->SetString(std::format("{}", static_cast<int>(info.usFightNum)).c_str());
	}
}

//-----------------------------------------------------------------------------
void CPkDialog::SetEndDialogContent() {
}

//-----------------------------------------------------------------------------
void CPkDialog::ClearStartDialog() {
	for (int i(0); i < TEAM_NUM; i++) {
		for (int j(0); j < MEM_NUM; j++)
			for (int k(0); k < S_ITEM_NUM; k++)
				lvStartTeams[i]->GetList()->GetItems()->GetItem(j)->GetIndex(k)->SetString("");
	}
}

//-----------------------------------------------------------------------------
string CPkDialog::ShowStartDialogDebug(const stNetTeamFightAsk& sNetTeamFightAsk) {
	int iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	int iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);

	string sShow;
	for (int i(0); i < iLeftNum; i++) {
		sShow += std::format("{}\t{}\t{}\n", static_cast<int>(sNetTeamFightAsk.Info[i].chLv),
				sNetTeamFightAsk.Info[i].szJob, sNetTeamFightAsk.Info[i].szName);
	}

	for (int i(0); i < iRightNum; i++) {
		const auto& info = sNetTeamFightAsk.Info[i + iLeftNum];
		sShow += std::format("{}\t{}\t{}\n", info.szName, info.szJob, static_cast<int>(info.chLv));
	}

	return sShow;
}

//-----------------------------------------------------------------------------
void CPkDialog::ClearEndDialog() {
	for (int i(0); i < TEAM_NUM; i++) {
		for (int j(0); j < MEM_NUM; j++)
			for (int k(0); k < E_ITEM_NUM; k++)
				lvEndTeams[i]->GetList()->GetItems()->GetItem(j)->GetIndex(k)->SetString("");
	}
}

//-----------------------------------------------------------------------------
void CPkDialog::ShowStartDialog(bool bShow) {
	frmTeamPkStart->SetIsShow(bShow);
}

//-----------------------------------------------------------------------------
void CPkDialog::ShowEndDialog(bool bShow) {
	frmTeamPkEnd->SetIsShow(bShow);
}

//-----------------------------------------------------------------------------
void CPkDialog::_MainMousePkStartEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	if (stricmp("frmTeamPk", pSender->GetForm()->GetName()) == 0) {
		CS_TeamFightAnswer(name == "btnYes");

		g_stUIPKDialog.frmTeamPkStart->Close();
	}
}

//-----------------------------------------------------------------------------
void CPkDialog::_MainMousePkEndEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	if (stricmp("frmTeamPk", pSender->GetForm()->GetName()) == 0) {
		g_stUIPKDialog.frmTeamPkEnd->Close();
	}
}
