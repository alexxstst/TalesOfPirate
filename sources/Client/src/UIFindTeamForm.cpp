#include "stdafx.h"
#include "UIFindTeamForm.h"
#include "UIBoxForm.h"
#include "PacketCmd.h"
#include "GameApp.h"
#include "MapRecordStore.h"
#include "UIMiniMapForm.h"
#include "UIBoatForm.h"
#include "UIGlobalVar.h"
#include "Character.h"
#include <jobtype.h>	// common

#include "StringLib.h"
extern const char* g_szJobName[MAX_JOB_TYPE];

using namespace std;

namespace GUI {
	CFindTeamMgr::CFindTeamMgr() {
		m_dwLastTick = 0;
	}

	CFindTeamMgr::~CFindTeamMgr() {
	}


	bool CFindTeamMgr::Init() {
		//
		// 
		//
		frmFindTeam = CFormMgr::s_Mgr.Find("frmFindTeam");
		if (!frmFindTeam) {
			ToLogService("common", "frmFindTeam not found.");
			return false;
		}
		frmFindTeam->evtEntrustMouseEvent = _evtFindTeamMouseButton;

		labListPage = dynamic_cast<CLabelEx*>(frmFindTeam->Find("labListPage"));
		if (!labListPage) {
			ToLogService("common", "frmFindTeam:labListPage not found.");
			return false;
		}

		btnAddme = dynamic_cast<CTextButton*>(frmFindTeam->Find("btnAddme"));
		if (!btnAddme) {
			ToLogService("common", "frmFindTeam:btnAddme not found.");
			return false;
		}

		btnDelme = dynamic_cast<CTextButton*>(frmFindTeam->Find("btnDelme"));
		if (!btnDelme) {
			ToLogService("common", "frmFindTeam:btnDelme not found.");
			return false;
		}

		for (int i = 0; i < FINDTEAM_PAGE_SIZE; ++i) {
			std::string szName = std::format("labName_{}", i);
			labName[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName.c_str()));
			if (!labName[i]) {
				ToLogService("common", "frmFindTeam:{} not found.", szName);
				return false;
			}

			szName = std::format("labLevel_{}", i);
			labLevel[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName.c_str()));
			if (!labLevel[i]) {
				ToLogService("common", "frmFindTeam:{} not found.", szName);
				return false;
			}

			szName = std::format("labJob_{}", i);
			labJob[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName.c_str()));
			if (!labJob[i]) {
				ToLogService("common", "frmFindTeam:{} not found.", szName);
				return false;
			}

			szName = std::format("labPlace_{}", i);
			labPlace[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName.c_str()));
			if (!labPlace[i]) {
				ToLogService("common", "frmFindTeam:{} not found.", szName);
				return false;
			}

			szName = std::format("btnSubmit_{}", i);
			btnSubmit[i] = dynamic_cast<CTextButton*>(frmFindTeam->Find(szName.c_str()));
			if (!btnSubmit[i]) {
				ToLogService("common", "frmFindTeam:{} not found.", szName);
				return false;
			}
		}

		return true;
	}


	void CFindTeamMgr::CloseForm() {
		frmFindTeam->SetIsShow(false);
	}


	void CFindTeamMgr::ShowFindTeamForm(bool bShow) {
		if (g_stUIMap.IsGuildWar()) {
			frmFindTeam->SetIsShow(false);
			return;
		}

		frmFindTeam->SetIsShow(bShow);
	}


	void CFindTeamMgr::SetFindTeamPage(int nCurPage, int nPageNum) {
		m_nCurPage = nCurPage;
		m_nPageNum = nPageNum;

		labListPage->SetCaption(std::format("{}/{}", nCurPage, nPageNum).c_str());
	}


	void CFindTeamMgr::AddFindTeamInfo(int nSeq, const char* szName, long nLevel, long nJob, const char* szPlace) {
		if (0 <= nSeq && nSeq < FINDTEAM_PAGE_SIZE) {
			labName[nSeq]->SetCaption(szName);
			labName[nSeq]->SetIsShow(true);

			labLevel[nSeq]->SetCaption(std::format("{}", nLevel).c_str());
			labLevel[nSeq]->SetIsShow(true);

			if (0 <= nJob && nJob < MAX_JOB_TYPE) {
				labJob[nSeq]->SetCaption(g_szJobName[nJob]);
				labJob[nSeq]->SetIsShow(true);
			}

			CMapInfo* pMapInfo = GetMapInfo(szPlace);
			if (pMapInfo) {
				labPlace[nSeq]->SetCaption(pMapInfo->szName);
				labPlace[nSeq]->SetIsShow(true);
			}
			else {
				labPlace[nSeq]->SetCaption(szPlace);
				labPlace[nSeq]->SetIsShow(true);
			}

			btnSubmit[nSeq]->SetIsShow(true);
		}
	}

	void CFindTeamMgr::RemoveTeamInfo() {
		for (int i = 0; i < FINDTEAM_PAGE_SIZE; ++i) {
			labName[i]->SetIsShow(false);
			labLevel[i]->SetIsShow(false);
			labJob[i]->SetIsShow(false);
			labPlace[i]->SetIsShow(false);
			btnSubmit[i]->SetIsShow(false);
		}
	}

	void CFindTeamMgr::SetOwnFindTeamState(bool bState) {
		if (bState) {
			btnAddme->SetIsEnabled(false);
			btnDelme->SetIsEnabled(true);
		}
		else {
			btnAddme->SetIsEnabled(true);
			btnDelme->SetIsEnabled(false);
		}
	}


	void CFindTeamMgr::FindTeamAsk(const char* szName) {
		m_strTeamLeader = szName;

		char szBuffer[256] = {0};
		FmtLang(szBuffer, sizeof(szBuffer), GetLanguageString(863), szName);
		CBoxMgr::ShowSelectBox(_evtFindTeamCheckEvent, szBuffer, true);
	}


	// 
	void CFindTeamMgr::_evtFindTeamMouseButton(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		string strName = pSender->GetName();

		if (strName == "btnLeftPage") {
			if (g_stUIFindTeam.m_nCurPage > 1) {
				CS_VolunteerList(g_stUIFindTeam.m_nCurPage - 1, FINDTEAM_PAGE_SIZE);
			}
		}
		else if (strName == "btnRightPage") {
			if (g_stUIFindTeam.m_nCurPage < g_stUIFindTeam.m_nPageNum) {
				CS_VolunteerList(g_stUIFindTeam.m_nCurPage + 1, FINDTEAM_PAGE_SIZE);
			}
		}
		else if (strName == "btnAddme") {
			// 
			CS_VolunteerAdd();
		}
		else if (strName == "btnDelme") {
			// 
			CS_VolunteerDel();
		}
		else if (strName.substr(0, 10) == "btnSubmit_") {
			if (g_stUIBoat.GetHuman()->getLv() < 8) {
				g_pGameApp->MsgBox("Only players lv8 and above can request party!");
				return;
			}

			DWORD dwCurTick = g_pGameApp->GetCurTick();
			if (g_stUIFindTeam.m_dwLastTick + 1000 * FINDTEAM_INTERVAL > dwCurTick) {
				g_pGameApp->MsgBox(SafeVFormat(GetLanguageString(883), FINDTEAM_INTERVAL,
													 FINDTEAM_INTERVAL - (dwCurTick - g_stUIFindTeam.m_dwLastTick) /
													 1000));
				return;
			}
			g_stUIFindTeam.m_dwLastTick = dwCurTick;

			int nSeq = strName[strName.size() - 1] - '0';
			if (0 <= nSeq && nSeq < FINDTEAM_PAGE_SIZE) {
				// 
				if (g_stUIFindTeam.labName[nSeq]->GetIsShow()) {
					CS_VolunteerSel(g_stUIFindTeam.labName[nSeq]->GetCaption());
				}
			}
		}
	}


	// 
	void CFindTeamMgr::_evtFindTeamCheckEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		CS_VolunteerAsr(nMsgType == CForm::mrYes, g_stUIFindTeam.m_strTeamLeader.c_str());
	}
}
