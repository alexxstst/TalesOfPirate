//----------------------------------------------------------------------
// :
// :lh 2005-05-31
//----------------------------------------------------------------------
#pragma once

#include <array>
#include "boolset.h"
#include "skillstatetype.h"

struct stSkillState;
class CCharacter;
class CEffectObj;
class CSkillStateRecord;

class CChaStateMgr {
public:
	CChaStateMgr(CCharacter* pCha);

	void ChaDestroy(); //
	void ChaDied() {
		ChaDestroy();
	} //

	CBoolSet& Synchro(const stSkillState* pState, int nCount); //

	int GetSkillStateNum() {
		return static_cast<int>(_states.size());
	}

	CSkillStateRecord* GetSkillState(unsigned int nID) {
		return _states[nID]->pInfo;
	}

	bool HasSkillState(unsigned int nID);
	int GetStateLv(unsigned int nID);

	static CSkillStateRecord* GetLastActInfo() {
		return _pLastActInfo;
	}

	static int GetLastShopLevel() {
		return _nShopLevel;
	}

	struct stChaState {
		bool IsDel;

		unsigned char chStateLv;
		CSkillStateRecord* pInfo;
		CEffectObj* pEffect;
		unsigned long lTimeRemaining;
	};

	stChaState GetStateData(unsigned int nID);

private:
	CCharacter* _pCha;


	void _ResetStates();

	std::array<stChaState, SKILL_STATE_MAXID> _sChaState{};
	std::vector<stChaState*> _states{};

	static CSkillStateRecord* _pLastActInfo; //
	static int _nShopLevel; //
};


inline bool CChaStateMgr::HasSkillState(unsigned int nID) {
	return nID < SKILL_STATE_MAXID && _sChaState[nID].chStateLv;
}

inline int CChaStateMgr::GetStateLv(unsigned int nID) {
	return HasSkillState(nID) ? _sChaState[nID].chStateLv : 0;
}

inline CChaStateMgr::stChaState CChaStateMgr::GetStateData(unsigned int nID) {
	return _sChaState.at(nID);
}
