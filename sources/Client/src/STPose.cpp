#include "StdAfx.h"
#include "stpose.h"
#include "SteadyFrameSync.h"
#include "actor.h"
#include "Character.h"
#include "PacketCmd.h"
#include "NetProtocol.h"
#include "CharacterRecord.h"
#include "stmove.h"
#include "GameApp.h"

#define DEFAULT_NUM   -999
//---------------------------------------------------------------------------
// class CPoseState
//---------------------------------------------------------------------------
CPoseState::CPoseState(CActor* p)
	: CActionState(p), _nPose(POSE_WAITING), _isKeepPose(false) {
	_IsOver = true;
}

bool CPoseState::_Start() {
	// if( GetActor()->GetCha()->IsShop() ) return false;

	CCharacter* pCha = GetActor()->GetCha();
	pCha->PlayPose(_nPose, PLAY_ONCE, -1, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps(), true);

	if (_IsSend) {
		stNetFace face;
		face.sPose = _nPose;
		face.sAngle = pCha->getYaw();

		CS_BeginAction(GetActor()->GetCha(), enumACTION_FACE, &face, this);
	}
	return true;
}

void CPoseState::ActionEnd(DWORD pose_id) {
	if (pose_id != _nPose) {
		return;
	}

	PopState();
}

//---------------------------------------------------------------------------
// class CInsertState
//---------------------------------------------------------------------------
CInsertState::CInsertState(CActor* p)
	: CActionState(p), _nAngle(DEFAULT_NUM), _IsPlayPose(false), _IsFirst(true), _eAngle(enumInit) {
}

CInsertState::~CInsertState() {
}

bool CInsertState::_Start() {
	// false,,,,
	int rv = true;
	if (GetActor()->GetCha()->GetChaState()->IsFalse(enumChaStateMove))
		rv = false;

	if (GetActor()->GetCha()->IsShop())
		rv = false;

	if (_IsOver)
		rv = false;

	if (!rv) {
		return !_IsFirst;
	}

	static DWORD time = 0;
	if (CGameApp::GetCurTick() <= time) return true;
	time = CGameApp::GetCurTick() + 500;

	_IsFirst = false;
	CCharacter* pCha = GetActor()->GetCha();
	if (!_IsPlayPose) {
		GetActor()->GetCha()->DespawnMount();
		_IsPlayPose = true;
		pCha->PlayPose(POSE_SEAT, PLAY_ONCE, -1, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps(), true);
	}

	switch (_eAngle) {
	case enumAngle:
		pCha->FaceTo(_nAngle);
		break;
	case enumScrPos:
		_nAngle = pCha->FaceTo(_nScrX, _nScrY);
		break;
	}
	_eAngle = enumInit;

	if (_IsSend) {
		stNetFace face;
		face.sPose = POSE_SEAT;
		face.sAngle = _nAngle;
		CS_BeginAction(GetActor()->GetCha(), enumACTION_SKILL_POSE, &face, this);
	}
	return true;
}

void CInsertState::Cancel() {
	if (GetActor()->GetCha()->IsShop()) return;

	if (!_IsSend) return;

	static DWORD dwTime = 0;
	if (CGameApp::GetCurTick() < dwTime) {
		dwTime = CGameApp::GetCurTick() + 500;
		return;
	}

	if (g_stUISystem.m_sysProp.m_gameOption.bShowMounts) {
		GetActor()->GetCha()->RespawnMount();
	}

	_IsCancel = true;

	stNetFace face;
	face.sPose = POSE_WAITING;
	face.sAngle = _nAngle;

	CS_BeginAction(GetActor()->GetCha(), enumACTION_SKILL_POSE, &face, this);
}

//---------------------------------------------------------------------------
// class CEquipState
//---------------------------------------------------------------------------
CEquipState::CEquipState(CActor* p)
	: CActionState(p), _eType(enumInit) {
	_pUseItem = new stNetUseItem;
	_pUnfix = new stNetItemUnfix;

	_IsOver = true;
}

CEquipState::~CEquipState() {
	delete _pUseItem;
	delete _pUnfix;
}

bool CEquipState::_Start() {
	if (GetActor()->GetCha()->IsShop()) return false;

	switch (_eType) {
	case enumUseItem:
		CS_BeginAction(GetActor()->GetCha(), enumACTION_ITEM_USE, (void*)_pUseItem);
		//	2008-7-30	yangyinyu	add	begin!
		//	
		//	1	
		//	2008-7-30	yangyinyu	add	end!
		break;
	case enumUnfix:
		CS_BeginAction(GetActor()->GetCha(), enumACTION_ITEM_UNFIX, (void*)_pUnfix);
		break;
	}
	return true;
}

void CEquipState::SetUnfixData(stNetItemUnfix& data) {
	memcpy(_pUnfix, &data, sizeof(*_pUnfix));
	_eType = enumUnfix;
}

void CEquipState::SetUseItemData(stNetUseItem& data) {
	memcpy(_pUseItem, &data, sizeof(*_pUseItem));
	_eType = enumUseItem;
}
