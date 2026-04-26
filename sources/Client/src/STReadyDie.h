#pragma once
#include "STStateObj.h"
#include "Actor.h"

class CCharacter;
class CArcTrack;
class CMonsterItem;

class CReadyDieState : public CActionState // 
{
public:
	CReadyDieState(CActor* p);
	~CReadyDieState();

	virtual std::string_view GetExplain() {
		return "CReadyDieState";
	}

	virtual void ActionEnd(DWORD pose_id);
	virtual void FrameMove();
	virtual void ActionFrame(DWORD pose_id, int key_frame);

	void SetState(eActorState s) {
		_state = s;
	}

	void SetAttack(CCharacter* pCha) {
		_pAttack = pCha;
	}

protected:
	virtual bool _Start();

	virtual bool _IsAllowCancel() {
		return false;
	}

	virtual void _End();

private:
	void _Died();

private:
	enum eDieState {
		enumInit,
		enumDie,
		enumFallDown,
	};

	eDieState _eDieState;
	eActorState _state;
	bool _IsActionEnd; // 
	CCharacter* _pCha;
	CCharacter* _pAttack; // 
	bool _isFlyOff;

	int _nFallDownTime;
	bool _IsAlreadyEffect; // 
	int _nDelayTime;

	CArcTrack* _pArcTrack;
};
