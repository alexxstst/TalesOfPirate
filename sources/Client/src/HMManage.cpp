#include "stdafx.h"
#include "HMManage.h"
#include "SkillRecord.h"
#include "STStateObj.h"
#include "Actor.h"
#include "HMAttack.h"
#include "Character.h"
#include "GameApp.h"

static CStateSynchro* pSynchro = NULL;

const int UNKNOWN_FIGHT_ID = ERROR_FIGHT_ID - 1;

//---------------------------------------------------------------------------
// class CServerHarm
//---------------------------------------------------------------------------
CServerHarm::CServerHarm(CActor* pActor)
	: _pActor(pActor) {
	_pCha = _pActor->GetCha();

	InitMemory();
}

CServerHarm::~CServerHarm() {
}

void CServerHarm::InitMemory() {
	_IsOuter = false;
	_pSkill = NULL;
	_nFightID = UNKNOWN_FIGHT_ID;
	_nReadyExec = 0;
}

void CServerHarm::Gouge(int nGouge) {
	//if( _harm.empty() ) return;

	//float fRate = (float)nGouge;
	//fRate = 1.0f - fRate / (fRate + 1.0f);

	//if( _pSkill->IsAttackArea() )
	//{
	//    static CStateSynchro* pSynchro = NULL;
	//    while( !_harm.empty() )
	//    {
	//        pSynchro = _harm.front()->Gouge( fRate );
	//        if( pSynchro )
	//        {
	//            pSynchro->Exec();
	//            delete pSynchro;
	//        }
	//    }
	//}
	//else
	//{
	//    CStateSynchro* pSynchro = _harm.back()->Gouge( fRate );
	//    if( pSynchro )
	//    {
	//        pSynchro->Exec();
	//        // delete pSynchro;
	//    }
	//}
}

void CServerHarm::ExecAll(int nCount) {
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("CServerHarm ExecAll(int), FightID[{}], Ready[{}], Size[{}], Count[{}]",
										 GetFightID(), _nReadyExec, _harm.size(), nCount));

	while (!_harm.empty() && nCount > 0) {
		DWORD time = CGameApp::GetCurTick();

		pSynchro = _harm.front();
		_harm.pop_front();

		pSynchro->Exec(time);
		nCount--;

		time += 50;
	}
}

void CServerHarm::ExecAll() {
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("CServerHarm ExecAll, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
										 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));

	while (!_harm.empty()) {
		pSynchro = _harm.front();
		_harm.pop_front();

		pSynchro->Exec();
	}
}

void CServerHarm::Exec() {
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("CServerHarm Exec, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
										 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));

	if (GetIsExecEnd()) return;

	if (_harm.size() > 0 && _nReadyExec <= 0) {
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format(
									 "!!!!!!!!!!!!!!!!!CServerHarm Exec, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
									 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));
	}

	_nReadyExec--;

	if (!_harm.empty()) {
		if (_pSkill->IsHarmRange()) {
			ExecAll();
		}
		else {
			while (!_harm.empty()) {
				DWORD time = CGameApp::GetCurTick();

				pSynchro = _harm.front();
				_harm.pop_front();

				pSynchro->Exec(time);

				time += 50;
			}
			//pSynchro = _harm.front();
			//_harm.pop_front();

			//pSynchro->Exec();
		}
	}

	if (!_IsOuter && _harm.empty() && _nReadyExec <= 0) {
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("CServerHarm Over, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
											 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));
		_nFightID = ERROR_FIGHT_ID;
	}
}

void CServerHarm::HitRepresent(CSkillRecord* pSkill, int nAngle) {
	CCharacter* pCha = NULL;
	for (synchros::iterator it = _harm.begin(); it != _harm.end(); it++) {
		pCha = (*it)->GetTarget();
		if (pCha) {
			pCha->HitEffect(nAngle);
			if (pSkill->sTargetEffectID > 0) pCha->SelfEffect(pSkill->sTargetEffectID, pSkill->sTargetDummyLink, false,
															  -1, nAngle);
		}
	}
}

bool CServerHarm::AddRep(CAttackRepSynchro* s) {
	if (_pSkill->IsPlayCyc()) return false;

	return false;
}

bool CServerHarm::IsAllowStateOver() {
	if (_harm.empty()) return true;

	if (!_pSkill->IsHarmRange()) {
		return _nReadyExec >= (int)_harm.size();
	}

	return true;
}

void CServerHarm::SetIsOuter(bool v) {
	if (GetIsExecEnd()) return;

	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format(
								 "CServerHarm SetIsOuter, FightID[{}], Ready[{}], Size[{}], IsOuter[{}], OldOuter[{}]",
								 GetFightID(), _nReadyExec, _harm.size(), v, _IsOuter));

	_IsOuter = v;
	if (!v) {
		if (_nReadyExec > 0 && _pSkill) {
			if (_pSkill->IsHarmRange()) {
				// 
				return;
			}
			else if (_nReadyExec < (int)_harm.size()) {
				// 
				ExecAll((int)_harm.size() - _nReadyExec);
				return;
			}
		}

		if (!_harm.empty()) ExecAll();

		// ,
		if (_nReadyExec <= 0) {
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("CServerHarm Over(Outer), FightID[{}]", GetFightID()));
			_nFightID = ERROR_FIGHT_ID;
		}
		return;
	}
}

bool CServerHarm::AddHarm(CAttackEffect* s, CSkillRecord* pSkill) {
	if (pSkill != _pSkill) {
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("AddHarm skill false, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
											 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));
		return false;
	}

	if (_pSkill->IsNoHarm()) return false;

	if (GetIsExecEnd()) return false;

	if (!_IsOuter) {
		if (!_pSkill->IsHarmRange()) {
			if (_nReadyExec <= (int)_harm.size()) {
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format(
											 "AddHarm attrange false, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
											 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));
				return false;
			}
		}
		else if (_nReadyExec <= 0) {
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("AddHarm false, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]",
												 GetFightID(), _nReadyExec, _harm.size(), _IsOuter));
			return false;
		}
	}

	s->SetServerHarm(this);
	s->Reset();
	_harm.push_back(s);

	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("AddHarm, FightID[{}], Ready[{}], Size[{}], IsOuter[{}]", GetFightID(),
										 _nReadyExec, _harm.size(), _IsOuter));
	return true;
}
