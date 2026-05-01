#include "stdafx.h"
#include "ChaRecordStore.h"
#include "STAttack.h"
#include "SteadyFrameSync.h"
#include "Actor.h"
#include "NetProtocol.h"
#include "STMove.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "SkillRecord.h"
#include "EffectObj.h"
#include "Scene.h"
#include "terrainattrib.h"
#include "HMAttack.h"
#include "SceneItem.h"
#include "UICommand.h"
#include "effdelay.h"
#include "HMManage.h"
#include "UITeam.h"
#include "uiskillcommand.h"
#include "uiequipform.h"
#include "uiitemcommand.h"
#include "uiboatform.h"
#include "isskilluse.h"
#include "UIBoothForm.h"
#include "cameractrl.h"
#include "state_reading.h"

using namespace std;

//---------------------------------------------------------------------------
// class CWaitAttackState
//---------------------------------------------------------------------------
CWaitAttackState::CWaitAttackState(CActor* p)
	: CActionState(p), _pSkillInfo(NULL), _pTarget(NULL), _eUseSkill(enumInit), _nAngle(0)
	  , _IsHaveAngle(false), _nKeyFrameNum(1), _pSelf(NULL), _nSkillActionIndex(0)
	  , _nAttackState(0), _nSkillSpeed(0), _nActionEnd(0), _fSkillRate(1.0f) {
	_pSelf = GetActor()->GetCha();
	_cHit.SetAttack(_pSelf);

	_IsSend = false;

	_pHarm = GetActor()->CreateHarmMgr();
	_pHarm->SetIsOuter(true);

	_nSkillSpeed = _pSelf->getAttackSpeed();
}

CWaitAttackState::~CWaitAttackState() {
	_pHarm->SetIsOuter(false);
}

void CWaitAttackState::End() {
	CActionState::End();
}

void CWaitAttackState::SetTarget(CCharacter* p) {
	_pTarget = p;
	_cHit.SetTarget(p);
	SetAttackPoint(p->GetServerX(), p->GetServerY());
}

void CWaitAttackState::SetServerID(int n) {
	_nServerID = n;
	_pHarm->SetFightID(n);
}

void CWaitAttackState::SetSkill(CSkillRecord* p) {
	if (!p) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(398));
	}

	_pSkillInfo = p;
	_pHarm->SetSkill(p);
	_cHit.SetSkill(p);
}

void CWaitAttackState::CalcSkillSpeed() {
	// 
	if (!_pSkillInfo->IsPlayRand()) {
		static int nCount = 0;
		static MPPoseInfo* pInfo = NULL;
		static int frame[defSKILL_POSE_NUM] = {1};
		static int total_frame = 0;
		nCount = _pSkillInfo->GetPoseNum();
		total_frame = 0;
		for (int i = 0; i < nCount; i++) {
			pInfo = _pSelf->GetPoseInfoBig(_pSkillInfo->sActionPose[i]);
			if (!pInfo) continue;

			frame[i] = pInfo->end - pInfo->start + 1;
			total_frame += frame[i];
		}
		float fAverage = (float)total_frame / (float)nCount;
		for (int i = 0; i < nCount; i++) {
			_fSkillSpeed[i] = fAverage / (float)frame[i];

			if (_fSkillSpeed[i] < 0.1f) _fSkillSpeed[i] = 0.1f; // 0.1f
			if (_fSkillSpeed[i] > 10.0f) _fSkillSpeed[i] = 10.0f; // 10.0f
		}
	}
}

bool CWaitAttackState::_Start() {
	// 
	CalcSkillSpeed();
	_StartActor();
	return true;
}

void CWaitAttackState::_End() {
	//if( _pTarget && _pTarget->IsEnabled() )
	//{
	//	if( _nAttackState & enumFSTATE_TARGET_DIE )
	//	{
	//		CAttackEffect::ChaDied( _pTarget, _pSelf );
	//	}
	//}

	// if( _IsHaveAngle ) _pSelf->FaceTo( _nAngle );
}

void CWaitAttackState::_StartActor() {
	// Turn to the target
	if (_pSelf->IsBoat() || _pSkillInfo->GetDistance() <= 0) {
		_UseSkill();
		return;
	}

	_eUseSkill = enumStart;

	if (_pTarget) {
		if (_pTarget == _pSelf) {
			_UseSkill();
			return;
		}

		_pSelf->FaceTo(_pTarget->GetCurX(), _pTarget->GetCurY());
	}
	else {
		_pSelf->FaceTo(_nAttackX, _nAttackY);
	}
}

void CWaitAttackState::_UseSkill() {
	// Make an attack
	_eUseSkill = enumUseSkill;

	_nSkillPoseID = _GetPoseID();

	_pSelf->SwitchFightPose();

	if (_pSkillInfo->IsPlayCyc()) {
		_pSelf->PlayPose(_nSkillPoseID,
						 (_pSkillInfo->IsPlayCyc() && _pSkillInfo->GetPoseNum() > 1) ? PLAY_ONCE : PLAY_LOOP,
						 _nSkillSpeed, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps());
		if (!_pSkillInfo->IsPlayRand()) {
			_pSelf->SetPoseVelocity(_pSelf->GetPoseVelocity() * _fSkillRate);
		}
	}
	else {
		if (_pSelf->IsBoat()) {
			_pSelf->PlayPose(_nSkillPoseID, PLAY_ONCE, 100, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps());
		}
		else {
			_pSelf->PlayPose(_nSkillPoseID, PLAY_ONCE, _nSkillSpeed, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps()); //  mdr.st

			//  Минимальная PoseVelocity для атаки: 1.0 на 30 FPS, 0.5 на 60 FPS,
			//  0.25 на 120 FPS — нормализованная под текущий FPS относительно 30-FPS-референса.
			const float per = 1.0f / Corsairs::Client::Frame::SteadyFrameSync::Instance().GetAnimMultiplier();
			if (_pSelf->GetPoseVelocity() < per); // 1.0f
			{
				_pSelf->SetPoseVelocity(per); // 1.0f
			}
		}
	}

	_nKeyFrameNum = _pSelf->GetCurPoseKeyFrameNum();
	if (_nKeyFrameNum <= 0 && _pSkillInfo->sActionKeyFrme != -1) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(399), _pSelf->getLogName(), _pSkillInfo->szName.c_str(),
											 _nSkillPoseID));
	}

	// If there are additional special effects operations, such as: boarding and disembarking, etc., a special effect should be played
	for (int i = 0; i < defSKILL_OPERATE_NUM; i++) {
		if (_pSkillInfo->chOperate[i] != 0) {
			_pSelf->OperatorEffect(_pSkillInfo->chOperate[i], _nAttackX, _nAttackY);
		}
		else {
			break;
		}
	}
}

void CWaitAttackState::ActionFrame(DWORD pose_id, int key_frame) {
	// key_frame
	// ACTION_BEGIN_HIT = -1 
	// ACTION_END_HIT = -2 
	// 

	//if( !_IsPoseValid(pose_id) )
	//{
	//	return;
	//}

	if (key_frame > 0) key_frame = 0;

	// 
	if (_pSkillInfo->Id == 83 && key_frame >= 0) {
		g_pGameApp->GetMainCam()->SetCameraDither(TRUE);
	}

	// 
	if (_pSkillInfo->sActionKeyFrme == key_frame) {
		if (_pSkillInfo->sWhop != -1)
			CGameScene::PlayEnvSound(_pSkillInfo->sWhop, _pSelf->GetCurX(), _pSelf->GetCurY());

		for (int i = 0; i < defSKILL_ACTION_EFFECT; i++) {
			if (_pSkillInfo->sActionEffect[i] > 0) {
				if (_pSkillInfo->sApplyDistance == 0 && _pSkillInfo->sActionEffectType[i] == 1 && _pSkillInfo->
					GetShape() == enumRANGE_TYPE_CIRCLE) {
					_pSelf->SelfEffect(_pSkillInfo->sActionEffect[i], _pSkillInfo->sActionDummyLink[i], false,
									   _pSkillInfo->GetRange(), _pSelf->getYaw());
				}
				else {
					_pSelf->SelfEffect(_pSkillInfo->sActionEffect[i], _pSkillInfo->sActionDummyLink[i], false, -1,
									   _pSelf->getYaw());
				}
			}
			else {
				break;
			}
		}
	}

	if (_pSkillInfo->sItemEffect1[0] > 0 && _pSkillInfo->sItemEffect1[1] == key_frame) _pSelf->ItemEffect(
		_pSkillInfo->sItemEffect1[0], _pSkillInfo->sItemDummyLink, _pSelf->getYaw());
	if (_pSkillInfo->sItemEffect2[0] > 0 && _pSkillInfo->sItemEffect2[1] == key_frame) _pSelf->ItemEffect(
		_pSkillInfo->sItemEffect2[0], _pSkillInfo->sItemDummyLink, _pSelf->getYaw());

	// 
	if (_pSkillInfo->sSkyEffect != 0 && _pSkillInfo->sSkyEffectActionKeyFrame == key_frame) {
		int nTargetID = -1;

		static D3DXVECTOR3 pos;
		pos.x = (float)_nAttackX / 100;
		pos.y = (float)_nAttackY / 100;
		pos.z = CGameApp::GetCurScene()->GetGridHeight(pos.x, pos.y);

		if (_pTarget) {
			nTargetID = _pTarget->getID();

			static lwMatrix44 mat;
			if (_pSkillInfo->sTargetDummyLink >= 0 && _pTarget->GetObjDummyRunTimeMatrix(
				&mat, _pSkillInfo->sTargetDummyLink) >= 0) {
				pos = *(D3DXVECTOR3*)&mat._41;
			}
		}

		if (CEffectObj* pEff = _pSelf->SkyEffect(_pSkillInfo->sSkyEffect, _pSkillInfo->sSkyEffectActionDummyLink,
												 _pSkillInfo->sSkyEffectItemDummyLink, _pSkillInfo->sSkySpd, &pos,
												 nTargetID, _pSkillInfo)) {
			// ,
			// 
			pEff->GetEffDelay()->SetServerHarm(_cHit, _pHarm);
		}
		else {
			// 
			_cHit.EffectExec(_pHarm);
		}
	}

	// 
	_cHit.ActionExec(_pHarm, key_frame);

	// 
	if (_pTarget && key_frame >= 0 && _pTarget->getChaCtrlType() == enumCHACTRL_MONS_TREE) {
		_pTarget->PlayPose(7, PLAY_ONCE_SMOOTH);
	}

	//if( _pSkillInfo->IsPlayCyc() && key_frame>=0 && (_nAttackState & enumFSTATE_CANCEL) && _pSelf->IsPlayer() )
	//{
	//	_PopThis();
	//}
}

void CWaitAttackState::ActionBegin(DWORD pose_id) {
	// 

	//if( !_IsPoseValid(pose_id) )
	//{
	//    return;
	//}

	_pHarm->ReadyExec();
	ActionFrame(pose_id, ACTION_BEGIN_HIT);
}

void CWaitAttackState::_PopThis() {
	if (!_pSkillInfo->IsPlayCyc()) {
		_pHarm->SetIsOuter(false);
		PopState();
	}
	else if (_pHarm->IsAllowStateOver()) {
		_pHarm->SetIsOuter(false);
		PopState();
	}
	return;
}

void CWaitAttackState::ActionEnd(DWORD pose_id) {
	// 

	//if( !_IsPoseValid(pose_id) )
	//{
	//	return;
	//}

	if (_IsPoseValid(pose_id)) {
		_nActionEnd++;
		if (_pSkillInfo->IsPlayCyc() || _nActionEnd == 1) {
			if (_pSelf->IsMainCha()) g_logManager.LogDebug("common", "ActionEnd: {}", pose_id);
			_cHit.ActionExec(_pHarm, ACTION_END_HIT);
		}
	}

	if (_IsOver) {
		_PopThis();
	}
	else if (_eUseSkill == enumUseSkill && !g_NetIF->IsConnected()) {
		_IsOver = true;
	}

	if (GetIsExecEnd()) return;

	if (_pSkillInfo->IsPlayCyc() && _pSkillInfo->GetPoseNum() > 1) {
		_nSkillPoseID = _GetPoseID();

		if (_pHarm->GetCount() >= 2) {
			int nTime = (int)((float)_nSkillSpeed * (1.0f - (float)_pHarm->GetCount() / 16.0f));
			if (nTime <= 0) nTime = 1;
			_pSelf->PlayPose(_nSkillPoseID, PLAY_ONCE, nTime, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps());
		}
		else {
			_pSelf->PlayPose(_nSkillPoseID, PLAY_ONCE, _nSkillSpeed, Corsairs::Client::Frame::SteadyFrameSync::Instance().GetFps());
		}

		if (!_pSkillInfo->IsPlayRand()) {
			_pSelf->SetPoseVelocity(_pSelf->GetPoseVelocity() * _fSkillRate);
		}
	}
}

void CWaitAttackState::FrameMove() {
	switch (_eUseSkill) {
	case enumStart:
		if (!_pSelf->GetIsFaceTo())
			return;

		_UseSkill();
		break;
	case enumUseSkill:
		if (!_pSelf->IsBoat() && _pTarget && _pTarget != _pSelf && _pSkillInfo->GetDistance() > 0) {
			_pSelf->FaceTo(_pTarget->GetCurX(), _pTarget->GetCurY());
		}
		break;
	}
}

void CWaitAttackState::MoveEnd(int x, int y, int nState) {
	// MoveEnd
}

void CWaitAttackState::ServerEnd(int nState) {
	CActionState::ServerEnd(nState);

	_nAttackState = nState;
}

//---------------------------------------------------------------------------
// class CAttackState
//---------------------------------------------------------------------------
CAttackState::stLastAttack CAttackState::_sLastTarget;
DWORD CAttackState::_dwLastAttackTime = 0;

CAttackState::CAttackState(CActor* p)
	: CWaitAttackState(p), _pMove(NULL), _nTotalDis(0), _IsMoveOver(false)
	  , _pCommand(NULL), _dwEndTime(0), _IsForce(false), _IsLocalCancel(false), _nActionKeyCount(0),
	  _nActionEndCount(0) {
	_IsSend = true;

	_pMove = new CServerMoveState(GetActor());
	_pMove->SetParent(this);
	_pMove->SetWalkLine(false);
	_pMove->SetIsSend(false);
}

CAttackState::~CAttackState() {
	if (_pMove) delete _pMove;
}

void CAttackState::_UseSkill() {
	if (_pCommand) {
		_pCommand->StartCommand();
	}

	CWaitAttackState::_UseSkill();
	_pSelf->StopMove();
	_pSkillInfo->SetAttackTime(CGameApp::GetCurTick() + _nSkillSpeed);

	_dwEndTime = CGameApp::GetCurTick() + _nSkillSpeed;
}

bool CAttackState::_Start() {
	if (_pSelf->GetDefaultSkillInfo() == _pSkillInfo) {
		_sLastTarget.pTarget = _pTarget;
		_sLastTarget.dwTime = CGameApp::GetCurTick();
	}
	else {
		_sLastTarget.Reset();
	}

	if (_pSelf->IsMainCha()) {
		// cooldown
		_pSelf->ResetReadySkill();
	}

	if (!IsAllowUse()) return false;

	if (!_pSkillInfo) return false;

	if (_pSkillInfo->sID == 241) {
		if (_pSkillInfo->GetLevel() > _pSkillInfo->GetJobMax(_pSelf->getGameAttr()->get(ATTR_JOB))) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(400), _pSkillInfo->GetLevel(),
												 _pSkillInfo->GetJobMax(_pSelf->getGameAttr()->get(ATTR_JOB))));
			return false;
		}

		// 
		g_stUIBooth.ShowSetupBoothForm(_pSkillInfo->GetLevel());
		PopState();
		return true;
	}
	//add by ALLEN 2007-10-16
	else if (_pSkillInfo->sID == 461) {
		if (CReadBookMgr::IsCanReadBook(GetActor()->GetCha())) {
			CReadBookMgr::ShowReadBookForm();

			PopState();

			CReadingState* readingState = new CReadingState(GetActor());
			GetActor()->SwitchState(readingState);

			return true;
		}
		else {
			return false;
		}
	}

	if (_pCommand && !_pCommand->IsAllowUse()) {
		return false;
	}

	// 
	if (_pSkillInfo->GetDistance() > 0) {
		_nTotalDis = _pSelf->GetDefaultChaInfo()->sRadii + _pSkillInfo->GetDistance();
		if (_pTarget) _nTotalDis += _pTarget->GetDefaultChaInfo()->sRadii;
	}
	else {
		_nTotalDis = 0;
	}

	// 
	if (_pSkillInfo->szFireSpeed == "0") {
		SetSkillSpeed(_pSelf->getAttackSpeed());
	}
	else {
		SetSkillSpeed(_pSkillInfo->GetFireSpeed());
	}

	if (_nSkillSpeed == 0) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(401));
	}

	stNetSkillInfo param;
	param.lSkillID = _pSkillInfo->sID;
	bool isFindPath = false;

	// ,,
	if (_pSkillInfo->GetDistance() == 0) {
		_nAttackX = _pSelf->GetServerX();
		_nAttackY = _pSelf->GetServerY();
		_UseSkill();
	}
	else {
		if (_pTarget) {
			if (_pSkillInfo->IsAttackArea()) {
				// 
				int dis = GetDistance(_pTarget->GetCurX(), _pTarget->GetCurY(), _pTarget->GetServerX(),
									  _pTarget->GetServerY());
				int PRE_ATTACK = 100;
				if (dis <= PRE_ATTACK) {
					_nAttackX = _pTarget->GetServerX();
					_nAttackY = _pTarget->GetServerY();
				}
				else {
					GetDistancePos(_pTarget->GetCurX(), _pTarget->GetCurY(), _pTarget->GetServerX(),
								   _pTarget->GetServerY(), PRE_ATTACK, _nAttackX, _nAttackY);
				}

				// _pTarget = NULL;		
			}
			else {
				_nAttackX = _pTarget->GetServerX();
				_nAttackY = _pTarget->GetServerY();
			}
		}

		_pMove->SetMoveTo(_nAttackX, _nAttackY);
		_pMove->Start();

		if (!_pMove->GetIsInit()) {
			if (GetDistance(_nAttackX, _nAttackY, _pSelf->GetServerX(), _pSelf->GetServerY()) <= _nTotalDis) {
				_StartActor();
			}
			else {
				return false;
			}
		}
		else {
			isFindPath = true;
			memcpy(&param.SMove, &_pMove->GetPathInfo(), sizeof(param.SMove));
		}
	}

	param.chMove = 2;
	if (!isFindPath) {
		param.SMove.pos_num = 2;
		param.SMove.pos_buf[0].x = _pSelf->GetServerX();
		param.SMove.pos_buf[0].y = _pSelf->GetServerY();
		param.SMove.pos_buf[1].x = _nAttackX;
		param.SMove.pos_buf[1].y = _nAttackY;
	}
	param.SMove.dwAveragePing = g_NetIF->GetAveragePing();
	if (_pSkillInfo->IsAttackArea()) {
		param.lTarInfo1 = _nAttackX;
		param.lTarInfo2 = _nAttackY;
	}
	else {
		param.lTarInfo1 = _pTarget->getAttachID();
		param.lTarInfo2 = _pTarget->lTag;
	}
	SetAttackPoint(_nAttackX, _nAttackY);
	CS_BeginAction(_pSelf, enumACTION_SKILL, (void*)&param, this);

	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("FireSpeed:{}", _nSkillSpeed));
	_pSkillInfo->SetAttackTime(CGameApp::GetCurTick() + _nSkillSpeed); // 
	CalcSkillSpeed();

	_dwLastAttackTime = CGameApp::GetCurTick();

	// 
	if (_pSkillInfo->IsAttackArea()) {
		if (GetDistance(_nAttackX, _nAttackY, _pSelf->GetServerX(), _pSelf->GetServerY()) <= _nTotalDis) {
			_StartActor();
		}
	}
	else if (_pTarget) {
		if (_pTarget->DistanceFrom(_pSelf) <= _nTotalDis) {
			_StartActor();
		}
	}

	// 
	if (_eUseSkill == enumInit) {
		_pMove->ChaRun();
		_pMove->PreMove();
	}
	return true;
}

void CAttackState::FrameMove() {
	if (!_pMove->GetIsExecEnd()) {
		// 
		_pMove->FrameMove();
		if (_pMove->GetIsExecEnd()) {
			if (_IsMoveOver) {
				PopState();
				return;
			}
			if (_eUseSkill != enumUseSkill) {
				_StartActor();
			}
			return;
		}
	}
	else if (_eUseSkill == enumInit) {
		_StartActor();
	}

	if (_nAttackState && !_pSkillInfo->IsPlayCyc() && _nActionEndCount > 0) {
		PopState();
		return;
	}

	switch (_eUseSkill) {
	case enumUseSkill:
		if (!_pSelf->IsBoat() && _pTarget && _pTarget != _pSelf && _pSkillInfo->GetDistance() > 0 && !_IsMoveOver) {
			_pSelf->FaceTo(_pTarget->GetCurX(), _pTarget->GetCurY());
		}

		if (_nActionKeyCount > 0 && _IsLocalCancel && (_nAttackState || _IsMoveOver)) {
			if (_pSkillInfo->IsEffectHarm() || _dwEndTime <= CGameApp::GetCurTick()) {
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("change pose velocity:{}, actionkeycount:{}",
													 _pSelf->GetPoseVelocity(), _nActionKeyCount));

				_pSelf->SetPoseVelocity(_pSelf->GetPoseVelocity() * 100.0f);
				//_pSelf->SetPoseVelocity( 100.0f );
				_nActionKeyCount = -9999;
			}
		}
		return;
	case enumStart:
		if (_IsMoveOver) {
			PopState();
			return;
		}

		if (!_pSelf->GetIsFaceTo())
			return;

		_UseSkill();
		return;
	}
}

void CAttackState::PushPoint(int x, int y) {
	// 
	if (_eUseSkill == enumInit) {
		_pMove->PushPoint(x, y);
		return;
	}

	if (!_pSkillInfo->IsPlayCyc())
		return;

	int dis = GetDistance(_pSelf->GetCurX(), _pSelf->GetCurY(), x, y);
	if (dis < 200) return;

	_pMove->PushPoint(x, y);
	_pMove->ChaRun();
	_eUseSkill = enumInit;
}

void CAttackState::ActionBegin(DWORD pose_id) {
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("ActionBegin: {}", pose_id));
	CWaitAttackState::ActionBegin(pose_id);
}

void CAttackState::ActionEnd(DWORD pose_id) {
	if (_eUseSkill != enumUseSkill)
		return;

	CWaitAttackState::ActionEnd(pose_id);

	if (_IsMoveOver) {
		PopState();
	}
	_nActionEndCount++;
}

void CAttackState::ActionFrame(DWORD pose_id, int key_frame) {
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("ActionFrame:{}, keyframe:{}, pose velocity:{}", pose_id, key_frame,
										 _pSelf->GetPoseVelocity()));

	if (_eUseSkill != enumUseSkill)
		return;

	CWaitAttackState::ActionFrame(pose_id, key_frame);
	if (key_frame >= 0) _nActionKeyCount++;
}

void CAttackState::MoveEnd(int x, int y, int nState) {
	_pMove->MoveEnd(x, y, nState);

	// enumMSTATE_INRANGE,,, 
	if (nState == enumMSTATE_INRANGE) {
		_pSkillInfo->SetAttackTime(CGameApp::GetCurTick() + _nSkillSpeed);
	}
	else {
		_IsMoveOver = true;
	}
}

bool CAttackState::IsAllowUse() {
	if (!_pSelf->IsValid()) return false;

	if (_pTarget && !_pTarget->IsValid()) return false;

	if (!_pSkillInfo->IsPlayCyc() && !_pSkillInfo->IsAttackTime(CGameApp::GetCurTick()))
		return false;

	if (_IsForce) return true;

	if (!g_SkillUse.IsUse(_pSkillInfo, _pSelf, _pTarget)) {
		g_pGameApp->SysInfo(g_SkillUse.GetError());
		return false;
	}

	return true;
}

bool CAttackState::_IsAllowCancel() {
	bool rv = _AllowCancel();
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format(
								 "Attack Cancel- AllowCancel:{}, IsWait:{}, IsCancle:{}, IsOver:{}, FightID:{}, Skill:{}",
								 rv, GetIsWait(), GetIsCancel(), GetIsOver(), GetServerID(), _pSkillInfo->szName));
	return rv;
}

void CAttackState::Cancel() {
	_IsLocalCancel = true;

	if (!_IsAllowCancel()) return;
	_IsCancel = true;

	if (_IsSend) {
		if (!_pSkillInfo->IsPlayCyc()) {
			if (enumInit != _eUseSkill)
				return;
		}
		CS_EndAction(this);
	}
}

void CAttackState::_End() {
	CWaitAttackState::_End();

	//if( _nAttackState & enumFSTATE_TARGET_DIE )
	//{
	//	if( _pTarget )
	//	{
	//		CAttackEffect::ChaDied( _pTarget, _pSelf );
	//	}
	//}

	if (_nAttackState & enumFSTATE_DIE) {
		CAttackEffect::ChaDied(_pSelf, _pTarget);
	}
}

//---------------------------------------------------------------------------
// class CTraceAttackState
//---------------------------------------------------------------------------
CTraceAttackState::CTraceAttackState(CActor* p)
	: CAttackState(p), _IsTrace(true), _IsSuccess(false) {
}

CTraceAttackState::~CTraceAttackState() {
	if (!_IsSuccess) _sLastTarget.Reset();
}

void CTraceAttackState::StartFailed() {
	CheckTrace();
}

void CTraceAttackState::_End() {
	// 
	CAttackState::_End();
	CheckTrace();
}

void CTraceAttackState::CheckTrace() {
	if (_pSelf->GetDefaultSkillInfo() == _pSkillInfo && _IsTrace && _pTarget && _pTarget->IsEnabled() && _pTarget->
		IsValid()) {
		// 
		if (_pSelf->GetDefaultChaInfo()->chTerritory != _pTarget->GetDefaultChaInfo()->chTerritory) {
			if (!g_IsMoveAble(_pSelf->getChaCtrlType()
							  , _pSelf->GetDefaultChaInfo()->chTerritory
							  , _pSelf->GetScene()->GetGridRegion(_pTarget->GetCurX() / 100, _pTarget->GetCurY() / 100))
			)
				return;
		}

		if (!g_SkillUse.IsUse(_pSkillInfo, _pSelf, _pTarget)) {
			return;
		}

		CTraceAttackState* st = new CTraceAttackState(GetActor());
		st->SetSkill(_pSelf->GetDefaultSkillInfo());
		st->SetTarget(_pTarget);
		st->SetCommand(_pCommand);

		GetActor()->AddState(st);

		_IsSuccess = true;
		return;
	}
}

void CTraceAttackState::MoveEnd(int x, int y, int nState) {
	CAttackState::MoveEnd(x, y, nState);


	if ((nState & enumMSTATE_CANCEL)
		|| (enumMSTATE_NOTARGET & nState)
		|| (enumMSTATE_CANTMOVE & nState)
	) {
		_IsTrace = false;
	}
}

void CTraceAttackState::ServerEnd(int nState) {
	CAttackState::ServerEnd(nState);

	if ((enumFSTATE_CANCEL & nState)
		|| (enumFSTATE_TARGET_DIE & nState)
		|| (enumFSTATE_TARGET_IMMUNE & nState)
		|| (enumFSTATE_TARGET_NO & nState)
		|| (enumFSTATE_OFF & nState)
	) {
		_IsTrace = false;
	}
}

//---------------------------------------------------------------------------
// class CHitAttackState
//---------------------------------------------------------------------------
CHitAttackState::CHitAttackState(CActor* p)
	: CActionState(p) {
	_pSelf = GetActor()->GetCha();
	_nIndex = 0;

	memset(_nPose, 0, sizeof(_nPose));
	_nPoseNum = 0;
	_fSpeed = 1.0f;
}

CHitAttackState::~CHitAttackState() {
}

void CHitAttackState::ActionBegin(DWORD pose_id) {
	g_pGameApp->SysInfo(std::format("Pose [{}] ActionBegin:{}", _nCurPose, pose_id));
}

void CHitAttackState::ActionFrame(DWORD pose_id, int key_frame) {
	g_pGameApp->SysInfo(std::format("Pose [{}] ActionFrame ID:{} Key:{}", _nCurPose, pose_id, key_frame));
}

void CHitAttackState::ActionEnd(DWORD pose_id) {
	g_pGameApp->SysInfo(std::format("Pose [{}] ActionEnd:{}", _nCurPose, pose_id));

	_nIndex++;
	if (_nIndex >= _nPoseNum) {
		PopState();
		return;
	}
	_nCurPose = _nPose[_nIndex];
	_pSelf->PlayPose(_nCurPose, PLAY_ONCE);
	_pSelf->SetPoseVelocity(_fSpeed);
}

void CHitAttackState::_End() {
	g_pGameApp->SysInfo(GetLanguageString(402));
}

bool CHitAttackState::_Start() {
	if (_fSpeed <= 0.0001f) _fSpeed = 1.0f;
	if (_fSpeed > 1000.0f) _fSpeed = 1000.0f;

	std::string str = SafeVFormat(GetLanguageString(403), _nPoseNum, _fSpeed);
	for (int i = 0; i < _nPoseNum; i++) {
		std::format_to(std::back_inserter(str), " {}", _nPose[i]);
	}
	g_pGameApp->SysInfo(str);

	_nIndex = 0;
	_nCurPose = _nPose[_nIndex];
	_pSelf->PlayPose(_nCurPose, PLAY_ONCE);
	_pSelf->SetPoseVelocity(_fSpeed);
	return true;
}

//---------------------------------------------------------------------------
// class CAllPoseState
//---------------------------------------------------------------------------
CAllPoseState::CAllPoseState(CActor* p)
	: CActionState(p), _nCurPose(0), _fSpeed(100.0f), _nCurChaID(1) {
	_pSelf = GetActor()->GetCha();
}

CAllPoseState::~CAllPoseState() {
}

void CAllPoseState::ActionBegin(DWORD pose_id) {
	g_pGameApp->SysInfo(std::format("ChaID:[{}] Pose [{}] ActionBegin:{}", _nCurChaID, _nCurPose, pose_id));
}

void CAllPoseState::ActionFrame(DWORD pose_id, int key_frame) {
	g_pGameApp->SysInfo(std::format("ChaID:[{}] Pose [{}] ActionFrame ID:{} Key:{}",
									_nCurChaID, _nCurPose, pose_id, key_frame));
}

void CAllPoseState::ActionEnd(DWORD pose_id) {
	g_pGameApp->SysInfo(std::format("ChaID:[{}] Pose [{}] ActionEnd:{}", _nCurChaID, _nCurPose, pose_id));

	_nCurPose++;
	if (_nCurPose >= _nPoseEnd) {
		_nCurPose = _nPoseStart;

		for (;;) {
			_nCurChaID++;
			if (_nCurChaID >= _nChaEnd) {
				PopState();
				return;
			}
			else {
				CCharacter* pCha = _pSelf->GetScene()->AddCharacter(_nCurChaID);
				if (pCha) {
					pCha->setPos(_pSelf->GetCurX(), _pSelf->GetCurY());
					CCharacter* pOld = _pSelf;
					_pSelf = pCha;

					CAllPoseState* pState = new CAllPoseState(_pSelf->GetActor());
					pState->SetChaID(_nCurChaID);
					pState->SetChaRange(_nChaStart, _nChaEnd);
					_pSelf->GetActor()->AddState(pState);

					pOld->GetActor()->ExecAllNet();
					pOld->SetValid(FALSE);
					return;
				}
			}
		}
	}
	_pSelf->PlayPose(_nCurPose, PLAY_ONCE);
	_pSelf->SetPoseVelocity(_fSpeed);
}

void CAllPoseState::_End() {
	g_pGameApp->SysInfo(GetLanguageString(402));
}

bool CAllPoseState::_Start() {
	if (_fSpeed <= 0.0001f) _fSpeed = 1.0f;
	if (_fSpeed > 1000.0f) _fSpeed = 1000.0f;

	_nPoseStart = 1;
	_nPoseEnd = 54;
	_nChaStart = 1;
	_nChaEnd = ChaRecordStore::Instance()->GetMaxId() + 1;

	_nCurPose = _nPoseStart;

	_pSelf->PlayPose(_nCurPose, PLAY_ONCE);
	_pSelf->SetPoseVelocity(_fSpeed);
	return true;
}
