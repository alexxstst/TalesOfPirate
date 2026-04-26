//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include "Character.h"
#include "GameApp.h"
#include ".\guildmemberdata.h"
#include ".\guildmembersmgr.h"

using namespace std;

vector<CGuildMemberData*> CGuildMembersMgr::m_pGuildMembers;

CGuildMembersMgr::CGuildMembersMgr(void) {
}

CGuildMembersMgr::~CGuildMembersMgr(void) {
}

void CGuildMembersMgr::AddGuildMember(CGuildMemberData* pGuildMember) {
	m_pGuildMembers.push_back(pGuildMember);
}

bool CGuildMembersMgr::DelGuildMember(CGuildMemberData* pGuildMember) {
	vector<CGuildMemberData*>::iterator Iter;
	for (Iter = m_pGuildMembers.begin(); Iter != m_pGuildMembers.end(); Iter++) {
		if ((*Iter) == pGuildMember) {
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI
			m_pGuildMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CGuildMembersMgr::DelGuildMemberByID(DWORD dwID) {
	vector<CGuildMemberData*>::iterator Iter;
	for (Iter = m_pGuildMembers.begin(); Iter != m_pGuildMembers.end(); Iter++) {
		if ((*Iter)->GetID() == dwID) {
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI
			m_pGuildMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CGuildMembersMgr::DelGuildMemberByName(string strName) {
	vector<CGuildMemberData*>::iterator Iter;
	for (Iter = m_pGuildMembers.begin(); Iter != m_pGuildMembers.end(); Iter++) {
		if ((*Iter)->GetName() == strName) {
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI
			m_pGuildMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

CGuildMemberData* CGuildMembersMgr::FindGuildMemberByID(DWORD dwID) {
	vector<CGuildMemberData*>::iterator Iter;
	for (Iter = m_pGuildMembers.begin(); Iter != m_pGuildMembers.end(); Iter++) {
		if ((*Iter)->GetID() == dwID) {
			return (*Iter);
		}
	}
	return NULL;
}

CGuildMemberData* CGuildMembersMgr::FindGuildMemberByName(string strName) {
	vector<CGuildMemberData*>::iterator Iter;
	for (Iter = m_pGuildMembers.begin(); Iter != m_pGuildMembers.end(); Iter++) {
		if ((*Iter)->GetName() == strName) {
			return (*Iter);
		}
	}
	return NULL;
}

CGuildMemberData* CGuildMembersMgr::FindGuildMemberByIndex(DWORD dwIndex) {
	if (dwIndex >= GetTotalGuildMembers()) return NULL;
	return m_pGuildMembers[dwIndex];
}

DWORD CGuildMembersMgr::GetTotalGuildMembers() {
	return static_cast<DWORD>(m_pGuildMembers.size());
}

void CGuildMembersMgr::ResetAll() {
	vector<CGuildMemberData*>::iterator Iter;
	while (m_pGuildMembers.size() > 0) {
		Iter = m_pGuildMembers.begin();
		CGuildMemberData* pNode = *Iter;
		//delete pNode;
		SAFE_DELETE(pNode); // UI
		m_pGuildMembers.erase(Iter);
	}
}

CGuildMemberData* CGuildMembersMgr::GetSelfData() {
	CCharacter* pCharacter = CGameScene::GetMainCha();
	if (!pCharacter) return NULL;
	return CGuildMembersMgr::FindGuildMemberByID(pCharacter->getHumanID());
}
