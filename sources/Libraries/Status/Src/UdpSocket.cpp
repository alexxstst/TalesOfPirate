#include "udpsocket.h"

#pragma comment( lib, "Ws2_32.lib" )
//---------------------------------------------------------------------------
// class CUdpSocket
//---------------------------------------------------------------------------
CUdpSocket::CUdpSocket()
: m_Socket(INVALID_SOCKET)
{
}

CUdpSocket::~CUdpSocket()
{
	Close();
}

bool CUdpSocket::IsCreate()
{
	return m_Socket != INVALID_SOCKET;
}

void CUdpSocket::Close()
{
	if( !IsCreate() ) return;

	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
	WSACleanup();
}

bool CUdpSocket::RegeditReadEvent( HWND hWnd, DWORD dwMessage )
{
	//FD_READ  
	//FD_WRITE  
	//FD_OOB OOB 
	//FD_ACCEPT  
	//FD_CONNECT  
	//FD_CLOSE  
	//FD_QOS (QoS) 
	//FD_GROUP_QOS  
	//FD_ROUTING_INTERFACE_CHANGE  
	//FD_ADDRESS_LIST_CHANGE  

	if (WSAAsyncSelect(m_Socket, hWnd, dwMessage, FD_READ) == SOCKET_ERROR)
	{
		return true;
	}
	return false;
}

bool CUdpSocket::OnNetworkEvent( WPARAM wParam, LPARAM lParam )
{
	// (SOCKET)wParam
    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_READ:
        // 
        break;
    case FD_WRITE:
        break;
    case FD_OOB:
        break;
    case FD_ACCEPT:
        break;
    case FD_CONNECT:
        break;
    case FD_CLOSE:
        break;
    }
    return true;
}

//---------------------------------------------------------------------------
// class CUdpServer
//---------------------------------------------------------------------------
CUdpServer::CUdpServer()
{
}

bool CUdpServer::Init( int nPort )
{
	Close();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0)
	{
		WSACleanup();
		return false;
	}

	m_Socket = INVALID_SOCKET;
	if ((m_Socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{ 
		return false;
	}

	SOCKADDR_IN m_saAddr;
	m_nPort = nPort;							// 
	ZeroMemory(&m_saAddr, sizeof(m_saAddr));
	m_saAddr.sin_family      = AF_INET;
	m_saAddr.sin_port        = htons(m_nPort);  // 0
	m_saAddr.sin_addr.s_addr = INADDR_ANY;      // IP
	if (bind(m_Socket, (LPSOCKADDR) &m_saAddr, sizeof(m_saAddr)) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CUdpServer::Read( ReceiveEvent pEvent, void* pParam )
{
    int nBytesRead;
    int nBufferLength;
    int nEnd;
    int nSpaceRemaining;
	char chIncomingDataBuffer[4096];
    SOCKADDR_IN m_saFromAddr;
    int nLenght = sizeof(m_saFromAddr);
    ZeroMemory(&m_saFromAddr, sizeof(SOCKADDR_IN));
    nEnd = 0;
    nBufferLength = sizeof(chIncomingDataBuffer);
    nSpaceRemaining = sizeof(chIncomingDataBuffer);
    nSpaceRemaining -= nEnd;

    nBytesRead = recvfrom(m_Socket, (LPSTR) (chIncomingDataBuffer + nEnd), nSpaceRemaining, 0, (LPSOCKADDR) &m_saFromAddr, &nLenght);
    nEnd += nBytesRead;
    if (nBytesRead == SOCKET_ERROR)
    {
        return false;
    }

    // IPinet_ntoa(m_saFromAddr.sin_addr);
    // ntohs(m_saFromAddr.sin_port);
    chIncomingDataBuffer[nEnd] = '\0';
    if (lstrlen(chIncomingDataBuffer) != 0) 
    {
		if( pEvent )
		{
			pEvent( this, inet_ntoa(m_saFromAddr.sin_addr), ntohs(m_saFromAddr.sin_port), chIncomingDataBuffer, nEnd, pParam );
		}
    }
    return true;
}

bool CUdpServer::Send(const char* szIP, unsigned int nPort, const char* pSendData, unsigned int nLength)
{
    DWORD        dwIP;
    SOCKADDR_IN  saAddr;
    if (m_Socket == INVALID_SOCKET)
    {
        return false;
    }
    if ((dwIP = inet_addr(szIP)) == INADDR_NONE)
    {
        return false;
    }
    saAddr.sin_family      = AF_INET;
    saAddr.sin_port        = htons(nPort);
    saAddr.sin_addr.s_addr = dwIP;
    if (sendto(m_Socket, pSendData, nLength, 0, (LPSOCKADDR) &saAddr, sizeof(saAddr)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
// class CUdpClient
//---------------------------------------------------------------------------
CUdpClient::CUdpClient()
{
	m_Socket	= INVALID_SOCKET;
	m_nAddrLen	= sizeof(m_ServerAddr);
	memset( &m_ServerAddr, 0, sizeof(m_ServerAddr) );
}

bool CUdpClient::Init(const char* szIP, unsigned int nPort, int nReadTimeOut)
{
	Close();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0)
	{
		WSACleanup();
		return false;
	}

	m_ServerAddr.sin_family=AF_INET; 
	m_ServerAddr.sin_port=htons(nPort);				// server 
	m_ServerAddr.sin_addr.s_addr=inet_addr(szIP);	// server  
	m_Socket = socket(AF_INET,SOCK_DGRAM,0); 
	if( m_Socket == INVALID_SOCKET )
		return false;

	if (setsockopt( m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nReadTimeOut, sizeof(nReadTimeOut)) != 0)
	{
		return false;
	}

	return true;
}

bool CUdpClient::Send(const char* pSendData, unsigned int nLength )
{
	if (sendto(m_Socket,pSendData,nLength,0,(struct sockaddr*)&m_ServerAddr,m_nAddrLen)!=SOCKET_ERROR) 
	{ 
		return true;
	}
	return false;
}

bool CUdpClient::Read(char* pReadBuf, unsigned int nReadLen)
{
	if (recvfrom(m_Socket,pReadBuf,nReadLen,0,(struct sockaddr*)&m_ServerAddr,&m_nAddrLen) != SOCKET_ERROR) 
	{
		return true;
	}
	return false;
}


