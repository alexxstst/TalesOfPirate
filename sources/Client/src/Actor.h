#pragma once
#include "STStateObj.h"
#include "CharacterAction.h"

enum eActorState {
	enumNormal, // 
	enumDied, // 
	enumRemains, // 
};


// 
class CActorDie {
public:
	virtual ~CActorDie() {
	}

	virtual void Exec() {
	}
};

// 
class CSceneItem;

class CMonsterItem : public CActorDie {
public:
	CMonsterItem();
	virtual ~CMonsterItem();

	void Exec();

	void SetCha(CCharacter* pCha) {
		_pCha = pCha;
	}

	void SetItem(CSceneItem* pItem) {
		_pItem = pItem;
	}

	CSceneItem* GetItem() {
		return _pItem;
	}

private:
	CSceneItem* _pItem;
	CCharacter* _pCha;
};

// 
struct stNetNpcMission;

class CMissionTrigger : public CActorDie {
public:
	CMissionTrigger();
	~CMissionTrigger();

	virtual void Exec();

	void SetData(stNetNpcMission& v);

private:
	stNetNpcMission* _pData;
};

class CCharacter;
class CActionState;
class CStateSynchro;
class CSkillRecord;
class CServerHarm;

class CActor {
public:
	CActor(CCharacter* pCha);
	~CActor();

	void InitState();

	CActionState* GetCurState() {
		return _pCurState;
	}

	CCharacter* GetCha() {
		return _pCha;
	}

	eActorState GetState() {
		return _eState;
	}

	void SetState(eActorState v);

	bool IsEmpty() {
		return _statelist.empty();
	}

public:
	void PlayPose(int poseid, bool isKeep = false, bool isSend = false);

	void SetSleep() {
		_nWaitingTime = -1;
	}

public: // CActionState 
	bool SwitchState(CActionState* pState); // ,,
	bool InsertState(CActionState* pState, bool IsFront = false); // ,
	bool AddState(CActionState* pState);

	CActionState* FindStateClass(const type_info& info); // 
	void OverAllState(); // ,

	void CancelState(); // 
	void FrameMove(DWORD dwTimeParam);

	CActionState* GetServerState();
	CActionState* GetServerStateByID(int id);
	CActionState* GetNextState();

	int GetQueueCount() {
		return (int)_statelist.size();
	}

	bool IsEnabled() {
		return GetState() == enumNormal;
	}

	void FailedAction();

public: // 
	void ActionBegin(DWORD pose_id);
	void ActionKeyFrame(DWORD pose_id, int key_frame);
	void ActionEnd(DWORD pose_id);

	void Stop();
	void IdleState();

	void ExecAllNet(); // 

	void ExecDied();
	bool AddDieExec(CActorDie* pDieExec);

	void ClearQueueState();

protected:
	//void            _InitIdle( DWORD pose );

	typedef std::list<CStateSynchro*> synchro;
	void _ExecSynchro(synchro& s);
	void _ClearSynchro(synchro& s);

protected:
	CActionState* _pCurState; // 
	typedef std::list<CActionState*> states;
	states _statelist;

	CCharacter* _pCha; // 

public:
	CServerHarm* CreateHarmMgr();
	CServerHarm* FindHarm(int nFightID);
	CServerHarm* FindHarm(); // 

private:
	typedef std::vector<CServerHarm*> fights;
	fights _fights;

	typedef std::vector<CActorDie*> dies; // 
	dies _dies;

protected:
	int _nWaitingTime; // Wait,Pose
	eActorState _eState;
};

// 
inline void CActor::ActionKeyFrame(DWORD pose_id, int key_frame) {
	if (_pCurState) _pCurState->ActionFrame(pose_id, key_frame);
}

inline void CActor::ActionBegin(DWORD pose_id) {
	if (_pCurState) _pCurState->ActionBegin(pose_id);
}

inline bool CActor::AddDieExec(CActorDie* pDieExec) {
	_dies.push_back(pDieExec);
	return true;
}
