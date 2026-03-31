#include "StdAfx.h"
#include "chastate.h"
#include "SkillStateRecord.h"
#include "netprotocol.h"
#include "character.h"
#include "EffectObj.h"

//---------------------------------------------------------------------------
// class CChaStateMgr
//---------------------------------------------------------------------------
CSkillStateRecord* CChaStateMgr::_pLastActInfo = nullptr;
int CChaStateMgr::_nShopLevel = 0;

CChaStateMgr::stChaState CChaStateMgr::_sInitState[SKILL_STATE_MAXID];
bool CChaStateMgr::_IsInit = false;

CChaStateMgr::CChaStateMgr(CCharacter* pCha)
: _pCha(pCha)
{
	if( !_IsInit )
	{
		_IsInit=true;

		memset( &_sInitState, 0, sizeof(_sInitState) );
		for( int i=1; i<SKILL_STATE_MAXID; i++ )
		{
			_sInitState[i].pInfo = GetCSkillStateRecordInfo( i );
		}
	}
	memcpy( _sChaState, _sInitState, sizeof(_sChaState) );
}

void CChaStateMgr::ChaDestroy()
{
	for (const auto& _state : _states)
	{
		if(_state->pEffect ) 
		{
			{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(29), _state->pEffect->getIdxID(), _state->pEffect); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }

			_state->pEffect->SetValid( FALSE );
			_state->pEffect = nullptr;
		}
	}

	memcpy( _sChaState, _sInitState, sizeof(_sChaState) );
	_states.clear();
}

CBoolSet& CChaStateMgr::Synchro(const stSkillState* pState, int nCount) {
	static CBoolSet _ChaState;
	static stChaState* stTmp[SKILL_STATE_MAXID] = { nullptr };
	static unsigned int nTmpCount = 0;

	// 
	static bool IsExist[SKILL_STATE_MAXID] = {};
	memset(IsExist, 0, sizeof(IsExist));

	static int nID = 0;
	nTmpCount = 0;
	for (const auto& _state : _states)
	{
		_state->IsDel = true;

		nID = _state->pInfo->nID;
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
			{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(30), pChaState->pInfo->nID, pChaState->pInfo->szName.c_str(), pChaState->pInfo->sEffect); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }

			// Existing delete
			if (pChaState->pEffect) {
				{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(31), pChaState->pEffect->getIdxID(), pChaState->pEffect); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }

				pChaState->pEffect->SetValid(FALSE);
				pChaState->pEffect = nullptr;
			}
			pChaState->chStateLv = 0;
		}
		else {
			// increase
			_states.push_back(pChaState);
			{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(32), pChaState->pInfo->nID, pChaState->pInfo->szName.c_str(), pChaState->pInfo->sEffect); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }

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

			if (pInfo->nID == 96) //@mothannakh dw 1 boss freeze stun fix
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
			else if (pInfo->nID == 159) // flash stun bug
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
			else if (pInfo->nID == 45) // Primal Rage/ stun bug
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->nID == 98) // death night
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->nID == 116) //  Black Dragon Terror
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->nID == 87) // Algae Entanglement
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}
			else if (pInfo->nID == 86) // Tornado
			{
				_ChaState.SetFalse(enumChaStateMove);
				_ChaState.SetFalse(enumChaStateAttack);
				_ChaState.SetFalse(enumChaStateUseSkill);
				_ChaState.SetFalse(enumChaStateTrade);
				_ChaState.SetFalse(enumChaStateUseItem);
				_pCha->_isArrive = true;
			}

			if (pInfo->nID == 99) {
				_nShopLevel = pChaState->chStateLv;
				_ChaState.SetFalse(enumChaStateNoShop);
			}

			if (pInfo->sEffect > 0 && !pChaState->pEffect) {
				pChaState->pEffect = _pCha->SelfEffect(pInfo->sEffect, pInfo->sDummy1, true);
				{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(33), pInfo->sEffect, pInfo->sDummy1, pChaState->pEffect); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
			}
		}
	}
	return _ChaState;
}
