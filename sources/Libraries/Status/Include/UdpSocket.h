#pragma once

#include <winsock.h>

class CUdpSocket
{
public:
	CUdpSocket();
	virtual ~CUdpSocket();
	virtual void Close();

	bool	IsCreate();			// 

	bool	RegeditReadEvent( HWND hWnd, DWORD dwMessage );
	bool	OnNetworkEvent( WPARAM wParam, LPARAM lParam );

	SOCKET	GetSocket()		{ return m_Socket;		}

protected:
	SOCKET		m_Socket;

};

class CUdpServer;
typedef void (*ReceiveEvent) ( CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len, void* pParam );

class CUdpServer : public CUdpSocket
{
public:
	CUdpServer();

	bool	Init( int nPort );

	bool	Read( ReceiveEvent pEvent, void* pParam=NULL );
	bool	Send( const char* szIP, unsigned int nPort, const char* pSendData, unsigned int nLength );

protected:
	unsigned int	m_nPort{};

};

class CUdpClient : public CUdpSocket
{
public:
	CUdpClient();

	bool	Init( const char* szIP, unsigned int nPort, int nReadTimeOut );

	bool	Send( const char* pSendData, unsigned int nLength );
	bool	Read( char* pReadBuf, unsigned int nReadLen );

protected:
	sockaddr_in		m_ServerAddr;
	int				m_nAddrLen;

};
