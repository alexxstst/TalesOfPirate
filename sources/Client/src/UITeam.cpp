#include "stdafx.h"
#include "UITeam.h"
#include "UIChat.h"
#include "GuildMemberData.h"
#include "GuildMembersMgr.h"
#include "UIGuildMgr.h"
#include "NetChat.h"
#include "TalkSessionFormMgr.h"
#include "StringLib.h"
using namespace std;
using namespace GUI;


CMemberData::CMemberData()
	: _nHp(0), _nSp(0), _nLv(0), _nMaxHp(0), _nMaxSp(0), _bOnline(false),
	  _pFace(NULL), _byWork(0), _dwIndex(0) {
}

CMemberData::~CMemberData() {
	SAFE_DELETE(_pFace);
}

void CMemberData::SetFace(const stNetTeamChaPart& pFace) {
	if (!_pFace) _pFace = new stNetTeamChaPart;
	memcpy(_pFace, &pFace, sizeof(stNetTeamChaPart));
}

//---------------------------------------------------------------------------
// CMember
//---------------------------------------------------------------------------

eShowStyle CMember::_nShowStyle = enumShowQQName;

CMember::CMember(CTeam* pTeam, unsigned long id, const char* strName, const char* strMotto, DWORD icon_id)
	: _nID(id), _pData(NULL), _pTeam(pTeam), _strName(strName), _strMotto(strMotto), _pPointer(0), _nIcon_id(icon_id),
	  _strShowName(""), _bFlash(false) {
	if (pTeam->GetStyle() == enumTeamGroup) {
		_pData = new CMemberData;
	}
}

CMember::~CMember() {
	g_stUIChat.TeamSend(enumSTM_DEL_MEMBER, this, _pTeam->GetStyle());

	//if( _pData )
	//{
	//    delete _pData;
	//}
	SAFE_DELETE(_pData); // UI
}

void CMember::Refresh() {
	g_stUIChat.TeamSend(enumSTM_NODE_DATA_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetName(const char* str) {
	_strName = str;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetMotto(const char* str) {
	_strMotto = str;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetShowStyle(eShowStyle style) {
	_nShowStyle = style;
}

void CMember::ModifyAttr(string motto, string job, DWORD dwLv, DWORD nIcon_ID, string guildName) {
	_strMotto = motto;
	_strJob = job;
	_dwLv = dwLv;
	_nIcon_id = nIcon_ID;
	_strGuildName = guildName;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetJobName(const char* str) {
	_strJob = str;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetIconID(DWORD iconId) {
	_nIcon_id = iconId;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

void CMember::SetOnline(bool isOnline) {
	_bIsOnline = isOnline;
	g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, this, _pTeam->GetStyle());
}

const char* CMember::GetShowName() {
	_strShowName = GetName();
	string strMotto = GetMotto();
	if (!strMotto.empty()) {
		_strShowName += "(" + strMotto + ")";
	}
	_strShowName = StringLimit(_strShowName, 14);
	return _strShowName.c_str();
}

//---------------------------------------------------------------------------
// CTeam
//---------------------------------------------------------------------------
CTeam::CTeam(eTeamStyle nStyle, const char* szName)
	: _eStyle(nStyle), _pPointer(0), _strName(szName), _nCount(0) {
	g_stUIChat.TeamSend(enumSTM_ADD_GROUP, this, nStyle);
}

CTeam::~CTeam() {
	Clear();
}

CMember* CTeam::Find(unsigned long nID) {
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		if ((*it)->GetID() == nID) {
			return *it;
		}
	}
	return NULL;
}

CMember* CTeam::FindByName(const char* szName) {
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		if (stricmp((*it)->GetName(), szName) == 0) {
			return *it;
		}
	}
	return NULL;
}

CMember* CTeam::Add(unsigned long nID, const char* szName, const char* szMotto, DWORD icon_id) {
	if (nID > 0) {
		_nCount++;
		CMember* tmp;
		string strMotto;
		if (szMotto == NULL)
			strMotto = GetLanguageString(778);
		else
			strMotto = szMotto;

		tmp = new CMember(this, nID, szName, strMotto.c_str(), icon_id);

		CMember* pMember = g_stUIChat.GetTeamMgr()->Find(enumTeamFrnd)->Find(nID);
		if (pMember) {
			pMember->SetMotto(strMotto.c_str());
		}
		pMember = g_stUIChat.GetTeamMgr()->Find(enumTeamMaster)->Find(nID);
		if (pMember) {
			pMember->SetMotto(strMotto.c_str());
		}
		pMember = g_stUIChat.GetTeamMgr()->Find(enumTeamPrentice)->Find(nID);
		if (pMember) {
			pMember->SetMotto(strMotto.c_str());
		}
		pMember = g_stUIChat.GetTeamMgr()->Find(enumTeamGroup)->Find(nID);
		if (pMember) {
			pMember->SetMotto(strMotto.c_str());
		}

		CGuildMemberData* pGuildMember = CGuildMembersMgr::FindGuildMemberByID(nID);
		if (pGuildMember) {
			pGuildMember->SetMottoName(szMotto);
			CTextGraph* pItem = static_cast<CTextGraph*>(pGuildMember->GetPointer());
			if (pItem) {
				string str = pGuildMember->GetName();
				if (strlen(szMotto) > 0) {
					str += "(" + string(szMotto) + ")";
				}
				pItem->SetHint(str.c_str());
				str = StringLimit(str, 14);
				pItem->SetString(str.c_str());
			}
			CUIGuildMgr::RefreshList();
		}
		CTalkSessionFormMgr::RefreshSessionMember(nID, strMotto.c_str());
		_member.push_back(tmp);

		g_stUIChat.TeamSend(enumSTM_ADD_MEMBER, tmp, this->GetStyle());
		return tmp;
	}
	return NULL;
}

bool CTeam::Del(unsigned long nID) {
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		if ((*it)->GetID() == nID) {
			_nCount--;
			//delete *it;
			SAFE_DELETE(*it); // UI
			_member.erase(it);

			g_stUIChat.TeamSend(enumSTM_AFTER_DEL_MEMBER, NULL, GetStyle());
			return true;
		}
	}
	return false;
}

bool CTeam::DelByName(const char* szName) {
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		if (stricmp((*it)->GetName(), szName) == 0) {
			_nCount--;
			//delete *it;
			SAFE_DELETE(*it); // UI
			_member.erase(it);

			g_stUIChat.TeamSend(enumSTM_AFTER_DEL_MEMBER, NULL, GetStyle());
			return true;
		}
	}
	return false;
}

void CTeam::Clear() {
	_nCount = 0;
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		//  delete *it;
		SAFE_DELETE(*it); // UI
	}

	_member.clear();

	g_stUIChat.TeamSend(enumSTM_DEL_GROUP, this, _eStyle);
}

CMember* CTeam::GetMember(unsigned long nIndex) {
	if (nIndex >= _nCount) {
		return NULL;
	}

	unsigned int n = 0;
	for (members::iterator it = _member.begin(); it != _member.end(); it++) {
		if (n == nIndex)
			return *it;

		n++;
	}
	return NULL;
}

//---------------------------------------------------------------------------
// CTeamMgr
//---------------------------------------------------------------------------
DWORD CTeamMgr::_dwTeamLeaderID = 0;

CTeamMgr::CTeamMgr() {
	_pFrndTeam = new CTeam(enumTeamFrnd, GetLanguageString(466).c_str());
	_pGroupTeam = new CTeam(enumTeamGroup, GetLanguageString(299).c_str());
	_pRoadTeam = new CTeam(enumTeamRoad, GetLanguageString(469).c_str());
	_pMasterTeam = new CTeam(enumTeamMaster, GetLanguageString(850).c_str());
	_pPrenticeTeam = new CTeam(enumTeamPrentice, GetLanguageString(851).c_str());
	_pGMTeam = new CTeam(enumTeamGM, "GM");
	_pBlockedTeam = new CTeam(enumTeamBlocked, "Blocked");
}

CTeamMgr::~CTeamMgr() {
	SAFE_DELETE(_pFrndTeam);
	SAFE_DELETE(_pGroupTeam);
	SAFE_DELETE(_pRoadTeam);
	SAFE_DELETE(_pMasterTeam);
	SAFE_DELETE(_pPrenticeTeam);
	SAFE_DELETE(_pGMTeam);
	SAFE_DELETE(_pBlockedTeam);
}

CTeam* CTeamMgr::Add(eTeamStyle eTeam, const char* szName) {
	switch (eTeam) {
	case enumTeamFrnd: {
		_pFrndTeam->Clear();
		return _pFrndTeam;
	}
	case enumTeamGroup: {
		_pGroupTeam->Clear();
		return _pGroupTeam;
	}
	case enumTeamRoad: {
		_pRoadTeam->Clear();
		return _pRoadTeam;
	}
	case enumTeamMaster: {
		_pMasterTeam->Clear();
		return _pMasterTeam;
	}
	case enumTeamPrentice: {
		_pPrenticeTeam->Clear();
		return _pPrenticeTeam;
	}
	case enumTeamGM: {
		_pGMTeam->Clear();
		return _pGMTeam;
	}
	case enumTeamBlocked: {
		_pBlockedTeam->Clear();
		return _pBlockedTeam;
	}
	}
	return NULL;
}

bool CTeamMgr::Del(eTeamStyle eTeam, const char* szName) {
	switch (eTeam) {
	case enumTeamFrnd: {
		_pFrndTeam->Clear();
		return true;
	}
	case enumTeamGroup: {
		_dwTeamLeaderID = 0;

		_pGroupTeam->Clear();
		return true;
	}
	case enumTeamRoad: {
		_pRoadTeam->Clear();
		return true;
	}
	case enumTeamMaster: {
		_pMasterTeam->Clear();
		return true;
	}
	case enumTeamPrentice: {
		_pPrenticeTeam->Clear();
		return true;
	}
	case enumTeamGM: {
		_pGMTeam->Clear();
		return true;
	}
	case enumTeamBlocked: {
		_pBlockedTeam->Clear();
		return true;
	}
	}
	return false;
}

CTeam* CTeamMgr::Find(eTeamStyle eTeam, const char* szName) {
	switch (eTeam) {
	case enumTeamFrnd: {
		return _pFrndTeam;
	}
	case enumTeamGroup: {
		return _pGroupTeam;
	}
	case enumTeamRoad: {
		return _pRoadTeam;
	}
	case enumTeamMaster: {
		return _pMasterTeam;
	}
	case enumTeamPrentice: {
		return _pPrenticeTeam;
	}
	case enumTeamGM: {
		return _pGMTeam;
	}
	case enumTeamBlocked: {
		return _pBlockedTeam;
	}
	}
	return NULL;
}

void CTeamMgr::ChangeStyle(eShowStyle style) {
	CMember::SetShowStyle(style);
	for (DWORD i = 0; i < _pFrndTeam->GetCount(); i++) {
		DWORD j = _pFrndTeam->GetCount();
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pFrndTeam->GetMember(i), enumTeamFrnd);
	}
	for (DWORD i = 0; i < _pGroupTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pGroupTeam->GetMember(i), enumTeamGroup);
	}
	for (DWORD i = 0; i < _pRoadTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pRoadTeam->GetMember(i), enumTeamRoad);
	}
	for (DWORD i = 0; i < _pMasterTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pMasterTeam->GetMember(i), enumTeamMaster);
	}
	for (DWORD i = 0; i < _pPrenticeTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pPrenticeTeam->GetMember(i), enumTeamPrentice);
	}
	for (DWORD i = 0; i < _pGMTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pGMTeam->GetMember(i), enumTeamGM);
	}
	for (DWORD i = 0; i < _pBlockedTeam->GetCount(); i++) {
		g_stUIChat.TeamSend(enumSTM_NODE_CHANGE, _pBlockedTeam->GetMember(i), enumTeamBlocked);
	}
}

void CTeamMgr::SceneSwitch() {
	_pRoadTeam->Clear();
	g_stUIChat._curSelectMember = NULL;
}

void CTeamMgr::ResetAll() {
	_pRoadTeam->Clear();
	_pGroupTeam->Clear();
	_pFrndTeam->Clear();
	_pMasterTeam->Clear();
	_pPrenticeTeam->Clear();
	_pGMTeam->Clear();
	_pBlockedTeam->Clear();

	g_stUIChat._curSelectMember = NULL;
	g_stUIChat._bForbid = false;
}
