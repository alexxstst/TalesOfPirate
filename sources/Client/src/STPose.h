#pragma once
#include "STStateObj.h"

// 
class CPoseState : public CActionState {
public:
	CPoseState(CActor* p);

	virtual const char* GetExplain() {
		return "CPoseState";
	}

	virtual void BeforeNewState() {
		PopState();
	}

	virtual void ActionEnd(DWORD pose_id);

	virtual bool IsKeepPose() {
		return _isKeepPose;
	}

	virtual void FrameMove() {
	}

	virtual void Cancel() {
	}

	void SetPose(int nPos) {
		_nPose = nPos;
	}

	void SetKeepPose(bool v) {
		_isKeepPose = v;
	}

protected:
	virtual bool _Start();

	virtual bool _IsAllowCancel() {
		return false;
	}

protected:
	int _nPose;
	bool _isKeepPose;
};

// 
class CInsertState : public CActionState {
public:
	CInsertState(CActor* p);
	~CInsertState();

	virtual const char* GetExplain() {
		return "CInsertState";
	}

	virtual void Cancel();

	void SetAngle(int v) {
		_nAngle = v;
		_eAngle = enumAngle;
	}

	void SetFaceTo(int nScrX, int nScrY) {
		_nScrX = nScrX;
		_nScrY = nScrY;
		_eAngle = enumScrPos;
	}

protected:
	virtual bool _Start();

protected:
	enum eAngle {
		enumInit,
		enumAngle,
		enumScrPos,
	};

	eAngle _eAngle;
	int _nAngle;
	int _nScrX, _nScrY;

	bool _IsPlayPose;
	bool _IsFirst;
};

// 
struct stNetUseItem;
struct stNetItemUnfix;

class CEquipState : public CActionState {
public:
	CEquipState(CActor* p);
	~CEquipState();

	virtual const char* GetExplain() {
		return "CEquipState";
	}

	void SetUnfixData(stNetItemUnfix& data);
	void SetUseItemData(stNetUseItem& data);

protected:
	virtual bool _Start();

private:
	enum eType {
		enumInit,
		enumUseItem,
		enumUnfix,
	};

	eType _eType;

	stNetUseItem* _pUseItem;
	stNetItemUnfix* _pUnfix;
};
