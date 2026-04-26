#include "StdAfx.h"
#include "uimissionform.h"
#include "uiform.h"
#include "uimemo.h"
#include "PacketCmd.h"
#include "gameapp.h"

using namespace std;

using namespace GUI;
//---------------------------------------------------------------------------
// class CMissionMgr  
//---------------------------------------------------------------------------

CMissionMgr::CMissionMgr() {
	m_pMisForm = NULL;
	m_pMisInfo = NULL;
	m_pMisClose = NULL;
	m_pMisBtn1 = NULL;
	m_pMisBtn2 = NULL;

	m_dwNpcID = -1;
	m_byMisCmd = -1;
}

CMissionMgr::~CMissionMgr() {
}

bool CMissionMgr::Init() // 
{
	//npc
	m_pMisForm = _FindForm("frmNPCMission");
	if (!m_pMisForm) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(740).c_str());
		return false;
	}

	m_pMisForm->evtEntrustMouseEvent = _MouseEvent;
	m_pMisInfo = dynamic_cast<CMemoEx*>(m_pMisForm->Find("memMission"));
	//m_pMisInfo->evtClickItem = _ItemClickEvent;

	if (!m_pMisInfo) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(45), m_pMisForm->GetName(), "memMission"));
		return false;
	}

	m_pMisBtn1 = dynamic_cast<CTextButton*>(m_pMisForm->Find("btnYes"));
	if (!m_pMisBtn1) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(45), m_pMisForm->GetName(), "btnYes"));
		return false;
	}

	m_pMisBtn2 = dynamic_cast<CTextButton*>(m_pMisForm->Find("btnComplete"));
	if (!m_pMisBtn2) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(45), m_pMisForm->GetName(), "btnComplete"));
		return false;
	}

	m_pMisClose = dynamic_cast<CTextButton*>(m_pMisForm->Find("btnClose"));
	if (!m_pMisClose) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(45), m_pMisForm->GetName(), "btnClose"));
		return false;
	}

	return true;
}

void CMissionMgr::End() {
}

void CMissionMgr::_ItemClickEvent(string strItem) {
	const char* pStr = strItem.c_str();
}

void CMissionMgr::_MouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	if (stricmp("frmNPCMission", pSender->GetForm()->GetName()) == 0) {
		// ,
		if (strName == "btnNo" || strName == "btnClose") {
			pSender->GetForm()->Close();
		}
		else if (strName == "btnYes" || strName == "btnComplete") {
			BYTE bySel = 0;
			if (g_stUIMission.m_pMisInfo->IsSelPrize()) {
				bySel = g_stUIMission.m_pMisInfo->GetSelPrize();
				if (bySel == (BYTE)-1) {
					g_pGameApp->MsgBox("%s", GetLanguageString(741).c_str());
					return;
				}
			}
			pSender->GetForm()->Close();
			CS_MissionPage(g_stUIMission.m_dwNpcID, g_stUIMission.m_byMisCmd, bySel);
		}
	}
}

void CMissionMgr::CloseForm() {
	if (m_pMisForm->GetIsShow())
		m_pMisForm->Close();
}

void CMissionMgr::ShowMissionPage(DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page) {
	m_pMisBtn1->SetIsShow(false);
	m_pMisBtn2->SetIsShow(false);

	if (byCmd == ROLE_MIS_BTNACCEPT) //
	{
		m_pMisBtn1->SetIsShow(true);
	}
	else if (byCmd == ROLE_MIS_BTNDELIVERY) //
	{
		m_pMisBtn2->SetIsShow(true);
		m_pMisBtn2->SetIsEnabled(true);
		m_pMisInfo->SetIsSelect(TRUE);
	}
	else if (byCmd == ROLE_MIS_BTNPENDING) //
	{
		m_pMisBtn2->SetIsShow(true);
		m_pMisBtn2->SetIsEnabled(false);
	}

	m_dwNpcID = dwNpcID;
	m_byMisCmd = byCmd;

	m_pMisInfo->Init();
	m_pMisInfo->SetMisPage(page);
	m_pMisForm->SetIsShow(true);
}
