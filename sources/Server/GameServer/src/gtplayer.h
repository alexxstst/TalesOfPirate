/********************************************************************
	created:	2005/02/28
	created:	28:2:2005   9:23
	filename: 	sources\Server\GameServer\src\gtplayer.h
	file path:	\sources\Server\GameServer\src
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
	uplayer();
	uplayer(char const* gt_name, std::int64_t gt_addr, DWORD atorID);

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
	void          SetGate(GateServer* gt);
	GateServer*   GetGate() const;

	void          SetGateAddr(unsigned long gt_addr);
	unsigned long GetGateAddr() const;

	void          SetDBChaId(DWORD dwDBChaId);
	DWORD         GetDBChaId(void);

	virtual void  OnLogin();
	virtual void  OnLogoff();

private:
	uplayer ply;
};


#endif
