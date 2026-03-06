#pragma once
#include <cstdint>

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
		return nullptr;
	}

	void Exec();
	void Exec(std::uint32_t time);

	bool GetIsExec() const {
		return _isExec;
	}

	void SetServerHarm(CServerHarm* pHarm) {
		_pServerHarm = pHarm;
	}

	CServerHarm* GetServerHarm() const {
		return _pServerHarm;
	}

	void Reset() {
		_isExec = false;
	}

	virtual CCharacter* GetHarmCha() = 0;

	virtual const char* GetClassName() {
		return "CStateSynchro";
	}

protected:
	virtual void _Exec() {
	}

	bool _isExec;

	CServerHarm* _pServerHarm;

private:
	int _nID; // CSynchroManage
	std::uint32_t _dwExecTime; //
	std::uint32_t _dwCreateTime;
};

//
class CSynchroManage {
	friend class CStateSynchro;

public:
	CSynchroManage();
	~CSynchroManage();

	void FrameMove(std::uint32_t dwTime);
	void Reset();

	static CSynchroManage* I() {
		return _pInstance;
	}

private:
	void _CollectEmpty(); //
	int _AddState(CStateSynchro* pState);
	bool _DelState(CStateSynchro* pState);

	enum {
		MAX_SIZE = 4096,
		ERROR_ID = -1,
	};

	CStateSynchro* _All[MAX_SIZE]{};
	std::uint32_t _dwHead{}, _dwTail{}; //
	std::uint32_t _nSynchroNum{}; //

	static CSynchroManage* _pInstance;
};

//
inline void CStateSynchro::Exec() {
	_isExec = true;
}

inline void CStateSynchro::Exec(std::uint32_t time) {
	if (time < _dwExecTime) {
		_dwExecTime = time;
	}
}
