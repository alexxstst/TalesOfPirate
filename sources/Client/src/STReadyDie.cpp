#include "stdafx.h"
#include "streadydie.h"
#include "Actor.h"
#include "Character.h"
#include "CharacterRecord.h"
#include "GameApp.h"
#include "arctrack.h"
#include "uistartform.h"
#include "chastate.h"

//---------------------------------------------------------------------------
// CReadyDieState
//---------------------------------------------------------------------------
CReadyDieState::CReadyDieState(CActor* p)
	: CActionState(p), _state(enumDied), _IsActionEnd(false), _eDieState(enumInit)
	  , _nFallDownTime(0), _IsAlreadyEffect(false), _pAttack(NULL), _pArcTrack(NULL), _isFlyOff(false) {
	_pCha = GetActor()->GetCha();
}

CReadyDieState::~CReadyDieState() {
	if (_pArcTrack) delete _pArcTrack;
}

bool CReadyDieState::_Start() {
	if (!GetActor()->IsEnabled())
		return false;

	GetActor()->SetState(_state);
	_pCha->GetScene()->GetSign()->ChaDisappear(_pCha);

	// ,
	if (_pCha->IsMainCha()) {
		g_stUIStart.MainChaDied();
	}
	else if (_pAttack && _pCha->GetDefaultChaInfo()->chDieAction == 1) {
		if ((rand() % 9) == 0) {
			_pArcTrack = new CArcTrack;

			int x, y;
			int dis = rand() % 15 + 5;
			GetInDistancePos(int(_pCha->GetPos().x * 10.0f), int(_pCha->GetPos().y * 10.0f),
							 int(_pAttack->GetPos().x * 10.0f), int(_pAttack->GetPos().y * 10.0f), dis, x, y);
			D3DXVECTOR3 end;
			end.x = (float)x / 10.0f;
			end.y = (float)y / 10.0f;

			end.z = (float)_pCha->GetScene()->GetGridHeight(end.x, end.y);
			if (dis < 10)
				_pArcTrack->Start(_pCha->GetPos(), end, 5.0f, 0.3f);
			else
				_pArcTrack->Start(_pCha->GetPos(), end, 8.0f, 0.5f);

			_pCha->setYaw(GetLineAngle(_pAttack->GetCurX(), _pAttack->GetCurY(), _pCha->GetCurX(), _pCha->GetCurX()));

			_isFlyOff = true;
		}
	}
	_Died();
	return true;
}

void CReadyDieState::_Died() {
	_eDieState = enumDie;

	_pCha->GetActor()->ExecDied();

	_nDelayTime = CGameApp::GetFrameFPS() * 2;

	_pCha->DieTime();
	_pCha->PlayPose(POSE_DIE, PLAY_ONCE);

	if (_pCha->GetCurPoseKeyFrameNum() <= 0) {
		_pCha->SelfEffect(_pCha->GetDefaultChaInfo()->sDieEff);
		_IsAlreadyEffect = true;
	}
}

void CReadyDieState::ActionEnd(DWORD pose_id) {
	switch (_eDieState) {
	case enumDie: {
		_eDieState = enumFallDown;
		_pCha->PlayPose(POSE_FALLDOWN, PLAY_ONCE);
	}
	break;
	}
}

void CReadyDieState::FrameMove() {
	if (_isFlyOff) {
		_pArcTrack->FrameMove();

		_pCha->SetPos((float*)_pArcTrack->GetPos());
		if (_pArcTrack->IsEnd()) {
			int nCurX = int(_pArcTrack->GetPos().x * 100.0f);
			int nCurY = int(_pArcTrack->GetPos().y * 100.0f);

			_pCha->setPos(nCurX, nCurY);
			_isFlyOff = false;
		}
	}

	if (_eDieState == enumFallDown) {
		_nFallDownTime++;
		if (_nFallDownTime == _nDelayTime) {
			_pCha->PlayAni(_pCha->GetDefaultChaInfo()->nDiedBehave, defCHA_DIE_EFFECT_NUM);
			PopState();
		}
	}
}

void CReadyDieState::ActionFrame(DWORD pose_id, int key_frame) {
	switch (_eDieState) {
	case enumDie:
		if (!_IsAlreadyEffect) {
			_pCha->SelfEffect(_pCha->GetDefaultChaInfo()->sDieEff);
			_pCha->GetStateMgr()->ChaDied();
			_IsAlreadyEffect = true;
		}
		break;
	}
}

void CReadyDieState::_End() {
}
