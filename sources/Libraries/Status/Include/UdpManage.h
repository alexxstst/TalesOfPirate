#pragma once

#include "udpsocket.h"

// 

class CUdpManage;
typedef void (*ReceiveManageEvent) ( CUdpManage* pManage, CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len );

class CUdpManage
{
public:
	CUdpManage();
	~CUdpManage();
	
	bool			Init( int nPort, ReceiveManageEvent evtReceive );
	void			Exit();

private:
	static DWORD WINAPI ListenThreadProc( LPVOID lpParameter ){
		((CUdpManage*)lpParameter)->Run();
		return 0;
	}
	static  void	ReceiveEvent( CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len, void* pParam );

	void			Run();

private:
	int					_nPort{0};
	bool				_IsRun{true};
	ReceiveManageEvent	_evtReceive{ nullptr };
	HANDLE				_hThread{};
	CUdpServer			_Socket;

};
