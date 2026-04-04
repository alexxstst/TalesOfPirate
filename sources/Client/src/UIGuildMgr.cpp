//------------------------------------------------------------------------
//	2005.4.27	Arcol	create this file
//------------------------------------------------------------------------


#include "stdafx.h"
#include "UIPage.h"
#include "netguild.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "GuildMemberData.h"
#include "GuildMembersMgr.h"
#include "RecruitMemberData.h"
#include "RecruitMembersMgr.h"
#include "UIBoxForm.h"
#include "UIListView.h"
#include "UILabel.h"
#include "UIEdit.h"
#include "UICheckbox.h"
#include "GameApp.h"
#include "Character.h"
#include "uiguildmgr.h"
#include "commfunc.h"
#include "uiBoatForm.h"
#include "ChaAttrType.h"
#include "SMallMap.h"
#include "UI3DCompent.h"
#include "NetChat.h"
#include "UIsystemform.h"
#include "GlobalVar.h"
#include "UITreeView.h"
#include "UIGraph.h"

using namespace std;


CForm* CUIGuildMgr::m_pGuildMgrForm = NULL;
CForm* CUIGuildMgr::m_pGuildPermForm = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildName = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildMottoName = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildType = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildMaster = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildMemberCount = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildState = NULL;
CLabelEx* CUIGuildMgr::m_plabGuildRemainTime = NULL;
//CLabelEx*	CUIGuildMgr::m_plabGuildRank=NULL;
CListView* CUIGuildMgr::m_plstGuildMember = NULL;
CListView* CUIGuildMgr::m_plstRecruitMember = NULL;
CPage* CUIGuildMgr::m_ppgeClass = NULL;
CTextButton* CUIGuildMgr::m_pbtnGuildMottoEdit = NULL;
CTextButton* CUIGuildMgr::m_pbtnGuildDisband = NULL;
CTextButton* CUIGuildMgr::m_pbtnGuildLeave = NULL;
CTextButton* CUIGuildMgr::m_pbtnMemberRecruit = NULL;
CTextButton* CUIGuildMgr::m_pbtnMemberRefuse = NULL;
CTextButton* CUIGuildMgr::m_pbtnMemberKick = NULL;
CTextButton* CUIGuildMgr::m_pbtnMemberPerms = NULL;
CTextButton* CUIGuildMgr::m_pbtnMemberYesPerms = NULL;

//Guild Vault logs:
CListView* CUIGuildMgr::m_plistBankLog = NULL;
CTextButton* CUIGuildMgr::m_btnPrev = NULL;
CTextButton* CUIGuildMgr::m_btnNext = NULL;

CForm* CUIGuildMgr::m_pGuildMottoNameEditForm = NULL;
CEdit* CUIGuildMgr::m_pedtGuildMottoName = NULL;
CTextButton* CUIGuildMgr::m_pbtnGuildMottoFormOK = NULL;

CTreeView* CUIGuildMgr::m_trvPerm = NULL;


CUIGuildMgr::CUIGuildMgr(void) {
}

CUIGuildMgr::~CUIGuildMgr(void) {
}

bool CUIGuildMgr::Init() {
	FORM_LOADING_CHECK(m_pGuildMgrForm, "manage.clu", "frmManage");
	FORM_LOADING_CHECK(m_pGuildPermForm, "manage.clu", "frmGuildPerm");
	m_pGuildMgrForm->evtBeforeShow = OnBeforeShow;

	FORM_CONTROL_LOADING_CHECK(m_plabGuildName, m_pGuildMgrForm, CLabelEx, "manage.clu", "labName");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMottoName, m_pGuildMgrForm, CLabelEx, "manage.clu", "labMaxim");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildType,m_pGuildMgrForm,CLabelEx,"manage.clu","labState");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMaster, m_pGuildMgrForm, CLabelEx, "manage.clu", "labPeople");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMemberCount, m_pGuildMgrForm, CLabelEx, "manage.clu", "labNum");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildState,m_pGuildMgrForm,CLabelEx,"manage.clu","labReason");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildRemainTime,m_pGuildMgrForm,CLabelEx,"manage.clu","labRemain");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildMottoEdit, m_pGuildMgrForm, CTextButton, "manage.clu", "btnMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildDisband, m_pGuildMgrForm, CTextButton, "manage.clu", "btnDisband");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildLeave, m_pGuildMgrForm, CTextButton, "manage.clu", "btnLeave");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberRecruit, m_pGuildMgrForm, CTextButton, "manage.clu", "btnYes");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberRefuse, m_pGuildMgrForm, CTextButton, "manage.clu", "btnNo");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberKick, m_pGuildMgrForm, CTextButton, "manage.clu", "btnkick");
	FORM_CONTROL_LOADING_CHECK(m_plstGuildMember, m_pGuildMgrForm, CListView, "manage.clu", "lstNum");
	FORM_CONTROL_LOADING_CHECK(m_plstRecruitMember, m_pGuildMgrForm, CListView, "manage.clu", "lstAsk");
	FORM_CONTROL_LOADING_CHECK(m_ppgeClass, m_pGuildMgrForm, CPage, "manage.clu", "pgePublic");

	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberYesPerms, m_pGuildPermForm, CTextButton, "manage.clu", "btnYesPerm");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberPerms, m_pGuildMgrForm, CTextButton, "manage.clu", "btnperm");

	m_pbtnGuildLeave->evtMouseClick = [](CGuiData* pSender, int x, int y, DWORD key) {
		CBoxMgr::ShowSelectBox(_OnClickLeave, GetLanguageString(596).c_str(), true);
	};
	m_pbtnGuildDisband->evtMouseClick = [](CGuiData* pSender, int x, int y, DWORD key) {
		CBoxMgr::ShowPasswordBox(_OnPassDismiss, "Disband Guild");
	};

	m_pbtnGuildMottoEdit->evtMouseClick = _OnClickEditMottoName;
	m_pbtnMemberRecruit->evtMouseClick = _OnClickRecruit;
	m_pbtnMemberRefuse->evtMouseClick = _OnClickRefuse;
	m_pbtnMemberKick->evtMouseClick = _OnClickKick;
	m_ppgeClass->evtSelectPage = _OnClickSelectPage;
	m_pbtnMemberKick->SetIsEnabled(true);
	m_pbtnMemberRecruit->SetIsEnabled(false);
	m_pbtnMemberRefuse->SetIsEnabled(false);
	m_plabGuildName->SetIsCenter(true);
	m_plabGuildMottoName->SetIsCenter(true);
	m_plabGuildMaster->SetIsCenter(true);
	m_plabGuildMemberCount->SetIsCenter(true);

	m_pbtnMemberYesPerms->evtMouseClick = _OnClickConfirmPerm;
	m_pbtnMemberPerms->evtMouseClick = _OnClickPerm;
	m_pbtnMemberYesPerms->SetIsEnabled(true);
	m_pbtnMemberPerms->SetIsEnabled(true);


	FORM_LOADING_CHECK(m_pGuildMottoNameEditForm, "manage.clu", "frmEditMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pedtGuildMottoName, m_pGuildMottoNameEditForm, CEdit, "manage.clu", "edtMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildMottoFormOK, m_pGuildMottoNameEditForm, CTextButton, "manage.clu", "btnYes");
	m_pbtnGuildMottoFormOK->evtMouseClick = _OnClickMottoFormOK;

	//new permission form
	m_trvPerm = dynamic_cast<CTreeView*>(m_pGuildPermForm->Find("trvEditor"));
	m_trvPerm->evtMouseDown = _OnClickPermText;
	m_trvPerm->SetRowSpace(10);
	m_trvPerm->SetSelectColor(0);

	m_pGuildPermForm->evtEntrustMouseEvent = _evtPermFormMouseEvent;

	// Guild Vault logs:
	FORM_CONTROL_LOADING_CHECK(m_plistBankLog, m_pGuildMgrForm, CListView, "manage.clu", "listBankLog");
	FORM_CONTROL_LOADING_CHECK(m_btnNext, m_pGuildMgrForm, CTextButton, "manage.clu", "btnNext");
	FORM_CONTROL_LOADING_CHECK(m_btnPrev, m_pGuildMgrForm, CTextButton, "manage.clu", "btnPrev");
	m_btnPrev->evtMouseClick = _OnClickPrevLogs;
	m_btnNext->evtMouseClick = _OnClickNextLogs;
	//m_plistBankLog->GetList()->GetScroll()->evtChange = _evtLogDrag;
	//m_plistBankLog->GetList()->GetScroll()->Init();
	g_stUIGuildMgr.curLogPage = 1;

	return true;
}

/*
void CUIGuildMgr::_evtLogDrag(){
	CListItems* _pItems = m_plistBankLog->GetList()->GetItems();
	CScroll*	_pScroll = m_plistBankLog->GetList()->GetScroll();
	_pItems->SetFirstShowRow( (DWORD)_pScroll->GetStep().GetPosition() );


	if(_pScroll->GetStep().GetRate() == 1.f && !isUpdating){
	isUpdating = true;
	CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDLOGS, &g_stUIGuildMgr.banklogs);
	if(_pScroll->GetDrag())
	_pScroll->GetDrag()->SetDragCursor((CCursor::eState)23);
	}


}
*/


void CUIGuildMgr::_evtPermFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	if (strName == "btnPredef") {
		//toggle all other predef buttons
		char buf[32];
		for (int i = 1; i <= 6; i++) {
			sprintf(buf, "btnPredef%d", i);
			CTextButton* btn = dynamic_cast<CTextButton*>(m_pGuildPermForm->Find(buf));
			btn->SetIsShow(!btn->GetIsShow());
		}
	}
	else if (strName == "btnPredef1") {
		//admin
		SetActivePerm(emGldPermMax);
	}
	else if (strName == "btnPredef2") {
		//leader
		SetActivePerm(12029);
	}
	else if (strName == "btnPredef3") {
		//architect
		SetActivePerm(3585);
	}
	else if (strName == "btnPredef4") {
		//banker
		SetActivePerm(541);
	}
	else if (strName == "btnPredef5") {
		//trusted
		SetActivePerm(3613);
	}
	else if (strName == "btnPredef6") {
		//player
		SetActivePerm(513);
	}
}

void CUIGuildMgr::_OnClickPermText(CGuiData* pSender, int x, int y, DWORD key) {
	CItemObj* permText = m_trvPerm->GetHitItem(x, y);
	if (permText->GetColor() != 0xFFFF0000) {
		permText->SetColor(0xFFFF0000);
	}
	else {
		permText->SetColor(0xFF00FF00);
	}
}


void CUIGuildMgr::_OnClickConfirmPerm(CGuiData* pSender, int x, int y, DWORD key) {
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();

	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm() & emGldPermMgr);
	if (pSelfData && perm == emGldPermMgr) {
		CItemRow* pRow = m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow) {
			CBoxMgr::ShowMsgBox(NULL, GetLanguageString(600).c_str(), true);
			return;
		}

		CGuildMemberData* pMemberData = static_cast<CGuildMemberData*>(pRow->GetPointer());
		unsigned int perms = 0;

		int posX = m_pGuildPermForm->GetX() + 30;
		int posY = m_pGuildPermForm->GetY() + 25;

		for (int i = 1; i <= emGldPermNum; i++) {
			int locY = posY + (i * 22);
			CItemObj* permText = m_trvPerm->GetHitItem(posX, locY);
			if (permText && permText->GetColor() == 0xFF00FF00) {
				perms += pow(2, i - 1);
			}
		}

		CS_SetGuildPerms(pMemberData->GetID(), perms);
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, "You do not have permissions to do this", true);
	}
}

void CUIGuildMgr::SetActivePerm(int perm) {
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();
	int posX = m_pGuildPermForm->GetX() + 30;
	int posY = m_pGuildPermForm->GetY() + 25;
	for (int i = 1; i <= emGldPermNum; i++) {
		int locY = posY + (i * 22);
		CItemObj* permText = m_trvPerm->GetHitItem(posX, locY);
		if (permText) {
			int permCheck = pow(2, i - 1);
			if (perm & permCheck) {
				permText->SetColor(0xFF00FF00);
			}
			else {
				permText->SetColor(0xFFFF0000);
			}
		}
	}
}

void CUIGuildMgr::_OnClickPerm(CGuiData* pSender, int x, int y, DWORD key) {
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();

	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm() & emGldPermMgr);
	if (pSelfData && perm == emGldPermMgr) {
		CItemRow* pRow = m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow) {
			CBoxMgr::ShowMsgBox(NULL, "No user selected", true);
			return;
		}

		CGuildMemberData* pMemberData = static_cast<CGuildMemberData*>(pRow->GetPointer());
		SetActivePerm(pMemberData->GetPerm());
		m_pGuildPermForm->Show();
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, "You do not have permissions to do this", true);
	}
}

void CUIGuildMgr::UpdateGuildLogs(const net::msg::ActionUpdateGuildLogsData& data) {
	g_stUIGuildMgr.banklogs.clear();
	m_plistBankLog->GetList()->GetScroll()->Reset();

	m_btnPrev->SetIsEnabled(false);
	m_btnNext->SetIsEnabled(!data.terminated);

	for (const auto& entry : data.logs) {
		BankLog l;
		l.type = static_cast<int>(entry.type);
		l.time = static_cast<int>(entry.time);
		l.parameter = static_cast<int>(entry.parameter);
		l.quantity = static_cast<int>(entry.quantity);
		l.userID = static_cast<int>(entry.userId);
		g_stUIGuildMgr.banklogs.push_back(l);
	}

	m_plistBankLog->GetList()->GetItems()->Clear();
	UpdateLogList();
}


void CUIGuildMgr::UpdateLogList() {
	m_plistBankLog->GetList()->Clear();
	m_plistBankLog->GetList()->GetItems()->Clear();
	int finish = g_stUIGuildMgr.curLogPage * 13;
	int start = finish - 13;

	int d = g_stUIGuildMgr.banklogs.size();
	for (int i = start; i < finish; i++) {
		if (i >= g_stUIGuildMgr.banklogs.size()) {
			break;
		}
		BankLog* curlog = &g_stUIGuildMgr.banklogs.at(i);
		CItemRow* p = m_plistBankLog->AddItemRow();
		CItem* desc = new CItem;
		p->SetIndex(0, desc);
		p->SetColor(COLOR_BLACK);

		CGuildMemberData* j = CGuildMembersMgr::FindGuildMemberByID(curlog->userID);
		string name = "Unknown";
		if (j) {
			// Member not found inside guild
			name = j->GetName();
		}
		char* buf1 = (char*)calloc(1, 200);
		char* buf2 = (char*)calloc(1, 200);

		time_t serverTime = curlog->time - 5 * 60 * 60; //GMT -5

		tm* k = gmtime(&serverTime);

		strftime(buf2, 200, "[%d/%m/%y] %H:%M ", k);
		switch (curlog->type) {
		case 0: {
			sprintf(buf1, "%s withdrew %d gold", name.c_str(), curlog->parameter);
			break;
		}
		case 1: {
			sprintf(buf1, "%s deposited %d gold", name.c_str(), curlog->parameter);
			break;
		}
		case 2: {
			sprintf(buf1, "%s withdrew %dx '%s'", name.c_str(), curlog->quantity,
					GetItemRecordInfo(curlog->parameter)->szName);
			break;
		}
		case 3: {
			sprintf(buf1, "%s deposited %dx '%s'", name.c_str(), curlog->quantity,
					GetItemRecordInfo(curlog->parameter)->szName);
			break;
		}
		}
		strcat(buf2, buf1);
		m_plistBankLog->GetList()->GetItems()->GetItem(i - start)->GetIndex(0)->SetString(buf2);
	}
	m_plistBankLog->Refresh();
	m_pGuildMgrForm->Refresh();
	m_btnPrev->SetIsEnabled(g_stUIGuildMgr.curLogPage > 1);
}

void CUIGuildMgr::RequestGuildLogs(const net::msg::ActionRequestGuildLogsData& data) {
	if (data.terminated) {
		m_btnNext->SetIsEnabled(false);
	}

	for (const auto& entry : data.logs) {
		BankLog l;
		l.type = static_cast<int>(entry.type);
		l.time = static_cast<int>(entry.time);
		l.parameter = static_cast<int>(entry.parameter);
		l.quantity = static_cast<int>(entry.quantity);
		l.userID = static_cast<int>(entry.userId);
		g_stUIGuildMgr.banklogs.push_back(l);
	}

	UpdateLogList();
}

void CUIGuildMgr::_OnClickNextLogs(CGuiData* pSender, int x, int y, DWORD key) {
	g_stUIGuildMgr.curLogPage += 1;
	// For "previous" button, check if client has enough data to fill the previous page
	if (g_stUIGuildMgr.banklogs.size() < g_stUIGuildMgr.curLogPage * 13) {
		// Ask server for more data
		uShort curSize = g_stUIGuildMgr.banklogs.size();
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDLOGS, &curSize);
	}
	else {
		// Just update the list with older logs
		UpdateLogList();
	}
}

void CUIGuildMgr::_OnClickPrevLogs(CGuiData* pSender, int x, int y, DWORD key) {
	if (g_stUIGuildMgr.curLogPage == 1) {
		g_pGameApp->MsgBox("Can't fetch newer data");
		return;
	}
	if (!m_btnNext->GetIsEnabled()) {
		// "Next" button is not enabled, which means we reached the end. After going back one page, make it available again.
		m_btnNext->SetIsEnabled(true);
	}
	g_stUIGuildMgr.curLogPage -= 1;
	// For "next" button, we'll always have the data, so just update the list.
	UpdateLogList();
}

void CUIGuildMgr::ShowForm() {
	RefreshForm();
	m_pGuildMgrForm->nTag = 1; // nTag==0
	m_pGuildMgrForm->Show();
}

void CUIGuildMgr::_OnClickEditMottoName(CGuiData* pSender, int x, int y, DWORD key) {
	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm() & emGldPermMotto);
	if (pSelfData && perm == emGldPermMotto) {
		m_pedtGuildMottoName->SetCaption(CGuildData::GetGuildMottoName().c_str());
		m_pGuildMottoNameEditForm->ShowModal();
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, GetLanguageString(594).c_str(), true);
	}
}

void CUIGuildMgr::_OnClickRecruit(CGuiData* pSender, int x, int y, DWORD key) {
	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm() & emGldPermRecruit);
	if (pSelfData && perm == emGldPermRecruit) {
		CItemRow* pRow = m_plstRecruitMember->GetList()->GetSelectItem();
		if (!pRow) {
			CBoxMgr::ShowMsgBox(NULL, GetLanguageString(597).c_str(), true);
			return;
		}
		CRecruitMemberData* pMemberData = static_cast<CRecruitMemberData*>(pRow->GetPointer());
		CM_GUILD_APPROVE(pMemberData->GetID());
		m_plstRecruitMember->GetList()->Del(pRow);
		CRecruitMembersMgr::DelRecruitMember(pMemberData);
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, GetLanguageString(598).c_str(), true);
	}
}

void CUIGuildMgr::_OnClickRefuse(CGuiData* pSender, int x, int y, DWORD key) {
	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm() & emGldPermRecruit);
	if (pSelfData && perm == emGldPermRecruit) {
		CItemRow* pRow = m_plstRecruitMember->GetList()->GetSelectItem();
		if (!pRow) {
			CBoxMgr::ShowMsgBox(NULL, GetLanguageString(597).c_str(), true);
			return;
		}
		CRecruitMemberData* pMemberData = static_cast<CRecruitMemberData*>(pRow->GetPointer());
		CM_GUILD_REJECT(pMemberData->GetID());
		m_plstRecruitMember->GetList()->Del(pRow);
		CRecruitMembersMgr::DelRecruitMember(pMemberData);
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, GetLanguageString(599).c_str(), true);
	}
}

void CUIGuildMgr::_OnClickKick(CGuiData* pSender, int x, int y, DWORD key) {
	CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
	if (pSelfData && (pSelfData->GetPerm() & emGldPermKick) == emGldPermKick) {
		CItemRow* pRow = m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow) {
			CBoxMgr::ShowMsgBox(NULL, GetLanguageString(600).c_str(), true);
			return;
		}
		CGuildMemberData* pMemberData = static_cast<CGuildMemberData*>(pRow->GetPointer());
		string str = GetLanguageString(601) + pMemberData->GetName() + GetLanguageString(602);
		CBoxMgr::ShowSelectBox(_OnPassKick, str.c_str(),true);
	}
	else {
		CBoxMgr::ShowMsgBox(NULL, GetLanguageString(603).c_str(), true);
	}
}

void CUIGuildMgr::_OnClickMottoFormOK(CGuiData* pSender, int x, int y, DWORD key) {
	string name = m_pedtGuildMottoName->GetCaption();
	if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, name)
		|| !IsValidName(name)
	) {
		g_pGameApp->MsgBox("%s", GetLanguageString(51).c_str());
		return;
	}
	CM_GUILD_MOTTO(name.c_str());
	m_pGuildMottoNameEditForm->Hide();
}

void CUIGuildMgr::RefreshForm() {
	RefreshAttribute();
	RefreshList();
}

void CUIGuildMgr::RefreshAttribute() {
	char buf[50];
	m_plabGuildName->SetCaption(CGuildData::GetGuildName().c_str());
	m_plabGuildMaster->SetCaption(CGuildData::GetGuildMasterName().c_str());
	m_plabGuildMottoName->SetCaption(CGuildData::GetGuildMottoName().c_str());

	sprintf(buf, "%d/%d", CGuildData::GetMemberCount(), CGuildData::GetMaxMembers());
	m_plabGuildMemberCount->SetCaption(buf);

	CGuildData::eState state = CGuildData::GetGuildState();
	string strState;
	if (state == CGuildData::normal) {
		strState = GetLanguageString(606);
		//m_plabGuildState->SetCaption(strState.c_str());
		//m_plabGuildRemainTime->SetCaption("");
	}
	else {
		strState = GetLanguageString(607);
		if (state & CGuildData::money) {
			strState += GetLanguageString(608);
		}
		if (state & CGuildData::repute) {
			strState += GetLanguageString(609);
		}
		if (state & CGuildData::member) {
			strState += GetLanguageString(610);
		}
		//m_plabGuildState->SetCaption(strState.c_str());
		__int64 remain = CGuildData::GetRemainTime();
		if (remain > 1440) {
			sprintf(buf, GetLanguageString(611).c_str(), remain / 1440);
		}
		else if (remain > 60) {
			sprintf(buf, GetLanguageString(612).c_str(), remain / 60);
		}
		else {
			sprintf(buf, GetLanguageString(613).c_str(), remain);
		}
		//m_plabGuildRemainTime->SetCaption(buf);
	}


	{
		// Display either disband or leave button
		auto isGuildLeader = [&]()-> bool {
			CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
			return (pSelfData && pSelfData->GetID() == CGuildData::GetGuildMasterID());
		}();

		m_pbtnGuildDisband->SetIsShow(isGuildLeader);
		m_pbtnGuildLeave->SetIsShow(!isGuildLeader);
	}

	m_pGuildMgrForm->Refresh();
}

void CUIGuildMgr::RefreshList() {
	char buf[50];
	m_plstGuildMember->GetList()->GetItems()->Clear();
	for (DWORD i = 0; i < CGuildMembersMgr::GetTotalGuildMembers(); i++) {
		CGuildMemberData* pMemberData = CGuildMembersMgr::FindGuildMemberByIndex(i);
		if (!pMemberData) continue;
		CItemRow* pRow = m_plstGuildMember->GetList()->NewItem();
		CItem* pMemberNameItem = new CItem(pMemberData->GetName().c_str(), COLOR_BLACK);
		CItem* pMemberJobItem = new CItem(pMemberData->GetJob().c_str(), COLOR_BLACK);
		CItem* pMemberLevelItem = new CItem(_i64toa(pMemberData->GetLevel(), buf, 10), COLOR_BLACK);
		pRow->SetIndex(0, pMemberNameItem);
		pRow->SetIndex(1, pMemberJobItem);
		pRow->SetIndex(2, pMemberLevelItem);
		pRow->SetPointer(pMemberData);
	}

	m_plstRecruitMember->GetList()->GetItems()->Clear();
	for (DWORD i = 0; i < CRecruitMembersMgr::GetTotalRecruitMembers(); i++) {
		CRecruitMemberData* pMemberData = CRecruitMembersMgr::FindRecruitMemberByIndex(i);
		if (!pMemberData) continue;
		CItemRow* pRow = m_plstRecruitMember->GetList()->NewItem();
		CItem* pMemberNameItem = new CItem(pMemberData->GetName().c_str(), COLOR_BLACK);
		CItem* pMemberJobItem = new CItem(pMemberData->GetJob().c_str(), COLOR_BLACK);
		CItem* pMemberLevelItem = new CItem(_i64toa(pMemberData->GetLevel(), buf, 10), COLOR_BLACK);
		pRow->SetIndex(0, pMemberNameItem);
		pRow->SetIndex(1, pMemberJobItem);
		pRow->SetIndex(2, pMemberLevelItem);
		pRow->SetPointer(pMemberData);
	}
	m_pGuildMgrForm->Refresh();
}

void CUIGuildMgr::_OnClickSelectPage(CGuiData* pSender) {
	m_pbtnGuildMottoEdit->SetIsShow(true);
	m_pbtnMemberPerms->SetIsShow(true);

	int n = m_ppgeClass->GetIndex();
	if (n == 0) //
	{
		// Members page
		m_pbtnMemberKick->SetIsEnabled(true);
		m_pbtnMemberRecruit->SetIsEnabled(false);
		m_pbtnMemberRefuse->SetIsEnabled(false);

		CTextButton* chkSortName = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortName"));
		CTextButton* chkSortClass = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortClass"));
		CTextButton* chkSortLevel = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortLevel"));
	}
	else if (n == 1) //
	{
		//Apply page
		m_pbtnMemberRecruit->SetIsEnabled(true);
		m_pbtnMemberRefuse->SetIsEnabled(true);
		m_pbtnMemberKick->SetIsEnabled(false);
	}
	else if (n == 2) //
	{
		//guildbank
		m_pbtnMemberRecruit->SetIsEnabled(false);
		m_pbtnMemberRefuse->SetIsEnabled(false);
		m_pbtnMemberKick->SetIsEnabled(false);

		CGuildMemberData* pSelfData = CGuildMembersMgr::GetSelfData();
		if (!pSelfData) {
			return;
		}

		CCompent* bankLocked = dynamic_cast<CCompent*>(m_pGuildMgrForm->Find("bankLocked"));
		CTextButton* btnGoldTake = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("btngoldtake"));
		CTextButton* btnGoldPut = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("btngoldput"));

		int perm = (pSelfData->GetPerm() & emGldPermViewBank);
		if (perm == emGldPermViewBank) {
			bankLocked->SetIsShow(false);
			btnGoldTake->SetIsShow(true);
			btnGoldPut->SetIsShow(true);
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDBANK, NULL);
		}
		else {
			bankLocked->SetIsShow(true);
			btnGoldTake->SetIsShow(false);
			btnGoldPut->SetIsShow(false);
		}
	}
	else if (n == 3) {
		g_stUIGuildMgr.curLogPage = 1;
		m_pbtnGuildMottoEdit->SetIsShow(false);
		m_pbtnMemberPerms->SetIsShow(false);
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_UPDATEGUILDLOGS, NULL);
	}
	//else if (n==3){ //guild stats form
	//CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDATTR, NULL);
	//}
	//else if (n==4){ //guild quest form
	//CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDQUEST, NULL);
	//}
}

void CUIGuildMgr::RemoveForm() {
	m_pGuildMgrForm->Close();
}

void CUIGuildMgr::_OnPassDismiss(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) return;
	stPasswordBox* pBox = (stPasswordBox*)pSender->GetForm()->GetPointer();
	if (!pBox) return;
	string str = pBox->edtPassword->GetCaption();
	if (str.length() > 0) {
		CM_GUILD_DISBAND(str.c_str());
	}
}

void CUIGuildMgr::_OnClickLeave(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType == CForm::mrYes) {
		CM_GUILD_LEAVE();
	}
}

void CUIGuildMgr::_OnPassKick(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) return;
	CItemRow* pRow = m_plstGuildMember->GetList()->GetSelectItem();
	if (!pRow) {
		CBoxMgr::ShowMsgBox(NULL, GetLanguageString(614).c_str(), true);
		return;
	}
	CGuildMemberData* pMemberData = static_cast<CGuildMemberData*>(pRow->GetPointer());
	CM_GUILD_KICK(pMemberData->GetID());
}

void CUIGuildMgr::OnBeforeShow(CForm* pForm,bool& IsShow) {
	if (!pForm->nTag) {
		CM_GUILD_LISTTRYPLAYER();
		IsShow = false;
	}
	pForm->nTag = 0;
}
