#include "StdAfx.h"
#include "uiskillcommand.h"
#include "Scene.h"
#include "GameApp.h"
#include "Character.h"
#include "STAttack.h"
#include "state_reading.h"
#include "Actor.h"
#include "smallmap.h"
#include "ItemTypeSet.h"
#include "ItemRecord.h"
#include "StringLib.h"
#include "uiboatform.h"
#include "tools.h"
#include "PacketCmd.h"
#include "isskilluse.h"
#include "uibankform.h"

using namespace GUI;

using namespace std;

static char szBuf[256] = {0};

CGuiPic CSkillCommand::_imgActive;
//---------------------------------------------------------------------------
// class CSkillCommand
//---------------------------------------------------------------------------
CSkillCommand::CSkillCommand(CSkillRecord* pSkill)
	: CCommandObj(), _pAniClock(NULL), _pSkill(pSkill), _dwPlayTime(0) {
	if (!_pSkill)
		ToLogService("error", "msgCSkillCommand::CSkillCommand(CSkillRecord * pSkill) pSkill is NULL");

	_pImage = new CGuiPic;

	string file = "texture/icon/";
	file += pSkill->szICON;
	//file += ".tga";
	_pImage->LoadImage(file.c_str(), 32, 32, 0);

	// Loads saved skill clock if player used it.
	DWORD savedSkillTime = g_pGameApp->GetSkillClock(_pSkill->sID);
	if (savedSkillTime) {
		_pAniClock = g_pGameApp->AddAniClock();
		if (_pAniClock) {
			DWORD skillTime = _GetSkillTime();
			DWORD curSkillTime = CGameApp::GetCurTick() + skillTime + 30;
			DWORD timePassed = curSkillTime - savedSkillTime;
			_dwPlayTime = savedSkillTime;
			_pAniClock->Resume(skillTime, timePassed);
		}
	}
}

CSkillCommand::CSkillCommand(const CSkillCommand& rhs)
	: _pImage(new CGuiPic(*rhs._pImage)), _pAniClock(NULL), _dwPlayTime(0) {
}

CSkillCommand& CSkillCommand::operator=(const CSkillCommand& rhs) {
	_pAniClock = NULL;
	*_pImage = *rhs._pImage;
	_dwPlayTime = 0;
	return *this;
}

CSkillCommand::~CSkillCommand() {
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CSkillCommand::Render(int x, int y) {
	if (_pSkill->GetSkillGrid().chState)
		_pImage->Render(x, y);
		//else if(338 <= _pSkill->sID && _pSkill->sID <= 341)	//
		//	_pImage->Render( x, y );
		//else if(_pSkill->sID == 459)	//
		//	_pImage->Render( x, y );
	else if (GetIsSpecial(enumHighLight))
		_pImage->Render(x, y);
	else
		_pImage->Render(x, y, (DWORD)0xff757575);

	if (_pAniClock) {
		_pAniClock->Render(x, y);
		if ((CGameApp::GetCurTick() > _dwPlayTime) || _pAniClock->IsEnd()) {
			_pAniClock = NULL;
			g_pGameApp->DeleteSkillClock(_pSkill->sID);
		}
	}

	if (_pSkill->GetIsActive()) {
		_imgActive.Render(x, y);
	}
}

bool CSkillCommand::UseCommand(bool value) {
	CCharacter* pCha = CGameScene::GetMainCha();
	if (!pCha) return false;

	//
	if (!_pSkill->GetIsUse())
		return false;

	if (_pSkill->IsReadingSkill()) {
		CReadingState* reading = new CReadingState(pCha->GetActor());
		return pCha->GetActor()->SwitchState(reading);
	}

	if (_pSkill->GetIsActive()) {
		int nState = _pSkill->nStateID;
		if (nState > 0) {
			CS_BeginAction(pCha, enumACTION_STOP_STATE, &nState);
		}
		return nState > 0;
	}

	if (_pSkill->GetDistance() <= 0) {
		CAttackState* attack = new CAttackState(pCha->GetActor());
		attack->SetSkill(_pSkill);
		attack->SetTarget(pCha);
		attack->SetCommand(this);
		return pCha->GetActor()->SwitchState(attack);
	}
	return false;
}

bool CSkillCommand::StartCommand() {
	if (_GetSkillTime() > 0) {
		_pAniClock = g_pGameApp->AddAniClock();
		if (_pAniClock) {
			_dwPlayTime = CGameApp::GetCurTick() + _GetSkillTime() + 30;

			_pAniClock->Play(_GetSkillTime());
			g_pGameApp->AddTipText("CSkillCommand::Exec[%s]", _pSkill->szName);
			g_pGameApp->SetSkillClock(_pSkill->sID, _dwPlayTime);
			return true;
		}
		g_pGameApp->AddTipText("CSkillCommand::Exec[%s] Failed", _pSkill->szName);
		return false;
	}
	return true;
}

bool CSkillCommand::IsAllowUse() {
	if (_pSkill->GetIsActive()) return true;

	CCharacter* pCha = g_stUIBoat.GetHuman();
	if (!pCha) return false;

	if (g_stUIBank.GetBankGoodsGrid()->GetForm()->GetIsShow()) //
	{
		g_pGameApp->SysInfo(g_oLangRec.GetString(748));
		return false;
	}

	if (pCha->GetChaState()->IsFalse(enumChaStateUseSkill)) {
		g_pGameApp->SysInfo(g_oLangRec.GetString(748));
		return false;
	}

	if (!g_SkillUse.IsValid(_pSkill, pCha)) {
		g_pGameApp->SysInfo("%s", g_SkillUse.GetError());
		return false;
	}

	// :1.,2.
	if (_GetSkillTime() <= 0) {
		return true;
	}

	DWORD dwtmp2 = (_dwPlayTime - CGameApp::GetCurTick()) / 1000;
	if (_dwRecordTime != NULL &&
		_dwRecordTime == dwtmp2) {
		return false;
	}

	if (_pAniClock) {
		DWORD dwtmp = (_dwPlayTime - CGameApp::GetCurTick()) / 1000;
		if (dwtmp == 0)
			return false;

		g_pGameApp->SysInfo("Skill:[%s] in cooldown mode, remaining time is %d sec(s)",
							_pSkill->szName,
							dwtmp);
		_dwRecordTime = dwtmp;
		return false;
	}

	return true;
}

bool CSkillCommand::IsAtOnce() {
	return _pSkill->GetIsActive() || _pSkill->GetDistance() <= 0 || enumSKILL_TYPE_SELF == _pSkill->chApplyTarget;
}

bool CSkillCommand::ReadyUse() {
	return CGameScene::GetMainCha()->ChangeReadySkill(_pSkill->nID);
}

void CSkillCommand::Error() {
	g_pGameApp->AddTipText(g_oLangRec.GetString(750), _pSkill->szName);
}

void CSkillCommand::AddHint(int x, int y) {
	const unsigned int HINT_WIDTH = 24;

	sprintf(szBuf, "%s", _pSkill->szName);
	PushHint(szBuf, COLOR_WHITE, 5, 1);

	CCharacter* pMain = CGameScene::GetMainCha();
	if (pMain) {
		switch (_pSkill->chFightType) {
		case enumSKILL_LAND_LIVE:
			PushHint(g_oLangRec.GetString(751), pMain->IsBoat() ? COLOR_RED : COLOR_WHITE);
			break;

		case enumSKILL_FIGHT:
			if (_pSkill->chType == 1) {
				if (_pSkill->chSrcType == 1) {
					PushHint(g_oLangRec.GetString(752), pMain->IsBoat() ? COLOR_RED : COLOR_WHITE);
				}
				else {
					PushHint(g_oLangRec.GetString(753), !pMain->IsBoat() ? COLOR_RED : COLOR_WHITE);
				}
			}
			else {
				PushHint(g_oLangRec.GetString(754), COLOR_WHITE);
			}
			break;

		case enumSKILL_SAIL:
			//PushHint( g_oLangRec.GetString(755), pMain->IsBoat() ? COLOR_WHITE : COLOR_RED );
			PushHint(g_oLangRec.GetString(755), COLOR_WHITE);
			break;

		case enumSKILL_SEE_LIVE:
			PushHint(g_oLangRec.GetString(756), pMain->IsBoat() ? COLOR_WHITE : COLOR_RED);
			break;
		}
	}

	PushHint(g_oLangRec.GetString(757));
	StringNewLine(szBuf, HINT_WIDTH, _pSkill->szDescribeHint, (unsigned int)strlen(_pSkill->szDescribeHint));
	PushHint(szBuf);

	PushHint(g_oLangRec.GetString(758));
	StringNewLine(szBuf, HINT_WIDTH, _pSkill->szEffectHint, (unsigned int)strlen(_pSkill->szEffectHint));
	PushHint(szBuf);

	PushHint(g_oLangRec.GetString(759));
	StringNewLine(szBuf, HINT_WIDTH, _pSkill->szExpendHint, (unsigned int)strlen(_pSkill->szExpendHint));
	PushHint(szBuf);

	if (_pSkill->GetIsActive()) {
		PushHint(g_oLangRec.GetString(760));
	}
}

bool CSkillCommand::_WriteNeed(int nType, int nValue, const char* szStr) {
	if (nType == cchSkillRecordKeyValue) {
		return false;
	}

	if (nType == enumSKILL_ITEM_NEED_TYPE) {
		CItemTypeInfo* pInfo = GetItemTypeInfo(nValue);
		if (pInfo) {
			sprintf(szBuf, "%s %s", szStr, pInfo->szDataName);
			PushHint(szBuf, COLOR_RED);
		}
	}
	else {
		CItemRecord* pInfo = GetItemRecordInfo(nValue);
		if (pInfo) {
			sprintf(szBuf, "%s %s", szStr, pInfo->szDataName);
			PushHint(szBuf, COLOR_RED);
		}
	}
	return true;
}

const char* CSkillCommand::GetName() {
	return _pSkill->szName;
}

const char* CSkillCommand::GetSkillName() {
	if (_pSkill->chType == 1) {
		sprintf(szBuf, "%s\nLV:%d, SP:%d", _pSkill->szName, _pSkill->GetLevel(), _pSkill->GetSPExpend());
	}
	else {
		sprintf(szBuf, "%s\nLV:%d", _pSkill->szName, _pSkill->GetLevel());
	}
	return szBuf;
}


bool CSkillCommand::GetIsSpecial(eSpecialType SpecialType) {
	int nID = GetSkillRecord()->nID;

	switch (SpecialType) {
	case enumHighLight: {
		if (338 <= nID && nID <= 341) return true; //
		if (459 == nID) return true; //
	}
	break;

	case enumNotUpgrade: {
		if (453 <= nID && nID <= 459) return true; //
	}
	break;

	default:
		break;
	}

	return false;
}
