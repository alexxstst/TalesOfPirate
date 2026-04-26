#pragma once
#include "STStateObj.h"
#include <queue>
#include "NetProtocol.h"

class CSceneItem;

class CWaitMoveState : public CActionState {
public:
	CWaitMoveState(CActor* p);
	~CWaitMoveState();

	virtual void FrameMove();
	virtual void PushPoint(int x, int y);

	virtual const char* GetExplain() {
		return "CWaitMoveState";
	}

	virtual void Cancel() {
	}

	CMoveList* GetMoveList() {
		return &_cMoveList;
	}

	void Clear() {
		_cMoveList.Clear();
	}

	void ChaRun();

	static void SetPreMoveTime(long time);

	static int GetPreMoveTime() {
		return _ulPreMoveTime;
	}

	static float GetMoveRate() {
		return _fMoveRate;
	}

	static DWORD GetPreMoveDis() {
		return _dwPreMoveDis;
	}

	static void RenderPing();
	static void RenderCameraInfo();

protected:
	virtual bool _Start();

	virtual bool _IsAllowCancel() {
		return false;
	}

	bool IsAllowMove();

protected:
	CMoveList _cMoveList;
	CCharacter* _pCha;

	static float _fMoveRate;
	static unsigned long _ulPreMoveTime; // 
	static DWORD _dwPreMoveDis;

private: // Ping
	enum {
		LAST_NUM = 3,
	};

	static int MaxPing;
	static int MinPing;
	static int LastPing[LAST_NUM];
	static int LastPingCnt;
	static int LastPingShow[LAST_NUM]; // ping
	static int nTotalPing;
	static int nTotalPingCnt;
	static float fAveragePing;
};

// 
class CMoveState : public CWaitMoveState {
public:
	CMoveState(CActor* p);

	virtual void MoveEnd(int x, int y, int nState);
	virtual void PushPoint(int x, int y);
	virtual void FrameMove();
	virtual void Cancel();

	virtual const char* GetExplain() {
		return "CMoveState";
	}

	void SetWalkLine(bool v) {
		_IsWalkLine = v;
	}

	void SetMoveTo(int x, int y) {
		_nTargetX = x;
		_nTargetY = y;
	}

	stNetMoveInfo& GetPathInfo() {
		return _stPathInfo;
	}

	static bool IsSameServer(CCharacter* pCha);

protected:
	virtual bool _Start();
	virtual void _End();
	virtual bool _IsAllowCancel();

protected:
	void WriteInfo(S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info);
	void _SynchroServer();

protected:
	int _nTargetX, _nTargetY;

	stNetMoveInfo _stPathInfo; // 
	bool _IsWalkLine; // 
};


// 
class CServerMoveState : public CMoveState {
public:
	CServerMoveState(CActor* p) : CMoveState(p), _IsClientMove(true) {
	}

	virtual const char* GetExplain() {
		return "CServerMoveState";
	}

	virtual void Cancel();
	virtual void FrameMove();
	virtual void MoveEnd(int x, int y, int nState);
	virtual void PushPoint(int x, int y);

	void PreMove();

	static DWORD GetLastTime() {
		return _dwLastMoveTime;
	}

protected:
	virtual bool _Start();
	virtual void _End();

	int _nLocalTargetX, _nLocalTargetY;

	static DWORD _dwLastMoveTime;
	static DWORD _dwLastEndTime; // 

	bool _IsClientMove;
};

// ,
class CBackMoveState : public CMoveState {
public:
	CBackMoveState(CActor* p) : CMoveState(p) {
	}

	virtual void FrameMove();

protected:
	virtual bool _Start();
};

// 
class COneMoveState : public CWaitMoveState {
public:
	COneMoveState(CActor* p);

	virtual void Cancel();
	virtual void FrameMove();
	virtual void PushPoint(int x, int y);
	virtual void MoveEnd(int x, int y, int nState);

	virtual const char* GetExplain() {
		return "COneMoveState";
	}

	void SetMoveTo(int x, int y) {
		_nStartX = x;
		_nStartY = y;
	}

	void SetWalkLine(bool v) {
		_IsStartWalkLine = v;
	}

	bool ContinueMove(int nTargetX, int nTargetY, bool isWalkLine, bool IsCheckTime = false);
	bool AddPath(int x, int y, bool IsWalkLine);
	bool IsSameServerPos(int x, int y);

	void SetRate(float r) {
		_fRate = r;
	}

	float GetRate() {
		return _fRate;
	}

	// 
	// :,,()
	static void CompartMoveList(CMoveList& outlist, S_BVECTOR<D3DXVECTOR3>& path, int length);

protected:
	virtual bool _Start();
	virtual void _End();

	void WriteInfo(S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info);
	bool SendInfo();
	bool StartMove(int nTargetX, int nTargetY, bool isWalkLine);
	void SynchroPos(int x, int y);

	float RefreshRate(int nLocalLen, int nServerLen);
	int GetSyschroDistance();

private:
	int _nStartX, _nStartY; //   
	bool _IsStartWalkLine; // 

	bool _IsWalkLine;
	int _nTargetX, _nTargetY;

	CMoveList _cLocalList; // 
	int _nLocalDis; // 

	int _nServerDis; // 
	int _nServerStepDis; // 

	int _nMoveCount; // 
	int _nStartCount; // 
	int _nSendCount;
	int _nEndCount; // 	
	bool _IsSendCancel;

	DWORD _dwSendTime;
	DWORD _dwEndTime; // 

	float _fRate;

private:
	struct NeedPath {
		NeedPath(int nX, int nY, bool bLine) : x(nX), y(nY), IsLine(bLine) {
		}

		int x, y;
		bool IsLine;
	};

	typedef std::list<NeedPath> path;
	path _cNeedList; // 

	static CMoveList _List;
};
