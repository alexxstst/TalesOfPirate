//------------------------------------------------------------------------
//	2005.3.15	Arcol	create this file
//	2005.4.5	Arcol	add the event handle function
//------------------------------------------------------------------------

#include "StdAfx.h"
#include "UIGlobalVar.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "NetChat.h"
#include "uicloneform.h"
#include ".\teaminviteformmgr.h"

using namespace std;

static CCloneForm dupe;
CTeamInviteFormMgr g_TeamInviteFormMgr;
vector<CTeamInviteFormMgr::PS_FormNode> CTeamInviteFormMgr::m_FormLink;

CTeamInviteFormMgr::CTeamInviteFormMgr(void) {
}

CTeamInviteFormMgr::~CTeamInviteFormMgr(void) {
}

bool CTeamInviteFormMgr::AddInviteForm(DWORD id, string inviterName) {
	if (GetInviteForm(id)) return false;
	PS_FormNode node = new S_FormNode;
	static CForm* frmAcceptGroup = CFormMgr::s_Mgr.Find("frmAcceptGroup");
	if (!frmAcceptGroup) {
		//delete node;
		SAFE_DELETE(node); // UI
		return false;
	}
	node->id = id;
	node->inviterName = inviterName;
	dupe.SetSample(frmAcceptGroup);
	node->pForm = dupe.Clone();
	//node->pForm=dynamic_cast<CForm*>(frmAcceptGroup->Clone());
	node->pForm->SetName(std::format("frmAcceptGroup_Arcol_{}", id).c_str());
	m_FormLink.push_back(node);
	//node->pForm->SetPos(200,200);
	//node->pForm->Refresh();
	CLabelEx* labGroupName = dynamic_cast<CLabelEx*>(node->pForm->Find("labGroupName"));
	if (labGroupName) {
		const std::string _str = SafeVFormat(GetLanguageString(62), inviterName.data());
		//labGroupName->SetIsCenter(true);
		labGroupName->SetCaption(_str.c_str());
	}
	CLabelEx* labGroup = dynamic_cast<CLabelEx*>(node->pForm->Find("labGroup"));
	if (labGroup) {
		//labGroup->SetIsCenter(true);
		labGroup->SetCaption(GetLanguageString(420).c_str());
	}
	node->pForm->evtEntrustMouseEvent = _MainMousePlayerGroupEvent;
	node->pForm->nTag = id;
	static int PosY = 100;
	PosY += 10;
	if (PosY > 150) PosY = 100;
	node->pForm->SetPos(10, PosY);
	node->pForm->Refresh();
	node->pForm->Show();
	return true;
}

bool CTeamInviteFormMgr::RemoveInviteForm(DWORD id) {
	PS_FormNode node = NULL;
	vector<PS_FormNode>::iterator Iter;
	for (Iter = m_FormLink.begin(); Iter != m_FormLink.end(); Iter++)
		if ((*Iter)->id == id) {
			node = *Iter;
			break;
		}
	if (!node) return false;
	node->pForm->Close();
	node->pForm->nTag = 0;
	dupe.Release(node->pForm);
	m_FormLink.erase(Iter);
	//delete node;
	SAFE_DELETE(node); // UI
	return true;
}

CTeamInviteFormMgr::PS_FormNode CTeamInviteFormMgr::GetInviteForm(DWORD id) {
	vector<PS_FormNode>::iterator Iter;
	for (Iter = m_FormLink.begin(); Iter != m_FormLink.end(); Iter++)
		if ((*Iter)->id == id)
			return *Iter;
	return NULL;
}

void CTeamInviteFormMgr::_MainMousePlayerGroupEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	if (name == "btnNo" || name == "btnClose") {
		CS_Team_Refuse((DWORD)pSender->GetForm()->nTag);
	}
	else if (name == "btnYes") {
		CS_Team_Confirm((DWORD)pSender->GetForm()->nTag);
	}
	g_stTeamInviteFormMgr.RemoveInviteForm(pSender->GetForm()->nTag);
}
