/********************************************************************
	created:	2005/02/28
	created:	28:2:2005   9:23
	filename: 	d:\project\Server\GameServer\src\gtplayer.h
	file path:	d:\project\Server\GameServer\src
	file base:	gtplayer
	file ext:	h
	author:		claude
	
	purpose:	
*********************************************************************/

#ifndef GTPLAYER_H_
#define GTPLAYER_H_

class GateServer;

// Unique Player structure
struct uplayer {
	uplayer() {
	}

	uplayer(char const* gt_name, std::int64_t gt_addr, DWORD atorID) {
		Init(gt_name, gt_addr, atorID);
	}

	void Init(char const* gt_name, std::int64_t gt_addr, DWORD atorID);

	DWORD m_dwDBChaId{}; // ID

	// Player
	GateServer* pGate{};
	std::int64_t m_ulGateAddr{}; //  GateServer
};

struct GatePlayer {
protected:
	GatePlayer() = default;
	~GatePlayer() = default;

public:
	void SetGate(GateServer* gt) {
		ply.pGate = gt;
	}

	GateServer* GetGate() const {
		return ply.pGate;
	}

	void SetGateAddr(unsigned long gt_addr) {
		ply.m_ulGateAddr = gt_addr;
	}

	unsigned long GetGateAddr() const {
		return ply.m_ulGateAddr;
	}

	void SetDBChaId(DWORD dwDBChaId) {
		ply.m_dwDBChaId = dwDBChaId;
	}

	DWORD GetDBChaId(void) {
		return ply.m_dwDBChaId;
	}

	virtual void OnLogin() {
	}

	virtual void OnLogoff() {
	}

private:
	uplayer ply;
};


#endif
