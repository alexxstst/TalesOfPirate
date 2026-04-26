#include "StdAfx.h"
#include "stseat.h"
#include "actor.h"
#include "Character.h"
#include "PacketCmd.h"
#include "NetProtocol.h"
#include "CharacterRecord.h"
#include "stmove.h"

//---------------------------------------------------------------------------
// class CSeatState
//---------------------------------------------------------------------------
CSeatState::CSeatState(CActor* p)
	: CActionState(p), _nHeight(ERROR_POSE_HEIGHT), _nPose(0), _nAngle(0), _nPosX(0), _nPosY(0), _IsSeat(false) {
	_IsOver = true;
	_IsSend = true;
}

bool CSeatState::_Start() {
	_pCha = GetActor()->GetCha();

	if (_pCha->GetIsMountEquipped() && _pCha->GetIsOnMount()) {
		_pCha->DespawnMount();
	}

	if (_IsSend) {
		if (GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _nPosX, _nPosY) > _pCha->GetDefaultChaInfo()->sRadii +
			101) {
			return false;
		}


		CGameScene* pScene = _pCha->GetScene();
		int count = pScene->GetChaCnt();
		CCharacter* pCha = NULL;
		for (int i = 0; i < count; i++) {
			pCha = pScene->GetCha(i);
			if (pCha->IsValid() && pCha->IsEnabled()) {
				if (pCha->GetCurX() == _nPosX && pCha->GetCurY() == _nPosY)
					return false;
			}
		}

		stNetLeanInfo lean;
		lean.lPose = _nPose;
		lean.lPosX = _nPosX;
		lean.lPosY = _nPosY;
		lean.lHeight = _nHeight;
		lean.lAngle = _nAngle;

		CS_BeginAction(GetActor()->GetCha(), enumACTION_LEAN, &lean, this);
	}

	_nOldChaHeight = _pCha->getPoseHeightOff();
	_nOldX = _pCha->GetCurX();
	_nOldY = _pCha->GetCurY();

	if (_nPosX != _pCha->GetCurX() || _nPosY != _pCha->GetCurY()) {
		_pCha->PlayPose(POSE_RUN, PLAY_LOOP_SMOOTH);
		_pCha->MoveTo(_nPosX, _nPosY);
		_pCha->FaceTo(_nAngle);
	}
	return true;
}

void CSeatState::_End() {
	_pCha->setPoseHeightOff(_nOldChaHeight);
	if (_pCha->GetIsMountEquipped() && !_pCha->GetIsOnMount()) {
		_pCha->SpawnMount(_pCha->GetIsMountEquipped());
	}
	if (GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _nOldX, _nOldY) > 0) {
		// 
		CWaitMoveState* state = new CWaitMoveState(GetActor());
		_pCha->MoveTo(_nOldX, _nOldY);

		state->SetIsSend(false);
		state->MoveEnd(_nOldX, _nOldY, 0);
		GetActor()->InsertState(state);
	}
}

void CSeatState::FrameMove() {
	if (!_pCha->GetIsArrive())
		return;

	if (!_pCha->GetIsFaceTo())
		return;

	if (!_IsSeat) {
		_pCha->setPos(_nPosX, _nPosY);
		_pCha->setYaw(_nAngle);
		// if( _nHeight!=ERROR_POSE_HEIGHT ) 
		{
			_pCha->setPoseHeightOff(_nHeight);
		}

		_pCha->PlayPose(_nPose, PLAY_LOOP);

		_IsSeat = true;
	}
}
