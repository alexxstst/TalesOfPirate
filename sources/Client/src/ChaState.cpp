#include "StdAfx.h"
#include "chastate.h"
#include "SkillStateRecord.h"
#include "SkillStateRecordStore.h"
#include "netprotocol.h"
#include "character.h"
#include "EffectObj.h"

//---------------------------------------------------------------------------
// class CChaStateMgr
//---------------------------------------------------------------------------
CSkillStateRecord* CChaStateMgr::_pLastActInfo = nullptr;
int CChaStateMgr::_nShopLevel = 0;

CChaStateMgr::CChaStateMgr(CCharacter* pCha)
	: _pCha(pCha) {
	_ResetStates();
}

void CChaStateMgr::ChaDestroy() {
	for (const auto& _state : _states) {
		if (_state->pEffect) {
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(29), _state->pEffect->getIdxID(), (void*)_state->pEffect));

			_state->pEffect->SetValid(FALSE);
			_state->pEffect = nullptr;
		}
	}

	_ResetStates();
	_states.clear();
}

void CChaStateMgr::_ResetStates() {
	_sChaState.fill({});
	SkillStateRecordStore::Instance()->ForEach([this](CSkillStateRecord& rec) {
		if (rec.Id > 0 && rec.Id < static_cast<int>(_sChaState.size()))
			_sChaState[rec.Id].pInfo = &rec;
	});
}

CBoolSet& CChaStateMgr::Synchro(const stSkillState* pState, int nCount) {
	static CBoolSet _ChaState;
	static stChaState* stTmp[SKILL_STATE_MAXID] = {nullptr};
	static unsigned int nTmpCount = 0;

	//
	static bool IsExist[SKILL_STATE_MAXID] = {};
	memset(IsExist, 0, sizeof(IsExist));

	static int nID = 0;
	nTmpCount = 0;
	for (const auto& _state : _states) {
		_state->IsDel = true;

		nID = _state->pInfo->Id;
		if (!IsExist[nID]) {
			IsExist[nID] = true;

			stTmp[nTmpCount++] = _state;
		}
	}

	stChaState* pChaState = nullptr;
	for (int i = 0; i < nCount; i++) {
		nID = pState[i].chID;
		if (nID < 0 || nID >= SKILL_STATE_MAXID) {
			continue;
		}

		pChaState = &_sChaState[nID];
		if (pChaState->pInfo && pState[i].chLv > 0) {
			pChaState->IsDel = false;
			pChaState->chStateLv = pState[i].chLv;
			pChaState->lTimeRemaining = pState[i].lTimeRemaining;

			if (!IsExist[nID]) {
				IsExist[nID] = true;

				stTmp[nTmpCount++] = pChaState;
			}
		}
	}

	_states.clear();

	CCharacter::hits& _hits = _pCha->_hits;
	_hits.clear();

	_ChaState.AllTrue();
	CSkillStateRecord* pInfo = nullptr;
	for (unsigned int i = 0; i < nTmpCount; i++) {
		pChaState = stTmp[i];
		if (pChaState->IsDel) {
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(30), pChaState->pInfo->Id, pChaState->pInfo->szName, static_cast<int>(pChaState->pInfo->sEffect)));

			// Existing delete
			if (pChaState->pEffect) {
				g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(31), pChaState->pEffect->getIdxID(), (void*)pChaState->pEffect));

				pChaState->pEffect->SetValid(FALSE);
				pChaState->pEffect = nullptr;
			}
			pChaState->chStateLv = 0;
		}
		else {
			// increase
			_states.push_back(pChaState);
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(32), pChaState->pInfo->Id, pChaState->pInfo->szName, static_cast<int>(pChaState->pInfo->sEffect)));

			pInfo = pChaState->pInfo;
			if (pInfo->sBitEffect > 0) {
				_hits.push_back(CCharacter::stHit(pInfo->sBitEffect, pInfo->sDummy2));
			}

			if (!pInfo->bCanMove)
				_ChaState.SetFalse(enumChaStateMove);
			if (!pInfo->bCanGSkill)
				_ChaState.SetFalse(enumChaStateAttack);
			if (!pInfo->bCanMSkill)
				_ChaState.SetFalse(enumChaStateUseSkill);
			if (!pInfo->bCanTrade)
				_ChaState.SetFalse(enumChaStateTrade);
			if (!pInfo->bCanItem)
				_ChaState.SetFalse(enumChaStateUseItem);
			if (!pInfo->bNoHide)
				_ChaState.SetFalse(enumChaStateNoHide);
			if (pInfo->IsDizzy)
				_ChaState.SetFalse(enumChaStateNoDizzy);
			if (pInfo->GetActNum() > 0) {
				_pLastActInfo = pInfo;
				_ChaState.SetFalse(enumChaStateNoAni);
			}

			if (pInfo->Id == 96) //@mothannakh dw 1 boss freeze stun fix
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_ChaState.SetFalse(enumChaStateNoHide);
				_ChaState.SetFalse(enumChaStateNoDizzy);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 159) // flash stun bug
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_ChaState.SetFalse(enumChaStateNoHide);
				_ChaState.SetFalse(enumChaStateNoDizzy);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 45) // Primal Rage/ stun bug
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 98) // death night
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 116) //  Black Dragon Terror
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 87) // Algae Entanglement
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->Id == 86) // Tornado
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}

			if (pInfo->Id == 99) {
				_nShopLevel = pChaState->chStateLv;
				_ChaState.SetFalse(enumChaStateNoShop);
			}

			if (pInfo->sEffect > 0 && !pChaState->pEffect) {
				pChaState->pEffect = _pCha->SelfEffect(pInfo->sEffect, pInfo->sDummy1, true);
				g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(33), static_cast<int>(pInfo->sEffect), static_cast<int>(pInfo->sDummy1), (void*)pChaState->pEffect));
			}
		}
	}
	return _ChaState;
}
