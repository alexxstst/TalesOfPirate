#pragma once

class CSceneItem;
class CCharacter;
class CWorldScene;
class CSkillRecord;
class CEvent;
class CSceneNode;
class CAutoAttack;

class CMouseDown {
public:
	CMouseDown();
	~CMouseDown();

	void Init(CWorldScene* pScene) {
		_pScene = pScene;
		Reset();
	}

	void Reset();

	void Start() {
		_eLastType = eLastNone;
	}

	void CheckWalkContinue();
	void MouseButtonDown();

	void FrameMove();

	// 
	void SetPickItem(CSceneItem* pItem, DWORD dwTime);
	void SetAttackCha(CSkillRecord* pSkill, CCharacter* pCha, DWORD dwTime);

	void SetNpc(CCharacter* pCha);
	void SetEvent(CSceneNode* pNode, CEvent* pEvent);
	void SetAreaSkill(CSkillRecord* pSkill);
	void SetMove();
	void SetDummyObj(int x, int y, int h, int t_angle, int action);
	void SetFollow(CCharacter* pCha);

	// 
	bool ActMove(CCharacter* pCha, int nScrX, int nScrY, bool isAdd = false, bool isLine = false,
				 bool IsCompart = true);
	bool ActAttackCha(CCharacter* pMain, CSkillRecord* pSkill, CCharacter* pTarget, bool isTrace = true,
					  bool IsForce = false, bool IsCompart = true);
	bool ActTalkNpc(CCharacter* pCha, CCharacter* pNpc);
	bool ActPickItem(CCharacter* pCha, CSceneItem* pItem, bool IsMove = true);
	bool ActAttackArea(CCharacter* pCha, CSkillRecord* pSkill, int nScrX, int nScrY, bool IsForce = false,
					   bool IsCompart = true);
	bool ActEvent(CCharacter* pCha, CSceneNode* pNode, CEvent* pEvent);
	bool ActDummyObj(CCharacter* pCha, int x, int y, int h, int t_angle, int action); // 

	// 
	bool ActCha(CCharacter* pCha, CCharacter* pTarget);

	// 
	bool ActShop(CCharacter* pCha, CCharacter* pTarget);

	bool PickItem(CCharacter* pMain);

	// 
	bool ActBackMove(CCharacter* pCha, int nScrX, int nScrY, bool isAdd = false);

	CAutoAttack* GetAutoAttack() {
		return _pAutoAttack;
	}

	void SetIsEnabled(bool v) {
		_IsEnabled = v;
	}

private:
	bool _AttackCha(CCharacter* pMain, CSkillRecord* pSkill, CCharacter* pTarget);
	bool _AttackArea(CCharacter* pMain, CSkillRecord* pSkill, int nScrX, int nScrY);

	static BOOL CALLBACK _EnumWindowsProc(HWND hWnd, LPARAM lParam);

private:
	CWorldScene* _pScene;

	//
	enum eLastType {
		eLastNone,
		eLastAttack,
		eLastPick,
		eLastEvent,
		eLastNpc,
		eLastArea,
		eLastMove,
		eLastDummy, // 
		eLastFollow,
	};

	eLastType _eLastType;

	CSkillRecord* _pAttackSkill;
	CCharacter* _pAttackCha;
	DWORD _dwAttackTime;

	CSceneItem* _pPickItem;
	DWORD _dwPickTime;

	CEvent* _pEvent;
	CSceneNode* _pEventNode;

	CCharacter* _pNpcCha;

	CSkillRecord* _pAreaSkill;

	CCharacter* _pFollow;

	int _nDummyX;
	int _nDummyY;
	int _nDummyH;
	int _nDummyAngle;
	int _nDummyAction;

	CAutoAttack* _pAutoAttack;
	bool _IsEnabled;
};


inline void CMouseDown::SetAttackCha(CSkillRecord* pSkill, CCharacter* pCha, DWORD dwTime) {
	_eLastType = eLastAttack;
	_pAttackSkill = pSkill;
	_pAttackCha = pCha;

	_dwAttackTime = dwTime;
}

inline void CMouseDown::SetNpc(CCharacter* pCha) {
	_eLastType = eLastNpc;
	_pNpcCha = pCha;
}

inline void CMouseDown::SetEvent(CSceneNode* pNode, CEvent* pEvent) {
	_eLastType = eLastEvent;
	_pEvent = pEvent;
	_pEventNode = pNode;
}

inline void CMouseDown::SetPickItem(CSceneItem* pItem, DWORD dwTime) {
	_eLastType = eLastPick;
	_pPickItem = pItem;

	_dwPickTime = dwTime;
}

inline void CMouseDown::SetAreaSkill(CSkillRecord* pSkill) {
	_eLastType = eLastArea;
	_pAreaSkill = pSkill;
}

inline void CMouseDown::SetMove() {
	_eLastType = eLastMove;
}

inline void CMouseDown::SetDummyObj(int x, int y, int h, int t_angle, int action) {
	_eLastType = eLastDummy;

	_nDummyX = x;
	_nDummyY = y;
	_nDummyH = h;
	_nDummyAngle = t_angle;
	_nDummyAction = action;
}

inline void CMouseDown::SetFollow(CCharacter* pCha) {
	_eLastType = eLastFollow;

	_pFollow = pCha;
}
