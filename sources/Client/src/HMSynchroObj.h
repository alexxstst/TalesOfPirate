#pragma once

class CSynchroManage;
class CServerHarm;
class CCharacter;

// 
class CStateSynchro {
	friend class CSynchroManage;

public:
	CStateSynchro();
	virtual ~CStateSynchro();

	// ()
	virtual CStateSynchro* Gouge(float fRate) {
		return NULL;
	}

	void Exec();
	void Exec(DWORD time);

	bool GetIsExec() {
		return _isExec;
	}

	void SetServerHarm(CServerHarm* pHarm) {
		_pServerHarm = pHarm;
	}

	CServerHarm* GetServerHarm() {
		return _pServerHarm;
	}

	void Reset() {
		_isExec = false;
	}

	virtual CCharacter* GetHarmCha() = 0;

	virtual std::string_view GetClassName() {
		return "CStateSynchro";
	}

protected:
	virtual void _Exec() {
	}

	bool _isExec;

	CServerHarm* _pServerHarm;

private:
	int _nID; // CSynchroManage
	DWORD _dwExecTime; // 
	DWORD _dwCreateTime;
};

// 
class CSynchroManage {
	friend class CStateSynchro;

public:
	CSynchroManage();
	~CSynchroManage();

	void FrameMove(DWORD dwTime);
	void Reset();

public:
	static CSynchroManage* I() {
		return _pInstance;
	}

private:
	void _CollectEmpty(); // 

private:
	int _AddState(CStateSynchro* pState);
	bool _DelState(CStateSynchro* pState);

private:
	enum {
		MAX_SIZE = 4096,
		ERROR_ID = -1,
	};

	CStateSynchro* _All[MAX_SIZE];
	DWORD _dwHead, _dwTail; // 
	DWORD _nSynchroNum; // 

private:
	static CSynchroManage* _pInstance;
};

// 
inline void CStateSynchro::Exec() {
	_isExec = true;
}

inline void CStateSynchro::Exec(DWORD time) {
	if (time < _dwExecTime) {
		_dwExecTime = time;
	}
}
