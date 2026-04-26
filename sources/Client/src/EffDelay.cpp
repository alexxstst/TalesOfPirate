#include "StdAfx.h"
#include "effdelay.h"
#include "EffectObj.h"
#include "Scene.h"
#include "GameApp.h"
#include "Actor.h"
#include "SkillRecord.h"
#include "Character.h"
#include "HMManage.h"
#include "STAttack.h"

//---------------------------------------------------------------------------
// class CHitRepresent
//---------------------------------------------------------------------------
void CHitRepresent::Exec(CServerHarm* pHarm) {
	if (_pSkill->sAgroundEffectID > 0) {
		CGameApp::GetCurScene()->CreateEffect(_pSkill->sAgroundEffectID, _nAttackX, _nAttackY);
	}

	if (_pSkill->sWaterEffectID > 0 && (CGameApp::GetCurScene()->GetGridRegion(_nAttackX / 100, _nAttackY / 100) &
		enumAREA_TYPE_LAND) == 0) {
		CGameApp::GetCurScene()->CreateEffect(_pSkill->sWaterEffectID, _nAttackX, _nAttackY);
	}

	if (_pSkill->sWhoped != -1) CGameScene::PlayEnvSound(_pSkill->sWhoped, _nAttackX, _nAttackY);

	int nAngle = 999;
	if (_pAttack) nAngle = _pAttack->getYaw();
	if (_pSkill->IsAttackArea()) {
		if (pHarm) pHarm->HitRepresent(_pSkill, nAngle);
	}
	else if (_pTarget) {
		_pTarget->HitEffect(nAngle);
		if (_pSkill->sTargetEffectID > 0) _pTarget->SelfEffect(_pSkill->sTargetEffectID, _pSkill->sTargetDummyLink,
															   false, -1, nAngle);
	}

	if (pHarm) pHarm->Exec();
}

//---------------------------------------------------------------------------
// class CEffDelay
//---------------------------------------------------------------------------
CEffDelay::CEffDelay(CEffectObj* pOwn)
	: _pOwn(pOwn), _eStyle(enumNone) {
}

CEffDelay::~CEffDelay() {
}

void CEffDelay::Exec() {
	if (_eStyle == enumNone) return;

	switch (_eStyle) {
	case enumPos: {
		CEffectObj* pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
		if (!pEffect) return;

		if (!pEffect->Create(_nEffectID))
			return;

		pEffect->Emission(-1, &_Pos, NULL);
		pEffect->SetValid(TRUE);
	}
	break;
	case enumHitEffect: {
		_cHit.EffectExec(_pHarm);
	}
	break;
	}

	Reset();
}
