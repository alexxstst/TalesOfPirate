#pragma once

#include "udpsocket.h"
#include <vector>
#include <map>

namespace client_udp
{
enum
{
	GATE_NAME_MAX = 16,
	NAME_MAX = 32,
};

class	CGroupInfo;
class	CRegionInfo;
class	CRegionMgr;

class	CGateInfo
{
public:
	CGateInfo( CGroupInfo* pGroup, const char* name );
	void			Start();			// Socket,
	void			Exit();				// Socket
	CGroupInfo*		GetOwner()	{ return pOwner;			}
	bool			Read();
	bool			Send();
	CUdpClient&		GetUdp()	{ return Socket;			}

	char			szName[GATE_NAME_MAX];
	int				nGateNum;

private:
	CUdpClient		Socket;
	CGroupInfo*		pOwner;

};

class	CGroupInfo
{
public:
	CGroupInfo( CRegionInfo* pRegion, const char* name );
	void			Add( CGateInfo* pGate )		{ if( pGate ) Gates.push_back( pGate );		}
	CGateInfo*		Find( const char* Name );
	CRegionInfo*	GetOwner()	{ return pOwner;		}
	int				GetNum();
	CGateInfo*		GetMinGate();		// Gate

	void			Start();
	void			Exit();
	void			Send();

	char			szName[NAME_MAX];
	int				nShowMin;
	int				nShowMax;

private:
	typedef std::vector<CGateInfo*> gates;
	gates			Gates;
	CRegionInfo*	pOwner;

};

class	CRegionInfo
{
public:
	CRegionInfo( CRegionMgr* Mgr, const char* name );
	CGroupInfo*		Find( const char* Name );
	void			Add( CGroupInfo* pGroup )	{ if( pGroup ) Groups.push_back( pGroup );	}
	void			Start();
	void			Exit();
	void			Send();
	CRegionMgr*		GetOwner()	{ return pOwner;		}

	char			szName[NAME_MAX];

private:
	typedef std::vector<CGroupInfo*> groups;
	groups			Groups;
	CRegionMgr*		pOwner;

};

typedef void (*GateChangeEvent) ( CGateInfo* pGate );

// 
class CRegionMgr
{
public:
	CRegionMgr();
	void	Init( HWND hWnd, UINT uRegeditNetMes );

	bool	Add( const char* szRegion, const char* szGroup, const char* szGate );	// 

	bool	EnterRegion( const char* szRegion );	// 
	int		GetGroupNum( const char* szGroup );		// Group,-1
	bool	ExitRegion();							// Group
	CRegionInfo*	GetActiovRegion()	{ return _pRegion;		}

	void	Exit();

	bool	OnMessage( UINT Message, WPARAM wParam, LPARAM lParam );

	void	FrameMove( DWORD dwTime );

public:		// 
	CRegionInfo*	Find( const char* Name );
	void			Add( CRegionInfo* pRegion )		{ if( pRegion ) _Regions.push_back( pRegion );	}

	void			GateCreateSocketCallBack( CGateInfo* pGate );
	void			GateCloseSocketCallBack( CGateInfo* pGate );

public:
	GateChangeEvent	evtGateChanteEvent;

private:
	typedef std::vector<CRegionInfo*> regions;
	regions			_Regions;			// 

	CRegionInfo*	_pRegion;			// 
	HWND			_hWnd;
	UINT			_uNetMessage;

	typedef std::map<SOCKET, CGateInfo*> gates;
	gates			_gates;				// socketCGateInfo

	DWORD			_dwLastSendTime;

};

extern CRegionMgr* GetRegionMgr();

}
