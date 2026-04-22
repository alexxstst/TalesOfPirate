// gtplayer.cpp — реализации ранее inline-методов из gtplayer.h.
#include "stdafx.h"
#include "gtplayer.h"
#include "GameServerApp.h"

uplayer::uplayer() = default;

uplayer::uplayer(char const* gt_name, std::int64_t gt_addr, DWORD atorID) {
	Init(gt_name, gt_addr, atorID);
}

void uplayer::Init(char const* gt_name, std::int64_t gt_addr, DWORD atorID) {
	pGate        = g_gmsvr->FindGate(gt_name);
	m_dwDBChaId  = atorID;
	m_ulGateAddr = gt_addr;
}

void          GatePlayer::SetGate(GateServer* gt)         { ply.pGate = gt; }
GateServer*   GatePlayer::GetGate() const                 { return ply.pGate; }

void          GatePlayer::SetGateAddr(unsigned long addr) { ply.m_ulGateAddr = addr; }
unsigned long GatePlayer::GetGateAddr() const             { return ply.m_ulGateAddr; }

void          GatePlayer::SetDBChaId(DWORD dwDBChaId)     { ply.m_dwDBChaId = dwDBChaId; }
DWORD         GatePlayer::GetDBChaId(void)                { return ply.m_dwDBChaId; }

void          GatePlayer::OnLogin()                       { /* virtual hook */ }
void          GatePlayer::OnLogoff()                      { /* virtual hook */ }
