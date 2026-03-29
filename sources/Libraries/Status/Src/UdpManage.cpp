#include "udpmanage.h"
#include <windows.h>

CUdpManage::CUdpManage() {}

CUdpManage::~CUdpManage()
{
	Exit();
}

bool CUdpManage::Init( int nPort, ReceiveManageEvent evtReceive )
{
	_evtReceive = evtReceive;
	_nPort = nPort;

	DWORD dwThread = 0;
	_hThread = CreateThread( NULL, 0, ListenThreadProc, this, 0, &dwThread);
	if( _hThread==NULL ) return false;

	SetThreadPriority( _hThread, THREAD_PRIORITY_LOWEST );
	return true;
}

void CUdpManage::Run()
{
	_Socket.Init( _nPort );

	// 
	int n = 1024;
	setsockopt( _Socket.GetSocket(), SOL_SOCKET, SO_RCVBUF, (char*)&n, sizeof(n) );

	int i = 0;
	while( _IsRun )
	{
		_Socket.Read( ReceiveEvent, this );
	}
	_Socket.Close();
}

void CUdpManage::ReceiveEvent( CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len, void* pParam )
{
	CUdpManage* pThis = ((CUdpManage*)pParam);
	if( pThis->_evtReceive ) pThis->_evtReceive( pThis, pUdpServer, szClientIP, nClientPort, pData, len );
}

void CUdpManage::Exit()
{
	_IsRun = false;
    if( _hThread )
    {
        if( WAIT_TIMEOUT == WaitForSingleObject( _hThread, 15000 ) ) 
		{
            TerminateThread( _hThread, -1 );
        }

        CloseHandle( _hThread );
		_hThread = NULL;
	}
	_Socket.Close();
}
