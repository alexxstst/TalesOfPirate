#include "udpclient.h"
#include <windows.h>
#include <algorithm>
#include <functional>
using namespace std;
using namespace client_udp;

namespace client_udp
{
	CRegionMgr* GetRegionMgr()
	{
		static CRegionMgr Mgr;
		return &Mgr;
	}
}

// binary_function
//template <class T>
//class NameIsEqual
//{
//public:
//	NameIsEqual( const char* n ) { strcpy( szName, n ); }
//	bool operator() (const T p) const
//	{
//		return strcmp( p->szName, szName ) == 0;
//	}
//
//private:
//	char szName[32];
//
//};
//
//NameIsEqual<CRegionInfo*> var( szRegion );
//regions::iterator it = find_if( _Regions.begin(), _Regions.end(), bind2nd( NameIsEqual<CRegionInfo>(), var ) );

//template <class T, class LIST>
//T Find ( const char* szName, LIST list )
//{
//	for( LIST::iterator it=list.begin(); it!=list.end(); it++ )
//		if( stricmp( (*it)->szName, szName )==0 )
//			return *it;
//	return NULL;
//}

//---------------------------------------------------------------------------
// class CGateInfo
//---------------------------------------------------------------------------
CGateInfo::CGateInfo( CGroupInfo* pGroup, const char* name ) 
: pOwner(pGroup), nGateNum(-1)
{ 
	strncpy( szName, name, sizeof(szName) );	
}

void CGateInfo::Start()
{
	if( Socket.IsCreate() )		
	{
		return;
	}

	if( !Socket.Init( szName, 1976, 15000 ) )
	{
		return;
	}

	GetRegionMgr()->GateCreateSocketCallBack( this );
}

void CGateInfo::Exit()
{
	if( !Socket.IsCreate() ) return;

	GetRegionMgr()->GateCloseSocketCallBack( this );
	Socket.Close();
}

bool CGateInfo::Read()
{
	if( !Socket.IsCreate() ) return false;

	char szBuf[64] = { 0 };
	if( !Socket.Read( szBuf, sizeof(szBuf) ) ) return false;

	nGateNum = atoi( szBuf );	
	char* pStr = strchr( szBuf, ',' );
	if( pStr )
	{
		pStr++;
		pOwner->nShowMin = atoi( pStr );
		pStr = strchr( pStr, ',' );
		if( pStr )
		{
			pStr++;
			pOwner->nShowMax = atoi( pStr );
		}
	}
	return true;
}

bool CGateInfo::Send()
{
	if( !Socket.IsCreate() ) return false;

	char szBuf[] = { "#" };	
	return Socket.Send( szBuf, (int)strlen(szBuf) );
}

//---------------------------------------------------------------------------
// class CGroupInfo
//---------------------------------------------------------------------------
CGroupInfo::CGroupInfo( CRegionInfo* pRegion, const char* name )				
: pOwner(pRegion), nShowMin(-1), nShowMax(-1)
{ 
	strncpy( szName, name, sizeof(szName) );	
}

CGateInfo* CGroupInfo::Find( const char* Name )
{
	auto it = find_if(Gates.begin(), Gates.end(), [&](CGateInfo* info) {
		return strcmp(Name, info->szName) == 0;
		});
	return it == Gates.end() ? nullptr : *it;
}

void CGroupInfo::Start()
{
	for( gates::iterator it=Gates.begin(); it!=Gates.end(); it++ )
		(*it)->Start();
}

void CGroupInfo::Exit()
{
	for( gates::iterator it=Gates.begin(); it!=Gates.end(); it++ )
		(*it)->Exit();
}

void CGroupInfo::Send()
{
	for( gates::iterator it=Gates.begin(); it!=Gates.end(); it++ )
		(*it)->Send();
}

int	CGroupInfo::GetNum()
{
	int nSum = 0;
	bool IsAdd = false;
	for( gates::iterator it=Gates.begin(); it!=Gates.end(); it++ )
	{
		if( (*it)->nGateNum >= 0 )
		{
			nSum += (*it)->nGateNum;
			IsAdd = true;
		}
	}

	if( !IsAdd ) return 0;
	if( nSum<=nShowMin ) return 1;
	if( nSum<=nShowMax ) return 2;
	return 3;
}

CGateInfo* CGroupInfo::GetMinGate()
{
	if( Gates.empty() ) return NULL;

	CGateInfo* pGate = NULL;
	int nCount = INT_MAX;
	for( gates::iterator it=Gates.begin(); it!=Gates.end(); it++ )
	{
		if( (*it)->nGateNum<0 )
			return NULL;

		if( nCount>(*it)->nGateNum  )
		{
			pGate = (*it);
			nCount = pGate->nGateNum;
		}
	}

	if( !pGate )
	{
		int nGateNo = rand()%(int)Gates.size();
		return Gates[nGateNo];
	}
	return pGate;
}

//---------------------------------------------------------------------------
// class CRegionInfo
//---------------------------------------------------------------------------
CRegionInfo::CRegionInfo( CRegionMgr* Mgr, const char* name )				
: pOwner(Mgr)
{ 
	strncpy( szName, name, sizeof(szName) );	
}

CGroupInfo*	CRegionInfo::Find( const char* Name )
{
	auto it = find_if(Groups.begin(), Groups.end(), [&](CGroupInfo* info) {
		return strcmp(Name, info->szName) == 0;
		});
	return it == Groups.end() ? nullptr : *it;
}

void CRegionInfo::Start()
{
	for( groups::iterator it=Groups.begin(); it!=Groups.end(); it++ )
		(*it)->Start();
}

void CRegionInfo::Exit()
{
	for( groups::iterator it=Groups.begin(); it!=Groups.end(); it++ )
		(*it)->Exit();
}

void CRegionInfo::Send()
{
	for( groups::iterator it=Groups.begin(); it!=Groups.end(); it++ )
		(*it)->Send();
}

//---------------------------------------------------------------------------
// class CRegionMgr
//---------------------------------------------------------------------------
CRegionMgr::CRegionMgr()
: _pRegion(NULL), _hWnd(NULL), evtGateChanteEvent(NULL), _dwLastSendTime(0)
{
	_uNetMessage = WM_USER + 100;
}

void CRegionMgr::Init( HWND hWnd, UINT uRegeditNetMes )
{
	_hWnd = hWnd;
	_uNetMessage = uRegeditNetMes;
}

CRegionInfo* CRegionMgr::Find( const char* Name )
{
	auto it = find_if(_Regions.begin(), _Regions.end(), [&](CRegionInfo* info) {
		return strcmp(Name, info->szName) == 0;
		});
	return it == _Regions.end() ? nullptr : *it;
}

bool CRegionMgr::Add( const char* szRegion, const char* szGroup, const char* szGate )
{
	CRegionInfo* pRegion = Find( szRegion );
	if( !pRegion )
	{
		pRegion = new CRegionInfo( this, szRegion );
		Add( pRegion );
	}


	CGroupInfo* pGroup = pRegion->Find( szGroup );
	if( !pGroup )
	{
		pGroup = new CGroupInfo( pRegion, szGroup );
		pRegion->Add( pGroup );
	}

	CGateInfo* pGate = pGroup->Find( szGate );
	if( !pGate )
	{
		pGate = new CGateInfo( pGroup, szGate );
		pGroup->Add( pGate );
		return true;
	}
	return false;
}

bool CRegionMgr::EnterRegion( const char* szRegion )
{
	if( _pRegion && stricmp( _pRegion->szName, szRegion )==0  )
	{
		return true;
	}

	ExitRegion();

	CRegionInfo* pRegion = Find( szRegion );
	if( pRegion )
	{
		_dwLastSendTime = 0;
		_pRegion = pRegion;
		_pRegion->Start();
		return true;
	}
	return false;
}

int	CRegionMgr::GetGroupNum( const char* szGroup )
{
	if( !_pRegion ) return -1;

	CGroupInfo* pGroup = _pRegion->Find( szGroup );
	if( !pGroup ) return -1;

	return pGroup->GetNum();
}

bool CRegionMgr::ExitRegion()
{
	if( !_pRegion ) return false;

	_pRegion = NULL;
	return true;
}

void CRegionMgr::Exit()
{
	_pRegion = 0;
	_gates.clear();
	for( regions::iterator it=_Regions.begin(); it!=_Regions.end(); it++ )
		(*it)->Exit();
}

bool CRegionMgr::OnMessage( UINT Message, WPARAM wParam, LPARAM lParam )
{
	if( Message!=_uNetMessage )
		return false;

	switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_READ:
		{
			gates::iterator it = _gates.find( (SOCKET)wParam );
			if( it==_gates.end() ) break;
			
			CGateInfo* pGate = it->second;
			if( pGate->Read() && evtGateChanteEvent )
			{
				evtGateChanteEvent( pGate );
			}
		}
        break;
    }
	return true;
}

void CRegionMgr::FrameMove( DWORD dwTime )
{
	if( !_pRegion ) return;

	if( dwTime > _dwLastSendTime )
	{
		_dwLastSendTime = dwTime + 10000;
		_pRegion->Send();
	}
}

void CRegionMgr::GateCreateSocketCallBack( CGateInfo* pGate )
{
	pGate->GetUdp().RegeditReadEvent( _hWnd, _uNetMessage );
	_gates.insert( gates::value_type( pGate->GetUdp().GetSocket(), pGate ) );
}

void CRegionMgr::GateCloseSocketCallBack( CGateInfo* pGate )
{
	gates::iterator it = _gates.find( (SOCKET)pGate->GetUdp().GetSocket() );
	if( it==_gates.end() ) return;

	_gates.erase( it );
}

